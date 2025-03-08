/*
 * native_memops.h
 *
 *  Created on: Feb 28, 2025
 *      Author: breacon
 */

#ifndef NATIVE_MEMOPS_H_
#define NATIVE_MEMOPS_H_

#include <windows.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <immintrin.h>
#ifdef _MSC_VER
#include <intrin.h> // For __cpuid
#else
#include <cpuid.h>  // For __get_cpuid
#endif
#include "nativelib.h"
#include "memcpy.h"
#include "memset.h"
#include "memcmp.h"

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L)
    
    #if defined(__AVX__) && !defined(__AVX2__) && !defined(__AVX512F__)
        #warning "native-memops: WARNING: AVX2 not supported, some stream functions might work less efficiently"
    #endif
    
#endif
#endif /* NATIVE_MEMOPS_H_ */
