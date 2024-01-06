/*
 *
 *  Created on: 2020��5��9��
 *      Author: CK
 */

#include <stdlib.h>
#include <device.h>
#include <port/drivers/fat_drivers.h>
#include <ff11/src/diskio.h>
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <string.h>
#include <unit_media.h>
#include "cpu_peri.h"
#include <spi_flash.h>

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_SpiFlashInstallFat(const char *TargetFs, s32 bstart, s32 bend, u32 doformat);
//    ModuleInstall_SpiFlashInstallFat(CFG_SPI_FLASH_FAT_MOUNT_NAME,CFG_SPI_FLASH_FAT_PART_START,CFG_SPI_FLASH_FAT_PART_END,CFG_SPI_FLASH_FAT_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"spi flash FAT"//SPI��QSPI�ӿڵ�nor flashоƬ
//parent:"spi flash"//��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"fat file system","spi flash"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"        //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_NORFLASH_SPI_FLASH_FAT == false )
//#warning  " spi flash FAT  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_NORFLASH_SPI_FLASH_FAT    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@string,1,10,
#define CFG_SPI_FLASH_FAT_MOUNT_NAME            "fat"      //"�ļ�ϵͳmount������",��Ҫ���ص�efs�ļ�ϵͳmount������
//%$#@num,-1,512,
#define CFG_SPI_FLASH_FAT_PART_START                 0        //"������ʼ"
#define CFG_SPI_FLASH_FAT_PART_END                   -1        //"��������"��-1��ʾ���һ��,��ʼ�����̶���0��ʼ���������-1�Ļ������������ǰ�顣����start=0,end=6,��ʹ�õķ�ΧΪ0~5
//%$#@enum,true,false,
#define CFG_SPI_FLASH_FAT_PART_FORMAT               false      //"����ѡ��,�Ƿ���Ҫ��ʽ���÷���"
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


static u32 sector_num = 0;
static u32 start_sector = 0;
#define     SECTOR_SIZE     _MAX_SS         //�ļ�ϵͳ������������С

int spi_flash_status(void);
int spi_flash_initialize(void);
int fat_spi_flash_read(BYTE *Buff, DWORD Sector, UINT Count);
int fat_spi_flash_write(BYTE *Buff, DWORD Sector, UINT Count);
int spi_flash_ioctl( BYTE Cmd, void *Buff);

extern struct Object *s_ptDeviceRoot;

struct FatDrvFuns W25QXX_Drv =
{
    .DrvStatus     = spi_flash_status,
    .DrvInitialize = spi_flash_initialize,
    .DrvRead       = fat_spi_flash_read,
    .DrvWrite      = fat_spi_flash_write,
    .DrvIoctl      = spi_flash_ioctl
};

// ============================================================================
// ����: W25qxx״̬
// ����: ��
// ���: 0 -- OK��1 -- ERROR
// ����:
// ============================================================================
s32 spi_flash_status(void)
{
    s32 res = 1; //RES_ERROR;

    if(SpiFlashIsInstall())
    {
        res = 0; //RES_OK;
    }
    else
    {
        res = 1; //RES_ERROR;
    }

    return res;
}

// ============================================================================
// ����: W25qxx��ʼ��
// ����:
// ����:
// ��ע: ���Խ��豸��ʼ����������;
// ============================================================================
s32 spi_flash_initialize(void)
{
    return (0);
}

// ============================================================================
// ����: ������
// ����: buff -- �����ݻ��档
//      sector -- Ŀ��ҳ�š�
//      count -- ҳ������
// ����: 0 -- ��ȷ; 1 -- ����; 3 -- �豸δ׼����
// ��ע:
// ============================================================================
s32 fat_spi_flash_read(u8 *buff, DWORD sector, u32 count)
{
    s32 res = 1; // RES_ERROR;

    if(SpiFlashRead(buff, ((u32)sector + start_sector) * SECTOR_SIZE, count * SECTOR_SIZE))
        res = 0;
    else
    {
        if(spi_flash_status())
            res = 3;
        else
            res = 1;
    }
    return res;
}

// ============================================================================
// ����: д����
// ����: buff -- д�����ݡ�
//      sector -- Ŀ��ҳ�š�
//      count -- ҳ������
// ����: 0 -- ��ȷ; 1 -- ����; 3 -- �豸δ׼����
// ��ע:
// ============================================================================
s32 fat_spi_flash_write(u8 *buff, DWORD sector, u32 count)
{
    s32 res = 1; // RES_ERROR;

    if(SpiFlashWrite(buff, ((u32)sector + start_sector) * SECTOR_SIZE, count * SECTOR_SIZE))
        res = 0;
    else
    {
        if(spi_flash_status())
            res = 3;
        else
            res = 1;
    }
    return res;

}

// ============================================================================
// ����:
// ����:
// ����:
// ��ע:
// ============================================================================
s32 spi_flash_ioctl( u8 cmd, void *buff)
{
    DRESULT res = 0; // RES_OK;

    switch (cmd) {
        /* Make sure that no pending write process */
        case CTRL_SYNC:
            res = 0; // RES_OK;
            break;

        /* Get number of sectors on the disk (DWORD) */
        case GET_SECTOR_COUNT:

            *(u32*)buff = sector_num;
            res =  0; // RES_OK;
            break;

        case GET_SECTOR_SIZE: /* Get R/W sector size (WORD) */
            *(u32*)buff = SECTOR_SIZE;
            res =  0; // RES_OK;
            break;

        /* Get erase block size in unit of sector (DWORD) */
        case GET_BLOCK_SIZE:
            *(u32*)buff = 1;    //SpiFlashWrite�Դ��˲������ܣ������ȡ�Ĳ�����С��ʵûʲô��
            res =  0; // RES_OK;
            break;

        default:
            res = 4; // RES_PARERR;
    }

    return res;
}

// =============================================================================
// ���ܣ���falsh�а�װfat�ļ�ϵͳ
// ������ TargetFs -- Ҫ���ص��ļ�ϵͳ
//      �������� -- ��ʼ�飬��������������ʱ�������ÿ飬ֻ�����ÿ����һ�飩���Ƿ��ʽ����
// ���أ��ɹ���true����ʧ�ܣ�false����
// ��ע��
// =============================================================================
bool_t ModuleInstall_SpiFlashInstallFat(const char *TargetFs, s32 bstart, s32 bend, u32 doformat)
{
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    u32 PageSize = 0;
    u32 PagePerBlock = 0;

    if(bend == -1)
    {
        __SpiFlashReq(totalblocks, &bend);
    }
    if((TargetFs != NULL) && (bstart < bend))
    {
        if(SpiFlashIsInstall())
        {

            __SpiFlashReq(unitbytes,(ptu32_t)&PageSize); 
            __SpiFlashReq(blockunits,(ptu32_t)&PagePerBlock); 
            sector_num = (bend - bstart) * (PageSize * PagePerBlock) / SECTOR_SIZE;    //��������˿�Ĵ�С������������С������������
            start_sector = bstart * (PageSize * PagePerBlock) / SECTOR_SIZE;
            if(doformat)
            {
                struct uesz sz;
                sz.unit = 0;
                sz.block = 1;
                if(-1 == __SpiFlashReq(format, bstart , bend, &sz))
                {
                    warning_printf("flash_fat"," Format failure.\r\n");
                    return false;
                }
            }

            if (__SpiFlashFsInstallInit(TargetFs,bstart,bend,&W25QXX_Drv) != 0)
            {
                return false;
            }
        }
    }
    return true;
}

