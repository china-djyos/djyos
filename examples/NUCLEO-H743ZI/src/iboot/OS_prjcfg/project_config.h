/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40       // "锁的数量",定义锁的数量，包含信号量和互斥量
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true   // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10       // "设备数量",定义设备数量
//*******************************  Configure file system  ******************************************//
#define     CFG_CLIB_BUFFERSIZE              512                // "C库文件用户态缓冲区尺寸"
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_RUN_MODE            (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)  // "STDIO模式",
#define CFG_STDIO_IN_NAME              "/dev/UART3"     // "标准输入设备名",
#define CFG_STDIO_OUT_NAME             "/dev/UART3"     // "标准输出设备名",
#define CFG_STDIO_ERR_NAME             "/dev/UART3"     // "标准err输出设备名",
//*******************************  Configure cpu peri uart  ******************************************//
#define CFG_UART_SENDBUF_LEN            32                   // "串口发送环形缓冲区大小",
#define CFG_UART_RECVBUF_LEN            32                   // "串口接收环形缓冲区大小",
#define CFG_UART1_ENABLE                false                 // "配置是否使用UART1",
#define CFG_UART2_ENABLE                false                // "配置是否使用UART2",
#define CFG_UART3_ENABLE                true                // "配置是否使用UART3",
#define CFG_UART4_ENABLE                false                // "配置是否使用UART4",
#define CFG_UART5_ENABLE                false                // "配置是否使用UART5",
#define CFG_UART6_ENABLE                false                // "配置是否使用UART6",
#define CFG_UART7_ENABLE                false                // "配置是否使用UART7",
#define CFG_UART8_ENABLE                false                // "配置是否使用UART8",
//*******************************  Configure shell  ******************************************//
#define     CFG_SHELL_LEVEL                  SHELL_LEVEL_BASE   // "shell命令实现级别",用于定义shell支持的命令数量
#define     CFG_SHOW_ADD_SHEELL              true               // #define CFG_SHOW_ADD_SHEELL        true        
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE               0x4000            //
#define     CFG_ARP_HASHLEN                  32                //占用一个指针
#define     CFG_TPL_PROTONUM                 5                 //占用一个 tagTplProtoItem 结构
#define     CFG_UDP_ENABLE                   true              //
#define     CFG_UDP_CBNUM                    10                //占用一个 tagUdpCB 结构
#define     CFG_UDP_HASHLEN                  4                 //占用一个指针
#define     CFG_TCP_ENABLE                   true              //
#define     CFG_TCP_REORDER                  true              //资源充足才打开
#define     CFG_TCP_CCBNUM                   10                //占一个 指针 和 tagCCB
#define     CFG_TCP_SCBNUM                   5                 //占一个 指针 和 tagSCB
#define     CFG_SOCKET_NUM                   10                //占一个 tagItem 结构
#define     CFG_TELNET_ENABLE                true              //
#define     CFG_TELNETD_ENABLE               true              //
#define     CFG_TELNETC_ENABLE               false             //
#define     CFG_FTP_ENABLE                   true              //
#define     CFG_FTPD_ENABLE                  true              //
#define     CFG_FTPC_ENABLE                  false             //
#define     CFG_TFTP_ENABLE                  true              //
#define     CFG_TFTPD_ENABLE                 false             //
#define     CFG_TFTPC_ENABLE                 true              //
#define     CFG_TFTP_PATHDEFAULT              "/"          // TFTP默认工作目录
#define     CFG_DHCP_ENABLE                  true              //
#define     CFG_DHCPD_ENABLE                 false             //
#define     CFG_DHCPC_ENABLE                 true              //
#define     CFG_DHCP_RENEWTIME               3600              //秒数
#define     CFG_DHCP_REBINDTIME              3600              //秒数
#define     CFG_DHCP_LEASETIME               3600              //秒数
#define     CFG_DHCPD_IPNUM                  0x40              //64
#define     CFG_DHCPD_IPSTART                "192.168.0.2"     //
#define     CFG_DHCPD_SERVERIP               "192.168.0.253"   //"   // ",
#define     CFG_DHCPD_ROUTERIP               "192.168.0.253"   //"   // ",
#define     CFG_DHCPD_NETIP                  "255.255.255.0"   //"   // ",
#define     CFG_DHCPD_DNS                    "192.168.0.253"   //"   // ",
#define     CFG_DHCPD_DOMAINNAME             "domain"          //"         // ",
#define     CN_PPP_ENABLE                    null              //是否需要 ppp 协议
//*******************************  Configure ymodem  ******************************************//
#define     CFG_YMODEM_BUF_NUM               32                 // "缓存ymodem包数量",每包1024字节
#define     CFG_YMODEM_PKG_TIMEOUT           (15*1000*1000)     // "包间隔超时时间",微秒
#define     CFG_YMODEM_TIMEOUT               (300*1000*1000)    // "ymodem传输总超时时间",微秒
#define     CFG_YMODEM_DEVNAME               "std"              // "ymodem传输设备，std表示使用标准输入输出设备"
#define     CFG_YMODEM_TIMEOUT               (600*1000*1000)    // #define CFG_YMODEM_TIMEOUT (600*1000*1000)
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096     // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         15       // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    15       // "事件类型数限值",事件类型数
#define CFG_MAINSTACK_LIMIT     4096     // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024     // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_OS_TINY             false    // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10       // "内存池数量",允许建立10个内存池
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN      // EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL       // 是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_IBOOT_VERSION       01         // Iboot发布版本号
//*******************************  Configure cpu onchip MAC  ******************************************//
#define CFG_MAC_ADDR0           00            // "网卡地址0",
#define CFG_MAC_ADDR1           00            // "网卡地址1",
#define CFG_MAC_ADDR2           00            // "网卡地址2",
#define CFG_MAC_ADDR3           00            // "网卡地址3",
#define CFG_MAC_ADDR4           00            // "网卡地址4",
#define CFG_MAC_ADDR5           00            // "网卡地址5",
#define CFG_ETH_LOOP_CYCLE      1000          // "网卡轮询接收周期",
#define CFG_ETH_LOOP_ENABLE     true         // "网卡接收是否轮询",
#define CFG_ETH_DEV_NAME        "STM32H7_ETH" // "网卡名称",
//*******************************  Configure network config  ******************************************//
#define     CFG_STATIC_IP                    true              //true=使用静态IPfalse=动态IP
#define     CFG_NETCARD_NAME                 "NUCLEO_H743_ETH" //
#define     CFG_MY_IPV4                      "192.168.0.179"   //
#define     CFG_MY_SUBMASK                   "255.255.255.0"   //
#define     CFG_MY_GATWAY                    "192.168.0.1"     //
#define     CFG_MY_DNS                       "192.168.0.1"     //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //主频，内核要用，必须定义

#endif
