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
// Copyright (c) 2014 ����Ȩ�����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
// ģ������: ��ʱ��Ӳ���ӿڲ����ݶ����Լ��ӿ�����
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 2:39:26 PM/Apr 1, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע

#ifndef TIMER_HARD_H_
#define TIMER_HARD_H_

#include "stdint.h"

#define CN_TIMER_ENCOUNT     ((u32)(1<<0))//��������
#define CN_TIMER_ENINT       ((u32)(1<<1))//���������쳣
#define CN_TIMER_REALINT     ((u32)(1<<2))//ʵʱ�ź�
#define CN_TIMER_RELOAD      ((u32)(1<<3))//�Ƿ�reload
#define CN_TIMER_ENUSE       ((u32)(1<<10))//����ʹ��

//TIMER�Ĳ�����
enum HardTimerCmdCode
{
    EN_TIMER_STARTCOUNT,    //ʹ�ܼ�����inoutpara������
    EN_TIMER_PAUSECOUNT,    //ֹͣ������inoutpara������
    EN_TIMER_SETCYCLE,      //�������ڣ�inoutparaΪu32,�����õ����ڣ�����ֵ��
    EN_TIMER_SETRELOAD,     //reloadģʽor not����inoutparaΪbool_t,true����reload
    EN_TIMER_ENINT,         //�ж�ʹ�ܣ�inoutpara������
    EN_TIMER_DISINT,        //�жϽ�ֹ��inoutpara������
    EN_TIMER_SETINTPRO,     //�ж��������ã�inoutparaΪbool_t,true����ʵʱ�ź�
    EN_TIMER_GETTIME,       //��ȡ��һ����������ڵļ���ֵ��inoutparaΪu32 *
    EN_TIMER_GETCYCLE,      //��ȡ��ʱ���ڣ�inoutparaΪu32 *,��λΪ������
    EN_TIMER_GETID,         //��ȡ��ʱ��ID��inoutparaΪu32 *����16λΪintID����16ΪtimerID
    EN_TIMER_GETSTATE,      //��ȡ��ʱ��״̬��inoutparaΪu32 *
    EN_TIMER_LASTOPCODE,
};

typedef u32 (*fntTimerIsr)(ptu32_t TimerHandle);

//��ע�ᶨʱ��ʱ����Ҫʹ����������ԭ��
// =============================================================================
// �������ܣ�fnHardTimerAlloc
//           ��ʱ������
// ���������
//          cycle, ָ�����䶨ʱ���Ķ�ʱ���ڣ������Կ���ʹ��API�������и����趨����λΪ΢�룩
//          timerisr,����Ķ�ʱ�����жϷ�����,�ж��е���
// �����������
// ����ֵ  ��NULL ���䲻�ɹ������򷵻ض�ʱ��������þ���Ľṹ�ɶ�ʱ��оƬ�Լ�����
// ˵��    :��һ����Ըշ���Ķ�ʱ�������ʱֹͣ���жϽ�ֹ������оƬ����ʵ��
//           �տ�ʼ����Ķ�ʱ��Ӧ���Ǹ������Զ��رյģ�������Ա����Լ���������
//           Ĭ�ϣ�ֹͣ�������첽�жϣ�reload,�жϽ�ֹ
// =============================================================================
typedef ptu32_t (*fntHardTimerAlloc)(fntTimerIsr timerisr);
// =============================================================================
// �������ܣ�fnHardTimerFree
//           ��ʱ���ͷ�
// ���������timerhandle�����ͷŶ�ʱ��
// �����������
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
typedef bool_t  (*fntHardTimerFree)(ptu32_t timerhandle);
// =============================================================================
// �������ܣ���鶨ʱ������Ƶ��
// ���������timerhandle�������Ķ�ʱ��
// �����������
// ����ֵ  ��Ƶ�ʣ�HzΪ��λ
// =============================================================================
typedef u32  (*fntHardTimerGetFreq)(ptu32_t timerhandle);
// =============================================================================
// �������ܣ�fnHardTimerCtrl
//           ������ʱ��
// ���������timerhard�������ö�ʱ����
//           ctrlcmd, ��������
// ���������inoutpara������������������ݲ�ͬ���������
// ����ֵ  ��true �����ɹ� false����ʧ��
// ˵����ctrlcmd��Ӧ��inoutpara�����Զ���˵���ο�enum HeadTimerCmdCode����
// =============================================================================
typedef bool_t  (*fntHardTimerCtrl)(ptu32_t timerhandle,\
                                   enum HardTimerCmdCode ctrlcmd, \
                                   ptu32_t inoutpara);

struct TimerChip
{
    char               *chipname;        //chip���֣�����Ϊ��̬����
    fntHardTimerAlloc  HardTimerAlloc;   //���䶨ʱ��
    fntHardTimerFree   HardTimerFree;    //�ͷŶ�ʱ��
    fntHardTimerGetFreq HardTimerGetFreq; //��ȡ��ʱ������Ƶ��
    fntHardTimerCtrl   HardTimerCtrl;    //���ƶ�ʱ��
};

bool_t  HardTimer_RegisterChip(struct TimerChip *timerchip);
bool_t  HardTimer_UnRegisterChip(void);
ptu32_t HardTimer_Alloc(fntTimerIsr timerhardisr);
bool_t  HardTimer_Free(ptu32_t timerhandle);
u32  HardTimer_GetFreq(ptu32_t timerhandle);
bool_t  HardTimer_Ctrl(ptu32_t timerhandle, \
                       enum HardTimerCmdCode ctrlcmd, \
                       ptu32_t inoutpara);

#endif /* TIMER_HARD_H_ */