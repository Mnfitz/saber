#ifndef SABER_GEOMETRY_OPERATORS_HPP
#define SABER_GEOMETRY_OPERATORS_HPP

// std
#include <type_traits>

namespace saber::geometry {
namespace detail {

// TRICKY j3fitz 08apr2026: Generic set of operators +, -, *, /, for geometry types...
// that also avoids the dreaded "error C2593: 'operator +' is ambiguous" compile error.
//
// The problem here is a "template" free function operator accepting a generic type "T".
// Using "T" allows us (good) to have single template operator impls for all our geometry
// types: (Point/Size/Rectangle/Matrix), rather than copy/paste for each type.
// However, "other types" can get sucked-into/match generic type "T", (bad) which don't
// make sense for our operators (like: int, or std::string). This confuses the compiler
// and results in dreaded compile error: "error C2593: 'operator +' is ambiguous"...
// as the compiler doesn't know whether "+" between two ints means: use regular addition,
// or use our custom "+" operator. Yuck!
//
// The solution is twofold:
// 1. define the operators in "same namespace" as the target types:
//    so ADL (Arg Dependent Lookup) will find/match an operator only when arguments
//    passed to operator are also in same namespace
// 2. use SFINAE to prevent unrelated types from being considered as candidates
//    for our operators
//
// We do this with a custom `is_geometry_compatible` trait, specialized for our
// geometry types, that is used to selectively SFINAE-enable our operators

template<typename T>
struct is_geometry_compatible : std::false_type {}; // by default: `false`

template<typename T>
constexpr bool is_geometry_compatible_v = is_geometry_compatible<T>::value;

} // namespace detail

// Forward declare the geometry types we want to use for our operators
template<typename T, ImplKind Impl>
class Point;

template<typename T, ImplKind Impl>
class Size;

template<typename T, ImplKind Impl>
class Rectangle;

template<typename T, ImplKind Impl>
class Matrix;

// Opt-in the geometry types for operator support by specializing `is_geometry_compatible` trait
template<typename T>
struct detail::is_geometry_compatible<Point<T, ImplKind::kScalar>> : std::true_type {};
template<typename T>
struct detail::is_geometry_compatible<Point<T, ImplKind::kSimd>> : std::true_type {};

template<typename T>
struct detail::is_geometry_compatible<Size<T, ImplKind::kScalar>> : std::true_type {};
template<typename T>
struct detail::is_geometry_compatible<Size<T, ImplKind::kSimd>> : std::true_type {};

template<typename T>
struct detail::is_geometry_compatible<Rectangle<T, ImplKind::kScalar>> : std::true_type {};
template<typename T>
struct detail::is_geometry_compatible<Rectangle<T, ImplKind::kSimd>> : std::true_type {};

template<typename T>
struct detail::is_geometry_compatible<Matrix<T, ImplKind::kScalar>> : std::true_type {};
template<typename T>
struct detail::is_geometry_compatible<Matrix<T, ImplKind::kSimd>> : std::true_type {};

/// @brief Binary Operator that adds 2 input types returning a result of the same type
/// Use it like this:
/// ```
/// auto result = type1 + type2;
/// ```
/// @tparam T underlying type
/// @tparam SFINAE enable this operator+() only for saber::geometry types
/// @param inLHS: Left hand side argument
/// @param inRHS: Right hand side argument
/// @return Sum result
template<typename T, typename SFINAE = std::enable_if_t<detail::is_geometry_compatible_v<T>>>
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
/// @tparam SFINAE enable this operator-() only for saber::geometry types
/// @param inLHS: Left hand side argument
/// @param inRHS: Right hand side argument
/// @return Difference result
template<typename T, typename SFINAE = std::enable_if_t<detail::is_geometry_compatible_v<T>>>
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
/// @tparam SFINAE enable this operator*() only for saber::geometry types
/// @param inLHS: Left hand side argument
/// @param inRHS: Right hand side argument
/// @return Product result
template<typename T, typename SFINAE = std::enable_if_t<detail::is_geometry_compatible_v<T>>>
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
/// @tparam SFINAE enable this operator/() only for saber::geometry types
/// @param inLHS: Left hand side argument
/// @param inRHS: Right hand side argument
/// @return Quotient result
template<typename T, typename SFINAE = std::enable_if_t<detail::is_geometry_compatible_v<T>>>
inline constexpr T operator/(const T& inLHS, const T& inRHS)
{
    // Incomprehensible c++ incantation to detect if T implements operator/=()
    constexpr bool kHasOperatorDivAssign = std::is_same_v<T&, decltype(std::declval<T&>() /= std::declval<T>())>;
    static_assert(kHasOperatorDivAssign, "T does not support operator/");

    T result = inLHS;
    result /= inRHS;
    return result;
}

/// @brief Binary Operator that compares 2 input values for equality returning a bool result
/// Use it like this:
/// ```
/// auto result = value1 == values2;
/// ```
/// @tparam T underlying type
/// @tparam SFINAE enable this operator==() only for saber::geometry types
/// @param inLHS: Left hand side argument
/// @param inRHS: Right hand side argument
/// @return Quotient result
template<typename T, typename SFINAE = std::enable_if_t<detail::is_geometry_compatible_v<T>>>
inline constexpr bool operator==(const T& inLHS, const T& inRHS)
{
    // Incomprehensible c++ incantation to detect if T implements IsEqual()
    constexpr bool kHasOperatorEqual = std::is_same_v<bool, decltype(std::declval<T&>().IsEqual(std::declval<T>()))>;
    static_assert(kHasOperatorEqual, "T does not support operator==");

    const bool result = inLHS.IsEqual(inRHS);
    return result;
}

template<typename T, typename SFINAE = std::enable_if_t<detail::is_geometry_compatible_v<T>>>
inline constexpr bool operator!=(const T& inLHS, const T& inRHS)
{
    // Incomprehensible c++ incantation to detect if T implements IsEqual()
    constexpr bool kHasOperatorEqual = std::is_same_v<bool, decltype(std::declval<T&>().IsEqual(std::declval<T>()))>;
    static_assert(kHasOperatorEqual, "T does not support operator!=");

    const bool result = !inLHS.IsEqual(inRHS);
    return result;
}

// Unary Operators

template<typename T, typename SFINAE = std::enable_if_t<detail::is_geometry_compatible_v<T>>>
inline constexpr T operator-(const T& inLHS) // Unary negation (e.g: not subtraction)
{ 
    const T zero{};
    return zero - inLHS;
}

template<typename T, typename SFINAE = std::enable_if_t<detail::is_geometry_compatible_v<T>>>
inline constexpr T operator+(const T& inLHS) // Unary addition = noOp
{
    return inLHS; // copy
}

} // namespace saber::geometry

#endif //SABER_GEOMETRY_OPERATORS_HPP