/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

#define CFG_RUNMODE_BAREAPP    1
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40                  // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true              // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure DevFile  ******************************************//
#define    CFG_DEVFILE_LIMIT               10                 // "设备数量",定义设备数量
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true     // "是否支持多种输入设备",
#define CFG_STDIO_STDOUT_FOLLOW    true     // "stdout是否跟随stdin",
#define CFG_STDIO_STDERR_FOLLOW    true     // "stderr是否跟随stdin",
#define CFG_STDIO_FLOAT_PRINT       true                 // "支持浮点打印"
#define CFG_STDIO_STDIOFILE         true                 // "支持标准IO文件"
#define CFG_STDIO_IN_NAME              "/dev/UART1"     // "标准输入设备名",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"     // "标准输出设备名",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"     // "标准err输出设备名",
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000               // "执行shell命令的栈尺寸"
#define     CFG_ADD_ROUTINE_SHELL            true                        // "是否添加常规shell命令"
#define     CFG_ADD_EXPAND_SHELL             true                        // "是否添加拓展shell命令"
#define     CFG_ADD_GLOBAL_FUN               false                       // "添加全局函数到shell"
#define     CFG_SHOW_ADD_SHEELL              true                        // "显示在编译窗口添加的shell命令"
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure dhcp  ******************************************//
#define    CFG_DHCPD_ENABLE                false             //
#define    CFG_DHCPC_ENABLE                true              //
#define    CFG_DHCP_RENEWTIME              3600              //秒数
#define    CFG_DHCP_REBINDTIME             3600              //秒数
#define    CFG_DHCP_LEASETIME              3600              //秒数
#define    CFG_DHCPD_IPNUM                 0x40              //64
#define    CFG_DHCPD_IPSTART               "192.168.0.2"     //
#define    CFG_DHCPD_SERVERIP              "192.168.0.253"   //
#define    CFG_DHCPD_ROUTERIP              "192.168.0.253"   //
#define    CFG_DHCPD_NETIP                 "255.255.255.0"   //
#define    CFG_DHCPD_DNS                   "192.168.0.253"   //
#define    CFG_DHCPD_DOMAINNAME            "domain"          //
#define  CFG_MODULE_ENABLE_DHCP          true
//*******************************  Configure tcp  ******************************************//
#define     CFG_TCP_REORDER             true    //资源充足才打开
#define     CFG_TCP_CCBNUM              10      //占一个 指针 和 struct ClienCB
#define     CFG_TCP_SCBNUM              5       //占一个 指针 和 struct ServerCB
#define  CFG_MODULE_ENABLE_TCP           true
//*******************************  Configure tpl  ******************************************//
#define CFG_TCP_REORDER  false
#define     CFG_TPL_PROTONUM            5       //占用一个 tagTplProtoItem 结构
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE          0x4000  //
#define  CFG_MODULE_ENABLE_TCPIP         true
#define  CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure icmp  ******************************************//
#define  CFG_MODULE_ENABLE_ICMP          true
//*******************************  Configure router  ******************************************//
#define CFG_IP_STRMAX           20 //最大路由条目数
#define  CFG_MODULE_ENABLE_ROUTER        true
//*******************************  Configure ftp  ******************************************//
#define    CFG_FTPD_ENABLE                 false             //暂未实现
#define    CFG_FTPC_ENABLE                 true              //
#define  CFG_MODULE_ENABLE_FTP           true
//*******************************  Configure arp  ******************************************//
#define    CFG_ARP_HASHLEN                 32                //占用一个指针
#define  CFG_MODULE_ENABLE_ARP           true
//*******************************  Configure udp  ******************************************//
#define    CFG_UDP_CBNUM                   10                //占用一个 tagUdpCB 结构
#define    CFG_UDP_HASHLEN                 4                 //占用一个指针
#define    CFG_UDP_PKGMSGLEN               1472              //udp最大包长度
#define    CFG_UDP_BUFLENDEFAULT           0x2000            //8KB
#define  CFG_MODULE_ENABLE_UDP           true
//*******************************  Configure timer  ******************************************//
#define CFG_TIMERS_LIMIT        5        // "定时器数量",可创建的定时器数量（不包含图形界面的定时器）
#define CFG_TIMER_SOUCE_HARD    true     // "硬件定时器提供时钟源",选择专用硬件还是tick/tickless做时钟源
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096                // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         15                  // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    15                  // "事件类型数限值",事件类型数
#define CFG_MAINSTACK_LIMIT     4096                // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024                // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_OS_TINY             false               // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure Objfile  ******************************************//
#define CFG_OBJECT_LIMIT        8            // "对象数初始值"，用完会自动扩充
#define CFG_HANDLE_LIMIT        8            // "句柄数初始值"，用完会自动扩充
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10                  // "内存池数量限值",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define     CFG_UART1_SENDBUF_LEN            32                            // "UART1发送环形缓冲区大小",
#define     CFG_UART1_RECVBUF_LEN            32                            // "UART1接收环形缓冲区大小",
#define     CFG_UART1_DMABUF_LEN             32                            // "UART1 DMA缓冲区大小",
#define     CFG_UART2_SENDBUF_LEN            32                            // "UART2发送环形缓冲区大小",
#define     CFG_UART2_RECVBUF_LEN            32                            // "UART2接收环形缓冲区大小",
#define     CFG_UART2_DMABUF_LEN             32                            // "UART2 DMA缓冲区大小",
#define     CFG_UART3_SENDBUF_LEN            32                            // "UART3发送环形缓冲区大小",
#define     CFG_UART3_RECVBUF_LEN            32                            // "UART3接收环形缓冲区大小",
#define     CFG_UART3_DMABUF_LEN             32                            // "UART3 DMA缓冲区大小",
#define     CFG_UART4_SENDBUF_LEN            32                            // "UART4发送环形缓冲区大小",
#define     CFG_UART4_RECVBUF_LEN            32                            // "UART4接收环形缓冲区大小",
#define     CFG_UART4_DMABUF_LEN             32                            // "UART4 DMA缓冲区大小",
#define     CFG_UART5_SENDBUF_LEN            32                            // "UART5发送环形缓冲区大小",
#define     CFG_UART5_RECVBUF_LEN            32                            // "UART5接收环形缓冲区大小",
#define     CFG_UART5_DMABUF_LEN             32                            // "UART5 DMA缓冲区大小",
#define     CFG_UART6_SENDBUF_LEN            32                            // "UART6发送环形缓冲区大小",
#define     CFG_UART6_RECVBUF_LEN            32                            // "UART6接收环形缓冲区大小",
#define     CFG_UART6_DMABUF_LEN             32                            // "UART6 DMA缓冲区大小",
#define     CFG_UART1_ENABLE                 true                          // "是否使用UART1",
#define     CFG_UART2_ENABLE                 false                         // "是否使用UART2",
#define     CFG_UART3_ENABLE                 false                         // "是否使用UART3",
#define     CFG_UART4_ENABLE                 false                         // "是否使用UART4",
#define     CFG_UART5_ENABLE                 false                         // "是否使用UART5",
#define     CFG_UART6_ENABLE                 false                         // "是否使用UART6",
//*******************************  Configure netconfig  ******************************************//
#define    CFG_STATIC_IP                   true              //
#define    CFG_SELECT_NETCARD              "STM32F4_ETH"     //必须与选中的网卡驱动中配置的名称相同
#define    CFG_MY_IPV4                     "192.168.0.179"   //
#define    CFG_MY_SUBMASK                  "255.255.255.0"   //
#define    CFG_MY_GATWAY                   "192.168.0.1"     //
#define    CFG_MY_DNS                      "192.168.0.1"     //
//*******************************  Configure lan8720  ******************************************//
#define    CFG_LAN8720_PHY_ADDRESS         0                 //phy的物理地址，硬件一般设计为 0
//*******************************  Configure cpu_peri_eth  ******************************************//
#define    CFG_ETH_NETCARD_NAME            "STM32F4_ETH"     //
#define    CFG_ETH_LOOP_CYCLE              1000              //中断模式无须填写
#define    CFG_ETH_LOOP_ENABLE             true              //
#define    CFG_ETH_HARD_MAC_ADDR           true              //
#define    CFG_ETH_MAC_ADDR0               00                //若选中"硬件生成Mac地址"则无须填写
#define    CFG_ETH_MAC_ADDR1               02                //若选中"硬件生成Mac地址"则无须填写
#define    CFG_ETH_MAC_ADDR2               03                //若选中"硬件生成Mac地址"则无须填写
#define    CFG_ETH_MAC_ADDR3               04                //若选中"硬件生成Mac地址"则无须填写
#define    CFG_ETH_MAC_ADDR4               05                //若选中"硬件生成Mac地址"则无须填写
#define    CFG_ETH_MAC_ADDR5               06                //若选中"硬件生成Mac地址"则无须填写
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (168*Mhz)         //主频，内核要用，必须定义

#endif