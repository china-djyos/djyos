#ifndef _SCM6XX_DMA_H_
#define _SCM6XX_DMA_H_

#include "SCM6XX_ALL.h"

typedef struct 
{
    UINT32 srcSaddr;
	UINT32 dstSaddr;
	UINT32 srcInc;
	UINT32 dstInc;
	UINT32 srcBlkOffset;
	UINT32 destBlkOffset;
	UINT32 transMode;
	UINT32 trUnit;
	UINT32 trCnt;
    UINT32 blocks;
    UINT32 intSrc;
}DMA_InitTypeDef;




/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */

void DMA_Init (SGCC_DMA_TypeDef* DMAx, DMA_InitTypeDef* DMA_InitStruct);
void DMA_Enable_Int(SGCC_DMA_TypeDef* DMAx,FunctionalState opt);
void DMA_Clear_Int(SGCC_DMA_TypeDef* DMAx);
void DMA_Enable(SGCC_DMA_TypeDef* DMAx,FunctionalState opt);
UINT32 DMA_Get_Left_TrCnt(SGCC_DMA_TypeDef* DMAx);
UINT32 DMA_Get_Left_RepTime(SGCC_DMA_TypeDef* DMAx);
UINT8 DMA_Get_Err_Status(SGCC_DMA_TypeDef* DMAx);
UINT8 DMA_Get_Int_Status(SGCC_DMA_TypeDef* DMAx);
void DMA_Clear_Error_Status(SGCC_DMA_TypeDef* DMAx);
void DMA_Clear_Int_Status(SGCC_DMA_TypeDef* DMAx);


#endif 





