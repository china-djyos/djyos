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
#include <device/flash/flash.h>
#include <device/include/unit_media.h>
#include <device.h>
#include <spibus.h>
#include <systime.h>
#include <math.h>
#include "at45db321.h"
#include <dbug.h>
#include <filesystems.h>
#include <efs_full.h>

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_At45InstallEfsFull(char *pBusName, const char *TargetFs, s32 bstart, s32 bend, u32 doformat);
//    ModuleInstall_At45InstallEfsFull(CFG_AT45_BUSNAME, CFG_AT45_EFSFULL_MOUNT_NAME,
//                  CFG_AT45_EFSFULL_PART_START, CFG_AT45_EFSFULL_PART_END, CFG_AT45_EFSFULL_PART_FORMAT);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"at45db321_install_efsfull"    //在norflash上安装efs文件系统
//parent:"at45db321"                //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"easyfullfilesystem","lock","spibus","cpu_peri_spi"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#ifndef CFG_AT45_EFSFULL_MOUNT_NAME           //****检查参数是否已经配置好
#warning   at45db321_install_efsfull组件参数未配置，使用默认值
//%$#@enum,512,528,
//%$#@string,1,10,
#define CFG_AT45_BUSNAME                       "SPI4"     //"SPI总线名称",AT45使用的总线名称
#define CFG_AT45_EFSFULL_MOUNT_NAME            "efsfull"      //"文件系统mount点名字",需要挂载的efs文件系统mount点名字
//%$#@num,-1,1024,
#define CFG_AT45_EFSFULL_PART_START                  0        //分区起始，填写块号，块号从0开始计算
#define CFG_AT45_EFSFULL_PART_END                   -1        //分区结束，-1表示最后一块
//%$#@enum,true,false,
#define CFG_AT45_EFSFULL_PART_FORMAT               false      //分区选项,是否需要格式化该分区。
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

extern s32 __at45_write(u32 unit, void *data, struct uopt opt);
extern s32 __at45_read(u32 unit, void *data, struct uopt opt);
extern s32 __at45_erase(u32 unit, struct uesz sz);
extern s32 __at45_req(enum ucmd cmd, ptu32_t args, ...);
extern s32 __AT45_FsInstallInit(const char *fs, s32 dwStart, s32 dwEnd,void *mediadrv);
s32 efs_at45_erase(struct __ecore *core, s64 units, s64 size);
s32 efs_at45_write(struct __ecore *core, s64 units, void *buf);
s32 efs_at45_read(struct __ecore *core, s64 units, void *buf);

struct __efs_full_media_drv EFS_FULL_AT45_DRV =
{
    .efs_erase_media = efs_at45_erase,
    .efs_read_media = efs_at45_read,
    .efs_write_media = efs_at45_write
};

// ============================================================================
// 功能：读；
// 参数：core -- 文件系统控制；
//      units -- 起始unit号；
//      buf -- 数据；
// 返回：成功（0）；失败（-1）；
// 备注：按unit空间大小读；
// ============================================================================
s32 efs_at45_read(struct __ecore *core, s64 units, void *buf)
{
    struct uopt opt = {0};

    opt.necc = 1;
    opt.main = 1;
    units = core->MStart + units;
    return (__at45_read((u32)units, (u8*)buf, opt));
}

// ============================================================================
// 功能：写；
// 参数：core -- 文件系统控制；
//      units -- 起始unit号；
//      buf -- 数据；
// 返回：成功（0）；失败（-1）；
// 备注：按unit空间大小写；
// ============================================================================
s32 efs_at45_write(struct __ecore *core, s64 units, void *buf)
{
    struct uopt opt = {0};

    opt.necc = 1;
    opt.main = 1;
    units = core->MStart + units;
    return (__at45_write((u32)units, (u8*)buf, opt));

}

// ============================================================================
// 功能：擦除；
// 参数：core -- 文件系统控制；
//      units -- 起始unit号擦除，需按可擦尺寸对齐；
//      size -- 需擦除的unit数量，需按可擦尺寸对齐；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 efs_at45_erase(struct __ecore *core, s64 units, s64 size)
{
    s32 res = -1;
    s64 erases;
    struct uesz sz;

    sz.unit = 1;
    sz.block = 0;
    erases = 1 << (core->blacksize - core->pagesize); // 可擦除空间所含有的unit数量
    if((size % erases) || (units % erases))
    {
        printf("\r\n: warn : efs    : not aligned area will be erased(%d, %d)", (u32)units, (u32)size); // 擦除的数据并没有对齐，可能存在问题
    }

    while(size > 0)
    {
        res = __at45_erase((u32)units, sz);
        if(-1 == res)
            return (-1);

        units += erases;
        size -= erases;
    }

    return (0);
}

// =============================================================================
// 功能：在at45中安装efs_full文件系统
// 参数：   pBusName -- AT45所要用的通信线
//      TargetFs -- 要挂载的文件系统
//      分区数据 -- 起始块，结束块数（擦除时不包括该块，只擦到该块的上一块），是否格式化；
// 返回：成功（true）；失败（false）；
// 备注：
// =============================================================================
bool_t ModuleInstall_At45InstallEfsFull(char *pBusName, const char *TargetFs, s32 bstart, s32 bend, u32 doformat)
{
    struct FsCore *super;
    char *notfind;
    struct obj *targetobj;
    if(ModuleInstall_at45db321(pBusName,0) == true)
    {
        if((TargetFs != NULL) && (bstart != bend))
        {
            if(doformat)
            {
                struct uesz sz;
                sz.unit = 0;
                sz.block = 1;
                if(-1 == __at45_req(format, bstart , bend, &sz))
                {
                    warning_printf("efsfull"," Format failure.");
                }
            }
            targetobj = obj_matchpath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("at45"," not found need to install file system.");
                return false;
            }
            super = (struct FsCore *)obj_GetPrivate(targetobj);
            if(strcmp(super->pFsType->pType, "EFSFULL") == 0)
            {
                if(__AT45_FsInstallInit(TargetFs,bstart,bend,&EFS_FULL_AT45_DRV) == 0)
                    return true;
            }
            error_printf("at45"," need to install file system not EFSFULL.");
        }
    }

    return false;
}
