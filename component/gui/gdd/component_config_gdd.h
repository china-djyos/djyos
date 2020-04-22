#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern void ModuleInstall_Gdd_AND_Desktop(void);
//    ModuleInstall_Gdd_AND_Desktop();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"graphical decorate development"//填写该组件的名字
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:system                          //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                          //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                            //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                          //初始化时机，可选值：early，medium，later, pre-main。
                                            //表示初始化时间，分别是早期、中期、后期
//dependence:"graphical kernel","message queue"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                            //选中该组件时，被依赖组件将强制选中，
                                            //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:                           //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                            //选中该组件时，被依赖组件不会被强制选中，
                                            //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                            //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT == false )
//#warning  " graphical_decorate_development  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,800,
#define CFG_DESKTOP_WIDTH       0           //"桌面宽度",桌面尺寸（像素数）宽度，0=显示器宽度
#define CFG_DESKTOP_HEIGHT      0           //"桌面高度",桌面尺寸（像素数）高度，0=显示器高度
//%$#@enum,true,false,
#define CFG_DESKTOP_BUF         true        //"桌面窗口是否带缓存"，内存稀少的硬件可不带缓存
//%$#@string,1,127,
#define CFG_DISPLAY_NAME        "DISPLAY_NAME"         //"显示器名",须与bsp中显示器驱动模块配置的显示器名字相同
#define CFG_DESKTOP_NAME        "DESKTOP_NAME"         //"给桌面起个名字"，
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#define CFG_DESKTOP_FORMAT      CN_SYS_PF_RGB565    //"像素格式",桌面窗口像素格式，常数在gkernel.h中定义，一般使用与显示器相同颜色
#define CFG_GRAY_BASE_COLOR     CN_COLOR_WHITE      //"灰度基色",像素格式设为灰度时才需要设置的“最亮”色，可在gkernel.h中找到常用颜色定义
#define CFG_FILL_COLOR          CN_COLOR_BLUE       //"填充色",创建桌面时的填充色，用888格式，可在gkernel.h中找到常用颜色定义
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
