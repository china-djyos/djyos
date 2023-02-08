#include <device/card/card.h>
#include <stdlib.h>
#include <fat/port/drivers/fat_drivers.h>
#include <fat/ff11/src/integer.h>
#include <fat/ff11/src/diskio.h>
#include <systime.h>
#include <djyfs/filesystems.h>
#include <dbug.h>
#include <string.h>
#include <device.h>
#include <stdbool.h>

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32  ModuleInstall_SD(const char *targetfs,s32 doformat);
//    ModuleInstall_SD(CFG_SDCARD_FSMOUNT_NAME,CFG_SDCARD_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip hsmmc"//CPU��hsmmc����
//parent                       //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                        //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                     //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                       //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                     //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                       //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"file system","heap","device file system"//������������������������none����ʾ�������������
                                       //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                       //����������������������г����á�,���ָ�
//weakdependence:"none"                //��������������������������none����ʾ�������������
                                       //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                       //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                       //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_HSMMC == false )
//#warning  " cpu_onchip_hsmmc  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_HSMMC    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
#define CFG_SDCARD_FORMAT            false    //�Ƿ���Ҫ������ʽ����
//%$#@string,1,10,
#define CFG_SDCARD_FSMOUNT_NAME      "fat"    //�谲װ���ļ�ϵͳ��mount������
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


extern struct Object *s_ptDeviceRoot;
//-----------------------------------------------------------------------------
//����:
//����:
//���:
//����:
//-----------------------------------------------------------------------------
int MMC_disk_status(void)
{
    return (0);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע: ���Խ��豸��ʼ����������;
//-----------------------------------------------------------------------------
int MMC_disk_initialize(void)
{
    static BYTE Done = 0;

    if(1 == Done)
        return (0);

    Done = 1;
    return (0);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
int MMC_disk_read(BYTE *buff, DWORD sector, UINT count)
{
    u32 *Buf = (u32*)buff;
    extern s32 S3c2416_BlkRead(const u32 *Buf, u32 BlkNo, u32 Count);

    if(S3c2416_BlkRead(Buf, sector, count))
        return (-1);// ʧ��

    return (0);// ��ȷ
}
//-----------------------------------------------------------------------------
//����:
//����: buff -- д�����ݡ�
//     sector -- Ŀ��ҳ�š�
//     count -- ҳ������
//����: 0 -- ��ȷ;-1 -- ����;
//��ע:
//-----------------------------------------------------------------------------
int MMC_disk_write(BYTE *buff, DWORD sector, UINT count)
{

    u32 *Buf = (u32*)buff;
    extern s32 S3c2416_BlkWrite(const u32 *Buf, u32 BlkNo, u32 Count);

    if(S3c2416_BlkWrite(Buf, sector, count))
        return (-1);// ʧ��
    return (0);// ��ȷ
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
int MMC_ioctl( BYTE cmd, void *buff)
{

    extern struct CardDescription Card;
    switch(cmd)
    {
        case GET_SECTOR_SIZE:
            *(DWORD *)(buff) = (1 << Card.BlkLenSettings);//
            break;
        case GET_SECTOR_COUNT:
            *(DWORD*)(buff) = Card.CapacityInBytes >> Card.BlkLenSettings;//
            break;
        case GET_BLOCK_SIZE:
            *(DWORD*)(buff) = 1;// todo
            break;
       default:
           break;
    }
    return (RES_OK);
}


struct FatDrvFuns SDDrv =
{
    .DrvStatus     = MMC_disk_status,
    .DrvInitialize = MMC_disk_initialize,
    .DrvRead       = MMC_disk_read,
    .DrvWrite      = MMC_disk_write,
    .DrvIoctl      = MMC_ioctl
};
extern struct CardDescription Card;
//-----------------------------------------------------------------------------
//����:
//����: targetfs -- Ҫ�����ļ�ϵͳ��mount������
//      Clean -- ������ʽ��;"1"--��;"0"--��
//����: "0" -- �ɹ�;
//      "-1" -- �����������;
//      "-2" -- �ڴ治��;
//      "-3" -- ����ʧ��;
//��ע: todo������һ��sd����ṹ��
//-----------------------------------------------------------------------------
s32  ModuleInstall_SD(const char *targetfs,u8 doformat)
{
    s32 Ret;
    const char *ChipName = "sdcard";    // �豸��;
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;

    if(NULL == ChipName)
        return (-1);

    Ret = S3c2416_HsmmcInit(1);
    if(0 == Ret)
    {
        if(Device_Create(ChipName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)ChipName)))
        {
            if((false == doformat) || ((doformat) &&
              (0 != S3c2416_BlkErase(0, ((Card.CapacityInBytes >> Card.BlkLenSettings)-1)))))
            {
                printk("\r\nMODULE : INSTALL : \"%s\" format failed during initialization.\r\n", defaultName);
                return (-3);// ʧ��
            }

            if(targetfs != NULL)
            {
                targetobj = OBJ_MatchPath(targetfs, &notfind);
                if(notfind)
                {
                    error_printf("SDCARD"," not found need to install file system.\r\n");
                    return -1;
                }
                super = (struct FsCore *)OBJ_GetPrivate(targetobj);
                super->MediaInfo = ChipName;
                if(strcmp(super->pFsType->pType, "FAT") == 0)      //�����"FAT"Ϊ�ļ�ϵͳ�������������ļ�ϵͳ��filesystem�ṹ��
                {
                    super->MediaDrv = &SDDrv;
                }
                else
                {
                    super->MediaDrv = 0;
                    error_printf("SDCARD","  install file system type not FAT\r\n");
                    return -1;
                }

                FullPath = malloc(strlen(ChipName)+strlen(s_ptDeviceRoot->name));  //��ȡmsc������·��
                sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,ChipName);
                File_BeMedia(FullPath,targetfs);     //��msc�Ϲ����ļ�ϵͳ
                free(FullPath);
            }
            else
            {
                warning_printf("SDCARD", "  No file system is installed\r\n");
            }
            return (0);
        }

    }

    return (-3);// ����ʧ��
}
