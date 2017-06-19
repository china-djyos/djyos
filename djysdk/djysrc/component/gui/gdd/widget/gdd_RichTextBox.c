//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
//�ļ�����: ��ť�ؼ�ʵ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2016-6-14
//   ����:  zhb.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#if 0
#include    "gdd.h"
#include  <gui/gdd/gdd_private.h>
#include    <widget.h>

#define CN_MAX_CHAR_ONE_LINE   128
#define CN_MAX_LINES           64
#define CN_HEIGHT_DEFAULT      40
#define CN_WIDTH_DEFAULT       120
typedef struct
{
   u8 cur_byte_idx;        //��ǰ���ַ���;
   bool_t x_scrollbar;     //�Ƿ���ˮƽ���������
   bool_t y_scrollbar;     //�Ƿ�����ֱ���������
   u16 height;             //������
   u16 width;              //������
   u16 cur_line_index;       //��ǰ������
   u16 len;               //RichTextBox�������ַ����ֽ���
   char *pcurchar;        //��ǰָ����ַ�
   char *text;            //RichTextBox�����е��ַ���ɵ��ַ���
}RichTextBox;

/*============================================================================*/
// =============================================================================
// ��������: label�ؼ����ƺ���
// �������: pMsg,������Ϣ�ṹ��ָ��
// �������: �ޡ�
// ����ֵ  :�ޡ�
// =============================================================================
static  void RichTextBox_paint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC hdc;
    RECT rc;
    hwnd=pMsg->hwnd;
    hdc =BeginPaint(hwnd);
    if(NULL!=hdc)
    {
        GetClientRect(hwnd,&rc);
        SetFillColor(hdc,RGB(255,255,255));
        FillRect(hdc,&rc);
        if(hwnd->Style&WS_BORDER)
        {
             if(hwnd->Style&LABEL_BORDER_FIXED3D)
             {
                 SetDrawColor(hdc,RGB(173,173,173));
                 DrawLine(hdc,0,0,0,RectH(&rc)-1); //L
                 SetDrawColor(hdc,RGB(234,234,234));
                 DrawLine(hdc,0,0,RectW(&rc)-1,0);   //U
                 DrawLine(hdc,RectW(&rc)-1,0,RectW(&rc)-1,RectH(&rc)-1); //R
                 DrawLine(hdc,0,RectH(&rc)-1,RectW(&rc)-1,RectH(&rc)-1); //D
             }
             else
             {
                 SetDrawColor(hdc,RGB(169,169,169));
                 DrawLine(hdc,0,0,0,RectH(&rc)-1); //L
                 DrawLine(hdc,0,0,RectW(&rc)-1,0);   //U
                 DrawLine(hdc,RectW(&rc)-1,0,RectW(&rc)-1,RectH(&rc)-1); //R
                 DrawLine(hdc,0,RectH(&rc)-1,RectW(&rc)-1,RectH(&rc)-1); //D
             }
        }
        SetTextColor(hdc,RGB(255,255,0));
        DrawText(hdc,hwnd->Text,-1,&rc,DT_VCENTER|DT_CENTER);
        EndPaint(hwnd,hdc);
      }
}

// =============================================================================
// ��������:��RichTextBox��Text���Ը�ֵ
// �������: pMsg,������Ϣ�ṹ��ָ��
// �������: �ޡ�
// ����ֵ  :�ޡ�
// ˵�����ڵ�ǰ
// =============================================================================
static bool_t RichTextBox_SetText(RichTextBox * pRxtBox,char *ptext)
{

}


// =============================================================================
// ��������:��RichTextBox������ַ���
// �������: pMsg,������Ϣ�ṹ��ָ��
// �������: �ޡ�
// ����ֵ  :�ޡ�
// ˵�����ڵ�ǰ
// =============================================================================
static bool_t RichTextBox_Add(RichTextBox * pRxtBox,u16 line,char *ptext,u8 len)
{

}

// =============================================================================
// ��������:��RichTextBoxָ��λ�ÿ�ʼɾ���ַ���
// �������: pMsg,������Ϣ�ṹ��ָ��
// �������: �ޡ�
// ����ֵ  :�ޡ�
// ˵�����ڵ�ǰ
// =============================================================================
static bool_t RichTextBox_Delete(RichTextBox * pRxtBox,u16 line,u8 idx,u32 len)
{

}

// =============================================================================
// ��������:��RichTextBoxָ��λ�ÿ�ʼ�����ַ���
// �������: pMsg,������Ϣ�ṹ��ָ��
// �������: �ޡ�
// ����ֵ  :�ޡ�
// ˵�����ڵ�ǰ
// =============================================================================
static bool_t RichTextBox_Insert(RichTextBox *pRxtBox,u16 line,u8 idx,\
                                   char *ptext,u32 len)
{

}

// =============================================================================
// ��������: label�ؼ���Ϣ��Ӧ����
// �������: pMsg,������Ϣ�ṹ��ָ��
// �������: �ޡ�
// ����ֵ  :�ޡ�
// =============================================================================
u32 RichTextBox_proc(struct WindowMsg *pMsg)
{
    HWND hwnd;
    RECT rc;
    HDC hdc;
    hwnd =pMsg->hwnd;
    GetWindowRect(hwnd,&rc);
    hdc =BeginPaint(hwnd);
    switch(pMsg->Code)
    {
        case    MSG_CREATE:
                printf("Richtextbox[%04XH]: MSG_CREATE.\r\n",hwnd->WinId);
                return 1;
                ////
        case    MSG_LBUTTON_DOWN:
                {
                     SetDrawColor(hdc,RGB(220,220,220));
                     DrawDottedLine(hdc,0,0,0,RectH(&rc)); //L
                     DrawDottedLine(hdc,0,0,RectW(&rc),0);   //U
                     DrawDottedLine(hdc,RectW(&rc),0,RectW(&rc),RectH(&rc)); //R
                     DrawDottedLine(hdc,0,RectH(&rc),RectW(&rc),RectH(&rc)); //D
                     InvalidateWindow(hwnd,FALSE);
                }
                break;
                ////
        case    MSG_LBUTTON_UP:
                {
//                    switch(_get_button_type(hwnd))
//                    {
//                        case    BS_NORMAL:
//                                hwnd->Style &= ~BS_PUSHED;
//                                PostMessage(GetParent(hwnd),MSG_NOTIFY,(BTN_UP<<16)|(hwnd->WinId),(ptu32_t)hwnd);
//                                break;
//                                ////
//                        case    BS_HOLD:
//                                break;
//                                ////
//                    }
                    InvalidateWindow(hwnd,TRUE);
                }
                break;
                ////
        case    MSG_PAINT:
                RichTextBox_paint(pMsg);
                return 1;
                ////


        default:
                return DefWindowProc(pMsg);

    }
    return 0;

}

#endif


