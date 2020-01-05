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
//�ļ�����: ��ѡ��ؼ�ʵ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include    "gdd.h"
#include    <gui/gdd/gdd_private.h>
#include    <gdd_widget.h>


//----CheckBox���ƺ���----------------------------------------------------------
//���ܣ�����CheckBox�ؼ���MSG_PAINT��Ϣ��Ӧ����
//������pMsg����Ϣָ��
//���أ��̶�true
//-----------------------------------------------------------------------------
static  bool_t CheckBox_Paint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC hdc;
    RECT rc;
    RECT rc0;

    hwnd=pMsg->hwnd;
    hdc =BeginPaint(hwnd);
    if(NULL!=hdc)
    {
        GetClientRect(hwnd,&rc0);

        SetTextColor(hdc,RGB(1,1,1));
        SetDrawColor(hdc,RGB(40,40,40));
        SetFillColor(hdc,RGB(200,200,200));

        FillRect(hdc,&rc0);

        if(hwnd->Style&CBS_SELECTED)
        {
            CopyRect(&rc,&rc0);
            rc.right =rc.left+RectH(&rc0);

            InflateRect(&rc,-2,-2);
            SetDrawColor(hdc,RGB(70,70,70));
            DrawRect(hdc,&rc);

            InflateRect(&rc,-1,-1);
            SetDrawColor(hdc,RGB(110,110,110));
            DrawRect(hdc,&rc);

            InflateRect(&rc,-1,-1);
            SetFillColor(hdc,RGB(240,240,240));
            FillRect(hdc,&rc);

            InflateRect(&rc,-4,-4);
            SetDrawColor(hdc,RGB(150,150,240));
            DrawRect(hdc,&rc);
            InflateRect(&rc,-1,-1);
            SetDrawColor(hdc,RGB(100,100,220));
            DrawRect(hdc,&rc);

            InflateRect(&rc,-1,-1);
            SetFillColor(hdc,RGB(50,50,200));
            FillRect(hdc,&rc);
        }
        else
        {
            CopyRect(&rc,&rc0);
            rc.right =rc.left+RectH(&rc0);

            InflateRect(&rc,-2,-2);
            SetDrawColor(hdc,RGB(100,100,100));
            DrawRect(hdc,&rc);

            InflateRect(&rc,-1,-1);
            SetDrawColor(hdc,RGB(160,160,160));
            DrawRect(hdc,&rc);

            InflateRect(&rc,-1,-1);
            SetFillColor(hdc,RGB(220,220,220));
            FillRect(hdc,&rc);

        }

        CopyRect(&rc,&rc0);
        InflateRectEx(&rc,-RectH(&rc),0,0,0);

        DrawText(hdc,hwnd->Text,-1,&rc,DT_LEFT|DT_VCENTER);
        EndPaint(hwnd,hdc);
    }

    return true;

}


//----CheckBoxѡ����Ӧ����---------------------------------------------------------
//���ܣ���
//������pMsg����Ϣָ��
//���أ��̶�true
//-----------------------------------------------------------------------------
static bool_t CheckBox_Down(struct WindowMsg *pMsg)
{
     HWND hwnd;
     hwnd =pMsg->hwnd;
     if(hwnd->Style&CBS_SELECTED)
     {
        hwnd->Style &= ~CBS_SELECTED;
//      InvalidateWindow(hwnd,FALSE);   //��������Ϣ������ܵ��°�ť��ɾ����
//                                      //InvalidateWindow������SendMessage֮�����
        SendMessage(Gdd_GetWindowParent(hwnd),MSG_NOTIFY,(CBN_UNSELECTED<<16)|(hwnd->WinId),(ptu32_t)hwnd);
     }
     else
     {
        hwnd->Style |=  CBS_SELECTED;
        PostMessage(Gdd_GetWindowParent(hwnd),MSG_NOTIFY,(CBN_SELECTED<<16)|(hwnd->WinId),(ptu32_t)hwnd);
     }

     InvalidateWindow(hwnd,FALSE);   //��������Ϣ������ܵ��°�ť��ɾ����
                                     //InvalidateWindow������SendMessage֮�����
     return true;

}

//----CheckBox��ѡ����Ӧ����---------------------------------------------------------
//���ܣ���
//������pMsg����Ϣָ��
//���أ��̶�true
//-----------------------------------------------------------------------------
static bool_t CheckBox_Up(struct WindowMsg *pMsg)
{
     return true;
}

//Ĭ�ϸ�ѡ����Ϣ�������������û���������û�д������Ϣ��
static struct MsgProcTable s_gCheckBoxMsgProcTable[] =
{
    {MSG_LBUTTON_DOWN,CheckBox_Down},
    {MSG_LBUTTON_UP,CheckBox_Up},
    {MSG_PAINT,CheckBox_Paint},
    {MSG_TOUCH_DOWN,CheckBox_Down},
    {MSG_TOUCH_UP,CheckBox_Up}
};

static struct MsgTableLink  s_gCheckBoxMsgLink;

HWND CreateCheckBox(  const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,ptu32_t pdata,
                    struct MsgTableLink *UserMsgTableLink)
{
    HWND pGddWin;

    s_gCheckBoxMsgLink.MsgNum = sizeof(s_gCheckBoxMsgProcTable) / sizeof(struct MsgProcTable);
    s_gCheckBoxMsgLink.myTable = (struct MsgProcTable *)&s_gCheckBoxMsgProcTable;
    pGddWin=CreateWindow(Text,WS_CHILD | WS_CAN_FOCUS|Style,x,y,w,h,hParent,WinId,
                            CN_WINBUF_PARENT,pdata,&s_gCheckBoxMsgLink);
    if(UserMsgTableLink != NULL)
        AddProcFuncTable(pGddWin,UserMsgTableLink);
    return pGddWin;
}



/*============================================================================*/
