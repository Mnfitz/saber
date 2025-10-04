#ifndef SABER_GEOMETRY_DETAIL_SIMD_HPP
#define SABER_GEOMETRY_DETAIL_SIMD_HPP

// saber
#include "saber/geometry/config.hpp"
#include "saber/geometry/detail/simd_traits.hpp"

namespace saber::geometry::detail {

template<typename T> // Primary template definition
struct Simd128;

/// @brief Platform independent API for 128bit SIMD operations.
///
/// Implemented as a "partial" template specialization for `<NBits=128>`.
/// (e.g: 128bits = 4x 32bit elements; or 2x 64bit elements).
///
/// For T=`int`:
///     |   0-31   |   32-63  |   64-95  |  96-127  |
///     |----------|----------|----------|----------|
///     |  int[0]  |  int[1]  |  int[2]  |  int[3]  |
///
/// For T=`double`: 
///     |        0-63         |       64-127        |
///     |---------------------|---------------------|
///     |      double[0]      |      double[1]      |
///
/// @tparam T Underlying type of element of a SIMD vector
template<typename T>
struct Simd128 :
	public Simd128Traits<T> // is-a: Simd128Traits<T>
{
	using typename Simd128Traits<T>::SimdType; // Expose `SimdType` as our own
	using typename Simd128Traits<T>::ValueType; // Expose `ValueType` as our own

	/// @brief Load 4 elements of type`<T>` from memory specified by `inAddr`.
	/// @code{.cpp}
	/// SimdType[0] = inAddr[0];
	/// SimdType[1] = inAddr[1];
	/// SimdType[2] = inAddr[2];
	/// SimdType[3] = inAddr[3];
	/// return SimdType;
	/// @endcode
	/// @param inAddr Address of &elements[4] to load
	/// @return Vector type`<T>` of loaded elements
	static constexpr SimdType Load4(const T* inAddr)
	{
		static_assert(sizeof(SimdType) >= 4*sizeof(T) , "4 elements of type<T> are too large to fit in 128bit SimdType");
		SimdType load4{}; // zero
		load4[0] = inAddr[0];
		load4[1] = inAddr[1];
		load4[2] = inAddr[2];
		load4[3] = inAddr[3];
		return load4;
	}

	/// @brief Load 2 elements of type`<T>` from memory specified by `inAddr`.
	/// Loaded elements are placed in low order position in result SimdType.
	/// Any unused high order elements are set to zero.
	/// @code{.cpp}
	/// SimdType[0] = inAddr[0];
	/// SimdType[1] = inAddr[1];
	/// SimdType[2..MAX] = 0;
	/// return SimdType;
	/// @endcode
	/// @param inAddr Address of &elements[2] to load
	/// @return Vector type`<T>` of loaded elements
	static constexpr SimdType Load2(const T* inAddr)
	{
		SimdType load2{}; // zero
		load2[0] = inAddr[0];
		load2[1] = inAddr[1];
		return load2;
	}

	/// @brief Load 1 element of type`<T>` from memory specified by `inAddr`.
	/// Loaded element is placed in low order position in result SimdType.
	/// Any unused high order elements are set to zero.
	/// @code{.cpp}
	/// SimdType[0] = inAddr[0];
	/// SimdType[1..MAX] = 0;
	/// return SimdType;
	/// @endcode
	/// @param inAddr Address of &element[1] to load
	/// @return Vector type`<T>` of loaded elements
	static constexpr SimdType Load1(const T* inAddr)
	{
		SimdType load1{}; // zero
		load1[0] = inAddr[0];
		return load1;
	}

	/// @brief Store 4 elements of type`<T>` to memory specified by `outAddr`.
	/// @code{.cpp}
	/// outAddr[0] = SimdType[0];
	/// outAddr[1] = SimdType[1];
	/// outAddr[2] = SimdType[2];
	/// outAddr[3] = SimdType[3];
	/// @endcode
	/// @param outAddr Address to store &elements[4]
	/// @param inStore4 Vector type`<T>` of elements to store
	static constexpr void Store4(T* outAddr, SimdType inStore4)
	{
		static_assert(sizeof(SimdType) >= 4*sizeof(T), "128bit SimdType is too small to contain 4 elements of type<T>");
		outAddr[0] = inStore4[0];
		outAddr[1] = inStore4[1];
		outAddr[2] = inStore4[2];
		outAddr[3] = inStore4[3];
	}

	/// @brief Store 2 elements of type`<T>` to memory specified by `outAddr`.
	/// The 2 lowest order elements are stored to memory.
	/// Any high order elements are ignored.
	/// @code{.cpp}
	/// outAddr[0] = SimdType[0];
	/// outAddr[1] = SimdType[1];
	/// (void) SimdType[2..MAX]; // unused
	/// @endcode
	/// @param outAddr Address to store &elements[2]
	/// @param inStore2 Vector type`<T>` of elements to store
	static constexpr void Store2(T* outAddr, SimdType inStore2)
	{
		outAddr[0] = inStore2[0];
		outAddr[1] = inStore2[1];
	}

	/// @brief Store 1 element of type`<T>` to memory specified by `outAddr`.
	/// The lowest order element is stored to memory.
	/// Any higher order elements are ignored.
	/// @code{.cpp}
	/// outAddr[0] = SimdType[0];
	/// (void) SimdType[1..MAX]; // unused
	/// @endcode
	/// @param outAddr Address to store &element[1]
	/// @param inStore1 Vector type`<T>` of elements to store
	static constexpr void Store1(T* outAddr, SimdType inStore1)
	{
		outAddr[0] = inStore1[0];
	}

	/// @brief Add all vector type`<T>` elements in `inRHS` to `inLHS`.
	/// @code{.cpp}
	/// for (i = 0; i < MAX; ++i)
	///     SimdType[i] = inLHS[i] + inRHS[i];
	/// return SimdType;
	/// @endcode
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<T>`
	static constexpr SimdType Add(SimdType inLHS, SimdType inRHS)
	{
		SimdType add{};
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize; ++i)
		{
			add[i] = inLHS[i] + inRHS[i];
		}
		return add;
	}

	/// @brief Subtract all vector type`<T>` elements in `inRHS` from `inLHS`.
	/// @code{.cpp}
	/// for (i = 0; i < MAX; ++i)
	///     SimdType[i] = inLHS[i] - inRHS[i];
	/// return SimdType;
	/// @endcode
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<T>`
	static constexpr SimdType Sub(SimdType inLHS, SimdType inRHS)
	{
		SimdType sub{};
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize; ++i)
		{
			sub[i] = inLHS[i] - inRHS[i];
		}
		return sub;
	}

	/// @brief Multiply all vector type`<T>` elements in `inRHS` to `inLHS`.
	/// @code{.cpp}
	/// for (i = 0; i < MAX; ++i)
	///     SimdType[i] = inLHS[i] * inRHS[i];
	/// return SimdType;
	/// @endcode
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<T>`
	static constexpr SimdType Mul(SimdType inLHS, SimdType inRHS)
	{
		SimdType mul{};
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize; ++i)
		{
			mul[i] = inLHS[i] * inRHS[i];
		}
		return mul;
	}

	/// @brief Divide all vector type`<T>` elements in `inRHS` from `inLHS`.
	/// @code{.cpp}
	/// for (i = 0; i < MAX; ++i)
	///     SimdType[i] = inLHS[i] / inRHS[i];
	/// return SimdType;
	/// @endcode
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Result vector type`<T>`
	static constexpr SimdType Div(SimdType inLHS, SimdType inRHS)
	{
		SimdType div{};
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize; ++i)
		{
			div[i] = (inRHS[i] != 0 ? (inLHS[i] / inRHS[i]) : 0);
		}
		return div;
	}

	// DupLo
	static constexpr SimdType DupLo(SimdType inSimd)
	{
		// DupLo(0123) = 0101;
		// DupLo(45) = 44;
		SimdType dup = inSimd;
		constexpr std::size_t hi = Simd128Traits<T>::kSize/2;
		for (std::size_t lo = 0; lo < hi; ++lo)
		{
			// The Hi elements are copies of the Lo
			dup[hi + lo] = dup[lo];
		}
		return dup;
	}

	// DupHi
	static constexpr SimdType DupHi(SimdType inSimd)
	{
		// DupHi(0123) = 2323;
		// DupHi(45) = 55;
		SimdType dup = inSimd;
		constexpr std::size_t hi = Simd128Traits<T>::kSize/2;
		for (std::size_t lo = 0; lo < hi; ++lo)
		{
			// The Lo elements are copies of the Hi
			dup[lo] = dup[hi + lo];
		}
		return dup;
	}

	/// @brief Check all vector type`<T>` elements in `inRHS` to `inLHS` for equality.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return true if all corresponding vector elements are equal
	static constexpr bool IsEq(SimdType inLHS, SimdType inRHS)
	{
		bool isEq = true;
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize; ++i)
		{
			// check for "exact" equality
			if (inRHS[i] == inLHS[i])
			{
				continue;
			}

			// check for "inexact" equality for floating point types
			if constexpr(std::is_floating_point_v<T>)
			{
				if (Inexact::Eq(inRHS[i], inLHS[i]))
				{
					continue;
				}
			}

			isEq = false;
			break;
		}
		return isEq;
	}

	/// @brief Check all vector type`<T>` elements in `inRHS` to `inLHS` for equality.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return mask of all corresponding vector elements that are equal
	static constexpr int IsEq2(SimdType inLHS, SimdType inRHS)
	{
		int eqMask = 0;
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize; ++i)
		{
			// check for "exact" equality
			if (inRHS[i] == inLHS[i])
			{
				eqMask |= (1 << i);
				continue;
			}

			// check for "inexact" equality for floating point types
			if constexpr(std::is_floating_point_v<T>)
			{
				if (Inexact::Eq(inRHS[i], inLHS[i]))
				{
					eqMask |= (1 << i);
					continue;
				}
			}
		}
		return eqMask;
	}

	static constexpr bool IsGe(SimdType inLHS, SimdType inRHS)
	{
		bool isGe = true;
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize; ++i)
		{
			// check for "exact" greater than
			if (inLHS[i] >= inRHS[i])
			{
				continue;
			}
			
			// check for "inexact" equality for floating point types
			if constexpr(std::is_floating_point_v<T>)
			{
				if (Inexact::Eq(inLHS[i], inRHS[i]))
				{
					continue;
				}
			}

			isGe = false;
			break;
		}
		return isGe;
	}
	static constexpr int IsGe2(SimdType inLHS, SimdType inRHS)
	{
		int geMask = 0;
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize; ++i)
		{
			// check for "exact" greater than
			if (inLHS[i] >= inRHS[i])
			{
				geMask |= (1 << i);
				continue;
			}
			
			// check for "inexact" equality for floating point types
			if constexpr(std::is_floating_point_v<T>)
			{
				if (Inexact::Eq(inLHS[i], inRHS[i]))
				{
					geMask |= (1 << i);
					continue;
				}
			}

			break;
		}
		return geMask;
	}

	static constexpr bool IsLe(SimdType inLHS, SimdType inRHS)
	{
		bool isLe = true;
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize; ++i)
		{
			// check for "exact" greater than
			if (inLHS[i] <= inRHS[i])
			{
				continue;
			}
			
			// check for "inexact" equality for floating point types
			if constexpr(std::is_floating_point_v<T>)
			{
				if (Inexact::Eq(inLHS[i], inRHS[i]))
				{
					continue;
				}
			}

			isLe = false;
			break;
		}
		return isLe;
	}
	static constexpr int IsLe2(SimdType inLHS, SimdType inRHS)
	{
		int leMask = 0;
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize; ++i)
		{
			// check for "exact" greater than
			if (inLHS[i] <= inRHS[i])
			{
				leMask |= (1 << i);
				continue;
			}
			
			// check for "inexact" equality for floating point types
			if constexpr(std::is_floating_point_v<T>)
			{
				if (Inexact::Eq(inLHS[i], inRHS[i]))
				{
					leMask |= (1 << i);
					continue;
				}
			}

			break;
		}
		return leMask;
	}

	static constexpr bool IsGt(SimdType inLHS, SimdType inRHS)
	{
		bool isGt = !IsLe(inRHS, inLHS);
		return isGt;
	}
	static constexpr bool IsGt2(SimdType inLHS, SimdType inRHS)
	{
		int leMask = IsLe2(inRHS, inLHS);
		int gtMask = leMask ^ ((1U << Simd128Traits<T>::kSize)-1);
		return gtMask;
	}

	static constexpr bool IsLt(SimdType inLHS, SimdType inRHS)
	{
		bool isLt = !IsGe(inRHS, inLHS);
		return isLt;
	}
	static constexpr int IsLt2(SimdType inLHS, SimdType inRHS)
	{
		int geMask = IsGe2(inRHS, inLHS);
		int ltMask = geMask ^ ((1U << Simd128Traits<T>::kSize)-1);
		return ltMask;
	}

	/// @brief Round all elements of SimdType toward positive infinity 
	/// @param inRound The SimdType to be rounded
	/// @return Return the rounded result
	static constexpr SimdType RoundCeil(SimdType inRound)
	{
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize; ++i)
		{
			inRound[i] = std::ceil(inRound[i]);
		}
		return inRound;
	}

	/// @brief Round all elements of SimdType toward negative infinity 
	/// @param inRound The SimdType to be rounded
	/// @return Return the rounded result
	static constexpr SimdType RoundFloor(SimdType inRound)
	{
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize; ++i)
		{
			inRound[i] = std::floor(inRound[i]);
		}
		return inRound;
	}

	/// @brief Round all elements of SimdType toward zero
	/// @param inRound The SimdType to be rounded
	/// @return Return the rounded result
	static constexpr SimdType RoundTrunc(SimdType inRound)
	{
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize; ++i)
		{
			inRound[i] = std::trunc(inRound[i]);
		}
		return inRound;
	}

	/// @brief Find the minimum/maximum values for each pair ofelement of SimdType
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return the minimum/maximum values for each pair of element of SimdType
	static constexpr SimdType MinMax(SimdType inLHS, SimdType inRHS)
	{
		// Make sure the SimdType is even
		static_assert(Simd128Traits<T>::kSize & 1 == 0, "Number of SimdType elements must be even");

		constexpr auto kMin = 0;
		constexpr auto kMax = Simd128Traits<T>::kSize/2;
		SimdType minMax{};
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize/2; i++)
		{
			minMax[i+kMin] = std::min(inLHS[i+kMin], inRHS[i+kMin]);
			minMax[i+kMax] = std::max(inLHS[i+kMax], inRHS[i+kMax]);
		}
		return minMax;
	}

	/// @brief Find the maximum/minimum values for each pair ofelement of SimdType
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return the maximum/minimum values for each pair of element of SimdType
	static constexpr SimdType MaxMin(SimdType inLHS, SimdType inRHS)
	{
		// Make sure the SimdType is even
		static_assert(Simd128Traits<T>::kSize & 1 == 0, "Number of SimdType elements must be even");

		constexpr auto kMin = 0;
		constexpr auto kMax = Simd128Traits<T>::kSize/2;
		SimdType maxMin{};
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize/2; i++)
		{
			maxMin[i+kMin] = std::max(inLHS[i+kMin], inRHS[i+kMin]);
			maxMin[i+kMax] = std::min(inLHS[i+kMax], inRHS[i+kMax]);
		}
		return maxMin;
	}

}; // struct Simd128<T>

} // namespace saber::geometry::detail

#if SABER_GEOMETRY_CONFIG_ISENABLED_SIMD

// If `saber::geometery` is enabled for SIMD...
// Include platform-specific SIMD template specializations here

#if SABER_CPU(X86)
#include "saber/geometry/detail/simd_sse.hpp"
#elif SABER_CPU(ARM)
#include "saber/geometry/detail/simd_neon.hpp"
// #elif SABER_CPU(PPC)
// #include "saber/geometry/detail/simd_altivec.hpp"
// #elif SABER_CPU(RISCV)
// #include "saber/geometry/detail/simd_rvv.hpp"
#endif // SABER_CPU()
#endif // SABER_GEOMETRY_CONFIG_ISENABLED_SIMD

#endif // SABER_GEOMETRY_DETAIL_SIMD_HPP
