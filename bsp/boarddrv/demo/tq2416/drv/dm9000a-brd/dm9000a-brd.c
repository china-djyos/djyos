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
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块:图形
//作者：lst
//版本：V1.0.0
//文件描述: tq2416板的dm9000a接口程序,把chipdrv目录下的dm9000a.c中与板件相关的
//          内容分拆而来
//其他说明:
//修订历史:
//1. 日期: 2015-09-06
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include <stdint.h>
#include <stddef.h>

#include "cpu_peri.h"
#include "dm9000a.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_DM9000aBrd(const char *devname,u8 *mac);
//    u8 mac[] = {0xaa,0xbb,0xcc,0xdd,0xee,0x02};
//    ModuleInstall_DM9000aBrd(CFG_DM9000_DEV_NAME,mac);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"ethernet dm900a"//dm9000a与板件相关的特性设置
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                    //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                 //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                   //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                 //初始化时机，可选值：early，medium，later, pre-main。
                                   //表示初始化时间，分别是早期、中期、后期
//dependence:"cpu peripheral register definition","ethernet dm9000a"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                   //选中该组件时，被依赖组件将强制选中，
                                   //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"            //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                   //选中该组件时，被依赖组件不会被强制选中，
                                   //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                   //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_ETHERNET_DM900A == false )
//#warning  " ethernet_dm900a  组件参数未配置，使用默认配置"
//%$#@target = header              //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_ETHERNET_DM900A    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_DM9000_DEV_NAME              "MAC_DM9000_DEV"      //"name",配置网卡名称
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

// =============================================================================
// 功能：dm9000a初始化中与板件相关的部分。
//      DM9000中断初始化。DM9000中断引脚接到CPU的GPIO引脚，因此需将该引脚配置为外部
//      中断，并配置触发电平；同时需连接中断线到中断系统
// 参数：无
// 返回值：
// =============================================================================
static bool_t __ConfigDm9000IO(void)
{
    struct SMC_REG *smc =SMC_REG_BASE;

    smc->SMBCR4 =(7<<22)|(1<<7)|(1<<4);  //Bank4 16bit
    //配置外部引脚EINT4/GPF4
    pg_gpio_reg->GPFCON &= ~(3<<8);
    pg_gpio_reg->GPFCON |= (2<<8);     //配置为输入
    pg_gpio_reg->EXTINT0 &= ~(7<<16);
    pg_gpio_reg->EXTINT0  |= (4<<16);   //配置上升沿触发
    pg_gpio_reg->EINTPEND |= (1<<4);
    pg_gpio_reg->EINTMASK &= (~(01<<4));

    return true;
}

static bool_t __ClearDm9000CpuInt(u32 irqno)
{
    if(pg_gpio_reg->EINTPEND & (1<<4))              //DM9000a外部中断标志
    {
        pg_gpio_reg->EINTPEND |= (1<<4);            //清外部中断标志
    }

    Int_ClearLine(irqno);

    return true;
}

#define CN_DMA9000A_CMDADDR    ((u16 *)0x20000000)
#define CN_DMA9000A_DATADDR    ((u16 *)(0x20000000|(0xf<<1)))
//----网卡安装程序------------------------------------------------------------
//功能：安装网卡。
//参数：无
//返回：true = 成功安装，false = 安装失败
//----------------------------------------------------------------------------
bool_t ModuleInstall_DM9000aBrd(const char *devname,u8 *mac)
{
    tagDm9000Para  para;

    __ConfigDm9000IO();
    //如果有多块dm9000a网卡，一下过程重复多次即可
    //dm9000a.h中没有给各个函数加上“网卡号”这个参数，是从效率考虑。
    //这样，有多块网卡时，这4个函数要实现4份，因函数体都很小，空间占用并不显著
    //增加，但执行却会快许多。
    para.devname = devname;
    para.mac = mac;
    para.clearextint = __ClearDm9000CpuInt;
    para.irqno = CN_INT_LINE_EINT4_7;
    para.cmdaddr = CN_DMA9000A_CMDADDR;
    para.dataddr = CN_DMA9000A_DATADDR;

    return Dm9000Install(&para);
}
