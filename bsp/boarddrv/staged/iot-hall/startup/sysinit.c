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

#define STM32F40_41xxx

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

#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
#define PLL_N      360
#define PLL_P      2
#endif /* STM32F427_437x || STM32F429_439xx */

#if defined (STM32F401xx)
#define PLL_N      336
#define PLL_P      4
#endif /* STM32F401xx */

// =============================================================================
// ���ܣ��ú���ʵ��ϵͳʱ�ӵĳ�ʼ������Ҫ������1��ϵͳʱ�Ӵ��ڲ�ʱ���л����ⲿʱ�ӣ�2������
//      HCLK��PCLK1��PCLK2��MCLK��Ƶϵ����3��ʹ�����ݺ�ָ��cache��4��ѡ��MCLKΪϵͳʱ��
// ��������
// ���أ���
// =============================================================================
void SysClockInit(void)
{
    //PLL (clocked by HSE) used as System clock source
    u32 StartUpCounter = 0, HSEStatus = 0;

    /* Enable HSE */
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);

    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != RESET)
    {
        HSEStatus = (uint32_t)0x01;
    }
    else
    {
        HSEStatus = (uint32_t)0x00;
    }

    if (HSEStatus == (uint32_t)0x01)
    {
        /* Select regulator voltage output Scale 1 mode */
        RCC->APB1ENR |= RCC_APB1ENR_PWREN;
        PWR->CR |= PWR_CR_VOS;

        /* HCLK = SYSCLK / 1*/
        RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    #if defined (STM32F40_41xxx) || defined (STM32F427_437xx) || defined (STM32F429_439xx)
    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
    #endif /* STM32F40_41xxx || STM32F427_437x || STM32F429_439xx */

    #if defined (STM32F401xx)
    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
    #endif /* STM32F401xx */

    /* Configure the main PLL */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);

    /* Enable the main PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till the main PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    #if defined (STM32F427_437xx) || defined (STM32F429_439xx)
    /* Enable the Over-drive to extend the clock frequency to 180 Mhz */
    PWR->CR |= PWR_CR_ODEN;
    while((PWR->CSR & PWR_CSR_ODRDY) == 0)
    {
    }
    PWR->CR |= PWR_CR_ODSWEN;
    while((PWR->CSR & PWR_CSR_ODSWRDY) == 0)
    {
    }
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
    #endif /* STM32F427_437x || STM32F429_439xx  */

    #if defined (STM32F40_41xxx)
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
    #endif /* STM32F40_41xxx  */

    #if defined (STM32F401xx)
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_2WS;
    #endif /* STM32F401xx */

    /* Select the main PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
    {
    }
    }
    else
    { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
    }
}

// =============================================================================
// ���ܣ�Ƭ��SRAM��ʼ��������GPIO��FMC�������ĳ�ʼ��
// ��������
// ���أ���
// =============================================================================
void SRAM_Init(void)
{
    /* Enable GPIOD, GPIOE, GPIOF and GPIOG interface clock */
    RCC->AHB1ENR   |= 0x00000078;

    /* Connect PDx pins to FMC Alternate function: PD0,1,4,5,8~15*/
    GPIOD->AFR[0]  = 0x00cc00cc;
    GPIOD->AFR[1]  = 0xcccccccc;
    /* Configure PDx pins in Alternate function mode */
    GPIOD->MODER   = 0xaaaa0a0a;
    /* Configure PDx pins speed to 100 MHz */
    GPIOD->OSPEEDR = 0xffff0f0f;
    /* Configure PDx pins Output type to push-pull */
    GPIOD->OTYPER  = 0x00000000;
    /* No pull-up, pull-down for PDx pins */
    GPIOD->PUPDR   = 0x55550505;//0x00000000;

    /* Connect PEx pins to FMC Alternate function :PE0,1,7~15*/
    GPIOE->AFR[0]  = 0xc00000cc;
    GPIOE->AFR[1]  = 0xcccccccc;
    /* Configure PEx pins in Alternate function mode */
    GPIOE->MODER   = 0xaaaa800a;
    /* Configure PEx pins speed to 100 MHz */
    GPIOE->OSPEEDR = 0xffffC00f;
    /* Configure PEx pins Output type to push-pull */
    GPIOE->OTYPER  = 0x00000000;
    /* No pull-up, pull-down for PEx pins */
    GPIOE->PUPDR   = 0x55554005;

    /* Connect PFx pins to FMC Alternate function:PF0~5,12~15 */
    GPIOF->AFR[0]  = 0x00cccccc;
    GPIOF->AFR[1]  = 0xcccc0000;
    /* Configure PFx pins in Alternate function mode */
    GPIOF->MODER   = 0xaa000aaa;
    /* Configure PFx pins speed to 100 MHz */
    GPIOF->OSPEEDR = 0xff000fff;
    /* Configure PFx pins Output type to push-pull */
    GPIOF->OTYPER  = 0x00000000;
    /* No pull-up, pull-down for PFx pins */
    GPIOF->PUPDR   = 0x55000555;

    /* Connect PGx pins to FMC Alternate function :PG0~5,10*/
    GPIOG->AFR[0]  = 0x00cccccc;
    GPIOG->AFR[1]  = 0x00000c00;
    /* Configure PGx pins in Alternate function mode */
    GPIOG->MODER   = 0x00200aaa;
    /* Configure PGx pins speed to 100 MHz */
    GPIOG->OSPEEDR = 0x00300fff;
    /* Configure PGx pins Output type to push-pull */
    GPIOG->OTYPER  = 0x00000000;
    /* No pull-up, pull-down for PGx pins */
    GPIOG->PUPDR   = 0x00100555;

    /*-- FMC Configuration ------------------------------------------------------*/
    /* Enable the FMC/FSMC interface clock */
    RCC->AHB3ENR         |= 0x00000001;

    #if defined (STM32F427_437xx) || defined (STM32F429_439xx)
      /* Configure and enable Bank1_SRAM2 */
      FMC_Bank1->BTCR[2]  = 0x00001011;
      FMC_Bank1->BTCR[3]  = 0x00000201;
      FMC_Bank1E->BWTR[2] = 0x0fffffff;
    #endif /* STM32F427_437xx || STM32F429_439xx */

    #if defined (STM32F40_41xxx)
      /* Configure and enable Bank1_SRAM3��NE3 */
    //bank1��NE1~4,ÿһ����һ��BCR+TCR�������ܹ��˸��Ĵ�����
    //��������ʹ��NE3 ��Ҳ�Ͷ�ӦBTCR[4],[5]��
    FSMC_Bank1->BTCR[4]=0X00000000;
    FSMC_Bank1->BTCR[5]=0X00000000;
    FSMC_Bank1E->BWTR[4]=0X00000000;
    //����BCR�Ĵ��� ʹ���첽ģʽ,ģʽA(��д����һ��ʱ��Ĵ���)
    //BTCR[ż��]:BCR�Ĵ���;BTCR[����]:BTR�Ĵ���
    FSMC_Bank1->BTCR[4]|=1<<12;//�洢��дʹ��
    FSMC_Bank1->BTCR[4]|=1<<4; //�洢�����ݿ��Ϊ16bit
    //����BTR�Ĵ���         ��HCLK=168M, 1��HCLK=6ns
    FSMC_Bank1->BTCR[5]|=8<<8; //���ݱ���ʱ�䣨DATAST��Ϊ9��HCLK 6*9=54ns
    FSMC_Bank1->BTCR[5]|=0<<4; //��ַ����ʱ�䣨ADDHLD��δ�õ�
    FSMC_Bank1->BTCR[5]|=6<<0; //��ַ����ʱ�䣨ADDSET��Ϊ0��HCLK 0ns
    //����дʱ��Ĵ���
    FSMC_Bank1E->BWTR[4]=0x0FFFFFFF;//Ĭ��ֵ
    //ʹ��BANK1����3
    FSMC_Bank1->BTCR[4]|=1<<0;
    #endif  /* STM32F40_41xxx */
}


