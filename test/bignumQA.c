#include <assert.h>
#include <stdio.h>

#include <bignum.h>
#include <bignummath.h>

#include <testbase.h>
#include <testmath.h>

/** @example bignumQA.c
 * Unit and functional tests of the bignum library. The meso serves as an
 * example code of its use
 */
void
main(void)
{
    testBase();
    testMath();

    printf("##############################\n");
    printf("#            Test OK         #\n");
    printf("##############################\n");
}