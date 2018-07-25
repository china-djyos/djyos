//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：SC1161Y_spi.c
// 模块描述:SC1161Y加密芯片接口驱动
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 8/10.2017
// =============================================================================

#include "stdint.h"
#include "string.h"
#include "cpu_peri.h"
#include "spibus.h"
#include "board-config.h"
#include "os.h"
#include "systime.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_SC1161Y(char *BusName);
//    NRSEC3000_Init(CFG_SC1161Y_BUS_NAME);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"SC1161Y"      //spi接口的加密芯片
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp组件             //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:可选                //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//grade:init                    //初始化时机，可选值：none，init，main。none表示无须初始化，
                                //init表示在调用main之前，main表示在main函数中初始化
//dependence:"lock","spibus"    //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#ifndef CFG_SC1161Y_BUS_NAME   //****检查参数是否已经配置好
#warning    SC1161Y组件参数未配置，使用默认值
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#define CFG_SC1161Y_BUS_NAME              ""      //"总线名称",器件的SPI总线名称，如"SPI0"
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

// =============================================================================
static struct SPI_Device sSc1161yDev;
static struct SemaphoreLCB sSc1161ySemp;   //芯片互斥访问保护

#define SC1161Y_SPI_SPEED       (5*1000*1000)//加密芯片建议用5M速度
#define SC1161Y_TIMEOUT         (3000*mS)   //手册上面写的最多3S

#define RD_DATA_HEAD_LEN   5
// =============================================================================
//功能：SPI接口的片选使能，SC1161Y片选使能后，需延时50us才可读写数据
//参数：无
//返回：无
// =============================================================================
void SC1161Y_CsActive(void)
{
    SPI_CsActive(&sSc1161yDev,SC1161Y_TIMEOUT);
    Djy_DelayUs(50);
}

// =============================================================================
//功能：SPI接口的片选禁能，SC1161Y片选禁能后，需延时10us才可重新片选
//参数：无
//返回：无
// =============================================================================
void SC1161Y_CsInActive(void)
{
    SPI_CsInactive(&sSc1161yDev);
    Djy_DelayUs(10);
}
// =============================================================================
//功能：SPI读写函数，因为SPI是读写同时进行的，因此读写函数一体
//参数：byWrBuf,写数据缓冲区指针，无写入数据可写为NULL
//     dwWrLen,写数据字节数，无写入数据可写为0
//     byRdBuf,读数据缓冲区指针，无读取数据可写为NULL
//     dwRdLen，读数据字节数，无读取数据可写0
//返回：true = 成功初始化，false = 初始化失败
// =============================================================================
//bool_t SC1161Y_SpiTransfer(u8* byWrBuf,u32 dwWrLen,u8* byRdBuf, u32 dwRdLen)
//{
//  struct SPI_DataFrame data;
//    s32 Ret = false;
//
//  if(Lock_SempPend(&sSc1161ySemp,SC1161Y_TIMEOUT))
//  {
//      SC1161Y_CsActive();
//
//      data.RecvBuf = byRdBuf;
//      data.RecvLen = dwRdLen;
//      data.RecvOff = dwWrLen;
//      data.SendBuf = byWrBuf;
//      data.SendLen = dwWrLen;
//
//      Ret = SPI_Transfer(&sSc1161yDev,&data,true,SC1161Y_TIMEOUT);
//      if(Ret != CN_SPI_EXIT_NOERR)
//          Ret = false;
//      else
//          Ret = true;
//
//      SC1161Y_CsInActive();
//      Lock_SempPost(&sSc1161ySemp);
//  }
//  return Ret;
//}

static bool_t __SC1161Y_SpiTransfer(u8* byWrBuf,u32 dwWrLen,u8* byRdBuf, u32 dwRdLen)
{
    bool_t Ret = false;
    struct SPI_DataFrame data;

    data.RecvBuf = byRdBuf;
    data.RecvLen = dwRdLen;
    data.RecvOff = dwWrLen;
    data.SendBuf = byWrBuf;
    data.SendLen = dwWrLen;

    Ret = SPI_Transfer(&sSc1161yDev,&data,true,SC1161Y_TIMEOUT);
    if(Ret != CN_SPI_EXIT_NOERR)
        Ret = false;
    else
        Ret = true;
    return Ret;
}

// =============================================================================
//功能：写加密芯片
//参数：byWrBuf,写数据缓冲区指针，不可写为NULL
//     dwWrLen,写数据字节数，不可写为0
//返回：true = 成功初始化，false = 初始化失败
// =============================================================================
bool_t SC1161Y_Write(u8* byWrBuf,u32 dwLen)
{
    bool_t Ret = false;

    if( (NULL == byWrBuf) || (0 == dwLen) )
        return Ret;
    if(Lock_SempPend(&sSc1161ySemp,SC1161Y_TIMEOUT))
    {
        SC1161Y_CsActive();
        Ret = __SC1161Y_SpiTransfer(byWrBuf,dwLen,NULL,0);
        SC1161Y_CsInActive();
        Lock_SempPost(&sSc1161ySemp);
    }

    return Ret;
}
// =============================================================================
//功能：读加密芯片
//参数：pbRdBuf,读数据缓冲区指针，不可写为NULL
//     timeout，延时时间
//返回：true = 成功初始化，false = 初始化失败
// =============================================================================
u32 SC1161Y_Read(u8* pbRdBuf,u32 timeout)
{
    bool_t Ret = false;
    u16 DataLen = 0,i;
    u32 Timeout = SC1161Y_TIMEOUT;
    u8 sRdData[RD_DATA_HEAD_LEN];

    if(NULL == pbRdBuf)
        return DataLen;
//  Djy_EventDelay(timeout);
    if(Lock_SempPend(&sSc1161ySemp,SC1161Y_TIMEOUT))
    {
        SC1161Y_CsActive();
        while(1)
        {
            Ret = __SC1161Y_SpiTransfer(NULL,0,sRdData,RD_DATA_HEAD_LEN);
            for(i = 0; i < RD_DATA_HEAD_LEN; i++)
            {
                if(sRdData[i] == 0x55)
                {
                    memcpy(pbRdBuf,&sRdData[i],RD_DATA_HEAD_LEN-i);

                    Ret = __SC1161Y_SpiTransfer(NULL,0,sRdData,i);
                    memcpy(pbRdBuf+RD_DATA_HEAD_LEN-i,sRdData,i);
                    DataLen =  (pbRdBuf[3] << 8) + pbRdBuf[4];
                    Ret = __SC1161Y_SpiTransfer(NULL,0,pbRdBuf+RD_DATA_HEAD_LEN,DataLen + 1);
                    DataLen = DataLen + RD_DATA_HEAD_LEN + 1;
                    break;
                }
            }
            if( (i != RD_DATA_HEAD_LEN) || (Timeout < 100) || (Ret != true))
                break;
            Djy_EventDelay(100);
            Timeout -= 100;
        }

        SC1161Y_CsInActive();
        Lock_SempPost(&sSc1161ySemp);
    }
    return DataLen;
}
// =============================================================================
//功能：注册加密芯片到SPI总线上，并配置SPI的工作模式，总线速度等。
//参数：总线名称，如“SPI1”
//返回：true = 成功初始化，false = 初始化失败
// =============================================================================
bool_t ModuleInstall_SC1161Y(char *BusName)
{
    sSc1161yDev.AutoCs = false;
    sSc1161yDev.CharLen = 8;
    sSc1161yDev.Cs = 1;
    sSc1161yDev.Freq = SC1161Y_SPI_SPEED;
    sSc1161yDev.Mode = SPI_MODE_3;
    sSc1161yDev.ShiftDir = SPI_SHIFT_MSB;

    if(NULL != SPI_DevAdd_s(&sSc1161yDev,BusName,"SC1161Y"))
    {
        SPI_BusCtrl(&sSc1161yDev,CN_SPI_SET_POLL,0,0);
    }

    if(NULL == Lock_SempCreate_s(&sSc1161ySemp,1,1,CN_BLOCK_FIFO,"SC1161Y_SEMP"))
        return false;

    return true;
}
