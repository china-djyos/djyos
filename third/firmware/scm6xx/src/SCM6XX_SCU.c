/***************************************************************************
描述：scu routine
平台：SCM6XX
时间：2017-9-12
作者：张彦欣                                                 
****************************************************************************/
#include "SCM6XX_SCU.h"


/*-----------------------------------------------------------------------------
  Function:		SCU_EnableXhfdt                                                                                                                                                                          
  input:      opt -- ENABLE/DISABLE	
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable XHOSC stop detection                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void SCU_Enable_XHOSC_Stop_Detection(FunctionalState opt) 
{
    if(opt == ENABLE)
    {
        SGCC_SCU_P->SYS_CTRL |= SYS_CTRL_XHFDT_EN;
    }
    else if(opt == DISABLE)
    {
        SGCC_SCU_P->SYS_CTRL &= ~SYS_CTRL_XHFDT_EN;
    }
}

/*-----------------------------------------------------------------------------
  Function:		SCU_EnableXlfdt                                                                                                                                                                          
  input:      opt -- ENABLE/DISABLE	
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable XLOSC stop detection                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void SCU_Enable_XLOSC_Stop_Detection(FunctionalState opt) 
{
    if(opt == ENABLE)
    {
        SGCC_SCU_P->SYS_CTRL |= SYS_CTRL_XLFDT_EN;
    }
    else if(opt == DISABLE)
    {
        SGCC_SCU_P->SYS_CTRL &= ~SYS_CTRL_XLFDT_EN;
    }
}


/*-----------------------------------------------------------------------------
  Function:		SCU_EnableVD                                                                                                                                                                          
  input:      opt -- ENABLE/DISABLE	
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable VD                                                                                              	                                   
 *-----------------------------------------------------------------------------*/
void SCU_Enable_VD(UINT8 opt) 
{
    if(opt == ENABLE)
    {
        SGCC_SCU_P->SYS_CTRL |= SYS_CTRL_VD_EN;
    }
    else if(opt == DISABLE)
    {
        SGCC_SCU_P->SYS_CTRL &= ~SYS_CTRL_VD_EN;
    }
}

/*-----------------------------------------------------------------------------
  Function:		SCU_ConfigGmacMode                                                                                                                                                                          
  input:      macMode -- configure GMAC mode,
                        SYS_CTRL_GMAC_MODE_MII / SYS_CTRL_GMAC_MODE_RMII
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: configure GMAC mode                                                                                              	                                   
 *-----------------------------------------------------------------------------*/
void SCU_Configure_GMAC_Mode(UINT8 macMode) 
{
    SGCC_SCU_P->SYS_CTRL &= ~SYS_CTRL_GMAC_MODE;
    SGCC_SCU_P->SYS_CTRL |= macMode;
     
}

/*-----------------------------------------------------------------------------
  Function:		SCU_EnFlashStdby                                                                                                                                                                          
  input:      opt -- ENABLE or DISABLE,
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable flash standby mode                                                                                              	                                   
 *-----------------------------------------------------------------------------*/
void SCU_Enable_FLASH_Standby(FunctionalState opt) 
{
    if(opt == ENABLE)
    {
        SGCC_SCU_P->SYS_CTRL |= SYS_CTRL_FLASH_STDBY_EN;
    }
    else if(opt == DISABLE)
    {
        SGCC_SCU_P->SYS_CTRL &= ~SYS_CTRL_FLASH_STDBY_EN;
    }
     
}

/*-----------------------------------------------------------------------------
  Function:		SCU_EnableWakeSrc                                                                                                                                                                          
  input:     	src:  WAKEUP_SRC_OTG_SUP/WAKEUP_SRC_UART0_RX/  WAKEUP_SRC_M4_SW  /            
                     WAKEUP_SRC_WDT_INT/ WAKEUP_SRC_TIMERA1_INT/WAKEUP_SRC_RTC_PRD /            
                    WAKEUP_SRC_RTC_ALM / WAKEUP_SRC_GPIO9 /  WAKEUP_SRC_GPIO8/            
                    WAKEUP_SRC_GPIO7~WAKEUP_SRC_GPIO0
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable  wakeup source                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void SCU_Enable_Wakeup_Src(UINT32 src) 
{
   SGCC_SCU_P->WAKEUP_EN = src;
}

/*-----------------------------------------------------------------------------
  Function:		SCU_DisableWakeSrc                                                                                                                                                                          
  input:     	src:  WAKEUP_SRC_OTG_SUP/WAKEUP_SRC_UART0_RX/  WAKEUP_SRC_M4_SW  /            
                     WAKEUP_SRC_WDT_INT/ WAKEUP_SRC_TIMERA1_INT/WAKEUP_SRC_RTC_PRD /            
                    WAKEUP_SRC_RTC_ALM / WAKEUP_SRC_GPIO9 /  WAKEUP_SRC_GPIO8/            
                    WAKEUP_SRC_GPIO7~WAKEUP_SRC_GPIO0
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable  wakeup source                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void SCU_Disable_Wakeup_Src(UINT32 src) 
{
   SGCC_SCU_P->WAKEUP_EN &= ~(src);
}

/*-----------------------------------------------------------------------------
  Function:		SCU_GetWakeEnSts                                                                                                                                                                          
  input:     	src:  WAKEUP_SRC_OTG_SUP/WAKEUP_SRC_UART0_RX/  WAKEUP_SRC_M4_SW  /            
                     WAKEUP_SRC_WDT_INT/ WAKEUP_SRC_TIMERA1_INT/WAKEUP_SRC_RTC_PRD /            
                    WAKEUP_SRC_RTC_ALM / WAKEUP_SRC_GPIO9 /  WAKEUP_SRC_GPIO8/            
                    WAKEUP_SRC_GPIO7~WAKEUP_SRC_GPIO0
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: get WAKE_EN status                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
UINT8 SCU_Get_Wakeup_Status(UINT32 src) 
{
   UINT32 temp;
   
   temp = SGCC_SCU_P->WAKEUP_EN;
   if(temp & src)
       return SUCCESS;
   else
       return ERROR;
   
}

/*-----------------------------------------------------------------------------
  Function:		SCU_EnableIntr                                                                                                                                                                          
  input:     	intrType:  SCU_INT_SRC_WAKEUP/SCU_INT_SRC_VD/SCU_INT_SRC_XLFDT/SCU_INT_SRC_XHFDT
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable interupt                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void SCU_Enable_Int_Src(UINT32 intrType) 
{
   SGCC_SCU_P->INT_EN |= intrType;
}




/*-----------------------------------------------------------------------------
  Function:		SCU_DisableIntr                                                                                                                                                                          
  input:     	intrType:  SCU_INT_SRC_WAKEUP/SCU_INT_SRC_VD/SCU_INT_SRC_XLFDT/SCU_INT_SRC_XHFDT
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: disable interupt                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void SCU_Disable_Int_Src(UINT32 intrType) 
{
   SGCC_SCU_P->INT_EN &= ~intrType;
}

/*-----------------------------------------------------------------------------
  Function:		SCU_GetIntrSts                                                                                                                                                                          
  input:     	intrType:  SCU_INT_SRC_WAKEUP/SCU_INT_SRC_VD/SCU_INT_SRC_XLFDT/SCU_INT_SRC_XHFDT
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable interupt                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
UINT8 SCU_Get_Int_Status(UINT32 intrType) 
{
   UINT32 temp;
   
   temp = SGCC_SCU_P->INT_STS;
   if(temp & intrType)
       return SUCCESS;
   else
       return ERROR;
}

/*-----------------------------------------------------------------------------
  Function:		SCU_ClearIntr                                                                                                                                                                          
  input:     	intrType:  SCU_INT_SRC_WAKEUP/SCU_INT_SRC_VD/SCU_INT_SRC_XLFDT/SCU_INT_SRC_XHFDT
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable interupt                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void SCU_Clear_Int_Status(UINT32 intrType) 
{
   SGCC_SCU_P->INT_STS = intrType;
}

/*-----------------------------------------------------------------------------
  Function:		SCU_GetSysSts                                                                                                                                                                          
  input:     	stsType:  SYS_STS_CPU_LOCKUP_STS/SYS_STS_VD_STS/SYS_STS_XLFDT_STS/SYS_STS_XHFDT_STS
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: get system status                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
UINT8 SCU_Get_System_Status(UINT32 stsType) 
{
   UINT32 temp;
   
   temp = SGCC_SCU_P->SYS_STS;
   if(temp & stsType)
       return SUCCESS;
   else
       return ERROR;
}

/*-----------------------------------------------------------------------------
  Function:		SCU_ClearSysSts                                                                                                                                                                          
  input:     	stsType:  SYS_STS_CPU_LOCKUP_STS/SYS_STS_VD_STS/SYS_STS_XLFDT_STS/SYS_STS_XHFDT_STS
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: clear status                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void SCU_Clear_System_Status(UINT32 stsType) 
{
   SGCC_SCU_P->SYS_STS = stsType;
}


/*-----------------------------------------------------------------------------
  Function:		CACHE_EnableHitCnt                                                                                                                                                                         
  input:     	opt -- ENABLE/DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable hit counter                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void CACHE_Enable_Hit_Cnt(FunctionalState opt) 
{
    if(opt == ENABLE)
    {
		    SGCC_SCU_P->CACHE_HIT |= CACHE_HIT_EN;
		}
		else
		{ 
			  SGCC_SCU_P->CACHE_HIT &= ~CACHE_HIT_EN;
		}
}

/*-----------------------------------------------------------------------------
  Function:		CACHE_RstHitCnt                                                                                                                                                                         
  input:     	opt -- ENABLE/DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: reset hit related counter                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void CACHE_Reset_Hit_Cnt(FunctionalState opt) 
{
    SGCC_SCU_P->CACHE_HIT |= CACHE_HIT_RST;
	  
	  //wait the bit clears
	  while(SGCC_SCU_P->CACHE_HIT & CACHE_HIT_RST);
		
}

/*-----------------------------------------------------------------------------
  Function:		CACHE_ReadHitCnt                                                                                                                                                                         
  input:     	None
  output:     None                                                                                    
  Returns:    hitCnt                                                                                      
  Description: read hit counter                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
UINT32 CACHE_Read_Hit_Cnt(void) 
{
	  UINT32 temp;
	  
	  temp = SGCC_SCU_P->CACHE_HIT & CACHE_HIT_CNT;
	  
	  return temp;
}

/*-----------------------------------------------------------------------------
  Function:		CACHE_ReadHitOvFlow                                                                                                                                                                         
  input:     	None
  output:     None                                                                                    
  Returns:    hitCnt                                                                                      
  Description: read hit overflow flag                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
UINT8 CACHE_Read_Hit_Overflow_Flag(void) 
{
	  UINT32 temp;
	  
	  temp = SGCC_SCU_P->CACHE_HIT;
	  if(temp & CACHE_HIT_OVF)
       return SUCCESS;
    else
       return ERROR;
}

/*-----------------------------------------------------------------------------
  Function:		CACHE_EnableMissCnt                                                                                                                                                                         
  input:     	opt -- ENABLE/DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable miss counter                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void CACHE_Enable_Miss_Cnt(FunctionalState opt) 
{
    if(opt == ENABLE)
    {
		    SGCC_SCU_P->CACHE_MISS |= CACHE_MISS_EN;
		}
		else
		{ 
			  SGCC_SCU_P->CACHE_MISS &= ~CACHE_MISS_EN;
		}
}

/*-----------------------------------------------------------------------------
  Function:		CACHE_RstMissCnt                                                                                                                                                                         
  input:     	opt -- ENABLE/DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: reset Miss related counter                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void CACHE_Reset_Miss_Cnt(FunctionalState opt) 
{
    SGCC_SCU_P->CACHE_MISS |= CACHE_MISS_RST;
	  
	  //wait the bit clears
	  while(SGCC_SCU_P->CACHE_MISS & CACHE_MISS_RST);
		
}

/*-----------------------------------------------------------------------------
  Function:		CACHE_ReadMissCnt                                                                                                                                                                         
  input:     	None
  output:     None                                                                                    
  Returns:    hitCnt                                                                                      
  Description: read Miss counter                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
UINT32 CACHE_Read_Miss_Cnt(void) 
{
	  UINT32 temp;
	  
	  temp = SGCC_SCU_P->CACHE_MISS & CACHE_MISS_CNT;
	  
	  return temp;
	  		
}

/*-----------------------------------------------------------------------------
  Function:		CACHE_ReadMissOvFlow                                                                                                                                                                         
  input:     	None
  output:     None                                                                                    
  Returns:    hitCnt                                                                                      
  Description: read Miss overflow flag                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
UINT8 CACHE_Read_Miss_Overflow_Flag(void) 
{
	  UINT32 temp;
	  
	  temp = SGCC_SCU_P->CACHE_MISS;
	  if(temp & CACHE_MISS_OVF)
       return SUCCESS;
    else
       return ERROR;
}
