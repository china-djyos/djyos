/**
  ******************************************************************************
  * @file    pt32z192xx_adc.c
  * @author  应用开发团队
  * @version 
  * @date    
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Analog to Digital Convertor (ADC) peripheral:
  *           + Initialization and Configuration
  *           + ADC Channels Configuration
  *           + Interrupts and flags management
  *
  *  @verbatim
================================================================================
                      ##### How to use this driver #####
================================================================================
    [..]
    (#) ADC pins configuration
       (++) Configure these ADC pins in analog mode using GPIO_Init();  
    (#) Configure the ADC conversion resolution, data alignment, external
        trigger and edge, scan direction and Enable/Disable the continuous mode
        using the ADC_Init() function.
    (#) Activate the ADC peripheral using ADC_Cmd() function.

    *** ADC channels group configuration ***
    ============================================
    [..] 
    (+) To configure the ADC channels features, use ADC_Init() and 
        ADC_ChannelConfig() functions.
    (+) To activate the continuous mode, use the ADC_ContinuousModeCmd()
        function.
    (+) To activate the Discontinuous mode, use the ADC_DiscModeCmd() functions. 
    (+) To activate the calibration mode, use the ADC_GetCalibrationFactor() functions.
    (+) To read the ADC converted values, use the ADC_GetConversionValue()
        function.

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
#include "pt32z192xx_adc.h"

/** @addtogroup PT32F0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup ADC 
  * @brief ADC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* ADC CFGR mask */
#define CR0_CLEAR_MASK           ((uint32_t)0xFFFFFF03)
#define CR1_CLEAR_MASK           ((uint32_t)0xFFFF00F8)
#define CALIBRATION_TIMEOUT         ((uint32_t)0x00000FFF)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup ADC_Private_Functions
  * @{
  */

/** @defgroup ADC_Group1 Initialization and Configuration functions
 *  @brief   Initialization and Configuration functions 
 *
@verbatim
 ===============================================================================
          ##### Initialization and Configuration functions #####
 ===============================================================================
    [..] This section provides functions allowing to:
        (+) Initialize and configure the ADC Prescaler
        (+) ADC Conversion Resolution (12bit..6bit)
        (+) ADC Continuous Conversion Mode (Continuous or Single conversion)
        (+) External trigger Edge and source 
        (+) Converted data alignment (left or right)
        (+) Enable or disable the ADC peripheral
   
@endverbatim
  * @{
  */

/**
  * @brief  Initializes the ADCx peripheral according to the specified parameters
  *         in the ADC_InitStruct.
  * @note   This function is used to configure the global features of the ADC ( 
  *         Resolution, Data Alignment, continuous mode activation, External 
  *         trigger source and edge, Sequence Scan Direction).   
  * @param  ADCx: where x can be 1 to select the ADC peripheral.
  * @param  ADC_InitStruct: pointer to an ADC_InitTypeDef structure that contains 
  *         the configuration information for the specified ADC peripheral.
  * @retval None
  */
void ADC_Init(CMSDK_ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_DMA));
  assert_param(IS_ADC_CONVMODE(ADC_InitStruct->ADC_ConvMode));
  assert_param(IS_ADC_RESOLUTION(ADC_InitStruct->ADC_Resolution));
  assert_param(IS_ADC_CLOCKMODE(ADC_InitStruct->ADC_ClkDiv));
  assert_param(IS_ADC_CHSEL(ADC_InitStruct->ADC_ChSel));	
	
  /* Get the ADCx CR0 value */
  tmpreg = ADCx->CR0;

  /* Clear RESO[3:2] SAIN[8:4] bits */
  tmpreg &= CR0_CLEAR_MASK;

  /*---------------------------- ADCx CR0 Configuration ---------------------*/
  tmpreg |= (ADC_InitStruct->ADC_Resolution) | (ADC_InitStruct->ADC_ChSel);

  /* Write to ADCx CR0 */
  ADCx->CR0 = tmpreg;
	
  /* Get the ADCx CR1 value */
  tmpreg = ADCx->CR1;
	
  /* Clear DMA_MODE[0] ADC_MODE[2:1] ADC_CLKDIV[15:8] bits */
  tmpreg &= CR1_CLEAR_MASK;

  /*---------------------------- ADCx CR1 Configuration ---------------------*/
  tmpreg |= (ADC_InitStruct->ADC_ConvMode) | (ADC_InitStruct->ADC_DMA) |  ((ADC_InitStruct->ADC_ClkDiv)<<8) ;

  /* Write to ADCx CR1 */
  ADCx->CR1 = tmpreg;
	
  /* Enable ADCx Channel */ 
  ADCx->CHER = (1<<((ADC_InitStruct->ADC_ChSel)>>4));

}

/**
  * @brief  Fills each ADC_InitStruct member with its default value.
  * @note   This function is used to initialize the global features of the ADC ( 
  *         Resolution, Data Alignment, continuous mode activation, External 
  *         trigger source and edge, Sequence Scan Direction).
  * @param  ADC_InitStruct: pointer to an ADC_InitTypeDef structure which will 
  *         be initialized.
  * @retval None
  */
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
{
  /* Reset ADC init structure parameters values */

   /* Initialize the ADC_DMA MODE member */
  ADC_InitStruct->ADC_DMA = DISABLE;	

  /* Initialize the ADC Conversion MODE member */
  ADC_InitStruct->ADC_ConvMode = ADC_ConvMode_OneShot;
	
   /* Initialize the ADC_Resolution member */
  ADC_InitStruct->ADC_Resolution = ADC_Resolution_12bit;		

  /* Initialize the ADC_ClkDiv member */
  ADC_InitStruct->ADC_ClkDiv = 2;
	
   /* Initialize the ADC_ChSel member */
  ADC_InitStruct->ADC_ChSel = ADC_ChSel_0;			

}

/**
  * @brief  Enables or disables the specified ADC peripheral.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  NewState: new state of the ADCx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_Cmd(CMSDK_ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Set the ADEN bit to Enable the ADC peripheral */
    ADCx->CR0 |= (uint32_t)ADC_CR0_ADEN;
  }
  else
  {
    /* Reset the ADEN to Disable the ADC peripheral */
    ADCx->CR0 &= (uint32_t)~ADC_CR0_ADEN;
  }
}

/**
  * @brief  Configure the ADC to either be clocked by the asynchronous clock(which is
  *         independent, the dedicated 14MHz clock) or the synchronous clock derived from
  *         the APB clock of the ADC bus interface divided by 2 or 4
  * @note   This function can be called only when ADC is disabled.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  ADC_ClockMode: This parameter can be : 1~0xFF

  * @retval None
  */
void ADC_ClockModeConfig(CMSDK_ADC_TypeDef* ADCx, uint32_t ADC_ClockMode)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CLOCKMODE(ADC_ClockMode));

  ADCx->CR1 &=(uint32_t)~ADC_CR1_CLKDIV;
  /* Configure the ADC Clock mode according to ADC_ClockMode */
  ADCx->CR1 |= (uint32_t)ADC_ClockMode;

}



/**
  * @}
  */


/** @defgroup ADC_Group5 Channels Configuration functions
 *  @brief    Channels Configuration functions 
 *
@verbatim
 ===============================================================================
            ##### Channels Configuration functions #####
 ===============================================================================
    [..] This section provides functions allowing to manage the ADC channels,
         it is composed of 3 sub sections :
         (#) Configuration and management functions for ADC channels: This subsection 
             provides functions allowing to configure the ADC channels :    
             (++) Select the ADC channels
             (++) Activate the Discontinuous Mode 
             (++) Activate the Continuous Mode.
             (++) Select the conversion Trigger and Edge for ADC channels
             -@@- Please Note that the following features for ADC channels are configurated
                  using the ADC_Init() function : 
                  (+@@) Activate the Continuous Mode
                  (+@@) Select the conversion Trigger and Edge for ADC channels
         (#) Control the ADC peripheral : This subsection permits to command the ADC:
             (++) Start the ADC conversion .
         (#) Get the conversion data: This subsection provides an important function in 
             the ADC peripheral since it returns the converted data of the current 
             ADC channel. When the Conversion value is read, the EOC Flag is 
             automatically cleared.

@endverbatim
  * @{
  */

/**
  * @brief  Configures for the selected ADC and its sampling time.
  * @param  ADCx: where x can be 1 to select the ADC peripheral.
  * @param  ADC_Channel: the ADC channel to configure. 
  *          This parameter can be any combination of the following values:
  *            @arg ADC_Channel_0: ADC Channel0 selected
  *            @arg ADC_Channel_1: ADC Channel1 selected
  *            @arg ADC_Channel_2: ADC Channel2 selected
  *            @arg ADC_Channel_3: ADC Channel3 selected
  *            @arg ADC_Channel_4: ADC Channel4 selected
  *            @arg ADC_Channel_5: ADC Channel5 selected
  * @retval None
  */
void ADC_ChannelConfig(CMSDK_ADC_TypeDef* ADCx, uint32_t ADC_Channel)
{

  /* Check the parameters */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  /* Clear the ADC Channel Selection bits */
  ADCx->CR0 &=(uint32_t)~ADC_CR0_SAIN;
  /* Configure the ADC Channel */
  ADCx->CR0 |= (uint32_t)ADC_Channel;
	
  /* Enable ADCx Channel */ 
  ADCx->CHER = (1<<(ADC_Channel>>4));	
	
}

/**
  * @brief  Enable the Continuous mode for the selected ADCx channels.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  adc_mode: ADC Convertion mode.
  *          This parameter can be: ADC_ConvMode_OneShot
  *          This parameter can be: ADC_ConvMode_TimTrigger
  *          This parameter can be: ADC_ConvMode_Continue
  * @note   It is not possible to have both discontinuous mode and continuous mode
  *         enabled. In this case (If DISCEN and CONT are Set), the ADC behaves 
  *         as if continuous mode was disabled
  * @retval None
  */
void ADC_ConvModeConfig(CMSDK_ADC_TypeDef* ADCx, uint32_t adc_mode)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CONVMODE(adc_mode));
	
  /* Clear the ADC Mode Selection bits */
  ADCx->CR1 &=(uint32_t)~ADC_CR1_MODE;
  /* Configure the ADC Channel */
  ADCx->CR1 |= (uint32_t)adc_mode;

}



///**
//  * @brief  Active the Calibration operation for the selected ADC.
//  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
//  * @retval ADC Offset Calibration Value 
//  */
//uint32_t ADC_GetCalibrationFactor(CMSDK_ADC_TypeDef* ADCx)
//{
//  uint32_t tmpreg = 0, calibrationcounter = 0, calibrationstatus = 0;

//  /* Check the parameters */
//  assert_param(IS_ADC_ALL_PERIPH(ADCx));
//  
//  /* Set the ADC calibartion */
//  ADCx->CR |= (uint32_t)ADC_CR_ADCAL;
//  /*Enables ADC conversion */	
//  ADCx->CR |= (uint32_t)ADC_CR_ADEN;	
//  
//  /* Wait until ADC is readied */
//  do
//  {
//    calibrationstatus = ADCx->CR & ADC_SR_ADRDY;
//    calibrationcounter++;  
//  } while((calibrationcounter != CALIBRATION_TIMEOUT) && (calibrationstatus == 0x00));
//    
//  if((uint32_t)(ADCx->CR & ADC_CR_ADCAL) == SET)
//  {
//  ADCx->CR |=ADC_CR_ADSTART;		
//  do
//  {
//    calibrationstatus = ADCx->CR & ADC_SR_EOC;
//    calibrationcounter++;  
//  } while((calibrationcounter != CALIBRATION_TIMEOUT) && (calibrationstatus == 0x00));		

//    ADCx->CR |=	ADC_SR_EOC;
//    /*Get the Offset Calibration Value from the ADC data register */
//    tmpreg = ADCx->DR;
//  }
//  else
//  {
//    /* Error factor */
//    tmpreg = 0x00000000;
//  }
//    /* Reset the ADEN to Disable the ADC peripheral */
//    ADCx->CR &= (uint32_t)~ADC_CR_ADEN;	
//  return tmpreg;
//}

/**
  * @brief  Stop the on going conversions for the selected ADC.
  * @note   When ADSTP is set, any on going conversion is aborted, and the ADC 
  *         data register is not updated with current conversion. 
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @retval None
  */
void ADC_StopOfConversion(CMSDK_ADC_TypeDef* ADCx)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  
  ADCx->CR0 &= (uint32_t)~ADC_CR0_SOC;
}

/**
  * @brief  Start Conversion for the selected ADC channels.
  * @note   In continuous mode, ADSTART is not cleared by hardware with the 
  *         assertion of EOSEQ because the sequence is automatic relaunched
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @retval None
  */
void ADC_StartOfConversion(CMSDK_ADC_TypeDef* ADCx)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  
  ADCx->CR0 |= (uint32_t)ADC_CR0_SOC;
}

/**
  * @brief  Returns the last ADCx conversion result data for ADC channel.  
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @retval The Data conversion value.
  */
uint16_t ADC_GetConversionValue(CMSDK_ADC_TypeDef* ADCx)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));

  /* Return the selected ADC conversion value */
  return (uint16_t) ADCx->DR;
}

/**
  * @}
  */



/** @defgroup ADC_Group7 Interrupts and flags management functions
 *  @brief   Interrupts and flags management functions.
 *
@verbatim   
 ===============================================================================
            ##### Interrupts and flags management functions #####
 ===============================================================================
    [..] This section provides functions allowing to configure the ADC Interrupts 
         and get the status and clear flags and Interrupts pending bits.
  
    [..] The ADC provide 6 Interrupts sources and 11 Flags which can be divided into 
         3 groups:

  *** Flags for ADC status ***
  ======================================================
    [..]
        (+)Flags :
           (##) ADC_FLAG_ADRDY : This flag is set after the ADC has been enabled (bit ADEN=1)
               and when the ADC reaches a state where it is ready to accept conversion requests


  *** Flags and Interrupts for ADC channel conversion ***
  =====================================================
    [..]
        (+)Flags :
           (##) ADC_FLAG_EOC : This flag is set by hardware at the end of each conversion
                of a channel when a new data result is available in the data register

        (+)Interrupts :
           (##) ADC_IT_EOC : specifies the interrupt source for end of conversion .

    [..] The user should identify which mode will be used in his application to 
         manage the ADC controller events: Polling mode or Interrupt mode.
  
    [..] In the Polling Mode it is advised to use the following functions:
         (+) ADC_GetFlagStatus() : to check if flags events occur.
         (+) ADC_ClearFlag()     : to clear the flags events.
  
    [..] In the Interrupt Mode it is advised to use the following functions:
         (+) ADC_ITConfig()       : to enable or disable the interrupt source.
         (+) ADC_GetITStatus()    : to check if Interrupt occurs.
         (+) ADC_ClearITPendingBit() : to clear the Interrupt pending Bit 
             (corresponding Flag).

@endverbatim
  * @{
  */

/**
  * @brief  Enables or disables the specified ADC interrupts.
  * @param  ADCx: where x can be 1 to select the ADC peripheral.
  * @param  ADC_IT: specifies the ADC interrupt sources to be enabled or disabled.
  *          This parameter can be one of the following values:
  *            @arg ADC_IT_EOC: End of conversion interrupt 
  * @param  NewState: new state of the specified ADC interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_ITConfig(CMSDK_ADC_TypeDef* ADCx, uint32_t ADC_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_ADC_CONFIG_IT(ADC_IT)); 

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC interrupts */
    ADCx->IER |= ADC_IT;
  }
  else
  {
    /* Disable the selected ADC interrupts */
    ADCx->IER &= (~(uint32_t)ADC_IT);
  }
}

/**
  * @brief  Checks whether the specified ADC flag is set or not.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  ADC_FLAG: specifies the flag to check. 
  *          This parameter can be one of the following values:
  *            @arg ADC_FLAG_EOC: End of conversion flag
  * @retval The new state of ADC_FLAG (SET or RESET).
  */
FlagStatus ADC_GetFlagStatus(CMSDK_ADC_TypeDef* ADCx, uint32_t ADC_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_GET_FLAG(ADC_FLAG));
  /* Check the status of the specified ADC flag */
  if ((ADCx->SR & ADC_FLAG) != (uint32_t)RESET)
  {
    /* ADC_FLAG is set */
    bitstatus = SET;
  }
  else
  {
    /* ADC_FLAG is reset */
    bitstatus = RESET;
  }
  /* Return the ADC_FLAG status */
  return  bitstatus;
}

/**
  * @brief  Clears the ADCx's pending flags.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @param  ADC_FLAG: specifies the flag to clear. 
  *          This parameter can be any combination of the following values:
  *            @arg ADC_FLAG_EOC: End of conversion flag
  * @retval None
  */
void ADC_ClearFlag(CMSDK_ADC_TypeDef* ADCx, uint32_t ADC_FLAG)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CLEAR_FLAG(ADC_FLAG));

  /* Clear the selected ADC flags */
  ADCx->SR = (uint32_t)ADC_FLAG;
}

/**
  * @brief  Checks whether the specified ADC interrupt has occurred or not.
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral
  * @param  ADC_IT: specifies the ADC interrupt source to check.
  *          This parameter can be one of the following values:
  *            @arg ADC_IT_EOC: End of conversion interrupt 
  * @retval The new state of ADC_IT (SET or RESET).
  */
ITStatus ADC_GetITStatus(CMSDK_ADC_TypeDef* ADCx, uint32_t ADC_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_GET_IT(ADC_IT));

  /* Get the ADC_IT enable bit status */
  enablestatus = (uint32_t)(ADCx->IER & ADC_IT); 

  /* Check the status of the specified ADC interrupt */
  if (((uint32_t)(ADCx->SR & ADC_IT) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
  {
    /* ADC_IT is set */
    bitstatus = SET;
  }
  else
  {
    /* ADC_IT is reset */
    bitstatus = RESET;
  }
  /* Return the ADC_IT status */
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


