/***************************************************************************
������PWM routine
ƽ̨�� 
ʱ�䣺2017-3-9
���ߣ�������                                                 
****************************************************************************/
#include "SCM6XX_PWM.h"
extern UINT8 port ;


/*-----------------------------------------------------------------------------
  Function:		PWM_Start                                                                                                                                                                            
  input:     	pwmx -- SGCC_PWMA0_P/SGCC_PWMA1_P/SGCC_PWMB0_P/SGCC_PWMB1_P
              period--���� 
              width--ռ�ձ�����
              div--Ԥ��Ƶ
              outRev--�������                  	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ����PWM�����ں�ռ�ձȲ�����                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void PWM_Start(SGCC_PWM_TypeDef* PWMx, UINT8 PWM_Period, UINT8 PWM_Width, UINT8 PWM_Div, UINT8 PWM_Out_Rev) 
{

	  PWMx->CONTROL = 0;
	  
	  PWMx->PERIOD = PWM_Period;
	  PWMx->WIDTH = PWM_Width;
	  PWMx->DIV = PWM_Div;
	  PWMx->CONTROL = (PWM_Out_Rev << PWM_CONTROL_REVB) | 0x5;
}

