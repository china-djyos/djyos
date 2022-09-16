//-----------------------------------------------------------------------------
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

#include <stddef.h>
#include <endian.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "device/unit_media.h"
#include "device/djy_flash.h"      // will be obsolete
#include "dbug.h"

// ============================================================================
// 功能：解析NAND FLASH的ONFI参数页。
// 参数：data -- NandParamterPage
//      onfi -- Nand描述结构体
//      little -- NandParamterPage的格式,"1"表示32位小端。
// 返回：成功(0); 失败(-1);
// 备注：参数页的具体数据结构,要看注意下NAND FLASH的参数手册
// ============================================================================
s32 deonfi(const struct NandONFI *data, struct NandDescr *onfi, u8 little)
{
    u8 i;

    char signature[4] = {'O', 'N', 'F', 'I'}; // 用于识别是否是ONFI数据

    // ONFI标记
    if(little)
    {
        *(u32 *)signature = swapl(*(u32 *)signature);
    }
    for (i = 0; i < 4; i++)
    {
        if (signature[i] != data->signature[i])
        {
            printf("\r\n: info : device : flash's ONFI is unavailable\r\n");
            return (-1); // 标记内容不符要求
        }
    }

    // ONFI版本号

    if(little)
        onfi->Revision = (u16)swapl(data->revision_num);
    else
        onfi->Revision = (u16)(data->revision_num);

    // Device Manufacturer
    for (i = 0; i < 12; i++)
    {
        onfi->Manufacturer[i] = data->dev_manufacturer[i];
    }
    if(little)
    {
        for (i = 0; i < 3; i++)
        {
            ((u32 *)onfi->Manufacturer)[i] = swapl(((u32 *)onfi->Manufacturer)[i]);
        }
    }
    onfi->Manufacturer[sizeof(onfi->Manufacturer)-1] = '\0';

    // Device Model
    for (i = 0; i < 20; i++)
    {
        onfi->DeviceModel[i] = data->dev_model[i];
    }
    if(little)
    {
        for (i = 0; i < 5; i++)
        {
            ((u32 *)onfi->DeviceModel)[i] = swapl(((u32 *)onfi->DeviceModel)[i]);
        }
    }
    onfi->DeviceModel[sizeof(onfi->DeviceModel) - 1] = '\0';

    // Number of data bytes per page
    if(little)
    {
        onfi->BytesPerPage = swapl((u32)(data->bytes_per_page));
    }
    else
    {
        onfi->BytesPerPage = (u32)data->bytes_per_page;
    }

    // Number of spare bytes per page
    if(little)
    {
        onfi->OOB_Size = swapl((u16)data->spare_bytes_per_page);
    }
    else
    {
        onfi->OOB_Size = (u16)data->spare_bytes_per_page;
    }

    // Number of pages per block
    if(little)
    {
        onfi->PagesPerBlk = swapl((u32)data->pages_per_block);
    }
    else
    {
        onfi->PagesPerBlk = (u32)data->pages_per_block;
    }

    // Number of blocks per LUN
    if(little)
    {
        onfi->BlksPerLUN = swapl((u32)data->blocks_per_LUN);
    }
    else
    {
        onfi->BlksPerLUN = (u32)data->blocks_per_LUN;
    }

    // Number of LUNs
    if(little)
    {
        onfi->LUNs = swapl((u8)data->LUNs);
    }
    else
    {
        onfi->LUNs = (u8)data->LUNs;
    }

 #if 0
    // Guaranteed valid block at the beginning of target
    for (i = 0; i < 4; i++)
    {
        temp[i] = (u8)data[104+i];
    }
    *(u32 *)temp = swapl(*(u32 *)temp);
    onfi->SafeBlkNo = temp[3];
 #endif

    printf("\r\n");
    printf(": info : device : flash's ONFI >>>>\r\n");
    printf("                  manufacturer is %s\r\n", onfi->Manufacturer);
    printf("                  model is %s\r\n", onfi->DeviceModel);
    printf("                  details (%d LUNs; %d blocks/LUN; %d pages/block; %d bytes/page; %d bytes/OOB",
           onfi->LUNs, onfi->BlksPerLUN, onfi->PagesPerBlk, onfi->BytesPerPage, onfi->OOB_Size);

 #if 0
   printf(";guaranteed %d blocks", onfi->SafeBlkNo );
 #endif
   printf(").");
    return (0);
}

#if 0
// ============================================================================
// 功能： 擦除整颗芯片(保留块除外)
// 参数：
// 返回： >=0 -- 存在的坏块数；-1 -- 失败;
// 备注：will be obsolete；
// ============================================================================
s32 EarseWholeChip(struct FlashChip *Chip)
{
    u32 i;
    u32 Blks;
    s32 BadBlks = 0;

    if(NULL == Chip)
        return (-1);

    if(F_NAND == Chip->Type)
    {
        Blks = Chip->Descr.Nand.BlksPerLUN * Chip->Descr.Nand.LUNs;

        for(i = Chip->Descr.Nand.ReservedBlks; i < Blks; i++)
        {
            if(0 != Chip->Ops.ErsBlk(i))
            {
                // 擦除失败
                BadBlks++;
//                TraceDrv(FLASH_TRACE_ERROR, "block %d cannot be erased!\r\n", i);
                error_printf("flash","block %d cannot be erased!\r\n", i);
            }
        }
    }
    else if(F_NOR == Chip->Type)
    {
        if(0 == Chip->Descr.Nor.ReservedBlks)
        {
            if(0 != Chip->Ops.Special.Nor.ErsChip())
            {
                // 擦除失败
                BadBlks = -1;
 //               TraceDrv(FLASH_TRACE_ERROR, "chip %d cannot be erased!\r\n", i);
                error_printf("flash","chip %d cannot be erased!\r\n", i);
            }
        }
        else
        {
            for(i = Chip->Descr.Nand.ReservedBlks; i < Chip->Descr.Nor.Blks; i++)
            {
                if(0 != Chip->Ops.ErsBlk(i))
                {
                    // 擦除失败
                    BadBlks++;
 //                   TraceDrv(FLASH_TRACE_ERROR, "block %d cannot be erased!\r\n", i);
                    error_printf("flash","block %d cannot be erased!\r\n", i);
                }
            }
        }

    }
    else
    {
        BadBlks = -1;
    }
    return (BadBlks);
}
#endif

// ============================================================================
// 坏块信息结构
// ============================================================================
// 标签          "bad blocks table" // 12字节
// 坏块数                                                  // 4字节
// 第一个坏块号                                      // 4字节
// 第二个坏块号                                      // 4字节
// 等等
// 最多128个坏块
//

// ============================================================================
// 功能：打印坏块信息
// 参数：req -- unit media请求；
//      table -- 坏块表；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static void __print_bads(u32 *table)
{
    u32 i;

    printf("\r\n: info : device : bad blocks information:");
    printf("\r\n                  bad block nums %d", *table);

    for(i=0; i<*table; i++)
    {
        if(!(i%8))
            printf("\r\n                  ");

        printf("[%03d]", table[i+1]);
    }

    printf("\r\n");
}

// ============================================================================
// 功能：检索坏块，并建立坏块表
// 参数：req -- unit media请求；
//      table -- 坏块表；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
#define BAD_BLOCK_TAG           "bad blocks table"
s32 nandscanbads( __um_req req, u32 **table)
{
    u32 i, blocks;
    u32 tmp[128]; // 预估有128个坏块；
    u8 bads = 0;

    if(req(totalblocks, (ptu32_t)&blocks))
        return (-1);

    for(i = 0; i < blocks; i++)
    {
        if(req(checkbad, i) != 0) // 是坏块
        {
            tmp[bads] = i;
            bads++;
            if(bads>128)
            {
                info_printf("device", "too much bad blocks for nand.\r\n");
                return (-1);
            }
        }
    }

    *table = malloc((bads << 2) + 4);
    if(!(*table))
    {
        error_printf("device", "no memory for bad blocks table.\r\n");
        return (-1);
    }

    **table = bads; // 开始用于记录bad数量
    memcpy((u8*)(*table+1), tmp, bads<<2);
    __print_bads(*table);
    return (0);
}

// ============================================================================
// 功能：是否是有效坏块表；
// 参数：badtable -- 坏块表；
// 返回：是（1）；否（0）；
// 备注：
// ============================================================================
s32 nandvalidbads(u32 *badtable)
{
    if(strcmp((char*)badtable, BAD_BLOCK_TAG))
        return (0);

    return (1);
}
// ============================================================================
// 功能：建立坏块表
// 参数：req -- 请求；
// 返回：成功（坏块表）；失败（NULL）；
// 备注：
// ============================================================================
u32 *nandbuildbads(__um_req req)
{
    u8 *tmp, *badcount;
    s32 res;
    u32 *table, bytes;

    if(!req)
        return (NULL);

    if(req(unitbytes, (ptu32_t)&bytes))
        return (NULL);

    tmp = malloc(bytes);
    if(!tmp)
        return (NULL);

    res = req(getbads, (ptu32_t)&tmp); // 获取坏块表
    if(-1==res)
    {
        // 获取失败；
        free(tmp);
        return (NULL);
    }

    // 查看坏块表是否正常（有标签才是正常的坏块表）
    if(strcmp(BAD_BLOCK_TAG, (char*)tmp)) // 不正常，建立坏块表
    {

        if(nandscanbads(req, &table))
        {
            table = NULL;
        }
        else
        {
            // 给坏块表增加标签，并写入nand
            strcpy((char*)tmp, BAD_BLOCK_TAG);
            memcpy((tmp+(strlen(BAD_BLOCK_TAG)+4)), table, ((table[0]+1)<<2)); //
            if(req(savebads, (ptu32_t)tmp)) // 保存坏块表
                debug_printf("device", "bad block table save failed for nand.\r\n");
        }
    }
    else // 正常， 提取出坏块表，防止在内存；
    {
        badcount = (tmp + strlen(BAD_BLOCK_TAG) + 4);
        bytes = (*((u32*)badcount) + 1) << 2;
        table = malloc(bytes);
        if(table)
            memcpy(table, badcount, bytes);
    }

    free(tmp);
    return (table);
}

// ============================================================================
// 功能：获取屏蔽坏块后的块号；
// 参数：badtable -- 坏块表；
//      block -- 块号；
//      req -- 请求；
// 返回：无；
// 备注：
// ============================================================================
void nandbadfreeblock(u32 *badtable, u32 *block, __um_req req)
{
    u32 i, count = badtable[0];

    if(!req || !badtable || !block)
        return;

    for(i = 1; i <= count; i++)
    {
        if(*block >= badtable[i])
        {
            *block += 1;
        }
        else
        {
            break;
        }
    }
}

// ============================================================================
// 功能：获取屏蔽坏块情况下的unit号；
// 参数：badtable -- 坏块表；
//      unit -- unit号；
//      req -- 请求；
// 返回：无；
// 备注：
// ============================================================================
void nandbadfreeunit(u32 *badtable, s64 *unit, __um_req req)
{
    u32 i, block = 0, count = badtable[0], movs = 0, units = 0;

    if(!req || !badtable || !unit)
        return;

    req(whichblock, (ptu32_t)&block, *unit);
    for(i = 1; i <= count; i++)
    {
        if(block >= badtable[i])
        {
            block += 1;
            req(blockunits, (ptu32_t)&units, block);
            movs += units;
        }
        else
        {
            break;
        }
    }

    *unit += movs;
}

#if 0
// ============================================================================
// 功能：NAND分区初始化；
// 参数：bstart -- 起始块；
//      bcount -- 块数；
//      doformat -- 格式化；
// 返回：
// 备注：
// ============================================================================
__attribute__((weak)) s32 __nand_part_init(u32 bstart, u32 bcount, u32 doformat)
{
    bstart = bstart;
    bcount = bcount;
    doformat = doformat;
    return (-1);
}


// ============================================================================
// 功能：embaed分区初始化；
// 参数：bstart -- 起始块；
//      bcount -- 块数；
//      doformat -- 格式化；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
__attribute__((weak)) s32 __embed_part_init(u32 bstart, u32 bcount, u32 doformat)
{
    bstart = bstart;
    bcount = bcount;
    doformat = doformat;
    return (-1);
}

// ============================================================================
// 功能：在NAND FLASH上安装分区；
// 参数： parts -- 分区数；
//      分区数据 -- 起始块，分区块数，是否格式化；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 ModuleInstall_PART_ON_NAND(u8 parts, ...)
{
    u8 part;
    u32 startblock, blocks, doformat;
    va_list list;
    s32 res = 0;

    va_start(list, parts);
    for(part=0; part<parts; part++)
    {
        startblock = (u32)va_arg(list, u32);
        blocks = (u32)va_arg(list, u32);
        doformat = (u32)va_arg(list, u32);
        if(__nand_part_init(startblock, blocks, doformat))
        {

            res = -1;
            break;
        }
    }

    va_end(list);
    return (res);
}


// ============================================================================
// 功能：在EMBEDED FLASH上安装分区；
// 参数：parts -- 分区数；
//      分区数据 -- 起始块，分区块数，是否格式化；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 ModuleInstall_PART_ON_EMBEDED(u8 parts, ...)
{
    u8 part;
    u32 startblock, blocks, doformat;
    va_list list;
    s32 res = 0;

    va_start(list, parts);
    for(part=0; part<parts; part++)
    {
        startblock = (u32)va_arg(list, u32);
        blocks = (u32)va_arg(list, u32);
        doformat = (u32)va_arg(list, u32);
        if(__embed_part_init(startblock, blocks, doformat))
        {
            res = -1;
            break;
        }
    }

    va_end(list);
    return (res);
}
#endif

