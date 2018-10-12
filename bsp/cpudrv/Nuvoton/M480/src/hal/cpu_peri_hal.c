// =============================================================================

// 文件名     ：cpu_peri_hal.c
// 模块描述: HAL需要使用到的接口函数
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 2/20.2017
// =============================================================================
#if 0
#include <stdio.h>
#include <stddef.h>
#include "cpu_peri.h"
#include <os.h>
#include <board-config.h>



//==============================================================================
//HAL_RCC会用到此量，因为DJYOS使用自己的时钟初始化过程，因此，在使用HAL库的时候，有些HAL
//内部会调用该量，因此需此处赋值
//例如ETH网络驱动中，stm32f7xx_hal_eth.c中会调用
uint32_t SystemCoreClock = CN_CFG_MCLK;            //HAL_RCC会用到此量

const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8] =  {0, 0, 0, 0, 1, 2, 3, 4};
//HAL里面会调用该函数作时间操作，因此，若用到HAL库，则需要提供该函数提供延时
//return ms


#endif
