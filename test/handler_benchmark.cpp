/////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2025 Matthew Fitzgerald
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
/////////////////////////////////////////////////////////////////////

// catch2
#include "catch2/catch_test_macros.hpp"
#include <catch2/benchmark/catch_benchmark.hpp>

// saber
#include "saber/handler.hpp"

// std
#include <array>
#include <string>
#include <tuple>
#include <vector>

TEST_CASE("saber::ValueHandler", "[saber]")
{
	struct Test
	{
		int mInt32{0};
		long long mInt64{0};
		std::tuple<int, long long> mTuple96{0, 0};
		std::array<int, 4> mArray128{};
		std::string mString{"How now brown cow"};
		std::vector<std::string> mVector{"How", "now", "brown", "cow"};
	} test{};

	BENCHMARK("saber::ValueHandler<int>")
	{
		saber::ValueHandler<int> testHandler{test.mInt32, 1};
		testHandler.Reset();
	};

	BENCHMARK("saber::ValueHandler<long long>")
	{
		saber::ValueHandler<long long> testHandler{test.mInt64, 1};
		testHandler.Reset();
	};

	BENCHMARK("saber::ValueHandler<std::tuple<int, long long>>")
	{
		saber::ValueHandler<std::tuple<int, long long>> testHandler{test.mTuple96, {1, 2}};
		testHandler.Reset();
	};

	BENCHMARK("saber::ValueHandler<std::array<int, 4>>")
	{
		saber::ValueHandler<std::array<int, 4>> testHandler{test.mArray128, {1, 2, 3, 4}};
		testHandler.Reset();
	};

	BENCHMARK("saber::ValueHandler<std::string>")
	{
		saber::ValueHandler<std::string> testHandler{test.mString, "Hello"};
		testHandler.Reset();
	};

	BENCHMARK("saber::ValueHandler<std::vector<std::string>>")
	{
		saber::ValueHandler<std::vector<std::string>> testHandler{test.mVector, {"a", "b", "c", "d"}};
		testHandler.Reset();
	};
}
