/**
  ******************************************************************************
  * @file    pt32z192xx_cmp_opa.h
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
#ifndef __PT32Z192XX_CMP_OPA_H
#define __PT32Z192XX_CMP_OPA_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx.h"

/** @addtogroup PT32Z192xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup Analog comparators and operational amplifiers
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  LED Init structure definition
  */

typedef struct
{

  uint32_t CMP_PosInputSEL;                    /*!< Defines the source of Comparator positive input. This parameter
                                                 can be a value of @ref  */
    uint32_t CMP_FilterSampling;                  /*!<Digital filter sampling times.*/

    FunctionalState CMP_HSMode;

  FunctionalState CMP_PolarityReversal;                /*!< Specifies whether the ADC data alignment is left or right.
                                                 This parameter can be a value of @ref ADC_data_align */
}CMPOPA_InitTypeDef;


/* Exported constants --------------------------------------------------------*/



/** @defgroup CMPOPA_Exported_Constants
  * @{
  */
#define IS_CMPOPA_ALL_PERIPH(PERIPH)                  (((PERIPH) == CMPOPA0) || ((PERIPH) == CMPOPA1))
#define IS_CMPOPA_INITDELAY_VALUE(VALUE) ((VALUE) <= 0x3FFF)
#define IS_CMPOPA_DAC_VALUE(VALUE)       ((VALUE) <= 0x1F)

/** @defgroup CMP_FEFSEL
  * @{
  */
#define CMP_REFSEL_PIN                         CMPOPA_CR1_REFSEL_PIN   /*!<  Synchronous clock mode no divided*/
#define CMP_REFSEL_DAC                         CMPOPA_CR1_REFSEL_DAC   /*!<  Synchronous clock mode divided by 2 */
#define CMP_REFSEL_BG                          CMPOPA_CR1_REFSEL_BG   /*!<  Synchronous clock mode divided by 4 */
#define IS_CMP_REFSEL(SOURCE)  (((SOURCE) == CMP_REFSEL_PIN) ||\
                                        ((SOURCE) == CMP_REFSEL_DAC) ||\
                                        ((SOURCE) == CMP_REFSEL_BG))
/**
  * @}
  */

/** @defgroup Digital filtering sampling times
  * @{
  */
#define CMP_FSTIMES_NONE                       CMPOPA_CR1_FS_NONE   /*!<  Synchronous clock mode no divided*/
#define CMP_FSTIMES_X2                         CMPOPA_CR1_FS_X2     /*!<  Synchronous clock mode divided by 2 */
#define CMP_FSTIMES_X4                         CMPOPA_CR1_FS_X4     /*!<  Synchronous clock mode divided by 2 */
#define CMP_FSTIMES_X8                         CMPOPA_CR1_FS_X8     /*!<  Synchronous clock mode divided by 2 */
#define CMP_FSTIMES_X16                        CMPOPA_CR1_FS_X16    /*!<  Synchronous clock mode divided by 2 */
#define CMP_FSTIMES_X32                        CMPOPA_CR1_FS_X32    /*!<  Synchronous clock mode divided by 2 */
#define CMP_FSTIMES_X64                        CMPOPA_CR1_FS_X64    /*!<  Synchronous clock mode divided by 2 */
#define IS_CMP_FSTIMES(TIMES)  (((TIMES) == CMP_FSTIMES_NONE) ||\
                                        ((TIMES) == CMP_FSTIMES_X2) ||\
                                        ((TIMES) == CMP_FSTIMES_X4) ||\
                                        ((TIMES) == CMP_FSTIMES_X8) ||\
                                        ((TIMES) == CMP_FSTIMES_X16) ||\
                                        ((TIMES) == CMP_FSTIMES_X32) ||\
                                        ((TIMES) == CMP_FSTIMES_X64))
/**
  * @}
  */

/** @defgroup CMP_interrupts_definition
  * @{
  */

#define CMP_IT_FALL                                 CMPOPA_IE_FALL
#define CMP_IT_RISE                                 CMPOPA_IE_RISE
#define CMP_IT_FALL_WEAK                            CMPOPA_IE_FALL_WEAK
#define CMP_IT_RISE_WEAK                            CMPOPA_IE_RISE_WEAK

#define IS_CMP_CONFIG_IT(IT) (((IT) != (uint32_t)RESET) && (((IT) & 0xFFFFFFF0) == (uint32_t)RESET))

#define IS_CMP_GET_IT(IT)    (((IT) == CMP_IT_FALL) ||\
                                      ((IT) == CMP_IT_RISE) ||\
                                      ((IT) == CMP_IT_FALL_WEAK) ||\
                                      ((IT) == CMP_IT_RISE_WEAK))

#define IS_CMP_CLEAR_IT(IT)  (((IT) != (uint32_t)RESET) && (((IT) & 0xFFFFFFF0) == (uint32_t)RESET))

/**
  * @}
  */

/** @defgroup CMP_flags_definition
  * @{
  */

#define CMP_FLAG_FALL                                 CMPOPA_EIS_FALL
#define CMP_FLAG_RISE                                 CMPOPA_EIS_RISE
#define CMP_FLAG_FALL_WEAK                            CMPOPA_EIS_FALL_WEAK
#define CMP_FLAG_RISE_WEAK                            CMPOPA_EIS_RISE_WEAK

#define IS_CMP_GET_FLAG(FLAG) (((FLAG) == CMP_FLAG_FALL) ||\
                                       ((FLAG) == CMP_FLAG_RISE) ||\
                                       ((FLAG) == CMP_FLAG_FALL_WEAK) ||\
                                       ((FLAG) == CMP_FLAG_RISE_WEAK))

#define IS_CMP_CLEAR_FLAG(FLAG)  (((FLAG) != (uint32_t)RESET) && (((FLAG) & 0xFFFFFFF0) == (uint32_t)RESET))

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


#ifdef __cplusplus
}
#endif

#endif /*__PT32Z192XX_CMP_OPA_H */

/**
  * @}
  */

/**
  * @}
  */

/************************  *****END OF FILE****/
