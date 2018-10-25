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
// �ļ���     ��cpu_peri_iic.h
// ģ������: IICģ��ײ�����ͷ�ļ�
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 09/17.2014
// =============================================================================

#ifndef CPU_PERI_IIC_H_
#define CPU_PERI_IIC_H_
#ifdef __cplusplus
extern "C" {
#endif
//IIC��������ź궨��
#define CN_IIC1     0
#define CN_IIC2     1

#define CN_IIC_SPEED_50KHZ       (50*1000)
#define CN_IIC_SPEED_100KHZ      (100*1000)
#define CN_IIC_SPEED_200KHZ      (200*1000)
#define CN_IIC_SPEED_300KHZ      (300*1000)
#define CN_IIC_SPEED_400KHZ      (400*1000)

//�������ߵȴ���ʱʱ��
#ifndef CONFIG_I2C_MBB_TIMEOUT
#define CONFIG_I2C_MBB_TIMEOUT  10000
#endif

//�����д��ʱʱ��
#ifndef CONFIG_I2C_TIMEOUT
#define CONFIG_I2C_TIMEOUT  1000
#endif

//�����д��������λ
#define I2C_READ_BIT  1
#define I2C_WRITE_BIT 0

#ifndef __IO
#define __IO  volatile
#endif

typedef struct I2C_StReg
{
      __IO uint16_t CR1;
      uint16_t  RESERVED0;
      __IO uint16_t CR2;
      uint16_t  RESERVED1;
      __IO uint16_t OAR1;
      uint16_t  RESERVED2;
      __IO uint16_t OAR2;
      uint16_t  RESERVED3;
      __IO uint16_t DR;
      uint16_t  RESERVED4;
      __IO uint16_t SR1;
      uint16_t  RESERVED5;
      __IO uint16_t SR2;
      uint16_t  RESERVED6;
      __IO uint16_t CCR;
      uint16_t  RESERVED7;
      __IO uint16_t TRISE;
      uint16_t  RESERVED8;
}tagI2CReg;


extern struct I2C_StReg   volatile * const pg_i2c1_reg;
extern struct I2C_StReg   volatile * const pg_i2c2_reg;

//===================CR1==================//
#define I2C_CR1_SWRST_MASK (1<<15)

#define I2C_CR1_ALERRT_MASK    (1<<13)
#define I2C_CR1_PEC_MASK       (1<<12)
#define I2C_CR1_POS_MASK       (1<<11)
#define I2C_CR1_ACK_MASK       (1<<10)
#define I2C_CR1_STOP_MASK      (1<<9)
#define I2C_CR1_START_MASK     (1<<8)
#define I2C_CR1_NOSTRETCH_MASK (1<<7)
#define I2C_CR1_ENGC_MASK (1<<6)
#define I2C_CR1_ENPEC_MASK     (1<<5)
#define I2C_CR1_ENARP_MASK     (1<<4)
#define I2C_CR1_SMBTYPE_MASK   (1<<3)

#define I2C_CR1_SMBUS_MASK     (1<<1)
#define I2C_CR1_PE_MASK        (1)

//===================CR2==================//
#define I2C_CR2_LAST_MASK     (1<<12)
#define I2C_CR2_DMAEN_MASK    (1<<11)
#define I2C_CR2_ITBUFEN_MASK  (1<<10)
#define I2C_CR2_ITEVTEN_MASK  (1<<9)
#define I2C_CR2_ITERREN_MASK  (1<<8)
/*I2C_CR2_FREQ[5:0]_MASK
 ����ʱ��Ƶ�ʱ���������ȷ������ʱ��Ƶ���Բ�����ȷ��ʱ������ķ�Χ��2��50MHz֮�䣺
000000����ֹ
000001����ֹ
000010��2 MHz
...
110010��50 MHz
����110010����ֹ*/

////===================OAR1==================//
#define I2C_OAR1_ADDMODE_MASK (1<<15)
/*ADD[9:8]���ӿڵ�ַ
7λ��ַģʽʱ���ù���
10λ��ַģʽʱΪ��ַ��9~8λ
ADD[7:1]���ӿڵ�ַ��ַ��7~1λ
*/
#define I2C_OAR1_ADD0_MASK    (1)//ADD0���ӿڵ�ַ10λ��ַģʽʱΪ��ַ��0λ

//===================OAR2==================//
/*ADD2[7:1]���ӿڵ�ַ��˫��ַģʽ�µ�ַ��7~1λ��*/
#define I2C_OAR2_ENDUAL_MASK  (1)//1����7λ��ַģʽ�£�OAR1��OAR2����ʶ��˫��ַģʽʹ��λ
//===================DR==================//
/*DR[7:0]��8λ���ݼĴ����ڴ�Ž��յ������ݻ�������ڷ��͵����ߵ����� */

//===================SR1==================//
#define I2C_SR1_SMBALERT_MASK (1<<15)
#define I2C_SR1_TIMEOUT_MASK  (1<<14)

#define I2C_SR1_PECERR_MASK  (1<<12)
#define I2C_SR1_OVR_MASK      (1<<11)
#define I2C_SR1_AF_MASK       (1<<10)
#define I2C_SR1_ARLO_MASK     (1<<9)
#define I2C_SR1_BERR_MASK     (1<<8)
#define I2C_SR1_TxE_MASK      (1<<7)
#define I2C_SR1_RxNE_MASK     (1<<6)

#define I2C_SR1_STOPF_MASK    (1<<4)
#define I2C_SR1_ADD10_MASK    (1<<3)
#define I2C_SR1_BTF_MASK      (1<<2)
#define I2C_SR1_ADDR_MASK     (1<<1)
#define I2C_SR1_SB_MASK       (1)

//===================SR2==================//
/*PEC[7:0]�����ݰ�������
��ENPEC=1ʱ��PEC[7:0]����ڲ���PEC��ֵ��*/
#define I2C_SR2_DUALF_MASK         (1<<7)
#define I2C_SR2_SMBHOST_MASK       (1<<6)
#define I2C_SR2_SMBDEFAULT_MASK    (1<<5)
#define I2C_SR2_GENCALL_MASK       (1<<4)

#define I2C_SR2_TRA_MASK           (1<<2)
#define I2C_SR2_BUSY_MASK          (1<<1)
#define I2C_SR2_MSL_MASK           (1)

//===ʱ�ӿ��ƼĴ���=====CCR==================//
#define I2C_CCR_FS_MASK            (1<<15)
#define I2C_CCR_DUTY_MASK         (1<<14)
/*CCR[11:0]������/��׼ģʽ�µ�ʱ�ӿ��Ʒ�Ƶϵ������ģʽ��*/

//===================TRISE==================//
/*TRISE[5:0]���ڿ���/��׼ģʽ�µ��������ʱ�䣨��ģʽ��
��Щλ����ʹ��I2C���߹淶�����������SCL����ʱ������䣬��������Ϊ1��
���磺��׼ģʽ���������SCL����ʱ��Ϊ1000ns�������I2C_CR2�Ĵ�����FREQ[5:0]�е�
ֵ����08h��tCK=125ns����TRISE[5:0]�б���д��09h(1000ns/125 ns = 8+1)��
�˲�����ֵҲ���Լӵ�TRISE[5:0]�ڡ�
����������һ������������������д��TRISE[5:0]��ȷ��tHIGH������
ע��ֻ�е�I2C������(PE=0)ʱ����������TRISE[5:0]��*/
bool_t IIC1_Init(void);
bool_t IIC2_Init(void);

#ifdef __cplusplus
}
#endif
#endif /* CPU_PERI_IIC_H_ */
