#include <stdint.h>
#include <cpu_peri.h>
#include <at32f435_437.h>
#include <at32f435_437_crm.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

/* on chip devices. */
static gpio_type *const art43x_gpio[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH };

// =============================================================================
// ����: GPIO�������ã��������ŵ�ģʽ���������͡��ٶȡ����������͵�
// ����: port����������port��ţ�����Ҫ����GPIOA,PIN5,��port=GPIO_A
//       Msk�����������룬���������GPIOA,PIN5����msk = (1<<5).
//       Mode,ģʽ����Ϊ���롢�����ģ�����롢AF���ù���
//       OutType,���������©���
//       Strength,����ǿ��
//       PUPD,����������
// ����: ��
// =============================================================================
bool_t GPIO_CfgPinFunc(u32 port, u32 Msk, u32 Mode, u32 OutType, u32 Strength, u32 PUPD)
{
    if ((port >= GPIO_NULL) || (Msk >= PIN_NULL) || (Mode >= GPIO_NULL_MODE) || 
        (OutType >= GPIO_OTYPE_NULL) || (Strength >= GPIO_DRIVE_NULL) || (PUPD >= GPIO_OTYPE_NULL)) 
    {
        return  false;
    }

    gpio_init_type gpio_init_struct;
    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_drive_strength = Strength;
    gpio_init_struct.gpio_out_type  = OutType;
    gpio_init_struct.gpio_mode = Mode;
    gpio_init_struct.gpio_pins = Msk;
    gpio_init_struct.gpio_pull = PUPD;
    gpio_init(art43x_gpio[port], &gpio_init_struct);

    return true;        
}

// =============================================================================
// ���ܣ�����IO�ĸ��ù���
// ������port����������port��ţ�����Ҫ����GPIOA,PIN5,��port=GPIO_A
//      pinnum,IO�ţ���GPIOA,PIN5,��pinnum = 5
//      af_no�����ù��ܺ�
// ���أ���
// =============================================================================
bool_t GPIO_MUX_Set(u32 port, u32 pinnum, u32 mux_no)
{
    if ((port >= GPIO_NULL) || (mux_no >= MUX_NUll) || (pinnum >= PIN_NULL))
    {
        return false;
    }

    gpio_pins_source_type gpio_pin_source;
    // //���ڵĿ���IO��ź͸���ʱ�õ�Դ��ŵĶ�Ӧ��ϵ��������������޸��˶�Ӧ��ϵ������ҲҪ��Ӧ�޸�
    // gpio_pin_source = log2(pinnum);

    switch (pinnum)
    {
        case GPIO_PINS_0:       gpio_pin_source = GPIO_PINS_SOURCE0;    break;
        case GPIO_PINS_1:       gpio_pin_source = GPIO_PINS_SOURCE1;    break;
        case GPIO_PINS_2:       gpio_pin_source = GPIO_PINS_SOURCE2;    break;
        case GPIO_PINS_3:       gpio_pin_source = GPIO_PINS_SOURCE3;    break;
        case GPIO_PINS_4:       gpio_pin_source = GPIO_PINS_SOURCE4;    break;
        case GPIO_PINS_5:       gpio_pin_source = GPIO_PINS_SOURCE5;    break;
        case GPIO_PINS_6:       gpio_pin_source = GPIO_PINS_SOURCE6;    break;
        case GPIO_PINS_7:       gpio_pin_source = GPIO_PINS_SOURCE7;    break;
        case GPIO_PINS_8:       gpio_pin_source = GPIO_PINS_SOURCE8;    break;
        case GPIO_PINS_9:       gpio_pin_source = GPIO_PINS_SOURCE9;    break;
        case GPIO_PINS_10:       gpio_pin_source = GPIO_PINS_SOURCE10;    break;
        case GPIO_PINS_11:       gpio_pin_source = GPIO_PINS_SOURCE11;    break;
        case GPIO_PINS_12:       gpio_pin_source = GPIO_PINS_SOURCE12;    break;
        case GPIO_PINS_13:       gpio_pin_source = GPIO_PINS_SOURCE13;    break;
        case GPIO_PINS_14:       gpio_pin_source = GPIO_PINS_SOURCE14;    break;
        case GPIO_PINS_15:       gpio_pin_source = GPIO_PINS_SOURCE15;    break;

        default:
            break;
    }

    gpio_pin_mux_config(art43x_gpio[port], gpio_pin_source, mux_no);
    return true;
}

// =============================================================================
//����: ��ĳgpio port�������
//����: port����������port��ţ�����Ҫ����GPIOA,��port=GPIO_A
//����: ���õ�����
// =============================================================================
u32 GPIO_GetData(u32 port)
{
    if(port >= GPIO_NULL)
    {
        return 0;
    }
    return gpio_input_data_read(art43x_gpio[port]);
}

// =============================================================================
// ����: ���������������ĳgpio port
// ����: port����������port��ţ�����Ҫ����GPIOA,��port=GPIO_A
//      data�������������
// ����: ��
// =============================================================================
void GPIO_OutData(u32 port, u32 data)
{
    if(port < GPIO_NULL)
    {
        gpio_port_wirte(art43x_gpio[port], data);
    }
}

// =============================================================================
//����: ��port��msk����1��bit��Ӧ��λ����ߵ�ƽ
//����: port����������port��ţ�����Ҫ����GPIOA,��port=GPIO_A
//      mask��port����Ҫ����ߵ�ƽ��λ���룬�������GPIOA,PIN5��msk = (1<<5)
//����: ��
// =============================================================================
void GPIO_SettoHigh(u32 port,u32 msk)
{
    if ((port < GPIO_NULL) || (msk < PIN_NULL))
    {
        gpio_bits_write(art43x_gpio[port], msk, TRUE);
    }
}

// =============================================================================
//����: ��port��msk����1��bit��Ӧ��λ����͵�ƽ
//����: port����������port��ţ�����Ҫ����GPIOA,��port=GPIO_A
//      mask��port����Ҫ����ߵ�ƽ��λ����,�������PIOA,PIN5��msk = (1<<5)
//����: ��
// =============================================================================
void GPIO_SettoLow(u32 port,u32 msk)
{
    if ((port < GPIO_NULL) || (msk < PIN_NULL))
    {
        gpio_bits_write(art43x_gpio[port], msk, FALSE);
    }
}


// =============================================================================
// ����: ��GPIOA-GPIOI��ʱ�����
// ������port,�˿ںţ���GPIO_A
// ���أ���
// =============================================================================
void GPIO_PowerOn(u32 port)
{
    if(port < GPIO_NULL)
    {
        switch (port)
        {
            case GPIO_A:
                crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
                break;
            case GPIO_B:
                crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
                break;
            case GPIO_C:
                crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
                break;
            case GPIO_D:
                crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
                break;
            case GPIO_E:
                crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);
                break;
            case GPIO_F:
                crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);
                break;
            case GPIO_G:
                crm_periph_clock_enable(CRM_GPIOG_PERIPH_CLOCK, TRUE);
                break;
            case GPIO_H:
                crm_periph_clock_enable(CRM_GPIOH_PERIPH_CLOCK, TRUE);
                break;
            default:
                break;
        }
    }

}

// =============================================================================
// ����: Ϊ�˽��ܣ����͹��ģ�ֱ�ӹر�GPIOA-GPIOI��ʱ�����
// ������port,�˿ںţ���GPIO_A
// ���أ���
// =============================================================================
void GPIO_PowerOff(u32 port)
{
    if(port < GPIO_NULL)
    {
        switch (port)
        {
            case GPIO_A:
                crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, FALSE);
                break;
            case GPIO_B:
                crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, FALSE);
                break;
            case GPIO_C:
                crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, FALSE);
                break;
            case GPIO_D:
                crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, FALSE);
                break;
            case GPIO_E:
                crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, FALSE);
                break;
            case GPIO_F:
                crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, FALSE);
                break;
            case GPIO_G:
                crm_periph_clock_enable(CRM_GPIOG_PERIPH_CLOCK, FALSE);
                break;
            case GPIO_H:
                crm_periph_clock_enable(CRM_GPIOH_PERIPH_CLOCK, FALSE);
                break;
            default:
                break;
        }
    }
}

// =============================================================================
// ����: IOд������������
// ������port,�˿ںţ���GPIO_A
// 		mask��port����Ҫ����ߵ�ƽ��λ���룬�������GPIOA,PIN5��msk = (1<<5)
// ���أ���
// =============================================================================
void GPIO_SetWP(u32 port, u32 msk)
{
    if ((port < GPIO_NULL) || (msk < PIN_NULL))
    {
        gpio_pin_wp_config(art43x_gpio[port], msk);
    }

}

// =============================================================================
// ����: �򿪻�ر�IO�ļ�������ƶ�/��������
// ������port,�˿ںţ���GPIO_A
// 		mask��port����Ҫ����ߵ�ƽ��λ���룬�������GPIOA,PIN5��msk = (1<<5)
// ���أ���
// =============================================================================
void GPIO_SetHDRV(u32 port, u32 msk, bool_t Status)
{
    if ((port < GPIO_NULL) || (msk < PIN_NULL))
    {
        gpio_pins_huge_driven_config(art43x_gpio[port], msk, Status);
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

    if (NULL == Pin)
    {
        return false;
    }

    while(num > 0)
    {
        GPIO_PowerOn(Pin->PORT);

        flag = GPIO_CfgPinFunc(Pin->PORT, Pin->Pinx, Pin->MODER,
                                    Pin->O_TYPER ,Pin->O_STRENGTH, Pin->PUPD);
        if(flag == false)
        {
            return false;
        }
        if ((GPIO_MODE_MUX == Pin->MODER) || (Pin->MUX != MUX_NUll))
        {
            GPIO_MUX_Set(Pin->PORT, Pin->Pinx, Pin->MUX);
        }

        Pin++;
        num--;
    }
    return true;
}

// =============================================================================
// ����: ָ����IO���ֵΪ0
// ������IO��PIN�ṹ
// ���أ���
// =============================================================================
void PIO_Clear(const Pin *Pin)
{
    GPIO_SettoLow(Pin->PORT,Pin->Pinx);
}

// =============================================================================
// ����: ָ����IO���ֵΪ1
// ������IO��PIN�ṹ
// ���أ���
// =============================================================================
void PIO_Set(const Pin *Pin)
{
    GPIO_SettoHigh(Pin->PORT,Pin->Pinx);
}


// =============================================================================
// ����: ��ȡָ��IO������ֵ
// ������IO��PIN�ṹ
// ���أ�-1 -- ʧ�ܣ�0��1��IO��״̬
// =============================================================================
s8 PIO_Get( const Pin *Pin)
{
    unsigned int reg;

    if (NULL == Pin)
    {
        return -1;
    }

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
    if (NULL == str)
    {
        return -1;
    }
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
s32 PIN_Init(void *str, char *data, u32 len)
{
    s32 ret =-1;
    struct PIN *p = (struct PIN *)malloc(sizeof(struct PIN));

    if ((NULL == p) || (NULL == str))
    {
        return ret;
    }

    p->PORT=GPIO_A;
    p->Pinx=PIN0;
    p->MODER=GPIO_IN_MODE;
    p->PUPD=GPIO_PUPD_NONE;
    p->MUX=MUX_NUll;
    p->O_TYPER=GPIO_OTYPE_PP;
    p->O_STRENGTH=GPIO_DRIVE_SS;

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
        p->MUX = data[3];
    if (len >= 5)
        p->O_TYPER = data[4];
    if (len >= 6)
        p->O_STRENGTH = data[5];

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

    if (NULL == str)
    {
        return -1;
    }

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
    if (NULL != str)
    {
        u32 port = transformation(str);
        u32 pinx = 1 << data[0];
        GPIO_SettoHigh(port,pinx);
    }
}

void PIN_SettoLow(void *str,char *data,u32 len)
{
    if (NULL != str)
    {
        u32 port = transformation(str);
        u32 pinx = 1 << data[0];
        GPIO_SettoLow(port,pinx);
    }
}

void PIN_PowerOn(void *str,char *data,u32 len)
{
    if (NULL != str)
    {
        u32 port = transformation(str);
        GPIO_PowerOn(port);
    }
}

void PIN_PowerOff(void *str,char *data,u32 len)
{
    if (NULL != str)
    {
        u32 port = transformation(str);
        GPIO_PowerOff(port);
    }
}

