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
// 文件名     ：sysinit.c
// 模块描述: 系统初始化部分，主要是内核启动相关的初始化，如时钟，总线等
// 模块版本: V1.00
// 创建人员: hm
// 创建时间: 07/07.2015
// =============================================================================

#include "stdint.h"
#include "cpu_peri.h"
#include "board-config.h"

// ===================== PLL配置所需的宏定义 =====================================
// PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
// USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ
// //SYSCLK = PLL_VCO / PLL_P
// =============================================================================
#define PLL_M      8
#define PLL_Q      7

#if defined (STM32F40_41xxx)
#define PLL_N      336
#define PLL_P      2
#endif /* STM32F40_41xxx */

// =============================================================================
// 功能：该函数实现系统时钟的初始化，主要包括：1、系统时钟从内部时钟切换到外部时钟；2、配置
//       HCLK、PCLK1、PCLK2、MCLK分频系数；3、使能数据和指令cache；4、选用MCLK为系统时钟
//       本函数的时钟设置，必须与board-config.h中的CN_CFG_MCLK等常量定义一致。
// 参数：无
// 返回：无
// =============================================================================
void SysClockInit(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                         */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Set access cycle for CPU @ 192MHz */
    FMC->CYCCTL = (FMC->CYCCTL & ~FMC_CYCCTL_CYCLE_Msk) | (8 << FMC_CYCCTL_CYCLE_Pos);
    /* Configure power down bias, must set 1 before entering power down mode.
       So set it at the very beginning */
    CLK->LDOCTL |= CLK_LDOCTL_PDBIASEN_Msk;
    /* Hand over the control of PF.4~11 I/O function from RTC module to GPIO module */
    CLK->APBCLK0 |= CLK_APBCLK0_RTCCKEN_Msk;
    RTC->GPIOCTL0 &= ~(RTC_GPIOCTL0_CTLSEL0_Msk | RTC_GPIOCTL0_CTLSEL1_Msk |
                       RTC_GPIOCTL0_CTLSEL2_Msk | RTC_GPIOCTL0_CTLSEL3_Msk);
    RTC->GPIOCTL1 &= ~(RTC_GPIOCTL1_CTLSEL4_Msk | RTC_GPIOCTL1_CTLSEL5_Msk |
                       RTC_GPIOCTL1_CTLSEL6_Msk | RTC_GPIOCTL1_CTLSEL7_Msk);
    CLK->APBCLK0 &= ~CLK_APBCLK0_RTCCKEN_Msk;

    /* Set XT1_OUT(PF.2) and XT1_IN(PF.3) to input mode */
    PF->MODE &= ~(GPIO_MODE_MODE2_Msk | GPIO_MODE_MODE3_Msk);

    /* Enable HIRC clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Enable HXT clock (external XTAL 12MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(192000000);
    CLK->PCLKDIV = (CLK_PCLKDIV_PCLK0DIV2 | CLK_PCLKDIV_PCLK1DIV2); // PCLK divider set 2

    /* Lock protected registers */
    SYS_LockReg();
}

// =============================================================================
// 功能：片外SRAM初始化，包括GPIO和FMC控制器的初始化
// 参数：无
// 返回：无
// =============================================================================
void SRAM_Init(void)
{

}


// =============================================================================
// ==========================HAL INIT ==========================================
#if 0 //def USE_HAL_DRIVER
uint32_t HAL_GetTick(void)
{
    static u32 sTick = 0,sTickLast = 0;
    u16 TimCnt;

    TIM_HandleTypeDef TimHandle;

    TimHandle.Instance = TIM6;
    TimCnt = (__HAL_TIM_GET_COUNTER(&TimHandle))&0xFFFF;//0.5ms

    if( sTickLast > TimCnt)
    {
        sTick += 32768;
    }
    sTickLast = TimCnt;

    return (sTick + (TimCnt >> 1));
}
void HAL_SuspendTick(void)
{
    TIM_HandleTypeDef TimHandle;
    TimHandle.Instance = TIM6;
    HAL_TIM_Base_Stop(&TimHandle);
}
void HAL_ResumeTick(void)
{
    TIM_HandleTypeDef TimHandle;
    TimHandle.Instance = TIM6;
    HAL_TIM_Base_Start(&TimHandle);
}


//此处用TIM6
void HAL_TickInit(void)
{
    u32 uwPrescalerValue;
    TIM_HandleTypeDef TimHandle;

    __HAL_RCC_TIM6_CLK_ENABLE();

    uwPrescalerValue = ((CN_CFG_MCLK/4) / 1000) - 1;    //Counter Clock = 2K
    TimHandle.Instance = TIM6;
    TimHandle.Init.Period        = 0xFFFF;
    TimHandle.Init.Prescaler     = uwPrescalerValue;
    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;

    HAL_TIM_Base_DeInit(&TimHandle);
    HAL_TIM_Base_Init(&TimHandle);
    HAL_TIM_Base_Start(&TimHandle);
}
#endif
