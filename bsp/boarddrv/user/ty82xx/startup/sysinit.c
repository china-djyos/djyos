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
// Õâ·ÝÊÚÈ¨Ìõ¿î£¬ÔÚÊ¹ÓÃÕß·ûºÏÏÂÁÐÌõ¼þµÄÇéÐÎÏÂ£¬ÊÚÓèÊ¹ÓÃÕßÊ¹ÓÃ¼°ÔÙÉ¢²¥±¾
// Èí¼þ°ü×°Ô­Ê¼Âë¼°¶þ½øÎ»¿ÉÖ´ÐÐÐÎÊ½µÄÈ¨Àû£¬ÎÞÂÛ´Ë°ü×°ÊÇ·ñ¾­¸Ä×÷½ÔÈ»£º
//
// 1. ¶ÔÓÚ±¾Èí¼þÔ´´úÂëµÄÔÙÉ¢²¥£¬±ØÐë±£ÁôÉÏÊöµÄ°æÈ¨Ðû¸æ¡¢±¾Ìõ¼þÁÐ±í£¬ÒÔ
//    ¼°ÏÂÊöµÄÃâÔðÉùÃ÷¡£
// 2. ¶ÔÓÚ±¾Ì×¼þ¶þ½øÎ»¿ÉÖ´ÐÐÐÎÊ½µÄÔÙÉ¢²¥£¬±ØÐëÁ¬´øÒÔÎÄ¼þÒÔ¼°£¯»òÕßÆäËû¸½
//    ÓÚÉ¢²¥°ü×°ÖÐµÄÃ½½é·½Ê½£¬ÖØÖÆÉÏÊöÖ®°æÈ¨Ðû¸æ¡¢±¾Ìõ¼þÁÐ±í£¬ÒÔ¼°ÏÂÊö
//    µÄÃâÔðÉùÃ÷¡£

// ÃâÔðÉùÃ÷£º±¾Èí¼þÊÇ±¾Èí¼þ°æÈ¨³ÖÓÐÈËÒÔ¼°¹±Ï×ÕßÒÔÏÖ×´£¨"as is"£©Ìá¹©£¬
// ±¾Èí¼þ°ü×°²»¸ºÈÎºÎÃ÷Ê¾»òÄ¬Ê¾Ö®µ£±£ÔðÈÎ£¬°üÀ¨µ«²»ÏÞÓÚ¾ÍÊÊÊÛÐÔÒÔ¼°ÌØ¶¨Ä¿
// µÄµÄÊÊÓÃÐÔÎªÄ¬Ê¾ÐÔµ£±£¡£°æÈ¨³ÖÓÐÈË¼°±¾Èí¼þÖ®¹±Ï×Õß£¬ÎÞÂÛÈÎºÎÌõ¼þ¡¢
// ÎÞÂÛ³ÉÒò»òÈÎºÎÔðÈÎÖ÷Òå¡¢ÎÞÂÛ´ËÔðÈÎÎªÒòºÏÔ¼¹ØÏµ¡¢ÎÞ¹ýÊ§ÔðÈÎÖ÷Òå»òÒò·ÇÎ¥
// Ô¼Ö®ÇÖÈ¨£¨°üÀ¨¹ýÊ§»òÆäËûÔ­ÒòµÈ£©¶øÆð£¬¶ÔÓÚÈÎºÎÒòÊ¹ÓÃ±¾Èí¼þ°ü×°Ëù²úÉúµÄ
// ÈÎºÎÖ±½ÓÐÔ¡¢¼ä½ÓÐÔ¡¢Å¼·¢ÐÔ¡¢ÌØÊâÐÔ¡¢³Í·£ÐÔ»òÈÎºÎ½á¹ûµÄËðº¦£¨°üÀ¨µ«²»ÏÞ
// ÓÚÌæ´úÉÌÆ·»òÀÍÎñÖ®¹ºÓÃ¡¢Ê¹ÓÃËðÊ§¡¢×ÊÁÏËðÊ§¡¢ÀûÒæËðÊ§¡¢ÒµÎñÖÐ¶ÏµÈµÈ£©£¬
// ²»¸ºÈÎºÎÔðÈÎ£¬¼´ÔÚ¸ÃÖÖÊ¹ÓÃÒÑ»ñÊÂÇ°¸æÖª¿ÉÄÜ»áÔì³É´ËÀàËðº¦µÄÇéÐÎÏÂÒàÈ»¡£
// -----------------------------------------------------------------------------
// =============================================================================
// Copyright (C) 2012-2020 ³¤Ô°¼Ì±£×Ô¶¯»¯ÓÐÏÞ¹«Ë¾ All Rights Reserved
// ÎÄ¼þÃû     £ºsysinit.c
// Ä£¿éÃèÊö: CPUÊ±ÖÓµÄ³õÊ¼»¯ºÍÆ¬ÄÚÆ¬ÍâRAMµÈµÄ³õÊ¼»¯
// Ä£¿é°æ±¾: V1.00
// ´´½¨ÈËÔ±: HM
// ´´½¨Ê±¼ä: 08/13.2015
// =============================================================================
#include "stdint.h"
#include "cpu_peri.h"
#include "arch_feature.h"
#include "mpu.h"


// =============================================================================
// ATMEL æ—¶é’Ÿé…ç½®ï¼ŒOSC = 10MHz,PLLA = 10*(0x1D+1)=300M, HCLK300Mï¼Œå¤–è®¾æ—¶é’Ÿ150M Hz
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
// ATMEL Ê±ÖÓÅäÖÃ£¬OSC = 10MHz,PLLA = 10*(0x1D+1)=300M, HCLK300M£¬ÍâÉèÊ±ÖÓ150M Hz
/* Clock Settings (300MHz HCLK, 150MHz MCK)=> PRESC = 1, MDIV = 2 */
//      ±¾º¯ÊýµÄÊ±ÖÓÉèÖÃ£¬±ØÐëÓëboard-config.hÖÐµÄCN_CFG_MCLKµÈ³£Á¿¶¨ÒåÒ»ÖÂ¡£
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








