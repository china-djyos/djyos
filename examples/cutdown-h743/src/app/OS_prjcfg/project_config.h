/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

#define CFG_RUNMODE_BAREAPP    1
//*******************************  Configure component lock  ******************************************//
#define     CFG_LOCK_LIMIT                   3                      // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure component heap  ******************************************//
#define     CFG_DYNAMIC_MEM                  false                // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure component stdio  ******************************************//
#define     CFG_STDIO_FLOAT_PRINT            false             //
#define     CFG_STDIO_STDIOFILE              false             //
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)//
#define     CFG_STDIO_IN_NAME                "/dev/UART1"      //
#define     CFG_STDIO_OUT_NAME               "/dev/UART1"      //
#define     CFG_STDIO_ERR_NAME               "/dev/UART1"      //
//*******************************  Configure component kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     256           // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         3             // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    3             // "事件类型数限值",事件类型数
#define CFG_MAINSTACK_LIMIT     1024           // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024           // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_OS_TINY             true          // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure os Obj file  ******************************************//
#define     CFG_OBJECT_LIMIT                 5                 //用完会自动扩充
#define     CFG_HANDLE_LIMIT                 5                 //用完会自动扩充
//*******************************  Configure os memory poll  ******************************************//
#define     CFG_MEMPOOL_LIMIT                5                    // "内存池数量限值",
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //主频，内核要用，必须定义

#endif
