#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//#include "filesystems.h"
//    extern s32 ModuleInstall_EFS(const char *target, u32 opt, void *config);
//    ModuleInstall_EFS(CFG_EFS_MOUNT_POINT, MS_INSTALLCREAT, CFG_EFS_FILE_BLOCK_SIZE);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"easyfilesystem"     //easy文件系统
//parent:"filesystem"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:system                    //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                    //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                      //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                    //初始化时机，可选值：early，medium，later。
                                      //表示初始化时间，分别是早期、中期、后期
//dependence:"filesystem"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                      //选中该组件时，被依赖组件将强制选中，
                                      //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"               //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                      //选中该组件时，被依赖组件不会被强制选中，
                                      //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                        //该组件的互斥组件名（可以是none，表示无互斥组件），
                                      //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#ifndef CFG_OPTIONS   //****检查参数是否已经配置好
#warning    efsfilesystem组件参数未配置，使用默认值
//%$#@num,0,100,
#define CFG_EFS_FILE_BLOCK_SIZE           0x1000                 //EFS文件系统用的媒体块大小.at45块大小为0x1000；如用其它媒体这里需要修改
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#define CFG_EFS_MOUNT_POINT               "efs"      //"name",EFS文件系统安装目录
#endif//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure
