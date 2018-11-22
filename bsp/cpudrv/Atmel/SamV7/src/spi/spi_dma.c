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

// �ļ���     ��spi_dma.c
// ģ������: SPIģ�飬XDMAD��spi�����м��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 09/11.2015
// =============================================================================
#include "cpu_peri.h"
#include "xdmac.h"
#include "xdmad.h"
#include "spi_dma.h"


static void SPID_CacheInvalid(SpiDma *pSpid)
{
    u32 addr,size;

    addr = (u32)(pSpid->pTxChannel->pBuff);
    size = pSpid->pTxChannel->BuffSize;
    SCB_CleanInvalidateDCache_by_Addr((u32*)addr,size);

    addr = (u32)(pSpid->pRxChannel->pBuff);
    size = pSpid->pRxChannel->BuffSize;
    SCB_CleanInvalidateDCache_by_Addr((u32*)addr,size);
}
// =============================================================================
// ���ܣ����õ�DMA�ص���������TwiChannel��δ���ûص���������Ĭ�ϵ��ô˺���
// ������channel,DMA�ţ���0-23����24��ͨ��
//       pArg��TwiDma��DMA����ָ��
// ���أ���
// =============================================================================
static void SPID_RxTx_Cb(uint32_t channel, SpiDma* pArg)
{
    if ( (channel != pArg->pRxChannel->ChNum)
            && (channel != pArg->pTxChannel->ChNum) )
        return;

    /* Release the DMA channels */
    XDMAD_FreeChannel(pArg->pXdmad, pArg->pRxChannel->ChNum);
    XDMAD_FreeChannel(pArg->pXdmad, pArg->pTxChannel->ChNum);

    SPID_CacheInvalid(pArg);
//  SCB_CleanInvalidateDCache();
    /* Release the dataflash semaphore */
    pArg->sempaphore = 1;
}

// =============================================================================
// ���ܣ����õ�DMA�ص���������TwiChannel��δ���ûص���������Ĭ�ϵ��ô˺���
// ������channel,DMA�ţ���0-23����24��ͨ��
//       pArg��TwiDma��DMA����ָ��
// ���أ���
// =============================================================================
static uint8_t __Spid_LinkListConfig(SpiDma *pSpid)
{
    sXdmadCfg xdmadRxCfg,xdmadTxCfg;
    uint32_t xdmaCndc, xdmaInt;
    uint32_t spiId;
    if ((unsigned int)pSpid->pSpiReg == (unsigned int)SPI0 ) spiId = ID_SPI0;
    if ((unsigned int)pSpid->pSpiReg == (unsigned int)SPI1 ) spiId = ID_SPI1;

    /* Setup TX  */

    xdmadTxCfg.mbr_sa = (uint32_t)pSpid->pTxChannel->pBuff;

    xdmadTxCfg.mbr_da = (uint32_t)&pSpid->pSpiReg->SPI_TDR;

    xdmadTxCfg.mbr_ubc =  XDMA_UBC_NVIEW_NDV0 |
        XDMA_UBC_NDE_FETCH_DIS|
        XDMA_UBC_NSEN_UPDATED | pSpid->pTxChannel->BuffSize;

    xdmadTxCfg.mbr_cfg = XDMAC_CC_TYPE_PER_TRAN |
        XDMAC_CC_MBSIZE_SINGLE |
        XDMAC_CC_DSYNC_MEM2PER |
        XDMAC_CC_CSIZE_CHK_1 |
        XDMAC_CC_DWIDTH_BYTE|
        XDMAC_CC_SIF_AHB_IF0 |
        XDMAC_CC_DIF_AHB_IF1 |
        XDMAC_CC_SAM_INCREMENTED_AM |
        XDMAC_CC_DAM_FIXED_AM |
        XDMAC_CC_PERID(XDMAIF_Get_ChannelNumber(  spiId, XDMAD_TRANSFER_TX ));


    xdmadTxCfg.mbr_bc = 0;
    xdmadTxCfg.mbr_sus = 0;
    xdmadTxCfg.mbr_dus =0;

    /* Setup RX Link List */

    xdmadRxCfg.mbr_ubc = XDMA_UBC_NVIEW_NDV0 |
        XDMA_UBC_NDE_FETCH_DIS|
        XDMA_UBC_NDEN_UPDATED | pSpid->pRxChannel->BuffSize;

    xdmadRxCfg.mbr_da = (uint32_t)pSpid->pRxChannel->pBuff;

    xdmadRxCfg.mbr_sa = (uint32_t)&pSpid->pSpiReg->SPI_RDR;
    xdmadRxCfg.mbr_cfg = XDMAC_CC_TYPE_PER_TRAN |
        XDMAC_CC_MBSIZE_SINGLE |
        XDMAC_CC_DSYNC_PER2MEM |
        XDMAC_CC_CSIZE_CHK_1 |
        XDMAC_CC_DWIDTH_BYTE|
        XDMAC_CC_SIF_AHB_IF1 |
        XDMAC_CC_DIF_AHB_IF0 |
        XDMAC_CC_SAM_FIXED_AM |
        XDMAC_CC_DAM_INCREMENTED_AM |
        XDMAC_CC_PERID(XDMAIF_Get_ChannelNumber(  spiId, XDMAD_TRANSFER_RX ));


    xdmadRxCfg.mbr_bc = 0;
    xdmadRxCfg.mbr_sus = 0;
    xdmadRxCfg.mbr_dus =0;

    xdmaCndc = 0;

    /* Put all interrupts on for non LLI list setup of DMA */
      xdmaInt =  (XDMAC_CIE_BIE   |
                   XDMAC_CIE_DIE   |
                   XDMAC_CIE_FIE   |
                   XDMAC_CIE_RBIE  |
                   XDMAC_CIE_WBIE  |
                   XDMAC_CIE_ROIE);

    if (XDMAD_ConfigureTransfer( pSpid->pXdmad, pSpid->pRxChannel->ChNum,
            &xdmadRxCfg, xdmaCndc, 0, xdmaInt))
        return SPID_ERROR;

    if (XDMAD_ConfigureTransfer( pSpid->pXdmad, pSpid->pTxChannel->ChNum,
            &xdmadTxCfg, xdmaCndc, 0, xdmaInt))
        return SPID_ERROR;
    return 0;
}

// =============================================================================
// ���ܣ���ȡDMAͨ���ţ������ûص�����
// ������pSpid��TwiDma��DMA����ָ��
// ���أ�0����ȷ������������
// =============================================================================
static uint8_t __Spid_ChannelConfig(SpiDma *pSpid)
{
    uint32_t spiDmaTxChannel,spiDmaRxChannel;
    /* Driver initialize */
    XDMAD_Initialize(  pSpid->pXdmad, 0 );

//  XDMAD_FreeChannel( pSpid->pXdmad, spiDmaTxChannel);
//  XDMAD_FreeChannel( pSpid->pXdmad, spiDmaRxChannel);

    /* Allocate a DMA channel for SPI0/1 TX. */
    spiDmaTxChannel = XDMAD_AllocateChannel( pSpid->pXdmad,
            XDMAD_TRANSFER_MEMORY, pSpid->SpiId);
    if ( spiDmaTxChannel == XDMAD_ALLOC_FAILED ) {
        return SPID_ERROR;
    }
    /* Allocate a DMA channel for SPI0/1 RX. */
    spiDmaRxChannel =
        XDMAD_AllocateChannel( pSpid->pXdmad, pSpid->SpiId, XDMAD_TRANSFER_MEMORY);
    if ( spiDmaRxChannel == XDMAD_ALLOC_FAILED ) {
        return SPID_ERROR;
    }

    /* Setup callbacks for SPI0/1 RX */
    if(pSpid->pRxChannel->callback == NULL)
    {
        XDMAD_SetCallback(pSpid->pXdmad, spiDmaRxChannel,
            (XdmadTransferCallback)SPID_RxTx_Cb, pSpid);
    }
    else
    {
        XDMAD_SetCallback(pSpid->pXdmad, spiDmaRxChannel,
            (XdmadTransferCallback)pSpid->pRxChannel->callback, pSpid);
    }

    if (XDMAD_PrepareChannel( pSpid->pXdmad, spiDmaRxChannel ))
        return SPID_ERROR;

    /* Setup callbacks for SPI0/1 TX (ignored) */
    if(pSpid->pTxChannel->callback == NULL)
    {
        XDMAD_SetCallback(pSpid->pXdmad, spiDmaTxChannel,
                (XdmadTransferCallback)SPID_RxTx_Cb, pSpid);
    }
    else
    {
        XDMAD_SetCallback(pSpid->pXdmad, spiDmaTxChannel,
            (XdmadTransferCallback)pSpid->pTxChannel->callback, pSpid);
    }
    if ( XDMAD_PrepareChannel( pSpid->pXdmad, spiDmaTxChannel ))
        return SPID_ERROR;

    pSpid->pRxChannel->ChNum = spiDmaRxChannel;
    pSpid->pTxChannel->ChNum = spiDmaTxChannel;
    return 0;
}

// =============================================================================
// ���ܣ�����SPI��DMA�ж�ʹ�ܣ��ȹر������жϣ�����������ж�ʹ��
// ������pXdmac��iChannel��IntEn��XDMAC�������Ĵ�����DMAͨ�����ж�mskλ
// ���أ���
// =============================================================================
void SPID_DmaIntEnable(Xdmac *pXdmac,u8 iChannel,u32 IntEn)
{
    XDMAC_DisableChannelIt (pXdmac, iChannel, 0xFF);
    XDMAC_EnableChannelIt (pXdmac,iChannel, IntEn );
}

// =============================================================================
// ���ܣ�����SPI��DMA�ж�ʧ�ܣ��ر������ж�
// ������pXdmac��iChannel��IntEn��XDMAC�������Ĵ�����DMAͨ����
// ���أ���
// =============================================================================
void SPID_DmaIntDisable(Xdmac *pXdmac,u8 iChannel)
{
    XDMAC_DisableChannelIt (pXdmac, iChannel, 0xFF);
}

// =============================================================================
// ���ܣ�����SPI��DMA���ܣ���Ҫ�ǳ�ʼ��XDMAC�Ŀ������ͽ��գ�����ͨ���Ų�������ز���
// ������pSpid,SpiDma���ƿ�ָ��
// ���أ�0
// =============================================================================
uint32_t SPID_Configure(SpiDma *pSpid)
{

    if(__Spid_ChannelConfig(pSpid))
        return SPID_ERROR_LOCK;

    if(__Spid_LinkListConfig(pSpid))
        return SPID_ERROR_LOCK;

    return 0;
}

void SPID_FastConfig(Xdmac *pXdmac,u8 iChannel,u32 sa,u32 da,u32 len)
{
    XDMAC_SetSourceAddr(pXdmac, iChannel, sa);
    XDMAC_SetDestinationAddr(pXdmac, iChannel, da);
    XDMAC_SetMicroblockControl(pXdmac, iChannel, len);
}

// =============================================================================
// ���ܣ�SPI�Ľ�������DMAʹ�ܣ�����ʼʹ��DMA���շ���SPI���ݣ�����cache
// ������pSpid,SpiDma���ƿ�ָ��
// ���أ�0
// =============================================================================
uint32_t SPID_RxTxStart(SpiDma *pSpid)
{
    SPID_CacheInvalid(pSpid);
//  SCB_CleanInvalidateDCache();
    pSpid->sempaphore = 0;
    __DMB();

    /* Start DMA 0(RX) && 1(TX) */
    if (XDMAD_StartTransfer( pSpid->pXdmad, pSpid->pRxChannel->ChNum ))
        return SPID_ERROR_LOCK;

    if (XDMAD_StartTransfer( pSpid->pXdmad, pSpid->pTxChannel->ChNum ))
        return SPID_ERROR_LOCK;

    return 0;
}

// =============================================================================
// ���ܣ���ͣSPI��DMAͨ�����պͷ���
// ������pSpid,SpiDma���ƿ�ָ��
// ���أ�0
// =============================================================================
uint32_t SPID_RxTxPause( SpiDma *pSpid)
{
    SPID_CacheInvalid(pSpid);

    assert(pSpid);

    pSpid->sempaphore=1;
    __DMB();
    /* Start DMA 0(RX) && 1(TX) */
    if (XDMAD_StopTransfer( pSpid->pXdmad, pSpid->pRxChannel->ChNum ))
        return SPID_ERROR_LOCK;

    if (XDMAD_StopTransfer( pSpid->pXdmad, pSpid->pTxChannel->ChNum ))
        return SPID_ERROR_LOCK;

    return 0;
}

// =============================================================================
// ���ܣ���ֹSPI��DMAͨ�����պͷ��ͣ����ͷ�DMAͨ��
// ������pSpid,SpiDma���ƿ�ָ��
// ���أ�0����ȷ���أ�����������
// =============================================================================
uint32_t SPID_DisableDma(SpiDma* pSpi)
{
    /* Disable the SPI Peripheral */
    PMC_DisablePeripheral ( pSpi->SpiId );

    XDMAD_StopTransfer(pSpi->pXdmad, pSpi->pRxChannel->ChNum);
    XDMAD_StopTransfer(pSpi->pXdmad, pSpi->pTxChannel->ChNum);

    XDMAD_SetCallback(pSpi->pXdmad,pSpi->pRxChannel->ChNum,NULL,NULL);
    XDMAD_SetCallback(pSpi->pXdmad,pSpi->pTxChannel->ChNum,NULL,NULL);

    if(XDMAD_FreeChannel( pSpi->pXdmad, pSpi->pRxChannel->ChNum) != XDMAD_OK)
    {
        return SPID_ERROR;
    }

    memory_barrier();

    SPID_CacheInvalid(pSpi);
//  SCB_CleanInvalidateDCache();
    /* Release the dataflash semaphore */
    pSpi->sempaphore = 1;

    /* Invoke the callback associated with the current command */
    pSpi->pRxChannel->callback = NULL;
    pSpi->pTxChannel->callback = NULL;

    return 0;
}
