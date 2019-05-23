/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure Nls Charset  ******************************************//
#define     CFG_LOCAL_CHARSET                "gb2312"                          // "本地字符集",
//*******************************  Configure lock  ******************************************//
#define     CFG_LOCK_LIMIT                   40                                // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true                  // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure device file system  ******************************************//
#define    CFG_DEVFILE_LIMIT               10                         // "设备数量",定义设备数量
//*******************************  Configure file system  ******************************************//
#define     CFG_CLIB_BUFFERSIZE              512                               // "C库文件用户态缓冲区尺寸"
//*******************************  Configure easyfile system  ******************************************//
#define     CFG_EFS_FILE_BLOCK_SIZE          0x1000                            //  单个文件大小的上限
#define CFG_EFS_INSTALL_OPTION            MS_INSTALLFORMAT               // EFS文件系统安装选项，16777216:文件系统不存在时则新建；256：格式化文件系统
#define     CFG_EFS_MOUNT_POINT              "efs"                             // "name",EFS文件系统安装目录
//*******************************  Configure font  ******************************************//
#define     CFG_FONT_DEFAULT                 CN_FONT_GB2312_SONG_16                // "默认字体",字体名在include/font目录中找
//*******************************  Configure gb2312 dot  ******************************************//
#define     CFG_GB2312_12_SONG               zk_disable                        // "12点阵宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"                 // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_16_SONG              from_array                             // "16点阵宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_16_SONG_FILENAME "zk_gb2316_16song.bin"                 // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_16_YUAN              zk_disable                             // "16点阵圆体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_16_YUAN_FILENAME "zk_gb2316_16yuan.bin"                 // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_16_KAI              zk_disable                              // "16点阵楷体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_16_KAI_FILENAME  "zk_gb2316_16kai.bin"                  // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_16_HEI              zk_disable                              // "16点阵黑",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_16_HEI_FILENAME  "zk_gb2316_16hei.bin"                  // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_16_FANG              zk_disable                             // "16点阵仿宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_16_FANG_FILENAME "zk_gb2316_16fang.bin"                 // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_24_SONG              zk_disable                             // "24点阵宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_24_SONG_FILENAME "zk_gb2316_24song.bin"                 // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_24_YUAN              zk_disable                             // "24点阵圆体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_24_YUAN_FILENAME "zk_gb2316_24yuan.bin"                 // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_24_KAI              zk_disable                              // "24点阵楷体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_24_KAI_FILENAME  "zk_gb2316_24kai.bin"                  // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_24_HEI              zk_disable                              // "24点阵黑体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_24_HEI_FILENAME  "zk_gb2316_24hei.bin"                  // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_24_FANG              zk_disable                             // "24点阵仿宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_24_FANG_FILENAME "zk_gb2316_24fang.bin"                 // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_32_SONG              zk_disable                             // "32点阵宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_32_SONG_FILENAME "zk_gb2316_32song.bin"                 // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_32_YUAN              zk_disable                             // "32点阵圆体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_32_YUAN_FILENAME "zk_gb2316_32yuan.bin"                 // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_32_KAI              zk_disable                              // "32点阵楷体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_32_KAI_FILENAME  "zk_gb2316_32kai.bin"                  // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_32_HEI              zk_disable                              // "32点阵黑体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_32_HEI_FILENAME  "zk_gb2316_32hei.bin"                  // "字库文件名",若从文件读取，则配置文件名
#define CFG_GB2312_32_FANG              zk_disable                             // "32点阵仿宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
    #define CFG_GB2312_32_FANG_FILENAME "zk_gb2316_32fang.bin"                 // "字库文件名",若从文件读取，则配置文件名
//*******************************  Configure graphical kernel  ******************************************//
#define     CFG_GKERNEL_CMD_DEEP             1024                              // "缓冲区长度",gdd向gkernel传递命令的缓冲区长度（字节数）
//*******************************  Configure graphical decorate development(gdd)  ******************************************//
#define     CFG_DESKTOP_WIDTH                0                                // "桌面宽度",桌面尺寸（像素数）宽度，0=显示器宽度
#define     CFG_DESKTOP_HEIGHT               0                                // "桌面高度",桌面尺寸（像素数）高度，0=显示器高度
#define     CFG_DISPLAY_NAME                 "LCD_F7"                         // "显示器名",须与bsp中显示器驱动模块配置的显示器名字相同
#define     CFG_DESKTOP_NAME                 "Mydesktop"                      // "桌面名"
#define     CFG_INPUTDEV_NAME                "INPUTDEV_NAME"                  // "输入设备名称",使用bsp中输入设备所配置的名字，多输入设备的话，每个设备间用逗号隔开
#define     CFG_DESKTOP_FORMAT               CN_SYS_PF_RGB565                 // "像素格式",桌面窗口像素格式，常数在gkernel.h中定义，一般使用与显示器相同颜色
#define     CFG_GRAY_BASE_COLOR              CN_COLOR_WHITE                   // "灰度基色",像素格式设为灰度时才需要设置的“最亮”色，可在gkernel.h中找到常用颜色定义
#define     CFG_FILL_COLOR                   CN_COLOR_BLUE                    // "填充色",创建桌面时的填充色，用888格式，可在gkernel.h中找到常用颜色定义
//*******************************  Configure human machine interface  ******************************************//
#define     CFG_HMIIN_DEV_LIMIT              2                                // "name",人机交互输入设备数量，如键盘、鼠标等
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true             // "是否支持多种输入设备",
#define CFG_STDIO_STDOUT_FOLLOW    true             // "stdout是否跟随stdin",
#define CFG_STDIO_STDERR_FOLLOW    true             // "stderr是否跟随stdin",
#define CFG_STDIO_FLOAT_PRINT       true                 // "支持浮点打印"
#define CFG_STDIO_STDIOFILE         true                 // "支持标准IO文件"
#define CFG_STDIO_IN_NAME              "/dev/UART1"             // "标准输入设备名",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"             // "标准输出设备名",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"             // "标准err输出设备名",
//*******************************  Configure shell  ******************************************//
#define    CFG_SHELL_STACK                 0x1000                    // "执行shell命令的栈尺寸"
#define    CFG_ADD_ROUTINE_SHELL           true                      // "是否添加常规shell命令"
#define    CFG_ADD_EXPAND_SHELL            true                      // "是否添加拓展shell命令"
#define    CFG_ADD_GLOBAL_FUN              false                     // "添加全局函数到shell"
#define    CFG_SHOW_ADD_SHEELL             true                      // "显示在编译窗口添加的shell命令"
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure tpl  ******************************************//
#define    CFG_TPL_PROTONUM                5                         // "支持的传输协议数"，占用一个 tagTplProtoItem 结构
//*******************************  Configure tcpip  ******************************************//
#define    CFG_NETPKG_MEMSIZE              0x4000                    // "数据包缓冲区尺寸"
#define  CFG_MODULE_ENABLE_TCPIP         true
#define  CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure icmp  ******************************************//
#define  CFG_MODULE_ENABLE_ICMP          true
//*******************************  Configure router  ******************************************//
#define    CFG_IP_STRMAX                   20                        // 最大路由条目数
#define  CFG_MODULE_ENABLE_ROUTER        true
//*******************************  Configure dhcp  ******************************************//
#define    CFG_DHCPD_ENABLE                false              // "DHCP 服务器使能"
#define    CFG_DHCPC_ENABLE                true               // "DHCP 客户端使能"
#define    CFG_DHCP_RENEWTIME              3600               // "renew timer",秒数
#define    CFG_DHCP_REBINDTIME             3600               // "rebind timer",秒数
#define    CFG_DHCP_LEASETIME              3600               // "lease timer",秒数
#define    CFG_DHCPD_IPNUM                 0x40               // "IP池尺寸",64
#define    CFG_DHCPD_IPSTART               "192.168.0.2"      // "DHCP起始IP",
#define    CFG_DHCPD_SERVERIP              "192.168.0.253"    // "DHCP SERVER IP"
#define    CFG_DHCPD_ROUTERIP              "192.168.0.253"    // "DHCP ROUTER SERVER IP"
#define    CFG_DHCPD_NETIP                 "255.255.255.0"    // "DHCP MASK IP"
#define    CFG_DHCPD_DNS                   "192.168.0.253"    // "DHCP DNSSERVER IP"
#define    CFG_DHCPD_DOMAINNAME            "domain"           // "DHCP domain name"
#define  CFG_MODULE_ENABLE_DHCP          true
//*******************************  Configure tcp  ******************************************//
#define     CFG_TCP_REORDER                  true                              // "TCP乱序重组使能",资源充足才打开
#define     CFG_TCP_CCBNUM                   10                                // "tcp 客户端数限值"，占一个 指针 和 struct ClienCB
#define     CFG_TCP_SCBNUM                   5                                 // "tcp 服务器数限值"，占一个 指针 和 struct ServerCB
#define  CFG_MODULE_ENABLE_TCP           true
//*******************************  Configure ftp  ******************************************//
#define    CFG_FTPD_ENABLE                 false                     // "tftp 服务器使能",暂未实现
#define    CFG_FTPC_ENABLE                 true                      // "tftp 客户端使能"
#define  CFG_MODULE_ENABLE_FTP           true
//*******************************  Configure telnet  ******************************************//
#define    CFG_TELNETD_ENABLE              true                      // "telnet 服务器使能"
#define    CFG_TELNETC_ENABLE              false                     // "telnet 客户端使能"
#define  CFG_MODULE_ENABLE_TELNET        true
//*******************************  Configure tftp  ******************************************//
#define    CFG_FTPD_ENABLE                 true               // "ftp 服务器使能"
#define    CFG_FTPC_ENABLE                 false              // "ftp 客户端使能"
#define    CFG_TFTP_PATHDEFAULT            "/"                // TFTP默认工作目录
#define  CFG_MODULE_ENABLE_TFTP          true
//*******************************  Configure arp  ******************************************//
#define    CFG_ARP_HASHLEN                 32                        // "ARP哈希表长度"，占用一个指针
#define  CFG_MODULE_ENABLE_ARP           true
//*******************************  Configure ppp  ******************************************//
#define  CFG_MODULE_ENABLE_PPP           true
//*******************************  Configure sock  ******************************************//
#define CFG_TCP_REORDER  false
#define     CFG_SOCKET_NUM              10      //占一个 tagItem 结构
//*******************************  Configure udp  ******************************************//
#define    CFG_UDP_CBNUM                   10                        // "UDP socket数限值"，占用一个 tagUdpCB 结构
#define    CFG_UDP_HASHLEN                 4                         // "udp socket 哈希表长度"，占用一个指针
#define    CFG_UDP_PKGMSGLEN               1472                      // udp最大包长度
#define    CFG_UDP_BUFLENDEFAULT           0x2000                    // 8KB
#define  CFG_MODULE_ENABLE_UDP           true
//*******************************  Configure network config  ******************************************//
#define    CFG_STATIC_IP                   true               //  "使用静态IP?",
#define    CFG_SELECT_NETCARD              "STM32H7_ETH"      //  "网卡名称",必须与选中的网卡驱动中配置的名称相同
#define    CFG_MY_IPV4                     "192.168.0.179"    //  "静态IP",
#define    CFG_MY_SUBMASK                  "255.255.255.0"    //  "子网掩码",
#define    CFG_MY_GATWAY                   "192.168.0.1"      //  "网关",
#define    CFG_MY_DNS                      "192.168.0.1"      //  "DNS",
//*******************************  Configure ethernet phy lan8720  ******************************************//
#define    CFG_LAN8720_PHY_ADDRESS         0                  //  phy的物理地址，硬件一般设计为 0
//*******************************  Configure cpu onchip MAC  ******************************************//
#define    CFG_ETH_NETCARD_NAME            "STM32H7_ETH"      //  "网卡名称",
#define    CFG_ETH_LOOP_CYCLE              1000               //  "网卡轮询周期(uS)",中断模式无须填写
#define    CFG_ETH_LOOP_ENABLE             false              //  "网卡接收是否轮询",
#define    CFG_ETH_HARD_MAC_ADDR           true               //  "硬件生成Mac地址",
#define    CFG_ETH_MAC_ADDR0               00                 //  "MAC ADDR0",若选中"硬件生成Mac地址",则无须填写
#define    CFG_ETH_MAC_ADDR1               02                 //  "MAC ADDR1",若选中"硬件生成Mac地址",则无须填写
#define    CFG_ETH_MAC_ADDR2               03                 //  "MAC ADDR2",若选中"硬件生成Mac地址",则无须填写
#define    CFG_ETH_MAC_ADDR3               04                 //  "MAC ADDR3",若选中"硬件生成Mac地址",则无须填写
#define    CFG_ETH_MAC_ADDR4               05                 //  "MAC ADDR4",若选中"硬件生成Mac地址",则无须填写
#define    CFG_ETH_MAC_ADDR5               06                 //  "MAC ADDR5",若选中"硬件生成Mac地址",则无须填写
//*******************************  Configure Software Timers  ******************************************//
#define CFG_TIMERS_LIMIT        5                // "定时器数量",可创建的定时器数量（不包含图形界面的定时器）
#define CFG_TIMER_SOUCE_HARD    true             // "硬件定时器提供时钟源",选择专用硬件还是tick/tickless做时钟源
//*******************************  Configure kernel  ******************************************//
#define     CFG_INIT_STACK_SIZE              4096                          // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define     CFG_EVENT_LIMIT                  20                            // "事件数量限值",事件数量
#define     CFG_EVENT_TYPE_LIMIT             20                            // "事件类型数限值",事件类型数
#define     CFG_MAINSTACK_LIMIT              4096                          // "main函数栈尺寸",main函数运行所需的栈尺寸
#define     CFG_IDLESTACK_LIMIT              1024                          // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define     CFG_OS_TINY                      false                         // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8            // "对象数初始值"，用完会自动扩充
#define CFG_HANDLE_LIMIT        8            // "句柄数初始值"，用完会自动扩充
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10                      // "内存池数量限值",
//*******************************  Configure time  ******************************************//
#define     CFG_LOCAL_TIMEZONE               8                                 // "时区",北京时间是东8区
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN                     // EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL                      // 是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_IBOOT_VERSION       01                        // Iboot发布版本号
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"                           // "Iboot保存"
//*******************************  Configure ioiicconfig  ******************************************//
#define    IO_IIC_BUS_NAME                 "IoIic"             // 
//*******************************  Configure touchscreen FT5X26  ******************************************//
#define    CT_MAX_TOUCH                    5                         // "触控数",支持最多5点触摸
#define    CFG_TOUCH_ADJUST_FILE           "/yaf2/touch_init.dat"        // 保存触摸屏矫正参数的文件
#define    CFG_FT5X26_BUS_NAME             "Ioiic2"                   // "IIC总线名称",触摸芯片使用的IIC总线名称
#define    CFG_FT5X26_TOUCH_NAME           "FT5X26"                  // "触摸屏名称",配置触摸屏名称
#define    CFG_DESKTOP_NAME                "Mydesktop"               // "桌面名称",配置触摸屏所在显示器桌面的名称
//*******************************  Configure IIC to 8gpio pcf8574  ******************************************//
#define    CFG_PCF8574_BUS_NAME            "IoIic"                   // "IIC总线名"，
//*******************************  Configure cpu onchip lcd  ******************************************//
#define    CFG_LCD_XSIZE                   1024                      // "LCD水平像素宽度",
#define    CFG_LCD_YSIZE                   600                       // "LCD竖直像素宽度",
#define    CFG_DISPLAY_NAME                "LCD_F7"                  // "显示器LCD名称",
#define    CFG_LCD_HEAP_NAME               "extram"                  // "LCD使用堆名称",
//*******************************  Configure cpu peri random  ******************************************//
#define    CFG_RNG_NUM_MAX                 65535                     // "随机数最大值",
#define    CFG_RNG_NUM_MIN                 1024                      // "随机数最小值",
//*******************************  Configure cpu_peri_cpu peri uart  ******************************************//
#define    CFG_UART_SENDBUF_LEN            32                        // "串口发送环形缓冲区大小",
#define    CFG_UART_RECVBUF_LEN            32                        // "串口接收环形缓冲区大小",
#define    CFG_UART1_ENABLE                true                      // "配置是否使用UART1",
#define    CFG_UART2_ENABLE                false                     // "配置是否使用UART2",
#define    CFG_UART3_ENABLE                false                     // "配置是否使用UART3",
#define    CFG_UART4_ENABLE                false                     // "配置是否使用UART4",
#define    CFG_UART5_ENABLE                false                     // "配置是否使用UART5",
#define    CFG_UART6_ENABLE                false                     // "配置是否使用UART6",
#define    CFG_UART7_ENABLE                false                     // "配置是否使用UART7",
#define    CFG_UART8_ENABLE                false                     // "配置是否使用UART8",
//*******************************  Configure third lib yaf2 file system  ******************************************//
#define    CFG_YAF_ECC                     0                 //YAF文件系统文件使能设备ECC功能。0不使用ecc，YAF2_ENABLE_DEVICE_ECC使用ecc
#define    CFG_YAF_INSTALL_OPTION          MS_INSTALLCREAT   //YAF文件系统安装选项，安装时是否格式化整个文件系统；
#define    CFG_YAF_MOUNT_POINT             "yaf2"            //YAF文件系统安装目录
//*******************************  Configure cpu peri nand  ******************************************//
#define    CFG_NFLASH_PART_FORMAT          false             //是否需要擦除该芯片。
//*******************************  Configure nand_install_yaf  ******************************************//
#define    CFG_NFLASH_YAFFSMOUNT_NAME      "yaf2"            //需安装的文件系统的mount的名字
#define    CFG_NFLASH_YAF_PART_START       0                 //分区起始，填写块号，块号从0开始计算
#define    CFG_NFLASH_YAF_PART_END         -1                //分区结束，-1表示最后一块
#define    CFG_NFLASH_YAF_PART_FORMAT      false             //分区选项,是否需要格式化该分区。
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //主频，内核要用，必须定义

#endif
