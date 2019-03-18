/***************************************************************************
描述：PWM routine
平台： 
时间：2017-3-9
作者：张彦欣                                                 
****************************************************************************/
#include "SCM6XX_PWM.h"
extern UINT8 port ;


/*-----------------------------------------------------------------------------
  Function:		PWM_Start                                                                                                                                                                            
  input:     	pwmx -- SGCC_PWMA0_P/SGCC_PWMA1_P/SGCC_PWMB0_P/SGCC_PWMB1_P
              period--周期 
              width--占空比配置
              div--预分频
              outRev--输出反向                  	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 配置PWM的周期和占空比并启动                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void PWM_Start(SGCC_PWM_TypeDef* PWMx, UINT8 PWM_Period, UINT8 PWM_Width, UINT8 PWM_Div, UINT8 PWM_Out_Rev) 
{

	  PWMx->CONTROL = 0;
	  
	  PWMx->PERIOD = PWM_Period;
	  PWMx->WIDTH = PWM_Width;
	  PWMx->DIV = PWM_Div;
	  PWMx->CONTROL = (PWM_Out_Rev << PWM_CONTROL_REVB) | 0x5;
}

