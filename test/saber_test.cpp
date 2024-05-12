
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0

#include "catch2/catch_test_macros.hpp"
#if 0
static int Factorial( int number )
{
//	return number <= 1 ? number : Factorial( number - 1 ) * number;  // fail
 return number <= 1 ? 1      : Factorial( number - 1 ) * number;  // pass
}

TEST_CASE( "Factorial of 0 is 1 (fail)", "[single-file]" )
{
	REQUIRE( Factorial(0) == 1 );
}

TEST_CASE( "Factorials of 1 and higher are computed (pass)", "[single-file]" )
{
	REQUIRE( Factorial(1) == 1 );
	REQUIRE( Factorial(2) == 2 );
	REQUIRE( Factorial(3) == 6 );
	REQUIRE( Factorial(10) == 3628800 );
}

// Compile & run:
// - g++ -std=c++14 -Wall -I$(CATCH_SINGLE_INCLUDE) -o saber_test saber_test.cpp && saber_test --success
// - cl -EHsc -I%CATCH_SINGLE_INCLUDE% saber_test.cpp && saber_test --success

// Expected compact output (all assertions):
//
// prompt> saber_test --reporter compact --success
// saber_test.cpp:14: failed: Factorial(0) == 1 for: 0 == 1
// saber_test.cpp:18: passed: Factorial(1) == 1 for: 1 == 1
// saber_test.cpp:19: passed: Factorial(2) == 2 for: 2 == 2
// saber_test.cpp:20: passed: Factorial(3) == 6 for: 6 == 6
// saber_test.cpp:21: passed: Factorial(10) == 3628800 for: 3628800 (0x375f00) == 3628800 (0x375f00)
// Failed 1 test case, failed 1 assertion.

TEST_CASE( "vectors can be sized and resized", "[vector]" )
{
	// This setup will be done 4 times in total, once for each section
	std::vector<int> v( 5 );

	REQUIRE( v.size() == 5 );
	REQUIRE( v.capacity() >= 5 );

	SECTION( "resizing bigger changes size and capacity" )
	{
		v.resize( 10 );

		REQUIRE( v.size() == 10 );
		REQUIRE( v.capacity() >= 10 );
	}
	SECTION( "resizing smaller changes size but not capacity" )
	{
		v.resize( 0 );

		REQUIRE( v.size() == 0 );
		REQUIRE( v.capacity() >= 5 );
	}
	SECTION( "reserving bigger changes capacity but not size" )
	{
		v.reserve( 10 );

		REQUIRE( v.size() == 5 );
		REQUIRE( v.capacity() >= 10 );
	}
	SECTION( "reserving smaller does not change size or capacity" )
	{
		v.reserve( 0 );

		REQUIRE( v.size() == 5 );
		REQUIRE( v.capacity() >= 5 );
	}
	SECTION( "reserving bigger changes capacity but not size" )
	{
		v.reserve( 10 );

		REQUIRE( v.size() == 5 );
		REQUIRE( v.capacity() >= 10 );
		SECTION( "reserving down unused capacity does not change capacity" )
		{
			v.reserve( 7 );
			REQUIRE( v.size() == 5 );
			REQUIRE( v.capacity() >= 10 );
		}
	}
}
#endif
