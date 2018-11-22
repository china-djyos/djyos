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

// �ļ���     ��board.c
// ģ������: �����ز��ֳ�ʼ�������õ�
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 5/11.2015
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "os.h"
#include "cpu_peri.h"

// =============================================================================
// ===========================����IIC��IOģ��ʹ��=================================
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //out
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //in
// =============================================================================


// =============================================================================
// ���ܣ����ݾ���İ�����ô��ڵ�GPIO�����Ź��ܣ�����������أ����Ըú������ڸ��ļ���CPU
//      ��������ֱ�ӵ��øú�������ʼ�����ڵ�GPIO����
//      ��Ҫ����GPIO�˿ںʹ���ʱ��ʹ�ܡ�GPIO���á���ӳ�䡢ʱ�ӵ�
// ��������
// ���أ�true
// =============================================================================
bool_t Board_UartGpioInit(u8 SerialNo)
{
    //��ʼ��IO�˿�λuart����
    //Ports  :  GPA10 GPA9 GPA8 GPA7 GPA6 GPA5 GPA4 GPA3 GPA2 GPA1 GPA0
    //Signal :   RXD1 TXD1  xx   xx   xx   xx   xx   xx   xx   xx   xx
    switch(SerialNo)
    {
    case CN_UART1:
        RCC->AHB1ENR|=1<<0;     //ʹ��PORTA��ʱ��
        RCC->APB2ENR|=1<<4;     //ʹ�ܴ���1ʱ��
        GPIO_CfgPinFunc(GPIO_A,PIN9|PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,���ù���,�������
        GPIO_AFSet(GPIO_A,9,7); //PA9,AF7
        GPIO_AFSet(GPIO_A,10,7);//PA10,AF7
        break;
    case CN_UART2:
        RCC->APB1ENR |= (1<<17);
        RCC->AHB1ENR |= (1<<0);//port A clock enable
        GPIO_CfgPinFunc(GPIO_A,PIN2|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,���ù���,�������
        GPIO_AFSet(GPIO_A,2,7); //PA2,AF7
        GPIO_AFSet(GPIO_A,3,7);//PA3,AF7
        break;
    case CN_UART3:
        RCC->APB1ENR |= (1<<18);//uart3 clock enable
        RCC->AHB1ENR |= (1<<1);// port b clock enable
        GPIO_CfgPinFunc(GPIO_B,PIN10|PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);
        GPIO_AFSet(GPIO_B,10,7);    //PB10,AF7
        GPIO_AFSet(GPIO_B,11,7);//PB11,AF7
        break;
    case CN_UART4:
        RCC->APB1ENR |= (1<<19);//uart4 clock enable
        RCC->AHB1ENR |= (1<<0);//port A clock enable
        GPIO_CfgPinFunc(GPIO_A,PIN0|PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,���ù���,�������
        GPIO_AFSet(GPIO_A,0,7); //PA2,AF7
        GPIO_AFSet(GPIO_A,1,7);//PA3,AF7
        break;
    case CN_UART5:
        RCC->APB1ENR |= (1<<20);//uart5 clock enable
        RCC->AHB1ENR |= (1<<2);//port C clock enable
        GPIO_CfgPinFunc(GPIO_C,PIN11|PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,���ù���,�������
        GPIO_AFSet(GPIO_C,11,7);    //PA2,AF7
        GPIO_AFSet(GPIO_C,12,7);//PA3,AF7
        break;
    case CN_UART6:
        RCC->APB2ENR|= (1<<5);      //ʹ�ܴ���6ʱ��
        RCC->AHB1ENR |= (1<<2);//port C clock enable
        GPIO_CfgPinFunc(GPIO_C,PIN6|PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,���ù���,�������
        GPIO_AFSet(GPIO_C,6,7); //PA2,AF7
        GPIO_AFSet(GPIO_C,7,7);//PA3,AF7
        break;
    default:
        break;
    }
    return true;
}

// =============================================================================
// ���ܣ����ݾ���İ������IIC��GPIO�����Ź��ܣ�����������أ����Ըú������ڸ��ļ���CPU
//      IIC����ֱ�ӵ��øú�������ʼ�����ڵ�GPIO����
//      ��Ҫ����GPIO�˿ں�����ʱ��ʹ�ܡ�GPIO���á���ӳ�䡢ʱ�ӵ�
// ��������
// ���أ�true
// =============================================================================
bool_t Board_IicGpioInit(u8 I2Cx)
{
    if(I2Cx == CN_I2C1)
    {
        RCC->AHB1ENR|=1<<1;    //ʹ��PORTBʱ��

        GPIO_CfgPinFunc(GPIO_B,PIN6|PIN7,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,���ù���,�������
        return true;
    }
    return false;
}

void IIC_SetDaIn(u8 IICx)
{
    if(IICx == CN_I2C1)
    {
        GPIOB->MODER&=~(3<<(6*2));
        GPIOB->MODER|=0<<6*2;   //PB6����ģʽ
    }
    else
    {
        //����δ�õ�
    }
}
void IIC_SetDaOut(u8 IICx)
{
    if(IICx == CN_I2C1)
    {
        GPIOB->MODER&=~(3<<(6*2));
        GPIOB->MODER|=1<<6*2;//PB6���ģʽ
    }
    else
    {
        //����δ�õ�
    }
}
void IIC_Scl(u8 IICx,u8 level)
{
    if(IICx == CN_I2C1)
    {
        PBout(7) = level; //SCL;
    }
}
void IIC_Sda(u8 IICx,u8 level)
{
    if(IICx == CN_I2C1)
    {
        PBout(6) = level; //SDA
    }
}

u8 IIC_ReadSda(u8 IICx)
{
    u8 i = 0;
    if(IICx == CN_I2C1)
    {
        i = PBin(6);
    }
    return i;
}

// =============================================================================
// ���ܣ����ݾ���İ������SPI��GPIO�����Ź��ܣ�����������أ����Ըú������ڸ��ļ���CPU
//      SPI����ֱ�ӵ��øú�������ʼ�����ڵ�GPIO����
//      ��Ҫ����GPIO�˿ں�����ʱ��ʹ�ܡ�GPIO���á���ӳ�䡢ʱ�ӵ�
// ��������
// ���أ�true
// =============================================================================
bool_t Board_SpiGpioInit(u8 SPIx)
{
    if(SPIx == CN_SPI1)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;     //ʹ��I2C1ʱ��

        RCC->APB2RSTR   |=  RCC_APB2RSTR_SPI1;  //��λSPI1
        RCC->APB2RSTR   &=  ~RCC_APB2RSTR_SPI1; //ֹͣ��λSPI1
        //MOSI,MISO,SCK,CSS
        GPIO_CfgPinFunc(GPIO_A,PIN5|PIN6|PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,���ù���,�������
//      GPIO_AFSet(GPIO_A,4,5);
        GPIO_AFSet(GPIO_A,5,5);
        GPIO_AFSet(GPIO_A,6,5);
        GPIO_AFSet(GPIO_A,7,5);
    }
    else
    {
        return false;
    }
    return true;
}
















