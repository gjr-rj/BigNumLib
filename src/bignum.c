#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bignum.h>

#define BOOL  unsigned char
#define TRUE  1
#define FALSE 0

#define BITS_PER_BYTE 8

typedef struct _bignumlist
{
    bignum val;
    struct _bignumlist* next;

} bignumlist, *pbignumlist;

static bignumlist bnTop = {NULL, NULL};
static bignumerr_t bnLastError_ = BN_OK;

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
bnHexLenOf_(unsigned int chrLen)
{
    unsigned int rc = (chrLen < 2) ? 1 : (chrLen / 2) + (chrLen % 2);
    return rc;
}

/*----------------------------------------------------------------------------*/
static unsigned char
bn2ChrToByte_(char* charVal)
{
    bignumerr_t rc = BN_OK;
    unsigned char hex = 0;

    if (0 == charVal[0])
    {
        rc = BN_ERR;
    }

    if (BN_OK == rc)
    {
        for (int i = 0; i < 2; i++)
        {
            if (0 == charVal[i])
            {
                continue;
            }

            if (1 == i)
            {
                hex = hex << 4;
            }

            if (IS_DIGIT(charVal[i]))
            {
                hex |= DIGIT_TO_HEX(charVal[i]);
            }
            else if (IS_AF(charVal[i]))
            {
                hex |= AF_TO_HEX(charVal[i]);
            }
            else if (IS_af(charVal[i]))
            {
                hex |= af_TO_HEX(charVal[i]);
            }
            else
            {
                rc = BN_ERR;
                break;
            }
        }
    }

    bnLastError_ = rc;
    return hex;
}

/*----------------------------------------------------------------------------*/
static void
copyPaddingLeft2Char_(unsigned char* Chr2Bytes,
                      char* charVal,
                      unsigned int chrLen,
                      unsigned int offsetWrite)
{
    unsigned int index;
    char* temp;

    if (offsetWrite < chrLen)
    {
        index = chrLen - offsetWrite;

        if (index > 1)
        {
            temp = (charVal + (index - 2));
            Chr2Bytes[0] = temp[0];
            Chr2Bytes[1] = temp[1];
        }
        else
        {
            Chr2Bytes[0] = charVal[0];
            Chr2Bytes[1] = '\0';
        }
    }
    else
    {
        Chr2Bytes[0] = '\0';
    }
}

/*----------------------------------------------------------------------------*/
static bignumerr_t
bnStrHexToByte_(unsigned char* ByteVal,
            unsigned int ByteLen,
            char* charVal,
            unsigned int chrLen)
{
    unsigned char c;
    unsigned char tempChr2[2];
    bignumerr_t rc = BN_OK;

    for (unsigned int i = 0; i < ByteLen; i++)
    {
        copyPaddingLeft2Char_(tempChr2, charVal, chrLen, i * 2);

        c = bn2ChrToByte_(tempChr2);
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
    bignumerr_t rc = BN_OK;
    unsigned char* byteVal;
    unsigned numBytes = bnGetSizeInBytes_();
    unsigned int chrLen = bnStrLenOf_(charVal);
    unsigned int byteLen = bnHexLenOf_(chrLen);

    if (0 >= chrLen)
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
            rc = bnStrHexToByte_(byteVal, byteLen, charVal, chrLen);

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