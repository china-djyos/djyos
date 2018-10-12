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
// Copyright (c) 2018 ����Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

//����ģ��: GDD
//����:  LiuWei.
//�汾��V1.0.0
//�ļ�����: ��Ϣϵͳ����ʵ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include    <gui/gdd/gdd_private.h>

/*============================================================================*/

struct MsgList
{
    struct WindowMsg Msg;
    struct MsgList *Next;
};

struct WinMsgQueueCB
{
    struct MutexLCB *mutex_lock;            //��Ϣ������
    struct SemaphoreLCB   *sem_msg;         //��Ϣ�ź���
    struct SemaphoreLCB   *sem_sync_send;   //ͬ����Ϣ�����ź���
    struct SemaphoreLCB   *sem_sync_recv;   //ͬ����Ϣ�����ź���

    //ͬ�����˳���Ϣ��һ��������ֻ��һ����ֱ�Ӷ��塣
    struct WindowMsg     sync_msg;          //ͬ����Ϣ
    struct WindowMsg     quit_msg;          //�˳���Ϣ
    //����������Ϣ�����ж�������Ҫ������post��Ϣ�����ʵ���������
    list_t  list_msg_close;                 //CLOSE��Ϣ����
    list_t  list_msg_ncpaint;               //NCPAINT��Ϣ����
    list_t  list_msg_paint;                 //PAINT��Ϣ����
    list_t  list_msg_timer;                 //TIMER��Ϣ����

    //Post������Ϣ����
    void    *post_pbuf;         //��¼ԭʼ��post��Ϣ��������ַ(���ڶ�̬�ڴ��ͷ�)
    struct MsgList *post_free;     //ָ����е�post��Ϣ��������
    struct MsgList *post_first;    //ָ���һ��post��Ϣ����
    struct MsgList *post_last;     //ָ�����һ��post��Ϣ����ڵ�
};

//----����һ����Ϣ--------------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���.
//������dst: Ŀ����Ϣָ��.
//      src: Դ��Ϣָ��.
//���أ���.
//------------------------------------------------------------------------------
static  void __CopyMsg(struct WindowMsg *dst,const struct WindowMsg* src)
{
//  dst->hwnd   =src->hwnd;
//  dst->Code   =src->Code;
//  dst->Param1 =src->Param1;
//  dst->Param2 =src->Param2;
    *dst = *src;
    dst->ExData =NULL;
}

//----��ʼ��һ����Ϣ-----------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���.
//������msg: ��Ϣ�ṹ��ָ��.
//     hwnd: ��Ϣ���ھ��.
//     code: ��Ϣ�����.
//     param1: ��Ϣ����1.
//     param2: ��Ϣ����2.
//���أ���.
//-----------------------------------------------------------------------------
void __InitMsg(struct WindowMsg *msg,HWND hwnd,u32 code,u32 param1,ptu32_t param2)
{
    msg->hwnd   =hwnd;
    msg->Code   =code;
    msg->Param1 =param1;
    msg->Param2 =param2;
    //msg->Time     =WDD_GetTickMS();
    msg->ExData =NULL;
}

//----����һ����Ϣ����---------------------------------------------------------
//����: �ú���Ϊ�ڲ�����.
//������size: �첽��Ϣ����������.
//���أ���Ϣ����ָ��.
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

    //������Ϣ������
    pMsgQ->mutex_lock =Lock_MutexCreate(NULL);

    //������Ϣ�ź���
//  pMsgQ->sem_msg  =Lock_SempCreate(1000,0,CN_BLOCK_PRIO,NULL);
    pMsgQ->sem_msg  =Lock_SempCreate(size,0,CN_BLOCK_PRIO,NULL);

    pMsgQ->sem_sync_send =Lock_SempCreate(1,1,CN_BLOCK_PRIO,NULL);
    pMsgQ->sem_sync_recv =Lock_SempCreate(1,0,CN_BLOCK_PRIO,NULL);

    //����post��Ϣ��������
    pMsgQ->post_pbuf =(void*)malloc(size*sizeof(struct MsgList));
    if( (NULL==pMsgQ->post_pbuf)||(NULL==pMsgQ->mutex_lock)
            ||(NULL==pMsgQ->sem_msg)||(NULL==pMsgQ->sem_sync_send)
            ||(NULL==pMsgQ->sem_sync_recv) )
    {
        goto ErrorExit;
    }

    //��ʼ��post��Ϣ��������
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

    __InitMsg(&pMsgQ->sync_msg,0,0,0,0);
    __InitMsg(&pMsgQ->quit_msg,0,0,0,0);

    return pMsgQ;

ErrorExit:
    free(pMsgQ->post_pbuf);
    Lock_SempDelete(pMsgQ->sem_sync_recv);
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
        Lock_SempDelete(pMsgQ->sem_sync_recv);

        Lock_MutexDelete(pMsgQ->mutex_lock);

        free(pMsgQ->post_pbuf);
        free(pMsgQ);
    }

}

//----�ɷ���Ϣ------------------------------------------------------------------
//����: ͬ������,ֱ������Ϣ�����ڹ��̴������,�ú����Ż᷵��.
//������pMsg: ��Ҫ�ɷ�����Ϣָ��.
//���أ���Ϣ������.
//------------------------------------------------------------------------------
u32 DispatchMessage(struct WindowMsg *pMsg)
{
    u32 res=0;

//  if(NULL!=pMsg)
//  {
//      if(__GetWindowEvent(pMsg->hwnd) == Djy_MyEventId())
//      { //ͬ�߳���,ֱ�ӵ��ô��ڹ���.
//          res =__WinMsgProc(pMsg);
//      }
//  }
    res =__WinMsgProc(pMsg);
    return res;
}

//----��������Ϣѭ��-----------------------------------------------------------
//���ܣ������ڵ���Ϣѭ��������ִ����Ϣ����ȡ���ַ����ܣ������ڱ�close�����±�
//      �������أ��Ӷ��߳̽�����
//��������
//���أ�0
//-----------------------------------------------------------------------------
ptu32_t __MessageLoop( void )
{
    struct WindowMsg msg;
    bool_t SyncMsg;
    u32 result;
    HWND MyHwnd;
    Djy_GetEventPara((ptu32_t *)&MyHwnd,NULL);
    while(GetMessage(&msg,MyHwnd,&SyncMsg))
    {
        //�����������Ƿ��������ڵ�close��Ϣ��������ɺ��˳���Ϣѭ��
        //עmsg.hwnd�����ڵ���DispatchMessage����ʣ���Ϊ����MSG_CLOSE��˵����Ϣ
        //���hwnd�ڵ���DispatchMessage���Ѿ��������
        if( (msg.Code == MSG_CLOSE) && (msg.hwnd == MyHwnd) )
        {
            DispatchMessage(&msg);
            break;
        }
        else
            result = DispatchMessage(&msg);

        if(SyncMsg) //����ͬ����Ϣ����Ҫ���ش����������ͷ��ź�����
        {
            MyHwnd->pMsgQ->sync_msg.Param1 = result;
            Lock_SempPost(MyHwnd->pMsgQ->sem_sync_send);
        }
    }
    Djy_EvttUnregist( Djy_MyEvttId( ) );    //��Ϣѭ���������ͷ��¼�����
    return result;
}

//----����ͬ����Ϣ--------------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,�����ڿ��̷߳���ͬ����Ϣ.
//������hwnd: ���ھ��.
//      msg: ��Ϣ����.
//      param1: ��Ϣ����1.
//      param2: ��Ϣ����2.
//���أ���Ϣ������.
//------------------------------------------------------------------------------
static u32 __PostSyncMessage(HWND hwnd,u32 msg,u32 param1,ptu32_t param2)
{
    struct WinMsgQueueCB *pMsgQ;
    u32 res;


    pMsgQ =__GetWindowMsgQ(hwnd);

    Lock_MutexPend(pMsgQ->mutex_lock,CN_TIMEOUT_FOREVER);

    __InitMsg(&pMsgQ->sync_msg,hwnd,msg,param1,param2);


    Lock_SempPost(pMsgQ->sem_sync_recv);
    Lock_SempPost(pMsgQ->sem_msg);    //������Ϣ�ź���

    Lock_SempPend(pMsgQ->sem_sync_send,CN_TIMEOUT_FOREVER);
    res=pMsgQ->sync_msg.Param1;

    Lock_MutexPost(pMsgQ->mutex_lock);

    return res;

}

//----��ȡͬ����Ϣ--------------------------------------------------------------
//����: �Ѵ��ڵ�ͬ����Ϣȡ��
//������pMsgQ: ��Ϣ����ָ��.
//      pMsg: ��Ϣ������ָ��,��õ���Ϣ���ŵ�����Ϣ��������.
//���أ�TRUE:�ɹ���ȡ����Ϣ; FALSE:û�л����Ϣ.
//------------------------------------------------------------------------------
static bool_t __PeekSyncMessage(struct WinMsgQueueCB *pMsgQ,struct WindowMsg *pMsg)
{
    if(Lock_SempPend(pMsgQ->sem_sync_recv, 0))
    {
        __CopyMsg(pMsg,&pMsgQ->sync_msg);

        return true;
    }
    else
        return false;
}

//----����ͬ����Ϣ--------------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���.
//������pMsgQ:��Ϣ����ָ��.
//���أ���.
//------------------------------------------------------------------------------
static void __HandleSyncMessage(struct WinMsgQueueCB *pMsgQ)
{
    if(Lock_SempPend(pMsgQ->sem_sync_recv, 0))
    {
        struct WindowMsg *pMsg;

        pMsg =&pMsgQ->sync_msg;
        pMsgQ->sync_msg.Param1 = __WinMsgProc(pMsg);

        Lock_SempPost(pMsgQ->sem_sync_send);

    }
}

//----������Ϣ------------------------------------------------------------------
//����: ͬ������,ֱ������Ϣ�����ڹ��̴������,�����Ż᷵��,�ú������Զ������߳�
//      �Ϳ��߳�ͬ����Ϣ���ʹ���.
//������hwnd: ���ھ��.
//      msg: ��Ϣ����.
//      param1: ��Ϣ����1.
//      param2: ��Ϣ����2.
//���أ���Ϣ������.
//��ע�������ͬһ���������Լ������Ӵ��ڷ�Χ�ڵ���SendMessage����Ϣ���������ݹ�
//      ���ã����ã�Ϊ��ջ��ȫ������ʹ��PostMessage����������Ҫ�ȴ�ִ�н����
//------------------------------------------------------------------------------
u32 SendMessage(HWND hwnd,u32 msg,u32 param1,ptu32_t param2)
{
    u32 res=0;
    struct WindowMsg Msg;

    if(msg==MSG_CLOSE)
    {
        //MSG_CLOSE �����첽����
        PostMessage(hwnd,msg,param1,param2);
    }
    else
    {
        if(__HWND_Lock(hwnd))
        {
            if(__GetWindowEvent(hwnd) == Djy_MyEventId())
            {
                //�����ͬһ�߳���,ֱ�ӵ��ô��ڹ���
                __InitMsg(&Msg,hwnd,msg,param1,param2);
                res = __WinMsgProc(&Msg);
                __HWND_Unlock(hwnd);

            }
            else
            {
                __HWND_Unlock(hwnd);
                res=__PostSyncMessage(hwnd,msg,param1,param2);
            }
        }
    }

    return res;
}

//----���ͷǿͻ���������Ϣ-------------------------------------------------------
//����:����һ���ǿͻ���������Ϣ����Ϣ����.�ú���Ϊ�ڲ�����,����麯�������Ϸ���.
//������pMsgQ: ��Ϣ����ָ��.
//      hwnd: ��Ҫ���ƵĴ��ھ��.
//���أ���.
//------------------------------------------------------------------------------
static void    __PostNCPaintMessage(struct WinMsgQueueCB *pMsgQ,HWND hwnd)
{
    //�жϴ����Ƿ�Ϊ�ɼ�,������ڲ��ɼ�,�򲻷��ͻ�����Ϣ.
    if(IsWindowVisible(hwnd))
    {
        //�жϴ���node_msg_ncpaint�Ƿ�Ϊ��,�����Ϊ��,
        //���ʾ�ô��ڽڵ��Ѿ����뵽��Ϣ���е� list_msg_ncpaint��.
        if(dListIsEmpty(&hwnd->node_msg_ncpaint))
        {
            //���뵽��Ϣ����list_msg_ncpaint������ĩβ.
            dListInsertBefore(&pMsgQ->list_msg_ncpaint,&hwnd->node_msg_ncpaint);
        }

        //������Ϣ�ź���
        Lock_SempPost(pMsgQ->sem_msg);
    }

}

//----���Ϳͻ���������Ϣ---------------------------------------------------------
//����:����һ���ͻ���������Ϣ����Ϣ����.�ú���Ϊ�ڲ�����,����麯�������Ϸ���.
//������pMsgQ: ��Ϣ����ָ��.
//      hwnd: ��Ҫ���ƵĴ��ھ��.
//      bErase: �Ƿ��ػ汳��
//���أ���.
//------------------------------------------------------------------------------
static void    __PostPaintMessage(struct WinMsgQueueCB *pMsgQ,HWND hwnd,bool_t bErase)
{
    //�жϴ����Ƿ�Ϊ�ɼ�,������ڲ��ɼ�,�򲻷��ͻ�����Ϣ.
    if(IsWindowVisible(hwnd))
    {
        //�жϴ���node_msg_paint�Ƿ�Ϊ��,�����Ϊ��,
        //���ʾ�ô��ڽڵ��Ѿ����뵽��Ϣ���е� list_msg_paint��.
        if(dListIsEmpty(&hwnd->node_msg_paint))
        {
            //���뵽��Ϣ����list_msg_paint������ĩβ.
            dListInsertBefore(&pMsgQ->list_msg_paint,&hwnd->node_msg_paint);
        }

        if(bErase)
        {
            hwnd->Flag |= WF_ERASEBKGND;
        }

        //������Ϣ�ź���
        Lock_SempPost(pMsgQ->sem_msg);
    }
}

//----���Ͷ�ʱ����Ϣ-----------------------------------------------------------
//����:���Ͷ�ʱ����Ϣ����Ϣ����.�ú���Ϊ�ڲ�����.
//������ptmr: ��ʱ���ṹָ��.
//���أ���.
//------------------------------------------------------------------------------
void    __PostTimerMessage(struct WinTimer *ptmr)
{
    struct WinMsgQueueCB *pMsgQ;
    HWND hwnd=ptmr->hwnd;

    if(__HWND_Lock(hwnd))
    {
        pMsgQ =__GetWindowMsgQ(hwnd);

        if(dListIsEmpty(&ptmr->node_msg_timer))
        {
            //���뵽��Ϣ����list_msg_msg_timer������ĩβ.
            dListInsertBefore(&pMsgQ->list_msg_timer,&ptmr->node_msg_timer);
        }

        //������Ϣ�ź���
        Lock_SempPost(pMsgQ->sem_msg);

        __HWND_Unlock(hwnd);
    }
}

//----���ʹ��ڹر���Ϣ---------------------------------------------------------
//����:�ú���Ϊ�ڲ�����,����麯�������Ϸ���.
//������pMsgQ:��Ϣ����ָ��.
//      hwnd:��Ҫ�رյĴ��ھ��.
//���أ���.
//-----------------------------------------------------------------------------
void __PostCloseMessage(struct WinMsgQueueCB *pMsgQ,HWND hwnd)
{
    if(hwnd == GetDesktopWindow( ))
        return;                 //���治����close��Ϣ��
    //�жϴ���node_msg_close�Ƿ�Ϊ��,�����Ϊ��,
    //���ʾ�ô��ڽڵ��Ѿ����뵽��Ϣ���е� list_msg_close��.
    if(dListIsEmpty(&hwnd->node_msg_close))
    {
        //���뵽��Ϣ����list_msg_close������ĩβ.
        dListInsertBefore(&pMsgQ->list_msg_close,&hwnd->node_msg_close);
    }
    //������Ϣ�ź���
    Lock_SempPost(pMsgQ->sem_msg);

}

//----�����첽��Ϣ--------------------------------------------------------------
//����: ������Ϣ����Ϣ������,���ȴ�����,��������.�ú���Ϊ�ڲ�����,����麯�������Ϸ���.
//������pMsgQ:��Ϣ����ָ��.
//      hwnd:���ھ��.
//      msg:��Ϣ����.
//      param1:��Ϣ����1.
//      param2:��Ϣ����2.
//���أ�TRUE:�ɹ�;FALSE:ʧ��.
//------------------------------------------------------------------------------
bool_t    __PostMessage(struct WinMsgQueueCB *pMsgQ,HWND hwnd,u32 msg,u32 param1,ptu32_t param2)
{
    struct MsgList *new_msg;
    bool_t res=FALSE;

    //�ж��Ƿ��п��е�QMSG
    if(NULL!=pMsgQ->post_free)
    {
        //�ӿ��е�post����ȡһ��QMSG
        new_msg = pMsgQ->post_free;
        pMsgQ->post_free  = new_msg->Next;

        //��ʼ���µ���Ϣ����
        new_msg->Next =NULL;
        __InitMsg(&new_msg->Msg,hwnd,msg,param1,param2);

        if(NULL==pMsgQ->post_first)
        {
            pMsgQ->post_first =new_msg;
        }

        //������Ϣ���뵽post����ĩβ.
        if(NULL==pMsgQ->post_last)
        {
            pMsgQ->post_last =new_msg;
        }
        else
        {
            pMsgQ->post_last->Next =new_msg;
            pMsgQ->post_last =new_msg;
        }

        //������Ϣ�ź���
        Lock_SempPost(pMsgQ->sem_msg);
        res =TRUE;
    }

    return res;
}

//----�����첽��Ϣ--------------------------------------------------------------
//����: ������Ϣ����Ϣ������,���ȴ�����,��������.
//������hwnd:���ھ��.
//      msg:��Ϣ����.
//      param1:��Ϣ����1.
//      param2:��Ϣ����2.
//���أ�TRUE:�ɹ�;FALSE:ʧ��.
//------------------------------------------------------------------------------
bool_t    PostMessage(HWND hwnd,u32 msg,u32 param1,ptu32_t param2)
{
    struct WinMsgQueueCB *pMsgQ;
    bool_t res=FALSE;

    if(__HWND_Lock(hwnd))
    {
        pMsgQ =__GetWindowMsgQ(hwnd);
        if(NULL!=pMsgQ)
        {
            switch(msg)
            {
                case MSG_PAINT:
                    __PostPaintMessage(pMsgQ,hwnd,param1);
                    res=TRUE;
                    break;
                    ////

                case MSG_NCPAINT:
                    __PostNCPaintMessage(pMsgQ,hwnd);
                    res=TRUE;
                    break;
                    ////

                case MSG_CLOSE:
                    __PostCloseMessage(pMsgQ,hwnd);
                    res=TRUE;
                    break;
                    ////

                default:
                    res=__PostMessage(pMsgQ,hwnd,msg,param1,param2);
                    break;
                    ////
            }

        }

        __HWND_Unlock(hwnd);
    }
    return res;
}

//----��ȡ�첽��Ϣ--------------------------------------------------------------
//����: ����Ϣ�������Է�������ʽ��ȡһ���첽��Ϣ.�ú���Ϊ�ڲ�����,����麯��
//      �����ĺϷ���.
//������pMsgQ: ��Ϣ����ָ��.
//      pMsg: ��Ϣ������ָ��,��õ���Ϣ���ŵ�����Ϣ��������.
//���أ�TRUE:�ɹ���ȡ����Ϣ; FALSE:û�л����Ϣ.
//------------------------------------------------------------------------------
static  bool_t    __PeekPostMessage(struct WinMsgQueueCB *pMsgQ,struct WindowMsg *pMsg)
{
    struct MsgList *new_msg;
    bool_t res=FALSE;

    //�ж��Ƿ�����Ϣ
    if(NULL!=pMsgQ->post_first)
    {
        //��post����ȡһ��QMSG
        new_msg = pMsgQ->post_first;
        pMsgQ->post_first  = new_msg->Next;

        //��ʼ���µ���Ϣ����
        new_msg->Next =NULL;
        __CopyMsg(pMsg,&new_msg->Msg);

        //�����ǰ��ȡ�������һ����Ϣ,��post_last ����Ϊ NULL
        if(new_msg==pMsgQ->post_last)
        {
            pMsgQ->post_last =NULL;
        }

        //���������QMSG��ӵ�����������
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

//----��ȡ�ǿͻ���������Ϣ-------------------------------------------------------
//����: ����Ϣ�������Է�������ʽ��ȡһ���ǿͻ���������Ϣ.�ú���Ϊ�ڲ�����,�����
//      ���������ĺϷ���.
//������pMsgQ: ��Ϣ����ָ��.
//      pMsg: ��Ϣ������ָ��,��õ���Ϣ���ŵ�����Ϣ��������.
//���أ�TRUE:�ɹ���ȡ����Ϣ; FALSE:û�л����Ϣ.
//------------------------------------------------------------------------------
static  bool_t    __PeekNCPaintMessage(struct WinMsgQueueCB *pMsgQ,struct WindowMsg *pMsg)
{
    list_t *n;
    HWND hwnd;

    if(!dListIsEmpty(&pMsgQ->list_msg_ncpaint))
    {
        n =pMsgQ->list_msg_ncpaint.next;
        dListRemove(n);
        hwnd =dListEntry(n,struct WINDOW,node_msg_ncpaint);
        __InitMsg(pMsg,hwnd,MSG_NCPAINT,0,0);
        return TRUE;
    }
    return FALSE;
}

//----��ȡ�ͻ���������Ϣ---------------------------------------------------------
//����: ����Ϣ�������Է�������ʽ��ȡһ���ͻ���������Ϣ.�ú���Ϊ�ڲ�����,�����
//      ���������ĺϷ���.
//������pMsgQ: ��Ϣ����ָ��.
//      pMsg: ��Ϣ������ָ��,��õ���Ϣ���ŵ�����Ϣ��������.
//���أ�TRUE:�ɹ���ȡ����Ϣ; FALSE:û�л����Ϣ.
//------------------------------------------------------------------------------
static  bool_t    __PeekPaintMessage(struct WinMsgQueueCB *pMsgQ,struct WindowMsg *pMsg)
{
    list_t *n;
    HWND hwnd;

    if(!dListIsEmpty(&pMsgQ->list_msg_paint))
    {
        n =pMsgQ->list_msg_paint.next;
        dListRemove(n);
        hwnd =dListEntry(n,struct WINDOW,node_msg_paint);
        __InitMsg(pMsg,hwnd,MSG_PAINT,0,0);
        return TRUE;
    }
    return FALSE;
}

//----��ȡ��ʱ����Ϣ------------------------------------------------------------
//����: ����Ϣ�������Է�������ʽ��ȡһ����ʱ����Ϣ.�ú���Ϊ�ڲ�����,����麯��
//      �����ĺϷ���.
//������pMsgQ: ��Ϣ����ָ��.
//      pMsg: ��Ϣ������ָ��,��õ���Ϣ���ŵ�����Ϣ��������.
//���أ�TRUE:�ɹ���ȡ����Ϣ; FALSE:û�л����Ϣ.
//------------------------------------------------------------------------------
static  bool_t    __PeekTimerMessage(struct WinMsgQueueCB *pMsgQ,struct WindowMsg *pMsg)
{
    list_t *n;
    struct WinTimer *ptmr;

    if(!dListIsEmpty(&pMsgQ->list_msg_timer))
    {
//      n =pMsgQ->list_msg_timer.next;
        n =dListGetAfter(&pMsgQ->list_msg_timer);
        dListRemove(n);
        ptmr =dListEntry(n,struct WinTimer,node_msg_timer);
        __InitMsg(pMsg,ptmr->hwnd,MSG_TIMER,ptmr->Id,(u32)ptmr);
        return TRUE;
    }
    return FALSE;
}

//----��ȡ���ڹر���Ϣ-----------------------------------------------------------
//����: ����Ϣ�������Է�������ʽ��ȡһ�����ڹر���Ϣ.�ú���Ϊ�ڲ�����,����麯��
//      �����ĺϷ���.
//������pMsgQ: ��Ϣ����ָ��.
//      pMsg: ��Ϣ������ָ��,��õ���Ϣ���ŵ�����Ϣ��������.
//���أ�TRUE:�ɹ���ȡ����Ϣ; FALSE:û�л����Ϣ.
//------------------------------------------------------------------------------
static  bool_t    __PeekCloseMessage(struct WinMsgQueueCB *pMsgQ,struct WindowMsg *pMsg)
{
    list_t *n;
    HWND hwnd;

    if(!dListIsEmpty(&pMsgQ->list_msg_close))
    {
        n =dListGetAfter(&pMsgQ->list_msg_close);
        dListRemove(n);
        hwnd =dListEntry(n,struct WINDOW,node_msg_close);
        __InitMsg(pMsg, hwnd, MSG_CLOSE, 0, (ptu32_t)hwnd->PrivateData);
        return TRUE;
    }
    return FALSE;
}

/*============================================================================*/

//----��ȡ��Ϣ------------------------------------------------------------------
//����: ����Ϣ��������������ʽ��ȡһ����Ϣ.
//������pMsg: ��Ϣ������ָ��,��õ���Ϣ���ŵ�����Ϣ��������.
//     hwnd: ���ھ��.
//���أ��������MSG_QUITʱ,�ú�������FALSE,���򷵻�TRUE.
//------------------------------------------------------------------------------
bool_t    GetMessage(struct WindowMsg *pMsg,HWND hwnd,bool_t *SyncMsg)
{
    s32 res=FALSE;
    struct WinMsgQueueCB *pMsgQ;
    bool_t GetMsg;

    *SyncMsg = false;
    while(1)
    {
//      if(PeekMessage(pMsg,hwnd))
//      {
//          if(pMsg->Code == MSG_QUIT)
//          {
//              res=FALSE;
//          }
//          else
//          {
//              res=TRUE;
//          }
//          break;
//      }
//      else

        if(__HWND_Lock(hwnd))
        {
            GetMsg = true;
            pMsgQ =__GetWindowMsgQ(hwnd);
            if(NULL!=pMsgQ)
            {
                res = true;
                //ע��if����˳�򣬾��Ǹ�����Ϣ�����ȼ��������������
                if(__PeekCloseMessage(pMsgQ,pMsg))
                {
                }
//                  __HWND_Unlock(hwnd);
                else if(__PeekSyncMessage(pMsgQ,pMsg))
                {
                    *SyncMsg = true;
                }
//                  __HWND_Lock(hwnd);
                else if(__PeekPostMessage(pMsgQ,pMsg))
                {
                }

                else if(__PeekNCPaintMessage(pMsgQ,pMsg))
                {
                }

                else if(__PeekPaintMessage(pMsgQ,pMsg))
                {
                }

                else if(__PeekTimerMessage(pMsgQ,pMsg))
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
            struct WinMsgQueueCB *pMsgQ;

            if(__HWND_Lock(hwnd))
            {
                pMsgQ =__GetWindowMsgQ(hwnd);
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

/*============================================================================*/
