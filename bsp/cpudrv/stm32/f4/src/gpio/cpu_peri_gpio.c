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
// 模块描述: STM32F4xx的GPIO驱动
// 模块版本: V1.10
// 创建人员: hm
// 创建时间: 08/07.2015
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


#define tagGpioReg GPIO_TypeDef

static tagGpioReg volatile * const tg_GPIO_Reg[] = {GPIOA,GPIOB,GPIOC,GPIOD,
                                                GPIOE,GPIOF,GPIOG,GPIOH,GPIOI};

// =============================================================================
// 功能: GPIO引脚配置，包括引脚的模式、输入类型、速度、上下拉类型等
// 参数: port，被操作的port编号，比如要操作GPIOA,PIN5,则port=GPIO_A
//       Msk，操作的掩码，如操作的是GPIOA,PIN5，则msk = (1<<5).
//       Mode,模式，分为输入、输出、模拟输入、AF复用功能
//       OutType,推挽输出或开漏输出
//       Speed,速度，如GPIO_SPEED_50M
//       PUPD,上拉或下拉
// 返回: 无
// =============================================================================
void GPIO_CfgPinFunc(u32 port,u32 Msk,u32 Mode,
                        u32 OutType,u32 Speed,u32 PUPD)
{
    if(port > GPIO_I)
        return;
    u32 pinpos=0,pos=0,curpin=0;
    for(pinpos=0;pinpos<16;pinpos++)
    {
        pos=1<<pinpos;  //一个个位检查
        curpin=Msk&pos;//检查引脚是否要设置
        if(curpin==pos) //需要设置
        {
            tg_GPIO_Reg[port]->MODER&=~(3<<(pinpos*2)); //先清除原来的设置
            tg_GPIO_Reg[port]->MODER|=Mode<<(pinpos*2); //设置新的模式
            if((Mode==0X01)||(Mode==0X02))  //如果是输出模式/复用功能模式
            {
                tg_GPIO_Reg[port]->OSPEEDR&=~(3<<(pinpos*2));   //清除原来的设置
                tg_GPIO_Reg[port]->OSPEEDR|=(Speed<<(pinpos*2));//设置新的速度值
                tg_GPIO_Reg[port]->OTYPER&=~(1<<pinpos) ;       //清除原来的设置
                tg_GPIO_Reg[port]->OTYPER|=OutType<<pinpos;     //设置新的输出模式
            }
            tg_GPIO_Reg[port]->PUPDR&=~(3<<(pinpos*2)); //先清除原来的设置
            tg_GPIO_Reg[port]->PUPDR|=PUPD<<(pinpos*2); //设置新的上下拉
        }
    }
}

// =============================================================================
// 功能：设置利用功能号
// 参数：port，被操作的port编号，比如要操作GPIOA,PIN5,则port=GPIO_A
//      pinnum,IO号，如GPIOA,PIN5,则pinnum = 5
//      af_no，利用功能号
// 返回：无
// =============================================================================
u32 GPIO_AFSet(u32 port,u32 pinnum,u32 af_no)
{
    if(port > GPIO_I)
        return 0;
    tg_GPIO_Reg[port]->AFR[pinnum>>3]&=~(0X0F<<((pinnum&0X07)*4));
    tg_GPIO_Reg[port]->AFR[pinnum>>3]|=(u32)af_no<<((pinnum&0X07)*4);
    return 1;
}

// =============================================================================
//功能: 把某gpio port整体读入
//参数: port，被操作的port编号，比如要操作GPIOA,则port=GPIO_A
//返回: 读得的数据
// =============================================================================
u32 GPIO_GetData(u32 port)
{
    if(port >GPIO_I)
        return 0;
    return tg_GPIO_Reg[port]->IDR;
}

// =============================================================================
// 功能: 把数据整体输出到某gpio port
// 参数: port，被操作的port编号，比如要操作GPIOA,则port=GPIO_A
//      data，待输出的数据
// 返回: 无
// =============================================================================
void GPIO_OutData(u32 port,u32 data)
{
    if(port >GPIO_I)
        return;
    tg_GPIO_Reg[port]->ODR = data;
}

// =============================================================================
//功能: 在port中msk中是1的bit对应的位输出高电平
//参数: port，被操作的port编号，比如要操作GPIOA,则port=GPIO_A
//      mask，port中需要输出高电平的位掩码，比如操作GPIOA,PIN5，msk = (1<<5)
//返回: 无
// =============================================================================
void GPIO_SettoHigh(u32 port,u32 msk)
{
    if(port >GPIO_I)
        return;
    tg_GPIO_Reg[port]->BSRR = msk & 0xffff;
}

// =============================================================================
//功能: 在port中msk中是1的bit对应的位输出低电平
//参数: port，被操作的port编号，比如要操作GPIOA,则port=GPIO_A
//      mask，port中需要输出高电平的位掩码,比如操作PIOA,PIN5，msk = (1<<5)
//返回: 无
// =============================================================================
void GPIO_SettoLow(u32 port,u32 msk)
{
    if(port > GPIO_I)
        return;
    tg_GPIO_Reg[port]->BSRR = ((msk & 0xffff)<<16);
}

// =============================================================================
// 功能: 打开GPIOA-GPIOI的时钟输出
// 参数：port,端口号，如GPIO_A
// 返回：无
// =============================================================================
void GPIO_PowerOn(u32 port)
{
    //stm32没有关闭gpio电源的功能，只能关掉或打开他们的时钟。
    if(port > GPIO_I)
        return ;
    RCC->AHB1ENR |= (1<<port);
}

// =============================================================================
// 功能: 为了节能，降低功耗，直接关闭GPIOA-GPIOI的时钟输出
// 参数：port,端口号，如GPIO_A
// 返回：无
// =============================================================================
void GPIO_PowerOff(u32 port)
{
    //stm32没有关闭gpio电源的功能，只能关掉或打开他们的时钟。
    if(port > GPIO_I)
        return ;
    RCC->AHB1ENR &= ~(1<<port);
}



