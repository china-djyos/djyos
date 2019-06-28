//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

#include <string.h>
#include <stdlib.h>
#include <cpu_peri.h>
#include <djyos.h>
#include <device.h>
#include <device/flash/flash.h>
#include <spibus.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_NOR(void);
//    ModuleInstall_NOR();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"norflash W25QXX"//SPI�ӿڵ�nor flashоƬ
//parent:"file system"//��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"file system","heap","spi bus","cpu onchip spi"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_NORFLASH_W25QXX == false )
//#warning  " norflash_W25QXX  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_NORFLASH_W25QXX    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
#define CFG_W25QXX_START_BLK               0         //"��ʼ��",�ļ�ϵͳ��ʼ��
//%$#@enum,1,2,
#define CFG_W25QXX_FLAG                   (2)        //"FLASH���λ",FLASH_BUFFERED(2) OR FLASH_ERASE_ALL(1)
//%$#@string,1,20,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#define CFG_W25QXX_BUS_NAME               ""         //"��������",FLASH���߽ӿ�����
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

//
//  ������Դ��,BITBAND�߼�
/*
#define BITBAND(addr, bitnum)         ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)              *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)       MEM_ADDR(BITBAND(addr, bitnum))
#define GPIOA_ODR_Addr                (GPIOA_BASE+20)
#define GPIOB_ODR_Addr                (GPIOB_BASE+20) //0x40020414
#define PAout(n)                       BIT_ADDR(GPIOA_ODR_Addr,n)  //���
#define PBout(n)                       BIT_ADDR(GPIOB_ODR_Addr,n)  //���
*/
static struct SPI_Device *s_ptSpiPort; // ����ʹ�õ�SPI�˿�
static struct FlashChip *s_pChip; // ������

//-----------------------------------------------------------------------------
//����: Norר���߼���дʹ��
//����:
//����: 0 -- �ɹ�; -1 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
static s32 __WriteEnable(void)
{
    struct SPI_DataFrame CommandFrame;
    s32 Ret = 0;
    u8 Command[1] = {0x06};

    CommandFrame.RecvBuf = NULL;
    CommandFrame.RecvLen = 0;
    CommandFrame.RecvOff = 0;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 1;

    SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER);

    if(CN_SPI_EXIT_NOERR !=
            SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
    {
        Ret = -1;
    }

    SPI_CsInactive(s_ptSpiPort);

    return (Ret);
}

//-----------------------------------------------------------------------------
//����: ��ȡNor״̬
//����:
//����: 0 -- �ɹ�; -1 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
static s32 __Status1(u8 *Status)
{
    struct SPI_DataFrame CommandFrame;
    s32 Ret = 0;
    u8 Command[1] = {0x05};

    CommandFrame.RecvBuf = Status;
    CommandFrame.RecvLen = 1;
    CommandFrame.RecvOff = 1;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 1;

    SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER);

    if(CN_SPI_EXIT_NOERR !=
            SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
    {
        Ret = -1;
    }

    SPI_CsInactive(s_ptSpiPort);

    return (Ret);
}

//-----------------------------------------------------------------------------
//����: �������Ƿ����
//����:
//����:  -1 -- δ���; 0 -- ���;
//��ע:
//-----------------------------------------------------------------------------
static s32 __Done(void)
{
    u8 Status;

    if(__Status1(&Status))
        return ( -1);

    if(Status & 0x1)
        return (-1);

    return (0);
}

//-----------------------------------------------------------------------------
//����: ��ȡNor ID��Ϣ
//����:
//����: ���� -- ID��Ϣ; 0 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
static u16 __ID(void)
{
    struct SPI_DataFrame CommandFrame;
    s32 Ret = 0;
    u16 ID;
    u8 Command[4];

    Command[0] = 0x90;
    Command[1] = 0x0;
    Command[2] = 0x0;
    Command[3] = 0x0;

    CommandFrame.RecvBuf = (u8*)(&ID);
    CommandFrame.RecvLen = 2;
    CommandFrame.RecvOff = 4;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 4;

    SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER);

    Ret = SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER);
    if(CN_SPI_EXIT_NOERR != Ret)
    {
        ID = 0;
    }

    SPI_CsInactive(s_ptSpiPort);

    return (ID);
}

//-----------------------------------------------------------------------------
//����: ����һ��sector
//����: SectorNo -- �Ⱥ�
//����: 0 -- �ɹ�; 1 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 w25qxx_SectorErase(u32 SectorNo)
{
    struct SPI_DataFrame CommandFrame;
    s32 Ret = 0;
    u8 Command[4];

    SectorNo = (SectorNo << 4); // SectorNoתPageNo
    Command[0] = 0x20;
    Command[1] = ((SectorNo >> 8) & 0xFF);
    Command[2] = (SectorNo & 0xFF);
    Command[3] = 0x0;

    CommandFrame.RecvBuf = NULL;
    CommandFrame.RecvLen = 0;
    CommandFrame.RecvOff = 0;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 4;

    if(__WriteEnable())
        return (-1);

    SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER);

    if(CN_SPI_EXIT_NOERR !=
            SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
    {
        Ret = -1;
    }

    SPI_CsInactive(s_ptSpiPort);

    Djy_EventDelay(400000);// �г�//��ʱ�г�.

    if((0 == Ret) && (__Done()))
    {
        Ret = -1;
    }

    return (Ret);
}
//-----------------------------------------------------------------------------
//����: ����һ��block
//����: BlkNo -- ���
//����: 0 -- �ɹ�; 1 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 w25qxx_BlkErase(u32 BlkNo)
{
    struct SPI_DataFrame CommandFrame;
    s32 Ret = 0;
    u8 Command[4];

    Command[0] = 0xD8;
    Command[1] = (BlkNo & 0xFF);
    Command[2] = 0x0;
    Command[3] = 0x0;

    CommandFrame.RecvBuf = NULL;
    CommandFrame.RecvLen = 0;
    CommandFrame.RecvOff = 0;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 4;

    if(__WriteEnable())
        return (-1);

    SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER);

    if(CN_SPI_EXIT_NOERR !=
            SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
    {
        Ret = -1;
    }

    SPI_CsInactive(s_ptSpiPort);

    Djy_EventDelay(1000000);// �г�//��ʱ�г�.

    if((0 == Ret) && (__Done()))
    {
        Ret = -1;
    }

    return (Ret);
}

//-----------------------------------------------------------------------------
//����: ��������оƬ
//����:
//����: 0 -- �ɹ�; 1 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 w25qxx_ChipErase(void)
{
    struct SPI_DataFrame CommandFrame;
    s32 Ret = 0;
    u8 Command[1] = {0xC7};

    CommandFrame.RecvBuf = NULL;
    CommandFrame.RecvLen = 0;
    CommandFrame.RecvOff = 0;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 1;

    if(__WriteEnable())
        return (-1);

    SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER);

    if(CN_SPI_EXIT_NOERR !=
            SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
    {
        Ret = -1;
    }

    SPI_CsInactive(s_ptSpiPort);

    Djy_EventDelay(40000000);// �г�//��ʱ�г�.

    if((0 == Ret) && (__Done()))
    {
        Ret = -1;
    }

    return (Ret);
}

//-----------------------------------------------------------------------------
//����: ��һҳ
//����: PageNo -- ҳ��;
//      Data -- ������;
//      Flags -- ����;
//����: >0 -- �����ֽ���; -1 -- ��������; -2 -- ��ʧ��
//��ע:
//-----------------------------------------------------------------------------
s32 w25qxx_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    struct SPI_DataFrame CommandFrame;
    s32 Ret = -2;
    u8 Command[4];

    if(NULL == Data)
        return (-1);

    Command[0] = 0x03;
    Command[1] = ((PageNo >> 8) & 0xFF);
    Command[2] = (PageNo & 0xFF);
    Command[3] = 0;

    CommandFrame.RecvBuf = Data;
    CommandFrame.RecvLen = 256;
    CommandFrame.RecvOff = 4;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 4;

    SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER);

    if(CN_SPI_EXIT_NOERR ==
            SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
    {
        Ret = 256;
    }

    SPI_CsInactive(s_ptSpiPort);

    return (Ret);
}

//-----------------------------------------------------------------------------
//����: дһҳ
//����: PageNo -- ҳ��;
//      Data -- д����;
//      Flags -- ����;
//����: >0 -- д���ֽ���; -1 -- ��������; -2 -- д��ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 w25qxx_PageWrite(u32 PageNo, u8 *Data, u32 Flags)
{
    struct SPI_DataFrame CommandFrame;
    s32 Ret;
    u8 Command[4];

    if(NULL == Data)
        return (-1);

    if(__WriteEnable())
        return (-2);

    Command[0] = 0x02;
    Command[1] = ((PageNo >> 8) & 0xFF);
    Command[2] = (PageNo & 0xFF);
    Command[3] = 0;

    CommandFrame.RecvBuf = NULL;
    CommandFrame.RecvLen = 0;
    CommandFrame.RecvOff = 0;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 4;

    SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER);
    Ret = SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER);
    if(CN_SPI_EXIT_NOERR == Ret)
    {
        CommandFrame.RecvBuf = NULL;
        CommandFrame.RecvLen = 0;
        CommandFrame.RecvOff = 0;
        CommandFrame.SendBuf = Data;
        CommandFrame.SendLen = 256;
        Ret = SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER);
        if(CN_SPI_EXIT_NOERR == Ret)
        {
            Ret = 256;
        }
        else
        {
            Ret = -2;
        }
    }
    else
    {
        Ret = -2;
    }

    SPI_CsInactive(s_ptSpiPort);

    Djy_EventDelay(3000);// ��ʱ�г�.

    if(__Done())
        Ret = -2;// δ���,дʧ��

    return (Ret);
}

//-----------------------------------------------------------------------------
//����: Nor�豸��ʼ��
//����: Nor -- �豸�ṹ��
//����: -1 -- ʧ��; 0 -- �ɹ�;
//��ע:
//-----------------------------------------------------------------------------

s32 Nor_Init(struct FlashChip *Nor)
{
    struct SPI_Device *SpiPort;


    if(s_ptSpiPort = SPI_DevAdd("SPI1","w25qxx",0,8,SPI_MODE_0,SPI_SHIFT_MSB,1000*1000,false))
    {
        SPI_BusCtrl(s_ptSpiPort,CN_SPI_SET_POLL,0,0);
    }
    else
    {
        printf("w25qxx init failed.\n\r");
        return false;
    }

    Nor->Descr.Nor.PortType = NOR_SPI;
    Nor->Descr.Nor.Port = (void*)s_ptSpiPort;

    Nor->Descr.Nor.BytesPerPage = 256;
    Nor->Descr.Nor.PagesPerSector = 16;
    Nor->Descr.Nor.SectorsPerBlk = 16;
    Nor->Descr.Nor.Blks = 256;

    Nor->Ops.RdPage = w25qxx_PageRead;
    Nor->Ops.WrPage = w25qxx_PageWrite;
    Nor->Ops.ErsBlk = w25qxx_BlkErase;
    Nor->Ops.Special.Nor.ErsChip = w25qxx_ChipErase;

    return (0);
}
//-----------------------------------------------------------------------------
//����: Norģ�鰲װ
//����:
//����: 0 -- �ɹ�; -1 -- ����ʧ��; -2 -- �ڴ治��;
//��ע:
//-----------------------------------------------------------------------------

s32 ModuleInstall_NOR(void)
{
    u32 Len;
    struct FlashChip *Chip;

    if(NULL == CFG_W25QXX_BUS_NAME)
        return (-1);

    Len = strlen(CFG_W25QXX_BUS_NAME) + 1;

    Chip = (struct FlashChip*)malloc(sizeof(*Chip) + Len);
    if(NULL == Chip)
    {
        TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
        return (-2);
    }
    memset(Chip, 0x0, sizeof(*Chip));
    s_pChip = Chip;
    strcpy(Chip->Name, CFG_W25QXX_BUS_NAME); // �豸����
    Chip->Type = F_NOR;
    Chip->Descr.Nor.ReservedBlks = CFG_W25QXX_START_BLK; //

    if(Nor_Init(Chip))
    {
        free(Chip);
        return (-2);
    }

    Chip->Buf = (u8*)malloc(Chip->Descr.Nor.BytesPerPage);// NAND�ײ㻺��
    if(NULL == Chip->Buf)
    {
        TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
        free(Chip);
        return (-2);
    }

    dev_Create(Chip->Name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip);// �豸����"/dev"

    return (0);

}
//-----------------------------------------------------------------------------
//����: ��������
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
void W25qxx_LocalTest(void)
{
    u32 i, j, k;
    u8 Test[256];
    s32 Ret;
    u16 temp;
    temp = __ID();

#if (1)
    for(i=0; i<s_pChip->Descr.Nor.BytesPerPage; i++)
        Test[i] = i;

    for(k=0; k<s_pChip->Descr.Nor.Blks; k++)
    {
        Ret = s_pChip->Ops.ErsBlk(k);
        if(Ret)
            while(1);

        for(i=0; i<(s_pChip->Descr.Nor.PagesPerSector*s_pChip->Descr.Nor.SectorsPerBlk); ++i)
        {
            Ret = s_pChip->Ops.RdPage((i+(k*(s_pChip->Descr.Nor.PagesPerSector*s_pChip->Descr.Nor.SectorsPerBlk))),
                                       s_pChip->Buf, 0);
            if(s_pChip->Descr.Nor.BytesPerPage != Ret)
                while(1);

            for(j=0; j<s_pChip->Descr.Nor.BytesPerPage; j++)
                if(s_pChip->Buf[j] != 0xFF)
                    while(1);
        }


        for(i=0; i<(s_pChip->Descr.Nor.PagesPerSector*s_pChip->Descr.Nor.SectorsPerBlk); ++i)
        {
            Ret = s_pChip->Ops.WrPage((i+(k*(s_pChip->Descr.Nor.PagesPerSector*s_pChip->Descr.Nor.SectorsPerBlk))), Test, 0);
            if(s_pChip->Descr.Nor.BytesPerPage != Ret)
                while(1);
        }

        for(i=0; i<(s_pChip->Descr.Nor.PagesPerSector*s_pChip->Descr.Nor.SectorsPerBlk); ++i)
        {
            Ret = s_pChip->Ops.RdPage((i+(k*(s_pChip->Descr.Nor.PagesPerSector*s_pChip->Descr.Nor.SectorsPerBlk))),
                                      s_pChip->Buf, 0);
            if(s_pChip->Descr.Nor.BytesPerPage != Ret)
                while(1);

            for(j=0; j<s_pChip->Descr.Nor.BytesPerPage; j++)
                if(s_pChip->Buf[j] != j)
                    while(1);
        }
    }
#endif
    if(EarseWholeChip(s_pChip))
        while(1);
    k = (s_pChip->Descr.Nor.PagesPerSector * s_pChip->Descr.Nor.SectorsPerBlk * s_pChip->Descr.Nor.Blks);
    for(i=0; i<k; i++)
    {
        Ret = s_pChip->Ops.RdPage(i, s_pChip->Buf, 0);
        if(s_pChip->Descr.Nor.BytesPerPage != Ret)
            while(1);

        for(j=0; j<s_pChip->Descr.Nor.BytesPerPage; j++)
            if(s_pChip->Buf[j] != 0xFF)
                while(1);
    }

    while(1);
}
