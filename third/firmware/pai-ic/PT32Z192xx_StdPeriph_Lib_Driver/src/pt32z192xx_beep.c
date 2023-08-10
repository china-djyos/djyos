#if 0       //lst dbg
/**
  ******************************************************************************
  * @file    pt32z192xx_beep.c
  * @author  应用开发团队
  * @version
  * @date
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the Analog to 7SEG LED Display peripheral:
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
#include "pt32z192xx_beep.h"

/** @addtogroup PT32F0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup BEEP
  * @brief BEEP driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/** @defgroup BEEP_Group1 Configuration of the BEEP computation unit functions
 *  @brief   Configuration of the BEEP  unit functions
 *
@verbatim
 ===============================================================================
                     ##### BEEP configuration functions #####
 ===============================================================================

@endverbatim
  * @{
  */
/**
  * @brief
  * @param
  * @param
  * @retval
  */
/** @defgroup BEEP_Private_Functions
  * @{
  */
void Beep_Init(Beep_InitTypeDef* Beep_InitStruct)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_BEEP_SUBDIV(Beep_InitStruct->Subdivide));
  assert_param(IS_BEEP_CPSR(Beep_InitStruct->Prescaler));
  /*  */
  tmpreg = BEEP->CR;
  /*  */
  tmpreg &= (uint32_t)~(uint32_t)(BEEP_CR_CPSR | BEEP_CR_SUBDIV);
  /*  */
  tmpreg  |= ((uint32_t)(Beep_InitStruct->Subdivide));
    tmpreg  |= ((uint32_t)((Beep_InitStruct->Prescaler)<<0x01));

  BEEP->CR = tmpreg;
}

/**
  * @brief  Enables or disables the  BEEP peripheral.
  * @param  NewState: new state of the USARTx peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void Beep_Cmd(FunctionalState NewState)
{
    /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /**/
    BEEP->CR |= BEEP_CR_EN;
  }
  else
  {
    /* */
    BEEP->CR &= (uint32_t)~((uint32_t)BEEP_CR_EN);
  }
}


/**
  * @brief
  * @param
  * @param
  * @retval
  */
void Beep_PrescalerConfig(uint8_t Prescaler)
{
  uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_BEEP_CPSR(Prescaler));
  /*  */
  tmpreg = BEEP->CR;
  /*  */
  tmpreg &= (uint32_t)~(uint32_t)(BEEP_CR_CPSR);
  /*  */
    tmpreg  |= ((uint32_t)(Prescaler<<0x01));
  /* Write to CRC CR */
  BEEP->CR = tmpreg;
}

/**
  * @brief
  * @param
  * @param
  * @retval
  */
void Beep_SubDivConfig(uint8_t Subdivide)
{
  uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_BEEP_SUBDIV(Subdivide));
  /*  */
  tmpreg = BEEP->CR;
  /*  */
  tmpreg &= (uint32_t)~(uint32_t)(BEEP_CR_SUBDIV);
  /*  */
    tmpreg  |= ((uint32_t)(Subdivide));
  /* Write to CRC CR */
  BEEP->CR = tmpreg;
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

/************************ (C)  *****END OF FILE****/
#endif          //lst dbg
