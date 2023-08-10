/**
  ******************************************************************************
  * @file    pt32z192xx_dma.h
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
#ifndef __PT32Z192XX_DMA_H
#define __PT32Z192XX_DMA_H

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
 * @brief Definition of DMA channel Init Structure
 */
 
typedef struct
{
	
  uint32_t DMACH_SrcAddr;         /*!< Source Address                                                                 */

  uint32_t DMACH_DstAddr;         /*!< Destination Address                                                            */
 
	uint32_t DMACH_BlkCnt;          /*!< Number of Blocks for a DMA transfer (1 ~ 65,535)                               */                            
 
	uint32_t DMACH_SrcDataSize;     /*!< Number of Bits for a data (8-bit/16-bit/32-bit) Source Address To DAM Control  */                      
 
	uint32_t DMACH_DstDataSize;     /*!< Number of Bits for a data (8-bit/16-bit/32-bit) DAM Control To Destination     */      
 
	uint32_t DMACH_SrcAdrInc;       /*!< Source Address Increase Mode Enable                                            */
 
	uint32_t DMACH_DstAdrInc;       /*!< Destination Address Increase Mode Enable                                       */

} DMACH_InitTypeDef;

/**
  * @}
  */


/**
 * @brief DMA Channel Number
 */

#define PDMA_CH0                  (0)                         /*!< channel 0 number                       */
#define PDMA_CH1                  (1)                         /*!< channel 1 number                       */
#define PDMA_CH2                  (2)                         /*!< channel 2 number                       */
#define PDMA_CH3                  (3)                         /*!< channel 3 number                       */

#define IS_DMA_CH(CH)             (CH < 4)                    /*!< check channel number parameter         */

/**
  * @}
  */


/**
 * @brief DMA Transfer width 
 */

#define WIDTH_8BIT                (0)                           /*!< 8-bit transfer width                   */
#define WIDTH_16BIT               (1)                           /*!< 16-bit transfer width                  */
#define WIDTH_32BIT               (2)                           /*!< 32-bit transfer width                  */

#define IS_DMA_WIDTH(WIDTH)      (WIDTH < 3)                    /*!< check transfer width parameter         */

/**
  * @}
  */

/**
 * @brief DMA Transfer size 
 */

#define IS_DMA_BLK_CNT(CNT)      ((CNT > 0) && (CNT < 65536))  /*!< block count per transfer               */
/**
  * @}
  */

/**
 * @brief DMA Transfer Dirction 
 */

#define Peripheral2Memory             ((uint32_t)0x00000000)
#define Memory2Peripheral             DMACH_CTL_DIR

#define IS_DMA_DIRECTION(DIRECTION)   (((DIRECTION) == Peripheral2Memory) || \
                                       ((DIRECTION)  == Memory2Peripheral))
/**
  * @}
  */

/**
 * @brief DMA Interrupts Definition
 */
 
#define DMA_IT_CH0_END               DMA_IER_CH0
#define DMA_IT_CH1_END               DMA_IER_CH1
#define DMA_IT_CH2_END               DMA_IER_CH2
#define DMA_IT_CH3_END               DMA_IER_CH3
#define DMA_IT_CONFIG_ERR            DMA_IER_MME

#define IS_DMA_CONFIG_IT(IT)       (((IT) == DMA_IT_CH0_END)  || \
																		((IT) == DMA_IT_CH1_END)  || \
																		((IT) == DMA_IT_CH2_END)  || \
																		((IT) == DMA_IT_CH3_END)  || \
																		((IT) == DMA_IT_CONFIG_ERR))	
																		
#define IS_DMA_GET_IT(IT)          (((IT) == DMA_IT_CH0_END)  || \
																		((IT) == DMA_IT_CH1_END)  || \
																		((IT) == DMA_IT_CH2_END)  || \
																		((IT) == DMA_IT_CH3_END)  || \
																		((IT) == DMA_IT_CONFIG_ERR))																			
/**
  * @}
  */

/**
 * @brief DMA Flag Definition
 */
 
#define DMA_FLAG_CH0_END               DMA_STATUS_CH0_FINISH
#define DMA_FLAG_CH1_END               DMA_STATUS_CH1_FINISH
#define DMA_FLAG_CH2_END               DMA_STATUS_CH2_FINISH
#define DMA_FLAG_CH3_END               DMA_STATUS_CH3_FINISH
#define DMA_FLAG_CH0_BUSY              DMA_STATUS_CH0_BUSY
#define DMA_FLAG_CH1_BUSY              DMA_STATUS_CH1_BUSY
#define DMA_FLAG_CH2_BUSY              DMA_STATUS_CH2_BUSY
#define DMA_FLAG_CH3_BUSY              DMA_STATUS_CH3_BUSY
#define DMA_FLAG_CONFIG_ERR            DMA_STATUS_MME

#define IS_DMA_FLAG(FLAG)          (((FLAG) == DMA_FLAG_CH0_END)   || \
																		((FLAG) == DMA_FLAG_CH1_END)   || \
																		((FLAG) == DMA_FLAG_CH2_END)   || \
																		((FLAG) == DMA_FLAG_CH3_END)   || \
																		((FLAG) == DMA_FLAG_CH0_BUSY)  || \
																		((FLAG) == DMA_FLAG_CH1_BUSY)  || \
																		((FLAG) == DMA_FLAG_CH2_BUSY)  || \
																		((FLAG) == DMA_FLAG_CH3_BUSY)  || \
																		((FLAG) == DMA_FLAG_CONFIG_ERR))	
																																					
/**
  * @}
  */

/**
 * @brief DMA Peripheral Definition
 */
 
#define DMA_UART0_TX                DMA_CHCFG_UART0_TX
#define DMA_UART0_RX                DMA_CHCFG_UART0_RX
#define DMA_UART1_TX                DMA_CHCFG_UART1_TX
#define DMA_UART1_RX                DMA_CHCFG_UART1_RX
#define DMA_UART2_TX                DMA_CHCFG_UART2_TX
#define DMA_UART2_RX                DMA_CHCFG_UART2_RX
#define DMA_SPI0_TX                 DMA_CHCFG_SPI0_TX
#define DMA_SPI0_RX                 DMA_CHCFG_SPI0_RX
#define DMA_SPI1_TX                 DMA_CHCFG_SPI1_TX
#define DMA_SPI1_RX                 DMA_CHCFG_SPI1_RX
#define DMA_QSPI_TX                 DMA_CHCFG_QSPI_TX
#define DMA_QSPI_RX                 DMA_CHCFG_QSPI_RX
#define DMA_TIMER3                  DMA_CHCFG_TIMER3
#define DMA_TIMER4                  DMA_CHCFG_TIMER4
#define DMA_ADC                     DMA_CHCFG_ADC


#define IS_DMA_PERIPHERAL(PERIPHERAL)  (((PERIPHERAL) == DMA_UART0_TX)   || \
																     		((PERIPHERAL) == DMA_UART0_RX)   || \
																		    ((PERIPHERAL) == DMA_UART1_TX)   || \
																	    	((PERIPHERAL) == DMA_UART1_RX)   || \
																	     	((PERIPHERAL) == DMA_UART2_TX)   || \
																		    ((PERIPHERAL) == DMA_UART2_RX)   || \
																		    ((PERIPHERAL) == DMA_SPI0_TX)    || \
																	    	((PERIPHERAL) == DMA_SPI0_RX)    || \
																	    	((PERIPHERAL) == DMA_SPI1_TX)    || \
																	    	((PERIPHERAL) == DMA_SPI1_RX)    || \
																	    	((PERIPHERAL) == DMA_QSPI_RX)    || \
																	    	((PERIPHERAL) == DMA_QSPI_RX)    || \
																	    	((PERIPHERAL) == DMA_TIMER3)     || \
																	    	((PERIPHERAL) == DMA_TIMER4)     || \
																				((PERIPHERAL) == DMA_ADC))	
																																					
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup 
  * @{
  */
void DMA_Config(uint32_t DMA_Ch, DMACH_InitTypeDef *DMACH_InitStruct);
void DMA_Cmd(uint32_t DMA_Ch,FunctionalState NewState);
void DMA_M2MTransmit_Cmd(uint32_t DMA_Ch,FunctionalState NewState);
void DMA_DirectionConfig(uint32_t DMA_Ch,uint32_t Dirction);
void DMA_AutoRepeat_Cmd(uint32_t DMA_Ch,FunctionalState NewState);
void DMA_SrcAddrssInc_Cmd(uint32_t DMA_Ch,FunctionalState NewState);
void DMA_DstAddrssInc_Cmd(uint32_t DMA_Ch,FunctionalState NewState);
void DMA_SSIZEConfig(uint32_t DMA_Ch,uint32_t SSIZE);
void DMA_DSIZEConfig(uint32_t DMA_Ch,uint32_t DSIZE);
void DMA_SrcAddressConfig(uint32_t DMA_Ch,uint32_t SrcAddr);
void DMA_DstAddressConfig(uint32_t DMA_Ch,uint32_t DstAddr);
uint32_t DMA_GetSrcAddress(uint32_t DMA_Ch);
uint32_t DMA_GetDstAddress(uint32_t DMA_Ch);
uint16_t DMA_GetRemainderCnt(uint32_t DMA_Ch);
void DMA_ITConfig(uint32_t DMA_IT, FunctionalState NewState);
void DMA_ClearITFlag(uint32_t DMA_FLAG);
FlagStatus DMA_GetFlagStatus(uint32_t DMA_FLAG);
void DMA_PeripheralConfig(uint32_t DMA_Ch,uint32_t Peripheral);

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

#endif /* __STM32F0XX_DMA_H */

/**
  * @}
  */

/**
  * @}
  */


