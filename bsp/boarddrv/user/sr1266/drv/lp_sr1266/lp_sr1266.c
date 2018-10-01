// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：lp_SR1266.c
// 模块描述: SR1266板板低功耗方案源代码及实现
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 6/20.2017
// =============================================================================
#include "stdint.h"
#include "stdio.h"
#include <lowpower.h>
#include <os.h>
#include <cpu_peri.h>
#include <stm32l4xx_ll_rcc.h>
#include <stm32l4xx_ll_pwr.h>
// =============================================================================
#define __HAL_RCC_PERI_CLK_SLEEP_DISABLE()  {	RCC->AHB1SMENR  = 0x0;	\
											RCC->AHB2SMENR  = 0x0;\
											RCC->AHB3SMENR  = 0x0;\
											RCC->APB1SMENR1 = 0x0;\
											RCC->APB1SMENR2 = 0x0;\
											RCC->APB2SMENR  = 0x0;\
											}

// =============================================================================
// 功能：将GPIO引脚配置成为模拟输入可以降低功耗，根据进入的低功耗等级，可配置GPIO的数量不同
// 参数：
// 返回：
// =============================================================================
void GPIO_LowPower(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_6|GPIO_PIN_9|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
}

// =============================================================================
// 功能：进入低功耗前，操作系统会调用该函数，做必须的准备工作
// 参数：
// 返回：
// =============================================================================
u32 EntrySleepReCall(u32 SleepLevel)
{
	switch(SleepLevel)
	{
	case CN_SLEEP_L0:		//SLEEP
		break;
	case CN_SLEEP_L1:		//LP SLEEP
		__HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);
		break;
	case CN_SLEEP_L2:		//STOP
		__HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);
		break;
	case CN_SLEEP_L3:
		break;
	case CN_SLEEP_L4:
		break;
	default:
		break;
	}
	return 0;
}
// =============================================================================
// 功能：退出低功耗后，操作系统会调用该函数
// 参数：
// 返回：
// =============================================================================
u32 ExitSleepReCall(u32 SleepLevel)
{
	switch(SleepLevel)
	{
	case CN_SLEEP_L0:
		break;
	case CN_SLEEP_L1:
		__HAL_RCC_PWR_CLK_ENABLE();
		break;
	case CN_SLEEP_L2:		//back to run mode
		__HAL_RCC_PWR_CLK_ENABLE();
		break;
	case CN_SLEEP_L3:
		break;
	case CN_SLEEP_L4:
		break;
	default:
		break;
	}
	return 0;
}

// =============================================================================

bool_t __LP_BSP_RestoreRamL3(void)
{
    return true;
}
bool_t __LP_BSP_SaveRamL3(void)
{
   return true;
}

// =============================================================================
// 功能：板件低功耗配置：
//      1、GPIO没用到的，都配置为浮空模拟输入
//      2、低功耗模式下，关闭除LPUART1和LPTIM1的时钟；
//      3、配置进入的低功耗级别为STOP2
// 参数：
// 返回：
// =============================================================================
void SR1266_LowPowerCfg(void)
{
	GPIO_LowPower();

	__HAL_RCC_PERI_CLK_SLEEP_DISABLE();	//禁止SLEEP/STOP模式下所有外设
	__HAL_RCC_LPTIM1_CLK_SLEEP_ENABLE();
	__HAL_RCC_LPUART1_CLK_SLEEP_ENABLE();

	LP_SetSleepLevel(CN_SLEEP_L2);
}




