//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
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
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern void Board_GpioInit(void);
//    Board_GpioInit();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"boardinit"    //�����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���             //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"cpu_peri_gpio"    //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure

// =============================================================================
// ���ܣ����ݾ���İ�����ô��ڵ�GPIO�����Ź��ܣ�����������أ����Ըú������ڸ��ļ���CPU
//      ��������ֱ�ӵ��øú�������ʼ�����ڵ�GPIO����
//      ��Ҫ����GPIO�˿ںʹ���ʱ��ʹ�ܡ�GPIO���á���ӳ�䡢ʱ�ӵ�
// ��������
// ���أ�true
// =============================================================================
bool_t Board_UartGpioInit(u8 SerialNo)
{
    switch(SerialNo)
    {
    case CN_UART1:
        RCC->APB2ENR|=1<<4;      //ʹ�ܴ���1ʱ��
        GPIO_PowerOn(GPIO_A);
        GPIO_CfgPinFunc(GPIO_A,PIN9|PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,���ù���,�������
         GPIO_AFSet(GPIO_A,9,7);
        GPIO_AFSet(GPIO_A,10,7);
        break;
    case CN_UART2:
        RCC->APB1ENR|=1<<17;     //ʹ�ܴ���1ʱ��
        GPIO_PowerOn(GPIO_D);
        GPIO_CfgPinFunc(GPIO_D,PIN5|PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,���ù���,�������
         GPIO_AFSet(GPIO_D,5,7);
        GPIO_AFSet(GPIO_D,6,7);
        break;
    case CN_UART3:
        RCC->APB1ENR |= (1<<18);//uart3 clock enable
        GPIO_PowerOn(GPIO_D);
        GPIO_CfgPinFunc(GPIO_D,PIN8|PIN9,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);
         GPIO_AFSet(GPIO_D,8,7);
        GPIO_AFSet(GPIO_D,9,7);
        break;
    case CN_UART4:
        RCC->APB1ENR |= (1<<19);//uart4 clock enable
        GPIO_PowerOn(GPIO_A);
        GPIO_CfgPinFunc(GPIO_A,PIN0|PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);
         GPIO_AFSet(GPIO_A,0,8);
        GPIO_AFSet(GPIO_A,1,8);
        break;
    case CN_UART5:
        RCC->APB1ENR |= (1<<20);//uart5 clock enable
        GPIO_PowerOn(GPIO_C);;//port C clock enable
        GPIO_PowerOn(GPIO_D);;//port D clock enable
        GPIO_CfgPinFunc(GPIO_C,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,���ù���,�������
        GPIO_CfgPinFunc(GPIO_D,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,���ù���,�������
        GPIO_AFSet(GPIO_C,12,8);
        GPIO_AFSet(GPIO_D,2,8);
        break;
    case CN_UART6:
        RCC->APB2ENR|= (1<<5);      //ʹ�ܴ���6ʱ��
        GPIO_PowerOn(GPIO_C);;//port C clock enable
        GPIO_CfgPinFunc(GPIO_C,PIN6|PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU);
         GPIO_AFSet(GPIO_C,6,8);
        GPIO_AFSet(GPIO_C,7,8);
        break;
    default:
        break;
    }
    return true;
}
// =============================================================================
// ���ܣ����ݾ���İ��485����Ŀ��ƹܽţ�����485Ϊ����״̬�ð����Ӳ����·ʵ���������
// ���������ں��磺CN_UART1
// ���أ���
// =============================================================================

 void Board_UartHalfDuplexSend(u8 SerialNo)
{
    switch(SerialNo)
    {
        case CN_UART1:break;
        case CN_UART2:break;
        default:      break;
    }
}
 // =============================================================================
 // ���ܣ����ݾ���İ��485����Ŀ��ƹܽţ�����485Ϊ������״̬
 // ���������ں��磺CN_UART1
 // ���أ���
 // =============================================================================

void Board_UartHalfDuplexRecv(u8 SerialNo)
{

    switch(SerialNo)
    {
    case CN_UART1: break;
    case CN_UART2: break;
    default:       break;
    }
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

    return true;
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
    switch (SPIx)
    {
        case CN_SPI1:
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // ʹ��PORTBʱ��
            RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // ʹ��SPI1ʱ��

            RCC->AHB1ENR |= 1<<6; // ʹ��PORTGʱ��

            RCC->APB2RSTR |= RCC_APB2RSTR_SPI1; // ��λSPI1
            RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1; // ֹͣ��λSPI1

            GPIO_CfgPinFunc(GPIO_B,PIN4|PIN5|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,
                    GPIO_SPEED_50M,GPIO_PUPD_PU); // GPB3��4��5ΪSCK��MISO��MOSI
            GPIO_AFSet(GPIO_B,3,5);
            GPIO_AFSet(GPIO_B,4,5);
            GPIO_AFSet(GPIO_B,5,5);
            break;

        default: return false;
    }
    return true;
}
// =============================================================================
// ���ܣ����ݶ˿ںſ���Ƭѡ
// ������SPIPort�˿ں�  level==0ʹ��ʹ�ܻ�  1ʧ��
// ���أ�true
// =============================================================================
bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level)
{
    switch(SPIPort)
    {
        case CN_SPI1:
            if(level)
               GPIO_SettoHigh(GPIO_A,PIN15);
            else
              GPIO_SettoLow(GPIO_A,PIN15);
            break;
        default:  return false;
    }
    return true;
}
// =============================================================================
// ���ܣ�������ص�gpio����
// ��������
// ���أ�true
// =============================================================================

bool_t Board_EthGpioInit(void)
{
    GPIO_PowerOn(GPIO_A);
    GPIO_PowerOn(GPIO_B);
    GPIO_PowerOn(GPIO_C);

    GPIO_CfgPinFunc(GPIO_A,PIN1|PIN2|PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_NONE);
    GPIO_CfgPinFunc(GPIO_B,PIN10|PIN11|PIN12|PIN13,GPIO_MODE_AF,
                    GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);
    GPIO_CfgPinFunc(GPIO_C,PIN1|PIN4|PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_NONE);

    GPIO_AFSet(GPIO_A,1,11);
    GPIO_AFSet(GPIO_A,2,11);
    GPIO_AFSet(GPIO_A,7,11);
    GPIO_AFSet(GPIO_B,10,11);
    GPIO_AFSet(GPIO_B,11,11);
    GPIO_AFSet(GPIO_B,12,11);
    GPIO_AFSet(GPIO_B,13,11);
    GPIO_AFSet(GPIO_C,1,11);
    GPIO_AFSet(GPIO_C,4,11);
    GPIO_AFSet(GPIO_C,5,11);

    return true;
}
// =============================================================================
// ���ܣ����ݾ���İ������SPI��GPIO�����Ź��ܣ�����������أ����Ըú������ڸ��ļ���CPU
//      SPI����ֱ�ӵ��øú�������ʼ�����ڵ�GPIO����
//      ��Ҫ����GPIO�˿ں�����ʱ��ʹ�ܡ�GPIO���á���ӳ�䡢ʱ�ӵ�
// ��������
// ���أ�true
// =============================================================================
bool_t Board_UsbGpioInit(void)
{

//todo usb�ܽŴ���ȫ

    return true;
}
// =============================================================================
// ���ܣ�3G�����GPIO����
// ��������
// ���أ�true
// =============================================================================
bool_t Board_3G_GpioInit(void)
{
//    Board_SpiGpioInit(CN_SPI2);//b14 b15
    Board_UartGpioInit(CN_UART6);

    GPIO_PowerOn(GPIO_C);
    GPIO_PowerOn(GPIO_D);

    // PD15=3G_RESET��PC8=3G_WAKE��PC9=3G_EN��
    GPIO_CfgPinFunc(GPIO_D,PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_NONE);
    GPIO_CfgPinFunc(GPIO_C,PIN8|PIN9,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_NONE);

    // С�ģ���������ʱ���������
    GPIO_SettoLow(GPIO_C, PIN9); // ģ�鹩��
    GPIO_SettoLow(GPIO_D, PIN15); // ȥ��ģ�鸴λ
    GPIO_SettoLow(GPIO_C, PIN8); // ʹ��ģ��
    Djy_DelayUs(120000);
    GPIO_SettoHigh(GPIO_C, PIN8); // �ر�ģ��
    Djy_DelayUs(120000);
    GPIO_SettoLow(GPIO_C, PIN8); // ʹ��ģ��

    return true;
}

// =============================================================================
// ���ܣ�Zigbee�����GPIO����
// ��������
// ���أ�true
// =============================================================================
bool_t Board_Zigbee_GpioInit(void)
{

    Board_UartGpioInit(CN_UART3);//d8 d9

    GPIO_PowerOn(GPIO_D);
    //link sleep reset wake
    GPIO_CfgPinFunc(GPIO_D,PIN10|PIN11|PIN12|PIN13,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_NONE);

    return true;
}

// =============================================================================
// ���ܣ�Flash�����GPIO����
// ��������
// ���أ�true
// =============================================================================
bool_t Board_Flash_GpioInit(void)
{
    Board_SpiGpioInit(CN_SPI1);
    GPIO_PowerOn(GPIO_B);
    GPIO_PowerOn(GPIO_A);
    //FLASH_WP FLASH_HOLD
    GPIO_CfgPinFunc(GPIO_B,PIN6|PIN7,GPIO_MODE_OUT,GPIO_OTYPE_PP,
            GPIO_SPEED_100M,GPIO_PUPD_NONE);
    //cs
    GPIO_CfgPinFunc(GPIO_A,PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,
            GPIO_SPEED_100M,GPIO_PUPD_NONE);
    GPIO_SettoHigh(GPIO_B,PIN6|PIN7);//wp HOLD
    return true;
}

// =============================================================================
// ���ܣ�Lora�����GPIO����
// ��������
// ���أ�true
// =============================================================================
bool_t Board_Lora_GpioInit(void)
{



    return true;
}
// =============================================================================
// ���ܣ�485/232�����GPIO����  ��ȡ���ڹ���״̬ 485/232
// ��������
// ���أ�true
// =============================================================================
bool_t Board_Uart_GpioInit(void)
{
    //uart0_485_DE
    GPIO_CfgPinFunc(GPIO_A,PIN8,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);
    Board_UartGpioInit(CN_UART1);
    //uart1_485_DE
    GPIO_CfgPinFunc(GPIO_D,PIN7,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);
    Board_UartGpioInit(CN_UART2);

    return true;
}

// =============================================================================
// ���ܣ�Board_GpioInit���������õ���GPIO��ʼ��
//  �����ｫ���п����õ��ĸ���ģʽ��gpio������������Ҫ��ȡ��ע�ͼ���
// ��������
// ���أ���
// =============================================================================

void Board_GpioInit(void)
{
    Board_EthGpioInit();//����
    Board_Uart_GpioInit();//232/485
    Board_3G_GpioInit();
    Board_Zigbee_GpioInit();
    Board_Flash_GpioInit();
    Board_Lora_GpioInit();
}






