//-----------------------------------------------------------------------------
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

/*#include "norflash.h"*/
#include "silan_m0_cache.h"
#include "silan_irq.h"
#include "int.h"
#include "string.h"
#include <stdlib.h>
#include "flash.h"

static struct EmbdFlashDescr{
    u16     BytesPerPage;                // 页中包含的字节数
    u16     PagesPerSmallSect;           // small sector的页数
    u16     PagesPerLargeSect;           // large sector的页数
    u16     PagesPerNormalSect;          // Normal sector的页数
    u16     SmallSectorsPerPlane;        // 每plane的small sector数
    u16     LargeSectorsPerPlane;        // 每plane的large sector数
    u16     NormalSectorsPerPlane;       // 每plane的normal sector数
    u16     Planes;                      // plane数
    u32     MappedStAddr;
} *sp_tFlashDesrc;
extern u32 gc_ptIbootSize;
extern u32 gc_ptFlashOffset;
extern u32 gc_ptFlashRange;


extern int32_t EraseSomeBlocks(uint32_t addr, uint32_t size);
extern int32_t ProgramOnePackage(char *data, uint32_t addr, uint32_t size);

#define PAGE_SIZE          (0x100)      /* 256 byte */
#define SECTOR_SIZE        (0x1000)      /* 4 Kbyte */
#define BLOCK_SIZE         (0x10000)     /* 8 Bbyte*/
#define FLASH_SIZE         (0x200000)    /* 2 MBytes */

extern u32 Lock_Cache_Add_Start;
extern u32 Lock_Cache_Add_End;

static struct EmbdFlashDescr *sp_tFlashDesrc;


#define MPU_APP_FLASH_REGION     (12)
#define MPU_APP_START_ADDRESS    (0x00400000)            // 内置FLASH起始地址
#define MPU_APP_END_ADDRESS      (0x00400000 + 0x200000) // 2MB

extern u32 gc_ptIbootSize;
// ============================================================================
// 功能：喂狗
// 参数：
// 返回：
// 备注：
// ============================================================================
extern bool_t BrdWdt_FeedDog(void) __attribute__((weak));
bool_t BrdWdt_FeedDog(void)
{
    return (TRUE); // 空函数
}

// ============================================================================
// 功能：内置FLASH的初始化
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 Flash_Init(struct EmbdFlashDescr *Description)
{
    Description->BytesPerPage = PAGE_SIZE;
    Description->PagesPerSmallSect = 0;
    Description->PagesPerLargeSect = 0;
    Description->PagesPerNormalSect = 1;
    Description->SmallSectorsPerPlane = 0;
    Description->LargeSectorsPerPlane = 0;
    Description->NormalSectorsPerPlane = gc_ptFlashRange/PAGE_SIZE;
    Description->Planes = 1;
    Description->MappedStAddr = gc_ptFlashOffset;
    return (0);
}


// ============================================================================
// 功能：获取内置FLASH的信息
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 Flash_GetDescr(struct EmFlashDescr *Description)
{
    Description->BytesPerPage = sp_tFlashDesrc->BytesPerPage;
    Description->TotalPages = ( sp_tFlashDesrc->PagesPerSmallSect *
                                sp_tFlashDesrc->SmallSectorsPerPlane +
                                sp_tFlashDesrc->PagesPerLargeSect *
                                sp_tFlashDesrc->LargeSectorsPerPlane +
                                sp_tFlashDesrc->PagesPerNormalSect *
                                sp_tFlashDesrc->NormalSectorsPerPlane) *
                                sp_tFlashDesrc->Planes;
    Description->ReservedPages = gc_ptIbootSize / sp_tFlashDesrc->BytesPerPage;
    Description->MappedStAddr  = sp_tFlashDesrc->MappedStAddr;
    return (0);
}


// ============================================================================
// 功能：进入flash 操作模式(操作模式包括写、擦除)
// 参数：
// 返回：
// 备注：
// ============================================================================

static u32 restored;

static void enterFlashOperaterMode(void)
{
    u32 optStartAddr, Remain, optAddrLen;

    restored = pic_get_enable();
    pic_clr_enable(restored);

    silan_m0_cache_disable();
    silan_m0_cache_clear();

    optStartAddr =    ((u32) &Lock_Cache_Add_Start) & 0xFFFFFFF0;
    Remain       =    ((u32) &Lock_Cache_Add_Start) & 0x0000000F;
    optAddrLen   =    ((((u32) &Lock_Cache_Add_End) - ((u32) &Lock_Cache_Add_Start)) + Remain + M0_CACHE_LINE_SIZE - 1) / M0_CACHE_LINE_SIZE;

    silan_m0_cache_lock(optStartAddr, optAddrLen);

    silan_m0_cache_enable();

    iap_norflash_init();
}


// ============================================================================
// 功能：退出 flash 操作模式
// 参数：
// 返回：
// 备注：
// ============================================================================

static void exitFlashOperaterMode(void)
{

    u32 optStartAddr, Remain, optAddrLen;

    optStartAddr =    ((u32) &Lock_Cache_Add_Start) & 0xFFFFFFF0;
    Remain       =    ((u32) &Lock_Cache_Add_Start) & 0x0000000F;
    optAddrLen   =    ((((u32) &Lock_Cache_Add_End) - ((u32) &Lock_Cache_Add_Start)) + Remain + M0_CACHE_LINE_SIZE - 1) / M0_CACHE_LINE_SIZE;

    silan_m0_cache_unlock(optStartAddr, optAddrLen, 0);

    pic_set_enable(restored);

}


// ============================================================================
// 功能：扇区擦除
// 参数：SectorNo -- 扇区号;
// 返回："0" -- 成功;"-1" -- 失败;
// 备注：
// ============================================================================
static s32 Flash_SectorEarse(u32 SectorNo)
{
    u32 Addr;
    u8 retry = 0; // 擦除有可能会失败；
    s32 Ret = 0;
    u32 SECTORError=0;//保存出错类型信息

    atom_high_t high_atom;                      //原子操作
    high_atom =Int_HighAtomStart();

    enterFlashOperaterMode();
    Addr = SectorNo * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr;
    EraseSomeSectors(Addr,SECTOR_SIZE);
    exitFlashOperaterMode();

    Int_HighAtomEnd(high_atom);

    return Ret;
}


// ============================================================================
// 功能：写某页
// 参数：PageNo -- 页号;
//       Data -- 写缓冲;
//       Flags -- 写操作控制位;
// 返回：">0" -- 正常，写入的字节数;
//       "-1" -- 参数错误;
//       "-2" -- 写失败;
// 备注：
// ============================================================================
static s32 Flash_PageProgram(u32 Page, u8 *Data, u32 Flags)
{
    u32 datLen;
    u32 DatAddr = Page * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr;
    u32 DatToWrite = 0;

    atom_high_t high_atom;                      //原子操作
    high_atom =Int_HighAtomStart();

    enterFlashOperaterMode();
    datLen       = sp_tFlashDesrc->BytesPerPage;
    DatToWrite = ProgramOnePackage((u8*)Data, DatAddr, datLen);

    exitFlashOperaterMode();

    Int_HighAtomEnd(high_atom);
    return DatToWrite;
}


// ============================================================================
// 功能：读某页
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 Flash_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    u8  *ptReadAddr;
    u32 readDatNo;
    u32 loop;

    ptReadAddr   = (u8*)(PageNo * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr);
    readDatNo    = sp_tFlashDesrc->BytesPerPage;

    if(Data)
    {
        for(loop = 0;loop < readDatNo;loop++)
        {
            Data[loop] = ptReadAddr[loop];
        }

    }

    return (sp_tFlashDesrc->BytesPerPage);
}

// ============================================================================
// 功能：查找page所在sector
// 参数：PageNo -- 页号
//       Remains -- 剩余页数
//       SectorNo -- 页所在sector
// 返回：
// 备注： sector0和normal sector大小时一样的，将其区分开来，只是为了便于阅读理解
// ============================================================================
s32 Flash_PageToSector(u32 PageNo, u32 *Remains, u32 *SectorNo)
{
    s32 ret;
    u16 secNum;
    u16 remainPage;

    //该页处于第几扇区
    secNum      = PageNo * sp_tFlashDesrc->BytesPerPage / SECTOR_SIZE;
    remainPage  = PageNo * sp_tFlashDesrc->BytesPerPage % SECTOR_SIZE /PAGE_SIZE;
    SectorNo[0] = secNum;
    Remains [0] = remainPage;

    return secNum;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================

s32 ModuleInstall_EmbededFlash(const char *ChipName, u32 Flags, u16 ResPages)
{
    u32 Len;
    struct FlashChip *Chip;
    struct MutexLCB *FlashLock;
    struct EmFlashDescr FlashDescr;
    u8 *Buf;
    s32 Ret = 0;

    if (!ChipName)
        return (-1);

    if(sp_tFlashDesrc)
        return (-4); // 设备已注册

    sp_tFlashDesrc = malloc(sizeof(*sp_tFlashDesrc));
    if(!sp_tFlashDesrc)
        return (-1);

    //初始化FLASH信息
    if(Flash_Init(sp_tFlashDesrc))
    {
        error_printf("null","解析内置FLASH信息失败\r\n");
        Ret = -3;
        goto __FAILURE;
    }

    Flash_GetDescr(&FlashDescr);// 获取FLASH信息
    if(ResPages > FlashDescr.TotalPages)
    {
        Ret = -1;
        goto __FAILURE;
    }

    FlashDescr.ReservedPages += ResPages;
    Len = strlen (ChipName) + 1;
    Chip = (struct FlashChip*) malloc(sizeof(struct FlashChip) + Len);
    if (NULL == Chip)
    {
//        TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
        error_printf("null","out of memory!\r\n");
        Ret = -2;
        goto __FAILURE;
    }

    memset(Chip, 0x00, sizeof(*Chip));
    Chip->dwPageBytes             = FlashDescr.BytesPerPage;
    Chip->dwPagesReserved         = FlashDescr.ReservedPages;
    Chip->dwTotalPages            = FlashDescr.TotalPages;
    Chip->Type                    = F_ALIEN;
    Chip->Descr.Embd              = FlashDescr;
    Chip->Ops.ErsBlk              = Flash_SectorEarse;
    Chip->Ops.WrPage              = Flash_PageProgram;
    Chip->Ops.RdPage              = Flash_PageRead;
    Chip->Ops.PageToBlk           = Flash_PageToSector;

    if(Flags & FLASH_BUFFERED)
    {
        Buf = (u8*)malloc(sp_tFlashDesrc->BytesPerPage);
        if(!Buf)
        {
//            TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
            error_printf("null","out of memory!\r\n");
            Ret = -2;
            goto __FAILURE;
        }

        FlashLock = Lock_MutexCreate("Embedded Flash Lock");
        if(!FlashLock)
        {
            Ret = -3;
            goto __FAILURE;
        }

        Chip->Buf = Buf;
        Chip->Lock =(void*)FlashLock;
    }

    if(-1 == dev_add(NULL, Chip->Name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip)) // 设备接入"/dev"
    {
        info_printf("null","device","add embedded flash falied.");
        Ret = -3;
        goto __FAILURE;
    }

    if(Flags & FLASH_ERASE_ALL)
        EarseWholeChip(Chip);

    __FAILURE:
    if(Ret)
    {
        if(sp_tFlashDesrc)
            free(sp_tFlashDesrc);
        if(FlashLock)
            Lock_MutexDelete(FlashLock);
        if(Buf)
            free(Buf);
        if(Chip)
            free(Chip);
    }
    return (Ret);

}






























































































































































































































































































































































