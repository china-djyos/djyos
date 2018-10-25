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
//����ģ��:�ж�ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: cm3�쳣����ģ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-08-30
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��:
//------------------------------------------------------
#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "board-config.h"

#include "blackbox.h"

#define CN_EXP_TYPE_UNDEF_CODE      (CN_BLACKBOX_TYPE_HARD_START+0)
#define CN_EXP_TYPE_PREFETCH        (CN_BLACKBOX_TYPE_HARD_START+1)
#define CN_EXP_TYPE_DATA_ABORT      (CN_BLACKBOX_TYPE_HARD_START+2)

extern u8 usr_top[];
extern u8 usr_bottom[];
extern u8 svc_top[];
extern u8 svc_bottom[];
extern u8 irq_top[];
extern u8 irq_bottom[];
extern u8 fiq_top[];
extern u8 fiq_bottom[];
extern u8 undef_top[];
extern u8 undef_bottom[];
extern u8 abort_top[];
extern u8 abort_bottom[];

extern void (* engine_irq)(ufast_t ufl_line);


//�쳣�������ƺͱ�־
#define CN_HARDEXP_DECODERNAME          "HARDEXP_DECODER"
#define CN_SYS_EXP_CPUINFO_VALIDFLAG    ((u32)(0x87654321))

//���쳣��Ϣ������CPU ��CORE ��Ϣ������CORE���У��;���CPU�ͺ��޹�
struct SysExpCoreInfo
{
    u32 spr_sp;
    u32 ger_r4;
    u32 ger_r5;
    u32 ger_r6;
    u32 ger_r7;
    u32 ger_r8;
    u32 ger_r9;
    u32 ger_r10;
    u32 ger_r11;    //r4-r11��������ջҲ����

    u32 ger_r0;
    u32 ger_r1;
    u32 ger_r2;
    u32 ger_r3;
    u32 ger_r12;
    u32 spr_lr;
    u32 spr_pc;
    u32 spr_xpsr;   //r0-r3,r12-r15���Զ���ѹջ
};

struct SysExpRegInfo
{
    u32 fpu_s16;
    u32 fpu_s17;
    u32 fpu_s18;
    u32 fpu_s19;
    u32 fpu_s20;
    u32 fpu_s21;
    u32 fpu_s22;
    u32 fpu_s23;
    u32 fpu_s24;
    u32 fpu_s25;
    u32 fpu_s26;
    u32 fpu_s27;
    u32 fpu_s28;
    u32 fpu_s29;
    u32 fpu_s30;
    u32 fpu_s31;
    struct SysExpCoreInfo CoreInfo;
    u32 fpu_s0;
    u32 fpu_s1;
    u32 fpu_s2;
    u32 fpu_s3;
    u32 fpu_s4;
    u32 fpu_s5;
    u32 fpu_s6;
    u32 fpu_s7;
    u32 fpu_s8;
    u32 fpu_s9;
    u32 fpu_s10;
    u32 fpu_s11;
    u32 fpu_s12;
    u32 fpu_s13;
    u32 fpu_s14;
    u32 fpu_s15;
    u32 fpu_fpscr;
    u32 fpu_res;
};

struct SysExceptionInfo
{
    u32 SysExpType;                     //�쳣��Ϣ����
    u32 SysExpCpuFlag;                 //���쳣��Ϣ����Ч����
    struct SysExpRegInfo ExpRegInfo;
    //struct SysExpNvicInfo NvicInfo;
};

extern u32 g_u32ExpTable[16];
void HardExp_Init(void);
void HardExp_ConnectSystick(void (*tick)(u32 inc_ticks));

#ifdef __cplusplus
}
#endif

#endif // _EXCEPTION_H_

