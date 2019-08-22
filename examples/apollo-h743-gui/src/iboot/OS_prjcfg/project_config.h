/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure ascii charset  ******************************************//
#define  CFG_MODULE_ENABLE_ASCII_CHARSET true
//*******************************  Configure black box  ******************************************//
#define  CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure board config  ******************************************//
#define  CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure board driver lcd config  ******************************************//
#define  CFG_MODULE_ENABLE_BOARD_DRIVER_LCD_CONFIG  true
//*******************************  Configure can bus  ******************************************//
#define CFG_CAN_BUS_MSGQ_NUM        200     //
#define CFG_CAN_BUS_TIMEOUT         200     //
#define CFG_CAN_BUS_RCV_RING_LEN    100     //
#define  CFG_MODULE_ENABLE_CAN_BUS       true
//*******************************  Configure cpu onchip dma  ******************************************//
#define  CFG_MODULE_ENABLE_CPU_ONCHIP_DMA  true
//*******************************  Configure cpu onchip lcd  ******************************************//
#define CFG_LCD_XSIZE                   600 //
#define CFG_LCD_YSIZE                   480 //
#define CFG_DISPLAY_NAME        "LCD_F7"    //
#define CFG_LCD_HEAP_NAME       "extram"    //
#define  CFG_MODULE_ENABLE_CPU_ONCHIP_LCD  true
//*******************************  Configure cpu onchip MAC  ******************************************//
#define CFG_ETH_NETCARD_NAME    "STM32H7_ETH"   //
#define CFG_ETH_LOOP_CYCLE      1000         //中断模式无须填写
#define CFG_ETH_LOOP_ENABLE     true            //
#define CFG_ETH_HARD_MAC_ADDR   true            //
#define CFG_ETH_MAC_ADDR0      00           //若选中"硬件生成Mac地址"则无须填写
#define CFG_ETH_MAC_ADDR1      02           //若选中"硬件生成Mac地址"则无须填写
#define CFG_ETH_MAC_ADDR2      03           //若选中"硬件生成Mac地址"则无须填写
#define CFG_ETH_MAC_ADDR3      04           //若选中"硬件生成Mac地址"则无须填写
#define CFG_ETH_MAC_ADDR4      05           //若选中"硬件生成Mac地址"则无须填写
#define CFG_ETH_MAC_ADDR5      06           //若选中"硬件生成Mac地址"则无须填写
#define  CFG_MODULE_ENABLE_CPU_ONCHIP_MAC  true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define  CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure cpu onchip random  ******************************************//
#define CFG_RNG_NUM_MAX      65535         //
#define CFG_RNG_NUM_MIN      1024          //
#define  CFG_MODULE_ENABLE_CPU_ONCHIP_RANDOM  true
//*******************************  Configure cpu onchip systime  ******************************************//
#define  CFG_MODULE_ENABLE_CPU_ONCHIP_SYSTIME  true
//*******************************  Configure cpu onchip timer  ******************************************//
#define  CFG_MODULE_ENABLE_CPU_ONCHIP_TIMER  true
//*******************************  Configure cpu onchip uart  ******************************************//
#define CFG_UART_SENDBUF_LEN            32                  //
#define CFG_UART_RECVBUF_LEN            32                  //
#define CFG_UART1_ENABLE                true                //
#define CFG_UART2_ENABLE                false               //
#define CFG_UART3_ENABLE                false               //
#define CFG_UART4_ENABLE                false               //
#define CFG_UART5_ENABLE                false               //
#define CFG_UART6_ENABLE                false               //
#define CFG_UART7_ENABLE                false               //
#define CFG_UART8_ENABLE                false               //
#define  CFG_MODULE_ENABLE_CPU_ONCHIP_UART  true
//*******************************  Configure debug information  ******************************************//
#define  CFG_MODULE_ENABLE_DEBUG_INFORMATION  true
//*******************************  Configure djybus  ******************************************//
#define  CFG_MODULE_ENABLE_DJYBUS        true
//*******************************  Configure easy file system  ******************************************//
#define CFG_EFS_FILE_BLOCK_SIZE           4096                 // 单个文件大小的上限
#define CFG_EFS_INSTALL_OPTION            MS_INSTALLFORMAT      //EFS文件系统安装选项，16777216:文件系统不存在时则新建；256：格式化文件系统
#define CFG_EFS_MOUNT_POINT               "efs"      //EFS文件系统安装目录
#define  CFG_MODULE_ENABLE_EASY_FILE_SYSTEM  true
//*******************************  Configure ethernet phy lan8720  ******************************************//
#define CFG_LAN8720_PHY_ADDRESS     0   //phy的物理地址，硬件一般设计为 0
#define  CFG_MODULE_ENABLE_ETHERNET_PHY_LAN8720  true
//*******************************  Configure file system  ******************************************//
#define CFG_CLIB_BUFFERSIZE            512      //
#define  CFG_MODULE_ENABLE_FILE_SYSTEM   true
//*******************************  Configure font  ******************************************//
#define CFG_FONT_DEFAULT  "gb2312_song_16"      //字体名在include/font目录中找
#define  CFG_MODULE_ENABLE_FONT          true
//*******************************  Configure gb2312 charset  ******************************************//
#define  CFG_MODULE_ENABLE_GB2312_CHARSET  true
//*******************************  Configure gb2312 dot  ******************************************//
#define CFG_GB2312_12_SONG              zk_disable              //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"  //若从文件读取则配置文件名
#define CFG_GB2312_16_SONG              from_array              //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
    #define CFG_GB2312_16_SONG_FILENAME "zk_gb2316_16song.bin"  //若从文件读取则配置文件名
#define CFG_GB2312_16_YUAN              zk_disable              //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
    #define CFG_GB2312_16_YUAN_FILENAME "zk_gb2316_16yuan.bin"  //若从文件读取则配置文件名
#define CFG_GB2312_16_KAI              zk_disable               //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
    #define CFG_GB2312_16_KAI_FILENAME  "zk_gb2316_16kai.bin"   //若从文件读取则配置文件名
#define CFG_GB2312_16_HEI              zk_disable               //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
    #define CFG_GB2312_16_HEI_FILENAME  "zk_gb2316_16hei.bin"   //若从文件读取则配置文件名
#define CFG_GB2312_16_FANG              zk_disable              //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
    #define CFG_GB2312_16_FANG_FILENAME "zk_gb2316_16fang.bin"  //若从文件读取则配置文件名
#define CFG_GB2312_24_SONG              zk_disable              //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
    #define CFG_GB2312_24_SONG_FILENAME "zk_gb2316_24song.bin"  //若从文件读取则配置文件名
#define CFG_GB2312_24_YUAN              zk_disable              //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
    #define CFG_GB2312_24_YUAN_FILENAME "zk_gb2316_24yuan.bin"  //若从文件读取则配置文件名
#define CFG_GB2312_24_KAI              zk_disable               //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
    #define CFG_GB2312_24_KAI_FILENAME  "zk_gb2316_24kai.bin"   //若从文件读取则配置文件名
#define CFG_GB2312_24_HEI              zk_disable               //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
    #define CFG_GB2312_24_HEI_FILENAME  "zk_gb2316_24hei.bin"   //若从文件读取则配置文件名
#define CFG_GB2312_24_FANG              zk_disable              //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
    #define CFG_GB2312_24_FANG_FILENAME "zk_gb2316_24fang.bin"  //若从文件读取则配置文件名
#define CFG_GB2312_32_SONG              zk_disable              //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
    #define CFG_GB2312_32_SONG_FILENAME "zk_gb2316_32song.bin"  //若从文件读取则配置文件名
#define CFG_GB2312_32_YUAN              zk_disable              //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
    #define CFG_GB2312_32_YUAN_FILENAME "zk_gb2316_32yuan.bin"  //若从文件读取则配置文件名
#define CFG_GB2312_32_KAI              zk_disable               //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
    #define CFG_GB2312_32_KAI_FILENAME  "zk_gb2316_32kai.bin"   //若从文件读取则配置文件名
#define CFG_GB2312_32_HEI              zk_disable               //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
    #define CFG_GB2312_32_HEI_FILENAME  "zk_gb2316_32hei.bin"   //若从文件读取则配置文件名
#define CFG_GB2312_32_FANG              zk_disable              //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
    #define CFG_GB2312_32_FANG_FILENAME "zk_gb2316_32fang.bin"  //若从文件读取则配置文件名
#define  CFG_MODULE_ENABLE_GB2312_DOT    true
//*******************************  Configure graphical kernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024    //gdd向gkernel传递命令的缓冲区长度（字节数）
#define  CFG_MODULE_ENABLE_GRAPHICAL_KERNEL  true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //即使选false也允许使用malloc-free分配内存但使用有差别详见 《……用户手册》内存分配章节
#define  CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure human machine interface  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2       //人机交互输入设备数量如键盘、鼠标等
#define  CFG_MODULE_ENABLE_HUMAN_MACHINE_INTERFACE  true
//*******************************  Configure IIC to 8gpio pcf8574  ******************************************//
#define CFG_PCF8574_BUS_NAME       "IoIic"        //
#define  CFG_MODULE_ENABLE_IIC_TO_8GPIO_PCF8574  true
//*******************************  Configure iicbus  ******************************************//
#define  CFG_MODULE_ENABLE_IICBUS        true
//*******************************  Configure int  ******************************************//
#define  CFG_MODULE_ENABLE_INT           true
//*******************************  Configure io analog iic bus  ******************************************//
#define  CFG_MODULE_ENABLE_IO_ANALOG_IIC_BUS  true
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //定义初始化过程使用的栈空间一般按默认值就可以了
#define CFG_EVENT_LIMIT         15      //事件数量
#define CFG_EVENT_TYPE_LIMIT    15      //事件类型数
#define CFG_MAINSTACK_LIMIT     4096    //main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024    //IDLE事件处理函数运行的栈尺寸一般按默认值就可以了
#define CFG_OS_TINY             false   //true=用于资源紧缺的场合内核会裁剪掉：事件类型名字事件处理时间统计。
#define  CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //用完会自动扩充
#define CFG_HANDLE_LIMIT        8   //用完会自动扩充
#define  CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure loader  ******************************************//
#define CFG_UPDATEIBOOT_EN      false       //
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN     //EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL      //是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_IBOOT_VERSION       01        //Iboot发布版本号
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"           //
#define  CFG_MODULE_ENABLE_LOADER        true
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //定义锁的数量包含信号量和互斥量
#define  CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
#define  CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure message queue  ******************************************//
#define  CFG_MODULE_ENABLE_MESSAGE_QUEUE true
//*******************************  Configure multiplex  ******************************************//
#define  CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure network config  ******************************************//
#define CFG_STATIC_IP       true            //
#define CFG_SELECT_NETCARD  "STM32H7_ETH"   //必须与选中的网卡驱动中配置的名称相同
#define CFG_MY_IPV4         "192.168.0.179" //
#define CFG_MY_SUBMASK      "255.255.255.0" //
#define CFG_MY_GATWAY       "192.168.0.1"   //
#define CFG_MY_DNS          "192.168.0.1"   //
#define  CFG_MODULE_ENABLE_NETWORK_CONFIG  true
//*******************************  Configure Nls Charset  ******************************************//
#define CFG_LOCAL_CHARSET       "gb2312"        //
#define  CFG_MODULE_ENABLE_NLS_CHARSET   true
//*******************************  Configure ring buffer and line buffer  ******************************************//
#define  CFG_MODULE_ENABLE_RING_BUFFER_AND_LINE_BUFFER  true
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000      //
#define CFG_ADD_ROUTINE_SHELL      true        //
#define CFG_ADD_EXPAND_SHELL       true        //
#define CFG_ADD_GLOBAL_FUN         false       //
#define CFG_SHOW_ADD_SHEELL        true        //
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure Software Timers  ******************************************//
#define CFG_TIMERS_LIMIT        5       //可创建的定时器数量（不包含图形界面的定时器）
#define CFG_TIMER_SOUCE_HARD    true    //选择专用硬件还是tick/tickless做时钟源
#define  CFG_MODULE_ENABLE_SOFTWARE_TIMERS  true
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true         //
#define CFG_STDIO_STDOUT_FOLLOW    true         //
#define CFG_STDIO_STDERR_FOLLOW    true         //
#define CFG_STDIO_FLOAT_PRINT      true         //
#define CFG_STDIO_STDIOFILE        true         //
#define CFG_STDIO_IN_NAME              "/dev/UART1"    //
#define CFG_STDIO_OUT_NAME             "/dev/UART1"    //
#define CFG_STDIO_ERR_NAME             "/dev/UART1"    //
#define  CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure STM32 commond code  ******************************************//
#define  CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure tcp  ******************************************//
#define     CFG_TCP_REORDER             true    //资源充足才打开
#define     CFG_TCP_CCBNUM              10      //占一个 指针 和 struct ClienCB
#define     CFG_TCP_SCBNUM              5       //占一个 指针 和 struct ServerCB
#define  CFG_MODULE_ENABLE_TCP           true
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE          0x4000  //
#define  CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //北京时间是东8区
#define  CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure touch  ******************************************//
#define  CFG_MODULE_ENABLE_TOUCH         true
//*******************************  Configure touchscreen FT5X26  ******************************************//
#define CT_MAX_TOUCH  5                         //支持最多5点触摸
#define CFG_TOUCH_ADJUST_FILE   "/yaf2/touch_init.dat"  //保存触摸屏矫正参数的文件
#define CFG_FT5X26_BUS_NAME     "IoIic"        //触摸芯片使用的IIC总线名称
#define CFG_FT5X26_TOUCH_NAME   "FT5X26"       //配置触摸屏名称
#define CFG_DESKTOP_NAME        "desktop"      //配置触摸屏所在显示器桌面的名称
#define  CFG_MODULE_ENABLE_TOUCHSCREEN_FT5X26  true
//*******************************  Configure uart device file  ******************************************//
#define  CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure xip app file system  ******************************************//
#define  CFG_MODULE_ENABLE_XIP_APP_FILE_SYSTEM  true
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //主频，内核要用，必须定义

#endif
