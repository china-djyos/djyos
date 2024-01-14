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
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
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
// =============================================================================

#define DMA_WAIT_TIME (2*mS)
//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Channel:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,范围:0~7
//par:外设地址
//mar:存储器地址

bool_t DMA_ClearIntFlag(dma_channel_type *DMA_Channel);

// =============================================================================
// 功能：检查通道是否合法
// 参数：DMA_Channel:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
// 返回：true -- 合法，false -- 不合法
// =============================================================================
static bool_t DMA_CheckChannel(dma_channel_type *DMA_Channel)
{
    if ((DMA_Channel == DMA1_CHANNEL1) || (DMA_Channel == DMA1_CHANNEL2) || (DMA_Channel == DMA1_CHANNEL3) 
        || (DMA_Channel == DMA1_CHANNEL4) || (DMA_Channel == DMA1_CHANNEL5) || (DMA_Channel == DMA1_CHANNEL6)
        || (DMA_Channel == DMA1_CHANNEL7) || (DMA_Channel == DMA2_CHANNEL1) || (DMA_Channel == DMA2_CHANNEL2) 
        || (DMA_Channel == DMA2_CHANNEL3) || (DMA_Channel == DMA2_CHANNEL4) || (DMA_Channel == DMA2_CHANNEL5) 
        || (DMA_Channel == DMA2_CHANNEL6) || (DMA_Channel == DMA2_CHANNEL7)) 
    {
        return true;
    }
    return false;
}

// =============================================================================
// 功能：根据通道获取通道的MUX寄存器地址
// 参数：DMA_Channel:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
// 返回：对应通道的MUX地址
// =============================================================================
static dmamux_channel_type *DMA_GetChannelMUX(dma_channel_type *DMA_Channel)
{
    dmamux_channel_type *MUX = NULL;

    switch ((u32)DMA_Channel)
    {
        case DMA1_CHANNEL1_BASE:     MUX = DMA1MUX_CHANNEL1;break;
        case DMA1_CHANNEL2_BASE:     MUX = DMA1MUX_CHANNEL2;break;
        case DMA1_CHANNEL3_BASE:     MUX = DMA1MUX_CHANNEL3;break;
        case DMA1_CHANNEL4_BASE:     MUX = DMA1MUX_CHANNEL4;break;
        case DMA1_CHANNEL5_BASE:     MUX = DMA1MUX_CHANNEL5;break;
        case DMA1_CHANNEL6_BASE:     MUX = DMA1MUX_CHANNEL6;break;
        case DMA1_CHANNEL7_BASE:     MUX = DMA1MUX_CHANNEL7;break;
        case DMA2_CHANNEL1_BASE:     MUX = DMA2MUX_CHANNEL1;break;
        case DMA2_CHANNEL2_BASE:     MUX = DMA2MUX_CHANNEL2;break;
        case DMA2_CHANNEL3_BASE:     MUX = DMA2MUX_CHANNEL3;break;
        case DMA2_CHANNEL4_BASE:     MUX = DMA2MUX_CHANNEL4;break;
        case DMA2_CHANNEL5_BASE:     MUX = DMA2MUX_CHANNEL5;break;
        case DMA2_CHANNEL6_BASE:     MUX = DMA2MUX_CHANNEL6;break;
        case DMA2_CHANNEL7_BASE:     MUX = DMA2MUX_CHANNEL7;break;

    default:
        break;
    }

    return MUX;
}

// =============================================================================
// 功能：DMAx的各通道配置
// 参数：DMA_Channel,DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//      req: 用途
//      par:外设地址
//      mar:存储器地址
//      dir,方向，从外设到内存、内存到内存，外设到外设
//      msize,
//      psize,
//      ndtr,
// 返回：true -- 成功，false -- 失败
// =============================================================================
bool_t DMA_Config(dma_channel_type *DMA_Channel, dmamux_requst_id_sel_type req, u32 par, u32 mar,
        u16 dir, u8 msize, u8 psize, u16 ndtr)
{
    dma_init_type dma_init_struct;
    dma_type *DMAx = NULL;
    // DMA通道合法

    if (DMA_CheckChannel(DMA_Channel) && ((dir == DMA_DIR_M2M) || (dir == DMA_DIR_M2P) || (dir == DMA_DIR_P2M))
        && ((msize == DMA_MEM_DATABITS_8) || (msize == DMA_MEM_DATABITS_16) || (msize == DMA_MEM_DATABITS_32))
        && ((psize == DMA_PER_DATABITS_8) || (psize == DMA_PER_DATABITS_16) || (psize == DMA_PER_DATABITS_32))
        )
    {
        // 这里要开时钟，不然有可能DMA的寄存器会设置不成功
        if (((u32)DMA_Channel >= (u32)DMA2_CHANNEL1) && ((u32)DMA_Channel <= (u32)DMA2_CHANNEL7))//得到当前stream是属于DMA2还是DMA1
        {
            // RCC->AHB1ENR|=1<<22;//DMA2时钟使能
            DMAx=DMA2;
            crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);   //DMA2时钟使能
        }else
        {
            // RCC->AHB1ENR|=1<<21;//DMA1时钟使能
            DMAx=DMA1;
            crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);   //DMA1时钟使能
        }
        // 设置DMA寄存器之前要把dma给关了
        dma_channel_enable(DMA_Channel, FALSE);

//        DMA_ClearIntFlag(DMA_Channel);
        dma_reset(DMA_Channel);
        dma_default_para_init(&dma_init_struct);
        dma_init_struct.buffer_size = ndtr;
        dma_init_struct.direction = dir;
        dma_init_struct.memory_base_addr = (uint32_t)mar;
        dma_init_struct.memory_data_width = msize;
        dma_init_struct.memory_inc_enable = TRUE;
        dma_init_struct.peripheral_base_addr = par;
        dma_init_struct.peripheral_data_width = psize;
        dma_init_struct.peripheral_inc_enable = FALSE;
        dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
        dma_init_struct.loop_mode_enable = FALSE;
        dma_init(DMA_Channel, &dma_init_struct);

        dmamux_enable(DMAx, TRUE);
        dmamux_init(DMA_GetChannelMUX(DMA_Channel), req);
    }
    else
    {
        return false;
    }


    return true;
}

// =============================================================================
// 功能：开启一次DMA传输
// 参数：DMA_Channel:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//       ndtr:数据传输量
// 返回：true -- 成功，false -- 失败
// =============================================================================
bool_t DMA_Enable(dma_channel_type *DMA_Channel, u32 mar, u16 ndtr)
{
//    dma_type *DMAx;
//    u8 streamx;

    if (false == DMA_CheckChannel(DMA_Channel))
    {
        return false;
    }

    if (((u32)DMA_Channel >= (u32)DMA2_CHANNEL1) && ((u32)DMA_Channel <= (u32)DMA2_CHANNEL7))//得到当前stream是属于DMA2还是DMA1
    {
//        DMAx=DMA2;
        // RCC->AHB1ENR|=1<<22;//DMA2时钟使能
        crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);   //DMA2时钟使能
    }else
    {
//        DMAx=DMA1;
        // RCC->AHB1ENR|=1<<21;//DMA1时钟使能
        crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);   //DMA1时钟使能
    }
    // 设置DMA寄存器之前要把dma给关了
    dma_channel_enable(DMA_Channel, FALSE);

    // dma_reset(DMA_Channel);
    DMA_ClearIntFlag(DMA_Channel);

    DMA_Channel->maddr = mar;
    DMA_Channel->dtcnt_bit.cnt = ndtr;

    dma_channel_enable(DMA_Channel, TRUE); //打开DMA传输

    // if((u32)DMA_Channel>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
    // {
    //     DMAx=DMA2;
    //     RCC->AHB1ENR|=1<<22;//DMA2时钟使能
    // }else
    // {
    //     DMAx=DMA1;
    //     RCC->AHB1ENR|=1<<21;//DMA1时钟使能
    // }

    // streamx=(((u32)DMA_Channel-(u32)DMAx)-0X10)/0X18;       //得到stream通道号

    // DMA_Channel->CR&=~(1<<0);       //关闭DMA传输
    // while(DMA_Channel->CR&0X1);     //确保DMA可以被设置

    // if(streamx>=6)
    //     DMAx->HIFCR|=0X3D<<(6*(streamx-6)+16);              //清空之前该stream上的所有中断标志
    // else if(streamx>=4)
    //     DMAx->HIFCR|=0X3D<<6*(streamx-4);
    // else if(streamx>=2)
    //     DMAx->LIFCR|=0X3D<<(6*(streamx-2)+16);
    // else
    //     DMAx->LIFCR|=0X3D<<6*streamx;

    // DMA_Channel->NDTR=ndtr;         //DMA 数据量
    // DMA_Channel->M0AR=mar;          //DMA 存储器0地址

    // DMA_Channel->CR|= (1<<0);           //开启DMA传输

    return true;
}

// =============================================================================
// 功能：禁止使用相应的DMA流
// 参数：DMA_Channel:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
// 返回：true -- 成功，false -- 失败
// =============================================================================
bool_t DMA_Disable(dma_channel_type *DMA_Channel)
{
    if (false == DMA_CheckChannel(DMA_Channel))
    {
        return false;
    }
    // DMA_Channel->CR&=~(1<<0);       //关闭DMA传输
    // while(DMA_Channel->CR&0X1);     //确保DMA可以被设置
    // 设置DMA寄存器之前要把dma给关了
    dma_channel_enable(DMA_Channel, FALSE);

    return true;
}

// =============================================================================
// 功能：清除相应流上的所有中断标志位
// 参数：DMA_Channel:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
// 返回：true -- 成功，false -- 失败
// =============================================================================
bool_t DMA_ClearIntFlag(dma_channel_type *DMA_Channel)
{
//    dma_type *DMAx;
//    u8 streamx;
    uint32_t temp = 0;

    if (false == DMA_CheckChannel(DMA_Channel))
    {
        return false;
    }

    if (((u32)DMA_Channel >= (u32)DMA2_CHANNEL1) && ((u32)DMA_Channel <= (u32)DMA2_CHANNEL7))//得到当前stream是属于DMA2还是DMA1
    {
//        DMAx=DMA2;
        // RCC->AHB1ENR|=1<<22;//DMA2时钟使能
        crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);   //DMA2时钟使能
    }else
    {
//        DMAx=DMA1;
        // RCC->AHB1ENR|=1<<21;//DMA1时钟使能
        crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);   //DMA1时钟使能
    }
    // 设置DMA寄存器之前要把dma给关了
    dma_channel_enable(DMA_Channel, FALSE);

    temp = (uint32_t)DMA_Channel;

    if((temp & 0x6FF) < 0x608)
    {
        /* dma1 channel */
        DMA1->clr |= (uint32_t)(0x0F << ((((temp & 0xFF) - 0x08) / 0x14) * 4));
    }
    else if((temp & 0x6FF) < 0x688)
    {
        /* dma2 channel */
        DMA2->clr |= (uint32_t)(0x0F << ((((temp & 0xFF) - 0x08) / 0x14) * 4));
    }

    // if((u32)DMA_Channel>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
    // {
    //     DMAx=DMA2;
    //     RCC->AHB1ENR|=1<<22;//DMA2时钟使能
    // }else
    // {
    //     DMAx=DMA1;
    //     RCC->AHB1ENR|=1<<21;//DMA1时钟使能
    // }

    // streamx=(((u32)DMA_Channel-(u32)DMAx)-0X10)/0X18;       //得到stream通道号

    // DMA_Channel->CR&=~(1<<0);       //关闭DMA传输
    // while(DMA_Channel->CR&0X1);     //确保DMA可以被设置

    // if(streamx>=6)
    //     DMAx->HIFCR|=0X3D<<(6*(streamx-6)+16);              //清空之前该stream上的所有中断标志
    // else if(streamx>=4)
    //     DMAx->HIFCR|=0X3D<<6*(streamx-4);
    // else if(streamx>=2)
    //     DMAx->LIFCR|=0X3D<<(6*(streamx-2)+16);
    // else
    //     DMAx->LIFCR|=0X3D<<6*streamx;

    return true;
}

// =============================================================================
// 功能：使能相应流上的中断
// 参数：DMA_Channel:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//      SrcInt,中断位掩码，如DMA_INT_HTIE
// 返回：true -- 成功，false -- 失败
// =============================================================================
bool_t DMA_IntEnable(dma_channel_type *DMA_Channel, u8 SrcInt)
{
    // DMA_Channel->CR&=~(1<<0);       //关闭DMA传输
    // while(DMA_Channel->CR&0X1);     //确保DMA可以被设置

    // DMA_Channel->CR |= SrcInt;

    if (false == DMA_CheckChannel(DMA_Channel))
    {
        return false;
    }
    // 设置DMA寄存器之前要把dma给关了
    dma_channel_enable(DMA_Channel, FALSE); //关闭DMA传输

    dma_interrupt_enable(DMA_Channel, SrcInt, TRUE);    //开中断
    return true;
}

// =============================================================================
// 功能：判断相应流的中断
// 参数：DMA_Channel:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//      SrcInt,中断位掩码，如DMA_INT_HTIE，为(1<<3)
// 返回：true -- 成功，false -- 失败
// =============================================================================
bool_t DMA_IntDisable(dma_channel_type *DMA_Channel,u8 SrcInt)
{
    // DMA_Channel->CR&=~(1<<0);       //关闭DMA传输
    // while(DMA_Channel->CR&0X1);     //确保DMA可以被设置

    // DMA_Channel->CR &= ~SrcInt;

    if (false == DMA_CheckChannel(DMA_Channel))
    {
        return false;
    }
    // 设置DMA寄存器之前要把dma给关了
    dma_channel_enable(DMA_Channel, FALSE); //关闭DMA传输

    dma_interrupt_enable(DMA_Channel, SrcInt, FALSE);    //关中断

    return true;
}


