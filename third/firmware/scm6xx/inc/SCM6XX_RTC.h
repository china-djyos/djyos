#ifndef _SCM6XX_RTC_H_
#define _SCM6XX_RTC_H_

#include "SCM6XX_ALL.h"

typedef struct 
{
	 UINT8 yearD;
	 UINT8 yearU;
	 UINT8 monthD;
	 UINT8 monthU;
	 UINT8 dayD;
	 UINT8 dayU;
	 UINT8 weekDay;
	 UINT8 hourD;
	 UINT8 hourU;
	 UINT8 minuteD;
	 UINT8 minuteU;
	 UINT8 secondD;
	 UINT8 secondU;
}DATETIME;

//interrupt type
#define RTC_PERIOD_INTR  0x20
#define RTC_ALARM_INTR   0x10

//period interrupt type
#define RTC_PINT_SECOND  1
#define RTC_PINT_MINUTE  2
#define RTC_PINT_HOUR    3
#define RTC_PINT_DAY     4
#define RTC_PINT_MONTH   5



/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
void RTC_Reset_Reg(void);
void RTC_Enable(FunctionalState opt);
void RTC_Alarm_Enable(FunctionalState opt);
void RTC_Get_Time(DATETIME * Date_Time);
void RTC_Set_Time(DATETIME Date_Time);
void RTC_Enable_Period_Int(UINT8 RTC_Int_Src, FunctionalState opt);
void RTC_Enable_Alarm_Int(DATETIME Date_Time);
void RTC_Disable_Alarm_Int(void);
void RTC_Clear_Int(UINT8 Int_Type);
UINT8 RTC_Get_Flag_Status(UINT8 RTC_Flag);

#endif 

