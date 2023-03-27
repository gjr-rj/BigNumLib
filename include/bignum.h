#ifndef BIGNUN_H_
#define BIGNUN_H_

#include <bignumerr.h>>

typedef unsigned char* bignum;

/**
 * \brief initializes the big num, defining the maximum size in bits of the
 * numbers to be manipulated. .
 *
 * This function initializes the big num API by defining the size in bits to be
 * used.
 *
 * \param numBits Max size of big number in bits. The number needs to be a
 * multiple of 8 to give the exact amount of bytes. Only one init is allowed.
 * For new startups it is necessary that the previous one is closed.
 *
 * \return BN_OK to sucess or error.
 *
 */
int bigNumInit(unsigned int numBits);

#endif /* #ifndef BIGNUN_H_ */