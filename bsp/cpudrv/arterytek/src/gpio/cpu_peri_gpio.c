#include <stdint.h>
#include <cpu_peri.h>
#include <at32f435_437.h>
#include <at32f435_437_crm.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip gpio"//gpio操作函数集
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO == false )
//#warning  " cpu_onchip_gpio  组件参数未配置，使用默认配置"
//%$#@target = header              //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

/* artery stuff. */

/* on chip devices. */
static gpio_type *const art43x_gpio[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH };

// =============================================================================
// 功能: GPIO引脚配置，包括引脚的模式、输入类型、速度、上下拉类型等
// 参数: port，被操作的port编号，比如要操作GPIOA,PIN5,则port=GPIO_A
//       Msk，操作的掩码，如操作的是GPIOA,PIN5，则msk = (1<<5).
//       Mode,模式，分为输入、输出、模拟输入、AF复用功能
//       OutType,推挽输出或开漏输出
//       Strength,驱动强度
//       PUPD,上拉或下拉
// 返回: 无
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
// 功能：设置IO的复用功能
// 参数：port，被操作的port编号，比如要操作GPIOA,PIN5,则port=GPIO_A
//      pinnum,IO号，如GPIOA,PIN5,则pinnum = 5
//      af_no，复用功能号
// 返回：无
// =============================================================================
bool_t GPIO_MUX_Set(u32 port, u32 pinnum, u32 mux_no)
{
    if ((port >= GPIO_NULL) || (mux_no >= MUX_NUll) || (pinnum >= PIN_NULL))
    {
        return false;
    }

    gpio_pins_source_type gpio_pin_source;
    // //现在的可以IO序号和复用时用的源序号的对应关系是这样，如果库修改了对应关系，这里也要对应修改
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
//功能: 把某gpio port整体读入
//参数: port，被操作的port编号，比如要操作GPIOA,则port=GPIO_A
//返回: 读得的数据
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
// 功能: 把数据整体输出到某gpio port
// 参数: port，被操作的port编号，比如要操作GPIOA,则port=GPIO_A
//      data，待输出的数据
// 返回: 无
// =============================================================================
void GPIO_OutData(u32 port, u32 data)
{
    if(port < GPIO_NULL)
    {
        gpio_port_wirte(art43x_gpio[port], data);
    }
}

// =============================================================================
//功能: 在port中msk中是1的bit对应的位输出高电平
//参数: port，被操作的port编号，比如要操作GPIOA,则port=GPIO_A
//      mask，port中需要输出高电平的位掩码，比如操作GPIOA,PIN5，msk = (1<<5)
//返回: 无
// =============================================================================
void GPIO_SettoHigh(u32 port,u32 msk)
{
    if ((port < GPIO_NULL) || (msk < PIN_NULL))
    {
        gpio_bits_write(art43x_gpio[port], msk, TRUE);
    }
}

// =============================================================================
//功能: 在port中msk中是1的bit对应的位输出低电平
//参数: port，被操作的port编号，比如要操作GPIOA,则port=GPIO_A
//      mask，port中需要输出高电平的位掩码,比如操作PIOA,PIN5，msk = (1<<5)
//返回: 无
// =============================================================================
void GPIO_SettoLow(u32 port,u32 msk)
{
    if ((port < GPIO_NULL) || (msk < PIN_NULL))
    {
        gpio_bits_write(art43x_gpio[port], msk, FALSE);
    }
}


// =============================================================================
// 功能: 打开GPIOA-GPIOI的时钟输出
// 参数：port,端口号，如GPIO_A
// 返回：无
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
// 功能: 为了节能，降低功耗，直接关闭GPIOA-GPIOI的时钟输出
// 参数：port,端口号，如GPIO_A
// 返回：无
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
// 功能: IO写保护功能设置
// 参数：port,端口号，如GPIO_A
// 		mask，port中需要输出高电平的位掩码，比如操作GPIOA,PIN5，msk = (1<<5)
// 返回：无
// =============================================================================
void GPIO_SetWP(u32 port, u32 msk)
{
    if ((port < GPIO_NULL) || (msk < PIN_NULL))
    {
        gpio_pin_wp_config(art43x_gpio[port], msk);
    }

}

// =============================================================================
// 功能: 打开或关闭IO的极大电流推动/吸入能力
// 参数：port,端口号，如GPIO_A
// 		mask，port中需要输出高电平的位掩码，比如操作GPIOA,PIN5，msk = (1<<5)
// 返回：无
// =============================================================================
void GPIO_SetHDRV(u32 port, u32 msk, bool_t Status)
{
    if ((port < GPIO_NULL) || (msk < PIN_NULL))
    {
        gpio_pins_huge_driven_config(art43x_gpio[port], msk, Status);
    }

}

// =============================================================================
// 功能: 初始化GPIO 引脚
// 参数：struct PIN
// 返回：true/false
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
// 功能: 指定的IO输出值为0
// 参数：IO的PIN结构
// 返回：无
// =============================================================================
void PIO_Clear(const Pin *Pin)
{
    GPIO_SettoLow(Pin->PORT,Pin->Pinx);
}

// =============================================================================
// 功能: 指定的IO输出值为1
// 参数：IO的PIN结构
// 返回：无
// =============================================================================
void PIO_Set(const Pin *Pin)
{
    GPIO_SettoHigh(Pin->PORT,Pin->Pinx);
}


// =============================================================================
// 功能: 获取指定IO的输入值
// 参数：IO的PIN结构
// 返回：-1 -- 失败，0或1是IO的状态
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


//以下代码为microPython提供支持。
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
// 功能: GPIO引脚配置，包括引脚的模式、输入类型、速度、上下拉类型等
// 参数: str，被操作的port编号，比如要操作MGPIOA
//       date[0]，操作的掩码，如操作的是MPIN0
//       date[1],模式，分为输入、输出、模拟输入、AF复用功能,如MGPIO_MODE_IN
//       date[2] 上拉或下拉，如MGPIO_PUPD_NONE
//       date[3],AF模式值
//       date[4],推挽输出或开漏输出
//       date[5],速度，如GPIO_SPEED_50M
// 返回: 无
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

