#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//#if(CFG_OS_TINY == flase)
//   extern s32 kernel_command(void);
//   kernel_command();
//#endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"kernel"//内核组件
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:required               //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                //初始化时机，可选值：early，medium，later, pre-main。
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
#if ( CFG_MODULE_ENABLE_KERNEL == false )
//#warning  " kernel  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_KERNEL    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,8192,
#define CFG_INIT_STACK_SIZE     4096    //"初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         15      //"事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    15      //"事件类型数限值",事件类型数
#define CFG_IDLESTACK_LIMIT     1024    //"IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_IDLE_MONITOR_CYCLE  30      //"IDLE监视周期",监视IDLE事件持续低于1/16 CPU占比的时间，秒数，0=不监视
//%$#@num,0,65536,
#define CFG_MAINSTACK_LIMIT     4096    //"main函数栈尺寸",main函数运行所需的栈尺寸
//%$#@enum,true,false
#define CFG_IDLE_WDT_RESET      false   //"IDLE监视狗叫复位使能"，IDLE事件监视狗叫时，是否复位
#define CFG_OS_TINY             false   //"tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//%$#@string,1,10
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

