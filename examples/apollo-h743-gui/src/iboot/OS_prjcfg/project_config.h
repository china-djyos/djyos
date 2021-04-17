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
//*******************************  Configure file system  ******************************************//
#define CFG_CLIB_BUFFERSIZE            512       // "C库文件用户态缓冲区尺寸"
#define    CFG_MODULE_ENABLE_FILE_SYSTEM   true
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
//*******************************  Configure xip app file system  ******************************************//
#define    CFG_MODULE_ENABLE_XIP_APP_FILE_SYSTEM  true
//*******************************  Configure cpu onchip timer  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_TIMER  true
//*******************************  Configure cpu onchip systime  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_SYSTIME  true
//*******************************  Configure cpu onchip dma  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_DMA  true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure cpu onchip random  ******************************************//
#define CFG_RNG_NUM_MAX      65535          // "随机数最大值",
#define CFG_RNG_NUM_MIN      1024           // "随机数最小值",
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_RANDOM  true
//*******************************  Configure STM32 commond code  ******************************************//
#define    CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure djybus  ******************************************//
#define    CFG_MODULE_ENABLE_DJYBUS        true
//*******************************  Configure can bus  ******************************************//
#define CFG_CAN_BUS_MSGQ_NUM        200      // "消息队列长度",
#define CFG_CAN_BUS_TIMEOUT         200      // "总线超时(ms)",
#define CFG_CAN_BUS_RCV_RING_LEN    100      // "接收buffer消息数",
#define    CFG_MODULE_ENABLE_CAN_BUS       true
//*******************************  Configure iicbus  ******************************************//
#define    CFG_MODULE_ENABLE_IICBUS        true
//*******************************  Configure io analog iic bus  ******************************************//
#define    CFG_MODULE_ENABLE_IO_ANALOG_IIC_BUS  true
//*******************************  Configure IIC to 8gpio pcf8574  ******************************************//
#define CFG_PCF8574_BUS_NAME       "IoIic"         // "IIC总线名"，
#define    CFG_MODULE_ENABLE_IIC_TO_8GPIO_PCF8574  true
//*******************************  Configure easy file system  ******************************************//
#define CFG_EFS_FILE_SIZE_LIMIT           4096                 //
#define CFG_EFS_MAX_CREATE_FILE_NUM       50                   //
#define CFG_EFS_MAX_OPEN_FILE_NUM         10                   //
#define CFG_EFS_MOUNT_POINT               "efs"       // "EFS文件系统安装目录"
#define CFG_EFS_INSTALL_OPTION            MS_INSTALLFORMAT       // "安装选项"，MS_INSTALLFORMAT,MS_INSTALLCREAT,MS_INSTALLUSE中一个或多个的“或”
#define    CFG_MODULE_ENABLE_EASY_FILE_SYSTEM  true
//*******************************  Configure font  ******************************************//
#define CFG_FONT_DEFAULT  "gb2312_song_16"       // "默认字体",字体名在include/font目录中找
#define    CFG_MODULE_ENABLE_FONT          true
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
//*******************************  Configure Nls Charset  ******************************************//
#define CFG_LOCAL_CHARSET       "gb2312"         // "本地字符集",
#define    CFG_MODULE_ENABLE_NLS_CHARSET   true
//*******************************  Configure gb2312 charset  ******************************************//
#define    CFG_MODULE_ENABLE_GB2312_CHARSET  true
//*******************************  Configure gb2312 dot  ******************************************//
#define CFG_GB2312_12_SONG              zk_disable               // "12点阵宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_16_SONG              from_array               // "16点阵宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_16_YAHEI             zk_disable              //GB2312字体,zk_disable：不需要from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_16_YUAN              zk_disable               // "16点阵圆体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_16_KAI              zk_disable                // "16点阵楷体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_16_HEI              zk_disable                // "16点阵黑",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_16_FANG              zk_disable               // "16点阵仿宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_24_SONG              zk_disable               // "24点阵宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_24_YAHEI              zk_disable             // "24点阵微软雅黑",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   //  "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_24_YUAN              zk_disable               // "24点阵圆体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_24_KAI              zk_disable                // "24点阵楷体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_24_HEI              zk_disable                // "24点阵黑体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_24_FANG              zk_disable               // "24点阵仿宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_32_SONG              zk_disable               // "32点阵宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_32_YUAN              zk_disable               // "32点阵圆体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_32_KAI              zk_disable                // "32点阵楷体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_32_HEI              zk_disable                // "32点阵黑体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_32_FANG              zk_disable               // "32点阵仿宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"   // "字库文件名",若从文件读取，则配置文件名
#define    CFG_MODULE_ENABLE_GB2312_DOT    true
//*******************************  Configure ascii charset  ******************************************//
#define    CFG_MODULE_ENABLE_ASCII_CHARSET true
//*******************************  Configure Software Timers  ******************************************//
#define CFG_TIMERS_LIMIT        5        // "定时器数量",可创建的定时器数量（不包含图形界面的定时器）
#define CFG_TIMER_SOUCE_HARD    true     // "硬件定时器提供时钟源",选择专用硬件还是tick做时钟源
#define    CFG_MODULE_ENABLE_SOFTWARE_TIMERS  true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8       // "时区",北京时间是东8区
#define    CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure board driver lcd config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_DRIVER_LCD_CONFIG  true
//*******************************  Configure debug information  ******************************************//
#define    CFG_MODULE_ENABLE_DEBUG_INFORMATION  true
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
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true   // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure cpu onchip lcd  ******************************************//
#define CFG_LCD_XSIZE                   600  //  "LCD水平像素宽度",
#define CFG_LCD_YSIZE                   480  //  "LCD竖直像素宽度",
#define CFG_DISPLAY_NAME        "LCD_F7"     //  "显示器LCD名称",
#define CFG_LCD_HEAP_NAME       "extram"     //  "LCD使用堆名称",
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_LCD  true
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
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE          0x4000   // "数据包缓冲区尺寸"
#define    CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure ethernet phy lan8720  ******************************************//
#define CFG_LAN8720_PHY_ADDRESS     0    //  phy的物理地址，硬件一般设计为 0
#define    CFG_MODULE_ENABLE_ETHERNET_PHY_LAN8720  true
//*******************************  Configure cpu onchip MAC  ******************************************//
#define CFG_ETH_NETCARD_NAME    "STM32H7_ETH"    //  "网卡名称",
#define CFG_ETH_LOOP_CYCLE      1000          //  "网卡轮询周期(uS)",中断模式无须填写
#define CFG_ETH_LOOP_ENABLE     true             //  "网卡接收是否轮询",
#define CFG_ETH_HARD_MAC_ADDR   true             //  "硬件生成Mac地址",
#define CFG_ETH_MAC_ADDR0      00            //  "MAC ADDR0",若选中"硬件生成Mac地址",则无须填写
#define CFG_ETH_MAC_ADDR1      02            //  "MAC ADDR1",若选中"硬件生成Mac地址",则无须填写
#define CFG_ETH_MAC_ADDR2      03            //  "MAC ADDR2",若选中"硬件生成Mac地址",则无须填写
#define CFG_ETH_MAC_ADDR3      04            //  "MAC ADDR3",若选中"硬件生成Mac地址",则无须填写
#define CFG_ETH_MAC_ADDR4      05            //  "MAC ADDR4",若选中"硬件生成Mac地址",则无须填写
#define CFG_ETH_MAC_ADDR5      06            //  "MAC ADDR5",若选中"硬件生成Mac地址",则无须填写
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_MAC  true
//*******************************  Configure tcp  ******************************************//
#define     CFG_TCP_REORDER             true     // "TCP乱序重组使能",资源充足才打开
#define     CFG_TCP_CCBNUM              10       // "tcp 客户端数限值"，占一个 指针 和 struct ClienCB
#define     CFG_TCP_SCBNUM              5        // "tcp 服务器数限值"，占一个 指针 和 struct ServerCB
#define    CFG_MODULE_ENABLE_TCP           true
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
#define CFG_SHELL_STACK            0x1000       // "执行shell命令的栈尺寸",
#define CFG_ADD_ROUTINE_SHELL      true         // "是否添加常规shell命令",
#define CFG_ADD_EXPAND_SHELL       true         // "是否添加拓展shell命令",
#define CFG_ADD_GLOBAL_FUN         false        // "添加全局函数到shell",
#define CFG_SHOW_ADD_SHEELL        true         // "显示在编译窗口添加的shell命令",
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure network config  ******************************************//
#define CFG_STATIC_IP       true             //  "使用静态IP?",
#define CFG_SELECT_NETCARD  "STM32H7_ETH"    //  "网卡名称",必须与选中的网卡驱动中配置的名称相同
#define CFG_MY_IPV4         "192.168.0.179"  //  "静态IP",
#define CFG_MY_SUBMASK      "255.255.255.0"  //  "子网掩码",
#define CFG_MY_GATWAY       "192.168.0.1"    //  "网关",
#define CFG_MY_DNS          "192.168.0.1"    //  "DNS",
#define    CFG_MODULE_ENABLE_NETWORK_CONFIG  true
//*******************************  Configure touchscreen FT5X26  ******************************************//
#define CT_MAX_TOUCH  5                          // "触控数",支持最多5点触摸
#define CFG_TOUCH_ADJUST_FILE   "/yaf2/touch_init.dat"   // 保存触摸屏矫正参数的文件
#define CFG_FT5X26_BUS_NAME     "IoIic"         // "IIC总线名称",触摸芯片使用的IIC总线名称
#define CFG_FT5X26_TOUCH_NAME   "FT5X26"        // "触摸设备名称",
#define CFG_FT5X26_DISPLAY_NAME "display"      //
#define    CFG_MODULE_ENABLE_TOUCHSCREEN_FT5X26  true
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
