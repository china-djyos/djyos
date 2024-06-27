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
#include    "../gdd_private.h"
#include  <widget/gdd_progressbar.h>


bool_t  Widget_MakeProgressRect(HWND hwnd, RECT *dst,const RECT *src,u32 Range,u32 Val,EN_PB_MODE mode)
{
    s32 a0=0,a1;
    u32 left =0;
    u32 top =0;
    PROGRESSBAR_DATA *pPB;
    pPB=(PROGRESSBAR_DATA*)hwnd->PrivateData;
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
        Range=src->right;
    }

    if(Val>Range)
    {
        Val=Range;
    }

    switch(mode)
    {
        case    PBM_LEFT:

                dst[0].left =pPB->PosBef;
                dst[0].top =src->top;
                dst[0].right =Val;
                dst[0].bottom =src->bottom;
                if(pPB->PosBef ==0&&dst[0].right==0)
                {
                    dst[0].right =Range;
                }
                return TRUE;
                ////
        case    PBM_RIGHT:
                left=Range-Val;
                dst[0].left =left;
                dst[0].top =src->top;
                dst[0].right =pPB->PosBef;
                dst[0].bottom =src->bottom;
                a0=pPB->PosBef-left;
                if(a0<0)
                {
                    a0=-1*a0;
                }
                if(Range%a0!=0)
                {
                    dst[0].right =Range;
                }
                return TRUE;
                ////
        case    PBM_TOP:
                dst[0].left =src->left;
                dst[0].top =pPB->PosBef;
                dst[0].right =src->right;
                dst[0].bottom =Val;
                if(dst[0].bottom ==0&&dst[0].top==0)
                {
                    dst[0].bottom =Range;
                }
                return TRUE;
                ////
        case    PBM_BOTTOM:
                top=Range-Val;
                dst[0].left =src->left;
                dst[0].top =top;
                dst[0].right =src->right;
                dst[0].bottom =pPB->PosBef;
                a0=pPB->PosBef-top;
                if(a0<0)
                {
                    a0=-1*a0;
                }
                if(Range%a0!=0)
                {
                    dst[0].bottom =Range;
                }
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
static  bool_t __Widget_ProgressBarPaint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC hdc;
    RECT m_rc[1];
    RECT rc0;
    s32 left=0,top=0,right=0,bottom=0;
    bool_t judge=TRUE;
    PROGRESSBAR_DATA *pPB;
    hwnd =pMsg->hwnd;
    pPB=(PROGRESSBAR_DATA*)hwnd->PrivateData;
    hdc =GDD_BeginPaint(hwnd);
    if(NULL!=hdc)
    {
        GDD_GetClientRect(hwnd,&rc0);
        if(hwnd->Style&PBS_VER)
        {
            pPB->Range=rc0.bottom;
            if(pPB->Flag&PBF_ORG_BOTTOM)
            {
                Widget_MakeProgressRect(hwnd,m_rc,&rc0,pPB->Range,pPB->Pos,PBM_BOTTOM);
                if(m_rc->top>pPB->PosBef&&m_rc->bottom!=rc0.bottom)
                {
                        top=m_rc->top;
                        bottom=m_rc->bottom;
                        m_rc->top=bottom;
                        m_rc->bottom=top;
                        pPB->PosBef=m_rc->bottom;
                        judge=FALSE;
                        GDD_FillRectEx(hdc,&m_rc[0],pPB->BGColor);
                }
            }
            else
            {
                Widget_MakeProgressRect(hwnd,m_rc,&rc0,pPB->Range,pPB->Pos,PBM_TOP);
                if(pPB->Pos<pPB->PosBef)
                {
                    top=m_rc->top;
                    bottom=m_rc->bottom;
                    m_rc->top=bottom;
                    m_rc->bottom=top;
                    pPB->PosBef=m_rc->bottom;
                    judge=FALSE;
                    GDD_FillRectEx(hdc,&m_rc[0],pPB->BGColor);
                }
            }
        }
        else
        {
            if(pPB->Flag&PBF_ORG_RIGHT)
            {
                Widget_MakeProgressRect(hwnd,m_rc,&rc0,pPB->Range,pPB->Pos,PBM_RIGHT);
                if(m_rc->left>pPB->PosBef&&m_rc->right!=rc0.right)
                {

                        left=m_rc->left;
                        right=m_rc->right;
                        m_rc->left=right;
                        m_rc->right=left;
                        pPB->PosBef=m_rc->right;
                        judge=FALSE;
                        GDD_FillRectEx(hdc,&m_rc[0],pPB->BGColor);
                }
            }
            else
            {
                Widget_MakeProgressRect(hwnd,m_rc,&rc0,pPB->Range,pPB->Pos,PBM_LEFT);
                if(pPB->Pos<pPB->PosBef)
                {
                    left=m_rc->left;
                    right=m_rc->right;
                    m_rc->left=right;
                    m_rc->right=left;
                    pPB->PosBef=m_rc->right;
                    judge=FALSE;
                    GDD_FillRectEx(hdc,&m_rc[0],pPB->BGColor);
                }
            }
        }
        if(judge==TRUE)//�ж��Ƿ��������ϴ��룬��ֹ�ٴν���
        {
            if(pPB->Pos==pPB->PosBef)
            {
                GDD_FillRectEx(hdc,&m_rc[0],pPB->BGColor);
            }else
            {
              if(hwnd->Style&PBS_VER)
              {
                 if(pPB->Flag&PBF_ORG_BOTTOM)
                 {
                     pPB->PosBef=m_rc->top;
                 }
                 else
                 {
                     pPB->PosBef=m_rc->bottom;
                 }
              }
              else
              {
                  if(pPB->Flag&PBF_ORG_RIGHT)
                  {
                      pPB->PosBef=m_rc->left;
                  }
                  else
                  {
                      pPB->PosBef=m_rc->right;
                  }
              }
              GDD_FillRectEx(hdc,&m_rc[0],pPB->FGColor);
            }
        }
        if(pPB->Flag&PBF_SHOWTEXT)
        {
            GDD_SetTextColor(hdc,pPB->TextColor);
            if(pPB->text != NULL)
            {
                GDD_DrawText(hdc,(char*)pPB->text,-1,&rc0,pPB->DrawTextFlag);
            }
            else
            {
                GDD_DrawText(hdc,hwnd->Text,-1,&rc0,pPB->DrawTextFlag);
            }
        }
        GDD_EndPaint(hwnd,hdc);
        return true;
    }
    return false;
}

//----MSG_CREATE��Ϣ��Ӧ����----------------------------------------------------
//���ܣ��������������ݽṹ��
//������pMsg����Ϣָ��
//���أ��ɹ��򷵻�true��ʧ���򷵻�false.
//--------------------------------------------------------------------------
static bool_t __Widget_ProgressBarCreate(struct WindowMsg *pMsg)
{
     HWND hwnd;
     hwnd =pMsg->hwnd;
     PROGRESSBAR_DATA *pPB;
     RECT rc0;
     if(pMsg->Param1==0)
     {
         pPB =(PROGRESSBAR_DATA*)malloc(sizeof(PROGRESSBAR_DATA));
         GDD_GetClientRect(hwnd,&rc0);
         memset(pPB, 0, sizeof(PROGRESSBAR_DATA));
         pMsg->Param1=(ptu32_t)pPB;
         pPB->Flag =PBF_SHOWTEXT|PBF_ORG_LEFT;
         pPB->Pos   =0;
         pPB->FGColor =RGB(0,160,0);;
         pPB->BGColor =RGB(10,10,10);
         pPB->DrawTextFlag =DT_VCENTER|DT_CENTER;
         if(hwnd->Style&PBS_VER)
         {
             pPB->Range = rc0.bottom;
         }else
         {
             pPB->Range = rc0.right;
         }
     }
     GDD_SetWindowPrivateData(hwnd,(ptu32_t)pMsg->Param1);
     GDD_InvalidateWindow(hwnd,FALSE);
     return true;

}

//----���ý��������ݽṹ��Ϣ��Ӧ����-------------------------------------------
//���ܣ����ý��������ݽṹ��Ϣ��Ӧ����
//������pMsg����Ϣָ��
//���أ��ɹ��򷵻�true��ʧ���򷵻�false.
//-----------------------------------------------------------------------------
 bool_t __Widget_ProgressBarSetData(HWND hwnd ,const u32 NewPos,const u32 Flag,const u32 TextColor,const u32 FGColor,const u32 BGColor)
{
    int PosBef;
    PROGRESSBAR_DATA *pPB;
    if(BGColor!=NULL&&Flag!=NULL&&TextColor!=NULL&&TextColor!=NULL&&FGColor!=NULL&&BGColor!=NULL)
    {
        pPB =(PROGRESSBAR_DATA*)GDD_GetWindowPrivateData(hwnd);

        if(pPB!=NULL)
        {
            pPB->Pos=NewPos;
            pPB->Flag=Flag;
            pPB->TextColor=TextColor;
            pPB->FGColor=FGColor;
            pPB->BGColor=BGColor;
            GDD_InvalidateWindow(hwnd,FALSE);
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
 bool_t __Widget_ProgressBarGetData(HWND hwnd, u32 Pos, u32 Flag, u32 TextColor, u32 FGColor, u32 BGColor)
{
        PROGRESSBAR_DATA *pPB;
        pPB =(PROGRESSBAR_DATA*)GDD_GetWindowPrivateData(hwnd);
        if(pPB!=NULL)
        {
            Pos=pPB->Pos;
            Flag=pPB->Flag;
            TextColor=pPB->TextColor;
            FGColor=pPB->FGColor;
            BGColor=pPB->BGColor;
            return true;
        }
        RECT rc0;
        if(pPB->Range!=NULL)
        {
            return pPB->Range;
        }else
        {
            GDD_GetClientRect(hwnd,&rc0);
            if(hwnd->Style&PBS_VER)
            {
                return rc0.bottom;
            }else
            {
                return rc0.right;
            }
        }
    return false;
}

//----���ý�������Χ��Ϣ��Ӧ����-----------------------------------------------
//���ܣ����ý�������Χ��Ϣ��Ӧ����
//������pMsg����Ϣָ��
//���أ��ɹ��򷵻�true��ʧ���򷵻�false.
//-----------------------------------------------------------------------------
 bool_t __Widget_ProgressBarSetRange(HWND hwnd,const PROGRESSBAR_DATA *pbA)
{
    PROGRESSBAR_DATA *pPB;
    pPB =(PROGRESSBAR_DATA*)GDD_GetWindowPrivateData(hwnd);
    if(pPB!=NULL)
    {
        pPB->Range =pbA->Range;
        pPB->TextColor =pbA->TextColor;
        GDD_InvalidateWindow(hwnd,FALSE);
        return true;
    }
    return false;
}

//----��ȡ��������Χ��Ϣ��Ӧ����-----------------------------------------------
//���ܣ���ȡ��������Χ��Ϣ��Ӧ����
//������pMsg����Ϣָ��
//���أ��ɹ��򷵻ط�Χֵ��ʧ���򷵻�0.
//-----------------------------------------------------------------------------
 u32 __Widget_ProgressBarGetRange(HWND hwnd)
{
    PROGRESSBAR_DATA *pPB;
    pPB =(PROGRESSBAR_DATA*)GDD_GetWindowPrivateData(hwnd);
    RECT rc0;
    if(pPB->Range!=NULL)
    {
        return pPB->Range;
    }else
    {
        GDD_GetClientRect(hwnd,&rc0);
        if(hwnd->Style&PBS_VER)
        {
            return rc0.bottom;
        }else
        {
            return rc0.right;
        }
    }
    return 0;
}

//----���ý��������ȵ�ǰֵ��Ϣ��Ӧ����-------------------------------------------
//���ܣ����ý�������Χ��Ϣ��Ӧ����
//������pMsg����Ϣָ��
//���أ��ɹ��򷵻�true��ʧ���򷵻�false.
//-----------------------------------------------------------------------------
bool_t __Widget_ProgressBarSetPos(HWND hwnd,const u32 NewPos)
{
    PROGRESSBAR_DATA *pPB;
    pPB =(PROGRESSBAR_DATA*)GDD_GetWindowPrivateData(hwnd);
    if(pPB!=NULL)
    {
        pPB->Pos =NewPos;
        pPB->Flag =PBF_SHOWTEXT|PBF_ORG_LEFT;
        pPB->TextColor=RGB(240,240,240);
        GDD_PostMessage(hwnd,MSG_PAINT,FALSE,NewPos);
        return true;
    }
    return false;
}

//----��ȡ���������ȵ�ǰֵ��Ϣ��Ӧ����-------------------------------------------
//���ܣ���ȡ��������Χ��Ϣ��Ӧ����
//������pMsg����Ϣָ��
//���أ��ɹ��򷵻ط�Χֵ��ʧ���򷵻�0.
//-----------------------------------------------------------------------------
 u32 __Widget_ProgressBarGetPos(HWND hwnd)
{
    PROGRESSBAR_DATA *pPB;
    pPB =(PROGRESSBAR_DATA*)GDD_GetWindowPrivateData(hwnd);
    if(pPB!=NULL)
    {
        return pPB->Pos;
    }
    return 0;
}


//Ĭ�Ͻ�������Ϣ�������������û���������û�д������Ϣ��
static struct MsgProcTable s_gProgressBarMsgProcTable[] =
{
    {MSG_CREATE,__Widget_ProgressBarCreate},
    {MSG_PAINT,__Widget_ProgressBarPaint}

};


static struct MsgTableLink  s_gProgressBarMsgLink;

HWND Widget_CreateProgressBar(  const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,ptu32_t pdata,
                    struct MsgTableLink *UserMsgTableLink)
{
    HWND pGddWin;
    if(hParent == NULL)
        hParent = GDD_GetDesktopWindow(NULL);
    //������GDD_GetMessage��������������ȡ����Ϣ��ȷ�� GDD_AddProcFuncTable ����
    //��ɺ󣬼���Ϣ����������������ȡ����Ϣ����
    if(__GDD_Lock())
    {
        s_gProgressBarMsgLink.MsgNum = sizeof(s_gProgressBarMsgProcTable) / sizeof(struct MsgProcTable);
        s_gProgressBarMsgLink.myTable = (struct MsgProcTable *)&s_gProgressBarMsgProcTable;
        pGddWin=GDD_CreateWindow(Text,&s_gProgressBarMsgLink, x,y,w,h,CN_WINBUF_PARENT,
                                 Style,CN_SYS_PF_DISPLAY, CN_COLOR_WHITE,WinId,pdata,hParent);
        if(UserMsgTableLink != NULL)
            GDD_AddProcFuncTable(pGddWin,UserMsgTableLink);
        __GDD_Unlock();
        return pGddWin;
    }
    else
        return NULL;
}


/*============================================================================*/
