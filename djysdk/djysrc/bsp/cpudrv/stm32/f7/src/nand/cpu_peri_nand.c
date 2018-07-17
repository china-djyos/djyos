//-----------------------------------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
#include <device/flash/flash.h>
#include <cpu_peri.h>
#include <djyos.h>
#include "stm32f7xx_hal_conf.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面

//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern s32 ModuleInstall_NAND(const char *ChipName, u32 Flags, u16 StartBlk);
//  ModuleInstall_NAND(CFG_NAND_CHIP_NAME,CFG_NAND_INIT_FLAG,CFG_NAND_STARTBLK );
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"nand"          //填写该组件的名字
//parent:"devfile"                //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp组件                                            //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:可选                //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//grade:init                    //初始化时机，可选值：none，init，main。none表示无须初始化，
                                //init表示在调用main之前，main表示在main函数中初始化
//dependence:"cpu_peri_gpio","devfile","djyfs"     //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#ifndef CFG_NAND_CHIP_NAME   //****检查参数是否已经配置好
#warning    NAND组件参数未配置，使用默认值
//%$#@num,0,100,
#define CFG_NAND_STARTBLK       0       //"保留的块数",从第0块开始计算
#define CFG_NAND_INIT_FLAG      0       //"初始化标志",为2表示缓存，为1表示擦除flash
//%$#@enum,true,false,
//%$#@string,1,32,
#define CFG_NAND_CHIP_NAME      "nand"  //"芯片名字",
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
//@#$%component end configure

#define NAND_RB                ((GPIO_GetData(GPIO_D)&PIN6)>>6) //NAND Flash的闲/忙引脚
#define NAND_ADDRESS            0X80000000  //nand flash的访问地址,接NCE3,地址为:0X8000 0000
#define NAND_CMD                1<<16       //发送命令
#define NAND_ADDR               1<<17       //发送地址

//NAND FLASH状态
#define NSTA_READY              0X40        //nand已经准备好
#define NSTA_ERROR              0X01        //nand错误
#define NSTA_TIMEOUT            0X02        //超时


//static u8 *s_pu8HammingCode;    // ECC校验结果
#define s_u8SizeofHammingCode   (4)

struct NandDescr *s_ptNandInfo;
static void ResetNand(void);
static s32 StatusOfNand(void);
static bool_t WaitNandReady(void);

static NAND_HandleTypeDef NAND_Handler;    //NAND FLASH句柄

static s32 gb_NandFlashReady=-3;


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
    s32 Ret;
    u32 SpareOffset = s_ptNandInfo->BytesPerPage;


    *(vu8*)(NAND_ADDRESS|NAND_CMD)=PAGE_PROGRAM_CMD_BYTE1;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)(SpareOffset&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((SpareOffset>>8)&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)PageNo;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((PageNo>>8)&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((PageNo>>16)&0xff);
    Djy_DelayUs(1);

    for(i = 0; i < s_ptNandInfo->OOB_Size; i++)
        *(vu8*)NAND_ADDRESS=*(vu8*)Data++;

    *(vu8*)(NAND_ADDRESS|NAND_CMD)=PAGE_PROGRAM_CMD_BYTE2; // 写入Main数据完成
    WaitNandReady();//时序要求
    Ret = StatusOfNand();
    if(Ret)
        return (-2);

    return (s_ptNandInfo->OOB_Size);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//输出: ">0" -- 读成功; "-2" -- 写失败;
//返回:
//-----------------------------------------------------------------------------
static s32 stm32f7_SpareRead(u32 PageNo, u8 *Data)
{
    u8 i;
    s32 Ret;
    u32 SpareOffset = s_ptNandInfo->BytesPerPage;

    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE1);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8) SpareOffset&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((SpareOffset>>8)&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)(PageNo&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((PageNo>>8&0xff)));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)(PageNo>>16&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE2);
    WaitNandReady();// 时序要求

    for(i = 0; i < s_ptNandInfo->OOB_Size; i++)
        Data[i] = (*(vu8*)NAND_ADDRESS);

    Ret = StatusOfNand();
    if(Ret)
        return (-2);

    return (s_ptNandInfo->OOB_Size);
}
static u8 NAND_ReadStatus(void)
{
    vu8 data=0;
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(STATUS_READ_CMD_BYTE);
    Djy_DelayUs(1);
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
    s32 Ret;
    u8 *Spare;
    Spare = (u8*)Data + s_ptNandInfo->BytesPerPage;// 注意：这里是基于驱动都有统一的缓冲块逻辑
    EccOffset = s_ptNandInfo->OOB_Size - s_u8SizeofHammingCode;// 4个字节(1-bit ECC校验)
    switch (Flags & MASK_ECC)
    {
        case HW_ECC :
            HAL_NAND_ECC_Enable(&NAND_Handler);
            break;
        case SW_ECC :
        case NO_ECC :
            HAL_NAND_ECC_Disable(&NAND_Handler);
            break;
        default :return (-1);
    }
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=PAGE_PROGRAM_CMD_BYTE1;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)0;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)(0);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)PageNo;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((PageNo>>8)&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((PageNo>>16)&0xff);
    Djy_DelayUs(1);

    for(i = 0; i < s_ptNandInfo->BytesPerPage; i++)
        *(vu8*)NAND_ADDRESS=*(vu8*)Data++;

    if(HW_ECC & Flags)
    {
        HAL_NAND_GetECC(&NAND_Handler,&ECC_DATE,1000);
        HAL_NAND_ECC_Disable(&NAND_Handler);
        *(u32*)(Spare + EccOffset)=ECC_DATE;
    }

    *(vu8*)(NAND_ADDRESS|NAND_CMD)=PAGE_PROGRAM_CMD_BYTE2;// 写入Main数据完成
    WaitNandReady();// 时序要求

    Ret = StatusOfNand();
    if(Ret)
        return (-2);

    if (!((SPARE_REQ & Flags) || (HW_ECC & Flags)))
        return (s_ptNandInfo->BytesPerPage);// 只写页,结束退出

    if(!(SPARE_REQ & Flags))
        memset(Spare, 0xFF, EccOffset);// 未要求写spare,则默认写0xFF

    if(-2 == stm32f7_SpareProgram(PageNo, Spare))
        return (-2);

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
s32  stm32f7_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    //逻辑:
    //
    u32 i;
    s32 Ret;
    u8 *Spare;
    u32 EccRet, EccOffset;
    u32 ECCval;

    switch (Flags & MASK_ECC)
    {
        case HW_ECC :
            HAL_NAND_ECC_Enable(&NAND_Handler);
            break;
        case SW_ECC :
        case NO_ECC :
            HAL_NAND_ECC_Disable(&NAND_Handler);
            break;
        default : return (-1);
    }
    u32 SpareOffset=0;
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE1);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8) SpareOffset&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((SpareOffset>>8)&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)(PageNo&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((PageNo>>8&0xff)));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)(PageNo>>16&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE2);
    WaitNandReady();// 时序要求

    for(i = 0; i < s_ptNandInfo->BytesPerPage; i++)
        Data[i] = (*(vu8*)NAND_ADDRESS);

    Ret = StatusOfNand();
    if(Ret)
        return (-2);

    if (!((SPARE_REQ & Flags) || (HW_ECC & Flags)))
        return (s_ptNandInfo->BytesPerPage);// 只读页,结束退出

    Spare = Data + s_ptNandInfo->BytesPerPage;// 注意：这里是基于驱动都有统一的缓冲块逻辑
    EccOffset = s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size - s_u8SizeofHammingCode;
    if(HW_ECC & Flags)
    {
       HAL_NAND_GetECC(&NAND_Handler,&ECCval,1000);
       HAL_NAND_ECC_Disable(&NAND_Handler);
    }
    if(-2 == stm32f7_SpareRead(PageNo, Spare))
        return (-2);

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
                           TraceDrv(FLASH_TRACE_DEBUG, "cannot be fixed");
                           return (-3);
                       }
                       break;
                   }
                }
            }
            break;
        }
    }
    return (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size);
}

//-----------------------------------------------------------------------------
//功能:擦除快
//参数:块号
//返回: "0" -- 成功;"-1" -- 失败;
//备注:
//-----------------------------------------------------------------------------
s32 stm32f7_BlockErase(u32 BlkNo)
{
    s32 Ret;
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(BLOCK_ERASE_CMD_BYTE1);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((BlkNo << 6)&0xff));// 3个地址周期表示的是页号
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((BlkNo >> 2)&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((BlkNo >> 10)&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(BLOCK_ERASE_CMD_BYTE2);
    WaitNandReady();
    Ret = StatusOfNand();
    return (Ret);
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
    s32 Ret = 0;
    u32 PageNo = BlkNo * s_ptNandInfo->PagesPerBlk;

    Spare = malloc (s_ptNandInfo->OOB_Size);
    if (NULL == Spare)
        return (-2);

    for (i = 0; i < 2; i++)// 只判断每块的首两页
    {
        if(-2 == stm32f7_SpareRead(PageNo + i, Spare))
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
static s32  stm32f7_BadMark(u32 BlkNo)
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
    Djy_DelayUs(26);// 时序要求

    for(i = 0; i < 786; i++)
        OnfiBuf[i] = (*(vu8*)NAND_ADDRESS);

    if(DecodeONFI((const char*)OnfiBuf, Descr, 0))
        Ret = -1;

    //此处修改Descr以保留NANDFLASH最后1M作为异常信息记录存储介质,存储介质为256M NANDFLASH
    //2个LUN，1个Lun有1024Blocks(128M),每个Block有64page(128k),每个Page有(2k+64)bytes。
    //异常信息存储使用NANDFLASH最后1M空间，使用了8个Block,512个Pages。
    Descr->LUNs=1;
    Descr->BlksPerLUN=2040;
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
    NAND_Handler.Init.TCLRSetupTime=10;//设置TCLR(tCLR=CLE到RE的延时)=(TCLR+TSET+2)*THCLK,THCLK=1/180M=4.6ns
    NAND_Handler.Init.TARSetupTime=10; //设置TAR(tAR=ALE到RE的延时)=(TAR+TSET+1)*THCLK,THCLK=1/180M=4.6n。

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
    Djy_DelayUs(100*mS);
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
    WaitNandReady();
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
        *(vu8*)(NAND_ADDRESS|NAND_CMD)=(STATUS_READ_CMD_BYTE);
        Status = (*(vu8*)NAND_ADDRESS);
    }
    while(Status & 0x1);

    return(0);
}
//-----------------------------------------------------------------------------
//功能: 等待RB引脚为某个电瓶
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------

static u8 NAND_WaitRB(vu8 rb)
{
    vu16 time=0;
    while(time<10000)
    {
        time++;
        if(NAND_RB==rb)return 0;
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
//功能:
//参数: ChipName --
//      Clean -- 器件格式化;"1"--是;"0"--否。
//返回: "0" -- 成功;
//      "-1" -- 输入参数错误;
//      "-2" -- 内存不足;
//      "-3" -- 设备信息获取失败。
//备注:
//-----------------------------------------------------------------------------
s32 ModuleInstall_NAND(const char *ChipName, u32 Flags, u16 StartBlk)
{
    /*逻辑:
       1.芯片管脚等基本设置;
       2.(包括判断逻辑)获取芯片的信息;
       3.在dev分支下建立NAND节点;
       4.初始化NAND节点,包括FLASH的操作函数;
    */
    u32 Len;
    struct FlashChip *Chip;
    struct NandDescr ChipDesrc = {0};

    if (NULL == ChipName)
    {
        TraceDrv(FLASH_TRACE_ERROR, "unavailable param! \r\n");
        return (-1);
    }

    stm32f7_NAND_ControllerConfig();// 芯片管脚等基本配置

    // 获取NAND信息
    if(stm32f7_GetNandDescr(&ChipDesrc))
    {
        TraceDrv(FLASH_TRACE_ERROR, "解析NAND信息失败\r\n");
        gb_NandFlashReady=-3;
        return gb_NandFlashReady;
    }

    if(StartBlk >= ChipDesrc.BlksPerLUN * ChipDesrc.LUNs)
        return (-1);

    ChipDesrc.ReservedBlks = StartBlk;
    ChipDesrc.Controller = HW_ECC_SUPPORTED;
    ChipDesrc.BadMarkOffset = ChipDesrc.OOB_Size - 4 - 1;
    Len = strlen (ChipName) + 1;

    Chip = (struct FlashChip*)malloc(sizeof(struct FlashChip) + Len);
    if (NULL == Chip)
    {
        TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
        gb_NandFlashReady=-2;
        return gb_NandFlashReady;
    }

    memset (Chip, 0x00, sizeof(*Chip));

    Chip->Type            = F_NAND;
    Chip->Descr.Nand      = ChipDesrc;
    Chip->Ops.RdPage      = stm32f7_PageRead;
    Chip->Ops.WrPage      = stm32f7_PageProgram;
    Chip->Ops.ErsBlk      = stm32f7_BlockErase;
    Chip->Ops.Special.Nand.ChkBlk = stm32f7_BadChk;
    Chip->Ops.Special.Nand.MrkBad = stm32f7_BadMark;

    strcpy(Chip->Name, ChipName);// 设备名

    s_ptNandInfo = (struct NandDescr *)&(Chip->Descr);

    Chip->Buf = (u8*)malloc(s_ptNandInfo->OOB_Size + s_ptNandInfo->BytesPerPage);// NAND底层缓冲
    if(NULL == Chip->Buf)
    {
        TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
        free(Chip);
        gb_NandFlashReady=-2;
        return gb_NandFlashReady;
    }

    dev_add(NULL, Chip->Name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip);// 设备接入"/dev"

    if(Flags & FLASH_ERASE_ALL)
        EarseWholeChip(Chip);
    gb_NandFlashReady=0;
    return (0);// 成功;
}



bool_t NandFlash_Ready(void)
{
    if(gb_NandFlashReady)
        return false;
    else
        return true;
}




/******************************************************************************
                         简易文件系统函数
******************************************************************************/
//#ifdef EFS_ON
//-----------------------------------------------------------------------------
//功能: 写小于一页的数据
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------






#define __WritePage(a,b,c) stm32f7_PageProgram(a,b,c)
#define __ReadPage(a,b,c) stm32f7_PageRead(a,b,c)
static s32 __WriteFragment(u32 PageNo, u32 Offset, const u8 *Buf, u32 Size)
{
    u32 i;
    s32 Ret;

    *(vu8*)(NAND_ADDRESS|NAND_CMD) = PAGE_PROGRAM_CMD_BYTE1;

    *(vu8*)(NAND_ADDRESS|NAND_ADDR) = (Offset);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR) = (Offset>>8);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR) = (PageNo);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR) = (PageNo>>8);

    for(i = 0; i < Size; i++)
        *(vu8*)NAND_ADDRESS=(Buf[i]);

    *(vu8*)(NAND_ADDRESS|NAND_CMD) = PAGE_PROGRAM_CMD_BYTE2;

    Djy_EventDelay(700);// 切出
    WaitNandReady();// 时序要求
    Ret = StatusOfNand();
    if(Ret)
        return (-2);
    return (Size);
}

//-----------------------------------------------------------------------------
//功能: 读小于一页的数据
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static s32 __ReadFragment(u32 PageNo, u32 Offset, u8 *Buf, u32 Size)
{
    u32 i;
    s32 Ret;

    WaitNandReady();

    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE1);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR) = (Offset);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR) = (Offset>>8);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR) = (PageNo);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR) = (PageNo>>8);
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE2);

    Djy_EventDelay(25);
    WaitNandReady(); // 时序要求

    for(i = 0; i < Size; i++)
        Buf[i] = (*(vu8*)NAND_ADDRESS);

    Ret = StatusOfNand();

    if(Ret)
        return (-2);

    return (Size);
}
//-----------------------------------------------------------------------------
//功能: 写数据
//参数: BlkNo -- 块号;
//      Offset -- 块内偏置;
//      Buf -- 写缓冲;
//      Size -- 写数据大小
//      Flags -- 0x0 -- 无功能; 0x1 - ECC功能开启(数据大小为必须256)
//返回:
//备注:
//-----------------------------------------------------------------------------
u32 EFS_IF_WriteData(u32 BlkNo, u32 Offset, u8 *Buf, u32 Size, u8 Flags)
{
    s32 Ret;
    u32 PageNo = (BlkNo << 6) + (Offset >> 11);
    u32 PageOffset = Offset & 0x7FF;
    u32 WrLen = Size;

    if(!Buf)
        return (0);

    // ECC的数据源为256字节，放入数据后端,3个字节
    if(1 == Flags)
    {
        hamming_compute_256x(Buf, 256, Buf+256);
        WrLen = Size + 3;
        PageOffset += 3 * (PageOffset >> 8);
    }

    for(;;)
    {
        if((!PageOffset) && (WrLen >= s_ptNandInfo->BytesPerPage))
            Ret = __WritePage(PageNo, Buf, NO_ECC);
        else if((WrLen >= s_ptNandInfo->BytesPerPage) || // 写数据大与等于一页,但内容跨页
                (((WrLen+PageOffset) > s_ptNandInfo->BytesPerPage) && (!Flags))) // 写数据小于一页,但内容跨页(只有考虑非ECC的情况)
            Ret = __WriteFragment(PageNo, PageOffset, (const u8*)Buf, (s_ptNandInfo->BytesPerPage - PageOffset));
        else
            Ret = __WriteFragment(PageNo, PageOffset, (const u8*)Buf, WrLen);

        if(Ret <= 0)
            break;// 错误或者已写完

        WrLen -= Ret;
        if(WrLen <= 0)
            break;
        Buf += Ret;
        PageNo++;
        if(PageOffset)
            PageOffset = 0;
    }

    if(Ret > 0)
        return (Size);
    else
        return (0);
}
//-----------------------------------------------------------------------------
//功能: 读数据
//参数: BlkNo -- 块号;
//      Offset -- 块内偏置;
//      Buf -- 读缓冲;
//      Size -- 读数据大小
//      Flags -- 0x0 -- 无功能; 0x1 - ECC功能开启(数据大小为必须256)
//返回:
//备注:
//-----------------------------------------------------------------------------
u32 EFS_IF_ReadData(u32 BlkNo, u32 Offset, u8 *Buf, u32 Size, u8 Flags)
{
    s32 Ret;
    u32 PageNo = (BlkNo << 6) + (Offset >> 11);
    u32 PageOffset = Offset & 0x7FF;
    u32 RdLen = Size;

    if(!Buf)
        return (0);

    if(1 == Flags)
    {
        RdLen += 3;
        PageOffset += 3 * (PageOffset >> 8);
    }

    for(;;)
    {
        if((!PageOffset) && (RdLen >= s_ptNandInfo->BytesPerPage))
            Ret = __ReadPage(PageNo, Buf, NO_ECC);
        else if((RdLen >= s_ptNandInfo->BytesPerPage) || // 写数据大与等于一页,但内容跨页
                (((RdLen+PageOffset) > s_ptNandInfo->BytesPerPage) && (!Flags))) // 读数据小于一页,但内容跨页(只有非ECC的情况需要考虑)
            Ret = __ReadFragment(PageNo, PageOffset, Buf, (s_ptNandInfo->BytesPerPage - PageOffset));
        else
            Ret = __ReadFragment(PageNo, PageOffset, Buf, RdLen);

        if(Ret <= 0)
            break;// 错误或者已写完

        RdLen -= Ret;
        if(RdLen <= 0)
            break;
        Buf += Ret;
        PageNo++;
        if(PageOffset)
            PageOffset = 0;
    }


    if(Ret <= 0)
        return (0);

    if((1 == Flags) && (256 == Size))
    {
        u8 *HammingCode;
        u32 EccRet;

        HammingCode = Buf + 256;
        EccRet = hamming_verify_256x(Buf, 256, HammingCode);
        if (EccRet && (EccRet != HAMMING_ERROR_SINGLE_BIT))
        {
            TraceDrv(FLASH_TRACE_DEBUG, "cannot be fixed");
            return (0);
        }
    }

    return (Size);
}
//-----------------------------------------------------------------------------
//功能: 擦除
//参数: BlkNo -- 块号;
//返回:
//备注:
//-----------------------------------------------------------------------------
bool_t EFS_IF_Erase(u32 BlkNo)
{
    if(0==stm32f7_BlockErase(BlkNo))
        return (TRUE);
    return (FALSE);
}
//-----------------------------------------------------------------------------
//功能: 检查文件是否可写
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static u8 TempBuf[259];
s32 EFS_IF_IsFragmentWritten(u32 BlkNo, u32 Offset)
{
    u16 i;
    u32 PageOffset = Offset & 0x7FF;
    u8 EccOffset = 3 * (PageOffset >> 8);

    if(0 == EFS_IF_ReadData(BlkNo, (Offset+EccOffset), TempBuf, 259, 0))
        return (-1);

    for(i = 0; i < 259; i++)
    {
        if(0xFF != TempBuf[i])
            return (-1);
    }

    return (0);
}

//-----------------------------------------------------------------------------
//功能: 未实现
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
bool_t EFS_IF_CheckBlockReady(u32 block,u32 offset,
        u8 *buf,u32 size)
{
    u32 CurBlock,CurBlockOffset;
    u32 BlockSize;
    bool_t result = TRUE;

    if(NULL == s_ptNandInfo)
    {
        return FALSE;
    }

    CurBlock = block;
    CurBlockOffset = offset;
    BlockSize = s_ptNandInfo->BytesPerPage * s_ptNandInfo->PagesPerBlk;

    while(size)
    {
        if(-1 == EFS_IF_IsFragmentWritten(CurBlock,CurBlockOffset))
        {
            result = FALSE;
            break;
        }
        CurBlockOffset += 256;
        CurBlock += CurBlockOffset / BlockSize;
        CurBlockOffset = CurBlockOffset % BlockSize;
        size = (size > 256) ? (size - 256) :0;
    }
    return result;// 文件写入操作不存在改写逻辑
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


    Chip->Ops.WrPage(PageNo, Buf, HW_ECC | SPARE_REQ);

    memset(Buf, 0, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Chip->Ops.RdPage(PageNo, Buf, HW_ECC | SPARE_REQ);
    for(i=0; i<(s_ptNandInfo->BytesPerPage); i++)
    {
        if(((u8)i) != Buf[i])
            while(1);
    }
    // ECC一位纠错  todo
    Buf[1] = 0x0;
    Chip->Ops.WrPage(PageNo, Buf, NO_ECC | SPARE_REQ);

    memset(Buf, 0, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Chip->Ops.RdPage(PageNo, Buf, HW_ECC | SPARE_REQ);
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
        printf("test block :%d   \r",TestBlocks);
        for(TestPages = 0; TestPages < s_ptNandInfo->PagesPerBlk; TestPages++)
        {
            u32 CurPage = TestPages + (TestBlocks * s_ptNandInfo->PagesPerBlk);
            memset(Buf, 0xAA, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
            Chip->Ops.RdPage(CurPage, Buf, NO_ECC | SPARE_REQ);
            for(i=0; i<(s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size); i++)
            {
                if(0xFF != Buf[i])
                {
                    ErrorCount += 1;
                    //PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
                    while(1);
                }
            }

            PageTest(CurPage, 0,TestBlocks);
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

#endif
