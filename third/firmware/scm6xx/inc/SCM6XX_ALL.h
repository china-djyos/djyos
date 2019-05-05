/*--------------------------------------------
描述：总的头文件
平台：SCM6XX 
时间：2018-1-9
作者：张彦欣
-----------------------------------------------*/
#ifndef _SCM6XX_ALL_H_
#define _SCM6XX_ALL_H_

//#include <string.h>
//#include <stdlib.h>
//#include <stdarg.h>
//#include <stdio.h>



#include "SCM6XX_TYPES.h"
#include "SCM6XX_REGBIT.h"
#include "SCM6XX_MCU.h"
#include "SCM6XX_SCU.h"
#include "SCM6XX_CLK.h"
#include "SCM6XX_RESET.h"
#include "SCM6XX_FLASH.h"
#include "SCM6XX_TIMER.h"
#include "SCM6XX_SPI.h"
#include "SCM6XX_UART.h"
#include "SCM6XX_GPIO.h"
#include "SCM6XX_I2C.h"
#include "SCM6XX_DMA.h"
#include "SCM6XX_ANA_CTRL.h"
#include "SCM6XX_WDT.h"
#include "SCM6XX_PWM.h"
#include "SCM6XX_RTC.h"
#include "SCM6XX_CRC.h"
#include "SCM6XX_7816.h"
#include "SCM6XX_IOCTRL.h"
#include "SCM6XX_MISC.h"
//#include "SCM6XX_int.h"

#define PCLK  40000  //40000以K（兆）为单位

//for modules





#define DMA0    0
#define DMA1    1
#define DMA2    2
#define DMA3    3
#define GMAC0   4
#define GMAC1   5
#define OTG     6
#define FLASH   7
#define U7816   8
#define PWM0    9
#define PWMA    9
#define UART0   10
#define UART1   11
#define UART2   12
#define UART3   13
#define I2C0    14
#define I2C1    15
#define SPI0    16
#define SPI1    17
#define GPIO    18
#define RTC     19
#define TIMERA  20
#define TIMERB  21
#define WDT     22
#define ANACTRL  23
#define PWM1     24
#define PWMB     24
#define UART4    25
#define UART5    26
#define UART6    27
#define UART7    28
#define I2C2     29
#define SPI2     30
#define SPI3     31
#define CRC      32
#define TIMERC   33
#define TIMERD   34
#define ADC      35


//#include "_CLK.h"
//#include "flash_test.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */


#endif //_SM1226_ALL_H_
	
	
