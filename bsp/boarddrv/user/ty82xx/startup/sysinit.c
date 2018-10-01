//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// -----------------------------------------------------------------------------
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��sysinit.c
// ģ������: CPUʱ�ӵĳ�ʼ����Ƭ��Ƭ��RAM�ȵĳ�ʼ��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 08/13.2015
// =============================================================================
#include "stdint.h"
#include "cpu_peri.h"
#include "arch_feature.h"
#include "mpu.h"


// =============================================================================
// ATMEL 时钟配置，OSC = 10MHz,PLLA = 10*(0x1D+1)=300M, HCLK300M，外设时钟150M Hz
/* Clock Settings (300MHz HCLK, 150MHz MCK)=> PRESC = 1, MDIV = 2 */
#define SYS_BOARD_OSCOUNT   (CKGR_MOR_MOSCXTST(0x8U))

#if (CN_CFG_MCLK == 300*Mhz)
#define SYS_BOARD_PLLAR     (CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(0x1DU) | \
                CKGR_PLLAR_PLLACOUNT(0x3fU) | CKGR_PLLAR_DIVA(0x1U))
#elif (CN_CFG_MCLK == 250*Mhz)
#define SYS_BOARD_PLLAR     (CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(24U) | \
                CKGR_PLLAR_PLLACOUNT(0x3fU) | CKGR_PLLAR_DIVA(0x1U))
#elif (CN_CFG_MCLK == 200*Mhz)
#define SYS_BOARD_PLLAR     (CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(19U) | \
                CKGR_PLLAR_PLLACOUNT(0x3fU) | CKGR_PLLAR_DIVA(0x1U))
#elif (CN_CFG_MCLK == 150*Mhz)
#define SYS_BOARD_PLLAR     (CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(14U) | \
                CKGR_PLLAR_PLLACOUNT(0x3fU) | CKGR_PLLAR_DIVA(0x1U))
#endif
#define SYS_BOARD_MCKR      (PMC_MCKR_PRES_CLK_1 | PMC_MCKR_CSS_PLLA_CLK \
                | PMC_MCKR_MDIV_PCK_DIV2)

void TCM_Disable(void)
{

    __DSB();
    __ISB();
    SCB->ITCMCR &= ~(uint32_t)SCB_ITCMCR_EN_Msk;
    SCB->DTCMCR &= ~(uint32_t)SCB_ITCMCR_EN_Msk;
    __DSB();
    __ISB();
}
// =============================================================================
// ATMEL ʱ�����ã�OSC = 10MHz,PLLA = 10*(0x1D+1)=300M, HCLK300M������ʱ��150M Hz
/* Clock Settings (300MHz HCLK, 150MHz MCK)=> PRESC = 1, MDIV = 2 */
//      ��������ʱ�����ã�������board-config.h�е�CN_CFG_MCLK�ȳ�������һ�¡�
// =============================================================================
void SysClk_Init(void)
{
    uint32_t read_MOR;

    EFC->EEFC_FCR = (EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FCMD_CGPB | EEFC_FCR_FARG(8));
    EFC->EEFC_FCR = (EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FCMD_CGPB | EEFC_FCR_FARG(7));

    TCM_Disable();

    /* Set FWS according to SYS_BOARD_MCKR configuration */
    EFC->EEFC_FMR = EEFC_FMR_FWS(5);

     /* Before switching MAIN OSC on external crystal : enable it and don't
     * disable at the same time RC OSC in case of if MAIN OSC is still using RC
     * OSC
     */

    read_MOR = PMC->CKGR_MOR;
     /* enable external crystal - enable RC OSC */
    read_MOR |= (CKGR_MOR_KEY_PASSWD |CKGR_MOR_XT32KFME);
    PMC->CKGR_MOR = read_MOR;

    /* Select XTAL 32k instead of internal slow RC 32k for slow clock */
//  if ( (SUPC->SUPC_SR & SUPC_SR_OSCSEL) != SUPC_SR_OSCSEL_CRYST )
//  {
//      SUPC->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_XTALSEL_CRYSTAL_SEL;
//
//      while( !(SUPC->SUPC_SR & SUPC_SR_OSCSEL) );
//  }

    /* Initialize main oscillator */
    if ( !(PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) )
    {
      PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | SYS_BOARD_OSCOUNT
                    | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;

      while ( !(PMC->PMC_SR & PMC_SR_MOSCXTS) )
      {
      }
    }

    /* Switch to 3-20MHz Xtal oscillator */
    PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | SYS_BOARD_OSCOUNT
                    | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCSEL;

    while ( !(PMC->PMC_SR & PMC_SR_MOSCSELS) )
    {
    }

    PMC->PMC_MCKR = (PMC->PMC_MCKR & ~(uint32_t)PMC_MCKR_CSS_Msk)
                    | PMC_MCKR_CSS_MAIN_CLK;

    while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) )
    {
    }

    /* Initialize PLLA */
    PMC->CKGR_PLLAR = SYS_BOARD_PLLAR;
    while ( !(PMC->PMC_SR & PMC_SR_LOCKA) )
    {
    }

    /* Switch to main clock */
    PMC->PMC_MCKR = (SYS_BOARD_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
    while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) )
    {
    }

    /* Switch to PLLA */
    PMC->PMC_MCKR = SYS_BOARD_MCKR;
    while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) )
    {
    }

//  SystemCoreClock = CHIP_FREQ_CPU_MAX;
}








