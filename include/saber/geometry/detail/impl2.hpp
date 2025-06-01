#ifndef SABER_GEOMETRY_DETAIL_IMPL2_HPP
#define SABER_GEOMETRY_DETAIL_IMPL2_HPP

// std
#include <array>
#include <tuple>
#include <type_traits>

// saber
#include "saber/inexact.hpp"
#include "saber/geometry/detail/simd.hpp"

namespace saber::geometry::detail {
    
template<typename T>
struct Impl2 final
{
    class Simd;

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
            std::get<0>(mTuple) -= std::get<0>(inRHS.mTuple);
            std::get<1>(mTuple) -= std::get<1>(inRHS.mTuple);
            return *this;
        }

        constexpr Scalar& operator*=(const Scalar& inRHS)
        {
            std::get<0>(mTuple) *= std::get<0>(inRHS.mTuple);
            std::get<1>(mTuple) *= std::get<1>(inRHS.mTuple);
            return *this;
        }

        constexpr Scalar& operator/=(const Scalar& inRHS)
        {
            std::get<0>(mTuple) /= std::get<0>(inRHS.mTuple);
            std::get<1>(mTuple) /= std::get<1>(inRHS.mTuple);
            return *this;
        }

        constexpr bool IsEqual(const Scalar& inRHS) const
        {
            // Point said to copy/paste this section into the scalar portion
            bool result = false;
            if constexpr (std::is_floating_point_v<T>)
            {   
                // Floating point comparisons are always inexact within an epsilon
                result = Inexact::IsEq(std::get<0>(mTuple), std::get<0>(inRHS.mTuple)) 
                    && Inexact::IsEq(std::get<1>(mTuple), std::get<1>(inRHS.mTuple));
            }
            else
            {
                // Integer comparisons are always exact
                result = (std::get<0>(mTuple) == std::get<0>(inRHS.mTuple)) && (std::get<1>(mTuple) == std::get<1>(inRHS.mTuple));
            }
            return result;
        }

        constexpr void RoundNearest()
        {
            constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
            static_assert(isFloatingPoint, "RoundNearest() only supports floating point types");
            
            std::get<0>(mTuple) = std::round(std::get<0>(mTuple));
            std::get<1>(mTuple) = std::round(std::get<1>(mTuple));
        }

        constexpr void RoundCeil()
        {
            constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
            static_assert(isFloatingPoint, "RoundCeil() only supports floating point types");
            
            std::get<0>(mTuple) = std::ceil(std::get<0>(mTuple));
            std::get<1>(mTuple) = std::ceil(std::get<1>(mTuple));
        }

        constexpr void RoundFloor()
        {
            constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
            static_assert(isFloatingPoint, "RoundFloor() only supports floating point types");
            
            std::get<0>(mTuple) = std::floor(std::get<0>(mTuple));
            std::get<1>(mTuple) = std::floor(std::get<1>(mTuple));
        }

        constexpr void RoundTrunc()
        {
            constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
            static_assert(isFloatingPoint, "RoundTrunc() only supports floating point types");
            
            std::get<0>(mTuple) = std::trunc(std::get<0>(mTuple));
            std::get<1>(mTuple) = std::trunc(std::get<1>(mTuple));
        }

    private:
        friend class Simd; // Permit Simd class to provide constexpr api

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

        constexpr auto GetSimdType() const
        {
            return Simd128<T>::Load2(&mArray[0]);
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
                    Scalar lhs{mArray[0], mArray[1]};
                    const Scalar rhs{inRHS.mArray[0], inRHS.mArray[1]};
                    lhs += rhs;
                    mArray[0] = lhs.mArray[0];
                    mArray[1] = lhs.mArray[1]; 
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
                    Scalar lhs{mArray[0], mArray[1]};
                    const Scalar rhs{inRHS.mArray[0], inRHS.mArray[1]};
                    lhs -= rhs;
                    mArray[0] = lhs.mArray[0];
                    mArray[1] = lhs.mArray[1]; 
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
                    Scalar lhs{mArray[0], mArray[1]};
                    const Scalar rhs{inRHS.mArray[0], inRHS.mArray[1]};
                    lhs *= rhs;
                    mArray[0] = lhs.mArray[0];
                    mArray[1] = lhs.mArray[1]; 
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
                    Scalar lhs{mArray[0], mArray[1]};
                    const Scalar rhs{inRHS.mArray[0], inRHS.mArray[1]};
                    lhs /= rhs;
                    mArray[0] = lhs.mArray[0];
                    mArray[1] = lhs.mArray[1]; 
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
                    Scalar lhs{mArray[0], mArray[1]};
                    const Scalar rhs{inRHS.mArray[0], inRHS.mArray[1]};
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

        constexpr void RoundNearest()
        {
            constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
            static_assert(isFloatingPoint, "RoundNearest() only supports floating point types");
            
            // protect our interface so it can remain constexpr
            do 
            {
#if __cpp_lib_is_constant_evaluated
                if (std::is_constant_evaluated())
                {
                    // Delegate to Scalar Impl which is constexpr capable
                    const Scalar round{mArray[0], mArray[1]};
                    round.RoundNearest();
                    mArray[0] = std::get<0>(round.mTuple);
                    mArray[1] = std::get<1>(round.mTuple);

                    break;
                }
#endif // __cpp_lib_is_constant_evaluated
 
                auto round = Simd128<T>::Load2(&mArray[0]);
                round = Simd128<T>::RoundNearest(round);
                Simd128<T>::Store2(&mArray[0], round);
            } while (false);
        }

        constexpr void RoundCeil()
        {
            constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
            static_assert(isFloatingPoint, "RoundCeil() only supports floating point types");
            
            // protect our interface so it can remain constexpr
            do 
            {
#if __cpp_lib_is_constant_evaluated
                if (std::is_constant_evaluated())
                {
                    // Delegate to Scalar Impl which is constexpr capable
                    const Scalar round{mArray[0], mArray[1]};
                    round.RoundCeil();
                    mArray[0] = std::get<0>(round.mTuple);
                    mArray[1] = std::get<1>(round.mTuple);

                    break;
                }
#endif // __cpp_lib_is_constant_evaluated
 
                auto round = Simd128<T>::Load2(&mArray[0]);
                round = Simd128<T>::RoundCeil(round);
                Simd128<T>::Store2(&mArray[0], round);
            } while (false);
        }

        constexpr void RoundFloor()
        {
            constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
            static_assert(isFloatingPoint, "RoundFloor() only supports floating point types");
            
            // protect our interface so it can remain constexpr
            do 
            {
#if __cpp_lib_is_constant_evaluated
                if (std::is_constant_evaluated())
                {
                    // Delegate to Scalar Impl which is constexpr capable
                    const Scalar round{mArray[0], mArray[1]};
                    round.RoundFloor();
                    mArray[0] = std::get<0>(round.mTuple);
                    mArray[1] = std::get<1>(round.mTuple);

                    break;
                }
#endif // __cpp_lib_is_constant_evaluated
 
                auto round = Simd128<T>::Load2(&mArray[0]);
                round = Simd128<T>::RoundFloor(round);
                Simd128<T>::Store2(&mArray[0], round);
            } while (false);
        }

        constexpr void RoundTrunc()
        {
            constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
            static_assert(isFloatingPoint, "RoundTrunc() only supports floating point types");
            
            // protect our interface so it can remain constexpr
            do 
            {
#if __cpp_lib_is_constant_evaluated
                if (std::is_constant_evaluated())
                {
                    // Delegate to Scalar Impl which is constexpr capable
                    const Scalar round{mArray[0], mArray[1]};
                    round.RoundTrunc();
                    mArray[0] = std::get<0>(round.mTuple);
                    mArray[1] = std::get<1>(round.mTuple);

                    break;
                }
#endif // __cpp_lib_is_constant_evaluated
 
                auto round = Simd128<T>::Load2(&mArray[0]);
                round = Simd128<T>::RoundTrunc(round);
                Simd128<T>::Store2(&mArray[0], round);
            } while (false);
        }

    private:
        std::array<T,2> mArray{}; // Impl2: so 2 elements are assumed
    }; // class Simd
}; // struct Impl2<>

template<typename T, ImplKind Impl> // Primary template declaration
struct Impl2Traits;

template<typename T> // Partial template specialization
struct Impl2Traits<T, ImplKind::kScalar>
{
    using ImplType = typename Impl2<T>::Scalar; // VOODOO: Nested template type requires `typename` prefix
};

template<typename T> // Partial template specialization
struct Impl2Traits<T, ImplKind::kSimd>
{
    using ImplType = typename Impl2<T>::Simd; // VOODOO: Nested template type requires `typename` prefix
};

} // namespace saber::geometry::detail

#endif // SABER_GEOMETRY_DETAIL_IMPL2_HPP