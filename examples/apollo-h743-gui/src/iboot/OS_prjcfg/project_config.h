/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure Nls Charset  ******************************************//
#define     CFG_LOCAL_CHARSET                "gb2312"                          // "�����ַ���",
//*******************************  Configure lock  ******************************************//
#define     CFG_LOCK_LIMIT                   40                                // "��������",�������������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true                  // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure device file system  ******************************************//
#define    CFG_DEVFILE_LIMIT               10                         // "�豸����",�����豸����
//*******************************  Configure file system  ******************************************//
#define     CFG_CLIB_BUFFERSIZE              512                               // "C���ļ��û�̬�������ߴ�"
//*******************************  Configure easyfile system  ******************************************//
#define     CFG_EFS_FILE_BLOCK_SIZE          0x1000                            //  �����ļ���С������
#define CFG_EFS_INSTALL_OPTION            MS_INSTALLFORMAT               // EFS�ļ�ϵͳ��װѡ�16777216:�ļ�ϵͳ������ʱ���½���256����ʽ���ļ�ϵͳ
#define     CFG_EFS_MOUNT_POINT              "efs"                             // "name",EFS�ļ�ϵͳ��װĿ¼
//*******************************  Configure font  ******************************************//
#define     CFG_FONT_DEFAULT                 CN_FONT_GB2312_SONG_16                // "Ĭ������",��������include/fontĿ¼����
//*******************************  Configure gb2312 dot  ******************************************//
#define     CFG_GB2312_12_SONG               zk_disable                        // "12��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"                 // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_SONG              from_array                             // "16��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_16_SONG_FILENAME "zk_gb2316_16song.bin"                 // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_YUAN              zk_disable                             // "16����Բ��",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_16_YUAN_FILENAME "zk_gb2316_16yuan.bin"                 // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_KAI              zk_disable                              // "16������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_16_KAI_FILENAME  "zk_gb2316_16kai.bin"                  // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_HEI              zk_disable                              // "16�����",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_16_HEI_FILENAME  "zk_gb2316_16hei.bin"                  // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_16_FANG              zk_disable                             // "16���������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_16_FANG_FILENAME "zk_gb2316_16fang.bin"                 // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_SONG              zk_disable                             // "24��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_24_SONG_FILENAME "zk_gb2316_24song.bin"                 // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_YUAN              zk_disable                             // "24����Բ��",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_24_YUAN_FILENAME "zk_gb2316_24yuan.bin"                 // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_KAI              zk_disable                              // "24������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_24_KAI_FILENAME  "zk_gb2316_24kai.bin"                  // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_HEI              zk_disable                              // "24�������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_24_HEI_FILENAME  "zk_gb2316_24hei.bin"                  // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_24_FANG              zk_disable                             // "24���������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_24_FANG_FILENAME "zk_gb2316_24fang.bin"                 // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_SONG              zk_disable                             // "32��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_32_SONG_FILENAME "zk_gb2316_32song.bin"                 // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_YUAN              zk_disable                             // "32����Բ��",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_32_YUAN_FILENAME "zk_gb2316_32yuan.bin"                 // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_KAI              zk_disable                              // "32������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_32_KAI_FILENAME  "zk_gb2316_32kai.bin"                  // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_HEI              zk_disable                              // "32�������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_32_HEI_FILENAME  "zk_gb2316_32hei.bin"                  // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
#define CFG_GB2312_32_FANG              zk_disable                             // "32���������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
    #define CFG_GB2312_32_FANG_FILENAME "zk_gb2316_32fang.bin"                 // "�ֿ��ļ���",�����ļ���ȡ���������ļ���
//*******************************  Configure graphical kernel  ******************************************//
#define     CFG_GKERNEL_CMD_DEEP             1024                              // "����������",gdd��gkernel��������Ļ��������ȣ��ֽ�����
//*******************************  Configure graphical decorate development(gdd)  ******************************************//
#define     CFG_DESKTOP_WIDTH                0                                // "������",����ߴ磨����������ȣ�0=��ʾ�����
#define     CFG_DESKTOP_HEIGHT               0                                // "����߶�",����ߴ磨���������߶ȣ�0=��ʾ���߶�
#define     CFG_DISPLAY_NAME                 "LCD_F7"                         // "��ʾ����",����bsp����ʾ������ģ�����õ���ʾ��������ͬ
#define     CFG_DESKTOP_NAME                 "Mydesktop"                      // "������"
#define     CFG_INPUTDEV_NAME                "INPUTDEV_NAME"                  // "�����豸����",ʹ��bsp�������豸�����õ����֣��������豸�Ļ���ÿ���豸���ö��Ÿ���
#define     CFG_DESKTOP_FORMAT               CN_SYS_PF_RGB565                 // "���ظ�ʽ",���洰�����ظ�ʽ��������gkernel.h�ж��壬һ��ʹ������ʾ����ͬ��ɫ
#define     CFG_GRAY_BASE_COLOR              CN_COLOR_WHITE                   // "�ҶȻ�ɫ",���ظ�ʽ��Ϊ�Ҷ�ʱ����Ҫ���õġ�������ɫ������gkernel.h���ҵ�������ɫ����
#define     CFG_FILL_COLOR                   CN_COLOR_BLUE                    // "���ɫ",��������ʱ�����ɫ����888��ʽ������gkernel.h���ҵ�������ɫ����
//*******************************  Configure human machine interface  ******************************************//
#define     CFG_HMIIN_DEV_LIMIT              2                                // "name",�˻����������豸����������̡�����
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true             // "�Ƿ�֧�ֶ��������豸",
#define CFG_STDIO_STDOUT_FOLLOW    true             // "stdout�Ƿ����stdin",
#define CFG_STDIO_STDERR_FOLLOW    true             // "stderr�Ƿ����stdin",
#define CFG_STDIO_FLOAT_PRINT       true                 // "֧�ָ����ӡ"
#define CFG_STDIO_STDIOFILE         true                 // "֧�ֱ�׼IO�ļ�"
#define CFG_STDIO_IN_NAME              "/dev/UART1"             // "��׼�����豸��",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"             // "��׼����豸��",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"             // "��׼err����豸��",
//*******************************  Configure shell  ******************************************//
#define    CFG_SHELL_STACK                 0x1000                    // "ִ��shell�����ջ�ߴ�"
#define    CFG_ADD_ROUTINE_SHELL           true                      // "�Ƿ���ӳ���shell����"
#define    CFG_ADD_EXPAND_SHELL            true                      // "�Ƿ������չshell����"
#define    CFG_ADD_GLOBAL_FUN              false                     // "���ȫ�ֺ�����shell"
#define    CFG_SHOW_ADD_SHEELL             true                      // "��ʾ�ڱ��봰����ӵ�shell����"
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure tpl  ******************************************//
#define    CFG_TPL_PROTONUM                5                         // "֧�ֵĴ���Э����"��ռ��һ�� tagTplProtoItem �ṹ
//*******************************  Configure tcpip  ******************************************//
#define    CFG_NETPKG_MEMSIZE              0x4000                    // "���ݰ��������ߴ�"
#define  CFG_MODULE_ENABLE_TCPIP         true
#define  CFG_MODULE_ENABLE_TCPIP         true
//*******************************  Configure icmp  ******************************************//
#define  CFG_MODULE_ENABLE_ICMP          true
//*******************************  Configure router  ******************************************//
#define    CFG_IP_STRMAX                   20                        // ���·����Ŀ��
#define  CFG_MODULE_ENABLE_ROUTER        true
//*******************************  Configure dhcp  ******************************************//
#define    CFG_DHCPD_ENABLE                false              // "DHCP ������ʹ��"
#define    CFG_DHCPC_ENABLE                true               // "DHCP �ͻ���ʹ��"
#define    CFG_DHCP_RENEWTIME              3600               // "renew timer",����
#define    CFG_DHCP_REBINDTIME             3600               // "rebind timer",����
#define    CFG_DHCP_LEASETIME              3600               // "lease timer",����
#define    CFG_DHCPD_IPNUM                 0x40               // "IP�سߴ�",64
#define    CFG_DHCPD_IPSTART               "192.168.0.2"      // "DHCP��ʼIP",
#define    CFG_DHCPD_SERVERIP              "192.168.0.253"    // "DHCP SERVER IP"
#define    CFG_DHCPD_ROUTERIP              "192.168.0.253"    // "DHCP ROUTER SERVER IP"
#define    CFG_DHCPD_NETIP                 "255.255.255.0"    // "DHCP MASK IP"
#define    CFG_DHCPD_DNS                   "192.168.0.253"    // "DHCP DNSSERVER IP"
#define    CFG_DHCPD_DOMAINNAME            "domain"           // "DHCP domain name"
#define  CFG_MODULE_ENABLE_DHCP          true
//*******************************  Configure tcp  ******************************************//
#define     CFG_TCP_REORDER                  true                              // "TCP��������ʹ��",��Դ����Ŵ�
#define     CFG_TCP_CCBNUM                   10                                // "tcp �ͻ�������ֵ"��ռһ�� ָ�� �� struct ClienCB
#define     CFG_TCP_SCBNUM                   5                                 // "tcp ����������ֵ"��ռһ�� ָ�� �� struct ServerCB
#define  CFG_MODULE_ENABLE_TCP           true
//*******************************  Configure ftp  ******************************************//
#define    CFG_FTPD_ENABLE                 false                     // "tftp ������ʹ��",��δʵ��
#define    CFG_FTPC_ENABLE                 true                      // "tftp �ͻ���ʹ��"
#define  CFG_MODULE_ENABLE_FTP           true
//*******************************  Configure telnet  ******************************************//
#define    CFG_TELNETD_ENABLE              true                      // "telnet ������ʹ��"
#define    CFG_TELNETC_ENABLE              false                     // "telnet �ͻ���ʹ��"
#define  CFG_MODULE_ENABLE_TELNET        true
//*******************************  Configure tftp  ******************************************//
#define    CFG_FTPD_ENABLE                 true               // "ftp ������ʹ��"
#define    CFG_FTPC_ENABLE                 false              // "ftp �ͻ���ʹ��"
#define    CFG_TFTP_PATHDEFAULT            "/"                // TFTPĬ�Ϲ���Ŀ¼
#define  CFG_MODULE_ENABLE_TFTP          true
//*******************************  Configure arp  ******************************************//
#define    CFG_ARP_HASHLEN                 32                        // "ARP��ϣ����"��ռ��һ��ָ��
#define  CFG_MODULE_ENABLE_ARP           true
//*******************************  Configure ppp  ******************************************//
#define  CFG_MODULE_ENABLE_PPP           true
//*******************************  Configure sock  ******************************************//
#define CFG_TCP_REORDER  false
#define     CFG_SOCKET_NUM              10      //ռһ�� tagItem �ṹ
//*******************************  Configure udp  ******************************************//
#define    CFG_UDP_CBNUM                   10                        // "UDP socket����ֵ"��ռ��һ�� tagUdpCB �ṹ
#define    CFG_UDP_HASHLEN                 4                         // "udp socket ��ϣ����"��ռ��һ��ָ��
#define    CFG_UDP_PKGMSGLEN               1472                      // udp��������
#define    CFG_UDP_BUFLENDEFAULT           0x2000                    // 8KB
#define  CFG_MODULE_ENABLE_UDP           true
//*******************************  Configure network config  ******************************************//
#define    CFG_STATIC_IP                   true               //  "ʹ�þ�̬IP?",
#define    CFG_SELECT_NETCARD              "STM32H7_ETH"      //  "��������",������ѡ�е��������������õ�������ͬ
#define    CFG_MY_IPV4                     "192.168.0.179"    //  "��̬IP",
#define    CFG_MY_SUBMASK                  "255.255.255.0"    //  "��������",
#define    CFG_MY_GATWAY                   "192.168.0.1"      //  "����",
#define    CFG_MY_DNS                      "192.168.0.1"      //  "DNS",
//*******************************  Configure ethernet phy lan8720  ******************************************//
#define    CFG_LAN8720_PHY_ADDRESS         0                  //  phy�������ַ��Ӳ��һ�����Ϊ 0
//*******************************  Configure cpu onchip MAC  ******************************************//
#define    CFG_ETH_NETCARD_NAME            "STM32H7_ETH"      //  "��������",
#define    CFG_ETH_LOOP_CYCLE              1000               //  "������ѯ����(uS)",�ж�ģʽ������д
#define    CFG_ETH_LOOP_ENABLE             false              //  "���������Ƿ���ѯ",
#define    CFG_ETH_HARD_MAC_ADDR           true               //  "Ӳ������Mac��ַ",
#define    CFG_ETH_MAC_ADDR0               00                 //  "MAC ADDR0",��ѡ��"Ӳ������Mac��ַ",��������д
#define    CFG_ETH_MAC_ADDR1               02                 //  "MAC ADDR1",��ѡ��"Ӳ������Mac��ַ",��������д
#define    CFG_ETH_MAC_ADDR2               03                 //  "MAC ADDR2",��ѡ��"Ӳ������Mac��ַ",��������д
#define    CFG_ETH_MAC_ADDR3               04                 //  "MAC ADDR3",��ѡ��"Ӳ������Mac��ַ",��������д
#define    CFG_ETH_MAC_ADDR4               05                 //  "MAC ADDR4",��ѡ��"Ӳ������Mac��ַ",��������д
#define    CFG_ETH_MAC_ADDR5               06                 //  "MAC ADDR5",��ѡ��"Ӳ������Mac��ַ",��������д
//*******************************  Configure Software Timers  ******************************************//
#define CFG_TIMERS_LIMIT        5                // "��ʱ������",�ɴ����Ķ�ʱ��������������ͼ�ν���Ķ�ʱ����
#define CFG_TIMER_SOUCE_HARD    true             // "Ӳ����ʱ���ṩʱ��Դ",ѡ��ר��Ӳ������tick/tickless��ʱ��Դ
//*******************************  Configure kernel  ******************************************//
#define     CFG_INIT_STACK_SIZE              4096                          // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define     CFG_EVENT_LIMIT                  20                            // "�¼�������ֵ",�¼�����
#define     CFG_EVENT_TYPE_LIMIT             20                            // "�¼���������ֵ",�¼�������
#define     CFG_MAINSTACK_LIMIT              4096                          // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define     CFG_IDLESTACK_LIMIT              1024                          // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define     CFG_OS_TINY                      false                         // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8            // "��������ʼֵ"��������Զ�����
#define CFG_HANDLE_LIMIT        8            // "�������ʼֵ"��������Զ�����
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10                      // "�ڴ��������ֵ",
//*******************************  Configure time  ******************************************//
#define     CFG_LOCAL_TIMEZONE               8                                 // "ʱ��",����ʱ���Ƕ�8��
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN                     // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL                      // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_IBOOT_VERSION       01                        // Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"                           // "Iboot����"
//*******************************  Configure ioiicconfig  ******************************************//
#define    IO_IIC_BUS_NAME                 "IoIic"             // 
//*******************************  Configure touchscreen FT5X26  ******************************************//
#define    CT_MAX_TOUCH                    5                         // "������",֧�����5�㴥��
#define    CFG_TOUCH_ADJUST_FILE           "/yaf2/touch_init.dat"        // ���津���������������ļ�
#define    CFG_FT5X26_BUS_NAME             "Ioiic2"                   // "IIC��������",����оƬʹ�õ�IIC��������
#define    CFG_FT5X26_TOUCH_NAME           "FT5X26"                  // "����������",���ô���������
#define    CFG_DESKTOP_NAME                "Mydesktop"               // "��������",���ô�����������ʾ�����������
//*******************************  Configure IIC to 8gpio pcf8574  ******************************************//
#define    CFG_PCF8574_BUS_NAME            "IoIic"                   // "IIC������"��
//*******************************  Configure cpu onchip lcd  ******************************************//
#define    CFG_LCD_XSIZE                   1024                      // "LCDˮƽ���ؿ��",
#define    CFG_LCD_YSIZE                   600                       // "LCD��ֱ���ؿ��",
#define    CFG_DISPLAY_NAME                "LCD_F7"                  // "��ʾ��LCD����",
#define    CFG_LCD_HEAP_NAME               "extram"                  // "LCDʹ�ö�����",
//*******************************  Configure cpu peri random  ******************************************//
#define    CFG_RNG_NUM_MAX                 65535                     // "��������ֵ",
#define    CFG_RNG_NUM_MIN                 1024                      // "�������Сֵ",
//*******************************  Configure cpu_peri_cpu peri uart  ******************************************//
#define    CFG_UART_SENDBUF_LEN            32                        // "���ڷ��ͻ��λ�������С",
#define    CFG_UART_RECVBUF_LEN            32                        // "���ڽ��ջ��λ�������С",
#define    CFG_UART1_ENABLE                true                      // "�����Ƿ�ʹ��UART1",
#define    CFG_UART2_ENABLE                false                     // "�����Ƿ�ʹ��UART2",
#define    CFG_UART3_ENABLE                false                     // "�����Ƿ�ʹ��UART3",
#define    CFG_UART4_ENABLE                false                     // "�����Ƿ�ʹ��UART4",
#define    CFG_UART5_ENABLE                false                     // "�����Ƿ�ʹ��UART5",
#define    CFG_UART6_ENABLE                false                     // "�����Ƿ�ʹ��UART6",
#define    CFG_UART7_ENABLE                false                     // "�����Ƿ�ʹ��UART7",
#define    CFG_UART8_ENABLE                false                     // "�����Ƿ�ʹ��UART8",
//*******************************  Configure third lib yaf2 file system  ******************************************//
#define    CFG_YAF_ECC                     0                 //YAF�ļ�ϵͳ�ļ�ʹ���豸ECC���ܡ�0��ʹ��ecc��YAF2_ENABLE_DEVICE_ECCʹ��ecc
#define    CFG_YAF_INSTALL_OPTION          MS_INSTALLCREAT   //YAF�ļ�ϵͳ��װѡ���װʱ�Ƿ��ʽ�������ļ�ϵͳ��
#define    CFG_YAF_MOUNT_POINT             "yaf2"            //YAF�ļ�ϵͳ��װĿ¼
//*******************************  Configure cpu peri nand  ******************************************//
#define    CFG_NFLASH_PART_FORMAT          false             //�Ƿ���Ҫ������оƬ��
//*******************************  Configure nand_install_yaf  ******************************************//
#define    CFG_NFLASH_YAFFSMOUNT_NAME      "yaf2"            //�谲װ���ļ�ϵͳ��mount������
#define    CFG_NFLASH_YAF_PART_START       0                 //������ʼ����д��ţ���Ŵ�0��ʼ����
#define    CFG_NFLASH_YAF_PART_END         -1                //����������-1��ʾ���һ��
#define    CFG_NFLASH_YAF_PART_FORMAT      false             //����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
