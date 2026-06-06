// ============================================================================
// test_events.cpp
// Unit tests for saber::event public API
//   - EventCallback::Make<>()
//   - EventManager::Make()
//   - EventManager::Register<>() (consume + observe overloads)
//   - EventManager::Unregister()
//   - EventManager::Notify<>()
//
// Framework: Catch2 v3
// Standard:  C++17
// ============================================================================

#include <catch2/catch_test_macros.hpp>

#include "saber/event/events.hpp"

// ============================================================================
// Test event types
// ============================================================================

struct DamageEvent
{
    int mAmount{};
};

struct HealEvent
{
    int mAmount{};
};

struct EmptyEvent {};

// Convenience alias
using namespace saber::event;

// ============================================================================
// Helpers
// ============================================================================

// Builds a callback that appends inEvent.mAmount into `outLog` and returns 0.
template<typename EventType>
EventCallback MakeLoggingCallback(std::vector<int>& outLog)
{
    return EventCallback::Make<EventType>([&outLog](const EventType& inEvent) -> int
    {
        outLog.push_back(inEvent.mAmount);
        return 0;
    });
}

// ============================================================================
// SECTION: EventCallback
// ============================================================================

TEST_CASE("EventCallback: Make produces a callable wrapper", "[EventCallback]")
{
    SECTION("Callback invoked with correct event value")
    {
        int received = -1;
        auto cb = EventCallback::Make<DamageEvent>([&received](const DamageEvent& e) -> int
        {
            received = e.mAmount;
            return 0;
        });

        DamageEvent ev{42};
        cb(std::any{ev});

        REQUIRE(received == 42);
    }

    SECTION("Return value from lambda is forwarded through operator()")
    {
        auto cb = EventCallback::Make<DamageEvent>([](const DamageEvent&) -> int
        {
            return 99;
        });

        DamageEvent ev{0};
        const int result = cb(std::any{ev});
        REQUIRE(result == 99);
    }

    SECTION("operator() is const — callable on a const EventCallback")
    {
        const auto cb = EventCallback::Make<DamageEvent>([](const DamageEvent& e) -> int
        {
            return e.mAmount;
        });

        DamageEvent ev{7};
        REQUIRE(cb(std::any{ev}) == 7);
    }

    SECTION("Callback captures mutable state correctly across multiple calls")
    {
        int callCount = 0;
        auto cb = EventCallback::Make<DamageEvent>([&callCount](const DamageEvent&) -> int
        {
            ++callCount;
            return 0;
        });

        DamageEvent ev{};
        cb(std::any{ev});
        cb(std::any{ev});
        cb(std::any{ev});

        REQUIRE(callCount == 3);
    }

    SECTION("EventCallback is copy-constructible (Observe semantics)")
    {
        int received = 0;
        auto original = EventCallback::Make<DamageEvent>([&received](const DamageEvent& e) -> int
        {
            received = e.mAmount;
            return 0;
        });

        EventCallback copy = original; // Must compile and work independently
        DamageEvent ev{55};
        copy(std::any{ev});

        REQUIRE(received == 55);
    }

    SECTION("EventCallback is move-constructible (Consume semantics)")
    {
        int received = 0;
        auto original = EventCallback::Make<DamageEvent>([&received](const DamageEvent& e) -> int
        {
            received = e.mAmount;
            return 0;
        });

        EventCallback moved = std::move(original);
        DamageEvent ev{88};
        moved(std::any{ev});

        REQUIRE(received == 88);
    }
}

// ============================================================================
// SECTION: EventManager factory
// ============================================================================

TEST_CASE("EventManager::Make returns a non-null manager", "[EventManager]")
{
    auto manager = EventManager::Make();
    REQUIRE(manager != nullptr);
}

TEST_CASE("EventManager::Make returns independent instances", "[EventManager]")
{
    auto m1 = EventManager::Make();
    auto m2 = EventManager::Make();
    REQUIRE(m1 != m2);
}

// ============================================================================
// SECTION: Register + Notify (basic dispatch)
// ============================================================================

TEST_CASE("Notify dispatches to a single registered callback", "[Register][Notify]")
{
    auto manager = EventManager::Make();
    std::vector<int> log;

    auto token = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(log));

    manager->Notify<DamageEvent>(DamageEvent{10});

    REQUIRE(log.size() == 1);
    REQUIRE(log[0] == 10);

    manager->Unregister(token);
}

TEST_CASE("Notify dispatches to multiple callbacks registered for the same event type", "[Register][Notify]")
{
    auto manager = EventManager::Make();
    std::vector<int> logA, logB;

    auto tA = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(logA));
    auto tB = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(logB));

    manager->Notify<DamageEvent>(DamageEvent{25});

    REQUIRE(logA.size() == 1);
    REQUIRE(logA[0] == 25);
    REQUIRE(logB.size() == 1);
    REQUIRE(logB[0] == 25);

    manager->Unregister(tA);
    manager->Unregister(tB);
}

TEST_CASE("Notify does NOT dispatch to callbacks registered for a different event type", "[Register][Notify]")
{
    auto manager = EventManager::Make();
    std::vector<int> damageLog, healLog;

    auto tDmg  = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(damageLog));
    auto tHeal = manager->Register<HealEvent>(MakeLoggingCallback<HealEvent>(healLog));

    manager->Notify<DamageEvent>(DamageEvent{5});

    REQUIRE(damageLog.size() == 1);
    REQUIRE(healLog.empty());        // HealEvent callback must NOT fire

    manager->Unregister(tDmg);
    manager->Unregister(tHeal);
}

TEST_CASE("Notify with no registered callbacks is a no-op", "[Notify]")
{
    auto manager = EventManager::Make();
    // Should not throw or crash
    REQUIRE_NOTHROW(manager->Notify<DamageEvent>(DamageEvent{99}));
}

TEST_CASE("Notify delivers correct event data (field values preserved)", "[Notify]")
{
    auto manager = EventManager::Make();
    int capturedAmount = -1;

    auto token = manager->Register<DamageEvent>(
        EventCallback::Make<DamageEvent>([&capturedAmount](const DamageEvent& e) -> int
        {
            capturedAmount = e.mAmount;
            return 0;
        }));

    manager->Notify<DamageEvent>(DamageEvent{1337});

    REQUIRE(capturedAmount == 1337);
    manager->Unregister(token);
}

TEST_CASE("Notify can fire the same event type multiple times", "[Notify]")
{
    auto manager = EventManager::Make();
    std::vector<int> log;
    auto token = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(log));

    manager->Notify<DamageEvent>(DamageEvent{1});
    manager->Notify<DamageEvent>(DamageEvent{2});
    manager->Notify<DamageEvent>(DamageEvent{3});

    REQUIRE(log == std::vector<int>{1, 2, 3});
    manager->Unregister(token);
}

TEST_CASE("Notify can fire different event types independently", "[Notify]")
{
    auto manager = EventManager::Make();
    std::vector<int> damageLog, healLog;

    auto tDmg  = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(damageLog));
    auto tHeal = manager->Register<HealEvent>(MakeLoggingCallback<HealEvent>(healLog));

    manager->Notify<DamageEvent>(DamageEvent{10});
    manager->Notify<HealEvent>(HealEvent{20});

    REQUIRE(damageLog == std::vector<int>{10});
    REQUIRE(healLog   == std::vector<int>{20});

    manager->Unregister(tDmg);
    manager->Unregister(tHeal);
}

// ============================================================================
// SECTION: Register overloads (consume vs. observe)
// ============================================================================

TEST_CASE("Register rvalue overload (consume) — callback fires correctly", "[Register]")
{
    auto manager = EventManager::Make();
    bool fired = false;

    auto cb = EventCallback::Make<DamageEvent>([&fired](const DamageEvent&) -> int
    {
        fired = true;
        return 0;
    });

    auto token = manager->Register<DamageEvent>(std::move(cb)); // rvalue overload
    manager->Notify<DamageEvent>(DamageEvent{});

    REQUIRE(fired);
    manager->Unregister(token);
}

TEST_CASE("Register const-ref overload (observe) — callback fires correctly", "[Register]")
{
    auto manager = EventManager::Make();
    bool fired = false;

    const auto cb = EventCallback::Make<DamageEvent>([&fired](const DamageEvent&) -> int
    {
        fired = true;
        return 0;
    });

    auto token = manager->Register<DamageEvent>(cb); // const-ref overload
    manager->Notify<DamageEvent>(DamageEvent{});

    REQUIRE(fired);
    manager->Unregister(token);
}

TEST_CASE("Register const-ref overload does not consume the original callback", "[Register]")
{
    // After registering via const&, the original callback must still be usable
    auto manager = EventManager::Make();
    int callCount = 0;

    const auto cb = EventCallback::Make<DamageEvent>([&callCount](const DamageEvent&) -> int
    {
        ++callCount;
        return 0;
    });

    auto token = manager->Register<DamageEvent>(cb); // observe copy

    // Also invoke the original directly — it must still be intact
    cb(std::any{DamageEvent{}});

    manager->Notify<DamageEvent>(DamageEvent{});

    // Direct invocation (1) + notify (1) = 2
    REQUIRE(callCount == 2);
    manager->Unregister(token);
}

TEST_CASE("Register returns distinct tokens for each registration", "[Register]")
{
    auto manager = EventManager::Make();
    std::vector<int> log;

    auto t1 = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(log));
    auto t2 = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(log));
    auto t3 = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(log));

    REQUIRE(t1 != t2);
    REQUIRE(t2 != t3);
    REQUIRE(t1 != t3);

    manager->Unregister(t1);
    manager->Unregister(t2);
    manager->Unregister(t3);
}

TEST_CASE("Same callback can be registered multiple times, each fires independently", "[Register]")
{
    // Registering the same logical callback twice should produce two independent
    // subscriptions — both should fire on Notify.
    auto manager = EventManager::Make();
    int callCount = 0;

    const auto cb = EventCallback::Make<DamageEvent>([&callCount](const DamageEvent&) -> int
    {
        ++callCount;
        return 0;
    });

    auto t1 = manager->Register<DamageEvent>(cb);
    auto t2 = manager->Register<DamageEvent>(cb);

    manager->Notify<DamageEvent>(DamageEvent{});

    REQUIRE(callCount == 2);

    manager->Unregister(t1);
    manager->Unregister(t2);
}

// ============================================================================
// SECTION: Unregister
// ============================================================================

TEST_CASE("Unregister prevents the callback from firing on subsequent Notify", "[Unregister]")
{
    auto manager = EventManager::Make();
    std::vector<int> log;

    auto token = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(log));
    manager->Notify<DamageEvent>(DamageEvent{1}); // fires once before unregister

    manager->Unregister(token);
    manager->Notify<DamageEvent>(DamageEvent{2}); // must NOT fire

    REQUIRE(log.size() == 1);
    REQUIRE(log[0] == 1);
}

TEST_CASE("Unregistering one token does not affect other registered callbacks", "[Unregister]")
{
    auto manager = EventManager::Make();
    std::vector<int> logA, logB;

    auto tA = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(logA));
    auto tB = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(logB));

    manager->Unregister(tA); // only remove A

    manager->Notify<DamageEvent>(DamageEvent{7});

    REQUIRE(logA.empty()); // A was unregistered
    REQUIRE(logB.size() == 1);
    REQUIRE(logB[0] == 7);

    manager->Unregister(tB);
}

TEST_CASE("Unregister of the only remaining callback leaves manager in clean notify-safe state", "[Unregister]")
{
    auto manager = EventManager::Make();
    std::vector<int> log;

    auto token = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(log));
    manager->Unregister(token);

    // Notifying with zero callbacks must not crash
    REQUIRE_NOTHROW(manager->Notify<DamageEvent>(DamageEvent{42}));
    REQUIRE(log.empty());
}

TEST_CASE("Unregister is a no-op for an already-removed token (double unregister safety)", "[Unregister]")
{
    // The spec does not require this to be safe, but the implementation
    // uses find_if and will simply do nothing if the token is absent.
    // Verify it does not crash.
    auto manager = EventManager::Make();
    std::vector<int> log;

    auto token = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(log));
    manager->Unregister(token);

    REQUIRE_NOTHROW(manager->Unregister(token)); // second unregister — must not crash
}

TEST_CASE("Unregister middle element — remaining callbacks still fire", "[Unregister]")
{
    // Tests that the swap-and-pop O(1) removal does not corrupt the list
    // when the removed element is not the last one.
    auto manager = EventManager::Make();
    std::vector<int> logA, logB, logC;

    auto tA = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(logA));
    auto tB = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(logB));
    auto tC = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(logC));

    manager->Unregister(tB); // Remove the middle registration

    manager->Notify<DamageEvent>(DamageEvent{3});

    REQUIRE(logA.size() == 1);
    REQUIRE(logB.empty()); // B was removed
    REQUIRE(logC.size() == 1);

    manager->Unregister(tA);
    manager->Unregister(tC);
}

TEST_CASE("Unregister first element — remaining callbacks still fire", "[Unregister]")
{
    auto manager = EventManager::Make();
    std::vector<int> logA, logB;

    auto tA = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(logA));
    auto tB = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(logB));

    manager->Unregister(tA);

    manager->Notify<DamageEvent>(DamageEvent{9});

    REQUIRE(logA.empty());
    REQUIRE(logB.size() == 1);

    manager->Unregister(tB);
}

// ============================================================================
// SECTION: Register + Unregister interleaving
// ============================================================================

TEST_CASE("Re-registering after unregister works correctly", "[Register][Unregister]")
{
    auto manager = EventManager::Make();
    std::vector<int> log;

    auto t1 = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(log));
    manager->Notify<DamageEvent>(DamageEvent{1});
    manager->Unregister(t1);

    auto t2 = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(log));
    manager->Notify<DamageEvent>(DamageEvent{2});
    manager->Unregister(t2);

    REQUIRE(log == std::vector<int>{1, 2});
}

TEST_CASE("Interleaved register/unregister across event types is isolated", "[Register][Unregister]")
{
    auto manager = EventManager::Make();
    std::vector<int> damageLog, healLog;

    auto tDmg  = manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(damageLog));
    auto tHeal = manager->Register<HealEvent>(MakeLoggingCallback<HealEvent>(healLog));

    manager->Unregister(tDmg);

    manager->Notify<DamageEvent>(DamageEvent{5});
    manager->Notify<HealEvent>(HealEvent{10});

    REQUIRE(damageLog.empty());
    REQUIRE(healLog == std::vector<int>{10});

    manager->Unregister(tHeal);
}

// ============================================================================
// SECTION: Edge cases
// ============================================================================

TEST_CASE("EmptyEvent (no data fields) can be registered and notified", "[EdgeCase]")
{
    auto manager = EventManager::Make();
    bool fired = false;

    auto token = manager->Register<EmptyEvent>(
        EventCallback::Make<EmptyEvent>([&fired](const EmptyEvent&) -> int
        {
            fired = true;
            return 0;
        }));

    manager->Notify<EmptyEvent>(EmptyEvent{});

    REQUIRE(fired);
    manager->Unregister(token);
}

TEST_CASE("Large number of registrations all fire correctly", "[EdgeCase]")
{
    auto manager = EventManager::Make();
    constexpr int kCount = 500;
    int totalFired = 0;

    std::vector<EventManager::Token> tokens;
    tokens.reserve(kCount);

    for (int i = 0; i < kCount; ++i)
    {
        tokens.push_back(manager->Register<DamageEvent>(
            EventCallback::Make<DamageEvent>([&totalFired](const DamageEvent&) -> int
            {
                ++totalFired;
                return 0;
            })));
    }

    manager->Notify<DamageEvent>(DamageEvent{1});
    REQUIRE(totalFired == kCount);

    for (auto& t : tokens)
        manager->Unregister(t);
}

TEST_CASE("All callbacks unregistered in reverse order — no crashes or missed callbacks mid-way", "[EdgeCase]")
{
    auto manager = EventManager::Make();
    std::vector<int> log;
    constexpr int kCount = 10;

    std::vector<EventManager::Token> tokens;
    tokens.reserve(kCount);

    for (int i = 0; i < kCount; ++i)
        tokens.push_back(manager->Register<DamageEvent>(MakeLoggingCallback<DamageEvent>(log)));

    // Unregister in reverse
    for (int i = kCount - 1; i >= 0; --i)
        manager->Unregister(tokens[i]);

    manager->Notify<DamageEvent>(DamageEvent{99});
    REQUIRE(log.empty());
}