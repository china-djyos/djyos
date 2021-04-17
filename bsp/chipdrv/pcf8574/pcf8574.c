#include "cpu_peri.h"
#include <djyos.h>
#include <Legacy/stm32_hal_legacy.h>
#include <stddef.h>
#include <stdint.h>
//#include <stm32f7xx_hal_conf.h>
#include <systime.h>
#include "iicbus.h"
#include "board.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern bool_t ModuleInstall_PCF8574(void);
//  ModuleInstall_PCF8574();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"IIC to 8gpio pcf8574"//iic接口的8位IO扩展
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"io analog iic bus"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_IIC_TO_8GPIO_PCF8574 == false )
//#warning  " IIC_to_8gpio_pcf8574  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_IIC_TO_8GPIO_PCF8574    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_PCF8574_BUS_NAME       "IoIic"        //"IIC总线名"，
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


#define PCF8574_ADDR    0X40    //PCF8574地址(左移了一位)

static struct IIC_Device *ps_PCF8574_Dev = NULL;

__attribute__((weak))  void PCF8574_Pin_Init(void)
{
    return;
}

//向PCF8574写入8位IO值
//DataToWrite:要写入的数据
void PCF8574_WriteOneByte(u8 DataToWrite)
{
    IIC_Write(ps_PCF8574_Dev,PCF8574_ADDR|0X00,&DataToWrite,1,true,CN_TIMEOUT_FOREVER);
}

//读取PCF8574的8位IO值
//返回值:读到的数据
u8 PCF8574_ReadOneByte(void)
{
    u8 temp=0;

    IIC_Read(ps_PCF8574_Dev,PCF8574_ADDR|0X01,&temp,1,CN_TIMEOUT_FOREVER);

    return temp;
}

//设置PCF8574某个IO的高低电平
//bit:要设置的IO编号,0~7
//sta:IO的状态;0或1
void PCF8574_WriteBit(u8 bit,u8 sta)
{
    u8 data;
    data=PCF8574_ReadOneByte(); //先读出原来的设置
    if(sta==0)
        data&=~(1<<bit);
    else
        data|=1<<bit;
    PCF8574_WriteOneByte(data); //写入新的数据
}

//读取PCF8574的某个IO的值
//bit：要读取的IO编号,0~7
//返回值:此IO的值,0或1
u8 PCF8574_ReadBit(u8 bit)
{
    u8 data;
    data=PCF8574_ReadOneByte(); //先读取这个8位IO的值
    if(data&(1<<bit))
        return 1;
    else
        return 0;
}


bool_t ModuleInstall_PCF8574()
{
    //添加FT5X26到IIC总线
    ps_PCF8574_Dev = IIC_DevAdd(CFG_PCF8574_BUS_NAME,"IIC_Dev_PCF8574",PCF8574_ADDR>>1,0,8);
    if(NULL != ps_PCF8574_Dev)
    {
        IIC_BusCtrl(ps_PCF8574_Dev,CN_IIC_SET_CLK,100*1000,0);

        PCF8574_Pin_Init();

        return true;
    }

    return false;
}

