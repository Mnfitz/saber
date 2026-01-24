#ifndef SABER_GEOMETRY_DETAIL_IMPL8_HPP
#define SABER_GEOMETRY_DETAIL_IMPL8_HPP

// saber
#include "saber/inexact.hpp"
#include "saber/geometry/config.hpp"
#include "saber/geometry/detail/impl2.hpp"
#include "saber/geometry/detail/impl4.hpp"

// std
#include <array>
#include <type_traits>

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

	private:
		std::array<T, 8> mArray; // Impl8: so 8 elements are assumed
	}; // struct Scalar

	// TODO mnfitz 22dec2025: Add SIMD implementation

}; // struct Impl8<>

template <typename T, ImplKind Impl>
class Helper; // primary template class Helper<T>

template <typename T>
class Helper<T, ImplKind::kScalar>
{
	constexpr typename Impl8<T>::Scalar MatrixIdentity()
	{
		return { 1, 0, 0,
				 0, 1, 0,
				 0, 0 };
	}

	constexpr typename Impl8<T>::Scalar MatrixZero()
	{
		return { 0, 0, 0,
				 0, 0, 0,
				 0, 0 };
	}

	constexpr typename Impl8<T>::Scalar MatrixScale(T inX, T inY)
	{
		return { inX, 0, 0,
				0, inY, 0,
				0, 0 };
	}

	constexpr typename Impl8<T>::Scalar MatrixScale(const typename Impl2<T>::Scalar& inImpl2)
	{
		return { inImpl2.Get<0>(), 0, 0,
				0, inImpl2.Get<1>(), 0,
				0, 0 };
	}

	constexpr typename Impl8<T>::Scalar MatrixTranslation(T inX, T inY)
	{
		return { 1, 0, inX,
				0, 1, inY,
				0, 0 };
	}

	constexpr typename Impl8<T>::Scalar MatrixTranslation(const typename Impl2<T>::Scalar& inImpl2)
	{
		return { 1, 0, inImpl2.Get<0>(),
				0, 1, inImpl2.Get<1>(),
				0, 0 };
	}

	typename Impl8<T>::Scalar MatrixRotation(T inRads)
	{
		const T sin = std::sin(inRads);
		const T cos = std::cos(inRads);
		return { cos, -sin, 0,
				sin, cos, 0,
				0, 0 };
	}

	typename Impl8<T>::Scalar MatrixMul(typename Impl8<T>::Scalar& ioLHS, const typename Impl8<T>::Scalar& inRHS)
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

		ioLHS.Get<6>() = 0;
		ioLHS.Get<7>() = 0;

		return ioLHS;
	}

	typename Impl8<T>::Scalar MatrixInv(typename Impl8<T>::Scalar& ioLHS)
	{
		const T det = ((ioLHS.Get<0>() * ioLHS.Get<4>()) - (ioLHS.Get<1>() * ioLHS.Get<3>()));
		const bool isNotInvertible = Inexact<T>::IsEq(det, 0);
		if (isNotInvertible)
		{
			throw std::runtime_error("Saber: Matrix is not invertible");
		}
		const T invDet = (1 / det);

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

		ioLHS.Get<6>() = 0;
		ioLHS.Get<7>() = 0;

		return ioLHS;
	}

}; // specialized template class Helper<T>

// Primary MatrixIdentity template declaration
template<typename T, ImplKind Impl>
constexpr auto MatrixIdentity()
{
	return Helper<T, Impl>::MatrixIdentity();
}

template<typename T, ImplKind Impl>
constexpr auto MatrixZero()
{
	return Helper<T, Impl>::MatrixZero();
}

template<typename T, ImplKind Impl>
constexpr auto MatrixScale(T inX, T inY)
{
	return Helper<T, Impl>::MatrixScale(inX, inY);
}

template<typename T>
constexpr auto MatrixScale(const typename Impl2<T>::Scalar& inImpl2)
{
	return Helper<T, ImplKind::kScalar>::MatrixScale(inImpl2);
}

template<typename T, ImplKind Impl>
constexpr auto MatrixTranslation(T inX, T inY)
{
	return Helper<T, Impl>::MatrixTranslation(inX, inY);
}

template<typename T>
constexpr auto MatrixTranslation(const typename Impl2<T>::Scalar& inImpl2)
{
	return Helper<T, ImplKind::kScalar>::MatrixTranslation(inImpl2);
}

template<typename T>
constexpr auto MatrixMul(typename Impl8<T>::Scalar& ioLHS, const typename Impl8<T>::Scalar& inRHS)
{
	return Helper<T, ImplKind::kScalar>::MatrixMul(ioLHS, inRHS);
}

template<typename T>
typename auto MatrixInv(typename Impl8<T>::Scalar& ioLHS)
{
	return Helper<T, ImplKind::kScalar>::MatrixInv(ioLHS);
}

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