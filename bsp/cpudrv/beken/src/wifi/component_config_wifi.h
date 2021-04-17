#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中

//extern bool_t ModuleInstall_Wifi(void);
//  ModuleInstall_Wifi( );

//%$#@end initcode  ****初始化代码结束
//%$#@describe      ****组件描述开始
//component name:"cpu onchip MAC"//CPU的mac驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"tcpip","lock","int"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_MAC == false )
//#warning  " cpu_onchip_MAC  组件参数未配置，使用默认配置"
//%$#@target = header    //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_MAC    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,1000,10000,
//%$#@enum,true,false,
//%$#@string,1,32,
#define CFG_WIFI_DEV_NAME        "BK7251_WiFi"//"网卡名称",
//%$#@string,1,100,
#define CFG_FAST_DATA_FILE_NAME "/efs/fast_data.dat"    //"存快联信息的文件路径"
#define CFG_MAC_DATA_FILE_NAME "/efs/mac.dat"    //"存MAC地址的文件路径"
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure
// =============================================================================
