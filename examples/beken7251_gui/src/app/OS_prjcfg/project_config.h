/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

#define CFG_RUNMODE_BAREAPP    1
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40          // "��������",�������������������ź����ͻ�����
#define    CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true      // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10          // "�豸����",�����豸����
#define    CFG_MODULE_ENABLE_DEVICE_FILE_SYSTEM  true
//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure djybus  ******************************************//
#define    CFG_MODULE_ENABLE_DJYBUS        true
//*******************************  Configure debug information  ******************************************//
#define    CFG_MODULE_ENABLE_DEBUG_INFORMATION  true
//*******************************  Configure graphical kernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024        // "����������",gdd��gkernel��������Ļ��������ȣ��ֽ�����
#define    CFG_MODULE_ENABLE_GRAPHICAL_KERNEL  true
//*******************************  Configure int  ******************************************//
#define    CFG_MODULE_ENABLE_INT           true
//*******************************  Configure multiplex  ******************************************//
#define    CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure kernel  ******************************************//
#define    CFG_INIT_STACK_SIZE             4096                 // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define    CFG_EVENT_LIMIT                 15                   // "�¼�������ֵ",�¼�����
#define    CFG_EVENT_TYPE_LIMIT            15                   // "�¼���������ֵ",�¼�������
#define    CFG_MAINSTACK_LIMIT             4096                 // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define    CFG_IDLESTACK_LIMIT             1024                 // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define    CFG_OS_TINY                     false                // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8       // "��������ʼֵ"��������Զ�����
#define CFG_HANDLE_LIMIT        8       // "�������ʼֵ"��������Զ�����
#define    CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure loader  ******************************************//
#define    CFG_UPDATEIBOOT_EN              false              // "�Ƿ�֧�����߸���Iboot"��
#define    CFG_APP_RUNMODE                 EN_DIRECT_RUN      // EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define    CFG_APP_VERIFICATION            VERIFICATION_NULL  // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define    CFG_IBOOT_VERSION_SMALL         00                 // Iboot�����汾��,xx.xx.__
#define    CFG_IBOOT_VERSION_MEDIUM        00                 // Iboot�����汾��,xx.__.xx
#define    CFG_IBOOT_VERSION_LARGE         01                 // Iboot�����汾��,__.xx.xx
#define    CFG_IBOOT_UPDATE_NAME           "/yaf2/iboot.bin"  // "������ibootĬ�ϴ洢·��"
#define    CFG_APP_UPDATE_NAME             "/yaf2/app.bin"    // "������appĬ�ϴ洢·��"
#define    CFG_MODULE_ENABLE_LOADER        true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10          // "�ڴ��������ֵ",
#define    CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure ring buffer and line buffer  ******************************************//
#define    CFG_MODULE_ENABLE_RING_BUFFER_AND_LINE_BUFFER  true
//*******************************  Configure stdio  ******************************************//
#define    CFG_STDIO_STDIN_MULTI           true                  // "�Ƿ�֧�ֶ��������豸",
#define    CFG_STDIO_STDOUT_FOLLOW         true                  // "stdout�Ƿ����stdin",
#define    CFG_STDIO_STDERR_FOLLOW         true                  // "stderr�Ƿ����stdin",
#define    CFG_STDIO_FLOAT_PRINT           true                  // "֧�ָ����ӡ"
#define    CFG_STDIO_STDIOFILE             true                  // "֧�ֱ�׼IO�ļ�"
#define    CFG_STDIO_IN_NAME               ("/dev/UART2")        // "��׼�����豸��",
#define    CFG_STDIO_OUT_NAME              ("/dev/UART2")        // "��׼����豸��",
#define    CFG_STDIO_ERR_NAME              ("/dev/UART2")        // "��׼err����豸��",
#define    CFG_MODULE_ENABLE_STDIO         true
//*******************************  Configure shell  ******************************************//
#define    CFG_SHELL_STACK                 0x1000                // "ִ��shell�����ջ�ߴ�"
#define    CFG_ADD_ROUTINE_SHELL           true                  // "�Ƿ���ӳ���shell����"
#define    CFG_ADD_EXPAND_SHELL            true                  // "�Ƿ������չshell����"
#define    CFG_ADD_GLOBAL_FUN              false                 // "���ȫ�ֺ�����shell"
#define    CFG_SHOW_ADD_SHEELL             true                  // "��ʾ�ڱ��봰����ӵ�shell����"
#define    CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure spi bus  ******************************************//
#define    CFG_MODULE_ENABLE_SPI_BUS       true
//*******************************  Configure uart device file  ******************************************//
#define    CFG_MODULE_ENABLE_UART_DEVICE_FILE  true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure cpu onchip uart  ******************************************//
#define    CFG_UART1_SENDBUF_LEN           64                    // "UART1���ͻ��λ�������С",
#define    CFG_UART1_RECVBUF_LEN           64                    // "UART1���ջ��λ�������С",
#define    CFG_UART1_DMABUF_LEN            64                    // "UART1 DMA���λ�������С",
#define    CFG_UART2_SENDBUF_LEN           64                    // "UART2���ͻ��λ�������С",
#define    CFG_UART2_RECVBUF_LEN           64                    // "UART2���ջ��λ�������С",
#define    CFG_UART2_DMABUF_LEN            64                    // "UART2 DMA���λ�������С",
#define    CFG_UART3_SENDBUF_LEN           64                    // "UART3���ͻ��λ�������С",
#define    CFG_UART3_RECVBUF_LEN           64                    // "UART3���ջ��λ�������С",
#define    CFG_UART3_DMABUF_LEN            64                    // "UART3 DMA���λ�������С",
#define    CFG_UART4_SENDBUF_LEN           64                    // "UART4���ͻ��λ�������С",
#define    CFG_UART4_RECVBUF_LEN           64                    // "UART4���ջ��λ�������С",
#define    CFG_UART4_DMABUF_LEN            64                    // "UART4 DMA���λ�������С",
#define    CFG_UART1_ENABLE                false                 // "�Ƿ�ʹ��UART1",
#define    CFG_UART2_ENABLE                true                  // "�Ƿ�ʹ��UART2",
#define    CFG_UART3_ENABLE                false                 // "�Ƿ�ʹ��UART3",
#define    CFG_UART4_ENABLE                false                 // "�Ƿ�ʹ��UART4",
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_UART  true
//*******************************  Configure LCD driver st7796s  ******************************************//
#define    CFG_LCD_XSIZE                   320                 // "LCD���",
#define    CFG_LCD_YSIZE                   480                 // "LCD�߶�",
#define    CFG_ST7796S_DISPLAY_NAME        "lcdst7796s"        // "��ʾ������",����Һ����ʾ������
#define    CFG_ST7796S_HEAP_NAME           "extram"            // "����ʹ�ö���",����Һ��������ʹ�õĶ�����
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure cpu onchip flash  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_FLASH  true
//*******************************  Configure spi  ******************************************//
#define    CFG_SPI_CLK                     30000000          //SPI ʱ�ӡ�
#define    CFG_SPI_CPOL                    1                 //spiʱ�Ӽ��ԣ�1��SCK�ڿ���״̬���ڸߵ�ƽ��0����֮����
#define    CFG_SPI_CPHA                    1                 //spiʱ����λ��1����SCK���ڵĵڶ������ز������ݡ�0����SCK���ڵĵ�һ�����ز������ݣ�
#define    CFG_SPI_FLASH_RAM_POWER         true              //�Ƿ��flash��ram�ĵ�Դ��
#define    CFG_SPI_WORK_MODE_INTE          false             //����SPI�Ĺ���ģʽ��trueΪ�ж�ģʽͨ�ţ�falseΪ��ͨģʽ��
//******************************* Core Clock ******************************************//
#define    CFG_CORE_MCLK                   (180.0*Mhz)       //��Ƶ���ں�Ҫ�ã����붨��

#endif
