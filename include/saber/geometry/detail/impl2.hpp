#ifndef SABER_GEOMETRY_DETAIL_IMPL2_HPP
#define SABER_GEOMETRY_DETAIL_IMPL2_HPP

// std
#include <array>
#include <tuple>

// saber
#include "saber/geometry/detail/simd.hpp"

namespace saber::geometry::detail {

template<typename Lambda, int=(Lambda{}(), 0)>
constexpr bool is_constexpr(Lambda) { return false; }

constexpr bool is_constexpr(...) { return true; }

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
            std::get<0>(mTuple) += std::get<0>(inRHS.mTuple); // REVISIT mnfitz 20oct2024: Investigate why std::get<>(inRHS) doesn't work
            std::get<1>(mTuple) += std::get<1>(inRHS.mTuple);
            return *this;
        }

        constexpr Scalar& operator-=(const Scalar& inRHS)
        {
            std::get<0>(mTuple) -= inRHS.Get<0>(); // REVISIT mnfitz 20oct2024: Investigate why std::get<>(inRHS) doesn't work
            std::get<1>(mTuple) -= inRHS.Get<1>();
            return *this;
        }

        constexpr Scalar& operator*=(const Scalar& inRHS)
        {
            std::get<0>(mTuple) *= inRHS.Get<0>(); // REVISIT mnfitz 20oct2024: Investigate why std::get<>(inRHS) doesn't work
            std::get<1>(mTuple) *= inRHS.Get<1>();
            return *this;
        }

        constexpr Scalar& operator/=(const Scalar& inRHS)
        {
            std::get<0>(mTuple) /= inRHS.Get<0>(); // REVISIT mnfitz 20oct2024: Investigate why std::get<>(inRHS) doesn't work
            std::get<1>(mTuple) /= inRHS.Get<1>();
            return *this;
        }

    private:
        std::tuple<T,T> mTuple{}; // 2 is assumed
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
            constexpr auto lambda = [](){return 0;};
            if (is_constexpr(lambda))
            {
                Scalar lhs{Get<0>(), Get<1>()};
                const Scalar rhs{inRHS.Get<0>(), inRHS.Get<1>()};
                lhs += rhs;
                mArray[0] = lhs.Get<0>();
                mArray[1] = lhs.Get<1>(); 
            }
            else
            {
                auto lhs = Simd128<T>::Load2(&mArray[0]);
                auto rhs = Simd128<T>::Load2(&inRHS.mArray[0]);
                auto result = Simd128<T>::Add(lhs, rhs);
                Simd128<T>::Store2(&mArray[0], result);
            }
            return *this;
        }

        constexpr Simd& operator-=(const Simd& inRHS)
        {
            auto lhs = Simd128<T>::Load2(&mArray[0]);
            auto rhs = Simd128<T>::Load2(&inRHS.mArray[0]);
            auto result = Simd128<T>::Sub(lhs, rhs);
            Simd128<T>::Store2(&mArray[0], result);
            return *this;
        }

        constexpr Simd& operator*=(const Simd& inRHS)
        {
            auto lhs = Simd128<T>::Load2(&mArray[0]);
            auto rhs = Simd128<T>::Load2(&inRHS.mArray[0]);
            auto result = Simd128<T>::Mul(lhs, rhs);
            Simd128<T>::Store2(&mArray[0], result);
            return *this;
        }

        constexpr Simd& operator/=(const Simd& inRHS)
        {
            auto lhs = Simd128<T>::Load2(&mArray[0]);
            auto rhs = Simd128<T>::Load2(&inRHS.mArray[0]);
            auto result = Simd128<T>::Div(lhs, rhs);
            Simd128<T>::Store2(&mArray[0], result);
            return *this;
        }

    private:
        std::array<T,2> mArray{}; // 2 is assumed
    };
}; // struct Impl2<>

} // namespace saber::geometry::detail

#endif // SABER_GEOMETRY_DETAIL_IMPL2_HPP