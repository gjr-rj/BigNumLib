#ifndef COMP_OPEN_SSL_H
#define COMP_OPEN_SSL_H

#include <openssl/bn.h>

typedef int(testpefoperopenssl)(BIGNUM* r, BIGNUM* a, BIGNUM* b);

int openSSLAdd(BIGNUM* r, BIGNUM* a, BIGNUM* b);
int openSSLSub(BIGNUM* r, BIGNUM* a, BIGNUM* b);
void testOpenSSLhPerformance(unsigned int bigNumSize,
                             char* funcName,
                             testpefoperopenssl* funcOp);

#define PERFORMANCE_OPENSSL(x, y) testOpenSSLhPerformance(x, #y, y)

#endif