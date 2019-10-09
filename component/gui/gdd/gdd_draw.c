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
//�ļ�����: ��ͼ�����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include    <gui/gdd/gdd_private.h>
#include <loc_string.h>
/*============================================================================*/

static  bool_t    BeginDraw(HDC hdc)
{
    return TRUE;
}

static  void    EndDraw(HDC hdc)
{

}


static  void    __LPtoDP(HDC hdc,POINT *pt,s32 count)
{
    RECT rc;
    switch(hdc->DCType)
    {
        case    DC_TYPE_PAINT:
        case    DC_TYPE_CLIENT:
            GK_GetArea(hdc->pGkWin, &rc);
            while(count--)
            {
                pt->x +=(hdc->hwnd->CliRect.left - rc.left);
                pt->y +=(hdc->hwnd->CliRect.top - rc.top);
                pt++;
            }
            break;
            ////

        case    DC_TYPE_WINDOW:
            while(count--)
            {
                //pt->x +=(hdc->hwnd->CliRect.left - hdc->hwnd->WinRect.left);
                //pt->y +=(hdc->hwnd->CliRect.top - hdc->hwnd->WinRect.top);
                //pt++;
            }
            break;

        default:
            break;
            ////
    }
}
/*============================================================================*/

u32 AlphaBlendColor(u32 bk_c,u32 fr_c,u8 alpha)
{
    u8 r1,g1,b1;
    u8 r2,g2,b2;

    r1 =bk_c>>16;
    g1 =bk_c>>8;
    b1 =bk_c;

    r2 =fr_c>>16;
    g2 =fr_c>>8;
    b2 =fr_c;


    r1 =(((r2-r1)*alpha)>>8) + r1;
    g1 =(((g2-g1)*alpha)>>8) + g1;
    b1 =(((b2-b1)*alpha)>>8) + b1;

    return   RGB(r1,g1,b1);

}
/*============================================================================*/

void    UpdateDisplay(u32 timeout)
{
    GK_SyncShow(timeout);
}

/*============================================================================*/
/*============================================================================*/

void    __InitDC(DC *pdc,struct GkWinObj *gk_win,HWND hwnd,s32 dc_type)
{

    pdc->pGkWin     =gk_win;
    pdc->hwnd       =hwnd;
    pdc->DCType     =dc_type;
    pdc->pFontDef   =Font_GetCurFont();
    pdc->pFont      =pdc->pFontDef;
    pdc->pCharset   =Charset_NlsGetCurCharset();

    pdc->CurX       =0;
    pdc->CurY       =0;
    pdc->DrawColor  =hwnd->DrawColor;
    pdc->FillColor  =hwnd->FillColor;
    pdc->TextColor  =hwnd->TextColor;
    pdc->SyncTime   =0;
    //pdc->RopCode    =(struct RopGroup){ 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };
    pdc->RopCode=gk_win->RopCode;

}

bool_t  DeleteDC(HDC hdc)
{
    if(hdc != NULL)
    {
//        GUI_DeleteDefFont(hdc->pFontDef);
        free(hdc);
        return TRUE;
    }
    return FALSE;
}


//----���õ�ǰ��դ��-------------------------------------------------------------
//����: ��.
//������hdc: ��ͼ�����ľ��.
//     RopCode: �µĹ�դ��.
//���أ��ɵĹ�դ��.
//------------------------------------------------------------------------------
struct RopGroup SetRopCode(HDC hdc,struct RopGroup RopCode)
{
    struct RopGroup old=(struct RopGroup){ 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };
    if(hdc!=NULL)
    {
        old =hdc->RopCode;
        hdc->RopCode =RopCode;
    }
    return old;
}

//----��õ�ǰ��դ��-------------------------------------------------------------
//����: ��.
//������hdc: ��ͼ�����ľ��.
//���أ���ǰ��դ��.
//------------------------------------------------------------------------------
struct RopGroup GetRopCode(HDC hdc)
{
    struct RopGroup old=(struct RopGroup){ 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };
    if(hdc!=NULL)
    {
        old =hdc->RopCode;
    }
    return old;

}

//----���õ�ǰ����λ��-----------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���.
//������hdc: ��ͼ�����ľ��.
//      x,y: �µ�����λ��.
//     old_pt: ����ɵ�����λ��,����ò���ΪNULL,�ղ�����ɵ�����λ��.
//���أ���.
//------------------------------------------------------------------------------
static  void    __MoveTo(HDC hdc,s32 x,s32 y,POINT *old_pt)
{

    if(old_pt!=NULL)
    {
        old_pt->x =hdc->CurX;
        old_pt->y =hdc->CurY;
    }

    hdc->CurX =x;
    hdc->CurY =y;
}

//----���õ�ǰ����λ��-----------------------------------------------------------
//����: ��ǰ����λ�û�Ӱ��LineTo���ߵ���ʼλ��.
//������hdc: ��ͼ�����ľ��.
//      x,y: �µ�����λ��.
//     old_pt: ����ɵ�����λ��,����ò���ΪNULL,�ղ�����ɵ�����λ��.
//���أ���.
//------------------------------------------------------------------------------
void    MoveTo(HDC hdc,s32 x,s32 y,POINT *old_pt)
{
    if(hdc!=NULL)
    {
        __MoveTo(hdc,x,y,old_pt);
    }
}

//----���õ�ǰ������ɫ-----------------------------------------------------------
//����: ��ǰ������ɫ��Ӱ�����л����ຯ��,��DrawRect,DrawCircle...
//������hdc: ��ͼ�����ľ��.
//      color: �µĻ�����ɫ.
//���أ��ɵĻ�����ɫ.
//------------------------------------------------------------------------------
u32 SetDrawColor(HDC hdc,u32 color)
{
    u32 old;

    if(hdc!=NULL)
    {
        old =hdc->DrawColor;
        hdc->DrawColor = color;
    }
    else
    {
        old =0;
    }
    return old;
}

//----��õ�ǰ������ɫ-----------------------------------------------------------
//����: ��.
//������hdc: ��ͼ�����ľ��.
//���أ���ǰ������ɫ.
//------------------------------------------------------------------------------
u32 GetDrawColor(HDC hdc)
{
    u32 old;

    if(hdc!=NULL)
    {
        old =hdc->DrawColor;
    }
    else
    {
        old =0;
    }
    return old;
}

//----���õ�ǰ�����ɫ-----------------------------------------------------------
//����: ��ǰ�����ɫ��Ӱ����������ຯ��,��FillRect,FillCircle...
//������hdc: ��ͼ�����ľ��.
//      color: �µ������ɫ.
//���أ��ɵĻ�����ɫ.
//------------------------------------------------------------------------------
u32 SetFillColor(HDC hdc,u32 color)
{
    u32 old;

    if(hdc!=NULL)
    {
        old =hdc->FillColor;
        hdc->FillColor = color;
    }
    else
    {
        old =0;
    }
    return old;
}

//----��õ�ǰ�����ɫ-----------------------------------------------------------
//����: ��.
//������hdc: ��ͼ�����ľ��.
//���أ���ǰ�����ɫ.
//------------------------------------------------------------------------------
u32 GetFillColor(HDC hdc)
{
    u32 old;

    if(hdc!=NULL)
    {
        old =hdc->FillColor;
    }
    else
    {
        old =0;
    }
    return old;
}

//----���õ�ǰ������ɫ-----------------------------------------------------------
//����: ��ǰ������ɫ��Ӱ���������ֻ����ຯ��,��TextOut,DrawText...
//������hdc: ��ͼ�����ľ��.
//      color: �µ�������ɫ.
//���أ��ɵ�������ɫ.
//------------------------------------------------------------------------------
u32 SetTextColor(HDC hdc,u32 color)
{
    u32 old;

    if(hdc!=NULL)
    {
        old =hdc->TextColor;
        hdc->TextColor = color;
    }
    else
    {
        old =0;
    }
    return old;
}

//----��õ�ǰ������ɫ-----------------------------------------------------------
//����: ��.
//������hdc: ��ͼ�����ľ��.
//���أ���ǰ������ɫ.
//------------------------------------------------------------------------------
u32 GetTextColor(HDC hdc)
{
    u32 old;

    if(hdc!=NULL)
    {
        old =hdc->TextColor;
    }
    else
    {
        old =0;
    }
    return old;
}

//----���õ�ǰͬ��ʱ��-----------------------------------------------------------
//����: ��
//������hdc: ��ͼ�����ľ��.
//      sync_time: �µ�ͬ��ʱ��.
//���أ��ɵ�ͬ��ʱ��.
//------------------------------------------------------------------------------
u32 SetSyncTime(HDC hdc,u32 sync_time)
{
    u32 old;

    if(hdc!=NULL)
    {
        old =hdc->SyncTime;
        hdc->SyncTime = sync_time;
    }
    else
    {
        old =0;
    }
    return old;
}

//----��õ�ǰͬ��ʱ��-----------------------------------------------------------
//����: ��
//������hdc: ��ͼ�����ľ��.
//���أ���ǰͬ��ʱ��.
//------------------------------------------------------------------------------
u32 GetSyncTime(HDC hdc)
{
    u32 old;

    if(hdc!=NULL)
    {
        old =hdc->SyncTime;
    }
    else
    {
        old =0;
    }
    return old;
}

//----��������---------------------------------------------------------------
//����: ���þ�����������壬�ڸ�DC����ʾ�ַ���ʱ����ʹ�ø�����
//������hdc: ��ͼ�����ľ��.
//      hFont: ��ǰ������.
//���أ��ɵ�������.
//------------------------------------------------------------------------------
struct FontObj *SetFont(HDC hdc, struct FontObj *hFont)
{
    struct FontObj old;

    if(hdc!=NULL)
    {
        old =hdc->pFont;
        hdc->pFont = hFont;
    }
    else
    {
        old =NULL;
    }
    return old;

}

//----�������---------------------------------------------------------------
//����: ȡ�� DC ����������
//������hdc: ��ͼ�����ľ��.
//���أ���ǰ������.
//------------------------------------------------------------------------------
struct FontObj *GetFont(HDC hdc)
{
    HFONT old;

    if(hdc!=NULL)
    {
        old =hdc->pFont;
    }
    else
    {
        old =NULL;
    }
    return old;

}

//----�����ַ���------------------------------------------------------------
//����: ���þ���������ַ������ڸ�DC����ʾ�ַ���ʱ����ʹ�ø��ַ���
//������hdc: ��ͼ�����ľ��.
//      hFont: ��ǰ�ַ������.
//���أ��ɵ�������.
//------------------------------------------------------------------------------
struct Charset *SetCharset(HDC hdc,struct Charset *pCharset)
{
    struct Charset * old;

    if(hdc!=NULL)
    {
        old = hdc->pCharset;
        hdc->pFont = pCharset;
    }
    else
    {
        old =NULL;
    }
    return old;

}

//----����ַ���---------------------------------------------------------------
//����: ȡ�� DC �������ַ���
//������hdc: ��ͼ�����ľ��.
//���أ���ǰ�ַ������.
//------------------------------------------------------------------------------
struct Charset *GetCharset(HDC hdc)
{
    struct Charset * old;

    if(hdc!=NULL)
    {
        old =hdc->pCharset;
    }
    else
    {
        old =NULL;
    }
    return old;

}

//----����ָ��λ��������ɫ-------------------------------------------------------
//����: ��
//������hdc: ��ͼ�����ľ��.
//      x,y: ����.
//      color:��ɫֵ.
//���أ���.
//------------------------------------------------------------------------------
void SetPixel(HDC hdc,s32 x,s32 y,u32 color)
{
    POINT pt;
    if(BeginDraw(hdc))
    {
        pt.x =x;
        pt.y =y;
        __LPtoDP(hdc,&pt,1);
        GK_SetPixel(hdc->pGkWin,pt.x,pt.y,color,hdc->RopCode.Rop2Mode,hdc->SyncTime);
        EndDraw(hdc);
    }
}

//----����----------------------------------------------------------------------
//����: ���Ƶ������ؿ������ֱ��,ʹ��DrawColor��Ϊ��ɫֵ.
//������hdc: ��ͼ�����ľ��.
//      x0,y0: ��ʼ����.
//      x1,y1: ��������.
//���أ���.
//------------------------------------------------------------------------------
void    DrawLine(HDC hdc,s32 x0,s32 y0,s32 x1,s32 y1)
{
    POINT pt[2];

    if(BeginDraw(hdc))
    {
        pt[0].x =x0;
        pt[0].y =y0;
        pt[1].x =x1;
        pt[1].y =y1;
        __LPtoDP(hdc,pt,2);

        GK_LinetoIe(hdc->pGkWin,pt[0].x,pt[0].y,pt[1].x,pt[1].y,
                hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
    }
}

void    DrawDottedLine(HDC hdc,s32 x0,s32 y0,s32 x1,s32 y1)
{
    POINT pt[2];
    u32 i,num;
    s32 dot;
    s8 temp=1;
    if(BeginDraw(hdc))
    {
        if(x0==x1)
        {
            dot=y1-y0;
            if(dot==0)
                return;
            else if(dot<0)
            {
                dot=-dot;
                temp=-1;
            }
            else
            {
            }
            if(dot%2!=0)
                dot=dot+1;
            num=dot/2;
            for(i=1;i<=num;i++)
            {
                 pt[0].x =x0;
                 pt[0].y =y0+2*temp*(i-1);
                 pt[1].x =x1;
                 pt[1].y =y0+2*temp*(i);
                 __LPtoDP(hdc,pt,2);
                GK_LinetoIe(hdc->pGkWin,pt[0].x,pt[0].y,pt[1].x,pt[1].y,
                        hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
            }
        }
        else if(y0==y1)
        {
          dot=x1-x0;
          if(dot==0)
              return;
          else if(dot<0)
          {
              dot=-dot;
              temp=-1;
          }
          else
          {
          }
          if(dot%2!=0)
                dot=dot+1;
          num=dot/2;
          for(i=1;i<=num;i++)
          {
             pt[0].x =x0+2*temp*(i-1);
             pt[0].y =y0;
             pt[1].x =x0+2*temp*(i);
             pt[1].y =y1;
             __LPtoDP(hdc,pt,2);
            GK_LinetoIe(hdc->pGkWin,pt[0].x,pt[0].y,pt[1].x,pt[1].y,
                    hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
          }
        }
        else
        {
        }
    }
}
//----����----------------------------------------------------------------------
//����: ʹ��ָ����ɫ���Ƶ������ؿ������ֱ��.
//������hdc: ��ͼ�����ľ��.
//      x0,y0: ��ʼ����.
//      x1,y1: ��������.
//���أ���.
//------------------------------------------------------------------------------
void    DrawLineEx(HDC hdc,s32 x0,s32 y0,s32 x1,s32 y1,u32 color)
{
    POINT pt[2];

    if(BeginDraw(hdc))
    {
        pt[0].x =x0;
        pt[0].y =y0;
        pt[1].x =x1;
        pt[1].y =y1;
        __LPtoDP(hdc,pt,2);

        GK_LinetoIe(hdc->pGkWin,pt[0].x,pt[0].y,pt[1].x,pt[1].y,
                color,hdc->RopCode.Rop2Mode,hdc->SyncTime);
    }
}

//----ʹ�õ�ǰλ�û���-----------------------------------------------------------
//����: ���Ƶ������ؿ������ֱ��,ʹ��DrawColor��Ϊ��ɫֵ,ʹ�õ�ǰλ����Ϊ��ʼ����,
//     ������ɺ�,���������������Ϊ��ǰ����λ��.
//������hdc: ��ͼ�����ľ��.
//      x,y: ��������.
//���أ���.
//------------------------------------------------------------------------------
void    DrawLineTo(HDC hdc,s32 x,s32 y)
{
    POINT pt[2];

    if(BeginDraw(hdc))
    {
        pt[0].x =hdc->CurX;
        pt[0].y =hdc->CurY;
        pt[1].x =x;
        pt[1].y =y;

        __LPtoDP(hdc,pt,2);

        GK_LinetoIe(hdc->pGkWin,pt[0].x,pt[0].y,pt[1].x,pt[1].y,
                hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);

        __MoveTo(hdc,x,y,NULL);
        EndDraw(hdc);
    }
}

//----��ָ��λ�û����ַ���-------------------------------------------------------
//����: ��ָ��λ�û����ַ���,ʹ��TextColor��Ϊ��ɫֵ,֧�ֻس��뻻�з�.
//������hdc: ��ͼ�����ľ��.
//      x,y: ����λ��.
//      text: �ַ���.
//      count: Ҫ���Ƶ��ַ�����,�ò���С��0ʱ,�����������ַ���.
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
bool_t    TextOut(HDC hdc,s32 x,s32 y,const char *text,s32 count)
{
        POINT pt;

        if(BeginDraw(hdc))
        {
            pt.x =x;
            pt.y =y;
            __LPtoDP(hdc,&pt,1);

            x =pt.x;
            y =pt.y;

//            if(count<0)
//            {
//                count = strlen(text);
//            }

            if(text!=NULL)
            {
               GK_DrawText(hdc->pGkWin,hdc->pFont,hdc->pCharset,
                                      x,y,text,count,hdc->TextColor,
                                      hdc->RopCode.Rop2Mode,hdc->SyncTime);

            }

            EndDraw(hdc);
            return TRUE;
        }

        return FALSE;
}

//----�����ַ�����ռ���ؿ��-----------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������Ϸ���.
//������ft: ʹ�õ�������
//     text: �ַ���.
//     count: Ҫ������ַ�����,�ò���С��0ʱ,�����������ַ�����ռ���ؿ��.
//���أ��ַ������ؿ��.
//------------------------------------------------------------------------------
static  s32 __GetTextWidth(HDC hdc,const char *text,s32 count)
{
    s32 width;
    struct Charset *Set;
    struct FontObj *Font;
    s32 len, loop;
    u32 wc;

    width=0;
    Set = hdc->pCharset;
    if(Set!=NULL)
    {
        Font = hdc->pFont;
        if(Font!=NULL)
        {
            for(loop = 0; (loop < count) || (count == -1);loop++)
            {
                len= Set->MbToUcs4(&wc, text, -1);
                if(len == -1)
                { // ��Ч�ַ�
                    text++;
                }
                else if(len == 0)
                {
                    break;
                }
                else
                { // ��Ч�ַ�
                    text += len;
                    width += Font->GetCharWidth(wc);
                }
            }
        }
    }
    return width;
}

//----�����ַ�����ռ���δ�С-----------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������Ϸ���.
//������ft: ʹ�õ�������
//     text: �ַ���.
//     count: Ҫ������ַ�����,�ò���С��0ʱ,�����������ַ�����ռ���ؿ��.
//     rect: ������ľ���.
//     flag: �ַ����Ʊ��.
//���أ���.
//------------------------------------------------------------------------------
void AdjustTextRect(HDC hdc,const char *text,s32 count, RECT *prc,u32 flag)
{
    s32 Height;
    s32 i;

    Height = Font_GetFontLineHeight(hdc->pFont);
    if(1)
    {
        switch(flag&DT_ALIGN_V_MASK)
        {
            case    DT_VCENTER:
                    prc->top += ((RectH(prc) - Height)>>1);
                    prc->bottom = prc->top + Height;
                    break;
                    ////
            case    DT_TOP:
                    break;
                    ////
            case    DT_BOTTOM:
                    prc->top += (RectH(prc) - Height);
                    prc->bottom =prc->top + Height;
                    break;
                    ////
            default:
                    break;

        }

        switch(flag&DT_ALIGN_H_MASK)
        {
            case    DT_CENTER:
                    i =__GetTextWidth(hdc,text,count);
                    i =(RectW(prc)-i)>>1;
                    prc->left  += i;
                    prc->right -= i;
                    break;
                    ////
            case    DT_LEFT:
                    prc->left  += 1;
                    break;
                    ////
            case    DT_RIGHT:
                    i =__GetTextWidth(hdc,text,count);
                    prc->left = prc->right-i-1;
                    break;
                    ////
            default:
                    break;

        }


    }
}

//----����һ���ַ����ж�����-----------------------------------------------------
//����: ��
//������str: �ַ���ָ��
//���أ��ַ�������.
//------------------------------------------------------------------------------
s32  GetStrLineCount(const char *str)
{
    s32 count,linenum = 0;
    const char *line=str;
    char *linenext;
    count=0;
    while(1)
    {
        linenext = mbstrchr(line, "\n", &count);
        if(linenext != NULL)
        {
            linenum++;
            line = linenext+1;
        }
        else
        {
            if(count != 0)
                linenum++;
            break;
        }
    }
    return linenum;
}

//----�ھ��η�Χ�ڻ����ַ���---------------------------------------------------
//����: ��ָ�����η�Χ�ڻ����ַ���,ʹ��TextColor��Ϊ��ɫֵ,֧�ֻس��뻻�з�,
//      �ú�������ָ���Ƿ�����ַ����߿�ͱ���,��ǰָ�����뷽ʽ�����.
//������hdc: ��ͼ�����ľ��.
//      x,y: ����λ��.
//      count: Ҫ���Ƶ��ַ�����,�ò���С��0ʱ,�����������ַ���.
//      prc: Ҫ���Ƶ����ھ���λ�úʹ�С.
//      flag: ���Ʊ�־������DT_VCENTER �峣��
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//-----------------------------------------------------------------------------
bool_t    DrawText(HDC hdc,const char *text,s32 count,const RECT *prc,u32 flag)
{

    RECT rc,rc0;
    s32 line_count,charnum;

    if(NULL==prc)
    {
        return FALSE;
    }

    if(NULL==text)
    {
        return FALSE;
    }

    line_count =GetStrLineCount(text);

    if(BeginDraw(hdc))
    {
        if(line_count<=1)
        {
            //����
            CopyRect(&rc0,prc);
            CopyRect(&rc,prc);
            InflateRect(&rc,-1,-1);
            AdjustTextRect(hdc,text,count,&rc,flag);

            if(flag&DT_BORDER)
            {
                DrawRect(hdc,&rc0);
                InflateRect(&rc0,-1,-1);
            }

            if(flag&DT_BKGND)
            {
                FillRect(hdc,&rc0);
            }

           TextOut(hdc,rc.left,rc.top,text,count);

        }
        else
        {
            //����
            s32 x0,y0,line_h;
            char *p0,*p1;

            line_h = Font_GetFontLineHeight(hdc->pFont);

            CopyRect(&rc0,prc);
            CopyRect(&rc,prc);
            InflateRect(&rc,-1,-1);

            if(flag&DT_BORDER)
            {
                DrawRect(hdc,&rc0);
                InflateRect(&rc0,-1,-1);
            }

            if(flag&DT_BKGND)
            {
                FillRect(hdc,&rc0);
            }

            y0 =rc.top;
            switch(flag&DT_ALIGN_V_MASK)
            {
                case    DT_VCENTER://(line_count-1)*line_h)Ϊ�м��
                    y0 += ((RectH(&rc)-(line_count*line_h)-((line_count-1)*line_h))/2);
                        break;
                        ////
                case    DT_TOP:
                        break;
                        ////
                case    DT_BOTTOM://(line_count-1)*line_h)Ϊ�м��
                        y0 += (RectH(&rc) - (line_count*line_h)-((line_count-1)*line_h));
                        break;
                        ////
                default:
                        break;

            }

            p0 =(char*)text;
            while(p0!=NULL)
            {
                p1 = mbstrchr(p0,"\n",&charnum);

                if(p1 == p0)    //�����������з�
                {
                    p0++;       //���з��϶���1���ֽڵ�
                }
                else
                {
                    x0 =rc.left;
                    switch(flag&DT_ALIGN_H_MASK)
                    {
                        case    DT_CENTER:
                                x0 =(RectW(&rc)-__GetTextWidth(hdc,p0,charnum))>>1;
                                break;
                                ////
                        case    DT_LEFT:
                                x0  += 1;
                                break;
                                ////
                        case    DT_RIGHT:
                                x0 =(RectW(&rc)-__GetTextWidth(hdc,p0,charnum));
                                break;
                                ////
                        default:
                                break;

                    }

                    TextOut(hdc,x0,y0,p0,charnum);

                    if(p1==NULL)
                    {   //���һ����ʾ���,���˳�
                        break;
                    }

                    p0 = p1++;      //�������з����϶���1���ֽ�
                }
                y0 += line_h;
            }
        }

        EndDraw(hdc);
        return TRUE;
    }

    return FALSE;

}

//----���ƾ���------------------------------------------------------------------
//����: ʹ��DrawColor����һ�����ľ���.
//������hdc: ��ͼ�����ľ��.
//      prc: ���β���.
//���أ���.
//------------------------------------------------------------------------------
void    DrawRect(HDC hdc,const RECT *prc)
{
    s32 x0,y0;
    s32 x1,y1;
    RECT rc;

    if(prc != NULL)
    {
        CopyRect(&rc,prc);
        if(BeginDraw(hdc))
        {
            __LPtoDP(hdc,(POINT*)&rc,2);

            x0 =rc.left;
            y0 =rc.top;
            x1 =rc.right-1;
            y1 =rc.bottom-1;

            //Left
            GK_LinetoIe(hdc->pGkWin,x0,y0,x0,y1,hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
            //Top
            GK_LinetoIe(hdc->pGkWin,x0,y0,x1,y0,hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
            //Right
            GK_LinetoIe(hdc->pGkWin,x1,y0,x1,y1,hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
            //Bottom
            GK_LinetoIe(hdc->pGkWin,x0,y1,x1,y1,hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);

            EndDraw(hdc);
        }
    }

}

//----������------------------------------------------------------------------
//����: ʹ��FillColor���һ��ʵ�ľ���.
//������hdc: ��ͼ�����ľ��.
//      prc: ���β���.
//���أ���.
//------------------------------------------------------------------------------
void    FillRect(HDC hdc,const RECT *prc)
{
    RECT rc;

    if(hdc!=NULL)
    {
        if(prc!=NULL)
        {
            CopyRect(&rc,prc);
            if(BeginDraw(hdc))
            {
                __LPtoDP(hdc,(POINT*)&rc,2);

                GK_FillRect(hdc->pGkWin,&rc,hdc->FillColor,hdc->FillColor,
                            CN_FILLRECT_MODE_N,hdc->SyncTime);
                EndDraw(hdc);
            }
        }
    }

}
//----������------------------------------------------------------------------
//����: ʹ��ָ����ɫ���һ��ʵ�ľ���.
//������hdc: ��ͼ�����ľ��.
//      prc: ���β���.
//���أ���.
//------------------------------------------------------------------------------
void    FillRectEx(HDC hdc,const RECT *prc,u32 color)
{
    RECT rc;

    if(hdc!=NULL)
    {
        if(prc!=NULL)
        {
            CopyRect(&rc,prc);
            if(BeginDraw(hdc))
            {
                __LPtoDP(hdc,(POINT*)&rc,2);

                GK_FillRect(hdc->pGkWin,&rc,color,color,
                            CN_FILLRECT_MODE_N,hdc->SyncTime);
                EndDraw(hdc);
            }
        }
    }

}

//----����������------------------------------------------------------------------
//����: ʹ��Color1��Ϊ��ʼɫ,Color2��Ϊ����ɫ,����������.
//������hdc: ��ͼ�����ľ��.
//      prc: ���β���.
//      Color1: ��ʼ��ɫ.
//      Color2: ������ɫ.
//      mode: �������ģʽ,����������ֵ:
//            CN_FILLRECT_MODE_LR:���������
//            CN_FILLRECT_MODE_UD:���ϵ���
//            CN_FILLRECT_MODE_LU2RD:�����ϵ�����
//            CN_FILLRECT_MODE_RU2LD:�����ϵ�����
//���أ���.
//------------------------------------------------------------------------------
void    GradientFillRect(HDC hdc,const RECT *prc,u32 Color1,u32 Color2,u32 mode)
{
    struct Rectangle gk_rc;
    RECT rc;

    if(hdc!=NULL)
    {
        if(prc!=NULL)
        {
            if(BeginDraw(hdc))
            {
                CopyRect(&rc,prc);

                __LPtoDP(hdc,(POINT*)&rc,2);

                gk_rc.left = rc.left;
                gk_rc.top = rc.top;
                gk_rc.right = rc.right;
                gk_rc.bottom = rc.bottom;

                switch(mode)
                {
                    case CN_FILLRECT_MODE_LR:
                            mode =CN_FILLRECT_MODE_LR;
                            break;
                    case CN_FILLRECT_MODE_UD:
                            mode =CN_FILLRECT_MODE_UD;
                            break;
                    case CN_FILLRECT_MODE_LU2RD:
                            mode =CN_FILLRECT_MODE_LU2RD;
                            break;
                    case CN_FILLRECT_MODE_RU2LD:
                            mode =CN_FILLRECT_MODE_RU2LD;
                            break;
                    default:
                            mode =CN_FILLRECT_MODE_N;
                            break;

                }
                GK_FillRect(hdc->pGkWin,&gk_rc,Color1,Color2,
                            mode,hdc->SyncTime);
                EndDraw(hdc);
            }
        }
    }

}

//----�������Ч���ľ���------------------------------------------------------------------
//����: ʹ��Color1��Ϊ��ʼɫ(��,�ϱ߽�),Color2��Ϊ����ɫ(��,�±߽�),���һ������Ч���ľ���.
//������hdc: ��ͼ�����ľ��.
//      prc: ���β���.
//      Color1: ��ʼ��ɫ.
//      Color2: ������ɫ.
//���أ���.
//------------------------------------------------------------------------------
void    Fill3DRect(HDC hdc,const RECT *prc,u32 Color1,u32 Color2)
{
    u32 c;
    RECT rc;

    if(hdc!=NULL)
    if(prc!=NULL)
    {
        CopyRect(&rc,prc);
        c=SetDrawColor(hdc,Color1);
        DrawLine(hdc,0,0,0,RectH(&rc)-1); //L
        DrawLine(hdc,0,0,RectW(&rc)-1,0); //U

        SetDrawColor(hdc,Color2);
        DrawLine(hdc,RectW(&rc)-1,0,RectW(&rc)-1,RectH(&rc)-1); //R
        DrawLine(hdc,0,RectH(&rc)-1,RectW(&rc)-1,RectH(&rc)-1); //D
        SetDrawColor(hdc,c);

        c=SetFillColor(hdc,AlphaBlendColor(Color1,Color2,128));

        InflateRect(&rc,-1,-1);
        FillRect(hdc,&rc);
        SetFillColor(hdc,c);
    }

}

//----��Բ------------------------------------------------------------------
//����: ʹ��DrawColor��һ������Բ.
//������hdc: ��ͼ�����ľ��.
//      cx,cy: Բ����������
//      r: Բ�İ뾶
//���أ���.
//------------------------------------------------------------------------------
void    DrawCircle(HDC hdc,s32 cx,s32 cy,s32 r)
{
     s32 x = 0;
     s32 y = r;
     s32 delta = 2*(1-r);
     s32 direction;
     u32 color;
     ////

     if(hdc!=NULL)
     {
         color =GetDrawColor(hdc);
         while (y >= 0)
         {

            SetPixel(hdc,cx+x, cy+y, color);
            SetPixel(hdc,cx-x, cy+y, color);
            SetPixel(hdc,cx-x, cy-y, color);
            SetPixel(hdc,cx+x, cy-y, color);
            ////

            if (delta < 0)
            {
                 if ((2*(delta+y)-1) < 0)
                 {
                  direction = 1;
                 }
                 else
                 {
                  direction = 2;
                 }
             }
             else if(delta > 0)
             {
                 if ((2*(delta-x)-1) <= 0)
                 {
                  direction = 2;
                 }
                 else
                 {
                  direction = 3;
                 }
             }
             else
             {
                   direction=2;
             }

             ////

             switch(direction)
             {

              case 1:
                     x++;
                      delta += (2*x+1);
                      break;

              case 2:
                      x++;
                      y--;
                      delta += 2*(x-y+1);
                       break;

              case 3:
                      y--;
                      delta += (-2*y+1);
                       break;

             }

         }
    }
}

//----���Բ------------------------------------------------------------------
//����: ʹ��FillColor���һ��ʵ��Բ.
//������hdc: ��ͼ�����ľ��.
//      cx,cy: Բ����������
//      r: Բ�İ뾶
//���أ���.
//------------------------------------------------------------------------------
void    FillCircle(HDC hdc,s32 cx,s32 cy,s32 r)
{
      s32 i;
      s32 imax = ((s32)((s32)r*707))/1000+1;
      s32 sqmax = (s32)r*(s32)r+(s32)r/2;
      s32 x=r;
      u32 color_bk;

      color_bk =SetDrawColor(hdc,GetFillColor(hdc));

      DrawLine(hdc,cx-r,cy,cx+r+1,cy);
      for (i=1; i<= imax; i++)
      {
        if ((i*i+x*x) >sqmax)
        {

          if (x>imax)
          {
            DrawLine(hdc,cx-i+1,cy+x, cx+i,cy+x);
               DrawLine(hdc,cx-i+1,cy-x, cx+i,cy-x);
          }
          x--;
        }

        DrawLine(hdc,cx-x,cy+i, cx+x+1,cy+i);
        DrawLine(hdc,cx-x,cy-i, cx+x+1,cy-i);
      }

      SetDrawColor(hdc,color_bk);
}

//----����Բ------------------------------------------------------------------
//����: ʹ��DrawColor��һ��������Բ.
//������hdc: ��ͼ�����ľ��.
//      cx,cy: ��Բ����������
//      rx: ��Բ��ˮƽ�뾶
//      ry: ��Բ�Ĵ�ֱ�뾶
//���أ���.
//------------------------------------------------------------------------------
void     DrawEllipse(HDC hdc,s32 cx, s32 cy, s32 rx, s32 ry)
{
    s32 OutConst, Sum, SumY;
    s32 x,y;
    s32 xOld;
    u32 _rx = rx;
    u32 _ry = ry;

    OutConst =   _rx*_rx*_ry*_ry
               +(_rx*_rx*_ry>>1);
    xOld = x = rx;

     for(y=0; y<=ry; y++)
     {
        if(y==ry)
        {
          x=0;
        }
        else
        {
          SumY =((s32)(rx*rx))*((s32)(y*y));
          while (Sum = SumY + ((s32)(ry*ry))*((s32)(x*x)),
                 (x>0) && (Sum>OutConst)) x--;
        }

        if(y)
        {
          DrawLine(hdc,cx-xOld,cy-y+1,cx-x,cy-y);
          DrawLine(hdc,cx-xOld,cy+y-1,cx-x,cy+y);
          DrawLine(hdc,cx+xOld,cy-y+1,cx+x,cy-y);
          DrawLine(hdc,cx+xOld,cy+y-1,cx+x,cy+y);
        }
        xOld = x;
     }

}

//----�����Բ------------------------------------------------------------------
//����: ʹ��FillColor���һ��ʵ����Բ.
//������hdc: ��ͼ�����ľ��.
//      cx,cy: ��Բ����������
//      rx: ��Բ��ˮƽ�뾶
//      ry: ��Բ�Ĵ�ֱ�뾶
//���أ���.
//------------------------------------------------------------------------------
void FillEllipse(HDC hdc,s32 cx, s32 cy, s32 rx, s32 ry)
{
    s32 OutConst, Sum, SumY;
    s32 x,y;
    u32 _rx = rx;
    u32 _ry = ry;
    u32 color_bk;

    color_bk =SetDrawColor(hdc,GetFillColor(hdc));

    OutConst = _rx*_rx*_ry*_ry
                +(_rx*_rx*_ry>>1);
     x = rx;
     for (y=0; y<=ry; y++)
     {
          SumY =((s32)(rx*rx))*((s32)(y*y));

          while (Sum = SumY + ((s32)(ry*ry))*((s32)(x*x)),
                 (x>0) && (Sum>OutConst))
          {
             x--;
          }
          DrawLine(hdc,cx-x, cy+y, cx+x,cy+y);

          if(y)
          {
             DrawLine(hdc,cx-x, cy-y, cx+x,cy-y);
          }
    }

    SetDrawColor(hdc,color_bk);

}

//----��������------------------------------------------------------------------
//����: ʹ��DrawColor��������.
//������hdc: ��ͼ�����ľ��.
//      xCenter,yCenter: ���ε���������
//      radius: ���ΰ뾶
//      angle1: ������ʼ��Ƕ�
//      angle2: ���ν�����Ƕ�
//���أ���.
//------------------------------------------------------------------------------
void    DrawSector(HDC hdc, s32 xCenter, s32 yCenter, s32 radius,s32 angle1,s32 angle2)
{
  s32 x, y, d,c1,c2,c,step=0;
  s32 quarter1,quarter2,quarter3,quarter4;
  u32 color;

  if(radius<=0)
  {
      return;
  }

  x = xCenter;
  y = yCenter + radius;
  d = 3 - 2 * radius;
  c1=(s32)(angle1*radius*3.14159/180);
  c2=(s32)(angle2*radius*3.14159/180);
  quarter1=(s32)(radius*3.14159/2);
  quarter2=(s32)(radius*3.14159);
  quarter3=(s32)(radius*3.14159*3/2);
  quarter4=(s32)(radius*3.14159*2);

  color =GetDrawColor(hdc);
  while(1)
  {
      c=quarter4-step;
    if(c>=c1 && c<=c2)    SetPixel(hdc,xCenter + (y - yCenter), yCenter + (x - xCenter ),color );

    if(step>=c1 && step<=c2) SetPixel(hdc,xCenter + (y - yCenter), yCenter - (x - xCenter),color );

    c=quarter2+step;
    if(c>=c1 && c<=c2) SetPixel(hdc, xCenter - (y - yCenter), yCenter + (x - xCenter ),color );

    c=quarter2-step;
    if(c>=c1 && c<=c2) SetPixel(hdc, xCenter - (y - yCenter), yCenter - (x - xCenter),color );

    if (  x - xCenter  >=  y - yCenter  ) break;

    c=quarter3+step;
    if(c>=c1 && c<=c2) SetPixel(hdc, x, y,color );

    c=quarter1-step;
    if(c>=c1 && c<=c2) SetPixel(hdc,x, yCenter - (y - yCenter),color );

    c=quarter3-step;
    if(c>=c1 && c<=c2) SetPixel(hdc, xCenter - (x - xCenter), y,color );

    c= quarter1+step;
    if(c>=c1 && c<=c2) SetPixel(hdc, xCenter - (x - xCenter), yCenter - (y - yCenter),color );

    if ( d < 0 )
    { d = d + ((x - xCenter) << 2) + 6;
    }
    else
    { d = d + (((x - xCenter) - (y - yCenter)) << 2 ) + 10;
      y--;
    }
    x++;
    step++;
  }

}

//----�������------------------------------------------------------------------
//����: ʹ��FillColor�������.
//������hdc: ��ͼ�����ľ��.
//      xCenter,yCenter: ���ε���������
//      radius: ���ΰ뾶
//      angle1: ������ʼ��Ƕ�
//      angle2: ���ν�����Ƕ�
//���أ���.
//------------------------------------------------------------------------------
void    FillSector(HDC hdc, s32 xCenter, s32 yCenter, s32 radius,s32 angle1,s32 angle2)
{
  s32 x, y, d,c1,c2,c,step=0;
  s32 quarter1,quarter2,quarter3,quarter4;
  u32 color;

  if(radius<=0)
  {
      return;
  }

  x = xCenter;
  y = yCenter + radius;
  d = 3 - 2 * radius;
  c1=(s32)(angle1*radius*3.14159/180);
  c2=(s32)(angle2*radius*3.14159/180);
  quarter1=(s32)(radius*3.14159/2);
  quarter2=(s32)(radius*3.14159);
  quarter3=(s32)(radius*3.14159*3/2);
  quarter4=(s32)(radius*3.14159*2);
  color =GetFillColor(hdc);

  while(1)
  {
      c=quarter4-step;
    if(c>=c1 && c<=c2)    DrawLineEx(hdc,xCenter + (y - yCenter), yCenter + (x - xCenter ) ,xCenter,yCenter ,color);

    if(step>=c1 && step<=c2) DrawLineEx(hdc,xCenter + (y - yCenter), yCenter - (x - xCenter),xCenter,yCenter ,color);

    c=quarter2+step;
    if(c>=c1 && c<=c2) DrawLineEx(hdc, xCenter - (y - yCenter), yCenter + (x - xCenter ),xCenter,yCenter ,color);

    c=quarter2-step;
    if(c>=c1 && c<=c2) DrawLineEx(hdc, xCenter - (y - yCenter), yCenter - (x - xCenter),xCenter,yCenter,color);

    if (  x - xCenter  >=  y - yCenter  ) break;

    c=quarter3+step;
    if(c>=c1 && c<=c2) DrawLineEx(hdc, x, y,xCenter,yCenter ,color);

    c=quarter1-step;
    if(c>=c1 && c<=c2) DrawLineEx(hdc,x, yCenter - (y - yCenter),xCenter,yCenter ,color);

    c=quarter3-step;
    if(c>=c1 && c<=c2) DrawLineEx(hdc, xCenter - (x - xCenter), y ,xCenter,yCenter,color);

    c= quarter1+step;
    if(c>=c1 && c<=c2) DrawLineEx(hdc, xCenter - (x - xCenter), yCenter - (y - yCenter) ,xCenter,yCenter,color);

    if ( d < 0 )
    { d = d + ((x - xCenter) << 2) + 6;
    }
    else
    { d = d + (((x - xCenter) - (y - yCenter)) << 2 ) + 10;
      y--;
    }
    x++;
    step++;
  }
}

//----����3��Bezier��----------------------------------------------------------
//����: ��ָ���������������ı�������
//������hdc: ��ͼ�����ľ��.
//      pt: �߶������
//      cnt: ���������
//���أ���.
//-----------------------------------------------------------------------------
void DrawBezier3(HDC hdc,const POINT *pt,s32 cnt)
{
    float t,t1,t2,xt,yt,step;
    s32 x0,y0;
    ////

    if(cnt <= 0)
    {
        return;
    }

    if(pt == NULL)
    {
        return;
    }

    x0     =pt[0].x;
    y0     =pt[0].y;
    step   =1.0/cnt;

    for (t=0;t<=1;t+=step)
    {
        yt =1-t;
        t1 =yt*yt;
        t2 =3*yt*t;

        xt =pt[0].x*t1*yt+pt[1].x*t2*yt+pt[2].x*t2*t+pt[3].x*t*t*t;
        yt =pt[0].y*yt*t1+pt[1].y*t2*yt+pt[2].y*t2*t+pt[3].y*t*t*t;

        DrawLine(hdc,x0,y0,xt,yt);

        x0 =xt;
        y0 =yt;

    }

}


//----��������------------------------------------------------------------------
//����: ʹ��DrawColor��������.
//������hdc: ��ͼ�����ľ��.
//      pt: ���������.
//      count: ���������.
//���أ���.
//------------------------------------------------------------------------------
void    DrawPolyLine(HDC hdc,const POINT *pt,s32 count)
{
    s32 i;

    if(count>1)
    {
        for(i=1;i<count;i++)
        {
            DrawLine(hdc,pt[i-1].x,pt[i-1].y,pt[i].x,pt[i].y);
        }
    }
}

//----������Ͽ�------------------------------------------------------------------
//����: ʹ��DrawColor������Ͽ�߿�,ʹ��TextColor������Ͽ�����.
//������hdc: ��ͼ�����ľ��.
//      prc: ��Ͽ���β���.
//      Text:��Ͽ�����.
//���أ���.
//------------------------------------------------------------------------------

void    DrawGroupBox(HDC hdc,const RECT *prc,const char *Text)
{
    s32 i,text_w,text_h,text_offset;
    u32 old_color;

    POINT pt[6];
    RECT rc;

    rc.left =0;
    rc.top =0;
    rc.right =0;
    rc.bottom =0;
    AdjustTextRect(hdc,Text,-1,&rc,DT_CENTER|DT_VCENTER);

    text_w =RectW(&rc);
    text_h =RectH(&rc);
    text_offset =8;

    pt[0].x =prc->left+text_offset;
    pt[0].y =prc->top+(RectH(&rc)>>1);

    pt[1].x =pt[0].x-text_offset;
    pt[1].y =pt[0].y;

    pt[2].x =pt[1].x;
    pt[2].y =prc->top+RectH(prc)-1;

    pt[3].x =pt[2].x+RectW(prc)-1;
    pt[3].y =pt[2].y;

    pt[4].x =pt[3].x;
    pt[4].y =pt[0].y;

    pt[5].x =pt[0].x+text_w;
    pt[5].y =pt[0].y;

    ////Bottom
    for(i=0;i<6;i++)
    {
        pt[i].x+=1;
        pt[i].y+=1;
    }

    old_color=GetDrawColor(hdc);
    SetDrawColor(hdc,AlphaBlendColor(RGB(255,255,255),old_color,128));
    DrawPolyLine(hdc,pt,6);

    ////Top
    for(i=0;i<6;i++)
    {
        pt[i].x-=1;
        pt[i].y-=1;
    }
    SetDrawColor(hdc,old_color);
    DrawPolyLine(hdc,pt,6);

    SetRect(&rc,pt[0].x+1,prc->top,text_w,text_h);
    DrawText(hdc,Text,-1,&rc,DT_CENTER|DT_VCENTER);


}
//----����λͼ------------------------------------------------------------------
//����:
//������hdc: ��ͼ�����ľ��.
//���أ���.
//------------------------------------------------------------------------------
bool_t    DrawBitmap(HDC hdc,s32 x,s32 y,struct RectBitmap *bitmap,u32 HyalineColor,struct RopGroup RopCode)
{
    POINT pt;
    if(BeginDraw(hdc))
    {
        pt.x =x;
        pt.y =y;
        __LPtoDP(hdc,&pt,1);

        GK_DrawBitMap(hdc->pGkWin,bitmap,pt.x,pt.y,HyalineColor,RopCode,hdc->SyncTime);
        EndDraw(hdc);
        return true;
    }
    return false;
}

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/


