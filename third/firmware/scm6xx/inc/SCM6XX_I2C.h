#ifndef _SCM6XX_I2C_H_
#define _SCM6XX_I2C_H_

#include "SCM6XX_ALL.h"

#define	I2C_TIMEOUT			40000
#define I2C_DEVIADDR 			0xa0		//device addr
#define	I2C_PAGESIZE 		64			//page size 64/16
#define SYS_FREQ  40000000   //以M（兆）为单位

#define I2C_MASTER 1
#define I2C_SLAVE 0 

typedef struct 
{
    UINT8 mode; //I2C_MASTER/I2C_SLAVE
    UINT32 scl;  //working frequency, for master and slave
    UINT32 prescale; //I2C_CFGR_DB0/I2C_CFGR_DB2/I2C_CFGR_DB4/I2C_CFGR_DB6,debounce , for master and slave
    UINT32 stretchEn; //I2C_CFGR_STR_EN/I2C_CFGR_STR_DIS, stretch or not, for slave only
    UINT32 timeoutEn; //I2C_CFGR_TO_EN/I2C_CFGR_TO_DIS, enable timeout  or not, for slave only
    UINT16 addrMode;//I2C_ADR_ADSE_10BIT/I2C_ADR_ADSE_7BIT,7-bit or 10-bit address, for slave only
    UINT16 addrMask;//I2C_ADR_ADMA_0~I2C_ADR_ADMA_7, for slave only
    UINT16 gCallEn;//I2C_ADR_GCA_EN/I2C_ADR_GCA_DIS, for slave only
    UINT16 slaveAddr;//slave address, for slave only
}I2C_InitTypeDef;

/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */

//void IIC_Init( SGCC_I2C_TypeDef* IICx,  I2C_InitTypeDef IIC_Init_Data) ;
void I2C_Enable_Int(SGCC_I2C_TypeDef* IICx, UINT32 IIC_Int_Type,FunctionalState opt);
void I2C_Enable(SGCC_I2C_TypeDef* IICx,FunctionalState opt);
void I2C_Write_Cmd( SGCC_I2C_TypeDef* IICx, UINT32 IIC_Cmd);
BOOL I2C_Get_Flag(SGCC_I2C_TypeDef* IICx, UINT32 IIC_Flag_Src); 
BOOL I2C_Wait_Flag(SGCC_I2C_TypeDef* IICx, UINT32 IIC_Flag_Src);
void I2C_Clear_Flag(SGCC_I2C_TypeDef* IICx, UINT32 IIC_Flag_Src);
void I2C_Send_Byte_Only(SGCC_I2C_TypeDef* IICx, UINT8 IIC_Data, UINT8 mode);
BOOL I2C_Send_Byte_Till_Done(SGCC_I2C_TypeDef* IICx, UINT8 IIC_Data, UINT8 mode); 
BOOL I2C_Send_Last_Byte(SGCC_I2C_TypeDef* IICx, UINT8 IIC_Data, UINT8 mode);
UINT8 I2C_Read_Buf(SGCC_I2C_TypeDef* IICx);
UINT8 I2C_Receive_Byte(SGCC_I2C_TypeDef* IICx); 
BOOL I2C_Receive_Last_Byte(SGCC_I2C_TypeDef* IICx, UINT8 *data );

#endif 





