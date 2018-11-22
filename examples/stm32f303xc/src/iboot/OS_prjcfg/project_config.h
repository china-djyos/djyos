/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40             // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true         // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)     // "STDIO模式",
#define     CFG_STDIO_IN_NAME                "/dev/UART1"           // "标准输入设备名",
#define     CFG_STDIO_OUT_NAME               "/dev/UART1"           // "标准输出设备名",
#define     CFG_STDIO_ERR_NAME               "/dev/UART1"           // "标准err输出设备名",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN            32                        // "UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN            32                        // "UART1接收环形缓冲区大小",
#define CFG_UART1_DMABUF_LEN             32                        // "UART1 DMA缓冲区大小"
#define CFG_UART2_SENDBUF_LEN            32                        // "UART2发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN            32                        // "UART2接收环形缓冲区大小",
#define CFG_UART2_DMABUF_LEN             32                        // "UART2 DMA缓冲区大小",
#define CFG_UART3_SENDBUF_LEN            32                        // "UART3发送环形缓冲区大小",
#define CFG_UART3_RECVBUF_LEN            32                        // "UART3接收环形缓冲区大小",
#define CFG_UART3_DMABUF_LEN             32                        // "UART3 DMA缓冲区大小",
#define CFG_UART4_SENDBUF_LEN            32                        // "UART4发送环形缓冲区大小",
#define CFG_UART4_RECVBUF_LEN            32                        // "UART4接收环形缓冲区大小",
#define CFG_UART4_DMABUF_LEN             32                        // "UART4 DMA缓冲区大小",
#define CFG_UART5_SENDBUF_LEN            32                        // "UART5发送环形缓冲区大小",
#define CFG_UART5_RECVBUF_LEN            32                        // "UART5接收环形缓冲区大小",
#define CFG_UART5_DMABUF_LEN             32                        // "UART5 DMA缓冲区大小",
#define CFG_UART1_MODE                   0                         // "UART1模式",CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART2_MODE                   1                         // "UART2模式",CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART3_MODE                   1                         // "UART3模式",CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART4_MODE                   1                         // "UART4模式",CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART5_MODE                   1                         // "UART5模式",CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART1_ENABLE                 true                      // "是否配置使用UART1",
#define CFG_UART2_ENABLE                 false                     // "是否配置使用UART2",
#define CFG_UART3_ENABLE                 false                     // "是否配置使用UART3",
#define CFG_UART4_ENABLE                 false                     // "是否配置使用UART4",
#define CFG_UART5_ENABLE                 false                     // "是否配置使用UART5",
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096           // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         10             // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    10             // "事件类型数限值",事件类型数
#define CFG_MAINSTACK_LIMIT     4096           // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024           // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_OS_TINY             false          // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure MemoryPool  ******************************************//
#define     CFG_MEMPOOL_LIMIT                10                    // "内存池数量",允许建立10个内存池
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN            // EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_IBOOT_CRC   EN_NO_APP_CRC             // 是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_APP_FILENAME        ""                     // CFG_APP_RUNMODE==EN_FORM_FILE，这里配置文件名
#define CFG_IBOOT_VERSION       "V1.0.0"               // Iboot发布版本号
#define CFG_IBOOT_UPDATE_NAME   "iboot.bin"            // iboot更新文件名，此二配置，以后改掉
#define CFG_IBOOT_UPDATE_PATH   "\\"            // iboot更新路径名，此二配置，以后改掉
//*******************************  Configure cpu_peri_iic  ******************************************//
#define CFG_IIC_BUF_LEN           32              // "IIC缓冲区大小",配置IIC缓冲区大小
#define CFG_IIC1_ENABLE           false            // "是否使用IIC1",
#define CFG_IIC2_ENABLE           false            // "是否使用IIC2",
//*******************************  Configure wdt  ******************************************//
#define     CFG_WDT_LIMIT                    10                //允许养狗数量
#define     CFG_WDTMSG_LIMIT                 3                 //操作看门狗的消息队列的最大长度
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (72*Mhz)          //主频，内核要用，必须定义

#endif