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

// �ļ���     ��cpu_peri_iic.c
// ģ������: IICģ��ײ�Ӳ���������Ĵ�������Ĳ���
// ģ��汾: V1.00
// ������Ա:
// ����ʱ��:
// =============================================================================

#include "board-config.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_I2C(u8 port);
//    #if CFG_I2C1_ENABLE==1
//    ModuleInstall_I2C(CN_I2C1);
//    #endif
//
//    #if CFG_I2C2_ENABLE==1
//    ModuleInstall_I2C(CN_I2C2);
//    #endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip iic"//CPU��iic��������
//parent:"iicbus"    //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                     //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                  //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                    //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                   //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                    //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"iicbus","int","lock"//������������������������none����ʾ�������������
                                    //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                    //����������������������г����á�,���ָ�
//weakdependence:"none"             //��������������������������none����ʾ�������������
                                    //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                    //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                    //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_IIC == false )
//#warning  " cpu_onchip_iic  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_IIC    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,16,512,
#define CFG_I2C_BUF_LEN         128         //"IIC��������С",
//%$#@enum,true,false,
#define CFG_I2C1_ENABLE         false       //"�Ƿ�����ʹ��IIC1",
#define CFG_I2C2_ENABLE         false       //"�Ƿ�����ʹ��IIC2",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

#if 0
#include "djyos.h"
#include "stdint.h"
#include "stdio.h"
#include "cpu_peri.h"
#include "iicbus.h"
#include "endian.h"
#include "lock.h"
#include "int.h"
#if 1

typedef struct _IIC_CTRL_
{
    vu32 IC_CON;
    vu32 IC_TAR;
    vu32 IC_SAR;
    vu32 IC_HS_MADDR;
    vu32 IC_DATA_CMD;
    vu32 IC_SS_SCL_HCNT;
    vu32 IC_SS_SCL_LCNT;
    vu32 IC_FS_SCL_HCNT;
    vu32 IC_FS_SCL_LCNT;
    vu32 IC_HS_SCL_HCNT;
    vu32 IC_HS_SCL_LCNT;
    vu32 IC_INTR_STAT;
    vu32 IC_INTR_MASK;
    vu32 IC_RAW_INTR_STAT;
    vu32 IC_RX_TL;
    vu32 IC_TX_TL;
    vu32 IC_CLR_INTR;
    vu32 IC_CLR_RX_UNDER;
    vu32 IC_CLR_RX_OVER;
    vu32 IC_CLR_TX_OVER;
    vu32 IC_CLR_RD_REQ;
    vu32 IC_CLR_TX_ABRT;
    vu32 IC_CLR_RX_DONE;
    vu32 IC_CLR_ACTIVITY;
    vu32 IC_CLR_STOP_DET;
    vu32 IC_CLR_START_DET;
    vu32 IC_CLR_GEN_CALL;
    vu32 IC_ENABLE;
    vu32 IC_STATUS;
    vu32 IC_TXFLR;
    vu32 IC_RXFLR;
    vu32 IC_REV0;
    vu32 IC_TX_ABRT_SOURCE;
    vu32 IC_REV1;
    vu32 IC_DMA_CR;
    vu32 IC_DMA_TDLR;
    vu32 IC_DMA_RDLR;
//  vu32 IC_REV[];
//  vu32 IC_COMP_PARAM_1;
//  vu32 IC_COMP_VERSION;
//  vu32 IC_COMP_TYPE;
}tagI2CReg;
// =============================================================================
#define CN_IIC1_BASE        (0x41050000)//�޷��ŵ�ַ
#define CN_IIC2_BASE        (0x41060000)//�޷��ŵ�ַ

static struct IIC_CB s_IIC1_CB;
static struct IIC_CB s_IIC2_CB;

#define IIC1_BUF_LEN  128
#define IIC2_BUF_LEN  128

struct IIC_IntParamSet
{
    struct SemaphoreLCB *pDrvPostSemp;   //�ź���
    u32 TransCount;       //����������������
    u32 TransTotalLen;//���յ���������
};

static struct IIC_IntParamSet IntParamset1;
static struct IIC_IntParamSet IntParamset2;

#define I2C_INTR_EN_MCU4    (0x42030000 + 0x610)
#define I2C_INTR_STS_MCU4   (0x42030000 + 0x510)

#define I2C1_INTR_SUB       6
#define I2C2_INTR_SUB       7

static int __iic_wait_addr(volatile tagI2CReg *dev, int write)
{
    uint16_t csr;
    unsigned timeout = 0;
    do
    {
        DJY_DelayUs(1);
        csr = dev->SR1;
        csr|=(u16)(dev->SR2<<16);
        if (!(csr & I2C_SR1_ADDR_MASK))//��ַû�з��ͷ���
            continue;

        csr=dev->SR1;   /*��״̬�Ĵ�����־*/
        if (csr & I2C_SR1_ARLO_MASK)       /*����ٲö�ʧ*/
            {
            #ifdef DEBUG
            printk("__i2c_wait: ARLO\n");
            #endif
            return -1;
            }

        if (write == I2C_WRITE_BIT) /*дʱ*/
            {
            if( (csr & I2C_SR1_AF_MASK))/*�ж�ack*/
              {
                #ifdef DEBUG
                printk("__i2c_wait: No RXACK\n");
                #endif
                return -1;
              }
            }

        return 0;
    } while (timeout++ < CONFIG_I2C_TIMEOUT);//0.01s��ʱ

    #ifdef DEBUG
    printk("__i2c_wait: timed out\n");
    #endif
    return -1;
}

// =============================================================================
// ������IICѰַд������ַ

// ���ܣ�Ѱַiic�豸���������߷�iic���豸��ַ�����һ��bit��ʾ��/дλ
// ������ dev,�豸ָ��
//     devaddr,���������ַ�����һbit���ֶ�д
//     dir,��д��ǣ�Ϊ0ʱΪд��Ϊ1ʱΪ��
//     rsta,�Ƿ�Ϊ��������
// ���أ�1,�ɹ���0,ʧ��
// =============================================================================

static int __iic_write_addr (volatile tagI2CReg *dev,
                            u8 devaddr, u8 dir)
{
    unsigned time = 0;
    dev->CR1 |=  I2C_CR1_PE_MASK;/*ʹ�� IIC start*/
    dev->CR1 |=  I2C_CR1_START_MASK;
    while(!(dev->SR1&I2C_SR1_SB_MASK))//û�з�����ʼ��������//
    {
        DJY_DelayUs(1);
        if(time++>(CONFIG_I2C_MBB_TIMEOUT))//????
            return 0;   //��ʱ����
    }

    dev->DR = (devaddr << 1) | dir;    /*д �� ַ*/

    if (__iic_wait_addr(dev,I2C_WRITE_BIT) < 0)
        return 0;

    return 1;
}

// =============================================================================
// ��������������
// ���ܣ��������ݵ�iic���������Ѿ������iic����Ѱַ���ڲ���ַ��д��Ч����
// ������ dev,����ָ��
//       data,���ݻ�����ָ��
//       length,data����������С���ֽڵ�λ
// ���أ��������������ֽڵ�λ
// =============================================================================
static int __iic_write(volatile tagI2CReg *dev, u8 *data, int length)
{
    int i;

    //����������ѭ�����ͣ�ÿ�η���һ�ֽ����ݣ���ȴ�ack�������Ϣ
    for (i = 0; i < length; i++)
    {
        dev->DR = data[i];
        while(dev->SR1&I2C_SR1_BTF_MASK);
    }
    return i;
}

// =============================================================================
// ����: ʹ��iic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_IntEnable(volatile tagI2CReg *reg)
{
    reg->CR2 |= I2C_CR2_ITEVTEN_MASK| I2C_CR2_ITBUFEN_MASK;

}

// =============================================================================
// ����: ��ֹiic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_IntDisable(volatile tagI2CReg *reg)
{
    reg->CR2 &= ~(I2C_CR2_ITEVTEN_MASK| I2C_CR2_ITBUFEN_MASK);
}

// =============================================================================
// ���ܣ���ʼ��ʱ������,ppc1020�ṩ��������i2c����ʱ������CCB/2���پ���i2cʱ�ӷ�Ƶ��
//        ����I2CFDR�Ĵ�������Ƶϵ�������ض��ļ��㷽������APIͨ��ö�ٲ���ȷ�����ֿ�
//        ���õĲ�����.
// ������reg���Ĵ���ָ��
//      i2cclk��ö������I2C_CLOCK_RATE_50K��ʱ��Ϊ50K
// ���أ�true=�ɹ���false=ʧ��
//Thigh = CCR ��TPCLK1
//Tlow = 2 �� CCR �� TPCLK1
//���DUTY = 1�� (�ٶȴﵽ400kHz)
//Thigh = 9 �� CCR ��TPCLK1
//Tlow = 16 �� CCR ��TPCLK1
// =============================================================================
static void __IIC_SetClk(volatile tagI2CReg *reg,u32 i2cclk)
{
    switch (i2cclk)
    {
            case CN_IIC_SPEED_50KHZ:    //24M    Thigh = CCR ��TPCLK1  Tlow = 2 �� CCR �� TPCLK1
                 pg_iic1_reg->CCR     = 240;//����/��׼ģʽ�µ�ʱ�ӿ��Ʒ�Ƶϵ��(��ģʽ)
                break;
            case CN_IIC_SPEED_100KHZ:
                 pg_iic1_reg->CCR     = 120;//����/��׼ģʽ�µ�ʱ�ӿ��Ʒ�Ƶϵ��(��ģʽ)
                break;
            case CN_IIC_SPEED_200KHZ:
                 pg_iic1_reg->CCR     = 60;//����/��׼ģʽ�µ�ʱ�ӿ��Ʒ�Ƶϵ��(��ģʽ)
                break;
            case CN_IIC_SPEED_300KHZ:
                pg_iic1_reg->CCR     =  40;//����/��׼ģʽ�µ�ʱ�ӿ��Ʒ�Ƶϵ��(��ģʽ)
                break;
            case CN_IIC_SPEED_400KHZ:
                 pg_iic1_reg->CCR     = 30;//����/��׼ģʽ�µ�ʱ�ӿ��Ʒ�Ƶϵ��(��ģʽ)
                break;
            default:
            break;
    }
}

// =============================================================================
// ����: I/Oģ��ķ�ʽ �ͷ�û�и�λ��ʹ�����ŵ�IIC����
// ����: port I/O�� �� CN_GPIO_B
//      TS_SDA:���ź�
//      TS_SCK:���ź�
// ����: true/false
// =============================================================================
static bool_t _IIC_Busfree(u32 port,u32 sda_pin,u32 sck_pin)
{
     u32 timeout=0;
     GPIO_CfgPinFunc(port,sda_pin,CN_GPIO_MODE_IN_FLOATING);//TS_SDA
     GPIO_CfgPinFunc(port,sck_pin,CN_GPIO_MODE_GPIO_OUT_OD_50Mhz);//TS_SCK

     while(1)
     {
          timeout++;
          GPIO_SettoLow(port,1<<sck_pin);
          DJY_DelayUs(10);

          GPIO_SettoHigh(port,1<<sck_pin);
          DJY_DelayUs(10);

          if(timeout>=CONFIG_I2C_TIMEOUT)
              return false;
          if( GPIO_GetData(port)&(1<<sda_pin))
              break;
    }

    GPIO_CfgPinFunc(port,sda_pin,CN_GPIO_MODE_GPIO_OUT_OD_50Mhz);//TS_SDA
    //����ֹͣ�ź� iic�����ͷ�
    GPIO_SettoLow(port,1<<sda_pin);
    DJY_DelayUs(10);
    GPIO_SettoHigh(port,1<<sda_pin);
    DJY_DelayUs(10);

    return true;
}

// =============================================================================
// ����: Ӳ��GPIO��ʼ����������Դ��
// ����: IIC_NO,���
// ����: ��
// =============================================================================
static bool_t __IIC_DefaultConfig(u8 IIC_NO)
{
    u8 tout;
    pg_rcc_reg->APB2ENR |=(1<<3)|(1<<0);             //��ʱ��B�͸��ù���
    switch(IIC_NO)
    {
    case CN_IIC1:
        pg_rcc_reg->APB1ENR |=(1<<21);               //��ʱ��
        pg_iic1_reg->CR1     |=I2C_CR1_SWRST_MASK;       /* ��λIIC����   */
        for (tout = 100; tout; tout--);
        pg_iic1_reg->CR1     = 0x0000;
        //����һЩ IIC��������û�и�λ����ʹ�ܹܽţ���stmpe811 �ڹ��������У���������ݶ˿�Ϊ�͵�ƽʱ����cpu��λ
        //������iic���߱�������һֱռ�� ���������I/Oģ��ķ�ʽ����ֹͣ�ź��ͷ�IIC����
        if(_IIC_Busfree(CN_GPIO_B,9,8)==false)
            return false;

        GPIO_CfgPinFunc(CN_GPIO_B,9,CN_GPIO_MODE_PERI_OUT_OD_50Mhz);//TS_SDA
        GPIO_CfgPinFunc(CN_GPIO_B,8,CN_GPIO_MODE_PERI_OUT_OD_50Mhz);//TS_SCK

        pg_iic1_reg->CR1     = 0x0001;//PEʹ��
        pg_iic1_reg->CR2     = 24;    //I2Cģ��ʱ��Ƶ��  24M
        pg_iic1_reg->CR1     = 0x0000;//PEʹ��
        pg_iic1_reg->TRISE   = 0x0025;//�ڿ���/��׼ģʽ�µ��������ʱ��(��ģʽ)
        pg_iic1_reg->CCR     = 0x00b4;//����/��׼ģʽ�µ�ʱ�ӿ��Ʒ�Ƶϵ��(��ģʽ)
        pg_iic1_reg->CR1    |= 0x0401;//PEʹ��&ACK
        pg_iic1_reg->OAR1    = 0x40A0;//4����ʼ�����������Ϊ��1����A�ӿڵ�ַ
        pg_iic1_reg->CCR|=I2C_CCR_FS_MASK;//����ģʽ
        break;
    case CN_IIC2:
        pg_rcc_reg->APB1ENR |=(1<<22);//��ʱ��

        if(_IIC_Busfree(CN_GPIO_B,11,10)==false)
            return false;

        GPIO_CfgPinFunc(CN_GPIO_B,11,CN_GPIO_MODE_PERI_OUT_OD_50Mhz);//TS_SDA
        GPIO_CfgPinFunc(CN_GPIO_B,10,CN_GPIO_MODE_PERI_OUT_OD_50Mhz);//TS_SCK

        pg_iic2_reg->CR1     |= I2C_CR1_SWRST_MASK;    /* ��λIIC����   */
          for (tout = 100; tout; tout--);
        pg_iic2_reg->CR1     = 0x0000;
        pg_iic2_reg->CR1    |= I2C_CR1_PE_MASK;
        pg_iic2_reg->CR2     = 24;
        pg_iic2_reg->CR1     = 0x0000;
        pg_iic2_reg->TRISE   = 0x0025;
        pg_iic2_reg->CCR     = 0x0078;
        pg_iic2_reg->CR1    |= 0x0401;
        pg_iic2_reg->OAR1    = 0x40A0;
        pg_iic2_reg->CCR|=I2C_CCR_FS_MASK;//����ģʽ
        break;
    default:
        break;
    }
    return true;
}


// =============================================================================
// ����: IIC�ж����ú���
// ����: IntLine,�ж���
//       isr,�жϷ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_IntConfig(u32 subint,u32 (*isr)(ptu32_t))
{
    u8 IntLine = CN_INT_LINE_I2C;

    Pic_IntRegister(IntLine,subint,I2C_INTR_EN_MCU4,I2C_INTR_STS_MCU4,isr);
}

// =============================================================================
// ����: ����ֹͣʱ�򣬽���������������֮���ͨ��
// ����: reg,iic����Ĵ�����ַ
// ����: ��
// =============================================================================
static void __IIC_GenerateStop(volatile tagI2CReg *reg)
{
    reg->CR1 |=(I2C_CR1_STOP_MASK);//����ֹͣ����

}
// =============================================================================
// ����: ��ȡ��Ӧ�Ĵ�����ֵ
// ����: reg iic����Ĵ�����ַ  buf �����ݻ�����  len�����ݳ���
// ����: �����ݳ��ȵ�λ�ֽ�
// =============================================================================

static u16 __iic_read(volatile tagI2CReg *reg, u8 *buf, u32 len)
{
    u8 i=0;
    while(len--)//��������
    {  /*  �Ƿ����ACKӦ��  */
    if (len>=1)                     reg->CR1 |=  I2C_CR1_ACK_MASK;
    else                            reg->CR1 &= ~I2C_CR1_ACK_MASK;
    while (!(reg->SR1 & I2C_SR1_RxNE_MASK));//�ȴ��������

        buf[i++] = reg->DR;

    }
    return i;

}

// =============================================================================
// ���ܣ���ѯ��ʽ��IIC���豸������
// ������reg,�Ĵ�����ַ
//       devaddr,���������ַ�����һbit���ֶ�д
//       memaddr//�Ĵ�����ַ
//       maddrlen�ļĴ�����ַ�ĳ���
//       addr,���ʵļĴ�����ַ
//       buf, �洢������
//       len, �洢�������ĳ���
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================
static s32 __IIC_ReadPoll(volatile tagI2CReg *reg,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{

    u8 mem_addr_buf[4];
    //����ַ����С�˱任
    fill_little_32bit(mem_addr_buf,0,memaddr);

    // ����д������ַ
    if(0 == __iic_write_addr(reg,devaddr,I2C_WRITE_BIT))
        return -1;

    // ���������Ĵ�����ַ

    if(maddrlen != __iic_write(reg,mem_addr_buf,maddrlen))
        return -1;

    //���Ͷ�������ַ
    if(0 == __iic_write_addr(reg,devaddr,I2C_READ_BIT))
        return -1;

    //���ܼĴ�������
    if(len != __iic_read(reg,buf,len))
            return -1;

    reg->CR1 = I2C_CR1_STOP_MASK;   // STOP
    // stop
//    reg->CR1 &= ~I2C_CR1_PE_MASK;    // Disable IIC

    return len;
}
// =============================================================================
// ���ܣ���ѯ��ʽ��IIC���豸��д����
// ������reg,�Ĵ�����ַ
//       devaddr,�豸��ַ
//       memaddr,�豸�ڲ���ַ
//       maddrlen,�豸�ڲ���ַ����
//       buf,�洢����
//       len,���ݳ��ȣ��ֽ�
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================
static s32 __IIC_WritePoll(volatile tagI2CReg *reg,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{
    u8 mem_addr_buf[4];
    //����ַ����С�˱任
    fill_little_32bit(mem_addr_buf,0,memaddr);

    // Write Device Address
    if(0 == __iic_write_addr(reg,devaddr,I2C_WRITE_BIT))
        return -1;

    // Write Memory Address
    if(maddrlen != __iic_write(reg,mem_addr_buf,maddrlen))
        return -1;

    //Write Data
    if(len != __iic_write(reg,buf,len))
        return -1;

    return len;
}

// =============================================================================
// ���ܣ���ѯ��ʽ��дIIC�豸
// ������reg,�Ĵ�����ַ
//       DevAddr,�豸��ַ
//       MemAddr,�豸�ڲ���ַ
//       MemAddrLen,�豸�ڲ���ַ����
//       Buf,�洢����
//       Length,���ݳ��ȣ��ֽ�
//       WrRdFlag,��д��ǣ�Ϊ0ʱд��1ʱΪ��
// ���أ�
// =============================================================================
static bool_t __IIC_WriteReadPoll(tagI2CReg *reg,u8 DevAddr,u32 MemAddr,\
                                u8 MemAddrLen,u8* Buf, u32 Length,u8 WrRdFlag)
{
    __IIC_IntDisable(reg);
    if(WrRdFlag == CN_IIC_WRITE_FLAG)   //д
    {
        if(Length == __IIC_WritePoll(reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            return true;
        else
            return false;
    }
    else                                //��
    {
        if(Length == __IIC_ReadPoll(reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            return true;
        else
            return false;
    }
}
// =============================================================================
// ����: ����дʱ������дʱ��Ĺ���Ϊ��������ַ��д�����洢��ַ��д�������洢��ַ��
//       ��ʱ������жϣ��������üĴ���Ϊ����ģʽ��֮�󽫻ᷢ�������жϣ����жϷ���
//       �����У�ÿ�η���һ�����ֽ�����ֱ��len��������post�ź���iic_bus_semp��������
//       ֹͣʱ��
// ����: SpecificFlag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//      DevAddr,������ַ��ǰ7���أ��ѽ��ڲ���ַ��ռ��bitλ���£��ú����轫�õ�ַ��
//               ��һλ�����������һλ��/д����;
//      MemAddr,�洢���ڲ���ַ�������͵������ϵĵ�ַ���õ�ַδ��������������ַ�ϵ�
//               ����λ��
//      MenAddrLen,�洢���ڲ���ַ�ĳ��ȣ��ֽڵ�λ��δ������������ַ����ı���λ��
//      Length,���յ������������������ݵĵ�����һ�ֽڣ���count-1��ֹͣ����ACK�źţ�
//          �����յ��ֽ���Ϊcountʱ������ֹͣʱ�򣬲��ͷ��ź���iic_semp;
//      IIC_BusSemp,�������ʱ�������ͷŵ��ź��������ͳ����IIC_PortReadʱ������������
//          �����ͷŸ��ź�������������ʱ��
// ����: TRUE��������ʱ��ɹ���FALSEʧ��
// =============================================================================
static bool_t __IIC_GenerateWriteStart(tagI2CReg *reg,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct SemaphoreLCB *iic_semp)
{
    u8 mem_addr_buf[4];
    //ͨ��specific_flag��������������
    if((u32)reg == CN_IIC1_BASE)
    {
        IntParamset0.TransTotalLen = length;
        IntParamset0.TransCount = 0;
        IntParamset0.pDrvPostSemp = iic_semp;           //iic_bus_semp
    }
    else if((u32)reg == CN_IIC2_BASE)
    {
        IntParamset1.TransTotalLen = length;
        IntParamset1.TransCount = 0;
        IntParamset1.pDrvPostSemp = iic_semp;
    }
    else
    {
        return false;
    }

    //����ַ����С�˱任
    fill_little_32bit(mem_addr_buf,0,mem_addr);
    __IIC_IntDisable(reg);                 //���ж�

    //�ȷ���������ַ���ڲ���ַ��Ȼ���͵�һ������

        if(__iic_write_addr(reg, dev_addr, I2C_WRITE_BIT) != 0)
        {
            //���������ڲ���ַ
            if(maddr_len - 1 == __iic_write(reg,mem_addr_buf,maddr_len-1))
            {
                reg->DR = mem_addr_buf[maddr_len-1];
                __IIC_IntEnable(reg);
                return true;
            }
        }

    return false;
}

// =============================================================================
// ����: ������ʱ��������ʱ��Ĺ���Ϊ��������ַ��д�����洢��ַ��д����������ַ������
//       ��������ַ���������ʱ������жϣ��������üĴ���Ϊ����ģʽ��֮�󽫻ᷢ��
//       ���������жϣ����ж��н����յ������ݵ���IIC_PortWriteд�뻺�壬���յ�len��
//       ���������ݺ��ͷ��ź���iic_semp
// ����: SpecificFlag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//      DevAddr,������ַ��ǰ7���أ��ѽ��ڲ���ַ��ռ��bitλ���£��ú����轫�õ�ַ��
//               ��һλ�����������һλ��/д����;
//     MemAddr,�洢���ڲ���ַ�������͵������ϵĵ�ַ���õ�ַδ��������������ַ�ϵ�
//               ����λ��
//      MenAddrLen,�洢���ڲ���ַ�ĳ��ȣ��ֽڵ�λ��δ������������ַ����ı���λ��
//      Length,���յ������������������ݵĵ�����һ�ֽڣ���count-1��ֹͣ����ACK�źţ�����
//             �յ��ֽ���Ϊcountʱ������ֹͣʱ�򣬲��ͷ��ź���iic_semp;
//     IIC_BusSemp,�����ʱ���������ͷŵ��ź������������ź�����
// ����: TRUE��������ʱ��ɹ���FALSEʧ��
// =============================================================================
static bool_t __IIC_GenerateReadStart( tagI2CReg *reg,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct SemaphoreLCB *iic_semp)
{
    u8 mem_addr_buf[4];

    if((u32)reg == CN_IIC1_BASE)
    {
        IntParamset0.TransTotalLen = length;
        IntParamset0.TransCount = 0;
        IntParamset0.pDrvPostSemp = iic_semp;               //iic_buf_semp
    }
    else if((u32)reg == CN_IIC2_BASE)
    {
        IntParamset1.TransTotalLen = length;
        IntParamset1.TransCount = 0;
        IntParamset1.pDrvPostSemp = iic_semp;
    }
    else
    {
        return false;
    }

    fill_little_32bit(mem_addr_buf,0,mem_addr);
    __IIC_IntDisable(reg);//���ж�

    //д������ַ��Ѱַ��ַ
           if(__iic_write_addr(reg, dev_addr, I2C_WRITE_BIT) != 0)
        {
            if(__iic_write(reg, mem_addr_buf, maddr_len) == maddr_len)//�������
            {
                if(length && __iic_write_addr(reg,dev_addr, I2C_READ_BIT) != 0)
                {
                    reg->CR1 |=  I2C_CR1_ACK_MASK;
                    __IIC_IntEnable(reg);//ʹ���ж�
                    return true;
                }
            }
        }
    return false;
}

// =============================================================================
// ����: �������ζ�д�ص����������ֶ�д�Ĳ�ֹͬͣʱ�򣬵����ڷ���ʱ����ֱ��ֹͣʱ��
//      ��Ϊ��������ֹͣ�ظ�ACK����ֹͣ
// ����: reg,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
// ����: ��
// =============================================================================
static void __IIC_GenerateEnd(tagI2CReg *reg)
{
    if(reg == NULL)
        return;
    if((reg->SR1 & I2C_SR1_TxE_MASK )&& (reg->SR1 & I2C_SR1_BTF_MASK))//����
    {
        __IIC_GenerateStop(reg);
    }
    else //����
    {
        reg->CR1 &= ~(I2C_CR1_ACK_MASK);//ֹͣ�ظ�ACK
        __IIC_GenerateStop(reg);
    }
}

// =============================================================================
// ���ܣ�IIC���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�IICʱ��Ƶ������
// ������reg,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
static s32 __IIC_BusCtrl(tagI2CReg *reg,u32 cmd,u32 data1,u32 data2)
{
    if( reg == NULL )
        return 0;
    switch(cmd)
    {
    case CN_IIC_SET_CLK: //����ʱ��Ƶ��
        __IIC_SetClk(reg,data1);
        break;
    case CN_IIC_DMA_USED://ʹ��dma����

        break;
    case CN_IIC_DMA_UNUSED://��ֹdma����

        break;
    case CN_IIC_SET_POLL:           //ʹ����ѯ��ʽ���ͽ���
        __IIC_IntDisable(reg);
        break;
    case CN_IIC_SET_INT:         //ʹ���жϷ�ʽ���ͽ���
        __IIC_IntEnable(reg);
        break;
    default:
        return 0;
    }
    return 1;
}

// =============================================================================
// ���ܣ�        IIC�����뷢���жϷ��������ú���ʵ�ֵĹ������£�
//       1.ÿ���������һ���������ֽڷ���һ���жϣ�
//       2.���ж���ж�ʹ��ͬһ���жϺţ�������ݾ����������ʹ�õ����ĸ��жϣ�
//       3.���жϱ�־�����ж�ACK�źţ�ÿ��д�ֽڣ�������������Ӧ�޸ģ�
//       4.���մﵽ������һ���ֽ�ʱ�������ò�����ACK�źţ�
//       5.���ջ������ʱ����post�ź���IntParam->pDrvPostSemp��
//       6.���ջ������ʱ�������ֹͣʱ��
// ������i2c_int_line,�жϺţ�������û�õ�
// ���أ�true falst
// =============================================================================
static u32 __IIC_ISR(u32 intsub)
{
    static struct IIC_CB *ICB;
    static struct IIC_IntParamSet *IntParam;
    tagI2CReg *reg;
    u8 ch;
    u32 IicErrorNo;
    switch (i2c_int_line)
    {
        case CN_INT_LINE_I2C1_EV:
                reg = (tagI2CReg*)CN_IIC1_BASE;
                ICB = &s_IIC1_CB;
                IntParam = &IntParamset0;
                break;
        case CN_INT_LINE_I2C2_EV:
                reg = (tagI2CReg*)CN_IIC2_BASE;
                ICB = &s_IIC2_CB;
                IntParam = &IntParamset1;
                break;
        default:
                return false;
    }

    if(reg->SR1 & I2C_SR1_BTF_MASK)        //�Ѿ���������
    {
        if(reg->SR1 & I2C_SR1_TxE_MASK)      //�����ж�
        {
            if(!(reg->SR1 & I2C_SR1_RxNE_MASK))
            {
                //�ӷ��ͻ�������һ���ֽڵ����ݣ�������
                if(IIC_PortRead(ICB,&ch,1) > 0)
                {
                    reg->DR = ch;
                    IntParam->TransCount ++;;
                }
                else if(IntParam->TransCount == IntParam->TransTotalLen)
                {
                  Lock_SempPost(IntParam->pDrvPostSemp);
                  __IIC_IntDisable(reg);//���ж�
                  __IIC_GenerateStop(reg);
                }
            }
            else                        //δ�յ�ACK�ź�
            {
                IicErrorNo = CN_IIC_POP_NO_ACK_ERR;//���ô�����API����
                IIC_ErrPop(ICB,IicErrorNo);
                return 1;
            }
        }
        else                            //�����ж�
        {
            while((IntParam->TransCount < IntParam->TransTotalLen))
            {
                // ���һ���ֽ�master����ACK����ʾ��������ֹ
                if(IntParam->TransCount == IntParam->TransTotalLen - 1)
                {
                    reg->CR1 &=~ I2C_CR1_ACK_MASK;
                }
                while (!(reg->SR1 & I2C_SR1_RxNE_MASK));//�ȴ��������
                    ch = reg->DR;
                //д����
                IIC_PortWrite(ICB,&ch,1);
                IntParam->TransCount ++;
            }
            if((IntParam->TransCount == IntParam->TransTotalLen)  &&
                                        (reg->SR1 & I2C_SR1_BTF_MASK))
            {
                __IIC_GenerateStop(reg);
                __IIC_IntDisable(reg);//���ж�
                Lock_SempPost(IntParam->pDrvPostSemp);//�ͷ������ź���
            }
        }
    }
    else//δ����ͨ��
    {
        if(reg->SR1 & I2C_SR1_ARLO_MASK)//�ٲö�ʧ�ж�
        {
            reg->SR1 &= ~I2C_SR1_ARLO_MASK;//����ٲö�ʧ�жϱ�־λ
            IicErrorNo = CN_IIC_POP_MAL_LOST_ERR;
            IIC_ErrPop(ICB,IicErrorNo);
        }
    }

    return true;
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
bool_t ModuleInstall_IIC1(void)
{
    static u8 s_IIC1Buf[IIC1_BUF_LEN];
    struct IIC_Param IIC1_Config;

    IIC1_Config.BusName             = "IIC1";
    IIC1_Config.IICBuf              = (u8*)&s_IIC1Buf;
    IIC1_Config.IICBufLen           = IIC1_BUF_LEN;
    IIC1_Config.SpecificFlag        = CN_IIC1_BASE;
    IIC1_Config.pGenerateWriteStart = (WriteStartFunc)__IIC_GenerateWriteStart;
    IIC1_Config.pGenerateReadStart  = (ReadStartFunc)__IIC_GenerateReadStart;
    IIC1_Config.pGenerateEnd        = (GenerateEndFunc)__IIC_GenerateEnd;
    IIC1_Config.pBusCtrl            = (IICBusCtrlFunc)__IIC_BusCtrl;
    IIC1_Config.pWriteReadPoll      = (WriteReadPoll)__IIC_WriteReadPoll;

    __IIC_DefaultConfig(CN_IIC1);
    __IIC_IntConfig(I2C1_INTR_SUB,__IIC_ISR);

    if(NULL == IIC_BusAdd_s(&s_IIC1_CB,&IIC1_Config))
        return 0;
    return 1;
}

// =============================================================================
// ���ܣ�IIC�ײ������ĳ�ʼ�����������IIC���ߵĳ�ʼ��������Ҫ�������£�
//       1.��ʼ�����߿��ƿ�IIC_CB���ص������ͻ������ĳ�ʼ����ֵ��
//       2.Ĭ�ϵ�Ӳ����ʼ������GPIO��IIC�Ĵ����ȣ�
//       3.�жϳ�ʼ������ɶ�д�ж����ã�
//       4.����IICBusAdd��IICBusAdd_r�������߽�㣻
// ������para,�޾�������
// ���أ�1�ɹ�  0ʧ��
// =============================================================================
bool_t ModuleInstall_IIC2(void)
{
    static u8 s_IIC2Buf[IIC2_BUF_LEN];
    struct IIC_Param IIC2_Config;

    IIC2_Config.BusName             = "IIC2";   //�������ƣ���IIC2
    IIC2_Config.IICBuf              = (u8*)&s_IIC2Buf; //���߻�����ָ��
    IIC2_Config.IICBufLen           = IIC2_BUF_LEN; //���߻�������С���ֽ�
    IIC2_Config.SpecificFlag        = CN_IIC2_BASE; //ָ����ǣ���IIC�Ĵ�����ַ
    IIC2_Config.pGenerateWriteStart = (WriteStartFunc)__IIC_GenerateWriteStart; //д��������
    IIC2_Config.pGenerateReadStart  = (ReadStartFunc)__IIC_GenerateReadStart;//����������
    IIC2_Config.pGenerateEnd        = (GenerateEndFunc)__IIC_GenerateEnd;//����ͨ��
    IIC2_Config.pBusCtrl            = (IICBusCtrlFunc)__IIC_BusCtrl;//���ƺ���
    IIC2_Config.pWriteReadPoll      = (WriteReadPoll)__IIC_WriteReadPoll;//��ѯ���ж�δ��ʱʹ��

    if(false==__IIC_GpioConfig(CN_IIC2))
        return 0;
    __IIC_IntConfig(I2C2_INTR_SUB,__IIC_ISR);

    if(NULL == IIC_BusAdd_s(&s_IIC2_CB,&IIC2_Config))
        return 0;
    return 1;
}
#endif
#endif
