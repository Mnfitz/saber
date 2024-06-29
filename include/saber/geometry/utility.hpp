#ifndef SABER_GEOMETRY_UTILITY_HPP
#define SABER_GEOMETRY_UTILITY_HPP
#pragma once

// saber
#include "saber/utility.hpp"
#include "saber/geometry/point.hpp"
#include "saber/geometry/size.hpp"

namespace saber {
namespace detail {

// Converts from Point to Size
template<typename T>
struct ConvertTraits<geometry::Size<T>, geometry::Point<T>>
{
    geometry::Size<T> operator()(const geometry::Point<T>& inValue) const
    {
        // C++17 is smart enough to convert assignment to in-place constructor
        geometry::Size<T> size{inValue.X(), inValue.Y()};
        return size;
    }
};

// Converts from Size to Point
template<typename T>
struct ConvertTraits<geometry::Point<T>, geometry::Size<T>>
{
    geometry::Point<T> operator()(const geometry::Size<T>& inValue) const
    {
        // C++17 is smart enough to convert assignment to in-place constructor
        geometry::Point<T> point{inValue.Width(), inValue.Height()};
        return point;
    }
};

} // namespace detail
} // namespace saber

#endif