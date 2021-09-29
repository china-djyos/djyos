/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //定义设备数量
//*******************************  Configure System: file system  ******************************************//
#define CFG_CLIB_BUFFERSIZE            512      //
//*******************************  Configure shell  ******************************************//
#define    CFG_SHELL_STACK                 0x1400             // "执行shell命令的栈尺寸"
#define    CFG_ADD_ROUTINE_SHELL           true               // "是否添加常规shell命令"
#define    CFG_ADD_EXPAND_SHELL            true               // "是否添加拓展shell命令"
#define    CFG_ADD_GLOBAL_FUN              false              // "添加全局函数到shell"
#define    CFG_SHOW_ADD_SHEELL             true               // "显示在编译窗口添加的shell命令"
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure System: stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true    //
#define CFG_STDIO_STDOUT_FOLLOW    true    //
#define CFG_STDIO_STDERR_FOLLOW    true    //
#define CFG_STDIO_FLOAT_PRINT       true        //
#define CFG_STDIO_STDIOFILE         true        //
#define CFG_STDIO_IN_NAME              "/dev/UART2"    //
#define CFG_STDIO_OUT_NAME             "/dev/UART2"    //
#define CFG_STDIO_ERR_NAME             "/dev/UART2"    //
//*******************************  Configure ftp  ******************************************//
//#define    CFG_FTPD_ENABLE                 false              // "tftp 服务器使能",暂未实现
//#define    CFG_FTPC_ENABLE                 false               // "tftp 客户端使能"
//#define  CFG_MODULE_ENABLE_FTP             false
//*******************************  Configure telnet  ******************************************//
//#define    CFG_TELNETD_ENABLE              false               // "telnet 服务器使能"
//#define    CFG_TELNETC_ENABLE              false              // "telnet 客户端使能"
//#define  CFG_MODULE_ENABLE_TELNET          false
//*******************************  Configure tftp  ******************************************//
//#define    CFG_FTPD_ENABLE                 false               // "ftp 服务器使能"
//#define    CFG_FTPC_ENABLE                 false              // "ftp 客户端使能"
//#define    CFG_TFTP_PATHDEFAULT            "/"                // TFTP默认工作目录
//#define  CFG_MODULE_ENABLE_TFTP          false
//*******************************  Configure icmp  ******************************************//
#define     CFG_MODULE_ENABLE_ICMP          true
//*******************************  Configure router  ******************************************//
#define     CFG_IP_STRMAX                   20                  // 最大路由条目数
#define     CFG_MODULE_ENABLE_ROUTER        true
//*******************************  Configure sock  ******************************************//
#define     CFG_SOCKET_NUM              10      //占一个 tagItem 结构
//*******************************  Configure System: tcp  ******************************************//
#define     CFG_TCP_REORDER             false    //资源充足才打开
#define     CFG_TCP_CCBNUM              10      //占一个 指针 和 struct ClienCB
#define     CFG_TCP_SCBNUM              5       //占一个 指针 和 struct ServerCB
#define  CFG_MODULE_ENABLE_TCP           true
//*******************************  Configure tpl  ******************************************//
#define    CFG_TPL_PROTONUM                5                  // "支持的传输协议数"，占用一个 tagTplProtoItem 结构
//*******************************  Configure System: tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE          0x4000  //
#define  CFG_MODULE_ENABLE_TCPIP         true
#define  CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure System: kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //定义初始化过程使用的栈空间一般按默认值就可以了
#define CFG_EVENT_LIMIT         20      //事件数量
#define CFG_EVENT_TYPE_LIMIT    20      //事件类型数
#define CFG_MAINSTACK_LIMIT     4096    //main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024    //IDLE事件处理函数运行的栈尺寸一般按默认值就可以了
#define CFG_OS_TINY             false   //true=用于资源紧缺的场合内核会裁剪掉：事件类型名字事件处理时间统计。
//*******************************  Configure System: heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //即使选false也允许使用malloc-free分配内存但使用有差别详见 《……用户手册》内存分配章节
//*******************************  Configure System: lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //定义锁的数量包含信号量和互斥量
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //用完会自动扩充
#define CFG_HANDLE_LIMIT        8   //用完会自动扩充
//*******************************  Configure System: memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
//*******************************  Configure System: time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //北京时间是东8区
//*******************************  Configure System: loader  ******************************************//
#define  CFG_APP_RUNMODE  CN_DIRECT_RUN     //CN_DIRECT_RUN=直接从flash中运行；CN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL      //是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_IBOOT_VERSION       01        //Iboot发布版本号
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"           //
//*******************************  Configure netconfig  ******************************************//
#define    CFG_STATIC_IP                   true               // "静态IP",true=使用静态IP，false=动态IP
#define    CFG_NETCARD_NAME                "SCM610_ETH"       // "网卡名",
#define    CFG_MY_IPV4                     "192.168.0.179"    // "静态IP",
#define    CFG_MY_SUBMASK                  "255.255.255.0"    // "子网掩码",
#define    CFG_MY_GATWAY                   "192.168.0.1"      // "网关",
#define    CFG_MY_DNS                      "192.168.0.1"      // "DNS",
//*******************************  Configure emflash insatall xip  ******************************************//
#define CFG_EFLASH_XIP_PART_START      6          //分区起始，填写块号，块号从0开始计算
#define CFG_EFLASH_XIP_PART_END        -1         //分区结束，-1表示最后一块
#define CFG_EFLASH_XIP_PART_FORMAT     false      //分区选项,是否需要格式化该分区。
//*******************************  Configure cpu_peri_emflash  ******************************************//
#define    CFG_EFLASH_PART_FORMAT          false              // 分区选项,是否需要擦除该芯片。
//*******************************  Configure cpu onchip MAC  ******************************************//
#define CFG_GMAC_LOOP_MODE         0            //
#define CFG_GMAC_LOOP_CYCLE        1000         //
#define CFG_GMAC_MAC_ADDR0         0x11         //
#define CFG_GMAC_MAC_ADDR1         0x22         //
#define CFG_GMAC_MAC_ADDR2         0x33         //
#define CFG_GMAC_MAC_ADDR3         0x44         //
#define CFG_GMAC_MAC_ADDR4         0x55         //
#define CFG_GMAC_MAC_ADDR5         0x66         //
//*******************************  Configure cpu onchip iic  ******************************************//
#define CFG_I2C_BUF_LEN         128         //
#define CFG_I2C1_ENABLE         true       //
#define CFG_I2C2_ENABLE         false       //
#define CFG_I2C3_ENABLE         false       //
//*******************************  Configure cpu peri uart  ******************************************//
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
#define    CFG_UART2_ENABLE                true              //
#define    CFG_UART3_ENABLE                false             //
#define    CFG_UART4_ENABLE                true              //
#define    CFG_UART5_ENABLE                false             //
#define    CFG_UART6_ENABLE                false             //
#define    CFG_UART7_ENABLE                false             //
#define    CFG_UART8_ENABLE                false             //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (150*Mhz)         //主频，内核要用，必须定义

#endif
