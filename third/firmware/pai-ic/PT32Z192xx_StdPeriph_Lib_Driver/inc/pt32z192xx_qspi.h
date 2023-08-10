/**
  ******************************************************************************
  * @file    pt32z192xx_QSPI.h
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
#ifndef __PT32Z192XX_QSPI_H
#define __PT32Z192XX_QSPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx.h"

/** @addtogroup PT32Z192xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup QSPI
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  QSPI Init structure definition  
  */

typedef struct
{
  uint16_t QSPI_Direction;           /*!< Specifies the QSPI unidirectional or bidirectional data mode.
                                         This parameter can be a value of @ref QSPI_data_direction */

  uint16_t QSPI_Mode;                /*!< Specifies the QQSPI mode (QSPI/Microwire).
                                         This parameter can be a value of @ref QQSPI_mode */
  
  uint16_t QSPI_DataSize;            /*!< Specifies the QSPI data size.
                                         This parameter can be a value of @ref QSPI_data_size */

  uint16_t QSPI_CPOL;                /*!< Specifies the serial clock steady state.
                                         This parameter can be a value of @ref QSPI_Clock_Polarity */

  uint16_t QSPI_CPHA;                /*!< Specifies the clock active edge for the bit capture.
                                         This parameter can be a value of @ref QSPI_Clock_Phase */

 
  uint16_t QSPI_BaudRatePrescaler;   /*!< Specifies the Baud Rate prescaler value which will be
                                         used to configure the transmit and receive SCK clock.
                                         This parameter can be a value of @ref QSPI_BaudRate_Prescaler
                                         @note The communication clock is derived from the master
                                               clock. The slave clock does not need to be set. */																				 
																							 
}QSPI_InitTypeDef;



/* Exported constants --------------------------------------------------------*/
                       

/** @defgroup QSPI_mode 
  * @{
  */

#define QSPI_Mode_SPI                 ((uint16_t)0x0000)
#define QSPI_Mode_Microwire           ((uint16_t)0x0080)
#define IS_QSPI_MODE(MODE) (((MODE) == QSPI_Mode_SPI) || \
                           ((MODE)  == QSPI_Mode_Microwire))
/**
  * @}
  */

/** @defgroup QSPI_data_size
  * @{
  */

#define QSPI_DataSize_4b                  QSPI_CTRLR0_DFS_4B
#define QSPI_DataSize_5b                  QSPI_CTRLR0_DFS_5B
#define QSPI_DataSize_6b                  QSPI_CTRLR0_DFS_6B
#define QSPI_DataSize_7b                  QSPI_CTRLR0_DFS_7B
#define QSPI_DataSize_8b                  QSPI_CTRLR0_DFS_8B
#define QSPI_DataSize_9b                  QSPI_CTRLR0_DFS_9B
#define QSPI_DataSize_10b                 QSPI_CTRLR0_DFS_10B
#define QSPI_DataSize_11b                 QSPI_CTRLR0_DFS_11B
#define QSPI_DataSize_12b                 QSPI_CTRLR0_DFS_12B
#define QSPI_DataSize_13b                 QSPI_CTRLR0_DFS_13B
#define QSPI_DataSize_14b                 QSPI_CTRLR0_DFS_14B
#define QSPI_DataSize_15b                 QSPI_CTRLR0_DFS_15B
#define QSPI_DataSize_16b                 QSPI_CTRLR0_DFS_16B
#define QSPI_DataSize_17b                 QSPI_CTRLR0_DFS_17B
#define QSPI_DataSize_18b                 QSPI_CTRLR0_DFS_18B
#define QSPI_DataSize_19b                 QSPI_CTRLR0_DFS_19B
#define QSPI_DataSize_20b                 QSPI_CTRLR0_DFS_20B
#define QSPI_DataSize_21b                 QSPI_CTRLR0_DFS_21B
#define QSPI_DataSize_22b                 QSPI_CTRLR0_DFS_22B
#define QSPI_DataSize_23b                 QSPI_CTRLR0_DFS_23B
#define QSPI_DataSize_24b                 QSPI_CTRLR0_DFS_24B
#define QSPI_DataSize_25b                 QSPI_CTRLR0_DFS_25B
#define QSPI_DataSize_26b                 QSPI_CTRLR0_DFS_26B
#define QSPI_DataSize_27b                 QSPI_CTRLR0_DFS_27B
#define QSPI_DataSize_28b                 QSPI_CTRLR0_DFS_28B
#define QSPI_DataSize_29b                 QSPI_CTRLR0_DFS_29B
#define QSPI_DataSize_30b                 QSPI_CTRLR0_DFS_30B
#define QSPI_DataSize_31b                 QSPI_CTRLR0_DFS_31B
#define QSPI_DataSize_32b                 QSPI_CTRLR0_DFS_32B

#define IS_QSPI_DATA_SIZE(SIZE) (((SIZE) == QSPI_DataSize_4b)  || \
                                 ((SIZE) == QSPI_DataSize_5b)  || \
                                 ((SIZE) == QSPI_DataSize_6b)  || \
                                 ((SIZE) == QSPI_DataSize_7b)  || \
                                 ((SIZE) == QSPI_DataSize_8b)  || \
                                 ((SIZE) == QSPI_DataSize_9b)  || \
                                 ((SIZE) == QSPI_DataSize_10b) || \
                                 ((SIZE) == QSPI_DataSize_11b) || \
                                 ((SIZE) == QSPI_DataSize_12b) || \
                                 ((SIZE) == QSPI_DataSize_13b) || \
                                 ((SIZE) == QSPI_DataSize_14b) || \
                                 ((SIZE) == QSPI_DataSize_15b) || \
                                 ((SIZE) == QSPI_DataSize_16b) || \
                                 ((SIZE) == QSPI_DataSize_17b) || \
                                 ((SIZE) == QSPI_DataSize_18b) || \
                                 ((SIZE) == QSPI_DataSize_19b) || \
                                 ((SIZE) == QSPI_DataSize_20b) || \
                                 ((SIZE) == QSPI_DataSize_21b) || \
                                 ((SIZE) == QSPI_DataSize_22b) || \
                                 ((SIZE) == QSPI_DataSize_23b) || \
                                 ((SIZE) == QSPI_DataSize_24b) || \
                                 ((SIZE) == QSPI_DataSize_25b) || \
                                 ((SIZE) == QSPI_DataSize_26b) || \
                                 ((SIZE) == QSPI_DataSize_27b) || \
										             ((SIZE) == QSPI_DataSize_28b) || \
                                 ((SIZE) == QSPI_DataSize_29b) || \
                                 ((SIZE) == QSPI_DataSize_30b) || \
                                 ((SIZE) == QSPI_DataSize_31b) || \
                                 ((SIZE) == QSPI_DataSize_32b))													
/**
  * @}
  */


/** @defgroup QSPI_Microwire_ControlDataSize
  * @{
  */

#define Microwire_ControlDataSize_1b                  QSPI_CTRLR0_CFS_1B
#define Microwire_ControlDataSize_2b                  QSPI_CTRLR0_CFS_2B
#define Microwire_ControlDataSize_3b                  QSPI_CTRLR0_CFS_3B
#define Microwire_ControlDataSize_4b                  QSPI_CTRLR0_CFS_4B
#define Microwire_ControlDataSize_5b                  QSPI_CTRLR0_CFS_5B
#define Microwire_ControlDataSize_6b                  QSPI_CTRLR0_CFS_6B
#define Microwire_ControlDataSize_7b                  QSPI_CTRLR0_CFS_7B
#define Microwire_ControlDataSize_8b                  QSPI_CTRLR0_CFS_8B
#define Microwire_ControlDataSize_9b                  QSPI_CTRLR0_CFS_9B
#define Microwire_ControlDataSize_10b                 QSPI_CTRLR0_CFS_10B
#define Microwire_ControlDataSize_11b                 QSPI_CTRLR0_CFS_11B
#define Microwire_ControlDataSize_12b                 QSPI_CTRLR0_CFS_12B
#define Microwire_ControlDataSize_13b                 QSPI_CTRLR0_CFS_13B
#define Microwire_ControlDataSize_14b                 QSPI_CTRLR0_CFS_14B
#define Microwire_ControlDataSize_15b                 QSPI_CTRLR0_CFS_15B
#define Microwire_ControlDataSize_16b                 QSPI_CTRLR0_CFS_16B


#define IS_QSPI_Microwire_ControlDataSize(SIZE) (((SIZE) == Microwire_ControlDataSize_1b)  || \
                                                 ((SIZE) == Microwire_ControlDataSize_2b)  || \
                                                 ((SIZE) == Microwire_ControlDataSize_3b)  || \
                                                 ((SIZE) == Microwire_ControlDataSize_4b)  || \
                                                 ((SIZE) == Microwire_ControlDataSize_5b)  || \
																							   ((SIZE) == Microwire_ControlDataSize_6b)  || \
																							   ((SIZE) == Microwire_ControlDataSize_7b)  || \
																								 ((SIZE) == Microwire_ControlDataSize_8b)  || \
																								 ((SIZE) == Microwire_ControlDataSize_9b)  || \
																								 ((SIZE) == Microwire_ControlDataSize_10b) || \
																								 ((SIZE) == Microwire_ControlDataSize_11b) || \
																								 ((SIZE) == Microwire_ControlDataSize_12b) || \
																								 ((SIZE) == Microwire_ControlDataSize_13b) || \
																								 ((SIZE) == Microwire_ControlDataSize_14b) || \
																								 ((SIZE) == Microwire_ControlDataSize_15b) || \
																								 ((SIZE) == Microwire_ControlDataSize_16b))
																									 
/**
  * @}
  */



/** @defgroup QSPI_Clock_Polarity 
  * @{
  */

#define QSPI_CPOL_Low                    ((uint16_t)0x0000)
#define QSPI_CPOL_High                    QSPI_CTRLR0_CPOL
#define IS_QSPI_CPOL(CPOL) (((CPOL) == QSPI_CPOL_Low) || \
                           ((CPOL) == QSPI_CPOL_High))
/**
  * @}
  */

/** @defgroup QSPI_Clock_Phase 
  * @{
  */

#define QSPI_CPHA_1Edge                  ((uint16_t)0x0000)
#define QSPI_CPHA_2Edge                   QSPI_CTRLR0_CPHA
#define IS_QSPI_CPHA(CPHA) (((CPHA) == QSPI_CPHA_1Edge) || \
                           ((CPHA) == QSPI_CPHA_2Edge))
/**
  * @}
  */


/** @defgroup QSPI_Direction 
  * @{
  */

#define QSPI_Direction_TXRX                QSPI_CTRLR0_TMOD_TXRX
#define QSPI_Direction_TX                  QSPI_CTRLR0_TMOD_TX
#define QSPI_Direction_RX                  QSPI_CTRLR0_TMOD_RX
#define QSPI_Direction_EPPROMREAD          QSPI_CTRLR0_TMOD_EPPREAD

#define IS_QSPI_DIRECTION(DIRECTION) (((DIRECTION) == QSPI_Direction_TXRX) || \
                                      ((DIRECTION) == QSPI_Direction_TX)   || \
																			((DIRECTION) == QSPI_Direction_RX)   || \
																			((DIRECTION) == QSPI_Direction_EPPROMREAD))
/**
  * @}
  */

/** @defgroup QSPI SPI Frame Mode 
  * @{
  */

#define QSPI_SPI_SINGLE                  QSPI_CTRLR0_SPI_FRF_SINGLE
#define QSPI_SPI_DUAL                    QSPI_CTRLR0_SPI_FRF_DUAL
#define QSPI_SPI_QUAD                    QSPI_CTRLR0_SPI_FRF_QUAD


#define IS_QSPI_FRAMEMODE(Mode)      (((Mode) == QSPI_SPI_SINGLE) || \
                                      ((Mode) == QSPI_SPI_DUAL)   || \
																			((Mode) == QSPI_SPI_QUAD))
/**
  * @}
  */


/** @defgroup QSPI Microwire Transfer Mode 
  * @{
  */

#define QSPI_Microwire_Sequence          0x0000
#define QSPI_Microwire_Non_Sequence      0x0001



#define IS_QSPI_MICROWIREMODE(MODE)  (((MODE) == QSPI_Microwire_Sequence) || \
																			((MODE) == QSPI_Microwire_Non_Sequence))
/**
  * @}
  */
	
/** @defgroup QSPI Microwire Transfer Direction 
  * @{
  */
 
#define QSPI_Microwire_TX          0x0002
#define QSPI_Microwire_RX          0x0000



#define IS_QSPI_MICROWIREDIRECTION(DIRECTION)  (((DIRECTION) == QSPI_Microwire_TX) || \
																			          ((DIRECTION) == QSPI_Microwire_RX))
/**
  * @}
  */
		
	

/** @defgroup QSPI_BaudRate_Prescaler 
  * @{
  */

#define IS_QSPI_BAUDRATE_PRESCALER(PRESCALER) (PRESCALER<= 0xFFFF)	
																							
/**
  * @}
  */
	
/** @defgroup QSPI_TXFIFO_Threshold
  * @{
  */

#define IS_QSPI_TXFIFO_THRESHOLD(THRESHOLD) (THRESHOLD<= 0xFF)	
																							
/**
  * @}
  */	
	
/** @defgroup QSPI_DMATXFIFO_Threshold
  * @{
  */

#define IS_QSPI_DMATXFIFO_THRESHOLD(THRESHOLD) (THRESHOLD<= 0xF)	
																							
/**
  * @}
  */	
	
/** @defgroup QSPI_TXFIFO_StartLevel
  * @{
  */

#define IS_QSPI_TXFIFO_STARTLEVEL(LEVEL) (LEVEL<= 0xFF)	
																							
/**
  * @}
  */	

/** @defgroup QSPI_RXFIFO_Threshold
  * @{
  */

#define IS_QSPI_RXFIFO_THRESHOLD(THRESHOLD) (THRESHOLD<= 0xFF)	
																							
/**
  * @}
  */
	
/** @defgroup QSPI_DMARXFIFO_Threshold
  * @{
  */

#define IS_QSPI_DMARXFIFO_THRESHOLD(THRESHOLD) (THRESHOLD<= 0xF)	
																							
/**
  * @}
  */	


	
/** @defgroup QSPI_ReceiveNum 
  * @{
  */

#define IS_QSPI_ReceiveNum(Num) (Num<= 0xFFFF)	
																							
/**
  * @}
  */	
	
/** @defgroup QSPI Status Flag 
  * @{
  */
 

#define IS_QSPI_STATUS_FLAG(FLAG)              (((FLAG) == QSPI_SR_BUSY) || \
																			          ((FLAG) == QSPI_SR_TFNF) || \
																								((FLAG) == QSPI_SR_TFE)  || \
																								((FLAG) == QSPI_SR_RNE)  || \
																								((FLAG) == QSPI_SR_RFF))																							
/**
  * @}
  */



/** @defgroup QSPI_Interrupts_definition 
  * @{
  */

#define QSPI_IT_TXEIM               QSPI_IMR_TXEIM
#define QSPI_IT_TXOIM               QSPI_IMR_TXOIM
#define QSPI_IT_RXUIM               QSPI_IMR_RXUIM
#define QSPI_IT_RXOIM               QSPI_IMR_RXOIM
#define QSPI_IT_RXFIM               QSPI_IMR_RXFIM

#define IS_QSPI_CONFIG_IT(IT)      (((IT) == QSPI_IT_TXEIM) || \
																		((IT) == QSPI_IT_TXOIM) || \
																		((IT) == QSPI_IT_RXUIM)  || \
																		((IT) == QSPI_IT_RXOIM)  || \
																		((IT) == QSPI_IT_RXFIM))		
																		
#define IS_QSPI_GET_IT(IT)         (((IT) == QSPI_IT_TXEIM) || \
																		((IT) == QSPI_IT_TXOIM) || \
																		((IT) == QSPI_IT_RXUIM)  || \
																		((IT) == QSPI_IT_RXOIM)  || \
																		((IT) == QSPI_IT_RXFIM))																			
/**
  * @}
  */


/** @defgroup QSPI_Raw_Interrupts_Status definition 
  * @{
  */

#define QSPI_RAWIT_STATUS_TXEIM               QSPI_RISR_TXEIR
#define QSPI_RAWIT_STATUS_TXOIM               QSPI_RISR_TXOIR
#define QSPI_RAWIT_STATUS_RXUIM               QSPI_RISR_RXUIR
#define QSPI_RAWIT_STATUS_RXOIM               QSPI_RISR_RXOIR
#define QSPI_RAWIT_STATUS_RXFIM               QSPI_RISR_RXFIR

#define IS_QSPI_GET_RAWIT_STATUS(IT)	      (((IT) == QSPI_RAWIT_STATUS_TXEIM)  || \
																	           ((IT) == QSPI_RAWIT_STATUS_TXOIM)  || \
																	           ((IT) == QSPI_RAWIT_STATUS_RXUIM)  || \
																		         ((IT) == QSPI_RAWIT_STATUS_RXOIM)  || \
																	           ((IT) == QSPI_RAWIT_STATUS_RXFIM))	   
/**
  * @}
  */

/** @defgroup QSPI_DMA_Direction definition 
  * @{
  */

#define QSPI_DMA_TX               QSPI_DMACR_TDMAE
#define QSPI_DMA_RX               QSPI_DMACR_RDMAE


#define IS_QSPI_DMA_DIRECTION(DIRECTION)	  (((DIRECTION) == QSPI_DMA_TX)  || \
																	           ((DIRECTION) == QSPI_DMA_RX))  
/**
  * @}
  */

/** @defgroup QSPI_Flash_TRANS_TYPE  
  * @{
  */

#define QSPI_FLASH_TRANS_TYPE_TT0               QSPI_SPI_CTRLR_TRANS_TYPE_TT0
#define QSPI_FLASH_TRANS_TYPE_TT1               QSPI_SPI_CTRLR_TRANS_TYPE_TT1
#define QSPI_FLASH_TRANS_TYPE_TT2               QSPI_SPI_CTRLR_TRANS_TYPE_TT2

#define IS_QSPI_FLASH_TRANS_TYPE(TYPE)	    (((TYPE) == QSPI_FLASH_TRANS_TYPE_TT0)  || \
																	           ((TYPE) == QSPI_FLASH_TRANS_TYPE_TT1)  || \
																						 ((TYPE) == QSPI_FLASH_TRANS_TYPE_TT1))
/**
  * @}
  */

/** @defgroup QSPI_Flash_ADDR_LENGTH  
  * @{
  */

#define QSPI_FLASH_ADDR_L0                QSPI_SPI_CTRLR_ADDR_L0
#define QSPI_FLASH_ADDR_L4                QSPI_SPI_CTRLR_ADDR_L4
#define QSPI_FLASH_ADDR_L8                QSPI_SPI_CTRLR_ADDR_L8
#define QSPI_FLASH_ADDR_L12               QSPI_SPI_CTRLR_ADDR_L12
#define QSPI_FLASH_ADDR_L16               QSPI_SPI_CTRLR_ADDR_L16
#define QSPI_FLASH_ADDR_L20               QSPI_SPI_CTRLR_ADDR_L20
#define QSPI_FLASH_ADDR_L24               QSPI_SPI_CTRLR_ADDR_L24
#define QSPI_FLASH_ADDR_L28               QSPI_SPI_CTRLR_ADDR_L28
#define QSPI_FLASH_ADDR_L32               QSPI_SPI_CTRLR_ADDR_L32
#define QSPI_FLASH_ADDR_L36               QSPI_SPI_CTRLR_ADDR_L36
#define QSPI_FLASH_ADDR_L40               QSPI_SPI_CTRLR_ADDR_L40
#define QSPI_FLASH_ADDR_L44               QSPI_SPI_CTRLR_ADDR_L44
#define QSPI_FLASH_ADDR_L48               QSPI_SPI_CTRLR_ADDR_L48
#define QSPI_FLASH_ADDR_L52               QSPI_SPI_CTRLR_ADDR_L52
#define QSPI_FLASH_ADDR_L56               QSPI_SPI_CTRLR_ADDR_L56
#define QSPI_FLASH_ADDR_L60               QSPI_SPI_CTRLR_ADDR_L60

#define IS_QSPI_Flash_ADDR_LENGTH(LENGTH)       (((LENGTH) == QSPI_FLASH_ADDR_L0)   || \
                                                 ((LENGTH) == QSPI_FLASH_ADDR_L4)   || \
                                                 ((LENGTH) == QSPI_FLASH_ADDR_L8)   || \
                                                 ((LENGTH) == QSPI_FLASH_ADDR_L12)  || \
                                                 ((LENGTH) == QSPI_FLASH_ADDR_L16)  || \
																							   ((LENGTH) == QSPI_FLASH_ADDR_L20)  || \
																							   ((LENGTH) == QSPI_FLASH_ADDR_L24)  || \
																								 ((LENGTH) == QSPI_FLASH_ADDR_L28)  || \
																								 ((LENGTH) == QSPI_FLASH_ADDR_L32)  || \
																								 ((LENGTH) == QSPI_FLASH_ADDR_L36)  || \
																								 ((LENGTH) == QSPI_FLASH_ADDR_L40)  || \
																								 ((LENGTH) == QSPI_FLASH_ADDR_L44)  || \
																								 ((LENGTH) == QSPI_FLASH_ADDR_L48)  || \
																								 ((LENGTH) == QSPI_FLASH_ADDR_L52)  || \
																								 ((LENGTH) == QSPI_FLASH_ADDR_L56)  || \
																								 ((LENGTH) == QSPI_FLASH_ADDR_L60))

/**
  * @}
  */

/** @defgroup QSPI_Flash_Instruction_LENGTH  
  * @{
  */

#define QSPI_FLASH_INST_L0                QSPI_SPI_CTRLR_INST_L0
#define QSPI_FLASH_INST_L4                QSPI_SPI_CTRLR_INST_L4
#define QSPI_FLASH_INST_L8                QSPI_SPI_CTRLR_INST_L8
#define QSPI_FLASH_INST_L16               QSPI_SPI_CTRLR_INST_L16


#define IS_QSPI_Flash_INST_LENGTH(LENGTH)       (((LENGTH) == QSPI_FLASH_INST_L0)   || \
                                                 ((LENGTH) == QSPI_FLASH_INST_L4)   || \
                                                 ((LENGTH) == QSPI_FLASH_INST_L8)   || \
                                                 ((LENGTH) == QSPI_FLASH_INST_L16))

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void QSPI_StructInit(QSPI_InitTypeDef* QSPI_InitStruct);
void QSPI_Init(QSPI_InitTypeDef* QSPI_InitStruct);
void QSPI_Cmd(FunctionalState NewState);
void QSPI_ModeSel(uint16_t Mode_Select);
void QSPI_DataSizeConfig(uint16_t QSPI_DataSize);
void QSPI_DirectionConfig(uint16_t Transmit_Direction);
void QSPI_LoopBackConfig(FunctionalState NewState);
void QSPI_CS_OverTurnConfig(FunctionalState NewState);
void QSPI_Microwire_ControlDataSizeConfig(uint32_t Microwire_ControlDataSize);
void QSPI_SPIFrameConfig(uint32_t Frame_Mode);
void QSPI_ReceiveNumConfig(uint32_t Num);
void QSPI_MicrowireModeConfig(uint16_t Microwire_Mode);
void QSPI_MicrowireDirectionConfig(uint16_t Microwire_Direction);
void QSPI_MicrowireHandShaking(FunctionalState NewState);
void QSPI_CSCmd(FunctionalState NewState);
void QSPI_BaudRateConfig(uint16_t BaudRate);
void QSPI_TXFIFO_ThresholdConfig(uint8_t Threshold);
void QSPI_TXFIFO_StartLevelConfig(uint8_t Level);
void QSPI_RXFIFO_ThresholdConfig(uint8_t Threshold);
uint16_t QSPI_GetTXFIFONum(void);
uint16_t QSPI_GetRXFIFONum(void);
FlagStatus QSPI_GetFlagStatus(uint32_t QSPI_STATUS);
void QSPI_DMACmd(uint32_t DMA_Direction,FunctionalState NewState);
void QSPI_DMATXFIFO_ThresholdConfig(uint8_t Threshold);
void QSPI_DMARXFIFO_ThresholdConfig(uint8_t Threshold);
void QSPI_SendData(uint32_t Data);
uint32_t QSPI_ReceiveData(void);
void QSPI_ITConfig(uint32_t QSPI_IT, FunctionalState NewState);
void QSPI_ClearITFlag(uint32_t QSPI_FLAG);
void QSPI_ClearALLITFlag(void);
ITStatus QSPI_GetITStatus(uint32_t QSPI_IT);
FlagStatus QSPI_GetRawITStatus(uint32_t QSPI_RawIT);
void QSPI_FLASH_TransTypeConfig(uint32_t TransType);
void QSPI_FLASH_AddrLengthConfig(uint32_t Addr_Length);
void QSPI_FLASH_INSTLengthConfig(uint32_t Instruction_Length);






#ifdef __cplusplus
}
#endif

#endif /*__PT32Z192XX_QSPI_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C)  *****END OF FILE****/
