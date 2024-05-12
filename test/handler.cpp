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

// std
#include <type_traits>

TEST_CASE( "saber::ValueHandler", "[saber]" )
{
	int test1 = 0;

	SECTION("ValueHandler Not Movable or Copyable")
	{
		constexpr bool kIsCopyConstructable = std::is_trivially_copy_constructible_v<saber::ValueHandler<int>>;
		REQUIRE(!kIsCopyConstructable);
		constexpr bool kIsCopyAssignable = std::is_trivially_copy_assignable_v<saber::ValueHandler<int>>;
		REQUIRE(!kIsCopyAssignable);
		constexpr bool kIsMoveConstructable = std::is_trivially_move_constructible_v<saber::ValueHandler<int>>;
		REQUIRE(!kIsMoveConstructable);
		constexpr bool kIsMoveAssignable = std::is_trivially_move_assignable_v<saber::ValueHandler<int>>;
		REQUIRE(!kIsMoveAssignable);
	}

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

TEST_CASE("saber::ReferenceHandler", "[saber]")
{
	SECTION("Delete std::vector Via Reset()")
	{
		std::vector<std::string> test{"a","b","c"};
		auto test4 = std::make_unique<std::vector<std::string>>(test);
		auto ref1 = test4.release();

		saber::ReferenceHandler<std::vector<std::string>> refHandler{ref1};
		REQUIRE(refHandler.Get() == ref1);

		refHandler.Reset();
		REQUIRE(refHandler.Get() == nullptr);
	}
	
	SECTION("ReferenceHandler Not Copyable")
	{
		constexpr bool kIsCopyConstructable = std::is_trivially_copy_constructible_v<saber::ReferenceHandler<int>>;
		REQUIRE(!kIsCopyConstructable);
		constexpr bool kIsCopyAssignable = std::is_trivially_copy_assignable_v<saber::ReferenceHandler<int>>;
		REQUIRE(!kIsCopyAssignable);
	}

	SECTION("ReferenceHandler Move Assign")
	{
		std::vector<std::string> test{"a","b","c"};
		auto test5 = std::make_unique<std::vector<std::string>>(test);
		auto ref2 = test5.release();
		saber::ReferenceHandler<std::vector<std::string>> refHandler{ref2};
		saber::ReferenceHandler<std::vector<std::string>> emptyHandler{};

		REQUIRE(refHandler.Get() == ref2);
		REQUIRE(emptyHandler.Get() == nullptr);

		emptyHandler = std::move(refHandler);
		REQUIRE(refHandler.Get() == nullptr);
		REQUIRE(emptyHandler.Get() == ref2);
	}

	SECTION("ReferenceHandler Move Construct")
	{
		std::vector<std::string> test{"a","b","c"};
		auto test6 = std::make_unique<std::vector<std::string>>(test);
		auto ref3 = test6.release();
		saber::ReferenceHandler<std::vector<std::string>> refHandler{ref3};

		REQUIRE(refHandler.Get() == ref3);

		saber::ReferenceHandler<std::vector<std::string>> emptyHandler{std::move(refHandler)};
		REQUIRE(refHandler.Get() == nullptr);
		REQUIRE(emptyHandler.Get() == ref3);
	}
}