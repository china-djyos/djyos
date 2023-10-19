#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cpu-optional.h"

#define DJY_BOARD       "EXPLORER-STM32F4"

#define Mhz             1000000
#define CN_CFG_MCLK     (168*Mhz)           //主频，内核要用，必须定义
#define CN_CFG_FCLK     CN_CFG_MCLK
#define CN_CFG_AHBCLK   CN_CFG_MCLK         //总线时钟
#define CN_CFG_PCLK2    (CN_CFG_MCLK/2)     //高速外设时钟
#define CN_CFG_PCLK1    (CN_CFG_MCLK/4)     //低速外设时钟
#define CN_CFG_EXTCLK   (8*Mhz)             //外部时钟=8M

/*____以下定义tick参数____*/
#define CN_CFG_TICK_US 1000  //tick间隔，以us为单位。
#define CN_CFG_TICK_HZ 1000  //内核时钟频率，单位为hz。
#define CN_USE_TICKLESS_MODE    (0U)
#if (!CN_USE_TICKLESS_MODE)
#define CN_CFG_FINE_US 0x00000186  //1/168M,tick输入时钟周期，以uS为单位，32位定点数整数、小数各占16位，这也限制了ticks最长不超过65535uS
#define CN_CFG_FINE_HZ CN_CFG_MCLK  //tick输入时钟频率，是CN_CFG_FINE_US的倒数。
#else
#define CN_CFG_USE_USERTIMER        (0U)//是否使用LPTIMER作为系统时钟
#define CN_CFG_TIME_BASE_HZ         CN_CFG_MCLK//(32000U)//(8000U)
#if (!CN_CFG_USE_USERTIMER)
#define CN_CFG_USE_BYPASSTIMER      (0U)//0表示不使用旁路定时器，1表示使用旁路定时器
#define CN_CFG_TIME_PRECISION       (500U)/*精度单位：US*/
#else
#define CN_CFG_USERTIMER_PRESC      (1U)//若不使用SYSTICK定时器，需指定用户定时器的分频数
#define CN_CFG_FINE_US (0x1F4000U)  //1/32000,tick输入时钟周期，以uS为单位，32位定点数整数、小数各占16位，这也限制了ticks最长不超过65535uS
#define CN_CFG_FINE_HZ (0x831U)  //tick输入时钟频率，是CN_CFG_FINE_US的倒数
#define CN_CFG_TIME_PRECISION       (200U)/*精度单位：US*/
#endif
#endif

//CPU架构相关配置,可选或者可能可选的才在这里配置,例如大小端,是可选的,在这里配置,
//而CPU字长固定,故字长在BSP的arch.h文件中定义
//存储器大小端的配置
#define CN_CFG_LITTLE_ENDIAN        0
#define CN_CFG_BIG_ENDIAN           1
#define CN_CFG_BYTE_ORDER          CN_CFG_LITTLE_ENDIAN

#ifdef __cplusplus
}
#endif
#endif // __CPU_OPTIONAL_H__




