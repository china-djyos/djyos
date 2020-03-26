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

#include "stdint.h"
#include "cpu_peri.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    ModuleInstall_CpuWdt(0);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip wdt"//CPU的看门狗外设驱动
//parent:"watch dog"       //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:pre-main            //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"watch dog"        //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_WDT == false )
//#warning  " cpu_onchip_wdt  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_WDT    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,1000,20000000,
#define CFG_CPUWDT_WDTCYCLE         (5*1000*1000)   //"看门狗超时时间"，单位us
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

#define CN_WDT_WDTCYCLE  5*1000*1000      //单位为微秒
#define CN_WDT_DOGNAME   "FreescaleKxxWDT"

/********************************************************************/
/*
 * Watchdog timer unlock routine. Writing 0xC520 followed by 0xD928
 * will unlock the write once registers in the WDOG so they are writable
 * within the WCT period.
 *
 * Parameters:
 * none
 ********************************************************************/
void WDT_Unlock(void)
{
    atom_high_t atom;
    //UNLOCK时序必须保证不被打断
    atom = Int_HighAtomStart();

    /* Write 0xC520 to the unlock register */
    WDOG->UNLOCK = 0xC520;
    /* Followed by 0xD928 to complete the unlock */
    WDOG->UNLOCK = 0xD928;

    Int_HighAtomEnd(atom);
}
/********************************************************************/
/*
 * Watchdog timer disable routine
 *
 * Parameters:
 * none
 *******************************************************************/
void WDT_Disable(void)
{
    /* First unlock the watchdog so that we can write to registers */
    WDT_Unlock();

    /* Clear the WDOGEN bit to disable the watchdog */
    WDOG->STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}

/********************************************************************/
/*
 * Watchdog timer enable routine
 *
 * Parameters:
 * none
 *******************************************************************/
void WDT_Enable(void)
{
    WDT_Unlock();

    WDOG->STCTRLH |= WDOG_STCTRLH_WDOGEN_MASK;
}

// =============================================================================
// 函数功能:设定WDT的喂狗周期，在周期内没有喂狗，则狗叫
// 输入参数:cycle,周期，微秒
// 返回值  :true成功false失败
// =============================================================================
void WDT_SetCycle(u32 cycle)
{
    u8 temp,wdt_clk;
    temp = (WDOG->PRESC & 0x700)>>8;
    wdt_clk = CN_BUS_CLK/((temp + 1)*Mhz);

    //WDT的时钟来源于BUS时钟，因此，其时钟为内核时钟的一半
    WDT_Unlock();
    WDOG->TOVALH = (u16)((wdt_clk * cycle) & 0xFFFF0000) >> 16;
    WDOG->STCTRLL = (u16)((wdt_clk * cycle) & 0xFFFF);
}

// =============================================================================
// 函数功能:WDT_FreescaleWdtFeed
//          Freescale喂狗函数
// 输入参数:
// 返回值  :true成功false失败
// =============================================================================
bool_t WDT_WdtFeed(void)
{
    atom_high_t atom;

    WDT_Unlock();
    //UNLOCK时序必须保证不被打断
    atom = Int_HighAtomStart();
    WDOG->REFRESH = 0xA602;
    WDOG->REFRESH = 0xB480;

    Int_HighAtomEnd(atom);

    //just for test
    GPIO_PinToggle(GPIO_PORT_A,GPIO_PIN(29));
    return true;
}

// =============================================================================
// 函数功能:当中断标志被置位后，WDT的中断发生，经过WCT时间后，系统会被RESET
// 输出参数:无
// 返回值  :
// =============================================================================
u32 WDT_ISR(ptu32_t IntLine)
{
    WDT_Unlock();

    WDOG->STCTRLL |= (1<<15);   //清中断

    return 0;
}

// =============================================================================
// 函数功能:WDT_IntInit，注册看门狗中断，FREESCALE的看门狗叫时，先发生中断，后RESET
// 输出参数:无
// 返回值  :true成功false失败
// =============================================================================
void WDT_IntInit(void)
{
    u8 IntLine = CN_INT_LINE_WDT;
    if(true == Int_Register(IntLine))
    {
        Int_Register(IntLine);
        Int_IsrConnect(IntLine,WDT_ISR);
        Int_SettoReal(IntLine);
        Int_ClearLine(IntLine);
        Int_RestoreRealLine(IntLine);
    }
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
// =============================================================================
bool_t WDT_FslInit(u32 setcycle)
{
    bool_t result;

    //初始化WDT模块
    WDT_SetCycle(CN_WDT_WDTCYCLE);
//  WDT_IntInit();

//  result = WdtHal_RegisterWdtChip(CN_WDT_DOGNAME,CN_WDT_WDTCYCLE,\
//                                  WDT_WdtFeed,NULL,NULL);
    result = WdtHal_RegisterWdtChip(CN_WDT_DOGNAME, CN_WDT_WDTCYCLE, WDT_WdtFeed);
#if(CFG_DEFEND_ON_BOOT == true)
    __BrdBoot_FeedEnd();
#endif
    return result;
}

