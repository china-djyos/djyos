/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include <stdint.h>
#include <stddef.h>
//manual config start
//�˴���д�ֶ����ã�DIDE���������ļ�ʱ�����ᱻ�޸�
//manual config end

#define    CN_RUNMODE_IBOOT                0                //IBOOTģʽ����
#define    CN_RUNMODE_APP                2                //��IBOOT���ص�APP
#define    CFG_RUNMODE                CN_RUNMODE_IBOOT                //��IBOOT���ص�APP
#define    CN_RUNMODE_BOOTSELF                1                //����IBOOT��������ģʽAPP
//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10       // "�豸����",�����豸����
#define    CFG_MODULE_ENABLE_DEVICE_FILE_SYSTEM  true
//*******************************  Configure int  ******************************************//
#define    CFG_MODULE_ENABLE_INT           true
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8    // "��������ʼֵ"��������Զ�����
#define CFG_HANDLE_LIMIT        8    // "�������ʼֵ"��������Զ�����
#define    CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40       // "��������",�������������������ź����ͻ�����
#define    CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10       // "�ڴ��������ֵ",
#define    CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure message queue  ******************************************//
#define    CFG_MODULE_ENABLE_MESSAGE_QUEUE true
//*******************************  Configure multiplex  ******************************************//
#define    CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure ring buffer and line buffer  ******************************************//
#define    CFG_MODULE_ENABLE_RING_BUFFER_AND_LINE_BUFFER  true
//*******************************  Configure cpu drive inner flash  ******************************************//
#define CFG_EFLASH_PART_FORMAT     false       // ����ѡ��,�Ƿ���Ҫ������оƬ��
#define CFG_EFLASH_PAGE_SIZE                 512       // Ƭ��flash��ҳ��С����λ�ֽڡ�
#define CFG_EFLASH_SMALL_SECT_PAGE_NUM       256          // Ƭ��flash��С�����У��ж���ҳ��
#define CFG_EFLASH_LARGE_SECT_PAGE_NUM       0          // Ƭ��flash�Ĵ������У��ж���ҳ��
#define CFG_EFLASH_NORMAL_SECT_PAGE_NUM      0          // Ƭ��flash�ı�׼�����У��ж���ҳ��
#define CFG_EFLASH_PLANE_SMALL_SECT_NUM      16          // Ƭ��flash�����洢���У��ж���С������
#define CFG_EFLASH_PLANE_LARGE_SECT_NUM      0          // Ƭ��flash�����洢���У��ж��ٴ�������
#define CFG_EFLASH_PLANE_NORMAL_SECT_NUM     0        // Ƭ��flash�����洢���У��ж��ٱ�׼������
#define CFG_EFLASH_PLANE_NUM                 1          // Ƭ��flash�����洢�������
#define CFG_EFLASH_MAPPED_START_ADDR         0x8000000  // Ƭ��flash��ӳ����ʼ��ַ��
#define    CFG_MODULE_ENABLE_CPU_DRIVE_INNER_FLASH  true
//*******************************  Configure emflash insatall xip  ******************************************//
#define CFG_EFLASH_XIP_PART_START      6           // ������ʼ����д��ţ���Ŵ�0��ʼ����
#define CFG_EFLASH_XIP_PART_END        -1          // ����������-1��ʾ���һ�飬�����������6����ʼ����0����÷���ʹ�õĿ�Ϊ0��1��2��3��4��5��
#define CFG_EFLASH_XIP_PART_FORMAT     false       // ����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
#define CFG_EFLASH_XIPFSMOUNT_NAME   "xip-app"     // �谲װ���ļ�ϵͳ��mount�����֣�NULL��ʾ��flash�������ļ�ϵͳ
#define    CFG_MODULE_ENABLE_EMFLASH_INSATALL_XIP  true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure STM32 commond code  ******************************************//
#define    CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096     // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         15       // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    15       // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096     // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024     // "IDLE�¼�ջ�ߴ�",IDLE�¼������������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_IDLE_MONITOR_CYCLE  30       // "IDLE��������",����IDLE�¼���������1/16 CPUռ�ȵ�ʱ�䣬������0=������
#define CFG_OS_TINY             false    // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure misc  ******************************************//
#define    CFG_MODULE_ENABLE_MISC          true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8       // "ʱ��",����ʱ���Ƕ�8��
#define    CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure debug information  ******************************************//
#define    CFG_MODULE_ENABLE_DEBUG_INFORMATION  true
//*******************************  Configure flash  ******************************************//
#define    CFG_MODULE_ENABLE_FLASH         true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true   // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure cpu onchip uart  ******************************************//
#define    CFG_UART_SENDBUF_LEN            32                //
#define    CFG_UART_RECVBUF_LEN            32                //
#define    CFG_UART1_ENABLE                false             //
#define    CFG_UART2_ENABLE                true              //
#define    CFG_UART3_ENABLE                true              //
#define    CFG_UART4_ENABLE                false             //
#define    CFG_UART5_ENABLE                false             //
#define    CFG_UART6_ENABLE                false             //
#define    CFG_UART7_ENABLE                false             //
#define    CFG_UART8_ENABLE                false             //
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_UART  true
//*******************************  Configure loader  ******************************************//
#define CFG_UPDATEIBOOT_EN      false        // "�Ƿ�֧�����߸���Iboot"��
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN      // "APP����ģʽ",EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL    // "APPУ�鷽��",
#define CFG_IBOOT_VERSION_SMALL       00         // "Iboot�汾��:��",xx.xx.__��APP����
#define CFG_IBOOT_VERSION_MEDIUM      00         // "Iboot�汾��:��",xx.__.xx��APP����
#define CFG_IBOOT_VERSION_LARGE       01         // "Iboot�汾��:��",__.xx.xx��APP����
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"            // "������ibootĬ�ϴ洢·��"
#define CFG_APP_UPDATE_NAME        "/yaf2/app.bin"             // "������appĬ�ϴ洢·��"
#define CFG_FORCED_UPDATE_PATH     "/SD/djyapp.bin"            // "ǿ���������ļ�·��"
#define    CFG_MODULE_ENABLE_LOADER        true
//*******************************  Configure stdio  ******************************************//
#define    CFG_STDIO_STDIN_MULTI           true              //
#define    CFG_STDIO_STDOUT_FOLLOW         true              //
#define    CFG_STDIO_STDERR_FOLLOW         true              //
#define    CFG_STDIO_FLOAT_PRINT           true              //
#define    CFG_STDIO_STDIOFILE             true              //
#define    CFG_STDIO_IN_NAME               "/dev/UART2"      //
#define    CFG_STDIO_OUT_NAME              "/dev/UART2"      //
#define    CFG_STDIO_ERR_NAME              "/dev/UART2"      //
#define    CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000       // "ִ��shell�����ջ�ߴ�",
#define CFG_ADD_ROUTINE_SHELL      true         // "�Ƿ����ӳ���shell����",
#define CFG_ADD_EXPAND_SHELL       true         // "�Ƿ�������չshell����",
#define CFG_ADD_GLOBAL_FUN         false        // "����ȫ�ֺ�����shell",
#define CFG_SHOW_ADD_SHEELL        true         // "��ʾ�ڱ��봰�����ӵ�shell����",
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Core Clock  ******************************************//
#define    CFG_CORE_MCLK                   (216.0*Mhz)       //��Ƶ���ں�Ҫ�ã����붨��
//*******************************  DjyosProduct Configuration  ******************************************//
#define    PRODUCT_MANUFACTURER_NAME       ""                //��������
#define    PRODUCT_PRODUCT_CLASSIFY        ""                //��Ʒ����
#define    PRODUCT_PRODUCT_MODEL           ""                //��Ʒ�ͺ�
#define    PRODUCT_VERSION_LARGE           0                 //�汾��,__.xx.xx
#define    PRODUCT_VERSION_MEDIUM          0                 //�汾��,xx.__.xx
#define    PRODUCT_VERSION_SMALL           0                 //�汾��,xx.xx.__
#define    PRODUCT_PRODUCT_MODEL_CODE      ""                //��Ʒ�ͺű���
#define    PRODUCT_PASSWORD                ""                //��Ʒ��Կ
#define    PRODUCT_OTA_ADDRESS             ""                //OTA��������ַ
#define    PRODUCT_BOARD_TYPE              "NUCLEO-H743ZI"   //�������
#define    PRODUCT_CPU_TYPE                "stm32h743xx"     //CPU����


#endif