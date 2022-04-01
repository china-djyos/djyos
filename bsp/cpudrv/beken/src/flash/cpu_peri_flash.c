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
#include <djyfs/filesystems.h>
#include <device.h>
#include <device/unit_media.h>
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
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
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
bool_t data_mode = false;      //true：对flash的数据操作带CRC，false：对flash的数据状态不带CRC

struct NorDescr *nordescription;

extern void calc_crc(u32 *buf, u32 packet_num);
void encrypt(u32 *rx, u8 *tx, u32 num);
//static struct FlashDescr{
//    u32     BytesPerPage;                // 页中包含的字节数
//    u32     PagesPerSector;               //  sector的页数
//    u32     AllSectorNum;               //  所有的sector数
//    u32     MappedStAddr;
//} *description;

// ============================================================================
// 功能：设置内置FLASH的信息
// 参数：
// 返回：
// 备注：
// ============================================================================
//static s32 SetFlash_Init(struct FlashDescr *Description)
//{
//    Description->BytesPerPage = 256;
//    Description->PagesPerSector = 16;
//    Description->AllSectorNum = 1024;
//    Description->MappedStAddr = 0x0;
//    return (0);
//}

// ============================================================================
// 功能：设置操作flash数据时需不需要考虑CRC
// 参数：flag:true--待写入的数据已经有crc，或者不需要CRC，
//      false--需要crc，但数据不带crc，须写入函数添加CRC
// 返回：
// 备注：
// ============================================================================
void SetOperFalshMode(bool_t flag)
{
    data_mode = flag;
}

// ============================================================================
// 功能：获取操作flash数据时需不需要考虑CRC
// 参数：flag:true--不需要考虑CRC，false--需要计算CRC
// 返回：
// 备注：
// ============================================================================
bool_t GetOperFalshMode(void)
{
    return data_mode;
}


static s32 SetFlash_Init(struct NorDescr *Description)
{
    Description->BytesPerPage = 256;
    Description->PagesPerSector = 16;
    Description->SectorNum = 1024;
//    Description->MappedStAddr = 0x0;
    return (0);
}

// ============================================================================
// 功能：读取带crc区的flash数据，返回的数据中包含了crc区域，但作为普通数据使用
// 参数：address -- 地址；
//      data -- 读数据缓存；
//      size -- 读取的字节数
// 返回：无
// 备注：
// ============================================================================
void djy_flash_read_crc(uint32_t address, void *data, uint32_t size)
{
    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
//    flash_protection_op(0,FLASH_PROTECT_NONE);
    flash_read(data, size, address);
//    flash_protection_op(0,FLASH_PROTECT_ALL);
    Lock_MutexPost(flash_mutex);
}
// ============================================================================
// 功能：读取不带crc的flash数据，即把读出的数据中去掉crc后提交给用户。
// 参数：address -- 地址；计算了crc之后的物理地址。
//      data -- 读数据缓存；
//      size -- 读取的字节数
// 返回：无
// 备注：
// ============================================================================
void djy_flash_read(uint32_t address, void *data, uint32_t size)
{
    u32 i = 0;   //读的时候这里也不做偏移，和擦除一样，在xip里做好偏移。
    u8 buf[272];
    if (size == 0)
    {
        return;
    }
    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
    while(size)
    {
        memset(buf, 0xFF, 272);
        flash_read((char *)buf, 272, address);//读取带crc的256个数据，因为带crc，所以要读272
        i = 0;
        while(i < 272)  //把crc数据去掉，只留有效数据
        {
            if(size > 32)
                memcpy(data, buf + i, 32);
            else
            {
                memcpy(data, buf + i, size);
                size = 0;
                break;
            }
            data += 32;
            i += 34;
            size -= 32;
        }
        address += 272;
    }
    Lock_MutexPost(flash_mutex);
}
// ============================================================================
// 功能：写入flash，写入时是否添加crc，有 GetOperFalshMode() 函数确定。
// 参数：address，写入地址,是物理地址。
//      data -- 读数据缓存；
//      size -- 读取的字节数
// 返回：无
// 备注：
// ============================================================================
void djy_flash_write(uint32_t address, const void *data, uint32_t size)
{
    u32 i, len;
    u8 buf_crc[272],buf[256];
    if (size == 0)
        return;

    if(address + size >= 0x400000)
    {
        error_printf("embedflash"," write address out of range.\r\n");
        return;
    }

    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
    if(GetOperFalshMode() == true)
    {
        //不考虑crc,如果crc不正确，也可以正常写入但不能用地址映射方式访问，否则CPU
        //会进入不可知状态（仿真器也无法连接）。
        flash_write((char *)data, size, address);
    }
    else
    {
        while(size)             //写不带crc的数据，这种方式会自动算好crc再写入flash
        {
            memset(buf_crc, 0xff, 272);
            if(size >= 256)
            {
                encrypt((u32 *)data, buf_crc, 256 / 32);
                size -= 256;
                data += 256;
                len = 272;
                calc_crc((u32 *)buf_crc, 256 / 32);     //计算256字节的crc数据
            }
            else
            {
                memset(buf, 0xff, 256);
                memcpy(buf, data, size);
                if(size % 32)
                    i = (size + 32) - (size % 32);
                else
                    i = size;
                encrypt((u32 *)buf, buf_crc, i / 32);
                size = 0;
                len = i * 34 / 32;
                calc_crc((u32 *)buf_crc, i / 32);
            }

            flash_write((char *)buf_crc, len, address);
            address += len;
        }
    }

//    flash_write((char *)data, size, practical_addr);
//    flash_protection_op(0,FLASH_PROTECT_ALL);
    Lock_MutexPost(flash_mutex);
}
// ============================================================================
// 功能：写入flash，写入时是否添加crc，由GetOperFalshMode() 函数确定。
// 参数：address，写入地址,如果GetOperFalshMode==true，是物理地址，否则是逻辑地址
//      data -- 读数据缓存；
//      size -- 读取的字节数
// 返回：无
// 备注：
// ============================================================================
void djy_flash_write_ori(uint32_t address, const void *data, uint32_t size)
{
    u32 i, len,inaddress;
    u8 buf_crc[272],buf[256];
    if (size == 0)
        return;

    if(address + size >= 0x400000)
    {
        error_printf("embedflash"," write address out of range.\r\n");
        return;
    }

    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
    if(GetOperFalshMode() == true)
    {
        //不考虑crc,如果crc不正确，也可以正常写入但不能用地址映射方式访问，否则CPU
        //会进入不可知状态（仿真器也无法连接）。
        flash_write((char *)data, size, address);
    }
    else
    {
        inaddress = address *34/32; //逻辑地址转换为物理地址。
        while(size)             //写不带crc的数据，这种方式会自动算好crc再写入flash
        {
            memset(buf_crc, 0xff, 272);
            if(size >= 256)
            {
                encrypt((u32 *)data, buf_crc, 256 / 32);
                size -= 256;
                data += 256;
                len = 272;
                calc_crc((u32 *)buf_crc, 256 / 32);     //计算256字节的crc数据
            }
            else
            {
                memset(buf, 0xff, 256);
                memcpy(buf, data, size);
                if(size % 32)
                    i = (size + 32) - (size % 32);
                else
                    i = size;
                encrypt((u32 *)buf, buf_crc, i / 32);
                size = 0;
                len = i * 34 / 32;
                calc_crc((u32 *)buf_crc, i / 32);
            }

            flash_write((char *)buf_crc, len, inaddress);
            inaddress += len;
        }
    }

//    flash_write((char *)data, size, practical_addr);
//    flash_protection_op(0,FLASH_PROTECT_ALL);
    Lock_MutexPost(flash_mutex);
}


//-----------------------------------------------------------------------------
//功能：擦除flash一个扇区。一个sector是4K。
//参数：address，被擦除的扇区所在的物理地址
//返回：无
//-----------------------------------------------------------------------------
void djy_flash_erase(uint32_t address)
{
//    u32 practical_addr = address * 34 / 32;
    address &= (0xFFF000);
    if(address >= 0x400000)
        return;
    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
//    flash_protection_op(0,FLASH_PROTECT_NONE);
    flash_ctrl(CMD_FLASH_ERASE_SECTOR, &address);
//    flash_protection_op(0,FLASH_PROTECT_ALL);
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
            *left = (nordescription->PagesPerSector - 1) - ((*unit / nordescription->BytesPerPage) % nordescription->PagesPerSector);
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

            *block = *unit / (nordescription->BytesPerPage * nordescription->PagesPerSector);

            break;
        }
        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = nordescription->BytesPerPage;
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
                end = nordescription->SectorNum;// 结束的号；
            do
            {
                djy_flash_erase((u32)((--end * ((u32)(nordescription->BytesPerPage * nordescription->PagesPerSector)))));
            }
            while(end!=start);

            break;
        }
        case blockunits:
        case totalblocks:
        {
            *((u32*)args) = nordescription->PagesPerSector;
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
// 参数：fs -- 需要挂载的文件系统，
//      mediadrv -- 媒体驱动，
//      bstart -- 起始块，
//      bend -- 结束块（不包括该块，只到该块的上一块）
// 返回：0 -- 成功， -1 -- 失败
// 备注：todo:逻辑反了，不是文件系统安装到flash，而是flash挂接到文件系统中，须改之。
// ============================================================================
s32 EmbFsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv)
{
     char *FullPath,*notfind;
     struct Object *targetobj;
     struct FsCore *super;
     s32 res,endblock = bend;

     if(mediadrv == NULL)
         return -1;
     targetobj = OBJ_MatchPath(fs, &notfind);
     if(notfind)
     {
         error_printf("embedflash"," not found need to install file system.\r\n");
         return -1;
     }
     super = (struct FsCore *)OBJ_GetPrivate(targetobj);
     super->MediaInfo = flash_um;
     super->MediaDrv = mediadrv;

     super->MediaStart = bstart * nordescription->PagesPerSector; // 起始unit号

     //计算总容量，如果计算的总容量大于芯片的总容量了，则把芯片的总容量减起始位置
     //这里的XIP是指XIP-APP和XIP-IBOOT，如果这两个文件系统的 pType 改了，这里也要做相应的修改
     if(strstr(super->pFsType->pType, "XIP"))
     {

         if(-1 == (s32)endblock)
             endblock = nordescription->SectorNum*32/34;       // 最大块号
         super->AreaSize = ((endblock * nordescription->BytesPerPage * nordescription->PagesPerSector)- (super->MediaStart * nordescription->BytesPerPage)) * 34 / 32;
         if(super->AreaSize + ((super->MediaStart * nordescription->BytesPerPage) * 34 / 32)  >
                                     nordescription->SectorNum * nordescription->BytesPerPage * nordescription->PagesPerSector)
         {
             super->AreaSize = nordescription->SectorNum * nordescription->BytesPerPage *
                     nordescription->PagesPerSector - ((super->MediaStart * nordescription->BytesPerPage) * 34 / 32);
         }
         info_printf("embedf","%s fileOS total capacity(size : %lld)， available capacity (size : %lld).", fs, super->AreaSize,super->AreaSize / 34 * 32);
     }
     else
     {
         if(-1 == (s32)endblock)
             endblock = nordescription->SectorNum; // 最大块号
         //不是XIP的文件系统，填什么区域就是什么区域，不会去做*34/32的偏移，在使用时，需要自行计算好位置。
         super->AreaSize = (endblock * nordescription->BytesPerPage * nordescription->PagesPerSector)- (super->MediaStart * nordescription->BytesPerPage);
         if(super->AreaSize + (super->MediaStart * nordescription->BytesPerPage) >
                                     nordescription->SectorNum * nordescription->BytesPerPage * nordescription->PagesPerSector)
         {
             super->AreaSize = nordescription->SectorNum * nordescription->BytesPerPage *
                     nordescription->PagesPerSector - (super->MediaStart * nordescription->BytesPerPage);
         }
         info_printf("embedf","%s fileOS available capacity (size : %lld).", fs, super->AreaSize);
     }
     res = strlen(flash_name) + strlen(s_ptDeviceRoot->name) + 1;
     FullPath = malloc(res);
     memset(FullPath, 0, res);
     sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,flash_name);   //获取该设备的全路径
     File_BeMedia(FullPath,fs); //往该设备挂载文件系统
     free(FullPath);
     info_printf("embedf","device : %s added(start:%d, end:%d).", fs, bstart, bend);
     return (0);
}
// ============================================================================
// 功能：初始化片内flash
// 参数：无
// 返回：0 -- 成功， -1 -- 失败
// 备注：
// ============================================================================
s32 ModuleInstall_Flash(void)
{
    if(!nordescription) //初始化nor的信息
    {
        nordescription = malloc(sizeof(struct NorDescr));
        if(!nordescription)
        {
            printf("\r\n: erro : device : memory out.\r\n");
            return -1;
        }
        memset(nordescription, 0x0, (sizeof(struct NorDescr)));
    }
    SetFlash_Init(nordescription);


    flash_um = malloc(sizeof(struct umedia)+nordescription->BytesPerPage);
    if(!flash_um)
        return (-1);

    flash_um->mreq = djy_flash_req;
    flash_um->type = embed;
    flash_um->ubuf = (u8*)flash_um + sizeof(struct umedia);

    if(!Device_Create((const char*)flash_name, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)flash_um)))
    {
        error_printf("embedflash","device : %s addition failed.", flash_name);
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
// 参数：无
// 返回：已成功安装（true）；未成功安装（false）；
// 备注：
// =============================================================================
bool_t flash_is_install(void)
{
    return sflashInited;
}

//因调试会导致BK7251内部flash的crc校验错乱，而只要访问crc错乱区域，CPU就会进入一种
//“不可知”状态，连仿真器也连不上，只能使用烧录器重烧程序，给仿真调试带来极大的困难。
//调用本函数前，如果程序未执行到crc错乱的地方，则能够被捕捉到。因此，本函数应及早调
//用。一般来说，在程序刚刚启动时调用最好，此时尚未初始化串口，无法打印。

struct flash_CrcUnit
{
    u8 DataUnit[32];
    u16 CrcUnit;
};
//start是物理起始地址，len是物理长度，须是1088的整数倍（对应逻辑长度1Kbytes）
bool_t flash_CheckCrc(u32 start, u32 len)
{
    struct flash_CrcUnit flashbuf[32];
    u16 flashcrc[32];
    bool_t result = true;;
    u32 address,verify;
    for(address = start; address < len; address +=sizeof(flashbuf))
    {
        flash_read((char *)flashbuf, sizeof(flashbuf), address);
        for(verify = 0; verify < 32; verify++)
            flashcrc[verify] = flashbuf[verify].CrcUnit;
        calc_crc((u32 *)flashbuf, 32);     //计算32个单元的crc数据
        for(verify = 0; verify < 32; verify++)
        {
            if(flashcrc[verify] != flashbuf[verify].CrcUnit)
            {
                result = false;
                return result;
            }
        }
    }
    return result;
}

