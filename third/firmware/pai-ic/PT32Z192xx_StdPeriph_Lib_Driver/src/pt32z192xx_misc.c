/**
  ******************************************************************************
  * @file    pt32z192xx_misc.c
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   This file provides all the miscellaneous firmware functions (add-on
  *          to CMSIS functions).
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
#include "pt32z192xx_misc.h"

/** @addtogroup PT32F0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup MISC 
  * @brief MISC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup MISC_Private_Functions
  * @{
  */
/**
  *
@verbatim
 *******************************************************************************
                   ##### Interrupts configuration functions #####
 *******************************************************************************
    [..] This section provide functions allowing to configure the NVIC interrupts
        (IRQ). The Cortex-M0 exceptions are managed by CMSIS functions.
         (#) Enable and Configure the priority of the selected IRQ Channels. 
             The priority can be 0..3. 

        -@- Lower priority values gives higher priority.
        -@- Priority Order:
            (#@) Lowest priority.
            (#@) Lowest hardware priority (IRQn position).  
  
@endverbatim
*/

/**
  * @brief  Initializes the NVIC peripheral according to the specified
  *         parameters in the NVIC_InitStruct.
  * @param  NVIC_InitStruct: pointer to a NVIC_InitTypeDef structure that contains
  *         the configuration information for the specified NVIC peripheral.
  * @retval None
  */
void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct)
{
  uint32_t tmppriority = 0x00;
  
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));
  assert_param(IS_NVIC_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelPriority));  
    
  if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
  {
    /* Compute the Corresponding IRQ Priority --------------------------------*/    
    tmppriority = NVIC->IP[NVIC_InitStruct->NVIC_IRQChannel >> 0x02];
    tmppriority &= (uint32_t)(~(((uint32_t)0xFF) << ((NVIC_InitStruct->NVIC_IRQChannel & 0x03) * 8)));
    tmppriority |= (uint32_t)((((uint32_t)NVIC_InitStruct->NVIC_IRQChannelPriority << 6) & 0xFF) << ((NVIC_InitStruct->NVIC_IRQChannel & 0x03) * 8));    
    
    NVIC->IP[NVIC_InitStruct->NVIC_IRQChannel >> 0x02] = tmppriority;
    
    /* Enable the Selected IRQ Channels --------------------------------------*/
		  NVIC->ISER[(((uint32_t)NVIC_InitStruct->NVIC_IRQChannel) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)NVIC_InitStruct->NVIC_IRQChannel) & 0x1FUL));
  }
  else
  {
    /* Disable the Selected IRQ Channels -------------------------------------*/
    NVIC->ICER[0] = (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
  }
}

/**
  * @brief  Selects the condition for the system to enter low power mode.
  * @param  LowPowerMode: Specifies the new mode for the system to enter low power mode.
  *          This parameter can be one of the following values:
  *            @arg NVIC_LP_SEVONPEND: Low Power SEV on Pend.
  *            @arg NVIC_LP_SLEEPDEEP: Low Power DEEPSLEEP request.
  *            @arg NVIC_LP_SLEEPONEXIT: Low Power Sleep on Exit.
  * @param  NewState: new state of LP condition. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void NVIC_SystemLPConfig(uint8_t LowPowerMode, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_NVIC_LP(LowPowerMode));
  
  assert_param(IS_FUNCTIONAL_STATE(NewState));  
  
  if (NewState != DISABLE)
  {
    SCB->SCR |= LowPowerMode;
  }
  else
  {
    SCB->SCR &= (uint32_t)(~(uint32_t)LowPowerMode);
  }
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

