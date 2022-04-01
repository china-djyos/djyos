/*
 * SM4/SMS4 algorithm test programme
 * 2012-4-21
 */

#include <string.h>
#include <stdio.h>
#include "sm4.h"

int main()
{
    unsigned char key[16] = "1234567890abcdef";
    unsigned char input[128] = "123456789012345678901234567890";
    unsigned char output[144];
    sm4_context ctx;
    unsigned long i,n=1;

    while(1)
    {
        sm4_encrypt_ecb128(key,n,input,output);
        sm4_decrypt_ecb128(key,(n/16+1)*16,output,output);
    }
    //decrypt 10000 times testing vector based on standards.
    i = 0;
    while (i<10000)
    {
        sm4_encrypt_ecb128(key,n,input,output);
        i++;
    }

    return 0;
}

