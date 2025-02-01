#ifndef SABER_GEOMETRY_OPERATORS_HPP
#define SABER_GEOMETRY_OPERATORS_HPP
#pragma once

// std
#include <type_traits>

namespace saber::geometry {

/// @brief Binary Operator that adds 2 input types returning a result of the same type
/// Use it like this: 
/// ```
/// auto result = type1 + type2;
/// ```
/// @tparam T underlying type
/// @param inLHS: Left hand side argument
/// @param inRHS: Right hand side argument 
/// @return Sum result
template<typename T>
inline constexpr T operator+(const T& inLHS, const T& inRHS)
{
    // Incomprehensible c++ incantation to detect if T implements operator+=()
    constexpr bool kHasOperatorPlusAssign = std::is_same_v<T&, decltype(std::declval<T&>() += std::declval<T>())>;
    static_assert(kHasOperatorPlusAssign, "T does not support operator+");

    T result = inLHS;
    result += inRHS;
    return result;
}

/// @brief Binary Operator that subtracts 2 input types returning a result of the same type
/// Use it like this: 
/// ```
/// auto result = type1 - type2;
/// ```
/// @tparam T underlying type
/// @param inLHS: Left hand side argument
/// @param inRHS: Right hand side argument 
/// @return Difference result
template<typename T>
inline constexpr T operator-(const T& inLHS, const T& inRHS)
{
    // Incomprehensible c++ incantation to detect if T implements operator-=()
    constexpr bool kHasOperatorMinusAssign = std::is_same_v<T&, decltype(std::declval<T&>() -= std::declval<T>())>;
    static_assert(kHasOperatorMinusAssign, "T does not support operator-");

    T result = inLHS;
    result -= inRHS;
    return result;
}

/// @brief Binary Operator that multiplies 2 input types returning a result of the same type
/// Use it like this: 
/// ```
/// auto result = type1 * type2;
/// ```
/// @tparam T underlying type
/// @param inLHS: Left hand side argument
/// @param inRHS: Right hand side argument 
/// @return Product result
template<typename T>
inline constexpr T operator*(const T& inLHS, const T& inRHS)
{
    // Incomprehensible c++ incantation to detect if T implements operator*=()
    constexpr bool kHasOperatorMultAssign = std::is_same_v<T&, decltype(std::declval<T&>() *= std::declval<T>())>; 
    static_assert(kHasOperatorMultAssign, "T does not support operator*");

    T result = inLHS;
    result *= inRHS;
    return result;
}

/// @brief Binary Operator that divides 2 input types returning a result of the same type
/// Use it like this: 
/// ```
/// auto result = type1 / type2;
/// ```
/// @tparam T underlying type
/// @param inLHS: Left hand side argument
/// @param inRHS: Right hand side argument 
/// @return Quotient result
template<typename T>
inline constexpr T operator/(const T& inLHS, const T& inRHS)
{
    // Incomprehensible c++ incantation to detect if T implements operator/=()
    constexpr bool kHasOperatorDivAssign = std::is_same_v<T&, decltype(std::declval<T&>() /= std::declval<T>())>;
    static_assert(kHasOperatorDivAssign, "T does not support operator/");

    T result = inLHS;
    result /= inRHS;
    return result;
}

template<typename T>
inline constexpr bool operator==(const T& inLHS, const T& inRHS)
{
    // Incomprehensible c++ incantation to detect if T implements IsEqual()
    constexpr bool kHasOperatorEqual = std::is_same_v<bool, decltype(std::declval<T&>().IsEqual(std::declval<T>()))>;
    static_assert(kHasOperatorEqual, "T does not support operator==");

    const bool result = inLHS.IsEqual(inRHS);
    return result;
}

template<typename T>
inline constexpr bool operator!=(const T& inLHS, const T& inRHS)
{
    // Incomprehensible c++ incantation to detect if T implements IsEqual()
    constexpr bool kHasOperatorEqual = std::is_same_v<bool, decltype(std::declval<T&>().IsEqual(std::declval<T>()))>;
    static_assert(kHasOperatorEqual, "T does not support operator!=");

    const bool result = !inLHS.IsEqual(inRHS);
    return result;
}

// Unary Operators

template<typename T>
inline constexpr T operator-(const T& inLHS) // Unary negation, not subtraction
{ 
    const T zero{};
    return zero - inLHS;
}

template<typename T>
inline constexpr T operator+(const T& inLHS) // Unary addition = noOp
{
    return inLHS; // copy
}

} // namespace saber::geometry

#endif //SABER_GEOMETRY_OPERATORS_HPP