/**
  ******************************************************************************
  * @file    pt32z192xx_uart.h
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
#ifndef __PT32Z192XX_UART_H
#define __PT32Z192XX_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx.h"

/** @addtogroup PT32Z192xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup UART
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/

   
   
/** 
  * @brief  UART Init Structure definition  
  */ 
typedef struct
{
  uint32_t UART_BaudRate;
  uint16_t UART_WordLength;
  uint16_t UART_StopBit;
  uint16_t UART_Parity;
  uint32_t UART_SampleRate;
	uint16_t UART_RXEN;
	uint32_t FCPU;
} UART_InitTypeDef;

/** @defgroup UART_FIFO_TYPE
  * @{
  */ 
typedef enum 
{ 
  Rx_FIFO, 
  Tx_FIFO
} UART_FifoTypeDef;

/**
  * @}
  */ 

/** @defgroup UART_Oversampling_Rate
  * @{
  */ 
#define IS_UART_ALL_PERIPH(PERIPH)    (((PERIPH) == UART0) || \
                                       ((PERIPH) == UART1) || \
                                       ((PERIPH) == UART2))

#define IS_USART_0_PERIPH(PERIPH)      ((PERIPH) == UART0)

#define UART_SAMPLERATEX4              ((uint32_t)0x00000004)
#define UART_SAMPLERATEX8              ((uint32_t)0x00000008)
#define UART_SAMPLERATEX16             ((uint32_t)0x00000010)

#define IS_UART_BAUDRATE(BAUDRATE)     (((BAUDRATE) > 0) && ((BAUDRATE) < 0x005B8D81))
#define IS_UART_SAMPLERATE(SAMPLERATE) (((SAMPLERATE) == UART_SAMPLERATEX4) || ((SAMPLERATE) == UART_SAMPLERATEX8) || \
                                       ((SAMPLERATE) == UART_SAMPLERATEX16))	
#define IS_UART_RXSTATUS(RXSTATUS)     (((RXSTATUS) == ENABLE) || ((RXSTATUS) == DISABLE))	

/**
  * @}
  */ 

/** @defgroup UART_Global_definition 
  * @{
  */
#define IS_UART_TIMEOUT(TIMEOUT)       ((TIMEOUT) <= 0x000000FF)
#define IS_UART_PRESCALER(PRESCALER)   ((PRESCALER) <= 0x00000005)
#define IS_UART_DATA(DATA)             ((DATA) <= 0x1FF)
#define IS_UART_DUTYCYCLE(DUTYCYCLE)   ((DUTYCYCLE) <= 0x00000064)

/**
  * @}
  */ 
	
/** @defgroup UART_Frame_Formate 
  * @{
  */ 
#define UARTM_8D                       ((uint32_t)0x00000001)
#define UARTM_7D_P                     ((uint32_t)0x00000003)
#define UARTM_9D                       ((uint32_t)0x00000004)
#define UARTM_8D_W                     ((uint32_t)0x00000005)
#define UARTM_8D_P                     ((uint32_t)0x00000007)

#define UART05StopBits                 ((uint32_t)0x00000000)
#define UART1StopBit                   ((uint32_t)0x00000008)
#define UART15StopBits                 ((uint32_t)0x00000010)
#define UART2StopBits                  ((uint32_t)0x00000018)



#define IS_UART_WORD_LENGTH(LENGTH)    (((LENGTH) == UARTM_8D) || ((LENGTH) == UARTM_7D_P) || \
                                       ((LENGTH) == UARTM_9D) || ((LENGTH) == UARTM_8D_W)  || \
                                       ((LENGTH) == UARTM_8D_P))
																			
#define IS_UART_STOPBITS(STOPBITS)     (((STOPBITS) == UART05StopBits) || ((STOPBITS) == UART1StopBit) || \
                                       ((STOPBITS) == UART15StopBits) || ((STOPBITS) == UART2StopBits))			
/**
  * @}
  */ 

/** @defgroup UART_HalfDuplex_Mode 
  * @{
  */ 

#define UART_Mode_Rx                   ((uint32_t)0x00000000)
#define UART_Mode_Tx                   ((uint32_t)0x00000001)
#define IS_UART_MODE(MODE)             (((MODE) == UART_Mode_Tx) || \
                                       ((MODE) == UART_Mode_Rx))
/**
  * @}
  */ 

/** @defgroup UART_Polarity_Mode 
  * @{
  */ 

#define UART_InvPin_Tx                  UART_CR_TXPOL
#define UART_InvPin_Rx                  UART_CR_RXPOL
#define IS_UART_INVERSTION_PIN(PIN)     ((((PIN) & (uint32_t)0xFFFCFFFF) == 0x00) && \
                                        ((PIN) != (uint32_t)0x00))																				
																				
/**
  * @}
  */ 

/** @defgroup UART_Polarity_Mode 
  * @{
  */ 
#define UART_ODD_PARITY                ((uint32_t)0x00000020)
#define UART_EVEN_PARITY               ((uint32_t)0x00000000)
#define IS_UART_PARITY(MODE)           (((MODE) == UART_ODD_PARITY) || \
                                       ((MODE) == UART_EVEN_PARITY))
/**
  * @}
  */ 

/** @defgroup USART_Flags 
  * @{
  */
#define  UART_FLAG_RXNE                      UART_ISR_RXNE          /*!< Receiver FIFO buffer non-empty */
#define  UART_FLAG_RXHF                      UART_ISR_RXHF          /*!< Receiver FIFO buffer half-full */
#define  UART_FLAG_RXF                       UART_ISR_RXF           /*!< Receiver FIFO buffer full */
#define  UART_FLAG_TONE                      UART_ISR_TONE          /*!< Receiver FIFO buffer cleared timeout  */
#define  UART_FLAG_TOIDLE                    UART_ISR_TOIDLE        /*!< Receiver FIFO buffer idel */
#define  UART_FLAG_PERR                      UART_ISR_PERR          /*!< Parity error */      
#define  UART_FLAG_FERR                      UART_ISR_FERR          /*!< Frame error */
#define  UART_FLAG_OVERR                     UART_ISR_OVERR         /*!< Receiver FIFO buffer overflow */    
#define  UART_FLAG_TXE                       UART_ISR_TXE           /*!< Transmitter FIFO buffer empty */         
#define  UART_FLAG_TXHE                      UART_ISR_TXHE          /*!< Transmitter FIFO buffer half-empty */       
#define  UART_FLAG_TXF                       UART_ISR_TXF           /*!< Transmitter FIFO buffer full */   
#define  UART_FLAG_TXEND                     UART_ISR_TXEND         /*!< Transfer completed */

#define IS_UART_FLAG(FLAG)  (((FLAG) == UART_FLAG_RXNE)   || ((FLAG) == UART_FLAG_RXHF)  || \
                             ((FLAG) == UART_FLAG_RXF)    || ((FLAG) == UART_FLAG_TONE)  || \
                             ((FLAG) == UART_FLAG_TOIDLE) || ((FLAG) == UART_FLAG_PERR)  || \
                             ((FLAG) == UART_FLAG_FERR)   || ((FLAG) == UART_FLAG_OVERR) || \
                             ((FLAG) == UART_FLAG_TXE)    || ((FLAG) == UART_FLAG_TXHE)  || \
                             ((FLAG) == UART_FLAG_TXF)    || ((FLAG) == UART_FLAG_TXEND)) 
														 
#define  UART_CLEAR_FLAG_PERR                      UART_ISR_PERR  >> 0x01          /*!< Parity error */      
#define  UART_CLEAR_FLAG_FERR                      UART_ISR_FERR  >> 0x01          /*!< Frame error */
#define  UART_CLEAR_FLAG_OVERR                     UART_ISR_OVERR >> 0x01          /*!< Receiver FIFO buffer overflow */ 


#define IS_UART_CLEAR_FLAG(FLAG)  (((FLAG) == UART_CLEAR_FLAG_PERR)   || ((FLAG) == UART_CLEAR_FLAG_FERR)  ||\
                                   ((FLAG) == UART_CLEAR_FLAG_OVERR))
/**
  * @}
  */ 
	
/** @defgroup UART_Interrupt_definition                                                                                                                           
  * @brief UART Interrupt definition                                                                                                                                                               
  * @{                                                                                                          
  */                                                                                                            
                                                                                                                
#define  UART_IT_RXNEIE                     UART_IER_RXNEIE          /*!< Receiver FIFO buffer non-empty */                                          
#define  UART_IT_RXHFIE                     UART_IER_RXHFIE          /*!< Receiver FIFO buffer half-full */                                          
#define  UART_IT_RXFIE                      UART_IER_RXFIE           /*!< Receiver FIFO buffer full */                                               
#define  UART_IT_TONEIE                     UART_IER_TONEIE          /*!< Receiver FIFO buffer cleared timeout  */                                   
#define  UART_IT_TOIDLEIE                   UART_IER_TOIDLEIE        /*!< Receiver FIFO buffer idel */                                               
#define  UART_IT_PERRIE                     UART_IER_PERRIE          /*!< Parity error */                                                            
#define  UART_IT_FERRIE                     UART_IER_FERRIE          /*!< Frame error */                                                             
#define  UART_IT_OVERRIE                    UART_IER_OVERRIE         /*!< Receiver FIFO buffer overflow */                                           
#define  UART_IT_TXEIE                      UART_IER_TXEIE           /*!< Transmitter FIFO buffer empty */                                           
#define  UART_IT_TXHEIE                     UART_IER_TXHEIE          /*!< Transmitter FIFO buffer half-empty */                                      
#define  UART_IT_TXFIE                      UART_IER_TXFIE           /*!< Transmitter FIFO buffer full */                                            
#define  UART_IT_TXENDIE                    UART_IER_TXENDIE         /*!< Transfer completed */                                               


#define IS_UART_CONFIG_IT(SOURCE) (((SOURCE) == UART_IT_RXNEIE) || ((SOURCE) == UART_IT_RXHFIE) || \
                                    ((SOURCE) == UART_IT_RXFIE) || ((SOURCE) == UART_IT_TONEIE) || \
                                    ((SOURCE) == UART_IT_TOIDLEIE) || ((SOURCE) == UART_IT_PERRIE) || \
                                    ((SOURCE) == UART_IT_FERRIE) || ((SOURCE) == UART_IT_OVERRIE) || \
                                    ((SOURCE) == UART_IT_TXEIE) || ((SOURCE) == UART_IT_TXHEIE) || \
                                    ((SOURCE) == UART_IT_TXFIE) || ((SOURCE) == UART_IT_TXENDIE))
																		
#define IS_UART_GET_IT(IT)         (((IT) == UART_IT_RXNEIE) || ((IT) == UART_IT_RXHFIE) || \
                                    ((IT) == UART_IT_RXFIE) || ((IT) == UART_IT_TONEIE) || \
                                    ((IT) == UART_IT_TOIDLEIE) || ((IT) == UART_IT_PERRIE) || \
                                    ((IT) == UART_IT_FERRIE) || ((IT) == UART_IT_OVERRIE) || \
                                    ((IT) == UART_IT_TXEIE) || ((IT) == UART_IT_TXHEIE) || \
                                    ((IT) == UART_IT_TXFIE) || ((IT) == UART_IT_TXENDIE))																		
																
/**
  * @}
  */ 

void UART_Init(CMSDK_UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct);
void UART_StructInit(UART_InitTypeDef* UART_InitStruct);
void UART_Cmd(CMSDK_UART_TypeDef* UARTx, FunctionalState NewState);
void UART_RxConfig(CMSDK_UART_TypeDef* UARTx, FunctionalState NewState);
void USART_InvPinCmd(CMSDK_UART_TypeDef* UARTx, uint32_t UART_InvPin, FunctionalState NewState);
void UART_SetReceiverTimeOut(CMSDK_UART_TypeDef* UARTx, uint32_t UART_ReceiverTimeOut);
void UART_SetPrescaler(CMSDK_UART_TypeDef* UARTx, uint8_t UART_Prescaler);
void UART_SetGuardTime(CMSDK_UART_TypeDef* UARTx, uint8_t UART_GuardTime);
void UART_SendData(CMSDK_UART_TypeDef* UARTx, uint16_t Data);
void UART_StringSend(CMSDK_UART_TypeDef* UARTx, uint8_t *String);
uint16_t UART_ReceiveData(CMSDK_UART_TypeDef* UARTx);
uint16_t UART_ReceiveData_Timeout(CMSDK_UART_TypeDef* UARTx, uint16_t *Data, uint32_t UART_ReceiverTimeOut);
uint16_t UART_StringReceive(CMSDK_UART_TypeDef* UARTx, uint8_t *Data);
void UART_LoopBackConfig(CMSDK_UART_TypeDef* UARTx, FunctionalState NewStatus);
void UART_FifoReset(CMSDK_UART_TypeDef * UARTx,  UART_FifoTypeDef FIFO);
void UART_IrDACmd(CMSDK_UART_TypeDef* UARTx, FunctionalState NewState);
void UART_IrDAInvPinCmd(CMSDK_UART_TypeDef* UARTx, FunctionalState NewState);
void UART_SetDutyCycle(CMSDK_UART_TypeDef* UARTx, uint8_t UART_DutyCycle);
void UART_HalfDuplexCmd(CMSDK_UART_TypeDef* UARTx, FunctionalState NewState);
void UART_HalfDuplex_DirectionModeCmd(CMSDK_UART_TypeDef* UARTx, uint32_t UART_DirectionMode);
void UART_ITConfig(CMSDK_UART_TypeDef* UARTx, uint32_t UART_IT, FunctionalState NewState);
FlagStatus UART_GetFlagStatus(CMSDK_UART_TypeDef* UARTx, uint32_t UART_FLAG);
void UART_ClearFlag(CMSDK_UART_TypeDef* UARTx, uint32_t UART_CLEAR_FLAG);
ITStatus UART_GetITStatus(CMSDK_UART_TypeDef* UARTx,uint32_t UART_IT);
void UART_DMAcmd(CMSDK_UART_TypeDef* UARTx);
#ifdef __cplusplus
}
#endif

#endif /* __PT32Z192XX_UART_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ *****END OF FILE****/
