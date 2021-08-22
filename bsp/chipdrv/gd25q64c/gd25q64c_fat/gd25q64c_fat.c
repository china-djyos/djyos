/*
 * gd25q64c_fat.c
 *
 *  Created on: 2020��5��9��
 *      Author: CK
 */

#include <stdlib.h>
#include <device.h>
#include <fat/port/drivers/fat_drivers.h>
#include <fat/ff11/src/diskio.h>
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <string.h>
#include <device/unit_media.h>
#include "cpu_peri.h"
#include "gd25q64c.h"

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_Gd25q64cInstallFat(const char *TargetFs, s32 bend, u32 doformat);
//    ModuleInstall_Gd25q64cInstallFat(CFG_W25_FAT_MOUNT_NAME,CFG_W25_FAT_PART_END,CFG_W25_FAT_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"gd25q64c install fat"//SPI��QSPI�ӿڵ�nor flashоƬ
//parent:"nor flash gd25q64c"//��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"fat file system","nor flash gd25q64c"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"        //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_NORFLASH_GD25Q64C_FAT == false )
//#warning  " norflash_GD25Q64C  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_NORFLASH_GD25Q64C_FAT    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
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

#if _MIN_SS == _MAX_SS
#define     SECTOR_SIZE     _MIN_SS     //ҳ�ߴ���밴���ļ�ϵͳ�Ķ�����
#else
#define     SECTOR_SIZE     256
#endif

int gd25q64c_status(void);
int gd25q64c_initialize(void);
int fat_gd25q64c_read(BYTE *Buff, DWORD Sector, UINT Count);
int fat_gd25q64c_write(BYTE *Buff, DWORD Sector, UINT Count);
int gd25q64c_ioctl( BYTE Cmd, void *Buff);


struct FatDrvFuns GD25Q64C_Drv =
{
    .DrvStatus     = gd25q64c_status,
    .DrvInitialize = gd25q64c_initialize,
    .DrvRead       = fat_gd25q64c_read,
    .DrvWrite      = fat_gd25q64c_write,
    .DrvIoctl      = gd25q64c_ioctl
};

// ============================================================================
// ����: gd25q64c״̬
// ����: ��
// ���: 0 -- OK��1 -- ERROR
// ����:
// ============================================================================
s32 gd25q64c_status(void)
{
    s32 res = 1; //RES_ERROR;

    if(Gd25_is_install())
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
// ����: gd25q64c��ʼ��
// ����:
// ����:
// ��ע: ���Խ��豸��ʼ����������;
// ============================================================================
s32 gd25q64c_initialize(void)
{
    return (0);
}

// ============================================================================
// ����: gd25q64c������
// ����: buff -- �����ݻ��档
//      sector -- Ŀ��ҳ�š�
//      count -- ҳ������
// ����: 0 -- ��ȷ; 1 -- ����; 3 -- �豸δ׼����
// ��ע:
// ============================================================================
s32 fat_gd25q64c_read(u8 *buff, DWORD sector, u32 count)
{
    s32 res = 1; // RES_ERROR;

    if(Gd25q64c_Read(buff, ((u32)sector) * SECTOR_SIZE, count * SECTOR_SIZE))
        res = 0;
    else
    {
        if(gd25q64c_status())
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
s32 fat_gd25q64c_write(u8 *buff, DWORD sector, u32 count)
{
    s32 res = 1; // RES_ERROR;
    if(Gd25q64c_Write(buff, ((u32)sector) * SECTOR_SIZE, count * SECTOR_SIZE))
        res = 0;
    else
    {
        if(gd25q64c_status())
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
s32 gd25q64c_ioctl( u8 cmd, void *buff)
{
    DRESULT res = 0; // RES_OK;

    switch (cmd) {
        /* Make sure that no pending write process */
        case CTRL_SYNC:
            res = 0; // RES_OK; todo:ֱ�ӷ���OK���������й��ŵĲ����أ�
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
// ������TargetFs��Ҫ���ص��ļ�ϵͳ
//       bend����������������ʱ�������ÿ飬ֻ�����ÿ����һ�飩��
//       doformat��1=��װʱ��ʽ����0=��װʱ����ʽ��
// ���أ��ɹ���true����ʧ�ܣ�false����
// ��ע��
// =============================================================================
bool_t ModuleInstall_Gd25q64cInstallFat(const char *TargetFs,s32 bend, u32 doformat)
{
    char *notfind;
    struct Object *targetobj;
    struct FsCore *super;

    if((TargetFs != NULL) && (0 != bend))
    {
        if(Gd25_is_install())
        {
            if(bend == -1)
                __gd25q64c_req(totalblocks, &bend);
            sector_num = bend * 65536 / SECTOR_SIZE;

//            printf("sector_num = %d, doformat = %d \r\n",sector_num,doformat);
            if(doformat)
            {
                struct uesz sz;
                sz.unit = 0;
                sz.block = 1;
                if(-1 == __gd25q64c_req(format, 0 , bend, &sz))
                {
                    warning_printf("w25q"," Format failure.");
                    return false;
                }
            }
//            printf("Format success\r\n");
            targetobj = OBJ_MatchPath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("w25q"," not found need to install file system.\r\n");
                return false;
            }
            super = (struct FsCore *)OBJ_GetPrivate(targetobj);
            if(strcmp(super->pFsType->pType, "FAT") == 0)      //�����"FAT"Ϊ�ļ�ϵͳ�������������ļ�ϵͳ��filesystem�ṹ��
            {
                if(__GD25_FsInstallInit(TargetFs,0,bend,&GD25Q64C_Drv) == true)
                    return true;
                else
                    printf("Init fail\r\n");
            }
//            printf("FAT fail\r\n");
        }
        else
            printf("Gd25_is_install fail\r\n");
    }
    else
        printf("TargetFs = %s, bend = %d\r\n",TargetFs,bend);
    return true;
}

