/*
 * aes_x86.c
 *
 * This file contains routines to do AES encryption and decryption using AES instructions in
 * x86 hardware.  This is considerably faster than doing it in software, of course.
 *
 * If you're compiling with gcc, you'll likely need to pass the -maes and -msse4.2 flags to
 * the compiler.
 *
 */

#include <wmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include "aes.h"
#include <string.h>

#define __cpuid(info, func)						\
	__asm__ __volatile__ ("cpuid":					\
			      "=a" (info[0]), "=b" (info[1]), "=c" (info[2]), "=d" (info[3]) : "a" (func));

static
unsigned
Check_CPU_support_AES ()
{
	int cpu_info[4];

	__cpuid (cpu_info, 1);
	return (!! (cpu_info[2] & 0x2000000) );
}

//
// From the Intel AES instructions manual, pages 24-25.
//
static
inline
__m128i AES_128_ASSIST (__m128i temp1, __m128i temp2)
{
	__m128i temp3;
	temp2 = _mm_shuffle_epi32 (temp2 ,0xff);
	temp3 = _mm_slli_si128 (temp1, 0x4);
	temp1 = _mm_xor_si128 (temp1, temp3);
	temp3 = _mm_slli_si128 (temp3, 0x4);
	temp1 = _mm_xor_si128 (temp1, temp3);
	temp3 = _mm_slli_si128 (temp3, 0x4);
	temp1 = _mm_xor_si128 (temp1, temp3);
	temp1 = _mm_xor_si128 (temp1, temp2);
	return temp1;
}

static
inline
void AES_128_Key_Expansion (const unsigned char *userkey,
                            unsigned char *key)
{
	__m128i temp1, temp2;
	__m128i *Key_Schedule = (__m128i*)key;

	temp1 = _mm_loadu_si128((__m128i*)userkey);
	_mm_storeu_si128(Key_Schedule, temp1);
	temp2 = _mm_aeskeygenassist_si128 (temp1, 0x1);
	temp1 = AES_128_ASSIST(temp1, temp2);
	_mm_storeu_si128(Key_Schedule+1, temp1);
	temp2 = _mm_aeskeygenassist_si128 (temp1, 0x2);
	temp1 = AES_128_ASSIST(temp1, temp2);
	_mm_storeu_si128(Key_Schedule+2, temp1);
	temp2 = _mm_aeskeygenassist_si128 (temp1, 0x4);
	temp1 = AES_128_ASSIST(temp1, temp2);
	_mm_storeu_si128(Key_Schedule+3, temp1);
	temp2 = _mm_aeskeygenassist_si128 (temp1, 0x8);
	temp1 = AES_128_ASSIST(temp1, temp2);
	_mm_storeu_si128(Key_Schedule+4, temp1);
	temp2 = _mm_aeskeygenassist_si128 (temp1, 0x10);
	temp1 = AES_128_ASSIST(temp1, temp2);
	_mm_storeu_si128(Key_Schedule+5, temp1);
	temp2 = _mm_aeskeygenassist_si128 (temp1, 0x20);
	temp1 = AES_128_ASSIST(temp1, temp2);
	_mm_storeu_si128(Key_Schedule+6, temp1);
	temp2 = _mm_aeskeygenassist_si128 (temp1, 0x40);
	temp1 = AES_128_ASSIST(temp1, temp2);
	_mm_storeu_si128(Key_Schedule+7, temp1);
	temp2 = _mm_aeskeygenassist_si128 (temp1, 0x80);
	temp1 = AES_128_ASSIST(temp1, temp2);
	_mm_storeu_si128(Key_Schedule+8, temp1);
	temp2 = _mm_aeskeygenassist_si128 (temp1, 0x1b);
	temp1 = AES_128_ASSIST(temp1, temp2);
	_mm_storeu_si128(Key_Schedule+9, temp1);
	temp2 = _mm_aeskeygenassist_si128 (temp1, 0x36);
	temp1 = AES_128_ASSIST(temp1, temp2);
	_mm_storeu_si128(Key_Schedule+10, temp1);
}

static
inline
void KEY_256_ASSIST_1(__m128i* temp1, __m128i * temp2)
{
        __m128i temp4;

        *temp2 = _mm_shuffle_epi32(*temp2, 0xff);
        temp4 = _mm_slli_si128 (*temp1, 0x4);
        *temp1 = _mm_xor_si128 (*temp1, temp4);
        temp4 = _mm_slli_si128 (temp4, 0x4);
        *temp1 = _mm_xor_si128 (*temp1, temp4);
        temp4 = _mm_slli_si128 (temp4, 0x4);
        *temp1 = _mm_xor_si128 (*temp1, temp4);
        *temp1 = _mm_xor_si128 (*temp1, *temp2);
}

static
inline
void KEY_256_ASSIST_2(__m128i* temp1, __m128i * temp3)
{
        __m128i temp2, temp4;
        temp4 = _mm_aeskeygenassist_si128 (*temp1, 0x0);
        temp2 = _mm_shuffle_epi32(temp4, 0xaa);
        temp4 = _mm_slli_si128 (*temp3, 0x4);
        *temp3 = _mm_xor_si128 (*temp3, temp4);
        temp4 = _mm_slli_si128 (temp4, 0x4);
        *temp3 = _mm_xor_si128 (*temp3, temp4);
        temp4 = _mm_slli_si128 (temp4, 0x4);
        *temp3 = _mm_xor_si128 (*temp3, temp4);
        *temp3 = _mm_xor_si128 (*temp3, temp2);
}

static inline
void AES_256_Key_Expansion (const unsigned char *userkey,
                            unsigned char *key)
{
        __m128i temp1, temp2, temp3;
        __m128i * Key_Schedule = (__m128i*)key;

        temp1 = _mm_loadu_si128((__m128i*)userkey);
        temp3 = _mm_loadu_si128((__m128i*)(userkey+16));
        Key_Schedule[0] = temp1;
        Key_Schedule[1] = temp3;
        temp2 = _mm_aeskeygenassist_si128 (temp3,0x01);
        KEY_256_ASSIST_1(&temp1, &temp2);
        Key_Schedule[2]=temp1;
        KEY_256_ASSIST_2(&temp1, &temp3);
        Key_Schedule[3]=temp3;
        temp2 = _mm_aeskeygenassist_si128 (temp3,0x02);
        KEY_256_ASSIST_1(&temp1, &temp2);
        Key_Schedule[4]=temp1;
        KEY_256_ASSIST_2(&temp1, &temp3);
        Key_Schedule[5]=temp3;
        temp2 = _mm_aeskeygenassist_si128 (temp3,0x04);
        KEY_256_ASSIST_1(&temp1, &temp2);
        Key_Schedule[6]=temp1;
        KEY_256_ASSIST_2(&temp1, &temp3);
        Key_Schedule[7]=temp3;
        temp2 = _mm_aeskeygenassist_si128 (temp3,0x08);
        KEY_256_ASSIST_1(&temp1, &temp2);
        Key_Schedule[8]=temp1;
        KEY_256_ASSIST_2(&temp1, &temp3);
        Key_Schedule[9]=temp3;
        temp2 = _mm_aeskeygenassist_si128 (temp3,0x10);
        KEY_256_ASSIST_1(&temp1, &temp2);
        Key_Schedule[10]=temp1;
        KEY_256_ASSIST_2(&temp1, &temp3);
        Key_Schedule[11]=temp3;
        temp2 = _mm_aeskeygenassist_si128 (temp3,0x20);
        KEY_256_ASSIST_1(&temp1, &temp2);
        Key_Schedule[12]=temp1;
        KEY_256_ASSIST_2(&temp1, &temp3);
        Key_Schedule[13]=temp3;
        temp2 = _mm_aeskeygenassist_si128 (temp3,0x40);
        KEY_256_ASSIST_1(&temp1, &temp2);
        Key_Schedule[14]=temp1;
}


/*
 * This code is adopted from the Intel AES instructions manual (page 28).
 * Note that the key comes from the crypto_aes struct, and must be set
 * in advance of calling this routine.
 *
 * The encryption algorithm here is ECB mode.
 */
static
unsigned
do_encrypt_ecb(const uint8_t *in, uint8_t *out, unsigned nbytes,
               const __m128i * round_keys, unsigned number_of_rounds)
{
    __m128i             t1;
    unsigned            i,j;
    unsigned            nblocks = (nbytes + AES_BLOCKSIZE - 1) / AES_BLOCKSIZE;

    // Encryption always happens in chunks of blocksize bytes.
    // No way around this - encryption doesn't work on odd-size boundaries.
    // length is now the number of blocks, not bytes

    for(i = 0; i < nblocks; i++){
        t1 = _mm_loadu_si128(&(((__m128i *)in)[i]));
        t1 = _mm_xor_si128(t1, round_keys[0]);
        for( j = 1; j < number_of_rounds; j++){
            t1 = _mm_aesenc_si128(t1, round_keys[j]);
        }
        t1 = _mm_aesenclast_si128(t1, round_keys[j]);
        _mm_storeu_si128 (&(((__m128i *)out)[i]), t1);
    }
    return nbytes;
}

static
unsigned
do_decrypt_ecb(const uint8_t *in, uint8_t *out, unsigned nbytes,
               const __m128i * round_keys, unsigned number_of_rounds)
{
    __m128i             t1;
    __m128i             rk;
    uint32_t            i,j;
    unsigned            nblocks = (nbytes + AES_BLOCKSIZE - 1) / AES_BLOCKSIZE;

    for(i=0; i < nblocks; i++) {
        t1 = _mm_loadu_si128(&(((__m128i *)in)[i]));
        t1 = _mm_xor_si128 (t1, round_keys[number_of_rounds]);
        for(j = number_of_rounds - 1; j > 0; j--) {
            rk = _mm_aesimc_si128(round_keys[j]);
            t1 = _mm_aesdec_si128(t1, rk);
        }
        t1 = _mm_aesdeclast_si128(t1, round_keys[0]);
        _mm_storeu_si128 (&(((__m128i *)out)[i]), t1);
    }
    return nbytes;
}

unsigned
aes_encrypt_ecb (const uint8_t *src, uint8_t *dst, unsigned nbytes,
                 const aes_key_block * key_block)
{
    /* Technically, it works fine regardless, but we want to be compatible with the
       handwritten version.
     */
    if (! (key_block->mode & AES_MODE_ENCRYPT) ) {
        return 0;
    }
    if (key_block->mode & AES_MODE_128) {
        return do_encrypt_ecb (src, dst, nbytes, (const __m128i *)key_block->round_keys_encrypt,
                               AES_N_ROUNDS_128);
    } else if (key_block->mode & AES_MODE_256) {
        return do_encrypt_ecb (src, dst, nbytes, (const __m128i *)key_block->round_keys_encrypt,
                               AES_N_ROUNDS_256);
    } else {
        return 0;
    }
}

unsigned
aes_decrypt_ecb (const uint8_t *src, uint8_t *dst, unsigned nbytes,
                 const aes_key_block * key_block)
{
    /* Technically, it works fine regardless, but we want to be compatible with the
       handwritten version.
     */
    if (! (key_block->mode & AES_MODE_DECRYPT) ) {
        return 0;
    }
    if (key_block->mode & AES_MODE_128) {
        return do_decrypt_ecb (src, dst, nbytes, (const __m128i *)key_block->round_keys_encrypt,
                               AES_N_ROUNDS_128);
    } else if (key_block->mode & AES_MODE_256) {
        return do_decrypt_ecb (src, dst, nbytes, (const __m128i *)key_block->round_keys_encrypt,
                               AES_N_ROUNDS_256);
    } else {
        return 0;
    }
}

unsigned
aes_init_key_block (const uint8_t * key, unsigned mode, aes_key_block * key_block)
{
    key_block->mode = mode;
    if (!! (mode & AES_MODE_128)) {
        AES_128_Key_Expansion (key, (unsigned char *)key_block->round_keys_encrypt);
    } else if (!! (mode & AES_MODE_256)) {
        AES_256_Key_Expansion (key, (unsigned char *)key_block->round_keys_encrypt);
    } else {
        key_block->mode = 0;
        return 0;
    }
    return 1;
}
