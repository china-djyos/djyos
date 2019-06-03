#ifndef _SCM6XX_ANA_CTRL_H_
#define _SCM6XX_ANA_CTRL_H_

#include "SCM6XX_ALL.h"

typedef struct
{
	UINT32 smpWidth; //ADC_CON1_SMP_WIDTH_(n)
    UINT32 adcMode;  //ADC_CON1_ADC_MODE_0~ADC_CON1_ADC_MODE_3
    UINT32 trigEn;   //ENABLE/DISABLE
    UINT32 avgTime; //ADC_CON1_AVGTIMES_1/ADC_CON1_AVGTIMES_2/ADC_CON1_AVGTIMES_4/ADC_CON1_AVGTIMES_8
    UINT32 chSel; //ADC_CON3_MCHS_(n)
    UINT32 chCombined; //ADC_CON3_CHNCOMBINED_1/ADC_CON3_CHNCOMBINED_0
} ADC_PARA ;


void ADC_Enable_Chn_Int(FunctionalState opt);
void ADC_Enable_Calib_Int(FunctionalState opt);
void ADC_Enable_Samp_Int(FunctionalState opt);
UINT8 ADC_Get_Flag_Status (UINT32 ADC_Flag);
void ADC_Clear_Int_Status(UINT32 ADC_Int_Status);
UINT8 ADC_Get_Ready_Status (UINT32 ADC_Ready_Flag);
void ADC_Config(ADC_PARA 	ADC_Para);
void ADC_Reset(FunctionalState opt);
void ADC_SetCs(FunctionalState opt);
void ADC_Set_Cs(FunctionalState opt);
void ADC_Select_Ch(UINT8 chNo);
UINT16 ADC_Get_Data(UINT8 chNo);
void ADC_Enable(FunctionalState opt);
void ADC_Enable_Auto_Cal(FunctionalState opt);
#endif 

