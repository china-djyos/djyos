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

//思路:
//   采用一个buf表示页和oob区域,而不是将他们分开表示。原因是读部分数据时,ECC校验
//   却是需要整个页和OOB数据。因此带ECC校验时,每次都需读写的整个页和OOB的数据才行
//

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <device.h>
#include <device/djy_flash.h>
#include <cpu_peri.h>
#include <int.h>
#include <device/unit_media.h>
#include <dbug.h>
#include <djyfs/filesystems.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。


//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//   extern s32 ModuleInstall_NorFlash(u32 doformat)
//   ModuleInstall_NorFlash(CFG_NFLASH_PART_FORMAT);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu peripheral register definition"//CPU的nor驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","file system"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"xip_app","xip_iboot"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_PERIPHERAL_REGISTER_DEFINITION == false )
//#warning  " cpu_peripheral_register_definition  组件参数未配置，使用默认配置"
//%$#@target = header   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_PERIPHERAL_REGISTER_DEFINITION    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,true,false,
#define CFG_NORFLASH_PART_FORMAT     false      //是否需要擦除该芯片。
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure

//W25Q64容量大小8M-每页256Bytes 每扇区4K 每块 32k或者64K可配

static struct umedia *emflash_um;

struct NorFlashDescr{
    u16     BytesPerPage;                // 页中包含的字节数-256Bytes
    u16     BytesPerSector;              // 扇区中的字节数
    u16     PagesPerSector;              // 每扇区中的页数
//    u16     SectorsPerBlock;           // 每块中的扇区数
    u16     PagesPerBlock;               //每块中的页数
    u32     TotalPages;                  // 总的页数
    u32     TotalBlocks;                  //总的块数
    u32     MappedStAddr;
} *sgpt_NorFlash;


#define CN_PAGE_SIZE   (256)             //页大小
#define CN_SECTOR_SIZE (4*1024)          //扇区大小
#define CN_BLOCK_SIZE  (64*1024)         //块大小
#define CN_CHIP_SIZE   (8*1024*1024)     //芯片大小

extern void flash_write_bytes(uint32_t addr,uint8_t *buf, uint8_t len);
extern void flash_read_bytes(uint32_t addr,uint8_t *buf,uint32_t len);
extern void flash_erase(uint8_t cmd,u32 addr);
static bool_t sflashInited = false;
static struct MutexLCB *sgpt_FlashLock;
#define NFlashLockTimeOut 0xffffffff

static const char *flashName = "norFlash";      //该flash在obj在的名字

//新接口
extern struct Object *s_ptDeviceRoot;
//s32 __Flash_FsInstallInit(const char *fs, u32 bstart, u32 bcount);
static s32 __Flash_init(void);

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
// 功能：Flash 信息初始化
// 参数：入口参数:NONE
// 返回：
// 备注：
// ============================================================================
static s32 __Flash_init(void)
{
    if(!sgpt_NorFlash)
        return (-1);

    if(sgpt_NorFlash == NULL)
    {
        sgpt_FlashLock = Lock_MutexCreate("Nor Flash Lock");        //创建nand的互斥锁
    }
//    stm32f7_NAND_ControllerConfig();// 芯片管脚等基本配置

    sgpt_FlashLock = Lock_MutexCreate("Nand Flash Lock");        //创建nand的互斥锁
    sgpt_NorFlash->BytesPerPage   = CN_PAGE_SIZE;
    sgpt_NorFlash->BytesPerSector = CN_SECTOR_SIZE;
    sgpt_NorFlash->PagesPerBlock  = CN_BLOCK_SIZE / CN_PAGE_SIZE;
    sgpt_NorFlash->TotalBlocks    = CN_CHIP_SIZE / CN_BLOCK_SIZE;
    sgpt_NorFlash->PagesPerSector = CN_SECTOR_SIZE /CN_PAGE_SIZE;
    sgpt_NorFlash->MappedStAddr   = 0x0;
    sgpt_NorFlash->TotalPages     = CN_CHIP_SIZE / CN_PAGE_SIZE;

    return (0);
}

//-----------------------------------------------------------------------------
//功能:获取flash信息
//参数:
//返回: "0" -- 成功; "-1" -- 解析失败; "-2" -- 内存不足;
//备注:
//-----------------------------------------------------------------------------

static s32 Flash_GetDescr(struct NorFlashDescr *Descr)
{
    if(Descr == NULL)
    {
        return -1;
    }

    Descr->BytesPerPage   = sgpt_NorFlash->BytesPerPage;
    Descr->BytesPerSector = sgpt_NorFlash->BytesPerSector;
    Descr->PagesPerBlock  = sgpt_NorFlash->PagesPerBlock;
    Descr->TotalBlocks    = sgpt_NorFlash->TotalBlocks;
    Descr->TotalPages     = sgpt_NorFlash->TotalPages;
    Descr->PagesPerSector = sgpt_NorFlash->PagesPerSector;
    Descr->MappedStAddr   = sgpt_NorFlash->MappedStAddr;

    return (0);
}

//-----------------------------------------------------------------------------
//功能:块擦除 - 现在设置每块大小为64K
//参数:块号
//返回: "0" -- 成功;"-1" -- 失败;
//备注:
//-----------------------------------------------------------------------------

s32 Flash_BlockErase(u32 BlkNo)
{
    u16 BlockAddress;

    BlockAddress = BlkNo * 64*1024;

    if(Lock_MutexPend(sgpt_FlashLock, NFlashLockTimeOut) == false)
    {
        return (-1);
    }
    #ifndef FLASH_BLOCK_64K_ERASE
    #define FLASH_BLOCK_64K_ERASE 0xD8
    #endif
    flash_erase(FLASH_BLOCK_64K_ERASE,BlockAddress);
    Lock_MutexPost(sgpt_FlashLock);

    return (0);    //成功
}


//-----------------------------------------------------------------------------
//功能:
//参数: Data -- 数据空间。不能为NULL。
//      Flags --
//返回: ">0" -- 正常，写入的字节数;
//      "-1" -- 参数错误;
//      "-2" -- 写失败;
//备注:
//-----------------------------------------------------------------------------
s32 Flash_PageProgram(u32 PageNo, u8 *Data, u32 Flags)
{
    u8 *Buf = (u8 *)Data;
    u32 PageAddress;

    if(Lock_MutexPend(sgpt_FlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    PageAddress = sgpt_NorFlash->MappedStAddr + PageNo*sgpt_NorFlash->BytesPerPage;
    flash_write_bytes(PageAddress,Buf,128);
    flash_write_bytes(PageAddress+128,Buf+128,128);

    Lock_MutexPost(sgpt_FlashLock);

    return (sgpt_NorFlash->BytesPerPage);

}

//-----------------------------------------------------------------------------
//功能:
//参数:
//
//备注: 不管读写是否正确，都将数据回传
//-----------------------------------------------------------------------------

s32  Flash_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    //逻辑:
    u8 *Buf = (u8 *)Data;
    u32 PageAddress;

    if(Lock_MutexPend(sgpt_FlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    PageAddress = sgpt_NorFlash->MappedStAddr + PageNo*sgpt_NorFlash->BytesPerPage;
    flash_read_bytes(PageAddress,Buf,sgpt_NorFlash->BytesPerPage);
    Lock_MutexPost(sgpt_FlashLock);

    return (sgpt_NorFlash->BytesPerPage);
}

u32 Flash_ReadID(void)
{
    u32 id;

    id = flash_read_jedec_id();

    return id;
}

// ============================================================================
// 功能：nand 命令
// 参数：ucmd -- 命令；
//      其他 -- 命令参数；
// 返回：
// 备注：
// ============================================================================
s32 __Flash_req(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case remain:                                 //所在块剩余页
        {
            va_list list;
            u32 *left;
            s64 *unit;

            left = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            *left = sgpt_NorFlash->PagesPerBlock - (*unit + 1) % sgpt_NorFlash->PagesPerBlock;
            break;
        }
        case whichblock:                              //哪一块
        {
            va_list list;
            u32 *block;
            s64 unit;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64)va_arg(list, u32);            //unit 第几页
            va_end(list);
            *block = unit / sgpt_NorFlash->PagesPerBlock;
            break;
        }

        case totalblocks:                             //总的块数
        {
            // args = &blocks
            *((u32*)args) =  sgpt_NorFlash->TotalBlocks;
            break;
        }

        case blockunits:                              //每块中的页数量
        {
            // args = &units
            *((u32*)args) = sgpt_NorFlash->PagesPerBlock;
            break;
        }

        case unitbytes:                               //每页的字节数
        {
            // args = &bytes
            *((u32*)args) = sgpt_NorFlash->BytesPerPage;
            break;
        }

        case format:
        {
            va_list list;
            u32 start, end;
            struct uesz *sz;

            start = (u32)args;                    //开始块
            va_start(list, args);
            end = va_arg(list, u32);              //结束块
            sz = (struct uesz*)va_arg(list, u32); //大小
            va_end(list);

            if(!sz->block)
                return (-1);

            if(-1==(s32)end)                      //结束块号判断
                end = CN_CHIP_SIZE / CN_BLOCK_SIZE;// 结束的号

            do
            {
                if(__Flash_erase((s64)--end, *sz))
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

           *((u32*)args) = sgpt_NorFlash->MappedStAddr;
           break;
        }
        case checkbad: break;
        default: res = -1; break;
    }

    return (res);
}

// ============================================================================
// 功能：nand 读；
// 参数：unit -- 读的序号（页）；
//      data -- 读的数据；
//      opt -- 读的方式；
// 返回：成功 -- （0）；失败 -- （-1）
// 备注：
// ============================================================================
s32 __Flash_read(s64 unit, void *data, struct uopt opt)
{
    s32 res;

    res = Flash_PageRead((u32)unit, data, 0);
    if(res!=sgpt_NorFlash->BytesPerPage)
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

s32 __Flash_write(s64 unit, void *data, struct uopt opt)
{
    s32 res;

    res = Flash_PageProgram((u32)unit, data, 0);
    if(res!=sgpt_NorFlash->BytesPerPage)
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

s32 __Flash_erase(s64 unit, struct uesz sz)
{
    u32 block;

    if(sz.unit)
    {
        if(__Flash_req(whichblock, (ptu32_t)&block, &unit))
            return (-1);
    }
    else
        block = (u32)unit;

    return (Flash_BlockErase(block));

}


// ============================================================================
// 功能：初始化片内flash
// 参数：fs -- 需要挂载的文件系统，MountPart -- 挂载到该媒体的第几个分区（分区从0开始）
//       bstart -- 起始块，bend -- 结束块（不包括该块，只到该块的上一块）
// 返回：0 -- 成功， -1 -- 失败
// 备注：
// ============================================================================

s32 __Flash_FsInstallInit(const char *fs, u32 bstart, u32 bend, void *mediadrv)
{
    u32 units, total = 0,endblock = bend;
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res;
    if(mediadrv == NULL)
        return -1;
    targetobj = OBJ_MatchPath(fs, &notfind);
    if(notfind)
    {
        error_printf("embed"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj);
    super->MediaInfo = emflash_um;
    super->MediaDrv = mediadrv;

    if(-1 == (s32)endblock)
    {
        endblock = bend = 128;// 最大块号
    }

    do
    {
        if(__Flash_req(blockunits, (ptu32_t)&units, --endblock))
        {
            return (-1);
        }

        total += units;     //计算该分区一共有多少页
    }
    while(endblock!=bstart);

    super->AreaSize = total * sgpt_NorFlash->BytesPerPage;
    endblock = 0;
    total = 0;
    while(endblock<bstart)
    {
        if(__Flash_req(blockunits, (ptu32_t)&units, endblock++))
        {
            return (-1);
        }
        total += units;
    }

    super->MediaStart = total; // 起始unit号

    res = strlen(flashName) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,flashName);   //获取该设备的全路径
    File_BeMedia(FullPath,fs); //往该设备挂载文件系统
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);

}

//-----------------------------------------------------------------------------
// 功能：安装片内Flash驱动
// 参数：  doformat -- 是擦除芯片化；
// 返回：成功（0）；失败（-1）；
// 备注：
//-----------------------------------------------------------------------------
s32 ModuleInstall_NorFlash(u32 doformat)
{
    if(!sgpt_NorFlash)
    {
        sgpt_NorFlash = malloc(sizeof(*sgpt_NorFlash));
        if(!sgpt_NorFlash)
        {
            return (-1);
        }

        __Flash_init();
    }

    if(doformat)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __Flash_req(format, (ptu32_t)0 , -1, &sz);           //格式化指定区域
    }

    emflash_um = malloc(sizeof(struct umedia)+sgpt_NorFlash->BytesPerPage);
    if(!emflash_um)
    {
        return (-1);
    }
    emflash_um->mreq = __Flash_req;
    emflash_um->type = embed;
    emflash_um->ubuf = (u8*)emflash_um + sizeof(struct umedia);

    if(!Device_Create((const char*)flashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)emflash_um)))
    {
        printf("\r\n: erro : device : %s addition failed.", flashName);
        free(emflash_um);
        return (-1);
    }

    sflashInited = true;
    return 0;
}

// =============================================================================
// 功能：判断flash是否安装
// 参数：  无
// 返回：已成功安装（true）；未成功安装（false）；
// 备注：
// =============================================================================
bool_t Norflash_is_install(void)
{
    return sflashInited;
}



