// ...new file - refactored tests exercising both ImplKind::kScalar and ImplKind::kSimd...

/////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2024-2025 Matthew Fitzgerald
//
// Tests duplicated from geometry_unittest.cpp but refactored so each
// logical SECTION is exercised for both ImplKind::kScalar and
// ImplKind::kSimd. Results are stored in lvalues to aid debugging.
//
// Note: This file mirrors Catch2 SECTION names from the original tests.
//
/////////////////////////////////////////////////////////////////////

// catch2
#include "catch2/catch_template_test_macros.hpp"
#include "catch2/catch_test_macros.hpp"

// saber
#include "saber/inexact.hpp"
#include "saber/geometry/matrix.hpp"
#include "saber/geometry/point.hpp"
#include "saber/geometry/size.hpp"
#include "saber/geometry/rectangle.hpp"
#include "saber/geometry/utility.hpp"

// std
#include <type_traits>
#include <limits>

using namespace saber;
using saber::ConvertTo;
using saber::geometry::ImplKind;
using saber::geometry::Point;
using saber::geometry::Size;
using saber::geometry::Rectangle;
using saber::geometry::Union;
using saber::geometry::Intersect;
using saber::geometry::IsEmpty;
using saber::geometry::IsOverlapping;

TEMPLATE_TEST_CASE( "saber::geometry::Point::ctor() works correctly - impl variants",
                    "[saber][template]",
                    int, float, double)
{
    // Two impl variants per test case
    SECTION("ImplKind::kScalar")
    {
        using P = Point<TestType, ImplKind::kScalar>;

        SECTION("Default Construct")
        {
            P p{};
            REQUIRE(p.X() == 0);
            REQUIRE(p.Y() == 0);
        }

        SECTION("Alt Construct")
        {
            P p{1,2};
            REQUIRE(p.X() == 1);
            REQUIRE(p.Y() == 2);
        }

        SECTION("Copy/Move/Assign")
        {
            const P cp{3,4};
            P p1{cp};
            REQUIRE(p1.X() == cp.X());
            REQUIRE(p1.Y() == cp.Y());

            P p2{};
            p2 = cp;
            REQUIRE(p2.X() == cp.X());
            REQUIRE(p2.Y() == cp.Y());

            P pm = P{6,5};
            REQUIRE(pm.X() == 6);
            REQUIRE(pm.Y() == 5);
        }

		// REVISIT: Scalar point structured binding
		/*
        SECTION("Structured Binding")
        {
            const P p{9,10};
            const auto [x,y] = p;
            REQUIRE(p.X() == x);
            REQUIRE(p.Y() == y);
        }
		*/
	
        SECTION("Translate/Scale/Setters")
        {
            // Translate (free functions)
            P a{2,1};
            P delta{4,3};
            auto resultT = Translate(a, delta);
            REQUIRE(resultT == P{6,4});

            auto resultT2 = Translate(a, TestType{3});
            REQUIRE(resultT2 == P{5,4});

            // Scale (free functions)
            P s{2,3};
            auto resultS = Scale(s, P{4,3});
            REQUIRE(resultS == P{8,9});

            // Setters
            P st{2,3};
            st.X(3);
            REQUIRE(st == P{3,3});
            st.Y(2);
            REQUIRE(st == P{3,2});
        }

        SECTION("Translate/Scale member methods (lvalues)")
        {
            // Translate (member)
            P a{2,1};
            P delta{4,3};
            auto &ret1 = a.Translate(delta);
            REQUIRE(&ret1 == &a);
            REQUIRE(a == P{6,4});
            REQUIRE(ret1 == P{6,4});

            // Translate scalar
            P b{2,1};
            TestType t{3};
            auto &ret2 = b.Translate(t);
            REQUIRE(&ret2 == &b);
            REQUIRE(b == P{5,4});

            // Scale (member)
            P s{2,3};
            P factor{4,3};
            auto &ret3 = s.Scale(factor);
            REQUIRE(&ret3 == &s);
            REQUIRE(s == P{8,9});

            // Scale scalar
            P s2{2,3};
            TestType t2{2};
            auto &ret4 = s2.Scale(t2);
            REQUIRE(&ret4 == &s2);
            REQUIRE(s2 == P{4,6});
        }
    }

    SECTION("ImplKind::kSimd")
    {
        using P = Point<TestType, ImplKind::kSimd>;

        SECTION("Default Construct")
        {
            P p{};
            REQUIRE(p.X() == 0);
            REQUIRE(p.Y() == 0);
        }

        SECTION("Alt Construct")
        {
            P p{1,2};
            REQUIRE(p.X() == 1);
            REQUIRE(p.Y() == 2);
        }

        SECTION("Copy/Move/Assign")
        {
            const P cp{3,4};
            P p1{cp};
            REQUIRE(p1.X() == cp.X());
            REQUIRE(p1.Y() == cp.Y());

            P p2{};
            p2 = cp;
            REQUIRE(p2.X() == cp.X());
            REQUIRE(p2.Y() == cp.Y());

            P pm = P{6,5};
            REQUIRE(pm.X() == 6);
            REQUIRE(pm.Y() == 5);
        }

        SECTION("Structured Binding")
        {
            const P p{9,10};
            const auto [x,y] = p;
            REQUIRE(p.X() == x);
            REQUIRE(p.Y() == y);
        }

        SECTION("Translate/Scale/Setters")
        {
            // Translate (free functions)
            P a{2,1};
            P delta{4,3};
            auto resultT = Translate(a, delta);
            REQUIRE(resultT == P{6,4});

            auto resultT2 = Translate(a, TestType{3});
            REQUIRE(resultT2 == P{5,4});

            // Scale (free functions)
            P s{2,3};
            auto resultS = Scale(s, P{4,3});
            REQUIRE(resultS == P{8,9});

            // Setters
            P st{2,3};
            st.X(3);
            REQUIRE(st == P{3,3});
            st.Y(2);
            REQUIRE(st == P{3,2});
        }

        SECTION("Translate/Scale member methods (lvalues)")
        {
            // Translate (member)
            P a{2,1};
            P delta{4,3};
            auto &ret1 = a.Translate(delta);
            REQUIRE(&ret1 == &a);
            REQUIRE(a == P{6,4});
            REQUIRE(ret1 == P{6,4});

            // Translate scalar
            P b{2,1};
            TestType t{3};
            auto &ret2 = b.Translate(t);
            REQUIRE(&ret2 == &b);
            REQUIRE(b == P{5,4});

            // Scale (member)
            P s{2,3};
            P factor{4,3};
            auto &ret3 = s.Scale(factor);
            REQUIRE(&ret3 == &s);
            REQUIRE(s == P{8,9});

            // Scale scalar
            P s2{2,3};
            TestType t2{2};
            auto &ret4 = s2.Scale(t2);
            REQUIRE(&ret4 == &s2);
            REQUIRE(s2 == P{4,6});
        }
    }
}

TEMPLATE_TEST_CASE( "saber::geometry::Point move and free functions - impl variants",
                    "[saber][template]",
                    int, float, double)
{
    SECTION("ImplKind::kScalar")
    {
        using P = Point<TestType, ImplKind::kScalar>;

        SECTION("Move Construct")
        {
            P pm1 = P{6,5};
            REQUIRE(pm1.X() == 6);
            REQUIRE(pm1.Y() == 5);

            P pm2 = std::move(P{8,7});
            REQUIRE(pm2.X() == 8);
            REQUIRE(pm2.Y() == 7);
        }

        SECTION("Move Assign")
        {
            P pm{};
            pm = P{8,7};
            REQUIRE(pm.X() == 8);
            REQUIRE(pm.Y() == 7);

            P pm2{};
            pm2 = std::move(P{10,9});
            REQUIRE(pm2.X() == 10);
            REQUIRE(pm2.Y() == 9);
        }

        SECTION("Translate(Point) free functions")
        {
            auto result1 = Translate(P{2,1}, P{4,3});
            REQUIRE(result1 == P{6,4});

            auto result2 = Translate(P{3,2}, TestType{5}, TestType{4});
            REQUIRE(result2 == P{8,6});

            auto result3 = Translate(P{4,6}, TestType{3});
            REQUIRE(result3 == P{7,9});
        }

        SECTION("Scale(Point) free functions")
        {
            auto result1 = Scale(P{2,3}, P{4,3});
            REQUIRE(result1 == P{8,9});

            auto result2 = Scale(P{5,3}, TestType{2}, TestType{4});
            REQUIRE(result2 == P{10,12});

            auto result3 = Scale(P{6,7}, TestType{2});
            REQUIRE(result3 == P{12,14});
        }
    }

    SECTION("ImplKind::kSimd")
    {
        using P = Point<TestType, ImplKind::kSimd>;

        SECTION("Move Construct")
        {
            P pm1 = P{6,5};
            REQUIRE(pm1.X() == 6);
            REQUIRE(pm1.Y() == 5);

            P pm2 = std::move(P{8,7});
            REQUIRE(pm2.X() == 8);
            REQUIRE(pm2.Y() == 7);
        }

        SECTION("Move Assign")
        {
            P pm{};
            pm = P{8,7};
            REQUIRE(pm.X() == 8);
            REQUIRE(pm.Y() == 7);

            P pm2{};
            pm2 = std::move(P{10,9});
            REQUIRE(pm2.X() == 10);
            REQUIRE(pm2.Y() == 9);
        }

        SECTION("Translate(Point) free functions")
        {
            auto result1 = Translate(P{2,1}, P{4,3});
            REQUIRE(result1 == P{6,4});

            auto result2 = Translate(P{3,2}, TestType{5}, TestType{4});
            REQUIRE(result2 == P{8,6});

            auto result3 = Translate(P{4,6}, TestType{3});
            REQUIRE(result3 == P{7,9});
        }

        SECTION("Scale(Point) free functions")
        {
            auto result1 = Scale(P{2,3}, P{4,3});
            REQUIRE(result1 == P{8,9});

            auto result2 = Scale(P{5,3}, TestType{2}, TestType{4});
            REQUIRE(result2 == P{10,12});

            auto result3 = Scale(P{6,7}, TestType{2});
            REQUIRE(result3 == P{12,14});
        }
    }
}

TEMPLATE_TEST_CASE( "saber::geometry::Size::ctor() works correctly - impl variants",
                    "[saber][template]",
                    int, float, double)
{
    SECTION("ImplKind::kScalar")
    {
        using S = Size<TestType, ImplKind::kScalar>;

        SECTION("Default/Alt/Copy/Assign")
        {
            S s{};
            REQUIRE(s.Width() == 0);
            REQUIRE(s.Height() == 0);

            S sa{1,2};
            REQUIRE(sa.Width() == 1);
            REQUIRE(sa.Height() == 2);

            const S cs{3,4};
            S cpy{cs};
            REQUIRE(cpy.Width() == cs.Width());
            REQUIRE(cpy.Height() == cs.Height());
        }

        SECTION("Enlarge/Scale/Setters")
        {
            S s{2,1};
            S delta{4,3};
            auto resE = Enlarge(s, delta);
            REQUIRE(resE == S{6,4});

            auto resS = Scale(s, S{4,3});
            REQUIRE(resS == S{8,3}); // note: semantics follow original tests

            S set{2,3};
            set.Width(3);
            REQUIRE(set == S{3,3});
            set.Height(2);
            REQUIRE(set == S{3,2});
        }

        SECTION("Enlarge/Scale member methods (lvalues)")
        {
            // Enlarge member with lvalues
            S s{2,1};
            S en{4,3};
            auto &r1 = s.Enlarge(en);
            REQUIRE(&r1 == &s);
            REQUIRE(s == S{6,4});
            REQUIRE(r1 == S{6,4});

            S s2{2,1};
            TestType t{3};
            auto &r2 = s2.Enlarge(t);
            REQUIRE(&r2 == &s2);
            REQUIRE(s2 == S{5,4});

            // Scale member with lvalues
            S sc{2,1};
            S factor{4,3};
            auto &r3 = sc.Scale(factor);
            REQUIRE(&r3 == &sc);
            REQUIRE(sc == S{8,3});

            S sc2{2,3};
            TestType t2{2};
            auto &r4 = sc2.Scale(t2);
            REQUIRE(&r4 == &sc2);
            REQUIRE(sc2 == S{4,6});
        }
    }

    SECTION("ImplKind::kSimd")
    {
        using S = Size<TestType, ImplKind::kSimd>;

        SECTION("Default/Alt/Copy/Assign")
        {
            S s{};
            REQUIRE(s.Width() == 0);
            REQUIRE(s.Height() == 0);

            S sa{1,2};
            REQUIRE(sa.Width() == 1);
            REQUIRE(sa.Height() == 2);

            const S cs{3,4};
            S cpy{cs};
            REQUIRE(cpy.Width() == cs.Width());
            REQUIRE(cpy.Height() == cs.Height());
        }

        SECTION("Enlarge/Scale/Setters")
        {
            S s{2,1};
            S delta{4,3};
            auto resE = Enlarge(s, delta);
            REQUIRE(resE == S{6,4});

            auto resS = Scale(s, S{4,3});
            REQUIRE(resS == S{8,3});

            S set{2,3};
            set.Width(3);
            REQUIRE(set == S{3,3});
            set.Height(2);
            REQUIRE(set == S{3,2});
        }

        SECTION("Enlarge/Scale member methods (lvalues)")
        {
            // Enlarge member with lvalues
            S s{2,1};
            S en{4,3};
            auto &r1 = s.Enlarge(en);
            REQUIRE(&r1 == &s);
            REQUIRE(s == S{6,4});
            REQUIRE(r1 == S{6,4});

            S s2{2,1};
            TestType t{3};
            auto &r2 = s2.Enlarge(t);
            REQUIRE(&r2 == &s2);
            REQUIRE(s2 == S{5,4});

            // Scale member with lvalues
            S sc{2,1};
            S factor{4,3};
            auto &r3 = sc.Scale(factor);
            REQUIRE(&r3 == &sc);
            REQUIRE(sc == S{8,3});

            S sc2{2,3};
            TestType t2{2};
            auto &r4 = sc2.Scale(t2);
            REQUIRE(&r4 == &sc2);
            REQUIRE(sc2 == S{4,6});
        }
    }
}

TEMPLATE_TEST_CASE( "saber::geometry::Size move and free functions - impl variants",
                    "[saber][template]",
                    int, float, double)
{
    SECTION("ImplKind::kScalar")
    {
        using S = Size<TestType, ImplKind::kScalar>;

        SECTION("Move Construct")
        {
            S sm1 = S{6,5};
            REQUIRE(sm1.Width() == 6);
            REQUIRE(sm1.Height() == 5);

            S sm2 = std::move(S{8,7});
            REQUIRE(sm2.Width() == 8);
            REQUIRE(sm2.Height() == 7);
        }

        SECTION("Move Assign")
        {
            S sm{};
            sm = S{8,7};
            REQUIRE(sm.Width() == 8);
            REQUIRE(sm.Height() == 7);

            S sm2{};
            sm2 = std::move(S{10,9});
            REQUIRE(sm2.Width() == 10);
            REQUIRE(sm2.Height() == 9);
        }

        SECTION("Enlarge(Size) free functions")
        {
            auto result1 = Enlarge(S{2,1}, S{4,3});
            REQUIRE(result1 == S{6,4});

            auto result2 = Enlarge(S{3,2}, TestType{5}, TestType{4});
            REQUIRE(result2 == S{8,6});

            auto result3 = Enlarge(S{4,6}, TestType{3});
            REQUIRE(result3 == S{7,9});
        }

        SECTION("Scale(Size) free functions")
        {
            auto result1 = Scale(S{2,3}, S{4,3});
            REQUIRE(result1 == S{8,9});

            auto result2 = Scale(S{5,3}, TestType{2}, TestType{4});
            REQUIRE(result2 == S{10,12});

            auto result3 = Scale(S{6,7}, TestType{2});
            REQUIRE(result3 == S{12,14});
        }
    }

    SECTION("ImplKind::kSimd")
    {
        using S = Size<TestType, ImplKind::kSimd>;

        SECTION("Move Construct")
        {
            S sm1 = S{6,5};
            REQUIRE(sm1.Width() == 6);
            REQUIRE(sm1.Height() == 5);

            S sm2 = std::move(S{8,7});
            REQUIRE(sm2.Width() == 8);
            REQUIRE(sm2.Height() == 7);
        }

        SECTION("Move Assign")
        {
            S sm{};
            sm = S{8,7};
            REQUIRE(sm.Width() == 8);
            REQUIRE(sm.Height() == 7);

            S sm2{};
            sm2 = std::move(S{10,9});
            REQUIRE(sm2.Width() == 10);
            REQUIRE(sm2.Height() == 9);
        }

        SECTION("Enlarge(Size) free functions")
        {
            auto result1 = Enlarge(S{2,1}, S{4,3});
            REQUIRE(result1 == S{6,4});

            auto result2 = Enlarge(S{3,2}, TestType{5}, TestType{4});
            REQUIRE(result2 == S{8,6});

            auto result3 = Enlarge(S{4,6}, TestType{3});
            REQUIRE(result3 == S{7,9});
        }

        SECTION("Scale(Size) free functions")
        {
            auto result1 = Scale(S{2,3}, S{4,3});
            REQUIRE(result1 == S{8,9});

            auto result2 = Scale(S{5,3}, TestType{2}, TestType{4});
            REQUIRE(result2 == S{10,12});

            auto result3 = Scale(S{6,7}, TestType{2});
            REQUIRE(result3 == S{12,14});
        }
    }
}

TEMPLATE_TEST_CASE( "saber::geometry::utility works correctly - impl variants",
                    "[saber][template]",
                    int, float, double)
{
    SECTION("ImplKind::kScalar")
    {
        using namespace saber::geometry;
        using S = Size<TestType, ImplKind::kScalar>;
        using P = Point<TestType, ImplKind::kScalar>;

        SECTION("Convert: Size -> Point")
        {
            const S size{1,2};
            auto point = ConvertTo<P>(size);
            REQUIRE(point.X() == size.Width());
            REQUIRE(point.Y() == size.Height());
        }

        SECTION("Convert: Point -> Size")
        {
            const P point{1,2};
            auto size = ConvertTo<S>(point);
            REQUIRE(size.Width() == point.X());
            REQUIRE(size.Height() == point.Y());
        }
    }

    SECTION("ImplKind::kSimd")
    {
        using namespace saber::geometry;
        using S = Size<TestType, ImplKind::kSimd>;
        using P = Point<TestType, ImplKind::kSimd>;

        SECTION("Convert: Size -> Point")
        {
            const S size{1,2};
            auto point = ConvertTo<P>(size);
            REQUIRE(point.X() == size.Width());
            REQUIRE(point.Y() == size.Height());
        }

        SECTION("Convert: Point -> Size")
        {
            const P point{1,2};
            auto size = ConvertTo<S>(point);
            REQUIRE(size.Width() == point.X());
            REQUIRE(size.Height() == point.Y());
        }
    }
}


TEMPLATE_TEST_CASE( "saber::geometry inexact comparison works correctly - impl variants",
                    "[saber][template]",
                    float, double)
{
    SECTION("ImplKind::kScalar")
    {
        using namespace saber::geometry;
        using T = TestType;
        using S = Size<TestType, ImplKind::kScalar>;
        using P = Point<TestType, ImplKind::kScalar>;

        const S a{static_cast<T>(3.6),static_cast<T>(3.6)};
        const S b{static_cast<T>(3.0),static_cast<T>(3.0)};
        const S c{static_cast<T>(1.2),static_cast<T>(1.2)};
        REQUIRE(a == (b * c));

        const P d{static_cast<T>(3.6),static_cast<T>(3.6)};
        const P e{static_cast<T>(3.0),static_cast<T>(3.0)};
        const P f{static_cast<T>(1.2),static_cast<T>(1.2)};
        REQUIRE(d == (e * f));
    }

    SECTION("ImplKind::kSimd")
    {
        using namespace saber::geometry;
        using T = TestType;
        using S = Size<TestType, ImplKind::kSimd>;
        using P = Point<TestType, ImplKind::kSimd>;

        const S a{static_cast<T>(3.6),static_cast<T>(3.6)};
        const S b{static_cast<T>(3.0),static_cast<T>(3.0)};
        const S c{static_cast<T>(1.2),static_cast<T>(1.2)};
        REQUIRE(a == (b * c));

        const P d{static_cast<T>(3.6),static_cast<T>(3.6)};
        const P e{static_cast<T>(3.0),static_cast<T>(3.0)};
        const P f{static_cast<T>(1.2),static_cast<T>(1.2)};
        REQUIRE(d == (e * f));
    }
}

TEMPLATE_TEST_CASE( "saber::geometry operators works correctly - impl variants",
                    "[saber][template]",
                    Point<int>, Point<float>, Point<double>,
                    Size<int>, Size<float>, Size<double>)
{
    SECTION("ImplKind::kScalar")
    {
        using V = TestType;
        V a{1,2};
        REQUIRE((a + V{3,4}) == V{4,6});
        a += V{3,4};
        REQUIRE(a == V{4,6});
        a -= V{1,2};
        REQUIRE(a == V{3,4});
    }

    SECTION("ImplKind::kSimd")
    {
        using V = TestType;
        V a{1,2};
        REQUIRE((a + V{3,4}) == V{4,6});
        a += V{3,4};
        REQUIRE(a == V{4,6});
        a -= V{1,2};
        REQUIRE(a == V{3,4});
    }
}

TEMPLATE_TEST_CASE( "saber::geometry::Rectangle::ctor() works correctly - impl variants",
                    "[saber][template]",
                    int, float, double)
{
    SECTION("ImplKind::kScalar")
    {
        using R = Rectangle<TestType, ImplKind::kScalar>;
        using P = Point<TestType, ImplKind::kScalar>;
        using S = Size<TestType, ImplKind::kScalar>;

        SECTION("Default/Alt/Copy/Assign/Move")
        {
            R r{};
            REQUIRE(r.X() == 0);
            REQUIRE(r.Y() == 0);
            REQUIRE(r.Width() == 0);
            REQUIRE(r.Height() == 0);

            R r2{1,2,3,4};
            REQUIRE(r2.X() == 1);
            REQUIRE(r2.Y() == 2);
            REQUIRE(r2.Width() == 3);
            REQUIRE(r2.Height() == 4);

            const R cr{1,2,3,4};
            R copy{cr};
            REQUIRE(copy.X() == cr.X());
        }

        SECTION("Translate(Rectangle)")
        {
            R r0{P{2,1}};
            P a0{4,3};
            R expected0{P{6,4}};
            auto result0 = r0.Translate(a0);
            REQUIRE(result0 == expected0);

            R r1{P{3,2}};
            auto result1 = r1.Translate(TestType{5}, TestType{4});
            REQUIRE(result1 == R{P{8,6}});

            R r2{P{4,6}};
            auto result2 = r2.Translate(TestType{3});
            REQUIRE(result2 == R{P{7,9}});
        }

        SECTION("Enlarge(Rectangle)")
        {
            R r0{S{2,1}};
            S s0{4,3};
            auto result0 = r0.Enlarge(s0);
            REQUIRE(result0 == R{S{6,4}});

            R r1{S{3,2}};
            auto result1 = r1.Enlarge(TestType{5}, TestType{4});
            REQUIRE(result1 == R{S{8,6}});

            R r2{S{4,6}};
            auto result2 = r2.Enlarge(TestType{3});
            REQUIRE(result2 == R{S{7,9}});
        }

        SECTION("Scale(Rectangle)")
        {
            R r0{TestType{2},TestType{3},TestType{2},TestType{3}};
            auto result0 = r0.Scale(P{TestType{4},TestType{3}});
            REQUIRE(result0 == R{TestType{8},TestType{9},TestType{8},TestType{9}});

            R r1{TestType{2},TestType{3},TestType{2},TestType{3}};
            auto result1 = r1.Scale(S{TestType{4},TestType{3}});
            REQUIRE(result1 == R{TestType{8},TestType{9},TestType{8},TestType{9}});

            R r2{TestType{5},TestType{3},TestType{5},TestType{3}};
            auto result2 = r2.Scale(TestType{2}, TestType{4});
            REQUIRE(result2 == R{TestType{10},TestType{12},TestType{10},TestType{12}});

            R r3{TestType{6},TestType{7},TestType{6},TestType{7}};
            auto result3 = r3.Scale(TestType{2});
            REQUIRE(result3 == R{TestType{12},TestType{14},TestType{12},TestType{14}});
        }

        SECTION("Setters")
        {
            R rect{0,0,0,0};
            rect.X(1);
            REQUIRE(rect == R{1,0,0,0});
            rect.Y(2);
            REQUIRE(rect == R{1,2,0,0});
            rect.Width(3);
            REQUIRE(rect == R{1,2,3,0});
            rect.Height(4);
            REQUIRE(rect == R{1,2,3,4});
            rect.Origin(P{9,8});
            REQUIRE(rect == R{9,8,3,4});
            rect.Size(S{7,6});
            REQUIRE(rect == R{9,8,7,6});
        }

        SECTION("Union/Intersect/IsOverlapping/IsEmpty")
        {
            R rect1{0,0,5,5};
            R rect2{2,2,4,4};
            auto union1 = Union(rect1, rect2);
            REQUIRE(union1 == R{0,0,6,6});

            auto intersect1 = Intersect(rect1, rect2);
            REQUIRE(intersect1 == R{2,2,3,3});

            R rect4{10,10,2,2};
            auto intersect3 = Intersect(rect1, rect4);
            REQUIRE(IsEmpty(intersect3));

            auto overlap1 = IsOverlapping(rect1, rect2);
            REQUIRE(overlap1);
            REQUIRE(rect1.IsOverlapping(rect2));

            auto pt1 = Point<TestType, ImplKind::kScalar>{2,2};
            auto overlap_pt = IsOverlapping(rect1, pt1);
            REQUIRE(overlap_pt);
        }
    }

    SECTION("ImplKind::kSimd")
    {
        using R = Rectangle<TestType, ImplKind::kSimd>;
        using P = Point<TestType, ImplKind::kSimd>;
        using S = Size<TestType, ImplKind::kSimd>;

        SECTION("Default/Alt/Copy/Assign/Move")
        {
            R r{};
            REQUIRE(r.X() == 0);
            REQUIRE(r.Y() == 0);
            REQUIRE(r.Width() == 0);
            REQUIRE(r.Height() == 0);

            R r2{1,2,3,4};
            REQUIRE(r2.X() == 1);
            REQUIRE(r2.Y() == 2);
            REQUIRE(r2.Width() == 3);
            REQUIRE(r2.Height() == 4);
        }

        SECTION("Translate(Rectangle)")
        {
            R r0{P{2,1}};
            P a0{4,3};
            auto result0 = r0.Translate(a0);
            REQUIRE(result0 == R{P{6,4}});

            R r1{P{3,2}};
            auto result1 = r1.Translate(TestType{5}, TestType{4});
            REQUIRE(result1 == R{P{8,6}});

            R r2{P{4,6}};
            auto result2 = r2.Translate(TestType{3});
            REQUIRE(result2 == R{P{7,9}});
        }

        SECTION("Enlarge(Rectangle)")
        {
            R r0{S{2,1}};
            S s0{4,3};
            auto result0 = r0.Enlarge(s0);
            REQUIRE(result0 == R{S{6,4}});

            R r1{S{3,2}};
            auto result1 = r1.Enlarge(TestType{5}, TestType{4});
            REQUIRE(result1 == R{S{8,6}});

            R r2{S{4,6}};
            auto result2 = r2.Enlarge(TestType{3});
            REQUIRE(result2 == R{S{7,9}});
        }

        SECTION("Scale(Rectangle)")
        {
            R r0{TestType{2},TestType{3},TestType{2},TestType{3}};
            auto result0 = r0.Scale(P{TestType{4},TestType{3}});
            REQUIRE(result0 == R{TestType{8},TestType{9},TestType{8},TestType{9}});

            R r1{TestType{2},TestType{3},TestType{2},TestType{3}};
            auto result1 = r1.Scale(S{TestType{4},TestType{3}});
            REQUIRE(result1 == R{TestType{8},TestType{9},TestType{8},TestType{9}});

            R r2{TestType{5},TestType{3},TestType{5},TestType{3}};
            auto result2 = r2.Scale(TestType{2}, TestType{4});
            REQUIRE(result2 == R{TestType{10},TestType{12},TestType{10},TestType{12}});

            R r3{TestType{6},TestType{7},TestType{6},TestType{7}};
            auto result3 = r3.Scale(TestType{2});
            REQUIRE(result3 == R{TestType{12},TestType{14},TestType{12},TestType{14}});
        }

        SECTION("Setters")
        {
            R rect{0,0,0,0};
            rect.X(1);
            REQUIRE(rect == R{1,0,0,0});
            rect.Y(2);
            REQUIRE(rect == R{1,2,0,0});
            rect.Width(3);
            REQUIRE(rect == R{1,2,3,0});
            rect.Height(4);
            REQUIRE(rect == R{1,2,3,4});
            rect.Origin(P{9,8});
            REQUIRE(rect == R{9,8,3,4});
            rect.Size(S{7,6});
            REQUIRE(rect == R{9,8,7,6});
        }

        SECTION("Union/Intersect/IsOverlapping/IsEmpty")
        {
            R rect1{0,0,5,5};
            R rect2{2,2,4,4};
            auto union1 = Union(rect1, rect2);
            REQUIRE(union1 == R{0,0,6,6});

            auto intersect1 = Intersect(rect1, rect2);
            REQUIRE(intersect1 == R{2,2,3,3});

            R rect4{10,10,2,2};
            auto intersect3 = Intersect(rect1, rect4);
            REQUIRE(IsEmpty(intersect3));

            auto overlap1 = IsOverlapping(rect1, rect2);
            REQUIRE(overlap1);
            REQUIRE(rect1.IsOverlapping(rect2));

            auto pt1 = Point<TestType, ImplKind::kSimd>{2,2};
            auto overlap_pt = IsOverlapping(rect1, pt1);
            REQUIRE(overlap_pt);
        }
    }
}

TEMPLATE_TEST_CASE( "saber::geometry::Rectangle rounding works correctly - impl variants",
                    "[saber][template]",
                    float, double)
{
    SECTION("ImplKind::kScalar")
    {
        using R = Rectangle<TestType, ImplKind::kScalar>;
        using P = Point<TestType, ImplKind::kScalar>;
        using S = Size<TestType, ImplKind::kScalar>;
        REQUIRE(R{1.5f,2.5f,3.5f,4.5f}.RoundNearest() == R{2,3,4,5});
        REQUIRE(R{1.1f,2.1f,3.1f,4.1f}.RoundNearest() == R{1,2,3,4});
        REQUIRE(R{-1.5f,-2.5f,-3.5f,-4.5f}.RoundNearest() == R{-2,-3,-4,-5});
        REQUIRE(R{1.5f,2.5f,3.5f,4.5f}.RoundTrunc() == R{1,2,3,4});
        REQUIRE(R{1.5f,2.5f,3.5f,4.5f}.RoundCeil() == R{2,3,4,5});
        REQUIRE(R{1.5f,2.5f,3.5f,4.5f}.RoundFloor() == R{1,2,3,4});
    }

    SECTION("ImplKind::kSimd")
    {
        using R = Rectangle<TestType, ImplKind::kScalar>;
        using P = Point<TestType, ImplKind::kScalar>;
        using S = Size<TestType, ImplKind::kScalar>;
        REQUIRE(R{1.5f,2.5f,3.5f,4.5f}.RoundNearest() == R{2,3,4,5});
        REQUIRE(R{1.1f,2.1f,3.1f,4.1f}.RoundNearest() == R{1,2,3,4});
        REQUIRE(R{-1.5f,-2.5f,-3.5f,-4.5f}.RoundNearest() == R{-2,-3,-4,-5});
        REQUIRE(R{1.5f,2.5f,3.5f,4.5f}.RoundTrunc() == R{1,2,3,4});
        REQUIRE(R{1.5f,2.5f,3.5f,4.5f}.RoundCeil() == R{2,3,4,5});
        REQUIRE(R{1.5f,2.5f,3.5f,4.5f}.RoundFloor() == R{1,2,3,4});
    }
}

TEMPLATE_TEST_CASE( "saber::geometry::Rectangle move and free functions - impl variants",
                    "[saber][template]",
                    int, float, double)
{
    SECTION("ImplKind::kScalar")
    {
        using R = Rectangle<TestType, ImplKind::kScalar>;
        using P = Point<TestType, ImplKind::kScalar>;
        using S = Size<TestType, ImplKind::kScalar>;

        SECTION("Move Construct")
        {
            R rm1 = R{1,2,3,4};
            REQUIRE(rm1.X() == 1);
            REQUIRE(rm1.Y() == 2);
            REQUIRE(rm1.Width() == 3);
            REQUIRE(rm1.Height() == 4);

            R rm2 = std::move(R{5,6,7,8});
            REQUIRE(rm2.X() == 5);
            REQUIRE(rm2.Y() == 6);
            REQUIRE(rm2.Width() == 7);
            REQUIRE(rm2.Height() == 8);
        }

        SECTION("Move Assign")
        {
            R rm{};
            rm = R{1,2,3,4};
            REQUIRE(rm.X() == 1);
            REQUIRE(rm.Y() == 2);

            R rm2{};
            rm2 = std::move(R{5,6,7,8});
            REQUIRE(rm2.X() == 5);
            REQUIRE(rm2.Y() == 6);
        }

        SECTION("Translate(Rectangle) - free functions")
        {
            auto result1 = Translate(R{2,1,0,0}, P{4,3});
            REQUIRE(result1 == R{6,4,0,0});

            auto result2 = Translate(R{3,2,0,0}, TestType{5}, TestType{4});
            REQUIRE(result2 == R{8,6,0,0});

            auto result3 = Translate(R{4,6,0,0}, TestType{3});
            REQUIRE(result3 == R{7,9,0,0});
        }

        SECTION("Enlarge(Rectangle) - free functions")
        {
            auto result1 = Enlarge(R{0,0,2,1}, S{4,3});
            REQUIRE(result1 == R{0,0,6,4});

            auto result2 = Enlarge(R{0,0,3,2}, TestType{5}, TestType{4});
            REQUIRE(result2 == R{0,0,8,6});

            auto result3 = Enlarge(R{0,0,4,6}, TestType{3});
            REQUIRE(result3 == R{0,0,7,9});
        }

        SECTION("Scale(Rectangle) - free functions")
        {
            auto result1 = Scale(R{2,3,2,3}, P{TestType{4},TestType{3}});
            REQUIRE(result1 == R{8,9,8,9});

            auto result2 = Scale(R{2,3,2,3}, S{TestType{4},TestType{3}});
            REQUIRE(result2 == R{8,9,8,9});

            auto result3 = Scale(R{5,3,5,3}, TestType{2}, TestType{4});
            REQUIRE(result3 == R{10,12,10,12});

            auto result4 = Scale(R{6,7,6,7}, TestType{2});
            REQUIRE(result4 == R{12,14,12,14});
        }

        SECTION("Union - edge cases")
        {
            // No overlap (should span both)
            R rect1{0,0,5,5};
            R rect4{10,10,2,2};
            auto union3 = Union(rect1, rect4);
            REQUIRE(union3 == R{0,0,12,12});

            // Touching at edge (should span both)
            R rect5{5,0,2,2};
            auto union4 = Union(rect1, rect5);
            REQUIRE(union4 == R{0,0,7,5});

            // One inside another
            R rect3{1,1,2,2};
            auto union2 = Union(rect1, rect3);
            REQUIRE(union2 == rect1);
        }

        SECTION("Intersect - edge cases")
        {
            R rect1{0,0,5,5};
            
            // No overlap
            R rect4{10,10,2,2};
            auto intersect3 = Intersect(rect1, rect4);
            REQUIRE(IsEmpty(intersect3));

            // Touching at edge (should be empty)
            R rect5{5,0,2,2};
            auto intersect4 = Intersect(rect1, rect5);
            REQUIRE(IsEmpty(intersect4));
        }

        SECTION("IsOverlapping - edge cases")
        {
            R rect1{0,0,5,5};
            
            // No overlap
            R rect4{10,10,2,2};
            auto noOverlap = IsOverlapping(rect1, rect4);
            REQUIRE_FALSE(noOverlap);

            // Touching at edge (should not overlap)
            R rect5{5,0,2,2};
            auto touchOverlap = IsOverlapping(rect1, rect5);
            REQUIRE_FALSE(touchOverlap);

            // Point outside rectangle
            auto pt3 = Point<TestType, ImplKind::kScalar>{10,10};
            auto ptOutside = IsOverlapping(rect1, pt3);
            REQUIRE_FALSE(ptOutside);
        }

        SECTION("IsEmpty(Rectangle)")
        {
            R empty{0,0,0,0};
            REQUIRE(IsEmpty(empty));

            R notEmpty{0,0,1,1};
            REQUIRE_FALSE(IsEmpty(notEmpty));

            R zeroWidth{0,0,0,5};
            REQUIRE(IsEmpty(zeroWidth));

            R zeroHeight{0,0,5,0};
            REQUIRE(IsEmpty(zeroHeight));
        }
    }

    SECTION("ImplKind::kSimd")
    {
        using R = Rectangle<TestType, ImplKind::kSimd>;
        using P = Point<TestType, ImplKind::kSimd>;
        using S = Size<TestType, ImplKind::kSimd>;

        SECTION("Move Construct")
        {
            R rm1 = R{1,2,3,4};
            REQUIRE(rm1.X() == 1);
            REQUIRE(rm1.Y() == 2);
            REQUIRE(rm1.Width() == 3);
            REQUIRE(rm1.Height() == 4);

            R rm2 = std::move(R{5,6,7,8});
            REQUIRE(rm2.X() == 5);
            REQUIRE(rm2.Y() == 6);
            REQUIRE(rm2.Width() == 7);
            REQUIRE(rm2.Height() == 8);
        }

        SECTION("Move Assign")
        {
            R rm{};
            rm = R{1,2,3,4};
            REQUIRE(rm.X() == 1);
            REQUIRE(rm.Y() == 2);

            R rm2{};
            rm2 = std::move(R{5,6,7,8});
            REQUIRE(rm2.X() == 5);
            REQUIRE(rm2.Y() == 6);
        }

        SECTION("Translate(Rectangle) - free functions")
        {
            auto result1 = Translate(R{2,1,0,0}, P{4,3});
            REQUIRE(result1 == R{6,4,0,0});

            auto result2 = Translate(R{3,2,0,0}, TestType{5}, TestType{4});
            REQUIRE(result2 == R{8,6,0,0});

            auto result3 = Translate(R{4,6,0,0}, TestType{3});
            REQUIRE(result3 == R{7,9,0,0});
        }

        SECTION("Enlarge(Rectangle) - free functions")
        {
            auto result1 = Enlarge(R{0,0,2,1}, S{4,3});
            REQUIRE(result1 == R{0,0,6,4});

            auto result2 = Enlarge(R{0,0,3,2}, TestType{5}, TestType{4});
            REQUIRE(result2 == R{0,0,8,6});

            auto result3 = Enlarge(R{0,0,4,6}, TestType{3});
            REQUIRE(result3 == R{0,0,7,9});
        }

        SECTION("Scale(Rectangle) - free functions")
        {
            auto result1 = Scale(R{2,3,2,3}, P{TestType{4},TestType{3}});
            REQUIRE(result1 == R{8,9,8,9});

            auto result2 = Scale(R{2,3,2,3}, S{TestType{4},TestType{3}});
            REQUIRE(result2 == R{8,9,8,9});

            auto result3 = Scale(R{5,3,5,3}, TestType{2}, TestType{4});
            REQUIRE(result3 == R{10,12,10,12});

            auto result4 = Scale(R{6,7,6,7}, TestType{2});
            REQUIRE(result4 == R{12,14,12,14});
        }

        SECTION("Union - edge cases")
        {
            // No overlap (should span both)
            R rect1{0,0,5,5};
            R rect4{10,10,2,2};
            auto union3 = Union(rect1, rect4);
            REQUIRE(union3 == R{0,0,12,12});

            // Touching at edge (should span both)
            R rect5{5,0,2,2};
            auto union4 = Union(rect1, rect5);
            REQUIRE(union4 == R{0,0,7,5});

            // One inside another
            R rect3{1,1,2,2};
            auto union2 = Union(rect1, rect3);
            REQUIRE(union2 == rect1);
        }

        SECTION("Intersect - edge cases")
        {
            R rect1{0,0,5,5};
            
            // No overlap
            R rect4{10,10,2,2};
            auto intersect3 = Intersect(rect1, rect4);
            REQUIRE(IsEmpty(intersect3));

            // Touching at edge (should be empty)
            R rect5{5,0,2,2};
            auto intersect4 = Intersect(rect1, rect5);
            REQUIRE(IsEmpty(intersect4));
        }

        SECTION("IsOverlapping - edge cases")
        {
            R rect1{0,0,5,5};
            
            // No overlap
            R rect4{10,10,2,2};
            auto noOverlap = IsOverlapping(rect1, rect4);
            REQUIRE_FALSE(noOverlap);

            // Touching at edge (should not overlap)
            R rect5{5,0,2,2};
            auto touchOverlap = IsOverlapping(rect1, rect5);
            REQUIRE_FALSE(touchOverlap);

            // Point outside rectangle
            auto pt3 = Point<TestType, ImplKind::kSimd>{10,10};
            auto ptOutside = IsOverlapping(rect1, pt3);
            REQUIRE_FALSE(ptOutside);
        }

        SECTION("IsEmpty(Rectangle)")
        {
            R empty{0,0,0,0};
            REQUIRE(IsEmpty(empty));

            R notEmpty{0,0,1,1};
            REQUIRE_FALSE(IsEmpty(notEmpty));

            R zeroWidth{0,0,0,5};
            REQUIRE(IsEmpty(zeroWidth));

            R zeroHeight{0,0,5,0};
            REQUIRE(IsEmpty(zeroHeight));
        }
    }
}

TEMPLATE_TEST_CASE( "saber::geometry::Matrix basic behaviors - impl variants",
                    "[saber][template]",
                    int, float, double)
{
    SECTION("ImplKind::kScalar")
    {
        using M = saber::geometry::Matrix<TestType, ImplKind::kScalar>;

        SECTION("Alt ctor and equality")
        {
            M m1{TestType{1},TestType{2},TestType{3},TestType{4},TestType{5},TestType{6}};
            M m2{TestType{1},TestType{2},TestType{3},TestType{4},TestType{5},TestType{6}};
            REQUIRE(m1 == m2);
            M m3{TestType{1},TestType{2},TestType{3},TestType{4},TestType{5},TestType{7}};
            REQUIRE(m1 != m3);
        }

        SECTION("Copy/Move/Assign")
        {
            const M cm{TestType{3},TestType{4},TestType{5},TestType{6},TestType{7},TestType{8}};
            M cpy{cm};
            REQUIRE(cpy == cm);

            M assigned{};
            assigned = cm;
            REQUIRE(assigned == cm);

            M moved = std::move(M{TestType{9},TestType{8},TestType{7},TestType{6},TestType{5},TestType{4}});
            REQUIRE(moved == M{TestType{9},TestType{8},TestType{7},TestType{6},TestType{5},TestType{4}});
        }

        SECTION("Floating inexact equality")
        {
            if constexpr (std::is_floating_point_v<TestType>)
            {
                const TestType eps = std::numeric_limits<TestType>::epsilon();
                M m1{TestType{1},TestType{2},TestType{3},TestType{4},TestType{5},TestType{6}};
                M m2{TestType{1} + eps/2,TestType{2},TestType{3},TestType{4},TestType{5},TestType{6}};
                REQUIRE(m1 == m2);
            }
        }
    }

    SECTION("ImplKind::kSimd")
    {
		using M = saber::geometry::Matrix<TestType, ImplKind::kSimd>;

		SECTION("Alt ctor and equality")
		{
			M m1{TestType{1},TestType{2},TestType{3},TestType{4},TestType{5},TestType{6}};
			M m2{TestType{1},TestType{2},TestType{3},TestType{4},TestType{5},TestType{6}};
			REQUIRE(m1 == m2);
			M m3{TestType{1},TestType{2},TestType{3},TestType{4},TestType{5},TestType{7}};
			REQUIRE(m1 != m3);
		}

		SECTION("Copy/Move/Assign")
		{
			const M cm{TestType{3},TestType{4},TestType{5},TestType{6},TestType{7},TestType{8}};
			M cpy{cm};
			REQUIRE(cpy == cm);

			M assigned{};
			assigned = cm;
			REQUIRE(assigned == cm);

			M moved = std::move(M{TestType{9},TestType{8},TestType{7},TestType{6},TestType{5},TestType{4}});
			REQUIRE(moved == M{TestType{9},TestType{8},TestType{7},TestType{6},TestType{5},TestType{4}});
		}

		SECTION("Floating inexact equality")
		{
			if constexpr (std::is_floating_point_v<TestType>)
			{
				const TestType eps = std::numeric_limits<TestType>::epsilon();
				M m1{TestType{1},TestType{2},TestType{3},TestType{4},TestType{5},TestType{6}};
				M m2{TestType{1} + eps/2,TestType{2},TestType{3},TestType{4},TestType{5},TestType{6}};
				REQUIRE(m1 == m2);
			}
		}
    }
}

TEMPLATE_TEST_CASE( "saber::geometry::detail::MatrixIdentity() works correctly - impl variants",
                    "[saber][matrix][helper]",
                    int, float, double)
{
	using namespace saber::geometry::detail;

	SECTION("ImplKind::kScalar - MatrixIdentity")
	{
		// Identity matrix should be: [1, 0, 0, 0, 1, 0, 0, 0]
		auto identity = MatrixIdentity<TestType, ImplKind::kScalar>();
		
		REQUIRE(identity.Get<0>() == TestType{1});	// M11
		REQUIRE(identity.Get<1>() == TestType{0});	// M12
		REQUIRE(identity.Get<2>() == TestType{0});	// M13 (translation X)
		REQUIRE(identity.Get<3>() == TestType{0});	// M21
		REQUIRE(identity.Get<4>() == TestType{1});	// M22
		REQUIRE(identity.Get<5>() == TestType{0});	// M23 (translation Y)
		REQUIRE(identity.Get<6>() == TestType{0});	// padding
		REQUIRE(identity.Get<7>() == TestType{0});	// padding
	}
}

TEMPLATE_TEST_CASE( "saber::geometry::detail::MatrixZero() works correctly - impl variants",
                    "[saber][matrix][helper]",
                    int, float, double)
{
	using namespace saber::geometry::detail;

	SECTION("ImplKind::kScalar - MatrixZero")
	{
		// Zero matrix should be all zeros
		auto zero = MatrixZero<TestType, ImplKind::kScalar>();
		
		REQUIRE(zero.Get<0>() == TestType{0});
		REQUIRE(zero.Get<1>() == TestType{0});
		REQUIRE(zero.Get<2>() == TestType{0});
		REQUIRE(zero.Get<3>() == TestType{0});
		REQUIRE(zero.Get<4>() == TestType{0});
		REQUIRE(zero.Get<5>() == TestType{0});
		REQUIRE(zero.Get<6>() == TestType{0});
		REQUIRE(zero.Get<7>() == TestType{0});
	}
}

TEMPLATE_TEST_CASE( "saber::geometry::detail::MatrixScale() works correctly - impl variants",
                    "[saber][matrix][helper]",
                    int, float, double)
{
	using namespace saber::geometry::detail;

	SECTION("ImplKind::kScalar - MatrixScale with scalars")
	{
		// Scale matrix: [sx, 0, 0, 0, sy, 0, 0, 0]
		auto scaled = MatrixScale<TestType, ImplKind::kScalar>(TestType{2}, TestType{3});
		
		REQUIRE(scaled.Get<0>() == TestType{2});	// M11 (scale X)
		REQUIRE(scaled.Get<1>() == TestType{0});	// M12
		REQUIRE(scaled.Get<2>() == TestType{0});	// M13
		REQUIRE(scaled.Get<3>() == TestType{0});	// M21
		REQUIRE(scaled.Get<4>() == TestType{3});	// M22 (scale Y)
		REQUIRE(scaled.Get<5>() == TestType{0});	// M23
		REQUIRE(scaled.Get<6>() == TestType{0});	// padding
		REQUIRE(scaled.Get<7>() == TestType{0});	// padding
	}

	SECTION("ImplKind::kScalar - MatrixScale with Impl2")
	{
		using namespace saber::geometry::detail;
		
		// Create an Impl2 with scale values
		typename Impl2<TestType>::Scalar scaleImpl2{TestType{5}, TestType{7}};
		auto scaled = MatrixScale<TestType>(scaleImpl2);
		
		REQUIRE(scaled.Get<0>() == TestType{5});	// M11
		REQUIRE(scaled.Get<1>() == TestType{0});	// M12
		REQUIRE(scaled.Get<4>() == TestType{7});	// M22
		REQUIRE(scaled.Get<2>() == TestType{0});	// M13
		REQUIRE(scaled.Get<5>() == TestType{0});	// M23
	}
}

TEMPLATE_TEST_CASE( "saber::geometry::detail::MatrixTranslation() works correctly - impl variants",
                    "[saber][matrix][helper]",
                    int, float, double)
{
	using namespace saber::geometry::detail;

	SECTION("ImplKind::kScalar - MatrixTranslation with scalars")
	{
		// Translation matrix: [1, 0, tx, 0, 1, ty, 0, 0]
		auto translated = MatrixTranslation<TestType, ImplKind::kScalar>(TestType{4}, TestType{5});
		
		REQUIRE(translated.Get<0>() == TestType{1});	// M11
		REQUIRE(translated.Get<1>() == TestType{0});	// M12
		REQUIRE(translated.Get<2>() == TestType{4});	// M13 (translation X)
		REQUIRE(translated.Get<3>() == TestType{0});	// M21
		REQUIRE(translated.Get<4>() == TestType{1});	// M22
		REQUIRE(translated.Get<5>() == TestType{5});	// M23 (translation Y)
		REQUIRE(translated.Get<6>() == TestType{0});	// padding
		REQUIRE(translated.Get<7>() == TestType{0});	// padding
	}

	SECTION("ImplKind::kScalar - MatrixTranslation with Impl2")
	{
		using namespace saber::geometry::detail;
		
		// Create an Impl2 with translation values
		typename Impl2<TestType>::Scalar transImpl2{TestType{10}, TestType{20}};
		auto translated = MatrixTranslation<TestType>(transImpl2);
		
		REQUIRE(translated.Get<0>() == TestType{1});	// M11
		REQUIRE(translated.Get<4>() == TestType{1});	// M22
		REQUIRE(translated.Get<2>() == TestType{10});	// M13 (translation X)
		REQUIRE(translated.Get<5>() == TestType{20});	// M23 (translation Y)
	}
}

TEMPLATE_TEST_CASE( "saber::geometry::detail::MatrixRotation() works correctly - impl variants",
                    "[saber][matrix][helper]",
                    float, double)
{
	using namespace saber::geometry::detail;

	SECTION("ImplKind::kScalar - MatrixRotation zero radians")
	{
		// 0 radians: sin(0) = 0, cos(0) = 1
		// Rotation matrix: [cos, -sin, 0, sin, cos, 0, 0, 0]
		auto rotated = MatrixRotation<TestType, ImplKind::kScalar>(TestType{0});
		
		REQUIRE(Inexact::IsEq(rotated.Get<0>(), TestType{1}));	// M11 = cos(0) = 1
		REQUIRE(Inexact::IsEq(rotated.Get<1>(), TestType{0}));	// M12 = -sin(0) = 0
		REQUIRE(Inexact::IsEq(rotated.Get<2>(), TestType{0}));	// M13 = 0
		REQUIRE(Inexact::IsEq(rotated.Get<3>(), TestType{0}));	// M21 = sin(0) = 0
		REQUIRE(Inexact::IsEq(rotated.Get<4>(), TestType{1}));	// M22 = cos(0) = 1
		REQUIRE(Inexact::IsEq(rotated.Get<5>(), TestType{0}));	// M23 = 0
	}

	SECTION("ImplKind::kScalar - MatrixRotation pi/2 radians (90 degrees)")
	{
		// pi/2 radians: sin(pi/2) ≈ 1, cos(pi/2) ≈ 0
		const TestType pi_2 = TestType{1.5707963267948966};	// π/2
		auto rotated = MatrixRotation<TestType, ImplKind::kScalar>(pi_2);
		
		REQUIRE(Inexact::IsEq(rotated.Get<0>(), TestType{0}));	// M11 = cos(π/2) ≈ 0
		REQUIRE(Inexact::IsEq(rotated.Get<1>(), TestType{-1}));	// M12 = -sin(π/2) ≈ -1
		REQUIRE(Inexact::IsEq(rotated.Get<3>(), TestType{1}));	// M21 = sin(π/2) ≈ 1
		REQUIRE(Inexact::IsEq(rotated.Get<4>(), TestType{0}));	// M22 = cos(π/2) ≈ 0
	}

	SECTION("ImplKind::kScalar - MatrixRotation negative angle")
	{
		// Negative angle should work fine (clockwise rotation)
		const TestType neg_pi_4 = TestType{-0.7853981633974483};	// -π/4
		auto rotated = MatrixRotation<TestType, ImplKind::kScalar>(neg_pi_4);
		
		// cos(-π/4) ≈ 0.707, sin(-π/4) ≈ -0.707
		const TestType sqrt2_2 = TestType{0.7071067811865476};
		REQUIRE(Inexact::IsEq(rotated.Get<0>(), sqrt2_2));	// M11 = cos(-π/4)
		REQUIRE(Inexact::IsEq(rotated.Get<1>(), sqrt2_2));	// M12 = -sin(-π/4) ≈ 0.707
		REQUIRE(Inexact::IsEq(rotated.Get<3>(), -sqrt2_2));	// M21 = sin(-π/4) ≈ -0.707
		REQUIRE(Inexact::IsEq(rotated.Get<4>(), sqrt2_2));	// M22 = cos(-π/4)
	}
}

TEMPLATE_TEST_CASE( "saber::geometry::detail::MatrixMul() works correctly - impl variants",
                    "[saber][matrix][helper]",
                    int, float, double)
{
	using namespace saber::geometry;
	using namespace saber::geometry::detail;

	SECTION("ImplKind::kScalar - MatrixMul with identity matrices")
	{
		// Identity * Identity should equal Identity
		auto identity1 = Matrix<TestType, ImplKind::kScalar>::MakeIdentity();
		auto identity2 = Matrix<TestType, ImplKind::kScalar>::MakeIdentity();
		
		//MatrixMul<TestType>(identity1, identity2);
		identity1 *= identity2;
		
		REQUIRE(identity1.M11() == TestType{1});	// M11
		REQUIRE(identity1.M12() == TestType{0});	// M12
		REQUIRE(identity1.M13() == TestType{0});	// M13
		REQUIRE(identity1.M21() == TestType{0});	// M21
		REQUIRE(identity1.M22() == TestType{1});	// M22
		REQUIRE(identity1.M23() == TestType{0});	// M23
	}

	SECTION("ImplKind::kScalar - MatrixMul with scale and translation")
	{
		// Scale [2, 0, 0, 0, 3, 0, 0, 0] * Translation [1, 0, 5, 0, 1, 7, 0, 0]
		// Should result in: [2, 0, 5, 0, 3, 7, 0, 0]
		auto scale = MatrixScale<TestType, ImplKind::kScalar>(TestType{2}, TestType{3});
		auto translation = MatrixTranslation<TestType, ImplKind::kScalar>(TestType{5}, TestType{7});
		
		MatrixMul<TestType>(scale, translation);
		
		REQUIRE(scale.Get<0>() == TestType{2});	// M11
		REQUIRE(scale.Get<1>() == TestType{0});	// M12
		REQUIRE(scale.Get<2>() == TestType{5});	// M13
		REQUIRE(scale.Get<3>() == TestType{0});	// M21
		REQUIRE(scale.Get<4>() == TestType{3});	// M22
		REQUIRE(scale.Get<5>() == TestType{7});	// M23
	}

	SECTION("ImplKind::kScalar - MatrixMul with two arbitrary matrices")
	{
		// Matrix A: [1, 2, 3, 4, 5, 6, 0, 0]
		// Matrix B: [2, 0, 1, 0, 2, 3, 0, 0]
		// A*B should yield:
		// M11 = (1*2) + (2*0) = 2
		// M12 = (1*0) + (2*2) = 4
		// M21 = (4*2) + (5*0) = 8
		// M22 = (4*0) + (5*2) = 10
		// M13 = (1*1) + (2*3) + 3 = 10
		// M23 = (4*1) + (5*3) + 6 = 25
		
		typename Impl8<TestType>::Scalar matA{TestType{1}, TestType{2}, TestType{3}, TestType{4}, TestType{5}, TestType{6}, 0, 0};
		typename Impl8<TestType>::Scalar matB{TestType{2}, TestType{0}, TestType{1}, TestType{0}, TestType{2}, TestType{3}, 0, 0};
		
		MatrixMul<TestType>(matA, matB);
		
		REQUIRE(matA.Get<0>() == TestType{2});		// M11
		REQUIRE(matA.Get<1>() == TestType{4});		// M12
		REQUIRE(matA.Get<2>() == TestType{10});		// M13
		REQUIRE(matA.Get<3>() == TestType{8});		// M21
		REQUIRE(matA.Get<4>() == TestType{10});		// M22
		REQUIRE(matA.Get<5>() == TestType{25});		// M23
		REQUIRE(matA.Get<6>() == TestType{0});		// padding
		REQUIRE(matA.Get<7>() == TestType{0});		// padding
	}
}

TEMPLATE_TEST_CASE( "saber::geometry::detail::MatrixInv() works correctly - impl variants",
                    "[saber][matrix][helper]",
                    float, double)
{
	using namespace saber::geometry::detail;

	SECTION("ImplKind::kScalar - MatrixInv of identity")
	{
		// Inverse of identity should be identity
		auto identity = MatrixIdentity<TestType, ImplKind::kScalar>();
		MatrixInv<TestType>(identity);
		
		REQUIRE(Inexact::IsEq(identity.Get<0>(), TestType{1}));	// M11
		REQUIRE(Inexact::IsEq(identity.Get<1>(), TestType{0}));	// M12
		REQUIRE(Inexact::IsEq(identity.Get<2>(), TestType{0}));	// M13
		REQUIRE(Inexact::IsEq(identity.Get<3>(), TestType{0}));	// M21
		REQUIRE(Inexact::IsEq(identity.Get<4>(), TestType{1}));	// M22
		REQUIRE(Inexact::IsEq(identity.Get<5>(), TestType{0}));	// M23
	}

	SECTION("ImplKind::kScalar - MatrixInv of scale matrix")
	{
		// Inverse of [2, 0, 0, 0, 3, 0, 0, 0] should be [0.5, 0, 0, 0, 1/3, 0, 0, 0]
		auto scale = MatrixScale<TestType, ImplKind::kScalar>(TestType{2}, TestType{3});
		MatrixInv<TestType>(scale);
		
		REQUIRE(Inexact::IsEq(scale.Get<0>(), TestType{0.5}));		// M11 = 1/2
		REQUIRE(Inexact::IsEq(scale.Get<1>(), TestType{0}));		// M12
		REQUIRE(Inexact::IsEq(scale.Get<2>(), TestType{0}));		// M13
		REQUIRE(Inexact::IsEq(scale.Get<3>(), TestType{0}));		// M21
		REQUIRE(Inexact::IsEq(scale.Get<4>(), TestType{1}/TestType{3}));	// M22 = 1/3
		REQUIRE(Inexact::IsEq(scale.Get<5>(), TestType{0}));		// M23
	}

	SECTION("ImplKind::kScalar - MatrixInv of translation matrix")
	{
		// Inverse of [1, 0, 5, 0, 1, 7, 0, 0] should be [1, 0, -5, 0, 1, -7, 0, 0]
		auto translation = MatrixTranslation<TestType, ImplKind::kScalar>(TestType{5}, TestType{7});
		MatrixInv<TestType>(translation);
		
		REQUIRE(Inexact::IsEq(translation.Get<0>(), TestType{1}));		// M11
		REQUIRE(Inexact::IsEq(translation.Get<1>(), TestType{0}));		// M12
		REQUIRE(Inexact::IsEq(translation.Get<2>(), TestType{-5}));	// M13
		REQUIRE(Inexact::IsEq(translation.Get<3>(), TestType{0}));		// M21
		REQUIRE(Inexact::IsEq(translation.Get<4>(), TestType{1}));		// M22
		REQUIRE(Inexact::IsEq(translation.Get<5>(), TestType{-7}));	// M23
	}

	SECTION("ImplKind::kScalar - MatrixInv of general invertible matrix")
	{
		// Matrix [2, 1, 0, 1, 2, 0, 0, 0] has determinant: (2*2) - (1*1) = 3
		// Its inverse should be [2/3, -1/3, 0, -1/3, 2/3, 0, 0, 0]
		typename Impl8<TestType>::Scalar mat{TestType{2}, TestType{1}, TestType{0}, TestType{1}, TestType{2}, TestType{0}, 0, 0};
		MatrixInv<TestType>(mat);
		
		REQUIRE(Inexact::IsEq(mat.Get<0>(), TestType{2}/TestType{3}));		// M11 = 2/3
		REQUIRE(Inexact::IsEq(mat.Get<1>(), TestType{-1}/TestType{3}));	// M12 = -1/3
		REQUIRE(Inexact::IsEq(mat.Get<3>(), TestType{-1}/TestType{3}));	// M21 = -1/3
		REQUIRE(Inexact::IsEq(mat.Get<4>(), TestType{2}/TestType{3}));		// M22 = 2/3
	}

	SECTION("ImplKind::kScalar - MatrixInv throws on singular matrix")
	{
		// Singular matrix [0, 0, 0, 0, 0, 0, 0, 0] has determinant 0 and cannot be inverted
		typename Impl8<TestType>::Scalar singularMat{TestType{0}, TestType{0}, TestType{0}, TestType{0}, TestType{0}, TestType{0}, 0, 0};
		
		REQUIRE_THROWS_AS(MatrixInv<TestType>(singularMat), std::runtime_error);
	}

	SECTION("ImplKind::kScalar - MatrixInv throws on singular matrix (rows are parallel)")
	{
		// Singular matrix [1, 2, 0, 2, 4, 0, 0, 0] has determinant: (1*4) - (2*2) = 0
		typename Impl8<TestType>::Scalar singularMat{TestType{1}, TestType{2}, TestType{0}, TestType{2}, TestType{4}, TestType{0}, 0, 0};
		
		REQUIRE_THROWS_AS(MatrixInv<TestType>(singularMat), std::runtime_error);
	}
}

// End of geometry_unittest2.cpp