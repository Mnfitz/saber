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

/// @brief Represents a rectangle in 2D space.
/// @tparam T The type of the rectangle's coordinates (e.g., int, float).
/// @tparam ImplType The implementation kind (e.g., scalar or SIMD).
template<typename T, ImplKind Impl = ImplKind::kDefault>
class Rectangle
{
public:
	using ValueType = T;

public:
	/// @brief Default constructor. Initializes a rectangle with default values.
	constexpr Rectangle() = default;

	/// @brief Constructs a rectangle from a point and a size.
	/// @param inPoint The origin point of the rectangle.
	/// @param inSize The size (width and height) of the rectangle.
	constexpr Rectangle(const Point<T, Impl>& inPoint, const geometry::Size<T, Impl>& inSize);

	/// @brief Constructs a rectangle from x, y, width, and height.
	/// @param inX The x-coordinate of the origin.
	/// @param inY The y-coordinate of the origin.
	/// @param inWidth The width of the rectangle.
	/// @param inHeight The height of the rectangle.
	constexpr Rectangle(T inX, T inY, T inWidth, T inHeight);

	/// @brief Constructs a rectangle from a point, with size initialized to zero.
	/// @param inPoint The origin point of the rectangle.
	constexpr Rectangle(const Point<T, Impl>& inPoint);

	/// @brief Constructs a rectangle from a size, with origin initialized to zero.
	/// @param inSize The size (width and height) of the rectangle.
	constexpr Rectangle(const geometry::Size<T, Impl>& inSize);

	/// @brief Destructor.
	~Rectangle() = default;

	/// @brief Move constructor.
	constexpr Rectangle(Rectangle&& ioMove) noexcept = default;

	/// @brief Move assignment operator.
	constexpr Rectangle& operator=(Rectangle&& ioMove) noexcept = default;

	/// @brief Copy constructor.
	constexpr Rectangle(const Rectangle& inCopy) = default;

	/// @brief Copy assignment operator.
	constexpr Rectangle& operator=(const Rectangle& inCopy) = default;

	// Getters

	/// @brief Gets the origin point of the rectangle.
	/// @return The origin as a Point.
	constexpr Point<T, Impl> Origin() const;

	/// @brief Gets the size of the rectangle.
	/// @return The size as a Size object.
	constexpr geometry::Size<T, Impl> Size() const;

	/// @brief Gets the x-coordinate of the rectangle's origin.
	/// @return The x-coordinate.
	constexpr T X() const;

	/// @brief Gets the y-coordinate of the rectangle's origin.
	/// @return The y-coordinate.
	constexpr T Y() const;

	/// @brief Gets the width of the rectangle.
	/// @return The width.
	constexpr T Width() const;

	/// @brief Gets the height of the rectangle.
	/// @return The height.
	constexpr T Height() const;

	// Setters

	/// @brief Sets the origin of the rectangle.
	/// @param inPoint The new origin point.
	constexpr void Origin(const Point<T, Impl>& inPoint);

	/// @brief Sets the size of the rectangle.
	/// @param inSize The new size.
	constexpr void Size(const geometry::Size<T, Impl>& inSize);

	/// @brief Sets the x-coordinate of the rectangle's origin.
	/// @param inX The new x-coordinate.
	constexpr void X(T inX);

	/// @brief Sets the y-coordinate of the rectangle's origin.
	/// @param inY The new y-coordinate.
	constexpr void Y(T inY);

	/// @brief Sets the width of the rectangle.
	/// @param inWidth The new width.
	constexpr void Width(T inWidth);

	/// @brief Sets the height of the rectangle.
	/// @param inHeight The new height.
	constexpr void Height(T inHeight);

	// Mutators

	/// @brief Translates the rectangle by a point offset.
	/// @param inPoint The point by which to translate.
	/// @return Reference to this rectangle.
	constexpr Rectangle& Translate(const Point<T, Impl>& inPoint);

	/// @brief Translates the rectangle by x and y offsets.
	/// @param inX The x offset.
	/// @param inY The y offset.
	/// @return Reference to this rectangle.
	constexpr Rectangle& Translate(T inX, T inY);

	/// @brief Translates the rectangle by the same offset in both x and y.
	/// @param inXY The offset for both axes.
	/// @return Reference to this rectangle.
	constexpr Rectangle& Translate(T inXY);

	/// @brief Enlarges the rectangle by a size.
	/// @param inSize The size to add.
	/// @return Reference to this rectangle.
	constexpr Rectangle& Enlarge(const geometry::Size<T, Impl>& inSize);

	/// @brief Enlarges the rectangle by x and y amounts.
	/// @param inX Amount to enlarge width.
	/// @param inY Amount to enlarge height.
	/// @return Reference to this rectangle.
	constexpr Rectangle& Enlarge(T inX, T inY);

	/// @brief Enlarges the rectangle by the same amount in both width and height.
	/// @param inXY Amount to enlarge both width and height.
	/// @return Reference to this rectangle.
	constexpr Rectangle& Enlarge(T inXY);

	/// @brief Scales the rectangle's origin and size by a point.
	/// @param inPoint The point to scale by.
	/// @return Reference to this rectangle.
	constexpr Rectangle& Scale(const Point<T, Impl>& inPoint);

	/// @brief Scales the rectangle's origin and size by a size.
	/// @param inSize The size to scale by.
	/// @return Reference to this rectangle.
	constexpr Rectangle& Scale(const geometry::Size<T, Impl>& inSize);

	/// @brief Scales the rectangle's origin and size by x and y factors.
	/// @param inX The x scale factor.
	/// @param inY The y scale factor.
	/// @return Reference to this rectangle.
	constexpr Rectangle& Scale(T inX, T inY);

	/// @brief Scales the rectangle's origin and size by the same factor.
	/// @param inXY The scale factor for both axes.
	/// @return Reference to this rectangle.
	constexpr Rectangle& Scale(T inXY);

	/// @brief Unions this rectangle with another.
	/// @param inRectangle The rectangle to union with.
	/// @return Reference to this rectangle.
	constexpr Rectangle& Union(const Rectangle& inRectangle);

	/// @brief Intersects this rectangle with another.
	/// @param inRectangle The rectangle to intersect with.
	/// @return Reference to this rectangle.
	constexpr Rectangle& Intersect(const Rectangle& inRectangle);

	/// @brief Checks if given point overlaps this rectangle.
	/// @param inPoint The point to check.
	/// @return True if the rectangle overlaps the point, false otherwise.
	constexpr bool IsOverlapping(const Point<T, Impl>& inPoint) const;

	/// @brief Checks if given rectangle overlaps this rectangle.
	/// @param inRectangle The rectangle to check.
	/// @return True if this rectangle overlaps the other, false otherwise.
	constexpr bool IsOverlapping(const Rectangle& inRectangle) const;

	// --- Rounding ---

	/// @brief Round this rectangle to nearest integer value; both origin and scale. Halfway cases round away from zero. Compatible with std::round().
	/// @tparam U Underlying Rectangle<T> type (U: because T already in-use by Rectangle<T>)
	/// @tparam SFINAE Enable only for floating point types
	/// @return Reference to this rectangle.
	template<typename U = T, typename SFINAE = std::enable_if_t<std::is_floating_point_v<U>>>
	constexpr Rectangle& RoundNearest();

	/// @brief Round this rectangle toward -infinity to nearest integer value; both origin and scale. Compatible with std::floor().
	/// @tparam U Underlying Rectangle<T> type
	/// @tparam SFINAE Enable only for floating point types
	/// @return Reference to this rectangle.
	template<typename U = T, typename SFINAE = std::enable_if_t<std::is_floating_point_v<U>>>
	constexpr Rectangle& RoundFloor();

	/// @brief Round this rectangle toward +infinity to nearest integer value; both origin and scale. Compatible with std::ceil().
	/// @tparam U Underlying Rectangle<T> type
	/// @tparam SFINAE Enable only for floating point types
	/// @return Reference to this rectangle.
	template<typename U = T, typename SFINAE = std::enable_if_t<std::is_floating_point_v<U>>>
	constexpr Rectangle& RoundCeil();

	/// @brief Round this rectangle toward zero to nearest integer value; both origin and scale. Compatible with std::trunc().
	/// @tparam U Underlying Rectangle<T> type
	/// @tparam SFINAE Enable only for floating point types
	/// @return Reference to this rectangle.
	template<typename U = T, typename SFINAE = std::enable_if_t<std::is_floating_point_v<U>>>
	constexpr Rectangle& RoundTrunc();

private:
	// Private APIs

	/// @brief Checks if this rectangle is equal to another.
	/// @param inRectangle The rectangle to compare with.
	/// @return True if equal, false otherwise.
	constexpr bool IsEqual(const Rectangle& inRectangle) const;

	// Friend functions
private:
	friend constexpr bool operator==<>(const Rectangle& inLHS, const Rectangle& inRHS);
	friend constexpr bool operator!=<>(const Rectangle& inLHS, const Rectangle& inRHS);

	template<typename T, ImplKind Impl>
	friend constexpr bool IsEmpty(const Rectangle<T, Impl>& inRectangle);

private:
	using ImplType = typename detail::Impl4Traits<T, Impl>::ImplType; // VOODOO: Nested template type requires `typename` prefix
	ImplType mImpl{};
}; // class Rectangle<>

// ------------------------------------------------------------------
#pragma region Inline Class Functions

// Ctors
template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>::Rectangle(T inX, T inY, T inWidth, T inHeight) : mImpl{inX, inY, inWidth, inHeight}
{
	// Do nothing
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>::Rectangle(const Point<T, Impl>& inPoint, const geometry::Size<T, Impl>& inSize) : mImpl{inPoint.mImpl, inSize.mImpl}
{
	// Do nothing
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>::Rectangle(const Point<T, Impl>& inPoint) : mImpl{inPoint.mImpl, geometry::Size<T, Impl>{}.mImpl}
{
	// Do nothing
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>::Rectangle(const geometry::Size<T, Impl>& inSize) : mImpl{Point<T, Impl>{}.mImpl, inSize.mImpl}
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
	mImpl.SetLo(inPoint.mImpl);
}

template<typename T, ImplKind Impl>
inline constexpr void Rectangle<T, Impl>::Size(const geometry::Size<T, Impl>& inSize)
{
	mImpl.SetHi(inSize.mImpl);
}

template<typename T, ImplKind Impl>
inline constexpr void Rectangle<T, Impl>::X(T inX)
{
	mImpl.Get<0>() = inX;
}

template<typename T, ImplKind Impl>
inline constexpr void Rectangle<T, Impl>::Y(T inY)
{
	mImpl.Get<1>() = inY;
}

template<typename T, ImplKind Impl>
inline constexpr void Rectangle<T, Impl>::Width(T inWidth)
{
	mImpl.Get<2>() = inWidth;
}

template<typename T, ImplKind Impl>
inline constexpr void Rectangle<T, Impl>::Height(T inHeight)
{
	mImpl.Get<3>() = inHeight;
}

// Mutators
template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Translate(const Point<T, Impl>& inPoint)
{
	mImpl += Rectangle{inPoint}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Translate(T inX, T inY)
{
	mImpl += Rectangle{Point<T, Impl>{inX, inY}}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Translate(T inXY)
{
	mImpl += Rectangle{Point<T, Impl>{inXY, inXY}}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Enlarge(const geometry::Size<T, Impl>& inSize)
{
	mImpl += Rectangle{inSize}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Enlarge(T inX, T inY)
{
	mImpl += Rectangle{geometry::Size<T, Impl>{inX, inY}}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Enlarge(T inXY)
{
	mImpl += Rectangle{geometry::Size<T, Impl>{inXY, inXY}}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Scale(const Point<T, Impl>& inPoint)
{
	mImpl *= Rectangle{inPoint, saber::ConvertTo<geometry::Size<T, Impl>>(inPoint)}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Scale(const geometry::Size<T, Impl>& inSize)
{
	mImpl *= Rectangle{saber::ConvertTo<Point<T, Impl>>(inSize), inSize}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Scale(T inX, T inY)
{
	mImpl *= Rectangle{inX, inY, inX, inY}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Scale(T inXY)
{
	mImpl *= Rectangle{inXY, inXY, inXY, inXY}.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Union(const Rectangle& inRectangle)
{
	mImpl.Union(inRectangle.mImpl);
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::Intersect(const Rectangle& inRectangle)
{
	mImpl.Intersect(inRectangle.mImpl);
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr bool Rectangle<T, Impl>::IsOverlapping(const Point<T, Impl>& inPoint) const
{
	const bool isOverlapping = mImpl.IsOverlapping(inPoint.mImpl);
	return isOverlapping;
}

template<typename T, ImplKind Impl>
inline constexpr bool Rectangle<T, Impl>::IsOverlapping(const Rectangle& inRectangle) const
{
	const bool isOverlapping = mImpl.IsOverlapping(inRectangle.mImpl);
	return isOverlapping;
}

#pragma endregion

template<typename T, ImplKind Impl>
inline constexpr bool Rectangle<T, Impl>::IsEqual(const Rectangle& inRectangle) const
{
	auto result = mImpl.IsEqual(inRectangle.mImpl);
	return result;
}

// ------------------------------------------------------------------
#pragma region Inline Rounding operations

template<typename T, ImplKind Impl>
template<typename U, typename SFINAE>
inline constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::RoundNearest()
{
	mImpl.RoundNearest();
	return *this;
}

template<typename T, ImplKind Impl>
template<typename U, typename SFINAE>
inline constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::RoundFloor()
{
	mImpl.RoundFloor();
	return *this;
}

template<typename T, ImplKind Impl>
template<typename U, typename SFINAE>
inline constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::RoundCeil()
{
	mImpl.RoundCeil();
	return *this;
}

template<typename T, ImplKind Impl>
template<typename U, typename SFINAE>
inline constexpr Rectangle<T, Impl>& Rectangle<T, Impl>::RoundTrunc()
{
	mImpl.RoundTrunc();
	return *this;
}

#pragma endregion

#pragma region Free Functions
/// @brief Round to nearest even integer value; both origin and scale. Halfway cases round away from zero.
/// @tparam T: Underlying `Rectangle<>` type
/// @tparam ImplType: Optional underlying implementation type
/// @param inRectangle: `Rectangle<>` object to be rounded
/// @return Rounded `Rectangle<>` result
template<typename T, ImplKind Impl, typename SFINAE = std::enable_if_t<std::is_floating_point_v<T>>>
inline constexpr Rectangle<T, Impl> RoundNearest(const Rectangle<T, Impl>& inRectangle)
{
	constexpr bool kIsFloatingPoint = std::is_floating_point_v<T>;
	static_assert(kIsFloatingPoint, "RoundNearest() only supports floating point types");

	auto result{inRectangle};
	return result.RoundNearest(); // RVO should apply here
}

/// @brief Round towards zero to nearest integer value; both origin and scale.
/// @tparam T: Underlying `Rectangle<>` type
/// @tparam ImplType: Optional underlying implementation type
/// @param inRectangle: `Rectangle<>` object to be rounded
/// @return Rounded `Rectangle<>` result
template<typename T, ImplKind Impl, typename SFINAE = std::enable_if_t<std::is_floating_point_v<T>>>
inline constexpr Rectangle<T, Impl> RoundTrunc(const Rectangle<T, Impl>& inRectangle)
{
	constexpr bool kIsFloatingPoint = std::is_floating_point_v<T>;
	static_assert(kIsFloatingPoint, "RoundTrunc() only supports floating point types");

	auto result{inRectangle};
	return result.RoundTrunc(); // RVO should apply here
}

/// @brief Round towards +infinity to the nearest integer value; both origin and scale.
/// @tparam T: Underlying `Rectangle<>` type
/// @tparam ImplType: Optional underlying implementation type
/// @param inRectangle: `Rectangle<>` object to be rounded
/// @return Rounded `Rectangle<>` result
template<typename T, ImplKind Impl, typename SFINAE = std::enable_if_t<std::is_floating_point_v<T>>>
inline constexpr Rectangle<T, Impl> RoundCeil(const Rectangle<T, Impl>& inRectangle)
{
	constexpr bool kIsFloatingPoint = std::is_floating_point_v<T>;
	static_assert(kIsFloatingPoint, "RoundCeil() only supports floating point types");

	auto result{inRectangle};
	return result.RoundCeil(); // RVO should apply here
}

/// @brief Round towards -infinity to the nearest integer value; both origin and scale.
/// @tparam T: Underlying `Rectangle<>` type
/// @tparam Impl: Optional underlying implementation type
/// @param inRectangle: `Rectangle<>` object to be rounded
/// @return Rounded `Rectangle<>` result
template<typename T, ImplKind Impl, typename SFINAE = std::enable_if_t<std::is_floating_point_v<T>>>
inline constexpr Rectangle<T, Impl> RoundFloor(const Rectangle<T, Impl>& inRectangle)
{
	constexpr bool kIsFloatingPoint = std::is_floating_point_v<T>;
	static_assert(kIsFloatingPoint, "RoundFloor() only supports floating point types");

	auto result{inRectangle};
	return result.RoundFloor(); // RVO should apply here
}

// Mutators

/// @brief Translates the rectangle by a point offset.
/// @param inPoint The point by which to translate.
/// @return Reference to this rectangle.
template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl> Translate(const Rectangle<T, Impl>& inRectangle, const Point<T, Impl>& inPoint)
{
	auto result{inRectangle};
	return result.Translate(inPoint); // RVO does NOT apply here :sad:
}

/// @brief Translates the rectangle by x and y offsets.
/// @param inX The x offset.
/// @param inY The y offset.
/// @return Reference to this rectangle.
template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl> Translate(const Rectangle<T, Impl>& inRectangle, T inX, T inY)
{
	auto result{inRectangle};
	return result.Translate(inX, inY);
}

/// @brief Translates the rectangle by the same offset in both x and y.
/// @param inXY The offset for both axes.
/// @return Reference to this rectangle.
template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl> Translate(const Rectangle<T, Impl>& inRectangle, T inXY)
{
	auto result{inRectangle};
	return result.Translate(inXY);
}

/// @brief Enlarges the rectangle by a size.
/// @param inSize The size to add.
/// @return Reference to this rectangle.
template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl> Enlarge(const Rectangle<T, Impl>& inRectangle, const geometry::Size<T, Impl>& inSize)
{
	auto result{inRectangle};
	return result.Enlarge(inSize);
}

/// @brief Enlarges the rectangle by x and y amounts.
/// @param inX Amount to enlarge width.
/// @param inY Amount to enlarge height.
/// @return Reference to this rectangle.
template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl> Enlarge(const Rectangle<T, Impl>& inRectangle, T inX, T inY)
{
	auto result{inRectangle};
	return result.Enlarge(inX, inY);
}

/// @brief Enlarges the rectangle by the same amount in both width and height.
/// @param inXY Amount to enlarge both width and height.
/// @return Reference to this rectangle.
template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl> Enlarge(const Rectangle<T, Impl>& inRectangle, T inXY)
{
	auto result{inRectangle};
	return result.Enlarge(inXY);
}

/// @brief Scales the rectangle's origin and size by a point.
/// @param inPoint The point to scale by.
/// @return Reference to this rectangle.
template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl> Scale(const Rectangle<T, Impl>& inRectangle, const Point<T, Impl>& inPoint)
{
	auto result{inRectangle};
	return result.Scale(inPoint);
}

/// @brief Scales the rectangle's origin and size by a size.
/// @param inSize The size to scale by.
/// @return Reference to this rectangle.
template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl> Scale(const Rectangle<T, Impl>& inRectangle, const geometry::Size<T, Impl>& inSize)
{
	auto result{inRectangle};
	return result.Scale(inSize);
}

/// @brief Scales the rectangle's origin and size by x and y factors.
/// @param inX The x scale factor.
/// @param inY The y scale factor.
/// @return Reference to this rectangle.
template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl> Scale(const Rectangle<T, Impl>& inRectangle, T inX, T inY)
{
	auto result{inRectangle};
	return result.Scale(inX, inY);
}

/// @brief Scales the rectangle's origin and size by the same factor.
/// @param inXY The scale factor for both axes.
/// @return Reference to this rectangle.
template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl> Scale(const Rectangle<T, Impl>& inRectangle, T inXY)
{
	auto result{inRectangle};
	return result.Scale(inXY);
}

/// @brief Compute the union of two rectangles and return the resulting rectangle.
/// @tparam T Underlying coordinate type
/// @tparam Impl Optional implementation kind (scalar or simd)
/// @param inLHS Left-hand rectangle
/// @param inRHS Right-hand rectangle
/// @return Rectangle representing the minimal bounding rectangle that contains both inputs
template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl> Union(const Rectangle<T, Impl>& inLHS, const Rectangle<T, Impl>& inRHS)
{
	auto result{inLHS};
	return result.Union(inRHS);
}

/// @brief Compute the intersection of two rectangles and return the overlapping rectangle.
/// @tparam T Underlying coordinate type
/// @tparam Impl Optional implementation kind (scalar or simd)
/// @param inLHS Left-hand rectangle
/// @param inRHS Right-hand rectangle
/// @return Rectangle representing the overlapping area; may be empty
template<typename T, ImplKind Impl>
inline constexpr Rectangle<T, Impl> Intersect(const Rectangle<T, Impl>& inLHS, const Rectangle<T, Impl>& inRHS)
{
	auto intersect{inLHS};
	return intersect.Intersect(inRHS);
}

/// @brief Test whether a rectangle is empty (zero area) using its underlying impl.
/// @tparam T Underlying coordinate type
/// @tparam Impl Optional implementation kind (scalar or simd)
/// @param inRectangle Rectangle to test
/// @return true when the rectangle area is empty (no overlap), false otherwise
template<typename T, ImplKind Impl>
inline constexpr bool IsEmpty(const Rectangle<T, Impl>& inRectangle)
{
	// Note: Compiler cannot deduce IsEmpty's ImplKind, so we must specify template arg
	const auto isEmpty = IsEmpty(inRectangle.mImpl);
	return isEmpty;
}

/// @brief Test whether a point lies within a rectangle.
/// @tparam T Underlying coordinate type
/// @tparam Impl Optional implementation kind (scalar or simd)
/// @param inRectangle Rectangle to test against
/// @param inPoint Point to check
/// @return true when the point is inside the rectangle, false otherwise
template<typename T, ImplKind Impl>
inline constexpr bool IsOverlapping(const Rectangle<T, Impl>& inRectangle, const Point<T, Impl>& inPoint)
{
	const auto isOverlapping = inRectangle.IsOverlapping(inPoint);
	return isOverlapping;
}

/// @brief Test whether two rectangles overlap (have a non-empty intersection).
/// @tparam T Underlying coordinate type
/// @tparam Impl Optional implementation kind (scalar or simd)
/// @param inLHS First rectangle
/// @param inRHS Second rectangle
/// @return true when rectangles overlap, false otherwise
template<typename T, ImplKind Impl>
inline constexpr bool IsOverlapping(const Rectangle<T, Impl>& inLHS, const Rectangle<T, Impl>& inRHS)
{
	const auto isOverlapping = inLHS.IsOverlapping(inRHS);
	return isOverlapping;
}

#pragma endregion
#pragma endregion

#if 0
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
} // namespace saber::geometry

#endif // SABER_GEOMETRY_RECTANGLE_HPP