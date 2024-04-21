#ifndef __SYS_KEYADC_H__
#define __SYS_KEYADC_H__


#define KEYADC_BASE  (0x01C23400)
#define KEYADC_CTRL_REG                 KEYADC_BASE+(0x0)
#define KEYADC_INTC_REG                 KEYADC_BASE+(0x4)
#define KEYADC_INTS_REG                 KEYADC_BASE+(0x8)
#define KEYADC_DATA_REG                 KEYADC_BASE+(0xC)

//变换模式
#define ADC_NormalMode    0
#define ADC_SingleMode    1
#define ADC_ContinueMode  2
/*UP DOWN 值*/
#define ADC_1_9_V    0
#define ADC_1_8_V    1
#define ADC_1_7_V    2
#define ADC_1_6_V    3

#define ADC0_KEYUP_PENDING              (1<<4)
#define ADC0_ALRDY_HOLD_PENDING         (1<<3)
#define ADC0_HOLDKEY_PENDING            (1<<2)
#define ADC0_KEYDOWN_PENDING            (1<<1)
#define ADC0_DATA_PENDING               (1<<0)


int Key_ADC_Read_Value(void);
void Init_Key_ADC(void);


#endif

