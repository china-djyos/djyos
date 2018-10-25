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

// �ļ���     ��spi_dma.h
// ģ������: SPIģ�飬XDMAD��spi�����м��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 09/11.2015
// =============================================================================

#ifndef __SPI_DMA_H__
#define __SPI_DMA_H__

#include "xdmac.h"

#define SPID_ERROR   1
#define SPID_ERROR_LOCK 2

typedef void (*SpiCallback)( uint8_t, void* ) ;
typedef struct
{
    uint8_t *pBuff;             // Pointer to the Buffer.
    uint32_t BuffSize;          //Buff size in bytes.
    uint32_t ChNum;             //Dma channel num.
    SpiCallback callback;       //Callback function invoked at the end of transfer.
    void *pArgument;            //Callback arguments.
} SpiChannel ;

typedef struct
{
    uint8_t SpiId ;
    Spi* pSpiReg ;
    sXdmad* pXdmad;             // Pointer to DMA driver
    SpiChannel *pRxChannel ;    //Current Rx channel
    SpiChannel *pTxChannel ;    //Current tx channel
    volatile uint8_t sempaphore;//flag to indicate the current transfer.
} SpiDma;


void SPID_DmaIntEnable(Xdmac *pXdmac,u8 iChannel,u32 IntEn);
void SPID_DmaIntDisable(Xdmac *pXdmac,u8 iChannel);
uint32_t SPID_Configure(SpiDma *pSpid);
void SPID_FastConfig(Xdmac *pXdmac,u8 iChannel,u32 sa,u32 da,u32 len);
uint32_t SPID_RxTxStart(SpiDma *pSpid);
uint32_t SPID_RxTxPause( SpiDma *pSpid);
uint32_t SPID_DisableDma(SpiDma* pSpi);


#endif /* SRC_USER_CPUDRV_INCLUDE_SPI_DMA_H_ */
