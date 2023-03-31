#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <bignum.h>

#define BOOL  unsigned char
#define TRUE  1
#define FALSE 0

#define BITS_PER_BYTE 8

#define NUM_CHAR_TO_REPRESENT_HEX 2
#define NUM_CHAR_TO_REPRESENT_BIN 8

typedef bignumerr_t (*bnFuncAddCharToByte)(unsigned char*, char);
typedef struct _bignumlist
{
    bignum val;
    struct _bignumlist* next;

} bignumlist, *pbignumlist;

typedef struct
{
    int numCharPerByte;
    bnFuncAddCharToByte callbackToConvert;
} bignumopinfo;

static bignumlist bnTop = {NULL, NULL};
static bignumerr_t bnLastError_ = BN_OK;
static bignumopinfo bnInfo_ = {0, NULL};

static unsigned int numBits_ = 0;

#define IS_DIGIT(n)     ((n >= '0') && (n <= '9'))
#define DIGIT_TO_HEX(n) (n - '0')

#define IS_AF(n)     ((n >= 'A') && (n <= 'F'))
#define AF_TO_HEX(n) (n - 'A' + 10)

#define IS_af(n)     ((n >= 'a') && (n <= 'f'))
#define af_TO_HEX(n) (n - 'a' + 10)

static unsigned int bnStrLenOf_(char* charVal);
static unsigned int bnHexLenOf_(unsigned int chrLen);
static unsigned char bn2ChrToByte_(char* charVal);
static bignumerr_t bnPutBytes_(bignum num,
                               unsigned char* byteChar,
                               unsigned int size);

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
        rc = BN_ERR;
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
        rc = BN_ERR;
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
        rc = BN_ERR;
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
        memset(num, 0, numBytes);
        memcpy(num, byteChar, size);
    }

    bnLastError_ = rc;
    return rc;
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnSetGeneric(bignum num, char* charVal)
{
    bignumerr_t rc = BN_OK;
    unsigned char* byteVal;
    unsigned numBytes = bnGetSizeInBytes_();
    unsigned int chrLen = bnStrLenOf_(charVal);
    unsigned int byteLen = bnLenInBytesOf_(chrLen, bnInfo_.numCharPerByte);

    if ((0 >= chrLen) || (0 >= byteLen))
    {
        rc = BN_ERR;
    }

    if (byteLen > numBytes)
    {
        rc = BN_ERR_OVERFLOW;
    }

    if (BN_OK == rc)
    {
        byteVal = (unsigned char*)malloc(byteLen);

        if (NULL != byteVal)
        {
            rc = bnStrToByte_(byteVal, byteLen, charVal, chrLen);

            if (BN_OK == rc)
            {
                rc = bnPutBytes_(num, byteVal, byteLen);
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
        rc = (bignum)calloc(numBytes, sizeof(unsigned char));

        if (NULL != rc)
        {
            pbignumlist node = (pbignumlist)malloc(sizeof(bignumlist));
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
    rc = bnSetGeneric(num, charVal);

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
    rc = bnSetGeneric(num, charVal);

    bnLastError_ = rc;
    return rc;
}

/*----------------------------------------------------------------------------*/
bignumerr_t
bigNumSet(bignum num, bignum bnVal)
{
    unsigned numBytes = bnGetSizeInBytes_();
    bignumerr_t rc = bnValidateInitialize_(num);

    if (BN_OK == rc)
    {
        rc = bnValidateInitialize_(bnVal);
    }

    if (BN_OK == rc)
    {
        memcpy(num, bnVal, numBytes);
    }

    bnLastError_ = rc;
    return rc;
}

/*----------------------------------------------------------------------------*/
void
bigNumPrint(bignum num, BN_PRINT_FLAGS flag)
{
    unsigned numBytes = bnGetSizeInBytes_();
    bnLastError_ = bnValidateInitialize_(num);

    unsigned char count = 0;
    unsigned char numHexByLine = flag & 0xFF;
    BOOL toPrint = (flag & BN_FLAG_PRINT_ZERO) ? TRUE : FALSE;
    char pref[3] = {'0', 'x', '\0'};
    char space = (flag & BN_FLAG_PRINT_SPACE) ? ' ' : '\0';
    char breakLine = '\0';

    pref[0] = (flag & BN_FLAG_PRINT_0X) ? '0' : '\0';

    if (BN_OK == bnLastError_)
    {
        for (unsigned i = numBytes; i > 0; i--)
        {
            if (!toPrint)
            {
                if (num[i - 1] > 0)
                {
                    toPrint = TRUE;
                }
            }

            if (toPrint)
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

                printf("%s%02X%c%c", pref, num[i - 1], space, breakLine);
            }
        }

        if ((flag & BN_FLAG_PRINT_BREAK) && ('\0' == breakLine))
        {
            printf("\n");
        }
    }
}