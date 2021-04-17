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
#include <lock.h>
#include <gui/gk_display.h>
#include <hmi-input.h>
#include    "gdd_private.h"
#include "dbug.h"
static tpInputMsgQ sg_ptGddMsgQ;


//-----------------------------------------------------------------------------
//功能：添加输入设备，用于接收来自HMI设备的输入消息。
//参数：用于GDD的输入设备名。注意，非设计用于GDD的输入设备，不放在这里。
//返回：true = 成功初始化，false =失败。
//-----------------------------------------------------------------------------
bool_t GDD_AddInputDev(const char *InputDevName)
{
    bool_t flag;
    enum _STDIN_INPUT_TYPE_ DevType;
    flag = HmiIn_SetFocus((char*)InputDevName,sg_ptGddMsgQ);
    DevType = HmiIn_CheckDevType(InputDevName);
    switch(DevType)
    {
        case EN_HMIIN_MOUSE_2D:
            GDD_CreateMouseIcon( );
            break;
        case EN_HMIIN_MOUSE_3D:
            break;
        case EN_HMIIN_SINGLE_TOUCH:
            break;
        case EN_HMIIN_MUTI_TOUCH:
            break;
        case EN_HMIIN_AREA_TOUCH:
            break;
        default:break;
    }
    return flag;
}

//-----------------------------------------------------------------------------
//功能：将该设备的输入焦点从 设备队列中删除并 设置为默认值
//参数：用于GDD的输入设备名。
//返回：true = 成功初始化，false =失败。
//-----------------------------------------------------------------------------
bool_t GDD_DeleteInputDev(const char *InputDevName)
{
    bool_t flag;
    flag = HmiIn_SetFocus((char*)InputDevName,HmiIn_GetFocusDefault());
    return flag;
}

//----输入设备初始化-----------------------------------------------------------
//功能：创建输入设备队列，用于接收来自HMI设备的输入消息。
//参数：用于GDD的输入设备名列表。注意，非设计用于GDD的输入设备，不放在这里。
//返回：true = 成功初始化，false =失败。
//-----------------------------------------------------------------------------
bool_t GDD_InputDevInit(void)
{
    GDD_StartTimer(GDD_CreateTimer(GDD_GetDesktopWindow( ), CN_HMIINPUT_TIMER_ID, 30));

    sg_ptGddMsgQ = HmiIn_CreatInputMsgQ(20);

    if(sg_ptGddMsgQ == NULL)
    {
        debug_printf("gdd","create HmiInput Msgqueue error\n\r");
        return false;
    }
    else
    {
        return true;
    }
}

void GDD_HmiInput(void)
{
    HWND hwnd;
    struct InputDeviceMsg msg;

    if(HmiIn_ReadMsg(sg_ptGddMsgQ, &msg, 0) == true)
    {
        //单点触摸屏消息，模拟成鼠标消息
        if(msg.input_type == EN_HMIIN_SINGLE_TOUCH)
        {
            struct SingleTouchMsg *TouchMsg;
            static s32 z=0;
            u16 Touch_Msg;
            bool_t NC;
            POINT pt;
            RECT rc;
            TouchMsg = &msg.input_data.SingleTouchMsg;
            pt.x = TouchMsg->x;
            pt.y = TouchMsg->y;
            __GDD_Lock();
            //TouchDown所在窗口.
            if(TouchMsg->display != NULL)
                hwnd = GDD_GetWindowFromPoint(TouchMsg->display->desktop, &pt);
            else
                hwnd = GDD_GetWindowFromPoint(GDD_GetDesktopWindow()->pGkWin, &pt);
            //GDD_MoveWindow(sg_pMouseHwnd, pt.x-4, pt.y-4);
//          GDD_UpdateDisplay(CN_TIMEOUT_FOREVER);
            if(hwnd != NULL)
            {
                GDD_GetClientRectToScreen(hwnd,&rc);
                if(GDD_PtInRect(&rc, &pt))
                    NC = false;
                else
                    NC = true;

                z = TouchMsg->z;
                if(z>0)     //touch按下或者滑动
                {
                    if((TouchMsg->MoveX ==0) && (TouchMsg->MoveY ==0))
                    {
                        if(NC)
                            Touch_Msg = MSG_NCTOUCH_DOWN;
                        else
                            Touch_Msg = MSG_TOUCH_DOWN;
                    }
                    else
                    {
                        if(NC)
                            Touch_Msg = MSG_NCTOUCH_MOVE;
                        else
                            Touch_Msg = MSG_TOUCH_MOVE;
                    }
                }
                else        //touch离开
                {
                    if(NC)
                        Touch_Msg = MSG_NCTOUCH_UP;
                    else
                        Touch_Msg = MSG_TOUCH_UP;
                }
                GDD_PostMessage(hwnd, Touch_Msg,
                            ((u16)(TouchMsg->MoveY) << 16) | (u16)(TouchMsg->MoveX),
                            ((u16)(pt.y) << 16) | (u16)(pt.x));

            }
            __GDD_Unlock();
        }

        if(msg.input_type == EN_HMIIN_KEYBOARD)
        {
            u8* key;
            u8 val,event;
            u32 KeyTime;

            hwnd = GDD_GetFocusWindow();
            if(NULL != hwnd)
            {
                key  = msg.input_data.key_board.key_value;
                KeyTime = msg.input_data.key_board.time;

                val  =key[0];
                event =key[1];

                if(event==0x00)
                {
                    GDD_PostMessage(hwnd, MSG_KEY_DOWN, val, KeyTime);
                }

                if(event==0xF0)
                {
                    GDD_PostMessage(hwnd, MSG_KEY_UP, val, KeyTime);
                }
            }
        }


    }

    return;
}

