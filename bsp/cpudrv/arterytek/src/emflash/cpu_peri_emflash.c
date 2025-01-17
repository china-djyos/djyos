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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

#include <string.h>
#include <stdlib.h>
#include <device.h>
#include <device/djy_flash.h>
#include <cpu_peri.h>
#include <int.h>
#include <math.h>
#include <device/unit_media.h>
#include <dbug.h>
#include <xip.h>
#include <djyfs/filesystems.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//s32 ModuleInstall_EmbededFlash(u32 doformat);
//ModuleInstall_EmbededFlash(CFG_EFLASH_PART_FORMAT);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu drive inner flash"//片内flash
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                         //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                      //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                        //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                       //初始化时机，可选值：early，medium，later, pre-main。
                                        //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","lock"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                        //选中该组件时，被依赖组件将强制选中，
                                        //如果依赖多个组件，则依次列出
//weakdependence:"xip app file system","xip iboot file system"  //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                        //选中该组件时，被依赖组件不会被强制选中，
                                        //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                          //该组件的互斥组件名（可以是none，表示无互斥组件），
                                        //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_DRIVE_INNER_FLASH == false )
//#warning  " cpu_drive_inner_flash  组件参数未配置，使用默认配置"
//%$#@target = header   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_DRIVE_INNER_FLASH    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,true,false,
#define CFG_EFLASH_PART_FORMAT     false      //分区选项,是否需要擦除该芯片。
//%$#@num,0,,
#define CFG_EFLASH_PAGE_SIZE                512         //"片内flash的页大小单位字节"其实是自己定了为了统一，实际上芯片手册并没有规定
#define CFG_EFLASH_SECT_PAGE_NUM            4           //"片内flash的扇区中有多少页"
#define CFG_EFLASH_BLOCK_SECT_NUM           32          //"片内flash的块中有多少扇区"
#define CFG_EFLASH_PLANE1_SECT_NUM          256         //"片1一共有多少扇区"
#define CFG_EFLASH_PLANE2_SECT_NUM          256         //"片2一共有多少扇区"
#define CFG_EFLASH_PLANE1_BLOCK_NUM         8           //"片1一共有多少块"
#define CFG_EFLASH_PLANE2_BLOCK_NUM         8           //"片2一共有多少块"
//%$#@string,1,32,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure
// ============================================================================

static struct EmbdFlashDescr{
    u16     BytesPerPage;                   // 页中包含的字节数
    u16     PagesPerSect;                   // 一个扇区中的页数
    u16     SectsPerBolck;                  // 块中的扇区数
    u16     SectorsPerPlane1;               // 片1的扇区数
    u16     SectorsPerPlane2;               // 片2的扇区数
    u16     BlocksPerPlane1;               // 片1的块数
    u16     BlocksPerPlane2;               // 片2的块数
    u32     MappedStAddr;
} *s_ptEmbdFlash;
extern u32 gc_ptIbootSize;

static const char *EmflashName = "emflash";      //该flash在obj在的名字
static struct umedia *emflash_um;
extern struct Object *s_ptDeviceRoot;
static bool_t sEmflashInited = false;
s32 __embed_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
s32 __embed_read(s64 unit, void *data, struct uopt opt);
s32 __embed_req(enum ucmd cmd, ptu32_t args, ...);
s32 __embed_write(s64 unit, void *data, struct uopt opt);
s32 __embed_erase(s64 unit, struct uesz sz);
// ============================================================================
// 功能： 喂狗
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
// 功能： 内置FLASH初始化
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 Flash_Init(struct EmbdFlashDescr *Description)
{
    Description->BytesPerPage = CFG_EFLASH_PAGE_SIZE;
    Description->PagesPerSect = CFG_EFLASH_SECT_PAGE_NUM;
    Description->SectsPerBolck = CFG_EFLASH_BLOCK_SECT_NUM;
    Description->SectorsPerPlane1 = CFG_EFLASH_PLANE1_SECT_NUM;
    Description->SectorsPerPlane2 = CFG_EFLASH_PLANE2_SECT_NUM;
    Description->BlocksPerPlane1 = CFG_EFLASH_PLANE1_BLOCK_NUM;
    Description->BlocksPerPlane2 = CFG_EFLASH_PLANE2_BLOCK_NUM;
    Description->MappedStAddr = FLASH_BASE;
    return (0);
}

// ============================================================================
// 功能： 擦除扇区
// 参数： SectorNo -- 扇区编号;
// 返回： "0" -- 成功;"-1" -- 失败;
// 备注：
// ============================================================================
static s32 Flash_SectorEarse(u32 SectorNo)
{
    s32 Ret = -1;
    flash_status_type status = FLASH_OPERATE_DONE;
    flash_unlock();

    status = flash_operation_wait_for(ERASE_TIMEOUT);

    if(status != FLASH_OPERATE_TIMEOUT)
    {
        if((status == FLASH_PROGRAM_ERROR) || (status == FLASH_EPP_ERROR))
        {
            flash_flag_clear(FLASH_PRGMERR_FLAG | FLASH_EPPERR_FLAG);
        }
        Int_CutTrunk();
        status = flash_sector_erase(SectorNo * s_ptEmbdFlash->PagesPerSect * s_ptEmbdFlash->BytesPerPage + s_ptEmbdFlash->MappedStAddr);
        Int_ContactTrunk();
        if(status == FLASH_OPERATE_DONE)
        {
            Ret = 0;
        }
    }

    flash_lock();
    return Ret;
}

// ============================================================================
// 功能： 擦除块
// 参数： BlockNo -- 扇区编号;
// 返回： "0" -- 成功;"-1" -- 失败;
// 备注：
// ============================================================================
static s32 Flash_BlockEarse(u32 BlockNo)
{
    s32 Ret = -1;
    flash_status_type status = FLASH_OPERATE_DONE;
    flash_unlock();

    status = flash_operation_wait_for(ERASE_TIMEOUT);

    if(status != FLASH_OPERATE_TIMEOUT)
    {
        if((status == FLASH_PROGRAM_ERROR) || (status == FLASH_EPP_ERROR))
        {
            flash_flag_clear(FLASH_PRGMERR_FLAG | FLASH_EPPERR_FLAG);
        }
        Int_CutTrunk();
        status = flash_block_erase(BlockNo * s_ptEmbdFlash->SectsPerBolck * 
                    s_ptEmbdFlash->PagesPerSect * s_ptEmbdFlash->BytesPerPage + s_ptEmbdFlash->MappedStAddr);
        Int_ContactTrunk();
        if(status == FLASH_OPERATE_DONE)
        {
            Ret = 0;
        }
    }

    flash_lock();
    return Ret;
}

// ============================================================================
// 功能：写某页
// 参数：PageNo -- 页号;
//       Data -- 写缓冲;
//       Flags -- 写操作控制位;
// 返回：">0" -- 正常，写入的字节数;
//       "-1" -- 参数错误;
// 备注：
// ============================================================================
static s32 Flash_PageProgram(u32 Page, u8 *Data, u32 Flags)
{
    u32 i = 0;
    flash_status_type status = FLASH_OPERATE_DONE;
    u32 *pData = (u32*)Data;

    u32 Addr = Page * s_ptEmbdFlash->BytesPerPage + s_ptEmbdFlash->MappedStAddr;

    if(!Data)
        return (-1);

    flash_unlock();

    for(i = 0; i < s_ptEmbdFlash->BytesPerPage;)
    {
        if(*(u32*)Addr != *pData)
        {
            Int_CutTrunk();
            status = flash_word_program(Addr, *pData);
            Int_ContactTrunk();
            if(status != FLASH_OPERATE_DONE)
            {
                break;
            }
        }
        pData++;
        i += 4;
        Addr += 4;
    }

    flash_lock();

    return i;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 Flash_PageRead(u32 Page, u8 *Data, u32 Flags)
{
    u32 Addr = Page * s_ptEmbdFlash->BytesPerPage + s_ptEmbdFlash->MappedStAddr;

    if(!Data)
        return (-1);

    memcpy(Data, (u8*)Addr, s_ptEmbdFlash->BytesPerPage);
    return (s_ptEmbdFlash->BytesPerPage);

}

//-----------------------------------------------------------------------------
// 功能：安装片内Flash驱动
// 参数：doformat -- 是否格式化；
// 返回：成功（0）；失败（-1）；
// 备注：如果还不知道要安装什么文件系统，或者不安装文件系统TargetFs填NULL，TargetPart填-1；
//-----------------------------------------------------------------------------
s32 ModuleInstall_EmbededFlash(u32 doformat)
{
    if(s_ptEmbdFlash)
        return (-4); // 设备已注册

    s_ptEmbdFlash = malloc(sizeof(*s_ptEmbdFlash));
    if(!s_ptEmbdFlash)
        return (-1);

    Flash_Init(s_ptEmbdFlash);

    if(doformat)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __embed_req(format, (ptu32_t)0 , -1, &sz);           //格式化指定区域
    }

    emflash_um = malloc(sizeof(struct umedia)+s_ptEmbdFlash->BytesPerPage);
    if(!emflash_um)
    {
        return (-1);
    }

    emflash_um->mreq = __embed_req;
    emflash_um->type = embed;
    emflash_um->ubuf = (u8*)emflash_um + sizeof(struct umedia);

    if(!Device_Create((const char*)EmflashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)emflash_um)))
    {
        printf("\r\n: erro : device : %s addition failed.", EmflashName);
        free(emflash_um);
        return (-1);
    }
    sEmflashInited = true;
    return (0);
}
// =============================================================================
// 功能：判断emflash是否安装
// 参数：  无
// 返回：已成功安装（true）；未成功安装（false）；
// 备注：
// =============================================================================
bool_t emflash_is_install(void)
{
    return sEmflashInited;
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

            // 减1是因为，比如当前块一共有8页，*unit是0的话，代表当前页是第0页，返回还剩7页
            *left = s_ptEmbdFlash->SectsPerBolck * s_ptEmbdFlash->PagesPerSect 
                        - (*unit % (s_ptEmbdFlash->SectsPerBolck * s_ptEmbdFlash->PagesPerSect)) - 1;
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

            *block = *unit / (s_ptEmbdFlash->SectsPerBolck * s_ptEmbdFlash->PagesPerSect);
            break;
        }

        case totalblocks:
        {
            *((u32*)args) = s_ptEmbdFlash->BlocksPerPlane1 + s_ptEmbdFlash->BlocksPerPlane2;
            break;
        }
        case totalSectors:
        {
            *((u32*)args) = s_ptEmbdFlash->SectorsPerPlane1 + s_ptEmbdFlash->SectorsPerPlane2;
            break;
        }

        case blockunits:
        {
            va_list list;
            u32 *units = (u32*)args;

            *units = s_ptEmbdFlash->SectsPerBolck * s_ptEmbdFlash->PagesPerSect;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = s_ptEmbdFlash->BytesPerPage;
            break;
        }

        case format:
        {
            va_list list;
            s32 start, end;
            struct uesz *sz;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(!sz->block)
                return (-1);

            if(-1==(s32)end)
                end = s_ptEmbdFlash->BlocksPerPlane1 + s_ptEmbdFlash->BlocksPerPlane2; // 结束的号；

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

            *((u32*)args) = s_ptEmbdFlash->MappedStAddr;
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
s32 __embed_read(s64 unit, void *data, struct uopt opt)
{
    s32 res;

    res = Flash_PageRead((u32)unit, data, 0);
    if(res!=s_ptEmbdFlash->BytesPerPage)
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
s32 __embed_write(s64 unit, void *data, struct uopt opt)
{
    s32 res;

    res = Flash_PageProgram((u32)unit, data, 0);
    if(res!=s_ptEmbdFlash->BytesPerPage)
        return (-1);

    return (0);
}

// ============================================================================
// 功能：embed 擦除
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

    return (Flash_BlockEarse(block));
}


// ============================================================================
// 功能：初始化片内flash
// 参数：fs -- 需要挂载的文件系统，mediadrv -- 媒体驱动，
//       bstart -- 起始块，bend -- 结束块（不包括该块，只到该块的上一块）
// 返回：0 -- 成功， -1 -- 失败
// 备注：
// ============================================================================
s32 __embed_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv)
{
    u32 units, total = 0;
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res,endblock = bend;
    targetobj = OBJ_MatchPath(fs, &notfind);
    if(notfind)
    {
        error_printf("embed"," not found need to install file system.\r\n");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj);
    super->MediaInfo = emflash_um;
    super->MediaDrv = mediadrv;

    if(-1 == (s32)endblock)
    {
        endblock = bend = s_ptEmbdFlash->BlocksPerPlane1 + s_ptEmbdFlash->BlocksPerPlane2;
    }

    do
    {
        if(__embed_req(blockunits, (ptu32_t)&units, --endblock))
        {
            return (-1);
        }

        total += units;     //计算该分区一共有多少页
    }
    while(endblock!=bstart);

    super->AreaSize = total * s_ptEmbdFlash->BytesPerPage;
    endblock = 0;
    total = 0;
    while(endblock<bstart)
    {
        if(__embed_req(blockunits, (ptu32_t)&units, endblock++))
        {
            return (-1);
        }
        total += units;
    }

    super->MediaStart = total; // 起始unit号

    res = strlen(EmflashName) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,EmflashName);   //获取该设备的全路径
    File_BeMedia(FullPath,fs); //往该设备挂载文件系统
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);

}

void djy_flash_write_ori(uint32_t address, u8 *data, uint32_t size)
{
    u32 i = 0;
    flash_status_type status = FLASH_OPERATE_DONE;
    u32 len = 0;
    u32 *pData = NULL;

    if (size % 4)
    {
        len = (size / 4 + 1) * 4;
    }

    pData = malloc(len);

    if (pData)
    {
        memset(pData, 0xff, len);
        memcpy((char *)pData, data, size);
        flash_unlock();

        for(i = 0; i < size;)
        {
            if(*(u32*)address == 0xffffffff)
            {
                Int_CutTrunk();
                status = flash_word_program(address, *pData);
                Int_ContactTrunk();
                if(status != FLASH_OPERATE_DONE)
                {
                    break;
                }
            }
            pData++;
            i += 4;
            address += 4;
        }

        flash_lock();
        free(pData);
    }
    else
    {
        error_printf("embed"," malloc fail.\r\n");
    }

}

