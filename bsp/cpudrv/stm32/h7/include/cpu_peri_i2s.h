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

#include <stm32h7xx_hal_i2s.h>

#ifdef __cplusplus
extern "C" {
#endif

#define tagI2sReg SPI_TypeDef

#define CN_I2S1     0
#define CN_I2S2     1
#define CN_I2S3     2
#define CN_I2S6     3
#define CN_I2S_NUM  4

// I2s��ģʽ
#define CFG_I2S_MODE_SLAVE_TX               I2S_MODE_SLAVE_TX            
#define CFG_I2S_MODE_SLAVE_RX               I2S_MODE_SLAVE_RX         
#define CFG_I2S_MODE_MASTER_TX              I2S_MODE_MASTER_TX        
#define CFG_I2S_MODE_MASTER_RX              I2S_MODE_MASTER_RX        
#define CFG_I2S_MODE_SLAVE_FULLDUPLEX       I2S_MODE_SLAVE_FULLDUPLEX 
#define CFG_I2S_MODE_MASTER_FULLDUPLEX      I2S_MODE_MASTER_FULLDUPLEX

// i2s��׼
#define CFG_I2S_STANDARD_PHILIPS            I2S_STANDARD_PHILIPS  
#define CFG_I2S_STANDARD_MSB                I2S_STANDARD_MSB      
#define CFG_I2S_STANDARD_LSB                I2S_STANDARD_LSB      
#define CFG_I2S_STANDARD_PCM_SHORT          I2S_STANDARD_PCM_SHORT
#define CFG_I2S_STANDARD_PCM_LONG           I2S_STANDARD_PCM_LONG 

// i2s���ݸ�ʽ
#define CFG_I2S_DATAFORMAT_16B              I2S_DATAFORMAT_16B         
#define CFG_I2S_DATAFORMAT_16B_EXTENDED     I2S_DATAFORMAT_16B_EXTENDED
#define CFG_I2S_DATAFORMAT_24B              I2S_DATAFORMAT_24B         
#define CFG_I2S_DATAFORMAT_32B              I2S_DATAFORMAT_32B         

// i2s�Ĳ�����
#define CFG_I2S_AUDIOFREQ_192K              I2S_AUDIOFREQ_192K   
#define CFG_I2S_AUDIOFREQ_96K               I2S_AUDIOFREQ_96K    
#define CFG_I2S_AUDIOFREQ_48K               I2S_AUDIOFREQ_48K    
#define CFG_I2S_AUDIOFREQ_44K               I2S_AUDIOFREQ_44K    
#define CFG_I2S_AUDIOFREQ_32K               I2S_AUDIOFREQ_32K    
#define CFG_I2S_AUDIOFREQ_22K               I2S_AUDIOFREQ_22K    
#define CFG_I2S_AUDIOFREQ_16K               I2S_AUDIOFREQ_16K    
#define CFG_I2S_AUDIOFREQ_11K               I2S_AUDIOFREQ_11K    
#define CFG_I2S_AUDIOFREQ_8K                I2S_AUDIOFREQ_8K     
#define CFG_I2S_AUDIOFREQ_DEFAULT           I2S_AUDIOFREQ_DEFAULT

// i2s��ʱ�Ӽ���
#define CFG_I2S_CPOL_LOW                    I2S_CPOL_LOW 
#define CFG_I2S_CPOL_HIGH                   I2S_CPOL_HIGH


void I2sDMA_Enable(u8 port);
void I2sDMA_Disable(u8 port);
u8 I2sWaitSendComplete(u8 port, u32 timeout);
bool_t I2sInit(u8 Port, u32 I2sMode, u32 I2sStandard, u32 I2sClockPolarity, u32 I2sDataFormat, u32 I2sFreq);
bool_t ModuleInstall_I2S(u8 Port);
#ifdef __cplusplus
}
#endif

#endif




