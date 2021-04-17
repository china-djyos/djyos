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
//����:  zhb.
//�汾��V1.0.0
//�ļ�����: label�ؼ�ʵ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2016-6-14
//   ����:  zhb.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include    "gdd.h"
#include    "../gdd_private.h"
#include    <gdd_widget.h>

/*============================================================================*/
// =============================================================================
// ��������: label�ؼ����ƺ���
// �������: pMsg,������Ϣ�ṹ��ָ��
// �������: �ޡ�
// ����ֵ  :�ޡ�
// =============================================================================
static  bool_t __Widget_LabelPaint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC hdc;
    RECT rc;
    hwnd=pMsg->hwnd;
    hdc =GDD_BeginPaint(hwnd);
    if(NULL!=hdc)
    {
        GDD_GetClientRect(hwnd,&rc);
        GDD_SetFillColor(hdc,RGB(255,0,0));
        GDD_FillRect(hdc,&rc);
        if(hwnd->Style&WS_BORDER)
        {
             GDD_SetDrawColor(hdc,RGB(169,169,169));
             GDD_DrawLine(hdc,0,0,0,GDD_RectH(&rc)-1);      //L
             GDD_DrawLine(hdc,0,0,GDD_RectW(&rc)-1,0);      //U
             GDD_DrawLine(hdc,GDD_RectW(&rc)-1,0,GDD_RectW(&rc)-1,GDD_RectH(&rc)-1); //R
             GDD_DrawLine(hdc,0,GDD_RectH(&rc)-1,GDD_RectW(&rc)-1,GDD_RectH(&rc)-1); //D
        }

        GDD_SetTextColor(hdc,RGB(1,1,1));
        GDD_DrawText(hdc,hwnd->Text,-1,&rc,DT_VCENTER|DT_LEFT);
        GDD_EndPaint(hwnd,hdc);
      }
    return true;
}

//Ĭ�ϱ�ǩ��Ϣ�������������û���������û�д������Ϣ��
static struct MsgProcTable s_gLabelMsgProcTable[] =
{
    {MSG_PAINT,__Widget_LabelPaint}
};

static struct MsgTableLink  s_gLabelMsgLink;

HWND Widget_CreateLabel(  const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,ptu32_t pdata,
                    struct MsgTableLink *UserMsgTableLink)
{
    HWND pGddWin;
    s_gLabelMsgLink.MsgNum = sizeof(s_gLabelMsgProcTable) / sizeof(struct MsgProcTable);
    s_gLabelMsgLink.myTable =(struct MsgProcTable *)&s_gLabelMsgProcTable;
    pGddWin = GDD_CreateWindow(Text, WS_CHILD | Style, x, y, w, h, hParent, WinId,
                            CN_WINBUF_PARENT, pdata, &s_gLabelMsgLink);

    if(UserMsgTableLink != NULL)
        GDD_AddProcFuncTable(pGddWin,UserMsgTableLink);
    return pGddWin;
}
