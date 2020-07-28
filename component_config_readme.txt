DJYOS的内核以及组件、bsp等的裁剪和配置功能，由源码和IDE配合完成，手痒难耐的hacker，也可以自己裁剪。
源码中（c/cpp或h/hpp）中放置一段特定格式的描述文本，IDE扫描得到配置信息，以图形化的方式进行配置、裁剪。
IDE中有图形配置界面，配置完成后，会生成 projec_config.h 文件放在工程目录中；如果是第三方组件，则把生成的符号放在命令行中。
这样做的好处：
1、不管如何配置，djysrc保持不变，不需要为特定工程修改djysrc，使djysrc能够维持一份copy，便于维护。
2、即使不创建工程，单独打开djysrc，源码也是可读的，不会出现找不到#define的常量而导致代码不可读的问题。

具体又分两种情况：
1、只包含1个C文件的组件。
2、2个以上文件组成的组件。

对于第一种情况，c/cpp文件在组件配置块之前增加这句：
#include  "projec_config.h"

对于第二种情况，整个组件和目录是唯一对应的，即组件全部代码必须在一个目录树中。
该目录中，必须有一个名为component_config_myname.h的头文件，不允许放在子目录中。
component_config_myname.h文件的内容，与第一种情况的c/cpp文件一致。
c/cpp文件在组件配置块之前增加这句：
#include component_config_myname.h

配置语法：

1、组件配置块

整个配置内容包含在“//@#$%component configure” 和 “//@#$%component end configure”两个标签中间，这对标签每个文件中只能出现一次

2、组件初始化代码描述块

用于帮助IDE生成初始化代码。
代码包含在标签“//%$#@initcode”和“//%$#@end initcode”之间，处于行注释状态，IDE将删除"//"后，copy到初始化文件中。

3、组件描述块

用于描述组件的属性，例如组件名、父组件名等，包含在标签“//%$#@describe”和“//%$#@end describe”之间
组件属性包括：
//component name:"example"      //该组件的名字
//parent:"none"                 //该组件的父组件名字，none表示没有父组件
//attribute:用户组件            //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:可选                   //选填“必选、可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，IDE裁剪界面中不显示
//init time:medium              //初始化时机，可选值：early，medium，later,pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出

4、组件参数配置块

包含在标签“//%$#@configue//%$#@end configue ”之间，用于配置组件的参数。
DJYOS自有模块的参数配置一律使用有参数值格式：
#define     CFG_PARAM   value       //"name",参数的注释
只有第三方组件，允许使用无参数值格式：
#define     CFG_PARAM               //"name",参数的注释
其中"name"是显示在IDE图形配置界面中的名字，与真正的注释用逗号隔开。
在第一个参数配置开始，加入了一段代码：（SDCARD_POWER来自组件名）
#if ( CFG_MODULE_ENABLE_SDCARD_POWER == false )
//#warning  " board_config  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_SDCARD_POWER    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
这样写的好处是，即使用户没有配置，组件也能正常编译、阅读，并且能够给出警告，增强了可读性，避免了传统开源软件必须结合makefile和configure这些天书式的文件才能读懂的毛病。
各参数自身的属性，使用标签加以说明，IDE的图形配置界面中，将据此做规则检查，各标签说明如下：
//%$#@num,0,100,                ————配置项是数值，随后是最小值，最大值，用逗号分隔，留空则不限值
//%$#@enum,true,false,          ————配置项是enum，紧跟着罗列出全部可选值
//%$#@string,1,10,              ————配置项是字符串，紧跟着串长度最小值，最大值，空则不限长度
//%$#@select,                   ————IDE中设置勾选框，注释掉为默认不勾选，否则默认勾选，勾选的将定义一个无值的宏 。只用于第三方组件，不允许自有组件使用。
//%$#@SYMBOL,                   ————直接定义无值符号，只用于第三方组件，不允许自有组件使用。
//%$#@free,                     ————自由配置，IDE不做规则检查
//%$#@object_num,0,1,2,..,100   ————配置object的数量，紧跟着罗列出全部可选值,与object_para绑定，选择此项中任意一值时，object_para项要做出动态增加与减少
//%$#@object_para,              ————配置每个object的参数，与object_num绑定，目前必须以上组件参数配置块的最后

5、被排除的文件

主要用于第三方开源代码，也允许自有代码使用，把不需要的文件排除在外。
//%$#@exclude       ****编译排除文件列表
//列出所有排除文件或文件夹，用分号“;”隔开，以本文件所在目录为当前目录，允许用通配符
//%$#@end exclude   ****排除文件列表结束

6、加到include查找路径的文件夹

用于第三方开源代码，列出本使用本开源代码时需要加到include查找路径的全部文件夹。
//%$#@API include path         ****API头文件路径列表，用分号“;”隔开，以本文件所在目录为当前目录，允许用通配符，应用本库时，DIDE将自动把本列表中的头文件加到 -I 搜索路径中
//./include;
//%$#@end API include path

//%$#@self include path         ****编译自身的头文件路径列表，用分号“;”隔开，以本文件所在目录为当前目录，允许用通配符，编译本库时，DIDE将自动把本列表中的头文件加到 -I 搜索路径中
//./include;
//%$#@end self include path

7、选中某组件后，链接APP时需要添加的二进制库
//%$#@library path
//./ip/my_lib_name.a
//%$#@end library path

//@#$%component end configure

依赖关系原则：
1、依赖具体的设备名称、文件名称的组件，放在main中初始化。
2、功能组件不可以依赖bsp中的组件，也不可以依赖第三方组件，反之可以。
