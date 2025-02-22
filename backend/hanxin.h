/*  hanxin.h - definitions for Han Xin code */
/*
    libzint - the open source barcode library
    Copyright (C) 2009-2024 Robin Stuart <rstuart114@gmail.com>
    Copyright (C) 2016 Zoe Stuart

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

#ifndef Z_HANXIN_H
#define Z_HANXIN_H

/* Data from table B1: Data capacity of Han Xin Code */
static const unsigned short hx_total_codewords[84] = {
      25,   37,   50,   54,   69,   84,  100,  117,  136,  155,
     161,  181,  203,  225,  249,  273,  299,  325,  353,  381,
     411,  422,  453,  485,  518,  552,  587,  623,  660,  698,
     737,  754,  794,  836,  878,  922,  966, 1011, 1058, 1105,
    1126, 1175, 1224, 1275, 1327, 1380, 1434, 1489, 1513, 1569,
    1628, 1686, 1745, 1805, 1867, 1929, 1992, 2021, 2086, 2151,
    2218, 2286, 2355, 2425, 2496, 2528, 2600, 2673, 2749, 2824,
    2900, 2977, 3056, 3135, 3171, 3252, 3334, 3416, 3500, 3585,
    3671, 3758, 3798, 3886
};

static const unsigned short hx_data_codewords[4][84] = { {
          21,   31,   42,   46,   57,   70,   84,   99,  114,  131,
         135,  153,  171,  189,  209,  229,  251,  273,  297,  321,
         345,  354,  381,  407,  436,  464,  493,  523,  554,  586,
         619,  634,  666,  702,  738,  774,  812,  849,  888,  929,
         946,  987, 1028, 1071, 1115, 1160, 1204, 1251, 1271, 1317,
        1368, 1416, 1465, 1517, 1569, 1621, 1674, 1697, 1752, 1807,
        1864, 1920, 1979, 2037, 2096, 2124, 2184, 2245, 2309, 2372,
        2436, 2501, 2568, 2633, 2663, 2732, 2800, 2870, 2940, 3011,
        3083, 3156, 3190, 3264
    }, {
          17,   25,   34,   38,   49,   58,   70,   81,   96,  109,
         113,  127,  143,  157,  175,  191,  209,  227,  247,  267,
         287,  296,  317,  339,  362,  386,  411,  437,  462,  488,
         515,  528,  556,  586,  614,  646,  676,  707,  740,  773,
         788,  823,  856,  893,  929,  966, 1004, 1043, 1059, 1099,
        1140, 1180, 1221, 1263, 1307, 1351, 1394, 1415, 1460, 1505,
        1552, 1600, 1649, 1697, 1748, 1770, 1820, 1871, 1925, 1976,
        2030, 2083, 2140, 2195, 2219, 2276, 2334, 2392, 2450, 2509,
        2569, 2630, 2658, 2720
    }, {
          13,   19,   26,   30,   37,   46,   54,   63,   74,   83,
          87,   97,  109,  121,  135,  147,  161,  175,  191,  205,
         221,  228,  245,  261,  280,  298,  317,  337,  358,  376,
         397,  408,  428,  452,  474,  498,  522,  545,  572,  597,
         608,  635,  660,  689,  717,  746,  774,  805,  817,  847,
         880,  910,  943,  975, 1009, 1041, 1076, 1091, 1126, 1161,
        1198, 1234, 1271, 1309, 1348, 1366, 1404, 1443, 1485, 1524,
        1566, 1607, 1650, 1693, 1713, 1756, 1800, 1844, 1890, 1935,
        1983, 2030, 2050, 2098
    }, {
           9,   15,   20,   22,   27,   34,   40,   47,   54,   61,
          65,   73,   81,   89,   99,  109,  119,  129,  141,  153,
         165,  168,  181,  195,  208,  220,  235,  251,  264,  280,
         295,  302,  318,  334,  352,  368,  386,  405,  424,  441,
         450,  469,  490,  509,  531,  552,  574,  595,  605,  627,
         652,  674,  697,  721,  747,  771,  796,  809,  834,  861,
         892,  914,  941,  969,  998, 1012, 1040, 1069, 1099, 1130,
        1160, 1191, 1222, 1253, 1269, 1300, 1334, 1366, 1400, 1433,
        1469, 1504, 1520, 1554
    }
};

/* Value 'k' from Annex A */
static const char hx_module_k[84] = {
     0,  0,  0, 14, 16, 16, 17, 18, 19, 20,
    14, 15, 16, 16, 17, 17, 18, 19, 20, 20,
    21, 16, 17, 17, 18, 18, 19, 19, 20, 20,
    21, 17, 17, 18, 18, 19, 19, 19, 20, 20,
    17, 17, 18, 18, 18, 19, 19, 19, 17, 17,
    18, 18, 18, 18, 19, 19, 19, 17, 17, 18,
    18, 18, 18, 19, 19, 17, 17, 17, 18, 18,
    18, 18, 19, 19, 17, 17, 17, 18, 18, 18,
    18, 18, 17, 17
};

/* Value 'r' from Annex A */
static const char hx_module_r[84] = {
     0,  0,  0, 15, 15, 17, 18, 19, 20, 21,
    15, 15, 15, 17, 17, 19, 19, 19, 19, 21,
    21, 17, 16, 18, 17, 19, 18, 20, 19, 21,
    20, 17, 19, 17, 19, 17, 19, 21, 19, 21,
    18, 20, 17, 19, 21, 18, 20, 22, 17, 19,
    15, 17, 19, 21, 17, 19, 21, 18, 20, 15,
    17, 19, 21, 16, 18, 17, 19, 21, 15, 17,
    19, 21, 15, 17, 18, 20, 22, 15, 17, 19,
    21, 23, 17, 19
};

/* Value of 'm' from Annex A */
static const char hx_module_m[84] = {
    0,  0,  0,  1,  1,  1,  1,  1,  1,  1,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  4,  4,  4,  4,  4,  4,  4,  4,  4,
    5,  5,  5,  5,  5,  5,  5,  5,  6,  6,
    6,  6,  6,  6,  6,  6,  6,  7,  7,  7,
    7,  7,  7,  7,  7,  8,  8,  8,  8,  8,
    8,  8,  8,  8,  9,  9,  9,  9,  9,  9,
    9,  9, 10, 10
};

/* Error correction block sizes from Table D1 */
static const unsigned char hx_table_d1[] = {
/* #blocks, k, 2t, #blocks, k, 2t, #blocks, k, 2t */
     1,   21,   4,   0,    0,   0,   0,    0,   0,  /* version 1 */
     1,   17,   8,   0,    0,   0,   0,    0,   0,
     1,   13,  12,   0,    0,   0,   0,    0,   0,
     1,    9,  16,   0,    0,   0,   0,    0,   0,
     1,   31,   6,   0,    0,   0,   0,    0,   0,  /* version 2 */
     1,   25,  12,   0,    0,   0,   0,    0,   0,
     1,   19,  18,   0,    0,   0,   0,    0,   0,
     1,   15,  22,   0,    0,   0,   0,    0,   0,
     1,   42,   8,   0,    0,   0,   0,    0,   0,  /* version 3 */
     1,   34,  16,   0,    0,   0,   0,    0,   0,
     1,   26,  24,   0,    0,   0,   0,    0,   0,
     1,   20,  30,   0,    0,   0,   0,    0,   0,
     1,   46,   8,   0,    0,   0,   0,    0,   0,  /* version 4 */
     1,   38,  16,   0,    0,   0,   0,    0,   0,
     1,   30,  24,   0,    0,   0,   0,    0,   0,
     1,   22,  32,   0,    0,   0,   0,    0,   0,
     1,   57,  12,   0,    0,   0,   0,    0,   0,  /* version 5 */
     1,   49,  20,   0,    0,   0,   0,    0,   0,
     1,   37,  32,   0,    0,   0,   0,    0,   0,
     1,   14,  20,   1,   13,  22,   0,    0,   0,
     1,   70,  14,   0,    0,   0,   0,    0,   0,  /* version 6 */
     1,   58,  26,   0,    0,   0,   0,    0,   0,
     1,   24,  20,   1,   22,  18,   0,    0,   0,
     1,   16,  24,   1,   18,  26,   0,    0,   0,
     1,   84,  16,   0,    0,   0,   0,    0,   0,  /* version 7 */
     1,   70,  30,   0,    0,   0,   0,    0,   0,
     1,   26,  22,   1,   28,  24,   0,    0,   0,
     2,   14,  20,   1,   12,  20,   0,    0,   0,
     1,   99,  18,   0,    0,   0,   0,    0,   0,  /* version 8 */
     1,   40,  18,   1,   41,  18,   0,    0,   0,
     1,   31,  26,   1,   32,  28,   0,    0,   0,
     2,   16,  24,   1,   15,  22,   0,    0,   0,
     1,  114,  22,   0,    0,   0,   0,    0,   0,  /* version 9 */
     2,   48,  20,   0,    0,   0,   0,    0,   0,
     2,   24,  20,   1,   26,  22,   0,    0,   0,
     2,   18,  28,   1,   18,  26,   0,    0,   0,
     1,  131,  24,   0,    0,   0,   0,    0,   0,  /* version 10 */
     1,   52,  22,   1,   57,  24,   0,    0,   0,
     2,   27,  24,   1,   29,  24,   0,    0,   0,
     2,   21,  32,   1,   19,  30,   0,    0,   0,
     1,  135,  26,   0,    0,   0,   0,    0,   0,  /* version 11 */
     1,   56,  24,   1,   57,  24,   0,    0,   0,
     2,   28,  24,   1,   31,  26,   0,    0,   0,
     2,   22,  32,   1,   21,  32,   0,    0,   0,
     1,  153,  28,   0,    0,   0,   0,    0,   0,  /* version 12 */
     1,   62,  26,   1,   65,  28,   0,    0,   0,
     2,   32,  28,   1,   33,  28,   0,    0,   0,
     3,   17,  26,   1,   22,  30,   0,    0,   0,
     1,   86,  16,   1,   85,  16,   0,    0,   0,  /* version 13 */
     1,   71,  30,   1,   72,  30,   0,    0,   0,
     2,   37,  32,   1,   35,  30,   0,    0,   0,
     3,   20,  30,   1,   21,  32,   0,    0,   0,
     1,   94,  18,   1,   95,  18,   0,    0,   0,  /* version 14 */
     2,   51,  22,   1,   55,  24,   0,    0,   0,
     3,   30,  26,   1,   31,  26,   0,    0,   0,
     4,   18,  28,   1,   17,  24,   0,    0,   0,
     1,  104,  20,   1,  105,  20,   0,    0,   0,  /* version 15 */
     2,   57,  24,   1,   61,  26,   0,    0,   0,
     3,   33,  28,   1,   36,  30,   0,    0,   0,
     4,   20,  30,   1,   19,  30,   0,    0,   0,
     1,  115,  22,   1,  114,  22,   0,    0,   0,  /* version 16 */
     2,   65,  28,   1,   61,  26,   0,    0,   0,
     3,   38,  32,   1,   33,  30,   0,    0,   0,
     5,   19,  28,   1,   14,  24,   0,    0,   0,
     1,  126,  24,   1,  125,  24,   0,    0,   0,  /* version 17 */
     2,   70,  30,   1,   69,  30,   0,    0,   0,
     4,   33,  28,   1,   29,  26,   0,    0,   0,
     5,   20,  30,   1,   19,  30,   0,    0,   0,
     1,  136,  26,   1,  137,  26,   0,    0,   0,  /* version 18 */
     3,   56,  24,   1,   59,  26,   0,    0,   0,
     5,   35,  30,   0,    0,   0,   0,    0,   0,
     6,   18,  28,   1,   21,  28,   0,    0,   0,
     1,  148,  28,   1,  149,  28,   0,    0,   0,  /* version 19 */
     3,   61,  26,   1,   64,  28,   0,    0,   0,
     7,   24,  20,   1,   23,  22,   0,    0,   0,
     6,   20,  30,   1,   21,  32,   0,    0,   0,
     3,  107,  20,   0,    0,   0,   0,    0,   0,  /* version 20 */
     3,   65,  28,   1,   72,  30,   0,    0,   0,
     7,   26,  22,   1,   23,  22,   0,    0,   0,
     7,   19,  28,   1,   20,  32,   0,    0,   0,
     3,  115,  22,   0,    0,   0,   0,    0,   0,  /* version 21 */
     4,   56,  24,   1,   63,  28,   0,    0,   0,
     7,   28,  24,   1,   25,  22,   0,    0,   0,
     8,   18,  28,   1,   21,  22,   0,    0,   0,
     2,  116,  22,   1,  122,  24,   0,    0,   0,  /* version 22 */
     4,   56,  24,   1,   72,  30,   0,    0,   0,
     7,   28,  24,   1,   32,  26,   0,    0,   0,
     8,   18,  28,   1,   24,  30,   0,    0,   0,
     3,  127,  24,   0,    0,   0,   0,    0,   0,  /* version 23 */
     5,   51,  22,   1,   62,  26,   0,    0,   0,
     7,   30,  26,   1,   35,  26,   0,    0,   0,
     8,   20,  30,   1,   21,  32,   0,    0,   0,
     2,  135,  26,   1,  137,  26,   0,    0,   0,  /* version 24 */
     5,   56,  24,   1,   59,  26,   0,    0,   0,
     7,   33,  28,   1,   30,  28,   0,    0,   0,
    11,   16,  24,   1,   19,  26,   0,    0,   0,
     3,  105,  20,   1,  121,  22,   0,    0,   0,  /* version 25 */
     5,   61,  26,   1,   57,  26,   0,    0,   0,
     9,   28,  24,   1,   28,  22,   0,    0,   0,
    10,   19,  28,   1,   18,  30,   0,    0,   0,
     2,  157,  30,   1,  150,  28,   0,    0,   0,  /* version 26 */
     5,   65,  28,   1,   61,  26,   0,    0,   0,
     8,   33,  28,   1,   34,  30,   0,    0,   0,
    10,   19,  28,   2,   15,  26,   0,    0,   0,
     3,  126,  24,   1,  115,  22,   0,    0,   0,  /* version 27 */
     7,   51,  22,   1,   54,  22,   0,    0,   0,
     8,   35,  30,   1,   37,  30,   0,    0,   0,
    15,   15,  22,   1,   10,  22,   0,    0,   0,
     4,  105,  20,   1,  103,  20,   0,    0,   0,  /* version 28 */
     7,   56,  24,   1,   45,  18,   0,    0,   0,
    10,   31,  26,   1,   27,  26,   0,    0,   0,
    10,   17,  26,   3,   20,  28,   1,   21,  28,
     3,  139,  26,   1,  137,  28,   0,    0,   0,  /* version 29 */
     6,   66,  28,   1,   66,  30,   0,    0,   0,
     9,   36,  30,   1,   34,  32,   0,    0,   0,
    13,   19,  28,   1,   17,  32,   0,    0,   0,
     6,   84,  16,   1,   82,  16,   0,    0,   0,  /* version 30 */
     6,   70,  30,   1,   68,  30,   0,    0,   0,
     7,   35,  30,   3,   33,  28,   1,   32,  28,
    13,   20,  30,   1,   20,  28,   0,    0,   0,
     5,  105,  20,   1,   94,  18,   0,    0,   0,  /* version 31 */
     6,   74,  32,   1,   71,  30,   0,    0,   0,
    11,   33,  28,   1,   34,  32,   0,    0,   0,
    13,   19,  28,   3,   16,  26,   0,    0,   0,
     4,  127,  24,   1,  126,  24,   0,    0,   0,  /* version 32 */
     7,   66,  28,   1,   66,  30,   0,    0,   0,
    12,   30,  24,   1,   24,  28,   1,   24,  30,
    15,   19,  28,   1,   17,  32,   0,    0,   0,
     7,   84,  16,   1,   78,  16,   0,    0,   0,  /* version 33 */
     7,   70,  30,   1,   66,  28,   0,    0,   0,
    12,   33,  28,   1,   32,  30,   0,    0,   0,
    14,   21,  32,   1,   24,  28,   0,    0,   0,
     5,  117,  22,   1,  117,  24,   0,    0,   0,  /* version 34 */
     8,   66,  28,   1,   58,  26,   0,    0,   0,
    11,   38,  32,   1,   34,  32,   0,    0,   0,
    15,   20,  30,   2,   17,  26,   0,    0,   0,
     4,  148,  28,   1,  146,  28,   0,    0,   0,  /* version 35 */
     8,   68,  30,   1,   70,  24,   0,    0,   0,
    10,   36,  32,   3,   38,  28,   0,    0,   0,
    16,   19,  28,   3,   16,  26,   0,    0,   0,
     4,  126,  24,   2,  135,  26,   0,    0,   0,  /* version 36 */
     8,   70,  28,   2,   43,  26,   0,    0,   0,
    13,   32,  28,   2,   41,  30,   0,    0,   0,
    17,   19,  28,   3,   15,  26,   0,    0,   0,
     5,  136,  26,   1,  132,  24,   0,    0,   0,  /* version 37 */
     5,   67,  30,   4,   68,  28,   1,   69,  28,
    14,   35,  30,   1,   32,  24,   0,    0,   0,
    18,   18,  26,   3,   16,  28,   1,   14,  28,
     3,  142,  26,   3,  141,  28,   0,    0,   0,  /* version 38 */
     8,   70,  30,   1,   73,  32,   1,   74,  32,
    12,   34,  30,   3,   34,  26,   1,   35,  28,
    18,   21,  32,   1,   27,  30,   0,    0,   0,
     5,  116,  22,   2,  103,  20,   1,  102,  20,  /* version 39 */
     9,   74,  32,   1,   74,  30,   0,    0,   0,
    14,   34,  28,   2,   32,  32,   1,   32,  30,
    19,   21,  32,   1,   25,  26,   0,    0,   0,
     7,  116,  22,   1,  117,  22,   0,    0,   0,  /* version 40 */
    11,   65,  28,   1,   58,  24,   0,    0,   0,
    15,   38,  32,   1,   27,  28,   0,    0,   0,
    20,   20,  30,   1,   20,  32,   1,   21,  32,
     6,  136,  26,   1,  130,  24,   0,    0,   0,  /* version 41 */
    11,   66,  28,   1,   62,  30,   0,    0,   0,
    14,   34,  28,   3,   34,  32,   1,   30,  30,
    18,   20,  30,   3,   20,  28,   2,   15,  26,
     5,  105,  20,   2,  115,  22,   2,  116,  22,  /* version 42 */
    10,   75,  32,   1,   73,  32,   0,    0,   0,
    16,   38,  32,   1,   27,  28,   0,    0,   0,
    22,   19,  28,   2,   16,  30,   1,   19,  30,
     6,  147,  28,   1,  146,  28,   0,    0,   0,  /* version 43 */
    11,   66,  28,   2,   65,  30,   0,    0,   0,
    18,   33,  28,   2,   33,  30,   0,    0,   0,
    22,   21,  32,   1,   28,  30,   0,    0,   0,
     6,  116,  22,   3,  125,  24,   0,    0,   0,  /* version 44 */
    11,   75,  32,   1,   68,  30,   0,    0,   0,
    13,   35,  28,   6,   34,  32,   1,   30,  30,
    23,   21,  32,   1,   26,  30,   0,    0,   0,
     7,  105,  20,   4,   95,  18,   0,    0,   0,  /* version 45 */
    12,   67,  28,   1,   63,  30,   1,   62,  32,
    21,   31,  26,   2,   33,  32,   0,    0,   0,
    23,   21,  32,   2,   24,  30,   0,    0,   0,
    10,  116,  22,   0,    0,   0,   0,    0,   0,  /* version 46 */
    12,   74,  32,   1,   78,  30,   0,    0,   0,
    18,   37,  32,   1,   39,  30,   1,   41,  28,
    25,   21,  32,   1,   27,  28,   0,    0,   0,
     5,  126,  24,   4,  115,  22,   1,  114,  22,  /* version 47 */
    12,   67,  28,   2,   66,  32,   1,   68,  30,
    21,   35,  30,   1,   39,  30,   0,    0,   0,
    26,   21,  32,   1,   28,  28,   0,    0,   0,
     9,  126,  24,   1,  117,  22,   0,    0,   0,  /* version 48 */
    13,   75,  32,   1,   68,  30,   0,    0,   0,
    20,   35,  30,   3,   35,  28,   0,    0,   0,
    27,   21,  32,   1,   28,  30,   0,    0,   0,
     9,  126,  24,   1,  137,  26,   0,    0,   0,  /* version 49 */
    13,   71,  30,   2,   68,  32,   0,    0,   0,
    20,   37,  32,   1,   39,  28,   1,   38,  28,
    24,   20,  32,   5,   25,  28,   0,    0,   0,
     8,  147,  28,   1,  141,  28,   0,    0,   0,  /* version 50 */
    10,   73,  32,   4,   74,  30,   1,   73,  30,
    16,   36,  32,   6,   39,  30,   1,   37,  30,
    27,   21,  32,   3,   20,  26,   0,    0,   0,
     9,  137,  26,   1,  135,  26,   0,    0,   0,  /* version 51 */
    12,   70,  30,   4,   75,  32,   0,    0,   0,
    24,   35,  30,   1,   40,  28,   0,    0,   0,
    23,   20,  32,   8,   24,  30,   0,    0,   0,
    14,   95,  18,   1,   86,  18,   0,    0,   0,  /* version 52 */
    13,   73,  32,   3,   77,  30,   0,    0,   0,
    24,   35,  30,   2,   35,  28,   0,    0,   0,
    26,   21,  32,   5,   21,  30,   1,   23,  30,
     9,  147,  28,   1,  142,  28,   0,    0,   0,  /* version 53 */
    10,   73,  30,   6,   70,  32,   1,   71,  32,
    25,   35,  30,   2,   34,  26,   0,    0,   0,
    29,   21,  32,   4,   22,  30,   0,    0,   0,
    11,  126,  24,   1,  131,  24,   0,    0,   0,  /* version 54 */
    16,   74,  32,   1,   79,  30,   0,    0,   0,
    25,   38,  32,   1,   25,  30,   0,    0,   0,
    33,   21,  32,   1,   28,  28,   0,    0,   0,
    14,  105,  20,   1,   99,  18,   0,    0,   0,  /* version 55 */
    19,   65,  28,   1,   72,  28,   0,    0,   0,
    24,   37,  32,   2,   40,  30,   1,   41,  30,
    31,   21,  32,   4,   24,  32,   0,    0,   0,
    10,  147,  28,   1,  151,  28,   0,    0,   0,  /* version 56 */
    15,   71,  30,   3,   71,  32,   1,   73,  32,
    24,   37,  32,   3,   38,  30,   1,   39,  30,
    36,   19,  30,   3,   29,  26,   0,    0,   0,
    15,  105,  20,   1,   99,  18,   0,    0,   0,  /* version 57 */
    19,   70,  30,   1,   64,  28,   0,    0,   0,
    27,   38,  32,   2,   25,  26,   0,    0,   0,
    38,   20,  30,   2,   18,  28,   0,    0,   0,
    14,  105,  20,   1,  113,  22,   1,  114,  22,  /* version 58 */
    17,   67,  30,   3,   92,  32,   0,    0,   0,
    30,   35,  30,   1,   41,  30,   0,    0,   0,
    36,   21,  32,   1,   26,  30,   1,   27,  30,
    11,  146,  28,   1,  146,  26,   0,    0,   0,  /* version 59 */
    20,   70,  30,   1,   60,  26,   0,    0,   0,
    29,   38,  32,   1,   24,  32,   0,    0,   0,
    40,   20,  30,   2,   17,  26,   0,    0,   0,
     3,  137,  26,   1,  136,  26,  10,  126,  24,  /* version 60 */
    22,   65,  28,   1,   75,  30,   0,    0,   0,
    30,   37,  32,   1,   51,  30,   0,    0,   0,
    42,   20,  30,   1,   21,  30,   0,    0,   0,
    12,  126,  24,   2,  118,  22,   1,  116,  22,  /* version 61 */
    19,   74,  32,   1,   74,  30,   1,   72,  28,
    30,   38,  32,   2,   29,  30,   0,    0,   0,
    39,   20,  32,   2,   37,  26,   1,   38,  26,
    12,  126,  24,   3,  136,  26,   0,    0,   0,  /* version 62 */
    21,   70,  30,   2,   65,  28,   0,    0,   0,
    34,   35,  30,   1,   44,  32,   0,    0,   0,
    42,   20,  30,   2,   19,  28,   2,   18,  28,
    12,  126,  24,   3,  117,  22,   1,  116,  22,  /* version 63 */
    25,   61,  26,   2,   62,  28,   0,    0,   0,
    34,   35,  30,   1,   40,  32,   1,   41,  32,
    45,   20,  30,   1,   20,  32,   1,   21,  32,
    15,  105,  20,   2,  115,  22,   2,  116,  22,  /* version 64 */
    25,   65,  28,   1,   72,  28,   0,    0,   0,
    18,   35,  30,  17,   37,  32,   1,   50,  32,
    42,   20,  30,   6,   19,  28,   1,   15,  28,
    19,  105,  20,   1,  101,  20,   0,    0,   0,  /* version 65 */
    33,   51,  22,   1,   65,  22,   0,    0,   0,
    40,   33,  28,   1,   28,  28,   0,    0,   0,
    49,   20,  30,   1,   18,  28,   0,    0,   0,
    18,  105,  20,   2,  117,  22,   0,    0,   0,  /* version 66 */
    26,   65,  28,   1,   80,  30,   0,    0,   0,
    35,   35,  30,   3,   35,  28,   1,   36,  28,
    52,   18,  28,   2,   38,  30,   0,    0,   0,
    26,   84,  16,   0,    0,   0,   0,    0,   0,  /* version 67 */
    26,   70,  30,   0,    0,   0,   0,    0,   0,
    45,   31,  26,   1,    9,  26,   0,    0,   0,
    52,   20,  30,   0,    0,   0,   0,    0,   0,
    16,  126,  24,   1,  114,  22,   1,  115,  22,  /* version 68 */
    23,   70,  30,   3,   65,  28,   1,   66,  28,
    40,   35,  30,   1,   43,  30,   0,    0,   0,
    46,   20,  30,   7,   19,  28,   1,   16,  28,
    19,  116,  22,   1,  105,  22,   0,    0,   0,  /* version 69 */
    20,   70,  30,   7,   66,  28,   1,   63,  28,
    40,   35,  30,   1,   42,  32,   1,   43,  32,
    54,   20,  30,   1,   19,  30,   0,    0,   0,
    17,  126,  24,   2,  115,  22,   0,    0,   0,  /* version 70 */
    24,   70,  30,   4,   74,  32,   0,    0,   0,
    48,   31,  26,   2,   18,  26,   0,    0,   0,
    54,   19,  28,   6,   15,  26,   1,   14,  26,
    29,   84,  16,   0,    0,   0,   0,    0,   0,  /* version 71 */
    29,   70,  30,   0,    0,   0,   0,    0,   0,
     6,   34,  30,   3,   36,  30,  38,   33,  28,
    58,   20,  30,   0,    0,   0,   0,    0,   0,
    16,  147,  28,   1,  149,  28,   0,    0,   0,  /* version 72 */
    31,   66,  28,   1,   37,  26,   0,    0,   0,
    48,   33,  28,   1,   23,  26,   0,    0,   0,
    53,   20,  30,   6,   19,  28,   1,   17,  28,
    20,  115,  22,   2,  134,  24,   0,    0,   0,  /* version 73 */
    29,   66,  28,   2,   56,  26,   2,   57,  26,
    45,   36,  30,   2,   15,  28,   0,    0,   0,
    59,   20,  30,   2,   21,  32,   0,    0,   0,
    17,  147,  28,   1,  134,  26,   0,    0,   0,  /* version 74 */
    26,   70,  30,   5,   75,  32,   0,    0,   0,
    47,   35,  30,   1,   48,  32,   0,    0,   0,
    64,   18,  28,   2,   33,  30,   1,   35,  30,
    22,  115,  22,   1,  133,  24,   0,    0,   0,  /* version 75 */
    33,   65,  28,   1,   74,  28,   0,    0,   0,
    43,   36,  30,   5,   27,  28,   1,   30,  28,
    57,   20,  30,   5,   21,  32,   1,   24,  32,
    18,  136,  26,   2,  142,  26,   0,    0,   0,  /* version 76 */
    33,   66,  28,   2,   49,  26,   0,    0,   0,
    48,   35,  30,   2,   38,  28,   0,    0,   0,
    64,   20,  30,   1,   20,  32,   0,    0,   0,
    19,  126,  24,   2,  135,  26,   1,  136,  26,  /* version 77 */
    32,   66,  28,   2,   55,  26,   2,   56,  26,
    49,   36,  30,   2,   18,  32,   0,    0,   0,
    65,   18,  28,   5,   27,  30,   1,   29,  30,
    20,  137,  26,   1,  130,  26,   0,    0,   0,  /* version 78 */
    30,   75,  32,   2,   71,  32,   0,    0,   0,
    46,   35,  30,   6,   39,  32,   0,    0,   0,
     3,   12,  30,  70,   19,  28,   0,    0,   0,
    20,  147,  28,   0,    0,   0,   0,    0,   0,  /* version 79 */
    35,   70,  30,   0,    0,   0,   0,    0,   0,
    49,   35,  30,   5,   35,  28,   0,    0,   0,
    70,   20,  30,   0,    0,   0,   0,    0,   0,
    21,  136,  26,   1,  155,  28,   0,    0,   0,  /* version 80 */
    34,   70,  30,   1,   64,  28,   1,   65,  28,
    54,   35,  30,   1,   45,  30,   0,    0,   0,
    68,   20,  30,   3,   18,  28,   1,   19,  28,
    19,  126,  24,   5,  115,  22,   1,  114,  22,  /* version 81 */
    33,   70,  30,   3,   65,  28,   1,   64,  28,
    52,   35,  30,   3,   41,  32,   1,   40,  32,
    67,   20,  30,   5,   21,  32,   1,   24,  32,
     2,  150,  28,  21,  136,  26,   0,    0,   0,  /* version 82 */
    32,   70,  30,   6,   65,  28,   0,    0,   0,
    52,   38,  32,   2,   27,  32,   0,    0,   0,
    73,   20,  30,   2,   22,  32,   0,    0,   0,
    21,  126,  24,   4,  136,  26,   0,    0,   0,  /* version 83 */
    30,   74,  32,   6,   73,  30,   0,    0,   0,
    54,   35,  30,   4,   40,  32,   0,    0,   0,
    75,   20,  30,   1,   20,  28,   0,    0,   0,
    30,  105,  20,   1,  114,  22,   0,    0,   0,  /* version 84 */
     3,   45,  22,  55,   47,  20,   0,    0,   0,
     2,   26,  26,  62,   33,  28,   0,    0,   0,
    79,   18,  28,   4,   33,  30,   0,    0,   0
};

/* vim: set ts=4 sw=4 et : */
#endif /* Z_HANXIN_H */
