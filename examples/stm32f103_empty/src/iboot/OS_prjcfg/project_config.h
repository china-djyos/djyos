/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //定义锁的数量包含信号量和互斥量
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //即使选false也允许使用malloc-free分配内存但使用有差别详见 《……用户手册》内存分配章节
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //用完会自动扩充
#define CFG_HANDLE_LIMIT        8   //用完会自动扩充
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //定义初始化过程使用的栈空间一般按默认值就可以了
#define CFG_EVENT_LIMIT         15      //事件数量
#define CFG_EVENT_TYPE_LIMIT    15      //事件类型数
#define CFG_MAINSTACK_LIMIT     4096    //main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024    //IDLE事件处理函数运行的栈尺寸一般按默认值就可以了
#define CFG_OS_TINY             false   //true=用于资源紧缺的场合内核会裁剪掉：事件类型名字事件处理时间统计。
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //定义设备数量
//*******************************  Configure cpu onchip uart  ******************************************//
#define    CFG_UART1_SENDBUF_LEN           32                //
#define    CFG_UART1_RECVBUF_LEN           32                //
#define    CFG_UART1_DMABUF_LEN            32                //
#define    CFG_UART2_SENDBUF_LEN           32                //
#define    CFG_UART2_RECVBUF_LEN           32                //
#define    CFG_UART2_DMABUF_LEN            32                //
#define    CFG_UART3_SENDBUF_LEN           32                //
#define    CFG_UART3_RECVBUF_LEN           32                //
#define    CFG_UART3_DMABUF_LEN            32                //
#define    CFG_UART4_SENDBUF_LEN           32                //
#define    CFG_UART4_RECVBUF_LEN           32                //
#define    CFG_UART5_SENDBUF_LEN           32                //
#define    CFG_UART5_RECVBUF_LEN           32                //
#define    CFG_UART1_ENABLE                true              //
#define    CFG_UART2_ENABLE                false             //
#define    CFG_UART3_ENABLE                false             //
#define    CFG_UART4_ENABLE                false             //
#define    CFG_UART5_ENABLE                false             //
//*******************************  Configure loader  ******************************************//
#define    CFG_APP_RUNMODE                 CN_DIRECT_RUN     //CN_DIRECT_RUN=直接从flash中运行；CN_FORM_FILE=从文件系统加载到内存运行，
#define    CFG_APP_VERIFICATION            VERIFICATION_NULL //是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define    CFG_IBOOT_VERSION               01                //Iboot发布版本号
#define    CFG_IBOOT_UPDATE_NAME           "/yaf2/iboot.bin" //
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true         //
#define CFG_STDIO_STDOUT_FOLLOW    true         //
#define CFG_STDIO_STDERR_FOLLOW    true         //
#define CFG_STDIO_FLOAT_PRINT      true         //
#define CFG_STDIO_STDIOFILE        true         //
#define CFG_STDIO_IN_NAME              "/dev/UART1"    //
#define CFG_STDIO_OUT_NAME             "/dev/UART1"    //
#define CFG_STDIO_ERR_NAME             "/dev/UART1"    //
//*******************************  Configure shell  ******************************************//
#define    CFG_SHELL_STACK                 0x1000            //
#define    CFG_ADD_ROUTINE_SHELL           true              //
#define    CFG_ADD_EXPAND_SHELL            true              //
#define    CFG_ADD_GLOBAL_FUN              false             //
#define    CFG_SHOW_ADD_SHEELL             true              //
#define  CFG_MODULE_ENABLE_SHELL         true
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (216.0*Mhz)       //主频，内核要用，必须定义

#endif
