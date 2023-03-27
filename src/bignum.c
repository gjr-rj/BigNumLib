#include <stdio.h>
#include <bignum.h>

#define BITS_PER_BYTE 8

static unsigned int numBits_ = 0;

/*----------------------------------------------------------------------------*/
int
bigNumInit(unsigned int numBits)
{
    int rc = BN_ERR;

    if (0 == numBits_)
    {
        if ((numBits % BITS_PER_BYTE) == 0)
        {
            numBits_ = numBits;
            rc = BN_OK;
        }
        else
        {
            rc = BN_ERR_NOT_SIZE_BYTE;
        }
    }
    else
    {
        rc = BN_ERR_ALREADY_INIT;
    }

    return rc;
}