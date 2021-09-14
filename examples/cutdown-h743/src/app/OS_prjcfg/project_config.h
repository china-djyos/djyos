/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

#define CFG_RUNMODE_BAREAPP    1
//*******************************  Configure lock  ******************************************//
#define     CFG_LOCK_LIMIT                   3                         // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure heap  ******************************************//
#define     CFG_DYNAMIC_MEM                  false                   // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     256              // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         3                // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    3                // "事件类型数限值",事件类型数
#define CFG_MAINSTACK_LIMIT     1024              // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024              // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_OS_TINY             true             // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //用完会自动扩充
#define CFG_HANDLE_LIMIT        8   //用完会自动扩充
//*******************************  Configure loader  ******************************************//
#define    CFG_APP_RUNMODE                 CN_DIRECT_RUN     //CN_DIRECT_RUN=直接从flash中运行；CN_FORM_FILE=从文件系统加载到内存运行，
#define    CFG_APP_VERIFICATION            VERIFICATION_NULL //是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define    CFG_IBOOT_VERSION               01                //Iboot发布版本号
#define    CFG_IBOOT_UPDATE_NAME           "/yaf2/iboot.bin" //
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true      // "是否支持多种输入设备",
#define CFG_STDIO_STDOUT_FOLLOW    true      // "stdout是否跟随stdin",
#define CFG_STDIO_STDERR_FOLLOW    true      // "stderr是否跟随stdin",
#define     CFG_STDIO_FLOAT_PRINT            false                // "支持浮点打印"
#define     CFG_STDIO_STDIOFILE              false                // "支持标准IO文件"
#define CFG_STDIO_IN_NAME              "/dev/UART1"      // "标准输入设备名",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"      // "标准输出设备名",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"      // "标准err输出设备名",
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //主频，内核要用，必须定义

#endif