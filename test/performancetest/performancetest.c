//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ�飺ϵͳ���ܲ���ģ��
//����:
//�汾��V1.0.0
//�ļ�����:
//����˵��:
//�޶���ʷ:
//
#include "systime.h"
#include "stdint.h"
#include "stdio.h"
#include "djyos.h"
#include "int.h"
#include "lock.h"
#include "atomic.h"
#include "project_config.h"
#include "stdlib.h"
#include "string.h"
#include "timer_hard.h"

#define CN_TEST_TIMES   1000    //���Դ���
static u8 Cpu_Tatio[3] = {10,50,99};        //����Cpu��ռ����

u32 u32g_GetTimeCost;            //����ϵͳʱ�亯��������ʱ��
u32 u32g_TimerFreq;
struct SemaphoreLCB* TestSemp;
struct MutexLCB* TestMutex;
u32 TimeRecordStart,TimeRecordEnd;
u32 TimeRecord[CN_TEST_TIMES+2];    //������Σ���ĩ���β�Ҫ
u32 TestOffset;
u32 TimeRecord2[CN_TEST_TIMES+2];   //������Σ���ĩ���β�Ҫ
u32 TestOffset2;
bool_t TestEnd;
ptu32_t TimerHandle;
u32 u32g_TimerFreq;
u32 IntLine;
void Test_ShowValue(u32 *data,u8 Ratio,u32 freq,char *info);
u32 __Test_TimerStart(void);
bool_t __Test_TimerEnd(u32 *val);
void Test_IntSyncHook(ucpu_t SchType);
void Test_HighPrioHook(ucpu_t SchType);
void Test_LowPrioHook(ucpu_t SchType);

//���¼���������BSP��ʵ�֣�һ����CPU����Ŀ¼��
__attribute__ ((weak)) u32 __Test_TimerStart(void)
{
    printf("����BSP��ʵ�ֶ�ʱ��غ���\r\n");
    return 0;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((weak)) bool_t __Test_TimerEnd(u32 *val)
{
    printf("����BSP��ʵ�ֶ�ʱ��غ���\r\n");
    return false;
}

__attribute__ ((weak)) void Test_IntSyncHook(ucpu_t SchType)
{
    printf("����BSP��ʵ�ֶ�ʱ��غ���\r\n");
    return;
}
#pragma GCC diagnostic pop

void Test_PushTimeISR(u32 time_counter)
{
    if(TestOffset < CN_TEST_TIMES+2)
    {
        TimeRecord[TestOffset++] = time_counter;
    }
}
void Test_PushTimeHook(u32 time_counter)
{
    if(TestOffset2 < CN_TEST_TIMES+2)
    {
        TimeRecord2[TestOffset2++] = time_counter;
    }
}

//�����̣߳����ȼ�һ��һ�ͣ������ȼ��̻߳������󲻵��������������������л��������ȼ���
//�����ȼ��ͷŻ������󣬻����������ȼ��ͱ�����˻�����������ĸ����ȼ��߳���ռ���Ӷ������л���
ptu32_t Test_MutexHighPrioEvent(void)
{
    while(1)                  //����ʱ��ӦΪ2��������
    {
        TimeRecordStart = __Test_TimerStart();
        Lock_MutexPend(TestMutex, CN_TIMEOUT_FOREVER); //�϶������������л��������ȼ��߳�
        Djy_EventDelay(1000);
        Lock_MutexPost(TestMutex);   //�ͷŻ�ʹ�����ȼ��̵߳õ���������������ռ
        if(TestEnd)
            break;
    }

    return 0;
}

ptu32_t Test_MutexLowPrioEvent(void)
{
    while(1)                  //����ʱ��ӦΪ2��������
    {
        Lock_MutexPend(TestMutex, CN_TIMEOUT_FOREVER);
        Djy_EventDelay(1000);
        TimeRecordStart = __Test_TimerStart();
        Lock_MutexPost(TestMutex);   //�ͷŻ�ʹ�����ȼ��̵߳õ���������������ռ
        if(TestEnd)
            break;
    }

    return 0;
}

void Test_MutexTest(void)
{
    u16 evttID1;
    u16 evttID2;
    u32 loop;
    TestEnd = false;
    printf(" ***************** pend/post�����������¼��л����� ***************** \r\n\n");
    //�����ź���
    TestMutex = Lock_MutexCreate(NULL);          //��ʼ���ź���Ϊ0
    if(TestMutex == NULL)
    {
        printf("TestMutex Create Failed\r\n");
        return;
    }
    //ע���߳�1 ���ȼ�Ϊ30,���߳����ȼ���,���ȵ�������������
    evttID1 = Djy_EvttRegist(EN_CORRELATIVE,30,0,0,
                        Test_MutexHighPrioEvent,NULL,1000,NULL);

    if(evttID1 !=CN_EVTT_ID_INVALID)
    {
        Djy_RegisterHook(evttID1,Test_HighPrioHook);
        Djy_EventPop(evttID1,NULL,0,0,0,0);
    }
     //ע���߳�2,���ȼ�Ϊ40,���߳����ȼ���,
    evttID2 = Djy_EvttRegist(EN_CORRELATIVE,40,0,0,
                        Test_MutexLowPrioEvent,NULL,1000,NULL);

    if(evttID2!=CN_EVTT_ID_INVALID)
    {
        Djy_RegisterHook(evttID2,Test_LowPrioHook);
        //���Ե�ʱ��֤�����̵߳�������ȼ�Ϊ��B�߳�,�����ȵ�ʱ���һ����ȡ�õ�Cpuִ��Ȩ�Ĳ��Ǹ��߳�
        Djy_EventPop(evttID2,NULL,0,0,0,0);
    }
    Djy_SetEventPrio(Djy_MyEventId(), CN_PRIO_RRS);     //ʹ�������߳��Ǹ����ȼ��߳�
    for(loop = 0; loop < sizeof(Cpu_Tatio) / sizeof(u8); loop++)
    {
        atom_set32(&TestOffset,0);
        atom_set32(&TestOffset2,0);
        while(1)
        {
            Djy_DelayUs(Cpu_Tatio[loop]*100);
            Djy_EventDelay(10000 - Cpu_Tatio[loop]*100);
            if(TestOffset>=CN_TEST_TIMES+2)         //����������ͷ���Դ
            {
                break;
            }
        }
        Test_ShowValue(TimeRecord,Cpu_Tatio[loop],u32g_TimerFreq,"�ͷŻ�����ʹ�����е��¼������л��ٶ�");
        printf("\r\r\n");
        Test_ShowValue(TimeRecord2,Cpu_Tatio[loop],u32g_TimerFreq,"���󻥳��������������л��ٶ�");
        printf("\r\r\n");
    }
    TestEnd = true;
    Djy_EventDelay(2000);       //wait for release evtt
    Djy_EvttUnregist(evttID1);
    Djy_EvttUnregist(evttID2);
    Lock_MutexDelete(TestMutex);
}

// =============================================================================
// ��������:�ź���A�����߳�
//       ���ȼ� 2
// �����������
// ���������
// ����ֵ  ��
// =============================================================================
ptu32_t Test_SempLowPrioEvent(void)
{
    while(1)
    {
        TimeRecordStart = __Test_TimerStart();
        //��Evtt�����У�Evtt���ȡ�����ź����л���EvttA
        Lock_SempPost(TestSemp);//�ͷŵ�ʱ����������,��ʱ�������л���B�߳�
        if(TestEnd)
            break;
        Djy_EventDelay(1000);
    }
    return 0;
}

// =============================================================================
// ��������:�ź���B�����߳�
//       ���ȼ� 1
// �����������
// ���������
// ����ֵ  ��
// =============================================================================
ptu32_t Test_SempHighPrioEvent(void)
{
    while(1)
    {
        TimeRecordStart = __Test_TimerStart();
        //��Evtt�����У�Evtt���ȡ�����ź����л���EvttA
        Lock_SempPend(TestSemp, CN_TIMEOUT_FOREVER);
        if(TestEnd)
            break;
        Djy_EventDelay(1000);
    }
    return 0;
}

//���Ӻ�����ֻ�Ǽ�¼��Ӧ�߳�����ʱ���ʱ��
//�߳�B���Ӻ���,��ͳһ���߳�������Ϊ�߳��л���������ȡʱ��,��Ӧ�̷߳��������Լ��г���ʱ������,��ϵͳ��
//���е��߳̽϶� ���ȱȽϸ���,Ӧ��֤��ǰ�����߳���ϵͳ�����ȼ���ߵ�,��Ȼ���Խ���޷�Ԥ��
void Test_HighPrioHook(ucpu_t SchType)
{
    //�����ʱ��Ż�ȡʱ��,�г���������
    if(SchType == EN_SWITCH_IN)
    {
         //��¼����
         if(__Test_TimerEnd(&TimeRecordEnd))     //��ȡ��������ϵͳʱ��
         {
             if(TestOffset < CN_TEST_TIMES+2)
             {
                 TimeRecord[TestOffset++] = TimeRecordEnd - TimeRecordStart - u32g_GetTimeCost;
             }
             else
             {
             }
         }
    }
}

//���Ӻ�����ֻ�Ǽ�¼��Ӧ�߳�����ʱ���ʱ��
//�߳�A���Ӻ���,��ͳһ���߳�������Ϊ�߳��л���������ȡʱ��,��Ӧ�̷߳��������Լ��г���ʱ������,��ϵͳ��
//���е��߳̽϶� ���ȱȽϸ���,Ӧ��֤��ǰ�����߳���ϵͳ�����ȼ���ߵ�,��Ȼ���Խ���޷�Ԥ��
void Test_LowPrioHook(ucpu_t SchType)
{
    //�����ʱ��Ż�ȡʱ��,�г���������
    if(SchType == EN_SWITCH_IN)
    {
        //��¼����
        if(__Test_TimerEnd(&TimeRecordEnd))     //��ȡ��������ϵͳʱ��
        {
            if(TestOffset < CN_TEST_TIMES+2)
            {
                TimeRecord[TestOffset++] = TimeRecordEnd - TimeRecordStart - u32g_GetTimeCost;
            }
            else
            {
            }
        }
    }
}

void Test_ShowValue(u32 *data,u8 Ratio,u32 freq,char *info)
{
    u32 MaxVal,MinVal,AverageVal=0;
    u32 loop1;

    MaxVal = 0;
    MinVal = CN_LIMIT_UINT32;
    //��һ�κ����һ�μ�¼�����ݣ������д�����֮��
    for(loop1 = 1; loop1 < CN_TEST_TIMES+1; loop1++)
    {
        if(data[loop1] < MinVal)
            MinVal = data[loop1];
        if(data[loop1] > MaxVal)
            MaxVal = data[loop1];
        AverageVal += data[loop1];
    }
//    AverageVal = AverageVal/9998;
    printf("%s���Խ������(��λ:ns):\r\n",info);
    if(Ratio != 0)
        printf("cpuռ��������%d%%:\r\n",Ratio);
    printf("���ֵ:\t%.2f\r\n",(float)MaxVal*10E8/freq);
    printf("��Сֵ:\t%.2f\r\n",(float)MinVal*10E8/freq);
    printf("ƽ��ֵ:\t%.2f\r\n",(float)AverageVal*10E8/freq/CN_TEST_TIMES);
    printf("����%d�Σ�����ӡǰ10�β�������(��λ:ns):\r\n", CN_TEST_TIMES);
    for(loop1 = 1; loop1 < 11; loop1++)
    {
        printf("%.2f\t",(float)data[loop1]*10E8/freq);
    }
    printf("\r\n");
}
// =============================================================================
// ��������:ʵʱ�ж��Լ��첽�źŲ���ģ��
//
// ����˵����meaNUM  :�����Ĵ��� DataNum
//        DataNum :�ն˴�ӡ���������Ŀ
//        MeaType :�������� EN_REAL_INT Ϊʵʱ�ж�  EN_SYNC_SINGLE Ϊ�첽�ź�
// ���������
// ����ֵ  ��
// =============================================================================
void Test_SempTest(void)
{
    u16 evttID1;
    u16 evttID2;
    u32 loop;
    TestEnd = false;
    printf(" ***************** pend/post�ź��������¼��л����� ***************** \r\n\n");
    //�����ź���
    TestSemp = Lock_SempCreate(1,0,0,NULL);          //��ʼ���ź���Ϊ0
    if(TestSemp == NULL)
    {
        printf("m_Sem Create Failed\r\n");
        return;
    }
    //ע���߳�B ���ȼ�Ϊ30,���߳����ȼ���,���ȵ�������������
    evttID1 = Djy_EvttRegist(EN_CORRELATIVE,30,0,0,
                        Test_SempHighPrioEvent,NULL,3000,NULL);

    if(evttID1 !=CN_EVTT_ID_INVALID)
    {
        //ע���߳�B�Ĺ��Ӻ���:ע��-ֻҪ���߳��������(�л�/�����Լ��г��������)
        Djy_RegisterHook(evttID1,Test_HighPrioHook);
        Djy_EventPop(evttID1,NULL,0,0,0,0);
    }
     //ע���߳�A,���ȼ�Ϊ40,���߳����ȼ���,
    evttID2 = Djy_EvttRegist(EN_CORRELATIVE,40,0,0,
                        Test_SempLowPrioEvent,NULL,3000,NULL);

    if(evttID2!=CN_EVTT_ID_INVALID)
    {
        //�߳�Bע�ṳ�Ӻ���,���Ӻ������ж������뻹���г���A->B ����B->A
        Djy_RegisterHook(evttID2,Test_LowPrioHook);
        //���Ե�ʱ��֤�����̵߳�������ȼ�Ϊ��B�߳�,�����ȵ�ʱ���һ����ȡ�õ�Cpuִ��Ȩ�Ĳ��Ǹ��߳�
        Djy_EventPop(evttID2,NULL,0,0,0,0);
    }

    for(loop = 0; loop < sizeof(Cpu_Tatio) / sizeof(u8); loop++)
    {
        atom_set32(&TestOffset,0);
        atom_set32(&TestOffset2,0);
        while(1)
        {
            Djy_DelayUs(Cpu_Tatio[loop]*100);
            Djy_EventDelay(10000 - Cpu_Tatio[loop]*100);
            if(TestOffset>=CN_TEST_TIMES+2)         //����������ͷ���Դ
            {
                break;
            }
        }
        Test_ShowValue(TimeRecord,Cpu_Tatio[loop],u32g_TimerFreq,"�ͷ��ź���ʹ�����е��¼������л��ٶ�");
        printf("\r\n");
        Test_ShowValue(TimeRecord2,Cpu_Tatio[loop],u32g_TimerFreq,"�����ź��������������л��ٶ�");
        printf("\r\n");
    }
    TestEnd = true;
    Djy_EventDelay(2000);       //wait for release evtt
    Djy_EvttUnregist(evttID1);
    Djy_EvttUnregist(evttID2);
    Lock_SempDelete(TestSemp);
}

//ʵʱ�ж� �Լ��첽 �źű�ʶ
typedef enum{
    EN_REAL_INT     = true,             //ʵʱ�ж�
    EN_SYNC_INT  = false,            //�첽�ź�
}EN_INT_FLAG;

// =============================================================================
// ģ��˵��:eventDelay eventDelayTo �����¼��л� ģ�����
//
// ����˵����meaNUM  :�����Ĵ��� DataNum
//        DataNum :�ն˴�ӡ���������Ŀ
//        MeaType :�������� EN_REAL_INT Ϊʵʱ�ж�  EN_SYNC_SINGLE Ϊ�첽�ź�
// ���������
// ����ֵ  ��
// =============================================================================

//������ʱ�л������õĵ����ȼ��¼����������Ǹ���Զ�������¼�
ptu32_t Test_DelayThreadLow(void)
{
    while(1)
    {
        if(TestEnd)
            break;
    }
    return 0;
}

// =============================================================================
// ��������:eventDelay eventDelayTo �����¼��л� ģ�����
//�����β�  :
//      MeaNum   :   ��������
//      OutPutNum:   ����̨���������
//����ֵ   : ��
// =============================================================================
void Test_EventDelay(void)
{
    u16 evttID;

    TestEnd = false;
    atom_set32(&TestOffset,0);
    //start to run low pro thread
    evttID = Djy_EvttRegist(EN_CORRELATIVE,249,0,0,
                            Test_DelayThreadLow,NULL,1000,NULL);
    //�¼���������������Ϊ0,�����shell����,�������ɼ�shell�����в���
    if(evttID!=CN_EVTT_ID_INVALID)
    {
        Djy_EventPop(evttID,NULL,0,0,0,0);
        Djy_RegisterHook(evttID, Test_HighPrioHook);
    }
    while(1)
    {
        TimeRecordStart = __Test_TimerStart();
        Djy_EventDelay(1000);
        if(TestOffset>=CN_TEST_TIMES+2)         //����������ͷ���Դ
        {
            break;
        }
    }
    Test_ShowValue(TimeRecord, 0, u32g_TimerFreq,"�����ȼ�ʱ��EventDelay�����л��������ȼ��¼����л�ʱ��");
    TestEnd = true;
    Djy_EvttUnregist(evttID);
}
//�����ȼ��߳�

ptu32_t Test_PopThreadHight(void)
{
    u32 ptimes = 1;
    while(1)
    {
        Djy_WaitEvttPop(Djy_MyEvttId(), &ptimes, CN_TIMEOUT_FOREVER);
        if(TestEnd)
            break;
    }
    return 0;
}

// =============================================================================
// ��������:�����ȼ����������ȼ��¼������л�  SysMeaPopModuleInstall
//�����β�  :
//      MeaNum   :   ��������
//      OutPutNum:   ����̨���������
//����ֵ   : ��
// =============================================================================
void Test_PopHighPrioEvent(void)
{
    u16 evttID;

    printf(" ***************** ���������ȼ��¼�������ռ�ٶȲ��� ***************** \r\n\n");
    TestEnd = false;
    atom_set32(&TestOffset,0);
    //start to run low pro thread
    evttID = Djy_EvttRegist(EN_CORRELATIVE,30,0,0,
                            Test_PopThreadHight,NULL,1000,NULL);
    if(evttID!=CN_EVTT_ID_INVALID)
    {
        Djy_EventPop(evttID,NULL,0,0,0,0);
        Djy_RegisterHook(evttID, Test_HighPrioHook);
    }
    while(1)
    {
        TimeRecordStart = __Test_TimerStart();
        Djy_EventPop(evttID,NULL,0,0,0,0);
        Djy_EventDelay(1000);
        if(TestOffset>=CN_TEST_TIMES+2)         //����������ͷ���Դ
        {
            break;
        }
    }
    Test_ShowValue(TimeRecord, 0, u32g_TimerFreq,"���������ȼ��¼����л��������ȼ��¼��л�ʱ��");
    TestEnd = true;
    Djy_EvttUnregist(evttID);
}

ptu32_t Test_IntSyncThread(void)
{
    while(1)
    {
        Int_AsynSignalSync(IntLine);
        if(TestEnd)
            break;
    }

    return (ptu32_t)0;
}


// =============================================================================
// ��������:�ж�ͬ��ģ�����  SysMeaIntSynModuleInstall
//�����β�  :
//      MeaNum   :   ��������
//      OutPutNum:   ����̨���������
//      intLine  :   �ж���-����ָ���ж�ͬ�����ԵĶ�ʱ�����ڵ��ж���
//����ֵ   : ��
// =============================================================================
void Test_IntSync(void)
{
    u32 loop;
    u16 evttID;

    printf(" ***************** �ж�ͬ���¼���Ӧ�ٶȲ��� ***************** \r\n\n");
    TestEnd = false;
    atom_set32(&TestOffset,0);
    atom_set32(&TestOffset2,0);

    evttID = Djy_EvttRegist(EN_CORRELATIVE,30,0,0,
                            Test_IntSyncThread,NULL,2000,NULL);
    if(evttID!=CN_EVTT_ID_INVALID)
    {
        Djy_EventPop(evttID,NULL,0,0,0,0);
        Djy_RegisterHook(evttID, Test_IntSyncHook);
    }

    for(loop = 0; loop < sizeof(Cpu_Tatio) / sizeof(u8); loop++)
    {
        atom_set32(&TestOffset,0);
        atom_set32(&TestOffset2,0);
        while(1)
        {
            Djy_DelayUs(Cpu_Tatio[loop]*100);
            Djy_EventDelay(10000 - Cpu_Tatio[loop]*100);
            if(TestOffset2>=CN_TEST_TIMES+2)         //����������ͷ���Դ
            {
                break;
            }
        }
        Test_ShowValue(TimeRecord2, Cpu_Tatio[loop],u32g_TimerFreq,"�첽�ź�ͬ����Ӧʱ��");

        printf("\r\n");
    }
    TestEnd = true;
    Djy_EventDelay(2000);
    Djy_EvttUnregist(evttID);
}

ptu32_t SysMeaNoSwiPthread(void)
{
    return 0;
}

// =============================================================================
// ��������:�ź�������������POP ������ȣ����޲��� �л�ģ��
//�����β�  :
//      MeaNum   :   ��������
//      OutPutNum:   ����̨���������
//����ֵ   : ��
// =============================================================================
void Test_coreFunc(void)
{
    u16 evttID;
    u32 loop;

    printf(" ***************** ���Ĺ��ܺ���ִ��ʱ����� ***************** \r\n\n");
    evttID = Djy_EvttRegist(EN_CORRELATIVE,249,0,0,
                    SysMeaNoSwiPthread,NULL,2000,NULL);

    if(evttID!=CN_EVTT_ID_INVALID)
    {
        Djy_EventPop(evttID,NULL,0,0,0,0);
    }
    //����Djy_EventPopִ��ʱ�䣬���������¼����ƿ�
    for(loop = 0; loop < CN_TEST_TIMES+2; loop++)
    {
        TimeRecordStart = __Test_TimerStart();
        Djy_EventPop(evttID,NULL,0,0,0,0);
        __Test_TimerEnd(&TimeRecordEnd);
        TimeRecord[loop] = TimeRecordEnd - TimeRecordStart - u32g_GetTimeCost;
    }
    Test_ShowValue(TimeRecord, 0, u32g_TimerFreq,
                    "Djy_EventPopִ��ʱ�䣨���������¼���");
    printf("\r\n");
    //����Djy_EventPopִ��ʱ�䣬��Ҫ�����¼����ƿ�
    for(loop = 0; loop < CN_TEST_TIMES+2; loop++)
    {
        Djy_EventDelay(1000);   //�������µ������¼�������ϣ�ʹ��һ�ε����봴���¼����ƿ�
        TimeRecordStart = __Test_TimerStart();
        Djy_EventPop(evttID,NULL,0,0,0,0);
        __Test_TimerEnd(&TimeRecordEnd);
        TimeRecord[loop] = TimeRecordEnd - TimeRecordStart - u32g_GetTimeCost;
    }
    Test_ShowValue(TimeRecord, 0, u32g_TimerFreq,
                    "Djy_EventPopִ��ʱ�䣨�������¼���");
    printf("\r\n");

    //���� Lock_SempPend ִ��ʱ��
    //��ʼ��100000���źţ�ʹ Lock_SempPend �����ܳɹ�
    TestSemp = Lock_SempCreate(100000,80000,CN_BLOCK_FIFO,NULL);
    if(TestSemp == NULL)
    {
        printf("m_Sem Create Failed\r\n");
        return;
    }
    for(loop = 0; loop < CN_TEST_TIMES+2; loop++)
    {
        TimeRecordStart = __Test_TimerStart();
        Lock_SempPend(TestSemp, CN_TIMEOUT_FOREVER);
        __Test_TimerEnd(&TimeRecordEnd);
        TimeRecord[loop] = TimeRecordEnd - TimeRecordStart - u32g_GetTimeCost;

        TimeRecordStart = __Test_TimerStart();
        Lock_SempPost(TestSemp);
        __Test_TimerEnd(&TimeRecordEnd);
        TimeRecord2[loop] = TimeRecordEnd - TimeRecordStart - u32g_GetTimeCost;
    }
    Test_ShowValue(TimeRecord, 0, u32g_TimerFreq,"Lock_SempPend �������¼��л�ִ��ʱ��");
    printf("\r\n");
    Test_ShowValue(TimeRecord2, 0, u32g_TimerFreq,"Lock_SempPost �������¼��л�ִ��ʱ��");
    printf("\r\n");
    Lock_SempDelete(TestSemp);

    //���� Lock_MutexPend ִ��ʱ��
    //��ʼ��100000���źţ�ʹ Lock_SempPend �����ܳɹ�
    TestMutex = Lock_MutexCreate(NULL);
    if(TestMutex == NULL)
    {
        printf("TestMutex Create Failed\r\n");
        return;
    }
    for(loop = 0; loop < CN_TEST_TIMES+2; loop++)
    {
        TimeRecordStart = __Test_TimerStart();
        Lock_MutexPend(TestMutex, CN_TIMEOUT_FOREVER);
        __Test_TimerEnd(&TimeRecordEnd);
        TimeRecord[loop] = TimeRecordEnd - TimeRecordStart - u32g_GetTimeCost;

        TimeRecordStart = __Test_TimerStart();
        Lock_MutexPost(TestMutex);
        __Test_TimerEnd(&TimeRecordEnd);
        TimeRecord2[loop] = TimeRecordEnd - TimeRecordStart - u32g_GetTimeCost;
    }
    Test_ShowValue(TimeRecord, 0, u32g_TimerFreq,"Lock_MutexPend �������¼��л�ִ��ʱ��");
    printf("\r\n");
    Test_ShowValue(TimeRecord2, 0, u32g_TimerFreq,"Lock_MutexPost �������¼��л�ִ��ʱ��");
    printf("\r\n");
    Lock_MutexDelete(TestMutex);

}

// =============================================================================
// ��������:ʵʱ�ж��Լ��첽�źŲ���ģ��
//
// ����˵����meaNUM  :�����Ĵ��� DataNum
//        DataNum :�ն˴�ӡ���������Ŀ
//        MeaType :�������� EN_REAL_INT Ϊʵʱ�ж�  EN_SYNC_SINGLE Ϊ�첽�ź�
// ���������
// ����ֵ  ��
// =============================================================================
void Test_IntISRTest(EN_INT_FLAG MeaType)
{
    u32 loop;
    TestEnd = false;

    HardTimer_Ctrl(TimerHandle,EN_TIMER_SETINTPRO,MeaType);    //����ʵʱ�ź�
    for(loop = 0; loop < sizeof(Cpu_Tatio) / sizeof(u8); loop++)
    {
        atom_set32(&TestOffset,0);
        while(1)
        {
            Djy_DelayUs(Cpu_Tatio[loop]*100);
            Djy_EventDelay(10000 - Cpu_Tatio[loop]*100);
            if(TestOffset>=CN_TEST_TIMES+2)         //����������ͷ���Դ
            {
                break;
            }
        }
        if(MeaType == EN_REAL_INT)
            Test_ShowValue(TimeRecord, Cpu_Tatio[loop],u32g_TimerFreq,"ʵʱ�ж�ISR��Ӧʱ��");
        else
            Test_ShowValue(TimeRecord, Cpu_Tatio[loop],u32g_TimerFreq,"�첽�ź�ISR��Ӧʱ��");

        printf("\r\n");
    }
    TestEnd = true;
}

extern void *TimerReg;
void SysPerformTest(void)
{
    u32 RunTime,RunTime1;

    //��ʼ���������䶨ʱ��
    TimerHandle = HardTimer_Alloc(NULL);
    HardTimer_Ctrl(TimerHandle, EN_TIMER_GETREG, (ptu32_t)&TimerReg);
    HardTimer_Ctrl(TimerHandle,EN_TIMER_GETID,(ptu32_t)&IntLine);   //ȡ�жϺ�
    IntLine = IntLine>>16;
    u32g_TimerFreq = HardTimer_GetFreq(TimerHandle);   //��ȡ��ʱ��Ƶ��
    HardTimer_Ctrl(TimerHandle,EN_TIMER_SETCYCLE,u32g_TimerFreq/10000);  //�ж�����=0.1mS
    HardTimer_Ctrl(TimerHandle,EN_TIMER_ENINT,true);           //�ж�ʹ��
    HardTimer_Ctrl(TimerHandle,EN_TIMER_STARTCOUNT,0);         //��ʼ����
    printf("<<<<<<<<<<<<<<<<<ϵͳ���ܲ��Կ�ʼ��>>>>>>>>>>>>>>>\r\n\n");
    __Test_TimerEnd(&RunTime);
    __Test_TimerEnd(&RunTime1);
    u32g_GetTimeCost = RunTime1 - RunTime;

    printf("���Եİ����Ϊ:%s\r\n",DJY_BOARD);
    printf("������ƵΪ:%d\r\n",CFG_CORE_MCLK);

//---------------���ж���ع����Ȳ⣬Ȼ���Ѷ�ʱ�����ڼ�ʱ-----------------
    //ʵʱ�ж���Ӧʱ�����
    printf(" ***************** ʵʱ�ж�ISR��Ӧʱ����� ***************** \r\n\n");
    Test_IntISRTest(EN_REAL_INT);
    //ʵʱ�ж���Ӧʱ�����
    printf(" ***************** �첽�ź�ISR��Ӧʱ����� ***************** \r\n\n");
    Test_IntISRTest(EN_SYNC_INT);

    //�첽�ź�ͬ�������л�����
    Test_IntSync();

//---------------���ж���ع��ܲ�����ϣ���ʱ�����ڼ�ʱ-----------------
    HardTimer_Ctrl(TimerHandle,EN_TIMER_SETCYCLE,0xffffffff);
    //�ź��������л����л��ٶȲ���
    Test_SempTest();

    //�����������л����л��ٶȲ���
    Test_MutexTest();

    //�������Ĺ��ܺ�����ִ��ʱ��
    Test_coreFunc();

    //���������ȼ��¼������л�ʱ�����
    Test_PopHighPrioEvent();

    //eventdelay�����л����л�ʱ�����
    Test_EventDelay();

    /*��������DMIPS���ԡ� */
    extern void DhryStoneTest(void);
    DhryStoneTest();
    /*������������FMIPS���ԡ� */
    extern int WhetStoneTest(void);
    WhetStoneTest();

    printf("System perform test had finished\r\n");
    HardTimer_Free(TimerHandle);
    return ;
}



