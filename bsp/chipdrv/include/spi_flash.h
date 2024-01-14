/*
 *
 *  Created on: 2020��5��7��
 *      Author: CK
 */

#ifndef __SPI_FLASH_H_
#define __SPI_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif


struct SPI_FlashInfo
{
    char            *BusName;               //оƬ����
    u32             ChipID;                 //оƬID
    u32             PageSize;               //ҳ��С
    u32             PageNumInSector;        //ÿ�������ж���ҳ
    u32             SectorNumInBlock;       //ÿ�����ж�������
    u32             Capacity;               //����������λ�ֽ�
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
