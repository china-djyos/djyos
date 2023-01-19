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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
//�ļ�����: ���ڼ��ؼ����Ƽ�����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include    "gdd_private.h"
#include <gkernel.h>
#include "dbug.h"
#include "pool.h"
#include "component_config_gdd.h"
/*
typedef struct  __WNDCLASS  //���������ݽṹ
{
    WNDPROC *WinProc;
    u32 DrawColor;
    u32 FillColor;
    u32 TextColor;
}WNDCLASS;
*/

extern HWND g_CursorHwnd;         //��괰��
extern void GK_SetVisible(struct GkWinObj *gkwin, u32 visible,u32 SyncTime);
/*============================================================================*/

extern struct MemCellPool *g_ptHwndPool;
HWND HWND_Desktop=NULL;
u16 sg_MainWindEvtt;

static ptu32_t __GDD_DefWindowProcNCPAINT(struct WindowMsg *pMsg);
static ptu32_t __GDD_DefWindowProcERASEBKGND(struct WindowMsg *pMsg);
static ptu32_t __GDD_DefWindowProcPAINT(struct WindowMsg *pMsg);
static ptu32_t __GDD_DefWindowProcCLOSE(struct WindowMsg *pMsg);
static ptu32_t __GDD_DefWindowProcSYNC(struct WindowMsg *pMsg);

//Ĭ�ϴ�����Ϣ�������������û����ڹ���û�д������Ϣ��
static struct MsgProcTable s_gDefWindowMsgProcTable[] =
{
    {MSG_NCPAINT,__GDD_DefWindowProcNCPAINT},
    {MSG_ERASEBKGND,__GDD_DefWindowProcERASEBKGND},
    {MSG_PAINT,__GDD_DefWindowProcPAINT},
    {MSG_SYNC_DISPLAY,__GDD_DefWindowProcSYNC},
    {MSG_CLOSE,__GDD_DefWindowProcCLOSE},
//  {MSG_DESTROY,DefWindowProc_DESTROY},
};

static struct MsgTableLink  s_gDefWindowMsgLink;

//----ȡ���ھ��---------------------------------------------------------------
//���ܣ�����gkwinָ�룬ȥ������gdd���ھ��
//������gkwin��gkwinָ��
//���أ�gdd���ھ��
//---------------------------------------------------------------------------
HWND GDD_GetHwnd(struct GkWinObj *gkwin)
{
    return (HWND)GK_GetUserTag(gkwin);
}

//----��Ļ����ת�ͻ�����---------------------------------------------------------
//����: ��һ����Ļ����ֵ��ת��Ϊ���ڿͻ������������
//������hwnd:���ھ��
//      pt:  ��Ҫת������Ļ�����ָ��
//      count: ��Ҫת�������������
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
bool_t    GDD_ScreenToClient(HWND hwnd,POINT *pt,s32 count)
{
    s32 i;
    if(NULL!=pt && hwnd!=NULL)
    {
        if(__HWND_Lock(hwnd))
        {
            for(i=0;i<count;i++)
            {
                pt[i].x -= hwnd->CliRect.left;
                pt[i].y -= hwnd->CliRect.top;
            }
            __HWND_Unlock(hwnd);
            return TRUE;
        }
    }
    return  FALSE;
}

//----�ͻ�����ת��Ļ����---------------------------------------------------------
//����: ��
//������hwnd:���ھ��
//      pt:  ��Ҫת���������ָ��
//      count: ��Ҫת�������������
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
bool_t    GDD_ClientToScreen(HWND hwnd,POINT *pt,s32 count)
{
    s32 i;
    if(NULL!=pt)
    {
        if(__HWND_Lock(hwnd))
        {
            for(i=0;i<count;i++)
            {
                pt[i].x += hwnd->CliRect.left;
                pt[i].y += hwnd->CliRect.top;
            }
            __HWND_Unlock(hwnd);
            return  TRUE;
        }

    }
    return  FALSE;
}

//----��Ļ����ת��������---------------------------------------------------------
//����: ��
//������hwnd:���ھ��
//      pt:  ��Ҫת���������ָ��
//      count: ��Ҫת�������������
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
bool_t    GDD_ScreenToWindow(HWND hwnd,POINT *pt,s32 count)
{
    s32 i;
    RECT rc;
    if(NULL!=pt)
    {
        if(__HWND_Lock(hwnd))
        {
            GK_GetArea(hwnd->pGkWin, &rc);
            for(i=0;i<count;i++)
            {
                pt[i].x -= rc.left;
                pt[i].y -= rc.top;
            }
            __HWND_Unlock(hwnd);
            return  TRUE;
        }
    }
    return  FALSE;
}

//----��������ת��Ļ����---------------------------------------------------------
//����: ��
//������hwnd:���ھ��
//      pt:  ��Ҫת���������ָ��
//      count: ��Ҫת�������������
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
bool_t    GDD_WindowToScreen(HWND hwnd,POINT *pt,s32 count)
{
    s32 i;
    RECT rc;
    if(NULL!=pt)
    {
        if(__HWND_Lock(hwnd))
        {
            GK_GetArea(hwnd->pGkWin, &rc);
            for(i=0;i<count;i++)
            {
                pt->x += rc.left;
                pt->y += rc.top;
            }
            __HWND_Unlock(hwnd);
            return TRUE;
        }
    }
    return FALSE;
}


//----������洰�ھ��-----------------------------------------------------------
//����: ��
//��������
//���أ����洰�ھ��.
//------------------------------------------------------------------------------
HWND    GDD_GetDesktopWindow(char *display)
{
    HWND hwnd;

    hwnd =NULL;
    if(__HWND_Lock(HWND_Desktop))
    {
        hwnd =HWND_Desktop;
        __HWND_Unlock(HWND_Desktop);
    }
    return hwnd;
}

//----��ô��ھ�����-----------------------------------------------------------
//����: ��ô��ھ�����,����Ϊ��Ļ����.
//������hwnd:���ھ��
//      prc:���ڱ���������ݵ�ָ��.
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
bool_t    GDD_GetWindowRect(HWND hwnd,RECT *prc)
{
    if(NULL!=prc)
    if(__HWND_Lock(hwnd))
    {
        GK_GetArea(hwnd->pGkWin, prc);
        __HWND_Unlock(hwnd);
        return TRUE;
    }
    return FALSE;
}

//----��ô��ڿͻ�������-------------------------------------------------------
//����: ��ô��ڿͻ�������,����Ϊ�ͻ����꣬�Կͻ���������Ͻ�Ϊԭ��
//������hwnd:���ھ��
//      prc:���ڱ���������ݵ�ָ��.
//���أ�TRUE:�ɹ�; FALSE:ʧ��
//------------------------------------------------------------------------------
bool_t    GDD_GetClientRect(HWND hwnd,RECT *prc)
{
    if(NULL!=prc)
    if(__HWND_Lock(hwnd))
    {
        prc->left =0;
        prc->top =0;
        prc->right =GDD_RectW(&hwnd->CliRect);
        prc->bottom =GDD_RectH(&hwnd->CliRect);
        __HWND_Unlock(hwnd);
        return TRUE;
    }
    return FALSE;
}

//----��ô��ڿͻ���������ת��Ϊ��Ļ����---------------------------------------
//����: ��
//������hwnd:���ھ��
//      prc:���ڱ���������ݵ�ָ��.
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
bool_t    GDD_GetClientRectToScreen(HWND hwnd,RECT *prc)
{
    if(NULL!=prc)
    {
        if(__HWND_Lock(hwnd))
        {
            GDD_CopyRect(prc,&hwnd->CliRect);
            __HWND_Unlock(hwnd);
            return TRUE;
        }
    }
    return FALSE;
}

//----��ô�����Ϣ����-----------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������Ϸ���,���̰߳�ȫ
//������hwnd:���ھ��
//���أ�������Ϣ����ָ��.
//------------------------------------------------------------------------------
struct WinMsgQueueCB*   __GDD_GetWindowMsgQ(HWND hwnd)
{
    return hwnd->pMsgQ;
}

//----��ô����߳�Id-------------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������Ϸ���,���̰߳�ȫ
//������hwnd:���ھ��
//���أ������߳�Id.
//------------------------------------------------------------------------------
u32 __GDD_GetWindowEvent(HWND hwnd)
{
    return hwnd->EventID;
}

//----��ô��ڷ��-------------------------------------------------------------
//����: ȡ���ڷ����һ��32λ���룬���и�16bit���ڹ������0~7bit�������ֿؼ�
//      ���ͣ�8~15bit���ڿؼ����
//������hwnd:���ھ��
//���أ����ڷ��.
//------------------------------------------------------------------------------
u32 GDD_GetWindowStyle(HWND hwnd)
{
    u32 style=0;

    if(__HWND_Lock(hwnd))
    {
        style =hwnd->Style;
        __HWND_Unlock(hwnd);
    }
    return style;
}

//----��ô���˽������-----------------------------------------------------------
//����: ��
//������hwnd:���ھ��
//���أ�����˽������.
//------------------------------------------------------------------------------
ptu32_t GDD_GetWindowPrivateData(HWND hwnd)
{
    ptu32_t data;

    data=0;
    if(__HWND_Lock(hwnd))
    {
        data = hwnd->PrivateData;
        __HWND_Unlock(hwnd);
    }
    return data;
}

//----���ô���˽������-----------------------------------------------------------
//����: ��
//������hwnd:���ھ��
//      data: ˽������
//���أ���.
//------------------------------------------------------------------------------
void GDD_SetWindowPrivateData(HWND hwnd,ptu32_t data)
{
    if(__HWND_Lock(hwnd))
    {
        hwnd->PrivateData=data;
        __HWND_Unlock(hwnd);
    }
}

//----ȡ�����ھ��-------------------------------------------------------------
//���ܣ����������ڵı��ⷵ�ش��ھ����ע�⣬�����ڱ����ǲ����ظ��ģ����Ӵ��ڿ���
//      �ᣬ�ʱ�����ֻ��ȡ�����ڵľ����
//������WinText�����ڱ��⣬��gkwin��win_name��ͬ
//���أ������ھ�������򷵻�NULL
//-----------------------------------------------------------------------------
HWND GDD_GetWindowHandle(char *WinText)
{
    HWND wnd,first,result = NULL;
    wnd = GDD_GetWindowChild(GDD_GetDesktopWindow(NULL ));
    if(wnd == NULL)
        return NULL;
    first = wnd;
    if(__GDD_Lock( ))
    {
        do
        {
            if(strcmp(WinText, wnd->Text) == 0)
            {
                result = wnd;
                break;
            }
            wnd = GDD_GetWindowNext(wnd);
        } while( wnd != first);
        __GDD_Unlock( );
    }
    return result;
}

//----ȡ������-----------------------------------------------------------------
//���ܣ�ȡ���ڵĸ����ھ��
//������hwnd�����ھ��
//���أ������ھ�������򷵻�NULL
//-----------------------------------------------------------------------------
HWND GDD_GetWindowParent(HWND hwnd)
{
    HWND wnd=NULL;

    if(__GDD_Lock( ))
    {
        wnd = (HWND)GK_GetUserTag(GK_GetParentWin(hwnd->pGkWin));
        __GDD_Unlock( );
    }
    return wnd;
}

//----ȡ�Ӵ���-----------------------------------------------------------------
//���ܣ�ȡ���ڵ��Ӵ��ھ��,ʵ�ʸ� GDD_GetWindowFirst ��Ч
//������hwnd�����ھ��
//���أ��Ӵ��ھ�������򷵻�NULL
//-----------------------------------------------------------------------------
HWND GDD_GetWindowChild(HWND hwnd)
{
    HWND wnd=NULL;

    if(__GDD_Lock( ))
    {
        wnd = (HWND)GK_GetUserTag(GK_GetChildWin(hwnd->pGkWin));
        __GDD_Unlock( );
    }
    return wnd;
}

//----ȡĩ�Ҵ���---------------------------------------------------------------
//���ܣ��Ӻ��������ȡһ���������Ӵ��ڵĴ��ھ��
//������hwnd�����ھ��
//���أ��Ӵ��ھ�������򷵻�NULL
//-----------------------------------------------------------------------------
HWND __GDD_GetWindowTwig(HWND hwnd)
{
    HWND wnd=NULL;

    if(__GDD_Lock( ))
    {
        wnd = (HWND)GK_GetUserTag(GK_GetTwig(hwnd->pGkWin));
        __GDD_Unlock( );
    }
    return wnd;
}

//----ȡһ��ǰ����-------------------------------------------------------------
//���ܣ�ȡ���ڵ�ǰ���ھ��
//������hwnd�����ھ��
//���أ�ǰ���ھ�������򷵻�NULL
//-----------------------------------------------------------------------------
HWND GDD_GetWindowPrevious(HWND hwnd)
{
    HWND wnd=NULL;

    if(__GDD_Lock( ))
    {
        wnd = (HWND)GK_GetUserTag(GK_GetPreviousWin(hwnd->pGkWin));
        __GDD_Unlock( );
    }
    return wnd;
}

//----ȡһ���󴰿�-------------------------------------------------------------
//���ܣ�ȡ���ڵĺ󴰿ھ��
//������hwnd�����ھ��
//���أ��󴰿ھ�������򷵻�NULL
//-----------------------------------------------------------------------------
HWND GDD_GetWindowNext(HWND hwnd)
{
    HWND wnd=NULL;

    if(__GDD_Lock( ))
    {
        wnd = (HWND)GK_GetUserTag(GK_GetNextWin(hwnd->pGkWin));
        __GDD_Unlock( );
    }
    return wnd;
}

//----ȡһ����ǰ����-------------------------------------------------------------
//���ܣ�ȡ����ͬ�������е���ǰ���ھ��
//������hwnd�����ھ��
//���أ���ǰ���ھ�������򷵻�NULL
//-----------------------------------------------------------------------------
HWND GDD_GetWindowFirst(HWND hwnd)
{
    HWND wnd=NULL;

    if(__GDD_Lock( ))
    {
        wnd = (HWND)GK_GetUserTag(GK_GetFirstWin(hwnd->pGkWin));
        __GDD_Unlock( );
    }
    return wnd;
}

//----ȡһ����󴰿�-------------------------------------------------------------
//���ܣ�ȡ���ڵ�ͬ����������󴰿ھ��
//������hwnd�����ھ��
//���أ���󴰿ھ�������򷵻�NULL
//-----------------------------------------------------------------------------
HWND GDD_GetWindowLast(HWND hwnd)
{
    HWND wnd=NULL;

    if(__GDD_Lock( ))
    {
        wnd = (HWND)GK_GetUserTag(GK_GetLastWin(hwnd->pGkWin));
        __GDD_Unlock( );
    }
    return wnd;
}

//----ȡһ����󴰿�-------------------------------------------------------------
//���ܣ�ȡ���ڵ�ͬ����������󴰿ھ��
//������hwnd�����ھ��
//���أ���󴰿ھ�������򷵻�NULL
//-----------------------------------------------------------------------------
char *GDD_GetWindowText(HWND hwnd)
{
    return hwnd->Text;
}

//----��Id���Ҵ����е��Ӵ���(�ؼ�)-----------------------------------------------
//����: ��.
//������hwnd:���ھ��
//     Id: ��Ҫ���ҵ��Ӵ���(�ؼ�)Id.
//���أ����ҵ����Ӵ���(�ؼ�)���.
//------------------------------------------------------------------------------
//����ϵ� winID��������������߼���
//HWND    GDD_GetDlgItem(HWND hwnd,u16 Id)
//{
//    HWND Current = hwnd;
//    if(__GDD_Lock())
//    {
//        Current = (HWND)GK_GetUserTag(GK_TraveChild(hwnd->pGkWin,Current->pGkWin));
//        while(Current != NULL)
//        {
//            if(Current->WinId == Id)
//                break;
//            Current = (HWND)GK_GetUserTag(GK_TraveChild(hwnd->pGkWin,Current->pGkWin));
//        }
//        __GDD_Unlock();
//    }
//    return Current;
//}

//----��������Ҵ���(�����ڻ��Ӵ���)-------------------------------------------
//����: ��.
//������pt:�����
//���أ����ҵ��Ĵ��ھ��.
//------------------------------------------------------------------------------
HWND    GDD_GetWindowFromPoint(struct GkWinObj *desktop, POINT *pt)
{
    struct GkWinObj *GkWin = NULL;
    if((NULL == desktop) || (NULL == pt))
        return NULL;
    if(__GDD_Lock( ))
    {
        GkWin = GK_GetWinFromPt(desktop, pt);
        __GDD_Unlock( );
    }
    if(GkWin != NULL)
        return (HWND)GK_GetUserTag(GkWin);
    else
        return NULL;
}

//----���ô�������-------------------------------------------------------------
//����: ��.
//������hwnd: �����õĴ���
//      NewName���µ�����
//���أ���
//------------------------------------------------------------------------------
void GDD_SetWindowName(HWND hwnd, char *NewName)
{
    GK_SetName(hwnd->pGkWin, NewName);
    return ;
}

//----��ʼ���������ݽṹ---------------------------------------------------------
//����: �ú���Ϊ�ڲ�����.
//������pwin:�������ݽṹָ��.
//      proc:���ڹ��̻ص�����ָ�룮
//      Text:������������ָ��.
//      Style:���ڷ��.
//      x,y,w,h:����λ�úʹ�С.
//      hParent:�����ھ��.
//      WinId: ����Id.
//���أ���.
//------------------------------------------------------------------------------
static  void __GDD_InitWindow(HWND pwin,u32 Style,u32 WinId)
{
    RECT rc;

    pwin->Style     =Style;
    pwin->WinId     =WinId&0x0000FFFF;
    pwin->Text = pwin->pGkWin->win_name;
    dListInit(&pwin->list_timer);
    dListInit(&pwin->node_msg_close);
    dListInit(&pwin->node_msg_ncpaint);
    dListInit(&pwin->node_msg_paint);
    GK_GetArea(pwin->pGkWin, &rc);

    if(Style&WS_BORDER)
    {
        pwin->BorderSize =DEF_BORDER_SIZE;
        __GDD_InflateRectEx(&rc, -DEF_BORDER_SIZE,
                            -DEF_BORDER_SIZE,
                            -DEF_BORDER_SIZE,
                            -DEF_BORDER_SIZE);

    }
    else
    {
        pwin->BorderSize =0;
    }

    if(Style&WS_DLGFRAME)
    {
//      pwin->DlgFrameSize =DEF_DLGFRAME_SIZE;
        __GDD_InflateRectEx(&rc, -DEF_DLGFRAME_SIZE,
                            -DEF_DLGFRAME_SIZE,
                            -DEF_DLGFRAME_SIZE,
                            -DEF_DLGFRAME_SIZE);
    }
//    else
//    {
//        pwin->DlgFrameSize =0;
//    }

    if(Style&WS_CAPTION)
    {
        pwin->CaptionSize =DEF_CAPTION_SIZE;
        __GDD_InflateRectEx(&rc,0,-DEF_CAPTION_SIZE,0,0);
    }
    else
    {
        pwin->CaptionSize =0;
    }
    GDD_CopyRect(&pwin->CliRect,&rc);

}


//������ƺ���
bool_t GDD_DesktopPaint(struct WindowMsg *pMsg)
{
    HDC hdc;
    RECT rc;

    HWND hwnd=pMsg->hwnd;
    hdc =GDD_BeginPaint(hwnd);
    if(hdc == NULL)
        return false;
    GDD_GetClientRect(hwnd,&rc);

    GDD_GradientFillRect(hdc,&rc,RGB(120,120,255),RGB(20,20,80),CN_FILLRECT_MODE_UD);

    GDD_EndPaint(hwnd,hdc);
    return true;
}

void GDD_HmiInput(void);


//----GDD����ʱ������--------------------------------------------------------
//���ܣ�GDD����ʱ���Ļص��������ö�ʱ����GDD��ʼ��ʱ������Ŀǰֻ����ɨ���˻�����
//      ���룬������ꡢ���̵ȡ����ϵͳû�������豸��Ҳ���Բ�������
//������pMsg����Ϣָ��
//���أ��̶�true
//-----------------------------------------------------------------------------
static bool_t __GDD_ServerTimerHandle(struct WindowMsg *pMsg)
{
    HWND hwnd;
    struct WinTimer *pTmr;
    u16 TmrId;
    if(pMsg==NULL)
        return false;
    hwnd=pMsg->hwnd;
    if(hwnd==NULL)
        return false;
    TmrId=pMsg->Param1;
    pTmr=GDD_FindTimer(hwnd,TmrId);
    if(pTmr==NULL)
        return false;
    switch(TmrId)
    {
        case CN_HMIINPUT_TIMER_ID:
            GDD_HmiInput( );
            break;
       default:
          break;
    }

    return true;
}

//������Ϣ��������
static struct MsgProcTable s_gDesktopMsgProcTable[] =
{
    {MSG_PAINT,GDD_DesktopPaint},
    {MSG_TIMER,__GDD_ServerTimerHandle},
};

ptu32_t __GDD_MessageLoop( void );
static struct MsgTableLink  s_gDesktopMsgLink;

//=======================================================
//��ȡ��ǰ��Ϣ�����s_gDefWindowMsgLink��ʼ�����
//=====================================================
static u32 __GDD_GetNumberOfPrev(HWND Hwnd)
{
    u32 num = 0;
    if(Hwnd == NULL)
        return num;

    while(Hwnd->MyMsgTableLink[num++] != &s_gDefWindowMsgLink);

    return num;
}

//----��ʼ�����洰��-----------------------------------------------------------
//����: ��ʼ��gdd�����洰��
//������desktop,���ʼ��������ָ��
//���أ����洰�ھ��.
//------------------------------------------------------------------------------
HWND    GDD_InitGddDesktop(struct GkWinObj *desktop)
{
    HWND pGddWin=NULL;
    u32 Style;
    u16 MyEvtt;
    u16 MyEventid;

    pGddWin = Mb_Malloc(g_ptHwndPool, CN_TIMEOUT_FOREVER);
    if(NULL!=pGddWin)
    {
        memset(pGddWin, 0, sizeof(struct WINDOW));
        Style = WS_VISIBLE | WS_CAN_FOCUS;
//      s_gDesktopMsgLink.LinkNext = NULL;
//      s_gDesktopMsgLink.MsgNum = sizeof(s_gDesktopMsgProcTable) / sizeof(struct MsgProcTable);
//      s_gDesktopMsgLink.myTable = s_gDesktopMsgProcTable;
        pGddWin->pGkWin     = desktop;
        pGddWin->PrivateData = 0;
        pGddWin->MyMsgTableLink = (struct MsgTableLink **)\
                                           malloc(2*sizeof( struct MsgTableLink *));

        if( pGddWin->MyMsgTableLink ==NULL)
        {
            Mb_Free(g_ptHwndPool,pGddWin);
           return NULL;
        }
        else
        {
            pGddWin->MyMsgTableLink[0] = &s_gDesktopMsgLink;
            pGddWin->MyMsgTableLink[1] = &s_gDefWindowMsgLink;
        }

//      dListInit(&pGddWin->MsgProcFuncTable);
//
      s_gDefWindowMsgLink.MsgNum = sizeof(s_gDefWindowMsgProcTable)
                                      / sizeof(struct MsgProcTable);
      s_gDefWindowMsgLink.myTable = s_gDefWindowMsgProcTable;
//      dListInsertAfter(&pGddWin->MsgProcFuncTable,
//                          &s_gDefWindowMsgLink.TableLink);
//
      s_gDesktopMsgLink.MsgNum = sizeof(s_gDesktopMsgProcTable)
                                      / sizeof(struct MsgProcTable);
      s_gDesktopMsgLink.myTable = s_gDesktopMsgProcTable;
//      dListInsertAfter(&pGddWin->MsgProcFuncTable,
//                          &s_gDesktopMsgLink.TableLink);

        pGddWin->mutex_lock =Lock_MutexCreate(NULL);
        pGddWin->pMsgQ      =__GUI_CreateMsgQ(32);
        pGddWin->DrawColor = CN_DEF_DRAW_COLOR;
        pGddWin->FillColor = CN_DEF_FILL_COLOR;
        pGddWin->TextColor = CN_DEF_TEXT_COLOR;
        if((pGddWin->mutex_lock==NULL)||(pGddWin->pMsgQ==NULL))
        {
            if(pGddWin->mutex_lock!=NULL)
            {
                Lock_MutexDelete(pGddWin->mutex_lock);
            }
            if(pGddWin->pMsgQ!=NULL)
            {
                __GUI_DeleteMsgQ(pGddWin->pMsgQ);
            }
            Mb_Free(g_ptHwndPool,pGddWin);
            return NULL;
        }

        GK_SetUserTag(desktop,pGddWin);
        __GDD_InitWindow(pGddWin,Style,0);
        HWND_Desktop = pGddWin;

        MyEvtt = DJY_EvttRegist(EN_CORRELATIVE, CFG_GUI_RUN_PRIO, 0, 0, __GDD_MessageLoop,
                                NULL, 1024, "desktop");
        if(MyEvtt != CN_EVTT_ID_INVALID)
        {
            MyEventid=DJY_EventPop(MyEvtt, NULL, 0, (ptu32_t)pGddWin, 0, 0);
            if(MyEventid!= CN_EVENT_ID_INVALID)
            {
//              pGddWin->EventID   = MyEventid;
                GDD_SetWindowShow(pGddWin); //��ʾ����
            }
            else
            {
                DJY_EvttUnregist( MyEvtt );
                Mb_Free(g_ptHwndPool,pGddWin);
                pGddWin = NULL;
            }
        }
        else
        {
            Mb_Free(g_ptHwndPool,pGddWin);
            pGddWin = NULL;
        }
        GDD_SendMessage(pGddWin,MSG_CREATE,0,0);
    }
    return pGddWin;
}

//----������Ϣ������-----------------------------------------------------------
//���ܣ�ÿ�����ڶ���һ����Ϣ�������������struct MsgTableLink��������ӵ���
//      Ϣ����������ԭ�е�������һ��ϵͳ�ڲ�����Ϣ������ʱ���������Ǻ��
//      �ȳ��ģ���������ĺ����������ȱ����ҡ�
//������hwnd���������Ĵ��ھ��
//      pNewMsgTableLink������ӵ���Ϣ��
//���أ���
// ----------------------------------------------------------------------------
void GDD_AddProcFuncTable(HWND hwnd,struct MsgTableLink *pNewMsgTableLink)
{
    u32 num;
    u32 i;
    struct MsgTableLink ** tab;
    if(pNewMsgTableLink==NULL || hwnd==NULL)
        return;
    num =  __GDD_GetNumberOfPrev(hwnd);
    tab = (struct MsgTableLink **)malloc((num+1)*sizeof( struct MsgTableLink *));
    if(tab==NULL)
        return;
    if(__HWND_Lock(hwnd))
    {
        tab[0] = pNewMsgTableLink;
        for(i=0;i<num;i++)
            tab[i+1] = hwnd->MyMsgTableLink[i];
        free(hwnd->MyMsgTableLink);
        hwnd->MyMsgTableLink = tab;
        __HWND_Unlock(hwnd);
    }

}

HWND __GDD_CreateWindow(struct GkWinObj *pGkWin,u32 Style,
                     HWND hParent,u16 WinId,bool_t mainwin,
                     ptu32_t pdata,
                     struct MsgTableLink *pUserMsgTableLink)
{
    HWND pGddWin=NULL;
    u32 num;
    bool_t unfill;

    unfill = (Style & WS_UNFILL)==WS_UNFILL;

    if(__GDD_Lock())
    {
//      pGddWin=M_Malloc(sizeof(struct WINDOW) + sizeof(struct GkWinObj),100*mS);
        pGddWin = Mb_Malloc(g_ptHwndPool, CN_TIMEOUT_FOREVER);
        if(NULL!=pGddWin)
        {
            memset(pGddWin, 0, sizeof(struct WINDOW));
            if(!mainwin)
            {
                pGddWin->Flag &= ~WF_MAINWIN;
                pGddWin->EventID   = hParent->EventID;
                pGddWin->mutex_lock =hParent->mutex_lock;  //�������ʹ�ø�������
                pGddWin->pMsgQ      =hParent->pMsgQ;       //�������ʹ�ø�������Ϣ����
                pGddWin->DrawColor = hParent->DrawColor;
                pGddWin->FillColor = hParent->FillColor;
                pGddWin->TextColor = hParent->TextColor;
            }
            else
            {
                pGddWin->Flag |= WF_MAINWIN;
                pGddWin->mutex_lock =Lock_MutexCreate(NULL);
                pGddWin->pMsgQ      =__GUI_CreateMsgQ(32);
                pGddWin->DrawColor = CN_DEF_DRAW_COLOR;
                pGddWin->FillColor = CN_DEF_FILL_COLOR;
                pGddWin->TextColor = CN_DEF_TEXT_COLOR;

                if((pGddWin->mutex_lock==NULL)||(pGddWin->pMsgQ==NULL))
                {
                    Lock_MutexDelete(pGddWin->mutex_lock);
                    __GUI_DeleteMsgQ(pGddWin->pMsgQ);
                    __GDD_Unlock();
                    return NULL;
                }
            }
            pGddWin->pGkWin = pGkWin;
            pGddWin->PrivateData = pdata;

            num = (pUserMsgTableLink != NULL)? 2:1;
            pGddWin->MyMsgTableLink = (struct MsgTableLink **)\
                        malloc(num*sizeof( struct MsgTableLink *));

            if(pGddWin->MyMsgTableLink == NULL)
            {
                Lock_MutexDelete(pGddWin->mutex_lock);
                __GUI_DeleteMsgQ(pGddWin->pMsgQ);
                __GDD_Unlock( );
                return NULL;
            }
            else
            {
               if(num==1)
               {
                   pGddWin->MyMsgTableLink[0] = &s_gDefWindowMsgLink;
               }
               else
               {
                   pGddWin->MyMsgTableLink[0] = pUserMsgTableLink;
                   pGddWin->MyMsgTableLink[1] = &s_gDefWindowMsgLink;

               }
            }
            GK_SetUserTag(pGkWin,pGddWin);
            //��ʼ����������
            __GDD_InitWindow(pGddWin,Style,WinId);
        }
        __GDD_Unlock();

        if(NULL!=pGddWin)
        {
//          GDD_SendMessage(pGddWin,MSG_CREATE,(u32)pdata,0);
            GDD_PostMessage(pGddWin,MSG_CREATE,(u32)pdata,0);
            GDD_PostMessage(pGddWin,MSG_NCPAINT,0,0);
            GDD_InvalidateWindow(pGddWin,!unfill);
        }

    }
    return pGddWin;
}

//----��������-----------------------------------------------------------------
//����: �������ڣ��������Ĵ���Ϊ�����ڵĸ������ڣ�ʹ�ø����ڵ���Ϣ���к���Ϣѭ����
//������Text: �������֣���copy��gkwin��name��Ա��������CN_GKWIN_NAME_LIMIT��
//              ���ֽ�������TODO������Ϊ�ַ�����
//      Style: ���ڷ��(�ο� WS_VISIBLE �峣��)
//      x,y,w,h: ����λ�úʹ�С,λ������ڸ�����
//      hParent: �����ھ��.�����NULL,��Ĭ������Ϊ������.
//      WinId: ����Id.�������������ϳ�����Ҫʹ��
//      BufProperty�����ڵ�buf���ԣ�ȡֵΪ CN_WINBUF_PARENT �ȡ�
//      pdata: �û��Զ��帽������.//
//      PixelFormat,���ظ�ʽ����gkernel.h�ж��壬 CN_SYS_PF_DISPLAY ��ʾ����ʾ����ͬ���Ƽ���
//      BaseColor, �Ҷ�ͼ��ɫ��(����PixelFormat == CN_SYS_PF_GRAY1 ~8ʱ����)
//      pUserMsgTableLink:������Ӧ����Ϣ��
//���أ����ھ��.
//-----------------------------------------------------------------------------
HWND GDD_CreateWindow(const char *Text,u32 Style,
                     s32 x,s32 y,s32 w,s32 h,
                     HWND hParent,u16 WinId,
                     u32 BufProperty,ptu32_t pdata,
                     u16 PixelFormat,u32 BaseColor,
                     struct MsgTableLink *pUserMsgTableLink)
{
    struct GkWinObj *pGkWin;
    HWND pGddWin;
    struct RopGroup RopCode = (struct RopGroup){ 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };
    if(NULL==hParent)
    {
        hParent = HWND_Desktop;
    }
    pGkWin = GK_CreateWin(hParent->pGkWin,x,y,x+w,y+h,
                        RGB(0,0,0), BufProperty, Text,
                        PixelFormat, 0,BaseColor,RopCode);
    if(NULL != pGkWin)
    {
        pGddWin = __GDD_CreateWindow(pGkWin, Style,hParent,WinId,false,pdata,pUserMsgTableLink);
        if(pGddWin == NULL)
        {
            GK_DestroyWin(pGkWin);
        }
        return pGddWin;
    }
    else
    {
        return NULL;
    }
}

//�� GDD_CreateWindow ������ȣ�����һ����ͬ�����ڴ��������ڡ�
HWND __GDD_CreateMainWindow(const char *Text,u32 Style,
                     s32 x,s32 y,s32 w,s32 h,
                     HWND hParent,u16 WinId,
                     u32 BufProperty,ptu32_t pdata,
                     u16 PixelFormat,u32 BaseColor,
                     struct MsgTableLink *pUserMsgTableLink)
{
    struct GkWinObj *pGkWin;
    HWND pGddWin;
    struct RopGroup RopCode = (struct RopGroup){ 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };
    if(NULL==hParent)
    {
        hParent = HWND_Desktop;
    }
    pGkWin = GK_CreateWin(hParent->pGkWin,x,y,x+w,y+h,
                        RGB(0,0,0), BufProperty, Text,
                        PixelFormat, 0,BaseColor,RopCode);
    if(NULL != pGkWin)
    {
        // �� GDD_CreateWindow ������ȣ����˴���ͬ������������� true��
        pGddWin = __GDD_CreateWindow(pGkWin, Style,hParent,WinId,true,pdata,pUserMsgTableLink);
        if(pGddWin == NULL)
        {
            GK_DestroyWin(pGkWin);
        }
        return pGddWin;
    }
    else
    {
        return NULL;
    }
}

//----ɾ��һ���Ӵ������ݽṹ---------------------------------------------------
//����:�ú���Ϊ�ڲ�����,�����ͷŴ��ڽṹ�ڴ�,�����Լ��Ը�����ȥ�Ƴ�.
//��������Ҫɾ�����Ӵ��ھ��
//���أ���.
//------------------------------------------------------------------------------
void __GDD_DeleteChildWindowData(HWND hwnd)
{

    dListRemove(&hwnd->node_msg_close);
    dListRemove(&hwnd->node_msg_ncpaint);
    dListRemove(&hwnd->node_msg_paint);
    __GDD_RemoveWindowTimer(hwnd);
//  __GUI_DeleteMsgQ(hwnd->pMsgQ);      //�Ӵ��ڹ�����������Ϣ����
//  Lock_MutexDelete(hwnd->mutex_lock); //�Ӵ���û��˽�е� mutex_lock,�����ͷ�.

    free(hwnd->MyMsgTableLink);
    memset(hwnd, 0, sizeof(struct WINDOW));
    Mb_Free(g_ptHwndPool,hwnd);

}

//----ɾ��һ�����������ݽṹ---------------------------------------------------
//����:�ú���Ϊ�ڲ�����,�����ͷŴ��ڽṹ�ڴ�,�����Լ��Ը�����ȥ�Ƴ�.
//��������Ҫɾ���������ھ��
//���أ���.
//------------------------------------------------------------------------------
void __GDD_DeleteMainWindowData(HWND hwnd)
{
    dListRemove(&hwnd->node_msg_close);
    dListRemove(&hwnd->node_msg_ncpaint);
    dListRemove(&hwnd->node_msg_paint);
    __GDD_RemoveWindowTimer(hwnd);
    __GUI_DeleteMsgQ(hwnd->pMsgQ);
    Lock_MutexDelete(hwnd->mutex_lock);

    free(hwnd->MyMsgTableLink);
    memset(hwnd, 0, sizeof(struct WINDOW));
    Mb_Free(g_ptHwndPool,hwnd);

}

//----����һ������--------------------------------------------------------------
//����: �����������ں��Ӵ���,ͨ�������ڷ���close��Ϣ��ʵ�֡�close����ʱ����ͬʱ
//      ���ٺ�����ڡ�
//������hwnd:��Ҫ���ٵĴ��ھ��
//���أ���.
//------------------------------------------------------------------------------
void    GDD_DestroyWindow(HWND hwnd)
{
    GDD_PostMessage(hwnd, MSG_CLOSE, 0, 0);
}

//----����ȫ���Ӵ���-----------------------------------------------------------
//����: ��hwnd���ڱ��������⣬����ͬDestroyWindow
//������hwnd:��Ҫ���ٵĴ��ھ��
//���أ���.
//------------------------------------------------------------------------------
void GDD_DestroyAllChild(HWND hwnd)
{
    HWND Current = hwnd;

    if(__GDD_Lock())
    {
        Current = (HWND)GK_GetUserTag(GK_TraveChild(hwnd->pGkWin,Current->pGkWin));
        while(Current != NULL)
        {
//            GDD_SetWindowHide(Current);     //MSG_CLOSE��Ϣ�������ģ������ش���
          GDD_PostMessage(Current, MSG_CLOSE, 0, 0);
            Current = (HWND)GK_GetUserTag(GK_TraveChild(hwnd->pGkWin,Current->pGkWin));
        }
        __GDD_Unlock();
    }
//  GDD_PostMessage(hwnd, MSG_SYNC_DISPLAY,0,0);
    return ;
}

//----ƫ�ƴ���------------------------------------------------------------------
//����: ƫ��һ������λ��,�����Ӵ���.�ú���Ϊ�ڲ�����,���̰߳�ȫ
//������hwnd:���ھ��.
//      dx,dy: ˮƽ�ʹ�ֱ�����ƫ����.
//���أ���.
//------------------------------------------------------------------------------
static  void    __GDD_OffsetWindow(HWND hwnd,s32 dx,s32 dy)
{
    HWND wnd;
    RECT rc;
    struct GkWinObj *Ancestor, *Current;

    Ancestor = hwnd->pGkWin;
    Current = Ancestor;
    Current = GK_TraveScion(Ancestor,Current);
    __GDD_OffsetRect(&hwnd->CliRect,dx,dy);
    while(Current != NULL)
    {
        wnd = (HWND)GK_GetUserTag(Current);
        GDD_OffsetRect(&wnd->CliRect,dx,dy);
        Current = GK_TraveScion(Ancestor,Current);
    }
    GK_GetArea(hwnd->pGkWin,&rc);
    GK_MoveWin(  hwnd->pGkWin,rc.left+dx,rc.top+dy,0);
}

//----ƫ�ƴ���------------------------------------------------------------------
//����: ƫ��һ������λ��,�����Ӵ���.
//������hwnd:���ھ��.
//      dx,dy: ˮƽ�ʹ�ֱ�����ƫ����.
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
bool_t    GDD_OffsetWindow(HWND hwnd,s32 dx,s32 dy)
{
    if(hwnd == HWND_Desktop)
    {//���治�����ƶ�
        return false;
    }
    if(__GDD_Lock())
    {
        __GDD_OffsetWindow(hwnd,dx,dy);
        __GDD_Unlock();
        return true;
    }
    return FALSE;

}

//----�ƶ�����------------------------------------------------------------------
//����: �ƶ�һ������λ��,�����Ӵ���.�ú���Ϊ�ڲ�����.���̰߳�ȫ.
//������hwnd:���ھ��.
//      x,y: ����ڸ����ڿͻ�������λ��.
//���أ���.
//------------------------------------------------------------------------------
static void __GDD_MoveWindow(HWND hwnd,s32 x,s32 y)
{
    HWND wnd;
    struct GkWinObj *Ancestor, *Current;
    struct Rectangle rc;
    s32 dx,dy;
    POINT point;
    point.x=x;
    point.y=y;
    Ancestor = hwnd->pGkWin;
    Current = Ancestor;
    GK_GetArea(Ancestor, &rc);//��ʾ����,��ʾ���ľ�������
    wnd=GDD_GetWindowParent(hwnd);
    GDD_ScreenToClient(wnd,(POINT *)&rc,2);//ת��Ϊ�ͻ���
    dx = x - rc.left;
    dy = y - rc.top;
    Current = GK_TraveScion(Ancestor,Current);
    __GDD_OffsetRect(&hwnd->CliRect, dx, dy);
    while(Current != NULL)
    {
        wnd = (HWND)GK_GetUserTag(Current);
        __GDD_OffsetRect(&wnd->CliRect,dx,dy);
        Current = GK_TraveScion(Ancestor,Current);
    }
    GDD_ClientToScreen(wnd,&point,1);
    GDD_ScreenToWindow(wnd,&point,1);
    GK_MoveWin(  hwnd->pGkWin,point.x,point.y,0);
}

//----�ƶ�����------------------------------------------------------------------
//����: �ƶ�һ������λ��,�����Ӵ���.
//������hwnd:���ھ��.
//      x,y: ����ڸ����ڿͻ�������λ��.
//���أ�TRUE:�ɹ�;FALSE:ʧ��.
//------------------------------------------------------------------------------
bool_t    GDD_MoveWindow(HWND hwnd,s32 x,s32 y)
{
    if(hwnd == HWND_Desktop)
    {//���治�����ƶ�
        return false;
    }
    if(__GDD_Lock())
    {
        __GDD_MoveWindow(hwnd,x,y);
        __GDD_Unlock();
        return TRUE;
    }
    return FALSE;
}

//----�жϴ����Ƿ�ɼ�---------------------------------------------------------
//����: ��
//������hwnd:���ھ��.
//���أ�TRUE:���ڿɼ�;FALSE:���ڲ��ɼ�.
//------------------------------------------------------------------------------
bool_t    GDD_IsWindowVisible(HWND hwnd)
{
    bool_t res=FALSE;

//    if(__HWND_Lock(hwnd))
//    {
        res = GK_IsWinVisible(hwnd->pGkWin);
//        __HWND_Unlock(hwnd);
//    }
    return res;
}

//----���ô���Ϊ��Ч-----------------------------------------------------------
//����: ���ô���Ϊ��Ч����˼��:������Ҫ�ػ�.�����ػ���Ϣ���,�ú���Ϊ�ڲ�����.
//������hwnd:���ھ��.
//      bErase: �Ƿ��������.
//���أ���
//------------------------------------------------------------------------------
void    __GDD_InvalidateWindow(HWND hwnd,bool_t bErase)
{
    if(GDD_IsWindowVisible(hwnd))
    {
            GDD_PostMessage(hwnd,MSG_PAINT,bErase,0);
    }
}

//----���ô���Ϊ��Ч-------------------------------------------------------------
//����: ��
//������hwnd:���ھ��.
//      bErase��true= ��Ҫ����������false=����Ҫ��������
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
bool_t    GDD_InvalidateWindow(HWND hwnd,bool_t bErase)
{
//    if(__HWND_Lock(hwnd))
//    {
        __GDD_InvalidateWindow(hwnd,bErase);
//        __HWND_Unlock(hwnd);
        return TRUE;
//    }
//    return FALSE;
}


//----��ʾ����-------------------------------------------------------------
//����: ���ô���Ϊ��ʾ(�����Ӵ���)
//������hwnd:���ھ��.
//���أ�TRUE:�ɹ�;FALSE:ʧ��.
//------------------------------------------------------------------------------
bool_t GDD_SetWindowShow(HWND hwnd)
{
    if(__HWND_Lock(hwnd))
    {
        hwnd->Style |= WS_VISIBLE;
        GK_SetVisible(hwnd->pGkWin,CN_GKWIN_VISIBLE,0);
        __HWND_Unlock(hwnd);
        return TRUE;
    }
     return FALSE;
}
//----���ش���-------------------------------------------------------------
//����: ���ô���Ϊ����(�����Ӵ���)
//������hwnd:���ھ��.
//���أ�TRUE:�ɹ�;FALSE:ʧ��.
//------------------------------------------------------------------------------
bool_t GDD_SetWindowHide(HWND hwnd)
{
    if(__HWND_Lock(hwnd))
    {
        hwnd->Style &= ~ WS_VISIBLE;
        GK_SetVisible(hwnd->pGkWin,CN_GKWIN_HIDE,0);
        __HWND_Unlock(hwnd);
        return TRUE;
    }
    return FALSE;
}


bool_t GDD_SetWindowRopCode(HWND hwnd, struct RopGroup RopCode)
{
    if(hwnd==NULL)
         return FALSE;

     if(__HWND_Lock(hwnd))
     {
        GK_SetRopCode(hwnd->pGkWin,RopCode,0);
        __HWND_Unlock(hwnd);
        return TRUE;
     }
     return FALSE;
}

bool_t GDD_SetWindowHyalineColor(HWND hwnd,u32 HyalineColor)
{
    if(hwnd==NULL)
        return FALSE;
    if(__HWND_Lock(hwnd))
    {
        GK_SetHyalineColor(hwnd->pGkWin,HyalineColor);
        __HWND_Unlock(hwnd);
        return TRUE;
    }
    return FALSE;
}

bool_t GDD_SetWindowFillColor(HWND hwnd,u32 FillColor)
{
    HWND pGddWin=NULL;
    if(hwnd==NULL)
       return FALSE;
    if(__HWND_Lock(hwnd))
    {
        pGddWin=hwnd;
        pGddWin->FillColor=FillColor;
        __HWND_Unlock(hwnd);
        return TRUE;
    }
    return FALSE;
}

bool_t GDD_GetWindowFillColor(HWND hwnd,u32 *pFillColor)
{
    HWND pGddWin=NULL;
    if(hwnd==NULL)
       return FALSE;
    if(__HWND_Lock(hwnd))
    {
        pGddWin=hwnd;
        *pFillColor=pGddWin->FillColor;
        __HWND_Unlock(hwnd);
        return TRUE;
    }
    return FALSE;
}



bool_t GDD_SetWindowTextColor(HWND hwnd,u32 TextColor)
{
    HWND pGddWin=NULL;
    if(hwnd==NULL)
       return FALSE;
    if(__HWND_Lock(hwnd))
    {
        pGddWin=hwnd;
        pGddWin->TextColor=TextColor;
        __HWND_Unlock(hwnd);
        return TRUE;
    }
    return FALSE;
}

bool_t GDD_GetWindowTextColor(HWND hwnd,u32 *pTextColor)
{
    HWND pGddWin=NULL;
    if(hwnd==NULL)
       return FALSE;
    if(__HWND_Lock(hwnd))
    {
        pGddWin=hwnd;
        *pTextColor=pGddWin->TextColor;
        __HWND_Unlock(hwnd);
        return TRUE;
    }
    return FALSE;
}
//----��ô��ڻ�ͼ������(DC)-----------------------------------------------------
//����: �ú�����õ�DC,�������������ڷ�Χ�ڻ�ͼ���(�ǿͻ���+�ͻ���).������
//      GDD_ReleaseDC �����ɶԵ��ã�������ڴ�й©��
//������hwnd:���ھ��.��DC���´�������ʼ���ġ�
//���أ�DC���.
//------------------------------------------------------------------------------
HDC GDD_GetWindowDC(HWND hwnd)
{
    DC *pdc;

    pdc =malloc(sizeof(DC));
    if(pdc!=NULL)
    {
        memset(pdc, 0, sizeof(DC));
        GDD_InitDC(pdc,hwnd->pGkWin,hwnd,DC_TYPE_WINDOW);
    }
    return DC2HDC(pdc);

}

//----��ô��ڿͻ�����ͼ������(DC)-----------------------------------------------
//����: �ú�����õ�DC,ֻ���ڴ��ڿͻ�����Χ�ڻ�ͼ���).������
//      GDD_ReleaseDC �����ɶԵ��ã�������ڴ�й©��
//������hwnd:���ھ��.
//���أ�DC���.��DC���´�������ʼ���ġ�
//------------------------------------------------------------------------------
HDC GDD_GetDC(HWND hwnd)
{
    DC *pdc;

    pdc =malloc(sizeof(DC));
    if(pdc!=NULL)
    {
        memset(pdc, 0, sizeof(DC));
        GDD_InitDC(pdc,hwnd->pGkWin,hwnd,DC_TYPE_CLIENT);
    }
    return DC2HDC(pdc);

}

//----�ͷŴ��ڿͻ�����ͼ������(DC)-----------------------------------------------
//����: �ͷŴ��ڿͻ�����ͼ������(DC)
//������hdc�����.
//���أ��ɹ�=true��ʧ��=false
//------------------------------------------------------------------------------
bool_t    GDD_ReleaseDC(HDC hdc)
{
       return  GDD_DeleteDC(hdc);
}

//----��������------------------------------------------------------------------
//����: ��������,���ڶԴ��ڻ������,�ú�������TRUEʱ,�������HWND_Unlock����;
//      �����ú�������FALSE,���������HWND_Unlock.
//������hwnd:���ھ��
//���أ��ɹ�:TRUE; ʧ��:FLASE;
//------------------------------------------------------------------------------
bool_t    __HWND_Lock(HWND hwnd)
{
    bool_t result = false;
    if(__GDD_Lock())    //������סGDD�����ܻ�������
    {
        result = Lock_MutexPend(hwnd->mutex_lock, CN_TIMEOUT_FOREVER);
        __GDD_Unlock( );
    }
    return result;
}

//----��������------------------------------------------------------------------
//����: �����������ɹ���,�ɸú������н�������.
//������hwnd:���ھ��
//���أ���
//------------------------------------------------------------------------------
void __HWND_Unlock(HWND hwnd)
{
    Lock_MutexPost(hwnd->mutex_lock);
}

//----ָ�����ڿ�ʼ��ͼ-----------------------------------------------------------
//����: ��������ʼ��һ����ͼ���������hwnd�����Ծ����Ƿ���մ��ڣ��þ�����ڻ��ƴ��ڵ�
//      �ɻ��������ú���ֻ����MSG_PAINT�е���,������EndPaint�ɶ�ʹ��.������� GDD_PostMessage
//      ���� MSG_PAINT ��Ϣʱ��param1 = true�����ᴥ��һ�� MSG_ERASEBKGND ��Ϣ������
//      ������Ϣ����������֮��
//������hwnd: ���ھ��.
//���أ�DC���.��DC���´�������ʼ���ġ�
//------------------------------------------------------------------------------
HDC GDD_BeginPaint(HWND hwnd)
{
    HDC hdc;
    struct WindowMsg msg;

    hdc = malloc(sizeof(DC));
    if(hdc!=NULL)
    {
        memset(hdc, 0, sizeof(DC));
        GDD_InitDC(hdc,hwnd->pGkWin,hwnd,DC_TYPE_PAINT);

        if(__HWND_Lock(hwnd))
        {
            if(hwnd->Flag&WF_ERASEBKGND)
            {
                hwnd->Flag &= ~WF_ERASEBKGND;

                msg.hwnd =hwnd;
                msg.Code =MSG_ERASEBKGND;
                msg.Param1 =(ptu32_t)hdc;
                msg.Param2 =0;
                msg.ExData =NULL;
                __HWND_Unlock(hwnd);
                __GDD_WinMsgProc(&msg);
            }
            else
                __HWND_Unlock(hwnd);
        }
        return hdc;
    }

    return NULL;
}

//----ָ�����ڽ�����ͼ-----------------------------------------------------------
//����: �ú���������BeginPaint�ɶ�ʹ��.��������ʱ��������Ҫ��ʾ������ˢ�µ���ʾ���ϣ�
//      ��ˢ��������ʾ����������ˢ��ָ������
//������hwnd: ���ھ����ִ��hwnd���ڵ������ڣ������棩����Ϣѭ�������ִ��ˢ����ʾ��
//      ��Ϣ��ˢ�µ�������ͼ��ϵͳ�������ڱ����ڡ�
//      hdc: DC���,������BeginPaint���ص�DC���.
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//ע�⣺������ڶ�������ڣ���ִ�з�hwnd���ڵ����������Ŷ��е���Ϣ��
//------------------------------------------------------------------------------
bool_t    GDD_EndPaint(HWND hwnd,HDC hdc)
{
    GDD_SendMessage(hwnd, MSG_SYNC_DISPLAY, 0, 0);
    return  GDD_DeleteDC(hdc);
}

//----Ĭ�ϵĴ��ڷǿͻ���������Ϣ������---------------------------------------
//����: ���ƴ��ڵķǿͻ���.
//������pMsg: ��Ϣָ��.
//���أ���.
//-----------------------------------------------------------------------------
static ptu32_t __GDD_DefWindowProcNCPAINT(struct WindowMsg *pMsg)
{
    HWND hwnd=pMsg->hwnd;
    HDC hdc;
    RECT rc;


    hdc =GDD_GetWindowDC(hwnd);
    if(NULL!=hdc)
    {
        GDD_GetWindowRect(hwnd,&rc);
        GDD_ScreenToWindow(hwnd,(POINT*)&rc,2);

        if(__HWND_Lock(hwnd))
        {
            if(hwnd->Style&WS_BORDER)
            {
                GDD_SetDrawColor(hdc,WINDOW_BORDER_COLOR);
                GDD_DrawRect(hdc,&rc);
                GDD_InflateRect(&rc,-1,-1);
            }

            if(hwnd->Style&WS_DLGFRAME)
            {
                GDD_SetDrawColor(hdc,WINDOW_DLGFRAME_COLOR1);
                GDD_DrawRect(hdc,&rc);
                GDD_InflateRect(&rc,-1,-1);

                GDD_SetDrawColor(hdc,WINDOW_DLGFRAME_COLOR2);
                GDD_DrawRect(hdc,&rc);
                GDD_InflateRect(&rc,-1,-1);

                GDD_SetDrawColor(hdc,WINDOW_DLGFRAME_COLOR3);
                GDD_DrawRect(hdc,&rc);
                GDD_InflateRect(&rc,-1,-1);
            }

            if(hwnd->Style&WS_CAPTION)
            {
                rc.bottom =rc.top+hwnd->CaptionSize;

                GDD_GradientFillRect(hdc,&rc,RGB(0,100,200),RGB(0,30,100),CN_FILLRECT_MODE_UD);

                GDD_SetTextColor(hdc,WINDOW_CAPTION_TEXT_COLOR);
                GDD_InflateRect(&rc,-1,-1);
                GDD_DrawText(hdc, hwnd->Text, -1, &rc, DT_LEFT | DT_VCENTER);
            }
            __HWND_Unlock(hwnd);
        }
        GDD_ReleaseDC(hdc);
    }
    return 0;
}

//----Ĭ�ϵĴ��ڿͻ�������������Ϣ������-------------------------------------
//����: ʹ�ô��ڵ�Ĭ�ϱ�����ɫ����䴰�ڿͻ�������.
//������pMsg: ��Ϣָ��.
//���أ���.
//-----------------------------------------------------------------------------
static ptu32_t __GDD_DefWindowProcERASEBKGND(struct WindowMsg *pMsg)
{
    HDC hdc;
    RECT rc;

    hdc =(HDC)pMsg->Param1;
    if(hdc!=NULL)
    {
        GDD_GetClientRect(pMsg->hwnd,&rc);
        GDD_FillRect(hdc,&rc);
        return 1;
    }
    return 0;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----Ĭ�ϵĴ��ڿͻ���������Ϣ������-------------------------------------
//����: ����ûʲô�������ȿ��š�.
//������pMsg: ��Ϣָ��.
//���أ���.
//-----------------------------------------------------------------------------
static ptu32_t __GDD_DefWindowProcPAINT(struct WindowMsg *pMsg)
{
    return true;
}

static ptu32_t __GDD_DefWindowProcSYNC(struct WindowMsg *pMsg)
{
    GK_SyncShow(CN_TIMEOUT_FOREVER);
    return true;
}
#pragma GCC diagnostic pop

void __GDD_InitMsg(struct WindowMsg *msg,HWND hwnd,u32 code,u32 param1,ptu32_t param2);
//----Ĭ�ϵĴ��ڹر���Ϣ������-------------------------------------
//����: �رմ��ڣ�����ǽ��㴰�ڻ����ǽ��㴰�ڵ����ȴ��ڣ��Ͱѽ��㴰��ת�Ƶ����档
//      ��ĩ�Ҵ��ڿ�ʼ�����ɾ���������ݽṹ��
//      ����û����ڶ�������Ϣ������������Ҫָ��Ϊ MSG_ADOPT_NORMAL��������
//      ����Ԥ֪�Ľ����
//������pMsg: ��Ϣָ��.
//���أ���.
//-----------------------------------------------------------------------------
static ptu32_t __GDD_DefWindowProcCLOSE(struct WindowMsg *pMsg)
{
    HWND Current;
    struct WindowMsg SubMsg;
    HWND hwnd,next;

//  if(__GDD_Lock())
//  {
        hwnd = pMsg->hwnd;
        //���ұ�ɾ�������Ƿ�focus���ڵ����ȴ��ڣ����ǣ�����Ҫת��focus����
        //�ӱ�ɾ���ڵ��ֵܴ������ҵ�һ������focus�Ĵ��ڣ���Ϊ�µ�focus���ڡ�
        //������û���ҵ����򿴸������Ƿ�����focus����������Ϊfocus���ڡ�
        //������ desktop Ϊ focus ����
        if(GDD_IsFocusAncestor(hwnd))
        {
            next = GDD_GetWindowNext(hwnd);
            while(next != hwnd)
            {
                if( GDD_IsFocusEnable(next) )
                {
                    GDD_SetFocusWindow(next);
                    break;
                }
                else
                    next = GDD_GetWindowNext(hwnd);
            }
            if(next == hwnd)
            {
                if(GDD_IsFocusEnable(GDD_GetWindowParent(next)))
                {
                    GDD_SetFocusWindow(GDD_GetWindowParent(next));
                }
                else
                {
                    GDD_SetFocusWindow(GDD_GetDesktopWindow(NULL ));
                }
            }
        }
        Current = __GDD_GetWindowTwig(hwnd);
        while(Current != NULL)
        {
            //ĩ�Ҵ���ֱ�ӵ�����Ϣ���������������WinMsgProc������ʽʵ�֣�����
            //������ȷ�����û��������Ϣ��������������ȷ������Ϣ�������ļ̳�
            //��ϵ��
            //���ｫ����ݹ飬���ݹ����ֻ��1������Ϊ twig ���������Ӵ����ˡ�
            __GDD_InitMsg(&SubMsg, Current, MSG_CLOSE, 0, (ptu32_t)Current->PrivateData);
            __GDD_WinMsgProc(&SubMsg);
            Current = __GDD_GetWindowTwig(hwnd);
        }
        GK_DestroyWin(hwnd->pGkWin);
//      __GDD_Unlock();
//  }
    return 0;
}

//----Ĭ�ϵ����ٴ�����Ϣ������-------------------------------------
//����: ����.
//������pMsg: ��Ϣָ��.
//���أ���.
//-----------------------------------------------------------------------------
//static bool_t DefWindowProc_DESTROY(struct WindowMsg *pMsg)
//{
//    PostQuitMessage(pMsg->hwnd,0);
//    return TRUE;
//}

//----����Ϣ���������в��Ҷ�Ӧ����Ϣ-----------------------------------------
//���ܣ���
//������Code�������ҵ���Ϣ
//      MsgTable��Դ��
//      Num�����е���Ϣ����
//���أ����ҵ����򷵻���Ϣ��MsgTable�е�ƫ�ƣ����򷵻�-1
//-----------------------------------------------------------------------------
static s32 __GDD_GetWinMsgFunc(u32 Code,struct MsgTableLink * msgtablink)
{
    u32 loop = 0;
    u32 CodeTab;
    struct MsgProcTable *MsgTable;
    u32 Num;

    MsgTable = msgtablink->myTable;
    Num = msgtablink->MsgNum;
    if(MsgTable == NULL)//��ӵ��ĺ����������û���ҵ�
        return -1;
    for(loop = 0; loop < Num; loop++)
    {
        CodeTab = MsgTable[loop].MsgCode & MSG_BODY_MASK;
        if(Code == CodeTab)
        {
            break;
        }
        else if(CodeTab == 0)
        {
            return (-1);
        }
    }
    if(loop >= Num)
        loop = -1;
    return loop;
}
//----������Ϣ������---------------------------------------------------------
//����: ��������Ϣ����Ϣ����hwnd->MyMsgTableLink��Աʵ�ָ��ǣ�ֻҪ���Ӵ�����
//      ʵ��ͬ����Ϣ�������ڵĸ���Ϣ�������ͻᱻ���滻������
//������pMsg: ��Ϣָ��.
//���أ���Ϣ������.
//-----------------------------------------------------------------------------
ptu32_t __GDD_WinMsgProc(struct WindowMsg *pMsg)
{
    HWND hwnd;
    u32 MsgCtrl;
    s32 offset,LastTable = 0, FirstTable = 0;
    bool_t MsgGot = false;
    ptu32_t result = 0;
    struct MsgTableLink *MyTableLinkNode;
    struct MsgProcTable *MyTable;
    s32 num = 0;

    if(__GDD_Lock())
    {
        hwnd = pMsg->hwnd;
        if(hwnd->pGkWin == NULL)        //��gkwin�ľ����һ������Ч�ġ�
        {
            __GDD_Unlock();
            return 0;
        }
        //����Ϣ����̳л��ƣ��ȴ������ӵ���Ϣ��ʼ���ң�ֱ���ҵ�һ������Ҫ�̳��ϼ�
        //��Ϣ����Ϣ������.
        //���е���C++�ļ̳л��ơ��൱���û��̳пؼ����ؼ��̳д���ϵͳ��
        //MyMsgTableLink�൱��ָ�����飬Խ��������Ϣ���±�Խ��
        MyTableLinkNode = hwnd->MyMsgTableLink[0];
        while(1)
        {
            offset = __GDD_GetWinMsgFunc(pMsg->Code & MSG_BODY_MASK, MyTableLinkNode);
            if(offset != -1)        //��Ϣ�����ҵ�����Ϣ��������
            {
                LastTable = num;    //LastTable��¼���һ��������������Ϣ��
                if(MsgGot == false)
                {
                    FirstTable = num;
                    MsgGot = true;
                }
                MyTable = MyTableLinkNode->myTable;
                MsgCtrl =  MyTable[offset].MsgCode & MSG_CONTROL_MSK;
                if((MsgCtrl & MSG_ADOPT_MSK) == MSG_ADOPT_NONE) //���̳�
                {
                    break;      //�ҵ���һ������Ҫ�̳е���Ϣ�����������������ǰ��
                }
            }
            if(MyTableLinkNode == &s_gDefWindowMsgLink) //�Ѿ������һ����Ϣ��
                break;
            else
                MyTableLinkNode = hwnd->MyMsgTableLink[++num];
        }

        if(MsgGot == true)
        {
            //��LastTable��ʼ����ִ����Ϣ�������������ռ̳�Ҫ���ȼ������Ϣ���ȴ���
            for(; LastTable >=FirstTable; LastTable--)
            {
                MyTableLinkNode = hwnd->MyMsgTableLink[LastTable];
                offset = __GDD_GetWinMsgFunc(pMsg->Code & MSG_BODY_MASK, MyTableLinkNode);
                if(offset != -1)
                {
                    MyTable = MyTableLinkNode->myTable;
                    if(MyTable[offset].MsgProc !=NULL)
                        result = MyTable[offset].MsgProc(pMsg);
                }
            }
        }
        if(pMsg->Code == MSG_CLOSE)
        {
            if(hwnd->Flag & WF_MAINWIN)
                __GDD_DeleteMainWindowData(hwnd);   //������Ҫɾ���Ķ�����һЩ
            else
                __GDD_DeleteChildWindowData(hwnd);
        }
        __GDD_Unlock();
    }
    return result;

}

//----ɾ��һ�����������ݽṹ---------------------------------------------------
//���������������ڴ��������ں�������ʧ��ʱ�������������á�
//��������Ҫɾ���������ھ��
//���أ���.
//------------------------------------------------------------------------------
void __GDD_ClearMainWindow(HWND hwnd)
{
    dListRemove(&hwnd->node_msg_close);
    dListRemove(&hwnd->node_msg_ncpaint);
    dListRemove(&hwnd->node_msg_paint);

    __GDD_RemoveWindowTimer(hwnd);
    __GUI_DeleteMsgQ(hwnd->pMsgQ);
    GK_DestroyWin(hwnd->pGkWin);
    free(hwnd->pGkWin);
    hwnd->pGkWin =NULL;

    Lock_MutexDelete(hwnd->mutex_lock);
    hwnd->mutex_lock =NULL;
    free(hwnd);

}

//----����GUIӦ��---------------------------------------------------------------
//����: ����GUIӦ�ã�ͬʱ���������ڣ��Լ���Ϣ���к���Ϣѭ����
//������AppName��Appname��Ҳ�Ǵ������֣��������ڴ��ڱ������������ʾ������������copy
//      ��gkwin��name��Ա��������CN_GKWIN_NAME_LIMIT������������TODO������Ϊ�ַ�����
//      Style: ���ڷ��(�ο� WS_VISIBLE �峣��)
//      x,y,w,h: ����λ�úʹ�С,λ������ڸ����ڣ���� w == -1����ʾʹ������ߴ硣
//      MemSize����Ӧ��������ڴ�������ִ����Ϣѭ�����߳�ջ�ߴ�
//      BufProperty�����ڵ�buf���ԣ�ȡֵΪ CN_WINBUF_PARENT�ȡ�
//      pdata: �û��Զ��帽������.
//      PixelFormat,���ظ�ʽ����gkernel.h�ж��壬 CN_SYS_PF_DISPLAY ��ʾ����ʾ����ͬ���Ƽ���
//      BaseColor, �Ҷ�ͼ��ɫ��(����PixelFormat == CN_SYS_PF_GRAY1 ~8ʱ����)
//      pUserMsgTableLink:������Ӧ����Ϣ��
//���أ�Ӧ�ö�Ӧ�������ھ��
//-----------------------------------------------------------------------------
HWND GDD_CreateGuiApp(char *AppName,struct MsgTableLink  *MyMsgLink,
                      s32 x,s32 y,s32 w,s32 h,
                      u32 MemSize, u32 BufProperty,u32 Style,u16 PixelFormat,u32 BaseColor)
{
    HWND result = NULL;
    RECT rc;
    u16 MyEvtt;
    u16 MyEventId;

    if(w == -1)
    {
        GDD_GetClientRect(GDD_GetDesktopWindow(NULL),&rc);
        x = rc.left;
        y = rc.top;
        w = GDD_RectW(&rc);
        h = GDD_RectH(&rc);
    }

    //���������ڣ�ÿ��ͼ��Ӧ�ö�Ӧһ��������
    result = __GDD_CreateMainWindow(AppName, WS_CAN_FOCUS|Style, x,y,w,h, NULL, 0x0000,
                                 BufProperty, 0, PixelFormat, BaseColor, MyMsgLink);
    if(result != NULL)
    {

        MyEvtt = DJY_EvttRegist(EN_CORRELATIVE, CFG_GUI_RUN_PRIO, 0, 0, __GDD_MessageLoop,
                                NULL, MemSize, AppName);
        if(MyEvtt != CN_EVTT_ID_INVALID)
        {
            MyEventId=DJY_EventPop(MyEvtt, NULL, 0, (ptu32_t)result, 0, 0);
            if(MyEventId != CN_EVENT_ID_INVALID)
            {
                result->EventID = MyEventId;
                GDD_SetFocusWindow(result);
                GDD_SetWindowShow(result); //��ʾ����
            }
            else
            {
                DJY_EvttUnregist( MyEvtt );
                __GDD_ClearMainWindow(result);
                result = NULL;
            }
        }
        else
        {
            __GDD_ClearMainWindow(result);
            result = NULL;
        }
    }

    if(result == NULL)
    {
        debug_printf("gdd","Create APP main window failure\n\r");
    }
    return result;
}

//----�ȴ�GUI APP�˳�----------------------------------------------------------
//���ܣ�������ǰӦ�ã��ȴ���Ϊname��APP�˳��������ڵ�ǰAPP�еȴ�����ֱ�ӷ���false
//������name��GUI APP ���֡�
//���أ�false = �쳣�˳����ڵ�ǰAPP�е���Ҳ����false
//      true = APP�˳���
// ----------------------------------------------------------------------------
void GDD_WaitGuiAppExit(char *AppName)
{
    DJY_WaitEvttCompleted(DJY_GetEvttId(AppName), 1, CN_TIMEOUT_FOREVER);
}

//----GDD����ģ���ʼ������----------------------------------------------------
//����: �ú���Ϊ�ڲ�����.
//��������
//���أ���
//------------------------------------------------------------------------------
void    GDD_WindowInit(void)
{
    s_gDefWindowMsgLink.MsgNum = sizeof(s_gDefWindowMsgProcTable)
                                    / sizeof(struct MsgProcTable);
    s_gDefWindowMsgLink.myTable   = s_gDefWindowMsgProcTable;
}

