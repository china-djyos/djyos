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
// Copyright (c) 2014 ����Ȩ�����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������
// 3. ��������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ��������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

//����ģ��: GDD
//����:  LiuWei.
//�汾��V1.0.0
//�ļ�����:�������ؼ�ʵ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2015-03-23
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include    "gdd.h"
#include    <gui/gdd/gdd_private.h>
#include    <gdd_widget.h>


bool_t    MakeProgressRect(RECT *dst,const RECT *src,u32 Range,u32 Val,EN_PB_MODE mode)
{
    s32 a0,a1;

    if(NULL == dst)
    {
        return FALSE;
    }

    if(NULL == src)
    {
        return FALSE;
    }

    //��ֹ���ݹ���������
    if(Range>0x0FFFFFFF)
    {
        Range >>= 16;
        Val >>= 16;
    }
    else if(Range>0x00FFFFFF)
    {
        Range >>= 12;
        Val >>= 12;
    }
    else if(Range>0x000FFFFF)
    {
        Range >>= 8;
        Val >>= 8;
    }

    //��ֹ����Ϊ0�������쳣
    if(Range==0)
    {
        Range=1;
        Val=1;
    }

    if(Val>Range)
    {
        Val=Range;
    }

    switch(mode)
    {
        case    PBM_LEFT:
                a0 =(Val*RectW(src))/Range;
                a1 =RectW(src)-a0;

                dst[0].left =src->left;
                dst[0].top =src->top;
                dst[0].right =src->left+a0;
                dst[0].bottom =src->bottom;

                dst[1].left =src->left+a0;
                dst[1].top =src->top;
                dst[1].right =src->right;
                dst[1].bottom =src->bottom;
                return TRUE;
                ////
        case    PBM_RIGHT:
                a0 =(Val*RectW(src))/Range;
                a1 =RectW(src)-a0;

                dst[0].left =src->left+a1;
                dst[0].top =src->top;
                dst[0].right =src->right;
                dst[0].bottom =src->bottom;

                dst[1].left =src->left;
                dst[1].top =src->top;
                dst[1].right =src->left+a1;
                dst[1].bottom =src->bottom;
                return TRUE;
                ////
        case    PBM_TOP:
                a0 =(Val*RectH(src))/Range;
                a1 =RectH(src)-a0;

                dst[0].left =src->left;
                dst[0].top =src->top;
                dst[0].right =src->right;
                dst[0].bottom =src->top+a0;

                dst[1].left =src->left;
                dst[1].top =src->top+a0;
                dst[1].right =src->right;
                dst[1].bottom =src->bottom;
                return TRUE;
                ////

        case    PBM_BOTTOM:
                a0 =(Val*RectH(src))/Range;
                a1 =RectH(src)-a0;

                dst[0].left =src->left;
                dst[0].top =src->top+a1;
                dst[0].right =src->right;
                dst[0].bottom =src->bottom;

                dst[1].left =src->left;
                dst[1].top =src->top;
                dst[1].right =src->right;
                dst[1].bottom =src->top+a1;
                return TRUE;
                ////
        default:
                return FALSE;


    }
}


//----������(Progressbar)���ƺ���----------------------------------------------------------
//���ܣ����ǽ�����(Progressbar)�ؼ���MSG_PAINT��Ϣ��Ӧ����
//������pMsg����Ϣָ��
//���أ��̶�true//HWND hwnd,PROGRESSBAR_DATA *pPB)//todo �������ݲ�һ��
//-----------------------------------------------------------------------------
static  bool_t ProgressBar_Paint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC hdc;
    RECT m_rc[2];
    RECT rc0;
    s32 gfill_mode=0;
    PROGRESSBAR_DATA *pPB;
    hwnd =pMsg->hwnd;
    pPB=(PROGRESSBAR_DATA*)hwnd->PrivateData;
    hdc =BeginPaint(hwnd);
    if(NULL!=hdc)
    {
        GetClientRect(hwnd,&rc0);
        if(hwnd->Style&PBS_VER)
        {
            gfill_mode =CN_FILLRECT_MODE_LR;
            if(pPB->Flag&PBF_ORG_BOTTOM)
            {
                MakeProgressRect(m_rc,&rc0,pPB->Range,pPB->Pos,PBM_BOTTOM);
            }
            else
            {
                MakeProgressRect(m_rc,&rc0,pPB->Range,pPB->Pos,PBM_TOP);
            }
        }
        else
        {
            gfill_mode =CN_FILLRECT_MODE_UD;
            if(pPB->Flag&PBF_ORG_RIGHT)
            {
                MakeProgressRect(m_rc,&rc0,pPB->Range,pPB->Pos,PBM_RIGHT);
            }
            else
            {
                MakeProgressRect(m_rc,&rc0,pPB->Range,pPB->Pos,PBM_LEFT);
            }
        }

        if(hwnd->Style&PBS_FLAT)
        {
            SetFillColor(hdc,pPB->FGColor);
            FillRect(hdc,&m_rc[0]);
            SetFillColor(hdc,pPB->BGColor);
            FillRect(hdc,&m_rc[1]);
        }
        else
        {

            GradientFillRect(hdc,&m_rc[0],
                            AlphaBlendColor(pPB->FGColor,RGB(250,250,250),100),
                            AlphaBlendColor(pPB->FGColor,RGB(10,10,10),160),
                            gfill_mode);

            GradientFillRect(hdc,&m_rc[1],
                            AlphaBlendColor(pPB->BGColor,RGB(250,250,250),100),
                            AlphaBlendColor(pPB->BGColor,RGB(10,10,10),160),
                            gfill_mode);

        }

        if(pPB->Flag&PBF_SHOWTEXT)
        {
            SetTextColor(hdc,pPB->TextColor);
            DrawText(hdc,hwnd->Text,-1,&rc0,pPB->DrawTextFlag);
        }
        EndPaint(hwnd,hdc);
        return true;
    }
    return false;
}

//----MSG_CREATE��Ϣ��Ӧ����----------------------------------------------------
//���ܣ��������������ݽṹ��
//������pMsg����Ϣָ��
//���أ��ɹ��򷵻�true��ʧ���򷵻�false.
//--------------------------------------------------------------------------
static bool_t ProgressBar_Create(struct WindowMsg *pMsg)
{
     HWND hwnd;
     hwnd =pMsg->hwnd;
     PROGRESSBAR_DATA *pPB;

     if(pMsg->Param1==0)
     {
         pPB =(PROGRESSBAR_DATA*)malloc(sizeof(PROGRESSBAR_DATA));
         pMsg->Param1=(ptu32_t)pPB;
         pPB->Flag =0;
         pPB->Range =100;
         pPB->Pos   =0;
         pPB->FGColor =RGB(200,0,0);
         pPB->BGColor =RGB(0,0,200);
         pPB->DrawTextFlag =DT_VCENTER|DT_CENTER;
     }
     SetWindowPrivateData(hwnd,(void*)pMsg->Param1);
     return true;

}

//----���ý��������ݽṹ��Ϣ��Ӧ����-------------------------------------------
//���ܣ����ý��������ݽṹ��Ϣ��Ӧ����
//������pMsg����Ϣָ��
//���أ��ɹ��򷵻�true��ʧ���򷵻�false.
//-----------------------------------------------------------------------------
static bool_t ProgressBar_SetData(struct WindowMsg *pMsg)
{
    HWND hwnd;
    hwnd =pMsg->hwnd;
    PROGRESSBAR_DATA *pPB;
    if(pMsg->Param1!=0)
    {
        pPB =(PROGRESSBAR_DATA*)GetWindowPrivateData(hwnd);
        if(pPB!=NULL)
        {
            memcpy(pPB,(char*)pMsg->Param1,sizeof(PROGRESSBAR_DATA));
            InvalidateWindow(pMsg->hwnd,FALSE);
            return true;
        }
    }
    return false;
}

//----��ȡ���������ݽṹ��Ϣ��Ӧ����-------------------------------------------
//���ܣ���ȡ���������ݽṹ��Ϣ��Ӧ����
//������pMsg����Ϣָ��
//���أ��ɹ��򷵻�true��ʧ���򷵻�false.
//-----------------------------------------------------------------------------
static bool_t ProgressBar_GetData(struct WindowMsg *pMsg)
{
    HWND hwnd;
    hwnd =pMsg->hwnd;
    PROGRESSBAR_DATA *pPB;
    if(pMsg->Param1!=0)
    {
        pPB =(PROGRESSBAR_DATA*)GetWindowPrivateData(hwnd);
        if(pPB!=NULL)
        {
            memcpy((char*)pMsg->Param1,pPB,sizeof(PROGRESSBAR_DATA));
            InvalidateWindow(pMsg->hwnd,FALSE);
            return true;
        }
    }
    return false;
}

//----���ý�������Χ��Ϣ��Ӧ����-----------------------------------------------
//���ܣ����ý�������Χ��Ϣ��Ӧ����
//������pMsg����Ϣָ��
//���أ��ɹ��򷵻�true��ʧ���򷵻�false.
//-----------------------------------------------------------------------------
static bool_t ProgressBar_SetRange(struct WindowMsg *pMsg)
{
    HWND hwnd;
    hwnd =pMsg->hwnd;
    PROGRESSBAR_DATA *pPB;
    pPB =(PROGRESSBAR_DATA*)GetWindowPrivateData(hwnd);
    if(pPB!=NULL)
    {
        pPB->Range =(u32)pMsg->Param1;
        InvalidateWindow(pMsg->hwnd,FALSE);
        return true;
    }
    return false;
}

//----��ȡ��������Χ��Ϣ��Ӧ����-----------------------------------------------
//���ܣ���ȡ��������Χ��Ϣ��Ӧ����
//������pMsg����Ϣָ��
//���أ��ɹ��򷵻ط�Χֵ��ʧ���򷵻�0.
//-----------------------------------------------------------------------------
static u32 ProgressBar_GetRange(struct WindowMsg *pMsg)
{
    HWND hwnd;
    hwnd =pMsg->hwnd;
    PROGRESSBAR_DATA *pPB;
    pPB =(PROGRESSBAR_DATA*)GetWindowPrivateData(hwnd);
    if(pPB!=NULL)
    {
        return pPB->Range;
    }
    return 0;
}

//----���ý��������ȵ�ǰֵ��Ϣ��Ӧ����-----------------------------------------------
//���ܣ����ý�������Χ��Ϣ��Ӧ����
//������pMsg����Ϣָ��
//���أ��ɹ��򷵻�true��ʧ���򷵻�false.
//-----------------------------------------------------------------------------
static bool_t ProgressBar_SetPos(struct WindowMsg *pMsg)
{
    HWND hwnd;
    hwnd =pMsg->hwnd;
    PROGRESSBAR_DATA *pPB;
    pPB =(PROGRESSBAR_DATA*)GetWindowPrivateData(hwnd);
    if(pPB!=NULL)
    {
        pPB->Pos =(u32)pMsg->Param1;
        InvalidateWindow(pMsg->hwnd,FALSE);
        return true;
    }
    return false;
}

//----��ȡ���������ȵ�ǰֵ��Ϣ��Ӧ����-----------------------------------------------
//���ܣ���ȡ��������Χ��Ϣ��Ӧ����
//������pMsg����Ϣָ��
//���أ��ɹ��򷵻ط�Χֵ��ʧ���򷵻�0.
//-----------------------------------------------------------------------------
static u32 ProgressBar_GetPos(struct WindowMsg *pMsg)
{
    HWND hwnd;
    hwnd =pMsg->hwnd;
    PROGRESSBAR_DATA *pPB;
    pPB =(PROGRESSBAR_DATA*)GetWindowPrivateData(hwnd);
    if(pPB!=NULL)
    {
        return pPB->Pos;
    }
    return 0;
}


//Ĭ�Ͻ�������Ϣ�����������������û���������û�д�������Ϣ��
static struct MsgProcTable s_gProgressBarMsgProcTable[] =
{
    {MSG_ProcessBar_SETDATA,ProgressBar_SetData},
    {MSG_ProcessBar_GETDATA,ProgressBar_GetData},
    {MSG_ProcessBar_SETRANGE,ProgressBar_SetRange},
    {MSG_ProcessBar_GETRANGE,ProgressBar_GetRange},
    {MSG_ProcessBar_SETPOS,ProgressBar_SetPos},
    {MSG_ProcessBar_GETPOS,ProgressBar_GetPos},
    {MSG_CREATE,ProgressBar_Create},
    {MSG_PAINT,ProgressBar_Paint}
};


static struct MsgTableLink  s_gProgressBarMsgLink;

HWND CreateProgressBar(  const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,void *pdata,
                    struct MsgTableLink *UserMsgTableLink)
{
    HWND pGddWin;
    s_gProgressBarMsgLink.MsgNum = sizeof(s_gProgressBarMsgProcTable) / sizeof(struct MsgProcTable);
    s_gProgressBarMsgLink.myTable = (struct MsgProcTable *)&s_gProgressBarMsgProcTable;
    pGddWin = CreateWindow(Text,WS_CHILD|Style,x,y,w,h,hParent,WinId, 
                                CN_WINBUF_PARENT,pdata,&s_gProgressBarMsgLink);
    if(UserMsgTableLink != NULL)
        AddProcFuncTable(pGddWin,UserMsgTableLink);
    return pGddWin;
}


/*============================================================================*/