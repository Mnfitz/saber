#ifndef SABER_EVENTS_EVENTMANAGER_HPP
#define SABER_EVENTS_EVENTMANAGER_HPP

// saber
#include "saber/config.hpp"
#include "saber/exception.hpp"
#include "saber/utility.hpp"
#include "saber/raii/resource_handler.hpp"

// std
#include <algorithm>
#include <any>
#include <atomic>
#include <cstdint>
#include <execution>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <optional>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <vector>

namespace saber {

/// @brief Tag selecting the RAII-returning EventManager::Register overload.
struct UsingRAII {};

/// @brief Controls how an EventManager dispatches matching callbacks.
enum class ThreadingPolicy
{
	/// @brief Invoke callbacks sequentially; concurrent manager access is not supported.
	kSingle,
	/// @brief Invoke matching callbacks in parallel; callback order is unspecified and callback state must be synchronized.
	kParallel
};

namespace events {

class EventManager;
using EventToken = TaggedType<uint64_t, EventManager>;

} // namespace events
} // namespace saber

// --- Deleter specialization: declare only, don't define the body yet ---
namespace saber::raii {

// SABER-76: "Create RAII handler for EventManager::Register"
// An EventToken is a value, not a pointer, so there is nothing for ReferenceHandler
// to hold a `EventToken*` to. Instead we hand ReferenceHandler a custom handle type
// via the nested `pointer` typedef below; ReferenceHandler (really std::unique_ptr)
// then stores that handle *by value*, so the token needs no heap allocation.
template<>
struct ResourceDeleter<events::EventToken>
{
public:
    // A default constructed Deleter has no EventManager, and so does nothing.
    // This is what makes a default constructed (empty) TokenHandler possible.
    constexpr ResourceDeleter() noexcept = default;

    explicit ResourceDeleter(std::weak_ptr<events::EventManager>&& inEventManager) noexcept :
        mWeakManager{std::move(inEventManager)}
    {
    }

    // Declaration only — EventManager is still incomplete here, which is fine
    // because we aren't calling any of its members yet.
    void operator()(events::EventToken& inEventToken) const noexcept;

private:
    std::weak_ptr<events::EventManager> mWeakManager{};
}; // struct ResourceDeleter

} // namespace saber::raii

namespace saber::events {
namespace detail {

template<typename ...Args>
struct EventArgsTraits
{
	using ValueType = std::tuple<Args...>;
	inline static const std::type_index kTypeIndex = typeid(ValueType);
}; // struct EventArgsTraits<>

template<typename T>
struct LambdaTraits : public LambdaTraits<decltype(&T::operator())>
{
	// This primary template is never instantiated;
	// Only used to deduce the argument types of a
	// lambda by forwarding to the specialization for
	// its operator().
}; // struct LambdaTraits<>

template<typename C, typename R, typename A1, typename A2>
struct LambdaTraits<R(C::*)(A1, A2) const> // For most (const) lambdas: `auto l = [/*const*/]() {};` by default "const capture"
{
	using Arg1Type = std::decay_t<A1>;
	using Arg2Type = std::decay_t<A2>;
	using ResultType = std::decay_t<R>;
}; // struct LambdaTraits<const>

template<typename C, typename R, typename A1, typename A2>
struct LambdaTraits<R(C::*)(A1, A2)> // For mutable lambdas: `auto l = [/*mutable*/]() mutable {};`
{
	using Arg1Type = std::decay_t<A1>;
	using Arg2Type = std::decay_t<A2>;
	using ResultType = std::decay_t<R>;
}; // struct LambdaTraits<mutable>

inline EventToken GenerateToken()
{
	static std::atomic<std::uint64_t> sCounter{0};
	return EventToken{sCounter.fetch_add(1, std::memory_order_relaxed)};
}

} // namespace detail

// ------------------------------------------------------------------
#if SABER_PRAGMA(REGION)
#pragma region class EventCallback
#endif

/// @brief Type-erases a callback for one sender and event type so it can be stored and dispatched by an EventManager.
///
/// @details
/// Construct an EventCallback from a callable invocable as `(SenderType&, const EventType&)`. The sender and event
/// types are inferred from the callable's two parameters; they identify the event kind used when the manager routes
/// notifications. The callable's return value is discarded, so `void` is the intended return type. The wrapper owns
/// its stored callable, subject to the callable's own capture semantics. Copying the wrapper copies that callable;
/// moving transfers it. Captured references and raw pointers remain non-owning and must remain valid for every dispatch.
///
/// EventCallback does not expose a public invocation operation. Register it with an EventManager, which supplies
/// the matching sender and event during notification. A parallel EventManager may invoke the same callback
/// concurrently; mutable state accessed by a callback must therefore be synchronized by the callback or its owner.
/// Constructors that bind an object document their distinct ownership and expiration behavior below.
class EventCallback
{
public:
	/// @brief Wraps a free function pointer as an event callback.
	/// @tparam Function Function type pointed to by `inFunction`.
	/// @param inFunction Function to invoke for each matching notification; it must accept the inferred sender and event arguments.
	/// @details The pointer is copied into the wrapper; the function is not owned. Its return value, if any, is ignored.
	template<typename Function>
	EventCallback(Function* inFunction) :
		EventCallback{[=](auto&&... inArgs) { inFunction(inArgs...); }}
	{
		// This space intentionally blank
	}

	/// @brief Wraps a member function bound to a non-owning object pointer.
	/// @tparam Object Type of the object receiving the member-function call.
	/// @tparam Method Member-function pointer type.
	/// @param inObject Object on which to invoke `inMethod`; it must remain alive through every dispatch.
	/// @param inMethod Member function to invoke for each matching notification.
	/// @details The object and method pointers are copied; the object is not owned. The member function must accept
	///          the inferred sender and event arguments. Its return value, if any, is ignored.
	template<typename Object, typename Method>
	EventCallback(Object* inObject, Method inMethod) :
		EventCallback{[=](auto&&... inArgs) { (inObject->*inMethod)(inArgs...); }}
	{
		// This space intentionally blank
	}

	/// @brief Wraps a member function bound to a shared object and takes ownership of the supplied shared pointer.
	/// @tparam Object Type of the object receiving the member-function call.
	/// @tparam Method Member-function pointer type.
	/// @param ioObject Rvalue shared pointer whose ownership is moved into the callback.
	/// @param inMethod Member function to invoke for each matching notification.
	/// @details The callback keeps the object alive as long as this wrapper or a copy retaining the captured shared
	///          pointer exists. The member function must accept the inferred sender and event arguments; its return
	///          value, if any, is ignored.
	template<typename Object, typename Method>
	EventCallback(std::shared_ptr<Object>&& ioObject, Method inMethod) :
		EventCallback{[object = std::move(ioObject), inMethod](auto&&... inArgs) { (object->*inMethod)(inArgs...); }}
	{
		// This space intentionally blank
	}

	/// @brief Wraps a callback gated by the lifetime of a weakly referenced object.
	/// @tparam Object Type of the weakly referenced object.
	/// @tparam MethodOrLambda Member-function pointer or callable type.
	/// @param ioObject Rvalue weak pointer used to check whether the object is alive at dispatch time.
	/// @param ioMethodOrLambda Member function to invoke on the locked object, or a callable to invoke while the
	///        object is alive. The value is forwarded into the wrapper.
	/// @details If the weak pointer has expired, dispatch is skipped. A member function is called on the locked
	///          object; a callable is invoked with the event arguments and does not receive the object itself. Any
	///          return value is ignored. This wrapper does not keep the object alive between notifications.
	template<typename Object, typename MethodOrLambda>
	EventCallback(std::weak_ptr<Object>&& ioObject, MethodOrLambda&& ioMethodOrLambda) :
		EventCallback{[weakObject = std::move(ioObject), methodOrLambda = std::forward<MethodOrLambda>(ioMethodOrLambda)](auto&&... inArgs)
		{
			if (auto object = weakObject.lock())
			{
				constexpr bool isMethod = std::is_member_function_pointer_v<MethodOrLambda>;
				if constexpr (isMethod)
				{
					auto& method = methodOrLambda;
					(object->*method)(inArgs...);
				}
				else // isLambda
				{
					auto& lambda = methodOrLambda;
					lambda(inArgs...);
				}
			}
		}}
	{
		// This space intentionally blank
	}

	/// @brief Wraps a callable whose sender and event argument types determine this callback's event kind.
	/// @tparam Lambda Callable type with a non-overloaded, non-generic call operator accepting the sender and event.
	/// @param ioLambda Callable to store; it is forwarded into the wrapper and may be moved when passed as an rvalue.
	/// @details The callable must be invocable as `(SenderType&, const EventType&)`. Its argument types are deduced
	///          and used for event matching. The callable's return value is ignored; `void` is the intended return
	///          type. Captures follow their normal C++ copy, move, and lifetime semantics.
	template<typename Lambda>
	EventCallback(Lambda&& ioLambda) :
		mAnyCallback{std::forward<Lambda>(ioLambda)}
	{
		// Deduce the argument types of the input ioLambda
		using LambdaType = std::decay_t<Lambda>;
		using AutoDeduceTraits = detail::LambdaTraits<decltype(&LambdaType::operator())>;
		using SenderType = typename AutoDeduceTraits::Arg1Type;
		using EventType = typename AutoDeduceTraits::Arg2Type;

		using ArgsTraits = detail::EventArgsTraits<SenderType, EventType>;

		// Store static function ptr* to routine taht knows how
		// to invoke the lambda using the correct argument types
		mInvoker = +[](std::any& inCallback, const std::any& inArgs)
		{
			using ArgsType = typename ArgsTraits::ValueType;
			const auto& args = std::any_cast<const ArgsType&>(inArgs);
			const auto& [sender, event] = args;

			auto& callback = std::any_cast<Lambda&>(inCallback);
			callback(sender, event);
		};
		// Store "id" that describes the sender/event args suitable for this callback
		mEventKind = ArgsTraits::kTypeIndex;

		static_assert(std::is_invocable_v<Lambda, SenderType&, const EventType&>,
					  "Must be invocable as: ioLambda(SenderType&, const EventType&) -> void");
	}

	/// @brief Gets the type key used by EventManager to match notifications to this callback.
	/// @return The `std::type_index` for the tuple of the deduced sender and event types.
	/// @details The key identifies a sender/event type pair; it does not expose or invoke the stored callable.
	std::type_index GetEventKind() const
	{
		return mEventKind;
	}

private:
	// Only EventManager is allowed to invoke operator(), since it's the
	// only class that knows the correct contained-types to use for `inAnyArgs`
	friend class EventManager;
	void operator()(const std::any& inAnyArgs) const
	{
		mInvoker(mAnyCallback, inAnyArgs);
	}

private:
	using InvokerType = void(*)(std::any&, const std::any&);

	// The original callable stored with type-erasure so many different
	// callable types can be stored in the same container.
	mutable std::any mAnyCallback{};
	InvokerType mInvoker{};
	std::type_index mEventKind{typeid(void)};
}; // class EventCallback<>

#if SABER_PRAGMA(REGION)
#pragma endregion {}
#endif

// ------------------------------------------------------------------
#if SABER_PRAGMA(REGION)
#pragma region class EventManager
#endif

/// @brief Registers event callbacks and dispatches notifications to callbacks of the matching event kind.
///
/// @details
/// An EventManager maintains registrations as state. Each registration has a unique Token that can be used to
/// unregister it, or a TokenHandler that unregisters it automatically when the handler is reset or destroyed.
/// Registering an lvalue EventCallback copies it; registering an rvalue transfers it. Multiple registrations may
/// use copies of the same callback and each registration is dispatched independently. Unregistering a token removes
/// only its registration; a token must belong to this manager and must not already have been unregistered.
///
/// Notify matches the exact sender/event type pair inferred by EventCallback. It dispatches only to registrations
/// present in the notification's callback-list snapshot; changes made during a notification affect later snapshots.
/// With `ThreadingPolicy::kSingle`, dispatch is sequential and the manager is not intended for concurrent access.
/// With `ThreadingPolicy::kParallel`, matching callbacks may run concurrently and without a defined order; callers
/// must synchronize mutable callback state and any objects accessed concurrently. Do not destroy the manager while
/// another thread is using it.
///
/// @par Example
/// @code
/// struct Sender {};
/// struct Changed { int value; };
/// Sender sender{};
/// auto manager = saber::events::EventManager::Make();
/// auto registration = manager->Register(saber::UsingRAII{},
///     saber::events::EventCallback([](Sender&, const Changed& event) {
///         (void)event.value;
///     }));
/// manager->Notify(sender, Changed{42});
/// @endcode
class EventManager :
	public std::enable_shared_from_this<EventManager> // is-a: enable_shared_from_this
{
public:
	/// @brief Registration token type used to identify one callback registration.
	/// @details Tokens are generated uniquely and are valid only for the EventManager that created them.
	using Token = EventToken;

	/// @brief Move-only RAII owner of a registration token.
	/// @details Destroying or resetting a valid handler unregisters its callback. The handler must not outlive its
	///          manager if automatic unregistration is required; if the manager has already been destroyed, cleanup
	///          safely does nothing.
	using TokenHandler = raii::ResourceHandler<Token, raii::ResourceDeleter<Token>>;

public:
	/// @brief Creates a manager with the requested dispatch policy.
	/// @param inPolicy `kSingle` dispatches sequentially; `kParallel` may invoke matching callbacks concurrently.
	/// @return A shared pointer to the newly created manager.
	/// @details The default policy is `ThreadingPolicy::kSingle`. Use shared ownership because RAII registration
	///          handlers keep a weak reference to the manager for automatic unregistration.
	static std::shared_ptr<EventManager> Make(ThreadingPolicy inPolicy = ThreadingPolicy::kSingle);

	/// @brief Destroys the manager and releases its registrations.
	/// @details Outstanding TokenHandlers become harmless when destroyed after the manager; they cannot unregister
	///          from a manager that no longer exists.
	virtual ~EventManager() = default;

public:
	/// @brief Registers a copy of a callback and returns an RAII registration handle.
	/// @param inCallback Callback to copy and register; the original remains unchanged and may be reused.
	/// @return Move-only handler that unregisters this registration when reset or destroyed.
	/// @details Each call creates an independent registration, even when passed the same callback. The handler must
	///          not be discarded if the registration should remain active.
	[[nodiscard]] TokenHandler Register(UsingRAII, const EventCallback& inCallback);

	/// @brief Registers a callback by moving it and returns an RAII registration handle.
	/// @param ioCallback Callback whose stored callable is moved into the manager.
	/// @return Move-only handler that unregisters this registration when reset or destroyed.
	/// @details The source callback is consumed. The handler must not be discarded if the registration should remain active.
	[[nodiscard]] TokenHandler Register(UsingRAII, EventCallback&& ioCallback);

	/// @brief Registers a copy of a callback and returns its token.
	/// @param inCallback Callback to copy and register; the original remains unchanged and may be reused.
	/// @return Token identifying this registration; retain it to unregister the callback.
	/// @details Each call creates an independent registration. Unlike the UsingRAII overload, this registration remains
	///          active until Unregister is called or the manager is destroyed.
	[[nodiscard]] Token Register(const EventCallback& inCallback);

	/// @brief Registers a callback by moving it and returns its token.
	/// @param ioCallback Callback whose stored callable is moved into the manager.
	/// @return Token identifying this registration; retain it to unregister the callback.
	/// @details The source callback is consumed. The registration remains active until Unregister is called or the
	///          manager is destroyed.
	[[nodiscard]] Token Register(EventCallback&& ioCallback);

	/// @brief Removes a callback registration from this manager.
	/// @param inToken Token returned by Register for this manager.
	/// @details The token must identify an active registration owned by this manager. Passing a foreign or already
	///          removed token violates the precondition and triggers the library's requirement failure. A callback
	///          already included in an in-progress notification snapshot may still finish dispatching.
	void Unregister(Token inToken);

	/// @brief Dispatches an event to all registered callbacks with the matching sender and event types.
	/// @tparam SenderType Concrete sender type, deduced from `inSender`.
	/// @tparam EventType Concrete event type, deduced from `inEvent`.
	/// @param inSender Mutable reference to the event sender, passed to callbacks as `SenderType&`.
	/// @param inEvent Event data, passed to callbacks as `const EventType&`.
	/// @details Only registrations with the same inferred sender/event type pair are invoked. The callback collection
	///          is snapshotted for dispatch: registration changes during notification are observed by later calls,
	///          while callbacks already in the current snapshot may still run. Under `kParallel`, matching callbacks
	///          may execute concurrently and in unspecified order. Callback exceptions do not propagate to the caller.
	template<typename SenderType, typename EventType>
	void Notify(SenderType& inSender, const EventType& inEvent);

protected:
	EventManager() = default;

	static void Invoke(const EventCallback& inCallback, const std::any& inAnyArgs);

private:
	/// @brief NVI hook that stores a callback and returns its manager-owned registration token.
	/// @param inEventKind Exact sender/event type key obtained from the callback.
	/// @param ioCallback Callback to move into the manager's registration state.
	/// @return A token unique to this registration and owned by this manager.
	/// @details This private pure virtual is the implementation point behind the public Register overloads. Derived
	///          managers must store the callback together with its event-kind key so OnNotify can later find it, and
	///          must ensure OnUnregister can remove the returned token. The base class handles lvalue copying, rvalue
	///          forwarding, and construction of optional RAII handlers before reaching this hook.
	virtual Token OnRegister(std::type_index inEventKind, EventCallback&& ioCallback) = 0;

	/// @brief NVI hook that removes one previously registered callback from manager state.
	/// @param inToken Token returned by this manager's OnRegister implementation.
	/// @details Implementations must reject tokens that are not active registrations belonging to this manager.
	///          A notification already using a snapshot may still hold and invoke the removed callback until that
	///          dispatch completes; new notification snapshots must not include it.
	virtual void OnUnregister(Token inToken) = 0;

	/// @brief NVI hook that dispatches type-erased event arguments to matching registered callbacks.
	/// @param inEventKind Exact sender/event type key corresponding to `inAnyArgs`.
	/// @param inAnyArgs Type-erased tuple containing a sender reference and a const event reference.
	/// @details Implementations compare `inEventKind` with each registration's key and invoke only matches through
	///          EventManager::Invoke. The erased argument value must remain alive and unmodified for all invocations.
	///          In a parallel manager, accesses to registration state must be synchronized, callbacks may run
	///          concurrently, and exceptions must not escape Notify's public boundary.
	virtual void OnNotify(std::type_index inEventKind, std::any&& inAnyArgs) = 0;

}; // class EventManager

#if SABER_PRAGMA(REGION)
#pragma endregion {}
#endif

namespace detail {

// ------------------------------------------------------------------
#if SABER_PRAGMA(REGION)
#pragma region class EventManagerImpl
#endif

class EventManagerImpl final :
	public EventManager // EventManagerImpl is-a EventManager
{
public:
	~EventManagerImpl() override = default;

private:
	friend class EventManager; // allow Make() to construct it
    EventManagerImpl() = default;

private:
	Token OnRegister(std::type_index inEventKind, EventCallback&& ioCallback) override;
	void OnUnregister(Token inToken) override;
	void OnNotify(std::type_index inEventKind, std::any&& inAnyArgs) override;

private:
	using CallbackList = std::vector<std::tuple<Token, std::type_index, EventCallback>>;
	using CallbackElement = CallbackList::value_type;
	std::shared_ptr<CallbackList> mCallbackList{ std::make_shared<CallbackList>() };

	// Writers call this before changing registrations. A notification snapshot shares the current vector;
	// when that happens, detach the writer onto a copy so the active iteration remains unchanged.
	CallbackList& GetCallbackListOrCopy()
	{
		// This implementation is single-threaded; use_count() is not a synchronization primitive.
		{
			const bool hasActiveSnapshot = (mCallbackList.use_count() > 1);
			if (hasActiveSnapshot)
			{
				mCallbackList = std::make_shared<CallbackList>(*mCallbackList);
			}
		}
		return *mCallbackList;
	}

}; // class EventManagerImpl

// --- inlines ---

inline EventManager::Token
	EventManagerImpl::OnRegister(std::type_index inEventKind, EventCallback&& ioCallback)
{
	// Create a manager-unique identity, then append the owned callback and its routing key to live state.
	Token token = GenerateToken();
	auto& callbackList = GetCallbackListOrCopy();
	callbackList.emplace_back(token, inEventKind, std::move(ioCallback));
	return token;
}

inline void EventManagerImpl::OnUnregister(Token inToken)
{
	// Detach first if a notification is iterating the old list, then locate the unique registration to remove.
	auto& callbackList = GetCallbackListOrCopy();
	auto findToken = [inToken](const CallbackElement& inElement)
	{
		const auto& token = std::get<0>(inElement); // <0> is `Token` in the tuple
		const bool wasFound = (token == inToken);
		return wasFound;
	};

	// There will only ever be one matching token, therefore, we can use std::find_if to
	// find the first matching token and erase it without needing to go through the entire list with std::remove_if
	const auto didFind = std::find_if(callbackList.begin(), callbackList.end(), findToken);

	SABER_REQUIRE(didFind != callbackList.end());

	// REVISIT: Move assign might throw an exception?
	// If so, we might have to do something like this:
	//		std::iter_swap(didFind, std::prev(callbackList.end()));
	//		callbackList.pop_back();

	// Fill the hole with the last element, then pop it. This is O(1) after lookup and intentionally does not preserve order.
	*didFind = std::move(callbackList.back());
	callbackList.pop_back();
}

inline void EventManagerImpl::OnNotify(std::type_index inEventKind, std::any&& inAnyArgs)
{
	// Keep the current vector alive for this dispatch. Reentrant mutations will detach to a copy, so this
	// notification sees a fixed registration set even if a callback registers or unregisters callbacks.
	auto snapshot = mCallbackList;
	auto findAllCallbacks = [&inAnyArgs, &inEventKind](const auto& inElement) -> void
	{
		const auto& [token, kind, callback] = inElement;
		if (inEventKind == kind)
		{
			// Stop exceptions from propogating to top level with try/catch
			try
			{
				EventManager::Invoke(callback, inAnyArgs); // Reference operator(): invoke the callback
			}
			catch (...)
			{
				// SABER_ASSERT drops you into debugger for debug builds
				// but does nothing for release builds
				SABER_ASSERT(!"Unexpected exception");
			}
		}
	};

	// Visit the stable snapshot. Only matching event kinds are invoked; its shared ownership is released
	// automatically on return, allowing an obsolete pre-mutation vector to be reclaimed.
	std::for_each(snapshot->begin(), snapshot->end(), findAllCallbacks);
}

#if SABER_PRAGMA(REGION)
#pragma endregion {}
#endif

// ------------------------------------------------------------------
#if SABER_PRAGMA(REGION)
#pragma region class EventManagerParallelImpl
#endif

// Parallel implementation. The mutex protects publication and mutation of the current callback vector, but is
// deliberately not held while user callbacks run. Notifications retain immutable snapshots; writers use
// copy-on-write when any notification is still reading the published vector.
class EventManagerParallelImpl final :
	public EventManager // is-a: EventManager
{
public:
	~EventManagerParallelImpl() override = default;

private:
	friend class EventManager; // allow Make() to construct it
    EventManagerParallelImpl() = default;
	auto& GetCallbackListOrCopy();

private:
	Token OnRegister(std::type_index inEventKind, EventCallback&& ioCallback) override;
	void OnUnregister(Token inToken) override;
	void OnNotify(std::type_index inEventKind, std::any&& inAnyArgs) override;

private:
	using CallbackList = std::vector<std::tuple<Token, std::type_index, EventCallback>>;
	using CallbackElement = CallbackList::value_type;
	std::shared_ptr<CallbackList> mCallbackList{ std::make_shared<CallbackList>() };
	std::mutex mMutex;
}; // class EventManagerParallelImpl

// --- inlines ---

inline auto& EventManagerParallelImpl::GetCallbackListOrCopy()
{
	// The caller holds mMutex. If a notifier retains the current vector, publish a private copy before writing;
	// readers then continue safely against the unchanged snapshot while this thread updates the new vector.
	{
		const bool hasActiveSnapshots = (mCallbackList.use_count() > 1);
		if (hasActiveSnapshots)
		{
			mCallbackList = std::make_shared<CallbackList>(*mCallbackList);
		}
	}
	return *mCallbackList;
}

inline EventManager::Token
	EventManagerParallelImpl::OnRegister(std::type_index inArgsType, EventCallback&& ioCallback)
{
	Token token;
	{
		// Serialize token creation and publication of the updated registration vector.
		std::lock_guard<std::mutex> lock(mMutex);
		
		token = GenerateToken();
		auto& callbackList = GetCallbackListOrCopy();
		callbackList.emplace_back(token, inArgsType, std::move(ioCallback));
	} 
	return token;
}

inline void EventManagerParallelImpl::OnUnregister(Token inToken)
{
	{
		// Serialize lookup and removal; copy-on-write preserves callback lists already captured by notifiers.
		std::lock_guard<std::mutex> lock(mMutex);
		
		auto& callbackList = GetCallbackListOrCopy();
		auto element = std::find_if(callbackList.begin(), callbackList.end(),
			[inToken](const CallbackElement& inElement)
			{ 
				const auto& [token, typeIndex, callback] = inElement;
				const bool wasFound = (token == inToken);
				return wasFound; 
			});
		SABER_REQUIRE(element != callbackList.end());

		// Fancy swap delete; overwrite iterator with last element in list then remove last element
		*element = std::move(callbackList.back());
		callbackList.pop_back();
	}
}

inline void EventManagerParallelImpl::OnNotify(std::type_index inEventKind, std::any&& inAnyArgs)
{
	std::shared_ptr<CallbackList> snapshot;
	{
		// Capture a consistent list while synchronized, then release the mutex before executing user code.
		std::lock_guard<std::mutex> lock(mMutex);
		snapshot = mCallbackList;
	}

	// The snapshot is immutable for this dispatch. Different matching callbacks may run concurrently, so callback
	// captures and externally shared state must be safe for concurrent invocation.
	auto invokeMatchingCallbacks = [inEventKind, &inAnyArgs](const auto& inElement)
	{
		const auto& [token, kind, callback] = inElement;
		if (inEventKind == kind)
		{
			try {
				EventManager::Invoke(callback, inAnyArgs);
			}
			catch (...)
			{
				SABER_ASSERT(!"Unexpected exception");
			}
		}
	};
	std::for_each(std::execution::par, snapshot->begin(), snapshot->end(), invokeMatchingCallbacks);
}

#if SABER_PRAGMA(REGION)
#pragma endregion {}
#endif

} // namespace detail

// ------------------------------------------------------------------
#if SABER_PRAGMA(REGION)
#pragma region EventManager inlines
#endif

inline /*static*/ std::shared_ptr<EventManager>
	EventManager::Make(ThreadingPolicy inPolicy)
{
	std::shared_ptr<EventManager> result{};
	switch (inPolicy)
	{
	case ThreadingPolicy::kParallel:
		result.reset(new detail::EventManagerParallelImpl());
		break;
	case ThreadingPolicy::kSingle:
		result.reset(new detail::EventManagerImpl());
		break;
	//default:
		// We don't use default here because there are only 2 enum values, 
		// and we want the compiler to helpfully alert us if someone ever adds a 3rd enum
	} // switch (inPolicy)

	return result;
}

inline EventManager::TokenHandler
	EventManager::Register(UsingRAII, const EventCallback& inCallback)
{
	return Register(UsingRAII{}, EventCallback{inCallback});
}

inline EventManager::TokenHandler
	EventManager::Register(UsingRAII, EventCallback&& ioCallback)
{
	// SABER-76: Token is a value and not a pointer, so TokenHandler stores it inline in
	// TokenDeleter::pointer; there is no heap allocation here. TokenDeleter is stateful
	// (it must know which EventManager to Unregister() from), so it is passed explicitly.
	using TokenDeleter = raii::ResourceDeleter<Token>;
	return TokenHandler{Register(std::move(ioCallback)), TokenDeleter{weak_from_this()}}; // RAII handler for the token
}

inline EventManager::Token
	EventManager::Register(const EventCallback& inCallback)
{
	return Register(EventCallback{inCallback});
}

inline EventManager::Token
	EventManager::Register(EventCallback&& ioCallback)
{
	// TRICKY: virtuals are unable to accept template types, so use typeid and std::type_index
	// to allow us to find callbacks of a certain type from the callback list
	const auto kind = ioCallback.GetEventKind();
	return OnRegister(kind, std::move(ioCallback));
}

inline void EventManager::Unregister(Token inToken)
{
	OnUnregister(inToken);
}

template<typename SenderType, typename EventType>
inline void EventManager::Notify(SenderType& inSender, const EventType& inEvent)
{
	using ArgsTraits = detail::EventArgsTraits<SenderType, EventType>;
	using ArgsType = typename ArgsTraits::ValueType;

	const auto kEventKind = ArgsTraits::kTypeIndex;
	ArgsType args{inSender, inEvent};
	OnNotify(kEventKind, std::any{args}); // Explicit copy
}

inline /*static*/ void EventManager::Invoke(const EventCallback& inCallback, const std::any& inAnyArgs)
{
	inCallback(inAnyArgs);
}

#if SABER_PRAGMA(REGION)
#pragma endregion {}
#endif

} // namespace saber::events

namespace saber::raii {

inline void ResourceDeleter<events::EventToken>::operator()(events::EventToken& inEventToken) const noexcept
{
	// Check whether the token and Event Manager exists
	if (inEventToken)
	{
		auto eventManager = mWeakManager.lock();
		if (eventManager)
		{
			eventManager->Unregister(inEventToken); // OK now — EventManager is complete
		}
	}
}

} // namespace saber::raii

#endif // SABER_EVENTS_EVENTMANAGER_HPP