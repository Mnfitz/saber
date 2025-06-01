#ifndef SABER_GEOMETRY_RECTANGLE_HPP
#define SABER_GEOMETRY_RECTANGLE_HPP
#pragma once

// saber
#include "saber/geometry/config.hpp"
#include "saber/geometry/operators.hpp"
#include "saber/geometry/point.hpp"
#include "saber/geometry/size.hpp"
#include "saber/geometry/detail/impl4.hpp"
#include "saber/utility.hpp"

// std
#include <utility>

namespace saber::geometry {

/// @brief 
/// @tparam T 
/// @tparam ImplType 
template<typename T, ImplKind Impl = ImplKind::kDefault>
class Rectangle 
{
public:
    using ValueType = T;

public: 
    constexpr Rectangle() = default;
	constexpr Rectangle(const Point<T, Impl>& inPoint, const geometry::Size<T, Impl>& inSize);
	constexpr Rectangle(T inX, T inY, T inWidth, T inHeight);
	constexpr Rectangle(const Point<T, Impl>& inPoint);
	constexpr Rectangle(const geometry::Size<T, Impl>& inSize);
    ~Rectangle() = default;

    // RO5 is all default implemented
    constexpr Rectangle(Rectangle&& ioMove) noexcept = default;
    constexpr Rectangle& operator=(Rectangle&& ioMove) noexcept = default;
    constexpr Rectangle(const Rectangle& inCopy) = default;
    constexpr Rectangle& operator=(const Rectangle& inCopy) = default;

	// Getters
    constexpr Point<T, Impl> Origin() const;
	constexpr geometry::Size<T, Impl> Size() const; 
	constexpr T X() const;
	constexpr T Y() const;
	constexpr T Width() const;
	constexpr T Height() const;

	// Setters
	constexpr void Origin(const Point<T, Impl>& inPoint);
	constexpr void Size(const geometry::Size<T, Impl>& inSize); 
	constexpr void X(T inX);
	constexpr void Y(T inY);
	constexpr void Width(T inWidth);
	constexpr void Height(T inHeight);

	// Mutators
	constexpr Rectangle& Translate(const Point<T, Impl>& inPoint); //+- Origin
	constexpr Rectangle& Translate(T inX, T inY);
	constexpr Rectangle& Translate(T inXY);

	constexpr Rectangle& Enlarge(const geometry::Size<T, Impl>& inSize); //+- Size
	constexpr Rectangle& Enlarge(T inX, T inY);
	constexpr Rectangle& Enlarge(T inXY);

	// REVISIT mnfitz 26may2025: This would prevent implicit conversion from an initializer list
	/*
	saber::Rectangle<float> rect;
	rect.Scale({2.0f, 3.0f}); // error: "ambiguous"
	rect.Translate({2.0f, 4.0f}); // OK
	*/
	constexpr Rectangle& Scale(const Point<T, Impl>& inPoint); // */ Origin
	constexpr Rectangle& Scale(const geometry::Size<T, Impl>& inSize); // */ Size
	constexpr Rectangle& Scale(T inX, T inY); // Origin + Size
	constexpr Rectangle& Scale(T inXY); // */ Origin + Size

#if 0
    
    // Mathematical Origin operations
    constexpr Rectangle& operator+=(const Point& inPoint);
    constexpr Rectangle& operator-=(const Point& inPoint);
    constexpr Rectangle& operator*=(const Point& inPoint);
    constexpr Rectangle& operator/=(const Point& inPoint);

	// Mathematical Size operations
    constexpr Rectangle& operator+=(const Size& inSize);
    constexpr Rectangle& operator-=(const Size& inSize);
    constexpr Rectangle& operator*=(const Size& inSize);
    constexpr Rectangle& operator/=(const Size& inSize);

    // --- Rounding ---

    // TRICKY mnfitz 22feb2025: SFINAE-enable rounding methods only for floating point types.
	// C++ Black magic: SFINAE (template substitution failure) will disallow
	// template `T` types that do not satisfy `enable_if` condition. This prevents rounding
	// methods from "being enabled" for non floating point types, like: `Size<int>`

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
#endif

private:
	// Private APIs
	constexpr bool IsEqual(const Rectangle& inRectangle) const;

// Friend functions
private:
    friend constexpr bool operator==<Rectangle>(const Rectangle& inLHS, const Rectangle& inRHS);
    friend constexpr bool operator!=<Rectangle>(const Rectangle& inLHS, const Rectangle& inRHS);

private:
	using ImplType = typename detail::Impl4Traits<T, Impl>::ImplType; // VOODOO: Nested template type requires `typename` prefix
    ImplType mImpl{};
}; // class Rectangle<>

// ------------------------------------------------------------------
#pragma region Inline Class Functions

// Ctors
template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>::Rectangle(T inX, T inY, T inWidth, T inHeight) :
    mImpl{inX, inY, inWidth, inHeight}
{
    // Do nothing
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>::Rectangle(const Point<T, Impl>& inPoint, const geometry::Size<T, Impl>& inSize) :
    mImpl{inPoint.mImpl, inSize.mImpl}
{
    // Do nothing
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>::Rectangle(const Point<T, Impl>& inPoint) :
    mImpl{inPoint.mImpl, geometry::Size<T, Impl>{}.mImpl}
{
    // Do nothing
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>::Rectangle(const geometry::Size<T, Impl>& inSize) :
    mImpl{Point<T, Impl>{}.mImpl, inSize.mImpl}
{
    // Do nothing
}

// Getters
template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl> Rectangle<T, Impl>::Origin() const
{
    return Point<T, Impl>{X(), Y()};
}

template<typename T, ImplKind Impl>
inline constexpr geometry::Size<T, Impl> Rectangle<T, Impl>::Size() const
{
    return geometry::Size<T, Impl>{Width(), Height()};
}

template<typename T, ImplKind Impl>
inline constexpr T Rectangle<T, Impl>::X() const
{
    return mImpl.Get<0>();
}

template<typename T, ImplKind Impl>
inline constexpr T Rectangle<T, Impl>::Y() const
{
    return mImpl.Get<1>();
}

template<typename T, ImplKind Impl>
inline constexpr T Rectangle<T, Impl>::Width() const
{
    return mImpl.Get<2>();
}

template<typename T, ImplKind Impl>
inline constexpr T Rectangle<T, Impl>::Height() const
{
    return mImpl.Get<3>();
}

// Setters
template<typename T, ImplKind Impl>
inline constexpr void Rectangle<T, Impl>::Origin(const Point<T, Impl>& inPoint)
{
	// REVISIT mnfitz 26apr2025: Investigate Set2() that sets both members at once
	mImpl.Set<0>(inPoint.X());
	mImpl.Set<1>(inPoint.Y());
}

template<typename T, ImplKind Impl>
inline constexpr void Rectangle<T, Impl>::Size(const geometry::Size<T, Impl>& inSize)
{
	// REVISIT mnfitz 26apr2025: Investigate Set2() that sets both members at once
	mImpl.Set<2>(inSize.Width());
	mImpl.Set<3>(inSize.Height());
}

template<typename T, ImplKind Impl>
constexpr void Rectangle<T, Impl>::X(T inX)
{
	mImpl.Set<0>(inX);
}

template<typename T, ImplKind Impl>
constexpr void Rectangle<T, Impl>::Y(T inY)
{
	mImpl.Set<1>(inY);
}

template<typename T, ImplKind Impl>
constexpr void Rectangle<T, Impl>::Width(T inWidth)
{
	mImpl.Set<2>(inWidth);
}

template<typename T, ImplKind Impl>
constexpr void Rectangle<T, Impl>::Height(T inHeight)
{
	mImpl.Set<3>(inHeight);
}

// Mutators
template<typename T, ImplKind Impl>
constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Translate(const Point<T, Impl>& inPoint)
{
	mImpl += Rectangle{inPoint}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Translate(T inX, T inY)
{
	mImpl += Rectangle{Point<T, Impl>{inX, inY}}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Translate(T inXY)
{
	mImpl += Rectangle{Point<T, Impl>{inXY, inXY}}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Enlarge(const geometry::Size<T, Impl>& inSize)
{
	mImpl += Rectangle{inSize}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Enlarge(T inX, T inY)
{
	mImpl += Rectangle{geometry::Size<T, Impl>{inX, inY}}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Enlarge(T inXY)
{
	mImpl += Rectangle{geometry::Size<T, Impl>{inXY, inXY}}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Scale(const Point<T, Impl>& inPoint)
{
	mImpl *= Rectangle{inPoint, saber::ConvertTo<geometry::Size<T, Impl>>(inPoint)}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Scale(const geometry::Size<T, Impl>& inSize)
{
	mImpl *= Rectangle{saber::ConvertTo<Point<T, Impl>>(inSize), inSize}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Scale(T inX, T inY)
{
	mImpl *= Rectangle{inX, inY, inX, inY}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Scale(T inXY)
{
	mImpl *= Rectangle{inXY, inXY, inXY, inXY}.mImpl;
	return *this;
}

#pragma endregion

template<typename T, ImplKind Impl>
inline constexpr bool Rectangle<T, Impl>::IsEqual(const Rectangle& inRectangle) const
{
    auto result = mImpl.IsEqual(inRectangle.mImpl);
    return result;
}

#if 0
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

#pragma endregion

// ------------------------------------------------------------------
#pragma region Free Functions

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl> Translate(const Point<T, Impl>& inPoint, const Point<T, Impl>& inTranslate)
{
    Point<T, Impl> result{inPoint};
    result += inTranslate;
    return result;
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl> Translate(const Point<T, Impl>& inPoint, T inX, T inY)
{
    const Point<T, Impl> translate{inX, inY};
    return Translate(inPoint, translate);
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl> Translate(const Point<T, Impl>& inPoint, T inTranslate)
{
    return Translate(inPoint, inTranslate, inTranslate);
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl> Scale(const Point<T, Impl>& inPoint, const Point<T, Impl>& inScale)
{
    Point<T, Impl> result{inPoint};
    result *= inScale;
    return result;
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl> Scale(const Point<T, Impl>& inPoint, T inScaleX, T inScaleY)
{
    const Point<T, Impl> scale{inScaleX, inScaleY};
    return Scale(inPoint, scale);
}

template<typename T, ImplKind Impl>
inline constexpr Point<T, Impl> Scale(const Point<T, Impl>& inPoint, T inScale)
{
    return Scale(inPoint, inScale, inScale);
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
#endif // #if 0
}// namespace saber::geometry

#endif // SABER_GEOMETRY_RECTANGLE_HPP