#pragma once

#include <bignumerr.h>
#include <bignum.h>

unsigned int bnGetSizeInBytes(void);
void bnSetLastError(bignumerr_t err);
bignumerr_t bnValidateInitialize(bignum num);

#ifndef NDEBUG
void bnVerifyLen(bignum num,
                 const char* function,
                 const char* file,
                 const unsigned int line);
#    define BNVERIFYLEN(x) \
        bnVerifyLen(x, __FUNCTION__, __FILE__, (unsigned int)__LINE__)
#else /* #ifndef NDEBUG */
#    define BNVERIFYLEN(x) ((void)0)
#endif /* #ifndef NDEBUG */