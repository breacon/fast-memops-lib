/*
 * memset.c
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

typedef union conversion
{
	float f_t;
	int i_t;

}Conv;

__inline void __attribute__((__always_inline__)) memsetw(void * __restrict__ buffer, int byteValue, const size_t size)
{
	(void)memset(buffer,byteValue,size);
	return;
}

static void _aligned_native_memset_512(void * __restrict__ buffer, int byteValue, const size_t size)
{
	Conv tbyteValue = {.i_t=(byteValue & 0x000000FF)};
	tbyteValue.i_t += (tbyteValue.i_t<<8) + (tbyteValue.i_t<<16) + (tbyteValue.i_t<<24);
	__m512 reg = _mm512_set1_ps(tbyteValue.f_t);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+=64)
	{
//		_mm_prefetch(((char*__restrict__)buffer)+byteIndex,_MM_HINT_NTA);
		_mm512_store_ps((float *)(((char*__restrict__)buffer)+byteIndex),reg);
	}
}
static void _aligned_native_memset_256(void * __restrict__ buffer, int byteValue, const size_t size)
{
//	__m256 reg = _mm512_set1_ps(*((const float*)(&(byteValue & 0x000000FF))));
//	int tbyteValue = (byteValue & 0x000000FF);
	Conv tbyteValue = {.i_t=(byteValue & 0x000000FF)};
	tbyteValue.i_t += (tbyteValue.i_t<<8) + (tbyteValue.i_t<<16) + (tbyteValue.i_t<<24);
	__m256 reg = _mm256_set1_ps(tbyteValue.f_t);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+=32)
	{
//		_mm_prefetch(((char*__restrict__)buffer)+byteIndex,_MM_HINT_NTA);
		_mm256_store_ps((float *)(((char*__restrict__)buffer)+byteIndex),reg);
	}
}
static void _aligned_native_memset_128(void * __restrict__ buffer, int byteValue, const size_t size)
{
//	__m128 reg = _mm_set1_ps(*((const float*)(&(byteValue & 0x000000FF))));
//	int tbyteValue = (byteValue & 0x000000FF);
	Conv tbyteValue = {.i_t=(byteValue & 0x000000FF)};
	tbyteValue.i_t += (tbyteValue.i_t<<8) + (tbyteValue.i_t<<16) + (tbyteValue.i_t<<24);
	__m128 reg = _mm_set1_ps(tbyteValue.f_t);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+=16)
	{
#ifdef NT_MEMOPS_C23
		if([[unlikely]] byteIndex % 64 == 0)
		{
			_mm_prefetch((void*)(((char*)src)+byteIndex+64),_MM_HINT_T1);
			_mm_prefetch((void*)(((char*)dest)+byteIndex+64),_MM_HINT_T1);
		}
#endif
//		_mm_prefetch(((char*__restrict__)buffer)+byteIndex,_MM_HINT_NTA);
		_mm_store_ps((float *)(((char*__restrict__)buffer)+byteIndex),reg);
	}
}

void aligned_native_memset(void * __restrict__ buffer, int byteValue, const size_t size)
{
	void (*funcs[4])(void*__restrict__,int,size_t) = {&memsetw,&_aligned_native_memset_128,&_aligned_native_memset_256,&_aligned_native_memset_512};
	(memset((void*)(((char*)buffer)+size - (size%(2<<(LVL+2)))),byteValue,size%(2<<(LVL+2))) == buffer)?(void)(1):(void)funcs[LVL](buffer,byteValue,size - (size%(2<<(LVL+2))));
	return;
}

static void _stream_native_memset_512(void * __restrict__ buffer, int byteValue, const size_t size)
{
//	__m512 reg = _mm512_set1_ps(*((const float*)(&(byteValue & 0x000000FF))));
//	int tbyteValue = (byteValue & 0x000000FF);
	Conv tbyteValue = {.i_t=(byteValue & 0x000000FF)};
	tbyteValue.i_t += (tbyteValue.i_t<<8) + (tbyteValue.i_t<<16) + (tbyteValue.i_t<<24);
	__m512 reg = _mm512_set1_ps(tbyteValue.f_t);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+=64)
	{
		_mm512_stream_ps((float *)(((char*__restrict__)buffer)+byteIndex),reg);
	}

}

static void _stream_native_memset_256(void * __restrict__ buffer, int byteValue, const size_t size)
{
//	__m256 reg = _mm512_set1_ps(*((const float*)(&(byteValue & 0x000000FF))));
	Conv tbyteValue = {.i_t=(byteValue & 0x000000FF)};
	tbyteValue.i_t += (tbyteValue.i_t<<8) + (tbyteValue.i_t<<16) + (tbyteValue.i_t<<24);
	__m256 reg = _mm256_set1_ps(tbyteValue.f_t);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+=32)
	{
		_mm256_stream_ps((float *)(((char*__restrict__)buffer)+byteIndex),reg);
	}

}

static void _stream_native_memset_128(void * __restrict__ buffer, int byteValue, const size_t size)
{
//	__m128 reg = _mm_set1_ps(*((const float*)(&(byteValue & 0x000000FF))));
//	int tbyteValue = (byteValue & 0x000000FF);
//	__m128 reg = _mm_set1_ps(*((float*const)(&tbyteValue)));
	Conv tbyteValue = {.i_t=(byteValue & 0x000000FF)};
	tbyteValue.i_t += (tbyteValue.i_t<<8) + (tbyteValue.i_t<<16) + (tbyteValue.i_t<<24);
	__m128 reg = _mm_set1_ps(tbyteValue.f_t);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+=16)
	{
		_mm_stream_ps((float *)(((char*__restrict__)buffer)+byteIndex),reg);
	}

}

void stream_native_memset(void * __restrict__ buffer, int byteValue, const size_t size)
{
	void (*funcs[4])(void*__restrict__,int,const size_t) = {&memsetw,&_stream_native_memset_128,&_stream_native_memset_256,&_stream_native_memset_512};
	( memset((void*)(((char*)buffer) + size - (size % (2<<(LVL+2)))),byteValue,size % (2<<(LVL+2))) == buffer )?(void)1:funcs[LVL](buffer,byteValue,size - (size%(2<<(LVL+2))));
	return;
}
void stream_native_memset_cache(void * __restrict__ buffer, int byteValue, const size_t size)
{
	void (*funcs[4])(void*__restrict__,int,const size_t) = {&memsetw,&_stream_native_memset_128,&_stream_native_memset_256,&_stream_native_memset_512};
	( memset((void*)(((char*)buffer) + size - (size % (2<<(LVL+2)))),byteValue,size % (2<<(LVL+2))) == buffer )?(void)1:funcs[LVL](buffer,byteValue,size - (size%(2<<(LVL+2)))), _mm_prefetch(buffer,_MM_HINT_T0);
	return;
}

static void _native_memset_512(void * __restrict__ buffer, int byteValue, const size_t size)
{
//	__m512 reg = _mm512_set1_ps(*((const float*)(&(byteValue & 0x000000FF))));
//	int tbyteValue = (byteValue & 0x000000FF);
//	int tbyteValue = (byteValue & 0x000000FF);
//	tbyteValue += (tbyteValue<<8) + (tbyteValue<<16) + (tbyteValue<<24);
//	__m512 reg = _mm512_set1_ps(*((float*const)(&tbyteValue)));
	Conv tbyteValue = {.i_t=(byteValue & 0x000000FF)};
	tbyteValue.i_t += (tbyteValue.i_t<<8) + (tbyteValue.i_t<<16) + (tbyteValue.i_t<<24);
	__m512 reg = _mm512_set1_ps(tbyteValue.f_t);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+=64)
	{
		_mm512_storeu_ps((float *)(((char*__restrict__)buffer)+byteIndex),reg);
	}
}
static void _native_memset_256(void * __restrict__ buffer, int byteValue, const size_t size)
{
//	__m256 reg = _mm256_set1_ps(*((const float*)(&(byteValue & 0x000000FF))));
//	int tbyteValue = (byteValue & 0x000000FF);
	Conv tbyteValue = {.i_t=(byteValue & 0x000000FF)};
	tbyteValue.i_t += (tbyteValue.i_t<<8) + (tbyteValue.i_t<<16) + (tbyteValue.i_t<<24);
	__m256 reg = _mm256_set1_ps(tbyteValue.f_t);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+=32)
	{
		_mm256_storeu_ps((float *)(((char*__restrict__)buffer)+byteIndex),reg);
	}
}
static void _native_memset_128(void * __restrict__ buffer, int byteValue, const size_t size)
{
//	__m128 reg = _mm_set1_ps(*((const float*)(&(byteValue & 0x000000FF))));
//	int tbyteValue = (byteValue & 0x000000FF);
	Conv tbyteValue = {.i_t=(byteValue & 0x000000FF)};
	tbyteValue.i_t += (tbyteValue.i_t<<8) + (tbyteValue.i_t<<16) + (tbyteValue.i_t<<24);
	__m128 reg = _mm_set1_ps(tbyteValue.f_t);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex+=16)
	{
		_mm_storeu_ps((float *)(((char*__restrict__)buffer)+byteIndex),reg);
	}
}

void native_memset(void * __restrict__ buffer, int byteValue, const size_t size)
{
	void (*funcs[4])(void*__restrict__,int,const size_t) = {&memsetw,&_native_memset_128,&_native_memset_256,&_native_memset_512};
	( memset((void*)(((char*)buffer) + size - (size % (2<<(LVL+2)))),byteValue,size % (2<<(LVL+2))) == buffer )?(void)1:funcs[LVL](buffer,byteValue,size - (size%(2<<(LVL+2))));
	return;
}
