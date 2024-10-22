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
//所属模块: CPU外设定义
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 包含CPU所有外设的定义
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

//dma相关寄存器
struct DMA_StReg volatile * const pg_dma1_reg
                        = (struct DMA_StReg *)0x40020000;
struct DMA_StReg volatile * const pg_dma2_reg
                        = (struct DMA_StReg *)0x40020400;

//dma1的各通道寄存器
struct DMA_StChannelReg volatile * const pg_dma1_channel1_reg
                        = (struct DMA_StChannelReg *)0x40020008;
struct DMA_StChannelReg volatile * const pg_dma1_channel2_reg
                        = (struct DMA_StChannelReg *)0x4002001c;
struct DMA_StChannelReg volatile * const pg_dma1_channel3_reg
                        = (struct DMA_StChannelReg *)0x40020030;
struct DMA_StChannelReg volatile * const pg_dma1_channel4_reg
                        = (struct DMA_StChannelReg *)0x40020044;
struct DMA_StChannelReg volatile * const pg_dma1_channel5_reg
                        = (struct DMA_StChannelReg *)0x40020058;
struct DMA_StChannelReg volatile * const pg_dma1_channel6_reg
                        = (struct DMA_StChannelReg *)0x4002006c;
struct DMA_StChannelReg volatile * const pg_dma1_channel7_reg
                        = (struct DMA_StChannelReg *)0x40020080;
//dma2的各通道寄存器
struct DMA_StChannelReg volatile * const pg_dma2_channel1_reg
                        = (struct DMA_StChannelReg *)0x40020408;
struct DMA_StChannelReg volatile * const pg_dma2_channel2_reg
                        = (struct DMA_StChannelReg *)0x4002041c;
struct DMA_StChannelReg volatile * const pg_dma2_channel3_reg
                        = (struct DMA_StChannelReg *)0x40020430;
struct DMA_StChannelReg volatile * const pg_dma2_channel4_reg
                        = (struct DMA_StChannelReg *)0x40020444;
struct DMA_StChannelReg volatile * const pg_dma2_channel5_reg
                        = (struct DMA_StChannelReg *)0x40020458;

//gpio相关寄存器定义
struct GPIO_StReg volatile * const pg_gpio_rega
                        = (struct GPIO_StReg *)0x40010800;
struct GPIO_StReg volatile * const pg_gpio_regb
                        = (struct GPIO_StReg *)0x40010c00;
struct GPIO_StReg volatile * const pg_gpio_regc
                        = (struct GPIO_StReg *)0x40011000;
struct GPIO_StReg volatile * const pg_gpio_regd
                        = (struct GPIO_StReg *)0x40011400;
struct GPIO_StReg volatile * const pg_gpio_rege
                        = (struct GPIO_StReg *)0x40011800;
struct GPIO_StReg volatile * const pg_gpio_regf
                        = (struct GPIO_StReg *)0x40011c00;
struct GPIO_StReg volatile * const pg_gpio_regg
                        = (struct GPIO_StReg *)0x40012000;

struct AFIO_StReg volatile * const pg_afio_reg
                        = (struct AFIO_StReg *)0x40010000;
struct GPIO_StReg volatile * const pg_gpio_reg [7] = {
                                (struct GPIO_StReg *)0x40010800,
                                (struct GPIO_StReg *)0x40010c00,
                                (struct GPIO_StReg *)0x40011000,
                                (struct GPIO_StReg *)0x40011400,
                                (struct GPIO_StReg *)0x40011800,
                                (struct GPIO_StReg *)0x40011c00,
                                (struct GPIO_StReg *)0x40012000,
                               };

//串口相关寄存器
struct st_usart_reg volatile * const pg_uart1_reg
                        = (struct st_usart_reg *)0x40013800;
struct st_usart_reg volatile * const pg_uart2_reg
                        = (struct st_usart_reg *)0x40004400;
struct st_usart_reg volatile * const pg_uart3_reg
                        = (struct st_usart_reg *)0x40004800;
struct st_usart_reg volatile * const pg_uart4_reg
                        = (struct st_usart_reg *)0x40004c00;
struct st_usart_reg volatile * const pg_uart5_reg
                        = (struct st_usart_reg *)0x40005000;

//rcc(复位和时钟控制寄存器)
struct st_rcc_reg volatile * const pg_rcc_reg  = (struct st_rcc_reg *)0x40021000;

//片内flash控制寄存器
struct st_inflash_fpec_reg volatile * const pg_inflash_fpec_reg
                                = (struct st_inflash_fpec_reg *)0x40022000;
struct st_inflash_ob_reg volatile * const pg_inflash_ob_reg
                                = (struct st_inflash_ob_reg *)0x1FFFF800;

//iic
struct I2C_StReg volatile * const pg_iic1_reg
                                = (struct I2C_StReg *)0x40005400;
struct I2C_StReg volatile * const pg_iic2_reg
                                = (struct I2C_StReg *)0x40005800;
