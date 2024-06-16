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
}