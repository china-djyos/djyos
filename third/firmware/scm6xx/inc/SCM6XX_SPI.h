#ifndef _SCM6XX_SPI_H_
#define _SCM6XX_SPI_H_

#include "SCM6XX_ALL.h"

#define SSN_HIGH   1
#define SSN_LOW  0

typedef struct 
{
	 UINT8 sckDiv;
	 UINT8 txEn;
	 UINT8 rxEn;
	 UINT8 mode;
	 UINT8 txFifoLvl;
	 UINT8 rxFifoLvl;
	 UINT8 DataNumEn;
	 UINT8 DataNum;
	 UINT8 waitNum;
}SPI_MASTER;

typedef struct 
{
	 UINT8 txEn;
	 UINT8 rxEn;
	 UINT8 mode;
	 UINT8 txFifoLvl;
	 UINT8 rxFifoLvl;
}SPI_SLAVE;


/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
void SPI_Reset_Reg(SGCC_SPI_TypeDef * SPIx);
void SPI_Configure_Mode(SGCC_SPI_TypeDef * SPIx, UINT32 SPI_Mode);
void SPI_Init_Master(SGCC_SPI_TypeDef * SPIx, SPI_MASTER * Init_Master);
void SPI_Init_Slave(SGCC_SPI_TypeDef * SPIx, SPI_SLAVE * Init_Slave );
void SPI_Start_Transfer_As_Master(SGCC_SPI_TypeDef * SPIx);
void SPI_Start_Transfer_As_Slave(SGCC_SPI_TypeDef * SPIx);
void SPI_Stop_Transfer_As_Master(SGCC_SPI_TypeDef * SPIx);
void SPI_Drive_SSN(SGCC_SPI_TypeDef * SPIx, FunctionalState opt);
void SPI_Enable(SGCC_SPI_TypeDef * SPIx, FunctionalState opt);
void SPI_Enable_Int(SGCC_SPI_TypeDef * SPIx, UINT8 SPI_Int_Src, FunctionalState opt);
void SPI_Clear_Int_Flag(SGCC_SPI_TypeDef * SPIx, UINT8 SPI_Int_Src);
void SPI_Clear_Tx_FIFO(SGCC_SPI_TypeDef * SPIx);
void SPI_Clear_Rx_FIFO(SGCC_SPI_TypeDef * SPIx);
void SPI_Write_Byte(SGCC_SPI_TypeDef * SPIx, UINT8 dat);
void SPI_Write_Bytes(SGCC_SPI_TypeDef * SPIx, UINT8 *dat, UINT8 len);
UINT8 SPI_Read_Byte (SGCC_SPI_TypeDef * SPIx);
UINT8 SPI_Read_Bytes (SGCC_SPI_TypeDef * SPIx, UINT8 *dat);
#endif 



