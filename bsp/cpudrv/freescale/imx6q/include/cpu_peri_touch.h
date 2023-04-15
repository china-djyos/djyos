#ifndef __CPU_PERI_TOUCH_H__
#define __CPU_PERI_TOUCH_H__
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
  �˴�Ϊ�����ӵĶ��壬���ڴ�����
  2012.09.04
*/

// ����LCD������������LCD_Type�Ķ��壬�Ӷ�ȷ����������������
#include "cpu_peri_lcd.h"


#define TOUCH_X1_BIT            5               // X1��ADC�˿�
#define TOUCH_X2_BIT            7               // X2��ADC�˿�
#define TOUCH_Y1_BIT            4               // Y1��ADC�˿�
#define TOUCH_Y2_BIT            6               // Y2��ADC�˿�

#if (LCD_Type == LCDW35)
#define TOUCH_WIDTH             320             // ��������ˮƽ�ֱ���
#define TOUCH_HEIGHT            240             // �������Ĵ�ֱ�ֱ���
#define TOUCH_DEFAULT_LB        80              // ���Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_RB        940             // �ұ�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_TB        130             // �ϱ�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_BB        900             // �±�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)

#elif (LCD_Type == LCDW43)
#define TOUCH_WIDTH             480             // ��������ˮƽ�ֱ���
#define TOUCH_HEIGHT            272             // �������Ĵ�ֱ�ֱ���
#define TOUCH_DEFAULT_LB        70   //80       // ���Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_RB        970  //950      // �ұ�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_TB        150  //200      //x, �ϱ�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_BB        900  //880      //x, �±�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)

#elif (LCD_Type == LCDA70)
#define TOUCH_WIDTH             800             // ��������ˮƽ�ֱ���
#define TOUCH_HEIGHT            480             // �������Ĵ�ֱ�ֱ���
#define TOUCH_DEFAULT_LB        65              // ���Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_RB        960             // �ұ�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_TB        135             // �ϱ�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#define TOUCH_DEFAULT_BB        905             // �±�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
#endif


#define TOUCH_DEBOUNCE_PIXEL    8  // ȥ�����������������������ȶ��ڰ뾶Ϊ�������صķ�Χ�ڲ�������
#define TOUCH_DEBOUNCE_TIME             5       // ȥ����ʱ�䣬��λΪ32*ADCת��ʱ��

///////////////////////////////////////////////////////////////////
//      ���¹̶�
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
