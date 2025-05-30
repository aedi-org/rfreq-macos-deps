/* -*- c++ -*- */
/*
 * Copyright 2014 Free Software Foundation, Inc.
 *
 * This file is part of VOLK
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

/*!
 * \page volk_32f_tanh_32f
 *
 * \b Overview
 *
 * Computes the hyperbolic tangent of each element of the aVector:
 *
 * c[i] = tanh(a[i])
 *
 * <b>Dispatcher Prototype</b>
 * \code
 * void volk_32f_tanh_32f(float* cVector, const float* aVector, unsigned int num_points)
 * \endcode
 *
 * \b Inputs
 * \li aVector: The buffer of points.
 * \li num_points: The number of values in input buffer.
 *
 * \b Outputs
 * \li cVector: The output buffer.
 *
 * \b Example
 * \code
 *   int N = 10;
 *   unsigned int alignment = volk_get_alignment();
 *   float* in = (float*)volk_malloc(sizeof(float)*N, alignment);
 *   float* out = (float*)volk_malloc(sizeof(float)*N, alignment);
 *
 *   for(unsigned int ii = 0; ii < N; ++ii){
 *       // the approximate artanh(x) for x<1
 *       float x = (float)ii / (float)N;
 *       in[ii] = 0.5 * std::log((1.f+x)/(1.f-x));
 *   }
 *
 *   volk_32f_tanh_32f(out, in, N);
 *
 *   for(unsigned int ii = 0; ii < N; ++ii){
 *       printf("out(%i) = %f\n", ii, out[ii]);
 *   }
 *
 *   volk_free(in);
 *   volk_free(out);
 * \endcode
 */

#ifndef INCLUDED_volk_32f_tanh_32f_a_H
#define INCLUDED_volk_32f_tanh_32f_a_H

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>


#ifdef LV_HAVE_GENERIC

static inline void
volk_32f_tanh_32f_generic(float* cVector, const float* aVector, unsigned int num_points)
{
    unsigned int number = 0;
    float* cPtr = cVector;
    const float* aPtr = aVector;
    for (; number < num_points; number++) {
        *cPtr++ = tanhf(*aPtr++);
    }
}

#endif /* LV_HAVE_GENERIC */


#ifdef LV_HAVE_GENERIC

static inline void
volk_32f_tanh_32f_series(float* cVector, const float* aVector, unsigned int num_points)
{
    float* cPtr = cVector;
    const float* aPtr = aVector;
    for (unsigned int number = 0; number < num_points; number++) {
        if (*aPtr > 4.97)
            *cPtr++ = 1;
        else if (*aPtr <= -4.97)
            *cPtr++ = -1;
        else {
            float x2 = (*aPtr) * (*aPtr);
            float a = (*aPtr) * (135135.0f + x2 * (17325.0f + x2 * (378.0f + x2)));
            float b = 135135.0f + x2 * (62370.0f + x2 * (3150.0f + x2 * 28.0f));
            *cPtr++ = a / b;
            aPtr++;
        }
    }
}

#endif /* LV_HAVE_GENERIC */


#ifdef LV_HAVE_SSE
#include <xmmintrin.h>

static inline void
volk_32f_tanh_32f_a_sse(float* cVector, const float* aVector, unsigned int num_points)
{
    unsigned int number = 0;
    const unsigned int quarterPoints = num_points / 4;

    float* cPtr = cVector;
    const float* aPtr = aVector;

    __m128 aVal, cVal, x2, a, b;
    __m128 const1, const2, const3, const4, const5, const6;
    const1 = _mm_set_ps1(135135.0f);
    const2 = _mm_set_ps1(17325.0f);
    const3 = _mm_set_ps1(378.0f);
    const4 = _mm_set_ps1(62370.0f);
    const5 = _mm_set_ps1(3150.0f);
    const6 = _mm_set_ps1(28.0f);
    for (; number < quarterPoints; number++) {

        aVal = _mm_load_ps(aPtr);
        x2 = _mm_mul_ps(aVal, aVal);
        a = _mm_mul_ps(
            aVal,
            _mm_add_ps(
                const1,
                _mm_mul_ps(x2,
                           _mm_add_ps(const2, _mm_mul_ps(x2, _mm_add_ps(const3, x2))))));
        b = _mm_add_ps(
            const1,
            _mm_mul_ps(
                x2,
                _mm_add_ps(const4,
                           _mm_mul_ps(x2, _mm_add_ps(const5, _mm_mul_ps(x2, const6))))));

        cVal = _mm_div_ps(a, b);

        _mm_store_ps(cPtr, cVal); // Store the results back into the C container

        aPtr += 4;
        cPtr += 4;
    }

    number = quarterPoints * 4;
    volk_32f_tanh_32f_series(cPtr, aPtr, num_points - number);
}
#endif /* LV_HAVE_SSE */


#ifdef LV_HAVE_AVX
#include <immintrin.h>

static inline void
volk_32f_tanh_32f_a_avx(float* cVector, const float* aVector, unsigned int num_points)
{
    unsigned int number = 0;
    const unsigned int eighthPoints = num_points / 8;

    float* cPtr = cVector;
    const float* aPtr = aVector;

    __m256 aVal, cVal, x2, a, b;
    __m256 const1, const2, const3, const4, const5, const6;
    const1 = _mm256_set1_ps(135135.0f);
    const2 = _mm256_set1_ps(17325.0f);
    const3 = _mm256_set1_ps(378.0f);
    const4 = _mm256_set1_ps(62370.0f);
    const5 = _mm256_set1_ps(3150.0f);
    const6 = _mm256_set1_ps(28.0f);
    for (; number < eighthPoints; number++) {

        aVal = _mm256_load_ps(aPtr);
        x2 = _mm256_mul_ps(aVal, aVal);
        a = _mm256_mul_ps(
            aVal,
            _mm256_add_ps(
                const1,
                _mm256_mul_ps(
                    x2,
                    _mm256_add_ps(const2,
                                  _mm256_mul_ps(x2, _mm256_add_ps(const3, x2))))));
        b = _mm256_add_ps(
            const1,
            _mm256_mul_ps(
                x2,
                _mm256_add_ps(
                    const4,
                    _mm256_mul_ps(x2,
                                  _mm256_add_ps(const5, _mm256_mul_ps(x2, const6))))));

        cVal = _mm256_div_ps(a, b);

        _mm256_store_ps(cPtr, cVal); // Store the results back into the C container

        aPtr += 8;
        cPtr += 8;
    }

    number = eighthPoints * 8;
    volk_32f_tanh_32f_series(cPtr, aPtr, num_points - number);
}
#endif /* LV_HAVE_AVX */

#if LV_HAVE_AVX && LV_HAVE_FMA
#include <immintrin.h>

static inline void
volk_32f_tanh_32f_a_avx_fma(float* cVector, const float* aVector, unsigned int num_points)
{
    unsigned int number = 0;
    const unsigned int eighthPoints = num_points / 8;

    float* cPtr = cVector;
    const float* aPtr = aVector;

    __m256 aVal, cVal, x2, a, b;
    __m256 const1, const2, const3, const4, const5, const6;
    const1 = _mm256_set1_ps(135135.0f);
    const2 = _mm256_set1_ps(17325.0f);
    const3 = _mm256_set1_ps(378.0f);
    const4 = _mm256_set1_ps(62370.0f);
    const5 = _mm256_set1_ps(3150.0f);
    const6 = _mm256_set1_ps(28.0f);
    for (; number < eighthPoints; number++) {

        aVal = _mm256_load_ps(aPtr);
        x2 = _mm256_mul_ps(aVal, aVal);
        a = _mm256_mul_ps(
            aVal,
            _mm256_fmadd_ps(
                x2, _mm256_fmadd_ps(x2, _mm256_add_ps(const3, x2), const2), const1));
        b = _mm256_fmadd_ps(
            x2, _mm256_fmadd_ps(x2, _mm256_fmadd_ps(x2, const6, const5), const4), const1);

        cVal = _mm256_div_ps(a, b);

        _mm256_store_ps(cPtr, cVal); // Store the results back into the C container

        aPtr += 8;
        cPtr += 8;
    }

    number = eighthPoints * 8;
    volk_32f_tanh_32f_series(cPtr, aPtr, num_points - number);
}
#endif /* LV_HAVE_AVX && LV_HAVE_FMA */

#endif /* INCLUDED_volk_32f_tanh_32f_a_H */


#ifndef INCLUDED_volk_32f_tanh_32f_u_H
#define INCLUDED_volk_32f_tanh_32f_u_H

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>


#ifdef LV_HAVE_SSE
#include <xmmintrin.h>

static inline void
volk_32f_tanh_32f_u_sse(float* cVector, const float* aVector, unsigned int num_points)
{
    unsigned int number = 0;
    const unsigned int quarterPoints = num_points / 4;

    float* cPtr = cVector;
    const float* aPtr = aVector;

    __m128 aVal, cVal, x2, a, b;
    __m128 const1, const2, const3, const4, const5, const6;
    const1 = _mm_set_ps1(135135.0f);
    const2 = _mm_set_ps1(17325.0f);
    const3 = _mm_set_ps1(378.0f);
    const4 = _mm_set_ps1(62370.0f);
    const5 = _mm_set_ps1(3150.0f);
    const6 = _mm_set_ps1(28.0f);
    for (; number < quarterPoints; number++) {

        aVal = _mm_loadu_ps(aPtr);
        x2 = _mm_mul_ps(aVal, aVal);
        a = _mm_mul_ps(
            aVal,
            _mm_add_ps(
                const1,
                _mm_mul_ps(x2,
                           _mm_add_ps(const2, _mm_mul_ps(x2, _mm_add_ps(const3, x2))))));
        b = _mm_add_ps(
            const1,
            _mm_mul_ps(
                x2,
                _mm_add_ps(const4,
                           _mm_mul_ps(x2, _mm_add_ps(const5, _mm_mul_ps(x2, const6))))));

        cVal = _mm_div_ps(a, b);

        _mm_storeu_ps(cPtr, cVal); // Store the results back into the C container

        aPtr += 4;
        cPtr += 4;
    }

    number = quarterPoints * 4;
    volk_32f_tanh_32f_series(cPtr, aPtr, num_points - number);
}
#endif /* LV_HAVE_SSE */


#ifdef LV_HAVE_AVX
#include <immintrin.h>

static inline void
volk_32f_tanh_32f_u_avx(float* cVector, const float* aVector, unsigned int num_points)
{
    unsigned int number = 0;
    const unsigned int eighthPoints = num_points / 8;

    float* cPtr = cVector;
    const float* aPtr = aVector;

    __m256 aVal, cVal, x2, a, b;
    __m256 const1, const2, const3, const4, const5, const6;
    const1 = _mm256_set1_ps(135135.0f);
    const2 = _mm256_set1_ps(17325.0f);
    const3 = _mm256_set1_ps(378.0f);
    const4 = _mm256_set1_ps(62370.0f);
    const5 = _mm256_set1_ps(3150.0f);
    const6 = _mm256_set1_ps(28.0f);
    for (; number < eighthPoints; number++) {

        aVal = _mm256_loadu_ps(aPtr);
        x2 = _mm256_mul_ps(aVal, aVal);
        a = _mm256_mul_ps(
            aVal,
            _mm256_add_ps(
                const1,
                _mm256_mul_ps(
                    x2,
                    _mm256_add_ps(const2,
                                  _mm256_mul_ps(x2, _mm256_add_ps(const3, x2))))));
        b = _mm256_add_ps(
            const1,
            _mm256_mul_ps(
                x2,
                _mm256_add_ps(
                    const4,
                    _mm256_mul_ps(x2,
                                  _mm256_add_ps(const5, _mm256_mul_ps(x2, const6))))));

        cVal = _mm256_div_ps(a, b);

        _mm256_storeu_ps(cPtr, cVal); // Store the results back into the C container

        aPtr += 8;
        cPtr += 8;
    }

    number = eighthPoints * 8;
    volk_32f_tanh_32f_series(cPtr, aPtr, num_points - number);
}
#endif /* LV_HAVE_AVX */

#if LV_HAVE_AVX && LV_HAVE_FMA
#include <immintrin.h>

static inline void
volk_32f_tanh_32f_u_avx_fma(float* cVector, const float* aVector, unsigned int num_points)
{
    unsigned int number = 0;
    const unsigned int eighthPoints = num_points / 8;

    float* cPtr = cVector;
    const float* aPtr = aVector;

    __m256 aVal, cVal, x2, a, b;
    __m256 const1, const2, const3, const4, const5, const6;
    const1 = _mm256_set1_ps(135135.0f);
    const2 = _mm256_set1_ps(17325.0f);
    const3 = _mm256_set1_ps(378.0f);
    const4 = _mm256_set1_ps(62370.0f);
    const5 = _mm256_set1_ps(3150.0f);
    const6 = _mm256_set1_ps(28.0f);
    for (; number < eighthPoints; number++) {

        aVal = _mm256_loadu_ps(aPtr);
        x2 = _mm256_mul_ps(aVal, aVal);
        a = _mm256_mul_ps(
            aVal,
            _mm256_fmadd_ps(
                x2, _mm256_fmadd_ps(x2, _mm256_add_ps(const3, x2), const2), const1));
        b = _mm256_fmadd_ps(
            x2, _mm256_fmadd_ps(x2, _mm256_fmadd_ps(x2, const6, const5), const4), const1);

        cVal = _mm256_div_ps(a, b);

        _mm256_storeu_ps(cPtr, cVal); // Store the results back into the C container

        aPtr += 8;
        cPtr += 8;
    }

    number = eighthPoints * 8;
    volk_32f_tanh_32f_series(cPtr, aPtr, num_points - number);
}
#endif /* LV_HAVE_AVX && LV_HAVE_FMA */

#ifdef LV_HAVE_RVV
#include <riscv_vector.h>

static inline void
volk_32f_tanh_32f_rvv(float* bVector, const float* aVector, unsigned int num_points)
{
    size_t vlmax = __riscv_vsetvlmax_e32m2();

    const vfloat32m2_t c1 = __riscv_vfmv_v_f_f32m2(135135.0f, vlmax);
    const vfloat32m2_t c2 = __riscv_vfmv_v_f_f32m2(17325.0f, vlmax);
    const vfloat32m2_t c3 = __riscv_vfmv_v_f_f32m2(378.0f, vlmax);
    const vfloat32m2_t c4 = __riscv_vfmv_v_f_f32m2(62370.0f, vlmax);
    const vfloat32m2_t c5 = __riscv_vfmv_v_f_f32m2(3150.0f, vlmax);
    const vfloat32m2_t c6 = __riscv_vfmv_v_f_f32m2(28.0f, vlmax);

    size_t n = num_points;
    for (size_t vl; n > 0; n -= vl, aVector += vl, bVector += vl) {
        vl = __riscv_vsetvl_e32m2(n);
        vfloat32m2_t x = __riscv_vle32_v_f32m2(aVector, vl);
        vfloat32m2_t xx = __riscv_vfmul(x, x, vl);
        vfloat32m2_t a, b;
        a = __riscv_vfadd(xx, c3, vl);
        a = __riscv_vfmadd(a, xx, c2, vl);
        a = __riscv_vfmadd(a, xx, c1, vl);
        a = __riscv_vfmul(a, x, vl);
        b = c6;
        b = __riscv_vfmadd(b, xx, c5, vl);
        b = __riscv_vfmadd(b, xx, c4, vl);
        b = __riscv_vfmadd(b, xx, c1, vl);
        __riscv_vse32(bVector, __riscv_vfdiv(a, b, vl), vl);
    }
}
#endif /*LV_HAVE_RVV*/

#endif /* INCLUDED_volk_32f_tanh_32f_u_H */
