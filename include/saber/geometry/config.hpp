#pragma once
#ifndef SABER_GEOMETRY_CONFIG_HPP
#define SABER_GEOMETRY_CONFIG_HPP

// saber
#include "saber/config.hpp"

#ifndef SABER_GEOMETRY_CONFIG_ISENABLED_SIMD
/// @brief Macro controlling whether `saber::geometry` will compile using
/// an underlying SIMD (Single Instruction Multiple Data) implementation.
/// By default, `saber::geometry` compiles using a Scaler implementation.
/// To enable the SIMD implementation, specify this compiler switch:
/// @code{.cpp}
/// -DSABER_GEOMETRY_CONFIG_ISENABLED_SIMD=1
/// @endcode
#define SABER_GEOMETRY_CONFIG_ISENABLED_SIMD	1 /*0*/
#endif // SABER_GEOMETRY_CONFIG_ISENABLED_SIMD

namespace saber::geometry {

/// @brief The set of all possible implementations for saber geometry classes
enum class ImplKind
{
    kSimd = 0,
    kScalar,

#if SABER_GEOMETRY_CONFIG_ISENABLED_SIMD
	kDefault = kSimd
#else
	kDefault = kScalar
#endif // SABER_GEOMETRY_CONFIG_ISENABLED_SIMD
}; // enum class ImplKind

} // namespace saber::geometry

#endif // SABER_GEOMETRY_CONFIG_HPP
