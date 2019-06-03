/***************************************************************************
描述：reset routine
平台：SCM6XX 
时间：2018-1-11
作者：张彦欣                                                 
****************************************************************************/
#include "SCM6XX_RESET.h"



/*-----------------------------------------------------------------------------
  Function:		RMU_SoftRstSys                                                                                                                                                                          
  input:     	None                	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 软复位整颗芯片                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void RMU_Software_Reset_System(void) 
{
	SGCC_RESET_P->SOFT_RST0 = SOFT_RST0_SYS;  //系统软件复位
	
	while(SGCC_RESET_P->SOFT_RST0 & SOFT_RST0_SYS);   //等待硬件自清零
}

/*-----------------------------------------------------------------------------
  Function:		RMU_SoftRstSingMod                                                                                                                                                                          
  input:     	module-外设模块                	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 软复位某单个外设模块，包括总线端和工作域                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void RMU_Software_Reset_Module(UINT8 module) 
{
	switch(module)
	{
		  case DMA3:
		      SGCC_RESET_P->SOFT_RST0 = SOFT_RST0_DMA3;
          while(SGCC_RESET_P->SOFT_RST0 & SOFT_RST0_DMA3);  
	        break;
	    case DMA2:
		      SGCC_RESET_P->SOFT_RST0 = SOFT_RST0_DMA2;
          while(SGCC_RESET_P->SOFT_RST0 & SOFT_RST0_DMA2);  
	        break;
	    case DMA1:
		  	  SGCC_RESET_P->SOFT_RST0 = SOFT_RST0_DMA1;
          while(SGCC_RESET_P->SOFT_RST0 & SOFT_RST0_DMA1);  
	        break;
	    case DMA0:
		     	SGCC_RESET_P->SOFT_RST0 = SOFT_RST0_DMA0;
          while(SGCC_RESET_P->SOFT_RST0 & SOFT_RST0_DMA0);  
	        break;	
	    case GMAC1:
		    	SGCC_RESET_P->SOFT_RST0 = SOFT_RST0_GMAC1;
          while(SGCC_RESET_P->SOFT_RST0 & SOFT_RST0_GMAC1);  
	        break;
	    case GMAC0:
		    	SGCC_RESET_P->SOFT_RST0 = SOFT_RST0_GMAC0;
          while(SGCC_RESET_P->SOFT_RST0 & SOFT_RST0_GMAC0);  
	        break;
	    case OTG:
		  	  SGCC_RESET_P->SOFT_RST0 = (SOFT_RST0_OTG | SOFT_RST0_OTG_PHY);
          while((SGCC_RESET_P->SOFT_RST0 & SOFT_RST0_OTG) || (SGCC_RESET_P->SOFT_RST0 & SOFT_RST0_OTG_PHY));  
	        break;
		  case WDT:
		  	  SGCC_RESET_P->SOFT_RST1 = SOFT_RST1_WDT;
          while(SGCC_RESET_P->SOFT_RST1 & SOFT_RST1_WDT);  
	        break;
	    case TIMERA:
	    	  SGCC_RESET_P->SOFT_RST1 = SOFT_RST1_TIMERA;
          while(SGCC_RESET_P->SOFT_RST1 & SOFT_RST1_TIMERA);  
	        break;
      case TIMERB:
	    	  SGCC_RESET_P->SOFT_RST1 = SOFT_RST1_TIMERB;
          while(SGCC_RESET_P->SOFT_RST1 & SOFT_RST1_TIMERB);  
	        break;
	    case RTC:
	    	  SGCC_RESET_P->SOFT_RST1 = SOFT_RST1_RTC;
          while(SGCC_RESET_P->SOFT_RST1 & SOFT_RST1_RTC);
          SGCC_RESET_P->PER_RST = PER_RST_RTC;
          while(SGCC_RESET_P->PER_RST & PER_RST_RTC);  
	        break;
	    case GPIO:
	    	  SGCC_RESET_P->SOFT_RST1 = SOFT_RST1_GPIO;
          while(SGCC_RESET_P->SOFT_RST1 & SOFT_RST1_GPIO);  
	        break;
	    case SPI0:
	    	  SGCC_RESET_P->SOFT_RST1 = SOFT_RST1_SPI0;
          while(SGCC_RESET_P->SOFT_RST1 & SOFT_RST1_SPI0);
          SGCC_RESET_P->PER_RST = PER_RST_SPI0;
          while(SGCC_RESET_P->PER_RST & PER_RST_SPI0);   
	        break;
	    case SPI1:
	    	  SGCC_RESET_P->SOFT_RST1 = SOFT_RST1_SPI1;
          while(SGCC_RESET_P->SOFT_RST1 & SOFT_RST1_SPI1);
          SGCC_RESET_P->PER_RST = PER_RST_SPI1;
          while(SGCC_RESET_P->PER_RST & PER_RST_SPI1);   
	        break;
	    case I2C0:
	    	  SGCC_RESET_P->SOFT_RST1 = SOFT_RST1_I2C0;
          while(SGCC_RESET_P->SOFT_RST1 & SOFT_RST1_I2C0);  
	        break;
	    case I2C1:
	    	  SGCC_RESET_P->SOFT_RST1 = SOFT_RST1_I2C1;
          while(SGCC_RESET_P->SOFT_RST1 & SOFT_RST1_I2C1);  
	        break;    
	    case UART0:
	    	  SGCC_RESET_P->SOFT_RST1 = SOFT_RST1_UART0;
          while(SGCC_RESET_P->SOFT_RST1 & SOFT_RST1_UART0);  
	        break;    
	    case UART1:
	    	  SGCC_RESET_P->SOFT_RST1 = SOFT_RST1_UART1;
          while(SGCC_RESET_P->SOFT_RST1 & SOFT_RST1_UART1);  
	        break;    
	    case UART2:
	    	  SGCC_RESET_P->SOFT_RST1 = SOFT_RST1_UART2;
          while(SGCC_RESET_P->SOFT_RST1 & SOFT_RST1_UART2);  
	        break;    
	    case UART3:
	    	  SGCC_RESET_P->SOFT_RST1 = SOFT_RST1_UART3;
          while(SGCC_RESET_P->SOFT_RST1 & SOFT_RST1_UART3);  
	        break;    
	    case PWM0:
	    	  SGCC_RESET_P->SOFT_RST1 = SOFT_RST1_PWM0;
          while(SGCC_RESET_P->SOFT_RST1 & SOFT_RST1_PWM0);  
	        break;    
	    case U7816:
	    	  SGCC_RESET_P->SOFT_RST1 = SOFT_RST1_7816;
          while(SGCC_RESET_P->SOFT_RST1 & SOFT_RST1_7816);  
	        break;   
	    case TIMERC:
	    	  SGCC_RESET_P->SOFT_RST2 = SOFT_RST2_TIMERC;
          while(SGCC_RESET_P->SOFT_RST2 & SOFT_RST2_TIMERC);  
	        break;
      case TIMERD:
	    	  SGCC_RESET_P->SOFT_RST2 = SOFT_RST2_TIMERD;
          while(SGCC_RESET_P->SOFT_RST2 & SOFT_RST2_TIMERD);  
	        break;
	    case CRC:
	    	  SGCC_RESET_P->SOFT_RST2 = SOFT_RST2_CRC;
          while(SGCC_RESET_P->SOFT_RST2 & SOFT_RST2_CRC);  
	        break; 
	    case SPI2:
	    	  SGCC_RESET_P->SOFT_RST2 = SOFT_RST2_SPI2;
          while(SGCC_RESET_P->SOFT_RST2 & SOFT_RST2_SPI2);
          SGCC_RESET_P->PER_RST = PER_RST_SPI2;
          while(SGCC_RESET_P->PER_RST & PER_RST_SPI2);   
	        break; 
	    case SPI3:
	    	  SGCC_RESET_P->SOFT_RST2 = SOFT_RST2_SPI3;
          while(SGCC_RESET_P->SOFT_RST2 & SOFT_RST2_SPI3);
          SGCC_RESET_P->PER_RST = PER_RST_SPI3;
          while(SGCC_RESET_P->PER_RST & PER_RST_SPI3);   
	        break; 
	    case I2C2:
	    	  SGCC_RESET_P->SOFT_RST2 = SOFT_RST2_I2C2;
          while(SGCC_RESET_P->SOFT_RST2 & SOFT_RST2_I2C2);  
	        break; 
	    case UART4:
	    	  SGCC_RESET_P->SOFT_RST2 = SOFT_RST2_UART4;
          while(SGCC_RESET_P->SOFT_RST2 & SOFT_RST2_UART4);  
	        break; 
	    case UART5:
	    	  SGCC_RESET_P->SOFT_RST2 = SOFT_RST2_UART5;
          while(SGCC_RESET_P->SOFT_RST2 & SOFT_RST2_UART5);  
	        break; 
	    case UART6:
	    	  SGCC_RESET_P->SOFT_RST2 = SOFT_RST2_UART6;
          while(SGCC_RESET_P->SOFT_RST2 & SOFT_RST2_UART6);  
	        break; 
	    case UART7:
	    	  SGCC_RESET_P->SOFT_RST2 = SOFT_RST2_UART7;
          while(SGCC_RESET_P->SOFT_RST2 & SOFT_RST2_UART7);  
	        break; 
	    case PWM1:
	    	  SGCC_RESET_P->SOFT_RST2 = SOFT_RST2_PWM1;
          while(SGCC_RESET_P->SOFT_RST2 & SOFT_RST2_PWM1);  
	        break;     
      default:
	    	break;
	}
	
}


/*-----------------------------------------------------------------------------
  Function:		RMU_GetRstSts                                                                                                                                                                            
  input:     	rstSts :RST_STS_WDT/RST_STS_POR                 	  
  output:     None                                                                                    
  Return:    reset status                                                                                      
  Description: return reset status                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
UINT8 RMU_Get_Reset_Status(UINT32 Rst_Status) 
{
    UINT32 temp;
    temp = SGCC_RESET_P->RST_STS;
    
    if(temp & Rst_Status)
    	return ERROR; 
    else
        return SUCCESS;
}

/*-----------------------------------------------------------------------------
  Function:		RMU_ClrRstSts                                                                                                                                                                            
  input:     	rstSts :RST_STS_WDT/RST_STS_POR                 	  
  output:     None                                                                                    
  Return:    None                                                                              
  Description: clear reset status                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void RMU_Clear_Reset_Status(UINT32 Rst_Status) 
{
    SGCC_RESET_P->RST_STS = Rst_Status;
}


