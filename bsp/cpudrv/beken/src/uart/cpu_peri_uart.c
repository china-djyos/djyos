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
#include "cpu_peri.h"
#include <device/djy_uart.h>
#include "stdlib.h"
#include "uart/uart.h"
#include <icu_pub.h>
#include "arm_arch.h"
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
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip uart"//CPU的uart外设驱动
//parent:"uart device file"      //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","lock","uart device file","heap"//该组件的依赖组件名（可以是none，表示无依赖组件），
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
//%$#@num,0,4096,
#define CFG_UART1_SENDBUF_LEN       256      //"UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN       256      //"UART1接收环形缓冲区大小",

#define CFG_UART2_SENDBUF_LEN       256      //"UART2发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN       256      //"UART2接收环形缓冲区大小",
//%$#@enum,true,false
#define CFG_UART1_ENABLE           true        //"是否使用UART1",
#define CFG_UART2_ENABLE           true       //"是否使用UART2",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
//@#$%component end configure
// =============================================================================

#define REG_READ(addr)          (*((volatile UINT32 *)(addr)))
#define REG_WRITE(addr, _data)  (*((volatile UINT32 *)(addr)) = (_data))

#if (CN_BEKEN_SDK_V3 == 1)
extern void uart_hw_set_change(UINT8 uport, bk_uart_config_t *uart_config);
static bk_uart_config_t djybsp_uart[CN_UART_NUM] = {
    {
        .baud_rate = UART_BAUDRATE_115200,
        .data_width = BK_DATA_WIDTH_8BIT,
        .parity = BK_PARITY_NO,
        .stop_bits = BK_STOP_BITS_1,
        .flow_control = FLOW_CTRL_DISABLED,
    },
    {
       .baud_rate = UART_BAUDRATE_115200,
       .data_width = BK_DATA_WIDTH_8BIT,
       .parity = BK_PARITY_NO,
       .stop_bits = BK_STOP_BITS_1,
       .flow_control = FLOW_CTRL_DISABLED,
    },
};
#else
extern void uart_hw_set_change(UINT8 uport, uart_config_t *uart_config);
static uart_config_t djybsp_uart[CN_UART_NUM] = {
    {
        .baud_rate = UART_BAUDRATE_115200,
        .data_width = DATA_WIDTH_8BIT,
        .parity = BK_PARITY_NO,
        .stop_bits = BK_STOP_BITS_1,
        .flow_control = FLOW_CTRL_DISABLED,
    },
    {
       .baud_rate = UART_BAUDRATE_115200,
       .data_width = DATA_WIDTH_8BIT,
       .parity = BK_PARITY_NO,
       .stop_bits = BK_STOP_BITS_1,
       .flow_control = FLOW_CTRL_DISABLED,
    },
};
#endif

static u16 UART_SndBufLen = 0;
static u16 UART_RxBufLen = 0;
static u8 TxDirectPort = CN_LIMIT_UINT8;        //用于printk发送的串口号
static u8 RxDirectPort = CN_LIMIT_UINT8;        //用于直接接收的串口号
static struct UartGeneralCB *pUartCB[CN_UART_NUM];
static const char *sUartName[CN_UART_NUM] = {
        "UART1","UART2",
};

static const uint8_t volatile *sUartReg[CN_UART_NUM] = {
                                            CN_UART1,
                                            CN_UART2,
};
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static ptu32_t __UART_Ctrl(uint8_t port,u32 cmd, va_list *arg0);

__attribute__((weak))  void Board_UartHalfDuplexSend(u8 SerialNo)
{
    return;
}
__attribute__((weak))  void Board_UartHalfDuplexRecv(u8 SerialNo)
{
    return ;
}

static void __UART_BaudSet(uint8_t port,uint32_t data)
{
    if((data == 0) || (port > CN_UART2))
        return;
    djybsp_uart[port].baud_rate = data;
        uart_hw_set_change(port,&djybsp_uart[port]);
}
// =============================================================================
// 功能: 关串口中断
// 参数: SerialNo,串口号，0,1
// 返回: 无
// =============================================================================
static void __UART_CloseInte(uint8_t SerialNo)
{
    u32 param;
    if(SerialNo == CN_UART1)
    {
        param = IRQ_UART1_BIT;
        sddev_control(ICU_DEV_NAME, CMD_ICU_INT_DISABLE, &param);
    }
    else if(SerialNo == CN_UART2)
    {
        param = IRQ_UART2_BIT;
        sddev_control(ICU_DEV_NAME, CMD_ICU_INT_DISABLE, &param);
    }
}

// =============================================================================
// 功能: 开串口中断
// 参数: SerialNo,串口号，0,1
// 返回: 无
// =============================================================================
static void __UART_OpenInte(uint8_t SerialNo)
{
    u32 param;
    if(SerialNo == CN_UART1)
    {
        param = IRQ_UART1_BIT;
        sddev_control(ICU_DEV_NAME, CMD_ICU_INT_ENABLE, &param);
    }
    else if(SerialNo == CN_UART2)
    {
        param = IRQ_UART2_BIT;
        sddev_control(ICU_DEV_NAME, CMD_ICU_INT_ENABLE, &param);
    }
}

// =============================================================================
// 功能: 硬件参数配置和寄存器的初始化，包括波特率、停止位、校验位、数据位，默认情况下:
//       波特率:115200  ； 停止位:1 ; 校验:无 ; 数据位:8bit
// 参数: SerialNo,串口号，0~3
// 返回: 无
// =============================================================================
static void __UART_HardInit(u8 SerialNo)
{
    switch(SerialNo)
    {
        case CN_UART1:
            uart1_init();
//            __UART_BaudSet(SerialNo, 115200);
            break;
        case CN_UART2:
            uart2_init();
            break;
    }
}

// =============================================================================
// 功能: 使串口停止工作，包括时钟和uart模块
// 参数: port,串口号
// 返回: 无
// =============================================================================
static void __UART_Disable(u32 port)
{
    switch(port)
    {
        case CN_UART1:  uart1_exit();  break;
        case CN_UART2:  uart2_exit(); break;
        default:break;
    }
}

// =============================================================================
// 功能: 对串口传输参数配置，包括波特率、奇偶校验、数据位、停止位
// 参数:  Reg,被操作的寄存器组指针
//        port,串口号
//        data,结构体tagCOMParam类型的指针数值
// 返回: 无
// =============================================================================
static void __UART_ComConfig(u32 port,struct COMParam *COM)
{
//  struct COMParam *COM;
    if((COM == 0) || (port > CN_UART2))
        return;
//    COM = (struct COMParam *)data;
    djybsp_uart[port].baud_rate = COM->BaudRate;

    switch(COM->DataBits)               // data bits
    {
#if (CN_BEKEN_SDK_V3 == 1)
        case CN_UART_DATABITS_7:
            djybsp_uart[port].data_width = BK_DATA_WIDTH_7BIT;
            break;

        case CN_UART_DATABITS_8:
            djybsp_uart[port].data_width = BK_DATA_WIDTH_8BIT;
            break;
#else
        case CN_UART_DATABITS_7:
            djybsp_uart[port].data_width = DATA_WIDTH_7BIT;
            break;

        case CN_UART_DATABITS_8:
            djybsp_uart[port].data_width = DATA_WIDTH_8BIT;
            break;
#endif
        default:break;
    }

    switch(COM->StopBits)  //todu:F7的 0.5个停止位没有写进来
    {
        case CN_UART_STOPBITS_1:    djybsp_uart[port].stop_bits = BK_STOP_BITS_1;break;
        case CN_UART_STOPBITS_2:    djybsp_uart[port].stop_bits = BK_STOP_BITS_2;break;
        default:break;
    }
    uart_hw_set_change(port,&djybsp_uart[port]);
}

// =============================================================================
// 功能: 启动串口发送，包括两种情况，使用DMA或不使用DMA
// 参数: Reg,被操作的串口寄存器指针.
// 返回: 发送的个数
// =============================================================================
static u32 __UART_SendStart (ptu32_t port)
{

    u8 val[256];
    u32 i = 0,len = 0;
    if(port>CN_UART2)
       return 0;
    Board_UartHalfDuplexSend((u8)port);//切换到发送
    uart_set_tx_fifo_needwr_int((u8)port,1);

    __UART_CloseInte((u8)port);
    uart_set_tx_stop_end_int((u8)port, 1);

    if(uart_is_tx_fifo_empty((u8)port) == 1)
    {
        len = UART_PortRead((struct UartGeneralCB *)pUartCB[port],val,256);
        if(0 != len)
        {
            for(i = 0; i < len; i++)
            {
                uart_write_byte(port,val[i]);
            }
        }

    }

    __UART_OpenInte((u8)port);
    return 1;
}

// =============================================================================
// 功能：UART中断,若为idle中断，则从DMA缓冲区中读出数据，并重新启动DMA，否则调用HAL中断
//       处理函数，最终会调用到HAL_UART_XXXXCallback（）
// 参数：串口号
// 返回：1=成功，0=失败
// =============================================================================
static void djybsp_uart_rx_isr(s32 port, void *param)
{
    uint8_t val = 0;
    uint8_t fifo[256];
    uint32_t fifo_status_reg = 0;
    u32 num = 0;

    if(port>CN_UART2)
        return ;

    if(UART1_PORT == port)
        fifo_status_reg = REG_UART1_FIFO_STATUS;
    else
        fifo_status_reg = REG_UART2_FIFO_STATUS;
    while(REG_READ(fifo_status_reg) & FIFO_RD_READY)
    {
        UART_READ_BYTE(port, val);
        fifo[num++] = val;
        if(num >= sizeof(fifo))
            break;
    }
    if(num > 0)
    {
        if(num != UART_PortWrite((struct UartGeneralCB *)pUartCB[port],fifo,num) && (port ==1))
        {
            UART_ErrHandle((struct UartGeneralCB *)pUartCB[port],CN_UART_BUF_OVER_ERR);
        }
    }
    return ;
}

static void djybsp_uart_tx_isr(s32 port, void *param)
{
    uint8_t val = 0;
    if(port>CN_UART2)
        return ;

    while(uart_is_tx_fifo_full(port)==0)
    {
        if(0 != UART_PortRead((struct UartGeneralCB *)pUartCB[port],&val,1))
        {
            uart_write_byte(port,val);
        }
        else
        {
            uart_set_tx_fifo_needwr_int(port,0);
            break;
        }
    }
    return ;
}

static void djybsp_uart_tx_end_isr(s32 port, void *param)
{
    if(port>CN_UART2)
        return ;

    Board_UartHalfDuplexRecv(port);
    return ;
}

void djybsp_uart_rx_over_isr(s32 port, void *param)
{
    if(port>CN_UART2)
        return ;
    UART_ErrHandle((struct UartGeneralCB *)pUartCB[port],CN_UART_FIFO_OVER_ERR);

    return ;
}
#pragma GCC diagnostic pop

// =============================================================================
// 功能: 初始化UART对应的中断线，并初始化中断入口函数
// 参数：SerialNo,串口号
// 返回: 无
// =============================================================================
static void __UART_IntInit(u32 port)
{
    if(port>CN_UART2)
       return ;
    uart_rx_callback_set(port, djybsp_uart_rx_isr, (void *)port);
    uart_tx_fifo_needwr_callback_set(port,djybsp_uart_tx_isr,(void *)port);
    uart_tx_end_callback_set(port,djybsp_uart_tx_end_isr,(void *)port);
    uart_rx_fifo_over_callback_set(port,djybsp_uart_rx_over_isr,(void *)port);
}

// =============================================================================
// 功能: 串口设备的控制函数,与具体的硬件寄存器设置相关
// 参数: Reg,被操作的串口寄存器指针.
//       cmd,操作类型
//       data1,data2,含义依cmd而定
// 返回: 无意义.
// =============================================================================
static ptu32_t __UART_Ctrl(uint8_t port,u32 cmd, va_list *arg0)
{
    ptu32_t result = 0;

    if(port>CN_UART2)
        return 0;

    switch(cmd)
    {
        case CN_DEV_CTRL_START:
            uart_reset(port);
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
            u32 data;
            data = va_arg(*arg0, u32);
            __UART_BaudSet(port, data);
        }
            break;
        case CN_UART_EN_RTS:
            break;
        case CN_UART_DIS_RTS:
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
            struct COMParam *COM;
            COM = va_arg(*arg0, void *);
            __UART_ComConfig(port,COM);
        }
            break;
        default: break;
    }
    return result;
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
            uart1_init();
            UART_SndBufLen = CFG_UART1_SENDBUF_LEN;
            UART_RxBufLen  = CFG_UART1_RECVBUF_LEN;
            break;
        case CN_UART2:
            uart2_init();
            UART_SndBufLen = CFG_UART2_SENDBUF_LEN;
            UART_RxBufLen  = CFG_UART2_RECVBUF_LEN;
            break;
        default:printk("ModuleInstall_UART:port Err\r\n");break;
    }

    if(port < CN_UART_NUM)
    {
        UART_Param.Name         = sUartName[port];
        UART_Param.UartPortTag  = (ptu32_t)sUartReg[port];
        UART_Param.Baud         = 115200;
        UART_Param.TxRingBufLen = UART_SndBufLen;
        UART_Param.RxRingBufLen = UART_RxBufLen;
        UART_Param.StartSend    = __UART_SendStart;
        UART_Param.UartCtrl     = (UartControl)__UART_Ctrl;

        Board_UartHalfDuplexRecv(port);
        pUartCB[port] = UART_InstallGeneral(&UART_Param);
        if( pUartCB[port] != NULL)
        {
//            __UART_HardInit(port);              //硬件初始化
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
    //bk_send_string(TxDirectPort,str);
    bk_send_string_len(TxDirectPort,str,len);
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
    while(1)
    {
        if( ((char)(result = uart_read_byte(RxDirectPort)))!=-1 )
        {
            break;
        }
        DJY_EventDelay(500);
    }
    return result;
}

__attribute__((weak)) void bk_printf(const char *fmt, ...)
{

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
    else
    {
        PutStrDirect = NULL ;
    }

    if(TxDirectPort != CN_LIMIT_UINT8)
    {
        __UART_HardInit(TxDirectPort);
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
    else
    {
        GetCharDirect = NULL ;
    }

    if((s32)RxDirectPort >= 0)
    {
        if(TxDirectPort != RxDirectPort)
            __UART_HardInit(RxDirectPort);
        GetCharDirect = Uart_GetCharDirect;
    }
    return;
}

#include "shell.h"
typedef void (*Ex_shell_func)(void);
void __asm_ExecuteCmd(union param *ptab,
       enum param_typr *pflag,Ex_shell_func fun,u32 num)
{

}

