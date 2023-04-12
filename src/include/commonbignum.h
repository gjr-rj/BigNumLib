#pragma once

#include <assert.h>

#define BITS_PER_BYTE    8
#define BYTE_TO_OVERFLOW 2

#define BOOL  unsigned char
#define TRUE  1
#define FALSE 0

typedef struct _bignumlocal
{
    unsigned int numBytes;
    unsigned char bytes[];
}* bignumlocal;
