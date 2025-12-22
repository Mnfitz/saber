#ifndef SABER_GEOMETRY_POINT_HPP
#define SABER_GEOMETRY_POINT_HPP
#pragma once

// saber
#include "saber/geometry/config.hpp"
#include "saber/geometry/operators.hpp"
#include "saber/geometry/detail/impl2.hpp"

// std
#include <utility>

namespace saber::geometry {

// Forward Declaration
template<typename T, ImplKind Impl>
class Rectangle;

/// @brief 
/// @tparam T 
/// @tparam ImplType 
template<typename T, ImplKind Impl = ImplKind::kDefault>
class Point 
{
public:
    using ValueType = T;

public: 
    /// @brief Constructs a 2D point with the given coordinates.
    /// @param inX X coordinate
    /// @param inY Y coordinate
    constexpr Point(T inX, T inY);

    constexpr Point() = default;
    ~Point() = default;

    // RO5 is all default implemented
    constexpr Point(Point&& ioMove) noexcept = default;
    constexpr Point& operator=(Point&& ioMove) noexcept = default;

    constexpr Point(const Point& inCopy) = default;
    constexpr Point& operator=(const Point& inCopy) = default;

    // Getters
    /// @brief Get the X coordinate
    /// @return X value
    constexpr T X() const;
    /// @brief Get the Y coordinate
    /// @return Y value
    constexpr T Y() const;

    // Setters
    /// @brief Set the X coordinate
    /// @param inX Value to set
    constexpr void X(T inX);
	/// @brief Set the Y coordinate
	/// @param inY Value to set
	constexpr void Y(T inY);

    // Mathematical operations
    /// @brief Component-wise add another point to this point
    /// @param inPoint Point to add
    /// @return Reference to this point
    constexpr Point& operator+=(const Point& inPoint);
    /// @brief Component-wise subtract another point from this point
    /// @param inPoint Point to subtract
    /// @return Reference to this point
    constexpr Point& operator-=(const Point& inPoint);
    /// @brief Component-wise multiply by another point
    /// @param inPoint Point to multiply by
    /// @return Reference to this point
    constexpr Point& operator*=(const Point& inPoint);
    /// @brief Component-wise divide by another point
    /// @param inPoint Point to divide by
    /// @return Reference to this point
    constexpr Point& operator/=(const Point& inPoint);

    // --- Rounding ---

    // TRICKY mnfitz 22feb2025: SFINAE-enable rounding methods only for floating point types.
	// C++ Black magic: SFINAE (template substitution failure) will disallow
	// template `T` types that do not satisfy `enable_if` condition. This prevents rounding
	// methods from "being enabled" for non floating point types, like: `Point<int>`

    /// @brief Round this `Point<>` to nearest integer value. Halfway cases round away from zero. Compatible with `std::round()`
	/// @tparam U: Underlying `Point<T>` type (U: cuz T already in-use by Point<T>)
	/// @tparam SFINAE: Enable `RoundNearest()` only for floating point types
	/// @return Ref& to this `Point<>`
	template<typename U=T, typename SFINAE = std::enable_if_t<std::is_floating_point_v<U>>>
	constexpr Point& RoundNearest();

	/// @brief Round this `Point<>` toward -infinity to nearest integer value. Compatible with `std::floor()`
	/// @tparam U: Underlying `Point<T>` type (U: cuz T already in-use by Point<T>)
	/// @tparam SFINAE: Enable `RoundFloor()` only for floating point types
	/// @return Ref& to this `Point<>`
	template<typename U=T, typename SFINAE = std::enable_if_t<std::is_floating_point_v<U>>>
	constexpr Point& RoundFloor();

	/// @brief Round this `Point<>` toward +infinity to nearest integer value. Compatible with `std::ceil()`
	/// @tparam U: Underlying `Point<T>` type (U: cuz T already in-use by Point<T>)
	/// @tparam SFINAE: Enable `RoundCeil()` only for floating point types
	/// @return Ref& to this `Point<>`
	template<typename U=T, typename SFINAE = std::enable_if_t<std::is_floating_point_v<U>>>
	constexpr Point& RoundCeil();

	/// @brief Round this `Point<>` toward zero to nearest integer value. Compatible with `std::trunc()`
	/// @tparam U: Underlying `Point<T>` type (U: cuz T already in-use by Point<T>)
	/// @tparam SFINAE: Enable `RoundTrunc()` only for floating point types
	/// @return Ref& to this `Point<>`
	template<typename U=T, typename SFINAE = std::enable_if_t<std::is_floating_point_v<U>>>
	constexpr Point& RoundTrunc();

    /// @brief Translate this point by another point (component-wise)
    /// @param inTranslate Point to translate by
    /// @return Reference to this point
    constexpr Point& Translate(const Point& inTranslate);
	/// @brief Translate this point by explicit x and y offsets
	/// @param inX X offset
	/// @param inY Y offset
	/// @return Reference to this point
	constexpr Point& Translate(T inX, T inY);
	/// @brief Translate this point by the same offset in both axes
	/// @param inXY Offset for both X and Y
	/// @return Reference to this point
	constexpr Point& Translate(T inXY);

	/// @brief Scale this point (component-wise) by another point
	/// @param inScale Scale factors
	/// @return Reference to this point
	constexpr Point& Scale(const Point& inScale);
	/// @brief Scale this point by explicit x and y factors
	/// @param inX X scale factor
	/// @param inY Y scale factor
	/// @return Reference to this point
	constexpr Point& Scale(T inX, T inY);
	/// @brief Scale this point uniformly by the same factor
	/// @param inXY Scale factor for both axes
	/// @return Reference to this point
	constexpr Point& Scale(T inXY);

private:
	// Private APIs
	constexpr bool IsEqual(const Point& inPoint) const;

// Friend functions
private:
    friend constexpr bool operator==<Point>(const Point& inLHS, const Point& inRHS);
    friend constexpr bool operator!=<Point>(const Point& inLHS, const Point& inRHS);
    friend class Rectangle<T, Impl>;

private:
    using ImplType = typename detail::Impl2Traits<T, Impl>::ImplType; // VOODOO: Nested template type requires `typename` prefix
    ImplType mImpl{};
}; // class Point<>

// ------------------------------------------------------------------
#pragma region Inline Class Functions

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl>::Point(T inX, T inY) :
    mImpl{inX, inY}
{
    // Do nothing
}

// Getters
template<typename T, ImplKind Impl>
inline constexpr T Point<T, Impl>::X() const
{
    return mImpl.Get<0>();
}

template<typename T, ImplKind Impl>
inline constexpr T Point<T, Impl>::Y() const
{
    return mImpl.Get<1>();
}

// Setters
template<typename T, ImplKind Impl>
inline constexpr void Point<T, Impl>::X(T inX)
{
	mImpl.Set<0>(inX);
}

template<typename T, ImplKind Impl>
inline constexpr void Point<T, Impl>::Y(T inY)
{
	mImpl.Set<1>(inY);
}

#pragma endregion

// ------------------------------------------------------------------
#pragma region Inline Mathematical operations

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl>& Point<T, Impl>::operator+=(const Point& inPoint)
{
    mImpl += inPoint.mImpl;
    return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl>& Point<T, Impl>::operator-=(const Point& inPoint)
{
    mImpl -= inPoint.mImpl;
    return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl>& Point<T, Impl>::operator*=(const Point& inPoint)
{
    mImpl *= inPoint.mImpl;
    return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl>& Point<T, Impl>::operator/=(const Point& inPoint)
{
    mImpl /= inPoint.mImpl;
    return *this;
}

template<typename T, ImplKind Impl>
inline constexpr bool Point<T, Impl>::IsEqual(const Point& inPoint) const
{
    auto result = mImpl.IsEqual(inPoint.mImpl);
    return result;
}

template<typename T, ImplKind Impl>
template<typename U, typename SFINAE>
inline constexpr Point<T, Impl>& Point<T, Impl>::RoundNearest()
{
	mImpl.RoundNearest();
	return *this;
}

template<typename T, ImplKind Impl>
template<typename U, typename SFINAE>
inline constexpr Point<T, Impl>& Point<T, Impl>::RoundFloor()
{
	mImpl.RoundFloor();
	return *this;
}

template<typename T, ImplKind Impl>
template<typename U, typename SFINAE>
inline constexpr Point<T, Impl>& Point<T, Impl>::RoundCeil()
{
	mImpl.RoundCeil();
	return *this;
}

template<typename T, ImplKind Impl>
template<typename U, typename SFINAE>
inline constexpr Point<T, Impl>& Point<T, Impl>::RoundTrunc()
{
	mImpl.RoundTrunc();
	return *this;
}

// Mutators
template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl>& Point<T, Impl>::Translate(const Point<T, Impl>& inTranslate)
{
	*this += inTranslate;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl>& Point<T, Impl>::Translate(T inX, T inY)
{
	const Point<T, Impl> translate{inX, inY};
	return Translate(translate);
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl>& Point<T, Impl>::Translate(T inXY)
{
	return Translate(inXY, inXY);
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl>& Point<T, Impl>::Scale(const Point<T, Impl>& inScale)
{
	*this *= inScale;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl>& Point<T, Impl>::Scale(T inX, T inY)
{
	const Point<T, Impl> scale{inX, inY};
	return Scale(scale);
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl>& Point<T, Impl>::Scale(T inXY)
{
	return Scale(inXY, inXY);
}

#pragma endregion

// ------------------------------------------------------------------
#pragma region Free Functions

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl> Translate(const Point<T, Impl>& inPoint, const Point<T, Impl>& inTranslate)
{
    Point<T, Impl> result{inPoint};
	result.Translate(inTranslate);
	return result;
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl> Translate(const Point<T, Impl>& inPoint, T inX, T inY)
{
    const Point<T, Impl> translate{inX, inY};
    return Translate(inPoint, translate);
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl> Translate(const Point<T, Impl>& inPoint, T inXY)
{
    return Translate(inPoint, inXY, inXY);
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl> Scale(const Point<T, Impl>& inPoint, const Point<T, Impl>& inScale)
{
    Point<T, Impl> result{inPoint};
	result.Scale(inScale);
	return result;
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl> Scale(const Point<T, Impl>& inPoint, T inX, T inY)
{
    const Point<T, Impl> scale{inX, inY};
    return Scale(inPoint, scale);
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl> Scale(const Point<T, Impl>& inPoint, T inXY)
{
    return Scale(inPoint, inXY, inXY);
}

/// @brief Round to nearest even integer value. Halfway cases round away from zero.
/// @tparam T: Underlying `Point<>` type
/// @tparam ImplType: Optional underlying implementation type
/// @param inPoint: `Point<>` object to be rounded
/// @return Rounded `Point<>` result
template<typename T, ImplKind Impl, typename SFINAE = std::enable_if_t<std::is_floating_point_v<T>>>
inline constexpr Point<T, Impl> RoundNearest(const Point<T, Impl>& inPoint)
{
	constexpr bool kIsFloatingPoint = std::is_floating_point_v<T>;
	static_assert(kIsFloatingPoint, "RoundNearest() only supports floating point types");

	auto result{inPoint};
	return result.RoundNearest(); // RVO should apply here
}

/// @brief Round towards zero to nearest integer value.
/// @tparam T: Underlying `Point<>` type
/// @tparam ImplType: Optional underlying implementation type
/// @param inPoint: `Point<>` object to be rounded
/// @return Rounded `Point<>` result
template<typename T, ImplKind Impl, typename SFINAE = std::enable_if_t<std::is_floating_point_v<T>>>
inline constexpr Point<T, Impl> RoundTrunc(const Point<T, Impl>& inPoint)
{
	constexpr bool kIsFloatingPoint = std::is_floating_point_v<T>;
	static_assert(kIsFloatingPoint, "RoundTrunc() only supports floating point types");

	auto result{inPoint};
	return result.RoundTrunc(); // RVO should apply here
}

/// @brief Round towards +infinity to the nearest integer value.
/// @tparam T: Underlying `Point<>` type
/// @tparam ImplType: Optional underlying implementation type
/// @param inPoint: `Point<>` object to be rounded
/// @return Rounded `Point<>` result
template<typename T, ImplKind Impl, typename SFINAE = std::enable_if_t<std::is_floating_point_v<T>>>
inline constexpr Point<T, Impl> RoundCeil(const Point<T, Impl>& inPoint)
{
	constexpr bool kIsFloatingPoint = std::is_floating_point_v<T>;
	static_assert(kIsFloatingPoint, "RoundCeil() only supports floating point types");

	auto result{inPoint};
	return result.RoundCeil(); // RVO should apply here
}

/// @brief Round towards -infinity to the nearest integer value.
/// @tparam T: Underlying `Point<>` type
/// @tparam ImplType: Optional underlying implementation type
/// @param inPoint: `Point<>` object to be rounded
/// @return Rounded `Point<>` result
template<typename T, ImplKind Impl, typename SFINAE = std::enable_if_t<std::is_floating_point_v<T>>>
inline constexpr Point<T, Impl> RoundFloor(const Point<T, Impl>& inPoint)
{
	constexpr bool kIsFloatingPoint = std::is_floating_point_v<T>;
	static_assert(kIsFloatingPoint, "RoundFloor() only supports floating point types");

	auto result{inPoint};
	return result.RoundFloor(); // RVO should apply here
}

#pragma endregion

// ------------------------------------------------------------------
#pragma region Structured Bindings

// TRICKY mnfitz 14oct2024: Turn on structured binging support for C++17 or later
#ifdef __cpp_structured_bindings

// Structured Binding Support 

// Prefer free function over class method
template<std::size_t Index, typename T>
inline T get(const Point<T>& inPoint)
{
    static_assert(2 == std::tuple_size_v<Point<T>>); // Use own std::tuple_size<> specialization
    static_assert(Index < 2, "Unexpected Index for Point<T, Impl>");

    T result{};
    if constexpr (Index == 0)
    {
        result = inPoint.X();
    }
    else if constexpr (Index == 1)
    {
        result = inPoint.Y();
    }

    return result;
}

template<typename T>
struct std::tuple_size<Point<T>> // Partial template specialization for: Point<T, Impl>
{
    // Number of elements in Point<T, Impl>'s structured binding
    static constexpr std::size_t value = 2;
};

template<std::size_t Index, typename T>
struct std::tuple_element<Index, Point<T>> // Partial template specialization for: Point<T, Impl>
{
    // Type of elements in Point<T, Impl`>'s structured
    using type = T;
};

#endif //__cpp_structured_bindings
#pragma endregion

}// namespace saber::geometry

#endif // SABER_GEOMETRY_POINT_HPP