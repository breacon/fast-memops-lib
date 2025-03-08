/*
 * memcmp.h
 *
 *  Created on: Feb 28, 2025
 *      Author: breacon
 */

#ifndef MEMCMP_H_
#define MEMCMP_H_

#include "nativelib.h"

extern int native_memcmp(const void * buf1, const void * buf2, size_t size);
extern int aligned_native_memcmp(const void * buf1, const void * buf2, size_t size);
extern int stream_native_memcmp(const void * buf1, const void * buf2, size_t size);

#endif /* MEMCMP_H_ */
