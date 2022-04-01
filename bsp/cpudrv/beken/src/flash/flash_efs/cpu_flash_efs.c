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
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <cpu_peri.h>
#include <device/djy_flash.h>
#include <device/unit_media.h>
#include <device.h>
#include <spibus.h>
#include <systime.h>
#include <math.h>
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <djyfs/efs.h>

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_EmbFlashInstallEfs(const char *TargetFs, s32 bstart, s32 bend, u32 doformat);
//    ModuleInstall_EmbFlashInstallEfs(CFG_EMBFLASH_EFS_MOUNT_NAME, CFG_EMBFLASH_EFS_PART_START,
//                                                        CFG_EMBFLASH_EFS_PART_END, CFG_EMBFLASH_EFS_PART_FORMAT);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"embflash install efs"    //在norflash上安装efs文件系统
//parent:"cpu onchip flash"                //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","easy file system","lock","cpu onchip flash"
                                //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_EMBFLASH_INSTALL_EFS == false )
//#warning  " embflash_install_efs  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_EMBFLASH_INSTALL_EFS    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,512,528,
//%$#@string,1,10,
#define CFG_EMBFLASH_EFS_MOUNT_NAME            "efs"      //"文件系统mount点名字",需要挂载的efs文件系统mount点名字
//%$#@num,-1,1024
#define CFG_EMBFLASH_EFS_PART_START                  0        //"分区起始"，填写块号，块号从0开始计算
#define CFG_EMBFLASH_EFS_PART_END                   -1        //"分区结束"，-1是最后一块
//%$#@enum,true,false,
#define CFG_EMBFLASH_EFS_PART_FORMAT               false      //"分区选项",是否需要格式化该分区。
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

extern struct NorDescr *nordescription;
//extern bool_t flash_is_install(void);
//extern void djy_flash_read(uint32_t address, void *data, uint32_t size);
//extern void djy_flash_write(uint32_t address, const void *data, uint32_t size);
//extern void djy_flash_erase(uint32_t address);
//extern s32 djy_flash_req(enum ucmd cmd, ptu32_t args, ...);
extern s32 EmbFsInstallInit(const char *fs, s32 dwStart, s32 dwEnd,void *mediadrv);
//extern void djy_flash_read_crc(uint32_t address, void *data, uint32_t size);

extern void flash_protection_op(UINT8 mode, PROTECT_TYPE type);

u32 efs_embflash_write(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags);
u32 efs_embflash_read(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags);
bool_t efs_embflash_erase(u32 dwBlock);
bool_t efs_embflash_CheckBlockReady(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize);


struct __efs_media_drv EFS_EMBFLASH_DRV =
{
    .efs_erase_media = efs_embflash_erase,
    .efs_read_media = efs_embflash_read,
    .efs_write_media = efs_embflash_write,
    .efs_check_block_ready = efs_embflash_CheckBlockReady
};
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ============================================================================
// 功能： 写数据
// 参数： dwBlock -- 块号;
//       dwOffset -- 块内偏置;
//       pBuf -- 写缓冲;
//       dwSize -- 写数据大小
//       bFlags -- 无功能(0x0); ECC功能开启，数据大小为必须256（0x1）。
// 返回：错误（0）；成功（写出数据大小）；
// 备注：
// ============================================================================
u32 efs_embflash_write(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags)
{
    u32 addr = (dwBlock * nordescription->PagesPerSector * nordescription->BytesPerPage) + dwOffset;
    bool_t crc_start;
    if(!pBuf)
        return (0);

    if(dwSize > 0)
    {
        flash_protection_op(0,FLASH_PROTECT_NONE);
        djy_flash_req(lock, CN_TIMEOUT_FOREVER);
        crc_start = GetOperFalshMode();
        SetOperFalshMode(true);
        djy_flash_write(addr, pBuf, dwSize);
        SetOperFalshMode(crc_start);
        djy_flash_req(unlock, 0);
        flash_protection_op(0,FLASH_PROTECT_ALL);
    }
    return dwSize;
}

// ============================================================================
// 功能： 读数据
// 参数： dwBlock -- 块号;
//       Offset -- 块内偏置;
//       pBuf -- 读缓冲;
//       dwSize -- 读数据大小
//       bFlags -- 0x0 -- 无功能; 0x1 - ECC功能开启(数据大小为必须256)
// 返回：失败（0）；成功（读入的字节数）；
// 备注：
// ============================================================================
u32 efs_embflash_read(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags)
{
    u32 addr = (dwBlock * nordescription->PagesPerSector * nordescription->BytesPerPage) + dwOffset;
    if(!pBuf)
        return (0);
    djy_flash_req(lock, CN_TIMEOUT_FOREVER);
    if(dwSize > 0)
    {
        djy_flash_read_crc(addr, pBuf, dwSize);
    }
    djy_flash_req(unlock, 0);
    return dwSize;
}
#pragma GCC diagnostic pop

// ============================================================================
// 功能： 擦除
// 参数： BlkNo -- 块号;
// 返回：成功（TRUE）；失败（FALSE）；
// 备注：
// ============================================================================
bool_t efs_embflash_erase(u32 dwBlock)
{
    u32 addr = dwBlock * nordescription->PagesPerSector * nordescription->BytesPerPage;

    flash_protection_op(0,FLASH_PROTECT_NONE);
    djy_flash_req(lock, CN_TIMEOUT_FOREVER);
    djy_flash_erase(addr);
    djy_flash_req(unlock, 0);
    flash_protection_op(0,FLASH_PROTECT_ALL);
    return (TRUE);
}
// ============================================================================
// 功能： 检查指定地址是否准备好
// 参数：dwBlock -- 块号；dwOffset -- 块偏移； pBuf -- 需要判断的数据； dwSize -- 数据长度
// 返回：成功（TRUE）；失败（FALSE）；
// 备注：
// ============================================================================
bool_t efs_embflash_CheckBlockReady(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize)
{
    u32 i, addr = (dwBlock * nordescription->PagesPerSector * nordescription->BytesPerPage) + dwOffset;
    u8 *buf = NULL;
    djy_flash_req(lock, CN_TIMEOUT_FOREVER);
    if(dwSize > 0)
    {
        buf = malloc(dwSize);
        if(buf)
        {
            djy_flash_read_crc(addr, buf, dwSize);
            for(i=0; i < dwSize; i++)
            {
                if((0xFF != buf[i]) && (buf[i] != pBuf[i]))
                {   //如果指定地址里面的数据既不是全FF又不是待判断的数据，则该区域没有准备好
                    free(buf);
                    djy_flash_req(unlock, 0);
                    return (FALSE);
                }
            }
            free(buf);
        }
    }
    djy_flash_req(unlock, 0);
    return (TRUE);
}
// =============================================================================
// 功能：在at45中安装efs文件系统
// 参数：
//      TargetFs -- 要挂载的文件系统
//      分区数据 -- 起始块，结束块数（擦除时不包括该块，只擦到该块的上一块），是否格式化；
// 返回：成功（true）；失败（false）；
// 备注：
// =============================================================================
bool_t ModuleInstall_EmbFlashInstallEfs(const char *TargetFs, s32 bstart, s32 bend, u32 doformat)
{
    struct FsCore *super;
    char *notfind;
    struct Object *targetobj;
    if(flash_is_install() == true)
    {
        if((TargetFs != NULL) && (bstart != bend))
        {
            if(doformat)
            {
                struct uesz sz;
                sz.unit = 0;
                sz.block = 1;
                warning_printf("efs"," Format flash.\r\n");
                flash_protection_op(0,FLASH_PROTECT_NONE);
                if(-1 == djy_flash_req(format, bstart , bend, &sz))
                {
                    warning_printf("efs"," Format failure.");
                }
                flash_protection_op(0,FLASH_PROTECT_ALL);
            }
            targetobj = OBJ_MatchPath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("efs"," not found need to install file system.");
                return false;
            }
            super = (struct FsCore *)OBJ_GetPrivate(targetobj);
            if(strcmp(super->pFsType->pType, "EFS") == 0)
            {
                flash_protection_op(0,FLASH_PROTECT_NONE);
                if(EmbFsInstallInit(TargetFs,bstart,bend,&EFS_EMBFLASH_DRV) == 0)
                {
                    flash_protection_op(0,FLASH_PROTECT_ALL);
                    return true;
                }
                flash_protection_op(0,FLASH_PROTECT_ALL);
            }
            error_printf("efs"," need to install file system not EFS.");
        }
    }

    return false;
}
