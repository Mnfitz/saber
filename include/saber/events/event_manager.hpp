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


// --- forward declarations, in the real namespace ---
namespace saber {

// Marker type for RAII usage
struct UsingRAII {};

enum class ThreadingPolicy
{
	kSingle,
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
struct LambdaTraits<R(C::*)(A1, A2) const> // For most lambdas: `auto l = [/*const*/]() {};` by default "const capture"
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

class EventCallback
{
public:
	template<typename Function>
	EventCallback(Function* inFunction) :
		EventCallback{[=](auto&&... inArgs) { inFunction(inArgs...); }}
	{
		// This space intentionally blank
	}

	template<typename Object, typename Method>
	EventCallback(Object* inObject, Method inMethod) :
		EventCallback{[=](auto&&... inArgs) { (inObject->*inMethod)(inArgs...); }}
	{
		// This space intentionally blank
	}

	template<typename Object, typename Method>
	EventCallback(std::shared_ptr<Object>&& ioObject, Method inMethod) :
		EventCallback{[object = std::move(ioObject), inMethod](auto&&... inArgs) { (object->*inMethod)(inArgs...); }}
	{
		// This space intentionally blank
	}

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

	template<typename Lambda>
	EventCallback(Lambda&& ioLambda) :
		mAnyCallback{std::forward<Lambda>(ioLambda)}
	{
		// Deduce the argument types of the input ioLambda
		using LambdaType = std::decay_t<Lambda>;
		using CallbackTraits = detail::LambdaTraits<decltype(&LambdaType::operator())>;
		using SenderType = typename CallbackTraits::Arg1Type;
		using EventType = typename CallbackTraits::Arg2Type;

		using ArgsTraits = detail::EventArgsTraits<SenderType, EventType>;

		mInvoker = +[](std::any& inCallback, const std::any& inArgs)
		{
			using ArgsType = typename ArgsTraits::ValueType;
			const auto& args = std::any_cast<const ArgsType&>(inArgs);
			const auto& [sender, event] = args;

			auto& callback = std::any_cast<Lambda&>(inCallback);
			callback(sender, event);
		};
		mEventKind = ArgsTraits::kTypeIndex;

		static_assert(std::is_invocable_v<Lambda, SenderType&, const EventType&>,
					  "Must be invocable as: ioLambda(SenderType&, const EventType&) -> void");
	}

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

class EventManager :
	public std::enable_shared_from_this<EventManager> // is-a: enable_shared_from_this
{
public:
	using Token = EventToken;
	using TokenHandler = raii::ResourceHandler<Token, raii::ResourceDeleter<Token>>; // RAII handler for EventManager::Token

public:
	// By default we make this unique, until it needs to be shared
	static std::shared_ptr<EventManager> Make(ThreadingPolicy inPolicy = ThreadingPolicy::kSingle);

	virtual ~EventManager() = default;

public:
	[[nodiscard]] TokenHandler Register(UsingRAII, const EventCallback& inCallback);

	[[nodiscard]] TokenHandler Register(UsingRAII, EventCallback&& ioCallback);

	[[nodiscard]] Token Register(const EventCallback& inCallback);

	[[nodiscard]] Token Register(EventCallback&& ioCallback);

	void Unregister(Token inToken);

	template<typename SenderType, typename EventType>
	void Notify(SenderType& inSender, const EventType& inEvent);

protected:
	EventManager() = default;

	static void Invoke(const EventCallback& inCallback, const std::any& inAnyArgs);

private:
	virtual Token OnRegister(std::type_index inEventKind, EventCallback&& ioCallback) = 0;

	virtual void OnUnregister(Token inToken) = 0;

	virtual void OnNotify(std::type_index inEventKind, std::any&& inAnyArgs) = 0;

}; // class EventManager

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

namespace detail {

// ------------------------------------------------------------------
#if SABER_PRAGMA(REGION)
#pragma region class EventManagerImpl
#endif

class EventManagerImpl final : public EventManager // EventManagerImpl is-a EventManager
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

	// GetCallbackListOrCopy() enforces Copy On Write safety for the callback list
	CallbackList& GetCallbackListOrCopy()
	{
		// NOTE: use_count() is not thread-safe; use_count() checks assume no concurrent access
		{
			const bool isNotifying = (mCallbackList.use_count() > 1);
    		if (isNotifying)
			{
				// Copy-on-write: make copy of in-flight callbacklist...
				// The use_count() of this new copy in mCallbackList becomes: "==1"
				// The use_count() of the previous mCallback instance is now: "-=1"
				mCallbackList = std::make_shared<CallbackList>(*mCallbackList);
			}
		}

		return *mCallbackList;
	}

}; // class EventManagerImpl

inline EventManager::Token EventManagerImpl::OnRegister(std::type_index inEventKind, EventCallback&& ioCallback)
{
	// SABER-76: "Create RAII handler for EventManager::Register"
	Token token = GenerateToken();
	auto& callbackList = GetCallbackListOrCopy();
	callbackList.emplace_back(token, inEventKind, std::move(ioCallback));
	return token;
}

inline void EventManagerImpl::OnUnregister(Token inToken)
{
	// Search for the token in list and remove it
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

	// Use an optimal O(1) removal for performance; note that list order is not considered important
	*didFind = std::move(callbackList.back());
	callbackList.pop_back(); // Remove the last element
}

inline void EventManagerImpl::OnNotify(std::type_index inEventKind, std::any&& inAnyArgs)
{
    // "snapshot" the current state of the mCallbackList...
    // This protects against modification of mCallbackList due to
    // re-entrant Register/Unregister calls during OnNotify()

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

	// Search for the token in the callback list and invoke all associated callbacks using std::for_each
	// use std::for_each for C++11, preferred way
	// snapshot's .use_count() is decremented here (RAII)...
    // if mCallbackList was modified during OnNotify(), the snapshot's .use_count()
    // will ==0, and the snapshot's old-copy-of the callback list is also deleted.
    // however, if no change was made to mCallbackList, snapshot's .use_count()
    // will >=1, and no list deletion occurs.
	std::for_each(snapshot->begin(), snapshot->end(), findAllCallbacks);
}

#if SABER_PRAGMA(REGION)
#pragma endregion {}
#endif

// ------------------------------------------------------------------
#if SABER_PRAGMA(REGION)
#pragma region class EventManagerParallelImpl
#endif

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

inline auto& EventManagerParallelImpl::GetCallbackListOrCopy()
{
	{
		const bool isNotifying = (mCallbackList.use_count() > 1);
		if (isNotifying)
		{
			// Copy-on-write: make copy of in-flight callbacklist...
			// The use_count() of this new copy in mCallbackList becomes: "==1"
			// The use_count() of the previous mCallback instance is now: "-=1"
			mCallbackList = std::make_shared<CallbackList>(*mCallbackList);
		}
	}

	return *mCallbackList;
}

inline EventManager::Token EventManagerParallelImpl::OnRegister(std::type_index inArgsType, EventCallback&& ioCallback)
{
	Token token;
	{
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
        std::lock_guard<std::mutex> lock(mMutex);

        snapshot = mCallbackList; // now a synchronized read
    }

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

} // namespace detail

inline /*static*/ std::shared_ptr<EventManager> EventManager::Make(ThreadingPolicy inPolicy)
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
	// default:
	// We don't use default here because there are only 2 enum values, 
	// and we want the compiler to helpfully alert us if someone ever adds a 3rd enum
	} 
	
	return result;
}

} // namespace saber::events

namespace saber::raii {

inline void ResourceDeleter<saber::events::EventToken>::operator()(saber::events::EventToken& inEventToken) const noexcept
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