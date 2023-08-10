/**
  ******************************************************************************
  * @file    pt32z192xx_spi.c
  * @author  应用开发团队
  * @version 
  * @date    
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Serial peripheral interface (SPI):
  *           + Initialization and Configuration
  *           + Data transfers functions
  *           + Interrupts and flags management
  *
  *  @verbatim

 ===============================================================================
                       ##### How to use this driver #####
 ===============================================================================
    [..]  
        (#) Peripherals alternate function: 
            (++) Connect the pin to the desired peripherals' Alternate 
                 Function (AF) using GPIO_PinAFConfig() function.
            (++) Configure the desired pin in alternate function by:
                 GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AF.
            (++) Select the type, pull-up/pull-down and output speed via 
                 GPIO_PuPd, GPIO_OType and GPIO_Speed members.
            (++) Call GPIO_Init() function.
  
        (#) Program the Polarity, Phase, First Data, Baud Rate Prescaler, Slave 
            Management, Peripheral Mode and CRC Polynomial values using the SPI_Init()
            function.In I2S mode, program the Mode, Standard, Data Format, MCLK 
            Output, Audio frequency and Polarity using I2S_Init() function.
            
        (#) Enable the NVIC and the corresponding interrupt using the function 
            SPI_ITConfig() if you need to use interrupt mode. 
   
        (#) Enable the SPI using the SPI_Cmd() function or enable the I2S using
            I2S_Cmd().
  
        (#) Optionally, you can enable/configure the following parameters without
            re-initialization (i.e there is no need to call again SPI_Init() function):
            (++) When bidirectional mode (SPI_Direction_1Line_Rx or SPI_Direction_1Line_Tx)
                 is programmed as Data direction parameter using the SPI_Init() 
                 function it can be possible to switch between SPI_Direction_Tx 
                 or SPI_Direction_Rx using the SPI_BiDirectionalLineConfig() function.
            (++) When SPI_NSS_Soft is selected as Slave Select Management parameter 
                 using the SPI_Init() function it can be possible to manage the 
                 NSS internal signal using the SPI_NSSInternalSoftwareConfig() function.
            (++) Reconfigure the data size using the SPI_DataSizeConfig() function.
            (++) Enable or disable the SS output using the SPI_SSOutputCmd() function.  
  
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
#include "pt32z192xx_spi.h"

/** @addtogroup PT32F0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup SPI
  * @brief SPI driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* SPI registers Masks */
#define CR0_CLEAR_MASK       ((uint16_t)0x0030)
#define CR1_CLEAR_MASK2      ((uint16_t)0xFFFB)
#define CR2_LDMA_MASK        ((uint16_t)0x9FFF)

#define I2SCFGR_CLEAR_Mask   ((uint16_t)0xF040)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup SPI_Private_Functions
  * @{
  */

/** @defgroup SPI_Group1 Initialization and Configuration functions
 *  @brief   Initialization and Configuration functions 
 *
@verbatim   
 ===============================================================================
           ##### Initialization and Configuration functions #####
 ===============================================================================
    [..] This section provides a set of functions allowing to initialize the SPI Direction,
         SPI Mode, SPI Data Size, SPI Polarity, SPI Phase, SPI NSS Management, SPI Baud
         Rate Prescaler, SPI First Bit and SPI CRC Polynomial.

    [..] The SPI_Init() function follows the SPI configuration procedures for Master mode
         and Slave mode (details for these procedures are available in reference manual).
         
    [..] When the Software NSS management (SPI_InitStruct->SPI_NSS = SPI_NSS_Soft) is selected,
         use the following function to manage the NSS bit:
         void SPI_NSSInternalSoftwareConfig(CMSDK_SPI_TypeDef* SPIx, uint16_t SPI_NSSInternalSoft);

    [..] In Master mode, when the Hardware NSS management (SPI_InitStruct->SPI_NSS = SPI_NSS_Hard)
         is selected, use the follwoing function to enable the NSS output feature.
         void SPI_SSOutputCmd(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState);

    [..] The NSS pulse mode can be managed by the SPI TI mode when enabling it using the following function:
         void SPI_TIModeCmd(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState);
         And it can be managed by software in the SPI Motorola mode using this function: 
         void SPI_NSSPulseModeCmd(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState);

    [..] This section provides also functions to initialize the I2S Mode, Standard, 
         Data Format, MCLK Output, Audio frequency and Polarity.
  
    [..] The I2S_Init() function follows the I2S configuration procedures for Master mode
         and Slave mode.
  
@endverbatim
  * @{
  */

/**
  * @brief  Fills each SPI_InitStruct member with its default value.
  * @param  SPI_InitStruct: pointer to a SPI_InitTypeDef structure which will be initialized.
  * @retval None
  */
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct)
{
/*--------------- Reset SPI init structure parameters values -----------------*/
  /* Initialize the SPI_Mode member */
  SPI_InitStruct->SPI_Mode = SPI_Mode_Slave;
  /* Initialize the SPI_DataSize member */
  SPI_InitStruct->SPI_DataSize = SPI_DataSize_8b;
  /* Initialize the SPI_CPOL member */
  SPI_InitStruct->SPI_CPOL = SPI_CPOL_Low;
  /* Initialize the SPI_CPHA member */
  SPI_InitStruct->SPI_CPHA = SPI_CPHA_1Edge;
  /* Initialize the SPI_NSS member */
  SPI_InitStruct->SPI_NSS = SPI_NSS_Hard;
  /* Initialize the SPI_BaudRatePrescaler member */
  SPI_InitStruct->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  /* Initialize the SPI_BaudRatePrescaler member */	
	SPI_InitStruct->SPI_BaudRatePostPrescaler = SPI_BaudRatePostPrescaler_1;
}

/**
  * @brief  Initializes the SPIx peripheral according to the specified 
  *         parameters in the SPI_InitStruct.
  * @param  SPIx: where x can be 1 or 2 to select the SPI peripheral.
  * @note   SPI2 is not available for STM32F031 devices. 
  * @param  SPI_InitStruct: pointer to a SPI_InitTypeDef structure that
  *         contains the configuration information for the specified SPI peripheral.
  * @retval None
  */
void SPI_Init(CMSDK_SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)
{
  uint16_t tmpreg = 0;

  /* check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));

  /* Check the SPI parameters */
  assert_param(IS_SPI_MODE(SPI_InitStruct->SPI_Mode));
  assert_param(IS_SPI_DATA_SIZE(SPI_InitStruct->SPI_DataSize));
  assert_param(IS_SPI_CPOL(SPI_InitStruct->SPI_CPOL));
  assert_param(IS_SPI_CPHA(SPI_InitStruct->SPI_CPHA));
  assert_param(IS_SPI_NSS(SPI_InitStruct->SPI_NSS));
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_InitStruct->SPI_BaudRatePrescaler));
	assert_param(IS_SPI_BAUDRATE_POSTPRESCALER(SPI_InitStruct->SPI_BaudRatePostPrescaler));

  /*---------------------------- SPIx CR1 Configuration ------------------------*/
  /* Get the SPIx CR0 value */
  tmpreg = SPIx->CR0;
  /* Clear DSS,CPOL,CPHA and post BaudRatePrescaler bits */
  tmpreg &= CR0_CLEAR_MASK;
  /* Configure SPIx: direction, NSS management, first transmitted bit, BaudRate prescaler
  master/slave mode, CPOL and CPHA */
  /* Set BIDImode, BIDIOE and RxONLY bits according to SPI_Direction value */
  /* Set SSM, SSI bit according to SPI_NSS values */
  /* Set LSBFirst bit according to SPI_FirstBit value */
  /* Set BR bits according to SPI_BaudRatePrescaler value */
  /* Set CPOL bit according to SPI_CPOL value */
  /* Set CPHA bit according to SPI_CPHA value */
  tmpreg |= (uint16_t)((uint32_t)SPI_InitStruct->SPI_DataSize | SPI_InitStruct->SPI_CPOL |
                      SPI_InitStruct->SPI_CPHA | ((SPI_InitStruct->SPI_BaudRatePostPrescaler)<<8));  
  /* Write to SPIx CR1 */
  SPIx->CR0 = tmpreg;
  /*-------------------------SPI Mode Configuration -----------------------*/
  /* Clear MS bits */
  SPIx->CR1 &=(uint16_t)~SPI_CR1_MSTR;
  /* Configure SPIx: Mode */
  SPIx->CR1 |= (uint16_t)(SPI_InitStruct->SPI_Mode); 
  /*---------------------------- SPIx CPSR Configuration --------------------*/
  /* Write to SPIx Prescaler */
  SPIx->CPSR = SPI_InitStruct->SPI_BaudRatePrescaler;
	
	tmpreg = SPIx->CSCR;
	tmpreg &= ~SPI_ICR_SWSEL;
	tmpreg |= SPI_InitStruct->SPI_NSS;
	SPIx->CSCR = tmpreg;
  
}


/**
  * @brief  Enables or disables the specified SPI peripheral.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  NewState: new state of the SPIx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_Cmd(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected SPI peripheral */
    SPIx->CR1 |= SPI_CR1_SPIEN;
  }
  else
  {
    /* Disable the selected SPI peripheral */
    SPIx->CR1 &= (uint16_t)~((uint16_t)SPI_CR1_SPIEN);
  }
}

/**
  * @brief  Enables or disables the SPI Wrap-around mode.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  NewState: new state of the SPIx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_LOOPBACK(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState)
{
	/* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
	 if (NewState != DISABLE)
  {
    /* Enable the selected SPI peripheral */
    SPIx->CR1 |= SPI_CR1_LOOPBACK;
  }
  else
  {
    /* Disable the selected SPI peripheral */
    SPIx->CR1 &= (uint16_t)~((uint16_t)SPI_CR1_LOOPBACK);
  }
}


/**
  * @brief  Enables or disables the TI Mode.
  *   
  * @note   This function can be called only after the SPI_Init() function has 
  *         been called. 
  * @note   When TI mode is selected, the control bits SSM, SSI, CPOL and CPHA 
  *         are not taken into consideration and are configured by hardware 
  *         respectively to the TI mode requirements.
  *    
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  NewState: new state of the selected SPI TI communication mode.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_TIModeCmd(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the TI mode for the selected SPI peripheral */
		SPIx->CR0 &= (uint16_t)~((uint16_t)SPI_CR0_FRF);
    SPIx->CR0 |= SPI_CR0_FRF_TI;
  }
  else
  {
    /* Disable the TI mode for the selected SPI peripheral */
    SPIx->CR0 &= (uint16_t)~((uint16_t)SPI_CR0_FRF);
  }
}

/**
  * @brief  Enables or disables the National Microwire Mode.
  *   
  * @note   This function can be called only after the SPI_Init() function has 
  *         been called. 
  * @note   When Microwire mode is selected, the control bits SSM, SSI, CPOL and CPHA 
  *         are not taken into consideration and are configured by hardware 
  *         respectively to the Microwire mode requirements.
  *    
  * @param  SPIx: where x can be 1 or 2 to select the SPI peripheral.
  * @note   SPI2 is not available for STM32F031 devices.
  * @param  NewState: new state of the selected SPI TI communication mode.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_MicroWireModeCmd(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the TI mode for the selected SPI peripheral */
		SPIx->CR0 &= (uint16_t)~((uint16_t)SPI_CR0_FRF);
    SPIx->CR0 |= SPI_CR0_FRF_MICROWIRE;
  }
  else
  {
    /* Disable the TI mode for the selected SPI peripheral */
    SPIx->CR0 &= (uint16_t)~((uint16_t)SPI_CR0_FRF);
  }
}

/**
  * @brief  Configures the data size for the selected SPI.
  * @param  SPIx: where x can be 0 or 1  to select the SPI peripheral.
  * @param  SPI_DataSize: specifies the SPI data size.
  *         For the SPIx peripheral this parameter can be one of the following values:
  *            @arg SPI_DataSize_4b: Set data size to 4 bits
  *            @arg SPI_DataSize_5b: Set data size to 5 bits
  *            @arg SPI_DataSize_6b: Set data size to 6 bits
  *            @arg SPI_DataSize_7b: Set data size to 7 bits
  *            @arg SPI_DataSize_8b: Set data size to 8 bits
  *            @arg SPI_DataSize_9b: Set data size to 9 bits
  *            @arg SPI_DataSize_10b: Set data size to 10 bits
  *            @arg SPI_DataSize_11b: Set data size to 11 bits
  *            @arg SPI_DataSize_12b: Set data size to 12 bits
  *            @arg SPI_DataSize_13b: Set data size to 13 bits
  *            @arg SPI_DataSize_14b: Set data size to 14 bits
  *            @arg SPI_DataSize_15b: Set data size to 15 bits
  *            @arg SPI_DataSize_16b: Set data size to 16 bits
  * @retval None
  */
void SPI_DataSizeConfig(CMSDK_SPI_TypeDef* SPIx, uint16_t SPI_DataSize)
{
  uint16_t tmpreg = 0;
  
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_DATA_SIZE(SPI_DataSize));
  /* Read the CR2 register */
  tmpreg = SPIx->CR0;
  /* Clear DS[3:0] bits */
  tmpreg &= (uint16_t)~SPI_CR0_DSS;
  /* Set new DS[3:0] bits value */
  tmpreg |= SPI_DataSize;
  SPIx->CR0 = tmpreg;
}




/**
  * @brief  Configures internally by software the NSS pin for the selected SPI.
  * @note   This function can be called only after the SPI_Init() function has 
  *         been called.  
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  SPI_NSSInternalSoft: specifies the SPI NSS internal state.
  *          This parameter can be one of the following values:
  *            @arg SPI_NSSInternalSoft_Set: Set NSS pin internally
  *            @arg SPI_NSSInternalSoft_Reset: Reset NSS pin internally
  * @retval None
  */
void SPI_NSSInternalSoftwareConfig(CMSDK_SPI_TypeDef* SPIx, uint16_t SPI_NSSInternalSoft)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_NSS_INTERNAL(SPI_NSSInternalSoft));

  if (SPI_NSSInternalSoft != SPI_NSSInternalSoft_Reset)
  {
    /* Set NSS pin internally by software */
    SPIx->CSCR |= SPI_NSSInternalSoft_Set;
  }
  else
  {
    /* Reset NSS pin internally by software */
    SPIx->CSCR &= SPI_NSSInternalSoft_Reset;
  }
}



/**
  * @}
  */

/** @defgroup SPI_Group2 Data transfers functions
 *  @brief   Data transfers functions
 *
@verbatim
 ===============================================================================
                    ##### Data transfers functions #####
 ===============================================================================
    [..] This section provides a set of functions allowing to manage the SPI
         data transfers.

    [..] In reception, data are received and then stored into an internal Rx buffer while 
         In transmission, data are first stored into an internal Tx buffer before being 
         transmitted.

    [..] The read access of the SPI_DR register can be done using 
         SPI_ReceiveData8() (when data size is equal or inferior than 8bits) and.
         and returns the Rx buffered value. Whereas a write access to the SPI_DR 
         can be done using SPI_SendData8() (when data size is equal or inferior than 8bits)
         and stores the written data into Tx buffer.

@endverbatim
  * @{
  */

/**
  * @brief  Transmits a Data through the SPIx/I2Sx peripheral.
  * @param  SPIx: where x can be 1 or 2 in SPI mode to select the SPI peripheral.
  * @param  Data: Data to be transmitted.
  * @retval None
  */
void SPI_SendData8(CMSDK_SPI_TypeDef* SPIx, uint8_t Data)
{
  uint32_t spixbase = 0x00;

  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));

  spixbase = (uint32_t)SPIx; 
  spixbase += 0x08;
  
  *(__IO uint8_t *) spixbase = Data;
}



/**
  * @brief  Returns the most recent received data by the SPIx/I2Sx peripheral. 
  * @param  SPIx: where x can be 1 or 2 in SPI mode to select the SPI peripheral. 
  * @retval The value of the received data.
  */
uint8_t SPI_ReceiveData8(CMSDK_SPI_TypeDef* SPIx)
{
  uint32_t spixbase = 0x00;
  
  spixbase = (uint32_t)SPIx; 
  spixbase += 0x08;
  
  return *(__IO uint8_t *) spixbase;
}

/**
  * @brief  Transmits a Data through the SPIx/I2Sx peripheral.
  * @param  SPIx: where x can be
  *   - 1, 2 or 3 in SPI mode 
  *   - 2 or 3 in I2S mode
  * @param  Data : Data to be transmitted.
  * @retval None
  */
void SPI_SendData(CMSDK_SPI_TypeDef* SPIx, uint16_t Data)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* Write in the DR register the data to be sent */
  SPIx->DR = Data;
}

/**
  * @brief  Returns the most recent received data by the SPIx/I2Sx peripheral. 
  * @param  SPIx: where x can be
  *   - 1, 2 or 3 in SPI mode 
  *   - 2 or 3 in I2S mode
  * @retval The value of the received data.
  */
uint16_t SPI_ReceiveData(CMSDK_SPI_TypeDef* SPIx)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* Return the data in the DR register */
  return SPIx->DR;
}





void SPI_ITConfig(CMSDK_SPI_TypeDef* SPIx, uint8_t SPI_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_SPI_CONFIG_IT(SPI_IT));

  if (NewState != DISABLE)
  {

    SPIx->IE |= SPI_IT;
  }
  else
  {

    SPIx->IE &= (uint16_t)~SPI_IT;
  }
}




FlagStatus SPI_GetFlagStatus(CMSDK_SPI_TypeDef* SPIx, uint16_t SPI_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_GET_FLAG(SPI_FLAG));


  if ((SPIx->SR & SPI_FLAG) != (uint16_t)RESET)
  {

    bitstatus = SET;
  }
  else
  {

    bitstatus = RESET;
  }

  return  bitstatus;
}


void SPI_ClearFlag(CMSDK_SPI_TypeDef* SPIx, uint16_t SPI_FLAG)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_CLEAR_FLAG(SPI_FLAG));


  SPIx->ICR = SPI_FLAG;
}


ITStatus SPI_GetITStatus(CMSDK_SPI_TypeDef* SPIx, uint32_t SPI_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;

  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_GET_IT(SPI_IT));


  enablestatus = (uint32_t)(SPIx->IE & SPI_IT); 


  if (((uint32_t)(SPIx->EIS & SPI_IT) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
  {

    bitstatus = SET;
  }
  else
  {

    bitstatus = RESET;
  }

  return  bitstatus;
}

void SPI_Rx_DMA(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState)
{
	 /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    SPIx->DMACR |= SPI_DMA_Rx;
  }
  else
  {

    SPIx->DMACR &= (uint16_t)~SPI_DMA_Rx;
  }
}

void SPI_Tx_DMA(CMSDK_SPI_TypeDef* SPIx, FunctionalState NewState)
{
	 /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {

    SPIx->DMACR |= SPI_DMA_Tx;
  }
  else
  {

    SPIx->DMACR &= (uint16_t)~SPI_DMA_Tx;
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

/**
  * @}
  */

/************************ (C)  *****END OF FILE****/
