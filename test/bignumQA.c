#include <assert.h>
#include <stdio.h>

#include <bignum.h>
#include <bignummath.h>

#include <testbase.h>
#include <testmath.h>
#include <testmathperformance.h>

/** @example bignumQA.c
 * Unit and functional tests of the bignum library. The meso serves as an
 * example code of its use
 */
void
main(void)
{
    // testBase();
    // testMath();

    printf("Base, time to add interger\n");
    PERFORMANCE_MATH(32, baseCompAddInt);
    printf("-------------------------------\n");
    PERFORMANCE_MATH(32, bigNumAdd);
    PERFORMANCE_MATH(3200, bigNumAdd);
    PERFORMANCE_MATH(3200000, bigNumAdd);
    printf("-------------------------------\n");
    PERFORMANCE_MATH(32, bigNumSub);
    PERFORMANCE_MATH(3200, bigNumSub);
    PERFORMANCE_MATH(3200000, bigNumSub);
    printf("-------------------------------\n");
    printf("-------------------------------\n");

    printf("Base, time to Mult interger\n");
    PERFORMANCE_MATH(32, baseCompMultInt);

    printf("##############################\n");
    printf("#            Test OK         #\n");
    printf("##############################\n");
}