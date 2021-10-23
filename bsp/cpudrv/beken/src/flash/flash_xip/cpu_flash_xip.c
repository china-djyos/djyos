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
#include <Iboot_info.h>
#include <djyfs/filesystems.h>
#include <device/djy_flash.h> // will be obsolete
#include <device/unit_media.h>
#include <xip.h>
#include <endian.h>
#include <objhandle.h>
#include "cpu_peri.h"

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_FlashInstallXIP(const char *TargetFs,s32 bstart, s32 bend, u32 doformat);
//    ModuleInstall_FlashInstallXIP(CFG_EFLASH_XIPFSMOUNT_NAME,CFG_EFLASH_XIP_PART_START,
//                                              CFG_EFLASH_XIP_PART_END, CFG_EFLASH_XIP_PART_FORMAT);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"emflash insatall xip"//片内flash安装xip
//parent:"cpu onchip flash"//填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                         //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                      //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                        //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                       //初始化时机，可选值：early，medium，later, pre-main。
                                        //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","lock","cpu onchip flash"//该组件的依赖组件名（可以是none，表示无依赖组件），
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
//%$#@num,-1,1024,
#define CFG_EFLASH_XIP_PART_START      128          //"分区起始块号，含"，填写块号，块号从0开始计算，CPU视角，物理块号*32/34
#define CFG_EFLASH_XIP_PART_END        -1         //"分区结束块号，不含"，-1表示最后一块，CPU视角，物理块号*32/34
//%$#@enum,true,false,
#define CFG_EFLASH_XIP_PART_FORMAT     false      //"分区选项",是否需要格式化该分区。
//%$#@enum,"xip-app","xip-iboot",NULL
#define CFG_EFLASH_XIPFSMOUNT_NAME   "xip-app"    //"文件系统的mount位置"
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure
// ============================================================================
extern struct NorDescr *nordescription;
u8 is_protect = 1;   //1 -- 有写保护，0 -- 无写保护
//extern bool_t flash_is_install(void);
extern u32 gc_ProductSn;
extern void calc_crc(u32 *buf, u32 packet_num);

//extern void djy_flash_read(uint32_t address, void *data, uint32_t size);
//extern void djy_flash_write(uint32_t address, const void *data, uint32_t size);
//extern void djy_flash_erase(uint32_t address);
//extern s32 djy_flash_req(enum ucmd cmd, ptu32_t args, ...);
extern s32 EmbFsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
//extern void djy_flash_read_crc(uint32_t address, void *data, uint32_t size);
extern void flash_protection_op(UINT8 mode, PROTECT_TYPE type);
s32 xip_flash_write(struct __icore *core, u8 *data, u32 bytes, u32 pos);
s32 xip_flash_read(struct __icore *core, u8 *data, u32 bytes, u32 pos);
s32 xip_flash_erase(struct __icore *core, u32 bytes, u32 pos);

struct __xip_drv XIP_FLASH_DRV =
{
    .xip_erase_media = xip_flash_erase,
    .xip_read_media = xip_flash_read,
    .xip_write_media = xip_flash_write
};
// ============================================================================
// 功能：写数据，无论数据是否已经添加crc校验码。
// 参数：core -- xip文件系统管理信息
//      data -- 数据缓冲
//      bytes -- 写字节数，不会超过一个缓存大小；
//      pos -- 数据地址，无论data是否包含crc，都是逻辑地址。
// 返回：成功（0）；失败（-1）；将要没有可写空间（-2）；
// 备注：当写到最后一个unit时，会尝试擦除
// ============================================================================
s32 xip_flash_write(struct __icore *core, u8 *data, u32 bytes, u32 pos)
{
    struct umedia *um = (struct umedia *)core->vol;
    static u8 *app_head = NULL;
    static ptu32_t iboot_sn_addr = NULL;
    struct objhandle *hdl = (struct objhandle *)core->root->child->handles.next;
    struct __icontext *cx = (struct __icontext *)hdl->context;
    struct __ifile *file = (struct __ifile*)handle_GetHostObjectPrivate(hdl);
    u32 j, page_size, offset = Iboot_GetAppHeadSize();
    u32 unit;
    u32 check_len = bytes;
    struct ProductInfo *p_productinfo;
    u8 iboot_sn_buf[32];

    if(GetOperFalshMode() == true)
    {
        //data包含了crc，只需要把起始地址 * 34 / 32，后面的数据直接写就行了，因为已经有crc了
        unit = pos + (core->MStart * nordescription->BytesPerPage) * 34 / 32;
    }
    else
    {
        //没有算好crc的数据则需要先算出数据地址，再整体 * 34 / 32，因为数据里没有crc，需要这里整体算好
        unit = (pos + (core->MStart * nordescription->BytesPerPage)) * 34 / 32;
    }
    djy_flash_req(lock, CN_TIMEOUT_FOREVER);
    if(strstr(CN_XIP_APP_PATH,core->root->name))
    {
        if(bytes > 0)
        {
            while(check_len > 0)    //此循环检查flash是否已经擦除
            {
                if(check_len > nordescription->BytesPerPage)
                    page_size = nordescription->BytesPerPage;
                else
                    page_size = check_len;
                if(GetOperFalshMode() == true)
                    djy_flash_read_crc(unit, um->ubuf, page_size);
                else
                    djy_flash_read(unit, um->ubuf, page_size);

                for(j=0; j<page_size; j++)
                {
                    if(0xFF!=um->ubuf[j])        //判断当前页是否为FF
                    {
                        if(um->ubuf[j]!=data[j])
                        {
                            djy_flash_req(unlock, 0);
                            error_printf("EmFlash"," Data already exists in flash.\r\n");
                            return (-1);
                        }
                    }
                }
                check_len -= page_size;
            }

            if(pos == offset)
            {
                if(GetOperFalshMode() == true)      //无须考虑crc，即文件已经算好crc
                {
                    offset = (offset * 34 / 32) - offset;   //存在crc的bin文件，app的文件头要在这里先保留下来
                    app_head = malloc(offset + Iboot_GetAppHeadSize());
                    if(app_head == NULL)
                    {
                        djy_flash_req(unlock, 0);
                        return (-1);
                    }
                    memcpy(app_head, cx->apphead, cx->Wappsize);
                    memcpy(app_head + cx->Wappsize, data, offset);
                    //判断App中是否有提供SN号，编译时生产周数和序号都填的是0x2a(*号)
                    if(*(app_head + ((sizeof(struct AppHead) + (u32)offsetof(struct ProductInfo, ProductionTime)) / 32 * 34)) == '*')
                    {
                        iboot_sn_addr = (u32)(&gc_ProductSn) / 32 * 34;
                        if(iboot_sn_addr)
                        {
                            memset(iboot_sn_buf, 0xff, sizeof(iboot_sn_buf));
                            djy_flash_read_crc(iboot_sn_addr, iboot_sn_buf, sizeof(iboot_sn_buf));  //app未提供SN号，从iboot中获取。
                            if(iboot_sn_buf[0] != 0xff)
                            {   //把iboot的SN复制到app的中
                                memcpy(app_head + ((sizeof(struct AppHead) + (u32)offsetof(struct ProductInfo, ProductionTime)) / 32 * 34),
                                                    iboot_sn_buf + sizeof(p_productinfo->TypeCode), sizeof(p_productinfo->ProductionTime) + sizeof(p_productinfo->ProductionNumber));
                            }
                        }
                    }
                    pos += offset;
                    data += offset;
                    bytes -= offset;
                    if(GetOperFalshMode() == true)
                    {
                        unit = pos + (core->MStart * nordescription->BytesPerPage) * 34 / 32;
                    }
                    else
                    {
                        unit = pos + (core->MStart * nordescription->BytesPerPage);
                        unit = unit * 34 / 32;
                    }
                }
                else
                {
                    if(*(cx->apphead + (sizeof(struct AppHead) + (u32)offsetof(struct ProductInfo, ProductionTime))) == 0xff)
                    {
                        iboot_sn_addr = (u32)(&gc_ProductSn) / 32 * 34;
                        if(iboot_sn_addr)
                        {
                            memset(iboot_sn_buf, 0xff, sizeof(iboot_sn_buf));
                            djy_flash_read_crc(iboot_sn_addr, iboot_sn_buf, sizeof(iboot_sn_buf));
                            if(iboot_sn_buf[0] != 0xff)
                            {   //把iboot的SN复制到app的中
                                memcpy(cx->apphead + (sizeof(struct AppHead) + (u32)offsetof(struct ProductInfo, ProductionTime)),
                                            iboot_sn_buf + sizeof(p_productinfo->TypeCode),
                                    sizeof(p_productinfo->ProductionTime) + sizeof(p_productinfo->ProductionNumber));
                            }
                        }
                    }
                }
                if(is_protect == 1)
                {
                    is_protect = 0;
                    flash_protection_op(0,FLASH_PROTECT_NONE);
                }
            }
            if(GetOperFalshMode() == true)
            {
                if(pos == 0)
                {
                    offset = (offset * 34 / 32) - offset;       //保留下来的文件头，在这里重新填充数据，并计算crc
                    if(app_head == NULL)
                    {
                        djy_flash_req(unlock, 0);
                        return (-1);
                    }
                    u8 *name = (u8 *)core->root->child->name;
                    u8 flag = 1;
                    u32 app_head_size = offset + Iboot_GetAppHeadSize();

    //                    file->sz += file->cxbase;
                    fill_little_32bit(app_head + 4, 0, file->sz);
                    app_head += 32; //32为文件头信息的前32个字节
                    for(j = 0; j < 96 + 8; j++)     //这个96是在文件头里存app文件名数组的大小,加8是多出来的8个字节的CRC
                    {
                        if((j % 34) == 0)
                            j += 2;
                        if(j < 96 + 8)
                        {
                            if(flag)
                                app_head[j] = *name;
                            else
                                app_head[j] = 0xff;
                            if(*name++ == 0)
                                flag=0;
                        }
                    }
                    app_head -= 32;
                    j = 0;
                    while(j < app_head_size)
                    {
                        j += 32;
                        memset(app_head + j, 0xff, 2);
                        j += 2;
                    }
                    calc_crc((u32 *)app_head, Iboot_GetAppHeadSize() / 32);
                    djy_flash_write(unit, app_head, app_head_size);
                    free(app_head);
                }
                else
                {
                    djy_flash_write(unit, data, bytes);
                }
            }
            else
                djy_flash_write(unit, data, bytes);

            if(pos == 0)
            {
                if(is_protect == 0)
                {
                    is_protect = 1;
                    flash_protection_op(0,FLASH_PROTECT_ALL);
                }
            }
        }

//        // 如果当前写入页是一个块中的最后一页，则预先删除后续的sector
//        // (page+1)用于防止格式化了不属于xip的空间
//        if(GetOperFalshMode() == true)
//        {
//            djy_flash_req(whichblock, (ptu32_t)&block, &unit);
//            djy_flash_req(remain, (ptu32_t)&more, &unit);
//            if(!more)
//            {
//                // +1是表示当前unit的后面一个
//                if(pos + bytes >= core->ASize)
//                    return (-2);
//                djy_flash_req(whichblock, (ptu32_t)&block, &unit);
//                //block是当前页所在的块号，block+1是为了擦除下一个块（block+1是要擦除的块，擦到block+1+1块就不擦了）
//                djy_flash_req(format, block+1, block+1+1);
//            }
//        }
    }
    else
    {
        if(strstr(CN_XIP_IBOOT_PATH,core->root->name))
        {
            djy_flash_write(unit, data, bytes);
        }
    }
    djy_flash_req(unlock, 0); //
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
s32 xip_flash_read(struct __icore *core, u8 *data, u32 bytes, u32 pos)
{
    s64 unit;
    s32 left = bytes;
    unit = pos + (core->MStart * nordescription->BytesPerPage) * 34 / 32;
    djy_flash_req(lock, CN_TIMEOUT_FOREVER);
    if(left > 0)
    {

            djy_flash_read(unit, data, left);
    }

    djy_flash_req(unlock, 0); //
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
s32 xip_flash_erase(struct __icore *core, u32 bytes, u32 pos)
{
    s64 unit;
    u32 page_size, page_num, all;
    s32 left = bytes;

    if(is_protect == 1)
    {
        is_protect = 0;
        flash_protection_op(0,FLASH_PROTECT_NONE);
    }

    unit = pos + ((core->MStart * nordescription->BytesPerPage) * 34 / 32);

    page_size = nordescription->BytesPerPage;
    page_num = nordescription->PagesPerSector;
    all = page_size * page_num;
    djy_flash_req(lock, CN_TIMEOUT_FOREVER);
    while(left > 0)
    {
        djy_flash_erase(unit);
        unit += all;
        left -= all;
    }
    djy_flash_req(unlock, 0);

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
    s32 left = xip_core->ASize, start, all, page_size, page_num;
    start = (xip_core->MStart * nordescription->BytesPerPage) * 34 / 32;
    page_size = nordescription->BytesPerPage;
    page_num = nordescription->PagesPerSector;
    all = page_size * page_num;
    if(is_protect == 1)
    {
        is_protect = 0;
        flash_protection_op(0,FLASH_PROTECT_NONE);
    }
    while(left > 0)
    {
        djy_flash_erase(start);
        start += all;
        left -= all;
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
bool_t ModuleInstall_FlashInstallXIP(const char *TargetFs,s32 bstart, s32 bend, u32 doformat)
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
                warning_printf("xip"," Format flash.\r\n");
                flash_protection_op(0,FLASH_PROTECT_NONE);
                if(-1 == djy_flash_req(format, bstart , bend, &sz))
                {
                    warning_printf("xip"," Format failure.\r\n");
                }
                flash_protection_op(0,FLASH_PROTECT_ALL);
            }
            targetobj = OBJ_MatchPath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("EmFlash"," not found need to install file system.\r\n");
                return false;
            }
            super = (struct FsCore *)OBJ_GetPrivate(targetobj);
            if((strcmp(super->pFsType->pType, "XIP-APP") == 0) || (strcmp(super->pFsType->pType, "XIP-IBOOT") == 0))
            {
                if(EmbFsInstallInit(TargetFs,bstart,bend,&XIP_FLASH_DRV) == 0)
                    return true;
            }
            error_printf("EmFlash"," need to install file system not XIP.");
        }
    }
    return false;
}
