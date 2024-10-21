#ifndef SABER_GEOMETRY_DETAIL_IMPL2_HPP
#define SABER_GEOMETRY_DETAIL_IMPL2_HPP

// std
#include <array>
#include <tuple>

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
            std::get<0>(mTuple) += inRHS.Get<0>(); // REVISIT mnfitz 20oct2024: Investigate why std::get<>(inRHS) doesn't work
            std::get<1>(mTuple) += inRHS.Get<1>();
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
            mArray[0] += inRHS.mArray[0];
            mArray[1] += inRHS.mArray[1];
            return *this;
        }

        constexpr Simd& operator-=(const Simd& inRHS)
        {
            mArray[0] -= inRHS.mArray[0];
            mArray[1] -= inRHS.mArray[1];
            return *this;
        }

        constexpr Simd& operator*=(const Simd& inRHS)
        {
            mArray[0] *= inRHS.mArray[0];
            mArray[1] *= inRHS.mArray[1];
            return *this;
        }

        constexpr Simd& operator/=(const Simd& inRHS)
        {
            mArray[0] /= inRHS.mArray[0];
            mArray[1] /= inRHS.mArray[1];
            return *this;
        }

    private:
        std::array<T,2> mArray{}; // 2 is assumed
    };
}; // struct Impl2<>

} // namespace saber::geometry::detail

#endif // SABER_GEOMETRY_DETAIL_IMPL2_HPP