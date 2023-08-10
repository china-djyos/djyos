/**
  ******************************************************************************
  * @file    pt32z192xx_DMA.c
  * @author  MCD Application Team
  * @version
  * @date   
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of DMA computation unit peripheral:
  *            + Configuration of the DMA computation unit
  *            + DMA computation of one/many 32-bit data
  *            + DMA Independent register (IDR) access
  *
  *  @verbatim
 ===============================================================================
                     ##### How to use this driver #####
 ===============================================================================
    [..]
    
         (+) Enable DMA AHB clock using RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA, ENABLE)
             function
         (+) If required, select the reverse operation on input data 
             using DMA_ReverseInputDataSelect()  
         (+) If required, enable the reverse operation on output data
             using DMA_ReverseOutputDataCmd(Enable)
         (+) use DMA_CalcDMA() function to compute the DMA of a 32-bit data
             or use DMA_CalcBlockDMA() function to compute the DMA if a 32-bit 
             data buffer
            (@) To compute the DMA of a new data use DMA_ResetDR() to reset
                 the DMA computation unit before starting the computation
                 otherwise you can get wrong DMA values.
      
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
#include "pt32z192xx_dma.h"

/** @addtogroup PT32F0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup DMA 
  * @brief DMA driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup DMA_Private_Functions
  * @{
  */

/** @defgroup DMA_Group1 Configuration of the DMA computation unit functions
 *  @brief   Configuration of the DMA computation unit functions 
 *
@verbatim
 ===============================================================================
                     ##### DMA configuration functions #####
 ===============================================================================

@endverbatim
  * @{
  */
/**
  * @brief  Configure specific DMA channel.
  * @param  DMA_Ch: DMA_CH0 ~ DMA_CH3.
  * @param  DMACH_InitStruct: pointer to DMACH_InitTypeDef structure that contains the configuration
  * @retval None
  */
void DMA_Config(uint32_t DMA_Ch, DMACH_InitTypeDef *DMACH_InitStruct)
{
  uint32_t tmpreg = 0;
  CMSDK_DMACH_TypeDef *DMACHx = (CMSDK_DMACH_TypeDef *)(CMSDK_DMA_BASE + DMA_Ch*0x20);

  /* Check the parameters                                                                                   */
  assert_param(IS_DMA_CH(DMA_Ch));
  assert_param(IS_DMA_WIDTH(DMACH_InitStruct->DMACH_SrcDataSize));
  assert_param(IS_DMA_WIDTH(DMACH_InitStruct->DMACH_DstDataSize));	
  assert_param(IS_DMA_BLK_CNT(DMACH_InitStruct->DMACH_BlkCnt));
  assert_param(IS_FUNCTIONAL_STATE(DMACH_InitStruct->DMACH_SrcAdrInc));
  assert_param(IS_FUNCTIONAL_STATE(DMACH_InitStruct->DMACH_DstAdrInc));


	
  /* DMA Channel x Configuration */
	
  DMACHx->SADR   = DMACH_InitStruct->DMACH_SrcAddr;
	
	
  DMACHx->DADR   = DMACH_InitStruct->DMACH_DstAddr;	

  DMACHx->BLKCNT = DMACH_InitStruct->DMACH_BlkCnt;

  /* Get the DMACHx CTL value */
  tmpreg = DMACHx->CTL;	
	
  /* Clear Configuration bits */
  tmpreg &= ~(DMACH_CTL_SINC | DMACH_CTL_DINC | DMACH_CTL_SSIZE | DMACH_CTL_DSIZE);	

  tmpreg |= (((DMACH_InitStruct->DMACH_SrcDataSize) << 3) | \
	           ((DMACH_InitStruct->DMACH_DstDataSize) << 7) | \
             ((DMACH_InitStruct->DMACH_SrcAdrInc) << 1) 	| \
						 ((DMACH_InitStruct->DMACH_DstAdrInc) << 2));
						 
	DMACHx->CTL = tmpreg;
}

/**
  * @brief  Enables or disables the specified DMA peripheral.
  * @param  NewState: new state of the DMA peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DMA_Cmd(uint32_t DMA_Ch,FunctionalState NewState)
{
  CMSDK_DMACH_TypeDef *DMACHx = (CMSDK_DMACH_TypeDef *)(CMSDK_DMA_BASE + DMA_Ch*0x20);	
  /* Check the parameters */
  assert_param(IS_DMA_CH(DMA_Ch));	
  assert_param(IS_FUNCTIONAL_STATE(NewState));

	
	DMACHx->CTL &= (uint32_t)~((uint32_t)DMACH_CTL_EN);	
	
  if (NewState != DISABLE)
  {
    /* Enable the selected DMA_CH peripheral */
    DMACHx->CTL |= DMACH_CTL_EN;
  }
  else
  {
    /* Disable the selected DMA_CH peripheral */
  	DMACHx->CTL &= (uint32_t)~((uint32_t)DMACH_CTL_EN);	
  }
}


/**
  * @brief  Enables or disables DMA Memory to Memory Transmission.
  * @param  NewState: new state of the DMA Memory to Memory Transmissionl. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DMA_M2MTransmit_Cmd(uint32_t DMA_Ch,FunctionalState NewState)
{
  CMSDK_DMACH_TypeDef *DMACHx = (CMSDK_DMACH_TypeDef *)(CMSDK_DMA_BASE + DMA_Ch*0x20);	
  /* Check the parameters */
  assert_param(IS_DMA_CH(DMA_Ch));	
  assert_param(IS_FUNCTIONAL_STATE(NewState));

	
	DMACHx->CTL &= (uint32_t)~((uint32_t)DMACH_CTL_M2M);	
	
  if (NewState != DISABLE)
  {
    /* Enable Memory to Memory Transmission */
    DMACHx->CTL |= DMACH_CTL_M2M;
  }
  else
  {
    /* Disable Memory to Memory Transmission */
  	DMACHx->CTL &= (uint32_t)~((uint32_t)DMACH_CTL_M2M);	
  }
}

/**
  * @brief  Config DMA Memory and Peripheral Transmission Direction.
  * @param  DMA_Ch  : DMA_CH0 ~ DMA_CH3.
  * @param  Dirction: Config DMA Memory and Peripheral Transmission Direction. 
  *          This parameter can be: Peripheral2Memory
  *                               : Memory2Peripheral
  * @retval None
  */
void DMA_DirectionConfig(uint32_t DMA_Ch,uint32_t Dirction)
{
	CMSDK_DMACH_TypeDef *DMACHx = (CMSDK_DMACH_TypeDef *)(CMSDK_DMA_BASE + DMA_Ch*0x20);	
	
  /* Check the parameters */
  assert_param(IS_DMA_CH(DMA_Ch));	
  assert_param(IS_DMA_DIRECTION(Dirction));


	DMACHx->CTL &= (uint32_t)~((uint32_t)DMACH_CTL_DIR);	
	
  DMACHx->CTL |= Dirction;

}

/**
  * @brief  Enables or disables DMA Auto Repeat Transmit Function.
  * @param  DMA_Ch  : DMA_CH0 ~ DMA_CH3.
  * @param  NewState: new state of the DMA Auto Repeat Transmit. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DMA_AutoRepeat_Cmd(uint32_t DMA_Ch,FunctionalState NewState)
{
	CMSDK_DMACH_TypeDef *DMACHx = (CMSDK_DMACH_TypeDef *)(CMSDK_DMA_BASE + DMA_Ch*0x20);
  /* Check the parameters */
  assert_param(IS_DMA_CH(DMA_Ch));	
  assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	
	DMACHx->CTL &= (uint32_t)~((uint32_t)DMACH_CTL_RT);	
	
  if (NewState != DISABLE)
  {
    /* Enable Auto Repeat Transmit */
    DMACHx->CTL |= DMACH_CTL_RT;
  }
  else
  {
    /* Disable Auto Repeat Transmit */
  	DMACHx->CTL &= (uint32_t)~((uint32_t)DMACH_CTL_RT);	
  }
}


/**
  * @brief  Enables or disables DMA Source Address Increase.
  * @param  DMA_Ch  : DMA_CH0 ~ DMA_CH3.
  * @param  NewState: new state of the Source Address Increase. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DMA_SrcAddrssInc_Cmd(uint32_t DMA_Ch,FunctionalState NewState)
{
	CMSDK_DMACH_TypeDef *DMACHx = (CMSDK_DMACH_TypeDef *)(CMSDK_DMA_BASE + DMA_Ch*0x20);	
  /* Check the parameters */
  assert_param(IS_DMA_CH(DMA_Ch));	
  assert_param(IS_FUNCTIONAL_STATE(NewState));

	
	DMACHx->CTL &= (uint32_t)~((uint32_t)DMACH_CTL_SINC);	
	
  if (NewState != DISABLE)
  {
    /* Enable Source Address Increase */
    DMACHx->CTL |= DMACH_CTL_SINC;
  }
  else
  {
    /* Disable Source Address Increase */
  	DMACHx->CTL &= (uint32_t)~((uint32_t)DMACH_CTL_SINC);	
  }
}

/**
  * @brief  Enables or disables DMA Destination Address Increase.
  * @param  DMA_Ch  : DMA_CH0 ~ DMA_CH3.
  * @param  NewState: new state of the Destination Address Increase. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DMA_DstAddrssInc_Cmd(uint32_t DMA_Ch,FunctionalState NewState)
{
	CMSDK_DMACH_TypeDef *DMACHx = (CMSDK_DMACH_TypeDef *)(CMSDK_DMA_BASE + DMA_Ch*0x20);	
  /* Check the parameters */
  assert_param(IS_DMA_CH(DMA_Ch));	
  assert_param(IS_FUNCTIONAL_STATE(NewState));

	
	DMACHx->CTL &= (uint32_t)~((uint32_t)DMACH_CTL_DINC);	
	
  if (NewState != DISABLE)
  {
    /* Enable Destination Address Increase */
    DMACHx->CTL |= DMACH_CTL_DINC;
  }
  else
  {
    /* Disable Destination Address Increase */
  	DMACHx->CTL &= (uint32_t)~((uint32_t)DMACH_CTL_DINC);	
  }
}

/**
  * @brief  Config Data Size From Source Address to DMA Control  .
  * @param  DMA_Ch  : DMA_CH0 ~ DMA_CH3.
  * @param  Size:  Data From Source Address to DMA Control Size. 
  *          This parameter can be: WIDTH_8BIT
  *                               : WIDTH_16BIT
  *                               : WIDTH_32BIT
  * @retval None
  */
void DMA_SSIZEConfig(uint32_t DMA_Ch,uint32_t SSIZE)
{
	
	CMSDK_DMACH_TypeDef *DMACHx = (CMSDK_DMACH_TypeDef *)(CMSDK_DMA_BASE + DMA_Ch*0x20);
  /* Check the parameters */
  assert_param(IS_DMA_CH(DMA_Ch));	
  assert_param(IS_DMA_WIDTH(SSIZE));
	
	
	DMACHx->CTL &= (uint32_t)~((uint32_t)DMACH_CTL_SSIZE);	
	
  DMACHx->CTL |= SSIZE;

}

/**
  * @brief  Config Data Size From DMA Control to Destination Address .
  * @param  DMA_Ch  : DMA_CH0 ~ DMA_CH3.
  * @param  Size:  Data Size From DMA Control to Destination Address. 
  *          This parameter can be: WIDTH_8BIT
  *                               : WIDTH_16BIT
  *                               : WIDTH_32BIT
  * @retval None
  */
void DMA_DSIZEConfig(uint32_t DMA_Ch,uint32_t DSIZE)
{
	CMSDK_DMACH_TypeDef *DMACHx = (CMSDK_DMACH_TypeDef *)(CMSDK_DMA_BASE + DMA_Ch*0x20);
  /* Check the parameters */
  assert_param(IS_DMA_CH(DMA_Ch));	
  assert_param(IS_DMA_WIDTH(DSIZE));
	
	
	DMACHx->CTL &= (uint32_t)~((uint32_t)DMACH_CTL_DSIZE);	
	
  DMACHx->CTL |= DSIZE;

}

/**
  * @brief  Config DMA Source Address .
  * @param  DMA_Ch  : DMA_CH0 ~ DMA_CH3.
  * @retval None
  */
void DMA_SrcAddressConfig(uint32_t DMA_Ch,uint32_t SrcAddr)
{
	CMSDK_DMACH_TypeDef *DMACHx = (CMSDK_DMACH_TypeDef *)(CMSDK_DMA_BASE + DMA_Ch*0x20);	
  /* Check the parameters */
  assert_param(IS_DMA_CH(DMA_Ch));	


	
  DMACHx->SADR = SrcAddr;

}

/**
  * @brief  Config DMA Destination Address .
  * @param  DMA_Ch  : DMA_CH0 ~ DMA_CH3.
  * @retval None
  */
void DMA_DstAddressConfig(uint32_t DMA_Ch,uint32_t DstAddr)
{
  CMSDK_DMACH_TypeDef *DMACHx = (CMSDK_DMACH_TypeDef *)(CMSDK_DMA_BASE + DMA_Ch*0x20);	
  /* Check the parameters */
  assert_param(IS_DMA_CH(DMA_Ch));	


	
  DMACHx->DADR = DstAddr;

}


/**
  * @brief  Get DMA_CHx Current Source Address .
  * @param  DMA_Ch  : DMA_CH0 ~ DMA_CH3.
  * @retval None
  */
uint32_t DMA_GetSrcAddress(uint32_t DMA_Ch)
{
	CMSDK_DMACH_TypeDef *DMACHx = (CMSDK_DMACH_TypeDef *)(CMSDK_DMA_BASE + DMA_Ch*0x20);	
  /* Check the parameters */
  assert_param(IS_DMA_CH(DMA_Ch));	


  
  /*Get DMA_CHx Current Source Address */
  return  DMACHx->CSADR;
}

/**
  * @brief  Get DMA_CHx Current Destination Address .
  * @param  DMA_Ch  : DMA_CH0 ~ DMA_CH3.
  * @retval None
  */
uint32_t DMA_GetDstAddress(uint32_t DMA_Ch)
{
	CMSDK_DMACH_TypeDef *DMACHx = (CMSDK_DMACH_TypeDef *)(CMSDK_DMA_BASE + DMA_Ch*0x20);
  /* Check the parameters */
  assert_param(IS_DMA_CH(DMA_Ch));	

	
  
  /*Get DMA_CHx Current Destination Address */
  return  DMACHx->CDADR;
}


/**
  * @brief  Get DMA_CHx Remainder Cnt .
  * @param  DMA_Ch  : DMA_CH0 ~ DMA_CH3.
  * @retval None
  */
uint16_t DMA_GetRemainderCnt(uint32_t DMA_Ch)
{
	CMSDK_DMACH_TypeDef *DMACHx = (CMSDK_DMACH_TypeDef *)(CMSDK_DMA_BASE + DMA_Ch*0x20);
  /* Check the parameters */
  assert_param(IS_DMA_CH(DMA_Ch));	
	
  
  /*Get DMA_CHx Current Destination Address */
  return  DMACHx->TCNT;
}


/**
  * @brief  Enables or disables the specified DMA interrupts.
  * @param  DMA_IT: specifies the DMA interrupt source to be enabled or disabled. 
  *          This parameter can be one of the following values:
  *            @arg DMA_IT_CH0_END   : DMA_CH0 Transmit End Interrupt Mask
  *            @arg DMA_IT_CH1_END   : DMA_CH1 Transmit End Interrupt Mask
  *            @arg DMA_IT_CH2_END   : DMA_CH2 Transmit End Interrupt Mask
  *            @arg DMA_IT_CH3_END   : DMA_CH3 Transmit End Interrupt Mask	
  *            @arg DMA_IT_CONFIG_ERR: DMA Config Error Interrupt Mask 		
  * @param  NewState: new state of the specified DMA interrupt.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */

void DMA_ITConfig(uint32_t DMA_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_DMA_CONFIG_IT(DMA_IT));

  if (NewState != DISABLE)
  {
    /* Enable the selected QSPI interrupt */
    DMA->IER |= DMA_IT;
  }
  else
  {
    /* Disable the selected QSPI interrupt */
    DMA->IER &= (uint32_t)~DMA_IT;
  }
}


/**
  * @brief  Clears the DMA flag.
  * @param  DMA_FLAG: specifies the DMA flag to clear. 
  *          This parameter can be one of the following values:
  *            @arg DMA_IT_CH0_END   : DMA_CH0 Transmit End Interrupt Mask
  *            @arg DMA_IT_CH1_END   : DMA_CH1 Transmit End Interrupt Mask
  *            @arg DMA_IT_CH2_END   : DMA_CH2 Transmit End Interrupt Mask
  *            @arg DMA_IT_CH3_END   : DMA_CH3 Transmit End Interrupt Mask	
  *            @arg DMA_IT_CONFIG_ERR: DMA Config Error Interrupt Mask 	
  * @retval None
  */
void DMA_ClearITFlag(uint32_t DMA_FLAG)
{
  /* Check the parameters */
  assert_param(IS_DMA_GET_IT(DMA_FLAG));

  DMA->CLR = DMA_FLAG;
	
}


/**
  * @brief  Get DMA Status.
  * @param  DMA_FLAG: Specify the flag to be checked.
  *         This parameter can be one of the following values:
  *            @arg DMA_FLAG_CH0_END    : DMA_CH0 Transmit End Flag
  *            @arg DMA_FLAG_CH1_END    : DMA_CH1 Transmit End Flag
  *            @arg DMA_FLAG_CH2_END    : DMA_CH2 Transmit End Flag
  *            @arg DMA_FLAG_CH3_END    : DMA_CH3 Transmit End Flag
  *            @arg DMA_FLAG_CH0_BUSY   : DMA_CH0 Transmit Busy Flag
  *            @arg DMA_FLAG_CH1_BUSY   : DMA_CH1 Transmit Busy Flag
  *            @arg DMA_FLAG_CH2_BUSY   : DMA_CH2 Transmit Busy Flag
  *            @arg DMA_FLAG_CH3_BUSY   : DMA_CH3 Transmit Busy Flag	
  *            @arg DMA_FLAG_CONFIG_ERR : DMA Config Error Busy Flag 	
  * @return FlagStatus of TIM_FLAG (SET or RESET).
  */
FlagStatus DMA_GetFlagStatus(uint32_t DMA_FLAG)
{
  /* Check the parameters */
  assert_param(IS_DMA_FLAG(DMA_FLAG));	

  if ((DMA->STATUS & DMA_FLAG) != 0)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}


/**
  * @brief  Config DMA Peripheral .
  * @param  DMA_Ch      : DMA_CH0 ~ DMA_CH3.
  * @param  Peripheral  : DAM CH Relevance Peripheral.
  *            @arg DMA_UART0_TX    
  *            @arg DMA_UART0_RX    
  *            @arg DMA_UART1_TX   
  *            @arg DMA_UART1_RX    
  *            @arg DMA_UART2_TX  
  *            @arg DMA_UART2_RX   
  *            @arg DMA_SPI0_TX   
  *            @arg DMA_SPI0_RX   	
  *            @arg DMA_SPI1_TX 	
  *            @arg DMA_SPI1_RX  
  *            @arg DMA_QSPI_TX   
  *            @arg DMA_QSPI_RX   
  *            @arg DMA_TIMER3   	
  *            @arg DMA_TIMER3 
  *            @arg DMA_ADC
  *         This parameter can be one of the following values:
  * @retval None
  */
void DMA_PeripheralConfig(uint32_t DMA_Ch,uint32_t Peripheral)
{
  /* Check the parameters */
  assert_param(IS_DMA_CH(DMA_Ch));
  assert_param(IS_DMA_PERIPHERAL(Peripheral));	
	
	DMA->CHCFG &= (uint32_t)~((uint32_t)DMA_CHCFG_MASK << DMA_Ch*8);	
	
  DMA->CHCFG |= (Peripheral << DMA_Ch*8);

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
