// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��gd25q64c.c
// ģ������: Qflash��ʹ��QSPIͨ��
// ģ��汾: V1.00
// ������Ա: wangxy
// ����ʱ��: 2018.08.30
// =============================================================================

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
#include <device/include/unit_media.h>
#include <device.h>
#include <djyfs/filesystems.h>
#include <include/spi_pub.h>
#include "dbug.h"
#include "ips6404l.h"
#include "cpu_peri_spi.h"

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_Ips6404l(void);
//    ModuleInstall_Ips6404l();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"Ips6404l"//SPI��QSPI�ӿڵ�ramоƬ
//parent:"none"//��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none",//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"cpu onchip spi","cpu onchip qspi"        //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_NORFLASH_IPS6404 == false )
//#warning  " SPIRAM_IPS6404  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_NORFLASH_IPS6404    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@string,1,20,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

struct MutexLCB *ips6404l_lock;           //оƬ������ʱ���
u16 ips6404l_type          = 0x0d5d;
//=====================================================================
//�������� Ƭѡ����,��board�ж���
//������  0 -- ���ͣ�1 -- ���ߡ�
//����ֵ��
//���ܣ�Ƭѡ���ͣ�ѡ������
//=====================================================================
__attribute__((weak)) void Ips6404l_Cs(u8 flag)
{
//    gpio_output(GPIO26, flag);
    printf("board undefined Ips6404l_Cs \r\n");
}

//=====================================================================
//����������ʼ���õ���IO����board�ж���
//������
//����ֵ��
//���ܣ���ʼ��Ƭѡ
//=====================================================================
__attribute__((weak)) void Ips6404l_GpioInit(void)
{
    printf("board undefined Ips6404l_GpioInit \r\n");
//    gpio_config(GPIO26, GMODE_OUTPUT);
//    gpio_output(GPIO26, 1);
}


//=====================================================================
//��������SPI���ͽ��պ���
//���������͵����ݵ�ַ�����ݳ��ȡ����ܵ����ݵ�ַ�����ݳ��ȡ�ƫ���������ͼ�����֮���ٿ�ʼ��һ����һ����ǰ���ǿն���
//����ֵ��true���ɹ� �� false:ʧ��
//���ܣ�SPI���ͽ���
//=====================================================================
static bool_t Ips6404l_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen,u32 RecvOff)
{
    if(SPI_TxRx(sdata, slen, rdata, rlen, RecvOff) == false)
        return false;
    return true;
}

//=====================================================================
//��������дһҳ
//���������ͻ�������ҳ��
//����ֵ��true -- �ɹ��� false -- ʧ��
//���ܣ�
//=====================================================================
bool_t Ips6404l_Write(u32 WriteAddr, u8* pBuffer, u32 len)
{
    u8 *sndbuf = NULL;
    Lock_MutexPend(ips6404l_lock,CN_TIMEOUT_FOREVER);

    sndbuf = malloc(4 + len);
    if(sndbuf)
    {
        sndbuf[0] = Ips6404l_WriteData;
        sndbuf[3] = WriteAddr & 0xff;
        sndbuf[2] = (WriteAddr >> 8) & 0xff;
        sndbuf[1] = (WriteAddr >> 16) & 0xff;
        memcpy(sndbuf + 4, pBuffer, len);

        Ips6404l_Cs(0);

        if(Ips6404l_TxRx(sndbuf, 4 + len ,NULL,0,0) == false)
        {
            Ips6404l_Cs(1);
            free(sndbuf);
            Lock_MutexPost(ips6404l_lock);
            error_printf("Ips64"," write fail.\r\n");
            return false;
        }

        Ips6404l_Cs(1);
        free(sndbuf);
    }
    else
    {
        Lock_MutexPost(ips6404l_lock);
        error_printf("Ips64"," sndbuf malloc fail.\r\n");
        return false;
    }

    Lock_MutexPost(ips6404l_lock);
    return true;

}


//=====================================================================
//����������һҳ
//���������ջ�������ҳ��
//����ֵ�� true:�ɹ���false��ʧ��
//���ܣ�
//=====================================================================
bool_t Ips6404l_Read(u32 ReadAddr, u8* pBuffer, u32 len)
{
    u8 sndbuf[4];
    Lock_MutexPend(ips6404l_lock,CN_TIMEOUT_FOREVER);

    sndbuf[0] = Ips6404l_ReadData;
    sndbuf[3] = ReadAddr & 0xff;
    sndbuf[2] = (ReadAddr >> 8) & 0xff;
    sndbuf[1] = (ReadAddr >> 16) & 0xff;

    Ips6404l_Cs(0);

    if(Ips6404l_TxRx(sndbuf,4,pBuffer,len,4) == false)
    {
        Ips6404l_Cs(1);
        Lock_MutexPost(ips6404l_lock);
        error_printf("Ips64"," read fail.\r\n");
        return false;
    }

    Ips6404l_Cs(1);

    Lock_MutexPost(ips6404l_lock);
    return true;
}
//=====================================================================
//����������оƬID
//������
//����ֵ������:�ɹ���-1��ʧ��
//���ܣ�
//=====================================================================
u16 Ips6404l_ReadID(void)
{
    u8 sndbuf[4];
    u8 rcvbuf[2];
    u16 deviceid;

    sndbuf[0]=Ips6404l_ManufactDeviceID;
    sndbuf[1]=0x00;
    sndbuf[2]=0x00;
    sndbuf[3]=0x00;

    Ips6404l_Cs(0);
    if(Ips6404l_TxRx(sndbuf,4,rcvbuf,2,4) == false)
    {
        Ips6404l_Cs(1);
        return -1;
    }
    Ips6404l_Cs(1);

    deviceid=(rcvbuf[0]<<8)|rcvbuf[1];
    return deviceid;
}


// =============================================================================
// ���ܣ���ʼ��QSPI FLASHģ�飬У��оƬID�Ƿ���ȷ
// ��������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע�������߼������ļ�ϵͳ��ֱ�ӷ����߼��������÷�����
// =============================================================================
s32 ModuleInstall_Ips6404l(void)
{
    u16 temp;

    Ips6404l_GpioInit();
    temp=Ips6404l_ReadID();

    if(temp==ips6404l_type)
   {
        printf("Ips6404l Read ID Success,ID:%x\r\n",ips6404l_type);
    }
    else
    {
       printf("Ips6404l Read ID Error,True ID:%x",ips6404l_type);
       printf("    Read ID:%x\r\n",temp);
       return -1;
    }
    ips6404l_lock = Lock_MutexCreate("gd25q64c Lock");
    if(!ips6404l_lock)
    {
        printf("\r\nMODULE INSTALL : error : cannot create Ips6404l flash lock.");
        return -1;
    }
    return 0;
}



