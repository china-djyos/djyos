
//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下二条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
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
/*
 * app_main.c
 *
 *  Created on: 2014-5-28
 *      Author: Administrator
 */

#include "stdint.h"
#include "cpu_peri.h"
#include "systime.h"
#include "stddef.h"
#include <os.h>

//初始化PF9和PF10为输出口.并使能这两个口的时钟
//LED IO初始化
#define LD2     0	//PA5
u32 led_mask[] = {1UL << 5};
void LED_Init(void)
{
	RCC->AHB2ENR|=1<<0;//使能PORTA时钟

	GPIOA->MODER    &= ~((3UL << 2*5));
	GPIOA->MODER    |=  ((1UL << 2*5));
	GPIOA->OTYPER   &= ~((1UL << 2*5));
	GPIOA->OSPEEDR  &= ~((3UL << 2*5));
	GPIOA->OSPEEDR  |=  ((2UL << 2*5));
	GPIOA->PUPDR    &= ~((3UL << 2*5));
	GPIOA->PUPDR    |=  ((1UL << 2*5));
}
void LED_On (unsigned int num)
{
	GPIOA->BSRR |= led_mask[num];
}
void LED_Off (unsigned int num)
{
	GPIOA->BSRR |= led_mask[num]<<16;
}

#include <stm32l4xx_hal_gpio.h>
#include <stm32l4xx_hal_rcc.h>
#include <lowpower.h>
#define BUTTONn                            1
GPIO_TypeDef*  BUTTON_PORT[BUTTONn] = {GPIOC};
const uint16_t BUTTON_PIN[BUTTONn] = {GPIO_PIN_13};
static u8 flg = 0;
ptu32_t Button_Irq(u8 irqline)
{
	LED_On(LD2);
	if(__HAL_GPIO_EXTI_GET_IT(BUTTON_PIN[0]) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(BUTTON_PIN[0]);
	}
	flg = 0;
	Int_ClearLine(CN_INT_LINE_EXTI15_10);
	return 1;
}
void ButtonInit(u8 mode)
{
	GPIO_InitTypeDef gpioinitstruct = {0};

	__HAL_RCC_GPIOC_CLK_ENABLE();
	if(mode == 0)
	{
		/* Configure Button pin as input */
		gpioinitstruct.Pin    = BUTTON_PIN[0];
		gpioinitstruct.Mode   = GPIO_MODE_INPUT;
		gpioinitstruct.Pull   = GPIO_NOPULL;
		gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;

		HAL_GPIO_Init(BUTTON_PORT[0], &gpioinitstruct);
	}
	else
	{
	    gpioinitstruct.Pin    = BUTTON_PIN[0];
	    gpioinitstruct.Mode   = GPIO_MODE_IT_FALLING;
	    gpioinitstruct.Pull   = GPIO_NOPULL;
	    gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
	    HAL_GPIO_Init(BUTTON_PORT[0], &gpioinitstruct);

	    Int_Register(CN_INT_LINE_EXTI15_10);
	    Int_SetClearType(CN_INT_LINE_EXTI15_10,CN_INT_CLEAR_AUTO);
	    Int_IsrConnect(CN_INT_LINE_EXTI15_10,Button_Irq);
//	    Int_SettoAsynSignal(CN_INT_LINE_EXTI15_10);
	    Int_SettoReal(CN_INT_LINE_EXTI15_10);
	    Int_ClearLine(CN_INT_LINE_EXTI15_10);
//	    Int_RestoreAsynLine(CN_INT_LINE_EXTI15_10);
	    Int_RestoreRealLine(CN_INT_LINE_EXTI15_10);
	}
}

u8 ButtonGet(void)
{
	return HAL_GPIO_ReadPin(BUTTON_PORT[0], BUTTON_PIN[0]);
}

#include <stm32l4xx_hal_rcc.h>
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}
void SystemClock_24MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, update MSI to 24Mhz (RCC_MSIRANGE_9) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_9;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

}

void test_run_range2_24mhz(void)
  {
  printf("\n\r Executing test (RUN Range 2, 24MHz - with FLASH ART ON) \n\r");
  printf(" Please measure current then use Reset button to select another test \n\r");

  /* Set the System clock to 24 MHz */
//  SystemClock_24MHz();

  /* Enable PWR clock */
//  __HAL_RCC_PWR_CLK_ENABLE();
  /* Configure the main internal regulator output voltage */
//  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);
  /* Disable PWR clock */
//  __HAL_RCC_PWR_CLK_DISABLE();

  /* Set all GPIO in analog state to reduce power consumption */
  GPIO_AnalogConfig();

  /* Disable USART2 clock */
//  __HAL_RCC_USART2_CLK_DISABLE();

  /* Suspend Tick increment to prevent wakeup by Systick interrupt.         */
  /* Otherwise the Systick interrupt will wake up the device within 1ms     */
  /* (HAL time base).                                                       */
//  HAL_SuspendTick();

  pg_systick_reg->ctrl &= ~(1<<bo_systick_ctrl_enable);    //除能

 __NOP();
 __NOP();
  while(1);

}

#include "uartctrl.h"
#define SramTestLen 1024
u8 SRAMTEST[SramTestLen];



u32 SRAMTestRunMode_Range1(u32 times)
{
	//enter runMode Range1
	__HAL_RCC_PWR_CLK_ENABLE();
  	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  	while(times)
  	{
		if(0 != memtester_main((u32)SRAMTEST,SramTestLen))
		{
			LED_On(LD2);
			break;
		}
		times --;
  	}
  	return times;
}
u32 SRAMTestRunMode_Range2(u32 times)
{
	//enter runMode Range1
	__HAL_RCC_PWR_CLK_ENABLE();
  	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);

  	while(times)
  	{
		if(0 != memtester_main((u32)SRAMTEST,SramTestLen))
		{
			LED_On(LD2);
			break;
		}
		times --;
  	}

  	return times;
}
u32 SRAMTestLPRunMode(u32 times)
{
	SystemClock_Decrease(RCC_MSIRANGE_5,FLASH_LATENCY_0);
	HAL_PWREx_EnableLowPowerRunMode();
  	while(times)
  	{
		if(0 != memtester_main((u32)SRAMTEST,SramTestLen))
		{
			LED_On(LD2);
			break;
		}
		times --;
  	}

  	HAL_PWREx_DisableLowPowerRunMode();
  	SystemClock_Increase(RCC_MSIRANGE_9,FLASH_LATENCY_3);
  	return times;
}

void IntTimeTest(void)
{
	LED_On(LD2);
	pg_systick_reg->ctrl &= ~(1<<bo_systick_ctrl_enable);    //除能
	ButtonInit(1);

	SystemClock_Decrease(RCC_MSIRANGE_1,FLASH_LATENCY_0);
	while(1)
	{

	}
}


ptu32_t djy_main(void)
{
	extern void Sh_GetStatus(char *param);
	extern void Sh_GetRunMode(char *param);
	Sh_GetRunMode(NULL);
	Sh_GetStatus(NULL);
	LED_Init();
	ButtonInit(0);

//	SystemClock_Config();
//	ButtonInit(1);
//	RTC_Init();
	LED_On(LD2);

//	IntTimeTest();

	static u8 flag = 0;
//	ADC_ADC_Init();
//	LPTIM1_LpTimerInit();

//	if(0!= SRAMTestLPRunMode(3))
//	{
//		while(1);
//	}

	ADC_ContinuousSampleModuleInit();
//	LPTIM1_LpTimerInit();
	while(1)
	{
//		Djy_EventDelay(520*mS);
//		LED_On(LD2);
//		Djy_EventDelay(20*mS);
//		LED_Off(LD2);

		Djy_DelayUs(100*mS);

//		if(0 != memtester_main((u32)SRAMTEST,SramTestLen))
//		{
//			LED_On(LD2);
//			while(1);
//		}

//		Djy_DelayUs(5000*mS);
//		__HAL_RCC_PWR_CLK_ENABLE();
//		HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);
//		__HAL_RCC_PWR_CLK_DISABLE();
//		GPIO_AnalogConfig();
//
		  pg_systick_reg->ctrl &= ~(1<<bo_systick_ctrl_enable);    //除能
//
//		 __NOP();
//		 __NOP();
//		  while(1);
//		test_run_range2_24mhz();
//		while(1);

//		if(flag == 0)
//		{
			if(!ButtonGet())
			{
//				GPIO_AnalogConfig();
//				flag = 1;
//				ADC_ADC_Init();
//				ADC_ADC_Start();
//				LP_SetSleepLevel(CN_SLEEP_L1);
				ADC_ContinousSampleStart(0);
				Djy_EventDelay(1*mS);
//				IntTimeTest();
//				LP_SetSleepLevel(CN_SLEEP_L2);
//				LPTIM1_TimerStart(5000);
//				LP_SetSleepLevel(CN_SLEEP_L0);

//				while(1)
//				{
//					LED_On(LD2);
//					LP_SetSleepLevel(CN_SLEEP_L2);
//					Djy_EventDelay(1*mS);
//					LED_Off(LD2);
//					LP_SetSleepLevel(CN_SLEEP_L2);
//					Djy_EventDelay(1*mS);
//				}
			}
//		}
	}
	return 0;
}


