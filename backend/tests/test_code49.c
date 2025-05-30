/*
    libzint - the open source barcode library
    Copyright (C) 2020-2025 Robin Stuart <rstuart114@gmail.com>

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the project nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
 */
/* SPDX-License-Identifier: BSD-3-Clause */

#include "testcommon.h"

static void test_large(const testCtx *const p_ctx) {
    int debug = p_ctx->debug;

    struct item {
        const char *pattern;
        int length;
        int ret;
        int expected_rows;
        int expected_width;
        const char *expected_errtxt;
    };
    /* s/\/\*[ 0-9]*\*\//\=printf("\/\*%3d*\/", line(".") - line("'<")): */
    static const struct item data[] = {
        /*  0*/ { "A", 49, 0, 8, 70, "" }, /* ANSI/AIM BC6-2000 Table 1 */
        /*  1*/ { "A", 50, ZINT_ERROR_TOO_LONG, -1, -1, "Error 432: Input too long, requires 50 codewords (maximum 49)" },
        /*  2*/ { "0", 81, 0, 8, 70, "" }, /* ANSI/AIM BC6-2000 Table 1 */
        /*  3*/ { "0", 82, ZINT_ERROR_TOO_LONG, -1, -1, "Error 430: Input length 82 too long (maximum 81)" },
    };
    const int data_size = ARRAY_SIZE(data);
    int i, length, ret;
    struct zint_symbol *symbol = NULL;

    char data_buf[4096];

    testStartSymbol(p_ctx->func_name, &symbol);

    for (i = 0; i < data_size; i++) {

        if (testContinue(p_ctx, i)) continue;

        symbol = ZBarcode_Create();
        assert_nonnull(symbol, "Symbol not created\n");

        testUtilStrCpyRepeat(data_buf, data[i].pattern, data[i].length);
        assert_equal(data[i].length, (int) strlen(data_buf), "i:%d length %d != strlen(data_buf) %d\n", i, data[i].length, (int) strlen(data_buf));

        length = testUtilSetSymbol(symbol, BARCODE_CODE49, -1 /*input_mode*/, -1 /*eci*/, -1 /*option_1*/, -1, -1, -1 /*output_options*/, data_buf, data[i].length, debug);

        ret = ZBarcode_Encode(symbol, TCU(data_buf), length);
        assert_equal(ret, data[i].ret, "i:%d ZBarcode_Encode ret %d != %d (%s)\n", i, ret, data[i].ret, symbol->errtxt);
        assert_equal(symbol->errtxt[0] == '\0', ret == 0, "i:%d symbol->errtxt not %s (%s)\n", i, ret ? "set" : "empty", symbol->errtxt);
        assert_zero(strcmp(symbol->errtxt, data[i].expected_errtxt), "i:%d strcmp(%s, %s) != 0\n", i, symbol->errtxt, data[i].expected_errtxt);

        if (ret < ZINT_ERROR) {
            assert_equal(symbol->rows, data[i].expected_rows, "i:%d symbol->rows %d != %d\n", i, symbol->rows, data[i].expected_rows);
            assert_equal(symbol->width, data[i].expected_width, "i:%d symbol->width %d != %d\n", i, symbol->width, data[i].expected_width);
        }

        ZBarcode_Delete(symbol);
    }

    testFinish();
}

static void test_input(const testCtx *const p_ctx) {
    int debug = p_ctx->debug;

    struct item {
        int input_mode;
        int output_options;
        int option_1;
        int option_3;
        const char *data;
        int length;
        int ret;
        int expected_rows;
        int expected_width;
        const char *expected;
        int expected_option_1;
        int expected_option_3;
        const char *comment;
    };
    /*
       NUL U+0000, S1 SP (39)
       US U+001F (\037, 31), S1 5
    */
    static const struct item data[] = {
        /*  0*/ { UNICODE_MODE, -1, -1, 0, "é", -1, ZINT_ERROR_INVALID_DATA, 0, 0, "Error 431: Invalid character at position 1 in input, extended ASCII not allowed", -1, 0, "ASCII only" },
        /*  1*/ { UNICODE_MODE, -1, -1, 0, "EXAMPLE 2", -1, 0, 2, 70, "(16) 14 33 10 22 25 21 14 41 38 2 35 14 18 13 0 22", 2, 0, "2.3.7 Symbol Example" },
        /*  2*/ { UNICODE_MODE, -1, -1, 0, "12345", -1, 0, 2, 70, "(16) 5 17 9 48 48 48 48 27 48 48 13 23 0 13 2 0", 2, 0, "2.3 Example 1: Numeric Encodation (Start 2, Numeric)" },
        /*  3*/ { UNICODE_MODE, -1, -1, 0, "123456", -1, 0, 2, 70, "(16) 5 17 9 6 48 48 48 34 48 48 36 9 23 41 2 11", 2, 0, "2.3 Example 1: Numeric Encodation" },
        /*  4*/ { UNICODE_MODE, -1, -1, 0, "12345678", -1, 0, 2, 70, "(16) 5 17 9 14 6 48 48 0 48 48 25 42 2 17 2 37", 2, 0, "2.3 Example 1: Numeric Encodation" },
        /*  5*/ { UNICODE_MODE, -1, -1, 0, "123456789", -1, 0, 2, 70, "(16) 5 17 9 46 16 37 48 31 48 48 7 26 9 39 2 32", 2, 0, "2.3 Example 1: Numeric Encodation" },
        /*  6*/ { UNICODE_MODE, -1, -1, 0, "1234567", -1, 0, 2, 70, "(16) 43 45 2 11 39 48 48 40 48 48 33 36 38 6 2 15", 2, 0, "2.3 Example 1: Numeric Encodation" },
        /*  7*/ { UNICODE_MODE, -1, -1, 0, "\037", -1, 0, 2, 70, "(16) 5 48 48 48 48 48 48 48 48 48 4 33 13 15 4 18", 2, 0, "US (Start 4, Alphanumeric S1)" },
        /*  8*/ { UNICODE_MODE, -1, -1, 0, "\000\037", 2, 0, 2, 70, "(16) 38 43 5 48 48 48 48 33 48 48 45 7 38 43 4 37", 2, 0, "NUL S1 US (Start 4, Alphanumeric S1)" },
        /*  9*/ { UNICODE_MODE, -1, -1, 0, "a\000", 2, 0, 2, 70, "(16) 10 43 38 48 48 48 48 38 48 48 32 33 14 15 5 48", 2, 0, "a S1 NUL (Start 5, Alphanumeric S2)" },
        /* 10*/ { UNICODE_MODE, -1, -1, 0, "ab", -1, 0, 2, 70, "(16) 10 44 11 48 48 48 48 12 48 48 27 39 42 0 5 13", 2, 0, "a S2 b (Start 5, Alphanumeric S2)" },
        /* 11*/ { UNICODE_MODE, -1, -1, 0, "\000A\000a\000", 5, 0, 2, 70, "(16) 38 10 43 38 44 10 43 30 38 48 25 23 38 32 4 12", 2, 0, "NUL A S1 NUL S2 a S1 (C18 30) NUL (Start 4, Alphanumeric S1)" },
        /* 12*/ { UNICODE_MODE, -1, -1, 0, "1234\037aA12345A", -1, 0, 3, 70, "(24) 1 2 3 4 43 5 44 4 10 10 48 5 17 9 48 0 10 48 19 2 13 32 7 33", 3, 0, "1 2 3 4 S1 US S2 (C18 4) a A NS 12345 NS (C28 0) A (Start 0, Alpha)" },
        /* 13*/ { GS1_MODE, -1, -1, 0, "[90]12345[91]AB12345", -1, 0, 4, 70, "(32) 45 48 47 15 4 7 9 28 48 45 9 1 10 11 48 25 5 17 9 48 48 48 48 27 48 48 37 39 26 8 14", 4, 0, "FNC1 NS 9012345 (C18 28) NS FNC1 9 1 A B NS (C28 25) 12345 Pad (4) (C38 27) (Start 0, Alpha)" },
        /* 14*/ { GS1_MODE | GS1PARENS_MODE, -1, -1, 0, "(90)12345(91)AB12345", -1, 0, 4, 70, "(32) 45 48 47 15 4 7 9 28 48 45 9 1 10 11 48 25 5 17 9 48 48 48 48 27 48 48 37 39 26 8 14", 4, 0, "FNC1 NS 9012345 (C18 28) NS FNC1 9 1 A B NS (C28 25) 12345 Pad (4) (C38 27) (Start 0, Alpha)" },
        /* 15*/ { GS1_MODE, -1, -1, 0, "[90](90)", -1, 0, 2, 70, "(16) 45 9 0 43 36 9 0 44 43 37 39 24 44 25 0 16", 2, 0, "" },
        /* 16*/ { GS1_MODE | ESCAPE_MODE | GS1PARENS_MODE, -1, -1, 0, "(90)\\(90\\)", -1, 0, 2, 70, "(16) 45 9 0 43 36 9 0 44 43 37 39 24 44 25 0 16", 2, 0, "" },
        /* 17*/ { UNICODE_MODE, -1, -1, 0, "1234567890123456789012345678901234567890", -1, 0, 5, 70, "(40) 5 17 9 29 22 18 5 7 17 9 29 22 18 5 17 19 9 29 22 18 5 17 9 11 29 22 18 48 48 48 48 16", 5, 0, "" },
        /* 18*/ { UNICODE_MODE, -1, 1, 0, "1234567890123456789012345678901234567890", -1, ZINT_ERROR_INVALID_OPTION, -1, -1, "Error 433: Minimum number of rows out of range (2 to 8)", 1, 0, "" },
        /* 19*/ { UNICODE_MODE, -1, 9, 0, "1234567890123456789012345678901234567890", -1, ZINT_ERROR_INVALID_OPTION, -1, -1, "Error 433: Minimum number of rows out of range (2 to 8)", 9, 0, "" },
        /* 20*/ { UNICODE_MODE, -1, 2, 0, "1234567890123456789012345678901234567890", -1, 0, 5, 70, "(40) 5 17 9 29 22 18 5 7 17 9 29 22 18 5 17 19 9 29 22 18 5 17 9 11 29 22 18 48 48 48 48 16", 5, 0, "" },
        /* 21*/ { UNICODE_MODE, -1, 3, 0, "1234567890123456789012345678901234567890", -1, 0, 5, 70, "(40) 5 17 9 29 22 18 5 7 17 9 29 22 18 5 17 19 9 29 22 18 5 17 9 11 29 22 18 48 48 48 48 16", 5, 0, "" },
        /* 22*/ { UNICODE_MODE, -1, 4, 0, "1234567890123456789012345678901234567890", -1, 0, 5, 70, "(40) 5 17 9 29 22 18 5 7 17 9 29 22 18 5 17 19 9 29 22 18 5 17 9 11 29 22 18 48 48 48 48 16", 5, 0, "" },
        /* 23*/ { UNICODE_MODE, -1, 5, 0, "1234567890123456789012345678901234567890", -1, 0, 5, 70, "(40) 5 17 9 29 22 18 5 7 17 9 29 22 18 5 17 19 9 29 22 18 5 17 9 11 29 22 18 48 48 48 48 16", 5, 0, "" },
        /* 24*/ { UNICODE_MODE, -1, 6, 0, "1234567890123456789012345678901234567890", -1, 0, 6, 70, "(48) 5 17 9 29 22 18 5 7 17 9 29 22 18 5 17 19 9 29 22 18 5 17 9 11 29 22 18 48 48 48 48 16", 6, 0, "" },
        /* 25*/ { UNICODE_MODE, -1, 7, 0, "1234567890123456789012345678901234567890", -1, 0, 7, 70, "(56) 5 17 9 29 22 18 5 7 17 9 29 22 18 5 17 19 9 29 22 18 5 17 9 11 29 22 18 48 48 48 48 16", 7, 0, "" },
        /* 26*/ { UNICODE_MODE, -1, 8, 0, "1234567890123456789012345678901234567890", -1, 0, 8, 70, "(64) 5 17 9 29 22 18 5 7 17 9 29 22 18 5 17 19 9 29 22 18 5 17 9 11 29 22 18 48 48 48 48 16", 8, 0, "" },
        /* 27*/ { UNICODE_MODE, -1, -1, 0, "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVW", -1, 0, 8, 70, "(64) 10 11 12 13 14 15 16 42 17 18 19 20 21 22 23 42 24 25 26 27 28 29 30 42 31 32 33 34 35", 8, 0, "" },
        /* 28*/ { UNICODE_MODE, -1, -1, 0, "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWX", -1, ZINT_ERROR_TOO_LONG, 0, 0, "Error 432: Input too long, requires 50 codewords (maximum 49)", -1, 0, "" },
        /* 29*/ { UNICODE_MODE, COMPLIANT_HEIGHT, -1, 4, "12345", -1, 0, 2, 70, "(16) 5 17 9 48 48 48 48 27 48 48 13 23 0 13 2 0", 2, 4, "option_3 separator" },
        /* 30*/ { UNICODE_MODE, COMPLIANT_HEIGHT, -1, 5, "12345", -1, 0, 2, 70, "(16) 5 17 9 48 48 48 48 27 48 48 13 23 0 13 2 0", 2, 1, "option_3 invalid 5 -> 1" },
        /* 31*/ { UNICODE_MODE, -1, -1, 5, "12345", -1, 0, 2, 70, "(16) 5 17 9 48 48 48 48 27 48 48 13 23 0 13 2 0", 2, 5, "option_3 invalid 5 ignored unless COMPLIANT_HEIGHT" },
    };
    const int data_size = ARRAY_SIZE(data);
    int i, length, ret;
    struct zint_symbol *symbol = NULL;

    char escaped[1024];
    char cmp_buf[8192];
    char cmp_msg[1024];

    int do_bwipp = (debug & ZINT_DEBUG_TEST_BWIPP) && testUtilHaveGhostscript(); /* Only do BWIPP test if asked, too slow otherwise */

    testStartSymbol(p_ctx->func_name, &symbol);

    for (i = 0; i < data_size; i++) {

        if (testContinue(p_ctx, i)) continue;

        symbol = ZBarcode_Create();
        assert_nonnull(symbol, "Symbol not created\n");

        symbol->debug = ZINT_DEBUG_TEST; /* Needed to get codeword dump in errtxt */

        length = testUtilSetSymbol(symbol, BARCODE_CODE49, data[i].input_mode, -1 /*eci*/,
                                    data[i].option_1, -1 /*option_2*/, data[i].option_3, data[i].output_options,
                                    data[i].data, data[i].length, debug);

        ret = ZBarcode_Encode(symbol, TCU(data[i].data), length);
        assert_equal(ret, data[i].ret, "i:%d ZBarcode_Encode ret %d != %d (%s)\n", i, ret, data[i].ret, symbol->errtxt);

        if (p_ctx->generate) {
            printf("        /*%3d*/ { %s, %s, %d, %d, \"%s\", %d, %s, %d, %d, \"%s\", %d, %d, \"%s\" },\n",
                    i, testUtilInputModeName(data[i].input_mode), testUtilOutputOptionsName(data[i].output_options),
                    data[i].option_1, data[i].option_3,
                    testUtilEscape(data[i].data, length, escaped, sizeof(escaped)), data[i].length,
                    testUtilErrorName(data[i].ret), symbol->rows, symbol->width,
                    symbol->errtxt, symbol->option_1, symbol->option_3, data[i].comment);
        } else {
            assert_zero(strcmp((char *) symbol->errtxt, data[i].expected), "i:%d strcmp(%s, %s) != 0\n", i, symbol->errtxt, data[i].expected);
            if (ret < ZINT_ERROR) {
                assert_equal(symbol->rows, data[i].expected_rows, "i:%d symbol->rows %d != %d (%s)\n", i, symbol->rows, data[i].expected_rows, data[i].data);
                assert_equal(symbol->width, data[i].expected_width, "i:%d symbol->width %d != %d (%s)\n", i, symbol->width, data[i].expected_width, data[i].data);

                if (do_bwipp && testUtilCanBwipp(i, symbol, data[i].option_1, -1, -1, debug)) {
                    char modules_dump[4096];
                    assert_notequal(testUtilModulesDump(symbol, modules_dump, sizeof(modules_dump)), -1, "i:%d testUtilModulesDump == -1\n", i);
                    ret = testUtilBwipp(i, symbol, data[i].option_1, -1, -1, data[i].data, length, NULL, cmp_buf, sizeof(cmp_buf), NULL);
                    assert_zero(ret, "i:%d %s testUtilBwipp ret %d != 0\n", i, testUtilBarcodeName(symbol->symbology), ret);

                    ret = testUtilBwippCmp(symbol, cmp_msg, cmp_buf, modules_dump);
                    assert_zero(ret, "i:%d %s testUtilBwippCmp %d != 0 %s\n  actual: %s\nexpected: %s\n",
                                   i, testUtilBarcodeName(symbol->symbology), ret, cmp_msg, cmp_buf, modules_dump);
                }
            }
            assert_equal(symbol->option_1, data[i].expected_option_1, "i:%d symbol->option_1 %d != %d\n",
                        i, symbol->option_1, data[i].expected_option_1);
            assert_zero(symbol->option_2, "i:%d symbol->option_2 %d != 0\n", i, symbol->option_2);
            assert_equal(symbol->option_3, data[i].expected_option_3, "i:%d symbol->option_3 %d != %d\n",
                        i, symbol->option_3, data[i].expected_option_3);
        }

        ZBarcode_Delete(symbol);
    }

    testFinish();
}

static void test_encode(const testCtx *const p_ctx) {
    int debug = p_ctx->debug;

    struct item {
        int input_mode;
        int option_1;
        const char *data;
        int ret;

        int expected_rows;
        int expected_width;
        const char *comment;
        const char *expected;
    };
    static const struct item data[] = {
        /*  0*/ { UNICODE_MODE, -1, "MULTIPLE ROWS IN CODE 49", 0, 5, 70, "ANSI/AIM BC6-2000 Figure 1",
                    "1011111011001011101011100110000110111101011011111010111101000100001111"
                    "1010100001000010001001111000101110100110001111010010001011100011001111"
                    "1011001100000101101101110111000010110010110000111011101011110001101111"
                    "1010011001100100001111010010001100101011101111110011010001001111101111"
                    "1011001111001011101000000101001110111110111010001011010001101111101111"
                },
        /*  1*/ { UNICODE_MODE, -1, "EXAMPLE 2", 0, 2, 70, "ANSI/AIM BC6-2000 Figure 3",
                    "1011000111011100101111001001000110110011110010100010001111000100101111"
                    "1011000100110010001100010110010000100001101001111010000001001011101111"
                },
        /*  2*/ { UNICODE_MODE, 3, "EXAMPLE 2", 0, 3, 70, "Min 3 rows",
                    "1011000111011100101111001001000110110011110010100010001111000100101111"
                    "1011000100110010001010111011111100110011110010111010111011001111101111"
                    "1011001111001011101110011111001010100001000010001010111001111001101111"
                },
        /*  3*/ { UNICODE_MODE, 8, "EXAMPLE 2", 0, 8, 70, "Min 8 rows",
                    "1011000111011100101111001001000110110011110010100010001111000100101111"
                    "1011000100110010001010111011111100110011110010111010111011001111101111"
                    "1010101110111111001010111011111100110011110010111011001110110001001111"
                    "1011001111001011101100111100101110101011101111110010111001000001101111"
                    "1010101110111111001100111100101110101011101111110011001110110001001111"
                    "1011001111001011101010111011111100101011101111110011001110110001001111"
                    "1010101110111111001010111011111100101011101111110010111001000001101111"
                    "1011110110100100001010000100010000111010010011111011001000111011001111"
                },
    };
    const int data_size = ARRAY_SIZE(data);
    int i, length, ret;
    struct zint_symbol *symbol = NULL;

    char escaped[1024];
    char bwipp_buf[8192];
    char bwipp_msg[1024];

    int do_bwipp = (debug & ZINT_DEBUG_TEST_BWIPP) && testUtilHaveGhostscript(); /* Only do BWIPP test if asked, too slow otherwise */

    testStartSymbol(p_ctx->func_name, &symbol);

    for (i = 0; i < data_size; i++) {

        if (testContinue(p_ctx, i)) continue;

        symbol = ZBarcode_Create();
        assert_nonnull(symbol, "Symbol not created\n");

        length = testUtilSetSymbol(symbol, BARCODE_CODE49, data[i].input_mode, -1 /*eci*/, data[i].option_1, -1, -1, -1 /*output_options*/, data[i].data, -1, debug);

        ret = ZBarcode_Encode(symbol, TCU(data[i].data), length);
        assert_equal(ret, data[i].ret, "i:%d ZBarcode_Encode ret %d != %d (%s)\n", i, ret, data[i].ret, symbol->errtxt);

        if (p_ctx->generate) {
            printf("        /*%3d*/ { %s, %d, \"%s\", %s, %d, %d, \"%s\",\n",
                    i, testUtilInputModeName(data[i].input_mode), data[i].option_1,
                    testUtilEscape(data[i].data, length, escaped, sizeof(escaped)),
                    testUtilErrorName(data[i].ret), symbol->rows, symbol->width, data[i].comment);
            testUtilModulesPrint(symbol, "                    ", "\n");
            printf("                },\n");
        } else {
            if (ret < ZINT_ERROR) {
                int width, row;

                assert_equal(symbol->rows, data[i].expected_rows, "i:%d symbol->rows %d != %d (%s)\n", i, symbol->rows, data[i].expected_rows, data[i].data);
                assert_equal(symbol->width, data[i].expected_width, "i:%d symbol->width %d != %d (%s)\n", i, symbol->width, data[i].expected_width, data[i].data);

                ret = testUtilModulesCmp(symbol, data[i].expected, &width, &row);
                assert_zero(ret, "i:%d testUtilModulesCmp ret %d != 0 width %d row %d (%s)\n", i, ret, width, row, data[i].data);

                if (do_bwipp && testUtilCanBwipp(i, symbol, data[i].option_1, -1, -1, debug)) {
                    ret = testUtilBwipp(i, symbol, data[i].option_1, -1, -1, data[i].data, length, NULL, bwipp_buf, sizeof(bwipp_buf), NULL);
                    assert_zero(ret, "i:%d %s testUtilBwipp ret %d != 0\n", i, testUtilBarcodeName(symbol->symbology), ret);

                    ret = testUtilBwippCmp(symbol, bwipp_msg, bwipp_buf, data[i].expected);
                    assert_zero(ret, "i:%d %s testUtilBwippCmp %d != 0 %s\n  actual: %s\nexpected: %s\n",
                                   i, testUtilBarcodeName(symbol->symbology), ret, bwipp_msg, bwipp_buf, data[i].expected);
                }
            }
        }

        ZBarcode_Delete(symbol);
    }

    testFinish();
}

static void test_rt(const testCtx *const p_ctx) {
    int debug = p_ctx->debug;

    struct item {
        int input_mode;
        int output_options;
        const char *data;
        int length;
        int ret;
        int expected_eci;
        const char *expected;
        int expected_length;
        int expected_raw_eci;
    };
    /* s/\/\*[ 0-9]*\*\//\=printf("\/\*%3d*\/", line(".") - line("'<")): */
    static const struct item data[] = {
        /*  0*/ { UNICODE_MODE, -1, "AB\000123", 6, 0, 0, "", -1, 0 },
        /*  1*/ { UNICODE_MODE, BARCODE_RAW_TEXT, "AB\000123", 6, 0, 0, "AB\000123", 6, 3 },
        /*  2*/ { DATA_MODE, -1, "AB\000123", 6, 0, 0, "", -1, 0 },
        /*  3*/ { DATA_MODE, BARCODE_RAW_TEXT, "AB\000123", 6, 0, 0, "AB\000123", 6, 3 },
        /*  4*/ { GS1_MODE, -1, "[01]04912345123459[15]970331[30]128[10]ABC123", -1, 0, 0, "", -1, 0 },
        /*  5*/ { GS1_MODE, BARCODE_RAW_TEXT, "[01]04912345123459[15]970331[30]128[10]ABC123", -1, 0, 0, "01049123451234591597033130128\03510ABC123", -1, 3 },
    };
    const int data_size = ARRAY_SIZE(data);
    int i, length, ret;
    struct zint_symbol *symbol = NULL;

    int expected_length;

    char escaped[4096];
    char escaped2[4096];

    testStartSymbol(p_ctx->func_name, &symbol);

    for (i = 0; i < data_size; i++) {

        if (testContinue(p_ctx, i)) continue;

        symbol = ZBarcode_Create();
        assert_nonnull(symbol, "Symbol not created\n");

        length = testUtilSetSymbol(symbol, BARCODE_CODE49, data[i].input_mode, -1 /*eci*/,
                                    -1 /*option_1*/, -1 /*option_2*/, -1 /*option_3*/, data[i].output_options,
                                    data[i].data, data[i].length, debug);
        expected_length = data[i].expected_length == -1 ? (int) strlen(data[i].expected) : data[i].expected_length;

        ret = ZBarcode_Encode(symbol, TCU(data[i].data), length);
        assert_equal(ret, data[i].ret, "i:%d ZBarcode_Encode ret %d != %d (%s)\n", i, ret, data[i].ret, symbol->errtxt);

        if (ret < ZINT_ERROR) {
            assert_equal(symbol->eci, data[i].expected_eci, "i:%d eci %d != %d\n",
                        i, symbol->eci, data[i].expected_eci);
            if (symbol->output_options & BARCODE_RAW_TEXT) {
                assert_nonnull(symbol->raw_segs, "i:%d raw_segs NULL\n", i);
                assert_nonnull(symbol->raw_segs[0].source, "i:%d raw_segs[0].source NULL\n", i);
                assert_equal(symbol->raw_segs[0].length, expected_length,
                            "i:%d raw_segs[0].length %d != expected_length %d\n",
                            i, symbol->raw_segs[0].length, expected_length);
                assert_zero(memcmp(symbol->raw_segs[0].source, data[i].expected, expected_length),
                            "i:%d raw_segs[0].source memcmp(%s, %s, %d) != 0\n", i,
                            testUtilEscape((const char *) symbol->raw_segs[0].source, symbol->raw_segs[0].length,
                                            escaped, sizeof(escaped)),
                            testUtilEscape(data[i].expected, expected_length, escaped2, sizeof(escaped2)),
                            expected_length);
                assert_equal(symbol->raw_segs[0].eci, data[i].expected_raw_eci,
                            "i:%d raw_segs[0].eci %d != expected_raw_eci %d\n",
                            i, symbol->raw_segs[0].eci, data[i].expected_raw_eci);
            } else {
                assert_null(symbol->raw_segs, "i:%d raw_segs not NULL\n", i);
            }
        }

        ZBarcode_Delete(symbol);
    }

    testFinish();
}

int main(int argc, char *argv[]) {

    testFunction funcs[] = { /* name, func */
        { "test_large", test_large },
        { "test_input", test_input },
        { "test_encode", test_encode },
        { "test_rt", test_rt },
    };

    testRun(argc, argv, funcs, ARRAY_SIZE(funcs));

    testReport();

    return 0;
}

/* vim: set ts=4 sw=4 et : */
