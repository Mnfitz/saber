#ifndef SABER_EXCEPTION_HPP
#define SABER_EXCEPTION_HPP

// saber
#include "saber/config.hpp"

// std
#include <stdexcept>
#include <string>
#include <string_view>

namespace saber {

///@brief Common base class for all `saber` exception types in this header.
///@details Derives from `std::runtime_error`. Catch `saber::Exception` to handle
///both `saber::RequireException` and `saber::EnsureException`; it is also
///catchable as `std::runtime_error` or `std::exception`.
struct Exception : public std::runtime_error
{
	///@brief Constructs the exception with a diagnostic message.
	///@details Copies the message into the base `std::runtime_error`.
	///@param inMsg Diagnostic message.
	Exception(std::string_view inMsg) :
		std::runtime_error{std::string{inMsg}}
	{
		// This space intentionally blank
	}
}; // struct Exception

///@brief Exception thrown when a required condition is false.
///@details Derives from `saber::Exception` (and therefore `std::runtime_error`)
///and is thrown by `SABER_REQUIRE`. Use it for failed preconditions or other
///conditions that indicate a caller or program-logic error. It can be caught as
///`saber::RequireException`, `saber::Exception`, `std::runtime_error`, or
///`std::exception`.
struct RequireException : public Exception
{
	///@brief Constructs the exception with a diagnostic message.
	///@details Copies the message into the `saber::Exception` base.
	///@param inMsg Diagnostic message describing the failed requirement.
	RequireException(std::string_view inMsg) :
		Exception{inMsg}
	{
		// This space intentionally blank
	}
}; // struct RequireException

///@brief Exception thrown when an ensured condition is false.
///@details Derives from `saber::Exception` (and therefore `std::runtime_error`)
///and is thrown by `SABER_ENSURE`. Use it for failed runtime checks or violated
///invariants. It can be caught as `saber::EnsureException`, `saber::Exception`,
///`std::runtime_error`, or `std::exception`.
struct EnsureException : public Exception
{
	///@brief Constructs the exception with a diagnostic message.
	///@details Copies the message into the `saber::Exception` base.
	///@param inMsg Diagnostic message describing the failed condition.
	EnsureException(std::string_view inMsg) :
		Exception{inMsg}
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

///@brief Checks a condition and throws a `saber` exception if it is false.
///@details Both public macros accept a boolean expression and an optional
///message expression. The condition is evaluated once. If true, the macro does
///nothing. If false, `SABER_REQUIRE` throws `saber::RequireException` and
///`SABER_ENSURE` throws `saber::EnsureException`; both derive from
///`saber::Exception`, which derives from `std::runtime_error`.
///
///With one argument, the diagnostic contains the macro name and the stringified
///condition. With two arguments, the diagnostic is the macro-specific prefix
///followed by the evaluated message converted/concatenated as a `std::string`.
///A string literal such as `"file must be open"` is accepted and appears without
///quote characters in the resulting diagnostic. The message expression is
///evaluated only when the condition is false and must be compatible with
///`std::string` concatenation. Parenthesize conditions containing commas so the
///preprocessor treats them as one macro argument.
///
///Use `SABER_REQUIRE` to check a requirement or precondition; use `SABER_ENSURE`
///to check a runtime condition or invariant. Catch the specific Saber exception
///or the common `saber::Exception`/`std::runtime_error`/`std::exception` base.
///
///@par Usage examples
///@code{.cpp}
///SABER_REQUIRE(file != nullptr);
///SABER_REQUIRE(file != nullptr, "file must be open");
///SABER_ENSURE(count > 0, "count must be positive");
///@endcode
///
///@def SABER_REQUIRE(...)
///@brief Checks a requirement; throws `saber::RequireException` on failure.
///@param ... A boolean condition, optionally followed by a message expression.
///
///@def SABER_ENSURE(...)
///@brief Checks an invariant or runtime condition; throws `saber::EnsureException` on failure.
///@param ... A boolean condition, optionally followed by a message expression.

#define SABER_PRIVATE_REQUIRE_1(expr) \
	((expr) ? (void)0 : throw saber::RequireException("SABER_REQUIRE failed: " #expr))

#define SABER_PRIVATE_REQUIRE_2(expr, msg) \
	((expr) ? (void)0 : throw saber::RequireException(std::string{"SABER_REQUIRE failed: "} + (msg)))

#define SABER_PRIVATE_REQUIRE_SELECT(_1, _2, name, ...) name

#define SABER_PRIVATE_ENSURE_1(expr) \
	((expr) ? (void)0 : throw saber::EnsureException("SABER_ENSURE failed: " #expr))

#define SABER_PRIVATE_ENSURE_2(expr, msg) \
	((expr) ? (void)0 : throw saber::EnsureException(std::string{"SABER_ENSURE failed: "} + (msg)))

#define SABER_PRIVATE_ENSURE_SELECT(_1, _2, name, ...) name

#define SABER_REQUIRE(...) \
	SABER_PRIVATE_REQUIRE_SELECT(__VA_ARGS__, \
		SABER_PRIVATE_REQUIRE_2, SABER_PRIVATE_REQUIRE_1)(__VA_ARGS__)

#define SABER_ENSURE(...) \
	SABER_PRIVATE_ENSURE_SELECT(__VA_ARGS__, \
		SABER_PRIVATE_ENSURE_2, SABER_PRIVATE_ENSURE_1)(__VA_ARGS__)

#if SABER_COMPILER(MSVC)
#pragma warning(pop)
#endif

#endif // SABER_EXCEPTION_HPP