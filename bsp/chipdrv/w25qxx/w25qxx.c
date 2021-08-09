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
#include <device/djy_flash.h>
#include <spibus.h>
#include <dbug.h>
#include "w25qxx.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_W25qxx(void);
//    ModuleInstall_W25qxx();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"norflash W25QXX"//SPI��QSPI�ӿڵ�nor flashоƬ
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
#if ( CFG_MODULE_ENABLE_NORFLASH_W25QXX == false )
//#warning  " norflash_W25QXX  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_NORFLASH_W25QXX    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,true,false,
#define CFG_W25QXX_PART_ERASE               false      //����ѡ��,�Ƿ���Ҫ������оƬ��
//%$#@string,1,20,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


u8 W25QXX_QPI_MODE=0;       //QSPIģʽ��־:0,SPIģʽ;1,QSPIģʽ.
struct MutexLCB *W25qxx_Lock;           //оƬ������ʱ���
struct NorDescr *W25qxx_description = NULL;
bool_t W25qxx_InitFlag = false;

//-----------------------------------------------------------------------------
//����: ��״̬�Ĵ���
//����: regno��״̬�Ĵ������
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
u8 W25QXX_ReadSR(u8 regno, u8 *sta)
{
    u8 command = 0;
    bool_t ret = true;
    switch(regno)
    {
        case 1:
            command=W25X_ReadStatusReg1;    //��״̬�Ĵ���1
            break;
        case 2:
            command=W25X_ReadStatusReg2;    //��״̬�Ĵ���2
            break;
        case 3:
            command=W25X_ReadStatusReg3;    //��״̬�Ĵ���3
            break;
        default:
            command=W25X_ReadStatusReg1;
            break;
    }
    if(W25QXX_QPI_MODE)
    {
        //QSPI����ַλ0��4�ߴ����ݣ�8λ��ַ���޵�ַ��4�ߴ���ָ��޿����ڣ�1�ֽ�����
        if(QSPI_Send_CMD(command,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES) == false)
            ret = false;
    }
    else
    {
        //SPI����ַλ0�����ߴ����ݣ�8λ��ַ���޵�ַ�����ߴ���ָ��޿����ڣ�1�ֽ�����
        if(QSPI_Send_CMD(command,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_1_LINE) == false)
            ret = false;
    }
    if(QSPI_Receive(sta, 1) == false)
        ret = false;

    return ret;
}


//-----------------------------------------------------------------------------
//����: �ȴ�����
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t W25QXX_WaitBusy(u32 timeout)
{
    bool_t ret = true;
    u32 time = 0;
    u8 sta = 0;
    if(W25QXX_ReadSR(1, &sta) == false)
        return false;
    while((sta & 0x01) == 0x01)   //�ȴ�����
    {
        if(W25QXX_ReadSR(1, &sta) == false)
            return false;
        DJY_EventDelay(1000);
        time ++;
        if(time > timeout)
        {
            ret = false;
            error_printf("W25q","Wait busy timeput.\r\n");
            break;
        }
    }
    return ret;
}

//-----------------------------------------------------------------------------
//����: д״̬�Ĵ���
//����: regno��״̬�Ĵ�����ţ�data: ��д�������
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
u8 W25QXX_WriteSR(u8 regno, u8 data)
{
    u8 command=0;
    switch(regno)
    {
        case 1:
            command=W25X_WriteStatusReg1;    //д״̬�Ĵ���1
            break;
        case 2:
            command=W25X_WriteStatusReg2;    //д״̬�Ĵ���2
            break;
        case 3:
            command=W25X_WriteStatusReg3;    //д״̬�Ĵ���3
            break;
        default:
            command=W25X_WriteStatusReg1;
            break;
    }

    if(W25QXX_WaitBusy(5000) == false)
    {
        return false;
    }

    if(W25QXX_QPI_MODE)
    {
        //QSPI����ַλ0��4�ߴ����ݣ�8λ��ַ���޵�ַ��4�ߴ���ָ��޿����ڣ�1�ֽ�����
        QSPI_Send_CMD(command,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES);
    }
    else
    {
        //SPI����ַλ0�����ߴ����ݣ�8λ��ַ���޵�ַ�����ߴ���ָ��޿����ڣ�1�ֽ�����
        QSPI_Send_CMD(command,0,0, QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_1_LINE);
    }
    return QSPI_Transmit(&data, 1);
}

//-----------------------------------------------------------------------------
//����: дʹ�ܵȴ�
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t W25QXX_WriteEnableWait(void)
{
    u8 sta = 0;
    u32 wait = 40;
    if(W25QXX_ReadSR(1, &sta))
    {
        while((sta & 2) != 2)
        {
            wait --;
            if(wait == 0)
                return false;
            sta = 0;
            W25QXX_ReadSR(1, &sta);
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
//����: дʹ��
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t W25QXX_WriteEnable(void)
{
    if(W25QXX_WaitBusy(5000) == false)
    {
        return false;
    }

    if(W25QXX_QPI_MODE)
    {
        //QSPI����ַλ0�������ݣ�8λ��ַ���޵�ַ��4�ߴ���ָ��޿����ڣ�0�ֽ�����
         if(QSPI_Send_CMD(W25X_WriteEnable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE) == false)
             return false;
    }
    else
    {
        //SPI����ַλ0�������ݣ�8λ��ַ���޵�ַ�����ߴ���ָ��޿����ڣ�0�ֽ�����
        if(QSPI_Send_CMD(W25X_WriteEnable,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE) == false)
            return false;
    }
    return W25QXX_WriteEnableWait();
//    return true;
}

//-----------------------------------------------------------------------------
//����: дʧ��
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t W25QXX_WriteDisable(void)
{
    bool_t ret = false;

    if(W25QXX_WaitBusy(5000) == false)
    {
        return false;
    }

    if(W25QXX_QPI_MODE)
    {
        //QSPI����ַλ0�������ݣ�8λ��ַ���޵�ַ��4�ߴ���ָ��޿����ڣ�0�ֽ�����
        ret = QSPI_Send_CMD(W25X_WriteDisable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
    }
    else
    {
        //SPI����ַλ0�������ݣ�8λ��ַ���޵�ַ�����ߴ���ָ��޿����ڣ�0�ֽ�����
        ret = QSPI_Send_CMD(W25X_WriteDisable,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
    }
    return ret;
}

//-----------------------------------------------------------------------------
//����: ��ID
//����: ��
//����: оƬID
//��ע:
//-----------------------------------------------------------------------------
u16 W25QXX_ReadID(void)
{
    u8 temp[2];
    u16 deviceid;
    bool_t ret = false;

    if(W25QXX_WaitBusy(5000) == false)
    {
        return false;
    }

    if(W25QXX_QPI_MODE)
    {
        //QSPI����ַλ0��4�ߴ������ݣ�24λ��ַ��4�ߴ����ַ��4�ߴ���ָ��޿����ڣ�2�ֽ�����
        ret = QSPI_Send_CMD(W25X_ManufactDeviceID,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES);
    }
    else
    {
        //SPI����ַλ0�����ߴ������ݣ�24λ��ַ�����ߴ����ַ�����ߴ���ָ��޿����ڣ�2�ֽ�����
        ret = QSPI_Send_CMD(W25X_ManufactDeviceID,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_1_LINE);
    }
    if(ret)
    {
        QSPI_Receive(temp,2);
        deviceid=(temp[0] << 8) | temp[1];
    }
    else
        deviceid = 0;
    return deviceid;
}
//-----------------------------------------------------------------------------
//����: оƬ����
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t W25QXX_EraseChip(void)
{
    Lock_MutexPend(W25qxx_Lock,CN_TIMEOUT_FOREVER);
    if(W25QXX_WriteEnable())
    {
        if(W25QXX_WaitBusy(5000))
        {
            //QSPI����ַΪ0�������ݣ�8λ��ַ���޵�ַ��4�ߴ���ָ��޿�����
            if(QSPI_Send_CMD(W25X_ChipErase,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE))
            {
                if(W25QXX_WaitBusy(5000))
                {
                    Lock_MutexPost(W25qxx_Lock);
                    return true;
                }
            }
        }
    }
    Lock_MutexPost(W25qxx_Lock);
    return false;
}

//-----------------------------------------------------------------------------
//����: ��������
//����: addr��������ַ
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t W25QXX_EraseSector(u32 addr)
{
    addr *= 4096;
    Lock_MutexPend(W25qxx_Lock,CN_TIMEOUT_FOREVER);
    if(W25QXX_WriteEnable())
    {
//        if(W25QXX_WaitBusy(5000))
//        {
            //QSPI����ַΪaddr�������ݣ�32λ��ַ��4�ߴ����ַ��4�ߴ���ָ��޿�����
            if(QSPI_Send_CMD(W25X_SectorErase,addr,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_NONE))
            {
//                if(W25QXX_WaitBusy(5000))
//                {
                    Lock_MutexPost(W25qxx_Lock);
                    return true;
//                }
            }
//        }
    }
    Lock_MutexPost(W25qxx_Lock);
    return false;
}

//-----------------------------------------------------------------------------
//����: ��������
//����: addr��������ַ
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t W25QXX_EraseBlock(u32 addr)
{
    addr *= 65536;
    Lock_MutexPend(W25qxx_Lock,CN_TIMEOUT_FOREVER);
    if(W25QXX_WriteEnable())
    {
//        if(W25QXX_WaitBusy(5000))
//        {
            //QSPI����ַΪaddr�������ݣ�32λ��ַ��4�ߴ����ַ��4�ߴ���ָ��޿�����
            if(QSPI_Send_CMD(W25X_BlockErase,addr,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_NONE))
            {
//                if(W25QXX_WaitBusy(5000))
//                {
                    Lock_MutexPost(W25qxx_Lock);
                    return true;
//                }
            }
//        }
    }
    Lock_MutexPost(W25qxx_Lock);
    return false;
}

//-----------------------------------------------------------------------------
//����: ��flash
//����: buf�����ݻ�������addr�����ĵ�ַ��len����ȡ���ֽ���
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t W25QXX_Read(u8* buf,u32 addr,u32 len)
{
    Lock_MutexPend(W25qxx_Lock,CN_TIMEOUT_FOREVER);

    if(W25QXX_WaitBusy(5000) == false)
    {
        Lock_MutexPost(W25qxx_Lock);
        return false;
    }


    //QSPI�����ٶ����ݣ���ַΪaddr��4�ߴ������ݣ�32λ��ַ��4�ߴ����ַ��4�ߴ���ָ�2�������ڣ�
    if(QSPI_Send_CMD(W25X_FastReadData,addr,2,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_4_LINES))
    {
        if(QSPI_Receive(buf, len))
        {
            Lock_MutexPost(W25qxx_Lock);
            return true;
        }
        else
        {
            Lock_MutexPost(W25qxx_Lock);
            return false;
        }
    }
    Lock_MutexPost(W25qxx_Lock);
    return false;
}

//-----------------------------------------------------------------------------
//����: дһҳflash
//����: buf�����ݻ�������addr��д�ĵ�ַ��len��д���ֽ���
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t W25QXX_WritePage(u8* buf,u32 addr,u32 len)
{
    Lock_MutexPend(W25qxx_Lock,CN_TIMEOUT_FOREVER);
    if(W25QXX_WriteEnable())
    {
        //QSPI��ҳд����ַΪaddr��4�ߴ������ݣ�32λ��ַ��4�ߴ����ַ��4�ߴ���ָ�0�������ڣ�
        if(QSPI_Send_CMD(W25X_PageProgram,addr,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_4_LINES))
        {
            if(QSPI_Transmit(buf, len))
            {
//                if(W25QXX_WaitBusy(5000))
//                {
                    Lock_MutexPost(W25qxx_Lock);
                    return true;
//                }
//                else
//                {
//                    Lock_MutexPost(W25qxx_Lock);
//                    return false;
//                }
            }
        }
    }
    Lock_MutexPost(W25qxx_Lock);
    return false;
}

//-----------------------------------------------------------------------------
//����: �����������ܵ�д��д֮ǰ��Ҫȷ����д�ĵ�ַ��Χ�ڵ�����ȫΪ0xff�������д��ʧ��
//����: buf�����ݻ�������addr��д�ĵ�ַ��len����ȡ���ֽ���
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t W25QXX_WriteNoErase(u8* buf,u32 addr,u32 len)
{
    u32 remain = 0;
    bool_t ret = false;

    Lock_MutexPend(W25qxx_Lock,CN_TIMEOUT_FOREVER);
    remain = 256 - addr % 256;  //��ҳ��ʣ����ֽ���
    if(len <= remain)
        remain = len;
    while(1)
    {
        if(W25QXX_WritePage(buf, addr, remain))
        {
            if(remain == len)
            {
                ret = true;
                break;
            }
            else
            {
                buf += remain;
                addr += remain;
                len -= remain;
                if(len > 256)
                    remain = 256;
                else
                    remain = len;
            }
        }
        else
            break;

    }
    Lock_MutexPost(W25qxx_Lock);
    return ret;
}

//-----------------------------------------------------------------------------
//����: дflash
//����: buf�����ݻ�������addr��д�ĵ�ַ��len����ȡ���ֽ���
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
u8 W25QXX_BUFFER[4096];
bool_t W25QXX_Write(u8* buf,u32 addr,u16 len)
{
    u32 sec;
    u16 sec_off;
    u16 sec_remain;
    u16 i;
    bool_t ret = true;

    Lock_MutexPend(W25qxx_Lock,CN_TIMEOUT_FOREVER);
    sec = addr / 4096; //������ַ
    sec_off = addr % 4096;//����ƫ��
    sec_remain = 4096- sec_off; //����ʣ���С

    if(len <= sec_remain)
        sec_remain = len;
    while(1)
    {
        if(W25QXX_Read(W25QXX_BUFFER, sec*4096, 4096))
        {
            for(i=0; i < sec_remain; i++)//
            {
                if(W25QXX_BUFFER[sec_off + i] != 0XFF)
                    break;
            }
            if(i < sec_remain)    //��Ҫ����
            {
                if(W25QXX_EraseSector(sec))
                {
                    for(i=0; i<sec_remain; i++)
                    {
                        W25QXX_BUFFER[i + sec_off] = buf[i];
                    }
                    if(W25QXX_WriteNoErase(W25QXX_BUFFER, sec*4096, 4096) == false)
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
                if(W25QXX_WriteNoErase(buf, addr, sec_remain) == false)
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

                buf += sec_remain;
                addr += sec_remain;
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
    Lock_MutexPost(W25qxx_Lock);
    return ret;
}

//-----------------------------------------------------------------------------
//����: W25QXX_Qspiʹ��
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t W25QXX_QspiEnable(void)
{
    u8 sta2;
    bool_t ret = true;

    if(W25QXX_ReadSR(2, &sta2) == false)       //��״̬�Ĵ���2��ֵ
        return false;
    if((sta2 & 0X02)==0)           //QEλδʹ��
    {
        W25QXX_WriteEnable();      //дʹ��
        sta2 |= 1<<1;              //ʹ��QEλ
        if(W25QXX_WriteSR(2, sta2) == false) //д״̬�Ĵ���2
            ret = false;

        //дʹ��QSPIָ���ַΪ0�������ݣ�8λ��ַ���޵�ַ�����ߴ���ָ��޿����ڣ�0���ֽ�����
//        QSPI_Send_CMD(W25X_ExitQPIMode,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
        if(QSPI_Send_CMD(W25X_EnterQPIMode,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE))
        {
            W25QXX_QPI_MODE=1;              //���QSPIģʽ
            if(W25QXX_ReadSR(2, &sta2))       //��״̬�Ĵ���2��ֵ
            {
                if((sta2 & 0X02)==0)           //QEλδʹ��
                    ret = false;
            }
            else
                ret = false;
        }
        else
            ret = false;
    }
    else
    {
        if(QSPI_Send_CMD(W25X_EnterQPIMode,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE))
            W25QXX_QPI_MODE=1;              //���QSPIģʽ
        else
            ret = false;
    }
    return ret;
}

//-----------------------------------------------------------------------------
//����: W25QXX_Qspiʧ��
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t W25QXX_QspiDisable(void)
{
    //дʧ��QSPIָ���ַΪ0�������ݣ�8λ��ַ���޵�ַ��4�ߴ���ָ��޿����ڣ�0���ֽ�����
    if(QSPI_Send_CMD(W25X_ExitQPIMode,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE))
    {
        W25QXX_QPI_MODE=0;              //���SPIģʽ
        return true;
    }
    else
        return false;
}

//-----------------------------------------------------------------------------
//����: W25QXX��ʼ��
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t W25QXX_Init(void)
{
    u8 temp;
    u16 chip_id;
    bool_t ret = false;
    if(W25QXX_QspiEnable())
    {
        chip_id = W25QXX_ReadID();
        if(chip_id == 0XEF18)
        {
            if(W25QXX_ReadSR(3, &temp))      //��״̬�Ĵ���3
            {
                if((temp & 0X01) == 0)          //�жϵ�ǰ�Ƿ�Ϊ4��ַģʽ��0��3��ַģʽ��1��4��ַģʽ
                {
                    if(W25QXX_WriteEnable())  //дʹ��
                    {
                        //дʹ��4�ֽڵ�ַָ���ַΪ0�������ݣ�8λ��ַ���޵�ַ��4�ߴ���ָ��޿����ڣ�0���ֽ�����
                        if(QSPI_Send_CMD(W25X_Enable4ByteAddr,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE) == false)
                            return ret;
                    }
                    else
                        return ret;
                }
                if(W25QXX_WriteEnable())
                {
                    //д���ö�����ָ���ַΪ0��4�ߴ������ݣ�8λ��ַ���޵�ַ��4�ߴ���ָ��޿����ڣ�0���ֽ�����
                    if(QSPI_Send_CMD(W25X_SetReadParam,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES))
                    {
                        temp = 0<<4;                  //����P4&P5=00,2��dummy clocks,44M
                        if(QSPI_Transmit(&temp,1))
                            ret = true;
                    }
                }
            }
        }
        else
            error_printf("W25q","chip ID error.\r\n");
    }
    return ret;
}


// ============================================================================
// ���ܣ�QFLASH ����
// ������ucmd -- ���
//      ���� -- ���������
// ���أ�0 -- �ɹ���-1 -- ʧ��
// ��ע��
// ============================================================================
s32 __W25qxx_Req(enum ucmd cmd, ptu32_t args, ...)
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
            *block = *unit / W25qxx_description->PagesPerBlock;
            break;
        }

        case totalblocks:
        {
            *((u32*)args) =  W25qxx_description->BlockNum;
            break;
        }

        case blockunits:
        {
            // args = &units
            *((u32*)args) = W25qxx_description->PagesPerBlock;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = W25qxx_description->BytesPerPage;
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

            if(-1 == (s32)end)
                end = W25qxx_description->SectorNum;
            else if (start)
                end += start;

            do
            {
                if(W25QXX_EraseBlock(--end) == false)
                {
                    res = -1;
                    break;
                }
            }
            while(end!=start);
            break;
        }
        default: res = -1;
        break;
    }

    return (res);
}

//-----------------------------------------------------------------------------
//����: Norģ�鰲װ
//����:
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 ModuleInstall_W25qxx(void)
{
    W25qxx_Lock = Lock_MutexCreate("gd25q16c Lock");
    if(!W25qxx_Lock)
    {
        printf("\r\nMODULE INSTALL : error : cannot create w25qxx flash lock.");
        return false;
    }

    if(W25QXX_Init() == false)
    {
        error_printf("W25q","Install fail.\r\n");
        return false;
    }

    if(!W25qxx_description)
    {
        W25qxx_description = malloc(sizeof(struct NorDescr));
        if(!W25qxx_description)
        {
            printf("\r\n: erro : device : memory out.\r\n");
            return false;
        }
        memset(W25qxx_description, 0x0, (sizeof(struct NorDescr)));
        W25qxx_description->PortType = NOR_SPI;
        W25qxx_description->Port = NULL;
        W25qxx_description->BytesPerPage = 256;
        W25qxx_description->SectorNum = 8192;
        W25qxx_description->BlockNum = 512;
        W25qxx_description->BlocksPerSector = 0;
        W25qxx_description->PagesPerSector = 16;
        W25qxx_description->SectorsPerBlock = 16;
        W25qxx_description->PagesPerBlock = 256;
        W25qxx_description->ReservedBlks = 0;
    }

    if(CFG_W25QXX_PART_ERASE)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        if(-1 == __W25qxx_Req(format, 0 , -1, &sz))
        {
            warning_printf("W25q"," Format failure.");
        }
    }

    W25qxx_InitFlag = true;

    return true;

}

// =============================================================================
// ���ܣ��ж�w25q�Ƿ�װ
// ������  ��
// ���أ��ѳɹ���װ��true����δ�ɹ���װ��false����
// ��ע��
// =============================================================================
bool_t W25qxx_is_install(void)
{
    return W25qxx_InitFlag;
}





#if 0   //ע�͵�����SPI����оƬ�ķ�ʽ������̫���ˣ��Ժ�Ҫ�����ַ�ʽ��ʱ��������
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

    DJY_EventDelay(400000);// �г�//��ʱ�г�.

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

    DJY_EventDelay(1000000);// �г�//��ʱ�г�.

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

    DJY_EventDelay(40000000);// �г�//��ʱ�г�.

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

    DJY_EventDelay(3000);// ��ʱ�г�.

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

s32 ModuleInstall_W25qxx(void)
{
    u32 Len;
    struct FlashChip *Chip;

    if(NULL == CFG_W25QXX_BUS_NAME)
        return (-1);

    Len = strlen(CFG_W25QXX_BUS_NAME) + 1;

    Chip = (struct FlashChip*)malloc(sizeof(*Chip) + Len);
    if(NULL == Chip)
    {
        error_printf("w25qxx","out of memory!\r\n");
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
        error_printf("w25qxx","out of memory!\r\n");
        free(Chip);
        return (-2);
    }

    Device_Create(Chip->Name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip);// �豸����"/dev"

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
#endif




