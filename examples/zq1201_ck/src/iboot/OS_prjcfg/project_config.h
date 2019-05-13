/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure System: lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //�����������������ź����ͻ�����
//*******************************  Configure System: heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //��ʹѡfalseҲ����ʹ��malloc-free�����ڴ浫ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //�����豸����
//*******************************  Configure System: stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)//
#define     CFG_STDIO_IN_NAME                "/dev/UART2"      //
#define     CFG_STDIO_OUT_NAME               "/dev/UART2"      //
#define     CFG_STDIO_ERR_NAME               "/dev/UART2"      //
//*******************************  Configure cpu_peri_System: uart  ******************************************//
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
#define CFG_UART1_ENABLE                 false               //
#define CFG_UART2_ENABLE                 true              //
#define CFG_UART3_ENABLE                 false              //
#define CFG_UART4_ENABLE                 false              //
//*******************************  Configure shell  ******************************************//
#define     CFG_ADD_ROUTINE_SHELL            true              //
#define     CFG_ADD_EXPAND_SHELL             true              //
#define     CFG_ADD_GLOBAL_FUN               false             //
#define     CFG_SHOW_ADD_SHEELL              true              //
//*******************************  Configure System: Software Timers  ******************************************//
#define CFG_TIMER_SOUCE     1       //1=��Ӳ����ʱ���ṩʱ��Դ0=��tick�ṩʱ��Դ
#define CFG_TIMERS_LIMIT    5       //�ɴ����Ķ�ʱ��������������ͼ�ν���Ķ�ʱ����
//*******************************  Configure System: kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //�����ʼ������ʹ�õ�ջ�ռ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         10      //�¼�����
#define CFG_EVENT_TYPE_LIMIT    10      //�¼�������
#define CFG_MAINSTACK_LIMIT     4096    //main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024    //IDLE�¼����������е�ջ�ߴ�һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             false   //true=������Դ��ȱ�ĳ����ں˻�ü������¼����������¼�����ʱ��ͳ�ơ�
//*******************************  Configure System: memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //������10���ڴ��
//*******************************  Configure System: loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN     //EN_DIRECT_RUN=ֱ�Ӵ�flash�����У�EN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL      //�Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define CFG_APP_FILENAME        ""              //CFG_APP_RUNMODE==EN_FORM_FILE�����������ļ���
#define CFG_IBOOT_VERSION       01        //Iboot�����汾��
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (12*Mhz)          //��Ƶ���ں�Ҫ�ã����붨��

#endif
