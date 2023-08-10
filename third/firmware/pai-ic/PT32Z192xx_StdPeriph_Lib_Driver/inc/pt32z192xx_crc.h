/**
  ******************************************************************************
  * @file    pt32z192xx_crc.h
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   This file contains all the functions prototypes for the CRC firmware 
  *          library.
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
#ifndef __PT32Z192XX_CRC_H
#define __PT32Z192XX_CRC_H

#ifdef __cplusplus
 extern "C" {
#endif

/*!< Includes ----------------------------------------------------------------*/
#include "pt32z192xx.h"

/** @addtogroup PT32Z192xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup CRC
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  LED Init structure definition
  */
  
typedef struct
{

  uint32_t CRC_DIN_Width;                      /*!< Defines the source of Comparator positive input. This parameter
                                                 can be a value of @ref  */						
	FunctionalState Bit_Input_Reverse;
	
  FunctionalState Byte_Input_Reverse;                /*!< Specifies whether the ADC data alignment is left or right.
                                                 This parameter can be a value of @ref ADC_data_align */
  FunctionalState Bit_Output_Reverse;                /*!< Specifies whether the ADC data alignment is left or right.
                                                 This parameter can be a value of @ref ADC_data_align */	
  uint32_t CRC_Seed;                      /*!< Defines the source of Comparator positive input. This parameter
                                                 can be a value of @ref  */	
  uint32_t CRC_Poly;                      /*!< Defines the source of Comparator positive input. This parameter
                                                 can be a value of @ref  */			
}CRC_InitTypeDef;


/* Exported constants --------------------------------------------------------*/

/** @defgroup 
  * @{
  */

#define IS_CRC_SEED(SEED) ((SEED) <= 0xFFFF)
#define IS_CRC_POLY(POLY) ((POLY) <= 0xFFFF)

#define CRC_DATAIN_X8                     0x00000000
#define CRC_DATAIN_X16                    0x00000004
#define IS_CRC_DINWIDTH(WIDTH) ((WIDTH == CRC_DATAIN_X8) || (WIDTH == CRC_DATAIN_X16))

/**
  * @}
  */

/** @defgroup CRC_PolynomialSize
  * @brief    
  * @{
  */


/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Configuration of the CRC computation unit **********************************/

/* CRC computation ************************************************************/


/* Independent register (IDR) access (write/read) *****************************/


#ifdef __cplusplus
}
#endif

#endif /* __STM32F0XX_CRC_H */

/**
  * @}
  */

/**
  * @}
  */


