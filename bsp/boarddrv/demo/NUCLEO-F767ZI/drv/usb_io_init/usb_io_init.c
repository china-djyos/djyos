//-----------------------------------------------------------------------------
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
#include <stdint.h>
#include <stddef.h>
#include <cpu_peri_gpio.h>
#include <stm32f7xx_hal.h>
#include <systime.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束
//%$#@describe      ****组件描述开始
//component name:"usb io init"//usbio的初始化
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp               //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"        //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始

extern const Pin USB_FS[2];
extern const Pin USB_HS[2];
extern const Pin HUB_RST[1];
extern const Pin USB_LED1[1]; // USB运行状态指示灯
extern const Pin M4G_RST[1];
extern const Pin M4G_POWER[1];
extern const Pin HUB_POWER[1];
extern const Pin TOUCH_POWER[1];
extern void RunIboot(void);
#define USB_4G_PORT          1
#define USB_HUB_PORT         2
// ============================================================================
// 功能：USB IAP状态灯亮
// 参数：
// 返回：
// ===========================================================================
void USB_IOs(void)
{

}

// ============================================================================
// 功能：USB IAP状态灯亮
// 参数：
// 返回：
// ============================================================================
void USB_IAP_TurnOnLED(void)
{

}

// ============================================================================
// 功能：USB IAP状态灯灭
// 参数：
// 返回：
// ============================================================================
void USB_IAP_TurnOffLED(void)
{

}

void USB_UDiskReset(void)
{

}
// ============================================================================
// 功能：USB设备复位逻辑
// 参数：bHostID：usb的端口号；bDevID：该端口上的设备号
// 返回：
// 备注：端口1接着4G模块，端口2接着HUB
// ============================================================================
void USB_DeviceReset(u8 bHostID, u8 bDevID)
{

}

// ============================================================================
// 功能：
// 参数：
// 返回：
// ============================================================================
void USB_UserInstall(const char *target_fs,u8 bHostID)
{

}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void USB_FS_GPIO_Init(void)
{

}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void USB_HS_GPIO_Init(void)
{

}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void M4G_PowerSet(void)
{

}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void M4G_PowerClr(void)
{

}
