#include "board-config.h"
#include <stdio.h>
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
//#include "cpu_peri_dma.h"
// =============================================================================
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
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip uart"//CPU的uart外设驱动
//parent:"uart device file"        //填写该组件的父组件名字，none表示没有父组件
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
//mutex:"none"                     //该组件的互斥组件名（可以是none，表示无互斥组件），
                                   //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue  	****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_UART == false )
//#warning  " cpu_onchip_uart  组件参数未配置，使用默认配置"
//%$#@target = header   		//header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_UART    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,512,
#define CFG_UART1_SENDBUF_LEN       64      //"UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN       64      //"UART1接收环形缓冲区大小",
#define CFG_UART1_DMABUF_LEN        64      //"UART1 DMA环形缓冲区大小",

#define CFG_UART2_SENDBUF_LEN       64      //"UART2发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN       64      //"UART2接收环形缓冲区大小",
#define CFG_UART2_DMABUF_LEN        64      //"UART2 DMA环形缓冲区大小",

#define CFG_UART3_SENDBUF_LEN       64      //"UART3发送环形缓冲区大小",
#define CFG_UART3_RECVBUF_LEN       64      //"UART3接收环形缓冲区大小",
#define CFG_UART3_DMABUF_LEN        64      //"UART3 DMA环形缓冲区大小",

#define CFG_UART4_SENDBUF_LEN       64      //"UART4发送环形缓冲区大小",
#define CFG_UART4_RECVBUF_LEN       64      //"UART4接收环形缓冲区大小",
#define CFG_UART4_DMABUF_LEN        64      //"UART4 DMA环形缓冲区大小",

#define CFG_UART5_SENDBUF_LEN       64      //"UART5发送环形缓冲区大小",
#define CFG_UART5_RECVBUF_LEN       64      //"UART5接收环形缓冲区大小",
#define CFG_UART5_DMABUF_LEN        64      //"UART5 DMA环形缓冲区大小",

#define CFG_UART6_SENDBUF_LEN       64      //"UART6发送环形缓冲区大小",
#define CFG_UART6_RECVBUF_LEN       64      //"UART6接收环形缓冲区大小",
#define CFG_UART6_DMABUF_LEN        64      //"UART6 DMA环形缓冲区大小",

#define CFG_UART7_SENDBUF_LEN       64      //"UART7发送环形缓冲区大小",
#define CFG_UART7_RECVBUF_LEN       64      //"UART7接收环形缓冲区大小",
#define CFG_UART7_DMABUF_LEN        64      //"UART7 DMA环形缓冲区大小",

#define CFG_UART8_SENDBUF_LEN       64      //"UART8发送环形缓冲区大小",
#define CFG_UART8_RECVBUF_LEN       64      //"UART8接收环形缓冲区大小",
#define CFG_UART8_DMABUF_LEN        64      //"UART8 DMA环形缓冲区大小",

//%$#@enum,true,false
#define CFG_UART1_ENABLE           true        //"是否使用UART1",
#define CFG_UART1_ENABLE_DMA       false       //"UART1使能DMA",
#define CFG_UART2_ENABLE           false       //"是否使用UART2",
#define CFG_UART2_ENABLE_DMA       false       //"UART2使能DMA",
#define CFG_UART3_ENABLE           false       //"是否使用UART3",
#define CFG_UART3_ENABLE_DMA       false       //"UART3使能DMA",
#define CFG_UART4_ENABLE           false       //"是否使用UART4",
#define CFG_UART4_ENABLE_DMA       false       //"UART4使能DMA",
#define CFG_UART5_ENABLE           false       //"是否使用UART5",
#define CFG_UART5_ENABLE_DMA       false       //"UART5使能DMA",
#define CFG_UART6_ENABLE           false       //"是否使用UART6",
#define CFG_UART6_ENABLE_DMA       false       //"UART6使能DMA",
#define CFG_UART7_ENABLE           false       //"是否使用UART7",
#define CFG_UART7_ENABLE_DMA       false       //"UART7使能DMA",
#define CFG_UART8_ENABLE           false       //"是否使用UART8",
#define CFG_UART8_ENABLE_DMA       false       //"UART8使能DMA",
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
//@#$%component end configure
// =============================================================================

//#define CFG_UART0_SENDBUF_LEN 		   256
//#define CFG_UART0_RECVBUF_LEN 		   256
//
//#define CFG_UART1_SENDBUF_LEN 		   256
//#define CFG_UART1_RECVBUF_LEN 		   256
//
//#define CFG_UART2_SENDBUF_LEN 		   256
//#define CFG_UART2_RECVBUF_LEN 		   256

static u32 imxStartSend(ptu32_t PrivateTag)
{
	return 0;
}

static ptu32_t imxUartCtrl(ptu32_t PrivateTag,u32 cmd, va_list *arg0)
{
	return 0;
}

struct imxUartPort {
	int port;
	struct UartGeneralCB *UartGeneralCB;
};

static struct imxUartPort imxUartPort[1] = {
	[0] = {
		.port = 0,
		.UartGeneralCB = NULL,
	},
};

static struct UartParam imxUartParam[1] = {
	[0] = {

		.Name          = "UART0",
		.TxRingBufLen  = 256,
		.RxRingBufLen  = 256,
		.Baud          = 115200,
		.mode          = CN_UART_GENERAL,
		.UartPortTag   = 0,
		.StartSend     = imxStartSend,
		.UartCtrl      = imxUartCtrl,
	},
};

ptu32_t ModuleInstall_UART(ptu32_t serial_no)
{
	struct UartParam *Param;
	struct imxUartPort *up;
	up->UartGeneralCB = UART_InstallGeneral(Param);
}

s32 imxPutStrDirect(const char *buf,u32 len)
{
    int c;
    while (len) {
        c = *buf++;
	/* write c to hardware. */
        len--;
    }
}
char imxGetCharDirect(void)
{
    int c;
    c = 'C'; /* read from hardware. */
    return c;
}

void Stdio_KnlInOutInit(char * StdioIn, char *StdioOut)
{
	PutStrDirect = imxPutStrDirect;
	GetCharDirect = imxGetCharDirect;
}

