#include "csi_core.h"
#include "cpu_peri.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_myname.h****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern void Board_GpioInit(void);
//    Board_GpioInit();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"board"       //板件特性驱动
//parent:"none"                              //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp组件                         //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:必选                               //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                            //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                          //初始化时机，可选值：early，medium，later。
                                            //表示初始化时间，分别是早期、中期、后期
//dependence:"none"                //该组件的依赖组件名（可以是none，表示无依赖组件），
                                            //选中该组件时，被依赖组件将强制选中，
                                            //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"                     //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                            //选中该组件时，被依赖组件不会被强制选中，
                                            //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                              //该组件的依赖组件名（可以是none，表示无依赖组件），
                                            //如果依赖多个组件，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header                       //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

#define LED1        IO_CONFIG_PB8
#define LED2        IO_CONFIG_PA6

void Board_GpioInit(void)
{
    silan_io_driver_config(LED1, IO_ATTR_DS_8mA);
    silan_io_func_config(LED1, IO_FUNC_GPIO);
    silan_io_output(LED1);
    silan_io_set_low(LED1);
    
    silan_io_driver_config(LED2, IO_ATTR_DS_8mA);
    silan_io_func_config(LED2, IO_FUNC_GPIO);
    silan_io_output(LED2);
    silan_io_set_low(LED2);
}

