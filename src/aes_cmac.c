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

/* includes ***********************************************************/

#include "aes.h"
#include "aes_cmac.h"
#include "moda_internal.h"

#include <string.h>

/* defines ************************************************************/

#define WORD_BLOCK_SIZE (AES_BLOCK_SIZE / MODA_WORD_SIZE)

#define LSB 0x01U

#if (MODA_WORD_SIZE == 1U)
    #define MSB 0x80U        
#elif (MODA_WORD_SIZE == 2U)
    #define MSB 0x8000U
#elif (MODA_WORD_SIZE == 4U)
    #define MSB 0x80000000U
#else
    #define MSB 0x8000000000000000U
#endif

/* static function prototypes *****************************************/

/**
 * XOR an aligned AES block (may be aliased)
 *
 * @param[out] acc accumulator
 * @param[in] mask XORed with accumulator
 *
 * */
static void xor128(moda_word_t *acc, const moda_word_t *mask);

/**
 * word copy an non-aliased aligned AES block
 *
 * @param[out] to copy destination
 * @param[in] from copy input
 *
 * */
static void copy128(moda_word_t *MODA_RESTRICT to, const moda_word_t *MODA_RESTRICT from);

/**
 * left shift (by one bit) a 128bit vector
 *
 * @param[in/out] v vector to shift
 *
 * */
static void leftShift128(moda_word_t *v);

#if (MODA_WORD_SIZE > 1U)
#ifndef MODA_BIG_ENDIAN
/**
 * Swap the byte endianness of a word
 *
 * @param[in] w input word
 * @return reversed word
 *
 * */
static moda_word_t swapw(moda_word_t w);

#endif
#endif


/* functions  *********************************************************/

void MODA_AES_CMAC(const struct aes_ctxt *aes, const uint8_t *in, uint32_t inLen, uint8_t *t, uint8_t tSize)
{
    moda_word_t k[WORD_BLOCK_SIZE];
    moda_word_t k1[WORD_BLOCK_SIZE];
    moda_word_t k2[WORD_BLOCK_SIZE];
    moda_word_t m[WORD_BLOCK_SIZE];
    uint32_t b;
    uint32_t n;
    uint32_t pos = 0U;
    
    ASSERT((aes != NULL))
    ASSERT((tSize <= AES_BLOCK_SIZE))
    
    n = (inLen / AES_BLOCK_SIZE);

    if( (inLen % AES_BLOCK_SIZE) != 0U ){

        n += 1U;    
    }

    if(n == 0U){

        n = 1U;
    }
    
    xor128(k, k);

    MODA_AES_Encrypt(aes, (uint8_t *)k);

    copy128(k1, k);
    leftShift128(k1);

    if((*(uint8_t *)k & 0x80U) == 0x80U){

        ((uint8_t *)k1)[AES_BLOCK_SIZE - 1U] ^= 0x87U;
    }

    copy128(k2, k1);
    leftShift128(k2);

    if((*(uint8_t *)k1 & 0x80U) == 0x80U){

        ((uint8_t *)k2)[AES_BLOCK_SIZE - 1U] ^= 0x87U;
    }        

    xor128(k, k);

    uint32_t size = inLen;
    
    for(b = 0U; b < n; b++){

        xor128(m, m);

        (void)memcpy(m, &in[pos], (size < AES_BLOCK_SIZE) ? (size_t)size : AES_BLOCK_SIZE);

        /* if the last block */
        if(size <= AES_BLOCK_SIZE){

            if(size == AES_BLOCK_SIZE){

                xor128(m, k1);
            }
            else{

                ((uint8_t *)m)[size] = 0x80U;
                xor128(m, k2);
            }            
        }
        else{

            pos += AES_BLOCK_SIZE;            
        }
        
        xor128(m, k);

        MODA_AES_Encrypt(aes, (uint8_t *)m);

        copy128(k, m);

        size -= AES_BLOCK_SIZE;
    }

    (void)memcpy(t, k, (size_t)tSize);
}


/* static functions  **************************************************/

static void leftShift128(moda_word_t *v)
{
    moda_word_t t;
    moda_word_t tt;
    moda_word_t carry;
    uint8_t i;
    
    carry = 0U;

    for(i=(uint8_t)WORD_BLOCK_SIZE; i > 0U; i--){

        t = v[i-1U];

#if (MODA_WORD_SIZE > 1U)
#ifndef MODA_BIG_ENDIAN
        t = swapw(t);        
#endif
#endif
        tt = t;
        tt <<= 1;
        tt |= carry;

#if (MODA_WORD_SIZE > 1U)
#ifndef MODA_BIG_ENDIAN
        tt = swapw(tt);        
#endif
#endif
        carry = ((t & MSB) == MSB) ? LSB : 0x0U;
        v[i-1U] = tt;
    }
}

static void xor128(moda_word_t *acc, const moda_word_t *mask)
{
    uint8_t i;
    for(i=0U; i < WORD_BLOCK_SIZE; i++){

        acc[i] ^= mask[i];
    }
}

static void copy128(moda_word_t *MODA_RESTRICT to, const moda_word_t *MODA_RESTRICT from)
{
    uint8_t i;
    for(i=0U; i < WORD_BLOCK_SIZE; i++){

        to[i] = from[i];
    }
}

#if (MODA_WORD_SIZE > 1U)
#ifndef MODA_BIG_ENDIAN
static moda_word_t swapw(moda_word_t w)
{
#if MODA_WORD_SIZE == 1U 
    return w;
#elif MODA_WORD_SIZE == 2U
    return ((w >> 8U) & 0xffU) | ((w << 8U) & 0xff00U);    
#elif MODA_WORD_SIZE == 4U
    return  ((w << 24U) & 0xff000000U)    |
            ((w <<  8U) & 0xff0000U)      |
            ((w >>  8U) & 0xff00U)        |
            ((w >> 24U) & 0xffU);
#else
    return  ((w << 56U) & 0xff00000000000000U)    |
            ((w << 40U) & 0xff000000000000U)      |
            ((w << 24U) & 0xff0000000000U)        |
            ((w <<  8U) & 0xff00000000U)          |
            ((w >>  8U) & 0xff000000U)            |
            ((w >> 24U) & 0xff0000U)              |
            ((w >> 40U) & 0xff00U)                |
            ((w >> 56U) & 0xffU);            
#endif
}

#endif
#endif
