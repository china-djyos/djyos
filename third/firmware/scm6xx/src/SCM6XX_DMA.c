/***************************************************************************
描述：DMA routine
平台：
时间：2017-8-7
作者：张彦欣                                                 
****************************************************************************/
#include "SCM6XX_DMA.h"

/*-----------------------------------------------------------------------------
  Function:		DMA_Init                                                                                                                                                                                 
  input:     DMAx: select the DMA channel 
             DMA_InitStruct:parameters to configure DMA channel	                                     
  Returns:    None                                                                                      
  Description:Initialize DMA channel                                    
 *-----------------------------------------------------------------------------*/
void DMA_Init (SGCC_DMA_TypeDef* DMAx, DMA_InitTypeDef* DMA_InitStruct)
{
   
    DMAx->TRANS_SRC_ADDR = DMA_InitStruct->srcSaddr  ;
    DMAx->TRANS_DST_ADDR = DMA_InitStruct->dstSaddr ;

    DMAx->TRANS_FMT1 = (DMA_InitStruct->srcBlkOffset | DMA_InitStruct->srcInc | DMA_InitStruct->destBlkOffset | DMA_InitStruct->dstInc \
                       | DMA_InitStruct->trUnit |  DMA_InitStruct->transMode);
                       
    DMAx->TRANS_FMT2_STAT2 = (DMA_InitStruct->blocks << 16) | DMA_InitStruct->trCnt;
    if(DMA_InitStruct->transMode != DMA_TRANS_FMT1_REG_TRANS_MODE_COMMON)
    {
        if(DMA_InitStruct->intSrc <= 31)
        {
            DMAx->TRIGEN1_REG = 0;
            DMAx->TRIGEN0_REG =  ((UINT32)1 << DMA_InitStruct->intSrc);
        }
        else
        {
            DMAx->TRIGEN0_REG = 0;
            DMAx->TRIGEN1_REG =  ((UINT32)1 << (DMA_InitStruct->intSrc - 32));
        }
    }
}

/*-----------------------------------------------------------------------------
  Function:		DMA_EnableIntr                                                                                                                                                                                
  input:    DMAx: select the DMA 
            intEn: ENABLE/DISABLE                
  Returns:    None                                                                                      
  Description: enable DMA interrupt                                                                                           
               	                                  
 *-----------------------------------------------------------------------------*/
void DMA_Enable_Int(SGCC_DMA_TypeDef* DMAx,FunctionalState opt)
{ 
    if(opt == ENABLE)
    {
	    DMAx->TRANS_FMT1 |= DMA_TRANS_FMT1_REG_INTEN;
    }
    else
    {
        DMAx->TRANS_FMT1 &= ~DMA_TRANS_FMT1_REG_INTEN;
    }
}
/*-----------------------------------------------------------------------------
  Function:		DMA_ClearIntr                                                                                                                                                                                
  input:    DMAx: select the DMA                      
  Returns:    None                                                                                      
  Description: clear DMA interrupt                                                                                           
               	                                  
 *-----------------------------------------------------------------------------*/
void DMA_Clear_Int(SGCC_DMA_TypeDef* DMAx)
{ 
   
	DMAx->TRANS_STAT1 = DMA_TRANS_STAT1_REG_DMA_INT;
    
}
/*-----------------------------------------------------------------------------
  Function:		DMA_Enable                                                                                                                                                                              
  input:    DMAx: select the DMA 
            opt:      ENABLE/DISABLE                
  Returns:    None                                                                                      
  Description: enable DMA                                                                                           
               	                                  
 *-----------------------------------------------------------------------------*/
void DMA_Enable(SGCC_DMA_TypeDef* DMAx,FunctionalState opt)
{ 
    if(opt == ENABLE)
    {
	    DMAx->TRANS_CTRL = DMA_TRANS_CTRL_REG_DMAEN;
    }
    else
    {
        DMAx->TRANS_CTRL &= ~DMA_TRANS_CTRL_REG_DMAEN;
    }
}

/*-----------------------------------------------------------------------------
  Function:		DMA_GetLeftTrCnt                                                                                                                                                                            
  input:    DMAx: select the DMA 
  Returns:    left transfer count
  Description: get left transfer count                                                                                           
               	                                  
 *-----------------------------------------------------------------------------*/
UINT32 DMA_Get_Left_TrCnt(SGCC_DMA_TypeDef* DMAx)
{ 
    UINT32 temp;
    temp = DMAx->TRANS_FMT2_STAT2 & DMA_TRANS_STAT2_REG_TR_CNT_LEFT;
    
    return(temp);
}

/*-----------------------------------------------------------------------------
  Function:		DMA_GetLeftRepTime                                                                                                                                                                            
  input:    DMAx: select the DMA 
  Returns:    left transfer count
  Description: get left transfer count                                                                                           
               	                                  
 *-----------------------------------------------------------------------------*/
UINT32 DMA_Get_Left_RepTime(SGCC_DMA_TypeDef* DMAx)
{ 
    UINT32 temp;
    temp = (DMAx->TRANS_FMT2_STAT2 & DMA_TRANS_STAT2_REG_REPTIMES_LEFT) >> 16;
    
    return(temp);
}

/*-----------------------------------------------------------------------------
  Function:		DMA_GetErrSts                                                                                                                                                                            
  input:    DMAx: select the DMA 
  Returns:    error status
  Description: get error status                                                                                          
               	                                  
 *-----------------------------------------------------------------------------*/
UINT8 DMA_Get_Err_Status(SGCC_DMA_TypeDef* DMAx)
{ 
    UINT32 temp;
    temp = (DMAx->TRANS_STAT1 & DMA_TRANS_STAT1_REG_DMA_ERR) >> 2;
    
    return(temp);
}

/*-----------------------------------------------------------------------------
  Function:		DMA_GetIntSts                                                                                                                                                                            
  input:    DMAx: select the DMA 
  Returns:    interrupt status
  Description: get interrupt status                                                                                          
               	                                  
 *-----------------------------------------------------------------------------*/
UINT8 DMA_Get_Int_Status(SGCC_DMA_TypeDef* DMAx)
{ 
    UINT32 temp;
    temp = (DMAx->TRANS_STAT1 & DMA_TRANS_STAT1_REG_DMA_INT) >> 1;
    
    return(temp);
}

/*-----------------------------------------------------------------------------
  Function:		DMA_ClrErrStatus                                                                                                                                                                             
  input:    DMAx: select the DMA 
  Returns:    NONE
  Description: clear error status                                                                                           
               	                                  
 *-----------------------------------------------------------------------------*/
void DMA_Clear_Error_Status(SGCC_DMA_TypeDef* DMAx)
{ 
    DMAx->TRANS_STAT1 = DMA_TRANS_STAT1_REG_DMA_ERR;
    
}

/*-----------------------------------------------------------------------------
  Function:		DMA_ClrIntStatus                                                                                                                                                                             
  input:    DMAx: select the DMA 
  Returns:    NONE
  Description: clear interrupt status                                                                                           
               	                                  
 *-----------------------------------------------------------------------------*/
void DMA_Clear_Int_Status(SGCC_DMA_TypeDef* DMAx)
{ 
    DMAx->TRANS_STAT1 = DMA_TRANS_STAT1_REG_DMA_INT;
    
}


