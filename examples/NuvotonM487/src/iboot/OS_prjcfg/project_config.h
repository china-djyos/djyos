/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//******************************* Configure DevFile ******************************************//
#define CFG_DEVFILE_LIMIT       10      //定义设备数量
//******************************* Configure kernel ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //定义初始化过程使用的栈空间一般按默认值就可以了
#define CFG_EVENT_LIMIT         10      //事件数量
#define CFG_EVENT_TYPE_LIMIT    10      //事件类型数
#define CFG_MAINSTACK_LIMIT     4096    //main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024    //IDLE事件处理函数运行的栈尺寸一般按默认值就可以了
#define CFG_OS_TINY             false   //true=用于资源紧缺的场合内核会裁剪掉：事件类型名字事件处理时间统计。
//******************************* Configure lock ******************************************//
#define CFG_LOCK_LIMIT          40      //定义锁的数量包含信号量和互斥量
//******************************* Configure MemoryPool ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //允许建立10个内存池
//******************************* Configure loader ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN     //EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_IBOOT_CRC   EN_NO_APP_CRC      //是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_APP_FILENAME        ""              //CFG_APP_RUNMODE==EN_FORM_FILE，这里配置文件名
#define CFG_IBOOT_VERSION       "V1.0.0"        //Iboot发布版本号
#define CFG_IBOOT_UPDATE_NAME   "iboot.bin"     //iboot更新文件名，此二配置，以后改掉
#define CFG_IBOOT_UPDATE_PATH   "iboot.bin"     //iboot更新路径名，此二配置，以后改掉
//******************************* Configure heap ******************************************//
#define CFG_DYNAMIC_MEM 1  //1 = 支持动态内存分配即使不支持也允许使用malloc-free分配内存但使用有差别详见《……用户手册》内存分配章节
//******************************* Configure stdio ******************************************//
#define CFG_STDIO_RUN_MODE            (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW) //
#define CFG_STDIO_IN_NAME              "/dev/UART0"//
#define CFG_STDIO_OUT_NAME             "/dev/UART0"//
#define CFG_STDIO_ERR_NAME             "/dev/UART0"//
//******************************* Configure time ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //北京时间是东8区
//******************************* Configure cpu_peri_uart ******************************************//
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
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (192*Mhz)         //主频，内核要用，必须定义

#endif
