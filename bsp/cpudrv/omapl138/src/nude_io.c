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
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��: ԭʼ�������
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ��׼�����������Ҫʹ�ò���ϵͳ���ܣ����ļ��ṩһ���ڳ�ʼ�����֮ǰ��
//          ������������������
//����˵��:
//�޶���ʷ:
//1. ����:2009-07-22
//   ����:  ������.
//   �°汾�ţ�1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include <string.h>
#include "stdint.h"
#include "cpu_peri.h"

/*
static volatile struct uart_reg *pg_uart0_reg
                        = (struct uart_reg *)0x50000000;
uint8_t Nude_ScanByte(void)
{
    uint32_t fifo_reg;
    for(;;)
    {
        fifo_reg = pg_uart0_reg->UFSTAT;
        if((fifo_reg & 0x3f) != 0)
            return pg_uart0_reg->URXH;
    }
}
bool_t Nude_ReadByte(uint8_t *byte)
{
    uint32_t fifo_reg;
    fifo_reg = pg_uart0_reg->UFSTAT;
    if((fifo_reg & 0x3f) != 0)
    {
        *byte = pg_uart0_reg->URXH;
        return true;
    }else
        return false;
}

void Nude_PrintfByte(uint8_t byte)
{
    uint32_t fifo_reg;
    for(;;)
    {
        fifo_reg = pg_uart0_reg->UFSTAT;
        if(((fifo_reg>>8) & 0x3f) < 0x3f)
        {
            pg_uart0_reg->UTXH = byte;
            return ;
        }
    }
}

void Nude_PrintfStr(uint8_t *str)
{
    uint32_t i=0;
    while(str[i] != 0)
    {
        Nude_PrintfByte(str[i]);
        i++;
    }
}

void Nude_PrintfHex(uint32_t hex,uint32_t size)
{
    uint8_t uni;
    if((size < 1 )|| (size >8))
        return ;
    for(;size > 0; size--)
    {
        uni = (hex >>(size*4-4))&0xf;
        if(uni < 10)
            Nude_PrintfByte(uni+0x30);
        else
            Nude_PrintfByte(uni+0x37);
    }
}

bool_t Nude_TerminalInit(void)
{
    uint32_t temp;
    //��ʼ��IO�˿�λuart����
    //Ports  :  GPH10 GPH9 GPH8 GPH7 GPH6 GPH5 GPH4 GPH3 GPH2 GPH1  GPH0
    //Signal :   xx    xx   xx   xx   xx   xx   xx  RXD0 TXD0 nRTS0 nCTS0
    //Binary :   xx    xx   xx   xx   xx   xx   xx   xx   xx   xx   xx
    temp = g_ptGpioReg->GPHCON;
    temp &= ~0xff;
    temp |= 0xaa;
    g_ptGpioReg->GPHCON = temp;
    g_ptGpioReg->GPHUP |= 0xf;   // The pull up function is disabled GPH[3:0]

    pg_uart0_reg->ULCON = 0x3;
    pg_uart0_reg->UCON =  0x5;
    pg_uart0_reg->UFCON = 0x57;   //bit0=1ʹ��fifo.
    pg_uart0_reg->UMCON = 0x0;
    pg_uart0_reg->UBRDIV = ((cn_pclk<<2)/115200 -32)>>6;
    return true;
}

*/