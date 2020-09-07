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
//文件描述: 消息系统机制实现
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2014-11-10
//   作者:  LiuWei.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include    "gdd_private.h"
#include <gkernel.h>
#include <align.h>
#include <shell.h>

/*============================================================================*/

struct MsgList
{
    struct WindowMsg Msg;
    struct MsgList *Next;
};

struct WinMsgQueueCB
{
    struct MutexLCB *mutex_lock;            //消息队列锁
    struct SemaphoreLCB   *sem_msg;         //消息信号量
    struct SemaphoreLCB   *sem_sync_send;   //同步消息发送信号量
//  struct SemaphoreLCB   *sem_sync_recv;   //同步消息接收信号量

    //同步和刷新显示消息，一个主窗口只有一条，直接定义。
    struct WindowMsg     sync_msg;          //同步消息
    struct WindowMsg     refresh_msg;       //刷新显示消息，必须在最后，以确保所有
                                            //绘制消息都执行了才刷新显示
//  struct WindowMsg     quit_msg;          //退出消息
    //以下四种消息，因每个窗口（及子窗口）只有一条，故单独成链表。
    list_t  list_msg_close;                 //CLOSE消息链表
    list_t  list_msg_ncpaint;               //NCPAINT消息链表
    list_t  list_msg_paint;                 //PAINT消息链表
    list_t  list_msg_timer;                 //TIMER消息链表

    //Post类型消息数据
    void    *post_pbuf;         //记录原始的post消息缓冲区首址(用于动态内存释放)
    struct MsgList *post_free;     //指向空闲的post消息缓冲链表
    struct MsgList *post_first;    //指向第一个post消息链表
    struct MsgList *post_last;     //指向最后一个post消息链表节点
};

//----复制一条消息--------------------------------------------------------------
//描述: 该函数为内部调用,不检查函数参数的合法性.
//参数：dst: 目的消息指针.
//      src: 源消息指针.
//返回：无.
//------------------------------------------------------------------------------
static  void __GDD_CopyMsg(struct WindowMsg *dst,const struct WindowMsg* src)
{
//  dst->hwnd   =src->hwnd;
//  dst->Code   =src->Code;
//  dst->Param1 =src->Param1;
//  dst->Param2 =src->Param2;
    *dst = *src;
    dst->ExData =NULL;
}

//----初始化一条消息-----------------------------------------------------------
//描述: 该函数为内部调用,不检查函数参数的合法性.
//参数：msg: 消息结构体指针.
//     hwnd: 消息窗口句柄.
//     code: 消息代码号.
//     param1: 消息参数1.
//     param2: 消息参数2.
//返回：无.
//-----------------------------------------------------------------------------
void __GDD_InitMsg(struct WindowMsg *msg,HWND hwnd,u32 code,u32 param1,ptu32_t param2)
{
    msg->hwnd   =hwnd;
    msg->Code   =code;
    msg->Param1 =param1;
    msg->Param2 =param2;
    //msg->Time     =WDD_GetTickMS();
    msg->ExData =NULL;
}

//----创建一个消息队列---------------------------------------------------------
//描述: 该函数为内部调用.
//参数：size: 异步消息缓冲区数量.
//返回：消息队列指针.
//------------------------------------------------------------------------------
struct WinMsgQueueCB*   __GUI_CreateMsgQ(s32 size)
{
    struct WinMsgQueueCB *pMsgQ=NULL;
    s32 i;

    pMsgQ =(struct WinMsgQueueCB*)malloc(sizeof(struct WinMsgQueueCB));
    if(NULL==pMsgQ)
    {
        return NULL;
    }
    memset(pMsgQ, 0, sizeof(struct WinMsgQueueCB));

    //创建消息队列锁
    pMsgQ->mutex_lock =Lock_MutexCreate(NULL);

    //创建消息信号量
//  pMsgQ->sem_msg  =Lock_SempCreate(1000,0,CN_BLOCK_PRIO,NULL);
    pMsgQ->sem_msg  =Lock_SempCreate(1,0,CN_BLOCK_PRIO,NULL);

//    pMsgQ->sem_sync_send =Lock_SempCreate(1,1,CN_BLOCK_PRIO,NULL);
    pMsgQ->sem_sync_send =Lock_SempCreate(1,0,CN_BLOCK_PRIO,NULL);
//  pMsgQ->sem_sync_recv =Lock_SempCreate(1,0,CN_BLOCK_PRIO,NULL);

    //创建post消息链表缓冲区
    pMsgQ->post_pbuf =(void*)malloc(size*sizeof(struct MsgList));
    if( (NULL==pMsgQ->post_pbuf)||(NULL==pMsgQ->mutex_lock)
            ||(NULL==pMsgQ->sem_msg)||(NULL==pMsgQ->sem_sync_send))
//          ||(NULL==pMsgQ->sem_sync_recv) )
    {
        goto ErrorExit;
    }
    memset(pMsgQ->post_pbuf, 0, size*sizeof(struct MsgList));
    //初始化post消息链表缓冲区
    pMsgQ->post_free =(struct MsgList*)pMsgQ->post_pbuf;
    for(i=0;i<(size-1);i++)
    {
        pMsgQ->post_free[i].Next =  &pMsgQ->post_free[i+1];
    }
    pMsgQ->post_free[size-1].Next =NULL;

    pMsgQ->post_first =NULL;
    pMsgQ->post_last  =NULL;

    //...
    dListInit(&pMsgQ->list_msg_close);
    dListInit(&pMsgQ->list_msg_ncpaint);
    dListInit(&pMsgQ->list_msg_paint);
    dListInit(&pMsgQ->list_msg_timer);

    __GDD_InitMsg(&pMsgQ->sync_msg,0,0,0,0);
//  __GDD_InitMsg(&pMsgQ->quit_msg,0,0,0,0);

    return pMsgQ;

ErrorExit:
    free(pMsgQ->post_pbuf);
//  Lock_SempDelete(pMsgQ->sem_sync_recv);
    Lock_SempDelete(pMsgQ->sem_sync_send);
    Lock_SempDelete(pMsgQ->sem_msg);
    Lock_MutexDelete(pMsgQ->mutex_lock);
    free(pMsgQ);
    return NULL;
}

void    __GUI_DeleteMsgQ(struct WinMsgQueueCB *pMsgQ)
{
    if(pMsgQ)
    {
        Lock_SempDelete(pMsgQ->sem_msg);
        Lock_SempDelete(pMsgQ->sem_sync_send);
//      Lock_SempDelete(pMsgQ->sem_sync_recv);

        Lock_MutexDelete(pMsgQ->mutex_lock);

        free(pMsgQ->post_pbuf);
        free(pMsgQ);
    }

}

//----派发消息------------------------------------------------------------------
//描述: 同步处理,直到该消息被窗口过程处理完成,该函数才会返回.
//参数：pMsg: 需要派发的消息指针.
//返回：消息处理结果.
//------------------------------------------------------------------------------
u32 GDD_DispatchMessage(struct WindowMsg *pMsg)
{
    u32 res=0;

//  if(NULL!=pMsg)
//  {
//      if(__GDD_GetWindowEvent(pMsg->hwnd) == DJY_GetMyEventId())
//      { //同线程内,直接调用窗口过程.
//          res =__GDD_WinMsgProc(pMsg);
//      }
//  }
    res =__GDD_WinMsgProc(pMsg);
    return res;
}

//----主窗口消息循环-----------------------------------------------------------
//功能：主窗口的消息循环函数，执行消息的提取、分发功能，主窗口被close将导致本
//      函数返回，从而线程结束。
//参数：无
//返回：0
//-----------------------------------------------------------------------------
ptu32_t __GDD_MessageLoop( void )
{
    struct WindowMsg msg;
    bool_t SyncMsg;
    u32 result = 0;
    HWND MyHwnd;
    DJY_GetEventPara((ptu32_t *)&MyHwnd,NULL);
    MyHwnd->EventID   = DJY_GetMyEventId();

    while(GDD_GetMessage(&msg,MyHwnd,&SyncMsg))
    {
        //如果被处理的是发给主窗口的close消息，处理完成后退出消息循环
        //注msg.hwnd不能在调用DispatchMessage后访问，因为对于MSG_CLOSE来说，消息
        //里的hwnd在调用DispatchMessage后已经被清除。
        if( (msg.Code == MSG_CLOSE) && (msg.hwnd == MyHwnd) )
        {
            result = GDD_DispatchMessage(&msg);
            break;
        }
        else
            result = GDD_DispatchMessage(&msg);

        if(SyncMsg) //若是同步消息，则要返回处理结果，并释放信号量。
        {
            MyHwnd->pMsgQ->sync_msg.Param1 = result;
            Lock_SempPost(MyHwnd->pMsgQ->sem_sync_send);
        }
    }
    DJY_EvttUnregist( DJY_GetMyEvttId( ) );    //消息循环结束，释放事件类型
    return result;
}

//----发送同步消息--------------------------------------------------------------
//描述: 该函数为内部调用,仅用于跨线程发送同步消息.
//参数：hwnd: 窗口句柄.
//      msg: 消息代码.
//      param1: 消息参数1.
//      param2: 消息参数2.
//返回：消息处理结果.
//------------------------------------------------------------------------------
static u32 __GDD_PostSyncMessage(HWND hwnd,u32 msg,u32 param1,ptu32_t param2)
{
    struct WinMsgQueueCB *pMsgQ;
    u32 res;


    pMsgQ =__GDD_GetWindowMsgQ(hwnd);

    Lock_MutexPend(pMsgQ->mutex_lock,CN_TIMEOUT_FOREVER);

    __GDD_InitMsg(&pMsgQ->sync_msg,hwnd,msg,param1,param2);


//    Lock_SempPost(pMsgQ->sem_sync_recv);

    Lock_SempPost(pMsgQ->sem_msg);    //发送消息信号量

    Lock_SempPend(pMsgQ->sem_sync_send,CN_TIMEOUT_FOREVER);


    res=pMsgQ->sync_msg.Param1;

    Lock_MutexPost(pMsgQ->mutex_lock);

    return res;

}

//----获取同步消息--------------------------------------------------------------
//描述: 把窗口的同步消息取出
//参数：pMsgQ: 消息队列指针.
//      pMsg: 消息缓冲区指针,获得的消息会存放到该消息缓冲区中.
//返回：TRUE:成功获取了消息; FALSE:没有获得消息.
//------------------------------------------------------------------------------
static bool_t __GDD_PeekSyncMessage(struct WinMsgQueueCB *pMsgQ,struct WindowMsg *pMsg)
{
//    if(Lock_SempPend(pMsgQ->sem_sync_recv, 0))
    if(pMsgQ->sync_msg.hwnd != NULL)
    {
        __GDD_CopyMsg(pMsg,&pMsgQ->sync_msg);
        pMsgQ->sync_msg.hwnd = NULL;
        return true;
    }
    else
        return false;
}

////----处理同步消息--------------------------------------------------------------
////描述: 该函数为内部调用,不检查函数参数的合法性.
////参数：pMsgQ:消息队列指针.
////返回：无.
////------------------------------------------------------------------------------
//static void __HandleSyncMessage(struct WinMsgQueueCB *pMsgQ)
//{
//    if(Lock_SempPend(pMsgQ->sem_sync_recv, 0))
//    {
//        struct WindowMsg *pMsg;
//
//        pMsg =&pMsgQ->sync_msg;
//        pMsgQ->sync_msg.Param1 = __GDD_WinMsgProc(pMsg);
//
//        Lock_SempPost(pMsgQ->sem_sync_send);
//
//    }
//}

//----发送消息------------------------------------------------------------------
//描述: 同步处理,直到该消息被窗口过程处理完成,函数才会返回,该函数会自动处理本线程
//      和跨线程同步消息发送处理.
//参数：hwnd: 窗口句柄.
//      msg: 消息代码.
//      param1: 消息参数1.
//      param2: 消息参数2.
//返回：消息处理结果.
//特注：如果在同一个主窗口以及所属子窗口范围内调用SendMessage发消息，将引发递归
//      调用，慎用！为了栈安全，建议使用PostMessage，除非你需要等待执行结果。
//------------------------------------------------------------------------------
u32 GDD_SendMessage(HWND hwnd,u32 msg,u32 param1,ptu32_t param2)
{
    u32 res=0;
    struct WindowMsg Msg;

    if(msg==MSG_CLOSE)
    {
        //MSG_CLOSE 必须异步处理，在PostMessage函数里面有判非法hwnd的代码
        GDD_PostMessage(hwnd,msg,param1,param2);
    }
    else
    {
        if(__GDD_Lock())
        {
            if(hwnd->pGkWin == NULL)        //无gkwin的句柄，一定是无效的。
            {
                __GDD_Unlock();
                return false;
            }
            if(__GDD_GetWindowEvent(hwnd) == DJY_GetMyEventId())
            {
                //如果是同一线程内,直接调用窗口过程
                __GDD_InitMsg(&Msg,hwnd,msg,param1,param2);
                res = __GDD_WinMsgProc(&Msg);
                __GDD_Unlock();

            }
            else
            {
                __GDD_Unlock();
                res=__GDD_PostSyncMessage(hwnd,msg,param1,param2);
            }
        }
    }

    return res;
}

//----发送非客户区绘制消息-------------------------------------------------------
//描述:发送一条非客户区绘制消息到消息队列.该函数为内部调用,不检查函数参数合法性.
//参数：pMsgQ: 消息队列指针.
//      hwnd: 需要绘制的窗口句柄.
//返回：无.
//------------------------------------------------------------------------------
static void    __GDD_PostNC_PaintMessage(struct WinMsgQueueCB *pMsgQ,HWND hwnd)
{
    //判断窗口是否为可见,如果窗口不可见,则不发送绘制消息.
    if(GDD_IsWindowVisible(hwnd))
    {
        //判断窗口node_msg_ncpaint是否为空,如果不为空,
        //则表示该窗口节点已经加入到消息队列的 list_msg_ncpaint中.
        if(dListIsEmpty(&hwnd->node_msg_ncpaint))
        {
            //加入到消息队列list_msg_ncpaint的链表末尾.
            dListInsertBefore(&pMsgQ->list_msg_ncpaint,&hwnd->node_msg_ncpaint);
        }

        //发送消息信号量
        Lock_SempPost(pMsgQ->sem_msg);
    }

}

//----发送客户区绘制消息---------------------------------------------------------
//描述:发送一条客户区绘制消息到消息队列.该函数为内部调用,不检查函数参数合法性.
//参数：pMsgQ: 消息队列指针.
//      hwnd: 需要绘制的窗口句柄.
//      bErase: 是否重绘背景
//返回：无.
//------------------------------------------------------------------------------
static void    __GDD_PostPaintMessage(struct WinMsgQueueCB *pMsgQ,HWND hwnd,bool_t bErase)
{
    //判断窗口是否为可见,如果窗口不可见,则不发送绘制消息.
    if(GDD_IsWindowVisible(hwnd))
    {
        //判断窗口node_msg_paint是否为空,如果不为空,
        //则表示该窗口节点已经加入到消息队列的 list_msg_paint中.
        if(dListIsEmpty(&hwnd->node_msg_paint))
        {
            //加入到消息队列list_msg_paint的链表末尾.
            dListInsertBefore(&pMsgQ->list_msg_paint,&hwnd->node_msg_paint);
        }

        if(bErase)
        {
            hwnd->Flag |= WF_ERASEBKGND;
        }

        //发送消息信号量
        Lock_SempPost(pMsgQ->sem_msg);
    }
}

//----发送定时器消息-----------------------------------------------------------
//描述:发送定时器消息到消息队列.该函数为内部调用.
//参数：ptmr: 定时器结构指针.
//返回：无.
//------------------------------------------------------------------------------
void    __GDD_PostTimerMessage(struct WinMsgQueueCB *pMsgQ,HWND hwnd,struct WinTimer *ptmr)
{
    if(__HWND_Lock(hwnd))
    {
//      pMsgQ =__GDD_GetWindowMsgQ(hwnd);

        if(dListIsEmpty(&ptmr->node_msg_timer))
        {
            //加入到消息队列list_msg_msg_timer的链表末尾.
            dListInsertBefore(&pMsgQ->list_msg_timer,&ptmr->node_msg_timer);
        }

        //发送消息信号量
        Lock_SempPost(pMsgQ->sem_msg);

        __HWND_Unlock(hwnd);
    }
}

//----发送窗口关闭消息---------------------------------------------------------
//描述:该函数为内部调用,不检查函数参数合法性.
//参数：pMsgQ:消息队列指针.
//      hwnd:需要关闭的窗口句柄.
//返回：无.
//-----------------------------------------------------------------------------
void __GDD_PostCloseMessage(struct WinMsgQueueCB *pMsgQ,HWND hwnd)
{
    if(hwnd == GDD_GetDesktopWindow( ))
        return;                 //桌面不接收close消息。
    //判断窗口node_msg_close是否为空,如果不为空,
    //则表示该窗口节点已经加入到消息队列的 list_msg_close中.
    if(dListIsEmpty(&hwnd->node_msg_close))
    {
        //加入到消息队列list_msg_close的链表末尾.
        dListInsertBefore(&pMsgQ->list_msg_close,&hwnd->node_msg_close);
    }
    //发送消息信号量
    Lock_SempPost(pMsgQ->sem_msg);

}

//----发送刷新显示消息----------------------------------------------------------
//描述: 该函数为内部调用,仅用于跨线程发送同步消息.
//参数：pMsgQ: 消息队列
//返回：消息处理结果.
//------------------------------------------------------------------------------
static void __GDD_PostRefreshMessage(struct WinMsgQueueCB *pMsgQ,HWND hwnd)
{
    __GDD_InitMsg(&pMsgQ->refresh_msg, hwnd, MSG_SYNC_DISPLAY, 0, 0);
    return ;
}

//----发送异步消息--------------------------------------------------------------
//描述: 发送消息到消息队列中,不等待处理,立即返回.该函数为内部调用,不检查函数参数合法性.
//参数：pMsgQ:消息队列指针.
//      hwnd:窗口句柄.
//      msg:消息代码.
//      param1:消息参数1.
//      param2:消息参数2.
//返回：TRUE:成功;FALSE:失败.
//------------------------------------------------------------------------------
bool_t    __GDD_PostMessage(struct WinMsgQueueCB *pMsgQ,HWND hwnd,u32 msg,u32 param1,ptu32_t param2)
{
    struct MsgList *new_msg;
    bool_t res=FALSE;

    //判断是否有空闲的QMSG
    if(NULL!=pMsgQ->post_free)
    {
        //从空闲的post链表取一个QMSG
        new_msg = pMsgQ->post_free;
        pMsgQ->post_free  = new_msg->Next;

        //初始化新的消息数据
        new_msg->Next =NULL;
        __GDD_InitMsg(&new_msg->Msg,hwnd,msg,param1,param2);

        if(NULL==pMsgQ->post_first)
        {
            pMsgQ->post_first =new_msg;
        }

        //将新消息插入到post链表末尾.
        if(NULL==pMsgQ->post_last)
        {
            pMsgQ->post_last =new_msg;
        }
        else
        {
            pMsgQ->post_last->Next =new_msg;
            pMsgQ->post_last =new_msg;
        }

        //发送消息信号量
        Lock_SempPost(pMsgQ->sem_msg);
        res =TRUE;
    }
    else
    {
        new_msg = pMsgQ->post_first;
        //printf("************ %s message num = %d\r\n", hwnd->Text, Lock_SempQueryFree(pMsgQ->sem_msg));
        while(new_msg != NULL)
        {
            printf("------%s code = %d\r\n", new_msg->Msg.hwnd->Text, new_msg->Msg.Code);
            new_msg = new_msg->Next;
        }
    }

    return res;
}

//----发送异步消息--------------------------------------------------------------
//描述: 发送消息到消息队列中,不等待处理,立即返回.
//参数：hwnd:窗口句柄.
//      msg:消息代码.
//      param1:消息参数1.
//      param2:消息参数2.
//返回：TRUE:成功;FALSE:失败.
//------------------------------------------------------------------------------
bool_t    GDD_PostMessage(HWND hwnd,u32 msg,u32 param1,ptu32_t param2)
{
    struct WinMsgQueueCB *pMsgQ;
    bool_t res=FALSE;

    if(__GDD_Lock())
    {
        if(hwnd->pGkWin == NULL)        //无gkwin的句柄，一定是无效的。
        {
            __GDD_Unlock();
            return false;
        }
        pMsgQ =__GDD_GetWindowMsgQ(hwnd);
        if(NULL!=pMsgQ)
        {
            switch(msg)
            {
                case MSG_PAINT:
                    __GDD_PostPaintMessage(pMsgQ,hwnd,param1);
                    res=TRUE;
                    break;

                case MSG_NCPAINT:
                    __GDD_PostNC_PaintMessage(pMsgQ,hwnd);
                    res=TRUE;
                    break;

                case MSG_CLOSE:
                    __GDD_PostCloseMessage(pMsgQ,hwnd);
                    res=TRUE;
                    break;

                case MSG_TIMER:
                    __GDD_PostTimerMessage(pMsgQ,hwnd,(struct WinTimer *)param2);
                    res=TRUE;
                    break;

                case MSG_SYNC_DISPLAY:
                    __GDD_PostRefreshMessage(pMsgQ,hwnd);
                    res=TRUE;
                    break;

                default:
                    res=__GDD_PostMessage(pMsgQ,hwnd,msg,param1,param2);
                    break;
            }

        }

        __GDD_Unlock();
    }
    return res;
}

//----获取异步消息--------------------------------------------------------------
//描述: 丛消息队列中以非阻塞方式获取一条异步消息.该函数为内部调用,不检查函数
//      参数的合法性.
//参数：pMsgQ: 消息队列指针.
//      pMsg: 消息缓冲区指针,获得的消息会存放到该消息缓冲区中.
//返回：TRUE:成功获取了消息; FALSE:没有获得消息.
//------------------------------------------------------------------------------
static  bool_t    __GDD_PeekPostMessage(struct WinMsgQueueCB *pMsgQ,struct WindowMsg *pMsg)
{
    struct MsgList *new_msg;
    bool_t res=FALSE;

    //判断是否有消息
    if(NULL!=pMsgQ->post_first)
    {
        //从post链表取一个QMSG
        new_msg = pMsgQ->post_first;
        pMsgQ->post_first  = new_msg->Next;

        //初始化新的消息数据
        new_msg->Next =NULL;
        __GDD_CopyMsg(pMsg,&new_msg->Msg);

        //如果当前获取的是最后一条消息,则将post_last 设置为 NULL
        if(new_msg==pMsgQ->post_last)
        {
            pMsgQ->post_last =NULL;
        }

        //将处理完的QMSG添加到空闲链表中
        if(NULL==pMsgQ->post_free)
        {
            pMsgQ->post_free =new_msg;
        }
        else
        {
            new_msg->Next =pMsgQ->post_free;
            pMsgQ->post_free =new_msg;
        }

        res =TRUE;
    }

    return res;
}

//----获取非客户区绘制消息-------------------------------------------------------
//描述: 丛消息队列中以非阻塞方式获取一条非客户区绘制消息.该函数为内部调用,不检查
//      函数参数的合法性.
//参数：pMsgQ: 消息队列指针.
//      pMsg: 消息缓冲区指针,获得的消息会存放到该消息缓冲区中.
//返回：TRUE:成功获取了消息; FALSE:没有获得消息.
//------------------------------------------------------------------------------
static  bool_t    __GDD_PeekNC_PaintMessage(struct WinMsgQueueCB *pMsgQ,struct WindowMsg *pMsg)
{
    list_t *n;
    HWND hwnd;

    if(!dListIsEmpty(&pMsgQ->list_msg_ncpaint))
    {
        n =pMsgQ->list_msg_ncpaint.next;
        dListRemove(n);
        hwnd =dListEntry(n,struct WINDOW,node_msg_ncpaint);
        __GDD_InitMsg(pMsg,hwnd,MSG_NCPAINT,0,0);
        return TRUE;
    }
    return FALSE;
}

//----获取客户区绘制消息---------------------------------------------------------
//描述: 丛消息队列中以非阻塞方式获取一条客户区绘制消息.该函数为内部调用,不检查
//      函数参数的合法性.
//参数：pMsgQ: 消息队列指针.
//      pMsg: 消息缓冲区指针,获得的消息会存放到该消息缓冲区中.
//返回：TRUE:成功获取了消息; FALSE:没有获得消息.
//------------------------------------------------------------------------------
static  bool_t    __GDD_PeekPaintMessage(struct WinMsgQueueCB *pMsgQ,struct WindowMsg *pMsg)
{
    list_t *n;
    HWND hwnd;

    if(!dListIsEmpty(&pMsgQ->list_msg_paint))
    {
        n =pMsgQ->list_msg_paint.next;
        dListRemove(n);
        hwnd =dListEntry(n,struct WINDOW,node_msg_paint);
        __GDD_InitMsg(pMsg,hwnd,MSG_PAINT,0,0);
        return TRUE;
    }
    return FALSE;
}

//----获取定时器消息------------------------------------------------------------
//描述: 丛消息队列中以非阻塞方式获取一条定时器消息.该函数为内部调用,不检查函数
//      参数的合法性.
//参数：pMsgQ: 消息队列指针.
//      pMsg: 消息缓冲区指针,获得的消息会存放到该消息缓冲区中.
//返回：TRUE:成功获取了消息; FALSE:没有获得消息.
//------------------------------------------------------------------------------
static  bool_t    __GDD_PeekTimerMessage(struct WinMsgQueueCB *pMsgQ,struct WindowMsg *pMsg)
{
    list_t *n;
    struct WinTimer *ptmr;

    if(!dListIsEmpty(&pMsgQ->list_msg_timer))
    {
//      n =pMsgQ->list_msg_timer.next;
        n =dListGetAfter(&pMsgQ->list_msg_timer);
        dListRemove(n);
        ptmr =dListEntry(n,struct WinTimer,node_msg_timer);
        __GDD_InitMsg(pMsg,ptmr->hwnd,MSG_TIMER,ptmr->Id,(u32)ptmr);
        return TRUE;
    }
    return FALSE;
}

//----获取窗口关闭消息-----------------------------------------------------------
//描述: 丛消息队列中以非阻塞方式获取一条窗口关闭消息.该函数为内部调用,不检查函数
//      参数的合法性.
//参数：pMsgQ: 消息队列指针.
//      pMsg: 消息缓冲区指针,获得的消息会存放到该消息缓冲区中.
//返回：TRUE:成功获取了消息; FALSE:没有获得消息.
//------------------------------------------------------------------------------
static  bool_t    __GDD_PeekCloseMessage(struct WinMsgQueueCB *pMsgQ,struct WindowMsg *pMsg)
{
    list_t *n;
    HWND hwnd;

    if(!dListIsEmpty(&pMsgQ->list_msg_close))
    {
        n =dListGetAfter(&pMsgQ->list_msg_close);
        dListRemove(n);
        hwnd =dListEntry(n,struct WINDOW,node_msg_close);
        __GDD_InitMsg(pMsg, hwnd, MSG_CLOSE, 0, (ptu32_t)hwnd->PrivateData);
        return TRUE;
    }
    return FALSE;
}

//----获取刷新显示消息-----------------------------------------------------------
//描述: 把窗口的刷新显示消息取出
//参数：pMsgQ: 消息队列指针.
//      pMsg: 消息缓冲区指针,获得的消息会存放到该消息缓冲区中.
//返回：TRUE:成功获取了消息; FALSE:没有获得消息.
//------------------------------------------------------------------------------
static bool_t __GDD_PeekRefreshMessage(struct WinMsgQueueCB *pMsgQ,struct WindowMsg *pMsg)
{
//    if(Lock_SempPend(pMsgQ->sem_sync_recv, 0))
    if(pMsgQ->refresh_msg.hwnd != NULL)
    {
        __GDD_CopyMsg(pMsg,&pMsgQ->refresh_msg);
        pMsgQ->refresh_msg.hwnd = NULL;
        return true;
    }
    else
        return false;
}

/*============================================================================*/

//----获取消息------------------------------------------------------------------
//描述: 丛消息队列中以阻塞方式获取一条消息.
//参数：pMsg: 消息缓冲区指针,获得的消息会存放到该消息缓冲区中.
//     hwnd: 窗口句柄.
//返回：当获得了MSG_QUIT时,该函数返回FALSE,否则返回TRUE.
//------------------------------------------------------------------------------
bool_t    GDD_GetMessage(struct WindowMsg *pMsg,HWND hwnd,bool_t *SyncMsg)
{
    s32 res=FALSE;
    struct WinMsgQueueCB *pMsgQ;
    bool_t GetMsg;

    *SyncMsg = false;
    while(1)
    {

        if(__HWND_Lock(hwnd))
        {
            GetMsg = true;
            pMsgQ =__GDD_GetWindowMsgQ(hwnd);
            if(NULL!=pMsgQ)
            {
                res = true;
                //注，if语句的顺序，就是各类消息的优先级，不得随意调整
                if(__GDD_PeekCloseMessage(pMsgQ,pMsg))
                {
                }
                else if(__GDD_PeekSyncMessage(pMsgQ,pMsg))
                {
                    *SyncMsg = true;
                }
                else if(__GDD_PeekPostMessage(pMsgQ,pMsg))
                {
                }
                else if(__GDD_PeekNC_PaintMessage(pMsgQ,pMsg))
                {
                }

                else if(__GDD_PeekPaintMessage(pMsgQ,pMsg))
                {
                }
                else if(__GDD_PeekTimerMessage(pMsgQ,pMsg))
                {
                }
                else if(__GDD_PeekRefreshMessage(pMsgQ,pMsg))
                {
                }
                else
                    GetMsg = false;
            }
            __HWND_Unlock(hwnd);
        }

        if(GetMsg)
        {
            break;
        }
        else
        {
            if(__HWND_Lock(hwnd))
            {
                pMsgQ =__GDD_GetWindowMsgQ(hwnd);
                __HWND_Unlock(hwnd);

                if(NULL!=pMsgQ)
                {
                    Lock_SempPend(pMsgQ->sem_msg,CN_TIMEOUT_FOREVER);
                }

            }
        }
    }
    return res;
}

ptu32_t GDD_GetGK_Message(void)
{
    define_align_buf(TaskBuffer,CN_USERCALL_MSG_SIZE);
//    u8 TaskBuffer[CN_USERCALL_MSG_SIZE];
    HWND hwnd;
    u16 command;
    while(1)
    {
        command = GK_ReadRequest((u8*)TaskBuffer, CN_USERCALL_MSG_SIZE, CN_TIMEOUT_FOREVER);
        switch(command)
        {
            case CN_GKUC_REPAINT:
            {
                struct GkucParaRepaint *Param = (struct GkucParaRepaint *)(TaskBuffer);
                hwnd = (HWND)GK_GetUserTag(Param->gk_win);
                GDD_PostMessage(hwnd,MSG_NCPAINT,0,0);
                GDD_PostMessage(hwnd,MSG_PAINT,0,0);
            }break;
            default:
                break;
        }
    }
}
