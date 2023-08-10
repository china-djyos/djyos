/**
  ******************************************************************************
  * @file    pt32z192xx_adc.h
  * @author  应用开发团队
  * @version 
  * @date    
  * @brief   This file contains all the functions prototypes for the ADC firmware 
  *          library
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PT32Z192XX_ADC_H
#define __PT32Z192XX_ADC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx.h"

/** @addtogroup PT32Z192xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup ADC
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  ADC Init structure definition
  */
  
typedef struct
{

  uint32_t ADC_ConvMode;                   
	
	uint32_t ADC_Resolution;                  		
	
	uint32_t ADC_ClkDiv;                      		

  uint32_t ADC_ChSel; 

  FunctionalState ADC_DMA;
	
}ADC_InitTypeDef;




/* Exported constants --------------------------------------------------------*/

/** @defgroup ADC_Exported_Constants
  * @{
  */ 
#define IS_ADC_ALL_PERIPH(PERIPH)                  ((PERIPH) == ADC)


/** @defgroup ADC_ConvMode
  * @{
  */ 
#define ADC_ConvMode_OneShot               0x00000000   
#define ADC_ConvMode_TimTrigger            0x00000002   
#define ADC_ConvMode_Continue              0x00000004

#define IS_ADC_CONVMODE(MODE)   (((MODE) == ADC_ConvMode_OneShot)    ||\
				                         ((MODE) == ADC_ConvMode_TimTrigger) ||\
				                         ((MODE) == ADC_ConvMode_Continue))

/**
  * @}
  */ 


/** @defgroup ADC_Resolution
  * @{
  */ 
#define ADC_Resolution_12bit               0x00000000   
#define ADC_Resolution_10bit               0x00000004   
#define ADC_Resolution_8bit                0x00000008
#define ADC_Resolution_6bit                0x0000000C

#define IS_ADC_RESOLUTION(RESOLUTION)   (((RESOLUTION) == ADC_Resolution_12bit) ||\
				                                 ((RESOLUTION) == ADC_Resolution_10bit) ||\
				                                 ((RESOLUTION) == ADC_Resolution_8bit)  ||\
				                                 ((RESOLUTION) == ADC_Resolution_6bit))
/**
  * @}
  */ 
  
/** @defgroup ADC_ClkDiv
  * @{
  */ 
#define IS_ADC_CLOCKMODE(CLOCK)  ((1<=CLOCK)&&(CLOCK<=0xFF))


/** @defgroup ADC_ChSel
  * @{
  */ 
#define ADC_ChSel_0                  0x00000000   
#define ADC_ChSel_1                  0x00000010   
#define ADC_ChSel_2                  0x00000020
#define ADC_ChSel_3                  0x00000030
#define ADC_ChSel_4                  0x00000040
#define ADC_ChSel_5                  0x00000050
 
#define IS_ADC_CHSEL(CH)          (((CH) == ADC_ChSel_0) ||\
				                           ((CH) == ADC_ChSel_1) ||\
				                           ((CH) == ADC_ChSel_2) ||\
				                           ((CH) == ADC_ChSel_3) ||\
																	 ((CH) == ADC_ChSel_4) ||\
																	 ((CH) == ADC_ChSel_5))
/**
  * @}
  */ 



/** @defgroup ADC_channels 
  * @{
  */ 
  
#define ADC_Channel_0                              ADC_ChSel_0
#define ADC_Channel_1                              ADC_ChSel_1
#define ADC_Channel_2                              ADC_ChSel_2
#define ADC_Channel_3                              ADC_ChSel_3
#define ADC_Channel_4                              ADC_ChSel_4
#define ADC_Channel_5                              ADC_ChSel_5

#define IS_ADC_CHANNEL(CHANNEL)         (((CHANNEL) == ADC_Channel_0) ||\
                                         ((CHANNEL) == ADC_Channel_1) ||\
                                         ((CHANNEL) == ADC_Channel_2) ||\
                                         ((CHANNEL) == ADC_Channel_3) ||\
                                         ((CHANNEL) == ADC_Channel_4) ||\
                                         ((CHANNEL) == ADC_Channel_5))

/**
  * @}
  */ 
  
/** @defgroup ADC_interrupts_definition 
  * @{
  */ 

#define ADC_IT_EOC                                 ADC_IER_EOC
 
#define IS_ADC_CONFIG_IT(IT) (((IT) != (uint32_t)RESET) && (((IT) & 0xFFFFFFFE) == (uint32_t)RESET))

#define IS_ADC_GET_IT(IT)    ((IT) == ADC_IT_EOC)                     

#define IS_ADC_CLEAR_IT(IT)  (((IT) != (uint32_t)RESET) && (((IT) & 0xFFFFFFFE) == (uint32_t)RESET))

/**
  * @}
  */ 

/** @defgroup ADC_flags_definition 
  * @{
  */ 
  
#define ADC_FLAG_EOC                               ADC_SR_EOC

#define IS_ADC_CLEAR_FLAG(FLAG) (((FLAG) != (uint32_t)RESET) && (((FLAG) & 0xFFFFFFFE) == (uint32_t)RESET))

#define IS_ADC_GET_FLAG(FLAG) ((FLAG) == ADC_FLAG_EOC)     
                               
/**
  * @}
  */
  
/**
  * @}
  */ 

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 

/*  Function used to set the ADC configuration to the default reset state *****/


/* Initialization and Configuration functions *********************************/ 


/* This Function is obsolete and maintained for legacy purpose only.
   ADC_ClockModeConfig() function should be used instead */


/* Power saving functions *****************************************************/





/* Channels Configuration functions *******************************************/



/* Interrupts and flags management functions **********************************/

void ADC_Init(CMSDK_ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct);
void ADC_Cmd(CMSDK_ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_ClockModeConfig(CMSDK_ADC_TypeDef* ADCx, uint32_t ADC_ClockMode);
void ADC_HighSampModeCmd(CMSDK_ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_ChannelConfig(CMSDK_ADC_TypeDef* ADCx, uint32_t ADC_Channel);
void ADC_ContinuousModeCmd(CMSDK_ADC_TypeDef* ADCx, FunctionalState NewState);
uint32_t ADC_GetCalibrationFactor(CMSDK_ADC_TypeDef* ADCx);
void ADC_StopOfConversion(CMSDK_ADC_TypeDef* ADCx);
void ADC_StartOfConversion(CMSDK_ADC_TypeDef* ADCx);
uint16_t ADC_GetConversionValue(CMSDK_ADC_TypeDef* ADCx);
void ADC_ITConfig(CMSDK_ADC_TypeDef* ADCx, uint32_t ADC_IT, FunctionalState NewState);
FlagStatus ADC_GetFlagStatus(CMSDK_ADC_TypeDef* ADCx, uint32_t ADC_FLAG);
void ADC_ClearFlag(CMSDK_ADC_TypeDef* ADCx, uint32_t ADC_FLAG);
ITStatus ADC_GetITStatus(CMSDK_ADC_TypeDef* ADCx, uint32_t ADC_IT);


#ifdef __cplusplus
}
#endif

#endif /*__PT32Z192XX_ADC_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************  *****END OF FILE****/
