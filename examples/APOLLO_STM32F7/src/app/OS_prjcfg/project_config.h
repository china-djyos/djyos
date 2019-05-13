/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure System: lock  ******************************************//
#define CFG_LOCK_LIMIT          40        // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure System: heap  ******************************************//
#define CFG_DYNAMIC_MEM true    // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10        // "设备数量",定义设备数量
//*******************************  Configure System: stdio  ******************************************//
#define CFG_STDIO_FLOAT_PRINT       true         // "支持浮点打印"
#define CFG_STDIO_STDIOFILE         true         // "支持标准IO文件"
#define CFG_STDIO_RUN_MODE            (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)   // "STDIO模式",
#define CFG_STDIO_IN_NAME              "/dev/UART1"      // "标准输入设备名",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"      // "标准输出设备名",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"      // "标准err输出设备名",
//*******************************  Configure System: kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096      // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         15        // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    15        // "事件类型数限值",事件类型数
#define CFG_MAINSTACK_LIMIT     4096      // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024      // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_OS_TINY             false     // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8    // "对象数初始值"，用完会自动扩充
#define CFG_HANDLE_LIMIT        8    // "句柄数初始值"，用完会自动扩充
//*******************************  Configure System: memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10        // "内存池数量限值",
//*******************************  Configure System: loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN       // EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL        // 是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_IBOOT_VERSION       01          // Iboot发布版本号
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"            // "Iboot保存"
//*******************************  Configure cpu_peri_System: uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN       64        // "UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN       64        // "UART1接收环形缓冲区大小",
#define CFG_UART1_DMABUF_LEN        64        // "UART1 DMA环形缓冲区大小",
#define CFG_UART2_SENDBUF_LEN       64        // "UART2发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN       64        // "UART2接收环形缓冲区大小",
#define CFG_UART2_DMABUF_LEN        64        // "UART2 DMA环形缓冲区大小",
#define CFG_UART3_SENDBUF_LEN       64        // "UART3发送环形缓冲区大小",
#define CFG_UART3_RECVBUF_LEN       64        // "UART3接收环形缓冲区大小",
#define CFG_UART3_DMABUF_LEN        64        // "UART3 DMA环形缓冲区大小",
#define CFG_UART4_SENDBUF_LEN       64        // "UART4发送环形缓冲区大小",
#define CFG_UART4_RECVBUF_LEN       64        // "UART4接收环形缓冲区大小",
#define CFG_UART4_DMABUF_LEN        64        // "UART4 DMA环形缓冲区大小",
#define CFG_UART5_SENDBUF_LEN       64        // "UART5发送环形缓冲区大小",
#define CFG_UART5_RECVBUF_LEN       64        // "UART5接收环形缓冲区大小",
#define CFG_UART5_DMABUF_LEN        64        // "UART5 DMA环形缓冲区大小",
#define CFG_UART6_SENDBUF_LEN       64        // "UART6发送环形缓冲区大小",
#define CFG_UART6_RECVBUF_LEN       64        // "UART6接收环形缓冲区大小",
#define CFG_UART6_DMABUF_LEN        64        // "UART6 DMA环形缓冲区大小",
#define CFG_UART7_SENDBUF_LEN       64        // "UART7发送环形缓冲区大小",
#define CFG_UART7_RECVBUF_LEN       64        // "UART7接收环形缓冲区大小",
#define CFG_UART7_DMABUF_LEN        64        // "UART7 DMA环形缓冲区大小",
#define CFG_UART8_SENDBUF_LEN       64        // "UART8发送环形缓冲区大小",
#define CFG_UART8_RECVBUF_LEN       64        // "UART8接收环形缓冲区大小",
#define CFG_UART8_DMABUF_LEN        64        // "UART8 DMA环形缓冲区大小",
#define CFG_UART1_ENABLE           true          // "是否使用UART1",
#define CFG_UART2_ENABLE           false         // "是否使用UART2",
#define CFG_UART3_ENABLE           false         // "是否使用UART3",
#define CFG_UART4_ENABLE           false         // "是否使用UART4",
#define CFG_UART5_ENABLE           false         // "是否使用UART5",
#define CFG_UART6_ENABLE           false         // "是否使用UART6",
#define CFG_UART7_ENABLE           false         // "是否使用UART7",
#define CFG_UART8_ENABLE           false         // "是否使用UART8",
//*******************************  Configure shell  ******************************************//
#define    CFG_ADD_ROUTINE_SHELL           true              //
#define    CFG_ADD_EXPAND_SHELL            true              //
#define    CFG_ADD_GLOBAL_FUN              false             //
#define    CFG_SHOW_ADD_SHEELL             true              //
#define  CFG_MODULE_ENABLE_SHELL         true
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (216*Mhz)         //主频，内核要用，必须定义

#endif