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
// ģ������: ������Ź��Ľӿڶ���
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 3:48:11 PM/Dec 18, 2013
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע
#ifndef WDT_SOFT_H_
#define WDT_SOFT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "blackbox.h"

enum _EN_WDT_CMD
{
    EN_WDTCMD_ADD =0,
    EN_WDTCMD_DEL,
    EN_WDTCMD_CLEAN,
    //the following the api could use, but the up not
    EN_WDTCMD_PAUSE,
    EN_WDTCMD_RESUME,
    EN_WDTCMD_SET_OWNER,
    EN_WDTCMD_SET_CYCLE,                    //���ù�������
    EN_WDTCMD_SET_YIP_ACTION,               //���ù��ж���
    EN_WDTCMD_SET_HOOK,                     //���ù����ƺ���
    EN_WDTCMD_SET_CONSUME_LEVEL,            //���ù����߼�����ʱ�䴥��ˮƽ
    EN_WDTCMD_SET_SCHED_LEVEL,              //�������������߼����������޶�
    EN_WDTCMD_INVALID,                      //��Ч����
};

struct Wdt;
typedef struct Wdt tagWdt;
typedef u32 (* fnYipHook)(struct Wdt *wdt);

ptu32_t ModuleInstall_Wdt(void);
tagWdt *Wdt_Create(char *dogname,u32 yip_cycle,\
                   fnYipHook yiphook,
                   enum EN_BlackBoxAction yip_action,
                   u32 ExhaustLevelSet,
                   u32 ExhaustLimit);
tagWdt *Wdt_Create_s(tagWdt *wdt, char *dogname,u32 yip_cycle,
                     fnYipHook yiphook,
                     enum EN_BlackBoxAction yip_action,
                     u32 ExhaustLevelSet,
                     u32 ExhaustLimit);
bool_t Wdt_Delete(tagWdt *wdt);
bool_t Wdt_Delete_s(tagWdt *wdt);
bool_t Wdt_Clean(tagWdt *wdt);
bool_t Wdt_Ctrl(tagWdt *wdt, u32 type, ptu32_t para);
bool_t WdtHal_RegisterWdtChip(char *chipname, u32 yipcycle,\
                              bool_t (*wdtchip_feed)(void));

#ifdef __cplusplus
}
#endif

#endif //WDTSOFT_H_

