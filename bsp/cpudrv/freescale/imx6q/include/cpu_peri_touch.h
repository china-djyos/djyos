#ifndef __CPU_PERI_TOUCH_H__
#define __CPU_PERI_TOUCH_H__
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
  此处为新增加的定义，用于触摸屏
  2012.09.04
*/

// 引入LCD的驱动，借用LCD_Type的定义，从而确定触摸屏的物理定义
#include "cpu_peri_lcd.h"


#define TOUCH_X1_BIT            5               // X1的ADC端口
#define TOUCH_X2_BIT            7               // X2的ADC端口
#define TOUCH_Y1_BIT            4               // Y1的ADC端口
#define TOUCH_Y2_BIT            6               // Y2的ADC端口

#if (LCD_Type == LCDW35)
#define TOUCH_WIDTH             320             // 触摸屏的水平分辨率
#define TOUCH_HEIGHT            240             // 触摸屏的垂直分辨率
#define TOUCH_DEFAULT_LB        80              // 左边缘对应的默认AD转换值(16bit)
#define TOUCH_DEFAULT_RB        940             // 右边缘对应的默认AD转换值(16bit)
#define TOUCH_DEFAULT_TB        130             // 上边缘对应的默认AD转换值(16bit)
#define TOUCH_DEFAULT_BB        900             // 下边缘对应的默认AD转换值(16bit)

#elif (LCD_Type == LCDW43)
#define TOUCH_WIDTH             480             // 触摸屏的水平分辨率
#define TOUCH_HEIGHT            272             // 触摸屏的垂直分辨率
#define TOUCH_DEFAULT_LB        70   //80       // 左边缘对应的默认AD转换值(16bit)
#define TOUCH_DEFAULT_RB        970  //950      // 右边缘对应的默认AD转换值(16bit)
#define TOUCH_DEFAULT_TB        150  //200      //x, 上边缘对应的默认AD转换值(16bit)
#define TOUCH_DEFAULT_BB        900  //880      //x, 下边缘对应的默认AD转换值(16bit)

#elif (LCD_Type == LCDA70)
#define TOUCH_WIDTH             800             // 触摸屏的水平分辨率
#define TOUCH_HEIGHT            480             // 触摸屏的垂直分辨率
#define TOUCH_DEFAULT_LB        65              // 左边缘对应的默认AD转换值(16bit)
#define TOUCH_DEFAULT_RB        960             // 右边缘对应的默认AD转换值(16bit)
#define TOUCH_DEFAULT_TB        135             // 上边缘对应的默认AD转换值(16bit)
#define TOUCH_DEFAULT_BB        905             // 下边缘对应的默认AD转换值(16bit)
#endif


#define TOUCH_DEBOUNCE_PIXEL    8  // 去抖动像素数，即触摸坐标稳定在半径为多少像素的范围内才输出结果
#define TOUCH_DEBOUNCE_TIME             5       // 去抖动时间，单位为32*ADC转换时间

///////////////////////////////////////////////////////////////////
//      以下固定
#define TOUCH_X1                        (0x0001<<TOUCH_X1_BIT)
#define TOUCH_X2                        (0x0001<<TOUCH_X2_BIT)
#define TOUCH_Y1                        (0x0001<<TOUCH_Y1_BIT)
#define TOUCH_Y2                        (0x0001<<TOUCH_Y2_BIT)

/*
bool_t touch_hard_init(void);
ufast_t touch_scan_hard(struct SingleTouchMsg *touch_data);
void touch_ratio_adjust(char *display_name,struct ST_TouchAdjust *adjust);
*/

#ifdef __cplusplus
}
#endif

#endif /* __CPU_PERI_TOUCH_H__ */
