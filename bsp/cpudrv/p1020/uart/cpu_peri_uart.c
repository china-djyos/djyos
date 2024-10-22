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
//Copyright (C), 2013-2020,  都江堰操作系统开发团队
//版权所有 (C), 2013-2020,   都江堰操作系统开发团队
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
#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include <device/djy_uart.h>
#include "cpu_peri_uart.h"

#include "os.h"
#include "int_hard.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern ptu32_t ModuleInstall_UART(ptu32_t SerialNo);
//    #if CFG_UART0_ENABLE ==1
//    ModuleInstall_UART(CN_UART0);
//    #endif
//
//    #if CFG_UART1_ENABLE ==1
//    ModuleInstall_UART(CN_UART1);
//    #endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip uart"//CPU的uart外设驱动
//parent:"uart device file"      //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                         //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                      //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                        //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                      //初始化时机，可选值：early，medium，later, pre-main。
                                        //表示初始化时间，分别是早期、中期、后期
//dependence:"uart device file","device file system","heap"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                        //选中该组件时，被依赖组件将强制选中，
                                        //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"                 //该组件的弱依赖组件名（可以是none，表示无依赖组件），
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
//%$#@num,32,2048,
#define CFG_UART0_SENDBUF_LEN            2048                 //"UART0发送环形缓冲区大小",
#define CFG_UART0_RECVBUF_LEN            2048                 //"UART0接收环形缓冲区大小",

#define CFG_UART1_SENDBUF_LEN            2048                 //"UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN            2048                 //"UART1接收环形缓冲区大小",
//%$#@enum,true,false,
#define CFG_UART0_ENABLE                 true                 //"是否配置使用UART0",
#define CFG_UART1_ENABLE                 false                //"是否配置使用UART1",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

#define CN_UART0_BASE   (cn_uart_baddr)
#define CN_UART1_BASE   (cn_uart_baddr + 0x100)

//#define UART0_SendBufLen  2048
//#define UART0_RecvBufLen  2048

//#define UART1_SendBufLen  2048
//#define UART1_RecvBufLen  2048

static struct Object *pUartCB[CN_UART_NUM];

//----使能接收中断-------------------------------------------------------------
//功能: 使能uart的接收中断。
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __UART_RxIntEnable(tagUartReg volatile *reg)
{
    reg->ULCR &= ~(0x80);                //配置ULCR[DLAB]=0
    reg->UDMB_IER |= (u8)0x01;
}

//----禁止接收中断-------------------------------------------------------------
//功能: 禁止uart的接收中断。
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __UART_RxIntDisable(tagUartReg volatile *reg)
{
    reg->ULCR &= ~(0x80);                //配置ULCR[DLAB]=0
    reg->UDMB_IER &= ~(u8)0x01;
}

//----使能发送中断-------------------------------------------------------------
//功能: 使能uart的发送中断。
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __UART_TxIntEnable(tagUartReg volatile *reg)
{
    reg->ULCR &= ~(0x80);                //配置ULCR[DLAB]=0
    reg->UDMB_IER |= (u8)0x02;
}

//----禁止发送中断-------------------------------------------------------------
//功能: 禁止uart的发送中断。
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __UART_TxIntDisable(tagUartReg volatile *reg)
{
    reg->ULCR &= ~(0x80);                //配置ULCR[DLAB]=0
    reg->UDMB_IER &= ~(u8)0x02;
}

//----设置baud-----------------------------------------------------------------
//功能: 设置串口baud
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __UART_BaudSet(tagUartReg volatile *reg,u32 baud)
{
    u32 temp;

    reg->ULCR |= 0x80; //设置波特率时，设置DLB和DMB需配置ULCR[DLAB]=1
    temp = (u32)(cfg_core_ccb_clk)/(baud*16);
    reg->URBR_THR_DLB = (u8)temp;
    reg->UDMB_IER = (u8)(temp>>8);
    reg->ULCR &= ~(0x80);                //配置ULCR[DLAB]=0，恢复默认
}
//----设置硬件参数-------------------------------------------------------------
//功能: 设置硬件参数，包括波特率、停止位、校验位、数据位
//参数: reg,被操作的寄存器组指针
//返回: 无
//-----------------------------------------------------------------------------
void __UART_DefaultSet(tagUartReg volatile *reg)
{
    __UART_BaudSet(reg,115200);            //波特率设置
    reg->UIIR_FCR_AFR;                         //读一下清除所有中断标志
    reg->UIIR_FCR_AFR |= 0x01;              //配置使用FIFO
//    reg->UIIR_FCR_AFR   |= 0x08;            //DMA模式选择为1
    reg->UIIR_FCR_AFR |= 0xc6;              //接收14字节触发中断，清收发fifo
    reg->ULCR = 0x03 | (0<<2) | (000<<3);   //数据8bit、停止位=1、无校验
    reg->UMCR = 0;
    reg->UDMB_IER =  0x03;//使能收发中断，此时中断线控制尚未使能
}
//----检查串口发送寄存器是否空-------------------------------------------------
//功能: 检查发送寄存器的状态，如果空就返回true
//参数：无。
//返回: true = 缓冲区空，false = 非空
//-----------------------------------------------------------------------------
bool_t __UART_TxTranEmpty(volatile tagUartReg *reg)
{
    return (0 != (reg->ULSR & 0x20));
}

//----串口中断系统初始化--------------------------------------------------------
//功能: 初始化中断系统，两个串口共用一个中断线
//参数：串口号
//返回: 无
//-----------------------------------------------------------------------------
extern uint32_t UART_ISR(ptu32_t IntLine);

void __UART_IntInit(ptu32_t SerialNo)
{
    u8 IntLine;
    IntLine = cn_int_line_duart;
    //中断线的初始化
    Int_Register(IntLine);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_SetLineTrigerType(IntLine,EN_INT_TRIGER_HIGHLEVEL);//set the uart active high
    Int_ClearLine(IntLine);     //清掉初始化产生的发送fifo空的中断
    Int_RestoreAsynLine(IntLine);
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
    if(Reg == NULL)
        return 0;

    switch(cmd)
    {
        case CN_DEV_CTRL_START:
            __UART_RxIntEnable(Reg);
            __UART_TxIntEnable(Reg);
            break;
        case CN_DEV_CTRL_STOP:
            __UART_RxIntDisable(Reg);
            __UART_TxIntDisable(Reg);
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
            __UART_RxIntDisable(Reg);
            break;
        case CN_UART_RX_RESUME:      //恢复接收
            __UART_RxIntEnable(Reg);
            break;
        case CN_UART_RECV_HARD_LEVEL:    //设置接收fifo触发水平
        {
            (Reg)->UIIR_FCR_AFR &= 0x3f;
            switch (data1)
            {
                case 1:
                    (Reg)->UIIR_FCR_AFR |= 0x00;//1字节触发
                    break;
                case 4:
                    (Reg)->UIIR_FCR_AFR |= 0x10;//4字节触发
                    break;
                case 8:
                    (Reg)->UIIR_FCR_AFR |= 0x20;//8字节触发
                    break;
                case 14:
                    (Reg)->UIIR_FCR_AFR |= 0x30;//14字节触发
                    break;
                default :break;
            }
            break;
        }
        case CN_UART_COM_SET:
            break;
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
//u32 __UART_SendDirectly(tagUartReg *Reg,u8 *send_buf,u32 len,u32 timeout)
//{
//    u32  result;
//
//    if(Reg == NULL)
//        return 0;
//    __UART_TxIntDisable(Reg);
//    for(result=0; result < len; result ++)
//    {
//        while((false == __UART_TxTranEmpty(Reg))
//            && (timeout > 0))//超时或者发送缓冲为空时退出
//        {
//            timeout--;
//            DJY_DelayUs(1);
//        }
//        if(timeout == 0)
//            break;
//        Reg->URBR_THR_DLB = send_buf[result];
//    }
//    __UART_TxIntEnable(Reg);
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
    u8 trans,num,ch[16],port;

    if((u32)Reg == CN_UART0_BASE)
        port = CN_UART0;
    else if((u32)Reg == CN_UART1_BASE)
        port = CN_UART1;
    else
        return 0;

    __UART_TxIntDisable(Reg);
    if(__UART_TxTranEmpty(Reg))
    {
        trans = UART_PortRead(pUartCB[port],ch,16);
        for(num = 0; num < trans; num++)
        {
            (Reg)->URBR_THR_DLB = ch[num];
        }
    }
    __UART_TxIntEnable(Reg);
    return trans;
}
//----uart中断---------------------------------------------------------------
//功能: 接收，读取fifo中的数据并从右手写入设备
//      发送，从右手读数据并发送
//参数: 中断号.
//返回: 0.
//-----------------------------------------------------------------------------
uint32_t UART_ISR(ptu32_t IntLine)
{
    struct Object *UCB = NULL;
    tagUartReg *Reg;
    uint32_t recv_trans,num;
    uint8_t ch[20],IIR=0;

    Reg =  (tagUartReg *)CN_UART0_BASE;
    IIR = Reg->UIIR_FCR_AFR;
    UCB = pUartCB[CN_UART0];
    if(IIR & 0x01)//uart0无中断
    {
        Reg =  (tagUartReg *)CN_UART1_BASE;
        IIR = Reg->UIIR_FCR_AFR;
        if(IIR & 0x01)//uart1无中断
            return 0;
        UCB = pUartCB[CN_UART1];
    }
    IIR = IIR &0x7;
    switch(IIR)
    {
        case 2:     //UTHR寄存器为空
        {
            recv_trans = UART_PortRead(UCB,ch,16);
            for(num = 0; num < recv_trans; num++)
            {
                Reg->URBR_THR_DLB = ch[num];
            }
        }break;
        case 4:     //接收到有效数据或使能FIFO时，接收触发水平到
        case 12:     //接收超时，fifo中至少有一个数据，连续4字符时间无新数据
        {
            for(num = 0; num<20; num++)
            {
                if(Reg->ULSR & 0x01)    //判断ULSR的DR位，判断是否还有数据可读
                    ch[num] = Reg->URBR_THR_DLB;
                else
                    break;
            }
            UART_PortWrite(UCB,ch,num);
        }break;
        case 6:     //出错标志，停止位错误、帧错误、溢出错误等
        {
            IIR = Reg->ULSR;//清中断标志
        }break;
        default:
            break;
    }
    return 0;
}

//----初始化uart1模块----------------------------------------------------------
//功能：初始化uart1模块,并装载串口设备
//参数：串口初始化模块的参数为1、2、3...串口号
//返回：1=成功，0=失败
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_UART(ptu32_t serial_no)
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
    default:
        return 0;
    }

    //硬件初始化
    __UART_DefaultSet((tagUartReg *)UART_Param.UartPortTag);
    __UART_IntInit(serial_no);

    pUartCB[serial_no] = UART_InstallPort(&UART_Param);
    if( pUartCB[serial_no] == NULL)
        return 0;
    else
        return 1;
}

static tagUartReg  *PutStrDirectReg = NULL;
static tagUartReg  *GetCharDirectReg = NULL;

//----串行发送服务（直接写终端）-------------------------------------------------
//功能: 这个是直接写串口函数，不会经过事件弹出
//参数: 所需要发送的字符串，当然，前提是你提供的一定是字符串'\0'结束
//返回: 发送的个数
//-----------------------------------------------------------------------------
s32 Uart_PutStrDirect(const char *str,u32 len)
{
    u32  result,timeout;
    tagUartReg *Reg;

    Reg = PutStrDirectReg;
    __UART_TxIntDisable(Reg);
    for(result=0; result < len; result ++)
    {
        timeout = 0;
        while((false == __UART_TxTranEmpty(Reg))
            && (timeout++ < 10000));//超时或者发送缓冲为空时退出
        if(timeout >= 10000)
            break;
        Reg->URBR_THR_DLB = ((uint8_t*)str)[result];
    }
    __UART_TxIntEnable(Reg);
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
    char result = '\n';


    return result;
}

//----初始化内核级IO-----------------------------------------------------------
//功能：初始化内核级输入和输出所需的runtime函数指针。
//参数：无
//返回：无
//-----------------------------------------------------------------------------
void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut)
{
    u32 TxDirectPort = 0;
    u32 RxDirectPort = 0;

    if(!strcmp(StdioOut,"/dev/UART0"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART0_BASE;
        TxDirectPort = CN_UART0;
    }
    else if(!strcmp(StdioOut,"/dev/UART1"))
    {
        PutStrDirectReg = (tagUartReg*)CN_UART1_BASE;
        TxDirectPort = CN_UART1;
    }
    else
    {
        PutStrDirectReg = NULL ;
    }

    if(PutStrDirectReg != NULL)
    {
        __UART_DefaultSet(PutStrDirectReg);
        PutStrDirect = Uart_PutStrDirect;
    }

    if(!strcmp(StdioIn,"/dev/UART0"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART0_BASE;
        RxDirectPort = CN_UART0;
    }
    else if(!strcmp(StdioIn,"/dev/UART1"))
    {
        GetCharDirectReg = (tagUartReg*)CN_UART1_BASE;
        RxDirectPort = CN_UART1;
    }
    else
    {
        GetCharDirectReg = NULL ;
    }
    if(GetCharDirectReg != NULL)
    {
        if(TxDirectPort != RxDirectPort)
            __UART_DefaultSet(GetCharDirectReg);
        GetCharDirect = Uart_GetCharDirect;
    }
    return;
}



