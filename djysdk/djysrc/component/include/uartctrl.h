//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
#include "driver.h"
#include "ring.h"

#ifdef __cplusplus
extern "C" {
#endif
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

//���ڴ�������,ʹ��MULPLEX��SENSINGBIT��4�����ص���31������
#define CN_UART_FIFO_OVER_ERR    (1<<3) //Ӳ���������������
#define CN_UART_BUF_OVER_ERR     (1<<4) //���ջ������������
#define CN_UART_HARD_COMM_ERR    (1<<5) //����Ӳ��ͨ�Ŵ���

//�����豸���������������uart_ctrl������
#define CN_UART_START                 (CN_DRV_CTRL_USER+0 )  //��������
#define CN_UART_STOP                  (CN_DRV_CTRL_USER+1 )  //ֹͣ����
#define CN_UART_SET_BAUD              (CN_DRV_CTRL_USER+2 )  //����uartBaud.
#define CN_UART_COM_SET               (CN_DRV_CTRL_USER+3 )  //���ô��ڲ���
#define CN_UART_HALF_DUPLEX_SEND      (CN_DRV_CTRL_USER+4 )  //��������
#define CN_UART_HALF_DUPLEX_RECV      (CN_DRV_CTRL_USER+5 )  //��������
//#define CN_UART_SETTO_HALF_DUPLEX     (CN_DRV_CTRL_USER+6 )  //����Ϊȫ˫����ʽ
//#define CN_UART_SETTO_ALL_DUPLEX      (CN_DRV_CTRL_USER+7 )  //����Ϊȫ˫����ʽ
#define CN_UART_COMPLETED_SEND        (CN_DRV_CTRL_USER+8 )  //��ɷ��͹���
#define CN_UART_RX_PAUSE              (CN_DRV_CTRL_USER+9 )  //��ͣ�������ݵ�������
#define CN_UART_RX_RESUME             (CN_DRV_CTRL_USER+10)  //���¿�ʼ�������ݵ�������
#define CN_UART_RX_OVERTIME           (CN_DRV_CTRL_USER+11)  //���ý��ճ�ʱʱ�䣬������
#define CN_UART_RECV_SOFT_LEVEL       (CN_DRV_CTRL_USER+12)  //���ý����������������ˮƽ
#define CN_UART_SEND_SOFT_LEVEL       (CN_DRV_CTRL_USER+13)  //���ý����������������ˮƽ
#define CN_UART_RECV_HARD_LEVEL       (CN_DRV_CTRL_USER+14)  //���ý���fifo����ˮƽ
#define CN_UART_SEND_HARD_LEVEL       (CN_DRV_CTRL_USER+15)  //���÷���fifo����ˮƽ
#define CN_UART_RECV_ERROR_EVTT       (CN_DRV_CTRL_USER+16)  //���ô��󵯳��¼�����
#define CN_UART_EN_RTS                (CN_DRV_CTRL_USER+17)  //ʹ��rts����
#define CN_UART_DIS_RTS               (CN_DRV_CTRL_USER+18)  //��ֹrts����
#define CN_UART_EN_CTS                (CN_DRV_CTRL_USER+19)  //ʹ��cts����
#define CN_UART_DIS_CTS               (CN_DRV_CTRL_USER+20)  //��ֹcts����
#define CN_UART_DMA_USED              (CN_DRV_CTRL_USER+21)  //ʹ��dma����
#define CN_UART_DMA_UNUSED            (CN_DRV_CTRL_USER+22)  //��ֹdma����

#ifdef __cplusplus
}
#endif

#endif //__UARTCTRL_H__



