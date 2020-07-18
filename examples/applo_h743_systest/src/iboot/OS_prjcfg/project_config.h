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
//*******************************  Configure watch dog  ******************************************//
#define CFG_WDT_LIMIT           10      //������������
#define    CFG_MODULE_ENABLE_WATCH_DOG     true
//*******************************  Configure cpu onchip timer  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_TIMER  true
//*******************************  Configure cpu onchip systime  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_SYSTIME  true
//*******************************  Configure cpu onchip dma  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_DMA  true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure STM32 commond code  ******************************************//
#define    CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096     // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         15       // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    15       // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     4096     // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024     // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_IDLE_MONITOR_CYCLE  30      //����IDLE�¼���������1/16 CPUռ�ȵ�ʱ��������0=������
#define CFG_OS_TINY             false    // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure misc  ******************************************//
#define    CFG_MODULE_ENABLE_MISC          true
//*******************************  Configure Software Timers  ******************************************//
#define CFG_TIMERS_LIMIT        5        // "��ʱ������",�ɴ����Ķ�ʱ��������������ͼ�ν���Ķ�ʱ����
#define CFG_TIMER_SOUCE_HARD    true     // "Ӳ����ʱ���ṩʱ��Դ",ѡ��ר��Ӳ������tick��ʱ��Դ
#define    CFG_MODULE_ENABLE_SOFTWARE_TIMERS  true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8       // "ʱ��",����ʱ���Ƕ�8��
#define    CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true   // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure cpu onchip uart  ******************************************//
#define CFG_UART_SENDBUF_LEN            32                   //  "���ڷ��ͻ��λ�������С",
#define CFG_UART_RECVBUF_LEN            32                   //  "���ڽ��ջ��λ�������С",
#define CFG_UART1_ENABLE                true                 //  "�����Ƿ�ʹ��UART1",
#define CFG_UART2_ENABLE                false                //  "�����Ƿ�ʹ��UART2",
#define CFG_UART3_ENABLE                false                //  "�����Ƿ�ʹ��UART3",
#define CFG_UART4_ENABLE                false                //  "�����Ƿ�ʹ��UART4",
#define CFG_UART5_ENABLE                false                //  "�����Ƿ�ʹ��UART5",
#define CFG_UART6_ENABLE                false                //  "�����Ƿ�ʹ��UART6",
#define CFG_UART7_ENABLE                false                //  "�����Ƿ�ʹ��UART7",
#define CFG_UART8_ENABLE                false                //  "�����Ƿ�ʹ��UART8",
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_UART  true
//*******************************  Configure tpl  ******************************************//
#define     CFG_TPL_PROTONUM            5        // "֧�ֵĴ���Э����"��ռ��һ�� tagTplProtoItem �ṹ
#define    CFG_MODULE_ENABLE_TPL           true
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true          // "�Ƿ�֧�ֶ��������豸",
#define CFG_STDIO_STDOUT_FOLLOW    true          // "stdout�Ƿ����stdin",
#define CFG_STDIO_STDERR_FOLLOW    true          // "stderr�Ƿ����stdin",
#define CFG_STDIO_FLOAT_PRINT      true          // "֧�ָ����ӡ"
#define CFG_STDIO_STDIOFILE        true          // "֧�ֱ�׼IO�ļ�"
#define CFG_STDIO_IN_NAME              "/dev/UART1"     // "��׼�����豸��",
#define CFG_STDIO_OUT_NAME             "/dev/UART1"     // "��׼����豸��",
#define CFG_STDIO_ERR_NAME             "/dev/UART1"     // "��׼err����豸��",
#define    CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000       // "ִ��shell�����ջ�ߴ�",
#define CFG_ADD_ROUTINE_SHELL      true         // "�Ƿ���ӳ���shell����",
#define CFG_ADD_EXPAND_SHELL       true         // "�Ƿ������չshell����",
#define CFG_ADD_GLOBAL_FUN         false        // "���ȫ�ֺ�����shell",
#define CFG_SHOW_ADD_SHEELL        true         // "��ʾ�ڱ��봰����ӵ�shell����",
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Core Clock  ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��
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
#define    PRODUCT_BOARD_TYPE              ""                //�������
#define    PRODUCT_CPU_TYPE                ""                //CPU����



#endif
