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
#include "at45db321.h"
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <djyfs/efs.h>

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_Gd25InstallEfs(const char *TargetFs, s32 bstart, s32 bend, u32 doformat);
//    ModuleInstall_Gd25InstallEfs(CFG_GB25_EFS_MOUNT_NAME,
//                  CFG_GB25_EFS_PART_START, CFG_GB25_EFS_PART_END, CFG_GB25_EFS_PART_FORMAT);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"gb25q16c install efs"    //在norflash上安装efs文件系统
//parent:"nor flash gd25q16c"                //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"easyfilesystem","lock","spibus","cpu_peri_spi","nor flash gd25q16c"
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
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#ifndef CFG_GB25_EFS_MOUNT_NAME           //****检查参数是否已经配置好
#warning   gb25q16c install efs组件参数未配置，使用默认值
//%$#@enum,512,528,
//%$#@string,1,10,
#define CFG_GB25_EFS_MOUNT_NAME            "efs"      //"文件系统mount点名字",需要挂载的efs文件系统mount点名字
//%$#@num,-1,1024,
#define CFG_GB25_EFS_PART_START                  0        //分区起始，填写块号，块号从0开始计算
#define CFG_GB25_EFS_PART_END                   -1        //分区结束，-1表示最后一块
//%$#@enum,true,false,
#define CFG_GB25_EFS_PART_FORMAT               false      //分区选项,是否需要格式化该分区。
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

extern bool_t Gd25_is_install(void);
extern s32 __gd25q16c_write(u32 unit, void *data, struct uopt opt);
extern s32 __gd25q16c_read(u32 unit, void *data, struct uopt opt);
extern s32 __gd25q16c_erase(u32 unit, struct uesz sz);
extern s32 __gd25q16c_req(enum ucmd cmd, ptu32_t args, ...);
extern s32 __GD25_FsInstallInit(const char *fs, s32 dwStart, s32 dwEnd, void *mediadrv);

u32 efs_gd25_write(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags);
u32 efs_gd25_read(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags);
bool_t efs_gd25_erase(u32 dwBlock);
bool_t efs_gd25_CheckBlockReady(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize);

extern struct NorDescr *qflashdescription;

struct __efs_media_drv EFS_GD25_DRV =
{
    .efs_erase_media = efs_gd25_erase,
    .efs_read_media = efs_gd25_read,
    .efs_write_media = efs_gd25_write,
    .efs_check_block_ready = efs_gd25_CheckBlockReady
};

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
u32 efs_gd25_write(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags)
{
    u32 page = (dwBlock * qflashdescription->PagesPerSector) + (dwOffset / qflashdescription->BytesPerPage); // 页号
    u32 offset = dwOffset & 0xFF, wrLen = dwSize; // 页内的偏置
    s32 res , nowlen;
    u8 data[qflashdescription->BytesPerPage];
    struct uopt opt = {0};
    if(!pBuf)
        return (0);

    memset(data, 0xff, qflashdescription->BytesPerPage);
    if(dwSize < qflashdescription->BytesPerPage - offset)
        nowlen = dwSize;            //需要写的数据没有超过当前页，把数据全写入
    else
        nowlen = qflashdescription->BytesPerPage - offset; //需要写的数据超过当前页，先把当前页写满

    memcpy(data + offset, pBuf, nowlen);
    for(;;)
    {
        res = __gd25q16c_write(page, data, opt);
        if(res < 0)
            break; // 错误
        wrLen -= nowlen;    //把需要写的数据长度减去上次写的数据长度
        if(wrLen <= 0)
        {
            wrLen = 0;
            break; // 写完
        }
        pBuf += nowlen; //待写数据的地址偏移

        memset(data, 0xff, qflashdescription->BytesPerPage);
        if(wrLen >= qflashdescription->BytesPerPage)
            nowlen = qflashdescription->BytesPerPage;      //需要写的数据超过当前页，先把页写满
        else
            nowlen = wrLen; //需要写的数据没有超过当前页，把数据全写入

        memcpy(data, pBuf, nowlen);
        page++;     //页号加1
    }
    return (dwSize - wrLen);
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
u32 efs_gd25_read(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags)
{
    s32 res , rdLen = (s32)dwSize;
    u32 page = (dwBlock * qflashdescription->PagesPerSector) + (dwOffset / qflashdescription->BytesPerPage); // 页号
    u32 offset = dwOffset & 0xFF; // 页内的偏置
    u8 data[qflashdescription->BytesPerPage];
    struct uopt opt = {0};
    if(!pBuf)
        return (0);

    if(__gd25q16c_read(page, data, opt) != 0)       //读出当前页的所有数据
        return (0);     //读失败

    if(rdLen < (s32)(qflashdescription->BytesPerPage - offset))
        memcpy(pBuf, data + offset, rdLen);        //需要读的数据全在当前页中
    else
        memcpy(pBuf, data + offset, qflashdescription->BytesPerPage - offset);  //需要读的数据只有一部分在当前页中，先读这一部分

    rdLen -= qflashdescription->BytesPerPage - offset;
    if(rdLen <= 0)
        return (dwSize);  //数据全读完

    pBuf += qflashdescription->BytesPerPage - offset;      //读缓存地址偏移
    if(offset)
        offset = 0;

    page++;
    for(;;)
    {
        res = __gd25q16c_read(page, data, opt);
        if(res < 0)
            break; // 错误

        if(rdLen >= (s32)(qflashdescription->BytesPerPage))
            memcpy(pBuf, data, qflashdescription->BytesPerPage);       //需要读的数据大于1页大小，则先读取1页的数据量
        else
        {
            memcpy(pBuf, data, rdLen);      //需要读的数据全在当前页中
            rdLen = 0;
            break;
        }
        rdLen -= qflashdescription->BytesPerPage;
        pBuf += qflashdescription->BytesPerPage;
        page++;
    }

    return (dwSize - (u32)rdLen);
}

// ============================================================================
// 功能： 擦除
// 参数： BlkNo -- 块号;
// 返回：成功（TRUE）；失败（FALSE）；
// 备注：
// ============================================================================
bool_t efs_gd25_erase(u32 dwBlock)
{
    struct uesz sz;

    sz.unit = 0;
    sz.block = 1;
    if(!__gd25q16c_erase(dwBlock,sz))
        return (TRUE);

    return (FALSE);
}
// ============================================================================
// 功能： 检查指定地址是否准备好
// 参数：dwBlock -- 块号；dwOffset -- 块偏移； pBuf -- 需要判断的数据； dwSize -- 数据长度
// 返回：成功（TRUE）；失败（FALSE）；
// 备注：
// ============================================================================
bool_t efs_gd25_CheckBlockReady(u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize)
{
    static u8 data[259];
    u8 *buf = pBuf;
    u32 block = dwBlock,left = dwSize,i, rLen,offset = dwOffset,blockBytes = qflashdescription->PagesPerSector * qflashdescription->BytesPerPage;

    block += offset / blockBytes;
    offset = offset % blockBytes;

    do{
        rLen = left;
        if(rLen > 256)
            rLen = 256;

        if(0 == efs_gd25_read(block, offset, data, rLen,0))
            return (FALSE);

        for(i=0; i < rLen; i++)
        {
            if((0xFF != data[i]) && (data[i] != buf[i]))        //如果指定地址里面的数据既不是全FF又不是待判断的数据，则该区域没有准备好
                return (FALSE);
        }

        left -= rLen;
        offset += rLen;
    }while(left);

    return (TRUE);
}
// =============================================================================
// 功能：在gd25中安装efs文件系统
// 参数：   pBusName -- AT45所要用的通信线
//      TargetFs -- 要挂载的文件系统
//      分区数据 -- 起始块，结束块数（擦除时不包括该块，只擦到该块的上一块），是否格式化；
// 返回：成功（true）；失败（false）；
// 备注：
// =============================================================================
bool_t ModuleInstall_Gd25InstallEfs(const char *TargetFs, s32 bstart, s32 bend, u32 doformat)
{
    struct FsCore *super;
    char *notfind;
    struct Object *targetobj;
    if(Gd25_is_install() == true)
    {
        if((TargetFs != NULL) && (bstart != bend))
        {
            if(doformat)
            {
                struct uesz sz;
                sz.unit = 0;
                sz.block = 1;
                if(-1 == __gd25q16c_req(format, bstart , bend, &sz))
                {
                    warning_printf("efs"," Format failure.");
                }
            }
            targetobj = OBJ_MatchPath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("at45"," not found need to install file system.");
                return false;
            }
            super = (struct FsCore *)OBJ_GetPrivate(targetobj);
            if(strcmp(super->pFsType->pType, "EFS") == 0)
            {
                if(__GD25_FsInstallInit(TargetFs,bstart,bend,&EFS_GD25_DRV) == 0)
                    return true;
            }
            error_printf("at45"," need to install file system not EFS.");
        }
    }

    return false;
}
