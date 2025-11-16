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

// REVISIT: Point<T, typename T::ImplKind>
// Converts from Point to Size
template<typename T>
struct ConvertTraits<geometry::Size<T>, geometry::Point<T>>
{
    geometry::Size<T> operator()(const geometry::Point<T>& inPoint) const
    {
        // C++17 is smart enough to convert assignment to in-place constructor
        geometry::Size<T> size{inPoint.X(), inPoint.Y()};
        return size;
    }
};

// Converts from Size to Point
template<typename T>
struct ConvertTraits<geometry::Point<T>, geometry::Size<T>>
{
    geometry::Point<T> operator()(const geometry::Size<T>& inSize) const
    {
        // C++17 is smart enough to convert assignment to in-place constructor
        geometry::Point<T> point{inSize.Width(), inSize.Height()};
        return point;
    }
};

} // namespace saber

#endif // SABER_GEOMETRY_UTILITY_HPP