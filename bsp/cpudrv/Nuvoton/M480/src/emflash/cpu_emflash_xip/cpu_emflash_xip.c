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
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <misc/misc.h>
#include <systime.h>
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <device/djy_flash.h> // will be obsolete
#include <device/unit_media.h>
#include <xip.h>

#if 0
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern s32 ModuleInstall_EmFlashInstallXIP(const char *TargetFs,s32 bstart, s32 bend, u32 doformat);
//    ModuleInstall_EmFlashInstallXIP(CFG_EFLASH_XIPFSMOUNT_NAME,CFG_EFLASH_XIP_PART_START,
//                                              CFG_EFLASH_XIP_PART_END, CFG_EFLASH_XIP_PART_FORMAT);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"emflash insatall xip"//片内flash安装xip
//parent:"cpu drive inner flash"//填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                         //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                      //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                        //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                       //初始化时机，可选值：early，medium，later, pre-main。
                                        //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","lock","cpu drive inner flash"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                        //选中该组件时，被依赖组件将强制选中，
                                        //如果依赖多个组件，则依次列出
//weakdependence:"xip app file system","xip iboot file system"  //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                        //选中该组件时，被依赖组件不会被强制选中，
                                        //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                        //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_EMFLASH_INSATALL_XIP == false )
//#warning  " emflash_insatall_xip  组件参数未配置，使用默认配置"
//%$#@target = header   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_EMFLASH_INSATALL_XIP    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,-1,12,
#define CFG_EFLASH_XIP_PART_START      6          //分区起始，填写块号，块号从0开始计算
#define CFG_EFLASH_XIP_PART_END        -1         //分区结束，-1表示最后一块，如果结束块是6，起始块是0，则该分区使用的块为0，1，2，3，4，5块
//%$#@enum,true,false,
#define CFG_EFLASH_XIP_PART_FORMAT     false      //分区选项,是否需要格式化该分区。
//%$#@enum,"xip-app","xip-iboot",NULL
#define CFG_EFLASH_XIPFSMOUNT_NAME   EN_XIP_APP_TARGET    //需安装的文件系统的mount的名字，NULL表示该flash不挂载文件系统
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure
// ============================================================================

extern bool_t emflash_is_install(void);
extern s32 __embed_read(s64 unit, void *data, struct uopt opt);
extern s32 __embed_req(enum ucmd cmd, ptu32_t args, ...);
extern s32 __embed_write(s64 unit, void *data, struct uopt opt);
extern s32 __embed_erase(s64 unit, struct uesz sz);
extern s32 __embed_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
s32 xip_emflash_write(struct __icore *core, u8 *data, u32 bytes, u32 pos);
s32 xip_emflash_read(struct __icore *core, u8 *data, u32 bytes, u32 pos);
s32 xip_emflash_erase(struct __icore *core, u32 bytes, u32 pos);


struct __xip_drv XIP_EMFLASH_DRV =
{
    .xip_erase_media = xip_emflash_erase,
    .xip_read_media = xip_emflash_read,
    .xip_write_media = xip_emflash_write
};
// ============================================================================
// 功能：写数据
// 参数：core -- xip文件系统管理信息
//      data -- 数据缓冲
//      bytes -- 写字节数，不会超过一个缓存大小；
//      pos -- 数据地址
// 返回：成功（0）；失败（-1）；将要没有可写空间（-2）；
// 备注：当写到最后一个unit时，会尝试擦除
// ============================================================================
s32 xip_emflash_write(struct __icore *core, u8 *data, u32 bytes, u32 pos)
{
    struct umedia *um = (struct umedia *)core->vol;
    struct uesz esz = {0};
    struct uopt opt = {0};
    u32 j, offset, once, more;
    s32 left;
    s64 unit;
    u32 block = 0;

    left = bytes;
    unit = (pos / core->bufsz) + core->MStart;
    offset = pos & (core->bufsz-1); // unit内偏移
    __embed_req(lock, CN_TIMEOUT_FOREVER);
    while(left>0)
    {
#if 0
        // 如果当前写入页是一个块中的最后一页，则预先删除后续的sector
        // (page+1)用于防止格式化了不属于xip的空间
        __embed_req(remain, (ptu32_t)&more, &unit);
        if(!more)
        {
            // +1是表示当前unit的后面一个
            if(((unit-um->ustart+1)<<um->usz)<um->asz)
            {
                struct uesz sz = {0};
                sz.unit = 1;
                __embed_erase(unit+1, sz); // 不管有没有擦除成功，因为如果后续写入的话，会有回读校验
            }
        }
#endif
        if(__embed_read(unit, um->ubuf, opt))
        {
            __embed_req(unlock, 0); //
            return (-1);
        }

        once = core->bufsz;
        if(left<(s32)once)
            once = left;

        for(j=0; j<(once); j++)
        {
            if(um->ubuf[j+offset]!=data[j])
            {
                if(0xFF!=um->ubuf[j+offset])        //判断当前页是否为FF
                {
                    __embed_req(unlock, 0);
                    return (-1);
                }
            }
        }

        memcpy((um->ubuf + offset), data, once);
        if(__embed_write(unit, um->ubuf, opt))
        {
            __embed_req(unlock, 0);
            return (-1);
        }

        left -= once;
        offset = 0;
        data += once;
        if(left)
            unit++;
    }

    // 如果当前写入页是一个块中的最后一页，则预先删除后续的sector
    // (page+1)用于防止格式化了不属于xip的空间
    __embed_req(remain, (ptu32_t)&more, &unit);
    if(!more)
    {
        // +1是表示当前unit的后面一个
        if(((unit-core->MStart+1)* core->bufsz) >= core->ASize)
        {
            return (-2);
        }
        esz.block = 1;
//        esz->unit = 0;
        __embed_req(whichblock, (ptu32_t)&block, &unit);
        //block是当前页所在的块号，block+1是为了擦除下一个块（block+1是要擦除的块，擦到block+1+1块就不擦了）
        __embed_req(format, block+1, block+1+1, &esz);
    }

    __embed_req(unlock, 0); //
    return (0);
}

// ============================================================================
// 功能：读数据
// 参数：core -- xip文件系统管理信息
//      data -- 数据缓冲
//      bytes -- 读字节数
//      pos -- 数据地址
// 返回：成功（0）；失败（-1）；
// 备注：单次也就只会读一个unit，目前
// ============================================================================
s32 xip_emflash_read(struct __icore *core, u8 *data, u32 bytes, u32 pos)
{
    struct umedia *um = (struct umedia *)core->vol;
    struct uopt opt = {0};
    s64 unit;
    u32 offset;
    s32 left = bytes, once;

    unit = (pos / core->bufsz) + core->MStart;
    offset = pos & (core->bufsz - 1); // unit内偏移
    __embed_req(lock, CN_TIMEOUT_FOREVER);
    while(left>0)
    {
        once = MIN((core->bufsz - offset), left);
        if(__embed_read(unit, um->ubuf, opt))
        {
            __embed_req(unlock, 0); //
            return (-1);
        }

        memcpy(data, (um->ubuf + offset), once);
        left -= once;
        offset = 0;
        unit++;
        data += once;
    }

    __embed_req(unlock, 0); //
    return (0);
}

// ============================================================================
// 功能：擦除数据
// 参数：core -- xip文件系统管理信息
//       bytes -- 字节数
//       pos -- 数据地址
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 xip_emflash_erase(struct __icore *core, u32 bytes, u32 pos)
{
    struct uesz esz = {0};
    s64 unit;
    u32 erases, offset;
    s32 left = bytes;

    esz.unit = 1;
    unit = (pos / core->bufsz) + core->MStart;
    offset = pos & (core->bufsz-1); // unit内偏移
    __embed_req(lock, CN_TIMEOUT_FOREVER);
    while(left>0)
    {
        if(__embed_req(remain, (ptu32_t)&erases, (ptu32_t)&unit))       //获取当前页在所在块中的位置
        {
            printf("\r\n: erro : xipfs  : erase unit %lld failed, cannot get remain.", unit);
            return (-1);
        }

        if(__embed_erase(unit, esz))
        {
            printf("\r\n: erro : xipfs  : erase unit %lld failed, with erases %d.", unit, erases);
            return (-1);
        }

        erases++; // 擦除增一，表示包括当前的unit
        left -= ((erases * core->bufsz) - offset);
        unit += erases;     //加erases等于下一块的第一页
        offset = 0;
    }

    return (0);
}


// ============================================================================
// 功能：格式化xip文件系统
// 参数：core -- xip文件系统管理信息
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 xip_fs_format(void *core)
{
    struct __icore *xip_core = core;
    u32 remainunit = 0,block = 0,endunit = 0;
    s64 startunit = xip_core->MStart;
    struct uesz esz = {0};
    esz.block = 1;
    endunit = (xip_core->ASize/xip_core->bufsz) + startunit;
    __embed_req(whichblock, (ptu32_t)&block, (ptu32_t)&startunit);
    if(__embed_erase(block, esz))
    {
        printf("\r\n: erro : xipfs  : erase block %d failed.", block);
        return (-1);
    }
    while(1)
    {
        __embed_req(remain, (ptu32_t)&remainunit, (ptu32_t)&startunit);
        startunit = startunit + remainunit + 1;    //加1后会等于下一块的第一页
        if(startunit >= endunit)
        {
            break;
        }
        __embed_req(whichblock, (ptu32_t)&block, &startunit);
        if(__embed_erase(block, esz))
        {
            printf("\r\n: erro : xipfs  : erase block %d failed.", block);
            return (-1);
        }
    }
    return (0);
}

//-----------------------------------------------------------------------------
// 功能：安装片内Flash驱动
// 参数：TargetFs -- 要挂载的文件系统
//      分区数据 -- 起始块，结束块数（如果结束块是6，起始块是0，则该分区使用的块为0，1，2，3，4，5块，不包括第六块），是否格式化；
// 返回：成功（0）；失败（-1）；
// 备注：如果还不知道要安装什么文件系统，或者不安装文件系统TargetFs填NULL，TargetPart填-1；
//-----------------------------------------------------------------------------
bool_t ModuleInstall_EmFlashInstallXIP(const char *TargetFs,s32 bstart, s32 bend, u32 doformat)
{
    struct FsCore *super;
    char *notfind;
    struct Object *targetobj;
    if(emflash_is_install() == true)
    {
        if((TargetFs != NULL) && (bstart != bend))
        {
            if(doformat)
            {
                struct uesz sz;
                sz.unit = 0;
                sz.block = 1;
                if(-1 == __embed_req(format, bstart , bend, &sz))
                {
                    warning_printf("xip"," Format failure.");
                }
            }
            targetobj = OBJ_MatchPath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("EmFlash"," not found need to install file system.");
                return false;
            }
            super = (struct FsCore *)OBJ_GetPrivate(targetobj);
            if((strcmp(super->pFsType->pType, "XIP-APP") == 0) || (strcmp(super->pFsType->pType, "XIP-IBOOT") == 0))
            {
                if(__embed_FsInstallInit(TargetFs,bstart,bend,&XIP_EMFLASH_DRV) == 0)
                    return true;
            }
            error_printf("EmFlash"," need to install file system not XIP.");
        }
    }
    return false;
}
#endif
