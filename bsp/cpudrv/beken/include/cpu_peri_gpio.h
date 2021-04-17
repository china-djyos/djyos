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
#ifndef __CPU_PERI_GPIO_H__
#define __CPU_PERI_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "gpio_pub.h"

#define PIN_LOW                         0x00
#define PIN_HIGH                        0x01

#define PIN_MODE_OUTPUT                 0x00
#define PIN_MODE_INPUT                  0x01
#define PIN_MODE_INPUT_PULLUP           0x02
#define PIN_MODE_INPUT_PULLDOWN         0x03
#define PIN_MODE_OUTPUT_OD              0x04

#define PIN_IRQ_MODE_RISING             0x00
#define PIN_IRQ_MODE_FALLING            0x01
#define PIN_IRQ_MODE_RISING_FALLING     0x02
#define PIN_IRQ_MODE_HIGH_LEVEL         0x03
#define PIN_IRQ_MODE_LOW_LEVEL          0x04

#define PIN_IRQ_DISABLE                 0x00
#define PIN_IRQ_ENABLE                  0x01

#define PIN_IRQ_PIN_NONE                -1

int ModuleInstall_GPIO(void);
int djy_gpio_irq_enable( GPIO_INDEX pin, uint32_t enabled);
int djy_gpio_attach_irq(GPIO_INDEX pin,uint32_t mode,
                            void (*hdr)(void *args), void *args);
uint32_t djy_gpio_read( GPIO_INDEX pin);
void djy_gpio_write(GPIO_INDEX pin, uint32_t value);
void djy_gpio_mode( GPIO_INDEX pin, uint32_t mode);


#define MGPIO_PIN_IN        PIN_MODE_INPUT        //普通输入模式
#define MGPIO_PIN_OUT       PIN_MODE_OUTPUT       //普通输出模式
#define MGPIO_PIN_AF        0                     //AF功能模式

#define MGPIO_PIN_PULLNONE        PIN_MODE_INPUT       //不带上下拉
#define MGPIO_PIN_PULLUP          PIN_MODE_INPUT_PULLUP       //上拉
#define MGPIO_PIN_PULLDOWM        PIN_MODE_INPUT_PULLDOWN       //下拉


s32 PIN_Init(void *str,char *data,u32 len);
u32 PIN_Get(void *str,char *data,u32 len);
void PIN_SettoHigh(void *str,char *data,u32 len);
void PIN_SettoLow(void *str,char *data,u32 len);
void PIN_PowerOn(void *str,char *data,u32 len);
void PIN_PowerOff(void *str,char *data,u32 len);

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_GPIO_H__
