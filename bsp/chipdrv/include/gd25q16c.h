#ifndef __gd25q16c
#define __gd25q16c
#include "stdint.h"
#include "cpu_peri.h"
#include "systime.h"
#include "stddef.h"


#define gd25q16c_ReadStatusReg1     0x05
//#define gd25q16c_ReadConfigurationReg1     0x35
#define gd25q16c_ReadStatusReg2     0x35

enum ReadRegister
{
    StatusReg1=0,
    StatusReg2,
};

#define gd25q16c_WriteEnable        0x06
#define gd25q16c_WriteDisable       0x04
#define gd25q16c_WriteRegister      0x01
#define gd25q16c_ManufactDeviceID   0x90
#define gd25q16c_ReadData           0x03
#define gd25q16c_QuadReadData       0x6B
#define gd25q16c_FastReadData       0x0B
#define gd25q16c_PageProgram        0x02
#define gd25q16c_QuadPageProgram    0x32
#define gd25q16c_SectorErase        0x20
#define gd25q16c_ChipErase          0xC7



//bool_t gd25q16c_Init(void);
//void gd25q16c_Qspi_Enable(void);
s32 gd25q16c_ReadSR(u8 regno);
s32 gd25q16c_Write_SR1_CFR1_SR2(u8 *sr);
s32 gd25q16c_Write_Enable(void);
s32 gd25q16c_Write_Disable(void);
u16 gd25q16c_ReadID(void);
s32 gd25q16c_ReadMemory(u8* pBuffer,u32 ReadAddr,u32 NumByteToRead);
s32 gd25q16c_ReadPage(u8* pBuffer,u32 PageNum);
bool_t gd25q16c_WaitReady(u32 timeout);
s32 gd25q16c_WritePage(u8* pBuffer,u32 PageNum);
s32 gd25q16c_WriteMemory(u8* pBuffer,u32 WriteAddr,u32 NumByteToWrite);
s32 gd25q16c_Erase_Sector(u32 SectorNum);
s32 gd25q16c_Erase_Chip(void);
bool_t gd25q16c_EraseWait(void);




#endif

