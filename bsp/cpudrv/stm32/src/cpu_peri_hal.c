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
//所属模块: bsp
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: HAL需要使用到的接口函数,stm32的hal库的延时函数有问题，如果在ISR中调用，或者
//         在关中断（关调度）的情况下调用，会死循环。因此在这里重新实现。
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include <stdio.h>
#include <stddef.h>
#include "cpu_peri.h"
#include <os.h>
#include <board-config.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

#ifdef USE_HAL_DRIVER

//==============================================================================
//HAL_RCC会用到SystemCoreClock，因为DJYOS使用自己的时钟初始化过程，用户使用hal库时，
//就会没有定义，因此需此处定义和赋值
//例如ETH网络驱动中，stm32f7xx_hal_eth.c中会调用
uint32_t SystemCoreClock = CN_CFG_MCLK;         //HAL_RCC会用到此量

#if(CFG_STM32_HAL_TIMER == 1)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM1;
#elif(CFG_STM32_HAL_TIMER == 2)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM2;
#elif(CFG_STM32_HAL_TIMER == 3)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM3;
#elif(CFG_STM32_HAL_TIMER == 4)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM4;
#elif(CFG_STM32_HAL_TIMER == 5)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM5;
#elif(CFG_STM32_HAL_TIMER == 6)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM6;
#elif(CFG_STM32_HAL_TIMER == 7)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM7;
#elif(CFG_STM32_HAL_TIMER == 8)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM8;
#else
#error "error ： 没有配置定时器！！";
#endif

//HAL里面会调用该函数作时间操作，因此，若用到HAL库，则需要提供该函数提供延时
//return ms

//------------------------------------------------------------------------------
//功能：替换hal库中被weak修饰的同名函数，因djyos不使用stm32的hal库中的延时功能，故在此
//     重新实现.
//------------------------------------------------------------------------------
uint32_t HAL_GetTick(void)
{
    static u32 sTicks = 0;
    static u16 sTimCntLast = 0;
    u16 TimCntCurrent;

//    TimCntCurrent = (__HAL_TIM_GET_COUNTER(&HalTimHandle))&0xFFFF;//0.5ms
    TimCntCurrent = (STM32_HAL_TIMER->CNT)&0xFFFF;
    if( sTimCntLast > TimCntCurrent)
    {
        sTicks += 32768;    //时钟分辨率0.5mS，16bit是32768mS
    }
    sTimCntLast = TimCntCurrent;
    return sTicks + (TimCntCurrent>>1);
}

//------------------------------------------------------------------------------
//功能：替换hal库中被weak修饰的同名函数，因djyos不使用stm32的hal库中的延时功能，故在此
//     重新实现.
//------------------------------------------------------------------------------
void HAL_SuspendTick(void)
{
    TIM_HandleTypeDef HalTimHandle;
    HalTimHandle.Instance = STM32_HAL_TIMER;
    HAL_TIM_Base_Stop(&HalTimHandle);
}
//------------------------------------------------------------------------------
//功能：替换hal库中被weak修饰的同名函数，因djyos不使用stm32的hal库中的延时功能，故在此
//     重新实现.
//------------------------------------------------------------------------------
void HAL_ResumeTick(void)
{
    TIM_HandleTypeDef HalTimHandle;
    HalTimHandle.Instance = STM32_HAL_TIMER;
    HAL_TIM_Base_Start(&HalTimHandle);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
//------------------------------------------------------------------------------
//功能：替换hal库中被weak修饰的同名函数，因djyos不使用stm32的hal库中的延时功能，故在此
//     把它定义为空函数.
//------------------------------------------------------------------------------
HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority)
{
    return HAL_OK;
}
#pragma GCC diagnostic pop

//------------------------------------------------------------------------------
//功能：
void Hal_Timer_Clk_Enable(void)
{
    //此处的函数在board.c中实现，用于使能对应的TIM的时钟
    //下面以TIM6为例
#if(CFG_STM32_HAL_TIMER == 1)
    __HAL_RCC_TIM1_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 2)
    __HAL_RCC_TIM2_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 3)
    __HAL_RCC_TIM3_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 4)
    __HAL_RCC_TIM4_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 5)
    __HAL_RCC_TIM5_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 6)
    __HAL_RCC_TIM6_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 7)
    __HAL_RCC_TIM7_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 8)
    __HAL_RCC_TIM8_CLK_ENABLE();
#else
#error "error ： 没有配置定时器！！";
#endif
}
//此处用TIM6
void HAL_TickInit(void)
{
    u32 uwPrescalerValue;
    TIM_HandleTypeDef HalTimHandle;
    Hal_Timer_Clk_Enable();

    uwPrescalerValue = ((CN_CFG_MCLK/4) / 1000) - 1;    //Counter Clock = 2K
    HalTimHandle.Instance = STM32_HAL_TIMER;
    HalTimHandle.Init.Period        = 0xFFFFFFFF;
    HalTimHandle.Init.Prescaler     = uwPrescalerValue;
    HalTimHandle.Init.ClockDivision = 0;
    HalTimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;

#if defined(TIM_AUTORELOAD_PRELOAD_ENABLE)
    HalTimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
#endif
    HAL_TIM_Base_DeInit(&HalTimHandle);
    HAL_TIM_Base_Init(&HalTimHandle);
//    MODIFY_REG(STM32_HAL_TIMER->CR1, TIM_CR1_ARPE, TIM_CR1_ARPE);
    HAL_TIM_Base_Start(&HalTimHandle);
}
#endif

