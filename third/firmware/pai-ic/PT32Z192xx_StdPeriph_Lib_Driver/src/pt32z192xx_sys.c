/**
  ******************************************************************************
  * @file    pt32z192xx_sys.c
  * @author  应用开发团队
  * @version 
  * @date    
  * @brief   This file provides firmware functions to manage the following 
  *          
  *          
  *         
  *           
  *          
  *
 @verbatim

 ===============================================================================
                        #####  specific features #####
 ===============================================================================
    [..] After reset the device is running from HSI (24 MHz) with Flash 1 WS, 
         all peripherals are off except internal SRAM, Flash and SWD.
         (#) There is no prescaler on High speed (AHB) and Low speed (APB) busses;
             all peripherals mapped on these busses are running at HSI speed.
         (#) The clock for all peripherals is switched off, except the SRAM and FLASH.
         (#) All GPIOs are in input floating state, except the SWD pins which
             are assigned to be used for debug purpose.
    [..] Once the device started from reset, the user application has to:
         (#) Configure the clock source to be used to drive the System clock
             (if the application needs higher frequency/performance)
         (#) Configure the System clock frequency and Flash settings
         (#) Configure the AHB and APB busses prescalers
         (#) Configure the clock source(s) for peripherals which clocks are not
             derived from the System clock (ADC, I2C, UART,and IWDG)

 @endverbatim
  
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
#include "pt32z192xx_sys.h"

/** @addtogroup PT32F0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup SYS 
  * @brief SYS driver modules
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup SYS_Private_Functions
  * @{
  */

/** @defgroup 
 *  @brief   
 *
@verbatim
 ===============================================================================
 ##### Internal-external clocks, PLL, CSS and MCO configuration functions #####
 ===============================================================================
    [..] This section provides functions allowing to configure the internal/external clocks,
         PLL,and MCO.
         (#) HSI (high-speed internal), 24 MHz factory-trimmed RC used directly 
             or through the PLL as System clock source.
             The HSI clock can be used also to clock the USART, I2C and CEC peripherals.
         (#) LSI (low-speed internal), 32 KHz low consumption RC used as IWDGclock source.
         (#) HSE (high-speed external), 4 to 25 MHz crystal oscillator used directly or
             through the PLL as System clock source. 
         (#) PLL (clocked by HSI or HSE), for System clock.
         (#) CSS (Clock security system), once enabled and if a HSE clock failure occurs 
             (HSE used directly or through PLL as System clock source), the System clock
             is automatically switched to HSI and an interrupt is generated if enabled. 
             The interrupt is linked to the Cortex-M0 NMI (Non-Maskable Interrupt) 
             exception vector.   
         (#) MCO (microcontroller clock output), used to output SYSCLK clock on PA8 pin.

@endverbatim
  * @{
  */
/**
  * @brief  
  *         
  *         
  * @param  None
  * @retval None
  */
void System_GetClocksFreq(System_ClocksTypeDef * System_Clocks)
{
  uint8_t SystemCoreClockDiv;
	uint8_t PeripheralsClockDiv;
  uint8_t SystemCoreClockSrc;
  uint8_t DoubleClockSrc;

  /*   */	
	SystemCoreClockSrc = (CMSDK_SYSCON->SYSCLK & SYSCON_SYSCLK_SYSCLKSEL);
	DoubleClockSrc =  (CMSDK_SYSCON->SYSCLK & SYSCON_SYSCLK_FDCLKSEL) >> 0x02;
	SystemCoreClockDiv = (CMSDK_SYSCON->SYSCLK & SYSCON_SYSCLK_HCLKDIV) >> 0x04;
	PeripheralsClockDiv = (CMSDK_SYSCON->SYSCLK & SYSCON_SYSCLK_PCLKDIV) >> 0x08;

  if (SystemCoreClockSrc==0) /*   */
  {
    System_Clocks->SYSCLK_Frequency = IOSC_RC24M_VALUE >> SystemCoreClockDiv;	
  }
  else if (SystemCoreClockSrc==1) /*  */
  {
    System_Clocks->SYSCLK_Frequency = EOSC_CRYSTAL_VALUE >> SystemCoreClockDiv;
  }
  else if (SystemCoreClockSrc==2) /*  */
  {
		 if(DoubleClockSrc == 0) /*  */
		 {			 
     System_Clocks->SYSCLK_Frequency = (IOSC_RC24M_VALUE*2) >> SystemCoreClockDiv;
		 }		 
		 else if(DoubleClockSrc == 1) /*  */
		 {			 
     System_Clocks->SYSCLK_Frequency = (EOSC_CRYSTAL_VALUE*2) >> SystemCoreClockDiv;
		 }		 
		 else
		 {			 
     System_Clocks->SYSCLK_Frequency = (I0SC_32K_VALUE*2) >> SystemCoreClockDiv;	 
		 }			 
  }		
  else /*  */
  {
    System_Clocks->SYSCLK_Frequency = I0SC_32K_VALUE >> SystemCoreClockDiv;		
  }

	System_Clocks->PCLK_Frequency = System_Clocks->SYSCLK_Frequency >> PeripheralsClockDiv;
  	
	
}
/**
  * @}
  */	




/**
  * @brief  Reset Remap Bit.
  * @param  None
  * @retval None
  */
void System_Retriming(void)
{
  CMSDK_SYSCON->RETRIMING = KEY_RETRIMING;
}



/** @defgroup RSTCU_Private_Function RSTCU private functions
  * @{
  */

/*********************************************************************************************************//**
 * @brief Check whether the specific global reset flag is set or not.
 * @param RSTCU_RSTF: specify the reset flag.
 *        This parameter can be:
 *        @arg RSTCU_FLAG_SYSRST: Get system reset flag
 *        @arg RSTCU_FLAG_EXTRST: Get external pin reset flag
 *        @arg RSTCU_FLAG_WDTRST: Get WDT reset flag
 *        @arg RSTCU_FLAG_PORST : Get power on reset flag
 * @retval SET or RESET
 ************************************************************************************************************/
FlagStatus System_GetResetFlagStatus(RSTCU_RSTF_TypeDef RSTCU_RSTF)
{
  uint32_t tmp;

  /* Check the parameters */
  assert_param(IS_RSTCU_FLAG(RSTCU_RSTF));

  tmp = (CMSDK_SYSCON->RSTINFO & (1UL << RSTCU_RSTF));
  if (tmp != RESET)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/*********************************************************************************************************//**
 * @brief Clear the specific global reset flag.
 * @param RSTCU_RSTF: specify the reset flag.
 *        This parameter can be:
 *        @arg RSTCU_FLAG_SYSRST: Clear system reset flag
 *        @arg RSTCU_FLAG_EXTRST: Clear external pin reset flag
 *        @arg RSTCU_FLAG_WDTRST: Clear WDT reset flag
 *        @arg RSTCU_FLAG_PORST : Clear power on reset flag
 * @retval None
 ************************************************************************************************************/
void System_ClearResetFlag(RSTCU_RSTF_TypeDef RSTCU_RSTF)
{
  /* Check the parameters */
  assert_param(IS_RSTCU_FLAG(RSTCU_RSTF));

  CMSDK_SYSCON->RSTINFO = 1UL << RSTCU_RSTF;  /* Write 1 to clear */
}

/*********************************************************************************************************//**
 * @brief Clear all of the global reset flag.
 * @retval None
 ************************************************************************************************************/
void System_ClearAllResetFlag(void)
{
  CMSDK_SYSCON->RSTINFO = (uint32_t)0xFF ;   /* Write 1 to clear */
}


///**
//  * @brief  Enables or disables the specified Reset Source.

//  * @param  RstSource: specifies the Reset source to be enabled or disabled. 
//  *          This parameter can be one of the following values:
//  *            @arg 
//  *            @arg 
//  *            @arg 
//  *            @arg 
//  * @param  NewState: new state of the specified SPI interrupt.
//  *          This parameter can be: ENABLE or DISABLE.
//  * @retval None
//  */
//void System_RestConfig(uint32_t RstSource, FunctionalState NewState)
//{
//  /* Check the parameters */
//  assert_param(IS_FUNCTIONAL_STATE(NewState));
//  assert_param(IS_RSTEN_SOURCE(RstSource));

//  if (NewState != DISABLE)
//  {
//    /* Enable the selected SPI interrupt */
//    CMSDK_SYSCON->RSTENCTRL |= RstSource;
//  }
//  else
//  {
//    /* Disable the selected SPI interrupt */
//    CMSDK_SYSCON->RSTENCTRL &= (uint16_t)~RstSource;
//  }
//}



///**
//  * @brief  Returns the Remap Status.
//  * @param  None
//  * @retval Remap Status
//  */
//uint32_t System_GetCustomerID(void)
//{
//  return ( CMSDK_SYSCON->CUSTOMER_ID_INFO );
//}


///**
//  * @brief  Returns the Remap Status.
//  * @param  None
//  * @retval Remap Status
//  */
//uint32_t System_GetAgentID(void)
//{
//  return ( CMSDK_SYSCON->AGENT_ID_INFO );
//}


/**
  * @brief  Returns the Remap Status.
  * @param  None
  * @retval Remap Status
  */
uint32_t System_GetUID(uint32_t ID)
{	
  return ( (*(uint32_t*)(CMSDK_SYSCTRL_BASE+0x030+ID)));
}

///**
//  * @brief  Returns the Remap Status.
//  * @param  None
//  * @retval Remap Status
//  */
//uint32_t System_GetCD(void)
//{	
//  return ( CMSDK_SYSCON->CID_INFO );
//}



