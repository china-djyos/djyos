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
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <misc.h>
#include <systime.h>
#include <lock.h>
#include "dbug.h"
#include <device/flash/flash.h>

struct __LowLevel{
    u32 StartPage;
    u32 EndPage;
    u32 PageBytes;
    u32 EraseBytes;
    s32 (*WritePage)(u32 PageNo, u8 *Data, u32 Flags);
    s32 (*ReadPage)(u32 PageNo, u8 *Data, u32 Flags);
    s32 (*Erase)(u32 dwSector); // 也可以是block
    s32 (*GetSector)(u32 dwPage, u32 *pRemain, u32 *pSector); // 也可以是block
    struct MutexLCB *Lock;
    u8 *Buf;
//    u8 bCRC;
};

extern u32 gc_pAppRange; // 来自于LDS定义
extern u32 gc_pAppOffset; // 来自于LDS定义
// ============================================================================
// 功能：IAP的volume初始化
// 参数：pDev -- flash设备
//      pBase -- 返回IAP在volume的偏置。
// 返回：IAP的volume
// 备注：TODO: 进一步完善配置校验
// ============================================================================
void *LowLevelInit(void *pDev, u32 *pBase, u32 *pBufSize)
{
    struct __LowLevel *LowLevel;
    u32 left;
    struct FlashChip *flash = (struct FlashChip*)pDev;
    u32 capacity = gc_pAppRange; // IAP空间在FLASH的大小
    u32 offset = gc_pAppOffset; // IAP空间在FLASH的起始偏置(地址)
    u32 round = 0;

    if(F_ALIEN == flash->Type)
    {
        if(offset < flash->Descr.Embd.MappedStAddr)
        {
            error_printf("iapfs","low level offset is wrong.");
            return (NULL);
        }

        offset = offset - flash->Descr.Embd.MappedStAddr;
    }
    else
    {
        offset = 0;
    }

    if(offset % flash->dwPageBytes) // 地址页对齐校验
    {
        error_printf("iapfs","low level offset should be a page size rounded.");
        return (NULL);
    }

    offset = offset / flash->dwPageBytes; // 将地址转成页序号，即起始页
    if(offset < flash->dwPagesReserved)
    {
        offset = flash->dwPagesReserved;
    }

    left = flash->dwTotalPages - offset; // TODO:这里的逻辑要改掉

    if(capacity % flash->dwPageBytes)
        round    = 1; // 容量页对齐

    capacity = capacity / flash->dwPageBytes - round; // 转换为页号
    if(!capacity || (capacity > left ))
    {
        info_printf("iapfs","the settings of range (%xH) is not available, adjust to (%xH).",
                capacity, left);
        capacity = left;
    }

    LowLevel = malloc(sizeof(struct __LowLevel));
    if(!LowLevel)
        return (NULL);

    LowLevel->StartPage = offset;
    LowLevel->EndPage = capacity + offset - 1; // 页号从零计
    LowLevel->PageBytes = flash->dwPageBytes;
    LowLevel->WritePage = flash->Ops.WrPage;
    LowLevel->ReadPage = flash->Ops.RdPage;
    LowLevel->Erase = flash->Ops.ErsBlk;
    LowLevel->GetSector = flash->Ops.PageToBlk;
#if 0
    if(F_ALIEN == flash->Type)
    {
        LowLevel->bCRC = 1;
    }
    else
    {
        LowLevel->bCRC = 0;
    }
#endif
    if(flash->Lock)
    {
        LowLevel->Lock = (struct MutexLCB*)flash->Lock;
    }
    else
    {
        free(LowLevel);
        return (NULL); // 失败，驱动必须提供互斥锁
    }

        
    if(flash->Buf)
    {
        LowLevel->Buf = flash->Buf;
    }
    else
    {
        free(LowLevel);
        return (NULL); // 失败，驱动必须提供缓冲
    }    

    *pBase = 0; //
    *pBufSize = flash->dwPageBytes;
    info_printf("iapfs"," ");
    if(F_ALIEN == flash->Type)
    {
        debug_printf("iapfs","embedded flash");
    }
    else if(F_NOR == flash->Type)
    {
        debug_printf("iapfs","nor flash");
    }
    else if(F_NAND == flash->Type)
    {
        debug_printf("iapfs","nand flash");
    }
    else
    {
        debug_printf("iapfs","unknown");
    }

    debug_printf("iapfs","%d bytes/page", LowLevel->PageBytes);
    debug_printf("iapfs","ranges %d pages(from page no.%d to %d).", (LowLevel->EndPage-LowLevel->StartPage),
            LowLevel->StartPage, LowLevel->EndPage);

    return ((void*)LowLevel);
}

// ============================================================================
// 功能：IAP的volume卸载
// 参数：
// 返回：
// 备注：
// ============================================================================
void LowLevelDeInit(void *LowLevel)
{
    free(LowLevel);
}

// ============================================================================
// 功能：页偏置
// 参数：
// 返回：
// 备注：
// ============================================================================
static u32 __PageFix(struct __LowLevel *LowLevel, u32 PageNo)
{
    return (PageNo + LowLevel->StartPage);
}

// ============================================================================
// 功能：写数据
// 参数：LowLevel -- IAP的volume
//      Buf -- 数据缓冲
//        Bytes -- 写字节数
//        Addr -- 数据地址
// 返回：实际写入的字节数
// 备注：
// ============================================================================
s32 LowLevelWrite(void *pLowLevel, u8 *pBuf, u32 dwBytes, u32 dwAddr)
{
    u32 i,j, page, offset, bytesToWr, once, remain, sector;
    s32 ret;
    struct __LowLevel *LL = (struct __LowLevel *)pLowLevel;
    u8 *data = (u8*)pBuf;

    bytesToWr = dwBytes;
    Lock_MutexPend(LL->Lock, CN_TIMEOUT_FOREVER);
    page = dwAddr / LL->PageBytes;
    page = __PageFix(LL, page);
    offset = dwAddr % LL->PageBytes;
    while(bytesToWr > 0)
    {
        // 如果当前写入页是一个块中的最后一页，则预先删除后续的sector
        // (page+1)用于防止格式化了不属于IAP的空间
        LL->GetSector(page, &remain, &sector);
        if((!remain) && ((page+1) < LL->EndPage))
            LL->Erase(sector+1); // 不管有没有擦除成功，因为如果后续写入的话，会有回读校验

        once = MIN((LL->PageBytes - offset), bytesToWr);
        if(once != LL->PageBytes)
        {
            ret = LL->ReadPage(page, LL->Buf, NO_ECC);
            if(!ret)
                goto __WRITE_FAILURE; //

            // 做简单的检查
            for(i = offset, j = 0; i < (offset + once); i++, j++)
            {
                if((0xFF != LL->Buf[i]) && (LL->Buf[i] != data[j]))
                {
                    error_printf("iapfs","data (%xH)(in flash, page:%xH, offset:%xH) cannot be rewrote to (%xH) for safety.",
                             LL->Buf[i], page, i, data[j]);
                    goto __WRITE_FAILURE; //
                }
            }
        }

        memcpy((LL->Buf + offset), data, once);
        ret = LL->WritePage(page, LL->Buf, NO_ECC);
        if(ret != LL->PageBytes)
            goto __WRITE_FAILURE;

        bytesToWr -= once;
        offset = 0;
        page++;
        data += once;
    }

__WRITE_FAILURE:
    Lock_MutexPost(LL->Lock);    
    
    return ((s32)(dwBytes - bytesToWr));
}

// ============================================================================
// 功能：读数据
// 参数：LowLevel -- IAP的volume
//       Buf -- 数据缓冲
//       Bytes -- 读字节数
//        Addr -- 数据地址
// 返回：实际读到的字节数
// 备注：
// ============================================================================
s32 LowLevelRead(void *LowLevel, u8 *Buf, u32 Bytes, u32 Addr)
{
    s32 Ret;
    u32 PageNo, Offset;
    u32 LenToRd, OnceToRd;
    struct __LowLevel *LL = (struct __LowLevel *)LowLevel;
    u8 *DataBuf = (u8*)Buf;

    LenToRd = Bytes;
    Lock_MutexPend(LL->Lock, CN_TIMEOUT_FOREVER);
    PageNo = Addr / LL->PageBytes;
    PageNo = __PageFix(LL, PageNo);
    Offset = Addr % LL->PageBytes;
    while(LenToRd > 0)
    {
        OnceToRd = MIN((LL->PageBytes - Offset), LenToRd);
        Ret = LL->ReadPage(PageNo, LL->Buf, NO_ECC);
        if(!Ret)
            goto FAILURE; //

        memcpy(DataBuf, (LL->Buf + Offset), OnceToRd);
        LenToRd -= OnceToRd;
        Offset = 0;
        PageNo++;
        DataBuf += OnceToRd;
    }

FAILURE:
    Lock_MutexPost(LL->Lock);
    return((s32)(Bytes - LenToRd));
}

// ============================================================================
// 功能：擦除数据
// 参数：pLowLevel -- IAP的volume
//       dwBytes -- 字节数
//       dwAddr -- 数据地址
// 返回：实际写入的字节数
// 备注：
// ============================================================================
s32 LowLevelErase(void *pLowLevel, u32 dwBytes, u32 dwAddr)
{
    s32 res;
    u32 pageNo;
    u32 remain;
    u32 sectorNo;
    u32 offset;
    u32 bytesLeft = dwBytes;
    struct __LowLevel *LL = (struct __LowLevel *)pLowLevel;
    
    Lock_MutexPend(LL->Lock, CN_TIMEOUT_FOREVER);
    offset = dwAddr % LL->PageBytes;
    while(bytesLeft)
    {
        pageNo = dwAddr / LL->PageBytes;
        pageNo = __PageFix(LL, pageNo);
        LL->GetSector(pageNo, &remain, &sectorNo);
        res = LL->Erase(sectorNo);
        if(res)
        {
            error_printf("iapfs","erase sector %d failed.", sectorNo);
            break;
        }
            
        if(bytesLeft < (remain) * LL->PageBytes)
            bytesLeft = 0;
        else    
            bytesLeft -= ((remain) * LL->PageBytes - offset);
        
        dwAddr += (remain * LL->PageBytes - offset);
        offset = 0;
    }
    Lock_MutexPost(LL->Lock);
    return (dwBytes - bytesLeft);
}

// ============================================================================
// 功能：格式化IAP的volume空间
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 LowLevelFormat(void *pLowLevel)
{
    s32 res;
    u32 page;
    u32 pagesLeft;
    u32 remain;
    u32 sector;
    u8 advance = 0;
    struct __LowLevel *LL = (struct __LowLevel *)pLowLevel;

    pagesLeft = LL->EndPage - LL->StartPage + 1; // 页号从零计
    page = LL->StartPage;
    Lock_MutexPend(LL->Lock, CN_TIMEOUT_FOREVER);
    while(pagesLeft)
    {
        LL->GetSector(page, &remain, &sector);
        res = LL->Erase(sector);
        if(res)
            break;

        if(pagesLeft < (remain))
            pagesLeft = 0;
        else
            pagesLeft -= (remain);

        page += remain;

        switch(advance)
        {
        case 0 : debug_printf("iapfs","\b\b\b.  ");advance = 1;break;
        case 1 : debug_printf("iapfs","\b\b\b.. ");advance = 2;break;
        case 2 : debug_printf("iapfs","\b\b\b...");advance = 0;break;
        default: break;
        }
    }
    Lock_MutexPost(LL->Lock);

    return(res);
}


// ============================================================================
// 功能：计算一段数据的CRC
// 参数：
// 返回：
// 备注：
// ============================================================================
u32 LowLevelCRC32(void *LowLevel, u32 dwOffset, u32 dwLen)
{
    u32 offset;
    s32 res;
    u32 crc;
    u32 pageNo;
    u32 once;
    u8 *data = NULL;
    u32 bytesLeft = dwLen;
    struct __LowLevel *LL = (struct __LowLevel *)LowLevel;

    Lock_MutexPend(LL->Lock, CN_TIMEOUT_FOREVER);
    crc32init(&crc);
    pageNo = dwOffset / LL->PageBytes;
    pageNo = __PageFix(LL, pageNo);
    offset = dwOffset % LL->PageBytes;
    while(bytesLeft > 0)
    {
        once = MIN((LL->PageBytes - offset), bytesLeft);
        res = LL->ReadPage(pageNo, LL->Buf, NO_ECC);
        if(!res)
        {
            error_printf("iapfs","read page %d failed.", pageNo);
            goto FAILURE; //
        }
        data = LL->Buf + offset;
        crc32run(&crc, data, once);
        bytesLeft -= once;
        offset = 0;
        pageNo++;
    }
    crc32exit(&crc);

FAILURE:
    Lock_MutexPost(LL->Lock);
    return (crc);
}
