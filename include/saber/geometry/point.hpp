#ifndef SABER_GEOMETRY_POINT_HPP
#define SABER_GEOMETRY_POINT_HPP
#pragma once

// saber
#include "saber/inexact.hpp"
#include "saber/geometry/operators.hpp"
#include "saber/geometry/detail/impl2.hpp" 

// std
#include <utility>

namespace saber::geometry {

// REVIEW mnfitz 15jun2024:
// design issues involved with developing Point class
// 1. Appropriate namespace
// 2. Separate function declaration and definition vs java style combined
// 3. Has-a: naked types or std::tuple or std::array

// template<typename T>
// using ImplType = typename detail::Impl2<T>::Scalar;

// Point<float> myFloat{5,7};
template<typename T, typename ImplType = typename detail::Impl2<T>::Simd> // TRICKY mnfitz 19oct2024: nested type in a template class needs 'typename' prefix
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

// Private APIs
private:
    constexpr bool IsEqual(const Point& inPoint) const;

// Friend functions
private:
    friend constexpr bool operator==<Point>(const Point& inLHS, const Point& inRHS);
    friend constexpr bool operator!=<Point>(const Point& inLHS, const Point& inRHS);

    // REVISIT mnfitz 15jun2024:
    // Figure out operators supporting scalar operations

private:
    ImplType mImpl{};
}; // class Point<>

// Inline Class Methods

template<typename T, typename ImplType>
inline constexpr Point<T, ImplType>::Point(T inX, T inY) :
    mImpl{inX, inY}
{
    // Do nothing
}

template<typename T, typename ImplType>
inline constexpr T Point<T, ImplType>::X() const
{
    return mImpl.Get<0>();
}

template<typename T, typename ImplType>
inline constexpr T Point<T, ImplType>::Y() const
{
    return mImpl.Get<1>();
}

// Mathematical operations

template<typename T, typename ImplType>
inline constexpr Point<T, ImplType>& Point<T, ImplType>::operator+=(const Point& inPoint)
{
    mImpl += inPoint.mImpl;
    return *this;
}

template<typename T, typename ImplType>
inline constexpr Point<T, ImplType>& Point<T, ImplType>::operator-=(const Point& inPoint)
{
    mImpl -= inPoint.mImpl;
    return *this;
}

template<typename T, typename ImplType>
inline constexpr Point<T, ImplType>& Point<T, ImplType>::operator*=(const Point& inPoint)
{
    mImpl *= inPoint.mImpl;
    return *this;
}

template<typename T, typename ImplType>
inline constexpr Point<T, ImplType>& Point<T, ImplType>::operator/=(const Point& inPoint)
{
    mImpl /= inPoint.mImpl;
    return *this;
}

template<typename T, typename ImplType>
inline constexpr bool Point<T, ImplType>::IsEqual(const Point& inPoint) const
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

// Free functions 
template<typename T, typename ImplType>
inline constexpr Point<T, ImplType> Translate(const Point<T, ImplType>& inPoint, const Point<T, ImplType>& inTranslate)
{
    Point<T, ImplType> result{inPoint};
    result += inTranslate;
    return result;
}

template<typename T, typename ImplType>
inline constexpr Point<T, ImplType> Translate(const Point<T, ImplType>& inPoint, T inX, T inY)
{
    Point<T, ImplType> translate{inX, inY};
    return Translate(inPoint, translate);
}

template<typename T, typename ImplType>
inline constexpr Point<T, ImplType> Translate(const Point<T, ImplType>& inPoint, T inTranslate)
{
    return Translate(inPoint, inTranslate, inTranslate);
}

template<typename T, typename ImplType>
inline constexpr Point<T, ImplType> Scale(const Point<T, ImplType>& inPoint, const Point<T, ImplType>& inScale)
{
    Point<T, ImplType> result{inPoint};
    result += inScale;
    return result;
}

template<typename T, typename ImplType>
inline constexpr Point<T, ImplType> Scale(const Point<T, ImplType>& inPoint, T inScaleX, T inScaleY)
{
    Point<T, ImplType> scale{inScaleX, inScaleY};
    return Scale(inPoint, scale);
}

template<typename T, typename ImplType>
inline constexpr Point<T, ImplType> Scale(const Point<T, ImplType>& inPoint, T inScale)
{
    return Scale(inPoint, inScale, inScale);
}

// TRICKY mnfitz 14oct2024: Turn on structured binging support for C++17 or later
#ifdef __cpp_structured_bindings

// Structured Binding Support 

// Prefer free function over class method
template<std::size_t Index, typename T>
inline T get(const Point<T>& inPoint)
{
    static_assert(2 == std::tuple_size_v<Point<T>>); // Use own std::tuple_size<> specialization
    static_assert(Index < 2, "Unexpected Index for Point<T, ImplType>");

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
struct std::tuple_size<Point<T>> // Partial template specialization for: Point<T, ImplType>
{
    // Number of elements in Point<T, ImplType>'s structured binding
    static constexpr std::size_t value = 2;
};

template<std::size_t Index, typename T>
struct std::tuple_element<Index, Point<T>> // Partial template specialization for: Point<T, ImplType>
{
    // Type of elements in Point<T, ImplType>'s structured
    using type = T;
};

#endif //__cpp_structured_bindings

}// namespace saber::geometry

#endif // SABER_GEOMETRY_POINT_HPP