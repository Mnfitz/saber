#ifndef SABER_GEOMETRY_SIZE_HPP
#define SABER_GEOMETRY_SIZE_HPP
#pragma once

// saber
#include "saber/geometry/operators.hpp"

namespace saber::geometry {

// REVIEW mnfitz 15jun2024:
// design issues involved with developing Size class
// 1. Appropriate namespace
// 2. Separate function declaration and definition vs java style combined
// 3. Has-a: naked types or std::tuple or std::array
// 4. Include guards should incorperate namespace location
// 5. Build up binary free operators from class operators +=, etc
template<typename T, typename ImplType = typename detail::Impl2<T>::Simd> // TRICKY mnfitz 19oct2024: nested type in a template class needs 'typename' prefix
class Size 
{
public: 
    constexpr Size(T inWidth, T inHeight);

    constexpr Size() = default;
    ~Size() = default;

    // RO5 is all default implemented
    constexpr Size(Size&& ioMove) noexcept = default;
    constexpr Size& operator=(Size&& ioMove) noexcept = default;

    constexpr Size(const Size& inCopy) = default;
    constexpr Size& operator=(const Size& inCopy) = default;

    constexpr T Width() const;
    constexpr T Height() const;

    // Mathematical operations
    constexpr Size& operator+=(const Size& inSize);
    constexpr Size& operator-=(const Size& inSize);
    constexpr Size& operator*=(const Size& inSize);
    constexpr Size& operator/=(const Size& inSize);

    // Private APIs
private:
    constexpr bool IsEqual(const Size& inSize) const;
    constexpr void RoundNearest();
    constexpr void RoundFloor();
    constexpr void RoundCeil();
    constexpr void RoundZero();

    // Friend Functions
private:
    friend constexpr bool operator==<Size>(const Size& inLHS, const Size& inRHS);
    friend constexpr bool operator!=<Size>(const Size& inLHS, const Size& inRHS);
    friend constexpr Size RoundNearest(const Size& inSize);
    friend constexpr Size RoundFloor(const Size& inSize);
    friend constexpr Size RoundCeil(const Size& inSize);
    friend constexpr Size RoundZero(const Size& inSize);

private:
    ImplType mImpl{};
}; // class size

// ------------------------------------------------------------------
#pragma region Inline Class Functions

template<typename T, typename ImplType>
inline constexpr Size<T, ImplType>::Size(T inWidth, T inHeight) :
    mImpl{inWidth, inHeight}
{
    // Do nothing
}

template<typename T, typename ImplType>
inline constexpr T Size<T, ImplType>::Width() const
{
    return mImpl.Get<0>();
}

template<typename T, typename ImplType>
inline constexpr T Size<T, ImplType>::Height() const
{
    return mImpl.Get<1>();
}

#pragma endregion

// ------------------------------------------------------------------
#pragma region Inline Mathematical operations

template<typename T, typename ImplType>
inline constexpr Size<T, ImplType>& Size<T, ImplType>::operator+=(const Size& inSize)
{
    mImpl += inSize.mImpl;
    return *this;
}

template<typename T, typename ImplType>
inline constexpr Size<T, ImplType>& Size<T, ImplType>::operator-=(const Size& inSize)
{
    mImpl -= inSize.mImpl;
    return *this;
}

template<typename T, typename ImplType>
inline constexpr Size<T, ImplType>& Size<T, ImplType>::operator*=(const Size& inSize)
{
    mImpl *= inSize.mImpl;
    return *this;
}

template<typename T, typename ImplType>
inline constexpr Size<T, ImplType>& Size<T, ImplType>::operator/=(const Size& inSize)
{
    mImpl /= inSize.mImpl;
    return *this;
}

template<typename T, typename ImplType>
inline constexpr bool Size<T, ImplType>::IsEqual(const Size& inSize) const
{
    auto result = mImpl.IsEqual(inSize.mImpl);
    return result;
}

template<typename T, typename ImplType>
inline constexpr void Size<T, ImplType>::RoundNearest()
{
    mImpl.RoundNearest();
}

template<typename T, typename ImplType>
inline constexpr void Size<T, ImplType>::RoundFloor()
{

}

template<typename T, typename ImplType>
inline constexpr void Size<T, ImplType>::RoundCeil()
{

}

template<typename T, typename ImplType>
inline constexpr void Size<T, ImplType>::RoundZero()
{

}

#pragma endregion

// ------------------------------------------------------------------
#pragma region Free Functions

template<typename T, typename ImplType>
inline constexpr Size<T, ImplType> Enlarge(const Size<T, ImplType>& inSize, const Size<T, ImplType>& inMagnitude)
{
    Size<T, ImplType> result{inSize};
    result += inMagnitude;
    return result;
}

template<typename T, typename ImplType>
inline constexpr Size<T, ImplType> Enlarge(const Size<T, ImplType>& inSize, T inX, T inY)
{
    const Size<T, ImplType> magnitude{inX, inY};
    return Enlarge(inSize, magnitude);
}

template<typename T, typename ImplType>
inline constexpr Size<T, ImplType> Enlarge(const Size<T, ImplType>& inSize, T inMagnitude)
{
    return Enlarge(inSize, inMagnitude, inMagnitude);
}

template<typename T, typename ImplType>
inline constexpr Size<T, ImplType> Scale(const Size<T, ImplType>& inSize, const Size<T, ImplType>& inScale)
{
    Size<T, ImplType> result{inSize};
    result *= inScale;
    return result;
}

template<typename T, typename ImplType>
inline constexpr Size<T, ImplType> Scale(const Size<T, ImplType>& inSize, T inScaleX, T inScaleY)
{
    const Size<T, ImplType> scale{inScaleX, inScaleY};
    return Scale(inSize, scale);
}

template<typename T, typename ImplType>
inline constexpr Size<T, ImplType> Scale(const Size<T, ImplType>& inSize, T inScale)
{
    return Scale(inSize, inScale, inScale);
}

/// @brief Round to nearest integer value. Halfway cases round away from zero.
/// @tparam T: Underlying type
/// @tparam ImplType: Underlying implementation type
/// @param inSize: Size object to be rounded
/// @return Rounded result Size
template<typename T, typename ImplType>
inline constexpr Size<T, ImplType> RoundNearest(const Size<T, ImplType>& inSize)
{
    auto result{inSize};
    result.RoundNearest();
    return result;
}

/// @brief Round towards zero to the nearest integer value.
/// @tparam T: Underlying type
/// @tparam ImplType: Underlying implementation type
/// @param inSize: Size object to be rounded
/// @return Rounded result Size
template<typename T, typename ImplType>
inline constexpr Size<T, ImplType> RoundTrunc(const Size<T, ImplType>& inSize)
{
    auto result{inSize};
    //result.RoundTrunc();
    return result;
}

/// @brief Round towards +infinity to the nearest integer value.
/// @tparam T: Underlying type
/// @tparam ImplType: Underlying implementation type
/// @param inSize: Size object to be rounded
/// @return Rounded result Size
template<typename T, typename ImplType>
inline constexpr Size<T, ImplType> RoundCeil(const Size<T, ImplType>& inSize)
{
    auto result{inSize};
    //result.RoundCeil();
    return result;
}

/// @brief Round towards -infinity to the nearest integer value.
/// @tparam T: Underlying type
/// @tparam ImplType: Underlying implementation type
/// @param inSize: Size object to be rounded
/// @return Rounded result Size
template<typename T, typename ImplType>
inline constexpr Size<T, ImplType> RoundFloor(const Size<T, ImplType>& inSize)
{
    auto result{inSize};
    //result.RoundFloor();
    return result;
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