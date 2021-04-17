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
//����ģ��: �͹���ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:ʵ�ֵ͹���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-06-24
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __LOWPOWER_H__
#define __LOWPOWER_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "djyos.h"

//�����㷨:
//1.������,Ĭ��ִ��L0���͹���.��ģʽ��ȫ��Ӱ���������.
//2.Ӧ�ó��򲻿����������ú�������͹��ĵĺ���.
//3.Ӧ�ó�����Ե���api�����͹��ļ���.
//4.Ӧ�ó�����Ե���SaveLPL-RestoreLPL��������ͣ�߼��͹���ģʽ,���ص�Ĭ�ϵ�L0ģʽ.
//5.�͹���ģʽͳһ��ϵͳ�����¼�Ϊ���

//�͹��ļ�����:
//�ر�ע��,����Ӳ������,��Щ������ĳЩӲ���¿��ܻ��ص�,������stm32��cortex-mϵ��
//mcu��,L0��L1���ǵ�ͬ��(�û��Ļص��������ܲ�һ��).
//�͹��Ŀ����Ǹ߼�����,�ڱ�д�ص�����ǰ,����ǳ��˽��Լ���Ӳ��ϵͳ.
//L0.ÿ��IDLE�¼�ִ��,��ִ��ջ���,������ǳ������,һ�����ж�,������tick�ж�,
//   �������Ѽ���ִ�С�
//L1.ÿ��IDLE�¼�ִ��,��ִ��ջ���,�������˯��,���������ڴ�,tickʱ����������,
//   һ�����ж�,������tick�ж�,�������Ѽ���ִ��,��������ʱ����ܳ���L0��
//   ע:�����CPU��˵,L0��L1��������ͬ��,����Щ����Ҳ��������
//L2.ÿ��IDLE�¼�ִ��,��ִ��ջ���,�������˯��,�����ڴ�,ʱ��ֹͣ,���ж���
//   ����CPU,���Ѻ�,�����ж�,Ȼ���������,ֱ����һ�ν���͹���״̬��
//L3.ÿ��IDLE�¼�ִ��,��ִ��ջ���,�������˯��,�ڴ����,ʱ��ֹͣ,���ûص�����,��ȫ
//   ���ڴ����ݱ��浽����ʧ�洢����.��λ����CPU��,��������,�ѱ��浽����ʧ�洢���е�����
//   �ָ���RAM,Ȼ��������С�
//L4.ÿ��IDLE�¼�ִ��,��ִ��ջ���,�������˯��,�ڴ����,ʱ��ֹͣ,Ӳ������CPU��,����
//   �������������С�

#define CN_SLEEP_NORMAL    0       //���߷�ʽ:����
#define CN_SLEEP_ERROR     0xff    //���߷�ʽ:����
#define CN_SLEEP_L0        1       //���߷�ʽ:L0
#define CN_SLEEP_L1        2       //���߷�ʽ:L1
#define CN_SLEEP_L2        3       //���߷�ʽ:L2
#define CN_SLEEP_L3        4       //���߷�ʽ:L3
#define CN_SLEEP_L4        5       //���߷�ʽ:L4

struct LowPowerCtrl;

//���º�������bsp��ʵ��,���ݾ���Ӳ��,����ú��������й�,����bsp\boarddrv��ʵ��,
//�����cpu�й�,����bsp\cpudrvĿ¼�ṩ,��ֲ��ϵͳʱ����.
//���͹�����ص�Ӳ����ʼ��
bool_t __LP_BSP_HardInit(void);

// ����Ӳ��ģʽ�����ڵ͹���Ӳ���廨���ţ�������٣�����BSP�ṩһ��ģʽ���ú����������Ĺ���
// ��BSP�涨������ֻ�����ӿڣ�APP��ѭBSP������ֲ�ʹ�ñ�������
bool_t __LP_BSP_SetSleepMode(ptu32_t param);

// ����boot����ñ�������֪�Ǵ�L3���Ǵ�L4����,��������������.
// ����ֵ��ΪCN_WAKEUP_NORMAL��CN_WAKEUP_ERROR�� CN_WAKEUP_L3��CN_WAKEUP_L4֮һ��
// ������������ص�startup��.
u32 __LP_BSP_GetSleepLevel(void);
//��SleepLevel���浽ר��Ӳ������flash��,ֻ��L3��L4��Ҫ.
bool_t __LP_BSP_SaveSleepLevel(u32 SleepLevel);    //����true=�ɹ�,false=ʧ��
//�ӷ���ʧ�洢���лָ�RAM,L3���͹���ʹ��,�������������ʧ��,ϵͳ��normal��ʽ����
bool_t __LP_BSP_RestoreRamL3(void);     //����true=�ɹ��ָ�,false=ʧ��
//��RAM�洢������ʧ�洢����,L3���͹���ʹ��,�������false,ϵͳ������������
bool_t __LP_BSP_SaveRamL3(void);     //����true=�ɹ�����,false=ʧ��

void __LP_BSP_EntrySleepL0(u32 pend_ticks);
void __LP_BSP_EntrySleepL1(u32 pend_ticks);
void __LP_BSP_EntrySleepL2(u32 pend_ticks);
void __LP_BSP_EntrySleepL3(void);
void __LP_BSP_EntrySleepL4(void);

//��cps.S��ʵ��,����L3���͹���ǰ����,���������ġ�
void __LP_BSP_AsmSaveReg(struct ThreadVm *running_vm,
                            bool_t (*SaveRamL3)(void),
                            void (*EntrySleepLevel)(u8 sleep_level, u32 pend_ticks));

bool_t __LP_BSP_SetSleepMode(ptu32_t param);
//��ֹsleep,ʹDisableCounter+1
u32 LP_DisableSleep(void);
//ʹ��sleep,ʹDisableCounter-1,�ﵽ0������sleep.�ڽ�ֹ״̬��,ֻ����CN_SLEEP_L0
//���͹���
u32 LP_EnableSleep(void);
// ���ý�������ʱ������ģʽ,���ñ�������,������������,����Ҫ�ȴ�ϵͳ�����¼�ִ
// ��,������û�б���ֹʱ,�Ż������Ӧ������ģʽ
u32 LP_SetSleepLevel(u32 Level);
u32 LP_GetSleepLevel(void);
void LP_SetTriggerTick(u32 tick);
u32 LP_GetTriggerTick(void);
u32 LP_EntryLowPower(struct ThreadVm *vm,u32 pend_ticks);
void LP_SetHook(u32 (*EntrySleepReCall)(u32 SleepLevel),
                u32 (*ExitSleepReCall)(u32 SleepLevel));
//ע��͹������
void Register_LowPower_Function (void (*EntrySleepLevel)(u8 sleep_level, u32 pend_ticks),
                                 bool_t (*SaveSleepLevel)(u32 SleepLevel),
                                 bool_t (*SaveRamL3)(void),
                                 void (*AsmSaveReg)(struct ThreadVm *running_vm,
                                                     bool_t (*SaveRamL3)(void),
                                                     void (*EntrySleepLevel)(u8 sleep_level, u32 pend_ticks)));

#ifdef __cplusplus
}
#endif

#endif //__LOWPOWER_H__



