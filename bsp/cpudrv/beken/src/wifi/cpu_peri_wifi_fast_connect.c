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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <cpu_peri.h>
#include <driver_info_to_fs.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

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


