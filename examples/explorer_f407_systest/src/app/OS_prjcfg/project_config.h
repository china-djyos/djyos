/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

#define CFG_RUNMODE_BAREAPP    1
//*******************************  Configure devfile  ******************************************//
#define    CFG_DEVFILE_LIMIT               10                //定义设备数量
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40                // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true            // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_FLOAT_PRINT       true               // "支持浮点打印"
#define CFG_STDIO_STDIOFILE         true               // "支持标准IO文件"
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)          // "STDIO模式",
#define     CFG_STDIO_IN_NAME                "/dev/UART1"                // "标准输入设备名",
#define     CFG_STDIO_OUT_NAME               "/dev/UART1"                // "标准输出设备名",
#define     CFG_STDIO_ERR_NAME               "/dev/UART1"                // "标准err输出设备名",
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000             // "执行shell命令的栈尺寸"
#define     CFG_ADD_ROUTINE_SHELL            true                      // "是否添加常规shell命令"
#define     CFG_ADD_EXPAND_SHELL             true                      // "是否添加拓展shell命令"
#define     CFG_ADD_GLOBAL_FUN               false                     // "添加全局函数到shell"
#define     CFG_SHOW_ADD_SHEELL              true                      // "显示在编译窗口添加的shell命令"
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure timer  ******************************************//
#define CFG_TIMER_SOUCE     1                 // "时钟源",1=由硬件计时器提供时钟源，0=由tick提供时钟源
#define CFG_TIMERS_LIMIT    5                 // "定时器数量",可创建的定时器数量（不包含图形界面的定时器）
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096              // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         15                // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    15                // "事件类型数限值",事件类型数
#define CFG_MAINSTACK_LIMIT     4096              // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024              // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_OS_TINY             false             // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure Objfile  ******************************************//
#define CFG_OBJECT_LIMIT        8          // "对象数初始值"，用完会自动扩充
#define CFG_HANDLE_LIMIT        8          // "句柄数初始值"，用完会自动扩充
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10                // "内存池数量限值",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define     CFG_UART1_SENDBUF_LEN            32                          // "UART1发送环形缓冲区大小",
#define     CFG_UART1_RECVBUF_LEN            32                          // "UART1接收环形缓冲区大小",
#define     CFG_UART1_DMABUF_LEN             32                          // "UART1 DMA缓冲区大小",
#define     CFG_UART2_SENDBUF_LEN            32                          // "UART2发送环形缓冲区大小",
#define     CFG_UART2_RECVBUF_LEN            32                          // "UART2接收环形缓冲区大小",
#define     CFG_UART2_DMABUF_LEN             32                          // "UART2 DMA缓冲区大小",
#define     CFG_UART3_SENDBUF_LEN            32                          // "UART3发送环形缓冲区大小",
#define     CFG_UART3_RECVBUF_LEN            32                          // "UART3接收环形缓冲区大小",
#define     CFG_UART3_DMABUF_LEN             32                          // "UART3 DMA缓冲区大小",
#define     CFG_UART4_SENDBUF_LEN            32                          // "UART4发送环形缓冲区大小",
#define     CFG_UART4_RECVBUF_LEN            32                          // "UART4接收环形缓冲区大小",
#define     CFG_UART4_DMABUF_LEN             32                          // "UART4 DMA缓冲区大小",
#define     CFG_UART5_SENDBUF_LEN            32                          // "UART5发送环形缓冲区大小",
#define     CFG_UART5_RECVBUF_LEN            32                          // "UART5接收环形缓冲区大小",
#define     CFG_UART5_DMABUF_LEN             32                          // "UART5 DMA缓冲区大小",
#define     CFG_UART6_SENDBUF_LEN            32                          // "UART6发送环形缓冲区大小",
#define     CFG_UART6_RECVBUF_LEN            32                          // "UART6接收环形缓冲区大小",
#define     CFG_UART6_DMABUF_LEN             32                          // "UART6 DMA缓冲区大小",
#define     CFG_UART1_ENABLE                 true                        // "是否使用UART1",
#define     CFG_UART2_ENABLE                 false                       // "是否使用UART2",
#define     CFG_UART3_ENABLE                 false                       // "是否使用UART3",
#define     CFG_UART4_ENABLE                 false                       // "是否使用UART4",
#define     CFG_UART5_ENABLE                 false                       // "是否使用UART5",
#define     CFG_UART6_ENABLE                 false                       // "是否使用UART6",
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (168*Mhz)         //主频，内核要用，必须定义

#endif
