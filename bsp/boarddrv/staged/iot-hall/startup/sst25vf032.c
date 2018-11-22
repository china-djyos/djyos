//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

// �ļ���     ��SST25VF.c
// ģ������: SPI FLASH оƬ������������SPIBUS��������ģ��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 21/10.2015
// =============================================================================

#include "stdint.h"
#include "string.h"
#include "os.h"
#include "cpu_peri.h"
#include "spibus.h"
#include "sst25vf032.h"

// =============================================================================
//�����붨��
#define CMD_RDSR                    0x05  /* ��״̬�Ĵ���     */
#define CMD_WRSR                    0x01  /* д״̬�Ĵ���     */
#define CMD_EWSR                    0x50  /* ʹ��д״̬�Ĵ��� */
#define CMD_WRDI                    0x04  /* �ر�дʹ��       */
#define CMD_WREN                    0x06  /* ��дʹ��       */
#define CMD_READ                    0x03  /* ������           */
#define CMD_FAST_READ               0x0B  /* ���ٶ�           */
#define CMD_BP                      0x02  /* �ֽڱ��         */
#define CMD_AAIP                    0xAD  /* �Զ���ַ������� */
#define CMD_ERASE_4K                0x20  /* ��������:4K      */
#define CMD_ERASE_32K               0x52  /* ��������:32K     */
#define CMD_ERASE_64K               0xD8  /* ��������:64K     */
#define CMD_ERASE_FULL              0xC7  /* ȫƬ����         */
#define CMD_JEDEC_ID                0x9F  /* �� JEDEC_ID      */
#define CMD_EBSY                    0x70  /* ��SOæ���ָʾ */
#define CMD_DBSY                    0x80  /* �ر�SOæ���ָʾ */

#define CMD_SE  CMD_ERASE_4K
#define CHIP_SIZE      (0x3FFFFF+1)
#define SECTOR_SIZE    4096
#define SECTOR_NUM     (CHIP_SIZE/SECTOR_SIZE)
// =============================================================================
#define SST25VF_CS_LOW  GPIO_SettoLow(GPIO_A,PIN4)
#define SST25VF_CS_HIGH GPIO_SettoHigh(GPIO_A,PIN4)
#define SST25VF_TIMEOUT (1*1000*1000)

struct SPI_Device *s_ptSST25VF_Dev;

void __SST25VF_SpiInterfalce(u8* wBuf,u16 wLen,u8* rBuf,u16 rLen)
{
    struct SPI_DataFrame frame;

    frame.RecvBuf = rBuf;
    frame.RecvLen = rLen;
    frame.RecvOff = wLen;
    frame.SendBuf = wBuf;
    frame.SendLen = wLen;

    SST25VF_CS_LOW;
    SPI_Transfer(s_ptSST25VF_Dev,&frame,1,SST25VF_TIMEOUT);
    SST25VF_CS_HIGH;
}

// =============================================================================
// ���ܣ���ȡFLASH��״̬�Ĵ���
//      BIT7    6    5   4   3   2   1   0
//      SPR    RV     TB     BP2   BP1  BP0 WEL  BUSY
//      SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//      TB,BP2,BP1,BP0:FLASH����д��������
//      WEL:дʹ������
//      BUSY:æ���λ(1,æ;0,����)
//      Ĭ��:0x00
// ��������
// ���أ�״̬�Ĵ���
// =============================================================================
u8 __SST25VF_ReadSR(void)
{
    u8 byte = 0;
    u8 cmd = CMD_RDSR;

    __SST25VF_SpiInterfalce(&cmd,1,&byte,1);
    return byte;
}

// =============================================================================
// ���ܣ�дFLASH��״̬�Ĵ�����ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д
// ��������
// ���أ�״̬�Ĵ���
// =============================================================================
void __SST25VF_WriteSR(u8 sr)
{
    u8 data[2] =  {CMD_EWSR,sr};

    __SST25VF_SpiInterfalce(data,1,NULL,0);
    data[0] = CMD_WRSR;
    __SST25VF_SpiInterfalce(data,2,NULL,0);
}

// =============================================================================
// ���ܣ��ȴ�����
// ��������
// ���أ���
// =============================================================================

void __SST25VF_WaitBusy(void)
{
    while((__SST25VF_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}

// =============================================================================
// ���ܣ�дʹ�ܣ���WEL��λ
// ��������
// ���أ���
// =============================================================================
void __SST25VF_WriteEnable(void)
{
    u8 cmd = CMD_WREN;
    __SST25VF_SpiInterfalce(&cmd,1,NULL,0);
}

// =============================================================================
// ���ܣ�д���ܣ���WEL����
// ��������
// ���أ���
// =============================================================================
void __SST25VF_WriteDisable(void)
{
    u8 cmd = CMD_WRDI;
    __SST25VF_SpiInterfalce(&cmd,1,NULL,0);
}

// =============================================================================
// ���ܣ���SOæ���ָʾ
// ��������
// ���أ���
// =============================================================================
void __SST25VF_EBSY(void)
{
    u8 cmd = CMD_EBSY;
    __SST25VF_SpiInterfalce(&cmd,1,NULL,0);
}

// =============================================================================
// ���ܣ��ر�SOæ���ָʾ
// ��������
// ���أ���
// =============================================================================
void __SST25VF_DBSY(void)
{
    u8 cmd = CMD_DBSY;
    __SST25VF_SpiInterfalce(&cmd,1,NULL,0);
}

// =============================================================================
// ���ܣ�����һ������
// ������Addr:������С 4K��������ַ 0~511,����һ��ɽ��������ʱ��:150ms
// ���أ���
// =============================================================================
void SST25VF_EraseSector(u32 Addr)
{
    u8 data[4] ;

    data[0] = CMD_SE;
    data[1] = (u8)((Addr)>>16);
    data[2] = (u8)((Addr)>>8);
    data[3] = (u8)((Addr)>>0);

    __SST25VF_WriteEnable();                  //SET WEL
    __SST25VF_WaitBusy();
    __SST25VF_SpiInterfalce(data,4,NULL,0);
    __SST25VF_WaitBusy();                      //�ȴ��������
}

// =============================================================================
// ���ܣ���������оƬ
// ������Addr:������С 4K��������ַ 0~511,����һ��ɽ��������ʱ��:150ms
// ���أ���
// =============================================================================
void SST25VF_EraseChip(void)
{
    u8 cmd = CMD_ERASE_FULL;
    __SST25VF_WriteEnable();                  //SET WEL
    __SST25VF_WaitBusy();
    __SST25VF_SpiInterfalce(&cmd,1,NULL,0);
    __SST25VF_WaitBusy();                      //�ȴ�оƬ��������
}

// =============================================================================
// ���ܣ���ȡJEDEC-ID��Ϊ0xBF��0x25��0x4A
// �������������ṩ�Ļ�����
// ���أ���
// =============================================================================
void SST25VF_ReadID(u8 *id)
{
    u8 cmd = CMD_JEDEC_ID;
    __SST25VF_SpiInterfalce(&cmd,1,id,3);
}

// =============================================================================
// ���ܣ���FLASH
// ������ReadAddr�������ݵĵ�ַ����Χ��0x00-0x3FFFFF
//      pBuffer�����ݻ�����
//      NumByteToRead�������ݵ��ֽ���
// ���أ���
// =============================================================================
bool_t SST25VF_Read(u32 ReadAddr,u8* pBuffer,u16 NumByteToRead)
{
    u8 data[] = {CMD_READ,(u8)((ReadAddr)>>16),(u8)((ReadAddr)>>8),(u8)ReadAddr};

    if ((ReadAddr + NumByteToRead > CHIP_SIZE-1)||(NumByteToRead == 0))
        return false;

     __SST25VF_SpiInterfalce(data,4,pBuffer,NumByteToRead);
     return true;
}


void __SST25VF_AutoIncProgramA(u8 Byte1, u8 Byte2, u32 Addr)
{
    u8 data[6] = {CMD_AAIP,(Addr & 0xFF0000) >> 16,(Addr & 0xFF00) >> 8,
        Addr & 0xFF,Byte1,Byte2};

    __SST25VF_WriteEnable();
    __SST25VF_SpiInterfalce(data,6,NULL,0);
    __SST25VF_WaitBusy();
}

void __SST25VF_AutoIncProgramB(u8 Byte1, u8 Byte2)
{
    u8 data[3] = {CMD_AAIP,Byte1,Byte2};

    __SST25VF_WriteEnable();

    __SST25VF_SpiInterfalce(data,3,NULL,0);
    __SST25VF_WaitBusy();
}

//
u8 SectorBuf[SECTOR_SIZE];
bool_t __SST25VF_SectorProgram(u16 sector,u32 offset,u8* pbuffer, u16 bytes)
{
    bool_t result = false;
    u32 i,secAddr;
    u8* pwBuf;

    if( (sector >= SECTOR_NUM) || (bytes == 0) || (pbuffer == NULL))
        return false;

    secAddr = sector*SECTOR_SIZE;
    //���������Ƭ������һ��sector�����һ��sector,���ȡsector���ٲ���
    if( (offset) || (offset+bytes != SECTOR_SIZE))
    {
        SST25VF_Read(secAddr,SectorBuf,SECTOR_SIZE);
        memcpy(SectorBuf+offset,pbuffer,bytes);
        pwBuf = SectorBuf;
    }
    else
    {
        pwBuf = pbuffer;
    }

    //��������sector
    SST25VF_EraseSector(secAddr);

    //д����sector
    __SST25VF_AutoIncProgramA(pwBuf[0],pwBuf[1],secAddr);
    for(i = 1; i < SECTOR_SIZE/2; i++)
    {
        __SST25VF_AutoIncProgramB(pwBuf[i*2],pwBuf[i*2+1]);
    }

    __SST25VF_WriteDisable();
    return result;
}

// =============================================================================
// ���ܣ�дFLASH
// ������wAddr,��ַ��ΧΪ0-0x3FFFFF
//      pbuffer,���ݻ�����
//      bytes,�ֽ���
// ���أ�true,��ȷ��false,����
// =============================================================================
bool_t SST25VF_Write(u32 wAddr,u8* pbuffer, u16 bytes)
{
    bool_t result = false;
    u16 i,temp;
    u16 secoff,secremain,sector;

    if( (wAddr + bytes > CHIP_SIZE-1) || (bytes == 0))
        return false;

    sector = wAddr / SECTOR_SIZE;       //������
    secoff = wAddr % SECTOR_SIZE;       //����ƫ��
    secremain = SECTOR_SIZE - secoff;   //��������

    if(bytes < secremain)
        temp = 1;
    else
    {
        i = bytes - secremain;                            /* �жϻ�ռ�˼�������*/
        if (i % 4096 == 0)
           temp = i / 4096 + 1;
        else
           temp = i / 4096 + 2;
    }

    //д������Ҫд�Ŀ�
    for(i = 0; i < temp; i++)
    {
        if(i == 0)
        {
            __SST25VF_SectorProgram(sector,secoff,pbuffer,
                    (bytes>secremain)? secremain:bytes);
        }
        else if(i == temp-1)
        {
            __SST25VF_SectorProgram(sector+i,0,
                pbuffer+secremain+i*SECTOR_SIZE,(bytes-secremain)%SECTOR_SIZE);
        }
        else
        {
            __SST25VF_SectorProgram(sector+i,0,
                    pbuffer+secremain+i*SECTOR_SIZE,SECTOR_SIZE);
        }
    }
    return result;
}

// =============================================================================
// ���ܣ�дFLASHָ����ַһ���ֽ�����
// ������wAddr,��ַ��ΧΪ0-0x3FFFFF
//      Data,�ֽ���
// ���أ���
// =============================================================================
void SST25VF_WriteByte(u32 Addr, u8 Data)
{
    u32 sector;
    u8 data[5] = {CMD_BP,(Addr & 0xFF0000) >> 16,
                (Addr & 0xFF00) >> 8,Addr & 0xFF,Data};

//  sector = Addr / 4096;                /* ������ַ 0~511 for w25x16  4096=4k */
//
//    SST25VF_Read(sector*SECTOR_SIZE,SectorBuf,SECTOR_SIZE);
//    SectorBuf[Addr-sector*SECTOR_SIZE] = Data;
//
//    SST25VF_EraseSector(sector);       /* �����������*/
    __SST25VF_WriteEnable();
    __SST25VF_SpiInterfalce(data,5,NULL,0);
    __SST25VF_WaitBusy();                          /* �ȴ�д���*/
}

// =============================================================================
// ���ܣ���ʼ��������Ӳ����ʼ����װ���豸�����ã���֤ID
// ��������
// ���أ���
// =============================================================================
bool_t SST25VF_ModuleInit(ptu32_t para)
{
    u8 id[3];

    GPIO_CfgPinFunc(GPIO_A,PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,
            GPIO_SPEED_100M,GPIO_PUPD_PU);//��ֹ����

    if(s_ptSST25VF_Dev = SPI_DevAdd("SPI1","SST25VF",1,8,SPI_MODE_3,SPI_SHIFT_MSB,10*1000*1000,false))
    {
        SPI_BusCtrl(s_ptSST25VF_Dev,CN_SPI_SET_POLL,0,0);
    }
    else
    {
        printf("SST25VF init failed.\n\r");
        return false;
    }

    __SST25VF_WriteSR(0x02);                    /* ʹ��״̬�Ĵ����е�д�洢��*/
    __SST25VF_DBSY();

    SST25VF_ReadID(id);
    printk("id = 0x%x,0x%x,0x%x\r\n",id[0],id[1],id[2]);
    if( (id[0] == 0xBF) && (id[1] == 0x25) && (id[2] == 0x4A))
    {
        return true;
    }
    else
        return false;
}





















