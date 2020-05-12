/*
 * w25qxx.h
 *
 *  Created on: 2020Äê5ÔÂ7ÈÕ
 *      Author: CK
 */

#ifndef __W25QXX_H_
#define __W25QXX_H_

#ifdef __cplusplus
extern "C" {
#endif



#define DataMode_No             (0<<6)
#define DataMode_Single         (1<<6)
#define DatatMode_Two           (2<<6)
#define DataMode_Four           (3<<6)

#define AddrLen_8Bit            (0<<4)
#define AddrLen_16Bit           (1<<4)
#define AddrLen_24Bit           (2<<4)
#define AddrLen_32Bit           (3<<4)

#define AddrMode_No             (0<<2)
#define AddrMode_Single         (1<<2)
#define AddrtMode_Two           (2<<2)
#define AddrMode_Four           (3<<2)

#define InstructMode_No         (0<<0)
#define InstructMode_Single     (1<<0)
#define InstructMode_Two        (2<<0)
#define InstructMode_Four       (3<<0)

#define W25X_WriteEnable        0x06
#define W25X_WriteDisable       0x04
#define W25X_ReadStatusReg1     0x05
#define W25X_ReadStatusReg2     0x35
#define W25X_ReadStatusReg3     0x15
#define W25X_WriteStatusReg1    0x01
#define W25X_WriteStatusReg2    0x31
#define W25X_WriteStatusReg3    0x11
#define W25X_ReadData           0x03
#define W25X_FastReadData       0x0B
#define W25X_FastReadDual       0x3B
#define W25X_PageProgram        0x02
#define W25X_BlockErase         0xD8
#define W25X_SectorErase        0x20
#define W25X_ChipErase          0xC7
#define W25X_PowerDown          0xB9
#define W25X_ReleasePowerDown   0xAB
#define W25X_DeviceID           0xAB
#define W25X_ManufactDeviceID   0x90
#define W25X_JedecDeviceID      0x9F
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9
#define W25X_SetReadParam       0xC0
#define W25X_EnterQPIMode       0x38
#define W25X_ExitQPIMode        0xFF

bool_t W25qxx_is_install(void);
bool_t W25QXX_Write(u8* buf,u32 addr,u16 len);
bool_t W25QXX_Read(u8* buf,u32 addr,u32 len);
bool_t W25QXX_EraseChip(void);
bool_t W25QXX_EraseSector(u32 addr);
s32 __W25qxx_Req(enum ucmd cmd, ptu32_t args, ...);
bool_t W25QXX_EraseBlock(u32 addr);

#ifdef __cplusplus
}
#endif

#endif /* IBOOT_W25QXX_W25QXX_H_ */
