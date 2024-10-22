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
//所属模块: uart驱动程序(具体驱动部分)
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:
//其他说明:
//修订历史:
//1. 日期:2013-11-10
//   作者:  罗侍田.
//   新版本号：1.0.0
//   修改说明: 移植字44b0的1.0.1版
//------------------------------------------------------
#include "stdint.h"

#include <device/djy_uart.h>
#include "cpu_peri_uart.h"
#include <cdef21469.h>
#include "def21469.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern ptu32_t ModuleInstall_UART(u32 serial_no);
//
//    #if CFG_UART1_ENABLE ==1
//    ModuleInstall_UART(CN_UART1);
//    #endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip uart"//CPU的uart外设驱动
//parent:"uart device file"      //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                       //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                    //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                      //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                    //初始化时机，可选值：early，medium，later, pre-main。
                                      //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","lock","uart device file"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                      //选中该组件时，被依赖组件将强制选中，
                                      //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"               //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                      //选中该组件时，被依赖组件不会被强制选中，
                                      //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                      //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_UART == false )
//#warning  " cpu_onchip_uart  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_UART    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,32,512,

#define CFG_UART1_SENDBUF_LEN            256               //"UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN            256               //"UART1接收环形缓冲区大小",

//%$#@enum,true,false,
#define CFG_UART1_ENABLE                 false             //"配置是否使用UART1",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================




static struct UartUCB tg_uart1_CB;
static djy_handle_t pg_uart1_rhdl;

//#define uart1_buf_len  2048

uint8_t uart1_send_buf[CFG_UART1_SENDBUF_LEN];
uint8_t uart1_recv_buf[CFG_UART1_RECVBUF_LEN];

//----使能接收中断-------------------------------------------------------------
//功能: 使能uart的接收中断。
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __Uart_RecvIntEnable(tag_UartReg volatile *reg)
{
    reg->rUART0LCR &= ~UARTDLAB;    // clear UARTDLAB bit , then we can access to UART0THR and UART0RBR or UART0IER

    reg->rUART0IER_DLH   |= UARTRBFIE;      // enables UART0 receive interrupt

}

//----禁止接收中断-------------------------------------------------------------
//功能: 禁止uart的接收中断。
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __Uart_RecvIntDisable(tag_UartReg volatile *reg)
{
/// reg->rUART0LCR &= ~UARTDLAB;                // clear UARTDLAB bit , then we can access to UART0THR and UART0RBR or UART0IER

/// reg->rUART0IER_DLH   &= ~UARTRBFIE; // disables UART0 receive interrupt

}

//----使能发送中断-------------------------------------------------------------
//功能: 使能uart的发送中断。
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __Uart_SendIntEnable(tag_UartReg volatile *reg)
{
    reg->rUART0LCR &= ~UARTDLAB;                // clear UARTDLAB bit , then we can access to UART0THR and UART0RBR or UART0IER

    reg->rUART0IER_DLH   |= UARTTBEIE;  // enables UART0 transmit interrupt

}

//----禁止发送中断-------------------------------------------------------------
//功能: 禁止uart的发送中断。
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __Uart_SendIntDisable(tag_UartReg volatile *reg)
{
    reg->rUART0LCR &= ~UARTDLAB;                // clear UARTDLAB bit , then we can access to UART0THR and UART0RBR or UART0IER

    reg->rUART0IER_DLH   &= ~UARTTBEIE; // disables UART0 transmit interrupt

}

//----设置baud-----------------------------------------------------------------
//功能: 设置串口baud
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __UART_BaudSet(tag_UartReg volatile *reg,u32 baud)
{
    u32 temp;
    /* Sets the Baud rate for UART0 */
    reg->rUART0LCR |= UARTDLAB;  //enables access to Divisor register to set bauda rate
    temp = (u32)(CN_CFG_PCLK1)/(baud*16);
    reg->rUART0THR_RBR_DLL = (u8)temp;      //0x5b8 = 1464 for divisor value and gives a baud rate of 9600 for core clock 262.144MHz
    reg->rUART0IER_DLH = (u8)(temp>>8);

}
//----设置硬件参数-------------------------------------------------------------
//功能: 设置硬件参数，包括波特率、停止位、校验位、数据位
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __Uart_DefaultSet(tag_UartReg volatile *reg)
{
    __UART_BaudSet(reg,115200);            //波特率设置

    /* Configures UART0 LCR */
    reg->rUART0LCR = UARTWLS8;              // word length 8
                                        // parity enable None parity
                                        // One stop bits

    *pUART0RXCTL = UARTEN;       //enables UART0 in receive mode

    *pUART0TXCTL = UARTEN;       //enables UART0 in core driven mode

    __Uart_RecvIntEnable((tag_UartReg *)reg);
    __Uart_SendIntEnable((tag_UartReg *)reg);

}
//----检查串口发送寄存器是否空-------------------------------------------------
//功能: 检查发送寄存器的状态，如果空就返回true
//参数：无。
//返回: true = 缓冲区空，false = 非空
//-----------------------------------------------------------------------------
bool_t __Uart_TxTranEmpty(volatile tag_UartReg *reg)
{
    return ((0 != (reg->rUART0LSR & UARTTHRE))&&(0 != (reg->rUART0LSR & UARTTEMT)));
}

//----串口设备控制函数---------------------------------------------------------
//功能: 串口设备的控制函数,与具体的硬件寄存器设置相关
//参数: uart_dev,被操作的串口设备指针.
//      cmd,操作类型
//      data,含义依cmd而定
//返回: 无意义.
//-----------------------------------------------------------------------------
ptu32_t __Uart_Ctrl(djy_handle_t uart_dev,
                            u32 cmd, u32 data1,u32 data2)
{
    struct UartUCB *uart_port;
    uart_port = (struct UartUCB*)Driver_DevGetMyTag(uart_dev);

    switch(cmd)
    {
        case CN_DEV_CTRL_START:
            __Uart_RecvIntEnable((tag_UartReg *)uart_port->my_reg);
            __Uart_SendIntEnable((tag_UartReg *)uart_port->my_reg);
            break;
        case CN_DEV_CTRL_STOP:
            __Uart_RecvIntDisable((tag_UartReg *)uart_port->my_reg);
            __Uart_SendIntDisable((tag_UartReg *)uart_port->my_reg);
            break;
        case CN_DEV_CTRL_SHUTDOWN:
            break;
        case CN_DEV_CTRL_SLEEP:
            break;
        case CN_DEV_CTRL_RESUME:
            break;
        case CN_UART_SET_BAUD:  //设置Baud
        if(uart_port->baud !=data1)
        {
            uart_port->baud = data1;
            __UART_BaudSet((tag_UartReg *)uart_port->my_reg,data1);
        }
            break;
        case CN_UART_RX_PAUSE:      //暂停接收
            __Uart_RecvIntDisable((tag_UartReg *)uart_port->my_reg);
            break;
        case CN_UART_RX_RESUME:      //恢复接收
            __Uart_RecvIntEnable((tag_UartReg *)uart_port->my_reg);
            break;
        case CN_UART_RECV_HARD_LEVEL:    //设置接收fifo触发水平
        {
            break;
        }
        default: break;
    }
    return 0;
}


//----串行发送服务（直接写终端）----------------------------------------------
//功能: 这个是直接写串口函数，不会经过事件弹出
//参数: uart_dev,被操作的串口设备指针.
//      send_buf,被发送的缓冲数据
//      len,发送的数据字节数
//      timeout,超时
//返回: 发送的个数
//-----------------------------------------------------------------------------
u32 __Uart_SendDirectly(djy_handle_t uart_dev,
                                      u8 *send_buf,u32 len,u32 timeout)
{
    u32  result,timecount;
    struct UartUCB   *uart_port;

    uart_port = (struct UartUCB*)Driver_DevGetMyTag(uart_dev);
    __Uart_SendIntDisable((tag_UartReg *)uart_port->my_reg);
    for(result=0; result < len; result ++)
    {
        timecount=timeout;
        while((false == __Uart_TxTranEmpty((tag_UartReg *)uart_port->my_reg))
            && (timecount > 0))//超时或者发送缓冲为空时退出
        {
            timecount--;
            DJY_DelayUs(1);
        }
        if(timecount == 0)
            break;

        ((tag_UartReg *)uart_port->my_reg)->rUART0THR_RBR_DLL = send_buf[result];

        timecount=timeout;
        while((false == __Uart_TxTranEmpty((tag_UartReg *)uart_port->my_reg))
            && (timecount > 0))//超时或者发送缓冲为空时退出
        {
            timecount--;
            DJY_DelayUs(1);
        }
        if(timecount == 0)
            break;

    }
    __Uart_SendIntEnable((tag_UartReg *)uart_port->my_reg);
    return result;
}

//----启动串口发送函数---------------------------------------------------------
//功能: 启动串口发送，直接发送fifo大小的数据，并产生发送空中断，在中断中将
//      send_ring_buf中的数据完成所有数据的发送，相当于启动了一次发送功能
//参数: uart_dev,被操作的串口设备指针.
//返回: 发送的个数
//-----------------------------------------------------------------------------
u32 __Uart_SendStart(djy_handle_t uart_dev)
{
    u8 trans,num,ch[16];
    struct UartUCB *uart_port;
    uart_port = (struct UartUCB *)Driver_DevGetMyTag(uart_dev);

    __Uart_SendIntDisable((tag_UartReg *)uart_port->my_reg);
    if(__Uart_TxTranEmpty((tag_UartReg *)uart_port->my_reg))
    {
        trans = Driver_DevReadRight(uart_dev,(ptu32_t)ch,1,0);
        for(num = 0; num < trans; num++)
        {
            ((tag_UartReg *)uart_port->my_reg)->rUART0THR_RBR_DLL = ch[num];
        }
        while (!__Uart_TxTranEmpty((tag_UartReg *)uart_port->my_reg));
    }
    __Uart_SendIntEnable((tag_UartReg *)uart_port->my_reg);
    return trans;
}
//----uart中断---------------------------------------------------------------
//功能: 接收，读取fifo中的数据并从右手写入设备
//      发送，从右手读数据并发送
//参数: 中断号.
//返回: 0.
//-----------------------------------------------------------------------------
uint32_t Uart_Int(ptu32_t uart_int_line)
{

    static djy_handle_t *uart_rhdl;
    tag_UartReg *reg;
    uint32_t recv_trans,num;

    uint8_t ch[20],IIR=0;

    reg =  (tag_UartReg *)tg_uart1_CB.my_reg;
    IIR = reg->rUART0IIR;
    uart_rhdl = &pg_uart1_rhdl;

    IIR = (IIR>>1) &0x7;
    switch(IIR)
    {
        case 1:     //Transmitter data
        {
            while (!__Uart_TxTranEmpty((tag_UartReg *)reg));
            recv_trans = Driver_DevReadRight(*uart_rhdl,(ptu32_t)ch,1,0);
            for(num = 0; num < recv_trans; num++)
            {
                reg->rUART0THR_RBR_DLL = ch[num];
            }
            while (!__Uart_TxTranEmpty((tag_UartReg *)reg));
        }break;
        case 2:     //Receive data
        {
            for(num = 0; num<1; num++)
            {
                ch[num] = reg->rUART0THR_RBR_DLL;

            }
            Driver_DevWriteRight(*uart_rhdl,(ptu32_t)ch,num,0);
        }break;
       default:
       {
            while (!__Uart_TxTranEmpty((tag_UartReg *)reg));
            break;
       }
    }
    return 0;
}

//----初始化uart1模块----------------------------------------------------------
//功能：初始化uart1模块,并装载串口设备
//参数：串口初始化模块的参数为1、2、3...串口号
//返回：1=成功，0=失败
//-----------------------------------------------------------------------------
ptu32_t Uart_ModuleInit(ptu32_t para)//todo fix
{
    static djy_handle_t *uart_rhdl;
    struct UartUCB *UCB;
    char *name;
    uint8_t *sendbuf,*recvbuf;
    uint32_t sendbuflen,recvbuflen;
    ptu32_t base_addr;

    switch(para)
    {
    case 1://串口1
        UCB = &tg_uart1_CB;
        base_addr = cn_uart1_base;
        name = "uart1";
        sendbuf = uart1_send_buf;
        sendbuflen = CFG_UART1_SENDBUF_LEN;
        recvbuf = uart1_recv_buf;
        recvbuflen = CFG_UART1_RECVBUF_LEN;
        uart_rhdl = &pg_uart1_rhdl;
        break;
    default:
        return 0;
    }
    UCB->my_reg = (tag_UartReg *)base_addr;
    __Uart_DefaultSet((tag_UartReg *)UCB->my_reg);
    UCB->baud = 115200;
    UCB->start_send  = __Uart_SendStart;
    UCB->direct_send = __Uart_SendDirectly;
    UCB->Uart_Ctrl   = __Uart_Ctrl;
    //中断线的初始化
//    Int_Register(cn_int_line_SP7I);
//    Int_IsrConnect(cn_int_line_SP7I,Uart_Int);    Int_IsrConnect(cn_int_line_UARTI,Uart_Int);
//    Int_SettoAsynSignal(cn_int_line_SP7I);
//    Int_ClearLine(cn_int_line_SP7I);     //清掉初始化产生的发送fifo空的中断
//    Int_RestoreAsynLine(cn_int_line_SP7I);

    Int_Register(cn_int_line_UARTI);
    Int_IsrConnect(cn_int_line_UARTI,Uart_Int);
    Int_SettoAsynSignal(cn_int_line_UARTI);
    Int_ClearLine(cn_int_line_UARTI);     //清掉初始化产生的发送fifo空的中断
    Int_RestoreAsynLine(cn_int_line_UARTI);

    *uart_rhdl = Uart_InstallPort(UCB,name,sendbuf,
                                sendbuflen,recvbuf,recvbuflen);
    if(*uart_rhdl != NULL)
        Driver_DevOpenRightFast(*uart_rhdl,0);

    return 0;
}


////////   test use only
u32 Uart_SendServiceDirectly(char *str)
{
    return 0;
}

