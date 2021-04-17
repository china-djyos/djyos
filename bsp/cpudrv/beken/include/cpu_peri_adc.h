//----------------------------------------------------
// Copyright (c) 2018, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2018������Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
//����ģ��:
//���ߣ�
//�汾��V1.0.0
//�ļ�����:
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 
//   ����: 
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#ifndef CPU_PERI_ADC_H
#define CPU_PERI_ADC_H

#define  CN_AD_FAILSE           ( 0 )
#define  CN_AD_SUCCESS      ( 1 )
#define  CN_ARG_ERR             ( 0x38  )
#define  CN_RAED_ERR_DAT    (0xffffffff)  //������ǰ��16λ,��ʶ������

/*channel define*/

#define CN_AD_CHANNEL0  IO_CONFIG_PA0
#define CN_AD_CHANNEL1  IO_CONFIG_PA1
#define CN_AD_CHANNEL2  IO_CONFIG_PA2
#define CN_AD_CHANNEL3  IO_CONFIG_PA3
#define CN_AD_CHANNEL4  IO_CONFIG_PA4
#define CN_AD_CHANNEL5  IO_CONFIG_PA5
#define CN_AD_CHANNEL6  IO_CONFIG_PA6
#define CN_AD_CHANNEL7  IO_CONFIG_PA7

/*AD pin config choose from below when read .match with channel config*/

#define CN_AD_CFG_PA0  IO_CONFIG_PA0
#define CN_AD_CFG_PA1  IO_CONFIG_PA1
#define CN_AD_CFG_PA2  IO_CONFIG_PA2
#define CN_AD_CFG_PA3  IO_CONFIG_PA3
#define CN_AD_CFG_PA4  IO_CONFIG_PA4
#define CN_AD_CFG_PA5  IO_CONFIG_PA5
#define CN_AD_CFG_PA6  IO_CONFIG_PA6
#define CN_AD_CFG_PA7  IO_CONFIG_PA7

//comand
#define  CN_ADC_CHANNEL_SET             (0x11)
#define  CN_ADC_CHANNEL_RETSET      (0x12)
#define  CN_ADC_CHANNEL_PINCFG       (0x13)
#define  CN_ADC_CHANNEL_RD_DAT       (0x14)     //read data

//vef select
typedef enum{
    EN_ADC_VREF_EXT33 = 0,
    EN_ADC_VREF_IN24,
}tagVef;

int vbat_voltage_get(void);
void ModuleInstall_ADC(void);
//extern void appADC_Ctrl(u32 handle,u32 cmd, u32 data1,u32 data2);


#endif
