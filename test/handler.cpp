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
#include "saber/handler.hpp"

TEST_CASE( "saber::ValueHandler", "[saber]" )
{
	int test1 = 0;

	SECTION("Save/Restore Via Destructor")
	{
		REQUIRE(test1 == 0);
		{
			saber::ValueHandler<int> testHandler{ test1, 1 };
			REQUIRE(test1 == 1);
		}
		REQUIRE(test1 == 0);
	}

	SECTION("Save/Restore Via Reset")
	{
		REQUIRE(test1 == 0);
		
		saber::ValueHandler<int> testHandler{ test1, 10 };
		REQUIRE(test1 == 10);
		
		testHandler.Reset();
		REQUIRE(test1 == 0);
	}

	std::vector<std::string> test2{ "a","b","c" };
	std::vector<std::string> test3{ "d","e","f" };
	std::vector<std::string> original{};

	SECTION("Save/Restore std::vector Via Destructor")
	{
		original = test2;
		REQUIRE(original == test2);
		{
			saber::ValueHandler<std::vector<std::string>> testHandler{ test2, test3 };
			REQUIRE(test3 == test2);
		}
		REQUIRE(original == test2);
	}

	SECTION("Save/Restore std::vector Via Reset")
	{
		original = test2;
		REQUIRE(original == test2);
		
		saber::ValueHandler<std::vector<std::string>> testHandler{ test2, test3 };
		REQUIRE(test3 == test2);
		
		testHandler.Reset();
		REQUIRE(original == test2);
	}
}