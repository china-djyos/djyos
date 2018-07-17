#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_myname.h****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//   ModuleInstall_EFS(CFG_MOUNT_POINT, CFG_MOUNT_DEV, CFG_OPTIONS, CFG_FILE_BLOCK_SIZE);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"easyfilesystem"     //填写该组件的名字
//parent:"filesystem"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:核心组件                   //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:可选                         //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                      //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//grade:init                          //初始化时机，可选值：none，init，main。none表示无须初始化，
                                      //init表示在调用main之前，main表示在main函数中初始化
//dependence:"none"                   //该组件的依赖组件名（可以是none，表示无依赖组件），
                                      //选中该组件时，被依赖组件将强制选中，
                                      //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"               //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                      //选中该组件时，被依赖组件不会被强制选中，
                                      //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                        //该组件的依赖组件名（可以是none，表示无依赖组件），
                                      //如果依赖多个组件，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#ifndef CFG_OPTIONS   //****检查参数是否已经配置好
#warning    easyfilesystem组件参数未配置，使用默认值
//%$#@num,0,100,
#define CFG_OPTIONS             0       //"安排选项",EFS文件系统文件安装逻辑
#define CFG_FILE_BLOCK_SIZE     0       //"文件最小块(block)",简易文件系统文件最小尺寸
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#define CFG_MOUNT_POINT         ""      //"安装目录",简易文件系统安装目录
#define CFG_MOUNT_DEV           ""      //"设备路径 ",简易文件系统所在设备路径
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure
