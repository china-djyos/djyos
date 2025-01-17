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
//所属模块: uart驱动程序
//作者：lst
//版本：V1.0.0
//文件描述: 驱动2440的uart
//其他说明:
//修订历史:
//1. 日期:2009-03-10
//   作者:lst
//   新版本号：1.0.0
//   修改说明: 移植自44b0的1.0.1版
//------------------------------------------------------

#include "stdint.h"
#include "string.h"
#include "errno.h"
#include "systime.h"
#include "cpu_peri.h"
#include "int.h"
#include "djyos.h"
#include <device/djy_uart.h>
#include "string.h"
#include "cpu_peri_uart.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern ptu32_t ModuleInstall_UART(u32 serial_no);
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
#define CFG_UART0_SENDBUF_LEN            256               //"UART0发送环形缓冲区大小",
#define CFG_UART0_RECVBUF_LEN            256               //"UART0接收环形缓冲区大小",

#define CFG_UART1_SENDBUF_LEN            256               //"UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN            256               //"UART1接收环形缓冲区大小",

#define CFG_UART2_SENDBUF_LEN            256               //"UART2发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN            256               //"UART2接收环形缓冲区大小",
//%$#@enum,true,false,
#define CFG_UART0_ENABLE                 true              //"配置是否使用UART0",
#define CFG_UART1_ENABLE                 false             //"配置是否使用UART1",
#define CFG_UART2_ENABLE                 false             //"配置是否使用UART2",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

// =============================================================================
#define CN_UART0_BASE   (0x50000000)
#define CN_UART1_BASE   (0x50004000)
#define CN_UART2_BASE   (0x50008000)
// =============================================================================
//static tagUartReg volatile * const tg_UART_Reg[] = {(tagUartReg *)CN_UART0_BASE,
//                                                  (tagUartReg *)CN_UART1_BASE,
//                                                  (tagUartReg *)CN_UART2_BASE};
//#define UART0_SendBufLen            256
//#define UART0_RecvBufLen            256

//#define UART1_SendBufLen            256
//#define UART1_RecvBufLen            256

//#define UART2_SendBufLen            256
//#define UART2_RecvBufLen            256

static struct UartGeneralCB *pUartCB[CN_UART_NUM];

u32 UART0_Int(ptu32_t IntLine);
u32 UART1_Int(ptu32_t IntLine);
u32 UART2_Int(ptu32_t IntLine);

//----启动uart的中断源---------------------------------------------------------
//功能: 2410中，uart中断可能被收、发、err三个源触发，本函数选择性地启动其中一个
//      或数个中断源，具体由rsc_msk中被置1的位确定。
//参数: port，被使能的串口号
//      flag, 标记，可为接收、发送、错误
//返回: 无
//-----------------------------------------------------------------------------
void __UART_SubIntEnable(u32 port, u32 flag)
{
    pg_int_reg->INTSUBMSK &= ~(BIT_SUB_INT(port,flag) & BIT_ALL_UART);
}

//----禁止uart的中断源---------------------------------------------------------
//功能: 2410中，uart中断可能被收、发、err三个源触发，本函数选择性地禁止其中一个
//      或数个中断源，具体由rsc_msk中被置1的位确定。
//参数: port，关闭中断的串口号
//      flag, 标记，可为接收、发送、错误
//返回: 无
//-----------------------------------------------------------------------------
void __UART_SubIntDisable(u32 port, u32 flag)
{
    pg_int_reg->INTSUBMSK |= (BIT_SUB_INT(port,flag) & BIT_ALL_UART);
}

//----检查串口发送缓冲区是否满-------------------------------------------------
//功能: 检查发送fifo缓冲区的状态，如果fifo满就返回true
//参数：无。
//返回: true = 缓冲区满，false = 非满
//-----------------------------------------------------------------------------
bool_t __UART_TxFifoFull(tagUartReg *Reg)
{
    return (Reg->UFSTAT &0x4000);
}

//----检查串口发送fifo是否空---------------------------------------------------
//功能: 检查发送fifo缓冲区的状态，如果fifo空就返回true
//参数：无。
//返回: true = 缓冲区空，false = 非空
//-----------------------------------------------------------------------------
u32 __UART_GetTxFifoNum(tagUartReg *Reg)
{
    return (Reg->UFSTAT &0x3f00)>> 8;
}

//----检查串口发送寄存器是否空-------------------------------------------------
//功能: 检查发送寄存器的状态，如果空就返回true
//参数：无。
//返回: true = 缓冲区空，false = 非空
//-----------------------------------------------------------------------------
bool_t __UART_TxTranEmpty(volatile tagUartReg *Reg)
{
    return (0!=(Reg->UTRSTAT & 0x04));
}

//----设置baud-----------------------------------------------------------------
//功能: 设置串口baud
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __UART_BaudSet(volatile tagUartReg *Reg,u32 baud)
{
    Reg->UBRDIV = ((CN_CFG_PCLK<<2)/baud -32)>>6;
    Reg->UERSTAT;     //读一下清除错误状态
}

//----设置硬件参数-------------------------------------------------------------
//功能: 设置硬件参数，包括波特率、停止位、校验位、数据位,波特率默认115200
//参数: uart_reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __UART_DefaultConfig(volatile tagUartReg *Reg)
{
    //系统初始化时已经使中断处于禁止状态，无需再禁止和清除中断。
    //初始化uart硬件控制数据结构
    Reg->ULCON = 0x3;
    Reg->UCON =  0xc5;
    Reg->UFCON = 0x17;   //bit0=1使能fifo.
    Reg->UMCON = 0x0;
    Reg->UBRDIV = ((CN_CFG_PCLK<<2)/115200 -32)>>6;
    Reg->UERSTAT;     //读一下清除错误状态

}

//----配置串口的io口-----------------------------------------------------------
//功能: 配置串口所使用的io口，输入输出等配置
//参数: port,串口号，可为1、2、3
//返回: 无
//-----------------------------------------------------------------------------
void __UART_GPIOConfig(u32 port)
{
    u32 temp;
    //Ports  :  GPH10 GPH9 GPH8 GPH7 GPH6 GPH5 GPH4 GPH3 GPH2 GPH1  GPH0
    //Signal :   xx    xx   xx   xx   xx   xx   xx  RXD0 TXD0 nRTS0 nCTS0
    //Binary :   xx    xx   xx   xx   xx   xx   xx   xx   xx   xx   xx
    switch(port)
    {
    case CN_UART0:
        temp = pg_gpio_reg->GPHCON;
        temp &= ~0xff;
        temp |= 0xaa;
        pg_gpio_reg->GPHCON = temp;
        // The pull up function is disabled GPH[3:0]
        pg_gpio_reg->GPHUP |= 0xf;
        break;
    case CN_UART1:
        temp = pg_gpio_reg->GPHCON;
        temp &= ~0xf00;
        temp |= 0xa00;
        pg_gpio_reg->GPHCON = temp;
        // The pull up function is disabled GPH[5:4]
        pg_gpio_reg->GPHUP |= 0x30;
        break;
    case CN_UART2:
        temp = pg_gpio_reg->GPHCON;
        temp &= ~0xf000;
        temp |= 0xa000;
        pg_gpio_reg->GPHCON = temp;
        // The pull up function is disabled GPH[7:6]
        pg_gpio_reg->GPHUP |= 0xc0;
        break;
    default:
        break;
    }
}

//----配置串口中断-----------------------------------------------------------
//功能: 配置串口的中断，配置中断服务函数，并配置串口中断为异步信号
//参数: port,串口号，可为1、2、3
//返回: 无
//-----------------------------------------------------------------------------
void __UART_IntConfig(u32 port)
{
    //中断线的初始化
    //初始化通用串口数据结构
    switch(port)
    {
    case CN_UART0:
     Int_Register(cn_int_line_uart0);
        Int_IsrConnect(cn_int_line_uart0,UART0_Int);
        Int_SettoAsynSignal(cn_int_line_uart0);
        pg_int_reg->SUBSRCPND = BIT_SUB_TXD0+BIT_SUB_RXD0+BIT_SUB_ERR0;
        Int_ClearLine(cn_int_line_uart0);       //清掉初始化产生的发送fifo空的中断
        Int_RestoreAsynLine(cn_int_line_uart0);
        break;
    case CN_UART1:
     Int_Register(cn_int_line_uart1);
        Int_IsrConnect(cn_int_line_uart1,UART1_Int);
        Int_SettoAsynSignal(cn_int_line_uart1);
        pg_int_reg->SUBSRCPND = BIT_SUB_TXD1+BIT_SUB_RXD1+BIT_SUB_ERR1;
        Int_ClearLine(cn_int_line_uart1);       //清掉初始化产生的发送fifo空的中断
        Int_RestoreAsynLine(cn_int_line_uart1);
        break;
    case CN_UART2:
     Int_Register(cn_int_line_uart2);
        Int_IsrConnect(cn_int_line_uart2,UART2_Int);
        Int_SettoAsynSignal(cn_int_line_uart2);
        pg_int_reg->SUBSRCPND = BIT_SUB_TXD2+BIT_SUB_RXD2+BIT_SUB_ERR2;
        Int_ClearLine(cn_int_line_uart2);       //清掉初始化产生的发送fifo空的中断
        Int_RestoreAsynLine(cn_int_line_uart2);
        break;
    default:
        break;
    }
}

//----串行发送服务（直接写终端）----------------------------------------------
//功能: 这个是直接写串口函数，不会经过事件弹出
//参数: uart_dev,被操作的串口设备指针.
//      send_buf,被发送的缓冲数据
//      len,发送的数据字节数
//      timeout,超时
//返回: 发送的个数
//-----------------------------------------------------------------------------
//bool_t __UART_SendDirectly(tagUartReg *Reg,u8 *send_buf,u32 len,u32 timeout)
//{
//    volatile u32  result,port;
//
//    if((u32)Reg == CN_UART0_BASE)
//        port = CN_UART0;
//    else if((u32)Reg == CN_UART1_BASE)
//        port = CN_UART1;
//    else if((u32)Reg == CN_UART2_BASE)
//        port = CN_UART2;
//    else
//        return false;
//
//    __UART_SubIntDisable(port,SUB_TXD_FLG);
//    for(result=0; result < len; result ++)
//    {
//        while((false == __UART_TxTranEmpty(Reg))&& (timeout > 0))//超时或者发送缓冲为空时退出
//        {
//            timeout--;
//            DJY_DelayUs(1);
//        }
//        if(timeout == 0)
//            break;
//        Reg->UTXH = send_buf[result];
//    }
//    __UART_SubIntEnable(port,SUB_TXD_FLG);
//
//    return result;
//}
//----启动串口发送函数---------------------------------------------------------
//功能: 启动串口发送，直接发送fifo大小的数据，并产生发送空中断，在中断中将
//      send_ring_buf中的数据完成所有数据的发送，相当于启动了一次发送功能
//参数: uart_dev,被操作的串口设备指针.
//返回: 发送的个数
//-----------------------------------------------------------------------------
u32 __UART_SendStart(tagUartReg *Reg)
{
    u8 trans,num,ch[UART_TX_FIFO_SIZE],sending_num,port;
    struct Object *UCB;

    switch((u32)Reg)
    {
    case CN_UART0_BASE:     port = CN_UART0; break;
    case CN_UART1_BASE:     port = CN_UART1;break;
    case CN_UART2_BASE:     port = CN_UART2;break;
    default:return 0;
    }

    __UART_SubIntDisable(port,SUB_TXD_FLG);
    sending_num = __UART_GetTxFifoNum(Reg);
    if(sending_num <= UART_TX_FIFO_SIZE)//fifo bytes
    {
        trans = UART_PortRead(pUartCB[port],ch,UART_TX_FIFO_SIZE-sending_num);
        for(num = 0; num < trans; num++)
        {
            Reg->UTXH = ch[num];
        }
    }
    __UART_SubIntEnable(port,SUB_TXD_FLG);
    return trans;
}

//----串口设备控制函数---------------------------------------------------------
//功能: 串口设备的控制函数,与具体的硬件寄存器设置相关
//参数: uart_dev,被操作的串口设备指针.
//      cmd,操作类型
//      data,含义依cmd而定
//返回: 无意义.
//-----------------------------------------------------------------------------
ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd, va_list *arg0)
{
    u32  port;

    if((u32)Reg == CN_UART0_BASE)
        port = CN_UART0;
    else if((u32)Reg == CN_UART1_BASE)
        port = CN_UART1;
    else if((u32)Reg == CN_UART2_BASE)
        port = CN_UART2;
    else
        return false;

    switch(cmd)
    {
        case CN_DEV_CTRL_START:
            __UART_SubIntEnable(port,SUB_RXD_FLG);
            __UART_SubIntEnable(port,SUB_TXD_FLG);
            break;
        case CN_DEV_CTRL_STOP:
            __UART_SubIntDisable(port,SUB_RXD_FLG);
            __UART_SubIntDisable(port,SUB_TXD_FLG);
            break;
        case CN_DEV_CTRL_SHUTDOWN:
            break;
        case CN_DEV_CTRL_SLEEP:
            break;
        case CN_DEV_CTRL_RESUME:
            break;
        case CN_UART_SET_BAUD:  //设置Baud
        {
            u32 data;
            data = va_arg(*arg0, u32);
            __UART_BaudSet(Reg, data);
        }
            break;
        case CN_UART_RX_PAUSE:      //暂停接收
            __UART_SubIntDisable(port,SUB_RXD_FLG);
            break;
        case CN_UART_RX_RESUME:      //恢复接收
            __UART_SubIntEnable(port,SUB_RXD_FLG);
            break;
        case CN_UART_SEND_HARD_LEVEL:    //设置发送fifo触发水平
        {
            u32 data;
            data = va_arg(*arg0, u32);
            Reg->UFCON &= 0x3f;
            switch (data)
            {
                case 0:
                    break;
                case 4:
                    Reg->UFCON |= 0x40;
                    break;
                case 8:
                    Reg->UFCON |= 0x80;
                    break;
                case 12:
                    Reg->UFCON |= 0xc0;
                    break;
                default :break;
            }
            break;
        }
        case CN_UART_RECV_HARD_LEVEL:    //设置接收fifo触发水平
        {
            Reg->UFCON &= 0xcf;
            switch (data1)
            {
                case 0:
                    break;
                case 4:
                    Reg->UFCON |= 0x10;
                    break;
                case 8:
                    Reg->UFCON |= 0x20;
                    break;
                case 12:
                    Reg->UFCON |= 0x30;
                    break;
                default :break;
            }
            break;
        }
        default: break;
    }
    return 0;
}

//----uart0中断---------------------------------------------------------------
//功能: 如果是接收信号触发的中断:
//      1.检查接收fifo中的数据量.把数据从接收寄存器全部copy到物理层缓冲区中.
//      2.如果物理层缓冲区满.置串口控制块状态字的溢出位,并发出uart错误事件.
//      3.发送串口接收事件.两个事件都是状态量事件
//      如果是发送信号引发的中断:
//      1.检查发送fifo有多少空位
//      2.从设备右手接口读出适量数据，
//      3.把读出的数据写入发送fifo
//      如果是出错信号引发中断:弹出串口出错事件
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
//-----------------------------------------------------------------------------
u32 UART0_Int(ptu32_t IntLine)
{
    u32 recv_trans,num;
    u32 fifo_reg;
    u8 ch[UART_TX_FIFO_SIZE];
    struct UartGeneralCB *UCB;
    tagUartReg *Reg;

    UCB = pUartCB[CN_UART0];
    if(UCB == NULL)
        return 0;
    Reg = (tagUartReg *)CN_UART0_BASE;

    if(pg_int_reg->SUBSRCPND & BIT_SUB_RXD0)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_RXD0;
        fifo_reg = Reg->UFSTAT;
        recv_trans = fifo_reg & 0x3f;
        for(num = 0; num < recv_trans; num++)//fifo缓冲区有数据,直到处理完成
            ch[num] = Reg->URXH;
        UART_PortWrite(UCB,ch,recv_trans);
        if( fifo_reg & cn_rx_over)
        {
            Reg->UFCON |= cn_rx_fifo_reset;
            UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
        }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_TXD0)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_TXD0;
        fifo_reg = Reg->UFSTAT;
        recv_trans = UART_TX_FIFO_SIZE-((fifo_reg>>8) & 0x3f);
        recv_trans = UART_PortRead(UCB,ch,recv_trans);
        for(num = 0; num < recv_trans; num++)
        {
            Reg->UTXH = ch[num];
        }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_ERR0)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_ERR0;
        UART_ErrHandle(UCB,CN_UART_HARD_COMM_ERR);
    }
    return 0;
}

//----uart1中断---------------------------------------------------------------
//功能: 如果是接收信号触发的中断:
//      1.检查接收fifo中的数据量.把数据从接收寄存器全部copy到物理层缓冲区中.
//      2.如果物理层缓冲区满.置串口控制块状态字的溢出位,并发出uart错误事件.
//      3.发送串口接收事件.两个事件都是状态量事件
//      如果是发送信号引发的中断:
//      1.检查发送fifo有多少空位
//      2.从设备右手接口读出适量数据，
//      3.把读出的数据写入发送fifo
//      如果是出错信号引发中断:弹出串口出错事件
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
//-----------------------------------------------------------------------------
u32 UART1_Int(ptu32_t uart_int_line)
{
   u32 recv_trans,num;
    u32 fifo_reg;
    u8 ch[UART_TX_FIFO_SIZE];
    struct UartGeneralCB *UCB;
    tagUartReg *Reg;

    UCB = pUartCB[CN_UART1];
    if(UCB == NULL)
        return 0;
    Reg = (tagUartReg *)CN_UART1_BASE;

    if(pg_int_reg->SUBSRCPND & BIT_SUB_RXD1)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_RXD1;
        fifo_reg = Reg->UFSTAT;
        recv_trans = fifo_reg & 0x3f;
        for(num = 0; num < recv_trans; num++)
        //fifo缓冲区有数据,直到处理完成
            ch[num] = Reg->URXH;
        UART_PortWrite(UCB,ch,recv_trans);
        if( fifo_reg & cn_rx_over)
        {
            Reg->UFCON |= cn_rx_fifo_reset;
            UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
       }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_TXD1)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_TXD1;
        fifo_reg = Reg->UFSTAT;
        recv_trans = UART_TX_FIFO_SIZE-((fifo_reg>>8) & 0x3f);
        recv_trans = UART_PortRead(UCB,ch,recv_trans);
        for(num = 0; num < recv_trans; num++)
        {
            Reg->UTXH = ch[num];
        }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_ERR1)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_ERR1;
        UART_ErrHandle(UCB,CN_UART_HARD_COMM_ERR);
    }
    return 0;
}

//----uart2中断---------------------------------------------------------------
//功能: 如果是接收信号触发的中断:
//      1.检查接收fifo中的数据量.把数据从接收寄存器全部copy到物理层缓冲区中.
//      2.如果物理层缓冲区满.置串口控制块状态字的溢出位,并发出uart错误事件.
//      3.发送串口接收事件.两个事件都是状态量事件
//      如果是发送信号引发的中断:
//      1.检查发送fifo有多少空位
//      2.从设备右手接口读出适量数据，
//      3.把读出的数据写入发送fifo
//      如果是出错信号引发中断:弹出串口出错事件
//参数: 中断函数没有参数.
//返回: 中断函数没有返回值.
//-----------------------------------------------------------------------------
u32 UART2_Int(ptu32_t uart_int_line)
{
    u32 recv_trans,num;
    u32 fifo_reg;
    u8 ch[UART_TX_FIFO_SIZE];
    struct UartGeneralCB *UCB;
    tagUartReg *Reg;

    UCB = pUartCB[CN_UART2];
    if(UCB == NULL)
        return 0;
    Reg = (tagUartReg *)CN_UART2_BASE;

    if(pg_int_reg->SUBSRCPND & BIT_SUB_RXD2)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_RXD2;
        fifo_reg = Reg->UFSTAT;
        recv_trans = fifo_reg & 0x3f;
        for(num = 0; num < recv_trans; num++)
        //fifo缓冲区有数据,直到处理完成
            ch[num]= Reg->URXH;
        UART_PortWrite(UCB,ch,recv_trans);
        if( fifo_reg & cn_rx_over)
        {
            Reg->UFCON |= cn_rx_fifo_reset;
            UART_ErrHandle(UCB,CN_UART_FIFO_OVER_ERR);
       }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_TXD2)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_TXD2;
        fifo_reg = Reg->UFSTAT;
        recv_trans = UART_TX_FIFO_SIZE-((fifo_reg>>8) & 0x3f);
        recv_trans = UART_PortRead(UCB,ch,recv_trans);
        for(num = 0; num < recv_trans; num++)
        {
            Reg->UTXH = ch[num];
        }
    }
    if(pg_int_reg->SUBSRCPND & BIT_SUB_ERR2)
    {
        pg_int_reg->SUBSRCPND = BIT_SUB_ERR2;
        UART_ErrHandle(UCB,CN_UART_HARD_COMM_ERR);
    }
    return 0;
}

//----初始化uart0模块----------------------------------------------------------
//功能：初始化uart0模块
//参数：模块初始化函数没有参数
//返回：true = 成功初始化，false = 初始化失败
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_UART(ptu32_t serial_no)
{
    struct Object *UCB;
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
//        UART_Param.DirectlySend = (UartDirectSend)__UART_SendDirectly;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART1://串口1
        UART_Param.Name         = "UART1";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART1_BASE;
        UART_Param.TxRingBufLen = CFG_UART1_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART1_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
//        UART_Param.DirectlySend = (UartDirectSend)__UART_SendDirectly;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    case CN_UART2://串口2
        UART_Param.Name         = "UART2";
        UART_Param.Baud         = 115200;
        UART_Param.UartPortTag  = CN_UART2_BASE;
        UART_Param.TxRingBufLen = CFG_UART2_SENDBUF_LEN;
        UART_Param.RxRingBufLen = CFG_UART2_RECVBUF_LEN;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
//        UART_Param.DirectlySend = (UartDirectSend)__UART_SendDirectly;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;
        break;
    default:
        return 0;
    }

    //硬件初始化
    __UART_GPIOConfig(serial_no);
    __UART_DefaultConfig((tagUartReg *)UART_Param.UartPortTag);
    __UART_IntConfig(serial_no);

    pUartCB[serial_no] = UART_InstallGeneral(&UART_Param);
    if( pUartCB[serial_no] == NULL)
        return 0;
    else
        return 1;

}

// =============================================================================

#include <stdio.h>

static tagUartReg *reg_stdio=NULL;
static int port_stdio=0;

// =============================================================================
// 功能：字符终端直接发送函数，采用轮询方式，直接写寄存器，用于printk，或者stdout
//       没有初始化
// 参数：str，发送字符串指针
//      len，发送的字节数
// 返回：0，发生错误；result,发送数据长度，字节单位
// =============================================================================
s32 Uart_PutStrDirect(const char *str,u32 len)
{
    u32 result = 0,port,timeout = 100*mS;
    struct Object *UCB = NULL;
    tagUartReg *Reg;
    u32 BaseAddr;

    if(reg_stdio==NULL)
    {
        return 0;
    }

    Reg = (struct UART_REG *)reg_stdio;

    __UART_SubIntDisable(port,SUB_TXD_FLG);
    for(result=0; result < len; result ++)
    {
        // 超时或者发送缓冲为空时退出
        while((false == __UART_TxTranEmpty(Reg))&& (timeout > 0))
        {
            timeout--;
            DJY_DelayUs(1);
        }
        if(timeout == 0)
            break;
        Reg->UTXH = str[result];
    }
    __UART_SubIntEnable(port,SUB_TXD_FLG);
    return result;
}

// =============================================================================
// 功能：字符终端直接接收函数，采用轮询方式，直接读寄存器，用于stdin初始化前
// 返回：数据
// =============================================================================
char Uart_GetCharDirect(void)
{
    while(!(reg_stdio->UTRSTAT & 0x1));
    return reg_stdio->URXH;
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
        reg_stdio = (struct UART_REG*)CN_UART0_BASE;
        port_stdio = CN_UART0;
    }
    else if(!strcmp(StdioOut,"/dev/UART1"))
    {
        reg_stdio = (struct UART_REG*)CN_UART1_BASE;
        port_stdio = CN_UART1;
    }
    else if(!strcmp(StdioOut,"/dev/UART2"))
    {
        reg_stdio = (struct UART_REG*)CN_UART2_BASE;
        port_stdio = CN_UART2;
    }
    else
    {
        reg_stdio = NULL ;
        port_stdio =0xFFFF;
    }

    if(reg_stdio != NULL)
    {
        __UART_GPIOConfig(port_stdio);
        __UART_IntConfig(port_stdio);

        __UART_DefaultConfig(reg_stdio);
        __UART_BaudSet(reg_stdio,115200);

        PutStrDirect = Uart_PutStrDirect;
        GetCharDirect = Uart_GetCharDirect;
    }

}
