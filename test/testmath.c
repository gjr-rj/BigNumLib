#include <assert.h>
#include <stdio.h>

#include <bignum.h>
#include <bignummath.h>
#include <testmath.h>

/*----------------------------------------------------------------------------*/
void
testAdd(void)
{
    bignumerr_t err = bigNumInit(320);
    if (BN_OK == err)
    {
        bignum n1 = bigNumNew();
        bignum n2 = bigNumNew();
        bignum n3 = bigNumNew();

        assert(bigNumAdd(n3, NULL, NULL) == BN_ERR_NOT_INST);
        assert(bigNumCmpHex(n3, "0") == 0);

        assert(bigNumAdd(n3, n2, n1) == BN_OK);
        assert(bigNumCmpHex(n3, "0") == 0);

        assert(bigNumSetHex(n2, "1") == BN_OK);
        assert(bigNumAdd(n3, n2, n1) == BN_OK);
        assert(bigNumCmpHex(n3, "1") == 0);

        assert(bigNumSetHex(n1, "1") == BN_OK);
        assert(bigNumAdd(n3, n2, n1) == BN_OK);
        assert(bigNumCmpHex(n3, "2") == 0);

        assert(bigNumSetHex(n1, "FE") == BN_OK);
        assert(bigNumAdd(n3, n2, n1) == BN_OK);
        assert(bigNumCmpHex(n3, "FF") == 0);

        assert(bigNumSetHex(n1, "EF") == BN_OK);
        assert(bigNumAdd(n3, n2, n1) == BN_OK);
        assert(bigNumCmpHex(n3, "F0") == 0);

        assert(bigNumSetHex(n2, "1") == BN_OK);
        assert(bigNumSetHex(n1,
                            "FFFFFFFFFFFFFFFFFFFF"
                            "FFFFFFFFFFFFFFFFFFFF"
                            "FFFFFFFFFFFFFFFFFFFF"
                            "FFFFFFFFFFFFFFFFFFFF") == BN_OK);
        assert(bigNumAdd(n3, n2, n1) == BN_ERR_OVERFLOW);
        assert(bigNumCmpHex(n3, "0") == 0);

        assert(bigNumAdd(n3, n1, n2) == BN_ERR_OVERFLOW);
        assert(bigNumCmpHex(n3, "0") == 0);

        assert(bigNumSetHex(n1, "1010101010101010") == BN_OK);
        assert(bigNumSetHex(n2, "0101010101010101") == BN_OK);
        assert(bigNumAdd(n3, n2, n1) == BN_OK);
        assert(bigNumCmpHex(n3, "1111111111111111") == 0);

        assert(bigNumSetHex(n1, "fffffffffffffff") == BN_OK);
        assert(bigNumSetHex(n2, "fffffffffffffff") == BN_OK);
        assert(bigNumAdd(n3, n2, n1) == BN_OK);
        assert(bigNumCmpHex(n3, "1ffffffffffffffe") == 0);

        assert(bigNumSetHex(n1, "ED148554796DC") == BN_OK);
        assert(bigNumSetHex(n2, "488D75FEAB6") == BN_OK);
        assert(bigNumAdd(n3, n2, n1) == BN_OK);
        assert(bigNumCmpHex(n3, "ED5D12CA78192") == 0);

        bigNumFinish();
    }

    err = bigNumInit(64);
    if (BN_OK == err)
    {
        bignum n1 = bigNumNew();
        bignum n2 = bigNumNew();
        bignum n3 = bigNumNew();

        assert(bigNumSetHex(n1, "FFFFFFFFFFFFFFF") == BN_OK);
        assert(bigNumSetHex(n2, "FFFFFFFFFFFFFFF") == BN_OK);
        assert(bigNumAdd(n3, n1, n2) == BN_OK);
        assert(bigNumCmpHex(n3, "1FFFFFFFFFFFFFFE") == 0);

        assert(bigNumSetHex(n1, "FFFFFFFFFFFFFFFF") == BN_OK);
        assert(bigNumSetHex(n2, "FFFFFFFFFFFFFFFF") == BN_OK);
        assert(bigNumAdd(n3, n1, n2) == BN_ERR_OVERFLOW);
        assert(bigNumCmpHex(n3, "FFFFFFFE") == 0);

        bigNumFinish();
    }
}

/*----------------------------------------------------------------------------*/
void
testSub(void)
{
    bignumerr_t err = bigNumInit(320);
    if (BN_OK == err)
    {
        bignum n1 = bigNumNew();
        bignum n2 = bigNumNew();
        bignum n3 = bigNumNew();

        assert(bigNumSub(n3, NULL, NULL) == BN_ERR_NOT_INST);
        assert(bigNumCmpHex(n3, "0") == 0);

        assert(bigNumSub(n3, n2, n1) == BN_OK);
        assert(bigNumCmpHex(n3, "0") == 0);

        assert(bigNumSetHex(n2, "1") == BN_OK);
        assert(bigNumSub(n3, n2, n1) == BN_OK);
        assert(bigNumCmpHex(n3, "1") == 0);

        assert(bigNumSub(n3, n1, n2) == BN_ERR_OVERFLOW);
        assert(bigNumCmpHex(n3,
                            "FFFFFFFFFFFFFFFFFFFF"
                            "FFFFFFFFFFFFFFFFFFFF"
                            "FFFFFFFFFFFFFFFFFFFF"
                            "FFFFFFFFFFFFFFFFFFFF") == 0);

        assert(bigNumSetHex(n2, "345F00") == BN_OK);
        assert(bigNumSub(n3, n2, n1) == BN_OK);
        assert(bigNumCmpHex(n3, "345F00") == 0);

        assert(bigNumSub(n3, n1, n2) == BN_ERR_OVERFLOW);
        assert(bigNumCmpHex(n3,
                            "FFFFFFFFFFFFFFFFFFFF"
                            "FFFFFFFFFFFFFFFFFFFF"
                            "FFFFFFFFFFFFFFFFFFFF"
                            "FFFFFFFFFFFFFFCBA100") == 0);

        assert(bigNumSetHex(n2, "1") == BN_OK);
        assert(bigNumSetHex(n1, "1") == BN_OK);
        assert(bigNumSub(n3, n2, n1) == BN_OK);
        assert(bigNumCmpHex(n3, "0") == 0);

        assert(bigNumSetHex(n1, "FE") == BN_OK);
        assert(bigNumSetHex(n2, "FF") == BN_OK);
        assert(bigNumSub(n3, n2, n1) == BN_OK);
        assert(bigNumCmpHex(n3, "1") == 0);

        assert(bigNumSetHex(n1, "1010101010101010") == BN_OK);
        assert(bigNumSetHex(n2, "0101010101010101") == BN_OK);
        assert(bigNumSub(n3, n2, n1) == BN_ERR_OVERFLOW);
        assert(bigNumCmpHex(n3, "F0F0F0F0F0F0F0F1") == 0);

        assert(bigNumSub(n3, n1, n2) == BN_OK);
        assert(bigNumCmpHex(n3, "F0F0F0F0F0F0F0F") == 0);

        assert(bigNumSetHex(n1, "fffffffffffffff") == BN_OK);
        assert(bigNumSetHex(n2, "fffffffffffffff") == BN_OK);
        assert(bigNumSub(n3, n2, n1) == BN_OK);
        assert(bigNumCmpHex(n3, "0") == 0);

        assert(bigNumSetHex(n1, "ED148554796DC") == BN_OK);
        assert(bigNumSetHex(n2, "488D75FEAB6") == BN_OK);
        assert(bigNumSub(n3, n1, n2) == BN_OK);
        assert(bigNumCmpHex(n3, "ECCBF7DE7AC26") == 0);

        assert(bigNumSub(n3, n2, n1) == BN_ERR_OVERFLOW);
        bigNumPrint(n3, BN_FLAG_PRINT_SPACE | BN_FLAG_PRINT_BREAK);
        assert(bigNumCmpHex(n3,
                            "FFFFFFFFFFFFFFFFFFFF"
                            "FFFFFFFFFFFFFFFFFFFF"
                            "FFFFFFFFFFFFFFFFFFFF"
                            "FFFFFFF13340821853DA") == 0);

        bigNumFinish();
    }

    err = bigNumInit(64);
    if (BN_OK == err)
    {
        bignum n1 = bigNumNew();
        bignum n2 = bigNumNew();
        bignum n3 = bigNumNew();

        assert(bigNumSetHex(n1, "FFFFFFFFFFFFFFF") == BN_OK);
        assert(bigNumSetHex(n2, "F000000000000000") == BN_OK);
        assert(bigNumSub(n3, n1, n2) == BN_ERR_OVERFLOW);
        assert(bigNumCmpHex(n3, "1FFFFFFFFFFFFFFF") == 0);

        assert(bigNumSub(n3, n2, n1) == BN_OK);
        assert(bigNumCmpHex(n3, "E000000000000001") == 0);

        assert(bigNumSetHex(n1, "1234567890000") == BN_OK);
        assert(bigNumSetHex(n2, "1000987654321") == BN_OK);
        assert(bigNumSub(n3, n1, n2) == BN_OK);
        assert(bigNumCmpHex(n3, "233BE023BCDF") == 0);

        assert(bigNumSub(n3, n2, n1) == BN_ERR_OVERFLOW);
        assert(bigNumCmpHex(n3, "FFFFDCC41FDC4321") == 0);
        bigNumFinish();
    }
}

/*----------------------------------------------------------------------------*/
void
testMath(void)
{
    testAdd();
    testSub();
}