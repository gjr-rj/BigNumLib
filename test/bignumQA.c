#include <assert.h>
#include <bignum.h>

/** @example bignumQA.c
 * Unit and functional tests of the bignum library. The meso serves as an
 * example code of its use
 */
void main(void) {
    bignum n0 = bigNumNew();

    assert(BN_FREE == n0);
    assert(bigNumLastErr() == BN_ERR_NOT_INIT);

    assert(bigNumInit(3) == BN_ERR_NOT_SIZE_BYTE);
    assert(bigNumInit(7) == BN_ERR_NOT_SIZE_BYTE);
    assert(bigNumInit(42) == BN_ERR_NOT_SIZE_BYTE);

    if (BN_OK == bigNumInit(8)) {
        bignum n1 = bigNumNew();
        bignum n2 = bigNumNew();
        bignum n3 = bigNumNew();
        bignum n4 = bigNumNew();

        assert(bigNumCmpInt(n1, 0) == 0);
        assert(bigNumCmpHex(n2, 0) == 0);
        assert(bigNumCmpBit(n3, 0) == 0);
        assert(bigNumCmp(n3, n4) == 0);

        assert(bigNumSetInt(n1, 16) == BN_OK);
        assert(bigNumSetHex(n2, "10") == BN_OK);
        assert(bigNumSetBit(n3, "1000") == BN_OK);
        assert(bigNumSet(n4, n3) == BN_OK);

        assert(bigNumCmpInt(n1, 16) == 0);
        assert(bigNumCmpInt(n1, 16) == 0);
        assert(bigNumCmpInt(n1, 8) == 0);
        assert(bigNumCmpInt(n1, 8) == 0);

        assert(bigNumCmpInt(n1, 10) == 1);
        assert(bigNumCmpInt(n1, 10) == 1);
        assert(bigNumCmpInt(n1, 16) == -1);
        assert(bigNumCmpInt(n1, 16) == -1);

        assert(bigNumCmpHex(n1, "10") == 0);
        assert(bigNumCmpBit(n2, "10000") == 0);
        assert(bigNumCmp(n3, n4) == 0);

        assert(bigNumSetHex(n2, "A3") == BN_OK);
        assert(bigNumSetBit(n3, "10001") == BN_OK);

        assert(bigNumSetInt(n2, 163) == 0);
        assert(bigNumSetInt(n3, 17) == 0);

        assert(bigNumInit(7) == BN_ERR_ALREADY_INIT);
        assert(bigNumInit(8) == BN_ERR_ALREADY_INIT);

        assert(bigNumSetHex(n2, "1g") == BN_ERR);
        assert(bigNumSetBit(n3, "1020") == BN_ERR);

        n1 = bigNumNew();

        bigNumFree(n1);
        bigNumFree(n1);

        n1 = bigNumNew();

        bignumFree(n2);

        bigNumFinish();
    }

    if (BN_OK == bigNumInit(16)) {
        bignum n1 = bigNumNew();
        bignum n2 = bigNumNew();
        bignum n3 = bigNumNew();
        bignum n4 = bigNumNew();

        assert(bigNumCmpInt(n1, 0) == 0);
        assert(bigNumCmpHex(n2, 0) == 0);
        assert(bigNumCmpBit(n3, 0) == 0);
        assert(bigNumCmp(n3, n4) == 0);

        assert(bigNumSetInt(n1, 16) == BN_OK);
        assert(bigNumSetHex(n2, "10") == BN_OK);
        assert(bigNumSetBit(n3, "1000") == BN_OK);
        assert(bigNumSet(n4, n3) == BN_OK);

        assert(bigNumCmpInt(n1, 16) == 0);
        assert(bigNumCmpInt(n1, 16) == 0);
        assert(bigNumCmpInt(n1, 8) == 0);
        assert(bigNumCmpInt(n1, 8) == 0);

        assert(bigNumCmpInt(n1, 10) == 1);
        assert(bigNumCmpInt(n1, 10) == 1);
        assert(bigNumCmpInt(n1, 16) == -1);
        assert(bigNumCmpInt(n1, 16) == -1);

        assert(bigNumCmpHex(n1, "10") == 0);
        assert(bigNumCmpBit(n2, "10000") == 0);
        assert(bigNumCmp(n3, n4) == 0);

        assert(bigNumSetHex(n2, "A3") == BN_OK);
        assert(bigNumSetBit(n3, "10001") == BN_OK);

        assert(bigNumSetInt(n2, 163) == 0);
        assert(bigNumSetInt(n3, 17) == 0);

        assert(bigNumInit(7) == BN_ERR_ALREADY_INIT);
        assert(bigNumInit(8) == BN_ERR_ALREADY_INIT);

        assert(bigNumSetHex(n2, "1g") == BN_ERR);
        assert(bigNumSetBit(n3, "1020") == BN_ERR);

        n1 = bigNumNew();

        bigNumFree(n1);
        bigNumFree(n1);

        n1 = bigNumNew();

        bignumFree(n2);

        bigNumFinish();
    }

    if (BN_OK == bigNumInit(40)) {
        bignum n1 = bigNumNew();
        bignum n2 = bigNumNew();
        bignum n3 = bigNumNew();
        bignum n4 = bigNumNew();

        assert(bigNumCmpInt(n1, 0) == 0);
        assert(bigNumCmpHex(n2, 0) == 0);
        assert(bigNumCmpBit(n3, 0) == 0);
        assert(bigNumCmp(n3, n4) == 0);

        assert(bigNumSetInt(n1, 16) == BN_OK);
        assert(bigNumSetHex(n2, "10") == BN_OK);
        assert(bigNumSetBit(n3, "1000") == BN_OK);
        assert(bigNumSet(n4, n3) == BN_OK);

        assert(bigNumCmpInt(n1, 16) == 0);
        assert(bigNumCmpInt(n1, 16) == 0);
        assert(bigNumCmpInt(n1, 8) == 0);
        assert(bigNumCmpInt(n1, 8) == 0);

        assert(bigNumCmpInt(n1, 10) == 1);
        assert(bigNumCmpInt(n1, 10) == 1);
        assert(bigNumCmpInt(n1, 16) == -1);
        assert(bigNumCmpInt(n1, 16) == -1);

        assert(bigNumCmpHex(n1, "10") == 0);
        assert(bigNumCmpBit(n2, "10000") == 0);
        assert(bigNumCmp(n3, n4) == 0);

        assert(bigNumSetHex(n2, "A3") == BN_OK);
        assert(bigNumSetBit(n3, "10001") == BN_OK);

        assert(bigNumSetInt(n2, 163) == 0);
        assert(bigNumSetInt(n3, 17) == 0);

        assert(bigNumInit(7) == BN_ERR_ALREADY_INIT);
        assert(bigNumInit(8) == BN_ERR_ALREADY_INIT);

        assert(bigNumSetHex(n2, "1g") == BN_ERR);
        assert(bigNumSetBit(n3, "1020") == BN_ERR);

        n1 = bigNumNew();

        bigNumFree(n1);
        bigNumFree(n1);

        n1 = bigNumNew();

        bignumFree(n2);

        bigNumFinish();
    }

    if (BN_OK == bigNumInit(80000)) {
        bignum n1 = bigNumNew();
        bignum n2 = bigNumNew();
        bignum n3 = bigNumNew();
        bignum n4 = bigNumNew();

        assert(bigNumCmpInt(n1, 0) == 0);
        assert(bigNumCmpHex(n2, 0) == 0);
        assert(bigNumCmpBit(n3, 0) == 0);
        assert(bigNumCmp(n3, n4) == 0);

        assert(bigNumSetInt(n1, 16) == BN_OK);
        assert(bigNumSetHex(n2, "10") == BN_OK);
        assert(bigNumSetBit(n3, "1000") == BN_OK);
        assert(bigNumSet(n4, n3) == BN_OK);

        assert(bigNumCmpInt(n1, 16) == 0);
        assert(bigNumCmpInt(n1, 16) == 0);
        assert(bigNumCmpInt(n1, 8) == 0);
        assert(bigNumCmpInt(n1, 8) == 0);

        assert(bigNumCmpInt(n1, 10) == 1);
        assert(bigNumCmpInt(n1, 10) == 1);
        assert(bigNumCmpInt(n1, 16) == -1);
        assert(bigNumCmpInt(n1, 16) == -1);

        assert(bigNumCmpHex(n1, "10") == 0);
        assert(bigNumCmpBit(n2, "10000") == 0);
        assert(bigNumCmp(n3, n4) == 0);

        assert(bigNumSetHex(n2, "A3") == BN_OK);
        assert(bigNumSetBit(n3, "10001") == BN_OK);

        assert(bigNumSetInt(n2, 163) == 0);
        assert(bigNumSetInt(n3, 17) == 0);

        assert(bigNumInit(7) == BN_ERR_ALREADY_INIT);
        assert(bigNumInit(8) == BN_ERR_ALREADY_INIT);

        assert(bigNumSetHex(n2, "1g") == BN_ERR);
        assert(bigNumSetBit(n3, "1020") == BN_ERR);

        n1 = bigNumNew();

        bigNumFree(n1);
        bigNumFree(n1);

        n1 = bigNumNew();

        bignumFree(n2);

        bigNumFinish();
    }
}