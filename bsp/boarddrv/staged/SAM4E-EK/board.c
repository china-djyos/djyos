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

// �ļ���     ��board.c
// ģ������: �����ز��ֳ�ʼ�������õ�
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 5/11.2015
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "os.h"
#include "cpu_peri.h"


//�������Ŷ���
static const Pin DBGU_Pin[] = {
    {PIO_PA9A_URXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT},
    {PIO_PA10A_UTXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
};
static const Pin USART1_Pin[] = {
    {PIO_PA22A_TXD1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT},
    {PIO_PA21A_RXD1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
};

static const Pin GMAC_Pin[] = {
        {PIO_PD14A_GRXCK, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD0A_GTXCK, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD1A_GTXEN, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD16A_GTX3, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD15A_GTX2, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD3A_GTX1, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD2A_GTX0, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD12A_GRX3, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD11A_GRX2, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD6A_GRX0, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD5A_GRX0, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD7A_GRXER, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD4A_GCRSDV, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD10A_GCRS, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD13A_GCOL, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD8A_GMDC, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD9A_GMDIO, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD10A_GCRS, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},

        {PIO_PD28, PIOD, ID_PIOD, PIO_INPUT, PIO_PULLUP},
};

static const Pin Phy_Interrupt_Pin[] = {
        {PIO_PD28, PIOD, ID_PIOD, PIO_INPUT,    PIO_IT_LOW_LEVEL}
};

// =============================================================================
// ���ܣ����ݾ���İ�����ô��ڵ�GPIO�����Ź��ܣ�����������أ����Ըú������ڸ��ļ���CPU
//      ��������ֱ�ӵ��øú�������ʼ�����ڵ�GPIO����
//      ��Ҫ����GPIO�˿ںʹ���ʱ��ʹ�ܡ�GPIO���á���ӳ�䡢ʱ�ӵ�
// ��������
// ���أ�true
// =============================================================================
bool_t Board_UartGpioInit(u8 SerialNo)
{
    PMC_EnablePeripheral(ID_PIOA);
    switch(SerialNo)
    {
    case CN_UART0:
        PIO_Configure(DBGU_Pin,PIO_LISTSIZE(DBGU_Pin));
        break;
    case CN_USART1:
        PIO_Configure(USART1_Pin,PIO_LISTSIZE(USART1_Pin));
        break;
    default:
        return false;
    }
    return true;
}

// =============================================================================
// ���ܣ�485ͨ��ʱ��˫������Ϊ���ͣ���GPIO�����ã����ǰ�˫�����˺����ɿ���
// ���������ں�
// ���أ���
// =============================================================================
void Board_UartHalfDuplexSend(u8 SerialNo)
{

}

// =============================================================================
// ���ܣ�485ͨ��ʱ��˫������Ϊ���գ���GPIO�����ã����ǰ�˫�����˺����ɿ���
// ���������ں�
// ���أ���
// =============================================================================
void Board_UartHalfDuplexRecv(u8 SerialNo)
{

}

// =============================================================================
// ���ܣ�����������PHY оƬKSZ8051MNL IOͨ���ߵĳ�ʼ��
// ���������ں�
// ���أ���
// =============================================================================
void Board_PhyIoInit(void)
{
    PMC_EnablePeripheral(ID_PIOD);
    PIO_Configure(GMAC_Pin,PIO_LISTSIZE(GMAC_Pin));

    //PHY �ж���������
    extern void GMAC_PhyIntHandler(const Pin* pPin);
    PIO_InitInterrupts(Phy_Interrupt_Pin);
    PIO_ConfigureIt(Phy_Interrupt_Pin,GMAC_PhyIntHandler);
    PIO_EnableIt(Phy_Interrupt_Pin);
}
