/**
  ******************************************************************************
  * @file    pt32z192xx_gpio.h
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
#ifndef __PT32Z192XX_GPIO_H
#define __PT32Z192XX_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx.h"
/** @addtogroup PT32Z192xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup GPIO
  * @{
  */
/* Exported types ------------------------------------------------------------*/

#define IS_GPIO_ALL_PERIPH(PERIPH) (((PERIPH) == CMSDK_PA) || \
                                    ((PERIPH) == CMSDK_PB) || \
                                    ((PERIPH) == CMSDK_PC) || \
                                                                        ((PERIPH) == CMSDK_PD))

/** @defgroup Configuration_Mode_enumeration
  * @{
  */
typedef enum
{
  GPIO_Mode_IN   = 0x00, /*!< GPIO Input Mode              */

  GPIO_OType_PP,         /*!< GPIO Output Mode             */

    GPIO_OType_OD,

  GPIO_Mode_AN           /*!< GPIO Analog In/Out Mode      */

}GPIOMode_TypeDef;

#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_IN)|| ((MODE) == GPIO_OType_PP) || \
                            ((MODE) == GPIO_OType_OD)|| ((MODE) == GPIO_Mode_AN))
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup Configuration_Pull-Up_Pull-Down_enumeration
  * @{
  */
typedef enum
{
  GPIO_PuPd_NOPULL = 0x00,

  GPIO_PuPd_UP     = 0x01,

  GPIO_PuPd_DOWN   = 0x02

}GPIOPuPd_TypeDef;

#define IS_GPIO_PUPD(PUPD) (((PUPD) == GPIO_PuPd_NOPULL) || ((PUPD) == GPIO_PuPd_UP) || \
                            ((PUPD) == GPIO_PuPd_DOWN))
/**
  * @}
  */


/** @defgroup Bit_SET_and_Bit_RESET_enumeration
  * @{
  */
typedef enum
{
  Bit_RESET = 0,

  Bit_SET

}BitAction;

#define IS_GPIO_BIT_ACTION(ACTION) (((ACTION) == Bit_RESET) || ((ACTION) == Bit_SET))
/**
  * @}
  */

/**
  * @brief  GPIO Init structure definition
  */
typedef struct
{
  uint32_t GPIO_Pin;              /*!< Specifies the GPIO pins to be configured.
                                       This parameter can be any value of @ref GPIO_pins_define */

  GPIOMode_TypeDef GPIO_Mode;     /*!< Specifies the operating mode for the selected pins.
                                       This parameter can be a value of @ref GPIOMode_TypeDef   */

  GPIOPuPd_TypeDef GPIO_PuPd;     /*!< Specifies the operating Pull-up/Pull down for the selected pins.
                                       This parameter can be a value of @ref GPIOPuPd_TypeDef   */
}GPIO_InitTypeDef;


typedef enum
{
  GPIOTI_Trigger_Rising,

  GPIOTI_Trigger_Falling,

  GPIOTI_Trigger_Rising_Falling,

    GPIOTI_Trigger_High_Level,

    GPIOTI_Trigger_Low_Level

}GPIOITrigger_TypeDef;

#define IS_GPIOTI_TRIGGER(TRIGGER) (((TRIGGER) == GPIOTI_Trigger_Rising) || \
                                    ((TRIGGER) == GPIOTI_Trigger_Falling) || \
                                    ((TRIGGER) == GPIOTI_Trigger_Rising_Falling) || \
                                    ((TRIGGER) == GPIOTI_Trigger_High_Level) || \
                                    ((TRIGGER) == GPIOTI_Trigger_Low_Level))
/**
  * @brief  EXTI Init Structure definition
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup GPIO_Exported_Constants
  * @{
  */

/** @defgroup GPIO_pins_define
  * @{
  */
#define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< Pin 0 selected    */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /*!< Pin 1 selected    */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /*!< Pin 2 selected    */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /*!< Pin 3 selected    */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /*!< Pin 4 selected    */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /*!< Pin 5 selected    */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /*!< Pin 6 selected    */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /*!< Pin 7 selected    */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< Pin 8 selected    */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected    */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< Pin 10 selected   */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< Pin 11 selected   */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< Pin 12 selected   */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< Pin 13 selected   */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< Pin 14 selected   */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< Pin 15 selected   */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< All pins selected */

#define IS_GPIO_PIN(PIN) ((PIN) != (uint16_t)0x00)

#define IS_GET_GPIO_PIN(PIN) (((PIN) == GPIO_Pin_0) || \
                              ((PIN) == GPIO_Pin_1) || \
                              ((PIN) == GPIO_Pin_2) || \
                              ((PIN) == GPIO_Pin_3) || \
                              ((PIN) == GPIO_Pin_4) || \
                              ((PIN) == GPIO_Pin_5) || \
                              ((PIN) == GPIO_Pin_6) || \
                              ((PIN) == GPIO_Pin_7) || \
                              ((PIN) == GPIO_Pin_8) || \
                              ((PIN) == GPIO_Pin_9) || \
                              ((PIN) == GPIO_Pin_10) || \
                              ((PIN) == GPIO_Pin_11) || \
                              ((PIN) == GPIO_Pin_12) || \
                              ((PIN) == GPIO_Pin_13) || \
                              ((PIN) == GPIO_Pin_14) || \
                              ((PIN) == GPIO_Pin_15))

/**
  * @}
  */

/** @defgroup GPIO_Pin_sources
  * @{
  */
#define GPIO_PinSource0            ((uint8_t)0x00)
#define GPIO_PinSource1            ((uint8_t)0x01)
#define GPIO_PinSource2            ((uint8_t)0x02)
#define GPIO_PinSource3            ((uint8_t)0x03)
#define GPIO_PinSource4            ((uint8_t)0x04)
#define GPIO_PinSource5            ((uint8_t)0x05)
#define GPIO_PinSource6            ((uint8_t)0x06)
#define GPIO_PinSource7            ((uint8_t)0x07)
#define GPIO_PinSource8            ((uint8_t)0x08)
#define GPIO_PinSource9            ((uint8_t)0x09)
#define GPIO_PinSource10           ((uint8_t)0x0A)
#define GPIO_PinSource11           ((uint8_t)0x0B)
#define GPIO_PinSource12           ((uint8_t)0x0C)
#define GPIO_PinSource13           ((uint8_t)0x0D)
#define GPIO_PinSource14           ((uint8_t)0x0E)
#define GPIO_PinSource15           ((uint8_t)0x0F)

#define IS_GPIO_PIN_SOURCE(PINSOURCE) (((PINSOURCE) == GPIO_PinSource0) || \
                                       ((PINSOURCE) == GPIO_PinSource1) || \
                                       ((PINSOURCE) == GPIO_PinSource2) || \
                                       ((PINSOURCE) == GPIO_PinSource3) || \
                                       ((PINSOURCE) == GPIO_PinSource4) || \
                                       ((PINSOURCE) == GPIO_PinSource5) || \
                                       ((PINSOURCE) == GPIO_PinSource6) || \
                                       ((PINSOURCE) == GPIO_PinSource7) || \
                                       ((PINSOURCE) == GPIO_PinSource8) || \
                                       ((PINSOURCE) == GPIO_PinSource9) || \
                                       ((PINSOURCE) == GPIO_PinSource10) || \
                                       ((PINSOURCE) == GPIO_PinSource11) || \
                                       ((PINSOURCE) == GPIO_PinSource12) || \
                                       ((PINSOURCE) == GPIO_PinSource13) || \
                                       ((PINSOURCE) == GPIO_PinSource14) || \
                                       ((PINSOURCE) == GPIO_PinSource15))
/**
  * @}
  */

#define GPIO_FLAG_PR0                          GPIO_PR_PR0                    /*!< Port interrupt pending bit set, bit 0 */
#define GPIO_FLAG_PR1                          GPIO_PR_PR1                    /*!< Port interrupt pending bit set, bit 1 */
#define GPIO_FLAG_PR2                          GPIO_PR_PR2                    /*!< Port interrupt pending bit set, bit 2 */
#define GPIO_FLAG_PR3                          GPIO_PR_PR3                    /*!< Port interrupt pending bit set, bit 3 */
#define GPIO_FLAG_PR4                          GPIO_PR_PR4                    /*!< Port interrupt pending bit set, bit 4 */
#define GPIO_FLAG_PR5                          GPIO_PR_PR5                    /*!< Port interrupt pending bit set, bit 5 */
#define GPIO_FLAG_PR6                          GPIO_PR_PR6                    /*!< Port interrupt pending bit set, bit 6 */
#define GPIO_FLAG_PR7                          GPIO_PR_PR7                    /*!< Port interrupt pending bit set, bit 7 */
#define GPIO_FLAG_PR8                          GPIO_PR_PR8                    /*!< Port interrupt pending bit set, bit 8 */
#define GPIO_FLAG_PR9                          GPIO_PR_PR9                    /*!< Port interrupt pending bit set, bit 9 */
#define GPIO_FLAG_PR10                         GPIO_PR_PR10                   /*!< Port interrupt pending bit set, bit 10 */
#define GPIO_FLAG_PR11                         GPIO_PR_PR11                   /*!< Port interrupt pending bit set, bit 11 */
#define GPIO_FLAG_PR12                         GPIO_PR_PR12                   /*!< Port interrupt pending bit set, bit 12 */
#define GPIO_FLAG_PR13                         GPIO_PR_PR13                   /*!< Port interrupt pending bit set, bit 13 */
#define GPIO_FLAG_PR14                         GPIO_PR_PR14                   /*!< Port interrupt pending bit set, bit 14 */
#define GPIO_FLAG_PR15                         GPIO_PR_PR15                   /*!< Port interrupt pending bit set, bit 15 */

#define IS_GPIO_CLEAR_FLAG(FLAG) (((FLAG) != (uint32_t)RESET) && (((FLAG) & 0xFFFF0000) == (uint32_t)RESET))

#define IS_GPIO_GET_FLAG(FLAG)   (((FLAG) == GPIO_FLAG_PR0) || ((FLAG) == GPIO_FLAG_PR1) || \
                                  ((FLAG) == GPIO_FLAG_PR2) || ((FLAG) == GPIO_FLAG_PR3) || \
                                  ((FLAG) == GPIO_FLAG_PR4) || ((FLAG) == GPIO_FLAG_PR5) || \
                                  ((FLAG) == GPIO_FLAG_PR6) || ((FLAG) == GPIO_FLAG_PR7) || \
                                  ((FLAG) == GPIO_FLAG_PR8) || ((FLAG) == GPIO_FLAG_PR9) || \
                                  ((FLAG) == GPIO_FLAG_PR10) || ((FLAG) == GPIO_FLAG_PR11) || \
                                  ((FLAG) == GPIO_FLAG_PR12) || ((FLAG) == GPIO_FLAG_PR13) || \
                                  ((FLAG) == GPIO_FLAG_PR14) || ((FLAG) == GPIO_FLAG_PR15))

/** @defgroup GPIO_Interrupt_definition
  * @brief GPIO Interrupt definition
  * @{
  */

#define GPIO_IT_ITE0                          GPIO_IES_IES0                  /*!< Port interrupt enable bit set, bit 0 */
#define GPIO_IT_ITE1                          GPIO_IES_IES1                  /*!< Port interrupt enable bit set, bit 1 */
#define GPIO_IT_ITE2                          GPIO_IES_IES2                  /*!< Port interrupt enable bit set, bit 2 */
#define GPIO_IT_ITE3                          GPIO_IES_IES3                  /*!< Port interrupt enable bit set, bit 3 */
#define GPIO_IT_ITE4                          GPIO_IES_IES4                  /*!< Port interrupt enable bit set, bit 4 */
#define GPIO_IT_ITE5                          GPIO_IES_IES5                  /*!< Port interrupt enable bit set, bit 5 */
#define GPIO_IT_ITE6                          GPIO_IES_IES6                  /*!< Port interrupt enable bit set, bit 6 */
#define GPIO_IT_ITE7                          GPIO_IES_IES7                  /*!< Port interrupt enable bit set, bit 7 */
#define GPIO_IT_ITE8                          GPIO_IES_IES8                  /*!< Port interrupt enable bit set, bit 8 */
#define GPIO_IT_ITE9                          GPIO_IES_IES9                  /*!< Port interrupt enable bit set, bit 9 */
#define GPIO_IT_ITE10                         GPIO_IES_IES10                 /*!< Port interrupt enable bit set, bit 10 */
#define GPIO_IT_ITE11                         GPIO_IES_IES11                 /*!< Port interrupt enable bit set, bit 11 */
#define GPIO_IT_ITE12                         GPIO_IES_IES12                 /*!< Port interrupt enable bit set, bit 12 */
#define GPIO_IT_ITE13                         GPIO_IES_IES13                 /*!< Port interrupt enable bit set, bit 13 */
#define GPIO_IT_ITE14                         GPIO_IES_IES14                 /*!< Port interrupt enable bit set, bit 14 */
#define GPIO_IT_ITE15                         GPIO_IES_IES15                 /*!< Port interrupt enable bit set, bit 15 */


#define IS_GPIO_CONFIG_IT(IT)       (((IT) != (uint32_t)RESET) && (((IT) & 0xFFFF0000) == (uint32_t)RESET))
#define IS_GPIO_GET_IT(IT)        (((IT) == GPIO_IT_ITE0) || \
                                     ((IT) == GPIO_IT_ITE1) || \
                                     ((IT) == GPIO_IT_ITE2) || \
                                     ((IT) == GPIO_IT_ITE3) || \
                                     ((IT) == GPIO_IT_ITE4) || \
                                     ((IT) == GPIO_IT_ITE5) || \
                                     ((IT) == GPIO_IT_ITE6) || \
                                     ((IT) == GPIO_IT_ITE7) || \
                                     ((IT) == GPIO_IT_ITE8) || \
                                     ((IT) == GPIO_IT_ITE9) || \
                                     ((IT) == GPIO_IT_ITE10) || \
                                     ((IT) == GPIO_IT_ITE11) || \
                                     ((IT) == GPIO_IT_ITE12) || \
                                     ((IT) == GPIO_IT_ITE13) || \
                                     ((IT) == GPIO_IT_ITE14) || \
                                     ((IT) == GPIO_IT_ITE15))
#define IS_GPIO_CLEAR_IT(IT) (((IT) != (uint32_t)RESET) && (((IT) & 0xFFFF0000) == (uint32_t)RESET))

/**
  * @}
  */

/** @defgroup GPIO_Alternate_function_selection_define
  * @{
  */

/**
  * @brief  AF Primary selection
  */
#define GPIO_AFS_Primary     ((uint8_t)0x00)
/**
  * @brief  AF 0 selection
  */

#define GPIO_AF_0            ((uint8_t)0x00) /*



                                              */
/**
  * @brief  AF 1 selection
  */
#define GPIO_AF_1            ((uint8_t)0x01) /*

                                              */
/**
  * @brief  AF 2 selection
  */
#define GPIO_AF_2            ((uint8_t)0x02) /*
                                              */
/**
  * @brief  AF 3 selection
  */
#define GPIO_AF_3            ((uint8_t)0x03) /*  */

/**
  * @brief  AF 4 selection
  */
#define GPIO_AF_4            ((uint8_t)0x04) /*
                                              */

/**
  * @brief  AF 5 selection
  */
#define GPIO_AF_5            ((uint8_t)0x05) /*
                                              */

/**
  * @brief  AF 6 selection
  */
#define GPIO_AF_6            ((uint8_t)0x06) /*  */


#define IS_GPIO_AF(AF)   (((AF) == GPIO_AF_0) || ((AF) == GPIO_AF_1) || \
                          ((AF) == GPIO_AF_2) || ((AF) == GPIO_AF_3) || \
                          ((AF) == GPIO_AF_4) || ((AF) == GPIO_AF_5) || \
                          ((AF) == GPIO_AF_6))

/**
  * @}
  */



/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Initialization and Configuration functions *********************************/
void GPIO_Init(CMSDK_GPIO_TypeDef* CMSDK_Px, GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_AnalogFunctionConfig(CMSDK_GPIO_TypeDef* CMSDK_Px, uint32_t GPIO_Pin, FunctionalState NewState);
/* GPIO Read and Write functions **********************************************/
uint8_t GPIO_ReadInputDataBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin);
uint16_t GPIO_ReadInputData(CMSDK_GPIO_TypeDef* CMSDK_Px);
uint8_t GPIO_ReadOutputDataBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin);
uint16_t GPIO_ReadOutputData(CMSDK_GPIO_TypeDef* CMSDK_Px);
void GPIO_SetBits(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin);
void GPIO_ResetBits(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin);
void GPIO_WriteBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin, BitAction BitVal);
void GPIO_Write(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t PortVal);
void GPIO_ReverseBits(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin);
/* GPIO Alternate functions configuration functions ***************************/
void GPIO_PinAFConfig(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_PinSource, uint8_t GPIO_AF);

/* Interrupts and flags management functions **********************************/
void GPIO_ITConfig(CMSDK_GPIO_TypeDef* CMSDK_Px, uint32_t GPIO_IT, FunctionalState NewState);
void GPIO_TriTypeConfig(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin, uint32_t Trigger_Type);
FlagStatus GPIO_GetFlagStatus(CMSDK_GPIO_TypeDef* CMSDK_Px, uint32_t GPIO_IT_FLAG);
void GPIO_ClearFlag(CMSDK_GPIO_TypeDef* CMSDK_Px, uint32_t GPIO_IT_FLAG);
void GPIO_ClearITPendingBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint32_t GPIO_IT);
ITStatus GPIO_GetITStatus(CMSDK_GPIO_TypeDef* CMSDK_Px, uint32_t GPIO_IT);
void GPIO_PullUpConfig(CMSDK_GPIO_TypeDef* CMSDK_Px, uint32_t GPIO_Pin, FunctionalState NewState);
void GPIO_PullDownConfig(CMSDK_GPIO_TypeDef* CMSDK_Px, uint32_t GPIO_Pin, FunctionalState NewState);
void GPIO_LowByte_SetBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin);
void GPIO_LowByte_ClrBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin);
void GPIO_LowByte_WriteBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin,uint16_t bitDat);
void GPIO_HigByte_SetBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin);
void GPIO_HigByte_ClrBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin);
void GPIO_HigByte_WriteBit(CMSDK_GPIO_TypeDef* CMSDK_Px, uint16_t GPIO_Pin,uint16_t bitDat);
#ifdef __cplusplus
}
#endif

#endif /* __PT32Z192XX_GPIO_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT  *****END OF FILE****/
