#ifndef SABER_GEOMETRY_POINT_HPP
#define SABER_GEOMETRY_POINT_HPP
#pragma once

// saber
#include "saber/geometry/operators.hpp"

namespace saber::geometry {

// REVIEW mnfitz 15jun2024:
// design issues involved with developing Point class
// 1. Appropriate namespace
// 2. Separate function declaration and definition vs java style combined
// 3. Has-a: naked types or std::tuple or std::array

template<typename T>
class Point
{
public: 
    constexpr Point(T inX, T inY);

    constexpr Point() = default;
    ~Point() = default;

    // RO5 is all default implemented
    constexpr Point(Point&& ioMove) noexcept = default;
    constexpr Point& operator=(Point&& ioMove) noexcept = default;

    constexpr Point(const Point& inCopy) = default;
    constexpr Point& operator=(const Point& inCopy) = default;

    constexpr T X() const;
    constexpr T Y() const;

    // Mathematical operations
    constexpr Point& operator+=(const Point& inPoint);
    constexpr Point& operator-=(const Point& inPoint);
    constexpr Point& operator*=(const Point& inPoint);
    constexpr Point& operator/=(const Point& inPoint);

    // REVISIT mnfitz 15jun2024:
    // Figure out operators supporting scalar operations

private:
    T mX{};
    T mY{};
}; // class point

// Inline Class Functions

template<typename T>
inline constexpr Point<T>::Point(T inX, T inY) :
    mX{inX},
    mY{inY}
{
    // Do nothing
}

template<typename T>
inline constexpr T Point<T>::X() const
{
    return mX;
}

template<typename T>
inline constexpr T Point<T>::Y() const
{
    return mY;
}

// Mathematical operations

template<typename T>
inline constexpr Point<T>& Point<T>::operator+=(const Point& inPoint)
{
    mX += inPoint.mX;
    mY += inPoint.mY;
    return *this;
}

template<typename T>
inline constexpr Point<T>& Point<T>::operator-=(const Point& inPoint)
{
    mX -= inPoint.mX;
    mY -= inPoint.mY;
    return *this;
}

template<typename T>
inline constexpr Point<T>& Point<T>::operator*=(const Point& inPoint)
{
    mX *= inPoint.mX;
    mY *= inPoint.mY;
    return *this;
}

template<typename T>
inline constexpr Point<T>& Point<T>::operator/=(const Point& inPoint)
{
    mX /= inPoint.mX;
    mY /= inPoint.mY;
    return *this;
}

}// namespace saber::geometry

#endif // SABER_GEOMETRY_POINT_HPP