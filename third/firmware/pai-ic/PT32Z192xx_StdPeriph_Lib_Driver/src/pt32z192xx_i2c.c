/**
  ******************************************************************************
  * @file    pt32z192xx_i2c.c
  * @author  应用开发团队
  * @version 
  * @date    
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Inter-Integrated circuit (I2C):
  *           + Initialization and Configuration
  *           + Communications handling
  *           + I2C registers management
  *           + Data transfers management
  *           + Interrupts and flags management
  *
  *  @verbatim
 ============================================================================
                     ##### How to use this driver #####
 ============================================================================
   [..]
   (#) Peripherals alternate function: 
       (++) Connect the pin to the desired peripherals' Alternate 
            Function (AF) using GPIO_PinAFConfig() function.
       (++) Configure the desired pin in alternate function by:
            GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AF
       (++) Select the type, OpenDrain and speed via  
            GPIO_PuPd, GPIO_OType and GPIO_Speed members
       (++) Call GPIO_Init() function.
   (#) Program the Mode, Timing , Own address, Ack and Acknowledged Address 
       using the I2C_Init() function.
   (#) Optionally you can enable/configure the following parameters without
       re-initialization (i.e there is no need to call again I2C_Init() function):
       (++) Enable the acknowledge feature using I2C_AcknowledgeConfig() function.
       (++) Enable the dual addressing mode using I2C_DualAddressCmd() function.
       (++) Enable the general call using the I2C_GeneralCallCmd() function.
       (++) Enable the clock stretching using I2C_StretchClockCmd() function.
       (++) Enable the PEC Calculation using I2C_CalculatePEC() function.
       (++) For SMBus Mode:
            (+++) Enable the SMBusAlert pin using I2C_SMBusAlertCmd() function.
   (#) Enable the NVIC and the corresponding interrupt using the function
       I2C_ITConfig() if you need to use interrupt mode.
   (#) When using the DMA mode 
      (++) Configure the DMA using DMA_Init() function.
      (++) Active the needed channel Request using I2C_DMACmd() function.
   (#) Enable the I2C using the I2C_Cmd() function.
   (#) Enable the DMA using the DMA_Cmd() function when using DMA mode in the 
       transfers. 
   [..]
   (@) When using I2C in Fast Mode Plus, SCL and SDA pin 20mA current drive capability
       must be enabled by setting the driving capability control bit in SYSCFG.

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
#include "pt32z192xx_i2c.h"

/** @addtogroup PT32F0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup I2C 
  * @brief I2C driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_BUFSIZE         9
#define BUFSIZE             8     //slave
#define MAX_TIMEOUT         0x002FFFFF

#define I2CSTATE_IDLE       0x000
#define I2CSTATE_PENDING    0x001
#define I2CSTATE_ACK        0x101
#define I2CSTATE_NACK       0x102
#define I2CSTATE_SLA_NACK   0x103
#define I2CSTATE_ARB_LOSS   0x104


#define I2C_IDLE            0
#define I2C_STARTED         1
#define I2C_RESTARTED       2
#define I2C_REPEATED_START  3
#define DATA_ACK            4
#define DATA_NACK           5
#define I2C_WR_STARTED      6
#define I2C_RD_STARTED      7
#define I2C_BROADCASTED     8
#define I2C_START_FAIL      0xFFFFFFFF

#define I2CMASTER           0x01
#define I2CSLAVE            0x02

#define SLAVE_ADDR          0x00       
#define READ_WRITE          0x01
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint32_t _I2cMode;                       // I2CMASTER or I2CSLAVE

/** @defgroup I2C_Private_TypesDefinitions
  * @{
  */
volatile uint32_t I2CMasterState = I2CSTATE_IDLE;
volatile uint32_t I2CSlaveState = I2CSTATE_IDLE;

volatile uint8_t I2CMasterBuffer[I2C_BUFSIZE];    // Master Mode
volatile uint8_t I2CSlaveBuffer[I2C_BUFSIZE];     // Master Mode

volatile uint8_t I2CWrBuffer[I2C_BUFSIZE];        // Slave Mode
volatile uint8_t I2CRdBuffer[I2C_BUFSIZE];        // Slave Mode

volatile uint32_t I2CReadLength;
volatile uint32_t I2CWriteLength;

volatile uint32_t RdIndex;
volatile uint32_t WrIndex;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup I2C_Private_Functions
  * @{
  */


/** @defgroup I2C_Group1 Initialization and Configuration functions
 *  @brief   Initialization and Configuration functions 
 *
@verbatim   
 ===============================================================================
           ##### Initialization and Configuration functions #####
 ===============================================================================
    [..] This section provides a set of functions allowing to initialize the I2C Mode,
         I2C Timing, I2C filters, I2C Addressing mode, I2C OwnAddress1.

    [..] The I2C_Init() function follows the I2C configuration procedures (these procedures 
         are available in reference manual).

    [..] When the Software Reset is performed using I2C_SoftwareResetCmd() function, the internal
         states machines are reset and communication control bits, as well as status bits come 
         back to their reset value.

    [..] Before enabling Stop mode using I2C_StopModeCmd() I2C Clock source must be set to
         HSI and Digital filters must be disabled.

    [..] Before enabling Own Address 2 via I2C_DualAddressCmd() function, OA2 and mask should be
         configured using I2C_OwnAddress2Config() function.

    [..] I2C_SlaveByteControlCmd() enable Slave byte control that allow user to get control of 
         each byte in slave mode when NBYTES is set to 0x01.

@endverbatim
  * @{
  */



/**
  * @brief  Initializes the I2Cx peripheral according to the specified
  *         parameters in the I2C_InitStruct.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  I2C_InitStruct: pointer to a I2C_InitTypeDef structure that
  *         contains the configuration information for the specified I2C peripheral.
  * @retval None
  */
void I2C_Init(CMSDK_I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_OWN_ADDRESS(I2C_InitStruct->I2C_OwnAddress));
  assert_param(IS_FUNCTIONAL_STATE(I2C_InitStruct->I2C_Ack));
  assert_param(IS_SCLK_DIV(I2C_InitStruct->I2C_SCLK_DIV));
  assert_param(IS_I2C_BROADCAST(I2C_InitStruct->I2C_BroadCast));

  /* Disable I2Cx Peripheral */
  I2Cx->CCR = I2C_CCR_MASK;

  /*---------------------------- I2Cx BroadCast Configuration ---------------------*/
  /* ENABLE I2Cx BroadCast */	
	if((I2C_InitStruct->I2C_BroadCast)!= DISABLE)
	{
  I2Cx->OAR |= I2C_OAR_BCSTEN;
	}
  /* Disable I2Cx BroadCast */		
	else
	{
    I2Cx->OAR &= (uint32_t)~((uint32_t)I2C_OAR_BCSTEN);	
	}
	
  /*---------------------------- I2Cx OAR Configuration ---------------------*/
  /* Clear OAR register */
  I2Cx->OAR &= (uint32_t)~I2C_OAR_ADDR;
  /* Configure I2Cx: Own Address */
  tmpreg = (uint32_t)((I2C_InitStruct->I2C_OwnAddress)<<0x01);
  /* Write to I2Cx OAR */
  I2Cx->OAR = tmpreg;

  /*---------------------------- I2Cx ACK Configuration ----------------------*/
  /* ENABLE I2Cx ACK */	
	if((I2C_InitStruct->I2C_Ack)==ENABLE)
	{
  I2Cx->CSR = I2C_Ack_Enable;
	}
  /* Disable I2Cx ACK */		
	else
	{
  I2Cx->CCR = I2C_Ack_Disable;		
	}
  /*---------------------------- I2Cx Frequency division factor Configuration  ----------------------*/
  /* Clear CR register CR bit */
  I2Cx->CCR = I2C_CCR_CR;
  /* Configure I2Cx: CR[2:0] */	
  I2Cx->CSR	= I2C_InitStruct->I2C_SCLK_DIV;
	
}

/**
  * @brief  Fills each I2C_InitStruct member with its default value.
  * @param  I2C_InitStruct: pointer to an I2C_InitTypeDef structure which will be initialized.
  * @retval None
  */
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct)
{
  /*---------------- Reset I2C init structure parameters values --------------*/
  /* Initialize the I2C_OwnAddress1 member */
  I2C_InitStruct->I2C_OwnAddress = 0;
  /* Initialize the I2C_Ack member */
  I2C_InitStruct->I2C_Ack = DISABLE;
  /* Initialize the  Frequency division factor when pclk=24000000 member SCLK=100000 */
  I2C_InitStruct->I2C_SCLK_DIV= I2C_SCLK_DIVX192;
}

/**
  * @brief  Enables or disables the specified I2C peripheral.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2Cx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_Cmd(CMSDK_I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* Enable the selected I2C peripheral */
    I2Cx->CSR = I2C_CSR_EN;
  }
  else
  {
    /* Disable the selected I2C peripheral */
    I2Cx->CCR = I2C_CCR_EN;
  }
}




/**
  * @brief  Enables or disables the I2C general call mode.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2C general call mode.
  *          This parameter can be: ENABLE or DISABLE.  
  * @retval None
  */
void I2C_GeneralCallCmd(CMSDK_I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable general call mode */
    I2Cx->OAR |= I2C_OAR_BCSTEN;
  }
  else
  {
    /* Disable general call mode */
    I2Cx->OAR &= (uint32_t)~((uint32_t)I2C_OAR_BCSTEN);
  }
} 


/**
  * @brief  Configures the slave address to be transmitted after start generation.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  Address: specifies the slave address to be programmed.
  * @note   This function should be called before generating start condition.
  * @retval None
  */
void I2C_SlaveAddressConfig(CMSDK_I2C_TypeDef* I2Cx, uint16_t Address)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_SLAVE_ADDRESS(Address));


  /* Set I2Cx DR */
  I2Cx->DR = (uint32_t)Address;

  /* Send Address */
  I2Cx->CCR = I2C_CCR_IS | I2C_CCR_START ;
}
  

/**
  * @}
  */


/** @defgroup I2C_Group2 Communications handling functions
 *  @brief   Communications handling functions 
 *
@verbatim
 ===============================================================================
                  ##### Communications handling functions #####
 ===============================================================================  
    [..] This section provides a set of functions that handles I2C communication.

    [..] Automatic End mode is enabled using I2C_AutoEndCmd() function. When Reload
         mode is enabled via I2C_ReloadCmd() AutoEnd bit has no effect.

    [..] I2C_NumberOfBytesConfig() function set the number of bytes to be transferred,
         this configuration should be done before generating start condition in master 
         mode.

    [..] When switching from master write operation to read operation in 10Bit addressing
         mode, master can only sends the 1st 7 bits of the 10 bit address, followed by 
         Read direction by enabling HEADR bit using I2C_10BitAddressHeader() function.

    [..] In master mode, when transferring more than 255 bytes Reload mode should be used
         to handle communication. In the first phase of transfer, Nbytes should be set to 
         255. After transferring these bytes TCR flag is set and I2C_TransferHandling()
         function should be called to handle remaining communication.

    [..] In master mode, when software end mode is selected when all data is transferred
         TC flag is set I2C_TransferHandling() function should be called to generate STOP
         or generate ReStart.

@endverbatim
  * @{
  */
  
  
/**
  * @brief  Generates I2Cx communication START condition.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2C START condition generation.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_GenerateSTART(CMSDK_I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Generate a START condition */
		I2Cx->CCR = I2C_CCR_IS;
    I2Cx->CSR = I2C_CSR_START;
  }
  else
  {
    /* Disable the START condition generation */
    I2Cx->CCR = I2C_CCR_IS |I2C_CCR_START;
  }
}  
  
/**
  * @brief  Generates I2Cx communication STOP condition.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2C STOP condition generation.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_GenerateSTOP(CMSDK_I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Generate a STOP condition */
		I2Cx->CSR = I2C_CSR_STOP;
    I2Cx->CCR = I2C_CCR_IS;
  }
  else
  {
    /* Disable the STOP condition generation */
    I2Cx->CCR = I2C_CCR_IS | I2C_CSR_STOP;
  }
}  



/**
  * @brief  Generates I2C communication Acknowledge.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  NewState: new state of the Acknowledge.
  *          This parameter can be: ENABLE or DISABLE.  
  * @retval None
  */
void I2C_AcknowledgeConfig(CMSDK_I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable ACK generation */
    I2Cx->CCR = I2C_CCR_IS | I2C_CCR_START | I2C_CCR_STOP ;
		I2Cx->CSR = I2C_CSR_ACK;
  }
  else
  {
    /* Enable NACK generation */
    I2Cx->CCR = I2C_CCR_IS | I2C_CCR_ACK;
  }
}



/**
  * @}
  */  


  
/** @defgroup I2C_Group5 Data transfers management functions
 *  @brief   Data transfers management functions 
 *
@verbatim
 ===============================================================================
                ##### Data transfers management functions #####
 ===============================================================================  
    [..] This subsection provides a set of functions allowing to manage 
         the I2C data transfers.

    [..] The read access of the I2C_RXDR register can be done using 
         the I2C_ReceiveData() function and returns the received value.
         Whereas a write access to the I2C_TXDR can be done using I2C_SendData()
         function and stores the written data into TXDR.
@endverbatim
  * @{
  */  
  
/**
  * @brief  Sends a data byte through the I2Cx peripheral.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  Data: Byte to be transmitted..
  * @retval None
  */
void I2C_SendData(CMSDK_I2C_TypeDef* I2Cx, uint8_t Data)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  
  /* Write in the DR register the data to be sent */
  I2Cx->DR = (uint8_t)Data;
	I2Cx->CCR = I2C_CCR_IS | I2C_CCR_START; 
		
}

/**
  * @brief  Returns the most recent received data by the I2Cx peripheral.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @retval The value of the received data.
  */
uint8_t I2C_ReceiveData(CMSDK_I2C_TypeDef* I2Cx)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  
  /* Return the data in the DR register */
  return (uint8_t)I2Cx->DR;
}  

/**
  * @}
  */ 

uint32_t I2C_GetStatus(CMSDK_I2C_TypeDef* I2Cx)
{ 
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));  
  /* Return the SR register value */  
  return (I2Cx->SR);
} 


/** @defgroup I2C_Group2 Communications handling functions
 *  @brief   Communications handling functions 
 *
@verbatim
 ===============================================================================
                  ##### Communications handling functions #####
 ===============================================================================  
    [..] This section provides a set of functions that handles I2C communication.

    [..] Automatic End mode is enabled using I2C_AutoEndCmd() function. When Reload
         mode is enabled via I2C_ReloadCmd() AutoEnd bit has no effect.

    [..] I2C_NumberOfBytesConfig() function set the number of bytes to be transferred,
         this configuration should be done before generating start condition in master 
         mode.

    [..] When switching from master write operation to read operation in 10Bit addressing
         mode, master can only sends the 1st 7 bits of the 10 bit address, followed by 
         Read direction by enabling HEADR bit using I2C_10BitAddressHeader() function.

    [..] In master mode, when transferring more than 255 bytes Reload mode should be used
         to handle communication. In the first phase of transfer, Nbytes should be set to 
         255. After transferring these bytes TCR flag is set and I2C_TransferHandling()
         function should be called to handle remaining communication.

    [..] In master mode, when software end mode is selected when all data is transferred
         TC flag is set I2C_TransferHandling() function should be called to generate STOP
         or generate ReStart.

@endverbatim
  * @{
  */
  
/**
  * @brief  Enables or disables the I2C automatic end mode (stop condition is 
  *         automatically sent when nbytes data are transferred).
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2C automatic end mode.
  *          This parameter can be: ENABLE or DISABLE.
  * @note   This function has effect if Reload mode is disabled.
  * @retval None
  */

void I2C_TransferHandling(CMSDK_I2C_TypeDef* I2Cx, uint8_t StatValue)
{

	switch ( StatValue )
	{
	case 0x08:
		/*
		 * A START condition has been transmitted.
		 * We now send the slave address and initialize
		 * the write buffer
		 * (we always start with a write after START+SLA)
		 */
		WrIndex = 0;
		I2Cx->DR = I2CMasterBuffer[WrIndex++];
		I2Cx->CCR = (I2C_CCR_IS | I2C_CCR_START);
		I2CMasterState = I2CSTATE_PENDING;
		break;
	
	case 0x10:
		/*
		 * A repeated START condition has been transmitted.
		 * Now a second, read, transaction follows so we
		 * initialize the read buffer.
		 */
		RdIndex = 0;
		/* Send SLA with R bit set, */
		I2Cx->DR = I2CMasterBuffer[WrIndex++];
		I2Cx->CCR = (I2C_CCR_IS | I2C_CCR_START);
	break;
	
	case 0x18:
		/*
		 * SLA+W has been transmitted; ACK has been received.
		 * We now start writing bytes.
		 */
		I2Cx->DR = I2CMasterBuffer[WrIndex++];
		I2Cx->CCR = I2C_CCR_IS;
		break;

	case 0x20:	
		/*
		 * SLA+W has been transmitted; NOT ACK has been received.
		 * Send a stop condition to terminate the transaction
		 * and signal I2CEngine the transaction is aborted.
		 */
		I2Cx->CSR = I2C_CSR_STOP;
		I2Cx->CCR = I2C_CCR_IS;
		I2CMasterState = I2CSTATE_SLA_NACK;
		break;

	case 0x28:
		/*
		 * Data in I2DAT has been transmitted; ACK has been received.
		 * Continue sending more bytes as long as there are bytes to send
		 * and after this check if a read transaction should follow.
		 */
		if ( WrIndex < I2CWriteLength )
		{
			/* Keep writing as long as bytes avail */
			I2Cx->DR = I2CMasterBuffer[WrIndex++];
		}
		else
		{
			if ( I2CReadLength != 0 )
			{
				/* Send a Repeated START to initialize a read transaction */
				/* (handled in state 0x10)                                */
				/* Set Repeated-start flag */
				I2Cx->CSR = I2C_CSR_START;	
			}
			else
			{
				I2CMasterState = I2CSTATE_ACK;
				I2Cx->CSR = I2C_CSR_STOP;      /* Set Stop flag */
			}
		}
		I2Cx->CCR = I2C_CCR_IS;
		break;

	case 0x30:		
		/*
		 * Data byte in I2DAT has been transmitted; NOT ACK has been received
		 * Send a STOP condition to terminate the transaction and inform the
		 * I2CEngine that the transaction failed.
		 */
		I2Cx->CSR = I2C_CSR_STOP;
		I2Cx->CCR = I2C_CCR_IS;
		I2CMasterState = I2CSTATE_NACK;
		break;

	case 0x38:
		/*
		 * Arbitration loss in SLA+R/W or Data bytes.
		 * This is a fatal condition, the transaction did not complete due
		 * to external reasons (e.g. hardware system failure).
		 * Inform the I2CEngine of this and cancel the transaction
		 * (this is automatically done by the I2C hardware)
		 */
		I2CMasterState = I2CSTATE_ARB_LOSS;
		I2Cx->CCR = I2C_CCR_IS;
		break;

	case 0x40:		
		/*
		 * SLA+R has been transmitted; ACK has been received.
		 * Initialize a read.
		 * Since a NOT ACK is sent after reading the last byte,
		 * we need to prepare a NOT ACK in case we only read 1 byte.
		 */
		if ( I2CReadLength == 1 )
		{
			/* last (and only) byte: send a NACK after data is received */
			I2Cx->CCR = I2C_CCR_ACK;
		}
		else
		{
			/* more bytes to follow: send an ACK after data is received */
			I2Cx->CSR = I2C_CSR_ACK;
		}
		I2Cx->CCR = I2C_CCR_IS;
		break;

	case 0x48:		
		/*
		 * SLA+R has been transmitted; NOT ACK has been received.
		 * Send a stop condition to terminate the transaction
		 * and signal I2CEngine the transaction is aborted.
		 */
		I2Cx->CSR = I2C_CSR_STOP;
		I2Cx->CCR = I2C_CCR_IS;
		I2CMasterState = I2CSTATE_SLA_NACK;
		break;

	case 0x50:
		/*
		 * Data byte has been received; ACK has been returned.
		 * Read the byte and check for more bytes to read.
		 * Send a NOT ACK after the last byte is received
		 */
		I2CSlaveBuffer[RdIndex++] = I2Cx->DR;
		if ( RdIndex < (I2CReadLength-1) )
		{
			/* lmore bytes to follow: send an ACK after data is received */
			I2Cx->CSR = I2C_CSR_ACK;
		}
		else
		{
			/* last byte: send a NACK after data is received */
			I2Cx->CCR = I2C_CCR_ACK;
		}
		I2Cx->CCR = I2C_CCR_IS;
		break;
	
	case 0x58:	
		/*
		 * Data byte has been received; NOT ACK has been returned.
		 * This is the last byte to read.
		 * Generate a STOP condition and flag the I2CEngine that the
		 * transaction is finished.
		 */
		I2CSlaveBuffer[RdIndex++] = I2Cx->DR;
		I2CMasterState = I2CSTATE_ACK;
	  /* Set Stop flag */
		I2Cx->CSR = I2C_CSR_STOP;	
	  /* Clear SI flag */
		I2Cx->CCR = I2C_CCR_IS;	
		break;        
  /* Slave Mode */
  /* An own SLA_W has been received. */		
	case 0x60:					
	case 0x68:
	RdIndex = 0;
  /* Assert ACK after SLV_W is received */	
	I2Cx->CSR = I2C_CSR_ACK;	
	I2Cx->CCR = I2C_CCR_IS;
	I2CSlaveState = I2C_WR_STARTED;
	break;

  case 0x70:
	case 0x78:
	RdIndex = 0;
	I2CSlaveState = I2C_BROADCASTED;
	/* Assert ACK after data is received */
	I2Cx->CSR = I2C_CSR_ACK;	
	I2Cx->CCR = I2C_CCR_IS;
	break;
	
	
	case 0x80:					
  /*  Data receive */		
	if ( I2CSlaveState == I2C_WR_STARTED )
	{
	  I2CRdBuffer[RdIndex++] = I2Cx->DR;
		/* Assert ACK after data is received */
	  I2Cx->CSR = I2C_CSR_ACK;	
	}
	else
	{
		/* Assert NACK */
	  I2Cx->CCR = I2C_CCR_ACK;	
	}
	I2Cx->CCR = I2C_CCR_IS;
	break;
	
	case 0x90:					
	case 0x98:
	if ( I2CSlaveState == I2C_BROADCASTED )
	{
	  I2CRdBuffer[RdIndex++] = I2Cx->DR;
		/* Assert NACK */
	  I2Cx->CCR = I2C_CCR_ACK;	
	}

	I2Cx->CCR = I2C_CCR_IS;
	break;

  /* Stop condition or repeated start has  been received */	
	case 0xA0:					
  /* Assert ACK.  */
	I2Cx->CSR = I2C_CSR_ACK;	
	I2Cx->CCR = I2C_CCR_IS;
	I2CSlaveState = I2C_IDLE;
	break;	
			
	case 0xA8:
  /* An own SLA_R has been received. */		
	case 0xB0:
	WrIndex = 0;	
	RdIndex = 0;
	/* Assert ACK after SLV_R is received */
	I2Cx->CSR = I2C_CSR_ACK;	
	I2CSlaveState = I2C_RD_STARTED;
  I2Cx->DR = I2CRdBuffer[WrIndex++];/* write the same data back to master */
	I2Cx->CCR = I2C_CCR_IS;
	break;
  /* Data byte has been transmitted */ 	
	case 0xB8:					
	case 0xC8:

	if ( I2CSlaveState == I2C_RD_STARTED )
	{
	  I2Cx->DR = I2CRdBuffer[WrIndex++];/* write the same data back to master */	 
		/* Assert ACK  */
	  I2Cx->CSR = I2C_CSR_ACK;		
	}
	else
	{
		/* Assert NACK  */
	  I2Cx->CCR = I2C_CCR_ACK;		
	}	
	I2Cx->CCR = I2C_CCR_IS;
	break;

	case 0xC0:					
  /* Data byte has been transmitted, NACK */
	/* Assert NACK  */
	I2Cx->CCR = I2C_CCR_ACK;			
	I2Cx->CCR = I2C_CCR_IS;
	I2CSlaveState = DATA_NACK;
	/* assert ACK */
	I2Cx->CSR = I2C_CSR_ACK;			
	WrIndex = 0x0;
	break;
	default:
	I2Cx->CCR = I2C_CCR_IS;
	
  if (_I2cMode == I2CSLAVE)
  {
    I2Cx->CSR = I2C_CSR_EN;
   }
  break;
  }

}

/**
  * @brief  Create I2C start condition, a timeout value is set if the I2C never gets started,
  *         and timed out. It's a fatal error.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2C automatic end mode.
  *          This parameter can be: ENABLE or DISABLE.
  * @note   This function has effect if Reload mode is disabled.
  * @retval None
  */

uint32_t I2C_StartHandling(CMSDK_I2C_TypeDef* I2Cx)
{
  uint32_t timeout = 0;
  
  /*--- Issue a start condition ---*/
  /* Set Start flag and enable I2C peripheral.*/
  I2Cx->CSR = I2C_CSR_EN | I2C_CSR_START; 
	
  while((I2CMasterState != I2CSTATE_PENDING) && (timeout < MAX_TIMEOUT))
	{
    timeout++;	
	}
  return (timeout < MAX_TIMEOUT);
 }

/**
  * @brief  Create I2C stop condition, a timeout value is set if the I2C never gets started,
  *         and timed out. It's a fatal error.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2C automatic end mode.
  *          This parameter can be: ENABLE or DISABLE.
  * @note   This function has effect if Reload mode is disabled.
  * @retval None
  */
uint32_t I2C_StopHandling(CMSDK_I2C_TypeDef* I2Cx)
{
  uint32_t timeout = 0;
  /* Set Stop flag */
  I2Cx->CSR = I2C_CSR_STOP;	
	/* Clear SI flag */
  I2Cx->CCR = I2C_CCR_IS;
  /*--- Wait for STOP detected ---*/
  while((I2Cx->CSR & I2C_CSR_STOP) && (timeout < MAX_TIMEOUT))
	{
    timeout++;	
	}
  return (timeout >= MAX_TIMEOUT);
} 

void i2cInit(CMSDK_I2C_TypeDef* I2Cx ,uint32_t I2CMode ,IRQn_Type I2Cx_IRQn)
{

  _I2cMode = I2CMode;
	/* Clear all flag */
	I2Cx->CCR = I2C_CCR_ACK | I2C_CCR_IS | I2C_CCR_START | I2C_CCR_EN;
  if (_I2cMode == I2CSLAVE)
  {
    I2Cx->OAR = SLAVE_ADDR;
    I2CSlaveState = I2C_IDLE;
  }
  /* Enable the I2C Interrupt */	
  NVIC_EnableIRQ(I2Cx_IRQn);
  /* Enable the I2C peripheral */	
  I2Cx->CSR  = I2C_CSR_EN;

}


/**
  * @brief  The routine to complete a I2C transaction from start to stop. All the intermitten
  *         steps are handled in the interrupt handler.Before this routine is called, the read
  *         Before this routine is called, the read length, write length and I2C master buffer
  *         need to be filled.    
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.       
  * @retval Any of the I2CSTATE
  */
uint32_t i2cEngine(CMSDK_I2C_TypeDef* I2Cx) 
{
	
	uint32_t timeout0 = 0;
	
  I2CMasterState = I2CSTATE_IDLE;
  RdIndex = 0;
  WrIndex = 0;
  if ( I2C_StartHandling(I2Cx) != TRUE )
  {
	I2C_StopHandling(I2Cx);			
	return ( I2C_START_FAIL );
  }

  /* wait until the state is a terminal state */
  while((I2CMasterState < 0x100) && (timeout0 < MAX_TIMEOUT))
	{
    timeout0++;		
	}
  if(timeout0==MAX_TIMEOUT)
	{
    return ( I2C_START_FAIL );
	}
		
  return ( I2CMasterState );
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


