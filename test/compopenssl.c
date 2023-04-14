#include <stdio.h>
#include <string.h>

#include <compopenssl.h>

/*----------------------------------------------------------------------------*/
int
openSSLAdd(BIGNUM* r, BIGNUM* a, BIGNUM* b)
{
    BN_add(r, a, b);

    return 0;
}

/*----------------------------------------------------------------------------*/
int
openSSLSub(BIGNUM* r, BIGNUM* a, BIGNUM* b)
{
    BN_sub(r, a, b);

    return 0;
}

/*----------------------------------------------------------------------------*/
void
testOpenSSLhPerformance(unsigned int bigNumSize,
                        char* funcName,
                        testpefoperopenssl* funcOp)
{
    clock_t start;
    clock_t stop;
    double diff;
    float tExec = 0;
    unsigned int rc = 0;
    char* soma_str;

    BIGNUM* num1 = BN_new();
    BIGNUM* num2 = BN_new();
    BIGNUM* soma = BN_new();

    char* chrNum1 = (char*)malloc(bigNumSize / 4);
    char* chrNum2 = (char*)malloc(bigNumSize / 4);

    if (NULL != chrNum1)
    {
        memset(chrNum1, 'c', bigNumSize / 4);
    }
    chrNum1[(bigNumSize / 4) - 1] = '\0';

    if (NULL != chrNum2)
    {
        memset(chrNum2, 'a', bigNumSize / 4);
    }
    chrNum2[(bigNumSize / 4) - 1] = '\0';

    BN_hex2bn(&num1, chrNum1);
    BN_hex2bn(&num2, chrNum2);

    start = clock();
    while (tExec < 1)
    {
        rc++;
        (void)funcOp(soma, num1, num2);
        soma_str = BN_bn2hex(soma);
        free(soma_str);
        stop = clock();
        diff = difftime(stop, start);
        tExec = (float)diff / CLOCKS_PER_SEC;
    }

    free(chrNum1);
    free(chrNum2);

    BN_free(num1);
    BN_free(num2);
    BN_free(soma);

    printf("%s (%u): %u per sec\n", funcName, bigNumSize, rc);
}