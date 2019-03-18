#ifndef _SCM6XX_SCU_H_
#define _SCM6XX_SCU_H_

#include "SCM6XX_ALL.h"


// ±÷”‘¥
#define RCLF  0
#define XTLF  1
#define RCHF  2
#define XTHF  3

/*interrupt source*/ 
#define SCU_INT_SRC_WAKEUP       (1 << 3)        
#define SCU_INT_SRC_VD           (1 << 2)        
#define SCU_INT_SRC_XLFDT        (1 << 1)        
#define SCU_INT_SRC_XHFDT        (1 << 0)        

/*wake source*/                      
#define WAKEUP_SRC_OTG_SUP              (1 << 16)
#define WAKEUP_SRC_UART0_RX             (1 << 15)
#define WAKEUP_SRC_M4_SW                (1 << 14)
#define WAKEUP_SRC_WDT_INT              (1 << 13)
#define WAKEUP_SRC_TIMERA1_INT          (1 << 12)
#define WAKEUP_SRC_RTC_PRD              (1 << 11)
#define WAKEUP_SRC_RTC_ALM              (1 << 10)
#define WAKEUP_SRC_GPIO9              (1 << 9)
#define WAKEUP_SRC_GPIO8             (1 << 8)
#define WAKEUP_SRC_GPIO7              (1 << 7)
#define WAKEUP_SRC_GPIO6             (1 << 6)
#define WAKEUP_SRC_GPIO5              (1 << 5)
#define WAKEUP_SRC_GPIO4             (1 << 4)
#define WAKEUP_SRC_GPIO3              (1 << 3)
#define WAKEUP_SRC_GPIO2            (1 << 2)
#define WAKEUP_SRC_GPIO1              (1 << 1)
#define WAKEUP_SRC_GPIO0            (1 << 0)
  

/*sleep mode*/
#define SLEEP_MODE       0
#define DEEP_SLEEP_MODE  1


void SCU_Enable_XHOSC_Stop_Detection(FunctionalState opt);
void SCU_Enable_XLOSC_Stop_Detection(FunctionalState opt);
void SCU_Enable_VD(UINT8 opt);
void SCU_Configure_GMAC_Mode(UINT8 macMode);
void SCU_Enable_FLASH_Standby(FunctionalState opt);
void SCU_Enable_Wakeup_Src(UINT32 src);
void SCU_Disable_Wakeup_Src(UINT32 src);
UINT8 SCU_Get_Wakeup_Status(UINT32 src);
void SCU_Enable_Int_Src(UINT32 intrType);
void SCU_Disable_Int_Src(UINT32 intrType);
UINT8 SCU_Get_Int_Status(UINT32 intrType);
void SCU_Clear_Int_Status(UINT32 intrType);
UINT8 SCU_Get_System_Status(UINT32 stsType);
void SCU_Clear_System_Status(UINT32 stsType);
void CACHE_Enable_Hit_Cnt(FunctionalState opt);
void CACHE_Reset_Hit_Cnt(FunctionalState opt);
UINT32 CACHE_Read_Hit_Cnt(void);
UINT8 CACHE_Read_Hit_Overflow_Flag(void);
void CACHE_Enable_Miss_Cnt(FunctionalState opt);
void CACHE_Reset_Miss_Cnt(FunctionalState opt);
UINT32 CACHE_Read_Miss_Cnt(void);
UINT8 CACHE_Read_Miss_Overflow_Flag(void);
#endif 

