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
//文件描述: 窗口及控件机制及管理
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2014-11-10
//   作者:  LiuWei.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include    <gui/gdd/gdd_private.h>
#include <gkernel.h>
#include "dbug.h"
/*
typedef struct  __WNDCLASS  //窗口类数据结构
{
    WNDPROC *WinProc;
    u32 DrawColor;
    u32 FillColor;
    u32 TextColor;
}WNDCLASS;
*/

extern HWND g_CursorHwnd;         //光标窗口
extern void GK_SetVisible(struct GkWinObj *gkwin, u32 visible,u32 SyncTime);
/*============================================================================*/

HWND HWND_Desktop=NULL;
u16 sg_MainWindEvtt;

static ptu32_t DefWindowProc_NCPAINT(struct WindowMsg *pMsg);
static ptu32_t DefWindowProc_ERASEBKGND(struct WindowMsg *pMsg);
static ptu32_t DefWindowProc_PAINT(struct WindowMsg *pMsg);
static ptu32_t DefWindowProc_CLOSE(struct WindowMsg *pMsg);
static ptu32_t DefWindowProc_SYNC(struct WindowMsg *pMsg);

//默认窗口消息处理函数表，处理用户窗口过程没有处理的消息。
static struct MsgProcTable s_gDefWindowMsgProcTable[] =
{
    {MSG_NCPAINT,DefWindowProc_NCPAINT},
    {MSG_ERASEBKGND,DefWindowProc_ERASEBKGND},
    {MSG_PAINT,DefWindowProc_PAINT},
    {MSG_SYNC_DISPLAY,DefWindowProc_SYNC},
    {MSG_CLOSE&MSG_BODY_MASK,DefWindowProc_CLOSE},
//  {MSG_DESTROY,DefWindowProc_DESTROY},
};

static struct MsgTableLink  s_gDefWindowMsgLink;

//----锁定窗口------------------------------------------------------------------
//描述: 锁定窗口,用于对窗口互斥操作,该函数返回TRUE时,必须调用HWND_Unlock解锁;
//      而当该函数返回FALSE,则无需调用HWND_Unlock.
//参数：hwnd:窗口句柄
//返回：成功:TRUE; 失败:FLASE;
//------------------------------------------------------------------------------
bool_t    __HWND_Lock(HWND hwnd)
{
    bool_t result = false;
    if(__GDD_Lock())    //不先锁住GDD，可能会死锁的
    {
        result = Lock_MutexPend(hwnd->mutex_lock, CN_TIMEOUT_FOREVER);
        __GDD_Unlock( );
    }
    return result;
}

//----解锁窗口------------------------------------------------------------------
//描述: 当窗口锁定成功后,由该函数进行解锁操作.
//参数：hwnd:窗口句柄
//返回：无
//------------------------------------------------------------------------------
void __HWND_Unlock(HWND hwnd)
{
    Lock_MutexPost(hwnd->mutex_lock);
}

//----取窗口句柄---------------------------------------------------------------
//功能：根据gkwin指针，去所属的gdd窗口句柄
//参数：gkwin，gkwin指针
//返回：gdd窗口句柄
//---------------------------------------------------------------------------
HWND GetGddHwnd(struct GkWinObj *gkwin)
{
    return (HWND)GK_GetUserTag(gkwin);
}

//----屏幕坐标转客户坐标---------------------------------------------------------
//描述: 把一组屏幕坐标值，转换为窗口客户区的相对坐标
//参数：hwnd:窗口句柄
//      pt:  需要转换的屏幕坐标点指针
//      count: 需要转换的坐标点数量
//返回：TRUE:成功; FALSE:失败.
//------------------------------------------------------------------------------
bool_t    ScreenToClient(HWND hwnd,POINT *pt,s32 count)
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

//----客户坐标转屏幕坐标---------------------------------------------------------
//描述: 略
//参数：hwnd:窗口句柄
//      pt:  需要转换的坐标点指针
//      count: 需要转换的坐标点数量
//返回：TRUE:成功; FALSE:失败.
//------------------------------------------------------------------------------
bool_t    ClientToScreen(HWND hwnd,POINT *pt,s32 count)
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

//----屏幕坐标转窗口坐标---------------------------------------------------------
//描述: 略
//参数：hwnd:窗口句柄
//      pt:  需要转换的坐标点指针
//      count: 需要转换的坐标点数量
//返回：TRUE:成功; FALSE:失败.
//------------------------------------------------------------------------------
bool_t    ScreenToWindow(HWND hwnd,POINT *pt,s32 count)
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

//----窗口坐标转屏幕坐标---------------------------------------------------------
//描述: 略
//参数：hwnd:窗口句柄
//      pt:  需要转换的坐标点指针
//      count: 需要转换的坐标点数量
//返回：TRUE:成功; FALSE:失败.
//------------------------------------------------------------------------------
bool_t    WindowToScreen(HWND hwnd,POINT *pt,s32 count)
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


//----获得桌面窗口句柄-----------------------------------------------------------
//描述: 略
//参数：无
//返回：桌面窗口句柄.
//------------------------------------------------------------------------------
HWND    GetDesktopWindow(void)
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

//----获得窗口矩形区-----------------------------------------------------------
//描述: 获得窗口矩形区,矩形为屏幕坐标.
//参数：hwnd:窗口句柄
//      prc:用于保存矩形数据的指针.
//返回：TRUE:成功; FALSE:失败.
//------------------------------------------------------------------------------
bool_t    GetWindowRect(HWND hwnd,RECT *prc)
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

//----获得窗口客户矩形区-------------------------------------------------------
//描述: 获得窗口客户矩形区,矩形为客户坐标.
//参数：hwnd:窗口句柄
//      prc:用于保存矩形数据的指针.
//返回：TRUE:成功; FALSE:失败
//------------------------------------------------------------------------------
bool_t    GetClientRect(HWND hwnd,RECT *prc)
{
    if(NULL!=prc)
    if(__HWND_Lock(hwnd))
    {
        prc->left =0;
        prc->top =0;
        prc->right =RectW(&hwnd->CliRect);
        prc->bottom =RectH(&hwnd->CliRect);
        __HWND_Unlock(hwnd);
        return TRUE;
    }
    return FALSE;
}

//----获得窗口客户矩形区并转换为屏幕坐标---------------------------------------
//描述: 略
//参数：hwnd:窗口句柄
//      prc:用于保存矩形数据的指针.
//返回：TRUE:成功; FALSE:失败.
//------------------------------------------------------------------------------
bool_t    GetClientRectToScreen(HWND hwnd,RECT *prc)
{
    if(NULL!=prc)
    {
        if(__HWND_Lock(hwnd))
        {
            CopyRect(prc,&hwnd->CliRect);
            __HWND_Unlock(hwnd);
            return TRUE;
        }
    }
    return FALSE;
}

//----获得窗口消息队列-----------------------------------------------------------
//描述: 该函数为内部调用,不检查函数参数合法性,非线程安全
//参数：hwnd:窗口句柄
//返回：窗口消息队列指针.
//------------------------------------------------------------------------------
struct WinMsgQueueCB*   __GetWindowMsgQ(HWND hwnd)
{
    return hwnd->pMsgQ;
}

//----获得窗口线程Id-------------------------------------------------------------
//描述: 该函数为内部调用,不检查函数参数合法性,非线程安全
//参数：hwnd:窗口句柄
//返回：窗口线程Id.
//------------------------------------------------------------------------------
u32 __GetWindowEvent(HWND hwnd)
{
    return hwnd->EventID;
}

//----获得窗口风格-------------------------------------------------------------
//描述: 取窗口风格，是一个32位掩码，其中高16bit用于公共风格，0~7bit用于区分控件
//      类型，8~15bit用于控件风格。
//参数：hwnd:窗口句柄
//返回：窗口风格.
//------------------------------------------------------------------------------
u32 GetWindowStyle(HWND hwnd)
{
    u32 style=0;

    if(__HWND_Lock(hwnd))
    {
        style =hwnd->Style;
        __HWND_Unlock(hwnd);
    }
    return style;
}

//----获得窗口私有数据-----------------------------------------------------------
//描述: 略
//参数：hwnd:窗口句柄
//返回：窗口私有数据.
//------------------------------------------------------------------------------
void* GetWindowPrivateData(HWND hwnd)
{
    void *data;

    data=NULL;
    if(__HWND_Lock(hwnd))
    {
        data = hwnd->PrivateData;
        __HWND_Unlock(hwnd);
    }
    return data;
}

//----设置窗口私有数据-----------------------------------------------------------
//描述: 略
//参数：hwnd:窗口句柄
//      data: 私有数据
//返回：无.
//------------------------------------------------------------------------------
void SetWindowPrivateData(HWND hwnd,void *data)
{
    if(__HWND_Lock(hwnd))
    {
        hwnd->PrivateData=data;
        __HWND_Unlock(hwnd);
    }
}

//----取主窗口句柄-------------------------------------------------------------
//功能：根据主窗口的标题返回窗口句柄，注意，主窗口标题是不会重复的，但子窗口可能
//      会，故本函数只能取主窗口的句柄。
//参数：WinText，窗口标题，与gkwin的win_name相同
//返回：父窗口句柄，无则返回NULL
//-----------------------------------------------------------------------------
HWND Gdd_GetWindowHandle(char *WinText)
{
    HWND wnd,first,result = NULL;
    wnd = GetWindowChild(GetDesktopWindow( ));
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
            wnd = GetWindowNext(wnd);
        } while( wnd != first);
        __GDD_Unlock( );
    }
    return result;
}

//----取父窗口-----------------------------------------------------------------
//功能：取窗口的父窗口句柄
//参数：hwnd，窗口句柄
//返回：父窗口句柄，无则返回NULL
//-----------------------------------------------------------------------------
HWND Gdd_GetWindowParent(HWND hwnd)
{
    HWND wnd=NULL;

    if(__GDD_Lock( ))
    {
        wnd = (HWND)GK_GetUserTag(GK_GetParentWin(hwnd->pGkWin));
        __GDD_Unlock( );
    }
    return wnd;
}

//----取子窗口-----------------------------------------------------------------
//功能：取窗口的子窗口句柄
//参数：hwnd，窗口句柄
//返回：子窗口句柄，无则返回NULL
//-----------------------------------------------------------------------------
HWND GetWindowChild(HWND hwnd)
{
    HWND wnd=NULL;

    if(__GDD_Lock( ))
    {
        wnd = (HWND)GK_GetUserTag(GK_GetChildWin(hwnd->pGkWin));
        __GDD_Unlock( );
    }
    return wnd;
}

//----取末梢窗口---------------------------------------------------------------
//功能：从后代窗口中取一个不再有子窗口的窗口句柄
//参数：hwnd，窗口句柄
//返回：子窗口句柄，无则返回NULL
//-----------------------------------------------------------------------------
HWND __GetWindowTwig(HWND hwnd)
{
    HWND wnd=NULL;

    if(__GDD_Lock( ))
    {
        wnd = (HWND)GK_GetUserTag(GK_GetTwig(hwnd->pGkWin));
        __GDD_Unlock( );
    }
    return wnd;
}

//----取一个前窗口-------------------------------------------------------------
//功能：取窗口的前窗口句柄
//参数：hwnd，窗口句柄
//返回：前窗口句柄，无则返回NULL
//-----------------------------------------------------------------------------
HWND GetWindowPrevious(HWND hwnd)
{
    HWND wnd=NULL;

    if(__GDD_Lock( ))
    {
        wnd = (HWND)GK_GetUserTag(GK_GetPreviousWin(hwnd->pGkWin));
        __GDD_Unlock( );
    }
    return wnd;
}

//----取一个后窗口-------------------------------------------------------------
//功能：取窗口的后窗口句柄
//参数：hwnd，窗口句柄
//返回：后窗口句柄，无则返回NULL
//-----------------------------------------------------------------------------
HWND GetWindowNext(HWND hwnd)
{
    HWND wnd=NULL;

    if(__GDD_Lock( ))
    {
        wnd = (HWND)GK_GetUserTag(GK_GetNextWin(hwnd->pGkWin));
        __GDD_Unlock( );
    }
    return wnd;
}

//----取一个最前窗口-------------------------------------------------------------
//功能：取窗口同级窗口中的最前窗口句柄
//参数：hwnd，窗口句柄
//返回：最前窗口句柄，无则返回NULL
//-----------------------------------------------------------------------------
HWND GetWindowFirst(HWND hwnd)
{
    HWND wnd=NULL;

    if(__GDD_Lock( ))
    {
        wnd = (HWND)GK_GetUserTag(GK_GetFirstWin(hwnd->pGkWin));
        __GDD_Unlock( );
    }
    return wnd;
}

//----取一个最后窗口-------------------------------------------------------------
//功能：取窗口的同级窗口中最后窗口句柄
//参数：hwnd，窗口句柄
//返回：最后窗口句柄，无则返回NULL
//-----------------------------------------------------------------------------
HWND GetWindowLast(HWND hwnd)
{
    HWND wnd=NULL;

    if(__GDD_Lock( ))
    {
        wnd = (HWND)GK_GetUserTag(GK_GetLastWin(hwnd->pGkWin));
        __GDD_Unlock( );
    }
    return wnd;
}

//----取一个最后窗口-------------------------------------------------------------
//功能：取窗口的同级窗口中最后窗口句柄
//参数：hwnd，窗口句柄
//返回：最后窗口句柄，无则返回NULL
//-----------------------------------------------------------------------------
char *GetWindowText(HWND hwnd)
{
    return hwnd->Text;
}

//----按Id查找窗口中的子窗口(控件)-----------------------------------------------
//描述: 略.
//参数：hwnd:窗口句柄
//     Id: 需要查找的子窗口(控件)Id.
//返回：查找到的子窗口(控件)句柄.
//------------------------------------------------------------------------------
HWND    GetDlgItem(HWND hwnd,u16 Id)
{
    HWND Current = hwnd;
    if(__GDD_Lock())
    {
        Current = (HWND)GK_GetUserTag(GK_TraveChild(hwnd->pGkWin,Current->pGkWin));
        while(Current != NULL)
        {
            if(Current->WinId == Id)
                break;
            Current = (HWND)GK_GetUserTag(GK_TraveChild(hwnd->pGkWin,Current->pGkWin));
        }
        __GDD_Unlock();
    }
    return Current;
}

//----按坐标查找窗口(主窗口或子窗口)-------------------------------------------
//描述: 略.
//参数：pt:坐标点
//返回：查找到的窗口句柄.
//------------------------------------------------------------------------------
HWND    GetWindowFromPoint(struct GkWinObj *desktop, POINT *pt)
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

//----设置窗口名字-------------------------------------------------------------
//描述: 略.
//参数：hwnd: 待设置的窗口
//      NewName，新的名字
//返回：无
//------------------------------------------------------------------------------
void SetWindowName(HWND hwnd, char *NewName)
{
    GK_SetName(hwnd->pGkWin, NewName);
    return ;
}

//----初始化窗口数据结构---------------------------------------------------------
//描述: 该函数为内部调用.
//参数：pwin:窗口数据结构指针.
//      proc:窗口过程回调函数指针．
//      Text:窗口文字内容指针.
//      Style:窗口风格.
//      x,y,w,h:窗口位置和大小.
//      hParent:父窗口句柄.
//      WinId: 窗口Id.
//返回：无.
//------------------------------------------------------------------------------
static  void __InitWindow(HWND pwin,u32 Style,u32 WinId)
{
    RECT rc;

    pwin->Flag =0;

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
        __InflateRectEx(&rc, -DEF_BORDER_SIZE,
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
        __InflateRectEx(&rc, -DEF_DLGFRAME_SIZE,
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
        __InflateRectEx(&rc,0,-DEF_CAPTION_SIZE,0,0);
    }
    else
    {
        pwin->CaptionSize =0;
    }
    CopyRect(&pwin->CliRect,&rc);

}


//桌面绘制函数
bool_t DesktopPaint(struct WindowMsg *pMsg)
{
    HDC hdc;
    RECT rc;

    HWND hwnd=pMsg->hwnd;
    hdc =BeginPaint(hwnd);

    GetClientRect(hwnd,&rc);

    GradientFillRect(hdc,&rc,RGB(120,120,255),RGB(20,20,80),CN_FILLRECT_MODE_UD);

    EndPaint(hwnd,hdc);
    return true;
}

void GDD_HmiInput(void);


//----GDD服务定时器函数--------------------------------------------------------
//功能：GDD服务定时器的回调函数，该定时器在GDD初始化时创建，目前只用来扫描人机界面
//      输入，例如鼠标、键盘等。如果系统没有输入设备，也可以不创建。
//参数：pMsg，消息指针
//返回：固定true
//-----------------------------------------------------------------------------
static bool_t GddServerTimerHandle(struct WindowMsg *pMsg)
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

//桌面消息处理函数表，
static struct MsgProcTable s_gDesktopMsgProcTable[] =
{
    {MSG_PAINT,DesktopPaint},
    {MSG_TIMER,GddServerTimerHandle},
};

ptu32_t __MessageLoop( void );
static struct MsgTableLink  s_gDesktopMsgLink;

//=======================================================
//获取当前消息链表从s_gDefWindowMsgLink开始的深度
//=====================================================
static u32 __GetNumber_Of_Prev(HWND Hwnd)
{
    u32 num = 0;
    if(Hwnd == NULL)
        return num;

    while(Hwnd->MyMsgTableLink[num++] != &s_gDefWindowMsgLink);

    return num;
}

//----初始化桌面窗口-----------------------------------------------------------
//描述: 初始化gdd的桌面窗口
//参数：proc: 桌面窗口过程回调函数
//      desktop,须初始化的桌面指针
//      pdata: 用户自定义附加数据.
//返回：桌面窗口句柄.
//------------------------------------------------------------------------------
HWND    InitGddDesktop(struct GkWinObj *desktop)
{
    HWND pGddWin=NULL;
    u32 Style;
    u16 MyEvtt;
    u16 MyEventid;

    pGddWin=malloc(sizeof(struct WINDOW));
    if(NULL!=pGddWin)
    {

        Style = WS_VISIBLE | WS_CAN_FOCUS;
//      s_gDesktopMsgLink.LinkNext = NULL;
//      s_gDesktopMsgLink.MsgNum = sizeof(s_gDesktopMsgProcTable) / sizeof(struct MsgProcTable);
//      s_gDesktopMsgLink.myTable = s_gDesktopMsgProcTable;
        pGddWin->pGkWin     = desktop;
        pGddWin->PrivateData = NULL;
        pGddWin->MyMsgTableLink = (struct MsgTableLink **)\
                                           malloc(2*sizeof( struct MsgTableLink *));

        if( pGddWin->MyMsgTableLink ==NULL)
        {
           free(pGddWin);
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
            free(pGddWin);
            return NULL;
        }

        GK_SetUserTag(desktop,pGddWin);
        __InitWindow(pGddWin,Style,0);
        HWND_Desktop = pGddWin;

        MyEvtt = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0, __MessageLoop,
                                NULL, 2048, "desktop");
        if(MyEvtt != CN_EVTT_ID_INVALID)
        {
            MyEventid=Djy_EventPop(MyEvtt, NULL, 0, (ptu32_t)pGddWin, 0, 0);
            if(MyEventid!= CN_EVENT_ID_INVALID)
            {
                pGddWin->EventID   = MyEventid;
                SetWindowShow(pGddWin); //显示窗口
            }
            else
            {
                Djy_EvttUnregist( MyEvtt );
                free(pGddWin);
                pGddWin = NULL;
            }
        }
        else
        {
            free(pGddWin);
            pGddWin = NULL;
        }
        SendMessage(pGddWin,MSG_CREATE,0,0);
    }
    return pGddWin;
}

//----增加消息函数表-----------------------------------------------------------
//功能：每个窗口都有一个消息处理函数表的链表（struct MsgTableLink），新添加的消
//      息处理函数表将与原有的链接在一起。系统在查找消息处理函数时，该链表是后进
//      先出的，即最后加入的函数表，反而先被查找。
//参数：hwnd，被操作的窗口句柄
//      pNewMsgTableLink，带添加的消息链
//返回：无
// ----------------------------------------------------------------------------
void AddProcFuncTable(HWND hwnd,struct MsgTableLink *pNewMsgTableLink)
{
    u32 num;
    u32 i;
    struct MsgTableLink ** tab;
    if(pNewMsgTableLink==NULL || hwnd==NULL)
        return;
    num =  __GetNumber_Of_Prev(hwnd);
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
//----创建窗口-----------------------------------------------------------------
//描述: 该函数可以创建主窗口和子窗口(控件)
//参数：Text: 窗口名字，将copy到gkwin的name成员，长度限CN_GKWIN_NAME_LIMIT，
//              （字节数），TODO：待改为字符数。
//      Style: 窗口风格(参考 WS_CHILD 族常量)
//      x,y,w,h: 窗口位置和大小,相对于父窗口
//      hParent: 父窗口句柄.如果是NULL,则默认桌面为父窗口.
//      WinId: 窗口Id.如果是主窗口,该参数被忽略.
//      BufProperty，窗口的buf属性，取值为 CN_WINBUF_PARENT等。
//      pdata: 用户自定义附加数据.
//      pUserMsgTableLink:窗口响应的消息表
//返回：窗口句柄.
//-----------------------------------------------------------------------------
HWND    CreateWindow(const char *Text,u32 Style,
                     s32 x,s32 y,s32 w,s32 h,
                     HWND hParent,u32 WinId,
                     u32 BufProperty,void *pdata,
                     struct MsgTableLink *pUserMsgTableLink)
{
    HWND pGddWin=NULL;
    struct GkWinObj *pGkWin=NULL;
    u32 num;

    struct RopGroup RopCode = (struct RopGroup){ 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };

    if(NULL==hParent)
    {
        hParent = HWND_Desktop;
    }

    if(__GDD_Lock())
    {
//      pGddWin=M_Malloc(sizeof(struct WINDOW) + sizeof(struct GkWinObj),100*mS);
        pGddWin=M_Malloc(sizeof(struct WINDOW), 100*mS);
        memset(pGddWin, 0, sizeof(struct WINDOW));
        if(NULL!=pGddWin)
        {
//          pGkWin = (struct GkWinObj*)(pGddWin+1);
//
//          if(!GK_CreateWin(hParent->pGkWin, pGkWin,x,y,x+w,y+h,
//                              RGB(0,0,0), BufProperty, Text,
//                              CN_SYS_PF_DISPLAY, 0,RGB(255, 255, 255),RopCode))
            pGkWin = GK_CreateWin(hParent->pGkWin,x,y,x+w,y+h,
                                RGB(0,0,0), BufProperty, Text,
                                CN_SYS_PF_DISPLAY, 0,RGB(255, 255, 255),RopCode,
                                (Style & WS_UNFILL)==WS_UNFILL);
            if(!pGkWin)
            {
                free(pGddWin);
                pGddWin = NULL;
            }
            else
            {
                if(Style&WS_CHILD)
                {
                    pGddWin->EventID   = hParent->EventID;
                    pGddWin->mutex_lock =hParent->mutex_lock;  //子窗口使用父窗口锁
                    pGddWin->pMsgQ      =hParent->pMsgQ;       //子窗口使用父窗口消息队列
                    pGddWin->DrawColor = hParent->DrawColor;
                    pGddWin->FillColor = hParent->FillColor;
                    pGddWin->TextColor = hParent->TextColor;
                }
                else
                {
                    pGddWin->mutex_lock =Lock_MutexCreate(NULL);
                    pGddWin->pMsgQ      =__GUI_CreateMsgQ(128);
                    pGddWin->DrawColor = CN_DEF_DRAW_COLOR;
                    pGddWin->FillColor = CN_DEF_FILL_COLOR;
                    pGddWin->TextColor = CN_DEF_TEXT_COLOR;

                    if((pGddWin->mutex_lock==NULL)||(pGddWin->pMsgQ==NULL))
                    {
                        Lock_MutexDelete(pGddWin->mutex_lock);
                        __GUI_DeleteMsgQ(pGddWin->pMsgQ);
                        GK_DestroyWin(pGkWin);
                        free(pGkWin);
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
                    GK_DestroyWin(pGkWin);
//                  free(pGkWin);
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
                //初始化窗口数据
                __InitWindow(pGddWin,Style,WinId);
//              UpdateDisplay(CN_TIMEOUT_FOREVER);
                //将新窗口添加到父窗口
            }
        }
        __GDD_Unlock();

        if(NULL!=pGddWin)
        {
            SendMessage(pGddWin,MSG_CREATE,(u32)pdata,0);
            PostMessage(pGddWin,MSG_NCPAINT,0,0);
            InvalidateWindow(pGddWin,TRUE);
        }

    }
    return pGddWin;
}

//----删除一个子窗口数据结构---------------------------------------------------
//描述:该函数为内部调用,除了释放窗口结构内存,还将自己丛父窗口去移除.
//参数：需要删除的子窗口句柄
//返回：无.
//------------------------------------------------------------------------------
void __DeleteChildWindowData(HWND hwnd)
{

    dListRemove(&hwnd->node_msg_close);
    dListRemove(&hwnd->node_msg_ncpaint);
    dListRemove(&hwnd->node_msg_paint);
    __RemoveWindowTimer(hwnd);
//    GK_DestroyWin(hwnd->pGkWin);

    free(hwnd->MyMsgTableLink);
//  free(hwnd->pGkWin);
//  hwnd->pGkWin =NULL;
    hwnd->mutex_lock =NULL; //子窗口没有私有的 mutex_lock,不用释放.
    free(hwnd);

}

//----删除一个主窗口数据结构---------------------------------------------------
//描述:该函数为内部调用,除了释放窗口结构内存,还将自己丛父窗口去移除.
//参数：需要删除的主窗口句柄
//返回：无.
//------------------------------------------------------------------------------
void __DeleteMainWindowData(HWND hwnd)
{
    dListRemove(&hwnd->node_msg_close);
    dListRemove(&hwnd->node_msg_ncpaint);
    dListRemove(&hwnd->node_msg_paint);
    __RemoveWindowTimer(hwnd);
    __GUI_DeleteMsgQ(hwnd->pMsgQ);
//    GK_DestroyWin(hwnd->pGkWin);
//  UpdateDisplay(CN_TIMEOUT_FOREVER);
    Lock_MutexDelete(hwnd->mutex_lock);

    free(hwnd->MyMsgTableLink);
//  free(hwnd->pGkWin);
//  hwnd->pGkWin =NULL;
    hwnd->mutex_lock =NULL;
    free(hwnd);

}

//----销毁一个窗口--------------------------------------------------------------
//描述: 可以是主窗口和子窗口,通过给窗口发送close消息来实现。close处理时，将同时
//      销毁后代窗口。
//参数：hwnd:需要销毁的窗口句柄
//返回：无.
//------------------------------------------------------------------------------
void    DestroyWindow(HWND hwnd)
{
    PostMessage(hwnd, MSG_CLOSE, 0, 0);
}

//----销毁全部子窗口-----------------------------------------------------------
//描述: 除hwnd窗口本身不销毁外，其余同DestroyWindow
//参数：hwnd:需要销毁的窗口句柄
//返回：无.
//------------------------------------------------------------------------------
void DestroyAllChild(HWND hwnd)
{
    HWND Current = hwnd;

    if(__GDD_Lock())
    {
        Current = (HWND)GK_GetUserTag(GK_TraveChild(hwnd->pGkWin,Current->pGkWin));
        while(Current != NULL)
        {
//            SetWindowHide(Current);     //MSG_CLOSE消息是最后处理的，先隐藏窗口
          PostMessage(Current, MSG_CLOSE, 0, 0);
            Current = (HWND)GK_GetUserTag(GK_TraveChild(hwnd->pGkWin,Current->pGkWin));
        }
        __GDD_Unlock();
    }
    return ;

}

//----偏移窗口------------------------------------------------------------------
//描述: 偏移一个窗口位置,包括子窗口.该函数为内部调用,非线程安全
//参数：hwnd:窗口句柄.
//      dx,dy: 水平和垂直方向的偏移量.
//返回：无.
//------------------------------------------------------------------------------
static  void    __OffsetWindow(HWND hwnd,s32 dx,s32 dy)
{
    HWND wnd;
    RECT rc;
    struct GkWinObj *Ancestor, *Current;

    Ancestor = hwnd->pGkWin;
    Current = Ancestor;
    Current = GK_TraveScion(Ancestor,Current);
    __OffsetRect(&hwnd->CliRect,dx,dy);
    while(Current != NULL)
    {
        wnd = (HWND)GK_GetUserTag(Current);
        OffsetRect(&wnd->CliRect,dx,dy);
        Current = GK_TraveScion(Ancestor,Current);
    }
    GK_GetArea(hwnd->pGkWin,&rc);
    GK_MoveWin(  hwnd->pGkWin,rc.left+dx,rc.top+dy,0);
}

//----偏移窗口------------------------------------------------------------------
//描述: 偏移一个窗口位置,包括子窗口.
//参数：hwnd:窗口句柄.
//      dx,dy: 水平和垂直方向的偏移量.
//返回：TRUE:成功; FALSE:失败.
//------------------------------------------------------------------------------
bool_t    OffsetWindow(HWND hwnd,s32 dx,s32 dy)
{
    if(hwnd == HWND_Desktop)
    {//桌面不允许移动
        return false;
    }
    if(__GDD_Lock())
    {
        __OffsetWindow(hwnd,dx,dy);
        __GDD_Unlock();
        return true;
    }
    return FALSE;

}

//----移动窗口------------------------------------------------------------------
//描述: 移动一个窗口位置,包括子窗口.该函数为内部调用.非线程安全.
//参数：hwnd:窗口句柄.
//      x,y: 相对于父窗口客户区坐标位置.
//返回：无.
//------------------------------------------------------------------------------
static void __MoveWindow(HWND hwnd,s32 x,s32 y)
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
    GK_GetArea(Ancestor, &rc);//显示区域,显示器的绝对坐标
    wnd=Gdd_GetWindowParent(hwnd);
    ScreenToClient(wnd,(POINT *)&rc,2);//转化为客户区
    dx = x - rc.left;
    dy = y - rc.top;
    Current = GK_TraveScion(Ancestor,Current);
    __OffsetRect(&hwnd->CliRect, dx, dy);
    while(Current != NULL)
    {
        wnd = (HWND)GK_GetUserTag(Current);
        __OffsetRect(&wnd->CliRect,dx,dy);
        Current = GK_TraveScion(Ancestor,Current);
    }
    ClientToScreen(wnd,&point,1);
    ScreenToWindow(wnd,&point,1);
    GK_MoveWin(  hwnd->pGkWin,point.x,point.y,0);
}

//----移动窗口------------------------------------------------------------------
//描述: 移动一个窗口位置,包括子窗口.
//参数：hwnd:窗口句柄.
//      x,y: 相对于父窗口客户区坐标位置.
//返回：TRUE:成功;FALSE:失败.
//------------------------------------------------------------------------------
bool_t    MoveWindow(HWND hwnd,s32 x,s32 y)
{
    if(hwnd == HWND_Desktop)
    {//桌面不允许移动
        return false;
    }
    if(__GDD_Lock())
    {
        __MoveWindow(hwnd,x,y);
        __GDD_Unlock();
        return TRUE;
    }
    return FALSE;
}

//----判断窗口是否可见---------------------------------------------------------
//描述: 略
//参数：hwnd:窗口句柄.
//返回：TRUE:窗口可见;FALSE:窗口不可见.
//------------------------------------------------------------------------------
bool_t    IsWindowVisible(HWND hwnd)
{
    bool_t res=FALSE;

//    if(__HWND_Lock(hwnd))
//    {
        res = GK_IsWinVisible(hwnd->pGkWin);
//        __HWND_Unlock(hwnd);
//    }
    return res;
}

//----设置窗口为无效-----------------------------------------------------------
//描述: 设置窗口为无效的意思是:窗口需要重绘.发送重绘消息便可,该函数为内部调用.
//参数：hwnd:窗口句柄.
//      bErase: 是否擦除背景.
//返回：无
//------------------------------------------------------------------------------
void    __InvalidateWindow(HWND hwnd,bool_t bErase)
{
    if(IsWindowVisible(hwnd))
    {
            PostMessage(hwnd,MSG_PAINT,bErase,0);
    }
}

//----设置窗口为无效-------------------------------------------------------------
//描述: 略
//参数：hwnd:窗口句柄.
//      bErase，true= 需要擦除背景，false=不需要擦除背景
//返回：TRUE:成功; FALSE:失败.
//------------------------------------------------------------------------------
bool_t    InvalidateWindow(HWND hwnd,bool_t bErase)
{
//    if(__HWND_Lock(hwnd))
//    {
        __InvalidateWindow(hwnd,bErase);
//        __HWND_Unlock(hwnd);
        return TRUE;
//    }
//    return FALSE;
}


//----显示窗口-------------------------------------------------------------
//描述: 设置窗口为显示(包括子窗口)
//参数：hwnd:窗口句柄.
//返回：TRUE:成功;FALSE:失败.
//------------------------------------------------------------------------------
bool_t SetWindowShow(HWND hwnd)
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
//----隐藏窗口-------------------------------------------------------------
//描述: 设置窗口为隐藏(包括子窗口)
//参数：hwnd:窗口句柄.
//返回：TRUE:成功;FALSE:失败.
//------------------------------------------------------------------------------
bool_t SetWindowHide(HWND hwnd)
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


bool_t SetWindowRopCode(HWND hwnd, struct RopGroup RopCode)
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

bool_t SetWindowHyalineColor(HWND hwnd,u32 HyalineColor)
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

bool_t SetWindowFillColor(HWND hwnd,u32 FillColor)
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

bool_t GetWindowFillColor(HWND hwnd,u32 *pFillColor)
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



bool_t SetWindowTextColor(HWND hwnd,u32 TextColor)
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

bool_t GetWindowTextColor(HWND hwnd,u32 *pTextColor)
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
//----获得窗口绘图上下文(DC)-----------------------------------------------------
//描述: 该函数获得的DC,可以在整个窗口范围内绘图输出(非客户区+客户区).
//参数：hwnd:窗口句柄.
//返回：DC句柄.
//------------------------------------------------------------------------------
HDC GetWindowDC(HWND hwnd)
{
    DC *pdc;

    pdc =malloc(sizeof(DC));
    if(pdc!=NULL)
    {
        __InitDC(pdc,hwnd->pGkWin,hwnd,DC_TYPE_WINDOW);
    }
    return DC2HDC(pdc);

}

//----获得窗口客户区绘图上下文(DC)-----------------------------------------------
//描述: 该函数获得的DC,只能在窗口客户区范围内绘图输出).
//参数：hwnd:窗口句柄.
//返回：DC句柄.
//------------------------------------------------------------------------------
HDC GetDC(HWND hwnd)
{
    DC *pdc;

    pdc =malloc(sizeof(DC));
    if(pdc!=NULL)
    {
        __InitDC(pdc,hwnd->pGkWin,hwnd,DC_TYPE_CLIENT);
    }
    return DC2HDC(pdc);

}

//----获得窗口客户区绘图上下文(DC)-----------------------------------------------
//描述: 该函数获得的DC,只能在窗口客户区范围内绘图输出).
//参数：hwnd:窗口句柄.
//返回：DC句柄.
//------------------------------------------------------------------------------
bool_t    ReleaseDC(HWND hwnd,HDC hdc)
{
       return  DeleteDC(hdc);
}

//----指定窗口开始绘图-----------------------------------------------------------
//描述: 该函数只能在MSG_PAINT中调用,必须与EndPaint成对使用.
//参数：hwnd: 窗口句柄.
//返回：DC句柄.
//------------------------------------------------------------------------------
HDC BeginPaint(HWND hwnd)
{
    HDC hdc;
    struct WindowMsg msg;

    hdc =malloc(sizeof(DC));
    if(hdc!=NULL)
    {
        __InitDC(hdc,hwnd->pGkWin,hwnd,DC_TYPE_PAINT);

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
                __WinMsgProc(&msg);
            }
            else
                __HWND_Unlock(hwnd);
        }
        return hdc;
    }

    return NULL;
}

//----指定窗口结束绘图-----------------------------------------------------------
//描述: 该函数只能在MSG_PAINT中调用,必须与BeginPaint成对使用.
//参数：hwnd: 窗口句柄.
//      hdc: DC句柄,必须是BeginPaint返回的DC句柄.
//返回：TRUE:成功; FALSE:失败.
//------------------------------------------------------------------------------
bool_t    EndPaint(HWND hwnd,HDC hdc)
{
//  UpdateDisplay(CN_TIMEOUT_FOREVER);
    PostMessage(hwnd, MSG_SYNC_DISPLAY, 0, 0);
    return  DeleteDC(hdc);
}

//----默认的窗口非客户区绘制消息处理函数---------------------------------------
//描述: 绘制窗口的非客户区.
//参数：pMsg: 消息指针.
//返回：无.
//-----------------------------------------------------------------------------
static ptu32_t DefWindowProc_NCPAINT(struct WindowMsg *pMsg)
{
    HWND hwnd=pMsg->hwnd;
    HDC hdc;
    RECT rc;


    hdc =GetWindowDC(hwnd);
    if(NULL!=hdc)
    {
        GetWindowRect(hwnd,&rc);
        ScreenToWindow(hwnd,(POINT*)&rc,2);

        if(__HWND_Lock(hwnd))
        {
            if(hwnd->Style&WS_BORDER)
            {
                SetDrawColor(hdc,WINDOW_BORDER_COLOR);
                DrawRect(hdc,&rc);
                InflateRect(&rc,-1,-1);
            }

            if(hwnd->Style&WS_DLGFRAME)
            {
                SetDrawColor(hdc,WINDOW_DLGFRAME_COLOR1);
                DrawRect(hdc,&rc);
                InflateRect(&rc,-1,-1);

                SetDrawColor(hdc,WINDOW_DLGFRAME_COLOR2);
                DrawRect(hdc,&rc);
                InflateRect(&rc,-1,-1);

                SetDrawColor(hdc,WINDOW_DLGFRAME_COLOR3);
                DrawRect(hdc,&rc);
                InflateRect(&rc,-1,-1);
            }

            if(hwnd->Style&WS_CAPTION)
            {
                rc.bottom =rc.top+hwnd->CaptionSize;

                GradientFillRect(hdc,&rc,RGB(0,100,200),RGB(0,30,100),CN_FILLRECT_MODE_UD);

                SetTextColor(hdc,WINDOW_CAPTION_TEXT_COLOR);
                InflateRect(&rc,-1,-1);
                DrawText(hdc, hwnd->Text, -1, &rc, DT_LEFT | DT_VCENTER);
            }
            __HWND_Unlock(hwnd);
        }
        ReleaseDC(hwnd,hdc);
    }
    return 0;
}

//----默认的窗口客户区擦除背景消息处理函数-------------------------------------
//描述: 使用窗口的默认背景颜色，填充窗口客户区背景.
//参数：pMsg: 消息指针.
//返回：无.
//-----------------------------------------------------------------------------
static ptu32_t DefWindowProc_ERASEBKGND(struct WindowMsg *pMsg)
{
    HDC hdc;
    RECT rc;

    hdc =(HDC)pMsg->Param1;
    if(hdc!=NULL)
    {
        GetClientRect(pMsg->hwnd,&rc);
        FillRect(hdc,&rc);
        return 1;
    }
    return 0;
}

//----默认的窗口客户区绘制消息处理函数-------------------------------------
//描述: 好像没什么事做，先空着。.
//参数：pMsg: 消息指针.
//返回：无.
//-----------------------------------------------------------------------------
static ptu32_t DefWindowProc_PAINT(struct WindowMsg *pMsg)
{
    return true;
}

static ptu32_t DefWindowProc_SYNC(struct WindowMsg *pMsg)
{
    GK_SyncShow(CN_TIMEOUT_FOREVER);
    return true;
}
void __InitMsg(struct WindowMsg *msg,HWND hwnd,u32 code,u32 param1,ptu32_t param2);
//----默认的窗口关闭消息处理函数-------------------------------------
//描述: 关闭窗口，如果是焦点窗口或者是焦点窗口的祖先窗口，就把焦点窗口转移到桌面。
//      从末梢窗口开始，逐个删除窗口数据结构。
//      如果用户窗口定义了消息处理函数，就需要指定为 MSG_ADOPT_NORMAL，否则将有
//      不可预知的结果。
//参数：pMsg: 消息指针.
//返回：无.
//-----------------------------------------------------------------------------
static ptu32_t DefWindowProc_CLOSE(struct WindowMsg *pMsg)
{
    HWND Current;
    struct WindowMsg SubMsg;
    HWND hwnd,next;

    if(__GDD_Lock())
    {
        hwnd = pMsg->hwnd;
        //查找被删除窗口是否focus窗口的祖先窗口，如是，则需要转移focus窗口
        //从被删窗口的兄弟窗口中找到一个允许focus的窗口，设为新的focus窗口。
        //若还是没有找到，则看父窗口是否允许focus，允许则设为focus窗口。
        //否则以 desktop 为 focus 窗口
        if(IsFocusAncestor(hwnd))
        {
            next = GetWindowNext(hwnd);
            while(next != hwnd)
            {
                if( IsFocusEnable(next) )
                {
                    SetFocusWindow(next);
                    break;
                }
                else
                    next = GetWindowNext(hwnd);
            }
            if(next == hwnd)
            {
                if(IsFocusEnable(Gdd_GetWindowParent(next)))
                {
                    SetFocusWindow(Gdd_GetWindowParent(next));
                }
                else
                {
                    SetFocusWindow(GetDesktopWindow( ));
                }
            }
        }
        Current = __GetWindowTwig(hwnd);
        while(Current != NULL)
        {
            //末梢窗口直接调用消息处理函数，必须调用WinMsgProc函数方式实现，这样
            //才能正确调用用户定义的消息处理函数，并且正确处理消息处理函数的继承
            //关系。
            //这里将引起递归，但递归深度只有1级，因为 twig 不会再有子窗口了。
            __InitMsg(&SubMsg, Current, MSG_CLOSE, 0, (ptu32_t)Current->PrivateData);
            __WinMsgProc(&SubMsg);
            Current = __GetWindowTwig(hwnd);
        }
        GK_DestroyWin(hwnd->pGkWin);
        __GDD_Unlock();
    }
    return 0;
}

//----默认的销毁窗口消息处理函数-------------------------------------
//描述: 销毁.
//参数：pMsg: 消息指针.
//返回：无.
//-----------------------------------------------------------------------------
//static bool_t DefWindowProc_DESTROY(struct WindowMsg *pMsg)
//{
//    PostQuitMessage(pMsg->hwnd,0);
//    return TRUE;
//}

//----在消息处理函数表中查找对应的消息-----------------------------------------
//功能：略
//参数：Code：待查找的消息
//      MsgTable，源表，以0结束。
//      Num，表中的消息数量
//返回：若找到，则返回消息在MsgTable中的偏移，否则返回-1
//-----------------------------------------------------------------------------
static s32 GetWinMsgFunc(u32 Code,struct MsgTableLink * msgtablink)
{
    u32 loop = 0;
    u32 CodeTab;
    struct MsgProcTable *MsgTable;
    u32 Num;

    MsgTable = msgtablink->myTable;
    Num = msgtablink->MsgNum;
    if(MsgTable == NULL)//添加到的函数处理表中没有找到
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
//----窗口消息处理函数---------------------------------------------------------
//描述: 处理窗口消息，消息可由hwnd->MyMsgTableLink成员实现覆盖，只要在子窗口中
//      实现同名消息，父窗口的该消息处理函数就会被“替换”掉。
//参数：pMsg: 消息指针.
//返回：消息处理结果.
//-----------------------------------------------------------------------------
ptu32_t __WinMsgProc(struct WindowMsg *pMsg)
{
    HWND hwnd;
    u32 MsgCtrl;
    s32 offset;
    bool_t Adopt = false;
    ptu32_t result = 0;
    struct MsgTableLink *MyTableLinkNode;
//    struct MsgTableLink *MyPrebak;
    struct MsgProcTable *MyTable;
    s32 num = 0;

    hwnd = pMsg->hwnd;
    MyTableLinkNode = hwnd->MyMsgTableLink[num];
//    MyPrebak = &MyTableLinkNode;
//    MyTableLinkNode->pLinkTab = NULL;
//  MyTableLinkNode = Head;
    //这是一级级消息处理继承机制，先从最低一级继承出发
    //以控件的paint消息处理为例说明一下：
    //一般来说，MyMsgTableLink直接指向的是用户的消息表，如果从用户表中找到了
    //      paint消息函数，则调用后返回。否则：
    //      MyMsgTableLink->LinkNext指向的是控件的消息表，里面一般能找到 paint
    //      消息处理函数，调用后返回。否则：
    //      从窗口系统的默认消息处理函数表 s_gDefWindowMsgProcTable中找。
    //这就是类似C++的继承机制。相当于用户继承控件，控件继承窗口系统。

    while(MyTableLinkNode != NULL)
    {
        MyTable = MyTableLinkNode->myTable;
        if(MyTable != NULL)
        {
            offset = GetWinMsgFunc(pMsg->Code & MSG_BODY_MASK, MyTableLinkNode);
            if(offset != -1)
            {
                MsgCtrl =  MyTable[offset].MsgCode & MSG_CONTROL_MSK;
                if((MsgCtrl & MSG_ADOPT_MSK) == MSG_ADOPT_NONE) //不继承
                {
                    if(MyTable[offset].MsgProc !=NULL)
                        result = MyTable[offset].MsgProc(pMsg);
                    while((Adopt == true)&&(num!=0) )
                    {
                        MyTableLinkNode = hwnd->MyMsgTableLink[--num];
                        MyTable = MyTableLinkNode->myTable;
                        offset = GetWinMsgFunc(pMsg->Code,MyTableLinkNode);
                        if((offset != -1)&&(MyTable[offset].MsgProc !=NULL))
                        {
                            result = MyTable[offset].MsgProc(pMsg);
                        }
                    }
                    break;
                }
                else
                {
                    Adopt = true;
                }
            }
        }
        if(MyTableLinkNode == &s_gDefWindowMsgLink)
            MyTableLinkNode =NULL;
        else
            MyTableLinkNode = hwnd->MyMsgTableLink[++num];
    }
    if(pMsg->Code == MSG_CLOSE)
    {
        if(hwnd->Style&WS_CHILD)
            __DeleteChildWindowData(hwnd);
        else
            __DeleteMainWindowData(hwnd);   //主窗口要删除的东西多一些
    }
    return result;

}

//----删除一个主窗口数据结构---------------------------------------------------
//描述：本函数用于创建主窗口函数调用失败时，清除相关数据用。
//参数：需要删除的主窗口句柄
//返回：无.
//------------------------------------------------------------------------------
void __ClearMainWindow(HWND hwnd)
{
    dListRemove(&hwnd->node_msg_close);
    dListRemove(&hwnd->node_msg_ncpaint);
    dListRemove(&hwnd->node_msg_paint);

    __RemoveWindowTimer(hwnd);
    __GUI_DeleteMsgQ(hwnd->pMsgQ);
    GK_DestroyWin(hwnd->pGkWin);
//  UpdateDisplay(CN_TIMEOUT_FOREVER);  //删除尚未生效的窗口，无须刷新
    free(hwnd->pGkWin);
    hwnd->pGkWin =NULL;

    Lock_MutexDelete(hwnd->mutex_lock);
    hwnd->mutex_lock =NULL;
    free(hwnd);

}

//----创建GUI应用---------------------------------------------------------------
//功能：为GUI应用创建一个主窗口，并建立消息循环。
//参数：AppName，Appname，也是窗口名字，将出现在窗口标题栏（如果显示标题栏）
//      MyMsgLink，窗口响应的消息表
//      MemSize，该应用所需的内存量
//      WinBuf，APP使用的主窗口是否带缓冲区，参看 CN_WINBUF_BUF 常量定义
//返回：应用对应的主窗口句柄
//-----------------------------------------------------------------------------
HWND GDD_CreateGuiApp(char *AppName,struct MsgTableLink  *MyMsgLink,
                      u32 MemSize, u32 WinBuf,u32 Style)
{
    HWND result = NULL;
    RECT rc;
    u16 MyEvtt;
    u16 MyEventId;

    GetClientRect(GetDesktopWindow(),&rc);

    //创建主窗口，每个图形应用对应一个主窗口
    result = CreateWindow(AppName, WS_CAN_FOCUS|Style, rc.left, rc.top,
                                 RectW(&rc), RectH(&rc), NULL, 0x0000,
                                 WinBuf, NULL, MyMsgLink);
    if(result != NULL)
    {

        MyEvtt = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0, __MessageLoop,
                                NULL, MemSize, AppName);
        if(MyEvtt != CN_EVTT_ID_INVALID)
        {
            MyEventId=Djy_EventPop(MyEvtt, NULL, 0, (ptu32_t)result, 0, 0);
            if(MyEventId != CN_EVENT_ID_INVALID)
            {
                result->EventID   = MyEventId;
                SetFocusWindow(result);
                SetWindowShow(result); //显示窗口
            }
            else
            {
                Djy_EvttUnregist( MyEvtt );
                __ClearMainWindow(result);
                result = NULL;
            }
        }
        else
        {
            __ClearMainWindow(result);
            result = NULL;
        }
    }

    if(result == NULL)
    {
        debug_printf("gdd","Create APP main window failure\n\r");
    }
    return result;
}

//----等待GUI APP退出----------------------------------------------------------
//功能：阻塞当前应用，等待名为name的APP退出，不可在当前APP中等待，将直接返回false
//参数：name：GUI APP 名字。
//返回：false = 异常退出，在当前APP中调用也返回false
//      true = APP退出。
// ----------------------------------------------------------------------------
void GDD_WaitGuiAppExit(char *AppName)
{
    Djy_WaitEvttCompleted(Djy_GetEvttId(AppName), 1, CN_TIMEOUT_FOREVER);
}

//----GDD窗口模块初始化函数----------------------------------------------------
//描述: 该函数为内部调用.
//参数：无
//返回：无
//------------------------------------------------------------------------------
void    GDD_WindowInit(void)
{
    s_gDefWindowMsgLink.MsgNum = sizeof(s_gDefWindowMsgProcTable)
                                    / sizeof(struct MsgProcTable);
    s_gDefWindowMsgLink.myTable   = s_gDefWindowMsgProcTable;
}

