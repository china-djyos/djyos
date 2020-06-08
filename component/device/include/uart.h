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
#ifndef __UART_H__
#define __UART_H__
#include "device.h"
#include "ring.h"
#include <uartctrl.h>
#ifdef __cplusplus
extern "C" {
#endif

struct UartGeneralCB;
struct UartPollCB;

typedef u32 (* UartStartSend)(ptu32_t PrivateTag);
typedef u32 (*UartSendPkg)(ptu32_t PrivateTag, u8 *buf, u32 len);
//typedef u32 (* UartDirectSend)(ptu32_t PrivateTag,u8 *send_buf,u32 len,u32 timeout);
typedef ptu32_t (*UartControl)(ptu32_t PrivateTag,u32 cmd, va_list *arg0);

// ����ģ���ʼ���ṹ��
struct UartParam
{
    const char *Name;                   //UART���ƣ���UART0
    u32 TxRingBufLen;                   //���ͻ����������ֽ���
    u32 RxRingBufLen;                   //���ջ����������ֽ���
    u32 Baud;                           //Ĭ�ϵĲ�����
    u8  mode;                           //���ڹ���ģʽ���μ� CN_UART_GENERAL ˵��
    ptu32_t UartPortTag;                //UART˽�б�ǩ����Ĵ�����ַ
    UartStartSend StartSend;            //�������ͻص�����ָ��
//    UartDirectSend DirectlySend;        //ֱ����ѯ���ͻص�����ָ��
    UartControl UartCtrl;               //���ƺ����ص�����ָ��
//    u8 mode;                          //0�����շ�ģʽ 1Ӧ��ģʽ
};

u32 UART_PortWrite(struct UartGeneralCB *pUGCB, u8 *pBuf, u32 dwLen);
u32 UART_PortRead(struct UartGeneralCB *pUGCB, u8 *pBuf, u32 dwLen);
u32 UART_ErrHandle(struct UartGeneralCB *pUGCB, u32 dwErrNo);

u32 UART_PollPortWrite(struct UartPollCB *pUPCB, u32 dwLen);
u32 UART_PollPortRead(struct UartPollCB *pUPCB);
u32 UART_PollErrHandle(struct UartPollCB *pUPCB, u32 dwErrNo);


#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_UART_H__


