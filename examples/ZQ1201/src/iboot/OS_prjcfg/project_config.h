/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40               // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true           // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       10               // "设备数量",定义设备数量
//*******************************  Configure filesystem  ******************************************//
#define CFG_CLIB_BUFFERSIZE            512          // "C库文件用户态缓冲区尺寸"
//*******************************  Configure stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW) // "STDIO模式",
#define     CFG_STDIO_IN_NAME                "/dev/UART2"       // "标准输入设备名",
#define     CFG_STDIO_OUT_NAME               "/dev/UART2"       // "标准输出设备名",
#define     CFG_STDIO_ERR_NAME               "/dev/UART2"       // "标准err输出设备名",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN            32                          // "UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN            32                          // "UART1接收环形缓冲区大小",
#define CFG_UART1_DMABUF_LEN             32                          // "UART1 DMA缓冲区大小",
#define CFG_UART2_SENDBUF_LEN            32                          // "UART2发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN            32                          // "UART2接收环形缓冲区大小",
#define CFG_UART2_DMABUF_LEN             32                          // "UART2 DMA缓冲区大小",
#define CFG_UART3_SENDBUF_LEN            32                          // "UART3发送环形缓冲区大小",
#define CFG_UART3_RECVBUF_LEN            32                          // "UART3接收环形缓冲区大小",
#define CFG_UART3_DMABUF_LEN             32                          // "UART3 DMA缓冲区大小",
#define CFG_UART4_SENDBUF_LEN            32                          // "UART4发送环形缓冲区大小",
#define CFG_UART4_RECVBUF_LEN            32                          // "UART4接收环形缓冲区大小",
#define CFG_UART4_DMABUF_LEN             32                          // "UART4 DMA缓冲区大小",
#define CFG_UART1_ENABLE                 false                        // "是否使用UART1",
#define CFG_UART2_ENABLE                 true                       // "是否使用UART2",
#define CFG_UART3_ENABLE                 false                       // "是否使用UART3",
#define CFG_UART4_ENABLE                 false                       // "是否使用UART4",
//*******************************  Configure timer  ******************************************//
#define     CFG_TIMER_SOUCE                  1                         // "时钟源",1=由硬件计时器提供时钟源，0=由tick提供时钟源
#define     CFG_TIMERS_LIMIT                 5                         // "定时器数量",可创建的定时器数量（不包含图形界面的定时器）
//*******************************  Configure ymodem  ******************************************//
#define     CFG_YMODEM_BUF_NUM               32                    // "缓存ymodem包数量",每包1024字节
#define     CFG_YMODEM_PKG_TIMEOUT           (15*1000*1000)        // "包间隔超时时间",微秒
#define     CFG_YMODEM_TIMEOUT               (300*1000*1000)       // "ymodem传输总超时时间",微秒
#define     CFG_YMODEM_DEVNAME               "std"                 // "ymodem传输设备，std表示使用标准输入输出设备"
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096             // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         10               // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    10               // "事件类型数限值",事件类型数
#define CFG_MAINSTACK_LIMIT     4096             // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024             // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_OS_TINY             false            // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10               // "内存池数量",允许建立10个内存池
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8               // "时区",北京时间是东8区
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN              // EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_IBOOT_CRC   EN_NO_APP_CRC               // 是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_APP_FILENAME        ""                       // CFG_APP_RUNMODE==EN_FORM_FILE，这里配置文件名
#define CFG_IBOOT_VERSION       "V1.0.0"                 // Iboot发布版本号
#define CFG_IBOOT_UPDATE_NAME   "iboot.bin"              // iboot更新文件名，此二配置，以后改掉
#define CFG_IBOOT_UPDATE_PATH   "\\"              // iboot更新路径名，此二配置，以后改掉
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (12*Mhz)          //主频，内核要用，必须定义

#endif