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

#include <stdlib.h>
#include <string.h>
#include <device.h>
#include <device/djy_flash.h>
#include <flashd.h>
#include <cpu_peri.h>
#include <mpu.h>
#include <int.h>
#include "stdint.h"
#include "dbug.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern s32 ModuleInstall_EmbededFlash(const char *ChipName, u32 Flags, u16 ResPages);
//    ModuleInstall_EmbededFlash(CFG_FLASH_CHIP_NAME,CFG_FLASH_FLAG,CFG_FLASH_RES);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip flash"//CPU片内flash读写
//parent                       //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                               //初始化时机，可选值：early，medium，later, pre-main。
                                                //表示初始化时间，分别是早期、中期、后期
//dependence:"iap","device file system","heap","lock"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                                //选中该组件时，被依赖组件将强制选中，
                                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"                         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                                //选中该组件时，被依赖组件不会被强制选中，
                                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_FLASH == false )
//#warning  " cpu_onchip_flash  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_FLASH    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
#define CFG_FLASH_FLAG                   (2)        //"标记号"，1表示擦除芯片全部，2表示缓存
#define CFG_FLASH_RES                    (0)        //"保留页数或块数",
//%$#@enum,true,false,
//%$#@string,1,30,
#define CFG_FLASH_CHIP_NAME              "embedded flash"     //"芯片名",
//%$#@num,0,,
#define CFG_EFLASH_PAGE_SIZE                 512      //片内flash的页大小，单位字节。
#define CFG_EFLASH_SMALL_SECT_PAGE_NUM       16         //片内flash的小扇区中，有多少页。
#define CFG_EFLASH_LARGE_SECT_PAGE_NUM       224         //片内flash的大扇区中，有多少页。
#define CFG_EFLASH_NORMAL_SECT_PAGE_NUM      256         //片内flash的标准扇区中，有多少页。
#define CFG_EFLASH_PLANE_SMALL_SECT_NUM      2         //片内flash的主存储块中，有多少小扇区。
#define CFG_EFLASH_PLANE_LARGE_SECT_NUM      1         //片内flash的主存储块中，有多少大扇区。
#define CFG_EFLASH_PLANE_NORMAL_SECT_NUM     15       //片内flash的主存储块中，有多少标准扇区。
#define CFG_EFLASH_PLANE_NUM                 1         //片内flash的主存储块个数。
#define CFG_EFLASH_MAPPED_START_ADDR         0x400000 //片内flash的映射起始地址。

//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

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
}*sp_tEmFlashDesrc;

#define MPU_APP_FLASH_REGION     (12)
#define MPU_APP_START_ADDRESS    (0x00400000) // 内置FLASH起始地址
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
// 功能：
// 参数：
// 返回：
// 备注： todo 功能有些问题
// ============================================================================
void __CacheMpuEnable(u8 IsEnable)
{
    uint32_t dwRegionBaseAddr;
    uint32_t dwRegionAttr;

    Int_CutTrunk();

    /* Clean up data and instruction buffer */
    __DSB();
    __ISB();

    if(!IsEnable)
    {
        dwRegionBaseAddr =
            MPU_APP_START_ADDRESS |
            MPU_REGION_VALID |
            MPU_APP_FLASH_REGION;      //12

        //在升级程序时，可对flash 1M的位置读写
        dwRegionAttr =
            MPU_AP_FULL_ACCESS |
            INNER_OUTER_NORMAL_NOCACHE_TYPE( NON_SHAREABLE ) |
            MPU_CalMPURegionSize(IFLASH_END_ADDRESS - IFLASH_START_ADDRESS) |
            MPU_REGION_ENABLE;

        MPU_SetRegion(dwRegionBaseAddr,dwRegionAttr);
    }
    else
    {
        dwRegionBaseAddr =
                MPU_APP_START_ADDRESS |
                MPU_REGION_VALID |
                MPU_APP_FLASH_REGION;      //12

        dwRegionAttr =
            MPU_AP_READONLY |
            INNER_NORMAL_WB_NWA_TYPE( NON_SHAREABLE ) |
            MPU_CalMPURegionSize(IFLASH_END_ADDRESS - IFLASH_START_ADDRESS) |
            MPU_REGION_ENABLE;

        MPU_UpdateRegions(MPU_APP_FLASH_REGION, dwRegionBaseAddr, dwRegionAttr);
    }

    /* Clean up data and instruction buffer to make the new region taking
       effect at once */
    __DSB();
    __ISB();

    Int_ContactTrunk();
}

// ============================================================================
// 功能：获取内置FLASH的信息
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 EEFC_Init(struct EmbdFlashDescr *Description)
{
    Description->BytesPerPage = CFG_EFLASH_PAGE_SIZE;
    Description->PagesPerSmallSect = CFG_EFLASH_SMALL_SECT_PAGE_NUM;
    Description->PagesPerLargeSect = CFG_EFLASH_LARGE_SECT_PAGE_NUM;
    Description->PagesPerNormalSect = CFG_EFLASH_NORMAL_SECT_PAGE_NUM;
    Description->SmallSectorsPerPlane = CFG_EFLASH_PLANE_SMALL_SECT_NUM;
    Description->LargeSectorsPerPlane = CFG_EFLASH_PLANE_LARGE_SECT_NUM;
    Description->NormalSectorsPerPlane = CFG_EFLASH_PLANE_NORMAL_SECT_NUM;
    Description->Planes = CFG_EFLASH_PLANE_NUM;
    Description->MappedStAddr = CFG_EFLASH_MAPPED_START_ADDR;
    return (0);
}

// ============================================================================
// 功能：获取内置FLASH的信息
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 EEFC_GetDescr(struct EmFlashDescr *Description)
{
    Description->BytesPerPage = sp_tEmFlashDesrc->BytesPerPage;
    Description->TotalPages = (sp_tEmFlashDesrc->PagesPerSmallSect *
                            sp_tEmFlashDesrc->SmallSectorsPerPlane +
                            sp_tEmFlashDesrc->PagesPerLargeSect *
                            sp_tEmFlashDesrc->LargeSectorsPerPlane +
                            sp_tEmFlashDesrc->PagesPerNormalSect *
                            sp_tEmFlashDesrc->NormalSectorsPerPlane) *
                            sp_tEmFlashDesrc->Planes;
    Description->ReservedPages = gc_ptIbootSize / sp_tEmFlashDesrc->BytesPerPage;
    Description->MappedStAddr = sp_tEmFlashDesrc->MappedStAddr;
    return (0);
}

// ============================================================================
// 功能：擦除块
// 参数：BlkNo -- 块号;
// 返回："0" -- 成功;"-1" -- 失败;
// 备注：sector0和normal sector大小时一样的，将其区分开来，只是为了便于阅读理解
// ============================================================================
 s32 EEFC_SectorEarse(u32 SectorNo)
{
    s32 Res;
    u32 Addr;
    u32 End;
    u32 SizeofSect0 = sp_tEmFlashDesrc->BytesPerPage *
                    (sp_tEmFlashDesrc->PagesPerSmallSect *
                    sp_tEmFlashDesrc->SmallSectorsPerPlane +
                    sp_tEmFlashDesrc->PagesPerLargeSect *
                    sp_tEmFlashDesrc->LargeSectorsPerPlane); // 第0块的地址空间
    u32 SizeofNormalSect = sp_tEmFlashDesrc->BytesPerPage *
                           sp_tEmFlashDesrc->PagesPerNormalSect;

    if(!SectorNo) // sector 0
    {
        Addr = 0;
        End = Addr + SizeofSect0;
    }
    else
    {
        Addr =  SizeofSect0 + SizeofNormalSect * (SectorNo - 1);
        End = Addr + SizeofNormalSect;
    }

    Addr += sp_tEmFlashDesrc->MappedStAddr;
    End += sp_tEmFlashDesrc->MappedStAddr;

    __CacheMpuEnable(0);
    if(FLASHD_IsLocked(Addr, End))
        FLASHD_Unlock(Addr, End, NULL, NULL);
    BrdWdt_FeedDog(); // 喂狗
    Int_CutTrunk();
    Res = FLASHD_EraseSector(Addr);
    Int_ContactTrunk();
    BrdWdt_FeedDog(); // 喂狗

    if(Res)
        Res = -1;

    __CacheMpuEnable(1);

    return (Res);
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
s32 EEFC_PageProgram(u32 Page, u8 *Data, u32 Flags)
{
    u32 Ret;

    u32 Addr = Page * sp_tEmFlashDesrc->BytesPerPage + sp_tEmFlashDesrc->MappedStAddr;

    if(!Data)
        return (-1);

    __CacheMpuEnable(0);
    if(FLASHD_IsLocked(Addr, Addr+sp_tEmFlashDesrc->BytesPerPage))
        FLASHD_Unlock(Addr, Addr+sp_tEmFlashDesrc->BytesPerPage, NULL, NULL);

    BrdWdt_FeedDog(); // 喂狗
    Int_CutTrunk();
    Ret = FLASHD_Write(Addr, Data, sp_tEmFlashDesrc->BytesPerPage);
    Int_ContactTrunk();
    BrdWdt_FeedDog(); // 喂狗
    if(Ret)
        return (-2);

    __CacheMpuEnable(1);
    return (sp_tEmFlashDesrc->BytesPerPage);
}

// ============================================================================
// 功能：读某页
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 EEFC_PageRead(u32 Page, u8 *Data, u32 Flags)
{
    u32 Addr = Page * sp_tEmFlashDesrc->BytesPerPage + sp_tEmFlashDesrc->MappedStAddr;

    if(!Data)
        return (-1);

    memcpy(Data, (u8*)Addr, sp_tEmFlashDesrc->BytesPerPage);
    return (sp_tEmFlashDesrc->BytesPerPage);

}

// ============================================================================
// 功能：查找page所在sector
// 参数：PageNo -- 页号
//       Remains -- 剩余页数
//       SectorNo -- 页所在sector
// 返回：
// 备注：sector0和normal sector大小时一样的，将其区分开来，只是为了便于阅读理解
// ============================================================================
s32 EEFC_PageToSector(u32 PageNo, u32 *Remains, u32 *SectorNo)
{
    u32 PagesLeft, PagesDone;
    u32 i;
    u32 Sector;
    u32 PagesSector0;

    if((!Remains) || (!SectorNo))
        return (-1);

    Sector = 0;
    PagesDone = 0;
    PagesSector0 = ((sp_tEmFlashDesrc->PagesPerSmallSect *
                     sp_tEmFlashDesrc->SmallSectorsPerPlane) +
                    (sp_tEmFlashDesrc->PagesPerLargeSect *
                     sp_tEmFlashDesrc->LargeSectorsPerPlane));
    PagesLeft = PagesSector0 - (PageNo % PagesSector0);
    if(PageNo < (PagesDone + PagesSector0))
        goto DONE;
    Sector++;

    PagesDone += PagesSector0;
    PagesLeft = sp_tEmFlashDesrc->PagesPerNormalSect -
                ((PageNo - PagesDone) % sp_tEmFlashDesrc->PagesPerNormalSect);
    for(i = 1; i <= sp_tEmFlashDesrc->NormalSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + sp_tEmFlashDesrc->PagesPerNormalSect * i))
            goto DONE;
        Sector++;
    }

    return (-1);

DONE:
    *SectorNo = Sector; // sector从零计
    *Remains = PagesLeft -1; // page从零计
    return (0);
}

// ============================================================================
// 功能：
// 参数：ResPages：保留页数
// 返回：
// 备注：
// ============================================================================
s32 ModuleInstall_EmbededFlash(const char *ChipName, u32 Flags, u16 ResPages)
{

    u32 Len;
    struct FlashChip *Chip;
    struct EmFlashDescr FlashDescr;
    struct MutexLCB *FlashLock;
    u8 *Buf;
    s32 Ret = 0;

    if (!ChipName)
        return (-1);

    if(sp_tEmFlashDesrc)
        return (-4); // 设备已注册

    sp_tEmFlashDesrc = malloc(sizeof(*sp_tEmFlashDesrc));
    if(!sp_tEmFlashDesrc)
        return (-1);

    // 获取FLASH信息
    if(EEFC_Init(sp_tEmFlashDesrc))
    {
//        TraceDrv(FLASH_TRACE_ERROR, "解析内置FLASH信息失败\r\n");
        error_printf("null","解析内置FLASH信息失败\r\n");
        Ret = -3;
        goto FAILURE;
    }

    EEFC_GetDescr(&FlashDescr);
    if(ResPages > FlashDescr.TotalPages)
    {
        Ret = -1;
        goto FAILURE;
    }

    FlashDescr.ReservedPages += ResPages;
    Len = strlen (ChipName) + 1;
    Chip = (struct FlashChip*)malloc(sizeof(struct FlashChip) + Len);
    if (NULL == Chip)
    {
//        TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
        error_printf("null","out of memory!\r\n");
        Ret = -2;
        goto FAILURE;
    }

    memset(Chip, 0x00, sizeof(*Chip));
    Chip->dwPageBytes             = FlashDescr.BytesPerPage;
    Chip->dwPagesReserved         = FlashDescr.ReservedPages;
    Chip->dwTotalPages            = FlashDescr.TotalPages;
    Chip->Type                    = F_ALIEN;
    Chip->Descr.Embd              = FlashDescr;
    Chip->Ops.ErsBlk              = EEFC_SectorEarse;
    Chip->Ops.WrPage              = EEFC_PageProgram;
    Chip->Ops.RdPage              = EEFC_PageRead;
    Chip->Ops.PageToBlk           = EEFC_PageToSector;
    strcpy(Chip->Name, ChipName); // 设备名
    if(Flags & FLASH_BUFFERED)
    {
        Buf = (u8*)malloc(sp_tEmFlashDesrc->BytesPerPage);
        if(!Buf)
        {
//            TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
            error_printf("null","out of memory!\r\n");
            Ret = -2;
            goto FAILURE;
        }

        FlashLock = Lock_MutexCreate("Embedded Flash Lock");
        if(!FlashLock)
        {
            Ret = -3;
            goto FAILURE;
        }

        Chip->Buf = Buf;
        Chip->Lock =(void*)FlashLock;
    }

    if(-1 == Device_Create(Chip->Name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip)) // 设备接入"/dev"
    {
        info_printf("null","device","add embedded flash falied.");
        Ret = -3;
        goto FAILURE;
    }

    if(Flags & FLASH_ERASE_ALL)
        EarseWholeChip(Chip);

FAILURE:
    if(Ret)
    {
        if(sp_tEmFlashDesrc)
            free(sp_tEmFlashDesrc);
        if(FlashLock)
            Lock_MutexDelete(FlashLock);
        if(Buf)
            free(Buf);
        if(Chip)
            free(Chip);
    }
    return (Ret);
}
