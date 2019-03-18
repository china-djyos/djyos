#ifndef _SCM6XX_CRC_H_
#define _SCM6XX_CRC_H_

#include "SCM6XX_ALL.h"


typedef struct 
{
	UINT16 polyType;
    UINT16 inRev;
    UINT16 outRev;
    UINT16 xorOut;
    UINT16 crcMode;    
    UINT32 initVal;
}CRC_InitTypeDef;


/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */

void CRC_Init(CRC_InitTypeDef CRC_Init);
void CRC_Enable(FunctionalState opt);
void CRC_Get_Word(UINT32* buf);
void CRC_Input_Word(UINT32* buf, UINT16 len);
BOOL CRC_Get_Verify_Result(void);

#endif 

