**Fast-Memops-Lib**

a small library for windows (x64) that provides some basic fast, optimized memory operations like memcpy, memcmp and memset

**NOTICE**
   - some "stream" operations require AVX2 (if you only have AVX)
   - in my limited testing, this did come out to be marginaly faster than MingW-w64's libc implementation, I believe the benefits tend to grow with larger buffers.
  - stream operations require aligned buffers
  - stream_..._cache operations only cache the beginning of the out/destination buffer at the end of the operaion since the 'stream'        operations skip the cache.

**NOTES**

  -  yes, I know, it's bad.
  -  yes, I know, there are a lot of garbage comments and inconsistencies.
  -  yes, I know, probably could and should have done a lot more prefetching, I am currently experimenting with it.
  -  feel free, to educate me/ bring up something I have not addressed/ suggest an improvment.

