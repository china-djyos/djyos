/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include <stdint.h>
#include <stddef.h>
//manual config start
//此处填写手动配置，DIDE生成配置文件时，不会被修改
//manual config end

#define    CN_RUNMODE_IBOOT                0                //IBOOT模式运行
#define    CN_RUNMODE_APP                2                //由IBOOT加载的APP
#define    CFG_RUNMODE                CN_RUNMODE_IBOOT                //由IBOOT加载的APP
//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10       // "设备数量",定义设备数量
#define    CFG_MODULE_ENABLE_DEVICE_FILE_SYSTEM  true
//*******************************  Configure int  ******************************************//
#define    CFG_MODULE_ENABLE_INT           true
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8    // "对象数初始值"，用完会自动扩充
#define CFG_HANDLE_LIMIT        8    // "句柄数初始值"，用完会自动扩充
#define    CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40       // "锁的数量",定义锁的数量，包含信号量和互斥量
#define    CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10       // "内存池数量限值",
#define    CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure message queue  ******************************************//
#define    CFG_MODULE_ENABLE_MESSAGE_QUEUE true
//*******************************  Configure multiplex  ******************************************//
#define    CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure ring buffer and line buffer  ******************************************//
#define    CFG_MODULE_ENABLE_RING_BUFFER_AND_LINE_BUFFER  true
//*******************************  Configure watch dog  ******************************************//
#define CFG_WDT_LIMIT           10      //允许养狗数量
#define    CFG_MODULE_ENABLE_WATCH_DOG     true
//*******************************  Configure cpu onchip timer  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_TIMER  true
//*******************************  Configure cpu onchip systime  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_SYSTIME  true
//*******************************  Configure cpu onchip dma  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_DMA  true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure STM32 commond code  ******************************************//
#define    CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096     // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         15       // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    15       // "事件类型数限值",事件类型数
#define CFG_MAINSTACK_LIMIT     4096     // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024     // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_IDLE_MONITOR_CYCLE  30      //监视IDLE事件持续低于1/16 CPU占比的时间秒数，0=不监视
#define CFG_OS_TINY             false    // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure misc  ******************************************//
#define    CFG_MODULE_ENABLE_MISC          true
//*******************************  Configure Software Timers  ******************************************//
#define CFG_TIMERS_LIMIT        5        // "定时器数量",可创建的定时器数量（不包含图形界面的定时器）
#define CFG_TIMER_SOUCE_HARD    true     // "硬件定时器提供时钟源",选择专用硬件还是tick做时钟源
#define    CFG_MODULE_ENABLE_SOFTWARE_TIMERS  true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8       // "时区",北京时间是东8区
#define    CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true   // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure cpu onchip uart  ******************************************//
#define CFG_UART_SENDBUF_LEN            32                   //  "串口发送环形缓冲区大小",
#define CFG_UART_RECVBUF_LEN            32                   //  "串口接收环形缓冲区大小",
#define CFG_UART1_ENABLE                true                 //  "配置是否使用UART1",
#define CFG_UART2_ENABLE                false                //  "配置是否使用UART2",
#define CFG_UART3_ENABLE                false                //  "配置是否使用UART3",
#define CFG_UART4_ENABLE                false                //  "配置是否使用UART4",
#define CFG_UART5_ENABLE                false                //  "配置是否使用UART5",
#define CFG_UART6_ENABLE                false                //  "配置是否使用UART6",
#define CFG_UART7_ENABLE                false                //  "配置是否使用UART7",
#define CFG_UART8_ENABLE                false                //  "配置是否使用UART8",
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_UART  true
//*******************************  Configure tpl  ******************************************//
#define     CFG_TPL_PROTONUM            5        // "支持的传输协议数"，占用一个 tagTplProtoItem 结构
#define    CFG_MODULE_ENABLE_TPL           true
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true          // "是否支持多种输入设备",
#define CFG_STDIO_STDOUT_FOLLOW    true          // "stdout是否跟随stdin",
#define CFG_STDIO_STDERR_FOLLOW    true          // "stderr是否跟随stdin",
#define CFG_STDIO_FLOAT_PRINT      true          // "支持浮点打印"
#define CFG_STDIO_STDIOFILE        true          // "支持标准IO文件"
#define CFG_STDIO_IN_NAME              "/dev/UART1"     // "标准输入设备名",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"     // "标准输出设备名",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"     // "标准err输出设备名",
#define    CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000       // "执行shell命令的栈尺寸",
#define CFG_ADD_ROUTINE_SHELL      true         // "是否添加常规shell命令",
#define CFG_ADD_EXPAND_SHELL       true         // "是否添加拓展shell命令",
#define CFG_ADD_GLOBAL_FUN         false        // "添加全局函数到shell",
#define CFG_SHOW_ADD_SHEELL        true         // "显示在编译窗口添加的shell命令",
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Core Clock  ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //主频，内核要用，必须定义
//*******************************  DjyosProduct Configuration  ******************************************//
#define    PRODUCT_MANUFACTURER_NAME       ""                //厂商名称
#define    PRODUCT_PRODUCT_CLASSIFY        ""                //产品分类
#define    PRODUCT_PRODUCT_MODEL           ""                //产品型号
#define    PRODUCT_VERSION_LARGE           0                 //版本号,__.xx.xx
#define    PRODUCT_VERSION_MEDIUM          0                 //版本号,xx.__.xx
#define    PRODUCT_VERSION_SMALL           0                 //版本号,xx.xx.__
#define    PRODUCT_PRODUCT_MODEL_CODE      ""                //产品型号编码
#define    PRODUCT_PASSWORD                ""                //产品秘钥
#define    PRODUCT_OTA_ADDRESS             ""                //OTA服务器地址
#define    PRODUCT_BOARD_TYPE              ""                //板件类型
#define    PRODUCT_CPU_TYPE                ""                //CPU类型



#endif
