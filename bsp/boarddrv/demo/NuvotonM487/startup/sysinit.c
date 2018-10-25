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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// �ļ���     ��sysinit.c
// ģ������: ϵͳ��ʼ�����֣���Ҫ���ں�������صĳ�ʼ������ʱ�ӣ����ߵ�
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 07/07.2015
// =============================================================================

#include "stdint.h"
#include "cpu_peri.h"
#include "board-config.h"

// ===================== PLL��������ĺ궨�� =====================================
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
// ���ܣ��ú���ʵ��ϵͳʱ�ӵĳ�ʼ������Ҫ������1��ϵͳʱ�Ӵ��ڲ�ʱ���л����ⲿʱ�ӣ�2������
//       HCLK��PCLK1��PCLK2��MCLK��Ƶϵ����3��ʹ�����ݺ�ָ��cache��4��ѡ��MCLKΪϵͳʱ��
//       ��������ʱ�����ã�������board-config.h�е�CN_CFG_MCLK�ȳ�������һ�¡�
// ��������
// ���أ���
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
// ���ܣ�Ƭ��SRAM��ʼ��������GPIO��FMC�������ĳ�ʼ��
// ��������
// ���أ���
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


//�˴���TIM6
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
