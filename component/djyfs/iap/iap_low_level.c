﻿//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <misc.h>
#include <systime.h>
#include <device/flash/flash.h> // will be obsolete
#include <device/include/unit_media.h>
#include <iap.h>

// ============================================================================
// 功能：写数据
// 参数：core -- IAP文件系统管理信息
//      data -- 数据缓冲
//      bytes -- 写字节数，不会超过一个缓存大小；
//      pos -- 数据地址
// 返回：成功（0）；失败（-1）；将要没有可写空间（-2）；
// 备注：当写到最后一个unit时，会尝试擦除
// ============================================================================
s32 __ll_write(struct __icore *core, u8 *data, u32 bytes, u32 pos)
{
    struct umedia *um = (struct umedia *)core->vol;
    struct uesz esz = {0};
    u32 j, offset, once, more;
    s32 left;
    s64 unit;
    u32 block = 0;

    left = bytes;
    unit = (pos >> um->usz) + core->MStart;
    offset = pos & ((1 << um->usz)-1); // unit内偏移
    um->mreq(lock, CN_TIMEOUT_FOREVER);
    while(left>0)
    {
#if 0
        // 如果当前写入页是一个块中的最后一页，则预先删除后续的sector
        // (page+1)用于防止格式化了不属于IAP的空间
        um->mreq(remain, (ptu32_t)&more, &unit);
        if(!more)
        {
            // +1是表示当前unit的后面一个
            if(((unit-um->ustart+1)<<um->usz)<um->asz)
            {
                struct uesz sz = {0};
                sz.unit = 1;
                um->merase(unit+1, sz); // 不管有没有擦除成功，因为如果后续写入的话，会有回读校验
            }
        }
#endif
        if(um->mread(unit, um->ubuf, um->opt))
        {
            um->mreq(unlock, 0); //
            return (-1);
        }

        once = 1<<um->usz;
        if(left<(s32)once)
            once = left;

        for(j=0; j<(once); j++)
        {
            if(um->ubuf[j+offset]!=data[j])
            {
                if(0xFF!=um->ubuf[j+offset])
                {
                    um->mreq(unlock, 0);
                    return (-1);
                }
            }
        }

        memcpy((um->ubuf + offset), data, once);
        if(um->mwrite(unit, um->ubuf, um->opt))
        {
            um->mreq(unlock, 0);
            return (-1);
        }

        left -= once;
        offset = 0;
        data += once;
        if(left)
            unit++;
    }

    // 如果当前写入页是一个块中的最后一页，则预先删除后续的sector
    // (page+1)用于防止格式化了不属于IAP的空间
    um->mreq(remain, (ptu32_t)&more, &unit);
    if(!more)
    {
        // +1是表示当前unit的后面一个
        if(((unit-core->MStart+1)<<um->usz) >= core->ASize)
        {
            return (-2);
        }
        esz.block = 1;
//        esz->unit = 0;
        um->mreq(whichblock, (ptu32_t)&block, &unit);
        //block+1是为了擦除下一个块
        um->mreq(format, block+1, block+1+1, &esz);
    }

    um->mreq(unlock, 0); //
    return (0);
}

// ============================================================================
// 功能：读数据
// 参数：core -- IAP文件系统管理信息
//      data -- 数据缓冲
//      bytes -- 读字节数
//      pos -- 数据地址
// 返回：成功（0）；失败（-1）；
// 备注：单次也就只会读一个unit，目前
// ============================================================================
s32 __ll_read(struct __icore *core, u8 *data, u32 bytes, u32 pos)
{
    struct umedia *um = (struct umedia *)core->vol;
    s64 unit;
    u32 offset;
    s32 left = bytes, once;

    unit = (pos >> um->usz) + core->MStart;
    offset = pos & ((1 << um->usz) - 1); // unit内偏移
    um->mreq(lock, CN_TIMEOUT_FOREVER);
    while(left>0)
    {
        once = MIN(((1 << um->usz) - offset), left);
        if(um->mread(unit, um->ubuf, um->opt))
        {
            um->mreq(unlock, 0); //
            return (-1);
        }

        memcpy(data, (um->ubuf + offset), once);
        left -= once;
        offset = 0;
        unit++;
        data += once;
    }

    um->mreq(unlock, 0); //
    return (0);
}

// ============================================================================
// 功能：擦除数据
// 参数：core -- IAP文件系统管理信息
//       dwBytes -- 字节数
//       dwAddr -- 数据地址
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 __ll_erase(struct __icore *core, u32 bytes, u32 pos)
{
    struct umedia *um = (struct umedia *)core->vol;
    struct uesz esz = {0};
    s64 unit;
    u32 erases, offset;
    s32 left = bytes;

    esz.unit = 1;
    unit = (pos >> um->usz) + core->MStart;
    offset = pos & ((1 << um->usz)-1); // unit内偏移
    um->mreq(lock, CN_TIMEOUT_FOREVER);
    while(left>0)
    {
        if(um->mreq(remain, (ptu32_t)&erases, (ptu32_t)&unit))
        {
            printf("\r\n: erro : iapfs  : erase unit %lld failed, cannot get remain.", unit);
            return (-1);
        }

        if(um->merase(unit, esz))
        {
            printf("\r\n: erro : iapfs  : erase unit %lld failed, with erases %d.", unit, erases);
            return (-1);
        }

        erases++; // 擦除增一，表示包括当前的unit
        left -= ((erases << um->usz) - offset);
        unit += erases;
        offset = 0;
    }

    return (0);
}

