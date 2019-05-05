/***************************************************************************
描述：I2C routine
平台：SCM6XX
时间：2013-5-23
作者：                                                 
****************************************************************************/
#include "SCM6XX_I2C.h"


/*-----------------------------------------------------------------------------
  Function:		I2C_Delay                                                                                                                                                                            
  input:     	None        	  
  output:     None                                        
  Returns:    None                                                                                      
  Description: Delay                                                                                                       	                                   
 *-----------------------------------------------------------------------------*/
void I2C_Delay( void ) 
{
	UINT16 Temp16;

	for( Temp16=0; Temp16<4000; Temp16++ );
}

/*-----------------------------------------------------------------------------
  Function:		I2C_Init                                                                                                                                                                            
  input:    i2cx -- SGCC_I2C0_P/SGCC_I2C1_P
            i2cInit-- i2c parameters  	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: configure I2C_CLKR, I2C_CFGR                  | I2C_CFGR_STAD_EN                                                                                   	                                   
 *-----------------------------------------------------------------------------*/
void IIC_Init( SGCC_I2C_TypeDef* IICx,  I2C_InitTypeDef IIC_Init_Data) 
{
    UINT32 temp;
    
    temp = SYS_FREQ/(4 * IIC_Init_Data.scl);
    IICx->CLKR = temp;
    
    temp = IICx->CFGR;
    temp &= ~I2C_CFGR_DBCN;
    temp |= IIC_Init_Data.prescale;
    IICx->CFGR = temp;
    
    if(IIC_Init_Data.mode == I2C_SLAVE)
    {
        temp = IICx->CFGR;
        temp &= ~I2C_CFGR_TO;
        temp |= IIC_Init_Data.timeoutEn;
        
        temp &= ~I2C_CFGR_STR;
        temp |= IIC_Init_Data.stretchEn;
        IICx->CFGR = temp;
        
        IICx->ADR = IIC_Init_Data.addrMode | IIC_Init_Data.addrMask | IIC_Init_Data.gCallEn | IIC_Init_Data.slaveAddr;
    }
}


/*-----------------------------------------------------------------------------
  Function:		I2C_EnableIntr                                                                                                                                                                                
  input:    i2cx -- SGCC_I2C0_P/SGCC_I2C1_P  
            intType --  I2C_CFGR_DEI_EN/I2C_CFGR_DEI_DIS
                        I2C_CFGR_AKF_EN/I2C_CFGR_AKF_DIS
                        I2C_CFGR_STAD_EN/I2C_CFGR_STAD_DIS
                        I2C_CFGR_STOD_EN/I2C_CFGR_STOD_DIS
                        I2C_CFGR_BYD_EN/I2C_CFGR_BYD_DIS
                        I2C_CFGR_RXOV_EN/I2C_CFGR_RXOV_DIS
                        I2C_CFGR_TXOV_EN/I2C_CFGR_TXOV_DIS
                        I2C_CFGR_ARB_EN/I2C_CFGR_ARB_DIS
                        I2C_CFGR_BSE_EN/I2C_CFGR_BSE_DIS
                        I2C_CFGR_TIO_EN/I2C_CFGR_TIO_DIS
            opt -- ENABLE/DISABLE
  Returns:    None                                                                                      
  Description:                                                                                            
               	                                  
 *-----------------------------------------------------------------------------*/
void I2C_Enable_Int(SGCC_I2C_TypeDef* IICx, UINT32 IIC_Int_Type,FunctionalState opt)
{
    if(opt == ENABLE)
    {
        IICx->CFGR |= (IIC_Int_Type | I2C_CFGR_INT_EN);
    }
    else
    {
        IICx->CFGR &= ~I2C_CFGR_INT_EN;
    }
}


/*-----------------------------------------------------------------------------
  Function:		I2C_Enable                                                                                                                                                                                
  input:    i2cx -- SGCC_I2C0_P/SGCC_I2C1_P  
            opt -- ENABLE/DISABLE
  Returns:    None                                                                                      
  Description:                                                                                            
               	                                  
 *-----------------------------------------------------------------------------*/
void I2C_Enable(SGCC_I2C_TypeDef* IICx,FunctionalState opt)
{
    if(opt == ENABLE)
    {
        IICx->CFGR |= I2C_CFGR_IICE_EN;
    }
    else
    {
        IICx->CFGR &= ~I2C_CFGR_IICE_EN;
    }
}

/*-----------------------------------------------------------------------------
  Function:		I2C_WrCmd                                                                                                                                                                           
  input:     	i2cx-- SGCC_I2C0_P/SGCC_I2C1_P  
                cmd -- I2C_CMDR_MODE_MASTER/I2C_CMDR_MODE_SLAVE
                       I2C_CMDR_RSTA_EN/I2C_CMDR_RSTA_DIS
                       I2C_CMDR_NACK/I2C_CMDR_ACK
                       I2C_CMDR_DMA_EN/I2C_CMDR_DMA_DIS
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: write control register                                                                                                    	                                   
 *-----------------------------------------------------------------------------*/
void I2C_Write_Cmd( SGCC_I2C_TypeDef* IICx, UINT32 IIC_Cmd) 
{
	
	IICx->CMDR = IIC_Cmd;
	
	
}


/*-----------------------------------------------------------------------------
  Function:		I2C_Getsts                                                                                                                                                                            
  input:     	cond -- I2C_SR_ADMB/I2C_SR_TIPB/I2C_SR_TRDB/I2C_SR_BUFB/I2C_SR_GCAMB/I2C_SR_DATAERR/I2C_SR_ACKFAIL/I2C_SR_START/I2C_SR_STOP/I2C_SR_BYTEDONE
                       /I2C_SR_RXOV/I2C_SR_TXOV/I2C_SR_ARBLOST/I2C_SR_BUFERR/I2C_SR_TIMEOUT
  output:     None                                        
  Returns:    0--ok  1--failure                                                                                      
  Description: wait some condition                                                 I2C_SR_STAI_EN                                                       	                                   
 *-----------------------------------------------------------------------------*/
BOOL I2C_Get_Flag(SGCC_I2C_TypeDef* IICx, UINT32 IIC_Flag_Src)     
{

	if((IICx->SR & IIC_Flag_Src))		return SUCCESS; //ok

	else return ERROR; 
	
}

/*-----------------------------------------------------------------------------
  Function:		I2C_WaitCond                                                                                                                                                                            
  input:     	cond -- I2C_SR_ADM/I2C_SR_TIP/I2C_SR_TRD/I2C_SR_BUF/I2C_SR_GCAM/I2C_SR_DATAERR/I2C_SR_ACKFAIL/I2C_SR_START/I2C_SR_STOP/I2C_SR_BYTEDONE
                       /I2C_SR_RXOV/I2C_SR_TXOV/I2C_SR_ARBLOST/I2C_SR_BUFERR/I2C_SR_TIMEOUT
  output:     None                                        
  Returns:    1--ok  0--failure                                                                                      
  Description: wait some condition                                                 I2C_SR_STAI_EN                                                       	                                   
 *-----------------------------------------------------------------------------*/
BOOL I2C_Wait_Flag(SGCC_I2C_TypeDef* IICx, UINT32 IIC_Flag_Src)     
{
	UINT32 status;


	
			
	//wait start interrupt
	//for( i=0; i<I2C_TIMEOUT; i++ )
    while(1)
	{

		status = IICx->SR;  
	//	i2cx->SR = temp;  //清中断
		
		if((status & IIC_Flag_Src))		return 1; //ok
	}

//	return 0; //failure.
	
}

/*-----------------------------------------------------------------------------
  Function:		I2C_ClrSts                                                                                                                                                                            
  input:     	sts -- I2C_SR_DATAERR/I2C_SR_ACKFAIL/I2C_SR_START/I2C_SR_STOP/I2C_SR_BYTEDONE/I2C_SR_RXOV/I2C_SR_TXOV/I2C_SR_ARBLOST/I2C_SR_BUFERR/I2C_SR_TIMEOUT
  output:     None                                        
  Returns:    0--ok  1--failure                                                                                      
  Description: clear status        
 *-----------------------------------------------------------------------------*/
void I2C_Clear_Flag(SGCC_I2C_TypeDef* IICx, UINT32 IIC_Flag_Src)     
{
	IICx->SR = IIC_Flag_Src;  
}
/*---------------------------------------------------------------------------------------
  Function:		I2C_SendByteOnly                                                                                                                                                                            
  input:     	data--data to send  
              ctl -- control register      	
              mode -- I2C_CMDR_MODE_MASTER/I2C_CMDR_MODE_SLAVE
  output:     None                                        
  Returns:    0--ok  1--failure                                                                                        
  Description: send data                                                                                                        	                                   
 *-----------------------------------------------------------------------------*/
void I2C_Send_Byte_Only(SGCC_I2C_TypeDef* IICx, UINT8 IIC_Data, UINT8 mode) 
{
	IICx->BUFR = IIC_Data;
    IICx->CMDR = mode;
}

/*---------------------------------------------------------------------------------------
  Function:		I2C_SendByteTillDone                                                                                                                                                                            
  input:     	data--data to send  
              ctl -- control register      	
              mode -- I2C_CMDR_MODE_MASTER/I2C_CMDR_MODE_SLAVE
  output:     None                                        
  Returns:    0--ok  1--failure                                                                                        
  Description: send data                                                                                                        	                                   
 *-----------------------------------------------------------------------------*/
BOOL I2C_Send_Byte_Till_Done(SGCC_I2C_TypeDef* IICx, UINT8 IIC_Data, UINT8 mode) 
{
    BOOL result;
	
	IICx->BUFR = IIC_Data;
    IICx->CMDR = mode;
  
    
	result = I2C_Wait_Flag(IICx, I2C_SR_BYTEDONE ); //等待数据发送中断  
	if(result)
	{
	    IICx->SR = ( I2C_SR_BYTEDONE);
	}
    IICx->SR = I2C_SR_START;
    
    return result;
}


/*---------------------------------------------------------------------------------------
  Function:		I2C_SendlastByte                                                                                                                                                                            
  input:     	data--data to send  
              ctl -- control register     
              mode -- I2C_CMDR_MODE_MASTER/I2C_CMDR_MODE_SLAVE
  output:     None                                        
  Returns:    0--ok  1--failure                                                                                        
  Description: send data                                                                                                        	                                   
 *-----------------------------------------------------------------------------*/
BOOL I2C_Send_Last_Byte(SGCC_I2C_TypeDef* IICx, UINT8 IIC_Data, UINT8 mode) 
{
  BOOL result;
	
	IICx->BUFR = IIC_Data;
    if(mode == I2C_CMDR_MODE_MASTER)
	    IICx->CMDR |= I2C_CMDR_MODE_MASTER;
	
	result = I2C_Wait_Flag(IICx, I2C_SR_BYTEDONE); //等待数据发送中断 
	if(result)
	{
	    IICx->SR |= ( I2C_SR_BYTEDONE);
        IICx->CMDR = I2C_CMDR_MODE_SLAVE;		
	}
	while (!(IICx->SR & (1 << 9)));
    IICx->SR |= (1 << I2C_SR_STOIB);
  return result;
}

/*-----------------------------------------------------------------------------
  Function:		I2C_WriteBuf                                                                                                                                                                            
input:     i2cx:SGCC_I2C0_P/SGCC_I2C1_P	 
           data: data to write 
  output:    None                                   
  Returns:   received data         	                                     
  Description: receive data                                                                                                        	                                   
 *-----------------------------------------------------------------------------*/
void I2C_Write_Buf(SGCC_I2C_TypeDef* i2cx, UINT8 data) 
{
   i2cx->BUFR = data;		
}

/*-----------------------------------------------------------------------------
  Function:		I2C_ReadBuf                                                                                                                                                                            
input:     i2cx:SGCC_I2C0_P/SGCC_I2C1_P	   
  output:    None                                   
  Returns:   received data         	                                     
  Description: receive data                                                                                                        	                                   
 *-----------------------------------------------------------------------------*/
UINT8 I2C_Read_Buf(SGCC_I2C_TypeDef* IICx) 
{
   UINT8 temp;
	
	temp = IICx->BUFR;		
	return temp;
}

/*-----------------------------------------------------------------------------
  Function:		I2C_ReceiveByte                                                                                                                                                                            
  input:     	None   
  output:     data--data received  
              ctl -- control register                                        
  Returns:    1--ok  0--failure         	                                     
  Description: receive data                                                                                                        	                                   
 *-----------------------------------------------------------------------------*/
UINT8 I2C_Receive_Byte(SGCC_I2C_TypeDef* IICx) 
{
	unsigned char data;
	
  while(!((IICx->SR & (1<<10)) && (IICx->SR & (1<<4))));	
	IICx->SR = ( I2C_SR_BYTEDONE);
  data = IICx->BUFR;		
  IICx->CMDR &= ~(1 << 2);	
	return data;	
}


/*-----------------------------------------------------------------------------
  Function:		I2C_ReceivelastByte                                                                                                                                                                            
  input:     	None   
  output:     data--data received  
              ctl -- control register                                        
  Returns:    0--ok  1--failure         	                                     
  Description: receive last data                                                                                                        	                                   
 *-----------------------------------------------------------------------------*/
BOOL I2C_Receive_Last_Byte(SGCC_I2C_TypeDef* IICx, UINT8 *data ) 
{
   BOOL result;

   result = I2C_Wait_Flag(IICx, I2C_SR_BYTEDONE); //等待数据接收中断 
   
	if(result)
	{
		IICx->SR |= ( I2C_SR_BYTEDONE);
		*data = IICx->BUFR;
		IICx->CMDR = I2C_CMDR_NACK;  //NACK+STOP
		while(!(IICx->SR & I2C_SR_STOP));
        IICx->SR |= I2C_SR_STOP;
		return 1;
	}
	else
	{
		return 0; //failure.
	}
}



