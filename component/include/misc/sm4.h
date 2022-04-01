/**
 * \file sm4.h
 */
#ifndef XYSSL_SM4_H
#define XYSSL_SM4_H

#include <stdint.h>
#define SM4_ENCRYPT     1
#define SM4_DECRYPT     0

/**
 * \brief          SM4 context structure
 */
typedef struct
{
    s32 mode;                   /*!<  encrypt/decrypt   */
    u32 sk[32];       /*!<  SM4 subkeys       */
}sm4_context;


#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief          SM4-ECB block encryption/decryption
 * \param ctx      SM4 context
 * \param mode     SM4_ENCRYPT or SM4_DECRYPT
 * \param length   length of the input data
 * \param input    input block
 * \param output   output block
 */
void sm4_encrypt_ecb128( u8 *key,
                    s32 length,
                    u8 *input,
                    u8 *output);
u32 sm4_decrypt_ecb128( u8 *key,
                    s32 length,
                    u8 *input,
                    u8 *output);

/**
 * \brief          SM4-CBC buffer encryption/decryption
 * \param ctx      SM4 context
 * \param mode     SM4_ENCRYPT or SM4_DECRYPT
 * \param length   length of the input data
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 */
void sm4_crypt_cbc( sm4_context *ctx,
                     s32 mode,
                     s32 length,
                     u8 iv[16],
                     u8 *input,
                     u8 *output );

#ifdef __cplusplus
}
#endif

#endif /* sm4.h */
