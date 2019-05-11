/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure component lock  ******************************************//
#define CFG_LOCK_LIMIT          40          // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure component heap  ******************************************//
#define CFG_DYNAMIC_MEM true      // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10          // "设备数量",定义设备数量
//*******************************  Configure component stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW) // "STDIO模式",
#define     CFG_STDIO_IN_NAME                "/dev/UART2"       // "标准输入设备名",
#define     CFG_STDIO_OUT_NAME               "/dev/UART2"       // "标准输出设备名",
#define     CFG_STDIO_ERR_NAME               "/dev/UART2"       // "标准err输出设备名",
//*******************************  Configure cpu_peri_component uart  ******************************************//
#define     CFG_UART1_SENDBUF_LEN            32                 // "UART1发送环形缓冲区大小",
#define     CFG_UART1_RECVBUF_LEN            32                 // "UART1接收环形缓冲区大小",
#define     CFG_UART1_DMABUF_LEN             32                 // "UART1 DMA缓冲区大小",
#define     CFG_UART2_SENDBUF_LEN            32                 // "UART2发送环形缓冲区大小",
#define     CFG_UART2_RECVBUF_LEN            32                 // "UART2接收环形缓冲区大小",
#define     CFG_UART2_DMABUF_LEN             32                 // "UART2 DMA缓冲区大小",
#define     CFG_UART3_SENDBUF_LEN            32                 // "UART3发送环形缓冲区大小",
#define     CFG_UART3_RECVBUF_LEN            32                 // "UART3接收环形缓冲区大小",
#define     CFG_UART3_DMABUF_LEN             32                 // "UART3 DMA缓冲区大小",
#define     CFG_UART4_SENDBUF_LEN            32                 // "UART4发送环形缓冲区大小",
#define     CFG_UART4_RECVBUF_LEN            32                 // "UART4接收环形缓冲区大小",
#define     CFG_UART4_DMABUF_LEN             32                 // "UART4 DMA缓冲区大小",
#define     CFG_UART5_SENDBUF_LEN            32                 // "UART5发送环形缓冲区大小",
#define     CFG_UART5_RECVBUF_LEN            32                 // "UART5接收环形缓冲区大小",
#define     CFG_UART5_DMABUF_LEN             32                 // "UART5 DMA缓冲区大小",
#define     CFG_UART6_SENDBUF_LEN            32                 // "UART6发送环形缓冲区大小",
#define     CFG_UART6_RECVBUF_LEN            32                 // "UART6接收环形缓冲区大小",
#define     CFG_UART6_DMABUF_LEN             32                 // "UART6 DMA缓冲区大小",
#define     CFG_UART7_SENDBUF_LEN            32                 // "UART7发送环形缓冲区大小",
#define     CFG_UART7_RECVBUF_LEN            32                 // "UART7接收环形缓冲区大小",
#define     CFG_UART7_DMABUF_LEN             32                 // "UART7 DMA缓冲区大小",
#define     CFG_UART8_SENDBUF_LEN            32                 // "UART8发送环形缓冲区大小",
#define     CFG_UART8_RECVBUF_LEN            32                 // "UART8接收环形缓冲区大小",
#define     CFG_UART8_DMABUF_LEN             32                 // "UART8 DMA缓冲区大小",
#define     CFG_UART1_ENABLE                 false              // "是否使用UART1",
#define     CFG_UART2_ENABLE                 true               // "是否使用UART2",
#define     CFG_UART3_ENABLE                 false              // "是否使用UART3",
#define     CFG_UART4_ENABLE                 false              // "是否使用UART4",
#define     CFG_UART5_ENABLE                 false              // "是否使用UART5",
#define     CFG_UART6_ENABLE                 false              // "是否使用UART6",
#define     CFG_UART7_ENABLE                 false              // "是否使用UART7",
#define     CFG_UART8_ENABLE                 false              // "是否使用UART8",
//*******************************  Configure shell  ******************************************//
#define     CFG_ADD_ROUTINE_SHELL            true                  // "是否添加常规shell命令"
#define     CFG_ADD_EXPAND_SHELL             true                  // "是否添加拓展shell命令"
#define     CFG_ADD_GLOBAL_FUN               false                 // "添加全局函数到shell"
#define     CFG_SHOW_ADD_SHEELL              true                  // "显示在编译窗口添加的shell命令"
#define     CFG_MODULE_ENABLE_SHELL          true
#define     CFG_SHELL_STACK                  (0x1000)
//*******************************  Configure component kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096        // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         15          // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    15          // "事件类型数限值",事件类型数
#define CFG_MAINSTACK_LIMIT     4096        // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024        // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_OS_TINY             false       // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure os memory poll  ******************************************//
#define     CFG_MEMPOOL_LIMIT                10                 // "内存池数量",允许建立10个内存池
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN     //EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL      //是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_IBOOT_VERSION       01        //Iboot发布版本号
//*******************************  Configure filesystem  ******************************************//
#define     CFG_CLIB_BUFFERSIZE              512               //
//*******************************  Configure yaf2filesystem  ******************************************//
#define     CFG_YAF_ECC                      0                 //YAF文件系统文件使能设备ECC功能。0不使用ecc，YAF2_ENABLE_DEVICE_ECC使用ecc
#define     CFG_YAF_INSTALL_OPTION           true              //YAF文件系统安装选项，安装时是否格式化整个文件系统；
#define     CFG_YAF_MOUNT_POINT              "yaf2"            //YAF文件系统安装目录
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (150*Mhz)         //主频，内核要用，必须定义

#endif
