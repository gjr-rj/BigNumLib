#pragma once

#include <assert.h>

#define BYTE_TO_OVERFLOW 1

#define BOOL  unsigned char
#define TRUE  1
#define FALSE 0

typedef struct _bignumlocal
{
    unsigned int numBytes;
    unsigned char bytes[];
}* bignumlocal;
