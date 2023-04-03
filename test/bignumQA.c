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
    char numStr[1000];
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

        assert(bigNumCmpInt(n1, 0) == 0);
        assert(bigNumShiftLeft(n1, 0) == BN_ERR_INVALID_VALUE);
        assert(bigNumShiftLeft(n1, 1) == BN_OK);
        assert(bigNumShiftLeft(n1, 2) == BN_OK);
        assert(bigNumShiftLeft(n1, 3) == BN_OK);
        assert(bigNumShiftLeft(n1, 6) == BN_OK);
        assert(bigNumShiftLeft(n1, 7) == BN_OK);
        assert(bigNumShiftLeft(n1, 8) == BN_OK);
        assert(bigNumShiftLeft(n1, 9) == BN_OK);

        assert(bigNumShiftRight(n1, 0) == BN_ERR_INVALID_VALUE);
        assert(bigNumShiftRight(n1, 1) == BN_OK);
        assert(bigNumShiftRight(n1, 2) == BN_OK);
        assert(bigNumShiftRight(n1, 3) == BN_OK);
        assert(bigNumShiftRight(n1, 6) == BN_OK);
        assert(bigNumShiftRight(n1, 7) == BN_OK);
        assert(bigNumShiftRight(n1, 8) == BN_OK);
        assert(bigNumShiftRight(n1, 9) == BN_OK);

        assert(bigNumShiftLeft(n1, 24) == BN_OK);
        assert(bigNumShiftLeft(n1, 31) == BN_OK);
        assert(bigNumShiftLeft(n1, 32) == BN_OK);
        assert(bigNumShiftLeft(n1, 33) == BN_OK);

        assert(bigNumSetHex(n1, "0") == BN_OK);
        assert(bigNumShiftRight(n1, 1) == BN_OK);
        assert(bigNumShiftRight(n1, 2) == BN_OK);
        assert(bigNumShiftRight(n1, 3) == BN_OK);
        assert(bigNumShiftRight(n1, 6) == BN_OK);
        assert(bigNumShiftRight(n1, 7) == BN_OK);
        assert(bigNumShiftRight(n1, 8) == BN_OK);
        assert(bigNumShiftRight(n1, 9) == BN_OK);

        assert(bigNumShiftLeft(n1, 24) == BN_OK);
        assert(bigNumShiftLeft(n1, 31) == BN_OK);
        assert(bigNumShiftLeft(n1, 32) == BN_OK);
        assert(bigNumShiftLeft(n1, 33) == BN_OK);

        assert(bigNumSetHex(n1, "1") == BN_OK);
        assert(bigNumShiftLeft(n1, 1) == BN_OK);
        assert(bigNumShiftLeft(n1, 2) == BN_OK);
        assert(bigNumShiftLeft(n1, 3) == BN_OK);
        assert(bigNumShiftLeft(n1, 6) == BN_OK);

        assert(bigNumShiftRight(n1, 6) == BN_OK);
        assert(bigNumShiftRight(n1, 3) == BN_OK);
        assert(bigNumShiftRight(n1, 2) == BN_OK);
        assert(bigNumShiftRight(n1, 1) == BN_OK);

        assert(bigNumSetHex(n1, "111111") == BN_OK);
        assert(bigNumToStrHex(n1, numStr, 1000) == BN_OK);
        printf ("Num str = %s\n", numStr);
        assert(bigNumToStrBin(n1, numStr, 1000) == BN_OK);
        printf ("Num str = %s\n", numStr);

        assert(bigNumShiftRight(n1, 6) == BN_OK);
        assert(bigNumShiftRight(n1, 3) == BN_OK);
        assert(bigNumShiftRight(n1, 2) == BN_OK);
        assert(bigNumShiftRight(n1, 1) == BN_OK);

        assert(bigNumShiftLeft(n1, 7) == BN_OK);
        assert(bigNumShiftLeft(n1, 8) == BN_OK);
        assert(bigNumShiftLeft(n1, 9) == BN_OK);

        assert(bigNumSetHex(n1, "1") == BN_OK);
        assert(bigNumToStrHex(n1, numStr, 1000) == BN_OK);
        printf ("Num str = %s\n", numStr);
        assert(bigNumToStrBin(n1, numStr, 1000) == BN_OK);
        printf ("Num str = %s\n", numStr);

        assert(bigNumShiftLeft(n1, 24) == BN_OK);
        assert(bigNumShiftLeft(n1, 31) == BN_OK);
        assert(bigNumShiftLeft(n1, 32) == BN_OK);
        assert(bigNumShiftLeft(n1, 33) == BN_OK);

        assert(bigNumSetHex(n1, "2") == BN_OK);
        assert(bigNumToStrHex(n1, numStr, 1000) == BN_OK);
        printf ("Num str = %s\n", numStr);
        assert(bigNumToStrBin(n1, numStr, 1000) == BN_OK);
        printf ("Num str = %s\n", numStr);

        assert(bigNumShiftLeft(n1, 1) == BN_OK);
        assert(bigNumShiftLeft(n1, 2) == BN_OK);
        assert(bigNumShiftLeft(n1, 3) == BN_OK);
        assert(bigNumShiftLeft(n1, 6) == BN_OK);
        assert(bigNumShiftLeft(n1, 7) == BN_OK);
        assert(bigNumShiftLeft(n1, 8) == BN_OK);
        assert(bigNumShiftLeft(n1, 9) == BN_OK);

        assert(bigNumShiftLeft(n1, 24) == BN_OK);
        assert(bigNumShiftLeft(n1, 31) == BN_OK);
        assert(bigNumShiftLeft(n1, 32) == BN_OK);
        assert(bigNumShiftLeft(n1, 33) == BN_OK);

        assert(bigNumSetHex(n1, "3") == BN_OK);
        assert(bigNumShiftLeft(n1, 1) == BN_OK);
        assert(bigNumShiftLeft(n1, 2) == BN_OK);
        assert(bigNumShiftLeft(n1, 3) == BN_OK);
        assert(bigNumShiftLeft(n1, 6) == BN_OK);
        assert(bigNumShiftLeft(n1, 7) == BN_OK);
        assert(bigNumShiftLeft(n1, 8) == BN_OK);
        assert(bigNumShiftLeft(n1, 9) == BN_OK);

        assert(bigNumShiftLeft(n1, 24) == BN_OK);
        assert(bigNumShiftLeft(n1, 31) == BN_OK);
        assert(bigNumShiftLeft(n1, 32) == BN_OK);
        assert(bigNumShiftLeft(n1, 33) == BN_OK);

        assert(bigNumSetHex(n1, "FE") == BN_OK);
        assert(bigNumToStrHex(n1, numStr, 1000) == BN_OK);
        printf ("Num str = %s\n", numStr);
        assert(bigNumToStrBin(n1, numStr, 1000) == BN_OK);
        printf ("Num str = %s\n", numStr);

        assert(bigNumShiftLeft(n1, 1) == BN_OK);
        assert(bigNumShiftLeft(n1, 2) == BN_OK);
        assert(bigNumShiftLeft(n1, 3) == BN_OK);

        assert(bigNumShiftRight(n1, 1) == BN_OK);
        assert(bigNumShiftRight(n1, 2) == BN_OK);
        assert(bigNumShiftRight(n1, 3) == BN_OK);

        assert(bigNumShiftLeft(n1, 6) == BN_OK);
        assert(bigNumShiftLeft(n1, 7) == BN_OK);
        assert(bigNumShiftLeft(n1, 8) == BN_OK);
        assert(bigNumShiftLeft(n1, 9) == BN_OK);

        assert(bigNumShiftLeft(n1, 24) == BN_OK);
        assert(bigNumShiftLeft(n1, 31) == BN_OK);
        assert(bigNumShiftLeft(n1, 32) == BN_OK);
        assert(bigNumShiftLeft(n1, 33) == BN_OK);

        assert(bigNumSetHex(n1, "FF") == BN_OK);
        assert(bigNumShiftLeft(n1, 1) == BN_OK);
        assert(bigNumShiftLeft(n1, 2) == BN_OK);
        assert(bigNumShiftLeft(n1, 3) == BN_OK);
        assert(bigNumShiftLeft(n1, 6) == BN_OK);
        assert(bigNumShiftLeft(n1, 7) == BN_OK);
        assert(bigNumShiftLeft(n1, 8) == BN_OK);
        assert(bigNumShiftLeft(n1, 9) == BN_OK);

        assert(bigNumShiftLeft(n1, 24) == BN_OK);
        assert(bigNumShiftLeft(n1, 31) == BN_OK);
        assert(bigNumShiftLeft(n1, 32) == BN_OK);
        assert(bigNumShiftLeft(n1, 33) == BN_OK);

        assert(bigNumSetHex(n2, NULL) == BN_ERR_INVALID_VALUE);
        assert(bigNumSetHex(n2, "") == BN_ERR_INVALID_VALUE);
        assert(bigNumSetHex(n2, "0") == BN_OK);
        assert(bigNumSetHex(n2, "1") == BN_OK);
        assert(bigNumSetHex(n2, "2") == BN_OK);
        assert(bigNumSetHex(n2, "9") == BN_OK);
        assert(bigNumSetHex(n2, "a") == BN_OK);
        assert(bigNumSetHex(n2, "f") == BN_OK);
        assert(bigNumSetHex(n2, "F") == BN_OK);
        assert(bigNumSetHex(n2, "g") == BN_ERR_INVALID_VALUE);
        assert(bigNumSetHex(n2, "A") == BN_OK);
        assert(bigNumSetHex(n2, "01") == BN_OK);
        assert(bigNumSetHex(n2, "20") == BN_OK);
        assert(bigNumSetHex(n2, "9W") == BN_ERR_INVALID_VALUE);
        assert(bigNumSetHex(n2, "aA") == BN_OK);
        assert(bigNumSetHex(n2, "fF") == BN_OK);
        assert(bigNumSetHex(n2, "39W") == BN_ERR_INVALID_VALUE);
        assert(bigNumSetHex(n2, "3aA") == BN_OK);
        assert(bigNumSetHex(n2, "44fF") == BN_OK);
        assert(bigNumSetHex(n2, "3456A") == BN_OK);
        bigNumPrint(n2, 16 | BN_FLAG_PRINT_0X | BN_FLAG_PRINT_SPACE);
        printf("\n---------------------------\n");

        assert(bigNumSetHex(n2, "68844DCBD44DDCBD") == BN_ERR_OVERFLOW);
        assert(bigNumSetHex(n2,
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF") == BN_ERR_OVERFLOW);

        assert(bigNumSetInt(n1, 1) == BN_OK);
        bigNumPrint(n1, BN_FLAG_PRINT_0X);
        printf("\n---------------------------\n");

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

        assert(bigNumSetHex(n2, "10") == BN_OK);
        assert(bigNumSetBin(n3, "1000") == BN_OK);
        assert(bigNumToStrHex(n3, numStr, 1000) == BN_OK);
        printf ("Num str = %s\n", numStr);
        assert(bigNumToStrBin(n3, numStr, 1000) == BN_OK);
        printf ("Num str = %s\n", numStr);

        bigNumPrint(n3,
                    BN_FLAG_PRINT_SPACE | BN_FLAG_PRINT_0X |
                            BN_FLAG_PRINT_BREAK);

        assert(bigNumSet(n4, n3) == BN_OK);

        assert(bigNumSetInt(n1, 16) == BN_OK);
        assert(bigNumCmpInt(n1, 16) == 0);
        assert(bigNumCmpHex(n1, "10") == 0);
        assert(bigNumCmpBin(n1, "10000") == 0);

        assert(bigNumCmp(n3, n4) == 0);

        assert(bigNumSetHex(n2, "A3") == BN_OK);
        assert(bigNumSetBin(n3, "10001") == BN_OK);
        bigNumPrint(n3,
                    BN_FLAG_PRINT_SPACE | BN_FLAG_PRINT_0X |
                            BN_FLAG_PRINT_BREAK);

        assert(bigNumSetInt(n2, 163) == 0);
        assert(bigNumSetInt(n3, 17) == 0);

        assert(bigNumInit(48) == BN_ERR_ALREADY_INIT);
        assert(bigNumInit(72) == BN_ERR_ALREADY_INIT);

        assert(bigNumSetHex(n2, "1g") == BN_ERR_INVALID_VALUE);
        assert(bigNumSetBin(n3, "1020") == BN_ERR_INVALID_VALUE);

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

        assert(bigNumCmpInt(n1, 0) == 0);

        assert(bigNumSetInt(n1, 838493355) == BN_OK);
        assert(bigNumToStrHex(n1, numStr, 1000) == BN_OK);
        printf ("Num str = %s\n", numStr);
        assert(bigNumToStrBin(n1, numStr, 1000) == BN_OK);
        printf ("Num str = %s\n", numStr);

        assert(bigNumCmpInt(n1, 838493355) == 0);
        assert(bigNumCmpBin(n1, "00110001111110100110010010101011") == 0);
        assert(bigNumCmpHex(n1, "31FA64AB") == 0);

        assert(bigNumCmpInt(n1, 938493355) == -1);
        assert(bigNumCmpBin(n1, "001100011111101001100100101010111") == -1);
        assert(bigNumCmpHex(n1, "31FA64AB1") == -1);

        assert(bigNumCmpInt(n1, 83849335) == 1);
        assert(bigNumCmpBin(n1, "0011000111111010011001001010101") == 1);
        assert(bigNumCmpHex(n1, "31FA64A") == 1);

        assert(bigNumCmpHex(n1, "31FA 64AB") == BN_ERR_INVALID_VALUE);

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

        assert(bigNumSetHex(n2, "10") == BN_OK);
        assert(bigNumSetBin(n3, "100011111111111111000") == BN_OK);
        bigNumPrint(n3,
                    BN_FLAG_PRINT_SPACE | BN_FLAG_PRINT_0X |
                            BN_FLAG_PRINT_BREAK);

        assert(bigNumSet(n4, n3) == BN_OK);

        assert(bigNumCmpInt(n1, 838493355) == 0);
        assert(bigNumCmpInt(n1, 8) == 1);

        assert(bigNumSetHex(n2, "A3") == BN_OK);
        assert(bigNumSetBin(n3, "111111111111110001") == BN_OK);
        assert(bigNumCmpInt(n3, 262129) == 0);
        bigNumPrint(n3,
                    BN_FLAG_PRINT_SPACE | BN_FLAG_PRINT_0X |
                            BN_FLAG_PRINT_BREAK);

        assert(bigNumSetInt(n2, 163) == 0);
        assert(bigNumSetInt(n3, 17) == 0);

        assert(bigNumInit(32) == BN_ERR_ALREADY_INIT);
        assert(bigNumInit(64) == BN_ERR_ALREADY_INIT);

        assert(bigNumSetHex(n2, "1g") == BN_ERR_INVALID_VALUE);
        assert(bigNumSetBin(n3, "1020") == BN_ERR_INVALID_VALUE);

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

        assert(bigNumCmpInt(n1, 0) == 0);
        assert(bigNumCmpHex(n2, "0") == 0);
        assert(bigNumCmpBin(n3, "0") == 0);
        assert(bigNumCmp(n3, n4) == 0);

        assert(bigNumSetInt(n1, 83849) == BN_OK);

        assert(bigNumSetHex(n2, "10") == BN_OK);
        assert(bigNumSetBin(n3, "000010011110") == BN_OK);
        bigNumPrint(n3,
                    BN_FLAG_PRINT_SPACE | BN_FLAG_PRINT_0X |
                            BN_FLAG_PRINT_BREAK);

        assert(bigNumSet(n4, n3) == BN_OK);

        assert(bigNumCmpInt(n1, 83849) == 0);
        assert(bigNumCmpInt(n1, 16) == 1);
        assert(bigNumCmpInt(n1, 883849) == -1);

        assert(bigNumCmp(n3, n4) == 0);

        assert(bigNumSetHex(n2, "A3") == BN_OK);
        assert(bigNumSetBin(n3, "10001") == BN_OK);

        assert(bigNumSetInt(n2, 163) == 0);
        assert(bigNumSetInt(n3, 17) == 0);

        assert(bigNumInit(32) == BN_ERR_ALREADY_INIT);
        assert(bigNumInit(64) == BN_ERR_ALREADY_INIT);

        assert(bigNumSetHex(n2, "1g") == BN_ERR_INVALID_VALUE);
        assert(bigNumSetBin(n3, "1020") == BN_ERR_INVALID_VALUE);
        bigNumPrint(n3,
                    BN_FLAG_PRINT_SPACE | BN_FLAG_PRINT_0X |
                            BN_FLAG_PRINT_BREAK);

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

    err = bigNumInit(32000);
    if (BN_OK == err)
    {
        bignum n1 = bigNumNew();
        bignum n2 = bigNumNew();
        bignum n3 = bigNumNew();
        bignum n4 = bigNumNew();

        assert(bigNumCmpInt(n1, 0) == 0);
        assert(bigNumShiftLeft(n1, 0) == BN_ERR_INVALID_VALUE);
        assert(bigNumShiftLeft(n1, 1) == BN_OK);
        assert(bigNumShiftLeft(n1, 2) == BN_OK);
        assert(bigNumShiftLeft(n1, 3) == BN_OK);
        assert(bigNumShiftLeft(n1, 6) == BN_OK);
        assert(bigNumShiftLeft(n1, 7) == BN_OK);
        assert(bigNumShiftLeft(n1, 8) == BN_OK);
        assert(bigNumShiftLeft(n1, 9) == BN_OK);

        assert(bigNumShiftLeft(n1, 24) == BN_OK);
        assert(bigNumShiftLeft(n1, 31) == BN_OK);
        assert(bigNumShiftLeft(n1, 32) == BN_OK);
        assert(bigNumShiftLeft(n1, 33) == BN_OK);

        assert(bigNumSetHex(n1, "0") == BN_OK);
        assert(bigNumShiftLeft(n1, 1) == BN_OK);
        assert(bigNumShiftLeft(n1, 2) == BN_OK);
        assert(bigNumShiftLeft(n1, 3) == BN_OK);
        assert(bigNumShiftLeft(n1, 6) == BN_OK);
        assert(bigNumShiftLeft(n1, 7) == BN_OK);
        assert(bigNumShiftLeft(n1, 8) == BN_OK);
        assert(bigNumShiftLeft(n1, 9) == BN_OK);

        assert(bigNumShiftLeft(n1, 24) == BN_OK);
        assert(bigNumShiftLeft(n1, 31) == BN_OK);
        assert(bigNumShiftLeft(n1, 32) == BN_OK);
        assert(bigNumShiftLeft(n1, 33) == BN_OK);

        assert(bigNumSetHex(n1, "1") == BN_OK);
        assert(bigNumShiftLeft(n1, 1) == BN_OK);
        assert(bigNumShiftLeft(n1, 2) == BN_OK);
        assert(bigNumShiftLeft(n1, 3) == BN_OK);
        assert(bigNumShiftLeft(n1, 6) == BN_OK);
        assert(bigNumShiftLeft(n1, 7) == BN_OK);
        assert(bigNumShiftLeft(n1, 8) == BN_OK);
        assert(bigNumShiftLeft(n1, 9) == BN_OK);

        assert(bigNumSetHex(n1, "1") == BN_OK);
        assert(bigNumShiftLeft(n1, 24) == BN_OK);
        assert(bigNumShiftLeft(n1, 31) == BN_OK);
        assert(bigNumShiftLeft(n1, 32) == BN_OK);
        assert(bigNumShiftLeft(n1, 33) == BN_OK);

        assert(bigNumSetHex(n1, "2") == BN_OK);
        assert(bigNumShiftLeft(n1, 1) == BN_OK);
        assert(bigNumShiftLeft(n1, 2) == BN_OK);
        assert(bigNumShiftLeft(n1, 3) == BN_OK);
        assert(bigNumShiftLeft(n1, 6) == BN_OK);
        assert(bigNumShiftLeft(n1, 7) == BN_OK);
        assert(bigNumShiftLeft(n1, 8) == BN_OK);
        assert(bigNumShiftLeft(n1, 9) == BN_OK);

        assert(bigNumShiftLeft(n1, 24) == BN_OK);
        assert(bigNumShiftLeft(n1, 31) == BN_OK);
        assert(bigNumShiftLeft(n1, 32) == BN_OK);
        assert(bigNumShiftLeft(n1, 33) == BN_OK);

        assert(bigNumSetHex(n1, "3") == BN_OK);
        assert(bigNumShiftLeft(n1, 1) == BN_OK);
        assert(bigNumShiftLeft(n1, 2) == BN_OK);
        assert(bigNumShiftLeft(n1, 3) == BN_OK);
        assert(bigNumShiftLeft(n1, 6) == BN_OK);
        assert(bigNumShiftLeft(n1, 7) == BN_OK);
        assert(bigNumShiftLeft(n1, 8) == BN_OK);
        assert(bigNumShiftLeft(n1, 9) == BN_OK);

        assert(bigNumShiftLeft(n1, 24) == BN_OK);
        assert(bigNumShiftLeft(n1, 31) == BN_OK);
        assert(bigNumShiftLeft(n1, 32) == BN_OK);
        assert(bigNumShiftLeft(n1, 33) == BN_OK);

        assert(bigNumSetHex(n1, "FE") == BN_OK);
        assert(bigNumShiftLeft(n1, 1) == BN_OK);
        assert(bigNumShiftLeft(n1, 2) == BN_OK);
        assert(bigNumShiftLeft(n1, 3) == BN_OK);
        assert(bigNumShiftLeft(n1, 6) == BN_OK);
        assert(bigNumShiftLeft(n1, 7) == BN_OK);
        assert(bigNumShiftLeft(n1, 8) == BN_OK);
        assert(bigNumShiftLeft(n1, 9) == BN_OK);

        assert(bigNumShiftLeft(n1, 24) == BN_OK);
        assert(bigNumShiftLeft(n1, 31) == BN_OK);
        assert(bigNumShiftLeft(n1, 32) == BN_OK);
        assert(bigNumShiftLeft(n1, 33) == BN_OK);

        assert(bigNumSetHex(n1, "FF") == BN_OK);
        assert(bigNumShiftLeft(n1, 1) == BN_OK);
        assert(bigNumShiftLeft(n1, 2) == BN_OK);
        assert(bigNumShiftLeft(n1, 3) == BN_OK);
        assert(bigNumShiftLeft(n1, 6) == BN_OK);
        assert(bigNumShiftLeft(n1, 7) == BN_OK);
        assert(bigNumShiftLeft(n1, 8) == BN_OK);
        assert(bigNumShiftLeft(n1, 9) == BN_OK);

        assert(bigNumSetHex(n1, "234122562367FF") == BN_OK);
        assert(bigNumSet(n2, n1) == BN_OK);
        assert(bigNumShiftLeft(n1, 4) == BN_OK);
        assert(bigNumShiftRight(n1, 4) == BN_OK);
        assert(bigNumCmp(n1, n2) == 0);

        assert(bigNumShiftLeft(n1, 7) == BN_OK);
        assert(bigNumShiftRight(n1, 7) == BN_OK);
        assert(bigNumCmp(n1, n2) == 0);

        assert(bigNumShiftLeft(n1, 8) == BN_OK);
        assert(bigNumShiftRight(n1, 8) == BN_OK);
        assert(bigNumCmp(n1, n2) == 0);

        assert(bigNumShiftLeft(n1, 9) == BN_OK);
        assert(bigNumShiftRight(n1, 9) == BN_OK);
        assert(bigNumCmp(n1, n2) == 0);

        assert(bigNumShiftLeft(n1, 13) == BN_OK);
        assert(bigNumShiftRight(n1, 13) == BN_OK);
        assert(bigNumCmp(n1, n2) == 0);

        assert(bigNumShiftLeft(n1, 34) == BN_OK);
        assert(bigNumShiftRight(n1, 34) == BN_OK);
        assert(bigNumCmp(n1, n2) == 0);

        assert(bigNumShiftLeft(n1, 99) == BN_OK);
        assert(bigNumShiftRight(n1, 99) == BN_OK);
        assert(bigNumCmp(n1, n2) == 0);

        assert(bigNumShiftLeft(n1, 133) == BN_OK);
        assert(bigNumShiftRight(n1, 133) == BN_OK);
        assert(bigNumCmp(n1, n2) == 0);

        assert(bigNumShiftLeft(n1, 30000) == BN_OK);
        assert(bigNumShiftRight(n1, 30000) == BN_OK);
        assert(bigNumCmp(n1, n2) == 0);

        assert(bigNumShiftLeft(n1, 1) == BN_OK);
        assert(bigNumShiftLeft(n1, 2) == BN_OK);
        assert(bigNumShiftLeft(n1, 3) == BN_OK);
        assert(bigNumShiftLeft(n1, 6) == BN_OK);
        assert(bigNumShiftLeft(n1, 7) == BN_OK);
        assert(bigNumShiftLeft(n1, 8) == BN_OK);
        assert(bigNumShiftLeft(n1, 9) == BN_OK);

        assert(bigNumShiftLeft(n1, 24) == BN_OK);
        assert(bigNumShiftLeft(n1, 31) == BN_OK);
        assert(bigNumShiftLeft(n1, 32) == BN_OK);
        assert(bigNumShiftLeft(n1, 33) == BN_OK);

        assert(bigNumSetHex(n2, "8844DCBD44DDC") == BN_OK);
        bigNumPrint(n2, 16 | BN_FLAG_PRINT_0X | BN_FLAG_PRINT_SPACE);
        printf("\n---------------------------\n");

        assert(bigNumSetHex(n2,
                            "564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF"
                            "AA4564564646464656456444fF") == BN_OK);
        bigNumPrint(n2, 16 | BN_FLAG_PRINT_0X | BN_FLAG_PRINT_SPACE);
        printf("\n---------------------------\n");

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

        assert(bigNumSetHex(n2, "10") == BN_OK);
        assert(bigNumSetBin(n3,
                            "10111111111111111111111111111100000000000000000000"
                            "00000111111111111111111111000000000000000000000001"
                            "11111111111111111111111100") == BN_OK);
        bigNumPrint(n3,
                    BN_FLAG_PRINT_SPACE | BN_FLAG_PRINT_0X |
                            BN_FLAG_PRINT_BREAK);
        printf("\n---------------------------\n");

        assert(bigNumSet(n4, n3) == BN_OK);

        assert(bigNumCmp(n3, n4) == 0);

        assert(bigNumSetHex(n2, "A3") == BN_OK);
        assert(bigNumCmpInt(n2, 163) == 0);

        assert(bigNumSetBin(n3, "10001") == BN_OK);
        assert(bigNumCmpInt(n3, 17) == 0);

        assert(bigNumSetInt(n2, 163) == 0);
        assert(bigNumCmpInt(n2, 163) == 0);
        assert(bigNumSetInt(n3, 17) == 0);

        assert(bigNumInit(32) == BN_ERR_ALREADY_INIT);
        assert(bigNumInit(64) == BN_ERR_ALREADY_INIT);

        assert(bigNumSetHex(n2, "1g") == BN_ERR_INVALID_VALUE);
        assert(bigNumSetBin(n3, "1020") == BN_ERR_INVALID_VALUE);

        n1 = bigNumNew();
        err = bigNumLastError();
        assert(BN_OK == err);

        assert(BN_OK == bigNumRand(n1));
        bigNumPrint(n1,
                    24 | BN_FLAG_PRINT_SPACE | BN_FLAG_PRINT_0X |
                            BN_FLAG_PRINT_BREAK);
        printf("\n---------------------------\n");

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