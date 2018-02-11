//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice, 
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, 
//    this list of conditions and the following disclaimer in the documentation 
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
// 
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
// 
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��:���ܺ�����
//���ߣ�mr.logic
//�汾��V1.0.0
//�ļ�����:CRCУ�鲿��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "arch_feature.h"
#include <djyfs/iofile.h>
#include "verify.h"
static const u16 crc16_tab[] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
};

const u8 ecc_table[] = {
0x00,0x55,0x59,0x0c,0x65,0x30,0x3c,0x69,0x69,0x3c,0x30,0x65,0x0c,0x59,0x55,0x00,
0x95,0xc0,0xcc,0x99,0xf0,0xa5,0xa9,0xfc,0xfc,0xa9,0xa5,0xf0,0x99,0xcc,0xc0,0x95,
0x99,0xcc,0xc0,0x95,0xfc,0xa9,0xa5,0xf0,0xf0,0xa5,0xa9,0xfc,0x95,0xc0,0xcc,0x99,
0x0c,0x59,0x55,0x00,0x69,0x3c,0x30,0x65,0x65,0x30,0x3c,0x69,0x00,0x55,0x59,0x0c,
0xa5,0xf0,0xfc,0xa9,0xc0,0x95,0x99,0xcc,0xcc,0x99,0x95,0xc0,0xa9,0xfc,0xf0,0xa5,
0x30,0x65,0x69,0x3c,0x55,0x00,0x0c,0x59,0x59,0x0c,0x00,0x55,0x3c,0x69,0x65,0x30,
0x3c,0x69,0x65,0x30,0x59,0x0c,0x00,0x55,0x55,0x00,0x0c,0x59,0x30,0x65,0x69,0x3c,
0xa9,0xfc,0xf0,0xa5,0xcc,0x99,0x95,0xc0,0xc0,0x95,0x99,0xcc,0xa5,0xf0,0xfc,0xa9,
0xa9,0xfc,0xf0,0xa5,0xcc,0x99,0x95,0xc0,0xc0,0x95,0x99,0xcc,0xa5,0xf0,0xfc,0xa9,
0x3c,0x69,0x65,0x30,0x59,0x0c,0x00,0x55,0x55,0x00,0x0c,0x59,0x30,0x65,0x69,0x3c,
0x30,0x65,0x69,0x3c,0x55,0x00,0x0c,0x59,0x59,0x0c,0x00,0x55,0x3c,0x69,0x65,0x30,
0xa5,0xf0,0xfc,0xa9,0xc0,0x95,0x99,0xcc,0xcc,0x99,0x95,0xc0,0xa9,0xfc,0xf0,0xa5,
0x0c,0x59,0x55,0x00,0x69,0x3c,0x30,0x65,0x65,0x30,0x3c,0x69,0x00,0x55,0x59,0x0c,
0x99,0xcc,0xc0,0x95,0xfc,0xa9,0xa5,0xf0,0xf0,0xa5,0xa9,0xfc,0x95,0xc0,0xcc,0x99,
0x95,0xc0,0xcc,0x99,0xf0,0xa5,0xa9,0xfc,0xfc,0xa9,0xa5,0xf0,0x99,0xcc,0xc0,0x95,
0x00,0x55,0x59,0x0c,0x65,0x30,0x3c,0x69,0x69,0x3c,0x30,0x65,0x0c,0x59,0x55,0x00,
};

u32 ecc_count_bits(u8 x)
{
    u32 r = 0;
    
#if (CN_BYTE_BITS == 32)
    x &= 0xFF;
#endif//#if (CN_BYTE_BITS == 32)

    while(x)
    {
        if(x & 1) r++;
        x >>= 1;
    }
    return r;
}

//----����eccֵ----------------------------------------------------------------
//����: ����eccЧ����
//����: data : ҪЧ��������׵�ַ
//      ecc : ���eccЧ����׵�ַ
//����: ��
//----------------------------------------------------------------------------
void Ecc_Make256(const u8 *data,u8 *ecc)
{
    u32 i;
    u8 col_parity = 0;
    u8 line_parity = 0;
    u8 line_parity_prime = 0;
    u8 t;
    u8 b;

    for(i = 0; i < 256; i++)
    {
        b = ecc_table[*data++];
        col_parity ^= b;

        if(b & 0x01)
        {
            line_parity ^= i;
            line_parity_prime ^= ~i;
        }

    }

    ecc[2] = (~col_parity) | 0x03;

    t = 0;
    if(line_parity       & 0x80) t |= 0x80;
    if(line_parity_prime & 0x80) t |= 0x40;
    if(line_parity       & 0x40) t |= 0x20;
    if(line_parity_prime & 0x40) t |= 0x10;
    if(line_parity       & 0x20) t |= 0x08;
    if(line_parity_prime & 0x20) t |= 0x04;
    if(line_parity       & 0x10) t |= 0x02;
    if(line_parity_prime & 0x10) t |= 0x01;
    ecc[1] = ~t;

    t = 0;
    if(line_parity       & 0x08) t |= 0x80;
    if(line_parity_prime & 0x08) t |= 0x40;
    if(line_parity       & 0x04) t |= 0x20;
    if(line_parity_prime & 0x04) t |= 0x10;
    if(line_parity       & 0x02) t |= 0x08;
    if(line_parity_prime & 0x02) t |= 0x04;
    if(line_parity       & 0x01) t |= 0x02;
    if(line_parity_prime & 0x01) t |= 0x01;
    ecc[0] = ~t;

    t = ecc[0];
    ecc[0] = ecc[1];
    ecc[1] = t;
}


//----ecc��Ⲣ��������-------------------------------------------------------
//����: ecc��Ⲣ��������
//����: data : ��������
//      old_ecc : ֮ǰЧ���ecc
//����: 0 : ��ȷ
//      1 : ���ݱ�����
//      2 : ֮ǰЧ���ecc�Ǵ����
//      -1: �����޷��ָ�
//----------------------------------------------------------------------------
s32 Ecc_Corect256(u8 *data, const u8 *old_ecc)
{
    u8 d0, d1, d2;
    u8 new_ecc[3];
    //��data����ecc���㣬�����µ�ecc
    Ecc_Make256(data,new_ecc);

    // �¾�ecc����������㣬�������1��˵��������
    d0 = old_ecc[0] ^ new_ecc[0];
    d1 = old_ecc[1] ^ new_ecc[1];
    d2 = old_ecc[2] ^ new_ecc[2];
    if((d0 | d1 | d2) == 0)
    {
        return 0;
    }

    if( ((d0 ^ (d0 >> 1)) & 0x55) == 0x55 )
    if( ((d1 ^ (d1 >> 1)) & 0x55) == 0x55 )
    if( ((d2 ^ (d2 >> 1)) & 0x54) == 0x54 )
    {
        unsigned byte;
        unsigned bit;
        u8 t;

        t = d0;
        d0 = d1;
        d1 = t;
        bit = byte = 0;

        if(d1 & 0x80) byte |= 0x80;
        if(d1 & 0x20) byte |= 0x40;
        if(d1 & 0x08) byte |= 0x20;
        if(d1 & 0x02) byte |= 0x10;
        if(d0 & 0x80) byte |= 0x08;
        if(d0 & 0x20) byte |= 0x04;
        if(d0 & 0x08) byte |= 0x02;
        if(d0 & 0x02) byte |= 0x01;

        if(d2 & 0x80) bit |= 0x04;
        if(d2 & 0x20) bit |= 0x02;
        if(d2 & 0x08) bit |= 0x01;

        data[byte] ^= (1 << bit);

        return 1;
    }

    if((ecc_count_bits(d0)+ecc_count_bits(d1)+ecc_count_bits(d2)) == 1)
    {
        return 2;
    }
    else
    {
        return -1;
    }
}

u16 crc16(u8 *buf, u32 len)
{
    u32 i;
    u16 cksum;

    cksum = 0;
    for (i = 0;  i < len;  i++) {
    cksum = crc16_tab[((cksum>>8) ^ *buf++) & 0xFF] ^ (cksum << 8);
}
    
#if (CN_BYTE_BITS == 32)
    cksum &= 0x0000ffff;
#endif	//#if (CN_BYTE_BITS == 32)

    return cksum;
}



//----CRC32��ѯ��-----------------------------------------------------------
//add by zhb 20170803
//------------------------------------------------------------------------------
const u32 g_CRC32_tab[] = {                            // CRC polynomial 0xedb88320
0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

//----����-----------------------------------------------------------
//����: �ӻ��λ������������ɸ��ֽ�,����ʵ�ʶ�����������,�����ƶ���ָ�����
//      �����������ݲ��㣬��ʵ��������ȡ��
//����: ring,Ŀ�껷�λ������ṹָ��
//      buffer,�������ݵĻ�����ָ��
//      len,�����������ݳ���.��λ���ֽ���
//����: ʵ�ʶ������ֽ���,������������㹻������,=len
//------------------------------------------------------------------------------
#define BUFSIZE     4096

uint32_t crc32_buf(char *buf,int len)
{
	int   i;
	uint32_t ret;
	ret = 0xFFFFFFFF;
	for(i = 0; i < len;i++)
	{
		 ret = g_CRC32_tab[(ret ^buf[i] ) &0xFF] ^ (ret >> 8);
	}
	ret = ~ret;
	return ret;
}

//---------------------------------------------------------------
//����: ��һ��������ݽ���CRC32У��ʱ���ڴ滺��������ʱ���ɽ�������ݲ�ֳ����ɸ�С�飬���ν���У��
//      ���յõ������������CRC32У����.
//����:  crc,��һ�ν���У��õ���У���룬��һ�ε���ʱ��ֵ�����壬������Ϊ0;
//      buf,��У������ݻ�������ָ��
//      len,��У������ݳ���.��λ���ֽ���
//      lastflg,���һ�����У��ı�ʶ��
//      startflg,��һ�����У��ı�ʶ��
//����: ʵ�ʶ������ֽ���,������������㹻������,=len
//------------------------------------------------------------------------------
u32 cal_crc32( u32 crc,char *buf, int len,bool_t startflg,bool_t lastflg)
{
	int   i;
	u32 ret;
	if(startflg)
	{
        ret = 0xFFFFFFFF;
	}
	else
	{
		ret=crc;
	}

    for(i = 0; i < len;i++)
    {
         ret = g_CRC32_tab[(ret ^buf[i] ) &0xFF] ^ (ret >> 8);
    }
    if(lastflg)
    {
       ret = ~ret;
    }
    return ret;
}


static int cal_file_crc(const char *in_file,uint32_t *pcrc)
{
	int fd,nread;
	uint32_t tmp_crc;
	static bool_t sflag=false,lflag=false;
	char buf[BUFSIZE];
	uint32_t crc;
	fd=open(in_file,O_RDONLY);
	if(fd<0)
	{
		printf("%s file open failed.\r\n",__FUNCTION__);
		return -1;
	}
	while(nread=read(fd,buf,BUFSIZE)>0)
	{
		if(!sflag)
		{
			if(nread==BUFSIZE)
			{
			   tmp_crc=cal_crc32(0,buf,BUFSIZE,true,false);
			}
			else
			{
			   tmp_crc=cal_crc32(0,buf,nread,true,true);
			}
			sflag=true;
		}
		else
		{
          if(nread!=BUFSIZE)
          {
        	  tmp_crc=cal_crc32(tmp_crc,buf,nread,false,true);
        	  lflag=true;
          }
          else
          {
        	  tmp_crc=cal_crc32(tmp_crc,buf,nread,false,false);
          }
		}

	}
	crc=tmp_crc;
	if(!lflag)
	{
		crc=~crc;
	}
	*pcrc=crc;
	return 0;

}

// =============================================================================
// ���ܣ���������ʼ��DjyBus���߸��ڵ㣬�����������ͽڵ�ĸ����
// ������type,����Դ���ͣ�EN_ARRAY_TYPE��ʾΪ���飬EN_FILE_TYPE��ʾΪ�ļ���
//     source,��У������Դ������ָ������ļ���;
//     len,��У������Դ���ȣ��������Դ��EN_FILE_TYPE����Ĳ�����Ч����ֱ������NULL;
//     pcrc,�洢У������ָ��.
// ���أ��ɹ�У���򷵻�0,ʧ���򷵻�-1.
// =============================================================================
int crc32(uint8_t type,char *source,ptu32_t len,uint32_t *pcrc)
{
	uint32_t crc;
	int ret;
	if((source==NULL)||(pcrc==NULL))
	{
		printf("%s para is invalid.\r\n",__FUNCTION__);
		return -1;
	}
	if(type==EN_ARRAY_TYPE)
	{
		crc=crc32_buf(source,len);
		*pcrc=crc;
		return 0;
	}
	else if(type==EN_FILE_TYPE)
	{
		ret=cal_file_crc(source,pcrc);
		return ret;
	}
	else
		return -1;
}

// ============================================================================
// ���ܣ�CRC�ֲ������ʼ��
// ������pCRC -- CRCֵ
// ���أ�0 -- ʧ�ܣ�-1 -- ʧ�ܣ�
// ��ע��
// ============================================================================
s32 crc32Init(u32 *pCRC)
{
	if(!pCRC)
		return (0);

	*pCRC = -1;
	return (-1);
}

// ============================================================================
// ���ܣ�CRC�ֲ�����
// ������pCRC -- CRCֵ��pBuf -- ���ݣ�dwLen -- ���ݳ��ȣ�
// ���أ�0 -- ʧ�ܣ�-1 -- ʧ�ܣ�
// ��ע��
// ============================================================================
s32 crc32Run(u32 *pCRC, u8 *pBuf, u32 dwLen)
{
	u32 i;
	u8 *buf;
	u32 crc;

	if((!pCRC) || (!pBuf))
		return (0);

	crc = *pCRC;
	buf = pBuf;

	for(i = 0; i < dwLen;i++)
	{
		crc = g_CRC32_tab[(crc ^buf[i] ) &0xFF] ^ (crc >> 8);
	}

	*pCRC = crc;
	return (-1);
}

// ============================================================================
// ���ܣ�CRC�ֲ������˳�
// ������pCRC -- CRCֵ��
// ���أ�0 -- ʧ�ܣ�-1 -- ʧ�ܣ�
// ��ע��
// ============================================================================
s32 crc32Exit(u32 *pCRC)
{
	u32 crc;

	if(!pCRC)
		return (0);

	crc = *pCRC;
	crc = ~crc;
	*pCRC = crc;

	return (-1);
}
