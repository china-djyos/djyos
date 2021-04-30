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
//文件描述: 按钮控件实现
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2014-11-10
//   作者:  LiuWei.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include    "gdd.h"
#include    "../gdd_private.h"
#include    <gdd_widget.h>


/*============================================================================*/

static  s32 GDD_GetButtonType(HWND hwnd)
{
    return hwnd->Style&BS_TYPE_MASK;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----按钮绘制函数-------------------------------------------------------------
//功能：这是按钮控件的MSG_PAINT消息响应函数
//参数：pMsg，消息指针
//返回：固定true
//-----------------------------------------------------------------------------
static  bool_t __Widget_ButtonCreate(struct WindowMsg *pMsg)
{
    return true;
}
#pragma GCC diagnostic pop

static  bool_t __Widget_ButtonPaint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC hdc;
    RECT rc;
    u32 color;
    if(pMsg==NULL)
        return false;
    hwnd=pMsg->hwnd;
    if(hwnd==NULL)
        return false;
    hdc =GDD_BeginPaint(hwnd);
    if(hdc==NULL)
        return false;
    GDD_GetClientRect(hwnd,&rc);
    Widget_GetAttr(hwnd,ENUM_WIDGET_FILL_COLOR,&color);
    GDD_SetFillColor(hdc,color);
    GDD_FillRect(hdc,&rc);
    if(hwnd->Style&WS_DISABLE)
    {
        GDD_FillRect(hdc,&rc);
        GDD_OffsetRect(&rc,1,1);
    }
    else if(hwnd->Style&BS_PUSHED)
    {
        GDD_SetTextColor(hdc,RGB(255,255,255));

        GDD_SetFillColor(hdc,RGB(0,0,0));
        GDD_FillRect(hdc,&rc);
        GDD_OffsetRect(&rc,1,1);
    }
    else
    {
        switch(hwnd->Style&BS_SURFACE_MASK)
        {
            case    BS_NICE:
              GDD_GradientFillRect(hdc,&rc,
                        RGB(210,210,210),RGB(150,150,150),CN_FILLRECT_MODE_UD);
                break;

                case    BS_SIMPLE:
                    GDD_SetDrawColor(hdc,CN_COLOR_BLACK);
                    GDD_DrawLine(hdc,0,0,0,GDD_RectH(&rc)); //L
                    GDD_DrawLine(hdc,0,0,GDD_RectW(&rc),0);   //U

                    GDD_SetDrawColor(hdc,CN_COLOR_BLACK);
                    GDD_DrawLine(hdc,GDD_RectW(&rc),0,GDD_RectW(&rc),GDD_RectH(&rc)); //R
                    GDD_DrawLine(hdc,0,GDD_RectH(&rc),GDD_RectW(&rc),GDD_RectH(&rc)); //D

                    GDD_InflateRect(&rc,-1,-1);

                    break;

                case    BS_FLAT:
                default:
                    GDD_SetFillColor(hdc,RGB(255,255,255));
                    GDD_FillRect(hdc,&rc);
                    break;

            }


        }

    Widget_GetAttr(hwnd,ENUM_WIDGET_TEXT_COLOR,&color);

    GDD_SetTextColor(hdc,color);

    GDD_DrawText(hdc,hwnd->Text,-1,&rc,DT_VCENTER|DT_CENTER);

//    GDD_UpdateDisplay(0);

    GDD_EndPaint(hwnd,hdc);

    return true;
}

//----Button按下响应函数---------------------------------------------------------
//功能：略
//参数：pMsg，消息指针
//返回：固定true
//-----------------------------------------------------------------------------
static  bool_t __Widget_ButtonDown(struct WindowMsg *pMsg)
{
    HWND hwnd;
    if(pMsg==NULL)
        return false;
    hwnd=pMsg->hwnd;
    if(hwnd==NULL)
        return false;

    switch(GDD_GetButtonType(hwnd))
    {
        case    BS_NORMAL:  //常规按钮
            hwnd->Style |= BS_PUSHED;
//          GDD_InvalidateWindow(hwnd,FALSE);   //父窗口消息处理可能导致按钮被删除，
//                                          //InvalidateWindow不能在SendMessage之后调用
            GDD_PostMessage(GDD_GetWindowParent(hwnd),MSG_NOTIFY,
                                (MSG_BTN_DOWN<<16)|(hwnd->WinId),(ptu32_t)hwnd);
            break;
            ////
        case    BS_HOLD:    //自锁按钮
            hwnd->Style ^= BS_PUSHED;
            if(hwnd->Style&BS_PUSHED)
            {
                GDD_PostMessage(GDD_GetWindowParent(hwnd),MSG_NOTIFY,
                                (MSG_BTN_DOWN<<16)|(hwnd->WinId),(ptu32_t)hwnd);
            }
            else
            {
                GDD_PostMessage(GDD_GetWindowParent(hwnd),MSG_NOTIFY,
                                (MSG_BTN_UP<<16)|(hwnd->WinId),(ptu32_t)hwnd);
            }
            break;
            ////

    }
    GDD_InvalidateWindow(hwnd,FALSE);   //父窗口消息处理可能导致按钮被删除，
                                    //InvalidateWindow不能在SendMessage之后调用
    return true;
}

//----Button弹起响应函数---------------------------------------------------------
//功能：略
//参数：pMsg，消息指针
//返回：固定true
//-----------------------------------------------------------------------------
static  bool_t __Widget_ButtonUp(struct WindowMsg *pMsg)
{
    HWND hwnd;
    if(pMsg==NULL)
        return false;
    hwnd=pMsg->hwnd;
    if(hwnd==NULL)
        return false;
    switch(GDD_GetButtonType(hwnd))
    {
        case    BS_NORMAL:
            hwnd->Style &= ~BS_PUSHED;
            GDD_InvalidateWindow(hwnd,false);   //父窗口消息处理可能导致按钮被删除，
                                            //InvalidateWindow不能在SendMessage之后调用
            GDD_SendMessage(GDD_GetWindowParent(hwnd),MSG_NOTIFY,
                                (MSG_BTN_UP<<16)|(hwnd->WinId),(ptu32_t)hwnd);
            break;
            ////
        case    BS_HOLD:
            break;
            ////
    }

    return true;
}


static bool_t __Widget_ButtonMove(struct WindowMsg *pMsg)
{
    HWND hwnd;
    if(pMsg==NULL)
        return false;
    hwnd=pMsg->hwnd;
    if(hwnd==NULL)
        return false;

    switch(GDD_GetButtonType(hwnd))
    {
        case    BS_NORMAL:
            GDD_InvalidateWindow(hwnd,false);   //父窗口消息处理可能导致按钮被删除
            GDD_SendMessage(GDD_GetWindowParent(hwnd),MSG_NOTIFY,
                    (MSG_BTN_PEN_MOVE << 16) | (hwnd->WinId), pMsg->Param1);
            break;
        case BS_HOLD:
            break;
    }
    return true;
}

//默认按钮消息处理函数表，处理用户函数表中没有处理的消息。
static struct MsgProcTable s_gButtonMsgProcTable[] =
{
    {MSG_CREATE,__Widget_ButtonCreate},
    {MSG_LBUTTON_DOWN,__Widget_ButtonDown},
    {MSG_LBUTTON_UP,__Widget_ButtonUp},
    {MSG_PAINT,__Widget_ButtonPaint},
    {MSG_TOUCH_DOWN,__Widget_ButtonDown},
    {MSG_TOUCH_UP,__Widget_ButtonUp},
    {MSG_TOUCH_MOVE,__Widget_ButtonMove},
    {MSG_NCTOUCH_MOVE,__Widget_ButtonMove},
};

static struct MsgTableLink  s_gButtonMsgLink;

HWND Widget_CreateButton(  const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,ptu32_t pdata,
                    struct MsgTableLink *UserMsgTableLink)
{
    HWND pGddWin;
    s_gButtonMsgLink.MsgNum = sizeof(s_gButtonMsgProcTable) / sizeof(struct MsgProcTable);
    s_gButtonMsgLink.myTable = (struct MsgProcTable *)&s_gButtonMsgProcTable;
    pGddWin=GDD_CreateWindow(Text,WS_CHILD | WS_CAN_FOCUS|Style,x,y,w,h,hParent,WinId,
                            CN_WINBUF_PARENT,pdata,&s_gButtonMsgLink);
    if(UserMsgTableLink != NULL)
        GDD_AddProcFuncTable(pGddWin,UserMsgTableLink);
    return pGddWin;
}

