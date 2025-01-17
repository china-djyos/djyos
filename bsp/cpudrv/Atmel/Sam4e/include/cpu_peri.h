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
//作者:  lst
//版本：V1.0.0
//其他说明:
//修订历史:
//1. 日期: 2013-05-29
//   作者:  lst.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __CPU_PERI_H__
#define __CPU_PERI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "os.h"
#include "sam4e.h"
#include "cpu_peri_int_line.h"
#include "cpu_peri_pmc.h"
#include "cpu_peri_efc.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_pio.h"
#include "cpu_peri_iic.h"
#include "cpu_peri_spi.h"
#include "cpu_peri_tc.h"
#include "cpu_peri_gmac.h"

#ifdef __cplusplus
}
#endif

#define CN_PERI_ID_SUPC                 (0x00)
#define CN_PERI_ID_RSTC                 (0x01)
#define CN_PERI_ID_RTC                  (0x02)
#define CN_PERI_ID_RTT                  (0x03)
#define CN_PERI_ID_WDT                  (0x04)
#define CN_PERI_ID_PMC                  (0x05)
#define CN_PERI_ID_EEFC0                (0x06)
#define CN_PERI_ID_EEFC1                (0x07)
#define CN_PERI_ID_UART0                (0x08)
#define CN_PERI_ID_UART1                (0x09)
#define CN_PERI_ID_SMC                  (0x0A)
#define CN_PERI_ID_PIOA                 (0x0B)
#define CN_PERI_ID_PIOB                 (0x0C)
#define CN_PERI_ID_PIOC                 (0x0D)
#define CN_PERI_ID_USART0               (0x0E)
#define CN_PERI_ID_USART1               (0x0F)
#define CN_PERI_ID_REV0                 (0X10)
#define CN_PERI_ID_REV1                 (0X11)
#define CN_PERI_ID_HSMCI                (0x12)
#define CN_PERI_ID_TWI0                 (0x13)
#define CN_PERI_ID_TWI1                 (0x14)
#define CN_PERI_ID_SPI                  (0x15)
#define CN_PERI_ID_SSC                  (0x16)
#define CN_PERI_ID_TC0                  (0x17)
#define CN_PERI_ID_TC1                  (0x18)
#define CN_PERI_ID_TC2                  (0x19)
#define CN_PERI_ID_TC3                  (0x1a)
#define CN_PERI_ID_TC4                  (0x1b)
#define CN_PERI_ID_TC5                  (0x1c)
#define CN_PERI_ID_ADC                  (0x1d)
#define CN_PERI_ID_DACC                 (0x1e)
#define CN_PERI_ID_PWM                  (0x1f)
#define CN_PERI_ID_CRCU                 (0x20)
#define CN_PERI_ID_ACC                  (0x21)
#define CN_PERI_ID_UDP                  (0x22)

#endif //__CPU_PERI_H__
