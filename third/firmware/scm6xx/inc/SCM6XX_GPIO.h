#ifndef _SCM6XX_GPIO_H_
#define _SCM6XX_GPIO_H_

#include "SCM6XX_ALL.h"

//GPIO0 port number
#define GPIO0_PORTNUM 32

/*direction*/
#define GPIO_INPUT 0
#define GPIO_OUTPUT 1

/*high or low level*/
#define GPIO_HIGH  1
#define GPIO_LOW   0


/*interrupt mask or unmask*/
#define GPIO_INTR_MASK    1
#define GPIO_INTR_UNMASK  0

/*interrupt trigger edge*/
#define GPIO_INTR_HIGHLVEL  0
#define GPIO_INTR_LOWLEVEL  1
#define GPIO_INTR_RISINGEDGE  2
#define GPIO_INTR_FALLINGEDGE  3
#define GPIO_INTR_BOTHEDGE  4


/*debounce mode*/
#define GPIO_DEBOUNCE_NONE  0
#define GPIO_DEBOUNCE_HIGHPULSE  1
#define GPIO_DEBOUNCE_LOWPULSE  2
#define GPIO_DEBOUNCE_BOTHPULSE  3


/*debounce cycle*/
#define GPIO_DEBOUNCE_CYCLE_1  0
#define GPIO_DEBOUNCE_CYCLE_2  1
#define GPIO_DEBOUNCE_CYCLE_3  2
#define GPIO_DEBOUNCE_CYCLE_4  3



/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
void GPIO_Reset_Reg(void);
void GPIO_Configure_Dir(UINT8 Port_Num, UINT8 Port_Dir);
void GPIO_Write_Out(UINT8 Port_Num, UINT8 Port_Level);
UINT8 GPIO_Read_In(UINT8 Port_Num);
void GPIO_Enable_Int(UINT8 Port_Num, FunctionalState opt);
void GPIO_Mask_Int(UINT8 Port_Num, UINT8 Int_Mask);
void GPIO_Clear_Int_Flag(UINT8 Port_Num);
void GPIO_Configure_Int_Mode(UINT8 Port_Num, UINT8 Trig_Mode);
UINT8 GPIO_Read_Int_Mode(UINT8 Trig_Mode);
void GPIO_Configure_Debounce(UINT8 Port_Num, UINT8 Debounce_Mode, UINT8 Debounce_Cycle);
void GPIO_Enable_Syn(UINT8 Port_Num, FunctionalState opt);


#endif 




