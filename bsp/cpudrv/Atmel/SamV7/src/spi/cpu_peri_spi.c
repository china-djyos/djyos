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
#include <stdint.h>
#include <string.h>
#include <math.h>
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
//    extern bool_t ModuleInstall_SPI(u8 port);
//    #if CFG_SPI0_ENABLE==1
//    ModuleInstall_SPI(CN_SPI0);
//    #endif
//
//    #if CFG_SPI1_ENABLE==1
//    ModuleInstall_SPI(CN_SPI1);
//    #endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip spi"//SPI��������
//parent:"spi bus"   //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                    //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                 //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                   //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                  //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                   //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"spi bus","int","time","lock","cpu onchip xdma"//������������������������none����ʾ�������������
                                   //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                   //����������������������г����á�,���ָ�
//weakdependence:"none"            //��������������������������none����ʾ�������������
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
#define CFG_SPI_BUF_LEN             128       //"�����С",����SPI��������С
#define CFG_SPI_DMA_BUF_LEN         128       //"DMA�����С",����DMA�����С
//%$#@enum,true,false,
#define CFG_SPI0_ENABLE             false     //"SPI0ʹ��",ʹ��SPI0
#define CFG_SPI1_ENABLE             false     //"SPI1ʹ��",ʹ��SPI1
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

#define tagSpiReg  Spi
static tagSpiReg volatile * const tg_SPI_Reg[] = {(tagSpiReg *) SPI0,
                                                    (tagSpiReg *) SPI1};

//�����ж�����ʹ�õľ�̬���ṹ��
struct SPI_IntParamSet
{
//  struct semaphore_LCB *pDrvPostSemp; //�ź���
    u32 SendDataLen;
    u32 RecvDataLen;
    u32 RecvOffset;
//  u8 CurrentCS;
//  u8 block_option;
};

#define SPI_PCS(npcs)       ((~(1 << npcs) & 0xF) << 16)
// =============================================================================
//����SPI���ƿ���жϾ�̬��
//#define CFG_SPI_BUF_LEN             128
//#define CFG_SPI_DMA_BUF_LEN         128

static u8 s_SPI0_Buf[CFG_SPI_BUF_LEN];
static u8 s_SPI0_DmaSendBuf[CFG_SPI_DMA_BUF_LEN];
static u8 s_SPI0_DmaRecvBuf[CFG_SPI_DMA_BUF_LEN];
static struct SPI_IntParamSet IntParamset0;
static u8 s_FirstRcvFlag0 = true;

static u8 s_SPI1_Buf[CFG_SPI_BUF_LEN];
static u8 s_SPI1_DmaSendBuf[CFG_SPI_DMA_BUF_LEN];
static u8 s_SPI1_DmaRecvBuf[CFG_SPI_DMA_BUF_LEN];
static struct SPI_IntParamSet IntParamset1;
static u8 s_FirstRcvFlag1 = true;

static struct SPI_CB *s_ptSpiCB[CN_SPI_NUM];
static u8 *pSpiDmaSendBuf[] = {s_SPI0_DmaSendBuf,s_SPI1_DmaSendBuf};
static u8 *pSpiDmaRecvBuf[] = {s_SPI0_DmaRecvBuf,s_SPI1_DmaRecvBuf};

extern sXdmad dmad;
static SpiDma Spid0;
static SpiChannel Spi0Tx, Spi0Rx;
static SpiDma Spid1;
static SpiChannel Spi1Tx, Spi1Rx;

// =============================================================================
// ����: �ж�ʹ�ܺ�ʧ�ܺ���
// ����: tpSPI,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __SPI_IntEnable(volatile tagSpiReg *Reg,u32 IntSrc)
{
    Reg->SPI_IER = IntSrc;
}
static void __SPI_IntDisable(volatile tagSpiReg *Reg,u32 IntSrc)
{
    Reg->SPI_IDR = IntSrc;
}

// =============================================================================
// ���ܣ�SPIʱ�����ú�����ʱ����ԴΪ50M����SCR��CPSR��Ƶ�õ�ʱ�ӣ�ʱ�Ӽ��㹫ʽΪ��
//       SCK =  PCLK / (CPSDVSR *[SCR+1])
// ������spi_dev���豸���
//      spiclk�������õ�ʱ���ٶȣ���λΪHz
// ���أ�true=�ɹ���false=ʧ��
// ˵�����������̶�SCR = 1;�������ֲᣬCPSDVSR����Ϊ2-254��ż������ˣ�Ƶ�ʷ�ΧΪ
//       12.5M ~ 100kHz
// =============================================================================
static void __SPI_SetClk(volatile tagSpiReg *Reg,u32 Fre,u8 cs)
{
    if(cs > CN_CS_MAX)
        return;

    Reg->SPI_CR |= SPI_CR_SPIDIS;
    Reg->SPI_CSR[cs] &= ~SPI_CSR_SCBR_Msk;
    Reg->SPI_CSR[cs] |= SPI_CSR_SCBR(CN_CFG_MCLK/2/Fre);
    Reg->SPI_CR |= SPI_CR_SPIEN;
}

// =============================================================================
// ���ܣ�SPI���ƼĴ����������ã���PHA��CPOL��ʱ�ӵȣ����ݸ��ּĴ�������
// ������tpSPI,SPI��������ַ
//       ptr,����ָ��
// ���أ���
// =============================================================================
static void __SPI_Config(volatile tagSpiReg *Reg,tagSpiConfig *ptr,u8 cs)
{
    if(cs > CN_CS_MAX)
        return;
    __SPI_SetClk(Reg,ptr->Freq,cs);

    Reg->SPI_CR |= SPI_CR_SPIDIS;
    //set the PHA
    if(ptr->Mode & SPI_CPHA)
    {
        Reg->SPI_CSR[cs] |= SPI_CSR_NCPHA;
    }
    else
    {
        Reg->SPI_CSR[cs] &= ~SPI_CSR_NCPHA;
    }
    //set the cpol
    if(ptr->Mode & SPI_CPOL)
    {
        Reg->SPI_CSR[cs] |= SPI_CSR_CPOL;
    }
    else
    {
        Reg->SPI_CSR[cs] &= ~SPI_CSR_CPOL;
    }

    if(ptr->CharLen <= 16 && ptr->CharLen >= 4)
    {
        Reg->SPI_CSR[cs] &= ~SPI_CSR_BITS_Msk;
        Reg->SPI_CSR[cs] |= SPI_CSR_BITS(ptr->CharLen - 8);
    }
    Reg->SPI_CR |= SPI_CR_SPIEN;
}

void SPI_DmaRxTxIntIsr(u32 channel, SpiDma *pArg)
{
    struct SPI_IntParamSet *param = NULL;
    tagSpiReg *Reg;

    u8 port,len;
    static u8 *firstRcvFlg;

    if((u32)pArg->pSpiReg == (u32)tg_SPI_Reg[CN_SPI0])
    {
        param = &IntParamset0;
        port = CN_SPI0;
        firstRcvFlg = &s_FirstRcvFlag0;
    }
    else if((u32)pArg->pSpiReg == (u32)tg_SPI_Reg[CN_SPI1])
    {
        param = &IntParamset1; port = CN_SPI1;
        firstRcvFlg = &s_FirstRcvFlag1;
    }
    else
    {
        return;
    }

    Reg = (tagSpiReg *)pArg->pSpiReg;
    SPID_RxTxPause(pArg);

    //�ȷ���ָ��ٷ���
    if(param->SendDataLen > 0)
    {
        if(param->SendDataLen > CFG_SPI_DMA_BUF_LEN)
        {
            param->SendDataLen -= CFG_SPI_DMA_BUF_LEN;
            param->RecvOffset  -= CFG_SPI_DMA_BUF_LEN;
            len = (param->SendDataLen > CFG_SPI_DMA_BUF_LEN) ?
                    CFG_SPI_DMA_BUF_LEN : param->SendDataLen;

            SPID_FastConfig(dmad.pXdmacs,pArg->pTxChannel->ChNum,
                    (u32)pSpiDmaSendBuf[port],(u32)Reg->SPI_TDR,len);

            SPID_FastConfig(dmad.pXdmacs,pArg->pTxChannel->ChNum,
                    (u32)Reg->SPI_RDR,(u32)pSpiDmaRecvBuf[port],len);

            SPID_DmaIntDisable(dmad.pXdmacs,pArg->pTxChannel->ChNum);
            SPID_RxTxStart(pArg);
            return;
        }
        else
        {
            param->RecvOffset -= param->SendDataLen;
            param->SendDataLen = 0;
        }
    }

    if(param->RecvOffset >0)
    {
        len = param->RecvOffset > CFG_SPI_DMA_BUF_LEN ?
                CFG_SPI_DMA_BUF_LEN : param->RecvOffset;

        SPID_FastConfig(dmad.pXdmacs,pArg->pTxChannel->ChNum,
                (u32)pSpiDmaSendBuf[port],(u32)Reg->SPI_TDR,len);

        SPID_FastConfig(dmad.pXdmacs,pArg->pTxChannel->ChNum,
                (u32)Reg->SPI_RDR,(u32)pSpiDmaRecvBuf[port],len);

        param->RecvOffset -= len;

        SPID_DmaIntDisable(dmad.pXdmacs,pArg->pTxChannel->ChNum);
        SPID_RxTxStart(pArg);
        return;
    }

    if(param->RecvDataLen > 0)
    {
        len = param->RecvDataLen > CFG_SPI_DMA_BUF_LEN ?
                CFG_SPI_DMA_BUF_LEN : param->RecvDataLen;

        if(*firstRcvFlg == false)
            SPI_PortWrite(s_ptSpiCB[port],pSpiDmaRecvBuf[port],len);
        else
            *firstRcvFlg = false;

        SPID_FastConfig(dmad.pXdmacs,pArg->pTxChannel->ChNum,
                (u32)pSpiDmaSendBuf[port],(u32)Reg->SPI_TDR,len);

        SPID_FastConfig(dmad.pXdmacs,pArg->pTxChannel->ChNum,
                (u32)Reg->SPI_RDR,(u32)pSpiDmaRecvBuf[port],len);

        param->RecvDataLen -= len;
        SPID_DmaIntDisable(dmad.pXdmacs,pArg->pTxChannel->ChNum);
        SPID_DmaIntEnable(dmad.pXdmacs,pArg->pRxChannel->ChNum,
                        XDMAC_CIE_BIE   |
                        XDMAC_CIE_DIE   |
                        XDMAC_CIE_FIE   |
                        XDMAC_CIE_RBIE  |
                        XDMAC_CIE_WBIE  |
                        XDMAC_CIE_ROIE);

        SPID_RxTxStart(pArg);
        return;
    }

    //finished
    *firstRcvFlg = true;
}
static bool_t __SPI_DmaConfig(u32 BaseAddr)
{
    SpiDma *pSpiDma;
    SpiChannel *pSpiTx,*pSpiRx;
    u8 PeriId,port;

    if(BaseAddr == (u32)tg_SPI_Reg[CN_SPI0])
    {
        port = CN_SPI0;
        PeriId = ID_SPI0;
        pSpiDma = &Spid0;
        pSpiTx  = &Spi0Tx;
        pSpiRx  = &Spi0Rx;
    }
    else if(BaseAddr == (u32)tg_SPI_Reg[CN_SPI1])
    {
        port = CN_SPI1;
        PeriId = ID_SPI1;
        pSpiDma = &Spid1;
        pSpiTx  = &Spi1Tx;
        pSpiRx  = &Spi1Rx;
    }
    else
        return false;

    memset(pSpiTx, 0, sizeof(SpiChannel));
    memset(pSpiRx, 0, sizeof(SpiChannel));

    pSpiTx->BuffSize = CFG_SPI_DMA_BUF_LEN;
    pSpiTx->pBuff = pSpiDmaSendBuf[port];
    pSpiRx->BuffSize= CFG_SPI_DMA_BUF_LEN;
    pSpiRx->pBuff = pSpiDmaRecvBuf[port];
    pSpiTx->pArgument = (void*)pSpiDma;
    pSpiRx->pArgument = (void*)pSpiDma;
    pSpiTx->callback = (SpiCallback)SPI_DmaRxTxIntIsr;
    pSpiRx->callback = (SpiCallback)SPI_DmaRxTxIntIsr;

    pSpiDma->pRxChannel = pSpiRx;
    pSpiDma->pTxChannel = pSpiTx;
    pSpiDma->pXdmad = &dmad;
    pSpiDma->SpiId  = PeriId;
    pSpiDma->pSpiReg = (Spi *)BaseAddr;
    pSpiDma->sempaphore = 1;

    if(SPID_Configure(pSpiDma))
        return false;

    return true;
}
// =============================================================================
// ����: SPIĬ��Ӳ����ʼ�����ã���Ҫ��ʱ�����ú�GPIOд������������
// ����: RegBaseAddr,�Ĵ�����ַ
// ����: ��
// =============================================================================
static void __SPI_HardConfig(u32 BaseAddr)
{
    tagSpiReg *Reg;
    u32 temp,Fre;

    Reg = (tagSpiReg *)BaseAddr;

    XDMAD_Initialize(&dmad,0);

    Reg->SPI_CR = SPI_CR_SPIDIS|SPI_CR_SWRST;           //��λ������
    Reg->SPI_MR = SPI_MR_MSTR|SPI_MR_MODFDIS|QSPI_MR_NBBITS_8_BIT;

    //Ĭ��ʹ��4M������
    Fre = 4*1000*1000;
    for(temp = 0; temp < 4; temp ++)
    {
        Reg->SPI_CSR[temp] = SPI_CSR_CSAAT|SPI_CSR_SCBR(CN_CFG_MCLK/2/Fre);
    }
    //ʹ��DMA��ʽ
//    __SPI_DmaConfig(BaseAddr);

    Reg->SPI_CR = SPI_CR_SPIEN;
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
    //����dma�շ������Ч��
    return 0;
}

// =============================================================================
// ����: SPI�ж����ú���
// ����: IntLine,�ж���
// ����: ��
// =============================================================================
static void __SPI_IntConfig(u8 IntLine)
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
// ���ܣ�SPIƬѡʹ�ܣ�ʹƬѡ��Ч,���������о��������Ƭѡ�Ĵ�������ֱ�ӷ���
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// ˵����SPI��������ֻ������һ��CS������Ҫ�Ӷ��CS�������Ӧ�ó����Լ�����CS
// =============================================================================
static bool_t __SPI_BusCsActive(tagSpiReg *Reg, u8 cs)
{
//  s_ChipSelect = cs;
    Reg->SPI_TDR |= SPI_TDR_PCS(cs);
    Reg->SPI_MR  &= ~SPI_MR_PCS(0x0F);
    Reg->SPI_MR  |= SPI_MR_PCS(cs);
    return true;
}

// =============================================================================
// ���ܣ�SPIƬѡʧ�ܣ�ʹƬѡ��Ч
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// =============================================================================
static bool_t __SPI_BusCsInActive(tagSpiReg *Reg, u8 cs)
{
    Reg->SPI_CR = SPI_CR_LASTXFER | SPI_CR_SPIDIS;
    return false;
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
    if( ((u32)Reg != (u32)tg_SPI_Reg[CN_SPI0]) &&
            ((u32)Reg != (u32)tg_SPI_Reg[CN_SPI1]) )
        return 0;

    switch(cmd)
    {
    case CN_SPI_SET_CLK:
        __SPI_SetClk(Reg,data1,data2);
        break;
    case CN_SPI_CS_CONFIG:
        __SPI_Config(Reg,(tagSpiConfig *)data1,data2);
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
// ���ܣ���SPI���ռĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ
// ���أ�����������
// =============================================================================
static u32 __SPI_Read( tagSpiReg * Reg )
{
    while ( (Reg->SPI_SR & SPI_SR_RDRF) == 0 ) ;

    return Reg->SPI_RDR & 0xFFFF ;
}

// =============================================================================
// ���ܣ�дSPI���ͼĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ��dwNpcs,Ƭѡ��wData��д����
// ���أ���
// =============================================================================
static void __SPI_Write( tagSpiReg * Reg, uint32_t dwNpcs, uint16_t wData )
{
    /* Send data */
    while ( (Reg->SPI_SR & SPI_SR_TXEMPTY) == 0 ) ;
    Reg->SPI_TDR = wData | SPI_PCS( dwNpcs ) ;
    while ( (Reg->SPI_SR & SPI_SR_TDRE) == 0 ) ;
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
    if(wrSize + rdSize == 0)
        return false;

    Reg->SPI_CR |= SPI_CR_SPIEN;
    Reg->SPI_RDR;
    len_limit = MAX(wrSize, rdSize + recvoff);
    for (i=0;i<len_limit;i++)
    {
        if (i<wrSize)
        {
            __SPI_Write(Reg,cs,srcAddr[i]);
        }
        else if((i>=wrSize)&&(i<len_limit))
        {
          __SPI_Write(Reg,cs,0);
        }
        if((destAddr) && (i>=recvoff) )
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
    struct SPI_IntParamSet *Param=NULL;
    struct SPI_CB *pSpiCb;
    SpiDma *pSpid;
    u32 len,port;

    if((u32)Reg == (u32)tg_SPI_Reg[CN_SPI0])
    {
        port = CN_SPI0; Param = &IntParamset0;  pSpid = &Spid0;
    }
    else if((u32)Reg == (u32)tg_SPI_Reg[CN_SPI1])
    {
        port = CN_SPI1;
    }
    else
        return false;

    Param->SendDataLen = sendlen;
    Param->RecvDataLen = recvlen;
    Param->RecvOffset  = recvoff;
    pSpiCb = s_ptSpiCB[port];

//    __SPI_IntDisable(Reg,SPI_IDR_TXBUFE);

    Reg->SPI_CR |= SPI_CR_SPIEN;

    //���������ͨ�ţ����Ƿ�������ٽ��գ�������ȷ���
    if( (sendlen > 0) && (pSpid->sempaphore == 1) )
    {
        len = SPI_PortRead(pSpiCb,pSpiDmaSendBuf[port],CFG_SPI_DMA_BUF_LEN);
        if(len > 0)
        {
            SPID_FastConfig(dmad.pXdmacs,pSpid->pTxChannel->ChNum,
                    (u32)pSpiDmaSendBuf[port],(u32)Reg->SPI_TDR,len);
            SPID_FastConfig(dmad.pXdmacs,pSpid->pRxChannel->ChNum,
                    (u32)Reg->SPI_RDR,(u32)pSpiDmaRecvBuf[port],len);

            //�����жϽ�ֹ
            SPID_DmaIntDisable(dmad.pXdmacs,pSpid->pRxChannel->ChNum);
            SPID_RxTxStart(pSpid);
        }
    }

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
// =============================================================================
bool_t ModuleInstall_SPI(u8 port)
{
    struct SPI_Param SPI_Config;
    u8 IntLine,PerId;

    switch(port)
    {
    case CN_SPI0:
        SPI_Config.BusName          = "SPI0";
        SPI_Config.SPIBuf           = (u8*)&s_SPI0_Buf;
        IntLine = CN_INT_LINE_SPI0;
        PerId   = ID_SPI0;
        break;
    case CN_SPI1:
        SPI_Config.BusName          = "SPI1";
        SPI_Config.SPIBuf           = (u8*)&s_SPI1_Buf;
        IntLine = CN_INT_LINE_SPI1;
        PerId   = ID_SPI1;
        break;
    default: return 0;
    }

    SPI_Config.SPIBufLen        = CFG_SPI_BUF_LEN;
    SPI_Config.SpecificFlag     = (ptu32_t)tg_SPI_Reg[port];
    SPI_Config.MultiCSRegFlag   = true;
    SPI_Config.pTransferPoll    = (TransferPoll)__SPI_TxRxPoll;
    SPI_Config.pTransferTxRx    = (TransferFunc)__SPI_TransferTxRx;
    SPI_Config.pCsActive        = (CsActiveFunc)__SPI_BusCsActive;
    SPI_Config.pCsInActive      = (CsInActiveFunc)__SPI_BusCsInActive;
    SPI_Config.pBusCtrl         = (SPIBusCtrlFunc)__SPI_BusCtrl;

    __SPI_HardConfig(SPI_Config.SpecificFlag);
//    __SPI_IntConfig(IntLine);
    PMC_EnablePeripheral(PerId);

    if(s_ptSpiCB[port] = SPI_BusAdd(&SPI_Config))
        return 1;
    else
        return 0;
}
