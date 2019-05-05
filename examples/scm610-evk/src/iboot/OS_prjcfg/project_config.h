/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40         // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true     // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       10         // "设备数量",定义设备数量
//*******************************  Configure stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)//
#define     CFG_STDIO_IN_NAME                "/dev/UART2"      //
#define     CFG_STDIO_OUT_NAME               "/dev/UART2"      //
#define     CFG_STDIO_ERR_NAME               "/dev/UART2"      //
//*******************************  Configure cpu_peri_uart  ******************************************//
#define     CFG_UART1_SENDBUF_LEN            32                //
#define     CFG_UART1_RECVBUF_LEN            32                //
#define     CFG_UART1_DMABUF_LEN             32                //
#define     CFG_UART2_SENDBUF_LEN            32                //
#define     CFG_UART2_RECVBUF_LEN            32                //
#define     CFG_UART2_DMABUF_LEN             32                //
#define     CFG_UART3_SENDBUF_LEN            32                //
#define     CFG_UART3_RECVBUF_LEN            32                //
#define     CFG_UART3_DMABUF_LEN             32                //
#define     CFG_UART4_SENDBUF_LEN            32                //
#define     CFG_UART4_RECVBUF_LEN            32                //
#define     CFG_UART4_DMABUF_LEN             32                //
#define     CFG_UART5_SENDBUF_LEN            32                //
#define     CFG_UART5_RECVBUF_LEN            32                //
#define     CFG_UART5_DMABUF_LEN             32                //
#define     CFG_UART6_SENDBUF_LEN            32                //
#define     CFG_UART6_RECVBUF_LEN            32                //
#define     CFG_UART6_DMABUF_LEN             32                //
#define     CFG_UART7_SENDBUF_LEN            32                //
#define     CFG_UART7_RECVBUF_LEN            32                //
#define     CFG_UART7_DMABUF_LEN             32                //
#define     CFG_UART8_SENDBUF_LEN            32                //
#define     CFG_UART8_RECVBUF_LEN            32                //
#define     CFG_UART8_DMABUF_LEN             32                //
#define     CFG_UART1_ENABLE                 false             //
#define     CFG_UART2_ENABLE                 true              //
#define     CFG_UART3_ENABLE                 false             //
#define     CFG_UART4_ENABLE                 false             //
#define     CFG_UART5_ENABLE                 false             //
#define     CFG_UART6_ENABLE                 false             //
#define     CFG_UART7_ENABLE                 false             //
#define     CFG_UART8_ENABLE                 false             //
//*******************************  Configure shell  ******************************************//
#define     CFG_ADD_ROUTINE_SHELL            true                 // "是否添加常规shell命令"
#define     CFG_ADD_EXPAND_SHELL             true                 // "是否添加拓展shell命令"
#define     CFG_ADD_GLOBAL_FUN               false                // "添加全局函数到shell"
#define     CFG_SHOW_ADD_SHEELL              true                 // "显示在编译窗口添加的shell命令"
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096       // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         15         // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    15         // "事件类型数限值",事件类型数
#define CFG_MAINSTACK_LIMIT     4096       // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024       // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_OS_TINY             false      // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure MemoryPool  ******************************************//
#define     CFG_MEMPOOL_LIMIT                10                //允许建立10个内存池
//*******************************  Configure IBOOT  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN        // EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL         // 是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_IBOOT_VERSION       01           // Iboot发布版本号
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (150*Mhz)         //主频，内核要用，必须定义

#endif