/* Copyright (c) 2013-2016 Cameron Harper
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * */

/**
 * @example test_aes_cmac.c
 *
 * Tests from SP800-38B
 *
 * */
 
#include "unity.h"

#include "moda.h"
#include "aes.h"
#include "aes_cmac.h"

#include <string.h>

void test_MODA_AES_CMAC_128_mlen0(void)
{
    struct aes_ctxt aes;
    static const uint8_t key[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};

    static const uint8_t expectedT[] = {0xbb,0x1d,0x69,0x29,0xe9,0x59,0x37,0x28,0x7f,0xa3,0x7d,0x12,0x9b,0x75,0x67,0x46};

    uint8_t t[AES_BLOCK_SIZE];

    MODA_AES_Init(&aes, AES_KEY_128, key);
    memset(t, 0x0, sizeof(t));

    MODA_AES_CMAC(&aes, NULL, 0U, t, sizeof(t));

    TEST_ASSERT_EQUAL_MEMORY(expectedT, t, sizeof(expectedT));    
}

void test_MODA_AES_CMAC_128_mlen128(void)
{
    struct aes_ctxt aes;
    static const uint8_t key[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
    static const uint8_t m[] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a};

    static const uint8_t expectedT[] = {0x07,0x0a,0x16,0xb4,0x6b,0x4d,0x41,0x44,0xf7,0x9b,0xdd,0x9d,0xd0,0x4a,0x28,0x7c};

    uint8_t t[AES_BLOCK_SIZE];

    MODA_AES_Init(&aes, AES_KEY_128, key);
    memset(t, 0x0, sizeof(t));

    MODA_AES_CMAC(&aes, m, sizeof(m), t, sizeof(t));

    TEST_ASSERT_EQUAL_MEMORY(expectedT, t, sizeof(expectedT));    
}

void test_MODA_AES_CMAC_128_mlen320(void)
{
    struct aes_ctxt aes;
    static const uint8_t key[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
    static const uint8_t m[] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11 };

    static const uint8_t expectedT[] = {0xdf,0xa6,0x67,0x47,0xde,0x9a,0xe6,0x30,0x30,0xca,0x32,0x61,0x14,0x97,0xc8,0x27};

    uint8_t t[AES_BLOCK_SIZE];

    MODA_AES_Init(&aes, AES_KEY_128, key);
    memset(t, 0x0, sizeof(t));

    MODA_AES_CMAC(&aes, m, sizeof(m), t, sizeof(t));

    TEST_ASSERT_EQUAL_MEMORY(expectedT, t, sizeof(expectedT));    
}

void test_MODA_AES_CMAC_128_mlen512(void)
{
    struct aes_ctxt aes;
    static const uint8_t key[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
    static const uint8_t m[] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};

    static const uint8_t expectedT[] = {0x51,0xf0,0xbe,0xbf,0x7e,0x3b,0x9d,0x92,0xfc,0x49,0x74,0x17,0x79,0x36,0x3c,0xfe};

    uint8_t t[AES_BLOCK_SIZE];

    MODA_AES_Init(&aes, AES_KEY_128, key);
    memset(t, 0x0, sizeof(t));

    MODA_AES_CMAC(&aes, m, sizeof(m), t, sizeof(t));

    TEST_ASSERT_EQUAL_MEMORY(expectedT, t, sizeof(expectedT));    
}

void test_MODA_AES_CMAC_192_mlen0(void)
{
    struct aes_ctxt aes;
    static const uint8_t key[] = {0x8e,0x73,0xb0,0xf7,0xda,0x0e,0x64,0x52,0xc8,0x10,0xf3,0x2b,0x80,0x90,0x79,0xe5,0x62,0xf8,0xea,0xd2,0x52,0x2c,0x6b,0x7b};

    static const uint8_t expectedT[] = {0xd1,0x7d,0xdf,0x46,0xad,0xaa,0xcd,0xe5,0x31,0xca,0xc4,0x83,0xde,0x7a,0x93,0x67};

    uint8_t t[AES_BLOCK_SIZE];

    MODA_AES_Init(&aes, AES_KEY_192, key);
    memset(t, 0x0, sizeof(t));

    MODA_AES_CMAC(&aes, NULL, 0U, t, sizeof(t));

    TEST_ASSERT_EQUAL_MEMORY(expectedT, t, sizeof(expectedT));    
}

void test_MODA_AES_CMAC_192_mlen128(void)
{
    struct aes_ctxt aes;
    static const uint8_t key[] = {0x8e,0x73,0xb0,0xf7,0xda,0x0e,0x64,0x52,0xc8,0x10,0xf3,0x2b,0x80,0x90,0x79,0xe5,0x62,0xf8,0xea,0xd2,0x52,0x2c,0x6b,0x7b};
    static const uint8_t m[] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a};

    static const uint8_t expectedT[] = {0x9e,0x99,0xa7,0xbf,0x31,0xe7,0x10,0x90,0x06,0x62,0xf6,0x5e,0x61,0x7c,0x51,0x84};

    uint8_t t[AES_BLOCK_SIZE];

    MODA_AES_Init(&aes, AES_KEY_192, key);
    memset(t, 0x0, sizeof(t));

    MODA_AES_CMAC(&aes, m, sizeof(m), t, sizeof(t));

    TEST_ASSERT_EQUAL_MEMORY(expectedT, t, sizeof(expectedT));    
}

void test_MODA_AES_CMAC_192_mlen320(void)
{
    struct aes_ctxt aes;
    static const uint8_t key[] = {0x8e,0x73,0xb0,0xf7,0xda,0x0e,0x64,0x52,0xc8,0x10,0xf3,0x2b,0x80,0x90,0x79,0xe5,0x62,0xf8,0xea,0xd2,0x52,0x2c,0x6b,0x7b};
    static const uint8_t m[] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11};

    static const uint8_t expectedT[] = {0x8a,0x1d,0xe5,0xbe,0x2e,0xb3,0x1a,0xad,0x08,0x9a,0x82,0xe6,0xee,0x90,0x8b,0x0e};

    uint8_t t[AES_BLOCK_SIZE];

    MODA_AES_Init(&aes, AES_KEY_192, key);
    memset(t, 0x0, sizeof(t));

    MODA_AES_CMAC(&aes, m, sizeof(m), t, sizeof(t));

    TEST_ASSERT_EQUAL_MEMORY(expectedT, t, sizeof(expectedT));    
}

void test_MODA_AES_CMAC_192_mlen512(void)
{
    struct aes_ctxt aes;
    static const uint8_t key[] = {0x8e,0x73,0xb0,0xf7,0xda,0x0e,0x64,0x52,0xc8,0x10,0xf3,0x2b,0x80,0x90,0x79,0xe5,0x62,0xf8,0xea,0xd2,0x52,0x2c,0x6b,0x7b};
    static const uint8_t m[] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};

    static const uint8_t expectedT[] = {0xa1,0xd5,0xdf,0x0e,0xed,0x79,0x0f,0x79,0x4d,0x77,0x58,0x96,0x59,0xf3,0x9a,0x11};

    uint8_t t[AES_BLOCK_SIZE];

    MODA_AES_Init(&aes, AES_KEY_192, key);
    memset(t, 0x0, sizeof(t));

    MODA_AES_CMAC(&aes, m, sizeof(m), t, sizeof(t));

    TEST_ASSERT_EQUAL_MEMORY(expectedT, t, sizeof(expectedT));    
}

void test_MODA_AES_CMAC_256_mlen0(void)
{
    struct aes_ctxt aes;
    static const uint8_t key[] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};

    static const uint8_t expectedT[] = {0x02,0x89,0x62,0xf6,0x1b,0x7b,0xf8,0x9e,0xfc,0x6b,0x55,0x1f,0x46,0x67,0xd9,0x83};

    uint8_t t[AES_BLOCK_SIZE];

    MODA_AES_Init(&aes, AES_KEY_256, key);
    memset(t, 0x0, sizeof(t));

    MODA_AES_CMAC(&aes, NULL, 0U, t, sizeof(t));

    TEST_ASSERT_EQUAL_MEMORY(expectedT, t, sizeof(expectedT));    
}

void test_MODA_AES_CMAC_256_mlen128(void)
{
    struct aes_ctxt aes;
    static const uint8_t key[] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
    static const uint8_t m[] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a};

    static const uint8_t expectedT[] = {0x28,0xa7,0x02,0x3f,0x45,0x2e,0x8f,0x82,0xbd,0x4b,0xf2,0x8d,0x8c,0x37,0xc3,0x5c};

    uint8_t t[AES_BLOCK_SIZE];

    MODA_AES_Init(&aes, AES_KEY_256, key);
    memset(t, 0x0, sizeof(t));

    MODA_AES_CMAC(&aes, m, sizeof(m), t, sizeof(t));

    TEST_ASSERT_EQUAL_MEMORY(expectedT, t, sizeof(expectedT));    
}

void test_MODA_AES_CMAC_256_mlen320(void)
{
    struct aes_ctxt aes;
    static const uint8_t key[] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
    static const uint8_t m[] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11};

    static const uint8_t expectedT[] = {0xaa,0xf3,0xd8,0xf1,0xde,0x56,0x40,0xc2,0x32,0xf5,0xb1,0x69,0xb9,0xc9,0x11,0xe6};

    uint8_t t[AES_BLOCK_SIZE];

    MODA_AES_Init(&aes, AES_KEY_256, key);
    memset(t, 0x0, sizeof(t));

    MODA_AES_CMAC(&aes, m, sizeof(m), t, sizeof(t));

    TEST_ASSERT_EQUAL_MEMORY(expectedT, t, sizeof(expectedT));    
}

void test_MODA_AES_CMAC_256_mlen512(void)
{
    struct aes_ctxt aes;
    static const uint8_t key[] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
    static const uint8_t m[] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10};

    static const uint8_t expectedT[] = {0xe1,0x99,0x21,0x90,0x54,0x9f,0x6e,0xd5,0x69,0x6a,0x2c,0x05,0x6c,0x31,0x54,0x10};

    uint8_t t[AES_BLOCK_SIZE];

    MODA_AES_Init(&aes, AES_KEY_256, key);
    memset(t, 0x0, sizeof(t));

    MODA_AES_CMAC(&aes, m, sizeof(m), t, sizeof(t));

    TEST_ASSERT_EQUAL_MEMORY(expectedT, t, sizeof(expectedT));    
}
