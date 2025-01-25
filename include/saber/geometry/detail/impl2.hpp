#ifndef SABER_GEOMETRY_DETAIL_IMPL2_HPP
#define SABER_GEOMETRY_DETAIL_IMPL2_HPP

// std
#include <array>
#include <tuple>
#include <type_traits>

// saber
#include "saber/geometry/detail/simd.hpp"

namespace saber::geometry::detail {

template<typename T>
struct Impl2
{
    class Scalar
    {
    public:
        // default ctor
        constexpr Scalar() = default;
        ~Scalar() = default;

        // alt ctor
        constexpr Scalar(T inFirst, T inSecond):
            mTuple(inFirst, inSecond)
        {
            
        }

        template<std::size_t Index>
        constexpr T Get() const
        {
            static_assert(Index < std::tuple_size_v<decltype(mTuple)>, "Provided index out of bounds.");
            return std::get<Index>(mTuple);
        }

        constexpr Scalar& operator+=(const Scalar& inRHS)
        {
            std::get<0>(mTuple) += std::get<0>(inRHS.mTuple);
            std::get<1>(mTuple) += std::get<1>(inRHS.mTuple);
            return *this;
        }

        constexpr Scalar& operator-=(const Scalar& inRHS)
        {
            std::get<0>(mTuple) -= inRHS.Get<0>();
            std::get<1>(mTuple) -= inRHS.Get<1>();
            return *this;
        }

        constexpr Scalar& operator*=(const Scalar& inRHS)
        {
            std::get<0>(mTuple) *= inRHS.Get<0>();
            std::get<1>(mTuple) *= inRHS.Get<1>();
            return *this;
        }

        constexpr Scalar& operator/=(const Scalar& inRHS)
        {
            std::get<0>(mTuple) /= inRHS.Get<0>();
            std::get<1>(mTuple) /= inRHS.Get<1>();
            return *this;
        }

        constexpr bool IsEqual(const Scalar& inRHS) const
        {
            // Point said to copy/paste this section into the scalar portion
            bool result = false;
            if constexpr (std::is_floating_point_v<T>)
            {   
                // Floating point comparisons are always inexact within an epsilon
                result = Inexact::IsEq(Get<0>(), inRHS.Get<0>()) && Inexact::IsEq(Get<1>(), inRHS.Get<1>());
            }
            else
            {
                // Integer comparisons are always exact
                result = (Get<0>() == inRHS.Get<0>()) && (Get<1>() == inRHS.Get<1>());
            }
            return result;
        }

    private:
        std::tuple<T,T> mTuple{}; // Impl2: so 2 elements are assumed
    };

    class Simd
    {
    public:
        // default ctor
        constexpr Simd() = default;
        ~Simd() = default;

        constexpr Simd(T inFirst, T inSecond):
            mArray{{inFirst, inSecond}} // TRICKY mnfitz 19oct2024: std::array takes a single parameter of a std::initializer_list
        {

        }

        template<std::size_t Index>
        constexpr T Get() const
        {
            static_assert(Index < std::tuple_size_v<decltype(mArray)>, "Provided index out of bounds.");
            return mArray[Index];
        }

        constexpr Simd& operator+=(const Simd& inRHS)
        {
            // NOTE: SIMD intrinsic functions lack a constexpr implementation; contaminating our interface
            // Use std::is_constant_evaluated() to protect our interface so it can remain constexpr
            // std::is_constant_evaulated() is only available in c++20 or later
            // Only in c++ 20 or later, can operator+= deliver a compile-time result
            do 
            {
#if __cpp_lib_is_constant_evaluated
                if (std::is_constant_evaluated())
                {
                    // Delegate to Scalar Impl which is constexpr capable
                    Scalar lhs{Get<0>(), Get<1>()};
                    const Scalar rhs{inRHS.Get<0>(), inRHS.Get<1>()};
                    lhs += rhs;
                    mArray[0] = lhs.Get<0>();
                    mArray[1] = lhs.Get<1>(); 
                    break;
                }
#endif // __cpp_lib_is_constant_evaluated

                auto lhs = Simd128<T>::Load2(&mArray[0]);
                auto rhs = Simd128<T>::Load2(&inRHS.mArray[0]);
                auto result = Simd128<T>::Add(lhs, rhs);
                Simd128<T>::Store2(&mArray[0], result);
            } while (false);

            return *this;
        }

        constexpr Simd& operator-=(const Simd& inRHS)
        {
            // protect our interface so it can remain constexpr
            do 
            {
#if __cpp_lib_is_constant_evaluated
                if (std::is_constant_evaluated())
                {
                    // Delegate to Scalar Impl which is constexpr capable
                    Scalar lhs{Get<0>(), Get<1>()};
                    const Scalar rhs{inRHS.Get<0>(), inRHS.Get<1>()};
                    lhs -= rhs;
                    mArray[0] = lhs.Get<0>();
                    mArray[1] = lhs.Get<1>(); 
                    break;
                }
#endif // __cpp_lib_is_constant_evaluated

                auto lhs = Simd128<T>::Load2(&mArray[0]);
                auto rhs = Simd128<T>::Load2(&inRHS.mArray[0]);
                auto result = Simd128<T>::Sub(lhs, rhs);
                Simd128<T>::Store2(&mArray[0], result);
            } while (false);

            return *this;
        }

        constexpr Simd& operator*=(const Simd& inRHS)
        {
            // protect our interface so it can remain constexpr
            do 
            {
#if __cpp_lib_is_constant_evaluated
                if (std::is_constant_evaluated())
                {
                    // Delegate to Scalar Impl which is constexpr capable
                    Scalar lhs{Get<0>(), Get<1>()};
                    const Scalar rhs{inRHS.Get<0>(), inRHS.Get<1>()};
                    lhs *= rhs;
                    mArray[0] = lhs.Get<0>();
                    mArray[1] = lhs.Get<1>(); 
                    break;
                }
#endif // __cpp_lib_is_constant_evaluated

                auto lhs = Simd128<T>::Load2(&mArray[0]);
                auto rhs = Simd128<T>::Load2(&inRHS.mArray[0]);
                auto result = Simd128<T>::Mul(lhs, rhs);
                Simd128<T>::Store2(&mArray[0], result);
            } while (false);

            return *this;
        }

        constexpr Simd& operator/=(const Simd& inRHS)
        {
            // protect our interface so it can remain constexpr
            do 
            {
#if __cpp_lib_is_constant_evaluated
                if (std::is_constant_evaluated())
                {
                    // Delegate to Scalar Impl which is constexpr capable
                    Scalar lhs{Get<0>(), Get<1>()};
                    const Scalar rhs{inRHS.Get<0>(), inRHS.Get<1>()};
                    lhs /= rhs;
                    mArray[0] = lhs.Get<0>();
                    mArray[1] = lhs.Get<1>(); 
                    break;
                }
#endif // __cpp_lib_is_constant_evaluated

                auto lhs = Simd128<T>::Load2(&mArray[0]);
                auto rhs = Simd128<T>::Load2(&inRHS.mArray[0]);
                auto result = Simd128<T>::Div(lhs, rhs);
                Simd128<T>::Store2(&mArray[0], result);
            } while (false);

            return *this;
        }

        constexpr bool IsEqual(const Simd& inRHS) const
        {
            bool result = false;
            // protect our interface so it can remain constexpr
            do 
            {
        #if __cpp_lib_is_constant_evaluated
                if (std::is_constant_evaluated())
                {
                    // Delegate to Scalar Impl which is constexpr capable
                    Scalar lhs{Get<0>(), Get<1>()};
                    const Scalar rhs{inRHS.Get<0>(), inRHS.Get<1>()};
                    result = lhs == rhs;
                    break;
                }
        #endif // __cpp_lib_is_constant_evaluated

                auto lhs = Simd128<T>::Load2(&mArray[0]);
                auto rhs = Simd128<T>::Load2(&inRHS.mArray[0]);
                result = Simd128<T>::IsEQ(lhs, rhs);
            } while (false);

            return result;
        }

    private:
        std::array<T,2> mArray{}; // Impl2: so 2 elements are assumed
    }; // class Simd

}; // struct Impl2<>

} // namespace saber::geometry::detail

#endif // SABER_GEOMETRY_DETAIL_IMPL2_HPP