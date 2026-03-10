#ifndef SABER_EXCEPTION_HPP
#define SABER_EXCEPTION_HPP

// saber
#include "saber/config.hpp"
#include "saber/conditionals.hpp"

// std
#include <iostream>
#include <stdexcept>
#include <string>

namespace saber {

// Custom exception type used by the macros below.  All runtime validation
// failures in the library throw this type so callers can catch a single
// exception class without having to worry about std::invalid_argument vs
// std::runtime_error.  The message passed to the constructor is stored in
// std::runtime_error and will be returned by what().
struct Exception : public std::runtime_error
{
    Exception(const std::string& inMsg) :
		std::runtime_error{inMsg}
    {
		// Nothing to do
	}
};

} // namespace saber

// Helpers to avoid warnings about constant conditional expressions when
// the test expression is a compile-time constant.  MSVC issues C4127 in
// that situation; we temporarily disable it around the macro definitions.
#if SABER_COMPILER(MSVC)
#pragma warning(push)
#pragma warning(disable:4127)
#endif

// Evaluate to an expression so the macros can be used inside other
// macros such as Catch2's REQUIRE_NOTHROW () which expect an expression
// rather than a compound statement.  The conditional operator works well
// here since the "false" arm throws and the whole expression has type
// void.
#define SABER_ENSURE(expr)       ((expr) ? (void)0 : throw saber::Exception(std::string("SABER_ENSURE failed: ") + #expr))
#define SABER_REQUIRE(expr)      ((expr) ? (void)0 : throw saber::Exception(std::string("SABER_REQUIRE failed: ") + #expr))

#if SABER_COMPILER(MSVC)
#pragma warning(pop)
#endif

#endif // SABER_EXCEPTION_HPP