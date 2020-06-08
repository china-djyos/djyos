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
// ����ʱ��: 17/09.2014
// =============================================================================
#include "cpu_peri.h"
#include "djyos.h"
#include "stdint.h"
//#include "arm32_stdint.h"
#include "stdio.h"
#include "iicbus.h"
#include "endian.h"
#include "lock.h"
#include "int.h"
#include "stm32l4xx_hal_conf.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//extern bool_t ModuleInstall_IIC(u8 port);
//    #if CFG_I2C1_ENABLE==1
//    ModuleInstall_IIC(CN_I2C1);
//    #endif
//
//    #if CFG_I2C2_ENABLE==1
//    ModuleInstall_IIC(CN_I2C2);
//    #endif
//
//    #if CFG_I2C3_ENABLE==1
//    ModuleInstall_TWI(CN_I2C3);
//    #endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip iic"//CPU��iic��������
//parent:"iicbus"    //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"iicbus","int","time","lock"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
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
#define CFG_I2C_BUF_LEN         128      //"����IIC��������С",
//%$#@enum,true,false,
#define CFG_I2C1_ENABLE         false    //"�����Ƿ�ʹ��IIC1",
#define CFG_I2C2_ENABLE         false    //"�����Ƿ�ʹ��IIC2",
#define CFG_I2C3_ENABLE         false    //"�����Ƿ�ʹ��IIC3",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

static I2C_TypeDef volatile * const tg_I2c_Reg[] = { I2C1,
                                                     I2C2,
                                                     I2C3};

#define tagI2CReg  I2C_TypeDef
static struct IIC_CB *s_ptIIC_CB[CN_IIC_NUM];


//#define CFG_I2C_BUF_LEN      128

static u32 IIC_BUF_LEN[CN_IIC_NUM]={CFG_I2C_BUF_LEN,CFG_I2C_BUF_LEN,CFG_I2C_BUF_LEN};
//iic������������
static u8 s_IIC1Buf[CFG_I2C_BUF_LEN];
static u8 s_IIC2Buf[CFG_I2C_BUF_LEN];
static u8 s_IIC3Buf[CFG_I2C_BUF_LEN];

static u8* s_IICBuf[CN_IIC_NUM]={s_IIC1Buf,
                                 s_IIC2Buf,
                                 s_IIC3Buf};

struct IIC_IntParamSet
{
    struct SemaphoreLCB *pDrvPostSemp;   //�ź���
    u32 TransCount;       //����������������
    u32 TransTotalLen;//���յ���������
};

static struct IIC_IntParamSet IntParamset[CN_IIC_NUM];

const char *IIC_Name[CN_IIC_NUM]=
         {"IIC1","IIC2","IIC3"};


// =============================================================================
// ���ܣ�IIC����Ƿ��д���
// ������ reg iic����ַ
// ���أ�true/false
// =============================================================================
static bool_t  _IIC_Chek(tagI2CReg *reg)
{
    if (reg->ISR & (I2C_ISR_NACKF))       /*����ٲö�ʧ*/
        {
            #ifdef DEBUG
            printk("__i2c_wait: nackf\n");
            #endif
            return false;
        }
    return true;
}


// =============================================================================
// ����: ʹ��iic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_IntEnable(tagI2CReg *reg)
{
    reg->CR1 |= (I2C_CR1_TXIE|I2C_CR1_RXIE);//���ͺͽ����ж�ͬʱʹ��

}

// =============================================================================
// ����: ��ֹiic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_IntDisable(tagI2CReg *reg)
{
    reg->CR1 &= ~(I2C_CR1_RXIE| I2C_CR1_TXIE);
}


// =============================================================================
// ����: ѡ������ʱ��
// ����: iic_port iic��ţ�
//        sel:ѡ��ʱ�� //bit:0- APB1-54MHz     1-sysclk-216MHz     2- HSI-108MHz
//        TIMINGR �Ĵ������� (4<<28)|(9<<20)|(1<<16)|(39<<8)|(50);
//        ��ʼֵ:0x40912732;
// ����: ��
// =============================================================================

static bool_t __IIC_ClkSet(u8 iic_port,u8 sel,u32 TIMINGR)
{
    if(iic_port>=CN_IIC_NUM)
        return false;
    RCC->CCIPR &=~(3<<((2*iic_port)+12));//���ԭ��������
    RCC->CCIPR |=(sel<<((2*iic_port)+12));
    tg_I2c_Reg[iic_port]->CR1 &=~(1);
    tg_I2c_Reg[iic_port]->TIMINGR=TIMINGR;
    return true;
}


// =============================================================================
// ����: I/Oģ��ķ�ʽ �ͷ�û�и�λ��ʹ�����ŵ�IIC����
// ����: port I/O�� �� CN_GPIO_B
//      TS_SDA:���ź�
//      TS_SCK:���ź�
// ����: true/false
// =============================================================================
bool_t IIC_Busfree(u32 port,u32 sda_pin,u32 sck_pin)
{
     u32 timeout=0;
     GPIO_PowerOn(port);
     GPIO_CfgPinFunc(port,sda_pin,GPIO_MODE_IN, //TS_SDA
                     GPIO_OTYPE_OD,GPIO_SPEED_100M,GPIO_PUPD_PU);
     GPIO_CfgPinFunc(port,sck_pin,GPIO_MODE_OUT,  //TS_SCK
                     GPIO_OTYPE_OD,GPIO_SPEED_100M,GPIO_PUPD_PU);
     while(1)
     {
          timeout++;
          GPIO_SettoLow(port,sck_pin);
          DJY_DelayUs(10);

          GPIO_SettoHigh(port,sck_pin);
          DJY_DelayUs(10);

          if(timeout>=CONFIG_I2C_TIMEOUT)
              return false;
          if( GPIO_GetData(port)&(sda_pin))
              break;
    }
     GPIO_CfgPinFunc(port,sda_pin,GPIO_MODE_OUT,  //TS_SCK
                         GPIO_OTYPE_OD,GPIO_SPEED_100M,GPIO_PUPD_PU);
    //����ֹͣ�ź� iic�����ͷ�
    GPIO_SettoLow(port,sda_pin);
    DJY_DelayUs(10);
    GPIO_SettoHigh(port,sda_pin);
    DJY_DelayUs(10);

    return true;
}

// =============================================================================
// ����: Ӳ��GPIO��ʼ��������ʱ��ѡ���iic��ʼ������
// ����: IIC_NO,���
// ����: true/false
// =============================================================================
static bool_t __IIC_HardConfig(u8 iic_port)
{
    RCC->APB1ENR1 |=(1<<(21+iic_port));//����ʱ��ʹ��
    //0- APB1-54    1-sysclk-216     2- HSI-108
    if(__IIC_ClkSet(iic_port,0,0x10E0A1E1)==false)//Ĭ��ʱ��ԴΪAPB1ʱ��
        return false;

    tg_I2c_Reg[iic_port]->CR1 &=~I2C_CR1_PE;//PEʹ��
    tg_I2c_Reg[iic_port]->CR1 &=~I2C_CR1_ANFOFF;//ʱ��50ns����ʱ��Ҫд1��ֹģ���˲�����С����
    tg_I2c_Reg[iic_port]->CR1 |=I2C_CR1_DNF_4BIT(0);//���˳�������������� 0~15 tI2CCLK
    tg_I2c_Reg[iic_port]->CR1 &=~I2C_CR1_NOSTRETCH;//
    tg_I2c_Reg[iic_port]->CR2 |=I2C_CR2_AUTOEND;//�Զ�����ģʽ
    tg_I2c_Reg[iic_port]->CR2 &=~I2C_CR2_ADD10;//7λѰַģʽ
    tg_I2c_Reg[iic_port]->OAR1=0x8000;//�����ַ
    tg_I2c_Reg[iic_port]->CR1  = I2C_CR1_PE;//PEʹ��

    return true;

}
// RELOAD CR2_24 λ�� 1���ڸ�ģʽ�£���� NBYTES
//��������ֽ��������ݴ���֮��TCR ��־���� 1������ TCIE �� 1 ʱ�������жϡ�ֻҪ
//TCR ��־�� 1��SCL ����ӳ������� NBYTES д��һ������ֵʱ��TCR ��������㡣
//���� NBYTE ���������һ�δ�����ֽ���ǰ������� RELOAD λ����
// =============================================================================
// ����: IIC�ж����ú���
// ����: IntLine,�ж���
//       isr,�жϷ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_IntConfig(u8 iic_port,u32 (*isr)(ptu32_t))
{
        ufast_t IntLine;
        switch (iic_port)
        {
        case CN_IIC1:
            IntLine=CN_INT_LINE_I2C1_EV;
            break;
        case CN_IIC2:
            IntLine=CN_INT_LINE_I2C2_EV;
            break;
        case CN_IIC3:
            IntLine=CN_INT_LINE_I2C3_EV;
            break;
            default:return;
        }
        Int_Register(IntLine);
        Int_IsrConnect(IntLine,isr);
        Int_SettoAsynSignal(IntLine);
        Int_ClearLine(IntLine);     //�����ʼ�������ķ���fifo�յ��ж�
        Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// ����: ����ֹͣʱ�򣬽���������������֮���ͨ��
// ����: reg,iic����Ĵ�����ַ
// ����: ��
// =============================================================================
static void __IIC_GenerateStop(tagI2CReg *reg)
{
    reg->CR2 |=I2C_CR2_STOP;//����ֹͣλ

}

// =============================================================================
// ���ܣ�IICд
// ������ reg,�Ĵ�����ַ
//        devaddr,���������ַ�����һbit���ֶ�д
//         buf_addr��ַд����
//       len_addr ��ַ����
//         bufд����
//       len ����
// ���أ�true/false
// =============================================================================
static bool_t __IIC_Write(tagI2CReg *reg,u8 devaddr, u8 *adder_nuf, u32 addr_len,u8 *buf, u32 len)
{
    u32 timeout=0;
    u32 i=0;
    reg->CR2 &=~I2C_CR2_SADD_10BIT(0x7f<<1);
    reg->CR2 |=I2C_CR2_SADD_10BIT(devaddr<<1);//��������ַ
    reg->CR2 &=~I2C_CR2_RD_WRN;//д����
     if(reg->ISR|=I2C_ISR_STOPF)
            reg->ICR|=I2C_ICR_STOPCF;
     if(reg->ISR|=I2C_ISR_NACKF)
            reg->ICR|=I2C_ICR_NACKCF;
     len=len+addr_len;
    if(len>255)
    {
        reg->CR2 |=I2C_CR2_RELOAD;//�����ֽڴ���255
        reg->CR2 |=I2C_CR2_NBYTES_8BIT(0xFF);
        reg->CR2 |=I2C_CR2_AUTOEND;//�Զ�������ʽ
        reg->CR2 |=I2C_CR2_START;//start

        while(timeout<CONFIG_I2C_MBB_TIMEOUT)
        {

            if((reg->ISR&I2C_ISR_TXE)&&(reg->ISR&I2C_ISR_TXIS)\
                      &&(!(reg->ISR&I2C_ISR_NACKF)))//�����ֽ�
            {
                if(i<addr_len)
                    *((volatile u8 *)&reg->TXDR)=adder_nuf[i];
                else
                    *((volatile u8 *)&reg->TXDR)=buf[i-addr_len];
                i++;
            }
            else//�жϳ�����߳�ʱ
            {
                if(_IIC_Chek(reg)==false)
                    return false;
                timeout++;
                DJY_DelayUs(10);
            }

            if(reg->ISR&I2C_ISR_TCR)//255�ֽڴ������
            {
                len-=255;
                if(len>255)
                {
                    reg->CR2 |=I2C_CR2_NBYTES_8BIT(0xFF);
                    reg->CR2 |=I2C_CR2_RELOAD;//�����ֽڴ���255
                }
                else
                {
                    reg->CR2 &=~I2C_CR2_NBYTES_8BIT(0xff);//����
                    reg->CR2 |=I2C_CR2_NBYTES_8BIT(len);
                    reg->CR2 &=~I2C_CR2_RELOAD;//�����ֽ�

                }
            }
            //�����ǽ���
            if((reg->ISR&I2C_ISR_TC)||(reg->ISR&I2C_ISR_STOPF))//�������
            {
                __IIC_GenerateStop(reg);
                return true;
            }
        }
        return false;

    }
    //д�ֽ�С��255�ֽ�
    else
    {
        reg->CR2 &=~I2C_CR2_RELOAD;//С��255�ֽ�
        reg->CR2 &=~I2C_CR2_NBYTES_8BIT(0xff);//����
        reg->CR2 |=I2C_CR2_NBYTES_8BIT(len);
        reg->CR2 |=I2C_CR2_AUTOEND;//�Զ�������ʽ
        reg->CR2 |=I2C_CR2_START;//start
        while(timeout<CONFIG_I2C_MBB_TIMEOUT)
        {

            if((reg->ISR&I2C_ISR_TXE)&&(reg->ISR&I2C_ISR_TXIS)\
                  &&(!(reg->ISR&I2C_ISR_NACKF)))//�����ֽ�
            {

                if(i<addr_len)
                     reg->TXDR = adder_nuf[i];
                else
                    reg->TXDR= buf[i-addr_len];
                i++;
            }

            else//�жϳ�����߳�ʱ
            {
                if(_IIC_Chek(reg)==false)
                    return false;
                timeout++;
                DJY_DelayUs(10);
            }
            //�ж��Ƿ������
            if((reg->ISR&I2C_ISR_TC)||(reg->ISR&I2C_ISR_STOPF))//�������
            {
                if(reg->ISR|I2C_ISR_STOPF)
                    return true;
                if(_IIC_Chek(reg)==false)
                    return false;
                return true;
            }
        }
        return false;
    }
}
// =============================================================================
// ���ܣ�IICд��ַ
// ������ reg,�Ĵ�����ַ
//        devaddr,���������ַ�����һbit���ֶ�д
//         mem_addrд����
//       maddrlen ����
//        Start_falg������ʼֹͣ����Ϊ
// ���أ�true/false
// =============================================================================
static bool_t __IIC_WriteAddr(tagI2CReg *reg,u8 devaddr, u8 *mem_addr, u32 maddrlen)
{
    u32 timeout=0;
    u32 i=0;
    reg->CR2 &=~I2C_CR2_SADD_10BIT(0x7f<<1);
    reg->CR2 |=I2C_CR2_SADD_10BIT(devaddr<<1);//��������ַ
    reg->CR2 &=~I2C_CR2_RD_WRN;//д����
     if(reg->ISR|=I2C_ISR_STOPF)
            reg->ICR|=I2C_ICR_STOPCF;
     if(reg->ISR|=I2C_ISR_NACKF)
            reg->ICR|=I2C_ICR_NACKCF;

    reg->CR2 &=~I2C_CR2_RELOAD;//С��255�ֽ�
    reg->CR2 &=~I2C_CR2_NBYTES_8BIT(0xff);//����
    reg->CR2 |=I2C_CR2_NBYTES_8BIT(maddrlen);
    reg->CR2 |=I2C_CR2_AUTOEND;//�Զ�������ʽ
    reg->CR2 |=I2C_CR2_START;//start
    while(timeout<CONFIG_I2C_MBB_TIMEOUT)
    {

        if((reg->ISR&I2C_ISR_TXE)&&(reg->ISR&I2C_ISR_TXIS)\
                &&(!(reg->ISR&I2C_ISR_NACKF)))//�����ֽ�
        {
            *((volatile u8 *)&reg->TXDR)=mem_addr[i++];
        }
        else//�жϳ�����߳�ʱ
        {
            if(_IIC_Chek(reg)==false)
                return false;
            timeout++;
            DJY_DelayUs(10);
        }
        //�ж��Ƿ������
        if((reg->ISR&I2C_ISR_TC)||(reg->ISR&I2C_ISR_STOPF))//�������
        {
//                __IIC_GenerateStop(reg);
                return(_IIC_Chek(reg));
        }
    }
    return false;

}

// =============================================================================
// ���ܣ�IIC��
// ������ reg,�Ĵ�����ַ
//        devaddr ������ַ
//         buf�ջ�����
//       len ����
// ���أ�true/false
// =============================================================================
static bool_t __IIC_Read(tagI2CReg *reg,u8 devaddr,u8 *buf, u32 len)
{
    u32 timeout=0,i=0;
    u32 timedef;
    reg->CR2 &=~I2C_CR2_SADD_10BIT(0x7f<<1);
    reg->CR2 |=I2C_CR2_SADD_10BIT(devaddr<<1);//��������ַ
    reg->CR2 |=I2C_CR2_RD_WRN;//������λ
    timedef=CONFIG_I2C_TIMEOUT*len;
    if(reg->ISR|=I2C_ISR_STOPF)//���ֹͣλ
        reg->ICR|=I2C_ICR_STOPCF;
    if(reg->ISR|=I2C_ISR_NACKF)
            reg->ICR|=I2C_ICR_NACKCF;
    if(len>255)
    {
        reg->CR2 |=I2C_CR2_RELOAD;//����255�ֽ�
        reg->CR2 |=I2C_CR2_NBYTES_8BIT(0xff);
        reg->CR2 |=I2C_CR2_AUTOEND;//�Զ�������ʽ
        reg->CR2 |=I2C_CR2_START;//start

        while((timeout<timedef))
        {
            if(reg->ISR&I2C_ISR_RXNE)//�����ֽ�
            {
                buf[i++]=reg->RXDR;
            }
            else//�жϳ�����߳�ʱ
            {
                if(_IIC_Chek(reg)==false)
                    return false;
                timeout++;
                DJY_DelayUs(10);
            }

            if(reg->ISR&I2C_ISR_TCR)//255�ֽڴ������
            {
                len-=255;
                if(len>255)
                {
                    reg->CR2 |=I2C_CR2_NBYTES_8BIT(0xFF);
                    reg->CR2 |=I2C_CR2_RELOAD;//�����ֽڴ���255
                }
                else
                {
                    reg->CR2 &=~I2C_CR2_NBYTES_8BIT(0xff);//����
                    reg->CR2 |=I2C_CR2_NBYTES_8BIT(len);
                    reg->CR2 &=~I2C_CR2_RELOAD;//�����ֽ�
//                    reg->CR2 |=I2C_CR2_AUTOEND;//�Զ�����ֹͣλ

                }
            }
            //�����ǽ���
            if((!(reg->ISR&I2C_ISR_RXNE))&&    //���һλ��ȡ���
            ((reg->ISR&I2C_ISR_TC)))
            {
                __IIC_GenerateStop(reg);
                return true;
            }
        }
        return false;
    }
    //��С��255�ֽ�
    else
    {
        reg->CR2 &=~I2C_CR2_RELOAD;//С��255�ֽ�
        reg->CR2 &=~I2C_CR2_NBYTES_8BIT(0xff);//����
        reg->CR2 |=I2C_CR2_NBYTES_8BIT(len);
        reg->CR2 |=I2C_CR2_AUTOEND;//�Զ�������ʽ
        reg->CR2 |=I2C_CR2_START;//start


        while((timeout<timedef))
        {
            if(reg->ISR&I2C_ISR_RXNE)//�����ֽ�
            {
                buf[i++]=reg->RXDR;
            }
            else
            {
                if(_IIC_Chek(reg)==false)
                    return false;
                timeout++;
                DJY_DelayUs(10);
            }
            //�ж��Ƿ�����ɴ�����ɻ����Ѿ�����ֹͣλ
            if((!(reg->ISR&I2C_ISR_RXNE))&&    //���һλ��ȡ���
            (reg->ISR&I2C_ISR_TC))//(reg->ISR&I2C_ISR_STOPF)
            {
                __IIC_GenerateStop(reg);//���Զ���ʽ��Ҫ�ֶ�����
                return true;
            }
        }
        return false;
    }

}

// =============================================================================
// ���ܣ���ѯ��ʽ��IIC���豸������
// ������reg,�Ĵ�����ַ
//       devaddr,���������ַ�����һbit���ֶ�д
//         memaddr//�Ĵ�����ַ
//         maddrlen�ļĴ�����ַ�ĳ���
//       addr,���ʵļĴ�����ַ
//       buf, �洢������
//       len, �洢�������ĳ���
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================
static s32 __IIC_ReadPoll(tagI2CReg *reg,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{

    u8 mem_addr_buf[4];
    //����ַ����С�˱任
    u8 i;
    for(i=0;i<maddrlen;i++)
    {
        mem_addr_buf[(maddrlen-1)-i]=((memaddr>>(8*i))&0xff);
    }
    // ����д������ַ���ڲ��Ĵ�����ַ
    if(false == __IIC_WriteAddr(reg, devaddr, mem_addr_buf, maddrlen))
        return -1;

    if(false == __IIC_Read(reg, devaddr, buf,len))
        return -1;
    __IIC_GenerateStop(reg);//���Զ���ʽ��Ҫ�ֶ�����
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
static s32 __IIC_WritePoll(tagI2CReg *reg,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{
    u8 mem_addr_buf[4];
    u8 i;
    for(i=0;i<maddrlen;i++)
    {
        mem_addr_buf[(maddrlen-1)-i]=((memaddr>>(8*i))&0xff);
    }

    if( __IIC_Write(reg, devaddr, mem_addr_buf, maddrlen,buf,len)==false)//д�洢����ַ
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
    static bool_t flag;

    __IIC_IntDisable(reg);
    reg->CR1 |= I2C_CR1_PE;//PEʹ��
    if(WrRdFlag == CN_IIC_WRITE_FLAG)   //д
    {
        if(Length == __IIC_WritePoll(reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            flag = true;
        else
            flag = false;
    }
    else                                //��
    {
        if(Length == __IIC_ReadPoll(reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            flag = true;
        else
            flag = false;
    }
    reg->CR1 &=~I2C_CR1_PE;//PEʧ��

    return  flag ;
}

// =============================================================================
// ����: ����дʱ������дʱ��Ĺ���Ϊ��������ַ��д�����洢��ַ��д�������洢��ַ��
//       ��ʱ������жϣ�֮�󽫻ᷢ�������жϣ����жϷ���
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
    u8 iic_port;
    //ͨ��specific_flag��������������
    if(reg==tg_I2c_Reg[CN_IIC1])
        iic_port=CN_IIC1;
    else if (reg==tg_I2c_Reg[CN_IIC2])
        iic_port=CN_IIC2;
    else if (reg==tg_I2c_Reg[CN_IIC3])
        iic_port=CN_IIC3;
    else
        return false;

    IntParamset[iic_port].TransTotalLen = length;
    IntParamset[iic_port].TransCount = 0;
    IntParamset[iic_port].pDrvPostSemp = iic_semp;           //iic_bus_semp

    //����ַ����С�˱任

    u8 i;
    for(i=0;i<maddr_len;i++)
    {
        mem_addr_buf[(maddr_len-1)-i]=((mem_addr>>(8*i))&0xff);
    }
    __IIC_IntDisable(reg);                 //���ж�
    //�ȷ���������ַ���ڲ���ַ��Ȼ���͵�һ������
    if(__IIC_WriteAddr(reg, dev_addr, mem_addr_buf,maddr_len) )
    {
        reg->CR2 &=~I2C_CR2_RD_WRN;//д��־
        reg->CR2 |=I2C_CR2_RELOAD;//�����ֽڴ���255
        reg->CR2 |=I2C_CR2_NBYTES_8BIT(0xFF);
        reg->CR2 |=I2C_CR2_AUTOEND;//�Զ�������ʽ
        __IIC_IntEnable(reg);//ʹ���ж�
        reg->CR2 |=I2C_CR2_START;//start
        return true;
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
    u8 iic_port;
    if(reg==tg_I2c_Reg[CN_IIC1])
        iic_port=CN_IIC1;
    else if(reg==tg_I2c_Reg[CN_IIC2])
        iic_port=CN_IIC2;
    else if(reg==tg_I2c_Reg[CN_IIC3])
        iic_port=CN_IIC3;
    else
        return false;

    IntParamset[iic_port].TransTotalLen = length;
    IntParamset[iic_port].TransCount = 0;
    IntParamset[iic_port].pDrvPostSemp = iic_semp;               //iic_buf_semp

    u8 i;
    for(i=0;i<maddr_len;i++)
    {
        mem_addr_buf[(maddr_len-1)-i]=((mem_addr>>(8*i))&0xff);
    }
    __IIC_IntDisable(reg);//���ж�

    //д������ַ��Ѱַ��ַ
    //д������ַ��Ѱַ��ַ
    if(__IIC_WriteAddr(reg, dev_addr, mem_addr_buf,maddr_len) )
    {
        reg->CR2 |=I2C_CR2_AUTOEND;//�Զ�������ʽ
        reg->CR2 |=I2C_CR2_RD_WRN;//����־
        __IIC_IntEnable(reg);//ʹ���ж�
        reg->CR2 |=I2C_CR2_START;//start
        return true;
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
    if(reg->CR2 & I2C_CR2_RD_WRN )//����
    {
        reg->CR1 |= I2C_CR2_NACK;//ֹͣ�ظ�ACK
        __IIC_GenerateStop(reg);
    }
    else //����
    {
        __IIC_GenerateStop(reg);
    }
}

//==============================================================================
// ���ܣ�IIC���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�IICʱ��Ƶ������
// ������TIMINGR-->λ 31:28 PRESCʱ��Ԥ��Ƶ����
//                   λ 23:20 SCLDEL[3:0]�����ݽ���ʱ��
//                   λ 19:16 SDADEL[3:0]�����ݱ���ʱ��
//                   λ 15:8 SCLH[7:0]��SCL �ߵ�ƽ����
//                   λ 7:0 SCLL[7:0]��SCL �͵�ƽ����
//   reg    ��ģ���ڼ�IIC�Ĵ�����ַ
// ���أ���
//==============================================================================
static void _IIC_ClkSet(tagI2CReg *reg,u32 data)
{
    u32 SCLH,SCLL;
    reg->CR1 &=~I2C_CR1_PE;
    SCLH=(CN_CFG_MCLK)/(data*2);
    SCLL=SCLH;
    reg->TIMINGR&=~((I2C_TIMINGE_SCLH_8BIT(0xff)|I2C_TIMINGE_SCLL_8BIT(0xff)));
    reg->TIMINGR|=(I2C_TIMINGE_SCLH_8BIT(SCLH)|I2C_TIMINGE_SCLL_8BIT(SCLL));
    reg->CR1 |=I2C_CR1_PE;//PEʹ��
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
                _IIC_ClkSet(reg,data1);
              break;
        case CN_IIC_DMA_USED://ʹ��dma����

            break;
        case CN_IIC_DMA_UNUSED://��ֹdma����

            break;
        case CN_IIC_SET_POLL:           //ʹ����ѯ��ʽ���ͽ���
            __IIC_IntDisable(reg);
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
static u32 __IIC_ISR(ufast_t i2c_int_line)
{
    static struct IIC_CB *ICB;
    static struct IIC_IntParamSet *IntParam;
    tagI2CReg *reg;
    u8 iic_port;
    u8 ch;
    u32 IicErrorNo;
    switch (i2c_int_line)
    {
        case CN_INT_LINE_I2C1_EV:
            iic_port=CN_IIC1;
            break;
        case CN_INT_LINE_I2C2_EV:
            iic_port=CN_IIC2;
            break;
        case CN_INT_LINE_I2C3_EV:
            iic_port=CN_IIC3;
            break;
        default:    return false;
    }
    reg = (tagI2CReg*)tg_I2c_Reg[iic_port];
    ICB = s_ptIIC_CB[iic_port];
    IntParam = &IntParamset[iic_port];

    //�ж��жϷ�ʽ
    //����ж�
    //���ͻ��߽�������
    //������
    if(reg->ISR & I2C_ISR_TXIS) //�����ж�
    {
        if(IIC_PortRead(ICB,&ch,1) > 0)
        {
            *((volatile u8 *)&reg->TXDR) = ch;
            IntParam->TransCount ++;;
        }
        else if(IntParam->TransCount == IntParam->TransTotalLen)
        {
          Lock_SempPost(IntParam->pDrvPostSemp);
          __IIC_IntDisable(reg);//���ж�
          __IIC_GenerateStop(reg);
        }
    }
    else if(reg->ISR & I2C_ISR_RXNE)//�����ж�
    {
        while((IntParam->TransCount < IntParam->TransTotalLen))
        {
            // ���һ���ֽ�master����ACK����ʾ��������ֹ
            if(IntParam->TransCount == IntParam->TransTotalLen - 1)
            {
                reg->CR2 |= I2C_CR2_NACK;
            }

            while (reg->ISR&I2C_ISR_RXNE)//�����ֽ�
                ch = reg->RXDR;
            //д����
            IIC_PortWrite(ICB,&ch,1);
            IntParam->TransCount ++;
        }
        if((IntParam->TransCount == IntParam->TransTotalLen)  &&
                                    (!(reg->ISR&I2C_ISR_RXNE)))//���һλ��ȡ���
        {
            __IIC_GenerateStop(reg);
            __IIC_IntDisable(reg);//���ж�
            Lock_SempPost(IntParam->pDrvPostSemp);//�ͷ������ź���
        }

    }

    else//δ����ͨ��
    {
        if(reg->ISR & (I2C_ISR_ARLO))//�ٲö�ʧ�ж�
        {
            reg->ICR = I2C_ICR_ARLOCF;//����ٲö�ʧ�жϱ�־λ
            IicErrorNo = CN_IIC_POP_MAL_LOST_ERR;
            IIC_ErrPop(ICB,IicErrorNo);
        }
        if(reg->ISR & (I2C_ISR_NACKF))
        {
            reg->ICR = I2C_ICR_ARLOCF;//����ٲö�ʧ�жϱ�־λ
            IicErrorNo = CN_IIC_POP_NO_ACK_ERR;
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
// ������iic_port  IIC��� CN_IIC1
//       Add_mode Ѱַģʽ
// ���أ���
// =============================================================================
bool_t ModuleInstall_IIC(u8 iic_port)
{

    struct IIC_Param IIC_Config[CN_IIC_NUM];

    IIC_Config[iic_port].BusName             = (char *)IIC_Name[iic_port];
    IIC_Config[iic_port].IICBuf              = (u8*)s_IICBuf[iic_port];
    IIC_Config[iic_port].IICBufLen           = IIC_BUF_LEN[iic_port];
    IIC_Config[iic_port].SpecificFlag        = (ptu32_t)tg_I2c_Reg[iic_port];
    IIC_Config[iic_port].pGenerateWriteStart = (WriteStartFunc)__IIC_GenerateWriteStart;
    IIC_Config[iic_port].pGenerateReadStart  = (ReadStartFunc)__IIC_GenerateReadStart;
    IIC_Config[iic_port].pGenerateEnd        = (GenerateEndFunc)__IIC_GenerateEnd;
    IIC_Config[iic_port].pBusCtrl            = (IICBusCtrlFunc)__IIC_BusCtrl;
    IIC_Config[iic_port].pWriteReadPoll      = (WriteReadPoll)__IIC_WriteReadPoll;

    __IIC_HardConfig(iic_port);
    __IIC_IntConfig(iic_port,__IIC_ISR);

    s_ptIIC_CB[iic_port] = IIC_BusAdd(&IIC_Config[iic_port]);
    if(s_ptIIC_CB[iic_port])
        return 0;
    else
        return 1;
}
