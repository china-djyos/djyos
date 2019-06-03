/***************************************************************************
描述：TC routine
平台：
时间：2017-8-8
作者：张彦欣                                                 
****************************************************************************/
#include "SCM6XX_TIMER.h"

/*-----------------------------------------------------------------------------
  Function:		TC_RestoreReg                                                                                                                                                                           
  input:     TCx：select which timer to configure     	  
  output:     None                                        
  Returns:    None                                                                                      
  Description: 恢复置TC相关寄存器为上电复位值                                                                                                   	                                   
 *-----------------------------------------------------------------------------*/
void TC_Reset_Reg(SGCC_TC_TypeDef *TCx)
{
    TCx->FSR = 0;    
    TCx->CR = 2;
    TCx->CR = 0;        
    TCx->PR = 0;       
    TCx->PC = 0;       
    TCx->MR = 0;       
    TCx->MCR  = 0;      
    TCx->EMR = 0;      
    TCx->EWCR = 0;  
    TCx->CCR = 0;
    TCx->CAPTR = 0;             
    TCx->INTIF = 0;     
    TCx->INTIE = 0;    
   
}
/*-----------------------------------------------------------------------------
  Function:		TC_Enable                                                                                                                                                                           
  input:     	TCx：select which timer 
                opt: ENABLE or DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: enable interrupt                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void TC_Enable_Int(SGCC_TC_TypeDef *TCx, FunctionalState opt) 
{
    TCx->CR = TC_TCCR_TMRST;
    
    if(opt == ENABLE)
    {
        TCx->CR = (~TC_TCCR_TMRST) | TC_TCCR_TMEN;
    }
    else
    {
        TCx->CR = ~(TC_TCCR_TMRST | TC_TCCR_TMEN);
    }	
}

/*-----------------------------------------------------------------------------
  Function:		TC_EnableIntr                                                                                                                                                                           
  input:     	TCx：select which timer 
             intrType:  TC_INTR_MR/TC_INTR_TCOV/TC_INTR_CAPRE/TC_INTR_CAPFE   
              opt: ENABLE or DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: enable interrupt                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void TC_Enable_Int_Src(SGCC_TC_TypeDef *TCx, UINT8 Timer_Int_Src, FunctionalState opt) 
{
    if(opt == ENABLE)
    {
	    TCx->INTIE |= Timer_Int_Src;
    }
    else
    {
        TCx->INTIE &=  ~Timer_Int_Src;
    }
}
/*-----------------------------------------------------------------------------
  Function:		TC_ClearIntr                                                                                                                                                                           
  input:    TCx：select which timer 	
            intrType  TC_INTR_MR/TC_INTR_TCOV/TC_INTR_CAPRE/TC_INTR_CAPFE                         	  
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: clear interrupt                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void TC_Clear_Int_Flag(SGCC_TC_TypeDef *TCx, UINT8 Timer_Int_Src) 
{
    TCx->INTIF |=  Timer_Int_Src;	
}

/*-----------------------------------------------------------------------------
  Function:		TC_GetIntrStatus                                                                       
  input:    TCx: select the timer        
            flag:which interrupt to get, can be TC_INTR_MR/TC_INTR_TCOV/TC_INTR_CAPRE/TC_INTR_CAPFE
  Returns:  None                                                                                        
  Description:  get UART status                                                                                         
               	                                  
 *-----------------------------------------------------------------------------*/
UINT8 TC_Get_Int_Status(SGCC_TC_TypeDef *TCx, UINT8 Timer_Int_Src)
{
    if(TCx->INTIF & Timer_Int_Src)
    {
        return SUCCESS;
    }
    else
    {
        return ERROR;
    }
}

/*-----------------------------------------------------------------------------
  Function:		TC_Timer16Bit                                                                                                                                                                           
  input:     	TCx：select which timer to work
              prescale--预分频
              matchEn--是否使能匹配，若匹配使能，则配置匹配寄存器、使能匹配中断并配置匹配发生时输出高低电平，若不使能匹配，则使能溢出中断
              matchVal--匹配寄存器        	  
  output:     None                                        
  Returns:    None                                                                                      
  Description: 配置TC工作于16位定时模式                                                                                                    	                                   
 *-----------------------------------------------------------------------------*/
void TC_Timer_Set_16Bit(SGCC_TC_TypeDef *TCx, FunctionalState Match_En, TC_16BitTimerCFG TC_Timer_Cfg, FunctionalState Int_En) 
{
   //不使能TC
   	TCx->CR = 0;
   	//tc mode
   	TCx->FSR = TC_TCFSR_TCSEL_TIMER;
   	//prescale
   	TCx->PR  = 	TC_Timer_Cfg.prescale;  
   	if(Match_En)
   	{
   	  	//match value
   	  	TCx->MR = TC_Timer_Cfg.matchVal;
   	    //enable match, 发生match时TC清零  
   	    TCx->MCR = TC_Timer_Cfg.matchCtl | 0x1;
		if(Int_En)
		{
   	        //enable match interrupt
   	        TCx->INTIE |= 0x1;
		}
        else
		{
   	        //disable match interrupt
   	        TCx->INTIE &= ~0x1;
		}	  
   	    //out control,输出高低电平
   	    TCx->EMR = TC_Timer_Cfg.outMode ;
		//若输出脉冲，80ms宽度
   	    TCx->EWCR = TC_Timer_Cfg.outPulseW;	  
   	}
   	else
   	{
		if(Int_En)
		{
   	        //enable overflow interrupt
   	        TCx->INTIE = 0x2;
		}
		else
		{
   	            //disable overflow interrupt
   	        TCx->INTIE &= ~0x2;
		}	  
   	    	  
   	}
   	    
   	//enable TC
   	TCx->CR = 0x2;
   	TCx->CR = 0x1;
	
}

/*-----------------------------------------------------------------------------
  Function:		TC_Timer32Bit                                                                                                                                                                            
  input:     	TCx--select which timer to work in 32bit mode
              prescale--预分频
              matchEn--是否使能匹配，若匹配使能，则配置匹配寄存器、使能匹配中断并配置匹配发生时输出高低电平，若不使能匹配，则使能溢出中断
              matchValL--匹配寄存器低16bit 
              matchValH--匹配寄存器高16bit       	  
  output:     None                                        
  Returns:    None                                                                                      
  Description: 配置TC工作于32位定时模式                                                                                                    	                                   
 *-----------------------------------------------------------------------------*/

void TC_Timer_Set_32Bit(SGCC_TC_TypeDef *TCx, UINT16 Prescale, FunctionalState Match_En, UINT16 Match_Val_L, UINT16 Match_Val_H) 
{
	
	SGCC_TC_TypeDef *TCy =   (SGCC_TC_TypeDef *)((UINT32)TCx + 0x40);
  
   	//不使能TC
   	TCx->CR = 0;
   	TCy->CR = 0;
   	
   	//tc mode
   	TCx->FSR = 0;
   	TCy->FSR = TC_TCFSR_CNTSRC_LOWOV | 0x1;
   		
   	//prescale
   	TCx->PR = 	Prescale;  
   	
   	if(Match_En)
   	{
   	    //match value
   	    TCx->MR = Match_Val_L;
   	    TCy->MR = Match_Val_H;
   	  	    
					  
   	    //enable match, 发生match时TC清零  
   	    TCy->MCR = 0x3;
   	    //enable match interrupt
   	    TCy->INTIE = 0x1;
   	    //out control,输出高低电平
   	    TCy->EMR = 0x3;
   	}
   	else
   	{
   	    //enable overflow interrupt
   	    TCy->INTIE = 0x2;
   	}
   	    
   	//enable TC
   	TCy->CR = 0x1;
   
   		
}

/*-----------------------------------------------------------------------------
  Function:		TC_Counter16Bit，                                                                                                                                                                           
  input:     	TCx：select which timer 
              matchEn-是否使能匹配
              tcCfg--参数的各成员变量值       	  
  output:     None                                        
  Returns:    None                                                                                      
  Description: 配置TC工作于16位计数模式                                                                                                       	                                   
 *-----------------------------------------------------------------------------*/
void TC_Counter_Set_16Bit(SGCC_TC_TypeDef *TCx,FunctionalState Match_En, TC_16BitCNTCFG TC_Counter_Cfg, FunctionalState Int_En) 
{
	UINT32 temp ;

	//counter mode
    temp = TC_Counter_Cfg.cntMode;
    temp |= TC_Counter_Cfg.cntSrc ;
    //不使能TC
   	TCx->CR = 0;
   	//counter mode
   	TCx->FSR = temp;
   	//prescale
	TCx->PR = TC_Counter_Cfg.prescale;
   	if(Match_En)
   	{
   	  	//match value
   	  	TCx->MR = TC_Counter_Cfg.matchVal;
   	    //enable match, 发生match时TC清零  
   	    TCx->MCR = 0x3;
   	    if(Int_En)
		{
   	        //enable match interrupt
   	        TCx->INTIE |= 0x1;
		}
		else
		{
   	        //disable match interrupt
   	        TCx->INTIE &= ~0x1;
		}	  
   	    //out control,输出80ms的脉冲
   	    TCx->EMR = TC_Counter_Cfg.outMode;
   	    TCx->EWCR = 2621;
   	    }
   	    else
   	    {
   	    	if(Int_En)
			{
   	            //enable overflow interrupt
   	            TCx->INTIE = 0x2;
			}
			else
			{
   	            //disable overflow interrupt
   	            TCx->INTIE &= ~0x2;
			}	  
   	    }
   	    
   	    //enable TC
   	    TCx->CR = 0x2;
   	    TCx->CR = 0x1;	
   
}

/*-----------------------------------------------------------------------------
  Function:		TC_Counter32Bit，                                                                                                                                                                           
  input:     	TCx：select which timer 
              tcCfg--参数的各成员变量值       	  
  output:     None                                        
  Returns:    None                                                                                      
  Description: 配置TC工作于32位计数模式                                                                                                       	                                   
 *-----------------------------------------------------------------------------*/
void TC_Counter_Set_32Bit(SGCC_TC_TypeDef *TCx,TC_32BitCNTCFG TC_Counter_Cfg) 
{
	UINT32 temp ;
    SGCC_TC_TypeDef *TCy =   (SGCC_TC_TypeDef *)((UINT32)TCx + 0x40);
    
	//counter mode
    temp = TC_Counter_Cfg.cntMode;
    temp |= TC_Counter_Cfg.cntSrc ;
  
 
   //不使能TC0/1
   	TCx->CR = 0;
   	TCy->CR = 0;
   	
   	//counter mode
   	TCx->FSR = temp;
   	TCy->FSR =  TC_TCFSR_CNTSRC_LOWOV| 0x1;  //TC1的计数源为TC0的溢TC_TCFSR_CNTSRC_LOWOV
   	
   	//prescale
	TCx->PR = TC_Counter_Cfg.prescale;
   	    
   	//match value
   	TCx->MR = TC_Counter_Cfg.matchValL;
   	TCy->MR = TC_Counter_Cfg.matchValH;
   	
   	//enable match, 发生match时TC清零  
   	TCy->MCR = 0x3;
   	       
   	//enable match interrupt
   	TCy->INTIE |= 0x1;

   	//out control,if输出脉冲，脉冲宽度80ms
   	TCy->EMR = TC_Counter_Cfg.outMode;
   	TCy->EWCR = 2621;
    //enable TC0/TC1
   	TCy->CR = 0x2;
   	TCy->CR = 0x1;	
}
/*-----------------------------------------------------------------------------
  Function:		TC_Capture16Bit                                                                                                                                                                         
  input:     	TCx：select which timer 
              tcCfg--capture配置 	  
  output:     None                                        
  Returns:    None                                                                                      
  Description: 配置TC工作于定时模式且捕捉模式使能                                                                                                          	                                   
 *-----------------------------------------------------------------------------*/
void TC_Capture_Set_16Bit(SGCC_TC_TypeDef *TCx, TC_CAPTCFG TC_Capture_Cfg,FunctionalState Int_En) 
{
    //不使能TC
   	TCx->CR = 0;		
	TCx->CCR = 0x0;
	//clear interrupt
	TCx->INTIF = 0;
			  
			
   	//tc mode & capture source
   	TCx->FSR = TC_Capture_Cfg.capSrc ;
   	//prescale
   	TCx->PR = 	TC_Capture_Cfg.prescale; 
   	  	
			  
   	if(TC_Capture_Cfg.capEdge == TC_CCR_CAPRE)
   	{
   	  	TCx->CCR |= 0x2;
		if(Int_En)
		{
   	        //enable capture interrupt
   	        TCx->INTIE |= 0x4;
		}
		else
		{
   	        //disable capture interrupt
   	        TCx->INTIE &= ~0x4;
		}	  
   	  	    
   	}
   	else if(TC_Capture_Cfg.capEdge == TC_CCR_CAPFE) 
   	{
   	  	TCx->CCR |= 0x4;
		if(Int_En)
		{
   	        //enable capture interrupt
   	        TCx->INTIE |= 0x8;
		}
		else
		{
   	        //disable capture interrupt
   	        TCx->INTIE &= ~0x8;
		}	  
   	  		  
   	}	
   	else  //上升和下降沿捕捉
   	{
   	    TCx->CCR |= 0x6;
		if(Int_En)
		{
   	        //enable capture interrupt
   	        TCx->INTIE |= 0xc;
		}
		else
		{
   	        //disable capture interrupt
   	        TCx->INTIE &= ~0xc;
		}	  
   	}
   	    
	//使能TC
   	TCx->CR = 0x2;
   	TCx->CR = 1;
			
   	//capture control
   	TCx->CCR |= 0x1;  //enable capture
	
  
  
}

/*-----------------------------------------------------------------------------
  Function:		TC_Capture32Bit                                                                                                                                                                         
  input:     	TCx：select which timer 
              tcCfg--捕捉模式配置 	  
  output:     None                                        
  Returns:    None                                                                                      
  Description: 配置TC工作于定时模式且捕捉模式使能                                                                                                          	                                   
 *-----------------------------------------------------------------------------*/
void TC_Capture_Set_32Bit(SGCC_TC_TypeDef *TCx, TC_CAPTCFG TC_Capture_Cfg) 
{
    SGCC_TC_TypeDef *TCy =   (SGCC_TC_TypeDef *)((UINT32)TCx + 0x40);
    
   	//不使能TC0/1的定时功能和TC1的捕捉功能
   	TCx->CR = 0;		
   	TCy->CR = 0;		
	TCy->CCR = 0x0;
	//clear interrupt
	TCy->INTIF = 0;
 
   	//tc mode & capture source
   	TCy->FSR = TC_Capture_Cfg.capSrc | TC_TCFSR_CNTSRC_LOWOV;  //TC1的计数源为TC0的溢出;捕捉源来自参数传递
   	//prescale
   	TCx->PR = 	TC_Capture_Cfg.prescale;   
   	if(TC_Capture_Cfg.capEdge == TC_CCR_CAPRE)
   	{
   	    TCy->CCR |= 0x2;
		//enable capture interrupt
        TCy->INTIE |= 0x4;
    }
   	else if(TC_Capture_Cfg.capEdge == TC_CCR_CAPFE) 
   	{
   		TCy->CCR |= 0x4;
	    //enable capture interrupt
   	    TCy->INTIE |= 0x8;
    }	
   	else  //上升和下降沿捕捉
   	{
   	   	TCy->CCR |= 0x6;
		//enable capture interrupt
   	    TCy->INTIE |= 0xc;
    }
   	    
	//使能TC
   	TCy->CR = 0x2;
   	TCy->CR = 1;
   	//capture control
   	TCy->CCR |= 0x1;  //enable capture
   	  
}
///*-----------------------------------------------------------------------------
//  Function:		Delay                                                                                                                                                                            
//  input:     	time，单位为ms                   	  
//  output:     None                                                                                    
//  Returns:    None                                                                                      
//  Description: 使用tc0定时                                                                                                 	                                   
// *-----------------------------------------------------------------------------*/
//void Delay( UINT16 time )
//{
//	TC_16BitTimerCFG tcCfg;
//	
//  TC_Reset_Reg(SGCC_TIMERC0_P);
//	tcCfg.prescale = 999;   //k
//	tcCfg.matchVal = time;
//	tcCfg.outMode = 3;
//    tcCfg.outPulseW = 0;
//	SGCC_TIMERC0_P->INTIF = 0;
//    TC_Timer_Set_16Bit(SGCC_TIMERC0_P, ENABLE, tcCfg, DISABLE) ;
//    
//	//等待计数溢出中断
//	while(!(SGCC_TIMERC0_P->INTIF & TC_INTR_MR));
//	
//	//清除中断
//    SGCC_TIMERC0_P->INTIF = 0;
//	//disable timer
//    SGCC_TIMERC0_P->CR = 0;
//	
//	TC_Reset_Reg(SGCC_TIMERC0_P);
//}


///*-----------------------------------------------------------------------------
//  Function:		DelayUs                                                                                                                                                                            
//  input:     	time，单位us                  	  
//  output:     None                                                                                    
//  Returns:    None                                                                                      
//  Description: 使用tc1定时                                                                                                 	                                   
// *-----------------------------------------------------------------------------*/
//void DelayUs( UINT16 time )
//{
//	TC_16BitTimerCFG tcCfg;
//	
//    TC_Reset_Reg(SGCC_TIMERC0_P);
//    
//	tcCfg.prescale = 39;   //40M/250k
//	tcCfg.matchVal = time;
//	tcCfg.outMode = 3;
//	tcCfg.outPulseW = 0;
//	SGCC_TIMERC0_P->INTIF = 0;
//    TC_Timer_Set_16Bit(SGCC_TIMERC0_P,ENABLE, tcCfg, DISABLE) ;
//    
//	//等待计数溢出中断
//	while(!(SGCC_TIMERC0_P->INTIF & TC_INTR_MR));
//	
//	//清除中断
//    SGCC_TIMERC0_P->INTIF = 0;
//	//disable timer
//    SGCC_TIMERC0_P->CR = 0;
//	
//	TC_Reset_Reg(SGCC_TIMERC0_P);
//}

