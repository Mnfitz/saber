#ifndef SABER_EVENT_EVENTS_HPP
#define SABER_EVENT_EVENTS_HPP

// std
#include <vector>

// saber
#include "saber/config.hpp"

namespace saber::event {

class EventCallback
{
	// We need to add some beef here so it can take user provided callbacks as constructor args
};

class EventManager
{
	// Consider using saber::TaggedType<std::uint64_t> as the basis for token in saber/utility.hpp
	struct Token
	{
	public:
		Token() = default;

		Token(std::uint64_t inId)
			: mId(inId)
		{

		}
	private:
		// 1. I need something to make it unique
		// 2. It should be small enough for pass-by-value
		std::uint64_t mId{ 0 }; // Unique identifier for the token
	};

public:
	template<typename EventType>
	// Consume:
	Token Register(EventCallback&& ioCallback);
	// Observe:
	Token Register(const EventCallback& inCallback);

	void Unregister(Token inToken);

	void Notify(Token inToken);

private:
	Token OnRegister() = 0;

	void OnUnregister() = 0;

	void OnNotify() = 0;

	//private:
	std::uint64_t counter{ 0 }; // Counter to generate unique tokens
	std::vector<std::tuple<Token, EventCallback>> mCallbackList;
}; // class EventManager

template<typename EventType>
inline EventManager::Token EventManager::Register(EventCallback&& ioCallback)
{
	//OnRegister();
	Token newToken{ counter++ }; // Create a unique token
	// TODO: Resolve observe vs consume callback parameter
	mCallbackList.push_back(std::tuple<Token, EventCallback>{newToken, ioCallback}); // Store the token in the list
	return newToken;
}

inline EventManager::Token EventManager::Unregister(Token inToken)
{
	//OnUnregister();
	// Search for the token in both lists and remove it
}

EventManager::Token EventManager::Notify(Token inToken)
{
	// Invoke any callbacks associated with the token
	//OnNotify();
	// Search for the token in the callback list and invoke all associated callbacks
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

		eventManager.Notify(damage); // Notify all consumers the myNPC event has occurred

		// Gameplay ends here

		eventManager.Unregister(tokenExplode1); // Order isn't as important, but we can unregister the explode event first
		eventManager.Unregister(tokenExplode2); // Unregister tokenExplode2
		eventManager.Unregister(tokenHP); // NPC is dead, unregister hitpoint adjust event
	*/

namespace detail {



} // namespace detail

} // namespace saber::event
#endif // SABER_EVENT_EVENTS_HPP