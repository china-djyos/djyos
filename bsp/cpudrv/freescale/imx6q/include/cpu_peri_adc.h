#ifndef __CPU_PERI_ADC_H__
#define __CPU_PERI_ADC_H__
#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct ADC_REG
{
    vu32 ADCCON;
    vu32 ADCTSC;
    vu32 ADCDLY;
    vu32 ADCDAT0;
    vu32 ADCDAT1;
    vu32 ADCUPDN;
    vu32 ADCMUX;
};

#define ADC_REG_BASE    (struct ADC_REG*)0x58000000


//ADC״̬���ƽṹ
struct adc_port_cb
{
   u32 location_x;    // X ����ֵ
   u32 location_y;    // Y ����ֵ
   u8  isdown;        // ̧�𣬻��ǰ��£�����Ϊ1
   u8  touch_over;    // �Ƿ������
};

ptu32_t module_init_adc(ptu32_t para);

void  ClearPending(u32 bit);

#ifdef __cplusplus
}
#endif

#endif /* __CPU_PERI_ADC_H__ */
