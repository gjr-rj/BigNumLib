#ifndef TEST_MATH_PERF_H
#define TEST_MATH_PERF_H

#include <bignum.h>
#include <bignumerr.h>
#include <bignummath.h>

typedef bignumerr_t(testpefoper)(bignum r, bignum a, bignum b);

bignumerr_t baseCompAddInt(bignum r, bignum a, bignum b);
bignumerr_t baseCompMultInt(bignum r, bignum a, bignum b);
void testMathPerformance(unsigned int bigNumSize,
                         char* funcName,
                         testpefoper* funcOp);

#define PERFORMANCE_MATH(x, y) testMathPerformance(x, #y, y)

#endif