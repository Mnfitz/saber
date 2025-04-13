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

	/// @brief Check all vector type`<T>` elements in `inRHS` to `inLHS` for equality.
	/// @param inLHS Left hand side vector term
	/// @param inRHS Right hand side vector term
	/// @return Return true if all corresponding vector elements are equal
	static constexpr bool IsEq(SimdType inLHS, SimdType inRHS)
	{
		bool isEq = true;
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize; ++i)
		{
			if (inRHS[i] != inLHS[i])
			{
				isEq = false;
				break;
			}
		}
		return isEq;
	}

	/// @brief Round all elements of SimdType to the nearest whole number 
	/// @param inRound The SimdType to be rounded
	/// @return Return the rounded result
	static constexpr SimdType RoundNearest(SimdType inRound)
	{
		for (std::size_t i = 0; i < Simd128Traits<T>::kSize; ++i)
		{
			inRound[i] = std::round(inRound[i]);
		}
		return inRound;
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
