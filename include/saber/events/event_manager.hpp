#ifndef SABER_EVENTS_EVENTMANAGER_HPP
#define SABER_EVENTS_EVENTMANAGER_HPP

// saber
#include "saber/config.hpp"
#include "saber/utility.hpp"
#include "saber/raii/reference_handler.hpp"

// std
#include <algorithm>
#include <any>
#include <memory>
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <vector>


// --- 1. Proper forward declarations, in the real namespace ---
namespace saber::events {
    class EventManager;
    using EventToken = saber::TaggedType<std::uint64_t, EventManager>;
} // namespace saber::events

// --- 2. Deleter specialization: declare only, don't define the body yet ---
namespace saber::raii::detail {

template<>
struct Deleter<saber::events::EventToken>
{
public:
    explicit Deleter(saber::events::EventManager& inEventManager) noexcept :
        mEventManager{inEventManager}
    {
    }

    // Declaration only — EventManager is still incomplete here, which is fine
    // because we aren't calling any of its members yet.
    void operator()(saber::events::EventToken* inToken) const noexcept;

private:
    saber::events::EventManager& mEventManager; // a reference to an incomplete type is OK
};

} // namespace saber::raii::detail

namespace saber::events {

template<typename SenderType, typename EventType>
using EventArgsType = std::tuple<const SenderType&, const EventType&>;

class EventManager; // forward declaration

using EventToken = saber::TaggedType<std::uint64_t, EventManager>;

// The `EventCallback` class type-erases a user-provided callable (e.g., a
// lambda) and provides a uniform `int operator()(std::any)` entry point that
// can be invoked by the event system regardless of the concrete event type.

class EventCallback 
{
public:
	template<typename SenderType, typename EventType, typename Lambda>
	static EventCallback Make(Lambda&& ioLambda)
	{
		// Use it like this:
		//     struct MyEvent{int mHitpointsRemaining{};};
		//     auto myCallback = EventCallback::Make<SenderType, MyEvent>([](const SenderType&, const MyEvent&){return 0;});
		static_assert(std::is_invocable_r_v<int, Lambda, const SenderType&, const EventType&>);
		return EventCallback(SenderTypeTag<SenderType>{}, EventTypeTag<EventType>{}, std::forward<Lambda>(ioLambda));
	}

	// Expose a uniform call operator that accepts the event as `std::any`.
	// This forwards the erased event and callable into the trampoline.
	int operator()(const std::any& inArg) const
	{
		return mInvoke(mCallback, inArg);
	}

private:
	friend class EventManager; // allow EventManager to construct it

	// Phantom tag so the constructor can deduce EventType without explicit
	// template arguments (constructors cannot have explicit template args in C++17).
	template<typename T> struct EventTypeTag {};
	template<typename T> struct SenderTypeTag {};

	std::type_index GetTypeIndex() const
	{
		return mTypeIndex;
	}

	// Constructor template: capture any callable `Lambda` that accepts
	// `(const SenderType&, const EventType&)` and returns `int`. We store the
	// callable in `mCallback` (as `std::any`) and create a small trampoline
	// function (`mInvoke`) that knows how to cast the `std::any` values back to
	// the original `Lambda` and event tuple and call the callable.
	template<typename SenderType, typename EventType, typename Lambda>
	EventCallback(SenderTypeTag<SenderType>, EventTypeTag<EventType>, Lambda inLambda) :

		// store the user-provided callable (type-erased)
		mCallback{std::move(inLambda)}, // Consume the input lambda; no need to make a copy

		mTypeIndex{typeid(EventArgsType<SenderType, EventType>)}, // store the type_index of the event args

		// trampoline: casts the erased callable and erased event back to
		// their concrete types and invokes the callable.
		mInvoke{+[](const std::any& inCallback, const std::any& inArgs)
		{
			// Recover the original callable (Lambda) from the std::any.
			auto& callback = std::any_cast<const Lambda&>(inCallback);
			// Recover the concrete sender/event values from the std::any.
			auto& args = std::any_cast<const EventArgsType<SenderType, EventType>&>(inArgs);
			auto [senderRef, eventRef] = args;
			// Call the original callback using its sender and event.
			return callback(senderRef, eventRef);
		}}
	{
	}

	// Trampoline function type: takes the erased callable and the erased
	// event value and returns an `int` result.
	using CallbackType = int(*)(const std::any& inCallback, const std::any& inArgs);

private:
	// The original callable stored with type-erasure so many different
	// callable types can be stored in the same container.
	std::any mCallback{};
	// Stores the type_index of the EventArgsType<SenderType, EventType>
	std::type_index mTypeIndex{typeid(void)}; 
	// Pointer to the trampoline function that knows how to cast and
	// invoke `mCallback` for the correct `EventType`.
	CallbackType mInvoke{};
};

// Marker type for RAII usage
struct UsingRAII{};

// Using NVI pattern here
class EventManager
{
public:
	using Token = EventToken;
	using TokenHandler = saber::raii::ReferenceHandler<Token, raii::detail::Deleter<Token>>; // RAII handler for EventManager::Token

public:
	static std::unique_ptr<EventManager> Make();

	virtual ~EventManager() = default;

public:
	[[nodiscard]] TokenHandler Register(UsingRAII, EventCallback&& ioCallback); // Consume

	[[nodiscard]] Token Register(const EventCallback& inCallback); // Observe

	[[nodiscard]] Token Register(EventCallback&& ioCallback);

	void Unregister(Token inToken);

	template<typename SenderType, typename EventType>
	void Notify(SenderType& inSender, const EventType& inEvent);

protected:
	EventManager() = default;

private:
	virtual Token OnRegister(std::type_index inArgsType, EventCallback&& ioCallback) = 0;

	virtual void OnUnregister(Token inToken) = 0;

	virtual void OnNotify(std::any inArgs) = 0;

}; // class EventManager

inline EventManager::TokenHandler EventManager::Register(UsingRAII, EventCallback&& ioCallback) // Consume
{
	// FIXME: Cannot use make_unique because it does not allow you to specify a custom deleter
	// TokenHandler specifies a custom deleter, but token is not a pointer
	auto token = std::make_unique<Token>(Register(std::move(ioCallback)));
	return TokenHandler{token.release()}; // RAII handler for the token
}

// TODO: Investigate sink parameter pattern(pass by value to avoid making addtl copies via const&) here
inline EventManager::Token EventManager::Register(const EventCallback& inCallback)
{
	return Register(EventCallback{inCallback});
}

inline EventManager::Token EventManager::Register(EventCallback&& ioCallback) // Consume
{
	// TRICKY: virtuals are unable to accept template types, so use typeid and std::type_index
	// to allow us to find callbacks of a certain type from the callback list
	std::type_index eventType = ioCallback.GetTypeIndex(); // use the template params
	return OnRegister(eventType, std::move(ioCallback));
}

inline void EventManager::Unregister(Token inToken)
{
	OnUnregister(inToken);
}

template<typename SenderType, typename EventType>
inline void EventManager::Notify(SenderType& inSender, const EventType& inEvent)
{
	EventArgsType<SenderType, EventType> args{inSender, inEvent};
	OnNotify(std::any{args}); // Explicit copy
}

namespace detail {

class EventManagerImpl final : public EventManager // EventManagerImpl is-a EventManager
{
public:
	~EventManagerImpl() override = default;

private:
	friend class EventManager; // allow Make() to construct it
    EventManagerImpl() = default;

private:
	Token OnRegister(std::type_index inArgsType, EventCallback&& ioCallback) override;

	void OnUnregister(Token inToken) override;

	void OnNotify(std::any inArgs) override;

private:
	std::uint64_t mCounter{ 0 }; // Counter to generate unique tokens

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

inline EventManager::Token EventManagerImpl::OnRegister(std::type_index inArgsType, EventCallback&& ioCallback)
{
	// SABER-76: "Create RAII handler for EventManager::Register"
	Token newToken{ mCounter++ }; // Create a unique token
	auto& callbackList = GetCallbackListOrCopy();
	callbackList.emplace_back(newToken, inArgsType, std::move(ioCallback));
	return newToken;
}

inline void EventManagerImpl::OnUnregister(Token inToken)
{
	// Search for the token in list and remove it
	auto& callbackList = GetCallbackListOrCopy();
	auto isTargetToken = [inToken](const CallbackElement& element)
	{
		const bool isTarget = std::get<0>(element) == inToken;
		return isTarget;
	};

	// There will only ever be one matching token, therefore, we can use std::find_if to
	// find the first matching token and erase it without needing to go through the entire list with std::remove_if
	const auto didFind = std::find_if(callbackList.begin(), callbackList.end(), isTargetToken);

	if (didFind != callbackList.end())
	{
		// REVISIT: Move assign might throw an exception?
		// If so, we might have to do something like this:
		//		std::iter_swap(didFind, std::prev(callbackList.end()));
		//		callbackList.pop_back();

		// Use an optimal O(1) removal for performance; note that list order is not considered important
		*didFind = std::move(callbackList.back());
		callbackList.pop_back(); // Remove the last element
	}
}

inline void EventManagerImpl::OnNotify(std::any inArgs)
{
    // "snapshot" the current state of the mCallbackList...
    // This protects against modification of mCallbackList due to
    // re-entrant Register/Unregister calls during OnNotify()

	const std::type_index targetType = inArgs.type();
	auto snapshot = mCallbackList;
	auto findAllCallbacks = [&inArgs, &targetType](const auto& element) -> void
	{
		const auto& [token, eventType, callback] = element;
		if (eventType == targetType)
		{
			callback(inArgs); // Reference operator(): invoke the callback
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

} // namespace detail

inline /*static*/ std::unique_ptr<EventManager> EventManager::Make()
{
	std::unique_ptr<EventManager> result{new detail::EventManagerImpl()};
	return result;
}

} // namespace saber::events

namespace saber::raii::detail {

inline void Deleter<saber::events::EventToken>::operator()(saber::events::EventToken* inToken) const noexcept
{
    if (inToken)
    {
        mEventManager.Unregister(*inToken); // OK now — EventManager is complete
    }
}

} // namespace saber::raii::detail

#endif // SABER_EVENTS_EVENTMANAGER_HPP