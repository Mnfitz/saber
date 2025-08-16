/////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2024 Matthew Fitzgerald
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
/////////////////////////////////////////////////////////////////////

// catch2
#include "catch2/catch_template_test_macros.hpp"
#include "catch2/catch_test_macros.hpp"

// saber
#include "saber/geometry/point.hpp"
#include "saber/geometry/size.hpp"
#include "saber/geometry/rectangle.hpp"
#include "saber/geometry/utility.hpp"

// std
#include <cstdio>
#include <type_traits>

using namespace saber;
using namespace saber::geometry;

TEMPLATE_TEST_CASE(	"saber::geometry::Point::ctor() works correctly",
					"[saber][template]",
					int, float, double)
{
	using namespace saber::geometry;

	SECTION("Default Construct")
	{
		REQUIRE(Point<TestType>{}.X() == 0);
		REQUIRE(Point<TestType>{}.Y() == 0);
	}

	SECTION("Alt Construct")
	{
		REQUIRE(Point<TestType>{1,2}.X() == 1);
		REQUIRE(Point<TestType>{1,2}.Y() == 2);
	}

	SECTION("Copy Construct")
	{
		const Point<TestType> point{3,4};
		REQUIRE(Point<TestType>{point}.X() == point.X());
		REQUIRE(Point<TestType>{point}.Y() == point.Y());
	}

	SECTION("Copy Assign")
	{
		Point<TestType> point1{};
		const Point<TestType> point2{5,6};
		REQUIRE((point1 = point2).X() == point2.X());
		REQUIRE((point1 = point2).Y() == point2.Y());
	}

	SECTION("Move Construct")
	{
		Point<TestType> point{};
		REQUIRE((Point<TestType>{Point<TestType>{6,5}}).X() == 6);
		REQUIRE((Point<TestType>{Point<TestType>{6,5}}).Y() == 5);
	}

	SECTION("Move Assign")
	{
		Point<TestType> point{};
		REQUIRE((point = Point<TestType>{8,7}).X() == 8);
		REQUIRE((point = Point<TestType>{8,7}).Y() == 7);
	}

	SECTION("Structured Binding")
	{
		const Point<TestType> point{9,10};
		const auto [x, y] = point; // accessor: structured binding!
		REQUIRE(point.X() == x);
		REQUIRE(point.Y() == y);
	}

	// REVISIT mnfitz 14oct2024: std::tie needs reference(?) support from structured bindings
	// SECTION("Structured Binding tie()")
	// {
	//	const Point<TestType> point1{9,10};
	// 	TestType x{}, y{};
	// 	std::tie(x, y) = point1; // accessor: structured binding!
	//
	//	REQUIRE(point1.X() == x);
	// 	REQUIRE(point1.Y() == y);
	//
	//	const Point<TestType> point2{11,12};
	//	TestType z{}, w{};
	//  std::tie(z, std::ignore) = point2;
	//  std::tie(std::ignore, w) = point2;
	//
	// 	REQUIRE(point2.X() == z);
	// 	REQUIRE(point2.Y() == w);
	// }

	SECTION("Translate(Point)")
	{
		REQUIRE(Translate<TestType>(Point<TestType>{2,1}, Point<TestType>{4,3}) == Point<TestType>{6,4});
		REQUIRE(Translate<TestType>(Point<TestType>{3,2}, 5,4) == Point<TestType>{8,6});
		REQUIRE(Translate<TestType>(Point<TestType>{4,6}, 3) == Point<TestType>{7,9});
	}

	SECTION("Scale(Point)")
	{
		REQUIRE(Scale<TestType>(Point<TestType>{2,3}, Point<TestType>{4,3}) == Point<TestType>{8,9});
		REQUIRE(Scale<TestType>(Point<TestType>{5,3}, 2,4) == Point<TestType>{10,12});
		REQUIRE(Scale<TestType>(Point<TestType>{6,7}, 2) == Point<TestType>{12,14});
	}

	SECTION("Setters")
	{
		auto x = Point<TestType>{2,3};
		x.X(3);
		REQUIRE(x == Point<TestType>{3,3});
		auto y = Point<TestType>{2,3};
		y.Y(2);
		REQUIRE(y == Point<TestType>{2,2});
	}
}

TEMPLATE_TEST_CASE(	"saber::geometry::Size::ctor() works correctly",
					"[saber][template]",
					int, float, double)
{
	using namespace saber::geometry;

	SECTION("Default Construct")
	{
		REQUIRE(Size<TestType>{}.Width() == 0);
		REQUIRE(Size<TestType>{}.Height() == 0);
	}

	SECTION("Alt Construct")
	{
		REQUIRE(Size<TestType>{1,2}.Width() == 1);
		REQUIRE(Size<TestType>{1,2}.Height() == 2);
	}

	SECTION("Copy Construct")
	{
		const Size<TestType> size{3,4};
		REQUIRE(Size<TestType>{size}.Width() == size.Width());
		REQUIRE(Size<TestType>{size}.Height() == size.Height());
	}

	SECTION("Copy Assign")
	{
		Size<TestType> size1{};
		const Size<TestType> size2{5,6};
		REQUIRE((size1 = size2).Width() == size2.Width());
		REQUIRE((size1 = size2).Height() == size2.Height());
	}

	SECTION("Move Construct")
	{
		Size<TestType> size{};
		REQUIRE((Size<TestType>{Size<TestType>{6,5}}).Width() == 6);
		REQUIRE((Size<TestType>{Size<TestType>{6,5}}).Height() == 5);
	}

	SECTION("Move Assign")
	{
		Size<TestType> size{};
		REQUIRE((size = Size<TestType>{8,7}).Width() == 8);
		REQUIRE((size = Size<TestType>{8,7}).Height() == 7);
	}

	SECTION("Structured Binding")
	{
		const Size<TestType> size{9,10};
		const auto [x, y] = size; // accessor: structured binding!
		REQUIRE(size.Width() == x);
		REQUIRE(size.Height() == y);
	}

	// REVISIT mnfitz 14oct2024: std::tie needs reference(?) support from structured bindings
	// SECTION("Structured Binding tie()")
	// {
	//	const Size<TestType> size1{9,10};
	// 	TestType x{}, y{};
	// 	std::tie(x, y) = size1; // accessor: structured binding!
	//
	//	REQUIRE(size1.Width() == x);
	// 	REQUIRE(size1.Height() == y);
	//
	//	const Size<TestType> size2{11,12};
	//	TestType z{}, w{};
	//  std::tie(z, std::ignore) = size2;
	//  std::tie(std::ignore, w) = size2;
	//
	// 	REQUIRE(size2.Width() == z);
	// 	REQUIRE(size2.Height() == w);
	// }

	SECTION("Enlarge(Size)")
	{
		REQUIRE(Enlarge<TestType>(Size<TestType>{2,1}, Size<TestType>{4,3}) == Size<TestType>{6,4});
		REQUIRE(Enlarge<TestType>(Size<TestType>{3,2}, 5,4) == Size<TestType>{8,6});
		REQUIRE(Enlarge<TestType>(Size<TestType>{4,6}, 3) == Size<TestType>{7,9});
	}

	SECTION("Scale(Size)")
	{
		REQUIRE(Scale<TestType>(Size<TestType>{2,3}, Size<TestType>{4,3}) == Size<TestType>{8,9});
		REQUIRE(Scale<TestType>(Size<TestType>{5,3}, 2,4) == Size<TestType>{10,12});
		REQUIRE(Scale<TestType>(Size<TestType>{6,7}, 2) == Size<TestType>{12,14});
	}

	SECTION("Setters")
	{
		auto width = Size<TestType>{2,3};
		width.Width(3);
		REQUIRE(width == Size<TestType>{3,3});
		auto height = Size<TestType>{2,3};
		height.Height(2);
		REQUIRE(height == Size<TestType>{2,2});
	}
}

TEMPLATE_TEST_CASE(	"saber::geometry::utility works correctly",
					"[saber][template]",
					int, float, double)
{
	using namespace saber::geometry;

	SECTION("Convert: Size -> Point")
	{
		const Size<TestType> size{1,2};
		auto point = saber::ConvertTo<Point<TestType>>(size);
		REQUIRE(point.X() == size.Width());
		REQUIRE(point.Y() == size.Height());
	}

	SECTION("Convert: Point -> Size")
	{
		const Point<TestType> point{1,2};
		auto size = saber::ConvertTo<Size<TestType>>(point);
		REQUIRE(size.Width() == point.X());
		REQUIRE(size.Height() == point.Y());
	}
}

TEMPLATE_TEST_CASE(	"saber::geometry inexact comparison works correctly",
					"[saber][template]",
					Point<float>, Point<double>,
					Size<float>, Size<double>)
{
	SECTION("operator==(lhs,rhs) (inexact)")
	{
#if __cplusplus >= 202002L // C++20 or greater
		// Check constexpr compiletime correctness
		constexpr auto isEqual = (TestType{1,2} == TestType{1,2});
		REQUIRE(isEqual);
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		using ValueType = typename TestType::ValueType;

		auto k3point6 = TestType{static_cast<ValueType>(3.6), static_cast<ValueType>(3.6)};
		auto k3point0 = TestType{static_cast<ValueType>(3.0), static_cast<ValueType>(3.0)};
		auto k1point2 = TestType{static_cast<ValueType>(1.2), static_cast<ValueType>(1.2)};
		REQUIRE(k3point6 == (k3point0 * k1point2));

		auto k0point3 = TestType{static_cast<ValueType>(0.3), static_cast<ValueType>(0.3)};
		auto k0point2 = TestType{static_cast<ValueType>(0.2), static_cast<ValueType>(0.2)};
		auto k0point1 = TestType{static_cast<ValueType>(0.1), static_cast<ValueType>(0.1)};
		REQUIRE(k0point3 == (k0point2 + k0point1));
	}

	SECTION("operator!=(lhs,rhs) (inexact)")
	{
#if __cplusplus >= 202002L // C++20 or greater
		// Check constexpr compiletime correctness
		constexpr auto isEqual = (TestType{1,2} != TestType{2,1});
		REQUIRE(isEqual);
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		using ValueType = typename TestType::ValueType;

		auto k3point6 = TestType{static_cast<ValueType>(3.6), static_cast<ValueType>(3.6)};
		auto k3point0 = TestType{static_cast<ValueType>(3.0), static_cast<ValueType>(3.0)};
		auto k1point2 = TestType{static_cast<ValueType>(1.2), static_cast<ValueType>(1.2)};
		REQUIRE(!(k3point6 != (k3point0 * k1point2)));

		auto k0point3 = TestType{static_cast<ValueType>(0.3), static_cast<ValueType>(0.3)};
		auto k0point2 = TestType{static_cast<ValueType>(0.2), static_cast<ValueType>(0.2)};
		auto k0point1 = TestType{static_cast<ValueType>(0.1), static_cast<ValueType>(0.1)};
		REQUIRE(!(k0point3 != (k0point2 + k0point1)));
	}

	SECTION("RoundNearest()")
	{
		REQUIRE(TestType{1.1f,-2.9f}.RoundNearest() == TestType{1.0f,-3.0f});
		REQUIRE(RoundNearest(TestType{1.1f,-2.9f}) == TestType{1.0f,-3.0f});

		REQUIRE(TestType{1.9f,-2.1f}.RoundNearest() == TestType{2.0f,-2.0f});
		REQUIRE(RoundNearest(TestType{1.9f,-2.1f}) == TestType{2.0f,-2.0f});

		REQUIRE(TestType{1.5f,-3.5f}.RoundNearest() == TestType{2.0f,-4.0f});
		REQUIRE(RoundNearest(TestType{1.5f,-3.5f}) == TestType{2.0f,-4.0f});

		REQUIRE(TestType{2.5f,-2.5f}.RoundNearest() == TestType{3.0f,-3.0f});
		REQUIRE(RoundNearest(TestType{2.5f,-2.5f}) == TestType{3.0f,-3.0f});
	}

	SECTION("RoundTrunc Point")
	{
		REQUIRE(TestType{1.1f,-2.9f}.RoundTrunc() == TestType{1.0f,-2.0f});
		REQUIRE(RoundTrunc(TestType{1.1f,-2.9f}) == TestType{1.0f,-2.0f});

		REQUIRE(TestType{1.9f,-2.1f}.RoundTrunc() == TestType{1.0f,-2.0f});
		REQUIRE(RoundTrunc(TestType{1.9f,-2.1f}) == TestType{1.0f,-2.0f});

		REQUIRE(TestType{1.5f,-3.5f}.RoundTrunc() == TestType{1.0f,-3.0f});
		REQUIRE(RoundTrunc(TestType{1.5f,-3.5f}) == TestType{1.0f,-3.0f});

		REQUIRE(TestType{2.5f,-2.5f}.RoundTrunc() == TestType{2.0f,-2.0f});
		REQUIRE(RoundTrunc(TestType{2.5f,-2.5f}) == TestType{2.0f,-2.0f});
	}

	SECTION("RoundCeil()")
	{
		REQUIRE(TestType{1.1f,-2.9f}.RoundCeil() == TestType{2.0f,-2.0f});
		REQUIRE(RoundCeil(TestType{1.1f,-2.9f}) == TestType{2.0f,-2.0f});

		REQUIRE(TestType{1.9f,-2.1f}.RoundCeil() == TestType{2.0f,-2.0f});
		REQUIRE(RoundCeil(TestType{1.9f,-2.1f}) == TestType{2.0f,-2.0f});

		REQUIRE(TestType{1.5f,-3.5f}.RoundCeil() == TestType{2.0f,-3.0f});
		REQUIRE(RoundCeil(TestType{1.5f,-3.5f}) == TestType{2.0f,-3.0f});

		REQUIRE(TestType{2.5f,-2.5f}.RoundCeil() == TestType{3.0f,-2.0f});
		REQUIRE(RoundCeil(TestType{2.5f,-2.5f}) == TestType{3.0f,-2.0f});
	}

	SECTION("RoundFloor Point")
	{
		REQUIRE(TestType{1.1f,-2.9f}.RoundFloor() == TestType{1.0f,-3.0f});
		REQUIRE(RoundFloor(TestType{1.1f,-2.9f}) == TestType{1.0f,-3.0f});

		REQUIRE(TestType{1.9f,-2.1f}.RoundFloor() == TestType{1.0f,-3.0f});
		REQUIRE(RoundFloor(TestType{1.9f,-2.1f}) == TestType{1.0f,-3.0f});

		REQUIRE(TestType{1.5f,-3.5f}.RoundFloor() == TestType{1.0f,-4.0f});
		REQUIRE(RoundFloor(TestType{1.5f,-3.5f}) == TestType{1.0f,-4.0f});

		REQUIRE(TestType{2.5f,-2.5f}.RoundFloor() == TestType{2.0f,-3.0f});
		REQUIRE(RoundFloor(TestType{2.5f,-2.5f}) == TestType{2.0f,-3.0f});
	}
}

TEMPLATE_TEST_CASE(	"saber::geometry operators works correctly",
					"[saber][template]",
					Point<int>, Point<float>, Point<double>,
					Size<int>, Size<float>, Size<double>)
{
	SECTION("operator==(lhs,rhs) (exact)")
	{
#if __cplusplus >= 202002L // C++20 or greater
		// Check constexpr compiletime correctness
		constexpr auto isEqual = (TestType{1,2} == TestType{1,2});
		REQUIRE(isEqual);
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		REQUIRE(TestType{3,4} == TestType{3,4});
	}

	SECTION("operator!=(lhs,rhs) (exact)")
	{
#if __cplusplus >= 202002L // C++20 or greater
		// Check constexpr compiletime correctness
		constexpr auto isNotEqual = (TestType{1,2} != TestType{2,1});
		REQUIRE(isNotEqual);
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		REQUIRE(TestType{4,3} != TestType{3,4});
	}

	SECTION("operator+=(rhs)")
	{
		TestType type{1,2};
		REQUIRE(((type += TestType{3,4}), type == TestType{4,6}));
	}

	SECTION("operator-=(rhs)")
	{
		TestType type{4,6};
		REQUIRE(((type -= TestType{1,2}), type == TestType{3,4}));
	}

	SECTION("operator*=(rhs)")
	{
		TestType type{2,3};
		REQUIRE(((type *= TestType{3,4}), type == TestType{6,12}));
	}

	SECTION("operator/=(rhs)")
	{
		TestType type{12,8};
		REQUIRE(((type /= TestType{6,2}), type == TestType{2,4}));
	}

	SECTION("operator+(lhs,rhs)")
	{
#if __cplusplus >= 202002L // C++20 or greater
		// Check constexpr compiletime correctness
		constexpr auto type1 = TestType{1,2} + TestType{3,4};
		REQUIRE(type1 == TestType{4,6});
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		TestType type{};
		REQUIRE(((type = TestType{1,2} + TestType{3,4}), type == TestType{4,6}));
	}

	SECTION("operator-(lhs,rhs)")
	{
#if __cplusplus >= 202002L // C++20 or greater
		// Check constexpr compiletime correctness
		constexpr auto type1 = TestType{7,8} - TestType{5,2};
		REQUIRE(type1 == TestType{2,6});
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		TestType type{};
		REQUIRE(((type = TestType{10,8} - TestType{3,4}), type == TestType{7,4}));
	}

	SECTION("operator*(lhs,rhs)")
	{
#if __cplusplus >= 202002L // C++20 or greater
		// Check constexpr compiletime correctness
		constexpr auto type1 = TestType{3,2} * TestType{4,7};
		REQUIRE(type1 == TestType{12,14});
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		TestType type{};
		REQUIRE(((type = TestType{2,3} * TestType{5,6}), type == TestType{10,18}));
	}

	SECTION("operator/(lhs,rhs)")
	{
#if __cplusplus >= 202002L // C++20 or greater
		// Check constexpr compiletime correctness
		constexpr auto type1 = TestType{12,8} / TestType{4,2};
		REQUIRE(type1 == TestType{3,4});
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		TestType type{};
		REQUIRE(((type = TestType{8,15} / TestType{2,3}), type == TestType{4,5}));
	}

	SECTION("operator-(unary)")
	{
#if __cplusplus >= 202002L // C++20 or greater
		// Check compiletime correctness
		constexpr auto type1 = -TestType{1,2};
		REQUIRE(type1 == TestType{-1,-2});
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		REQUIRE(-TestType{4,5} == TestType{-4,-5});
	}

	SECTION("operator+(unary)")
	{
#if __cplusplus >= 202002L // C++20 or greater
		// Check compiletime correctness
		constexpr auto type1 = +TestType{1,2};
		REQUIRE(type1 == TestType{1,2});
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		REQUIRE(+TestType{4,5} == TestType{4,5});
	}
}

TEMPLATE_TEST_CASE(	"saber::geometry::Rectangle::ctor() works correctly",
					"[saber][template]",
					int, float, double)
{
	using namespace saber::geometry;

	SECTION("Default Construct")
	{
		REQUIRE(Rectangle<TestType>{}.X() == 0);
		REQUIRE(Rectangle<TestType>{}.Y() == 0);
		REQUIRE(Rectangle<TestType>{}.Width() == 0);
		REQUIRE(Rectangle<TestType>{}.Height() == 0);
	}

	SECTION("Alt Construct")
	{
		REQUIRE(Rectangle<TestType>{1,2,3,4}.X() == 1);
		REQUIRE(Rectangle<TestType>{1,2,3,4}.Y() == 2);
		REQUIRE(Rectangle<TestType>{1,2,3,4}.Width() == 3);
		REQUIRE(Rectangle<TestType>{1,2,3,4}.Height() == 4);
	}

	SECTION("Copy Construct")
	{
		const Rectangle<TestType> rectangle{1,2,3,4};
		REQUIRE(Rectangle<TestType>{rectangle}.X() == rectangle.X());
		REQUIRE(Rectangle<TestType>{rectangle}.Y() == rectangle.Y());
		REQUIRE(Rectangle<TestType>{rectangle}.Width() == rectangle.Width());
		REQUIRE(Rectangle<TestType>{rectangle}.Height() == rectangle.Height());
	}

	SECTION("Copy Assign")
	{
		Rectangle<TestType> rectangle1{};
		const Rectangle<TestType> rectangle2{1,2,3,4};
		REQUIRE((rectangle1 = rectangle2).X() == rectangle2.X());
		REQUIRE((rectangle1 = rectangle2).Y() == rectangle2.Y());
		REQUIRE((rectangle1 = rectangle2).Width() == rectangle2.Width());
		REQUIRE((rectangle1 = rectangle2).Height() == rectangle2.Height());
	}

	SECTION("Move Construct")
	{
		Rectangle<TestType> rectangle{};
		REQUIRE((Rectangle<TestType>{Rectangle<TestType>{1,2,3,4}}).X() == 1);
		REQUIRE((Rectangle<TestType>{Rectangle<TestType>{1,2,3,4}}).Y() == 2);
		REQUIRE((Rectangle<TestType>{Rectangle<TestType>{1,2,3,4}}).Width() == 3);
		REQUIRE((Rectangle<TestType>{Rectangle<TestType>{1,2,3,4}}).Height() == 4);
	}

	SECTION("Move Assign")
	{
		Rectangle<TestType> rectangle{};
		REQUIRE((rectangle = Rectangle<TestType>{1,2,3,4}).X() == 1);
		REQUIRE((rectangle = Rectangle<TestType>{1,2,3,4}).Y() == 2);
		REQUIRE((rectangle = Rectangle<TestType>{1,2,3,4}).Width() == 3);
		REQUIRE((rectangle = Rectangle<TestType>{1,2,3,4}).Height() == 4);
	}

	/*
	// REVISIT mnfitz 26may2025: Add structured binding for Rectangle
	SECTION("Structured Binding")
	{
		const Rectangle<TestType> rectangle{1,2,3,4};
		const auto [x, y, width, height] = rectangle; // accessor: structured binding!
		REQUIRE(point.X() == x);
		REQUIRE(point.Y() == y);
		REQUIRE(point.Width() == width);
		REQUIRE(point.Height() == height);
	}
	*/

	// CLASS METHOD
	SECTION("Translate(Rectangle)")
	{
		REQUIRE(Rectangle<TestType>{Point<TestType>{2,1}}.Translate(Point<TestType>{4,3}) == Rectangle<TestType>{Point<TestType>{6,4}});
		REQUIRE(Rectangle<TestType>{Point<TestType>{3,2}}.Translate(5,4) == Rectangle<TestType>{Point<TestType>{8,6}});
		REQUIRE(Rectangle<TestType>{Point<TestType>{4,6}}.Translate(3) == Rectangle<TestType>{Point<TestType>{7,9}});
	}

	SECTION("Enlarge(Rectangle)")
	{
		REQUIRE(Rectangle<TestType>{Size<TestType>{2,1}}.Enlarge(Size<TestType>{4,3}) == Rectangle<TestType>{Size<TestType>{6,4}});
		REQUIRE(Rectangle<TestType>{Size<TestType>{3,2}}.Enlarge(5,4) == Rectangle<TestType>{Size<TestType>{8,6}});
		REQUIRE(Rectangle<TestType>{Size<TestType>{4,6}}.Enlarge(3) == Rectangle<TestType>{Size<TestType>{7,9}});
	}

	SECTION("Scale(Rectangle)")
	{
		REQUIRE(Rectangle<TestType>{2,3,2,3}.Scale(Point<TestType>{4,3}) == Rectangle<TestType>{8,9,8,9});
		REQUIRE(Rectangle<TestType>{2,3,2,3}.Scale(Size<TestType>{4,3}) == Rectangle<TestType>{8,9,8,9});
		REQUIRE(Rectangle<TestType>{5,3,5,3}.Scale(2,4) == Rectangle<TestType>{10,12,10,12});
		REQUIRE(Rectangle<TestType>{6,7,6,7}.Scale(2) == Rectangle<TestType>{12,14,12,14});
	}

	SECTION("Setters")
	{
		auto rectangle = Rectangle<TestType>{0,0,0,0};
		rectangle.X(1);
		REQUIRE(rectangle == Rectangle<TestType>{1,0,0,0});
		rectangle.Y(2);
		REQUIRE(rectangle == Rectangle<TestType>{1,2,0,0});
		rectangle.Width(3);
		REQUIRE(rectangle == Rectangle<TestType>{1,2,3,0});
		rectangle.Height(4);
		REQUIRE(rectangle == Rectangle<TestType>{1,2,3,4});
	}

	SECTION("Union")
    {
        // Overlapping rectangles
        auto rect1 = Rectangle<TestType>{0,0,5,5};
        auto rect2 = Rectangle<TestType>{2,2,4,4};
        auto union1 = Union(rect1, rect2);
        REQUIRE(union1 == Rectangle<TestType>{0,0,6,6});

        // One rectangle inside another
        auto rect3 = Rectangle<TestType>{1,1,2,2};
        auto union2 = Union(rect1, rect3);
        REQUIRE(union2 == rect1);

        // No overlap (should span both)
        auto rect4 = Rectangle<TestType>{10,10,2,2};
        auto union3 = Union(rect1, rect4);
        REQUIRE(union3 == Rectangle<TestType>{0,0,12,12});

        // Touching at edge (should span both)
        auto rect5 = Rectangle<TestType>{5,0,2,2};
        auto union4 = Union(rect1, rect5);
        REQUIRE(union4 == Rectangle<TestType>{0,0,7,5});
    }

	SECTION("Intersect")
    {
        // Overlapping rectangles
        auto rect1 = Rectangle<TestType>{0,0,5,5};
        auto rect2 = Rectangle<TestType>{2,2,4,4};
        REQUIRE(Intersect(rect1, rect2) == Rectangle<TestType>{2,2,3,3});

        // One rectangle inside another
        auto rect3 = Rectangle<TestType>{1,1,2,2};
        REQUIRE(Intersect(rect1, rect3) == rect3);

        // No overlap
        auto rect4 = Rectangle<TestType>{10,10,2,2};
        REQUIRE(IsEmpty(Intersect(rect1, rect4)));

        // Touching at edge (should be empty)
        auto rect5 = Rectangle<TestType>{5,0,2,2};
        REQUIRE(IsEmpty(Intersect(rect1, rect5)));
    }

	SECTION("IsOverlapping")
    {
        // Rectangle-Rectangle overlap
        auto rect1 = Rectangle<TestType>{0, 0, 5, 5};
        auto rect2 = Rectangle<TestType>{2, 2, 4, 4};
        REQUIRE(IsOverlapping(rect1, rect2));
        REQUIRE(IsOverlapping(rect2, rect1));
        REQUIRE(rect1.IsOverlapping(rect2));
        REQUIRE(rect2.IsOverlapping(rect1));

        // One rectangle inside another
        auto rect3 = Rectangle<TestType>{1, 1, 2, 2};
        REQUIRE(IsOverlapping(rect1, rect3));
        REQUIRE(IsOverlapping(rect3, rect1));
        REQUIRE(rect1.IsOverlapping(rect3));
        REQUIRE(rect3.IsOverlapping(rect1));

        // No overlap
        auto rect4 = Rectangle<TestType>{10, 10, 2, 2};
        REQUIRE_FALSE(IsOverlapping(rect1, rect4));
        REQUIRE_FALSE(IsOverlapping(rect4, rect1));
        REQUIRE_FALSE(rect1.IsOverlapping(rect4));
        REQUIRE_FALSE(rect4.IsOverlapping(rect1));

        // Touching at edge (should not overlap)
        auto rect5 = Rectangle<TestType>{5, 0, 2, 2};
        REQUIRE_FALSE(IsOverlapping(rect1, rect5));
        REQUIRE_FALSE(IsOverlapping(rect5, rect1));
        REQUIRE_FALSE(rect1.IsOverlapping(rect5));
        REQUIRE_FALSE(rect5.IsOverlapping(rect1));

        // Rectangle-Point overlap (inside)
        auto pt1 = Point<TestType>{2, 2};
        REQUIRE(IsOverlapping(rect1, pt1));
        REQUIRE(rect1.IsOverlapping(pt1));

        // Rectangle-Point overlap (on edge)
        auto pt2 = Point<TestType>{0, 0};
        REQUIRE(IsOverlapping(rect1, pt2));
        REQUIRE(rect1.IsOverlapping(pt2));

        // Rectangle-Point no overlap (outside)
        auto pt3 = Point<TestType>{10, 10};
        REQUIRE_FALSE(IsOverlapping(rect1, pt3));
        REQUIRE_FALSE(rect1.IsOverlapping(pt3));
    }

}

TEMPLATE_TEST_CASE(	"saber::geometry::Rectangle rounding works correctly",
					"[saber][template]",
					float, double)
{
	SECTION("RoundNearest(Rectangle)")
	{
		REQUIRE(Rectangle<TestType>{1.5f,2.5f,3.5f,4.5f}.RoundNearest() == Rectangle<TestType>{2,3,4,5});
		REQUIRE(Rectangle<TestType>{1.1f,2.1f,3.1f,4.1f}.RoundNearest() == Rectangle<TestType>{1,2,3,4});
		REQUIRE(Rectangle<TestType>{1.9f,2.9f,3.9f,4.9f}.RoundNearest() == Rectangle<TestType>{2,3,4,5});
		REQUIRE(Rectangle<TestType>{-1.5f,-2.5f,-3.5f,-4.5f}.RoundNearest() == Rectangle<TestType>{-2,-3,-4,-5});
		REQUIRE(Rectangle<TestType>{-1.1f,-2.1f,-3.1f,-4.1f}.RoundNearest() == Rectangle<TestType>{-1,-2,-3,-4});
		REQUIRE(Rectangle<TestType>{-1.9f,-2.9f,-3.9f,-4.9f}.RoundNearest() == Rectangle<TestType>{-2,-3,-4,-5});

		REQUIRE(RoundNearest(Rectangle<TestType>{1.5f,2.5f,3.5f,4.5f}) == Rectangle<TestType>{2,3,4,5});
		REQUIRE(RoundNearest(Rectangle<TestType>{1.1f,2.1f,3.1f,4.1f}) == Rectangle<TestType>{1,2,3,4});
		REQUIRE(RoundNearest(Rectangle<TestType>{1.9f,2.9f,3.9f,4.9f}) == Rectangle<TestType>{2,3,4,5});
		REQUIRE(RoundNearest(Rectangle<TestType>{-1.5f,-2.5f,-3.5f,-4.5f}) == Rectangle<TestType>{-2,-3,-4,-5});
		REQUIRE(RoundNearest(Rectangle<TestType>{-1.1f,-2.1f,-3.1f,-4.1f}) == Rectangle<TestType>{-1,-2,-3,-4});
		REQUIRE(RoundNearest(Rectangle<TestType>{-1.9f,-2.9f,-3.9f,-4.9f}) == Rectangle<TestType>{-2,-3,-4,-5});
	}

	SECTION("RoundTrunc(Rectangle)")
	{
		REQUIRE(Rectangle<TestType>{1.5f,2.5f,3.5f,4.5f}.RoundTrunc() == Rectangle<TestType>{1,2,3,4});
		REQUIRE(Rectangle<TestType>{1.1f,2.1f,3.1f,4.1f}.RoundTrunc() == Rectangle<TestType>{1,2,3,4});
		REQUIRE(Rectangle<TestType>{1.9f,2.9f,3.9f,4.9f}.RoundTrunc() == Rectangle<TestType>{1,2,3,4});
		REQUIRE(Rectangle<TestType>{-1.5f,-2.5f,-3.5f,-4.5f}.RoundTrunc() == Rectangle<TestType>{-1,-2,-3,-4});
		REQUIRE(Rectangle<TestType>{-1.1f,-2.1f,-3.1f,-4.1f}.RoundTrunc() == Rectangle<TestType>{-1,-2,-3,-4});
		REQUIRE(Rectangle<TestType>{-1.9f,-2.9f,-3.9f,-4.9f}.RoundTrunc() == Rectangle<TestType>{-1,-2,-3,-4});

		REQUIRE(RoundTrunc(Rectangle<TestType>{1.5f,2.5f,3.5f,4.5f}) == Rectangle<TestType>{1,2,3,4});
		REQUIRE(RoundTrunc(Rectangle<TestType>{1.1f,2.1f,3.1f,4.1f}) == Rectangle<TestType>{1,2,3,4});
		REQUIRE(RoundTrunc(Rectangle<TestType>{1.9f,2.9f,3.9f,4.9f}) == Rectangle<TestType>{1,2,3,4});
		REQUIRE(RoundTrunc(Rectangle<TestType>{-1.5f,-2.5f,-3.5f,-4.5f}) == Rectangle<TestType>{-1,-2,-3,-4});
		REQUIRE(RoundTrunc(Rectangle<TestType>{-1.1f,-2.1f,-3.1f,-4.1f}) == Rectangle<TestType>{-1,-2,-3,-4});
		REQUIRE(RoundTrunc(Rectangle<TestType>{-1.9f,-2.9f,-3.9f,-4.9f}) == Rectangle<TestType>{-1,-2,-3,-4});
	}

	SECTION("RoundCeil(Rectangle)")
	{
		REQUIRE(Rectangle<TestType>{1.5f,2.5f,3.5f,4.5f}.RoundCeil() == Rectangle<TestType>{2,3,4,5});
		REQUIRE(Rectangle<TestType>{1.1f,2.1f,3.1f,4.1f}.RoundCeil() == Rectangle<TestType>{2,3,4,5});
		REQUIRE(Rectangle<TestType>{1.9f,2.9f,3.9f,4.9f}.RoundCeil() == Rectangle<TestType>{2,3,4,5});
		REQUIRE(Rectangle<TestType>{-1.5f,-2.5f,-3.5f,-4.5f}.RoundCeil() == Rectangle<TestType>{-1,-2,-3,-4});
		REQUIRE(Rectangle<TestType>{-1.1f,-2.1f,-3.1f,-4.1f}.RoundCeil() == Rectangle<TestType>{-1,-2,-3,-4});
		REQUIRE(Rectangle<TestType>{-1.9f,-2.9f,-3.9f,-4.9f}.RoundCeil() == Rectangle<TestType>{-1,-2,-3,-4});

		REQUIRE(RoundCeil(Rectangle<TestType>{1.5f,2.5f,3.5f,4.5f}) == Rectangle<TestType>{2,3,4,5});
		REQUIRE(RoundCeil(Rectangle<TestType>{1.1f,2.1f,3.1f,4.1f}) == Rectangle<TestType>{2,3,4,5});
		REQUIRE(RoundCeil(Rectangle<TestType>{1.9f,2.9f,3.9f,4.9f}) == Rectangle<TestType>{2,3,4,5});
		REQUIRE(RoundCeil(Rectangle<TestType>{-1.5f,-2.5f,-3.5f,-4.5f}) == Rectangle<TestType>{-1,-2,-3,-4});
		REQUIRE(RoundCeil(Rectangle<TestType>{-1.1f,-2.1f,-3.1f,-4.1f}) == Rectangle<TestType>{-1,-2,-3,-4});
		REQUIRE(RoundCeil(Rectangle<TestType>{-1.9f,-2.9f,-3.9f,-4.9f}) == Rectangle<TestType>{-1,-2,-3,-4});
	}

	SECTION("RoundFloor(Rectangle)")
	{
		REQUIRE(Rectangle<TestType>{1.5f,2.5f,3.5f,4.5f}.RoundFloor() == Rectangle<TestType>{1,2,3,4});
		REQUIRE(Rectangle<TestType>{1.1f,2.1f,3.1f,4.1f}.RoundFloor() == Rectangle<TestType>{1,2,3,4});
		REQUIRE(Rectangle<TestType>{1.9f,2.9f,3.9f,4.9f}.RoundFloor() == Rectangle<TestType>{1,2,3,4});
		REQUIRE(Rectangle<TestType>{-1.5f,-2.5f,-3.5f,-4.5f}.RoundFloor() == Rectangle<TestType>{-2,-3,-4,-5});
		REQUIRE(Rectangle<TestType>{-1.1f,-2.1f,-3.1f,-4.1f}.RoundFloor() == Rectangle<TestType>{-2,-3,-4,-5});
		REQUIRE(Rectangle<TestType>{-1.9f,-2.9f,-3.9f,-4.9f}.RoundFloor() == Rectangle<TestType>{-2,-3,-4,-5});

		REQUIRE(RoundFloor(Rectangle<TestType>{1.5f,2.5f,3.5f,4.5f}) == Rectangle<TestType>{1,2,3,4});
		REQUIRE(RoundFloor(Rectangle<TestType>{1.1f,2.1f,3.1f,4.1f}) == Rectangle<TestType>{1,2,3,4});
		REQUIRE(RoundFloor(Rectangle<TestType>{1.9f,2.9f,3.9f,4.9f}) == Rectangle<TestType>{1,2,3,4});
		REQUIRE(RoundFloor(Rectangle<TestType>{-1.5f,-2.5f,-3.5f,-4.5f}) == Rectangle<TestType>{-2,-3,-4,-5});
		REQUIRE(RoundFloor(Rectangle<TestType>{-1.1f,-2.1f,-3.1f,-4.1f}) == Rectangle<TestType>{-2,-3,-4,-5});
		REQUIRE(RoundFloor(Rectangle<TestType>{-1.9f,-2.9f,-3.9f,-4.9f}) == Rectangle<TestType>{-2,-3,-4,-5});
	}

	// FREE FUNCTION
}

