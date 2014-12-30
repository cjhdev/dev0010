/* Copyright (c) 2013-2014 Cameron Harper
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

/* defines ************************************************************/

#ifdef NDEBUG

    /*lint -e(9026) Allow assert to be removed completely */
    #define ASSERT(X) ;

#else

    #include <assert.h>
    #include <stddef.h>

    /*lint -e(9026) Allow assert to be removed completely */
    #define ASSERT(X) /*lint -e(9034)*/assert(X);

#endif

#ifndef AES_CONST
    #define AES_CONST
#endif    
#ifndef SBOX
    /*lint -e(9026)*/
    #define SBOX(C) sbox[(C)]
#endif
#ifndef RSBOX
    /*lint -e(9026)*/
    #define RSBOX(C) rsbox[(C)]
#endif
#ifndef RCON
    /*lint -e(9026)*/
    #define RCON(C) rcon[(C)]
#endif

#define R1 0U
#define R2 1U
#define R3 2U
#define R4 3U

#define C1 0U
#define C2 4U
#define C3 8U
#define C4 12U

/*lint -e(9026)*/    
#define GALOIS_MUL2(B) ((((B) & 0x80U) == 0x80U) ? (uint8_t)(((B) << 1U) ^ 0x1bU) : (uint8_t)((B) << 1U))

/* globals ************************************************************/

static const uint8_t sbox[] AES_CONST = {
    0x63U, 0x7cU, 0x77U, 0x7bU, 0xf2U, 0x6bU, 0x6fU, 0xc5U,
    0x30U, 0x01U, 0x67U, 0x2bU, 0xfeU, 0xd7U, 0xabU, 0x76U,
    0xcaU, 0x82U, 0xc9U, 0x7dU, 0xfaU, 0x59U, 0x47U, 0xf0U,
    0xadU, 0xd4U, 0xa2U, 0xafU, 0x9cU, 0xa4U, 0x72U, 0xc0U,
    0xb7U, 0xfdU, 0x93U, 0x26U, 0x36U, 0x3fU, 0xf7U, 0xccU,
    0x34U, 0xa5U, 0xe5U, 0xf1U, 0x71U, 0xd8U, 0x31U, 0x15U,
    0x04U, 0xc7U, 0x23U, 0xc3U, 0x18U, 0x96U, 0x05U, 0x9aU,
    0x07U, 0x12U, 0x80U, 0xe2U, 0xebU, 0x27U, 0xb2U, 0x75U,
    0x09U, 0x83U, 0x2cU, 0x1aU, 0x1bU, 0x6eU, 0x5aU, 0xa0U,
    0x52U, 0x3bU, 0xd6U, 0xb3U, 0x29U, 0xe3U, 0x2fU, 0x84U,
    0x53U, 0xd1U, 0x00U, 0xedU, 0x20U, 0xfcU, 0xb1U, 0x5bU,
    0x6aU, 0xcbU, 0xbeU, 0x39U, 0x4aU, 0x4cU, 0x58U, 0xcfU,
    0xd0U, 0xefU, 0xaaU, 0xfbU, 0x43U, 0x4dU, 0x33U, 0x85U,
    0x45U, 0xf9U, 0x02U, 0x7fU, 0x50U, 0x3cU, 0x9fU, 0xa8U,
    0x51U, 0xa3U, 0x40U, 0x8fU, 0x92U, 0x9dU, 0x38U, 0xf5U,
    0xbcU, 0xb6U, 0xdaU, 0x21U, 0x10U, 0xffU, 0xf3U, 0xd2U,
    0xcdU, 0x0cU, 0x13U, 0xecU, 0x5fU, 0x97U, 0x44U, 0x17U,
    0xc4U, 0xa7U, 0x7eU, 0x3dU, 0x64U, 0x5dU, 0x19U, 0x73U,
    0x60U, 0x81U, 0x4fU, 0xdcU, 0x22U, 0x2aU, 0x90U, 0x88U,
    0x46U, 0xeeU, 0xb8U, 0x14U, 0xdeU, 0x5eU, 0x0bU, 0xdbU,
    0xe0U, 0x32U, 0x3aU, 0x0aU, 0x49U, 0x06U, 0x24U, 0x5cU,
    0xc2U, 0xd3U, 0xacU, 0x62U, 0x91U, 0x95U, 0xe4U, 0x79U,
    0xe7U, 0xc8U, 0x37U, 0x6dU, 0x8dU, 0xd5U, 0x4eU, 0xa9U,
    0x6cU, 0x56U, 0xf4U, 0xeaU, 0x65U, 0x7aU, 0xaeU, 0x08U,
    0xbaU, 0x78U, 0x25U, 0x2eU, 0x1cU, 0xa6U, 0xb4U, 0xc6U,
    0xe8U, 0xddU, 0x74U, 0x1fU, 0x4bU, 0xbdU, 0x8bU, 0x8aU,
    0x70U, 0x3eU, 0xb5U, 0x66U, 0x48U, 0x03U, 0xf6U, 0x0eU,
    0x61U, 0x35U, 0x57U, 0xb9U, 0x86U, 0xc1U, 0x1dU, 0x9eU,
    0xe1U, 0xf8U, 0x98U, 0x11U, 0x69U, 0xd9U, 0x8eU, 0x94U,
    0x9bU, 0x1eU, 0x87U, 0xe9U, 0xceU, 0x55U, 0x28U, 0xdfU,
    0x8cU, 0xa1U, 0x89U, 0x0dU, 0xbfU, 0xe6U, 0x42U, 0x68U,
    0x41U, 0x99U, 0x2dU, 0x0fU, 0xb0U, 0x54U, 0xbbU, 0x16U
};

/* private prototypes *************************************************/

static void localMemcpy(uint8_t *s1, const uint8_t *s2, uint8_t n);

/* public implementation **********************************************/

void MODA_AES_Init(struct aes_ctxt *aes, enum aes_key_size keySize, const uint8_t *key)
{
    uint8_t p;
    uint8_t j;
    uint8_t b = 0U;
    uint8_t swap;
    uint8_t *k;
    uint8_t ks;
    uint8_t i = 1U;

    static const uint8_t rcon[] AES_CONST = {
        0x8dU, 0x01U, 0x02U, 0x04U, 0x08U, 0x10U, 0x20U, 0x40U, 0x80U, 0x1bU, 0x36U
    };

    ASSERT((aes != NULL))
    ASSERT((key != NULL))
    
    switch(keySize){
    case AES_KEY_128:
        aes->r = 10U;
        b = 176U;
        break;

    case AES_KEY_192:
        aes->r = 12U;
        b = 208U;
        break;

    case AES_KEY_256:
        aes->r = 14U;
        b = 240U;
        break;         

    default:

        ASSERT(((keySize != AES_KEY_128)&&(keySize != AES_KEY_192)&&(keySize != AES_KEY_256)))
        break;
    }

    localMemcpy(aes->k, key, (uint8_t)keySize);
    k = aes->k;    
    ks = (uint8_t)keySize;
    p = ks;

    /* Rijndael key schedule */
    while(p < b){
        
        swap = k[p - 4U];
        k[p     ] = SBOX( k[p - 3U] ) ^ k[p      - ks] ^ RCON(i);
        k[p + 1U] = SBOX( k[p - 2U] ) ^ k[p + 1U - ks];
        k[p + 2U] = SBOX( k[p - 1U] ) ^ k[p + 2U - ks];
        k[p + 3U] = SBOX( swap      ) ^ k[p + 3U - ks];
        p += 4U;

        for(j=0U; j < 12U; j++){

            k[p] = k[p - 4U] ^ k[p - ks];
            p++;
        }

        if(p < b){
            
            if(keySize == AES_KEY_192){

                for(j=0U; j < 8U; j++){

                    k[p] = k[p - 4U] ^ k[p - ks];
                    p++;
                }
            }
            else{

                if(keySize == AES_KEY_256){

                    for(j=0U; j < 4U; j++){

                        k[p] = sbox[ k[p - 4U] ] ^ k[p - ks];
                        p++;
                    }

                    for(j=0U; j < 12U; j++){

                        k[p] = k[p - 4U] ^ k[p - ks];
                        p++;
                    }
                }
            }
        }

        i++;
    }
}

void MODA_AES_Encrypt(const struct aes_ctxt *aes, uint8_t *s)
{
    uint8_t r;    
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;    
    uint8_t i;
    uint8_t p = 0U;
    const uint8_t *k = aes->k;

    /* add round key, sbox and shiftrows */
    for(r = 0U; r < aes->r; r++){

        /* add round key, sbox, left shift row */

        /* row 1 */
        s[R1 + C1] = SBOX( s[R1 + C1] ^ k[p + R1 + C1] );   /*lint -e(835) */
        s[R1 + C2] = SBOX( s[R1 + C2] ^ k[p + R1 + C2] );   /*lint -e(835)*/
        s[R1 + C3] = SBOX( s[R1 + C3] ^ k[p + R1 + C3] );   /*lint -e(835)*/
        s[R1 + C4] = SBOX( s[R1 + C4] ^ k[p + R1 + C4] );   /*lint -e(835)*/

        /* row 2, left shift 1 */
        a = SBOX( s[R2 + C1] ^ k[p + R2 + C1] );
        s[R2 + C1] = SBOX( s[R2 + C2] ^ k[p + R2 + C2] );
        s[R2 + C2] = SBOX( s[R2 + C3] ^ k[p + R2 + C3] );
        s[R2 + C3] = SBOX( s[R2 + C4] ^ k[p + R2 + C4] );
        s[R2 + C4] = a;

        /* row 3, left shift 2 */
        a = SBOX( s[R3 + C1] ^ k[p + R3 + C1] );
        b = SBOX( s[R3 + C2] ^ k[p + R3 + C2] );
        s[R3 + C1] = SBOX( s[R3 + C3] ^ k[p + R3 + C3] );
        s[R3 + C2] = SBOX( s[R3 + C4] ^ k[p + R3 + C4] );
        s[R3 + C3] = a;
        s[R3 + C4] = b;

        /* row 4, left shift 3 */
        a = SBOX( s[R4 + C4] ^ k[p + R4 + C4] );
        s[R4 + C4] = SBOX( s[R4 + C3] ^ k[p + R4 + C3] );
        s[R4 + C3] = SBOX( s[R4 + C2] ^ k[p + R4 + C2] );
        s[R4 + C2] = SBOX( s[R4 + C1] ^ k[p + R4 + C1] );
        s[R4 + C1] = a;

        if((r+1U) == aes->r){

            p += 16U;

            /* final add round key */
            for(i=0U; i < 16U; i++){

                s[i] ^= aes->k[p+i];
            }
            
            break;
        }
         
        /* mix columns */
        for(i=0U; i < 16U; i += 4U){

            a = s[i     ];
            b = s[i + 1U];
            c = s[i + 2U];
            d = s[i + 3U];

            /* 2a + 3b + 1c + 1d 
             * 1a + 2b + 3c + 1d
             * 1a + 1b + 2c + 3d
             * 3a + 1b + 1c + 2d
             *
             * */
            s[i     ] ^= (a ^ b ^ c ^ d) ^ GALOIS_MUL2( (a ^ b) );
            s[i + 1U] ^= (a ^ b ^ c ^ d) ^ GALOIS_MUL2( (b ^ c) );
            s[i + 2U] ^= (a ^ b ^ c ^ d) ^ GALOIS_MUL2( (c ^ d) );
            s[i + 3U] ^= (a ^ b ^ c ^ d) ^ GALOIS_MUL2( (d ^ a) );
        }

        p += 16U;
    }   
}

void MODA_AES_Decrypt(const struct aes_ctxt *aes, uint8_t *s)
{
    uint8_t r;
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t x;
    uint8_t y;
    uint8_t i;
    uint8_t p;
    const uint8_t *k = aes->k;

    static const uint8_t rsbox[] AES_CONST = {
        0x52U, 0x09U, 0x6aU, 0xd5U, 0x30U, 0x36U, 0xa5U, 0x38U,
        0xbfU, 0x40U, 0xa3U, 0x9eU, 0x81U, 0xf3U, 0xd7U, 0xfbU,
        0x7cU, 0xe3U, 0x39U, 0x82U, 0x9bU, 0x2fU, 0xffU, 0x87U,
        0x34U, 0x8eU, 0x43U, 0x44U, 0xc4U, 0xdeU, 0xe9U, 0xcbU,
        0x54U, 0x7bU, 0x94U, 0x32U, 0xa6U, 0xc2U, 0x23U, 0x3dU,
        0xeeU, 0x4cU, 0x95U, 0x0bU, 0x42U, 0xfaU, 0xc3U, 0x4eU,
        0x08U, 0x2eU, 0xa1U, 0x66U, 0x28U, 0xd9U, 0x24U, 0xb2U,
        0x76U, 0x5bU, 0xa2U, 0x49U, 0x6dU, 0x8bU, 0xd1U, 0x25U,
        0x72U, 0xf8U, 0xf6U, 0x64U, 0x86U, 0x68U, 0x98U, 0x16U,
        0xd4U, 0xa4U, 0x5cU, 0xccU, 0x5dU, 0x65U, 0xb6U, 0x92U,
        0x6cU, 0x70U, 0x48U, 0x50U, 0xfdU, 0xedU, 0xb9U, 0xdaU,
        0x5eU, 0x15U, 0x46U, 0x57U, 0xa7U, 0x8dU, 0x9dU, 0x84U,
        0x90U, 0xd8U, 0xabU, 0x00U, 0x8cU, 0xbcU, 0xd3U, 0x0aU,
        0xf7U, 0xe4U, 0x58U, 0x05U, 0xb8U, 0xb3U, 0x45U, 0x06U,
        0xd0U, 0x2cU, 0x1eU, 0x8fU, 0xcaU, 0x3fU, 0x0fU, 0x02U,
        0xc1U, 0xafU, 0xbdU, 0x03U, 0x01U, 0x13U, 0x8aU, 0x6bU,
        0x3aU, 0x91U, 0x11U, 0x41U, 0x4fU, 0x67U, 0xdcU, 0xeaU,
        0x97U, 0xf2U, 0xcfU, 0xceU, 0xf0U, 0xb4U, 0xe6U, 0x73U,
        0x96U, 0xacU, 0x74U, 0x22U, 0xe7U, 0xadU, 0x35U, 0x85U,
        0xe2U, 0xf9U, 0x37U, 0xe8U, 0x1cU, 0x75U, 0xdfU, 0x6eU,
        0x47U, 0xf1U, 0x1aU, 0x71U, 0x1dU, 0x29U, 0xc5U, 0x89U,
        0x6fU, 0xb7U, 0x62U, 0x0eU, 0xaaU, 0x18U, 0xbeU, 0x1bU,
        0xfcU, 0x56U, 0x3eU, 0x4bU, 0xc6U, 0xd2U, 0x79U, 0x20U,
        0x9aU, 0xdbU, 0xc0U, 0xfeU, 0x78U, 0xcdU, 0x5aU, 0xf4U,
        0x1fU, 0xddU, 0xa8U, 0x33U, 0x88U, 0x07U, 0xc7U, 0x31U,
        0xb1U, 0x12U, 0x10U, 0x59U, 0x27U, 0x80U, 0xecU, 0x5fU,
        0x60U, 0x51U, 0x7fU, 0xa9U, 0x19U, 0xb5U, 0x4aU, 0x0dU,
        0x2dU, 0xe5U, 0x7aU, 0x9fU, 0x93U, 0xc9U, 0x9cU, 0xefU,
        0xa0U, 0xe0U, 0x3bU, 0x4dU, 0xaeU, 0x2aU, 0xf5U, 0xb0U,
        0xc8U, 0xebU, 0xbbU, 0x3cU, 0x83U, 0x53U, 0x99U, 0x61U,
        0x17U, 0x2bU, 0x04U, 0x7eU, 0xbaU, 0x77U, 0xd6U, 0x26U,
        0xe1U, 0x69U, 0x14U, 0x63U, 0x55U, 0x21U, 0x0cU, 0x7dU
    };
    
    p = (uint8_t)(aes->r << 4U);
    
    /* add round key */
    for(i=0U; i < 16U; i++){

        s[i] ^= aes->k[p+i];
    }

    p -= 16U;
 
    /* add round key, sbox and shiftrows */
    for(r = aes->r; (r != 0U); r--){

        if(r < aes->r){

            /* inverse mix columns */
            for(i=0U; i < 16U; i += 4U){

                a = s[i     ];
                b = s[i + 1U];
                c = s[i + 2U];
                d = s[i + 3U];

                /* 2a + 2b + 2c + 2d */
                e = GALOIS_MUL2( (a ^ b ^ c ^ d) );

                /* 13a + 9b + 13c + 9d */
                x = GALOIS_MUL2( (e ^ a ^ c) );                
                x = (a ^ b ^ c ^ d) ^ GALOIS_MUL2( x );

                /* 9a + 13b + 9c + 13d */
                y = GALOIS_MUL2( (e ^ b ^ d) );                
                y = (a ^ b ^ c ^ d) ^ GALOIS_MUL2( y );
                
                /* 14a + 11b + 13c + 9d
                 * 9a + 14b + 11c + 13d
                 * 13a + 9b + 14c + 11d
                 * 11a + 13b + 9c + 14d
                 *
                 * */
                s[i     ] ^= x ^ GALOIS_MUL2( (a ^ b) );
                s[i + 1U] ^= y ^ GALOIS_MUL2( (b ^ c) );
                s[i + 2U] ^= x ^ GALOIS_MUL2( (c ^ d) );
                s[i + 3U] ^= y ^ GALOIS_MUL2( (d ^ a) );
            }
        }

        /* right shift row, reverse-sbox, add round key */

        /* row 1 */
        s[R1 + C1] = RSBOX( s[R1 + C1] ) ^ k[p + R1 + C1];
        s[R1 + C2] = RSBOX( s[R1 + C2] ) ^ k[p + R1 + C2];
        s[R1 + C3] = RSBOX( s[R1 + C3] ) ^ k[p + R1 + C3];
        s[R1 + C4] = RSBOX( s[R1 + C4] ) ^ k[p + R1 + C4];
        
        /* row 2, right shift 1 */
        a = RSBOX( s[R2 + C4] ) ^ k[p + R2 + C1];
        s[R2 + C4] = RSBOX( s[R2 + C3] ) ^ k[p + R2 + C4];
        s[R2 + C3] = RSBOX( s[R2 + C2] ) ^ k[p + R2 + C3];
        s[R2 + C2] = RSBOX( s[R2 + C1] ) ^ k[p + R2 + C2];
        s[R2 + C1] = a;

        /* row 3, right shift 2 */
        a = RSBOX( s[R3 + C1] ) ^ k[p + R3 + C3];
        b = RSBOX( s[R3 + C2] ) ^ k[p + R3 + C4];
        s[R3 + C1] = RSBOX( s[R3 + C3] ) ^ k[p + R3 + C1];
        s[R3 + C2] = RSBOX( s[R3 + C4] ) ^ k[p + R3 + C2];
        s[R3 + C3] = a;
        s[R3 + C4] = b;

        /* row 4, right shift 3 */
        a = RSBOX( s[R4 + C1] ) ^ k[p + R4 + C4];
        s[R4 + C1] = RSBOX( s[R4 + C2] ) ^ k[p + R4 + C1];
        s[R4 + C2] = RSBOX( s[R4 + C3] ) ^ k[p + R4 + C2];
        s[R4 + C3] = RSBOX( s[R4 + C4] ) ^ k[p + R4 + C3];
        s[R4 + C4] = a;

        p -= 16U;
    }
}

/* private implementation *********************************************/

static void localMemcpy(uint8_t *s1, const uint8_t *s2, uint8_t n)
{
    uint8_t pos = 0U;

    while(pos != n){

        s1[pos] = s2[pos];
        pos++;
    }
}
