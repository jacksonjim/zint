/* plessey.c - Handles Plessey and MSI Plessey */
/*
    libzint - the open source barcode library
    Copyright (C) 2008-2025 Robin Stuart <rstuart114@gmail.com>

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

#include <stdio.h>
#include "common.h"

#define SSET_F  (IS_NUM_F | IS_UHX_F) /* SSET "0123456789ABCDEF" */

static const char PlessTable[16][8] = {
    {'1','3','1','3','1','3','1','3'}, {'3','1','1','3','1','3','1','3'}, {'1','3','3','1','1','3','1','3'},
    {'3','1','3','1','1','3','1','3'}, {'1','3','1','3','3','1','1','3'}, {'3','1','1','3','3','1','1','3'},
    {'1','3','3','1','3','1','1','3'}, {'3','1','3','1','3','1','1','3'}, {'1','3','1','3','1','3','3','1'},
    {'3','1','1','3','1','3','3','1'}, {'1','3','3','1','1','3','3','1'}, {'3','1','3','1','1','3','3','1'},
    {'1','3','1','3','3','1','3','1'}, {'3','1','1','3','3','1','3','1'}, {'1','3','3','1','3','1','3','1'},
    {'3','1','3','1','3','1','3','1'}
};

static const char MSITable[10][8] = {
    {'1','2','1','2','1','2','1','2'}, {'1','2','1','2','1','2','2','1'}, {'1','2','1','2','2','1','1','2'},
    {'1','2','1','2','2','1','2','1'}, {'1','2','2','1','1','2','1','2'}, {'1','2','2','1','1','2','2','1'},
    {'1','2','2','1','2','1','1','2'}, {'1','2','2','1','2','1','2','1'}, {'2','1','1','2','1','2','1','2'},
    {'2','1','1','2','1','2','2','1'}
};

/* Not MSI/Plessey but the older Plessey standard */
INTERNAL int plessey(struct zint_symbol *symbol, unsigned char source[], int length) {

    int i;
    unsigned char checkptr[67 * 4 + 8] = {0};
    static const char grid[9] = {1, 1, 1, 1, 0, 1, 0, 0, 1};
    char dest[570]; /* 8 + 67 * 8 + 2 * 8 + 9 + 1 = 570 */
    char *d = dest;
    unsigned int check_digits = 0;
    char c1, c2;
    int error_number = 0;
    const int raw_text = symbol->output_options & BARCODE_RAW_TEXT;

    if (length > 67) { /* 16 + 67 * 16 + 4 * 8 + 19 = 1139 */
        return errtxtf(ZINT_ERROR_TOO_LONG, symbol, 370, "Input length %d too long (maximum 67)", length);
    }
    if ((i = not_sane(SSET_F, source, length))) {
        return errtxtf(ZINT_ERROR_INVALID_DATA, symbol, 371,
                        "Invalid character at position %d in input (digits and \"ABCDEF\" only)", i);
    }

    /* Start character */
    memcpy(d, "31311331", 8);
    d += 8;

    /* Data area */
    for (i = 0; i < length; i++, d += 8) {
        const unsigned int check = source[i] - '0' - (source[i] >> 6) * 7;
        memcpy(d, PlessTable[check], 8);
        checkptr[4 * i] = check & 1;
        checkptr[4 * i + 1] = (check >> 1) & 1;
        checkptr[4 * i + 2] = (check >> 2) & 1;
        checkptr[4 * i + 3] = (check >> 3) & 1;
    }

    /* CRC check digit code adapted from code by Leonid A. Broukhis
       used in GNU Barcode */

    for (i = 0; i < (4 * length); i++) {
        if (checkptr[i]) {
            int j;
            for (j = 0; j < 9; j++)
                checkptr[i + j] ^= grid[j];
        }
    }

    for (i = 0; i < 8; i++) {
        switch (checkptr[length * 4 + i]) {
            case 0:
                memcpy(d, "13", 2);
                d += 2;
                break;
            case 1:
                memcpy(d, "31", 2);
                d += 2;
                check_digits |= (1 << i);
                break;
        }
    }

    /* Stop character */
    memcpy(d, "331311313", 9);
    d += 9;

    expand(symbol, dest, d - dest);

    /* TODO: Find documentation on BARCODE_PLESSEY dimensions/height */

    c1 = (char) xtoc(check_digits & 0xF);
    c2 = (char) xtoc(check_digits >> 4);

    hrt_cpy_nochk(symbol, source, length);
    if (symbol->option_2 == 1) {
        hrt_cat_chr_nochk(symbol, c1);
        hrt_cat_chr_nochk(symbol, c2);
    }

    if (raw_text && rt_printf_256(symbol, "%.*s%c%c", length, source, c1, c2)) {
        return ZINT_ERROR_MEMORY; /* `rt_printf_256()` only fails with OOM */
    }

    return error_number;
}

/* Modulo 10 check digit - Luhn algorithm
   See https://en.wikipedia.org/wiki/Luhn_algorithm */
static char msi_check_digit_mod10(const unsigned char source[], const int length) {
    static const char vals[2][10] = {
        { 0, 2, 4, 6, 8, 1, 3, 5, 7, 9 }, /* Doubled and digits summed */
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, /* Single */
    };
    int i, x = 0, undoubled = 0;

    for (i = length - 1; i >= 0; i--) {
        /* Note overflow impossible for max length 92 * max weight 9 * max val 15 == 12420 */
        x += vals[undoubled][ctoi(source[i])];
        undoubled = !undoubled;
    }

    return itoc((10 - x % 10) % 10);
}

/* Modulo 11 check digit - IBM weight system wrap = 7, NCR system wrap = 9
   See https://en.wikipedia.org/wiki/MSI_Barcode */
static char msi_check_digit_mod11(const unsigned char source[], const int length, const int wrap) {
    int i, x = 0, weight = 2;

    for (i = length - 1; i >= 0; i--) {
        /* Note overflow impossible for max length 92 * max weight 9 * max val 15 == 12420 */
        x += weight * ctoi(source[i]);
        weight++;
        if (weight > wrap) {
            weight = 2;
        }
    }

    return itoc((11 - x % 11) % 11); /* Will return ':' for 10 */
}

/* Plain MSI Plessey - does not calculate any check character */
static char *msi_plessey_nomod(struct zint_symbol *symbol, const unsigned char source[], const int length,
            const int raw_text, char *d) {

    int i;

    for (i = 0; i < length; i++, d += 8) {
        memcpy(d, MSITable[source[i] - '0'], 8);
    }

    hrt_cpy_nochk(symbol, source, length);

    if (raw_text && rt_cpy(symbol, source, length)) {
        return NULL; /* `rt_cpy()` only fails with OOM */
    }

    return d;
}

/* MSI Plessey with Modulo 10 check digit */
static char *msi_plessey_mod10(struct zint_symbol *symbol, const unsigned char source[], const int length,
            const int no_checktext, const int raw_text, char *d) {
    int i;
    char check_digit;

    /* Draw data section */
    for (i = 0; i < length; i++, d += 8) {
        memcpy(d, MSITable[source[i] - '0'], 8);
    }

    /* Calculate check digit */
    check_digit = msi_check_digit_mod10(source, length);

    /* Draw check digit */
    memcpy(d, MSITable[check_digit - '0'], 8);
    d += 8;

    hrt_cpy_nochk(symbol, source, length);
    if (!no_checktext) {
        hrt_cat_chr_nochk(symbol, check_digit);
    }

    if (raw_text && rt_cpy_cat(symbol, source, length, check_digit, NULL /*cat*/, 0)) {
        return NULL; /* `rt_cpy_cat()` only fails with OOM */
    }

    return d;
}

/* MSI Plessey with two Modulo 10 check digits */
static char *msi_plessey_mod1010(struct zint_symbol *symbol, const unsigned char source[], const int length,
            const int no_checktext, const int raw_text, char *d) {
    int i;
    unsigned char local_source[92 + 2];

    /* Append check digits */
    memcpy(local_source, source, length);
    local_source[length] = msi_check_digit_mod10(source, length);
    local_source[length + 1] = msi_check_digit_mod10(local_source, length + 1);

    /* Draw data section */
    for (i = 0; i < length + 2; i++, d += 8) {
        memcpy(d, MSITable[local_source[i] - '0'], 8);
    }

    if (no_checktext) {
        hrt_cpy_nochk(symbol, source, length);
    } else {
        hrt_cpy_nochk(symbol, local_source, length + 2);
    }

    if (raw_text && rt_cpy(symbol, local_source, length + 2)) {
        return NULL; /* `rt_cpy()` only fails with OOM */
    }
    return d;
}

/* MSI Plessey with Modulo 11 check digit */
static char *msi_plessey_mod11(struct zint_symbol *symbol, const unsigned char source[], const int length,
            const int no_checktext, const int wrap, const int raw_text, char *d) {
    /* Uses the IBM weight system if wrap = 7, and the NCR system if wrap = 9 */
    int i;
    unsigned char check_digits[2];
    int check_digits_len = 1;

    /* Draw data section */
    for (i = 0; i < length; i++, d += 8) {
        memcpy(d, MSITable[source[i] - '0'], 8);
    }

    /* Append check digit(s) */
    check_digits[0] = msi_check_digit_mod11(source, length, wrap);
    if (check_digits[0] == ':') {
        check_digits[0] = '1';
        check_digits[1] = '0';
        check_digits_len++;
    }
    for (i = 0; i < check_digits_len; i++, d += 8) {
        memcpy(d, MSITable[check_digits[i] - '0'], 8);
    }

    hrt_cpy_nochk(symbol, source, length);
    if (!no_checktext) {
        hrt_cat_nochk(symbol, check_digits, check_digits_len);
    }

    if (raw_text && rt_cpy_cat(symbol, source, length, '\xFF' /*separator (none)*/, check_digits, check_digits_len)) {
        return NULL; /* `check_digits_len()` only fails with OOM */
    }
    return d;
}

/* MSI Plessey with Modulo 11 check digit and Modulo 10 check digit */
static char *msi_plessey_mod1110(struct zint_symbol *symbol, const unsigned char source[], const int length,
            const int no_checktext, const int wrap, const int raw_text, char *d) {
    /* Uses the IBM weight system if wrap = 7, and the NCR system if wrap = 9 */
    int i;
    char check_digit;
    unsigned char local_source[92 + 3 + 1];
    int local_length = length;

    memcpy(local_source, source, length);

    /* Append first (mod 11) digit */
    check_digit = msi_check_digit_mod11(source, length, wrap);
    if (check_digit == ':') {
        local_source[local_length++] = '1';
        local_source[local_length++] = '0';
    } else {
        local_source[local_length++] = check_digit;
    }

    /* Append second (mod 10) check digit */
    local_source[local_length] = msi_check_digit_mod10(local_source, local_length);
    local_length++;

    /* Draw data section */
    for (i = 0; i < local_length; i++, d += 8) {
        memcpy(d, MSITable[local_source[i] - '0'], 8);
    }

    if (no_checktext) {
        hrt_cpy_nochk(symbol, source, length);
    } else {
        hrt_cpy_nochk(symbol, local_source, local_length);
    }

    if (raw_text && rt_cpy(symbol, local_source, local_length)) {
        return NULL; /* `rt_cpy()` only fails with OOM */
    }

    return d;
}

INTERNAL int msi_plessey(struct zint_symbol *symbol, unsigned char source[], int length) {
    int error_number = 0;
    int i;
    char dest[766]; /* 2 + 92 * 8 + 3 * 8 + 3 + 1 = 766 */
    char *d = dest;
    int check_option = symbol->option_2;
    int no_checktext = 0;
    const int raw_text = symbol->output_options & BARCODE_RAW_TEXT;

    if (length > 92) { /* 3 (Start) + 92 * 12 + 3 * 12 + 4 (Stop) = 1147 */
        return errtxtf(ZINT_ERROR_TOO_LONG, symbol, 372, "Input length %d too long (maximum 92)", length);
    }
    if ((i = not_sane(NEON_F, source, length))) {
        return errtxtf(ZINT_ERROR_INVALID_DATA, symbol, 377,
                        "Invalid character at position %d in input (digits only)", i);
    }

    if (check_option >= 11 && check_option <= 16) { /* +10 means don't print check digits in HRT */
        check_option -= 10;
        no_checktext = 1;
    }
    if ((check_option < 0) || (check_option > 6)) {
        check_option = 0;
    }

    /* Start character */
    memcpy(d, "21", 2);
    d += 2;

    switch (check_option) {
        case 0: d = msi_plessey_nomod(symbol, source, length, raw_text, d); break;
        case 1: d = msi_plessey_mod10(symbol, source, length, no_checktext, raw_text, d); break;
        case 2: d = msi_plessey_mod1010(symbol, source, length, no_checktext, raw_text, d); break;
        case 3: d = msi_plessey_mod11(symbol, source, length, no_checktext, 7 /*IBM wrap*/, raw_text, d); break;
        case 4: d = msi_plessey_mod1110(symbol, source, length, no_checktext, 7 /*IBM wrap*/, raw_text, d); break;
        case 5: d = msi_plessey_mod11(symbol, source, length, no_checktext, 9 /*NCR wrap*/, raw_text, d); break;
        case 6: d = msi_plessey_mod1110(symbol, source, length, no_checktext, 9 /*NCR wrap*/, raw_text, d); break;
    }

    if (!d) {
        return ZINT_ERROR_MEMORY; /* `rt_cpy()` etc. only fail with OOM */
    }

    /* Stop character */
    memcpy(d, "121", 3);
    d += 3;

    expand(symbol, dest, d - dest);

    /* TODO: Find documentation on BARCODE_MSI_PLESSEY dimensions/height */

    return error_number;
}

/* vim: set ts=4 sw=4 et : */
