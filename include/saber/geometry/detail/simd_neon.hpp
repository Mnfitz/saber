#ifndef SABER_GEOMETRY_DETAIL_SIMD_NEON_HPP
#define SABER_GEOMETRY_DETAIL_SIMD_NEON_HPP
#pragma once

// This file was generated using ChatGPT 4.1 using the following query:
// Using the pattern established in file "simd_sse.hpp", create an 
// equivalent set of C++ classes that has identical functionality, 
// except that it is implemented using arm64 neon intrinsic functions. 
// It will have 3 specializations of the template struct Simd128.

// neon
#include <arm_neon.h>
#include <array>
#include <limits>

namespace saber::geometry::detail {

// int
template<>
struct Simd128<int> :
    public Simd128Traits<int>
{
    using typename Simd128Traits<int>::SimdType; // int32x4_t
    using typename Simd128Traits<int>::ValueType; // int

    /// @brief Load 4 elements of type`<int>` from memory specified by `inAddr`.
    /// @param inAddr Address of &elements[4] to load
    /// @return Vector type`<int>` of loaded elements
    static SimdType Load4(const int* inAddr)
    {
        return vld1q_s32(inAddr);
    }

    /// @brief Load 2 elements of type`<int>` from memory specified by `inAddr`.
    /// Loaded elements are placed in low order position in result SimdType.
    /// Any unused high order elements are set to zero.
    /// @param inAddr Address of &elements[2] to load
    /// @return Vector type`<int>` of loaded elements
    static SimdType Load2(const int* inAddr)
    {
        int32x2_t low = vld1_s32(inAddr);
        return vcombine_s32(low, vdup_n_s32(0));
    }

    /// @brief Load 1 element of type`<int>` from memory specified by `inAddr`.
    /// Loaded element is placed in low order position in result SimdType.
    /// Any unused high order elements are set to zero.
    /// @param inAddr Address of &element[0] to load
    /// @return Vector type`<int>` of loaded elements
    static SimdType Load1(const int* inAddr)
    {
        return vset_lane_s32(inAddr[0], vdup_n_s32(0), 0);
    }

    /// @brief Store 4 elements of type`<int>` to memory specified by `outAddr`.
    /// @param outAddr Address to store &elements[4]
    /// @param inStore4 Vector type`<int>` of elements to store
    static void Store4(int* outAddr, SimdType inStore4)
    {
        vst1q_s32(outAddr, inStore4);
    }

    /// @brief Store 2 elements of type`<int>` to memory specified by `outAddr`.
    /// The 2 lowest order elements are stored to memory.
    /// Any high order elements are ignored.
    /// @param outAddr Address to store &elements[2]
    /// @param inStore2 Vector type`<int>` of elements to store
    static void Store2(int* outAddr, SimdType inStore2)
    {
        vst1_s32(outAddr, vget_low_s32(inStore2));
    }

    /// @brief Store 1 element of type`<int>` to memory specified by `outAddr`.
    /// The lowest order element is stored to memory.
    /// Any higher order elements are ignored.
    /// @param outAddr Address to store &element[1]
    /// @param inStore1 Vector type`<int>` of elements to store
    static void Store1(int* outAddr, SimdType inStore1)
    {
        outAddr[0] = vgetq_lane_s32(inStore1, 0);
    }

    /// @brief Add all vector type`<int>` elements in `inRHS` to `inLHS`.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Result vector type`<int>`
    static SimdType Add(SimdType inLHS, SimdType inRHS)
    {
        return vaddq_s32(inLHS, inRHS);
    }

    /// @brief Subtract all vector type`<int>` elements in `inRHS` from `inLHS`.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Result vector type`<int>`
    static SimdType Sub(SimdType inLHS, SimdType inRHS)
    {
        return vsubq_s32(inLHS, inRHS);
    }

    /// @brief Multiply all vector type`<int>` elements in `inRHS` to `inLHS`.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Result vector type`<int>`
    static SimdType Mul(SimdType inLHS, SimdType inRHS)
    {
        return vmulq_s32(inLHS, inRHS);
    }

    /// @brief Divide all vector type`<int>` elements in `inRHS` from `inLHS`.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Result vector type`<int>`
    static SimdType Div(SimdType inLHS, SimdType inRHS)
    {
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
        int32x2_t low = vget_low_s32(inSimd);
        return vcombine_s32(low, low);
    }

    /// @brief Duplicate the high half of the SIMD register into both halves.
    /// @param inSimd Input SIMD register.
    /// @return SIMD register with high half duplicated.
    static SimdType DupHi(SimdType inSimd)
    {
        int32x2_t hi = vget_high_s32(inSimd);
        return vcombine_s32(hi, hi);
    }

    /// @brief Compare two vector<int> values to check if all elements equal.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return true if corresponding elements are equal, false otherwise
    static bool IsEq(SimdType inLHS, SimdType inRHS)
    {
        uint32x4_t cmp = vceqq_s32(inLHS, inRHS);
        uint64x2_t pair = vreinterpretq_u64_u32(cmp);
        return (vgetq_lane_u64(pair, 0) == ~0ULL) && (vgetq_lane_u64(pair, 1) == ~0ULL);
    }

    /// @brief Compare two vector<int> values to check if all elements are greater than or equal.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return true if corresponding elements are greater than or equal, false otherwise
    static bool IsGe(SimdType inLHS, SimdType inRHS)
    {
        uint32x4_t cmp = vcgeq_s32(inLHS, inRHS);
        uint64x2_t pair = vreinterpretq_u64_u32(cmp);
        return (vgetq_lane_u64(pair, 0) == ~0ULL) && (vgetq_lane_u64(pair, 1) == ~0ULL);
    }

    /// @brief Compare two vector<int> values to check if all elements are less than or equal.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return true if corresponding elements are less than or equal, false otherwise
    static bool IsLe(SimdType inLHS, SimdType inRHS)
    {
        uint32x4_t cmp = vcleq_s32(inLHS, inRHS);
        uint64x2_t pair = vreinterpretq_u64_u32(cmp);
        return (vgetq_lane_u64(pair, 0) == ~0ULL) && (vgetq_lane_u64(pair, 1) == ~0ULL);
    }

    /// @brief Find the minimum value for each pair of element of SimdType
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return the minimum value for each pair of element of SimdType
    static SimdType Min(SimdType inLHS, SimdType inRHS)
    {
        return vminq_s32(inLHS, inRHS);
    }

    /// @brief Find the maximum value for each pair of element of SimdType
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return the maximum value for each pair of element of SimdType
    static SimdType Max(SimdType inLHS, SimdType inRHS)
    {
        return vmaxq_s32(inLHS, inRHS);
    }

    /// @brief Find the minimum/maximum values for each pair of element of SimdType
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return the minimum/maximum values for each pair of element of SimdType
    static SimdType MinMax(SimdType inLHS, SimdType inRHS)
    {
        int32x4_t min = vminq_s32(inLHS, inRHS);
        int32x4_t max = vmaxq_s32(inLHS, inRHS);
        return vcombine_s32(vget_low_s32(min), vget_high_s32(max));
    }

    /// @brief Find the maximum/minimum values for each pair of element of SimdType
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return the maximum/minimum values for each pair of element of SimdType
    static SimdType MaxMin(SimdType inLHS, SimdType inRHS)
    {
        int32x4_t max = vmaxq_s32(inLHS, inRHS);
        int32x4_t min = vminq_s32(inLHS, inRHS);
        return vcombine_s32(vget_low_s32(max), vget_high_s32(min));
    }

    /// @brief Compare two vector<int> values to check if all elements are equal and return a 4-bit mask.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return 4-bit mask: bit i set if lane i comparison is true.
    static int EqMask(SimdType inLHS, SimdType inRHS)
    {
        const uint32x4_t cmp = vceqq_s32(inLHS, inRHS);
        const int b0 = vgetq_lane_u32(cmp, 0) ? 1 : 0;
        const int b1 = vgetq_lane_u32(cmp, 1) ? 1 : 0;
        const int b2 = vgetq_lane_u32(cmp, 2) ? 1 : 0;
        const int b3 = vgetq_lane_u32(cmp, 3) ? 1 : 0;
        return (b0) | (b1 << 1) | (b2 << 2) | (b3 << 3);
    }

    /// @brief Return GE mask (bit per lane) for integer vectors.
    static int GeMask(SimdType inLHS, SimdType inRHS)
    {
        const uint32x4_t cmp = vcgeq_s32(inLHS, inRHS);
        const int b0 = vgetq_lane_u32(cmp, 0) ? 1 : 0;
        const int b1 = vgetq_lane_u32(cmp, 1) ? 1 : 0;
        const int b2 = vgetq_lane_u32(cmp, 2) ? 1 : 0;
        const int b3 = vgetq_lane_u32(cmp, 3) ? 1 : 0;
        return (b0) | (b1 << 1) | (b2 << 2) | (b3 << 3);
    }

    /// @brief Return LE mask (bit per lane) for integer vectors.
    static int LeMask(SimdType inLHS, SimdType inRHS)
    {
        const uint32x4_t cmp = vcleq_s32(inLHS, inRHS);
        const int b0 = vgetq_lane_u32(cmp, 0) ? 1 : 0;
        const int b1 = vgetq_lane_u32(cmp, 1) ? 1 : 0;
        const int b2 = vgetq_lane_u32(cmp, 2) ? 1 : 0;
        const int b3 = vgetq_lane_u32(cmp, 3) ? 1 : 0;
        return (b0) | (b1 << 1) | (b2 << 2) | (b3 << 3);
    }
};

// float
template<>
struct Simd128<float> :
    public Simd128Traits<float>
{
    using typename Simd128Traits<float>::SimdType; // float32x4_t
    using typename Simd128Traits<float>::ValueType; // float

    /// @brief Load 4 elements of type`<float>` from memory specified by `inAddr`.
    /// @param inAddr Address of &elements[4] to load
    /// @return Vector type`<float>` of loaded elements
    static SimdType Load4(const float* inAddr)
    {
        return vld1q_f32(inAddr);
    }

    /// @brief Load 2 elements of type`<float>` from memory specified by `inAddr`.
    /// Loaded elements are placed in low order position in result SimdType.
    /// Any unused high order elements are set to zero.
    /// @param inAddr Address of &elements[2] to load
    /// @return Vector type`<float>` of loaded elements
    static SimdType Load2(const float* inAddr)
    {
        float32x2_t low = vld1_f32(inAddr);
        return vcombine_f32(low, vdup_n_f32(0.0f));
    }

    /// @brief Load 1 element of type`<float>` from memory specified by `inAddr`.
    /// Loaded element is placed in low order position in result SimdType.
    /// Any unused high order elements are set to zero.
    /// @param inAddr Address of &element[1] to load
    /// @return Vector type`<float>` of loaded elements
    static SimdType Load1(const float* inAddr)
    {
        return vset_lane_f32(inAddr[0], vdup_n_f32(0.0f), 0);
    }

    /// @brief Store 4 elements of type`<float>` to memory specified by `outAddr`.
    /// @param outAddr Address to store &elements[4]
    /// @param inStore4 Vector type`<float>` of elements to store
    static void Store4(float* outAddr, SimdType inStore4)
    {
        vst1q_f32(outAddr, inStore4);
    }

    /// @brief Store 2 elements of type`<float>` to memory specified by `outAddr`.
    /// The 2 lowest order elements are stored to memory.
    /// Any high order elements are ignored.
    /// @param outAddr Address to store &elements[2]
    /// @param inStore2 Vector type`<float>` of elements to store
    static void Store2(float* outAddr, SimdType inStore2)
    {
        vst1_f32(outAddr, vget_low_f32(inStore2));
    }

    /// @brief Store 1 element of type`<float>` to memory specified by `outAddr`.
    /// The lowest order element is stored to memory.
    /// Any higher order elements are ignored.
    /// @param outAddr Address to store &element[1]
    /// @param inStore1 Vector type`<float>` of elements to store
    static void Store1(float* outAddr, SimdType inStore1)
    {
        outAddr[0] = vgetq_lane_f32(inStore1, 0);
    }

    /// @brief Add all vector type`<float>` elements in `inRHS` to `inLHS`.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Result vector type`<float>`
    static SimdType Add(SimdType inLHS, SimdType inRHS)
    {
        return vaddq_f32(inLHS, inRHS);
    }

    /// @brief Subtract all vector type`<float>` elements in `inRHS` from `inLHS`.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Result vector type`<float>`
    static SimdType Sub(SimdType inLHS, SimdType inRHS)
    {
        return vsubq_f32(inLHS, inRHS);
    }

    /// @brief Multiply all vector type`<float>` elements in `inRHS` to `inLHS`.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Result vector type`<float>`
    static SimdType Mul(SimdType inLHS, SimdType inRHS)
    {
        return vmulq_f32(inLHS, inRHS);
    }

    /// @brief Divide all vector type`<float>` elements in `inRHS` from `inLHS`.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Result vector type`<float>`
    static SimdType Div(SimdType inLHS, SimdType inRHS)
    {
        return vdivq_f32(inLHS, inRHS);
    }

    /// @brief Duplicate the low half of the SIMD register into both halves.
    /// @param inSimd Input SIMD register.
    /// @return SIMD register with low half duplicated.
    static SimdType DupLo(SimdType inSimd)
    {
        float32x2_t low = vget_low_f32(inSimd);
        return vcombine_f32(low, low);
    }

    /// @brief Duplicate the high half of the SIMD register into both halves.
    /// @param inSimd Input SIMD register.
    /// @return SIMD register with high half duplicated.
    static SimdType DupHi(SimdType inSimd)
    {
        float32x2_t hi = vget_high_f32(inSimd);
        return vcombine_f32(hi, hi);
    }

    /// @brief Compare two vector<float> values to check if all elements equal or inexactly equal.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return true if corresponding elements are equal, false otherwise
    static bool IsEq(SimdType inLHS, SimdType inRHS)
    {
        float32x4_t diff = vabsq_f32(vsubq_f32(inLHS, inRHS));
        float32x4_t maxMag = vmaxq_f32(vmaxq_f32(vabsq_f32(inLHS), vabsq_f32(inRHS)), vdupq_n_f32(1.0f));
        float32x4_t epsilon = vmulq_f32(maxMag, vdupq_n_f32(std::numeric_limits<float>::epsilon()));
        uint32x4_t cmp = vcleq_f32(diff, epsilon);
        uint64x2_t pair = vreinterpretq_u64_u32(cmp);
        return (vgetq_lane_u64(pair, 0) == ~0ULL) && (vgetq_lane_u64(pair, 1) == ~0ULL);
    }

    /// @brief Compare two vector<float> values to check if all elements are greater than or equal.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return true if corresponding elements are greater than or equal, false otherwise
    static bool IsGe(SimdType inLHS, SimdType inRHS)
    {
        uint32x4_t cmp = vcgeq_f32(inLHS, inRHS);
        uint64x2_t pair = vreinterpretq_u64_u32(cmp);
        return (vgetq_lane_u64(pair, 0) == ~0ULL) && (vgetq_lane_u64(pair, 1) == ~0ULL);
    }

    /// @brief Compare two vector<float> values to check if all elements are less than or equal.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return true if corresponding elements are less than or equal, false otherwise
    static bool IsLe(SimdType inLHS, SimdType inRHS)
    {
        uint32x4_t cmp = vcleq_f32(inLHS, inRHS);
        uint64x2_t pair = vreinterpretq_u64_u32(cmp);
        return (vgetq_lane_u64(pair, 0) == ~0ULL) && (vgetq_lane_u64(pair, 1) == ~0ULL);
    }

    /// @brief Round all <float> values toward the nearest whole number
    /// @param inRound Input to be rounded
    /// @return Return rounded SimdType values
    static SimdType RoundNearest(SimdType inRound)
    {
        return vrndnq_f32(inRound);
    }

    /// @brief Round all <float> values toward positive infinity
    /// @param inRound Input to be rounded
    /// @return Return rounded SimdType values
    static SimdType RoundCeil(SimdType inRound)
    {
        return vrndpq_f32(inRound);
    }

    /// @brief Round all <float> values toward negative infinity
    /// @param inRound Input to be rounded
    /// @return Return rounded SimdType values
    static SimdType RoundFloor(SimdType inRound)
    {
        return vrndmq_f32(inRound);
    }

    /// @brief Round all <float> values toward zero
    /// @param inRound Input to be rounded
    /// @return Return rounded SimdType values
    static SimdType RoundTrunc(SimdType inRound)
    {
        return vrndq_f32(inRound);
    }

    /// @brief Find the minimum value for each pair of element of SimdType
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return the minimum value for each pair of element of SimdType
    static SimdType Min(SimdType inLHS, SimdType inRHS)
    {
        return vminq_f32(inLHS, inRHS);
    }

    /// @brief Find the maximum value for each pair of element of SimdType
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return the maximum value for each pair of element of SimdType
    static SimdType Max(SimdType inLHS, SimdType inRHS)
    {
        return vmaxq_f32(inLHS, inRHS);
    }

    /// @brief Find the minimum/maximum values for each pair of element of SimdType
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return the minimum/maximum values for each pair of element of SimdType
    static SimdType MinMax(SimdType inLHS, SimdType inRHS)
    {
        float32x4_t min = vminq_f32(inLHS, inRHS);
        float32x4_t max = vmaxq_f32(inLHS, inRHS);
        return vcombine_f32(vget_low_f32(min), vget_high_f32(max));
    }

    /// @brief Find the maximum/minimum values for each pair of element of SimdType
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return the maximum/minimum values for each pair of element of SimdType
    static SimdType MaxMin(SimdType inLHS, SimdType inRHS)
    {
        float32x4_t max = vmaxq_f32(inLHS, inRHS);
        float32x4_t min = vminq_f32(inLHS, inRHS);
        return vcombine_f32(vget_low_f32(max), vget_high_f32(min));
    }

    /// @brief Compute per-lane equality mask for floating vectors using inexact comparison logic.
    /// @param inLHS Left hand side vector
    /// @param inRHS Right hand side vector
    /// @return 4-bit mask: bit i set if lane i considered equal (within tolerance).
    static int EqMask(SimdType inLHS, SimdType inRHS)
    {
        const float32x4_t absLHS = vabsq_f32(inLHS);
        const float32x4_t absRHS = vabsq_f32(inRHS);
        const float32x4_t minMag = vdupq_n_f32(1.0f);
        const float32x4_t maxMag = vmaxq_f32(vmaxq_f32(absLHS, absRHS), minMag);
        const float32x4_t epsilon = vmulq_f32(maxMag, vdupq_n_f32(std::numeric_limits<float>::epsilon()));
        const float32x4_t diff = vabsq_f32(vsubq_f32(inLHS, inRHS));
        const uint32x4_t cmp = vcleq_f32(diff, epsilon);

        const int b0 = vgetq_lane_u32(cmp, 0) ? 1 : 0;
        const int b1 = vgetq_lane_u32(cmp, 1) ? 1 : 0;
        const int b2 = vgetq_lane_u32(cmp, 2) ? 1 : 0;
        const int b3 = vgetq_lane_u32(cmp, 3) ? 1 : 0;
        return (b0) | (b1 << 1) | (b2 << 2) | (b3 << 3);
    }

    /// @brief Return GE mask (bit per lane) for float vectors using inexact-equality fallback.
    static int GeMask(SimdType inLHS, SimdType inRHS)
    {
        uint32x4_t cmp = vcgeq_f32(inLHS, inRHS);
        int mask = (vgetq_lane_u32(cmp,0)?1:0) | (vgetq_lane_u32(cmp,1)?2:0) | (vgetq_lane_u32(cmp,2)?4:0) | (vgetq_lane_u32(cmp,3)?8:0);
        if (mask != 0xF)
        {
            // blendv: where cmp is true keep inLHS else take inRHS
            const float32x4_t lhs_blend = vbslq_f32(cmp, inLHS, inRHS);
            mask &= EqMask(lhs_blend, inRHS);
        }
        return mask;
    }

    /// @brief Return LE mask (bit per lane) for float vectors using inexact-equality fallback.
    static int LeMask(SimdType inLHS, SimdType inRHS)
    {
        uint32x4_t cmp = vcleq_f32(inLHS, inRHS);
        int mask = (vgetq_lane_u32(cmp,0)?1:0) | (vgetq_lane_u32(cmp,1)?2:0) | (vgetq_lane_u32(cmp,2)?4:0) | (vgetq_lane_u32(cmp,3)?8:0);
        if (mask != 0xF)
        {
            const float32x4_t lhs_blend = vbslq_f32(cmp, inLHS, inRHS);
            mask &= EqMask(lhs_blend, inRHS);
        }
        return mask;
    }
};

// double
template<>
struct Simd128<double> :
    public Simd128Traits<double>
{
    using typename Simd128Traits<double>::SimdType; // float64x2_t
    using typename Simd128Traits<double>::ValueType; // double

    /// @brief Load 2 elements of type`<double>` from memory specified by `inAddr`.
    /// Loaded elements are placed in low order position in result SimdType.
    /// Any unused high order elements are set to zero.
    /// @param inAddr Address of &elements[2] to load
    /// @return Vector type`<double>` of loaded elements
    static SimdType Load2(const double* inAddr)
    {
        return vld1q_f64(inAddr);
    }

    /// @brief Load 1 element of type`<double>` from memory specified by `inAddr`.
    /// Loaded element is placed in low order position in result SimdType.
    /// Any unused high order elements are set to zero.
    /// @param inAddr Address of &element[1] to load
    /// @return Vector type`<double>` of loaded elements
    static SimdType Load1(const double* inAddr)
    {
        return vset_lane_f64(inAddr[0], vdup_n_f64(0.0), 0);
    }

    /// @brief Store 2 elements of type`<double>` to memory specified by `outAddr`.
    /// The 2 lowest order elements are stored to memory.
    /// Any high order elements are ignored.
    /// @param outAddr Address to store &elements[2]
    /// @param inStore2 Vector type`<double>` of elements to store
    static void Store2(double* outAddr, SimdType inStore2)
    {
        vst1q_f64(outAddr, inStore2);
    }

    /// @brief Store 1 element of type`<double>` to memory specified by `outAddr`.
    /// The lowest order element is stored to memory.
    /// Any higher order elements are ignored.
    /// @param outAddr Address to store &element[1]
    /// @param inStore1 Vector type`<double>` of elements to store
    static void Store1(double* outAddr, SimdType inStore1)
    {
        outAddr[0] = vgetq_lane_f64(inStore1, 0);
    }

    /// @brief Add all vector type`<double>` elements in `inRHS` to `inLHS`.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Result vector type`<double>`
    static SimdType Add(SimdType inLHS, SimdType inRHS)
    {
        return vaddq_f64(inLHS, inRHS);
    }

    /// @brief Subtract all vector type`<double>` elements in `inRHS` from `inLHS`.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Result vector type`<double>`
    static SimdType Sub(SimdType inLHS, SimdType inRHS)
    {
        return vsubq_f64(inLHS, inRHS);
    }

    /// @brief Multiply all vector type`<double>` elements in `inRHS` to `inLHS`.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Result vector type`<double>`
    static SimdType Mul(SimdType inLHS, SimdType inRHS)
    {
        return vmulq_f64(inLHS, inRHS);
    }

    /// @brief Divide all vector type`<double>` elements in `inRHS` from `inLHS`.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Result vector type`<double>`
    static SimdType Div(SimdType inLHS, SimdType inRHS)
    {
        return vdivq_f64(inLHS, inRHS);
    }

    /// @brief Duplicate the low half of the SIMD register into both halves.
    /// @param inSimd Input SIMD register.
    /// @return SIMD register with low half duplicated.
    static SimdType DupLo(SimdType inSimd)
    {
        float64x1_t low = vget_low_f64(inSimd);
        return vcombine_f64(low, low);
    }

    /// @brief Duplicate the high half of the SIMD register into both halves.
    /// @param inSimd Input SIMD register.
    /// @return SIMD register with high half duplicated.
    static SimdType DupHi(SimdType inSimd)
    {
        float64x1_t hi = vget_high_f64(inSimd);
        return vcombine_f64(hi, hi);
    }

    /// @brief Compare two vector<double> values to check if all elements equal.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return true if corresponding elements are equal, false otherwise
    static bool IsEq(SimdType inLHS, SimdType inRHS)
    {
        float64x2_t diff = vabsq_f64(vsubq_f64(inLHS, inRHS));
        float64x2_t maxMag = vmaxq_f64(vmaxq_f64(vabsq_f64(inLHS), vabsq_f64(inRHS)), vdupq_n_f64(1.0));
        float64x2_t epsilon = vmulq_f64(maxMag, vdupq_n_f64(std::numeric_limits<double>::epsilon()));
        uint64x2_t cmp = vcleq_f64(diff, epsilon);
        return (vgetq_lane_u64(cmp, 0) == ~0ULL) && (vgetq_lane_u64(cmp, 1) == ~0ULL);
    }

    /// @brief Compare two vector<double> values to check if all elements are greater than or equal.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return true if corresponding elements are greater than or equal, false otherwise
    static bool IsGe(SimdType inLHS, SimdType inRHS)
    {
        uint64x2_t cmp = vcgeq_f64(inLHS, inRHS);
        return (vgetq_lane_u64(cmp, 0) == ~0ULL) && (vgetq_lane_u64(cmp, 1) == ~0ULL);
    }

    /// @brief Compare two vector<double> values to check if all elements are less than or equal.
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return true if corresponding elements are less than or equal, false otherwise
    static bool IsLe(SimdType inLHS, SimdType inRHS)
    {
        uint64x2_t cmp = vcleq_f64(inLHS, inRHS);
        return (vgetq_lane_u64(cmp, 0) == ~0ULL) && (vgetq_lane_u64(cmp, 1) == ~0ULL);
    }

    /// @brief Round all <double> values toward the nearest whole number
    /// @param inRound Input to be rounded
    /// @return Return rounded SimdType values
    static SimdType RoundNearest(SimdType inRound)
    {
        return vrndnq_f64(inRound);
    }

    /// @brief Round all <double> values toward positive infinity
    /// @param inRound Input to be rounded
    /// @return Return rounded SimdType values
    static SimdType RoundCeil(SimdType inRound)
    {
        return vrndpq_f64(inRound);
    }

    /// @brief Round all <double> values toward negative infinity
    /// @param inRound Input to be rounded
    /// @return Return rounded SimdType values
    static SimdType RoundFloor(SimdType inRound)
    {
        return vrndmq_f64(inRound);
    }

    /// @brief Round all <double> values toward zero
    /// @param inRound Input to be rounded
    /// @return Return rounded SimdType values
    static SimdType RoundTrunc(SimdType inRound)
    {
        return vrndq_f64(inRound);
    }

    /// @brief Find the minimum value for each pair of element of SimdType
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return the minimum value for each pair of element of SimdType
    static SimdType Min(SimdType inLHS, SimdType inRHS)
    {
        return vminq_f64(inLHS, inRHS);
    }

    /// @brief Find the maximum value for each pair of element of SimdType
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return the maximum value for each pair of element of SimdType
    static SimdType Max(SimdType inLHS, SimdType inRHS)
    {
        return vmaxq_f64(inLHS, inRHS);
    }

    /// @brief Find the minimum/maximum values for each pair of element of SimdType
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return the minimum/maximum values for each pair of element of SimdType
    static SimdType MinMax(SimdType inLHS, SimdType inRHS)
    {
        float64x2_t min = vminq_f64(inLHS, inRHS);
        float64x2_t max = vmaxq_f64(inLHS, inRHS);
        return vcombine_f64(vget_low_f64(min), vget_high_f64(max));
    }

    /// @brief Find the maximum/minimum values for each pair of element of SimdType
    /// @param inLHS Left hand side vector term
    /// @param inRHS Right hand side vector term
    /// @return Return the maximum/minimum values for each pair of element of SimdType
    static SimdType MaxMin(SimdType inLHS, SimdType inRHS)
    {
        float64x2_t max = vmaxq_f64(inLHS, inRHS);
        float64x2_t min = vminq_f64(inLHS, inRHS);
        return vcombine_f64(vget_low_f64(max), vget_high_f64(min));
    }

    /// @brief Compute per-lane equality mask for double vectors using inexact comparison logic.
    /// @param inLHS Left hand side vector
    /// @param inRHS Right hand side vector
    /// @return 2-bit mask: bit i set if lane i considered equal (within tolerance).
    static int EqMask(SimdType inLHS, SimdType inRHS)
    {
        const float64x2_t absLHS = vabsq_f64(inLHS);
        const float64x2_t absRHS = vabsq_f64(inRHS);
        const float64x2_t minMag = vdupq_n_f64(1.0);
        const float64x2_t maxMag = vmaxq_f64(vmaxq_f64(absLHS, absRHS), minMag);
        const float64x2_t epsilon = vmulq_f64(maxMag, vdupq_n_f64(std::numeric_limits<double>::epsilon()));
        const float64x2_t diff = vabsq_f64(vsubq_f64(inLHS, inRHS));
        const uint64x2_t cmp = vcleq_f64(diff, epsilon);

        const int b0 = vgetq_lane_u64(cmp, 0) ? 1 : 0;
        const int b1 = vgetq_lane_u64(cmp, 1) ? 1 : 0;
        return (b0) | (b1 << 1);
    }

    /// @brief Return GE mask (bit per lane) for double vectors using inexact-equality fallback.
    static int GeMask(SimdType inLHS, SimdType inRHS)
    {
        uint64x2_t cmp = vcgeq_f64(inLHS, inRHS);
        int mask = (vgetq_lane_u64(cmp,0)?1:0) | (vgetq_lane_u64(cmp,1)?2:0);
        if (mask != 0x3)
        {
            // blend: use vbslq_f64 where cmp true keep LHS else RHS
            const float64x2_t lhs_blend = vreinterpretq_f64_u64(vbslq_u64(vreinterpretq_u64_f64(cmp), vreinterpretq_u64_f64(inLHS), vreinterpretq_u64_f64(inRHS)));
            mask &= EqMask(lhs_blend, inRHS);
        }
        return mask;
    }

    /// @brief Return LE mask (bit per lane) for double vectors using inexact-equality fallback.
    static int LeMask(SimdType inLHS, SimdType inRHS)
    {
        uint64x2_t cmp = vcleq_f64(inLHS, inRHS);
        int mask = (vgetq_lane_u64(cmp,0)?1:0) | (vgetq_lane_u64(cmp,1)?2:0);
        if (mask != 0x3)
        {
            const float64x2_t lhs_blend = vreinterpretq_f64_u64(vbslq_u64(vreinterpretq_u64_f64(cmp), vreinterpretq_u64_f64(inLHS), vreinterpretq_u64_f64(inRHS)));
            mask &= EqMask(lhs_blend, inRHS);
        }
        return mask;
    }
};

} // namespace saber::geometry::detail

#endif // SABER_GEOMETRY_DETAIL_SIMD_NEON_HPP