//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

//所属模块: GDD
//作者:  LiuWei.
//版本：V1.0.0
//文件描述: 窗口定时器系统机制和管理
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2014-11-10
//   作者:  LiuWei.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include <stdint.h>
#include <djyos.h>
#include    "gdd_private.h"
#include <gui\gdd_timer.h>

static  list_t  sg_GddTimerList;
static struct MutexLCB     *s_ptGddTimerQSync =NULL;  //定时器队列锁
static struct SemaphoreLCB *s_ptGddTimerRun =NULL;

//----分配定时器内存-------------------------------------------------------------
//描述: 略
//参数：无
//返回：定时器内存指针
//------------------------------------------------------------------------------
static  struct WinTimer*  __GDD_TimerAlloc(void)
{
    struct WinTimer *tmr;

    tmr =malloc(sizeof(struct WinTimer));
    //todo:此处加空指针判断
    memset(tmr, 0, sizeof(struct WinTimer));
    return tmr;
}

//----释放定时器内存-------------------------------------------------------------
//描述: 释放一个由__gdd_TimerAlloc分配的定时器内存.
//参数：定时器内存指针
//返回：无
//------------------------------------------------------------------------------
static  void    __GDD_TimerFree(struct WinTimer *ptmr)
{
    free(ptmr);
}

//----锁定定时器----------------------------------------------------------------
//描述: 锁定定时器,用于对定时器互斥操作,该函数返回TRUE时,必须调用__gdd_TimerUnlock解锁;
//      而当该函数返回FALSE,则无需调用__gdd_TimerUnlock.
//参数：定时器对象指针
//返回：成功:TRUE; 失败:FLASE;
//------------------------------------------------------------------------------
bool_t    __GDD_TimerLock(struct WinTimer *ptmr)
{
    if(NULL == ptmr)
        return FALSE;
    if(__GDD_Lock())
        return TRUE;

    __GDD_Unlock();
    return  FALSE;
}

//----解锁定时器----------------------------------------------------------------
//描述: 当定时器锁定成功后,由该函数进行解锁操作.
//参数：定时器对象指针
//返回：无
//------------------------------------------------------------------------------
void    __GDD_TimerUnlock(struct WinTimer *ptmr)
{
    __GDD_Unlock();
}

// =============================================================================
// 函数功能：添加定时器到定时器队列，该队列是依Alarm时刻由近到远排列的。
// 输入参数：timer,待添加的定时器
// 输出参数：无
// 返回值  ：无
// 说明    ：
// =============================================================================
static void __GDD_TimerAdd(struct WinTimer *timer)
{
    list_t *tmp;
    struct WinTimer *t;
    //找到一个Alarm时间大于timer->Alarm的节点，timer就插入该节点前面。
    //如果找不到，则会插入sg_GddTimerList前面，也是正确的。
    //下一行没有  ;  号
    dListForEach(tmp, &sg_GddTimerList)
    {
        t = dListEntry(tmp,struct WinTimer,node_sys);
        if(t->Alarm >= timer->Alarm)
            break;
    }
    dListInsertBefore(tmp,&timer->node_sys);
}

//----GDD定时器模块初始化------------------------------------------------------
//描述: 该函数由GDD内部调用
//参数：无
//返回：无
//------------------------------------------------------------------------------
bool_t GDD_TimerInit(void)
{
    dListInit(&sg_GddTimerList);
    s_ptGddTimerQSync = Lock_MutexCreate("Gdd Timer");
    s_ptGddTimerRun = Lock_SempCreate(1, 0, CN_BLOCK_FIFO, "Gdd Timer");
    if( (s_ptGddTimerQSync == NULL) || (s_ptGddTimerRun == NULL))
    {
        Lock_MutexDelete(s_ptGddTimerQSync);
        Lock_SempDelete(s_ptGddTimerRun);
        return false;
    }
    else
        return true;
}

//----创建定时器----------------------------------------------------------------
//描述: 创建一个定时器，处于停止运行状态。
//参数：hwnd: 定时器所属窗口.定时到则给该窗口发定时器消息。
//      Id:   定时器Id.
//      Flag: 定时器标记.
//      IntervalMS: 定时间隔时间(毫秒数). 0表示暂停工作。
//返回：定时器对象指针
//------------------------------------------------------------------------------
struct WinTimer*  GDD_CreateTimer(HWND hwnd,u16 Id,u32 IntervalMS)
{
    struct WinTimer *ptmr=NULL;

    if((IntervalMS == 0)||(GDD_FindTimer(hwnd,Id)!=NULL))
        return ptmr;
    if(__HWND_Lock(hwnd))
    {
        ptmr =__GDD_TimerAlloc();;
        if(NULL!=ptmr)
        {
            dListInit(&ptmr->node_hwnd);
            dListInit(&ptmr->node_msg_timer);

            ptmr->hwnd     =hwnd;
            ptmr->Id       =Id;
//            ptmr->Flag     =Flag;
            ptmr->Interval =IntervalMS;
            ptmr->Alarm    = CN_LIMIT_SINT64;       //新创建的定时器是暂停运行的
//          ptmr->HoldTime =GUI_GetTickMS();

            dListInsertBefore(&hwnd->list_timer,&ptmr->node_hwnd);
            Lock_MutexPend(s_ptGddTimerQSync,CN_TIMEOUT_FOREVER);
            __GDD_TimerAdd(ptmr);
            Lock_MutexPost(s_ptGddTimerQSync);
        }
        __HWND_Unlock(hwnd);
    }
    return  ptmr;
}

//----查找定时器----------------------------------------------------------------
//描述: 按Id查找指定窗口的定时器
//参数：hwnd: 要查找的定时器所在窗口.
//      Id: 要查找的定时器Id
//返回：成功返回定时器对象指定,否则返回NULL
//------------------------------------------------------------------------------
struct WinTimer*  GDD_FindTimer(HWND hwnd,u16 Id)
{
    list_t *n;
    struct WinTimer *ptmr=NULL;
    if(hwnd==NULL)
        return ptmr;
    if(__GDD_Lock())
    {
        Lock_MutexPend(s_ptGddTimerQSync,CN_TIMEOUT_FOREVER);
        dListForEach(n, &hwnd->list_timer)
        {
            ptmr =(struct WinTimer*)dListEntry(n,struct WinTimer,node_hwnd);
            if(ptmr->Id==Id)
            {
                break;
            }
            ptmr=NULL;
        }
        Lock_MutexPost(s_ptGddTimerQSync);
        __GDD_Unlock();
    }
    return ptmr;
}

//----重置定时器----------------------------------------------------------------
//描述: 重新设置定时器参数，定时器立即进入运行状态
//参数：ptmr: 定时器对象.
//      IntervalMS: 定时间隔时间(毫秒数). 0表示暂停工作。
//返回：无
//------------------------------------------------------------------------------
void GDD_ResetTimer(struct WinTimer *ptmr,u32 IntervalMS)
{
    if(ptmr==NULL)
        return;
    Lock_MutexPend(s_ptGddTimerQSync,CN_TIMEOUT_FOREVER);
    if(IntervalMS != 0)
    {
        ptmr->Interval  =IntervalMS;
        ptmr->Alarm      = DJY_GetSysTime( ) + IntervalMS * 1000;
    }
    else
    {
        ptmr->Alarm      = CN_LIMIT_SINT64;     //“无限长”定时，表示暂停
    }
    dListRemove(&ptmr->node_sys);
    __GDD_TimerAdd(ptmr);
    if(&ptmr->node_sys == dListGetAfter(&sg_GddTimerList))
    {
        //如果是队列头，则立即释放信号量，使定时器扫描函数更新定时时间
        Lock_SempPost(s_ptGddTimerRun);
    }
    Lock_MutexPost(s_ptGddTimerQSync);
    return ;
}

//----停止定时器----------------------------------------------------------------
//描述: 停止一个定时器，实际上就是把闹到点时间设为“无限长”
//参数：ptmr: 定时器对象.
//返回：无
//------------------------------------------------------------------------------
void GDD_StopTimer(struct WinTimer *ptmr)
{
    if(ptmr==NULL)
        return;
    Lock_MutexPend(s_ptGddTimerQSync,CN_TIMEOUT_FOREVER);
    ptmr->Alarm      = CN_LIMIT_SINT64;     //“无限长”定时，表示暂停
    dListRemove(&ptmr->node_sys);
    __GDD_TimerAdd(ptmr);
    Lock_MutexPost(s_ptGddTimerQSync);
    return ;
}

//----启动定时器----------------------------------------------------------------
//描述: 启动一个定时器，如果定时器已经启动，定时时间重新计算
//参数：ptmr: 定时器对象.
//返回：无
//------------------------------------------------------------------------------
void GDD_StartTimer(struct WinTimer *ptmr)
{
    if(ptmr==NULL)
        return;
    Lock_MutexPend(s_ptGddTimerQSync,CN_TIMEOUT_FOREVER);
    ptmr->Alarm      = DJY_GetSysTime( ) + ptmr->Interval * 1000;
    dListRemove(&ptmr->node_sys);
//  dListRemove(&ptmr->node_msg_timer);
    __GDD_TimerAdd(ptmr);
    Lock_MutexPost(s_ptGddTimerQSync);
    //看新启动的定时器是否放在队列头，
    if(&ptmr->node_sys == dListGetAfter(&sg_GddTimerList))
    {
        //如果是队列头，则立即释放信号量，使定时器扫描函数更新定时时间
        Lock_SempPost(s_ptGddTimerRun);
    }
    return ;
}
//----删除定时器----------------------------------------------------------------
//描述: 删除一个定时器
//参数：ptmr: 定时器对象.
//返回：无
//------------------------------------------------------------------------------
void GDD_DeleteTimer(struct WinTimer*ptmr)
{
    if(ptmr==NULL)
        return;
    Lock_MutexPend(s_ptGddTimerQSync,CN_TIMEOUT_FOREVER);
    dListRemove(&ptmr->node_sys);
    Lock_MutexPost(s_ptGddTimerQSync);

    if(__HWND_Lock(ptmr->hwnd))
    {
        dListRemove(&ptmr->node_hwnd);
        dListRemove(&ptmr->node_msg_timer);
        __HWND_Unlock(ptmr->hwnd);
    }
    __GDD_TimerFree(ptmr);
    return;
}

//----删除窗口所有定时器-------------------------------------------------------
//描述: 该函数为GDD内部调用
//参数：hwnd: 窗口句柄.
//返回：无.
//------------------------------------------------------------------------------
void __GDD_RemoveWindowTimer(HWND hwnd)
{
    list_t *n;
    struct WinTimer *ptmr;

    n = (&hwnd->list_timer)->next;
    for (;n != (&hwnd->list_timer); n = (&hwnd->list_timer)->next)
    {
        ptmr =(struct WinTimer*)dListEntry(n,struct WinTimer,node_hwnd);

        Lock_MutexPend(s_ptGddTimerQSync,CN_TIMEOUT_FOREVER);
        dListRemove(&ptmr->node_sys);
        Lock_MutexPost(s_ptGddTimerQSync);

        if(__HWND_Lock(hwnd))
        {
            dListRemove(&ptmr->node_hwnd);
            dListRemove(&ptmr->node_msg_timer);
            __HWND_Unlock(hwnd);
        }
        __GDD_TimerFree(ptmr);
    }
}

//----窗口定时器扫描-----------------------------------------------------------
//功能：扫描窗口定时器队列，该队列是由 sg_GddTimerList索引按到达时间先后排序
//参数：
//返回：不返回
//------------------------------------------------------------------------------

ptu32_t GDD_TimerScan(void)
{
    list_t *tmp;
    struct WinTimer *timer;
    s64 NowTime;
    while(1)
    {
        if(dListIsEmpty(&sg_GddTimerList))
        {
            //定时器队列空，只有创建定时器才释放信号量
            Lock_SempPend(s_ptGddTimerRun, CN_TIMEOUT_FOREVER);
        }
        else
        {
            NowTime = DJY_GetSysTime( );
            Lock_MutexPend(s_ptGddTimerQSync,CN_TIMEOUT_FOREVER);
            tmp = (&sg_GddTimerList)->next;
            timer = dListEntry(tmp,struct WinTimer,node_sys);
            if(timer->Alarm > NowTime)
            {
                Lock_MutexPost(s_ptGddTimerQSync);
                //改为等待信号量，新增定时器时触发该信号量，队列头部的定时器定时时间到
                //则超时返回。
                Lock_SempPend( s_ptGddTimerRun, timer->Alarm - NowTime);
            }
            else
            {
                timer->Alarm += timer->Interval * mS;
                dListRemove(tmp);
                __GDD_TimerAdd(timer);
                Lock_MutexPost(s_ptGddTimerQSync);
                GDD_PostMessage(timer->hwnd, MSG_TIMER, (ptu32_t)timer->Id, (ptu32_t)timer);
            }
        }
    }
}




