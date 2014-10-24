/*
 * aes.h
 *
 *
 *
 */


#ifndef AES_H_

#include        <stdint.h>

#define AES_BLOCKSIZE           16

#define AES_N_ROUNDS_128        10
#define AES_N_ROUNDS_256        14
#define AES_N_ROUND_KEYS_128    (AES_N_ROUNDS_128 + 1)
#define AES_N_ROUND_KEYS_256    (AES_N_ROUNDS_256 + 1)

#define AES_MODE_128            0x1
#define AES_MODE_256            0x2
#define AES_MODE_ENCRYPT        0x10
#define AES_MODE_DECRYPT        0x20

#define AES_KEYLEN_128          16
#define AES_KEYLEN_256          32

typedef struct aes_key_block {
    unsigned    mode;
    uint64_t    round_keys_encrypt[AES_N_ROUND_KEYS_256 * 2];
    uint64_t    round_keys_decrypt[AES_N_ROUND_KEYS_256 * 2];
} aes_key_block;

extern unsigned aes_encrypt_ecb (const uint8_t * src, uint8_t * dst, unsigned nbytes,
                                 const aes_key_block * key_block);

extern unsigned aes_decrypt_ecb (const uint8_t * src, uint8_t * dst, unsigned nbytes,
                                 const aes_key_block * key_block);

extern unsigned aes_init_key_block (const uint8_t * key, unsigned mode,
                                    aes_key_block * key_block);

#endif  /* AES_H_ */
