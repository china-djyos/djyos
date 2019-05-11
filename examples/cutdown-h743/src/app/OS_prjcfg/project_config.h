/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

#define CFG_RUNMODE_BAREAPP    1
//*******************************  Configure component lock  ******************************************//
#define     CFG_LOCK_LIMIT                   3                      // "��������",�������������������ź����ͻ�����
//*******************************  Configure component heap  ******************************************//
#define     CFG_DYNAMIC_MEM                  false                // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure component stdio  ******************************************//
#define     CFG_STDIO_FLOAT_PRINT            false             //
#define     CFG_STDIO_STDIOFILE              false             //
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)//
#define     CFG_STDIO_IN_NAME                "/dev/UART1"      //
#define     CFG_STDIO_OUT_NAME               "/dev/UART1"      //
#define     CFG_STDIO_ERR_NAME               "/dev/UART1"      //
//*******************************  Configure component kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     256           // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         3             // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    3             // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     1024           // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024           // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             true          // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure os Obj file  ******************************************//
#define     CFG_OBJECT_LIMIT                 5                 //������Զ�����
#define     CFG_HANDLE_LIMIT                 5                 //������Զ�����
//*******************************  Configure os memory poll  ******************************************//
#define     CFG_MEMPOOL_LIMIT                5                    // "�ڴ��������ֵ",
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //��Ƶ���ں�Ҫ�ã����붨��

#endif
