
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
    tmr_type* const STM32_HAL_TIMER = TMR1;
#elif(CFG_STM32_HAL_TIMER == 2)
    tmr_type* const STM32_HAL_TIMER = TMR2;
#elif(CFG_STM32_HAL_TIMER == 3)
    tmr_type* const STM32_HAL_TIMER = TMR3;
#elif(CFG_STM32_HAL_TIMER == 4)
    tmr_type* const STM32_HAL_TIMER = TMR4;
#elif(CFG_STM32_HAL_TIMER == 5)
    tmr_type* const STM32_HAL_TIMER = TMR5;
#elif(CFG_STM32_HAL_TIMER == 6)
    tmr_type* const STM32_HAL_TIMER = TMR6;
#elif(CFG_STM32_HAL_TIMER == 7)
    tmr_type* const STM32_HAL_TIMER = TMR7;
#elif(CFG_STM32_HAL_TIMER == 8)
    tmr_type* const STM32_HAL_TIMER = TMR8;
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

    (void) sTimCntLast;
    (void) TimCntCurrent;


    sTicks += 1000;
    return sTicks;


#if 0 /*##WIP##*/
//    TimCntCurrent = (__HAL_TIM_GET_COUNTER(&HalTimHandle))&0xFFFF;//0.5ms
    TimCntCurrent = (STM32_HAL_TIMER->CNT)&0xFFFF;
    if( sTimCntLast > TimCntCurrent)
    {
        sTicks += 32768;    //时钟分辨率0.5mS，16bit是32768mS
    }
    sTimCntLast = TimCntCurrent;
    return sTicks + (TimCntCurrent>>1);
#endif
}

//------------------------------------------------------------------------------
//功能：替换hal库中被weak修饰的同名函数，因djyos不使用stm32的hal库中的延时功能，故在此
//     重新实现.
//------------------------------------------------------------------------------
void HAL_SuspendTick(void)
{
}
//------------------------------------------------------------------------------
//功能：替换hal库中被weak修饰的同名函数，因djyos不使用stm32的hal库中的延时功能，故在此
//     重新实现.
//------------------------------------------------------------------------------
void HAL_ResumeTick(void)
{

}

//  ##WIP##  #pragma GCC diagnostic push
//  ##WIP##  #pragma GCC diagnostic ignored "-Wunused-parameter"
//  ##WIP##  //------------------------------------------------------------------------------
//  ##WIP##  //功能：替换hal库中被weak修饰的同名函数，因djyos不使用stm32的hal库中的延时功能，故在此
//  ##WIP##  //     把它定义为空函数.
//  ##WIP##  //------------------------------------------------------------------------------
//  ##WIP##  HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority)
//  ##WIP##  {
//  ##WIP##      return HAL_OK;
//  ##WIP##  }
//  ##WIP##  #pragma GCC diagnostic pop

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
#if 0 /* ##WIP## */
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
#endif
}
#endif

