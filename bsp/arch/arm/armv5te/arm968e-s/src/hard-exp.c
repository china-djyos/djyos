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
#include "hard-exp.h"
#include "exp_vector.h"
#include "board-config.h"
#include "project_config.h"
void* g_u32ExpTable[8] __attribute__ ((section(".data.ExpTable")));

void Init_Cpu(void);
void AppStart(void);
void __irq_Int_EngineAll(ufast_t intStatus);
void __fiq_Int_EngineAll(ufast_t intStatus);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#if(CFG_RUNMODE == CN_RUNMODE_IBOOT)
void HardExp_UndefVector(ucpu_t *sp)
{
    return;
}
void HardExp_SwiVector(ucpu_t *sp)
{
    return;
}
void HardExp_PrefectVector(ucpu_t *sp)
{
    return;
}
void HardExp_DataAbortVector(ucpu_t *sp)
{
    return;
}
void HardExp_NotUseVector(ucpu_t *sp)
{
    return;
}
//-----------------------------------------------------------------------------
//���ܣ���ʼ���쳣������
//��������
//���أ���
//-----------------------------------------------------------------------------
void HardExp_Init(void)
{
    g_u32ExpTable[CN_VECTOR_RESET]      = (void*)Init_Cpu;
    g_u32ExpTable[CN_VECTOR_UND]        = (void*)HardExp_UndefVector;
    g_u32ExpTable[CN_VECTOR_SWI]        = (void*)HardExp_SwiVector;
    g_u32ExpTable[CN_VECTOR_PABT]       = (void*)HardExp_PrefectVector;
    g_u32ExpTable[CN_VECTOR_DABT]       = (void*)HardExp_DataAbortVector;
    g_u32ExpTable[CN_VECTOR_NOTUSE]     = (void*)HardExp_NotUseVector;
    g_u32ExpTable[CN_VECTOR_IRQ]        = (void*)__irq_Int_EngineAll;
    g_u32ExpTable[CN_VECTOR_FIQ]        = (void*)__fiq_Int_EngineAll;
}
#else
void HardExp_UndefVector(ucpu_t *sp)
{
    volatile u32 t = 1;
    EnJtag();
    if(t)
        while(1);
    return;
}
void HardExp_SwiVector(ucpu_t *sp)
{
    volatile u32 t = 1;
    EnJtag();
    if(t)
        while(1);
    return;
}
void HardExp_PrefectVector(ucpu_t *sp)
{
    volatile u32 t = 1;
    EnJtag();
    if(t)
        while(1);
    return;
}
void HardExp_DataAbortVector(ucpu_t *sp)
{
    volatile u32 t = 1;
    EnJtag();
    if(t)
        while(1);
    return;
}
void HardExp_NotUseVector(ucpu_t *sp)
{
    volatile u32 t = 1;
    EnJtag();
    if(t)
        while(1);
    return;
}
//-----------------------------------------------------------------------------
//���ܣ���ʼ���쳣������
//��������
//���أ���
//-----------------------------------------------------------------------------
void HardExp_Init(void)
{
#if(CFG_RUNMODE == CN_RUNMODE_BOOTSELF)
    g_u32ExpTable[CN_VECTOR_RESET]      = (void*)Init_Cpu;
#else
    g_u32ExpTable[CN_VECTOR_RESET]      = (void*)AppStart;
#endif
    g_u32ExpTable[CN_VECTOR_UND]        = (void*)HardExp_UndefVector;
    g_u32ExpTable[CN_VECTOR_SWI]        = (void*)HardExp_SwiVector;
    g_u32ExpTable[CN_VECTOR_PABT]       = (void*)HardExp_PrefectVector;
    g_u32ExpTable[CN_VECTOR_DABT]       = (void*)HardExp_DataAbortVector;
    g_u32ExpTable[CN_VECTOR_NOTUSE]     = (void*)HardExp_NotUseVector;
    g_u32ExpTable[CN_VECTOR_IRQ]        = (void*)__irq_Int_EngineAll;
    g_u32ExpTable[CN_VECTOR_FIQ]        = (void*)__fiq_Int_EngineAll;
}
#endif

bool_t HardExp_InfoDecoderInit(void)
{
    return true;
}
//
//bool_t HardExp_Trap_c(uint32_t *regs)
//{
//
//}
#pragma GCC diagnostic pop
