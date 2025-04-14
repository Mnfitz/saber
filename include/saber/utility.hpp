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

#pragma once
#ifndef SABER_UTILITY_HPP
#define SABER_UTILITY_HPP

// saber
#include "saber/config.hpp"

namespace saber {

// ------------------------------------------------------------------
#pragma region struct ConvertTraits<>/ConvertTo<>

/// @name TypeConversion
/// Convert from one type to another with a standardized API
///
/// `struct ConvertTraits<>`: This is the "private implementation" portion of
/// `saber` type conversion. It defines the manner in which a type is converted
/// but is never used/invoked directly by a client of type conversion.
/// Each/every conversion must provide a "full specialization" of this
/// `ConvertTraits<>` functor template class. Like this:
/// @code
/// namespace saber {
/// // Convert from: std::array<float,2> to: std:tuple<int, int>
/// template<> // Fully specialized...
/// struct ConvertTraits<std::tuple<int, int>, std::array<float, 2>>
/// {
/// 	std::tuple<int, int> operator()(const std::array<float, 2>& inValue) const
/// 	{
/// 		auto tuple = std::make_tuple(
/// 			static_cast<int>(inValue.at(0)),
/// 			static_cast<int>(inValue.at(1)));
/// 		return tuple;
/// 	}
/// };
/// } // namespace saber
/// @endcode
///
/// `ConvertTo()`: This is the "public interface" portion of `saber` type
/// conversion. Clients call this function when performing type conversion.
/// When called, it invokes the appropriate `ConvertTraits<>` functor implementation.
/// Use it like this:
/// @code
/// #include "saber/utility.hpp"
/// int main()
/// {
///		const std::array<float, 2> array{3, 4};
///
///		// 2-argument template function...
///		// technically, you call it like this:
///		const auto tuple1 = saber::ConvertTo<std::tuple<int, int>, std::array<float, 2>>(array);
///
///		// but c++ is smart, and knows how to deduce the FromType
///		// because of the typeof(provided input parameter)...
///		// so this is the preferred way to call it:
///		const auto tuple2 = saber::ConvertTo<std::tuple<int, int>>(array);
/// }
/// @endcode
/// @{

/// @brief "Private" template functor for conversion implementations
///
/// Must be fully specialized for every conversion
/// @tparam ToType: Output type of conversion
/// @tparam FromType: Input type of conversion
template<typename ToType, typename FromType>
struct ConvertTraits
{
	/// @brief Return a `ToType` value as converted from `FromType`
	/// @param inValue: Input value to be converted
	/// @return Output converted value
	ToType operator()(const FromType& inValue) const
	{
		// TRICKY j3fitz 08apr2025: static_assert during missing specializations.
		// Note that static_assert must have dependency on input type so it will
		// selectively fire only when instanciated (during missing specialization).
		constexpr bool kIsImpossible = (sizeof(inValue) == -1);
		static_assert(kIsImpossible, "Missing ConvertTraits<> specialization for: <ToType,FromType>");
	}
};

/// @brief Convert an input value from one type to another
/// @tparam ToType: Output type of conversion
/// @tparam FromType: Input type of conversion
/// @param inValue: Input value to be converted
/// @return Output converted value
template<typename ToType, typename FromType>
inline ToType ConvertTo(const FromType& inValue)
{
	constexpr auto convert = ConvertTraits<ToType, FromType>{}; // "handoff" functor
	return convert(inValue); // operator()
}

/// @}

#pragma endregion {}

// ------------------------------------------------------------------
#pragma region class TaggedType<>

/// @name StrongTyping
///
/// Provides a strongly-typed interface for an underlying type.
/// Intended for use with intrinsic and/or generic types to provide
/// better type-checking across interfaces. Use this to enforce
/// type-checking for imperial vs. metric units, person names vs.
/// place names strings, etc. E.g.:
/// @code
/// // --- Bad: "int" ---
/// void PrintDays(int inDays)
/// {
/// 	std::cout << "This many days: " << inDays;
/// }
///
/// main()
/// {
/// 	struct Point { int x, y; };
/// 	const int kWeek = 7;
/// 	const Point kPoint = {-1, 0};
///
/// 	PrintDays(kWeek); // ok
/// 	PrintDays(kPoint.x); // what the?!?! this compiles! "int" as type: "not strong enuf"!
/// }
///
///
/// // --- Good: "Days" ---
/// // TRICKY: "struct DaysTag"...
/// // A "forward decl" we never define! It just needs to be a unique typename
/// using Days = TaggedType<int, struct DaysTag>; // has-a: "int"
///
/// void PrintDays(Days inDays)
/// {
/// 	std::cout << "This many days: " << inDays.Value();
/// }
///
/// main()
/// {
/// 	struct Point { int x, y; };
/// 	const Days kWeek{7};
/// 	const Point kPoint = {-1, 0};
///
/// 	PrintDays(kWeek); // ok
/// 	PrintDays(kPoint.x); // compile error! hooray! "Days" as type: "strongly typed"!
/// }
/// @endcode
/// @{

/// @brief Wrapper class providing a strongly-typed interface for an underlying type.
///
/// Use with intrinsic/generic types to provide better type-checking across interfaces.
/// @tparam T: Underlying intrinsic/generic type
/// @tparam Tag: Unique-ifying "tag type" (typically a dummy forward-declared class/struct)
template<typename T, typename Tag>
class TaggedType final // Don't allow subclassing
{
public:
	using ValueType = T;

public:
	constexpr TaggedType() noexcept = default;

	/*explicit*/ constexpr TaggedType(const T& inValue) noexcept : // "not explicit" so we can use: {aggregate initialization};
		mValue{inValue}
	{
		// This space intentionally blank
	}

	/// @brief Return the boolean sense of this `TaggedType<>`
	constexpr explicit operator bool() const noexcept
	{
		return mValue.operator bool();
	}

	/// @brief Get the underlying value of this `TaggedType<>`
	/// @return Ref& to underlying value
	constexpr T& Value() noexcept
	{
		return mValue;
	}

	/// @brief Get the underlying const value of this `TaggedType<>`
	/// @return Const ref& to underlying value
	constexpr const T& Value() const noexcept
	{
		return mValue;
	}

	/// @brief Set the underlying const value of this `TaggedType<>`
	/// @param inValue: New value
	void Value(const T& inValue) noexcept
	{
		mValue = inValue;
	}

	// --- friend functions

	/// @brief Compare two `TaggedType<>` instances for equality
	/// @param inLhs: Lefthand side operand
	/// @param inRhs: Righthand side operand
	/// @return true, if operands are equal; otherwise false
	friend bool operator==(const TaggedType& inLhs, const TaggedType& inRhs)
	{
		return inLhs.Value() == inRhs.Value();
	}

	/// @brief Compare two `TaggedType<>` instances for inequality
	/// @param inLhs: Lefthand side operand
	/// @param inRhs: Righthand side operand
	/// @return true, if operands are unequal; otherwise false
	friend bool operator!=(const TaggedType& inLhs, const TaggedType& inRhs)
	{
		return inLhs.Value() != inRhs.Value();
	}

private:
	T mValue{}; // has-a: T
}; // class TaggedType<>

/// @}

#pragma endregion {}

} // namepsace saber

#endif // SABER_UTILITY_HPP