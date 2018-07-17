// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：board.c
// 模块描述: 板件相关部分初始化或配置等
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 3/8.2017
// =============================================================================

#include <stdint.h>
#include <stddef.h>
#include <cpu_peri.h>
#include <board-config.h>
#include <stm32L4xx.h>
#include <stm32l4xx_hal_rcc.h>
#include <stm32l4xx_hal_gpio.h>
#include <stm32l4xx_hal_gpio_ex.h>

extern u32 SystemCoreClock;

void Board_GpioInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	//UART4:PA0,PA1,SIMCOM（GPRS）
	GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	//USART2:PA2,PA3
	GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	//USART1:PA9,PA10,RF
	GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	//USART3:PC4,PC5,SIM28ML（GPS）
	GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	//UART5:PC12,PD2,debug print
	GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
	GPIO_InitStruct.Pin = GPIO_PIN_12;					//UART5_TX
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_2;					//UART5_RX
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	//SPI1:PA5,PA6,PA7,SPIFLASH & 加密
	GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
	GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	//SPI1:CS1(PA4),CS2(PB0)
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, SET);

	//FLASH_RST:PD3 (IO_CTL8)
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, SET);

	//AT24,IIC:PB6,PB7
	GPIO_InitStruct.Mode  = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//AT24,EP:PB8 (IO_CTL5)
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//SPI2:PB13,PB14,PB15
	GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//SPI2:CS1(PB11),CS2(PB12)
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, SET);

	//AD0~AD1: PC0~PC1
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	//CFG0

	//COM_CTL0~COM_CTL6

	//IO0~IO8

	SystemCoreClock = CN_CFG_MCLK;
}


bool_t Board_SpiCsCtrl(u8 port,u8 cs,u8 state)
{
	switch(port)
	{
	case CN_SPI1:
		if(cs==0)		//AT45 FLASH
		{
			if(state)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, RESET);
			}
		}
		else
		{
			if(state)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
			}
		}
		break;
	case CN_SPI2:
		if(cs==0)
		{
			if(state)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, RESET);
			}
		}
		else
		{
			if(state)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, RESET);
			}
		}
		break;
	default:
		break;
	}
	return true;
}


void AT24_WPEn(u8 en)
{
	if(en)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);
	}
}


// =============================================================================
// SR1263的看门狗初始化
void Wdt_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void Wdt_FeedDog(void)
{
	HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
}
