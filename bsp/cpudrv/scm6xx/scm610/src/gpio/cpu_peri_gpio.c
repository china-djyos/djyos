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
// =============================================================================

// 文件名     ：cpu_peri_gpio.h
// 模块描述: scm6xx的GPIO驱动
// 模块版本:
// 创建人员:
// 创建时间: 10/28.2016
// =============================================================================
#include "stdint.h"
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
//attribute:bsp                  //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable               //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                 //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                 //初始化时机，可选值：early，medium，later, pre-main。
                                 //表示初始化时间，分别是早期、中期、后期
//dependence                   //该组件的依赖组件名（可以是none，表示无依赖组件），
                                 //选中该组件时，被依赖组件将强制选中，
                                 //如果依赖多个组件，则依次列出
//weakdependence:"none"          //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                 //选中该组件时，被依赖组件不会被强制选中，
                                 //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                 //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO == false )
//#warning  " cpu_onchip_gpio  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#include "stdint.h"
#include "cpu_peri.h"
#include "stddef.h"

typedef struct
{
    volatile u32 DR;
    volatile u32 DIR;
    volatile u32 INTER;
    volatile u32 INTMR;
    volatile u32 INTSR;
    volatile u32 DBSYR;
    volatile u32 INTCR;
    volatile u32 ETPR;
    volatile u32 PULLEN;
    volatile u32 PULLTYPE;
}tagGpioReg;

#define CN_PIN_MAX (113)

// =============================================================================
// 功能: GPIO 输入输出方向配置
// 参数: Mode
//     EN_INPUT_MODE  -配置为输入模式
//     EN_OUTPUT_MODE -配置为输出模式
//     PortNum - 要配置的端口号
//               端口号为0~113之间
//
// 返回: 配置成功返回true 配置失败返回false
// =============================================================================

bool_t Gpio_DirConfig(const u8 Mode,const u8 PortNum)
{
    bool_t bRet = false;

    if(Mode != EN_INPUT_MODE && Mode != EN_OUTPUT_MODE)
    {
        return bRet;
    }

    if(PortNum > CN_PIN_MAX)
    {
        return bRet;
    }

    switch(Mode)
    {
        case EN_INPUT_MODE:
             GPIO_Configure_Dir(PortNum,EN_INPUT_MODE);
             bRet = true;
             break;
        case EN_OUTPUT_MODE:
             GPIO_Configure_Dir(PortNum,EN_OUTPUT_MODE);
             bRet = true;
             break;
        default:;break;
    }

    return bRet;
}


// =============================================================================
// 功能: GPIO 输出电平配置
// 参数: Level
//     EN_GPIO_LEV_L  -输出低电平
//     EN_GPIO_LEV_H  -输出高电平
//     PortNum - 要配置的端口号
//               端口号为0~113之间
//
// 返回: 配置成功返回true 配置失败返回false
// =============================================================================

void GPIO_SettoHigh(u32 port,u32 msk)
{
    GPIO_Write_Out(port, EN_GPIO_LEV_H);
}


void GPIO_SettoLow(u32 port,u32 msk)
{
    GPIO_Write_Out(port, EN_GPIO_LEV_L);
}

// =============================================================================
// 功能: GPIO 读取端口电平
// 参数:
//     PortNum - 要配置的端口号
//               端口号为0~113之间
//
// 返回: 成功:返回对应端口电平  失败:返回-1
// =============================================================================

char Gpio_ReadLevel(u8 PortNum)
{
    char byRet = CN_RORT_READ_FAILED;

    if(PortNum > CN_PIN_MAX)
    {
        return byRet;
    }

    byRet = GPIO_Read_In(PortNum);

    return byRet;
}

// =============================================================================
// 功能: 外部中断使能
// 参数:
//     PortNum - 要配置的端口号
//               端口号为0~113之间
//
// 返回:
// =============================================================================

bool_t Gpio_ExIntEnable(u8 PortNum)
{
    bool_t bRet = false;

    if(PortNum > CN_PIN_MAX)
    {
        return bRet;
    }

    GPIO_Enable_Int(PortNum,1);

    return true;
}

// =============================================================================
// 功能: 外部中断失能
// 参数:
//     PortNum - 要配置的端口号
//               端口号为0~113之间
//
// 返回:
// =============================================================================

bool_t Gpio_ExIntDisable(u8 PortNum)
{
    bool_t bRet = false;

    if(PortNum > CN_PIN_MAX)
    {
        return bRet;
    }

    GPIO_Enable_Int(PortNum,0);

    return true;
}

// =============================================================================
// 功能: 外部中断屏蔽或不屏蔽
// 参数:
//     PortNum - 要配置的端口号
//               端口号为0~113之间
//
// 返回:
// =============================================================================

bool_t Gpio_IntMask(u8 PortNum,u8 MaskCode)
{
    bool_t bRet = false;

    if(PortNum > CN_PIN_MAX)
    {
        return bRet;
    }

    if(MaskCode != EN_GPIO_UMASK && MaskCode != EN_GPIO_MASK)
    {
        return bRet;
    }

    GPIO_Mask_Int(PortNum,MaskCode);

    return true;
}


// =============================================================================
// 功能: 清中断标志
// 参数:
//     PortNum - 要配置的端口号
//               端口号为0~113之间
//
// 返回:
// =============================================================================

bool_t Gpio_ClearIntFlag(u8 PortNum)
{
    bool_t bRet = false;

    if(PortNum > CN_PIN_MAX)
    {
        return bRet;
    }

    GPIO_Clear_Int_Flag(PortNum);

    return true;
}


// =============================================================================
// 功能: 外部中断屏蔽或不屏蔽
// 参数:
//     PortNum - 要配置的端口号
//               端口号为0~113之间
//
// 返回:
// =============================================================================

bool_t Gpio_IntModeConfig(u8 PortNum,u8 IntMode)
{
    bool_t bRet = false;

    if(PortNum > CN_PIN_MAX)
    {
        return bRet;
    }

    if(IntMode > EN_MODE_END)
    {
        return bRet;
    }

    GPIO_Configure_Int_Mode(PortNum,IntMode);

    return true;
}



// =============================================================================
//功能: 把某gpio port整体读入
//参数: port，被操作的port编号，比如要操作GPIOA,则port=GPIO_A
//返回: 读得的数据
// =============================================================================
u32 GPIO_GetData(u32 port)
{

}

// =============================================================================
// 功能: 把数据整体输出到某gpio port
// 参数: port，被操作的port编号，比如要操作GPIOA,则port=GPIO_A
//      data，待输出的数据
// 返回: 无
// =============================================================================
void GPIO_OutData(u32 port,u32 data)
{

}



// =============================================================================
// 功能: 打开GPIOA-GPIOI的时钟输出
// 参数：port,端口号，如GPIO_A
// 返回：无
// =============================================================================
void GPIO_PowerOn(u32 port)
{

}

// =============================================================================
// 功能: 为了节能，降低功耗，直接关闭GPIOA-GPIOI的时钟输出
// 参数：port,端口号，如GPIO_A
// 返回：无
// =============================================================================
void GPIO_PowerOff(u32 port)
{

}















