#ifndef SABER_EXCEPTION_HPP
#define SABER_EXCEPTION_HPP

// saber
#include "saber/config.hpp"

// std
#include <stdexcept>
#include <string>
#include <string_view>

namespace saber {

///@brief Common standard exception base type for `saber` exceptions.
///@details Alias for `std::exception`, allowing callers to catch `saber`
///exception types through a shared standard exception base.
using Exception = std::exception;

///@brief Exception thrown when a required condition is not satisfied.
///@details Derives from `std::logic_error` and is used by `SABER_REQUIRE`.
struct RequireException : public std::logic_error
{
	///@brief Constructs the exception with a diagnostic message.
	///@param inMsg Message describing the failed requirement.
	RequireException(std::string_view inMsg) :
		std::logic_error{std::string{inMsg}}
	{
		// This space intentionally blank
	}
}; // struct RequireException

///@brief Exception thrown when an ensured condition is not satisfied.
///@details Derives from `std::runtime_error` and is used by `SABER_ENSURE`.
struct EnsureException : public std::runtime_error
{
	///@brief Constructs the exception with a diagnostic message.
	///@param inMsg Message describing the failed ensure condition.
	EnsureException(std::string_view inMsg) :
		std::runtime_error{std::string{inMsg}}
	{
		// This space intentionally blank
	}
}; // struct EnsureException

} // namespace saber

// Helpers to avoid warnings about constant conditional expressions when
// the test expression is a compile-time constant.  MSVC issues C4127 in
// that situation; we temporarily disable it around the macro definitions.
#if SABER_COMPILER(MSVC)
#pragma warning(push)
#pragma warning(disable:4127)
#endif

// Super-architected macros for SABER_REQUIRE and SABER_ENSURE...
// that can be called with either:
//
//   * one -OR- two arguments!
//
// The first argument is the expression to test, and the optional second
// argument is a custom message to include in the exception if the test fails.
// If no custom message is provided, a default message is generated that
// includes the text of the expression.
//
// Example usage:
//   SABER_REQUIRE(x > 0); // default message: "SABER_REQUIRE failed: x > 0"
//   SABER_ENSURE(x > 0, "x must be positive"); // custom message: "SABER_ENSURE failed: x must be positive"

#define SABER_PRIVATE_REQUIRE_1(expr) \
	((expr) ? (void)0 : throw saber::RequireException("SABER_REQUIRE failed: " #expr))

#define SABER_PRIVATE_REQUIRE_2(expr, msg) \
	((expr) ? (void)0 : throw saber::RequireException("SABER_REQUIRE failed: " #msg))

#define SABER_PRIVATE_REQUIRE_SELECT(_1, _2, name, ...) name

#define SABER_PRIVATE_ENSURE_1(expr) \
	((expr) ? (void)0 : throw saber::EnsureException("SABER_ENSURE failed: " #expr))

#define SABER_PRIVATE_ENSURE_2(expr, msg) \
	((expr) ? (void)0 : throw saber::EnsureException("SABER_ENSURE failed: " #msg))

#define SABER_PRIVATE_ENSURE_SELECT(_1, _2, name, ...) name

///@def SABER_REQUIRE(...)
///@brief Checks that a required condition is satisfied.
///@details Accepts a boolean expression and an optional message. If the
///condition is false, throws `saber::RequireException`. With one argument,
///the expression text is used in the diagnostic; with two arguments, the
///message text is used.
///@param ... A boolean expression, optionally followed by a message.
#define SABER_REQUIRE(...) \
	SABER_PRIVATE_REQUIRE_SELECT(__VA_ARGS__, \
		SABER_PRIVATE_REQUIRE_2, SABER_PRIVATE_REQUIRE_1)(__VA_ARGS__)

///@def SABER_ENSURE(...)
///@brief Checks that an ensured condition is satisfied.
///@details Accepts a boolean expression and an optional message. If the
///condition is false, throws `saber::EnsureException`. With one argument,
///the expression text is used in the diagnostic; with two arguments, the
///message text is used.
///@param ... A boolean expression, optionally followed by a message.
#define SABER_ENSURE(...) \
	SABER_PRIVATE_ENSURE_SELECT(__VA_ARGS__, \
		SABER_PRIVATE_ENSURE_2, SABER_PRIVATE_ENSURE_1)(__VA_ARGS__)

#if SABER_COMPILER(MSVC)
#pragma warning(pop)
#endif

#endif // SABER_EXCEPTION_HPP