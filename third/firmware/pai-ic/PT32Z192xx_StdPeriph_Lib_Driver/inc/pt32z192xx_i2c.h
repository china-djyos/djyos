/**
  ******************************************************************************
  * @file    pt32z192xx_i2c.h
  * @author  应用开发团队
  * @version 
  * @date    
  * @brief   This file contains all the functions prototypes for the I2C firmware
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
#ifndef __PT32Z192XX_I2C_H
#define __PT32Z192XX_I2C_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx.h"

/** @addtogroup PT32Z192xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup I2C
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  I2C Init structure definition
  */

typedef struct
{
  FunctionalState I2C_BroadCast;    /*!< Specifies the I2C mode.
                                         This parameter can be a value of @ref I2C_mode*/

  uint8_t I2C_OwnAddress;          /*!< Specifies the device own address 1.
                                         This parameter can be a 7-bit or 10-bit address*/

  FunctionalState I2C_Ack;          /*!< Enables or disables the acknowledgement.
                                         This parameter can be a value of @ref I2C_acknowledgement*/

  uint32_t I2C_SCLK_DIV;            /*!< */
}I2C_InitTypeDef;

/* Exported constants --------------------------------------------------------*/


/** @defgroup I2C_Exported_Constants
  * @{
  */

#define IS_I2C_ALL_PERIPH(PERIPH)       ((PERIPH) == I2C0)
                                         
#define IS_I2C_BROADCAST(BROADCAST)     (((BROADCAST) == ENABLE) || ((BROADCAST) == DISABLE))	

/** @defgroup I2C_acknowledgement
  * @{
  */

#define I2C_Ack_Enable                  I2C_CSR_ACK
#define I2C_Ack_Disable                 I2C_CCR_ACK

/**
  * @}
  */
	
/** @defgroup I2C_Prescaler
  * @{
  */

#define I2C_SCLK_DIVX960                 ((uint32_t)0x00000080)
#define I2C_SCLK_DIVX256                 ((uint32_t)0x00000000)
#define I2C_SCLK_DIVX244                 ((uint32_t)0x00000001)
#define I2C_SCLK_DIVX192                 ((uint32_t)0x00000002)
#define I2C_SCLK_DIVX160                 ((uint32_t)0x00000003)
#define I2C_SCLK_DIVX120                 ((uint32_t)0x00000081)
#define I2C_SCLK_DIVX60                  ((uint32_t)0x00000082)


#define IS_SCLK_DIV(DIV)                 (((DIV) == I2C_SCLK_DIVX60)  || \
                                          ((DIV) == I2C_SCLK_DIVX120) || \
                                          ((DIV) == I2C_SCLK_DIVX160) || \
                                          ((DIV) == I2C_SCLK_DIVX192) || \
                                          ((DIV) == I2C_SCLK_DIVX244) || \
                                          ((DIV) == I2C_SCLK_DIVX256) || \
                                          ((DIV) == I2C_SCLK_DIVX960))
/**
  * @}
  */



/** @defgroup I2C_own_address1
  * @{
  */

#define IS_I2C_OWN_ADDRESS(ADDRESS)   ((ADDRESS) <= (uint32_t)0x0000007F)
/**
  * @}
  */


/** @defgroup I2C_slave_address
  * @{
  */

#define IS_I2C_SLAVE_ADDRESS(ADDRESS)   ((ADDRESS) <= (uint16_t)0x007F)
/**
  * @}
  */






/** @defgroup I2C_timeout
  * @{
  */

#define IS_I2C_TIMEOUT(TIMEOUT)   ((TIMEOUT) <= (uint16_t)0x0FFF)

/**
  * @}
  */

/** @defgroup I2C_registers 
  * @{
  */

#define I2C_Register_CSR                ((uint8_t)0x00)
#define I2C_Register_SR                 ((uint8_t)0x04)
#define I2C_Register_DR                 ((uint8_t)0x08)
#define I2C_Register_OAR                ((uint8_t)0x0C)
#define I2C_Register_CCR                ((uint8_t)0x10)


#define IS_I2C_REGISTER(REGISTER)       (((REGISTER) == I2C_Register_CSR) || \
                                         ((REGISTER) == I2C_Register_SR) || \
                                         ((REGISTER) == I2C_Register_DR) || \
                                         ((REGISTER) == I2C_Register_OAR) || \
                                         ((REGISTER) == I2C_Register_CCR))
/**
  * @}
  */

/** @defgroup I2C_interrupts_definition 
  * @{
  */

#define I2C_IT_IS                      I2C_CSR_IS

#define IS_I2C_CONFIG_IT(IT)            (((IT) == I2C_IT_IS))

/**
  * @}
  */

/** @defgroup I2C_flags_definition 
  * @{
  */

#define  I2C_FLAG_IS                   I2C_CSR_IS


#define IS_I2C_CLEAR_FLAG(FLAG)           (((FLAG) == I2C_FLAG_IS))      

#define IS_I2C_GET_FLAG(FLAG)             (((FLAG) == I2C_FLAG_IS)) 
/**
  * @}
  */






/** @defgroup I2C_StartStopMode_definition 
  * @{
  */

#define  I2C_No_StartStop                 ((uint32_t)0x00000000)
#define  I2C_Generate_Stop                I2C_CR2_STOP
#define  I2C_Generate_Start_Read          (uint32_t)(I2C_CR2_START | I2C_CR2_RD_WRN)
#define  I2C_Generate_Start_Write         I2C_CR2_START

                              
#define IS_START_STOP_MODE(MODE)        (((MODE) == I2C_Generate_Stop) || \
                                         ((MODE) == I2C_Generate_Start_Read) || \
                                         ((MODE) == I2C_Generate_Start_Write) || \
                                         ((MODE) == I2C_No_StartStop))
                               

/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


/* Initialization and Configuration functions *********************************/


/* Communications handling functions ******************************************/



/* I2C registers management functions *****************************************/

/* Data transfers management functions ****************************************/



/* Interrupts and flags management functions **********************************/



#ifdef __cplusplus
}
#endif
void I2C_Init(CMSDK_I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct);
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct);
void I2C_Cmd(CMSDK_I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_GeneralCallCmd(CMSDK_I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_SlaveAddressConfig(CMSDK_I2C_TypeDef* I2Cx, uint16_t Address);
void I2C_GenerateSTART(CMSDK_I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_GenerateSTOP(CMSDK_I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_AcknowledgeConfig(CMSDK_I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_SendData(CMSDK_I2C_TypeDef* I2Cx, uint8_t Data);
uint8_t I2C_ReceiveData(CMSDK_I2C_TypeDef* I2Cx);
uint32_t I2C_GetStatus(CMSDK_I2C_TypeDef* I2Cx);
void I2C_TransferHandling(CMSDK_I2C_TypeDef* I2Cx, uint8_t StatValue);
uint32_t I2C_StartHandling(CMSDK_I2C_TypeDef* I2Cx);
uint32_t I2C_StopHandling(CMSDK_I2C_TypeDef* I2Cx);
void i2cInit(CMSDK_I2C_TypeDef* I2Cx ,uint32_t I2CMode ,IRQn_Type I2Cx_IRQn);
uint32_t i2cEngine(CMSDK_I2C_TypeDef* I2Cx);
#endif /*__PT32Z192XX_I2C_H */

/**
  * @}
  */

/**
  * @}
  */

/************************   *****END OF FILE****/
