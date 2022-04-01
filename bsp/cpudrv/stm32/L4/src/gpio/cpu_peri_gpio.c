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
// ģ������: STM32F7xx��GPIO����
// ģ��汾:
// ������Ա:
// ����ʱ��: 10/28.2016
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
//attribute:bsp                  //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                 //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                 //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                 //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence                   //������������������������none����ʾ�������������
                                 //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                 //����������������������г�
//weakdependence:"none"          //��������������������������none����ʾ�������������
                                 //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                 //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                 //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO == false )
//#warning  " cpu_onchip_gpio  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
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
                                                GPIOE,GPIOF,GPIOG,GPIOH,GPIOI,GPIOK};

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
bool_t GPIO_CfgPinFunc(u32 port,u32 Msk,u32 Mode,
                        u32 OutType,u32 Speed,u32 PUPD)
{
    if(port > GPIO_I)
        return  false;
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
    return true;
}

// =============================================================================
// ���ܣ��������ù��ܺ�
// ������port����������port��ţ�����Ҫ����GPIOA,PIN5,��port=GPIO_A
//      pinnum,IO�ţ���GPIOA,PIN5,��pinnum = 5
//      af_no�����ù��ܺ�
// ���أ���
// =============================================================================
bool_t GPIO_AFSet(u32 port,u32 pinnum,u32 af_no)
{
    if(port > GPIO_I||af_no>AF15)
        return false;
    u16 flag,pinnum_1;
    for(pinnum_1=0;pinnum_1<16;pinnum_1++)
    {
        flag=(pinnum>>pinnum_1)&(0x01); //һ����λ���
        if(flag!=0)
        {
            tg_GPIO_Reg[port]->AFR[pinnum_1>>3]&=~(0X0F<<((pinnum_1&0X07)*4));
            tg_GPIO_Reg[port]->AFR[pinnum_1>>3]|=(u32)af_no<<((pinnum_1&0X07)*4);
        }
    }
    return true;
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
    tg_GPIO_Reg[port]->BSRR = msk<<16;
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
// =============================================================================
// ����: ����GPIO�Ĳ������á��ڴ����������ڼ�ֻ����ʹ���ַ��ʣ�32 λ������
// ������port,�˿ںţ���GPIO_A
// ���أ���
// =============================================================================
bool_t GPIO_SetLckr(u32 port,u32 Lckk)
{
    if(port > GPIO_I)
        return false;
    if(Lckk==GPIO_Lock)
    {
        tg_GPIO_Reg[port]->LCKR |=1<<16;
        tg_GPIO_Reg[port]->LCKR &=~(1<<16);
        tg_GPIO_Reg[port]->LCKR |=1<<16;

        if(tg_GPIO_Reg[port]->LCKR & (1<<16))
            return true;
        else
            return false;
    }
    else
    {
        tg_GPIO_Reg[port]->LCKR &=~(1<<16);
        if(tg_GPIO_Reg[port]->LCKR & (1<<16))
            return false;
        else
            return true;
    }
}

// =============================================================================
// ����: ��ʼ��GPIO ����
// ������struct PIN
// ���أ�true/false
// =============================================================================
bool_t PIO_Configure(const Pin *Pin, u32 num)
{
    u8 flag;
    while(num>0)
    {
        GPIO_PowerOn(Pin->PORT);

        flag = GPIO_CfgPinFunc(Pin->PORT,Pin->Pinx,Pin->MODER,
                Pin->O_TYPER,Pin->O_SPEEDR,Pin->PUPD);
        if(flag == false)
            return false;
        if(Pin->MODER == GPIO_MODE_AF)
            GPIO_AFSet(Pin->PORT,Pin->Pinx,Pin->AF);
        Pin++;
        num--;
    }
    return true;
}

void PIO_Clear(const Pin *Pin)
{
    GPIO_SettoLow(Pin->PORT,Pin->Pinx);
}
void PIO_Set(const Pin *Pin)
{
    GPIO_SettoHigh(Pin->PORT,Pin->Pinx);
}

unsigned char PIO_Get( const Pin *Pin )
{
    unsigned int reg;
    reg = GPIO_GetData(Pin->PORT);

    if ( (reg & Pin->Pinx) == 0 )
    {
        return 0 ;
    }
    else
    {
        return 1 ;
    }
}


//���´���ΪmicroPython�ṩ֧�֡�
u32 transformation(char *str)
{
    if(strcmp(str,"GPIO_A")==0)
        return GPIO_A;
    else if(strcmp(str,"GPIO_B")==0)
        return GPIO_B;
    else if(strcmp(str,"GPIO_C")==0)
        return GPIO_C;
    else if(strcmp(str,"GPIO_D")==0)
        return GPIO_D;
    else if(strcmp(str,"GPIO_E")==0)
        return GPIO_E;
    else if(strcmp(str,"GPIO_F")==0)
        return GPIO_F;
    else if(strcmp(str,"GPIO_G")==0)
        return GPIO_G;
    else if(strcmp(str,"GPIO_H")==0)
        return GPIO_H;
    else if(strcmp(str,"GPIO_I")==0)
        return GPIO_I;
    else if(strcmp(str,"GPIO_J")==0)
        return GPIO_J;
    else if(strcmp(str,"GPIO_I")==0)
        return GPIO_I;
    else
        return -1;
}

// =============================================================================
// ����: GPIO�������ã��������ŵ�ģʽ���������͡��ٶȡ����������͵�
// ����: str����������port��ţ�����Ҫ����MGPIOA
//       date[0]�����������룬���������MPIN0
//       date[1],ģʽ����Ϊ���롢�����ģ�����롢AF���ù���,��MGPIO_MODE_IN
//       date[2] ��������������MGPIO_PUPD_NONE
//       date[3],AFģʽֵ
//       date[4],���������©���
//       date[5],�ٶȣ���GPIO_SPEED_50M
// ����: ��
// =============================================================================
s32 PIN_Init(void *str,char *data,u32 len)
{
    s32 ret =-1;
    struct PIN *p = malloc(sizeof(struct PIN));

    p->PORT=GPIO_A;
    p->Pinx=PIN0;
    p->MODER=GPIO_MODE_IN;
    p->PUPD=GPIO_PUPD_NONE;
    p->AF=AF_NUll;
    p->O_TYPER=GPIO_OTYPE_PP;
    p->O_SPEEDR=GPIO_SPEED_H;

//    for(int i=0;i<len;i++)
//    {
//        printf("data[%d] is %d\r\n",i,data[i]);
//    }

    if(len>=1)
    {
        ret = transformation(str);
        p->PORT = ret;
        p->Pinx = 1 << data[0];
    }
    if (len >= 2)
        p->MODER = data[1];
    if (len >= 3)
        p->PUPD = data[2];
    if (len >= 4)
        p->AF = data[3];
    if (len >= 5)
        p->O_TYPER = data[4];
    if (len >= 6)
        p->O_SPEEDR = data[5];

//    printf("ret is %d\r\n",ret);
//    printf("p->PORT is %d\r\n",p->PORT);
//    printf("p->Pinx is %d\r\n",p->Pinx);
//    printf("p->MODER is %d\r\n",p->MODER);
//    printf("p->PUPD is %d\r\n",p->PUPD);
//    printf("p->AF is %d\r\n",p->AF);
//    printf("p->O_TYPER is %d\r\n",p->O_TYPER);
//    printf("p->O_SPEEDR is %d\r\n",p->O_SPEEDR);

    PIO_Configure(p, 1);

    free(p);

    return ret;
}

u32 PIN_Get(void *str,char *data,u32 len)
{
    unsigned int reg;

    u32 port = transformation(str);

    reg= GPIO_GetData(port);

    if ( (reg & (1<<data[0])) == 0 )
    {
        return 0 ;
    }
    else
    {
        return 1 ;
    }

}

void PIN_SettoHigh(void *str,char *data,u32 len)
{
    u32 port = transformation(str);
    u32 pinx = 1 << data[0];
    GPIO_SettoHigh(port,pinx);
}

void PIN_SettoLow(void *str,char *data,u32 len)
{
    u32 port = transformation(str);
    u32 pinx = 1 << data[0];
    GPIO_SettoLow(port,pinx);
}

void PIN_PowerOn(void *str,char *data,u32 len)
{
    u32 port = transformation(str);
    GPIO_PowerOn(port);
}

void PIN_PowerOff(void *str,char *data,u32 len)
{
    u32 port = transformation(str);
    GPIO_PowerOff(port);
}





