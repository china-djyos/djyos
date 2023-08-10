/**
  ******************************************************************************
  * @file    pt32z192xx_pwr.c
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Power Controller (PWR) peripheral:
  *           + Backup Domain Access
  *           + PVD configuration
  *           + WakeUp pins configuration
  *           + Low Power modes configuration
  *           + Flags management
  *
  ******************************************************************************
  * @attention
  *
  *
  *
  * 
  * 
  *
  *
  *      
  *
  *
  * 
  *
  *
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx_pwr.h"

/**
  ******************************************************************************
  * @file    Thor01_ana.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file provides all the AES firmware functions.
  ******************************************************************************
  ******************************************************************************
  */


/** @addtogroup Thor01_StdPeriph_Driver
  * @{
  */

void Power_Sleep(POWER_SLEEP_ENTRY_Enum SleepEntry)
{
   /* clear SLEEPDEEP bit of cortex System Control Register */
   Reset_SLEEPDEEP;

   if(SleepEntry == POWER_SLEEP_WFE)
   {
      __SEV();   //sev to set event register to 1, ensure event register set before WFE ,
      __WFE();   //WFE to clear event register to 0, so that the processor enter sleep mode                   
      __WFE();                    /* sleep then wait for event     */             
   }
   else
   {
      __WFI();                   /* sleep then wait for interrupt */
   }
}


void Power_DeepSleep(POWER_SLEEP_ENTRY_Enum SleepEntry)
{

   ww(0x40021004, 0x01);         //enable PMU 

   /* set SLEEPDEEP bit of cortex System Control Register */
   Set_SLEEPDEEP;

   if(SleepEntry == POWER_SLEEP_WFE)
   {
      __WFE();                    /* sleep then wait for event     */
      __WFE();                    
   }
   else
   {
      __WFI();                   /* sleep then wait for interrupt */
   }
   ww(0x40021004, 0x00);                  //disable PMU 
}


void Power_UPF(void)
{
   ww(0x40021004, 0x50490002);                  //enable LPPMU, 
}






/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
