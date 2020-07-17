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

// 文件名     ：board.c
// 模块描述: 板件相关部分初始化或配置等
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 5/11.2015
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "os.h"
#include "cpu_peri.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern void Board_Init(void);
//  Board_Init();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"board config"//组件名
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                      //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:required                    //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                     //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                    //初始化时机，可选值：early，medium，later, pre-main。
                                     //表示初始化时间，分别是早期、中期、后期
//dependence:"kernel","atsamv7","cpu onchip pio","cpu onchip gpio"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                     //选中该组件时，被依赖组件将强制选中，
                                     //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"              //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                     //选中该组件时，被依赖组件不会被强制选中，
                                     //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                     //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  组件参数未配置，使用默认配置"
//%$#@target = header                //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

//串口引脚定义
static const Pin uart3_pin[] = {
    {PIO_PD28A_URXD3, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
    {PIO_PD30A_UTXD3, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT}};

static const Pin usart0_pin[] = {
    {PIO_PB0C_RXD0, PIOB, ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT},
    {PIO_PB1C_TXD0, PIOB, ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT}};

static const Pin usart1_pin[] = {
    {PIO_PA21A_RXD1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT},
    {PIO_PB4D_TXD1, PIOB, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT}};

static const Pin Qspi_pin[] = {
        {(PIO_PA11A_QCS | PIO_PA13A_QIO0 | PIO_PA12A_QIO1 | PIO_PA17A_QIO2
                | PIO_PA14A_QSCK),PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD31A_QIO3, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
};

static const Pin Gmac_pin[] = {
        {(PIO_PD0A_GTXCK | PIO_PD1A_GTXEN | PIO_PD2A_GTX0 | PIO_PD3A_GTX1
        | PIO_PD4A_GRXDV | PIO_PD5A_GRX0 | PIO_PD6A_GRX1 | PIO_PD7A_GRXER
        | PIO_PD8A_GMDC | PIO_PD9A_GMDIO ),PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},

        {PIO_PC30, PIOC, ID_PIOC, PIO_INPUT,    PIO_PULLUP},
        {PIO_PA29, PIOA, ID_PIOA, PIO_INPUT,    PIO_DEFAULT}
};

static const Pin Gmac_Reset_Pin[] = {
        {PIO_PC10, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_PULLUP}
};

static const Pin Phy_Interrupt_Pin[] = {
        {PIO_PA19, PIOA, ID_PIOA, PIO_INPUT,    PIO_IT_LOW_LEVEL}
};


void Board_UartHalfDuplexSend(u8 SerialNo)
{

}
void Board_UartHalfDuplexRecv(u8 SerialNo)
{

}


// =============================================================================
// 功能：初始化板件所有GPIO功能，包括普通GPIO口，外设所用IO(如串口，网口等)
// 参数：无
// 返回：true
// 说明：该函数必须在所有外设功能启用前调用，建议在module-trim中Sys_ModuleInit第一个
//      函数调用
// =============================================================================
bool_t Board_Init(void)
{
    PIO_Configure(usart0_pin, PIO_LISTSIZE(usart0_pin));
    PIO_Configure(usart1_pin, PIO_LISTSIZE(usart1_pin));
    PIO_Configure(uart3_pin, PIO_LISTSIZE(uart3_pin));

    PIO_Configure(Qspi_pin, PIO_LISTSIZE(Qspi_pin));

    //硬件IO复位PHY芯片
    PIO_Configure(Gmac_Reset_Pin, PIO_LISTSIZE(Gmac_Reset_Pin));
    PIO_Clear(Gmac_Reset_Pin);
    DJY_DelayUs(1000);
    PIO_Set(Gmac_Reset_Pin);

    //GMAC的IO引脚初始化
    PIO_Configure(Gmac_pin, PIO_LISTSIZE(Gmac_pin));

    return true;
}
