/*
 * w25qxx_fat.c
 *
 *  Created on: 2020��5��9��
 *      Author: CK
 */

#include <stdlib.h>
#include <device.h>
#include <fs/fat/port/drivers/fat_drivers.h>
#include <fs/fat/ff11/src/diskio.h>
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <string.h>
#include "cpu_peri.h"
#include "../w25qxx.h"

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_W25qxxInstallFat(const char *TargetFs, s32 bend, u32 doformat);
//    ModuleInstall_W25qxxInstallFat(CFG_W25_FAT_MOUNT_NAME,CFG_W25_FAT_PART_END,CFG_W25_FAT_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"W25QXX FAT"//SPI��QSPI�ӿڵ�nor flashоƬ
//parent:"norflash W25QXX"//��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"fat file system",//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"        //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_NORFLASH_W25QXX_FAT == false )
//#warning  " norflash_W25QXX  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_NORFLASH_W25QXX_FAT    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@string,1,10,
#define CFG_W25_FAT_MOUNT_NAME            "fat"      //"�ļ�ϵͳmount������",��Ҫ���ص�efs�ļ�ϵͳmount������
//%$#@num,-1,512,
#define CFG_W25_FAT_PART_END                   -1        //����������-1��ʾ���һ��,��ʼ�����̶���0��ʼ���������-1�Ļ������������ǰ�顣����start=0,end=6,��ʹ�õķ�ΧΪ0~5
//%$#@enum,true,false,
#define CFG_W25_FAT_PART_FORMAT               false      //����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


u32 sector_num = 0;
#define     SECTOR_SIZE     512         //�ļ�ϵͳ���õ���512,�����flash�Ĳ���Ҳ�ð�512�������ܰ�256��������ʵ���ǰ�ҳ��С����512��

int w25qxx_status(void);
int w25qxx_initialize(void);
int fat_w25qxx_read(BYTE *Buff, DWORD Sector, UINT Count);
int fat_w25qxx_write(BYTE *Buff, DWORD Sector, UINT Count);
int w25qxx_ioctl( BYTE Cmd, void *Buff);

extern struct Object *s_ptDeviceRoot;

struct FatDrvFuns W25QXX_Drv =
{
    .DrvStatus     = w25qxx_status,
    .DrvInitialize = w25qxx_initialize,
    .DrvRead       = fat_w25qxx_read,
    .DrvWrite      = fat_w25qxx_write,
    .DrvIoctl      = w25qxx_ioctl
};

// ============================================================================
// ����: W25qxx״̬
// ����: ��
// ���: 0 -- OK��1 -- ERROR
// ����:
// ============================================================================
s32 w25qxx_status(void)
{
    s32 res = 1; //RES_ERROR;

    if(W25qxx_is_install())
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
s32 w25qxx_initialize(void)
{
    return (0);
}

// ============================================================================
// ����: w25qxx������
// ����: buff -- �����ݻ��档
//      sector -- Ŀ��ҳ�š�
//      count -- ҳ������
// ����: 0 -- ��ȷ; 1 -- ����; 3 -- �豸δ׼����
// ��ע:
// ============================================================================
s32 fat_w25qxx_read(u8 *buff, DWORD sector, u32 count)
{
    s32 res = 1; // RES_ERROR;

    if(W25QXX_Read(buff, ((u32)sector) * SECTOR_SIZE, count * SECTOR_SIZE))
        res = 0;
    else
    {
        if(w25qxx_status())
            res = 3;
        else
            res = 0;
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
s32 fat_w25qxx_write(u8 *buff, DWORD sector, u32 count)
{
    s32 res = 1; // RES_ERROR;

    if(W25QXX_Write(buff, ((u32)sector) * SECTOR_SIZE, count * SECTOR_SIZE))
        res = 0;
    else
    {
        if(w25qxx_status())
            res = 3;
        else
            res = 0;
    }
    return res;

}

// ============================================================================
// ����:
// ����:
// ����:
// ��ע:
// ============================================================================
s32 w25qxx_ioctl( u8 cmd, void *buff)
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
            *(u32*)buff = 8;    //�൱��һ���������ҳ��
            res =  0; // RES_OK;
            break;

        default:
            res = 4; // RES_PARERR;
    }

    return res;
}

// =============================================================================
// ���ܣ���w25q�а�װfat�ļ�ϵͳ
// ������ TargetFs -- Ҫ���ص��ļ�ϵͳ
//      �������� -- ��ʼ�飬��������������ʱ�������ÿ飬ֻ�����ÿ����һ�飩���Ƿ��ʽ����
// ���أ��ɹ���true����ʧ�ܣ�false����
// ��ע��
// =============================================================================
bool_t ModuleInstall_W25qxxInstallFat(const char *TargetFs,s32 bend, u32 doformat)
{
    static char *name = "w25qxx";
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;

    if((TargetFs != NULL) && (0 != bend))
    {
        if(W25qxx_is_install())
        {
            if(bend == -1)
                __W25qxx_Req(totalblocks, &bend);
            sector_num = bend * 65536 / 512;

            if(doformat)
            {
                struct uesz sz;
                sz.unit = 0;
                sz.block = 1;
                if(-1 == __W25qxx_Req(format, 0 , bend, &sz))
                {
                    warning_printf("w25q"," Format failure.");
                    return false;
                }
            }

            if(!dev_Create((const char*)name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)name))
            {
                error_printf("w25q"," register device(w25q) failed.\r\n");
                return false; // register failure
            }

            targetobj = obj_matchpath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("w25q"," not found need to install file system.\r\n");
                return false;
            }
            super = (struct FsCore *)obj_GetPrivate(targetobj);
            super->MediaInfo = name;
            if(strcmp(super->pFsType->pType, "FAT") == 0)      //�����"FAT"Ϊ�ļ�ϵͳ�������������ļ�ϵͳ��filesystem�ṹ��
            {
                super->MediaDrv = &W25QXX_Drv;
            }
            else
            {
                super->MediaDrv = 0;
                error_printf("w25q","  install file system type not FAT.\r\n");
                return false;
            }
            FullPath = malloc(strlen(name)+strlen(s_ptDeviceRoot->name) + 1);  //��ȡmsc������·��
            sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name, name);
            FsBeMedia(FullPath, TargetFs);     //��w25q�Ϲ����ļ�ϵͳ
            free(FullPath);
        }
    }
    return true;
}

