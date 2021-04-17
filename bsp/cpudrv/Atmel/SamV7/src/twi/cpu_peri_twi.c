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

// �ļ���     ��cpu_peri_iic.c
// ģ������: IICģ��ײ�Ӳ���������Ĵ�������Ĳ���
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 05/12.2014
// =============================================================================
#include "cpu_peri.h"
#include <string.h>
#include "endian.h"
#include "iicbus.h"
#include "xdmac.h"
#include "xdmad.h"
#include "twi_dma.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//extern bool_t ModuleInstall_TWI(u8 port);
//    #if CFG_TWI0_ENABLE==1
//    ModuleInstall_TWI(CN_TWI0);
//    #endif
//
//    #if CFG_TWI1_ENABLE==1
//    ModuleInstall_TWI(CN_TWI1);
//    #endif
//
//    #if CFG_TWI2_ENABLE==1
//    ModuleInstall_TWI(CN_TWI2);
//    #endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpi driver iic"//CPU��twi����������ʵ�ʾ���iic
//parent:"iicbus"    //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"iicbus","int","time","lock","cpu onchip xdma"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPI_DRIVER_IIC == false )
//#warning  " cpi_driver_iic  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPI_DRIVER_IIC    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
#define CFG_TWI_BUF_LEN         128    //"TWI�����С",����TWIʹ�õĻ�������С
#define CFG_TWI_DMA_BUF_LEN     64     //"DMA�����С",����TWIʹ�õ�DMA��������С
//%$#@enum,true,false,
#define CFG_TWI0_ENABLE         0      //"�Ƿ�ʹ��TWI0",
#define CFG_TWI1_ENABLE         0      //"�Ƿ�ʹ��TWI1",
#define CFG_TWI2_ENABLE         0      //"�Ƿ�ʹ��TWI2",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

//==============================================================================
#define CN_TWI_REG_BADDR0        0x40018000
#define CN_TWI_REG_BADDR1        0x4001C000
#define CN_TWI_REG_BADDR2         0x40060000
#define tagTwiReg Twihs
// =============================================================================
static tagTwiReg volatile * const tg_TWI_Reg[] = {(tagTwiReg *)CN_TWI_REG_BADDR0,
                                                (tagTwiReg *)CN_TWI_REG_BADDR1,
                                                (tagTwiReg *)CN_TWI_REG_BADDR2};

//�ж���ʹ�õ��������ͽṹ��
struct IIC_IntParamSet
{
    struct SemaphoreLCB *pDrvPostSemp;   //�ź���
    u32 TransCount;                     //����������������
    u32 TransTotalLen;
};

//#define CFG_TWI_BUF_LEN         128
//#define CFG_TWI_DMA_BUF_LEN     64

//���徲̬����
static u8 sTwi0Buf[CFG_TWI_BUF_LEN];
static u8 sTwi0DmaBuf[CFG_TWI_DMA_BUF_LEN];
static struct IIC_IntParamSet IntParamset0;

static u8 sTwi1DmaBuf[CFG_TWI_DMA_BUF_LEN];
static u8 sTwi1Buf[CFG_TWI_BUF_LEN];
static struct IIC_IntParamSet IntParamset1;

static u8 sTwi2DmaBuf[CFG_TWI_DMA_BUF_LEN];
static u8 sTwi2Buf[CFG_TWI_BUF_LEN];
static struct IIC_IntParamSet IntParamset2;

u8 *TwiDmaBuf[CN_TWI_NUM] = {sTwi0DmaBuf,sTwi1DmaBuf,sTwi2DmaBuf};
struct IIC_IntParamSet *pIntParamset[CN_TWI_NUM] =
                                {&IntParamset0,&IntParamset1,&IntParamset2};

extern sXdmad dmad;
static TwiDma Twid0;
static TwiChannel Twi0Tx, Twi0Rx;
static TwiDma Twid1;
static TwiChannel Twi1Tx, Twi1Rx;
static TwiDma Twid2;
static TwiChannel Twi2Tx, Twi2Rx;

//��̬�������Ƿ�ʹ��DMA��ʽ���ͽ��գ�Ĭ��Ϊ��
static volatile bool_t sTwiDmaUsed[CN_TWI_NUM] = {false,false,false};
static struct IIC_CB *pTwi_CB[CN_TWI_NUM];
// =============================================================================
// ���ܣ�����Ӧ��I2C������������λ
// ������iic��Ӧ����Ĵ����Ļ�ַ
// ���أ���
// =============================================================================
static void __TWI_Reset(volatile tagTwiReg *reg)
{
    reg->TWIHS_CR = TWIHS_CR_SWRST;
}

// =============================================================================
// ����: ʹ��iic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
//       IntSrc,�ж�Դ
// ����: ��
// =============================================================================
static void __TWI_IntDisable(volatile tagTwiReg *reg,u32 IntSrc)
{
    reg->TWIHS_IDR = IntSrc;
}

// =============================================================================
// ����: ��ֹiic�ж�
// ����: reg,�������ļĴ�����ָ��
//       IntSrc,�������ж�Դ
// ����: ��
// =============================================================================
static void __TWI_IntEnable(volatile tagTwiReg *reg,u32 IntSrc)
{
    reg->TWIHS_IER = IntSrc;
}

// =============================================================================
// ����: ��ֹiic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __TWI_GenerateStop(volatile tagTwiReg *reg)
{
    reg->TWIHS_CR = TWIHS_CR_STOP;
}

// =============================================================================
// ���ܣ�IICʱ�����ú�����ʱ����ԴΪMCK�����㷽���ɲο������ֲ�
// ������reg���豸���
//      iicclk�������õ�ʱ���ٶȣ���׼��ʱ����50KHz,100KHz,200KHz
// ���أ�true=�ɹ���false=ʧ��
// =============================================================================
static void __TWI_SetClk(volatile tagTwiReg *reg,u32 iicclk)
{
    uint32_t dwCkDiv = 0 ;
    uint32_t dwClDiv ;
    uint32_t dwOk = 0 ;

    /* Configure clock */
    while ( !dwOk )
    {
        dwClDiv = ((CN_CFG_MCLK/ (2 * iicclk)) - 4) / (1<<dwCkDiv) ;

        if ( dwClDiv <= 255 )
        {
            dwOk = 1 ;
        }
        else
        {
            dwCkDiv++ ;
        }
    }

    reg->TWIHS_CWGR = 0 ;
    reg->TWIHS_CWGR = (dwCkDiv << 16) | (dwClDiv << 8) | dwClDiv ;
}

// =============================================================================
// ���ܣ�����������֪ͨ����Դ��ַ��Ŀ���ַ�����ݳ��ȣ������������Ѿ����ú�
// ������pXdmac��XDMAC�Ŀ���������ָ��
//      iChannel��ͨ����
//      sa,da,len��Դ��ַ��Ŀ���ַ��DAM���ֽ���
// =============================================================================
static void __TWI_DmaFastConfig(Xdmac *pXdmac,u8 iChannel,u32 sa,u32 da,u32 len)
{
    XDMAC_SetSourceAddr(pXdmac, iChannel, sa);
    XDMAC_SetDestinationAddr(pXdmac, iChannel, da);
    XDMAC_SetMicroblockControl(pXdmac, iChannel, len);
}

// =============================================================================
// ���ܣ�TWI�����жϵ�DMA����������DMA�ж���ں������ã���Ҫ����TWI���ݣ�����������DMA,
//      ���ʹ�ø�DMAͨ��������������ʹ�ø��жϺ����������DMA
// ������channel��ͨ����
//       pArg��TwiDma
// ���أ���
// =============================================================================
static void TWI_DmaRxIntIsr(uint32_t channel, TwiDma* pArg)
{
    TwiChannel *pTwiRx;
    static struct IIC_IntParamSet *IntParam;

    static struct IIC_CB *pTwiCB;
    volatile tagTwiReg *Reg;
    uint8_t *ptwi_dma_buf;
    uint32_t num,Port,DmaBufLen;

    for(Port = 0; Port < CN_TWI_NUM; Port++)
    {
        if((u32)(pArg->pTwihsHw) == (u32)tg_TWI_Reg[Port])
        {
            break;
        }
    }
    if(Port == CN_UART_NUM)
        return;

    pTwiRx = pArg->pRxChannel;
    pTwiCB = pTwi_CB[Port];
    Reg = tg_TWI_Reg[Port];
    DmaBufLen = CFG_TWI_DMA_BUF_LEN;
    ptwi_dma_buf = TwiDmaBuf[Port];
    IntParam = pIntParamset[Port];

    TWID_SendPause(pArg);

    if(IntParam->TransTotalLen - IntParam->TransCount > DmaBufLen)
    {
        num = DmaBufLen;
        IIC_PortWrite(pTwiCB,ptwi_dma_buf,num);
        IntParam->TransCount += num;
        //�����´�DMA���յ����ݴ�С
        if(IntParam->TransTotalLen - IntParam->TransCount > DmaBufLen)
            num = DmaBufLen;
        else
            num = IntParam->TransTotalLen - IntParam->TransCount;
        //�����´�DMA����
        if(num > 0)
        {
            __TWI_DmaFastConfig(dmad.pXdmacs,pTwiRx->ChNum,
                    (u32)&Reg->TWIHS_RHR,(u32)ptwi_dma_buf,num);

            TWID_RcvData(pArg);
        }
    }
    else                    //�������յ���ȫ��������
    {
        num = IntParam->TransTotalLen - IntParam->TransCount;
        IIC_PortWrite(pTwiCB,ptwi_dma_buf,num);
        __TWI_GenerateStop(Reg);

        Lock_SempPost(IntParam->pDrvPostSemp);
    }

}

// =============================================================================
// ���ܣ�TWI��DMA���ͷ���������DMA�жϷ���������
// ������channel��ͨ����
//       pArg��TwiDma
// ���أ���
// =============================================================================
static void TWI_DmaTxIntIsr(uint32_t channel, TwiDma* pArg)
{
    TwiChannel *pTwiTx;
    static struct IIC_IntParamSet *IntParam;

    static struct IIC_CB *pTwiCB;
    volatile tagTwiReg *Reg;
    uint8_t *ptwi_dma_send_buf;
    uint32_t num,Port,DmaBufLen;

    for(Port = 0; Port < CN_TWI_NUM; Port++)
    {
        if((u32)(pArg->pTwihsHw) == (u32)tg_TWI_Reg[Port])
        {
            break;
        }
    }
    if(Port == CN_UART_NUM)
        return;

    pTwiTx = pArg->pTxChannel;
    pTwiCB = pTwi_CB[Port];
    Reg = tg_TWI_Reg[Port];
    DmaBufLen = CFG_TWI_DMA_BUF_LEN;
    ptwi_dma_send_buf = TwiDmaBuf[Port];
    IntParam = pIntParamset[Port];

    TWID_SendPause(pArg);

    if(IntParam->TransTotalLen - IntParam->TransCount > DmaBufLen)
    {
        IntParam->TransCount += DmaBufLen;
        //���㱾���跢�Ͷ�������
        if(IntParam->TransTotalLen - IntParam->TransCount > DmaBufLen)
            num = DmaBufLen;
        else
            num = IntParam->TransTotalLen - IntParam->TransCount;
        IIC_PortRead(pTwiCB,ptwi_dma_send_buf,  DmaBufLen);
        //����DMA����
        if(num > 0)
        {
            __TWI_DmaFastConfig(dmad.pXdmacs,pTwiTx->ChNum,
                    (u32)ptwi_dma_send_buf,(u32)&Reg->TWIHS_THR,num);

            TWID_SendData(pArg);
        }
    }
    else                    //����DMA�Ѿ��������
    {
        IntParam->TransCount = IntParam->TransTotalLen;
        __TWI_GenerateStop(Reg);
        Lock_SempPost(IntParam->pDrvPostSemp);
    }
    return ;
}
// =============================================================================
// ���ܣ�IIC�����DMA���ú��������ÿ�����ʹ��DMA��ʽ
// ������
// ���أ�
// =============================================================================
static void __TWI_DMA_Config(volatile tagTwiReg* Reg,u32 cmd,u32 Port)
{
    TwiDma *pTwiDma;
    TwiChannel *pTwiRx,*pTwiTx;
    u32 TwiId;

    switch(Port)
    {
    case CN_TWI0:pTwiDma = &Twid0;  pTwiRx = &Twi0Rx;   pTwiTx = &Twi0Tx; TwiId = ID_TWIHS0;break;
    case CN_TWI1:pTwiDma = &Twid1;  pTwiRx = &Twi1Rx;   pTwiTx = &Twi1Tx; TwiId = ID_TWIHS1;break;
    case CN_TWI2:pTwiDma = &Twid2;  pTwiRx = &Twi2Rx;   pTwiTx = &Twi2Tx; TwiId = ID_TWIHS2;break;
    default: return;
    }

    if(cmd == CN_IIC_DMA_USED)              //����ʹ��DMA��ʽ���ͺͽ���
    {
        sTwiDmaUsed[Port] = true;           //���ʹ��DMA
        //�رղ�ʹ��DMAʱ���ж�
        __TWI_IntDisable(Reg,TWIHS_IDR_RXRDY|TWIHS_IDR_TXRDY);

        memset(pTwiTx, 0, sizeof(TwiChannel));
        memset(pTwiRx, 0, sizeof(TwiChannel));
        pTwiTx->BuffSize = CFG_TWI_DMA_BUF_LEN;
        pTwiTx->pBuff = TwiDmaBuf[Port];
        pTwiRx->BuffSize= CFG_TWI_DMA_BUF_LEN;
        pTwiRx->pBuff = TwiDmaBuf[Port];
        pTwiTx->sempaphore = 1;
        pTwiRx->sempaphore = 1;
        pTwiTx->pArgument = (void*)pTwiDma;
        pTwiRx->pArgument = (void*)pTwiDma;

        pTwiTx->callback = (TwiCallback)TWI_DmaTxIntIsr;
        pTwiRx->callback = (TwiCallback)TWI_DmaRxIntIsr;

        pTwiDma->pRxChannel = pTwiRx;
        pTwiDma->pTxChannel = pTwiTx;
        pTwiDma->pXdmad = &dmad;

        TWID_Configure(pTwiDma,TwiId);

        TWID_EnableRxChannels(pTwiDma, pTwiRx);
        TWID_EnableTxChannels(pTwiDma, pTwiTx);
    }
    else                                    //����ʹ�÷�DMA��ʽ���ͺͽ���
    {
        sTwiDmaUsed[Port] = false;

        TWID_DisableRxChannels(pTwiDma,pTwiRx);
        TWID_DisableTxChannels(pTwiDma,pTwiTx);
    }
}

// =============================================================================
// ����: IICĬ��Ӳ����ʼ�����ã���Ҫ��ʱ�����ú�GPIOд������������
// ����: RegBaseAddr,�Ĵ�����ַ
//       iicno,IIC��������
// ����: ��
// =============================================================================
static void __TWI_HardConfig(ptu32_t RegBaseAddr,u8 iicno)
{
    volatile tagTwiReg *reg;
    reg = (volatile tagTwiReg *)RegBaseAddr;


    XDMAD_Initialize(&dmad,0);
    reg->TWIHS_CR = TWIHS_CR_SWRST;//reset the twi
    reg->TWIHS_RHR;        //clear the rhr
    //config master mode
    reg->TWIHS_CR = TWIHS_CR_MSDIS;
    reg->TWIHS_CR = TWIHS_CR_SVDIS;

    /* Set master mode */
    reg->TWIHS_CR = TWIHS_CR_MSEN;

    //config the SCK
    reg->TWIHS_CWGR = 0 ;
    reg->TWIHS_CWGR = (1 << 16) | (248 << 8) | 248 ;//100KHz
}

// =============================================================================
// ����: IIC�ж����ú���
// ����: IntLine,�ж���
//       ISR,�жϷ�����ָ��
// ����: ��
// =============================================================================
static void __TWI_IntConfig(u32 IntLine,u32 (*ISR)(ptu32_t))
{
    //�ж��ߵĳ�ʼ��
    Int_Register(IntLine);
    Int_IsrConnect(IntLine,ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);     //�����ʼ�������ķ���fifo�յ��ж�
    Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// ����: TWI���ߵȴ�������������ѯ���ͺͽ������ݵȴ�
// ����: Reg��TWI��ַ
//       wrFlag����д���
// ����: true�������˳�;false����ʱ�˳�
// =============================================================================
static bool_t __TWI_Wait(volatile tagTwiReg *Reg,u8 Flag)
{
    u32 status;
    bool_t result = false;
    u32 timeout = 200;

    while(timeout != 0)
    {
        status = Reg->TWIHS_SR;
        if( (status & Flag) == Flag )
        {
            result = true;
            break;
        }
        timeout--;
        DJY_DelayUs(10);
    }


    return result;
}

// =============================================================================
// ���ܣ���ѯ��ʽдTWI����
// ������Reg,TWI��ַ
//      DevAddr,�豸��ַ
//      MemAddr,�豸�ڲ���ַ
//      MemAddrLen,�ڲ���ַ���ȣ��ֽ���
//      Buf,������
//      Length,д�ֽ���
// ���أ�true,������ɣ�false,��ʱ�����
// =============================================================================
static bool_t __TWI_WritePoll(volatile tagTwiReg *Reg,u8 DevAddr,u32 MemAddr,\
        u8 MemAddrLen,u8* Buf, u32 Length)
{
    u32 i;

    Reg->TWIHS_MMR = 0;
    Reg->TWIHS_MMR = (MemAddrLen<<8)|(DevAddr<<16);
    Reg->TWIHS_IADR = 0;
    Reg->TWIHS_IADR = TWIHS_IADR_IADR(MemAddr);
    Reg->TWIHS_CR = TWIHS_CR_START;

    Reg->TWIHS_THR = Buf[0];
    for(i = 1; i < Length; i++)
    {
        //wait for
         __TWI_Wait(Reg,TWIHS_SR_TXRDY);
        Reg->TWIHS_THR = Buf[i];
    }

    __TWI_GenerateStop(Reg);

    __TWI_Wait(Reg,TWIHS_SR_TXCOMP);
    return true;
}

// =============================================================================
// ���ܣ���ѯ��ʽ��TWI����
// ������Reg,TWI��ַ
//      DevAddr,�豸��ַ
//      MemAddr,�豸�ڲ���ַ
//      MemAddrLen,�ڲ���ַ���ȣ��ֽ���
//      Buf,������
//      Length,д�ֽ���
// ���أ�true,������ɣ�false,��ʱ�����
// =============================================================================
static bool_t __TWI_ReadPoll(volatile tagTwiReg *Reg,u8 DevAddr,u32 MemAddr,\
        u8 MenAddrLen,u8* Buf, u32 Length)
{
    u32 i;
    Reg->TWIHS_RHR;

    //TWIHS_MMR_IADRSZ_2_BYTE; len ; device addr
    Reg->TWIHS_MMR = 0;
    Reg->TWIHS_MMR = (MenAddrLen<<8)|(TWIHS_MMR_MREAD)|(DevAddr<<16);
    Reg->TWIHS_IADR = 0;
    Reg->TWIHS_IADR = TWIHS_IADR_IADR(MemAddr);

    if(Length == 1)
        Reg->TWIHS_CR = TWIHS_CR_START | TWIHS_CR_STOP;
    else
        Reg->TWIHS_CR = TWIHS_CR_START;

    for(i = 0; i < Length; i ++)
    {
        if(i == Length - 1)
        {
            __TWI_GenerateStop(Reg);
        }
        __TWI_Wait(Reg,TWIHS_SR_RXRDY);
        Buf[i] = Reg->TWIHS_RHR;
    }

    __TWI_Wait(Reg,TWIHS_SR_TXCOMP);
    return true;
}

// =============================================================================
// ���ܣ���ѯ��ʽ��дIIC�豸
// ������SpecificFlag,�Ĵ�����ַ
//       DevAddr,�豸��ַ
//       MemAddr,�豸�ڲ���ַ
//       MemAddrLen,�豸�ڲ���ַ����
//       Buf,�洢����
//       Length,���ݳ��ȣ��ֽ�
//       WrRdFlag,��д��ǣ�Ϊ0ʱд��1ʱΪ��
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================
static bool_t __TWI_WriteReadPoll(ptu32_t  SpecificFlag,u8 DevAddr,u32 MemAddr,\
                                u8 MemAddrLen,u8* Buf, u32 Length,u8 WrRdFlag)
{
    bool_t result;
    if(WrRdFlag == CN_IIC_WRITE_FLAG)//��ѯд
    {
        result =  __TWI_WritePoll((volatile tagTwiReg *)SpecificFlag,DevAddr,
                                MemAddr,MemAddrLen,Buf,Length);
    }
    else                            //��ѯ��
    {
        result = __TWI_ReadPoll((volatile tagTwiReg *)SpecificFlag,DevAddr,
                                MemAddr,MemAddrLen,Buf,Length);
    }

    return result;
}

// =============================================================================
// ����: ����дʱ������дʱ��Ĺ���Ϊ��������ַ��д�����洢��ַ��д�������洢��ַ��
//       ��ʱ������жϣ��������üĴ���Ϊ����ģʽ��֮�󽫻ᷢ�������жϣ����жϷ���
//       �����У�ÿ�η���һ�����ֽ�����ֱ��len��������post�ź���iic_bus_semp��������
//       ֹͣʱ��
// ����: specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//      dev_addr,������ַ��ǰ7���أ�������������λ�Ѿ����£��ú����ڲ��轫�õ�ַ��
//               ��һλ�����޸����Ķ�д����λ�����һ����д0����ʾд�����һ����д
//               1,��ʾ��;
//      mem_addr,�洢���ڲ���ַ,�ú����跢�͵�maddr_len�ֽڵ�����
//      maddr_len,�洢���ڲ���ַ�ĳ��ȣ��ֽڵ�λ��
//      length,���͵������������������ݵĵ�����һ�ֽڣ���count-1��ֹͣ����ACK�źţ�
//          �����յ��ֽ���Ϊcountʱ������ֹͣʱ�򣬲��ͷ��ź���iic_semp;
//      iic_semp,�������ʱ�������ͷŵ��ź��������ͳ����IIC_PortReadʱ������������
//          �����ͷŸ��ź�������������ʱ��
// ����: TRUE��������ʱ��ɹ���FALSEʧ��
// =============================================================================
static bool_t __TWI_GenerateWriteStart(volatile tagTwiReg *Reg,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct SemaphoreLCB *iic_semp)
{
    TwiDma *pTwiDma;
    TwiChannel *pTxCh;
    struct IIC_CB *IIC;
    struct IIC_IntParamSet *IntParam;
    u8 ch,*pDmaBuf,Port,DmaBufLen;

    //ͨ��specific_flag��������������
    switch((u32)Reg)
    {
    case CN_TWI_REG_BADDR0:
        Port = CN_TWI0; pTwiDma = &Twid0;   pTxCh = &Twi0Tx;    break;
    case CN_TWI_REG_BADDR1:
        Port = CN_TWI1; pTwiDma = &Twid1;   pTxCh = &Twi1Tx;    break;
    case CN_TWI_REG_BADDR2:
        Port = CN_TWI2; pTwiDma = &Twid2;   pTxCh = &Twi2Tx;    break;
    default: return false;
    }

    IIC         = pTwi_CB[Port];
    IntParam    = pIntParamset[Port];
    pDmaBuf     = TwiDmaBuf[Port];
    DmaBufLen   = CFG_TWI_DMA_BUF_LEN;

    IntParam->TransTotalLen = length;
    IntParam->TransCount = 0;
    IntParam->pDrvPostSemp = iic_semp;

    Reg->TWIHS_MMR = 0;
    Reg->TWIHS_MMR = (maddr_len<<8)|(dev_addr<<16);
    Reg->TWIHS_IADR = 0;
    Reg->TWIHS_IADR = TWIHS_IADR_IADR(mem_addr);
    Reg->TWIHS_CR = TWIHS_CR_START;

    if(sTwiDmaUsed[Port] == false)      //ʹ�÷�DMA��ʽ ����
    {
        //����һ���ֽڵ�����
        while(!(Reg->TWIHS_SR & TWIHS_SR_TXRDY));
        IIC_PortRead(IIC,&ch,1);

        Reg->TWIHS_THR = ch;
        IntParam->TransCount++;
        __TWI_IntEnable(Reg,TWIHS_IER_TXRDY);
    }
    else                                    //����DMA����ʱ�Ĳ���
    {
        while(!(Reg->TWIHS_SR & TWIHS_SR_TXRDY));
        ch = IIC_PortRead(IIC,pDmaBuf,DmaBufLen);
        if(ch)
        {
            __TWI_DmaFastConfig(dmad.pXdmacs,pTxCh->ChNum,
                    (u32)pDmaBuf,(u32)&Reg->TWIHS_THR,ch);

            TWID_SendData(pTwiDma);
        }
    }

    return true;
}

// =============================================================================
// ����: ������ʱ��������ʱ��Ĺ���Ϊ��������ַ��д�����洢��ַ��д����������ַ������
//       ��������ַ���������ʱ������жϣ��������üĴ���Ϊ����ģʽ��֮�󽫻ᷢ��
//       ���������жϣ����ж��н����յ������ݵ���IIC_PortWriteд�뻺�壬���յ�len��
//       ���������ݺ��ͷ��ź���iic_semp
// ����: specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//      dev_addr,������ַ��ǰ7���أ�������������λ�Ѿ����£��ú����ڲ��轫�õ�ַ��
//               ��һλ�����޸����Ķ�д����λ�����һ����д0����ʾд�����һ����д
//               1,��ʾ��;
//      mem_addr,�洢���ڲ���ַ,�ú����跢�͵�maddr_len�ֽڵ�����
//      maddr_len,�洢���ڲ���ַ�ĳ��ȣ��ֽڵ�λ��
//      len,���յ������������������ݵĵ�����һ�ֽڣ���count-1��ֹͣ����ACK�źţ�����
//          �յ��ֽ���Ϊcountʱ������ֹͣʱ�򣬲��ͷ��ź���iic_semp;
//      iic_semp,�����ʱ���������ͷŵ��ź������������ź�����
// ����: TRUE��������ʱ��ɹ���FALSEʧ��
// =============================================================================
static bool_t __TWI_GenerateReadStart( volatile tagTwiReg *Reg,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct SemaphoreLCB *iic_semp)
{
    TwiDma *pTwiDma;
    TwiChannel *pRxCh;
    struct IIC_IntParamSet *IntParam;
    u8 DmaRcvLen,*pDmaBuf,Port,DmaBufLen;

    //ͨ��specific_flag��������������
    switch((u32)Reg)
    {
    case CN_TWI_REG_BADDR0:
        Port = CN_TWI0; pTwiDma = &Twid0;   pRxCh = &Twi0Rx;    break;
    case CN_TWI_REG_BADDR1:
        Port = CN_TWI1; pTwiDma = &Twid1;   pRxCh = &Twi1Rx;    break;
    case CN_TWI_REG_BADDR2:
        Port = CN_TWI2; pTwiDma = &Twid2;   pRxCh = &Twi2Rx;    break;
    default: return false;
    }

    IntParam    = pIntParamset[Port];
    pDmaBuf     = TwiDmaBuf[Port];
    DmaBufLen   = CFG_TWI_DMA_BUF_LEN;

    IntParam->TransTotalLen = length;
    IntParam->TransCount = 0;
    IntParam->pDrvPostSemp = iic_semp;

    Reg->TWIHS_RHR;

    //TWIHS_MMR_IADRSZ_2_BYTE; len ; device addr
    Reg->TWIHS_MMR = 0;
    Reg->TWIHS_MMR = (maddr_len<<8)|(TWIHS_MMR_MREAD)|(dev_addr<<16);
    Reg->TWIHS_IADR = 0;
    Reg->TWIHS_IADR = TWIHS_IADR_IADR(mem_addr);
    if(length == 1)
        Reg->TWIHS_CR = TWIHS_CR_START | TWIHS_CR_STOP;
    else
        Reg->TWIHS_CR = TWIHS_CR_START;

    if(sTwiDmaUsed[Port] == false)
    {
        __TWI_IntEnable(Reg,TWIHS_IER_RXRDY|TWIHS_IER_TXRDY);
    }
    else
    {
        DmaRcvLen = length>DmaBufLen ? DmaBufLen:length;
        if(DmaRcvLen)
        {
            __TWI_DmaFastConfig(dmad.pXdmacs,pRxCh->ChNum,
                    (u32)&Reg->TWIHS_RHR,(u32)pDmaBuf,DmaRcvLen);

            TWID_RcvData(pTwiDma);
        }
    }

    return true;
}

// =============================================================================
// ����: �������ζ�д�ص����������ֶ�д�Ĳ�ֹͬͣʱ�򣬵����ڷ���ʱ����ֱ��ֹͣʱ��
//      ��Ϊ��������ֹͣ�ظ�ACK����ֹͣ
// ����: specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
// ����: ��
// =============================================================================
static void __TWI_GenerateEnd(volatile tagTwiReg *Reg)
{
    if(((u32)Reg != CN_TWI_REG_BADDR0)
            && (u32)Reg == CN_TWI_REG_BADDR1
            && (u32)Reg == CN_TWI_REG_BADDR2)
        return;

    __TWI_IntDisable(Reg, TWIHS_IDR_TXRDY|TWIHS_IDR_RXRDY|
            TWIHS_IDR_TXCOMP);
    DJY_EventDelay(100);
    __TWI_GenerateStop(Reg);

    __TWI_Reset(Reg);
}

// =============================================================================
// ���ܣ�IIC���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�IICʱ��Ƶ������
// ������specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
static s32 __TWI_BusCtrl(volatile tagTwiReg *Reg,u32 cmd,u32 data1,u32 data2)
{
    u8 port;
    if(((u32)Reg != CN_TWI_REG_BADDR0) &&
            (u32)Reg != CN_TWI_REG_BADDR1)
        return 0;
    if((u32)Reg == CN_TWI_REG_BADDR0)
        port = CN_TWI0;
    else if((u32)Reg == CN_TWI_REG_BADDR1)
        port = CN_TWI1;
    else if((u32)Reg == CN_TWI_REG_BADDR2)
        port = CN_TWI2;
    else
        return 0;
    switch(cmd)
    {
    case CN_IIC_SET_CLK:
        __TWI_SetClk(Reg,data1);
        break;
    case CN_IIC_DMA_USED:
    case CN_IIC_DMA_UNUSED:
        __TWI_DMA_Config(Reg,cmd,port);
        break;
    default:
        break;
    }
    return 1;
}

// =============================================================================
// ���ܣ�IIC�����뷢���жϷ��������ú���ʵ�ֵĹ������£�
//       1.ÿ���������һ���������ֽڷ���һ���жϣ�
//       2.���ж���ж�ʹ��ͬһ���жϺţ�������ݾ����������ʹ�õ����ĸ��жϣ�
//       3.���жϱ�־�����ж�ACK�źţ�ÿ��д�ֽڣ�������������Ӧ�޸ģ�
//       4.���մﵽ������һ���ֽ�ʱ�������ò�����ACK�źţ�
//       5.���ջ������ʱ����post�ź���IntParam->pDrvPostSemp��
//       6.���ջ������ʱ�������ֹͣʱ��
// ������i2c_int_line,�жϺţ�������û�õ�
// ���أ�������
// =============================================================================
static u32 __TWI_ISR(ufast_t IntLine)
{
    static struct IIC_CB *ICB;
    static struct IIC_IntParamSet *IntParam;
    volatile tagTwiReg *reg;

    u8 ch,Port;
    u32 twi_sr,twi_imr;

    if(IntLine == CN_INT_LINE_TWIHS0)
        Port = CN_TWI0;
    else if(IntLine == CN_INT_LINE_TWIHS1)
        Port = CN_TWI1;
    else if(IntLine == CN_INT_LINE_TWIHS2)
        Port = CN_TWI2;
    else
        return 0;

    ICB = pTwi_CB[Port];
    reg = tg_TWI_Reg[Port];
    IntParam = pIntParamset[Port];

    twi_sr = reg->TWIHS_SR;
    twi_imr = reg->TWIHS_IMR;

    if(sTwiDmaUsed[Port] == false)      //ʹ�÷�DMA���ͺͽ���
    {
        if((twi_sr & TWIHS_SR_RXRDY) && (twi_imr & TWIHS_IMR_RXRDY))//�����ж�
        {
            ch = reg->TWIHS_RHR;
            if(IIC_PortWrite(ICB,&ch,1))
                IntParam->TransCount ++;
            if(IntParam->TransCount == IntParam->TransTotalLen - 1)
            {
                __TWI_GenerateStop(reg);   //������һ��ʱдstop
            }
            else if(IntParam->TransCount == IntParam->TransTotalLen)
            {
                __TWI_GenerateStop(reg);
                __TWI_IntDisable(reg,TWIHS_IDR_RXRDY);
                __TWI_IntEnable(reg,TWIHS_IER_TXCOMP);
            }
        }
        else if((twi_sr & TWIHS_SR_TXRDY) && (twi_imr & TWIHS_IMR_TXRDY))//����
        {
            if(IIC_PortRead(ICB,&ch,1))
            {
                reg->TWIHS_THR = ch;
                IntParam->TransCount ++;
            }
            else if(IntParam->TransCount == IntParam->TransTotalLen)
            {
                __TWI_IntDisable(reg,TWIHS_IDR_TXRDY);
                __TWI_IntEnable(reg,TWIHS_IER_TXCOMP);
                __TWI_GenerateStop(reg);
            }
        }
        else if((twi_sr & TWIHS_SR_TXCOMP) && (twi_imr & TWIHS_IMR_TXCOMP))
        {
            __TWI_IntDisable(reg,TWIHS_IDR_TXCOMP);
            Lock_SempPost(IntParam->pDrvPostSemp);
        }
    }
    return 0;
}

// =============================================================================
// ���ܣ�IIC�ײ������ĳ�ʼ�����������IIC���ߵĳ�ʼ��������Ҫ�������£�
//       1.��ʼ�����߿��ƿ�IIC_CB���ص������ͻ������ĳ�ʼ����ֵ��
//       2.Ĭ�ϵ�Ӳ����ʼ������GPIO��IIC�Ĵ����ȣ�
//       3.�жϳ�ʼ������ɶ�д�ж����ã�
//       4.����IICBusAdd��IICBusAdd_r�������߽�㣻
// ������para,�޾�������
// ���أ���
// =============================================================================
bool_t ModuleInstall_TWI(u8 port)
{
    struct IIC_Param Twi_Config;
    u32 IntLine,TwiId;

    switch(port)
    {
    case CN_TWI0:
        Twi_Config.BusName         = "TWI0";
        Twi_Config.IICBuf          = (u8*)&sTwi0Buf;
        IntLine = CN_INT_LINE_TWIHS0;
        TwiId   = ID_TWIHS0;
        break;
    case CN_TWI1:
        Twi_Config.BusName         = "TWI1";
        Twi_Config.IICBuf          = (u8*)&sTwi1Buf;
        IntLine = CN_INT_LINE_TWIHS1;
        TwiId   = ID_TWIHS1;
        break;
    case CN_TWI2:
        Twi_Config.BusName         = "TWI2";
        Twi_Config.IICBuf          = (u8*)&sTwi2Buf;
        IntLine = CN_INT_LINE_TWIHS2;
        TwiId   = ID_TWIHS2;
        break;
    default: return false;
    }

    Twi_Config.IICBufLen           = CFG_TWI_BUF_LEN;
    Twi_Config.SpecificFlag        = (u32)tg_TWI_Reg[port];
    Twi_Config.pWriteReadPoll      = (WriteReadPoll)__TWI_WriteReadPoll;
    Twi_Config.pGenerateWriteStart = (WriteStartFunc)__TWI_GenerateWriteStart;
    Twi_Config.pGenerateReadStart  = (ReadStartFunc)__TWI_GenerateReadStart;
    Twi_Config.pGenerateEnd        = (GenerateEndFunc)__TWI_GenerateEnd;
    Twi_Config.pBusCtrl            = (IICBusCtrlFunc)__TWI_BusCtrl;

    __TWI_HardConfig((u32)tg_TWI_Reg[port],port);
    __TWI_IntConfig(IntLine,__TWI_ISR);
    PMC_EnablePeripheral(TwiId);

    pTwi_CB[port] = IIC_BusAdd(&Twi_Config);
    if(pTwi_CB[port])
        return 0;
    else
        return 1;
}
