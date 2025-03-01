/*
 * nativelib.h
 *
 *  Created on: Feb 22, 2025
 *      Author: breacon
 */

#ifndef NATIVELIB_H_
#define NATIVELIB_H_

#include <windows.h>
#define WIN32_LEAN_AND_MEAN
#include <immintrin.h>
#ifdef _MSC_VER
#include <intrin.h> // For __cpuid
#else
#include <cpuid.h>  // For __get_cpuid
#endif

typedef unsigned long long int size_t;

#ifdef __AVXF512__
#define LVL 3
#else// NO AVX512
#ifdef __AVX__
#define LVL 2
#else//NO AVX
#ifdef __SSE_4_2__
#define LVL 1
#else// NO SSE4.2
#define LVL 0
#endif /* __SSE_4_2__ */
#endif /* __AVX__ */
#endif /* __AVXF512__ */

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L)
#define NT_MEMOPS_C23
#endif

//#if !defined(__SSE_4_2__) && defined(__GNUC__)
//#define GNUC_OVER_SSE
//#define LVL 4
//#endif

extern int _get_cache_line_size(int cache_level);
extern int address_aligned(void * address, unsigned int alignment);

#endif /* NATIVELIB_H_ */
