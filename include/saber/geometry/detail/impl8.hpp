#ifndef SABER_GEOMETRY_DETAIL_IMPL8_HPP
#define SABER_GEOMETRY_DETAIL_IMPL8_HPP

// std
#include <array>
#include <tuple>
#include <type_traits>

// saber
#include "saber/inexact.hpp"
#include "saber/geometry/detail/impl2.hpp"
#include "saber/geometry/detail/impl4.hpp"
#include "saber/geometry/detail/simd.hpp"

namespace saber::geometry::detail {

template<typename T>
struct Impl8 final
{	
	class Simd;

	class Scalar
	{
	public:
		// default ctor
		constexpr Scalar() = default;
		~Scalar() = default;

		// alt ctor
		constexpr Scalar(T inFirst, T inSecond, T inThird, T inFourth, T inFifth, T inSixth, T inSeventh, T inEigth) :
			mArray{inFirst, inSecond, inThird, inFourth, inFifth, inSixth, inSeventh, inEigth}
		{
			// Do nothing
		}

		constexpr Scalar(const typename Impl2<T>::Scalar& inFirst, const typename Impl2<T>::Scalar& inSecond, const typename Impl2<T>::Scalar& inThird, const typename Impl2<T>::Scalar& inFourth) :
			mArray{inFirst.Get<0>(), inFirst.Get<1>(), inSecond.Get<0>(), inSecond.Get<1>(), inThird.Get<0>(), inThird.Get<1>(), inFourth.Get<0>(), inFourth.Get<1>()}
		{
			// Do nothing
		}

		constexpr Scalar(const typename Impl4<T>::Scalar& inFirst, const typename Impl4<T>::Scalar& inSecond) :
			mArray{inFirst.Get<0>(), inFirst.Get<1>(), inFirst.Get<2>(), inFirst.Get<3>(), inSecond.Get<0>(), inSecond.Get<1>(), inSecond.Get<2>(), inSecond.Get<3>()}
		{
			// Do nothing
		}

		template<std::size_t Index>
		constexpr const T& Get() const
		{
			static_assert(Index < std::tuple_size_v<decltype(mArray)>, "Provided index out of bounds.");
			return mArray.at(Index);
		}

		template<std::size_t Index>
		constexpr T& Get()
		{
			static_assert(Index < std::tuple_size_v<decltype(mArray)>, "Provided index out of bounds.");
			return mArray.at(Index);
		}

		constexpr Scalar& operator+=(const Scalar& inRHS)
		{
			Get<0>() += inRHS.Get<0>();
			Get<1>() += inRHS.Get<1>();
			Get<2>() += inRHS.Get<2>();
			Get<3>() += inRHS.Get<3>();
			Get<4>() += inRHS.Get<4>();
			Get<5>() += inRHS.Get<5>();
			Get<6>() += inRHS.Get<6>();
			Get<7>() += inRHS.Get<7>();
			return *this;
		}

		constexpr Scalar& operator-=(const Scalar& inRHS)
		{
			Get<0>() -= inRHS.Get<0>();
			Get<1>() -= inRHS.Get<1>();
			Get<2>() -= inRHS.Get<2>();
			Get<3>() -= inRHS.Get<3>();
			Get<4>() -= inRHS.Get<4>();
			Get<5>() -= inRHS.Get<5>();
			Get<6>() -= inRHS.Get<6>();
			Get<7>() -= inRHS.Get<7>();
			return *this;
		}

		constexpr Scalar& operator*=(const Scalar& inRHS)
		{
			Get<0>() *= inRHS.Get<0>();
			Get<1>() *= inRHS.Get<1>();
			Get<2>() *= inRHS.Get<2>();
			Get<3>() *= inRHS.Get<3>();
			Get<4>() *= inRHS.Get<4>();
			Get<5>() *= inRHS.Get<5>();
			Get<6>() *= inRHS.Get<6>();
			Get<7>() *= inRHS.Get<7>();
			return *this;
		}

		constexpr bool IsEqual(const Scalar& inRHS) const
		{
			// Point said to copy/paste this section into the scalar portion
			bool result = false;
			if constexpr (std::is_floating_point_v<T>)
			{   
				// Floating point comparisons are always inexact within an epsilon
				result = Inexact::IsEq(Get<0>(), inRHS.Get<0>()) 
							&& Inexact::IsEq(Get<1>(), inRHS.Get<1>())
							&& Inexact::IsEq(Get<2>(), inRHS.Get<2>())
							&& Inexact::IsEq(Get<3>(), inRHS.Get<3>())
							&& Inexact::IsEq(Get<4>(), inRHS.Get<4>())
							&& Inexact::IsEq(Get<5>(), inRHS.Get<5>())
							&& Inexact::IsEq(Get<6>(), inRHS.Get<6>())
							&& Inexact::IsEq(Get<7>(), inRHS.Get<7>());
			}
			else
			{
				// Integer comparisons are always exact
				result = (Get<0>() == inRHS.Get<0>()) 
							&& (Get<1>() == inRHS.Get<1>())
							&& (Get<2>() == inRHS.Get<2>())
							&& (Get<3>() == inRHS.Get<3>())
							&& (Get<4>() == inRHS.Get<4>())
							&& (Get<5>() == inRHS.Get<5>())
							&& (Get<6>() == inRHS.Get<6>())
							&& (Get<7>() == inRHS.Get<7>());
			}
			return result;
		}
		
	private:
		//friend class Simd; // Permit Simd class to provide constexpr api

		template<typename T, ImplKind Impl>
		friend constexpr Scalar& MulMatrix(Scalar& ioLHS, const Scalar& inRHS)
		{
			// NOTE: 3x2 Matrixes only. We treat inRHS as if it were Transpose 2x3
			T m11 = (ioLHS.Get<0>() * inRHS.Get<0>()) + (ioLHS.Get<1>() * inRHS.Get<3>());
			T m12 = (ioLHS.Get<0>() * inRHS.Get<1>()) + (ioLHS.Get<1>() * inRHS.Get<4>());
			T m21 = (ioLHS.Get<3>() * inRHS.Get<0>()) + (ioLHS.Get<4>() * inRHS.Get<3>());
			T m22 = (ioLHS.Get<3>() * inRHS.Get<1>()) + (ioLHS.Get<4>() * inRHS.Get<4>());
			
			T m13 = ((ioLHS.Get<0>() * inRHS.Get<2>()) + (ioLHS.Get<1>() * inRHS.Get<5>()) + (ioLHS.Get<2>()));
			T m23 = ((ioLHS.Get<3>() * inRHS.Get<2>()) + (ioLHS.Get<4>() * inRHS.Get<5>()) + (ioLHS.Get<5>()));

			ioLHS.Get<0>() = m11;
			ioLHS.Get<1>() = m12;
			ioLHS.Get<2>() = m13;
			ioLHS.Get<3>() = m21;
			ioLHS.Get<4>() = m22;
			ioLHS.Get<5>() = m23;

			return ioLHS;
		}

		template<typename T, ImplKind Impl>
		friend constexpr Scalar& InvMatrix(Scalar& ioLHS)
		{
			const T det = ((ioLHS.Get<0>() * ioLHS.Get<4>()) - (ioLHS.Get<1>() * ioLHS.Get<3>()));
			const bool isNotInvertible = Inexact<T>::IsEq(det, 0);
			if (isNotInvertible)
			{
				throw std::runtime_error("Saber: Matrix is not invertible");
			}
			const T invDet = (1/det);

			T m11 = (ioLHS.Get<4>() * invDet);
			T m12 = -(ioLHS.Get<1>() * invDet);
			T m21 = -(ioLHS.Get<3>() * invDet);
			T m22 = (ioLHS.Get<0>() * invDet);
			
			T m13 = -((m11 * ioLHS.Get<2>()) + (m12 * ioLHS.Get<5>()));
			T m23 = -((m21 * ioLHS.Get<2>()) + (m22 * ioLHS.Get<5>()));

			ioLHS.Get<0>() = m11;
			ioLHS.Get<1>() = m12;
			ioLHS.Get<2>() = m13;
			ioLHS.Get<3>() = m21;
			ioLHS.Get<4>() = m22;
			ioLHS.Get<5>() = m23;

			return ioLHS;
		}

	private:
		std::array<T, 8> mArray; // Impl8: so 8 elements are assumed
	}; // struct Scalar

	// TODO mnfitz 22dec2025: Add SIMD implementation
}; // struct Impl8<>

#pragma region struct Impl8Traits
template<typename T, ImplKind Impl> // Primary template declaration
struct Impl8Traits;

template<typename T> // Partial template specialization
struct Impl8Traits<T, ImplKind::kScalar>
{
	using ImplType = typename Impl8<T>::Scalar; // VOODOO: Nested template type requires `typename` prefix
};

template<typename T> // Partial template specialization
struct Impl8Traits<T, ImplKind::kSimd>
{
	//using ImplType = typename Impl8<T>::Simd; // VOODOO: Nested template type requires `typename` prefix
	using ImplType = typename Impl8<T>::Scalar; // VOODOO: Nested template type requires `typename` prefix
};

#pragma endregion

} // namespace saber::geometry::detail

#endif // SABER_GEOMETRY_DETAIL_IMPL8_HPP