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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司�?有�?�著作权人保留一切权利�??
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然�?
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明�?
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或�?�其他附
//    于散播包装中的媒介方式，重制上述之版权宣告�?�本条件列表，以及下�?
//    的免责声明�??

// 免责声明：本软件是本软件版权持有人以及贡献�?�以现状�?"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就�?�售性以及特定目
// 的的适用性为默示性担保�?�版权持有人及本软件之贡献�?�，无论任何条件�?
// 无论成因或任何责任主义�?�无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生�?
// 任何直接性�?�间接�?��?�偶发�?��?�特殊�?��?�惩罚�?�或任何结果的损害（包括但不�?
// 于替代商品或劳务之购用�?�使用损失�?�资料损失�?�利益损失�?�业务中断等等）�?
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然�?
// -----------------------------------------------------------------------------
// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公�? All Rights Reserved
// 文件�?     ：sysinit.c
// 模块描述: CPU时钟的初始化和片内片外RAM等的初始�?
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 08/13.2015
// =============================================================================
#include "stdint.h"
#include "cpu_peri.h"
#include "arch_feature.h"
#include "mpu.h"

// =============================================================================
// ATMEL 时钟配置，OSC = 12MHz,PLLA = 300M, HCLK300M，外设时�?150M Hz
/* Clock Settings (300MHz HCLK, 150MHz MCK)=> PRESC = 1, MDIV = 2 */
#define SYS_BOARD_OSCOUNT   (CKGR_MOR_MOSCXTST(0x8U))
#define SYS_BOARD_PLLAR     (CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(0x18U) | \
                CKGR_PLLAR_PLLACOUNT(0x3fU) | CKGR_PLLAR_DIVA(0x1U))

#define SYS_BOARD_MCKR      (PMC_MCKR_PRES_CLK_1 | PMC_MCKR_CSS_PLLA_CLK \
                | PMC_MCKR_MDIV_PCK_DIV2)

// SDRAM寮曡剼�?�氫�?
/** List of all SDRAM pin definitions. */
#define BOARD_SDRAM_SIZE  (2*1024*1024)
#define PIN_SDRAM_D0_7    {0x000000FF, PIOC, ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_SDRAM_D8_13   {0x0000003F, PIOE, ID_PIOE, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_SDRAM_D14_15  {0x00018000, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_SDRAM_A0_9    {0x3FF00000, PIOC, ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_SDRAM_SDA10   {0x00002000, PIOD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT}

#define PIN_SDRAM_CAS     {0x00020000, PIOD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT}
#define PIN_SDRAM_RAS     {0x00010000, PIOD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT}
#define PIN_SDRAM_SDCKE   {0x00004000, PIOD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT}
#define PIN_SDRAM_SDCK    {0x00800000, PIOD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT}
#define PIN_SDRAM_SDSC    {0x00008000, PIOC, ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_SDRAM_NBS0    {0x00040000, PIOC, ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_SDRAM_NBS1    {0x00008000, PIOD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT}
#define PIN_SDRAM_SDWE    {0x20000000, PIOD, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT}
#define PIN_SDRAM_BA0     {0x00100000, PIOA, ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT}

#define BOARD_SDRAM_PINS PIN_SDRAM_D0_7, PIN_SDRAM_D8_13 , PIN_SDRAM_D14_15,\
        PIN_SDRAM_A0_9, PIN_SDRAM_SDA10, PIN_SDRAM_BA0, \
        PIN_SDRAM_CAS, PIN_SDRAM_RAS, PIN_SDRAM_SDCKE,PIN_SDRAM_SDCK,\
        PIN_SDRAM_SDSC,PIN_SDRAM_NBS0 ,PIN_SDRAM_NBS1,PIN_SDRAM_SDWE

#define memory_sync()        __DSB();__ISB();
// =============================================================================
// 功能：SDRAM初始化，包括GPIO和SDRAM控制器时钟及配置，与具体的控制的SDRAM相关，需根据�?
//       体的参数配置
// 参数：无
// 返回：无
// 说明：注意需要说明的是，SAMv7可能与SAM4s�? PMC地址并不兼容，其他的外设也需注意这点
// =============================================================================
void SDRAM_Init( void )
{
    const Pin pinsSdram[] = {BOARD_SDRAM_PINS};
    volatile uint32_t i;
    volatile uint8_t *pSdram = (uint8_t *) SDRAM_CS_ADDR;

    /* Configure PIO */
    PIO_Configure(pinsSdram, PIO_LISTSIZE(pinsSdram));
    PMC_EnablePeripheral(ID_SDRAMC);
    MATRIX->CCFG_SMCNFCS = CCFG_SMCNFCS_SDRAMEN;

    /* 1. SDRAM features must be set in the configuration register:
    asynchronous timings (TRC, TRAS, etc.), number of columns, rows,
    CAS latency, and the data bus width. */
    SDRAMC->SDRAMC_CR =
          SDRAMC_CR_NC_COL8      // 8 column bits
        | SDRAMC_CR_NR_ROW11     // 12 row bits (4K)
        | SDRAMC_CR_CAS_LATENCY3 // CAS Latency 3
        | SDRAMC_CR_NB_BANK2     // 2 banks
        | SDRAMC_CR_DBW          // 16 bit
        | SDRAMC_CR_TWR(4)
        | SDRAMC_CR_TRC_TRFC(11) // 63ns   min
        | SDRAMC_CR_TRP(5)       // Command period (PRE to ACT) 21 ns min
        | SDRAMC_CR_TRCD(5)      // Active Command to read/Write Command delay time 21ns min
        | SDRAMC_CR_TRAS(8)      // Command period (ACT to PRE)  42ns min
        | SDRAMC_CR_TXSR(13U);   // Exit self-refresh to active time  70ns Min

    /* 2. For mobile SDRAM, temperature-compensated self refresh (TCSR), drive
    strength (DS) and partial array self refresh (PASR) must be set in the
    Low Power Register. */

    /* 3. The SDRAM memory type must be set in the Memory Device Register.*/
    SDRAMC->SDRAMC_MDR = SDRAMC_MDR_MD_SDRAM;

    /* 4. A minimum pause of 200 娄脤s is provided to precede any signal toggle.*/
    for (i = 0; i < 100000; i++);

    /* 5. (1)A NOP command is issued to the SDRAM devices. The application must
    set Mode to 1 in the Mode Register and perform a write access to
    any SDRAM address.*/
    SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_NOP;
    *pSdram = 0;
    for (i = 0; i < 100000; i++);
    /* 6. An All Banks Precharge command is issued to the SDRAM devices.
    The application must set Mode to 2 in the Mode Register and perform a write
    access to any SDRAM address. */
    SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_ALLBANKS_PRECHARGE;
    *pSdram = 0;
    for (i = 0; i < 100000; i++);
    /* 7. Eight auto-refresh (CBR) cycles are provided. The application must
    set the Mode to 4 in the Mode Register and perform a write access to any
    SDRAM location eight times.*/
    for (i = 0 ; i< 8; i++) {
        SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_AUTO_REFRESH;
        *pSdram = 0;
    }
    for (i = 0; i < 100000; i++);
    /*8. A Mode Register set (MRS) cycle is issued to program the parameters of
    the SDRAM devices, in particular CAS latency and burst length. The
    application must set Mode to 3 in the Mode Register and perform a write
    access to the SDRAM. The write address must be chosen so that BA[1:0]
    are set to 0. For example, with a 16-bit 128 MB SDRAM (12 rows, 9 columns,
    4 banks) bank address, the SDRAM write access should be done at the address
    0x70000000.*/
    SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_LOAD_MODEREG;
    *pSdram = 0;

    for (i = 0; i < 100000; i++);
    /*9. For mobile SDRAM initialization, an Extended Mode Register set (EMRS)
    cycle is issued to program the SDRAM parameters (TCSR, PASR, DS). The
    application must set Mode to 5 in the Mode Register and perform a write
    access to the SDRAM. The write address must be chosen so that BA[1] or BA[0]
    are set to 1.
    For example, with a 16-bit 128 MB SDRAM, (12 rows, 9 columns, 4 banks) bank
    address the SDRAM write access should be done at the address 0x70800000 or
    0x70400000. */
    //SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_EXT_LOAD_MODEREG;
    // *((uint8_t *)(pSdram + SDRAM_BA0)) = 0;

    /* 10. The application must go into Normal Mode, setting Mode to 0 in the
    Mode Register and performing a write access at any location in the SDRAM. */
    SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_NORMAL;
    *pSdram = 0;
    for (i = 0; i < 100000; i++);
    /* 11. Write the refresh rate into the count field in the SDRAMC Refresh
    Timer register. (Refresh rate = delay between refresh cycles).
    The SDRAM device requires a refresh every 15.625 娄脤s or 7.81 娄脤s.
    With a 100 MHz frequency, the Refresh Timer Counter Register must be set
    with the value 1562(15.625 娄脤s x 100 MHz) or 781(7.81 娄脤s x 100 MHz). */
    // For IS42S16100E, 2048 refresh cycle every 32ms, every 15.625 娄脤s
    /* ((32 x 10(^-3))/2048) x150 x (10^6) */
    SDRAMC->SDRAMC_TR = 2343; ;
    SDRAMC->SDRAMC_CFR1 |= SDRAMC_CFR1_UNAL;
    /* After initialization, the SDRAM devices are fully functional. */
}

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
// 功能：系统时钟初始化，主时钟配置�?300MHz，外设时钟为150M
// 参数：无
// 返回：无
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
    if ( (SUPC->SUPC_SR & SUPC_SR_OSCSEL) != SUPC_SR_OSCSEL_CRYST )
    {
        SUPC->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_XTALSEL_CRYSTAL_SEL;

        while( !(SUPC->SUPC_SR & SUPC_SR_OSCSEL) );
    }

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








