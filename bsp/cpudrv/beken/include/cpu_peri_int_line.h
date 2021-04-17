//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms with or without
// modification are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT INDIRECT INCIDENTAL SPECIAL EXEMPLARY OR
// CONSEQUENTIAL DAMAGES (INCLUDING BUT NOT LIMITED TO PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE DATA OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY WHETHER IN
// CONTRACT STRICT LIABILITY OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE EVEN IF ADVISED OF THE
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
//所属模块:中断模块
//作者:lst
//版本：V1.0.0
//文件描述: 定义中断号
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2014-01-20
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef CPU_PERI_INT_LINE_H_
#define CPU_PERI_INT_LINE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CN_INT_LINE_UART1                             (0)
#define CN_INT_LINE_UART2                             (1)
#define CN_INT_LINE_I2C1                              (2)
#define CN_INT_LINE_IRDA                              (3)
#define CN_INT_LINE_I2S_PCM                           (4)
#define CN_INT_LINE_I2C2                              (5)
#define CN_INT_LINE_SPI                               (6)
#define CN_INT_LINE_GPIO                              (7)
#define CN_INT_LINE_TIMER                             (8)
#define CN_INT_LINE_PWM                               (9)
#define CN_INT_LINE_AUDIO                             (10)
#define CN_INT_LINE_SARADC                            (11)
#define CN_INT_LINE_SDIO                              (12)
#define CN_INT_LINE_USB                               (13)
#define CN_INT_LINE_FFT                               (14)
#define CN_INT_LINE_GDMA                              (15)
#define CN_INT_LINE_MODEM                             (16)
#define CN_INT_LINE_MAC_TX_RX_TIMER                   (17)
#define CN_INT_LINE_MAC_TX_RX_MISC                    (18)
#define CN_INT_LINE_MAC_RX_TRIGGER                    (19)
#define CN_INT_LINE_MAC_TX_TRIGGER                    (20)
#define CN_INT_LINE_MAC_PROT_TRIGGER                  (21)
#define CN_INT_LINE_MAC_GENERAL                       (22)
#define CN_INT_LINE_SDIO_DMA                          (23)
#define CN_INT_LINE_MAILBOX0                          (24)
#define CN_INT_LINE_MAILBOX1                          (25)
#define CN_INT_LINE_MAC_WAKEUP                        (26)
#define CN_INT_LINE_SPI_DMA                           (27)
#define CN_INT_LINE_DPLL_UNLOCK                       (28)
#define CN_INT_LINE_JPEG                              (29)
#define CN_INT_LINE_BLE                               (30)
#define CN_INT_LINE_PSAM                              (31)
#define CN_INT_LINE_LAST                              (31)
#define INT_LINE_2_EN_BIT(x)                      (1 << x)



#ifdef __cplusplus
}
#endif

#endif //__int_line_h__

