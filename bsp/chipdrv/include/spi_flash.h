/*
 *
 *  Created on: 2020年5月7日
 *      Author: CK
 */

#ifndef __SPI_FLASH_H_
#define __SPI_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif


struct SPI_FlashInfo
{
    char            *BusName;               //芯片名称
    u32             ChipID;                 //芯片ID
    u32             PageSize;               //页大小
    u32             PageNumInSector;        //每个扇区有多少页
    u32             SectorNumInBlock;       //每个块有多少扇区
    u32             Capacity;               //总容量，单位字节
};


bool_t SpiFlashIsInstall(void);
bool_t SpiFlashWrite(u8* buf,u32 addr,u32 len);
bool_t SpiFlashRead(u8* buf,u32 addr,u32 len);
bool_t SpiFlashEraseChip(void);
bool_t SpiFlashEraseSector(u32 addr);
s32 __SpiFlashReq(enum ucmd cmd, ptu32_t args, ...);
bool_t SpiFlashEraseBlock(u32 addr);
s32 __SpiFlashFsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);

#ifdef __cplusplus
}
#endif

#endif
