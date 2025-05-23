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
#ifndef SABER_INEXACT_HPP
#define SABER_INEXACT_HPP

// saber
#include "saber/config.hpp"

// std
#include <cmath>
#include <cstdlib>
#include <limits>
#include <type_traits>
#include <utility>

namespace saber {

class Inexact
{
public:
    template<typename T>
    static bool IsEq(T inLHS, T inRHS)
    {
        Eq isEqual{inLHS};
        const bool result = isEqual(inRHS);
        return result;
    }
    
    template<typename T>
    static bool IsNe(T inLHS, T inRHS)
    {
        Ne isNotEqual{inLHS};
        const bool result = isNotEqual(inRHS);
        return result;
    }
    
    template<typename T>
    struct Eq
    {
    public:
        Eq(const T& inLHS) :
            mLHS{inLHS}
        {
            constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
            static_assert(isFloatingPoint, "Eq only supports floating point types");
        };

        bool operator()(const T& inRHS) const
        {
            // magnitude: the further we get away from 0, the more inexactness we allow
            const T magnitude = std::max<T>(std::max<T>(std::abs(mLHS), std::abs(inRHS)), 1.0f); 
            // difference: compare the 2 numbers
            const T difference = std::abs(mLHS - inRHS);
            // epsilon: minimal permitted amount of inexactness
            const T epsilon = std::numeric_limits<T>::epsilon() * magnitude;
            // IsEqual: equality occurs if the difference is within a scaled epsilon
            const bool IsEqual = difference <= epsilon; // some arbirtary small number (Note: Intergral epsilon is 0)
            return IsEqual;
        }

    private:
        // Const reference because you want to guarantee to 
        // the caller that his value isn't going to be modified
        // Note: a reference member disqualifies this class from Ro5 move construct, move assign
        const T& mLHS{}; 
    };

    template<typename T>
    struct Ne
    {
    public:
        Ne(const T& inLHS) :
            mLHS{inLHS}
        {
            constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
            static_assert(isFloatingPoint, "Ne only supports floating point types");
        };

        bool operator()(const T& inRHS) const
        {
            Eq isEqual{mLHS};
            const bool isNotEqual = !isEqual(inRHS);
            return isNotEqual;
        }

    private:
        // Const reference because you want to guarantee to 
        // the caller that his value isn't going to be modified
        // Note: a reference member disqualifies this class from Ro5 move construct, move assign
        const T& mLHS{}; 
    };

};

} // namespace saber

#endif // SABER_INEXACT_HPP