/**
  ******************************************************************************
  * @file    pt32z192xx_flash.c
  * @author  应用开发团队
  * @version 
  * @date    
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the FLASH peripheral:
  *            - FLASH Interface configuration
  *            - FLASH Memory Programming
  *            - Interrupts and flags management
  *
  *  @verbatim
 ===============================================================================
                    ##### How to use this driver #####
 ===============================================================================
    [..] This driver provides functions to configure and program the Flash 
         memory of all STM32F0xx devices. These functions are split in 4 groups
         (#) FLASH Interface configuration functions: this group includes the 
             management of following features:
             (++) Set the latency

         (#) FLASH Memory Programming functions: this group includes all needed 
             functions to erase and program the main memory:
             (++) Erase function: Erase Page, erase all pages.
             (++) Program functions: Half Word and Word write.

         (#) FLASH Option Bytes Programming functions: this group includes all 
             needed functions to:
             (++) Lock and Unlock the Flash Option bytes.
             (++) Launch the Option Bytes loader
             (++) Erase the Option Bytes
             (++)Set/Reset the write protection
             (++) Set the Read protection Level
             (++) Program the user option Bytes
             (++) Set/Reset the BOOT1 bit
             (++) Enable/Disable the VDDA Analog Monitoring
             (++) Get the user option bytes
             (++) Get the Write protection
             (++) Get the read protection status

         (#) FLASH Interrupts and flag management functions: this group includes 
             all needed functions to:
             (++) Enable/Disable the flash interrupt sources
             (++) Get flags status
             (++) Clear flags
             (++) Get Flash operation status
             (++) Wait for last flash operation

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
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx_flash.h"
#include <stdio.h>
//#include <main.h>         //lst dbg
/** @addtogroup PT32F0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup FLASH 
  * @brief FLASH driver modules
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
 
/** @defgroup FLASH_Private_Functions
  * @{
  */ 

/** @defgroup FLASH_Group1 FLASH Interface configuration functions
  *  @brief   FLASH Interface configuration functions 
 *
@verbatim   
 ===============================================================================
               ##### FLASH Interface configuration functions #####
 ===============================================================================

    [..] FLASH_Interface configuration_Functions, includes the following functions:
       (+) void FLASH_SetLatency(uint32_t FLASH_Latency):
    [..] To correctly read data from Flash memory, the number of wait states (LATENCY) 
     must be correctly programmed according to the frequency of the CPU clock (HCLK) 
    [..]
        +--------------------------------------------- +
        |  Wait states  |   HCLK clock frequency (MHz) |
        |---------------|------------------------------|
        |0WS(1CPU cycle)|       0 < HCLK <= 24         |
        |---------------|------------------------------|
        |1WS(2CPU cycle)|       24 < HCLK <= 48        |
        +----------------------------------------------+
    [..]
       (+) void FLASH_PrefetchBufferCmd(FunctionalState NewState);
    [..]
     All these functions don't need the unlock sequence.

@endverbatim
  * @{
  */

/**
  * @brief  Sets the code latency value.
  * @param  FLASH_Latency: specifies the FLASH Latency value.
  *          This parameter can be one of the following values:
  *             @arg FLASH_Latency_0: FLASH 0 Latency cycle
  *             @arg FLASH_Latency_1: FLASH 1 Latency cycle
  *             @arg FLASH_Latency_2: FLASH 2 Latency cycle
  *             @arg FLASH_Latency_3: FLASH 3 Latency cycle
  *             @arg FLASH_Latency_4: FLASH 4 Latency cycle
  *             @arg FLASH_Latency_5: FLASH 5 Latency cycle
  *             @arg FLASH_Latency_6: FLASH 6 Latency cycle
  *             @arg FLASH_Latency_7: FLASH 7 Latency cycle	
  * @retval None
  */
void FLASH_SetLatency(uint32_t FLASH_Latency)
{
   uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_FLASH_LATENCY(FLASH_Latency));

  /* Read the ACR register */
  tmpreg = CMSDK_FLASH->ACR;

  /* Sets the Latency value */
  tmpreg &= (uint32_t) (~((uint32_t)FLASH_ACR_LATENCY));
  tmpreg |= FLASH_Latency;

  /* Write the ACR register */
  CMSDK_FLASH->ACR = ( FLASH_FKEY1 | tmpreg );
}


/**
  * @brief  Enables or disables Flash Cache.
  * @param  NewState: new state of the Flash Cache . 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void FlashCache_Cmd(FunctionalState NewState)
{
  uint32_t tmpreg = 0;	
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
	
  tmpreg = CMSDK_FLASH->ACR;

  tmpreg &= ~ FLASH_ACR_CACHE_EN;	
	
  if (NewState != DISABLE)
  {
    /* Enable the Flash Cache */
		tmpreg |= FLASH_ACR_CACHE_EN;
		
    CMSDK_FLASH->ACR = ( FLASH_FKEY1 | tmpreg );
  }
  else
  {
    /* Disable the Flash Cache */
     CMSDK_FLASH->ACR = ( FLASH_FKEY1 | tmpreg );
  }
}



/**
  * @}
  */

/** @defgroup FLASH_Group2 FLASH Memory Programming functions
 *  @brief   FLASH Memory Programming functions
 *
@verbatim   
 ===============================================================================
                ##### FLASH Memory Programming functions #####
 ===============================================================================

    [..] The FLASH Memory Programming functions, includes the following functions:
       (+) FLASH_Status FLASH_ErasePage(uint32_t Page_Address);
       (+) FLASH_Status FLASH_EraseAllPages(void);
       (+) FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data);
       (+) FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data);

    [..] Any operation of erase or program should follow these steps:
       (#) Call the desired function to erase page or program data

@endverbatim
  * @{
  */
	
/**
  * @brief  Erases a specified page in program memory.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access (recommended
  *         to protect the FLASH memory against possible unwanted operation)
  * @param  Page_Address: The page address in program memory to be erased.
  * @note   A Page is erased in the Program memory only if the address to load 
  *         is the start address of a page (multiple of 1024 bytes).
  * @retval FLASH Status: The returned value can be: 
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ErasePage(uint32_t Page_Address)
{	
  uint32_t tmpreg = 0;	
	
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters */
  assert_param(IS_FLASH_PROGRAM_ADDRESS(Page_Address));
 
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
  
  tmpreg = CMSDK_FLASH->ACR;

  tmpreg &= ~ (FLASH_OPMOD_WR | FLASH_ACR_CMD | FLASH_OPSTR_STRT);	
	
	tmpreg |= (FLASH_OPMOD_WR|FLASH_OPCMD_PERA|FLASH_OPSTR_STRT);
	
  if(status == FLASH_COMPLETE)
  { 
    CMSDK_FLASH->AR  = Page_Address;
		if(((Page_Address) <= MAINCODE_END_ADDR))
		{
      /* The main code area is erased by page */
		  CMSDK_FLASH->ACR = (FLASH_FKEY1|tmpreg);
      status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);  			
		}

		else
		{		
        status = FLASH_ADDR_ERR; 		
		}
  }
    
  /* Return the Erase Status */
  return status;
}

/**
  * @brief  Erases all FLASH pages.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access (recommended
  *         to protect the FLASH memory against possible unwanted operation)
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseAllPages(void)
{
	
  uint32_t tmpreg = 0;	
	
  FLASH_Status status = FLASH_COMPLETE;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
 
  tmpreg = CMSDK_FLASH->ACR;

  tmpreg &= ~ (FLASH_OPMOD_WR | FLASH_ACR_CMD | FLASH_OPSTR_STRT);	
	
	tmpreg |= (FLASH_OPMOD_WR|FLASH_OPCMD_MERA|FLASH_OPSTR_STRT);

	
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to erase all pages */
		CMSDK_FLASH->ACR = (FLASH_FKEY1|tmpreg);	
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

  }

  /* Return the Erase Status */
  return status;
}


/**
  * @brief  Programs a word at a specified address.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access (recommended
  *         to protect the FLASH memory against possible unwanted operation)
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data0,uint32_t Data1)
{
  uint32_t tmpreg = 0;	
	
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters */
  assert_param(IS_FLASH_PROGRAM_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
	
     if((status ==FLASH_COMPLETE)&&((Address)<= MAINCODE_END_ADDR)&&((Address&0x07)==0))
	{
  tmpreg = CMSDK_FLASH->ACR;

  tmpreg &= ~ (FLASH_OPMOD_WR | FLASH_ACR_CMD | FLASH_OPSTR_STRT);	
	
	tmpreg |= (FLASH_OPMOD_WR|FLASH_OPCMD_PRO|FLASH_OPSTR_STRT);
	
  CMSDK_FLASH->AR  = Address;
  CMSDK_FLASH->DR0  = Data0;
	CMSDK_FLASH->DR1  = Data1;	

  CMSDK_FLASH->ACR = (FLASH_FKEY1|tmpreg);
	
		while(!(CMSDK_FLASH->ISR&0x01));    //等待写操作命令完成标志位
		   CMSDK_FLASH->ISR |=0x01;      //清除标志位
  }	

  status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);  	


//  if(status ==FLASH_COMPLETE)
//  { 
//    /* if the previous operation is completed, proceed to erase all pages */
//    CMSDK_FLASH->AR  = Address;
//		CMSDK_FLASH->DR0  = Data0;
//		CMSDK_FLASH->DR1  = Data1;		
//		if(((Address) <= MAINCODE_END_ADDR)&&((Address&0x07)==0))
//		{
//      /* Data is programming into the main code area */
//		  CMSDK_FLASH->ACR = (FLASH_FKEY1|tmpreg);
//      status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);  			
//		}
//		  else
//		  {
//        status = FLASH_ADDR_ERR; 		
//		  }      			
//		}
//  }

  /* Return the Program Status */
  return status;
}

/**
  * @}
  */

/** @defgroup FLASH_Group2 FLASH Memory Reading functions
 *  @brief   FLASH Memory Reading functions
 *
@verbatim   
 ===============================================================================
                ##### FLASH Memory Reading functions #####
 ===============================================================================

    [..] The FLASH Memory Programming functions, includes the following functions:
       (+) uint32_t FLASH_ReadWord(uint32_t Address);
       (+) uint16_t FLASH_ReadHalfWord(uint32_t Address);
       (+) uint8_t FLASH_ReadByte(uint32_t Address);

@endverbatim
  * @{
  */
	
/**
  * @brief  Read a word at a specified address.
  * @note   
  * @note   
  *         
  * @param  
  * @note   
  *         
  * @retval 
  *         
  */
uint32_t FLASH_ReadWord(uint32_t Address)
{
  return *(__IO uint32_t*) Address;
}
/**
  * @brief  Read a half word at a specified address.
  * @note   
  * @note   
  *        
  * @param  
  * @param  
  * @retval 
  *         
  */
uint16_t FLASH_ReadHalfWord(uint32_t Address)
{
  return *(__IO uint16_t*) Address;
}

/**
  * @brief Read a Byte at a specified address. 
  * @note   
  * @note  
  *        
  * @param  
  * @param  
  * @retval 
  *         
  */
uint8_t FLASH_ReadByte(uint32_t Address)
{
  return *(__IO uint8_t*) Address;
}

/**
  * @}
  */ 
/** @defgroup FLASH_Group4 Interrupts and flags management functions
 *  @brief   Interrupts and flags management functions
 *
@verbatim   
 ===============================================================================
             ##### Interrupts and flags management functions #####
 ===============================================================================  

@endverbatim
  * @{
  */

/**
  * @brief  Enables or disables the specified FLASH interrupts.
  * @param  FLASH_IT: specifies the FLASH interrupt sources to be enabled or 
  *         disabled.
  *          This parameter can be any combination of the following values:
  *             @arg FLASH_IER_WREND      : FLASH End of CMD Interrupt
  *             @arg FLASH_IER_CMDER      : FLASH CMD Error Interrupt
  *             @arg FLASH_IER_ADER       : FLASH ADDR Error Interrupt
  *             @arg FLASH_IER_CACHE_HIT  : FLASH Cache Hit Counter Full Interrupt
  * @retval None 
  */
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FLASH_IT(FLASH_IT)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if(NewState != DISABLE)
  {
    /* Enable the interrupt sources */
    CMSDK_FLASH->IER |= FLASH_IT;
  }
  else
  {
    /* Disable the interrupt sources */
    CMSDK_FLASH->IER &= ~(uint32_t)FLASH_IT;
  }
}

/**
  * @brief  Checks whether the specified FLASH flag is set or not.
  * @param  FLASH_FLAG: specifies the FLASH flag to check.
  *          This parameter can be one of the following values:
  *             @arg FLASH_FLAG_WREND      : FLASH write/erase operations in progress flag 
  *             @arg FLASH_FLAG_CMDER      : FLASH CMD Errorr flag
  *             @arg FLASH_FLAG_ADER       : FLASH ADDR Error flag
  *             @arg FLASH_FLAG_CACHE_HIT  : FLASH Cache Hit Counter Full flag
  * @retval The new state of FLASH_FLAG (SET or RESET).
  */
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters */
  assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG));

  if((CMSDK_FLASH->ISR & FLASH_FLAG) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* Return the new state of FLASH_FLAG (SET or RESET) */
  return bitstatus; 
}

/**
  * @brief  Clears the FLASH's pending flags.
  * @param  FLASH_FLAG: specifies the FLASH flags to clear.
  *          This parameter can be any combination of the following values:
  *             @arg FLASH_FLAG_WREND      : FLASH write/erase operations in progress flag 
  *             @arg FLASH_FLAG_CMDER      : FLASH CMD Errorr flag
  *             @arg FLASH_FLAG_ADER       : FLASH ADDR Error flag
  *             @arg FLASH_FLAG_CACHE_HIT  : FLASH Cache Hit Counter Full flag
  * @retval None
  */
void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
  /* Check the parameters */
  assert_param(IS_FLASH_CLEAR_FLAG( FLASH_FLAG));
  
  /* Clear the flags */
  CMSDK_FLASH->ISR = FLASH_FLAG;
}

/**
  * @brief  Returns the FLASH Status.
  * @param  None
  * @retval FLASH Status: The returned value can be: 
  *         FLASH_BUSY, FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP or FLASH_COMPLETE.
  */
FLASH_Status FLASH_GetStatus(void)
{
  FLASH_Status FLASHstatus = FLASH_COMPLETE;
  
  if((CMSDK_FLASH->ACR & FLASH_ACR_START) == FLASH_ACR_START) 
  {
    FLASHstatus = FLASH_BUSY;
  }
  else 
  {  
    if((CMSDK_FLASH->ISR & (uint32_t)(FLASH_FLAG_CMDER)) != (uint32_t)0x00)
    { 
      FLASHstatus = FLASH_CMD_ERR;
    }
    else 
    {
      if((CMSDK_FLASH->ISR & (uint32_t)(FLASH_FLAG_ADER)) != (uint32_t)0x00)
      {
        FLASHstatus = FLASH_ADDR_ERR; 
      }
      else
      {
        FLASHstatus = FLASH_COMPLETE;
      }
    }
  }
  /* Return the FLASH Status */
  return FLASHstatus;
}


/**
  * @brief  Waits for a FLASH operation to complete or a TIMEOUT to occur.
  * @param  Timeout: FLASH programming Timeout
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, 
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout)
{ 
  FLASH_Status status = FLASH_COMPLETE;
   
  /* Check for the FLASH Status */
  status = FLASH_GetStatus();
  
  /* Wait for a FLASH operation to complete or a TIMEOUT to occur */
  while((status == FLASH_BUSY) && (Timeout != 0x00))
  {
    status = FLASH_GetStatus();
    Timeout--;
  }
  
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* Return the operation status */
  return status;
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
