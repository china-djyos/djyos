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
//�ļ�����: ��ť�ؼ�ʵ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
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

//----��ť���ƺ���-------------------------------------------------------------
//���ܣ����ǰ�ť�ؼ���MSG_PAINT��Ϣ��Ӧ����
//������pMsg����Ϣָ��
//���أ��̶�true
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

//----Button������Ӧ����---------------------------------------------------------
//���ܣ���
//������pMsg����Ϣָ��
//���أ��̶�true
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
        case    BS_NORMAL:  //���水ť
            hwnd->Style |= BS_PUSHED;
//          GDD_InvalidateWindow(hwnd,FALSE);   //��������Ϣ������ܵ��°�ť��ɾ����
//                                          //InvalidateWindow������SendMessage֮�����
            GDD_PostMessage(GDD_GetWindowParent(hwnd),MSG_NOTIFY,
                                (MSG_BTN_DOWN<<16)|(hwnd->WinId),(ptu32_t)hwnd);
            break;
            ////
        case    BS_HOLD:    //������ť
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
    GDD_InvalidateWindow(hwnd,FALSE);   //��������Ϣ������ܵ��°�ť��ɾ����
                                    //InvalidateWindow������SendMessage֮�����
    return true;
}

//----Button������Ӧ����---------------------------------------------------------
//���ܣ���
//������pMsg����Ϣָ��
//���أ��̶�true
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
            GDD_InvalidateWindow(hwnd,false);   //��������Ϣ������ܵ��°�ť��ɾ����
                                            //InvalidateWindow������SendMessage֮�����
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
            GDD_InvalidateWindow(hwnd,false);   //��������Ϣ������ܵ��°�ť��ɾ��
            GDD_SendMessage(GDD_GetWindowParent(hwnd),MSG_NOTIFY,
                    (MSG_BTN_PEN_MOVE << 16) | (hwnd->WinId), pMsg->Param1);
            break;
        case BS_HOLD:
            break;
    }
    return true;
}

//Ĭ�ϰ�ť��Ϣ�������������û���������û�д������Ϣ��
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

