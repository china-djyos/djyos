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
// =============================================================================
#include "stdio.h"
#include "djyos.h"
#include "int.h"
#include "blackbox.h"

#include "board-config.h"

/*
extern struct IntMasterCtrl  tg_int_global;

void (*user_systick)(u32 inc_ticks) = NULL;


void HardExp_ConnectSystick(void (*tick)(u32 inc_ticks))
{
    user_systick = tick;
}
*/

//void Exp_SystickTickHandler(void)
//{
//    tg_int_global.en_asyn_signal_counter = 1;
//    if(!DjyGetUpdateTickFlag())
//        DjyUpdateTicks(1);
//    else
//        DjySetUpdateTickFlag(false);
//    if(user_systick!=NULL)
//        user_systick(1);
//    tg_int_global.en_asyn_signal_counter = 0;
//}

void HardExp_Init(void)
{
}

enum EN_BlackBoxAction Exp_MemManageFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

enum EN_BlackBoxAction Exp_HardFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

enum EN_BlackBoxAction Exp_DebugFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

enum EN_BlackBoxAction Exp_BusFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

enum EN_BlackBoxAction Exp_UsageFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

bool_t  HardExp_Analysis(struct ExpThrowPara *parahead, u32 endian)
{
    return true;
}

bool_t  HardExp_Decoder(struct ExpThrowPara *parahead, u32 endian)
{
    return true;
}

bool_t HardExp_InfoDecoderInit(void)
{
    return true;
}

bool_t HardExp_Trap_c(uint32_t *regs)
{

}
