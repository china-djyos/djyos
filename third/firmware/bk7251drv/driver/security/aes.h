#ifndef _BK_AES_H_
#define _BK_AES_H_

#if (CFG_SOC_NAME == SOC_BK7221U)

#include "security_reg.h"


enum AES_MODE
{
    AES128      = 0,
    AES256      = 1,
    AES192      = 2
};

enum AES_ENCODE
{
    DECODE      = 0,
    ENCODE      = 1
};



typedef struct
{
    unsigned long           aes_key[8];
    unsigned long          *aes_block;

    /* mode:   AES mode
        AES128      = 0,
        AES256      = 1,
        AES192      = 2
     */
    unsigned char           mode;

    /* encode: encode or decode
        decode      = 0,
        encode      = 1
     */
    unsigned char           encode;
} SECURITY_AES_DRV_DESC;

void *hal_aes_init(const u8 *key, size_t key_size);
void hal_aes_encrypt(void *ctx, const u8 *plain, u8 *cipher);
void hal_aes_decrypt(void *ctx, const u8 *cipher, u8 *plain);
void hal_aes_deinit(void *ctx);

#endif
#endif

