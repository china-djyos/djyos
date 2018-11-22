#ifndef CPU_PERI_ADC_H
#define CPU_PERI_ADC_H

#define  CN_AD_FAILSE 		    ( 0 )
#define  CN_AD_SUCCESS		( 1 )
#define  CN_ARG_ERR          	( 0x38  )
#define  CN_RAED_ERR_DAT    (0xffffffff)  //这里用前面16位,标识读出错
#include "silan_iomux.h"

/*channel define*/

#define CN_AD_CHANNEL0  IO_CONFIG_PA0
#define CN_AD_CHANNEL1  IO_CONFIG_PA1
#define CN_AD_CHANNEL2  IO_CONFIG_PA2
#define CN_AD_CHANNEL3  IO_CONFIG_PA3
#define CN_AD_CHANNEL4  IO_CONFIG_PA4
#define CN_AD_CHANNEL5  IO_CONFIG_PA5
#define CN_AD_CHANNEL6  IO_CONFIG_PA6
#define CN_AD_CHANNEL7  IO_CONFIG_PA7

/*AD pin config choose from below when read .match with channel config*/

#define CN_AD_CFG_PA0  IO_CONFIG_PA0
#define CN_AD_CFG_PA1  IO_CONFIG_PA1
#define CN_AD_CFG_PA2  IO_CONFIG_PA2
#define CN_AD_CFG_PA3  IO_CONFIG_PA3
#define CN_AD_CFG_PA4  IO_CONFIG_PA4
#define CN_AD_CFG_PA5  IO_CONFIG_PA5
#define CN_AD_CFG_PA6  IO_CONFIG_PA6
#define CN_AD_CFG_PA7  IO_CONFIG_PA7

//comand
#define  CN_ADC_CHANNEL_SET	            (0x11)
#define  CN_ADC_CHANNEL_RETSET	    (0x12)
#define  CN_ADC_CHANNEL_PINCFG       (0x13)
#define  CN_ADC_CHANNEL_RD_DAT       (0x14)     //read data

//vef select
typedef enum{
	EN_ADC_VREF_EXT33 = 0,
	EN_ADC_VREF_IN24,
}tagVef;

void ModuleInstall_ADC(tagVef vref);
extern void appADC_Ctrl(u32 handle,u32 cmd, u32 data1,u32 data2);


#endif
