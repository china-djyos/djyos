#include <device/card/card.h>
#include <stdlib.h>
#include <fs/fat/port/drivers/fat_drivers.h>
#include <fs/fat/ff11/src/integer.h>
#include <fs/fat/ff11/src/diskio.h>

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32  ModuleInstall_SD(const char *ChipName, s32 Clean);
//    ModuleInstall_SD(CFG_SD_CHIP_NAME,CFG_SD_CLEAN);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_hsmmc"      //CPU��hsmmc����
//parent:none                          //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                        //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                     //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                       //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                     //��ʼ��ʱ������ѡֵ��early��medium��later��
                                       //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"djyfs","heap","devfile"  //������������������������none����ʾ�������������
                                       //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                       //����������������������г����á�,���ָ�
//weakdependence:"none"                //��������������������������none����ʾ�������������
                                       //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                       //����������������������г����á�,���ָ�
//mutex:"none"                         //������������������������none����ʾ�������������
                                       //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_SD_CLEAN   //****�������Ƿ��Ѿ����ú�
#warning    cpu_peri_hsmmc�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100,
#define CFG_SD_CLEAN                  (0)     //"�����Ƿ����SD",
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_SD_CHIP_NAME              ("/dev/sd")   //"SD����",�������ϼ���·��
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


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

//-----------------------------------------------------------------------------
//����:
//����: ChipName --
//      Clean -- ������ʽ��;"1"--��;"0"--��
//����: "0" -- �ɹ�;
//      "-1" -- �����������;
//      "-2" -- �ڴ治��;
//      "-3" -- ����ʧ��;
//��ע: todo������һ��sd����ṹ��
//-----------------------------------------------------------------------------
struct FatDrvFuns SDDrv =
{
    .DrvStatus     = MMC_disk_status,
    .DrvInitialize = MMC_disk_initialize,
    .DrvRead       = MMC_disk_read,
    .DrvWrite      = MMC_disk_write,
    .DrvIoctl      = MMC_ioctl
};
extern struct CardDescription Card;
s32  ModuleInstall_SD(const char *ChipName, s32 Clean)
{
    s32 Ret;
    char *Name;
    (void)Clean;

    if(NULL == ChipName)
        return (-1);

    Name = (char*)malloc(strlen(ChipName)+1);
    if(NULL == Name)
        return (-2);

    strcpy(Name, ChipName);

    Ret = S3c2416_HsmmcInit(1);
    if(0 == Ret)
    {
        if(dev_add(NULL,Name, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)&SDDrv)))
        {
            if((0 == Clean) || ((Clean) &&
              (0 == S3c2416_BlkErase(0, ((Card.CapacityInBytes >> Card.BlkLenSettings)-1)))))
                  return (0);// �ɹ�
        }

    }

    free(Name);
    return (-3);// ����ʧ��
}
