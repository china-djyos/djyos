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

//����ģ��: gui
//����:  ������.
//�汾��V1.0.0
//�ļ�����: gui kernelӦ�ó���ӿڲ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2011-10-07
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "djyos.h"
#include "lock.h"
#include <string.h>
#include <charset.h>
#include <loc_string.h>
#include "systime.h"
#include "gkernel.h"
#include "gk_syscall.h"
#include "gk_win.h"
#include <gui/gkernel/gk_display.h>
//----��������-----------------------------------------------------------------
//����: �������棬����ʾ�����������Ҫ�����������ʹ�á�������ʵ��һ����ͨ����
//      �ǳ����ƣ������Ҫ����:
//          1�����治��С����ʾ���ߴ磬�����ڿ��ԡ�
//          2�������rop����ֻ����0,���ڸ����Щ��
//          3����������ȼ�ֻ����0��
//          4��һ����ʾ���£�����ֻ��һ������display��desktopָ��ָ��
//      ��һ����ʾ���µ����д��ڶ���������Ӵ��ڡ�
//����: display�������������ʾ��
//      desktop���´��������棬�������ݽṹ�����ڴ��ɵ������ṩ
//      width��height������ĳߴ磬С����ʾ���ߴ������Ϊ��ʾ���ߴ�
//      color����������ʱ������ɫ
//      buf_mode�����建��ģʽ���μ� CN_WINBUF_PARENT �峣������
//      PixelFormat,���ظ�ʽ��CN_SYS_PF_DISPLAY��ʾ����ʾ����ͬ���Ƽ���
//      BaseColor, �Ҷ�ͼ��ɫ��(����pf_type == CN_SYS_PF_GRAY1 ~8ʱ����)
//����: true=�����ɹ���false=����ʧ��
//-----------------------------------------------------------------------------
bool_t GK_CreateDesktop(struct DisplayRsc *display,
                           struct GkWinRsc *desktop,
                           s32 width,s32 height,u32 color,
                           u32 buf_mode,u16 PixelFormat,
                           u32 BaseColor)
{
    struct GkscParaCreateDesktop para;
    struct GkWinRsc *result;
    if((NULL == display) || (NULL == desktop))
        return false;
    para.display = display;
    para.desktop = desktop;
    para.width = width;
    para.height = height;
    para.color = color;
    para.buf_mode = buf_mode;
    para.PixelFormat = PixelFormat;
    para.BaseColor = BaseColor;
    result = (void*)__GK_CreateDesktop(&para);
    if(result == NULL)
        return false;
    else
        return true;
}

//----��ȡ����ָ��-------------------------------------------------------------
//����: ������ʾ��������������ָ�롣��ָ��ָ��һ�����ڿ��ƿ�
//����: display_name������������ʾ��������
//����: ����ҵ����򷵻�ָ�룬���򷵻�NULL
//-----------------------------------------------------------------------------
struct GkWinRsc *GK_GetDesktop(const char *display_name)
{
    struct DisplayRsc *display;
    display = (struct DisplayRsc *)OBJ_SearchTree("display");
    if(display != NULL)
        display = (struct DisplayRsc *)OBJ_SearchChild(
                (struct Object*)display,display_name);
    if(display != NULL)
        return display->desktop;
    else
        return NULL;
}
//----��������-----------------------------------------------------------------
//����: ����һ�����ڣ��´����Ĵ���ֻ��һ����������û����䣬Ҳû�б߿��һ��
//      װ�Ρ�
//����: parent��������ָ��
//      newwin���´����Ĵ��ڣ��������ݽṹ�����ڴ��ɵ������ṩ
//      left��top��right��bottom�����ڵĳߴ磬����ڸ�����
//      color����������ʱ������ɫ
//      buf_mode�����建��ģʽ���μ� CN_WINBUF_PARENT �峣������
//      name���´��ڵ����֣���copy��newwin��
//      PixelFormat,���ظ�ʽ��CN_SYS_PF_DISPLAY��ʾ����ʾ����ͬ���Ƽ���
//      HyalineColor,�趨����͸��ɫ,��RopMode��ʹ����͸��ɫʱ,������KeyColor
//      BaseColor, �Ҷ�ͼ��ɫ��(����PixelFormat == CN_SYS_PF_GRAY1 ~8ʱ����)
//      RopMode, ����룬�μ� CN_ROP_ALPHA_SRC_MSK �峣������,0��ʾ�����⹦��
//����: true=�����ɹ���false=����ʧ��
//-----------------------------------------------------------------------------
bool_t GK_CreateWin(struct GkWinRsc *parent,
                         struct GkWinRsc *newwin,
                         s32 left,s32 top,s32 right,s32 bottom,
                         u32 color,u32 buf_mode,
                         const char *name,u16 PixelFormat,u32 HyalineColor,
                         u32 BaseColor,struct RopGroup RopMode)
{
    struct GkscParaCreateGkwin para;
    struct GkWinRsc *result;
    if((NULL == parent) || (NULL == newwin))
        return false;
    para.parent_gkwin = parent;
    para.gkwin = newwin;
    para.result = &result;
    para.left = left;
    para.top = top;
    para.right = right;
    para.bottom = bottom;
    para.color = color;
    para.buf_mode = buf_mode;
    para.name = (char*)name;
    para.PixelFormat = PixelFormat;
    para.HyalineColor = HyalineColor;
    para.BaseColor = BaseColor;
    para.RopCode = RopMode;
    __GK_SyscallChunnel(CN_GKSC_CREAT_GKWIN,CN_TIMEOUT_FOREVER,
                            &para,sizeof(para),NULL,0);
    if(result == NULL)
        return false;
    else
    {
        //gui kernel��������ʱ,�����buf����,���ڴ�����ͬʱ��������,����,
        //������ִ�����.
        //��buf���ڲ����ٴ���ͬʱ����ԭ����,����buf������ֱ�ӻ�����screen
        //����framebuffer�ϵ�,����������ʱ,��������δ����.ֱ������ GK_SyncShow
        //�󣬿�������ܴ����á�
        if(newwin->wm_bitmap == NULL)   //wm_bitmap==NULL ��ʾ�޴��ڻ�������
        {
            GK_SyncShow(CN_TIMEOUT_FOREVER);
            GK_FillWin(newwin,color,0);
        }
        return true;
    }
}
//----�첽��䴰��-------------------------------------------------------------
//����: ��ָ����ɫ���һ�����ڡ�
//����: gkwin�������Ĵ���
//      color�������ɫ
//����: ��
//-----------------------------------------------------------------------------
void GK_FillWin(struct GkWinRsc *gkwin,u32 color,u32 SyncTime)
{
    struct GkscParaFillWin para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.color = color;
    __GK_SyscallChunnel(CN_GKSC_FILL_WIN,SyncTime,&para,sizeof(para),NULL,0);
    return;
}
//----ˢ�´���-----------------------------------------------------------------
//����: ִ�л�����ͨ���е�����, ��ˢ�µ���ʾ����
//����: SyncTime, ͬ����ʱʱ��
//����: ��
//-----------------------------------------------------------------------------
void GK_SyncShow(u32 SyncTime)
{
    __GK_SyscallChunnel(CN_GKSC_SYNC_SHOW,SyncTime,NULL,0,NULL,0);
    return;
}

//----��ˢ��ʾ��---------------------------------------------------------------
//����: ����ˢ����ʾ��,�����Ƿ��и��¡�GK_SyncShowֻ��ˢ���иı�Ĳ���,
//      GK_ApiRefresh���ǲ�����û�иı�,ȫ����ˢ,��ֻ��Ծ�����ʾ����
//      ���������Ӧ�õ��ñ�����:
//      1. �о�����ʾ���������򷢳�����.
//      2. ��ʾ����������������,����ʾ���ڲ�û�л���.
//      3. ��⵽����ʾ������.
//����: Display, ��ˢ�µ���ʾ��
//      SyncTime, ͬ����ʱʱ��
//      color�������ɫ
//����: ��
//-----------------------------------------------------------------------------
void GK_RefreshDisplay(struct DisplayRsc *Display)
{
    __GK_SyscallChunnel(CN_GKSC_DSP_REFRESH,CN_TIMEOUT_FOREVER,
                    &Display,sizeof(struct DisplayRsc *),NULL,0);
    return;
}

#if 0
//----��䴰�ڵ�һ����---------------------------------------------------------
//����: ��ָ����ɫ���һ�����ڵ�һ���֣���ɫ��alphaֵ�������
//����: gkwin��������Ŀ�괰��
//      rect��������λ��
//      AlphaL,AlphaR,AlphaT,AlphaB,�������µ�Alphaֵ
//      color��Ҫ�����ɫ
//����: ��
//-----------------------------------------------------------------------------
void GK_FillPartWin(struct GkWinRsc *gkwin,
                       struct Rectangle rect,u32 color,
                       u32 SyncTime)
{
    struct GkscParaFillPartWin para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.rect = rect;
    para.color = color;
    __GK_SyscallChunnel(CN_GKSC_FILL_PART_WIN,SyncTime,&para,
                        sizeof(para),NULL,0);
    return;
}
#endif

//----����ɫ��䴰�ڵ�һ����---------------------------------------------------
//����: ��ָ����ɫ���һ�����ڵ�һ���֣���ɫ��alphaֵ�������
//����: gkwin��������Ŀ�괰��
//      rect��������λ��
//      Color0,��ɫ1�������CN_FILLRECT_MODE_LR�峣������
//      Color1,��ɫ2�������CN_FILLRECT_MODE_LR�峣������
//      Mode,����ģʽ,�����CN_FILLRECT_MODE_LR�峣������
//����: ��
//-----------------------------------------------------------------------------
void GK_FillRect(struct GkWinRsc *gkwin,struct Rectangle *rect,
                            u32 Color0,u32 Color1,u32 Mode,u32 SyncTime)
{
    struct GkscParaGradientFillWin para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.rect = *rect;
    para.Color0 = Color0;
    para.Color1 = Color1;
    para.Mode = Mode;
    __GK_SyscallChunnel(CN_GKSC_FILL_RECT,SyncTime,&para,
                        sizeof(para),NULL,0);
    return;
}

//----��ʾ�ı�-----------------------------------------------------------------
//����: ��Ĭ�������Ĭ���ַ�����ʾһ���ı�����
//����: gkwin����ʾ��Ŀ�괰��
//      pFont, ʹ�õ�����,NULL��ʾ��ϵͳ��ǰ����
//      pCharset, ʹ�õ��ַ���,NULL��ʾ��ϵͳ��ǰ�ַ���
//      x��y����ʾλ�ã������gkwin
//      text������ʾ���ַ���
//      count���ַ������ȣ�����������
//      color, ��ʾ��ɫ��RGB888��ʽ
//      Rop2Mode����Ԫ��դ�����룬�μ� CN_R2_BLACK �峣������
//����: ��
//-----------------------------------------------------------------------------
void GK_DrawText(struct GkWinRsc *gkwin,
                    struct FontRsc *pFont,
                    struct Charset *pCharset,
                    s32 x,s32 y,
                    const char *text,u32 count,u32 color,
                    u32 Rop2Code,u32 SyncTime)
{
    u16 TextBytes;
    struct GkscParaDrawText para;
    if((NULL == gkwin) || (NULL == text) || (0 == count))
        return;
    para.gkwin = gkwin;
    para.pFont = pFont;
    para.pCharset = pCharset;
    para.x = x;
    para.y = y;
    para.count = count;
    para.color = color;
    para.Rop2Code = Rop2Code;
    TextBytes = strlen(text);
    TextBytes += GetEOC_Size(pCharset);
    __GK_SyscallChunnel(CN_GKSC_DRAW_TEXT,SyncTime,
                        &para,sizeof(para),(void*)text,TextBytes);
    return;
}
//----����---------------------------------------------------------------------
//����: �������ء�
//����: gkwin�����Ƶ�Ŀ�괰��
//      x��y�����Ƶ�λ�ã������gkwin
//      color����������ɫ
//      Rop2Mode����Ԫ��դ�����룬�μ� CN_R2_BLACK �峣������
//����: ��
//-----------------------------------------------------------------------------
void GK_SetPixel(struct GkWinRsc *gkwin,s32 x,s32 y,
                        u32 color,u32 Rop2Code,u32 SyncTime)
{
    struct GkscParaSetPixel para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.x = x;
    para.y = y;
    para.color = color;
    para.Rop2Code = Rop2Code;
    __GK_SyscallChunnel(CN_GKSC_SET_PIXEL,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}
//----�������ؿ�ֱ�ߣ������������˵�-------------------------------------------
//����: ����ֱ�ߣ������������˵�
//����: gkwin�����Ƶ�Ŀ�괰��
//      x1��y1��x2��y2,Ҫ���Ƶ�ֱ�ߵ����յ�����
//      color�����Ƶ���ɫ
//      Rop2Mode����Ԫ��դ�����룬�μ� CN_R2_BLACK �峣������
//����: ��
//-----------------------------------------------------------------------------
void GK_Lineto(struct GkWinRsc *gkwin, s32 x1,s32 y1,
                    s32 x2,s32 y2,u32 color,u32 Rop2Code,u32 SyncTime)
{
    struct GkscParaLineto para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.x1 = x1;
    para.y1 = y1;
    para.x2 = x2;
    para.y2 = y2;
    para.color = color;
    para.Rop2Code = Rop2Code;
    __GK_SyscallChunnel(CN_GKSC_LINETO,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}
//----�������ؿ�ֱ�ߣ����������˵�---------------------------------------------
//����: ����ֱ�ߣ����������˵�
//����: gkwin�����Ƶ�Ŀ�괰��
//      x1��y1��x2��y2,Ҫ���Ƶ�ֱ�ߵ����յ�����
//      color�����Ƶ���ɫ
//      Rop2Mode����Ԫ��դ�����룬�μ� CN_R2_BLACK �峣������
//����: ��
//-----------------------------------------------------------------------------
void GK_LinetoIe(struct GkWinRsc *gkwin, s32 x1,s32 y1,
                    s32 x2,s32 y2,u32 color,u32 Rop2Code,u32 SyncTime)
{
    struct GkscParaLineto para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.x1 = x1;
    para.y1 = y1;
    para.x2 = x2;
    para.y2 = y2;
    para.color = color;
    para.Rop2Code = Rop2Code;
    __GK_SyscallChunnel(CN_GKSC_LINETO_INC_END,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}

//----����դλͼ---------------------------------------------------------------
//����: ����bitmap������դ������
//����: gkwin�����Ƶ�Ŀ�괰��
//      bitmap�������Ƶ�λͼ
//      pattern��Ҫ���õ�ͼ��λͼ
//      Rop4Msk��rop4��������Ҫ������λͼ��һ����ɫλͼ
//      KeyColor��͸��ɫ
//      x��y�����Ƶ�Ŀ��λ��
//      RopMode, ��դ�룬�μ� CN_ROP_ALPHA_SRC_MSK �峣������
//����: ��
//-----------------------------------------------------------------------------
void GK_DrawBitMap(struct GkWinRsc *gkwin,
                    struct RectBitmap *bitmap,
                    s32 x,s32 y,
                    u32 HyalineColor,struct RopGroup RopCode,u32 SyncTime)
{
    struct GkscParaDrawBitmapRop para;
    memset(&para,sizeof(para),0);
    if(NULL == gkwin)
        return;
    else
        para.gkwin = gkwin;
    if(NULL == bitmap)
        return;
    else
        para.bitmap = *bitmap;

    para.HyalineColor = HyalineColor;
    para.x = x;
    para.y = y;
    para.RopCode = RopCode;
    __GK_SyscallChunnel(CN_GKSC_DRAW_BITMAP_ROP,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}
//----�������ؿ���Բ-----------------------------------------------------------
//����: �������ؿ���Բ��
//����: gkwin�����Ƶ�Ŀ�괰��
//      x0��y0��Բ�����ĵ����꣬����ڴ���
//      r��Բ�İ뾶
//      color����Բʹ�õ���ɫ
//      Rop2Mode����Ԫ��դ�����룬�μ� CN_R2_BLACK �峣������
//����: ��
//-----------------------------------------------------------------------------
void GK_DrawCircle(struct GkWinRsc *gkwin,s32 x0,s32 y0,
                    u32 r,u32 color,u32 Rop2Code,u32 SyncTime)
{
    struct GkscParaDrawCircle para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.x0 = x0;
    para.y0 = y0;
    para.r = r;
    para.color = color;
    para.Rop2Code = Rop2Code;
    __GK_SyscallChunnel(CN_GKSC_DRAW_CIRCLE,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}
//----��Bezier����-------------------------------------------------------------
//����: ��Bezier���ߡ�
//����: gkwin�����Ƶ�Ŀ�괰��
//      x1��y1��x2��y2��x3��y3��x4��y4������Bezier���ߵ��ĸ����Ƶ�
//      color�����Ƶ���ɫ
//      Rop2Mode����Ԫ��դ�����룬�μ� CN_R2_BLACK �峣������
//����: ��
//-----------------------------------------------------------------------------
void GK_DrawBezier(struct GkWinRsc *gkwin,float x1,float y1,
                    float x2,float y2,float x3,float y3,float x4,float y4,
                    u32 color,u32 Rop2Code,u32 SyncTime)
{
    struct GkscParaBezier para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.x1 = x1;
    para.y1 = y1;
    para.x2 = x2;
    para.y2 = y2;
    para.x3 = x3;
    para.y3 = y3;
    para.x4 = x4;
    para.y4 = y4;
    para.color = color;
    para.Rop2Code = Rop2Code;
    __GK_SyscallChunnel(CN_GKSC_BEZIER,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}

//----��������ȡ����-----------------------------------------------------------
//����: ���ݴ�������꣬��ȡ���ڴ��ڣ������ڸô��ڵĿ������С����������������ʾ
//     ���� ��ǰ�˴��ڿ�ʼ������ң���Ϊ����͸��������˵�����串�ǵĴ��ڣ�������
//     ����Ҳ�ǿɼ��ġ�
//����: desktop��Ŀ������
//      pt��Ŀ�������
//����: ��
//-----------------------------------------------------------------------------
struct GkWinRsc* GK_GetWinFromPt(struct GkWinRsc *desktop,
                                    const struct PointCdn *pt)
{
    struct GkWinRsc *current;
    struct GkWinRsc *result=NULL;
    struct ClipRect *VisibleClip,*VisibleHead;

    if((NULL == desktop) || (NULL == pt))
        return NULL;
    current = desktop->disp->z_topmost;
    while(current != desktop)
    {
        if(current->WinProperty.Zprio > CN_ZPRIO_CTRL)
        {
            VisibleClip = ((struct GkWinRsc *)current)->visible_clip;
            VisibleHead = VisibleClip;
            while(VisibleClip != NULL)
            {
                if(PtInRect(&VisibleClip->rect,pt))
                {
                    result = (struct GkWinRsc *)current;
                    break;
                }
                if(VisibleClip->next == VisibleHead)
                {
                    VisibleClip = NULL;
                }
                else
                {
                    VisibleClip = VisibleClip->next;
                }
            }
        }
        if(result != NULL)
            break;
        current = current->z_back;
    }
    if(result)
        return result;
    else
        return desktop;
}

//---ȡĩ�˴���--------------------------------------------------------------
//����: ȡ��ǰ���ڵ�һ��ĩ�˺�����ڣ���û���Ӵ��ڵĴ��ڡ�
//����: Ancestor�����ȴ���
//      Current����ǰ����λ��
//����: ��һ���������
//-----------------------------------------------------------------------------
struct GkWinRsc* GK_GetTwig(struct GkWinRsc *Ancestor)
{
    struct Object *result;

    if(NULL == Ancestor)
        return NULL;
    result = OBJ_GetTwig(&Ancestor->node);
    return Container(result, struct GkWinRsc, node);
}

//---�����������--------------------------------------------------------------
//����: ���ݵ�ǰλ�ã�������һ��������ڣ���������ֱ������NULL��
//����: Ancestor�����ȴ���
//      Current����ǰ����λ��
//����: ��һ���������
//-----------------------------------------------------------------------------
struct GkWinRsc* GK_TraveScion(struct GkWinRsc *Ancestor,struct GkWinRsc *Current)
{
    struct Object *result;

    if((NULL == Ancestor) || (NULL == Current))
        return NULL;
    result = OBJ_TraveScion(&Ancestor->node, &Current->node);
    return Container(result, struct GkWinRsc, node);
}

//---�����Ӵ���--------------------------------------------------------------
//����: ���ݵ�ǰλ�ã�������һ���Ӵ��ڣ���������ֱ������NULL��
//����: Parent��������
//      Current����ǰ����λ��
//����: ��һ��������
//-----------------------------------------------------------------------------
struct GkWinRsc* GK_TraveChild(struct GkWinRsc *Parent,struct GkWinRsc *Current)
{
    struct Object *result;

    if((NULL == Parent) || (NULL == Current))
        return NULL;
    result = OBJ_TraveChild(&Parent->node, &Current->node);
    return Container(result, struct GkWinRsc, node);
}

//----���̴���-----------------------------------------------------------------
//���ܣ���һ�����ڴ��丸�������Ƴ�������������ڵ��Ӵ��ڡ����ڵ��������Բ��䡣
//������gkwin��Ŀ�괰��
//      NewParent���µĸ�����
//����: ��
//-----------------------------------------------------------------------------
void GK_AdoptWin(struct GkWinRsc *gkwin, struct GkWinRsc *NewParent)
{
    struct GkscParaAdoptWin para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.NewParent = NewParent;
    __GK_SyscallChunnel(CN_GKSC_ADOPT_WIN,CN_TIMEOUT_FOREVER,
                        &para,sizeof(para),NULL,0);
    return;
}

//----�ƶ�����-----------------------------------------------------------------
//����: �ƶ����ڡ�
//����: gkwin��Ŀ�괰��
//      left��top�������ƶ�������Ͻ����꣬����ڸ�����
//����: ��
//-----------------------------------------------------------------------------
void GK_MoveWin(struct GkWinRsc *gkwin, s32 left,s32 top,u32 SyncTime)
{
    struct GkscParaMoveWin para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.left = left;
    para.top = top;
    __GK_SyscallChunnel(CN_GKSC_MOVE_WIN,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}

//----�ı䴰��λ�óߴ�---------------------------------------------------------
//����: �ı䴰�ڵ�λ�úͳߴ磬�������������ڸ�����
//����: gkwin��Ŀ�괰��
//      left��top��right��bottom�����ڵ������꣬����ڸ�����
//����: ��
//-----------------------------------------------------------------------------
void GK_ChangeWinArea(struct GkWinRsc *gkwin, s32 left,s32 top,
                                       s32 right,s32 bottom, u32 SyncTime)
{
    struct GkscParaChangeWinArea para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.left = left;
    para.top = top;
    para.right = right;
    para.bottom = bottom;
    __GK_SyscallChunnel(CN_GKSC_CHANGE_WIN_AREA,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}

//----���ô��ڵı߽�ģʽ-------------------------------------------------------
//����: ���ô��ڵı߽�ģʽ��
//����: gkwin��Ŀ�괰��
//      mode�� ���ڵı߽�ģʽ��CN_BOUND_LIMITΪ���ޣ�CN_BOUND_UNLIMITΪ������
//����: ��
//-----------------------------------------------------------------------------
void GK_SetBoundMode(struct GkWinRsc *gkwin, u32 mode)
{
    struct GkscParaSetBoundMode para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.mode = mode;
    __GK_SyscallChunnel(CN_GKSC_SET_BOUND_MODE,CN_TIMEOUT_FOREVER,
                        &para,sizeof(para),NULL,0);
    return;
}
//----���ô�����ʾ���ȼ�-------------------------------------------------------
//����: ���ô�����ʾ���ȼ�����ʹ��prio��Ԫprio��ȣ�Ҳ����Ӱ����ʾ�������gkwin
//      �ᵽ����prio��ͬ���ֵܴ���ǰ�ˡ�
//����: gkwin��Ŀ�괰��
//      prio�������ȼ���
//      sync_time������ͼ��ʱ��ͬ�����ڴ�ʱ���ڲ����Ƿ������ɶ�����
//����: ��
//-----------------------------------------------------------------------------
void GK_SetPrio(struct GkWinRsc *gkwin, u32 prio,u32 SyncTime)
{
    struct GkscParaSetPrio para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.prio = prio;
    __GK_SyscallChunnel(CN_GKSC_SET_PRIO,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}

//----���ô����Ƿ����-------------------------------------------------------
//����: ���ô��ڵĿ�������
//����: gkwin��Ŀ�괰��
//      visible��CN_GKWIN_VISIBLE=���ӣ�CN_GKWIN_HIDE=����
//      sync_time������ͼ��ʱ��ͬ�����ڴ�ʱ���ڲ����Ƿ������ɶ�����
//����: ��
//-----------------------------------------------------------------------------
void GK_SetVisible(struct GkWinRsc *gkwin, u32 visible,u32 SyncTime)
{
    struct GkscParaSetVisible para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.Visible = visible;
    __GK_SyscallChunnel(CN_GKSC_SET_VISIBLE,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}

//----���ô��ڵĹ�դ����-------------------------------------------------------
//����: ���ô��ڵĹ�դ���ԡ�
//����: gkwin��Ŀ�괰��
//      Rop4Msk,����λͼ
//      RopMode, ����룬�μ� CN_ROP_ALPHA_SRC_MSK �峣�����壬��֧�ֶ�Ԫ��դ���
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t GK_SetRopCode(struct GkWinRsc *gkwin,
                        struct RopGroup RopCode,u32 SyncTime)
{
    struct GkscParaSetRopCode para;
    memset(&para,sizeof(para),0);
    if(NULL == gkwin)
        return false;
    if(memcmp(&RopCode, &gkwin->RopCode, sizeof(RopCode) )== 0)
        return true;
    para.gkwin = gkwin;
    para.RopCode = RopCode;
    __GK_SyscallChunnel(CN_GKSC_SET_ROP_CODE,SyncTime,
                        &para,sizeof(para),NULL,0);
    return true;
}

//----���ô���͸��ɫ-----------------------------------------------------------
//����: ���ô��ڵ�͸��ɫ��
//����: gkwin��Ŀ�괰��
//      KeyColor��Ҫ���õ�͸��ɫ
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t GK_SetHyalineColor(struct GkWinRsc *gkwin,u32 HyalineColor)
{
    struct GkscParaSetHyalineColor para;
    if(NULL == gkwin)
        return false;
    if(HyalineColor == gkwin->HyalineColor)
        return true;
    para.gkwin = gkwin;
    para.HyalineColor = HyalineColor;
    __GK_SyscallChunnel(CN_GKSC_SET_HYALINE_COLOR,CN_TIMEOUT_FOREVER,
                        &para,sizeof(para),NULL,0);
    return true;
}

//----���ô���ֱ��д������-----------------------------------------------------
//����: ���ô���ֱ��д�����ԡ�
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void GK_SetDirectScreen(struct GkWinRsc *gkwin,u32 SyncTime)
{
    if(NULL == gkwin)
        return;
    __GK_SyscallChunnel(CN_GKSC_SET_DIRECT_SCREEN,SyncTime,
                        &gkwin,sizeof(struct GkWinRsc *),NULL,0);
    return;
}
//----��������ֱ��д������-----------------------------------------------------
//����: ��������ֱ��д�����ԡ�
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void GK_SetUnDirectScreen(struct GkWinRsc *gkwin,u32 SyncTime)
{
    if(NULL == gkwin)
        return;
    __GK_SyscallChunnel(CN_GKSC_SET_UNDIRECT_SCREEN,SyncTime,
                        &gkwin,sizeof(struct GkWinRsc *),NULL,0);
    return;
}
//----���ٴ���-----------------------------------------------------------------
//����: �ͷŴ�����ռ�õ���Դ��
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void GK_DestroyWin(struct GkWinRsc *gkwin)
{
    if(NULL == gkwin)
        return;
    __GK_SyscallChunnel(CN_GKSC_DESTROY_WIN,CN_TIMEOUT_FOREVER,
                        &gkwin,sizeof(struct GkWinRsc *),NULL,0);
    return;
}

//----��ѯ�Կ���ʽ-------------------------------------------------------------
//����: ��ѯ�Կ�ʹ�õ���ɫ��ʽ����λͼʱ�����ʹ�ø��Կ���ͬ����ɫ��ʽ�������
//      ��������
//����: display������ѯ���Կ�
//����: ��ɫ��ʽ
//-----------------------------------------------------------------------------
u16 GK_GetPixelFormat(struct DisplayRsc *display)
{
    if(display != NULL)
        return display->pixel_format;
    else
        return 0;
}

//----ȡgkwin���û�����--------------------------------------------------------
//����: ��
//����: gkwin������ѯgkwin
//����: �û�����
//-----------------------------------------------------------------------------
void *GK_GetUserTag(struct GkWinRsc *gkwin)
{
    if(gkwin != NULL)
        return gkwin->UserTag;
    else
        return 0;
}

//----����gkwin���û�����------------------------------------------------------
//����: ��
//����: gkwin��������gkwin
//����: ��
//-----------------------------------------------------------------------------
void GK_SetUserTag(struct GkWinRsc *gkwin,void *Tag)
{
    if(gkwin != NULL)
        gkwin->UserTag = Tag;
    return;
}

//----ȡ������ָ��------------------------------------------------------
//����: ��
//����: gkwin��������
//����: �Ӵ���ָ��
//-----------------------------------------------------------------------------
struct GkWinRsc *GK_GetParentWin(struct GkWinRsc *gkwin)
{
    if(gkwin != NULL)
        return Container(OBJ_Parent(&gkwin->node), struct GkWinRsc, node);
    else
        return NULL;
}
//----ȡ�Ӵ���ָ��------------------------------------------------------
//����: ��
//����: gkwin��������
//����: �Ӵ���ָ��
//-----------------------------------------------------------------------------
struct GkWinRsc *GK_GetChildWin(struct GkWinRsc *gkwin)
{
    if(gkwin != NULL)
        return Container(OBJ_Child(&gkwin->node),struct GkWinRsc,node);
    else
        return NULL;
}

//----ȡǰ����ָ��------------------------------------------------------
//����: ��
//����: gkwin������
//����: ǰ����ָ��
//-----------------------------------------------------------------------------
struct GkWinRsc *GK_GetPreviousWin(struct GkWinRsc *gkwin)
{
    if(gkwin != NULL)
        return Container(OBJ_Previous(&gkwin->node), struct GkWinRsc, node);
    else
        return NULL;
}

//----ȡ�󴰿�ָ��------------------------------------------------------
//����: ��
//����: gkwin������
//����: �󴰿�ָ��
//-----------------------------------------------------------------------------
struct GkWinRsc *GK_GetNextWin(struct GkWinRsc *gkwin)
{
    if(gkwin != NULL)
        return Container(OBJ_Next(&gkwin->node), struct GkWinRsc, node);
    else
        return NULL;
}

//----ȡ��ǰ����ָ��------------------------------------------------------
//����: ȡͬ�鴰�ڵ���ǰ�˴���ָ�룬ע�⣬����ȫ�����ڵ�Z����ǰ�ˡ�
//����: gkwin������
//����: ��ǰ����ָ��
//-----------------------------------------------------------------------------
struct GkWinRsc *GK_GetFirstWin(struct GkWinRsc *gkwin)
{
    if(gkwin != NULL)
        return Container(OBJ_GetHead(&gkwin->node), struct GkWinRsc, node);
    else
        return NULL;
}

//----ȡ��󴰿�ָ��------------------------------------------------------
//����: ȡͬ�鴰�ڵ����˴���ָ�룬ע�⣬����ȫ�����ڵ�Z�����ˡ�
//����: gkwin������
//����: ��󴰿�ָ��
//-----------------------------------------------------------------------------
struct GkWinRsc *GK_GetLastWin(struct GkWinRsc *gkwin)
{
    if(gkwin != NULL)
        return Container(OBJ_GetTwig(&gkwin->node), struct GkWinRsc, node);
    else
        return NULL;
}

//----ȡ������ʾ����-----------------------------------------------------------
//����: ��
//����: gkwin������
//����: ��ʾ����,��ʾ���ľ�������
//-----------------------------------------------------------------------------
void GK_GetArea(struct GkWinRsc *gkwin, struct Rectangle *rc)
{
    if((gkwin != NULL) && (rc != NULL))
    {
        rc->left = gkwin->absx0;
        rc->top = gkwin->absy0;
        rc->right = gkwin->absx0 + gkwin->right - gkwin->left;
        rc->bottom = gkwin->absy0 + gkwin->bottom - gkwin->top;
    }
    return ;
}

//----���ڸ���-----------------------------------------------------------------
//����: �޸Ĵ������֣��������127�ַ����������ֽ��ضϡ�
//����: gcwin�����޸ĵĴ��ھ��(ָ��)
//      name���µ�����
//����: ��
//-----------------------------------------------------------------------------
void GK_SetName(struct GkWinRsc *gkwin,const char *name)
{
    s32 size;
    size = strnlen(name,CN_GKWIN_NAME_LIMIT+1);
    if(size > CN_GKWIN_NAME_LIMIT)     //���ֳ��ȳ�������
        size = CN_GKWIN_NAME_LIMIT;   //ǿ���е���������
    memcpy(gkwin->win_name,name,size);//copy���֣�����ܳ�������δcopy��������\0��
    gkwin->win_name[size] = '\0';     //�����(�ӽ�����)
}

//----���ô�����-----------------------------------------------------------
//����: ��
//����: gkwin������
//����: ��ʾ����
//-----------------------------------------------------------------------------
char *GK_GetName(struct GkWinRsc *gkwin)
{
    if(gkwin != NULL)
    {
        return gkwin->win_name;
    }
    else
        return NULL;
}

//----�ж������Ƿ�ɼ�---------------------------------------------------------
//����: ��
//����: gkwin��Ŀ�괰��
//����: true=�ɼ����п����򣩣�false=���ɼ�
//-----------------------------------------------------------------------------
bool_t GK_IsWinVisible(struct GkWinRsc *gkwin)
{
    if(NULL == gkwin)
        return false;
    if(gkwin->visible_clip == NULL)
        return false;
    else
        return true;
}