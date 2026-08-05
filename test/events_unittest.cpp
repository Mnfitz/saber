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

#include "saber/events/event_manager.hpp"

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

struct TestSender
{
    int mId{};
};

static TestSender sender{};

// Convenience alias
using namespace saber::events;

// ============================================================================
// Helpers
// ============================================================================

template<typename EventType>
std::any MakeEventArgs(const TestSender& inSender, const EventType& inEvent)
{
    return std::any{std::tuple<const TestSender&, const EventType&>{inSender, inEvent}};
}

template<typename EventType>
EventCallback MakeLoggingCallback(std::vector<int>& outLog)
{
    return EventCallback::Make<TestSender, EventType>([&outLog](const TestSender&, const EventType& inEvent) -> int
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
        auto cb = EventCallback::Make<TestSender, DamageEvent>([&received](const TestSender&, const DamageEvent& e) -> int
        {
            received = e.mAmount;
            return 0;
        });

        DamageEvent ev{42};
        cb(MakeEventArgs(sender, ev));

        REQUIRE(received == 42);
    }

    SECTION("Return value from lambda is forwarded through operator()")
    {
        auto cb = EventCallback::Make<TestSender, DamageEvent>([](const TestSender&, const DamageEvent&) -> int
        {
            return 99;
        });

        DamageEvent ev{0};
        const int result = cb(MakeEventArgs(sender, ev));
        REQUIRE(result == 99);
    }

    SECTION("operator() is const — callable on a const EventCallback")
    {
        const auto cb = EventCallback::Make<TestSender, DamageEvent>([](const TestSender&, const DamageEvent& e) -> int
        {
            return e.mAmount;
        });

        DamageEvent ev{7};
        REQUIRE(cb(MakeEventArgs(sender, ev)) == 7);
    }

    SECTION("Callback captures mutable state correctly across multiple calls")
    {
        int callCount = 0;
        auto cb = EventCallback::Make<TestSender, DamageEvent>([&callCount](const TestSender&, const DamageEvent&) -> int
        {
            ++callCount;
            return 0;
        });

        DamageEvent ev{};
        cb(MakeEventArgs(sender, ev));
        cb(MakeEventArgs(sender, ev));
        cb(MakeEventArgs(sender, ev));

        REQUIRE(callCount == 3);
    }

    SECTION("EventCallback is copy-constructible (Observe semantics)")
    {
        int received = 0;
        auto original = EventCallback::Make<TestSender, DamageEvent>([&received](const TestSender&, const DamageEvent& e) -> int
        {
            received = e.mAmount;
            return 0;
        });

        EventCallback copy = original;
        DamageEvent ev{55};
        copy(MakeEventArgs(sender, ev));

        REQUIRE(received == 55);
    }

    SECTION("EventCallback is move-constructible (Consume semantics)")
    {
        int received = 0;
        auto original = EventCallback::Make<TestSender, DamageEvent>([&received](const TestSender&, const DamageEvent& e) -> int
        {
            received = e.mAmount;
            return 0;
        });

        EventCallback moved = std::move(original);
        DamageEvent ev{88};
        moved(MakeEventArgs(sender, ev));

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

    auto token = manager->Register(MakeLoggingCallback<DamageEvent>(log));

    manager->Notify(sender, DamageEvent{10});

    REQUIRE(log.size() == 1);
    REQUIRE(log[0] == 10);

    manager->Unregister(token);
}

TEST_CASE("Notify dispatches to multiple callbacks registered for the same event type", "[Register][Notify]")
{
    auto manager = EventManager::Make();
    std::vector<int> logA, logB;

    auto tA = manager->Register(MakeLoggingCallback<DamageEvent>(logA));
    auto tB = manager->Register(MakeLoggingCallback<DamageEvent>(logB));

    manager->Notify(sender, DamageEvent{25});

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

    auto tDmg  = manager->Register(MakeLoggingCallback<DamageEvent>(damageLog));
    auto tHeal = manager->Register(MakeLoggingCallback<HealEvent>(healLog));

    manager->Notify(sender, DamageEvent{5});

    REQUIRE(damageLog.size() == 1);
    REQUIRE(healLog.empty());

    manager->Unregister(tDmg);
    manager->Unregister(tHeal);
}

TEST_CASE("Notify with no registered callbacks is a no-op", "[Notify]")
{
    auto manager = EventManager::Make();
    REQUIRE_NOTHROW(manager->Notify(sender, DamageEvent{99}));
}

TEST_CASE("Notify delivers correct event data (field values preserved)", "[Notify]")
{
    auto manager = EventManager::Make();
    int capturedAmount = -1;

    auto token = manager->Register(
        EventCallback::Make<TestSender, DamageEvent>([&capturedAmount](const TestSender&, const DamageEvent& e) -> int
        {
            capturedAmount = e.mAmount;
            return 0;
        }));

    manager->Notify(sender, DamageEvent{1337});

    REQUIRE(capturedAmount == 1337);
    manager->Unregister(token);
}

TEST_CASE("Notify can fire the same event type multiple times", "[Notify]")
{
    auto manager = EventManager::Make();
    std::vector<int> log;
    auto token = manager->Register(MakeLoggingCallback<DamageEvent>(log));

    manager->Notify(sender, DamageEvent{1});
    manager->Notify(sender, DamageEvent{2});
    manager->Notify(sender, DamageEvent{3});

    REQUIRE(log == std::vector<int>{1, 2, 3});
    manager->Unregister(token);
}

TEST_CASE("Notify can fire different event types independently", "[Notify]")
{
    auto manager = EventManager::Make();
    std::vector<int> damageLog, healLog;

    auto tDmg  = manager->Register(MakeLoggingCallback<DamageEvent>(damageLog));
    auto tHeal = manager->Register(MakeLoggingCallback<HealEvent>(healLog));

    manager->Notify(sender, DamageEvent{10});
    manager->Notify(sender, HealEvent{20});

    REQUIRE(damageLog == std::vector<int>{10});
    REQUIRE(healLog == std::vector<int>{20});

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

    auto cb = EventCallback::Make<TestSender, DamageEvent>([&fired](const TestSender&, const DamageEvent&) -> int
    {
        fired = true;
        return 0;
    });

    auto token = manager->Register(std::move(cb));
    manager->Notify(sender, DamageEvent{});

    REQUIRE(fired);
    manager->Unregister(token);
}

TEST_CASE("Register const-ref overload (observe) — callback fires correctly", "[Register]")
{
    auto manager = EventManager::Make();
    bool fired = false;

    const auto cb = EventCallback::Make<TestSender, DamageEvent>([&fired](const TestSender&, const DamageEvent&) -> int
    {
        fired = true;
        return 0;
    });

    auto token = manager->Register(cb);
    manager->Notify(sender, DamageEvent{});

    REQUIRE(fired);
    manager->Unregister(token);
}

TEST_CASE("Register const-ref overload does not consume the original callback", "[Register]")
{
    auto manager = EventManager::Make();
    int callCount = 0;

    const auto cb = EventCallback::Make<TestSender, DamageEvent>([&callCount](const TestSender&, const DamageEvent&) -> int
    {
        ++callCount;
        return 0;
    });

    auto token = manager->Register(cb);

    cb(MakeEventArgs(sender, DamageEvent{}));

    manager->Notify(sender, DamageEvent{});

    REQUIRE(callCount == 2);
    manager->Unregister(token);
}

TEST_CASE("Register returns distinct tokens for each registration", "[Register]")
{
    auto manager = EventManager::Make();
    std::vector<int> log;

    auto t1 = manager->Register(MakeLoggingCallback<DamageEvent>(log));
    auto t2 = manager->Register(MakeLoggingCallback<DamageEvent>(log));
    auto t3 = manager->Register(MakeLoggingCallback<DamageEvent>(log));

    REQUIRE(t1 != t2);
    REQUIRE(t2 != t3);
    REQUIRE(t1 != t3);

    manager->Unregister(t1);
    manager->Unregister(t2);
    manager->Unregister(t3);
}

TEST_CASE("Same callback can be registered multiple times, each fires independently", "[Register]")
{
    auto manager = EventManager::Make();
    int callCount = 0;

    const auto cb = EventCallback::Make<TestSender, DamageEvent>([&callCount](const TestSender&, const DamageEvent&) -> int
    {
        ++callCount;
        return 0;
    });

    auto t1 = manager->Register(cb);
    auto t2 = manager->Register(cb);

    manager->Notify(sender, DamageEvent{});

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

    auto token = manager->Register(MakeLoggingCallback<DamageEvent>(log));
    manager->Notify(sender, DamageEvent{1});

    manager->Unregister(token);
    manager->Notify(sender, DamageEvent{2});

    REQUIRE(log.size() == 1);
    REQUIRE(log[0] == 1);
}

TEST_CASE("Unregistering one token does not affect other registered callbacks", "[Unregister]")
{
    auto manager = EventManager::Make();
    std::vector<int> logA, logB;

    auto tA = manager->Register(MakeLoggingCallback<DamageEvent>(logA));
    auto tB = manager->Register(MakeLoggingCallback<DamageEvent>(logB));

    manager->Unregister(tA);

    manager->Notify(sender, DamageEvent{7});

    REQUIRE(logA.empty());
    REQUIRE(logB.size() == 1);
    REQUIRE(logB[0] == 7);

    manager->Unregister(tB);
}

TEST_CASE("Unregister of the only remaining callback leaves manager in clean notify-safe state", "[Unregister]")
{
    auto manager = EventManager::Make();
    std::vector<int> log;

    auto token = manager->Register(MakeLoggingCallback<DamageEvent>(log));
    manager->Unregister(token);

    REQUIRE_NOTHROW(manager->Notify(sender, DamageEvent{42}));
    REQUIRE(log.empty());
}

TEST_CASE("Unregister is a no-op for an already-removed token (double unregister safety)", "[Unregister]")
{
    auto manager = EventManager::Make();
    std::vector<int> log;

    auto token = manager->Register(MakeLoggingCallback<DamageEvent>(log));
    manager->Unregister(token);

    REQUIRE_NOTHROW(manager->Unregister(token));
}

TEST_CASE("Unregister middle element — remaining callbacks still fire", "[Unregister]")
{
    auto manager = EventManager::Make();
    std::vector<int> logA, logB, logC;

    auto tA = manager->Register(MakeLoggingCallback<DamageEvent>(logA));
    auto tB = manager->Register(MakeLoggingCallback<DamageEvent>(logB));
    auto tC = manager->Register(MakeLoggingCallback<DamageEvent>(logC));

    manager->Unregister(tB);

    manager->Notify(sender, DamageEvent{3});

    REQUIRE(logA.size() == 1);
    REQUIRE(logB.empty());
    REQUIRE(logC.size() == 1);

    manager->Unregister(tA);
    manager->Unregister(tC);
}

TEST_CASE("Unregister first element — remaining callbacks still fire", "[Unregister]")
{
    auto manager = EventManager::Make();
    std::vector<int> logA, logB;

    auto tA = manager->Register(MakeLoggingCallback<DamageEvent>(logA));
    auto tB = manager->Register(MakeLoggingCallback<DamageEvent>(logB));

    manager->Unregister(tA);

    manager->Notify(sender, DamageEvent{9});

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

    auto t1 = manager->Register(MakeLoggingCallback<DamageEvent>(log));
    manager->Notify(sender, DamageEvent{1});
    manager->Unregister(t1);

    auto t2 = manager->Register(MakeLoggingCallback<DamageEvent>(log));
    manager->Notify(sender, DamageEvent{2});
    manager->Unregister(t2);

    REQUIRE(log == std::vector<int>{1, 2});
}

TEST_CASE("Interleaved register/unregister across event types is isolated", "[Register][Unregister]")
{
    auto manager = EventManager::Make();
    std::vector<int> damageLog, healLog;

    auto tDmg  = manager->Register(MakeLoggingCallback<DamageEvent>(damageLog));
    auto tHeal = manager->Register(MakeLoggingCallback<HealEvent>(healLog));

    manager->Unregister(tDmg);

    manager->Notify(sender, DamageEvent{5});
    manager->Notify(sender, HealEvent{10});

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

    auto token = manager->Register(
        EventCallback::Make<TestSender, EmptyEvent>([&fired](const TestSender&, const EmptyEvent&) -> int
        {
            fired = true;
            return 0;
        }));

    manager->Notify<TestSender, EmptyEvent>(sender, EmptyEvent{});

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
        tokens.push_back(manager->Register(
            EventCallback::Make<TestSender, DamageEvent>([&totalFired](const TestSender&, const DamageEvent&) -> int
            {
                ++totalFired;
                return 0;
            })));
    }

    manager->Notify(sender, DamageEvent{1});
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
        tokens.push_back(manager->Register(MakeLoggingCallback<DamageEvent>(log)));

    for (int i = kCount - 1; i >= 0; --i)
        manager->Unregister(tokens[i]);

    manager->Notify(sender, DamageEvent{99});
    REQUIRE(log.empty());
}

// ============================================================================
// SECTION: EventManager::TokenHandler (RAII registration)
// ============================================================================

TEST_CASE("TokenHandler unregisters on scope exit", "[TokenHandler]")
{
    auto manager = EventManager::Make();
    std::vector<int> log;

    SECTION("Callback fires while handler is alive, and not after it dies")
    {
        {
            auto handler = manager->Register(UsingRAII{}, MakeLoggingCallback<DamageEvent>(log));
            REQUIRE(static_cast<bool>(handler));

            manager->Notify(sender, DamageEvent{7});
            REQUIRE(log == std::vector<int>{7});
        } // handler dtor: Unregister()

        manager->Notify(sender, DamageEvent{8});
        REQUIRE(log == std::vector<int>{7}); // Unchanged
    }

    SECTION("Reset() unregisters early and is idempotent")
    {
        auto handler = manager->Register(UsingRAII{}, MakeLoggingCallback<DamageEvent>(log));

        handler.Reset();
        REQUIRE(!static_cast<bool>(handler));
        REQUIRE(handler.Get() == nullptr);

        handler.Reset(); // Second Reset() must be a NOP
        manager->Notify(sender, DamageEvent{9});
        REQUIRE(log.empty());
    }

    SECTION("TokenHandler is movable but not copyable")
    {
        using TokenHandler = EventManager::TokenHandler;
        STATIC_REQUIRE(!std::is_copy_constructible_v<TokenHandler>);
        STATIC_REQUIRE(!std::is_copy_assignable_v<TokenHandler>);
        STATIC_REQUIRE(std::is_move_constructible_v<TokenHandler>);
        STATIC_REQUIRE(std::is_move_assignable_v<TokenHandler>);

        // No heap allocation: the Token lives inside the handler
        STATIC_REQUIRE(sizeof(TokenHandler) < 64);
    }

    SECTION("Move construct transfers ownership")
    {
        auto handler = manager->Register(UsingRAII{}, MakeLoggingCallback<DamageEvent>(log));
        const auto token = *handler.Get();

        auto moved = std::move(handler);
        REQUIRE(handler.Get() == nullptr);
        REQUIRE(*moved.Get() == token);

        manager->Notify(sender, DamageEvent{1});
        REQUIRE(log == std::vector<int>{1}); // Moved-from handler did not unregister
    }

    SECTION("Move assign unregisters the target's previous registration")
    {
        std::vector<int> otherLog;
        auto first = manager->Register(UsingRAII{}, MakeLoggingCallback<DamageEvent>(log));
        auto second = manager->Register(UsingRAII{}, MakeLoggingCallback<DamageEvent>(otherLog));

        first = std::move(second); // `first`'s registration is dropped here
        REQUIRE(second.Get() == nullptr);

        manager->Notify(sender, DamageEvent{5});
        REQUIRE(log.empty());
        REQUIRE(otherLog == std::vector<int>{5});
    }

    SECTION("Default constructed TokenHandler is empty and harmless")
    {
        EventManager::TokenHandler handler{};
        REQUIRE(!static_cast<bool>(handler));
        REQUIRE(handler.Get() == nullptr);

        handler = manager->Register(UsingRAII{}, MakeLoggingCallback<DamageEvent>(log));
        REQUIRE(static_cast<bool>(handler));
    }
}
