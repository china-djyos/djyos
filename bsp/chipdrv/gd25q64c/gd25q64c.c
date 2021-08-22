// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��gd25q64c.c
// ģ������: Qflash��ʹ��QSPIͨ��
// ģ��汾: V1.00
// ������Ա: wangxy
// ����ʱ��: 2018.08.30
// =============================================================================


#include "gd25q64c.h"

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
#include "shell.h"
#include <device/unit_media.h>
#include <device.h>
#include <djyfs/filesystems.h>
#include <include/spi_pub.h>
#include "dbug.h"
#include "cpu_peri_spi.h"

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_Gd25q64c(void);
//    ModuleInstall_Gd25q64c();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"nor flash gd25q64c"//spi�ӿڵ�norflash
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
#warning   gd25q64c�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,-1,1024,
//%$#@enum,true,false,
#define CFG_GD25_PART_FORMAT               false      //����ѡ��,�Ƿ���Ҫ������оƬ��
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


//*****************************************************************//
//QFLASH˵����Ŀǰ�����£���оƬ��flash����8M��һ������16ҳ��һҳ256�ֽ����ݣ�һ����16������һ��128����
//�ڶ�д����ǰ�����оƬ�ֲ���ÿ��ָ���ʱ��ͼʹ��QSPI_Send_CMD�������ã����磺
//QPI,���߶�����,��ַΪReadAddr,1�ߴ���ָ��_1�ߴ���24λ��ַ_8������_4�ߴ�������,NumByteToRead������
//���ʹ��QSPI_Receive��QSPI_Transmit
//*****************************************************************//


//struct SemaphoreLCB pgd25q64c_Lock;   //оƬ������ʱ���
struct MutexLCB *pgd25q64c_Lock;           //оƬ������ʱ���
bool_t gd25q64c_Init_Flag=false;
u16 gd25q64c_type          = 0xc816;
u8 gd25q64c_MODE       = 0;            //QSPIģʽ��־:0,SPIģʽ;1,QPIģʽ.

//struct FlashDescrible
//{
//    u32 gd25q64c_BlockrNum        = 128;          //һ��128��
//    u32 gd25q64c_SectorsPerBlock  = 16;          //һ����16����
//    u32 gd25q64c_SectorNum        = 2048;          //һ��2048������
//    u32 gd25q64c_SectorSize       = 4096;          //������С4096�ֽ�
//    u32 gd25q64c_PagesPerSector   = 16;          //һ������16ҳ
//    u32 gd25q64c_PageSize         = 256;          //һҳ256�ֽ�����
//};

u32 gd25q64c_BlockrNum        = 64;          //һ��128��
u32 gd25q64c_SectorsPerBlock  = 16;          //һ����16����
u32 gd25q64c_SectorNum        = 1024;          //һ��2048������
u32 gd25q64c_SectorSize       = 4096;          //������С4096�ֽ�
u32 gd25q64c_PagesPerSector   = 16;          //һ������16ҳ
u32 gd25q64c_PageSize         = 256;          //һҳ256�ֽ�����

u32 gd25q64c_OP_TIMEOUT= 1800000; // 30����
static char *flash_name = "gd25q64c";
struct NorDescr *gd25q64_des = NULL;
struct umedia *gd25_umedia;
extern struct Object *s_ptDeviceRoot;

bool_t Gd25q64c_Write_Enable(void);
bool_t Gd25q64c_WaitReady(u32 timeout);
//=====================================================================
//�������� Ƭѡ����,��board�ж���
//������
//����ֵ��
//���ܣ�Ƭѡ���ͣ�ѡ������
//=====================================================================
__attribute__((weak)) void Gd25q64c_CsActive(void)
{
//    u32 param;
//    param = 0x2;
//    spi_ctrl(CMD_SPI_SET_NSSID, (void *)&param);
    printf("board undefined gd25q64c_CsActive \r\n");
}
//=====================================================================
//��������Ƭѡ���ߣ���board�ж���
//������
//����ֵ��
//���ܣ�Ƭѡ���ߣ���������
//=====================================================================
__attribute__((weak)) void Gd25q64c_CsInactive(void)
{
//    u32 param;
//    param = 0x3;
//    spi_ctrl(CMD_SPI_SET_NSSID, (void *)&param);
    printf("board undefined Gd25q64c_CsInactive \r\n");
}

//=====================================================================
//����������ʼ���õ���IO����board�ж���
//������
//����ֵ��
//���ܣ���ʼ��Ƭѡ
//=====================================================================
__attribute__((weak)) void Gd25q64c_GpioInit(void)
{
    printf("board undefined Gd25q64c_GpioInit \r\n");
//    bk_gpio_config_output(GPIO36);
//    bk_gpio_output(GPIO36, GPIO_INT_LEVEL_LOW);
}


//=====================================================================
//��������SPI���ͽ��պ���
//������sdata,�����͵����ݵ�ַ��
//      slen�����ݳ��ȡ�
//      rdata�����ܵ����ݵ�ַ��
//      rlen���������ݳ��ȡ�
//      RecvOff������ƫ���������Ͷ��ٸ���֮���ٿ�ʼ��һ����һ����ǰ���ǿն���
//����ֵ��true���ɹ� �� false:ʧ��
//���ܣ�SPI���ͽ���
//=====================================================================
static bool_t Gd25q64c_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen,u32 RecvOff)
{
    if(SPI_TxRx(sdata, slen, rdata, rlen, RecvOff) == false)
        return false;
    return true;
}

//=====================================================================
//����������״̬�Ĵ���ָ��
//������regno�������״̬�Ĵ������� gd25q64c_ReadStatusReg1
//����ֵ��true���ɹ� �� falseʧ��
//���ܣ�
//=====================================================================
bool_t Gd25q64c_ReadSR(u8 regno, u8 *sta)
{
    u8 command;
//    u8 byte;
    switch(regno)
    {
        case StatusReg1:
            command = gd25q64c_ReadStatusReg1;           //��״̬�Ĵ���1ָ��
            break;
        case StatusReg2:
            command = gd25q64c_ReadStatusReg2;           //��״̬�Ĵ���2ָ��
            break;
        case StatusReg3:
            command = gd25q64c_ReadStatusReg3;           //��״̬�Ĵ���3ָ��
            break;
        default:
            command = gd25q64c_ReadStatusReg1;
            break;
    }

    Gd25q64c_CsActive();
    if(Gd25q64c_TxRx(&command,1,sta,1,1) == false)
    {
        Gd25q64c_CsInactive();
        return false;
    }
    Gd25q64c_CsInactive();
    return true;
//    return byte;
}

//=====================================================================
//��������д״̬�Ĵ���ָ��
//������regno -- �Ĵ����ţ�data -- д������
//����ֵ��true���ɹ� �� falseʧ��
//���ܣ�
//=====================================================================
bool_t Gd25q64c_WriteSR(u8 regno, u8 data)
{
    u8 command=0;
    u8 sndbuf[2];
    switch(regno)
    {
        case 1:
            command=gd25q64c_WriteRegister1;    //д״̬�Ĵ���1
            break;
        case 2:
            command=gd25q64c_WriteRegister2;    //д״̬�Ĵ���2
            break;
        case 3:
            command=gd25q64c_WriteRegister3;    //д״̬�Ĵ���3
            break;
        default:
            command=gd25q64c_WriteRegister1;
            break;
    }

    if(Gd25q64c_WaitReady(80000*mS) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Write Reg.");
        return -1;
    }

    if(Gd25q64c_Write_Enable() != true)                  //дʹ��
    {
        return -1;
    }

    sndbuf[0] = command;
    sndbuf[1] = data;

    Gd25q64c_CsActive();

    if(Gd25q64c_TxRx(sndbuf,2,NULL,0,0) == false)
    {
        Gd25q64c_CsInactive();
        return -1;
    }

    Gd25q64c_CsInactive();

    return 0;
}

////=====================================================================
////��������дʹ�ܵȴ�
////������
////����ֵ��true���ɹ� �� false��ʧ��
////���ܣ�
////=====================================================================
//bool_t Gd25q64c_WriteEnableWait(u32 timeout)
//{
//    u8 sr1=1;
//    u32 timeoutcnt=0;
//
//    if(Gd25q64c_ReadSR(StatusReg1, &sr1) == false)
//        return false;
//    while(1)   // �ȴ�дʹ��λ��λ
//    {
//        if(Gd25q64c_ReadSR(StatusReg1, &sr1) == false)
//            return false;
//        if( (sr1 & 0x02) == 0x02)
//            return true;
//        DJY_EventDelay(10*mS);
//        timeoutcnt += 10*mS;
//        if(timeoutcnt >= timeout)
//        {
//            return false;
//        }
//    }
//    return true;
//}
//
//
////=====================================================================
////��������дʧ�ܵȴ�
////������
////����ֵ��true���ɹ� �� false��ʧ��
////���ܣ�
////=====================================================================
//bool_t Gd25q64c_WriteDisableWait(u32 timeout)
//{
//    u8 sr1=1;
//    u32 timeoutcnt=0;
//
//    if(Gd25q64c_ReadSR(StatusReg1, &sr1) == false)
//        return false;
//    while(1)   // �ȴ�BUSYλ���
//    {
//        if(Gd25q64c_ReadSR(StatusReg1, &sr1) == false)
//            return false;
//        if( (sr1 & 0x02) != 0x02)
//            return true;
//        DJY_EventDelay(10*mS);
//        timeoutcnt += 10*mS;
//        if(timeoutcnt >= timeout)
//        {
//            return false;
//        }
//    }
//    return true;
//}

//=====================================================================
//��������дʹ��
//������
//����ֵ��true���ɹ� �� false��ʧ��
//���ܣ�����QFLASH��д����ǰ��Ҫ��ʹ��
//=====================================================================
bool_t Gd25q64c_Write_Enable(void)
{
    u8 sndbuf[1];
    u8 sr1=1;
    u32 timeoutcnt=0;

    sndbuf[0] = gd25q64c_WriteEnable;

    while(1)   // �ȴ�дʹ��λ��λ
    {
        Gd25q64c_CsActive();

        Gd25q64c_TxRx(sndbuf,1,NULL,0,0);

        Gd25q64c_CsInactive();
        if(Gd25q64c_ReadSR(StatusReg1, &sr1) == false)
            return false;
        if( (sr1 & 0x02) == 0x02)
            return true;
        DJY_EventDelay(10*mS);
        timeoutcnt += 10*mS;
        if(timeoutcnt >= 80000*mS)
        {
            printf("\r\n gd25q64c Write Enable fail.\r\n");
            return false;
        }
    }
    return true;

}


//=====================================================================
//��������дʧ��
//������
//����ֵ��true���ɹ� �� false��ʧ��
//���ܣ�
//=====================================================================
bool_t Gd25q64c_Write_Disable(void)

{
    u8 sndbuf[1];
    u8 sr1=1;
    u32 timeoutcnt=0;

    sndbuf[0] = gd25q64c_WriteDisable;

    while(1)   // �ȴ�дʹ��λ���
    {
        Gd25q64c_CsActive();

        Gd25q64c_TxRx(sndbuf,1,NULL,0,0);

        Gd25q64c_CsInactive();
        if(Gd25q64c_ReadSR(StatusReg1, &sr1) == false)
            return false;
        if( (sr1 & 0x02) != 0x02)
            return true;
        DJY_EventDelay(10*mS);
        timeoutcnt += 10*mS;
        if(timeoutcnt >= 80000*mS)
        {
            printf("\r\n gd25q64c Write Enable fail.\r\n");
            return false;
        }
    }
    return true;

}
//=====================================================================
//���������ȴ�QFLASH��busyλ���
//������
//����ֵ��true ���ɹ���false ��ʧ��
//���ܣ�
//=====================================================================
bool_t Gd25q64c_WaitReady(u32 timeout)
{
    u8 sr1=1;
    u32 timeoutcnt=0;

    if(Gd25q64c_ReadSR(StatusReg1, &sr1) == false)
        return false;
    while(1)   // �ȴ�BUSYλ���
    {
        if(Gd25q64c_ReadSR(StatusReg1, &sr1) == false)
            return false;
        if( (sr1 & 0x01) != 0x01)
            return true;
        DJY_EventDelay(10*mS);
        timeoutcnt += 10*mS;
        if(timeoutcnt >= timeout)
        {
            return false;
        }
    }
    return true;
}

//=====================================================================
//����������������
//������������
//����ֵ��true���ɹ� �� false��ʧ��
//���ܣ�
//=====================================================================
bool_t Gd25q64c_Erase_Sector(u32 SectorNum)
{
    u32 Dst_Addr;
    u8 sndbuf[4];
    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);

    if(Gd25q64c_WaitReady(80000*mS) == false)    //��ʱ����������ʱ��
    {
        Lock_MutexPost(pgd25q64c_Lock);
        printf("\r\n QFLASH : debug : device is busy before Sector Erase.");
        return false;
    }

    if(Gd25q64c_Write_Enable() != true)
    {
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Dst_Addr = SectorNum * gd25q64c_SectorSize;

    sndbuf[0] = gd25q64c_SectorErase;
    sndbuf[3] = Dst_Addr & 0xff;
    sndbuf[2] = (Dst_Addr >> 8) & 0xff;
    sndbuf[1] = (Dst_Addr >> 16) & 0xff;

    Gd25q64c_CsActive();

    if(Gd25q64c_TxRx(sndbuf,4,NULL,0,0) == false)
    {
        Gd25q64c_CsInactive();
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Gd25q64c_CsInactive();


//  Gd25q64c_WaitReady(200*mS);    //��������ʱ����200mS
    Lock_MutexPost(pgd25q64c_Lock);
    return true;
}

//=====================================================================
//�������������
//���������
//����ֵ��true:�ɹ���false��ʧ��
//���ܣ�
//=====================================================================
bool_t Gd25q64c_Erase_Block(u32 BlockNum)
{
    u32 Dst_Addr;
    u8 sndbuf[4];
    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);

    if(Gd25q64c_WaitReady(80000*mS) == false)    //��ʱ����������ʱ��
    {
        Lock_MutexPost(pgd25q64c_Lock);
        printf("\r\n QFLASH : debug : device is busy before Sector Erase.");
        return false;
    }

    if(Gd25q64c_Write_Enable() != true)
    {
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Dst_Addr = BlockNum * gd25q64_des->SectorsPerBlock * gd25q64c_SectorSize;

    sndbuf[0] = gd25q64c_BlockErase;
    sndbuf[3] = Dst_Addr & 0xff;
    sndbuf[2] = (Dst_Addr >> 8) & 0xff;
    sndbuf[1] = (Dst_Addr >> 16) & 0xff;

    Gd25q64c_CsActive();

    if(Gd25q64c_TxRx(sndbuf,4,NULL,0,0) == false)
    {
        Gd25q64c_CsInactive();
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Gd25q64c_CsInactive();


//  Gd25q64c_WaitReady(2000*mS);   //32K block����ʱ����1S��64K����1.5S
    Lock_MutexPost(pgd25q64c_Lock);
    return true;
}

//=====================================================================
//��������оƬ����
//������
//����ֵ��true:�ɹ���false��ʧ��
//���ܣ�����ȫ������
//=====================================================================
bool_t Gd25q64c_Erase_Chip(void)
{
    u8 status = 0;
    u32 time = 0;
    u8 sndbuf[1];
    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);

    if(Gd25q64c_WaitReady(80000*mS) == false)   //4Mbytes��оƬ����ʱ�����80S
    {
        printf("\r\n FLASH : debug : device is busy before Chip Erase.");
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    if(Gd25q64c_Write_Enable() != true)
    {
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    sndbuf[0] = gd25q64c_ChipErase;

    Gd25q64c_CsActive();

    if(Gd25q64c_TxRx(sndbuf,1,NULL,0,0) == false)
    {
        Gd25q64c_CsInactive();
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Gd25q64c_CsInactive();

    printf("\r\n gd25q64c Erase, Please Wait.");
    Gd25q64c_WaitReady(80000*mS);   //4Mbytes��оƬ����ʱ�����80S

    printf("\r\ngd25q64c Erase Chip Succeed\r\n");
    Lock_MutexPost(pgd25q64c_Lock);
    return true;
}



//=====================================================================
//��������дһҳ��д֮ǰȷ���������
//������pBuffer����д�������
//      PageNum��ҳ��
//����ֵ��true -- �ɹ��� false -- ʧ��
//���ܣ�
//=====================================================================
bool_t Gd25q64c_WritePage(u8* pBuffer,u32 PageNum)
{
    u8 sndbuf[4 + 256];
    u32 WriteAddr;
    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);

    WriteAddr = PageNum * gd25q64c_PageSize;

    if(Gd25q64c_WaitReady(80000*mS) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Page Write.");
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    if(Gd25q64c_Write_Enable() != true)                  //дʹ��
    {
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    sndbuf[0] = gd25q64c_PageProgram;
    sndbuf[3] = WriteAddr & 0xff;
    sndbuf[2] = (WriteAddr >> 8) & 0xff;
    sndbuf[1] = (WriteAddr >> 16) & 0xff;
    memcpy(sndbuf + 4, pBuffer, gd25q64c_PageSize);

    Gd25q64c_CsActive();

    if(Gd25q64c_TxRx(sndbuf, 4 + 256 ,NULL,0,0) == false)
    {
        Gd25q64c_CsInactive();
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Gd25q64c_CsInactive();

    Lock_MutexPost(pgd25q64c_Lock);
    return true;

}

//=====================================================================
//�������������������ܵ�д��д֮ǰ��Ҫȷ����д�ĵ�ַ��Χ�ڵ�����ȫΪ0xff�������д��ʧ��
//������pBuffer����д�������
//      WriteAddr��д���ַ
//      NumByteToWrite�����ݳ���
//����ֵ��true:�ɹ���false��ʧ��
//���ܣ���ַ���Ǵ�QFLASH�ĵ�N���ֽڿ�ʼ
//=====================================================================
bool_t Gd25q64c_WriteNoErase(u8* pBuffer,u32 WriteAddr,u32 NumByteToWrite)
{
    u32 remain = 0, page_num = 0;
    bool_t ret = false;
    u8 write_buf[256];
    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);
    page_num = WriteAddr / 256;
    remain = 256 - WriteAddr % 256;  //��ҳ��ʣ����ֽ���
    if(NumByteToWrite <= remain)
        remain = NumByteToWrite;
    while(1)
    {
        if(remain == 256)
        {
            if(Gd25q64c_WritePage(pBuffer, page_num))
            {
                if(remain == NumByteToWrite)
                {
                    ret = true;
                    break;
                }
                else
                {
                    pBuffer += remain;
                    page_num ++;
                    NumByteToWrite -= remain;
                    if(NumByteToWrite > 256)
                        remain = 256;
                    else
                        remain = NumByteToWrite;
                }
            }
            else
                break;
        }
        else
        {
            memset(write_buf, 0xff, sizeof(write_buf));
            memcpy(write_buf, pBuffer, remain);
            if(Gd25q64c_WritePage(write_buf, page_num))
            {
                ret = true;
                break;
            }
            else
                break;
        }


    }
    Lock_MutexPost(pgd25q64c_Lock);
    return ret;

}

//=====================================================================
//����������ĳһ��ַ��ʼд,дǰ���жϴ�д��������Ƿ���ڰ�ԭ�����ݴ�0��Ϊ1�ı��أ�
//      �������Ȳ���������������ֱ��д��
//���������ͻ�������д��ַ�����ݳ���
//����ֵ�� true:�ɹ���false��ʧ��
//���ܣ�
//=====================================================================
u8 Gd25Q64C_BUFFER[4096];
bool_t Gd25q64c_Write(u8* pBuffer,u32 WriteAddr,u32 len)
{
    u32 sec;
    u16 sec_off;
    u16 sec_remain;
    u16 i;
    u8 data;
    bool_t ret = true,temp;
    u32 t,t1,t2;

    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);
    sec = WriteAddr / gd25q64c_SectorSize;          //������ַ
    sec_off = WriteAddr % gd25q64c_SectorSize;      //����ƫ��
    sec_remain = gd25q64c_SectorSize- sec_off;      //����ʣ���С

    if(len <= sec_remain)
        sec_remain = len;

    while(1)
    {
//        extern u32 timesectorread,timesectorerase,timesectorwrite;       //toto,lst dbg
//        t = DJY_GetSysTime();
        temp = Gd25q64c_Read(Gd25Q64C_BUFFER, sec * gd25q64c_SectorSize, gd25q64c_SectorSize);
//        t1 = DJY_GetSysTime();
//        timesectorread += gd25q64c_SectorSize;
        if(temp)
        {
            for(i=0; i < sec_remain; i++)
            {
                data = Gd25Q64C_BUFFER[sec_off + i];
                if((data | pBuffer[i]) != data)     //����Ƿ��д�0��Ϊ1��bit
                    break;
            }
            if(i < sec_remain)    //��Ҫ����
            {
                temp = Gd25q64c_Erase_Sector(sec);
//                t = DJY_GetSysTime();
//                timesectorerase ++;
                if(temp)
                {
                    for(i=0; i<sec_remain; i++)
                    {
                        Gd25Q64C_BUFFER[i + sec_off] = pBuffer[i];
                    }
                    temp = Gd25q64c_WriteNoErase(Gd25Q64C_BUFFER, sec*4096, 4096);
//                    t1 = DJY_GetSysTime();
//                    timesectorwrite += t1 - t;
                    if(temp == false)
                    {
                        ret = false;
                        break;
                    }
                }
                else
                {
                    ret = false;
                    break;
                }

            }
            else
            {
                temp = Gd25q64c_WriteNoErase(pBuffer, WriteAddr, sec_remain);
//                t = DJY_GetSysTime();
//                timesectorwrite +=sec_remain;
                if(temp == false)
                {
                    ret = false;
                    break;
                }
            }
            if(len == sec_remain)
                break;  //д����
            else
            {
                sec++;
                sec_off=0;

                pBuffer += sec_remain;
                WriteAddr += sec_remain;
                len -= sec_remain;
                if(len > 4096)
                    sec_remain=4096;
                else
                    sec_remain=len;
            }
        }
        else
        {
            ret = false;
            break;
        }
    }

    Lock_MutexPost(pgd25q64c_Lock);
    return ret;
}

//=====================================================================
//����������һҳ
//������pBuffer�����ջ�����
//      PageNum��ҳ��
//����ֵ�� true:�ɹ���false��ʧ��
//���ܣ�
//=====================================================================
bool_t Gd25q64c_ReadPage(u8* pBuffer,u32 PageNum)
{
    u32 ReadAddr;
    u8 sndbuf[4];
    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);
    ReadAddr = PageNum * gd25q64c_PageSize;

    sndbuf[0] = gd25q64c_ReadData;
    sndbuf[3] = ReadAddr & 0xff;
    sndbuf[2] = (ReadAddr >> 8) & 0xff;
    sndbuf[1] = (ReadAddr >> 16) & 0xff;

    if(Gd25q64c_WaitReady(80000*mS) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Page Read.");
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Gd25q64c_CsActive();

    if(Gd25q64c_TxRx(sndbuf,4,pBuffer,256,4) == false)
    {
        Gd25q64c_CsInactive();
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Gd25q64c_CsInactive();

    Lock_MutexPost(pgd25q64c_Lock);
    return true;
}

//=====================================================================
//����������ĳһ��ַ��ʼ��
//������PBuffer�����ջ�������
//      ReadAddr����ʼ��ַ��
//      NumByteToRead����ȡ����
//����ֵ��true:�ɹ���false��ʧ��
//���ܣ�
//=====================================================================
bool_t Gd25q64c_Read(u8* pBuffer,u32 ReadAddr,u32 NumByteToRead)
{
    u8 sndbuf[4];

//    u32 t;
//    extern u32 timewait;       //toto,lst dbg
    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);

    sndbuf[0] = gd25q64c_ReadData;
    sndbuf[3] = ReadAddr & 0xff;
    sndbuf[2] = (ReadAddr >> 8) & 0xff;
    sndbuf[1] = (ReadAddr >> 16) & 0xff;

//    t = DJY_GetSysTime();
    if(Gd25q64c_WaitReady(80000*mS) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Memory Read.");
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }
//    timewait += DJY_GetSysTime() - t;
    Gd25q64c_CsActive();

    if(Gd25q64c_TxRx(sndbuf,4,pBuffer,NumByteToRead,4) == false)
    {
        Gd25q64c_CsInactive();
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Gd25q64c_CsInactive();

    Lock_MutexPost(pgd25q64c_Lock);
    return true;

#if 0
    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);
    if(QSPI_Send_CMD(gd25q64c_FastReadData,ReadAddr,8,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_1_LINE) != 0)
    {
        Lock_MutexPost(pgd25q64c_Lock);
        return -1;
    }
    //QPI,���ٶ�����,��ַΪReadAddr,1�ߴ�������_24λ��ַ_1�ߴ����ַ_1�ߴ���ָ��,8������,NumByteToRead������
    if(QSPI_Receive(pBuffer,NumByteToRead) != 0)
    {
        Lock_MutexPost(pgd25q64c_Lock);
        return -1;
    }
    Lock_MutexPost(pgd25q64c_Lock);
    return 0;
#endif
}

//=====================================================================
//����������оƬID
//������
//����ֵ������:�ɹ���-1��ʧ��
//���ܣ�
//=====================================================================
u32 Gd25q64c_ReadID(void)
{
    u8 sndbuf[4];
    u8 rcvbuf[3];
    u32 deviceid;

    sndbuf[0]=gd25q64c_JedecID;
    sndbuf[1]=0x00;
    sndbuf[2]=0x00;
    sndbuf[3]=0x00;

    if(Gd25q64c_WaitReady(80000*mS) == false)
    {
        printf("\r\n QFLASH : debug : device is busy before Read ID.");
        return -1;
    }

    Gd25q64c_CsActive();
//  if(Gd25q64c_TxRx(sndbuf,4,rcvbuf,2,4) == false)
    if(Gd25q64c_TxRx(sndbuf,1,rcvbuf,3,1) == false)
    {
        Gd25q64c_CsInactive();
        return -1;
    }
    Gd25q64c_CsInactive();

    deviceid=(rcvbuf[0]<<16)|(rcvbuf[1]<<8)|(rcvbuf[2]);
    return deviceid;
}

//=====================================================================
//��������ʹ������ģʽ
//������
//����ֵ��true:�ɹ���false��ʧ��
//���ܣ�
//=====================================================================
bool_t Gd25q64c_Qspi_Enable(void)
{
    u8 sr2;

    if(Gd25q64c_ReadSR(StatusReg2, &sr2) == false)
        return false;
    if((sr2 & 0X02)==0)           //QEλδʹ��
    {
        sr2 |= 1<<1;              //ʹ��QEλ
        if(Gd25q64c_WriteSR(StatusReg2, sr2) == false)
            return false;
    }
    return true;
}

// ============================================================================
// ���ܣ�����gd25q64c��ҳ���
// ������unit -- ҳ���߿�ţ�
//      sz -- ҳ����������
// ���أ�
// ��ע��
// ============================================================================
s32 __gd25q64c_erase(u32 unit, struct uesz sz)
{
    u32 block;

    if(sz.unit)
        block = unit / gd25q64_des->PagesPerBlock; // ������һҳ���ĸ��飨�ֲ��е���������
    else if (sz.block)
        block = (u32)unit;

    return Gd25q64c_Erase_Block(block);
}

// ============================================================================
// ���ܣ�QFLASH ����
// ������ucmd -- ���
//      ���� -- ���������
// ���أ�
// ��ע��
// ============================================================================
s32 __gd25q64c_req(enum ucmd cmd, ptu32_t args, ...)
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
            *block = *unit / gd25q64_des->PagesPerBlock;
            break;
        }

        case totalblocks:
        {
            // args = &blocks
            *((u32*)args) =  gd25q64_des->BlockNum;
            break;
        }

        case totalSectors:
        {
            // args = &blocks
            *((u32*)args) =  gd25q64_des->SectorNum;
            break;
        }

        case blockunits:
        {
            // args = &units
            *((u32*)args) = gd25q64_des->PagesPerSector;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = gd25q64_des->BytesPerPage;
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
                end = gd25q64_des->BlockNum;
            else if (start)
                end += start;

            do
            {
                if(__gd25q64c_erase((s64)--end, *sz) == false)
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
s32 ModuleInstall_Gd25q64c(void)
{
    u32 temp;

    pgd25q64c_Lock = Lock_MutexCreate("gd25q64c Lock");
    if(!pgd25q64c_Lock)
    {
        printf("\r\nMODULE INSTALL : error : cannot create gd25q64c flash lock.");
        return false;
    }
    Gd25q64c_GpioInit();
    temp=Gd25q64c_ReadID();

    printf("gd25q64c Read ID Success,ID:%x\r\n",temp);
//  if(temp==gd25q64c_type)
//  {
//      printf("gd25q64c Read ID Success,ID:%x\r\n",gd25q64c_type);
//  }
//  else
//  {
//     printf("gd25q64c Read ID Error,True ID:%x",gd25q64c_type);
//     printf("    Read ID:%x\r\n",temp);
//     return false;
//  }

    if(!gd25q64_des)
    {
        gd25q64_des = malloc(sizeof(struct NorDescr));
        if(!gd25q64_des)
        {
            printf("\r\n: erro : device : memory out.\r\n");
            return false;
        }
        memset(gd25q64_des, 0x0, (sizeof(struct NorDescr)));
        gd25q64_des->PortType = NOR_SPI;
        gd25q64_des->Port = NULL;
        gd25q64_des->BytesPerPage = gd25q64c_PageSize;
        gd25q64_des->SectorNum = gd25q64c_BlockrNum * gd25q64c_SectorsPerBlock;
        gd25q64_des->BlockNum = gd25q64c_BlockrNum;
        gd25q64_des->BlocksPerSector = 0;
        gd25q64_des->PagesPerSector = gd25q64c_PagesPerSector;
        gd25q64_des->SectorsPerBlock = gd25q64c_SectorsPerBlock;
        gd25q64_des->PagesPerBlock = gd25q64c_PagesPerSector * gd25q64c_SectorsPerBlock;
        gd25q64_des->ReservedBlks = 0;
    }

    if(CFG_GD25_PART_FORMAT)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        if(-1 == __gd25q64c_req(format, 0 , -1, &sz))
        {
            warning_printf("gd25"," Format failure.");
        }
    }

    gd25_umedia = malloc(sizeof(struct umedia)+gd25q64_des->BytesPerPage);
    if(!gd25_umedia)
        return false;

    gd25_umedia->mreq = __gd25q64c_req;
    gd25_umedia->type = nor;
    gd25_umedia->ubuf = (u8*)gd25_umedia + sizeof(struct umedia);

    if(!Device_Create((const char*)flash_name, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)gd25_umedia)))
    {
        printf("\r\n: erro : device : %s addition failed.", flash_name);
        free(gd25_umedia);
        return false;
    }

    gd25q64c_Init_Flag = TRUE;
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
    return gd25q64c_Init_Flag;
}

// ============================================================================
// ���ܣ���ʼ��SPI FLASHģ�飬�����ļ�ϵͳ
// ������   fs -- ��ý����Ҫ��װ�ļ�ϵͳmount�����֣�
//      mediadrv -- ý������
//      dwStart -- ��ʼ�飻
//      dwEnd -- ��������������ʱ�������ÿ飬ֻ�����ÿ����һ�飩��
// ���أ��ɹ���ʼ����true������ʼ��ʧ�ܣ�false����
// ��ע�������߼������ļ�ϵͳ��ֱ�ӷ����߼��������÷�����
// ============================================================================
bool_t __GD25_FsInstallInit(const char *fs, s32 dwStart, s32 dwEnd, void *mediadrv)
{
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res,BlockNum;

    if(mediadrv == NULL)
        return false;

    targetobj = OBJ_MatchPath(fs, &notfind);    //����mount��������mount���obj
    if(notfind)
    {
        error_printf("gd25"," not found need to install file system.");
        return false;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj); //��ȡobj��˽������
    super->MediaInfo = gd25_umedia;
    super->MediaDrv = mediadrv;

    if((s32)dwEnd == -1)
    {
        dwEnd = gd25q64_des->BlockNum;
        BlockNum = dwEnd - dwStart;     //��ȡ�ļ�ϵͳһ�����˶��ٿ�
    }
    else
    {
        BlockNum = dwEnd - dwStart;
    }
    super->AreaSize = BlockNum * gd25q64_des->SectorsPerBlock * gd25q64_des->PagesPerSector * gd25q64_des->BytesPerPage ;
    super->MediaStart = dwStart * gd25q64_des->PagesPerBlock; // ��ʼunit��

    res = strlen(flash_name) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,flash_name);      //��ȡ�豸��ȫ·��
    File_BeMedia(FullPath,fs);     //�����豸�����ļ�ϵͳ
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, dwStart, dwEnd);
    return (true);
}

bool_t showpage(char *param)
{
    s32 temp;
    u8 buf[256];
    temp = atoi(param);
    Gd25q64c_ReadPage(buf, temp);
    for(temp = 0;temp<256;temp+=8)
    {
        printf("0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x\r\n", buf[temp], buf[temp + 1], buf[temp + 2], buf[temp + 3], buf[temp + 4], buf[temp + 5], buf[temp + 6], buf[temp + 7]);
    }
    return true;
}

u32  data[256],rd[256];
bool_t test25(char *param)
{
    u32 loop,num;
    s32 temp;
    temp = atoi(param);

    for(num = 0; num < 4096; num++)
    {
        for(loop = 0; loop < 256; loop++)
        {
            data[loop] = num*256+loop+temp;
        }
        Gd25q64c_Write(data, num*1024,1024);
        Gd25q64c_Read(rd, num*1024,1024);
        for(loop = 0; loop < 256; loop++)
        {
            if(data[loop] != rd[loop])
            {
                printf("********w25q test error,n = %d\r\n",num);
                break;
            }
        }
        if(num % 100 == 0)
        {
            printf("-------------writed %d pages\r\n",num);
        }
      DJY_EventDelay(1*1000);
    }
    return true;
}

bool_t testread(char *param)
{
    char *page_addr,*next_param;
    u32 page,num,loop,n;

    page_addr = shell_inputs(param,&next_param);

    page = atoi(page_addr);
    num = atoi(next_param);

    Gd25q64c_Read(data, page*1024,1024);
    for(loop = 0; loop < num; loop++)
    {
        Gd25q64c_Read(rd, page*1024,1024);
        for(n = 0; n < 256; n++)
        {
            if(data[n] != rd[n])
            {
                printf("********w25q test error,n = %d\r\n",loop);
                break;
            }
        }
        if(loop % 100 == 0)
        {
            printf("-------------writed %d pages\r\n",loop);
        }
      DJY_EventDelay(1*1000);
    }
    return true;
}
ADD_TO_ROUTINE_SHELL(erase25, Gd25q64c_Erase_Chip,"����25ϵ��spi norflash");
ADD_TO_ROUTINE_SHELL(showpage, showpage,"��ʾһҳ����");
ADD_TO_ROUTINE_SHELL(test25, test25,"����оƬ");
ADD_TO_ROUTINE_SHELL(testread, testread,"����оƬ");

