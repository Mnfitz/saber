#ifndef SABER_GEOMETRY_COMPARATOR_HPP
#define SABER_GEOMETRY_COMPARATOR_HPP
#pragma once

// saber
#include "saber/utility.hpp"
#include "saber/geometry/point.hpp"
#include "saber/geometry/size.hpp"

// std
#include <algorithm>
#include <cmath>
#include <limits>

namespace saber {
namespace geometry {
namespace detail {

bool CompareInexact(float inLHS, float inRHS)
{
    // magnitude: the further we get away from 0, the more inexactness we allow
    const float magnitude = std::max(std::max(std::abs(inLHS), std::abs(inRHS)), 1.0f); 
    // difference: compare the 2 numbers
    const float difference = std::abs(inLHS - inRHS);
    // epsilon: minimal permitted amount of inexactness
    const float epsilon = std::numeric_limits<float>::epsilon() * magnitude;
    // isEqual: equality occurs if the difference is within a scaled epsilon
    const bool isEqual = difference < epsilon; // some arbirtary small number
    return isEqual;
}

bool CompareInexact(double inLHS, double inRHS)
{
    // magnitude: the further we get away from 0, the more inexactness we allow
    const double magnitude = std::max(std::max(std::abs(inLHS), std::abs(inRHS)), 1.0); 
    // difference: compare the 2 numbers
    const double difference = std::abs(inLHS - inRHS);
    // epsilon: minimal permitted amount of inexactness
    const double epsilon = std::numeric_limits<double>::epsilon() * magnitude;
    // isEqual: equality occurs if the difference is within a scaled epsilon
    const bool isEqual = difference < epsilon; // some arbirtary small number
    return isEqual;
}

} // namespace detail

// Primary template
template<typename T>
struct Comparator
{
public:
    Comparator(const T& inLHS) :
        mLHS{inLHS}
    {
        // Do nothing
    };

    bool operator()(const T& inRHS) const
    {
        bool result = mLHS == inRHS;
        return result;
    }

private:
    T mLHS{};
};

// float specialization
template<>
struct Comparator<float>
{
public:
    //ctor
    Comparator(float inLHS) :
        mLHS{inLHS}
    {
        // Do nothing
    };

    bool operator()(float inRHS) const
    {
        bool result = detail::CompareInexact(mLHS, inRHS);
        return result;
    }

private:
    float mLHS{};
};

// double specialization
template<>
struct Comparator<double>
{
public:
    //ctor
    Comparator(double inLHS) :
        mLHS{inLHS}
    {
        // Do nothing
    };

    bool operator()(double inRHS) const
    {
        bool result = detail::CompareInexact(mLHS, inRHS);
        return result;
    }

private:
    double mLHS{};
};
    
} // namespace geometry
} // namespace saber

#endif // SABER_GEOMETRY_COMPARATOR_HPP