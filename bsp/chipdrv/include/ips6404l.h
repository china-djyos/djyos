#ifndef __GD25Q64C_H
#define __GD25Q64C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "cpu_peri.h"
#include "systime.h"
#include "stddef.h"

#define Ips6404l_ManufactDeviceID   0x9F
#define Ips6404l_ReadData           0x03
#define Ips6404l_QuadReadData       0xEB
#define Ips6404l_FastReadData       0x0B

#define Ips6404l_WriteData          0x02
#define Ips6404l_QuadWriteData      0x38

#define Ips6404l_ResetEnable        0x66
#define Ips6404l_Reset              0x99
#define Ips6404l_BurstModeToggle    0xC0

#define Ips6404l_EnterQuadMode      0x35
#define Ips6404l_ExitQuadMode       0xF5

bool_t Ips6404l_Read(u32 ReadAddr, u8* pBuffer, u32 len);
bool_t Ips6404l_Write(u32 WriteAddr, u8* pBuffer, u32 len);
#ifdef __cplusplus
}
#endif


#endif

