//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//****新建组件，可以copy component_config_template.txt的内容到这里。
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern s32 ModuleInstall_Python(u32 stack_size);
//    ModuleInstall_Python(CFG_PYTHON_STACKSIZE);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"microPython113" //组件名
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:third               //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable               //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:pre-main            //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"shell"            //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_MICROPYTHON113 == false )
//#warning  " myname 组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_MICROPYTHON113    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,0x10000,
#define CFG_PYTHON_STACKSIZE    4096    //"运行python的环境stack尺寸"
#define CFG_PYTHON_HEAPSIZE     4096    //"运行python的环境heap尺寸"
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#@SYMBOL,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****排除文件列表结束
//%$#@API include path         ****API头文件路径列表，用分号“;”隔开，以本文件所在目录为当前目录，允许用通配符，应用本库时，DIDE将自动把本列表中的头文件加到 -I 搜索路径中
//./;./ports/djyos;${ProjDirPath}/libmicroPython_App_Debug/build;${ProjDirPath}/libmicroPython_App_Release/build;
//%$#@end API include path
//%$#@self include path         ****编译自身的头文件路径列表，用分号“;”隔开，以本文件所在目录为当前目录，允许用通配符，编译本库时，DIDE将自动把本列表中的头文件加到 -I 搜索路径中
//./include;
//%$#@end self include path
//%$#@library path             ****列出本组件引用的二进制文件列表，用分号“;”隔开，以本文件所在目录为当前目录，允许用通配符
//./ip/my_lib_name.a
//%$#@end library path

//@#$%component end configure

