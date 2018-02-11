/*================================================================================
 * 元件名称：Drv_SR5333
 * 元件描述：SR5333板卡驱动文件
 * 元件版本: V1.00
 * 开发人员: zulx
 * 定版时间:
 * 版本修订:
 * 修订人员:
 *================================================================================*/
#include "stdlib.h"
#include "stdint.h"
#include "cpu_peri.h"
#include "shell.h"
#include "powerctrl.h"
#include "lowpower.h"


void Peri_InLowPower(void)
{
	Peri_LowPowerCfg(EN_PERI_SDRAM,false);
	Peri_LowPowerCfg(EN_PERI_EMAC,false);
	Peri_LowPowerCfg(EN_PERI_CAMERA_HEADER,false);
	Peri_LowPowerCfg(EN_PERI_SENSOR1,false);
	Peri_LowPowerCfg(EN_PERI_SENSOR2,false);
	Peri_LowPowerCfg(EN_PERI_SENSOR3,false);
	Peri_LowPowerCfg(EN_PERI_SENSOR4,false);
	Peri_LowPowerCfg(EN_PERI_SENSOR5,false);
	Peri_LowPowerCfg(EN_PERI_SENSOR6,false);
	Peri_LowPowerCfg(EN_PERI_SENSOR7,false);
	Peri_LowPowerCfg(EN_PERI_SENSOR8,false);
	Peri_LowPowerCfg(EN_PERI_SENSOR9,false);
	Peri_LowPowerCfg(EN_PERI_SENSOR10,false);
	Peri_LowPowerCfg(EN_PERI_SENSOR11,false);
	Peri_LowPowerCfg(EN_PERI_SENSOR12,false);
	Peri_LowPowerCfg(EN_PERI_SENSOR13,false);
	Peri_LowPowerCfg(EN_PERI_RF1,false);
    Peri_LowPowerCfg(EN_PERI_RTC,false);
    Peri_LowPowerCfg(EN_PERI_ENCRYPTION,false);
    Peri_LowPowerCfg(EN_PERI_BATTERY_SAMPLE,false);
	Peri_LowPowerCfg(EN_PERI_30W_POWER,false);
	Peri_LowPowerCfg(EN_PERI_10W_POWER,false);
	extern void EMMC_Sleep(void);
	EMMC_Sleep();
}


void GPIO_InLowPower(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	//USART6_RTS1
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, 1);

	//USART6_RTS2
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);

	//USART6_RTS3
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 1);

	//USART6_RTS4
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);

	//USART6_RTS5
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);

	//USART4_RTS
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1);



	//RF_WKUP
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 1);

	//RF_SLEEP
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 1);

	//RF_RESET
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 1);

	//VDD12_KO1
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5, 0);

	//VDD12_KO2
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_6, 0);

	//VDD12_KO3
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_7, 0);

	//VDD12_KO4
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_8, 0);

	//VDD12_KO5
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_9, 0);

	//VDD12_KO6
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_10, 0);

	//VDD12_KO7
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_11, 0);

	//VDD12_KO8
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_12, 0);

	//VDD12_KO9
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, 0);

	//VDD12_KO10
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_14, 0);

	//VDD12_KO11
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_15, 0);

	//VDD12_KO12
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOK, GPIO_PIN_0, 0);

	//VDD12_KO13
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOK, GPIO_PIN_1, 0);

	//VDD12_KO14
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOK, GPIO_PIN_2, 0);

	//VDD12_KO15
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, 0);


	//悬空管脚PC2
	GPIO_InitStruct.Mode  = GPIO_MODE_OUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, 0);


	//悬空管脚PA6
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);



	//悬空管脚PB2
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


	//悬空管脚PB7
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//悬空管脚PB8
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//悬空管脚PC6
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	//悬空管脚PD11
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


	//悬空管脚PD12
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


	//悬空管脚PD13
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	//悬空管脚PE2
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);


	//悬空管脚PE3
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	//悬空管脚PE4
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);


	//悬空管脚PE5
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);


	//悬空管脚PE6
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);


	//悬空管脚PF8
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);


	//悬空管脚PF10
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);


	//悬空管脚PH2
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);


	//悬空管脚PH11
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed = GPIO_SPEED_L;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Pin   = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

   extern void KIKO_LowPowerCfg(u8 flag);
   KIKO_LowPowerCfg(InLowPower);

}


//========================================================
// 函数功能：进入休眠模式的外设关闭功能
// 输入参数：无
// 输出参数：无
// 返回值  ：  SR5333无需回采信号
//========================================================
void SR2711_EntrySleepReCall(u32 SleepLevel)
{
	 /* Disable GPIOs clock */
	  __HAL_RCC_GPIOA_CLK_DISABLE();
	  __HAL_RCC_GPIOB_CLK_DISABLE();
	  __HAL_RCC_GPIOC_CLK_DISABLE();
	  __HAL_RCC_GPIOD_CLK_DISABLE();
	  __HAL_RCC_GPIOE_CLK_DISABLE();
	  __HAL_RCC_GPIOF_CLK_DISABLE();
	  __HAL_RCC_GPIOG_CLK_DISABLE();
	  __HAL_RCC_GPIOH_CLK_DISABLE();
	  __HAL_RCC_GPIOI_CLK_DISABLE();
	  __HAL_RCC_GPIOJ_CLK_DISABLE();
	  __HAL_RCC_GPIOK_CLK_DISABLE();

}

//========================================================
// 函数功能：进入休眠模式的外设关闭功能
// 输入参数：无ci
// 输出参数：无
// 返回值  ：  SR5333无需回采信号
//========================================================
void SR2711_ExitSleepReCall(u32 SleepLevel)
{
	uint32_t  bySleepLevel;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
//	__HAL_RCC_GPIOC_CLK_ENABLE();
//	__HAL_RCC_GPIOD_CLK_ENABLE();
//	__HAL_RCC_GPIOE_CLK_ENABLE();
//	__HAL_RCC_GPIOF_CLK_ENABLE();
//	__HAL_RCC_GPIOG_CLK_ENABLE();
//	__HAL_RCC_GPIOH_CLK_ENABLE();
//	__HAL_RCC_GPIOI_CLK_ENABLE();
//	__HAL_RCC_GPIOJ_CLK_ENABLE();
//	__HAL_RCC_GPIOK_CLK_ENABLE();

	bySleepLevel = LP_GetSleepLevel();
	if(CN_SLEEP_L1 == bySleepLevel)
	{
		STOP_EXIT_SysClockInit();
	}

}
