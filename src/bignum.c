#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include <bignum.h>

#define BOOL  unsigned char
#define TRUE  1
#define FALSE 0

#define BITS_PER_BYTE 8

#define NUM_CHAR_TO_REPRESENT_HEX 2
#define NUM_CHAR_TO_REPRESENT_BIN 8

#define LOCAL_ERROR                (-99)
#define PARAM1_GRATER_THEN_PARAM2  1
#define PARAM1_SMALLER_THEN_PARAM2 (-1)
#define PARAM1_EQUAL_PARAM2        0

typedef bignumerr_t (*bnFuncAddCharToByte)(unsigned char*, char);
typedef bignumerr_t (*bnFuncActionBigNumToByte)(bignum,
                                                unsigned char*,
                                                unsigned int);

typedef struct _bignumlocal
{
    unsigned int numBytes;
    unsigned char bytes[];
}* bignumlocal;
typedef struct _bignumlist
{
    bignum val;
    struct _bignumlist* next;

} bignumlist, *pbignumlist;

typedef struct
{
    int numCharPerByte;
    bnFuncAddCharToByte callbackToConvert;
    bnFuncActionBigNumToByte callbackToAction;
} bignumopinfo;

static bignumlist bnTop = {NULL, NULL};
static bignumerr_t bnLastError_ = BN_OK;
static bignumopinfo bnInfo_ = {0, NULL, NULL};

static unsigned int numBits_ = 0;

#define IS_DIGIT(n)     ((n >= '0') && (n <= '9'))
#define DIGIT_TO_HEX(n) (n - '0')

#define IS_AF(n)     ((n >= 'A') && (n <= 'F'))
#define AF_TO_HEX(n) (n - 'A' + 10)

#define IS_af(n)     ((n >= 'a') && (n <= 'f'))
#define af_TO_HEX(n) (n - 'a' + 10)

static bignumerr_t bnValidateInitialize_(bignum num);
static unsigned int bnStrLenOf_(char* charVal);
static unsigned int bnGetSizeInBytes_(void);
static unsigned int bnStrLenOf_(char* charVal);
static unsigned int bnLenInBytesOf_(unsigned int chrLen, int nCharPerByte);
static unsigned int bnGetSizeInBytes_(void);

/*----------------------------------------------------------------------------*/
static void
bnShiftLeftNBytesEx_(bignum num, unsigned int nShiftByte)
{
    bignumlocal bnLocal = (bignumlocal)(num);
    unsigned int numBytes = bnGetSizeInBytes_();
    unsigned int numBytesTemp = bnLocal->numBytes + nShiftByte;
    unsigned int ori = bnLocal->numBytes;
    unsigned int dest;
    unsigned int size;

    assert(NULL != num);
    assert(bnLocal->numBytes > 0);
    assert(nShiftByte > 0);
    assert(nShiftByte < numBytes);

    if (numBytesTemp > numBytes)
    {
        numBytesTemp = numBytes;
    }

    while (ori > 0)
    {
        if (ori < nShiftByte)
        {
            ori = 0;
        }
        else
        {
            ori -= nShiftByte;
        }

        dest = ori + nShiftByte;

        size = nShiftByte;

        if (dest + size > numBytes)
        {
            size = numBytes - dest;
        }

        if (dest < numBytes)
        {
            memcpy((bnLocal->bytes + dest), (bnLocal->bytes + ori), size);
            if (0 == ori)
            {
                memset((bnLocal->bytes + ori), 0, nShiftByte);
            }
        }
    }

    bnLocal->numBytes = 1;
    for (unsigned int i = numBytesTemp; i > 0; i--)
    {
        if (bnLocal->bytes[i - 1] > 0)
        {
            bnLocal->numBytes = i;
            break;
        }
    }
}

/*----------------------------------------------------------------------------*/
static void
bnShiftLeftNBytes_(bignum num, unsigned int nShiftByte)
{
    bignumlocal bnLocal = (bignumlocal)(num);

    if ((nShiftByte > 0) &&
        ((bnLocal->numBytes > 1) || (bnLocal->bytes[0] != 0)))
    {
        unsigned int numBytes = bnGetSizeInBytes_();
        if (nShiftByte < numBytes)
        {
            bnShiftLeftNBytesEx_(num, nShiftByte);
        }
        else
        {
            bnLocal->numBytes = 1;
            memset(bnLocal->bytes, 0, numBytes);
        }
    }
}

/*----------------------------------------------------------------------------*/
static void
bnShiftLeftNBits_(bignum num, int nShiftBits)
{
    if (nShiftBits > 0)
    {
        unsigned int i;
        unsigned char carry = 0;
        unsigned int numBytesTemp = 0;
        unsigned int numBytes = bnGetSizeInBytes_();
        bignumlocal bnLocal = (bignumlocal)(num);

        assert(NULL != num);
        assert(bnLocal->numBytes > 0);
        assert(nShiftBits < BITS_PER_BYTE);

        for (i = 0; i < numBytes; i++)
        {
            unsigned char temp =
                    bnLocal->bytes[i] >> (BITS_PER_BYTE - nShiftBits);
            bnLocal->bytes[i] = (bnLocal->bytes[i] << nShiftBits) | carry;
            carry = temp;

            if (bnLocal->bytes[i] > 0)
            {
                numBytesTemp = i + 1;
            }

            if ((i >= bnLocal->numBytes) && (carry == 0))
            {
                if (0 == numBytesTemp)
                {
                    numBytesTemp = 1;
                }

                bnLocal->numBytes = numBytesTemp;
                break;
            }
        }
    }
}

/*----------------------------------------------------------------------------*/
static void
bnShiftRightNBytesEx_(bignum num, unsigned int nShiftByte)
{
    bignumlocal bnLocal = (bignumlocal)(num);
    unsigned int numBytes = bnGetSizeInBytes_();
    unsigned int numBytesTemp = bnLocal->numBytes;
    unsigned int ori = nShiftByte;
    unsigned int dest = 0;
    unsigned int size = nShiftByte;

    assert(NULL != num);
    assert(bnLocal->numBytes > 0);
    assert(nShiftByte > 0);
    assert(nShiftByte < bnLocal->numBytes);

    if (numBytesTemp > nShiftByte)
    {
        numBytesTemp = numBytesTemp - nShiftByte;
    }

    while (ori < bnLocal->numBytes)
    {
        if (ori + size > bnLocal->numBytes)
        {
            size = bnLocal->numBytes - ori;
        }

        if (size > 0)
        {
            if (size < nShiftByte)
            {
                memset((bnLocal->bytes + dest), 0, nShiftByte);
            }

            memcpy((bnLocal->bytes + dest), (bnLocal->bytes + ori), size);

            if (ori + size == bnLocal->numBytes)
            {
                memset((bnLocal->bytes + ori), 0, size);
            }
        }

        dest += nShiftByte;
        ori += nShiftByte;
    }

    bnLocal->numBytes = 1;
    for (unsigned int i = numBytesTemp; i > 0; i--)
    {
        if (bnLocal->bytes[i - 1] > 0)
        {
            bnLocal->numBytes = i;
            break;
        }
    }
}

/*----------------------------------------------------------------------------*/
static void
bnShifRightNBytes_(bignum num, unsigned int nShiftByte)
{
    bignumlocal bnLocal = (bignumlocal)(num);

    if ((nShiftByte > 0) &&
        ((bnLocal->numBytes > 1) || (bnLocal->bytes[0] != 0)))
    {
        unsigned int numBytes = bnGetSizeInBytes_();
        if (nShiftByte < bnLocal->numBytes)
        {
            bnShiftRightNBytesEx_(num, nShiftByte);
        }
        else
        {
            bnLocal->numBytes = 1;
            memset(bnLocal->bytes, 0, bnLocal->numBytes);
        }
    }
}

/*----------------------------------------------------------------------------*/
static void
bnShiftRightNBits_(bignum num, int nShiftBits)
{
    if (nShiftBits > 0)
    {
        unsigned int i;
        unsigned char carry = 0;
        unsigned int numBytesTemp = 0;
        unsigned int numBytes = bnGetSizeInBytes_();
        bignumlocal bnLocal = (bignumlocal)(num);

        assert(NULL != num);
        assert(bnLocal->numBytes > 0);
        assert(nShiftBits < BITS_PER_BYTE);

        for (i = bnLocal->numBytes; i > 0; i--)
        {
            unsigned char temp = bnLocal->bytes[i-1]
                                 << (BITS_PER_BYTE - nShiftBits);
            bnLocal->bytes[i-1] = (bnLocal->bytes[i-1] >> nShiftBits) | carry;
            carry = temp;

            if ((0 != bnLocal->bytes[i-1]) && (0 == numBytesTemp))
            {
                numBytesTemp = i;
            }
        }

        if (0 == numBytesTemp)
        {
            numBytesTemp = 1;
        }

        bnLocal->numBytes = numBytesTemp;
    }
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnValidateToShift_(const bignum num,
                   const unsigned int numBits,
                   unsigned int* nShiftByte,
                   int* nShiftBits)
{
    bignumerr_t rc = bnValidateInitialize_(num);

    if (BN_OK == rc)
    {
        if (numBits <= 0)
        {
            rc = BN_ERR_INVALID_VALUE;
        }
    }

    if (BN_OK == rc)
    {
        *nShiftByte = numBits / BITS_PER_BYTE;
        *nShiftBits = numBits % BITS_PER_BYTE;
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnGenerateRandNum_(bignum num)
{
    bignumerr_t rc = bnValidateInitialize_(num);
    bignumlocal bnLocal = (bignumlocal)(num);

    srand((unsigned int)time(NULL));

    bnLocal->numBytes = bnGetSizeInBytes_();

    for (unsigned int i = 0; i < bnLocal->numBytes; i++)
    {
        bnLocal->bytes[i] = rand() % 256;
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnValidateToContinue_(bignum num, char* charVal, int numCharPerByte)
{
    bignumerr_t rc = BN_OK;
    unsigned numBytes = bnGetSizeInBytes_();
    unsigned int chrLen = bnStrLenOf_(charVal);
    unsigned int byteLen = bnLenInBytesOf_(chrLen, numCharPerByte);

    assert(NULL != num);

    if ((0 >= chrLen) || (0 >= byteLen))
    {
        rc = BN_ERR_INVALID_VALUE;
    }

    if (byteLen > numBytes)
    {
        rc = BN_ERR_OVERFLOW;
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static int
bnIsToCompare_(unsigned int size1, unsigned int size2)
{
    int rc = PARAM1_EQUAL_PARAM2;
    if (size1 > size2)
    {
        rc = PARAM1_GRATER_THEN_PARAM2;
    }
    else if (size1 < size2)
    {
        rc = PARAM1_SMALLER_THEN_PARAM2;
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnCompareBigNumBytes_(bignum num, unsigned char* bytes, unsigned int size)
{
    int rc = PARAM1_EQUAL_PARAM2;
    bignumlocal bnLocal = (bignumlocal)(num);

    assert(BN_OK == bnValidateInitialize_(num));
    assert(NULL != bytes);

    for (unsigned int i = size; i > 0; i--)
    {
        if (bnLocal->bytes[i - 1] > bytes[i - 1])
        {
            rc = PARAM1_GRATER_THEN_PARAM2;
            break;
        }
        else if (bnLocal->bytes[i - 1] < bytes[i - 1])
        {
            rc = PARAM1_SMALLER_THEN_PARAM2;
            break;
        }
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static unsigned int
bnStrLenOf_(char* charVal)
{
    unsigned int rc = 0;

    if (NULL != charVal)
    {
        rc = (unsigned int)strlen(charVal);
    }
    return rc;
}

/*----------------------------------------------------------------------------*/
static unsigned int
bnLenInBytesOf_(unsigned int chrLen, int nCharPerByte)
{
    unsigned int rc = 0;

    if ((chrLen > 0) && (nCharPerByte > 0))
    {
        rc = (chrLen / nCharPerByte);
        if (chrLen % nCharPerByte > 0)
        {
            rc++;
        }
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnAddCharHexInByte_(unsigned char* byte, char charVal)
{
    bignumerr_t rc = BN_OK;

    assert(NULL != byte);

    if (IS_DIGIT(charVal))
    {
        *byte |= DIGIT_TO_HEX(charVal);
    }
    else if (IS_AF(charVal))
    {
        *byte |= AF_TO_HEX(charVal);
    }
    else if (IS_af(charVal))
    {
        *byte |= af_TO_HEX(charVal);
    }
    else
    {
        rc = BN_ERR_INVALID_VALUE;
    }
    return rc;
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnAddCharBinInByte_(unsigned char* byte, char charVal)
{
    bignumerr_t rc = BN_OK;

    assert(NULL != byte);

    if (charVal == '1')
    {
        *byte |= 0x01;
    }
    else if (charVal != '0')
    {
        rc = BN_ERR_INVALID_VALUE;
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static unsigned char
bnChrToByte_(char* charVal)
{
    bignumerr_t rc = BN_OK;
    unsigned char byte = 0;

    assert(NULL != charVal);
    assert(NULL != bnInfo_.callbackToConvert);

    if (0 == charVal[0])
    {
        rc = BN_ERR_INVALID_VALUE;
    }

    if (BN_OK == rc)
    {
        for (int i = 0; i < bnInfo_.numCharPerByte; i++)
        {
            if (0 == charVal[i])
            {
                break;
            }

            byte = byte << (BITS_PER_BYTE / bnInfo_.numCharPerByte);

            rc = bnInfo_.callbackToConvert(&byte, charVal[i]);

            if (BN_OK != rc)
            {
                break;
            }
        }
    }

    bnLastError_ = rc;
    return byte;
}

/*----------------------------------------------------------------------------*/
static void
copyPaddingLeftChar_(unsigned char* ChrBytes,
                     char* charVal,
                     unsigned int chrLen,
                     unsigned int pos,
                     int numChar)
{
    unsigned int index;
    char* temp;
    unsigned int offsetWrite = pos * numChar;

    assert(NULL != ChrBytes);
    assert(NULL != charVal);

    if (offsetWrite < chrLen)
    {
        index = chrLen - offsetWrite;

        if (index >= (unsigned int)numChar)
        {
            temp = (charVal + (index - numChar));
            for (int i = 0; i < numChar; i++)
            {
                ChrBytes[i] = temp[i];
            }
        }
        else
        {
            for (unsigned int i = 0; i < index; i++)
            {
                ChrBytes[i] = charVal[i];
            }

            ChrBytes[index] = '\0';
        }
    }
    else
    {
        ChrBytes[0] = '\0';
    }
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnStrToByte_(unsigned char* ByteVal,
             unsigned int ByteLen,
             char* charVal,
             unsigned int chrLen)
{
    unsigned char c;
    unsigned char tempChr[BITS_PER_BYTE];
    bignumerr_t rc = BN_OK;

    assert(NULL != ByteVal);
    assert(NULL != charVal);

    for (unsigned int i = 0; i < ByteLen; i++)
    {
        copyPaddingLeftChar_(
                tempChr, charVal, chrLen, i, bnInfo_.numCharPerByte);

        c = bnChrToByte_(tempChr);
        ByteVal[i] = c;
        if (BN_OK != bnLastError_)
        {
            rc = bnLastError_;
            break;
        }
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static unsigned int
bnGetSizeInBytes_(void)
{
    unsigned int rc = 0;
    if (0 != numBits_)
    {
        rc = numBits_ / BITS_PER_BYTE;
    }

    bnLastError_ = rc;
    return rc;
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnValidateInitialize_(bignum num)
{
    bignumerr_t rc = BN_ERR;

    if (0 != numBits_)
    {
        if (BN_FREE != num)
        {
            rc = BN_OK;
        }
        else
        {
            rc = BN_ERR_NOT_INST;
        }
    }
    else
    {
        rc = BN_ERR_NOT_INIT;
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnPutBytes_(bignum num, unsigned char* byteChar, unsigned int size)
{
    unsigned numBytes = bnGetSizeInBytes_();
    bignumerr_t rc = bnValidateInitialize_(num);

    assert(NULL != byteChar);
    if (size > numBytes)
    {
        rc = BN_ERR_OVERFLOW;
    }

    if (BN_OK == rc)
    {
        bignumlocal bnLocal = (bignumlocal)(num);

        memset(bnLocal->bytes, 0, numBytes);
        memcpy(bnLocal->bytes, byteChar, size);

        bnLocal->numBytes = 1;
        for (unsigned int i = size; i > 0; i--)
        {
            if (bnLocal->bytes[i - 1] > 0)
            {
                bnLocal->numBytes = i;
                break;
            }
        }
    }

    bnLastError_ = rc;
    return rc;
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnGeneric(bignum num, char* charVal)
{
    bignumerr_t rc =
            bnValidateToContinue_(num, charVal, bnInfo_.numCharPerByte);
    unsigned char* byteVal;
    unsigned int chrLen = bnStrLenOf_(charVal);
    unsigned int byteLen = bnLenInBytesOf_(chrLen, bnInfo_.numCharPerByte);

    if (BN_OK == rc)
    {
        byteVal = (unsigned char*)malloc(byteLen);

        if (NULL != byteVal)
        {
            rc = bnStrToByte_(byteVal, byteLen, charVal, chrLen);

            if (BN_OK == rc)
            {
                rc = bnInfo_.callbackToAction(num, byteVal, byteLen);
            }

            free(byteVal);
        }
        else
        {
            rc = BN_ERR_NOT_MEM;
        }
    }

    bnLastError_ = rc;
    return rc;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumLastError(void)
{
    return bnLastError_;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumInit(unsigned int numBits)
{
    unsigned int numBytes = numBits / BITS_PER_BYTE;
    bnLastError_ = BN_ERR;

    if (0 == numBits_)
    {
        if (((numBytes % sizeof(unsigned int)) == 0) && (numBytes > 0))
        {
            numBits_ = numBits;
            bnLastError_ = BN_OK;
        }
        else
        {
            bnLastError_ = BN_ERR_NOT_SIZE_INT;
        }
    }
    else
    {
        bnLastError_ = BN_ERR_ALREADY_INIT;
    }

    return bnLastError_;
}

/*----------------------------------------------------------------------------*/
bignum
bigNumNew(void)
{
    bignum rc = BN_FREE;
    bnLastError_ = BN_ERR;

    if (0 != numBits_)
    {
        unsigned int numBytes = bnGetSizeInBytes_();
        rc = (bignum)calloc(numBytes + sizeof(unsigned int),
                            sizeof(unsigned char));

        if (NULL != rc)
        {
            pbignumlist node = (pbignumlist)malloc(sizeof(bignumlist));
            rc[0] = 0x1;

            if (NULL != node)
            {
                node->val = rc;
                node->next = bnTop.next;
                bnTop.next = node;
                bnLastError_ = BN_OK;
            }
            else
            {
                free(rc);
                rc = BN_FREE;
            }
        }
        else
        {
            bnLastError_ = BN_ERR_NOT_MEM;
        }
    }
    else
    {
        bnLastError_ = BN_ERR_NOT_INIT;
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
void
bigNumFree(bignum* bnum)
{
    pbignumlist bnPrev;
    pbignumlist bnTemp = &bnTop;
    bnLastError_ = BN_ERR;

    if (BN_FREE != *bnum)
    {
        while (NULL != bnTemp->next)
        {
            bnPrev = bnTemp;
            bnTemp = bnTemp->next;

            if (bnTemp->val == *bnum)
            {
                bnPrev->next = bnTemp->next;
                free(bnTemp);
                bnLastError_ = BN_OK;
                break;
            }
        }

        free(*bnum);
        *bnum = BN_FREE;
    }
    else
    {
        bnLastError_ = BN_ERR_ALREADY_FREE;
    }
}

/*----------------------------------------------------------------------------*/
void
bigNumFinish(void)
{
    bnLastError_ = BN_ERR;

    while (NULL != bnTop.next)
    {
        bignum bnTemp = bnTop.next->val;
        bigNumFree(&bnTemp);
    }

    numBits_ = 0;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumSetInt(bignum num, unsigned int intVal)
{
    unsigned char* byteChar = (unsigned char*)&intVal;
    return bnPutBytes_(num, byteChar, sizeof(unsigned int));
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumSetHex(bignum num, char* charVal)
{
    bignumerr_t rc;
    bnInfo_.numCharPerByte = NUM_CHAR_TO_REPRESENT_HEX;
    bnInfo_.callbackToConvert = bnAddCharHexInByte_;
    bnInfo_.callbackToAction = bnPutBytes_;
    rc = bnGeneric(num, charVal);

    bnLastError_ = rc;
    return rc;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumSetBin(bignum num, char* charVal)
{
    bignumerr_t rc;
    bnInfo_.numCharPerByte = NUM_CHAR_TO_REPRESENT_BIN;
    bnInfo_.callbackToConvert = bnAddCharBinInByte_;
    bnInfo_.callbackToAction = bnPutBytes_;
    rc = bnGeneric(num, charVal);

    bnLastError_ = rc;
    return rc;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumSet(bignum num1, bignum num2)
{
    unsigned int numBytes = bnGetSizeInBytes_();
    bignumerr_t rc = bnValidateInitialize_(num1);

    if (BN_OK == rc)
    {
        rc = bnValidateInitialize_(num2);
    }

    if (BN_OK == rc)
    {
        unsigned int numBytesTot = numBytes + sizeof(unsigned int);
        memcpy(num1, num2, numBytesTot);
    }

    bnLastError_ = rc;
    return rc;
}

/*----------------------------------------------------------------------------*/
int
bigNumCmpInt(bignum num, unsigned int intVal)
{
    bnLastError_ = bnValidateInitialize_(num);
    int rc = LOCAL_ERROR;

    if (BN_OK == bnLastError_)
    {
        bignumlocal bnLocal = (bignumlocal)(num);
        if (bnLocal->numBytes <= sizeof(unsigned int))
        {
            unsigned int* byteInt = (unsigned int*)(&(bnLocal->bytes));
            if (*byteInt == intVal)
            {
                rc = PARAM1_EQUAL_PARAM2;
            }
            else if (*byteInt > intVal)
            {
                rc = PARAM1_GRATER_THEN_PARAM2;
            }
            else
            {
                rc = PARAM1_SMALLER_THEN_PARAM2;
            }
        }
        else
        {
            rc = PARAM1_GRATER_THEN_PARAM2;
        }
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
int
bigNumCmp(bignum num1, bignum num2)
{
    bnLastError_ = bnValidateInitialize_(num1);
    if (BN_OK == bnLastError_)
    {
        bnLastError_ = bnValidateInitialize_(num2);
    }

    int rc = LOCAL_ERROR;

    if (BN_OK == bnLastError_)
    {
        bignumlocal bnLocal1 = (bignumlocal)(num1);
        bignumlocal bnLocal2 = (bignumlocal)(num2);

        rc = bnIsToCompare_(bnLocal1->numBytes, bnLocal2->numBytes);

        if (0 == rc)
        {
            rc = (int)bnCompareBigNumBytes_(
                    num1, bnLocal2->bytes, bnLocal1->numBytes);
        }
    }

    return rc;
}

/*----------------------------------------------------------------------------*/
int
bigNumCmpHex(bignum num, char* charVal)
{
    int rc;

    bnLastError_ =
            bnValidateToContinue_(num, charVal, NUM_CHAR_TO_REPRESENT_HEX);
    if (BN_OK == bnLastError_)
    {
        bnLastError_ = bnValidateInitialize_(num);
    }

    if (BN_OK == bnLastError_)
    {
        bnInfo_.numCharPerByte = NUM_CHAR_TO_REPRESENT_HEX;
        bnInfo_.callbackToConvert = bnAddCharHexInByte_;
        bnInfo_.callbackToAction = bnCompareBigNumBytes_;
        bnLastError_ = bnGeneric(num, charVal);
    }

    rc = bnLastError_;
    return rc;
}

/*----------------------------------------------------------------------------*/
int
bigNumCmpBin(bignum num, char* charVal)
{
    int rc;

    bnLastError_ =
            bnValidateToContinue_(num, charVal, NUM_CHAR_TO_REPRESENT_BIN);
    if (BN_OK == bnLastError_)
    {
        bnLastError_ = bnValidateInitialize_(num);
    }

    if (BN_OK == bnLastError_)
    {
        bnInfo_.numCharPerByte = NUM_CHAR_TO_REPRESENT_BIN;
        bnInfo_.callbackToConvert = bnAddCharBinInByte_;
        bnInfo_.callbackToAction = bnCompareBigNumBytes_;
        bnLastError_ = bnGeneric(num, charVal);
    }

    rc = bnLastError_;
    return rc;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumRand(bignum num)
{
    return bnGenerateRandNum_(num);
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumShiftLeft(bignum num, const unsigned int numBits)
{
    unsigned int nShiftByte;
    int nShiftBits;
    bignumerr_t rc = bnValidateToShift_(num, numBits, &nShiftByte, &nShiftBits);

    if (BN_OK == rc)
    {
        bnShiftLeftNBits_(num, nShiftBits);
        bnShiftLeftNBytes_(num, nShiftByte);
    }

    bnLastError_ = rc;
    return rc;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumShiftRight(bignum num, unsigned int numBits)
{
    unsigned int nShiftByte;
    int nShiftBits;
    bignumerr_t rc = bnValidateToShift_(num, numBits, &nShiftByte, &nShiftBits);

    if (BN_OK == rc)
    {
        bnShifRightNBytes_(num, nShiftByte);
        bnShiftRightNBits_(num, nShiftBits);
    }

    bnLastError_ = rc;
    return rc;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumToStrHex(bignum num, unsigned char* valNum, unsigned int size)
{
    bignumerr_t rc = bnValidateInitialize_(num);

    return rc;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumToStrBin(bignum num, unsigned char* valNum, unsigned int size)
{
    bignumerr_t rc = bnValidateInitialize_(num);

    return rc;
}

/*----------------------------------------------------------------------------*/
void
bigNumPrint(bignum num, BN_PRINT_FLAGS flag)
{
    bignumlocal bnLocal = (bignumlocal)(num);

    unsigned numBytes = bnGetSizeInBytes_();
    bnLastError_ = bnValidateInitialize_(num);

    unsigned char count = 0;
    unsigned char numHexByLine = flag & 0xFF;
    BOOL toPrint0 = (flag & BN_FLAG_PRINT_ZERO) ? TRUE : FALSE;
    char pref[3] = {'0', 'x', '\0'};
    char space = (flag & BN_FLAG_PRINT_SPACE) ? ' ' : '\0';
    char breakLine = '\0';

    pref[0] = (flag & BN_FLAG_PRINT_0X) ? '0' : '\0';

    if (!toPrint0)
    {
        numBytes = bnLocal->numBytes;
    }

    if (BN_OK == bnLastError_)
    {
        for (unsigned i = numBytes; i > 0; i--)
        {
            if (numHexByLine > 0)
            {
                count++;
                breakLine = '\0';

                if ((count % numHexByLine) == 0)
                {
                    breakLine = '\n';
                    count = 0;
                }
            }

            printf("%s%02X%c%c", pref, bnLocal->bytes[i - 1], space, breakLine);
        }

        if ((flag & BN_FLAG_PRINT_BREAK) && ('\0' == breakLine))
        {
            printf("\n");
        }
    }
}
