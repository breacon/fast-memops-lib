/*
 * memcpyw.c
 *
 *  Created on: Feb 22, 2025
 *      Author: breacon
 */

#include "..\\include\\nativelib.h"
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
#include <string.h>

__inline void __attribute__((__always_inline__)) memcpyw(void * __restrict__ dest, const void * __restrict__ src, size_t size)
{
	(void)memcpy(dest,src,size);
	return;
}

static void _aligned_native_memcpy_512(const void * __restrict__ src, void * __restrict__ dest, size_t size)
{
	_mm_prefetch(src,_MM_HINT_NTA);
	_mm_prefetch(dest,_MM_HINT_NTA);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+= 64)
	{
		_mm_prefetch((void*)(((char*)src)+byteIndex+64),_MM_HINT_NTA);
		_mm_prefetch((void*)(((char*)dest)+byteIndex+64),_MM_HINT_NTA);
		__m512 reg = _mm512_load_ps((float*)(((const char*__restrict__)src)+byteIndex));
		_mm512_store_ps((float*)(((const char*__restrict__)dest)+byteIndex),reg);
	}

}

static void _aligned_native_memcpy_256(const void * __restrict__ src, void * __restrict__ dest, size_t size)
{
	_mm_prefetch(src,_MM_HINT_NTA);
	_mm_prefetch(dest,_MM_HINT_NTA);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+= 32)
		{
//			_mm_prefetch(src+byteIndex,_MM_HINT_NTA);
			__m256 reg = _mm256_load_ps((float*)(((const char*__restrict__)src)+byteIndex));
			_mm256_store_ps((float*)(((const char*__restrict__)dest)+byteIndex),reg);
		}
}

static void _aligned_native_memcpy_128(const void * __restrict__ src, void * __restrict__ dest, size_t size)
{
	_mm_prefetch(src,_MM_HINT_NTA);
	_mm_prefetch(dest,_MM_HINT_NTA);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+= 16)
	{
#ifdef NT_MEMOPS_C23
		if([[unlikely]] byteIndex % 64 == 0)
		{
			_mm_prefetch((void*)(((char*)src)+byteIndex+64),_MM_HINT_T1);
			_mm_prefetch((void*)(((char*)dest)+byteIndex+64),_MM_HINT_T1);
		}
#endif
//		_mm_prefetch(src+byteIndex,_MM_HINT_NTA);
		__m128 reg = _mm_load_ps((float*)(((const char*__restrict__)src)+byteIndex));
		_mm_store_ps((float*)(((const char*__restrict__)dest)+byteIndex),reg);
	}
}

void aligned_native_memcpy(const void * __restrict__ src, void * __restrict__ dest, size_t size)
{
//	_mm_prefetch(((char*__restrict__)dest) + size - (size % (2<<(LVL + 2))));
//	_mm_prefetch(((char*__restrict__)dest) + size - (size % (2<<(LVL + 2))));
	void (*funcs[4])(const void * __restrict__, void * __restrict__,size_t) = {(void(*)(const void * __restrict__, void * __restrict__,size_t))&memcpyw,&_aligned_native_memcpy_128,&_aligned_native_memcpy_256,&_aligned_native_memcpy_512};
	(memcpy(((char*__restrict__)dest) + size - (size % (2<<(LVL + 2))) , ((const char*__restrict__)src) + size - (size % (2<<(LVL + 2))) , (size % (2<<(LVL + 2))) ) == dest)?(void)(1):(void)funcs[LVL](src,dest,size - (size%(2<<(LVL + 2))));
	return;
}


//prefetch opt in wrapper func!!!

static void _stream_native_memcpy_512(const void * __restrict__ src, void * __restrict__ dest, size_t size)
{
	__m512i reg;
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+=64)
	{
		reg = _mm512_stream_load_si512((void*)(((const char*__restrict__)src)+byteIndex));
		_mm512_stream_ps((float*)(((const char*__restrict__)dest)+byteIndex),(__m512)reg);
	}
}
static void _stream_native_memcpy_256(const void * __restrict__ src, void * __restrict__ dest, size_t size)
{
	__m256i reg;
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+=32)
	{
		reg = _mm256_stream_load_si256((void*)(((const char*__restrict__)src)+byteIndex));
		_mm256_stream_ps((float*)(((const char*__restrict__)dest)+byteIndex),(__m256)reg);
	}
}
static void _stream_native_memcpy_128(const void * __restrict__ src, void * __restrict__ dest, size_t size)
{
	__m128i reg;
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+=16)
	{
		reg = _mm_stream_load_si128((__m128i *)(((const char*__restrict__)src)+byteIndex));
		_mm_stream_ps((float*)(((const char*__restrict__)dest)+byteIndex),(__m128)reg);
	}
}

/*
 * this function uses the 'stream' family of store/load operations for intel's intrinsics/SIMD ops
 * this function does not cache the buffer during or after use
 * this function ONLY works on ALIGNED BUFFERS
 *
 * */
void stream_native_memcpy(const void * __restrict__ src, void * __restrict__ dest, size_t size)
{
	
	#ifdef __AVX2__
	void (*funcs[4])(const void * __restrict__, void * __restrict__, size_t) = {(void(*)(const void * __restrict__, void * __restrict__,size_t))&memcpyw,&_stream_native_memcpy_128,&_stream_native_memcpy_256,&_stream_native_memcpy_512};
	#else
	void (*funcs[4])(const void * __restrict__, void * __restrict__, size_t) = {(void(*)(const void * __restrict__, void * __restrict__,size_t))&memcpyw,&_stream_native_memcpy_128,&_stream_native_memcpy_128,&_stream_native_memcpy_512};
	#endif
	(memcpy(((char*__restrict__)dest) + size - (size % (2<<(LVL + 2))) , ((const char*__restrict__)src) + size - (size % (2<<(LVL + 2))) , (size % (2<<(LVL + 2))) ) == dest)?(void)(1):(void)funcs[LVL](src,dest,size - (size%(2<<(LVL + 2))));
	return;
}
//same as the non caching function except that it caches the beginning of the buffer after the operation to make it available and ready to use
void stream_native_memcpy_cache(const void * __restrict__ src, void * __restrict__ dest, size_t size)
{
	#ifdef __AVX2__
	void (*funcs[4])(const void * __restrict__, void * __restrict__, size_t) = {(void(*)(const void * __restrict__, void * __restrict__,size_t))&memcpyw,&_stream_native_memcpy_128,&_stream_native_memcpy_256,&_stream_native_memcpy_512};
	#else
	void (*funcs[4])(const void * __restrict__, void * __restrict__, size_t) = {(void(*)(const void * __restrict__, void * __restrict__,size_t))&memcpyw,&_stream_native_memcpy_128,&_stream_native_memcpy_128,&_stream_native_memcpy_512};
	#endif
	(memcpy(((char*__restrict__)dest) + size - (size % (2<<(LVL + 2))) , ((const char*__restrict__)src) + size - (size % (2<<(LVL + 2))) , (size % (2<<(LVL + 2))) ) == dest)?(void)(1):(void)funcs[LVL](src,dest,size - (size%(2<<(LVL + 2)))), _mm_prefetch(dest,_MM_HINT_T0);
	return;
}


static void _native_memcpy_512(const void * __restrict__ src, void * __restrict__ dest, size_t size)
{
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+= 64)
	{
//		_mm_prefetch(src+byteIndex,_MM_HINT_NTA);
		__m512 reg = _mm512_loadu_ps((float*)(((const char*__restrict__)src)+byteIndex));
		_mm512_storeu_ps((float*)(((const char*__restrict__)dest)+byteIndex),reg);
	}

}

static void _native_memcpy_256(const void * __restrict__ src, void * __restrict__ dest, size_t size)
{
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+= 32)
		{
//			_mm_prefetch(src+byteIndex,_MM_HINT_NTA);
			__m256 reg = _mm256_loadu_ps((float*)(((const char*__restrict__)src)+byteIndex));
			_mm256_storeu_ps((float*)(((const char*__restrict__)dest)+byteIndex),reg);
		}
}

static void _native_memcpy_128(const void * __restrict__ src, void * __restrict__ dest, size_t size)
{
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+= 16)
	{
//		_mm_prefetch(src+byteIndex,_MM_HINT_NTA);
		__m128 reg = _mm_loadu_ps((float*)(((const char*__restrict__)src)+byteIndex));
		_mm_storeu_ps((float*)(((const char*__restrict__)dest)+byteIndex),reg);
	}
}


void native_memcpy(const void * __restrict__ src, void * __restrict__ dest, size_t size)
{
	void (*funcs[4])(const void * __restrict__, void * __restrict__, size_t) = {(void(*)(const void * __restrict__, void * __restrict__,size_t))&memcpyw,&_native_memcpy_128,&_native_memcpy_256,&_native_memcpy_512};
	(memcpy(((char*__restrict__)dest) + size - (size % (2<<(LVL + 2))) , ((const char*__restrict__)src) + size - (size % (2<<(LVL + 2))) , (size % (2<<(LVL + 2))) ) == dest)?(void)(1):(void)funcs[LVL](src,dest,size - (size%(2<<(LVL + 2))));
	return;
}
