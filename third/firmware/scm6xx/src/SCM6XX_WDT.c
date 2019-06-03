/***************************************************************************
描述：WDT routine
平台： 
时间：2016-3-9
作者：张彦欣                                                 
****************************************************************************/
#include "SCM6XX_WDT.h"


/*-----------------------------------------------------------------------------
  Function:		WDT_Start                                                                                                                                                                            
  input:     	period--overflow period, can be
                SYSWDT_CTRL_OTSET_3906us/SYSWDT_CTRL_OTSET_7812us/
                SYSWDT_CTRL_OTSET_15625us/SYSWDT_CTRL_OTSET_31250us/
                SYSWDT_CTRL_OTSET_62500us/SYSWDT_CTRL_OTSET_125ms/
                SYSWDT_CTRL_OTSET_250ms/SYSWDT_CTRL_OTSET_500ms/
                SYSWDT_CTRL_OTSET_1s/SYSWDT_CTRL_OTSET_2s   /  SYSWDT_CTRL_OTSET_4s   /
                SYSWDT_CTRL_OTSET_8s/SYSWDT_CTRL_OTSET_12s/SYSWDT_CTRL_OTSET_16s/
                SYSWDT_CTRL_OTSET_24s/SYSWDT_CTRL_OTSET_32s
                
               wakAble--DISABLE/ENABLE
               rstAble--DISABLE/ENABLE
               intEn--DISABLE/ENABLE
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 配置WDT的溢出周期并启动                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void WDT_Start(UINT32 WDT_Period, UINT8 WDT_Wake_Enable, UINT8  WDT_Reset_Enable, UINT8 WDT_Int_Enable) 
{
	  UINT32 temp = 0;
	  
	  temp |= (WDT_Period | 0x1);
	  
	  if(WDT_Wake_Enable)
	      temp |= SYSWDT_CTRL_WAKABLE ;
	  if(WDT_Reset_Enable)
	  	  temp |= SYSWDT_CTRL_RSTABLE;
	  if(WDT_Int_Enable)
	  	  temp |= SYSWDT_CTRL_INT_EN;
	  
	  SGCC_WDT_P->CTRL = temp;
}

/*-----------------------------------------------------------------------------
  Function:		WDT_Disable                                                                                                                                                                            
  input:     	None                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 不使能WDT                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void WDT_Disable(void) 
{
	  SGCC_WDT_P->CTRL = 0x0;
}

/*-----------------------------------------------------------------------------
  Function:		WDT_Restart                                                                                                                                                                            
  input:     	None                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 重启WDT                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void WDT_Reset_Start(void) 
{
	  UINT32 temp;
	  
	  //输入清WDT的密码
	  temp = SGCC_WDT_P->CTRL;
	  temp |= SYSWDT_CTRL_PASSWORD_WDTCLR;
	   SGCC_WDT_P->CTRL = temp;
	  
	  SGCC_WDT_P->CLR = 0xa5;
}


/*-----------------------------------------------------------------------------
  Function:		WDT_ClrWakeFlag                                                                                                                                                                            
  input:     	None                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 清唤醒标志                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void WDT_Clear_Wake_Flag(void) 
{
	  SGCC_WDT_P->CTRL |= SYSWDT_CTRL_CLRWAKE;
}


/*-----------------------------------------------------------------------------
  Function:		WDT_EnableIntr                                                                                                                                                                            
input:        opt:ENABLE/DISABLE                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable interrupt                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void WDT_Enable_Int(FunctionalState opt) 
{
    if(opt == ENABLE)
	    SGCC_WDT_P->CTRL |= SYSWDT_CTRL_INT_EN;
    else
        SGCC_WDT_P->CTRL &= ~SYSWDT_CTRL_INT_EN;
}
