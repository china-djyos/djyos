/**
  ******************************************************************************
  * @file    pt32z192xx_pwm.c
  * @author  应用开发团队
  * @version 
  * @date    
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the TIM peripheral:
  *            + TimeBase management
  *            + Output Compare management
  *            + Input Capture management
  *            + Interrupts, flags management
  *            + Clocks management
  *            + Synchronization management
  *            + Specific interface management
  *            + Specific remapping management      
  *              
  *  @verbatim
  
 ===============================================================================
                    ##### How to use this driver #####
 ===============================================================================
    [..] This driver provides functions to configure and program the TIM 
         of all STM32F0xx devices These functions are split in 8 groups: 
         (#) TIM TimeBase management: this group includes all needed functions 
             to configure the TM Timebase unit:
             (++) Set/Get Prescaler.
             (++) Set/Get Autoreload.
             (++) Counter modes configuration.
             (++) Set Clock division.
             (++) Select the One Pulse mode.
             (++) Update Request Configuration.
             (++) Update Disable Configuration.
             (++) Auto-Preload Configuration.
             (++) Enable/Disable the counter.
  
         (#) TIM Output Compare management: this group includes all needed 
             functions to configure the Capture/Compare unit used in Output 
             compare mode: 
             (++) Configure each channel, independently, in Output Compare mode.
             (++) Select the output compare modes.
             (++) Select the Polarities of each channel.
             (++) Set/Get the Capture/Compare register values.
             (++) Select the Output Compare Fast mode. 
             (++) Select the Output Compare Forced mode.  
             (++) Output Compare-Preload Configuration. 
             (++) Clear Output Compare Reference.
             (++) Select the OCREF Clear signal.
             (++) Enable/Disable the Capture/Compare Channels.    
  
         (#) TIM Input Capture management: this group includes all needed 
             functions to configure the Capture/Compare unit used in 
             Input Capture mode:
             (++) Configure each channel in input capture mode.
             (++) Configure Channel1/2 in PWM Input mode.
             (++) Set the Input Capture Prescaler.
             (++) Get the Capture/Compare values.  
             
        (#) Advanced-control timers (TIM1) specific features
            (++) Configures the Break input, dead time, Lock level, the OSSI,
                 the OSSR State and the AOE(automatic output enable)
            (++) Enable/Disable the TIM peripheral Main Outputs
            (++) Select the Commutation event
            (++) Set/Reset the Capture Compare Preload Control bit     
  
         (#) TIM interrupts, DMA and flags management.
             (++) Enable/Disable interrupt sources.
             (++) Get flags status.
             (++) Clear flags/ Pending bits.
             (++) Enable/Disable DMA requests. 
             (++) Configure DMA burst mode.
             (++) Select CaptureCompare DMA request.  
  
         (#) TIM clocks management: this group includes all needed functions 
             to configure the clock controller unit:
             (++) Select internal/External clock.
             (++) Select the external clock mode: ETR(Mode1/Mode2), TIx or ITRx.
  
         (#) TIM synchronization management: this group includes all needed. 
             functions to configure the Synchronization unit:
             (++) Select Input Trigger.  
             (++) Select Output Trigger.  
             (++) Select Master Slave Mode. 
             (++) ETR Configuration when used as external trigger.   
  
         (#) TIM specific interface management, this group includes all 
             needed functions to use the specific TIM interface:
             (++) Encoder Interface Configuration.
             (++) Select Hall Sensor.   
  
         (#) TIM specific remapping management includes the Remapping 
             configuration of specific timers
  
@endverbatim
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
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx_pwm.h"


/** @addtogroup PT32F0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup PWM 
  * @brief PWM driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup PWM_Private_Functions
  * @{
  */

/** @defgroup PWM_Group1 TimeBase management functions
 *  @brief   TimeBase management functions 
 *
@verbatim
 ===============================================================================
                 ##### TimeBase management functions #####
 ===============================================================================
  
        *** TIM Driver: how to use it in Timing(Time base) Mode ***
 ===============================================================================
    [..] To use the Timer in Timing(Time base) mode, the following steps are 
         mandatory:
         (#) Fill the TIM_TimeBaseInitStruct with the desired parameters.
         (#) Call TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct) to configure 
             the Time Base unit with the corresponding configuration.
         (#) Enable the NVIC if you need to generate the update interrupt. 
         (#) Enable the corresponding interrupt using the function 
             TIM_ITConfig(TIMx, TIM_IT_Update). 
         (#) Call the TIM_Cmd(ENABLE) function to enable the TIM counter.
    [..]
        (@) All other functions can be used seperatly to modify, if needed,
            a specific feature of the Timer. 

@endverbatim
  * @{
  */



/**
  * @brief  Initializes the TIMx Time Base Unit peripheral according to 
  *         the specified parameters in the TIM_TimeBaseInitStruct.
  * @param  TIMx: where x can be 1, 2, 3, 6, 7, 14, 15, 16 and 17 to select the TIM 
  *         peripheral.
  * @note   TIM7 is applicable only for STM32F072 devices
  * @note   TIM6 is not applivable for STM32F031 devices.
  * @note   TIM2 is not applicable for STM32F030 devices.  
  * @param  TIM_TimeBaseInitStruct: pointer to a TIM_TimeBaseInitTypeDef
  *         structure that contains the configuration information for
  *         the specified TIM peripheral.
  * @retval None
  */
	
	
	
/**
  * @brief  Initializes the PWMx Time Base Unit peripheral according to 
  *         the specified parameters in the TIM_TimeBaseInitStruct.
  * @param  PWMx: where x can be 0, 1 to select the PWM peripheral
  * @note   
  * @param  TimeBaseInit: pointer to a PWM_TimeBaseInitTypeDef
  *         structure that contains the configuration information for
  *         the specified PWM peripheral.
  * @retval None
  */	
	
void PMW_TimeBaseInit(CMSDK_PWM_TypeDef* PWMx, PWM_TimeBaseInitTypeDef* TimeBaseInit)
{
  uint32_t  tmpreg = 0;
	
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_COUNTER_RELOAD(TimeBaseInit->CounterReload));  
  assert_param(IS_PWM_CLOCKSEL(TimeBaseInit->ClockSel));
  assert_param(IS_PWM_PRESCALER(TimeBaseInit->Prescaler));
  assert_param(IS_PWM_COUNTERMODE(TimeBaseInit->CounterMode));	
	assert_param(IS_PWM_COUNTERDIRRV_EN(TimeBaseInit->CounterDirRv_EN));

  /* Set the counter reload value */
  PWMx->MR0 = TimeBaseInit->CounterReload;

  /* Set the PWM clcok  */
	tmpreg &= ~PWM_TCR_CLKSEL ;
	tmpreg |= TimeBaseInit->ClockSel;
	PWMx->TCR = tmpreg;
	
  /* Set the Prescaler value */
  PWMx->PR = TimeBaseInit->Prescaler;

  /* Select the Counter Mode */
	tmpreg = 0;
	tmpreg &= ~(PWM_MCR_DIR | PWM_MCR_DIR_RV);
	tmpreg |=  (TimeBaseInit->CounterMode | TimeBaseInit->CounterDirRv_EN);
	
  PWMx->MCR = tmpreg;

}

/**
  * @brief  Initializes the PWMx channelx output 
  * @param  PWMx: where x can be 0, 1 to select the PWM peripheral.
  * @note   
  * @param  OutInit: pointer to a PWM_OutputInitTypeDef
  *         structure that contains the configuration information for
  *         the specified PWM peripheral.
  * @retval None
  */	

void PWM_OutputInit(CMSDK_PWM_TypeDef* PWMx, PWM_OutputInitTypeDef* OutInit)
{

  uint32_t ch = 0;
	uint32_t wTmpReg = 0;
	
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_CHCHANNEL(OutInit->Channel));
  assert_param(IS_PWM_OM(OutInit->OutputMode));
  assert_param(IS_PWM_CHNCTL(OutInit->ControlN));
  assert_param(IS_PWM_IDLE(OutInit->IdleState));	
  assert_param(IS_PWM_COMPAREVALUE(OutInit->CompareValue));
  assert_param(IS_PWM_DEATHTIME(OutInit->DeathTime));

	ch = OutInit->Channel;
	
  /* Set the CHx Compare value */
	if(ch==PWM_CH_1)
	PWMx->MR1 = OutInit->CompareValue;
	if(ch==PWM_CH_2)
	PWMx->MR2 = OutInit->CompareValue;
	if(ch==PWM_CH_3)
	PWMx->MR3 = OutInit->CompareValue;
	if(ch==PWM_CH_4)
	PWMx->MR4 = OutInit->CompareValue;
	

  /* Set the CMR Reg */
	wTmpReg = PWMx->CMR;
	wTmpReg &= ~((PWM_CMR_MC1<< ch*4) | (PWM_CMR_PWM1_IDLE<< ch) |  (PWM_CMR_PWM1n_EN<< ch)) ;
	wTmpReg |=  (((OutInit->OutputMode)<< ch*4) | ((OutInit->ControlN)<< ch) | ((OutInit->IdleState)<< ch));  
  PWMx->CMR = wTmpReg;

  /* Set the Death time */
	PWMx->DT = OutInit->DeathTime;

}



/**
  * @brief  Fills each TIM_TimeBaseInitStruct member with its default value.
  * @param  TIM_TimeBaseInitStruct: pointer to a TIM_TimeBaseInitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void PWM_TimeBaseStructInit(PWM_TimeBaseInitTypeDef* TimeBaseInit)
{
  /* Set the default configuration */
  TimeBaseInit->CounterReload = 0xFFFF;
  TimeBaseInit->ClockSel = PCLK;
  TimeBaseInit->Prescaler = 0x0000;
  TimeBaseInit->CounterMode = PWM_CNT_MODE_UP;
  TimeBaseInit->CounterDirRv_EN = PWM_CounterDirRv_DIS;
}

/**
  * @brief  Configures the PWMn Match Reload Times Trigger INT.
  * @param  TIMx: where x can be 0, 1 to select the PWM peripheral.  
  * @param  RepeatTimes: specifies the Match Times to Trigger INT (0~0xF) 
  * @note   
  * @retval None
  */
void PWM_INT_RepeatTimes(CMSDK_PWM_TypeDef* PWMx, uint8_t RepeatTimes)
{

	
  /* Check the parameters */	
  assert_param(IS_PWM_ALL_PERIPH(PWMx)); 
  assert_param(IS_PWM_REPEATTIME(RepeatTimes) );

  /* Set the PWM INT_RepeatTimes */
	PWMx->INT_RPT = RepeatTimes;	
}


/**
  * @brief  Configures the PWMx Clock Source.
  * @param  TIMx: where x can be 0, 1 to select the PWM peripheral.  
  * @param  PWM_ClockSel: specifies the PWMx Clock Source
  *          This parameter can be one of the following values:
  *            @arg PCLK: 
  *            @arg HCLK: 
  * @note   
  * @retval None
  */
void PWM_ClockSourceConfig(CMSDK_PWM_TypeDef* PWMx, uint8_t PWM_ClockSel)
{

  uint32_t  tmpreg = 0;
	
  /* Check the parameters */	
  assert_param(IS_PWM_ALL_PERIPH(PWMx)); 
  assert_param(IS_PWM_CLOCKSEL(PWM_ClockSel));

  /* Set the PWM Clock Source */
	tmpreg = 	PWMx->TCR;
	tmpreg &= ~PWM_TCR_CLKSEL ;
	tmpreg |= PWM_ClockSel;
	PWMx->TCR = tmpreg;	
}

/**
  * @brief  Configures the PWMx Prescaler.
  * @param  PWMx: where x can be 0, 1 to select the PWM peripheral.  
  * @param  Prescaler: specifies the Prescaler Register value (0~255)
  * @note   The Prescaler is loaded at the PC update event or immediatly
  * @retval None
  */
void PWM_PrescalerConfig(CMSDK_PWM_TypeDef* PWMx, uint8_t Prescaler)
{
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_PRESCALER(Prescaler));
  
  /* Set the Prescaler value */
  PWMx->PR = Prescaler;
}


/**
  * @brief  Configures the PWMx Reload value.
  * @param  TIMx: where x can be 0, 1 to select the PWM peripheral.  
  * @param  Reload: specifies the Counter reload value (0~65535)
  * @note   The value is loaded at the update event or immediatly
  * @retval None
  */

void PWM_SetCounterReload(CMSDK_PWM_TypeDef* PWMx, uint16_t Reload)
{
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));	
  assert_param(IS_PWM_COUNTER_RELOAD(Reload));  

  /* Set the counter reload value */
  PWMx->MR0 = Reload;
}

/**
  * @brief  Configures the PWMx Current Value.
  * @param  TIMx: where x can be 0, 1 to select the PWM peripheral.     
  * @param  Counter: specifies the Counter register new value.
  * @note   The Function is valid when PWM no ouput
  * @retval None
  */
void PWM_SetCounter(CMSDK_PWM_TypeDef* PWMx, uint16_t Counter)
{
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));	
  assert_param(IS_PWM_COUNTER_VALUE(Counter));  
   
  /* Set the Counter Register value */
  PWMx->TC = Counter;
}

/**
  * @brief  Gets the PWMx Counter value.
  * @param  TIMx: where x can be 0, 1 to select the PWM peripheral.     
  * @retval Counter Register value.
  */
uint16_t PWM_GetCounter(CMSDK_PWM_TypeDef* PWMx)
{
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  
  /* Get the Counter Register value */
  return  PWMx->TC;
}

/**
  * @brief  Gets the PWMx Current INT RepeatTimes.
  * @param  TIMx: where x can be 0, 1 to select the PWM peripheral.     
  * @retval Current INT RepeatTimes.
  */
uint8_t PWM_GetINT_RepeatTimes(CMSDK_PWM_TypeDef* PWMx)
{
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  
  /* Get the Counter Register value */
  return  PWMx->INT_RPTC;
}


/**
  * @brief  Gets the PWMx Prescaler value.
  * @param  TIMx: where x can be 0, 1 to select the PWM peripheral.    
  * @retval Prescaler Register value.
  */
uint16_t PWM_GetPrescaler(CMSDK_PWM_TypeDef* PWMx)
{
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  
  /* Get the Prescaler Register value */
  return  PWMx->PR;
}

/**
  * @brief  Gets the PWMx Prescaler CNT value.
  * @param  TIMx: where x can be 0, 1 to select the PWM peripheral.    
  * @retval Prescaler Current Register value.
  */
uint16_t PWM_GetPrescalerCnt(CMSDK_PWM_TypeDef* PWMx)
{
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  
  /* Get the Prescaler Register value */
  return  PWMx->PC;
}

/**
  * @brief  Specifies the PWMx Counter Mode to be used.
  * @param  TIMx: where x can be 0, 1 to select the PWM peripheral.
  * @note   
  * @param  CounterMode: specifies the Counter Mode to be used
  *          This parameter can be one of the following values:
  *            @arg PWM_CNT_MODE_UP: TIM Up Counting Mode
  *            @arg PWM_CNT_MODE_DOWN: TIM Down Counting Mode
  * @param  DirRv_EN: specifies the Counter Direction Reverse
  *            @arg PWM_CounterDirRv_EN: TIM Direction Reverse Enable
  *            @arg PWM_CounterDirRv_DIS: TIM Direction Reverse Disable
  * @retval None
  */
void PWM_CounterModeConfig(CMSDK_PWM_TypeDef* PWMx, uint8_t CounterMode, uint8_t DirRv_EN)
{
  uint32_t tmpreg = 0;
  
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_COUNTERMODE(CounterMode));	
	assert_param(IS_PWM_COUNTERDIRRV_EN(DirRv_EN));
  
  /* Select the Counter Mode */
	tmpreg = PWMx->MCR;
	tmpreg &= ~(PWM_MCR_DIR | PWM_MCR_DIR_RV);
	tmpreg |=  (CounterMode | DirRv_EN);
	
  PWMx->MCR = tmpreg;
}

/**
  * @brief  Update settiongs of the PWMx
  * @param  PWMx: where x can be 0, 1 to select the PWM peripheral.  
  * @retval None
  */

void PWM_UpdateCmd(CMSDK_PWM_TypeDef* PWMx)
{
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
    /* Set the update bit */
    PWMx->TCR |= PWM_TCR_UPDATE;
}

/**
  * @brief  Enable or Disable PWMx Counter
  * @param  PWMx: where x can be 0, 1 to select the PWM peripheral.  
  * @param  NewState: This parameter can be ENABLE or DISABLE.
  * @retval None
  */

void PWM_Cmd(CMSDK_PWM_TypeDef* PWMx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));	

  if (NewState == ENABLE)
  {
    /* Set the update bit */
    PWMx->TCR |= PWM_TCR_EN;
  }
  else
  {
    /* Reset the update bit */
    PWMx->TCR &= (~PWM_TCR_EN);
  }
}


void PWM_BreakInput_Cmd(CMSDK_PWM_TypeDef* PWMx, FunctionalState NewState)
{		
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));	
		
  if (NewState != DISABLE)
  {
    /* Enable BreakInput*/
		PWMx->CMR |= PWM_CMR_BKE;
  }
  else
  {
    /* Disable BreakInput*/
		PWMx->CMR &= ~PWM_CMR_BKE;  
  }	
}

void PWM_BKI_LevelConfig(CMSDK_PWM_TypeDef* PWMx, uint32_t BKI_Level)
{
	uint32_t wTmpReg = 0;	
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_BIK_LEVEL(PWM_BKI_HIGHLEVEL));
	
	wTmpReg = PWMx->CMR;
	wTmpReg &= ~PWM_CMR_BKP;
	
	wTmpReg |=BKI_Level;
	
	PWMx->CMR = wTmpReg;
		

}

/**
  * @brief  Specifies the PWMx Counter Mode to be used.
  * @param  PWMx: where x can be 0, 1 to select the PWM peripheral.
  * @param  PWM_INT: Specify the PWM interrupts sources to be enabled or disabled.
  *          This parameter can be one of the following values:
  *        @arg PWM_INT_MR0      : PWM MR0 Reload value match interrupt
  *        @arg PWM_INT_MR1      : PWM CH1 MR1 vaule match interrupt
  *        @arg PWM_INT_MR2      : PWM CH2 MR2 vaule match interrupt
  *        @arg PWM_INT_MR3      : PWM CH3 MR3 vaule match interrupt
  *        @arg PWM_INT_MR4      : PWM CH4 MR4 vaule match interrupt
  *        @arg PWM_INT_CAP1     : PWM CH1 Capture interrupt
  *        @arg PWM_INT_CAP2     : PWM CH2 Capture interrupt
  *        @arg PWM_INT_CAP3     : PWM CH3 Capture interrupt
  *        @arg PWM_INT_CAP4     : PWM CH4 Capture interrupt
  *        @arg PWM_INT_BK       : PWM break interrupt
  * @param  NewState: This parameter can be ENABLE or DISABLE.
  * @retval None
  */

void PWM_IntConfig(CMSDK_PWM_TypeDef* PWMx, uint32_t PWM_INT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_PWM_INT(PWM_INT));	
  assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if((PWM_INT==PWM_INT_MR1) | (PWM_INT==PWM_INT_MR2) | (PWM_INT==PWM_INT_MR3) | (PWM_INT==PWM_INT_MR4) | (PWM_INT==PWM_INT_MR0))
	{
    if (NewState != DISABLE)
    {
      /* Enable the interrupt sources */
      PWMx->MCR |= PWM_INT;
    }
    else
    {
      /* Disable the interrupt sources */
      PWMx->MCR &= ~PWM_INT;
    }
  }
	
	if((PWM_INT==PWM_INT_CAP1) | (PWM_INT==PWM_INT_CAP2) | (PWM_INT==PWM_INT_CAP3) | (PWM_INT==PWM_INT_CAP4))
	{
    if (NewState != DISABLE)
    {
      /* Enable the interrupt sources */
      PWMx->CCR |= PWM_INT;
    }
    else
    {
      /* Disable the interrupt sources */
      PWMx->CCR &= ~PWM_INT;
    }
  }
	
	if(PWM_INT==PWM_INT_BK)
	{
    if (NewState != DISABLE)
    {
      /* Enable the interrupt sources */
      PWMx->CMR |= PWM_INT;
    }
    else
    {
      /* Disable the interrupt sources */
      PWMx->CMR &= ~PWM_INT;
    }
  }	
	
}

/**
  * @brief  Specifies the PWMx Counter Mode to be used.
  * @param  PWMx: where x can be 0, 1 to select the PWM peripheral.
  * @param  PWM_FLAG: Specify the flag to be checked.
  *         This parameter can be one of the following values:
  *         @arg PWM_FLAG_MR0       : PWM MR0 Reload value match flag
  *         @arg PWM_FLAG_MR1       : PWM CH1 MR1 vaule match flag
  *         @arg PWM_FLAG_MR2       : PWM CH2 MR2 vaule match flag
  *         @arg PWM_FLAG_MR3       : PWM CH3 MR3 vaule match flag
  *         @arg PWM_FLAG_MR4       : PWM CH4 MR4 vaule match flag
  *         @arg PWM_FLAG_CAP1_RE   : PWM CH1 Capture Rising  Edge flag
  *         @arg PWM_FLAG_CAP1_FE   : PWM CH1 Capture Falling Edge flag
  *         @arg PWM_FLAG_CAP2_RE   : PWM CH2 Capture Rising  Edge flag
  *         @arg PWM_FLAG_CAP2_FE   : PWM CH2 Capture Falling Edge flag
  *         @arg PWM_FLAG_CAP3_RE   : PWM CH3 Capture Rising  Edge flag
  *         @arg PWM_FLAG_CAP3_FE   : PWM CH3 Capture Falling Edge flag
  *         @arg PWM_FLAG_CAP4_RE   : PWM CH4 Capture Rising  Edge flag
  *         @arg PWM_FLAG_CAP4_FE   : PWM CH4 Capture Falling Edge flag
  *         @arg PWM_FLAG_BK        : PWM break flag
  * @return FlagStatus of PWM_FLAG (SET or RESET).
  */
FlagStatus PWM_GetFlagStatus(CMSDK_PWM_TypeDef* PWMx, uint32_t PWM_FLAG)
{
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_PWM_FLAG(PWM_FLAG));	

  if ((PWMx->ISR & PWM_FLAG) != 0)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/**
  * @brief  Specifies the PWMx Counter Mode to be used.
  * @param  PWMx: where x can be 0, 1 to select the PWM peripheral.
  * @param  PWM_FLAG: Specify the flag to be cleared.
  *         This parameter can be one of the following values:
  *         @arg PWM_FLAG_MR0       : PWM MR0 Reload value match flag
  *         @arg PWM_FLAG_MR1       : PWM CH1 MR1 vaule match flag
  *         @arg PWM_FLAG_MR2       : PWM CH2 MR2 vaule match flag
  *         @arg PWM_FLAG_MR3       : PWM CH3 MR3 vaule match flag
  *         @arg PWM_FLAG_MR4       : PWM CH4 MR4 vaule match flag
  *         @arg PWM_FLAG_CAP1_RE   : PWM CH1 Capture Rising  Edge flag
  *         @arg PWM_FLAG_CAP1_FE   : PWM CH1 Capture Falling Edge flag
  *         @arg PWM_FLAG_CAP2_RE   : PWM CH2 Capture Rising  Edge flag
  *         @arg PWM_FLAG_CAP2_FE   : PWM CH2 Capture Falling Edge flag
  *         @arg PWM_FLAG_CAP3_RE   : PWM CH3 Capture Rising  Edge flag
  *         @arg PWM_FLAG_CAP3_FE   : PWM CH3 Capture Falling Edge flag
  *         @arg PWM_FLAG_CAP4_RE   : PWM CH4 Capture Rising  Edge flag
  *         @arg PWM_FLAG_CAP4_FE   : PWM CH4 Capture Falling Edge flag
  *         @arg PWM_FLAG_BK        : PWM break flag
  * @retval None
  */
void PWM_ClearFlag(CMSDK_PWM_TypeDef* PWMx, uint32_t PWM_FLAG)
{
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_CLEAR_FLAG(PWM_FLAG));	
	
  /* Clear the flags */
  PWMx->ISR = PWM_FLAG;
}


/**
  * @brief  Enable or Disable PWMx CHx RisingEdge Capture
  * @param  PWMx: where x can be 0, 1 to select the PWM peripheral.  
  * @param  CHx:  Specify the Channel to be selected  
  *         This parameter can be one of the following values:
  *         @arg PWM_CH_1       : 
  *         @arg PWM_CH_2       : 
  *         @arg PWM_CH_3       : 
  *         @arg PWM_CH_4       : 
  * @param  NewState: This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void PWM_RE_CaptureConfig(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx, FunctionalState NewState)
{
	
	uint32_t wTmpReg = 0;
	
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_CHCHANNEL(CHx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));	
	
	wTmpReg = PWMx->CCR;
	
	wTmpReg &= ~(PWM_CCR_CAP1RE <<CHx*4);
	
  if (NewState != DISABLE)
  {
    /* Enable RisingEdge Capture*/
		wTmpReg |= (PWM_CCR_CAP1RE <<CHx*4);
  }
  else
  {
    /* Disable RisingEdge Capture*/
		wTmpReg |= 0;    
  }	
	  PWMx->CCR = wTmpReg;
}


/**
  * @brief  Enable or Disable PWMx CHx FallingEdge Capture
  * @param  PWMx: where x can be 0, 1 to select the PWM peripheral.  
  * @param  CHx:  Specify the Channel to be selected  
  *         This parameter can be one of the following values:
  *         @arg PWM_CH_1       : 
  *         @arg PWM_CH_2       : 
  *         @arg PWM_CH_3       : 
  *         @arg PWM_CH_4       : 
  * @param  NewState: This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void PWM_FE_CaptureConfig(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx, FunctionalState NewState)
{
	
	uint32_t wTmpReg = 0;
	
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_CHCHANNEL(CHx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));	
	
	wTmpReg = PWMx->CCR;
	wTmpReg &= ~(PWM_CCR_CAP1FE <<CHx*4);
	
  if (NewState != DISABLE)
  {
    /* Enable FallingEdge Capture*/
		wTmpReg |= (PWM_CCR_CAP1FE <<CHx*4);
  }
  else
  {
    /* Disable FallingEdge Capture*/
		wTmpReg |= 0;    
  }	
	  PWMx->CCR = wTmpReg;
}

/**
  * @brief  Enable or Disable PWMx CHx Capture Rester
  * @param  PWMx: where x can be 0, 1 to select the PWM peripheral.  
  * @param  CHx:  Specify the Channel to be selected  
  *         This parameter can be one of the following values:
  *         @arg PWM_CH_1       : 
  *         @arg PWM_CH_2       : 
  *         @arg PWM_CH_3       : 
  *         @arg PWM_CH_4       : 
  * @param  NewState: This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void PWM_RST_CaptureConfig(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx, FunctionalState NewState)
{
	
	uint32_t wTmpReg = 0;
	
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_CHCHANNEL(CHx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));	
	
	wTmpReg = PWMx->CCR;
	wTmpReg &= ~(PWM_CCR_CAP1RST <<CHx*4);
	
  if (NewState != DISABLE)
  {
    /* Enable Capture Reset*/
		wTmpReg |= (PWM_CCR_CAP1RST <<CHx*4);
  }
  else
  {
    /* Disable Capture Reset*/
		wTmpReg |= 0;    
  }	
	  PWMx->CCR = wTmpReg;
}


/**
  * @brief  Gets the PWMx CHx Capture Counter value.
  * @param  TIMx: where x can be 0, 1 to select the PWM peripheral. 
  * @param  CHx:  Specify the Channel to be selected  
  *         This parameter can be one of the following values:
  *         @arg PWM_CH_1       : 
  *         @arg PWM_CH_2       : 
  *         @arg PWM_CH_3       : 
  *         @arg PWM_CH_4       : 
  * @retval Counter Register value.
  */
uint16_t PWM_GetCaptureCounter(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx)
{
	uint16_t capture_cnt = 0;
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_CHCHANNEL(CHx));
  
  /* Get the Capture Counter Register value */
	if(CHx==PWM_CH_1)
	capture_cnt = PWMx->CR1 ;
	if(CHx==PWM_CH_2)
	capture_cnt = PWMx->CR2 ;
	if(CHx==PWM_CH_3)
	capture_cnt = PWMx->CR3 ;
	if(CHx==PWM_CH_4)
	capture_cnt = PWMx->CR4 ;
		
  return  capture_cnt;
}

/**
  * @brief  Gets the PWMx CHx Capture Edge .
  * @param  TIMx: where x can be 0, 1 to select the PWM peripheral. 
  * @param  CHx:  Specify the Channel to be selected  
  *         This parameter can be one of the following values:
  *         @arg PWM_CH_1       : 
  *         @arg PWM_CH_2       : 
  *         @arg PWM_CH_3       : 
  *         @arg PWM_CH_4       : 
  * @retval  Capture Edge.
  *         This parameter can be one of the following values:
  *         @arg CAPTURE_FE     :  capture falling edge 
  *         @arg CAPTURE_RE     :  capture rising  edge  
  */
uint16_t PWM_GetCaptureEdge(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx)
{
	uint16_t capture_edge = 0;
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_CHCHANNEL(CHx));
  
  /* Get the Capture Counter Register value */
	if(CHx==PWM_CH_1)
	capture_edge = (PWMx->CR1 >> 16);
	if(CHx==PWM_CH_2)
	capture_edge = (PWMx->CR2 >> 16) ;
	if(CHx==PWM_CH_3)
	capture_edge = (PWMx->CR3 >> 16) ;
	if(CHx==PWM_CH_4)
	capture_edge = (PWMx->CR4 >> 16) ;
		
  return  capture_edge;
}


/**
  * @brief  Set the PWMx CHx Output Mode Config
  * @param  PWMx: where x can be 0, 1 to select the PWM peripheral.  
  * @param  CHx:  Specify the Channel to be selected  
  *         This parameter can be one of the following values:
  *         @arg PWM_CH_1        : 
  *         @arg PWM_CH_2        : 
  *         @arg PWM_CH_3        : 
  *         @arg PWM_CH_4        : 
  * @param  Mode: Specify the Channel output mode  
  *         This parameter can be one of the following values:
  *         @arg NO_OUTPUT       :  PWM channel no output
  *         @arg COMPAER_MODE1   :  PWM channel output high when tc <= compare value,output low when tc > compare value 
  *         @arg COMPAER_MODE2   :  PWM channel output low when tc <= compare value,output high when tc > compare value
  *         @arg COMPAER_MODE3   :  PWM channel output reverse when tc match compare value
  * @retval None
  */
void PWM_OD_Config(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx, uint32_t Mode)
{
	
	uint32_t wTmpReg = 0;
	
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_CHCHANNEL(CHx));
  assert_param(IS_PWM_OM(Mode));	
	
	wTmpReg = PWMx->CMR;
	wTmpReg &= ~(PWM_CMR_MC1 <<CHx*4);
	wTmpReg |= (Mode <<CHx*4);
	
	/* Set PWMx CH Output Mode*/

	PWMx->CMR = wTmpReg;
}

/**
  * @brief  Set the PWMx CHx Output idle
  * @param  PWMx: where x can be 0, 1 to select the PWM peripheral.  
  * @param  CHx:  Specify the Channel to be selected  
  *         This parameter can be one of the following values:
  *         @arg PWM_CH_1        : 
  *         @arg PWM_CH_2        : 
  *         @arg PWM_CH_3        : 
  *         @arg PWM_CH_4        : 
  * @param  Mode: Specify the Channel output mode  
  *         This parameter can be one of the following values:
  *         @arg PWM_IDLE_LOW    :  PWM channel idle is low
  *         @arg PWM_IDLE_HIGH   :  PWM channel idle is high
  * @retval None
  */
void PWM_IDLE_Config(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx, uint32_t Idle)
{
	
	uint32_t wTmpReg = 0;
	
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_CHCHANNEL(CHx));
  assert_param(IS_PWM_IDLE(Idle));	
	
	wTmpReg = PWMx->CMR;	
	wTmpReg &= ~(PWM_CMR_PWM1_IDLE <<CHx);
	wTmpReg |= (Idle <<CHx);
	
	/* Set PWMx CH Output Idle*/
	PWMx->CMR = wTmpReg;
}

/**
  * @brief  Enable or Disable the PWMx CHx Complementary Output
  * @param  PWMx: where x can be 0, 1 to select the PWM peripheral.  
  * @param  CHx:  Specify the Channel to be selected  
  *         This parameter can be one of the following values:
  *         @arg PWM_CH_1        : 
  *         @arg PWM_CH_2        : 
  *         @arg PWM_CH_3        : 
  *         @arg PWM_CH_4        : 
  * @param  CHNCTL: Specify the Channel output mode  
  *         This parameter can be one of the following values:
  *         @arg PWM_CHNCTL_DISABLE  :  Disable PWMx CHx Complementary Output
  *         @arg PWM_CHNCTL_ENABLE   :  Enable PWMx CHx Complementary Output
  * @retval None
  */
void PWM_CHN_Config(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx, uint32_t CHNCTL)
{
	
	uint32_t wTmpReg = 0;
	
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_CHCHANNEL(CHx));
  assert_param(IS_PWM_CHNCTL(CHNCTL));	
	
	wTmpReg = PWMx->CMR;	
	wTmpReg &= ~(PWM_CMR_PWM1n_EN <<CHx);
	wTmpReg |= (CHNCTL <<CHx);
	
	/* Set PWMx CH Complementary Output*/
	PWMx->CMR = wTmpReg;
}

/**
  * @brief  Set the PWMx Death Time
  * @param  PWMx: where x can be 0, 1 to select the PWM peripheral.  
  * @param  DeathTime: Specify the Death value (0~0xFF)  
  * @retval None
  */
void PWM_DT_Config(CMSDK_PWM_TypeDef* PWMx, uint8_t DeathTime)
{

	
  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_DEATHTIME(DeathTime));	
	
	
	/* Set PWMx Death Time*/
	PWMx->DT = DeathTime;
}

/**
  * @brief  Set the PWMx CHx Compare Value
  * @param  PWMx: where x can be 0, 1 to select the PWM peripheral.    
  * @param  CHx:  Specify the Channel to be selected  
  *         This parameter can be one of the following values:
  *         @arg PWM_CH_1        : 
  *         @arg PWM_CH_2        : 
  *         @arg PWM_CH_3        : 
  *         @arg PWM_CH_4        : 
  * @param  Cmp: Specify the CHx Output Compare Value value (0~0xFFFF)  
  * @retval None
  */
void PWM_SetCompare(CMSDK_PWM_TypeDef* PWMx, uint8_t CHx, uint16_t Cmp)
{

  /* Check the parameters */
  assert_param(IS_PWM_ALL_PERIPH(PWMx));
  assert_param(IS_PWM_CHCHANNEL(CHx));	
  assert_param(IS_PWM_COMPAREVALUE(Cmp));	
	
	/* Set PWM CH Output Compare Value*/
	if(CHx==PWM_CH_1)
	PWMx->MR1 = Cmp;
	if(CHx==PWM_CH_2)
	PWMx->MR2 = Cmp;
	if(CHx==PWM_CH_3)
	PWMx->MR3 = Cmp;
	if(CHx==PWM_CH_4)
	PWMx->MR4 = Cmp;
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


