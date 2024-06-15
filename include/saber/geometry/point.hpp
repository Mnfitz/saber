#ifndef SABER_GEOMETRY_POINT_HPP
#define SABER_GEOMETRY_POINT_HPP
#pragma once

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
    Point(T inX, T inY);

    Point() = default;
    ~Point() = default;

    // RO5 is all default implemented
    Point(Point&& ioMove) noexcept = default;
    Point& operator=(Point&& ioMove) noexcept = default;

    Point(const Point& inCopy) = default;
    Point& operator=(const Point& inCopy) = default;

    T X() const;
    T Y() const;

    // Mathematical operations
    Point& operator+=(const Point& inPoint);
    Point& operator-=(const Point& inPoint);
    Point& operator*=(const Point& inPoint);
    Point& operator/=(const Point& inPoint);

    // REVISIT mnfitz 15jun2024:
    // Figure out operators supporting scalar operations

private:
    T mX{};
    T mY{};
}; // class point

// Inline Class Functions

template<typename T>
inline Point<T>::Point(T inX, T inY) :
    mX{inX},
    mY{inY}
{
    // Do nothing
}

template<typename T>
inline T Point<T>::X() const
{
    return mX;
}

template<typename T>
inline T Point<T>::Y() const
{
    return mY;
}

// Mathematical operations

template<typename T>
inline Point<T>& Point<T>::operator+=(const Point& inPoint)
{
    mX += inPoint.mX;
    mY += inPoint.mY;
    return this;
}

template<typename T>
inline Point<T>& Point<T>::operator-=(const Point& inPoint)
{
    mX -= inPoint.mX;
    mY -= inPoint.mY;
    return this;
}

template<typename T>
inline Point<T>& Point<T>::operator*=(const Point& inPoint)
{
    mX *= inPoint.mX;
    mY *= inPoint.mY;
    return this;
}

template<typename T>
inline Point<T>& Point<T>::operator/=(const Point& inPoint)
{
    mX = inPoint.mX;
    mY = inPoint.mY;
    return this;
}

// Inline Free Functions

/// @brief Binary Operator that adds 2 input points returning a result point
/// Use it like this: 
/// ```
/// auto resultPoint = point1 + point2;
/// ```
/// @tparam T underlying type for the `Point<T>` class
/// @param inLHS: Left hand side Point argument
/// @param inRHS: Right hand side Point argument 
/// @return point result
template<typename T>
inline Point<T> operator+(const Point<T>& inLHS, const Point<T>& inRHS)
{
    Point<T> outPoint = inLHS;
    outPoint += inRHS;
    return outPoint;
}

/// @brief Binary Operator that subtracts 2 input points returning a result point
/// Use it like this: 
/// ```
/// auto resultPoint = point1 - point2;
/// ```
/// @tparam T underlying type for the `Point<T>` class
/// @param inLHS: Left hand side Point argument
/// @param inRHS: Right hand side Point argument 
/// @return point result
template<typename T>
inline Point<T> operator-(const Point<T>& inLHS, const Point<T>& inRHS)
{
    Point<T> outPoint = inLHS;
    outPoint -= inRHS;
    return outPoint;
}

}// namespace saber::geometry

#endif // SABER_GEOMETRY_POINT_HPP