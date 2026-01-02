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
inline constexpr Matrix<T, Impl>::Matrix(T inM11, T inM12, T inM21, T inM22, T inM31, T inM32) :
	mImpl(inM11, inM12, inM21, inM22, inM31, inM32, 0, 0)
{
	// Do nothing
}


template<typename T, ImplKind Impl>
inline constexpr bool Matrix<T, Impl>::IsEqual(const Matrix& inMatrix) const
{
	auto result = mImpl.IsEqual(inMatrix.mImpl);
	return result;
}

// ------------------------------------------------------------------
} // namespace saber::geometry

#endif // SABER_GEOMETRY_MATRIX_HPP