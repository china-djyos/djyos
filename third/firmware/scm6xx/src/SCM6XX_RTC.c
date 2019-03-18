/***************************************************************************
描述：RTC routine
平台：
时间：2016-3-2
作者：张彦欣                                                 
****************************************************************************/
#include "SCM6XX_RTC.h"

/*-----------------------------------------------------------------------------
  Function:		RTC_RestoreReg                                                                                                                                                                           
  input:     	None                	  
  output:     NONE                                                                                    
  Returns:    NONE                                                                             
  Description: 将RTC寄存器恢复为上电复位值                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void RTC_Reset_Reg(void) 
{
	  SGCC_RTC_P->Control = 0x0;
	  SGCC_RTC_P->Time0 = 0x0;
    SGCC_RTC_P->Time1 = 0x0;
    SGCC_RTC_P->AlarmTime0 = 0x0;
    SGCC_RTC_P->AlarmTime1 = 0x0;
	  SGCC_RTC_P->Status = 0x30;
}

/*-----------------------------------------------------------------------------
  Function:		RTC_En                                                                                                                                                                           
  input:     	opt:ENABLE/DISABLE                	  
  output:     NONE                                                                                    
  Returns:    NONE                                                                             
  Description: 使能/不使能 RTC                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void RTC_Enable(FunctionalState opt) 
{
	 UINT32 temp;
	 temp = SGCC_RTC_P->Control;
	 
	 temp &= ~RTC_CTL_RTC_EN;
	 temp |= (opt << RTC_CTL_RTC_ENB);
   SGCC_RTC_P->Control = temp;
}

/*-----------------------------------------------------------------------------
  Function:		RTC_AlarmEn                                                                                                                                                                           
  input:     	opt:ENABLE/DISABLE                	  
  output:     NONE                                                                                    
  Returns:    NONE                                                                             
  Description: 使能/不使能 RTC ALARM                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void RTC_Alarm_Enable(FunctionalState opt) 
{
	 UINT32 temp;
	 temp = SGCC_RTC_P->Control;
	 
	 temp &= ~RTC_CTL_ALARM_EN;
	 temp |= (opt << RTC_CTL_ALARM_ENB);
   SGCC_RTC_P->Control = temp;
}

/*-----------------------------------------------------------------------------
  Function:		RTC_GetTime                                                                                                                                                                           
  input:     	None                	  
  output:     dateTime -- 从RTC读出的日历及时间参数                                                                                    
  Returns:    0--读取成功  1--读取失败                                                                                      
  Description: 读取实时时钟相关寄存器                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void RTC_Get_Time(DATETIME * Date_Time) 
{
	UINT32 temp1, temp2;
	
	//read current time register
	temp1 = SGCC_RTC_P->CurTime0;
	temp2 = SGCC_RTC_P->CurTime1;
    
	//get time information 
    Date_Time->secondU = temp1 & RTC_TIME0_BCD_SEC;
	Date_Time->secondD = (temp1 & RTC_TIME0_BCD_SEC_DEC) >> RTC_TIME0_BCD_SEC_DECBF;
    
    Date_Time->minuteU = (temp1 & RTC_TIME0_BCD_MIN) >> RTC_TIME0_BCD_MINBF;
	Date_Time->minuteD = (temp1 & RTC_TIME0_BCD_MIN_DEC) >> RTC_TIME0_BCD_MIN_DECBF;
	
    Date_Time->hourU = (temp1 & RTC_TIME0_BCD_HOUR) >> RTC_TIME0_BCD_HOURBF;
	Date_Time->hourD = (temp1 & RTC_TIME0_BCD_HOUR_DEC) >> RTC_TIME0_BCD_HOUR_DECBF;
    
    Date_Time->weekDay = (temp1 & RTC_TIME0_BCD_WEEKDAY) >> RTC_TIME0_BCD_WEEKDAYBF;
    
    Date_Time->dayU = (temp2 & RTC_TIME1_BCD_DATE) >> RTC_TIME1_BCD_DATEBF;
	Date_Time->dayD = (temp2 & RTC_TIME1_BCD_DATE_DEC) >> RTC_TIME1_BCD_DATE_DECBF;
    
    Date_Time->monthU = (temp2 & RTC_TIME1_BCD_MONTH) >> RTC_TIME1_BCD_MONTHBF;
	Date_Time->monthD = (temp2 & RTC_TIME1_BCD_MONTH_DEC) >> RTC_TIME1_BCD_MONTH_DECB;
    
    Date_Time->yearU = (temp2 & RTC_TIME1_BCD_YEAR) >> RTC_TIME1_BCD_YEARBF;
	Date_Time->yearD = (temp2 & RTC_TIME1_BCD_YEAR_DEC) >> RTC_TIME1_BCD_YEAR_DECBF;
}

/*-----------------------------------------------------------------------------
  Function:		RTC_SetTime                                                                                                                                                                           
  input:     	dateTime -- 需要设置的日历及时间参数                  	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 设置实时时钟相关寄存器                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void RTC_Set_Time(DATETIME Date_Time) 
{
	UINT32 temp = 0;
	
	//clear RTC_EN bit
	SGCC_RTC_P->Control &= ~RTC_CTL_RTC_EN;
	
	//check and read the RTC_EN_STATUS bit until 0 
	while(SGCC_RTC_P->Status & RTC_EN_STATUS);
	
	//clear all RTC interrupts
    SGCC_RTC_P->Status = RTC_ALARM_INT_STATUS|RTC_PRD_INT_STATUS;
	
	//set new time to register Time0
	temp = (Date_Time.weekDay << RTC_TIME0_BCD_WEEKDAYBF);
    temp|= (Date_Time.hourD << RTC_TIME0_BCD_HOUR_DECBF) ;
    temp|= (Date_Time.hourU << RTC_TIME0_BCD_HOURBF) ;
    temp|= (Date_Time.minuteD << RTC_TIME0_BCD_MIN_DECBF) ;
    temp|= (Date_Time.minuteU << RTC_TIME0_BCD_MINBF) ;
    temp|= (Date_Time.secondD << RTC_TIME0_BCD_SEC_DECBF) ;
    temp|= (Date_Time.secondU << RTC_TIME0_BCD_SECBF) ;
    
        
	SGCC_RTC_P->Time0 = temp;
    
    //set new time to register Time1
    temp = (Date_Time.yearD << RTC_TIME1_BCD_YEAR_DECBF) ;
    temp|= (Date_Time.yearU << RTC_TIME1_BCD_YEARBF) ;
    temp|= (Date_Time.monthD << RTC_TIME1_BCD_MONTH_DECB) ;
    temp|= (Date_Time.monthU << RTC_TIME1_BCD_MONTHBF) ;
    temp|= (Date_Time.dayD << RTC_TIME1_BCD_DATE_DECBF) ;
    temp|= (Date_Time.dayU << RTC_TIME1_BCD_DATEBF) ;
    
        
	SGCC_RTC_P->Time1 = temp;
	
    /*  //set RTC_EN bit*/
    SGCC_RTC_P->Control |= RTC_CTL_RTC_EN;
  
    //wait until RTC_EN_STATUS bit is changed to '1'
    while(!(SGCC_RTC_P->Status & RTC_EN_STATUS));
}

/*-----------------------------------------------------------------------------
  Function:	 RTC_EnablePeriodIntr                                                                                                                                                                           
  input:     intrType - periodic interrupt type, can be 	RTC_PINT_SECOND/RTC_PINT_MINUTE/RTC_PINT_HOUR/RTC_PINT_DAY/ RTC_PINT_MONTH
             operate - ENABLE/DISABLE  
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: 使能周期中断                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void RTC_Enable_Period_Int(UINT8 RTC_Int_Src, FunctionalState opt) 
{
    UINT32 temp = 0;
    if(opt == ENABLE)
    {
        temp = SGCC_RTC_P->Control ;  
        temp |= RTC_CTL_PRD_INT_EN;   //enable period interrupt
    
        temp &= ~RTC_CTL_PERINT_SEL;
    
	    switch(RTC_Int_Src)
        {
            case RTC_PINT_SECOND:
            temp |= RTC_CTL_PERINT_SEL_SECOND;  //select second interrupt
            break;
        
            case RTC_PINT_MINUTE:
            temp |= RTC_CTL_PERINT_SEL_MINUTE;  //select minute interrupt
            break;
        
            case RTC_PINT_HOUR:
            temp |= RTC_CTL_PERINT_SEL_HOUR;  //select hour interrupt
            break;
        
            case RTC_PINT_DAY:
            temp |= RTC_CTL_PERINT_SEL_DAY;  //select day interrupt
            break;
        
            case RTC_PINT_MONTH:
            temp |= RTC_CTL_PERINT_SEL_MONTH;  //select month interrupt
            break;
        
            default:
            break;
        } 
        SGCC_RTC_P->Control = temp;   
    }
    else
    {
        SGCC_RTC_P->Control &= ~(RTC_CTL_PERINT_SEL | RTC_CTL_PRD_INT_EN);  //disable periodic interrupt
    }
    
}


/*-----------------------------------------------------------------------------
  Function:		RTC_EnableAlarmIntr                                                                                                                                                                           
  input:     	alarmDateTime -- 产生闹钟中断的时间
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: 设置并使能闹钟中断                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void RTC_Enable_Alarm_Int(DATETIME Date_Time) 
{
	UINT32 temp = 0;
	
	//disable ALARM
	SGCC_RTC_P->Control &= ~RTC_CTL_ALARM_EN;
    //disable alarm interrupt
	SGCC_RTC_P->Control &= ~RTC_CTL_ALARM_INT_EN;
    
     //enable ALARM
	SGCC_RTC_P->Control |= RTC_CTL_ALARM_EN;
    
    
	//set new time to alarm registers 
  temp = (Date_Time.weekDay << RTC_TIME0_BCD_WEEKDAYBF);
  temp|= (Date_Time.hourD << RTC_TIME0_BCD_HOUR_DECBF) ;
  temp|= (Date_Time.hourU << RTC_TIME0_BCD_HOURBF) ;
  temp|= (Date_Time.minuteD << RTC_TIME0_BCD_MIN_DECBF) ;
  temp|= (Date_Time.minuteU << RTC_TIME0_BCD_MINBF) ;
  temp|= (Date_Time.secondD << RTC_TIME0_BCD_SEC_DECBF) ;
  temp|= (Date_Time.secondU << RTC_TIME0_BCD_SECBF) ;
    
        
	SGCC_RTC_P->AlarmTime0 = temp;
    
  //set new time to register Time1
  temp = (Date_Time.yearD << RTC_TIME1_BCD_YEAR_DECBF) ;
  temp|= (Date_Time.yearU << RTC_TIME1_BCD_YEARBF) ;
  temp|= (Date_Time.monthD << RTC_TIME1_BCD_MONTH_DECB) ;
  temp|= (Date_Time.monthU << RTC_TIME1_BCD_MONTHBF) ;
  temp|= (Date_Time.dayD << RTC_TIME1_BCD_DATE_DECBF) ;
  temp|= (Date_Time.dayU << RTC_TIME1_BCD_DATEBF) ;
    
  SGCC_RTC_P->AlarmTime1 = temp;

 
	//enable ALARM
  //SGCC_RTC_P->Control |= RTC_CTL_ALARM_EN;
  //enable alarm interrupt
	SGCC_RTC_P->Control |= RTC_CTL_ALARM_INT_EN;
}

/*-----------------------------------------------------------------------------
  Function:		RTC_DisableAlarmIntr                                                                                                                                                                           
  input:     	None
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: disable alarm interrupt                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void RTC_Disable_Alarm_Int(void) 
{
	//enable alarm interrupt
	SGCC_RTC_P->Control &= ~RTC_CTL_ALARM_INT_EN;
}


/*-----------------------------------------------------------------------------
  Function:		RTC_ClearIntr                                                                                                                                                                           
  input:     	intrType--interrupt type, can be RTC_PERIOD_INTR/RTC_ALARM_INTR                          	  
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: clear interrupt                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void RTC_Clear_Int(UINT8 Int_Type) 
{
    if(Int_Type == RTC_PERIOD_INTR)
        SGCC_RTC_P->Status = RTC_PRD_INT_STATUS;
    else if(Int_Type == RTC_ALARM_INTR)
	    SGCC_RTC_P->Status = RTC_ALARM_INT_STATUS;
} 

/*-----------------------------------------------------------------------------
  Function:		RTC_GetFlagStatus                                                                       
  input:    flag:which flag to get, can be RTC_PRD_INT_STATUS/RTC_ALARM_INT_STATUS/RTC_EN_STATUS      
  Returns:  None                                                                                        
  Description:  get UART status                                                                                         
               	                                  
 *-----------------------------------------------------------------------------*/
UINT8 RTC_Get_Flag_Status(UINT8 RTC_Flag)
{
    if(SGCC_RTC_P->Status & RTC_Flag)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}
