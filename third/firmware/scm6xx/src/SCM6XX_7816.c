/***************************************************************************
������7816 routine
ƽ̨��
ʱ�䣺2016-3-10
���ߣ�������                                                 
****************************************************************************/
#include "SCM6XX_7816.h"


/*-----------------------------------------------------------------------------
  Function:		U7816_RestoreReg                                                                      
  input:    None                         
  Returns:  None                                                                                        
  Description:  restore 7816 related registers                                                                                          
               	                                  
 *-----------------------------------------------------------------------------*/
void U7816_Reset_Reg(void)
{
      SGCC_U7816_P->FIFO = 0x1b;
	   __asm("nop");
	  SGCC_U7816_P->FIFO = 0x18;  //reset FIFO
	
	  SGCC_U7816_P->CHNL = 5;
	/*
	   //7816DATA0--GPIO11
	   SGCC_PINCTRL_P->PMUR_MFS0 &= ~(3 << 16);
	 
	   //7816CLK0--GPIO12
	   SGCC_PINCTRL_P->PMUR_MFS0 &= ~(3 << 18);
	 
	   //7816RST0--GPIO13
	   SGCC_PINCTRL_P->PMUR_MFS0 &= ~(3 << 20);
	 
	   //7816DATA1--GPIO14
	   SGCC_PINCTRL_P->PMUR_MFS0 &= ~(3 << 22);	 
	   //7816CLK1--GPIO15
	   SGCC_PINCTRL_P->PMUR_MFS0 &= ~(3 << 24);
	   //7816RST1--GPIO16
	   SGCC_PINCTRL_P->PMUR_MFS0 &= ~(3 << 26);
       */
    SGCC_U7816_P->CHNL = 0;
}

/*-----------------------------------------------------------------------------
  Function:		U7816_ConfigBaudRate                                                                                                                                                                           
  input:     	baudRate  -- baud rate configuration                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: configure U7816_BDR and U7816_DFRM                                                                                                   	                                   
 *-----------------------------------------------------------------------------*/
void U7816_Configure_BaudRate(UINT16 BaudRate) 
{
  SGCC_U7816_P->BDR = BaudRate;
}

/*-----------------------------------------------------------------------------
  Function:		U7816_ConfigDataFrame                                                                                                                                                                           
  input:     	tmod--U7816_DFRM_TMOD_T1(T=1)/U7816_DFRM_TMOD_T0(T=0)
              diconv--U7816_DFRM_DICONV_INV��������룩/U7816_DFRM_DICONV_NOR��������룩
              par--U7816_DFRM_PAR_ODD/U7816_DFRM_PAR_EVEN
              txegt--extended delay between two characters when transmit
                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: configure U7816_BDR and U7816_DFRM                                                                                                   	                                   
 *-----------------------------------------------------------------------------*/
void U7816_Configure_Data(UINT8 Tmod, UINT8 Diconv, UINT8 Par, UINT8 Txegt) 
{
	UINT32 temp = 0;
	
	temp |= Tmod  | Diconv | Par | Txegt;
  SGCC_U7816_P->DFRM = temp;
}


/*-----------------------------------------------------------------------------
  Function:		U7816_Init                                                                                                                                                                            
  input:     	baudRate  -- baud rate configuration                  	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: configure U7816_BDR and U7816_DFRM                                                                                                   	                                   
 *-----------------------------------------------------------------------------*/
void U7816_Init(UINT16 BaudRate) 
{
	
  SGCC_U7816_P->BDR = BaudRate;
  //����ʱguard timeΪ2etu��error signal���Ϊ1.5etu��żУ�飬������룬��LSB��T=0��
  //�Զ��ط������շ�����ʱ����
	SGCC_U7816_P->DFRM = 0x140; 
}


/*-----------------------------------------------------------------------------
  Function:		U7816_PrepareRx                                                                                                                                                                            
  input:     	rxLvl -- RXFIFO trigger level: U7816_FIFOCTRL_RTRIG_1 ~ U7816_FIFOCTRL_RTRIG_8
              errWid -- error width, can be U7816_DFRM_ERRWID_1etu, U7816_DFRM_ERRWID_1nhalfetu, U7816_DFRM_ERRWID_2etu
              ch  -- channel 0 or 1
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: prepare to receive                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void U7816_Prepare_Rx(UINT8 RxLvl, UINT8 ErrWid, UINT8 Ch) 
{
  //���ý�����ֵ
	SGCC_U7816_P->FIFO = RxLvl  ;
	SGCC_U7816_P->DFRM |= ErrWid ;

	if( Ch == 0x00 )
	{
		 SGCC_U7816_P->CHNL = U7816_CHNL_EN | U7816_CHNL_CSEL_0 | U7816_CHNL0_SCLK_EN;	//ѡ��ͨ��0��ʹ��7816ģ�顢��ʱ��
	}
	else
	{
		 SGCC_U7816_P->CHNL = U7816_CHNL_EN | U7816_CHNL_CSEL_1 |  U7816_CHNL1_SCLK_EN;	//ѡ��ͨ��1��ʹ��7816ģ�鼰ʱ��
	}
}

/*-----------------------------------------------------------------------------
  Function:		U7816_GetChars                                                                                                                                                                            
  input:      len	                      	  
  output:     data-- received data                                                                                      
  Returns:    None                                                                                    
  Description: get received data                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void U7816_Get_Chars(UINT8 *data, UINT16 len)
{
	UINT8 rxTrig;
  UINT8 i;
  UINT8 *rxData = data;
  
  rxTrig = ((SGCC_U7816_P->FIFO & U7816_FIFOCTRL_RTRIG) >> U7816_FIFOCTRL_RTRIGBF) + 1;
  while(len >= rxTrig)  //��Ҫ���յ����ݳ���RXFIFO��ֵ
  {
  	  while(!(SGCC_U7816_P->SR & U7816_SR_RXMTRG)); //���յ������ݴﵽtriggerֵ
  	  
  	  for(i = 0; i < rxTrig; i++)
  	  {
  	  	 *rxData++ = SGCC_U7816_P->DATA;
  	  	 len--;
  	  }
  }
  
	
	for(i = 0; i < len; i++)
	{
		  while((SGCC_U7816_P->SR & U7816_SR_RXEMPT)); //����FIFO�ǿ�
		  *rxData++ = SGCC_U7816_P->DATA;
	}
	
	
	
}

/*-----------------------------------------------------------------------------
  Function:		U7816_GetChar                                                                                                                                                                            
  input:     	None                      	  
  output:     data-- received data                                                                                      
  Returns:    1 -- success; 2 -- error; 3 -- timeout                                                                                      
  Description: get 1 byte data  when RXFIFO trigger level is 0, i.e. interrupt when received 1  byte                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
UINT8 U7816_Get_Char(UINT8 *data)
{

    TC_16BitTimerCFG tcCfg;
	
	//configure TM01
	//��ʹ��TC
	TC_Enable_Int(SGCC_TIMERA0_P, DISABLE) ;
    
  //����ж�
	TC_Clear_Int_Flag(SGCC_TIMERA0_P, TC_INTR_MR) ;
       
    //confiugure timer
    tcCfg.matchCtl = 2;
    tcCfg.outMode = 0;
    tcCfg.outPulseW = 0;
    tcCfg.prescale = 31999;
    tcCfg.matchVal = 3000 * (PCLK/(tcCfg.prescale + 1))  -1;
    
    TC_Timer_Set_16Bit(SGCC_TIMERA0_P, ENABLE, tcCfg, DISABLE) ;
	
	//�ж��Ƿ��ڽ���״ֱ̬��������������ж�
	while(!TC_Get_Int_Status(SGCC_TIMERA0_P, TC_INTR_MR))
	{
	    if(SGCC_U7816_P->SR & U7816_SR_RXMTRG) //���յ��������Ѵﵽtriggerֵ
	    {
				  *data = SGCC_U7816_P->DATA;
			    //disable timer
			    TC_Enable_Int(SGCC_TIMERA0_P, DISABLE) ;
			    return 1;
	    }
		if( SGCC_U7816_P->SR & U7816_SR_RXERR ) //���մ���
		{
			    //disable timer
			    TC_Enable_Int(SGCC_TIMERA0_P, DISABLE) ;
				//clear flag
				//SGCC_U7816_P->SR &= ~U7816_SR_RXERR;
				return 2; //error
		}
  }
	
  //disable timer
	TC_Enable_Int(SGCC_TIMERA0_P, DISABLE) ;
	//����ж�
	TC_Clear_Int_Flag(SGCC_TIMERA0_P, TC_INTR_MR) ;	
	return 3;
	
	/*
	while( !(SGCC_U7816_P->SR & U7816_SR_RXMTRG) );//���յ��������Ѵﵽtriggerֵ
	*data = SGCC_U7816_P->DATA;
	return 1;*/
}

/*-----------------------------------------------------------------------------
  Function:		U7816_PrepareTx                                                                                                                                                                            
  input:     	rpt -- repeat times
              ch  -- channel 0 or 1
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: prepare to transmit                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void U7816_Prepare_Tx(UINT8 Repeat, UINT8 Ch) 
{
    ///�����ط�����
	SGCC_U7816_P->DFRM &= ~(0x7 << U7816_DFRM_RTXBF); 
    SGCC_U7816_P->DFRM |= (Repeat << U7816_DFRM_RTXBF);

    if( Ch == 0x00 )
	{
		SGCC_U7816_P->CHNL = U7816_CHNL_EN | U7816_CHNL_CSEL_0 |  U7816_CHNL0_SCLK_EN;	//ѡ��ͨ��0��ʹ��7816ģ�顢���ͼ�ʱ��
	}
	else
	{
		SGCC_U7816_P->CHNL = U7816_CHNL_EN | U7816_CHNL_CSEL_1  | U7816_CHNL1_SCLK_EN;	//ѡ��ͨ��1��ʹ��7816ģ�顢���ͼ�ʱ��
	}
}

/*-----------------------------------------------------------------------------
  Function:		U7816_SendByte                                                                                                                                                                            
  input:    data -- data to send     	  
  output:   None                                                                                    
  Returns:    0 -- success; 1 -- error; 2 -- timeout                                                                                     
  Description: send 1 byte data                                                                                                   	                                   
 *-----------------------------------------------------------------------------*/
UINT8 U7816_Send_Byte(UINT8 Data)
{
	UINT16 i;
  
	
	//����FIFO������������д��һ�ֽ�����
	if(!(SGCC_U7816_P->SR & U7816_SR_TXFULL))
	{
	  SGCC_U7816_P->DATA = Data;
	}

  for( i = 0x00; i < 9000*6; i++ ) //36ms
	{
		if(!(SGCC_U7816_P->SR & U7816_SR_TXBUSY))	//TX���
		{
			return 1; //�ɹ�
		}

		if( (SGCC_U7816_P->SR & U7816_SR_TXERR))	//TX error
		{
  		return 0; //error
		}
	}
	return 2;
}



/*-----------------------------------------------------------------------------
  Function:		U7816_EnableIntr                                                                                                                                                                                
  input:    UARTx: select the UART 
            IntMask:      U7816_RXFULL_INTR/U7816_RXMTRG_INTR/U7816_TXERR_INTR/U7816_TXEMPT_INTR                  
  Returns:    None                                                                                      
  Description:                                                                                            
               	                                  
 *-----------------------------------------------------------------------------*/
void U7816_Enable_Int(UINT32 U7816_Int)
{
	SGCC_U7816_P->INTEN |= U7816_Int;
}

/*-----------------------------------------------------------------------------
  Function:		U7816_ClearIntr                                                                                                                                                                                
  input:    intType:      U7816_RXFULL_INTR/U7816_RXMTRG_INTR/U7816_TXERR_INTR/U7816_TXEMPT_INTR                 
  Returns:    None                                                                                      
  Description:                                                                                            
               	                                  
 *-----------------------------------------------------------------------------*/
void U7816_ClearIntr(UINT32 U7816_Int)
{
	SGCC_U7816_P->INTFLAG &= ~U7816_Int;
}



