#ifndef SABER_GEOMETRY_DETAIL_SIMD_SSE_HPP
#define SABER_GEOMETRY_DETAIL_SIMD_SSE_HPP
#pragma once

// sse
#include <emmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>

namespace saber::geometry::detail {

// int
template<>
struct Simd<128, int> :
    public SimdTraits<128, int> // is-a: SimdTraits<128, int>
{
using typename SimdTraits<128, int>::SimdType; // Expose `SimdType` as our own

	/// @brief Load 4 elements of type`<int>` from memory specified by `inAddr`.
	/// @param inAddr Address of &elements[4] to load
	/// @return Vector type`<int>` of loaded elements
	static constexpr SimdType Load4(const int* inAddr)
	{
		static_assert(sizeof(SimdType) >= 4 , "4 elements of type<int> are too large to fit in 128bit SimdType");
        auto load4 =  _mm_load_si128(reinterpret_cast<const __m128i*>(inAddr));
        return load4;
	}

	/// @brief Load 2 elements of type`<int>` from memory specified by `inAddr`.
	/// Loaded elements are placed in low order position in result SimdType.
	/// Any unused high order elements are set to zero.
	/// @param inAddr Address of &elements[2] to load
	/// @return Vector type`<int>` of loaded elements
	static constexpr SimdType Load2(const int* inAddr)
	{
		auto load2 =  _mm_loadl_epi64(reinterpret_cast<const __m128i*>(inAddr));
		return load2;
	}

	/// @brief Load 1 element of type`<int>` from memory specified by `inAddr`.
	/// Loaded element is placed in low order position in result SimdType.
	/// Any unused high order elements are set to zero.
	/// @param inAddr Address of &element[1] to load
	/// @return Vector type`<int>` of loaded elements
	static constexpr SimdType Load1(const int* inAddr)
	{
        auto load1 =  _mm_loadu_si32(inAddr);
		return load1;
	}

	/// @brief Store 4 elements of type`<int>` to memory specified by `outAddr`.
	/// @param outAddr Address to store &elements[4]
	/// @param inStore4 Vector type`<int>` of elements to store
	static constexpr void Store4(int* outAddr, SimdType inStore4)
	{
		static_assert(sizeof(SimdType) >= 4, "128bit SimdType is too small to contain 4 elements of type<int>");
        _mm_store_si128(reinterpret_cast<__m128i*>(outAddr), inStore4);
	}

	/// @brief Store 2 elements of type`<int>` to memory specified by `outAddr`.
	/// The 2 lowest order elements are stored to memory.
	/// Any high order elements are ignored.
	/// @param outAddr Address to store &elements[2]
	/// @param inStore2 Vector type`<int>` of elements to store
	static constexpr void Store2(int* outAddr, SimdType inStore2)
	{
        _mm_storel_epi64(reinterpret_cast<__m128i*>(outAddr), inStore2);

	}

	/// @brief Store 1 element of type`<int>` to memory specified by `outAddr`.
	/// The lowest order element is stored to memory.
	/// Any higher order elements are ignored.
	/// @param outAddr Address to store &element[1]
	/// @param inStore1 Vector type`<int>` of elements to store
	static constexpr void Store1(int* outAddr, SimdType inStore1)
	{
        _mm_storeu_si32(reinterpret_cast<__m128i*>(outAddr), inStore1);
	}

	/// @brief Add all vector type`<int>` elements in `inRHS` to `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<int>`
	static constexpr SimdType Add(SimdType inLHS, SimdType inRHS)
	{
		auto add = _mm_add_epi32(inLHS, inRHS);
		return add;
	}

	/// @brief Subtract all vector type`<int>` elements in `inRHS` from `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<int>`
	static constexpr SimdType Sub(SimdType inLHS, SimdType inRHS)
	{
		auto sub = _mm_sub_epi32(inLHS, inRHS);
		return sub;
	}

	/// @brief Multiply all vector type`<int>` elements in `inRHS` to `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<int>`
	static constexpr SimdType Mul(SimdType inLHS, SimdType inRHS)
	{
        auto mul = _mm_mullo_epi32(inLHS, inRHS);
		return mul;
	}

	/// @brief Divide all vector type`<int>` elements in `inRHS` from `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<int>`

    /*
	static constexpr SimdType Div(SimdType inLHS, SimdType inRHS)
	{
		// SSE does not implement integer division, therefore, 
        // default to primary template to do it the hard way. 
	}
    */
};

// float
template<>
struct Simd<128, float> :
    public SimdTraits<128, float> // is-a: SimdTraits<128, int>
{
using typename SimdTraits<128, float>::SimdType; // Expose `SimdType` as our own

	/// @brief Load 4 elements of type`<float>` from memory specified by `inAddr`.
	/// @param inAddr Address of &elements[4] to load
	/// @return Vector type`<float>` of loaded elements
	static constexpr SimdType Load4(const float* inAddr)
	{
		static_assert(sizeof(SimdType) >= 4 , "4 elements of type<float> are too large to fit in 128bit SimdType");
        auto load4 =  _mm_load_ps(inAddr);
        return load4;
	}

	/// @brief Load 2 elements of type`<float>` from memory specified by `inAddr`.
	/// Loaded elements are placed in low order position in result SimdType.
	/// Any unused high order elements are set to zero.
	/// @param inAddr Address of &elements[2] to load
	/// @return Vector type`<float>` of loaded elements
	static constexpr SimdType Load2(const float* inAddr)
	{
        auto load2 = _mm_setzero_ps();
		load2 =  _mm_loadl_pi(load2, reinterpret_cast<const __m64*>(inAddr));
		return load2;
	}

	/// @brief Load 1 element of type`<float>` from memory specified by `inAddr`.
	/// Loaded element is placed in low order position in result SimdType.
	/// Any unused high order elements are set to zero.
	/// @param inAddr Address of &element[1] to load
	/// @return Vector type`<float>` of loaded elements
	static constexpr SimdType Load1(const float* inAddr)
	{
        auto load1 =  _mm_load_ss(inAddr);
		return load1;
	}

	/// @brief Store 4 elements of type`<float>` to memory specified by `outAddr`.
	/// @param outAddr Address to store &elements[4]
	/// @param inStore4 Vector type`<float>` of elements to store
	static constexpr void Store4(float* outAddr, SimdType inStore4)
	{
		static_assert(sizeof(SimdType) >= 4, "128bit SimdType is too small to contain 4 elements of type<float>");
        _mm_store_ps(outAddr, inStore4);
	}

	/// @brief Store 2 elements of type`<float>` to memory specified by `outAddr`.
	/// The 2 lowest order elements are stored to memory.
	/// Any high order elements are ignored.
	/// @param outAddr Address to store &elements[2]
	/// @param inStore2 Vector type`<float>` of elements to store
	static constexpr void Store2(float* outAddr, SimdType inStore2)
	{
        _mm_storel_pi(reinterpret_cast<__m64*>(outAddr), inStore2);
	}

	/// @brief Store 1 element of type`<float>` to memory specified by `outAddr`.
	/// The lowest order element is stored to memory.
	/// Any higher order elements are ignored.
	/// @param outAddr Address to store &element[1]
	/// @param inStore1 Vector type`<float>` of elements to store
	static constexpr void Store1(float* outAddr, SimdType inStore1)
	{
        _mm_store_ss(outAddr, inStore1);
	}

	/// @brief Add all vector type`<float>` elements in `inRHS` to `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<float>`
	static constexpr SimdType Add(SimdType inLHS, SimdType inRHS)
	{
		auto add = _mm_add_ps(inLHS, inRHS);
		return add;
	}

	/// @brief Subtract all vector type`<float>` elements in `inRHS` from `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<float>`
	static constexpr SimdType Sub(SimdType inLHS, SimdType inRHS)
	{
		auto sub = _mm_sub_ps(inLHS, inRHS);
		return sub;
	}

	/// @brief Multiply all vector type`<float>` elements in `inRHS` to `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<float>`
	static constexpr SimdType Mul(SimdType inLHS, SimdType inRHS)
	{
        auto mul = _mm_mul_ps(inLHS, inRHS);
		return mul;
	}

	/// @brief Divide all vector type`<float>` elements in `inRHS` from `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<float>`
	static constexpr SimdType Div(SimdType inLHS, SimdType inRHS)
	{
		auto div = _mm_div_ps(inLHS, inRHS);
		return div;
	}
};

// double
template<>
struct Simd<128, double> :
    public SimdTraits<128, double> // is-a: SimdTraits<128, int>
{
using typename SimdTraits<128, double>::SimdType; // Expose `SimdType` as our own

/*
	/// @brief Load 4 elements of type`<double>` from memory specified by `inAddr`.
	/// @param inAddr Address of &elements[4] to load
	/// @return Vector type`<double>` of loaded elements
	static constexpr SimdType Load4(const double* inAddr)
	{
        // Load 4 impossible for 64 bit double to fit into 128 bit vector
	}
*/

	/// @brief Load 2 elements of type`<double>` from memory specified by `inAddr`.
	/// Loaded elements are placed in low order position in result SimdType.
	/// Any unused high order elements are set to zero.
	/// @param inAddr Address of &elements[2] to load
	/// @return Vector type`<double>` of loaded elements
	static constexpr SimdType Load2(const double* inAddr)
	{
		auto load2 =  _mm_load_pd(inAddr);
		return load2;
	}

	/// @brief Load 1 element of type`<double>` from memory specified by `inAddr`.
	/// Loaded element is placed in low order position in result SimdType.
	/// Any unused high order elements are set to zero.
	/// @param inAddr Address of &element[1] to load
	/// @return Vector type`<double>` of loaded elements
	static constexpr SimdType Load1(const double* inAddr)
	{
        auto load1 =  _mm_load_sd(inAddr);
		return load1;
	}

/*
	/// @brief Store 4 elements of type`<double>` to memory specified by `outAddr`.
	/// @param outAddr Address to store &elements[4]
	/// @param inStore4 Vector type`<double>` of elements to store
	static constexpr void Store4(double* outAddr, SimdType inStore4)
	{
		static_assert(sizeof(SimdType) >= 4, "128bit SimdType is too small to contain 4 elements of type<double>");
        _mm_store_ps(outAddr, inStore4);
	}
*/

	/// @brief Store 2 elements of type`<double>` to memory specified by `outAddr`.
	/// The 2 lowest order elements are stored to memory.
	/// Any high order elements are ignored.
	/// @param outAddr Address to store &elements[2]
	/// @param inStore2 Vector type`<double>` of elements to store
	static constexpr void Store2(double* outAddr, SimdType inStore2)
	{
        _mm_store_pd(outAddr, inStore2);
	}

	/// @brief Store 1 element of type`<double>` to memory specified by `outAddr`.
	/// The lowest order element is stored to memory.
	/// Any higher order elements are ignored.
	/// @param outAddr Address to store &element[1]
	/// @param inStore1 Vector type`<double>` of elements to store
	static constexpr void Store1(double* outAddr, SimdType inStore1)
	{
        _mm_store_sd(outAddr, inStore1);
	}

	/// @brief Add all vector type`<double>` elements in `inRHS` to `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<double>`
	static constexpr SimdType Add(SimdType inLHS, SimdType inRHS)
	{
		auto add = _mm_add_pd(inLHS, inRHS);
		return add;
	}

	/// @brief Subtract all vector type`<double>` elements in `inRHS` from `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<double>`
	static constexpr SimdType Sub(SimdType inLHS, SimdType inRHS)
	{
		auto sub = _mm_sub_pd(inLHS, inRHS);
		return sub;
	}

	/// @brief Multiply all vector type`<double>` elements in `inRHS` to `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<double>`
	static constexpr SimdType Mul(SimdType inLHS, SimdType inRHS)
	{
        auto mul = _mm_mul_pd(inLHS, inRHS);
		return mul;
	}

	/// @brief Divide all vector type`<double>` elements in `inRHS` from `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<double>`
	static constexpr SimdType Div(SimdType inLHS, SimdType inRHS)
	{
		auto div = _mm_div_pd(inLHS, inRHS);
		return div;
	}
};

} // namespace saber::geometry::detail

#endif // SABER_GEOMETRY_DETAIL_SIMD_SSE_HPP
