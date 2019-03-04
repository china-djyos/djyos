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
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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

// �ļ���     ��cpu_peri_lptimer.c
// ģ������: �͹��Ķ�ʱ���ײ�Ӳ������ģ��
// ģ��汾:V1.0.0
// ������Ա:chj
// ����ʱ��:2018/6/14
// =============================================================================
#include "cpu_peri_lptimer.h"
#include "cpu_peri_int_line.h"
#include "stm32l433xx.h"
#include "int.h"
#include "board-config.h"

#if (CN_USE_TICKLESS_MODE)
#if CN_CFG_USE_USERTIMER
#define DJYBSP_LPTIMER_NUM      (1U)
static LPTIM_TypeDef *djybsp_lptimer_reg[DJYBSP_LPTIMER_NUM] = {LPTIM1};

#define DJYLPTIMER_DEBUG   (0U)
#if     DJYLPTIMER_DEBUG
struct djylptimer_debug_t{
    uint16_t pre_cnt;
    uint16_t pre_cmp;
}djylptimer_debug={
   0,
   0,
};
#endif

// =============================================================================
// ���ܣ����������жϱ�־λ
// ������Ҫ����Ķ�ʱ����
// ���أ���
// =============================================================================
void DjyBsp_LptimerClearAllIsrFlag(uint8_t id)
{
    djybsp_lptimer_reg[id]->ICR = 0x7f;
}

// =============================================================================
// ���ܣ������Ӧ���жϱ�־λ
// ������Ҫ����Ķ�ʱ����
// ���أ���
// =============================================================================
uint8_t DjyBsp_LptimerClearIsrFlag(uint8_t id)
{
    uint8_t temp = CN_LPTIMER_NONE;
    if(djybsp_lptimer_reg[id]->ISR & LPTIM_ISR_CMPM)
    {
        if(djybsp_lptimer_reg[id]->ISR & LPTIM_ISR_ARRM)
        {
            djybsp_lptimer_reg[id]->ICR |= LPTIM_ICR_ARRMCF | LPTIM_ICR_CMPMCF;
            temp =  CN_LPTIMER_RELOAD_AND_CMP;
        }
        else
        {
            djybsp_lptimer_reg[id]->ICR |= LPTIM_ICR_CMPMCF;
            temp =  CN_LPTIMER_CMP;
        }
    }
    else
    {
        if(djybsp_lptimer_reg[id]->ISR & LPTIM_ISR_ARRM)
        {
            djybsp_lptimer_reg[id]->ICR |= LPTIM_ICR_ARRMCF;
            temp =  CN_LPTIMER_RELOAD;
        }
        else
        {
            djybsp_lptimer_reg[id]->ICR = 0x7f;
            temp =  CN_LPTIMER_NONE;
        }
    }
    return temp;
}

// =============================================================================
// ���ܣ���ȡ�жϱ�־λ
// ��������ʱ����
// ���أ���
// =============================================================================
bool_t DjyBsp_LptimerGetIsrFlag(uint8_t id)
{
    return ((djybsp_lptimer_reg[id]->ISR&LPTIM_ISR_CMPM) ? true : false);
}

// =============================================================================
// ���ܣ����ö�ʱ����װ��ֵ
// ������id:��ʱ����,period:��Ҫ���õ�ֵ
// ���أ���
// =============================================================================
void DjyBsp_LptimerSetReload(uint8_t id,uint16_t period)
{
    uint16_t temp = 0;
    uint16_t temp1=0,temp2=1;
    while(temp1!=temp2)
    {
        temp1 = djybsp_lptimer_reg[id]->CNT;
        temp2 = djybsp_lptimer_reg[id]->CNT;
    }
#if     DJYLPTIMER_DEBUG
    djylptimer_debug.pre_cnt = temp1;
#endif
    temp = (period + temp1 > (CN_LIMIT_UINT16)) ? \
            (period + temp1 - CN_LIMIT_UINT16) : (period + temp1);
    djybsp_lptimer_reg[id]->ICR |= (LPTIM_ICR_CMPOKCF | LPTIM_ICR_CMPMCF);
    djybsp_lptimer_reg[id]->CMP = temp;
#if     DJYLPTIMER_DEBUG
    djylptimer_debug.pre_cmp = temp;
#endif
    while(!(djybsp_lptimer_reg[id]->ISR & LPTIM_ISR_CMPOK));
}

// =============================================================================
// ���ܣ���ȡ��ʱ����cntֵ
// ��������ʱ����
// ���أ���
// =============================================================================
uint16_t DjyBsp_LptimerReadCnt(uint8_t id)
{
    uint16_t temp1=0,temp2=1;
    while(temp1!=temp2)
    {
        temp1 = djybsp_lptimer_reg[id]->CNT;
        temp2 = djybsp_lptimer_reg[id]->CNT;
    }
    if(djybsp_lptimer_reg[id]->ISR & LPTIM_ISR_ARRM)
        return (CN_LIMIT_UINT16 + temp1);
    return temp1;
}

// =============================================================================
// ���ܣ���ʱ��Ԥ��ʼ���������и��ֱ�־λ�Ļ��������������壬ϵͳ�����Ժ�����
// ��������ʱ����
// ���أ���
// =============================================================================
void DjyBsp_LptimerPreInit(uint8_t id)
{
}

// =============================================================================
// ���ܣ���ʱ����ʼ������
// ������id:��ʱ����,reload:��װ��ֵ��isr:�жϺ���
// ���أ���
// =============================================================================
void DjyBsp_LptimerInit(uint8_t id,uint16_t reload,uint32_t (*isr)(ptu32_t param))
{
    uint32_t presc=0;
    uint32_t int_line = 0;

    presc = 32768/CN_CFG_TIME_BASE_HZ;
    switch(presc)
    {
        case 1:
            presc = (0x00 << (LPTIM_CFGR_PRESC_Pos));
            break;
        case 2:
            presc = LPTIM_CFGR_PRESC_0;
            break;
        case 4:
            presc = LPTIM_CFGR_PRESC_1;
            break;
        case 8:
            presc = (0x03 << (LPTIM_CFGR_PRESC_Pos));
            //presc = LPTIM_CFGR_PRESC_2;
            break;
        default:
            return;
    }
    switch(id)
    {
        case 0:
            RCC->APB1ENR1 |= RCC_APB1ENR1_LPTIM1EN | RCC_APB1ENR1_PWREN;
            RCC->CCIPR = RCC_CCIPR_LPTIM1SEL; // LPTIM1ʱ��ѡLSI
            int_line = CN_INT_LINE_LPTIM1;
            break;
        default:
            return;
    }

    djybsp_lptimer_reg[id]->CFGR = presc; // 4��Ƶ
    djybsp_lptimer_reg[id]->IER = LPTIM_IER_CMPMIE | LPTIM_IER_ARRMIE; // ����ʱ���ж�

    Int_Register(int_line);
    Int_SetClearType(int_line,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(int_line,isr);
    Int_SettoAsynSignal(int_line);
    Int_ClearLine(int_line);
    Int_RestoreAsynLine(int_line);

    djybsp_lptimer_reg[id]->CR = LPTIM_CR_ENABLE; // ��LPTIM, ���ݲ���ʼ��ʱ
    djybsp_lptimer_reg[id]->ARR = reload;
    djybsp_lptimer_reg[id]->CMP = reload;
    DjyBsp_LptimerClearAllIsrFlag(id);
    djybsp_lptimer_reg[id]->CR |= LPTIM_CR_CNTSTRT;
    while(djybsp_lptimer_reg[id]->CNT==0);

}
#endif
#endif

