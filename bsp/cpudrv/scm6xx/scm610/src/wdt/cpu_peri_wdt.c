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

// 文件名     ：cpu_peri_wdt.c
// 模块描述: scm610芯片自带的看门狗初始化
// 创建时间: 14/2.2019
// 创建人    : huzb
// =============================================================================
#include <stdlib.h>
#include <stdint.h>
#include "cpu_peri.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_WWDG(u32 time);
//    ModuleInstall_WWDG(0);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip wdt"//CPU的窗口看门狗驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                  //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable               //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                 //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:later               //初始化时机，可选值：early，medium，later, pre-main。
                                 //表示初始化时间，分别是早期、中期、后期
//dependence:"watch dog"   //该组件的依赖组件名（可以是none，表示无依赖组件），
                                 //选中该组件时，被依赖组件将强制选中，
                                 //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"          //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                 //选中该组件时，被依赖组件不会被强制选中，
                                 //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                 //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_WDT == false )
//#warning  " cpu_onchip_wdt  组件参数未配置，使用默认配置"
//%$#@target = header              //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_WDT    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,,
#define CFG_WDT_WDTCYCLE        (3*1000*1000)       //"看门狗超时时间"，单位us
#define CFG_BOOT_TIME_LIMIT         30000000        //"启动加载超限时间",允许保护启动加载过程才需要配置此项
//%$#@enum,true,false,
#define CFG_DEFEND_ON_BOOT          false          //"保护启动过程",启动加载过程如果出现死机，看门狗将复位
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

typedef struct
{
    volatile u32 CTRL;
    volatile u32 CLR;
}tagWatReg;

#define CN_WAT_REG_BASE (0x40700000)

static volatile tagWatReg* sgpt_WatReg = (volatile tagWatReg*)CN_WAT_REG_BASE;

#define CN_WWDG_WDTCYCLE  32000*mS               //WWDG最大为58mS

#define CN_WDT_DOGNAME   "SCMWDG"


// =============================================================================
// 函数功能:使能开门狗
// 输入参数:
// 返回值  :true成功false失败
// =============================================================================
bool_t Wdt_Enable(void)
{
    sgpt_WatReg->CTRL |= (1<<0);

    return true;
}

// =============================================================================
// 函数功能:IWDG_Stm32WdtFeed
//          IWDG_Stm32WdtFeed的喂狗函数
// 输入参数:
// 返回值  :true成功false失败
// =============================================================================
bool_t Wdt_Feed(void)
{
    //先往Passad写入5A 在进行清操作
    sgpt_WatReg->CTRL &= ~(0xff << 8);
    sgpt_WatReg->CTRL |= (0x5A << 8);
    //进行喂狗操作
    sgpt_WatReg->CLR = 0xa5;

    return true;
}

// =============================================================================
// 函数功能:IWDG_PreSet,IWDG时钟源为内部RC，为40KHz
//          IWDG_PreSet设置时钟分步系数
// 输入参数:
// 返回值  :true成功false失败
// =============================================================================
bool_t Wdt_PreSet(u8 Value)
{
    return true;
}

// =============================================================================
// 函数功能:   WatDog_ReloadSet
//          设置看门狗溢出复位周期
// 输入参数:   value
//          芯片仅仅支持以下几种设置:
//          SYSWDT_OTSET_3906us
//          SYSWDT_OTSET_7812us
//          SYSWDT_OTSET_15625us
//          SYSWDT_OTSET_31250us
//          SYSWDT_OTSET_62500us
//          SYSWDT_OTSET_125ms
//          SYSWDT_OTSET_250ms
//          SYSWDT_OTSET_500ms
//          SYSWDT_OTSET_1s
//          SYSWDT_OTSET_2s
//          SYSWDT_OTSET_4s
//          SYSWDT_OTSET_8s
//          SYSWDT_OTSET_12s
//          SYSWDT_OTSET_16s
//          SYSWDT_OTSET_24s
//          SYSWDT_OTSET_32s
//
// 返回值  :true成功false失败
// =============================================================================

bool_t Wdt_ReloadSet(u16 value)
{
    sgpt_WatReg->CTRL &= ~(0xf << 4);
    sgpt_WatReg->CTRL |= value;

}

//**************************************************//
//todo：这三个函数用于实现保护加载过程，使用的时候需补全
u32 __FeedDog_Isr(ptu32_t intline)
{
    return false;
}
bool_t __BrdBoot_FeedStart(u32 bootfeedtime)
{
    return false;
}

bool_t __BrdBoot_FeedEnd(void)
{
    return false;
}
//**************************************************//

// =============================================================================
// 功能：板上看门狗芯片初始化，此函数在软看门狗组件后面初始化，如果启动了“防护启动加载过程”
//      的功能，本函数调用后，将停止自动喂狗。
// 输入参数:
// 返回值  :true成功false失败
// 说明：
// =============================================================================

bool_t Wdt_Initial(u32 setcycle)
{
    bool_t result;

    //复位使能
    sgpt_WatReg->CTRL |= (1<<1);

    //配置重装载值，即喂狗后，装载值,配置为5分频
    Wdt_ReloadSet(SYSWDT_OTSET_1s);
//  result = WdtHal_RegisterWdtChip(CN_WDT_DOGNAME,CFG_WDT_WDTCYCLE,\
//                                  Wdt_Feed,NULL,NULL);

    result = WdtHal_RegisterWdtChip(CN_WDT_DOGNAME, CFG_WDT_WDTCYCLE, Wdt_Feed);
#if(CFG_DEFEND_ON_BOOT == true)
    __BrdBoot_FeedEnd();
#endif
    Wdt_Enable();
    return result;
}
