

#include "saber_unittest.hpp"

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
#include "catch2/matchers/catch_matchers_string.hpp"

// saber
#include "saber/exception.hpp"
#include "saber/hash.hpp"
#include "saber/inexact.hpp"

// std
#include <cstdio>
#include <stdexcept>
#include <string>
#include <unordered_map>

using namespace saber;
TEST_CASE(	"saber::Exception and macros (REQUIRE, ENSURE, ASSERT)",
			"[saber][exception]")
{
	SECTION("SABER_REQUIRE with true condition does not throw")
	{
		bool cond = true;
		bool cond2 = (1 == 1);
		REQUIRE_NOTHROW(SABER_REQUIRE(cond));
		REQUIRE_NOTHROW(SABER_REQUIRE(cond2));
	}

	SECTION("SABER_REQUIRE with false condition throws saber::Exception")
	{
		bool falseCond = false;
		bool falseCond2 = (1 == 2);
		REQUIRE_THROWS_AS(SABER_REQUIRE(falseCond), saber::Exception);
		REQUIRE_THROWS_AS(SABER_REQUIRE(falseCond2), saber::Exception);
	}

	SECTION("SABER_REQUIRE exception message contains 'REQUIRE failed'")
	{
		bool falseCond = false;
		REQUIRE_THROWS_WITH(SABER_REQUIRE(falseCond), Catch::Matchers::ContainsSubstring("REQUIRE failed"));
	}

	SECTION("SABER_ENSURE with true condition does not throw")
	{
		bool cond = true;
		bool cond2 = (1 == 1);
		REQUIRE_NOTHROW(SABER_ENSURE(cond));
		REQUIRE_NOTHROW(SABER_ENSURE(cond2));
	}

	SECTION("SABER_ENSURE with false condition throws saber::Exception")
	{
		bool falseCond = false;
		bool falseCond2 = (1 == 2);
		REQUIRE_THROWS_AS(SABER_ENSURE(falseCond), saber::Exception);
		REQUIRE_THROWS_AS(SABER_ENSURE(falseCond2), saber::Exception);
	}

	SECTION("SABER_ENSURE exception message contains 'ENSURE failed'")
	{
		bool falseCond = false;
		REQUIRE_THROWS_WITH(SABER_ENSURE(falseCond), Catch::Matchers::ContainsSubstring("ENSURE failed"));
	}

	SECTION("saber::Exception derives from std::runtime_error")
	{
		try {
			throw saber::Exception("test message");
		} catch (const std::runtime_error &e) {
			REQUIRE(std::string(e.what()) == "test message");
		}
	}
}
TEST_CASE(	"saber::HashValue<> String hashing",
			"[saber][template]")
{
	SECTION("Hash32{\"string\"}")
	{
		constexpr auto kHello = Hash32{"Hello"};
		REQUIRE(kHello == Hash32{"Hello"});
		REQUIRE(kHello() == Hash32{"Hello"}());
		REQUIRE(kHello.Value() == Hash32{"Hello"}.Value());

		REQUIRE(kHello != Hash32{"Goodbye"});
	}

	SECTION("Hash32{L\"wstring\"}")
	{
		constexpr auto kHello = Hash32{L"Hello"};
		REQUIRE(kHello == Hash32{L"Hello"});
		REQUIRE(kHello() == Hash32{L"Hello"}());
		REQUIRE(kHello.Value() == Hash32{L"Hello"}.Value());

		REQUIRE(kHello != Hash32{L"Goodbye"});
	}

	SECTION("Hash32{char*, size}")
	{
		const std::string kHello{"Hello"};
		REQUIRE(Hash32{kHello.data(), kHello.size()} == Hash32{"Hello"});
		REQUIRE(Hash32{kHello.data(), kHello.size()}() == Hash32{"Hello"}());
		REQUIRE(Hash32{kHello.data(), kHello.size()}.Value() == Hash32{"Hello"}.Value());

		REQUIRE(Hash32{kHello.data(), kHello.size()} != Hash32{"Goodbye"});
	}

	SECTION("Hash32{wchar_t*, size}")
	{
		const std::wstring kHello{L"Hello"};
		REQUIRE(Hash32{kHello.data(), kHello.size()} == Hash32{L"Hello"});
		REQUIRE(Hash32{kHello.data(), kHello.size()}() == Hash32{L"Hello"}());
		REQUIRE(Hash32{kHello.data(), kHello.size()}.Value() == Hash32{L"Hello"}.Value());

		REQUIRE(Hash32{kHello.data(), kHello.size()} != Hash32{L"Goodbye"});
	}

	SECTION("Hash32{}: std::unordered_map<Hash32{key}, value>")
	{
		std::unordered_map<saber::Hash32, std::string> map =
		{
			{Hash32{"Circle"}, "Circle"},
			{Hash32{"Square"}, "Square"},
			{Hash32{"Triangle"}, "Triangle"}
		};

		REQUIRE(map[saber::Hash32{"Circle"}] == "Circle");
		REQUIRE(map[saber::Hash32{"Square"}] == "Square");
		REQUIRE(map[saber::Hash32{"Triangle"}] == "Triangle");
	}

	SECTION("Hash32 (known collisions)")
	{
		REQUIRE(Hash32{"costarring"} == Hash32{"liquid"});
		REQUIRE(Hash32{"declinate"} == Hash32{"macallums"});
		REQUIRE(Hash32{"altarage"} == Hash32{"zinke"});
	}

	SECTION("Hash64{\"string\"}")
	{
		constexpr auto kHello = Hash64{"Hello"};
		REQUIRE(kHello == Hash64{"Hello"});
		REQUIRE(kHello() == Hash64{"Hello"}());
		REQUIRE(kHello.Value() == Hash64{"Hello"}.Value());

		REQUIRE(kHello != Hash64{"Goodbye"});
	}

	SECTION("Hash64{L\"wstring\"}")
	{
		constexpr auto kHello = Hash64{L"Hello"};
		REQUIRE(kHello == Hash64{L"Hello"});
		REQUIRE(kHello() == Hash64{L"Hello"}());
		REQUIRE(kHello.Value() == Hash64{L"Hello"}.Value());

		REQUIRE(kHello != Hash64{L"Goodbye"});
	}

	SECTION("Hash64{char*, size}")
	{
		const std::string kHello{"Hello"};
		REQUIRE(Hash64{kHello.data(), kHello.size()} == Hash64{"Hello"});
		REQUIRE(Hash64{kHello.data(), kHello.size()}() == Hash64{"Hello"}());
		REQUIRE(Hash64{kHello.data(), kHello.size()}.Value() == Hash64{"Hello"}.Value());

		REQUIRE(Hash64{kHello.data(), kHello.size()} != Hash64{"Goodbye"});
	}

	SECTION("Hash64{wchar_t*, size}")
	{
		const std::wstring kHello{L"Hello"};
		REQUIRE(Hash64{kHello.data(), kHello.size()} == Hash64{L"Hello"});
		REQUIRE(Hash64{kHello.data(), kHello.size()}() == Hash64{L"Hello"}());
		REQUIRE(Hash64{kHello.data(), kHello.size()}.Value() == Hash64{L"Hello"}.Value());

		REQUIRE(Hash64{kHello.data(), kHello.size()} != Hash64{L"Goodbye"});
	}

	SECTION("Hash64{}: std::unordered_map<Hash64{key}, value>")
	{
		std::unordered_map<saber::Hash64, std::string> map =
		{
			{Hash64{"Circle"}, "Circle"},
			{Hash64{"Square"}, "Square"},
			{Hash64{"Triangle"}, "Triangle"}
		};

		REQUIRE(map[saber::Hash64{"Circle"}] == "Circle");
		REQUIRE(map[saber::Hash64{"Square"}] == "Square");
		REQUIRE(map[saber::Hash64{"Triangle"}] == "Triangle");
	}
}

TEMPLATE_TEST_CASE(	"saber::Inexact floating point comparisons",
					"[saber][template]",
					int, float, double)
{
	SECTION("Inexact::IsEq()")
	{
		REQUIRE(Inexact::IsEq(3.60, (3.0 * 1.2)));
	}

	SECTION("!Inexact::IsEq()")
	{
		REQUIRE(!Inexact::IsEq(3.59, (3.0 * 1.2)));
	}

	SECTION("Inexact::IsNeq()")
	{
		REQUIRE(Inexact::IsNe(0.29, (0.10 + 0.20)));
	}

	SECTION("!Inexact::IsNeq()")
	{
		REQUIRE(!Inexact::IsNe(0.30, (0.10 + 0.20)));
	}
}

