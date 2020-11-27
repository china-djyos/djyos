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
#include <math.h>
#include "stm32f7xx_hal_conf.h"
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <device/unit_media.h>
#include <board.h>
#include <ecc_256.h>
//#include <libc/misc/ecc/ecc_256.h>


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


//R/B引脚的状态获取放到了对应板件的board.c中
//#define NAND_RB                ((GPIO_GetData(GPIO_D)&PIN6)>>6) //NAND Flash的闲/忙引脚
#define NAND_ADDRESS            0X80000000  //nand flash的访问地址,接NCE3,地址为:0X8000 0000
#define NAND_CMD                1<<16       //发送命令
#define NAND_ADDR               1<<17       //发送地址

//NAND FLASH状态
#define NSTA_READY              0X40        //nand已经准备好
#define NSTA_ERROR              0X01        //nand错误
#define NSTA_TIMEOUT            0X02        //超时

extern s32 deonfi(const char *data, struct NandDescr *onfi, u8 little);

//static u8 *s_pu8HammingCode;    // ECC校验结果
#define s_u8SizeofHammingCode   (4)
//互斥锁超时时间
#define NFlashLockTimeOut     CN_CFG_TICK_US * 1000 * 10

struct NandDescr *__nandescription; // NAND器件描述
static void ResetNand(void);
//static s32 StatusOfNand(void);
static bool_t WaitNandReady(void);
static u8 NAND_WaitForReady(void);

static NAND_HandleTypeDef NAND_Handler;    //NAND FLASH句柄
static struct MutexLCB *NandFlashLock;
struct umedia *nand_umedia;
static s32 gb_NandFlashReady=-3;

static const char *NandFlashName = "nand";      //该flash在obj在的名字

//新接口
static u32 *badstable = NULL;
static s32 badslocation = 0;
static bool_t sNandflashInited = false;
extern struct Object *s_ptDeviceRoot;
s32 __nand_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
static s32 __nand_init(void);

//-----------------------------------------------------------------------------
//功能:获取ECC的奇数位/偶数位个数
//参数:flag： 0/1  = 奇数位/偶数
//返回:计算后的ecc值
//-----------------------------------------------------------------------------
u16 Get_ECC_OE(u8 flag,u32 eccval)
{
    u8 i;
    u16 eccbitnum=0;
    for(i=0;i<24;i++)
    {
        if((i%2)==flag)
        {
            if((eccval>>i)&0X01)
                eccbitnum+=1<<(i>>1);
        }
    }
    return eccbitnum;
}
//-----------------------------------------------------------------------------
//功能:ECC纠错
//参数:bakecc 写在nandfalsh的ecc值
//     rdecc读出一页时计算的ECC值
//返回:HAMMING_ERROR_SINGLE_BIT;//1bit ECC错误
//     HAMMING_ERROR_ECC;//超过 1 bit ECC错误
//-----------------------------------------------------------------------------
u32  NAND_ECC_verify(u8* buf,u32 bakecc,u32 rdecc)
{
    u16 Rdo,Rde,Bako,Bake;
    u16 eccchk=0;
    u16 errorpos=0;
    u32 bytepos=0;
    Rdo=Get_ECC_OE(1,rdecc);    //获取rdecc的奇数位
    Rde=Get_ECC_OE(0,rdecc);    //获取rdecc的偶数位
    Bako=Get_ECC_OE(1,bakecc);  //获取bakecc的奇数位
    Bake=Get_ECC_OE(0,bakecc);  //获取bakecc的偶数位
    eccchk=Rdo^Rde^Bako^Bake;
    if(eccchk==0XFFF)
    {
        errorpos=Rdo^Bako;
        bytepos=errorpos/8;
        buf[bytepos]^=1<<(errorpos%8);
        return HAMMING_ERROR_SINGLE_BIT;//1bit ECC错误
    }
    return HAMMING_ERROR_ECC;//超过bit ECC错误

}

//-----------------------------------------------------------------------------
//功能:
//参数:
//输出: ">0" -- 写成功; "-2" -- 写失败;
//返回:
//-----------------------------------------------------------------------------
static s32 stm32f7_SpareProgram(u32 PageNo, const u8 *Data)
{
    u32 i;
    u32 SpareOffset = __nandescription->BytesPerPage;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=PAGE_PROGRAM_CMD_BYTE1;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)(SpareOffset&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((SpareOffset>>8)&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)PageNo;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((PageNo>>8)&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((PageNo>>16)&0xff);
    DJY_DelayUs(1);

    for(i = 0; i < __nandescription->OOB_Size; i++)
        *(vu8*)NAND_ADDRESS=*(vu8*)Data++;

    *(vu8*)(NAND_ADDRESS|NAND_CMD)=PAGE_PROGRAM_CMD_BYTE2; // 写入Main数据完成

    DJY_DelayUs(200);
    if(NAND_WaitForReady()!=NSTA_READY)
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }
    Lock_MutexPost(NandFlashLock);
    return (__nandescription->OOB_Size);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//输出: ">0" -- 读成功; "-2" -- 读失败; "-3" -- nand没准备好
//返回:
//-----------------------------------------------------------------------------
static s32 stm32f7_SpareRead(u32 PageNo, u8 *Data)
{
    u8 i,tolerate = 0;
    u32 SpareOffset = __nandescription->BytesPerPage;
    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }
again:

    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE1);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8) SpareOffset&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((SpareOffset>>8)&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)(PageNo&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((PageNo>>8&0xff)));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)(PageNo>>16&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE2);
    if(WaitNandReady() == false)//时序要求
    {
        if(tolerate < 3)
        {
            tolerate++;
            goto again;
        }
        Lock_MutexPost(NandFlashLock);
        return (-3);
    }

    for(i = 0; i < __nandescription->OOB_Size; i++)
        Data[i] = (*(vu8*)NAND_ADDRESS);

    if(NAND_WaitForReady()!=NSTA_READY)
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }
    Lock_MutexPost(NandFlashLock);
    return (__nandescription->OOB_Size);
}
static u8 NAND_ReadStatus(void)
{
    vu8 data=0;
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(STATUS_READ_CMD_BYTE);
    DJY_DelayUs(1);
    data=(*(vu8*)NAND_ADDRESS);
    return data;
}
static u8 NAND_WaitForReady(void)
{
    u8 status=0;
    vu32 time=0;
    while(1)
    {
        status=NAND_ReadStatus();
        if(status&NSTA_READY)break;
        time++;
        if(time>=0X1FFFF)return NSTA_TIMEOUT;
    }
    return NSTA_READY;
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
s32  stm32f7_PageProgram(u32 PageNo, u8 *Data, u32 Flags)
{

    u32 i, EccOffset,ECC_DATE;
    u8 *Spare;
    u32 *Buf = (u32 *)Data;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    Spare = (u8*)Data + __nandescription->BytesPerPage;// 注意：这里是基于驱动都有统一的缓冲块逻辑
    EccOffset = __nandescription->OOB_Size - s_u8SizeofHammingCode;// 4个字节(1-bit ECC校验)
    switch (Flags & MASK_ECC)
    {
        case HW_ECC :
            HAL_NAND_ECC_Enable(&NAND_Handler);
            break;
        case SW_ECC :
        case NO_ECC :
            HAL_NAND_ECC_Disable(&NAND_Handler);
            break;
        default :
            Lock_MutexPost(NandFlashLock);
            return (-1);
    }
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=PAGE_PROGRAM_CMD_BYTE1;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)0;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)(0);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)PageNo;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((PageNo>>8)&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((PageNo>>16)&0xff);
    DJY_DelayUs(1);

    for(i = 0; i < __nandescription->BytesPerPage >> 2; i++)
        *(vu32*)NAND_ADDRESS=*(vu32*)Buf++;

    if(HW_ECC & Flags)
    {
        while(!(NAND_Handler.Instance->SR & (1 << 6)));
        HAL_NAND_GetECC(&NAND_Handler,&ECC_DATE,1000);
        HAL_NAND_ECC_Disable(&NAND_Handler);
        *(u32*)(Spare + EccOffset)=ECC_DATE;
    }

    *(vu8*)(NAND_ADDRESS|NAND_CMD)=PAGE_PROGRAM_CMD_BYTE2;// 写入Main数据完成

    if(NAND_WaitForReady()!=NSTA_READY)
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }
    if (!((SPARE_REQ & Flags) || (HW_ECC & Flags)))
    {
        Lock_MutexPost(NandFlashLock);
        return (__nandescription->BytesPerPage);// 只写页,结束退出
    }

    if(!(SPARE_REQ & Flags))
        memset(Spare, 0xFF, EccOffset);// 未要求写spare,则默认写0xFF

    if(-2 == stm32f7_SpareProgram(PageNo, Spare))
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }
    Lock_MutexPost(NandFlashLock);
    return (__nandescription->BytesPerPage + __nandescription->OOB_Size);

}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回: ">0" -- 正常，读出的字节数。
//      "-1" -- 输入参数错误;
//      "-2" -- 读失败;
//      "-3" -- ECC纠错失败;
//      "-4" -- nand没准备好
//备注: 不管读写是否正确，都将数据回传
//-----------------------------------------------------------------------------
s32  stm32f7_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    //逻辑:
    //
    u32 i,tolerate = 0;
    u8 *Spare;
    u32 EccRet, EccOffset;
    u32 ECCval;
    u32 *Buf = (u32 *)Data;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }
    switch (Flags & MASK_ECC)
    {
        case HW_ECC :
            HAL_NAND_ECC_Enable(&NAND_Handler);
            break;
        case SW_ECC :
        case NO_ECC :
            HAL_NAND_ECC_Disable(&NAND_Handler);
            break;
        default :
            Lock_MutexPost(NandFlashLock);
            return (-1);
    }
    u32 SpareOffset=0;
again:
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE1);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8) SpareOffset&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((SpareOffset>>8)&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)(PageNo&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((PageNo>>8&0xff)));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)(PageNo>>16&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE2);
    if(WaitNandReady() == false)// 时序要求
    {
        if(tolerate < 3)
        {
            tolerate++;
            goto again;
        }
        Lock_MutexPost(NandFlashLock);
        return (-4);
    }

    for(i = 0; i < __nandescription->BytesPerPage >> 2; i++)
        *(vu32*)Buf++ = (*(vu32*)NAND_ADDRESS);


    if(NAND_WaitForReady()!=NSTA_READY)
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }
    if (!((SPARE_REQ & Flags) || (HW_ECC & Flags)))
    {
        Lock_MutexPost(NandFlashLock);
        return (__nandescription->BytesPerPage);// 只读页,结束退出
    }
    Spare = Data + __nandescription->BytesPerPage;// 注意：这里是基于驱动都有统一的缓冲块逻辑
    EccOffset = __nandescription->BytesPerPage + __nandescription->OOB_Size - s_u8SizeofHammingCode;
    if(HW_ECC & Flags)
    {
       while(!(NAND_Handler.Instance->SR & (1 << 6)));
       HAL_NAND_GetECC(&NAND_Handler,&ECCval,1000);
       HAL_NAND_ECC_Disable(&NAND_Handler);
    }
    if(-2 == stm32f7_SpareRead(PageNo, Spare))
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }
    if((HW_ECC & Flags))
    {
        for(i=0;i<s_u8SizeofHammingCode;i++)
        {
            if((ECCval>>(8*i)&0xff) != Data[EccOffset+i])//判断效验码是否正确
            {
                for(i=0;i<s_u8SizeofHammingCode;i++)//判断是否为擦除的芯片
                {
                   if( Data[EccOffset+i]!=0xff)
                   {    //效验码错误并且不是刚擦除的芯片采用软件效验的方法纠错

                       EccRet = NAND_ECC_verify(Data,ECCval,
                                   (u32)(Data[EccOffset]|\
                                   Data[EccOffset+1]<<(1*8)|\
                                   Data[EccOffset+2]<<(2*8)|\
                                   Data[EccOffset+3]<<(3*8)));//(u32)*(&Data[EccOffset]));

                       if (EccRet && (EccRet != HAMMING_ERROR_SINGLE_BIT))
                       {
                           debug_printf("nand driver", "cannot be fixed");
                           Lock_MutexPost(NandFlashLock);
                           return (-3);
                       }
                       break;
                   }
                }
            }
            break;
        }
    }
    Lock_MutexPost(NandFlashLock);
    return (__nandescription->BytesPerPage + __nandescription->OOB_Size);
}

//-----------------------------------------------------------------------------
//功能:擦除快
//参数:块号
//返回: "0" -- 成功;"-1" -- 失败;
//备注:
//-----------------------------------------------------------------------------
s32 stm32f7_BlockErase(u32 BlkNo)
{

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-1);
    }
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(BLOCK_ERASE_CMD_BYTE1);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((BlkNo << 6)&0xff));// 3个地址周期表示的是页号
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((BlkNo >> 2)&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((BlkNo >> 10)&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(BLOCK_ERASE_CMD_BYTE2);

    DJY_EventDelay( 2*mS );
    if(NAND_WaitForReady()!=NSTA_READY)
    {
        Lock_MutexPost(NandFlashLock);
        return (-1);    //成功
    }
    Lock_MutexPost(NandFlashLock);
    return (0);    //成功
}

//-----------------------------------------------------------------------------
//功能:检查快的好坏
//参数:块好
//返回: "0" -- 好块;
//      "-1" -- 好块;
//      "-2" -- 内存不足
//      "-3" -- 读失败;
//备注:
//-----------------------------------------------------------------------------
static s32 stm32f7_BadChk(u32 BlkNo)
{
    u8 *Spare, i;
    s32 Ret = 0,ReadState = 0;
    u32 PageNo = BlkNo * __nandescription->PagesPerBlk;

    Spare = malloc (__nandescription->OOB_Size);
    if (NULL == Spare)
        return (-2);

    for (i = 0; i < 2; i++)// 只判断每块的首两页
    {
        ReadState = stm32f7_SpareRead(PageNo + i, Spare);
        if((ReadState == -2) || (ReadState == -3))
        {
            Ret = -3;
            break;
        }
        if ((0xAA == Spare[__nandescription->BadMarkOffset]) &&
            (0xAA == Spare[__nandescription->BadMarkOffset-1]))
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
static s32  stm32f7_BadMark(u32 BlkNo)
{
    //逻辑:
    //    不管BAD MARK位原来是多少以及是否有ECC校验问题, 只管将该位标志为坏块

    u8 *Spare, i;
    s32 Ret = 0;
    u32 PageNo = BlkNo * __nandescription->PagesPerBlk;

    Spare = malloc (__nandescription->OOB_Size);
    if (NULL == Spare)
        return (-2);
    memset(Spare, 0xFF, __nandescription->OOB_Size);
    Spare[__nandescription->BadMarkOffset] = 0xAA;
    Spare[__nandescription->BadMarkOffset-1] = 0xAA;

    for (i = 0; i < 2; i++)
    {
        if(-2 == stm32f7_SpareProgram(PageNo + i, Spare))
            Ret = -1;
    }
    free(Spare);
    return (Ret);
}
//-----------------------------------------------------------------------------
//功能:获取nand flash 的内部信息
//参数:
//返回: "0" -- 成功; "-1" -- 解析失败; "-2" -- 内存不足;
//备注:
//-----------------------------------------------------------------------------
static s32 stm32f7_GetNandDescr(struct NandDescr *Descr)
{
    u16 i;
    s32 Ret = 0;
    char *OnfiBuf;

    OnfiBuf = (char*)malloc(786);
    if (NULL == OnfiBuf)
        return (-1);

    memset(OnfiBuf, 0, 786);
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PARAMETER_PAGE_READ_CMD_BYTE);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(0);
    DJY_DelayUs(26);// 时序要求

    for(i = 0; i < 786; i++)
        OnfiBuf[i] = (*(vu8*)NAND_ADDRESS);

    if(deonfi((const char*)OnfiBuf, Descr, 0))
        Ret = -1;

    //此处修改Descr以保留NANDFLASH最后1M作为异常信息记录存储介质,存储介质为256M NANDFLASH
    //2个LUN，1个Lun有1024Blocks(128M),每个Block有64page(128k),每个Page有(2k+64)bytes。
    //异常信息存储使用NANDFLASH最后1M空间，使用了8个Block,512个Pages。
    Descr->BlksPerLUN -= 8;

    free (OnfiBuf);
    return (Ret);
}

static bool_t NAND_Reset(void)
{
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(RESET_CMD_BYTE);  //复位NAND
    if(NAND_WaitForReady()==NSTA_READY)
        return true;//复位成功
    else
        return false;                              //复位失败
}

static bool_t NAND_ModeSet(u8 mode)
{
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(RESET_CMD_BYTE);//发送设置特性命令
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(0X01);       //地址为0X01,设置mode
    *(vu8*)NAND_ADDRESS=mode;                   //P1参数,设置mode
    *(vu8*)NAND_ADDRESS=0;
    *(vu8*)NAND_ADDRESS=0;
    *(vu8*)NAND_ADDRESS=0;
    if(NAND_WaitForReady()==NSTA_READY)
        return true;//成功
    else
        return false;                              //失败
}

u32 NAND_ReadID(void)
{
    u8 deviceid[5];
    u32 id;
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(ID_READ_CMD_BYTE); //发送读取ID命令
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(0X00);
    //ID一共有5个字节
    deviceid[0]=(*(vu8*)NAND_ADDRESS);
    deviceid[1]=(*(vu8*)NAND_ADDRESS);
    deviceid[2]=(*(vu8*)NAND_ADDRESS);
    deviceid[3]=(*(vu8*)NAND_ADDRESS);
    deviceid[4]=(*(vu8*)NAND_ADDRESS);
    //镁光的NAND FLASH的ID一共5个字节，但是为了方便我们只取4个字节组成一个32位的ID值
    //根据NAND FLASH的数据手册，只要是镁光的NAND FLASH，那么一个字节ID的第一个字节都是0X2C
    //所以我们就可以抛弃这个0X2C，只取后面四字节的ID值。
    id=((u32)deviceid[1])<<24|((u32)deviceid[2])<<16|((u32)deviceid[3])<<8|deviceid[4];
    return id;
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static bool_t  stm32f7_NAND_ControllerConfig(void)
{
    FMC_NAND_PCC_TimingTypeDef ComSpaceTiming,AttSpaceTiming;

    NAND_Handler.State=HAL_NAND_STATE_RESET;
    NAND_Handler.Instance=FMC_Bank3;
    NAND_Handler.Init.NandBank=FMC_NAND_BANK3;                      //NAND挂在BANK3上
    NAND_Handler.Init.Waitfeature=FMC_NAND_PCC_WAIT_FEATURE_DISABLE;//关闭等待特性
    NAND_Handler.Init.MemoryDataWidth=FMC_NAND_PCC_MEM_BUS_WIDTH_8; //8位数据宽度
    NAND_Handler.Init.EccComputation=FMC_NAND_ECC_ENABLE;           //禁止ECC
    NAND_Handler.Init.ECCPageSize=FMC_NAND_ECC_PAGE_SIZE_2048BYTE;  //ECC页大小为2048字节
    NAND_Handler.Init.TCLRSetupTime=7;//设置TCLR(tCLR=CLE到RE的延时)=(TCLR+TSET+2)*THCLK,THCLK=1/180M=4.6ns
    NAND_Handler.Init.TARSetupTime=7; //设置TAR(tAR=ALE到RE的延时)=(TAR+TSET+1)*THCLK,THCLK=1/180M=4.6n。

    ComSpaceTiming.SetupTime=10;         //建立时间
    ComSpaceTiming.WaitSetupTime=10;    //等待时间
    ComSpaceTiming.HoldSetupTime=10;    //保持时间
    ComSpaceTiming.HiZSetupTime=10;     //高阻态时间

    AttSpaceTiming.SetupTime=10;         //建立时间
    AttSpaceTiming.WaitSetupTime=10;    //等待时间
    AttSpaceTiming.HoldSetupTime=10;    //保持时间
    AttSpaceTiming.HiZSetupTime=10;     //高阻态时间

    HAL_NAND_Init(&NAND_Handler,&ComSpaceTiming,&AttSpaceTiming);
    NAND_Reset();                       //复位NAND
    DJY_DelayUs(100*mS);
    NAND_ModeSet(4);                    //设置为MODE4,高速模式
    ResetNand();
    return true;
}

//void HAL_NAND_MspInit(NAND_HandleTypeDef *hnand)
//{
//    __HAL_RCC_FMC_CLK_ENABLE();             //使能FMC时钟
//
//}
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
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(RESET_CMD_BYTE);
//    while(WaitNandReady() == false);
}
#if 0
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
        *(vu8*)(NAND_ADDRESS|NAND_CMD)=(STATUS_READ_CMD_BYTE);
        Status = (*(vu8*)NAND_ADDRESS);
    }
    while(Status & 0x1);

    return(0);
}
#endif
//-----------------------------------------------------------------------------
//功能: 等待RB引脚为某个电瓶
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
extern unsigned char  NAND_RB_Get(void);
static u8 NAND_WaitRB(vu8 rb)
{
    vu32 time=0;
    while(time<30000)
    {
        if(NAND_RB_Get() == rb)return 0;    //NAND_RB_Get在板件中的board.c中实现，用于获取RG引脚的值。
        time++;
    }
    return 1;
}
//-----------------------------------------------------------------------------
//功能: Nand时序逻辑
//参数:
//返回:
//备注: 实际上是判断NAND芯片的R/Bn引脚，这种判断是必不可少的
//-----------------------------------------------------------------------------
static bool_t WaitNandReady(void)
{

    if(NAND_WaitRB(0))
        return false;
    if(NAND_WaitRB(1))
        return false;
    return true;

}
//-----------------------------------------------------------------------------
// 功能：安装nand驱动
// 参数：  doformat -- 是擦除芯片化；
// 返回：成功（0）；失败（-1）；
// 备注：
//-----------------------------------------------------------------------------
s32 ModuleInstall_NAND(u32 doformat)
{
    if(!__nandescription)
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
    nand_umedia = malloc(sizeof(struct umedia)+__nandescription->BytesPerPage+__nandescription->OOB_Size);
    memset(nand_umedia,0,sizeof(struct umedia));
    if(!nand_umedia)
        return (-1);

    nand_umedia->mreq = __nand_req;
    nand_umedia->type = nand;
    nand_umedia->ubuf = (u8*)nand_umedia + sizeof(struct umedia);

    if(!Device_Create((const char*)NandFlashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)nand_umedia)))
    {
        printf("\r\n: erro : device : %s addition failed.", NandFlashName);
        free(nand_umedia);
        return (-1);
    }

    sNandflashInited = true;
    return 0;
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

bool_t NandFlash_Ready(void)
{
    if(gb_NandFlashReady)
        return false;
    else
        return true;
}

#if (0)
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

    Chip = dListEntry(__nandescription, struct FlashChip, Descr);

    Buf = Chip->Buf;

    memset(Buf, 0x0, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    Chip->Ops.ErsBlk(TestPage/__nandescription->PagesPerBlk);

    Chip->Ops.RdPage(TestPage, Buf, NO_ECC | SPARE_REQ);

    //PrintBuf(Buf, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    for(i = 0; i < __nandescription->BytesPerPage; i++)
        Buf[i] = (u8)i;

    Chip->Ops.WrPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    memset(Buf, 0xFF, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    //测试ECC功能,1位出错情况
    // 1次
    Buf[1] = 0;

    Chip->Ops.WrPage(TestPage, Buf, NO_ECC);

    memset(Buf, 0xFF, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (__nandescription->BytesPerPage + __nandescription->OOB_Size));
    // 2次
    TestPage++;
    Chip->Ops.WrPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    Buf[2] = 0;
    Chip->Ops.WrPage(TestPage, Buf, NO_ECC);

    memset(Buf, 0xFF, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    // 3次
    TestPage++;
    Chip->Ops.WrPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    Buf[3] = 1;
    Chip->Ops.WrPage(TestPage, Buf, NO_ECC);

    memset(Buf, 0xFF, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    //测试ECC功能,2位出错情况,是不能纠错了
    TestPage++;
    Chip->Ops.WrPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    Buf[3] = 0;
    Chip->Ops.WrPage(TestPage, Buf, NO_ECC | SPARE_REQ);

    memset(Buf, 0xFF, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (__nandescription->BytesPerPage + __nandescription->OOB_Size));
    while(1);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
void PageTest(const u32 PageNo, const u8 Init,const u32 BlkNo )
{
    volatile u8 *Buf;
    u32 i,j;
    struct FlashChip *Chip;
    u32 ECC_Flags = 0;

    if(Init)
    {
        if(ModuleInstall_NAND("nand", 0, 0)) //安装nand设备
            while(1);
    }

    if(HW_ECC_SUPPORTED & __nandescription->Controller)
        ECC_Flags |= HW_ECC;

    if(SW_ECC_SUPPORTED & __nandescription->Controller)
        ECC_Flags |= SW_ECC;

    if(NO_ECC_SUPPORTED & __nandescription->Controller)
        ECC_Flags |= NO_ECC;

    Chip = dListEntry(__nandescription, struct FlashChip, Descr);

    Buf = Chip->Buf;

    for(i = 0; i < __nandescription->BytesPerPage; i++)
        Buf[i] = (u8)i;

    Chip->Ops.WrPage(PageNo, Buf, HW_ECC | SPARE_REQ);

    memset(Buf, 0, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    Chip->Ops.RdPage(PageNo, Buf, HW_ECC | SPARE_REQ);
    for(i=0; i<(__nandescription->BytesPerPage); i++)
    {
        if(((u8)i) != Buf[i])
            while(1);
    }
    // ECC一位纠错  todo
    Buf[1] = 0x0;
    Chip->Ops.WrPage(PageNo, Buf, NO_ECC | SPARE_REQ);

    memset(Buf, 0, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    Chip->Ops.RdPage(PageNo, Buf, HW_ECC | SPARE_REQ);
    for(i=0; i<(__nandescription->BytesPerPage); i++)
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
    u32 Blks;
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
    u32 i;
    u8 *Buf;
    u32 TestPages;
    u32 TestBlocks;
    u32 ECC_Flags = 0;
    u32 ErrorCount = 0;

    if(ModuleInstall_NAND("nand", 0, 0))//安装nand设备
        while(1);

    if(HW_ECC_SUPPORTED & __nandescription->Controller)
        ECC_Flags |= HW_ECC;

    if(SW_ECC_SUPPORTED & __nandescription->Controller)
        ECC_Flags |= SW_ECC;

    if(NO_ECC_SUPPORTED & __nandescription->Controller)
        ECC_Flags |= NO_ECC;

    Chip = dListEntry(__nandescription, struct FlashChip, Descr);

    Buf = Chip->Buf;

    for(TestBlocks = 0; TestBlocks < __nandescription->BlksPerLUN; TestBlocks++)
    {
        Chip->Ops.ErsBlk(TestBlocks);
        printf("test block :%d   \r",TestBlocks);
        for(TestPages = 0; TestPages < __nandescription->PagesPerBlk; TestPages++)
        {
            u32 CurPage = TestPages + (TestBlocks * __nandescription->PagesPerBlk);
            memset(Buf, 0xAA, (__nandescription->BytesPerPage + __nandescription->OOB_Size));
            Chip->Ops.RdPage(CurPage, Buf, NO_ECC | SPARE_REQ);
            for(i=0; i<(__nandescription->BytesPerPage + __nandescription->OOB_Size); i++)
            {
                if(0xFF != Buf[i])
                {
                    ErrorCount += 1;
                    //PrintBuf(Buf, (__nandescription->BytesPerPage + __nandescription->OOB_Size));
                    while(1);
                }
            }

            PageTest(CurPage, 0,TestBlocks);
        }
    }
    for(TestBlocks = 0; TestBlocks < __nandescription->BlksPerLUN; TestBlocks++)
    {
        if(Chip->Ops.ErsBlk(TestBlocks))
            while(1);
    }
    BadMarkFunctionCheck(Chip);
    ContinuityTest(Chip);
    while(1);
}
#endif

#if 1 // 新接口

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
            *block = unit / __nandescription->PagesPerBlk;
            break;
        }

        case totalblocks:
        {
            // args = &blocks
            *((u32*)args) =  __nandescription->BlksPerLUN * __nandescription->LUNs;
            break;
        }

        case blockunits:
        {
            // args = &units
            *((u32*)args) = __nandescription->PagesPerBlk;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = __nandescription->BytesPerPage;
            break;
        }
        case sparebytes:
        {
            *((u32*)args) = __nandescription->OOB_Size;
            break;
        }
        case checkbad:
        {
            if(badslocation == (s32)args)
                res = 1;
            else
                res = stm32f7_BadChk((u32)args); // args = block

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
            res = stm32f7_BadMark((u32)args); // args = block
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

            tmp = malloc(__nandescription->BytesPerPage);
            if(!tmp)
                return (-1);

            opt.main = 1;
            if(0 ==__nand_read(badslocation, (void*)(tmp), opt)) // 读坏块表
            {
                if(nandvalidbads((u32*)tmp))
                    escape = 1; // 存在坏块表，不擦除；

                if(-1 == end)
                    end = __nandescription->BlksPerLUN * __nandescription->LUNs;

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

        res = stm32f7_PageRead((u32)unit, (u8*)data, flags);
        if (!((SPARE_REQ & flags) || (HW_ECC & flags)))
        {
            if(res != (s32)__nandescription->BytesPerPage)
            {
                return (-1);
            }
        }
        else
        {
            if(res != (s32)(__nandescription->BytesPerPage + __nandescription->OOB_Size))
            {
                return (-1);
            }
        }
    }
    else
    {
        res = stm32f7_SpareRead((u32)unit, (u8*)data);
        if(res != (s32)__nandescription->OOB_Size)
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

        res = stm32f7_PageProgram((u32)unit, (u8*)data, flags);
        if (!((SPARE_REQ & flags) || (HW_ECC & flags)))
        {
            if(res != (s32)(__nandescription->BytesPerPage))
            {
                return (-1);
            }
        }
        else
        {
            if(res != (s32)(__nandescription->BytesPerPage + __nandescription->OOB_Size))
            {
                return (-1);
            }
        }
    }
    else
    {
        res = stm32f7_SpareProgram((u32)unit, (u8*)data);
        if(res != (s32)__nandescription->OOB_Size)
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
        block = (u32)(unit / __nandescription->PagesPerBlk);
    }
    else
    {
        block = unit;
        nandbadfreeblock(badstable, &block, __nand_req);
    }

    return (stm32f7_BlockErase(block));
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __nand_init(void)
{
    __nandescription = malloc(sizeof(*__nandescription));
    if(!__nandescription)
        return (-1);

    stm32f7_NAND_ControllerConfig();// 芯片管脚等基本配置
    if(stm32f7_GetNandDescr(__nandescription))
    {
        free(__nandescription);
        __nandescription = NULL;
        return (-1);
    }
    NandFlashLock = Lock_MutexCreate("Nand Flash Lock");        //创建nand的互斥锁
    __nandescription->ReservedBlks = 0;
    __nandescription->Controller = HW_ECC_SUPPORTED;
    __nandescription->BadMarkOffset = __nandescription->OOB_Size - 4 - 1;
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

    if(mediadrv == NULL)
        return -1;
    targetobj = OBJ_MatchPath(fs, &notfind);
    if(notfind)
    {
        error_printf("nand"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj);
    super->MediaInfo = nand_umedia;             //把nand的信息放到文件系统的核心数据结构当中
    super->MediaDrv = mediadrv;

    if(-1 == (s32)bend)
    {
        bend = __nandescription->BlksPerLUN * __nandescription->LUNs;  //计算文件系统所有区域的结束块
        BlockNum = bend - bstart;       //计算一共有多少块
    }
    else
    {
        BlockNum = bend - bstart;
    }
    super->AreaSize = __nandescription->BytesPerPage * __nandescription->PagesPerBlk * BlockNum;        //计算文件系统所用区域一共有多少字节
    super->MediaStart = bstart*__nandescription->PagesPerBlk; // 起始unit号

    res = strlen(NandFlashName)+strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,NandFlashName); //获取该设备的全路径
    File_BeMedia(FullPath,fs); //往该设备挂载文件系统
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);
}

#endif
