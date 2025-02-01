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
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
/////////////////////////////////////////////////////////////////////

// catch2
#include "catch2/catch_test_macros.hpp"

// saber
#include "saber/geometry/point.hpp"
#include "saber/geometry/size.hpp"
#include "saber/geometry/utility.hpp"
#include "saber/inexact.hpp"

// std
#include <cstdio>
#include <type_traits>

TEST_CASE("saber::geometry::point", "[saber]")
{
	SECTION("Default Construct")
	{
		saber::geometry::Point<int> point{};
		REQUIRE(point.X() == 0);
		REQUIRE(point.Y() == 0);
	}

	SECTION("Alt Construct")
	{
		saber::geometry::Point<int> point{1, 2};
		REQUIRE(point.X() == 1);
		REQUIRE(point.Y() == 2);
	}

    SECTION("Copy Construct")
	{
		saber::geometry::Point<int> point1{1,2};
        saber::geometry::Point<int> point2{point1};
		REQUIRE(point2.X() == 1);
		REQUIRE(point2.Y() == 2);
	}

    SECTION("Copy Assign")
	{
		saber::geometry::Point<int> point1{1,2};
        saber::geometry::Point<int> point2 = point1;
		REQUIRE(point2.X() == 1);
		REQUIRE(point2.Y() == 2);
	}

    SECTION("Move Construct")
	{
		saber::geometry::Point<int> point1{1,2};
        saber::geometry::Point<int> point2{std::move(point1)};
		REQUIRE(point2.X() == 1);
		REQUIRE(point2.Y() == 2);
	}

    SECTION("Move Assign")
	{
		saber::geometry::Point<int> point1{1,2};
        saber::geometry::Point<int> point2 = std::move(point1);
		REQUIRE(point2.X() == 1);
		REQUIRE(point2.Y() == 2);
	}

	SECTION("Structured Binding")
	{
		saber::geometry::Point<int> point{1,2};
		const auto [x, y] = point; // accessor: structured binding!
		REQUIRE(x == 1);
		REQUIRE(y == 2);
	}
	
// REVISIT mnfitz 14oct2024: std::tie needs reference(?) support from structured bindings
	// SECTION("Structured Binding tie()")
	// {
	// 	saber::geometry::Point<int> point{1,2};
	// 	int x{};
	// 	int y{};
	// 	//std::tie(x, y) = point; // accessor: structured binding!
	// 	/*
	// 	std::tie(x, std::ignore)
	// 	std::tie(std::ignore, y)
	// 	*/
	// 	REQUIRE(x == 1);
	// 	REQUIRE(y == 2);
	// }
	// 
}

TEST_CASE("saber::geometry::size", "[saber]")
{
	SECTION("Default Construct")
	{
		saber::geometry::Size<int> size{};
		REQUIRE(size.Width() == 0);
		REQUIRE(size.Height() == 0);
	}

	SECTION("Alt Construct")
	{
		saber::geometry::Size<int> size{1, 2};
		REQUIRE(size.Width() == 1);
		REQUIRE(size.Height() == 2);
	}

    SECTION("Copy Construct")
	{
		saber::geometry::Size<int> size1{1,2};
        saber::geometry::Size<int> size2{size1};
		REQUIRE(size2.Width() == 1);
		REQUIRE(size2.Height() == 2);
	}

    SECTION("Copy Assign")
	{
		saber::geometry::Size<int> size1{1,2};
        saber::geometry::Size<int> size2 = size1;
		REQUIRE(size2.Width() == 1);
		REQUIRE(size2.Height() == 2);
	}

    SECTION("Move Construct")
	{
		saber::geometry::Size<int> size1{1,2};
        saber::geometry::Size<int> size2{std::move(size1)};
		REQUIRE(size2.Width() == 1);
		REQUIRE(size2.Height() == 2);
	}

    SECTION("Move Assign")
	{
		saber::geometry::Size<int> size1{1,2};
        saber::geometry::Size<int> size2 = std::move(size1);
		REQUIRE(size2.Width() == 1);
		REQUIRE(size2.Height() == 2);
	}
	
	SECTION("Structured Binding")
	{
		saber::geometry::Size<int> size{1,2};
		const auto [width, height] = size; // accessor: structured binding!
		REQUIRE(width == 1);
		REQUIRE(height == 2);
	}
}

TEST_CASE("saber::geometry::utility", "[saber]")
{
	SECTION("Size -> Point")
	{
		saber::geometry::Size<int> size{1,2};
		auto point = saber::ConvertTo<saber::geometry::Point<int>>(size);
		REQUIRE(point.X() == 1);
		REQUIRE(point.Y() == 2);
	}

	SECTION("Point -> Size")
	{
		saber::geometry::Point<int> point{1,2};
		auto size = saber::ConvertTo<saber::geometry::Size<int>>(point);
		REQUIRE(size.Width() == 1);
		REQUIRE(size.Height() == 2);
	}
}

TEST_CASE("saber::geometry::operators Size", "[saber]")
{
	SECTION("Size += Size")
	{
		saber::geometry::Size<int> size1{1,2};
		saber::geometry::Size<int> size2{1,2};
		size1 += size2;
		REQUIRE(size1.Width() == 2);
		REQUIRE(size1.Height() == 4);
	}

	SECTION("Size -= Size")
	{
		saber::geometry::Size<int> size1{2,4};
		saber::geometry::Size<int> size2{1,2};
		size1 -= size2;
		REQUIRE(size1.Width() == 1);
		REQUIRE(size1.Height() == 2);
	}

	SECTION("Size *= Size")
	{
		saber::geometry::Size<int> size1{1,2};
		saber::geometry::Size<int> size2{1,2};
		size1 *= size2;
		REQUIRE(size1.Width() == 1);
		REQUIRE(size1.Height() == 4);
	}

	SECTION("Size /= Size")
	{
		saber::geometry::Size<int> size1{2,4};
		saber::geometry::Size<int> size2{1,2};
		size1 /= size2;
		REQUIRE(size1.Width() == 2);
		REQUIRE(size1.Height() == 2);
	}

	SECTION("Size + Size")
	{
		constexpr saber::geometry::Size<int> size1{1,2};
		constexpr saber::geometry::Size<int> size2{1,2};
		
#if __cplusplus >= 202002L // C++20 or greater
		// Check compiletime correctness
		constexpr auto result1 = size1 + size2;
		REQUIRE(result1.Width() == 2);
		REQUIRE(result1.Height() == 4);
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		const auto result2 = size1 + size2;
		REQUIRE(result2.Width() == 2);
		REQUIRE(result2.Height() == 4);
	}

	SECTION("+Size")
	{
		constexpr saber::geometry::Size<int> size1{1,2};

#if __cplusplus >= 202002L // C++20 or greater
		// Check compiletime correctness
		constexpr auto result1 = +size1;
		REQUIRE(result1.Width() == 1);
		REQUIRE(result1.Height() == 2);
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		const auto result2 = +size1;
		REQUIRE(result2.Width() == 1);
		REQUIRE(result2.Height() == 2);
	}

	SECTION("-Size")
	{
		constexpr saber::geometry::Size<int> size1{1,2};

#if __cplusplus >= 202002L // C++20 or greater
		// Check compiletime correctness
		constexpr auto result1 = -size1;
		REQUIRE(result1.Width() == -1);
		REQUIRE(result1.Height() == -2);
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		const auto result2 = -size1;
		REQUIRE(result2.Width() == -1);
		REQUIRE(result2.Height() == -2);
	}

	SECTION("Size - Size")
	{
		constexpr saber::geometry::Size<int> size1{2,4};
		constexpr saber::geometry::Size<int> size2{1,2};

#if __cplusplus >= 202002L // C++20 or greater
		// Check compiletime correctness
		constexpr auto result1 = size1 - size2;
		REQUIRE(result1.Width() == 1);
		REQUIRE(result1.Height() == 2);
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		const auto result2 = size1 - size2;
		REQUIRE(result2.Width() == 1);
		REQUIRE(result2.Height() == 2);
	}

	SECTION("Size * Size")
	{
		constexpr saber::geometry::Size<int> size1{1,2};
		constexpr saber::geometry::Size<int> size2{1,2};
		
#if __cplusplus >= 202002L // C++20 or greater
		// Check compiletime correctness
		constexpr auto result = size1 * size2;
		REQUIRE(result.Width() == 1);
		REQUIRE(result.Height() == 4);
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		const auto result2 = size1 * size2;
		REQUIRE(result2.Width() == 1);
		REQUIRE(result2.Height() == 4);

	}

	SECTION("Size / Size")
	{
		constexpr saber::geometry::Size<int> size1{2,4};
		constexpr saber::geometry::Size<int> size2{1,2};

#if __cplusplus >= 202002L // C++20 or greater
		// Check compiletime correctness
		constexpr auto result = size1 / size2;
		REQUIRE(result.Width() == 2);
		REQUIRE(result.Height() == 2);
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		const auto result2 = size1 / size2;
		REQUIRE(result2.Width() == 2);
		REQUIRE(result2.Height() == 2);
	}

	SECTION("Size == Size")
	{
		saber::geometry::Size<int> size1{1,2};
		saber::geometry::Size<int> size2{1,2};
		auto result = size1 == size2;
		REQUIRE(result);

		saber::geometry::Size<double> size3{1.0,2.0};
		saber::geometry::Size<double> size4{1.0,2.0};
		auto result2 = size3 == size4;
		REQUIRE(result2);
	}

	SECTION("Size<float> Inexact::EQ Size<float>")
	{
		saber::geometry::Size<float> size1{3.6f, 3.6f};
		saber::geometry::Size<float> size2{3.0f, 3.0f};
		saber::geometry::Size<float> size3 = saber::geometry::Scale(size2, 1.20f);
		auto result = size1 == size3;

		REQUIRE(result);
	}

	SECTION("Size<double> Inexact::EQ Size<double>")
	{
		saber::geometry::Size<double> size1{3.6, 3.6};
		saber::geometry::Size<double> size2{3.0, 3.0};
		saber::geometry::Size<double> size3 = saber::geometry::Scale(size2, 1.20);
		auto result = size1 == size3;

		REQUIRE(result);
	}

	SECTION("Size != Size")
	{
		saber::geometry::Size<int> size1{1,2};
		saber::geometry::Size<int> size2{2,2};
		auto result = size1 != size2;
		REQUIRE(result);

		saber::geometry::Size<double> size3{1.0,2.0};
		saber::geometry::Size<double> size4{2.0,2.0};
		auto result2 = size3 != size4;
		REQUIRE(result2);
	}
}

TEST_CASE("saber::geometry::operators Point", "[saber]")
{
	SECTION("Point += Point")
	{
		saber::geometry::Point<int> point1{1,2};
		saber::geometry::Point<int> point2{1,2};
		point1 += point2;
		REQUIRE(point1.X() == 2);
		REQUIRE(point1.Y() == 4);
	}

	SECTION("Point -= Point")
	{
		saber::geometry::Point<int> point1{2,4};
		saber::geometry::Point<int> point2{1,2};
		point1 -= point2;
		REQUIRE(point1.X() == 1);
		REQUIRE(point1.Y() == 2);
	}

	SECTION("Point *= Point")
	{
		saber::geometry::Point<int> point1{1,2};
		saber::geometry::Point<int> point2{1,2};
		point1 *= point2;
		REQUIRE(point1.X() == 1);
		REQUIRE(point1.Y() == 4);
	}

	SECTION("Point /= Point")
	{
		saber::geometry::Point<int> point1{2,4};
		saber::geometry::Point<int> point2{1,2};
		point1 /= point2;
		REQUIRE(point1.X() == 2);
		REQUIRE(point1.Y() == 2);
	}

	SECTION("Point + Point")
	{
		constexpr saber::geometry::Point<int> point1{1,2};
		constexpr saber::geometry::Point<int> point2{1,2};

#if __cplusplus >= 202002L // C++20 or greater
		// Check compiletime correctness
		constexpr auto result = point1 + point2;
		REQUIRE(result.X() == 2);
		REQUIRE(result.Y() == 4);
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		const auto result2 = point1 + point2;
		REQUIRE(result2.X() == 2);
		REQUIRE(result2.Y() == 4);
	}

	SECTION("Point - Point")
	{
		constexpr saber::geometry::Point<int> point1{2,4};
		constexpr saber::geometry::Point<int> point2{1,2};

#if __cplusplus >= 202002L // C++20 or greater
		// Check compiletime correctness
		constexpr auto result = point1 - point2;
		REQUIRE(result.X() == 1);
		REQUIRE(result.Y() == 2);
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		const auto result2 = point1 - point2;
		REQUIRE(result2.X() == 1);
		REQUIRE(result2.Y() == 2);
	}

	SECTION("Point * Point")
	{
		constexpr saber::geometry::Point<int> point1{1,2};
		constexpr saber::geometry::Point<int> point2{1,2};

#if __cplusplus >= 202002L // C++20 or greater
		// Check compiletime correctness
		constexpr auto result = point1 * point2;
		REQUIRE(result.X() == 1);
		REQUIRE(result.Y() == 4);
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		const auto result2 = point1 * point2;
		REQUIRE(result2.X() == 1);
		REQUIRE(result2.Y() == 4);
	}

	SECTION("Point / Point")
	{
		constexpr saber::geometry::Point<int> point1{2,4};
		constexpr saber::geometry::Point<int> point2{1,2};

#if __cplusplus >= 202002L // C++20 or greater
		// Check compiletime correctness
		constexpr auto result = point1 / point2;
		REQUIRE(result.X() == 2);
		REQUIRE(result.Y() == 2);
#endif // __cplusplus >= 202002L

		// Check runtime correctness
		const auto result2 = point1 / point2;
		REQUIRE(result2.X() == 2);
		REQUIRE(result2.Y() == 2);
	}

	SECTION("Point == Point")
	{
		saber::geometry::Point<int> point1{1,2};
		saber::geometry::Point<int> point2{1,2};
		auto result = point1 == point2;
		REQUIRE(result);

		saber::geometry::Point<float> point3{1.0f,2.0f};
		saber::geometry::Point<float> point4{1.0f,2.0f};		
		auto result2 = point3 == point4;
		REQUIRE(result2);
	}

	SECTION("Point<float> Inexact::EQ Point<float>")
	{
		saber::geometry::Point<float> point1{3.6f, 3.6f};
		saber::geometry::Point<float> point2{3.0f, 3.0f};
		saber::geometry::Point<float> point3 = saber::geometry::Scale(point2, 1.20f);
		auto result = point1 == point3;

		REQUIRE(result);
	}

	SECTION("Point<double> Inexact::EQ Point<double>")
	{
		saber::geometry::Point<double> point1{3.6, 3.6};
		saber::geometry::Point<double> point2{3.0, 3.0};
		saber::geometry::Point<double> point3 = saber::geometry::Scale(point2, 1.20);
		auto result = point1 == point3;

		REQUIRE(result);
	}

	SECTION("Point != Point")
	{
		saber::geometry::Point<int> point1{1,2};
		saber::geometry::Point<int> point2{2,4};
		auto result = point1 != point2;
		REQUIRE(result);

		saber::geometry::Point<float> point3{1.0f,2.0f};
		saber::geometry::Point<float> point4{2.0f,4.0f};
		auto result2 = point3 != point4;
		REQUIRE(result2);
	}

	SECTION("Translate Point -> Point")
	{
		saber::geometry::Point<int> point1{1,2};
		saber::geometry::Point<int> point2{2,4};

		saber::geometry::Point<int> expected{3,6};

		auto result = saber::geometry::Translate(point1, point2) == expected;
		REQUIRE(result);
	}

	SECTION("Translate Point -> X, Y")
	{
		saber::geometry::Point<int> point1{1,2};
		const int x = 2;
		const int y = 4;

		saber::geometry::Point<int> expected{3,6};

		auto result = saber::geometry::Translate(point1, x, y) == expected;
		REQUIRE(result);
	}

	SECTION("Translate Point -> X")
	{
		saber::geometry::Point<int> point1{1,2};
		const int x = 2; 

		saber::geometry::Point<int> expected{3,4};

		auto result = saber::geometry::Translate(point1, x) == expected;
		REQUIRE(result);
	}

	SECTION("Scale Point -> Point")
	{
		saber::geometry::Point<int> point1{1,2};
		saber::geometry::Point<int> point2{2,4};

		saber::geometry::Point<int> expected{2,8};

		auto result = saber::geometry::Scale(point1, point2) == expected;
		REQUIRE(result);
	}

	SECTION("Scale Point -> X, Y")
	{
		saber::geometry::Point<int> point1{1,2};
		const int x = 2;
		const int y = 4;

		saber::geometry::Point<int> expected{2,8};

		auto result = saber::geometry::Scale(point1, x, y) == expected;
		REQUIRE(result);
	}

	SECTION("Scale Point -> X")
	{
		saber::geometry::Point<int> point1{1,2};
		const int x = 2;

		saber::geometry::Point<int> expected{2,4};

		auto result = saber::geometry::Scale(point1, x) == expected;
		REQUIRE(result);
	}

	SECTION("Enlarge Size -> Size")
	{
		saber::geometry::Size<int> size1{1,2};
		saber::geometry::Size<int> size2{2,4};

		saber::geometry::Size<int> expected{3,6};

		auto result = saber::geometry::Enlarge(size1, size2) == expected;
		REQUIRE(result);
	}

	SECTION("Enlarge Size -> X, Y")
	{
		saber::geometry::Size<int> size1{1,2};
		const int x = 2;
		const int y = 4;

		saber::geometry::Size<int> expected{3,6};

		auto result = saber::geometry::Enlarge(size1, x, y) == expected;
		REQUIRE(result);
	}

	SECTION("Enlarge Size -> X")
	{
		saber::geometry::Size<int> size1{1,2};
		const int x = 2; 

		saber::geometry::Size<int> expected{3,4};

		auto result = saber::geometry::Enlarge(size1, x) == expected;
		REQUIRE(result);
	}

	SECTION("Scale Size -> Size")
	{
		saber::geometry::Size<int> size1{1,2};
		saber::geometry::Size<int> size2{2,4};

		saber::geometry::Size<int> expected{2,8};

		auto result = saber::geometry::Scale(size1, size2) == expected;
		REQUIRE(result);
	}

	SECTION("Scale Size -> X, Y")
	{
		saber::geometry::Size<int> size1{1,2};
		const int x = 2;
		const int y = 4;

		saber::geometry::Size<int> expected{2,8};

		auto result = saber::geometry::Scale(size1, x, y) == expected;
		REQUIRE(result);
	}

	SECTION("Scale Size -> X")
	{
		saber::geometry::Size<int> size1{1,2};
		const int x = 2;

		saber::geometry::Size<int> expected{2,4};

		auto result = saber::geometry::Scale(size1, x) == expected;
		REQUIRE(result);
	}
}

TEST_CASE("saber::geometry::Inexact::Eq float", "[saber]")
{
	SECTION("float Eq float")
	{
		float test1 = 3.6f;
		float test2 = 3 * 1.20f;
 		auto result = saber::Inexact::IsEq(test1, test2);
		REQUIRE(result);
	}

	SECTION("double Eq double")
	{
		double test1 = 3.6;
		double test2 = 3 * 1.20;
 		auto result = saber::Inexact::IsEq(test1, test2);
		REQUIRE(result);
	}

	SECTION("float !Eq float")
	{
		float test1 = 3.59f;
		float test2 = 3 * 1.20f;
 		auto result = saber::Inexact::IsEq(test1, test2);

		REQUIRE(!result);
	}

	SECTION("double !Eq double")
	{
		double test1 = 3.59;
		double test2 = 3 * 1.20;
 		auto result = saber::Inexact::IsEq(test1, test2);
		REQUIRE(!result);
	}
}

TEST_CASE("saber::geometry::Inexact::Ee float", "[saber]")
{
	SECTION("float Ne float")
	{
		float test1 = 3.59f;
		float test2 = 3 * 1.20f;
 		auto result = saber::Inexact::IsNe(test1, test2);
		REQUIRE(result);
	}

	SECTION("double Ne double")
	{
		double test1 = 3.59;
		double test2 = 3 * 1.20;
 		auto result = saber::Inexact::IsNe(test1, test2);
		REQUIRE(result);
	}

	SECTION("float !Ne float")
	{
		float test1 = 3.6f;
		float test2 = 3 * 1.20f;
 		auto result = saber::Inexact::IsNe(test1, test2);

		REQUIRE(!result);
	}

	SECTION("double !Ne double")
	{
		double test1 = 3.6;
		double test2 = 3 * 1.20;
 		auto result = saber::Inexact::IsNe(test1, test2);
		REQUIRE(!result);
	}

	/* 
	// Compile time test only
	SECTION("Non-Float FAIL TEST")
	{
		std::string test1 = "fail";
		std::string test2 = "test";
 		auto result = saber::Inexact::IsNe(test1, test2);
		REQUIRE(!result);
	}
	*/
}
