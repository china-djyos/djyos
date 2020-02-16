#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//以下参数在初始化module时确定，运行中不可更改
//如果定义了多stdin以及stdout跟随，则可以实现uart和Telnet均可以作为终端使用，且
//根据用户的输入命令的设备自动切换。注意，设为跟随时，stdin必须是可读写的。
#define CN_STDIO_STDOUT_FOLLOW      2       // stdout跟随stdin
#define CN_STDIO_STDERR_FOLLOW      4       // stderr跟随stdin
#define CN_STDIO_STDIN_MULTI        8       // 允许stdin多源，典型地是uart和telnet

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//#if(CFG_STDIO_STDIOFILE == true)
//    extern s32 ModuleInstall_STDIO(const char *in,const char *out, const char *err);
//    ModuleInstall_STDIO(CFG_STDIO_IN_NAME,CFG_STDIO_OUT_NAME,CFG_STDIO_ERR_NAME);
//#endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"stdio"        //填写该组件的名字
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:later               //初始化时机，可选值：early，medium，later, pre-main。
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
#if ( CFG_MODULE_ENABLE_STDIO == false )
//#warning  " stdio  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_STDIO    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
#define CFG_STDIO_STDIN_MULTI      true         //"是否支持多种输入设备",
#define CFG_STDIO_STDOUT_FOLLOW    true         //"stdout是否跟随stdin",
#define CFG_STDIO_STDERR_FOLLOW    true         //"stderr是否跟随stdin",
#define CFG_STDIO_FLOAT_PRINT      true         //"支持浮点打印"
#define CFG_STDIO_STDIOFILE        true         //"支持标准IO文件"
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#define CFG_STDIO_IN_NAME              "/dev/UART1"    //"标准输入设备名",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"    //"标准输出设备名",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"    //"标准err输出设备名",
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
