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
        // REVISIT mnfitz29jun2024: Which is better, C++17 "assignment" from initializer list or using "constructor{}"
        // Look at the release builds and check the compiler generated instructions to make the determination
        geometry::Size<T> size = {inValue.X(), inValue.Y()};
        return size;
    }
};

// Converts from Size to Point
template<typename T>
struct ConvertTraits<geometry::Point<T>, geometry::Size<T>>
{
    geometry::Point<T> operator()(const geometry::Size<T>& inValue) const
    {
        geometry::Point<T> point = {inValue.Width(), inValue.Height()};
        return point;
    }
};

} // namespace detail
} // namespace saber

#endif