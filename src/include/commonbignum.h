#pragma once

#include <assert.h>

#define BITS_PER_BYTE 8

#ifdef _TEST_BUILD_32_bits
#    ifdef __x86_64__
#        undef __x86_64__
#    endif
#    ifdef _M_X64
#        undef _M_X64
#    endif
#endif /* #ifdef _TEST_BUILD_32_bits */

/* using as many bytes as possible for operations */
#if defined(__x86_64__) || defined(_M_X64)
#    define BYTE_TO_OVERFLOW     4
#    define MASK_TO_INVERT_BYTES 0XFFFFFFFF
#    define MASK_TO_LAST_BYTE    0x00000000000000FF
typedef long long int_result;
typedef unsigned int int_val_to_add;
#else /* #if defined(__x86_64__) || defined(_M_X64) */
#    define BYTE_TO_OVERFLOW     2
#    define MASK_TO_INVERT_BYTES 0XFFFF
#    define MASK_TO_LAST_BYTE    0x000000FF
typedef unsigned int int_result;
typedef unsigned short int_val_to_add;
#endif /* #if defined(__x86_64__) || defined(_M_X64) */

#define MASK_TO_INVERT_ONE_BYTE 0XFF
#define BOOL                    unsigned char
#define TRUE                    1
#define FALSE                   0

typedef struct _bignumlocal
{
    unsigned int numBytes;
    unsigned char bytes[];
}* bignumlocal;
