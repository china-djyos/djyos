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
//����ģ��:����ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:�ں˵�shell���
//����˵��:
//�޶���ʷ:
//1. ����: 2016-04-26
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include "stdint.h"
#include "board-config.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "object.h"
#include "lock.h"
#include "int.h"
#include "systime.h"
#include "djyos.h"
#include "shell.h"
#include "wdt_soft.h"
#include "blackbox.h"
#include "dbug.h"
#include "component_config_core.h"

extern struct EventECB g_tECB_Table[];
extern struct EventType g_tEvttTable[];
extern struct EventECB  *s_ptEventFree; //��������ͷ,������

//void __M_ShowHeap(void);
//void __M_ShowHeapSpy(void);
bool_t event(char *param);
bool_t evtt(char *param);
bool_t stack(char *param);
bool_t spyk(u16 pl_ecb);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ============================================================================
// ���ܣ���ʾ�¼��б�
// ��������
// ���أ�
// ��ע��
// ============================================================================
bool_t event(char *param)
{
    u16 pl_ecb;
    u32 time1,MemSize,StackSize,heapsize=0,maxsize=0;
    char *name;
    bool_t native = false;

    MemSize = 0;
    printf("�¼���  ���ͺ�  ���ȼ� CPU  ջ�ߴ�   ��̬�ڴ� ����ڴ�   ����������ʱ��(us)");
    for(pl_ecb = 0; pl_ecb < CFG_EVENT_LIMIT; pl_ecb++)
    {
        if(g_tECB_Table[pl_ecb].previous !=
                        (struct EventECB*)&s_ptEventFree)
        {
#if CFG_OS_TINY == false
            time1 = g_tECB_Table[pl_ecb].consumed_time_second/10000;
            name = g_tEvttTable[g_tECB_Table[pl_ecb].evtt_id&(~CN_EVTT_ID_MASK)].evtt_name;
#else
            time1 = 0;
            name = "unkown";
#endif  //CFG_OS_TINY == false
            if(NULL == g_tECB_Table[pl_ecb].vm)
            {
                StackSize = 0;
            }
            else
            {
                StackSize = g_tECB_Table[pl_ecb].vm->stack_size;
            }

            printf("\r\n%05d   %05d   %03d    %02d%%  %08x %08x %08x %s : %lld",\
                   pl_ecb,g_tECB_Table[pl_ecb].evtt_id &(~CN_EVTT_ID_MASK),\
                                    g_tECB_Table[pl_ecb].prio,time1,\
                                    StackSize, g_tECB_Table[pl_ecb].HeapSize,
                                    g_tECB_Table[pl_ecb].HeapSizeMax, name,
                                    g_tECB_Table[pl_ecb].consumed_time);
            heapsize += g_tECB_Table[pl_ecb].HeapSize;
            maxsize += g_tECB_Table[pl_ecb].HeapSizeMax;
            MemSize += StackSize;
            if(g_tECB_Table[pl_ecb].HeapSize > 0x80000000)
                native = true;
        }
        else
        {
        }
    }

    printf("\r\n�����¼�ջ�ߴ��ܼ�:         %08x %08x %08x", MemSize,heapsize,maxsize);
    printf("\r\n������¼�����:             %d", CFG_EVENT_LIMIT);
    if(native == true)
        printf("\r\n��ܰ��ʾ�������¼�ʹ�õĶ�̬�ڴ�ͳ�Ƴ��ַǳ����ֵ���벻Ҫ���ţ�˵�����Ĵ����д���"
               "\r\n�ǶԳ��ڴ�������ͷŵ��������A�¼�������ڴ棬��B�¼��ͷ�");
    return true;
}
bool_t eventk(char *param)
{
    u16 pl_ecb,busy = CN_EVENT_ID_INVALID;
    u32 time1,MemSize,StackSize,heapsize=0,maxsize=0;
    char *name;
    bool_t native = false;

    MemSize = 0;
    printk("�¼���  ���ͺ�  ���ȼ� CPU  ջ�ߴ�   ��̬�ڴ� ����ڴ�   ����������ʱ��(us)");
    for(pl_ecb = 0; pl_ecb < CFG_EVENT_LIMIT; pl_ecb++)
    {
        if(g_tECB_Table[pl_ecb].previous !=
                        (struct EventECB*)&s_ptEventFree)
        {
#if CFG_OS_TINY == false
            time1 = g_tECB_Table[pl_ecb].consumed_time_second/10000;
            name = g_tEvttTable[g_tECB_Table[pl_ecb].evtt_id&(~CN_EVTT_ID_MASK)].evtt_name;
#else
            time1 = 0;
            name = "unkown";
#endif  //CFG_OS_TINY == false
            if(time1 >50)
                busy = pl_ecb;
            if(NULL == g_tECB_Table[pl_ecb].vm)
            {
                StackSize = 0;
            }
            else
            {
                StackSize = g_tECB_Table[pl_ecb].vm->stack_size;
            }

            printk("\r\n%05d   %05d   %03d    %02d%%  %08x %08x %08x %s : %lld",\
                   pl_ecb,g_tECB_Table[pl_ecb].evtt_id &(~CN_EVTT_ID_MASK),\
                    g_tECB_Table[pl_ecb].prio,time1,\
                    StackSize, g_tECB_Table[pl_ecb].HeapSize,
                    g_tECB_Table[pl_ecb].HeapSizeMax, name,
                    g_tECB_Table[pl_ecb].consumed_time);
            heapsize += g_tECB_Table[pl_ecb].HeapSize;
            maxsize += g_tECB_Table[pl_ecb].HeapSizeMax;
            MemSize += StackSize;
            if(g_tECB_Table[pl_ecb].HeapSize > 0x80000000)
                native = true;
        }
        else
        {
        }
    }

    printk("\r\n�����¼�ջ�ߴ��ܼ�:         %08x %08x %08x", MemSize,heapsize,maxsize);
    printk("\r\n������¼�����:             %d", CFG_EVENT_LIMIT);
    if(native == true)
        printk("\r\n��ܰ��ʾ�������¼�ʹ�õĶ�̬�ڴ�ͳ�Ƴ��ַǳ����ֵ���벻Ҫ���ţ�˵�����Ĵ����д���"
               "\r\n�ǶԳ��ڴ�������ͷŵ��������A�¼�������ڴ棬��B�¼��ͷ�");
    if(busy == CN_EVENT_ID_INVALID)
        printk("��ǰû���¼�ռ��CPU����50%%");
    else
        spyk(busy);
    return true;
}

// ============================================================================
// ���ܣ���ʾ�¼������б�
// ��������
// ���أ�
// ��ע��
// ============================================================================
//bool_t Sh_ShowEvtt(char *param)
bool_t evtt(char *param)
{
    u16 pl_ecb;
    u32 MemSize;
    char *name;

    MemSize = 0;
    printf("���ͺ�  ���ȼ� ������  ջ����   ����\r\n");
    for(pl_ecb = 0; pl_ecb < CFG_EVENT_TYPE_LIMIT; pl_ecb++)
    {
        if(g_tEvttTable[pl_ecb].property.registered ==1)
        {
#if CFG_OS_TINY == false
            name = g_tEvttTable[pl_ecb].evtt_name;
#else
            name = "unkown";
#endif  //CFG_OS_TINY == 0
            MemSize += g_tEvttTable[pl_ecb].stack_size;
            printf("%05d   %03d    %08x  %08x %s",pl_ecb,\
                    g_tEvttTable[pl_ecb].default_prio,\
                    (ptu32_t)g_tEvttTable[pl_ecb].thread_routine,\
                    g_tEvttTable[pl_ecb].stack_size,\
                    name);
//            printf("knlshell","%03d    ",g_tEvttTable[pl_ecb].default_prio);
//            printf("knlshell","%08x  %08x ",
//                        (ptu32_t)g_tEvttTable[pl_ecb].thread_routine,
//                        g_tEvttTable[pl_ecb].stack_size);
//            printf("knlshell","%s",name);
            printf("\r\n");
        }
        else
        {
//          printf("%05d   ����",pl_ecb);
        }
    }

    printf("��������ջ�����ܼ�:      %08x",MemSize);
    printf("\r\n������¼���������:      %d",CFG_EVENT_TYPE_LIMIT);
    return (TRUE);
}

// ============================================================================
// ���ܣ���ʾϵͳ�������Ѿ������̵߳��¼���ջ��Ϣ
// ��������
// ���أ�
// ��ע��
// ============================================================================
//bool_t Sh_ShowStack(char *param)
bool_t stack(char *param)
{
    u32 pl_ecb;
    u32 loop,StackSize,pads;
    u32 *stack;
    char *name;

    printf("\r\n�¼��� �߳�   ջ��     ջָ��   ջ�ߴ�   ʣ����   ������");
    for(pl_ecb = 0; pl_ecb < CFG_EVENT_LIMIT; pl_ecb++)
    {

        if(g_tECB_Table[pl_ecb].previous !=
                        (struct EventECB*)&s_ptEventFree)
        {
            printf("\n\r");
            printf("%05d  ",pl_ecb);
            if(g_tECB_Table[pl_ecb].vm)
                printf("�ѷ��� ");
            else
            {
                printf("δ���� ");
                continue ;
            }
            StackSize = g_tECB_Table[pl_ecb].vm->stack_size;
            pads = 0x64646464;
            stack = (u32*)(&(g_tECB_Table[pl_ecb].vm[1]));
            for(loop = 0; loop < (StackSize>>2)-1; loop++)
            {
                if(stack[loop] != pads)
                {
                    break;
                }
            }
#if CFG_OS_TINY == false
            name = g_tEvttTable[g_tECB_Table[pl_ecb].evtt_id&(~CN_EVTT_ID_MASK)].evtt_name;
#else
            name = "unkown";
#endif  //CFG_OS_TINY == false
            printf("%08x %08x %08x %08x %s",
                        (ptu32_t)(&g_tECB_Table[pl_ecb].vm[1]),
                        (ptu32_t)(g_tECB_Table[pl_ecb].vm->stack),
                        StackSize,(loop<<2),
                        name);
        }
        else
        {
//            printf("knlshell","%05d  �����¼����ƿ�",pl_ecb);
        }
    }

    printf("\n\rջָ�������һ���������л�ʱ�����ֵ");
    return (TRUE);
}
u32 knlYipHook(struct Wdt *wdt)
{
    printk("--------idle wdt yit--------");
    eventk(NULL);
    return 0;
}
#pragma GCC diagnostic pop

// ============================================================================
// ���ܣ�ͳ��һ�����¼�����ռ����
// �������ޣ�
// ���أ��ޣ�
// ��ע��
// ============================================================================
ptu32_t kernel_spy(void)
{
#if CFG_OS_TINY == false
    u32 pl_ecb;
    u32 cycle;
    struct Wdt *wdt;
    DJY_GetEventPara((ptu32_t*)(&cycle), NULL);
    cycle *=mS;
#if(CFG_IDLE_MONITOR_CYCLE > 0)
#if(CFG_IDLE_WDT_RESET == false)
    wdt = Wdt_Create("runtime watch", cycle * CFG_IDLE_MONITOR_CYCLE, knlYipHook, EN_BLACKBOX_DEAL_IGNORE, 0, 0);
#else
    wdt = Wdt_Create("runtime watch", cycle * CFG_IDLE_MONITOR_CYCLE, NULL, EN_BLACKBOX_DEAL_RESET, 0, 0);
#endif  //for (CFG_IDLE_WDT_RESET != 1)
#endif  //for (CFG_IDLE_MONITOR_CYCLE > 0)
    while(1)
    {

        for(pl_ecb = 0; pl_ecb < CFG_EVENT_LIMIT; pl_ecb++)
        {
           g_tECB_Table[pl_ecb].consumed_time_second =
                              (u32)g_tECB_Table[pl_ecb].consumed_time
                            - g_tECB_Table[pl_ecb].consumed_time_record;
           g_tECB_Table[pl_ecb].consumed_time_record =
                            (u32)g_tECB_Table[pl_ecb].consumed_time;
        }
#if(CFG_IDLE_MONITOR_CYCLE > 0)
        //���idle�¼�����ʱ�䳬�� 1/16����ι��
        if(g_tECB_Table[0].consumed_time_second > (cycle >> 4))
        {
            Wdt_Clean(wdt);
        }
#endif  //for (CFG_IDLE_MONITOR_CYCLE > 0)
        DJY_EventDelay(cycle); // ��ʱ1�룻
    }
#endif
}

// ============================================================================
// ���ܣ�kernel shell����ĵ���ģʽ
// ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 kernel_command(void)
{
    u16 res;

    u32 cycle = 1000; // 1sʱ�䣬�������


    res = DJY_EvttRegist(EN_CORRELATIVE, 1, 0, 0,
                        kernel_spy, NULL, 1024, "kernel spy");
    if(res==CN_EVTT_ID_INVALID)
        return(-1);

    DJY_EventPop(res, NULL, 0, (ptu32_t)cycle, 0, 0);
    return (0);
}

bool_t spy(char *param)
{
    u16 pl_ecb;
    u32 loop,StackSize,pads;
    u32 *stack;
    char *name;

    printf("\r\n�¼��� �߳�   ջ��     ջָ��   ջ�ߴ�   ʣ����   ������");
    pl_ecb = atoi(param);
    if(g_tECB_Table[pl_ecb].previous !=
                    (struct EventECB*)&s_ptEventFree)
    {
        printf("\n\r");
        printf("%05d  ",pl_ecb);
        if(g_tECB_Table[pl_ecb].vm)
            printf("�ѷ��� ");
        else
        {
            printf("δ���� ");
            return true ;
        }
        StackSize = g_tECB_Table[pl_ecb].vm->stack_size;
        pads = 0x64646464;
        stack = (u32*)(&(g_tECB_Table[pl_ecb].vm[1]));
        for(loop = 0; loop < (StackSize>>2)-1; loop++)
        {
            if(stack[loop] != pads)
            {
                break;
            }
        }
#if CFG_OS_TINY == false
        name = g_tEvttTable[g_tECB_Table[pl_ecb].evtt_id&(~CN_EVTT_ID_MASK)].evtt_name;
#else
        name = "unkown";
#endif  //CFG_OS_TINY == false
        printf("%08x %08x %08x %08x %s",
                    (ptu32_t)(&g_tECB_Table[pl_ecb].vm[1]),
                    (ptu32_t)(g_tECB_Table[pl_ecb].vm->stack),
                    StackSize,(loop<<2),name);
        stack = g_tECB_Table[pl_ecb].vm->stack;
        printf("\n\r%x: ",(u32)stack);
        for(;stack < g_tECB_Table[pl_ecb].vm->stack_top; stack++)
        {
            if((u32)stack % 32 == 0)
                printf("\n\r%x: ",(u32)stack);
            printf(" %08x",*stack);
        }
    }
    else
    {
        printf("%05d  �����¼����ƿ�",pl_ecb);
    }
    return (TRUE);
}
bool_t spyk(u16 pl_ecb)
{
    u32 loop,StackSize,pads;
    u32 *stack;
    char *name;

    printk("\r\n�¼��� �߳�   ջ��     ջָ��   ջ�ߴ�   ʣ����   ������");
    if(g_tECB_Table[pl_ecb].previous !=
                    (struct EventECB*)&s_ptEventFree)
    {
        printk("\n\r");
        printk("%05d  ",pl_ecb);
        if(g_tECB_Table[pl_ecb].vm)
            printk("�ѷ��� ");
        else
        {
            printk("δ���� ");
            return true ;
        }
        StackSize = g_tECB_Table[pl_ecb].vm->stack_size;
        pads = 0x64646464;
        stack = (u32*)(&(g_tECB_Table[pl_ecb].vm[1]));
        for(loop = 0; loop < (StackSize>>2)-1; loop++)
        {
            if(stack[loop] != pads)
            {
                break;
            }
        }
#if CFG_OS_TINY == false
        name = g_tEvttTable[g_tECB_Table[pl_ecb].evtt_id&(~CN_EVTT_ID_MASK)].evtt_name;
#else
        name = "unkown";
#endif  //CFG_OS_TINY == false
        printk("%08x %08x %08x %08x %s",
                    (ptu32_t)(&g_tECB_Table[pl_ecb].vm[1]),
                    (ptu32_t)(g_tECB_Table[pl_ecb].vm->stack),
                    StackSize,(loop<<2),name);
        stack = g_tECB_Table[pl_ecb].vm->stack;
        printk("\n\r%x: ",(u32)stack);
        for(;stack < g_tECB_Table[pl_ecb].vm->stack_top; stack++)
        {
            if((u32)stack % 32 == 0)
                printk("\n\r%x: ",(u32)stack);
            printk(" %08x",*stack);
        }
    }
    else
    {
        printk("%05d  �����¼����ƿ�",pl_ecb);
    }
    return (TRUE);
}

ADD_TO_ROUTINE_SHELL(spy,spy,"");
ADD_TO_ROUTINE_SHELL(stack,stack,"��ʾϵͳ�������Ѿ������̵߳��¼���ջ��Ϣ");
ADD_TO_ROUTINE_SHELL(event,event,"��ʾ�¼���");
ADD_TO_ROUTINE_SHELL(evtt,evtt,"��ʾ�¼����ͱ�");

