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

// 文件名     ：cpu_peri_spi.h
// 模块描述: SPI模块底层硬件驱动头文件
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 16/10.2015
// =============================================================================

#ifndef CPU_PERI_I2S_H_
#define CPU_PERI_I2S_H_

#include "stdint.h"
#include "ring_buffer_dma_write.h"
#include "ring_buffer_dma_read.h"

#ifdef __cplusplus
extern "C" {
#endif

struct IIS_Device
{
    u32 open_flag;      //设置打开方式
    u32 sample_rate;    //采样率
    u8 bits_width;      //位宽

    u8 tx_enable:1;     //发送DMA使能状态
    u8 rx_enable:1;     //接收DMA使能状态

    /* rx members */
    u8 *rx_fifo;      //发送缓存地址
    RB_DMA_WR_ST rb_dma_wr;     //注册到博通库里，一个专门处理DMA数据的缓冲区里
    RB_DMA_RD_ST wr_dma_rd;     //注册到博通库里，一个专门处理DMA数据的缓冲区里
    /* tx members */
    u8 *tx_fifo;      //发送缓存地址
    u32 tx_fill_pos;    //发送缓存填充位置
    u32 tx_fill_size;   //发送缓存填充大小
    u8 tx_paused;
};

struct IIS_OperParam
{
    u32 sample_rate;    //采样率
    u8 bits_width;      //位宽
    void (*i2s_rx_fin_handler)(u32);            //接收的回调函数   回调函数是成对的，一次得定义两个，定义了rx回调函数，那IIS_Read就无效了，是通过回调函数拿数据，tx也一样
    void (*i2s_rx_half_fin_handler)(u32);       //接收的回调函数

    void (*i2s_tx_fin_handler)(u32);            //发送的回调函数
    void (*i2s_tx_half_fin_handler)(u32);       //发送回调函数
};


enum i2s_ctrl
{
    IIS_DMA_RX_ENABLE = 0,      //IIS DMA接收使能
    IIS_DMA_TX_ENABLE,          //IIS DMA发送使能
    I2S_DMA_MASTER_SLAVE,      //IIS DMA主从模式设置
    I2S_SAMPLE_RATE_SET,        //IIS DMA采样率设置
    I2S_BIT_WIDTH_SET,          //IIS DMA位宽设置
};

int ModuleInstall_I2S(void);
s32 IIS_Open(struct IIS_OperParam *param, u32 mode);
s32 IIS_Read(void *buffer, s32 size);
s32 IIS_Close(void);
s32 IIS_Write(void *buffer, s32 size);
s32 IIS_Cotrol(enum i2s_ctrl cmd, ptu32_t data);

#ifdef __cplusplus
}
#endif

#endif
