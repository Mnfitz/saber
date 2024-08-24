#ifndef SABER_GEOMETRY_COMPARATOR_HPP
#define SABER_GEOMETRY_COMPARATOR_HPP
#pragma once

// std
#include <algorithm>
#include <cmath>
#include <limits>

namespace saber {
namespace geometry {
namespace detail {

inline bool CompareInexact(float inLHS, float inRHS)
{
    // magnitude: the further we get away from 0, the more inexactness we allow
    const float magnitude = std::max(std::max(std::abs(inLHS), std::abs(inRHS)), 1.0f); 
    // difference: compare the 2 numbers
    const float difference = std::abs(inLHS - inRHS);
    // epsilon: minimal permitted amount of inexactness
    const float epsilon = std::numeric_limits<float>::epsilon() * magnitude;
    // isEqual: equality occurs if the difference is within a scaled epsilon
    const bool isEqual = difference <= epsilon; // some arbirtary small number (Note: Intergral epsilon is 0)
    return isEqual;
}

inline bool CompareInexact(double inLHS, double inRHS)
{
    // magnitude: the further we get away from 0, the more inexactness we allow
    const double magnitude = std::max(std::max(std::abs(inLHS), std::abs(inRHS)), 1.0); 
    // difference: compare the 2 numbers
    const double difference = std::abs(inLHS - inRHS);
    // epsilon: minimal permitted amount of inexactness
    const double epsilon = std::numeric_limits<double>::epsilon() * magnitude;
    // isEqual: equality occurs if the difference is within a scaled epsilon
    const bool isEqual = difference <= epsilon; // some arbirtary small number
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
    // Const reference because you want to guarantee to 
    // the caller that his value isn't going to be modified
    // Note: a reference member disqualifies this class from Ro5 move construct, move assign
    const T& mLHS{}; 
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
    // Non-const, because it's our own value; copied from the caller and 
    // const data members prevent Ro5 move construct and move assign
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
    // Non-const, because it's our own value; copied from the caller and 
    // const data members prevent Ro5 move construct and move assign
    double mLHS{};
};
    
} // namespace geometry
} // namespace saber

#endif // SABER_GEOMETRY_COMPARATOR_HPP