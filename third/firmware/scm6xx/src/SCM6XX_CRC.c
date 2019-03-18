/***************************************************************************
描述：crc routine
平台：SCM6XX
时间：2017-8-29
作者：张彦欣                                                 
****************************************************************************/
#include "SCM6XX_CRC.h"


/*-----------------------------------------------------------------------------
  Function:		CRC_Init                                                                                                                                                                         
  input:     	crcInit
                
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: CRC initialization                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void CRC_Init(CRC_InitTypeDef CRC_Init) 
{
    SGCC_CRC_P->CTRL = 0;
	SGCC_CRC_P->CTRL = CRC_Init.polyType | CRC_Init.inRev | CRC_Init.outRev | CRC_Init.xorOut | CRC_Init.crcMode;
    SGCC_CRC_P->DATA.DATA_WORD = CRC_Init.initVal;
}


/*-----------------------------------------------------------------------------
  Function:		CRC_Enable                                                                                                                                                                         
  input:     	opt - ENABLE/DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: CRC enable                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void CRC_Enable(FunctionalState opt) 
{
	if(opt == ENABLE)
    {
        SGCC_CRC_P->CTRL |= CRC_CTRL_CRCEN_EN;
    }
    else
    {
        SGCC_CRC_P->CTRL = 0;
    }
}

/*-----------------------------------------------------------------------------
  Function:		CRC_GetWord                                                                                                                                                                         
  input:     	buf
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: get CRC result                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void CRC_Get_Word(UINT32* buf) 
{
	while(!(SGCC_CRC_P->STAT & CRC_STAT_CRC_RDY));
    *buf = SGCC_CRC_P->RESU;
}
/*-----------------------------------------------------------------------------
  Function:		CRC_GetHalfWord                                                                                                                                                                         
  input:     	buf
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: get CRC result                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void CRC_Get_Half_Word(UINT16* buf) 
{
	while(!(SGCC_CRC_P->STAT & CRC_STAT_CRC_RDY));
    *buf = (UINT16)SGCC_CRC_P->RESU;
}

/*-----------------------------------------------------------------------------
  Function:		CRC_GetByte                                                                                                                                                                        
  input:     	buf
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: get CRC result                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void CRC_Get_Byte(UINT8* buf) 
{
	while(!(SGCC_CRC_P->STAT & CRC_STAT_CRC_RDY));
    *buf = (UINT8)SGCC_CRC_P->RESU;
}

/*-----------------------------------------------------------------------------
  Function:		CRC_InputWord                                                                                                                                                                         
  input:     	buf
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: CRC                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void CRC_Input_Word(UINT32* buf, UINT16 len) 
{
    UINT16 i;
	for(i = 0; i < len; i++)
    {
        while(!(SGCC_CRC_P->STAT & (1 << CRC_STAT_CRC_RDYB)));
        SGCC_CRC_P->DATA.DATA_WORD = buf[i];
    }
}

/*-----------------------------------------------------------------------------
  Function:		CRC_InputHalfWord                                                                                                                                                                         
  input:     	buf
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: CRC                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void CRC_InputHalfWord(UINT16* buf, UINT16 len) 
{
    UINT16 i;
	for(i = 0; i < len; i++)
    {
        while(!(SGCC_CRC_P->STAT & (1 << CRC_STAT_CRC_RDYB)));
        (SGCC_CRC_P->DATA.DATA_HALFW[0]) = buf[i];
        
    }
}

/*-----------------------------------------------------------------------------
  Function:		CRC_InputByte                                                                                                                                                                        
  input:     	buf
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: CRC                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void CRC_InputByte(UINT8* buf, UINT16 len) 
{
    UINT16 i;
	for(i = 0; i < len; i++)
    {
        while(!(SGCC_CRC_P->STAT & (1 << CRC_STAT_CRC_RDYB)));
        (SGCC_CRC_P->DATA.DATA_BYTE[0]) = buf[i];
        
    }
}

/*-----------------------------------------------------------------------------
  Function:		CRC_GetVerifyResult                                                                                                                                                                         
  input:     None	
  output:     None                                                                                    
  Returns:    1 - PASS
              0 - FAIL 
  Description: CRC initialization                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
BOOL CRC_Get_Verify_Result(void) 
{
	if(SGCC_CRC_P->STAT & (1 << CRC_STAT_CRC_FAILB))
        return 0;
    else
        return 1;
}

