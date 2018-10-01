//-----------------------------------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 著作权由都江堰操作系统开源团队所有。著作权人保留一切权利。
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

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <device.h>
#include <device/flash/flash.h>
#include <cpu_peri.h>
#include <stm32f7xx_hal_flash.h>
#include <int.h>
#include <device/include/unit_media.h>

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//extern s32 __embed_part_init(u32 bstart, u32 bcount, u32 doformat);
//ModuleInstall_UnitMedia(__embed_part_init, CFG_PARTS, ...);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu_peri_emflash"     //片内flash读写
//parent:"iap"                          //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp组件                      //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:可选                            //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                        //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                      //初始化时机，可选值：early，medium，later。
                                        //表示初始化时间，分别是早期、中期、后期
//dependence:"iap","devfile","lock","stm32f7", //该组件的依赖组件名（可以是none，表示无依赖组件），
                                        //选中该组件时，被依赖组件将强制选中，
                                        //如果依赖多个组件，则依次列出
//weakdependence:"none"                 //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                        //选中该组件时，被依赖组件不会被强制选中，
                                        //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                          //该组件的依赖组件名（可以是none，表示无依赖组件），
                                        //如果依赖多个组件，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用

//%$#@num,0,100,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#define CFG_EMBEDED     __embed_part_init    //"驱动初始化"
//%$#@object_num,0,1,2,3,4,..,100
#define CFG_PARTS   0     //分区数
//%$#@object_para,
#define CFG_PART_START      //分区起始
#define CFG_PART_SIZE       //分区大小
#define CFG_PART_OPTION     //分区选项
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure
// ============================================================================

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
} *embeddescription;

extern u32 gc_ptIbootSize;

// ============================================================================
// 功能：喂狗
// 参数：
// 返回：
// 备注：函数由硬件看门狗模块实现，例如max706.c
// ============================================================================
extern bool_t BrdWdt_FeedDog(void) __attribute__((weak));
bool_t BrdWdt_FeedDog(void)
{
    return (TRUE); // 空函数
}

// ============================================================================
// 功能：获取内置FLASH的信息
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 EmFlash_Init(struct EmbdFlashDescr *Description)
{
    Description->BytesPerPage = 512;
    Description->PagesPerSmallSect = 64;
    Description->PagesPerLargeSect = 256;
    Description->PagesPerNormalSect = 512;
    Description->SmallSectorsPerPlane = 4;
    Description->LargeSectorsPerPlane = 1;
    Description->NormalSectorsPerPlane = 7; // STM32F767
    Description->Planes = 1;
    Description->MappedStAddr = 0x08000000;
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
    Description->BytesPerPage = embeddescription->BytesPerPage;
    Description->TotalPages = (embeddescription->PagesPerSmallSect *
                        embeddescription->SmallSectorsPerPlane +
                        embeddescription->PagesPerLargeSect *
                        embeddescription->LargeSectorsPerPlane +
                        embeddescription->PagesPerNormalSect *
                        embeddescription->NormalSectorsPerPlane) *
                        embeddescription->Planes;
    Description->ReservedPages = gc_ptIbootSize / embeddescription->BytesPerPage;
    Description->MappedStAddr = embeddescription->MappedStAddr;
    return (0);
}

// ============================================================================
// 功能：擦除块
// 参数：BlkNo -- 块号;
// 返回："0" -- 成功;"-1" -- 失败;
// 备注：
// ============================================================================
static s32 Flash_SectorEarse(u32 SectorNo)
{
    u8 retry = 0; // 擦除有可能会失败；
    s32 Ret = -1;
    u32 SECTORError=0;//保存出错类型信息
    static FLASH_EraseInitTypeDef EraseInitStruct;

    do
    {
        HAL_FLASH_Unlock();
        EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
        EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
        EraseInitStruct.Sector        = SectorNo;
        EraseInitStruct.NbSectors     = 1;

        BrdWdt_FeedDog(); // 喂狗
        Int_CutTrunk();
        if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
             Ret=-1;
        else
            Ret=0;
        Int_ContactTrunk();
        BrdWdt_FeedDog(); // 喂狗
        HAL_FLASH_Lock();
        SCB_CleanInvalidateDCache();//块擦除后，需清cache，否则读flash可能数据错误
    }
    while(Ret&&((retry++)<1));

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
    u32 Ret,i;
    u32 *pData = (u32*)Data;
    u32 Addr = Page * embeddescription->BytesPerPage + embeddescription->MappedStAddr;

    if(!Data)
        return (-1);

    HAL_FLASH_Unlock();

    for(i = 0; i < embeddescription->BytesPerPage;)
    {
        if(*(u32*)Addr != *pData)
        {
            BrdWdt_FeedDog(); // 喂狗
            Int_CutTrunk();
            Ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Addr,*pData);
            Int_ContactTrunk();
            BrdWdt_FeedDog(); // 喂狗
            if(Ret != HAL_OK)
                break;
        }
        pData++;
        i += 4;
        Addr += 4;
    }

    HAL_FLASH_Lock();

    return (i);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 Flash_PageRead(u32 Page, u8 *Data, u32 Flags)
{
    u32 Addr = Page * embeddescription->BytesPerPage + embeddescription->MappedStAddr;

    if(!Data)
        return (-1);

    memcpy(Data, (u8*)Addr, embeddescription->BytesPerPage);
    return (embeddescription->BytesPerPage);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 Flash_PageToSector(u32 PageNo, u32 *Remains, u32 *SectorNo)
{
    u32 PagesLeft, PagesDone;
    u32 i;
    u32 Sector;

    if((!Remains) || (!SectorNo))
        return (-1);

    Sector = 0;
    PagesDone = 0;
    PagesLeft = embeddescription->PagesPerSmallSect -
               (PageNo % embeddescription->PagesPerSmallSect);
    for(i = 1; i <= embeddescription->SmallSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + embeddescription->PagesPerSmallSect * i))
            goto DONE;
        Sector++;
    }

    PagesDone += embeddescription->SmallSectorsPerPlane *
            embeddescription->PagesPerSmallSect;
    PagesLeft = embeddescription->PagesPerLargeSect -
                   (PageNo % embeddescription->PagesPerLargeSect);
    for(i = 1; i <= embeddescription->LargeSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + embeddescription->PagesPerLargeSect * i))
            goto DONE;
        Sector++;
    }

    PagesDone += embeddescription->LargeSectorsPerPlane *
            embeddescription->PagesPerLargeSect;
    PagesLeft = embeddescription->PagesPerNormalSect -
                   (PageNo % embeddescription->PagesPerNormalSect);
    for(i = 1; i <= embeddescription->NormalSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + embeddescription->PagesPerNormalSect * i))
            goto DONE;
        Sector++;
    }

    return (-1);

DONE:
    *SectorNo = Sector; // 从sector零计
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

    if(embeddescription)
        return (-4); // 设备已注册

    embeddescription = malloc(sizeof(*embeddescription));
    if(!embeddescription)
        return (-1);

    EmFlash_Init(embeddescription);
    Flash_GetDescr(&FlashDescr);// 获取FLASH信息
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
        printf("\r\n: erro : device : out of memory.\r\n");
        Ret = -2;
        goto FAILURE;
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
    Chip->Ops.PageToBlk              = Flash_PageToSector;
    strcpy(Chip->Name, ChipName); // 设备名
    if(Flags & FLASH_BUFFERED)
    {
        Buf = (u8*)malloc(embeddescription->BytesPerPage); // NAND底层缓冲
        if(!Buf)
        {
            printf("\r\n: erro : device : out of memory.");
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

    dev_add(NULL, Chip->Name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip); // 设备接入"/dev"
    if(Flags & FLASH_ERASE_ALL)
        EarseWholeChip(Chip);

FAILURE:
    if(Ret)
    {
        if(embeddescription)
            free(embeddescription);
        if(FlashLock)
            Lock_MutexDelete(FlashLock);
        if(Buf)
            free(Buf);
        if(Chip)
            free(Chip);
    }
    return (Ret);
}

// ============================================================================
// 功能：embeded flash 命令
// 参数：ucmd -- 命令；
//      其他 -- 命令参数；
// 返回：
// 备注：
// ============================================================================
s32 __embed_req(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case remain:
        {
            va_list list;
            u32 *left;
            s64 *unit;

            left = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);
            if((*unit<64)&&(*unit>=0))
                *left = 63 - *unit;
            else if((*unit<128)&&(*unit>=64))
                *left = 127 - *unit;
            else if((*unit<192)&&(*unit>=128))
                *left = 191 - *unit;
            else if((*unit<256)&&(*unit>=192))
                *left = 255 - *unit;
            else if((*unit<512)&&(*unit>=256))
                *left = 511 - *unit;
            else if((*unit<1024)&&(*unit>=512))
                *left = 1023 - *unit;
            else if((*unit<1536)&&(*unit>=1024))
                *left = 1535 - *unit;
            else if((*unit<2048)&&(*unit>=1536))
                *left = 2047 - *unit;
            else if((*unit<2560)&&(*unit>=2048))
                *left = 2559 - *unit;
            else if((*unit<3072)&&(*unit>=2560))
                *left = 3071 - *unit;
            else if((*unit<3584)&&(*unit>=3072))
                *left = 3583 - *unit;
            else if((*unit<4096)&&(*unit>=3584))
                *left = 40951 - *unit;
            else
                res = -1;

            break;
        }

        case whichblock:
        {
            va_list list;
            s64 *unit;
            u32 *block;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            if((*unit<64)&&(*unit>=0))
                *block = 0;
            else if((*unit<128)&&(*unit>=64))
                *block = 1;
            else if((*unit<192)&&(*unit>=128))
                *block = 2;
            else if((*unit<256)&&(*unit>=192))
                *block = 3;
            else if((*unit<512)&&(*unit>=256))
                *block = 4;
            else if((*unit<1024)&&(*unit>=512))
                *block = 5;
            else if((*unit<1536)&&(*unit>=1024))
                *block = 6;
            else if((*unit<2048)&&(*unit>=1536))
                *block = 7;
            else if((*unit<2560)&&(*unit>=2048))
                *block = 8;
            else if((*unit<3072)&&(*unit>=2560))
                *block = 9;
            else if((*unit<3584)&&(*unit>=3072))
                *block = 10;
            else if((*unit<4096)&&(*unit>=3584))
                *block = 11;
            else
                res = -1;

            break;
        }

        case totalblocks:
        {
            // args = &blocks
            *((u32*)args) =  12;
            break;
        }

        case blockunits:
        {
            va_list list;
            u32 *units = (u32*)args;
            u32 block;

            va_start(list, args);
            block = (u32)va_arg(list, u32);
            va_end(list);

            switch(block)
            {
                case 0:
                case 1:
                case 2:
                case 3: *units = embeddescription->PagesPerSmallSect; break;
                case 4: *units = embeddescription->PagesPerLargeSect; break;
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:*units = embeddescription->PagesPerNormalSect; break;
                default: res = -1;
            }
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = embeddescription->BytesPerPage;
            break;
        }

        case format:
        {
            va_list list;
            u32 start, end;
            struct uesz *sz;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(!sz->block)
                return (-1);

            if(-1==end)
                end = 12; // 结束的号；
            else if(start)
                end += start;

            do
            {
                if(__embed_erase((s64)--end, *sz))
                {
                    res = -1;
                    break;
                }
            }
            while(end!=start);

            break;
        }

        case mapaddr:
        {

            *((u32*)args) = embeddescription->MappedStAddr;
            break;
        }
        case checkbad: break;
        default: res = -1; break;
    }

    return (res);
}

// ============================================================================
// 功能：embed 读；
// 参数：unit -- 读的序号（页）；
//      data -- 读的数据；
//      opt -- 读的方式；
// 备注：
// ============================================================================
static s32 __embed_read(s64 unit, void *data, struct uopt opt)
{
    s32 res;

    res = Flash_PageRead((u32)unit, data, 0);
    if(res!=embeddescription->BytesPerPage)
        return (-1);

    return (0);
}

// ============================================================================
// 功能：embed 写；
// 参数：unit -- 写的序号（页）；
//      data -- 写的数据；
//      opt -- 写的方式；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __embed_write(s64 unit, void *data, struct uopt opt)
{
    s32 res;

    res = Flash_PageProgram((u32)unit, data, 0);
    if(res!=embeddescription->BytesPerPage)
        return (-1);

    return (0);
}

// ============================================================================
// 功能：nand 擦除
// 参数：unit -- 擦除的序号；
//      sz -- 擦除的单位（unit或block）
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 __embed_erase(s64 unit, struct uesz sz)
{
    u32 block;

    if(sz.unit)
    {
        if(__embed_req(whichblock, (ptu32_t)&block, &unit))
            return (-1);
    }
    else
        block = (u32)unit;

    return (Flash_SectorEarse(block));
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 __embed_part_init(u32 bstart, u32 bcount, u32 doformat)
{
    struct umedia *um;
    struct uopt opt;
    char name[16], part[3];
    u32 units, total = 0;
    static u8 count;

    if(!embeddescription)
    {
        embeddescription = malloc(sizeof(*embeddescription));
        if(!embeddescription)
        {
            return (-1);
        }

        EmFlash_Init(embeddescription);
    }

    if(doformat)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __embed_req(format, (ptu32_t)bstart , bcount, &sz);
    }

    um = malloc(sizeof(struct umedia)+embeddescription->BytesPerPage);
    if(!um)
    {
        return (-1);
    }

    opt.hecc = 1;
    opt.main = 1;
    opt.necc = 1;
    opt.secc = 0;
    opt.spare = 0;
    if(-1 == bcount)
        bcount = 12; // 最大块号
    else
        bcount += bstart; // 结束块号

    do
    {
        if(__embed_req(blockunits, (ptu32_t)&units, --bcount))
        {
            return (-1);
        }

        total += units;
    }
    while(bcount!=bstart);

    um->asz = total * embeddescription->BytesPerPage;
    um->esz = 0; // 各个区域不同
    um->usz = log2(embeddescription->BytesPerPage);
    um->merase = __embed_erase;
    um->mread = __embed_read;
    um->mreq = __embed_req;
    um->mwrite = __embed_write;
    um->opt = opt;
    um->type = embed;
    um->ubuf = (u8*)um + sizeof(struct umedia);
    bcount = 0;
    total = 0;
    while(bcount<bstart)
    {
        if(__embed_req(blockunits, (ptu32_t)&units, bcount++))
        {
            return (-1);
        }

        total += units;
    }

    um->ustart = total; // 起始unit号
    itoa(count++, part, 10);
    sprintf(name, "embed part %s", part);
    if(um_add((const char*)name, um))
    {
        printf("\r\n: erro : device : %s addition failed.", name);
        return (-1);
    }

    printf("\r\n: info : device : %s added(start:%d, blocks:%d).", name, bstart, bcount);
    return (0);

}

