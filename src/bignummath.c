#include <string.h>

#include <commonbignum.h>
#include <internalbignum.h>

typedef enum
{
    bnopdefault,
    bnopcompl2,
    bnopinvert
} typeoperation;

/*----------------------------------------------------------------------------*/
static void
bnmthReset_(bignum num)
{
    unsigned int numBytes = bnGetSizeInBytes();
    bignumlocal bnLocal = (bignumlocal)(num);

    bnLocal->numBytes = 1;
    memset(bnLocal->bytes, 0, numBytes + BYTE_TO_OVERFLOW);
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnmthValidateOpParam_(bignum num1, bignum num2, bignum numResult)
{
    bignumerr_t rc = bnValidateInitialize(num1);

    if (BN_OK == rc)
    {
        rc = bnValidateInitialize(num2);
    }

    if (BN_OK == rc)
    {
        rc = bnValidateInitialize(numResult);
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static void
bnmthAddChr_(unsigned short* result,
             const unsigned char* num1,
             const unsigned char* num2,
             typeoperation typeAdd)
{
    assert(NULL != result);
    assert(NULL != num1);
    assert(NULL != num2);

    switch (typeAdd)
    {
        case bnopinvert:
            *result += *num1 + (*num2 ^ 0xFF);
            break;
        case bnopcompl2:
            *result += *num1 + (*num2 ^ 0xFE);
            break;
        default:
            *result += *num1 + *num2;
            break;
    }
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnmthSum_(bignum numResult,
          const bignum num1,
          const bignum num2,
          const typeoperation typeAdd)
{
    bignumerr_t rc = BN_OK;
    typeoperation tpOp = typeAdd;
    unsigned int numBytes = bnGetSizeInBytes();
    unsigned int sizeNumInBytes;
    bignumlocal bnLocalR = (bignumlocal)(numResult);
    bignumlocal bnLocal1 = (bignumlocal)(num1);
    bignumlocal bnLocal2 = (bignumlocal)(num2);

    assert(NULL != numResult);
    assert(NULL != num1);
    assert(NULL != num2);

    bnmthReset_(numResult);

    sizeNumInBytes = bnLocal1->numBytes;
    if (bnLocal2->numBytes > sizeNumInBytes)
    {
        sizeNumInBytes = bnLocal2->numBytes;
    }

    bnLocalR->numBytes = 0;
    for (unsigned int i = 0; i < numBytes; i++)
    {
        unsigned short* result = (unsigned short*)(bnLocalR->bytes + i);

        if (i >= sizeNumInBytes)
        {
            bnLocalR->numBytes = i;
            if (*result <= 0)
            {
                break;
            }
        }

        bnmthAddChr_(
                result, (bnLocal1->bytes + i), (bnLocal2->bytes + i), tpOp);

        if (0 == i)
        {
            if (bnopdefault != tpOp)
            {
                tpOp = bnopinvert;
            }
        }
    }

    if (0 == bnLocalR->numBytes)
    {
        bnLocalR->numBytes = numBytes;
    }

    if (*(bnLocalR->bytes + numBytes) > 0)
    {
        rc = BN_ERR_OVERFLOW;
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumAdd(bignum numResult, const bignum num1, const bignum num2)
{
    bignumerr_t rc = bnmthValidateOpParam_(numResult, num1, num2);

    if (BN_OK == rc)
    {
        rc = bnmthSum_(numResult, num1, num2, bnopdefault);
    }

    bnSetLastError(rc);
    return rc;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumSub(bignum numResult, const bignum num1, const bignum num2)
{
    bignumerr_t rc = bnmthValidateOpParam_(numResult, num1, num2);

    if (BN_OK == rc)
    {
        rc = bnmthSum_(numResult, num1, num2, bnopcompl2);
    }

    bnSetLastError(rc);
    return rc;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumMul(bignum numResult, const bignum num1, const bignum num2)
{
    bignumerr_t rc = BN_OK;

    bnSetLastError(rc);
    return rc;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumDiv(bignum numResult, const bignum num1, const bignum num2)
{
    bignumerr_t rc = BN_OK;

    bnSetLastError(rc);
    return rc;
}