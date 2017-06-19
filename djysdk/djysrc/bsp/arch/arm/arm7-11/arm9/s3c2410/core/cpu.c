//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
//����ģ��: ������
//���ߣ�lst
//�汾��V1.0.1
//�ļ�����: ����������CPUֱ����صĴ��롣
//����˵��:
//�޶���ʷ:
//2. ����: 2009-04-24
//   ����: lst
//   �°汾��: V1.0.1
//   �޸�˵��: ɾ����һЩΪdlsp�汾׼���Ķ���
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "align.h"
#include "stdint.h"
#include "stdlib.h"
#include "int.h"
#include "hard-exp.h"
#include "string.h"
#include "arch_feature.h"
#include "djyos.h"
#include "cpu.h"
#include "cpu_peri.h"

void __asm_reset_thread(void (*thread_routine)(void),struct ThreadVm *vm);

extern s64  g_s64OsTicks;             //����ϵͳ����ticks��
u32 g_u32CycleSpeed; //for(i=j;i>0;i--);ÿѭ��������*1.024
u32 g_u32HundreUsfor;
//tickѡ��t64-0��t12

//----�����߳�-----------------------------------------------------------------
//���ܣ�Ϊ�¼����ʹ���α���������ʼ�������Ļ�������װִ�к��������������߳�
//������evtt_id�����������߳���������¼�����id
//���أ��´������̵߳������ָ��
//ע: ��ֲ���к���
//-----------------------------------------------------------------------------
struct ThreadVm *__CreateThread(struct EventType *evtt,u32 *stack_size)
{
    struct ThreadVm  *result;
    ptu32_t  len;

    //���������ջ:�߳�+��󵥸�api�����ջ
    len = evtt->stack_size;
    //ջ����Ҫ���룬malloc�����ܱ�֤ջ���Ƕ���ģ����볤�ȿ���ʹջ������
    len = align_up_sys(len);
    result=(struct ThreadVm  *)__MallocStack(len);
    *stack_size = len;
    if(result==NULL)
    {
        Djy_SaveLastError(EN_MEM_TRIED);   //�ڴ治�㣬���ش���
        return result;
    }
    len = M_CheckSize(result);
    memset(result,'d',len);
    result->stack_top = (u32*)((ptu32_t)result+len); //ջ����ַ����ֲ����
    result->next = NULL;
    result->stack_size = len - sizeof(struct ThreadVm); //����ջ���
    result->host_vm = NULL;
    //��λ������������߳�
    __asm_reset_thread(evtt->thread_routine,result);
    return result;
}

//----��̬�����߳�-------------------------------------------------------------
//���ܣ�Ϊ�¼����ʹ����̣߳���ʼ�������Ļ�������װִ�к��������������߳�
//������evtt_id�����������߳���������¼�����id
//���أ��´������߳�ָ��
//ע: ��ֲ���к���
//-----------------------------------------------------------------------------
struct ThreadVm *__CreateStaticThread(struct EventType *evtt,void *Stack,
                                    u32 StackSize)
{
    struct ThreadVm  *result;

    result = (struct ThreadVm  *)align_up_sys(Stack);

    memset(Stack, 'd', StackSize-((ptu32_t)result - (ptu32_t)Stack));

    //��ʵ�ʷ����˶����ڴ棬djyos�ڴ����ʹ�ÿ��������ԣ����������ڴ�������
    //�����������Ա�֤��ʵ�ʳߴ��Ƕ���ġ�֮����ע�͵�������Ϊ��len����������ʱ��
    //����ֻ��ʵ�ʽ�����������ڴ����Ĺ涨�����������ע�͵�����Ҫ���ڴ����
    //ģ������ṩ����Ľ������ģ��������ǲ����ġ�
//    len = M_CheckSize(result);
    result->stack_top = align_down_sys((ptu32_t)Stack+StackSize); //ջ����ַ����ֲ����
    result->next = NULL;
    result->stack_size = (ptu32_t)(result->stack_top) - (ptu32_t)result
                            - sizeof(struct ThreadVm);       //����ջ���
    result->host_vm = NULL;
    //��λ�̲߳������߳�
    __asm_reset_thread(evtt->thread_routine,result);
    return result;
}

//----����ָ��ָ����ʱ����-----------------------------------------------------
//����: ����ָ����ʱ����,ʹ�����ú��ֱ������ͱ����Ż�ѡ��,djy_delay_us����׼ȷ��ʱ��
//��������
//����: ��
//��ע: ��������ֲ����
//-----------------------------------------------------------------------------
void __DjySetDelay(void)
{
    uint32_t counter,u32_fors=10000,clksum,timer_clk_hz;
    volatile uint32_t u32loops;

    clksum = 0xFFFF;  //�˴�����ȡ���������ֵ��S3Cxxxx Timer��16λ������
    timer_clk_hz =5000000; //Ӳ����ʱ��ʱ��Ϊ5MHZ.

    Int_CutLine(CN_INT_LINE_TIMER3); //��ֹTIMER3�ж�
    Timer_SetCounter(3,clksum);
    Timer_SetClkSource(3,0);  //pclkԤ��Ƶ����1/2��Ƶ
    Timer_SetPrecale(1,(CN_CFG_PCLK/2/timer_clk_hz));  //���ö�ʱ��pre_div
    Timer_SetType(3,0);  //���ζ�ʱ����
    Timer_Reload(3);
    Timer_Start(3);

    for(u32loops=u32_fors;u32loops>0;u32loops--);       //ѭ��u32_fors��
    Timer_Stop(3);

    //�˴����Խ׶���ȷ�������ݼ�����������0ʱ��˵��һ�������������޷��������ѭ��
    //���Сu32_forsֵ�������systick�жϷ����������ж��ٸ���������
    counter =Timer_Read(3);
    if(counter==0)
    {
        while(1);
    }

    counter = clksum - counter;    //ȡʵ����������
    g_u32CycleSpeed = ((uint64_t)counter * (u64)10E8)/timer_clk_hz/u32_fors;//���������64λ
    g_u32CycleSpeed = (g_u32CycleSpeed << 10) / 1000;     //����1.024��
    g_u32HundreUsfor = (1024<<7)/g_u32CycleSpeed;
}


u32 __DjyIsrTick(ptu32_t line)
{
    Djy_IsrTick(1);
    return 0;
}

//----��ʼ��tick---------------------------------------------------------------
//����: ��ʼ����ʱ��,������tick�жϺ���,������ʱ��.
//����: ��
//����: ��
//��ע: ����������ֲ���к���.
//-----------------------------------------------------------------------------
void __DjyInitTick(void)
{
    Int_Register(CN_INT_LINE_TIMER3);
    Int_CutLine(CN_INT_LINE_TIMER3);
    //����timer3��Ϊtick�ж�
    Int_IsrConnect(CN_INT_LINE_TIMER3,__DjyIsrTick);
    Int_SettoAsynSignal(CN_INT_LINE_TIMER3);          //tick�жϱ���Ϊ�첽�ź�
    //�������ö�ʱ������������board-config.h��CN_CFG_TICK_US��CN_CFG_TICK_HZ��
    //CN_CFG_FINE_US��CN_CFG_FINE_HZ�Ķ���һ��
    Timer_SetClkSource(3,0);          //pclkԤ��Ƶ����1/2��Ƶ
    Timer_SetPrecale(1,CN_CFG_TIMER_CLK/CN_CFG_FINE_HZ/2 -1);
    //���ϰѶ�ʱ������ʱ�ӵ�Ƶ����ΪCN_CFG_FINE_HZ
    //Timer_SetCounter(3,(uint16_t)((u32)CN_CFG_TICK_US*1000/CN_CFG_FINE_US));
    Timer_SetCounter(3, CN_CFG_FINE_HZ/CN_CFG_TICK_HZ);
    Timer_SetType(3,1);                //����tick��ʱ����������
    Timer_Reload(3);                    //���ض�ʱֵ
    Timer_Start(3);                     //������ʱ��
    Int_RestoreAsynLine(CN_INT_LINE_TIMER3);//����tick�ж�S
}

//----��ȡ��ǰʱ��(uS)---------------------------------------------------------
//���ܣ���ȡ��ǰʱ��(uS),�Ӽ������������������us����64λ��Ĭ�ϲ������
//      g_s64OsTicks Ϊ64λ��������64λϵͳ�У���ȡ g_s64OsTicks ��Ҫ����1��
//      ����,��Ҫʹ��ԭ�Ӳ�����
//��������
//���أ���ǰʱ��
//˵��: ����һ��׮����,��rtc.c�ļ��� DjyGetSysTime ��������
//-----------------------------------------------------------------------------
s64 __DjyGetSysTime(void)
{
    s64 time;
    static s64 BakTime = 0;
    u32 temp;
    atom_low_t atom_low;
    temp =CN_CFG_FINE_HZ/CN_CFG_TICK_HZ - pg_timer_reg->TCNTO3;

    atom_low = Int_LowAtomStart();
    time = g_s64OsTicks;
    Int_LowAtomEnd(atom_low);
    time = time*CN_CFG_TICK_US + (temp*CN_CFG_FINE_US >>16);
    if(time < BakTime)
        time += CN_CFG_TICK_US;
    BakTime = time;

    return time;
}

void reset(void)
{
    //wait for adding
}
extern void reboot(void)
{

}
extern void restart_system(void)
{

}

