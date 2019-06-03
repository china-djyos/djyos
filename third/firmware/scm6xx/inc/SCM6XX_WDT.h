#ifndef _SCM6XX_WDT_H_
#define _SCM6XX_WDT_H_

#include "SCM6XX_ALL.h"

/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
void WDT_Start(UINT32 WDT_Period, UINT8 WDT_Wake_Enable, UINT8  WDT_Reset_Enable, UINT8 WDT_Int_Enable);
void WDT_Disable(void);
void WDT_Reset_Start(void);
void WDT_Clear_Wake_Flag(void);
void WDT_Enable_Int(FunctionalState opt);

#endif 

