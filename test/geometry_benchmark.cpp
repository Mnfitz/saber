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
#include <catch2/benchmark/catch_benchmark.hpp>

// saber
#include "saber/geometry/point.hpp"
#include "saber/geometry/size.hpp"

// std
#include <array>
#include <ctime>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

saber::geometry::Point<float> sFloatPoint{};
saber::geometry::Point<double> sDoublePoint{};
saber::geometry::Point<int> sIntPoint{};

TEST_CASE("saber::geometry::Point", "[saber]")
{
	int gauranteedNotConstexpr = 0;
	{
		// Get current time
		std::time_t t = std::time(nullptr);
		std::tm* now = std::localtime(&t);
		const auto dayOfWeek = now->tm_wday;
		gauranteedNotConstexpr = dayOfWeek+1; // +1: -> [1...8]
	}

	saber::geometry::Point<float> floatPoint1{static_cast<float>(gauranteedNotConstexpr), -static_cast<float>(gauranteedNotConstexpr)};
	saber::geometry::Point<float> floatPoint2{2.0f, -3.0f};

	BENCHMARK("saber::geometry::Point<float> operator+")
	{
		auto floatPoint = floatPoint1 + floatPoint2;
		floatPoint += floatPoint + floatPoint1;
		floatPoint += floatPoint + floatPoint2;
		floatPoint += floatPoint + floatPoint1;
		floatPoint += floatPoint + floatPoint2;
		floatPoint += floatPoint + floatPoint1;
		floatPoint += floatPoint + floatPoint2;
		floatPoint += floatPoint + floatPoint1;
		floatPoint += floatPoint + floatPoint2;
		sFloatPoint += floatPoint;
	};

	BENCHMARK("saber::geometry::Point<float> operator-")
	{
		auto floatPoint = floatPoint1 - floatPoint2;
		floatPoint -= floatPoint - floatPoint1;
		floatPoint -= floatPoint - floatPoint2;
		floatPoint -= floatPoint - floatPoint1;
		floatPoint -= floatPoint - floatPoint2;
		floatPoint -= floatPoint - floatPoint1;
		floatPoint -= floatPoint - floatPoint2;
		floatPoint -= floatPoint - floatPoint1;
		floatPoint -= floatPoint - floatPoint2;
		sFloatPoint -= floatPoint;
	};

	BENCHMARK("saber::geometry::Point<float> operator*")
	{
		auto floatPoint = floatPoint1 * floatPoint2;
		floatPoint *= floatPoint * floatPoint1;
		floatPoint *= floatPoint * floatPoint2;
		floatPoint *= floatPoint * floatPoint1;
		floatPoint *= floatPoint * floatPoint2;
		floatPoint *= floatPoint * floatPoint1;
		floatPoint *= floatPoint * floatPoint2;
		floatPoint *= floatPoint * floatPoint1;
		floatPoint *= floatPoint * floatPoint2;
		sFloatPoint *= floatPoint;
	};

	floatPoint1 = {1.0f, static_cast<float>(gauranteedNotConstexpr/gauranteedNotConstexpr)};
	floatPoint2 = {1.0f, -1.0f};
	BENCHMARK("saber::geometry::Point<float> operator/")
	{
		auto floatPoint = floatPoint1 / floatPoint2;
		floatPoint /= floatPoint / floatPoint1;
		floatPoint /= floatPoint / floatPoint2;
		floatPoint /= floatPoint / floatPoint1;
		floatPoint /= floatPoint / floatPoint2;
		floatPoint /= floatPoint / floatPoint1;
		floatPoint /= floatPoint / floatPoint2;
		floatPoint /= floatPoint / floatPoint1;
		floatPoint /= floatPoint / floatPoint2;
		sFloatPoint /= floatPoint;
	};

	saber::geometry::Point<double> doublePoint1{ static_cast<double>(gauranteedNotConstexpr), -static_cast<double>(gauranteedNotConstexpr)};
	saber::geometry::Point<double> doublePoint2{2.0, -3.0};

	saber::geometry::Point<double> doublePoint3{};
	BENCHMARK("saber::geometry::Point<double> operator+")
	{
		auto doublePoint = doublePoint1 + doublePoint2;
		doublePoint += doublePoint + doublePoint1;
		doublePoint += doublePoint + doublePoint2;
		doublePoint += doublePoint + doublePoint1;
		doublePoint += doublePoint + doublePoint2;
		doublePoint += doublePoint + doublePoint1;
		doublePoint += doublePoint + doublePoint2;
		doublePoint += doublePoint + doublePoint1;
		doublePoint += doublePoint + doublePoint2;
		sDoublePoint += doublePoint;
	};

	BENCHMARK("saber::geometry::Point<double> operator-")
	{
		auto doublePoint = doublePoint1 - doublePoint2;
		doublePoint -= doublePoint - doublePoint1;
		doublePoint -= doublePoint - doublePoint2;
		doublePoint -= doublePoint - doublePoint1;
		doublePoint -= doublePoint - doublePoint2;
		doublePoint -= doublePoint - doublePoint1;
		doublePoint -= doublePoint - doublePoint2;
		doublePoint -= doublePoint - doublePoint1;
		doublePoint -= doublePoint - doublePoint2;
		sDoublePoint -= doublePoint;
	};

	BENCHMARK("saber::geometry::Point<double> operator*")
	{
		auto doublePoint = doublePoint1 * doublePoint2;
		doublePoint *= doublePoint * doublePoint1;
		doublePoint *= doublePoint * doublePoint2;
		doublePoint *= doublePoint * doublePoint1;
		doublePoint *= doublePoint * doublePoint2;
		doublePoint *= doublePoint * doublePoint1;
		doublePoint *= doublePoint * doublePoint2;
		doublePoint *= doublePoint * doublePoint1;
		doublePoint *= doublePoint * doublePoint2;
		sDoublePoint *= doublePoint;
	};

	doublePoint1 = {1.0, static_cast<double>(gauranteedNotConstexpr/gauranteedNotConstexpr)};
	doublePoint2 = {1.0, -1.0};
	BENCHMARK("saber::geometry::Point<double> operator/")
	{
		auto doublePoint = doublePoint1 / doublePoint2;
		doublePoint /= doublePoint / doublePoint1;
		doublePoint /= doublePoint / doublePoint2;
		doublePoint /= doublePoint / doublePoint1;
		doublePoint /= doublePoint / doublePoint2;
		doublePoint /= doublePoint / doublePoint1;
		doublePoint /= doublePoint / doublePoint2;
		doublePoint /= doublePoint / doublePoint1;
		doublePoint /= doublePoint / doublePoint2;
		sDoublePoint /= doublePoint;
	};

	saber::geometry::Point<int> intPoint1{static_cast<int>(gauranteedNotConstexpr), -static_cast<int>(gauranteedNotConstexpr)};
	saber::geometry::Point<int> intPoint2{2, -3};

	saber::geometry::Point<int> intPoint3{};
	BENCHMARK("saber::geometry::Point<int> operator+")
	{
		auto intPoint = intPoint1 + intPoint2;
		intPoint += intPoint + intPoint1;
		intPoint += intPoint + intPoint2;
		intPoint += intPoint + intPoint1;
		intPoint += intPoint + intPoint2;
		intPoint += intPoint + intPoint1;
		intPoint += intPoint + intPoint2;
		intPoint += intPoint + intPoint1;
		intPoint += intPoint + intPoint2;
		sIntPoint += intPoint;
	};

	BENCHMARK("saber::geometry::Point<int> operator-")
	{
		auto intPoint = intPoint1 - intPoint2;
		intPoint -= intPoint - intPoint1;
		intPoint -= intPoint - intPoint2;
		intPoint -= intPoint - intPoint1;
		intPoint -= intPoint - intPoint2;
		intPoint -= intPoint - intPoint1;
		intPoint -= intPoint - intPoint2;
		intPoint -= intPoint - intPoint1;
		intPoint -= intPoint - intPoint2;
		sIntPoint -= intPoint;
	};

	BENCHMARK("saber::geometry::Point<int> operator*")
	{
		auto intPoint = intPoint1 * intPoint2;
		intPoint *= intPoint * intPoint1;
		intPoint *= intPoint * intPoint2;
		intPoint *= intPoint * intPoint1;
		intPoint *= intPoint * intPoint2;
		intPoint *= intPoint * intPoint1;
		intPoint *= intPoint * intPoint2;
		intPoint *= intPoint * intPoint1;
		intPoint *= intPoint * intPoint2;
		sIntPoint *= intPoint;
	};

	intPoint1 = {1, static_cast<int>(gauranteedNotConstexpr/gauranteedNotConstexpr)};
	intPoint2 = {1, -1};
	BENCHMARK("saber::geometry::Point<int> operator/")
	{
		auto intPoint = intPoint1 / intPoint2;
		intPoint /= intPoint / intPoint1;
		intPoint /= intPoint / intPoint2;
		intPoint /= intPoint / intPoint1;
		intPoint /= intPoint / intPoint2;
		intPoint /= intPoint / intPoint1;
		intPoint /= intPoint / intPoint2;
		intPoint /= intPoint / intPoint1;
		intPoint /= intPoint / intPoint2;
		sIntPoint /= intPoint;
	};
}

saber::geometry::Size<float> sFloatSize{};
saber::geometry::Size<double> sDoubleSize{};
saber::geometry::Size<int> sIntSize{};


TEST_CASE("saber::geometry::Size", "[saber]")
{
	int gauranteedNotConstexpr = 0;
	{
		// Get current time
		std::time_t t = std::time(nullptr);
		std::tm* now = std::localtime(&t);
		const auto dayOfWeek = now->tm_wday;
		gauranteedNotConstexpr = dayOfWeek + 1; // +1: -> [1...8]
	}

	saber::geometry::Size<float> floatSize1{static_cast<float>(gauranteedNotConstexpr), -static_cast<float>(gauranteedNotConstexpr)};
	saber::geometry::Size<float> floatSize2{2.0f, -3.0f};

	BENCHMARK("saber::geometry::Size<float> operator+")
	{
		auto floatSize = floatSize1 + floatSize2;
		floatSize += floatSize + floatSize1;
		floatSize += floatSize + floatSize2;
		floatSize += floatSize + floatSize1;
		floatSize += floatSize + floatSize2;
		floatSize += floatSize + floatSize1;
		floatSize += floatSize + floatSize2;
		floatSize += floatSize + floatSize1;
		floatSize += floatSize + floatSize2;
		sFloatSize += floatSize;
	};

	BENCHMARK("saber::geometry::Size<float> operator-")
	{
		auto floatSize = floatSize1 - floatSize2;
		floatSize -= floatSize - floatSize1;
		floatSize -= floatSize - floatSize2;
		floatSize -= floatSize - floatSize1;
		floatSize -= floatSize - floatSize2;
		floatSize -= floatSize - floatSize1;
		floatSize -= floatSize - floatSize2;
		floatSize -= floatSize - floatSize1;
		floatSize -= floatSize - floatSize2;
		sFloatSize -= floatSize;
	};

	BENCHMARK("saber::geometry::Size<float> operator*")
	{
		auto floatSize = floatSize1 * floatSize2;
		floatSize *= floatSize * floatSize1;
		floatSize *= floatSize * floatSize2;
		floatSize *= floatSize * floatSize1;
		floatSize *= floatSize * floatSize2;
		floatSize *= floatSize * floatSize1;
		floatSize *= floatSize * floatSize2;
		floatSize *= floatSize * floatSize1;
		floatSize *= floatSize * floatSize2;
		sFloatSize *= floatSize;
	};

	floatSize1 = { 1.0f, static_cast<float>(gauranteedNotConstexpr / gauranteedNotConstexpr) };
	floatSize2 = { 1.0f, -1.0f };
	BENCHMARK("saber::geometry::Size<float> operator/")
	{
		auto floatSize = floatSize1 / floatSize2;
		floatSize /= floatSize / floatSize1;
		floatSize /= floatSize / floatSize2;
		floatSize /= floatSize / floatSize1;
		floatSize /= floatSize / floatSize2;
		floatSize /= floatSize / floatSize1;
		floatSize /= floatSize / floatSize2;
		floatSize /= floatSize / floatSize1;
		floatSize /= floatSize / floatSize2;
		sFloatSize /= floatSize;
	};

	saber::geometry::Size<double> doubleSize1{ static_cast<double>(gauranteedNotConstexpr), -static_cast<double>(gauranteedNotConstexpr) };
	saber::geometry::Size<double> doubleSize2{ 2.0, -3.0 };

	saber::geometry::Size<double> doubleSize3{};
	BENCHMARK("saber::geometry::Size<double> operator+")
	{
		auto doubleSize = doubleSize1 + doubleSize2;
		doubleSize += doubleSize + doubleSize1;
		doubleSize += doubleSize + doubleSize2;
		doubleSize += doubleSize + doubleSize1;
		doubleSize += doubleSize + doubleSize2;
		doubleSize += doubleSize + doubleSize1;
		doubleSize += doubleSize + doubleSize2;
		doubleSize += doubleSize + doubleSize1;
		doubleSize += doubleSize + doubleSize2;
		sDoubleSize += doubleSize;
	};

	BENCHMARK("saber::geometry::Size<double> operator-")
	{
		auto doubleSize = doubleSize1 - doubleSize2;
		doubleSize -= doubleSize - doubleSize1;
		doubleSize -= doubleSize - doubleSize2;
		doubleSize -= doubleSize - doubleSize1;
		doubleSize -= doubleSize - doubleSize2;
		doubleSize -= doubleSize - doubleSize1;
		doubleSize -= doubleSize - doubleSize2;
		doubleSize -= doubleSize - doubleSize1;
		doubleSize -= doubleSize - doubleSize2;
		sDoubleSize -= doubleSize;
	};

	BENCHMARK("saber::geometry::Size<double> operator*")
	{
		auto doubleSize = doubleSize1 * doubleSize2;
		doubleSize *= doubleSize * doubleSize1;
		doubleSize *= doubleSize * doubleSize2;
		doubleSize *= doubleSize * doubleSize1;
		doubleSize *= doubleSize * doubleSize2;
		doubleSize *= doubleSize * doubleSize1;
		doubleSize *= doubleSize * doubleSize2;
		doubleSize *= doubleSize * doubleSize1;
		doubleSize *= doubleSize * doubleSize2;
		sDoubleSize *= doubleSize;
	};

	doubleSize1 = { 1.0, static_cast<double>(gauranteedNotConstexpr / gauranteedNotConstexpr) };
	doubleSize2 = { 1.0, -1.0 };
	BENCHMARK("saber::geometry::Size<double> operator/")
	{
		auto doubleSize = doubleSize1 / doubleSize2;
		doubleSize /= doubleSize / doubleSize1;
		doubleSize /= doubleSize / doubleSize2;
		doubleSize /= doubleSize / doubleSize1;
		doubleSize /= doubleSize / doubleSize2;
		doubleSize /= doubleSize / doubleSize1;
		doubleSize /= doubleSize / doubleSize2;
		doubleSize /= doubleSize / doubleSize1;
		doubleSize /= doubleSize / doubleSize2;
		sDoubleSize /= doubleSize;
	};

	saber::geometry::Size<int> intSize1{ static_cast<int>(gauranteedNotConstexpr), -static_cast<int>(gauranteedNotConstexpr) };
	saber::geometry::Size<int> intSize2{ 2, -3 };

	saber::geometry::Size<int> intSize3{};
	BENCHMARK("saber::geometry::Size<int> operator+")
	{
		auto intSize = intSize1 + intSize2;
		intSize += intSize + intSize1;
		intSize += intSize + intSize2;
		intSize += intSize + intSize1;
		intSize += intSize + intSize2;
		intSize += intSize + intSize1;
		intSize += intSize + intSize2;
		intSize += intSize + intSize1;
		intSize += intSize + intSize2;
		sIntSize += intSize;
	};

	BENCHMARK("saber::geometry::Size<int> operator-")
	{
		auto intSize = intSize1 - intSize2;
		intSize -= intSize - intSize1;
		intSize -= intSize - intSize2;
		intSize -= intSize - intSize1;
		intSize -= intSize - intSize2;
		intSize -= intSize - intSize1;
		intSize -= intSize - intSize2;
		intSize -= intSize - intSize1;
		intSize -= intSize - intSize2;
		sIntSize -= intSize;
	};

	BENCHMARK("saber::geometry::Size<int> operator*")
	{
		auto intSize = intSize1 * intSize2;
		intSize *= intSize * intSize1;
		intSize *= intSize * intSize2;
		intSize *= intSize * intSize1;
		intSize *= intSize * intSize2;
		intSize *= intSize * intSize1;
		intSize *= intSize * intSize2;
		intSize *= intSize * intSize1;
		intSize *= intSize * intSize2;
		sIntSize *= intSize;
	};

	intSize1 = { 1, static_cast<int>(gauranteedNotConstexpr / gauranteedNotConstexpr) };
	intSize2 = { 1, -1 };
	BENCHMARK("saber::geometry::Size<int> operator/")
	{
		auto intSize = intSize1 / intSize2;
		intSize /= intSize / intSize1;
		intSize /= intSize / intSize2;
		intSize /= intSize / intSize1;
		intSize /= intSize / intSize2;
		intSize /= intSize / intSize1;
		intSize /= intSize / intSize2;
		intSize /= intSize / intSize1;
		intSize /= intSize / intSize2;
		sIntSize /= intSize;
	};
}
