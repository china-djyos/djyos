/***************************************************************************
描述：FLASH routine
平台：SCM6XX 
时间：2017-7-31
作者：张彦欣                                                 
****************************************************************************/
#include "SCM6XX_FLASH.h"

/*-----------------------------------------------------------------------------
  Function:		FLASH_SetNvrKey                                                                                                                                                                           
  input:     	None                 	  
  output:     None                                                                                    
  Returns:    1--success  0--fail                                                                                      
  Description: 设置information block保护字                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
UINT8 FLASH_Set_NVR_KEY(void) 
{
	  
    while(!(SGCC_FMC_P->FMC_STATUS & 0x1));  
    SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;   //clear program fail bit
    
	  SGCC_FMC_P->PROG_START = 1;  
    
    Write32(NVR_KEY_ADDR, NVR_KEY);
    
    while(1)
    {
    	if(SGCC_FMC_P->FMC_STATUS & 0x1)
    	{
    		if(SGCC_FMC_P->PROG_ERASE_FAIL & 0x3)
    		{
    			 SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;
    			 return ERROR;
    		}
    		else
    			 return SUCCESS;
    	}
    } 
}
/*-----------------------------------------------------------------------------
  Function:		FLASH_SetUpsKey                                                                                                                                                                           
  input:     	None                 	  
  output:     None                                                                                    
  Returns:    1--success  0--fail                                                                                  
  Description: 设置main block保护字                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
UINT8 FLASH_Set_UPS_KEY(void) 
{
    while(!(SGCC_FMC_P->FMC_STATUS & 0x1));  
    SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;   //clear program fail bit
	  SGCC_FMC_P->PROG_START = 1;  
    Write32(UPS_KEY_ADDR, UPS_KEY);
    while(1)
    {
    	if(SGCC_FMC_P->FMC_STATUS & 0x1)
    	{
    		if(SGCC_FMC_P->PROG_ERASE_FAIL & 0x3)
    		{
    			 SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;
    			 return ERROR;
    		}
    		else
    			 return SUCCESS;
    	}
    } 
}
/*-----------------------------------------------------------------------------
  Function:		FLASH_ProgSingleWord                                                                                                                                                                          
  input:     	addr: flash address
                buf:  
                len:                
  output:     None                                                                                    
  Returns:   1--success  0--fail                                                                                   
  Description: program 单个word                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
UINT8 FLASH_Program_Single_Word(UINT32 *addr, UINT32 dataWord) 
{
  /* */ while(!(SGCC_FMC_P->FMC_STATUS & 0x1));  
    
	  SGCC_FMC_P->T_HV_US_CNT = 50 ;
    	
    SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;   //clear program fail bit
    
   
	  SGCC_FMC_P->PROG_START = 1;  
    
    *addr = dataWord;
    
     while(1)
    {
    	if(SGCC_FMC_P->FMC_STATUS & 0x1)
    	{
    		if(SGCC_FMC_P->PROG_ERASE_FAIL & 0x3)
    		{
    			 SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;
    			 return ERROR;
    		}
    		else
    			 return SUCCESS;
    	}
    }
    
    /*
    while(!(SGCC_FMC_P->FMC_STATUS & 0x1));  
    SGCC_FMC_P->PROG_START = 1;  
    *addr++ = dataWord;
    */
}

/*-----------------------------------------------------------------------------
  Function:		FLASH_ProgWordsInPage                                                                                                                                                                          
  input:     	addr: flash address
                buf:  
                len:                
  output:     None                                                                                    
  Returns:    1--success  0--fail                                                                                   
  Description: program multiple words                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
UINT8 FLASH_Program_Word_In_Page(UINT32 *addr, UINT32 *buf, UINT32 len) 
{
    UINT32 i;
    
    while(!(SGCC_FMC_P->FMC_STATUS & 0x1));  
	
	  SGCC_FMC_P->T_HV_US_CNT = 2800 ;
    
    SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;   //clear program fail bit
    
	  SGCC_FMC_P->PROG_START = 1;  
    
    for(i = 0; i < len; i++)
    {
        addr[i] = buf[i];
    } 
		while(!(SGCC_FMC_P->FMC_STATUS & 0x1));
		
		if(SGCC_FMC_P->PROG_ERASE_FAIL & 0x3)
		{
			 SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;
			 return ERROR;
		}
		else
			 return SUCCESS;
		
  
}

///*-----------------------------------------------------------------------------
//  Function:		FLASH_ProgMultiWords                                                                                                                                                                          
//  input:     	addr: flash address
//                buf:  
//                len: number of words                 
//  output:     None                                                                                    
//  Returns:    1--success  0--fail                                                                                   
//  Description: program multiple words                                                                                                	                                   
// *-----------------------------------------------------------------------------*/
//UINT8 FLASH_ProgMultiWords(UINT32 *addr, UINT32 *buf, UINT32 len) 
//{
//    UINT32 i;
//    UINT32  pageNum, curPage;
//    
////    pageNo = ((UINT32)addr >> 8) & 0x7FF;
//    pageNum = len/32;
//    curPage = 0;
//    SGCC_FMC_P->T_HV_US_CNT = 2800 ;
//   
//    SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;   //clear program fail bit
//    
//    if(pageNum > 0)        //if more than one page need to be written
//	{
//		while(pageNum>0)
//		{
//			while(!(SGCC_FMC_P->FMC_STATUS & 0x1));  
//			 SGCC_FMC_P->PROG_START = 1; 
//			 for(i = 0; i < 32; i++)
//			{
//				*(addr+i+curPage*32) = buf[i+curPage*32];
//			}
//            while(1)
//            {
//    	        if(SGCC_FMC_P->FMC_STATUS & 0x1)
//    	        {
//    		        if(SGCC_FMC_P->PROG_ERASE_FAIL & 0x3)
//    		        {
//    			        SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;
//    			        return 0;
//    		        }
//    		        else
//    			        break;
//    	        }
//            }  
//			pageNum--;
//			curPage++;
//		}
//	}		
//	if(len%32)     
//	{
//		addr = addr+4*32*curPage;
//		while(!(SGCC_FMC_P->FMC_STATUS & 0x1));  
//		SGCC_FMC_P->PROG_START = 1; 
//		for(i=0;i<len%32;i++)
//		{
//			*(addr+i) = buf[i+32*curPage];
//		}
//		while(!(SGCC_FMC_P->FMC_STATUS & 0x1));
//		
//		if(SGCC_FMC_P->PROG_ERASE_FAIL & 0x3)
//		{
//			 SGCC_FMC_P->PROG_ERASE_FAIL = 0x3;
//			 return ERROR;
//		}
//		else
//			 return SUCCESS;
//	}
//	return 0XFF;
//}

/*-----------------------------------------------------------------------------
  Function:		FLASH_SectorErase                                                                                                                                                                          
  input:     eraseAdr-address to erase	                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: start sector erase                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
UINT8 FLASH_Sector_Erase(UINT32 *eraseAdr) 
{
    while(!(SGCC_FMC_P->FMC_STATUS & 0x1));
    SGCC_FMC_P->PROG_ERASE_FAIL = 0x4;   //clear erase fail bit
	  SGCC_FMC_P->SECTOR_ERASE_START = 1;  
    *eraseAdr = 0xffffffff;
    while(1)
    {
    	if(SGCC_FMC_P->FMC_STATUS & 0x1)
    	{
    		if(SGCC_FMC_P->PROG_ERASE_FAIL & 0x4)
    		{
    			 SGCC_FMC_P->PROG_ERASE_FAIL = 0x4;
    			 return ERROR;
    		}
    		else
    			 return SUCCESS;
    	}
    }  
}

/*-----------------------------------------------------------------------------
  Function:		FLASH_ChipErase                                                                                                                                                                          
  input:     	None                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 启动chip erase                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
UINT8 FLASH_Chip_Erase(UINT32 *eraseAdr) 
{
    while(!(SGCC_FMC_P->FMC_STATUS & 0x1));
    SGCC_FMC_P->PROG_ERASE_FAIL = 0x4;   //clear erase fail bit
	  SGCC_FMC_P->CHIP_ERASE_START = 1;  
    *eraseAdr = 0xffffffff;
    while(1)
    {
    	if(SGCC_FMC_P->FMC_STATUS & 0x1)
    	{
    		if(SGCC_FMC_P->PROG_ERASE_FAIL & 0x4)
    		{
    			 SGCC_FMC_P->PROG_ERASE_FAIL = 0x4;
    			 return ERROR;
    		}
    		else
    			 return SUCCESS;
    	}
    }  
}
/*-----------------------------------------------------------------------------
  Function:		EFLASH_Read                                                                                                                                                                            
  input:     
            len, WORD as unit
  output:     buf                                                                                    
  Returns:    None                                                                                     
  Description: 读FLASH                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void FLASH_Read(UINT32 *addr,  UINT32 *buf, UINT32 len)
{
	UINT32 i=0;
	
    for(i = 0; i < len; i++)
    {
        buf[i] = addr[i];
    }
}

/*-----------------------------------------------------------------------------
  Function:		EFLASH_EnStdby                                                                                                                                                                            
  input:    opt: ENABLE/DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                     
  Description: enable flash standby                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void FLASH_Enable_Standby(FunctionalState opt) 
{
    if(opt == ENABLE)
	    SGCC_FMC_P->STANDBY_EN |= STANDBY_EN_STANDBY_EN;
    else
        SGCC_FMC_P->STANDBY_EN &= ~STANDBY_EN_STANDBY_EN;
}

/*-----------------------------------------------------------------------------
  Function:		EFLASH_EnDeepPowerDown                                                                                                                                                                            
  input:    opt: ENABLE/DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                     
  Description: enable flash deep power down                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void FLASH_Enable_Deep_Power_Down(FunctionalState opt) 
{
    if(opt == ENABLE)
	    SGCC_FMC_P->STANDBY_EN |= STANDBY_EN_DPD_EN;
    else
        SGCC_FMC_P->STANDBY_EN &= ~STANDBY_EN_DPD_EN;
}
