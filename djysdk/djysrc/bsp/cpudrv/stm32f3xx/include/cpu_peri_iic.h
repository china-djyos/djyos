//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// �ļ���     ��cpu_peri_iic.h
// ģ������: IICģ��ײ�����ͷ�ļ�
// ģ��汾: V1.00
// ����ʱ��: 09/17.2014
// =============================================================================

#ifndef CPU_PERI_IIC_H_
#define CPU_PERI_IIC_H_
#ifdef __cplusplus
extern "C" {
#endif

#define tagI2cReg I2C_TypeDef

//IIC��������ź궨��            //F303VCTֻ��2��I2C��iic_portȡֵ
#define CN_IIC1     0
#define CN_IIC2     1


#define CN_IIC_NUM  2
//ʱ�Ӻ궨��
#define CN_IIC_SPEED_50KHZ       (50*1000)
#define CN_IIC_SPEED_100KHZ      (100*1000)
#define CN_IIC_SPEED_200KHZ      (200*1000)
#define CN_IIC_SPEED_300KHZ      (300*1000)
#define CN_IIC_SPEED_400KHZ      (400*1000)

//�������ߵȴ���ʱʱ��
#ifndef CONFIG_I2C_MBB_TIMEOUT
#define CONFIG_I2C_MBB_TIMEOUT	1000
#endif

//�����д��ʱʱ��
#ifndef CONFIG_I2C_TIMEOUT
#define CONFIG_I2C_TIMEOUT	100
#endif

//�����д��������λ
#define I2C_READ_BIT  1
#define I2C_WRITE_BIT 0

#define I2C_AddMode7bit         0
#define I2C_AddMode10bit        1

#define start    0
#define stop     1


//============bit define===========================//
#define I2C_CR1_DNF_4BIT(X)         (X<<8)
#define I2C_CR2_NBYTES_8BIT(X)      (X<<16)
#define I2C_CR2_SADD_10BIT(X)       (X)
#define I2C_ISR_ADDCODE_7BIT(X)     (X<<17)

#define I2C_TIMINGE_FRESC_4BIT(X)   (X<<28)

#define I2C_TIMINGE_SCLDEL_4BIT(X)  (X<<20)
#define I2C_TIMINGE_SDADEL_4BIT(X)  (X<<16)
#define I2C_TIMINGE_SCLH_8BIT(X)    (X<<0)
#define I2C_TIMINGE_SCLL_8BIT(X)    (X<<8)

bool_t IIC_Busfree(u32 port,u32 sda_pin,u32 sck_pin);
bool_t IIC_Init(u8 iic_port);

#ifdef __cplusplus
}
#endif
#endif /* CPU_PERI_IIC_H_ */