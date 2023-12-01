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

// 文件名     ：cpu_peri_uart.c
// 模块描述: DJYOS串口模块的底层驱动部分，主要实现寄存器级别的操作，如中断等
//          修改自stm32H7，删掉了DMA相关部分
// 模块版本: V1.10
// 创建人员:
// 创建时间:
// =============================================================================
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <systime.h>
#include <device/djy_uart.h>
#include "int.h"
#include "djyos.h"
#include "cpu_peri_uart.h"
#include "cpu_peri_int_line.h"
#include "stdlib.h"
#include "cpu_peri_io.h"
#include "dbug.h"
//#include "cpu_peri_dma.h"
// =============================================================================
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern ptu32_t ModuleInstall_UART(ptu32_t SerialNo);
//
//    #if CFG_UART0_ENABLE ==1
//    ModuleInstall_UART(CN_UART0);
//    #endif
//
//    #if CFG_UART1_ENABLE ==1
//    ModuleInstall_UART(CN_UART1);
//    #endif
//
//    #if CFG_UART2_ENABLE ==1
//    ModuleInstall_UART(CN_UART2);
//    #endif

//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip uart"//CPU的uart外设驱动
//parent:"uart device file"      //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                    //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                 //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                   //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                  //初始化时机，可选值：early，medium，later, pre-main。
                                   //表示初始化时间，分别是早期、中期、后期
//dependence:"uart device file"    //该组件的依赖组件名（可以是none，表示无依赖组件），
                                   //选中该组件时，被依赖组件将强制选中，
                                   //如果依赖多个组件，则依次列出
//weakdependence:"none"            //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                   //选中该组件时，被依赖组件不会被强制选中，
                                   //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                   //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_UART == false )
//#warning  " cpu_onchip_uart  组件参数未配置，使用默认配置"
//%$#@target = header    //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_UART    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,512,
#define CFG_UART0_SENDBUF_LEN       64      //"UART0发送环形缓冲区大小",
#define CFG_UART0_RECVBUF_LEN       64      //"UART0接收环形缓冲区大小",
#define CFG_UART1_SENDBUF_LEN       64      //"UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN       64      //"UART1接收环形缓冲区大小",
#define CFG_UART2_SENDBUF_LEN       64      //"UART2发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN       64      //"UART2接收环形缓冲区大小",
//%$#@enum,true,false
#define CFG_UART0_ENABLE           false       //"是否使用UART0",
#define CFG_UART1_ENABLE           false        //"是否使用UART1",
#define CFG_UART2_ENABLE           false       //"是否使用UART2",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
//@#$%component end configure
// =============================================================================

#define CN_UART0_BASE UART0_ADDR
#define CN_UART1_BASE UART1_ADDR
#define CN_UART2_BASE UART2_ADDR

static u32 TxByteTime;                    //正常发送一个字节所需要的时间
static struct tagUartReg *PutStrDirectReg;     //用于printk发送的串口寄存器
static struct tagUartReg *GetCharDirectReg;     //用于直接接收的串口寄存器
static u32 TxDirectPort;                  //用于printk发送的串口号
static u32 RxDirectPort;                  //用于直接接收的串口号
// =============================================================================
struct tagUartReg volatile * const tg_UART_Reg[] = {(struct tagUartReg *)CN_UART0_BASE,
                                                    (struct tagUartReg *)CN_UART1_BASE,
                                                    (struct tagUartReg *)CN_UART2_BASE,
                                                    };

static struct UartGeneralCB *pUartCB[CN_UART_NUM];

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

__attribute__((weak))  void Board_UartHalfDuplexSend(u8 SerialNo)
{
    return;
}
__attribute__((weak))  void Board_UartHalfDuplexRecv(u8 SerialNo)
{
    return ;
}
__attribute__((weak))  void UART_InLowPowerPinCfg(u8 SerialNo)
{
    return;
}
__attribute__((weak))  void UART_OutLowPowerPinCfg(u8 SerialNo)
{
    return ;
}
#pragma GCC diagnostic pop

// =============================================================================
static ptu32_t UART_ISR(ptu32_t port);

// =============================================================================
// 功能： 进入或退出低功耗模式
//     时钟使能 控制器使能 GPIO设置相应的状态
// 参数： port 串口号：如 CN_UART1
//     flag 进入低功耗 或退出低功耗标志：如 UART_IN_LOWPOWER
// 返回：true/false
// =============================================================================
bool_t UART_LowPowerConfig(u8 port,u8 flag)
{
   return false;
}

// =============================================================================
// 功能: 禁止uart的接收中断。
// 参数: flag,是否使用dma传输
//       port,串口号，1~8
// 返回: 无
// =============================================================================
void __UART_RxIntEnable(u8 port)
{
    tg_UART_Reg[port]->UART_DLH_IER |= (1<<UART_REG_IER_ERBFI_BIT);//中断使能
}

// =============================================================================
// 功能: 禁止uart的接收中断
// 参数: port,串口号
// 返回: 无
// =============================================================================
void __UART_RxIntDisable(u8 port)
{
    tg_UART_Reg[port]->UART_DLH_IER &= ~(1<<UART_REG_IER_ERBFI_BIT);
}

// =============================================================================
// 功能: 使能uart的发送中断
// 参数: port,串口号
// 返回: 无
// =============================================================================
static void __UART_TxIntEnable(u8 port)
{
   tg_UART_Reg[port]->UART_DLH_IER |= 1<<UART_REG_IER_ETBEI_BIT;
}

// =============================================================================
// 功能: 禁止UART的发送中断
// 参数: port,串口号
// 返回: 无
// =============================================================================
static void __UART_TxIntDisable(u8 port)
{
    tg_UART_Reg[port]->UART_DLH_IER &= ~(1<<UART_REG_IER_ETBEI_BIT);//禁止发送空中断
}

// =============================================================================
// 功能: 检查发送寄存器的状态，
// 参数：reg,被操作的寄存器组指针
// 返回: true = 缓冲区空，false = 非空
// =============================================================================
static bool_t __UART_TxTranEmpty(struct tagUartReg *reg)
{
    return ((reg->UART_USR & (1<<UART_REG_USR_TFE_BIT)) !=0);
}

bool_t UART_WaitTxEnd(u8 port)
{
    s32 timeout = 10000;
    struct tagUartReg *Reg;
    Reg = (struct tagUartReg *)tg_UART_Reg[port];
    while((false == __UART_TxTranEmpty(Reg))&& (timeout > 10))
    {
        timeout -=10;
        DJY_DelayUs(10);
    }
    return true;
}

// =============================================================================
// 功能: 检查接收寄存器的状态，有数据就返回true，否则返回false
// 参数：reg,被操作的寄存器组指针
// 返回: true = 有数据，false = 无数据
// =============================================================================
static bool_t __UART_RxHadChar(struct tagUartReg *Reg)
{
    return ((Reg->UART_USR & (1<<UART_REG_USR_RFNE_BIT)) !=0);
}

void uart_fifo_reset(struct tagUartReg *Reg)
{
    u32 value;

    value = Reg->UART_IIR_FCR;
    SET_REG_BIT(value, UART_REG_FCR_XFIFOR_BIT);
    SET_REG_BIT(value, UART_REG_FCR_RFIFOR_BIT);
    Reg->UART_IIR_FCR = value;

    for (value = 0; value < 0xff; value++);
}

// =============================================================================
// 功能: 设置串口baud,PCLK为25M，CPU主频为216M计算,该驱动只提供9600、19200、57600、
//       115200四种波特率设置
// 参数: Reg,被操作的寄存器组指针
//       port,串口号
//       baud,波特率
// 返回: 无
// =============================================================================
static void __UART_BaudSet(struct tagUartReg *Reg,u32 port,u32 baud)
{
    u32 nApbClk;
    u32 divisor;
    u32 time,value;

    nApbClk = CN_CFG_MCLK/4;
    divisor = (nApbClk + (baud << 3)) / (baud << 4);    //参考melis2.0

    // clear fifo
    uart_fifo_reset(Reg);

    // check uart busy
    time = 0xffffff;
    do
    {
        value = Reg->UART_USR;
    } while (time-- && TEST_REG_BIT(value, UART_REG_USR_BUSY_BIT));
    if (0 == time)
    {
        error_printf("uart","uart%d set baudrate check busy timeout\n", port);
        return ;
    }

    Reg->UART_LCR |= 1<<UART_REG_LCR_DLAB_BIT;
    Reg->UART_RBR_THR_DLL = divisor & 0xff;
    Reg->UART_DLH_IER = (divisor>>8) & 0xff;
    Reg->UART_LCR &= ~(1<<UART_REG_LCR_DLAB_BIT);

    // clear fifo
    uart_fifo_reset(Reg);

    if(TxDirectPort == port)
    {
        TxByteTime = 11000000/baud;     //1个字节传输时间，按10bit，+10%计算
    }
}

// =============================================================================
// 功能: 设置对应UART的IO口，包括时钟和IO配置
// 参数: SerialNo,串口号
// 注 :因串口复用引脚不确定性在需要初始化需要的引脚
// 返回: 无
// =============================================================================
static void __UART_ClkConfig(u8 SerialNo)
{

    switch(SerialNo)
    {
    case CN_UART0:
        break;
    case CN_UART1:
        break;
    case CN_UART2:
        break;
    default: break;

    }

}

// =============================================================================
// 功能: 对串口传输参数配置，包括波特率、奇偶校验、数据位、停止位
// 参数:  Reg,被操作的寄存器组指针
//        port,串口号
//        data,结构体tagCOMParam类型的指针数值
// 返回: 无
// =============================================================================
static void __UART_ComConfig(struct tagUartReg *Reg,u32 port,struct COMParam *COM)
{
    if((COM == NULL) || (Reg == NULL))
        return;
    __UART_BaudSet(Reg,port,COM->BaudRate);

    switch(COM->DataBits)               // data bits
    {
        case CN_UART_DATABITS_5:
            Reg->UART_LCR &= ~(3<<UART_REG_LCR_DLS_BIT);
            Reg->UART_LCR |= 0<<UART_REG_LCR_DLS_BIT;
            break;
        case CN_UART_DATABITS_6:
            Reg->UART_LCR &= ~(3<<UART_REG_LCR_DLS_BIT);
            Reg->UART_LCR |= 1<<UART_REG_LCR_DLS_BIT;
            break;
        case CN_UART_DATABITS_7:
            Reg->UART_LCR &= ~(3<<UART_REG_LCR_DLS_BIT);
            Reg->UART_LCR |= 2<<UART_REG_LCR_DLS_BIT;
            break;
        case CN_UART_DATABITS_8:
        case CN_UART_DATABITS_9:    //不支持9位，等同于8位处理
            Reg->UART_LCR &= ~(3<<UART_REG_LCR_DLS_BIT);
            Reg->UART_LCR |= 3<<UART_REG_LCR_DLS_BIT;
            break;
            default:break;
    }
    switch(COM->Parity)
    {
        case CN_UART_PARITY_NONE:
            Reg->UART_LCR &= ~(1<<UART_REG_LCR_PEN_BIT);
            break;

        case CN_UART_PARITY_ODD:
            Reg->UART_LCR &= ~(3<<UART_REG_LCR_EPS_BIT);
            Reg->UART_LCR |= (0<<UART_REG_LCR_EPS_BIT);
            Reg->UART_LCR |= (1<<UART_REG_LCR_PEN_BIT);//使能奇偶校验
            break;
        case CN_UART_PARITY_EVEN:
            Reg->UART_LCR &= ~(3<<UART_REG_LCR_EPS_BIT);
            Reg->UART_LCR |= (1<<UART_REG_LCR_EPS_BIT);
            Reg->UART_LCR |= (1<<UART_REG_LCR_PEN_BIT);//使能奇偶校验
            break;
        default:break;
    }
    switch(COM->StopBits)
    {
        case CN_UART_STOPBITS_1:
            Reg->UART_LCR &= ~(1<<UART_REG_LCR_STOP_BIT);
            break;
        case CN_UART_STOPBITS_1_5:
        case CN_UART_STOPBITS_2:       //不支持2位，等同于1.5位处理
            Reg->UART_LCR |= (1<<UART_REG_LCR_STOP_BIT);
            break;
        default:break;
    }
}

// =============================================================================
// 功能: 使串口开始工作，时钟和uart模块
// 参数: port,串口号
// 返回: 无
// =============================================================================
void __UART_Enable(u32 port)
{
    switch(port)
    {
    case CN_UART0:
        break;
    case CN_UART1:
        break;
    case CN_UART2:
        break;
    default: break;
    }
}

// =============================================================================
// 功能: 使串口停止工作，包括时钟和uart模块
// 参数: port,串口号
// 返回: 无
// =============================================================================
void __UART_Disable(u32 port)
{
    switch(port)
    {
    case CN_UART0:
        break;
    case CN_UART1:
        break;
    case CN_UART2:
        break;
    default: break;
    }
}

// =============================================================================
// 功能: 初始化UART对应的中断线，并初始化中断入口函数
// 参数：SerialNo,串口号
// 返回: 无
// =============================================================================
static void __UART_IntInit(u32 SerialNo)
{
    u8 IntLine;
    switch (SerialNo)
    {
        case CN_UART0:
                    IntLine = CN_INT_LINE_UART0;break;
        case CN_UART1:
                    IntLine = CN_INT_LINE_UART1;break;
        case CN_UART2:
                    IntLine = CN_INT_LINE_UART2;break;
                default:
                    return;
    }
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
    Int_SetIsrPara(IntLine,SerialNo);
}



// =============================================================================
// 功能: 硬件参数配置和寄存器的初始化，包括波特率、停止位、校验位、数据位，默认情况下:
//       波特率:115200  ； 停止位:1 ; 校验:无 ; 数据位:8bit
// 参数: SerialNo,串口号，0~3
// 返回: 无
// =============================================================================
static void __UART_HardInit(u8 SerialNo)
{
    u8 LW;
    u32 addr1,val;

    if(SerialNo >= CN_UART_NUM)
        return;
    switch(SerialNo)
    {
        case CN_UART0:
            LW=20;
            break;
        case CN_UART1:
            LW=21;
            break;
        case CN_UART2:
            LW=22;
            break;
    }

    //Open the clock gate for uart 时钟开//
    addr1 = 0x01c20068;
    val = read32(addr1);
    val |= 1 << LW;
    write32(addr1, val);
    // Deassert uart reset 复位//
    addr1 = 0x01c202d0;
    val = read32(addr1);
    val &=~( 1 << LW);
    write32(addr1, val);
    DJY_DelayUs(10000);
    // Deassert uart reset 复位//
    addr1 = 0x01c202d0;
    val = read32(addr1);
    val |= 1 << LW;
    write32(addr1, val);

    __UART_ClkConfig(SerialNo);
    tg_UART_Reg[SerialNo]->UART_IIR_FCR = 0xb7; //使能fifo

    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
   //初始化uart硬件控制数据结构
    __UART_BaudSet(tg_UART_Reg[SerialNo],SerialNo,115200);

    val = tg_UART_Reg[SerialNo]->UART_LCR;
    val &= ~0x1f;
    val |= (0x3 << 0) | (0 << 2) | (0x0 << 3);
    tg_UART_Reg[SerialNo]->UART_LCR = val;

    tg_UART_Reg[SerialNo]->UART_DLH_IER |= 0x8f;    //使能所有中断
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// =============================================================================
// 功能: 启动串口发送，其目的是触发中断，用中断方式发送数据。
// 参数: Reg,被操作的串口寄存器指针.
// 返回: 发送的个数
// =============================================================================
static u32 __UART_SendStart (ptu32_t MyReg)
{
    struct tagUartReg *Reg = (struct tagUartReg *)MyReg;
    u8 port;

    switch(MyReg)
    {
        case CN_UART0_BASE:        port = CN_UART0;    break;
        case CN_UART1_BASE:        port = CN_UART1;    break;
        case CN_UART2_BASE:        port = CN_UART2;    break;
        default:return 0;
    }

    Board_UartHalfDuplexSend(port);//切换到发送
    Reg->UART_DLH_IER |= 1<<UART_REG_IER_ETBEI_BIT;    //使能发送空中断

    return 1;
}
#pragma GCC diagnostic pop

// =============================================================================
// 功能: 串口设备的控制函数,与具体的硬件寄存器设置相关
// 参数: Reg,被操作的串口寄存器指针.
//       cmd,操作类型
//       data1,data2,含义依cmd而定
// 返回: 无意义.
// =============================================================================
static ptu32_t __UART_Ctrl(ptu32_t MyReg,u32 cmd, va_list *arg0)
{
    struct tagUartReg *Reg = (struct tagUartReg *)MyReg;
    ptu32_t result = 0;
    u32 port;

    if(Reg == NULL)
        return 0;

    switch((u32)Reg)
    {
        case CN_UART0_BASE: port = CN_UART0;break;
        case CN_UART1_BASE: port = CN_UART1;break;
        case CN_UART2_BASE: port = CN_UART2;break;
        default:return 0;
    }

    switch(cmd)
    {
        case CN_DEV_CTRL_START:
            __UART_Enable(port);
            break;
        case CN_DEV_CTRL_STOP:
            __UART_Disable(port);
            break;

        case CN_DEV_CTRL_SHUTDOWN:
            break;
        case CN_DEV_CTRL_SLEEP:
            break;
        case CN_DEV_CTRL_RESUME:
            break;
        case CN_UART_SET_BAUD:  //设置Baud
        {
            u32 baud = va_arg(*arg0, u32);
             __UART_BaudSet(Reg,port, baud);
            break;
        }
        case CN_UART_EN_RTS:
            Reg->UART_MCR |= 1<<UART_REG_MCR_RTS_BIT;
            break;
        case CN_UART_DIS_RTS:
            Reg->UART_MCR &= ~(1<<UART_REG_MCR_RTS_BIT);
            break;
        case CN_UART_EN_CTS:
            break;
        case CN_UART_DIS_CTS:
            break;
        case CN_UART_DMA_USED:
            break;
        case CN_UART_DMA_UNUSED:
            break;
        case CN_UART_COM_SET:
        {
            struct COMParam *COM = va_arg(*arg0, struct COMParam *);
            __UART_ComConfig(Reg,port,COM);
            break;
        }
        default: break;
    }
    return result;
}

// =============================================================================
// 功能: UART的中断服务函数，包括发送空、接收、错误三种中断。
//      1.接收中断，从接收FIFO中读数据，并调用UART_PortWrite写入接收缓冲区中；
//      2.发送中断，调用UART_PortRead读发送缓冲区中数据，并写入发送FIFO；
//      3.错误中断，弹出错误事件
// 参数: 中断号.
// 返回: 0.
// =============================================================================
u32 UART_ISR(ptu32_t port)
{
    struct UartGeneralCB *UCB;
    struct tagUartReg *Reg;
    u32 num,loop;
    u8 IIR;
    u8 fifobuf[64];

    UCB = pUartCB[port];
    Reg = (struct tagUartReg *)tg_UART_Reg[port];
    IIR = Reg->UART_IIR_FCR & 0x0f;

    switch(IIR)
    {
        case UART_INT_MS  :         // 0x00 modem status
            Reg->UART_MSR;
            break;
        case UART_INT_NIP :         // 0x01 no interrupt pending
            break;
        case UART_INT_THRE:         // 0x02 THR empty
//            Reg->UART_RBR_THR_DLL =0x31;
//            for(num = 0;num<10000000;num++)
//                if(Reg->UART_IIR_FCR & 0x0f == UART_INT_THRE)
//                    break;
            num = 32 - Reg->UART_TFL;    //取fifo空位
            num = UART_PortRead(UCB,fifobuf,num);
            if(num == 0)
            {
                Reg->UART_DLH_IER &= ~(1<<UART_REG_IER_ETBEI_BIT);    //禁止发送空中断
            }
            else
            {
                for(loop = 0; loop < num; loop++)
                {
                    Reg->UART_RBR_THR_DLL = fifobuf[loop];
                }
            }
            break;
        case UART_INT_RDA :         // 0x04 received data available
        case UART_INI_CT  :         // 0x0c character timeout
            num = Reg->UART_RFL;    //取fifo中数据量
            for(loop = 0; loop < num; loop++)
            {
                fifobuf[loop] = Reg->UART_RBR_THR_DLL;
            }
            if(num != 0)
            {
                num = UART_PortWrite(UCB,fifobuf,num);
                if(num != 1)
                {
                    UART_ErrHandle(UCB,CN_UART_BUF_OVER_ERR);
                }
            }

            break;
        case UART_INT_RLS :         // 0x06 receiver line status
            Reg->UART_LSR;          //clear int
            break;
        case UART_INT_BD  :         // 0x07 busy detect
            Reg->UART_USR;          //clear int
            break;
        default:break;
    }

//注，F1C系列CPU好像没有发送完成中断
//  if((Reg->ISR & USART_ISR_TC)&&(Reg->CR1 & USART_CR1_TCIE))//TC
//  {
//      Reg->CR1 &=~USART_CR1_TCIE;    //关TC中断
//      Board_UartHalfDuplexRecv(port);     //不管是DMA还是中断传输，发送完了数据后都走这里，进入接收状态
//  }
    return 0;
}

// =============================================================================
// 功能：初始化uart1模块,并装载串口设备，会调用uart.c的API函数。该函数主要完成的功能
//       如下：
//       1.初始化串口的硬件部分，如GPIO、中断、配置配置的COM参数，配置baud = 115200
//         stop bits = 1,无奇偶校验, data bits = 8
//       2.调用uart.c的API函数UART_InstallPort，装入串口设备
// 参数：串口号，0~3
// 返回：1=成功，0=失败
// =============================================================================
ptu32_t ModuleInstall_UART(u32 serial_no)
{
    struct UartParam UART_Param;
    switch(serial_no)
    {
    case CN_UART0://串口0
        UART_Param.Name         = "UART0";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART0_BASE;
        UART_Param.TxRingBufLen = CFG_UART0_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART0_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;

    case CN_UART1://串口1
        UART_Param.Name         = "UART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART1_BASE;
        UART_Param.TxRingBufLen = CFG_UART1_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART1_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART2://串口2
        UART_Param.Name         = "UART2";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART2_BASE;
        UART_Param.TxRingBufLen = CFG_UART2_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART2_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    default:
        return 0;
    }
    UART_Param.mode = CN_UART_GENERAL;
    Board_UartHalfDuplexRecv(serial_no);
    //硬件初始化
    __UART_HardInit(serial_no);
    __UART_IntInit(serial_no);
    pUartCB[serial_no] = UART_InstallGeneral(&UART_Param);
    if( pUartCB[serial_no] == NULL)
        return 0;
    else
        return 1;
}

// =============================================================================
// 功能：字符终端直接发送函数，采用轮询方式，直接写寄存器，用于printk，或者stdout
//       没有初始化
// 参数：str，发送字符串指针
//      len，发送的字节数
// 返回：0，发生错误；result,发送数据长度，字节单位
// =============================================================================
s32 Uart_PutStrDirect(const char *str,u32 len)
{
    u32 result = 0;
    s32 timeout = TxByteTime * len;
    u16 CR_Bak;
    u8 port;
    for(port=0;port<CN_UART_NUM;port++)
    {
        if(PutStrDirectReg ==tg_UART_Reg[port])
            break;
    }
    Board_UartHalfDuplexSend(port);
    CR_Bak = PutStrDirectReg->UART_DLH_IER;                          //Save INT
    PutStrDirectReg->UART_DLH_IER &= ~(1<<UART_REG_IER_ETBEI_BIT);//禁止发送空中断
    for(result=0; result < len+1; result ++)
    {
        // 超时或者发送缓冲为空时退出
        while((false == __UART_TxTranEmpty(PutStrDirectReg))&& (timeout > 10))
        {
            timeout -=10;
            DJY_DelayUs(10);
        }
        if( (timeout <= 10) || (result == len))
            break;
        PutStrDirectReg->UART_RBR_THR_DLL = str[result];
    }
    while((PutStrDirectReg->UART_LSR &(1<<UART_REG_LSR_TEMT_BIT))!=0)
    {
        timeout -=10;
        DJY_DelayUs(10);
        if(timeout < 10)
           break;
    }
    PutStrDirectReg->UART_DLH_IER = CR_Bak;                         //restore send INT
    Board_UartHalfDuplexRecv(port);
    return result;
}

// =============================================================================
// 功能：字符终端直接接收函数，采用轮询方式，直接读寄存器，用于stdin初始化前
// 参数：str，发送字符串指针
//      len，发送的字节数
// 返回：0，发生错误；result,发送数据长度，字节单位
// =============================================================================
char Uart_GetCharDirect(void)
{
    u16 CR_Bak;
    char result;

    CR_Bak = PutStrDirectReg->UART_DLH_IER;                          //Save INT
    PutStrDirectReg->UART_DLH_IER &= ~(1<<UART_REG_IER_ERBFI_BIT);//禁止接收中断
    while(__UART_RxHadChar(GetCharDirectReg) == false)
        DJY_EventDelay(1000);

    result = GetCharDirectReg->UART_RBR_THR_DLL;
    PutStrDirectReg->UART_DLH_IER = CR_Bak;                         //restore send INT
    return result;
}

//----初始化内核级IO-----------------------------------------------------------
//功能：初始化内核级输入和输出所需的runtime函数指针。
//参数：无
//返回：无
//-----------------------------------------------------------------------------
void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut)
{
    if(!strcmp(StdioOut,"/dev/UART0"))
    {
        PutStrDirectReg = (struct tagUartReg*)CN_UART0_BASE;
        TxDirectPort = CN_UART0;
    }
    else if(!strcmp(StdioOut,"/dev/UART1"))
    {
        PutStrDirectReg = (struct tagUartReg*)CN_UART1_BASE;
        TxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioOut,"/dev/UART2"))
    {
        PutStrDirectReg = (struct tagUartReg*)CN_UART2_BASE;
        TxDirectPort = CN_UART2;
    }
    else
    {
        PutStrDirectReg = NULL ;
    }

    if(PutStrDirectReg != NULL)
    {
        __UART_HardInit(TxDirectPort);
        TxByteTime = 95;      //初始化默认115200，发送一个byte是87uS,+10%容限
        PutStrDirect = Uart_PutStrDirect;
    }

    if(!strcmp(StdioIn,"/dev/UART0"))
    {
        GetCharDirectReg = (struct tagUartReg*)CN_UART0_BASE;
        RxDirectPort = CN_UART0;
    }
    else if(!strcmp(StdioIn,"/dev/UART1"))
    {
        GetCharDirectReg = (struct tagUartReg*)CN_UART1_BASE;
        RxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioIn,"/dev/UART2"))
    {
        GetCharDirectReg = (struct tagUartReg*)CN_UART2_BASE;
        RxDirectPort = CN_UART2;
    }
    else
    {
        GetCharDirectReg = NULL ;
    }
    if(GetCharDirectReg != NULL)
    {
        if(TxDirectPort != RxDirectPort)
            __UART_HardInit(RxDirectPort);
        GetCharDirect = Uart_GetCharDirect;
    }
    return;
}

