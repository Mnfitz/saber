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

template<typename T>
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

    // REVISIT mnfitz 15jun2024:
    // Figure out operators supporting scalar operations

private:
    T mWidth{};
    T mHeight{};
}; // class size

// Inline Class Functions

template<typename T>
inline constexpr Size<T>::Size(T inWidth, T inHeight) :
    mWidth{inWidth},
    mHeight{inHeight}
{
    // Do nothing
}

template<typename T>
inline constexpr T Size<T>::Width() const
{
    return mWidth;
}

template<typename T>
inline constexpr T Size<T>::Height() const
{
    return mHeight;
}

// Mathematical operations

template<typename T>
inline constexpr Size<T>& Size<T>::operator+=(const Size& inSize)
{
    mWidth += inSize.mWidth;
    mHeight += inSize.mHeight;
    return *this;
}

template<typename T>
inline constexpr Size<T>& Size<T>::operator-=(const Size& inSize)
{
    mWidth -= inSize.mWidth;
    mHeight -= inSize.mHeight;
    return *this;
}

template<typename T>
inline constexpr Size<T>& Size<T>::operator*=(const Size& inSize)
{
    mWidth *= inSize.mWidth;
    mHeight *= inSize.mHeight;
    return *this;
}

template<typename T>
inline constexpr Size<T>& Size<T>::operator/=(const Size& inSize)
{
    mWidth /= inSize.mWidth;
    mHeight /= inSize.mHeight;
    return *this;
}

}// namespace saber::geometry

#endif // SABER_GEOMETRY_SIZE_HPP