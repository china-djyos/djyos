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
//����ģ��: uart��������
//���ߣ�liuwei
//�汾��V1.0.1
//�ļ�����: ����44b0��uart
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2015-04-22
//   ����: liuwei
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CPU_UART_H__
#define __CPU_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

//UTRSTAT״̬λ����
#define UartTXShiftEmpty    (1<<2)  //1:������λ�Ĵ�����,0:�ǿ�
#define UartTxBufEmpty      (1<<1)  //1:���ͻ�������,0:�ǿ�
#define UartRxBufNotEmpty   (1<<0)  //1:���ܻ�����������,0:������
//UFCON FIFO���ƼĴ���
#define cn_tx_fifo_reset    (1<<2)  //д1��λ����fifo,������0
#define cn_rx_fifo_reset    (1<<1)  //д1��λ����fifo,������0
//UFSTAT FIFO״̬�Ĵ���
#define cn_rx_over          (1<<6)  //1:����fifo���,0:����

//ͨ��״̬������λ����
#define CN_SER_RXBUF_BUSY         (1<<0)
#define CN_SER_RXBUF_EMPTY     (1<<1)
#define CN_SER_RECEIVE_OVER    (1<<2)

//�жϿ���λ����2410���жϿ������У�ÿ������ֻ������һ���жϣ������ж�ԭ����ͨ��
//��ѯ SUBSRCPND �Ĵ�����Ӧ��λ���õ��ģ����� INTSUBMSK ����Ӧλ�ɶ����ؽ�ֹ
//����/�����жϡ�
#define BIT_SUB_ERR2    (0x1<<8)
#define BIT_SUB_TXD2    (0x1<<7)
#define BIT_SUB_RXD2    (0x1<<6)
#define BIT_SUB_ERR1    (0x1<<5)
#define BIT_SUB_TXD1    (0x1<<4)
#define BIT_SUB_RXD1    (0x1<<3)
#define BIT_SUB_ERR0    (0x1<<2)
#define BIT_SUB_TXD0    (0x1<<1)
#define BIT_SUB_RXD0    (0x1<<0)
#define BIT_ALL_UART    (0x1ff)

#define SUB_RXD_FLG     0
#define SUB_TXD_FLG     1
#define SUB_ERR_FLG     2
#define BIT_SUB_INT(m,n)   (0x1<<(m*3 + n))
//#define BIT_SUB_INT(m,n)   (0x1<<((m-1)*3 + n))

#define UART_TX_FIFO_SIZE  16  //Ӳ���ķ���fifo���
#define UART_RX_FIFO_SIZE  16  //Ӳ���Ľ���fifo���


struct UART_REG{
    vu32 ULCON;                   // line control reg
    vu32 UCON;                    // control reg
    vu32 UFCON;                   // FIFO control reg
    vu32 UMCON;                   // modem control reg
    vu32 UTRSTAT;                 // tx/rx status reg
    vu32 UERSTAT;                 // rx error status reg
    vu32 UFSTAT;                  // FIFO status reg
    vu32 UMSTAT;                  // modem status reg
    vu32 UTXH;                    // tx buffer reg
    vu32 URXH;                    // rx buffer reg
    vu32 UBRDIV;                  // baud rate divisor
    vu32 UDIVSLOT;                // baud rate divisor
};

#define UART0_REG_BASE  ((struct UART_REG*)0x50000000)
#define UART1_REG_BASE  ((struct UART_REG*)0x50004000)
#define UART2_REG_BASE  ((struct UART_REG*)0x50008000)
#define UART3_REG_BASE  ((struct UART_REG*)0x5000C000)

#define CN_UART0       0
#define CN_UART1       1
#define CN_UART2       2
#define CN_UART_NUM    3

ptu32_t UART_ModuleInit(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif // __CPU_UART_H__

