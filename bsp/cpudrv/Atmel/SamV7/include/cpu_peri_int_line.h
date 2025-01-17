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
#ifndef __int_line_h__
#define __int_line_h__

#ifdef __cplusplus
extern "C" {
#endif

#define CN_INT_LINE_SUPC                (0x00)
#define CN_INT_LINE_RSTC                (0x01)
#define CN_INT_LINE_RTC                 (0x02)
#define CN_INT_LINE_RTT                 (0x03)
#define CN_INT_LINE_WDT                 (0x04)
#define CN_INT_LINE_PMC                 (0x05)
#define CN_INT_LINE_EFC                 (0x06)
#define CN_INT_LINE_UART0               (0x07)
#define CN_INT_LINE_UART1               (0x08)
#define CN_INT_LINE_REV                 (0x09)
#define CN_INT_LINE_PIOA                (0x0A)
#define CN_INT_LINE_PIOB                (0x0B)
#define CN_INT_LINE_PIOC                (0x0C)
#define CN_INT_LINE_USART0              (0x0D)
#define CN_INT_LINE_USART1              (0x0E)
#define CN_INT_LINE_USART2              (0x0F)
#define CN_INT_LINE_PIOD                (0X10)
#define CN_INT_LINE_PIOE                (0X11)
#define CN_INT_LINE_HSMCI               (0x12)
#define CN_INT_LINE_TWIHS0              (0x13)
#define CN_INT_LINE_TWIHS1              (0x14)
#define CN_INT_LINE_SPI0                (0x15)
#define CN_INT_LINE_SSC                 (0x16)
#define CN_INT_LINE_TC0                 (0x17)
#define CN_INT_LINE_TC1                 (0x18)
#define CN_INT_LINE_TC2                 (0x19)
#define CN_INT_LINE_TC3                 (0x1a)
#define CN_INT_LINE_TC4                 (0x1b)
#define CN_INT_LINE_TC5                 (0x1c)
#define CN_INT_LINE_AFEC0               (0x1d)
#define CN_INT_LINE_DACC                (0x1e)
#define CN_INT_LINE_PWM0                (0x1f)
#define CN_INT_LINE_ICM                 (0x20)
#define CN_INT_LINE_ACC                 (0x21)
#define CN_INT_LINE_USBHS               (0x22)
#define CN_INT_LINE_MCAN0               (0x23)
#define CN_INT_LINE_MCAN0_LINE1         (0x24)
#define CN_INT_LINE_MCAN1               (0x25)
#define CN_INT_LINE_MCAN1_LINE1         (0x26)
#define CN_INT_LINE_GMAC                (0x27)
#define CN_INT_LINE_AFEC1               (0x28)
#define CN_INT_LINE_TWIHS2              (0x29)
#define CN_INT_LINE_SPI1                (0x2A)
#define CN_INT_LINE_QSPI                (0x2B)
#define CN_INT_LINE_UART2               (0x2C)
#define CN_INT_LINE_UART3               (0x2D)
#define CN_INT_LINE_UART4               (0x2E)
#define CN_INT_LINE_TC6                 (0x2F)
#define CN_INT_LINE_TC7                 (0x30)
#define CN_INT_LINE_TC8                 (0x31)
#define CN_INT_LINE_TC9                 (0x32)
#define CN_INT_LINE_TC10                (0x33)
#define CN_INT_LINE_TC11                (0x34)
#define CN_INT_LINE_MLB                 (0x35)
//RESERVE
//RESERVE
#define CN_INT_LINE_AES                 (0x38)
#define CN_INT_LINE_TRNG                (0x39)
#define CN_INT_LINE_XDMAC               (0x3A)
#define CN_INT_LINE_ISI                 (0x3B)
#define CN_INT_LINE_PWM1                (0x3C)
//RESERVE
#define CN_INT_LINE_SDRAMC              (0x3E)
#define CN_INT_LINE_RSWDT               (0x3F)

#define CN_INT_LINE_LAST                (0x3F)

#ifdef __cplusplus
}
#endif

#endif //__int_line_h__

