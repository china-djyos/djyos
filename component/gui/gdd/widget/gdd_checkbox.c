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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

//所属模块: GDD
//作者:  LiuWei.
//版本：V1.0.0
//文件描述: 复选框控件实现
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
#include  <widget/gdd_checkbox.h>


//----CheckBox绘制函数----------------------------------------------------------
//功能：这是CheckBox控件的MSG_PAINT消息响应函数
//参数：pMsg，消息指针
//返回：固定true
//-----------------------------------------------------------------------------
static  bool_t __Widget_CheckBoxPaint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC hdc;
    RECT rc;
    RECT rc0;

    hwnd=pMsg->hwnd;
    hdc =GDD_BeginPaint(hwnd);
    if(NULL!=hdc)
    {
        GDD_GetClientRect(hwnd,&rc0);

        GDD_SetTextColor(hdc,RGB(1,1,1));
        GDD_SetDrawColor(hdc,RGB(40,40,40));
        GDD_SetBackGroundColor(hdc,RGB(200,200,200));

        GDD_FillRectEx(hdc,&rc0,hdc->BGColor);

        if(hwnd->Style&CBS_SELECTED)
        {
//          GDD_CopyRect(&rc,&rc0);
            rc = rc0;
            rc.right =rc.left+GDD_RectH(&rc0);

            GDD_InflateRect(&rc,-2,-2);
            GDD_SetDrawColor(hdc,RGB(70,70,70));
            GDD_DrawRect(hdc,&rc);

            GDD_InflateRect(&rc,-1,-1);
            GDD_SetDrawColor(hdc,RGB(110,110,110));
            GDD_DrawRect(hdc,&rc);

            GDD_InflateRect(&rc,-1,-1);
            GDD_SetBackGroundColor(hdc,RGB(240,240,240));
            GDD_FillRectEx(hdc,&rc,hdc->BGColor);

            GDD_InflateRect(&rc,-4,-4);
            GDD_SetDrawColor(hdc,RGB(150,150,240));
            GDD_DrawRect(hdc,&rc);
            GDD_InflateRect(&rc,-1,-1);
            GDD_SetDrawColor(hdc,RGB(100,100,220));
            GDD_DrawRect(hdc,&rc);

            GDD_InflateRect(&rc,-1,-1);
            GDD_SetBackGroundColor(hdc,RGB(50,50,200));
            GDD_FillRectEx(hdc,&rc,hdc->BGColor);
        }
        else
        {
//          GDD_CopyRect(&rc,&rc0);
            rc = rc0;
            rc.right =rc.left+GDD_RectH(&rc0);

            GDD_InflateRect(&rc,-2,-2);
            GDD_SetDrawColor(hdc,RGB(100,100,100));
            GDD_DrawRect(hdc,&rc);

            GDD_InflateRect(&rc,-1,-1);
            GDD_SetDrawColor(hdc,RGB(160,160,160));
            GDD_DrawRect(hdc,&rc);

            GDD_InflateRect(&rc,-1,-1);
            GDD_SetBackGroundColor(hdc,RGB(220,220,220));
            GDD_FillRectEx(hdc,&rc,hdc->BGColor);

        }

//      GDD_CopyRect(&rc,&rc0);
        rc = rc0;
        GDD_InflateRectEx(&rc,-GDD_RectH(&rc),0,0,0);

        GDD_DrawText(hdc,hwnd->Text,-1,&rc,DT_LEFT|DT_VCENTER);
        GDD_EndPaint(hwnd,hdc);
    }

    return true;

}


//----CheckBox选中响应函数---------------------------------------------------------
//功能：略
//参数：pMsg，消息指针
//返回：固定true
//-----------------------------------------------------------------------------
static bool_t __Widget_CheckBoxDown(struct WindowMsg *pMsg)
{
     HWND hwnd;
     hwnd =pMsg->hwnd;
     if(hwnd->Style&CBS_SELECTED)
     {
        hwnd->Style &= ~CBS_SELECTED;
//      GDD_InvalidateWindow(hwnd,FALSE);   //父窗口消息处理可能导致按钮被删除，
//                                      //InvalidateWindow不能在SendMessage之后调用
        GDD_SendMessage(GDD_GetWindowParent(hwnd),MSG_NOTIFY,(CBN_UNSELECTED<<16)|(hwnd->WinId),(ptu32_t)hwnd);
     }
     else
     {
        hwnd->Style |=  CBS_SELECTED;
        GDD_PostMessage(GDD_GetWindowParent(hwnd),MSG_NOTIFY,(CBN_SELECTED<<16)|(hwnd->WinId),(ptu32_t)hwnd);
     }

     GDD_InvalidateWindow(hwnd,FALSE);   //父窗口消息处理可能导致按钮被删除，
                                     //InvalidateWindow不能在SendMessage之后调用
     return true;

}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----CheckBox不选中响应函数---------------------------------------------------------
//功能：略
//参数：pMsg，消息指针
//返回：固定true
//-----------------------------------------------------------------------------
static bool_t __Widget_CheckBoxUp(struct WindowMsg *pMsg)
{
     return true;
}
#pragma GCC diagnostic pop

//默认复选框消息处理函数表，处理用户函数表中没有处理的消息。
static struct MsgProcTable s_gCheckBoxMsgProcTable[] =
{
    {MSG_LBUTTON_DOWN,__Widget_CheckBoxDown},
    {MSG_LBUTTON_UP,__Widget_CheckBoxUp},
    {MSG_PAINT,__Widget_CheckBoxPaint},
    {MSG_TOUCH_DOWN,__Widget_CheckBoxDown},
    {MSG_TOUCH_UP,__Widget_CheckBoxUp}
};

static struct MsgTableLink  s_gCheckBoxMsgLink;

HWND Widget_CreateCheckBox(  const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,ptu32_t pdata,
                    struct MsgTableLink *UserMsgTableLink)
{
    HWND pGddWin;

    if(hParent == NULL)
        hParent = GDD_GetDesktopWindow(NULL);
    //加锁后，GDD_GetMessage函数将不能立即取出消息，确保 GDD_AddProcFuncTable 函数
    //完成后，即消息处理函数表完整后再取出消息处理。
    if(__GDD_Lock())
    {
        s_gCheckBoxMsgLink.MsgNum = sizeof(s_gCheckBoxMsgProcTable) / sizeof(struct MsgProcTable);
        s_gCheckBoxMsgLink.myTable = (struct MsgProcTable *)&s_gCheckBoxMsgProcTable;
        pGddWin=GDD_CreateWindow(Text,&s_gCheckBoxMsgLink, x,y,w,h,CN_WINBUF_PARENT,
                                 WS_CAN_FOCUS|Style,CN_SYS_PF_DISPLAY, CN_COLOR_WHITE,WinId,pdata,hParent);
        if(UserMsgTableLink != NULL)
            GDD_AddProcFuncTable(pGddWin,UserMsgTableLink);
        __GDD_Unlock();
        return pGddWin;
    }
    else
        return NULL;
}



/*============================================================================*/
