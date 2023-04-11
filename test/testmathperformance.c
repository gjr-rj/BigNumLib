#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <testmathperformance.h>

#define UNUSED_PARAMETER(x) ((void)x)

static unsigned int an_ = 0xFF01FEBB;
static unsigned int bn_ = 0xCCCCCCAA;

/*----------------------------------------------------------------------------*/
bignumerr_t
baseCompAddInt(bignum r, bignum a, bignum b)
{
    bignumerr_t rc = BN_OK;
    UNUSED_PARAMETER(r);
    UNUSED_PARAMETER(a);
    UNUSED_PARAMETER(b);

    unsigned int rn = an_ + bn_;

    return rc;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
baseCompMultInt(bignum r, bignum a, bignum b)
{
    bignumerr_t rc = BN_OK;
    UNUSED_PARAMETER(r);
    UNUSED_PARAMETER(a);
    UNUSED_PARAMETER(b);

    unsigned int rn = an_ * bn_;

    return rc;
}

/*----------------------------------------------------------------------------*/
void
testMathPerformance(unsigned int bigNumSize,
                    char* funcName,
                    testpefoper* funcOp)
{
    clock_t start;
    clock_t stop;
    double diff;
    float tExec = 0;
    unsigned int rc = 0;
    bignumerr_t err = bigNumInit(bigNumSize);

    if (BN_OK == err)
    {
        bignum n1 = bigNumNew();
        bignum n2 = bigNumNew();
        bignum n3 = bigNumNew();

        err = bigNumRand(n1);
        if (BN_OK == err)
        {
            err = bigNumRand(n2);
        }

        if (BN_OK == err)
        {
            start = clock();
            while (tExec < 1)
            {
                rc++;
                err = funcOp(n3, n1, n2);
                stop = clock();
                diff = difftime(stop, start);
                tExec = (float)diff / CLOCKS_PER_SEC;
            }
        }

        bigNumFinish();

        printf("%s (%u): %u per sec\n", funcName, bigNumSize, rc);
    }
}