/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //定义设备数量
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40               // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure io iic config  ******************************************//
#define CFG_IO_IIC_BUS_NAME   "Ioiic"
//*******************************  Configure font  ******************************************//
#define     CFG_FONT_DEFAULT                 "gb2312_song_16"       // "默认字体",字体名在include/font目录中找
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
//*******************************  Configure graphical kernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024    //gdd向gkernel传递命令的缓冲区长度（字节数）
//*******************************  Configure graphical decorate development  ******************************************//
#define CFG_DESKTOP_WIDTH       0           //桌面尺寸（像素数）宽度0=显示器宽度
#define CFG_DESKTOP_HEIGHT      0           //桌面尺寸（像素数）高度0=显示器高度
#define CFG_DISPLAY_NAME        "DISPLAY_NAME"         //须与bsp中显示器驱动模块配置的显示器名字相同
#define CFG_DESKTOP_NAME        "DESKTOP_NAME"         //
#define CFG_INPUTDEV_NAME       "INPUTDEV_NAME"         //使用bsp中输入设备所配置的名字多输入设备的话每个设备间用逗号隔开
#define CFG_DESKTOP_FORMAT      CN_SYS_PF_RGB565    //桌面窗口像素格式常数在gkernel.h中定义一般使用与显示器相同颜色
#define CFG_GRAY_BASE_COLOR     CN_COLOR_WHITE      //像素格式设为灰度时才需要设置的“最亮”色可在gkernel.h中找到常用颜色定义
#define CFG_FILL_COLOR          CN_COLOR_BLUE       //创建桌面时的填充色用888格式可在gkernel.h中找到常用颜色定义
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true           // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure human machine interface  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2       //人机交互输入设备数量如键盘、鼠标等
//*******************************  Configure kernel  ******************************************//
#define     CFG_INIT_STACK_SIZE              4096                      // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define     CFG_EVENT_LIMIT                  30                        // "事件数量限值",事件数量
#define     CFG_EVENT_TYPE_LIMIT             30                        // "事件类型数限值",事件类型数
#define     CFG_MAINSTACK_LIMIT              4096                      // "main函数栈尺寸",main函数运行所需的栈尺寸
#define     CFG_IDLESTACK_LIMIT              1024                      // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define     CFG_OS_TINY                      false                     // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //用完会自动扩充
#define CFG_HANDLE_LIMIT        8   //用完会自动扩充
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  CN_DIRECT_RUN              // CN_DIRECT_RUN=直接从flash中运行；CN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL               // 是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_IBOOT_VERSION       01                 // Iboot发布版本号
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"                    // "Iboot保存"
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
//*******************************  Configure Nls Charset  ******************************************//
#define CFG_LOCAL_CHARSET       "gb2312"        //
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true     // "是否支持多种输入设备",
#define CFG_STDIO_STDOUT_FOLLOW    true     // "stdout是否跟随stdin",
#define CFG_STDIO_STDERR_FOLLOW    true     // "stderr是否跟随stdin",
#define CFG_STDIO_FLOAT_PRINT       true         // "支持浮点打印"
#define CFG_STDIO_STDIOFILE         true         // "支持标准IO文件"
#define CFG_STDIO_IN_NAME              "/dev/UART1"     // "标准输入设备名",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"     // "标准输出设备名",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"     // "标准err输出设备名",
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000       // "执行shell命令的栈尺寸"
#define     CFG_ADD_ROUTINE_SHELL            true                  // "是否添加常规shell命令"
#define     CFG_ADD_EXPAND_SHELL             true                  // "是否添加拓展shell命令"
#define     CFG_ADD_GLOBAL_FUN               false                 // "添加全局函数到shell"
#define     CFG_SHOW_ADD_SHEELL              true                  // "显示在编译窗口添加的shell命令"
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE               0x4000                    // "数据包缓冲区尺寸"
#define  CFG_MODULE_ENABLE_TCPIP         true
#define  CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure ethernet phy lan8720  ******************************************//
#define CFG_LAN8720_PHY_ADDRESS     0   //phy的物理地址，硬件一般设计为 0
//*******************************  Configure network config  ******************************************//
#define CFG_STATIC_IP       true            //
#define CFG_SELECT_NETCARD  "STM32F7_ETH"   //必须与选中的网卡驱动中配置的名称相同
#define CFG_MY_IPV4         "192.168.0.179" //
#define CFG_MY_SUBMASK      "255.255.255.0" //
#define CFG_MY_GATWAY       "192.168.0.1"   //
#define CFG_MY_DNS          "192.168.0.1"   //
//*******************************  Configure tcp  ******************************************//
#define     CFG_TCP_REORDER                  true                      // "TCP乱序重组使能",资源充足才打开
#define     CFG_TCP_CCBNUM                   10                        // "tcp 客户端数限值"，占一个 指针 和 struct ClienCB
#define     CFG_TCP_SCBNUM                   5                         // "tcp 服务器数限值"，占一个 指针 和 struct ServerCB
#define  CFG_MODULE_ENABLE_TCP           true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8               // "时区",北京时间是东8区
//*******************************  Configure cpu onchip MAC  ******************************************//
#define CFG_ETH_NETCARD_NAME    "STM32F7_ETH"   //
#define CFG_ETH_LOOP_CYCLE      1000         //中断模式无须填写
#define CFG_ETH_LOOP_ENABLE     true            //
#define CFG_ETH_HARD_MAC_ADDR   true            //
#define CFG_ETH_MAC_ADDR0      00           //若选中"硬件生成Mac地址"则无须填写
#define CFG_ETH_MAC_ADDR1      02           //若选中"硬件生成Mac地址"则无须填写
#define CFG_ETH_MAC_ADDR2      03           //若选中"硬件生成Mac地址"则无须填写
#define CFG_ETH_MAC_ADDR3      04           //若选中"硬件生成Mac地址"则无须填写
#define CFG_ETH_MAC_ADDR4      05           //若选中"硬件生成Mac地址"则无须填写
#define CFG_ETH_MAC_ADDR5      06           //若选中"硬件生成Mac地址"则无须填写
//*******************************  Configure cpu onchip uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN       64      //
#define CFG_UART1_RECVBUF_LEN       64      //
#define CFG_UART1_DMABUF_LEN        64      //
#define CFG_UART2_SENDBUF_LEN       64      //
#define CFG_UART2_RECVBUF_LEN       64      //
#define CFG_UART2_DMABUF_LEN        64      //
#define CFG_UART3_SENDBUF_LEN       64      //
#define CFG_UART3_RECVBUF_LEN       64      //
#define CFG_UART3_DMABUF_LEN        64      //
#define CFG_UART4_SENDBUF_LEN       64      //
#define CFG_UART4_RECVBUF_LEN       64      //
#define CFG_UART4_DMABUF_LEN        64      //
#define CFG_UART5_SENDBUF_LEN       64      //
#define CFG_UART5_RECVBUF_LEN       64      //
#define CFG_UART5_DMABUF_LEN        64      //
#define CFG_UART6_SENDBUF_LEN       64      //
#define CFG_UART6_RECVBUF_LEN       64      //
#define CFG_UART6_DMABUF_LEN        64      //
#define CFG_UART7_SENDBUF_LEN       64      //
#define CFG_UART7_RECVBUF_LEN       64      //
#define CFG_UART7_DMABUF_LEN        64      //
#define CFG_UART8_SENDBUF_LEN       64      //
#define CFG_UART8_RECVBUF_LEN       64      //
#define CFG_UART8_DMABUF_LEN        64      //
#define CFG_UART1_ENABLE           true        //
#define CFG_UART2_ENABLE           false       //
#define CFG_UART3_ENABLE           false       //
#define CFG_UART4_ENABLE           false       //
#define CFG_UART5_ENABLE           false       //
#define CFG_UART6_ENABLE           false       //
#define CFG_UART7_ENABLE           false       //
#define CFG_UART8_ENABLE           false       //
//*******************************  Configure IIC to 8gpio pcf8574  ******************************************//
#define CFG_PCF8574_BUS_NAME       "IoIic"        //
//*******************************  Configure keyboard hard driver  ******************************************//
#define CFG_KEYBOARD_NAME              "KEYBOARD"        //配置键盘名称
//*******************************  Configure touchscreen FT5X26  ******************************************//
#define CT_MAX_TOUCH  5                         //支持最多5点触摸
#define CFG_TOUCH_ADJUST_FILE   "/yaf2/touch_init.dat"  //保存触摸屏矫正参数的文件
#define CFG_FT5X26_BUS_NAME     "IoIic"        //触摸芯片使用的IIC总线名称
#define CFG_FT5X26_TOUCH_NAME   "FT5X26"       //配置触摸屏名称
#define CFG_DESKTOP_NAME        "desktop"      //配置触摸屏所在显示器桌面的名称
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (216*Mhz)         //主频，内核要用，必须定义

#endif
