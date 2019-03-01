/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40           // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true       // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       10           // "设备数量",定义设备数量
//*******************************  Configure ioiicbus  ******************************************//
#define IO_IIC_USER_TAG      0       // 用户自己的标签用作在回调用户提供的IO控制函数的第二个参数
#define IO_IIC_BUS_NAME "IoIic"     // 该IIC总线的名字
#define IIC_IOCTRL_FUN   IIC_IoCtrlFunc   // 用户实现的IO控制函数名
//*******************************  Configure filesystem  ******************************************//
#define CFG_CLIB_BUFFERSIZE            512           // "C库文件用户态缓冲区尺寸"
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_RUN_MODE            (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)      // "STDIO模式",
#define CFG_STDIO_IN_NAME              "/dev/UART1"         // "标准输入设备名",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"         // "标准输出设备名",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"         // "标准err输出设备名",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define CFG_UART_SENDBUF_LEN            32                       // "串口发送环形缓冲区大小",
#define CFG_UART_RECVBUF_LEN            32                       // "串口接收环形缓冲区大小",
#define CFG_UART1_ENABLE                true                     // "配置是否使用UART1",
#define CFG_UART2_ENABLE                false                    // "配置是否使用UART2",
#define CFG_UART3_ENABLE                false                    // "配置是否使用UART3",
#define CFG_UART4_ENABLE                false                    // "配置是否使用UART4",
#define CFG_UART5_ENABLE                false                    // "配置是否使用UART5",
#define CFG_UART6_ENABLE                false                    // "配置是否使用UART6",
#define CFG_UART7_ENABLE                false                    // "配置是否使用UART7",
#define CFG_UART8_ENABLE                false                    // "配置是否使用UART8",
//*******************************  Configure shell  ******************************************//
#define     CFG_ADD_ROUTINE_SHELL            true                   // "是否添加常规shell命令"
#define     CFG_ADD_EXPAND_SHELL             true                   // "是否添加拓展shell命令"
#define     CFG_ADD_GLOBAL_FUN               false                  // "添加全局函数到shell"
#define     CFG_SHOW_ADD_SHEELL              true                   // "显示在编译窗口添加的shell命令"
//*******************************  Configure tcp  ******************************************//
#define     CFG_TCP_REORDER                  true                   // "TCP乱序重组使能",资源充足才打开
#define     CFG_TCP_CCBNUM                   10                     // "tcp 客户端数限值"，占一个 指针 和 struct ClienCB
#define     CFG_TCP_SCBNUM                   5                      // "tcp 服务器数限值"，占一个 指针 和 struct ServerCB
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE               0x4000                 // "数据包缓冲区尺寸"
#define     CFG_ARP_HASHLEN                  32                     // "ARP哈希表长度"，占用一个指针
#define     CFG_TPL_PROTONUM                 5                      // "支持的传输协议数"，占用一个 tagTplProtoItem 结构
#define     CFG_UDP_ENABLE                   true                   // "udp协议使能"
#define     CFG_UDP_CBNUM                    10                     // "UDP socket数限值"，占用一个 tagUdpCB 结构
#define     CFG_UDP_HASHLEN                  4                      // "udp socket 哈希表长度"，占用一个指针
#define     CFG_TCP_ENABLE                   true                   // #define     CFG_TCP_ENABLE              true    
#define     CFG_TCP_REORDER                  true                   // #define     CFG_TCP_REORDER             true    
#define     CFG_TCP_CCBNUM                   10                     // #define     CFG_TCP_CCBNUM              10      
#define     CFG_TCP_SCBNUM                   5                      // #define     CFG_TCP_SCBNUM              5       
#define     CFG_SOCKET_NUM                   10                     // "socket数限值"，占一个 tagItem 结构
#define     CFG_TELNET_ENABLE                true                   // "telnet 使能"
#define     CFG_TELNETD_ENABLE               true                   // "telnet 服务器使能"
#define     CFG_TELNETC_ENABLE               false                  // "telnet 客户端使能"
#define     CFG_FTP_ENABLE                   true                   // "ftp 使能"
#define     CFG_FTPD_ENABLE                  true                   // "ftp 服务器使能"
#define     CFG_FTPC_ENABLE                  false                  // "ftp 客户端使能"
#define     CFG_TFTP_ENABLE                  true                   // "tftp 使能"
#define     CFG_TFTPD_ENABLE                 false                  // "tftp 服务器使能",暂未实现
#define     CFG_TFTPC_ENABLE                 true                   // "tftp 客户端使能"
#define     CFG_TFTP_PATHDEFAULT             "/"                       // TFTP默认工作目录
#define     CFG_DHCP_ENABLE                  true                   // "DHCP 使能"
#define     CFG_DHCPD_ENABLE                 false                  // "DHCP 服务器使能"
#define     CFG_DHCPC_ENABLE                 true                   // "DHCP 客户端使能"
#define     CFG_DHCP_RENEWTIME               3600                   // "renew timer",秒数
#define     CFG_DHCP_REBINDTIME              3600                   // "rebind timer",秒数
#define     CFG_DHCP_LEASETIME               3600                   // "lease timer",秒数
#define     CFG_DHCPD_IPNUM                  0x40                   // "IP池尺寸",64
#define     CFG_DHCPD_IPSTART                "192.168.0.2"          // "DHCP起始IP",
#define     CFG_DHCPD_SERVERIP               "192.168.0.253"        // "DHCP SERVER IP"
#define     CFG_DHCPD_ROUTERIP               "192.168.0.253"        // "DHCP ROUTER SERVER IP"
#define     CFG_DHCPD_NETIP                  "255.255.255.0"        // "DHCP MASK IP"
#define     CFG_DHCPD_DNS                    "192.168.0.253"        // "DHCP DNSSERVER IP"
#define     CFG_DHCPD_DOMAINNAME             "domain"               // "DHCP domain name"
#define     CFG_PPP_ENABLE                   false                  // 是否需要 ppp 协议
//*******************************  Configure timer  ******************************************//
#define CFG_TIMER_SOUCE     1            // "时钟源",1=由硬件计时器提供时钟源，0=由tick提供时钟源
#define CFG_TIMERS_LIMIT    5            // "定时器数量",可创建的定时器数量（不包含图形界面的定时器）
//*******************************  Configure ymodem  ******************************************//
#define     CFG_YMODEM_BUF_NUM               32                     // "缓存ymodem包数量",每包1024字节
#define     CFG_YMODEM_PKG_TIMEOUT           (15*1000*1000)         // "包间隔超时时间",微秒
#define     CFG_YMODEM_TIMEOUT               (300*1000*1000)        // "ymodem传输总超时时间",微秒
#define     CFG_YMODEM_DEVNAME               "/dev/UART1"                  // "ymodem传输设备，std表示使用标准输入输出设备"
#define     CFG_YMODEM_TIMEOUT               (600*1000*1000)        // #define CFG_YMODEM_TIMEOUT (600*1000*1000)
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096         // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         15           // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    15           // "事件类型数限值",事件类型数
#define CFG_MAINSTACK_LIMIT     4096         // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024         // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_OS_TINY             false        // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10           // "内存池数量",允许建立10个内存池
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8           // "时区",北京时间是东8区
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN          // EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL           // 是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_IBOOT_VERSION       01             // Iboot发布版本号
//*******************************  Configure cpu_peri_eth  ******************************************//
#define     CFG_MAC_ADDR0                    00                 // "网卡地址0",
#define     CFG_MAC_ADDR1                    00                 // "网卡地址1",
#define     CFG_MAC_ADDR2                    00                 // "网卡地址2",
#define     CFG_MAC_ADDR3                    00                 // "网卡地址3",
#define     CFG_MAC_ADDR4                    00                 // "网卡地址4",
#define     CFG_MAC_ADDR5                    00                 // "网卡地址5",
#define     CFG_ETH_LOOP_CYCLE               1000               // "网卡轮询接收周期",
#define     CFG_ETH_LOOP_ENABLE              true               // "网卡接收是否轮询",
#define     CFG_ETH_DEV_NAME                 "STM32H7_ETH"      // "网卡名称",
//*******************************  Configure lan8720  ******************************************//
#define CFG_LAN8720_PHY_ADDRESS     0        // phy的物理地址，硬件一般设计为 0
//*******************************  Configure netconfig  ******************************************//
#define     CFG_STATIC_IP                    true                   // "IP属性",true=使用静态IP，false=动态IP
#define     CFG_NETCARD_NAME                 "NUCLEO_H743_ETH"      // "网卡名",
#define     CFG_MY_IPV4                      "192.168.0.179"        // "静态IP",
#define     CFG_MY_SUBMASK                   "255.255.255.0"        // "子网掩码",
#define     CFG_MY_GATWAY                    "192.168.0.1"          // "网关",
#define     CFG_MY_DNS                       "192.168.0.1"          // "DNS",
//*******************************  Configure PCF8574  ******************************************//
#define     CFG_PCF8574_BUS_NAME             "IoIic"              // "name"
//*******************************  Configure cpu_peri_emflash  ******************************************//
#define     CFG_EFLASH_PART_START            0                   // 分区起始
#define     CFG_EFLASH_PART_END              4                   // 分区结束，-1代表至存储模块结束地址
#define     CFG_EFLASH_PART_OPTION           0                   // 分区选项
#define     CFG_EFLASH_FSMOUNT_NAME          EN_XIP_IBOOT_TARGET  // 需安装的文件系统的mount的名字，NULL表示该flash不挂载文件系统
//*******************************  Configure yaf2filesystem  ******************************************//
#define     CFG_YAF_ECC                      0                     // YAF文件系统文件使能设备ECC功能。0不使用ecc，YAF2_ENABLE_DEVICE_ECC使用ecc
#define     CFG_YAF_INSTALL_OPTION           false                 // YAF文件系统安装选项，安装时是否格式化整个文件系统；
#define     CFG_YAF_MOUNT_POINT              "yaf2"                // "name",YAF文件系统安装目录
//*******************************  Configure cpu_peri_nand  ******************************************//
#define     CFG_NFLASH_FSMOUNT_NAME          "yaf2"         // 需安装的文件系统的mount的名字
#define     CFG_NFLASH_PART_START            0                 // 分区起始
#define     CFG_NFLASH_PART_END              -1                // 分区结束
#define     CFG_NFLASH_PART_OPTION           0                 // 分区选项
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //主频，内核要用，必须定义

#endif
