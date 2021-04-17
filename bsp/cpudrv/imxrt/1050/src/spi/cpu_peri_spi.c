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

// �ļ���     ��cpu_peri_spi.c
// ģ������: SPIģ��ײ�Ӳ������ģ��
// ģ��汾: V1.0.0
// ������Ա: chj
// ����ʱ��:2018/5/29
// =============================================================================
#include "string.h"
#include "board.h"
#include "spibus.h"
#include "heap.h"
#include "int.h"
#include "cpu_peri.h"
#include "cpu_peri_spi.h"
#include "cpu_peri_int_line.h"

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_lpspi.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_SPI(u8 Port);
//    #if (CFG_SPI1_ENABLE == 1)
//    ModuleInstall_SPI(CN_SPI1);
//    #endif
//    #if (CFG_SPI2_ENABLE == 1)
//    ModuleInstall_SPI(CN_SPI2);
//    #endif
//    #if (CFG_SPI3_ENABLE == 1)
//    ModuleInstall_SPI(CN_SPI3);
//    #endif
//    #if (CFG_SPI4_ENABLE == 1)
//    ModuleInstall_SPI(CN_SPI4);
//    #endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip spi"//SPI��������
//parent:"spi bus"   //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"spi bus","int"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:none           //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_SPI == false )
//#warning  " cpu_onchip_spi  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_SPI    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,32,512,
#define CFG_SPI_BUF_LEN              128   //"����SPI�����С",
//#define CFG_SPI_DMA_BUF_LEN        128   //"SPI��DMA����ߴ�",
//%$#@enum,true,false,
#define CFG_SPI1_ENABLE              false //"�Ƿ�����ʹ��SPI1",
#define CFG_SPI2_ENABLE              false //"�Ƿ�����ʹ��SPI2",
#define CFG_SPI3_ENABLE              false //"�Ƿ�����ʹ��SPI3",
#define CFG_SPI4_ENABLE              false //"�Ƿ�����ʹ��SPI4",
//%$#@string,1,30,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

#if !defined(LPSPI_CLK_SOURCE)
#define LPSPI_CLK_SOURCE         (1U) /* PLL3 PFD0 */
#endif
#if !defined(LPSPI_CLK_SOURCE_DIVIDER)
#define LPSPI_CLK_SOURCE_DIVIDER (8U) /* 8div */
#endif

#define EXAMPLE_LPSPI_CLOCK_FREQ (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (LPSPI_CLK_SOURCE_DIVIDER))

#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ     EXAMPLE_LPSPI_CLOCK_FREQ

#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */
#define EXAMPLE_LPSPI_MASTER_BASEADDR (LPSPI1)
#define CN_TIMEOUT  (200*1000)
typedef LPSPI_Type      tagSpiReg;

static tagSpiReg volatile * const tg_SpiReg[] ={
                                                 (tagSpiReg *)LPSPI1_BASE,
                                                 (tagSpiReg *)LPSPI2_BASE,
                                                 (tagSpiReg *)LPSPI3_BASE,
                                                 (tagSpiReg *)LPSPI4_BASE,
                                                };

//�����ж�����ʹ�õľ�̬���ṹ��
struct SPI_IntParamSet
{
    u32 SendDataLen;
    u32 RecvDataLen;
    u32 RecvOffset;
};

// =============================================================================
//����SPI���ƿ���жϾ�̬��
//#define CN_SPI_BUF_LEN              128
//#define CN_SPI_DMA_BUF_LEN          128
static struct SPI_CB *s_ptSpi1_CB;
static u8 s_Spi1_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset1;

static struct SPI_CB *s_ptSpi2_CB;
static u8 s_Spi2_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset2;

static struct SPI_CB *s_ptSpi3_CB;
static u8 s_Spi3_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset3;

static struct SPI_CB *s_ptSpi4_CB;
static u8 s_Spi4_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset4;

static lpspi_master_config_t g_masterConfig[CN_SPI_NUM];

#define TRANSFER_SIZE 64U

uint8_t masterRxData[TRANSFER_SIZE] = {0U};
uint8_t masterTxData[TRANSFER_SIZE] = {0U};

volatile uint32_t masterTxCount;
volatile uint32_t masterRxCount;
uint8_t g_masterRxWatermark;
uint8_t g_masterFifoSize;

__attribute__((weak))  bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level)
{
    return true;
}
__attribute__((weak))  void SPI_InLowPowerPinCfg(u8 port)
{
    return;
}
__attribute__((weak))  void SPI_OutLowPowerPinCfg(u8 port)
{
    return ;
}

// =============================================================================
// ����: �ж�ʹ�ܺ�ʧ�ܺ���
// ����: tpSPI,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __SPI_IntEnable(volatile tagSpiReg *Reg,u32 IntSrc)
{
    LPSPI_EnableInterrupts((tagSpiReg*)Reg, IntSrc);
}
static void __SPI_IntDisable(volatile tagSpiReg *Reg,u32 IntSrc)
{
    LPSPI_DisableInterrupts((tagSpiReg*)Reg, IntSrc);
}

// =============================================================================
// ���ܣ�SPIʱ�����ú�����
// ������spi_dev���豸���
//      Fre��������SCK��ʱ���ٶȣ���λΪHz
// ���أ���
// =============================================================================
static void __SPI_SetClk(volatile tagSpiReg *Reg,u32 Fre)
{
    u32 base = (u32)Reg;
    switch(base)
    {
        case LPSPI1_BASE:
                g_masterConfig[0].baudRate = Fre;
                break;
        case LPSPI2_BASE:
                g_masterConfig[1].baudRate = Fre;
                break;
        case LPSPI3_BASE:
                g_masterConfig[2].baudRate = Fre;
                break;
        case LPSPI4_BASE:
                g_masterConfig[3].baudRate = Fre;
                break;
        default:
                break;
    }
}

// =============================================================================
// ���ܣ�SPI���ƼĴ����������ã���PHA��CPOL��ʱ�ӵȣ����ݸ��ּĴ�������
// ������tpSPI,SPI��������ַ
//       ptr,����ָ��
// ���أ���
// =============================================================================
static void __SPI_Config(volatile tagSpiReg *Reg,tagSpiConfig *ptr)
{
    u8 port=0xff;
    u32 base=(u32)Reg;
    if(ptr->Freq > 40*1000*1000)
    {
        ptr->Freq = 40*1000*1000;
    }
    __SPI_SetClk(Reg,ptr->Freq);
    switch(base)
    {
        case LPSPI1_BASE:
                port=0;
                break;
        case LPSPI2_BASE:
                port=1;
                break;
        case LPSPI3_BASE:
                port=2;
                break;
        case LPSPI4_BASE:
                port=3;
                break;
        default:
                return;
    }

    if(ptr->CharLen!=8 && ptr->CharLen!=16 && ptr->CharLen!=32)
        return;

    //set the PHA
    if(ptr->Mode & SPI_CPHA)
    {
        g_masterConfig[port].cpha = kLPSPI_ClockPhaseSecondEdge;
    }
    else
    {
        g_masterConfig[port].cpha = kLPSPI_ClockPhaseFirstEdge;
    }
    //set the cpol
    if(ptr->Mode & SPI_CPOL)
    {
        g_masterConfig[port].cpol = kLPSPI_ClockPolarityActiveLow;
    }
    else
    {
        g_masterConfig[port].cpol = kLPSPI_ClockPolarityActiveHigh;
    }
    g_masterConfig[port].bitsPerFrame = ptr->CharLen;

    if(ptr->ShiftDir == SPI_SHIFT_LSB)
    {
        g_masterConfig[port].direction = kLPSPI_LsbFirst;
    }
    else
    {
        g_masterConfig[port].direction = kLPSPI_MsbFirst;
    }
}

static u32 Spi_GetFreq(void)
{
    u32 freq = 0;

    /* CLOCK_GetMux(kCLOCK_LpspiMux):
       00b: derive clock from PLL3 PFD1 720M
       01b: derive clock from PLL3 PFD0 720M
       10b: derive clock from PLL2      528M
       11b: derive clock from PLL2 PFD2 396M
    */
    switch(CLOCK_GetMux(kCLOCK_LpspiMux))
    {
    case 0:
        freq = CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk);
        break;

    case 1:
        freq = CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk);
        break;

    case 2:
        freq = CLOCK_GetFreq(kCLOCK_SysPllClk);
        break;

    case 3:
        freq = CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk);
        break;
    }

    freq /= (CLOCK_GetDiv(kCLOCK_LpspiDiv) + 1U);

    return freq;
}

// =============================================================================
// ����: SPIĬ��Ӳ����ʼ�����ã���Ҫ��ʱ�����ú�GPIOд������������
// ����: RegBaseAddr,�Ĵ�����ַ
// ����: ��
// =============================================================================
static void __SPI_HardConfig(u32 BaseAddr)
{
    u8 port=0;
    CLOCK_SetMux(kCLOCK_LpspiMux, LPSPI_CLK_SOURCE);
    CLOCK_SetDiv(kCLOCK_LpspiDiv, LPSPI_CLK_SOURCE_DIVIDER-1);

    switch(BaseAddr)
    {
        case LPSPI1_BASE:
                port=0;
                break;
        case LPSPI2_BASE:
                port=1;
                break;
        case LPSPI3_BASE:
                port=2;
                break;
        case LPSPI4_BASE:
                port=3;
                break;
        default:
                return;
    }
    //LPSPI_MasterGetDefaultConfig(&g_masterConfig[port]);

    g_masterConfig[port].baudRate = TRANSFER_BAUDRATE;
    g_masterConfig[port].bitsPerFrame = 8;
    g_masterConfig[port].cpol = kLPSPI_ClockPolarityActiveHigh;
   g_masterConfig[port].cpha = kLPSPI_ClockPhaseFirstEdge;
   g_masterConfig[port].direction = kLPSPI_MsbFirst;

   g_masterConfig[port].pcsToSckDelayInNanoSec = 1000000000 / g_masterConfig[port].baudRate;
   g_masterConfig[port].lastSckToPcsDelayInNanoSec = 1000000000 / g_masterConfig[port].baudRate;
   g_masterConfig[port].betweenTransferDelayInNanoSec = 1000000000 / g_masterConfig[port].baudRate;

   g_masterConfig[port].whichPcs = EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT;
   g_masterConfig[port].pcsActiveHighOrLow = kLPSPI_PcsActiveLow;

   g_masterConfig[port].pinCfg = kLPSPI_SdiInSdoOut;
   g_masterConfig[port].dataOutConfig = kLpspiDataOutRetained;

    LPSPI_MasterInit((tagSpiReg*)BaseAddr, &g_masterConfig[port], EXAMPLE_LPSPI_MASTER_CLOCK_FREQ);
    LPSPI_SetFifoWatermarks((tagSpiReg*)BaseAddr, 0, 0);
    //LPSPI_SetFifoWatermarks((tagSpiReg*)BaseAddr, txWatermark, g_masterRxWatermark);

    LPSPI_Enable((tagSpiReg*)BaseAddr, false);
    ((tagSpiReg*)BaseAddr)->CFGR1 &= (~LPSPI_CFGR1_NOSTALL_MASK);
    LPSPI_Enable((tagSpiReg*)BaseAddr, true);

    /*Flush FIFO , clear status , disable all the inerrupts.*/
    LPSPI_FlushFifo((tagSpiReg*)BaseAddr, true, true);
    LPSPI_ClearStatusFlags((tagSpiReg*)BaseAddr, kLPSPI_AllStatusFlag);
    LPSPI_DisableInterrupts((tagSpiReg*)BaseAddr, kLPSPI_AllInterruptEnable);

    ((tagSpiReg*)BaseAddr)->TCR =
            (((tagSpiReg*)BaseAddr)->TCR &
             ~(LPSPI_TCR_CONT_MASK | LPSPI_TCR_CONTC_MASK | LPSPI_TCR_RXMSK_MASK | LPSPI_TCR_PCS_MASK)) |
            LPSPI_TCR_CONT(0) | LPSPI_TCR_CONTC(0) | LPSPI_TCR_RXMSK(0) | LPSPI_TCR_TXMSK(0) | g_masterConfig[port].whichPcs;

}

// =============================================================================
// ���ܣ�SPIƬѡʹ�ܣ�ʹƬѡ��Ч,���������о��������Ƭѡ�Ĵ�������ֱ�ӷ���
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// ˵����SPI��������ֻ������һ��CS������Ҫ�Ӷ��CS�������Ӧ�ó����Լ�����CS
// =============================================================================
static bool_t __SPI_BusCsActive(tagSpiReg *Reg, u8 cs)
{
    u8 SPIPort;
    for(SPIPort = 0; SPIPort < CN_SPI_NUM; SPIPort++)
    {
        if(Reg == tg_SpiReg[SPIPort])
            break;
    }
//    switch(SPIPort)
//    {
//        case CN_SPI1:
//            GPIO_SettoLow(GPIO_F, PIN14);
//            break;
//        case CN_SPI2:
//
//            break;
//        case CN_SPI3:
//
//            break;
//        case CN_SPI4:
//
//            break;
//        case CN_SPI5:
//
//            break;
//        case CN_SPI6:
//
//            break;
//        default:
//            return false;
//    }
//    return true;
    return Board_SpiCsCtrl(SPIPort,cs,0);
}

// =============================================================================
// ���ܣ�SPIƬѡʧ�ܣ�ʹƬѡ��Ч
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// =============================================================================
static bool_t __SPI_BusCsInActive(tagSpiReg *Reg, u8 cs)
{
    u8 SPIPort;
    for(SPIPort = 0; SPIPort < CN_SPI_NUM; SPIPort++)
    {
        if(Reg == tg_SpiReg[SPIPort])
            break;
    }
//    switch(SPIPort)
//    {
//        case CN_SPI1:
//            GPIO_SettoHigh(GPIO_F, PIN14);
//            break;
//        case CN_SPI2:
//
//            break;
//        case CN_SPI3:
//
//            break;
//        case CN_SPI4:
//
//            break;
//        case CN_SPI5:
//
//            break;
//        case CN_SPI6:
//
//            break;
//        default:
//            return false;
//    }
//    return true;
    return Board_SpiCsCtrl(SPIPort,cs,1);
}

// =============================================================================
// ���ܣ�SPI���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�SPIʱ��Ƶ������
// ������specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
static s32 __SPI_BusCtrl(tagSpiReg *Reg,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    s32 result = 1;
    if( (Reg != tg_SpiReg[CN_SPI1]) && (Reg != tg_SpiReg[CN_SPI2])
         && (Reg != tg_SpiReg[CN_SPI3])  && (Reg != tg_SpiReg[CN_SPI4])
        )
        return 0;

    switch(cmd)
    {
        case CN_SPI_SET_CLK:
            __SPI_SetClk(Reg,data1);
            break;

        case CN_SPI_CS_CONFIG:
            __SPI_Config(Reg,(tagSpiConfig *)data1);
            break;

        case CN_SPI_SET_AUTO_CS_EN:
            // Ӳ����������Զ�����CS������
            break;

        case CN_SPI_SET_AUTO_CS_DIS:
            break;

        case CN_SPI_SET_POLL:
               break;

        case CN_SPI_SET_INT:
               break;
        default:        break;
    }
    return result;
}

// =============================================================================
// ���ܣ���SPI���ռĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ
// ���أ�����������
// =============================================================================
static s32 __SPI_Read( tagSpiReg * Reg )
{
    u32 time = CN_TIMEOUT;
    u32 readData=0;
    while ( ( LPSPI_GetRxFifoCount(Reg) == 0 ) && (time > 0))
    {
        time --;
        DJY_DelayUs(1);
    }

    if(time == 0)
        return -1;

    readData = LPSPI_ReadData(Reg);

    return readData;
}

// =============================================================================
// ���ܣ�дSPI���ͼĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ��wData��д����
// ���أ���
// =============================================================================
static s32 __SPI_Write( tagSpiReg * Reg,u8 wData )
{
    u32 time = CN_TIMEOUT;
    u32 fifoSize = LPSPI_GetTxFifoSize(Reg);
    /* Send data */
    while ( ( LPSPI_GetTxFifoCount(Reg) == fifoSize ) && (time > 0) )
    {
        time --;
        DJY_DelayUs(1);
    }
    if(time == 0)
        return -1;

    LPSPI_WriteData(Reg, wData);
    return 0;
}

// =============================================================================
// ���ܣ���ѯ��ʽ��д����
// ������Reg,�Ĵ�����ַ��srcAddr,����Դָ�룻wrSize��д��������destAddr������������
//       ��ַָ�룻rdSize������������recvoff,��ƫ�ƣ���δ��
// ���أ�true,��ȷ��false,����
// =============================================================================
static bool_t __SPI_TxRxPoll(tagSpiReg *Reg,u8 *srcAddr,u32 wrSize,
        u8 *destAddr, u32 rdSize,u32 recvoff,u8 cs)
{
    u32 i,len_limit;

    if( (!srcAddr) || ((rdSize != 0) && (!destAddr)))
        return false;

    len_limit = MAX(wrSize, rdSize + recvoff);

    for (i=0;i<len_limit;i++)
    {
        if (i<wrSize)
        {
            __SPI_Write(Reg,srcAddr[i]);
        }
        else if((i>=wrSize)&&(i<len_limit))
        {
          __SPI_Write(Reg,0);
        }

        if((destAddr) && (i>=recvoff))
        {
            destAddr[i-recvoff] = __SPI_Read(Reg);
        }
        else
        {
            __SPI_Read(Reg);
        }
    }

    return true;
}

// =============================================================================
// ���ܣ�SPI�ײ��������ͽ��պ�������������ɵĹ������£�
//       1.����ṹ��̬�����������ж���ʹ�ã�
//       2.������صļĴ�����ʹ�䴦�ڷ��ͽ�������״̬;
//       3.�����жϣ����жϷ���������������շ���
// ������specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       sendlen,�������ݳ��ȣ��ֽ�
//       recvlen,�������ݳ��ȣ��ֽ�
//       recvoff,����������ϡ�����ӽ��յ��ĵڼ����ֽڿ�ʼ�������ݣ��ֽ�
//       block_opt,����ѡ�trueʱ��Ϊ��������������ʽ��ײ������ͷ��ź���������CS
//       spi_semp,�ײ������Ҫ�ͷŵ��ź������Ƿ��ͷ���block_opt����
// ���أ�true,�޴���;false,ʧ��
// =============================================================================
static bool_t __SPI_TransferTxRx(tagSpiReg *Reg,u32 sendlen,u32 recvlen,
                                u32 recvoff)
{
    struct SPI_CB *pSCB=NULL;
    struct SPI_IntParamSet *Param=NULL;
    u32 fifo_size=0;
    u8 i=0;
    u8 ch=0,temp=0;

    if(Reg == tg_SpiReg[CN_SPI1])
    {
        pSCB = s_ptSpi1_CB;
        Param = &IntParamset1;
    }
    else if(Reg == tg_SpiReg[CN_SPI2])
    {
        pSCB = s_ptSpi2_CB;
        Param = &IntParamset2;
    }
    else if(Reg == tg_SpiReg[CN_SPI3])
    {
        pSCB = s_ptSpi3_CB;
        Param = &IntParamset3;
    }
    else if(Reg == tg_SpiReg[CN_SPI4])
    {
        pSCB = s_ptSpi4_CB;
        Param = &IntParamset4;
    }
    else
        return false;

    Param->SendDataLen = sendlen;
    Param->RecvDataLen = recvlen;
    Param->RecvOffset  = recvoff;
    fifo_size = LPSPI_GetTxFifoSize(Reg) - LPSPI_GetTxFifoCount(Reg);
    if(fifo_size>sendlen)//����������С��Ҫ���͵����ݻ���ʱ��û��Ҫ�������жϣ�ֱ����������Ｔ��
    {
        for(i=0;i<sendlen;i++)
        {
            temp = SPI_PortRead(pSCB,&ch,1);//�Ƿ���������Ҫ����
            if(temp>1)
            {
                __SPI_Write(Reg, ch);
                Param->SendDataLen --;
                Param->RecvOffset  --;
            }
            else
                return false;//��û���꣬�����Ѿ��ӻ������ж�����������
        }
        if(recvlen>0)
        {
            __SPI_IntEnable(Reg,kLPSPI_RxInterruptEnable);
            __SPI_Write(Reg, 0XAA);
        }
    }
    else
    {
        __SPI_IntEnable(Reg,kLPSPI_TxInterruptEnable|kLPSPI_WordCompleteInterruptEnable     \
                            |kLPSPI_FrameCompleteInterruptEnable|kLPSPI_TransferCompleteInterruptEnable);

        for(i=0;i<fifo_size;i++)
        {
            temp = SPI_PortRead(pSCB,&ch,1);//�Ƿ���������Ҫ����
            if(temp>1)
            {
                __SPI_Write(Reg, ch);
                Param->SendDataLen --;
                Param->RecvOffset  --;
            }
            else
                return false;//��û���꣬�����Ѿ��ӻ������ж�����������
        }//ϵͳ����뷢���жϣ���������ʣ�µ�
    }
    return true;
}

// =============================================================================
// ���ܣ�SPI�����뷢���жϷ��������ú���ʵ�ֵĹ������£�
//       1.�����շ�һ�壬��˷��Ϳ��ж���Ҳ�յ������ݣ�
//       2.SPI_PortRead()�ж���������ʱ��˵�����͵������Ѿ���ɣ��ɹط����ж�
//       3.��param->RecvDataLen����0������Ҫ�������ݣ���ӽ��յ���������offset�ֽ�
//         ��ʼ�洢���ݣ�������SPI_PortRead()
//       4.
// ������spi_int_line,�жϺţ�������û�õ�
// ���أ�������
// =============================================================================
u32 SPI_ISR(ptu32_t IntLine)
{
    struct SPI_CB *pSCB=NULL;
    struct SPI_IntParamSet *param;
    tagSpiReg *Reg;
    u32 sr=0;
    u32 fifo_size=0;
    u8 ch=0;
    s32 temp=0;

    static u8 firstrcv = 0;

    if(IntLine == CN_INT_LINE_LPSPI1)
    {
        pSCB = s_ptSpi1_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI1];
        param = &IntParamset1;
    }
    else if(IntLine == CN_INT_LINE_LPSPI2)
    {
        pSCB = s_ptSpi2_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI2];
        param = &IntParamset2;
    }
    else if(IntLine == CN_INT_LINE_LPSPI3)
    {
        pSCB = s_ptSpi3_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI3];
        param = &IntParamset3;
    }
    else if(IntLine == CN_INT_LINE_LPSPI4)
    {
        pSCB = s_ptSpi4_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI4];
        param = &IntParamset4;
    }
    else
        return 0;

    sr = LPSPI_GetStatusFlags(Reg);
    LPSPI_ClearStatusFlags(Reg, sr);
    if(sr&kLPSPI_TxDataRequestFlag)//�����ж�
    {
        fifo_size = LPSPI_GetTxFifoSize(Reg) - LPSPI_GetTxFifoCount(Reg);
        temp = SPI_PortRead(pSCB,&ch,fifo_size);//�Ƿ���������Ҫ����
        if(temp>0)
        {
            while(temp--)
            {
                param->SendDataLen --;
                param->RecvOffset  --;
                LPSPI_WriteData(Reg, ch);
            }
        }
        else                                            //�������ͽ���
        {
            //�ط����ж�
            __SPI_IntDisable(Reg,kLPSPI_TxInterruptEnable);
            param->RecvOffset -= param->SendDataLen;
            param->SendDataLen = 0;

            //��������ˣ��ж��Ƿ���Ҫ����
            if(param->RecvDataLen > 0)
            {
                __SPI_IntEnable(Reg,kLPSPI_RxInterruptEnable);    //ʹ�ܽ����ж�
                //MASTERģʽ�£����ͺͽ���ͬʱ����
                LPSPI_WriteData(Reg, 0xAA);
                firstrcv = 1;
            }
        }
    }
    else if(sr&kLPSPI_RxDataReadyFlag)    //�����ж�
    {
        ch=LPSPI_ReadData(Reg);
        //��������˶�������
        if(firstrcv != 1)
        {
            SPI_PortWrite(pSCB,&ch,1);
            param->RecvDataLen -= 1;
        }
        else
            firstrcv = 0;

        //�ж��Ƿ����������
        if(param->RecvDataLen > 0)
        {
            LPSPI_WriteData(Reg, 0xAA);
        }
        else                    //���������Ѿ����
        {
            param->RecvDataLen = 0;
            __SPI_IntDisable(Reg,kLPSPI_RxInterruptEnable);
        }
    }
    return 0;
}

// =============================================================================
// ����: SPI�ж����ú���
// ����: IntLine,�ж���
// ����: ��
// =============================================================================
static void __SPI_IntConfig(u32 IntLine)
{
    //�ж��ߵĳ�ʼ��
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,SPI_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// ���ܣ� ������˳��͹���ģʽ
//     ʱ��ʹ�� ������ʹ�� GPIO������Ӧ��״̬
// ������ port ���ںţ��� CN_SPI1
//     flag ����͹��� ���˳��͹��ı�־���� SPI_IN_LOWPOWER
// ���أ�true/false
// =============================================================================

bool_t SPI_LowPowerConfig(u8 port,u8 flag)
{
    if(port>=CN_SPI_NUM)
        return false;
    if(flag == InLowPower)
    {
        SPI_InLowPowerPinCfg(port);
        return true;
    }
    else if (flag == OutLowPower)
    {
        SPI_OutLowPowerPinCfg(port);
        return true;
    }
   return false;
}

// =============================================================================
// ���ܣ�SPI�ײ������ĳ�ʼ�����������SPI���ߵĳ�ʼ��������Ҫ�������£�
//       1.��ʼ�����߿��ƿ�SPI_CB���ص������ͻ������ĳ�ʼ����ֵ��
//       2.Ĭ�ϵ�Ӳ����ʼ������GPIO��SPI�Ĵ����ȣ�
//       3.�жϳ�ʼ������ɶ�д�ж����ã�
//       4.����SPIBusAdd��SPIBusAdd_r�������߽�㣻
// ��������
// ���أ���ʼ���ɹ�����1��ʧ�ܷ���0
// =============================================================================
bool_t SPI_Initialize(u8 port)
{
    struct SPI_Param SPI_Config;
    struct SPI_CB **pSpiCB;
    u8 IntLine;

    switch(port)
    {
        case CN_SPI1:
            SPI_Config.BusName          = "SPI1";
            SPI_Config.SPIBuf           = (u8*)&s_Spi1_Buf;
            IntLine = CN_INT_LINE_LPSPI1;
            pSpiCB = &s_ptSpi1_CB;
            break;
        case CN_SPI2:
            SPI_Config.BusName          = "SPI2";
            SPI_Config.SPIBuf           = (u8*)&s_Spi2_Buf;
            IntLine = CN_INT_LINE_LPSPI2;
            pSpiCB = &s_ptSpi2_CB;
            break;
        case CN_SPI3:
            SPI_Config.BusName          = "SPI3";
            SPI_Config.SPIBuf           = (u8*)&s_Spi3_Buf;
            IntLine = CN_INT_LINE_LPSPI3;
            pSpiCB = &s_ptSpi3_CB;
            break;
        case CN_SPI4:
            SPI_Config.BusName          = "SPI4";
            SPI_Config.SPIBuf           = (u8*)&s_Spi4_Buf;
            IntLine = CN_INT_LINE_LPSPI4;
            pSpiCB = &s_ptSpi4_CB;
            break;
        default:
            return (0);
    }
    SPI_Config.SpecificFlag     = (ptu32_t)tg_SpiReg[port];
    SPI_Config.SPIBufLen        = CFG_SPI_BUF_LEN;
    SPI_Config.MultiCSRegFlag   = false;
    SPI_Config.pTransferPoll    = (TransferPoll)__SPI_TxRxPoll;
    SPI_Config.pTransferTxRx    = (TransferFunc)__SPI_TransferTxRx;
    SPI_Config.pCsActive        = (CsActiveFunc)__SPI_BusCsActive;
    SPI_Config.pCsInActive      = (CsInActiveFunc)__SPI_BusCsInActive;
    SPI_Config.pBusCtrl         = (SPIBusCtrlFunc)__SPI_BusCtrl;

    *pSpiCB = SPI_BusAdd(&SPI_Config);// ��SPI�˿���ӵ�SPI����
    if(NULL == (*pSpiCB))
        return (0); // ʧ��

    __SPI_HardConfig((u32)SPI_Config.SpecificFlag); // SPI�Ĵ�������

    __SPI_IntConfig(IntLine); // ע���ж�

    return (1); // �ɹ�
}

//-----------------------------------------------------------------------------
//����: ��װSPI�˿�
//����: SPI�˿ں�
//����: -1 -- ʧ��; 0 -- �ɹ�;
//��ע:
//-----------------------------------------------------------------------------
s32 ModuleInstall_SPI(u8 Port)
{
    if(0 == SPI_Initialize(Port))
        return (-1);
    return (0);
}
