#ifndef SABER_GEOMETRY_SIZE_HPP
#define SABER_GEOMETRY_SIZE_HPP
#pragma once

// saber
#include "saber/geometry/config.hpp"
#include "saber/geometry/operators.hpp"
#include "saber/geometry/detail/impl2.hpp"

//std
#include <type_traits>

namespace saber::geometry {

// Forward Declaration
template<typename T, ImplKind Impl>
class Rectangle;

/// @brief 
/// @tparam T 
/// @tparam ImplKind 
template<typename T, ImplKind Impl = ImplKind::kDefault>
class Size 
{
public:
    using ValueType = T;

public: 
	constexpr Size(T inWidth, T inHeight);

	constexpr Size() = default;
	~Size() = default;

	// RO5 is all default implemented
	constexpr Size(Size&& ioMove) noexcept = default;
	constexpr Size& operator=(Size&& ioMove) noexcept = default;

	constexpr Size(const Size& inCopy) = default;
	constexpr Size& operator=(const Size& inCopy) = default;

	// Getters
	constexpr T Width() const;
	constexpr T Height() const;

	// Setters
	constexpr void Width(T inWidth);
	constexpr void Height(T inHeight);

	// Mathematical operations
	constexpr Size& operator+=(const Size& inSize);
	constexpr Size& operator-=(const Size& inSize);
	constexpr Size& operator*=(const Size& inSize);
	constexpr Size& operator/=(const Size& inSize);

	// --- Rounding ---

	// TRICKY mnfitz 22feb2025: SFINAE-enable rounding methods only for floating point types.
	// C++ Black magic: SFINAE (template substitution failure) will disallow
	// template `T` types that do not satisfy `enable_if` condition. This prevents rounding
	// methods from "being enabled" for non floating point types, like: `Size<int>`

	/// @brief Round this `Size<>` to nearest integer value. Halfway cases round away from zero. Compatible with `std::round()`
	/// @tparam U: Underlying `Size<T>` type (U: cuz T already in-use by Size<T>)
	/// @tparam SFINAE: Enable `RoundNearest()` only for floating point types
	/// @return Ref& to this `Size<>`
	template<typename U=T, typename SFINAE = std::enable_if_t<std::is_floating_point_v<U>>>
	constexpr Size& RoundNearest();

	/// @brief Round this `Size<>` toward -infinity to nearest integer value. Compatible with `std::floor()`
	/// @tparam U: Underlying `Size<T>` type (U: cuz T already in-use by Size<T>)
	/// @tparam SFINAE: Enable `RoundFloor()` only for floating point types
	/// @return Ref& to this `Size<>`
	template<typename U=T, typename SFINAE = std::enable_if_t<std::is_floating_point_v<U>>>
	constexpr Size& RoundFloor();

	/// @brief Round this `Size<>` toward +infinity to nearest integer value. Compatible with `std::ceil()`
	/// @tparam U: Underlying `Size<T>` type (U: cuz T already in-use by Size<T>)
	/// @tparam SFINAE: Enable `RoundCeil()` only for floating point types
	/// @return Ref& to this `Size<>`
	template<typename U=T, typename SFINAE = std::enable_if_t<std::is_floating_point_v<U>>>
	constexpr Size& RoundCeil();

	/// @brief Round this `Size<>` toward zero to nearest integer value. Compatible with `std::trunc()`
	/// @tparam U: Underlying `Size<T>` type (U: cuz T already in-use by Size<T>)
	/// @tparam SFINAE: Enable `RoundTrunc()` only for floating point types
	/// @return Ref& to this `Size<>`
	template<typename U=T, typename SFINAE = std::enable_if_t<std::is_floating_point_v<U>>>
	constexpr Size& RoundTrunc();

private:
	// Private APIs
	constexpr bool IsEqual(const Size& inSize) const;

private:
	// Friend Functions
	friend constexpr bool operator==<Size>(const Size& inLHS, const Size& inRHS);
	friend constexpr bool operator!=<Size>(const Size& inLHS, const Size& inRHS);
	friend class Rectangle<T, Impl>;

private:
	using ImplType = typename detail::Impl2Traits<T, Impl>::ImplType; // VOODOO: Nested template type requires `typename` prefix
    ImplType mImpl{};
}; // class Size

// ------------------------------------------------------------------
#pragma region Inline Class Functions

template<typename T, ImplKind Impl>
inline constexpr Size<T, Impl>::Size(T inWidth, T inHeight) :
	mImpl{inWidth, inHeight}
{
	// Do nothing
}

// Getters
template<typename T, ImplKind Impl>
inline constexpr T Size<T, Impl>::Width() const
{
	return mImpl.Get<0>();
}

template<typename T, ImplKind Impl>
inline constexpr T Size<T, Impl>::Height() const
{
	return mImpl.Get<1>();
}

// Setters
template<typename T, ImplKind Impl>
inline constexpr void Size<T, Impl>::Width(T inWidth)
{
	mImpl.Set<0>(inWidth);
}

template<typename T, ImplKind Impl>
inline constexpr void Size<T, Impl>::Height(T inHeight)
{
	mImpl.Set<1>(inHeight);
}

#pragma endregion

// ------------------------------------------------------------------
#pragma region Inline Mathematical operations

template<typename T, ImplKind Impl>
inline constexpr Size<T, Impl>& Size<T, Impl>::operator+=(const Size& inSize)
{
	mImpl += inSize.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Size<T, Impl>& Size<T, Impl>::operator-=(const Size& inSize)
{
	mImpl -= inSize.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Size<T, Impl>& Size<T, Impl>::operator*=(const Size& inSize)
{
	mImpl *= inSize.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Size<T, Impl>& Size<T, Impl>::operator/=(const Size& inSize)
{
	mImpl /= inSize.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr bool Size<T, Impl>::IsEqual(const Size& inSize) const
{
	auto result = mImpl.IsEqual(inSize.mImpl);
	return result;
}

template<typename T, ImplKind Impl>
template<typename U, typename SFINAE>
inline constexpr Size<T, Impl>& Size<T, Impl>::RoundNearest()
{
	mImpl.RoundNearest();
	return *this;
}

template<typename T, ImplKind Impl>
template<typename U, typename SFINAE>
inline constexpr Size<T, Impl>& Size<T, Impl>::RoundFloor()
{
	mImpl.RoundFloor();
	return *this;
}

template<typename T, ImplKind Impl>
template<typename U, typename SFINAE>
inline constexpr Size<T, Impl>& Size<T, Impl>::RoundCeil()
{
	mImpl.RoundCeil();
	return *this;
}

template<typename T, ImplKind Impl>
template<typename U, typename SFINAE>
inline constexpr Size<T, Impl>& Size<T, Impl>::RoundTrunc()
{
	mImpl.RoundTrunc();
	return *this;
}

#pragma endregion

// ------------------------------------------------------------------
#pragma region Free Functions

/// @brief Enlarge a `Size<>` using another `Size<>`
/// @tparam T: Underlying `Size<>` type
/// @tparam ImplKind: Optional underlying implementation type
/// @param inSize: `Size<>` object to emlarge
/// @param inMagnitude: Width/Height to be added to `inSize`
/// @return Resized `Size<>` result
template<typename T, ImplKind Impl>
inline constexpr Size<T, Impl> Enlarge(const Size<T, Impl>& inSize, const Size<T, Impl>& inMagnitude)
{
	Size<T, Impl> result{inSize};
	result += inMagnitude;
	return result;
}

/// @brief Enlarge a `Size<>` using scalar X and Y
/// @tparam T: Underlying `Size<>` type
/// @tparam ImplKind: Optional underlying implementation type
/// @param inSize: `Size<>` object to emlarge
/// @param inX: Width to be added to `inSize`
/// @param inY: Height to be added to `inSize`
/// @return Resized `Size<>` result
template<typename T, ImplKind Impl>
inline constexpr Size<T, Impl> Enlarge(const Size<T, Impl>& inSize, T inX, T inY)
{
	const Size<T, Impl> magnitude{inX, inY};
	return Enlarge(inSize, magnitude);
}

/// @brief Enlarge a `Size<>` using scalar inMagnitude
/// @tparam T: Underlying `Size<>` type
/// @tparam ImplKind: Optional underlying implementation type
/// @param inSize: `Size<>` object to emlarge
/// @param inMagnitude: Amount to add to `inSize` Width/Height
/// @return Resized `Size<>` result
template<typename T, ImplKind Impl>
inline constexpr Size<T, Impl> Enlarge(const Size<T, Impl>& inSize, T inMagnitude)
{
	return Enlarge(inSize, inMagnitude, inMagnitude);
}

/// @brief Scale a `Size<>` using another `Size<>`
/// @tparam T: Underlying `Size<>` type
/// @tparam ImplKind: Optional underlying implementation type
/// @param inSize: `Size<>` object to be scaled
/// @param inScale: Scale factor to apply
/// @return Scaled `Size<>` result
template<typename T, ImplKind Impl>
inline constexpr Size<T, Impl> Scale(const Size<T, Impl>& inSize, const Size<T, Impl>& inScale)
{
	Size<T, Impl> result{inSize};
	result *= inScale;
	return result;
}

/// @brief Scale a `Size<>` using a scalar X and Y
/// @tparam T: Underlying `Size<>` type
/// @tparam ImplKind: Optional underlying implementation type
/// @param inSize: `Size<>` object to be scaled
/// @param inScaleX: Width scale factor to apply
/// @param inScaleY: Height scale factor to apply
/// @return Scaled `Size<>` result
template<typename T, ImplKind Impl>
inline constexpr Size<T, Impl> Scale(const Size<T, Impl>& inSize, T inScaleX, T inScaleY)
{
	const Size<T, Impl> scale{inScaleX, inScaleY};
	return Scale(inSize, scale);
}

/// @brief Scale a `Size<>` using scalar XY
/// @tparam T: Underlying `Size<>` type
/// @tparam ImplKind: Optional underlying implementation type
/// @param inSize: `Size<>` object to be scaled
/// @param inScaleXY: Scale factor to apply
/// @return Scaled `Size<>` result
template<typename T, ImplKind Impl>
inline constexpr Size<T, Impl> Scale(const Size<T, Impl>& inSize, T inScaleXY)
{
	return Scale(inSize, inScaleXY, inScaleXY);
}

/// @brief Round to nearest even integer value. Halfway cases round away from zero.
/// @tparam T: Underlying `Size<>` type
/// @tparam ImplKind: Optional underlying implementation type
/// @param inSize: `Size<>` object to be rounded
/// @return Rounded `Size<>` result
template<typename T, ImplKind Impl, typename SFINAE = std::enable_if_t<std::is_floating_point_v<T>>>
inline constexpr Size<T, Impl> RoundNearest(const Size<T, Impl>& inSize)
{
	constexpr bool kIsFloatingPoint = std::is_floating_point_v<T>;
	static_assert(kIsFloatingPoint, "RoundNearest() only supports floating point types");

	auto result{inSize};
	return result.RoundNearest(); // RVO should apply here
}

/// @brief Round towards zero to nearest integer value.
/// @tparam T: Underlying `Size<>` type
/// @tparam ImplKind: Optional underlying implementation type
/// @param inSize: `Size<>` object to be rounded
/// @return Rounded `Size<>` result
template<typename T, ImplKind Impl, typename SFINAE = std::enable_if_t<std::is_floating_point_v<T>>>
inline constexpr Size<T, Impl> RoundTrunc(const Size<T, Impl>& inSize)
{
	constexpr bool kIsFloatingPoint = std::is_floating_point_v<T>;
	static_assert(kIsFloatingPoint, "RoundTrunc() only supports floating point types");

	auto result{inSize};
	return result.RoundTrunc(); // RVO should apply here
}

/// @brief Round towards +infinity to the nearest integer value.
/// @tparam T: Underlying `Size<>` type
/// @tparam ImplKind: Optional underlying implementation type
/// @param inSize: `Size<>` object to be rounded
/// @return Rounded `Size<>` result
template<typename T, ImplKind Impl, typename SFINAE = std::enable_if_t<std::is_floating_point_v<T>>>
inline constexpr Size<T, Impl> RoundCeil(const Size<T, Impl>& inSize)
{
	constexpr bool kIsFloatingPoint = std::is_floating_point_v<T>;
	static_assert(kIsFloatingPoint, "RoundCeil() only supports floating point types");

	auto result{inSize};
	return result.RoundCeil(); // RVO should apply here
}

/// @brief Round towards -infinity to the nearest integer value.
/// @tparam T: Underlying `Size<>` type
/// @tparam ImplKind: Optional underlying implementation type
/// @param inSize: `Size<>` object to be rounded
/// @return Rounded `Size<>` result
template<typename T, ImplKind Impl, typename SFINAE = std::enable_if_t<std::is_floating_point_v<T>>>
inline constexpr Size<T, Impl> RoundFloor(const Size<T, Impl>& inSize)
{
	constexpr bool kIsFloatingPoint = std::is_floating_point_v<T>;
	static_assert(kIsFloatingPoint, "RoundFloor() only supports floating point types");

	auto result{inSize};
	return result.RoundFloor(); // RVO should apply here
}

#pragma endregion 

// ------------------------------------------------------------------
#pragma region Structured Bindings

// TRICKY mnfitz 14oct2024: Turn on structured binging support for C++17 or later
#ifdef __cpp_structured_bindings

// Prefer free function over class method
template<std::size_t Index, typename T>
inline T get(const Size<T>& inSize)
{
	static_assert(2 == std::tuple_size<Size<T>>::value); // Use own std::tuple_size<> specialization
	static_assert(Index < 2, "Unexpected Index for Size<T>");

	T result{};
	if constexpr (Index == 0)
	{
		result = inSize.Width();
	}
	else if constexpr (Index == 1)
	{
		result = inSize.Height();
	}
	return result;
}

template<typename T>
struct std::tuple_size<Size<T>> // Partial template specialization for: Size<T>
{
	// Number of elements in Size<T>'s structured binding
	static constexpr std::size_t value = 2;
};

template<std::size_t Index, typename T>
struct std::tuple_element<Index, Size<T>> // Partial template specialization for: Size<T>
{
	// Type of elements in Size<T>'s structured binding
	using type = T;
};

#endif // __cpp_structured_bindings
#pragma endregion

}// namespace saber::geometry

#endif // SABER_GEOMETRY_SIZE_HPP