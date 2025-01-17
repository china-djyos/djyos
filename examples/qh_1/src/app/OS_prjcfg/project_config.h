/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include <stdint.h>
#include <stddef.h>
//manual config start
//此处填写手动配置，DIDE生成配置文件时，不会被修改


#define    CN_RUNMODE_IBOOT                0                //IBOOT模式运行
#define    CN_RUNMODE_BOOTSELF             1                //无须IBOOT，自启动模式APP
#define    CN_RUNMODE_APP                  2                //由IBOOT加载的APP
#define    CFG_RUNMODE                     CN_RUNMODE_APP   //由IBOOT加载的APP
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
//*******************************  Configure loader  ******************************************//
#define CFG_UPDATEIBOOT_EN      false       //
#define CFG_START_APP_IS_VERIFICATION      true       //
#define    CFG_APP_RUNMODE                 CN_DIRECT_RUN      // "APP运行模式",CN_DIRECT_RUN=直接从flash中运行；CN_FORM_FILE=从文件系统加载到内存运行，
#define    CFG_APP_VERIFICATION            VERIFICATION_NULL  // "APP校验方法",
#define CFG_IBOOT_VERSION_SMALL       00        //xx.xx.__APP忽略
#define CFG_IBOOT_VERSION_MEDIUM      00        //xx.__.xxAPP忽略
#define CFG_IBOOT_VERSION_LARGE       01        //__.xx.xxAPP忽略
#define    CFG_IBOOT_UPDATE_NAME           "/yaf2/iboot.bin"  // "待升级iboot默认存储路径"
#define CFG_APP_UPDATE_NAME        "/yaf2/app.bin"            //
#define    CFG_MODULE_ENABLE_LOADER        true
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
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure cpu onchip peripheral lowpower control  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_PERIPHERAL_LOWPOWER_CONTROL  true
//*******************************  Configure STM32 common code  ******************************************//
#define    CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure djybus  ******************************************//
#define    CFG_MODULE_ENABLE_DJYBUS        true
//*******************************  Configure iicbus  ******************************************//
#define    CFG_MODULE_ENABLE_IICBUS        true
//*******************************  Configure cpu onchip iic  ******************************************//
#define    CFG_I2C1_ENABLE                 false              //  "是否配置使用IIC1",
#define    CFG_I2C1_BUF_LEN                128                //  "IIC1缓冲区大小",
#define    CFG_I2C2_ENABLE                 false              //  "是否配置使用IIC2",
#define    CFG_I2C2_BUF_LEN                128                //  "IIC2缓冲区大小",
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_IIC  true
//*******************************  Configure font  ******************************************//
#define CFG_FONT_DEFAULT  "gb2312_song_16"       // "默认字体",字体名在include/font目录中找
#define    CFG_MODULE_ENABLE_FONT          true
//*******************************  Configure graphical kernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024     // "命令缓冲区长度",上层应用（例如gdd）向gkernel传递命令的缓冲区长度（字节数）
#define CFG_USER_REQUEST_DEEP       128     //gkernel向上层请求命令的缓冲区长度（字节数）
#define    CFG_MODULE_ENABLE_GRAPHICAL_KERNEL  true
//*******************************  Configure graphical decorate development  ******************************************//
#define CFG_DESKTOP_WIDTH       0           //桌面尺寸（像素数）宽度0=显示器宽度
#define CFG_DESKTOP_HEIGHT      0           //桌面尺寸（像素数）高度0=显示器高度
#define CFG_DESKTOP_BUF         true        //内存稀少的硬件可不带缓存
#define CFG_DISPLAY_NAME        "DISPLAY_NAME"         //须与bsp中显示器驱动模块配置的显示器名字相同
#define CFG_DESKTOP_NAME        "DESKTOP_NAME"         //
#define CFG_DESKTOP_FORMAT      CN_SYS_PF_RGB565    //桌面窗口像素格式常数在gkernel.h中定义，一般使用与显示器相同颜色
#define CFG_GRAY_BASE_COLOR     CN_COLOR_WHITE      //像素格式设为灰度时才需要设置的“最亮”色可在gkernel.h中找到常用颜色定义
#define CFG_FILL_COLOR          CN_COLOR_BLUE       //创建桌面时的填充色用888格式，可在gkernel.h中找到常用颜色定义
#define    CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT  true
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096     // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         15       // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    15       // "事件类型数限值",事件类型数
#define CFG_IDLESTACK_LIMIT     1024     // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_IDLE_MONITOR_CYCLE  30      //监视IDLE事件持续低于1/16 CPU占比的时间秒数，0=不监视
#define CFG_MAINSTACK_LIMIT     4096     // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLE_WDT_RESET      false   //IDLE事件监视狗叫时，是否复位
#define CFG_OS_TINY             false    // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure misc  ******************************************//
#define    CFG_MODULE_ENABLE_MISC          true
//*******************************  Configure Nls Charset  ******************************************//
#define    CFG_LOCAL_CHARSET               "gb2312"           // "本地字符集",
#define    CFG_MODULE_ENABLE_NLS_CHARSET   true
//*******************************  Configure ascii charset  ******************************************//
#define    CFG_MODULE_ENABLE_ASCII_CHARSET true
//*******************************  Configure ascii dot font  ******************************************//
#define    CFG_ASCII_8X8                   false              // "ASCII_8×8",8*8点阵ascii字体
#define    CFG_ASCII_6X12                  false              // "ASCII_6×12",6*12点阵ascii字体
#define    CFG_ASCII_8X16_SONG             true               // "ASCII_8×16宋体",8*16点阵ascii字体_宋体
#define    CFG_ASCII_8X16_YUAN             false              // "ASCII_8×16圆体",8*16点阵ascii字体_圆体
#define    CFG_ASCII_8X16_KAI              false              // "ASCII_8×16楷体",8*16点阵ascii字体_楷体
#define    CFG_ASCII_8X16_HEI              false              // "ASCII_8×16黑体",8*16点阵ascii字体_黑体
#define    CFG_ASCII_8X16_FANG             false              // "ASCII_8×16仿宋",8*16点阵ascii字体_仿宋
#define    CFG_ASCII_12X24_SONG            false              // "ASCII_12×24宋体",12*24点阵ascii字体_宋体
#define    CFG_ASCII_12X24_YUAN            false              // "ASCII_12×24圆体",12*24点阵ascii字体_圆体
#define    CFG_ASCII_12X24_KAI             false              // "ASCII_12×24楷体",12*24点阵ascii字体_楷体
#define    CFG_ASCII_12X24_HEI             false              // "ASCII_12×24黑体",12*24点阵ascii字体_黑体
#define    CFG_ASCII_12X24_FANG            false              // "ASCII_12×24仿宋",12*24点阵ascii字体_仿宋
#define CFG_ASCII_16X32_SONG  false         //12*24点阵ascii字体_宋体
#define    CFG_ASCII_16X32_YUAN            false              // "ASCII_16×32圆体",16*32点阵ascii字体_圆体
#define    CFG_ASCII_16X32_KAI             false              // "ASCII_16×32楷体",16*32点阵ascii字体_楷体
#define    CFG_ASCII_16X32_HEI             false              // "ASCII_16×32黑体",16*32点阵ascii字体_黑体
#define    CFG_ASCII_16X32_FANG            false              // "ASCII_16×32仿宋",16*32点阵ascii字体_仿宋
#define    CFG_MODULE_ENABLE_ASCII_DOT_FONT  true
//*******************************  Configure time  ******************************************//
#define    CFG_LOCAL_TIMEZONE              8                  // "时区",北京时间是东8区
#define    CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure debug information  ******************************************//
#define    CFG_MODULE_ENABLE_DEBUG_INFORMATION  true
//*******************************  Configure flash  ******************************************//
#define    CFG_MODULE_ENABLE_FLASH         true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true   // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure LCD driver ili9325  ******************************************//
#define    CFG_LCD_XSIZE                   240               //
#define    CFG_LCD_YSIZE                   320               //
#define    CFG_ILI9325_DISPLAY_NAME        "lcdili9325"      //配置液晶显示的名称
#define    CFG_ILI9325_HEAP_NAME           "extram"          //配置液晶驱动所使用的堆名称
#define    CFG_MODULE_ENABLE_LCD_DRIVER_ILI9325  true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure human machine interface  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2        // "输入设备数量上限",人机交互输入设备数量，如键盘、鼠标等
#define    CFG_MODULE_ENABLE_HUMAN_MACHINE_INTERFACE  true
//*******************************  Configure touch  ******************************************//
#define    CFG_MODULE_ENABLE_TOUCH         true
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
#define    CFG_SHELL_STACK                 0x1000             // "执行shell命令的栈尺寸",
#define    CFG_ADD_ROUTINE_SHELL           true               // "是否添加常规shell命令",
#define    CFG_ADD_EXPAND_SHELL            true               // "是否添加拓展shell命令",
#define    CFG_ADD_GLOBAL_FUN              false              // "添加全局函数到shell",
#define    CFG_SHOW_ADD_SHEELL             true               // "显示在编译窗口添加的shell命令",
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure stmpe811  ******************************************//
#define    CFG_CRT_CLK_FRE                 (100*1000)         //  "IIC总线速度"，总线速度，单位Hz
#define    CFG_STMPE811_DEVADDR            0x41               //  "设备地址"，IIC总线上的设备地址
#define    CFG_STMPE811_TOUCH_DEV_NAME     "TOUCH_STMPE811"   //  "触摸屏名称",配置触摸屏名称
#define CFG_STMPE811_DISPLAY_NAME        "LCD_DESKTOP_STMPE811"      //配置触摸屏所在显示器的名称
#define    CFG_MODULE_ENABLE_STMPE811      true
//*******************************  Core Clock  ******************************************//
#define  CFG_CORE_MCLK                   (168.0*Mhz)       //主频，内核要用，必须定义
//*******************************  DjyosProduct Configuration  ******************************************//
#define    PRODUCT_MANUFACTURER_NAME       ""                //厂商名称
#define    PRODUCT_PRODUCT_CLASSIFY        ""                //产品分类
#define    PRODUCT_PRODUCT_MODEL           ""                //产品型号
#define    PRODUCT_VERSION_LARGE           0                 //版本号,__.xx.xx
#define    PRODUCT_VERSION_MEDIUM          0                 //版本号,xx.__.xx
#define    PRODUCT_VERSION_SMALL           0                 //版本号,xx.xx.__
#define    PRODUCT_PRODUCT_MODEL_CODE      ""                //产品型号编码
#define    PRODUCT_BRANCH                  ""                //产品分支
#define    PRODUCT_PASSWORD                ""                //产品秘钥
#define    PRODUCT_OTA_ADDRESS             ""                //OTA服务器地址
#define    PRODUCT_BOARD_TYPE              ""                //板件类型
#define    PRODUCT_CPU_TYPE                ""                //CPU类型


#endif


#define    CN_RUNMODE_IBOOT                0                //IBOOT模式运行
#define    CN_RUNMODE_BOOTSELF             1                //无须IBOOT，自启动模式APP
#define    CN_RUNMODE_APP                  2                //由IBOOT加载的APP
#define    CFG_RUNMODE                     CN_RUNMODE_APP   //由IBOOT加载的APP
//*******************************  Configure ascii charset  ******************************************//
#define    CFG_MODULE_ENABLE_ASCII_CHARSET true
//*******************************  Configure ascii dot font  ******************************************//
#define CFG_ASCII_8X8         false         //8*8点阵ascii字体
#define CFG_ASCII_6X12        false         //6*12点阵ascii字体
#define CFG_ASCII_8X16_SONG   true          //8*16点阵ascii字体_宋体
#define CFG_ASCII_8X16_YUAN   false         //8*16点阵ascii字体_圆体
#define CFG_ASCII_8X16_KAI    false         //8*16点阵ascii字体_楷体
#define CFG_ASCII_8X16_HEI    false         //8*16点阵ascii字体_黑体
#define CFG_ASCII_8X16_FANG   false         //8*16点阵ascii字体_仿宋
#define CFG_ASCII_12X24_SONG  false         //12*24点阵ascii字体_宋体
#define CFG_ASCII_12X24_YUAN  false         //12*24点阵ascii字体_圆体
#define CFG_ASCII_12X24_KAI   false         //12*24点阵ascii字体_楷体
#define CFG_ASCII_12X24_HEI   false         //12*24点阵ascii字体_黑体
#define CFG_ASCII_12X24_FANG  false         //12*24点阵ascii字体_仿宋
#define CFG_ASCII_16X32_SONG  false         //12*24点阵ascii字体_宋体
#define CFG_ASCII_16X32_YUAN  false         //16*32点阵ascii字体_圆体
#define CFG_ASCII_16X32_KAI   false         //16*32点阵ascii字体_楷体
#define CFG_ASCII_16X32_HEI   false         //16*32点阵ascii字体_黑体
#define CFG_ASCII_16X32_FANG  false         //16*32点阵ascii字体_仿宋
#define    CFG_MODULE_ENABLE_ASCII_DOT_FONT  true
//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure cpu onchip iic  ******************************************//
#define CFG_I2C1_ENABLE          false         //
#define CFG_I2C1_BUF_LEN         128           //
#define CFG_I2C2_ENABLE          false         //
#define CFG_I2C2_BUF_LEN         128           //
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_IIC  true
//*******************************  Configure cpu onchip peripheral lowpower control  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_PERIPHERAL_LOWPOWER_CONTROL  true
//*******************************  Configure cpu onchip uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN            32                 //
#define CFG_UART1_RECVBUF_LEN            32                 //
#define CFG_UART1_DMABUF_LEN             32                 //
#define CFG_UART2_SENDBUF_LEN            32                 //
#define CFG_UART2_RECVBUF_LEN            32                 //
#define CFG_UART2_DMABUF_LEN             32                 //
#define CFG_UART3_SENDBUF_LEN            32                 //
#define CFG_UART3_RECVBUF_LEN            32                 //
#define CFG_UART3_DMABUF_LEN             32                 //
#define CFG_UART4_SENDBUF_LEN            32                  //
#define CFG_UART4_RECVBUF_LEN            32                  //
#define CFG_UART5_SENDBUF_LEN            32                   //
#define CFG_UART5_RECVBUF_LEN            32                   //
#define CFG_UART1_ENABLE                 true                 //
#define CFG_UART2_ENABLE                 false                //
#define CFG_UART3_ENABLE                 false                //
#define CFG_UART4_ENABLE                 false                //
#define CFG_UART5_ENABLE                 false                //
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_UART  true
//*******************************  Configure debug information  ******************************************//
#define    CFG_MODULE_ENABLE_DEBUG_INFORMATION  true
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //定义设备数量
#define    CFG_MODULE_ENABLE_DEVICE_FILE_SYSTEM  true
//*******************************  Configure djybus  ******************************************//
#define    CFG_MODULE_ENABLE_DJYBUS        true
//*******************************  Configure flash  ******************************************//
#define    CFG_MODULE_ENABLE_FLASH         true
//*******************************  Configure font  ******************************************//
#define CFG_FONT_DEFAULT  "gb2312_song_16"      //字体名在include/font目录中找
#define    CFG_MODULE_ENABLE_FONT          true
//*******************************  Configure graphical decorate development  ******************************************//
#define CFG_DESKTOP_WIDTH       0           //桌面尺寸（像素数）宽度0=显示器宽度
#define CFG_DESKTOP_HEIGHT      0           //桌面尺寸（像素数）高度0=显示器高度
#define CFG_DESKTOP_BUF         true        //内存稀少的硬件可不带缓存
#define CFG_DISPLAY_NAME        "DISPLAY_NAME"         //须与bsp中显示器驱动模块配置的显示器名字相同
#define CFG_DESKTOP_NAME        "DESKTOP_NAME"         //
#define CFG_DESKTOP_FORMAT      CN_SYS_PF_RGB565    //桌面窗口像素格式常数在gkernel.h中定义，一般使用与显示器相同颜色
#define CFG_GRAY_BASE_COLOR     CN_COLOR_WHITE      //像素格式设为灰度时才需要设置的“最亮”色可在gkernel.h中找到常用颜色定义
#define CFG_FILL_COLOR          CN_COLOR_BLUE       //创建桌面时的填充色用888格式，可在gkernel.h中找到常用颜色定义
#define    CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT  true
//*******************************  Configure graphical kernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024    //上层应用（例如gdd）向gkernel传递命令的缓冲区长度（字节数）
#define CFG_USER_REQUEST_DEEP       128     //gkernel向上层请求命令的缓冲区长度（字节数）
#define    CFG_MODULE_ENABLE_GRAPHICAL_KERNEL  true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //即使选false也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure human machine interface  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2       //人机交互输入设备数量如键盘、鼠标等
#define    CFG_MODULE_ENABLE_HUMAN_MACHINE_INTERFACE  true
//*******************************  Configure iicbus  ******************************************//
#define    CFG_MODULE_ENABLE_IICBUS        true
//*******************************  Configure int  ******************************************//
#define    CFG_MODULE_ENABLE_INT           true
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //定义初始化过程使用的栈空间一般按默认值就可以了
#define CFG_EVENT_LIMIT         15      //事件数量
#define CFG_EVENT_TYPE_LIMIT    15      //事件类型数
#define CFG_IDLESTACK_LIMIT     1024    //IDLE事件处理函数运行的栈尺寸一般按默认值就可以了
#define CFG_IDLE_MONITOR_CYCLE  30      //监视IDLE事件持续低于1/16 CPU占比的时间秒数，0=不监视
#define CFG_MAINSTACK_LIMIT     4096    //main函数运行所需的栈尺寸
#define CFG_IDLE_WDT_RESET      false   //IDLE事件监视狗叫时，是否复位
#define CFG_OS_TINY             false   //true=用于资源紧缺的场合内核会裁剪掉：事件类型名字，事件处理时间统计。
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //用完会自动扩充
#define CFG_HANDLE_LIMIT        8   //用完会自动扩充
#define    CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure loader  ******************************************//
#define CFG_UPDATEIBOOT_EN      false       //
#define CFG_START_APP_IS_VERIFICATION      true       //
#define  CFG_APP_RUNMODE  CN_DIRECT_RUN     //CN_DIRECT_RUN=直接从flash中运行；CN_FORM_FILE=从文件系统加载到内存运行
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL   //
#define CFG_IBOOT_VERSION_SMALL       00        //xx.xx.__APP忽略
#define CFG_IBOOT_VERSION_MEDIUM      00        //xx.__.xxAPP忽略
#define CFG_IBOOT_VERSION_LARGE       01        //__.xx.xxAPP忽略
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"           //
#define CFG_APP_UPDATE_NAME        "/yaf2/app.bin"            //
#define    CFG_MODULE_ENABLE_LOADER        true
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //定义锁的数量包含信号量和互斥量
#define    CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
#define    CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure message queue  ******************************************//
#define    CFG_MODULE_ENABLE_MESSAGE_QUEUE true
//*******************************  Configure misc  ******************************************//
#define    CFG_MODULE_ENABLE_MISC          true
//*******************************  Configure multiplex  ******************************************//
#define    CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure Nls Charset  ******************************************//
#define CFG_LOCAL_CHARSET       "gb2312"        //
#define    CFG_MODULE_ENABLE_NLS_CHARSET   true
//*******************************  Configure ring buffer and line buffer  ******************************************//
#define    CFG_MODULE_ENABLE_RING_BUFFER_AND_LINE_BUFFER  true
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000      //
#define CFG_ADD_ROUTINE_SHELL      true        //
#define CFG_ADD_EXPAND_SHELL       true        //
#define CFG_ADD_GLOBAL_FUN         false       //
#define CFG_SHOW_ADD_SHEELL        true        //
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true         //
#define CFG_STDIO_STDOUT_FOLLOW    true         //
#define CFG_STDIO_STDERR_FOLLOW    true         //
#define CFG_STDIO_FLOAT_PRINT      true         //
#define CFG_STDIO_STDIOFILE        true         //
#define CFG_STDIO_IN_NAME              "/dev/UART1"    //
#define CFG_STDIO_OUT_NAME             "/dev/UART1"    //
#define CFG_STDIO_ERR_NAME             "/dev/UART1"    //
#define    CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure STM32 common code  ******************************************//
#define    CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //北京时间是东8区
#define    CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure touch  ******************************************//
#define    CFG_MODULE_ENABLE_TOUCH         true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure LCD driver ili9325  ******************************************//
#define CFG_LCD_XSIZE   240             //
#define CFG_LCD_YSIZE   320             //
#define CFG_ILI9325_DISPLAY_NAME              "lcdili9325"    //配置液晶显示的名称
#define CFG_ILI9325_HEAP_NAME                 "extram"        //配置液晶驱动所使用的堆名称
#define    CFG_MODULE_ENABLE_LCD_DRIVER_ILI9325  true
//*******************************  Configure stmpe811  ******************************************//
#define CFG_CRT_CLK_FRE                 (100*1000)      //总线速度，单位Hz
#define CFG_STMPE811_DEVADDR            0x41            //IIC总线上的设备地址
#define CFG_STMPE811_TOUCH_DEV_NAME   "TOUCH_STMPE811"       //配置触摸屏名称
#define CFG_STMPE811_DISPLAY_NAME        "LCD_DESKTOP_STMPE811"      //配置触摸屏所在显示器的名称
#define    CFG_MODULE_ENABLE_STMPE811      true
//*******************************  Core Clock  ******************************************//
#define  CFG_CORE_MCLK                   (168.0*Mhz)       //主频，内核要用，必须定义
//*******************************  DjyosProduct Configuration  ******************************************//
#define    PRODUCT_MANUFACTURER_NAME       ""                //厂商名称
#define    PRODUCT_PRODUCT_CLASSIFY        ""                //产品分类
#define    PRODUCT_PRODUCT_MODEL           ""                //产品型号
#define    PRODUCT_VERSION_LARGE           0                 //版本号,__.xx.xx
#define    PRODUCT_VERSION_MEDIUM          0                 //版本号,xx.__.xx
#define    PRODUCT_VERSION_SMALL           0                 //版本号,xx.xx.__
#define    PRODUCT_PRODUCT_MODEL_CODE      ""                //产品型号编码
#define    PRODUCT_BRANCH                  ""                //产品分支
#define    PRODUCT_PASSWORD                ""                //产品秘钥
#define    PRODUCT_OTA_ADDRESS             ""                //OTA服务器地址
#define    PRODUCT_BOARD_TYPE              ""                //板件类型
#define    PRODUCT_CPU_TYPE                ""                //CPU类型


#endif


#define    CN_RUNMODE_IBOOT                0                //IBOOT模式运行
#define    CN_RUNMODE_BOOTSELF             1                //无须IBOOT，自启动模式APP
#define    CN_RUNMODE_APP                  2                //由IBOOT加载的APP
#define    CFG_RUNMODE                     CN_RUNMODE_APP   //由IBOOT加载的APP
//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10        // "设备数量",定义设备数量
#define    CFG_MODULE_ENABLE_DEVICE_FILE_SYSTEM  true
//*******************************  Configure int  ******************************************//
#define    CFG_MODULE_ENABLE_INT           true
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8     // "对象数初始值"，用完会自动扩充
#define CFG_HANDLE_LIMIT        8     // "句柄数初始值"，用完会自动扩充
#define    CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure loader  ******************************************//
#define CFG_UPDATEIBOOT_EN      false        // "是否支持在线更新Iboot"，
#define CFG_START_APP_IS_VERIFICATION      true        // "启动app时是否执行校验功能"，
#define    CFG_APP_RUNMODE                 CN_DIRECT_RUN       // "APP运行模式",CN_DIRECT_RUN=直接从flash中运行；CN_FORM_FILE=从文件系统加载到内存运行，
#define    CFG_APP_VERIFICATION            VERIFICATION_NULL   // "APP校验方法",
#define CFG_IBOOT_VERSION_SMALL       00         // "Iboot版本号:低",xx.xx.__，APP忽略
#define CFG_IBOOT_VERSION_MEDIUM      00         // "Iboot版本号:中",xx.__.xx，APP忽略
#define CFG_IBOOT_VERSION_LARGE       01         // "Iboot版本号:高",__.xx.xx，APP忽略
#define    CFG_IBOOT_UPDATE_NAME           "/yaf2/iboot.bin"   // "待升级iboot默认存储路径"
#define CFG_APP_UPDATE_NAME        "/yaf2/app.bin"             // "待升级app默认存储路径"
#define    CFG_MODULE_ENABLE_LOADER        true
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40        // "锁的数量",定义锁的数量，包含信号量和互斥量
#define    CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10        // "内存池数量限值",
#define    CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure message queue  ******************************************//
#define    CFG_MODULE_ENABLE_MESSAGE_QUEUE true
//*******************************  Configure multiplex  ******************************************//
#define    CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure ring buffer and line buffer  ******************************************//
#define    CFG_MODULE_ENABLE_RING_BUFFER_AND_LINE_BUFFER  true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure cpu onchip peripheral lowpower control  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_PERIPHERAL_LOWPOWER_CONTROL  true
//*******************************  Configure STM32 common code  ******************************************//
#define    CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure djybus  ******************************************//
#define    CFG_MODULE_ENABLE_DJYBUS        true
//*******************************  Configure iicbus  ******************************************//
#define    CFG_MODULE_ENABLE_IICBUS        true
//*******************************  Configure cpu onchip iic  ******************************************//
#define    CFG_I2C1_ENABLE                 false               //  "是否配置使用IIC1",
#define    CFG_I2C1_BUF_LEN                128                 //  "IIC1缓冲区大小",
#define    CFG_I2C2_ENABLE                 false               //  "是否配置使用IIC2",
#define    CFG_I2C2_BUF_LEN                128                 //  "IIC2缓冲区大小",
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_IIC  true
//*******************************  Configure font  ******************************************//
#define CFG_FONT_DEFAULT  "gb2312_song_16"        // "默认字体",字体名在include/font目录中找
#define    CFG_MODULE_ENABLE_FONT          true
//*******************************  Configure graphical kernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024      // "命令缓冲区长度",上层应用（例如gdd）向gkernel传递命令的缓冲区长度（字节数）
#define CFG_USER_REQUEST_DEEP       128      // "请求缓冲区长度",gkernel向上层请求命令的缓冲区长度（字节数）
#define    CFG_MODULE_ENABLE_GRAPHICAL_KERNEL  true
//*******************************  Configure graphical decorate development  ******************************************//
#define CFG_DESKTOP_WIDTH       0            // "桌面宽度",桌面尺寸（像素数）宽度，0=显示器宽度
#define CFG_DESKTOP_HEIGHT      0            // "桌面高度",桌面尺寸（像素数）高度，0=显示器高度
#define CFG_DESKTOP_BUF         true         // "桌面窗口是否带缓存"，内存稀少的硬件可不带缓存
#define CFG_DISPLAY_NAME        "DISPLAY_NAME"          // "显示器名",须与bsp中显示器驱动模块配置的显示器名字相同
#define CFG_DESKTOP_NAME        "DESKTOP_NAME"          // "给桌面起个名字"，
#define CFG_DESKTOP_FORMAT      CN_SYS_PF_RGB565     // "像素格式",桌面窗口像素格式，常数在gkernel.h中定义，一般使用与显示器相同颜色
#define CFG_GRAY_BASE_COLOR     CN_COLOR_WHITE       // "灰度基色",像素格式设为灰度时才需要设置的“最亮”色，可在gkernel.h中找到常用颜色定义
#define CFG_FILL_COLOR          CN_COLOR_BLUE        // "填充色",创建桌面时的填充色，用888格式，可在gkernel.h中找到常用颜色定义
#define    CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT  true
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096      // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         15        // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    15        // "事件类型数限值",事件类型数
#define CFG_IDLESTACK_LIMIT     1024      // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_IDLE_MONITOR_CYCLE  30       // "IDLE监视周期",监视IDLE事件持续低于1/16 CPU占比的时间，秒数，0=不监视
#define CFG_MAINSTACK_LIMIT     4096      // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLE_WDT_RESET      false    // "IDLE监视狗叫复位使能"，IDLE事件监视狗叫时，是否复位
#define CFG_OS_TINY             false     // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure misc  ******************************************//
#define    CFG_MODULE_ENABLE_MISC          true
//*******************************  Configure Nls Charset  ******************************************//
#define    CFG_LOCAL_CHARSET               "gb2312"            // "本地字符集",
#define    CFG_MODULE_ENABLE_NLS_CHARSET   true
//*******************************  Configure ascii charset  ******************************************//
#define    CFG_MODULE_ENABLE_ASCII_CHARSET true
//*******************************  Configure ascii dot font  ******************************************//
#define    CFG_ASCII_8X8                   false               // "ASCII_8×8",8*8点阵ascii字体
#define    CFG_ASCII_6X12                  false               // "ASCII_6×12",6*12点阵ascii字体
#define    CFG_ASCII_8X16_SONG             true                // "ASCII_8×16宋体",8*16点阵ascii字体_宋体
#define    CFG_ASCII_8X16_YUAN             false               // "ASCII_8×16圆体",8*16点阵ascii字体_圆体
#define    CFG_ASCII_8X16_KAI              false               // "ASCII_8×16楷体",8*16点阵ascii字体_楷体
#define    CFG_ASCII_8X16_HEI              false               // "ASCII_8×16黑体",8*16点阵ascii字体_黑体
#define    CFG_ASCII_8X16_FANG             false               // "ASCII_8×16仿宋",8*16点阵ascii字体_仿宋
#define    CFG_ASCII_12X24_SONG            false               // "ASCII_12×24宋体",12*24点阵ascii字体_宋体
#define    CFG_ASCII_12X24_YUAN            false               // "ASCII_12×24圆体",12*24点阵ascii字体_圆体
#define    CFG_ASCII_12X24_KAI             false               // "ASCII_12×24楷体",12*24点阵ascii字体_楷体
#define    CFG_ASCII_12X24_HEI             false               // "ASCII_12×24黑体",12*24点阵ascii字体_黑体
#define    CFG_ASCII_12X24_FANG            false               // "ASCII_12×24仿宋",12*24点阵ascii字体_仿宋
#define CFG_ASCII_16X32_SONG  false          // "ASCII_16X32宋体",12*24点阵ascii字体_宋体
#define    CFG_ASCII_16X32_YUAN            false               // "ASCII_16×32圆体",16*32点阵ascii字体_圆体
#define    CFG_ASCII_16X32_KAI             false               // "ASCII_16×32楷体",16*32点阵ascii字体_楷体
#define    CFG_ASCII_16X32_HEI             false               // "ASCII_16×32黑体",16*32点阵ascii字体_黑体
#define    CFG_ASCII_16X32_FANG            false               // "ASCII_16×32仿宋",16*32点阵ascii字体_仿宋
#define    CFG_MODULE_ENABLE_ASCII_DOT_FONT  true
//*******************************  Configure time  ******************************************//
#define    CFG_LOCAL_TIMEZONE              8                   // "时区",北京时间是东8区
#define    CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure debug information  ******************************************//
#define    CFG_MODULE_ENABLE_DEBUG_INFORMATION  true
//*******************************  Configure flash  ******************************************//
#define    CFG_MODULE_ENABLE_FLASH         true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true    // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure LCD driver ili9325  ******************************************//
#define    CFG_LCD_XSIZE                   240                //  "LCD宽度",
#define    CFG_LCD_YSIZE                   320                //  "LCD高度",
#define CFG_LCD_XSIZE_UM   36500            //
#define CFG_LCD_YSIZE_UM   48600            //
#define    CFG_ILI9325_DISPLAY_NAME        "lcdili9325"       //  "显示器名称",配置液晶显示的名称
#define    CFG_ILI9325_HEAP_NAME           "extram"           //  "驱动使用堆名",配置液晶驱动所使用的堆名称
#define    CFG_MODULE_ENABLE_LCD_DRIVER_ILI9325  true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure human machine interface  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2         // "输入设备数量上限",人机交互输入设备数量，如键盘、鼠标等
#define    CFG_MODULE_ENABLE_HUMAN_MACHINE_INTERFACE  true
//*******************************  Configure touch  ******************************************//
#define    CFG_MODULE_ENABLE_TOUCH         true
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true           // "是否支持多种输入设备",
#define CFG_STDIO_STDOUT_FOLLOW    true           // "stdout是否跟随stdin",
#define CFG_STDIO_STDERR_FOLLOW    true           // "stderr是否跟随stdin",
#define CFG_STDIO_FLOAT_PRINT      true           // "支持浮点打印"
#define CFG_STDIO_STDIOFILE        true           // "支持标准IO文件"
#define CFG_STDIO_IN_NAME              "/dev/UART1"      // "标准输入设备名",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"      // "标准输出设备名",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"      // "标准err输出设备名",
#define    CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure shell  ******************************************//
#define    CFG_SHELL_STACK                 0x1000              // "执行shell命令的栈尺寸",
#define    CFG_ADD_ROUTINE_SHELL           true                // "是否添加常规shell命令",
#define    CFG_ADD_EXPAND_SHELL            true                // "是否添加拓展shell命令",
#define    CFG_ADD_GLOBAL_FUN              false               // "添加全局函数到shell",
#define    CFG_SHOW_ADD_SHEELL             true                // "显示在编译窗口添加的shell命令",
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure stmpe811  ******************************************//
#define    CFG_CRT_CLK_FRE                 (100*1000)          //  "IIC总线速度"，总线速度，单位Hz
#define    CFG_STMPE811_DEVADDR            0x41                //  "设备地址"，IIC总线上的设备地址
#define    CFG_STMPE811_TOUCH_DEV_NAME     "TOUCH_STMPE811"    //  "触摸屏名称",配置触摸屏名称
#define CFG_STMPE811_DISPLAY_NAME        "LCD_DESKTOP_STMPE811"       //  "显示器名称",配置触摸屏所在显示器的名称
#define    CFG_MODULE_ENABLE_STMPE811      true
//*******************************  Core Clock  ******************************************//
#define  CFG_CORE_MCLK                   (168.0*Mhz)       //主频，内核要用，必须定义
//*******************************  DjyosProduct Configuration  ******************************************//
#define    PRODUCT_MANUFACTURER_NAME       ""                //厂商名称
#define    PRODUCT_PRODUCT_CLASSIFY        ""                //产品分类
#define    PRODUCT_PRODUCT_MODEL           ""                //产品型号
#define    PRODUCT_VERSION_LARGE           0                 //版本号,__.xx.xx
#define    PRODUCT_VERSION_MEDIUM          0                 //版本号,xx.__.xx
#define    PRODUCT_VERSION_SMALL           0                 //版本号,xx.xx.__
#define    PRODUCT_PRODUCT_MODEL_CODE      ""                //产品型号编码
#define    PRODUCT_BRANCH                  ""                //产品分支
#define    PRODUCT_PASSWORD                ""                //产品秘钥
#define    PRODUCT_OTA_ADDRESS             ""                //OTA服务器地址
#define    PRODUCT_BOARD_TYPE              ""                //板件类型
#define    PRODUCT_CPU_TYPE                ""                //CPU类型


#endif


#define    CN_RUNMODE_IBOOT                0                //IBOOT模式运行
#define    CN_RUNMODE_BOOTSELF             1                //无须IBOOT，自启动模式APP
#define    CN_RUNMODE_APP                  2                //由IBOOT加载的APP
#define    CFG_RUNMODE                     CN_RUNMODE_APP   //由IBOOT加载的APP
//*******************************  Configure ascii charset  ******************************************//
#define    CFG_MODULE_ENABLE_ASCII_CHARSET true
//*******************************  Configure ascii dot font  ******************************************//
#define CFG_ASCII_8X8         false         //8*8点阵ascii字体
#define CFG_ASCII_6X12        false         //6*12点阵ascii字体
#define CFG_ASCII_8X16_SONG   true          //8*16点阵ascii字体_宋体
#define CFG_ASCII_8X16_YUAN   false         //8*16点阵ascii字体_圆体
#define CFG_ASCII_8X16_KAI    false         //8*16点阵ascii字体_楷体
#define CFG_ASCII_8X16_HEI    false         //8*16点阵ascii字体_黑体
#define CFG_ASCII_8X16_FANG   false         //8*16点阵ascii字体_仿宋
#define CFG_ASCII_12X24_SONG  false         //12*24点阵ascii字体_宋体
#define CFG_ASCII_12X24_YUAN  false         //12*24点阵ascii字体_圆体
#define CFG_ASCII_12X24_KAI   false         //12*24点阵ascii字体_楷体
#define CFG_ASCII_12X24_HEI   false         //12*24点阵ascii字体_黑体
#define CFG_ASCII_12X24_FANG  false         //12*24点阵ascii字体_仿宋
#define CFG_ASCII_16X32_SONG  false         //12*24点阵ascii字体_宋体
#define CFG_ASCII_16X32_YUAN  false         //16*32点阵ascii字体_圆体
#define CFG_ASCII_16X32_KAI   false         //16*32点阵ascii字体_楷体
#define CFG_ASCII_16X32_HEI   false         //16*32点阵ascii字体_黑体
#define CFG_ASCII_16X32_FANG  false         //16*32点阵ascii字体_仿宋
#define    CFG_MODULE_ENABLE_ASCII_DOT_FONT  true
//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure cpu onchip iic  ******************************************//
#define CFG_I2C1_ENABLE          false         //
#define CFG_I2C1_BUF_LEN         128           //
#define CFG_I2C2_ENABLE          false         //
#define CFG_I2C2_BUF_LEN         128           //
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_IIC  true
//*******************************  Configure cpu onchip peripheral lowpower control  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_PERIPHERAL_LOWPOWER_CONTROL  true
//*******************************  Configure cpu onchip uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN            32                 //
#define CFG_UART1_RECVBUF_LEN            32                 //
#define CFG_UART1_DMABUF_LEN             32                 //
#define CFG_UART2_SENDBUF_LEN            32                 //
#define CFG_UART2_RECVBUF_LEN            32                 //
#define CFG_UART2_DMABUF_LEN             32                 //
#define CFG_UART3_SENDBUF_LEN            32                 //
#define CFG_UART3_RECVBUF_LEN            32                 //
#define CFG_UART3_DMABUF_LEN             32                 //
#define CFG_UART4_SENDBUF_LEN            32                  //
#define CFG_UART4_RECVBUF_LEN            32                  //
#define CFG_UART5_SENDBUF_LEN            32                   //
#define CFG_UART5_RECVBUF_LEN            32                   //
#define CFG_UART1_ENABLE                 true                 //
#define CFG_UART2_ENABLE                 false                //
#define CFG_UART3_ENABLE                 false                //
#define CFG_UART4_ENABLE                 false                //
#define CFG_UART5_ENABLE                 false                //
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_UART  true
//*******************************  Configure debug information  ******************************************//
#define    CFG_MODULE_ENABLE_DEBUG_INFORMATION  true
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //定义设备数量
#define    CFG_MODULE_ENABLE_DEVICE_FILE_SYSTEM  true
//*******************************  Configure djybus  ******************************************//
#define    CFG_MODULE_ENABLE_DJYBUS        true
//*******************************  Configure flash  ******************************************//
#define    CFG_MODULE_ENABLE_FLASH         true
//*******************************  Configure font  ******************************************//
#define CFG_FONT_DEFAULT  "gb2312_song_16"      //字体名在include/font目录中找
#define    CFG_MODULE_ENABLE_FONT          true
//*******************************  Configure graphical decorate development  ******************************************//
#define CFG_DESKTOP_WIDTH       0           //桌面尺寸（像素数）宽度0=显示器宽度
#define CFG_DESKTOP_HEIGHT      0           //桌面尺寸（像素数）高度0=显示器高度
#define CFG_DESKTOP_BUF         true        //内存稀少的硬件可不带缓存
#define CFG_DISPLAY_NAME        "DISPLAY_NAME"         //须与bsp中显示器驱动模块配置的显示器名字相同
#define CFG_DESKTOP_NAME        "DESKTOP_NAME"         //
#define CFG_DESKTOP_FORMAT      CN_SYS_PF_RGB565    //桌面窗口像素格式常数在gkernel.h中定义，一般使用与显示器相同颜色
#define CFG_GRAY_BASE_COLOR     CN_COLOR_WHITE      //像素格式设为灰度时才需要设置的“最亮”色可在gkernel.h中找到常用颜色定义
#define CFG_FILL_COLOR          CN_COLOR_BLUE       //创建桌面时的填充色用888格式，可在gkernel.h中找到常用颜色定义
#define    CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT  true
//*******************************  Configure graphical kernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024    //上层应用（例如gdd）向gkernel传递命令的缓冲区长度（字节数）
#define CFG_USER_REQUEST_DEEP       128     //gkernel向上层请求命令的缓冲区长度（字节数）
#define    CFG_MODULE_ENABLE_GRAPHICAL_KERNEL  true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //即使选false也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure human machine interface  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2       //人机交互输入设备数量如键盘、鼠标等
#define    CFG_MODULE_ENABLE_HUMAN_MACHINE_INTERFACE  true
//*******************************  Configure iicbus  ******************************************//
#define    CFG_MODULE_ENABLE_IICBUS        true
//*******************************  Configure int  ******************************************//
#define    CFG_MODULE_ENABLE_INT           true
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //定义初始化过程使用的栈空间一般按默认值就可以了
#define CFG_EVENT_LIMIT         15      //事件数量
#define CFG_EVENT_TYPE_LIMIT    15      //事件类型数
#define CFG_IDLESTACK_LIMIT     1024    //IDLE事件处理函数运行的栈尺寸一般按默认值就可以了
#define CFG_IDLE_MONITOR_CYCLE  30      //监视IDLE事件持续低于1/16 CPU占比的时间秒数，0=不监视
#define CFG_MAINSTACK_LIMIT     4096    //main函数运行所需的栈尺寸
#define CFG_IDLE_WDT_RESET      false   //IDLE事件监视狗叫时，是否复位
#define CFG_OS_TINY             false   //true=用于资源紧缺的场合内核会裁剪掉：事件类型名字，事件处理时间统计。
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //用完会自动扩充
#define CFG_HANDLE_LIMIT        8   //用完会自动扩充
#define    CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure loader  ******************************************//
#define CFG_UPDATEIBOOT_EN      false       //
#define CFG_START_APP_IS_VERIFICATION      true       //
#define  CFG_APP_RUNMODE  CN_DIRECT_RUN     //CN_DIRECT_RUN=直接从flash中运行；CN_FORM_FILE=从文件系统加载到内存运行
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL   //
#define CFG_IBOOT_VERSION_SMALL       00        //xx.xx.__APP忽略
#define CFG_IBOOT_VERSION_MEDIUM      00        //xx.__.xxAPP忽略
#define CFG_IBOOT_VERSION_LARGE       01        //__.xx.xxAPP忽略
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"           //
#define CFG_APP_UPDATE_NAME        "/yaf2/app.bin"            //
#define    CFG_MODULE_ENABLE_LOADER        true
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //定义锁的数量包含信号量和互斥量
#define    CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
#define    CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure message queue  ******************************************//
#define    CFG_MODULE_ENABLE_MESSAGE_QUEUE true
//*******************************  Configure misc  ******************************************//
#define    CFG_MODULE_ENABLE_MISC          true
//*******************************  Configure multiplex  ******************************************//
#define    CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure Nls Charset  ******************************************//
#define CFG_LOCAL_CHARSET       "gb2312"        //
#define    CFG_MODULE_ENABLE_NLS_CHARSET   true
//*******************************  Configure ring buffer and line buffer  ******************************************//
#define    CFG_MODULE_ENABLE_RING_BUFFER_AND_LINE_BUFFER  true
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000      //
#define CFG_ADD_ROUTINE_SHELL      true        //
#define CFG_ADD_EXPAND_SHELL       true        //
#define CFG_ADD_GLOBAL_FUN         false       //
#define CFG_SHOW_ADD_SHEELL        true        //
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true         //
#define CFG_STDIO_STDOUT_FOLLOW    true         //
#define CFG_STDIO_STDERR_FOLLOW    true         //
#define CFG_STDIO_FLOAT_PRINT      true         //
#define CFG_STDIO_STDIOFILE        true         //
#define CFG_STDIO_IN_NAME              "/dev/UART1"    //
#define CFG_STDIO_OUT_NAME             "/dev/UART1"    //
#define CFG_STDIO_ERR_NAME             "/dev/UART1"    //
#define    CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure STM32 common code  ******************************************//
#define    CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //北京时间是东8区
#define    CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure touch  ******************************************//
#define    CFG_MODULE_ENABLE_TOUCH         true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure LCD driver ili9325  ******************************************//
#define CFG_LCD_XSIZE   240             //
#define CFG_LCD_YSIZE   320             //
#define CFG_LCD_XSIZE_UM   36500            //
#define CFG_LCD_YSIZE_UM   48600            //
#define CFG_ILI9325_DISPLAY_NAME              "lcdili9325"    //配置液晶显示的名称
#define CFG_ILI9325_HEAP_NAME                 "extram"        //配置液晶驱动所使用的堆名称
#define    CFG_MODULE_ENABLE_LCD_DRIVER_ILI9325  true
//*******************************  Configure stmpe811  ******************************************//
#define CFG_CRT_CLK_FRE                 (100*1000)      //总线速度，单位Hz
#define CFG_STMPE811_DEVADDR            0x41            //IIC总线上的设备地址
#define CFG_STMPE811_TOUCH_DEV_NAME   "TOUCH_STMPE811"       //配置触摸屏名称
#define CFG_STMPE811_DISPLAY_NAME        "LCD_DESKTOP_STMPE811"      //配置触摸屏所在显示器的名称
#define    CFG_MODULE_ENABLE_STMPE811      true
//*******************************  Core Clock  ******************************************//
#define  CFG_CORE_MCLK                   (168.0*Mhz)       //主频，内核要用，必须定义
//*******************************  DjyosProduct Configuration  ******************************************//
#define    PRODUCT_MANUFACTURER_NAME       ""                //厂商名称
#define    PRODUCT_PRODUCT_CLASSIFY        ""                //产品分类
#define    PRODUCT_PRODUCT_MODEL           ""                //产品型号
#define    PRODUCT_VERSION_LARGE           0                 //版本号,__.xx.xx
#define    PRODUCT_VERSION_MEDIUM          0                 //版本号,xx.__.xx
#define    PRODUCT_VERSION_SMALL           0                 //版本号,xx.xx.__
#define    PRODUCT_PRODUCT_MODEL_CODE      ""                //产品型号编码
#define    PRODUCT_BRANCH                  ""                //产品分支
#define    PRODUCT_PASSWORD                ""                //产品秘钥
#define    PRODUCT_OTA_ADDRESS             ""                //OTA服务器地址
#define    PRODUCT_BOARD_TYPE              ""                //板件类型
#define    PRODUCT_CPU_TYPE                ""                //CPU类型


#endif


#define    CN_RUNMODE_IBOOT                0                //IBOOT模式运行
#define    CN_RUNMODE_BOOTSELF             1                //无须IBOOT，自启动模式APP
#define    CN_RUNMODE_APP                  2                //由IBOOT加载的APP
#define    CFG_RUNMODE                     CN_RUNMODE_APP   //由IBOOT加载的APP
//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10        // "设备数量",定义设备数量
#define    CFG_MODULE_ENABLE_DEVICE_FILE_SYSTEM  true
//*******************************  Configure int  ******************************************//
#define    CFG_MODULE_ENABLE_INT           true
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8     // "对象数初始值"，用完会自动扩充
#define CFG_HANDLE_LIMIT        8     // "句柄数初始值"，用完会自动扩充
#define    CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure loader  ******************************************//
#define CFG_UPDATEIBOOT_EN      false        // "是否支持在线更新Iboot"，
#define CFG_START_APP_IS_VERIFICATION      true        // "启动app时是否执行校验功能"，
#define    CFG_APP_RUNMODE                 CN_DIRECT_RUN       // "APP运行模式",CN_DIRECT_RUN=直接从flash中运行；CN_FORM_FILE=从文件系统加载到内存运行，
#define    CFG_APP_VERIFICATION            VERIFICATION_NULL   // "APP校验方法",
#define CFG_IBOOT_VERSION_SMALL       00         // "Iboot版本号:低",xx.xx.__，APP忽略
#define CFG_IBOOT_VERSION_MEDIUM      00         // "Iboot版本号:中",xx.__.xx，APP忽略
#define CFG_IBOOT_VERSION_LARGE       01         // "Iboot版本号:高",__.xx.xx，APP忽略
#define    CFG_IBOOT_UPDATE_NAME           "/yaf2/iboot.bin"   // "待升级iboot默认存储路径"
#define CFG_APP_UPDATE_NAME        "/yaf2/app.bin"             // "待升级app默认存储路径"
#define    CFG_MODULE_ENABLE_LOADER        true
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40        // "锁的数量",定义锁的数量，包含信号量和互斥量
#define    CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10        // "内存池数量限值",
#define    CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure message queue  ******************************************//
#define    CFG_MODULE_ENABLE_MESSAGE_QUEUE true
//*******************************  Configure multiplex  ******************************************//
#define    CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure ring buffer and line buffer  ******************************************//
#define    CFG_MODULE_ENABLE_RING_BUFFER_AND_LINE_BUFFER  true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure cpu onchip peripheral lowpower control  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_PERIPHERAL_LOWPOWER_CONTROL  true
//*******************************  Configure STM32 common code  ******************************************//
#define    CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure djybus  ******************************************//
#define    CFG_MODULE_ENABLE_DJYBUS        true
//*******************************  Configure iicbus  ******************************************//
#define    CFG_MODULE_ENABLE_IICBUS        true
//*******************************  Configure cpu onchip iic  ******************************************//
#define    CFG_I2C1_ENABLE                 false               //  "是否配置使用IIC1",
#define    CFG_I2C1_BUF_LEN                128                 //  "IIC1缓冲区大小",
#define    CFG_I2C2_ENABLE                 false               //  "是否配置使用IIC2",
#define    CFG_I2C2_BUF_LEN                128                 //  "IIC2缓冲区大小",
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_IIC  true
//*******************************  Configure font  ******************************************//
#define CFG_FONT_DEFAULT  "gb2312_song_16"        // "默认字体",字体名在include/font目录中找
#define    CFG_MODULE_ENABLE_FONT          true
//*******************************  Configure graphical kernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024      // "命令缓冲区长度",上层应用（例如gdd）向gkernel传递命令的缓冲区长度（字节数）
#define CFG_USER_REQUEST_DEEP       128      // "请求缓冲区长度",gkernel向上层请求命令的缓冲区长度（字节数）
#define    CFG_MODULE_ENABLE_GRAPHICAL_KERNEL  true
//*******************************  Configure graphical decorate development  ******************************************//
#define CFG_DESKTOP_WIDTH       0            // "桌面宽度",桌面尺寸（像素数）宽度，0=显示器宽度
#define CFG_DESKTOP_HEIGHT      0            // "桌面高度",桌面尺寸（像素数）高度，0=显示器高度
#define CFG_DESKTOP_BUF         true         // "桌面窗口是否带缓存"，内存稀少的硬件可不带缓存
#define CFG_DISPLAY_NAME        "DISPLAY_NAME"          // "显示器名",须与bsp中显示器驱动模块配置的显示器名字相同
#define CFG_DESKTOP_NAME        "DESKTOP_NAME"          // "给桌面起个名字"，
#define CFG_DESKTOP_FORMAT      CN_SYS_PF_RGB565     // "像素格式",桌面窗口像素格式，常数在gkernel.h中定义，一般使用与显示器相同颜色
#define CFG_GRAY_BASE_COLOR     CN_COLOR_WHITE       // "灰度基色",像素格式设为灰度时才需要设置的“最亮”色，可在gkernel.h中找到常用颜色定义
#define CFG_FILL_COLOR          CN_COLOR_BLUE        // "填充色",创建桌面时的填充色，用888格式，可在gkernel.h中找到常用颜色定义
#define    CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT  true
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096      // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         15        // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    15        // "事件类型数限值",事件类型数
#define CFG_IDLESTACK_LIMIT     1024      // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_IDLE_MONITOR_CYCLE  30       // "IDLE监视周期",监视IDLE事件持续低于1/16 CPU占比的时间，秒数，0=不监视
#define CFG_MAINSTACK_LIMIT     4096      // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLE_WDT_RESET      false    // "IDLE监视狗叫复位使能"，IDLE事件监视狗叫时，是否复位
#define CFG_OS_TINY             false     // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure misc  ******************************************//
#define    CFG_MODULE_ENABLE_MISC          true
//*******************************  Configure Nls Charset  ******************************************//
#define    CFG_LOCAL_CHARSET               "gb2312"            // "本地字符集",
#define    CFG_MODULE_ENABLE_NLS_CHARSET   true
//*******************************  Configure ascii charset  ******************************************//
#define    CFG_MODULE_ENABLE_ASCII_CHARSET true
//*******************************  Configure ascii dot font  ******************************************//
#define    CFG_ASCII_8X8                   false               // "ASCII_8×8",8*8点阵ascii字体
#define    CFG_ASCII_6X12                  false               // "ASCII_6×12",6*12点阵ascii字体
#define    CFG_ASCII_8X16_SONG             true                // "ASCII_8×16宋体",8*16点阵ascii字体_宋体
#define    CFG_ASCII_8X16_YUAN             false               // "ASCII_8×16圆体",8*16点阵ascii字体_圆体
#define    CFG_ASCII_8X16_KAI              false               // "ASCII_8×16楷体",8*16点阵ascii字体_楷体
#define    CFG_ASCII_8X16_HEI              false               // "ASCII_8×16黑体",8*16点阵ascii字体_黑体
#define    CFG_ASCII_8X16_FANG             false               // "ASCII_8×16仿宋",8*16点阵ascii字体_仿宋
#define    CFG_ASCII_12X24_SONG            false               // "ASCII_12×24宋体",12*24点阵ascii字体_宋体
#define    CFG_ASCII_12X24_YUAN            false               // "ASCII_12×24圆体",12*24点阵ascii字体_圆体
#define    CFG_ASCII_12X24_KAI             false               // "ASCII_12×24楷体",12*24点阵ascii字体_楷体
#define    CFG_ASCII_12X24_HEI             false               // "ASCII_12×24黑体",12*24点阵ascii字体_黑体
#define    CFG_ASCII_12X24_FANG            false               // "ASCII_12×24仿宋",12*24点阵ascii字体_仿宋
#define CFG_ASCII_16X32_SONG  false          // "ASCII_16X32宋体",12*24点阵ascii字体_宋体
#define    CFG_ASCII_16X32_YUAN            false               // "ASCII_16×32圆体",16*32点阵ascii字体_圆体
#define    CFG_ASCII_16X32_KAI             false               // "ASCII_16×32楷体",16*32点阵ascii字体_楷体
#define    CFG_ASCII_16X32_HEI             false               // "ASCII_16×32黑体",16*32点阵ascii字体_黑体
#define    CFG_ASCII_16X32_FANG            false               // "ASCII_16×32仿宋",16*32点阵ascii字体_仿宋
#define    CFG_MODULE_ENABLE_ASCII_DOT_FONT  true
//*******************************  Configure time  ******************************************//
#define    CFG_LOCAL_TIMEZONE              8                   // "时区",北京时间是东8区
#define    CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure debug information  ******************************************//
#define    CFG_MODULE_ENABLE_DEBUG_INFORMATION  true
//*******************************  Configure flash  ******************************************//
#define    CFG_MODULE_ENABLE_FLASH         true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true    // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure LCD driver ili9325  ******************************************//
#define    CFG_LCD_XSIZE                   240                //  "LCD宽度",
#define    CFG_LCD_YSIZE                   320                //  "LCD高度",
#define CFG_LCD_XSIZE_UM   36500            //
#define CFG_LCD_YSIZE_UM   48600            //
#define    CFG_ILI9325_DISPLAY_NAME        "lcdili9325"       //  "显示器名称",配置液晶显示的名称
#define    CFG_ILI9325_HEAP_NAME           "extram"           //  "驱动使用堆名",配置液晶驱动所使用的堆名称
#define    CFG_MODULE_ENABLE_LCD_DRIVER_ILI9325  true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure human machine interface  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2         // "输入设备数量上限",人机交互输入设备数量，如键盘、鼠标等
#define    CFG_MODULE_ENABLE_HUMAN_MACHINE_INTERFACE  true
//*******************************  Configure touch  ******************************************//
#define    CFG_MODULE_ENABLE_TOUCH         true
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true           // "是否支持多种输入设备",
#define CFG_STDIO_STDOUT_FOLLOW    true           // "stdout是否跟随stdin",
#define CFG_STDIO_STDERR_FOLLOW    true           // "stderr是否跟随stdin",
#define CFG_STDIO_FLOAT_PRINT      true           // "支持浮点打印"
#define CFG_STDIO_STDIOFILE        true           // "支持标准IO文件"
#define CFG_STDIO_IN_NAME              "/dev/UART1"      // "标准输入设备名",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"      // "标准输出设备名",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"      // "标准err输出设备名",
#define    CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure shell  ******************************************//
#define    CFG_SHELL_STACK                 0x1000              // "执行shell命令的栈尺寸",
#define    CFG_ADD_ROUTINE_SHELL           true                // "是否添加常规shell命令",
#define    CFG_ADD_EXPAND_SHELL            true                // "是否添加拓展shell命令",
#define    CFG_ADD_GLOBAL_FUN              false               // "添加全局函数到shell",
#define    CFG_SHOW_ADD_SHEELL             true                // "显示在编译窗口添加的shell命令",
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure stmpe811  ******************************************//
#define    CFG_CRT_CLK_FRE                 (100*1000)          //  "IIC总线速度"，总线速度，单位Hz
#define    CFG_STMPE811_DEVADDR            0x41                //  "设备地址"，IIC总线上的设备地址
#define    CFG_STMPE811_TOUCH_DEV_NAME     "TOUCH_STMPE811"    //  "触摸屏名称",配置触摸屏名称
#define CFG_STMPE811_DISPLAY_NAME        "LCD_DESKTOP_STMPE811"       //  "显示器名称",配置触摸屏所在显示器的名称
#define    CFG_MODULE_ENABLE_STMPE811      true
//*******************************  Core Clock  ******************************************//
#define  CFG_CORE_MCLK                   (168.0*Mhz)       //主频，内核要用，必须定义
//*******************************  DjyosProduct Configuration  ******************************************//
#define    PRODUCT_MANUFACTURER_NAME       ""                //厂商名称
#define    PRODUCT_PRODUCT_CLASSIFY        ""                //产品分类
#define    PRODUCT_PRODUCT_MODEL           ""                //产品型号
#define    PRODUCT_VERSION_LARGE           0                 //版本号,__.xx.xx
#define    PRODUCT_VERSION_MEDIUM          0                 //版本号,xx.__.xx
#define    PRODUCT_VERSION_SMALL           0                 //版本号,xx.xx.__
#define    PRODUCT_PRODUCT_MODEL_CODE      ""                //产品型号编码
#define    PRODUCT_BRANCH                  ""                //产品分支
#define    PRODUCT_PASSWORD                ""                //产品秘钥
#define    PRODUCT_OTA_ADDRESS             ""                //OTA服务器地址
#define    PRODUCT_BOARD_TYPE              ""                //板件类型
#define    PRODUCT_CPU_TYPE                ""                //CPU类型


#endif


#define    CN_RUNMODE_IBOOT                0                //IBOOT模式运行
#define    CN_RUNMODE_BOOTSELF             1                //无须IBOOT，自启动模式APP
#define    CN_RUNMODE_APP                  2                //由IBOOT加载的APP
#define    CFG_RUNMODE                     CN_RUNMODE_APP   //由IBOOT加载的APP
//*******************************  Configure ascii charset  ******************************************//
#define    CFG_MODULE_ENABLE_ASCII_CHARSET true
//*******************************  Configure ascii dot font  ******************************************//
#define CFG_ASCII_8X8         false         //8*8点阵ascii字体
#define CFG_ASCII_6X12        false         //6*12点阵ascii字体
#define CFG_ASCII_8X16_SONG   true          //8*16点阵ascii字体_宋体
#define CFG_ASCII_8X16_YUAN   false         //8*16点阵ascii字体_圆体
#define CFG_ASCII_8X16_KAI    false         //8*16点阵ascii字体_楷体
#define CFG_ASCII_8X16_HEI    false         //8*16点阵ascii字体_黑体
#define CFG_ASCII_8X16_FANG   false         //8*16点阵ascii字体_仿宋
#define CFG_ASCII_12X24_SONG  false         //12*24点阵ascii字体_宋体
#define CFG_ASCII_12X24_YUAN  false         //12*24点阵ascii字体_圆体
#define CFG_ASCII_12X24_KAI   false         //12*24点阵ascii字体_楷体
#define CFG_ASCII_12X24_HEI   false         //12*24点阵ascii字体_黑体
#define CFG_ASCII_12X24_FANG  false         //12*24点阵ascii字体_仿宋
#define CFG_ASCII_16X32_SONG  false         //12*24点阵ascii字体_宋体
#define CFG_ASCII_16X32_YUAN  false         //16*32点阵ascii字体_圆体
#define CFG_ASCII_16X32_KAI   false         //16*32点阵ascii字体_楷体
#define CFG_ASCII_16X32_HEI   false         //16*32点阵ascii字体_黑体
#define CFG_ASCII_16X32_FANG  false         //16*32点阵ascii字体_仿宋
#define    CFG_MODULE_ENABLE_ASCII_DOT_FONT  true
//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure cpu onchip iic  ******************************************//
#define CFG_I2C1_ENABLE          false         //
#define CFG_I2C1_BUF_LEN         128           //
#define CFG_I2C2_ENABLE          false         //
#define CFG_I2C2_BUF_LEN         128           //
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_IIC  true
//*******************************  Configure cpu onchip peripheral lowpower control  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_PERIPHERAL_LOWPOWER_CONTROL  true
//*******************************  Configure cpu onchip uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN            32                 //
#define CFG_UART1_RECVBUF_LEN            32                 //
#define CFG_UART1_DMABUF_LEN             32                 //
#define CFG_UART2_SENDBUF_LEN            32                 //
#define CFG_UART2_RECVBUF_LEN            32                 //
#define CFG_UART2_DMABUF_LEN             32                 //
#define CFG_UART3_SENDBUF_LEN            32                 //
#define CFG_UART3_RECVBUF_LEN            32                 //
#define CFG_UART3_DMABUF_LEN             32                 //
#define CFG_UART4_SENDBUF_LEN            32                  //
#define CFG_UART4_RECVBUF_LEN            32                  //
#define CFG_UART5_SENDBUF_LEN            32                   //
#define CFG_UART5_RECVBUF_LEN            32                   //
#define CFG_UART1_ENABLE                 true                 //
#define CFG_UART2_ENABLE                 false                //
#define CFG_UART3_ENABLE                 false                //
#define CFG_UART4_ENABLE                 false                //
#define CFG_UART5_ENABLE                 false                //
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_UART  true
//*******************************  Configure debug information  ******************************************//
#define    CFG_MODULE_ENABLE_DEBUG_INFORMATION  true
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //定义设备数量
#define    CFG_MODULE_ENABLE_DEVICE_FILE_SYSTEM  true
//*******************************  Configure djybus  ******************************************//
#define    CFG_MODULE_ENABLE_DJYBUS        true
//*******************************  Configure flash  ******************************************//
#define    CFG_MODULE_ENABLE_FLASH         true
//*******************************  Configure font  ******************************************//
#define CFG_FONT_DEFAULT  "gb2312_song_16"      //字体名在include/font目录中找
#define    CFG_MODULE_ENABLE_FONT          true
//*******************************  Configure graphical decorate development  ******************************************//
#define CFG_DESKTOP_WIDTH       0           //桌面尺寸（像素数）宽度0=显示器宽度
#define CFG_DESKTOP_HEIGHT      0           //桌面尺寸（像素数）高度0=显示器高度
#define CFG_DESKTOP_BUF         true        //内存稀少的硬件可不带缓存
#define CFG_DISPLAY_NAME        "DISPLAY_NAME"         //须与bsp中显示器驱动模块配置的显示器名字相同
#define CFG_DESKTOP_NAME        "DESKTOP_NAME"         //
#define CFG_DESKTOP_FORMAT      CN_SYS_PF_RGB565    //桌面窗口像素格式常数在gkernel.h中定义，一般使用与显示器相同颜色
#define CFG_GRAY_BASE_COLOR     CN_COLOR_WHITE      //像素格式设为灰度时才需要设置的“最亮”色可在gkernel.h中找到常用颜色定义
#define CFG_FILL_COLOR          CN_COLOR_BLUE       //创建桌面时的填充色用888格式，可在gkernel.h中找到常用颜色定义
#define    CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT  true
//*******************************  Configure graphical kernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024    //上层应用（例如gdd）向gkernel传递命令的缓冲区长度（字节数）
#define CFG_USER_REQUEST_DEEP       128     //gkernel向上层请求命令的缓冲区长度（字节数）
#define    CFG_MODULE_ENABLE_GRAPHICAL_KERNEL  true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //即使选false也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure human machine interface  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2       //人机交互输入设备数量如键盘、鼠标等
#define    CFG_MODULE_ENABLE_HUMAN_MACHINE_INTERFACE  true
//*******************************  Configure iicbus  ******************************************//
#define    CFG_MODULE_ENABLE_IICBUS        true
//*******************************  Configure int  ******************************************//
#define    CFG_MODULE_ENABLE_INT           true
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //定义初始化过程使用的栈空间一般按默认值就可以了
#define CFG_EVENT_LIMIT         15      //事件数量
#define CFG_EVENT_TYPE_LIMIT    15      //事件类型数
#define CFG_IDLESTACK_LIMIT     1024    //IDLE事件处理函数运行的栈尺寸一般按默认值就可以了
#define CFG_IDLE_MONITOR_CYCLE  30      //监视IDLE事件持续低于1/16 CPU占比的时间秒数，0=不监视
#define CFG_MAINSTACK_LIMIT     4096    //main函数运行所需的栈尺寸
#define CFG_IDLE_WDT_RESET      false   //IDLE事件监视狗叫时，是否复位
#define CFG_OS_TINY             false   //true=用于资源紧缺的场合内核会裁剪掉：事件类型名字，事件处理时间统计。
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //用完会自动扩充
#define CFG_HANDLE_LIMIT        8   //用完会自动扩充
#define    CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure loader  ******************************************//
#define CFG_UPDATEIBOOT_EN      false       //
#define CFG_START_APP_IS_VERIFICATION      true       //
#define  CFG_APP_RUNMODE  CN_DIRECT_RUN     //CN_DIRECT_RUN=直接从flash中运行；CN_FORM_FILE=从文件系统加载到内存运行
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL   //
#define CFG_IBOOT_VERSION_SMALL       00        //xx.xx.__APP忽略
#define CFG_IBOOT_VERSION_MEDIUM      00        //xx.__.xxAPP忽略
#define CFG_IBOOT_VERSION_LARGE       01        //__.xx.xxAPP忽略
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"           //
#define CFG_APP_UPDATE_NAME        "/yaf2/app.bin"            //
#define    CFG_MODULE_ENABLE_LOADER        true
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //定义锁的数量包含信号量和互斥量
#define    CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
#define    CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure message queue  ******************************************//
#define    CFG_MODULE_ENABLE_MESSAGE_QUEUE true
//*******************************  Configure misc  ******************************************//
#define    CFG_MODULE_ENABLE_MISC          true
//*******************************  Configure multiplex  ******************************************//
#define    CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure Nls Charset  ******************************************//
#define CFG_LOCAL_CHARSET       "gb2312"        //
#define    CFG_MODULE_ENABLE_NLS_CHARSET   true
//*******************************  Configure ring buffer and line buffer  ******************************************//
#define    CFG_MODULE_ENABLE_RING_BUFFER_AND_LINE_BUFFER  true
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000      //
#define CFG_ADD_ROUTINE_SHELL      true        //
#define CFG_ADD_EXPAND_SHELL       true        //
#define CFG_ADD_GLOBAL_FUN         false       //
#define CFG_SHOW_ADD_SHEELL        true        //
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true         //
#define CFG_STDIO_STDOUT_FOLLOW    true         //
#define CFG_STDIO_STDERR_FOLLOW    true         //
#define CFG_STDIO_FLOAT_PRINT      true         //
#define CFG_STDIO_STDIOFILE        true         //
#define CFG_STDIO_IN_NAME              "/dev/UART1"    //
#define CFG_STDIO_OUT_NAME             "/dev/UART1"    //
#define CFG_STDIO_ERR_NAME             "/dev/UART1"    //
#define    CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure STM32 common code  ******************************************//
#define    CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //北京时间是东8区
#define    CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure touch  ******************************************//
#define    CFG_MODULE_ENABLE_TOUCH         true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure LCD driver ili9325  ******************************************//
#define CFG_LCD_XSIZE   240             //
#define CFG_LCD_YSIZE   320             //
#define CFG_LCD_XSIZE_UM   36500            //
#define CFG_LCD_YSIZE_UM   48600            //
#define CFG_ILI9325_DISPLAY_NAME              "lcdili9325"    //配置液晶显示的名称
#define CFG_ILI9325_HEAP_NAME                 "extram"        //配置液晶驱动所使用的堆名称
#define    CFG_MODULE_ENABLE_LCD_DRIVER_ILI9325  true
//*******************************  Configure stmpe811  ******************************************//
#define CFG_CRT_CLK_FRE                 (100*1000)      //总线速度，单位Hz
#define CFG_STMPE811_DEVADDR            0x41            //IIC总线上的设备地址
#define CFG_STMPE811_TOUCH_DEV_NAME   "TOUCH_STMPE811"       //配置触摸屏名称
#define CFG_STMPE811_DISPLAY_NAME        "LCD_DESKTOP_STMPE811"      //配置触摸屏所在显示器的名称
#define    CFG_MODULE_ENABLE_STMPE811      true
//*******************************  Core Clock  ******************************************//
#define  CFG_CORE_MCLK                   (168.0*Mhz)       //主频，内核要用，必须定义
//*******************************  DjyosProduct Configuration  ******************************************//
#define    PRODUCT_MANUFACTURER_NAME       ""                //厂商名称
#define    PRODUCT_PRODUCT_CLASSIFY        ""                //产品分类
#define    PRODUCT_PRODUCT_MODEL           ""                //产品型号
#define    PRODUCT_VERSION_LARGE           0                 //版本号,__.xx.xx
#define    PRODUCT_VERSION_MEDIUM          0                 //版本号,xx.__.xx
#define    PRODUCT_VERSION_SMALL           0                 //版本号,xx.xx.__
#define    PRODUCT_PRODUCT_MODEL_CODE      ""                //产品型号编码
#define    PRODUCT_BRANCH                  ""                //产品分支
#define    PRODUCT_PASSWORD                ""                //产品秘钥
#define    PRODUCT_OTA_ADDRESS             ""                //OTA服务器地址
#define    PRODUCT_BOARD_TYPE              ""                //板件类型
#define    PRODUCT_CPU_TYPE                ""                //CPU类型


#endif
