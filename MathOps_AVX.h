#ifndef MATHOPS_AVX_H
#define MATHOPS_AVX_H

///////////////////////////////////
// Math ops for 8-way AVX code
// Author: Angus F. Hewlett
// Copyright FXpansion Audio UK Ltd. 2012
///////////////////////////////////

////////////////////////////////
// This file provides:-
// * Typedefs for n-way AVX (8x32) containers
// ** interleave amount configured by template parameter - can be any power of 2
// ** vec_float & vec_int - float32 and int32 respectively
// * copy constructors, assignment and basic math operators for those containers
// * General intrinsic-function-style math operations for 8-way AVX data ("addps()" etc.) operating on vec_float & vec_int types
// ** (as static functions in Mathops_AVXxN class - inherit to win)
// * typedefs / conversion routines for packing/unpacking to plain vectors of floats/ints
////////////////////////////////

#ifndef CEXCOMPILE
#include "MathOps_Common.h"
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
// class Mathops_AVX
// Math operations for interleaved N-way SSE data
///////////////////////////////////////////////////////////////////////////////////////////////////

template <const int intrlv> class MathOps
{
public:
	static constexpr int raw_vec_elem = 8;
	static constexpr int raw_vec_2pow = 3;
	static constexpr int interleave = intrlv;
	static constexpr int32 vec_elem = interleave * raw_vec_elem;
    static constexpr int alignment = 32;

	typedef __m256 vec_elem_t __attribute__ ((aligned (32)));
	typedef __m256i vec_int_t __attribute__ ((aligned (32)));

    
    ARG2WRAPPER(__m256, _mm256_min_ps);
    ARG2WRAPPER(__m256, _mm256_max_ps);
    ARG2WRAPPER(__m256, _mm256_add_ps);
    ARG2WRAPPER(__m256, _mm256_sub_ps);
    ARG2WRAPPER(__m256, _mm256_mul_ps);
    ARG2WRAPPER(__m256, _mm256_div_ps);
    ARG2WRAPPER(__m256, _mm256_or_ps);
    ARG2WRAPPER(__m256, _mm256_and_ps);
    ARG2WRAPPER(__m256, _mm256_andnot_ps);
    ARG2WRAPPER(__m256, _mm256_xor_ps);
    ARG2WRAPPERK(__m256, _mm256_cmp_ps);
    ARG1WRAPPER(__m256, _mm256_rcp_ps);
    ARG1WRAPPER(__m256, _mm256_sqrt_ps);
    ARG1WRAPPER(__m256, _mm256_floor_ps);

    typedef vec_float_impl_t<MathOps, interleave> vec_float  __attribute__ ((aligned (32)));
    typedef Interleaver<interleave, vec_float, __m256, _mm256_set1_ps> Inter;


	///////////////////////////////////////////////////////////////////////////////////////////////////
	// Intrinsic-style functions for f32 & i32 vector operations
	///////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////
	// Initialization - zeroes, set1float, set1int
	static vforceinline vec_float  zerops()
	{
        return Inter::assignE(_mm256_setzero_ps());
	};

	static vforceinline vec_float  set1ps(float q1)
	{ 
		return Inter::assignS(q1);
	};
    
    static vforceinline vec_float  loadps(const float* q1)
    {
        vec_float rv;
        if (intrlv > 0x0) rv.m[0x0] = _mm256_loadu_ps(&(q1[0]));
        if (intrlv > 0x1) rv.m[0x1] = _mm256_loadu_ps(&(q1[8]));
        if (intrlv > 0x2) rv.m[0x2] = _mm256_loadu_ps(&(q1[16]));
        if (intrlv > 0x3) rv.m[0x3] = _mm256_loadu_ps(&(q1[24]));
        if (intrlv > 0x4) rv.m[0x4] = _mm256_loadu_ps(&(q1[32]));
        if (intrlv > 0x5) rv.m[0x5] = _mm256_loadu_ps(&(q1[40]));
        if (intrlv > 0x6) rv.m[0x6] = _mm256_loadu_ps(&(q1[48]));
        if (intrlv > 0x7) rv.m[0x7] = _mm256_loadu_ps(&(q1[56]));
        if (intrlv > 0x8) rv.m[0x8] = _mm256_loadu_ps(&(q1[64]));
        if (intrlv > 0x9) rv.m[0x9] = _mm256_loadu_ps(&(q1[72]));
        if (intrlv > 0xA) rv.m[0xA] = _mm256_loadu_ps(&(q1[80]));
        if (intrlv > 0xB) rv.m[0xB] = _mm256_loadu_ps(&(q1[88]));
        if (intrlv > 0xC) rv.m[0xC] = _mm256_loadu_ps(&(q1[96]));
        if (intrlv > 0xD) rv.m[0xD] = _mm256_loadu_ps(&(q1[104]));
        if (intrlv > 0xE) rv.m[0xE] = _mm256_loadu_ps(&(q1[112]));
        if (intrlv > 0xF) rv.m[0xF] = _mm256_loadu_ps(&(q1[120]));
        return rv;
    };
    
    template <size_t increment> vforceinline vec_float gather(const float* base_address)
    {
        const __m256i scale_base = _mm256_set_epi32(0x00 * increment,0x01 * increment, 0x02 * increment, 0x03 * increment, 0x04 * increment, 0x05 * increment, 0x06 * increment, 0x07 * increment);
        vec_float rv;
        constexpr int32 inc_f = increment / sizeof(float);
        if (intrlv > 0x0) rv.m[0x0] = _mm256_i32gather_ps(base_address + (inc_f * 0x00), scale_base, 1);
        if (intrlv > 0x1) rv.m[0x1] = _mm256_i32gather_ps(base_address + (inc_f * 0x08), scale_base, 1);
        if (intrlv > 0x2) rv.m[0x2] = _mm256_i32gather_ps(base_address + (inc_f * 0x10), scale_base, 1);
        if (intrlv > 0x3) rv.m[0x3] = _mm256_i32gather_ps(base_address + (inc_f * 0x18), scale_base, 1);
        if (intrlv > 0x4) rv.m[0x4] = _mm256_i32gather_ps(base_address + (inc_f * 0x20), scale_base, 1);
        if (intrlv > 0x5) rv.m[0x5] = _mm256_i32gather_ps(base_address + (inc_f * 0x28), scale_base, 1);
        if (intrlv > 0x6) rv.m[0x6] = _mm256_i32gather_ps(base_address + (inc_f * 0x30), scale_base, 1);
        if (intrlv > 0x7) rv.m[0x7] = _mm256_i32gather_ps(base_address + (inc_f * 0x38), scale_base, 1);
        if (intrlv > 0x8) rv.m[0x8] = _mm256_i32gather_ps(base_address + (inc_f * 0x40), scale_base, 1);
        if (intrlv > 0x9) rv.m[0x9] = _mm256_i32gather_ps(base_address + (inc_f * 0x48), scale_base, 1);
        if (intrlv > 0xA) rv.m[0xA] = _mm256_i32gather_ps(base_address + (inc_f * 0x50), scale_base, 1);
        if (intrlv > 0xB) rv.m[0xB] = _mm256_i32gather_ps(base_address + (inc_f * 0x58), scale_base, 1);
        if (intrlv > 0xC) rv.m[0xC] = _mm256_i32gather_ps(base_address + (inc_f * 0x60), scale_base, 1);
        if (intrlv > 0xD) rv.m[0xD] = _mm256_i32gather_ps(base_address + (inc_f * 0x68), scale_base, 1);
        if (intrlv > 0xE) rv.m[0xE] = _mm256_i32gather_ps(base_address + (inc_f * 0x70), scale_base, 1);
        if (intrlv > 0xF) rv.m[0xF] = _mm256_i32gather_ps(base_address + (inc_f * 0x78), scale_base, 1);
        return rv;
    }
    
    
    template <int __N> vforceinline float _mm256_extract_ps (const __m256& __X)
    {
          __m128 __Y = _mm256_extractf128_ps (__X, __N >> 2);
          return _mm_extract_ps (__Y, __N % 4);
    }
    
    template <size_t increment> vforceinline void storeps(const __m256& data, float* e0)
    {
        *(e0 + (increment * 0x00)) = _mm256_extract_ps<0x0>(data);
        *(e0 + (increment * 0x01)) = _mm256_extract_ps<0x1>(data);
        *(e0 + (increment * 0x02)) = _mm256_extract_ps<0x2>(data);
        *(e0 + (increment * 0x03)) = _mm256_extract_ps<0x3>(data);
        *(e0 + (increment * 0x04)) = _mm256_extract_ps<0x4>(data);
        *(e0 + (increment * 0x05)) = _mm256_extract_ps<0x5>(data);
        *(e0 + (increment * 0x06)) = _mm256_extract_ps<0x6>(data);
        *(e0 + (increment * 0x07)) = _mm256_extract_ps<0x7>(data);
    };
    
    
    template <size_t increment> vforceinline void scatter(const vec_float& rv, float* base_address)
    {
        constexpr int32 inc_f = increment / sizeof(float);
        if (intrlv > 0x0) storeps<inc_f>(rv.m[0x0],  base_address + (inc_f * 0x00));
        if (intrlv > 0x1) storeps<inc_f>(rv.m[0x1],  base_address + (inc_f * 0x08));
        if (intrlv > 0x2) storeps<inc_f>(rv.m[0x2],  base_address + (inc_f * 0x10));
        if (intrlv > 0x3) storeps<inc_f>(rv.m[0x3],  base_address + (inc_f * 0x18));
        if (intrlv > 0x4) storeps<inc_f>(rv.m[0x4],  base_address + (inc_f * 0x20));
        if (intrlv > 0x5) storeps<inc_f>(rv.m[0x5],  base_address + (inc_f * 0x28));
        if (intrlv > 0x6) storeps<inc_f>(rv.m[0x6],  base_address + (inc_f * 0x30));
        if (intrlv > 0x7) storeps<inc_f>(rv.m[0x7],  base_address + (inc_f * 0x38));
        if (intrlv > 0x8) storeps<inc_f>(rv.m[0x8],  base_address + (inc_f * 0x40));
        if (intrlv > 0x9) storeps<inc_f>(rv.m[0x9],  base_address + (inc_f * 0x48));
        if (intrlv > 0xA) storeps<inc_f>(rv.m[0xA],  base_address + (inc_f * 0x50));
        if (intrlv > 0xB) storeps<inc_f>(rv.m[0xB],  base_address + (inc_f * 0x58));
        if (intrlv > 0xC) storeps<inc_f>(rv.m[0xC],  base_address + (inc_f * 0x60));
        if (intrlv > 0xD) storeps<inc_f>(rv.m[0xD],  base_address + (inc_f * 0x68));
        if (intrlv > 0xE) storeps<inc_f>(rv.m[0xE],  base_address + (inc_f * 0x70));
        if (intrlv > 0xF) storeps<inc_f>(rv.m[0xF],  base_address + (inc_f * 0x78));
    }
    
	
	//////////////////////////////////////
	// Float operations - add, sub, mul, min, max, bitwise or, bitwise and, abs
	GENERATE_INTERLEAVED_FUNCTION_2ARG(mulps, wrap__mm256_mul_ps);
	GENERATE_INTERLEAVED_FUNCTION_2ARG(addps, wrap__mm256_add_ps);
	GENERATE_INTERLEAVED_FUNCTION_2ARG(subps, wrap__mm256_sub_ps);
	GENERATE_INTERLEAVED_FUNCTION_2ARG(divps, wrap__mm256_div_ps);
	GENERATE_INTERLEAVED_FUNCTION_2ARG(minps, wrap__mm256_min_ps);
	GENERATE_INTERLEAVED_FUNCTION_2ARG(maxps, wrap__mm256_max_ps);
	GENERATE_INTERLEAVED_FUNCTION_2ARG(orps, wrap__mm256_or_ps);
	GENERATE_INTERLEAVED_FUNCTION_2ARG(andps, wrap__mm256_and_ps);
	GENERATE_INTERLEAVED_FUNCTION_2ARG(andnps, wrap__mm256_andnot_ps);
	GENERATE_INTERLEAVED_FUNCTION_2ARG(xorps, wrap__mm256_xor_ps);
    
	GENERATE_INTERLEAVED_FUNCTION_2ARG_K(cmpgeps, wrap__mm256_cmp_ps<_CMP_GE_OS>, _CMP_GE_OS);
	GENERATE_INTERLEAVED_FUNCTION_2ARG_K(cmpgtps, wrap__mm256_cmp_ps, _CMP_GT_OS);
	GENERATE_INTERLEAVED_FUNCTION_2ARG_K(cmpleps, wrap__mm256_cmp_ps, _CMP_LE_OS);
	GENERATE_INTERLEAVED_FUNCTION_2ARG_K(cmpltps, wrap__mm256_cmp_ps, _CMP_LT_OS);
	GENERATE_INTERLEAVED_FUNCTION_2ARG_K(cmpeqps, wrap__mm256_cmp_ps, _CMP_EQ_OS);
	GENERATE_INTERLEAVED_FUNCTION_2ARG_K(cmpneps, wrap__mm256_cmp_ps, _CMP_NEQ_OS);
    
	GENERATE_INTERLEAVED_FUNCTION_1ARG(rcpps, wrap__mm256_rcp_ps);
    GENERATE_INTERLEAVED_FUNCTION_1ARG(sqrtps, wrap__mm256_sqrt_ps);
	GENERATE_INTERLEAVED_FUNCTION_1ARG(floorps, wrap__mm256_floor_ps);

	static vforceinline vec_float  roundps(const vec_float& q1)
	{
		// this assumes ROUND_DOWN. TODO: replace with proper controlword-based rounding
		vec_float rup = (q1 + 0.5f);
		return floorps(rup);
	}

	static vforceinline vec_float  absps(const vec_float& q1)
	{
        int32 mask = 0x7FFFFFFF;
        vec_float tmp = *(reinterpret_cast<float*>(&mask));
		return andps(q1, tmp);
	};

	static vforceinline vec_float  notps(const vec_float& q1)
	{
		return xorps(q1, _mm256_castsi256_ps(_mm256_set1_epi32(0xFFFFFFFF)));
	};

	static vforceinline vec_float  clipps(const vec_float& val, const vec_float& lo_lim, const vec_float& hi_lim)
	{
		return maxps(lo_lim, minps(val, hi_lim));
	};

	static vforceinline vec_float  cubeps(const vec_float& q1)
	{
		return mulps(q1, mulps(q1,q1));
	};

	static vforceinline vec_float  clip01ps(const vec_float& q1)
	{
		return maxps(minps(q1, set1ps(1.f)), 0.f);
	};

	static vforceinline vec_float  maskps(const vec_float& maskee, const vec_float& conditional_mask)
	{
		return andps(maskee, conditional_mask);
	};


	//////////////////////////////////////
	// Types and conversions for extracting to raw data
	//////////////////////////////////////

	union vec_union_i32
	{
		vec_int_t mi[interleave];
		int32 i[vec_elem];
		float f[vec_elem];
		uint32 u[vec_elem];
		vec_elem_t m[interleave];
	};

	union vec_union_f
	{
		float f[vec_elem];
		uint32 u[vec_elem];
		int32 i[vec_elem];
		vec_elem_t m[interleave];
		vec_int_t mi[interleave];
		vec_float mv;
		vec_union_f() {};
		vec_union_f(const vec_float& other) { mv = other; };
        vec_union_f(const vec_union_f& other) { mv = other.mv; };
		~vec_union_f() {};
	};


	static vforceinline void  get_union_f(const vec_float& obj, vec_union_f& result)
	{
		INTERLEAVED_INDEX_ASSIGN_T(result.m, obj.m);
	};

	static vforceinline vec_float  get_vector_f(const vec_union_f& obj)
	{
		vec_float result;
		INTERLEAVED_INDEX_ASSIGN_T(result.m, obj.m);
		return result;
	};
} __attribute__ ((aligned (32)));

#endif
