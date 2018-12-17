/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40                // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure heap  ******************************************//
#define     CFG_DYNAMIC_MEM                  true                    // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       10                // "设备数量",定义设备数量
//*******************************  Configure filesystem  ******************************************//
#define CFG_CLIB_BUFFERSIZE            512            // "C库文件用户态缓冲区尺寸"
//*******************************  Configure stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)     // "STDIO模式",
#define     CFG_STDIO_IN_NAME                "/dev/UART3"           // "标准输入设备名",
#define     CFG_STDIO_OUT_NAME               "/dev/UART3"           // "标准输出设备名",
#define     CFG_STDIO_ERR_NAME               "/dev/UART3"           // "标准err输出设备名",
//*******************************  Configure cpu_peri_uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN       64                // "UART1发送环形缓冲区大小",
#define CFG_UART1_RECVBUF_LEN       64                // "UART1接收环形缓冲区大小",
#define CFG_UART1_DMABUF_LEN        64                // "UART1 DMA环形缓冲区大小",
#define CFG_UART2_SENDBUF_LEN       64                // "UART2发送环形缓冲区大小",
#define CFG_UART2_RECVBUF_LEN       64                // "UART2接收环形缓冲区大小",
#define CFG_UART2_DMABUF_LEN        64                // "UART2 DMA环形缓冲区大小",
#define CFG_UART3_SENDBUF_LEN       64                // "UART3发送环形缓冲区大小",
#define CFG_UART3_RECVBUF_LEN       64                // "UART3接收环形缓冲区大小",
#define CFG_UART3_DMABUF_LEN        64                // "UART3 DMA环形缓冲区大小",
#define CFG_UART4_SENDBUF_LEN       64                // "UART4发送环形缓冲区大小",
#define CFG_UART4_RECVBUF_LEN       64                // "UART4接收环形缓冲区大小",
#define CFG_UART4_DMABUF_LEN        64                // "UART4 DMA环形缓冲区大小",
#define CFG_UART5_SENDBUF_LEN       64                // "UART5发送环形缓冲区大小",
#define CFG_UART5_RECVBUF_LEN       64                // "UART5接收环形缓冲区大小",
#define CFG_UART5_DMABUF_LEN        64                // "UART5 DMA环形缓冲区大小",
#define CFG_UART6_SENDBUF_LEN       64                // "UART6发送环形缓冲区大小",
#define CFG_UART6_RECVBUF_LEN       64                // "UART6接收环形缓冲区大小",
#define CFG_UART6_DMABUF_LEN        64                // "UART6 DMA环形缓冲区大小",
#define CFG_UART7_SENDBUF_LEN       64                // "UART7发送环形缓冲区大小",
#define CFG_UART7_RECVBUF_LEN       64                // "UART7接收环形缓冲区大小",
#define CFG_UART7_DMABUF_LEN        64                // "UART7 DMA环形缓冲区大小",
#define CFG_UART8_SENDBUF_LEN       64                // "UART8发送环形缓冲区大小",
#define CFG_UART8_RECVBUF_LEN       64                // "UART8接收环形缓冲区大小",
#define CFG_UART8_DMABUF_LEN        64                // "UART8 DMA环形缓冲区大小",
#define CFG_UART1_ENABLE           true                  // "是否使用UART1",
#define CFG_UART2_ENABLE           true                 // "是否使用UART2",
#define CFG_UART3_ENABLE           true                 // "是否使用UART3",
#define CFG_UART4_ENABLE           true                 // "是否使用UART4",
#define CFG_UART5_ENABLE           true                 // "是否使用UART5",
#define CFG_UART6_ENABLE           true                 // "是否使用UART6",
#define CFG_UART7_ENABLE           true                 // "是否使用UART7",
#define CFG_UART8_ENABLE           true                 // "是否使用UART8",
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE               0x4000                 // "数据包缓冲区尺寸"
#define     CFG_ARP_HASHLEN                  32                     // "ARP哈希表长度"，占用一个指针
#define     CFG_TPL_PROTONUM                 5                      // "支持的传输协议数"，占用一个 tagTplProtoItem 结构
#define     CFG_UDP_ENABLE                   true                   // "udp协议使能"
#define     CFG_UDP_CBNUM                    10                     // "UDP socket数限值"，占用一个 tagUdpCB 结构
#define     CFG_UDP_HASHLEN                  4                      // "udp socket 哈希表长度"，占用一个指针
#define     CFG_TCP_ENABLE                   true                   // "tcp协议使能"
#define     CFG_TCP_REORDER                  true                   // "TCP乱序重组使能",资源充足才打开
#define     CFG_TCP_CCBNUM                   10                     // "tcp 客户端数限值"，占一个 指针 和 tagCCB
#define     CFG_TCP_SCBNUM                   5                      // "tcp 服务器数限值"，，占一个 指针 和 tagSCB
#define     CFG_SOCKET_NUM                   10                     // "socket数限值"，占一个 tagItem 结构
#define     CFG_TELNET_ENABLE                true                   // "telnet 使能"
#define     CFG_TELNETD_ENABLE               true                   // "telnet 服务器使能"
#define     CFG_TELNETC_ENABLE               false                  // "telnet 客户端使能"
#define     CFG_FTP_ENABLE                   true                   // "ftp 使能"
#define     CFG_FTPD_ENABLE                  true                   // "ftp 服务器使能"
#define     CFG_FTPC_ENABLE                  false                  // "ftp 客户端使能"
#define     CFG_TFTP_ENABLE                  true                   // "tftp 使能"
#define     CFG_TFTPD_ENABLE                 false                  // "tftp 服务器使能"
#define     CFG_TFTPC_ENABLE                 true                   // "tftp 客户端使能"
#define     CN_TFTP_PATHDEFAULT              "/efs/"                // TFTP默认工作目录
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
#define     CFG_DHCPD_DOMAINNAME             ""                     // "DHCP domain name"
#define     CN_PPP_ENABLE                    1                      // 是否需要 ppp 协议
#define CFG_MYCONFIG6               // "name",我的配置
#define     CFG_MYCONFIG7                    CN_CONFIG              // 我的配置
//*******************************  Configure timer  ******************************************//
#define CFG_TIMER_SOUCE     1           // "时钟源",1=由硬件计时器提供时钟源，0=由tick提供时钟源
#define CFG_TIMERS_LIMIT    5           // "定时器数量",可创建的定时器数量（不包含图形界面的定时器）
//*******************************  Configure kernel  ******************************************//
#define     CFG_INIT_STACK_SIZE              4096                    // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define     CFG_EVENT_LIMIT                  20                      // "事件数量限值",事件数量
#define     CFG_EVENT_TYPE_LIMIT             20                      // "事件类型数限值",事件类型数
#define     CFG_MAINSTACK_LIMIT              4096                    // "main函数栈尺寸",main函数运行所需的栈尺寸
#define     CFG_IDLESTACK_LIMIT              1024                    // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define     CFG_OS_TINY                      false                   // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure MemoryPool  ******************************************//
#define     CFG_MEMPOOL_LIMIT                10                      // "内存池数量",允许建立10个内存池
//*******************************  Configure time  ******************************************//
#define     CFG_LOCAL_TIMEZONE               8                       // "时区",北京时间是东8区
//*******************************  Configure loader  ******************************************//
#define     CFG_APP_RUNMODE                  EN_DIRECT_RUN           // EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define     CFG_IBOOT_CRC                    EN_NO_APP_CRC           // 是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define     CFG_APP_FILENAME                 ""                      // CFG_APP_RUNMODE==EN_FORM_FILE，这里配置文件名
#define     CFG_IBOOT_VERSION                "V1.0.0"                // Iboot发布版本号
#define     CFG_IBOOT_UPDATE_NAME            "iboot.bin"             // iboot更新文件名，此二配置，以后改掉
#define     CFG_IBOOT_UPDATE_PATH            "\\"                    // iboot更新路径名，此二配置，以后改掉
//*******************************  Configure cpu_peri_iic  ******************************************//
#define CFG_IIC_BUF_LEN           32           // "IIC缓冲区大小",配置IIC缓冲区大小
#define CFG_IIC1_ENABLE           false         // "是否使用IIC1",
#define CFG_IIC2_ENABLE           false         // "是否使用IIC2",
#define CFG_IIC3_ENABLE           false         // "是否使用IIC3",
#define CFG_IIC4_ENABLE           false         // "是否使用IIC4",
//*******************************  Configure cpu_peri_eth  ******************************************//
#define     CFG_MAC_ADDR0                    00                    // "网卡地址0",
#define     CFG_MAC_ADDR1                    00                    // "网卡地址1",
#define     CFG_MAC_ADDR2                    00                    // "网卡地址2",
#define     CFG_MAC_ADDR3                    00                    // "网卡地址3",
#define     CFG_MAC_ADDR4                    00                    // "网卡地址4",
#define     CFG_MAC_ADDR5                    00                    // "网卡地址5",
#define     CFG_ETH_LOOP_CYCLE               1000                  // "网卡轮询接收周期",
#define     CFG_ETH_LOOP_ENABLE              true                  // "网卡接收是否轮询",
#define     CFG_ETH_DEV_NAME                 "STM32F7_ETH"         // "网卡名称",
//*******************************  Configure netconfig  ******************************************//
#define     CFG_STATIC_IP                    true               // "IP属性",true=使用静态IP，false=动态IP
#define     CFG_NETCARD_NAME                 "NUCLEO-F767ZI_ETH" // "网卡名",
#define     CFG_MY_IPV4                      "192.168.0.179"    // "静态IP",
#define     CFG_MY_SUBMASK                   "255.255.255.0"    // "子网掩码",
#define     CFG_MY_GATWAY                    "192.168.0.1"      // "网关",
#define     CFG_MY_DNS                       "192.168.0.1"      // "DNS",
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (216*Mhz)         //主频，内核要用，必须定义

#endif
