/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //定义设备数量
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40       // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true   // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096     // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         15       // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    15       // "事件类型数限值",事件类型数
#define CFG_MAINSTACK_LIMIT     4096     // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024     // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_OS_TINY             false    // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //用完会自动扩充
#define CFG_HANDLE_LIMIT        8   //用完会自动扩充
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true          // "是否支持多种输入设备",
#define CFG_STDIO_STDOUT_FOLLOW    true          // "stdout是否跟随stdin",
#define CFG_STDIO_STDERR_FOLLOW    true          // "stderr是否跟随stdin",
#define CFG_STDIO_FLOAT_PRINT      true          // "支持浮点打印"
#define CFG_STDIO_STDIOFILE        true          // "支持标准IO文件"
#define CFG_STDIO_IN_NAME              "/dev/UART1"     // "标准输入设备名",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"     // "标准输出设备名",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"     // "标准err输出设备名",
//*******************************  Configure shell  ******************************************//
#define    CFG_SHELL_STACK                 0x1000             // "执行shell命令的栈尺寸"
#define    CFG_ADD_ROUTINE_SHELL           true               // "是否添加常规shell命令"
#define    CFG_ADD_EXPAND_SHELL            true               // "是否添加拓展shell命令"
#define    CFG_ADD_GLOBAL_FUN              false              // "添加全局函数到shell"
#define    CFG_SHOW_ADD_SHEELL             true               // "显示在编译窗口添加的shell命令"
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure time  ******************************************//
#define    CFG_LOCAL_TIMEZONE              8                  // "时区",北京时间是东8区
//*******************************  Configure cpu onchip uart  ******************************************//
#define CFG_UART0_SENDBUF_LEN            32                 //
#define CFG_UART0_RECVBUF_LEN            32                 //
#define CFG_UART0_DMABUF_LEN             32                 //
#define CFG_UART1_SENDBUF_LEN            32                 //
#define CFG_UART1_RECVBUF_LEN            32                 //
#define CFG_UART1_DMABUF_LEN             32                 //
#define CFG_UART2_SENDBUF_LEN            32                 //
#define CFG_UART2_RECVBUF_LEN            32                 //
#define CFG_UART2_DMABUF_LEN             32                 //
#define CFG_UART3_SENDBUF_LEN            32                 //
#define CFG_UART3_RECVBUF_LEN            32                 //
#define CFG_UART3_DMABUF_LEN             32                 //
#define CFG_UART4_SENDBUF_LEN            32                 //
#define CFG_UART4_RECVBUF_LEN            32                 //
#define CFG_UART4_DMABUF_LEN             32                 //
#define CFG_UART5_SENDBUF_LEN            32                 //
#define CFG_UART5_RECVBUF_LEN            32                 //
#define CFG_UART5_DMABUF_LEN             32                 //
#define CFG_UART0_ENABLE                 true               //
#define CFG_UART1_ENABLE                 false              //
#define CFG_UART2_ENABLE                 false              //
#define CFG_UART3_ENABLE                 false              //
#define CFG_UART4_ENABLE                 false              //
#define CFG_UART5_ENABLE                 false              //
//*******************************  Configure loader  ******************************************//
#define    CFG_APP_RUNMODE                 CN_DIRECT_RUN     //CN_DIRECT_RUN=直接从flash中运行；CN_FORM_FILE=从文件系统加载到内存运行，
#define    CFG_APP_VERIFICATION            VERIFICATION_NULL //是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define    CFG_IBOOT_VERSION               01                //Iboot发布版本号
#define    CFG_IBOOT_UPDATE_NAME           "/yaf2/iboot.bin" //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (192.0*Mhz)       //主频，内核要用，必须定义

#endif
