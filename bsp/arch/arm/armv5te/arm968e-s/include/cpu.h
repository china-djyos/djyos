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
//����ģ��:������
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:�ں�����CPUֱ����صĴ���ĺ�������,OS�ڲ�ʹ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------


#ifndef __CPU_H__
#define __CPU_H__
#include "stdint.h"
#include "djyos.h"
#ifdef __cplusplus
extern "C" {
#endif

#define ADD_TO_TEST_DEBUG                __attribute__ ((section(".test_debug")))

#define _FIQ_STACK_SIZE_                  0xFF0
#define _IRQ_STACK_SIZE_                  0xFF0
#define _SVC_STACK_SIZE_                  0x3F0
#define _SYS_STACK_SIZE_                  0x3F0
#define _UND_STACK_SIZE_                  0x010
#define _ABT_STACK_SIZE_                  0x010

#define NOINT                             0xc0

#define BOOT_MODE_MASK                    0x1F
#define BOOT_MODE_USR                     0x10
#define BOOT_MODE_FIQ                     0x11
#define BOOT_MODE_IRQ                     0x12
#define BOOT_MODE_SVC                     0x13
#define BOOT_MODE_ABT                     0x17
#define BOOT_MODE_UND                     0x1B
#define BOOT_MODE_SYS                     0x1F
#define BOOT_FIQ_IRQ_MASK                 0xC0
#define BOOT_IRQ_MASK                     0x80

#define BOOT_COLOR_UNUSED                 0xAAAAAAAA      //Pattern to fill UNUSED stack
#define BOOT_COLOR_SVC                    0xBBBBBBBB      //Pattern to fill SVC stack
#define BOOT_COLOR_IRQ                    0xCCCCCCCC      //Pattern to fill IRQ stack
#define BOOT_COLOR_FIQ                    0xDDDDDDDD      //Pattern to fill FIQ stack
#define BOOT_COLOR_SYS                    0xEEEEEEEE      //Pattern to fill SYS stack

/*****************************/
/* CPU Mode                  */
/*****************************/
#define USERMODE        0x10
#define FIQMODE         0x11
#define IRQMODE         0x12
#define SVCMODE         0x13
#define ABORTMODE       0x17
#define UNDEFMODE       0x1b
#define MODEMASK        0x1f
#define NOINT           0xc0

void *__asm_reset_thread(ptu32_t (*thread_routine)(void),
                           struct ThreadVm  *vm);
void __asm_reset_switch(ptu32_t (*thread_routine)(void),
                           struct ThreadVm *new_vm,struct ThreadVm *old_vm);
void __asm_start_thread(struct ThreadVm  *new_vm);
void __asm_turnto_context(struct ThreadVm  *new_vm);
void __asm_switch_context(struct ThreadVm *new_vm,struct ThreadVm *old_vm);
void __asm_switch_context_int(struct ThreadVm *new_vm,struct ThreadVm *old_vm);

uint64_t __DjyGetTicks(void);
void DjyUpdateTicks(uint32_t ticks);
void DjySetUpdateTickFlag(bool_t flag);
bool_t DjyGetUpdateTickFlag(void);

#ifdef __cplusplus
}
#endif

#endif /*__CPU_H__*/

