/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//******************************* Configure DevFile ******************************************//
#define CFG_DEVFILE_LIMIT       10      //�����豸����
//******************************* Configure kernel ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //�����ʼ������ʹ�õ�ջ�ռ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         10      //�¼�����
#define CFG_EVENT_TYPE_LIMIT    10      //�¼�������
#define CFG_MAINSTACK_LIMIT     4096    //main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024    //IDLE�¼����������е�ջ�ߴ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false   //true=������Դ��ȱ�ĳ����ں˻�ü������¼����������¼�����ʱ��ͳ�ơ�
//******************************* Configure lock ******************************************//
#define CFG_LOCK_LIMIT          40      //�����������������ź����ͻ�����
//******************************* Configure MemoryPool ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //������10���ڴ��
//******************************* Configure loader ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN     //EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_IBOOT_CRC   EN_NO_APP_CRC      //�Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_APP_FILENAME        ""              //CFG_APP_RUNMODE==EN_FORM_FILE�����������ļ���
#define CFG_IBOOT_VERSION       "V1.0.0"        //Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME   "iboot.bin"     //iboot�����ļ������˶����ã��Ժ�ĵ�
#define CFG_IBOOT_UPDATE_PATH   "iboot.bin"     //iboot����·�������˶����ã��Ժ�ĵ�
//******************************* Configure heap ******************************************//
#define CFG_DYNAMIC_MEM 1  //1 = ֧�ֶ�̬�ڴ���伴ʹ��֧��Ҳ����ʹ��malloc-free�����ڴ浫ʹ���в������������û��ֲᡷ�ڴ�����½�
//******************************* Configure stdio ******************************************//
#define CFG_STDIO_RUN_MODE            (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW) //
#define CFG_STDIO_IN_NAME              "/dev/UART0"//
#define CFG_STDIO_OUT_NAME             "/dev/UART0"//
#define CFG_STDIO_ERR_NAME             "/dev/UART0"//
//******************************* Configure time ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //����ʱ���Ƕ�8��
//******************************* Configure cpu_peri_uart ******************************************//
#define CFG_UART0_SENDBUF_LEN            32                 //
#define CFG_UART0_RECVBUF_LEN            32                 //
#define CFG_UART0_DMABUF_LEN             32                 //
#define CFG_UART1_SENDBUF_LEN            32                 //
#define CFG_UART1_RECVBUF_LEN            32                 //
#define CFG_UART1_DMABUF_LEN             32                 //
#define CFG_UART2_SENDBUF_LEN            32                 //
#define CFG_UART2_RECVBUF_LEN            32                 //
#define CFG_UART2_DMABUF_LEN             32                 //
#define CFG_UART3_SENDBUF_LEN            32                 //
#define CFG_UART3_RECVBUF_LEN            32                 //
#define CFG_UART3_DMABUF_LEN             32                 //
#define CFG_UART4_SENDBUF_LEN            32                 //
#define CFG_UART4_RECVBUF_LEN            32                 //
#define CFG_UART4_DMABUF_LEN             32                 //
#define CFG_UART5_SENDBUF_LEN            32                 //
#define CFG_UART5_RECVBUF_LEN            32                 //
#define CFG_UART5_DMABUF_LEN             32                 //
#define CFG_UART0_ENABLE                 true               //
#define CFG_UART1_ENABLE                 false              //
#define CFG_UART2_ENABLE                 false              //
#define CFG_UART3_ENABLE                 false              //
#define CFG_UART4_ENABLE                 false              //
#define CFG_UART5_ENABLE                 false              //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (192*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
