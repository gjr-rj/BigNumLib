#pragma once

#include <bignumerr.h>
#include <bignum.h>

unsigned int bnGetSizeInBytes(void);
void bnSetLastError(bignumerr_t err);
bignumerr_t bnValidateInitialize(bignum num);