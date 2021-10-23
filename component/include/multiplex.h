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
//����ģ��: multiplexģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:ʵ�ֶ�·���õĹ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-06-24
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __MULTIPLEX_H__
#define __MULTIPLEX_H__
#include "lock.h"
#ifdef __cplusplus
extern "C" {
#endif

#define CN_MULTIPLEX_MAX_SETS      10
#define CN_MULTIPLEX_MAX_OBJECTS   1000

#define CN_MULTIPLEX_SUCCESS   0   //���óɹ�
#define CN_MULTIPLEX_INVALID   1   //����֧��Multiplex����
#define CN_MULTIPLEX_ERROR     2   //������Ȼ֧��Multiplex�����ܾ��˱��ε���


#define CN_MULTIPLEX_MODEMSK                0xff000000  //ģʽλ����
#define CN_MULTIPLEX_STATUSMSK              0x00ffffff  //״̬λ����

//struct MultiplexObjectCB��SensingBit��Ա�Ķ��壬��24��Sensing bit
//����Լ��bit16~23���û����壬����uart��CN_UART_FIFO_OVER_ERR
#define CN_MULTIPLEX_SENSINGBIT_READ        (1<<0)  //����ɶ�
#define CN_MULTIPLEX_SENSINGBIT_WRITE       (1<<1)  //�����д
#define CN_MULTIPLEX_SENSINGBIT_ERROR       (1<<2)  //�������
#define CN_MULTIPLEX_SENSINGBIT_USERBASE    (1<<16)  // bit16~23���û�����

//����ģʽ������SensingBit.24~31
//#define CN_MULTIPLEX_SENSINGBIT_ONESHOT     (1<<28) //1=ֻ����һ���¼���0=�������
#define CN_MULTIPLEX_SENSINGBIT_OR          (1<<29) //1=SensingBit����һ����λ�ʹ�����0=����ȫ����λ�Ŵ���
#define CN_MULTIPLEX_SENSINGBIT_ET          (1<<30) //1=���ش�����Edge����0=��ƽ������Level��

//��·���ö����Ƿ�actived������PendingBit.31
#define CN_MULTIPLEX_OBJECT_ACTIVED         (1<<31) //1=object is actived

struct MultiplexSetsCB;
struct MultiplexObjectCB;

struct MultiplexSetsCB *Multiplex_Create(u32 ActiveLevel);
bool_t Multiplex_AddObject(struct MultiplexSetsCB *Sets,s32 Fd, u32 SensingBit);
bool_t Multiplex_DelObject(struct MultiplexSetsCB  *Sets,s32 Fd);
s32 Multiplex_Wait( struct MultiplexSetsCB *Sets,u32 *Status,u32 Timeout);

#ifdef __cplusplus
}
#endif

#endif //__MULTIPLEX_H__


