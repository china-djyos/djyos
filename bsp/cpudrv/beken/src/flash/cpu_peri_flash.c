//----------------------------------------------------
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
// =============================================================================
#include "cpu_peri.h"
#include "lock.h"
#include "systime.h"
#include "stdlib.h"
#include <xip.h>
#include <dbug.h>
#include <filesystems.h>
#include <device/include/unit_media.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern int ModuleInstall_Flash(void);
//  ModuleInstall_Flash();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip flash"//CPU的rtc外设驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_FLASH == false )
//#warning  " cpu_onchip_flash  组件参数未配置，使用默认配置"
//%$#@target = header    //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_FLASH    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
//@#$%component end configure

struct MutexLCB *flash_mutex = NULL;
static bool_t sflashInited = false;
static struct umedia *flash_um;
const char *flash_name = "emflash";      //该flash在obj在的名字
extern struct Object *s_ptDeviceRoot;

static struct FlashDescr{
    u32     BytesPerPage;                // 页中包含的字节数
    u32     PagesPerSector;               //  sector的页数
    u32     AllSectorNum;               //  所有的sector数
    u32     MappedStAddr;
} *description;

// ============================================================================
// 功能：设置内置FLASH的信息
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 SetFlash_Init(struct FlashDescr *Description)
{
    Description->BytesPerPage = 256;
    Description->PagesPerSector = 16;
    Description->AllSectorNum = 1024;
    Description->MappedStAddr = 0x0;
    return (0);
}


void djy_flash_read(uint32_t address, void *data, uint32_t size)
{
    if (size == 0)
    {
        return;
    }
    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
    flash_protection_op(0,FLASH_PROTECT_NONE);
    flash_read(data, size, address);
    flash_protection_op(0,FLASH_PROTECT_ALL);
    Lock_MutexPost(flash_mutex);
}

void djy_flash_write(uint32_t address, const void *data, uint32_t size)
{
    if (size == 0)
    {
        return;
    }
    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
    flash_protection_op(0,FLASH_PROTECT_NONE);
    flash_write((char *)data, size, address);
    flash_protection_op(0,FLASH_PROTECT_ALL);
    Lock_MutexPost(flash_mutex);
}

/*一个sector是4K*/
void djy_flash_erase(uint32_t address)
{
    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
    address &= (0xFFF000);

    flash_protection_op(0,FLASH_PROTECT_NONE);
    flash_ctrl(CMD_FLASH_ERASE_SECTOR, &address);
    flash_protection_op(0,FLASH_PROTECT_ALL);
    Lock_MutexPost(flash_mutex);
}


// ============================================================================
// 功能：embeded flash 命令
// 参数：ucmd -- 命令；
//      其他 -- 命令参数；
// 返回：
// 备注：
// ============================================================================
s32 djy_flash_req(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case remain:
        {
            va_list list;
            u32 *left,  *unit;

            left = (u32*)args;
            va_start(list, args);
            unit = (u32*)va_arg(list, u32);
            va_end(list);
            //PagesPerSector减1是因为页号从0开始
            *left = (description->PagesPerSector - 1) - ((*unit / description->BytesPerPage) % description->PagesPerSector);
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

            *block = *unit / (description->BytesPerPage * description->PagesPerSector);

            break;
        }
        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = description->BytesPerPage;
            break;
        }

        case format:
        {
            va_list list;
            s32 start, end;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            va_end(list);


            if(-1== end)
                end = description->AllSectorNum;// 结束的号；

            do
            {
                djy_flash_erase((u32)((--end * ((u32)(description->BytesPerPage * description->PagesPerSector)))+description->MappedStAddr));
            }
            while(end!=start);

            break;
        }

        case mapaddr:
        {

            *((u32*)args) = description->MappedStAddr;
            break;
        }
        case totalblocks:
        {
            *((u32*)args) = description->PagesPerSector;
            break;
        }
        case lock:
        {
            u32 time_ou = args;
            Lock_MutexPend(flash_mutex, time_ou);
            break;
        }
        case unlock:
        {
            Lock_MutexPost(flash_mutex);
            break;
        }

        case checkbad: break;
        default: res = -1; break;
    }

    return (res);
}

// ============================================================================
// 功能：在内flash安装文件系统
// 参数：fs -- 需要挂载的文件系统，mediadrv -- 媒体驱动，
//       bstart -- 起始块，bend -- 结束块（不包括该块，只到该块的上一块）
// 返回：0 -- 成功， -1 -- 失败
// 备注：
// ============================================================================
s32 FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv)
{
    u32 units, total = 0;
     char *FullPath,*notfind;
     struct Object *targetobj;
     struct FsCore *super;
     s32 res,endblock = bend;

     if(mediadrv == NULL)
         return -1;
     targetobj = obj_matchpath(fs, &notfind);
     if(notfind)
     {
         error_printf("embed"," not found need to install file system.\r\n");
         return -1;
     }
     super = (struct FsCore *)obj_GetPrivate(targetobj);
     super->MediaInfo = flash_um;
     super->MediaDrv = mediadrv;

     if(-1 == (s32)endblock)
         endblock = bend = description->AllSectorNum; // 最大块号

     super->AreaSize = (bend - bstart) * description->BytesPerPage * description->PagesPerSector;
     super->MediaStart = (bstart * description->BytesPerPage * description->PagesPerSector) + description->MappedStAddr; // 起始unit号
     super->MediaStart = super->MediaStart * 34 / 32;
     if(super->AreaSize + super->MediaStart > description->AllSectorNum * description->BytesPerPage * description->PagesPerSector)
     {
         error_printf("embed","fileOS beyond the flash range.\r\n");
         return -1;
     }
     res = strlen(flash_name) + strlen(s_ptDeviceRoot->name) + 1;
     FullPath = malloc(res);
     memset(FullPath, 0, res);
     sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,flash_name);   //获取该设备的全路径
     FsBeMedia(FullPath,fs); //往该设备挂载文件系统
     free(FullPath);

     printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
     return (0);
}
// ============================================================================
// 功能：初始化片内flash
// 参数：无
// 返回：0 -- 成功， -1 -- 失败
// 备注：
// ============================================================================
int ModuleInstall_Flash(void)
{
    description = malloc(sizeof(*description));
    if(!description)
        return (-1);

    SetFlash_Init(description);

    flash_um = malloc(sizeof(struct umedia)+description->BytesPerPage);
    if(!flash_um)
        return (-1);

    flash_um->mreq = djy_flash_req;
    flash_um->type = embed;
    flash_um->ubuf = (u8*)flash_um + sizeof(struct umedia);

    if(!dev_Create((const char*)flash_name, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)flash_um)))
    {
        printf("\r\n: erro : device : %s addition failed.", flash_name);
        free(flash_um);
        return (-1);
    }

    flash_mutex = Lock_MutexCreate("flash_mutex");
    if(flash_mutex==NULL)
        return -1;
    else
    {
        sflashInited = true;
        return 0;
    }

}

// =============================================================================
// 功能：判断flash是否安装
// 参数：  无
// 返回：已成功安装（true）；未成功安装（false）；
// 备注：
// =============================================================================
bool_t flash_is_install(void)
{
    return sflashInited;
}

///////////////////////////////////////////IAP FS/////////////////////////////////////////////
//#include <device/include/unit_media.h>
//
////flash 信息描述
//static struct EmbdFlashDescr
//{
//    u16     BytesPerPage;                //一页中包含的字节数
//    u16     PagesPerBlock;               //每块中的页数
//    u16     TotalPages;                  //总页数量
//    u32     BytesPerBlock;               //一块中的字节数
//    u16     ToltalBlock;                 //总块数量
//    u32     RemainBytes;                 //剩余字节数
//    u32     MappedStAddr;
//}*sp_tFlashDesrc;
//
//s32 __embed_req(enum ucmd cmd, ptu32_t args, ...)
//{
//    s32 res = 0;
//
//    switch(cmd)
//    {
//
//        case remain:
//        {
//            va_list list;
//            u32 *left;
//            s64 *unit;
//
//            left = (u32*)args;
//            va_start(list, args);
//            unit = (s64*)va_arg(list, u32);
//            va_end(list);
////            if((*unit<64)&&(*unit>=0))
////                *left = 63 - *unit;
////            else if((*unit<128)&&(*unit>=64))
////                *left = 127 - *unit;
////            else if((*unit<192)&&(*unit>=128))
////                *left = 191 - *unit;
////            else if((*unit<256)&&(*unit>=192))
////                *left = 255 - *unit;
////            else
////                res = -1;
//            if(*unit > sp_tFlashDesrc->TotalPages)
//                res = -1;
//            else
//                *left = sp_tFlashDesrc->PagesPerBlock - 1 - (*unit)%sp_tFlashDesrc->PagesPerBlock;
//
//            break;
//        }
//
//        case whichblock:                              //哪一块
//        {
//            va_list list;
//            s64  *unit;
//            u32 *block;
//
//            block = (u32*)args;
//            va_start(list, args);
//            unit = (s64*)va_arg(list, u32);
//            va_end(list);
//            *block = (*unit)/sp_tFlashDesrc->PagesPerBlock;
//            break;
//        }
//
//        case totalblocks:                             //总块数量
//        {
//            // args = &blocks
//            *((u32*)args) =  sp_tFlashDesrc->ToltalBlock;
//            break;
//        }
//
//        case blockunits:                              //每块中的页数量
//        {
//
//            *((u32*)args)  = sp_tFlashDesrc->PagesPerBlock;
//            break;
//        }
//
//        case unitbytes:                               //每一页中的字节数
//        {
//            // args = &bytes
//            *((u32*)args) = sp_tFlashDesrc->BytesPerPage;
//            break;
//        }
//
//        case format:                                  //格式
//        {
//            va_list list;
//            u32 start, end;
//            struct uesz *sz;
//
//            start = (u32)args;
//            va_start(list, args);
//            end = va_arg(list, u32);
//            sz = (struct uesz*)va_arg(list, u32);
//            va_end(list);
//
//            if(!sz->block)
//                return (-1);
//
////            tmp = malloc(sp_tFlashDesrc->BytesPerPage);
////            if(!tmp)
////                return (-1);
//
//            if(-1==end)                                //挂在区域全部擦除
//                    end = sp_tFlashDesrc->ToltalBlock;
//            else if (start)
//                    end += start;
//
//            do
//            {
//                if(__embed_erase((s64)--end, *sz))
//                {
//                    res = -1;
//                    break;
//                }
//            }
//            while(end!=start);
//
//            break;
//        }
//
//        case mapaddr:
//        {
//
//            *((u32*)args) = sp_tFlashDesrc->MappedStAddr;
//            break;
//        }
//        case checkbad: break;
//        default: res = -1; break;
//    }
//
//    return (res);
//}
//
//// ============================================================================
//// 功能：embed 读；
//// 参数：unit -- 读的序号（页）；
////      data -- 读的数据；
////      opt -- 读的方式；
//// 备注：
//// ============================================================================
//static s32 __embed_read(s64 unit, void *data, struct uopt opt)
//{
//    uint32_t start_addr = 0x80000 + 1024 * unit;
//    djy_flash_read(start_addr, data, 1024);
//    return 0;
//}
//
//// ============================================================================
//// 功能：embed 写；
//// 参数：unit -- 写的序号（页）；
////      data -- 写的数据；
////      opt -- 写的方式；
//// 返回：成功（0）；失败（-1）；
//// 备注：
//// ============================================================================
//static s32 __embed_write(s64 unit, void *data, struct uopt opt)
//{
//    if(sp_tFlashDesrc==NULL)
//        return -1;
//    if(unit>(sp_tFlashDesrc->TotalPages))
//        return -1;
//    uint32_t start_addr = 0x80000 + 1024 * unit;
//    djy_flash_write(start_addr, data, 1024);
//    return 0;
//}
//
//// ============================================================================
//// 功能：nand 擦除
//// 参数：unit -- 擦除的序号；
////      sz -- 擦除的单位（unit或block）
//// 返回：成功（0）；失败（-1）；
//// 备注：
//// ============================================================================
//s32 __embed_erase(s64 unit, struct uesz sz)
//{
//    u32 block;
//    if(sp_tFlashDesrc==NULL)
//        return -1;
//    if(unit>(sp_tFlashDesrc->TotalPages))
//        return -1;
//    if(sz.unit)
//    {
//        if(__embed_req(whichblock, (ptu32_t)&block, &unit))
//            return (-1);
//    }
//    else
//        block = (u32)unit;
//
//    uint32_t start_addr = 0x80000 + 4096 * block;
//    djy_flash_erase(start_addr);
//    return 0;
//}
//
//static s32 Flash_Init(struct EmbdFlashDescr *Description)
//{
//
//      Description->BytesPerPage     = 1024;
//      Description->PagesPerBlock    = 4;
////      Description->PagesPerSector   = SECTOR_SIZE / PAGE_SIZE;
////      Description->SectorsPerBlock  = BLOCK_SIZE / SECTOR_SIZE;
//      Description->TotalPages       = 600;
//      Description->ToltalBlock      = 150;
//      Description->RemainBytes      = 0;
//
//      Description->MappedStAddr = 0;
//    return (0);
//}
//
//s32 __embed_part_init(u32 bstart, u32 bcount, u32 doformat)
//{
//    struct umedia *um;
//    struct uopt opt;
//    char name[16], part[3];
//    u32 units, total = 0;
//    static u8 count;
//
//    if(!sp_tFlashDesrc)
//    {
//        sp_tFlashDesrc = malloc(sizeof(*sp_tFlashDesrc));
//        if(!sp_tFlashDesrc)
//        {
//            return (-1);
//        }
//        Flash_Init(sp_tFlashDesrc);
////        djy_flash_init();
//    }
//
////    if(doformat)
////    {
////            djybsp_spiflash_step(DJYBSP_SPIFLASH_START);
////            djybsp_erase_some_sectors(0xc0000,SECTOR_SIZE);
////            djybsp_spiflash_step(DJYBSP_SPIFLASH_END);
////    }
//
//    um = malloc(sizeof(struct umedia)+sp_tFlashDesrc->BytesPerPage);
//    if(!um)
//    {
//        return (-1);
//    }
//
//    opt.hecc = 0;
//    opt.main = 1;
//    opt.necc = 1;
//    opt.secc = 0;
//    opt.spare = 1;
//    if(-1 == bcount)//最大块数
//    {
//        bcount = sp_tFlashDesrc->ToltalBlock;
//        bcount -= bstart;
//        um->asz = sp_tFlashDesrc->BytesPerPage * sp_tFlashDesrc->PagesPerBlock * bcount;
//    }
//    else
//        um->asz = sp_tFlashDesrc->BytesPerPage * sp_tFlashDesrc->PagesPerBlock * bcount;
//
//
////    um->esz = log2(sp_tFlashDesrc->BytesPerPage * sp_tFlashDesrc->PagesPerBlock); //
////    um->usz = log2(sp_tFlashDesrc->BytesPerPage);
//    um->esz = 12;
//    um->usz = 10;
//    //um->esz = 0; // 各个区域不同
//    um->merase = __embed_erase;
//    um->mread = __embed_read;
//    um->mreq = __embed_req;
//    um->mwrite = __embed_write;
//    um->opt = opt;
//    um->type = nand;
//    um->ubuf = (u8*)um + sizeof(struct umedia);
//    um->ustart = bstart*sp_tFlashDesrc->PagesPerBlock; // 起始unit号
//    bcount = 0;
//    total = 0;
//
//    um->ustart = total; // 起始unit号
//    itoa(count++, part, 10);
//    sprintf(name, "embed part %s", part);
//    if(um_add((const char*)name, um))
//    {
//        printf("\r\n: erro : device : %s addition failed.", name);
//        return (-1);
//    }
//
//    printf("\r\n: info : device : %s added(start:%d, blocks:%d).", name, bstart, bcount);
//    return (0);
//
//}


