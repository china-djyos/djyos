/**
  ******************************************************************************
  * @file    pt32z192xx_spi.h
  * @author  应用开发团队
  * @version 
  * @date    
  * @brief   
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PT32Z192XX_SPI_H
#define __PT32Z192XX_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx.h"

/** @addtogroup PT32Z192xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup SPI
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  SPI Init structure definition  
  */

typedef struct
{
  uint16_t SPI_Direction;           /*!< Specifies the SPI unidirectional or bidirectional data mode.
                                         This parameter can be a value of @ref SPI_data_direction */

  uint16_t SPI_Mode;                /*!< Specifies the SPI mode (Master/Slave).
                                         This parameter can be a value of @ref SPI_mode */
  
  uint16_t SPI_DataSize;            /*!< Specifies the SPI data size.
                                         This parameter can be a value of @ref SPI_data_size */

  uint16_t SPI_CPOL;                /*!< Specifies the serial clock steady state.
                                         This parameter can be a value of @ref SPI_Clock_Polarity */

  uint16_t SPI_CPHA;                /*!< Specifies the clock active edge for the bit capture.
                                         This parameter can be a value of @ref SPI_Clock_Phase */

  uint16_t SPI_NSS;                 /*!< Specifies whether the NSS signal is managed by
                                         hardware (NSS pin) or by software using the SSI bit.
                                         This parameter can be a value of @ref SPI_Slave_Select_management */
 
  uint16_t SPI_BaudRatePrescaler;   /*!< Specifies the Baud Rate prescaler value which will be
                                         used to configure the transmit and receive SCK clock.
                                         This parameter can be a value of @ref SPI_BaudRate_Prescaler
                                         @note The communication clock is derived from the master
                                               clock. The slave clock does not need to be set. */
  uint16_t SPI_BaudRatePostPrescaler; /*!< Specifies the Baud Rate prescaler value which will be
                                         used to configure the transmit and receive SCK clock.
                                         This parameter can be a value of @ref SPI_BaudRate_Prescaler
                                         @note The communication clock is derived from the master
                                               clock. The slave clock does not need to be set. */																							 
																							 
}SPI_InitTypeDef;



/* Exported constants --------------------------------------------------------*/

/** @defgroup SPI_Exported_Constants
  * @{
  */

#define IS_SPI_ALL_PERIPH(PERIPH) (((PERIPH) == SPI0) || \
                                   ((PERIPH) == SPI1))
                                   

/** @defgroup SPI_mode 
  * @{
  */

#define SPI_Mode_Master                 ((uint16_t)0x0000)
#define SPI_Mode_Slave                  ((uint16_t)0x0004)
#define IS_SPI_MODE(MODE) (((MODE) == SPI_Mode_Master) || \
                           ((MODE) == SPI_Mode_Slave))
/**
  * @}
  */

/** @defgroup SPI_data_size
  * @{
  */

#define SPI_DataSize_4b                  SPI_CR0_DSS_4B
#define SPI_DataSize_5b                  SPI_CR0_DSS_5B
#define SPI_DataSize_6b                  SPI_CR0_DSS_6B
#define SPI_DataSize_7b                  SPI_CR0_DSS_7B
#define SPI_DataSize_8b                  SPI_CR0_DSS_8B
#define SPI_DataSize_9b                  SPI_CR0_DSS_9B
#define SPI_DataSize_10b                 SPI_CR0_DSS_10B
#define SPI_DataSize_11b                 SPI_CR0_DSS_11B
#define SPI_DataSize_12b                 SPI_CR0_DSS_12B
#define SPI_DataSize_13b                 SPI_CR0_DSS_13B
#define SPI_DataSize_14b                 SPI_CR0_DSS_14B
#define SPI_DataSize_15b                 SPI_CR0_DSS_15B
#define SPI_DataSize_16b                 SPI_CR0_DSS_16B
#define IS_SPI_DATA_SIZE(SIZE) (((SIZE) == SPI_DataSize_4b) || \
                                 ((SIZE) == SPI_DataSize_5b) || \
                                 ((SIZE) == SPI_DataSize_6b) || \
                                 ((SIZE) == SPI_DataSize_7b) || \
                                 ((SIZE) == SPI_DataSize_8b) || \
                                 ((SIZE) == SPI_DataSize_9b) || \
                                 ((SIZE) == SPI_DataSize_10b) || \
                                 ((SIZE) == SPI_DataSize_11b) || \
                                 ((SIZE) == SPI_DataSize_12b) || \
                                 ((SIZE) == SPI_DataSize_13b) || \
                                 ((SIZE) == SPI_DataSize_14b) || \
                                 ((SIZE) == SPI_DataSize_15b) || \
                                 ((SIZE) == SPI_DataSize_16b))
/**
  * @}
  */


/** @defgroup SPI_Clock_Polarity 
  * @{
  */

#define SPI_CPOL_Low                    ((uint16_t)0x0000)
#define SPI_CPOL_High                   SPI_CR0_CPOL
#define IS_SPI_CPOL(CPOL) (((CPOL) == SPI_CPOL_Low) || \
                           ((CPOL) == SPI_CPOL_High))
/**
  * @}
  */

/** @defgroup SPI_Clock_Phase 
  * @{
  */

#define SPI_CPHA_1Edge                  ((uint16_t)0x0000)
#define SPI_CPHA_2Edge                  SPI_CR0_CPHA
#define IS_SPI_CPHA(CPHA) (((CPHA) == SPI_CPHA_1Edge) || \
                           ((CPHA) == SPI_CPHA_2Edge))
/**
  * @}
  */

/** @defgroup SPI_Slave_Select_management 
  * @{
  */

#define SPI_NSS_Soft                    SPI_ICR_SWSEL
#define SPI_NSS_Hard                    ((uint16_t)0x0000)
#define IS_SPI_NSS(NSS) (((NSS) == SPI_NSS_Soft) || \
                         ((NSS) == SPI_NSS_Hard))
/**
  * @}
  */

/** @defgroup SPI_BaudRate_Prescaler 
  * @{
  */

#define SPI_BaudRatePrescaler_2         ((uint16_t)0x0002)
#define SPI_BaudRatePrescaler_4         ((uint16_t)0x0004)
#define SPI_BaudRatePrescaler_8         ((uint16_t)0x0008)
#define SPI_BaudRatePrescaler_16        ((uint16_t)0x0010)
#define SPI_BaudRatePrescaler_32        ((uint16_t)0x0020)
#define SPI_BaudRatePrescaler_64        ((uint16_t)0x0040)
#define SPI_BaudRatePrescaler_128       ((uint16_t)0x0080)
#define SPI_BaudRatePrescaler_256       ((uint16_t)0x0100)
#define IS_SPI_BAUDRATE_PRESCALER(PRESCALER) (((PRESCALER) == SPI_BaudRatePrescaler_2) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_4) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_8) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_16) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_32) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_64) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_128) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_256))
																							
																							
#define SPI_BaudRatePostPrescaler_1  		((uint16_t)0x0002)																					
#define IS_SPI_BAUDRATE_POSTPRESCALER(POSTPRESCALER) ( 1<=( POSTPRESCALER )<=255 )																							
/**
  * @}
  */




/**
  * @}
  */
/** @defgroup SPI_NSS_internal_software_management 
  * @{
  */

#define SPI_NSSInternalSoft_Set         SPI_ICR_SWCS
#define SPI_NSSInternalSoft_Reset       ((uint16_t)0xFFF7)
#define IS_SPI_NSS_INTERNAL(INTERNAL) (((INTERNAL) == SPI_NSSInternalSoft_Set) || \
                                       ((INTERNAL) == SPI_NSSInternalSoft_Reset))
/**
  * @}
  */


/**
  * @}
  */

/** @defgroup SPI_interrupts_definition 
  * @{
  */

#define SPI_IT_RFOF                  SPI_IE_RFOF
#define SPI_IT_RFNEOT                SPI_IE_RFNEOT
#define SPI_IT_RFHF                  SPI_IE_RFHF
#define SPI_IT_TFHF                  SPI_IE_TFHF

#define IS_SPI_CONFIG_IT(IT) (((IT) == SPI_IT_RFOF) || \
                                  ((IT) == SPI_IT_RFNEOT) || \
                                  ((IT) == SPI_IT_RFHF) || \
                                  ((IT) == SPI_IT_TFHF))
																	
#define IS_SPI_GET_IT(IT) (((IT) == SPI_IT_RFOF) || \
                                  ((IT) == SPI_IT_RFNEOT) || \
                                  ((IT) == SPI_IT_RFHF) || \
                                  ((IT) == SPI_IT_TFHF))

/**
  * @}
  */






/** @defgroup SPI_flags_definition 
  * @{
  */


#define SPI_FLAG_TFE                    SPI_SR_TFE                 
#define SPI_FLAG_TFNF                   SPI_SR_TFNF    
#define SPI_FLAG_RFNE                   SPI_SR_RFNE 
#define SPI_FLAG_RFF                    SPI_SR_RFF     
#define SPI_FLAG_BUSY                   SPI_SR_BUSY   

#define SPI_FLAG_RFOVF                  SPI_RIS_RFOVF                 
#define SPI_FLAG_RFNETO                 SPI_RIS_RFNETO    
#define SPI_FLAG_RFHF                   SPI_RIS_RFHF 
#define SPI_FLAG_TFHF                   SPI_RIS_TFHF 


#define IS_SPI_CLEAR_FLAG(FLAG) (((FLAG) == SPI_FLAG_RFOVF) || ((FLAG) == SPI_FLAG_RFNETO))
#define IS_SPI_GET_FLAG(FLAG) (((FLAG) == SPI_FLAG_TFE) || ((FLAG) == SPI_FLAG_TFNF) || \
                                   ((FLAG) == SPI_FLAG_RFNE) || ((FLAG) == SPI_FLAG_RFF) || \
                                   ((FLAG) == SPI_FLAG_BUSY))
																	 
#define SPI_DMA_Rx      							  0x01
#define SPI_DMA_Tx      							  0x02
																	 
/**
  * @}
  */


/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct);
void SPI_Init(CMSDK_SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct);
void SPI_Cmd(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_LOOPBACK(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_TIModeCmd(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_MicroWireModeCmd(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_DataSizeConfig(CMSDK_SPI_TypeDef* SPIx, uint16_t SPI_DataSize);
void SPI_NSSInternalSoftwareConfig(CMSDK_SPI_TypeDef* SPIx, uint16_t SPI_NSSInternalSoft);
void SPI_SendData8(CMSDK_SPI_TypeDef* SPIx, uint8_t Data);
uint8_t SPI_ReceiveData8(CMSDK_SPI_TypeDef* SPIx);
void SPI_ITConfig(CMSDK_SPI_TypeDef* SPIx, uint8_t SPI_IT, FunctionalState NewState);
FlagStatus SPI_GetFlagStatus(CMSDK_SPI_TypeDef* SPIx, uint16_t SPI_FLAG);
void SPI_ClearFlag(CMSDK_SPI_TypeDef* SPIx, uint16_t SPI_FLAG);
ITStatus SPI_GetITStatus(CMSDK_SPI_TypeDef* SPIx, uint32_t SPI_IT);
uint16_t SPI_ReceiveData(CMSDK_SPI_TypeDef* SPIx);
void SPI_SendData(CMSDK_SPI_TypeDef* SPIx, uint16_t Data);
void SPI_Rx_DMA(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_Tx_DMA(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif /*__PT32Z192XX_SPI_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C)  *****END OF FILE****/
