#ifndef SABER_GEOMETRY_DETAIL_SIMD_TRAITS_HPP
#define SABER_GEOMETRY_DETAIL_SIMD_TRAITS_HPP

// saber
#include "saber/geometry/config.hpp"

// std
#include <array>

#if SABER_CPU(X86)
// sse
#include <emmintrin.h>
#endif

namespace saber::geometry::detail {

// ------------------------------------------------------------------
#pragma region SimdTraits<NBits, T>

/// @brief Traits struct defining platform-specific SIMD types.
///
/// Used by the `detail::Simd<NBits, T>` implementation. Note:
/// This "primary" template definition describes it's types in a
/// platform-independent way. Use template specializations of this
/// struct to define platform-specific SIMD types for a given CPU.
/// @tparam T Typeof underlying SIMD elements described by this trait
template<typename T>
struct Simd128Traits
{
	/// @brief Number of elements of type T in a SIMD vector
	static constexpr std::size_t kSize = (128/8)/sizeof(T);

	/// @brief Underlying type of a SIMD element
	using ValueType = T;

	/// @brief Platform-specific type of a SIMD vector of elements
	using SimdType = std::array<T, kSize>; // As many `T`'s that will fit in NBits

}; // struct SimdTraits<>

#pragma endregion {}

#if SABER_GEOMETRY_CONFIG_ISENABLED_SIMD

// Template specializations of `SimdTraits<>` for each supported CPU.
// These specializations have the effect of "overridding" the type
// definitions of the "primary" `SimdTraits<>` definition...

// ------------------------------------------------------------------
#pragma region X86 SimdTraits<>
#if SABER_CPU(X86)

// Platform-specific SIMD definitions for X86...

// int
template<>
struct Simd128Traits<int>
{
	/// @brief Number of elements of type T in a SIMD vector
	static constexpr std::size_t kSize = 4;

	/// @brief Underlying type of a SIMD element
	using ValueType = int;

	/// @brief Platform-specific type of a SIMD vector of elements
	using SimdType = __m128i;  // vector of int

}; // struct SimdTraits<>

// float
template<>
struct Simd128Traits<float>
{
	/// @brief Number of elements of type T in a SIMD vector
	static constexpr std::size_t kSize = 4;

	/// @brief Underlying type of a SIMD element
	using ValueType = float;

	/// @brief Platform-specific type of a SIMD vector of elements
	using SimdType = __m128; // vector of float

}; // struct SimdTraits<>

// double
template<>
struct Simd128Traits<double>
{
	/// @brief Number of elements of type T in a SIMD vector
	static constexpr std::size_t kSize = 2;

	/// @brief Underlying type of a SIMD element
	using ValueType = double;

	/// @brief Platform-specific type of a SIMD vector of elements
	using SimdType = __m128d; // vector of double

}; // struct SimdTraits<>

#endif // SABER_CPU(X86)
#pragma endregion {}

// ------------------------------------------------------------------
#pragma region ARM SimdTraits<>
#if SABER_CPU(ARM)

// Platform-specific SIMD definitions for ARM...

#endif // SABER_CPU(ARM)
#pragma endregion {}

#if 0
// TODO: PPC && RISCV

// ------------------------------------------------------------------
#pragma region PPC SimdTraits<>
// #if SABER_CPU(PPC)
// Platform-specific SIMD definitions for PPC...

// #endif // SABER_CPU(ARM)
#pragma endregion {}

// ------------------------------------------------------------------
#pragma region RISCV SimdTraits<>
// #if SABER_CPU(RISCV)
// Platform-specific SIMD definitions for RISCV...

// #endif // SABER_CPU(RISCV)
#pragma endregion {}

#endif // 0
#endif // SABER_GEOMETRY_CONFIG_ISENABLED_SIMD

} // namespace saber::geometry::detail

#endif // SABER_GEOMETRY_DETAIL_SIMD_TRAITS_HPP
