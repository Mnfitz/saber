#ifndef SABER_GEOMETRY_MATRIX_HPP
#define SABER_GEOMETRY_MATRIX_HPP

// saber
#include "saber/geometry/config.hpp"
#include "saber/geometry/operators.hpp"
#include "saber/geometry/detail/impl8.hpp"
#include "saber/utility.hpp"

// std
#include <utility>

namespace saber::geometry {

/// @brief Represents a 2x3 matrix in 2D space.
/// @tparam T The type of the matrix's coordinates (e.g., float, double).
/// @tparam ImplType The implementation kind (e.g., scalar or SIMD).
template<typename T, ImplKind Impl = ImplKind::kDefault>
class Matrix
{
public:
	using ValueType = T;

public:
	/// @brief Default constructor. Initializes a matrix with default 0 values.
	constexpr Matrix() = default;

	constexpr Matrix(T inM11, T inM12, T inM13, T inM21, T inM22, T inM23);

	/// @brief Destructor.
	~Matrix() = default;

	/// @brief Move constructor.
	constexpr Matrix(Matrix&& ioMove) noexcept = default;

	/// @brief Move assignment operator.
	constexpr Matrix& operator=(Matrix&& ioMove) noexcept = default;

	/// @brief Copy constructor.
	constexpr Matrix(const Matrix& inCopy) = default;

	/// @brief Copy assignment operator.
	constexpr Matrix& operator=(const Matrix& inCopy) = default;

	// Math Operations
	constexpr Matrix& operator+=(const Matrix& inRHS);
	constexpr Matrix& operator-=(const Matrix& inRHS);
	constexpr Matrix& operator*=(const Matrix& inRHS);

	// Getters
	constexpr T M11() const;
	constexpr T M12() const;
	constexpr T M13() const;
	constexpr T M21() const;
	constexpr T M22() const;
	constexpr T M23() const;

	// Setters
	constexpr void M11(T inT);
	constexpr void M12(T inT);
	constexpr void M13(T inT);
	constexpr void M21(T inT);
	constexpr void M22(T inT);
	constexpr void M23(T inT);

private:
	// Private APIs

	/// @brief Checks if this matrix is equal to another.
	/// @param inMatrix The matrix to compare with.
	/// @return True if equal, false otherwise.
	constexpr bool IsEqual(const Matrix& inMatrix) const;

	// Friend functions
private:
	friend constexpr bool operator== <Matrix>(const Matrix& inLHS, const Matrix& inRHS);
	friend constexpr bool operator!= <Matrix>(const Matrix& inLHS, const Matrix& inRHS);

	template<typename T, ImplKind Impl>
	friend constexpr bool IsEmpty(const Matrix<T, Impl>& inMatrix);

private:
	using ImplType = typename detail::Impl8Traits<T, Impl>::ImplType; // VOODOO: Nested template type requires `typename` prefix
	ImplType mImpl{};
}; // class Matrix<>

// ------------------------------------------------------------------
#pragma region Inline Class Functions

// Ctors
template<typename T, ImplKind Impl>
inline constexpr Matrix<T, Impl>::Matrix(T inM11, T inM12, T inM13, T inM21, T inM22, T inM23) :
	mImpl(inM11, inM12, inM13, inM21, inM22, inM23, 0, 0)
{
	// Do nothing
}


template<typename T, ImplKind Impl>
inline constexpr bool Matrix<T, Impl>::IsEqual(const Matrix& inMatrix) const
{
	auto result = mImpl.IsEqual(inMatrix.mImpl);
	return result;
}

// Math Operations
template<typename T, ImplKind Impl>
inline constexpr Matrix<T, Impl>& Matrix<T, Impl>::operator+=(const Matrix& inRHS)
{
	mImpl += inRHS.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Matrix<T, Impl>& Matrix<T, Impl>::operator-=(const Matrix& inRHS)
{
	mImpl -= inRHS.mImpl;
	return *this;
}

template<typename T, ImplKind Impl>
inline constexpr Matrix<T, Impl>& Matrix<T, Impl>::operator*=(const Matrix& inRHS)
{
	auto test = detail::MatrixIdentity<T, Impl>();
	detail::MatrixMul(mImpl, inRHS.mImpl);
	return *this;
}

// Getters
template<typename T, ImplKind Impl>
inline constexpr T Matrix<T, Impl>::M11() const
{
	return mImpl.Get<0>();
}

template<typename T, ImplKind Impl>
inline constexpr T Matrix<T, Impl>::M12() const
{
	return mImpl.Get<1>();
}

template<typename T, ImplKind Impl>
inline constexpr T Matrix<T, Impl>::M13() const
{
	return mImpl.Get<2>();
}

template<typename T, ImplKind Impl>
inline constexpr T Matrix<T, Impl>::M21() const
{
	return mImpl.Get<3>();
}

template<typename T, ImplKind Impl>
inline constexpr T Matrix<T, Impl>::M22() const
{
	return mImpl.Get<4>();
}

template<typename T, ImplKind Impl>
inline constexpr T Matrix<T, Impl>::M23() const
{
	return mImpl.Get<5>();
}

// Setters
template<typename T, ImplKind Impl>
inline constexpr void Matrix<T, Impl>::M11(T inT)
{
	mImpl.Get<0>() = inT;
}

template<typename T, ImplKind Impl>
inline constexpr void Matrix<T, Impl>::M12(T inT)
{
	mImpl.Get<1>() = inT;
}

template<typename T, ImplKind Impl>
inline constexpr void Matrix<T, Impl>::M13(T inT)
{
	mImpl.Get<2>() = inT;
}

template<typename T, ImplKind Impl>
inline constexpr void Matrix<T, Impl>::M21(T inT)
{
	mImpl.Get<3>() = inT;
}

template<typename T, ImplKind Impl>
inline constexpr void Matrix<T, Impl>::M22(T inT)
{
	mImpl.Get<4>() = inT;
}

template<typename T, ImplKind Impl>
inline constexpr void Matrix<T, Impl>::M23(T inT)
{
	mImpl.Get<5>() = inT;
}

// ------------------------------------------------------------------
} // namespace saber::geometry

#endif // SABER_GEOMETRY_MATRIX_HPP