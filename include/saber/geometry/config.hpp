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

#endif // SABER_GEOMETRY_CONFIG_HPP
