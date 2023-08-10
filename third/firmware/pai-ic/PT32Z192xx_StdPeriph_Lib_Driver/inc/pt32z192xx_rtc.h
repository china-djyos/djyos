#ifndef __PT32Z192XX_RTC_H
#define __PT32Z192XX_RTC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx.h"

typedef enum
{
  External_32K   = 0x00,    /*!< GPIO Input Mode              */
	
  External_DIV_24M,         /*!< GPIO Output Mode             */
	
}RTC_CLOCK_TypeDef;


typedef struct
{ 
  uint32_t           MatchValue;            /*!< Value for MR register             */
  uint32_t           Loadvalue;             /*!< Value for LR register             */
	uint32_t           SourceClock;           /*!< Select RTC clock source           */
} RTC_InitTypeDef;

#define RTC_INT                         0x01
#define RTC_RISR                        0x01
#define RTC_IR                          0x01


#define IS_RTC_CLOCK(SCLOCK)            (((SCLOCK) == External_32K) || ((SCLOCK) == External_DIV_24M) )
#define	IS_RTC_ALL_PERIPH(PERIPH)       ((PERIPH) == RTC)
#define	IS_RTC_MatchValue(Value)        ((Value) < 0xFFFFFFFF)
#define	IS_RTC_Loadvalue(Value)         ((Value) < 0xFFFFFFFF)
#define IS_RTC_INT(RTC_IT)              (RTC_IT == RTC_INT)
#define IS_RTC_RISR_FLAG(FLAG)          (FLAG==RTC_RISR)
#define IS_RTC_IR_FLAG(FLAG)            (FLAG==RTC_IR)

void RTC_Init(CMSDK_RTC_TypeDef* RTCx, RTC_InitTypeDef* RTC_Init);
void RTC_BaseStructInit(RTC_InitTypeDef* RTC_BaseInit);
void RTC_Cmd(CMSDK_RTC_TypeDef* RTCx, FunctionalState NewState);
void RTC_ITConfig(CMSDK_RTC_TypeDef* RTCx, uint32_t RTC_IT, FunctionalState NewState);
FlagStatus RTC_GetFlagStatus(CMSDK_RTC_TypeDef* RTCx, uint32_t RTC_FLAG_Status);
FlagStatus RTC_GetFlagINT(CMSDK_RTC_TypeDef* RTCx, uint32_t RTC_FLAG_INT);
void RTC_ClearFlag(CMSDK_RTC_TypeDef* RTCx, uint32_t RTC_FLAG);
void RTC_Load(CMSDK_RTC_TypeDef* RTCx, uint32_t Loadvalue);
void RTC_Match(CMSDK_RTC_TypeDef* RTCx, uint32_t Matchvalue);




#ifdef __cplusplus
}
#endif





#endif /* __PT32Z192XX_RTC_H */

	
/************************  *****END OF FILE****/

