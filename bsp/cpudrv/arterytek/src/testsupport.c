#include <cpu_peri.h>
#include <djyos.h>
#include <int.h>
#include <systime.h>

tmr_type *timer;
bool_t TimerStarted = false;    //��Ҫ�޸ı�������������ں���SysPerformTest��Ҫ��
extern u32 IntLine;     //��ʹ�ö�ʱ�����жϺţ��ڲ�����ں���SysPerformTest�и�ֵ

void Test_PushTimeISR(u32 time_counter);
void Test_PushTimeHook(u32 time_counter);

//��������ʵ��TimerIsr���ڶ�ʱ�������У��붨��� weak ����
//���������ǣ��������жϷ��������뱾������ʱ������������Test_PushTimeISR����
u32 __STM32Timer_isr(ptu32_t TimerHandle)
{
    return 0;
}

//ʵ�����ܼ��ģ���е� weak ����
//���ò����ö�ʱ��������־��
u32 __Test_TimerStart(void)
{
    return 0;
}

//ʵ�����ܼ��ģ���е� weak ����
//�����ʱ����������״̬����ȡ���ϴε��� __Test_TimerStart ������ʱ��������ֹͣ��ʱ����
bool_t __Test_TimerEnd(u32 *val)
{
    return false;
}

//ʵ�����ܼ��ģ���е� weak ����
//�����ж�ͬ�����ԣ��������жϷ��������뱾������ʱ������������Test_PushTimeHook����
void Test_IntSyncHook(ucpu_t SchType)
{
    return;
}
