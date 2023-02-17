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
//文件描述: 绘图管理层
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2014-11-10
//   作者:  LiuWei.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include    "gdd_private.h"
#include <loc_string.h>
/*============================================================================*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static  bool_t    __GDD_BeginDraw(HDC hdc)
{
    return TRUE;
}

static  void    __GDD_EndDraw(HDC hdc)
{

}
#pragma GCC diagnostic pop

//----DC坐标转为窗口坐标---------------------------------------------------------
//功能：把以DC类型对象的左上角为原点的坐标，转换为以窗口左上角为原点的坐标，平移量是
//      DC到窗口区域的偏移量，例如DC_TYPE_CLIENT，则平移客户区原点到窗口原点之间的差值。
//参数：hdc，被操作的DC
//      pt，待转换的坐标
//      count，待平移的坐标数量
//返回：无
//-----------------------------------------------------------------------------
static  void    __GDD_Cdn_DC_toWin(HDC hdc,POINT *pt,s32 count)
{
//  RECT rc;
    switch(hdc->DCType)
    {
        case    DC_TYPE_PAINT:
        case    DC_TYPE_CLIENT:
//          GK_GetScreenArea(hdc->pGkWin, &rc);   //窗口在显示器上的坐标
            while(count--)
            {
//              pt->x +=(hdc->hwnd->CliRect.left - rc.left);
//              pt->y +=(hdc->hwnd->CliRect.top - rc.top);
                pt->x +=(hdc->hwnd->CliRect.left);
                pt->y +=(hdc->hwnd->CliRect.top);
                pt++;
            }
            break;
            ////

        case    DC_TYPE_WINDOW:
            while(count--)
            {
                //原坐标系与目标坐标系相同，无须修改。
            }
            break;

        default:
            break;
            ////
    }
}
/*============================================================================*/

u32 GDD_AlphaBlendColor(u32 bk_c,u32 fr_c,u8 alpha)
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

//----更新显示------------------------------------------------------------------
//功能：更新显示，不仅仅更新hwnd窗口，而是更新所有窗口。因为hwnd窗口的改变，可能导致其他
//      窗口的改变。又，可能出现跨显示器的窗口，故更新所有显示器。
//参数：hwnd，窗口句柄，执行hwnd所在的主窗口（或桌面）的消息循环，最后执行刷新显示的
//      消息，刷新的是整个图形系统，不限于本窗口。
//返回：无
//注意：如果存在多个主窗口，不执行非hwnd所在的主窗口在排队中的消息。
//-----------------------------------------------------------------------------
void    GDD_SyncShow(HWND hwnd)
{
    GDD_PostMessage(hwnd, MSG_SYNC_DISPLAY, 0, 0);
}

/*============================================================================*/
/*============================================================================*/
void    GDD_InitDC(HDC pdc,struct GkWinObj *gk_win,HWND hwnd,s32 dc_type)
{
    struct PointCdn sz;
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
    pdc->SyncTime   = CN_TIMEOUT_FOREVER;
    pdc->RopCode=gk_win->RopCode;
    switch(pdc->DCType)
    {
        case    DC_TYPE_PAINT:
        case    DC_TYPE_CLIENT:
            pdc->DrawArea = hwnd->CliRect;
            break;
            ////

        case    DC_TYPE_WINDOW:
            GK_GetSize(pdc->pGkWin, &sz);
            pdc->DrawArea.left = 0;
            pdc->DrawArea.top = 0;
            pdc->DrawArea.right = sz.x;
            pdc->DrawArea.bottom = sz.y;
            break;
        case    DC_TYPE_CAPTION:
            GK_GetSize(pdc->pGkWin, &sz);
            pdc->DrawArea.left = DEF_BORDER_SIZE + DEF_DLGFRAME_SIZE;
            pdc->DrawArea.top =  DEF_BORDER_SIZE + DEF_DLGFRAME_SIZE;
            pdc->DrawArea.right = sz.x - DEF_BORDER_SIZE - DEF_DLGFRAME_SIZE;;
            pdc->DrawArea.bottom =  DEF_BORDER_SIZE + DEF_DLGFRAME_SIZE + DEF_CAPTION_SIZE;
            break;

        default:
            break;
            ////
    }

}

bool_t  GDD_DeleteDC(HDC hdc)
{
    if(hdc != NULL)
    {
//        GUI_DeleteDefFont(hdc->pFontDef);
        free(hdc);
        return TRUE;
    }
    return FALSE;
}

//----重设可绘制区---------------------------------------------------------------
//功能：重新设置DC的可绘制区
//参数：hdc: 绘图上下文句柄.
//     NewArea: 新的可绘制区
//返回：无
//------------------------------------------------------------------------------
void GDD_SetDrawArea(HDC hdc, struct Rectangle *DrawArea)
{
    hdc->DrawArea = *DrawArea;
}
//----设置光栅码-------------------------------------------------------------
//描述: 设置DC的光栅操作编码，并返回原来的。
//参数：hdc: 绘图上下文句柄.
//     RopCode: 新的光栅码.
//返回：旧的光栅码.
//------------------------------------------------------------------------------
struct RopGroup GDD_SetRopCode(HDC hdc,struct RopGroup RopCode)
{
    struct RopGroup old=(struct RopGroup){ 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };
    if(hdc!=NULL)
    {
        old =hdc->RopCode;
        hdc->RopCode =RopCode;
    }
    return old;
}

//----获得当前光栅码-------------------------------------------------------------
//描述: 略.
//参数：hdc: 绘图上下文句柄.
//返回：当前光栅码.
//------------------------------------------------------------------------------
struct RopGroup GDD_GetRopCode(HDC hdc)
{
    struct RopGroup old=(struct RopGroup){ 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };
    if(hdc!=NULL)
    {
        old =hdc->RopCode;
    }
    return old;

}

//----设置当前坐标位置-----------------------------------------------------------
//描述: 该函数为内部调用,不检查函数参数的合法性.
//参数：hdc: 绘图上下文句柄.
//      x,y: 新的坐标位置.
//     old_pt: 输出旧的坐标位置,如果该参数为NULL,刚不输出旧的坐标位置.
//返回：无.
//------------------------------------------------------------------------------
static  void __GDD_MoveTo(HDC hdc,s32 x,s32 y,POINT *old_pt)
{

    if(old_pt!=NULL)
    {
        old_pt->x =hdc->CurX;
        old_pt->y =hdc->CurY;
    }

    hdc->CurX =x;
    hdc->CurY =y;
}

//----设置当前坐标位置-----------------------------------------------------------
//描述: 当前坐标位置会影响LineTo画线的起始位置.
//参数：hdc: 绘图上下文句柄.
//      x,y: 新的坐标位置.
//     old_pt: 输出旧的坐标位置,如果该参数为NULL,刚不输出旧的坐标位置.
//返回：无.
//------------------------------------------------------------------------------
void GDD_MoveTo(HDC hdc,s32 x,s32 y,POINT *old_pt)
{
    if(hdc!=NULL)
    {
        __GDD_MoveTo(hdc,x,y,old_pt);
    }
}

//----设置当前绘制颜色-----------------------------------------------------------
//描述: 当前绘制颜色会影响所有绘制类函数,如DrawRect,GDD_DrawCircle...
//参数：hdc: 绘图上下文句柄.
//      color: 新的绘制颜色.
//返回：旧的绘制颜色.
//------------------------------------------------------------------------------
u32 GDD_SetDrawColor(HDC hdc,u32 color)
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

//----获得当前绘制颜色-----------------------------------------------------------
//描述: 略.
//参数：hdc: 绘图上下文句柄.
//返回：当前绘制颜色.
//------------------------------------------------------------------------------
u32 GDD_GetDrawColor(HDC hdc)
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

//----设置当前填充颜色-----------------------------------------------------------
//描述: 当前填充颜色会影响所有填充类函数,如FillRect,GDD_FillCircle...
//参数：hdc: 绘图上下文句柄.
//      color: 新的填充颜色.
//返回：旧的绘制颜色.
//------------------------------------------------------------------------------
u32 GDD_SetFillColor(HDC hdc,u32 color)
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

//----获得当前填充颜色-----------------------------------------------------------
//描述: 略.
//参数：hdc: 绘图上下文句柄.
//返回：当前填充颜色.
//------------------------------------------------------------------------------
u32 GDD_GetFillColor(HDC hdc)
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

//----设置当前文字颜色-----------------------------------------------------------
//描述: 当前文字颜色会影响所有文字绘制类函数,如TextOut,GDD_DrawText...
//参数：hdc: 绘图上下文句柄.
//      color: 新的文字颜色.
//返回：旧的文字颜色.
//------------------------------------------------------------------------------
u32 GDD_SetTextColor(HDC hdc,u32 color)
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

//----获得当前文字颜色-----------------------------------------------------------
//描述: 略.
//参数：hdc: 绘图上下文句柄.
//返回：当前文字颜色.
//------------------------------------------------------------------------------
u32 GDD_GetTextColor(HDC hdc)
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

//----设置当前同步时间-----------------------------------------------------------
//描述: 略
//参数：hdc: 绘图上下文句柄.
//      sync_time: 新的同步时间.
//返回：旧的同步时间.
//------------------------------------------------------------------------------
u32 GDD_SetSyncTime(HDC hdc,u32 sync_time)
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

//----获得当前同步时间-----------------------------------------------------------
//描述: 略
//参数：hdc: 绘图上下文句柄.
//返回：当前同步时间.
//------------------------------------------------------------------------------
u32 GDD_GetSyncTime(HDC hdc)
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

//----设置字体---------------------------------------------------------------
//描述: 设置句柄关联的字体，在该DC上显示字符串时，将使用该字体
//参数：hdc: 绘图上下文句柄.
//      hFont: 当前字体句柄.
//返回：旧的字体句柄.
//------------------------------------------------------------------------------
struct FontObj *GDD_SetFont(HDC hdc, struct FontObj *hFont)
{
    struct FontObj *old;

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

//----获得字体---------------------------------------------------------------
//描述: 取得 DC 关联的字体
//参数：hdc: 绘图上下文句柄.
//返回：当前字体句柄.
//------------------------------------------------------------------------------
struct FontObj *GDD_GetFont(HDC hdc)
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

//----设置字符集------------------------------------------------------------
//描述: 设置句柄关联的字符集，在该DC上显示字符串时，将使用该字符集
//参数：hdc: 绘图上下文句柄.
//      hFont: 当前字符集句柄.
//返回：旧的字体句柄.
//------------------------------------------------------------------------------
struct Charset *GDD_SetCharset(HDC hdc,struct Charset *pCharset)
{
    struct Charset * old;

    if(hdc!=NULL)
    {
        old = hdc->pCharset;
        hdc->pCharset = pCharset;
    }
    else
    {
        old =NULL;
    }
    return old;
}

//----获得字符集---------------------------------------------------------------
//描述: 取得 DC 关联的字符集
//参数：hdc: 绘图上下文句柄.
//返回：当前字符集句柄.
//------------------------------------------------------------------------------
struct Charset *GDD_GetCharset(HDC hdc)
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

//----设置指定位置像素颜色-------------------------------------------------------
//描述: 略
//参数：hdc: 绘图上下文句柄.
//      x,y: 坐标.
//      color:颜色值.
//返回：无.
//------------------------------------------------------------------------------
void GDD_SetPixel(HDC hdc,s32 x,s32 y,u32 color)
{
    POINT pt;
    if(__GDD_BeginDraw(hdc))
    {
        pt.x =x;
        pt.y =y;
        __GDD_Cdn_DC_toWin(hdc,&pt,1);
        GK_SetPixel(hdc->pGkWin,NULL,pt.x,pt.y,color,hdc->RopCode.Rop2Mode,hdc->SyncTime);
        __GDD_EndDraw(hdc);
    }
}

//----画线----------------------------------------------------------------------
//描述: 绘制单个像素宽的任意直线,使用DrawColor作为颜色值.
//参数：hdc: 绘图上下文句柄.
//      x0,y0: 起始坐标.
//      x1,y1: 结束坐标.
//返回：无.
//------------------------------------------------------------------------------
void    GDD_DrawLine(HDC hdc,s32 x0,s32 y0,s32 x1,s32 y1)
{
    POINT pt[2];

    if(__GDD_BeginDraw(hdc))
    {
        pt[0].x =x0;
        pt[0].y =y0;
        pt[1].x =x1;
        pt[1].y =y1;
        __GDD_Cdn_DC_toWin(hdc,pt,2);

        GK_Lineto(hdc->pGkWin, &hdc->DrawArea, pt[0].x, pt[0].y, pt[1].x, pt[1].y,
                hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
        __GDD_EndDraw(hdc);
    }
}

//---------------虚线绘制函数---------------------------
//描述：绘制单个像素宽的虚线
//参数：hdc：绘图上下文句柄
//      x0,y0: 起始坐标.
//      x1,y1: 结束坐标.
//      temp_draw:每段绘制的长度
//      temp_past:每段不绘制的长度
//返回：无.
//------------------------------------------------------------------------------
void    GDD_DrawDottedLine(HDC hdc,s32 x0,s32 y0,s32 x1,s32 y1,s32 temp_draw,s32 temp_past)
{
    POINT pt[2];
    u32 i,num;
    s32 dot;
    s32 temp=temp_draw+temp_past;

    if(__GDD_BeginDraw(hdc))
    {
        if(x0==x1)//垂直线
        {
            dot=abs(y1-y0);

            if(dot==0)
                return;
            if(dot/(temp)!=0)
            num = dot/(temp)+1;
            else
                num = dot/(temp);
            if(y0>y1)//判断方向
            {
                temp=~temp+1;
                temp_past=~temp_past+1;
                temp_draw=~temp_draw+1;
            }
            for(i=1;i<num;i++)
            {
                 pt[0].x =x0;
                 pt[0].y =y0+(temp)*(i-1);
                 pt[1].x =x1;
                 pt[1].y =y0+i*(temp)-temp_past;
                 __GDD_Cdn_DC_toWin(hdc,pt,2);
                GK_Lineto(hdc->pGkWin,&hdc->DrawArea,pt[0].x,pt[0].y,pt[1].x,pt[1].y,
                        hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
            }
            //绘制最后一段
            pt[0].x =x0;
            pt[0].y =y0+(temp)*(num-1);
            pt[1].x =x1;
            pt[1].y =((y0+num*(temp)-temp_past)>dot)?y1:(y0+num*(temp)-temp_past);
             __GDD_Cdn_DC_toWin(hdc,pt,2);
            GK_Lineto(hdc->pGkWin,&hdc->DrawArea,pt[0].x,pt[0].y,pt[1].x,pt[1].y,
                    hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
        }

        else if(y0==y1)//水平线
        {
          dot=abs(x1-x0);
          if(dot==0)
              return;
          if(dot/(temp)!=0)
              num = dot/(temp)+1;
          else
              num = dot/(temp);
          if(x0>x1)//判断方向
          {
              temp=~temp+1;
              temp_past=~temp_past+1;
              temp_draw=~temp_draw+1;
          }

          for(i=1;i<=num;i++)
          {
             pt[0].x =x0+(temp)*(i-1);
             pt[0].y =y0;
             pt[1].x =x0+i*(temp)-temp_past;
             pt[1].y =y1;
             __GDD_Cdn_DC_toWin(hdc,pt,2);
            GK_Lineto(hdc->pGkWin,&hdc->DrawArea,pt[0].x,pt[0].y,pt[1].x,pt[1].y,
                    hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
          }
          //绘制最后一段
          pt[0].x =x0+(temp)*(num-1);
          pt[0].y =y0;
          pt[1].x =((x0+num*(temp)-temp_past)>dot)?x1:(x0+num*(temp)-temp_past);
          pt[1].y =y1;
          __GDD_Cdn_DC_toWin(hdc,pt,2);
         GK_Lineto(hdc->pGkWin,&hdc->DrawArea,pt[0].x,pt[0].y,pt[1].x,pt[1].y,
                 hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
        }

        else//绘制斜线
        {
            s32 dy,dx,past_x,past_y;
            s32 flag_x=1;
            dot = sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));//斜线长度
            if(dot==0)
                return;
            if(dot/(temp)!=0)
                num = dot/(temp)+1;
            else
                num = dot/(temp);

            dy =(y1-y0)*temp/dot;
            dx = (x1-x0)*temp/dot;
            past_x = (x1-x0)*temp_past/dot;
            past_y = (y1-y0)*temp_past/dot;

            for(i=1;i<num;i++)
            {
                pt[0].x =x0+dx*(i-1);
                pt[0].y =y0+dy*(i-1);
                pt[1].x =x0+dx*i-past_x;
                pt[1].y =y0+dy*i-past_y;
                __GDD_Cdn_DC_toWin(hdc,pt,2);
               GK_Lineto(hdc->pGkWin,&hdc->DrawArea,pt[0].x,pt[0].y,pt[1].x,pt[1].y,
                       hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
            }
            pt[0].x =x0+dx*(num-1);
            pt[0].y =y0+dy*(num-1);
            if(x0<x1)
                pt[1].x =(x0+dx*num-past_x)>dot?x1:x0+dx*num-past_x;
            else if(x0>x1)
                pt[1].x =(x0+dx*num-past_x)<dot?x1:x0+dx*num-past_x;

            if(y0<y1)
                pt[1].y =(y0+dy*num-past_y)>dot?y1:y0+dy*num-past_y;
            if(y0>y1)
                pt[1].y =(y0+dy*num-past_y)<dot?y1:y0+dy*num-past_y;
            __GDD_Cdn_DC_toWin(hdc,pt,2);
           GK_Lineto(hdc->pGkWin,&hdc->DrawArea,pt[0].x,pt[0].y,pt[1].x,pt[1].y,
                   hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
        }
        __GDD_EndDraw(hdc);
    }
}
//----画线----------------------------------------------------------------------
//描述: 使用指定颜色绘制单个像素宽的任意直线.
//参数：hdc: 绘图上下文句柄.
//      x0,y0: 起始坐标.
//      x1,y1: 结束坐标.
//返回：无.
//------------------------------------------------------------------------------
void    GDD_DrawLineEx(HDC hdc,s32 x0,s32 y0,s32 x1,s32 y1,u32 color)
{
    POINT pt[2];

    if(__GDD_BeginDraw(hdc))
    {
        pt[0].x =x0;
        pt[0].y =y0;
        pt[1].x =x1;
        pt[1].y =y1;
        __GDD_Cdn_DC_toWin(hdc,pt,2);

        GK_Lineto(hdc->pGkWin,&hdc->DrawArea,pt[0].x,pt[0].y,pt[1].x,pt[1].y,
                color,hdc->RopCode.Rop2Mode,hdc->SyncTime);
        __GDD_EndDraw(hdc);
    }
}

//----使用当前位置画线-----------------------------------------------------------
//描述: 绘制单个像素宽的任意直线,使用DrawColor作为颜色值,使用当前位置作为起始坐标,
//     绘制完成后,并将结束坐标更新为当前坐标位置.
//参数：hdc: 绘图上下文句柄.
//      x,y: 结束坐标.
//返回：无.
//------------------------------------------------------------------------------
void    GDD_DrawLineTo(HDC hdc,s32 x,s32 y)
{
    POINT pt[2];

    if(__GDD_BeginDraw(hdc))
    {
        pt[0].x =hdc->CurX;
        pt[0].y =hdc->CurY;
        pt[1].x =x;
        pt[1].y =y;

        __GDD_Cdn_DC_toWin(hdc,pt,2);

        GK_Lineto(hdc->pGkWin,&hdc->DrawArea,pt[0].x,pt[0].y,pt[1].x,pt[1].y,
                hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);

        __GDD_MoveTo(hdc,x,y,NULL);
        __GDD_EndDraw(hdc);
    }
}

//----在指定位置绘制字符串-------------------------------------------------------
//描述: 在指定位置绘制字符串,使用TextColor作为颜色值,支持回车与换行符.
//参数：hdc: 绘图上下文句柄.
//      x,y: 坐标位置.
//      text: 字符串.
//      count: 要绘制的字符数量,该参数小于0时,将绘制整个字符串.
//返回：TRUE:成功; FALSE:失败.
//------------------------------------------------------------------------------
bool_t    GDD_TextOut(HDC hdc,s32 x,s32 y,const char *text,s32 count)
{
        POINT pt;

        if(__GDD_BeginDraw(hdc))
        {
            pt.x =x;
            pt.y =y;
            __GDD_Cdn_DC_toWin(hdc,&pt,1);

            x =pt.x;
            y =pt.y;

//            if(count<0)
//            {
//                count = strlen(text);
//            }

            if(text!=NULL)
            {
                GK_DrawText(hdc->pGkWin, &hdc->DrawArea,hdc->pFont, hdc->pCharset,
                                      x,y,text,count,hdc->TextColor,
                                      hdc->RopCode.Rop2Mode,hdc->SyncTime);

            }

            __GDD_EndDraw(hdc);
            return TRUE;
        }

        return FALSE;
}

//----计算字符串所占像素宽度-----------------------------------------------------
//描述: 该函数为内部调用,不检查函数参数合法性.
//参数：ft: 使用的字体句柄
//     text: 字符串.
//     count: 要计算的字符数量,该参数小于0时,将计算整个字符串所占像素宽度.
//返回：字符串像素宽度.
//------------------------------------------------------------------------------
static  s32 __GDD_GetTextWidth(HDC hdc,const char *text,s32 count)
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
                { // 无效字符
                    text++;
                }
                else if(len == 0)
                {
                    break;
                }
                else
                { // 有效字符
                    text += len;
                    width += Font->GetCharWidth(wc);
                }
            }
        }
    }
    return width;
}

//----计算字符串所占矩形大小-----------------------------------------------------
//描述: 该函数为内部调用,不检查函数参数合法性.
//参数：ft: 使用的字体句柄
//     text: 字符串.
//     count: 要计算的字符数量,该参数小于0时,将计算整个字符串所占像素宽度.
//     rect: 调整后的矩形.
//     flag: 字符绘制标记.
//返回：无.
//------------------------------------------------------------------------------
void GDD_AdjustTextRect(HDC hdc,const char *text,s32 count, RECT *prc,u32 flag)
{
    s32 Height;
    s32 i;

    Height = Font_GetFontLineHeight(hdc->pFont);
    if(1)
    {
        switch(flag&DT_ALIGN_V_MASK)
        {
            case    DT_VCENTER:
                    prc->top += ((GDD_RectH(prc) - Height)>>1);
                    prc->bottom = prc->top + Height;
                    break;
                    ////
            case    DT_TOP:
                    break;
                    ////
            case    DT_BOTTOM:
                    prc->top += (GDD_RectH(prc) - Height);
                    prc->bottom =prc->top + Height;
                    break;
                    ////
            default:
                    break;

        }

        switch(flag&DT_ALIGN_H_MASK)
        {
            case    DT_CENTER:
                    i =__GDD_GetTextWidth(hdc,text,count);
                    i =(GDD_RectW(prc)-i)>>1;
                    prc->left  += i;
                    prc->right -= i;
                    break;
                    ////
            case    DT_LEFT:
                    prc->left  += 1;
                    break;
                    ////
            case    DT_RIGHT:
                    i =__GDD_GetTextWidth(hdc,text,count);
                    prc->left = prc->right-i-1;
                    break;
                    ////
            default:
                    break;

        }


    }
}

//----计算一个字符串有多少行-----------------------------------------------------
//描述: 略
//参数：str: 字符串指针
//返回：字符串行数.
//------------------------------------------------------------------------------
s32  GDD_GetStrLineCount(struct Charset *myCharset, const char *str)
{
    s32 count,linenum = 0;
    const char *line=str;
    char *linenext;
    count=0;
    while(1)
    {
        linenext = mbstrchr_l(line, "\n", &count, myCharset);
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

//----在矩形范围内绘制字符串---------------------------------------------------
//描述: 在指定矩形范围内绘制字符串,使用TextColor作为颜色值,支持回车与换行符,
//      该函数可以指定是否绘制字符串边框和背景,以前指定对齐方式的组合.
//参数：hdc: 绘图上下文句柄.
//      x,y: 坐标位置.
//      count: 要绘制的字符数量,该参数小于0时,将绘制整个字符串.
//      prc: 要绘制的所在矩形位置和大小.
//      flag: 绘制标志参数，DT_VCENTER 族常量
//返回：TRUE:成功; FALSE:失败.
//-----------------------------------------------------------------------------
bool_t    GDD_DrawText(HDC hdc,const char *text,s32 count,const RECT *prc,u32 flag)
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

    line_count = GDD_GetStrLineCount(hdc->pCharset, text);

    if(__GDD_BeginDraw(hdc))
    {
        if(line_count<=1)
        {
            //单行
//          GDD_CopyRect(&rc0,prc);
            rc0 = *prc;
//          GDD_CopyRect(&rc,prc);
            rc = *prc;
            GDD_InflateRect(&rc,-1,-1);
            GDD_AdjustTextRect(hdc,text,count,&rc,flag);

            if(flag&DT_BORDER)
            {
                GDD_DrawRect(hdc,&rc0);
                GDD_InflateRect(&rc0,-1,-1);
            }

            if(flag&DT_BKGND)
            {
                GDD_FillRect(hdc,&rc0);
            }

           GDD_TextOut(hdc,rc.left,rc.top,text,count);

        }
        else
        {
            //多行
            s32 x0,y0,line_h;
            char *p0,*p1;

            line_h = Font_GetFontLineHeight(hdc->pFont)+5;
//            printf("----line_h = %d----\r\n",line_h);
//          GDD_CopyRect(&rc0,prc);
//          GDD_CopyRect(&rc,prc);
            rc0 = *prc;
            rc = *prc;
            GDD_InflateRect(&rc,-1,-1);

            if(flag&DT_BORDER)
            {
                GDD_DrawRect(hdc,&rc0);
                GDD_InflateRect(&rc0,-1,-1);
            }

            if(flag&DT_BKGND)
            {
                GDD_FillRect(hdc,&rc0);
            }

            y0 =rc.top;
            switch(flag&DT_ALIGN_V_MASK)
            {
                case    DT_VCENTER://(line_count-1)*line_h)为行间距
                    y0 += ((GDD_RectH(&rc)-(line_count*line_h)-((line_count-1)*line_h))/2);
                        break;
                        ////
                case    DT_TOP:
                        break;
                        ////
                case    DT_BOTTOM://(line_count-1)*line_h)为行间距
                        y0 += (GDD_RectH(&rc) - (line_count*line_h)-((line_count-1)*line_h));
                        break;
                        ////
                default:
                        break;

            }

            p0 =(char*)text;
            while(p0!=NULL)
            {
                p1 = mbstrchr_l(p0, "\n", &charnum, hdc->pCharset);

                if(p1 == p0)    //遇到连续换行符
                {
                    p0++;       //换行符肯定是1个字节的
                }
                else
                {
                    x0 =rc.left;
                    switch(flag&DT_ALIGN_H_MASK)
                    {
                        case    DT_CENTER:
                                x0 =(GDD_RectW(&rc)-__GDD_GetTextWidth(hdc,p0,charnum))>>1;
                                break;
                                ////
                        case    DT_LEFT:
                                x0  += 1;
                                break;
                                ////
                        case    DT_RIGHT:
                                x0 =(GDD_RectW(&rc)-__GDD_GetTextWidth(hdc,p0,charnum));
                                break;
                                ////
                        default:
                                break;

                    }

                    GDD_TextOut(hdc,x0,y0,p0,charnum);

                    if(p1==NULL)
                    {   //最后一行显示完后,则退出
                        break;
                    }

                    p0 = p1++;      //跳过换行符，肯定是1个字节
                    y0 += line_h;
                }

//                printf("----y0 = %d----\r\n",y0);
            }
        }

        __GDD_EndDraw(hdc);
        return TRUE;
    }

    return FALSE;

}

//----绘制矩形------------------------------------------------------------------
//描述: 使用DrawColor绘制一个空心矩形.
//参数：hdc: 绘图上下文句柄.
//      prc: 矩形参数.
//返回：无.
//------------------------------------------------------------------------------
void    GDD_DrawRect(HDC hdc,const RECT *prc)
{
    s32 x0,y0;
    s32 x1,y1;
    RECT rc;

    if(prc != NULL)
    {
//      GDD_CopyRect(&rc,prc);
        rc = *prc;
        if(__GDD_BeginDraw(hdc))
        {
            __GDD_Cdn_DC_toWin(hdc,(POINT*)&rc,2);

            x0 =rc.left;
            y0 =rc.top;
            x1 =rc.right;
            y1 =rc.bottom;

            //Left
            GK_Lineto(hdc->pGkWin,&hdc->DrawArea,x0,y0,x0,y1,hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
            //bottom
            GK_Lineto(hdc->pGkWin,&hdc->DrawArea,x0,y1,x1,y1,hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
            //Right
            GK_Lineto(hdc->pGkWin,&hdc->DrawArea,x1,y1,x1,y0,hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
            //top
            GK_Lineto(hdc->pGkWin,&hdc->DrawArea,x1,y0,x0,y0,hdc->DrawColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);

            __GDD_EndDraw(hdc);
        }
    }
}

//----填充矩形------------------------------------------------------------------
//描述: 使用FillColor填充一个实心矩形.
//参数：hdc: 绘图上下文句柄.
//      prc: 矩形参数.
//返回：无.
//------------------------------------------------------------------------------
void    GDD_FillRect(HDC hdc,const RECT *prc)
{
    RECT rc;

    if(hdc!=NULL)
    {
        if(prc!=NULL)
        {
//          GDD_CopyRect(&rc,prc);
            rc = *prc;
            if(__GDD_BeginDraw(hdc))
            {
                __GDD_Cdn_DC_toWin(hdc,(POINT*)&rc,2);

                GK_FillRect(hdc->pGkWin,&rc,hdc->FillColor,hdc->FillColor,
                            CN_FILLRECT_MODE_N,hdc->SyncTime);
                __GDD_EndDraw(hdc);
            }
        }
    }

}
//----填充矩形------------------------------------------------------------------
//描述: 使用指定颜色填充一个实心矩形.
//参数：hdc: 绘图上下文句柄.
//      prc: 矩形参数.
//返回：无.
//------------------------------------------------------------------------------
void    GDD_FillRectEx(HDC hdc,const RECT *prc,u32 color)
{
    RECT rc;

    if(hdc!=NULL)
    {
        if(prc!=NULL)
        {
//          GDD_CopyRect(&rc,prc);
            rc = *prc;
            if(__GDD_BeginDraw(hdc))
            {
                __GDD_Cdn_DC_toWin(hdc,(POINT*)&rc,2);

                GK_FillRect(hdc->pGkWin,&rc,color,color,
                            CN_FILLRECT_MODE_N,hdc->SyncTime);
                __GDD_EndDraw(hdc);
            }
        }
    }

}

//----渐变填充矩形------------------------------------------------------------------
//描述: 使用Color1作为起始色,Color2作为结束色,渐变填充矩形.
//参数：hdc: 绘图上下文句柄.
//      prc: 矩形参数.
//      Color1: 超始颜色.
//      Color2: 结束颜色.
//      mode: 渐变递增模式,可以是以下值:
//            CN_FILLRECT_MODE_LR:丛左到右填充
//            CN_FILLRECT_MODE_UD:丛上到下
//            CN_FILLRECT_MODE_LU2RD:丛左上到右下
//            CN_FILLRECT_MODE_RU2LD:丛右上到左下
//返回：无.
//------------------------------------------------------------------------------
void    GDD_GradientFillRect(HDC hdc,const RECT *prc,u32 Color1,u32 Color2,u32 mode)
{
    struct Rectangle gk_rc;
    RECT rc;

    if(hdc!=NULL)
    {
        if(prc!=NULL)
        {
            if(__GDD_BeginDraw(hdc))
            {
//              GDD_CopyRect(&rc,prc);
                rc = *prc;

                __GDD_Cdn_DC_toWin(hdc,(POINT*)&rc,2);

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
                __GDD_EndDraw(hdc);
            }
        }
    }

}

//----填充立体效果的矩形------------------------------------------------------------------
//描述: 使用Color1作为起始色(左,上边界),Color2作为结束色(右,下边界),填充一个立体效果的矩形.
//参数：hdc: 绘图上下文句柄.
//      prc: 矩形参数.
//      Color1: 超始颜色.
//      Color2: 结束颜色.
//返回：无.
//------------------------------------------------------------------------------
void    GDD_Fill3DRect(HDC hdc,const RECT *prc,u32 Color1,u32 Color2)
{
    u32 c;
    RECT rc;

    if(hdc!=NULL)
    {
        if(prc!=NULL)
        {
            if(__GDD_BeginDraw(hdc))
            {
//              GDD_CopyRect(&rc,prc);
                rc = *prc;
                c=GDD_SetDrawColor(hdc,Color1);
                GDD_DrawLine(hdc,0,0,0,GDD_RectH(&rc)-1); //L
                GDD_DrawLine(hdc,0,0,GDD_RectW(&rc)-1,0); //U

                GDD_SetDrawColor(hdc,Color2);
                GDD_DrawLine(hdc,GDD_RectW(&rc)-1,0,GDD_RectW(&rc)-1,GDD_RectH(&rc)-1); //R
                GDD_DrawLine(hdc,0,GDD_RectH(&rc)-1,GDD_RectW(&rc)-1,GDD_RectH(&rc)-1); //D
                GDD_SetDrawColor(hdc,c);

                c=GDD_SetFillColor(hdc,GDD_AlphaBlendColor(Color1,Color2,128));

                GDD_InflateRect(&rc,-1,-1);
                GDD_FillRect(hdc,&rc);
                GDD_SetFillColor(hdc,c);
                __GDD_EndDraw(hdc);
            }
        }
    }

}

//----画圆------------------------------------------------------------------
//描述: 使用DrawColor画一个空心圆.
//参数：hdc: 绘图上下文句柄.
//      cx,cy: 圆的中心坐标
//      r: 圆的半径
//返回：无.
//------------------------------------------------------------------------------
void    GDD_DrawCircle(HDC hdc,s32 cx,s32 cy,s32 r)
{

     ////
     POINT pt;
     if(hdc!=NULL)
     {
         if(__GDD_BeginDraw(hdc))
         {
             pt.x = cx;
             pt.y = cy;
             __GDD_Cdn_DC_toWin(hdc, &pt, 1);
             GK_DrawCircle(hdc->pGkWin, cx, cy, r, hdc->DrawColor, hdc->RopCode.Rop2Mode, hdc->SyncTime);
             __GDD_EndDraw(hdc);
         }

     }
}

//----填充圆------------------------------------------------------------------
//描述: 使用FillColor填充一个实心圆.
//参数：hdc: 绘图上下文句柄.
//      cx,cy: 圆的中心坐标
//      r: 圆的半径
//返回：无.
//------------------------------------------------------------------------------
void    GDD_FillCircle(HDC hdc,s32 cx,s32 cy,s32 r)
{
    s32 i;
    s32 imax = ((s32)((s32)r*707))/1000+1;
    s32 sqmax = (s32)r*(s32)r+(s32)r/2;
    s32 x=r;

    if(hdc!=NULL)
    {
        cx +=(hdc->hwnd->CliRect.left);
        cy +=(hdc->hwnd->CliRect.top);
        if(__GDD_BeginDraw(hdc))
        {

            GK_Lineto(hdc->pGkWin, &hdc->DrawArea, cx-r,cy,cx+r+1,cy,
                    hdc->FillColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
            for (i=1; i<= imax; i++)
            {
                if ((i*i+x*x) >sqmax)
                {

                    if (x>imax)
                    {
                        GK_Lineto(hdc->pGkWin, &hdc->DrawArea, cx-i+1,cy+x, cx+i,cy+x,
                                hdc->FillColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
                        GK_Lineto(hdc->pGkWin, &hdc->DrawArea, cx-i+1,cy-x, cx+i,cy-x,
                                hdc->FillColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
                    }
                    x--;
                }

                GK_Lineto(hdc->pGkWin, &hdc->DrawArea,cx-x,cy+i, cx+x+1,cy+i,
                        hdc->FillColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
                GK_Lineto(hdc->pGkWin, &hdc->DrawArea,cx-x,cy-i, cx+x+1,cy-i,
                        hdc->FillColor,hdc->RopCode.Rop2Mode,hdc->SyncTime);
            }

            __GDD_EndDraw(hdc);
        }
    }
}

//----画椭圆------------------------------------------------------------------
//描述: 使用DrawColor画一个空心椭圆.
//参数：hdc: 绘图上下文句柄.
//      cx,cy: 椭圆的中心坐标
//      rx: 椭圆的水平半径
//      ry: 椭圆的垂直半径
//返回：无.
//------------------------------------------------------------------------------
void     GDD_DrawEllipse(HDC hdc,s32 cx, s32 cy, s32 rx, s32 ry)
{
    s32 OutConst, Sum, SumY;
    s32 x,y;
    s32 xOld;
    u32 _rx = rx;
    u32 _ry = ry;

    OutConst =   _rx*_rx*_ry*_ry
               +(_rx*_rx*_ry>>1);
    xOld = x = rx;

    if(__GDD_BeginDraw(hdc))
    {
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
                GDD_DrawLine(hdc,cx-xOld,cy-y+1,cx-x,cy-y);
                GDD_DrawLine(hdc,cx-xOld,cy+y-1,cx-x,cy+y);
                GDD_DrawLine(hdc,cx+xOld,cy-y+1,cx+x,cy-y);
                GDD_DrawLine(hdc,cx+xOld,cy+y-1,cx+x,cy+y);
            }
            xOld = x;
        }
        __GDD_EndDraw(hdc);
    }

}

//----填充椭圆------------------------------------------------------------------
//描述: 使用FillColor填充一个实心椭圆.
//参数：hdc: 绘图上下文句柄.
//      cx,cy: 椭圆的中心坐标
//      rx: 椭圆的水平半径
//      ry: 椭圆的垂直半径
//返回：无.
//------------------------------------------------------------------------------
void GDD_FillEllipse(HDC hdc,s32 cx, s32 cy, s32 rx, s32 ry)
{
    s32 OutConst, Sum, SumY;
    s32 x,y;
    u32 _rx = rx;
    u32 _ry = ry;
    u32 color;

    OutConst = _rx*_rx*_ry*_ry
                +(_rx*_rx*_ry>>1);
    x = rx;
    if(__GDD_BeginDraw(hdc))
    {
        color =GDD_GetFillColor(hdc);
        for (y=0; y<=ry; y++)
        {
             SumY =((s32)(rx*rx))*((s32)(y*y));

             while (Sum = SumY + ((s32)(ry*ry))*((s32)(x*x)),
                    (x>0) && (Sum>OutConst))
             {
                x--;
             }
             GDD_DrawLineEx(hdc,cx-x, cy+y, cx+x,cy+y,color);

             if(y)
             {
                GDD_DrawLineEx(hdc,cx-x, cy-y, cx+x,cy-y,color);
             }
        }
        __GDD_EndDraw(hdc);
    }

}

//----绘制扇形------------------------------------------------------------------
//描述: 使用DrawColor绘制扇形.
//参数：hdc: 绘图上下文句柄.
//      xCenter,yCenter: 扇形的中心坐标
//      radius: 扇形半径
//      angle1: 扇形起始点角度
//      angle2: 扇形结束点角度
//返回：无.
//------------------------------------------------------------------------------
void    GDD_DrawSector(HDC hdc, s32 xCenter, s32 yCenter, s32 radius,s32 angle1,s32 angle2)
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

    if(__GDD_BeginDraw(hdc))
    {
        color =GDD_GetDrawColor(hdc);
        while(1)
        {
              c=quarter4-step;
            if(c>=c1 && c<=c2)
                GDD_SetPixel(hdc,xCenter + (y - yCenter), yCenter + (x - xCenter ),color );

            if(step>=c1 && step<=c2)
                 GDD_SetPixel(hdc,xCenter + (y - yCenter), yCenter - (x - xCenter),color );

            c=quarter2+step;
            if(c>=c1 && c<=c2)
                GDD_SetPixel(hdc, xCenter - (y - yCenter), yCenter + (x - xCenter ),color );

            c=quarter2-step;
            if(c>=c1 && c<=c2)
                GDD_SetPixel(hdc, xCenter - (y - yCenter), yCenter - (x - xCenter),color );

            if (  x - xCenter  >=  y - yCenter  ) break;

            c=quarter3+step;
            if(c>=c1 && c<=c2)
                 GDD_SetPixel(hdc, x, y,color );

            c=quarter1-step;
            if(c>=c1 && c<=c2)
                 GDD_SetPixel(hdc,x, yCenter - (y - yCenter),color );

            c=quarter3-step;
            if(c>=c1 && c<=c2)
                 GDD_SetPixel(hdc, xCenter - (x - xCenter), y,color );

            c= quarter1+step;
            if(c>=c1 && c<=c2)
                 GDD_SetPixel(hdc, xCenter - (x - xCenter), yCenter - (y - yCenter),color );

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
        __GDD_EndDraw(hdc);
    }

}

//----填充扇形------------------------------------------------------------------
//描述: 使用FillColor填充扇形.
//参数：hdc: 绘图上下文句柄.
//      xCenter,yCenter: 扇形的中心坐标
//      radius: 扇形半径
//      angle1: 扇形起始点角度
//      angle2: 扇形结束点角度
//返回：无.
//------------------------------------------------------------------------------
void    GDD_FillSector(HDC hdc, s32 xCenter, s32 yCenter, s32 radius,s32 angle1,s32 angle2)
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

    if(__GDD_BeginDraw(hdc))
    {
        color =GDD_GetFillColor(hdc);

        while(1)
        {
            c=quarter4-step;
            if(c>=c1 && c<=c2)
                GDD_DrawLineEx(hdc,xCenter + (y - yCenter), yCenter + (x - xCenter ) ,xCenter,yCenter ,color);

            if(step>=c1 && step<=c2)
                GDD_DrawLineEx(hdc,xCenter + (y - yCenter), yCenter - (x - xCenter),xCenter,yCenter ,color);

            c=quarter2+step;
            if(c>=c1 && c<=c2)
                GDD_DrawLineEx(hdc, xCenter - (y - yCenter), yCenter + (x - xCenter ),xCenter,yCenter ,color);

            c=quarter2-step;
            if(c>=c1 && c<=c2)
                 GDD_DrawLineEx(hdc, xCenter - (y - yCenter), yCenter - (x - xCenter),xCenter,yCenter,color);

            if (  x - xCenter  >=  y - yCenter  ) break;

            c=quarter3+step;
            if(c>=c1 && c<=c2)
                 GDD_DrawLineEx(hdc, x, y,xCenter,yCenter ,color);

            c=quarter1-step;
            if(c>=c1 && c<=c2)
                 GDD_DrawLineEx(hdc,x, yCenter - (y - yCenter),xCenter,yCenter ,color);

            c=quarter3-step;
            if(c>=c1 && c<=c2)
                 GDD_DrawLineEx(hdc, xCenter - (x - xCenter), y ,xCenter,yCenter,color);

            c= quarter1+step;
            if(c>=c1 && c<=c2)
                GDD_DrawLineEx(hdc, xCenter - (x - xCenter), yCenter - (y - yCenter) ,xCenter,yCenter,color);

            if ( d < 0 )
            {
                d = d + ((x - xCenter) << 2) + 6;
            }
            else
            {
                d = d + (((x - xCenter) - (y - yCenter)) << 2 ) + 10;
                y--;
            }
            x++;
            step++;
        }
        __GDD_EndDraw(hdc);
    }
}

//----绘制3阶Bezier线----------------------------------------------------------
//描述: 按指定坐标点绘制连续的贝塞尔线
//参数：hdc: 绘图上下文句柄.
//      pt: 线段坐标点
//      cnt: 坐标点数量
//返回：无.
//-----------------------------------------------------------------------------
void GDD_DrawBezier3(HDC hdc,const POINT *pt,s32 cnt)
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

        GDD_DrawLine(hdc,x0,y0,xt,yt);

        x0 =xt;
        y0 =yt;

    }

}


//----绘制折线------------------------------------------------------------------
//描述: 使用DrawColor绘制由多段直线组成的折线.
//参数：hdc: 绘图上下文句柄.
//      pt: 折线坐标点数组.
//      count: 坐标点数量.
//返回：无.
//------------------------------------------------------------------------------
void    GDD_DrawPolyLine(HDC hdc,const POINT *pt,s32 count)
{
    s32 i;

    if(count>1)
    {
        for(i=1;i<count;i++)
        {
            GDD_DrawLine(hdc,pt[i-1].x,pt[i-1].y,pt[i].x,pt[i].y);
        }
    }
}

//----绘制组合框------------------------------------------------------------------
//描述: 使用DrawColor绘制组合框边框,使用TextColor绘制组合框文字.
//参数：hdc: 绘图上下文句柄.
//      prc: 组合框矩形参数.
//      Text:组合框文字.
//返回：无.
//------------------------------------------------------------------------------

void GDD_DrawGroupBox(HDC hdc,const RECT *prc,const char *Text)
{
    s32 i,text_w,text_h,text_offset;
    u32 old_color;

    POINT pt[6];
    RECT rc;

    rc.left =0;
    rc.top =0;
    rc.right =0;
    rc.bottom =0;
    GDD_AdjustTextRect(hdc,Text,-1,&rc,DT_CENTER|DT_VCENTER);

    text_w =GDD_RectW(&rc);
    text_h =GDD_RectH(&rc);
    text_offset =8;

    pt[0].x =prc->left+text_offset;
    pt[0].y =prc->top+(GDD_RectH(&rc)>>1);

    pt[1].x =pt[0].x-text_offset;
    pt[1].y =pt[0].y;

    pt[2].x =pt[1].x;
    pt[2].y =prc->top+GDD_RectH(prc)-1;

    pt[3].x =pt[2].x+GDD_RectW(prc)-1;
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

    old_color=GDD_GetDrawColor(hdc);
    GDD_SetDrawColor(hdc,GDD_AlphaBlendColor(RGB(255,255,255),old_color,128));
    GDD_DrawPolyLine(hdc,pt,6);

    ////Top
    for(i=0;i<6;i++)
    {
        pt[i].x-=1;
        pt[i].y-=1;
    }
    GDD_SetDrawColor(hdc,old_color);
    GDD_DrawPolyLine(hdc,pt,6);

    GDD_SetRect(&rc,pt[0].x+1,prc->top,text_w,text_h);
    GDD_DrawText(hdc,Text,-1,&rc,DT_CENTER|DT_VCENTER);


}
//----绘制位图------------------------------------------------------------------
//描述: 位图格式由 gui kernel 定义，图形句柄 hdc 的 rop 属性会影响输出效果。当你看到
//      “奇怪”的绘制结果时，请检查 hdc->RopCode 的设置。
//参数：hdc: 绘图上下文句柄.
//      x，y，绘图坐标
//      bitmap，待绘制的位图
//      HyalineColor，透明色，hdc的rop属性中的透明色使能属性置位才有效
//返回：无.
//------------------------------------------------------------------------------
bool_t    GDD_DrawBitmap(HDC hdc,s32 x,s32 y,struct RectBitmap *bitmap,u32 HyalineColor)
{
    POINT pt;
    if(__GDD_BeginDraw(hdc))
    {
        pt.x =x;
        pt.y =y;
        __GDD_Cdn_DC_toWin(hdc,&pt,1);

        GK_DrawBitMap(hdc->pGkWin,&hdc->DrawArea, bitmap,pt.x,pt.y,HyalineColor,hdc->RopCode,hdc->SyncTime);
        __GDD_EndDraw(hdc);
        return true;
    }
    return false;
}
