#ifndef BIGNUMERR_H_
#define BIGNUMERR_H_

#define BN_OK   0    /* sucess */
#define BN_ERR  (-1) /* generic error */

#define BN_ERR_ALREADY_INIT     1 /* bignum already initialized */
#define BN_ERR_NOT_SIZE_INT     2 /* not multiple of integer */
#define BN_ERR_NOT_INIT         3 /* not initialized*/
#define BN_ERR_NOT_MEM          4 /* enough of memory */
#define BN_ERR_ALREADY_FREE     5 /* bignum already free */
#define BN_ERR_NOT_INST         6 /* bignum not instanced */
#define BN_ERR_OVERFLOW         7 /* bignum overflow */

#endif /* #ifndef BIGNUMERR_H_ */