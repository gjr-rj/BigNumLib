#include <assert.h>
#include <stdio.h>

#include <bignum.h>

/** @example bignumQA.c
 * Unit and functional tests of the bignum library. The meso serves as an
 * example code of its use
 */
void
main(void)
{
    bignumerr_t err;
    bignum n0 = bigNumNew();

    err = bigNumLastError();
    assert(BN_FREE == n0);
    assert(BN_ERR_NOT_INIT == err);

    err = bigNumInit(3);
    assert(BN_ERR_NOT_SIZE_INT == err);

    err = bigNumInit(7);
    assert(BN_ERR_NOT_SIZE_INT == err);

    err = bigNumInit(42);
    assert(BN_ERR_NOT_SIZE_INT == err);

    err = bigNumInit(32);
    if (BN_OK == err)
    {
        bignum n1 = bigNumNew();
        bignum n2 = bigNumNew();
        bignum n3 = bigNumNew();
        bignum n4 = bigNumNew();
        /*
                assert(bigNumCmpInt(n1, 0) == 0);
                assert(bigNumCmpHex(n2, 0) == 0);
                assert(bigNumCmpBit(n3, 0) == 0);
                assert(bigNumCmp(n3, n4) == 0);
*/
        assert(bigNumSetInt(n1, 83849) == BN_OK);

        bigNumPrint(n1, BN_FLAG_PRINT_DEFAULT);
        printf("\n---------------------------\n");

        bigNumPrint(n1, 1);
        printf("\n---------------------------\n");

        bigNumPrint(n1, 3);
        printf("\n---------------------------\n");

        bigNumPrint(n1, 32 | BN_FLAG_PRINT_ZERO);
        printf("\n---------------------------\n");

        bigNumPrint(n1, 16 | BN_FLAG_PRINT_ZERO | BN_FLAG_PRINT_SPACE);
        printf("\n---------------------------\n");

        bigNumPrint(n1,
                    BN_FLAG_PRINT_SPACE | BN_FLAG_PRINT_0X |
                            BN_FLAG_PRINT_BREAK);
        printf("\n---------------------------\n");

        assert(bigNumSetInt(n1, 256) == BN_OK);
        assert(bigNumSetInt(n1, 256) == BN_OK);
        /*
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
        */
        assert(bigNumSetInt(n2, 163) == 0);
        assert(bigNumSetInt(n3, 17) == 0);

        assert(bigNumInit(48) == BN_ERR_ALREADY_INIT);
        assert(bigNumInit(72) == BN_ERR_ALREADY_INIT);
        /*
                        assert(bigNumSetHex(n2, "1g") == BN_ERR);
                        assert(bigNumSetBit(n3, "1020") == BN_ERR);
                */
        n1 = bigNumNew();
        err = bigNumLastError();
        assert(BN_OK == err);

        bigNumFree(&n1);
        err = bigNumLastError();
        assert(BN_OK == err);

        bigNumFree(&n1);
        err = bigNumLastError();
        assert(BN_ERR_ALREADY_FREE == err);

        n1 = bigNumNew();
        err = bigNumLastError();
        assert(BN_OK == err);

        bigNumFree(&n2);
        err = bigNumLastError();
        assert(BN_OK == err);

        bigNumFinish();
        err = bigNumLastError();
        assert(BN_OK == err);
    }

    err = bigNumInit(64);
    if (BN_OK == err)
    {
        bignum n1 = bigNumNew();
        bignum n2 = bigNumNew();
        bignum n3 = bigNumNew();
        bignum n4 = bigNumNew();
        /*
                assert(bigNumCmpInt(n1, 0) == 0);
                assert(bigNumCmpHex(n2, 0) == 0);
                assert(bigNumCmpBit(n3, 0) == 0);
                assert(bigNumCmp(n3, n4) == 0);
*/
        assert(bigNumSetInt(n1, 838493355) == BN_OK);

        bigNumPrint(n1, BN_FLAG_PRINT_DEFAULT);
        printf("\n---------------------------\n");

        bigNumPrint(n1, 1);
        printf("\n---------------------------\n");

        bigNumPrint(n1, 3);
        printf("\n---------------------------\n");

        bigNumPrint(n1, 32 | BN_FLAG_PRINT_ZERO);
        printf("\n---------------------------\n");

        bigNumPrint(n1, 16 | BN_FLAG_PRINT_ZERO | BN_FLAG_PRINT_SPACE);
        printf("\n---------------------------\n");

        bigNumPrint(n1,
                    BN_FLAG_PRINT_SPACE | BN_FLAG_PRINT_0X |
                            BN_FLAG_PRINT_BREAK);
        printf("\n---------------------------\n");

        /*
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
*/
        assert(bigNumSetInt(n2, 163) == 0);
        assert(bigNumSetInt(n3, 17) == 0);

        assert(bigNumInit(32) == BN_ERR_ALREADY_INIT);
        assert(bigNumInit(64) == BN_ERR_ALREADY_INIT);
        /*
                        assert(bigNumSetHex(n2, "1g") == BN_ERR);
                        assert(bigNumSetBit(n3, "1020") == BN_ERR);
                */
        n1 = bigNumNew();
        err = bigNumLastError();
        assert(BN_OK == err);

        bigNumFree(&n1);
        err = bigNumLastError();
        assert(BN_OK == err);

        bigNumFree(&n1);
        err = bigNumLastError();
        assert(BN_ERR_ALREADY_FREE == err);

        n1 = bigNumNew();
        err = bigNumLastError();
        assert(BN_OK == err);

        bigNumFree(&n2);
        err = bigNumLastError();
        assert(BN_OK == err);

        bigNumFinish();
        err = bigNumLastError();
        assert(BN_OK == err);
    }

    err = bigNumInit(320);
    if (BN_OK == err)
    {
        bignum n1 = bigNumNew();
        bignum n2 = bigNumNew();
        bignum n3 = bigNumNew();
        bignum n4 = bigNumNew();
        /*
                assert(bigNumCmpInt(n1, 0) == 0);
                assert(bigNumCmpHex(n2, 0) == 0);
                assert(bigNumCmpBit(n3, 0) == 0);
                assert(bigNumCmp(n3, n4) == 0);
*/
        assert(bigNumSetInt(n1, 83849) == BN_OK);
        /*
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
        */
        assert(bigNumSetInt(n2, 163) == 0);
        assert(bigNumSetInt(n3, 17) == 0);

        assert(bigNumInit(32) == BN_ERR_ALREADY_INIT);
        assert(bigNumInit(64) == BN_ERR_ALREADY_INIT);
        /*
                        assert(bigNumSetHex(n2, "1g") == BN_ERR);
                        assert(bigNumSetBit(n3, "1020") == BN_ERR);
                */
        n1 = bigNumNew();
        err = bigNumLastError();
        assert(BN_OK == err);

        bigNumFree(&n1);
        err = bigNumLastError();
        assert(BN_OK == err);

        bigNumFree(&n1);
        err = bigNumLastError();
        assert(BN_ERR_ALREADY_FREE == err);

        n1 = bigNumNew();
        err = bigNumLastError();
        assert(BN_OK == err);

        bigNumFree(&n2);
        err = bigNumLastError();
        assert(BN_OK == err);

        bigNumFinish();
        err = bigNumLastError();
        assert(BN_OK == err);
    }

    err = bigNumInit(320000);
    if (BN_OK == err)
    {
        bignum n1 = bigNumNew();
        bignum n2 = bigNumNew();
        bignum n3 = bigNumNew();
        bignum n4 = bigNumNew();
        /*
                assert(bigNumCmpInt(n1, 0) == 0);
                assert(bigNumCmpHex(n2, 0) == 0);
                assert(bigNumCmpBit(n3, 0) == 0);
                assert(bigNumCmp(n3, n4) == 0);
*/
        assert(bigNumSetInt(n1, 838493355) == BN_OK);

        bigNumPrint(n1, BN_FLAG_PRINT_DEFAULT);
        printf("\n---------------------------\n");

        bigNumPrint(n1, 1);
        printf("\n---------------------------\n");

        bigNumPrint(n1, 3);
        printf("\n---------------------------\n");

        bigNumPrint(n1, 32 | BN_FLAG_PRINT_ZERO);
        printf("\n---------------------------\n");

        bigNumPrint(n1, 16 | BN_FLAG_PRINT_ZERO | BN_FLAG_PRINT_SPACE);
        printf("\n---------------------------\n");

        bigNumPrint(n1,
                    BN_FLAG_PRINT_SPACE | BN_FLAG_PRINT_0X |
                            BN_FLAG_PRINT_BREAK);
        printf("\n---------------------------\n");
        /*
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
        */
        assert(bigNumSetInt(n2, 163) == 0);
        assert(bigNumSetInt(n3, 17) == 0);

        assert(bigNumInit(32) == BN_ERR_ALREADY_INIT);
        assert(bigNumInit(64) == BN_ERR_ALREADY_INIT);
        /*
                        assert(bigNumSetHex(n2, "1g") == BN_ERR);
                        assert(bigNumSetBit(n3, "1020") == BN_ERR);
                */
        n1 = bigNumNew();
        err = bigNumLastError();
        assert(BN_OK == err);

        bigNumFree(&n1);
        err = bigNumLastError();
        assert(BN_OK == err);

        bigNumFree(&n1);
        err = bigNumLastError();
        assert(BN_ERR_ALREADY_FREE == err);

        n1 = bigNumNew();
        err = bigNumLastError();
        assert(BN_OK == err);

        bigNumFree(&n2);
        err = bigNumLastError();
        assert(BN_OK == err);

        bigNumFinish();
        err = bigNumLastError();
        assert(BN_OK == err);
    }

    printf("##############################\n");
    printf("#            Test OK         #\n");
    printf("##############################\n");
}