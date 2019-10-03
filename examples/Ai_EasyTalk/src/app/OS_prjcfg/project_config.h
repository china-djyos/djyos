/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define    CFG_LOCK_LIMIT                  40                       // "��������",�������������������ź����ͻ�����
#define    CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure heap  ******************************************//
#define    CFG_DYNAMIC_MEM                 true                     // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure device file system  ******************************************//
#define    CFG_DEVFILE_LIMIT               10                //�����豸����
#define    CFG_MODULE_ENABLE_DEVICE_FILE_SYSTEM  true
//*******************************  Configure tpl  ******************************************//
#define     CFG_TPL_PROTONUM            5          // "֧�ֵĴ���Э����"��ռ��һ�� tagTplProtoItem �ṹ
#define    CFG_MODULE_ENABLE_TPL           true
//*******************************  Configure tcpip  ******************************************//
#define     CFG_NETPKG_MEMSIZE          0x4000     // "���ݰ��������ߴ�"
#define    CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure icmp  ******************************************//
#define    CFG_MODULE_ENABLE_ICMP          true
//*******************************  Configure router  ******************************************//
#define CFG_IP_STRMAX           20    // ���·����Ŀ��
#define    CFG_MODULE_ENABLE_ROUTER        true
//*******************************  Configure arp  ******************************************//
#define    CFG_ARP_HASHLEN                 32                   // "ARP��ϣ����"��ռ��һ��ָ��
#define    CFG_MODULE_ENABLE_ARP           true
//*******************************  Configure Nls Charset  ******************************************//
#define CFG_LOCAL_CHARSET       "gb2312"           // "�����ַ���",
#define    CFG_MODULE_ENABLE_NLS_CHARSET   true
//*******************************  Configure font  ******************************************//
#define    CFG_FONT_DEFAULT                "gb2312_song_16"     // "Ĭ������",��������include/fontĿ¼����
#define    CFG_MODULE_ENABLE_FONT          true
//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure djybus  ******************************************//
#define    CFG_MODULE_ENABLE_DJYBUS        true
//*******************************  Configure debug information  ******************************************//
#define    CFG_MODULE_ENABLE_DEBUG_INFORMATION  true
//*******************************  Configure udp  ******************************************//
#define     CFG_UDP_CBNUM               10         // "UDP socket����ֵ"��ռ��һ�� tagUdpCB �ṹ
#define     CFG_UDP_HASHLEN             4          // "udp socket ��ϣ����"��ռ��һ��ָ��
#define     CFG_UDP_PKGMSGLEN        1472          // udp��������
#define     CFG_UDP_BUFLENDEFAULT    0x2000        // 8KB
#define    CFG_MODULE_ENABLE_UDP           true
//*******************************  Configure dhcp  ******************************************//
#define    CFG_DHCPD_ENABLE                true                // "DHCP ������ʹ��"
#define    CFG_DHCPC_ENABLE                true                 // "DHCP �ͻ���ʹ��"
#define    CFG_DHCP_RENEWTIME              3600                 // "renew timer",����
#define    CFG_DHCP_REBINDTIME             3600                 // "rebind timer",����
#define    CFG_DHCP_LEASETIME              3600                 // "lease timer",����
#define    CFG_DHCPD_IPNUM                 0x40                 // "IP�سߴ�",64
#define    CFG_DHCPD_IPSTART               "192.168.0.2"        // "DHCP��ʼIP",
#define    CFG_DHCPD_SERVERIP              "192.168.0.253"      // "DHCP SERVER IP"
#define    CFG_DHCPD_ROUTERIP              "192.168.0.253"      // "DHCP ROUTER SERVER IP"
#define    CFG_DHCPD_NETIP                 "255.255.255.0"      // "DHCP MASK IP"
#define    CFG_DHCPD_DNS                   "192.168.0.253"      // "DHCP DNSSERVER IP"
#define    CFG_DHCPD_DOMAINNAME            "domain"             // "DHCP domain name"
#define    CFG_MODULE_ENABLE_DHCP          true
//*******************************  Configure file system  ******************************************//
#define    CFG_CLIB_BUFFERSIZE             512               //
#define    CFG_MODULE_ENABLE_FILE_SYSTEM   true
//*******************************  Configure flash  ******************************************//
#define    CFG_MODULE_ENABLE_FLASH         true
//*******************************  Configure tcp  ******************************************//
#define     CFG_TCP_REORDER             true           // "TCP��������ʹ��",��Դ����Ŵ�
#define     CFG_TCP_CCBNUM              15             // "tcp �ͻ�������ֵ"��ռһ�� ָ�� �� struct ClienCB
#define     CFG_TCP_SCBNUM              5              // "tcp ����������ֵ"��ռһ�� ָ�� �� struct ServerCB
#define    CFG_MODULE_ENABLE_TCP           true
//*******************************  Configure ftp  ******************************************//
#define    CFG_FTPD_ENABLE                 false                // "ftp ������ʹ��",��δʵ��
#define    CFG_FTPC_ENABLE                 true                 // "ftp �ͻ���ʹ��"
#define    CFG_MODULE_ENABLE_FTP           true
//*******************************  Configure gb2312 charset  ******************************************//
#define    CFG_MODULE_ENABLE_GB2312_CHARSET  true
//*******************************  Configure gb2312 dot  ******************************************//
#define    CFG_GB2312_12_SONG              zk_disable           // "12��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"    // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_SONG              from_array                // "16��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_16_SONG_FILENAME "zk_gb2316_16song.bin"    // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_YUAN              zk_disable                // "16����Բ��",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_16_YUAN_FILENAME "zk_gb2316_16yuan.bin"    // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_KAI              zk_disable                 // "16������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_16_KAI_FILENAME  "zk_gb2316_16kai.bin"     // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_HEI              zk_disable                 // "16�����",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_16_HEI_FILENAME  "zk_gb2316_16hei.bin"     // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_FANG              zk_disable                // "16���������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_16_FANG_FILENAME "zk_gb2316_16fang.bin"    // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_SONG              zk_disable                // "24��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_24_SONG_FILENAME "zk_gb2316_24song.bin"    // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_YUAN              zk_disable                // "24����Բ��",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_24_YUAN_FILENAME "zk_gb2316_24yuan.bin"    // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_KAI              zk_disable                 // "24������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_24_KAI_FILENAME  "zk_gb2316_24kai.bin"     // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_HEI              zk_disable                 // "24�������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_24_HEI_FILENAME  "zk_gb2316_24hei.bin"     // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_FANG              zk_disable                // "24���������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_24_FANG_FILENAME "zk_gb2316_24fang.bin"    // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_SONG              zk_disable                // "32��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_32_SONG_FILENAME "zk_gb2316_32song.bin"    // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_YUAN              zk_disable                // "32����Բ��",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_32_YUAN_FILENAME "zk_gb2316_32yuan.bin"    // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_KAI              zk_disable                 // "32������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_32_KAI_FILENAME  "zk_gb2316_32kai.bin"     // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_HEI              zk_disable                 // "32�������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_32_HEI_FILENAME  "zk_gb2316_32hei.bin"     // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_FANG              zk_disable                // "32���������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_32_FANG_FILENAME "zk_gb2316_32fang.bin"    // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define    CFG_MODULE_ENABLE_GB2312_DOT    true
//*******************************  Configure graphical kernel  ******************************************//
#define    CFG_GKERNEL_CMD_DEEP            1024                 // "����������",gdd��gkernel��������Ļ��������ȣ��ֽ�����
#define    CFG_MODULE_ENABLE_GRAPHICAL_KERNEL  true
//*******************************  Configure graphical decorate development  ******************************************//
#define    CFG_DESKTOP_WIDTH               240                     // "������",����ߴ磨����������ȣ�0=��ʾ�����
#define    CFG_DESKTOP_HEIGHT              320                     // "����߶�",����ߴ磨���������߶ȣ�0=��ʾ���߶�
#define    CFG_DISPLAY_NAME                "lcdst7796s"        // "��ʾ����",����bsp����ʾ������ģ�����õ���ʾ��������ͬ
#define    CFG_DESKTOP_NAME                "desktop"        // "������"
#define    CFG_INPUTDEV_NAME               "IoIic"       // "�����豸����",ʹ��bsp�������豸�����õ����֣��������豸�Ļ���ÿ���豸���ö��Ÿ���
#define    CFG_DESKTOP_FORMAT              CN_SYS_PF_RGB565      // "���ظ�ʽ",���洰�����ظ�ʽ��������gkernel.h�ж��壬һ��ʹ������ʾ����ͬ��ɫ
#define    CFG_GRAY_BASE_COLOR             CN_COLOR_WHITE        // "�ҶȻ�ɫ",���ظ�ʽ��Ϊ�Ҷ�ʱ����Ҫ���õġ�������ɫ������gkernel.h���ҵ�������ɫ����
#define    CFG_FILL_COLOR                  CN_COLOR_BLUE         // "���ɫ",��������ʱ�����ɫ����888��ʽ������gkernel.h���ҵ�������ɫ����
#define    CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT  true
//*******************************  Configure message queue  ******************************************//
#define    CFG_MODULE_ENABLE_MESSAGE_QUEUE true
//*******************************  Configure human machine interface  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2          // "name",�˻����������豸����������̡�����
#define    CFG_MODULE_ENABLE_HUMAN_MACHINE_INTERFACE  true
//*******************************  Configure iicbus  ******************************************//
#define    CFG_MODULE_ENABLE_IICBUS        true
//*******************************  Configure int  ******************************************//
#define    CFG_MODULE_ENABLE_INT           true
//*******************************  Configure io analog iic bus  ******************************************//
#define    CFG_MODULE_ENABLE_IO_ANALOG_IIC_BUS  true
//*******************************  Configure ioiicconfig  ******************************************//
#define    IO_IIC_BUS_NAME                 "IoIic"             // 
#define    CFG_MODULE_ENABLE_IOIICCONFIG   true
//*******************************  Configure multiplex  ******************************************//
#define    CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure kernel  ******************************************//
#define    CFG_INIT_STACK_SIZE             4096                      // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define    CFG_EVENT_LIMIT                 30                        // "�¼�������ֵ",�¼�����
#define    CFG_EVENT_TYPE_LIMIT            30                        // "�¼���������ֵ",�¼�������
#define    CFG_MAINSTACK_LIMIT             8192                      // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define    CFG_IDLESTACK_LIMIT             4096                      // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define    CFG_OS_TINY                     false                     // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure kernel object system  ******************************************//
#define    CFG_OBJECT_LIMIT                8                                  // "��������ʼֵ"��������Զ�����
#define    CFG_HANDLE_LIMIT                8                                  // "�������ʼֵ"��������Զ�����
#define    CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure loader  ******************************************//
#define    CFG_UPDATEIBOOT_EN              false                // "�Ƿ�֧�����߸���Iboot"��
#define    CFG_APP_RUNMODE                 EN_DIRECT_RUN        // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define    CFG_APP_VERIFICATION            VERIFICATION_NULL    // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define    CFG_IBOOT_VERSION_SMALL         00                   // Iboot�����汾��,xx.xx.__
#define    CFG_IBOOT_VERSION_MEDIUM        00                   // Iboot�����汾��,xx.__.xx
#define    CFG_IBOOT_VERSION_LARGE         01                   // Iboot�����汾��,__.xx.xx
#define    CFG_IBOOT_UPDATE_NAME           "/yaf2/iboot.bin"    // "������ibootĬ�ϴ洢·��"
#define    CFG_APP_UPDATE_NAME             "/yaf2/app.bin"      // "������appĬ�ϴ洢·��"
#define    CFG_MODULE_ENABLE_LOADER        true
//*******************************  Configure lowpower  ******************************************//
#define    CFG_MODULE_ENABLE_LOWPOWER      true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       20       // "�ڴ��������ֵ",
#define    CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure ppp  ******************************************//
#define    CFG_MODULE_ENABLE_PPP           true
//*******************************  Configure stdio  ******************************************//
#define    CFG_STDIO_STDIN_MULTI           true                     // "�Ƿ�֧�ֶ��������豸",
#define    CFG_STDIO_STDOUT_FOLLOW         true                     // "stdout�Ƿ����stdin",
#define    CFG_STDIO_STDERR_FOLLOW         true                     // "stderr�Ƿ����stdin",
#define    CFG_STDIO_FLOAT_PRINT           true                     // "֧�ָ����ӡ"
#define    CFG_STDIO_STDIOFILE             true                     // "֧�ֱ�׼IO�ļ�"
#define    CFG_STDIO_IN_NAME               ("/dev/UART2")           // "��׼�����豸��",
#define    CFG_STDIO_OUT_NAME              ("/dev/UART2")           // "��׼����豸��",
#define    CFG_STDIO_ERR_NAME              ("/dev/UART2")           // "��׼err����豸��",
#define    CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure shell  ******************************************//
#define    CFG_SHELL_STACK                 0x1000               // "ִ��shell�����ջ�ߴ�"
#define    CFG_ADD_ROUTINE_SHELL           true                 // "�Ƿ���ӳ���shell����"
#define    CFG_ADD_EXPAND_SHELL            true                 // "�Ƿ������չshell����"
#define    CFG_ADD_GLOBAL_FUN              false                // "���ȫ�ֺ�����shell"
#define    CFG_SHOW_ADD_SHEELL             true                 // "��ʾ�ڱ��봰����ӵ�shell����"
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure sock  ******************************************//
#define    CFG_SOCKET_NUM                  10                   // "socket����ֵ"��ռһ�� tagItem �ṹ
#define    CFG_MODULE_ENABLE_SOCK          true
//*******************************  Configure telnet  ******************************************//
#define    CFG_TELNETD_ENABLE              true                 // "telnet ������ʹ��"
#define    CFG_TELNETC_ENABLE              false                // "telnet �ͻ���ʹ��"
#define    CFG_MODULE_ENABLE_TELNET        true
//*******************************  Configure tftp  ******************************************//
#define    CFG_TFTP_PATHDEFAULT            "/"                  // TFTPĬ�Ϲ���Ŀ¼
#define    CFG_MODULE_ENABLE_TFTP          true
//*******************************  Configure touch  ******************************************//
#define    CFG_MODULE_ENABLE_TOUCH         true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure fat file system  ******************************************//
#define    CFG_FAT_MS_INSTALLUSE           MS_INSTALLUSE     //  ʹ��ʱ�ŷ�����װ����
#define    CFG_FAT_MEDIA_KIND              "MSC"             //ý��������� "MSC" "EMMC"��
#define    CFG_FAT_MOUNT_POINT             "fat"             //FAT�ļ�ϵͳ��װĿ¼
#define    CFG_MODULE_ENABLE_FAT_FILE_SYSTEM  true
//*******************************  Configure cpu onchip MAC  ******************************************//
#define CFG_MAC_ADDR0           00              // "������ַ0",
#define CFG_MAC_ADDR1           00              // "������ַ1",
#define CFG_MAC_ADDR2           00              // "������ַ2",
#define CFG_MAC_ADDR3           00              // "������ַ3",
#define CFG_MAC_ADDR4           00              // "������ַ4",
#define CFG_MAC_ADDR5           00              // "������ַ5",
#define CFG_ETH_LOOP_CYCLE      1000            // "������ѯ��������",
#define CFG_ETH_LOOP_ENABLE     true           // "���������Ƿ���ѯ",
#define CFG_ETH_DEV_NAME        "STM32F7_ETH"   // "��������",
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_MAC  true
//*******************************  Configure network config  ******************************************//
#define    CFG_STATIC_IP                   false                //  "IP����",true=ʹ�þ�̬IP��false=��̬IP
#define    CFG_NETCARD_NAME                "BK7251_WIFI"        //  "������",
#define    CFG_MY_IPV4                     "192.168.0.179"      //  "��̬IP",
#define    CFG_MY_SUBMASK                  "255.255.255.0"      //  "��������",
#define    CFG_MY_GATWAY                   "192.168.0.1"        //  "����",
#define    CFG_MY_DNS                      "192.168.0.1"        //  "DNS",
#define    CFG_MODULE_ENABLE_NETWORK_CONFIG  true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure cpu onchip adc  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_ADC  true
//*******************************  Configure cpu onchip audio in_out  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_AUDIO_IN_OUT  true
//*******************************  Configure cpu onchip pwm  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_PWM  true
//*******************************  Configure cpu onchip uart  ******************************************//
#define    CFG_UART1_SENDBUF_LEN           64                   // "UART1���ͻ��λ�������С",
#define    CFG_UART1_RECVBUF_LEN           64                   // "UART1���ջ��λ�������С",
#define    CFG_UART1_DMABUF_LEN            64                   // "UART1 DMA���λ�������С",
#define    CFG_UART2_SENDBUF_LEN           64                   // "UART2���ͻ��λ�������С",
#define    CFG_UART2_RECVBUF_LEN           64                   // "UART2���ջ��λ�������С",
#define    CFG_UART2_DMABUF_LEN            64                   // "UART2 DMA���λ�������С",
#define    CFG_UART3_SENDBUF_LEN           64                   // "UART3���ͻ��λ�������С",
#define    CFG_UART3_RECVBUF_LEN           64                   // "UART3���ջ��λ�������С",
#define    CFG_UART3_DMABUF_LEN            64                   // "UART3 DMA���λ�������С",
#define    CFG_UART4_SENDBUF_LEN           64                   // "UART4���ͻ��λ�������С",
#define    CFG_UART4_RECVBUF_LEN           64                   // "UART4���ջ��λ�������С",
#define    CFG_UART4_DMABUF_LEN            64                   // "UART4 DMA���λ�������С",
#define    CFG_UART1_ENABLE                false                // "�Ƿ�ʹ��UART1",
#define    CFG_UART2_ENABLE                true                 // "�Ƿ�ʹ��UART2",
#define    CFG_UART3_ENABLE                false                // "�Ƿ�ʹ��UART3",
#define    CFG_UART4_ENABLE                false                // "�Ƿ�ʹ��UART4",
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_UART  true
//*******************************  Configure LCD driver st7796s  ******************************************//
#define    CFG_LCD_XSIZE                   240      // "LCD���",
#define    CFG_LCD_YSIZE                   320      // "LCD�߶�",
#define    CFG_ST7796S_DISPLAY_NAME        "lcdst7796s"              // "��ʾ������",����Һ����ʾ������
#define    CFG_ST7796S_HEAP_NAME           "PSRAM"                  // "����ʹ�ö���",����Һ��������ʹ�õĶ�����
//*******************************  Configure qspi  ******************************************//
#define    CFG_QSPI_RAM_SIZE               8388608               // RAM�ĳߴ磬Ĭ��8M��
#define    CFG_QSPI_RAM_PAGE_SIZE          1024                  // RAM��ҳ�ߴ磬Ĭ��1K��
#define    CFG_QSPI_RAM_ALIGNMENT          0                     // Ҫ��Ķ���ߴ磬0��ʾ��ϵͳ�Ķ���ߴ�
#define    CFG_QSPI_RAM_DEDICATED          false                 // true��ʾר��heap��false��ʾͨ��heap��
#define    CFG_QSPI_RAM_HEAP_NAME          "PSRAM"               // �½���heap���֡�
//*******************************  Configure touchscreen FT6236  ******************************************//
#define    CT_MAX_TOUCH                    5                    // "������",֧�����5�㴥��
#define    CFG_TOUCH_ADJUST_FILE           "/yaf2/touch_init.dat"     // ���津���������������ļ�
#define    CFG_FT6236_BUS_NAME             "IoIic"              // "IIC��������",����оƬʹ�õ�IIC��������
#define    CFG_FT6236_TOUCH_NAME           "FT6236"             // "����������",���ô���������
#define    CFG_DESKTOP_NAME                "desktop"            // "��������",���ô�����������ʾ�����������
#define    CFG_MODULE_ENABLE_TOUCHSCREEN_FT6236  true
//*******************************  Configure cpu onchip flash  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_FLASH  true
//******************************* Core Clock ******************************************//
#define    CFG_CORE_MCLK                   (180.0*Mhz)       //��Ƶ���ں�Ҫ�ã����붨��

#endif
