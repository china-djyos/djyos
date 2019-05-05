#ifndef _SCM6XX_PWM_H_
#define _SCM6XX_PWM_H_

#include "SCM6XX_ALL.h"

/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
void PWM_Start(SGCC_PWM_TypeDef* PWMx, UINT8 PWM_Period, UINT8 PWM_Width, UINT8 PWM_Div, UINT8 PWM_Out_Rev);

#endif 

