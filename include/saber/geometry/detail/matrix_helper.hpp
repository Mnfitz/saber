#ifndef SABER_GEOMETRY_DETAIL_MATRIX_HELPER_HPP
#define SABER_GEOMETRY_DETAIL_MATRIX_HELPER_HPP

// saber
#include "saber/inexact.hpp"
#include "saber/geometry/detail/impl8.hpp"

// std
#include <type_traits>

namespace saber::geometry::detail {

template<typename T, ImplKind Impl>
class MatrixHelper; // primary template class Helper<T>

template<typename T>
class MatrixHelper<T, ImplKind::kScalar>
{
public:

	MatrixHelper()
	{
		// Safety check so no one tries to use this with a std::string
		static_assert(std::is_arithmetic_v<T>, "Matrix does not support non-arithmetic types");
	}

	static constexpr typename Impl8<T>::Scalar MatrixIdentity()
	{
		return { 1, 0, 0,
				 0, 1, 0,
				 0, 0 };
	}

	static constexpr typename Impl8<T>::Scalar MatrixZero()
	{
		return { 0, 0, 0,
				 0, 0, 0,
				 0, 0 };
	}

	static constexpr typename Impl8<T>::Scalar MatrixScale(T inX, T inY)
	{
		return { inX, 0, 0,
				0, inY, 0,
				0, 0 };
	}

	static constexpr typename Impl8<T>::Scalar MatrixScale(const typename Impl2<T>::Scalar& inImpl2)
	{
		return { inImpl2.Get<0>(), 0, 0,
				0, inImpl2.Get<1>(), 0,
				0, 0 };
	}

	static constexpr typename Impl8<T>::Scalar MatrixTranslation(T inX, T inY)
	{
		return { 1, 0, inX,
				0, 1, inY,
				0, 0 };
	}

	static constexpr typename Impl8<T>::Scalar MatrixTranslation(const typename Impl2<T>::Scalar& inImpl2)
	{
		return { 1, 0, inImpl2.Get<0>(),
				0, 1, inImpl2.Get<1>(),
				0, 0 };
	}

	static typename Impl8<T>::Scalar MatrixRotation(T inRads)
	{
		const T sin = std::sin(inRads);
		const T cos = std::cos(inRads);
		return { cos, -sin, 0,
				sin, cos, 0,
				0, 0 };
	}

	static typename Impl8<T>::Scalar MatrixMul(typename Impl8<T>::Scalar& ioLHS, const typename Impl8<T>::Scalar& inRHS)
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

	static typename Impl8<T>::Scalar MatrixInv(typename Impl8<T>::Scalar& ioLHS)
	{
		const T det = ((ioLHS.Get<0>() * ioLHS.Get<4>()) - (ioLHS.Get<1>() * ioLHS.Get<3>()));
		const bool isNotInvertible = Inexact::IsEq<T>(det, 0);
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

}; // specialized template class MatrixHelper<T>

// Primary MatrixIdentity template declaration
template<typename T, ImplKind Impl>
constexpr auto MatrixIdentity()
{
	return MatrixHelper<T, Impl>::MatrixIdentity();
}

template<typename T, ImplKind Impl>
constexpr auto MatrixZero()
{
	return MatrixHelper<T, Impl>::MatrixZero();
}

template<typename T, ImplKind Impl>
constexpr auto MatrixScale(T inX, T inY)
{
	return MatrixHelper<T, Impl>::MatrixScale(inX, inY);
}


// REVISIT: Declare Matrix friend to Point and Size, so that we can access the
// Impl2 Scalar directly and avoid this extra copy into Impl8 Scalar.
// Else, use the X Y accessors to construct then pass Impl 2 directly
template<typename T>
constexpr auto MatrixScale(const typename Impl2<T>::Scalar& inImpl2)
{
	return MatrixHelper<T, ImplKind::kScalar>::MatrixScale(inImpl2);
}

template<typename T, ImplKind Impl>
constexpr auto MatrixTranslation(T inX, T inY)
{
	return MatrixHelper<T, Impl>::MatrixTranslation(inX, inY);
}

template<typename T>
constexpr auto MatrixTranslation(const typename Impl2<T>::Scalar& inImpl2)
{
	return MatrixHelper<T, ImplKind::kScalar>::MatrixTranslation(inImpl2);
}

template<typename T, ImplKind Impl>
constexpr auto MatrixRotation(T inRads)
{
	return MatrixHelper<T, Impl>::MatrixRotation(inRads);
}

template<typename T>
constexpr auto MatrixMul(typename Impl8<T>::Scalar& ioLHS, const typename Impl8<T>::Scalar& inRHS)
{
	return MatrixHelper<T, ImplKind::kScalar>::MatrixMul(ioLHS, inRHS);
}

template<typename T>
typename auto MatrixInv(typename Impl8<T>::Scalar& ioLHS)
{
	return MatrixHelper<T, ImplKind::kScalar>::MatrixInv(ioLHS);
}

} // namespace saber::geometry::detail

#endif // SABER_GEOMETRY_DETAIL_MATRIX_HELPER_HPP