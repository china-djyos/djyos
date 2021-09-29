/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

#define CFG_RUNMODE_BAREAPP    1
//*******************************  Configure lock  ******************************************//
#define     CFG_LOCK_LIMIT                   3                     // "��������",�������������������ź����ͻ�����
//*******************************  Configure heap  ******************************************//
#define     CFG_DYNAMIC_MEM                  false               // "ȫ���ܶ�̬����",��ʹѡfalse��Ҳ����ʹ��malloc-free�����ڴ棬��ʹ���в����� �������û��ֲᡷ�ڴ�����½�
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     256          // "��ʼ��ջ�ռ�",�����ʼ������ʹ�õ�ջ�ռ䣬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_EVENT_LIMIT         3            // "�¼�������ֵ",�¼�����
#define CFG_EVENT_TYPE_LIMIT    3            // "�¼���������ֵ",�¼�������
#define CFG_MAINSTACK_LIMIT     1024          // "main����ջ�ߴ�",main�������������ջ�ߴ�
#define CFG_IDLESTACK_LIMIT     1024          // "IDLE�¼�ջ�ߴ�",IDLE�¼����������е�ջ�ߴ磬һ�㰴Ĭ��ֵ�Ϳ�����
#define CFG_OS_TINY             true         // "tiny���ں�",true=������Դ��ȱ�ĳ��ϣ��ں˻�ü������¼��������֣��¼�����ʱ��ͳ�ơ�
//*******************************  Configure kernel object system  ******************************************//
#define     CFG_OBJECT_LIMIT                 5                   // "��������ʼֵ"��������Զ�����
#define     CFG_HANDLE_LIMIT                 5                   // "�������ʼֵ"��������Զ�����
//*******************************  Configure memory pool  ******************************************//
#define     CFG_MEMPOOL_LIMIT                5                   // "�ڴ��������ֵ",
//*******************************  Configure loader  ******************************************//
#define     CFG_APP_RUNMODE                  CN_DIRECT_RUN           // CN_DIRECT_RUN=ֱ�Ӵ�flash�����У�CN_FORM_FILE=���ļ�ϵͳ���ص��ڴ����У�
#define     CFG_APP_VERIFICATION             VERIFICATION_NULL       // �Ƿ��APP�������CRCУ�飬��Ҫ�����������Ų���ҪCRCУ��
#define     CFG_IBOOT_VERSION                01                      // Iboot�����汾��
#define     CFG_IBOOT_UPDATE_NAME            "/yaf2/iboot.bin"       // "Iboot����"
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (80*Mhz)          //��Ƶ���ں�Ҫ�ã����붨��

#endif
