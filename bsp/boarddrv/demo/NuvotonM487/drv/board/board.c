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
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_myname.h****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"board config"//�����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                             //ѡ�third��system��bsp��user����������������IDE�з���
//select:required                           //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                            //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                           //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                            //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"kernel","Nuvoton M480","cpu onchip gpio"//������������������������none����ʾ�������������
                                            //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                            //����������������������г����á�,���ָ�
//weakdependence:"none"                     //��������������������������none����ʾ�������������
                                            //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                            //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                            //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header                       //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
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
    //Ports  :  PB12 PB13  PA2 PA3
    //Signal :  RXD0 TXD0  RXD1   TXD1
    switch(SerialNo)
    {
    case CN_UART0:
        CLK->APBCLK0 |= CLK_APBCLK0_UART0CKEN_Msk; // UART0 Clock Enable
        /* Select UART clock source is HXT */
        CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_UART0SEL_Msk) | (0x0 << CLK_CLKSEL1_UART0SEL_Pos);

        /* Set GPB multi-function pins for UART0 RXD and TXD */
        SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk);
        SYS->GPB_MFPH |= (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);
        break;
    case CN_UART1:
        CLK->APBCLK0 |= CLK_APBCLK0_UART1CKEN_Msk; // UART1 Clock Enable
        /* Select UART clock source is HXT */
        CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_UART1SEL_Msk) | (0x0 << CLK_CLKSEL1_UART1SEL_Pos);

        /* Set PA multi-function pins for UART1 TXD, RXD, CTS and RTS */
        SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA0MFP_Msk | SYS_GPA_MFPL_PA1MFP_Msk |
                           SYS_GPA_MFPL_PA2MFP_Msk | SYS_GPA_MFPL_PA3MFP_Msk);
        SYS->GPA_MFPL |= (0x8 << SYS_GPA_MFPL_PA0MFP_Pos) | (0x8 << SYS_GPA_MFPL_PA1MFP_Pos) |
                         (0x8 << SYS_GPA_MFPL_PA2MFP_Pos) | (0x8 << SYS_GPA_MFPL_PA3MFP_Pos);
        break;
    case CN_UART2:
        CLK->APBCLK0 |= CLK_APBCLK0_UART2CKEN_Msk; // UART2 Clock Enable
        /* Select UART clock source is HXT */
        CLK->CLKSEL3 = (CLK->CLKSEL3 & ~CLK_CLKSEL3_UART2SEL_Msk) | (0x0 << CLK_CLKSEL3_UART2SEL_Pos);

        break;
    case CN_UART3:
        CLK->APBCLK0 |= CLK_APBCLK0_UART3CKEN_Msk; // UART3 Clock Enable
        /* Select UART clock source is HXT */
        CLK->CLKSEL3 = (CLK->CLKSEL3 & ~CLK_CLKSEL3_UART3SEL_Msk) | (0x0 << CLK_CLKSEL3_UART3SEL_Pos);

        break;
    case CN_UART4:
        CLK->APBCLK0 |= CLK_APBCLK0_UART4CKEN_Msk; // UART4 Clock Enable
        /* Select UART clock source is HXT */
        CLK->CLKSEL3 = (CLK->CLKSEL3 & ~CLK_CLKSEL3_UART4SEL_Msk) | (0x0 << CLK_CLKSEL3_UART4SEL_Pos);
        break;
    case CN_UART5:
        CLK->APBCLK0 |= CLK_APBCLK0_UART5CKEN_Msk; // UART5 Clock Enable
        /* Select UART clock source is HXT */
        CLK->CLKSEL3 = (CLK->CLKSEL3 & ~CLK_CLKSEL3_UART5SEL_Msk) | (0x0 << CLK_CLKSEL3_UART5SEL_Pos);

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
#if 0
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
#endif
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
    if(SPIx == CN_SPI0)
    {
        /* Enable SPI0 peripheral clock */
        CLK->APBCLK0 |= CLK_APBCLK0_SPI0CKEN_Msk;
        /* Select PCLK0 as the clock source of SPI0 */
        CLK->CLKSEL2 |= CLK_CLKSEL2_SPI0SEL_PCLK1;

        /* Setup SPI0 multi-function pins */
        SYS->GPA_MFPL |= SYS_GPA_MFPL_PA0MFP_SPI0_MOSI | SYS_GPA_MFPL_PA1MFP_SPI0_MISO | SYS_GPA_MFPL_PA2MFP_SPI0_CLK | SYS_GPA_MFPL_PA3MFP_SPI0_SS;

        /* Enable SPI0 clock pin (PA2) schmitt trigger */
        PA->SMTEN |= GPIO_SMTEN_SMTEN2_Msk;

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
        case CN_UART0:break;
        case CN_UART1:break;
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
    if(SerialNo==CN_UART0||SerialNo==CN_UART1)
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
    case CN_UART0: break;
    case CN_UART1: break;
    default:       break;
    }
}

// =============================================================================
// ���ܣ����ݶ˿ںſ���Ƭѡ
// ������SPIPort�˿ں�  level==0ʹ��ʹ�ܻ�  1ʧ��
// ���أ�true
// =============================================================================
//#define BITBAND(addr, bitnum)       ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
//#define MEM_ADDR(addr)              *((volatile unsigned long  *)(addr))
//#define BIT_ADDR(addr, bitnum)      MEM_ADDR(BITBAND(addr, bitnum))
//#define GPIOB_ODR_Addr                (GPIOB_BASE+20) //0x40020414
//#define PAout(n)                       BIT_ADDR(GPIOA_ODR_Addr,n)  //���
//#define PBout(n)                       BIT_ADDR(GPIOB_ODR_Addr,n)  //���

bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level)
{
    /*
    switch(SPIPort)
    {
        case CN_SPI0:
           // if(level)
           //    PBout(14) = 1;
           // else
           //     PBout(14) = 0;
            break;
        default:  return false;
    }
    return true;
    */
}
static void Board_NORGpioInit (void )
{

    //Board_SpiGpioInit(CN_SPI0);
    //GPIO_CfgPinFunc(GPIO_B, PIN14, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M,
    //                GPIO_PUPD_PU); // GPB14ΪCS����NOR

    //RCC->AHB1ENR |= 1<<6; // ʹ��PORTGʱ��
    //GPIO_CfgPinFunc(GPIOG,PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M,
    //                GPIO_PUPD_PU); // GPG7ΪCS��������ģʽ
    //GPIOG->ODR |= 1<<7;

    //GPIO_SettoHigh(GPIO_B,PIN14);// CSΪ��
}
// =============================================================================
// ���ܣ�Board_GpioInit���������õ���GPIO��ʼ��
//  �����ｫ���п����õ��ĸ���ģʽ��gpio������������Ҫ��ȡ��ע�ͼ���
// ��������
// ���أ���
// =============================================================================

void Board_Init(void)
{
    //Board_EthGpioInit();//����
    Board_UartGpioInit(CN_UART0);
    //Board_UartGpioInit(CN_UART2);
    //Board_NORGpioInit();
}











