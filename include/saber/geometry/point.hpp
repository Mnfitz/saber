#ifndef SABER_GEOMETRY_POINT_HPP
#define SABER_GEOMETRY_POINT_HPP
#pragma once

// std
#include <vector>

namespace saber::geometry {

template<typename T>
class Point
{
public: 
    Point(T inX, T inY) :
        mX{inX},
        mY{inY}
    {
        // Do nothing
    }

    Point() = default;

    ~Point() = default;

    // RO5 is all default implemented
    Point(Point&& ioMove) = default;

    Point& operator=(Point&& ioMove) noexcept = default;

    Point(const Point& inCopy) = default;

    Point& operator=(const Point& inCopy) = default;

    T X() const
    {
        return mX;
    }

    T Y() const
    {
        return mY;
    }

    // Mathematical operations
    Point& operator+=(const Point& inPoint)
    {
        mX += inPoint.mX;
        mY += inPoint.mY;
        return this;
    }

    Point& operator-=(const Point& inPoint)
    {
        mX -= inPoint.mX;
        mY -= inPoint.mY;
        return this;
    }

    Point& operator*(const T inScalar)
    {
        mX = inScalar * mX;
        mY = inScalar * mY;
        return this;
    }

    Point& operator/(const T inDenominator)
    {
        mX = mX / inDenominator;
        mY = mY/ inDenominator;
        return this;
    }

private:
T mX{};
T mY{};
}; // class point

template<typename T>
Point<T> operator+(const Point<T>& inLHS, const Point<T>& inRHS)
{
    Point<T> outPoint = inLHS;
    outPoint += inRHS;
    return outPoint;
}

template<typename T>
Point<T> operator-(const Point<T>& inLHS, const Point<T>& inRHS)
{
    Point<T> outPoint = inLHS;
    outPoint -= inRHS;
    return outPoint;
}

}// namespace saber::geometry

#endif // SABER_GEOMETRY_POINT_HPP