#ifndef SABER_EVENT_EVENTS_HPP
#define SABER_EVENT_EVENTS_HPP

// saber
#include "saber/config.hpp"
#include "saber/utility.hpp"

// std
#include <algorithm>
#include <any>
#include <memory>
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <vector>

namespace saber::events {

template<typename SenderType, typename EventType>
using EventArgsType = std::tuple<const SenderType&, const EventType&>;

class EventManager; // forward declaration

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


// Using NVI pattern here
class EventManager
{
public:
	using Token = saber::TaggedType<std::uint64_t, EventManager>;

public:
	static std::unique_ptr<EventManager> Make();

	virtual ~EventManager() = default;

public:
	[[nodiscard]] Token Register(EventCallback&& ioCallback); // Consume

	[[nodiscard]] Token Register(const EventCallback& inCallback); // Observe

	void Unregister(Token inToken);

	template<typename SenderType, typename EventType>
	void Notify(SenderType& inSender, const EventType& inEvent);

protected:
	EventManager() = default;

private:
	virtual Token OnRegister(std::type_index inEventType, EventCallback&& ioCallback) = 0;

	virtual void OnUnregister(Token inToken) = 0;

	virtual void OnNotify(std::any inArgs) = 0;

}; // class EventManager

// TODO: Investigate sink parameter pattern(pass by value to avoid making addtl copies via const&) here
inline EventManager::Token EventManager::Register(EventCallback&& ioCallback) // Consume
{
	// TRICKY: virtuals are unable to accept template types, so use typeid and std::type_index
	// to allow us to find callbacks of a certain type from the callback list
	std::type_index eventType = ioCallback.GetTypeIndex(); // use the template params
	return OnRegister(eventType, std::move(ioCallback));
}

inline EventManager::Token EventManager::Register(const EventCallback& inCallback) // Observe
{
	// TRICKY: virtuals are unable to accept template types, so use typeid and std::type_index
	// to allow us to find callbacks of a certain type from the callback list
	EventCallback copy{inCallback}; // explicit copy
	std::type_index eventType = inCallback.GetTypeIndex(); // use the template params
    return OnRegister(eventType, std::move(copy));
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
	Token OnRegister(std::type_index inEventType, EventCallback&& ioCallback) override;

	void OnUnregister(Token inToken) override;

	void OnNotify(std::any inArgs) override;

private:
	std::uint64_t mCounter{ 0 }; // Counter to generate unique tokens
	std::vector<std::tuple<Token, std::type_index, EventCallback>> mCallbackList;
}; // class EventManagerImpl

inline EventManager::Token EventManagerImpl::OnRegister(std::type_index inEventType, EventCallback&& ioCallback)
{
	Token newToken{ mCounter++ }; // Create a unique token
	mCallbackList.emplace_back(newToken, inEventType, std::move(ioCallback));
	return newToken;
}

inline void EventManagerImpl::OnUnregister(Token inToken)
{
	// Search for the token in list and remove it
	using CallbackElement = decltype(mCallbackList)::value_type; // Get the type of elements in mCallbackList
	auto isTargetToken = [inToken](const CallbackElement& element) 
	{
		const bool isTarget = std::get<0>(element) == inToken;
		return isTarget;
	};

	// There will only ever be one matching token, therefore, we can use std::find_if to
	// find the first matching token and erase it without needing to go through the entire list with std::remove_if
	const auto didFind = std::find_if(mCallbackList.begin(), mCallbackList.end(), isTargetToken);

	if (didFind != mCallbackList.end()) 
	{
		// REVISIT: Move assign might throw an exception?
		// If so, we might have to do something like this:
		//		std::iter_swap(didFind, std::prev(mCallbackList.end()));
		//		mCallbackList.pop_back();

		// Use an optimal O(1) removal for performance; note that list order is not considered important
		*didFind = std::move(mCallbackList.back()); 
		mCallbackList.pop_back(); // Remove the last element
	}
}

inline void EventManagerImpl::OnNotify(std::any inArgs)
{
	const std::type_index targetType = inArgs.type();
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
	std::for_each(mCallbackList.begin(), mCallbackList.end(), findAllCallbacks);
}

} // namespace detail

inline /*static*/ std::unique_ptr<EventManager> EventManager::Make()
{
	std::unique_ptr<EventManager> result{new detail::EventManagerImpl()};
	return result;
}

} // namespace saber::events
#endif // SABER_EVENT_EVENTS_HPP