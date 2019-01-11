/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //�����������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //��ʹѡfalseҲ����ʹ��malloc-free�����ڴ浫ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //�����豸����
//*******************************  Configure cpu_peri_uart  ******************************************//
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
#define CFG_UART1_MODE                   1                  //CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART2_MODE                   1                  //CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART3_MODE                   1                  //CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART4_MODE                   1                  //CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART5_MODE                   1                  //CN_UART_POLL or CN_UART_GENERAL
#define CFG_UART1_ENABLE                 true               //
#define CFG_UART2_ENABLE                 false              //
#define CFG_UART3_ENABLE                 false              //
#define CFG_UART4_ENABLE                 false              //
#define CFG_UART5_ENABLE                 false              //
//*******************************  Configure wdt  ******************************************//
#define     CFG_WDT_LIMIT                    10                //������������
#define     CFG_WDTMSG_LIMIT                 3                 //�������Ź�����Ϣ���е���󳤶�
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //�����ʼ������ʹ�õ�ջ�ռ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         10      //�¼�����
#define CFG_EVENT_TYPE_LIMIT    10      //�¼�������
#define CFG_MAINSTACK_LIMIT     4096    //main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024    //IDLE�¼����������е�ջ�ߴ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false   //true=������Դ��ȱ�ĳ����ں˻�ü������¼����������¼�����ʱ��ͳ�ơ�
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //������10���ڴ��
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN     //EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_IBOOT_CRC   EN_NO_APP_CRC      //�Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_APP_FILENAME        ""              //CFG_APP_RUNMODE==EN_FORM_FILE�����������ļ���
#define CFG_IBOOT_VERSION       "V1.0.0"        //Iboot�����汾��
#define CFG_IBOOT_UPDATE_NAME   "iboot.bin"     //iboot�����ļ������˶����ã��Ժ�ĵ�
#define CFG_IBOOT_UPDATE_PATH   "\\"     //iboot����·�������˶����ã��Ժ�ĵ�
//*******************************  Configure cpu_peri_iic  ******************************************//
#define CFG_IIC_BUF_LEN           32       //����IIC��������С
#define CFG_IIC1_ENABLE           false     //
#define CFG_IIC2_ENABLE           false     //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (12*Mhz)          //��Ƶ���ں�Ҫ�ã����붨��

#endif