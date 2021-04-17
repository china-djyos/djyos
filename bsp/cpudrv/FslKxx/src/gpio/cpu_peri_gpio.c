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
// 文件名     ：cpu_peri_gpio.c
// 模块描述: GPIO模块的初始化的操作，操作包括输入读取和输出配置等
// 模块版本: V1.00
// 创建人员: hm
// 创建时间: 07/06.2014
// =============================================================================

#include "cpu_peri.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip gpio"//gpio操作函数集
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                     //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                  //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                    //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                    //初始化时机，可选值：early，medium，later, pre-main。
                                    //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                    //选中该组件时，被依赖组件将强制选中，
                                    //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"             //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                    //选中该组件时，被依赖组件不会被强制选中，
                                    //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                    //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO == false )
//#warning  " cpu_onchip_gpio  组件参数未配置，使用默认配置"
//%$#@target = header              //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

static GPIO_Type volatile * const PTR_GPIO_REG[] = {(GPIO_Type *)PTA_BASE,
                                            (GPIO_Type *)PTB_BASE,
                                            (GPIO_Type *)PTC_BASE,
                                            (GPIO_Type *)PTD_BASE,
                                            (GPIO_Type *)PTE_BASE,
                                            (GPIO_Type *)PTF_BASE};

// =============================================================================
// 功能：配置GPIO端口引脚的功能为输入或者输出，配置该功能前，相应的port口对应的引脚应
//       已经配置为GPIO功能，配置过程由port模块实现
// 参数：port,配置的端口号，PORTA~PORTF分别对应宏定义GPIO_PORT_A~GPIO_PORT_F
//       msk,相应端口上对应的引脚，由宏GPIO_PIN(X)实现，x为引脚号
//       func_no,输入或输出功能，宏GPIO_FUNC_INPUT/GPIO_FUNC_OUTPUT
// 返回：NONE
// =============================================================================
void GPIO_PinConfig(u32 port,u32 msk,u32 func_no)
{
    if((port > GPIO_PORT_MAX) || (msk > GPIO_PIN_MAX))
        return ;

    //配置相应的port口对应的引脚为GPIO功能---todo

    if(func_no == GPIO_FUNC_OUTPUT)
    {
        PTR_GPIO_REG[port]->PDDR |= (1<<msk);
    }
    else if(func_no == GPIO_FUNC_INPUT)
    {
        PTR_GPIO_REG[port]->PDDR &= ~(1<<msk);
    }
    else
    {
        return;
    }
}

// =============================================================================
// 功能：当port配置为输入时，可通过该API读取引脚上的数值，返回port引脚上输入值
// 参数：port,配置的端口号，PORTA~PORTF分别对应宏定义GPIO_PORT_A~GPIO_PORT_F
// 返回：port端口上的数值
// =============================================================================
u32  GPIO_DataGet(u32 port)
{
    if(port > GPIO_PORT_MAX)
        return 0;
    return PTR_GPIO_REG[port]->PDIR;
}

// =============================================================================
// 功能：端口配置为输出时，将数值输出到端口上
// 参数：port,配置的端口号，PORTA~PORTF分别对应宏定义GPIO_PORT_A~GPIO_PORT_F
//       data,输入到端口上的值
// 返回：无
// =============================================================================
void GPIO_DataOut(u32 port,u32 data)
{
    if(port > GPIO_PORT_MAX)
        return;

    PTR_GPIO_REG[port]->PDOR = data;
}

// =============================================================================
// 功能：设置引脚输出为1
// 参数：port,配置的端口号
//       msk,相应端口上对应的引脚
// 返回：实际配置的MCG输出频率
// =============================================================================
void GPIO_PinSet(u32 port,u32 msk)
{
    if((port > GPIO_PORT_MAX) || (msk > GPIO_PIN_MAX))
        return ;

    PTR_GPIO_REG[port]->PSOR |= (1<<msk);
}

// =============================================================================
// 功能：清除引脚输出为0
// 参数：port,配置的端口号
//       msk,相应端口上对应的引脚
// 返回：无
// =============================================================================
void GPIO_PinClear(u32 port,u32 msk)
{
    if((port > GPIO_PORT_MAX) || (msk > GPIO_PIN_MAX))
        return ;

    PTR_GPIO_REG[port]->PCOR |= (1<<msk);
}

// =============================================================================
// 功能：翻转输出引脚的电平
// 参数：port,配置的端口号
//       msk,相应端口上对应的引脚
// 返回：无
// =============================================================================
void GPIO_PinToggle(u32 port,u32 msk)
{
    if((port > GPIO_PORT_MAX) || (msk > GPIO_PIN_MAX))
        return ;

    PTR_GPIO_REG[port]->PTOR |= (1<<msk);
}

// =============================================================================
// 功能：配置时钟输出到相应的端口上
// 参数：port,配置的端口号
// 返回：无
// =============================================================================
void GPIO_PowerOn(u32 port)
{
//  if(port > GPIO_PORT_MAX)
//      return;
//  switch(port)
//  {
//  case GPIO_PORT_A:
//      SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK ;
//      break;
//  case GPIO_PORT_B:
//      SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK ;
//      break;
//  case GPIO_PORT_C:
//      SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK ;
//      break;
//  case GPIO_PORT_D:
//      SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK ;
//      break;
//  case GPIO_PORT_E:
//      SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK ;
//      break;
//  case GPIO_PORT_F:
//      SIM_SCGC5 |= SIM_SCGC5_PORTF_MASK ;
//      break;
//  default:
//      break;
//  }
}

// =============================================================================
// 功能：配置时钟不输出到相应的端口上，用于低功耗设计
// 参数：port,配置的端口号
// 返回：无
// =============================================================================
void GPIO_PowerOff(u32 port)
{
//  if(port > GPIO_PORT_MAX)
//      return;
//  switch(port)
//  {
//  case GPIO_PORT_A:
//      SIM_SCGC5 &= ~SIM_SCGC5_PORTA_MASK ;
//      break;
//  case GPIO_PORT_B:
//      SIM_SCGC5 &= ~SIM_SCGC5_PORTB_MASK ;
//      break;
//  case GPIO_PORT_C:
//      SIM_SCGC5 &= ~SIM_SCGC5_PORTC_MASK ;
//      break;
//  case GPIO_PORT_D:
//      SIM_SCGC5 &= ~SIM_SCGC5_PORTD_MASK ;
//      break;
//  case GPIO_PORT_E:
//      SIM_SCGC5 &= ~SIM_SCGC5_PORTE_MASK ;
//      break;
//  case GPIO_PORT_F:
//      SIM_SCGC5 &= ~SIM_SCGC5_PORTF_MASK ;
//      break;
//  default:
//      break;
//  }
}
