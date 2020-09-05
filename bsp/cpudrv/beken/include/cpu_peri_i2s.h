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

// �ļ���     ��cpu_peri_spi.h
// ģ������: SPIģ��ײ�Ӳ������ͷ�ļ�
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 16/10.2015
// =============================================================================

#ifndef CPU_PERI_I2S_H_
#define CPU_PERI_I2S_H_

#include "stdint.h"
#include "ring_buffer_dma_write.h"
#include "ring_buffer_dma_read.h"

#ifdef __cplusplus
extern "C" {
#endif

struct IIS_Device
{
    u32 open_flag;      //���ô򿪷�ʽ
    u32 sample_rate;    //������
    u8 bits_width;      //λ��

    u8 tx_enable:1;     //����DMAʹ��״̬
    u8 rx_enable:1;     //����DMAʹ��״̬

    /* rx members */
    u8 *rx_fifo;      //���ͻ����ַ
    RB_DMA_WR_ST rb_dma_wr;     //ע�ᵽ��ͨ���һ��ר�Ŵ���DMA���ݵĻ�������
    RB_DMA_RD_ST wr_dma_rd;     //ע�ᵽ��ͨ���һ��ר�Ŵ���DMA���ݵĻ�������
    /* tx members */
    u8 *tx_fifo;      //���ͻ����ַ
    u32 tx_fill_pos;    //���ͻ������λ��
    u32 tx_fill_size;   //���ͻ�������С
    u8 tx_paused;
};

struct IIS_OperParam
{
    u32 sample_rate;    //������
    u8 bits_width;      //λ��
    void (*i2s_rx_fin_handler)(u32);            //���յĻص�����   �ص������ǳɶԵģ�һ�εö���������������rx�ص���������IIS_Read����Ч�ˣ���ͨ���ص����������ݣ�txҲһ��
    void (*i2s_rx_half_fin_handler)(u32);       //���յĻص�����

    void (*i2s_tx_fin_handler)(u32);            //���͵Ļص�����
    void (*i2s_tx_half_fin_handler)(u32);       //���ͻص�����
};


enum i2s_ctrl
{
    IIS_DMA_RX_ENABLE = 0,      //IIS DMA����ʹ��
    IIS_DMA_TX_ENABLE,          //IIS DMA����ʹ��
    I2S_DMA_MASTER_SLAVE,      //IIS DMA����ģʽ����
    I2S_SAMPLE_RATE_SET,        //IIS DMA����������
    I2S_BIT_WIDTH_SET,          //IIS DMAλ������
};

int ModuleInstall_I2S(void);
s32 IIS_Open(struct IIS_OperParam *param, u32 mode);
s32 IIS_Read(void *buffer, s32 size);
s32 IIS_Close(void);
s32 IIS_Write(void *buffer, s32 size);
s32 IIS_Cotrol(enum i2s_ctrl cmd, ptu32_t data);

#ifdef __cplusplus
}
#endif

#endif
