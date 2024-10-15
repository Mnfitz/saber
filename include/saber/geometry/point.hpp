#ifndef SABER_GEOMETRY_POINT_HPP
#define SABER_GEOMETRY_POINT_HPP
#pragma once

// saber
#include "saber/inexact.hpp"
#include "saber/geometry/operators.hpp"

// std
#include <utility>

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

private:
    constexpr bool IsEqual(const Point& inPoint) const;
    friend constexpr bool operator==<Point>(const Point& inLHS, const Point& inRHS);
    friend constexpr bool operator!=<Point>(const Point& inLHS, const Point& inRHS);

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

template<typename T>
inline constexpr bool Point<T>::IsEqual(const Point& inPoint) const
{
    bool result = false;
    if constexpr (std::is_floating_point_v<T>)
    {
        result = Inexact::IsEq(X(), inPoint.X()) && Inexact::IsEq(Y(), inPoint.Y());
    }
    else
    {
        result = (X() == inPoint.X()) && (Y() == inPoint.Y());
    }
    return result;
}

// TRICKY mnfitz 14oct2024: Turn on structured binging support for C++17 or later
#ifdef __cpp_structured_bindings

// Structured Binding Support 

// Prefer free function over class method
template<std::size_t Index, typename T>
inline T get(const Point<T>& inPoint)
{
    static_assert(2 == std::tuple_size<Point<T>>::value); // Use own std::tuple_size<> specialization
    static_assert(Index < 2, "Unexpected Index for Point<T>");

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
struct std::tuple_size<Point<T>> // Partial template specialization for: Point<T>
{
    // Number of elements in Point<T>'s structured binding
    static constexpr std::size_t value = 2;
};

template<std::size_t Index, typename T>
struct std::tuple_element<Index, Point<T>> // Partial template specialization for: Point<T>
{
    // Type of elements in Point<T>'s structured
    using type = T;
};

#endif //__cpp_structured_bindings

}// namespace saber::geometry

#endif // SABER_GEOMETRY_POINT_HPP