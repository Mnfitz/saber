#ifndef SABER_GEOMETRY_DETAIL_IMPL_HPP
#define SABER_GEOMETRY_DETAIL_IMPL_HPP

// std
#include <array>

namespace saber::geometry::detail {

template<typename T>
struct Impl2
{
    class Scalar
    {
    public:

    private:
        std::array<T,2> mArray{}; // 2 is assumed
    };

    class Simd
    {
    public:

    private:
        std::array<T,2> mArray{}; // 2 is assumed
    };
}; // struct Impl2<>

} // namespace saber::geometry::detail

#endif // SABER_GEOMETRY_DETAIL_IMPL_HPP