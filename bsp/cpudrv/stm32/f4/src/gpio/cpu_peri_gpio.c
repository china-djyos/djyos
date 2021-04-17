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

// �ļ���     ��cpu_peri_gpio.h
// ģ������: STM32F4xx��GPIO����
// ģ��汾: V1.10
// ������Ա: hm
// ����ʱ��: 08/07.2015
// =============================================================================
#include "stdint.h"
#include "cpu_peri.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip gpio"//gpio����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO == false )
//#warning  " cpu_onchip_gpio  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header              //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


#define tagGpioReg GPIO_TypeDef

static tagGpioReg volatile * const tg_GPIO_Reg[] = {GPIOA,GPIOB,GPIOC,GPIOD,
                                                GPIOE,GPIOF,GPIOG,GPIOH,GPIOI};

// =============================================================================
// ����: GPIO�������ã��������ŵ�ģʽ���������͡��ٶȡ����������͵�
// ����: port����������port��ţ�����Ҫ����GPIOA,PIN5,��port=GPIO_A
//       Msk�����������룬���������GPIOA,PIN5����msk = (1<<5).
//       Mode,ģʽ����Ϊ���롢�����ģ�����롢AF���ù���
//       OutType,���������©���
//       Speed,�ٶȣ���GPIO_SPEED_50M
//       PUPD,����������
// ����: ��
// =============================================================================
void GPIO_CfgPinFunc(u32 port,u32 Msk,u32 Mode,
                        u32 OutType,u32 Speed,u32 PUPD)
{
    if(port > GPIO_I)
        return;
    u32 pinpos=0,pos=0,curpin=0;
    for(pinpos=0;pinpos<16;pinpos++)
    {
        pos=1<<pinpos;  //һ����λ���
        curpin=Msk&pos;//��������Ƿ�Ҫ����
        if(curpin==pos) //��Ҫ����
        {
            tg_GPIO_Reg[port]->MODER&=~(3<<(pinpos*2)); //�����ԭ��������
            tg_GPIO_Reg[port]->MODER|=Mode<<(pinpos*2); //�����µ�ģʽ
            if((Mode==0X01)||(Mode==0X02))  //��������ģʽ/���ù���ģʽ
            {
                tg_GPIO_Reg[port]->OSPEEDR&=~(3<<(pinpos*2));   //���ԭ��������
                tg_GPIO_Reg[port]->OSPEEDR|=(Speed<<(pinpos*2));//�����µ��ٶ�ֵ
                tg_GPIO_Reg[port]->OTYPER&=~(1<<pinpos) ;       //���ԭ��������
                tg_GPIO_Reg[port]->OTYPER|=OutType<<pinpos;     //�����µ����ģʽ
            }
            tg_GPIO_Reg[port]->PUPDR&=~(3<<(pinpos*2)); //�����ԭ��������
            tg_GPIO_Reg[port]->PUPDR|=PUPD<<(pinpos*2); //�����µ�������
        }
    }
}

// =============================================================================
// ���ܣ��������ù��ܺ�
// ������port����������port��ţ�����Ҫ����GPIOA,PIN5,��port=GPIO_A
//      pinnum,IO�ţ���GPIOA,PIN5,��pinnum = 5
//      af_no�����ù��ܺ�
// ���أ���
// =============================================================================
u32 GPIO_AFSet(u32 port,u32 pinnum,u32 af_no)
{
    if(port > GPIO_I)
        return 0;
    tg_GPIO_Reg[port]->AFR[pinnum>>3]&=~(0X0F<<((pinnum&0X07)*4));
    tg_GPIO_Reg[port]->AFR[pinnum>>3]|=(u32)af_no<<((pinnum&0X07)*4);
    return 1;
}

// =============================================================================
//����: ��ĳgpio port�������
//����: port����������port��ţ�����Ҫ����GPIOA,��port=GPIO_A
//����: ���õ�����
// =============================================================================
u32 GPIO_GetData(u32 port)
{
    if(port >GPIO_I)
        return 0;
    return tg_GPIO_Reg[port]->IDR;
}

// =============================================================================
// ����: ���������������ĳgpio port
// ����: port����������port��ţ�����Ҫ����GPIOA,��port=GPIO_A
//      data�������������
// ����: ��
// =============================================================================
void GPIO_OutData(u32 port,u32 data)
{
    if(port >GPIO_I)
        return;
    tg_GPIO_Reg[port]->ODR = data;
}

// =============================================================================
//����: ��port��msk����1��bit��Ӧ��λ����ߵ�ƽ
//����: port����������port��ţ�����Ҫ����GPIOA,��port=GPIO_A
//      mask��port����Ҫ����ߵ�ƽ��λ���룬�������GPIOA,PIN5��msk = (1<<5)
//����: ��
// =============================================================================
void GPIO_SettoHigh(u32 port,u32 msk)
{
    if(port >GPIO_I)
        return;
    tg_GPIO_Reg[port]->BSRR = msk & 0xffff;
}

// =============================================================================
//����: ��port��msk����1��bit��Ӧ��λ����͵�ƽ
//����: port����������port��ţ�����Ҫ����GPIOA,��port=GPIO_A
//      mask��port����Ҫ����ߵ�ƽ��λ����,�������PIOA,PIN5��msk = (1<<5)
//����: ��
// =============================================================================
void GPIO_SettoLow(u32 port,u32 msk)
{
    if(port > GPIO_I)
        return;
    tg_GPIO_Reg[port]->BSRR = ((msk & 0xffff)<<16);
}

// =============================================================================
// ����: ��GPIOA-GPIOI��ʱ�����
// ������port,�˿ںţ���GPIO_A
// ���أ���
// =============================================================================
void GPIO_PowerOn(u32 port)
{
    //stm32û�йر�gpio��Դ�Ĺ��ܣ�ֻ�ܹص�������ǵ�ʱ�ӡ�
    if(port > GPIO_I)
        return ;
    RCC->AHB1ENR |= (1<<port);
}

// =============================================================================
// ����: Ϊ�˽��ܣ����͹��ģ�ֱ�ӹر�GPIOA-GPIOI��ʱ�����
// ������port,�˿ںţ���GPIO_A
// ���أ���
// =============================================================================
void GPIO_PowerOff(u32 port)
{
    //stm32û�йر�gpio��Դ�Ĺ��ܣ�ֻ�ܹص�������ǵ�ʱ�ӡ�
    if(port > GPIO_I)
        return ;
    RCC->AHB1ENR &= ~(1<<port);
}



