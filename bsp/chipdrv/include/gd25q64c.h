#ifndef __GD25Q64C_H
#define __GD25Q64C_H
#include "stdint.h"
#include "cpu_peri.h"
#include "systime.h"
#include "stddef.h"
#include <device/unit_media.h>

enum ReadRegister
{
    StatusReg1=0,
    StatusReg2,
    StatusReg3,
};

#define gd25q64c_ReadStatusReg1     0x05
#define gd25q64c_ReadStatusReg2     0x35
#define gd25q64c_ReadStatusReg3     0x15

#define gd25q64c_WriteRegister1      0x01
#define gd25q64c_WriteRegister2      0x31
#define gd25q64c_WriteRegister3      0x11

#define gd25q64c_WriteEnable        0x06
#define gd25q64c_WriteDisable       0x04
#define gd25q64c_ManufactDeviceID   0x90
#define gd25q64c_JedecID            0x9f
#define gd25q64c_ReadData           0x03
#define gd25q64c_QuadReadData       0x6B
#define gd25q64c_FastReadData       0x0B
#define gd25q64c_PageProgram        0x02
#define gd25q64c_QuadPageProgram    0x32
#define gd25q64c_SectorErase        0x20
#define gd25q64c_BlockErase         0xD8
#define gd25q64c_ChipErase          0xC7



//bool_t gd25q64c_Init(void);
//void gd25q64c_Qspi_Enable(void);
bool_t Gd25q64c_ReadSR(u8 regno, u8 *sta);
bool_t Gd25q64c_WriteSR(u8 regno, u8 data);
bool_t Gd25q64c_Write_Enable(void);
bool_t Gd25q64c_Write_Disable(void);
u32 Gd25q64c_ReadID(void);
bool_t Gd25q64c_Read(u8* pBuffer,u32 ReadAddr,u32 NumByteToRead);
bool_t Gd25q64c_ReadPage(u8* pBuffer,u32 PageNum);
bool_t Gd25q64c_WaitReady(u32 timeout);
bool_t Gd25q64c_WritePage(u8* pBuffer,u32 PageNum);
bool_t Gd25q64c_WriteNoErase(u8* pBuffer,u32 WriteAddr,u32 NumByteToWrite);
bool_t Gd25q64c_Write(u8* pBuffer,u32 WriteAddr,u32 len);
bool_t Gd25q64c_Erase_Sector(u32 SectorNum);
bool_t Gd25q64c_Erase_Chip(void);
bool_t Gd25q64c_Erase_Block(u32 BlockNum);
bool_t Gd25_is_install(void);
s32 __gd25q64c_req(enum ucmd cmd, ptu32_t args, ...);
bool_t __GD25_FsInstallInit(const char *fs, s32 dwStart, s32 dwEnd, void *mediadrv);




#endif

