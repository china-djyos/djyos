#ifndef __SILAN_ADC_REGS_H__
#define __SILAN_ADC_REGS_H__
#include "silan_types.h"

typedef volatile struct adc_regs_struct
{
	u32 adc_cfg ; 
	u32 adc_sts;
	u32 adc_data;
	u32 adc_int_rsts;
	u32 adc_int_msts;
	u32 adc_int_msk;
	u32 adc_int_clr; //0x18
	u32 reseve[0x39];
	u32 adc_seq_cfg; //0x100
	u32 reseve1;
	u32 adc_seq_period;
	u32 adc_seq_thh;
	u32 adc_seq_data;	
}adc_regs_struct_t;

#define adc_regs  ((adc_regs_struct_t *)0x410C0000)

#define DISABLE   	        	1
#define NO_AUTO               	(1<<1)
#define CSEL(x)              	((x&0x7)<<4)
#define BYPASS                	(1<<7)
#define START			        (1<<8)
#define ADC_START   		    (1<<9)

#endif
