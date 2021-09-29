#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//#if !defined (CFG_RUNMODE_BAREAPP)
//    extern bool_t ModuleInstall_UpdateIboot(void);
//    ModuleInstall_UpdateIboot( );
//#endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"loader"       //加载器
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:required               //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:                   //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_LOADER == false )
//#warning  " loader  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_LOADER    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
#define CFG_UPDATEIBOOT_EN      false       //"是否支持在线更新Iboot"，
#define CFG_START_APP_IS_VERIFICATION      true       //"启动app时是否执行校验功能"，
//%$#@enum,CN_RUN_FORM_FILE,CN_DIRECT_RUN,
#define  CFG_APP_RUNMODE  CN_DIRECT_RUN     //"APP运行模式",CN_DIRECT_RUN=直接从flash中运行；CN_FORM_FILE=从文件系统加载到内存运行，
//%$#@enum,VERIFICATION_NULL,VERIFICATION_CRC,VERIFICATION_MD5,VERIFICATION_SSL,
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL   //"APP校验方法",
//%$#@num,0,100,
#define CFG_IBOOT_VERSION_SMALL       00        //"Iboot版本号:低",xx.xx.__，APP忽略
#define CFG_IBOOT_VERSION_MEDIUM      00        //"Iboot版本号:中",xx.__.xx，APP忽略
#define CFG_IBOOT_VERSION_LARGE       01        //"Iboot版本号:高",__.xx.xx，APP忽略
//%$#@string,1,40,
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"           //"待升级iboot默认存储路径"
#define CFG_APP_UPDATE_NAME        "/yaf2/app.bin"            //"待升级app默认存储路径"
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,

#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

