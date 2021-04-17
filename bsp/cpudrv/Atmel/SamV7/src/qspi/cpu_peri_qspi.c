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
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 9/12.2014
// =============================================================================


#include "string.h"
#include "cpu_peri.h"
#include "spibus.h"
#include "xdmac.h"
#include "xdmad.h"
#include "spi_dma.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    ModuleInstall_QSPI();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip qspi"//CPU��qspi��������
//parent:"spi bus"   //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                      //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                   //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                     //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                   //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                     //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"spi bus"//������������������������none����ʾ�������������
                                     //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                     //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                     //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_QSPI == false )
//#warning  " cpu_onchip_qspi  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_QSPI    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================
#define tagQSpiReg  Qspi
#define sQspi QSPI

static struct SPI_CB *s_ptQSPI_CB;

// =============================================================================
// ���ܣ�ʱ��ԴΪMCK����150M��Ƶ��      QSCK BAUD = PCLK / (SCBR + 1)
// ������reg���Ĵ�����ַ
//      spiclk�������õ�ʱ���ٶȣ���λΪHz
// ���أ���
// =============================================================================
static void __QSPI_SetClk(volatile tagQSpiReg *Reg,u32 Fre)
{
    Reg->QSPI_SCR &= ~QSPI_SCR_SCBR_Msk;
    Reg->QSPI_SCR |= QSPI_SCR_SCBR(CN_CFG_MCLK/2/Fre - 1);
}

// =============================================================================
// ���ܣ�SPI���ƼĴ����������ã���PHA��CPOL��ʱ�ӵȣ����ݸ��ּĴ�������
// ������tpSPI,SPI��������ַ
//       ptr,����ָ��
// ���أ���
// =============================================================================
static void __QSPI_Config(volatile tagQSpiReg *Reg,tagSpiConfig *ptr,u8 cs)
{
    if(cs > CN_CS_MAX)
        return;
    __QSPI_SetClk(Reg,ptr->Freq);

    //set the PHA
    if(ptr->Mode & SPI_CPHA)
    {
        Reg->QSPI_SCR |= QSPI_SCR_CPHA;
    }
    else
    {
        Reg->QSPI_SCR &= ~QSPI_SCR_CPHA;
    }
    //set the cpol
    if(ptr->Mode & SPI_CPOL)
    {
        Reg->QSPI_SCR |= QSPI_SCR_CPOL;
    }
    else
    {
        Reg->QSPI_SCR &= ~QSPI_SCR_CPOL;
    }

    if(ptr->CharLen <= 16 && ptr->CharLen >= 4)
    {
        Reg->QSPI_MR &= ~QSPI_MR_NBBITS(0xF);
        Reg->QSPI_MR |= QSPI_MR_NBBITS(ptr->CharLen - 8);
    }
}

// =============================================================================
// ����: SPIĬ��Ӳ����ʼ�����ã���Ҫ��ʱ�����ú�GPIOд������������
// ����: RegBaseAddr,�Ĵ�����ַ
// ����: ��
// =============================================================================
static void __QSPI_HardConfig(u32 BaseAddr)
{
    tagQSpiReg *Reg;
    u32 Fre;

    Reg = (tagQSpiReg *)BaseAddr;

    Reg->QSPI_CR = QSPI_CR_QSPIDIS|QSPI_CR_SWRST;           //��λ������
    //SPI Mode
    Reg->QSPI_MR = QSPI_MR_CSMODE(1)|QSPI_MR_DLYCS(128)| QSPI_MR_DLYBCT(64);
//    Reg->QSPI_SCR |= QSPI_SCR_DLYBS(32);

    //Ĭ��ʹ��4M������
    Fre = 4*1000*1000;
    __QSPI_SetClk(Reg,Fre);
    //ʹ��DMA��ʽ
//    __QSPI_DmaConfig(BaseAddr);

    Reg->QSPI_CR = QSPI_CR_QSPIEN;
}

// =============================================================================
// ����: SPI�ж����ú���
// ����: IntLine,�ж���
// ����: ��
// =============================================================================
//static void __QSPI_IntConfig(u8 IntLine)
//{
//    //�ж��ߵĳ�ʼ��
//    u32 SPI_ISR(ufast_t IntLine);
//    Int_SetClearType(IntLine,CN_INT_CLEAR_PRE);
//    Int_Register(IntLine);
//    Int_IsrConnect(IntLine,SPI_ISR);
//    Int_SettoAsynSignal(IntLine);
//    Int_ClearLine(IntLine);
//    Int_RestoreAsynLine(IntLine);
//}

// =============================================================================
// ���ܣ�SPIƬѡʹ�ܣ�ʹƬѡ��Ч,���������о��������Ƭѡ�Ĵ�������ֱ�ӷ���
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// ˵����SPI��������ֻ������һ��CS������Ҫ�Ӷ��CS�������Ӧ�ó����Լ�����CS
// =============================================================================
static bool_t __QSPI_BusCsActive(tagQSpiReg *Reg, u8 cs)
{
//  s_ChipSelect = cs;
//    Reg->QSPI_TDR |= SPI_TDR_PCS(cs);
//    Reg->QSPI_MR  &= ~SPI_MR_PCS(0x0F);
//    Reg->QSPI_MR  |= SPI_MR_PCS(cs);
    return true;
}

// =============================================================================
// ���ܣ�SPIƬѡʧ�ܣ�ʹƬѡ��Ч
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// =============================================================================
static bool_t __QSPI_BusCsInActive(tagQSpiReg *Reg, u8 cs)
{
    Reg->QSPI_CR = QSPI_CR_LASTXFER| QSPI_CR_QSPIDIS;       //ģʽ�������÷��ˣ����������������Ƭѡ
    return false;
}

// =============================================================================
// ���ܣ�SPI���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�SPIʱ��Ƶ������
// ������specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
static s32 __QSPI_BusCtrl(tagQSpiReg *Reg,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    s32 result = 1;
    if( ((u32)Reg !=  (u32)sQspi))
        return 0;

    switch(cmd)
    {
    case CN_SPI_SET_CLK:
        __QSPI_SetClk(Reg,data2);
        break;
    case CN_SPI_CS_CONFIG:
        __QSPI_Config(Reg,(tagSpiConfig *)data1,data2);
        break;
    case CN_SPI_SET_AUTO_CS_EN:
        // Ӳ����������Զ�����CS������
        break;
    case CN_SPI_SET_AUTO_CS_DIS:
        break;
    default:
        break;
    }
    return result;
}

// =============================================================================
// ���ܣ���SPI���ռĴ�������ѯ��ʽ,��__QSPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ
// ���أ�����������
// =============================================================================
static u32 __QSPI_Read( tagQSpiReg * Reg )
{
    while ( (Reg->QSPI_SR & QSPI_SR_RDRF) == 0 ) ;

    return Reg->QSPI_RDR & 0xFFFF ;
}

// =============================================================================
// ���ܣ�дSPI���ͼĴ�������ѯ��ʽ,��__QSPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ��dwNpcs,Ƭѡ��wData��д����
// ���أ���
// =============================================================================
static void __QSPI_Write( tagQSpiReg * Reg, uint32_t dwNpcs, uint16_t wData )
{
    /* Send data */
    while ( (Reg->QSPI_SR & QSPI_SR_TXEMPTY) == 0 ) ;
    Reg->QSPI_TDR = wData;
}

// =============================================================================
// ���ܣ���ѯ��ʽ��д����
// ������Reg,�Ĵ�����ַ��srcAddr,����Դָ�룻wrSize��д��������destAddr������������
//       ��ַָ�룻rdSize������������recvoff,��ƫ�ƣ���δ��
// ���أ�true,��ȷ��false,����
// =============================================================================
static bool_t __QSPI_TxRxPoll(tagQSpiReg *Reg,u8 *srcAddr,u32 wrSize,
        u8 *destAddr, u32 rdSize,u32 recvoff,u8 cs)
{
    u32 i;
    if(!srcAddr)
        return false;

    Reg->QSPI_CR = QSPI_CR_QSPIEN;
    i = Reg->QSPI_RDR;
    for (i=0;i<(wrSize+rdSize);i++)
    {
        if (i<wrSize)
        {
            __QSPI_Write(Reg,0x0,srcAddr[i]);
            __QSPI_Read(Reg);
        }
        else if((i>=wrSize)&&(i<(wrSize+rdSize)))
        {
          __QSPI_Write(Reg,0x0,0);
          if(destAddr)
            destAddr[i-wrSize] = __QSPI_Read(Reg);
        }
    }
//    Reg->QSPI_CR |= QSPI_CR_LASTXFER;

    return true;
}

// =============================================================================
// ���ܣ�SPI�ײ������ĳ�ʼ�����������SPI���ߵĳ�ʼ��������Ҫ�������£�
//       1.��ʼ�����߿��ƿ�SPI_CB���ص������ͻ������ĳ�ʼ����ֵ��
//       2.Ĭ�ϵ�Ӳ����ʼ������GPIO��SPI�Ĵ����ȣ�
//       3.�жϳ�ʼ������ɶ�д�ж����ã�
//       4.����SPIBusAdd��SPIBusAdd_r�������߽�㣻
// ��������
// ���أ���ʼ���ɹ�����1��ʧ�ܷ���0
// ˵�����˺���ֻ֧��QSPI������ͨ��SPIʱ���ҽӵ�spibus�������;
// =============================================================================
bool_t ModuleInstall_QSPI(ptu32_t para)
{
    struct SPI_Param QSPI_Config;
    u8 PerId;

    QSPI_Config.BusName             = "QSPI";
    QSPI_Config.SPIBuf          = NULL;//(u8*)&s_SPI0_Buf;
//  IntLine = CN_INT_LINE_QSPI;
    PerId   = ID_QSPI;

    QSPI_Config.SPIBufLen        = 0;//SPI_BUF_LEN;
    QSPI_Config.SpecificFlag     = (ptu32_t)sQspi;//(ptu32_t)tg_SPI_Reg[port];
    QSPI_Config.MultiCSRegFlag   = false;//true;
    QSPI_Config.pTransferPoll    = (TransferPoll)__QSPI_TxRxPoll;
    QSPI_Config.pTransferTxRx    = NULL;
    QSPI_Config.pCsActive        = (CsActiveFunc)__QSPI_BusCsActive;
    QSPI_Config.pCsInActive      = (CsInActiveFunc)__QSPI_BusCsInActive;
    QSPI_Config.pBusCtrl         = (SPIBusCtrlFunc)__QSPI_BusCtrl;

    __QSPI_HardConfig(QSPI_Config.SpecificFlag);
//    __QSPI_IntConfig(IntLine);
    PMC_EnablePeripheral(PerId);

    if(s_ptQSPI_CB = SPI_BusAdd(&QSPI_Config))
        return 1;
    else
        return 0;
}
