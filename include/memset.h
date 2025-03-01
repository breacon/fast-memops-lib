/*
 * memset.h
 *
 *  Created on: Feb 24, 2025
 *      Author: USER
 */

#ifndef MEMSET_H_
#define MEMSET_H_

extern void aligned_native_memset(void * __restrict__ buffer, int byteValue, const size_t size);
extern void stream_native_memset(void * __restrict__ buffer, int byteValue, const size_t size);
extern void stream_native_memset_cache(void * __restrict__ buffer, int byteValue, const size_t size);
extern void native_memset(void * __restrict__ buffer, int byteValue, const size_t size);

#endif /* MEMSET_H_ */
