//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后复制到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"gd32e10x"//vendor firmware
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:third               //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_GD32E10X == false )
//#warning  " gd32e10x  组件参数未配置，使用默认配置"
//%$#@target = cmdline   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_GD32E10X    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100
//%$#@enum,1,2,3,4,5,6,7,8
#define CFG_STM32_HAL_TIMER     6//配置HAL库使用的定时器
//%$#@string,1,16
//%$#@SYMBOL        ***不经配置界面，直接定义符号
#define GD32E103R_START
#define CFG_CPU_GD32E10X
#define USE_STDPERIPH_DRIVER
#define GD32E10X
//%$#@select,1
#define     _RTE_          //RTE是否使用库
//%$#@select,1
//#define GD32_DUMMY         //只能选中其中一个
#define GD32E10x
//%$#@free
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude
//./GD32E10x_usbfs_library;
//./CMSIS/GD/GD32E10x/Source/ARM/startup_gd32e10x.s;
//./CMSIS/GD/GD32E10x/Source/IAR/startup_gd32e10x.s;
//%$#@end exclude

//%$#@API include path         ****API头文件路径列表，用分号“;”隔开，以本文件所在目录为当前目录，允许用通配符，应用本库时，DIDE将自动把本列表中的头文件加到 -I 搜索路径中
//./;
//./CMSIS;
//./CMSIS/GD/GD32E10x/Include;
//./GD32E10x_standard_peripheral/Include;
//%$#@end API include path

//%$#@self include path         ****编译自身需要的头文件路径列表，用分号“;”隔开，以本文件所在目录为当前目录，允许用通配符，编译本库时，DIDE将自动把本列表中的头文件加到 -I 搜索路径中
//%$#@end self include path

//@#$%component end configure

