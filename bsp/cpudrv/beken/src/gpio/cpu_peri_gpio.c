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
//select:required               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                 //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                 //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                 //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"              //������������������������none����ʾ�������������
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

typedef void (*gpio_isr_handler_t)(void *param);

struct gpio_irq_desc
{
    gpio_isr_handler_t handler;
    void              *param;
    uint32_t           mode;
};

struct _gpio_dev
{
    struct gpio_irq_desc irq_desc[GPIONUM];
};

static struct _gpio_dev gpio_dev;

static void gpio_irq_dispatch(unsigned char index)
{
    struct _gpio_dev *_dev = &gpio_dev;

    if (index >= GPIONUM)
    {
        return;
    }
    if (_dev->irq_desc[index].handler != NULL)
    {
        _dev->irq_desc[index].handler(_dev->irq_desc[index].param);
    }
}

void djy_gpio_mode( GPIO_INDEX pin, uint32_t mode)
{
    switch (mode)
    {
    case PIN_MODE_INPUT:
        bk_gpio_config_input(pin);
        break;

    case PIN_MODE_INPUT_PULLUP:
        bk_gpio_config_input_pup(pin);
        break;

    case PIN_MODE_INPUT_PULLDOWN:
        bk_gpio_config_input_pdwn(pin);
        break;

    case PIN_MODE_OUTPUT:
        bk_gpio_config_output(pin);
        break;
    }
}

void djy_gpio_write(GPIO_INDEX pin, uint32_t value)
{
    bk_gpio_output(pin, value);
}

uint32_t djy_gpio_read( GPIO_INDEX pin)
{
    return bk_gpio_input(pin);
}

s32 djy_gpio_attach_irq(GPIO_INDEX pin,uint32_t mode,
                            void (*hdr)(void *args), void *args)
{
    if (pin >= GPIONUM)
        return -1;

    gpio_dev.irq_desc[pin].handler = hdr;
    gpio_dev.irq_desc[pin].param   = args;
    if (mode == PIN_IRQ_MODE_RISING)
    {
        gpio_dev.irq_desc[pin].mode = GPIO_INT_LEVEL_RISING;
    }
    else if (mode == PIN_IRQ_MODE_FALLING)
    {
        gpio_dev.irq_desc[pin].mode = GPIO_INT_LEVEL_FALLING;
    }
    return 0;
}


s32 djy_gpio_irq_enable( GPIO_INDEX pin, uint32_t enabled)
{
    if (pin >= GPIONUM)
        return -1;

    if (enabled)
        gpio_int_enable(pin, gpio_dev.irq_desc[pin].mode, gpio_irq_dispatch);
    else
        gpio_int_disable(pin);
    return 0;
}

s32 Djy_GpioInit(void)
{
    memset(&gpio_dev, 0, sizeof(gpio_dev));
    return 0;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

s32 PIN_Init(void *str,char *data,u32 len)
{
    s32 ret = 0;
    u32 pinx=0;
    u32 mode=PIN_MODE_OUTPUT;
    u32 pupd=PIN_MODE_INPUT;

    if(len>=1)
        pinx = data[0];
    if(len>=2)
        mode = data[1];
    if(len>=3)
        pupd = data[2];

    if(mode == PIN_MODE_INPUT)
    {
        mode = pupd;
    }

    djy_gpio_mode(pinx,mode);

    if(mode == PIN_MODE_OUTPUT && pupd == PIN_MODE_INPUT_PULLUP)
    {
        djy_gpio_write(pinx,1);
    }
    else if(mode == PIN_MODE_OUTPUT && pupd == PIN_MODE_INPUT_PULLDOWN)
    {
        djy_gpio_write(pinx,0);
    }

//    printf("str is %s\r\n",str);
//    printf("len is %d\r\n",len);
//    printf("pinx is %d\r\n",pinx);
//    printf("mode is %d\r\n",mode);

    return ret;
}

u32 PIN_Get(void *str,char *data,u32 len)
{
   return djy_gpio_read(data[0]);
}

void PIN_SettoHigh(void *str,char *data,u32 len)
{
    djy_gpio_write(data[0],1);
}

void PIN_SettoLow(void *str,char *data,u32 len)
{
    djy_gpio_write(data[0],0);
}

void PIN_PowerOn(void *str,char *data,u32 len)
{
    printf("PIN_PowerOn\r\n");
}

void PIN_PowerOff(void *str,char *data,u32 len)
{
    printf("PIN_PowerOff\r\n");
}
#pragma GCC diagnostic pop
