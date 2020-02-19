//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <cpu_peri.h>
#include <driver_info_to_fs.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

/** Swap the bytes in an uint16_t: much like lwip_htons() for little-endian */
#ifndef SWAP_BYTES_IN_WORD
#define SWAP_BYTES_IN_WORD(w) (((w) & 0xff) << 8) | (((w) & 0xff00) >> 8)
#endif /* SWAP_BYTES_IN_WORD */

#ifndef FOLD_U32T
#define FOLD_U32T(u)          (((u) >> 16) + ((u) & 0x0000ffffUL))
#endif


uint16_t wlan_standard_chksum(const void *dataptr, int len)
{
  const uint8_t *pb = (const uint8_t *)dataptr;
  const uint16_t *ps;
  uint16_t t = 0;
  uint32_t sum = 0;
  int odd = ((uint32_t)pb & 1);

  /* Get aligned to uint16_t */
  if (odd && len > 0) {
    ((uint8_t *)&t)[1] = *pb++;
    len--;
  }

  /* Add the bulk of the data */
  ps = (const uint16_t *)(const void *)pb;
  while (len > 1) {
    sum += *ps++;
    len -= 2;
  }

  /* Consume left-over byte, if any */
  if (len > 0) {
    ((uint8_t *)&t)[0] = *(const uint8_t *)ps;
  }

  /* Add end bytes */
  sum += t;

  /* Fold 32-bit sum to 16 bits
     calling this twice is probably faster than if statements... */
  sum = FOLD_U32T(sum);
  sum = FOLD_U32T(sum);

  /* Swap if alignment was odd */
  if (odd) {
    sum = SWAP_BYTES_IN_WORD(sum);
  }

  return (uint16_t)sum;
}

int wlan_fast_connect_info_read(wlan_fast_connect_t *data_info)
{
    int result = 0;
    uint32_t crc1, crc2;
    uint32_t data[(sizeof(struct wlan_fast_connect) + 4 - 1) / 4 + 1 ];

    if(data_info==NULL)
        return -1;

    memset(data, 0, sizeof(struct wlan_fast_connect));
//    djy_flash_read(FLASH_FAST_DATA_ADDR, data, sizeof(data));
    GetNameValueFS(CFG_FAST_DATA_FILE_NAME,data,sizeof(data));
    crc1 = data[(sizeof(struct wlan_fast_connect) + 4 - 1 ) / 4];
    crc2 = wlan_standard_chksum(data, sizeof(struct wlan_fast_connect));
    if ((data[0] != ~0x0)   && (crc1 == crc2)) // 0xFFFFFFFF
    {
        memcpy(data_info, data, sizeof(struct wlan_fast_connect));
        result = 0;
    }
    else
    {
        printf("fast_connect ap info crc failed\r\n");
        result = -1;
    }
    if( (data_info->channel < 1) || (data_info->channel > 13) )
    {
        printf("[%s] channel out of range! \n", __FUNCTION__);
        result = -2;
    }
    return result;
}

int wlan_fast_connect_info_write(wlan_fast_connect_t *data_info)
{
    uint32_t crc1, crc2;
    uint32_t data[(sizeof(struct wlan_fast_connect) + 4 - 1) / 4 + 1 ];
    if(data_info==NULL)
        return -1;
    memset(data, 0, sizeof(data));
//    djy_flash_read(FLASH_FAST_DATA_ADDR, data, sizeof(data));
    GetNameValueFS(CFG_FAST_DATA_FILE_NAME,data,sizeof(data));
    crc1 = wlan_standard_chksum(data_info, sizeof(struct wlan_fast_connect));
    crc2 = data[(sizeof(struct wlan_fast_connect) + 4 - 1 ) / 4];
    if ((memcmp(data, (uint8_t *) data_info, sizeof(struct wlan_fast_connect)) != 0) || (crc1 != crc2))
    {
        memcpy(data, data_info, sizeof(struct wlan_fast_connect));
        data[(sizeof(struct wlan_fast_connect) + 4 - 1 ) / 4] = crc1;
//        flash_protection_op(0,FLASH_PROTECT_NONE);
//        djy_flash_erase(FLASH_FAST_DATA_ADDR);
//        djy_flash_write(FLASH_FAST_DATA_ADDR, (uint8_t *)data, sizeof(struct wlan_fast_connect) + 4);
//        flash_protection_op(0,FLASH_PROTECT_ALL);
        SetNameValueFS(CFG_FAST_DATA_FILE_NAME,data,sizeof(struct wlan_fast_connect) + 4);
    }
    return 0;
}


