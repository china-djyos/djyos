/**
  ******************************************************************************
  * @file    stm32l4xx_ll_swpmi.c
  * @author  MCD Application Team
  * @version V1.7.2
  * @date    16-June-2017
  * @brief   SWPMI LL module driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#if defined(USE_FULL_LL_DRIVER)

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_ll_swpmi.h"
#include "stm32l4xx_ll_bus.h"
#ifdef  USE_FULL_ASSERT
#include "stm32_assert.h"
#else
#define assert_param(expr) ((void)0U)
#endif

/** @addtogroup STM32L4xx_LL_Driver
  * @{
  */

#if defined (SWPMI1)

/** @addtogroup SWPMI_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/** @addtogroup SWPMI_LL_Private_Macros
  * @{
  */

#define IS_LL_SWPMI_BITRATE_VALUE(__VALUE__) (((__VALUE__) <= 63))

#define IS_LL_SWPMI_SW_BUFFER_RX(__VALUE__) (((__VALUE__) == LL_SWPMI_SW_BUFFER_RX_SINGLE) \
                                          || ((__VALUE__) == LL_SWPMI_SW_BUFFER_RX_MULTI))

#define IS_LL_SWPMI_SW_BUFFER_TX(__VALUE__) (((__VALUE__) == LL_SWPMI_SW_BUFFER_TX_SINGLE) \
                                          || ((__VALUE__) == LL_SWPMI_SW_BUFFER_TX_MULTI))

#define IS_LL_SWPMI_VOLTAGE_CLASS(__VALUE__) (((__VALUE__) == LL_SWPMI_VOLTAGE_CLASS_C) \
                                           || ((__VALUE__) == LL_SWPMI_VOLTAGE_CLASS_B))

/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup SWPMI_LL_Exported_Functions
  * @{
  */

/** @addtogroup SWPMI_LL_EF_Init
  * @{
  */

/**
  * @brief  De-initialize the SWPMI peripheral registers to their default reset values.
  * @param  SWPMIx SWPMI Instance
  * @retval An ErrorStatus enumeration value
  *          - SUCCESS: SWPMI registers are de-initialized
  *          - ERROR: Not applicable
  */
ErrorStatus LL_SWPMI_DeInit(SWPMI_TypeDef *SWPMIx)
{
  /* Check the parameter */
  assert_param(IS_SWPMI_INSTANCE(SWPMIx));

  LL_APB1_GRP2_ForceReset(LL_APB1_GRP2_PERIPH_SWPMI1);
  LL_APB1_GRP2_ReleaseReset(LL_APB1_GRP2_PERIPH_SWPMI1);

  return SUCCESS;
}

/**
  * @brief  Initialize the SWPMI peripheral according to the specified parameters in the SWPMI_InitStruct.
  * @note   As some bits in SWPMI configuration registers can only be written when the SWPMI is deactivated (SWPMI_CR_SWPACT bit = 0),
  *         SWPMI IP should be in deactivated state prior calling this function. Otherwise, ERROR result will be returned.
  * @param  SWPMIx           SWPMI Instance
  * @param  SWPMI_InitStruct pointer to a @ref LL_SWPMI_InitTypeDef structure that contains
  *                          the configuration information for the SWPMI peripheral.
  * @retval An ErrorStatus enumeration value
  *          - SUCCESS: SWPMI registers are initialized
  *          - ERROR: SWPMI registers are not initialized
  */
ErrorStatus LL_SWPMI_Init(SWPMI_TypeDef *SWPMIx, LL_SWPMI_InitTypeDef *SWPMI_InitStruct)
{
  ErrorStatus status = SUCCESS;

  /* Check the parameters */
  assert_param(IS_SWPMI_INSTANCE(SWPMIx));
  assert_param(IS_LL_SWPMI_BITRATE_VALUE(SWPMI_InitStruct->BitRatePrescaler));
  assert_param(IS_LL_SWPMI_SW_BUFFER_TX(SWPMI_InitStruct->TxBufferingMode));
  assert_param(IS_LL_SWPMI_SW_BUFFER_RX(SWPMI_InitStruct->RxBufferingMode));
  assert_param(IS_LL_SWPMI_VOLTAGE_CLASS(SWPMI_InitStruct->VoltageClass));

  /* SWPMI needs to be in deactivated state, in order to be able to configure some bits */
  if (LL_SWPMI_IsActivated(SWPMIx) == 0)
  {
    /* Configure the BRR register (Bitrate) */
    LL_SWPMI_SetBitRatePrescaler(SWPMIx, SWPMI_InitStruct->BitRatePrescaler);

    /* Configure the voltage class */
    LL_SWPMI_SetVoltageClass(SWPMIx, SWPMI_InitStruct->VoltageClass);

    /* Set the new configuration of the SWPMI peripheral */
    MODIFY_REG(SWPMIx->CR,
              (SWPMI_CR_RXMODE | SWPMI_CR_TXMODE),
              (SWPMI_InitStruct->TxBufferingMode | SWPMI_InitStruct->RxBufferingMode));
  }
  /* Else (SWPMI not in deactivated state => return ERROR) */
  else
  {
    status = ERROR;
  }

  return status;
}

/**
  * @brief  Set each @ref LL_SWPMI_InitTypeDef field to default value.
  * @param  SWPMI_InitStruct pointer to a @ref LL_SWPMI_InitTypeDef structure that contains
  *                          the configuration information for the SWPMI peripheral.
  * @retval None
  */
void LL_SWPMI_StructInit(LL_SWPMI_InitTypeDef *SWPMI_InitStruct)
{
  /* Set SWPMI_InitStruct fields to default values */
  SWPMI_InitStruct->VoltageClass     = LL_SWPMI_VOLTAGE_CLASS_C;
  SWPMI_InitStruct->BitRatePrescaler = (uint32_t)0x00000001;
  SWPMI_InitStruct->TxBufferingMode  = LL_SWPMI_SW_BUFFER_TX_SINGLE;
  SWPMI_InitStruct->RxBufferingMode  = LL_SWPMI_SW_BUFFER_RX_SINGLE;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* defined (SWPMI1) */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
