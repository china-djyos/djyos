#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include <dbug.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中

//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip ct1711"       //adc组件
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"cpu onchip gpio"  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_CT1711 == false )
//#warning  "   ADC组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_CT1711    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,flase,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure

//IO方向设置
#define CT1711_OUT() gpio_config(GPIO36, GMODE_OUTPUT);
#define CT1711_IN()  gpio_config(GPIO36, GMODE_INPUT)

//IO操作函数
#define CT1711_DATA(n) gpio_output(GPIO36,n)
#define CT1711_READ    gpio_input(GPIO36)


void CT1711_Init(void)
{
    CT1711_OUT();

    CT1711_DATA(0);
    DJY_DelayUs(400);
    CT1711_DATA(1);
}

u8 CT1711_Read_Bit(void)
{
    u8 data;
    atom_low_t atom_low;

    atom_low = Int_LowAtomStart();
//    CT1711_DATA(1);
    CT1711_OUT();
//    CT1711_DATA(0);
    DJY_DelayUs(1);
    CT1711_IN();
//    CT1711_DATA(1);
    DJY_DelayUs(20);

//    CT1711_IN();
    if(CT1711_READ)
    {
        data = 0x01;
    }else{
        data = 0x00;
    }
//    CT1711_IN();

//    CT1711_OUT();
//    CT1711_DATA(1);
    DJY_DelayUs(30);
    Int_LowAtomEnd(atom_low);
    return data;
}


u8 CT1711_Read_Byte(void)
{
    u8 byte = 0;
    int i;
    for(i=8;i > 0;i--)
    {
        byte <<=1;
        byte |= CT1711_Read_Bit();
    }

    return byte;
}

float CT1711_Read_Temp_Degree(void)
{
    float temp = 0.00;
    unsigned char  cc0,cc1,sign;
    char temp_byte0,temp_byte1;
    int temp_val;

    CT1711_Init();

//    CT1711_IN();
    DJY_DelayUs(150*1000);
//    DJY_EventDelay(130*1000);

    cc0 = CT1711_Read_Bit();
    DJY_DelayUs(10);
    cc1 = CT1711_Read_Bit();
//    printf("cc0 = %d,cc1 = %d\r\n",cc0,cc1);
    DJY_DelayUs(10);
    cc0 = cc0 & 0x01;
    cc1 = cc1 & 0x01;
    if((cc0 == 0x00) && (cc1 == 0x00))
    {
        sign = CT1711_Read_Bit();
//        printf("sign = %d\r\n",sign);
        DJY_DelayUs(10);
        temp_byte0 = CT1711_Read_Byte();
        DJY_DelayUs(10);
        temp_byte1 = CT1711_Read_Byte();
        DJY_DelayUs(10);
        temp_val = (temp_byte0 << 8) + temp_byte1;
        if(sign == 0x01)
        {
            temp_val =~ temp_val;
            temp_val &= 0xffff;
            temp_val ++;
            temp = (-3.90625*temp_val/1000);
        }else{
            temp =  ((3.90625*(float)temp_val)/1000);
        }
    }

    return temp;
}
