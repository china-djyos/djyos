//#include "integer.h"
//#include "diskio.h"
#if 0   //todo---hemin modified for compile
#include <stdlib.h>
#include <cpu_peri_sdio.h>/* ��Դ������ԭ�� */
#include <fat/port/drivers/fat_drivers.h>
#include <fat/ff11/src/integer.h>
#include <fat/ff11/src/diskio.h>
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
#if 0
    SD_Error Status;
    DWORD Memory_Offset = sector << 9;/* ����ֵת��,Ŀǰ����ҳ��СΪ512ByteΪ��׼�ġ� */
    WORD Transfer_Length = count << 9;

    SD_ReadMultiBlocks((uint8_t*)buff, Memory_Offset, Transfer_Length, 1);
    Status = SD_WaitReadOperation();
    while (SD_GetStatus() != SD_TRANSFER_OK);/* Ŀǰ�������ȵķ�ʽ */
#else
    SD_Error Status;
    Status = SD_ReadDisk(buff, sector, count);
    if(SD_OK != Status)
        return (-1);
#endif
    return (0);/* ��ȷ���� */
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
#if 0
    SD_Error Status;
    DWORD Memory_Offset = sector << 9;/* ����ֵת��,Ŀǰ����ҳ��СΪ512ByteΪ��׼�ġ� */
    WORD Transfer_Length = count << 9;

    Status = SD_WriteMultiBlocks((uint8_t*)buff, Memory_Offset, Transfer_Length, 1);
    Status = SD_WaitWriteOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
    if ( Status != SD_OK )
    {
        return -1;/* ���󷵻� */
    }
#else
    SD_Error Status;
    Status = SD_WriteDisk(buff, sector, count);
    if(SD_OK != Status)
        return (-1);
#endif
    return (0);/* ��ȷ���� */
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
int MMC_ioctl( BYTE cmd, void *buff)
{

    extern SD_CardInfo SDCardInfo;
    switch(cmd)
    {
        case GET_SECTOR_SIZE:
            *(DWORD *)(buff) = SDCardInfo.CardBlockSize;
            break;
        case GET_SECTOR_COUNT:
            *(DWORD*)(buff) = SDCardInfo.CardCapacity >> 9;/*  */
            break;
        case GET_BLOCK_SIZE:
            *(DWORD*)(buff) = 1;/*  */
            break;
       default:
           break;
    }
    return (RES_OK);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
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
s32  ModuleInstall_SD(const char *ChipName, s32 Clean)
{
    SD_Error Ret;
    char *Name;
    (void)Clean;

    if(NULL == ChipName)
        return (-1);

    Name = (char*)malloc(strlen(ChipName)+1);
    if(NULL == Name)
        return (-2);

    strcpy(Name, ChipName);

    Ret = SD_Init();
    if(SD_OK == Ret)
    {
        if(Device_Create(Name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)&SDDrv))
            return (0);// �ɹ�
    }

    free(Name);
    return (-3);// ����ʧ��
}
#endif
