/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40            // "锁的数量",定义锁的数量，包含信号量和互斥量
#define    CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure djybus  ******************************************//
#define    CFG_MODULE_ENABLE_DJYBUS        true
//*******************************  Configure iicbus  ******************************************//
#define    CFG_MODULE_ENABLE_IICBUS        true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true        // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure int  ******************************************//
#define    CFG_MODULE_ENABLE_INT           true
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096          // "初始化栈空间",定义初始化过程使用的栈空间，一般按默认值就可以了
#define CFG_EVENT_LIMIT         15            // "事件数量限值",事件数量
#define CFG_EVENT_TYPE_LIMIT    15            // "事件类型数限值",事件类型数
#define CFG_MAINSTACK_LIMIT     4096          // "main函数栈尺寸",main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024          // "IDLE事件栈尺寸",IDLE事件处理函数运行的栈尺寸，一般按默认值就可以了
#define CFG_OS_TINY             false         // "tiny版内核",true=用于资源紧缺的场合，内核会裁剪掉：事件类型名字，事件处理时间统计。
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure kernel object system  ******************************************//
#define    CFG_OBJECT_LIMIT                8                 //用完会自动扩充
#define    CFG_HANDLE_LIMIT                8                 //用完会自动扩充
#define    CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //定义设备数量
#define    CFG_MODULE_ENABLE_DEVICE_FILE_SYSTEM  true
//*******************************  Configure loader  ******************************************//
#define CFG_UPDATEIBOOT_EN      false       //
#define    CFG_APP_RUNMODE                 EN_DIRECT_RUN      // EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define    CFG_APP_VERIFICATION            VERIFICATION_NULL  // 是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define    CFG_IBOOT_VERSION               02                 // Iboot发布版本号
#define    CFG_IBOOT_UPDATE_NAME           "/yaf2/iboot.bin"  // "Iboot保存"
#define    CFG_MODULE_ENABLE_LOADER        true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
#define    CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure message queue  ******************************************//
#define    CFG_MODULE_ENABLE_MESSAGE_QUEUE true
//*******************************  Configure multiplex  ******************************************//
#define    CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure ring buffer and line buffer  ******************************************//
#define    CFG_MODULE_ENABLE_RING_BUFFER_AND_LINE_BUFFER  true
//*******************************  Configure stdio  ******************************************//
#define    CFG_STDIO_STDIN_MULTI           true              //
#define    CFG_STDIO_STDOUT_FOLLOW         true              //
#define    CFG_STDIO_STDERR_FOLLOW         true              //
#define    CFG_STDIO_FLOAT_PRINT           true              //
#define    CFG_STDIO_STDIOFILE             true              //
#define    CFG_STDIO_IN_NAME               "/dev/UART3"      //
#define    CFG_STDIO_OUT_NAME              "/dev/UART3"      //
#define    CFG_STDIO_ERR_NAME              "/dev/UART3"      //
#define    CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000           // "执行shell命令的栈尺寸"
#define     CFG_ADD_ROUTINE_SHELL            true                    // "是否添加常规shell命令"
#define     CFG_ADD_EXPAND_SHELL             true                    // "是否添加拓展shell命令"
#define     CFG_ADD_GLOBAL_FUN               false                   // "添加全局函数到shell"
#define     CFG_SHOW_ADD_SHEELL              true                    // "显示在编译窗口添加的shell命令"
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure tcpip  ******************************************//
#define    CFG_NETPKG_MEMSIZE              0x4000            //
#define    CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure cpu onchip MAC  ******************************************//
#define    CFG_ETH_NETCARD_NAME            "STM32F7_ETH"     //
#define    CFG_ETH_LOOP_CYCLE              1000              //中断模式无须填写
#define    CFG_ETH_LOOP_ENABLE             true              //
#define    CFG_ETH_HARD_MAC_ADDR           true              //
#define    CFG_ETH_MAC_ADDR0               00                //若选中"硬件生成Mac地址"则无须填写
#define    CFG_ETH_MAC_ADDR1               02                //若选中"硬件生成Mac地址"则无须填写
#define    CFG_ETH_MAC_ADDR2               03                //若选中"硬件生成Mac地址"则无须填写
#define    CFG_ETH_MAC_ADDR3               04                //若选中"硬件生成Mac地址"则无须填写
#define    CFG_ETH_MAC_ADDR4               05                //若选中"硬件生成Mac地址"则无须填写
#define    CFG_ETH_MAC_ADDR5               06                //若选中"硬件生成Mac地址"则无须填写
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_MAC  true
//*******************************  Configure network config  ******************************************//
#define CFG_STATIC_IP       false            //
#define CFG_SELECT_NETCARD  "STM32F7_ETH"   //必须与选中的网卡驱动中配置的名称相同
#define CFG_MY_IPV4         "192.168.0.179" //
#define CFG_MY_SUBMASK      "255.255.255.0" //
#define CFG_MY_GATWAY       "192.168.0.1"   //
#define CFG_MY_DNS          "192.168.0.1"   //
#define    CFG_MODULE_ENABLE_NETWORK_CONFIG  true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure ymodem  ******************************************//
#define     CFG_YMODEM_BUF_NUM               32                      // "缓存ymodem包数量",每包1024字节
#define     CFG_YMODEM_PKG_TIMEOUT           (15*1000*1000)          // "包间隔超时时间",微秒
#define     CFG_YMODEM_TIMEOUT               (300*1000*1000)         // "ymodem传输总超时时间",微秒
#define     CFG_YMODEM_DEVNAME               "std"                   // "ymodem传输设备，std表示使用标准输入输出设备"
#define    CFG_MODULE_ENABLE_YMODEM        true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure cpu onchip iic  ******************************************//
#define CFG_IIC_BUF_LEN           32       //配置IIC缓冲区大小
#define CFG_IIC1_ENABLE           false     //
#define CFG_IIC2_ENABLE           false     //
#define CFG_IIC3_ENABLE           false     //
#define CFG_IIC4_ENABLE           false     //
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_IIC  true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure cpu drive inner flash  ******************************************//
#define CFG_EFLASH_PART_FORMAT     false      //分区选项,是否需要擦除该芯片。
#define    CFG_MODULE_ENABLE_CPU_DRIVE_INNER_FLASH  true
//*******************************  Configure cpu onchip dma  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_DMA  true
//*******************************  Configure cpu onchip uart  ******************************************//
#define    CFG_UART1_SENDBUF_LEN           64                //
#define    CFG_UART1_RECVBUF_LEN           64                //
#define    CFG_UART1_DMABUF_LEN            64                //
#define    CFG_UART2_SENDBUF_LEN           64                //
#define    CFG_UART2_RECVBUF_LEN           64                //
#define    CFG_UART2_DMABUF_LEN            64                //
#define    CFG_UART3_SENDBUF_LEN           64                //
#define    CFG_UART3_RECVBUF_LEN           64                //
#define    CFG_UART3_DMABUF_LEN            64                //
#define    CFG_UART4_SENDBUF_LEN           64                //
#define    CFG_UART4_RECVBUF_LEN           64                //
#define    CFG_UART4_DMABUF_LEN            64                //
#define    CFG_UART5_SENDBUF_LEN           64                //
#define    CFG_UART5_RECVBUF_LEN           64                //
#define    CFG_UART5_DMABUF_LEN            64                //
#define    CFG_UART6_SENDBUF_LEN           64                //
#define    CFG_UART6_RECVBUF_LEN           64                //
#define    CFG_UART6_DMABUF_LEN            64                //
#define    CFG_UART7_SENDBUF_LEN           64                //
#define    CFG_UART7_RECVBUF_LEN           64                //
#define    CFG_UART7_DMABUF_LEN            64                //
#define    CFG_UART8_SENDBUF_LEN           64                //
#define    CFG_UART8_RECVBUF_LEN           64                //
#define    CFG_UART8_DMABUF_LEN            64                //
#define    CFG_UART1_ENABLE                false             //
#define    CFG_UART2_ENABLE                false             //
#define    CFG_UART3_ENABLE                true              //
#define    CFG_UART4_ENABLE                false             //
#define    CFG_UART5_ENABLE                false             //
#define    CFG_UART6_ENABLE                false             //
#define    CFG_UART7_ENABLE                false             //
#define    CFG_UART8_ENABLE                false             //
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_UART  true
//*******************************  Configure STM32 commond code  ******************************************//
#define    CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure tpl  ******************************************//
#define     CFG_TPL_PROTONUM            5       //占用一个 tagTplProtoItem 结构
#define    CFG_MODULE_ENABLE_TPL           true
//*******************************  Configure icmp  ******************************************//
#define    CFG_MODULE_ENABLE_ICMP          true
//*******************************  Configure router  ******************************************//
#define CFG_IP_STRMAX           20 //最大路由条目数
#define    CFG_MODULE_ENABLE_ROUTER        true
//*******************************  Configure arp  ******************************************//
#define    CFG_ARP_HASHLEN                 32                //占用一个指针
#define    CFG_MODULE_ENABLE_ARP           true
//*******************************  Configure udp  ******************************************//
#define     CFG_UDP_CBNUM               10      //占用一个 tagUdpCB 结构
#define     CFG_UDP_HASHLEN             4       //占用一个指针
#define     CFG_UDP_PKGMSGLEN        1472       //udp最大包长度
#define     CFG_UDP_BUFLENDEFAULT    0x2000     //8KB
#define    CFG_MODULE_ENABLE_UDP           true
//*******************************  Configure dhcp  ******************************************//
#define    CFG_DHCPD_ENABLE                true             //
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
#define    CFG_MODULE_ENABLE_DHCP          true
//*******************************  Configure tcp  ******************************************//
#define     CFG_TCP_REORDER             true    //资源充足才打开
#define     CFG_TCP_CCBNUM              10      //占一个 指针 和 struct ClienCB
#define     CFG_TCP_SCBNUM              5       //占一个 指针 和 struct ServerCB
#define    CFG_MODULE_ENABLE_TCP           true
//*******************************  Configure ftp  ******************************************//
#define    CFG_FTPD_ENABLE                 false             //暂未实现
#define    CFG_FTPC_ENABLE                 true              //
#define    CFG_MODULE_ENABLE_FTP           true
//*******************************  Configure ppp  ******************************************//
#define    CFG_MODULE_ENABLE_PPP           true
//*******************************  Configure sock  ******************************************//
#define    CFG_SOCKET_NUM                  10                //占一个 tagItem 结构
#define    CFG_MODULE_ENABLE_SOCK          true
//*******************************  Configure telnet  ******************************************//
#define    CFG_TELNETD_ENABLE              true              //
#define    CFG_TELNETC_ENABLE              false             //
#define    CFG_MODULE_ENABLE_TELNET        true
//*******************************  Configure tftp  ******************************************//
#define    CFG_TFTP_PATHDEFAULT            "/"               //TFTP默认工作目录
#define    CFG_MODULE_ENABLE_TFTP          true
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (216*Mhz)         //主频，内核要用，必须定义

#endif
