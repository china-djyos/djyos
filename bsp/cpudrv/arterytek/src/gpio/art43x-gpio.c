#include <stdint.h>
#include <cpu_peri.h>

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

/* artery stuff. */
#include <at32f435_437.h>

/* on chip devices. */
const gpio_type* art43x_gpio[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH };


static inline usart_type *to_gpio_type(u32 port)
{
        return art43x_gpio[port];
}

/* djyos driver api. */
void GPIO_PowerOn(u32 port)
{
}

void GPIO_PowerOff(u32 port)
{
        gpio_reset(to_gpio_type(port));
}

u32 GPIO_AFSet(u32 port, u32 pinnum, u32 af_no)
{
        gpio_type *gpio_x;
        gpio_pins_source_type gpio_pin_source;
        gpio_mux_sel_type gpio_mux;

        if (port >= sizeof(art43x_gpio) / sizeof(*art43x_gpio))
                return -1;

        if (pinnum > GPIO_PINS_15)
                return -1;
        if (af_no > GPIO_MUX_15)
                return -1;

        gpio_x = art43x_gpio[port];
        gpio_pin_source = pinnum;
        gpio_mux = af_no;
        gpio_pin_mux_config(gpio_x, gpio_pin_source, gpio_mux);
        return 0;
}

void GPIO_CfgPinFunc(u32 port, u32 Msk, u32 Mode, u32 OutType, u32 Speed, u32 PUPD)
{
}

u32 GPIO_GetData(u32 port)
{
        return gpio_input_data_read(to_gpio_type(port));
}

void GPIO_OutData(u32 port, u32 data)
{
        gpio_port_wirte(to_gpio_type(port), data);
}

void GPIO_SettoHigh(u32 port,u32 msk)
{
}

void GPIO_SettoLow(u32 port,u32 msk)
{
}
