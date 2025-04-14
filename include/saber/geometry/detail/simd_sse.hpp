#ifndef SABER_GEOMETRY_DETAIL_SIMD_SSE_HPP
#define SABER_GEOMETRY_DETAIL_SIMD_SSE_HPP
#pragma once

// sse
#include <immintrin.h>

namespace saber::geometry::detail {

// int
template<>
struct Simd128<int> :
    public Simd128Traits<int> // is-a: Simd128Traits<int>
{
using typename Simd128Traits<int>::SimdType; // Expose `SimdType` as our own

	/// @brief Load 4 elements of type`<int>` from memory specified by `inAddr`.
	/// @param inAddr Address of &elements[4] to load
	/// @return Vector type`<int>` of loaded elements
	static SimdType Load4(const int* inAddr)
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
	static SimdType Load2(const int* inAddr)
	{
		auto load2 =  _mm_loadl_epi64(reinterpret_cast<const __m128i*>(inAddr));
		return load2;
	}

	/// @brief Load 1 element of type`<int>` from memory specified by `inAddr`.
	/// Loaded element is placed in low order position in result SimdType.
	/// Any unused high order elements are set to zero.
	/// @param inAddr Address of &element[1] to load
	/// @return Vector type`<int>` of loaded elements
	static SimdType Load1(const int* inAddr)
	{
        auto load1 =  _mm_loadu_si32(inAddr);
		return load1;
	}

	/// @brief Store 4 elements of type`<int>` to memory specified by `outAddr`.
	/// @param outAddr Address to store &elements[4]
	/// @param inStore4 Vector type`<int>` of elements to store
	static void Store4(int* outAddr, SimdType inStore4)
	{
		static_assert(sizeof(SimdType) >= 4, "128bit SimdType is too small to contain 4 elements of type<int>");
        _mm_store_si128(reinterpret_cast<__m128i*>(outAddr), inStore4);
	}

	/// @brief Store 2 elements of type`<int>` to memory specified by `outAddr`.
	/// The 2 lowest order elements are stored to memory.
	/// Any high order elements are ignored.
	/// @param outAddr Address to store &elements[2]
	/// @param inStore2 Vector type`<int>` of elements to store
	static void Store2(int* outAddr, SimdType inStore2)
	{
        _mm_storel_epi64(reinterpret_cast<__m128i*>(outAddr), inStore2);

	}

	/// @brief Store 1 element of type`<int>` to memory specified by `outAddr`.
	/// The lowest order element is stored to memory.
	/// Any higher order elements are ignored.
	/// @param outAddr Address to store &element[1]
	/// @param inStore1 Vector type`<int>` of elements to store
	static void Store1(int* outAddr, SimdType inStore1)
	{
        _mm_storeu_si32(reinterpret_cast<__m128i*>(outAddr), inStore1);
	}

	/// @brief Add all vector type`<int>` elements in `inRHS` to `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<int>`
	static SimdType Add(SimdType inLHS, SimdType inRHS)
	{
		auto add = _mm_add_epi32(inLHS, inRHS);
		return add;
	}

	/// @brief Subtract all vector type`<int>` elements in `inRHS` from `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<int>`
	static SimdType Sub(SimdType inLHS, SimdType inRHS)
	{
		auto sub = _mm_sub_epi32(inLHS, inRHS);
		return sub;
	}

	/// @brief Multiply all vector type`<int>` elements in `inRHS` to `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<int>`
	static SimdType Mul(SimdType inLHS, SimdType inRHS)
	{
        auto mul = _mm_mullo_epi32(inLHS, inRHS);
		return mul;
	}

	/// @brief Divide all vector type`<int>` elements in `inRHS` from `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<int>`
	static SimdType Div(SimdType inLHS, SimdType inRHS)
	{
		// Note: Intel SSE does not support SIMD integer division
		// Therefore, we do it the hard way
		std::array<int, 4> lhs{};
		std::array<int, 4> rhs{};
		Store4(lhs.data(), inLHS);
		Store4(rhs.data(), inRHS);
		lhs.at(0) = (rhs.at(0) != 0 ? lhs.at(0) / rhs.at(0) : 0);
		lhs.at(1) = (rhs.at(1) != 0 ? lhs.at(1) / rhs.at(1) : 0);
		lhs.at(2) = (rhs.at(2) != 0 ? lhs.at(2) / rhs.at(2) : 0);
		lhs.at(3) = (rhs.at(3) != 0 ? lhs.at(3) / rhs.at(3) : 0);
		auto div = Load4(lhs.data());
		return div;
	}

	/// @brief Compare two vector<int> values to check if all elements equal.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return true if corresponding elements are equal, false otherwise
	static bool IsEQ(SimdType inLHS, SimdType inRHS)
	{
        const auto eq = _mm_cmpeq_epi32(inLHS, inRHS);
		const auto mask = _mm_movemask_epi8(eq);
		const bool allEqual = (mask == 0xFFFF);
		return allEqual;
	}

	//static SimdType RoundNearest(SimdType inRound)
	// Not Implemented for integers

};

// float
template<>
struct Simd128<float> :
    public Simd128Traits<float> // is-a: Simd128Traits<int>
{
using typename Simd128Traits<float>::SimdType; // Expose `SimdType` as our own

	/// @brief Load 4 elements of type`<float>` from memory specified by `inAddr`.
	/// @param inAddr Address of &elements[4] to load
	/// @return Vector type`<float>` of loaded elements
	static SimdType Load4(const float* inAddr)
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
	static SimdType Load2(const float* inAddr)
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
	static SimdType Load1(const float* inAddr)
	{
        auto load1 =  _mm_load_ss(inAddr);
		return load1;
	}

	/// @brief Store 4 elements of type`<float>` to memory specified by `outAddr`.
	/// @param outAddr Address to store &elements[4]
	/// @param inStore4 Vector type`<float>` of elements to store
	static void Store4(float* outAddr, SimdType inStore4)
	{
		static_assert(sizeof(SimdType) >= 4, "128bit SimdType is too small to contain 4 elements of type<float>");
        _mm_store_ps(outAddr, inStore4);
	}

	/// @brief Store 2 elements of type`<float>` to memory specified by `outAddr`.
	/// The 2 lowest order elements are stored to memory.
	/// Any high order elements are ignored.
	/// @param outAddr Address to store &elements[2]
	/// @param inStore2 Vector type`<float>` of elements to store
	static void Store2(float* outAddr, SimdType inStore2)
	{
        _mm_storel_pi(reinterpret_cast<__m64*>(outAddr), inStore2);
	}

	/// @brief Store 1 element of type`<float>` to memory specified by `outAddr`.
	/// The lowest order element is stored to memory.
	/// Any higher order elements are ignored.
	/// @param outAddr Address to store &element[1]
	/// @param inStore1 Vector type`<float>` of elements to store
	static void Store1(float* outAddr, SimdType inStore1)
	{
        _mm_store_ss(outAddr, inStore1);
	}

	/// @brief Add all vector type`<float>` elements in `inRHS` to `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<float>`
	static SimdType Add(SimdType inLHS, SimdType inRHS)
	{
		auto add = _mm_add_ps(inLHS, inRHS);
		return add;
	}

	/// @brief Subtract all vector type`<float>` elements in `inRHS` from `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<float>`
	static SimdType Sub(SimdType inLHS, SimdType inRHS)
	{
		auto sub = _mm_sub_ps(inLHS, inRHS);
		return sub;
	}

	/// @brief Multiply all vector type`<float>` elements in `inRHS` to `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<float>`
	static SimdType Mul(SimdType inLHS, SimdType inRHS)
	{
        auto mul = _mm_mul_ps(inLHS, inRHS);
		return mul;
	}

	/// @brief Divide all vector type`<float>` elements in `inRHS` from `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<float>`
	static SimdType Div(SimdType inLHS, SimdType inRHS)
	{
		auto div = _mm_div_ps(inLHS, inRHS);
		return div;
	}

	/// @brief Compare two vector<float> values to check if all elements equal.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return true if corresponding elements are equal, false otherwise
	static bool IsEQ(SimdType inLHS, SimdType inRHS)
	{
		// Create a vector mask to remove the sign bit for floats so that we can take the absolute value of a vector of floats
		const auto signMask = ~(1U << (sizeof(float) * 8 - 1));
		const auto absMask = _mm_castsi128_ps(_mm_set1_epi32(signMask));

		// Take the absolute value of four floats at a time for LHS and RHS
		const auto absLHS = _mm_and_ps(inLHS, absMask);
		const auto absRHS = _mm_and_ps(inRHS, absMask);

		// Create the magnitude of the largest 4 floats, between LHS and RHS, such that they are greater than 1.0
		const auto minMagnitude = _mm_set_ps1(1);
		const auto magnitude = _mm_max_ps(_mm_max_ps(absLHS, absRHS), minMagnitude);

		// Create the tolerance for the comparison based on the largest LHS or RHS value being compared
		// The bigger the number, the greater the allowed inexactness for IsEqual()
		const auto epsilon = _mm_mul_ps(magnitude, _mm_set_ps1(std::numeric_limits<float>::epsilon()));

		// Compare LHS to RHS via subtraction, and take its absolute value
		const auto comparison = _mm_and_ps(_mm_sub_ps(inLHS, inRHS), absMask);

		// See if the difference is within the allowed computed epsilon/tolerance
		const auto result = _mm_cmple_ps(comparison, epsilon);

		// Get the 4 comparison results into a single comparable mask, such that it makes a simple bool
		const auto mask = _mm_movemask_ps(result);

		// See is all 4 bits of the mask were true (if so, IsEq() returns true)
		const bool approxEq = (mask == 0xF);
		return approxEq;
	}

	/// @brief Round all <float> values toward the nearest whole number
	/// @param inRound Input to be rounded
	/// @return Return rounded SimdType values
	static SimdType RoundNearest(SimdType inRound)
	{
		const auto pos = _mm_set_ps(0.5f, 0.5f, 0.5f, 0.5f);
		const auto neg = _mm_set_ps(-0.5f, -0.5f, -0.5f, -0.5f);

		const auto byHalf = _mm_blendv_ps(pos, neg, inRound);
		const auto round = _mm_add_ps(inRound, byHalf);
		return RoundTrunc(round);
	}

	/// @brief Round all <float> values toward positive infinity
	/// @param inRound Input to be rounded
	/// @return Return rounded SimdType values
	static SimdType RoundCeil(SimdType inRound)
	{
		auto round = _mm_round_ps(inRound, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);
		return round;
	}

	/// @brief Round all <float> values toward negative infinity
	/// @param inRound Input to be rounded
	/// @return Return rounded SimdType values
	static SimdType RoundFloor(SimdType inRound)
	{
		auto round = _mm_round_ps(inRound, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC);
		return round;
	}

	/// @brief Round all <float> values toward zero
	/// @param inRound Input to be rounded
	/// @return Return rounded SimdType values
	static SimdType RoundTrunc(SimdType inRound)
	{
		auto round = _mm_round_ps(inRound, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
		return round;
	}
};

// double
template<>
struct Simd128<double> :
    public Simd128Traits<double> // is-a: Simd128Traits<int>
{
using typename Simd128Traits<double>::SimdType; // Expose `SimdType` as our own

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
	static SimdType Load2(const double* inAddr)
	{
		auto load2 =  _mm_load_pd(inAddr);
		return load2;
	}

	/// @brief Load 1 element of type`<double>` from memory specified by `inAddr`.
	/// Loaded element is placed in low order position in result SimdType.
	/// Any unused high order elements are set to zero.
	/// @param inAddr Address of &element[1] to load
	/// @return Vector type`<double>` of loaded elements
	static SimdType Load1(const double* inAddr)
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
	static void Store2(double* outAddr, SimdType inStore2)
	{
        _mm_store_pd(outAddr, inStore2);
	}

	/// @brief Store 1 element of type`<double>` to memory specified by `outAddr`.
	/// The lowest order element is stored to memory.
	/// Any higher order elements are ignored.
	/// @param outAddr Address to store &element[1]
	/// @param inStore1 Vector type`<double>` of elements to store
	static void Store1(double* outAddr, SimdType inStore1)
	{
        _mm_store_sd(outAddr, inStore1);
	}

	/// @brief Add all vector type`<double>` elements in `inRHS` to `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<double>`
	static SimdType Add(SimdType inLHS, SimdType inRHS)
	{
		auto add = _mm_add_pd(inLHS, inRHS);
		return add;
	}

	/// @brief Subtract all vector type`<double>` elements in `inRHS` from `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<double>`
	static SimdType Sub(SimdType inLHS, SimdType inRHS)
	{
		auto sub = _mm_sub_pd(inLHS, inRHS);
		return sub;
	}

	/// @brief Multiply all vector type`<double>` elements in `inRHS` to `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<double>`
	static SimdType Mul(SimdType inLHS, SimdType inRHS)
	{
        auto mul = _mm_mul_pd(inLHS, inRHS);
		return mul;
	}

	/// @brief Divide all vector type`<double>` elements in `inRHS` from `inLHS`.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<double>`
	static SimdType Div(SimdType inLHS, SimdType inRHS)
	{
		auto div = _mm_div_pd(inLHS, inRHS);
		return div;
	}

	/// @brief Compare two vector<double> values to check if all elements equal.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return true if corresponding elements are equal, false otherwise
	static bool IsEQ(SimdType inLHS, SimdType inRHS)
	{
		// Create a vector mask to remove the sign bit for doubles so that we can take the absolute value of a vector of doubles
		constexpr auto signMask = ~(1ULL << (sizeof(double) * 8 - 1));
		// 64 bits in a double. 1 << 63
		// sizeof(double) = 8/*bytes*/ x 8 /*bits*/ = 64
		const auto absMask = _mm_castsi128_pd(_mm_set1_epi64x(signMask));

		// Take the absolute value of two doubles at a time for LHS and RHS
		const auto absLHS = _mm_and_pd(inLHS, absMask);
		const auto absRHS = _mm_and_pd(inRHS, absMask);

		// Create the magnitude of the largest 2 doubles, between LHS and RHS, such that they are greater than 1.0
		const auto minMagnitude = _mm_set1_pd(1);
		const auto magnitude = _mm_max_pd(_mm_max_pd(absLHS, absRHS), minMagnitude);

		// Create the tolerance for the comparison based on the largest LHS or RHS value being compared
		// The bigger the number, the greater the allowed inexactness for IsEqual()
		const auto epsilon = _mm_mul_pd(magnitude, _mm_set1_pd(std::numeric_limits<double>::epsilon()));

		// Compare LHS to RHS via subtraction, and take its absolute value
		const auto comparison = _mm_and_pd(_mm_sub_pd(inLHS, inRHS), absMask);

		// See if the difference is within the allowed computed epsilon/tolerance
		const auto result = _mm_cmple_pd(comparison, epsilon);

		// Get the 2 comparison results into a single comparable mask, such that it makes a simple bool
		const auto mask = _mm_movemask_pd(result);

		// See is all 2 bits of the mask were true (if so, IsEq() returns true)
		const bool approxEq = (mask == 0x3);
		return approxEq;
	}

	/// @brief Round all <double> values toward the nearest whole number
	/// @param inRound Input to be rounded
	/// @return Return rounded SimdType values
	static SimdType RoundNearest(SimdType inRound)
	{
		const auto pos = _mm_set_pd(0.5, 0.5);
		const auto neg = _mm_set_pd(-0.5, -0.5);

		const auto byHalf = _mm_blendv_pd(pos, neg, inRound);
		const auto round = _mm_add_pd(inRound, byHalf);
		return RoundTrunc(round);
	}

	/// @brief Round all <double> values toward positive infinity
	/// @param inRound Input to be rounded
	/// @return Return rounded SimdType values
	static SimdType RoundCeil(SimdType inRound)
	{
		auto round = _mm_round_pd(inRound, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);
		return round;
	}

	/// @brief Round all <double> values toward negative infinity
	/// @param inRound Input to be rounded
	/// @return Return rounded SimdType values
	static SimdType RoundFloor(SimdType inRound)
	{
		auto round = _mm_round_pd(inRound, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC);
		return round;
	}

	/// @brief Round all <double> values toward zero
	/// @param inRound Input to be rounded
	/// @return Return rounded SimdType values
	static SimdType RoundTrunc(SimdType inRound)
	{
		auto round = _mm_round_pd(inRound, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
		return round;
	}
};

} // namespace saber::geometry::detail

#endif // SABER_GEOMETRY_DETAIL_SIMD_SSE_HPP
