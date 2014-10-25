/*
 * This code encrypts input data using the Rijndael (AES) cipher.  The
 * key length can be either 128 or 256 bits.
 *
 * Author: Ethan L. Miller (elm@cs.ucsc.edu)
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

/***********************************************************************
 *
 * hexvalue
 *
 * This routine takes a single character as input, and returns the
 * hexadecimal equivalent.  If the character passed isn't a hex value,
 * the function returns -1.
 *
 ***********************************************************************
 */
static
int
hexvalue (char c)
{
    c = toupper (c);
    if (c >= '0' && c <= '9') {
        return (c - '0');
    } else if (c >= 'A' && c <= 'F') {
        return (10 + c - 'A');
    } else {
        return (-1);
    }
}


/***********************************************************************
 *
 * getpassword
 *
 * Get the key from the password.  The key is specified as a string of
 * hex digits, two per key byte.  password points at the character
 * currently being added to the key.  If it's '\0', the key is done.
 *
 ***********************************************************************
 */
int
get_hexkey (const char *hexkey, unsigned char *key, int keylen)
{
    int		i, kp, hk_len, r;
    unsigned char v;

    memset (key, 0, keylen);
    hk_len = strlen (hexkey);

    /* Fill in the key backwards */

    for (i = hk_len - 1, kp = keylen - 1; i >= 0; i -= 2, kp--) {
        r = hexvalue (hexkey[i]);
        if (r < 0) {
            return -1;
        }
        v = (unsigned char)r;
        if (i > 0) {
            r = hexvalue (hexkey[i-1]);
            if (r < 0) {
                return -1;
            }
            v |= (unsigned char)(r << 4);
        }
        key[kp] = v;
    }
    return hk_len;
}

struct option long_options[] = {
    {"encrypt",    no_argument, 0, 'e' },
    {"decrypt",    no_argument, 0, 'd' },
    {"mode",       required_argument, 0, 'm'},
    {"128",        required_argument, 0, '1'},
    {"256",        required_argument, 0, '2'},
    {"test",       no_argument, 0, 't'},
    {0,            0,                 0, 0   }
};

struct help_msgs {
    const char *        opts;
    const char *        msg;
} my_help_msgs[] = {
    {"-e, --encrypt", "Encrypt data"},
    {"-d, --decrypt", "Decrypt data"},
    {"-k, --key <KEY>", "Encryption key in hex (left padded with zeros)"},
    {"-m, --mode <MODE>", "Ciphertext mode (ECB / CBC / CTR).  Default is CBC."},
    {"-2, --256", "Key for 256-bit encryption (left padded with zeros)"},
    {"-1, --128", "Key for 128-bit encryption (left padded with zeros)"},
};

void
print_help (const char * prog, unsigned do_exit)
{
    fprintf (stderr, "Usage: %s [options]\n", prog);
    for (unsigned i = 0; i < sizeof (my_help_msgs) / sizeof (my_help_msgs[0]); ++i) {
        fprintf (stderr, "\t%-20s: %s\n", my_help_msgs[i].opts, my_help_msgs[i].msg);
    }
    if (do_exit) {
        exit (1);
    }
}

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
    int                 z = 0, itor = 0;

    while (1) {
        c = getopt_long (argc, argv, "edm:1:2:", long_options, NULL);

        if (c == -1) {
            break;
        }

        switch (c) {
        case 'e':
            flags &= ~AES_MODE_DECRYPT;
            flags |= AES_MODE_ENCRYPT;
            break;
        case 'd':
            flags &= ~AES_MODE_ENCRYPT;
            flags |= AES_MODE_DECRYPT;
            break;
        case '1':
            if (get_hexkey (optarg, key, AES_KEYLEN_128) < 0) {
                fprintf (stderr, "%s: Bad hex key %s\n", argv[0], optarg);
                print_help (argv[0], 1);
            }
            flags |= AES_MODE_128;
            break;
        case '2':
            if (get_hexkey (optarg, key, AES_KEYLEN_256) < 0) {
                fprintf (stderr, "%s: Bad hex key %s\n", argv[0], optarg);
                print_help (argv[0], 1);
            }
            flags |= AES_MODE_256;
            break;
        case 'm':
            flags &= ~AES_MODE_ALL_MODES;
            if (! strcmp (optarg, "CBC") || ! strcmp (optarg, "cbc")) {
                flags |= AES_MODE_CBC;
            } else if (! strcmp (optarg, "ECB") || ! strcmp (optarg, "ecb")) {
                flags |= AES_MODE_ECB;
            } else if (! strcmp (optarg, "CTR") || ! strcmp (optarg, "ctr")) {
                fprintf (stderr, "CTR mode not currently supported.\n");
                print_help (argv[0], 1);
            } else {
                fprintf (stderr, "Unrecognized mode %s.\n", optarg);
                print_help (argv[0], 1);
            }
            break;
        default:
            fprintf (stderr, "%s: option %c not recognized!\n", argv[0], c);
            print_help (argv[0], 1);
            break;
        }
    }

    if (! (flags & (AES_MODE_128 | AES_MODE_256))) {
        fprintf (stderr, "%s: must provide a key!\n", argv[0]);
        print_help (argv[0], 1);
    }
    
    for(itor = 0; itor < 256; itor++){
        /* Print the key, just in case */
        for (i = 0; i < (flags & AES_MODE_128 ? AES_KEYLEN_128 : AES_KEYLEN_256); i++) {
            sprintf (buf+2*i, "%02x", key[i]);
        }
        fprintf (stderr, "KEY: %s\n", buf);
        fprintf (stderr, "key: %s\n", buf);
        //
        key[15] = itor;
        fprintf(stderr, "itor = %02x\n key = %02x\n", itor, key[15]);
        ret = aes_init_key_block (key, flags & (AES_MODE_128 | AES_MODE_256 |
                                                AES_MODE_ENCRYPT | AES_MODE_DECRYPT), &key_block);
        z = 0;
        if (flags & (AES_MODE_ECB | AES_MODE_CBC)) {
            memset (out_buf, 0, AES_BLOCKSIZE);
            while (!feof (in_fp)) {
                /* Zero buffer in case it's not filled */
                memset (in_buf, 0, AES_BLOCKSIZE);
                l = fread (in_buf, 1, AES_BLOCKSIZE, in_fp);
                if (l == 0) {
                    break;
                }
                if (flags & AES_MODE_ENCRYPT) {
                    if (flags & AES_MODE_CBC) {
                        for (i = 0; i < AES_BLOCKSIZE / sizeof (uint64_t); ++i) {
                            in_buf[i] ^= out_buf[i];
                        }
                    }
                    ret = aes_encrypt_ecb ((uint8_t *)in_buf, (uint8_t *)out_buf, AES_BLOCKSIZE,
                                           &key_block);
                } else {
                    ret = aes_decrypt_ecb ((uint8_t *)in_buf, (uint8_t *)out_buf, AES_BLOCKSIZE,
                                           &key_block);
                    if (flags & AES_MODE_CBC) {
                        for (i = 0; i < AES_BLOCKSIZE / sizeof (uint64_t); ++i) {
                            out_buf[i] ^= prev_buf[i];
                            prev_buf[i] = in_buf[i];
                        }
                    }
                }
                fwrite (out_buf, 1, AES_BLOCKSIZE, out_fp);
                if (z == 0){
                    fprintf(stderr, "outbuf = %llu\n", out_buf[0]);
                    if(out_buf[0] == 85966670672){
                        fprintf(stderr, "truf\n");
                        }
                }
                z++;
            }
        } else if (flags & AES_MODE_CTR) {
            /* TBD */
        }
        rewind(in_fp);
        //
    }
}
