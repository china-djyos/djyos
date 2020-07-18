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
#include "uartctrl.h"
#include "pcf8574.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"board config"//�����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                       //ѡ�third��system��bsp��user����������������IDE�з���
//select:required                     //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                      //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                     //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                      //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"kernel","stm32f4","cpu onchip gpio"//������������������������none����ʾ�������������
                                      //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                      //����������������������г����á�,���ָ�
//weakdependence:"none"               //��������������������������none����ʾ�������������
                                      //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                      //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                      //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,flase,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
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
    GPIO_PowerOn(GPIO_C);
    GPIO_PowerOn(GPIO_D);
    GPIO_PowerOn(GPIO_G);
    RCC->APB2ENR|=1<<14;    //ʹ��SYSCFGʱ��
    SYSCFG->PMC|=1<<23;     //ʹ��RMII�ӿ�




    GPIO_CfgPinFunc(GPIO_A,PIN1|PIN2|PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_NONE);
    GPIO_CfgPinFunc(GPIO_D,PIN3,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_NONE);
    GPIO_CfgPinFunc(GPIO_C,PIN1|PIN4|PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_PU);
    GPIO_CfgPinFunc(GPIO_G,PIN11|PIN13|PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_PU);

    GPIO_AFSet(GPIO_A,1,11);    //PA1,AF11
    GPIO_AFSet(GPIO_A,2,11);    //PA2,AF11
    GPIO_AFSet(GPIO_A,7,11);    //PA7,AF11

    GPIO_AFSet(GPIO_C,1,11);    //PC1,AF11
    GPIO_AFSet(GPIO_C,4,11);    //PC4,AF11
    GPIO_AFSet(GPIO_C,5,11);    //PC5,AF11

    GPIO_AFSet(GPIO_G,11,11);   //PG11,AF11
    GPIO_AFSet(GPIO_G,13,11);   //PG13,AF11
    GPIO_AFSet(GPIO_G,14,11);   //PG14,AF11

    GPIO_SettoLow(GPIO_D,PIN3);
    DJY_EventDelay(10*1000);
    GPIO_SettoHigh(GPIO_D,PIN3);
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
    if(SPIx == CN_SPI1)
    {
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
#if 0 // CS���豸ȷ��
        GPIO_CfgPinFunc(GPIO_B,PIN14,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU); // GPB14ΪCS����NOR

        GPIO_CfgPinFunc(GPIOG,PIN7,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                GPIO_SPEED_100M,GPIO_PUPD_PU); // GPG7ΪCS��������ģʽ
        GPIOG->ODR|=1<<7;

        GPIO_SettoHigh(GPIO_B,PIN14);// CSΪ��
#endif
    }
    else
    {
        return false;
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

// ============================================================================
// ���ܣ���ȡ�˿ڵ�����RS_485/RS_232 RS485 ����false
// ������SerialNo,���ں�
// ���أ�true/false
// ============================================================================
u8   Board_CheckPortIsRS232(u8 SerialNo)
{

    if(SerialNo==CN_UART1||SerialNo==CN_UART2)
        return false;
    return true;
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
// ���ܣ����ݶ˿ںſ���Ƭѡ
// ������SPIPort�˿ں�  level==0ʹ��ʹ�ܻ�  1ʧ��
// ���أ�true
// =============================================================================
#define BITBAND(addr, bitnum)       ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)              *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)      MEM_ADDR(BITBAND(addr, bitnum))
#define GPIOB_ODR_Addr                (GPIOB_BASE+20) //0x40020414
#define PAout(n)                       BIT_ADDR(GPIOA_ODR_Addr,n)  //���
#define PBout(n)                       BIT_ADDR(GPIOB_ODR_Addr,n)  //���

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level)
{
    switch(SPIPort)
    {
        case CN_SPI1:
            if(level)
                PBout(14) = 1;
            else
                PBout(14) = 0;
            break;
        default:  return false;
    }
    return true;
}
#pragma GCC diagnostic pop

static void Board_NORGpioInit (void )
{

    Board_SpiGpioInit(CN_SPI1);
    GPIO_CfgPinFunc(GPIO_B, PIN14, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M,
                    GPIO_PUPD_PU); // GPB14ΪCS����NOR

    RCC->AHB1ENR |= 1<<6; // ʹ��PORTGʱ��
    GPIO_CfgPinFunc(GPIOG,PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M,
                    GPIO_PUPD_PU); // GPG7ΪCS��������ģʽ
    GPIOG->ODR |= 1<<7;

    GPIO_SettoHigh(GPIO_B,PIN14);// CSΪ��
}
// =============================================================================
// ���ܣ�Board_GpioInit���������õ���GPIO��ʼ��
//  �����ｫ���п����õ��ĸ���ģʽ��gpio������������Ҫ��ȡ��ע�ͼ���
// ��������
// ���أ���
// =============================================================================

void Board_Init(void)
{
    Board_EthGpioInit();//����
    Board_UartGpioInit(CN_UART1);
//    Board_UartGpioInit(CN_UART2);
    Board_NORGpioInit();
}

////���ڵ� PHY оƬ��LAN8720��λ
bool_t LAN8720_RESET(void)
{
//  PCF8574_WriteBit(ETH_RESET_IO,1);
//  DJY_DelayUs(100*mS);
//  PCF8574_WriteBit(ETH_RESET_IO,0);
//  DJY_DelayUs(100*mS);
    //PD3�����һ��100mS�ĸߵ�ƽ����
    return true;
}

