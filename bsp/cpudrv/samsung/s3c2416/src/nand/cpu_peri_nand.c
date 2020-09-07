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
#include <device.h>
#include <device/djy_flash.h>
#include <cpu_peri.h>
#include <djyos.h>
#include <device/unit_media.h>
#include <board.h>
#include <libc/misc/ecc/ecc_256.h>
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <math.h>
#include "stm32f7xx_hal_conf.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//   extern s32 ModuleInstall_NAND(u32 doformat);
//   ModuleInstall_NAND(CFG_NFLASH_PART_FORMAT);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip nand"//CPU的nand驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","file system"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                        //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_NAND == false )
//#warning  " cpu_onchip_nand  组件参数未配置，使用默认配置"
//%$#@target = header   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_NAND    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,true,false,
#define CFG_NFLASH_PART_FORMAT     false      //是否需要擦除该芯片。
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure

//上面的分区起始和分区结束我写的是-1到10000，具体数值可根据具体芯片参数填写

#define NAND_CONTROLLER_REG                 (pg_nand_reg)
#define NandChipOn()                        (NAND_CONTROLLER_REG->NFCONT &= ~(1<<1))
#define NandChipOff()                       (NAND_CONTROLLER_REG->NFCONT |= (1<<1))
#define SetNandCommand(cmd)                 (NAND_CONTROLLER_REG->NFCMD = cmd)
#define SetNandAddr(i, addr)                (NAND_CONTROLLER_REG->NFADDR = addr)
#define SetNandData(data)                   (NAND_CONTROLLER_REG->NFDATA = data)
#define GetNandData()                       (NAND_CONTROLLER_REG->NFDATA)

s32 __nand_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
s32 __nand_req(enum ucmd cmd, ptu32_t args, ...);
s32 __nand_read(s64 unit, void *data, struct uopt opt);
s32 __nand_write(s64 unit, void *data, struct uopt opt);
s32 __nand_erase(s64 unit, struct uesz sz);
static u32 *badstable = NULL;
static u32 badslocation = 0;
static bool_t sNandflashInited = false;
#define NFlashLockTimeOut     CN_CFG_TICK_US * 1000 * 10
extern s32 deonfi(const char *data, struct NandDescr *onfi, u8 little);
extern struct Object *s_ptDeviceRoot;
static struct NandDescr *s_ptNandInfo;
static struct MutexLCB *NandFlashLock;
struct umedia *nand_umedia;
static const char *NandFlashName = "nand";      //该flash在obj在的名字

static void ResetNand(void);
static s32 StatusOfNand(void);
static void WaitNandReady(void);
//-----------------------------------------------------------------------------
//功能:
//参数:
//输出: ">0" -- 写成功; "-2" -- 写失败;
//返回:
//-----------------------------------------------------------------------------
s32 S3C2416_SpareProgram(u32 PageNo, const u8 *Data)
{

    u32 i;
    s32 Ret;
    u32 SpareOffset = s_ptNandInfo->BytesPerPage;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    NandChipOn();

    SetNandCommand(PAGE_PROGRAM_CMD_BYTE1);

    NAND_CONTROLLER_REG->NFADDR = (u8)SpareOffset;
    NAND_CONTROLLER_REG->NFADDR = (u8)((SpareOffset>>8) & 0xF);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo>>8);
    NAND_CONTROLLER_REG->NFADDR = (u8)((PageNo>>16) & 0x1);

    for(i = 0; i < (s_ptNandInfo->OOB_Size); i++)
        SetNandData(Data[i]);

    SetNandCommand(PAGE_PROGRAM_CMD_BYTE2);// 写入Main数据完成

    DJY_EventDelay(700);// 切出
    WaitNandReady();//时序要求

    Ret = StatusOfNand();

    NandChipOff();

    if(Ret)
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }
    Lock_MutexPost(NandFlashLock);
    return (s_ptNandInfo->OOB_Size);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//输出: ">0" -- 读成功; "-2" -- 写失败;
//返回:
//-----------------------------------------------------------------------------
s32 S3C2416_SpareRead(u32 PageNo, u8 *Data)
{
    u8 i;
    s32 Ret;
    u32 SpareOffset = s_ptNandInfo->BytesPerPage;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    NandChipOn();

    SetNandCommand(PAGE_READ_CMD_BYTE1);

    NAND_CONTROLLER_REG->NFADDR = (u8)(SpareOffset);
    NAND_CONTROLLER_REG->NFADDR = (u8)((SpareOffset>>8) & 0xF);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo>>8);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo>>16) & 0x1;

    SetNandCommand(PAGE_READ_CMD_BYTE2);

    DJY_EventDelay(25);// 切出
    WaitNandReady();//时序要求

    for(i = 0; i < s_ptNandInfo->OOB_Size; i++)
        Data[i] = GetNandData();

    Ret = StatusOfNand();

    NandChipOff();

    if(Ret)
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }
    Lock_MutexPost(NandFlashLock);
    return (s_ptNandInfo->OOB_Size);
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
s32  S3C2416_PageProgram(u32 PageNo, const u8 *Data, u32 Flags)
{
    //逻辑:
    //
    u32 i, EccOffset;
    s32 Ret;
    u8 *Spare;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    // ECC 功能
    Ret = NAND_CONTROLLER_REG->NFMECC0;//测试
    switch (Flags & MASK_ECC)
    {
        case HW_ECC :
        {
            NAND_CONTROLLER_REG->NFCONT &= (~(1<<7));// 开启(解锁)Main ECC
            NAND_CONTROLLER_REG->NFCONT |= (1<<5);// 初始化Main ECC
            break;
        }
        case SW_ECC :
        case NO_ECC :
        {
            NAND_CONTROLLER_REG->NFCONT |= (1<<7);// 关闭Main ECC
            break;
        }
        default :
            Lock_MutexPost(NandFlashLock);
            return (-1);
    }

    NandChipOn();

    SetNandCommand(PAGE_PROGRAM_CMD_BYTE1);

    NAND_CONTROLLER_REG->NFADDR = 0x0;
    NAND_CONTROLLER_REG->NFADDR = 0x0;
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo>>8);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo>>16) & 0x1;

    for(i = 0; i < s_ptNandInfo->BytesPerPage; i++)
        SetNandData(Data[i]);

    SetNandCommand(PAGE_PROGRAM_CMD_BYTE2);// 写入Main数据完成

    DJY_EventDelay(700);// 切出
    WaitNandReady();// 时序要求

    Ret = StatusOfNand();

    NandChipOff();

    if(Ret)
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }

    if (!((SPARE_REQ & Flags) || (HW_ECC & Flags)))
    {
        Lock_MutexPost(NandFlashLock);
        return (s_ptNandInfo->BytesPerPage);// 只写页,结束退出
    }

    Spare = (u8*)Data + s_ptNandInfo->BytesPerPage;// 注意：这里是基于驱动都有统一的缓冲块逻辑

    if(HW_ECC & Flags)
    {
        NAND_CONTROLLER_REG->NFCONT |= (1<<7);// 关闭(锁)Mian ECC
        EccOffset = s_ptNandInfo->OOB_Size - 4;// 4个字节(1-bit ECC校验)
        Ret = NAND_CONTROLLER_REG->NFMECC0;//测试
        *(u32*)(Spare + EccOffset) = NAND_CONTROLLER_REG->NFMECC0;//
    }

    if(!(SPARE_REQ & Flags))
        memset(Spare, 0xFF, EccOffset);// 未要求写spare,则默认写0xFF

    if(-2 == S3C2416_SpareProgram(PageNo, Spare))
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }

    Lock_MutexPost(NandFlashLock);
    return (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回: ">0" -- 正常，读出的字节数。
//      "-1" -- 输入参数错误;
//      "-2" -- 读失败;
//      "-3" -- ECC纠错失败;
//备注: 不管读写是否正确，都将数据回传
//-----------------------------------------------------------------------------
s32  S3C2416_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    //逻辑:
    //
    u32 i;
    s32 Ret;
    u8 *Spare;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }
    // ecc 功能
    switch (Flags & MASK_ECC)
    {
        case HW_ECC :
        {
            NAND_CONTROLLER_REG->NFCONT &= (~(1<<7));// 开启(解锁)Main ECC
            NAND_CONTROLLER_REG->NFCONT |= (1<<5);// 初始化Main ECC
            break;
        }
        case SW_ECC :
        case NO_ECC :
        {
            NAND_CONTROLLER_REG->NFCONT |= (1<<7);// 关闭Main ECC
            break;
        }
        default :
            Lock_MutexPost(NandFlashLock);
            return (-1);
    }

    NandChipOn();

    SetNandCommand(PAGE_READ_CMD_BYTE1);

    NAND_CONTROLLER_REG->NFADDR = 0x0;
    NAND_CONTROLLER_REG->NFADDR = 0x0;
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo>>8);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo>>16) & 0x1;

    SetNandCommand(PAGE_READ_CMD_BYTE2);

    DJY_EventDelay(25);// 切出
    WaitNandReady();// 时序要求

    for(i = 0; i < s_ptNandInfo->BytesPerPage; i++)
        Data[i] = GetNandData();

    Ret = StatusOfNand();

    NandChipOff();

    if(Ret)
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }

    if (!((SPARE_REQ & Flags) || (HW_ECC & Flags)))
    {
        Lock_MutexPost(NandFlashLock);
        return (s_ptNandInfo->BytesPerPage);// 只读页,结束退出
    }

    Spare = Data + s_ptNandInfo->BytesPerPage;// 注意：这里是基于驱动都有统一的缓冲块逻辑

    if(HW_ECC & Flags)
        NAND_CONTROLLER_REG->NFCONT |= (1<<7);// 关闭(锁)Mian ECC

    if(-2 == S3C2416_SpareRead(PageNo, Spare))
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }

    if(HW_ECC & Flags)
    {
        u32 EccRes, EccOffset, EccStatus;

        EccOffset = s_ptNandInfo->OOB_Size - 4;//
        EccRes = *(u32*)(Spare + EccOffset);
        // 针对YAFFS2的逻辑,HW_ECC读一个新擦除块的页，ECC校验码为0xFFFFFFFF(-1),
        // 代入if里的逻辑会执行纠错，修改读回的数据, 需将屏蔽掉这个错误
        if(EccRes != -1)
        {
            NAND_CONTROLLER_REG->NFMECCD0 = ((u32)((EccRes & 0xFF00) << 8) | (u32)(EccRes & 0xFF));
            NAND_CONTROLLER_REG->NFMECCD1 = ((u32)((EccRes & 0xFF000000) >> 8) | (u32)(EccRes & 0xFF0000) >> 16);

            EccStatus = NAND_CONTROLLER_REG->NFECCERR0;
            switch(EccStatus & 0x3)
            {
                case 0: break;// 无错
                case 1:// 1位错，可纠错
                {
                    Data[(EccStatus >> 7) & 0x7ff] ^= (1 << ((EccStatus >> 4) & 0x7));
                    debug_printf("nand driver"，"fix: bit 0x%x in Byte 0x%x\r\n",
                            ((EccStatus >> 4) & 0x7), ((EccStatus >> 7) & 0x7ff));
                    break;
                }
                case 2:// 错误位太多, 不可纠错
                case 3:// ECC计算错误
                    debug_printf("nand driver", "cannot be fixed");
                    Lock_MutexPost(NandFlashLock);
                    return (-3);
            }
        }
    }
    Lock_MutexPost(NandFlashLock);
    return (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size);

}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回: "0" -- 成功;"-1" -- 失败;
//备注:
//-----------------------------------------------------------------------------
s32 S3C2416_BlockErase(u32 BlkNo)
{
    s32 Ret;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-1);
    }

    NandChipOn();

    SetNandCommand(BLOCK_ERASE_CMD_BYTE1);

    NAND_CONTROLLER_REG->NFADDR = (u8)(BlkNo << 6);// 3个地址周期表示的是页号
    NAND_CONTROLLER_REG->NFADDR = (u8)(BlkNo >> 2);
    NAND_CONTROLLER_REG->NFADDR = (u8)(BlkNo >> 10);

    SetNandCommand(BLOCK_ERASE_CMD_BYTE2);

    DJY_EventDelay(3000);// 切出
    WaitNandReady();

    Ret = StatusOfNand();

    NandChipOff();
    Lock_MutexPost(NandFlashLock);
    return (Ret);
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回: "0" -- 好块;
//      "-1" -- 坏块;
//      "-2" -- 内存不足
//      "-3" -- 读失败;
//备注:
//-----------------------------------------------------------------------------
s32 S3C2416_BadChk(u32 BlkNo)
{
    u8 *Spare, i;
    s32 Ret = 0;
    u32 PageNo = BlkNo * s_ptNandInfo->PagesPerBlk;

    Spare = malloc (s_ptNandInfo->OOB_Size);
    if (NULL == Spare)
        return (-2);

    for (i = 0; i < 2; i++)// 只判断每块的首两页
    {
        if(-2 == S3C2416_SpareRead(PageNo + i, Spare))
        {
            Ret = -3;
            break;
        }

        if ((0xAA == Spare[s_ptNandInfo->BadMarkOffset]) &&
            (0xAA == Spare[s_ptNandInfo->BadMarkOffset-1]))
        {
            Ret = -1;
            break;
        }
    }

    free(Spare);

    return (Ret);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回: "0" -- 成功;
//      "-1" -- 失败;
//      "-2" -- 内存不足;
//备注:
//-----------------------------------------------------------------------------
s32  S3C2416_BadMark(u32 BlkNo)
{
    //逻辑:
    //    不管BAD MARK位原来是多少以及是否有ECC校验问题, 只管将该位标志为坏块

    u8 *Spare, i;
    s32 Ret = 0;
    u32 PageNo = BlkNo * s_ptNandInfo->PagesPerBlk;

    Spare = malloc (s_ptNandInfo->OOB_Size);
    if (NULL == Spare)
        return (-2);


    memset(Spare, 0xFF, s_ptNandInfo->OOB_Size);
    Spare[s_ptNandInfo->BadMarkOffset] = 0xAA;
    Spare[s_ptNandInfo->BadMarkOffset-1] = 0xAA;

    for (i = 0; i < 2; i++)
    {
        if(-2 == S3C2416_SpareProgram(PageNo + i, Spare))
            Ret = -1;
    }

    free(Spare);

    return (Ret);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回: "0" -- 成功; "-1" -- 解析失败; "-2" -- 内存不足;
//备注:
//-----------------------------------------------------------------------------
static s32 S3C2416_GetNandDescr(struct NandDescr *Descr)
{
    u16 i;
    s32 Ret = 0;
    char *OnfiBuf;

    OnfiBuf = (char*)malloc(786);
    if (NULL == OnfiBuf)
        return (-1);

    memset(OnfiBuf, 0, 786);

    NandChipOn();

    SetNandCommand(PARAMETER_PAGE_READ_CMD_BYTE);

    NAND_CONTROLLER_REG->NFADDR = 0;

    WaitNandReady();// 时序要求

    for(i = 0; i < 786; i++)
        OnfiBuf[i] = GetNandData();

    NandChipOff();

//  PrintBuf(OnfiBuf, 786);//测试

    if(deonfi((const char*)OnfiBuf, Descr, 0))
        Ret = -1;

    free (OnfiBuf);

    return (Ret);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static void  S3C2416_NAND_ControllerConfig(void)
{
    //逻辑:
    //

    //nand config register
    NAND_CONTROLLER_REG->NFCONF = (0<<23)|// ECCType       [24:23]    0:1-bit ECC
                                  (1<<12)|// TACLS         [14:12]    CLE&ALE duration = HCLK*TACLS.
                                  (3<<8)| // TWRPH0        [10:8]     TWRPH0 duration = HCLK*(TWRPH0+1)
                                  (1<<4)| // TWRPH1        [6:4]      TWRPH1 duration = HCLK*(TWRPH1+1)
                                  (1<<3)| // AdvFlash(R)   [3]        Advanced NAND, 0:256/512, 1:1024/2048
                                  (0<<2)| // PageSize(R)   [2]        NAND memory page size
                                  (1<<1)| // AddrCycle(R)  [1]        NAND flash addr size
                                  (0<<0); // BusWidth(R/W) [0]        NAND bus width. 0:8-bit, 1:16-bit.

    //nand control register
    NAND_CONTROLLER_REG->NFCONT = (0<<13)| // Lock-tight   [13]    0:Disable lock, 1:Enable lock.
                                  (0<<12)| // Soft Lock    [12]    0:Disable lock, 1:Enable lock.
                                  (0<<10)| // EnablillegalAcINT[10] Illegal access interupt control.0:Disable,1:Enable
                                  (0<<9)|  // EnbRnBINT    [9] RnB interrupt. 0:Disable, 1:Enable
                                  (0<<8)|  // RnB_TrandMode[8] RnB transition detection config.0:Low->High,1:High->Low
                                  (1<<6)|  // SpareECCLock [6]     0:Unlock, 1:Lock
                                  (1<<5)|  // MainECCLock  [5]     0:Unlock, 1:Lock
                                  (1<<4)|  // InitECC(W)   [4]     1:Init ECC decoder/encoder.
                                  (1<<1)|  // Reg_nCE      [1]     0:nFCE=0, 1:nFCE=1.
                                  (1<<0);  // NANDC Enable [0]     operating mode. 0:Disable, 1:Enable.

    ResetNand();
}
//-----------------------------------------------------------------------------
// 功能：安装nand驱动
// 参数：  doformat -- 是否格式化；
// 返回：成功（0）；失败（-1）；
// 备注：
//-----------------------------------------------------------------------------
s32 ModuleInstall_NAND(u32 doformat)
{
    if(!s_ptNandInfo)
    {
        if(__nand_init())
        {
            printf("\r\n: erro : device : nand initialization failed(init).");
            return (-1);
        }
    }

    if(doformat)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __nand_req(format, 0 , -1, &sz);
    }

    if(!badstable)
    {
        badstable = nandbuildbads(__nand_req);
        if(!badstable)
        {
            printf("\r\n: erro : device : nand initialization failed(bad table).");
            return (-1);
        }
    }

    nand_umedia = malloc(sizeof(struct umedia)+s_ptNandInfo->BytesPerPage+s_ptNandInfo->OOB_Size);
    if(!nand_umedia)
        return (-1);

    nand_umedia->mreq = __nand_req;
    nand_umedia->type = nand;
    nand_umedia->ubuf = (u8*)um + sizeof(struct umedia);

    if(!Device_Create((const char*)NandFlashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)nand_umedia)))
    {
        printf("\r\n: erro : device : %s addition failed.", NandFlashName);
        free(nand_umedia);
        return (-1);
    }
    sNandflashInited = true;
    return (0);
}
// =============================================================================
// 功能：判断nandflash是否安装
// 参数：  无
// 返回：已成功安装（true）；未成功安装（false）；
// 备注：
// =============================================================================
bool_t Nandflash_is_install(void)
{
    return sNandflashInited;
}
/******************************************************************************
                         PRIVATE FUNCTION(本地私有函数)
******************************************************************************/
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static void ResetNand(void)
{
    NandChipOn();

    SetNandCommand(RESET_CMD_BYTE);

    WaitNandReady();

    NandChipOff();
}
//-----------------------------------------------------------------------------
//功能: 检查NAND操作是否出错
//参数:
//返回: 0 -- 正常; -1 -- 错误;
//备注: 未判断busy等状态
//-----------------------------------------------------------------------------
static s32 StatusOfNand(void)
{
    u8 Status = 0x1;

    do
    {
        SetNandCommand(STATUS_READ_CMD_BYTE);
        Status = GetNandData();
    }
    while(Status & 0x1);

    return(0);
}

//-----------------------------------------------------------------------------
//功能: Nand时序逻辑
//参数:
//返回:
//备注: 实际上是判断NAND芯片的R/Bn引脚，这种判断是必不可少的
//-----------------------------------------------------------------------------
static void WaitNandReady(void)
{
    while(0 == (NAND_CONTROLLER_REG->NFSTAT & 0x1));
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static void __WaitEccDone(void)
{
    ;//1-bit ECC 没有判断的位
}
#if 0
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
void RawTest(void)
{
    struct FlashChip *Chip;
    u32 i;
    u8 *Buf;
    u32 TestPage = 640;

    ModuleInstall_NAND("nand", 0, 0);//安装nand设备

    Chip = dListEntry(s_ptNandInfo, struct FlashChip, Descr);

    Buf = Chip->Buf;

    memset(Buf, 0x0, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Chip->Ops.ErsBlk(TestPage/s_ptNandInfo->PagesPerBlk);

    Chip->Ops.RdPage(TestPage, Buf, NO_ECC | SPARE_REQ);

    //PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    for(i = 0; i < s_ptNandInfo->BytesPerPage; i++)
        Buf[i] = (u8)i;

    Chip->Ops.WrPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    memset(Buf, 0xFF, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    //测试ECC功能,1位出错情况
    // 1次
    Buf[1] = 0;

    Chip->Ops.WrPage(TestPage, Buf, NO_ECC);

    memset(Buf, 0xFF, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
    // 2次
    TestPage++;
    Chip->Ops.WrPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    Buf[2] = 0;
    Chip->Ops.WrPage(TestPage, Buf, NO_ECC);

    memset(Buf, 0xFF, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    // 3次
    TestPage++;
    Chip->Ops.WrPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    Buf[3] = 1;
    Chip->Ops.WrPage(TestPage, Buf, NO_ECC);

    memset(Buf, 0xFF, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    //测试ECC功能,2位出错情况,是不能纠错了
    TestPage++;
    Chip->Ops.WrPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    Buf[3] = 0;
    Chip->Ops.WrPage(TestPage, Buf, NO_ECC | SPARE_REQ);

    memset(Buf, 0xFF, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
    while(1);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
void PageTest(const u32 PageNo, const u8 Init)
{
    volatile u8 *Buf;
    u32 i, Ret;
    struct FlashChip *Chip;
    u32 ECC_Flags = 0;

    if(Init)
    {
        if(ModuleInstall_NAND("nand", 0, 0)) //安装nand设备
            while(1);
    }

    if(HW_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= HW_ECC;

    if(SW_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= SW_ECC;

    if(NO_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= NO_ECC;

    Chip = dListEntry(s_ptNandInfo, struct FlashChip, Descr);

    Buf = Chip->Buf;

    for(i = 0; i < s_ptNandInfo->BytesPerPage; i++)
        Buf[i] = (u8)i;

    Chip->Ops.WrPage(PageNo, Buf, ECC_Flags | SPARE_REQ);

    memset(Buf, 0, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Ret = Chip->Ops.RdPage(PageNo, Buf, ECC_Flags | SPARE_REQ);
    for(i=0; i<(s_ptNandInfo->BytesPerPage); i++)
    {
        if(((u8)i) != Buf[i])
            while(1);
    }

    // ECC一位纠错
    Buf[1] = 0xF0;

    Chip->Ops.WrPage(PageNo, Buf, NO_ECC);

    memset(Buf, 0, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Ret = Chip->Ops.RdPage(PageNo, Buf, ECC_Flags | SPARE_REQ);
    for(i=0; i<(s_ptNandInfo->BytesPerPage); i++)
    {
        if(((u8)i) != Buf[i])
            while(1);
    }

}
void BadMarkFunctionCheck(struct FlashChip *Chip)
{
    u32 i;
    u32 Blks;

    if(NULL == Chip)
        while(1);

    Blks = Chip->Descr.Nand.BlksPerLUN * Chip->Descr.Nand.LUNs;

    for(i = 0; i < Blks; i++)
    {
        if(Chip->Ops.Special.Nand.MrkBad(i))
            while(1);
    }

    for(i = 0; i < Blks; i++)
    {
        if(-1 != Chip->Ops.Special.Nand.ChkBlk(i))
            while(1);
    }
}
void ContinuityTest(struct FlashChip *Chip)
{
    u32 i, j, Len;
    u32 Blks, Pages;
    s32 BadBlks = 0;
    u8 *Temp;

    if(NULL == Chip)
        while(1);

    Blks = Chip->Descr.Nand.BlksPerLUN * Chip->Descr.Nand.LUNs;

    for(i = Chip->Descr.Nand.ReservedBlks; i < Blks; i++)
    {
        if(0 != Chip->Ops.ErsBlk(i))
        {
            /* 擦除失败 */
//          TraceDrv(NAND_TRACE_ERROR, "block %d cannot be erased!\r\n", i);
            while(1);
        }
    }

    // 擦除后的校验
    Len = (Chip->Descr.Nand.BytesPerPage + Chip->Descr.Nand.OOB_Size);
    Temp = (u8*)malloc(Len);
    if(NULL == Temp)
        while(1);// 内存不足
    for(i = (Chip->Descr.Nand.ReservedBlks * Chip->Descr.Nand.PagesPerBlk); i < (Blks * Chip->Descr.Nand.PagesPerBlk); i++)
    {
        if(Len != Chip->Ops.RdPage(i, Temp, SPARE_REQ | NO_ECC))
            while(1);
        for(j = 0; j < Len; j++)
        {
            if(0xFF != Temp[j])
                while(1);
        }
    }
}

void ChipRawTest(void)
{
    struct FlashChip *Chip;
    u32 i, Ret;
    u8 *Buf;
    u32 TestPages;
    u32 TestBlocks;
    u32 ECC_Flags = 0;
    u32 ErrorCount = 0;

    if(ModuleInstall_NAND("nand", 0, 0))//安装nand设备
        while(1);

    if(HW_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= HW_ECC;

    if(SW_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= SW_ECC;

    if(NO_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= NO_ECC;

    Chip = dListEntry(s_ptNandInfo, struct FlashChip, Descr);

    Buf = Chip->Buf;

    for(TestBlocks = 0; TestBlocks < s_ptNandInfo->BlksPerLUN; TestBlocks++)
    {
        Chip->Ops.ErsBlk(TestBlocks);

        for(TestPages = 0; TestPages < s_ptNandInfo->PagesPerBlk; TestPages++)
        {
            u32 CurPage = TestPages + (TestBlocks * s_ptNandInfo->PagesPerBlk);
            memset(Buf, 0xAA, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
            Ret = Chip->Ops.RdPage(CurPage, Buf, NO_ECC | SPARE_REQ);
            for(i=0; i<(s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size); i++)
            {
                if(0xFF != Buf[i])
                {
                    ErrorCount += 1;
                    //PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
                    while(1);
                }
            }

            PageTest(CurPage, 0);
        }
    }


    for(TestBlocks = 0; TestBlocks < s_ptNandInfo->BlksPerLUN; TestBlocks++)
    {
        if(Chip->Ops.ErsBlk(TestBlocks))
            while(1);
    }


    BadMarkFunctionCheck(Chip);

    ContinuityTest(Chip);

    while(1);
}
#else

// ============================================================================
// 功能：nand 命令
// 参数：ucmd -- 命令；
//      其他 -- 命令参数；
// 返回：
// 备注：
// ============================================================================
s32 __nand_req(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case whichblock:
        {
            va_list list;
            u32 *block;
            s64 unit;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64)va_arg(list, u32);
            va_end(list);
            *block = unit / s_ptNandInfo->PagesPerBlk;
            break;
        }

        case totalblocks:
        {
            // args = &blocks
            *((u32*)args) =  s_ptNandInfo->BlksPerLUN * s_ptNandInfo->LUNs;
            break;
        }

        case blockunits:
        {
            // args = &units
            *((u32*)args) = s_ptNandInfo->PagesPerBlk;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = s_ptNandInfo->BytesPerPage;
            break;
        }

        case checkbad:
        {
            if(badslocation==(u32)args)
                res = 1;
            else
                res = S3C2416_BadChk((u32)args); // args = block

            break;
        }

        case savebads:
        {
            struct uopt opt = {0};
            opt.main = 1;
            if(0 != __nand_write(badslocation, (void*)args, opt)) // 坏块表放置在第0页
                res = -1;

            break;
        }

        case getbads:
        {
            struct uopt opt = {0};
            u32 **table = (u32**)args;

            opt.main = 1;
            if(!(*table))
            {
                res = 1;
            }
            else
            {
                res = __nand_read(badslocation, (void*)(*table), opt); // 坏块表放置在第0页
            }

            break;
        }

        case markbad:
        {
            res = S3C2416_BadMark((u32)args); // args = block
            break;
        }

        case format:
        {
            va_list list;
            s32 start, end;
            u8 *tmp, escape = 0;
            struct uesz *sz;
            struct uopt opt = {0};

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(!sz->block)
                return (-1);

            tmp = malloc(s_ptNandInfo->BytesPerPage);
            if(!tmp)
                return (-1);

            opt.main = 1;
            if(0 ==__nand_read(badslocation, (void*)(tmp), opt)) // 读坏块表
            {
                if(nandvalidbads((u32*)tmp))
                    escape = 1; // 存在坏块表，不擦除；

                if(-1 == (s32)end)
                    end = s_ptNandInfo->BlksPerLUN * s_ptNandInfo->LUNs;

                do
                {
                    if((badslocation == (--end)) && escape) // 坏块表在第一页
                        continue;

                    if(__nand_erase((s64)end, *sz))
                    {
                        res = -1;
                        break;
                    }
                }
                while(end!=start);
            }
            else
            {
                res = -1;
            }

            free(tmp);
            break;
        }

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
s32 __nand_read(s64 unit, void *data, struct uopt opt)
{
    u32 flags = 0;
    s32 res;
    nandbadfreeunit(badstable, &unit, __nand_req);
    if(opt.hecc)
        flags |= HW_ECC;
    else if(opt.secc)
        flags |= SW_ECC;
    else
        flags |= NO_ECC;

    if(opt.main)
    {
        if(opt.spare)
            flags |= SPARE_REQ;

        res = S3C2416_PageRead((u32)unit, (u8*)data, flags);
        if (!((SPARE_REQ & flags) || (HW_ECC & flags)))
        {
            if(res != (s32)s_ptNandInfo->BytesPerPage)
            {
                return (-1);
            }
        }
        else
        {
            if(res != (s32)(s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size))
            {
                return (-1);
            }
        }
    }
    else
    {
        res = S3C2416_SpareRead((u32)unit, (u8*)data);
        if(res != (s32)s_ptNandInfo->OOB_Size)
        {
            return (-1);
        }
    }

    return (0);
}

// ============================================================================
// 功能：nand 写；
// 参数：unit -- 写的序号（页）；
//      data -- 写的数据；
//      opt -- 写的方式；
// 返回：成功 -- （0）；失败 -- （-1）
// 返回：
// 备注：
// ============================================================================
s32 __nand_write(s64 unit, void *data, struct uopt opt)
{
    u32 flags = 0;
    s32 res;
    nandbadfreeunit(badstable, &unit, __nand_req);
    if(opt.hecc)
        flags |= HW_ECC;
    else if(opt.secc)
        flags |= SW_ECC;
    else
        flags |= NO_ECC;

    if(opt.main)
    {
        if(opt.spare)
            flags |= SPARE_REQ;

        res = S3C2416_PageProgram((u32)unit, (u8*)data, flags);
        if (!((SPARE_REQ & flags) || (HW_ECC & flags)))
        {
            if(res != (s32)(s_ptNandInfo->BytesPerPage))
            {
                return (-1);
            }
        }
        else
        {
            if(res != (s32)(s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size))
            {
                return (-1);
            }
        }
    }
    else
    {
        res = S3C2416_SpareProgram((u32)unit, (u8*)data);
        if(res != (s32)s_ptNandInfo->OOB_Size)
        {
            return (-1);
        }
    }

    return (0);
}

// ============================================================================
// 功能：nand 擦除
// 参数：unit -- 擦除的序号；
//      sz -- 擦除的单位（unit或block）
// 返回：成功 -- （0）；失败 -- （-1）
// 备注：
// ============================================================================
s32 __nand_erase(s64 unit, struct uesz sz)
{
    u32 block;

    if(sz.unit)
    {
        nandbadfreeunit(badstable, &unit, __nand_req);
        block = (u32)(unit / s_ptNandInfo->PagesPerBlk);
    }
    else
    {
        block = unit;
        nandbadfreeblock(badstable, &block, __nand_req);
    }

    return (S3C2416_BlockErase(block));
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __nand_init(void)
{
    s_ptNandInfo = malloc(sizeof(*s_ptNandInfo));
    if(!s_ptNandInfo)
        return (-1);

    S3C2416_NAND_ControllerConfig();// 芯片管脚等基本配置()
    if(S3C2416_GetNandDescr(s_ptNandInfo))
    {
        free(s_ptNandInfo);
        s_ptNandInfo = NULL;
        return (-1);
    }
    NandFlashLock = Lock_MutexCreate("Nand Flash Lock");        //创建nand的互斥锁
    s_ptNandInfo->ReservedBlks = 0;
    s_ptNandInfo->Controller = HW_ECC_SUPPORTED;
    s_ptNandInfo->BadMarkOffset = s_ptNandInfo->OOB_Size - 4 - 1;
    return (0);
}

// ============================================================================
// 功能：把nand和文件系统关联起来
// 参数：fs -- 需要挂载的文件系统，mediadrv -- 媒体驱动，
//       bstart -- 起始块，bend -- 结束块（不包括该块，只到该块的上一块）
// 返回：0 -- 成功， -1 -- 失败
// 备注：
// ============================================================================
s32 __nand_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv)
{
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res,BlockNum;
    targetobj = OBJ_MatchPath(fs, &notfind);
    if(notfind)
    {
        error_printf("nand"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj);
    super->MediaInfo = nand_umedia;
    super->MediaDrv = mediadrv;
    if(-1 == (s32)bend)
    {
        bend = s_ptNandInfo->BlksPerLUN * s_ptNandInfo->LUNs;
        BlockNum = bend - bstart;
    }
    else
    {
        BlockNum = bend - bstart;
    }
    super->AreaSize = s_ptNandInfo->BytesPerPage * s_ptNandInfo->PagesPerBlk * BlockNum;
    super->MediaStart = bstart*s_ptNandInfo->PagesPerBlk; // 起始unit号

    res = strlen(NandFlashName)+strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,NandFlashName); //获取该设备的全路径
    File_BeMedia(FullPath,fs); //往该设备挂载文件系统
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);
}

