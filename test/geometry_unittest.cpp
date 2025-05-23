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
	// 	std::tie(z, std::ignore) = point2;
	// 	std::tie(std::ignore, w) = point2;
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
	// 	std::tie(z, std::ignore) = size2;
	// 	std::tie(std::ignore, w) = size2;
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

