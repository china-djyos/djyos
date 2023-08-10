/**
  ******************************************************************************
  * @file    pt32z192xx_QSPI.c
  * @author  应用开发团队
  * @version 
  * @date    
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Serial peripheral interface (QSPI):
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
            Management, Peripheral Mode and CRC Polynomial values using the QSPI_Init()
            function.In I2S mode, program the Mode, Standard, Data Format, MCLK 
            Output, Audio frequency and Polarity using I2S_Init() function.
            
        (#) Enable the NVIC and the corresponding interrupt using the function 
            QSPI_ITConfig() if you need to use interrupt mode. 
   
        (#) Enable the QSPI using the QSPI_Cmd() function or enable the I2S using
            I2S_Cmd().
  
        (#) Optionally, you can enable/configure the following parameters without
            re-initialization (i.e there is no need to call again QSPI_Init() function):
            (++) When bidirectional mode (QSPI_Direction_1Line_Rx or QSPI_Direction_1Line_Tx)
                 is programmed as Data direction parameter using the QSPI_Init() 
                 function it can be possible to switch between QSPI_Direction_Tx 
                 or QSPI_Direction_Rx using the QSPI_BiDirectionalLineConfig() function.
            (++) When QSPI_NSS_Soft is selected as Slave Select Management parameter 
                 using the QSPI_Init() function it can be possible to manage the 
                 NSS internal signal using the QSPI_NSSInternalSoftwareConfig() function.
            (++) Reconfigure the data size using the QSPI_DataSizeConfig() function.
            (++) Enable or disable the SS output using the QSPI_SSOutputCmd() function.  
  
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
#include "pt32z192xx_qspi.h"

/** @addtogroup PT32F0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup QSPI
  * @brief QSPI driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* QSPI registers Masks */
#define CTRLR0_CLEAR_MASK    ((uint16_t)0x0FFF)


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup QSPI_Private_Functions
  * @{
  */

/** @defgroup QSPI_Group1 Initialization and Configuration functions
 *  @brief   Initialization and Configuration functions 
 *
@verbatim   
 ===============================================================================
           ##### Initialization and Configuration functions #####
 ===============================================================================
    [..] This section provides a set of functions allowing to initialize the QSPI Direction,
         QSPI Mode, QSPI Data Size, QSPI Polarity, QSPI Phase, QSPI NSS Management, QSPI Baud
         Rate Prescaler, QSPI First Bit and QSPI CRC Polynomial.

    [..] The QSPI_Init() function follows the QSPI configuration procedures for Master mode
         and Slave mode (details for these procedures are available in reference manual).
         
    [..] When the Software NSS management (QSPI_InitStruct->QSPI_NSS = QSPI_NSS_Soft) is selected,
         use the following function to manage the NSS bit:
         void QSPI_NSSInternalSoftwareConfig(CMSDK_QSPI_TypeDef* QSPIx, uint16_t QSPI_NSSInternalSoft);

    [..] In Master mode, when the Hardware NSS management (QSPI_InitStruct->QSPI_NSS = QSPI_NSS_Hard)
         is selected, use the follwoing function to enable the NSS output feature.
         void QSPI_SSOutputCmd(CMSDK_QSPI_TypeDef* QSPIx, FunctionalState NewState);

    [..] The NSS pulse mode can be managed by the QSPI TI mode when enabling it using the following function:
         void QSPI_TIModeCmd(CMSDK_QSPI_TypeDef* QSPIx, FunctionalState NewState);
         And it can be managed by software in the QSPI Motorola mode using this function: 
         void QSPI_NSSPulseModeCmd(CMSDK_QSPI_TypeDef* QSPIx, FunctionalState NewState);

    [..] This section provides also functions to initialize the I2S Mode, Standard, 
         Data Format, MCLK Output, Audio frequency and Polarity.
  
    [..] The I2S_Init() function follows the I2S configuration procedures for Master mode
         and Slave mode.
  
@endverbatim
  * @{
  */

/**
  * @brief  Fills each QSPI_InitStruct member with its default value.
  * @param  QSPI_InitStruct: pointer to a QSPI_InitTypeDef structure which will be initialized.
  * @retval None
  */
void QSPI_StructInit(QSPI_InitTypeDef* QSPI_InitStruct)
{
/*--------------- Reset QSPI init structure parameters values -----------------*/
  /* Initialize the QSPI_Directionr member*/
  QSPI_InitStruct->QSPI_Mode = QSPI_Direction_TXRX;
  /* Initialize the QSPI_Mode member */
  QSPI_InitStruct->QSPI_Mode = QSPI_Mode_SPI;
  /* Initialize the QSPI_DataSize member */
  QSPI_InitStruct->QSPI_DataSize = QSPI_DataSize_8b;
  /* Initialize the QSPI_CPOL member */
  QSPI_InitStruct->QSPI_CPOL = QSPI_CPOL_Low;
  /* Initialize the QSPI_CPHA member */
  QSPI_InitStruct->QSPI_CPHA = QSPI_CPHA_1Edge;
  /* Initialize the QSPI_BaudRatePrescaler member */
  QSPI_InitStruct->QSPI_BaudRatePrescaler = 0x12;

}

/**
  * @brief  Initializes the QSPIx peripheral according to the specified 
  *         parameters in the QSPI_InitStruct.
  * @param  QSPIx: where x can be 1 or 2 to select the QSPI peripheral.
  * @note   QSPI2 is not available for STM32F031 devices. 
  * @param  QSPI_InitStruct: pointer to a QSPI_InitTypeDef structure that
  *         contains the configuration information for the specified QSPI peripheral.
  * @retval None
  */
void QSPI_Init(QSPI_InitTypeDef* QSPI_InitStruct)
{
  uint16_t tmpreg = 0;


  /* Check the QSPI parameters */
  assert_param(IS_QSPI_DIRECTION(QSPI_InitStruct->QSPI_Direction));	
  assert_param(IS_QSPI_MODE(QSPI_InitStruct->QSPI_Mode));
  assert_param(IS_QSPI_DATA_SIZE(QSPI_InitStruct->QSPI_DataSize));
  assert_param(IS_QSPI_CPOL(QSPI_InitStruct->QSPI_CPOL));
  assert_param(IS_QSPI_CPHA(QSPI_InitStruct->QSPI_CPHA));
  assert_param(IS_QSPI_BAUDRATE_PRESCALER(QSPI_InitStruct->QSPI_BaudRatePrescaler));


  /*---------------------------- QSPI CTRLR0 Configuration ------------------------*/
  /* Get the QSPIx CR0 value */
  tmpreg = QSPI->CTRLR0;
  /* Clear DSS,CPOL,CPHA and post BaudRatePrescaler bits */
  tmpreg &= ~CTRLR0_CLEAR_MASK;
  /* Configure QSPIx: direction, NSS management, first transmitted bit, BaudRate prescaler
  master/slave mode, CPOL and CPHA */
  /* Set BIDImode, BIDIOE and RxONLY bits according to QSPI_Direction value */
  /* Set LSBFirst bit according to QSPI_FirstBit value */
  /* Set BR bits according to QSPI_BaudRatePrescaler value */
  /* Set CPOL bit according to QSPI_CPOL value */
  /* Set CPHA bit according to QSPI_CPHA value */
  tmpreg |= (uint16_t)((uint32_t)QSPI_InitStruct->QSPI_DataSize | QSPI_InitStruct->QSPI_CPOL |
                      QSPI_InitStruct->QSPI_CPHA | QSPI_InitStruct->QSPI_Direction | QSPI_InitStruct->QSPI_Mode );  
 
 
  QSPI->CTRLR0 = tmpreg;
  /*---------------------------- QSPI BAUDR Configuration --------------------*/
  /* Write to QSPIx Prescaler */
  QSPI->BAUDR = QSPI_InitStruct->QSPI_BaudRatePrescaler;
	

}


/**
  * @brief  Enables or disables the specified QSPI peripheral.
  * @param  QSPIx: where x can be 0 or 1 to select the QSPI peripheral.
  * @param  NewState: new state of the QSPIx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void QSPI_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected QSPI peripheral */
    QSPI->ENR = QSPI_EN;
  }
  else
  {
    /* Disable the selected QSPI peripheral */
    QSPI->ENR = 0;
  }
}

/**
  * @brief  Selcte QSPI Mode.
  *   
  * @note   This function can be called only after the QSPI_Init() function has 
  *         been called. 
  *    
  * @param  Mode_Select: select QSPI communication mode.
  *          This parameter can be: QSPI_Mode_SPI
  *                                 QSPI_Mode_Microwire             
  * @retval None
  */
void QSPI_ModeSel(uint16_t Mode_Select)
{
  /* Check the parameters */
  assert_param(IS_QSPI_MODE(Mode_Select));

	QSPI->CTRLR0 &= (uint32_t)~((uint32_t)QSPI_CTRLR0_FRF);	
	
  if (Mode_Select == QSPI_Mode_SPI)
  {
    /* Select SPI Mode */
    QSPI->CTRLR0 |= QSPI_CTRLR0_FRF_SPI;
  }
  else 
  {
    /* Select Microwire Mode */		
    QSPI->CTRLR0 |= QSPI_Mode_Microwire;
  }
}


/**
  * @brief  Configures the data size for the selected QSPI.
  * @param  QSPI_DataSize: specifies the QSPI data size.
  *         For the QSPIx peripheral this parameter can be one of the following values:
  *            @arg QSPI_DataSize_4b: Set data size to 4 bits
  *            @arg QSPI_DataSize_5b: Set data size to 5 bits
  *            @arg QSPI_DataSize_6b: Set data size to 6 bits
  *            @arg QSPI_DataSize_7b: Set data size to 7 bits
  *            @arg QSPI_DataSize_8b: Set data size to 8 bits
  *            @arg QSPI_DataSize_9b: Set data size to 9 bits
  *            @arg QSPI_DataSize_10b: Set data size to 10 bits
  *            @arg QSPI_DataSize_11b: Set data size to 11 bits
  *            @arg QSPI_DataSize_12b: Set data size to 12 bits
  *            @arg QSPI_DataSize_13b: Set data size to 13 bits
  *            @arg QSPI_DataSize_14b: Set data size to 14 bits
  *            @arg QSPI_DataSize_15b: Set data size to 15 bits
  *            @arg QSPI_DataSize_16b: Set data size to 16 bits
  *            @arg QSPI_DataSize_17b: Set data size to 17 bits
  *            @arg QSPI_DataSize_18b: Set data size to 18 bits
  *            @arg QSPI_DataSize_19b: Set data size to 19 bits
  *            @arg QSPI_DataSize_20b: Set data size to 20 bits
  *            @arg QSPI_DataSize_21b: Set data size to 21 bits
  *            @arg QSPI_DataSize_22b: Set data size to 22 bits
  *            @arg QSPI_DataSize_23b: Set data size to 23 bits
  *            @arg QSPI_DataSize_24b: Set data size to 24 bits
  *            @arg QSPI_DataSize_25b: Set data size to 25 bits
  *            @arg QSPI_DataSize_26b: Set data size to 26 bits
  *            @arg QSPI_DataSize_27b: Set data size to 27 bits
  *            @arg QSPI_DataSize_28b: Set data size to 28 bits
  *            @arg QSPI_DataSize_29b: Set data size to 29 bits
  *            @arg QSPI_DataSize_30b: Set data size to 30 bits
  *            @arg QSPI_DataSize_31b: Set data size to 31 bits
  *            @arg QSPI_DataSize_32b: Set data size to 32 bits	
  * @retval None
  */
void QSPI_DataSizeConfig(uint16_t QSPI_DataSize)
{
  
  /* Check the parameters */
  assert_param(IS_QSPI_DATA_SIZE(QSPI_DataSize));
	
	QSPI->CTRLR0 &= (uint32_t)~((uint32_t)QSPI_CTRLR0_DFS);	
	
  /* Set new DS[4:0] bits value */
  QSPI->CTRLR0 |= QSPI_DataSize;
}


/**
  * @brief  Selcte QSPI Transmit Mode.
  *   
  * @note   This function can be called only after the QSPI_Init() function has 
  *         been called. 
  * @param  Transmit_Direction: select QSPI Direction.
  *         This parameter can be:  QSPI_Direction_TXRX
  *                                 QSPI_Direction_TX             
  *                                 QSPI_Direction_RX             
  *                                 QSPI_Direction_EPPROMREAD             
  * @retval None
  */
void QSPI_DirectionConfig(uint16_t Transmit_Direction)
{
  /* Check the parameters */
  assert_param(IS_QSPI_DIRECTION(Transmit_Direction));

	QSPI->CTRLR0 &= (uint32_t)~((uint32_t)QSPI_CTRLR0_TMOD);	
	
  QSPI->CTRLR0 |= Transmit_Direction;
}

/**
  * @brief  QSPI Loopback Mode Enable.
  * @param  Enable or Disable QSPI Loopback Mode.
  * @param  NewState: new state of the QSPIx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.         
  * @retval None
  */
void QSPI_LoopBackConfig(FunctionalState NewState)
{
	
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	QSPI->CTRLR0 &= (uint32_t)~((uint32_t)QSPI_CTRLR0_SRL);	
	
  if (NewState != DISABLE)
  {
    /* Enable the Loopback mode */
    QSPI->CTRLR0 |= QSPI_CTRLR0_SRL;
  }
  else
  {
    /* Disable the Loopback mode */
	QSPI->CTRLR0 &= (uint32_t)~((uint32_t)QSPI_CTRLR0_SRL);	
  }

}

/**
  * @brief  QSPI CS Overturn Enable.
  * @param  NewState: new state of the QSPI CS Overturn. 
  *          This parameter can be: ENABLE or DISABLE.         
  * @retval None
  */
void QSPI_CS_OverTurnConfig(FunctionalState NewState)
{
	
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	QSPI->CTRLR0 &= (uint32_t)~((uint32_t)QSPI_CTRLR0_SSTE);	
	
  if (NewState != DISABLE)
  {
    /* Enable the CS Overturn */
    QSPI->CTRLR0 |= QSPI_CTRLR0_SSTE;
  }
  else
  {
    /* Disable the CS Overturne */
	QSPI->CTRLR0 &= (uint32_t)~((uint32_t)QSPI_CTRLR0_SSTE);	
  }

}

/**
  * @brief  Configures the Microwire control data size for the selected QSPI.
  * @param  Microwire_ControlDataSize: specifies the QSPI Microwire Mode ControlDataSize.
  *         For the QSPIx peripheral this parameter can be one of the following values:
  *            @arg Microwire_ControlDataSize_1b : Set data size to 1 bits
  *            @arg Microwire_ControlDataSize_2b : Set data size to 2 bits
  *            @arg Microwire_ControlDataSize_3b : Set data size to 3 bits
  *            @arg Microwire_ControlDataSize_4b : Set data size to 4 bits
  *            @arg Microwire_ControlDataSize_5b : Set data size to 5 bits
  *            @arg Microwire_ControlDataSize_6b : Set data size to 6 bits
  *            @arg Microwire_ControlDataSize_7b : Set data size to 7 bits
  *            @arg Microwire_ControlDataSize_8b : Set data size to 8 bits
  *            @arg Microwire_ControlDataSize_9b : Set data size to 9 bits
  *            @arg Microwire_ControlDataSize_10b: Set data size to 10 bits
  *            @arg Microwire_ControlDataSize_11b: Set data size to 11 bits
  *            @arg Microwire_ControlDataSize_12b: Set data size to 12 bits
  *            @arg Microwire_ControlDataSize_13b: Set data size to 13 bits
  *            @arg Microwire_ControlDataSize_14b: Set data size to 14 bits
  *            @arg Microwire_ControlDataSize_15b: Set data size to 15 bits
  *            @arg Microwire_ControlDataSize_16b: Set data size to 16 bits
  * @retval None
  */
void QSPI_Microwire_ControlDataSizeConfig(uint32_t Microwire_ControlDataSize)
{
  
  /* Check the parameters */
  assert_param(IS_QSPI_Microwire_ControlDataSize(Microwire_ControlDataSize));
	
	QSPI->CTRLR0 &= (uint32_t)~((uint32_t)QSPI_CTRLR0_CFS);	
	
  /* Set new DS[4:0] bits value */
  QSPI->CTRLR0 |= Microwire_ControlDataSize;
}


/**
  * @brief  Selcte QSPI Frame Mode.
  * @param  Frame_Mode: select QSPI Frame Mode.
  *         This parameter can be:  QSPI_SPI_SINGLE
  *                                 QSPI_SPI_DUAL             
  *                                 QSPI_SPI_QUAD                   
  * @retval None
  */
void QSPI_SPIFrameConfig(uint32_t Frame_Mode)
{
  /* Check the parameters */
  assert_param(IS_QSPI_FRAMEMODE(Frame_Mode));

	QSPI->CTRLR0 &= (uint32_t)~((uint32_t)QSPI_CTRLR0_SPI_FRF);	
	
  QSPI->CTRLR0 |= Frame_Mode;
}

/**
  * @brief  Config QSPI Receive Number.
  * @note   This function vaild when Transmit_Direction parameter select：QSPI_Direction_RX QSPI_Direction_EPPROMREAD
  * @param  Num: Config QSPI Receive Number.
  *         This parameter can be: 0<= Num <=0xFFFF
                                                    
  * @retval None
  */
void QSPI_ReceiveNumConfig(uint32_t Num)
{
  /* Check the parameters */
  assert_param(IS_QSPI_ReceiveNum(Num));
	
  QSPI->CTRLR1 = Num;
}


/**
  * @brief  Selcte QSPI Microwire Transmit mode .
  * @param  Microwire_Mode: select Microwire Transmit mode.
  *         This parameter can be:  Microwire_Sequence
  *                                 Microwire_Non_Sequence                                                                 
  * @retval None
  */
void QSPI_MicrowireModeConfig(uint16_t Microwire_Mode)
{
  /* Check the parameters */
  assert_param(IS_QSPI_MICROWIREMODE(Microwire_Mode));

	QSPI->MWCR &= (uint32_t)~((uint32_t)QSPI_MWCR_MWMOD);	
	
  QSPI->MWCR |= Microwire_Mode;
}

/**
  * @brief  Selcte QSPI Microwire Transmit Direction .
  * @param  Microwire_Direction: select Microwire Transmit Direction.
  *         This parameter can be:  Microwire_TX
  *                                 Microwire_RX                                                                 
  * @retval None
  */
void QSPI_MicrowireDirectionConfig(uint16_t Microwire_Direction)
{
  /* Check the parameters */
  assert_param(IS_QSPI_MICROWIREMODE(Microwire_Direction));

	QSPI->MWCR &= (uint32_t)~((uint32_t)QSPI_MWCR_MDD);	
	
  QSPI->MWCR |= Microwire_Direction;
	
}

/**
  * @brief  Enbale or Disable QSPI Microwire Handshaking Interface 
  * @param  NewState: new state of the QSPIx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.                                                                
  * @retval None
  */
void QSPI_MicrowireHandShaking(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	QSPI->MWCR &= (uint32_t)~((uint32_t)QSPI_MWCR_MHS);	
	
  if (NewState != DISABLE)
  {
    /* Enable QSPI Microwire Handshaking Interface */
    QSPI->MWCR |= QSPI_MWCR_MHS;
  }
  else
  {
    /* Disable QSPI Microwire Handshaking Interface */
	  QSPI->MWCR &= (uint32_t)~((uint32_t)QSPI_MWCR_MHS);	
  }
	
}

/**
  * @brief  Enables or disables the specified QSPI peripheral CS.
  * @param  NewState: new state of the QSPI peripheral CS . 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void QSPI_CSCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected QSPI peripheral cs */
    QSPI->SER = QSPI_CS_EN;
  }
  else
  {
    /* Disable the selected QSPI peripheral cs */
    QSPI->SER = 0;
  }
}

/**
  * @brief  Config QSPI BaudRate.
  * @param  BaudRate: Config QSPI BaudRate.
  *         This parameter can be: 0<= BaudRate <=0xFFFF
                                                    
  * @retval None
  */
void QSPI_BaudRateConfig(uint16_t BaudRate)
{
  /* Check the parameters */
  assert_param(IS_QSPI_BAUDRATE_PRESCALER(BaudRate));
	
  QSPI->BAUDR = BaudRate;
	
}

/**
  * @brief  Config QSPI TXFIFO_Threshold.
  * @param  Threshold: Config QSPI TXFIFO_Threshold Value.
  *         This parameter can be: 0<= Threshold <=0xFF
                                                    
  * @retval None
  */
void QSPI_TXFIFO_ThresholdConfig(uint8_t Threshold)
{
  /* Check the parameters */
  assert_param(IS_QSPI_TXFIFO_THRESHOLD(Threshold));
	
	QSPI->TXFTLR &= (uint32_t)~((uint32_t)QSPI_TXFTLR_TFT);	
	
	QSPI->TXFTLR |= Threshold;
}

/**
  * @brief  Config QSPI TXFIFO StartLevel.
  * @param  Level: Config QSPI TXFIFO StartLevel Value.
  *         This parameter can be: 0<= Level <=0xFF
                                                    
  * @retval None
  */
void QSPI_TXFIFO_StartLevelConfig(uint8_t Level)
{
  /* Check the parameters */
  assert_param(IS_QSPI_TXFIFO_STARTLEVEL(Level));
	
	QSPI->TXFTLR &= (uint32_t)~((uint32_t)QSPI_TXFTLR_TXFTHR);	
	
	QSPI->TXFTLR |= (Level<<16);
}


/**
  * @brief  Config QSPI RXFIFO_Threshold.
  * @param  Threshold: Config QSPI RXFIFO_Threshold Value.
  *         This parameter can be: 0<= Threshold <=0xFF
                                                    
  * @retval None
  */
void QSPI_RXFIFO_ThresholdConfig(uint8_t Threshold)
{
  /* Check the parameters */
  assert_param(IS_QSPI_RXFIFO_THRESHOLD(Threshold));
	
	QSPI->RXFTLR &= (uint32_t)~((uint32_t)QSPI_RXFTLR_RFT);	
	
	QSPI->RXFTLR |= Threshold;
}


/**
  * @brief  Get the number of valid data entries in the TXFIFO.
  * @param  None                                     
  * @retval None
  */
uint16_t QSPI_GetTXFIFONum(void)
{
  uint16_t TXFIFO_Num = 0;

  /* Get the number of valid data entries in the transmit FIFO */	
	TXFIFO_Num = QSPI->TXFLR ;
	
  return TXFIFO_Num;
}


/**
  * @brief  Get the number of valid data entries in the RXFIFO.
  * @param  None                                     
  * @retval None
  */
uint16_t QSPI_GetRXFIFONum(void)
{
  uint16_t RXFIFO_Num = 0;

  /* Get the number of valid data entries in the receive  FIFO */	
	RXFIFO_Num = QSPI->RXFLR ;
	
  return RXFIFO_Num;
}

/**
  * @brief  Get the flag status of the QSPI.
  * @param  QSPI_STATUS: Specify the flag to be checked.
  *         This parameter can be one of the following values:
  *         @arg QSPI_SR_BUSY       : QSPI Busy Flag
  *         @arg QSPI_SR_TFNF       : QSPI Transmit FIFO Not Full
  *         @arg QSPI_SR_TFE        : QSPI Transmit FIFO Empty
  *         @arg QSPI_SR_RNE        : QSPI Receive FIFO Not Empty
  *         @arg QSPI_SR_RFF        : QSPI Receive FIFO Full
  * @return FlagStatus of PWM_FLAG (SET or RESET).
  */
FlagStatus QSPI_GetFlagStatus(uint32_t QSPI_STATUS)
{
  /* Check the parameters */
  assert_param(IS_QSPI_STATUS_FLAG(QSPI_STATUS));	

  if ((	QSPI->SR & QSPI_STATUS) != 0)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/**
  * @brief  Enables or disables the specified QSPI DMA TX or RX.
  * @param  NewState: new state of the QSPIx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @param  DMA_Direction: DMA Transmit Direction. 
  *          This parameter can be: QSPI_DMA_TX.
  *                                 QSPI_DMA_RX.
  * @retval None
  */
void QSPI_DMACmd(uint32_t DMA_Direction,FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_QSPI_DMA_DIRECTION(DMA_Direction));	
  assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	QSPI->DMACR &= (uint32_t)~((uint32_t)DMA_Direction);	
  
  if (NewState != DISABLE)
  {
    /* Enable the selected QSPI DMA  */
    QSPI->DMACR |= DMA_Direction;
  }
  else
  {
    /* Disable the selected QSPI DMA */
	QSPI->DMACR &= (uint32_t)~((uint32_t)DMA_Direction);	
  }
}


/**
  * @brief  Config QSPI DMA TXFIFO_Threshold.
  * @param  Threshold: Config QSPI DMA TXFIFO_Threshold Value.
  *         This parameter can be: 0<= Threshold <=0xF
                                                    
  * @retval None
  */
void QSPI_DMATXFIFO_ThresholdConfig(uint8_t Threshold)
{
  /* Check the parameters */
  assert_param(IS_QSPI_DMATXFIFO_THRESHOLD(Threshold));
	
	QSPI->DMATDLR = Threshold;
	
}


/**
  * @brief  Config QSPI DMA RXFIFO_Threshold.
  * @param  Threshold: Config QSPI DMA RXFIFO_Threshold Value.
  *         This parameter can be: 0<= Threshold <=0xF
                                                    
  * @retval None
  */
void QSPI_DMARXFIFO_ThresholdConfig(uint8_t Threshold)
{
  /* Check the parameters */
  assert_param(IS_QSPI_DMARXFIFO_THRESHOLD(Threshold));
	
	QSPI->DMARDLR = Threshold;
	
}



/** @defgroup QSPI_Group2 Data transfers functions
 *  @brief   Data transfers functions
 *
@verbatim
 ===============================================================================
                    ##### Data transfers functions #####
 ===============================================================================
    [..] This section provides a set of functions allowing to manage the QSPI
         data transfers.

    [..] In reception, data are received and then stored into an internal Rx buffer while 
         In transmission, data are first stored into an internal Tx buffer before being 
         transmitted.

    [..] The read access of the QSPI_DR register can be done using 
         QSPI_ReceiveData8() (when data size is equal or inferior than 8bits) and.
         and returns the Rx buffered value. Whereas a write access to the QSPI_DR 
         can be done using QSPI_SendData8() (when data size is equal or inferior than 8bits)
         and stores the written data into Tx buffer.

@endverbatim
  * @{
  */


/**
  * @brief  Transmits a Data through the QSPIx peripheral.
  * @param  Data : Data to be transmitted.
  * @retval None
  */
void QSPI_SendData(uint32_t Data)
{
  
  /* Write in the DR register the data to be sent */
  QSPI->DR = Data;
}

/**
  * @brief  Returns the most recent received data by the QSPIx peripheral. 
  * @retval The value of the received data.
  */
uint32_t QSPI_ReceiveData(void)
{

  /* Return the data in the DR register */
  return QSPI->DR;
}


/** @defgroup QSPI_Group5 Interrupts and flags management functions
 *  @brief   Interrupts and flags management functions
  *
@verbatim   
 ===============================================================================
             ##### Interrupts and flags management functions #####
 ===============================================================================
    [..] This section provides a set of functions allowing to configure the QSPI/I2S Interrupts 
         sources and check or clear the flags or pending bits status.
         The user should identify which mode will be used in his application to manage 
         the communication: Polling mode, Interrupt mode or DMA mode. 

  *** Polling Mode ***
  ====================
    [..] In Polling Mode, the QSPI/I2S communication can be managed by 9 flags:
        (#) QSPI_FLAG_TFE : to indicate the status of the transmit buffer register
        (#) QSPI_FLAG_TFNF : to indicate the status of the receive buffer register
        (#) QSPI_FLAG_BUSY : to indicate the state of the communication layer of the QSPI.
        (#) QSPI_FLAG_RFNE : to indicate if a CRC Calculation error occur              
        (#) QSPI_FLAG_RFF : to indicate if a Mode Fault error occur

    [..]
        (@)Do not use the BSY flag to handle each data transmission or reception. It is better 
           to use the TXE and RXNE flags instead.

    [..] In this Mode it is advised to use the following functions:
        (+) FlagStatus QSPI_GetFlagStatus(CMSDK_QSPI_TypeDef* QSPIx, uint16_t QSPI_FLAG);
        (+) void QSPI_ClearFlag(CMSDK_QSPI_TypeDef* QSPIx, uint16_t QSPI_FLAG);

  *** Interrupt Mode ***
  ======================
    [..] In Interrupt Mode, the QSPI/I2S communication can be managed by 4 interrupt sources
         and 5 pending bits: 
    [..] Pending Bits:
        (#) QSPI_I2S_IT_TXE : to indicate the status of the transmit buffer register
        (#) QSPI_I2S_IT_RXNE : to indicate the status of the receive buffer register
        (#) QSPI_I2S_IT_OVR : to indicate if an Overrun error occur
        (#) I2S_IT_UDR : to indicate an Underrun Error occurs.
        (#) QSPI_I2S_FLAG_FRE : to indicate a Frame Format error occurs.

    [..] Interrupt Source:
        (#) QSPI_I2S_IT_TXE: specifies the interrupt source for the Tx buffer empty 
            interrupt.  
        (#) QSPI_I2S_IT_RXNE : specifies the interrupt source for the Rx buffer not 
            empty interrupt.
        (#) QSPI_I2S_IT_ERR : specifies the interrupt source for the errors interrupt.

    [..] In this Mode it is advised to use the following functions:
         (+) void QSPI_I2S_ITConfig(CMSDK_QSPI_TypeDef* QSPIx, uint8_t QSPI_I2S_IT, FunctionalState NewState);
         (+) ITStatus QSPI_I2S_GetITStatus(CMSDK_QSPI_TypeDef* QSPIx, uint8_t QSPI_I2S_IT);

  *** FIFO Status ***
  ===================
    [..] It is possible to monitor the FIFO status when a transfer is ongoing using the
         following function:
         (+) uint32_t QSPI_GetFIFOStatus(uint8_t QSPI_FIFO_Direction); 

  *** DMA Mode ***
  ================
    [..] In DMA Mode, the QSPI communication can be managed by 2 DMA Channel 
         requests:
        (#) QSPI_I2S_DMAReq_Tx: specifies the Tx buffer DMA transfer request.
        (#) QSPI_I2S_DMAReq_Rx: specifies the Rx buffer DMA transfer request.

    [..] In this Mode it is advised to use the following function:
        (+) void QSPI_I2S_DMACmd(CMSDK_QSPI_TypeDef* QSPIx, uint16_t QSPI_I2S_DMAReq, FunctionalState NewState).

@endverbatim
  * @{
  */

/**
  * @brief  Enables or disables the specified QSPI interrupts.
  * @param  QSPI_IT: specifies the QSPI interrupt source to be enabled or disabled. 
  *          This parameter can be one of the following values:
  *            @arg QSPI_IT_TXEIM: Transmit FIFO Empty Interrupt Mask
  *            @arg QSPI_IT_TXOIM: Transmit FIFO Overflow Interrupt Mask
  *            @arg QSPI_IT_RXUIM: Receive FIFO Underflow Interrupt Mask
  *            @arg QSPI_IT_RXOIM: Receive FIFO Overflow Interrupt Mask 	
  *            @arg QSPI_IT_RXFIM: Receive FIFO Full Interrupt Mask 		
  * @param  NewState: new state of the specified QSPI interrupt.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void QSPI_ITConfig(uint32_t QSPI_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_QSPI_CONFIG_IT(QSPI_IT));

  if (NewState != DISABLE)
  {
    /* Enable the selected QSPI interrupt */
    QSPI->IMR |= QSPI_IT;
  }
  else
  {
    /* Disable the selected QSPI interrupt */
    QSPI->IMR &= (uint16_t)~QSPI_IT;
  }
}



/**
  * @brief  Clears the QSPI flag.
  * @param  QSPI_FLAG: specifies the QSPI flag to clear. 
  *          This parameter can be one of the following values:
  *            @arg QSPI_IT_TXOIM: Transmit FIFO Overflow Interrupt Mask
  *            @arg QSPI_IT_RXUIM: Receive FIFO Underflow Interrupt Mask
  *            @arg QSPI_IT_RXOIM: Receive FIFO Overflow Interrupt Mask 	
  *         
  * @retval None
  */
void QSPI_ClearITFlag(uint32_t QSPI_FLAG)
{
  /* Check the parameters */
  assert_param(IS_QSPI_GET_IT(QSPI_FLAG));

  /* Clear Transmit FIFO Overflow Interrupt Flag */
	if(QSPI_FLAG==QSPI_IT_TXOIM)
	{
   QSPI->TXOICR;
	}
  /* Clear Receive FIFO Overflow Interrupt Flag */
	if(QSPI_FLAG==QSPI_IT_RXOIM)
	{
   QSPI->RXOICR;
	}	
  /* Clear Receive FIFO Underflow Interrupt Flag */
	if(QSPI_FLAG==QSPI_IT_RXUIM)
	{
   QSPI->RXUICR;
	}		
	
}

/**
  * @brief  Clears ALL the QSPI flag(TXOIS/RXOIS/RXUIS). 	
  *         
  * @retval None
  */
void QSPI_ClearALLITFlag(void)
{
  /* Clear TXOIS/RXOIS/RXUIS Flag */
   QSPI->ICR;
	
}



/**
  * @brief  Checks whether the specified QSPI interrupt has occurred or not.
  * @param  QSPI_IT: specifies the QSPI interrupt source to check. 
  *          This parameter can be one of the following values:
  *            @arg QSPI_IT_TXEIM: Transmit FIFO Empty Interrupt Mask
  *            @arg QSPI_IT_TXOIM: Transmit FIFO Overflow Interrupt Mask
  *            @arg QSPI_IT_RXUIM: Receive FIFO Underflow Interrupt Mask
  *            @arg QSPI_IT_RXOIM: Receive FIFO Overflow Interrupt Mask 	
  *            @arg QSPI_IT_RXFIM: Receive FIFO Full Interrupt Mask   
  * @retval The new state of QSPI_IT (SET or RESET).
  */
ITStatus QSPI_GetITStatus(uint32_t QSPI_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;

  /* Check the parameters */
  assert_param(IS_QSPI_GET_IT(QSPI_IT));

  /* Get the QSPI_IT enable bit status */
  enablestatus = (uint32_t)(QSPI->IMR & QSPI_IT); 

  /* Check the status of the specified QSPI interrupt */
  if (((uint32_t)(QSPI->ISR & QSPI_IT) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
  {
    /* QSPI_IT is set */
    bitstatus = SET;
  }
  else
  {
    /* QSPI_IT is reset */
    bitstatus = RESET;
  }
  /* Return the QSPI_IT status */
  return  bitstatus;
}


/**
  * @brief  Checks whether the specified QSPI interrupt has occurred or not.
  * @param  QSPI_RawIT: specifies the QSPI raw interrupt status to check. 
  *          This parameter can be one of the following values:
  *            @arg QSPI_RAWIT_STATUS_TXEIM: Transmit FIFO Empty Raw Interrupt Status
  *            @arg QSPI_RAWIT_STATUS_TXOIM: Transmit Transmit FIFO Overflow Raw Interrupt Status
  *            @arg QSPI_RAWIT_STATUS_RXUIM: Receive FIFO Underflow Raw Interrupt Status
  *            @arg QSPI_RAWIT_STATUS_RXOIM: Receive FIFO Overflow Raw Interrupt Status 	
  *            @arg QSPI_RAWIT_STATUS_RXFIM: FIFO Full Raw Interrupt Status   
  * @retval The new state of QSPI_IT (SET or RESET).
  */
FlagStatus QSPI_GetRawITStatus(uint32_t QSPI_RawIT)
{

  /* Check the parameters */
  assert_param(IS_QSPI_GET_RAWIT_STATUS(QSPI_RawIT));

  if ((	QSPI->RISR & QSPI_RawIT) != 0)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/**
  * @brief  Config QSPI-Flash Communication Address and instruction transfer format.
  * @param  TransType: Config Address and instruction transfer format.
  *         This parameter can be: QSPI_FLASH_TRANS_TYPE_TT0 : Instruction and Address will be sent in Standard SPI Mode. 
  *                                QSPI_FLASH_TRANS_TYPE_TT1 : Instruction will be sent in Standard SPI Mode and Address will be sent in the mode specified by CTRLR0.SPI_FRF
  *                                QSPI_FLASH_TRANS_TYPE_TT2 : Both Instruction and Address will be sent in the mode specified by SPI_FRF.                                                  
  * @retval None
  */
void QSPI_FLASH_TransTypeConfig(uint32_t TransType)
{
  /* Check the parameters */
  assert_param(IS_QSPI_FLASH_TRANS_TYPE(TransType));
	
	QSPI->SPI_CTRLR &= (uint32_t)~((uint32_t)QSPI_SPI_CTRLR_TRANS_TYPE);	
	
	QSPI->SPI_CTRLR |= TransType;
	
}

/**
  * @brief  Config QSPI-Flash Communication Length of Address to be transmitted.
  * @param  Addr_Length: Config Length of Address.
  *         This parameter can be: QSPI_FLASH_ADDR_L0   :  No Address 
  *                                QSPI_FLASH_ADDR_L4   :  4  bit Address length  
  *                                QSPI_FLASH_ADDR_L8   :  8  bit Address length 
  *                                QSPI_FLASH_ADDR_L12  :  12 bit Address length 
  *                                QSPI_FLASH_ADDR_L16  :  16 bit Address length 
  *                                QSPI_FLASH_ADDR_L20  :  20 bit Address length 
  *                                QSPI_FLASH_ADDR_L24  :  24 bit Address length 
  *                                QSPI_FLASH_ADDR_L28  :  28 bit Address length 
  *                                QSPI_FLASH_ADDR_L32  :  32 bit Address length 
  *                                QSPI_FLASH_ADDR_L36  :  36 bit Address length 
  *                                QSPI_FLASH_ADDR_L40  :  40 bit Address length 
  *                                QSPI_FLASH_ADDR_L44  :  44 bit Address length 
  *                                QSPI_FLASH_ADDR_L48  :  48 bit Address length 
  *                                QSPI_FLASH_ADDR_L52  :  52 bit Address length 
  *                                QSPI_FLASH_ADDR_L56  :  56 bit Address length 
  *                                QSPI_FLASH_ADDR_L60  :  60 bit Address length 
  * @retval None
  */
void QSPI_FLASH_AddrLengthConfig(uint32_t Addr_Length)
{
  /* Check the parameters */
  assert_param(IS_QSPI_Flash_ADDR_LENGTH(Addr_Length));
	
	QSPI->SPI_CTRLR &= (uint32_t)~((uint32_t)QSPI_SPI_CTRLR_ADDR_L);	
	
	QSPI->SPI_CTRLR |= Addr_Length;
	
}


/**
  * @brief  Config QSPI-Flash Communication Instruction Length.
  * @param  Instruction_Length: Config Length of Instruction.
  *         This parameter can be: QSPI_FLASH_INST_L0   :  No Instruction 
  *                                QSPI_FLASH_INST_L4   :  4  bit Instruction  length  
  *                                QSPI_FLASH_INST_L8   :  8  bit Instruction  length 
  *                                QSPI_FLASH_INST_L16  :  16 bit Instruction  length 
  * @retval None
  */
void QSPI_FLASH_INSTLengthConfig(uint32_t Instruction_Length)
{
  /* Check the parameters */
  assert_param(IS_QSPI_Flash_INST_LENGTH(Instruction_Length));
	
	QSPI->SPI_CTRLR &= (uint32_t)~((uint32_t)QSPI_SPI_CTRLR_INST_L);	
	
	QSPI->SPI_CTRLR |= Instruction_Length;
	
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
