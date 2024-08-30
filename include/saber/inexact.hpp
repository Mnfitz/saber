#ifndef SABER_INEXACT_HPP
#define SABER_INEXACT_HPP
#pragma once

// std
#include <algorithm>
#include <cmath>
#include <limits>
#include <cassert>
#include <type_traits>

namespace saber {

class Inexact
{
public:
    template<typename T>
    static bool IsEQ(T inLHS, T inRHS)
    {
        EQ equal{inLHS};
        const bool result = equal(inRHS);
        return result;
    }

    /*
    template<typename T>
    static bool IsNE(T inLHS, T inRHS)
    {
        EQ unequal{inLHS};
        const bool result = unequal(inRHS);
        return result;
    }
    */

    template<typename T>
    struct EQ
    {
    public:
        EQ(const T& inLHS) :
            mLHS{inLHS}
        {
            bool assertion = std::is_floating_point<T>::value;
            assert(assertion);
            // Do nothing
        };

        bool operator()(const T& inRHS) const
        {
            // magnitude: the further we get away from 0, the more inexactness we allow
            const T magnitude = std::max<T>(std::max<T>(std::abs(mLHS), std::abs(inRHS)), 1.0f); 
            // difference: compare the 2 numbers
            const T difference = std::abs(mLHS - inRHS);
            // epsilon: minimal permitted amount of inexactness
            const T epsilon = std::numeric_limits<T>::epsilon() * magnitude;
            // isEqual: equality occurs if the difference is within a scaled epsilon
            const bool isEqual = difference <= epsilon; // some arbirtary small number (Note: Intergral epsilon is 0)
            return isEqual;
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