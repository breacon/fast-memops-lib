/*
 * memcmp.c
 *
 *  Created on: Feb 24, 2025
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


static int _aligned_native_memcmp_512(const void * buf1, const void * buf2, const size_t size)
{
	register unsigned short curRes = 0;
	register int fullRes = 0;
	_mm_prefetch(buf1,_MM_HINT_NTA);
	_mm_prefetch(buf2,_MM_HINT_NTA);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex += 64)
	{
		_mm_prefetch((void*)(((char*)buf1)+byteIndex+64),_MM_HINT_NTA);
		_mm_prefetch((void*)(((char*)buf2)+byteIndex+64),_MM_HINT_NTA);
		__m512 reg1 = _mm512_load_ps((void*)(((char*)buf1)+byteIndex));
		__m512 reg2 = _mm512_load_ps((void*)(((char*)buf2)+byteIndex));
		curRes += ~((unsigned short)(_mm512_cmp_ps_mask(reg1,reg2,_MM_CMPINT_EQ)));
		fullRes+= curRes;
	}
	return fullRes;
}
static int _aligned_native_memcmp_256(const void * buf1, const void * buf2, const size_t size)
{
	register unsigned char curRes = 0;
	register int fullRes = 0;
	_mm_prefetch(buf1,_MM_HINT_NTA);
	_mm_prefetch(buf2,_MM_HINT_NTA);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex += 32)
	{
//		_mm_prefetch((void*)(((char*)buf1)+byteIndex+64),_MM_HINT_NTA);
//		_mm_prefetch((void*)(((char*)buf2)+byteIndex+64),_MM_HINT_NTA);
		__m256 reg1 = _mm256_load_ps((void*)(((char*)buf1)+byteIndex));
		__m256 reg2 = _mm256_load_ps((void*)(((char*)buf2)+byteIndex));
		curRes += ~((unsigned short)(_mm256_cmp_ps_mask(reg1,reg2,_MM_CMPINT_EQ)));
		fullRes += curRes;
	}
	return fullRes;
}
static int _aligned_native_memcmp_128(const void * buf1, const void * buf2, const size_t size)
{
	register unsigned char curRes = 0;
	register int fullRes = 0;
	_mm_prefetch(buf1,_MM_HINT_NTA);
	_mm_prefetch(buf2,_MM_HINT_NTA);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex += 16)
	{
#ifdef NT_MEMOPS_C23
		if([[unlikely]] byteIndex % 64 == 0)
		{
			_mm_prefetch((void*)(((char*)src)+byteIndex+64),_MM_HINT_T1);
			_mm_prefetch((void*)(((char*)dest)+byteIndex+64),_MM_HINT_T1);
		}
#endif
//		_mm_prefetch((void*)(((char*)buf1)+byteIndex+64),_MM_HINT_NTA);
//		_mm_prefetch((void*)(((char*)buf2)+byteIndex+64),_MM_HINT_NTA);
		__m128 reg1 = _mm_load_ps((void*)(((char*)buf1)+byteIndex));
		__m128 reg2 = _mm_load_ps((void*)(((char*)buf2)+byteIndex));
		curRes += ~((unsigned short)(_mm_cmp_ps_mask(reg1,reg2,_MM_CMPINT_EQ)));
		fullRes += curRes;
	}
	return fullRes;
}



int aligned_native_memcmp(const void * buf1, const void * buf2, const size_t size)
{
	int (*funcs[4])(const void*, const void*, size_t) = {&memcmp,&_aligned_native_memcmp_128,&_aligned_native_memcmp_256,&_aligned_native_memcmp_512};
	return memcmp((void*)(((char*)buf1)+size - (size%(2<<(LVL+2)))),(void*)(((char*)buf2)+size - (size%(2<<(LVL+2)))),size - (size%(2<<(LVL+2))))+(size < (2<<(LVL+2)))?0:funcs[LVL](buf1,buf2,size - (size%(2<<(LVL+2))));
}





static int _stream_native_memcmp_512(const void * buf1, const void * buf2, const size_t size)
{
	register unsigned short curRes = 0;
	register int fullRes = 0;
//	_mm_prefetch(buf1,_MM_HINT_NTA);
//	_mm_prefetch(buf2,_MM_HINT_NTA);
	__m512i reg1;
	__m512i reg2;
	for(size_t byteIndex = 0; byteIndex < size; byteIndex += 64)
	{
//		_mm_prefetch((void*)(((char*)buf1)+byteIndex+64),_MM_HINT_NTA);
//		_mm_prefetch((void*)(((char*)buf2)+byteIndex+64),_MM_HINT_NTA);
		reg1 = _mm512_stream_load_si512((void*)(((char*)buf1)+byteIndex));
		reg2 = _mm512_stream_load_si512((void*)(((char*)buf2)+byteIndex));
		curRes += ~((unsigned short)(_mm512_cmp_ps_mask((__m512)reg1,(__m512)reg2,_MM_CMPINT_EQ)));
		fullRes+= curRes;
	}
	return fullRes;
}
static int _stream_native_memcmp_256(const void * buf1, const void * buf2, const size_t size)
{
	register unsigned char curRes = 0;
	register int fullRes = 0;
//	_mm_prefetch(buf1,_MM_HINT_NTA);
//	_mm_prefetch(buf2,_MM_HINT_NTA);
	__m256i reg1;
	__m256i reg2;
	for(size_t byteIndex = 0; byteIndex < size; byteIndex += 32)
	{
//		_mm_prefetch((void*)(((char*)buf1)+byteIndex+64),_MM_HINT_NTA);
//		_mm_prefetch((void*)(((char*)buf2)+byteIndex+64),_MM_HINT_NTA);
		reg1 = _mm256_stream_load_si256((void*)(((char*)buf1)+byteIndex));
		reg2 = _mm256_stream_load_si256((void*)(((char*)buf2)+byteIndex));
		curRes += ~((unsigned short)(_mm256_cmp_ps_mask((__m256)reg1,(__m256)reg2,_MM_CMPINT_EQ)));
		fullRes += curRes;
	}
	return fullRes;
}
static int _stream_native_memcmp_128(const void * buf1, const void * buf2, const size_t size)
{
	register unsigned char curRes = 0;
	register int fullRes = 0;
//	_mm_prefetch(buf1,_MM_HINT_NTA);
//	_mm_prefetch(buf2,_MM_HINT_NTA);
	__m128i reg1;
	__m128i reg2;
	for(size_t byteIndex = 0; byteIndex < size; byteIndex += 16)
	{
//		_mm_prefetch((void*)(((char*)buf1)+byteIndex+64),_MM_HINT_NTA);
//		_mm_prefetch((void*)(((char*)buf2)+byteIndex+64),_MM_HINT_NTA);
		reg1 = _mm_stream_load_si128((void*)(((char*)buf1)+byteIndex));
		reg2 = _mm_stream_load_si128((void*)(((char*)buf2)+byteIndex));
		curRes += ~((unsigned short)(_mm_cmp_ps_mask((__m128)reg1,(__m128)reg2,_MM_CMPINT_EQ)));
		fullRes += curRes;
	}
	return fullRes;
}


int stream_native_memcmp(const void * buf1, const void * buf2, const size_t size)
{
	#ifdef __AVX2__
		int (*funcs[4])(const void*, const void*, size_t) = {&memcmp,&_stream_native_memcmp_128,&_stream_native_memcmp_256,&_stream_native_memcmp_512};
	#else
		int (*funcs[4])(const void*, const void*, size_t) = {&memcmp,&_stream_native_memcmp_128,&_stream_native_memcmp_128,&_stream_native_memcmp_512};
	#endif
	return memcmp((void*)(((char*)buf1)+size - (size%(2<<(LVL+2)))),(void*)(((char*)buf2)+size - (size%(2<<(LVL+2)))),size - (size%(2<<(LVL+2))))+(size < (2<<(LVL+2)))?0:funcs[LVL](buf1,buf2,size - (size%(2<<(LVL+2))));
}




static int _native_memcmp_512(const void * buf1, const void * buf2, const size_t size)
{
	register unsigned short curRes = 0;
	register int fullRes = 0;
	_mm_prefetch(buf1,_MM_HINT_NTA);
	_mm_prefetch(buf2,_MM_HINT_NTA);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex += 64)
	{
		_mm_prefetch((void*)(((char*)buf1)+byteIndex+64),_MM_HINT_NTA);
		_mm_prefetch((void*)(((char*)buf2)+byteIndex+64),_MM_HINT_NTA);
		__m512 reg1 = _mm512_loadu_ps((void*)(((char*)buf1)+byteIndex));
		__m512 reg2 = _mm512_loadu_ps((void*)(((char*)buf2)+byteIndex));
		curRes += ~((unsigned short)(_mm512_cmp_ps_mask(reg1,reg2,_MM_CMPINT_EQ)));
		fullRes+= curRes;
	}
	return fullRes;
}
static int _native_memcmp_256(const void * buf1, const void * buf2, const size_t size)
{
	register unsigned char curRes = 0;
	register int fullRes = 0;
	_mm_prefetch(buf1,_MM_HINT_NTA);
	_mm_prefetch(buf2,_MM_HINT_NTA);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex += 32)
	{
//		_mm_prefetch((void*)(((char*)buf1)+byteIndex+64),_MM_HINT_NTA);
//		_mm_prefetch((void*)(((char*)buf2)+byteIndex+64),_MM_HINT_NTA);
		__m256 reg1 = _mm256_loadu_ps((void*)(((char*)buf1)+byteIndex));
		__m256 reg2 = _mm256_loadu_ps((void*)(((char*)buf2)+byteIndex));
		curRes += ~((unsigned short)(_mm256_cmp_ps_mask(reg1,reg2,_MM_CMPINT_EQ)));
		fullRes += curRes;
	}
	return fullRes;
}
static int _native_memcmp_128(const void * buf1, const void * buf2, const size_t size)
{
	register unsigned char curRes = 0;
	register int fullRes = 0;
	_mm_prefetch(buf1,_MM_HINT_NTA);
	_mm_prefetch(buf2,_MM_HINT_NTA);
	for(size_t byteIndex = 0; byteIndex < size; byteIndex += 16)
	{
//		_mm_prefetch((void*)(((char*)buf1)+byteIndex+64),_MM_HINT_NTA);
//		_mm_prefetch((void*)(((char*)buf2)+byteIndex+64),_MM_HINT_NTA);
		__m128 reg1 = _mm_loadu_ps((void*)(((char*)buf1)+byteIndex));
		__m128 reg2 = _mm_loadu_ps((void*)(((char*)buf2)+byteIndex));
		curRes += ~((unsigned short)(_mm_cmp_ps_mask(reg1,reg2,_MM_CMPINT_EQ)));
		fullRes += curRes;
	}
	return fullRes;
}

int native_native_memcmp(const void * buf1, const void * buf2, const size_t size)
{
	int (*funcs[4])(const void*, const void*, size_t) = {&memcmp,&_native_memcmp_128,&_native_memcmp_256,&_native_memcmp_512};
	return memcmp((void*)(((char*)buf1)+size - (size%(2<<(LVL+2)))),(void*)(((char*)buf2)+size - (size%(2<<(LVL+2)))),size - (size%(2<<(LVL+2))))+(size < (2<<(LVL+2)))?0:funcs[LVL](buf1,buf2,size - (size%(2<<(LVL+2))));
}
