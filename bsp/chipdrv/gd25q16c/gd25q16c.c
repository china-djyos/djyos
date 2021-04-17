// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��gd25q16c.c
// ģ������: Qflash��ʹ��QSPIͨ��
// ģ��汾: V1.00
// ������Ա: wangxy
// ����ʱ��: 2018.08.30
// =============================================================================


#include "gd25q16c.h"
#include "stdint.h"
#include "cpu_peri.h"
#include "systime.h"
#include "stddef.h"
#include <string.h>
#include <stdlib.h>
#include <djyos.h>
#include <math.h>
#include "lock.h"
#include "spibus.h"
#include "djybus.h"
#include "driver.h"
#include <device/unit_media.h>
#include <device.h>
#include <djyfs/filesystems.h>
#include <include/spi_pub.h>
#include "dbug.h"
#include "cpu_peri_spi.h"

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_gd25q16c(void);
//    ModuleInstall_gd25q16c();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"nor flash gd25q16c"//spi�ӿڵ�norflash
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","heap","spi bus","cpu onchip spi"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"easy file system"     //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_GD25_PART_FORMAT           //****�������Ƿ��Ѿ����ú�
#warning   gd25q16c�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,-1,1024,
//%$#@enum,true,false,
#define CFG_GD25_PART_FORMAT               false      //����ѡ��,�Ƿ���Ҫ������оƬ��
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


//*****************************************************************//
//QFLASH˵����Ŀǰ�����£���оƬ��flash����2M��һ������16ҳ��һҳ256�ֽ�����
//�ڶ�д����ǰ�����оƬ�ֲ���ÿ��ָ���ʱ��ͼʹ��QSPI_Send_CMD�������ã����磺
//QPI,���߶�����,��ַΪReadAddr,1�ߴ���ָ��_1�ߴ���24λ��ַ_8������_4�ߴ�������,NumByteToRead������
//���ʹ��QSPI_Receive��QSPI_Transmit
//*****************************************************************//


//struct SemaphoreLCB pgd25q16c_Lock;   //оƬ������ʱ���
struct MutexLCB *pgd25q16c_Lock;           //оƬ������ʱ���
bool_t gd25q16c_Init_Flag=false;
u16 gd25q16c_TYPE          = 0xc814;
u8 gd25q16c_QPI_MODE       = 0;            //QSPIģʽ��־:0,SPIģʽ;1,QPIģʽ.

                                            //flash����16M
u32 gd25q16c_SectorSize    = 16 * 256;    //һ������4k�ֽ�����
u32 gd25q16c_PageSize      = 256;          //һҳ256�ֽ�����

u32 gd25q16c_OP_TIMEOUT= 1800000; // 30����
static char *flash_name = "gd25q16c";
struct NorDescr *qflashdescription = NULL;
struct umedia *gd25_umedia;
extern struct Object *s_ptDeviceRoot;

s32 gd25q16c_Write_Enable(void);
bool_t gd25q16c_WaitReady(u32 timeout);
//=====================================================================
//�������� Ƭѡ����
//������
//����ֵ��
//���ܣ�Ƭѡ���ͣ�ѡ������
//=====================================================================
static void gd25q16c_CsActive(void)
{
    u32 param;
    param = 0x2;
    spi_ctrl(CMD_SPI_SET_NSSID, (void *)&param);
}
//=====================================================================
//��������Ƭѡ����
//������
//����ֵ��
//���ܣ�Ƭѡ���ߣ���������
//=====================================================================
static void gd25q16c_CsInActive(void)
{
    u32 param;
    param = 0x3;
    spi_ctrl(CMD_SPI_SET_NSSID, (void *)&param);
}

//=====================================================================
//��������SPI���ͽ��պ���
//���������͵����ݵ�ַ�����ݳ��ȡ����ܵ����ݵ�ַ�����ݳ��ȡ�ƫ���������ͼ�����֮���ٿ�ʼ��һ����һ����ǰ���ǿն���
//����ֵ��0���ɹ� �� -1ʧ��
//���ܣ�SPI���ͽ���
//=====================================================================
static s32 gd25q16c_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen,u32 RecvOff)
{
    if(SPI_TxRx(sdata, slen, rdata, rlen, RecvOff) == false)
        return -1;
    return 0;
}

//=====================================================================
//�����������Ĵ���ָ��
//����������ļĴ���ö����
//����ֵ��-1:ʧ�� �� ���� ���Ĵ�����ֵ
//���ܣ�
//=====================================================================
s32 gd25q16c_ReadSR(u8 regno)
{
    u8 command[1];
    u8 byte;
    switch(regno)
    {
        case StatusReg1:
            command[0] = gd25q16c_ReadStatusReg1;           //��״̬�Ĵ���1ָ��
            break;
//        case ConfigurationReg1:
//           command[0] = gd25q16c_ReadConfigurationReg1;    //��״̬�Ĵ���1ָ��
//            break;
        case StatusReg2:
            command[0] = gd25q16c_ReadStatusReg2;           //��״̬�Ĵ���2ָ��
            break;
        default:
            command[0] = gd25q16c_ReadStatusReg1;
            break;
    }

    gd25q16c_CsActive();
    if(gd25q16c_TxRx(command,1,&byte,1,1) == -1)
    {
        gd25q16c_CsInActive();
        return -1;
    }
    gd25q16c_CsInActive();

    return byte;
}


//=====================================================================
//��������д�Ĵ���ָ��
//���������������Ĵ���ֵ������ָ��
//����ֵ��0���ɹ� �� -1ʧ��
//���ܣ���˳��д�����Ĵ���
//=====================================================================
s32 gd25q16c_Write_SR1_SR2(u8 *sr)
{
    u8 sndbuf[3];

    if(gd25q16c_WaitReady(200000) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Write Reg.");
        return -1;
    }

    if(gd25q16c_Write_Enable() != 0)                  //дʹ��
    {
        return -1;
    }

    sndbuf[0] = gd25q16c_WriteRegister;
    sndbuf[1] = *sr;
    sndbuf[2] = *(sr + 1);

    gd25q16c_CsActive();

    if(gd25q16c_TxRx(sndbuf,3,NULL,0,0) == -1)
    {
        gd25q16c_CsInActive();
        return -1;
    }

    gd25q16c_CsInActive();

    return 0;
}

//=====================================================================
//��������дʹ�ܵȴ�
//������
//����ֵ��0���ɹ� �� -1ʧ��
//���ܣ�
//=====================================================================
s16 gd25q16c_WriteEnableWait(void)
{
    u8 sndbuf[1];
    u8 rcvbuf[1];
    u32 wait = 20000;

    sndbuf[0] = gd25q16c_ReadStatusReg1;

    do
    {
        gd25q16c_CsActive();

        gd25q16c_TxRx(sndbuf,1,rcvbuf,1,1);

        gd25q16c_CsInActive();
//        DJY_DelayUs(100);
        wait --;
        if(wait == 0)
        {
            return -1;
        }
    }while((rcvbuf[0] & 0x02) != 0x02);

    return 0;
}

//=====================================================================
//��������дʧ�ܵȴ�
//������
//����ֵ��0���ɹ� �� -1ʧ��
//���ܣ�
//=====================================================================
s16 gd25q16c_WriteDisableWait(void)
{
    u8 sndbuf[1];
    u8 rcvbuf[1];
    u32 wait = 20000;

    sndbuf[0] = gd25q16c_ReadStatusReg1;

    do
    {
        gd25q16c_CsActive();

        gd25q16c_TxRx(sndbuf,1,rcvbuf,1,1);

        gd25q16c_CsInActive();
//        DJY_DelayUs(100);
        wait --;
        if(wait == 0)
        {
            return -1;
        }
    }while((rcvbuf[0] & 0x02) == 0x02);

    return 0;
}

//=====================================================================
//��������дʹ��
//������
//����ֵ��0���ɹ� �� -1ʧ��
//���ܣ�����QFLASH��д����ǰ��Ҫ��ʹ��
//=====================================================================
s32 gd25q16c_Write_Enable(void)
{
    u8 sndbuf[1];

    sndbuf[0] = gd25q16c_WriteEnable;

    gd25q16c_CsActive();

    gd25q16c_TxRx(sndbuf,1,NULL,0,0);

    gd25q16c_CsInActive();

    if(gd25q16c_WriteEnableWait() == -1)
    {
        printf("\r\n gd25q16c Write Enable fail.\r\n");
        return -1;
    }

    return 0;

}


//=====================================================================
//��������дʧ��
//������
//����ֵ��0���ɹ� �� -1ʧ��
//���ܣ�
//=====================================================================
s32 gd25q16c_Write_Disable(void)
{
    u8 sndbuf[1];

    sndbuf[0] = gd25q16c_WriteDisable;

    gd25q16c_CsActive();

    gd25q16c_TxRx(sndbuf,1,NULL,0,0);

    gd25q16c_CsInActive();

    if(gd25q16c_WriteDisableWait() == -1)
    {
        printf("\r\n gd25q16c Write Disable fail.\r\n");
        return -1;
    }

    return 0;

}

//=====================================================================
//���������ȴ�QFLASH��busyλ���
//������
//����ֵ��true ���ɹ���false ��ʧ��
//���ܣ�
//=====================================================================
bool_t gd25q16c_WaitReady(u32 timeout)
{
    u8 sr1=1;
    u32 timeoutcnt=0;

    sr1 = gd25q16c_ReadSR(StatusReg1);

    while( (sr1 & 0x01) == 0x01)   // �ȴ�BUSYλ���
    {
        sr1 = gd25q16c_ReadSR(StatusReg1);

//        DJY_EventDelay(100);
        timeoutcnt++;
        if(timeoutcnt == timeout)
        {
            return false;
        }
    }
    return true;
}

//=====================================================================
//����������������
//���������
//����ֵ��0:�ɹ���-1��ʧ��
//���ܣ�
//=====================================================================
s32 gd25q16c_Erase_Sector(u32 SectorNum)
{
    u32 Dst_Addr;
    u8 sndbuf[4];
    Lock_MutexPend(pgd25q16c_Lock,CN_TIMEOUT_FOREVER);

    if(gd25q16c_WaitReady(200000) == false)
    {
        Lock_MutexPost(pgd25q16c_Lock);
        printf("\r\n QFLASH : debug : device is busy before Sector Erase.");
        return -1;
    }


    if(gd25q16c_Write_Enable() != 0)
    {
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    Dst_Addr = SectorNum * gd25q16c_SectorSize;

    sndbuf[0] = gd25q16c_SectorErase;
    sndbuf[3] = Dst_Addr & 0xff;
    sndbuf[2] = (Dst_Addr >> 8) & 0xff;
    sndbuf[1] = (Dst_Addr >> 16) & 0xff;

    gd25q16c_CsActive();

    if(gd25q16c_TxRx(sndbuf,4,NULL,0,0) == -1)
    {
        gd25q16c_CsInActive();
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    gd25q16c_CsInActive();


    if(gd25q16c_WaitReady(200000) == false)
    {
        Lock_MutexPost(pgd25q16c_Lock);
        printf("\r\n QFLASH : debug : device is busy after Sector Erase.");
        return -1;
    }
    Lock_MutexPost(pgd25q16c_Lock);
    return 0;
}

//=====================================================================
//��������оƬ����
//������
//����ֵ��0���ɹ�;-1��ʧ��
//���ܣ�����ȫ������
//=====================================================================
s32 gd25q16c_Erase_Chip(void)
{
    u8 status = 0;
    u32 time = 0;
    u8 sndbuf[1];
    Lock_MutexPend(pgd25q16c_Lock,CN_TIMEOUT_FOREVER);
    if(gd25q16c_WaitReady(200000) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Chip Erase.");
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    if(gd25q16c_Write_Enable() != 0)
    {
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    sndbuf[0] = gd25q16c_ChipErase;

    gd25q16c_CsActive();

    if(gd25q16c_TxRx(sndbuf,1,NULL,0,0) == -1)
    {
        gd25q16c_CsInActive();
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    gd25q16c_CsInActive();

    printf("\r\n gd25q16c Erase, Please Wait.");
    do
    {
        status = gd25q16c_ReadSR(StatusReg1);
        DJY_EventDelay(100);
        if(time++ > 1000)
        {
            printf(".");
            time = 0;
        }
    }while((status & 0x01) == 0x01);

    printf("\r\ngd25q16c Erase Chip Succeed\r\n");
    Lock_MutexPost(pgd25q16c_Lock);
    return 0;
}



//=====================================================================
//��������дһҳ
//���������ͻ�������ҳ��
//����ֵ��0 ���ɹ���-1��ʧ��
//���ܣ�
//=====================================================================
s32 gd25q16c_WritePage(u8* pBuffer,u32 PageNum)
{
    u8 sndbuf[4 + 256];
    u32 WriteAddr;
    Lock_MutexPend(pgd25q16c_Lock,CN_TIMEOUT_FOREVER);

    WriteAddr = PageNum * gd25q16c_PageSize;

    if(gd25q16c_WaitReady(200000) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Page Write.");
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    if(gd25q16c_Write_Enable() != 0)                  //дʹ��
    {
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    sndbuf[0] = gd25q16c_PageProgram;
    sndbuf[3] = WriteAddr & 0xff;
    sndbuf[2] = (WriteAddr >> 8) & 0xff;
    sndbuf[1] = (WriteAddr >> 16) & 0xff;
    memcpy(sndbuf + 4, pBuffer, gd25q16c_PageSize);

    gd25q16c_CsActive();

    if(gd25q16c_TxRx(sndbuf, 4 + 256 ,NULL,0,0) == -1)
    {
        gd25q16c_CsInActive();
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    gd25q16c_CsInActive();

//    DJY_EventDelay(4000);// ��ʱ�г�. 4ms������ȥ��

    Lock_MutexPost(pgd25q16c_Lock);
    return 0;

}

//=====================================================================
//����������ĳһ��ַ��ʼд
//���������ͻ�������д��ַ�����ݳ���
//����ֵ�� 0:�ɹ���-1��ʧ��
//���ܣ���ַ���Ǵ�QFLASH�ĵ�N���ֽڿ�ʼ
//=====================================================================
s32 gd25q16c_WriteMemory(u8* pBuffer,u32 WriteAddr,u32 NumByteToWrite)
{
    u8 command[4];
    u8 *SendBuf;

    Lock_MutexPend(pgd25q16c_Lock,CN_TIMEOUT_FOREVER);

    if(gd25q16c_WaitReady(200000) == false)
    {
        printf("\r\n QFLASH : debug : device is busy before Memory Write.");
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    if(gd25q16c_Write_Enable() != 0)                  //дʹ��
    {
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    SendBuf = (u8*) malloc(4 + NumByteToWrite);

    command[0] = gd25q16c_PageProgram;
    command[3] = WriteAddr & 0xff;
    command[2] = (WriteAddr >> 8) & 0xff;
    command[1] = (WriteAddr >> 16) & 0xff;

    memcpy(SendBuf, command, 4);
    memcpy(SendBuf + 4, pBuffer, NumByteToWrite);

    gd25q16c_CsActive();

    if(gd25q16c_TxRx(SendBuf, 4 + NumByteToWrite ,NULL,0,0) == -1)
    {
        gd25q16c_CsInActive();
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }
    gd25q16c_CsInActive();

//    DJY_EventDelay(4000);// ��ʱ�г�. 4ms������ȥ��

    Lock_MutexPost(pgd25q16c_Lock);
    free(SendBuf);
    return 0;

}

//=====================================================================
//����������һҳ
//���������ջ�������ҳ��
//����ֵ�� 0:�ɹ���-1��ʧ��
//���ܣ�
//=====================================================================
s32 gd25q16c_ReadPage(u8* pBuffer,u32 PageNum)
{
    u32 ReadAddr;
    u8 sndbuf[4];
    Lock_MutexPend(pgd25q16c_Lock,CN_TIMEOUT_FOREVER);
    ReadAddr = PageNum * gd25q16c_PageSize;

    sndbuf[0] = gd25q16c_ReadData;
    sndbuf[3] = ReadAddr & 0xff;
    sndbuf[2] = (ReadAddr >> 8) & 0xff;
    sndbuf[1] = (ReadAddr >> 16) & 0xff;

    if(gd25q16c_WaitReady(200000) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Page Read.");
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    gd25q16c_CsActive();

    if(gd25q16c_TxRx(sndbuf,4,pBuffer,256,4) == -1)
    {
        gd25q16c_CsInActive();
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    gd25q16c_CsInActive();

    Lock_MutexPost(pgd25q16c_Lock);
    return 0;
}

//=====================================================================
//����������ĳһ��ַ��ʼ��
//���������ջ���������ʼ��ȡ��ַ����ȡ����
//����ֵ�� 0:�ɹ���-1��ʧ��
//���ܣ�
//=====================================================================
s32 gd25q16c_ReadMemory(u8* pBuffer,u32 ReadAddr,u32 NumByteToRead)
{
    u8 sndbuf[4];
    Lock_MutexPend(pgd25q16c_Lock,CN_TIMEOUT_FOREVER);

    sndbuf[0] = gd25q16c_ReadData;
    sndbuf[3] = ReadAddr & 0xff;
    sndbuf[2] = (ReadAddr >> 8) & 0xff;
    sndbuf[1] = (ReadAddr >> 16) & 0xff;

    if(gd25q16c_WaitReady(200000) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Memory Read.");
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    gd25q16c_CsActive();

    if(gd25q16c_TxRx(sndbuf,4,pBuffer,NumByteToRead,4) == -1)
    {
        gd25q16c_CsInActive();
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    gd25q16c_CsInActive();

    Lock_MutexPost(pgd25q16c_Lock);
    return 0;

#if 0
    Lock_MutexPend(pgd25q16c_Lock,CN_TIMEOUT_FOREVER);
    if(QSPI_Send_CMD(gd25q16c_FastReadData,ReadAddr,8,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_1_LINE) != 0)
    {
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }
    //QPI,���ٶ�����,��ַΪReadAddr,1�ߴ�������_24λ��ַ_1�ߴ����ַ_1�ߴ���ָ��,8������,NumByteToRead������
    if(QSPI_Receive(pBuffer,NumByteToRead) != 0)
    {
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }
    Lock_MutexPost(pgd25q16c_Lock);
    return 0;
#endif
}

//=====================================================================
//����������оƬID
//������
//����ֵ��0:�ɹ���-1��ʧ��
//���ܣ�
//=====================================================================
u16 gd25q16c_ReadID(void)
{
    u8 sndbuf[4];
    u8 rcvbuf[2];
    u16 deviceid;

    sndbuf[0]=gd25q16c_ManufactDeviceID;
    sndbuf[1]=0x00;
    sndbuf[2]=0x00;
    sndbuf[3]=0x00;

    if(gd25q16c_WaitReady(200000) == false)
    {
        printf("\r\n QFLASH : debug : device is busy before Read ID.");
        return -1;
    }

    gd25q16c_CsActive();

    if(gd25q16c_TxRx(sndbuf,4,rcvbuf,2,4) == -1)
    {
        gd25q16c_CsInActive();
        return -1;
    }


    gd25q16c_CsInActive();

    deviceid=(rcvbuf[0]<<8)|rcvbuf[1];
    return deviceid;
}

//=====================================================================
//��������ʹ������ģʽ
//������
//����ֵ��
//���ܣ�
//=====================================================================
void gd25q16c_Qspi_Enable(void)
{
    u8 sr1,sr2;
    u8 reg_w_buf[2];

    sr2=gd25q16c_ReadSR(StatusReg2);

    if((sr2 & 0X02)==0)           //QEλδʹ��
    {
        gd25q16c_Write_Enable();   //дʹ��
        if(gd25q16c_WriteEnableWait() == -1)
        {
            printf("\r\n gd25q16c Write Enable fail.\r\n");
        }
        sr1=gd25q16c_ReadSR(StatusReg1);       //�ȶ������üĴ���1��ԭʼֵ
        reg_w_buf[0]=sr1 ;
        sr2 |= 1<<1;              //ʹ��QEλ
        reg_w_buf[1]=sr2 ;

        gd25q16c_Write_SR1_SR2(reg_w_buf); //д״̬�Ĵ���
    }
    return;
}

// ============================================================================
// ���ܣ���gd25q16c
// ������
// ���أ�0 -- �ɹ��� -1 -- ʧ��
// ��ע��
// ============================================================================
s32 __gd25q16c_read(u32 unit, void *data, struct uopt opt)
{
    return gd25q16c_ReadPage((u8*)data,(u32)unit);
}

// ============================================================================
// ���ܣ�дgd25q16c
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 __gd25q16c_write(u32 unit, void *data, struct uopt opt)
{
    return gd25q16c_WritePage((u8*)data,(u32)unit);
}

// ============================================================================
// ���ܣ�����gd25q16c��ҳ������
// ������unit -- ҳ����������
//      sz -- ҳ����������
// ���أ�
// ��ע��
// ============================================================================
s32 __gd25q16c_erase(u32 unit, struct uesz sz)
{
    u32 Sector;

    if(sz.unit)
        Sector = unit / qflashdescription->PagesPerSector; // ������һҳ���ĸ��飨ʵ��оƬ����������
    else if (sz.block)
        Sector = (u32)unit;

    return gd25q16c_Erase_Sector(Sector);
}

// ============================================================================
// ���ܣ�QFLASH ����
// ������ucmd -- ���
//      ���� -- ���������
// ���أ�
// ��ע��
// ============================================================================
s32 __gd25q16c_req(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case whichblock://��ҳ�ڵڼ�����
        {
            va_list list;
            u32 *block;
            s64 *unit;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);
            *block = *unit / qflashdescription->PagesPerBlock;
            break;
        }

        case totalblocks://��ö������ע����˵����˼��һ����
        {
            // args = &blocks
            *((u32*)args) =  qflashdescription->SectorNum;
            break;
        }

        case blockunits:
        {
            // args = &units
            *((u32*)args) = qflashdescription->PagesPerSector;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = qflashdescription->BytesPerPage;
            break;
        }

        case format:
        {
            va_list list;
            u32 start, end;
            struct uesz *sz;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(!sz->block)
                return (-1);

            if(-1 == (s32)end)
                end = qflashdescription->BlockNum;
            else if (start)
                end += start;

            do
            {
                if(__gd25q16c_erase((s64)--end, *sz))
                {
                    res = -1;
                    break;
                }
            }
            while(end!=start);

            break;
        }

        case checkbad: break; // ��黵��
        default: res = -1; break;
    }

    return (res);
}

// =============================================================================
// ���ܣ���ʼ��QSPI FLASHģ�飬У��оƬID�Ƿ���ȷ
// ��������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע�������߼������ļ�ϵͳ��ֱ�ӷ����߼��������÷�����
// =============================================================================
s32 ModuleInstall_gd25q16c(void)
{
    u16 temp;

    pgd25q16c_Lock = Lock_MutexCreate("gd25q16c Lock");
    if(!pgd25q16c_Lock)
    {
        printf("\r\nMODULE INSTALL : error : cannot create gd25q16c flash lock.");
        return false;
    }

    temp=gd25q16c_ReadID();        //��ȡFLASH ID.Ŀǰֻ�����жϷ�ʽ����initPrj.c�ļ��г�ʼ���жϻ�û����

    if(temp==gd25q16c_TYPE)
   {
        printf("gd25q16c Read ID Success,ID:%x\r\n",gd25q16c_TYPE);
    }
    else
    {
       printf("gd25q16c Read ID Error,True ID:%x",gd25q16c_TYPE);
       printf("    Read ID:%x\r\n",temp);
       return false;
    }

    if(!qflashdescription)
    {
        qflashdescription = malloc(sizeof(struct NorDescr));
        if(!qflashdescription)
        {
            printf("\r\n: erro : device : memory out.\r\n");
            return false;
        }
        memset(qflashdescription, 0x0, (sizeof(struct NorDescr)));
        qflashdescription->PortType = NOR_SPI;
        qflashdescription->Port = NULL;
        qflashdescription->BytesPerPage = gd25q16c_PageSize;
        qflashdescription->SectorNum = 512;
        qflashdescription->BlockNum = 32;
        qflashdescription->BlocksPerSector = 0;
        qflashdescription->PagesPerSector = 16;
        qflashdescription->SectorsPerBlock = 16;
        qflashdescription->PagesPerBlock = 256;
        qflashdescription->ReservedBlks = 0;
    }

    if(CFG_GD25_PART_FORMAT)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        if(-1 == __gd25q16c_req(format, 0 , -1, &sz))
        {
            warning_printf("at45"," Format failure.");
        }
    }

    gd25_umedia = malloc(sizeof(struct umedia)+qflashdescription->BytesPerPage);
    if(!gd25_umedia)
        return false;

    gd25_umedia->mreq = __gd25q16c_req;
    gd25_umedia->type = nor;
    gd25_umedia->ubuf = (u8*)gd25_umedia + sizeof(struct umedia);

    if(!Device_Create((const char*)flash_name, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)gd25_umedia)))
    {
        printf("\r\n: erro : device : %s addition failed.", flash_name);
        free(gd25_umedia);
        return false;
    }

    gd25q16c_Init_Flag = TRUE;
    return TRUE;
}

// =============================================================================
// ���ܣ��ж�gd25�Ƿ�װ
// ������  ��
// ���أ��ѳɹ���װ��true����δ�ɹ���װ��false����
// ��ע��
// =============================================================================
bool_t Gd25_is_install(void)
{
    return gd25q16c_Init_Flag;
}

// ============================================================================
// ���ܣ���ʼ��SPI FLASHģ�飬�����ļ�ϵͳ
// ������   fs -- ��ý����Ҫ��װ�ļ�ϵͳmount�����֣�
//      mediadrv -- ý������
//      dwStart -- ��ʼ�飻
//      dwEnd -- ��������������ʱ�������ÿ飬ֻ�����ÿ����һ�飩��
// ���أ��ɹ���ʼ����0������ʼ��ʧ�ܣ�-1����
// ��ע�������߼������ļ�ϵͳ��ֱ�ӷ����߼��������÷�����
// ============================================================================
s32 __GD25_FsInstallInit(const char *fs, s32 dwStart, s32 dwEnd, void *mediadrv)
{
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res,BlockNum;

    if(mediadrv == NULL)
        return -1;

    targetobj = OBJ_MatchPath(fs, &notfind);    //����mount��������mount���obj
    if(notfind)
    {
        error_printf("at45"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj); //��ȡobj��˽������
    super->MediaInfo = gd25_umedia;
    super->MediaDrv = mediadrv;

    if((s32)dwEnd == -1)
    {
        dwEnd = qflashdescription->SectorNum;
        BlockNum = dwEnd - dwStart;     //��ȡ�ļ�ϵͳһ�����˶��ٿ�
    }
    else
    {
        BlockNum = dwEnd - dwStart;
    }
    super->AreaSize = BlockNum * (qflashdescription->BytesPerPage * qflashdescription->PagesPerSector);
    super->MediaStart = dwStart * qflashdescription->PagesPerSector; // ��ʼunit��

    res = strlen(flash_name) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,flash_name);      //��ȡ�豸��ȫ·��
    File_BeMedia(FullPath,fs);     //�����豸�����ļ�ϵͳ
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, dwStart, dwEnd);
    return (0);
}

