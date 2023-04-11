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
bnmthSetFullByte_(unsigned char* bytes, size_t size)
{
    assert(NULL != bytes);

    if (size > 0)
    {
        memset(bytes, 0xFF, size);
    }
}

/*----------------------------------------------------------------------------*/
static void
bnmthResetByte_(unsigned char* bytes, size_t size)
{
    assert(NULL != bytes);

    if (size > 0)
    {
        memset(bytes, 0, size);
    }
}

/*----------------------------------------------------------------------------*/
static void
bnmthReset_(bignum num)
{
    unsigned int numBytes = bnGetSizeInBytes();
    bignumlocal bnLocal = (bignumlocal)(num);

    bnLocal->numBytes = 1;
    bnmthResetByte_(bnLocal->bytes, numBytes + BYTE_TO_OVERFLOW);
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
static typeoperation
compl2Byte_(unsigned char* byte, typeoperation typeAdd)
{
    typeoperation rc = typeAdd;

    assert(NULL != byte);
    assert((bnopcompl2 == typeAdd) || (bnopinvert == typeAdd));

    if (bnopcompl2 == typeAdd)
    {
        if (0 != *byte)
        {
            unsigned char op = 0XFF;
            for (int i = 0; i < BITS_PER_BYTE; i++)
            {
                op <<= 0x01;
                if (((*byte ^ op) & *byte) > 0)
                {
                    *byte ^= op;
                    rc = bnopinvert;
                    break;
                }
            }
        }
    }
    else
    {
        *byte ^= 0xFF;
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static typeoperation
bnmthAddChr_(unsigned short* result,
             const unsigned char* num1,
             const unsigned char* num2,
             typeoperation typeAdd)
{
    unsigned char num2Temp;
    typeoperation rc = typeAdd;

    assert(NULL != result);
    assert(NULL != num1);
    assert(NULL != num2);

    switch (typeAdd)
    {
        case bnopinvert:
        case bnopcompl2:
            num2Temp = *num2;
            rc = compl2Byte_(&num2Temp, typeAdd);

            *result += *num1 + num2Temp;
            break;
        default:
            *result += *num1 + *num2;
            break;
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static void
compl2_(bignum num)
{
    unsigned int newSize;
    bignumlocal bnLocal = (bignumlocal)(num);
    unsigned int numBytes = bnGetSizeInBytes();
    typeoperation typeAdd = bnopcompl2;

    assert(NULL != num);

    for (unsigned int i = 0; i < numBytes; i++)
    {
        if ((i >= bnLocal->numBytes) && (bnopinvert == typeAdd))
        {
            bnmthSetFullByte_((bnLocal->bytes + i), numBytes - i);
            newSize = numBytes;
            break;
        }

        typeAdd = compl2Byte_((bnLocal->bytes + i), typeAdd);
        if (bnLocal->bytes[i] != 0)
        {
            newSize = i + 1;
        }
    }

    bnLocal->numBytes = newSize;
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnmthFastSum_(bignum numResult,
              const bignum num1,
              const bignum num2,
              const typeoperation typeAdd)
{
    bignumerr_t rc = BN_OK;

    bignumlocal bnLocalR = (bignumlocal)(numResult);
    bignumlocal bnLocal1 = (bignumlocal)(num1);
    bignumlocal bnLocal2 = (bignumlocal)(num2);

    assert(NULL != numResult);
    assert(NULL != num1);
    assert(NULL != num2);

    if ((1 == bnLocal1->numBytes) && (0 == (bnLocal1->bytes[0] & 0xFF)) &&
        (1 == bnLocal2->numBytes) && (0 == (bnLocal2->bytes[0] & 0xFF)))
    {
        bnmthReset_(numResult);
    }
    else if ((1 == bnLocal2->numBytes) && (0 == (bnLocal2->bytes[0] & 0xFF)))
    {
        rc = bigNumSet(numResult, num1);
    }
    else
    {
        rc = bigNumSet(numResult, num2);

        if (bnopdefault != typeAdd)
        {
            compl2_(numResult);
            rc = BN_ERR_OVERFLOW;
        }
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static unsigned int
bnmthHigherNumByte_(const bignum num1, const bignum num2)
{
    unsigned int rc;
    bignumlocal bnLocal1 = (bignumlocal)(num1);
    bignumlocal bnLocal2 = (bignumlocal)(num2);

    assert(NULL != num1);
    assert(NULL != num2);

    rc = bnLocal1->numBytes;

    if (bnLocal2->numBytes > rc)
    {
        rc = bnLocal2->numBytes;
    }
    return rc;
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnmthSlowSumComp2_(bignum numResult, const bignum num1, const bignum num2)
{
    bignumerr_t rc = BN_OK;
    typeoperation op = bnopcompl2;
    unsigned int numBytes = bnGetSizeInBytes();
    unsigned int sizeNumInBytes;
    bignumlocal bnLocalR = (bignumlocal)(numResult);
    bignumlocal bnLocal1 = (bignumlocal)(num1);
    bignumlocal bnLocal2 = (bignumlocal)(num2);

    assert(NULL != numResult);
    assert(NULL != num1);
    assert(NULL != num2);

    bnmthReset_(numResult);

    sizeNumInBytes = bnmthHigherNumByte_(num1, num2);
    bnLocalR->numBytes = numBytes;

    for (unsigned int i = 0; i < numBytes; i++)
    {
        unsigned short* result = (unsigned short*)(bnLocalR->bytes + i);

        if (i >= sizeNumInBytes)
        {
            *(bnLocalR->bytes + numBytes) = (unsigned char)*result;
            if (0 == *result)
            {
                bnLocalR->numBytes = numBytes;
                bnmthSetFullByte_((bnLocalR->bytes + i), numBytes - i);
            }
            else
            {
                *(bnLocalR->bytes + i) = 0;
            }

            break;
        }

        op = bnmthAddChr_(
                result, (bnLocal1->bytes + i), (bnLocal2->bytes + i), op);

        if ((*result & 0x00FF) > 0)
        {
            bnLocalR->numBytes = i + 1;
        }
    }

    *(bnLocalR->bytes + numBytes) ^= 0x01;

    if (*(bnLocalR->bytes + numBytes) > 0)
    {
        rc = BN_ERR_OVERFLOW;
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnmthSlowSum_(bignum numResult, const bignum num1, const bignum num2)
{
    bignumerr_t rc = BN_OK;
    unsigned int numBytes = bnGetSizeInBytes();
    unsigned int sizeNumInBytes;
    bignumlocal bnLocalR = (bignumlocal)(numResult);
    bignumlocal bnLocal1 = (bignumlocal)(num1);
    bignumlocal bnLocal2 = (bignumlocal)(num2);

    assert(NULL != numResult);
    assert(NULL != num1);
    assert(NULL != num2);

    bnmthReset_(numResult);

    sizeNumInBytes = bnmthHigherNumByte_(num1, num2);
    bnLocalR->numBytes = numBytes;

    for (unsigned int i = 0; i < numBytes; i++)
    {
        unsigned short* result = (unsigned short*)(bnLocalR->bytes + i);

        if (i >= sizeNumInBytes)
        {
            if (*result <= 0)
            {
                bnLocalR->numBytes = i;
                break;
            }
        }

        (void)bnmthAddChr_(result,
                           (bnLocal1->bytes + i),
                           (bnLocal2->bytes + i),
                           bnopdefault);
    }

    if (*(bnLocalR->bytes + numBytes) > 0)
    {
        rc = BN_ERR_OVERFLOW;
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnmthSum_(bignum numResult,
          const bignum num1,
          const bignum num2,
          const typeoperation typeAdd)
{
    bignumerr_t rc = BN_OK;

    bignumlocal bnLocalR = (bignumlocal)(numResult);
    bignumlocal bnLocal1 = (bignumlocal)(num1);
    bignumlocal bnLocal2 = (bignumlocal)(num2);

    assert(NULL != numResult);
    assert(NULL != num1);
    assert(NULL != num2);

    if (((1 == bnLocal1->numBytes) && (0 == (bnLocal1->bytes[0] & 0xFF))) ||
        ((1 == bnLocal2->numBytes) && (0 == (bnLocal2->bytes[0] & 0xFF))))
    {
        rc = bnmthFastSum_(numResult, num1, num2, typeAdd);
    }
    else
    {
        if (bnopdefault == typeAdd)
        {
            rc = bnmthSlowSum_(numResult, num1, num2);
        }
        else
        {
            rc = bnmthSlowSumComp2_(numResult, num1, num2);
        }
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

    BNVERIFYLEN(numResult);

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

    BNVERIFYLEN(numResult);

    bnSetLastError(rc);
    return rc;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumMul(bignum numResult, const bignum num1, const bignum num2)
{
    bignumerr_t rc = BN_OK;

    BNVERIFYLEN(numResult);

    bnSetLastError(rc);
    return rc;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumDiv(bignum numResult, const bignum num1, const bignum num2)
{
    bignumerr_t rc = BN_OK;

    BNVERIFYLEN(numResult);
    bnSetLastError(rc);
    return rc;
}