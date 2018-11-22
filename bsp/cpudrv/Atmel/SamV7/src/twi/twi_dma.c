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

// �ļ���     ��twi_dma.c
// ģ������: twiģ�飬XDMAD��twi�����м��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 09/11.2015
// =============================================================================

#include "cpu_peri.h"
#include "xdmad.h"
#include "xdmac.h"
#include "twi_dma.h"
#include "xdma_hardware_interface.h"

// =============================================================================
// ���ܣ����õ�DMA�ص���������TwiChannel��δ���ûص���������Ĭ�ϵ��ô˺���
// ������channel,DMA�ţ���0-23����24��ͨ��
//       pArg��TwiDma��DMA����ָ��
// ���أ���
// =============================================================================
static void TWID_Rx_Cb(uint32_t channel, TwiDma* pArg)
{

    TwiChannel *pTwidCh = pArg->pRxChannel;
    if (channel != pTwidCh->ChNum)
        return;

    /* Release the DMA channels */
    XDMAD_FreeChannel(pArg->pXdmad, pTwidCh->ChNum);
    pTwidCh->sempaphore = 1;
    __DMB();
}

// =============================================================================
// ���ܣ����õ�DMA�ص���������TwiChannel��δ���ûص���������Ĭ�ϵ��ô˺���
// ������channel,DMA�ţ���0-23����24��ͨ��
//       pArg��TwiDma��DMA����ָ��
// ���أ���
// =============================================================================
static void TWID_Tx_Cb(uint32_t channel, TwiDma* pArg)
{
    TwiChannel *pTwidCh = pArg->pTxChannel;
    if (channel != pTwidCh->ChNum)
        return;

    /* Release the DMA channels */
    XDMAD_FreeChannel(pArg->pXdmad, pTwidCh->ChNum);
    pTwidCh->sempaphore = 1;
    __DMB();
}
// =============================================================================
// ���ܣ�����DMA�Ĳ�����������С��ͨ����Դ��ַ��Ŀ���ַ���жϵ�
// ������pTwid,TwiDma���ƿ�ָ��
//       pTxCh������ͨ���ṹ��ָ��
// ���أ�0������������,����
// =============================================================================
static u8 __Twi_ConfigureRxDma(TwiDma *pTwiXdma, TwiChannel *pRxCh)
{
    uint32_t xdmaCndc, Rhr, xdmaInt;
    sXdmadCfg xdmadRxCfg;

    Rhr = (uint32_t)&(pTwiXdma->pTwihsHw->TWIHS_RHR);
    xdmadRxCfg.mbr_ubc = XDMA_UBC_NVIEW_NDV0 |
                          XDMA_UBC_NDE_FETCH_DIS|
                          XDMA_UBC_NDEN_UPDATED |
                          (uint32_t)pRxCh->BuffSize;

    xdmadRxCfg.mbr_da = (uint32_t)pRxCh->pBuff;
    xdmadRxCfg.mbr_sa = Rhr;

    xdmadRxCfg.mbr_cfg = XDMAC_CC_TYPE_PER_TRAN |
                           XDMAC_CC_MBSIZE_SINGLE |
                           XDMAC_CC_DSYNC_PER2MEM |
                           XDMAC_CC_CSIZE_CHK_1 |
                           XDMAC_CC_DWIDTH_BYTE |
                           XDMAC_CC_SIF_AHB_IF1 |
                           XDMAC_CC_DIF_AHB_IF0 |
                           XDMAC_CC_SAM_FIXED_AM |
                           XDMAC_CC_DAM_INCREMENTED_AM |
                           XDMAC_CC_PERID(XDMAIF_Get_ChannelNumber(
                                pTwiXdma->TwiId , XDMAD_TRANSFER_RX ));

    xdmadRxCfg.mbr_bc = 0;
    xdmadRxCfg.mbr_sus = 0;
    xdmadRxCfg.mbr_dus =0;
    xdmaCndc = 0;
    xdmaInt =  (XDMAC_CIE_BIE |
               XDMAC_CIE_RBIE  |
               XDMAC_CIE_WBIE );

    if (XDMAD_ConfigureTransfer( pTwiXdma->pXdmad, pRxCh->ChNum,
            &xdmadRxCfg, xdmaCndc, 0, xdmaInt))
      return 1;
    return 0;
}

// =============================================================================
// ���ܣ�����DMA�Ĳ�����������С��ͨ����Դ��ַ��Ŀ���ַ���жϵ�
// ������pTwid,TwiDma���ƿ�ָ��
//       pTxCh������ͨ���ṹ��ָ��
// ���أ�0������������,����
// =============================================================================
static u8 __Twi_ConfigureTxDma(TwiDma *pTwiXdma, TwiChannel *pTxCh)
{
    uint32_t xdmaCndc, Thr, xdmaInt;
    sXdmadCfg xdmadTxCfg;

    Thr = (uint32_t)&(pTwiXdma->pTwihsHw->TWIHS_THR);
    xdmadTxCfg.mbr_ubc =      XDMA_UBC_NVIEW_NDV0 |
                                 XDMA_UBC_NDE_FETCH_DIS|
                                 XDMA_UBC_NSEN_UPDATED |
                                 (uint32_t)pTxCh->BuffSize;

    xdmadTxCfg.mbr_sa = (uint32_t)pTxCh->pBuff;
    xdmadTxCfg.mbr_da = Thr;
    xdmadTxCfg.mbr_cfg = XDMAC_CC_TYPE_PER_TRAN |
                         XDMAC_CC_MBSIZE_SINGLE |
                         XDMAC_CC_DSYNC_MEM2PER |
                         XDMAC_CC_CSIZE_CHK_1 |
                         XDMAC_CC_DWIDTH_BYTE|
                         XDMAC_CC_SIF_AHB_IF0 |
                         XDMAC_CC_DIF_AHB_IF1 |
                         XDMAC_CC_SAM_INCREMENTED_AM |
                         XDMAC_CC_DAM_FIXED_AM |
                         XDMAC_CC_PERID(XDMAIF_Get_ChannelNumber(
                            pTwiXdma->TwiId, XDMAD_TRANSFER_TX ));

    xdmadTxCfg.mbr_bc = 0;
    xdmadTxCfg.mbr_sus = 0;
    xdmadTxCfg.mbr_dus =0;
    xdmaCndc = 0;

    xdmaInt =  (XDMAC_CIE_BIE |
               XDMAC_CIE_RBIE  |
               XDMAC_CIE_WBIE );
    if (XDMAD_ConfigureTransfer( pTwiXdma->pXdmad, pTxCh->ChNum,
            &xdmadTxCfg, xdmaCndc, 0, xdmaInt) )
      return TWIDMA_ERROR;

    return 0;
}

// =============================================================================
// ���ܣ�ʹ��TWID��DMA����ͨ�������ͷŻ�ȡ��DMAͨ��
// ������pTwid,TwiDma���ƿ�ָ��
//       pRxCh������ͨ���ṹ��ָ��
// ���أ�0������������,����
// =============================================================================
uint32_t TWID_EnableRxChannels(TwiDma *pTwid, TwiChannel *pRxCh)
{
    uint32_t Channel;
    pTwid->pRxChannel = pRxCh;

    /* Allocate a DMA channel for TWI RX. */
    Channel =  XDMAD_AllocateChannel( pTwid->pXdmad, pTwid->TwiId,
            XDMAD_TRANSFER_MEMORY);
    if ( Channel == XDMAD_ALLOC_FAILED )
    {
        return TWIDMA_ERROR;
    }
    pRxCh->ChNum = Channel ;

     /* Setup callbacks for TWI RX */
    if(pRxCh->callback)
    {
      XDMAD_SetCallback(pTwid->pXdmad, pRxCh->ChNum,
            (XdmadTransferCallback)pRxCh->callback, pRxCh->pArgument);
    }
    else
    {
        XDMAD_SetCallback(pTwid->pXdmad, pRxCh->ChNum,
                (XdmadTransferCallback)TWID_Rx_Cb, pTwid);
    }

    if (XDMAD_PrepareChannel( pTwid->pXdmad, pRxCh->ChNum ))
        return TWIDMA_ERROR;
    if (__Twi_ConfigureRxDma(pTwid, pRxCh))
        return TWIDMA_ERROR_LOCK;

    return 0;
}

// =============================================================================
// ���ܣ�ʹ��TWID��DMA����ͨ�������ͷŻ�ȡ��DMAͨ��
// ������pTwid,TwiDma���ƿ�ָ��
//       pTxCh������ͨ���ṹ��ָ��
// ���أ�0������������,����
// =============================================================================
uint32_t TWID_EnableTxChannels( TwiDma *pTwid, TwiChannel *pTxCh)
{

    uint32_t Channel;

    assert(pTxCh);

    /* Init USART Tx Channel. */
    pTwid->pTxChannel = pTxCh;

     /* Allocate a DMA channel for USART0/1 TX. */
    Channel =  XDMAD_AllocateChannel( pTwid->pXdmad, XDMAD_TRANSFER_MEMORY,
            pTwid->TwiId);
    if ( Channel == XDMAD_ALLOC_FAILED ) {
      return TWIDMA_ERROR;
    }
    pTxCh->ChNum = Channel;
    /* Setup callbacks for USART0/1 TX */
    if(pTwid->pTxChannel->callback) {
      XDMAD_SetCallback(pTwid->pXdmad, pTxCh->ChNum,
            (XdmadTransferCallback)pTxCh->callback, pTxCh->pArgument);
    } else {
      XDMAD_SetCallback(pTwid->pXdmad, pTxCh->ChNum,
              (XdmadTransferCallback)TWID_Tx_Cb, pTwid);
    }

    if ( XDMAD_PrepareChannel( pTwid->pXdmad, pTxCh->ChNum ))
        return TWIDMA_ERROR;

    if (__Twi_ConfigureTxDma(pTwid , pTwid->pTxChannel))
        return TWIDMA_ERROR_LOCK;

    return 0;
}

// =============================================================================
// ���ܣ���ֹTWID��DMA����ͨ�������ͷŻ�ȡ��DMAͨ��
// ������pTwid,TwiDma���ƿ�ָ��
//       pTxCh������ͨ���ṹ��ָ��
// ���أ�0
// =============================================================================
uint32_t TWID_DisableRxChannels( TwiDma *pTwid, TwiChannel *pRxCh)
{
    assert(pRxCh);

    XDMAD_StopTransfer(pTwid->pXdmad, pRxCh->ChNum);

    XDMAD_SetCallback(pTwid->pXdmad, pRxCh->ChNum, NULL, NULL);
     /* Free allocated DMA channel for USART TX. */
    if(XDMAD_FreeChannel( pTwid->pXdmad, pRxCh->ChNum) != XDMAD_OK) {
        return TWIDMA_ERROR;
    }

    memory_barrier();
    return 0;
}

// =============================================================================
// ���ܣ���ֹTWID��DMA����ͨ�������ͷŻ�ȡ��DMAͨ��
// ������pTwid,TwiDma���ƿ�ָ��
//       pTxCh������ͨ���ṹ��ָ��
// ���أ�0
// =============================================================================
uint32_t TWID_DisableTxChannels( TwiDma *pTwid, TwiChannel *pTxCh)
{
    assert(pTxCh);

    XDMAD_StopTransfer(pTwid->pXdmad, pTxCh->ChNum);

    XDMAD_SetCallback(pTwid->pXdmad, pTxCh->ChNum, NULL, NULL);
    /* Free allocated DMA channel for USART TX. */
    if(XDMAD_FreeChannel( pTwid->pXdmad, pTxCh->ChNum) != XDMAD_OK) {
        return TWIDMA_ERROR;
    }

    memory_barrier();
    return 0;
}

// =============================================================================
// ���ܣ���ʱֹͣDMA���գ��жϼ�����������Ȼ���ֲ���
// ������pTwid��twi dmaָ��
// ����: 0
// =============================================================================
uint32_t TWID_RcvPause( TwiDma *pTwid)
{
    u32 addr,size;

    addr = (u32)(pTwid->pRxChannel->pBuff);
    size = pTwid->pRxChannel->BuffSize;
    SCB_CleanInvalidateDCache_by_Addr((u32*)addr,size);
//  SCB_CleanInvalidateDCache();
    pTwid->pRxChannel->sempaphore=1;
    __DMB();
    /* Start DMA 0(RX) && 1(TX) */
    if (XDMAD_StopTransfer( pTwid->pXdmad, pTwid->pRxChannel->ChNum ))
        return TWIDMA_ERROR_LOCK;

    return 0;
}

// =============================================================================
// ���ܣ���ʱֹͣDMA���գ��жϼ�����������Ȼ���ֲ���
// ������pTwid��twi dmaָ��
// ����: 0
// =============================================================================
uint32_t TWID_SendPause( TwiDma *pTwid)
{
    u32 addr,size;

    addr = (u32)(pTwid->pTxChannel->pBuff);
    size = pTwid->pTxChannel->BuffSize;
    SCB_CleanInvalidateDCache_by_Addr((u32 *)addr,size);
//  SCB_CleanInvalidateDCache();
    pTwid->pTxChannel->sempaphore=1;
    __DMB();
    /* Start DMA 0(RX) && 1(TX) */
    if (XDMAD_StopTransfer( pTwid->pXdmad, pTwid->pTxChannel->ChNum ))
        return TWIDMA_ERROR_LOCK;

    return 0;
}

// =============================================================================
// ���ܣ�TWI�ķ�������DMAʹ�ܣ�����ʼʹ��DMA����TWI���ݣ�����cache
// ������pTwid,TwiDma���ƿ�ָ��
// ���أ�0
// =============================================================================
uint32_t TWID_SendData( TwiDma *pTwid)
{
    u32 addr,size;

    addr = (u32)(pTwid->pTxChannel->pBuff);
    size = pTwid->pTxChannel->BuffSize;
    SCB_CleanInvalidateDCache_by_Addr((u32*)addr,size);
//  SCB_CleanInvalidateDCache();
    pTwid->pTxChannel->sempaphore=0;
    __DMB();
    if (XDMAD_StartTransfer( pTwid->pXdmad, pTwid->pTxChannel->ChNum ))
        return TWIDMA_ERROR_LOCK;

    return 0;
}

// =============================================================================
// ���ܣ�TWI�Ľ�������DMAʹ�ܣ�����ʼʹ��DMA����TWI���ݣ�����cache
// ������pTwid,TwiDma���ƿ�ָ��
// ���أ�0
// =============================================================================
uint32_t TWID_RcvData( TwiDma *pTwid)
{
    u32 addr,size;

    addr = (u32)(pTwid->pRxChannel->pBuff);
    size = pTwid->pRxChannel->BuffSize;
    SCB_CleanInvalidateDCache_by_Addr((u32*)addr,size);
//  SCB_CleanInvalidateDCache();
    pTwid->pRxChannel->sempaphore=0;
    __DMB();
    if (XDMAD_StartTransfer( pTwid->pXdmad, pTwid->pRxChannel->ChNum ))
        return TWIDMA_ERROR_LOCK;

    return 0;
}

// =============================================================================
// ���ܣ�����TWI��DMA���ܣ���Ҫ�ǳ�ʼ��XDMAC�Ŀ�����
// ������pTwid,TwiDma���ƿ�ָ��
//       TwiId,TWI����ID��
// ���أ�0
// =============================================================================
uint32_t TWID_Configure( TwiDma *pTwid ,uint8_t TwiId)
{
    /* Initialize the UART structure */
    pTwid->TwiId  = TwiId;

    if (TwiId == ID_TWIHS0)
        pTwid->pTwihsHw = TWIHS0;
    if (TwiId == ID_TWIHS1)
        pTwid->pTwihsHw = TWIHS1;
    if (TwiId == ID_TWIHS2)
        pTwid->pTwihsHw = TWIHS2;

    pTwid->pXdmad->pXdmacs = XDMAC;

    /* Driver initialize */
    XDMAD_Initialize(  pTwid->pXdmad, 0 );
    return 0;
}

