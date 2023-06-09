#ifndef BIGNUN_H_
#define BIGNUN_H_

#include <bignumerr.h>

#define BN_FREE NULL /* bigNum is free  */

/* flags to print */
typedef enum
{
    BN_FLAG_PRINT_DEFAULT = 0x00, /* print default */
    BN_FLAG_PRINT_ZERO = 0x100,   /* print leading zero */
    BN_FLAG_PRINT_SPACE = 0x200,  /* print space between bytes */
    BN_FLAG_PRINT_0X = 0x400,     /* print charx '0x' before hexa number */
    BN_FLAG_PRINT_BREAK = 0x800   /* break line after print */
} BN_PRINT_FLAGS;

typedef unsigned char* bignum;
typedef int bignumerr_t;

typedef struct
{
    short major;
    short middle;
    short minor;
    short build;
    char strVersion[12];
} bignunversion_t;

/**
 * \brief return version of the lib.
 *
 * \return bignunversion_t
 *
 */
bignunversion_t bigNumGetVersion(void);

/**
 * \brief return error code of bignum function.
 *
 * This function returns the last error occurred in the execution of some
 * function of the bignum library. Except the same. That is, executing this
 * function is the only one that does not change the last error code.
 *
 * \return bignumerr_t
 * - last error
 *
 * \note BN_OK is the error code to sucess.
 *
 */
bignumerr_t bigNumLastError(void);

/**
 * \brief initializes the bignum, defining the maximum size in bits of
 * the numbers to be manipulated.
 *
 * This function initializes the bignum API by defining the size in
 * bits to be used.
 *
 * \param numBits Max size of big number in bits. The number needs to be
 * a multiple of 8 to give the exact amount of bytes. Only one init is
 * allowed. For new startups it is necessary that the previous one is
 * closed.
 *
 * \return bignumerr_t
 * - BN_OK to sucess
 * - error code.
 */
bignumerr_t bigNumInit(unsigned int numBits);

/**
 * \brief create an bignum instance.
 *
 * This function create an instance of bignum. Only after creating the instance
 * a bignum can be used.
 *
 * \return
 * - bignum instance to sucess
 * - BN_FREE to error.
 *
 * \note Use `bigNumLastError` to get the last error code.
 *
 */
bignum bigNumNew(void);

/**
 * \brief free a bignum instance.
 *
 * This function free an instance of bignum. After free the instance
 * a bignum can't be used.
 *
 * \param bnum to free
 *
 * \note Use `bigNumLastError` to get the last error code.
 */
void bigNumFree(bignum* bnum);

/**
 * \brief finish the bignum.
 *
 * \note Use `bigNumLastError` to get the last error code.
 */
void bigNumFinish(void);

/**
 * \brief Set a bignum with an positive integer value.
 *
 * This function set a bignum with an positive integer value.
 *
 * \param num bignum.
 *
 * \param intVal positive integer.
 *
 * \return bignumerr_t
 * - BN_OK to sucess
 * - error code.
 */
bignumerr_t bigNumSetInt(bignum num, unsigned int intVal);

/**
 * \brief Set a bignum with an hexadecimal string.
 *
 * This function set a bignum with an string representing hexadecimal number.
 *
 * \noteall characters must be hex representatives. The string must be less than
 * or equal to the length defined for the bignum. In case of error, bignum will
 * not have its value set.
 *
 * \param num bignum.
 *
 * \param charVal string.
 *
 * \return bignumerr_t
 * - BN_OK to sucess
 * - error code.
 */
bignumerr_t bigNumSetHex(bignum num, char* charVal);

/**
 * \brief Set a bignum with an binary string.
 *
 * This function set a bignum with an string representing a binary number.
 *
 * \note all characters must be 1 or 0. The string must be less than
 * or equal to the length defined for the bignum. In case of error, bignum will
 * not have its value set.
 *
 * \param num bignum.
 *
 * \param charVal string.
 *
 * \return bignumerr_t
 * - BN_OK to sucess
 * - error code.
 */
bignumerr_t bigNumSetBin(bignum num, char* charVal);

/**
 * \brief Set a bignum with another bignum.
 *
 * \param num1 bignum to set value.
 *
 * \param num2 bignum with value to set.
 *
 * \return bignumerr_t
 * - BN_OK to sucess
 * - error code.
 */
bignumerr_t bigNumSet(bignum num1, bignum num2);

/**
 * \brief Print a bignum.
 *
 * \param num bignum to print.
 *
 * \param flag
 *
 * The first byte defines a number up to 255 referring to the hex
 * number to be printed on the screen before breaking a line. If the value is 0,
 * the byte string will not be broken into blocks.
 *
 * \note Use `bigNumLastError` to get the last error code.
 *
 */
void bigNumPrint(bignum num, BN_PRINT_FLAGS flag);

/**
 * \brief Compare two values, a bignum and an integer.
 *
 * \param num number in bignum.
 *
 * \param intVal number in integer.
 *
 * \return int
 * - 0 to equal
 * - 1 to bignum greater than integer.
 * - (-2) to gibnum smaller the integer.
 *
 * \note Use `bigNumLastError` to get the last error code.
 */
int bigNumCmpInt(bignum num, unsigned int intVal);

/**
 * \brief Compare two bignum values.
 *
 * \param num1 number in bignum.
 *
 * \param num2 number in bignum.
 *
 * \return int
 * - 0 to equal
 * - 1 to num1 greater than num2.
 * - (-2) to num1 smaller the num2.
 *
 * \note Use `bigNumLastError` to get the last error code.
 */
int bigNumCmp(bignum num1, bignum num2);

/**
 * \brief Compare two values, a bignum and a string representing hexadecimal
 * number.
 *
 * \param num number in bignum.
 *
 * \param charVal string as Hex.
 *
 * \return int
 * - 0 to equal
 * - 1 to bignum greater than hex.
 * - (-2) to gibnum smaller the hex.
 *
 * \note Use `bigNumLastError` to get the last error code.
 */
int bigNumCmpHex(bignum num, char* charVal);

/**
 * \brief Compare two values, a bignum and a string representing binary number.
 *
 * \param num number in bignum.
 *
 * \param charVal string as binary.
 *
 * \return int
 * - 0 to equal
 * - 1 to bignum greater than binary.
 * - (-2) to gibnum smaller the binary.
 *
 * \note Use `bigNumLastError` to get the last error code.
 */
int bigNumCmpBin(bignum num, char* charVal);

/**
 * \brief generate random bignum.
 *
 * \param num bignum.
 *
 * \return bignumerr_t
 * - BN_OK to sucess
 * - error code.
 */
bignumerr_t bigNumRand(bignum num);

/**
 * \brief Shift bignum N bits to left.
 *
 * This function shift bignum N bits to left. The number to shift can be between
 * 0 and the maximum allowed by an unsigned int. However, if the number of bits
 * is greater than the number of bits informed when initializing the bignum, the
 * result will always be 0.
 *
 * \param num bignum.
 * \param num num bits to shift.
 *
 * \return bignumerr_t
 * - BN_OK to sucess
 * - error code.
 */
bignumerr_t bigNumShiftLeft(bignum num, const unsigned int numBits);

/**
 * \brief Shift bignum N bits to right.
 *
 * This function shift bignum N bits to rigth. The number to shift can be
 * between 0 and the maximum allowed by an unsigned int. However, if the number
 * of bits is greater than the number of bits informed when initializing the
 * bignum, the result will always be 0.
 *
 * \param num bignum.
 * \param num num bits to shift.
 *
 * \return bignumerr_t
 * - BN_OK to sucess
 * - error code.
 */
bignumerr_t bigNumShiftRight(bignum num, unsigned int numBits);

/**
 * \brief Converts the bignum to a string with hexadecimal representation.
 *
 * \param num bignum.
 * \param valNum will receive the Hex string.
 * \param size size of valNum.
 *
 * \return bignumerr_t
 * - BN_OK to sucess
 * - error code.
 */
bignumerr_t bigNumToStrHex(bignum num,
                           unsigned char* valNum,
                           unsigned int size);

/**
 * \brief Converts the bignum to a string with binary representation.
 *
 * \param num bignum.
 * \param valNum will receive the Bin string.
 * \param size size of valNum.
 *
 * \return bignumerr_t
 * - BN_OK to sucess
 * - error code.
 */
bignumerr_t bigNumToStrBin(bignum num,
                           unsigned char* valNum,
                           unsigned int size);

#endif /* #ifndef BIGNUN_H_ */
