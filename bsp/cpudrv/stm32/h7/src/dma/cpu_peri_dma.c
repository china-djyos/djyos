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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// =============================================================================

// 文件名     ：cpu_peri_dma.c
// 模块描述: DMA驱动
// 模块版本: V1.10
// 创建人员: hm
// 创建时间: 14/07.2015
// =============================================================================

#include "stddef.h"
#include "cpu_peri.h"
#include "systime.h"
#include "djyos.h"
#include "cpu_peri_dma.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip dma"//CPU的dma驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:required               //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                //初始化时机，可选值：early，medium，later, pre-main。
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
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_DMA == false )
//#warning  " cpu_onchip_dma  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_DMA    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#define DMA_WAIT_TIME (2*mS)
//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,范围:0~7
//par:外设地址
//mar:存储器地址

// =============================================================================
// 功能：DMAx的各通道配置
// 参数：DMA_Streamx,DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//      chx:DMA通道选择,范围:0~7
//      par:外设地址
//      mar:存储器地址
//      dir,方向，从外设到内存、内存到内存，外设到外设
//      msize,
//      psize,
//      ndtr,
// 返回：
// =============================================================================
u32 DMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u8 chx,u32 par,bool_t MultiBuffer,
        u32 mar0,u32 mar1,u8 dir,u8 msize,u8 psize,u16 ndtr)
{
    DMA_TypeDef *DMAx;
    u8 streamx;
    u32 WaitTime = DMA_WAIT_TIME;

    if( ((u32)DMA_Streamx < (u32)DMA1) || ((u32)DMA_Streamx > (u32)DMA2_Stream7)
        || (chx > 7) || (dir > DMA_DIR_M2M) || ( msize > DMA_DATABITS_32)
        || (psize > DMA_DATABITS_32) )
        return false;

    if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
    {
        DMAx=DMA2;
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;//DMA2时钟使能
    }else
    {
        DMAx=DMA1;
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;//DMA1时钟使能
    }

    while(DMA_Streamx->CR&0X01)                             //等待DMA可配置
    {
        if(WaitTime > 50)
        {
            DJY_DelayUs(50);
            WaitTime = WaitTime - 50;
        }
        else
        {
            if(DMAx == DMA2)
                RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;//DMA2时钟使能
            else
                RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;//DMA1时钟使能
            return false;
        }
    }
//  streamx = DMA_Streamx - DMA2_Stream0;
    streamx=(((u32)DMA_Streamx-(u32)DMAx)-0X10)/0X18;       //得到stream通道号
    if(streamx>=6)
        DMAx->HIFCR|=0X3D<<(6*(streamx-6)+16);              //清空之前该stream上的所有中断标志
    else if(streamx>=4)
        DMAx->HIFCR|=0X3D<<6*(streamx-4);
    else if(streamx>=2)
        DMAx->LIFCR|=0X3D<<(6*(streamx-2)+16);
    else
        DMAx->LIFCR|=0X3D<<6*streamx;

    DMA_Streamx->PAR=par;           //DMA外设地址
    DMA_Streamx->M0AR=mar0;         //DMA 存储器0地址
    DMA_Streamx->M1AR=mar1;         //DMA 存储器0地址
    DMA_Streamx->NDTR=ndtr;         //DMA 传输数据量
    DMA_Streamx->CR=0;              //先全部复位CR寄存器值
    if(MultiBuffer)
    {
        DMA_Streamx->CR |= DMA_SxCR_DBM;
        DMA_Streamx->CR |= DMA_SxCR_TCIE;//传输完成中断
    }
    else
        DMA_Streamx->CR|=(dir == DMA_DIR_P2M)?(1<<3):(1<<4);//半传输中断
    DMA_Streamx->CR &=~(1<<19);
    DMA_Streamx->CR|= (dir == DMA_DIR_P2M) ? (0<<6) : (1<<6);       //外设到存储器模式
    DMA_Streamx->CR|=0<<8;          //非循环模式(即使用普通模式)
    DMA_Streamx->CR|=0<<9;          //外设非增量模式
    DMA_Streamx->CR|=1<<10;         //存储器增量模式
    DMA_Streamx->CR|=psize<<11;     //外设数据长度:8位
    DMA_Streamx->CR|=msize<<13;     //存储器数据长度:8位
    DMA_Streamx->CR|=1<<16;         //中等优先级
    DMA_Streamx->CR|=1<<20;
    DMA_Streamx->CR|=0<<21;         //外设突发单次传输
    DMA_Streamx->CR|=0<<23;         //存储器突发单次传输
    DMA_Streamx->CR|=(u32)chx<<25;  //通道选择

//  DMA_Streamx->FCR=0X21;      //FIFO控制寄存器//

    return true;
}
void DMA_SetIntModeComplete(DMA_Stream_TypeDef *DMA_Streamx)
{
    DMA_Streamx->CR &=~(3<<3);
    DMA_Streamx->CR |=(1<<4);
}
// =============================================================================
// 功能：开启一次DMA传输
// 参数：DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//       ndtr:数据传输量
// 返回：无
// =============================================================================
void DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u32 mar,u16 ndtr)
{
    DMA_TypeDef *DMAx;
    u8 streamx;

    if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
    {
        DMAx=DMA2;
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;//DMA2时钟使能
    }else
    {
        DMAx=DMA1;
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;//DMA1时钟使能
    }

    streamx=(((u32)DMA_Streamx-(u32)DMAx)-0X10)/0X18;       //得到stream通道号

    DMA_Streamx->CR&=~(1<<0);       //关闭DMA传输
    while(DMA_Streamx->CR&0X1);     //确保DMA可以被设置

    if(streamx>=6)
        DMAx->HIFCR|=0X3D<<(6*(streamx-6)+16);              //清空之前该stream上的所有中断标志
    else if(streamx>=4)
        DMAx->HIFCR|=0X3D<<6*(streamx-4);
    else if(streamx>=2)
        DMAx->LIFCR|=0X3D<<(6*(streamx-2)+16);
    else
        DMAx->LIFCR|=0X3D<<6*streamx;

    DMA_Streamx->NDTR=ndtr;         //DMA 数据量
    DMA_Streamx->M0AR=mar;          //DMA 存储器0地址

    DMA_Streamx->CR|= (1<<0);           //开启DMA传输
}

// =============================================================================
// 功能：禁止使用相应的DMA流
// 参数：DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
// 返回：无
// =============================================================================
void DMA_Disable(DMA_Stream_TypeDef *DMA_Streamx)
{
    DMA_Streamx->CR&=~(1<<0);       //关闭DMA传输
    while(DMA_Streamx->CR&0X1);     //确保DMA可以被设置
}

// =============================================================================
// 功能：清除相应流上的所有中断标志位
// 参数：DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
// 返回：无
// =============================================================================
void DMA_ClearIntFlag(DMA_Stream_TypeDef *DMA_Streamx)
{
    DMA_TypeDef *DMAx;
    u8 streamx;

    if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
    {
        DMAx=DMA2;
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;//DMA2时钟使能
    }else
    {
        DMAx=DMA1;
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;//DMA2时钟使能
    }

    streamx=(((u32)DMA_Streamx-(u32)DMAx)-0X10)/0X18;       //得到stream通道号

//  DMA_Streamx->CR&=~(1<<0);       //关闭DMA传输
//  while(DMA_Streamx->CR&0X1);     //确保DMA可以被设置

    if(streamx>=6)
        DMAx->HIFCR|=0X3D<<(6*(streamx-6)+16);              //清空之前该stream上的所有中断标志
    else if(streamx>=4)
        DMAx->HIFCR|=0X3D<<6*(streamx-4);
    else if(streamx>=2)
        DMAx->LIFCR|=0X3D<<(6*(streamx-2)+16);
    else
        DMAx->LIFCR|=0X3D<<6*streamx;
}

// =============================================================================
// 功能：使能相应流上的中断
// 参数：DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//      SrcInt,中断位掩码，如DMA_INT_HTIE，为(1<<3)
// 返回：
// =============================================================================
void DMA_IntEnable(DMA_Stream_TypeDef *DMA_Streamx,u8 SrcInt)
{
    DMA_Streamx->CR&=~(1<<0);       //关闭DMA传输
    while(DMA_Streamx->CR&0X1);     //确保DMA可以被设置

    DMA_Streamx->CR |= SrcInt;
}

// =============================================================================
// 功能：判断相应流的中断
// 参数：DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//      SrcInt,中断位掩码，如DMA_INT_HTIE，为(1<<3)
// 返回：无
// =============================================================================
void DMA_IntDisable(DMA_Stream_TypeDef *DMA_Streamx,u8 SrcInt)
{
    DMA_Streamx->CR&=~(1<<0);       //关闭DMA传输
    while(DMA_Streamx->CR&0X1);     //确保DMA可以被设置

    DMA_Streamx->CR &= ~SrcInt;
}

static u32 s_u32StreamBusyMask = 0;
DMA_HandleTypeDef *DMA_Alloc(u8 *IntNo)
{
    static DMA_HandleTypeDef DMA_handle[16];
    u32 off;
    for(off = 0; off <16; off++)
    {
        if(((1<<off) & s_u32StreamBusyMask) == 0)
        {
            s_u32StreamBusyMask |= 1<<off;
            break;
        }
    }
    if(off < 16)
    {
        switch(off)
        {
            case 0:
                DMA_handle[off].Instance = DMA1_Stream0;
                *IntNo = CN_INT_LINE_DMA1_Stream0;
                break;
            case 1:
                DMA_handle[off].Instance = DMA1_Stream1;
                *IntNo = CN_INT_LINE_DMA1_Stream1;
                break;
            case 2:
                DMA_handle[off].Instance = DMA1_Stream2;
                *IntNo = CN_INT_LINE_DMA1_Stream2;
                break;
            case 3:
                DMA_handle[off].Instance = DMA1_Stream3;
                *IntNo = CN_INT_LINE_DMA1_Stream3;
                break;
            case 4:
                DMA_handle[off].Instance = DMA1_Stream4;
                *IntNo = CN_INT_LINE_DMA1_Stream4;
                break;
            case 5:
                DMA_handle[off].Instance = DMA1_Stream5;
                *IntNo = CN_INT_LINE_DMA1_Stream5;
                break;
            case 6:
                DMA_handle[off].Instance = DMA1_Stream6;
                *IntNo = CN_INT_LINE_DMA1_Stream6;
                break;
            case 7:
                DMA_handle[off].Instance = DMA1_Stream7;
                *IntNo = CN_INT_LINE_DMA1_Stream7;
                break;
            case 8:
                DMA_handle[off].Instance = DMA2_Stream0;
                *IntNo = CN_INT_LINE_DMA2_Stream0;
                break;
            case 9:
                DMA_handle[off].Instance = DMA2_Stream1;
                *IntNo = CN_INT_LINE_DMA2_Stream1;
                break;
            case 10:
                DMA_handle[off].Instance = DMA2_Stream2;
                *IntNo = CN_INT_LINE_DMA2_Stream2;
                break;
            case 11:
                DMA_handle[off].Instance = DMA2_Stream3;
                *IntNo = CN_INT_LINE_DMA2_Stream3;
                break;
            case 12:
                DMA_handle[off].Instance = DMA2_Stream4;
                *IntNo = CN_INT_LINE_DMA2_Stream4;
                break;
            case 13:
                DMA_handle[off].Instance = DMA2_Stream5;
                *IntNo = CN_INT_LINE_DMA2_Stream5;
                break;
            case 14:
                DMA_handle[off].Instance = DMA2_Stream6;
                *IntNo = CN_INT_LINE_DMA2_Stream6;
                break;
            case 15:
                DMA_handle[off].Instance = DMA2_Stream7;
                *IntNo = CN_INT_LINE_DMA2_Stream7;
                break;
            default:break;
        }
        return &DMA_handle[off];
    }
    else
    {
        return NULL;
    }
}

bool_t DMA_free(DMA_HandleTypeDef *dmahandle)
{
}

