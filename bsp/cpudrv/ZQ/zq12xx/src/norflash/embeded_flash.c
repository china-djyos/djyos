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
#ifdef CFG_CPU_ZQ12XX_M0
/*#include "norflash.h"*/
#include "silan_m0_cache.h"
#include "silan_irq.h"
#include "int.h"
#include "string.h"
#include <stdlib.h>
#include "flash.h"
#include "dbug.h"
#include <device/unit_media.h>
#include "filesystems.h"
#include "device.h"
#include "norflash.h"
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

//为了调试 方便，这里面 Debug 版本也设置可以通过终端下载

#define CN_APP_DEBUG  (true)

//当前链路状态

typedef enum EN_LINK_STATUS{
    EN_DOWN_APP_DEBUG_MODE = 0x00,  //下载APP debug模式
    EN_DOWN_APP_RELEASE_MODE,       //release 模式
    EN_DOWN_RISC_MODE,              //下载CK模式
    EN_DOWN_DSP_MODE,               //下载DSP模式

}EN_LinkStatus;

enum EN_SPI_OPT{
    EN_OPT_START = 0,
    EN_OPT_END,
};


/*对于app 而言第129块对应-〉0x40800
 *Risc的加载地址为：0x100000
 *若是升级Risc,那么写地址应对应0x100000：g_Map_Add_Start = 0x100000 - 0x40800
 *若是升级Dsp 0x100000 + 0x40000 为Dsp 的加载地址，此时：
 *若是升级Dsp g_Map_Add_Start　＝　0x100000 + 0x80000　－　0x40800
 * */

//默认 是升级app
static struct umedia *emflash_um;

static const char *EmflashName = "emflash";      //该flash在obj在的名字
extern struct Object *s_ptDeviceRoot;
static bool_t sEmflashInited = false;
//flash 信息描述
static struct EmbdFlashDescr
{
    u16     BytesPerPage;                //一页中包含的字节数
    u32     PagesPerBlock;               //每块中的页数
    u32     TotalPages;                  //总页数量
    u32     BytesPerBlock;               //一块中的字节数
    u16     ToltalBlock;                 //总块数量
    u32     RemainBytes;                 //剩余字节数
    u32     MappedStAddr;
}*sp_tFlashDesrc;

extern u32 Lock_Cache_Add_Start;
extern u32 Lock_Cache_Add_End;
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
// 功能：内置FLASH的初始化
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 Flash_Init(struct EmbdFlashDescr *Description)
{
      iap_norflash_init();

      Description->BytesPerPage     = PAGE_SIZE;                    //一页中包含的字节数
      Description->BytesPerBlock    = BLOCK_SIZE;                    //一页中包含的字节数
      Description->PagesPerBlock    = BLOCK_SIZE / PAGE_SIZE;       //12Pages
      Description->ToltalBlock      =  FLASH_SIZE /  BLOCK_SIZE;  //2097152/2048=1024
      Description->TotalPages       = (Description->PagesPerBlock)*(Description->ToltalBlock);    //2097152/512=4096
      Description->RemainBytes      = 0;   //898Page

      Description->MappedStAddr = 0x00000000;
     return (0);
}
// ============================================================================

// ============================================================================
// 功能：进入flash 操作模式(操作模式包括写、擦除)
// 参数：
// 返回：
// 备注：
// ============================================================================
extern void silan_m0_cache_enable(void);
extern void silan_m0_cache_disable(void);
extern void silan_m0_cache_clear(void);
extern void silan_m0_cache_lock(uint32_t lock_addr, uint32_t len);
extern void silan_m0_cache_unlock(uint32_t lock_addr, uint32_t len, uint8_t unvalid);
extern int32_t iap_norflash_init();

static void SpiFlashOptMode(u8 Mode)
{
     u32 optStartAddr;
     u32 Remain;
     u32 optAddrLen;

    switch(Mode)
    {
        case EN_OPT_START:
             silan_m0_cache_disable();
             silan_m0_cache_clear();
             optStartAddr =    ((u32) &Lock_Cache_Add_Start) & 0xFFFFFFF0;
             Remain       =    ((u32) &Lock_Cache_Add_Start) & 0x0000000F;
             optAddrLen   =    ((((u32) &Lock_Cache_Add_End) - ((u32) &Lock_Cache_Add_Start)) + Remain + M0_CACHE_LINE_SIZE - 1) / M0_CACHE_LINE_SIZE;
             silan_m0_cache_lock(optStartAddr, optAddrLen);
             silan_m0_cache_enable();
             iap_norflash_init();
             break;
        case EN_OPT_END:
             optStartAddr =    ((u32) &Lock_Cache_Add_Start) & 0xFFFFFFF0;
             Remain       =    ((u32) &Lock_Cache_Add_Start) & 0x0000000F;
             optAddrLen   =    ((((u32) &Lock_Cache_Add_End) - ((u32) &Lock_Cache_Add_Start)) + Remain + M0_CACHE_LINE_SIZE - 1) / M0_CACHE_LINE_SIZE;
             silan_m0_cache_unlock(optStartAddr, optAddrLen, 0);
             break;
    }
}


// 功能：块擦除
// 参数：SectorNo -- 扇区号;
// 返回："0" -- 成功;"-1" -- 失败;
// 备注：
// ============================================================================
static s32 Flash_BlockEarse(u32 SectorNo)
{
    u32 Addr;
    s32 Ret = 0;
    atom_high_t high_atom;
    high_atom =Int_HighAtomStart();
    SpiFlashOptMode(EN_OPT_START);
    Addr = SectorNo * BLOCK_SIZE + sp_tFlashDesrc->MappedStAddr ;
    EraseSomeBlocks(Addr,BLOCK_SIZE);
    SpiFlashOptMode(EN_OPT_END);
    Int_HighAtomEnd(high_atom);
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
    u32 datLen;
    u32 DatAddr;
    Flags =Flags;
    atom_high_t high_atom;
    DatAddr = (Page) * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr ;

    high_atom =Int_HighAtomStart();
    SpiFlashOptMode(EN_OPT_START);

    datLen       = sp_tFlashDesrc->BytesPerPage;
    //ProgramOnePackage((u8*)Data, DatAddr, datLen);
    ProgramOnePackage(Data,DatAddr,datLen);

    SpiFlashOptMode(EN_OPT_END);
    Int_HighAtomEnd(high_atom);
    return sp_tFlashDesrc->BytesPerPage;
}


// ============================================================================
// 功能：读某页
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 Flash_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    u8  *ptReadAddr;
    u32 readDatNo;
    u32 loop;
    Flags = Flags;

    ptReadAddr   = (u8*)(PageNo * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr );
    readDatNo    = sp_tFlashDesrc->BytesPerPage;

    if(Data)
    {
        for(loop = 0;loop < readDatNo;loop++)
        {
            Data[loop] = ptReadAddr[loop];
        }

    }
    return (sp_tFlashDesrc->BytesPerPage);
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

        case whichblock:   //找出该页所在块,现划分一块=4Pages
        {
            va_list list;
            s64 *unit;
            u32 *block;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            if(*unit <= sp_tFlashDesrc->TotalPages)     //判断是否超总页数
            {
                *block = *unit / sp_tFlashDesrc->PagesPerBlock;
            }
            else
            {
                res = -1;
            }

//            if((*unit<12)&&(*unit>=0))//one blk = 12 Pages
//               *block = 0;
           break;
        }

        case totalblocks:                                //总块数量
        {
            // args = &blocks
            *((u32*)args) =  sp_tFlashDesrc->ToltalBlock;
            break;
        }

        case blockunits:                              //每块中的页数量
        {

            *((u32*)args)  = sp_tFlashDesrc->PagesPerBlock;
            break;
        }

        case unitbytes:                               //每一页中的字节数
        {
            // args = &bytes
            *((u32*)args) = sp_tFlashDesrc->BytesPerPage;
            break;
        }

        case format:                                  //格式
        {
            va_list list;
            s32 start,end;
            struct uesz *sz;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(!sz->block)
               return (-1);

//            tmp = malloc(sp_tFlashDesrc->BytesPerPage);
//            if(!tmp)
//                return (-1);
//
            if(-1==end)
                end = sp_tFlashDesrc->ToltalBlock; // 结束的号；

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

            *((u32*)args) = sp_tFlashDesrc->MappedStAddr;
            break;
        }
        case checkbad: break;
        case remain :          //该页所在的块中还剩余多少页,页数是依次地增加的
        {
            va_list list;
            u32 *left;
            s64 *unit;

            left = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            if(*unit < sp_tFlashDesrc->TotalPages)
            {
                //求出该页所在的块

                *left = (sp_tFlashDesrc->PagesPerBlock - (*unit%sp_tFlashDesrc->PagesPerBlock))%sp_tFlashDesrc->PagesPerBlock;
            }
            else
                res = -1;

             break;
        }
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
    opt = opt;
    res = Flash_PageRead((u32)unit, data, 0);
    if(res!=sp_tFlashDesrc->BytesPerPage)
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
    opt = opt;
    res = Flash_PageProgram((u32)unit, data, 0);
    if(res!=sp_tFlashDesrc->BytesPerPage)
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
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res;
    s32 BlockNum;

    targetobj = OBJ_MatchPath(fs, &notfind);
    if(notfind)
    {
        error_printf("spiflash"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj);
    super->MediaInfo = emflash_um;
    super->MediaDrv = mediadrv;

    if(bend == -1)
    {
        bend = sp_tFlashDesrc->ToltalBlock;
        BlockNum = bend - bstart;
    }
    else
    {
        BlockNum = bend - bstart;
    }
    super->AreaSize = BlockNum * sp_tFlashDesrc->BytesPerBlock;
    super->MediaStart = bstart * sp_tFlashDesrc->PagesPerBlock; // 起始unit号

    res = strlen(EmflashName)+strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,EmflashName);      //获取设备的全路径
    File_BeMedia(FullPath,fs);     //往该设备挂载文件系统
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);

}
//-----------------------------------------------------------------------------
// 功能：安装片内Flash驱动
// 参数：doformat -- 是否格式化；
// 返回：成功（0）；失败（-1）；
// 备注：如果还不知道要安装什么文件系统，或者不安装文件系统TargetFs填NULL，TargetPart填-1；
//-----------------------------------------------------------------------------
s32 ModuleInstall_EmbededFlash(u32 doformat)
{
    if(!sp_tFlashDesrc)
    {
        sp_tFlashDesrc = malloc(sizeof(*sp_tFlashDesrc));
        if(!sp_tFlashDesrc)
        {
            return (-1);
        }

        Flash_Init(sp_tFlashDesrc);
    }

    if(doformat)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __embed_req(format, (ptu32_t)0 , -1, &sz);       //格式化指定区域
    }

    emflash_um = malloc(sizeof(struct umedia)+sp_tFlashDesrc->BytesPerPage);
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
    return 0;
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
#endif

