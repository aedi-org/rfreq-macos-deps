/* -*- c++ -*- */
/*
 * Copyright 2012, 2014 Free Software Foundation, Inc.
 *
 * This file is part of VOLK
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef INCLUDED_volk_8ic_x2_multiply_conjugate_16ic_a_H
#define INCLUDED_volk_8ic_x2_multiply_conjugate_16ic_a_H

#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <volk/volk_complex.h>

#ifdef LV_HAVE_AVX2
#include <immintrin.h>
/*!
  \brief Multiplys the one complex vector with the complex conjugate of the second complex
  vector and stores their results in the third vector \param cVector The complex vector
  where the results will be stored \param aVector One of the complex vectors to be
  multiplied \param bVector The complex vector which will be converted to complex
  conjugate and multiplied \param num_points The number of complex values in aVector and
  bVector to be multiplied together and stored into cVector
*/
static inline void volk_8ic_x2_multiply_conjugate_16ic_a_avx2(lv_16sc_t* cVector,
                                                              const lv_8sc_t* aVector,
                                                              const lv_8sc_t* bVector,
                                                              unsigned int num_points)
{
    unsigned int number = 0;
    const unsigned int quarterPoints = num_points / 8;

    __m256i x, y, realz, imagz;
    lv_16sc_t* c = cVector;
    const lv_8sc_t* a = aVector;
    const lv_8sc_t* b = bVector;
    __m256i conjugateSign =
        _mm256_set_epi16(-1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1);

    for (; number < quarterPoints; number++) {
        // Convert 8 bit values into 16 bit values
        x = _mm256_cvtepi8_epi16(_mm_load_si128((__m128i*)a));
        y = _mm256_cvtepi8_epi16(_mm_load_si128((__m128i*)b));

        // Calculate the ar*cr - ai*(-ci) portions
        realz = _mm256_madd_epi16(x, y);

        // Calculate the complex conjugate of the cr + ci j values
        y = _mm256_sign_epi16(y, conjugateSign);

        // Shift the order of the cr and ci values
        y = _mm256_shufflehi_epi16(_mm256_shufflelo_epi16(y, _MM_SHUFFLE(2, 3, 0, 1)),
                                   _MM_SHUFFLE(2, 3, 0, 1));

        // Calculate the ar*(-ci) + cr*(ai)
        imagz = _mm256_madd_epi16(x, y);

        // Perform the addition of products

        _mm256_store_si256((__m256i*)c,
                           _mm256_packs_epi32(_mm256_unpacklo_epi32(realz, imagz),
                                              _mm256_unpackhi_epi32(realz, imagz)));

        a += 8;
        b += 8;
        c += 8;
    }

    number = quarterPoints * 8;
    int16_t* c16Ptr = (int16_t*)&cVector[number];
    int8_t* a8Ptr = (int8_t*)&aVector[number];
    int8_t* b8Ptr = (int8_t*)&bVector[number];
    for (; number < num_points; number++) {
        float aReal = (float)*a8Ptr++;
        float aImag = (float)*a8Ptr++;
        lv_32fc_t aVal = lv_cmake(aReal, aImag);
        float bReal = (float)*b8Ptr++;
        float bImag = (float)*b8Ptr++;
        lv_32fc_t bVal = lv_cmake(bReal, -bImag);
        lv_32fc_t temp = aVal * bVal;

        *c16Ptr++ = (int16_t)(lv_creal(temp) > SHRT_MAX ? SHRT_MAX : lv_creal(temp));
        *c16Ptr++ = (int16_t)lv_cimag(temp);
    }
}
#endif /* LV_HAVE_AVX2 */


#ifdef LV_HAVE_SSE4_1
#include <smmintrin.h>
/*!
  \brief Multiplys the one complex vector with the complex conjugate of the second complex
  vector and stores their results in the third vector \param cVector The complex vector
  where the results will be stored \param aVector One of the complex vectors to be
  multiplied \param bVector The complex vector which will be converted to complex
  conjugate and multiplied \param num_points The number of complex values in aVector and
  bVector to be multiplied together and stored into cVector
*/
static inline void volk_8ic_x2_multiply_conjugate_16ic_a_sse4_1(lv_16sc_t* cVector,
                                                                const lv_8sc_t* aVector,
                                                                const lv_8sc_t* bVector,
                                                                unsigned int num_points)
{
    unsigned int number = 0;
    const unsigned int quarterPoints = num_points / 4;

    __m128i x, y, realz, imagz;
    lv_16sc_t* c = cVector;
    const lv_8sc_t* a = aVector;
    const lv_8sc_t* b = bVector;
    __m128i conjugateSign = _mm_set_epi16(-1, 1, -1, 1, -1, 1, -1, 1);

    for (; number < quarterPoints; number++) {
        // Convert into 8 bit values into 16 bit values
        x = _mm_cvtepi8_epi16(_mm_loadl_epi64((__m128i*)a));
        y = _mm_cvtepi8_epi16(_mm_loadl_epi64((__m128i*)b));

        // Calculate the ar*cr - ai*(-ci) portions
        realz = _mm_madd_epi16(x, y);

        // Calculate the complex conjugate of the cr + ci j values
        y = _mm_sign_epi16(y, conjugateSign);

        // Shift the order of the cr and ci values
        y = _mm_shufflehi_epi16(_mm_shufflelo_epi16(y, _MM_SHUFFLE(2, 3, 0, 1)),
                                _MM_SHUFFLE(2, 3, 0, 1));

        // Calculate the ar*(-ci) + cr*(ai)
        imagz = _mm_madd_epi16(x, y);

        _mm_store_si128((__m128i*)c,
                        _mm_packs_epi32(_mm_unpacklo_epi32(realz, imagz),
                                        _mm_unpackhi_epi32(realz, imagz)));

        a += 4;
        b += 4;
        c += 4;
    }

    number = quarterPoints * 4;
    int16_t* c16Ptr = (int16_t*)&cVector[number];
    int8_t* a8Ptr = (int8_t*)&aVector[number];
    int8_t* b8Ptr = (int8_t*)&bVector[number];
    for (; number < num_points; number++) {
        float aReal = (float)*a8Ptr++;
        float aImag = (float)*a8Ptr++;
        lv_32fc_t aVal = lv_cmake(aReal, aImag);
        float bReal = (float)*b8Ptr++;
        float bImag = (float)*b8Ptr++;
        lv_32fc_t bVal = lv_cmake(bReal, -bImag);
        lv_32fc_t temp = aVal * bVal;

        *c16Ptr++ = (int16_t)(lv_creal(temp) > SHRT_MAX ? SHRT_MAX : lv_creal(temp));
        *c16Ptr++ = (int16_t)lv_cimag(temp);
    }
}
#endif /* LV_HAVE_SSE4_1 */

#ifdef LV_HAVE_GENERIC
/*!
  \brief Multiplys the one complex vector with the complex conjugate of the second complex
  vector and stores their results in the third vector \param cVector The complex vector
  where the results will be stored \param aVector One of the complex vectors to be
  multiplied \param bVector The complex vector which will be converted to complex
  conjugate and multiplied \param num_points The number of complex values in aVector and
  bVector to be multiplied together and stored into cVector
*/
static inline void volk_8ic_x2_multiply_conjugate_16ic_generic(lv_16sc_t* cVector,
                                                               const lv_8sc_t* aVector,
                                                               const lv_8sc_t* bVector,
                                                               unsigned int num_points)
{
    unsigned int number = 0;
    int16_t* c16Ptr = (int16_t*)cVector;
    int8_t* a8Ptr = (int8_t*)aVector;
    int8_t* b8Ptr = (int8_t*)bVector;
    for (number = 0; number < num_points; number++) {
        float aReal = (float)*a8Ptr++;
        float aImag = (float)*a8Ptr++;
        lv_32fc_t aVal = lv_cmake(aReal, aImag);
        float bReal = (float)*b8Ptr++;
        float bImag = (float)*b8Ptr++;
        lv_32fc_t bVal = lv_cmake(bReal, -bImag);
        lv_32fc_t temp = aVal * bVal;

        *c16Ptr++ = (int16_t)(lv_creal(temp) > SHRT_MAX ? SHRT_MAX : lv_creal(temp));
        *c16Ptr++ = (int16_t)lv_cimag(temp);
    }
}
#endif /* LV_HAVE_GENERIC */

#endif /* INCLUDED_volk_8ic_x2_multiply_conjugate_16ic_a_H */

#ifndef INCLUDED_volk_8ic_x2_multiply_conjugate_16ic_u_H
#define INCLUDED_volk_8ic_x2_multiply_conjugate_16ic_u_H

#include <inttypes.h>
#include <stdio.h>
#include <volk/volk_complex.h>

#ifdef LV_HAVE_AVX2
#include <immintrin.h>
/*!
  \brief Multiplys the one complex vector with the complex conjugate of the second complex
  vector and stores their results in the third vector \param cVector The complex vector
  where the results will be stored \param aVector One of the complex vectors to be
  multiplied \param bVector The complex vector which will be converted to complex
  conjugate and multiplied \param num_points The number of complex values in aVector and
  bVector to be multiplied together and stored into cVector
*/
static inline void volk_8ic_x2_multiply_conjugate_16ic_u_avx2(lv_16sc_t* cVector,
                                                              const lv_8sc_t* aVector,
                                                              const lv_8sc_t* bVector,
                                                              unsigned int num_points)
{
    unsigned int number = 0;
    const unsigned int oneEigthPoints = num_points / 8;

    __m256i x, y, realz, imagz;
    lv_16sc_t* c = cVector;
    const lv_8sc_t* a = aVector;
    const lv_8sc_t* b = bVector;
    __m256i conjugateSign =
        _mm256_set_epi16(-1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1);

    for (; number < oneEigthPoints; number++) {
        // Convert 8 bit values into 16 bit values
        x = _mm256_cvtepi8_epi16(_mm_loadu_si128((__m128i*)a));
        y = _mm256_cvtepi8_epi16(_mm_loadu_si128((__m128i*)b));

        // Calculate the ar*cr - ai*(-ci) portions
        realz = _mm256_madd_epi16(x, y);

        // Calculate the complex conjugate of the cr + ci j values
        y = _mm256_sign_epi16(y, conjugateSign);

        // Shift the order of the cr and ci values
        y = _mm256_shufflehi_epi16(_mm256_shufflelo_epi16(y, _MM_SHUFFLE(2, 3, 0, 1)),
                                   _MM_SHUFFLE(2, 3, 0, 1));

        // Calculate the ar*(-ci) + cr*(ai)
        imagz = _mm256_madd_epi16(x, y);

        // Perform the addition of products

        _mm256_storeu_si256((__m256i*)c,
                            _mm256_packs_epi32(_mm256_unpacklo_epi32(realz, imagz),
                                               _mm256_unpackhi_epi32(realz, imagz)));

        a += 8;
        b += 8;
        c += 8;
    }

    number = oneEigthPoints * 8;
    int16_t* c16Ptr = (int16_t*)&cVector[number];
    int8_t* a8Ptr = (int8_t*)&aVector[number];
    int8_t* b8Ptr = (int8_t*)&bVector[number];
    for (; number < num_points; number++) {
        float aReal = (float)*a8Ptr++;
        float aImag = (float)*a8Ptr++;
        lv_32fc_t aVal = lv_cmake(aReal, aImag);
        float bReal = (float)*b8Ptr++;
        float bImag = (float)*b8Ptr++;
        lv_32fc_t bVal = lv_cmake(bReal, -bImag);
        lv_32fc_t temp = aVal * bVal;

        *c16Ptr++ = (int16_t)(lv_creal(temp) > SHRT_MAX ? SHRT_MAX : lv_creal(temp));
        *c16Ptr++ = (int16_t)lv_cimag(temp);
    }
}
#endif /* LV_HAVE_AVX2 */

#ifdef LV_HAVE_RVV
#include <riscv_vector.h>

static inline void volk_8ic_x2_multiply_conjugate_16ic_rvv(lv_16sc_t* cVector,
                                                           const lv_8sc_t* aVector,
                                                           const lv_8sc_t* bVector,
                                                           unsigned int num_points)
{
    size_t n = num_points;
    for (size_t vl; n > 0; n -= vl, aVector += vl, bVector += vl, cVector += vl) {
        vl = __riscv_vsetvl_e8m2(n);
        vint16m4_t va = __riscv_vle16_v_i16m4((const int16_t*)aVector, vl);
        vint16m4_t vb = __riscv_vle16_v_i16m4((const int16_t*)bVector, vl);
        vint8m2_t var = __riscv_vnsra(va, 0, vl), vai = __riscv_vnsra(va, 8, vl);
        vint8m2_t vbr = __riscv_vnsra(vb, 0, vl), vbi = __riscv_vnsra(vb, 8, vl);
        vint16m4_t vr = __riscv_vwmacc(__riscv_vwmul(var, vbr, vl), vai, vbi, vl);
        vint16m4_t vi =
            __riscv_vsub(__riscv_vwmul(vai, vbr, vl), __riscv_vwmul(var, vbi, vl), vl);
        vuint16m4_t vru = __riscv_vreinterpret_u16m4(vr);
        vuint16m4_t viu = __riscv_vreinterpret_u16m4(vi);
        vuint32m8_t v = __riscv_vwmaccu(__riscv_vwaddu_vv(vru, viu, vl), 0xFFFF, viu, vl);
        __riscv_vse32((uint32_t*)cVector, v, vl);
    }
}
#endif /*LV_HAVE_RVV*/

#ifdef LV_HAVE_RVVSEG
#include <riscv_vector.h>

static inline void volk_8ic_x2_multiply_conjugate_16ic_rvvseg(lv_16sc_t* cVector,
                                                              const lv_8sc_t* aVector,
                                                              const lv_8sc_t* bVector,
                                                              unsigned int num_points)
{
    size_t n = num_points;
    for (size_t vl; n > 0; n -= vl, aVector += vl, bVector += vl, cVector += vl) {
        vl = __riscv_vsetvl_e8m2(n);
        vint8m2x2_t va = __riscv_vlseg2e8_v_i8m2x2((const int8_t*)aVector, vl);
        vint8m2x2_t vb = __riscv_vlseg2e8_v_i8m2x2((const int8_t*)bVector, vl);
        vint8m2_t var = __riscv_vget_i8m2(va, 0), vai = __riscv_vget_i8m2(va, 1);
        vint8m2_t vbr = __riscv_vget_i8m2(vb, 0), vbi = __riscv_vget_i8m2(vb, 1);
        vint16m4_t vr = __riscv_vwmacc(__riscv_vwmul(var, vbr, vl), vai, vbi, vl);
        vint16m4_t vi =
            __riscv_vsub(__riscv_vwmul(vai, vbr, vl), __riscv_vwmul(var, vbi, vl), vl);
        __riscv_vsseg2e16_v_i16m4x2(
            (int16_t*)cVector, __riscv_vcreate_v_i16m4x2(vr, vi), vl);
    }
}

#endif /*LV_HAVE_RVVSEG*/

#endif /* INCLUDED_volk_8ic_x2_multiply_conjugate_16ic_u_H */
