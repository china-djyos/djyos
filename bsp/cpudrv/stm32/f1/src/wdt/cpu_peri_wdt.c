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
// 模块描述: WDT模块，使用了WDT组件管理看门狗
// 模块版本: V1.10
// 创建人员: hm
// 创建时间: 28/08.2014
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "systime.h"
#include "cpu_peri.h"
#include <int.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    #if CFG_IWDG_ENABLE==1
//    extern bool_t IWDG_Stm32Initial(u32 setcycle);
//    IWDG_Stm32Initial(CFG_WDT_WDTCYCLE);
//    #endif
//
//    #if CFG_WWDG_ENABLE==1
//    extern bool_t WWDG_STM32Init(u32 setcycle);
//    WWDG_STM32Init(CFG_WWDG_WDTCYCLE);
//    #endif
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
//%$#@num,0,,
#define CFG_IWDT_WDTCYCLE       5000000      //"IWDT看门狗超时时间",
#define CFG_WWDG_WDTCYCLE       50000        //"WWDG看门狗超时时间",
//%$#@enum,TIM6,TIM7,
#define CFG_WDT_TIM             TIM6    //启动加载用到的定时器（如果CFG_DEFEND_ON_BOOT没选，该选项是无用的）
//%$#@enum,true,false,
#define CFG_WWDG_ENABLE         false            //"是否配置使用WWDG",
#define CFG_IWDG_ENABLE         false            //"是否配置使用IWDG",
#define CFG_DEFEND_ON_BOOT      false            //"保护启动过程",启动加载过程如果出现死机，看门狗将复位（如果CFG_WWDG_ENABLE和CFG_IWDG_ENABLE都没选，该选项是无用的）
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏"s
//%$#@free,s
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

//#define CFG_WDT_WDTCYCLE  5000*mS//200*1000      //单位为微秒
//#define CFG_WWDG_WDTCYCLE  50*mS                 //WWDG最大为58mS
#define CN_WDT_DOGNAME   "STM32IWDG"
#define CN_WWDG_DOGNAME   "STM32WWDG"

#define CN_IWDG_BASE  0x40003000
#define CN_WWDG_BASE  0x40002C00
#define STM_IWDG      ((tagIWDGReg*)CN_IWDG_BASE)
#define STM_WWDG      ((tagWWDGReg*)CN_WWDG_BASE)

#define CN_PRE_VALUE   5
static u16 s_prevalue[]={4,8,16,32,64,128,256,256};


bool_t __BrdBoot_FeedEnd(void);
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ------------------------ STM32 独立开门狗 -----------------------------------
// ----------------------------------------------------------------------------
// =============================================================================
// 函数功能:使能开门狗
// 输入参数:
// 返回值  :true成功false失败
// =============================================================================
bool_t IWDG_Stm32Enable(void)
{
    STM_IWDG->IWDG_KR = 0xCCCC;
    return true;
}

// =============================================================================
// 函数功能:IWDG_Stm32WdtFeed
//          IWDG_Stm32WdtFeed的喂狗函数
// 输入参数:
// 返回值  :true成功false失败
// =============================================================================
bool_t IWDG_Stm32WdtFeed(void)
{
    STM_IWDG->IWDG_KR = 0xAAAA;
    return true;
}
// =============================================================================
// 函数功能:IWDG_PreSet,IWDG时钟源为内部RC，为40KHz
//          IWDG_PreSet设置时钟分步系数
// 输入参数:
// 返回值  :true成功false失败
// =============================================================================
bool_t IWDG_Stm32PreSet(u8 Value)
{
    //配置IWDG模式
    if(!(STM_IWDG->IWDG_SR & IWDG_SR_PVU_MASK))
    {
        STM_IWDG->IWDG_KR = 0x5555;
        STM_IWDG->IWDG_PR = IWDG_PR_PR & Value;
        //wait finish
        while((STM_IWDG->IWDG_SR & IWDG_SR_PVU_MASK));
        return true;
    }
    else
        return false;
}

// =============================================================================
// 函数功能:IWDG_Stm32ReloadSet,设置重装载值
//          IWDG_Stm32ReloadSet硬件看门狗初始化
// 输入参数:
// 返回值  :true成功false失败
// =============================================================================
bool_t IWDG_Stm32ReloadSet(u16 value)
{
    if(!(STM_IWDG->IWDG_SR & IWDG_SR_RVU_MASK))
    {
        STM_IWDG->IWDG_KR = 0x5555;
        STM_IWDG->IWDG_RLR = IWDG_RLR_RL & value;
        //wait finish
        while((STM_IWDG->IWDG_SR & IWDG_SR_RVU_MASK));
        return true;
    }
    else
        return false;
}

// =============================================================================
// 功能：板上看门狗芯片初始化，此函数在软看门狗组件后面初始化，如果启动了“防护启动加载过程”
//      的功能，本函数调用后，将停止自动喂狗。
// 输入参数:
// 返回值  :true成功false失败
// 说明：IWDG不可用，配置完PR和RL后，STM_IWDG->IWDG_SR相应位总是为1
// =============================================================================
bool_t IWDG_Stm32Initial(void)
{
    bool_t result;
    pg_rcc_reg->CSR |= (1<<0);//turn on LSI
    while(!(pg_rcc_reg->CSR & (1<<1)));

    IWDG_Stm32PreSet(CN_PRE_VALUE);     //配置时钟预分频，时钟源为40KHZ
    //配置重装载值，即喂狗后，装载值,配置为5分频
    IWDG_Stm32ReloadSet((40/s_prevalue[CN_PRE_VALUE]) * (CFG_IWDT_WDTCYCLE/mS));

//  result = WdtHal_RegisterWdtChip(CN_WDT_DOGNAME,CFG_IWDT_WDTCYCLE,\
//                                  IWDG_Stm32WdtFeed,NULL,NULL);

    result = WdtHal_RegisterWdtChip(CN_WDT_DOGNAME, CFG_IWDT_WDTCYCLE, IWDG_Stm32WdtFeed);
#if(CFG_DEFEND_ON_BOOT == true)
    __BrdBoot_FeedEnd();
#endif
    IWDG_Stm32Enable();
    return result;
}

bool_t WWDG_STM32WdtFeed(void)
{
    STM_WWDG->WWDG_CR |= 0x7F;
    return true;
}

//PCLK1=36M时钟，CK计时器时钟(PCLK1除以4096)除以8，为1.09，即WWDG计数器每
//1.09mS计数一次，WWDG看门狗最大喂狗时间为58mS
bool_t WWDG_STM32Init(void)
{
    bool_t result;
    pg_rcc_reg->APB1ENR |= (1<<11);//enable wwdg
    STM_WWDG->WWDG_CR &= ~(1<<7);//WDGA
    STM_WWDG->WWDG_CFR |= (3<<7);//WDGTB0,1
    WWDG_STM32WdtFeed();

//  result = WdtHal_RegisterWdtChip(CN_WWDG_DOGNAME,CFG_WWDG_WDTCYCLE,\
//                          WWDG_STM32WdtFeed,NULL,NULL);
    result = WdtHal_RegisterWdtChip(CN_WWDG_DOGNAME, CFG_WWDG_WDTCYCLE, WWDG_STM32WdtFeed);
#if(CFG_DEFEND_ON_BOOT == true)
    __BrdBoot_FeedEnd();
#endif
    STM_WWDG->WWDG_CR |= (1<<7);//使能看门狗
    return result;
}

bool_t BrdWdt_FeedDog(void)
{
#if(CFG_IWDG_ENABLE == true)
    IWDG_Stm32WdtFeed();
#endif

#if(CFG_WWDG_ENABLE == true)
    WWDG_STM32WdtFeed();
#endif

    return (TRUE); // 空函数
}
#if(CFG_DEFEND_ON_BOOT == true)

u32 __FeedDog_Isr(ptu32_t intline)
{
    CFG_WDT_TIM->CNT = 0;
    CFG_WDT_TIM->SR = 0;//清中断标志
    BrdWdt_FeedDog();
    Int_ClearLine(intline);
    return 1;
}
bool_t __BrdBoot_FeedStart(u32 bootfeedtime)
{

    u8 irqline = CN_INT_LINE_TIM6;

#if(CFG_WWDG_ENABLE == true)
    pg_rcc_reg->APB1ENR |= (1<<11);//enable wwdg
    STM_WWDG->WWDG_CR &= ~(1<<7);//WDGA
    STM_WWDG->WWDG_CFR |= (3<<7);//WDGTB0,1
    WWDG_STM32WdtFeed();
    STM_WWDG->WWDG_CR |= (1<<7);//使能看门狗
#endif

#if(CFG_IWDG_ENABLE == true)
    pg_rcc_reg->CSR |= (1<<0);//turn on LSI
    while(!(pg_rcc_reg->CSR & (1<<1)));
    IWDG_Stm32PreSet(CN_PRE_VALUE);     //配置时钟预分频，时钟源为40KHZ
    //配置重装载值，即喂狗后，装载值,配置为5分频
    IWDG_Stm32ReloadSet((40/s_prevalue[CN_PRE_VALUE]) * (CFG_IWDT_WDTCYCLE/mS));

    IWDG_Stm32Enable();
#endif

    pg_rcc_reg->APB1ENR |= (1<<4)|(1<<5);//时钟使能,timer6/7的使能位为比特4/5
    CFG_WDT_TIM->CR1 &= ~(TIM_CR1_CEN); //禁止TIMER
    CFG_WDT_TIM->CR1 |= TIM_CR1_ARPE | TIM_CR1_DIR;//自动重装
    CFG_WDT_TIM->DIER |= TIM_DIER_UIE;//使能更新中断
    CFG_WDT_TIM->PSC = 4000-1;//分频系数 为零 不分频(1/108)1uS
    CFG_WDT_TIM->ARR = 27000;//定时器预装初值
    Int_Register(irqline);
    Int_IsrConnect(irqline,__FeedDog_Isr);
    Int_SettoReal(irqline);
    Int_ClearLine(irqline);
    Int_RestoreRealLine(irqline);
    CFG_WDT_TIM->CR1 |= (TIM_CR1_CEN); //使能TIMER
    return true;
}

bool_t __BrdBoot_FeedEnd(void)
{
    u8 irqline = CN_INT_LINE_TIM6;
    BrdWdt_FeedDog();
    CFG_WDT_TIM->CR1 &=~(TIM_CR1_CEN); //禁止TIMER
    Int_SaveRealLine(irqline);
    Int_IsrDisConnect(irqline);
    Int_UnRegister(irqline);
//    RCC->APB1ENR &=(~RCC_APB1ENR_TIM12EN);
    pg_rcc_reg->APB1ENR &= (~((1<<4)|(1<<5)));//时钟使能,timer6/7的使能位为比特4/5
    return true;
}
#endif
