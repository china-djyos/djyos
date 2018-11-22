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
//����ģ��:IO��ʼ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:IO��ʼ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#if 0//ndef __cpu_peri_gpio_h_
#define __cpu_peri_gpio_h_

#ifdef __cplusplus
extern "C" {
#endif

//GPIO����ר�ú궨��
#define GPIO_MODE_IN        0       //��ͨ����ģʽ
#define GPIO_MODE_OUT       1       //��ͨ���ģʽ
#define GPIO_MODE_AF        2       //AF����ģʽ
#define GPIO_MODE_AIN       3       //ģ������ģʽ

#define GPIO_SPEED_2M       0       //GPIO�ٶ�2Mhz
#define GPIO_SPEED_25M      1       //GPIO�ٶ�25Mhz
#define GPIO_SPEED_50M      2       //GPIO�ٶ�50Mhz
#define GPIO_SPEED_100M     3       //GPIO�ٶ�100Mhz

#define GPIO_PUPD_NONE      0       //����������
#define GPIO_PUPD_PU        1       //����
#define GPIO_PUPD_PD        2       //����
#define GPIO_PUPD_RES       3       //����

#define GPIO_OTYPE_PP       0       //�������
#define GPIO_OTYPE_OD       1       //��©���

//GPIO�˿�����
#define GPIO_A              0
#define GPIO_B              1
#define GPIO_C              2
#define GPIO_D              3
#define GPIO_E              4
#define GPIO_F              5
#define GPIO_G              6
#define GPIO_H              7
#define GPIO_I              8

//GPIO���Ŷ���
#define PIN0                (1<<0)
#define PIN1                (1<<1)
#define PIN2                (1<<2)
#define PIN3                (1<<3)
#define PIN4                (1<<4)
#define PIN5                (1<<5)
#define PIN6                (1<<6)
#define PIN7                (1<<7)
#define PIN8                (1<<8)
#define PIN9                (1<<9)
#define PIN10               (1<<10)
#define PIN11               (1<<11)
#define PIN12               (1<<12)
#define PIN13               (1<<13)
#define PIN14               (1<<14)
#define PIN15               (1<<15)

void GPIO_CfgPinFunc(u32 port,u32 Pinx,u32 Mode,
                        u32 OutType,u32 OutSpeed,u32 PUPD);
u32 GPIO_AFSet(u32 port,u32 pinnum,u32 af_no);
u32 GPIO_GetData(u32 port);
void GPIO_OutData(u32 port,u32 data);
void GPIO_SettoHigh(u32 port,u32 msk);
void GPIO_SettoLow(u32 port,u32 msk);
void GPIO_PowerOn(u32 port);
void GPIO_PowerOff(u32 port);

#ifdef __cplusplus
}
#endif

#endif /*__cpu_peri_gpio_h_*/
