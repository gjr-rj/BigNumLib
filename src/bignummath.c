#include <string.h>
#include <stdio.h>

#include <commonbignum.h>
#include <internalbignum.h>

typedef enum
{
    bnopdefault,
    bnopcompl2,
    bnopinvert
} typeoperation;

#define NO_OPERATIONS_IN_ASSEMBLY
#ifdef _OPERATIONS_IN_ASSEMBLY
#ifdef _WIN64
int bnAddArrayOfBytes_(unsigned char* s,
                       const unsigned char* a,
                       const unsigned char* b,
                       const unsigned int maxSize,
                       const unsigned int sizeBiggerNum);

#define _OP_AS_WIN64
#endif /* #ifdef _WIN64 */
#endif /* #ifdef OPERATIN_IN_ASSEMBLY */

static const int nBytesToInvert_ = BITS_PER_BYTE * BYTE_TO_OVERFLOW;

static void adjustNumBytesResult_(bignumlocal bnLocalR,
                                  unsigned int index,
                                  unsigned int count);

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
compl2Byte_(int_val_to_add* byte, typeoperation typeAdd)
{
    typeoperation rc = typeAdd;

    assert(NULL != byte);
    assert((bnopcompl2 == typeAdd) || (bnopinvert == typeAdd));

    if (bnopcompl2 == typeAdd)
    {
        if (0 != *byte)
        {
            int_val_to_add op = MASK_TO_INVERT_BYTES;
            for (int i = 0; i < nBytesToInvert_; i++)
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
        *byte ^= MASK_TO_INVERT_BYTES;
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static typeoperation
bnmthAddChr_(int_result* result,
             const int_val_to_add* num1,
             const int_val_to_add* num2,
             typeoperation typeAdd)
{
    int_val_to_add num2Temp;
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

            *result += (int_result)*num1 + (int_result)num2Temp;
            break;
        default:
            *result += (int_result)*num1 + (int_result)*num2;
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

    for (unsigned int i = 0; i < numBytes; i += BYTE_TO_OVERFLOW)
    {
        if ((i >= bnLocal->numBytes) && (bnopinvert == typeAdd))
        {
            bnmthSetFullByte_((bnLocal->bytes + i), numBytes - i);
            newSize = numBytes;
            break;
        }

        typeAdd = compl2Byte_((int_val_to_add*)(bnLocal->bytes + i), typeAdd);
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
bnmthHighterNumByte_(const bignum num1, const bignum num2)
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

    sizeNumInBytes = bnmthHighterNumByte_(num1, num2);
    bnLocalR->numBytes = numBytes;

    for (unsigned int i = 0; i < numBytes; i += BYTE_TO_OVERFLOW)
    {
        int_result* result = (int_result*)(bnLocalR->bytes + i);

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
                bnLocalR->numBytes = i;
            }

            break;
        }

        op = bnmthAddChr_(result,
                          (int_val_to_add*)(bnLocal1->bytes + i),
                          (int_val_to_add*)(bnLocal2->bytes + i),
                          op);
    }

    adjustNumBytesResult_(bnLocalR, bnLocalR->numBytes, bnLocalR->numBytes);

    *(bnLocalR->bytes + numBytes) ^= 0x01;

    if (*(bnLocalR->bytes + numBytes) > 0)
    {
        rc = BN_ERR_OVERFLOW;
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static void
adjustNumBytesResult_(bignumlocal bnLocalR,
                      unsigned int index,
                      unsigned int count)
{
    for (unsigned int i = 0; i < count; i++)
    {
        if ((char)*(bnLocalR->bytes + index - i - 1) != '\0')
        {
            bnLocalR->numBytes = index - i;
            break;
        }
    }
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

    sizeNumInBytes = bnmthHighterNumByte_(num1, num2);
    bnLocalR->numBytes = numBytes;

#ifdef _OP_AS_WIN64
    {
        /* do not more fast */
        int hasOverflow = bnAddArrayOfBytes_(bnLocalR->bytes,
                                             bnLocal1->bytes,
                                             bnLocal2->bytes,
                                             numBytes,
                                             sizeNumInBytes);
        if (hasOverflow > 0)
        {
            rc = BN_ERR_OVERFLOW;
        }
    }
#else /* #ifdef _OP_AS_WIN64 */

    for (unsigned int i = 0; i < numBytes; i += BYTE_TO_OVERFLOW)
    {
        int_result* result = (int_result*)(bnLocalR->bytes + i);

        if (i >= sizeNumInBytes)
        {
            if (*result <= 0)
            {
                adjustNumBytesResult_(bnLocalR, i, BYTE_TO_OVERFLOW);
                break;
            }
        }

        (void)bnmthAddChr_(result,
                           (int_val_to_add*)(bnLocal1->bytes + i),
                           (int_val_to_add*)(bnLocal2->bytes + i),
                           bnopdefault);
    }

    if (*(bnLocalR->bytes + numBytes) > 0)
    {
        rc = BN_ERR_OVERFLOW;
    }

#endif /* #ifdef _WIN64 */

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