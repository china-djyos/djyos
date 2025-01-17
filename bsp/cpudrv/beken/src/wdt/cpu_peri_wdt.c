//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "wdt_soft.h"
#include <wdt_pub.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern void ModuleInstall_BrdWdt(void);
//    ModuleInstall_BrdWdt();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip iwdt"//组件名
//parent:"watch dog"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:pre-main               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"watch dog"        //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_IWDT == false )
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_IWDT    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100000000,
#define CFG_WDT_FEED_CYCLE          12000000       //"看门狗超时时间"，单位us
#define CFG_BOOT_TIME_LIMIT         30000000       //"启动加载超限时间",允许保护启动加载过程才需要配置此项
//%$#@enum,true,false,
#define CFG_DEFEND_ON_BOOT          false          //"保护启动过程",启动加载过程如果出现死机，看门狗将复位，BK7251不支持，只能选false
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


// BOARD WDT
// =============================================================================

static u32 sBootDogFeedTime = 0;
// =============================================================================
// 功能：板上看门狗喂狗函数,max706的喂狗时间是1.6s,根据手册，喂狗是边沿触发
// 参数：无
// 返回：无
// =============================================================================
bool_t BrdWdt_FeedDog(void)
{
//    printf("--hard feed dog\r\n");        //lst
    if(sddev_control(WDT_DEV_NAME, WCMD_RELOAD_PERIOD, 0) == 0)
        return true;
    else
        return false;
}

//----启动boot期间喂狗----------------------------------------------------------
//功能：启动boot期间喂狗功能，一般来说，须启动一个定时器，中断周期是CFG_WDT_FEED_CYCLE
//      的80%，中断设为实时中断，在其ISR中，执行喂狗操作。CFG_BOOT_TIME_LIMIT 时间之后，
//      停止喂狗。
//参数：无
//返回：true
//-----------------------------------------------------------------------------
bool_t BrdBoot_FeedStart( void)
{
//    sBootDogFeedTime = bootfeedtime;
//    sddev_control(WDT_DEV_NAME, WCMD_POWER_UP, 0);      //开看门狗
//    sddev_control(WDT_DEV_NAME, WCMD_SET_PERIOD, &sBootDogFeedTime);    //设置看门狗，狗叫时间
    return true;
}

//----停止启动boot期间喂狗-------------------------------------------------------
//功能：把硬件看门狗模块安装到系统后，正常的喂狗操作即将开始，由定时器ISR喂狗即结束。
//参数：无
//返回：true
//-----------------------------------------------------------------------------
bool_t __BrdWdt_FeedDog(void)
{
    BrdWdt_FeedDog();
    return true;
}

// =============================================================================
// 功能：板上看门狗芯片初始化，此函数在软看门狗组件后面初始化，如果启动了“防护启动加载过程”
//      的功能，本函数调用后，将停止自动喂狗。
// 参数：无
// 返回：无
// =============================================================================
bool_t ModuleInstall_BrdWdt(void)
{
    bool_t result;
    sBootDogFeedTime = CFG_WDT_FEED_CYCLE * 10 / 6;
    sddev_control(WDT_DEV_NAME, WCMD_POWER_UP, 0);      //开看门狗
    sddev_control(WDT_DEV_NAME, WCMD_SET_PERIOD, &sBootDogFeedTime);    //设置看门狗，狗叫时间
    result = WdtHal_RegisterWdtChip("Wdt_BK7251", CFG_WDT_FEED_CYCLE/4, BrdWdt_FeedDog);
#if(CFG_DEFEND_ON_BOOT == true)
    __BrdWdt_FeedDog();
#endif
    return result;
}


