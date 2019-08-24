/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

#define CFG_RUNMODE_BAREAPP    1
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40          // "锁的数量",定义锁的数量，包含信号量和互斥量
#define    CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true      // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10          // "设备数量",定义设备数量
#define    CFG_MODULE_ENABLE_DEVICE_FILE_SYSTEM  true
//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure djybus  ******************************************//
#define    CFG_MODULE_ENABLE_DJYBUS        true
//*******************************  Configure debug information  ******************************************//
#define    CFG_MODULE_ENABLE_DEBUG_INFORMATION  true
//*******************************  Configure graphical kernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024        // "缓冲区长度",gdd向gkernel传递命令的缓冲区长度（字节数）
#define    CFG_MODULE_ENABLE_GRAPHICAL_KERNEL  true
//*******************************  Configure int  ******************************************//
#define    CFG_MODULE_ENABLE_INT           true
//*******************************  Configure multiplex  ******************************************//
#define    CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure kernel  ******************************************//
#define    CFG_INIT_STACK_SIZE             4096                 // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define    CFG_EVENT_LIMIT                 15                   // "事件数量限值",事件数量
#define    CFG_EVENT_TYPE_LIMIT            15                   // "事件类型数限值",事件类型数
#define    CFG_MAINSTACK_LIMIT             4096                 // "main函数栈尺寸",main函数运行所需的栈尺寸
#define    CFG_IDLESTACK_LIMIT             1024                 // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define    CFG_OS_TINY                     false                // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8       // "对象数初始值"，用完会自动扩充
#define CFG_HANDLE_LIMIT        8       // "句柄数初始值"，用完会自动扩充
#define    CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure loader  ******************************************//
#define    CFG_UPDATEIBOOT_EN              false              // "是否支持在线更新Iboot"，
#define    CFG_APP_RUNMODE                 EN_DIRECT_RUN      // EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define    CFG_APP_VERIFICATION            VERIFICATION_NULL  // 是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define    CFG_IBOOT_VERSION_SMALL         00                 // Iboot发布版本号,xx.xx.__
#define    CFG_IBOOT_VERSION_MEDIUM        00                 // Iboot发布版本号,xx.__.xx
#define    CFG_IBOOT_VERSION_LARGE         01                 // Iboot发布版本号,__.xx.xx
#define    CFG_IBOOT_UPDATE_NAME           "/yaf2/iboot.bin"  // "待升级iboot默认存储路径"
#define    CFG_APP_UPDATE_NAME             "/yaf2/app.bin"    // "待升级app默认存储路径"
#define    CFG_MODULE_ENABLE_LOADER        true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10          // "内存池数量限值",
#define    CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure ring buffer and line buffer  ******************************************//
#define    CFG_MODULE_ENABLE_RING_BUFFER_AND_LINE_BUFFER  true
//*******************************  Configure stdio  ******************************************//
#define    CFG_STDIO_STDIN_MULTI           true                  // "是否支持多种输入设备",
#define    CFG_STDIO_STDOUT_FOLLOW         true                  // "stdout是否跟随stdin",
#define    CFG_STDIO_STDERR_FOLLOW         true                  // "stderr是否跟随stdin",
#define    CFG_STDIO_FLOAT_PRINT           true                  // "支持浮点打印"
#define    CFG_STDIO_STDIOFILE             true                  // "支持标准IO文件"
#define    CFG_STDIO_IN_NAME               ("/dev/UART2")        // "标准输入设备名",
#define    CFG_STDIO_OUT_NAME              ("/dev/UART2")        // "标准输出设备名",
#define    CFG_STDIO_ERR_NAME              ("/dev/UART2")        // "标准err输出设备名",
#define    CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure shell  ******************************************//
#define    CFG_SHELL_STACK                 0x1000                // "执行shell命令的栈尺寸"
#define    CFG_ADD_ROUTINE_SHELL           true                  // "是否添加常规shell命令"
#define    CFG_ADD_EXPAND_SHELL            true                  // "是否添加拓展shell命令"
#define    CFG_ADD_GLOBAL_FUN              false                 // "添加全局函数到shell"
#define    CFG_SHOW_ADD_SHEELL             true                  // "显示在编译窗口添加的shell命令"
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure spi bus  ******************************************//
#define    CFG_MODULE_ENABLE_SPI_BUS       true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure cpu onchip uart  ******************************************//
#define    CFG_UART1_SENDBUF_LEN           64                    // "UART1发送环形缓冲区大小",
#define    CFG_UART1_RECVBUF_LEN           64                    // "UART1接收环形缓冲区大小",
#define    CFG_UART1_DMABUF_LEN            64                    // "UART1 DMA环形缓冲区大小",
#define    CFG_UART2_SENDBUF_LEN           64                    // "UART2发送环形缓冲区大小",
#define    CFG_UART2_RECVBUF_LEN           64                    // "UART2接收环形缓冲区大小",
#define    CFG_UART2_DMABUF_LEN            64                    // "UART2 DMA环形缓冲区大小",
#define    CFG_UART3_SENDBUF_LEN           64                    // "UART3发送环形缓冲区大小",
#define    CFG_UART3_RECVBUF_LEN           64                    // "UART3接收环形缓冲区大小",
#define    CFG_UART3_DMABUF_LEN            64                    // "UART3 DMA环形缓冲区大小",
#define    CFG_UART4_SENDBUF_LEN           64                    // "UART4发送环形缓冲区大小",
#define    CFG_UART4_RECVBUF_LEN           64                    // "UART4接收环形缓冲区大小",
#define    CFG_UART4_DMABUF_LEN            64                    // "UART4 DMA环形缓冲区大小",
#define    CFG_UART1_ENABLE                false                 // "是否使用UART1",
#define    CFG_UART2_ENABLE                true                  // "是否使用UART2",
#define    CFG_UART3_ENABLE                false                 // "是否使用UART3",
#define    CFG_UART4_ENABLE                false                 // "是否使用UART4",
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_UART  true
//*******************************  Configure LCD driver st7796s  ******************************************//
#define    CFG_LCD_XSIZE                   320                 // "LCD宽度",
#define    CFG_LCD_YSIZE                   480                 // "LCD高度",
#define    CFG_ST7796S_DISPLAY_NAME        "lcdst7796s"        // "显示器名称",配置液晶显示的名称
#define    CFG_ST7796S_HEAP_NAME           "extram"            // "驱动使用堆名",配置液晶驱动所使用的堆名称
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure cpu onchip flash  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_FLASH  true
//*******************************  Configure spi  ******************************************//
#define    CFG_SPI_CLK                     30000000          //SPI 时钟。
#define    CFG_SPI_CPOL                    1                 //spi时钟极性（1：SCK在空闲状态处于高电平。0：反之）。
#define    CFG_SPI_CPHA                    1                 //spi时钟相位（1：在SCK周期的第二个边沿采样数据。0：在SCK周期的第一个边沿采样数据）
#define    CFG_SPI_FLASH_RAM_POWER         true              //是否打开flash和ram的电源。
#define    CFG_SPI_WORK_MODE_INTE          false             //设置SPI的工作模式，true为中断模式通信，false为普通模式。
//******************************* Core Clock ******************************************//
#define    CFG_CORE_MCLK                   (180.0*Mhz)       //主频，内核要用，必须定义

#endif
