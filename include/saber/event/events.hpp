#ifndef SABER_EVENT_EVENTS_HPP
#define SABER_EVENT_EVENTS_HPP

// saber
#include "saber/config.hpp"
#include "saber/utility.hpp"

// std
#include <algorithm>
#include <any>
#include <typeindex>
#include <typeinfo>
#include <vector>

namespace saber::event {

class EventCallback
{
	// We need to add some beef here so it can take user provided callbacks as constructor args
	int operator()(/*no parameters*/) const
	{
		return 0;
	}
};

// Using NVI pattern here
class EventManager
{
public:
	using Token = saber::TaggedType<std::uint64_t, EventManager>;

public:
	template<typename EventType>
	Token Register(EventCallback&& ioCallback); // Consume

	template<typename EventType>
	Token Register(const EventCallback& inCallback); // Observe

	void Unregister(Token inToken);

	template<typename EventType>
	void Notify(const EventType& inEvent);

private:
	virtual Token OnRegister(std::type_index inEventType, EventCallback&& ioCallback) = 0;

	virtual void OnUnregister(Token inToken) = 0;

	virtual void OnNotify(std::any inEventValue) = 0;

	~EventManager() = default;
}; // class EventManager

// TODO: Investigate sink parameter pattern(pass by value to avoid making addtl copies via const&) here
template<typename EventType>
inline EventManager::Token EventManager::Register(EventCallback&& ioCallback) // Consume
{
	// TRICKY: virtuals are unable to accept template types, so use typeid and std::type_index
	// to allow us to find callbacks of a certain type from the callback list
	std::type_index eventType = typeid(ioCallback);
	OnRegister(eventType, ioCallback);
}

template<typename EventType>
inline EventManager::Token EventManager::Register(const EventCallback& inCallback) // Observe
{
	// TRICKY: virtuals are unable to accept template types, so use typeid and std::type_index
	// to allow us to find callbacks of a certain type from the callback list
	std::type_index eventType = typeid(inCallback);
	OnRegister(inCallback);
}

inline void EventManager::Unregister(Token inToken)
{
	OnUnregister(inToken);
}

template<typename EventType>
inline void EventManager::Notify(const EventType& inEvent)
{
	// Get "work" value out of the event and pass it in
	OnNotify(typeid(inEvent), inEvent /*the work value*/);
}

class SimpleEventManager final : public EventManager // SimpleEventManager is-a EventManager
{
private:
	Token OnRegister(std::type_index inEventType, EventCallback&& ioCallback) override;

	void OnUnregister(Token inToken) override;

	void OnNotify(std::any inEventValue) override;

private:
	std::uint64_t counter{ 0 }; // Counter to generate unique tokens
	std::vector<std::tuple<Token, std::type_index, EventCallback>> mCallbackList;
}; // class SimpleEventManager

inline EventManager::Token SimpleEventManager::OnRegister(std::type_index inEventType, EventCallback&& ioCallback)
{
	Token newToken{ counter++ }; // Create a unique token
	mCallbackList.push_back({newToken, inEventType, ioCallback}); // Store the token in the list
	return newToken;
}

inline void SimpleEventManager::OnUnregister(Token inToken)
{
	// Search for the token in list and remove it
	using CallbackElement = decltype(mCallbackList)::value_type; // Get the type of elements in mCallbackList
	auto isTargetToken = [inToken](const CallbackElement& element) {
		const bool isTarget = std::get<0>(element) == inToken;
		return isTarget;
	};

	// There will only ever be one matching token, therefore, we can use std::find_if to
	// find the first matching token and erase it without needing to go through the entire list with std::remove_if
	const auto didFind = std::find_if(mCallbackList.begin(), mCallbackList.end(), isTargetToken);
	if (didFind != mCallbackList.end()) {

		// REVISIT: Move assign might throw an exception?
		// If so, we might have to do something like this:
		//		std::iter_swap(didFind, std::prev(mCallbackList.end()));
		//		mCallbackList.pop_back();

		// Use an optimal O(1) removal for performance; note that list order is not considered important
		*didFind = std::move(mCallbackList.back()); 
		mCallbackList.pop_back(); // Remove the last element
	}
}

inline void SimpleEventManager::OnNotify(std::any inEventValue)
{
	EventCallback eventType{};
	const std::type_index targetType = inEventValue.type();
	auto findAllCallbacks = [&inEventValue, &targetType](const auto& element) -> void
		{
			const auto& [token, eventType, callback] = element;
			if (eventType == targetType)
			{
				// TODO: Get the value from event to pass into callback
				callback(/*the work value*/); // Invoke the callback
			}
		};

	// Search for the token in the callback list and invoke all associated callbacks using std::for_each
#if 1
	// use range based for C++11, preferred way
	std::for_each(mCallbackList.begin(), mCallbackList.end(), findAllCallbacks);
#elif 0
	// use range based for C++17
	for (const auto& element : mCallbackList)
	{
		const EventCallback& callback = std::get<1>(element);
		callback(); // Invoke the callback
	}
#elif 0
	// use std::iterator C++03
	for (auto iter = mCallbackList.begin(); iter != mCallbackList.end(); ++iter)
	{
		const EventCallback& callback = std::get<1>(*iter);
		callback(); // Invoke the callback
	}
#elif 0
	// use for loop with index C
	for (size_t i = 0; i < mCallbackList.size(); ++i)
	{
		const EventCallback& callback = std::get<1>(mCallbackList[i]);
		callback(); // Invoke the callback
	}
#endif
}

	/*
	// I want to know when an NPC gets damaged
	// Therefore, we have to keep track of the
	// 1. list of tokens
	// 2. type of event
	// 3. function callback

		struct NPC
		{
			// NPC data here
			int hitpoints;
		};

		struct HitpointChanged // Event to modify NPCs
		{
			NPC& npc;
			int hitpoints;
		};

		void AdjustHitpoints(const HitpointsChanged& inChange);
		void ShowExplode(const HitpointsChanged& inChange);

		EventCallback hitpointAdjust{AdjustHitpoints, damage};
		EventCallback showExplode{ShowExplode};

		// Just 1 event manager to rule them all
		EventManager& eventManager = magic;

		auto tokenHP = eventManager.Register<HitpointChanged>(hitpointAdjust); // Consumer 1: NPC has a hitpoint adjust event registered
		auto tokenExplode1 = eventManager.Register<HitpointChanged>(showExplode); // Consumer 2: NPC has an explode event registered
		auto tokenExplode2 = eventManager.Register<HitpointChanged>(showExplode); // Consumer 3: NPC is registered for explode event again, but as tokens are unique, it is fine

		// Gameplay occurs here

		// Send a notification here
		NPC dummyNPC{};
		HitpointChanged damage{dummyNPC, 20}; // HP changed and the instance which it occurred

		eventManager.Notify<HitpointChanged>(damage); // Notify all consumers the myNPC event has occurred
		eventManager.Notify(5); // Notify all consumers the myNPC event has occurred

		// Gameplay ends here

		eventManager.Unregister(tokenExplode1); // Order isn't as important, but we can unregister the explode event first
		eventManager.Unregister(tokenExplode2); // Unregister tokenExplode2
		eventManager.Unregister(tokenHP); // NPC is dead, unregister hitpoint adjust event
	*/

namespace detail {



} // namespace detail

} // namespace saber::event
#endif // SABER_EVENT_EVENTS_HPP