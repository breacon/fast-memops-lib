/*
 * memcpy.h
 *
 *  Created on: Feb 22, 2025
 *      Author: breacon
 */

#ifndef MEMCPY_H_
#define MEMCPY_H_

#include "nativelib.h"
//#include <string.h>

extern void aligned_native_memcpy(const void * __restrict__ src, void * __restrict__ dest, size_t size);
extern void native_memcpy(const void * __restrict__ src, void * __restrict__ dest, size_t size);
extern void stream_native_memcpy(const void * __restrict__ src, void * __restrict__ dest, size_t size);
extern void stream_native_memcpy_cache(const void * __restrict__ src, void * __restrict__ dest, size_t size);



#endif /* MEMCPY_H_ */
