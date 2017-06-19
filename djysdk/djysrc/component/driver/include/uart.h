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
#ifndef __UART_H__
#define __UART_H__
#include "driver.h"
#include "ring.h"
#include <uartctrl.h>
#ifdef __cplusplus
extern "C" {
#endif
struct UartCB;

typedef u32 (* UartStartSend)(ptu32_t PrivateTag,u32 timeout);
//typedef u32 (* UartDirectSend)(ptu32_t PrivateTag,u8 *send_buf,u32 len,u32 timeout);
typedef ptu32_t (*UartControl)(ptu32_t PrivateTag,u32 cmd, u32 data1,u32 data2);

// ����ģ���ʼ���ṹ��
struct UartParam
{
    const char *Name;                   //UART���ƣ���UART0
    u32 TxRingBufLen;                   //���ͻ����������ֽ���
    u32 RxRingBufLen;                   //���ջ����������ֽ���
    u32 Baud;                           //Ĭ�ϵĲ�����
    ptu32_t UartPortTag;                //UART˽�б�ǩ����Ĵ�����ַ
    UartStartSend StartSend;            //�������ͻص�����ָ��
//    UartDirectSend DirectlySend;        //ֱ����ѯ���ͻص�����ָ��
    UartControl UartCtrl;               //���ƺ����ص�����ָ��
};

ptu32_t UART_AppWrite(struct UartCB *UCB,u8* src_buf,u32 len,
        u32 offset,bool_t block_option,u32 timeout);
ptu32_t UART_AppRead(struct UartCB *UCB,u8* dst_buf,u32 len,
        u32 offset,u32 timeout);
ptu32_t UART_PortWrite(struct UartCB *UCB,u8* buf,u32 len,u32 res);
ptu32_t UART_PortRead(struct UartCB *UCB,u8* dst_buf,u32 len,u32 res);
ptu32_t UART_ErrHandle(struct UartCB *UCB,u32 ErrNo);
ptu32_t UART_Ctrl(struct UartCB *UCB,u32 cmd,ptu32_t data1,ptu32_t data2);
bool_t UART_MultiplexAdd(struct UartCB *UCB,
                         struct MultiplexSetsCB *MultiplexSets,
                         struct DjyDevice *Dev,
                         u32 SensingBit);
struct UartCB *UART_InstallPort(struct UartParam *Param);
#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_UART_H__


