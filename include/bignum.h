#ifndef BIGNUN_H_
#define BIGNUN_H_

#include <bignumerr.h>

#define BN_FREE NULL

typedef unsigned char* bignum;
typedef int bignumerr_t;

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
void bigNumFree(bignum *bnum);

/**
 * \brief finish the bignum.
 *
 * \note Use `bigNumLastError` to get the last error code.
 */
void bigNumFinish(void);

#endif /* #ifndef BIGNUN_H_ */