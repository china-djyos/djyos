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
#define    CN_RUNMODE_BOOTSELF             1                //����IBOOT��������ģʽAPP
#define    CN_RUNMODE_APP                  2                //��IBOOT���ص�APP
#define    CFG_RUNMODE                     CN_RUNMODE_IBOOT   //��IBOOT���ص�APP
//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure cpu onchip peripheral lowpower control  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_PERIPHERAL_LOWPOWER_CONTROL  true
//*******************************  Configure cpu onchip uart  ******************************************//
#define CFG_UART_SENDBUF_LEN     (256)          //
#define CFG_UART_RECVBUF_LEN     (256)          //
#define CFG_USART1_ENABLE         true           //
#define CFG_USART2_ENABLE         false          //
#define CFG_USART3_ENABLE         false          //
#define CFG_UART4_ENABLE         false          //
#define CFG_UART5_ENABLE         false          //
#define CFG_LPUART1_ENABLE       false          //
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_UART  true
//*******************************  Configure debug information  ******************************************//
#define    CFG_MODULE_ENABLE_DEBUG_INFORMATION  true
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //�����豸����
#define    CFG_MODULE_ENABLE_DEVICE_FILE_SYSTEM  true
//*******************************  Configure flash  ******************************************//
#define    CFG_MODULE_ENABLE_FLASH         true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //��ʹѡfalseҲ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure int  ******************************************//
#define    CFG_MODULE_ENABLE_INT           true
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //�����ʼ������ʹ�õ�ջ�ռ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         10      //�¼�����
#define CFG_EVENT_TYPE_LIMIT    10      //�¼�������
#define CFG_IDLESTACK_LIMIT     1024    //IDLE�¼����������е�ջ�ߴ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_IDLE_MONITOR_CYCLE  30      //����IDLE�¼���������1/16 CPUռ�ȵ�ʱ��������0=������
#define CFG_MAINSTACK_LIMIT     4096    //main�������������ջ�ߴ�
#define CFG_OS_TINY             false   //true=������Դ��ȱ�ĳ����ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //������Զ�����
#define CFG_HANDLE_LIMIT        8   //������Զ�����
#define    CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure loader  ******************************************//
#define CFG_UPDATEIBOOT_EN      false       //
#define CFG_START_APP_IS_VERIFICATION      true       //
#define  CFG_APP_RUNMODE  CN_DIRECT_RUN     //CN_DIRECT_RUN=ֱ�Ӵ�flash�����У�CN_FORM_FILE=���ļ�ϵͳ���ص��ڴ�����
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL   //
#define CFG_IBOOT_VERSION_SMALL       00        //xx.xx.__APP����
#define CFG_IBOOT_VERSION_MEDIUM      00        //xx.__.xxAPP����
#define CFG_IBOOT_VERSION_LARGE       01        //__.xx.xxAPP����
#define CFG_IBOOT_UPDATE_NAME      "/yaf2/iboot.bin"           //
#define CFG_APP_UPDATE_NAME        "/yaf2/app.bin"            //
#define    CFG_MODULE_ENABLE_LOADER        true
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //�����������������ź����ͻ�����
#define    CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
#define    CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure message queue  ******************************************//
#define    CFG_MODULE_ENABLE_MESSAGE_QUEUE true
//*******************************  Configure misc  ******************************************//
#define    CFG_MODULE_ENABLE_MISC          true
//*******************************  Configure multiplex  ******************************************//
#define    CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure ring buffer and line buffer  ******************************************//
#define    CFG_MODULE_ENABLE_RING_BUFFER_AND_LINE_BUFFER  true
//*******************************  Configure shell  ******************************************//
#define CFG_SHELL_STACK            0x1000      //
#define CFG_ADD_ROUTINE_SHELL      true        //
#define CFG_ADD_EXPAND_SHELL       true        //
#define CFG_ADD_GLOBAL_FUN         false       //
#define CFG_SHOW_ADD_SHEELL        true        //
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true         //
#define CFG_STDIO_STDOUT_FOLLOW    true         //
#define CFG_STDIO_STDERR_FOLLOW    true         //
#define CFG_STDIO_FLOAT_PRINT      true         //
#define CFG_STDIO_STDIOFILE        true         //
#define CFG_STDIO_IN_NAME              "/dev/UART1"    //
#define CFG_STDIO_OUT_NAME             "/dev/UART1"    //
#define CFG_STDIO_ERR_NAME             "/dev/UART1"    //
#define    CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure STM32 commond code  ******************************************//
#define    CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //����ʱ���Ƕ�8��
#define    CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Core Clock  ******************************************//
#define  CFG_CORE_MCLK                   (12*Mhz)          //��Ƶ���ں�Ҫ�ã����붨��
//*******************************  DjyosProduct Configuration  ******************************************//
#define    PRODUCT_MANUFACTURER_NAME       ""                //��������
#define    PRODUCT_PRODUCT_CLASSIFY        ""                //��Ʒ����
#define    PRODUCT_PRODUCT_MODEL           ""                //��Ʒ�ͺ�
#define    PRODUCT_VERSION_LARGE           0                 //�汾��,__.xx.xx
#define    PRODUCT_VERSION_MEDIUM          0                 //�汾��,xx.__.xx
#define    PRODUCT_VERSION_SMALL           0                 //�汾��,xx.xx.__
#define    PRODUCT_PRODUCT_MODEL_CODE      ""                //��Ʒ�ͺű���
#define    PRODUCT_BRANCH                  ""                //��Ʒ��֧
#define    PRODUCT_PASSWORD                ""                //��Ʒ��Կ
#define    PRODUCT_OTA_ADDRESS             ""                //OTA��������ַ
#define    PRODUCT_BOARD_TYPE              ""                //�������
#define    PRODUCT_CPU_TYPE                ""                //CPU����


#endif
