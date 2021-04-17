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
// =============================================================================

// 文件名     ：cpu_peri_uart.c
// 模块描述: DJYOS串口模块的底层驱动部分，主要实现寄存器级别的操作，如中断等
// 模块版本: V1.10
// 创建人员:
// 创建时间:
// =============================================================================
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "djyos.h"
#include "int.h"
#include "systime.h"
#include "cpu_peri.h"
#include "uartctrl.h"
#include <device/djy_uart.h>
#include <silan_syscfg.h>
#include <silan_uart.h>
#include "cpu_peri_isr.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern ptu32_t ModuleInstall_UART(ptu32_t SerialNo);
//
//    #if CFG_UART1_ENABLE ==1
//    ModuleInstall_UART(CN_UART1);
//    #endif
//
//    #if CFG_UART2_ENABLE ==1
//    ModuleInstall_UART(CN_UART2);
//    #endif
//
//    #if CFG_UART3_ENABLE ==1
//    ModuleInstall_UART(CN_UART3);
//    #endif
//
//    #if CFG_UART4_ENABLE ==1
//    ModuleInstall_UART(CN_UART4);
//    #endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip uart"//CPU的uart外设驱动
//parent:"uart device file"      //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","lock","uart device file","uart device fileheap","cpu onchip dma"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
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
//%$#@num,16,512,
#define CFG_UART1_SENDBUF_LEN            32                 //"UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN            32                 //"UART1接收环形缓冲区大小",
#define CFG_UART1_DMABUF_LEN             32                 //"UART1 DMA缓冲区大小",

#define CFG_UART2_SENDBUF_LEN            32                 //"UART2发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN            32                 //"UART2接收环形缓冲区大小",
#define CFG_UART2_DMABUF_LEN             32                 //"UART2 DMA缓冲区大小",

#define CFG_UART3_SENDBUF_LEN            32                 //"UART3发送环形缓冲区大小",
#define CFG_UART3_RECVBUF_LEN            32                 //"UART3接收环形缓冲区大小",
#define CFG_UART3_DMABUF_LEN             32                 //"UART3 DMA缓冲区大小",

#define CFG_UART4_SENDBUF_LEN            32                 //"UART4发送环形缓冲区大小",
#define CFG_UART4_RECVBUF_LEN            32                 //"UART4接收环形缓冲区大小",
#define CFG_UART4_DMABUF_LEN             32                 //"UART4 DMA缓冲区大小",
//%$#@enum,true,false,
#define CFG_UART1_ENABLE                 false               //"是否使用UART1",
#define CFG_UART2_ENABLE                 true              //"是否使用UART2",
#define CFG_UART3_ENABLE                 false              //"是否使用UART3",
#define CFG_UART4_ENABLE                 false              //"是否使用UART4",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

typedef struct __UART_REG
{
    vu32 DR;
    vu32 RSR_ECR;
    vu32 REV0[4];
    vu32 FR;
    vu32 REV1;
    vu32 ILPR;
    vu32 IBRD;
    vu32 FBRD;
    vu32 LCR_H;
    vu32 CR;
    vu32 IFLS;
    vu32 IMSC;
    vu32 RIS;
    vu32 MIS;
    vu32 ICR;
    vu32 DMACR;
}tagUartReg;

#define UART_INTR_EN_MCU4    (0x42030000 + 0x610)
#define UART_INTR_STS_MCU4   (0x42030000 + 0x510)

extern const char *gc_pCfgStdinName;    //标准输入设备
extern const char *gc_pCfgStdoutName;   //标准输出设备
//extern const char *gc_pCfgStderrName;   //标准错误输出设备

// =============================================================================
static const tagUartReg volatile *sUartReg[CN_UART_NUM] = {
                                            (tagUartReg volatile *)0X41000000,
                                            (tagUartReg volatile *)0X41010000,
                                            (tagUartReg volatile *)0X41020000,
                                            (tagUartReg volatile *)0X41030000,
};
static const char *sUartName[CN_UART_NUM] = {
        "UART1","UART2","UART3","UART4"
};
static const u8 sUartFifo[]={
        64,16,16,16
};
// =============================================================================

u16 UART_SndBufLen;
u16 UART_RxBufLen;

static u32 TxByteTime;                    //正常发送一个字节所需要的时间
static u32 TxDirectPort;                  //用于printk发送的串口号
static u32 RxDirectPort;                  //用于直接接收的串口号
// =============================================================================

static struct UartCB *pUartCB[CN_UART_NUM];

// =============================================================================
static ptu32_t UART_ISR(ptu32_t IniLine);
__attribute__((weak)) void Board_UartHalfDuplexSend(u8 port)
{
    //user added if need out of the file
}
__attribute__((weak)) void Board_UartHalfDuplexRecv(u8 port)
{
    //user added if need out of the file
}

static void __UART_Enable(tagUartReg *Reg,u8 Enable)
{
    if(Enable)
    {
        Reg->CR |= (1<<0);
    }
    else
    {
        Reg->CR &= ~(1<<0);
    }
}
static void __UART_RxEnable(tagUartReg *Reg,u8 Enable)
{
    if(Enable)
    {
        Reg->CR |= (1<<9);
    }
    else
    {
        Reg->CR &= ~(1<<9);
    }
}
static void __UART_TxEnable(tagUartReg *Reg,u8 Enable)
{
    if(Enable)
    {
        Reg->CR |= (1<<8);
    }
    else
    {
        Reg->CR &= ~(1<<8);
    }
}

static void __UART_TxIntEnable(tagUartReg *Reg,u8 Enable)
{
    if(Enable)
    {
        Reg->IMSC |= (1<<5);
    }
    else
    {
        Reg->IMSC &= ~(1<<5);
    }
}

static void __UART_RxIntEnable(tagUartReg *Reg,u8 Enable)
{
    if(Enable)
    {
        Reg->IMSC |= (1<<4)|(1<<6);
    }
    else
    {
        Reg->IMSC &= ~((1<<4)|(1<<6));
    }
}
// =============================================================================
// 功能: 设置串口baud,PCLK为25M，CPU主频为216M计算,该驱动只提供9600、19200、57600、
//       115200四种波特率设置
// 参数: Reg,被操作的寄存器组指针
//       port,串口号
//       baud,波特率
// 返回: 无
// =============================================================================
static void __UART_BaudSet(tagUartReg *Reg,u8 port,u32 baudrate)
{
    u32 temp, remainder, divider, fraction;
    u32 uart_clk;

    __UART_Enable(Reg,0);
    uart_clk = get_sysclk_val_settled();
    temp = 16 * baudrate;
    divider = uart_clk / temp;
    remainder = uart_clk % temp;
    temp = (8 * remainder) / baudrate;
    fraction = (temp >> 1) + (temp & 1);
    Reg->IBRD = divider;
    Reg->FBRD = fraction;
    if(TxDirectPort == port)
    {
        TxByteTime = 11000000/baudrate;     //1个字节传输时间，按10bit，+10%计算
    }
    __UART_Enable(Reg,1);
}
// =============================================================================
// 功能: 有关串口参数的初始化，波特率，停止位，奇偶校验位
// 参数：port,串口号
//      data，参数
// 返回: 无
// =============================================================================
static void __UART_ComConfig(tagUartReg volatile *Reg,u32 port,struct COMParam *COM)
{
    struct COMParam *COM;
    if((COM == NULL) || (Reg == NULL))
        return;
    __UART_BaudSet(Reg,port,COM->BaudRate);

    __UART_Enable(Reg,0);
    switch(COM->DataBits)               // data bits
    {
        Reg->LCR_H &= ~(0x03<<5);
        case CN_UART_DATABITS_5:
            Reg->LCR_H |= (0<<5);
            break;
        case CN_UART_DATABITS_6:
            Reg->LCR_H |= (1<<5);
            break;
        case CN_UART_DATABITS_7:
            Reg->LCR_H |= (2<<5);
            break;
        case CN_UART_DATABITS_8:
            Reg->LCR_H |= (3<<5);
            break;
        default:
            Reg->LCR_H |= (3<<5);
            break;
    }

    switch(COM->Parity)        //
    {
        case CN_UART_PARITY_NONE:
            Reg->LCR_H &= ~(1<<1);
            break;
        case CN_UART_PARITY_ODD:
            Reg->LCR_H |= (1<<1);
            break;
        case CN_UART_PARITY_EVEN:
            Reg->LCR_H |= (3<<1);
            break;
        default:break;
    }

    switch(COM->StopBits)  //todu:F7的 0.5个停止位没有写进来
    {
        case CN_UART_STOPBITS_1:
            Reg->LCR_H &= ~(1<<3);
            break;
        case CN_UART_STOPBITS_2:
            Reg->LCR_H |= (1<<3);
            break;
        default:break;
    }
    __UART_Enable(Reg,1);
}

static bool_t __UART_TxFIFO_Empty(tagUartReg *Reg)
{
    return (bool_t)(Reg->FR & UART_FR_TXFE);
}

// =============================================================================
// 功能: 初始化UART对应的中断线，并初始化中断入口函数
// 参数：SerialNo,串口号
// 返回: 无
// =============================================================================
static void __UART_IntInit(u32 port)
{
    u8 IntLine;
    u8 subIntLine;

    switch(port)
    {
        case CN_UART1:
             subIntLine  = CN_SUBID_MISC_UART1;
             break;
        case CN_UART2:
             subIntLine  = CN_SUBID_MISC_UART2;
             break;
        case CN_UART3:
             subIntLine  = CN_SUBID_MISC_UART3;
             break;
        case CN_UART4:
             subIntLine  = CN_SUBID_MISC_UART4;
             break;
        default:subIntLine  = CN_SUBID_MISC_UART2;break;
    }

    IntLine = CN_INT_LINE_UART;

    *(u32*)UART_INTR_EN_MCU4 |= (1<<port);

    djybsp_isr_hdl_register(IntLine,subIntLine,UART_ISR,0);

}
// =============================================================================
// 功能: 硬件参数配置和寄存器的初始化，包括波特率、停止位、校验位、数据位，默认情况下:
//       波特率:115200  ； 停止位:1 ; 校验:无 ; 数据位:8bit
// 参数: SerialNo,串口号，0~3
// 返回: 无
// =============================================================================
static void __UART_HardInit(u8 port)
{
    if(port < CN_UART_NUM)
    {
        silan_module_clkon(CLK_UART1+port, CLK_ON);
        silan_uart_sys_init(UART_1+port, BAUDRATE_115200);

        __UART_RxIntEnable(sUartReg[port],1);   //rx int enable
        __UART_RxEnable(sUartReg[port],1);
        __UART_Enable(sUartReg[port],1);
    }
}

// =============================================================================
// 功能: 启动串口发送，包括两种情况，使用DMA或不使用DMA
// 参数: Reg,被操作的串口寄存器指针.
// 返回: 发送的个数
// =============================================================================
static u32 __UART_SendStart (tagUartReg *Reg)
{
    u8 len=0,i,port;
    u8 fifo[64];

    for(port = CN_UART1;port < CN_UART_NUM;port ++)
    {
        if(Reg == sUartReg[port])
            break;
    }
    if(port < CN_UART_NUM)
    {
        if(__UART_TxFIFO_Empty(Reg))
        {
            __UART_TxIntEnable(Reg,0);
            len = UART_PortRead(pUartCB[port],fifo,sUartFifo[port]);
            for(i = 0; i < len; i++)
            {
                Reg->DR = fifo[i];
            }

            __UART_TxIntEnable(Reg,1);
            __UART_TxEnable(Reg,1);
        }
    }
    return len;
}

// =============================================================================
// 功能: 串口设备的控制函数,与具体的硬件寄存器设置相关
// 参数: Reg,被操作的串口寄存器指针.
//       cmd,操作类型
//       data1,data2,含义依cmd而定
// 返回: 无意义.
// =============================================================================
static ptu32_t __UART_Ctrl(tagUartReg *Reg,u32 cmd,va_list *arg0)
{
    ptu32_t result = 0;
    u32 port;
    s32 timeout = 100000;
    for(port = CN_UART1;port < CN_UART_NUM;port ++)
    {
        if(Reg == sUartReg[port])
            break;
    }
    if(port < CN_UART_NUM)
    {
        switch(cmd)
        {
//        case CN_UART_START:
//          __UART_Enable(Reg,1);
//          break;
//        case CN_UART_STOP:
//          __UART_Enable(Reg,0);
//            break;
        case CN_UART_SET_BAUD:
        {
            u32 data;
            data = va_arg(*arg0, u32);
            __UART_BaudSet(Reg,port, data);
        }
            break;
        case CN_UART_COM_SET:
        {
            struct COMParam *COM;
            COM = va_arg(*arg0, void *);
            __UART_ComConfig(Reg,port,COM);
        }
            break;
        case CN_UART_HALF_DUPLEX_SEND: //发送数据
            Board_UartHalfDuplexSend(port);
            break;
        case CN_UART_HALF_DUPLEX_RECV: //接收数据
            while(__UART_TxFIFO_Empty(Reg) && (timeout > 10))
            {
                timeout -=10;
                DJY_DelayUs(10);
            }
            Board_UartHalfDuplexRecv(port);
            break;
        default:
            break;
        }
    }
    return result;
}

// =============================================================================
// 功能：UART中断,若为idle中断，则从DMA缓冲区中读出数据，并重新启动DMA，否则调用HAL中断
//       处理函数，最终会调用到HAL_UART_XXXXCallback（）
// 参数：串口号
// 返回：1=成功，0=失败
// =============================================================================
u32 UART_ISR(ptu32_t port)
{
    u32 num,status;
    tagUartReg volatile *Reg;
    u8 fifo[64];

//    status = (*(u32*)UART_INTR_STS_MCU4);
//
//    for(port = 0; port < CN_UART4;port++)
//    {
//      if(status & (1<<port))
//      {
//          break;
//      }
//    }

    if(port < CN_UART_NUM)
    {
        Reg = sUartReg[port];
        if( (Reg->RIS & (1<<4)) || (Reg->RIS & (1 << 6)))   //rx or idle int
        {
            Reg->ICR |= (1<<4)|(1<<6);
            num = 0;
            while(!(Reg->FR & UART_FR_RXFE))
            {
                if(num<sUartFifo[port])
                {
                    fifo[num++] = (u8)(Reg->DR);
                }
                else
                {
                    break;
                }
            }
            if(num > 0)
            {
                if(num != UART_PortWrite(pUartCB[port],fifo,num))
                {
                    UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
//                  printk("uart idle over!\r\n");
                }
            }
        }

        if(Reg->RIS & (1<<5))                   //tx int
        {
            Reg->ICR |= (1<<5);
            num = 0;
            while(!(Reg->FR & UART_FR_TXFF))
            {
                if(0 != UART_PortRead(pUartCB[port],&fifo[0],1))
                {
                    Reg->DR = fifo[0];
                }
                else
                {
                    break;
                }
            }
        }

        if(Reg->RIS & (0xF << 7))   //hard error int
        {
            if(Reg->RIS & (1<<7))   //frame error
            {
                printf("uart frame error!\r\n");
                Reg->ICR |= (1<<7);
            }
            if(Reg->RIS & (1<<8))   //parity error
            {
                printf("uart parity error!\r\n");
                Reg->ICR |= (1<<8);
            }
            if(Reg->RIS & (1<<9))   //BE
            {
                printf("uart be error!\r\n");
                Reg->ICR |= (1<<9);
            }
            if(Reg->RIS & (1<<10))  //over error
            {
                printf("uart over error!\r\n");
                Reg->ICR |= (1<<10);
            }
        }
    }
    return 1;
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
ptu32_t ModuleInstall_UART(u32 port)
{
    bool_t Ret = false;
    struct UartParam UART_Param;

    switch(port)
    {
        case CN_UART1:
            UART_SndBufLen = CFG_UART1_SENDBUF_LEN;
            UART_RxBufLen  = CFG_UART1_RECVBUF_LEN;
            break;
        case CN_UART2:
            UART_SndBufLen = CFG_UART2_SENDBUF_LEN;
            UART_RxBufLen  = CFG_UART2_RECVBUF_LEN;
            break;
        case CN_UART3:
            UART_SndBufLen = CFG_UART3_SENDBUF_LEN;
            UART_RxBufLen  = CFG_UART3_RECVBUF_LEN;
            break;
        case CN_UART4:
            UART_SndBufLen = CFG_UART4_SENDBUF_LEN;
            UART_RxBufLen  = CFG_UART4_RECVBUF_LEN;
            break;
        deault:printk("ModuleInstall_UART:port Err\r\n");break;
    }

    if(port < CN_UART_NUM)
    {
        UART_Param.Name         = sUartName[port];
        UART_Param.UartPortTag  = (ptu32_t)sUartReg[port];
        UART_Param.Baud         = 115200;
        UART_Param.TxRingBufLen = UART_SndBufLen;
        UART_Param.RxRingBufLen = UART_RxBufLen;
        UART_Param.StartSend    = (UartStartSend)__UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;

        pUartCB[port] = UART_InstallGeneral(&UART_Param);
        if( pUartCB[port] != NULL)
        {
            __UART_HardInit(port);              //硬件初始化
            __UART_IntInit(port);
            Ret = true;
        }
    }
    return Ret;
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
    u32 timeout,i=0;
    tagUartReg *Reg;
    TxByteTime = 11000000/115200;

    timeout= TxByteTime * len;

    Reg = sUartReg[TxDirectPort];

    while(1)
    {
        if( !(Reg->FR & (1<<5)) )
        {
            Reg->DR = str[i++];
            if(i==len)
                break;
        }
        else
        {
//          DJY_EventDelay(1000);
            DJY_DelayUs(1000);
        }
    }

    return len;
}

// =============================================================================
// 功能：字符终端直接接收函数，采用轮询方式，直接读寄存器，用于stdin初始化前
// 参数：str，发送字符串指针
//      len，发送的字节数
// 返回：0，发生错误；result,发送数据长度，字节单位
// =============================================================================
char Uart_GetCharDirect(void)
{
    u8 result;
    tagUartReg *Reg;
    Reg = sUartReg[RxDirectPort];
    while(1)
    {
        if( !(Reg->FR & (1<<4)) )
        {
            result = Reg->DR;
            break;
        }
        printk("bsp uart\r\n");
        DJY_EventDelay(500);
    }

//  Reg->IMSC &= ~((1<<4)|(1<<6));
//  while(Reg->FR & (1<<4) == 0);
//  result = Reg->DR;
//  Reg->FR &= ~(1<<4);
//  Reg->IMSC |= (1<<4)|(1<<6);

    return result;
}

//----初始化内核级IO-----------------------------------------------------------
//功能：初始化内核级输入和输出所需的runtime函数指针。
//参数：无
//返回：无
//-----------------------------------------------------------------------------
void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut)
{
    if(!strcmp(StdioOut,"/dev/UART1"))
    {
        TxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioOut,"/dev/UART2"))
    {
        TxDirectPort = CN_UART2;
    }
    else if(!strcmp(StdioOut,"/dev/UART3"))
    {
        TxDirectPort = CN_UART3;
    }
    else if(!strcmp(StdioOut,"/dev/UART4"))
    {
        TxDirectPort = CN_UART4;
    }
    else
    {
        PutStrDirect = NULL ;
    }

    //if(PutStrDirect != NULL)
    if(TxDirectPort >= 0)
    {
        __UART_HardInit(TxDirectPort);
        TxByteTime = 95;      //初始化默认115200，发送一个byte是87uS,+10%容限
        PutStrDirect = Uart_PutStrDirect;
    }

    if(!strcmp(StdioIn,"/dev/UART1"))
    {
        RxDirectPort = CN_UART1;
    }
    else if(!strcmp(StdioIn,"/dev/UART2"))
    {
        RxDirectPort = CN_UART2;
    }
    else if(!strcmp(StdioIn,"/dev/UART3"))
    {
        RxDirectPort = CN_UART3;
    }
    else if(!strcmp(StdioIn,"/dev/UART4"))
    {
        RxDirectPort = CN_UART4;
    }
    else
    {
        GetCharDirect = NULL ;
    }

   // if(GetCharDirect != NULL)
    if(RxDirectPort >= 0)
    {
        if(TxDirectPort != RxDirectPort)
            __UART_HardInit(RxDirectPort);
        GetCharDirect = Uart_GetCharDirect;
    }
    return;
}

#ifdef CFG_CPU_ZQ12XX_CK
#include "shell.h"
typedef void (*Ex_shell_func)(void);
void __asm_ExecuteCmd(union param *ptab,
       enum param_typr *pflag,Ex_shell_func fun,u32 num)
{

}
#endif

