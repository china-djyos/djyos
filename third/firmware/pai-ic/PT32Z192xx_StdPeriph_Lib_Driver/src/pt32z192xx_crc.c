#if 0       //lst dbg

/**
  ******************************************************************************
  * @file    pt32z192xx_crc.c
  * @author  MCD Application Team
  * @version
  * @date   
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of CRC computation unit peripheral:
  *            + Configuration of the CRC computation unit
  *            + CRC computation of one/many 32-bit data
  *            + CRC Independent register (IDR) access
  *
  *  @verbatim
 ===============================================================================
                     ##### How to use this driver #####
 ===============================================================================
    [..]
    
         (+) Enable CRC AHB clock using RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE)
             function
         (+) If required, select the reverse operation on input data 
             using CRC_ReverseInputDataSelect()  
         (+) If required, enable the reverse operation on output data
             using CRC_ReverseOutputDataCmd(Enable)
         (+) use CRC_CalcCRC() function to compute the CRC of a 32-bit data
             or use CRC_CalcBlockCRC() function to compute the CRC if a 32-bit 
             data buffer
            (@) To compute the CRC of a new data use CRC_ResetDR() to reset
                 the CRC computation unit before starting the computation
                 otherwise you can get wrong CRC values.
      
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
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pt32z192xx_crc.h"

/** @addtogroup PT32F0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup CRC 
  * @brief CRC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CRC_CR_MASK                    0xFFFFFFFC
#define CRC_CCITT_POLY                 0x00001021           // 0x1021
#define CRC_16_POLY                    0x00008005           // 0x8005

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup CRC_Private_Functions
  * @{
  */

/** @defgroup CRC_Group1 Configuration of the CRC computation unit functions
 *  @brief   Configuration of the CRC computation unit functions 
 *
@verbatim
 ===============================================================================
                     ##### CRC configuration functions #####
 ===============================================================================

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
void CRC_Init(CRC_InitTypeDef* CRC_InitStruct)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_CRC_DINWIDTH(CRC_InitStruct->CRC_DIN_Width));
	assert_param(IS_CRC_SEED(CRC_InitStruct->CRC_Seed));
  assert_param(IS_FUNCTIONAL_STATE(CRC_InitStruct->Bit_Input_Reverse));
	assert_param(IS_FUNCTIONAL_STATE(CRC_InitStruct->Byte_Input_Reverse));
	assert_param(IS_FUNCTIONAL_STATE(CRC_InitStruct->Bit_Output_Reverse));
	assert_param(IS_CRC_POLY(CRC_InitStruct->CRC_Poly));
  /*  */
  tmpreg = CRC->CR;
  /*  */
  tmpreg &= (uint32_t)~(uint32_t)CRC_CR_MASK;
  /*  */
  tmpreg  |= ((uint32_t)((CRC_InitStruct->CRC_DIN_Width)));
	tmpreg  |= ((uint32_t)((CRC_InitStruct->Bit_Input_Reverse)<<0x03));
	tmpreg  |= ((uint32_t)((CRC_InitStruct->Byte_Input_Reverse)<<0x04));
	tmpreg  |= ((uint32_t)((CRC_InitStruct->Bit_Output_Reverse)<<0x05));	
  /* Write to CRC CR */
  CRC->CR = tmpreg;
	
	CRC->SDR = CRC_InitStruct->CRC_Seed;
	
	CRC->POLY = CRC_InitStruct->CRC_Poly;
	

}

/**
  * @brief  Enables or disables the specified CRC peripheral.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  NewState: new state of the SPIx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CRC_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected SPI peripheral */
    CRC->CR |= CRC_CR_EN;
  }
  else
  {
    /* Disable the selected SPI peripheral */
    CRC->CR &= (uint16_t)~((uint16_t)CRC_CR_EN);
  }
}

/**
  * @brief  Enables or disables the specified CRC peripheral.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  NewState: new state of the SPIx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CRC_ReloadSeed(void)
{

    CRC->CR |= CRC_CR_INIT;
	
}





/**
  * @brief  Get the CRC-CCITT checksum from the given data
  * @param  seed: CRC initial data
  * @param  buffer: pointer to the given data to be calculated
  * @retval The checksum value
  */
uint16_t CRC_CCITT(uint16_t seed, uint8_t *buffer, uint32_t length)
{
  /* CRC-CCITT poly: 0x1021 */
  CRC->SDR = seed;
  CRC->POLY = CRC_CCITT_POLY;
  CRC->CR |= CRC_CR_EN | CRC_CR_INIT;
  while (length--)
  {
    wb(&CRC->DIN, *buffer++); // byte write
  }

  return (uint16_t)(CRC->DOUT);
}

/**
  * @brief  Get the CRC-CCITT checksum from the given data
  * @param  seed: CRC initial data
  * @param  buffer: pointer to the given data to be calculated
  * @retval The checksum value
  */
uint16_t CRC_16(uint16_t seed, uint8_t *buffer, uint32_t length)
{
  /* CRC-16 poly: 0x8005 */
  CRC->SDR = seed;
  CRC->POLY = CRC_16_POLY;
  CRC->CR |= CRC_CR_EN | CRC_CR_INIT;
  while (length--)
  {
    wb(&CRC->DIN, *buffer++); // byte write
  }

  return (uint16_t)(CRC->DOUT);
}

/**
  * @brief  Enables or disable the reverse operation on output data.
  *         The reverse operation on output data is performed on 32-bit.
  * @param  NewState: new state of the reverse operation on output data.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CRC_ReverseOutputDataBitCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable reverse operation on output data */
    CRC->CR |= CRC_CR_OUTPUT_BITREV;
  }
  else
  {
    /* Disable reverse operation on output data */
    CRC->CR &= (uint32_t)~((uint32_t)CRC_CR_OUTPUT_BITREV);
  }
}

/**
  * @brief  Enables or disable the reverse operation on output data.
  *         The reverse operation on output data is performed on 32-bit.
  * @param  NewState: new state of the reverse operation on output data.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CRC_ReverseInputDataBitCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable reverse operation on output data */
    CRC->CR |= CRC_CR_INPUT_BITREV;
  }
  else
  {
    /* Disable reverse operation on output data */
    CRC->CR &= (uint32_t)~((uint32_t)CRC_CR_INPUT_BITREV);
  }
}

/**
  * @brief  Enables or disable the reverse operation on output data.
  *         The reverse operation on output data is performed on 32-bit.
  * @param  NewState: new state of the reverse operation on output data.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CRC_ReverseInputDataCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable reverse operation on output data */
    CRC->CR |= CRC_CR_INPUT_BYTEREV;
  }
  else
  {
    /* Disable reverse operation on output data */
    CRC->CR &= (uint32_t)~((uint32_t)CRC_CR_INPUT_BYTEREV);
  }
}


/**
  * @brief  Initializes the INIT register.
  * @note   After resetting CRC calculation unit, CRC_InitValue is stored in DR register
  * @param  CRC_InitValue: Programmable initial CRC value
  * @retval None
  */
void CRC_Seed(uint32_t CRC_Seed)
{
  CRC->SDR = CRC_Seed;
}

/**
  * @brief  Initializes the polynomail coefficients.
  * @param  CRC_Pol: Polynomial to be used for CRC calculation.
  * @retval None
  */
void CRC_SetPolynomial(uint32_t CRC_Pol)
{
  CRC->POLY = CRC_Pol;
}

/**
  * @}
  */

/** @defgroup CRC_Group2 CRC computation of one/many 32-bit data functions
 *  @brief   CRC computation of one/many 32-bit data functions
 *
@verbatim
 ===============================================================================
                     ##### CRC computation functions #####
 ===============================================================================

@endverbatim
  * @{
  */


/**
  * @brief  Computes the 16-bit CRC of a given 16-bit data. This function is only 
  * @param  CRC_Data: data half-word(16-bit) to compute its CRC
  * @retval 16-bit CRC
  */
uint32_t CRC_CalcCRC16bits(uint16_t CRC_Data)
{
  *(uint16_t*)(CMSDK_CRC_BASE+0x0C) = (uint16_t) CRC_Data;
  
  return (CRC->DOUT);
}

/**
  * @brief  Computes the 8-bit CRC of a given 8-bit data. This function is only 
  *         applicable for STM32F072 devices.
  * @param  CRC_Data: 8-bit data to compute its CRC
  * @retval 8-bit CRC
  */
uint32_t CRC_CalcCRC8bits(uint8_t CRC_Data)
{
  *(uint8_t*)(CMSDK_CRC_BASE+0x0C) = (uint8_t) CRC_Data;

  return (CRC->DOUT);
}

/**
  * @brief  Computes the 32-bit CRC of a given buffer of data word(32-bit).
  * @param  pBuffer: pointer to the buffer containing the data to be computed
  * @param  BufferLength: length of the buffer to be computed
  * @retval 32-bit CRC
  */
uint32_t CRC_CalcBlockCRC(uint16_t pBuffer[], uint32_t BufferLength)
{
  uint32_t index = 0;
  
  for(index = 0; index < BufferLength; index++)
  {
  *(uint16_t*)(CMSDK_CRC_BASE+0x0C) = pBuffer[index];
  }
  return (CRC->DOUT);
}



/**
  * @brief  Returns the current CRC value.
  * @param  None
  * @retval 32-bit CRC
  */
uint32_t CRC_GetCRC(void)
{
  return (CRC->DOUT);
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

/************************ (C) *****END OF FILE****/

#endif          //lst dbg
