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
//����ģ��: uart��������(�����ӿڲ���)
//����:  ������.
//�汾��V1.0.0
//�ļ�����:
//����˵��:
//�޶���ʷ:
//1. ����:2013-11-10
//   ����:  ������.
//   �°汾�ţ�1.0.0
//   �޸�˵��: ��ֲ��44b0��1.0.1��
//-----------------------------------------------------------------------------
#ifndef __UARTCTRL_H__
#define __UARTCTRL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <device.h>
#include <ring.h>

// ���ڲ����ʶ���
enum tagUartBaud
{
    CN_UART_BAUD_110     =  110,
    CN_UART_BAUD_300     =  300,
    CN_UART_BAUD_600     =  600,
    CN_UART_BAUD_1200    =  1200,
    CN_UART_BAUD_2400    =  2400,
    CN_UART_BAUD_4800    =  4800,
    CN_UART_BAUD_9600    =  9600,
    CN_UART_BAUD_19200   =  19200,
    CN_UART_BAUD_38400   =  38400,
    CN_UART_BAUD_57600   =  57600,
    CN_UART_BAUD_115200  =  115200,
    CN_UART_BAUD_230400  =  230400,
    CN_UART_BAUD_460800  =  460800,
    CN_UART_BAUD_921600  =  921600,
};

// ��������λ
enum tagUartDataBits
{
    CN_UART_DATABITS_5  = 5,
    CN_UART_DATABITS_6  = 6,
    CN_UART_DATABITS_7  = 7,
    CN_UART_DATABITS_8  = 8,
    CN_UART_DATABITS_9  = 9
};

// ������żУ��
enum tagUartParity
{
    CN_UART_PARITY_NONE = 0,
    CN_UART_PARITY_ODD,
    CN_UART_PARITY_EVEN,
    CN_UART_PARITY_MARK,
    CN_UART_PARITY_SPACE
};

// ����ֹͣλ
enum tagUartStopBits
{
    CN_UART_STOPBITS_1 = 0,
    CN_UART_STOPBITS_1_5,
    CN_UART_STOPBITS_2
};

// ���ڴ�������ṹ��
struct COMParam
{
    enum tagUartBaud       BaudRate;
    enum tagUartDataBits   DataBits;
    enum tagUartParity     Parity;
    enum tagUartStopBits   StopBits;
}tagComParam;

//���ڴ�������,ʹ��MULPLEX��SENSINGBIT��16�����ص���23������
#define CN_UART_FIFO_OVER_ERR    (1<<16) //Ӳ���������������
#define CN_UART_BUF_OVER_ERR     (1<<17) //���ջ������������
#define CN_UART_HARD_COMM_ERR    (1<<18) //����Ӳ��ͨ�Ŵ���

//���ڹ���ģʽ���壺
#define CN_UART_GENERAL         0       //ͨ�ù���ģʽ������Ӧ���������
#define CN_UART_POLL            1       //��ѯʽ����������Ӳ���ǰ���232����485
                                        //���ӣ�ʵ�ʹ�����һ��һ��İ�˫��״̬

//�����豸���������������uart_ctrl������
#define CN_UART_SET_BAUD              (F_DEV_USER+0 )  //����uartBaud.
#define CN_UART_COM_SET               (F_DEV_USER+1 )  //���ô��ڲ���
#define CN_UART_HALF_DUPLEX_SEND      (F_DEV_USER+2 )  //��������
#define CN_UART_HALF_DUPLEX_RECV      (F_DEV_USER+3 )  //��������
#define CN_UART_COMPLETED_SEND        (F_DEV_USER+4 )  //��ɷ��͹���
#define CN_UART_RX_PAUSE              (F_DEV_USER+5 )  //��ͣ�������ݵ�������
#define CN_UART_RX_RESUME             (F_DEV_USER+6 )  //���¿�ʼ�������ݵ�������
#define CN_UART_RX_OVERTIME           (F_DEV_USER+7 )  //���ý��ճ�ʱʱ�䣬������
#define CN_UART_SEND_SOFT_LEVEL       (F_DEV_USER+8 )  //���ý����������������ˮƽ
#define CN_UART_RECV_HARD_LEVEL       (F_DEV_USER+9 )  //���ý���fifo����ˮƽ
#define CN_UART_SEND_HARD_LEVEL       (F_DEV_USER+10)  //���÷���fifo����ˮƽ
#define CN_UART_RECV_ERROR_EVTT       (F_DEV_USER+11)  //���ô��󵯳��¼�����
#define CN_UART_EN_RTS                (F_DEV_USER+12)  //ʹ��rts����
#define CN_UART_DIS_RTS               (F_DEV_USER+13)  //��ֹrts����
#define CN_UART_EN_CTS                (F_DEV_USER+14)  //ʹ��cts����
#define CN_UART_DIS_CTS               (F_DEV_USER+15)  //��ֹcts����
#define CN_UART_DMA_USED              (F_DEV_USER+16)  //ʹ��dma����
#define CN_UART_DMA_UNUSED            (F_DEV_USER+17)  //��ֹdma����
#define CN_UART_SEND_BUF              (F_DEV_USER+18)  //����������ֱ�ӷ���
#define CN_UART_RECV_BUF              (F_DEV_USER+19)  //����������ֱ�ӽ���

#ifdef __cplusplus
}
#endif

#endif //__UARTCTRL_H__



