#ifndef BIGNUN_MATH_H_
#define BIGNUN_MATH_H_

#include <bignumerr.h>
#include <bignum.h>

bignumerr_t bigNumAdd(bignum numResult, const bignum num1, const bignum num2);
bignumerr_t bigNumSub(bignum numResult, const bignum num1, const bignum num2);
bignumerr_t bigNumMul(bignum numResult, const bignum num1, const bignum num2);
bignumerr_t bigNumDiv(bignum numResult, const bignum num1, const bignum num2);

#endif
