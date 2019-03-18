/***************************************************************************
描述：clock routine
平台：SCM6XX 
时间：2017-9-11
作者：张彦欣                                                 
****************************************************************************/
#include "SCM6XX_CLK.h"


/*-----------------------------------------------------------------------------
  Function:		CLK_EnAllClk                                                                                                                                                                            
  input:     	None                  	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 使能所有模块时钟                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Enable_All_Clk(void) 
{
	SGCC_CLK_P->HCLK_EN = 0xff;
	SGCC_CLK_P->PCLK0_EN |= 0x7ffbf;  
	SGCC_CLK_P->PCLK1_EN |= 0xffff;  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_DisAllPeriClk                                                                                                                                                                            
  input:     	None                  	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 不使能所油馍栊模块时钟                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Disable_All_Clk(void) 
{
    SGCC_CLK_P->SYSCLK_CTRL &= ~(SYSCLK_CTRL_GMAC_PADCLK_EN | SYSCLK_CTRL_PHY_30M_EN);
	SGCC_CLK_P->HCLK_EN = 1;
	SGCC_CLK_P->PCLK0_EN = 0x8;  
	SGCC_CLK_P->PCLK1_EN = 0x6000;  
}

/*-----------------------------------------------------------------------------
  Function:		EnPeriClk                                                                                                                                                                           
  input:     	module-外设模块                	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 使能某外设模块时钟 ,IOCTRL没有单独的时钟使能位                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Enable_Peripheral_Clk(UINT8 module) 
{
	switch(module)
	{
	    case GPIO:
	    	SGCC_CLK_P->PCLK0_EN |= (PCLK0_EN_GPIO_DB | PCLK0_EN_GPIO);
	    	break;
      case UART0:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_UART0;
	    	break;
	    case UART1:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_UART1;
	    	break;
	    case UART2:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_UART2;
	    	break;
        case UART3:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_UART3;
	    	break;
	    case UART4:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_UART4;
	    	break;
	    case UART5:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_UART5;
	    	break;
	    case UART6:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_UART6;
	    	break;
        case UART7:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_UART7;
	    	break;	
      case DMA0:
	    	SGCC_CLK_P->HCLK_EN |= HCLK_EN_DMA0;
	    	break;    
	    case DMA1:
	    	SGCC_CLK_P->HCLK_EN |= HCLK_EN_DMA1;
	    	break;   	
	    case DMA2:
	    	SGCC_CLK_P->HCLK_EN |= HCLK_EN_DMA2;
	    	break;    	
	    case DMA3:
	    	SGCC_CLK_P->HCLK_EN |= HCLK_EN_DMA3;
	    	break;   	
	    case I2C0:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_I2C0;
	    	break;
	    case I2C1:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_I2C1;
	    	break;
	   case I2C2:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_I2C2;
	    	break; 	
     case U7816:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_7816;
	    	break;
     case PWM0:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_PWM0;
	    	break;
	   case PWM1:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_PWM1;
	    	break;
     case SPI0:
	    	SGCC_CLK_P->PCLK0_EN |= (PCLK0_EN_SPI0 | PCLK0_EN_SPI_MST0);
	    	break;
	   case SPI1:
	    	SGCC_CLK_P->PCLK0_EN |= (PCLK0_EN_SPI1 | PCLK0_EN_SPI_MST1);
	    	break;    
     case SPI2:
	    	SGCC_CLK_P->PCLK1_EN |= (PCLK1_EN_SPI2 | PCLK1_EN_SPI_MST2);
	    	break;      
     case SPI3:
	    	SGCC_CLK_P->PCLK1_EN |= (PCLK1_EN_SPI2 | PCLK1_EN_SPI_MST3);
	    	break;      
	   case CRC:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_CRC;
	    	break;
     case ANACTRL:
	    	SGCC_CLK_P->PCLK1_EN |= (PCLK1_EN_ANA | PCLK1_EN_ANA_W1);
	    	break;
     case ADC:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_ADC_CLK;
	    	break;
     case WDT:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_WDT;
	    	break;
	   case TIMERA:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_TIMERA;
	    	break;
	   case TIMERB:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_TIMERB;
	    	break;
	   case TIMERC:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_TIMERC;
	    	break;
	   case TIMERD:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_TIMERD;
	    	break;
	   case RTC:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_RTC;
	    	break;
	   case GMAC0:
	      SGCC_CLK_P->HCLK_EN |= HCLK_EN_GMAC0;
	      SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_GMAC_PADCLK_EN;
	    	break;
	   case GMAC1:
	      SGCC_CLK_P->HCLK_EN |= HCLK_EN_GMAC1;
	      SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_GMAC_PADCLK_EN;
	    	break;
	   case OTG:
	      SGCC_CLK_P->HCLK_EN |= HCLK_EN_OTG;
	      SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_PHY_30M_EN;
	    	break;
	   case FLASH:
	      SGCC_CLK_P->HCLK_EN |= HCLK_EN_FLASH;
	    	break; 	
	    default:
            break;
	    
	}
	
}

/*-----------------------------------------------------------------------------
  Function:		CLK_DisPeriClk                                                                                                                                                                           
  input:     	module-外设模块                	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 不使能某外设模块时钟                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Disable_Peripheral_Clk(UINT8 module) 
{	
	switch(module)
	{
	    case GPIO:
	    	SGCC_CLK_P->PCLK0_EN &= ~(PCLK0_EN_GPIO_DB | PCLK0_EN_GPIO);
	    	break;
      case UART0:
	    	SGCC_CLK_P->PCLK0_EN &= ~ PCLK0_EN_UART0;
	    	break;
	    case UART1:
	    	SGCC_CLK_P->PCLK0_EN &= ~ PCLK0_EN_UART1;
	    	break;
	    case UART2:
	    	SGCC_CLK_P->PCLK0_EN &= ~ PCLK0_EN_UART2;
	    	break;
        case UART3:
	    	SGCC_CLK_P->PCLK0_EN &= ~ PCLK0_EN_UART3;
	    	break;
	    case UART4:
	    	SGCC_CLK_P->PCLK1_EN &= ~ PCLK1_EN_UART4;
	    	break;
	    case UART5:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_UART5;
	    	break;
	    case UART6:
	    	SGCC_CLK_P->PCLK1_EN &= ~ PCLK1_EN_UART6;
	    	break;
        case UART7:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_UART7;
	    	break;	
      case DMA0:
	    	SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_DMA0;
	    	break;    
	    case DMA1:
	    	SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_DMA1;
	    	break;   	
	    case DMA2:
	    	SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_DMA2;
	    	break;    	
	    case DMA3:
	    	SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_DMA3;
	    	break;   	
	    case I2C0:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_I2C0;
	    	break;
	    case I2C1:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_I2C1;
	    	break;
	   case I2C2:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_I2C2;
	    	break; 	
     case U7816:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_7816;
	    	break;
     case PWM0:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_PWM0;
	    	break;
	   case PWM1:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_PWM1;
	    	break;
     case SPI0:
	    	SGCC_CLK_P->PCLK0_EN &= ~(PCLK0_EN_SPI0 | PCLK0_EN_SPI_MST0);
	    	break;
	   case SPI1:
	    	SGCC_CLK_P->PCLK0_EN &= ~(PCLK0_EN_SPI1 | PCLK0_EN_SPI_MST1);
	    	break;    
     case SPI2:
	    	SGCC_CLK_P->PCLK1_EN &= ~(PCLK1_EN_SPI2 | PCLK1_EN_SPI_MST2);
	    	break;      
     case SPI3:
	    	SGCC_CLK_P->PCLK1_EN &= ~(PCLK1_EN_SPI2 | PCLK1_EN_SPI_MST3);
	    	break;      
	   case CRC:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_CRC;
	    	break;
     case ANACTRL:
	    	SGCC_CLK_P->PCLK1_EN &= ~(PCLK1_EN_ANA | PCLK1_EN_ANA_W1);
	    	break;
     case ADC:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_ADC_CLK;
	    	break;
     case WDT:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_WDT;
	    	break;
	   case TIMERA:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_TIMERA;
	    	break;
	   case TIMERB:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_TIMERB;
	    	break;
	   case TIMERC:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_TIMERC;
	    	break;
	   case TIMERD:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_TIMERD;
	    	break;
	   case RTC:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_RTC;
	    	break;
	   case GMAC0:
	      SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_GMAC0;
	      SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_GMAC_PADCLK_EN;
	    	break;
	   case GMAC1:
	      SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_GMAC1;
	      SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_GMAC_PADCLK_EN;
	    	break;
	   case OTG:
	      SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_OTG;
	      SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_PHY_30M_EN;
	    	break;
	   case FLASH:
	      SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_FLASH;
	    	break; 	
	    default:
            break;
	}
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnableMatrix                                                                                                                                                                           
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable matrix hclk                                                                                              	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Enable_HCLK_Matrix(FunctionalState opt) 
{
    if(opt == ENABLE)
        SGCC_CLK_P->HCLK_EN |= HCLK_EN_MTX;
    else
        SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_MTX;    
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnableHosc                                                                                                                                                                           
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable HOSC                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Enable_HOSC(FunctionalState opt) 
{
    if(opt == ENABLE)
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_HOSC_EN;
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_HOSC_EN; 
        
    while(!(SGCC_CLK_P->SYSCLK_CTRL & SYSCLK_CTRL_HCLK_RD));   
    
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnableLosc                                                                                                                                                                           
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable LOSC                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Enable_LOSC(FunctionalState opt) 
{
    if(opt == ENABLE)
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_LOSC_EN;
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_LOSC_EN;    
    
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnablePll                                                                                                                                                                          
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable PLL                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Enable_PLL(FunctionalState opt) 
{
    if(opt == ENABLE)
    {
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_PLL_EN;
        //wait until PLL locked
        while(!(SGCC_CLK_P->SYSCLK_CTRL & SYSCLK_CTRL_PLL_LOCKED_O));  
    }
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_PLL_EN;  
        
    
    
}
/*-----------------------------------------------------------------------------
  Function:		CLK_EnableXhosc                                                                                                                                                                           
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable XHOSC                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Enable_XHOSC(FunctionalState opt) 
{
    if(opt == ENABLE)
    {
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_XHOSC_EN;
        //wait until xhosc ready
        while(!(SGCC_CLK_P->SYSCLK_CTRL & SYSCLK_CTRL_XHOSC_RD));
    }
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_XHOSC_EN;    
        
    
    
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnableXlosc                                                                                                                                                                           
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable XLOSC                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Enable_XLOSC(FunctionalState opt) 
{
    if(opt == ENABLE)
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_XLOSC_EN;
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_XLOSC_EN;    
    
}

/*-----------------------------------------------------------------------------
  Function:		CLK_SelectHclk                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV1/SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV2/SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV4/SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV8
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择ADC时钟为1M/2MHz                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Select_HCLK(UINT32 HCLK_Clk) 
{
	UINT32 temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_HCLK_SEL;
	temp |= HCLK_Clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_SelectPclk                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_PCLK_SEL_HCLK_DIV2/SYSCLK_CTRL_PCLK_SEL_HCLK_DIV4
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择ADC时钟为1M/2MHz                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Select_PCLK(UINT32 PCLK_Clk) 
{
	UINT32 temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_PCLK_SEL;
	temp |= PCLK_Clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_ConfigAdcPllDiv                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_ADC_PLL_DIV_32/SYSCLK_CTRL_ADC_PLL_DIV_64/SYSCLK_CTRL_ADC_PLL_DIV_128/SYSCLK_CTRL_ADC_PLL_DIV_256                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择ADC使用PLL的分频系数                                                                                              	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Configure_ADC_PLL_Division(UINT32 ADC_PLL_Clk) 
{
	UINT32 temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_ADC_PLL_DIV;
	temp |= ADC_PLL_Clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_ConfigAdcHoscDiv                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_ADC_HOSC_DIV_8/SYSCLK_CTRL_ADC_HOSC_DIV_16/SYSCLK_CTRL_ADC_HOSC_DIV_32/SYSCLK_CTRL_ADC_HOSC_DIV_64                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择ADC使用HOSC的分频系数                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Configure_ADC_HOSC_Division(UINT32 ADC_HOSC_Clk) 
{
	UINT32 temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_ADC_HOSC_DIV;
	temp |= ADC_HOSC_Clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}
/*-----------------------------------------------------------------------------
  Function:		CLK_SelectRtcClk                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_RTC_CLK_SEL_XLOSC/SYSCLK_CTRL_RTC_CLK_SEL_EXTAL32
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择RTC时钟为XLOSC或者external 32K                                                                                              	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Select_RTC_Clk(UINT32 RTC_Clk) 
{
	UINT32 temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_RTC_CLK_SEL;
	temp |= RTC_Clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_SelectGpioDbClk                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_GPIO_DBCLK_SEL_LOSC/SYSCLK_CTRL_GPIO_DBCLK_SEL_PCLK
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择GPIO DBCLK为LOSC或者PCLK                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Select_GPIO_Debounce_Clk(UINT32 GPIO_DB_Clk) 
{
	UINT32 temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_GPIO_DBCLK_SEL;
	temp |= GPIO_DB_Clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
}
/*-----------------------------------------------------------------------------
  Function:		CLK_SelectClkOut                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_ANA_CLK_MUX_LOSC/SYSCLK_CTRL_ANA_CLK_MUX_XLOSC/
              SYSCLK_CTRL_ANA_CLK_MUX_XHOSC_DIV4/SYSCLK_CTRL_ANA_CLK_MUX_HOSC_DIV16/
              SYSCLK_CTRL_ANA_CLK_MUX_PLL_DIV64     
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择CLK_OUT时钟源                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Select_Clk_Out(UINT32 Clk_Out_Clk) 
{
	UINT32 temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_ANA_CLK_MUX;
	temp |= Clk_Out_Clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnClkOut                                                                                                                                                                          
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable clk_out                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Enable_Clk_Out(FunctionalState opt) 
{
    if(opt == ENABLE)
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_ANA_CLK_EN;
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_ANA_CLK_EN;    
    
}


/*-----------------------------------------------------------------------------
  Function:		CLK_ConfigPll                                                                                                                                                                            
  input:     	pllConfig
 typedef struct
 {
    UINT32 refDiv;  //PLL_CTRL_PLL_REFDIV_1~4
    UINT32 fbDiv;   //PLL_CTRL_PLL_FBDIV_(n), n=0x0~0x7f
    UINT32 outDiv;  //PLL_CTRL_PLL_OUTDIV_1~4
    UINT32 refClk;  //PLL_CTRL_PLL_REF_XHOSC/PLL_CTRL_PLL_REF_HOSC/PLL_CTRL_PLL_REF_PAD50M
    UINT32 icpSelA; //PLL_CTRL_PLL_ICPSELA_0~1
    UINT32 icpSelB; //PLL_CTRL_PLL_ICPSELB_0~1
    UINT32 rSel;   //PLL_CTRL_PLL_RSEL_(n)
 } PLL_CONFIG ;    
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择CLK_OUT时钟源                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Configure_Pll(PLL_CONFIG Pll_Config) 
{
	SGCC_CLK_P->PLL_CTRL = Pll_Config.refDiv | Pll_Config.fbDiv | Pll_Config.outDiv | Pll_Config.refClk | Pll_Config.icpSelA | Pll_Config.icpSelB | Pll_Config.rSel;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_ConfigMainClk                                                                                                                                                                            
  input:     	clk--HOSC/LOSC/PLL    
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择系统主时钟                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void CLK_Configure_Main_Clk(UINT8 Main_Clk) 
{
  
    switch(Main_Clk)
	{			
	  	case LOSC:
          CLK_Enable_LOSC(ENABLE);
          SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_OSC_SEL;
          SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_PLL_SEL;
	  	    break;
			
	  	case HOSC:
          CLK_Enable_HOSC(ENABLE);
          SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_OSC_SEL;
          SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_PLL_SEL;
	  	    break;
	  	    
      case PLL:
      	  CLK_Enable_PLL(ENABLE) ;
          SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_PLL_SEL;
	  	    break;
	  	      
      default:
            break;
	  }
}

////////////////////////////////////////////////////////////////////////////

/*-----------------------------------------------------------------------------
  Function:		CLK_EnAllClk                                                                                                                                                                            
  input:     	None                  	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 使能所有模块时钟                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnAllClk() 
{
	SGCC_IOCTRL_P->IO_PD4 &= ~(IO_PUD4_GPIO112|IO_PUD4_GPIO113);
	//SGCC_ANACTL_P->XHOSC_REG = 0xF;
	//SGCC_CLK_P->SYSCLK_CTRL &= ~0x0000008;
	//SGCC_CLK_P->SYSCLK_CTRL |= 0x0000000F|SYSCLK_CTRL_PHY_30M_EN;
	
	//SGCC_CLK_P->SYSCLK_CTRL |= 0x0000000F;
	
	//SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_HCLK_SEL;
	
	//SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_PLL_SEL_PLL|SYSCLK_CTRL_PLL_EN|SYSCLK_CTRL_PHY_30M_EN|SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV4;
	

	SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_PLL_EN|SYSCLK_CTRL_PHY_30M_EN;
	
	
	//CLK_SelectHclk(SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV2);
	
	CLK_SelectPclk(SYSCLK_CTRL_PCLK_SEL_HCLK_DIV4);
	
	SGCC_CLK_P->HCLK_EN = 0xff;
	SGCC_CLK_P->PCLK0_EN |= 0x7ffbf;  
	SGCC_CLK_P->PCLK1_EN |= 0xffff; 
	
}


/*-----------------------------------------------------------------------------
  Function:		EnPeriClk                                                                                                                                                                           
  input:     	module-外设模块                	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 使能某外设模块时钟 ,IOCTRL没有单独的时钟使能位                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnPeriClk(uint8_t module) 
{
	switch(module)
	{
	    case GPIO:
	    	SGCC_CLK_P->PCLK0_EN |= (PCLK0_EN_GPIO_DB | PCLK0_EN_GPIO);
	    	break;
      case UART0:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_UART0;
	    	break;
	    case UART1:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_UART1;
	    	break;
	    case UART2:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_UART2;
	    	break;
        case UART3:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_UART3;
	    	break;
	    case UART4:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_UART4;
	    	break;
	    case UART5:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_UART5;
	    	break;
	    case UART6:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_UART6;
	    	break;
        case UART7:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_UART7;
	    	break;	
      case DMA0:
	    	SGCC_CLK_P->HCLK_EN |= HCLK_EN_DMA0;
	    	break;    
	    case DMA1:
	    	SGCC_CLK_P->HCLK_EN |= HCLK_EN_DMA1;
	    	break;   	
	    case DMA2:
	    	SGCC_CLK_P->HCLK_EN |= HCLK_EN_DMA2;
	    	break;    	
	    case DMA3:
	    	SGCC_CLK_P->HCLK_EN |= HCLK_EN_DMA3;
	    	break;   	
	    case I2C0:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_I2C0;
	    	break;
	    case I2C1:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_I2C1;
	    	break;
	   case I2C2:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_I2C2;
	    	break; 	
     case U7816:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_7816;
	    	break;
     case PWM0:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_PWM0;
	    	break;
	   case PWM1:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_PWM1;
	    	break;
     case SPI0:
	    	SGCC_CLK_P->PCLK0_EN |= (PCLK0_EN_SPI0 | PCLK0_EN_SPI_MST0);
	    	break;
	   case SPI1:
	    	SGCC_CLK_P->PCLK0_EN |= (PCLK0_EN_SPI1 | PCLK0_EN_SPI_MST1);
	    	break;    
     case SPI2:
	    	SGCC_CLK_P->PCLK1_EN |= (PCLK1_EN_SPI2 | PCLK1_EN_SPI_MST2);
	    	break;      
     case SPI3:
	    	SGCC_CLK_P->PCLK1_EN |= (PCLK1_EN_SPI2 | PCLK1_EN_SPI_MST3);
	    	break;      
	   case CRC:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_CRC;
	    	break;
     case ANACTRL:
	    	SGCC_CLK_P->PCLK1_EN |= (PCLK1_EN_ANA | PCLK1_EN_ANA_W1);
	    	break;
     case ADC:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_ADC_CLK;
	    	break;
     case WDT:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_WDT;
	    	break;
	   case TIMERA:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_TIMERA;
	    	break;
	   case TIMERB:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_TIMERB;
	    	break;
	   case TIMERC:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_TIMERC;
	    	break;
	   case TIMERD:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_TIMERD;
	    	break;
	   case RTC:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_RTC;
	    	break;
	   case GMAC0:
	      SGCC_CLK_P->HCLK_EN |= HCLK_EN_GMAC0;
	      SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_GMAC_PADCLK_EN;
	    	break;
	   case GMAC1:
	      SGCC_CLK_P->HCLK_EN |= HCLK_EN_GMAC1;
	      SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_GMAC_PADCLK_EN;
	    	break;
	   case OTG:
	      SGCC_CLK_P->HCLK_EN |= HCLK_EN_OTG;
	      SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_PHY_30M_EN;
	    	break;
	   case FLASH:
	      SGCC_CLK_P->HCLK_EN |= HCLK_EN_FLASH;
	    	break; 	
	    default:
            break;
	    
	}
	
}

/*-----------------------------------------------------------------------------
  Function:		CLK_DisPeriClk                                                                                                                                                                           
  input:     	module-外设模块                	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 不使能某外设模块时钟                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void CLK_DisPeriClk(uint8_t module) 
{	
	switch(module)
	{
	    case GPIO:
	    	SGCC_CLK_P->PCLK0_EN &= ~(PCLK0_EN_GPIO_DB | PCLK0_EN_GPIO);
	    	break;
      case UART0:
	    	SGCC_CLK_P->PCLK0_EN &= ~ PCLK0_EN_UART0;
	    	break;
	    case UART1:
	    	SGCC_CLK_P->PCLK0_EN &= ~ PCLK0_EN_UART1;
	    	break;
	    case UART2:
	    	SGCC_CLK_P->PCLK0_EN &= ~ PCLK0_EN_UART2;
	    	break;
        case UART3:
	    	SGCC_CLK_P->PCLK0_EN &= ~ PCLK0_EN_UART3;
	    	break;
	    case UART4:
	    	SGCC_CLK_P->PCLK1_EN &= ~ PCLK1_EN_UART4;
	    	break;
	    case UART5:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_UART5;
	    	break;
	    case UART6:
	    	SGCC_CLK_P->PCLK1_EN &= ~ PCLK1_EN_UART6;
	    	break;
        case UART7:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_UART7;
	    	break;	
      case DMA0:
	    	SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_DMA0;
	    	break;    
	    case DMA1:
	    	SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_DMA1;
	    	break;   	
	    case DMA2:
	    	SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_DMA2;
	    	break;    	
	    case DMA3:
	    	SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_DMA3;
	    	break;   	
	    case I2C0:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_I2C0;
	    	break;
	    case I2C1:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_I2C1;
	    	break;
	   case I2C2:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_I2C2;
	    	break; 	
     case U7816:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_7816;
	    	break;
     case PWM0:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_PWM0;
	    	break;
	   case PWM1:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_PWM1;
	    	break;
     case SPI0:
	    	SGCC_CLK_P->PCLK0_EN &= ~(PCLK0_EN_SPI0 | PCLK0_EN_SPI_MST0);
	    	break;
	   case SPI1:
	    	SGCC_CLK_P->PCLK0_EN &= ~(PCLK0_EN_SPI1 | PCLK0_EN_SPI_MST1);
	    	break;    
     case SPI2:
	    	SGCC_CLK_P->PCLK1_EN &= ~(PCLK1_EN_SPI2 | PCLK1_EN_SPI_MST2);
	    	break;      
     case SPI3:
	    	SGCC_CLK_P->PCLK1_EN &= ~(PCLK1_EN_SPI2 | PCLK1_EN_SPI_MST3);
	    	break;      
	   case CRC:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_CRC;
	    	break;
     case ANACTRL:
	    	SGCC_CLK_P->PCLK1_EN &= ~(PCLK1_EN_ANA | PCLK1_EN_ANA_W1);
	    	break;
     case ADC:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_ADC_CLK;
	    	break;
     case WDT:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_WDT;
	    	break;
	   case TIMERA:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_TIMERA;
	    	break;
	   case TIMERB:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_TIMERB;
	    	break;
	   case TIMERC:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_TIMERC;
	    	break;
	   case TIMERD:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_TIMERD;
	    	break;
	   case RTC:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_RTC;
	    	break;
	   case GMAC0:
	      SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_GMAC0;
	      SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_GMAC_PADCLK_EN;
	    	break;
	   case GMAC1:
	      SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_GMAC1;
	      SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_GMAC_PADCLK_EN;
	    	break;
	   case OTG:
	      SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_OTG;
	      SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_PHY_30M_EN;
	    	break;
	   case FLASH:
	      SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_FLASH;
	    	break; 	
	    default:
            break;
	    
	}
	    
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnableMatrix                                                                                                                                                                           
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable matrix hclk                                                                                              	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnableMatrix(uint8_t opt) 
{
    if(opt == ENABLE)
        SGCC_CLK_P->SYSCLK_CTRL |= HCLK_EN_MTX;
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~HCLK_EN_MTX;    
    
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnableHosc                                                                                                                                                                           
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable HOSC                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnableHosc(uint8_t opt) 
{
    if(opt == ENABLE)
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_HOSC_EN;
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_HOSC_EN; 
        
    while(!(SGCC_CLK_P->SYSCLK_CTRL & SYSCLK_CTRL_HCLK_RD));   
    
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnableLosc                                                                                                                                                                           
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable LOSC                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnableLosc(uint8_t opt) 
{
    if(opt == ENABLE)
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_LOSC_EN;
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_LOSC_EN;    
    
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnablePll                                                                                                                                                                          
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable PLL                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnablePll(uint8_t opt) 
{
    if(opt == ENABLE)
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_PLL_EN;
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_PLL_EN;  
        
    //wait until PLL locked
    while(!(SGCC_CLK_P->SYSCLK_CTRL & SYSCLK_CTRL_PLL_LOCKED_O));  
    
}
/*-----------------------------------------------------------------------------
  Function:		CLK_EnableXhosc                                                                                                                                                                           
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable XHOSC                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnableXhosc(uint8_t opt) 
{
    if(opt == ENABLE)
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_XHOSC_EN;
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_XHOSC_EN;    
        
    //wait until xhosc ready
    while(!(SGCC_CLK_P->SYSCLK_CTRL & SYSCLK_CTRL_XHOSC_RD));
    
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnableXlosc                                                                                                                                                                           
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable XLOSC                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnableXlosc(uint8_t opt) 
{
    if(opt == ENABLE)
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_XLOSC_EN;
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_XLOSC_EN;    
    
}

/*-----------------------------------------------------------------------------
  Function:		CLK_SelectHclk                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV1/SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV2/SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV4/SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV8
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择ADC时钟为1M/2MHz                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_SelectHclk(uint32_t clk) 
{
	uint32_t temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_HCLK_SEL;
	temp |= clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_SelectPclk                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_PCLK_SEL_HCLK_DIV2/SYSCLK_CTRL_PCLK_SEL_HCLK_DIV4
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择ADC时钟为1M/2MHz                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_SelectPclk(uint32_t clk) 
{
	uint32_t temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_PCLK_SEL;
	temp |= clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_ConfigAdcPllDiv                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_ADC_PLL_DIV_32/SYSCLK_CTRL_ADC_PLL_DIV_64/SYSCLK_CTRL_ADC_PLL_DIV_128/SYSCLK_CTRL_ADC_PLL_DIV_256                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择ADC使用PLL的分频系数                                                                                              	                                   
 *-----------------------------------------------------------------------------*/
void CLK_ConfigAdcPllDiv(uint32_t clk) 
{
	uint32_t temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_ADC_PLL_DIV;
	temp |= clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_ConfigAdcHoscDiv                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_ADC_HOSC_DIV_8/SYSCLK_CTRL_ADC_HOSC_DIV_16/SYSCLK_CTRL_ADC_HOSC_DIV_32/SYSCLK_CTRL_ADC_HOSC_DIV_64                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择ADC使用HOSC的分频系数                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_ConfigAdcHoscDiv(uint32_t clk) 
{
	uint32_t temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_ADC_HOSC_DIV;
	temp |= clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}
/*-----------------------------------------------------------------------------
  Function:		CLK_SelectRtcClk                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_RTC_CLK_SEL_XLOSC/SYSCLK_CTRL_RTC_CLK_SEL_EXTAL32
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择RTC时钟为XLOSC或者external 32K                                                                                              	                                   
 *-----------------------------------------------------------------------------*/
void CLK_SelectRtcClk(uint32_t clk) 
{
	uint32_t temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_RTC_CLK_SEL;
	temp |= clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_SelectGpioDbClk                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_GPIO_DBCLK_SEL_LOSC/SYSCLK_CTRL_GPIO_DBCLK_SEL_PCLK
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择GPIO DBCLK为LOSC或者PCLK                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_SelectGpioDbClk(uint32_t clk) 
{
	uint32_t temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_GPIO_DBCLK_SEL;
	temp |= clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}
/*-----------------------------------------------------------------------------
  Function:		CLK_SelectClkOut                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_ANA_CLK_MUX_LOSC/SYSCLK_CTRL_ANA_CLK_MUX_XLOSC/
              SYSCLK_CTRL_ANA_CLK_MUX_XHOSC_DIV4/SYSCLK_CTRL_ANA_CLK_MUX_HOSC_DIV16/
              SYSCLK_CTRL_ANA_CLK_MUX_PLL_DIV64     
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择CLK_OUT时钟源                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void CLK_SelectClkOut(uint32_t clk) 
{
	uint32_t temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_ANA_CLK_MUX;
	temp |= clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnClkOut                                                                                                                                                                          
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable clk_out                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnClkOut(uint8_t opt) 
{
    if(opt == ENABLE)
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_ANA_CLK_EN;
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_ANA_CLK_EN;    
    
}


/*-----------------------------------------------------------------------------
  Function:		CLK_ConfigPll                                                                                                                                                                            
  input:     	pllConfig
 typedef struct
 {
    uint32_t refDiv;  //PLL_CTRL_PLL_REFDIV_1~4
    uint32_t fbDiv;   //PLL_CTRL_PLL_FBDIV_(n), n=0x0~0x7f
    uint32_t outDiv;  //PLL_CTRL_PLL_OUTDIV_1~4
    uint32_t refClk;  //PLL_CTRL_PLL_REF_XHOSC/PLL_CTRL_PLL_REF_HOSC/PLL_CTRL_PLL_REF_PAD50M
    uint32_t icpSelA; //PLL_CTRL_PLL_ICPSELA_0~1
    uint32_t icpSelB; //PLL_CTRL_PLL_ICPSELB_0~1
    uint32_t rSel;   //PLL_CTRL_PLL_RSEL_(n)
 } PLL_CONFIG ;    
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择CLK_OUT时钟源                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void CLK_ConfigPll(PLL_CONFIG pllConfig) 
{
	SGCC_CLK_P->PLL_CTRL = pllConfig.refDiv | pllConfig.fbDiv | pllConfig.outDiv | pllConfig.refClk | pllConfig.icpSelA | pllConfig.icpSelB | pllConfig.rSel;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_ConfigMainClk                                                                                                                                                                            
  input:     	clk--HOSC/LOSC/PLL    
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择系统主时钟                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void CLK_ConfigMainClk(uint8_t clk) 
{
  
    switch(clk)
	{			
	  	case LOSC:
          CLK_EnableLosc(ENABLE);
          SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_OSC_SEL;
          SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_PLL_SEL;
	  	    break;
			
	  	case HOSC:
          CLK_EnableHosc(ENABLE);
          SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_OSC_SEL;
          SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_PLL_SEL;
	  	    break;
	  	    
      case PLL:
      	  CLK_EnablePll(ENABLE) ;
          SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_PLL_SEL;
	  	    break;
	  	      
      default:
            break;
	  }
}

