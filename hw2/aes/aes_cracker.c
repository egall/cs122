/*
 * This code encrypts input data using the Rijndael (AES) cipher.  The
 * key length can be either 128 or 256 bits.
 *
 * Author: Erik Steggall (esteggall@soe.ucsc.du)
 * Based on code from Ethan L. Miller (elm@cs.ucsc.edu)
 * Based on code from Philip J. Erdelsky (pje@acm.org)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <xlocale.h>
#include "aes.h"

#define AES_MODE_CBC    0x1000
#define AES_MODE_CTR    0x2000
#define AES_MODE_ECB    0x4000
#define AES_MODE_ALL_MODES (AES_MODE_CBC | AES_MODE_CTR | AES_MODE_ECB)

int
main(int argc, char **argv)
{
    aes_key_block       key_block;
    uint8_t             key[AES_KEYLEN_256];
    int                 ch;
    int                 flags = AES_MODE_CBC | AES_MODE_ENCRYPT;
    char                buf[256];
    uint64_t            in_buf[0x400];
    uint64_t            out_buf[0x400];
    uint64_t            prev_buf[0x400];
    FILE *              in_fp = stdin;
    FILE *              out_fp = stdout;
    int                 i, c, l;
    uint32_t            ctr;
    int                 ret;
    int                 z = 0, itor = 0, jtor = 0, ktor = 0, ltor = 0;

   memset (key, 0, AES_KEYLEN_128);

   for(ltor = 0; ltor < 256; ltor++){ 
       for(ktor = 0; ktor < 256; ktor++){
           for(jtor = 0; jtor < 256; jtor++){
                for(itor = 0; itor < 256; itor++){
                    aes_key_block       my_key_block;
                    key[15] = itor;
                    key[14] = jtor;
                    key[13] = ktor;
                    key[12] = ltor;
                    flags &= ~AES_MODE_ENCRYPT;
                    flags |= AES_MODE_DECRYPT;
                    flags |= AES_MODE_128;
        
                    ret = aes_init_key_block (key, flags & (AES_MODE_128 
                                                            | AES_MODE_DECRYPT), &key_block);
                    z = 0;
                    memset (out_buf, 0, AES_BLOCKSIZE);
                    /* Zero buffer in case it's not filled */
                    memset (in_buf, 0, AES_BLOCKSIZE);
                    l = fread (in_buf, 1, AES_BLOCKSIZE, in_fp);
                    if (l == 0) {
                        break;
                    }
                    ret = aes_decrypt_ecb ((uint8_t *)in_buf, (uint8_t *)out_buf, AES_BLOCKSIZE,
                                           &key_block);
                    fwrite (out_buf, 1, AES_BLOCKSIZE, out_fp);
                    if (z == 0){
                        if(out_buf[0] == 85966670672){
                            fprintf(stderr, "outbuf = %llu\n", out_buf[0]);
                            fprintf(stderr, "truf\n");
                            fprintf(stderr, "k = %02x %02x %02x %02x\n", key[12], key[13], key[14], key[15]);
                            return 0;
                        }
                    }
                    z++;
                    rewind(in_fp);
                }
            }
        }
    }
}
