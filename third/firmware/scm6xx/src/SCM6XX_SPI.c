/***************************************************************************
描述：SPI routine
平台：SCM6XX
时间：2016-4-8
作者：                                                
****************************************************************************/
#include "SCM6XX_SPI.h"

extern SGCC_UART_TypeDef* port;

/********************************************************
 Function:	SPI_ResetReg                                                                                                                                                                            
 input:     SPIx  
 output:     None                                        
 Returns:    None                                                                                      
 Description: reset spi register         
********************************************************/
void SPI_Reset_Reg(SGCC_SPI_TypeDef * SPIx)
{
    SPIx->SPCR0 = 0x0;
    SPIx->SPCR1 = 0x0;
    SPIx->SPCR2 = 0x80;
    SPIx->SPCR3 = 0xc;
    SPIx->SPCR3 = 0x21;
    SPIx->SPWAR = 0x0;
    SPIx->SPNUMR = 0x0;
}
/********************************************************
 Function:	SPI_ConfigMode                                                                                                                                                                            
 input:     SPIx
            mode: SPI_CR1_SCK_MOD0/SPI_CR1_SCK_MOD1/SPI_CR1_SCK_MOD2/SPI_CR1_SCK_MOD3	  
 output:     None                                        
 Returns:    None                                                                                      
 Description: 配置POL         
********************************************************/
void SPI_Configure_Mode(SGCC_SPI_TypeDef * SPIx, UINT32 SPI_Mode)
{
    UINT32 temp = 0;
	temp = SPIx->SPCR1;
    
    temp &= ~SPI_CR1_SCK_MOD;
    temp |= SPI_Mode;
    
    SPIx->SPCR1 = temp;
}
/********************************************************
 Function:	SPI_InitMaster                                                                                                                                                                            
 input:     SPIx  
 output:     None                                        
 Returns:    None                                                                                      
 Description: spi init as master         
********************************************************/
void SPI_Init_Master(SGCC_SPI_TypeDef * SPIx, SPI_MASTER * Init_Master )
{	
	SPIx->SPCR1 = Init_Master->rxEn | Init_Master->txEn | Init_Master->mode | SPI_CR1_MSTR_MSTR;

    SPIx->SPCR2 = Init_Master->txFifoLvl | Init_Master->rxFifoLvl;
	SPIx->SPI_SCKDIV = Init_Master->sckDiv;
    
    if(Init_Master->DataNumEn == SPI_CR3_DNUM_EN)
    {
        SPIx->SPNUMR = Init_Master->DataNum;
        SPIx->SPCR3 |= SPI_CR3_DNUM_EN;
    }
    else
        SPIx->SPCR3 &= ~SPI_CR3_DNUM_EN;
    
    SPIx->SPWAR = Init_Master->waitNum;
}

/********************************************************
 Function:	SPI_InitSlave                                                                                                                                                                            
 input:     SPIx  
 output:     None                                        
 Returns:    None                                                                                      
 Description: spi init as slave         
********************************************************/
void SPI_Init_Slave(SGCC_SPI_TypeDef * SPIx, SPI_SLAVE * Init_Slave )
{	
	SPIx->SPCR1 = Init_Slave->rxEn | Init_Slave->txEn | Init_Slave->mode | SPI_CR1_MSTR_SLAVE;

    SPIx->SPCR2 = Init_Slave->txFifoLvl | Init_Slave->rxFifoLvl;
}
/********************************************************
 Function:	SPI_StartTransferAsMaster                                                                                                                                                                            
 input:     SPIx	  
 output:     None                                        
 Returns:    None                                                                                      
 Description: select slave, enable SPI and start transfer  
********************************************************/
void SPI_Start_Transfer_As_Master(SGCC_SPI_TypeDef * SPIx)
{
    UINT32 temp = 0;
    temp = SPIx->SPCR3; 
    temp &= ~SPI_CR3_SSN_EN;
    temp |= (SPI_CR3_SPE_EN | SPI_CR3_STR_EN );
    
    SPIx->SPCR3 = temp;

}

/********************************************************
 Function:	SPI_StartTransferAsSlave                                                                                                                                                                            
 input:     SPIx	  
 output:     None                                        
 Returns:    None                                                                                      
 Description: select slave, enable SPI and start transfer  
********************************************************/
void SPI_Start_Transfer_As_Slave(SGCC_SPI_TypeDef * SPIx)
{
    UINT32 temp = 0;
    temp = SPIx->SPCR3; 
    temp &= ~SPI_CR3_SSN_EN;
    temp |= (SPI_CR3_SPE_EN | SPI_CR3_STR_EN | SPI_CR3_DEBN_EN);
    
    SPIx->SPCR3 = temp;

}
/********************************************************
 Function:	SPI_StopTransferAsMaster                                                                                                                                                                            
 input:     SPIx	  
 output:     None                                        
 Returns:    None                                                                                      
 Description: disable tx and rx, set ssn high , disable SPI   
********************************************************/
void SPI_Stop_Transfer_As_Master(SGCC_SPI_TypeDef * SPIx)
{
    SPIx->SPCR1 &= ~(SPI_CR1_R_EN | SPI_CR1_T_EN);
    
    SPIx->SPCR3 &= ~(SPI_CR3_SPE_EN | SPI_CR3_STR_EN);
    SPIx->SPCR3 |= SPI_CR3_SSN_EN;

}

/********************************************************
 Function:	SPI_DriveSSN                                                                                                                                                                            
 input:     SPIx
            opt--SSN_HIGH/SSN_LOW
 output:     None                                        
 Returns:    None                                                                                      
 Description: disable tx and rx, set ssn high , disable SPI   
********************************************************/
void SPI_Drive_SSN(SGCC_SPI_TypeDef * SPIx, FunctionalState opt)
{
    
    if(opt == SSN_HIGH)
    {
        SPIx->SPCR3 |= SPI_CR3_SSN_EN;
    }
    else
    {
        SPIx->SPCR3 &= ~SPI_CR3_SSN_EN;
    }

}

/********************************************************
 Function:	SPI_Enable                                                                                                                                                                            
 input:      SPIx -which SPI to enable or disable
             opt-ENABLE/DISABLE
 output:     None                                        
 Returns:    None                                                                                      
 Description:   
********************************************************/
void SPI_Enable(SGCC_SPI_TypeDef * SPIx, FunctionalState opt)
{
    if(opt == ENABLE)
	    SPIx->SPCR3 |= SPI_CR3_SPE_EN;
    else
        SPIx->SPCR3 &= ~SPI_CR3_SPE_EN;
}

/********************************************************
 Function:	SPI_EnableIntr                                                                                                                                                                            
 input:      SPIx -which SPI to enable or disable
             intrMask - which interrupt to enable , 
             SPI_INTR_TFE/SPI_INTR_RFF/SPI_INTR_SSNERR/SPI_INTR_TNF/SPI_INTR_RNE/SPI_INTR_BYTEDONE/SPI_INTR_TRANDONE
              SPI_CR0_RFF_EN/SPI_CR0_TFE_EN/SPI_CR0_SSN_EN/SPI_CR0_DUNM_EN/SPI_CR0_TNF_EN/SPI_CR0_RNE_EN/SPI_CR0_BYTEDONE_EN
 output:     None                                        
 Returns:    None                                                                                      
 Description:   
********************************************************/
void SPI_Enable_Int(SGCC_SPI_TypeDef * SPIx, UINT8 SPI_Int_Src, FunctionalState opt)
{
    if(opt == ENABLE)
        SPIx->SPCR0 |= SPI_Int_Src;
    else
        SPIx->SPCR0 &= ~SPI_Int_Src;
}

/********************************************************
 Function:	SPI_ClearIntr                                                                                                                                                                            
 input:      SPIx -which SPI to enable or disable
             intrType - which interrupt to clear 
 output:     None                                        
 Returns:    None                                                                                      
 Description:   
********************************************************/
void SPI_Clear_Int_Flag(SGCC_SPI_TypeDef * SPIx, UINT8 SPI_Int_Src)
{
    SPIx->SPINTSR = SPI_Int_Src;
}


/********************************************************
 Function:	SPI_ClearTxFIFO                                                                                                                                                                            
 input:      SPIx -which SPI to enable or disable
 output:     None                                        
 Returns:    None                                                                                      
 Description:   
********************************************************/
void SPI_Clear_Tx_FIFO(SGCC_SPI_TypeDef * SPIx)
{
    SPIx->SPCR3 |= 0x4;
}

/********************************************************
 Function:	SPI_ClearRxFIFO                                                                                                                                                                            
 input:      SPIx -which SPI to enable or disable
 output:     None                                        
 Returns:    None                                                                                      
 Description:   
********************************************************/
void SPI_Clear_Rx_FIFO(SGCC_SPI_TypeDef * SPIx)
{
    SPIx->SPCR3 |= 0x8;
}

/********************************************************
 Function:	SPI_WriteByte                                                                                                                                                                            
 input:     SPIx -- 
             dat -- data to send 
 output:     None                                        
 Returns:    None                                                                                      
 Description: spi write one data byte to txfifo       
********************************************************/
void SPI_Write_Byte(SGCC_SPI_TypeDef * SPIx, UINT8 dat)
{
    SPIx->SPDR = dat;
}    
/********************************************************
 Function:	SPI_WriteBytes                                                                                                                                                                            
 input:     SPIx -- 
             dat -- data to send 
 output:     None                                        
 Returns:    None                                                                                      
 Description: spi write data bytes to txfifo       
********************************************************/
void SPI_Write_Bytes(SGCC_SPI_TypeDef * SPIx, UINT8 *dat, UINT8 len)
{
    UINT8 i ;
    
    for(i = 0; i < len; i++)
    {
        while(((SPIx->SPSR & 0xF0) >> 4) == 8);  //wait until data in txfifo less than 8
        SPIx->SPDR = dat[i];
    }
}

/********************************************************
 Function:	SPI_RdByte                                                                                                                                                                            
 input:     SPIx --   
 output:     None                                        
 Returns:    读回的数据                                                                                      
 Description: SPI接收1字节数据        
********************************************************/
UINT8 SPI_Read_Byte (SGCC_SPI_TypeDef * SPIx)
{
    while(!(SPIx->SPINTSR & SPI_INTR_RNE));
   // while(!(SPIx->SPSR & 0xf));
	    return(SPIx->SPDR);
       
}

/********************************************************
 Function:	SPI_RxBytes                                                                                                                                                                            
 input:     SPIx --   
 output:     None                                        
 Returns:    读回的数据                                                                                      
 Description: SPI接收多字节数据        
********************************************************/
UINT8 SPI_Read_Bytes (SGCC_SPI_TypeDef * SPIx, UINT8 *dat)
{
	UINT8 len = 0;
   // UART_Printf(port, "%x ", SPIx->SPSR);
    if((SPIx->SPSR & 0xF)  != 0)//wait until data is available in rxfifo 
    {
        dat[len] = SPIx->SPDR;
        len++;
    }
	return len;
}



