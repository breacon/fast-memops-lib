/*
 * nativelib.c
 *
 *  Created on: Feb 22, 2025
 *      Author: breacon
 */

#include "..\\include\\nativelib.h"

int _get_cache_line_size(int cache_level) {
    unsigned int eax, ebx, ecx, edx;
    eax = 0x4;  // Cache information leaf
    ecx = cache_level;  // Specify the cache level (0 = L1, 1 = L2, 2 = L3)
    __asm__ __volatile__(
        "cpuid"
        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(eax), "c"(ecx));
    // Check if the cache level exists
    if ((eax & 0x1F) == 0) {
        return 0;
    }
    unsigned int cache_line_size = (ebx & 0xFFF) + 1; // Bits 0-11 contain the line size
    return cache_line_size;
}

// 0 alignment for cache alignment
int address_aligned(void * address, unsigned int alignment)
{
	return (alignment)?(((size_t)address % alignment) == 0):!(((size_t)address) & (_get_cache_line_size(2) - 1));
}

