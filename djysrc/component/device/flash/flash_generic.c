//-----------------------------------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
#include "flash.h"
#include "dbug.h"

// ============================================================================
// 功能：解析NAND FLASH的ONFI参数页。
// 参数：pRaw -- NandParamterPage
//       pONFI -- Nand描述结构体
//       bLittle -- NandParamterPage的格式,"1"表示32位小端。
// 返回：成功(0); 失败(-1);
// 备注：参数页的具体数据结构,要看注意下NAND FLASH的参数手册
// ============================================================================
s32 DecodeONFI(const char *pRaw, struct NandDescr *pONFI, u8 bLittle)
{
    u8 i;
    u32 offset;
    u8 temp[20] = {0};
    char signature[4] = {'O', 'N', 'F', 'I'}; // 用于识别是否是ONFI数据
 
    // ONFI标记
    offset = 0;
    if(bLittle)
    {
        *(u32 *)signature = swapl(*(u32 *)signature);
    }
    for (i = 0; i < 4; i++)
    {
        if (signature[i] != (char)pRaw[offset+i])
            return (-1); // 标记内容不符要求
    }
 
    // ONFI版本号
    offset = 4;
    for (i = 0; i < 4; i++)
    {
        temp[i] = (u8)pRaw[offset+i];
    }
    
    if(bLittle)
        pONFI->Revision = (u16)swapl(*(u32 *)temp);
    else
        pONFI->Revision = (u16)(*(u32 *)temp);
 
    // Device Manufacturer
    offset = 32;
    for (i = 0; i < 12; i++)
    {
        temp[i] = (u8)pRaw[offset+i];
    }
    if(bLittle)
    {
        for (i = 0; i < 3; i++)
        {
            ((u32 *)temp)[i] = swapl(((u32 *)temp)[i]);
        }
    }
    for (i = 0; i < 12; i++)
    {
        pONFI->Manufacturer[i] = (char)temp[i];
    }
    pONFI->Manufacturer[i] = '\0';
 
    // Device Model
    offset = 44;
    for (i = 0; i < 20; i++)
    {
        temp[i] = (u8)pRaw[offset+i];
    }
    if(bLittle)
    {
        for (i = 0; i < 5; i++)
        {
            ((u32 *)temp)[i] = swapl(((u32 *)temp)[i]);
        }
    }
    for (i = 0; i < 20; i++)
    {
        pONFI->DeviceModel[i] = (char)temp[i];
    }
    pONFI->DeviceModel[i] = '\0';
 
    // Number of data bytes per page
    offset = 80;
    for (i = 0; i < 4; i++)
    {
        temp[i] = (u8)pRaw[offset+i];
    }
    if(bLittle)
    {
        pONFI->BytesPerPage = swapl(*(u32 *)temp);
    }
    else
    {
        pONFI->BytesPerPage = *(u32 *)temp;
    }
 
    // Number of spare bytes per page
    offset = 84;
    for (i = 0; i < 4; i++)
    {
        temp[i] = (u8)pRaw[offset+i];
    }
    if(bLittle)
    {
        pONFI->OOB_Size = swapl(*(u32 *)temp);
    }
    else
    {
        pONFI->OOB_Size = *(u32 *)temp;
    }
 
    // Number of pages per block
    offset = 92;
    for (i = 0; i < 4; i++)
    {
        temp[i] = (u8)pRaw[offset+i];
    }
    if(bLittle)
    {
        pONFI->PagesPerBlk = swapl(*(u32 *)temp);
    }
    else
    {
        pONFI->PagesPerBlk = *(u32 *)temp;
    }
 
    // Number of blocks per LUN
    offset = 96;
    for (i = 0; i < 4; i++)
    {
        temp[i] = (u8)pRaw[offset+i];
    }
    if(bLittle)
    {
        pONFI->BlksPerLUN = swapl(*(u32 *)temp);
    }
    else
    {
        pONFI->BlksPerLUN = *(u32 *)temp;
    }
 
    // Number of LUNs
    offset = 100;
    for (i = 0; i < 4; i++)
    {
        temp[i] = (u8)pRaw[offset+i];
    }
    if(bLittle)
    {
        pONFI->LUNs = (u8)swapl(*(u32 *)temp);
    }
    else
    {
        pONFI->LUNs = (u8)(*(u32 *)temp);
    }
 
 #if 0
    // Guaranteed valid block at the beginning of target
    for (i = 0; i < 4; i++)
    {
        temp[i] = (u8)pRaw[104+i];
    }
    *(u32 *)temp = swapl(*(u32 *)temp);
    pONFI->SafeBlkNo = temp[3];
 #endif
 
    debug_printf("flash","\r\n");
    debug_printf("flash","FLASH's ONFI :\r\n");
    debug_printf("flash","    device's manufacturer is \"%s\"\r\n", pONFI->Manufacturer);
    debug_printf("flash","    device's model is %s\r\n", pONFI->DeviceModel);
    debug_printf("flash","    device's details, %d LUNs, %d blocks/LUN, %d pages/block, %d bytes/page, %d bytes/OOB.",
           pONFI->LUNs, pONFI->BlksPerLUN, pONFI->PagesPerBlk, pONFI->BytesPerPage, pONFI->OOB_Size);
    debug_printf("flash","\r\n\r\n");

 #if 0
//    TraceDrv(FLASH_TRACE_DEBUG, "Guaranteed block:0x%x\r\n", pONFI->SafeBlkNo );
    debug_printf("flash","Guaranteed block:0x%x\r\n", pONFI->SafeBlkNo);
 #endif
 
    return (0);
}

// ============================================================================
// 功能： 擦除整颗芯片(保留块除外)
// 参数：
// 返回： >=0 -- 存在的坏块数；-1 -- 失败;
// 备注：
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
#if 0
// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
u32 __FLASH_Write(tagOFile *devFile, u8 *pBuf, u32 dwLen, u32 dwTimeout)
{
    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
u32 __FLASH_Read(tagOFile *devFile, u8 *pBuf, u32 dwLen, u32 dwTimeout)
{
    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 FLASH_Register(char *name)
{

}
#endif
