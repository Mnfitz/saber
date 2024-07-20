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
inline T operator+(const T& inLHS, const T& inRHS)
{
    constexpr bool kHasOperatorPlusAssign = std::is_same_v<decltype(std::declval<T&>() += std::declval<T>()), T&>;
    static_assert(kHasOperatorPlusAssign, "T must provide operator+=()");

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
inline T operator-(const T& inLHS, const T& inRHS)
{
    constexpr bool kHasOperatorMinusAssign = std::is_same_v<decltype(std::declval<T&>() -= std::declval<T>()), T&>;
    static_assert(kHasOperatorMinusAssign, "T must provide operator-=()");

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
inline T operator*(const T& inLHS, const T& inRHS)
{
    constexpr bool kHasOperatorMultAssign = std::is_same_v<decltype(std::declval<T&>() *= std::declval<T>()), T&>;
    static_assert(kHasOperatorMultAssign, "T must provide operator*=()");

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
inline T operator/(const T& inLHS, const T& inRHS)
{
    constexpr bool kHasOperatorDivAssign = std::is_same_v<decltype(std::declval<T&>() /= std::declval<T>()), T&>;
    static_assert(kHasOperatorDivAssign, "T must provide operator/=()");

    T result = inLHS;
    result /= inRHS;
    return result;
}

} // namespace saber::geometry

#endif //SABER_GEOMETRY_OPERATORS_HPP