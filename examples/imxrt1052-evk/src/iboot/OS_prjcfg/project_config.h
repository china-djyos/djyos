/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure NlsCharset  ******************************************//
#define     CFG_LOCAL_CHARSET                "gb2312"               // "本地字符集",
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          100           // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true       // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       20           // "设备数量",定义设备数量
//*******************************  Configure font  ******************************************//
#define     CFG_FONT_DEFAULT                 CN_FONT_GB2312_SONG_16     // "默认字体",字体名在include/font目录中找
//*******************************  Configure gb2312_dot  ******************************************//
#define     CFG_GB2312_12_SONG               zk_disable             // "12点阵宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"      // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_16_SONG              from_array                  // "16点阵宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_16_SONG_FILENAME "zk_gb2316_16song.bin"      // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_16_YUAN              zk_disable                  // "16点阵圆体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_16_YUAN_FILENAME "zk_gb2316_16yuan.bin"      // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_16_KAI              zk_disable                   // "16点阵楷体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_16_KAI_FILENAME  "zk_gb2316_16kai.bin"       // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_16_HEI              zk_disable                   // "16点阵黑",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_16_HEI_FILENAME  "zk_gb2316_16hei.bin"       // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_16_FANG              zk_disable                  // "16点阵仿宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_16_FANG_FILENAME "zk_gb2316_16fang.bin"      // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_24_SONG              zk_disable                  // "24点阵宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_24_SONG_FILENAME "zk_gb2316_24song.bin"      // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_24_YUAN              zk_disable                  // "24点阵圆体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_24_YUAN_FILENAME "zk_gb2316_24yuan.bin"      // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_24_KAI              zk_disable                   // "24点阵楷体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_24_KAI_FILENAME  "zk_gb2316_24kai.bin"       // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_24_HEI              zk_disable                   // "24点阵黑体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_24_HEI_FILENAME  "zk_gb2316_24hei.bin"       // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_24_FANG              zk_disable                  // "24点阵仿宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_24_FANG_FILENAME "zk_gb2316_24fang.bin"      // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_32_SONG              zk_disable                  // "32点阵宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_32_SONG_FILENAME "zk_gb2316_32song.bin"      // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_32_YUAN              zk_disable                  // "32点阵圆体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_32_YUAN_FILENAME "zk_gb2316_32yuan.bin"      // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_32_KAI              zk_disable                   // "32点阵楷体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_32_KAI_FILENAME  "zk_gb2316_32kai.bin"       // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_32_HEI              zk_disable                   // "32点阵黑体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_32_HEI_FILENAME  "zk_gb2316_32hei.bin"       // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_32_FANG              zk_disable                  // "32点阵仿宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_32_FANG_FILENAME "zk_gb2316_32fang.bin"      // "字库文件名",若从文件读取，则配置文件名
//*******************************  Configure gkernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024         // "缓冲区长度",gdd向gkernel传递命令的缓冲区长度（字节数）
//*******************************  Configure gdd  ******************************************//
#define CFG_DESKTOP_WIDTH       480                // "桌面宽度",桌面尺寸（像素数）宽度，0=显示器宽度
#define CFG_DESKTOP_HEIGHT      272                // "桌面高度",桌面尺寸（像素数）高度，0=显示器高度
#define CFG_DISPLAY_NAME        "LCD_DISPLAY"              // "显示器名",须与bsp中显示器驱动模块配置的显示器名字相同
#define CFG_DESKTOP_NAME        "Desktop"              // "桌面名"
#define CFG_INPUTDEV_NAME       "TOUCH_FT5406"              // "输入设备名称",使用bsp中输入设备所配置的名字，多输入设备的话，每个设备间用逗号隔开
#define CFG_DESKTOP_FORMAT      CN_SYS_PF_RGB565         // "像素格式",桌面窗口像素格式，常数在gkernel.h中定义，一般使用与显示器相同颜色
#define CFG_GRAY_BASE_COLOR     CN_COLOR_WHITE           // "灰度基色",像素格式设为灰度时才需要设置的“最亮”色，可在gkernel.h中找到常用颜色定义
#define CFG_FILL_COLOR          CN_COLOR_BLUE            // "填充色",创建桌面时的填充色，用888格式，可在gkernel.h中找到常用颜色定义
//*******************************  Configure HmiInput  ******************************************//
#define     CFG_HMIIN_DEV_LIMIT              2                      // "name",人机交互输入设备数量，如键盘、鼠标等
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_RUN_MODE            (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)      // "STDIO模式",
#define CFG_STDIO_IN_NAME              "/dev/UART1"     // "输入设备名",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"     // "输出设备名",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"     // "err输出设备名",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN    64                  // "UART1发送缓冲大小",
#define CFG_UART1_RECVBUF_LEN    64                  // "UART1接收缓冲大小",
#define CFG_UART2_SENDBUF_LEN    64                  // "UART2发送缓冲大小",
#define CFG_UART2_RECVBUF_LEN    64                  // "UART2接收缓冲大小",
#define CFG_UART3_SENDBUF_LEN    64                  // "UART3发送缓冲大小",
#define CFG_UART3_RECVBUF_LEN    64                  // "UART3接收缓冲大小",
#define CFG_UART4_SENDBUF_LEN    64                  // "UART4发送缓冲大小",
#define CFG_UART4_RECVBUF_LEN    64                  // "UART4接收缓冲大小",
#define CFG_UART5_SENDBUF_LEN    64                  // "UART5发送缓冲大小",
#define CFG_UART5_RECVBUF_LEN    64                  // "UART5接收缓冲大小",
#define CFG_UART6_SENDBUF_LEN    64                  // "UART6发送缓冲大小",
#define CFG_UART6_RECVBUF_LEN    64                  // "UART6接收缓冲大小",
#define CFG_UART7_SENDBUF_LEN    64                  // "UART7发送缓冲大小",
#define CFG_UART7_RECVBUF_LEN    64                  // "UART7接收缓冲大小",
#define CFG_UART8_SENDBUF_LEN    64                  // "UART8发送缓冲大小",
#define CFG_UART8_RECVBUF_LEN    64                  // "UART8接收缓冲大小",
#define CFG_UART1_ENABLE         true                // "配置使用UART1",
#define CFG_UART2_ENABLE         false               // "配置使用UART2",
#define CFG_UART3_ENABLE         false               // "配置使用UART3",
#define CFG_UART4_ENABLE         false               // "配置使用UART4",
#define CFG_UART5_ENABLE         false               // "配置使用UART5",
#define CFG_UART6_ENABLE         false               // "配置使用UART6",
#define CFG_UART7_ENABLE         false               // "配置使用UART7",
#define CFG_UART8_ENABLE         false               // "配置使用UART8",
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096         // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         50           // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    50           // "事件类型数限值",事件类型数
#define CFG_MAINSTACK_LIMIT     4096         // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024         // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_OS_TINY             false        // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       20           // "内存池数量",允许建立10个内存池
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8           // "时区",北京时间是东8区
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN          // EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_IBOOT_CRC   EN_NO_APP_CRC           // 是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_APP_FILENAME        ""                   // CFG_APP_RUNMODE==EN_FORM_FILE，这里配置文件名
#define CFG_IBOOT_VERSION       "V1.0.0"             // Iboot发布版本号
#define CFG_IBOOT_UPDATE_NAME   "iboot.bin"          // iboot更新文件名，此二配置，以后改掉
#define CFG_IBOOT_UPDATE_PATH   "\\"          // iboot更新路径名，此二配置，以后改掉
//*******************************  Configure ft5406  ******************************************//
#define CFG_FT5406_RT_I2C_ADDRESS   0x38                     // "设备地址",硬件配置IIC设备地址
#define CFG_FT5406_RT_I2C_CLK_FRE   (100*1000)                // "总线速度",单位Hz
#define CFG_FT5406_BUS_NAME          "IIC1"             // "IIC总线名称",触摸芯片使用的IIC总线名称
#define CFG_FT5406_TOUCH_NAME        "TOUCH_FT5406"     // "触摸屏名称",触摸芯片的名称
#define CFG_FT5406_DESKTOP_NAME      "Desktop"                 // "桌面名称",触摸屏所使用的桌面的名称
//*******************************  Configure cpu_peri_iic  ******************************************//
#define     CFG_IIC1_BUF_LEN                 128                // "IIC1缓冲区大小",
#define     CFG_IIC2_BUF_LEN                 128                // "IIC2缓冲区大小",
#define     CFG_IIC3_BUF_LEN                 128                // "IIC3缓冲区大小",
#define     CFG_IIC4_BUF_LEN                 128                // "IIC4缓冲区大小",
#define     CFG_IIC1_ENABLE                  true               // "是否配置使用IIC1",
#define     CFG_IIC2_ENABLE                  false              // "是否配置使用IIC2",
#define     CFG_IIC3_ENABLE                  false              // "是否配置使用IIC3",
#define     CFG_IIC4_ENABLE                  false              // "是否配置使用IIC4",
//*******************************  Configure cpu_peri_lcd  ******************************************//
#define     CFG_LCD_DISPLAY_NAME             "LCD_DISPLAY"          // "显示器名称",
#define     CFG_LCD_HEAP_NAME                "sdram"                  // "显示使用的堆名称",
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (600*Mhz)          //主频，内核要用，必须定义

#endif