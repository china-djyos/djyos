#ifndef __SYS_I2C_H__
#define __SYS_I2C_H__

#include "sys_types.h"

#define  TWI0  0  //0x01C2 7000---0x01C2 73FF  1K
#define  TWI1  1  //0x01C2 7400---0x01C2 77FF  1K
#define  TWI2  2  //0x01C2 7800---0x01C2 7BFF  1K

//ËÙÂÊ
#define  Rate400K 400000
#define  Rate100K 100000

extern u8 slave_addr;

void AT24C256_Demo(void);
void I2C_Init(int TWIx);	
void I2C_Set_Slave_Addr(u8 addr);
void I2C_Set_Rate(int TWIx, u64_t rate);
int write_byte(int TWIx,u16 addr,u8 addr_len,u8* buff,u16 len);
int read_byte(int TWIx,u16 addr,u8 addr_len,u8* buff,u16 len);
//---------------------------------------------------------------
int f1c100s_i2c_data_write(int TWIx,u8 *buff,int len);
int f1c100s_i2c_write(int TWIx, u8 addr,u8 *buff,int len);
int f1c100s_i2c_read(int TWIx, u8 addr,u8 *buff,int len);
int f1c100s_i2c_stop(int TWIx);
int f1c100s_i2c_start(int TWIx);
void I2C_SetReadDelayUs(int us);
#endif
