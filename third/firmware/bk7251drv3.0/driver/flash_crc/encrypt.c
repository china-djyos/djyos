/**
 * @file   encrypt.c
 * @author nRzo <nrzo@nrzo-laptop>
 * @date   Mon Dec 12 21:16:14 2011
 *
 * @brief
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abc.h"
#include "sys_config.h"
#if CFG_SUPPORT_DJYOS   //CK
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define ENCRYPT_SIZE   0X04
#define CRC_SIZE       0X02
#define CRC_PAKEIT     0X20
/* #define ENCRYPT_CODE   0x12345678 */

#pragma pack(push, 1)
struct encrypt_packet
{
    u8 data[CRC_PAKEIT];
    struct
    {
        u16 bit8  : 1;
        u16 bit9  : 1;
        u16 bit10 : 1;
        u16 bit11 : 1;
        u16 bit12 : 1;
        u16 bit13 : 1;
        u16 bit14 : 1;
        u16 bit15 : 1;
        u16 bit0  : 1;
        u16 bit1  : 1;
        u16 bit2  : 1;
        u16 bit3  : 1;
        u16 bit4  : 1;
        u16 bit5  : 1;
        u16 bit6  : 1;
        u16 bit7  : 1;
    };
};
#pragma pack(pop)

/**
 * Convert string to integer data.
 * @param str: String to be converted
 * @return Result of the conversion
 */
int str2int(char *str)
{
    int result = 0;
    int negative = TRUE;

    while((*str != '\0') && (*str != ' '))
    {
        if (*str == '-')
            negative = FALSE;
        else if((*str >= '0') && (*str <= '9'))
            result = result * 16 + ((*str++) - '0'); /**< hex fomat */
        else if ((*str >= 'a') && (*str <= 'f'))
            result = result * 16 + 10 + (*(str++) - 'a');
        else if ((*str >= 'A') && (*str <= 'F'))
            result = result * 16 + 10 + (*(str++) - 'A');
        else
        {
            printf(" Error: Parameter failed!\n");
            result = 0x0;
            break;
        }
    }
    if (negative == FALSE)
        result = -result;

    return result;
}

u32 Get_FileSize (FILE *fp)
{
    u32 pos, file_size;

    pos = ftell( fp );			/**< Save the current position. */
    fseek (fp, 0L, SEEK_END);	/**< Jump to the end of the file. */
    file_size = ftell (fp);		/**< Get the end position. */
    fseek (fp, pos, SEEK_SET);	/**< Jump back to the original position. */

    return (file_size);
}

void calc_crc(u32 *buf, u32 packet_num)
{
    int i, bit, byte;
    for (i = 0; i < (int)packet_num; i++)
    {
        struct encrypt_packet *p = (struct encrypt_packet *)((u32)buf + i * sizeof(struct encrypt_packet));
        struct encrypt_packet bak;
        for (byte = 0; byte < CRC_PAKEIT; byte++)
        {
            for (bit = 7; bit >= 0; bit--)
            {
                u8 input;
                memcpy((char *)&bak, (char *)p, sizeof(struct encrypt_packet));
                input = (bak.data[byte] >> bit) & 0x01;
                p->bit15 =  bak.bit14 ^ input ^ bak.bit15;
                p->bit14 =  bak.bit13;
                p->bit13 =  bak.bit12;
                p->bit12 =  bak.bit11;
                p->bit11 =  bak.bit10;
                p->bit10 =  bak.bit9;
                p->bit9  =  bak.bit8;
                p->bit8  =  bak.bit7;
                p->bit7  =  bak.bit6;
                p->bit6  =  bak.bit5;
                p->bit5  =  bak.bit4;
                p->bit4  =  bak.bit3;
                p->bit3  =  bak.bit2;
                p->bit2  =  bak.bit1 ^ input ^ bak.bit15;
                p->bit1  =  bak.bit0;
                p->bit0  =  input ^ bak.bit15;
            }
        }
    }
}

#if 0
//这是博通提供的函数，用于把要算CRC校验码的数据，每隔32个字节空2个字节
//但是这存在一个问题addr有时候会出现不对齐的现象，比如addr会有等于34的情况。
//所以下面重新写了一个函数。
void encrypt(u32 *rx, u8 *tx, u32 num)
{
    u32 i, j, addr;
    u32 len = CRC_PAKEIT / sizeof(u32);

    for (i = 0; i < num; i++)
    {
        for (j = 0; j < len; j++)
        {
            addr = i * (CRC_PAKEIT + sizeof(u16)) + j * sizeof(u32);
            *(u32 *)(&tx[addr]) = enc_data_my(i * CRC_PAKEIT + j * sizeof(u32), *rx++);
        }
    }
}
#else
#include <endian.h>
void encrypt(u32 *rx, u8 *tx, u32 num)
{
    u32 i, j, addr;
    u32 len = CRC_PAKEIT / sizeof(u32);

    for (i = 0; i < num; i++)
    {
        for (j = 0; j < len; j++)
        {
            addr = i * (CRC_PAKEIT + sizeof(u16)) + j * sizeof(u32);
            fill_little_32bit(&tx[addr], 0, enc_data_my(i * CRC_PAKEIT + j * sizeof(u32), *rx++));
        }
    }
}
#endif
/**
 * @param argc
 * @param argv
 * @param envp
 *
 * @return ture or false
 */
int encrypt_main (int argc, char *argv[], char *envp[])
{
    u32 i, read_file_size = 0, write_file_size = 0, 
		   extend_buf_size = 0, packet_num = 0;
    u8 *rbuf = NULL, *wbuf = NULL, *text = NULL;
    FILE *fp_read = NULL, *fp_write_bin = NULL, 
		 *fp_write_txt = NULL;
    char out_file_bin[256] = {0};
    char out_file_txt[256] = {0};

    if (argc != 6)
    {
        printf ("Usage: encrypt 3231s.bin 3231.out\n");
        return -1;
    }

    fp_read = fopen (argv[1], "rb");
    if (fp_read == NULL)
    {
        printf ("Can't open file %s.\n", argv[1]);
        return -1;
    }

    strcpy(out_file_bin, argv[1]);
    strcpy(out_file_txt, argv[1]);
    i = strlen(out_file_bin);
    if ((out_file_bin[i - 4] == '.')
            && (out_file_bin[i - 3] == 'b')
            && (out_file_bin[i - 2] == 'i')
            && (out_file_bin[i - 1] == 'n'))
    {
        out_file_bin[i - 4] = '_';
        out_file_bin[i - 3] = 'c';
        out_file_bin[i - 2] = 'r';
        out_file_bin[i - 1] = 'c';
        out_file_bin[i]   = '.';
        out_file_bin[i + 1] = 'b';
        out_file_bin[i + 2] = 'i';
        out_file_bin[i + 3] = 'n';

        out_file_txt[i - 3] = 'o';
        out_file_txt[i - 2] = 'u';
        out_file_txt[i - 1] = 't';
    }
    else
    {
        strcat(out_file_bin, "_crc.bin");
        strcat(out_file_txt, ".out");
    }
    fp_write_bin = fopen (out_file_bin, "wb");
    if (fp_write_bin == NULL)
    {
        fclose (fp_read);
        printf ("Can't open file %s.\n", out_file_bin);
        return -1;
    }
    fp_write_txt = fopen (out_file_txt, "wb");
    if (fp_write_txt == NULL)
    {
        fclose (fp_read);
        fclose (fp_write_bin);
        printf ("Can't open file %s.\n", out_file_txt);
        return -1;
    }

    coef0 = (u32)str2int(argv[2]);
    coef1 = (u32)str2int(argv[3]);
    coef2 = (u32)str2int(argv[4]);
    coef3 = (u32)str2int(argv[5]);

    /// Step 1: 将bin文件结尾补充FF到4Byte的整数倍
    read_file_size = Get_FileSize (fp_read);

    if (read_file_size % CRC_PAKEIT)
        extend_buf_size = (read_file_size + CRC_PAKEIT) - (read_file_size % CRC_PAKEIT);
    else
        extend_buf_size = read_file_size;

    packet_num = (extend_buf_size / CRC_PAKEIT);

    /*每一个 packet占4字节， 而算crc是32个byte算一次*/

    write_file_size = extend_buf_size + packet_num * CRC_SIZE;

    rbuf = malloc (extend_buf_size);
    wbuf = malloc (write_file_size);
    text = malloc (write_file_size * 4);
    memset(rbuf, 0xff, extend_buf_size);
    memset(wbuf, 0xff, write_file_size);

    fread(rbuf, 1, read_file_size, fp_read);
    fclose (fp_read);

    /// Step 2: 将补充后的bin文件按4Byte分割加密，按其地址加密
    encrypt((u32 *)rbuf, wbuf, packet_num);

    /// Step 3: 将加密后的bin文件每32Byte为一整体进行CRC运算，低字节先输入，字节中最高位先输入
    calc_crc((u32 *)wbuf, packet_num);

    /// string
    for (i = 0; i < write_file_size; i++)
    {
        char str[5] = {0};
        sprintf(str, "%02x\r\n", wbuf[i]);
        memcpy(&text[i * 4], str, strlen(str));
    }

    fwrite(wbuf, 1, write_file_size, fp_write_bin);
    fwrite(text, 1, write_file_size * 4, fp_write_txt);
    fclose(fp_write_bin);
    fclose(fp_write_txt);

    free(rbuf);
    free(wbuf);
    free(text);

    printf (" Encrypt Seccessfully!\r\n");
    printf (" -FileSize: 0x%04x\r\n", write_file_size);

    return 0;
}
#endif