#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "systime.h"
#include "cpu_peri.h"
#include <device/djy_uart.h>
#include "int.h"
#include "djyos.h"
#include "cpu_peri_dma.h"


#include <gd32e10x.h>

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
//
//    #if CFG_UART5_ENABLE ==1
//    ModuleInstall_UART(CN_UART5);
//    #endif
//
//    #if CFG_UART6_ENABLE ==1
//    ModuleInstall_UART(CN_UART6);
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
//dependence:"device file system","lock","uart device file","heap","cpu onchip dma"//该组件的依赖组件名（可以是none，表示无依赖组件），
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
#define CFG_UART1_SENDBUF_LEN            32                 //"USART0 Tx ring buf size",
#define CFG_UART1_RECVBUF_LEN            32                 //"USART0 Rx ring buf size",
#define CFG_UART1_DMABUF_LEN             32                 //"USART0 DMA buf size",

#define CFG_UART2_SENDBUF_LEN            32                 //"USART1 Tx ring buf size",
#define CFG_UART2_RECVBUF_LEN            32                 //"USART1 Rx ring buf size",
#define CFG_UART2_DMABUF_LEN             32                 //"USART1 DMA buf size",

#define CFG_UART3_SENDBUF_LEN            32                 //"USART2 Tx ring buf size",
#define CFG_UART3_RECVBUF_LEN            32                 //"USART2 Rx ring buf size",
#define CFG_UART3_DMABUF_LEN             32                 //"USART2 DMA buf size",

#define CFG_UART4_SENDBUF_LEN            32                 //"UART3 Tx ring buf size",
#define CFG_UART4_RECVBUF_LEN            32                 //"UART3 Rx ring buf size",
#define CFG_UART4_DMABUF_LEN             32                 //"UART3 DMA buf size",

#define CFG_UART5_SENDBUF_LEN            32                 //"UART4 Tx ring buf size",
#define CFG_UART5_RECVBUF_LEN            32                 //"UART4 Rx ring buf size",
#define CFG_UART5_DMABUF_LEN             32                 //"UART4 DMA buf size",

//%$#@enum,true,false,
#define CFG_UART1_ENABLE                 true               //"enable USART0",
#define CFG_UART2_ENABLE                 false              //"enable USART1",
#define CFG_UART3_ENABLE                 false              //"enable USART2",
#define CFG_UART4_ENABLE                 false              //"enable UART3",
#define CFG_UART5_ENABLE                 false              //"enable UART4",


//%$#@string,1,10,
//%$#select,        ***浠庡垪鍑虹殑閫夐」涓�夋嫨鑻ュ共涓畾涔夋垚瀹�
//%$#@free,
#endif
//%$#@end configue  ****鍙傛暟閰嶇疆缁撴潫
//@#$%component end configure
// =============================================================================

#define CN_UART1_BASE 0x40013800 /* USART0 */
#define CN_UART2_BASE 0x40004400 /* USART1 */
#define CN_UART3_BASE 0x40004800 /* USART2 */
#define CN_UART4_BASE 0x40004C00 /*  UART3 */
#define CN_UART5_BASE 0x40005000 /*  UART4 */

static u32 gd32_StartSend(ptu32_t PrivateTag);
static ptu32_t gd32_UartCtrl(ptu32_t PrivateTag, u32 cmd, va_list *arg0);


const struct UartParam gd32_UART_Param[5] = {
#ifdef CN_UART1
	[0] = {
		.Name = "USART0",
		.Baud = 115200,
		.UartPortTag = USART0,
		.TxRingBufLen = CFG_UART1_SENDBUF_LEN,
		.RxRingBufLen = CFG_UART1_RECVBUF_LEN,
		.StartSend = gd32_StartSend,
		.UartCtrl = gd32_UartCtrl,
		.mode = CN_UART_GENERAL,
	},
#endif
#ifdef CN_UART2
	[1] = {
		.Name = "USART1",
		.Baud = 115200,
		.UartPortTag = USART1,
		.TxRingBufLen = CFG_UART2_SENDBUF_LEN,
		.RxRingBufLen = CFG_UART2_RECVBUF_LEN,
		.StartSend = gd32_StartSend,
		.UartCtrl = gd32_UartCtrl,
		.mode = CN_UART_GENERAL,
	},
#endif
#ifdef CN_UART3
	[2] = {
		.Name = "USART2",
		.Baud = 115200,
		.UartPortTag = USART2,
		.TxRingBufLen = CFG_UART3_SENDBUF_LEN,
		.RxRingBufLen = CFG_UART3_RECVBUF_LEN,
		.StartSend = gd32_StartSend,
		.UartCtrl = gd32_UartCtrl,
		.mode = CN_UART_GENERAL,
	},
#endif
#ifdef CN_UART4
	[3] = {
		.Name = "UART3",
		.Baud = 115200,
		.UartPortTag = UART3,
		.TxRingBufLen = CFG_UART4_SENDBUF_LEN,
		.RxRingBufLen = CFG_UART4_RECVBUF_LEN,
		.StartSend = gd32_StartSend,
		.UartCtrl = gd32_UartCtrl,
		.mode = CN_UART_GENERAL,
	},
#endif
#ifdef CN_UART5
	[4] = {
		.Name = "UART4",
		.Baud = 115200,
		.UartPortTag = UART4,
		.TxRingBufLen = CFG_UART5_SENDBUF_LEN,
		.RxRingBufLen = CFG_UART5_RECVBUF_LEN,
		.StartSend = gd32_StartSend,
		.UartCtrl = gd32_UartCtrl,
		.mode = CN_UART_GENERAL,
	},
#endif
};

static u32 TxByteTime;                    //正常发送一个字节所需要的时间
static u32 TxDirectPort;                  //用于printk发送的串口号
static u32 RxDirectPort;                  //用于直接接收的串口号

// =============================================================================

/* we need a debug uart at the very beginning. */

/*

	CN_INT_LINE_USART0_IRQN,
	CN_INT_LINE_USART1_IRQN,
	CN_INT_LINE_USART2_IRQN,
	CN_INT_LINE_UART3_IRQN,
	CN_INT_LINE_UART4_IRQN,
*/

#define DEVNAM1  "/dev/USART1"
#define DEVNAM2  "/dev/USART2"
#define DEVNAM3  "/dev/USART3"
#define DEVNAM4  "/dev/UART4"
#define DEVNAM5  "/dev/UART5"

/*
*/
struct gd32_uart {
	struct UartGeneralCB *pUartCB;
	int port;
	void *TxRingBuf;	/* Ring buffer size defined in -> pUartCB */
	void *RxRingBuf;
	const char *devnam;
	int irqno;

};

/*TODO: this macro should be public accessable. */
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))



#define CN_DMA_UNUSED   0
#define CN_DMA_USED     1

// UART对应的DMA Stream中断号
static u8 const UartDmaRxInt[16] = { 0 };
static u8 const UartDmaTxInt[16] = { 0 };

typedef void DMA_Stream_TypeDef;


#define DMA1_Stream0 (void *)0
#define DMA1_Stream1 (void *)0
#define DMA1_Stream2 (void *)0
#define DMA1_Stream3 (void *)0
#define DMA1_Stream4 (void *)0
#define DMA1_Stream5 (void *)0

static DMA_Stream_TypeDef *const UartDmaRxStream[] = {
	DMA1_Stream5,
	DMA1_Stream5,
	DMA1_Stream1,
	DMA1_Stream2,
	DMA1_Stream0,
	DMA1_Stream5
};
static DMA_Stream_TypeDef *const UartDmaTxStream[] = {
	DMA1_Stream3,
	DMA1_Stream3,
	DMA1_Stream3,
	DMA1_Stream4,
	DMA1_Stream3,
	DMA1_Stream3
};

static u8 const DMA_Tx_ch[] = {4,4,4,4,4,5};
static u8 const DMA_Rx_ch[] = {4,4,4,4,4,5};
// DMA正在使用标记，是否使用DMA标记
static bool_t s_UART_DmaSending[] = {false,false,false,false,false,false};
static bool_t s_UART_DmaUsed[] = {false,false,false,false,false,false};

//#define CFG_UART1_SENDBUF_LEN            32
//#define CFG_UART1_RECVBUF_LEN            32
//#define CFG_UART1_DMABUF_LEN             32
//
//#define CFG_UART2_SENDBUF_LEN            32
//#define CFG_UART2_RECVBUF_LEN            32
//#define CFG_UART2_DMABUF_LEN             32
//
//#define CFG_UART3_SENDBUF_LEN            32
//#define CFG_UART3_RECVBUF_LEN            32
//#define CFG_UART3_DMABUF_LEN             32
//
//#define CFG_UART4_SENDBUF_LEN            32
//#define CFG_UART4_RECVBUF_LEN            32
//#define CFG_UART4_DMABUF_LEN             32
//
//#define CFG_UART5_SENDBUF_LEN            32
//#define CFG_UART5_RECVBUF_LEN            32
//#define CFG_UART5_DMABUF_LEN             32

static u8 *pUART_DmaSendBuf[CN_UART_NUM];
static u8 *pUART_DmaRecvBuf[CN_UART_NUM];

// static struct UartGeneralCB *pUartCB[CN_UART_NUM];

__attribute__((weak))  void Board_UartHalfDuplexSend(u8 SerialNo)
{
	return;
}
__attribute__((weak))  void Board_UartHalfDuplexRecv(u8 SerialNo)
{
	return;
}
// =============================================================================
static ptu32_t UART_ISR(ptu32_t port);

static uint32_t UART_DmaRx_ISR(ptu32_t port);
static uint32_t UART_DmaTx_ISR(ptu32_t port);

// =============================================================================
// 功能: 禁止uart的发送中断。
// 参数: flag,是否使用dma传输
//       port,串口号，1~5
// 返回: 无
// =============================================================================
void __UART_RxIntEnable(u8 flag, u8 port)
{
	if (flag == CN_DMA_USED)
		Int_RestoreAsynLine(0); /* disable dma's irq */
	else
		/* ##WIP## disable irq. */;
}

// =============================================================================
// 功能: 禁止uart的接收中断
// 参数: reg,被操作的寄存器组指针
//       port,串口号
// 返回: 无
// =============================================================================
void __UART_RxIntDisable(u8 flag, u8 port)
{
	if (flag == CN_DMA_USED)
		Int_SaveAsynLine(0);
	else
		/* ##WIP## disable irq. */;
}

// =============================================================================
// 功能: 使能uart的发送中断
// 参数: reg,被操作的寄存器组指针
//       port,串口号
// 返回: 无
// =============================================================================
static void __UART_TxIntEnable(u8 flag,u8 port)
{
	if (flag == CN_DMA_USED)
		Int_RestoreAsynLine(0);
	else
		/* ##WIP## disable this uart's irq. */;
}



// =============================================================================
// 功能: 禁止UART的发送中断
// 参数: Reg,被操作的寄存器组指针
//       port,串口号
// 返回: 无
// =============================================================================
static void __UART_TxIntDisable(u8 flag, u8 port)
{
//    if(flag == CN_DMA_USED)
//	Int_SaveAsynLine(UartDmaTxInt[port]);
//    else
//	tg_UART_Reg[port]->CR1 &= ~(1<<7);//禁止发送空中断
}

// =============================================================================
// 功能: 初始化UART对应的中断线，并初始化中断入口函数
// 参数：SerialNo,串口号
// 返回: 无
// =============================================================================
static void __UART_IntInit(u32 SerialNo)
{
    u8 IntLine;
    int port, irq;

    port = SerialNo;
    irq = to_irq(port);
    if (irq < 0) return;

    IntLine = irq;
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,UART_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
    Int_SetIsrPara(IntLine,SerialNo);
}
// =============================================================================
// 功能: 查询等待串口发送完毕，超时返回
// 参数: sending，dma发送方式，串口是否已经发送完成
// 返回: 无
// =============================================================================
bool_t __uart_dma_timeout(bool_t sending)
{
}
// =============================================================================
// 功能: 启动串口DMA发送，使用DMA启动，在发送前需判断上次DMA是否已经发送完成，通过
//       查看标志UARTx_DMA_SENDING来判断UARTx是否已经发送完成
// 参数: PORT,串口号.
// 返回: 发送的个数
// =============================================================================
u32 __UART_DMA_SendStart(u32 port)
{
	u32 num, addr;
	u8 *DmaSendBuf;
	u32 DmaSendBufLen;

	switch (port)
	{
	case CN_UART1:
		DmaSendBufLen = CFG_UART1_DMABUF_LEN;        break;
	case CN_UART2:
		DmaSendBufLen = CFG_UART2_DMABUF_LEN;        break;
	case CN_UART3:
		DmaSendBufLen = CFG_UART3_DMABUF_LEN;        break;
	case CN_UART4:
		DmaSendBufLen = CFG_UART4_DMABUF_LEN;        break;
	case CN_UART5:
		DmaSendBufLen = CFG_UART5_DMABUF_LEN;        break;
	default:
		break;
	}
	DmaSendBuf = pUART_DmaSendBuf[port];
	//    if(true == __uart_dma_timeout(s_UART_DmaSending[port]))
	if (true == s_UART_DmaSending[port])
		return 0;
	//    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen);
	addr = (u32)DmaSendBuf;

	if (num > 0)
	{
		DMA_Enable(UartDmaTxStream[port], addr, num); //閸氼垰濮〥MA
		s_UART_DmaSending[port] = true;
	}
	else
	{
		//##WIP##	tg_UART_Reg[port]->CR1 |= (1<<6);
	}
	return 0;

}
// =============================================================================
// 功能: 直接发送函数，使用轮询方式。
// 参数: Reg,寄存器基址指针.
//       send_buf,发送数据缓冲区
//       len,数据长度，字节
//       timeout,超时时间，us
// 返回: 发送字节数
// =============================================================================
//static u32 __UART_SendDirectly(tagUartReg *Reg,u8 *send_buf,u32 len,u32 timeout)
//{
//    u32  result = 0;
//
//    if(Reg == NULL)
//        return result;
//
//    Reg->CR1 &= ~(1<<7);                            //disable send INT
//    for(result=0; result < len; result ++)
//    {
//        // 超时或者发送缓冲为空时退出
//        while((false == __UART_TxTranEmpty(Reg))&& (timeout > 0))
//        {
//            timeout--;
//            DJY_DelayUs(1);
//        }
//        if(timeout == 0)
//            break;
//        Reg->DR = send_buf[result];
//    }
//    Reg->CR1 |= (1<<7);                             //enable send INT
//    return result;
//}

// =============================================================================
// 功能: 启动串口发送，其目的是触发中断，用中断方式发送数据。
// 参数: Reg,被操作的串口寄存器指针.
// 返回: 发送的个数
// =============================================================================

static u32 gd32_StartSend(ptu32_t PrivateTag)
{
#if 0
	struct UartParam *param;
	struct uart_priv *priv;
	struct usart_type *usart_x;

	priv = (void *)PrivateTag;
	param = gd32_UART_Param + priv->port;

	usart_x = to_usart_type(priv->port);
	usart_interrupt_enable(usart_x, USART_IDLE_INT, FALSE);
	if (priv->dmabuf_len) {
		/* dma not supported yet. */
		while (1);
	}
	usart_interrupt_enable(usart_x, USART_IDLE_INT, TRUE);
#endif
	return 0;
}

// =============================================================================
// 功能: 设置uart使用dma收发，根据stm32各串口的收发dma通道配置寄存器。将重新初
//       始化所有dma寄存器，因为:
//       1、不使用dma期间可能有其他设备使用dma，从而更改了设置。
//       2、即使uart使用dma期间，也可能软件模块配合的问题导致其他设备使用dma.
// 参数: port,端口号，即串口号，从1~3
// 返回: 无
// =============================================================================
void __UART_SetDmaUsed(u32 port)
{
#if 0
	u8 *DmaRecvBuf, *DmaSendBuf;
	u32 DmaBufLen;
	switch (port)
	{
	case CN_UART1:
		DmaBufLen = CFG_UART1_DMABUF_LEN;        break;
	case CN_UART2:
		DmaBufLen = CFG_UART2_DMABUF_LEN;        break;
	case CN_UART3:
		DmaBufLen = CFG_UART3_DMABUF_LEN;        break;
	case CN_UART4:
		DmaBufLen = CFG_UART4_DMABUF_LEN;        break;
	case CN_UART5:
		DmaBufLen = CFG_UART5_DMABUF_LEN;        break;
	case CN_UART6:
		DmaBufLen = CFG_UART6_DMABUF_LEN;        break;
	default:        break;
	}

	if (pUART_DmaRecvBuf[port] == NULL)
		pUART_DmaRecvBuf[port] = (u8 *)M_Malloc(DmaBufLen, 0);
	if (pUART_DmaRecvBuf[port] == NULL)
		return;

	if (pUART_DmaSendBuf[port] == NULL)
		pUART_DmaSendBuf[port] = (u8 *)M_Malloc(DmaBufLen, 0);
	if (pUART_DmaSendBuf[port] == NULL)
	{
		free(pUART_DmaRecvBuf[port]);
		pUART_DmaRecvBuf[port] = NULL;
		return;
	}
	DmaRecvBuf = pUART_DmaRecvBuf[port];
	DmaSendBuf = pUART_DmaSendBuf[port];
	__UART_RxIntDisable(CN_DMA_UNUSED, port);
	__UART_TxIntDisable(CN_DMA_UNUSED, port);
	tg_UART_Reg[port]->CR1 |= (1 << 4);//enable idle int

	DMA_Config(UartDmaRxStream[port], DMA_Rx_ch[port], (u32) & (tg_UART_Reg[port]->DR),
		(u32)DmaRecvBuf, DMA_DIR_P2M, DMA_DATABITS_8, DMA_DATABITS_8, 32);

	DMA_Config(UartDmaTxStream[port], DMA_Tx_ch[port], (u32) & (tg_UART_Reg[port]->DR),
		(u32)DmaSendBuf, DMA_DIR_M2P, DMA_DATABITS_8, DMA_DATABITS_8, 32);

    DMA_IntEnable(UartDmaTxStream[port],DMA_INT_TCIE);  //使能发送完成中断
    DMA_IntEnable(UartDmaRxStream[port],DMA_INT_HTIE);  //使能接收半完成中断

    tg_UART_Reg[port]->CR3 |= 0x00c0;  //设置串口使用dma收发
    s_UART_DmaUsed[port] = CN_DMA_USED;
    DMA_Enable(UartDmaRxStream[port],(u32)DmaRecvBuf,32);//启动dma通道

    Int_Register(UartDmaTxInt[port]);
    Int_Register(UartDmaRxInt[port]);//注册接收DMA中断
	Int_SetClearType(UartDmaRxInt[port], CN_INT_CLEAR_AUTO);
	Int_IsrConnect(UartDmaRxInt[port], UART_DmaRx_ISR);
	Int_SetClearType(UartDmaTxInt[port], CN_INT_CLEAR_AUTO);
	Int_IsrConnect(UartDmaTxInt[port], UART_DmaTx_ISR);

	Int_SetIsrPara(UartDmaTxInt[port], port);
	Int_SetIsrPara(UartDmaRxInt[port], port);

	Int_SettoAsynSignal(UartDmaRxInt[port]);
	Int_SettoAsynSignal(UartDmaTxInt[port]);
	Int_RestoreAsynLine(UartDmaRxInt[port]);
	Int_RestoreAsynLine(UartDmaTxInt[port]);
#endif
}

//=============================================================================
// 功能: 设置不使用DMA，UART不使用DMA收发，每收发一个字节就必须中断一次，不建议在高速
//      连续通信中使用。
// 参数: port,端口号，即串口号，为1~3
// 返回: 无
// =============================================================================
void __UART_SetDmaUnUsed(u32 port)
{
	u8 IntLine;
	if (port == CN_UART1)             IntLine = CN_INT_LINE_USART0_IRQN;
	else if (port == CN_UART2)        IntLine = CN_INT_LINE_USART1_IRQN;
	else if (port == CN_UART3)        IntLine = CN_INT_LINE_USART2_IRQN;
	else if (port == CN_UART4)        IntLine = CN_INT_LINE_UART3_IRQN;
	else if (port == CN_UART5)        IntLine = CN_INT_LINE_UART4_IRQN;
	else
		return;

	free(pUART_DmaRecvBuf[port]);
	pUART_DmaRecvBuf[port] = NULL;
	free(pUART_DmaSendBuf[port]);
	pUART_DmaSendBuf[port] = NULL;

#if 0
    tg_UART_Reg[port]->CR3 &= ~0x00c0;     //璁剧疆涓嶄娇鐢╠ma浼犺緭
	Int_RestoreAsynLine(IntLine);
	if (s_UART_DmaUsed[port] == CN_DMA_USED)
	{
		Int_SaveAsynLine(UartDmaRxInt[port]);
		Int_SaveAsynLine(UartDmaTxInt[port]);
		s_UART_DmaUsed[port] = CN_DMA_UNUSED;
	}
	__UART_RxIntEnable(CN_DMA_UNUSED, port);
	__UART_TxIntEnable(CN_DMA_UNUSED, port);
	tg_UART_Reg[port]->CR1 &= ~(1 << 4);//disable idle int
#endif
}
// =============================================================================
// 功能: 串口设备的控制函数,与具体的硬件寄存器设置相关
// 参数: Reg,被操作的串口寄存器指针.
//       cmd,操作类型
//       data1,data2,含义依cmd而定
// 返回: 无意义.
// =============================================================================

ptu32_t gd32_UartCtrl(ptu32_t PrivateTag, u32 cmd, va_list *arg0)
{
	/* depends on clock and pinmux */
	uint32_t usart_periph;
	uint32_t baudval;

	return 0;

	switch (cmd) {
	case CN_DEV_CTRL_START:
		break;
	case CN_UART_SET_BAUD:
		baudval = va_arg(*arg0, uint32_t);
		break;
	case CN_UART_COM_SET:
		break;
	default:
		break;
	}

	return 0;
}

// =============================================================================
// 功能: 当uart2配置成dma方式是，dma1 ch7负责发送，ch6负责接收
// 参数: 中断函数没有参数.
// 返回: 中断函数没有返回值.
// =============================================================================
uint32_t UART_DmaTx_ISR(ptu32_t port)
{
	uint16_t num;
	u8 *DmaSendBuf;
	u32 DmaSendBufLen;

	//    switch(port)
	//    {
	//    case CN_UART1:
	//	DmaSendBufLen = CFG_UART1_DMABUF_LEN;        break;
	//    case CN_UART2:
	//	DmaSendBufLen = CFG_UART2_DMABUF_LEN;        break;
	//    case CN_UART3:
	//	DmaSendBufLen = CFG_UART3_DMABUF_LEN;        break;
	//    case CN_UART4:
	//	DmaSendBufLen = CFG_UART4_DMABUF_LEN;        break;
	//    case CN_UART5:
	//	DmaSendBufLen = CFG_UART5_DMABUF_LEN;        break;
	//    default:        break;
	//    }
	//
	//    DmaSendBuf = pUART_DmaSendBuf[port];
	//    DMA_ClearIntFlag(UartDmaTxStream[port]);
	//    num = UART_PortRead(pUartCB[port],DmaSendBuf,DmaSendBufLen);
	//    if(num == 0)
	//    {
	//	s_UART_DmaSending[port] = false;
	//	tg_UART_Reg[port]->CR1 |= (1<<6);
	//    }
	//    else
	//    {
	//	DMA_Enable(UartDmaTxStream[port],(u32)DmaSendBuf,num);
	//    }
	return 0;
}

// =============================================================================
// 功能: 当uart2配置成dma方式是，dma1 ch7负责发送，ch6负责接收
// 参数: 中断函数没有参数.
// 返回: 中断函数没有返回值.
// =============================================================================
uint32_t UART_DmaRx_ISR(ptu32_t port)
{
	u32 recvs, num, DmaRecvBufLen;
	u8 *DmaRecvBuf;

	switch (port)
	{
	case CN_UART1:
		DmaRecvBufLen = CFG_UART1_DMABUF_LEN;        break;
	case CN_UART2:
		DmaRecvBufLen = CFG_UART2_DMABUF_LEN;        break;
	case CN_UART3:
		DmaRecvBufLen = CFG_UART3_DMABUF_LEN;        break;
	case CN_UART4:
		DmaRecvBufLen = CFG_UART4_DMABUF_LEN;        break;
	case CN_UART5:
		DmaRecvBufLen = CFG_UART5_DMABUF_LEN;        break;
	default:
		break;
	}

	DmaRecvBuf = pUART_DmaRecvBuf[port];
	DMA_Disable(UartDmaRxStream[port]);
	DMA_ClearIntFlag(UartDmaRxStream[port]);
	// recvs = DmaRecvBufLen - UartDmaRxStream[port]->NDTR;
    if(recvs == DmaRecvBufLen)     //dma缓冲区满，可能有数据丢失
	{
		//	UART_ErrHandle(pUartCB[port],CN_UART_FIFO_OVER_ERR);
	}
	else
	{
		//	num = UART_PortWrite(pUartCB[port],DmaRecvBuf,recvs);
		if (num != recvs)
		{
			//	    UART_ErrHandle(pUartCB[port],CN_UART_BUF_OVER_ERR);
		}
	}

	DMA_Enable(UartDmaRxStream[port], (u32)DmaRecvBuf, DmaRecvBufLen);
	return 0;
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
#if 0
	struct UartGeneralCB *UCB;
	int intno = EXINT_LINE_0;

	struct UartParam *param;
	struct uart_priv *priv;
	usart_type *usart_x;
	flag_status flag;
	u8 c[1];
	uint16_t data;
	int rc;

	uint32_t sts;

	if (port > ARRAY_SIZE(gd32_UART_Param))
		return -1;
	param = gd32_UART_Param + port;

	priv = param->UartPortTag;
	if (!priv) {
		/* port error. */
		return -1;
	}

	usart_x = to_usart_type(priv->port);
	sts = usart_x->sts;

	if (sts & USART_RDBF_FLAG) {
		data = usart_data_receive(usart_x);
		c[0] = data;
		rc = UART_PortWrite(priv->pUartCB, c, 1U);
	}

	if (sts & USART_TDBE_FLAG) {
		rc = UART_PortRead(priv->pUartCB, c, 1U);
		data = c[0];

		if (rc) usart_data_transmit(usart_x, data);
		else usart_interrupt_enable(usart_x, USART_IDLE_INT, FALSE);

	}
#endif
	return 0;
}

ptu32_t ModuleInstall_UART(u32 serial_no)
{
	struct UartParam *param;
	struct uart_priv *priv;
	int port = serial_no;

	gd32_PutStrDirect(__func__, sizeof(__func__));

	
	port = CN_UART4; /* CN_UART4 is UART3 */
	param = &gd32_UART_Param[CN_UART4];
	if (UART_InstallGeneral(param)) return 1;
	return 0;
}

// =============================================================================
// 功能：字符终端直接发送函数，采用轮询方式，直接写寄存器，用于printk，或者stdout
//       没有初始化
// 参数：str，发送字符串指针
//      len，发送的字节数
// 返回：0，发生错误；result,发送数据长度，字节单位
// =============================================================================
static s32 gd32_PutStrDirect(const char *str, u32 len)
{
	int i, port = TxDirectPort;

	uint32_t usart_periph = UART3;
	uint32_t data;

	for (i = 0; i < len; i++) {
		data = str[i];
		usart_data_transmit(usart_periph, data);
		while (usart_flag_get(usart_periph, USART_FLAG_TBE) == RESET);
	}
	return i;
}

// =============================================================================
// 功能：字符终端直接接收函数，采用轮询方式，直接读寄存器，用于stdin初始化前
// 参数：str，发送字符串指针
//      len，发送的字节数
// 返回：0，发生错误；result,发送数据长度，字节单位
// =============================================================================

static char gd32_GetCharDirect(void)
{
	uint16_t ch;
	uint32_t usart_periph = UART3;
	char c;

	while (usart_flag_get(usart_periph, USART_FLAG_RBNE) == RESET);
	ch = usart_data_receive(usart_periph);

	c = ch;
	return c;
}

/*
initialize:
   PutStrDirect
   GetCharDirect
   TxByteTime
*/



void Stdio_KnlInOutInit(char *StdioIn, char *StdioOut)
{
	TxDirectPort = CN_UART4;
	TxByteTime = 95;
	PutStrDirect = gd32_PutStrDirect;

	RxDirectPort = CN_UART4;
	GetCharDirect = gd32_GetCharDirect;

	gd32_PutStrDirect("hit any key to continue...\n", 28);
	gd32_GetCharDirect();
	gd32_PutStrDirect("ok", 4);

	return;
}
