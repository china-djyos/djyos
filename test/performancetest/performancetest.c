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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块：系统性能测试模块
//作者:
//版本：V1.0.0
//文件描述:
//其他说明:
//修订历史:
//
#include "systime.h"
#include "stdint.h"
#include "stdio.h"
#include "djyos.h"
#include "int.h"
#include "lock.h"
#include "atomic.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。
#include "stdlib.h"
#include "string.h"
#include "timer_hard.h"

#define CN_TEST_TIMES   3000    //测试次数
static u8 Cpu_Tatio[3] = {10,50,99};        //控制Cpu的占用率

u32 u32g_GetTimeCost;            //测量系统时间函数所消耗时间
u32 u32g_TimerFreq;
struct SemaphoreLCB* TestSemp;
struct MutexLCB* TestMutex;
u32 TimeRecordStart,TimeRecordEnd;
u32 TimeRecord[CN_TEST_TIMES+2];    //多测两次，首末两次不要
u32 TestOffset;
u32 TimeRecord2[CN_TEST_TIMES+2];   //多测两次，首末两次不要
u32 TestOffset2;
bool_t TestEnd;
ptu32_t TimerHandle;
u32 u32g_TimerFreq;
u32 IntLine;        //不要修改变量名，bsp中要用
void Test_ShowValue(u32 *data,u8 Ratio,u32 freq,char *info);
u32 __Test_TimerStart(void);
bool_t __Test_TimerEnd(u32 *val);
void Test_IntSyncHook(ucpu_t SchType);
void Test_HighPrioHook(ucpu_t SchType);
void Test_LowPrioHook(ucpu_t SchType);

//函数在BSP中实现，一般在CPU驱动目录中，参见 djysrc\bsp\cpudrv\stm32\src\testsupport.c
//重置并设置定时器启动标志。
__attribute__ ((weak)) u32 __Test_TimerStart(void)
{
    printf("请在BSP中实现定时相关函数\r\n");
    return 0;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
//函数在BSP中实现，一般在CPU驱动目录中，参见 djysrc\bsp\cpudrv\stm32\src\testsupport.c
//如果定时器处于启动状态，读取从上次调用 __Test_TimerStart 以来的时间间隔，并停止定时器。
__attribute__ ((weak)) bool_t __Test_TimerEnd(u32 *val)
{
    printf("请在BSP中实现定时相关函数\r\n");
    return false;
}

//函数在BSP中实现，一般在CPU驱动目录中，参见 djysrc\bsp\cpudrv\stm32\src\testsupport.c
//用于异步信号同步测试，定时器中断引起处于异步信号阻塞中的事件切入时，读出从中断发生
//到进入本函数的时间间隔，并调用Test_PushTimeHook保存
__attribute__ ((weak)) void Test_IntSyncHook(ucpu_t SchType)
{
    printf("请在BSP中实现定时相关函数\r\n");
    return;
}
#pragma GCC diagnostic pop

//在BSP中实现的定时器ISR函数中调用，把中断响应延迟时间存入
void Test_PushTimeISR(u32 time_counter)
{
    if(TestOffset < CN_TEST_TIMES+2)
    {
        TimeRecord[TestOffset++] = time_counter;
    }
}
//在BSP中实现的Test_IntSyncHook函数调用，把中断响应延迟时间存入
void Test_PushTimeHook(u32 time_counter)
{
    if(TestOffset2 < CN_TEST_TIMES+2)
    {
        TimeRecord2[TestOffset2++] = time_counter;
    }
}

//两个线程，优先级一高一低，高优先级线程会因请求不到互斥量而阻塞，进而切换到低优先级。
//低优先级释放互斥量后，会因自身优先级低被获得了互斥量而激活的高优先级线程抢占，从而发生切换。
ptu32_t Test_MutexHighPrioEvent(void)
{
    while(1)                  //设置时候应为2的整数倍
    {
        TimeRecordStart = __Test_TimerStart();
        Lock_MutexPend(TestMutex, CN_TIMEOUT_FOREVER); //肯定导致阻塞，切换到高优先级线程
        DJY_EventDelay(1000);
        Lock_MutexPost(TestMutex);   //释放会使高优先级线程得到互斥量，产生抢占
        if(TestEnd)
            break;
    }

    return 0;
}

ptu32_t Test_MutexLowPrioEvent(void)
{
    while(1)                  //设置时候应为2的整数倍
    {
        Lock_MutexPend(TestMutex, CN_TIMEOUT_FOREVER);
        DJY_EventDelay(1000);
        TimeRecordStart = __Test_TimerStart();
        Lock_MutexPost(TestMutex);   //释放会使高优先级线程得到互斥量，产生抢占
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
    printf(" ***************** pend/post互斥量导致事件切换测试 ***************** \r\n\n");
    //创建信号量
    TestMutex = Lock_MutexCreate(NULL);          //初始化信号量为0
    if(TestMutex == NULL)
    {
        printf("TestMutex Create Failed\r\n");
        return;
    }
    //注册线程1 优先级为30,该线程优先级高,且先弹出，必先运行
    evttID1 = DJY_EvttRegist(EN_CORRELATIVE,30,0,0,
                        Test_MutexHighPrioEvent,NULL,1000,NULL);

    if(evttID1 !=CN_EVTT_ID_INVALID)
    {
        DJY_RegisterHook(evttID1,Test_HighPrioHook);
        DJY_EventPop(evttID1,NULL,0,0,0,0);
    }
     //注册线程2,优先级为40,该线程优先级低,
    evttID2 = DJY_EvttRegist(EN_CORRELATIVE,40,0,0,
                        Test_MutexLowPrioEvent,NULL,1000,NULL);

    if(evttID2!=CN_EVTT_ID_INVALID)
    {
        DJY_RegisterHook(evttID2,Test_LowPrioHook);
        //测试的时候保证当期线程的最高优先级为该B线程,防调度的时候第一个获取得到Cpu执行权的不是该线程
        DJY_EventPop(evttID2,NULL,0,0,0,0);
    }
    DJY_SetEventPrio(DJY_GetMyEventId(), CN_PRIO_RRS);     //使被测试线程是高优先级线程
    for(loop = 0; loop < sizeof(Cpu_Tatio) / sizeof(u8); loop++)
    {
        Atom_Set32(&TestOffset,0);
        Atom_Set32(&TestOffset2,0);
        while(1)
        {
            DJY_DelayUs(Cpu_Tatio[loop]*100);
            DJY_EventDelay(10000 - Cpu_Tatio[loop]*100);
            if(TestOffset2>=CN_TEST_TIMES+2)         //测试完成则释放资源
            {
                break;
            }
        }
        Test_ShowValue(TimeRecord,Cpu_Tatio[loop],u32g_TimerFreq,"释放互斥量使阻塞中的事件激活切换速度");
        printf("\r\r\n");
        Test_ShowValue(TimeRecord2,Cpu_Tatio[loop],u32g_TimerFreq,"请求互斥量被阻塞导致切换速度");
        printf("\r\r\n");
    }
    TestEnd = true;
    DJY_EventDelay(2000);       //wait for release evtt
    DJY_EvttUnregist(evttID1);
    DJY_EvttUnregist(evttID2);
    Lock_MutexDelete(TestMutex);
}

// =============================================================================
// 函数功能:信号量A测试线程
//       优先级 2
// 输入参数：空
// 输出参数：
// 返回值  ：
// =============================================================================
ptu32_t Test_SempLowPrioEvent(void)
{
    while(1)
    {
        TimeRecordStart = __Test_TimerStart();
        //因Evtt先运行，Evtt因获取不到信号量切换到EvttA
        Lock_SempPost(TestSemp);//释放的时候会产生调度,此时会重新切换回B线程
        if(TestEnd)
            break;
        DJY_EventDelay(1000);
    }
    return 0;
}

// =============================================================================
// 函数功能:信号量B测试线程
//       优先级 1
// 输入参数：空
// 输出参数：
// 返回值  ：
// =============================================================================
ptu32_t Test_SempHighPrioEvent(void)
{
    while(1)
    {
        TimeRecordStart = __Test_TimerStart();
        //因Evtt先运行，Evtt因获取不到信号量切换到EvttA
        Lock_SempPend(TestSemp, CN_TIMEOUT_FOREVER);
        if(TestEnd)
            break;
        DJY_EventDelay(1000);
    }
    return 0;
}

//钩子函数都只是记录对应线程切入时候的时间
//线程B钩子函数,现统一把线程切入作为线程切换结束来获取时间,对应线程发生切入以及切出的时候会调用,若系统中
//运行的线程较多 调度比较复杂,应保证当前测试线程是系统的优先级最高的,不然测试结果无法预料
void Test_HighPrioHook(ucpu_t SchType)
{
    //切入的时候才获取时间,切出不做处理
    if(SchType == EN_SWITCH_IN)
    {
         //记录数据
         if(__Test_TimerEnd(&TimeRecordEnd))     //读取结束测试系统时间
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

//钩子函数都只是记录对应线程切入时候的时间
//线程A钩子函数,现统一把线程切入作为线程切换结束来获取时间,对应线程发生切入以及切出的时候会调用,若系统中
//运行的线程较多 调度比较复杂,应保证当前测试线程是系统的优先级最高的,不然测试结果无法预料
void Test_LowPrioHook(ucpu_t SchType)
{
    //切入的时候才获取时间,切出不做处理
    if(SchType == EN_SWITCH_IN)
    {
        //记录数据
        if(__Test_TimerEnd(&TimeRecordEnd))     //读取结束测试系统时间
        {
            if(TestOffset2 < CN_TEST_TIMES+2)
            {
                TimeRecord2[TestOffset2++] = TimeRecordEnd - TimeRecordStart - u32g_GetTimeCost;
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
    //第一次和最后一次记录的数据，可能有错，舍弃之。
    for(loop1 = 1; loop1 < CN_TEST_TIMES+1; loop1++)
    {
        if(data[loop1] < MinVal)
            MinVal = data[loop1];
        if(data[loop1] > MaxVal)
            MaxVal = data[loop1];
        AverageVal += data[loop1];
    }
//    AverageVal = AverageVal/9998;
    printf("%s测试结果如下(单位:ns):\r\n",info);
    if(Ratio != 0)
        printf("cpu占用率设置%d%%:\r\n",Ratio);
    printf("最大值:\t%.2f\r\n",(float)MaxVal*10E8/freq);
    printf("最小值:\t%.2f\r\n",(float)MinVal*10E8/freq);
    printf("平均值:\t%.2f\r\n",(float)AverageVal*10E8/freq/CN_TEST_TIMES);
    printf("测量%d次，仅打印前10次测量数据(单位:ns):\r\n", CN_TEST_TIMES);
    for(loop1 = 1; loop1 < 11; loop1++)
    {
        printf("%.2f\t",(float)data[loop1]*10E8/freq);
    }
    printf("\r\n");
}
// =============================================================================
// 函数功能:实时中断以及异步信号测试模块
//
// 参数说明：meaNUM  :测量的次数 DataNum
//        DataNum :终端打印输出数据数目
//        MeaType :测试类型 EN_REAL_INT 为实时中断  EN_SYNC_SINGLE 为异步信号
// 输出参数：
// 返回值  ：
// =============================================================================
void Test_SempTest(void)
{
    u16 evttID1;
    u16 evttID2;
    u32 loop;
    TestEnd = false;
    printf(" ***************** pend/post信号量导致事件切换测试 ***************** \r\n\n");
    //创建信号量
    TestSemp = Lock_SempCreate(1,0,0,NULL);          //初始化信号量为0
    if(TestSemp == NULL)
    {
        printf("m_Sem Create Failed\r\n");
        return;
    }
    //注册线程B 优先级为30,该线程优先级高,且先弹出，必先运行
    evttID1 = DJY_EvttRegist(EN_CORRELATIVE,30,0,0,
                        Test_SempHighPrioEvent,NULL,3000,NULL);

    if(evttID1 !=CN_EVTT_ID_INVALID)
    {
        //注册线程B的钩子函数:注意-只要该线程引起调度(切换/切入以及切出都会调用)
        DJY_RegisterHook(evttID1,Test_HighPrioHook);
        DJY_EventPop(evttID1,NULL,0,0,0,0);
    }
     //注册线程A,优先级为40,该线程优先级低,
    evttID2 = DJY_EvttRegist(EN_CORRELATIVE,40,0,0,
                        Test_SempLowPrioEvent,NULL,3000,NULL);

    if(evttID2!=CN_EVTT_ID_INVALID)
    {
        //线程B注册钩子函数,钩子函数中判断是切入还是切出，A->B 还是B->A
        DJY_RegisterHook(evttID2,Test_LowPrioHook);
        //测试的时候保证当期线程的最高优先级为该B线程,防调度的时候第一个获取得到Cpu执行权的不是该线程
        DJY_EventPop(evttID2,NULL,0,0,0,0);
    }

    for(loop = 0; loop < sizeof(Cpu_Tatio) / sizeof(u8); loop++)
    {
        Atom_Set32(&TestOffset,0);
        Atom_Set32(&TestOffset2,0);
        while(1)
        {
            DJY_DelayUs(Cpu_Tatio[loop]*100);
            DJY_EventDelay(10000 - Cpu_Tatio[loop]*100);
            if(TestOffset2>=CN_TEST_TIMES+2)         //测试完成则释放资源
            {
                break;
            }
        }
        Test_ShowValue(TimeRecord,Cpu_Tatio[loop],u32g_TimerFreq,"释放信号量使阻塞中的事件激活切换速度");
        printf("\r\n");
        Test_ShowValue(TimeRecord2,Cpu_Tatio[loop],u32g_TimerFreq,"请求信号量被阻塞导致切换速度");
        printf("\r\n");
    }
    TestEnd = true;
    DJY_EventDelay(2000);       //wait for release evtt
    DJY_EvttUnregist(evttID1);
    DJY_EvttUnregist(evttID2);
    Lock_SempDelete(TestSemp);
}

//实时中断 以及异步 信号标识
typedef enum{
    EN_REAL_INT     = true,             //实时中断
    EN_SYNC_INT  = false,            //异步信号
}EN_INT_FLAG;

// =============================================================================
// 模块说明:eventDelay eventDelayTo 导致事件切换 模块测试
//
// 参数说明：meaNUM  :测量的次数 DataNum
//        DataNum :终端打印输出数据数目
//        MeaType :测试类型 EN_REAL_INT 为实时中断  EN_SYNC_SINGLE 为异步信号
// 输出参数：
// 返回值  ：
// =============================================================================

//测量延时切换功能用的低优先级事件处理函数，是个永远就绪的事件
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
// 函数功能:eventDelay eventDelayTo 导致事件切换 模块测试
//函数形参  :
//      MeaNum   :   测量次数
//      OutPutNum:   控制台输出数据量
//返回值   : 无
// =============================================================================
void Test_EventDelay(void)
{
    u16 evttID;

    TestEnd = false;
    Atom_Set32(&TestOffset,0);
    //start to run low pro thread
    evttID = DJY_EvttRegist(EN_CORRELATIVE,249,0,0,
                            Test_DelayThreadLow,NULL,1000,NULL);
    //事件的两个参数暂设为0,如果用shell启动,可用来采集shell命令行参数
    if(evttID!=CN_EVTT_ID_INVALID)
    {
        DJY_EventPop(evttID,NULL,0,0,0,0);
        DJY_RegisterHook(evttID, Test_HighPrioHook);
    }
    while(1)
    {
        TimeRecordStart = __Test_TimerStart();
        DJY_EventDelay(1000);
        if(TestOffset>=CN_TEST_TIMES+2)         //测试完成则释放资源
        {
            break;
        }
    }
    Test_ShowValue(TimeRecord, 0, u32g_TimerFreq,"高优先级时间EventDelay导致切换到低优先级事件的切换时间");
    TestEnd = true;
    DJY_EvttUnregist(evttID);
}
//高优先级线程

ptu32_t Test_PopThreadHight(void)
{
    u32 ptimes = 1;
    while(1)
    {
        DJY_WaitEvttPop(DJY_GetMyEvttId(), &ptimes, CN_TIMEOUT_FOREVER);
        if(TestEnd)
            break;
    }
    return 0;
}

// =============================================================================
// 函数功能:低优先级弹出高优先级事件导致切换  SysMeaPopModuleInstall
//函数形参  :
//      MeaNum   :   测量次数
//      OutPutNum:   控制台输出数据量
//返回值   : 无
// =============================================================================
void Test_PopHighPrioEvent(void)
{
    u16 evttID;

    printf(" ***************** 弹出高优先级事件导致抢占速度测试 ***************** \r\n\n");
    TestEnd = false;
    Atom_Set32(&TestOffset,0);
    //start to run low pro thread
    evttID = DJY_EvttRegist(EN_CORRELATIVE,30,0,0,
                            Test_PopThreadHight,NULL,1000,NULL);
    if(evttID!=CN_EVTT_ID_INVALID)
    {
        DJY_EventPop(evttID,NULL,0,0,0,0);
        DJY_RegisterHook(evttID, Test_HighPrioHook);
    }
    while(1)
    {
        TimeRecordStart = __Test_TimerStart();
        DJY_EventPop(evttID,NULL,0,0,0,0);
        DJY_EventDelay(1000);
        if(TestOffset>=CN_TEST_TIMES+2)         //测试完成则释放资源
        {
            break;
        }
    }
    Test_ShowValue(TimeRecord, 0, u32g_TimerFreq,"弹出高优先级事件后切换到高优先级事件切换时间");
    TestEnd = true;
    DJY_EvttUnregist(evttID);
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
// 函数功能:中断同步模块测试  SysMeaIntSynModuleInstall
//函数形参  :
//      MeaNum   :   测量次数
//      OutPutNum:   控制台输出数据量
//      intLine  :   中断线-用于指定中断同步测试的定时器所在的中断线
//返回值   : 无
// =============================================================================
void Test_IntSync(void)
{
    u32 loop;
    u16 evttID;

    printf(" ***************** 中断同步事件响应速度测试 ***************** \r\n\n");
    TestEnd = false;
    Atom_Set32(&TestOffset,0);
    Atom_Set32(&TestOffset2,0);

    evttID = DJY_EvttRegist(EN_CORRELATIVE,30,0,0,
                            Test_IntSyncThread,NULL,2000,NULL);
    if(evttID!=CN_EVTT_ID_INVALID)
    {
        DJY_EventPop(evttID,NULL,0,0,0,0);
        DJY_RegisterHook(evttID, Test_IntSyncHook);
    }

    for(loop = 0; loop < sizeof(Cpu_Tatio) / sizeof(u8); loop++)
    {
        Atom_Set32(&TestOffset,0);
        Atom_Set32(&TestOffset2,0);
        while(1)
        {
            DJY_DelayUs(Cpu_Tatio[loop]*100);
            DJY_EventDelay(10000 - Cpu_Tatio[loop]*100);
            if(TestOffset2>=CN_TEST_TIMES+2)         //测试完成则释放资源
            {
                break;
            }
        }
        Test_ShowValue(TimeRecord2, Cpu_Tatio[loop],u32g_TimerFreq,"异步信号同步响应时间");

        printf("\r\n");
    }
    TestEnd = true;
    DJY_EventDelay(2000);
    DJY_EvttUnregist(evttID);
}

ptu32_t SysMeaNoSwiPthread(void)
{
    return 0;
}

// =============================================================================
// 函数功能:信号量、互斥量、POP 引起调度，但无测试 切换模块
//函数形参  :
//      MeaNum   :   测量次数
//      OutPutNum:   控制台输出数据量
//返回值   : 无
// =============================================================================
void Test_coreFunc(void)
{
    u16 evttID;
    u32 loop;

    printf(" ***************** 核心功能函数执行时间测试 ***************** \r\n\n");
    evttID = DJY_EvttRegist(EN_CORRELATIVE,249,0,0,
                    SysMeaNoSwiPthread,NULL,2000,NULL);

    if(evttID!=CN_EVTT_ID_INVALID)
    {
        DJY_EventPop(evttID,NULL,0,0,0,0);
    }
    //测试Djy_EventPop执行时间，但不创建事件控制块
    for(loop = 0; loop < CN_TEST_TIMES+2; loop++)
    {
        TimeRecordStart = __Test_TimerStart();
        DJY_EventPop(evttID,NULL,0,0,0,0);
        __Test_TimerEnd(&TimeRecordEnd);
        TimeRecord[loop] = TimeRecordEnd - TimeRecordStart - u32g_GetTimeCost;
    }
    Test_ShowValue(TimeRecord, 0, u32g_TimerFreq,
                    "Djy_EventPop执行时间（不创建新事件）");
    printf("\r\n");
    //测试Djy_EventPop执行时间，需要创建事件控制块
    for(loop = 0; loop < CN_TEST_TIMES+2; loop++)
    {
        DJY_EventDelay(1000);   //将引发新弹出的事件处理完毕，使下一次弹出须创建事件控制块
        TimeRecordStart = __Test_TimerStart();
        DJY_EventPop(evttID,NULL,0,0,0,0);
        __Test_TimerEnd(&TimeRecordEnd);
        TimeRecord[loop] = TimeRecordEnd - TimeRecordStart - u32g_GetTimeCost;
    }
    Test_ShowValue(TimeRecord, 0, u32g_TimerFreq,
                    "Djy_EventPop执行时间（创建新事件）");
    printf("\r\n");

    //测试 Lock_SempPend 执行时间
    //初始化100000个信号，使 Lock_SempPend 总是能成功
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
    Test_ShowValue(TimeRecord, 0, u32g_TimerFreq,"Lock_SempPend 不引发事件切换执行时间");
    printf("\r\n");
    Test_ShowValue(TimeRecord2, 0, u32g_TimerFreq,"Lock_SempPost 不引发事件切换执行时间");
    printf("\r\n");
    Lock_SempDelete(TestSemp);

    //测试 Lock_MutexPend 执行时间
    //初始化100000个信号，使 Lock_SempPend 总是能成功
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
    Test_ShowValue(TimeRecord, 0, u32g_TimerFreq,"Lock_MutexPend 不引发事件切换执行时间");
    printf("\r\n");
    Test_ShowValue(TimeRecord2, 0, u32g_TimerFreq,"Lock_MutexPost 不引发事件切换执行时间");
    printf("\r\n");
    Lock_MutexDelete(TestMutex);

}

// =============================================================================
// 函数功能:实时中断以及异步信号测试模块
//
// 参数说明：meaNUM  :测量的次数 DataNum
//        DataNum :终端打印输出数据数目
//        MeaType :测试类型 EN_REAL_INT 为实时中断  EN_SYNC_SINGLE 为异步信号
// 输出参数：
// 返回值  ：
// =============================================================================
void Test_IntISRTest(EN_INT_FLAG MeaType)
{
    u32 loop;
    TestEnd = false;

    HardTimer_Ctrl(TimerHandle,EN_TIMER_SETINTPRO,MeaType);    //设置实时信号
    for(loop = 0; loop < sizeof(Cpu_Tatio) / sizeof(u8); loop++)
    {
        Atom_Set32(&TestOffset,0);
        while(1)
        {
            DJY_DelayUs(Cpu_Tatio[loop]*100);
            DJY_EventDelay(10000 - Cpu_Tatio[loop]*100);
            if(TestOffset>=CN_TEST_TIMES+2)         //测试完成则释放资源
            {
                break;
            }
        }
        if(MeaType == EN_REAL_INT)
            Test_ShowValue(TimeRecord, Cpu_Tatio[loop],u32g_TimerFreq,"实时中断ISR响应时间");
        else
            Test_ShowValue(TimeRecord, Cpu_Tatio[loop],u32g_TimerFreq,"异步信号ISR响应时间");

        printf("\r\n");
    }
    TestEnd = true;
}

extern void *TimerReg;      //用于BSP中的硬件操作
extern bool_t TimerStarted; //bsp中定义的
void SysPerformTest(void)
{
    u32 RunTime,RunTime1;

    //初始化、并分配定时器
    TimerHandle = HardTimer_Alloc(NULL);
    HardTimer_Ctrl(TimerHandle, EN_TIMER_GETREG, (ptu32_t)&TimerReg);
    HardTimer_Ctrl(TimerHandle,EN_TIMER_GETID,(ptu32_t)&IntLine);   //取中断号
    IntLine = IntLine>>16;
    u32g_TimerFreq = HardTimer_GetFreq(TimerHandle);   //获取定时器频率
    HardTimer_Ctrl(TimerHandle,EN_TIMER_SETCYCLE,u32g_TimerFreq/10000);  //中断周期=0.1mS
    //HardTimer_Ctrl(TimerHandle,EN_TIMER_ENINT,true);           //中断使能
    HardTimer_Ctrl(TimerHandle,EN_TIMER_STARTCOUNT,0);         //开始计数
    printf("<<<<<<<<<<<<<<<<<系统性能测试开始：>>>>>>>>>>>>>>>\r\n\n");
    TimerStarted = true;
    __Test_TimerEnd(&RunTime);
    TimerStarted = true;    //不能使用 __Test_TimerStart 函数
    __Test_TimerEnd(&RunTime1);
    u32g_GetTimeCost = RunTime1 - RunTime;  //测量 __Test_TimerEnd 函数本身的时间开销
    HardTimer_Ctrl(TimerHandle,EN_TIMER_PAUSECOUNT,0);         //开始计数
    HardTimer_Ctrl(TimerHandle,EN_TIMER_STARTCOUNT,0);
    HardTimer_Ctrl(TimerHandle,EN_TIMER_ENINT,true);           //中断使能
    printf("测试的板件名为:%s\r\n",DJY_BOARD);
    printf("测试主频为:%fMhz\r\n",CFG_CORE_MCLK);

//---------------与中断相关功能先测，然后会把定时器用于计时-----------------
    //实时中断响应时间测试
    printf(" ***************** 实时中断ISR响应时间测试 ***************** \r\n\n");
    Test_IntISRTest(EN_REAL_INT);
    //实时中断响应时间测试
    printf(" ***************** 异步信号ISR响应时间测试 ***************** \r\n\n");
    Test_IntISRTest(EN_SYNC_INT);

    DJY_EventDelay(500*mS);

    //异步信号同步引发切换测试
    Test_IntSync();

    DJY_EventDelay(500*mS);

//---------------与中断相关功能测量完毕，定时器用于计时-----------------
    HardTimer_Ctrl(TimerHandle,EN_TIMER_SETCYCLE,0xffffffff);
    //信号量引发切换的切换速度测试
    Test_SempTest();

    DJY_EventDelay(500*mS);

    //互斥量引发切换的切换速度测试
    Test_MutexTest();

    DJY_EventDelay(500*mS);

    //测量核心功能函数的执行时间
    Test_coreFunc();

    DJY_EventDelay(500*mS);

    //弹出高优先级事件导致切换时间测试
    Test_PopHighPrioEvent();

    DJY_EventDelay(500*mS);

    //eventdelay导致切换的切换时间测试
    Test_EventDelay();

    DJY_EventDelay(500*mS);

    /*定点运算DMIPS测试、 */
    extern void DhryStoneTest(void);
    DhryStoneTest();
    /*浮点运算运算FMIPS测试、 */
    extern int WhetStoneTest(void);
    WhetStoneTest();

    printf("System perform test had finished\r\n");
    HardTimer_Free(TimerHandle);
    return ;
}



