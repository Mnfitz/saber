/////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2025 Matthew Fitzgerald
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef SABER_GEOMETRY_UTILITY_HPP
#define SABER_GEOMETRY_UTILITY_HPP

// saber
#include "saber/config.hpp"
#include "saber/utility.hpp"
#include "saber/geometry/point.hpp"
#include "saber/geometry/size.hpp"

namespace saber {

#pragma region ConvertTraits for Geometry Types

// ConvertTo expects all its ConvertTraits to reside in saber namespace level

// ImplKind Simd
// Converts from Point to Size
template<typename T>
struct ConvertTraits<geometry::Size<T, geometry::ImplKind::kSimd>, geometry::Point<T, geometry::ImplKind::kSimd>>
{
    using Point = geometry::Point<T, geometry::ImplKind::kSimd>;
    using Size = geometry::Size<T, geometry::ImplKind::kSimd>;

    Size operator()(const Point& inPoint) const
    {
        // C++17 is smart enough to convert assignment to in-place constructor
        Size size{inPoint.X(), inPoint.Y()};
        return size;
    }
};

// Converts from Size to Point
template<typename T>
struct ConvertTraits<geometry::Point<T, geometry::ImplKind::kSimd>, geometry::Size<T, geometry::ImplKind::kSimd>>
{
    using Point = geometry::Point<T, geometry::ImplKind::kSimd>;
    using Size = geometry::Size<T, geometry::ImplKind::kSimd>;

    Point operator()(const Size& inSize) const
    {
        // C++17 is smart enough to convert assignment to in-place constructor
        Point point{inSize.Width(), inSize.Height()};
        return point;
    }
};

// ImplKind Scalar
// Converts from Point to Size
template<typename T>
struct ConvertTraits<geometry::Size<T, geometry::ImplKind::kScalar>, geometry::Point<T, geometry::ImplKind::kScalar>>
{
    using Point = geometry::Point<T, geometry::ImplKind::kScalar>;
    using Size = geometry::Size<T, geometry::ImplKind::kScalar>;

    Size operator()(const Point& inPoint) const
    {
        // C++17 is smart enough to convert assignment to in-place constructor
        Size size{inPoint.X(), inPoint.Y()};
        return size;
    }
};

// Converts from Size to Point
template<typename T>
struct ConvertTraits<geometry::Point<T, geometry::ImplKind::kScalar>, geometry::Size<T, geometry::ImplKind::kScalar>>
{
    using Point = geometry::Point<T, geometry::ImplKind::kScalar>;
    using Size = geometry::Size<T, geometry::ImplKind::kScalar>;

    Point operator()(const Size& inSize) const
    {
        // C++17 is smart enough to convert assignment to in-place constructor
        Point point{inSize.Width(), inSize.Height()};
        return point;
    }
};

#pragma endregion

namespace geometry {
// geometry specific utilities go here
} // namespace geometry
} // namespace saber

#endif // SABER_GEOMETRY_UTILITY_HPP