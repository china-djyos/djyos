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
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
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
//  extern int ModuleInstall_GPIO(void);
//  ModuleInstall_GPIO();
//%$#@end initcode  ****��ʼ���������
//%$#@describe      ****���������ʼ
//component name:"cpu peri gpio"//gpio����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                  //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                 //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                 //��ʼ��ʱ������ѡֵ��early��medium��later��
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
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
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

int djy_gpio_attach_irq(GPIO_INDEX pin,uint32_t mode,
                            void (*hdr)(void *args), void *args)
{
    if (pin >= GPIONUM)
        return -1;

    gpio_dev.irq_desc[pin].handler = hdr;
    gpio_dev.irq_desc[pin].param   = args;
    if (mode == PIN_IRQ_MODE_RISING)
    {
        gpio_dev.irq_desc[pin].mode = GMODE_INPUT_PULLDOWN;
    }
    else if (mode == PIN_IRQ_MODE_FALLING)
    {
        gpio_dev.irq_desc[pin].mode = GMODE_INPUT_PULLUP;
    }
    return 0;
}


int djy_gpio_irq_enable( GPIO_INDEX pin, uint32_t enabled)
{
    if (pin >= GPIONUM)
        return -1;

    if (enabled)
        gpio_int_enable(pin, gpio_dev.irq_desc[pin].mode, gpio_irq_dispatch);
    else
        gpio_int_disable(pin);
    return 0;
}

int ModuleInstall_GPIO(void)
{
    memset(&gpio_dev, 0, sizeof(gpio_dev));
    return 0;
}