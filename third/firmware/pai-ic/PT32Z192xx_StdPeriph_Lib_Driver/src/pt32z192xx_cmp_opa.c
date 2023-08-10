#if 0       //lst dbg
/**
  ******************************************************************************
  * @file    pt32z192xx_led.c
  * @author  应用开发团队
  * @version
  * @date
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the Analog comparators and operational amplifiers peripheral:
  *           + Initialization and Configuration
  *           +
  *           + Interrupts and flags management
  *
  *  @verbatim
================================================================================
                      ##### How to use this driver #####
================================================================================
    [..]


  *  @endverbatim
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

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx_cmp_opa.h"

/** @addtogroup PT32F0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup
  * @brief
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup
  * @{
  */

/** @defgroup
 *  @brief   Initialization and Configuration functions
 *
@verbatim
 ===============================================================================
          ##### Initialization and Configuration functions #####
 ===============================================================================


@endverbatim
  * @{
  */

/**
  * @brief
  *
  * @note
  *
  *
  * @param
  * @param
  *
  * @retval
  */
void CMP_Init(CMSDK_CMPOPA_TypeDef* CMPOPAx, CMPOPA_InitTypeDef* CMPOPA_InitStruct)
{
  uint32_t tmpreg = 0;


  assert_param(IS_CMPOPA_ALL_PERIPH(CMPOPAx));
  assert_param(IS_CMP_REFSEL(CMPOPA_InitStruct->CMP_PosInputSEL));
    assert_param(IS_CMP_FSTIMES(CMPOPA_InitStruct->CMP_FilterSampling));
  assert_param(IS_FUNCTIONAL_STATE(CMPOPA_InitStruct->CMP_HSMode));
    assert_param(IS_LED_DISPOS(CMPOPA_InitStruct->CMP_PolarityReversal));
  tmpreg = CMPOPAx->CR1;
  /*  */
  tmpreg &= (uint32_t)~(uint32_t)(CMPOPA_CR1_REFSEL | CMPOPA_CR1_FS);
  /*  */
  tmpreg  |= ((uint32_t)((CMPOPA_InitStruct->CMP_PosInputSEL) | (CMPOPA_InitStruct->CMP_FilterSampling)));
    tmpreg  |= ((uint32_t)(((CMPOPA_InitStruct->CMP_PosInputSEL)<<0x07) | ((CMPOPA_InitStruct->CMP_FilterSampling)<<0x08)));
  /* Write to CMPOPAx CR1 */
  CMPOPAx->CR1 = tmpreg;


}

/**
  * @brief
  * @note
  *
  *
  * @param
  *
  * @retval
  */
void CMP_StructInit(CMPOPA_InitTypeDef* CMPOPA_InitStruct)
{

  CMPOPA_InitStruct->CMP_PosInputSEL = CMP_REFSEL_PIN;


  CMPOPA_InitStruct->CMP_FilterSampling =CMP_FSTIMES_NONE;


    CMPOPA_InitStruct->CMP_HSMode = DISABLE;


    CMPOPA_InitStruct->CMP_PolarityReversal= DISABLE;

}

/**
  * @brief
  * @param
  * @param
  *
  * @retval
  */
void CMPOPA_Cmd(CMSDK_CMPOPA_TypeDef* CMPOPAx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_CMPOPA_ALL_PERIPH(CMPOPAx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    CMPOPAx->CR1 |= (uint32_t)CMPOPA_CR1_EN;
  }
  else
  {

    CMPOPAx->CR1 &= (uint32_t)~((uint32_t)CMPOPA_CR1_EN);
  }
}



void CMPOPA_OPA_Config(CMSDK_CMPOPA_TypeDef* CMPOPAx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_CMPOPA_ALL_PERIPH(CMPOPAx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /*  */
    CMPOPAx->CR2 |= (uint32_t)CMPOPA_CR2_OPAEN;
  }
  else
  {
    /*  */
    CMPOPAx->CR2 &= (uint32_t)~((uint32_t)CMPOPA_CR2_OPAEN);
  }
}


void CMPOPA_SetInitDelayValue(CMSDK_CMPOPA_TypeDef* CMPOPAx, uint32_t CMPOPA_InitaDelay)
{
  /* Check the parameters */
  assert_param(IS_CMPOPA_ALL_PERIPH(CMPOPAx));
    assert_param(IS_CMPOPA_INITDELAY_VALUE(CMPOPA_InitaDelay));
  /*  */
  CMPOPAx->INITDELAY = CMPOPA_InitaDelay;

}


void CMP_SetDACValue(CMSDK_CMPOPA_TypeDef* CMPOPAx, uint32_t CMPOPA_DACValue)
{
  /* Check the parameters */
  assert_param(IS_CMPOPA_ALL_PERIPH(CMPOPAx));
  assert_param(IS_CMPOPA_DAC_VALUE(CMPOPA_DACValue));
  /*  */
  CMPOPAx->DAC = CMPOPA_DACValue;

}






void CMP_ITConfig(CMSDK_CMPOPA_TypeDef* CMPOPAx, uint32_t CMP_IT, FunctionalState NewState)
{

  assert_param(IS_CMPOPA_ALL_PERIPH(CMPOPAx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_CMP_CONFIG_IT(CMP_IT));

  if (NewState != DISABLE)
  {

    CMPOPAx->IE |= CMP_IT;
  }
  else
  {

    CMPOPAx->IE &= (~(uint32_t)CMP_IT);
  }
}


FlagStatus CMP_GetFlagStatus(CMSDK_CMPOPA_TypeDef* CMPOPAx, uint32_t CMP_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters */
  assert_param(IS_CMPOPA_ALL_PERIPH(CMPOPAx));
  assert_param(IS_CMP_GET_FLAG(CMP_FLAG));
  /* Check the status of the specified ADC flag */
  if ((CMPOPAx->EIS & CMP_FLAG) != (uint32_t)RESET)
  {

    bitstatus = SET;
  }
  else
  {

    bitstatus = RESET;
  }

  return  bitstatus;
}


void CMP_ClearFlag(CMSDK_CMPOPA_TypeDef* CMPOPAx, uint32_t CMP_FLAG)
{

  assert_param(IS_CMPOPA_ALL_PERIPH(CMPOPAx));
  assert_param(IS_CMP_CLEAR_FLAG(CMP_FLAG));


  CMPOPAx->EIS = (uint32_t)CMP_FLAG;
}


ITStatus CMP_GetITStatus(CMSDK_CMPOPA_TypeDef* CMPOPAx, uint32_t CMP_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;

  /* Check the parameters */
  assert_param(IS_CMPOPA_ALL_PERIPH(CMPOPAx));
  assert_param(IS_CMP_GET_IT(CMP_IT));


  enablestatus = (uint32_t)(CMPOPAx->IE & CMP_IT);


  if (((uint32_t)(CMPOPAx->EIS & CMP_IT) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
  {

    bitstatus = SET;
  }
  else
  {

    bitstatus = RESET;
  }

  return  bitstatus;
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

/**
  * @}
  */
#endif          //lst dbg


