#ifndef SABER_GEOMETRY_DETAIL_SIMD_SSE_HPP
#define SABER_GEOMETRY_DETAIL_SIMD_SSE_HPP
#pragma once

// saber
#include "saber/config.hpp"

namespace saber::geometry::detail {

// int
template<>
struct Simd128<int> :
    public Simd128Traits<int> // is-a: Simd128Traits<int>
{
	using typename Simd128Traits<int>::SimdType; // __m128i
	using typename Simd128Traits<int>::ValueType; // int

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
		std::array<int, 4> lhs, rhs;
        Store4(lhs.data(), inLHS);
        Store4(rhs.data(), inRHS);
        for (int i = 0; i < 4; ++i)
            lhs[i] = (rhs[i] != 0 ? lhs[i] / rhs[i] : 0);
        return Load4(lhs.data());
	}

	/// @brief Duplicate the low half of the SIMD register into both halves.
    /// @param inSimd Input SIMD register.
    /// @return SIMD register with low half duplicated.
	static SimdType DupLo(SimdType inSimd)
	{
        auto dup = _mm_shuffle_epi32(inSimd, _MM_SHUFFLE(1, 0, 1, 0));
        return dup;
    }

    /// @brief Duplicate the high half of the SIMD register into both halves.
    /// @param inSimd Input SIMD register.
    /// @return SIMD register with high half duplicated.
    static SimdType DupHi(SimdType inSimd)
	{
        auto dup = _mm_shuffle_epi32(inSimd, _MM_SHUFFLE(3, 2, 3, 2));
        return dup;
    }

	/// @brief Compare two vector<int> values to check if all elements equal.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return true if corresponding elements are equal, false otherwise
	static bool IsEq(SimdType inLHS, SimdType inRHS)
	{
        const auto eq = _mm_cmpeq_epi32(inLHS, inRHS);
		const auto mask = _mm_movemask_epi8(eq);
		const bool allEq = (mask == 0xFFFF);
		return allEq;
	}

	static bool IsGe(SimdType inLHS, SimdType inRHS)
	{
		const auto lt = _mm_cmplt_epi32(inLHS, inRHS);
		const auto mask = _mm_movemask_epi8(lt);
		const bool allGe = (mask == 0x0000); // Note: inverted logic, !LT == GE
		return allGe;
	}

	static bool IsLe(SimdType inLHS, SimdType inRHS)
	{
		const auto gt = _mm_cmpgt_epi32(inLHS, inRHS);
		const auto mask = _mm_movemask_epi8(gt);
		const bool allLe = (mask == 0x0000); // Note; inverted logic, !GT == LE
		return allLe;
	}

	//static SimdType RoundNearest(SimdType inRound)
	// Not Implemented for integers

	/// @brief Find the minimum value for each pair of element of SimdType
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return the minimum value for each pair of element of SimdType
	static SimdType Min(SimdType inLHS, SimdType inRHS)
	{
		auto min = _mm_min_epi32(inLHS, inRHS);
		return min;
	}

	/// @brief Find the maximum value for each pair of element of SimdType
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return the maximum value for each pair of element of SimdType
	static SimdType Max(SimdType inLHS, SimdType inRHS)
	{
		auto max = _mm_max_epi32(inLHS, inRHS);
		return max;
	}

	/// @brief Find the minimum/maximum values for each pair ofelement of SimdType
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return the minimum/maximum values for each pair of element of SimdType
	static SimdType MinMax(SimdType inLHS, SimdType inRHS)
	{
		auto min = _mm_min_epi32(inLHS, inRHS); // elements 0 and 1
		auto max = _mm_max_epi32(inLHS, inRHS); // elements 2 and 3
		// use _mm_blend_epi16 to blend the 32 bit min and max values
		auto minMax = _mm_blend_epi16(min, max, 0xF0);
		return minMax;
	}

	/// @brief Find the maximum/minimum values for each pair ofelement of SimdType
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return the maximum/minimum values for each pair of element of SimdType
	static SimdType MaxMin(SimdType inLHS, SimdType inRHS)
	{
		auto max = _mm_max_epi32(inLHS, inRHS); // elements 0 and 1
		auto min = _mm_min_epi32(inLHS, inRHS); // elements 2 and 3
		// use _mm_blend_epi16 to blend the 32 bit min and max values
		auto maxMin = _mm_blend_epi16(max, min, 0xF0);
		return maxMin;
	}
};

// float
template<>
struct Simd128<float> :
    public Simd128Traits<float> // is-a: Simd128Traits<float>
{
	using typename Simd128Traits<float>::SimdType; // __m128
	using typename Simd128Traits<float>::ValueType; // float

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

	/// @brief Duplicate the low half of the SIMD register into both halves.
    /// @param inSimd Input SIMD register.
    /// @return SIMD register with low half duplicated.
	static SimdType DupLo(SimdType inSimd)
	{
		auto dup = _mm_shuffle_ps(inSimd, inSimd, _MM_SHUFFLE(1, 0, 1, 0));
		return dup;
	}

	/// @brief Duplicate the high half of the SIMD register into both halves.
    /// @param inSimd Input SIMD register.
    /// @return SIMD register with high half duplicated.
	static SimdType DupHi(SimdType inSimd)
	{
		auto dup = _mm_shuffle_ps(inSimd, inSimd, _MM_SHUFFLE(3, 2, 3, 2));
		return dup;
	}

	/// @brief Compare two vector<float> values to check if all elements equal or inexactly equal.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return true if corresponding elements are equal, false otherwise
	static bool IsEq(SimdType inLHS, SimdType inRHS)
	{
		// Create a vector mask to remove the sign bit for floats so that we can take the absolute value of a vector of floats
		constexpr auto signMask = ~(1U << (sizeof(float) * 8 - 1)); // Most significant bit of the float = sign bit (0x7FFFFFFF)
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

	static bool IsGe(SimdType inLHS, SimdType inRHS)
	{
		// Compare each inLHS with each inRHS to see if inLHS is 'exactly' greater than or equal
		const auto ge = _mm_cmpge_ps(inLHS, inRHS);
		const auto mask = _mm_movemask_ps(ge); // Convert _m128 to a binary mask
		bool allGe = (mask == 0xF); // if mask == 0b1111, all are 'exactly' greater than or equal
		if (!allGe)
		{
			// We are testing if LHS is greater than OR equal to RHS.
			// If an element is not 'exactly' greater than or equal, than we must check if it is 'inexactly' equal
			// If an element is 'exactly' greater than or equal, there is no need to check if it is 'inexactly' equal
			// Create a new lhs that eliminates from testing the elements that are already exactly greater than or equal.
			// We swap out LHS 'greater than' elements the corresponding RHS element, leaving the LHS elements that weren't greater than
			// Ex: LHS = [5.0, 1.0, 1.1, 1.1], RHS = [1.1, 1.1, 1.1, 1.1]. New LHS = [1.1, 1.0, 1.1, 1.1]
			const auto lhs = _mm_blendv_ps(inLHS, inRHS, ge); 
			allGe = IsEq(lhs, inRHS);
		}
		return allGe;
	}

	static bool IsLe(SimdType inLHS, SimdType inRHS)
	{
		// The cheap test
		const auto le = _mm_cmple_ps(inLHS, inRHS);
		const auto mask = _mm_movemask_ps(le);
		bool allLe = (mask == 0xF);
		// The expensive test
		if (!allLe)
		{
			// We are testing if LHS is less than OR equal to RHS.
			// If an element is not 'exactly' less than or equal, than we must check if it is 'inexactly' equal
			// If an element is 'exactly' less than or equal, there is no need to check if it is 'inexactly' equal
			// Create a new lhs that eliminates from testing the elements that are already exactly less than or equal.
			// We swap out LHS 'less than' elements the corresponding RHS element, leaving the LHS elements that weren't less than
			// Ex: LHS = [5.0, 1.0, 1.1, 1.1], RHS = [1.1, 1.1, 1.1, 1.1]. New LHS = [1.1, 1.0, 1.1, 1.1]
			const auto lhs = _mm_blendv_ps(inLHS, inRHS, le); 
			allLe = IsEq(lhs, inRHS);
		}
		return allLe;
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

	/// @brief Find the minimum value for each pair of element of SimdType
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return the minimum value for each pair of element of SimdType
	static SimdType Min(SimdType inLHS, SimdType inRHS)
	{
		auto min = _mm_min_ps(inLHS, inRHS);
		return min;
	}

	/// @brief Find the maximum value for each pair of element of SimdType
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return the maximum value for each pair of element of SimdType
	static SimdType Max(SimdType inLHS, SimdType inRHS)
	{
		auto max = _mm_max_ps(inLHS, inRHS);
		return max;
	}

	/// @brief Find the minimum/maximum values for each pair ofelement of SimdType
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return the minimum/maximum values for each pair of element of SimdType
	static SimdType MinMax(SimdType inLHS, SimdType inRHS)
	{
		auto min = _mm_min_ps(inLHS, inRHS); // elements 0 and 1
		auto max = _mm_max_ps(inLHS, inRHS); // elements 2 and 3
		// use _mm_blend_ps to blend the 32 bit min and max values
		auto minMax = _mm_blend_ps(min, max, 0x0C);
		return minMax;
	}

	/// @brief Find the maximum/minimum values for each pair ofelement of SimdType
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return the maximum/minimum values for each pair of element of SimdType
	static SimdType MaxMin(SimdType inLHS, SimdType inRHS)
	{
		auto max = _mm_max_ps(inLHS, inRHS); // elements 0 and 1
		auto min = _mm_min_ps(inLHS, inRHS); // elements 2 and 3
		// use _mm_blend_ps to blend the 32 bit min and max values
		auto maxMin = _mm_blend_ps(max, min, 0x0C);
		return maxMin;
	}
};

// double
template<>
struct Simd128<double> :
    public Simd128Traits<double> // is-a: Simd128Traits<double>
{
	using typename Simd128Traits<double>::SimdType; // __m128d
	using typename Simd128Traits<double>::ValueType; // double

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

	/// @brief Duplicate the low half of the SIMD register into both halves.
    /// @param inSimd Input SIMD register.
    /// @return SIMD register with low half duplicated.
	static SimdType DupLo(SimdType inSimd)
	{
		auto dup = _mm_shuffle_pd(inSimd, inSimd, 0x0);
		return dup;
	}

	/// @brief Duplicate the high half of the SIMD register into both halves.
    /// @param inSimd Input SIMD register.
    /// @return SIMD register with high half duplicated.
	static SimdType DupHi(SimdType inSimd)
	{
		auto dup = _mm_shuffle_pd(inSimd, inSimd, 0x1);
		return dup;
	}

	/// @brief Compare two vector<double> values to check if all elements equal.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return true if corresponding elements are equal, false otherwise
	static bool IsEq(SimdType inLHS, SimdType inRHS)
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

	static bool IsGe(SimdType inLHS, SimdType inRHS)
	{
		// Compare each inLHS with each inRHS to see if inLHS is 'exactly' greater than or equal
		const auto ge = _mm_cmpge_pd(inLHS, inRHS);
		const auto mask = _mm_movemask_pd(ge); // Convert _m128d to a binary mask
		bool allGe = (mask == 0x3); // if mask == 0b0011, all are 'exactly' greater than or equal
		if (!allGe)
		{
			// We are testing if LHS is greater than OR equal to RHS.
			// If an element is not 'exactly' greater than or equal, than we must check if it is 'inexactly' equal
			// If an element is 'exactly' greater than or equal, there is no need to check if it is 'inexactly' equal
			// Create a new lhs that eliminates from testing the elements that are already exactly greater than or equal.
			// We swap out LHS 'greater than' elements the corresponding RHS element, leaving the LHS elements that weren't greater than
			// Ex: LHS = [5.0, 1.0], RHS = [1.1, 1.1]. New LHS = [1.1, 1.0]
			const auto lhs = _mm_blendv_pd(inLHS, inRHS, ge); 
			allGe = IsEq(lhs, inRHS);
		}
		return allGe;
	}

	static bool IsLe(SimdType inLHS, SimdType inRHS)
	{
		// Compare each inLHS with each inRHS to see if inLHS is 'exactly' greater than or equal
		const auto le = _mm_cmple_pd(inLHS, inRHS);
		const auto mask = _mm_movemask_pd(le); // Convert _m128d to a binary mask
		bool allLe = (mask == 0x3); // if mask == 0b0011, all are 'exactly' greater than or equal
		if (!allLe)
		{
			// We are testing if LHS is less than OR equal to RHS.
			// If an element is not 'exactly' less than or equal, than we must check if it is 'inexactly' equal
			// If an element is 'exactly' less than or equal, there is no need to check if it is 'inexactly' equal
			// Create a new lhs that eliminates from testing the elements that are already exactly less than or equal.
			// We swap out LHS 'less than' elements the corresponding RHS element, leaving the LHS elements that weren't less than
			// Ex: LHS = [5.0, 1.0], RHS = [1.1, 1.1]. New LHS = [5.0, 1.1]
			const auto lhs = _mm_blendv_pd(inLHS, inRHS, le); 
			allLe = IsEq(lhs, inRHS);
		}
		return allLe;
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


	/// @brief Find the minimum value for each pair of element of SimdType
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return the minimum value for each pair of element of SimdType
	static SimdType Min(SimdType inLHS, SimdType inRHS)
	{
		auto min = _mm_min_pd(inLHS, inRHS);
		return min;
	}

	/// @brief Find the maximum value for each pair of element of SimdType
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return the maximum value for each pair of element of SimdType
	static SimdType Max(SimdType inLHS, SimdType inRHS)
	{
		auto max = _mm_max_pd(inLHS, inRHS);
		return max;
	}

	/// @brief Find the minimum/maximum values for each pair ofelement of SimdType
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return the minimum/maximum values for each pair of element of SimdType
	static SimdType MinMax(SimdType inLHS, SimdType inRHS)
	{
		auto min = _mm_min_pd(inLHS, inRHS); // elements 0 and 1
		auto max = _mm_max_pd(inLHS, inRHS); // elements 2 and 3
		// use _mm_blend_pd to blend the 64 bit min and max values
		auto minMax = _mm_blend_pd(min, max, 0x02);
		return minMax;
	}

	/// @brief Find the maximum/minimum values for each pair ofelement of SimdType
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return the maximum/minimum values for each pair of element of SimdType
	static SimdType MaxMin(SimdType inLHS, SimdType inRHS)
	{
		auto max = _mm_max_pd(inLHS, inRHS); // elements 0 and 1
		auto min = _mm_min_pd(inLHS, inRHS); // elements 2 and 3
		// use _mm_blend_pd to blend the 64 bit min and max values
		auto maxMin = _mm_blend_pd(max, min, 0x02);
		return maxMin;
	}
};

} // namespace saber::geometry::detail

#endif // SABER_GEOMETRY_DETAIL_SIMD_SSE_HPP
