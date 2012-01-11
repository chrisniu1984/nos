#ifndef _TYPES_H_
#define _TYPES_H_

#include <config.h>

typedef char                __s8;
typedef unsigned char       __u8;
typedef short               __s16;
typedef unsigned short      __u16;
typedef int                 __s32;
typedef unsigned int        __u32;

#ifdef _BITS_32_
typedef long long           __s64;
typedef unsigned long long  __u64;
#elif  _BITS_64_
typedef long                __s64;
typedef unsigned long       __u64;
#endif

#define NULL            0

#endif
