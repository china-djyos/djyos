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
//作者:  luost.
//版本：V1.0.0
//文件描述: gdd鼠标显示
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2017-01-14
//   作者:  luost.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include <stdint.h>
#include    "gdd_private.h"
#include <gui\gdd_focus.h>


static HWND sg_pMouseHwnd;

//----鼠标绘制函数-------------------------------------------------------------
//功能：这是按钮控件的MSG_PAINT消息响应函数
//参数：pMsg，消息指针
//返回：固定true
//-----------------------------------------------------------------------------
static  bool_t __GDD_MousePaint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC hdc;
    RECT rc;

    hwnd=pMsg->hwnd;
    hdc =GDD_BeginPaint(hwnd);
    if(NULL!=hdc)
    {
        GDD_GetClientRect(hwnd,&rc);
        GDD_SetBackGroundColor(hdc,RGB(255,0,0));
        GDD_FillRectEx(hdc,&rc,hdc->BGColor);

        GDD_SetDrawColor(hdc,RGB(1,1,1));
        GDD_DrawLine(hdc,0,4,8,4);
        GDD_DrawLine(hdc,4,0,4,8);

        GDD_EndPaint(hwnd,hdc);
    }
    return true;
}

//鼠标消息处理函数表，处理用户函数表中没有处理的消息。
static struct MsgProcTable s_gMouseMsgProcTable[] =
{
    {MSG_CREATE,__GDD_MousePaint}
};

static struct MsgTableLink  s_gMouseWinMsgLink;

bool_t GDD_CreateMouseIcon( void )
{
    HWND desktop;
    struct Rectangle rc ;
    struct PointCdn sz;
    s32 x,y;
    struct RopGroup RopCode;
    desktop = GDD_GetDesktopWindow(NULL );
    GK_GetDspSize(desktop->pGkWin, &sz);
    GK_GetScreenArea(desktop->pGkWin, &rc);
    x = sz.x / 2 - rc.left;
    y = sz.y / 2 -rc.top;
    s_gMouseWinMsgLink.MsgNum = sizeof(s_gMouseMsgProcTable) / sizeof(struct MsgProcTable);
    s_gMouseWinMsgLink.myTable = (struct MsgProcTable *)&s_gMouseMsgProcTable;
    sg_pMouseHwnd = GDD_CreateWindow("Mouse_Cursor",&s_gMouseWinMsgLink,x,y, 8, 8,
                                  CN_WINBUF_BUF,0,CN_SYS_PF_DISPLAY, CN_COLOR_WHITE, 0,0,NULL);
    if(sg_pMouseHwnd != NULL)
    {
       GK_SetPrio(sg_pMouseHwnd->pGkWin,CN_WINDOW_ZPRIO_MOUSE , CN_TIMEOUT_FOREVER);
//         GK_SetHyalineColor(sg_pMouseHwnd->pGkWin,RGB(0, 0, 0));
       RopCode = (struct RopGroup){ 0, 0, 0, CN_R2_XORPEN, 0, 0, 0  };
       GK_SetRopCode(sg_pMouseHwnd->pGkWin, RopCode, CN_TIMEOUT_FOREVER);
       return true;
    }
    else
        return false;
}


