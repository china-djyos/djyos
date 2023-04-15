#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cpu-optional.h"

#define DJY_BOARD        "TQ2416"

/*____与硬件相关的配置____*/
#define Mhz 1000000
#define CN_CFG_MCLK (532*Mhz)  //主频，内核要用，必须定义
#define CN_CFG_HCLK (CN_CFG_MCLK/4)  //高速外设时钟
#define CN_CFG_PCLK (CN_CFG_MCLK/8)  //低速外设时钟
#define CN_CFG_TIMER_CLK CN_CFG_PCLK  //定时器时钟源

/*____以下定义tick参数____*/
#define CN_CFG_TICK_US 1000  //tick间隔，以us为单位。
#define CN_CFG_TICK_HZ 1000  //内核时钟频率，单位为hz。
#define CN_USE_TICKLESS_MODE    (0U)
#if (!CN_USE_TICKLESS_MODE)
#define CN_CFG_FINE_US 0x000007B  //1/532,tick输入时钟周期，以uS为单位，32位定点数整数、小数各占16位，这也限制了ticks最长不超过65535uS
#define CN_CFG_FINE_HZ CN_CFG_MCLK  //tick输入时钟频率，是CN_CFG_FINE_US的倒数
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

//CPU架构相关配置,可选才在这里配置,例如大小端,是可选的,在这里配置,
//而CPU字长固定,故字长在BSP的arch_feature.h文件中定义
//存储器大小端的配置
#define CN_CFG_LITTLE_ENDIAN        0
#define CN_CFG_BIG_ENDIAN           1
#define CN_CFG_BYTE_ORDER          CN_CFG_LITTLE_ENDIAN

//LCD参数配置
#define CN_LCD_XSIZE        480     //LCD物理像素宽度.
#define CN_LCD_YSIZE        272     //LCD物理像素高度.
#define CN_LCD_PIXEL_FORMAT CN_SYS_PF_RGB565    //LCD颜色格式.

#define  CN_LCD_DRV_FRAME_BUFFER_EN  //定义该宏则使用FrameBuffer,否则直接屏幕输出.
#undef  CN_LCD_DRV_REMOTE_EN        //定义该宏则LCD驱动使用远程显示功能

#ifdef __cplusplus
}
#endif
#endif // __BOARD_CONFIG_H__
