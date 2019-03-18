/***************************************************************************
������GPIO routine
ƽ̨��SCM6XX
ʱ�䣺2017-8-27
���ߣ�                                                
****************************************************************************/
#include "SCM6XX_GPIO.h"

/*-----------------------------------------------------------------------------
  Function:		GPIO_RestoreVal                                                                                                                                                                            
  input:      None          	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ����GPIO�ܽŵķ�������or���                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_Reset_Reg(void) 
{
    SGCC_GPIO_P->DR = 0;
	  SGCC_GPIO_P->DIR = 0;
	  SGCC_GPIO_P->INTER = 0;
	  SGCC_GPIO_P->INTMR = 0;

	  SGCC_GPIO_P->DBSYR = 0;
	  SGCC_GPIO_P->INTCR = 0xffffffff;
	  SGCC_GPIO_P->PULLEN = 0;
}

/*-----------------------------------------------------------------------------
  Function:		GPIO_ConfigDir                                                                                                                                                                            
  input:    portNum: port number    (0-113)
            dir��
            	GPIO_INPUT:����
              GPIO_OUTPUT:���                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ����GPIO�ܽŵķ�������or���
  						4��gpio��ģ�飨��1��2��3��4��������ַ����Ϊ��0x40ai_0000��i=0��1��2��3��
								
 *-----------------------------------------------------------------------------*/
void GPIO_Configure_Dir(UINT8 Port_Num, UINT8 Port_Dir) 
{
	  UINT32 temp;
	  SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
		regnum = Port_Num / 32;
		 
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
	 	temp = GPIO_P->DIR;
	  
	  if(regnum)
		Port_Num = Port_Num%32;

		temp &= ~(1 << Port_Num);
	  temp |= (Port_Dir << Port_Num);

	
	  GPIO_P->DIR = temp ;
	  
}


/*-----------------------------------------------------------------------------
  Function:		GPIO_WrDataOut                                                                                                                                                                            
  input:    portNum: port number 
            level��GPIO_HIGH/GPIO_LOW            	                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ʹGPIO����ߵ͵�ƽ                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_Write_Out(UINT8 Port_Num, UINT8 Port_Level) 
{
	  UINT32 temp;
	  SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
		regnum = Port_Num / 32;
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	  temp = GPIO_P->DR;
	
		if(regnum)
			Port_Num = Port_Num%32;
		
		temp &= ~(1 << Port_Num);
		temp |= (Port_Level << Port_Num);

    GPIO_P->DR = temp ;
	
}

/*-----------------------------------------------------------------------------
  Function:		GPIO_RdDataIn                                                                                                                                                                            
  input:    portNum: port number     	                   	  
  output:     None                                                                                    
  Returns:    gpio level                                                                                      
  Description: ����GPIO port�ĵ�ƽ                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
UINT8 GPIO_Read_In(UINT8 Port_Num) 
{
	  UINT32 temp;
	  SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
	
		regnum = Port_Num / 32;
	
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
		if(regnum)
			Port_Num = Port_Num%32;
	
	  temp = GPIO_P->ETPR;
	
	  temp = (temp >> Port_Num) & 0x1;
	  
	  return((UINT8)temp);
}	  


/*-----------------------------------------------------------------------------
  Function:		GPIO_EnIntr                                                                                                                                                                            
  input:    portNum: port number 
            opt: ENABLE/DISABLE          	                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ʹ�ܻ�ʹ���ж�                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_Enable_Int(UINT8 Port_Num, FunctionalState opt) 
{
	  SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
	
		regnum = Port_Num / 32;
	
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
		if(regnum)
			Port_Num = Port_Num%32;
		
    if(opt == ENABLE)
    {
        GPIO_P->INTER |= (1 << Port_Num);
    }
    else
    {
        GPIO_P->INTER &= ~(1 << Port_Num);
    }

}	  
	
/*-----------------------------------------------------------------------------
  Function:		GPIO_MaskIntr                                                                                                                                                                            
  input:    portNum: port number 
            intrEn: GPIO_INTR_MASK/GPIO_INTR_UNMASK          	                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ���λ�������ж�                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_Mask_Int(UINT8 Port_Num, UINT8 Int_Mask) 
{
	  UINT32 temp;
	   SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
	
		regnum = Port_Num / 32;
	
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
		if(regnum)
			Port_Num = Port_Num%32;
		
	  	temp = GPIO_P->INTMR;
	  	temp &= ~(1 << Port_Num);
	    temp |= (Int_Mask << Port_Num);
	    GPIO_P->INTMR= temp ;

}	  

/*-----------------------------------------------------------------------------
  Function:		GPIO_ClrIntr                                                                                                                                                                            
  input:    portNum: port number           	                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ����ж�                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_Clear_Int_Flag(UINT8 Port_Num) 
{
		SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
	
		regnum = Port_Num / 32;
	
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
		if(regnum)
			Port_Num = Port_Num%32;

	    GPIO_P->INTCR = (1 << Port_Num) ;
}	  

/*-----------------------------------------------------------------------------
  Function:		GPIO_ConfigIntrMode                                                                                                                                                                           
  input:    portNum: port number 
            trigMode: 
            GPIO_INTR_RISINGEDGE  
            GPIO_INTR_FALLINGEDGE  
            GPIO_INTR_HIGHLVEL   
            GPIO_INTR_LOWLEVEL  
            GPIO_INTR_BOTHEDGE       	                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: �ж�ģʽ����                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_Configure_Int_Mode(UINT8 Port_Num, UINT8 Trig_Mode) 
{
	  UINT32 temp;
	  UINT8 div;
    UINT8 offSet;
	
	
		SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
	
		regnum = Port_Num / 32;
	
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
		if(regnum)
			Port_Num = Port_Num%32;
	
	
    
    div = Port_Num / 8;
    offSet = div * 8 + 4;
    
    temp = GPIO_P->DBSYR;
    
    switch(div)
    {
    	case 0:
    		temp &= ~GPIO_DBSYR_INTER_MODE1;
    		break;
    	case 1:
    		temp &= ~GPIO_DBSYR_INTER_MODE2;
    		break;	
    	case 2:
    		temp &= ~GPIO_DBSYR_INTER_MODE3;
    		break;	
    	case 3:
    		temp &= ~GPIO_DBSYR_INTER_MODE4;
    		break;
    	default:
    		break;
    }
    
    temp |= ((UINT32)Trig_Mode << offSet);
	GPIO_P->DBSYR = temp;
	     	    
	  
}	  

/*-----------------------------------------------------------------------------
  Function:		GPIO_RdIntrMode                                                                                                                                                                           
  input:    portNum: port number     	                   	  
  output:     None                                                                                    
  Returns:    gpio level                                                                                      
  Description: return GPIO port's trigger mode                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
UINT8 GPIO_Read_Int_Mode(UINT8 Port_Num) 
{
	UINT32 temp;
	UINT8 div;
    UINT8 offSet;
	
	SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
	
		regnum = Port_Num / 32;
	
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
		if(regnum)
			Port_Num = Port_Num%32;
	
	
    
    div = Port_Num / 8;
    offSet = div * 8 + 4;
	
    temp = GPIO_P->DBSYR;
	temp = (temp >> offSet) & 0x7;
	  
	  
	return((UINT8)temp);
}	  

/*-----------------------------------------------------------------------------
  Function:		GPIO_ConfigDebounce                                                                                                                                                                            
  input:     portNum- port number 
             debounceMode- debounce mode
             debounceCycle- debounce cycle       	                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ȥë��ʹ��                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_Configure_Debounce(UINT8 Port_Num, UINT8 Debounce_Mode, UINT8 Debounce_Cycle) 
{
	  UINT32 temp;

	  UINT8 div;
    UINT8 offSet;
    
		
	SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
	
		regnum = Port_Num / 32;
	
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
		if(regnum)
			Port_Num = Port_Num%32;
	
	
    div = Port_Num / 8;
    offSet = div * 8 ;
    
    temp = GPIO_P->DBSYR;
    
    switch(div)
    {
    	case 0:
    		temp &= ~(GPIO_DBSYR_DEBOUNCE1 | GPIO_DBSYR_DEBOUNCE_CYCLE1);
    		break;
    	case 1:
    		temp &= ~(GPIO_DBSYR_DEBOUNCE2 | GPIO_DBSYR_DEBOUNCE_CYCLE2);
    		break;	
    	case 2:
    		temp &= ~(GPIO_DBSYR_DEBOUNCE3 | GPIO_DBSYR_DEBOUNCE_CYCLE3);
    		break;	
    	case 3:
    		temp &= ~(GPIO_DBSYR_DEBOUNCE4 | GPIO_DBSYR_DEBOUNCE_CYCLE4);
    		break;
    	default:
    		break;
    }
    
    temp |= ((UINT32)Debounce_Mode << offSet);
    temp |= ((UINT32)Debounce_Cycle << (offSet + 2));
    
	  GPIO_P->DBSYR = temp;
	  
}


/*-----------------------------------------------------------------------------
  Function:		GPIO_EnSyn                                                                                                                                                                            
  input:     portNum: port number 
             synEn:enable or disable synchronization        	                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: ͬ��ʹ��                                                                                          	                                   
 *-----------------------------------------------------------------------------*/
void GPIO_Enable_Syn(UINT8 Port_Num, FunctionalState opt) 
{
	  UINT32 temp;
	  UINT8 offSet = (Port_Num / 8) * 8 + 7;
	
		SGCC_GPIO_TypeDef * GPIO_P;
		UINT8 regnum=0;
	
		regnum = Port_Num / 32;
	
		GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO_BASE + regnum*0x10000); //
	
		if(regnum)
			Port_Num = Port_Num%32;
	

	  temp = GPIO_P->DBSYR;
	  
	  if(opt)
	      temp |= ((UINT32)1 << offSet);
	  else
	  	  temp &= ~((UINT32)1 << offSet);
	  	  
	  GPIO_P->DBSYR = temp;
	  
}
