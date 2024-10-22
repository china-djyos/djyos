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

//所属模块: gui
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: gui kernel应用程序接口部分
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2011-10-07
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "stdint.h"
#include "stdlib.h"
#include "djyos.h"
#include "lock.h"
#include <string.h>
#include <charset.h>
#include <loc_string.h>
#include "systime.h"
#include "gkernel.h"
#include "gk_usercall.h"
#include "gk_win.h"
#include <gdd.h>
#include <msgqueue.h>
#include <gui/gk_display.h>

#define __gk_swap(a, b){a^=b; b^=a; a^=b;}
//----创建桌面-----------------------------------------------------------------
//功能: 创建桌面，新显示器加入后，首先要创建桌面才能使用。桌面其实和一个普通窗口
//      非常类似，差别主要在于:
//          1、桌面不能小于显示器尺寸，而窗口可以。
//          2、桌面的rop属性只能是0,窗口更灵活些。
//          3、桌面的优先级只能是0。
//          4、一个显示器下，桌面只有一个，由display的desktop指针指向。
//      在一个显示器下的所有窗口都是桌面的子窗口。
//参数: display，桌面的宿主显示器
//      desktop，新创建的桌面，保存数据结构所需内存由调用者提供
//      width，height，桌面的尺寸，小于显示器尺寸则调整为显示器尺寸
//      color，创建桌面时填充的颜色
//      buf_mode，定义缓冲模式，参见 CN_WINBUF_PARENT 族常量定义
//      PixelFormat,像素格式，CN_SYS_PF_DISPLAY表示与显示器相同，推荐。
//      BaseColor, 灰度图基色，(仅在pf_type == CN_SYS_PF_GRAY1 ~8时有用)
//返回: 新创建的桌面窗口指针，NULL= 失败
//-----------------------------------------------------------------------------
struct GkWinObj *GK_CreateDesktop(const char *DisplayName,
                        const char *DesktopName,
                        s32 width,s32 height,u32 color,
                        u32 buf_mode,u16 PixelFormat,
                        u32 BaseColor)
{
    struct Object *DisplayObject;
    struct GkscParaCreateDesktop para;
    struct GkWinObj *result = NULL;
    struct GkWinObj *desktop;
    DisplayObject = OBJ_SearchChild(OBJ_GetRoot(), "display");         //取显示器目录
    DisplayObject = OBJ_SearchChild(DisplayObject, DisplayName);    //取显示器对象
    desktop = malloc(sizeof(struct GkWinObj));
    if((NULL == DisplayObject) || (NULL == desktop))
    {
        free(desktop);
        return result;
    }
    memset(desktop, 0, sizeof(struct GkWinObj));
    para.display = (struct DisplayObj *)OBJ_GetPrivate(DisplayObject);
    para.desktop = desktop;
    // para.name = (char *)DesktopName;
    strncpy(para.name, DesktopName, CN_GKWIN_NAME_LIMIT);
    para.name[CN_GKWIN_NAME_LIMIT] = '\0';
    para.width = width;
    para.height = height;
    para.color = color;
    para.buf_mode = buf_mode;
    para.PixelFormat = PixelFormat;
    para.BaseColor = BaseColor;
    result = (void*)__GK_CreateDesktop(&para);
    return result;
}

//----获取显示器指针-------------------------------------------------------------
//功能: 根据显示器名，获得显示器的指针。
//参数: display_name，显示器的名字
//返回: 如果找到，则返回指针，否则返回NULL
//-----------------------------------------------------------------------------
struct DisplayObj *GK_GetDisplay(const char *display_name)
{
    struct DisplayObj *display = NULL;
    struct Object *DispObj;
    DispObj = OBJ_SearchChild(OBJ_GetRoot( ),"display");
    if(DispObj != NULL)
    {
        DispObj = OBJ_SearchChild(DispObj,display_name);
        if(DispObj != NULL)
        {
            display = (struct DisplayObj *)OBJ_GetPrivate(DispObj);
        }
    }
    return display;
}

//----获取桌面指针-------------------------------------------------------------
//功能: 根据显示器名，获得桌面的指针。该指针指向一个窗口控制块
//参数: display_name，桌面所在显示器的名字
//返回: 如果找到，则返回指针，否则返回NULL
//-----------------------------------------------------------------------------
struct GkWinObj *GK_GetDesktop(const char *display_name)
{
    struct DisplayObj *display = NULL;
    struct Object *DispObj;
    DispObj = OBJ_SearchChild(OBJ_GetRoot( ),"display");
    if(DispObj != NULL)
    {
        DispObj = OBJ_SearchChild(DispObj,display_name);
        if(DispObj != NULL)
        {
            display = (struct DisplayObj *)OBJ_GetPrivate(DispObj);
        }
    }
    if(display != NULL)
        return display->desktop;
    else
        return NULL;
}
//----创建窗口-----------------------------------------------------------------
//功能: 创建一个窗口，新创建的窗口只是一个矩形区域，没有填充，也没有边框等一切
//      装饰。
//参数: parent，父窗口指针
//      newwin，新创建的窗口，保存数据结构所需内存由调用者提供
//      left、top、right、bottom，窗口的尺寸，相对于父窗口
//      color，创建窗口时填充的颜色
//      buf_mode，定义缓冲模式，参见 CN_WINBUF_PARENT 族常量定义
//      name，新窗口的名字，将copy到newwin中
//      PixelFormat,像素格式，CN_SYS_PF_DISPLAY表示与显示器相同，推荐。
//      HyalineColor,设定窗口透明色,当RopMode中使能了透明色时,须设置KeyColor
//      BaseColor, 灰度图基色，(仅在PixelFormat == CN_SYS_PF_GRAY1 ~8时有用)
//      RopMode, 混合码，参见 CN_ROP_ALPHA_SRC_MSK 族常量定义,0表示无特殊功能
//返回: true=创建成功，false=创建失败
//-----------------------------------------------------------------------------
struct GkWinObj * GK_CreateWin(struct GkWinObj *parent,
                         s32 left,s32 top,s32 right,s32 bottom,
                         u32 color,u32 buf_mode,
                         const char *name,u16 PixelFormat,u32 HyalineColor,
                         u32 BaseColor,struct RopGroup RopMode)
{
    struct GkscParaCreateGkwin para;
    struct GkWinObj *result;
    if(NULL == parent)
        return false;
    para.parent_gkwin = parent;
//  para.gkwin = newwin;
    para.result = &result;
    para.left = left;
    para.top = top;
    para.right = right;
    para.bottom = bottom;
    para.color = color;
    para.buf_mode = buf_mode;
    // para.name = (char*)name;
    strncpy(para.name, name, CN_GKWIN_NAME_LIMIT);
    para.name[CN_GKWIN_NAME_LIMIT] = '\0';
    para.PixelFormat = PixelFormat;
    para.HyalineColor = HyalineColor;
    para.BaseColor = BaseColor;
    para.RopCode = RopMode;
    __GK_SyscallChunnel(CN_GKSC_CREAT_GKWIN,CN_TIMEOUT_FOREVER,
                            &para,sizeof(para),NULL,0);
    if(*para.result == NULL)
        return NULL;
    else
    {
        //gui kernel创建窗口时,如果是buf窗口,则在创建的同时完成了填充,否则,
        //在这里执行填充.
        //非buf窗口不能再创建同时填充的原因是,填充非buf窗口是直接绘制在screen
        //或者framebuffer上的,而创建窗口时,剪切域尚未建立.直到调用 GK_SyncShow
        //后，可视域才能创建好。
//      if((result)->wm_bitmap == NULL)   //wm_bitmap==NULL 表示无窗口缓冲区。
//      {
//          GK_SyncShow(CN_TIMEOUT_FOREVER);
//          GK_FillWin(result,color,0);
//      }
        return result;
    }
}
//----异步填充窗口-------------------------------------------------------------
//功能: 用指定颜色填充一个窗口。
//参数: gkwin，待填充的窗口
//      color，填充颜色
//返回: 无
//-----------------------------------------------------------------------------
void GK_FillWin(struct GkWinObj *gkwin,u32 color,u32 SyncTime)
{
    struct GkscParaFillWin para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.color = color;
    __GK_SyscallChunnel(CN_GKSC_FILL_WIN,SyncTime,&para,sizeof(para),NULL,0);
    return;
}
//----刷新窗口-----------------------------------------------------------------
//功能: 执行积攒在通道中的命令, 并刷新到显示器。
//参数: SyncTime, 同步超时时间
//返回: 无
//-----------------------------------------------------------------------------
void GK_SyncShow(u32 SyncTime)
{
    __GK_SyscallChunnel(CN_GKSC_SYNC_SHOW,SyncTime,NULL,0,NULL,0);
    return;
}

//----重刷显示器---------------------------------------------------------------
//功能: 重新刷新显示器,不管是否有更新。GK_SyncShow只是刷新有改变的部分,
//      GK_RefreshDisplay 则是不管有没有改变,全部重刷,但只针对具体显示器。
//      在以下情况应该调用本函数:
//      1. 由镜像显示器驱动程序调用，镜像显示器插入时，必须调用。
//      2. 显示器本身从休眠醒来,且显示器内部没有缓冲.
//      3. 检测到新显示器插入.
//参数: Display, 被刷新的显示器
//返回: 无
//-----------------------------------------------------------------------------
void GK_RefreshDisplay(struct DisplayObj *Display)
{
    __GK_SyscallChunnel(CN_GKSC_DSP_REFRESH,CN_TIMEOUT_FOREVER,
                    &Display,sizeof(struct DisplayObj *),NULL,0);
    return;
}

#if 0
//----填充窗口的一部分---------------------------------------------------------
//功能: 用指定颜色填充一个窗口的一部分，颜色按alpha值渐变过渡
//参数: gkwin，待填充的目标窗口
//      rect，待填充的位置
//      AlphaL,AlphaR,AlphaT,AlphaB,左右上下的Alpha值
//      color，要填充颜色
//返回: 无
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

//----渐变色填充窗口的一部分---------------------------------------------------
//功能: 用指定颜色填充一个窗口的一部分，颜色按alpha值渐变过渡
//参数: gkwin，待填充的目标窗口
//      range，指定窗口的可绘制部分，超出部分将不予填充
//      rect，待填充的位置
//      Color0,颜色1，含义见 CN_FILLRECT_MODE_LR 族常量定义
//      Color1,颜色2，含义见 CN_FILLRECT_MODE_LR 族常量定义
//      Mode,渐变模式,含义见 CN_FILLRECT_MODE_LR 族常量定义
//返回: 无
//-----------------------------------------------------------------------------
void GK_FillRect(struct GkWinObj *gkwin,struct Rectangle *range,struct Rectangle *rect,
                            u32 Color0,u32 Color1,u32 Mode,s32 radius,u32 SyncTime)
{
    struct GkscParaGradientFillWin para;
    s32 flag = 0;
    if(NULL == gkwin)
        return;
    if(NULL != range)       //若给定了限定区，则判断之
    {
        para.range = *range;
    }
    else
    {
        para.range = (struct Rectangle){0,0,0,0};
    }
    if((rect->right)<(rect->left))
    {
        __gk_swap((rect->right),(rect->left));
        if((Mode!= 0)||(Mode != CN_FILLRECT_MODE_N))
            flag+=1;
    }
    if((rect->bottom)<(rect->top))
    {
        __gk_swap((rect->bottom),(rect->top));
        if((Mode!= 0)||(Mode != CN_FILLRECT_MODE_N))
            flag+=2;
    }
    if(flag == 0);
    else if(flag == 1)
    {
        if(Mode == CN_FILLRECT_MODE_LR)
        {
            __gk_swap(Color0,Color1);
        }
        else if(Mode ==CN_FILLRECT_MODE_LU2RD)
        {
            Mode = CN_FILLRECT_MODE_RU2LD;
        }
        else if(Mode ==CN_FILLRECT_MODE_RU2LD)
        {
            Mode = CN_FILLRECT_MODE_LU2RD;
        }
    }
    else if(flag == 2)
    {
        if(Mode == CN_FILLRECT_MODE_UD)
        {
            __gk_swap(Color0,Color1);
        }
        else if(Mode == CN_FILLRECT_MODE_LU2RD)
        {
            Mode = CN_FILLRECT_MODE_RU2LD;
            __gk_swap(Color0,Color1);
        }
        else if(Mode == CN_FILLRECT_MODE_RU2LD)
        {
            Mode = CN_FILLRECT_MODE_LU2RD;
            __gk_swap(Color0,Color1);
        }
    }
    else if(flag == 3)
        __gk_swap(Color0,Color1);

    para.gkwin = gkwin;
    para.rect = *rect;
    para.Color0 = Color0;
    para.Color1 = Color1;
    para.Mode = Mode;
    para.radius = radius;
    __GK_SyscallChunnel(CN_GKSC_FILL_RECT,SyncTime,&para,
                        sizeof(para),NULL,0);
    return;
}

//----显示文本-----------------------------------------------------------------
//功能: 用默认字体和默认字符集显示一个文本串。
//参数: gkwin，显示的目标窗口
//      pFont, 使用的字体,NULL表示用系统当前字体
//      pCharset, 使用的字符集,NULL表示用系统当前字符集
//      x、y，显示位置，相对于gkwin
//      text，待显示的字符串
//      count，字符串长度，含串结束符
//      color, 显示颜色，RGB888格式
//      Rop2Mode，二元光栅操作码，参见 CN_R2_BLACK 族常量定义
//返回: 无
//-----------------------------------------------------------------------------
void GK_DrawText(struct GkWinObj *gkwin,struct Rectangle *range,
                    struct FontObj *pFont,
                    struct Charset *pCharset,
                    s32 x,s32 y,
                    const char *text,s32 count,u32 color,
                    u32 Rop2Code,u32 SyncTime)
{
    u16 TextBytes;
    struct GkscParaDrawText para;
    if((NULL == gkwin) || (NULL == text) || (0 == count))
        return;
    if(NULL != range)       //若给定了限定区，则判断之
    {
        para.range = *range;
    }
    else
    {
        para.range = (struct Rectangle){0,0,0,0};
    }

    para.gkwin = gkwin;
    para.pFont = pFont;
    para.pCharset = pCharset;
    para.x = x;
    para.y = y;
    para.count = count;
    para.color = color;
    para.Rop2Code = Rop2Code;
    TextBytes = strlen(text);
    TextBytes += Charset_Get_EOC_Size(pCharset);
    __GK_SyscallChunnel(CN_GKSC_DRAW_TEXT,SyncTime,
                        &para,sizeof(para),(void*)text,TextBytes);
    return;
}
//----画点---------------------------------------------------------------------
//功能: 绘制像素。
//参数: gkwin，绘制的目标窗口
//      x、y，绘制的位置，相对于gkwin
//      color，给定的颜色
//      Rop2Mode，二元光栅操作码，参见 CN_R2_BLACK 族常量定义
//返回: 无
//-----------------------------------------------------------------------------
void GK_SetPixel(struct GkWinObj *gkwin,struct Rectangle *range,s32 x,s32 y,
                        u32 color,u32 Rop2Code,u32 SyncTime)
{
    struct GkscParaSetPixel para;
    struct Rectangle *wlimit = &gkwin->limit;
    if(NULL == gkwin)
        return;
    if(NULL != range)       //若给定了限定区，则判断之
    {
        if((x < range->left) || (x >= range->right)||(y < range->top) || (y >= range->bottom))
            return;
    }
    if( (x >= wlimit->left) && (x < wlimit->right)&&(y >= wlimit->top) && (y < wlimit->bottom) )
    {
        para.gkwin = gkwin;
        para.x = x;
        para.y = y;
        para.color = color;
        para.Rop2Code = Rop2Code;
        __GK_SyscallChunnel(CN_GKSC_SET_PIXEL,SyncTime,
                            &para,sizeof(para),NULL,0);
    }
    return;
}
//----画单像素宽直线，不包含结束端点-------------------------------------------
//功能: 绘制直线，不包括结束端点
//参数: gkwin，绘制的目标窗口
//      x1、y1、x2、y2,要绘制的直线的起、终点坐标
//      color，绘制的颜色
//      Rop2Mode，二元光栅操作码，参见 CN_R2_BLACK 族常量定义
//返回: 无
//-----------------------------------------------------------------------------
void GK_Lineto(struct GkWinObj *gkwin,struct Rectangle *range, s32 x1,s32 y1,
                    s32 x2,s32 y2,u32 color,u32 Rop2Code,u32 SyncTime)
{
    struct GkscParaLineto para;
    if(NULL == gkwin)
        return;
    if(NULL != range)       //若给定了限定区，则判断之
    {
        para.range = *range;
    }
    else
    {
        para.range = (struct Rectangle){0,0,0,0};
    }

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
//----画单像素宽直线，包含结束端点---------------------------------------------
//功能: 绘制直线，包括结束端点
//参数: gkwin，绘制的目标窗口
//      x1、y1、x2、y2,要绘制的直线的起、终点坐标
//      color，绘制的颜色
//      Rop2Mode，二元光栅操作码，参见 CN_R2_BLACK 族常量定义
//返回: 无
//-----------------------------------------------------------------------------
//void GK_LinetoIe(struct GkWinObj *gkwin, s32 x1,s32 y1,
//                    s32 x2,s32 y2,u32 color,u32 Rop2Code,u32 SyncTime)
//{
//    struct GkscParaLineto para;
//    if(NULL == gkwin)
//        return;
//    para.gkwin = gkwin;
//    para.x1 = x1;
//    para.y1 = y1;
//    para.x2 = x2;
//    para.y2 = y2;
//    para.color = color;
//    para.Rop2Code = Rop2Code;
//    __GK_SyscallChunnel(CN_GKSC_LINETO_INC_END,SyncTime,
//                        &para,sizeof(para),NULL,0);
//    return;
//}

//----画光栅位图---------------------------------------------------------------
//功能: 绘制bitmap，带光栅操作。
//参数: gkwin，绘制的目标窗口
//      bitmap，待绘制的位图
//      pattern，要设置的图案位图
//      Rop4Msk，rop4操作才需要的掩码位图，一个单色位图
//      HyalineColor，透明色
//      x、y，绘制的目标位置
//      RopMode, 光栅码，参见 CN_ROP_ALPHA_SRC_MSK 族常量定义
//返回: 无
//-----------------------------------------------------------------------------
void GK_DrawBitMap(struct GkWinObj *gkwin,struct Rectangle *range,
                    struct RectBitmap *bitmap,s32 x,s32 y,
                    u32 HyalineColor,struct RopGroup RopCode,u32 SyncTime)
{
    struct GkscParaDrawBitmapRop para;
    memset(&para,0,sizeof(para));
    if(NULL == gkwin)
        return;
    else
        para.gkwin = gkwin;
    if(NULL == bitmap)
        return;
    else
        para.bitmap = *bitmap;
    if(NULL != range)       //若给定了限定区，则判断之
    {
        para.range = *range;
    }
    else
    {
        para.range = (struct Rectangle){0,0,0,0};
    }

    para.HyalineColor = HyalineColor;
    para.x = x;
    para.y = y;
    para.RopCode = RopCode;
    __GK_SyscallChunnel(CN_GKSC_DRAW_BITMAP_ROP,SyncTime,
                        &para,sizeof(para),NULL,0);
    return;
}

//----画单像素宽的圆-----------------------------------------------------------
//功能: 画单像素宽的圆。
//参数: gkwin，绘制的目标窗口
//      x0、y0，圆的中心点坐标，相对于窗口
//      r，圆的半径
//      color，画圆使用的颜色
//      Rop2Mode，二元光栅操作码，参见 CN_R2_BLACK 族常量定义
//返回: 无
//-----------------------------------------------------------------------------
void GK_DrawCircle(struct GkWinObj *gkwin,struct Rectangle *range,s32 x0,s32 y0,
                    u32 r,u32 color,u32 Rop2Code,u32 SyncTime)
{
    struct GkscParaDrawCircle para;
    if(NULL == gkwin)
        return;
    if(NULL != range)       //若给定了限定区，则判断之
    {
        para.range = *range;
    }
    else
    {
        para.range = (struct Rectangle){0,0,0,0};
    }
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
//----画Bezier曲线-------------------------------------------------------------
//功能: 画Bezier曲线。
//参数: gkwin，绘制的目标窗口
//      x1、y1、x2、y2、x3、y3、x4、y4，绘制Bezier曲线的四个控制点
//      color，绘制的颜色
//      Rop2Mode，二元光栅操作码，参见 CN_R2_BLACK 族常量定义
//返回: 无
//-----------------------------------------------------------------------------
void GK_DrawBezier(struct GkWinObj *gkwin,struct Rectangle *range,float x1,float y1,
                    float x2,float y2,float x3,float y3,float x4,float y4,
                    u32 color,u32 Rop2Code,u32 SyncTime)
{
    struct GkscParaBezier para;
    if(NULL == gkwin)
        return;
    if(NULL != range)       //若给定了限定区，则判断之
    {
        para.range = *range;
    }
    else
    {
        para.range = (struct Rectangle){0,0,0,0};
    }
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

//----根据坐标取窗口-----------------------------------------------------------
//功能: 根据传入的坐标，获取所在窗口，必须在该窗口的可视域中。必须从桌面所属显示
//     器的 最前端窗口开始往后查找，因为对于透明窗口来说，被其覆盖的窗口，被覆盖
//     部分也是可见的。
//参数: desktop，目标桌面
//      pt，目标坐标点
//返回: 无
//-----------------------------------------------------------------------------
struct GkWinObj* GK_GetWinFromPt(struct GkWinObj *desktop,
                                    const struct PointCdn *pt)
{
    struct GkWinObj *current;
    struct GkWinObj *result=NULL;
    struct ClipRect *VisibleClip,*VisibleHead;

    if((NULL == desktop) || (NULL == pt))
        return NULL;
    current = desktop->disp->z_topmost;
    while(current != desktop)
    {
        if(current->WinProperty.Zprio > CN_ZPRIO_CTRL)
        {
            VisibleClip = current->visible_clip;
            VisibleHead = VisibleClip;
            while(VisibleClip != NULL)
            {
                if(GDD_PtInRect(&VisibleClip->rect,pt))
                {
                    result = current;
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

//---取末端窗口--------------------------------------------------------------
//功能: 取当前窗口的一个末端后代窗口，即没有子窗口的窗口。
//参数: Ancestor，祖先窗口
//      Current，当前遍历位置
//返回: 下一个后代窗口
//-----------------------------------------------------------------------------
struct GkWinObj* GK_GetTwig(struct GkWinObj *Ancestor)
{
    struct Object *result;

    if(NULL == Ancestor)
        return NULL;
    result = OBJ_GetTwig(Ancestor->HostObj);
    return (struct GkWinObj*)OBJ_GetPrivate(result);
}

//---遍历后代窗口--------------------------------------------------------------
//功能: 根据当前位置，返回下一个后代窗口，反复调用直到返回NULL。
//参数: Ancestor，祖先窗口
//      Current，当前遍历位置
//返回: 下一个后代窗口
//-----------------------------------------------------------------------------
struct GkWinObj* GK_TraveScion(struct GkWinObj *Ancestor,struct GkWinObj *Current)
{
    struct Object *result;

    if((NULL == Ancestor) || (NULL == Current))
        return NULL;
    result = OBJ_ForeachScion(Ancestor->HostObj, Current->HostObj);
    return (struct GkWinObj*)OBJ_GetPrivate(result);
}

//---遍历子窗口--------------------------------------------------------------
//功能: 根据当前位置，返回下一个子窗口，反复调用直到返回NULL。
//参数: Parent，父窗口
//      Current，当前遍历位置
//返回: 下一个子窗口
//-----------------------------------------------------------------------------
struct GkWinObj* GK_TraveChild(struct GkWinObj *Parent,struct GkWinObj *Current)
{
    struct Object *result;

    if((NULL == Parent) || (NULL == Current))
        return NULL;
    result = OBJ_ForeachChild(Parent->HostObj, Current->HostObj);
    return (struct GkWinObj*)OBJ_GetPrivate(result);
}

//----过继窗口-----------------------------------------------------------------
//功能：把一个窗口从其父窗口中移出，变成其他窗口的子窗口。窗口的其他属性不变。
//参数：gkwin，目标窗口
//      NewParent，新的父窗口
//返回: 无
//-----------------------------------------------------------------------------
void GK_AdoptWin(struct GkWinObj *gkwin, struct GkWinObj *NewParent)
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

//----移动窗口-----------------------------------------------------------------
//功能: 移动窗口。
//参数: gkwin，目标窗口
//      left、top，窗口移动后的左上角坐标，相对于父窗口
//返回: 无
//-----------------------------------------------------------------------------
void GK_MoveWin(struct GkWinObj *gkwin, s32 left,s32 top)
{
    struct GkscParaMoveWin para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.left = left;
    para.top = top;
    __GK_SyscallChunnel(CN_GKSC_MOVE_WIN, CN_TIMEOUT_FOREVER,
                        &para,sizeof(para),NULL,0);
    return;
}

//----改变窗口位置尺寸---------------------------------------------------------
//功能: 改变窗口的位置和尺寸，所有坐标均相对于父窗口
//参数: gkwin，目标窗口
//      left、top、right、bottom，窗口的新坐标，相对于父窗口
//返回: 无
//-----------------------------------------------------------------------------
void GK_ChangeWinArea(struct GkWinObj *gkwin, s32 left,s32 top,
                                       s32 right,s32 bottom)
{
    struct GkscParaChangeWinArea para;
    if(NULL == gkwin)
        return;
    para.gkwin = gkwin;
    para.left = left;
    para.top = top;
    para.right = right;
    para.bottom = bottom;
    __GK_SyscallChunnel(CN_GKSC_CHANGE_WIN_AREA,CN_TIMEOUT_FOREVER,
                        &para,sizeof(para),NULL,0);
    return;
}

//----设置窗口的边界模式-------------------------------------------------------
//功能: 设置窗口的边界模式。
//参数: gkwin，目标窗口
//      mode， 窗口的边界模式，CN_BOUND_LIMIT 为受限，CN_BOUND_UNLIMIT为不受限
//返回: 无
//-----------------------------------------------------------------------------
void GK_SetBoundMode(struct GkWinObj *gkwin, u32 mode)
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
//----设置窗口显示优先级-------------------------------------------------------
//功能: 设置窗口显示优先级。即使新prio与元prio相等，也可能影响显示，它会把gkwin
//      提到所有prio相同的兄弟窗口前端。
//参数: gkwin，目标窗口
//      prio，新优先级。
//      sync_time，绘制图形时间同步，在此时间内不管是否绘制完成都返回
//返回: 无
//-----------------------------------------------------------------------------
void GK_SetPrio(struct GkWinObj *gkwin, s32 prio,u32 SyncTime)
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

//----设置窗口是否可视-------------------------------------------------------
//功能: 设置窗口的可视属性
//参数: gkwin，目标窗口
//      visible，CN_GKWIN_VISIBLE=可视，CN_GKWIN_HIDE=隐藏
//      sync_time，绘制图形时间同步，在此时间内不管是否绘制完成都返回
//返回: 无
//注意：若 sync_time != 无穷，则随后调用 GK_IsWinVisible 可能得到原来的状态
//-----------------------------------------------------------------------------
void GK_SetVisible(struct GkWinObj *gkwin, u32 visible,u32 SyncTime)
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

//----设置窗口的光栅属性-------------------------------------------------------
//功能: 设置窗口的光栅属性。
//参数: gkwin，目标窗口
//      Rop4Msk,掩码位图
//      RopMode, 混合码，参见 CN_ROP_ALPHA_SRC_MSK 族常量定义，不支持二元光栅混合
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t GK_SetRopCode(struct GkWinObj *gkwin,
                        struct RopGroup RopCode,u32 SyncTime)
{
    struct GkscParaSetRopCode para;
    memset(&para,0,sizeof(para));
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

//----设置窗口透明色-----------------------------------------------------------
//功能: 设置窗口的透明色。
//参数: gkwin，目标窗口
//      KeyColor，要设置的透明色
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t GK_SetHyalineColor(struct GkWinObj *gkwin,u32 HyalineColor)
{
    struct GkscParaSetHyalineColor para;
    if(NULL == gkwin)
        return false;
    if((HyalineColor == gkwin->HyalineColor) && (1 == gkwin->RopCode.HyalineEn))
        return true;
    para.gkwin = gkwin;
    para.HyalineColor = HyalineColor;
    __GK_SyscallChunnel(CN_GKSC_SET_HYALINE_COLOR,CN_TIMEOUT_FOREVER,
                        &para,sizeof(para),NULL,0);
    return true;
}

//----设置窗口直接写屏属性-----------------------------------------------------
//功能: 设置窗口直接写屏属性。
//参数: gkwin，目标窗口
//返回: 无
//-----------------------------------------------------------------------------
void GK_SetDirectScreen(struct GkWinObj *gkwin,u32 SyncTime)
{
    if(NULL == gkwin)
        return;
    __GK_SyscallChunnel(CN_GKSC_SET_DIRECT_SCREEN,SyncTime,
                        &gkwin,sizeof(struct GkWinObj *),NULL,0);
    return;
}
//----撤销窗口直接写屏属性-----------------------------------------------------
//功能: 撤销窗口直接写屏属性。
//参数: gkwin，目标窗口
//返回: 无
//-----------------------------------------------------------------------------
void GK_SetUnDirectScreen(struct GkWinObj *gkwin,u32 SyncTime)
{
    if(NULL == gkwin)
        return;
    __GK_SyscallChunnel(CN_GKSC_SET_UNDIRECT_SCREEN,SyncTime,
                        &gkwin,sizeof(struct GkWinObj *),NULL,0);
    return;
}
//----销毁窗口-----------------------------------------------------------------
//功能: 释放窗口所占用的资源。
//参数: gkwin，目标窗口
//返回: 无
//-----------------------------------------------------------------------------
void GK_DestroyWin(struct GkWinObj *gkwin)
{
    if(NULL == gkwin)
        return;
    __GK_SyscallChunnel(CN_GKSC_DESTROY_WIN,CN_TIMEOUT_FOREVER,
                        &gkwin,sizeof(struct GkWinObj *),NULL,0);
    return;
}

//----查询显卡格式-------------------------------------------------------------
//功能: 查询显卡使用的颜色格式，画位图时，如果使用跟显卡相同的颜色格式，将获得
//      最优性能
//参数: display，被查询的显卡
//返回: 颜色格式
//-----------------------------------------------------------------------------
u16 GK_GetPixelFormat(struct DisplayObj *display)
{
    if(display != NULL)
        return display->pixel_format;
    else
        return 0;
}

//----取gkwin的用户数据--------------------------------------------------------
//功能: 略
//参数: gkwin，被查询gkwin
//返回: 用户数据
//-----------------------------------------------------------------------------
void *GK_GetUserTag(struct GkWinObj *gkwin)
{
    if(gkwin != NULL)
        return gkwin->UserTag;
    else
        return NULL;
}

//----设置gkwin的用户数据------------------------------------------------------
//功能: 略
//参数: gkwin，被设置gkwin
//返回: 无
//-----------------------------------------------------------------------------
void GK_SetUserTag(struct GkWinObj *gkwin,void *Tag)
{
    if(gkwin != NULL)
        gkwin->UserTag = Tag;
    return;
}

//----取父窗口指针------------------------------------------------------
//功能: 略
//参数: gkwin，父窗口
//返回: 子窗口指针
//-----------------------------------------------------------------------------
struct GkWinObj *GK_GetParentWin(struct GkWinObj *gkwin)
{
    if(gkwin != NULL)
        return (struct GkWinObj*)OBJ_GetPrivate(OBJ_GetParent(gkwin->HostObj));
    else
        return NULL;
}
//----取子窗口指针------------------------------------------------------
//功能: 略，实际跟 GK_GetFirstWin 等效
//参数: gkwin，父窗口
//返回: 子窗口指针
//-----------------------------------------------------------------------------
struct GkWinObj *GK_GetChildWin(struct GkWinObj *gkwin)
{
    if(gkwin != NULL)
        return (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetChild(gkwin->HostObj));
    else
        return NULL;
}

//----取前窗口指针------------------------------------------------------
//功能: 略
//参数: gkwin，窗口
//返回: 前窗口指针
//-----------------------------------------------------------------------------
struct GkWinObj *GK_GetPreviousWin(struct GkWinObj *gkwin)
{
    if(gkwin != NULL)
        return (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetPrev(gkwin->HostObj));
    else
        return NULL;
}

//----取后窗口指针------------------------------------------------------
//功能: 略
//参数: gkwin，窗口
//返回: 后窗口指针
//-----------------------------------------------------------------------------
struct GkWinObj *GK_GetNextWin(struct GkWinObj *gkwin)
{
    if(gkwin != NULL)
        return (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetNext(gkwin->HostObj));
    else
        return NULL;
}

//----取最前窗口指针------------------------------------------------------
//功能: 取同组窗口的最前端窗口指针，注意，不是全部窗口的Z序最前端。
//参数: gkwin，窗口
//返回: 最前窗口指针
//-----------------------------------------------------------------------------
struct GkWinObj *GK_GetFirstWin(struct GkWinObj *gkwin)
{
    if(gkwin != NULL)
        return (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetHead(gkwin->HostObj));
    else
        return NULL;
}

//----取最后窗口指针------------------------------------------------------
//功能: 取同组窗口的最后端窗口指针，注意，不是全部窗口的Z序最后端。
//参数: gkwin，窗口
//返回: 最后窗口指针
//-----------------------------------------------------------------------------
struct GkWinObj *GK_GetLastWin(struct GkWinObj *gkwin)
{
    if(gkwin != NULL)
        return (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetPrev(OBJ_GetHead(gkwin->HostObj)));
    else
        return NULL;
}

//----取窗口区域-----------------------------------------------------------
//功能: 取窗口区域在显示器上的坐标，不判定是否可见
//参数: gkwin，窗口
//      rc，用于返回结果的矩形结构
//返回: 显示区域,显示器的绝对坐标
//-----------------------------------------------------------------------------
void GK_GetScreenArea(struct GkWinObj *gkwin, struct Rectangle *rc)
{
    if((gkwin != NULL) && (rc != NULL))
    {
        rc->left = gkwin->ScreenX;
        rc->top = gkwin->ScreenY;
        rc->right = gkwin->ScreenX + gkwin->area.right - gkwin->area.left;
        rc->bottom = gkwin->ScreenY + gkwin->area.bottom - gkwin->area.top;
    }
    return ;
}

//----取窗口区域-----------------------------------------------------------
//功能: 取窗口区域在父窗口上的坐标，不判定是否可见
//参数: gkwin，窗口
//      rc，用于返回结果的矩形结构
//返回: 显示区域,显示器的绝对坐标
//-----------------------------------------------------------------------------
void GK_GetLcArea(struct GkWinObj *gkwin, struct Rectangle *rc)
{
    if((gkwin != NULL) && (rc != NULL))
    {
        *rc = gkwin->area;
    }
    return ;
}

//----取窗口尺寸-----------------------------------------------------------
//功能: 取窗口尺寸
//参数: gkwin，窗口
//      size，用于返回尺寸的坐标
//返回: 显示区域,显示器的绝对坐标
//-----------------------------------------------------------------------------
void GK_GetSize(struct GkWinObj *gkwin, struct PointCdn *size)
{
    if((gkwin != NULL) && (size != NULL))
    {
        size->x = gkwin->area.right - gkwin->area.left;
        size->y = gkwin->area.bottom - gkwin->area.top;
    }
    return ;
}

//----取窗口相对父窗口位置-----------------------------------------------------------
//功能: 取窗口在父窗口坐标系下的位置，如果是桌面窗口，则取屏幕坐标系
//参数: gkwin，窗口
//      lc，用于返回结果
//返回: 显示区域,显示器的绝对坐标
//-----------------------------------------------------------------------------
void GK_GetLocation(struct GkWinObj *gkwin, struct PointCdn *lc)
{
    if((gkwin != NULL) && (lc != NULL))
    {
        lc->x = gkwin->area.left;
        lc->y = gkwin->area.top;
    }
    return ;
}

//----取窗口相对screen位置-----------------------------------------------------------
//功能: 取窗口在屏幕坐标系下的位置
//参数: gkwin，窗口
//      lc，用于返回结果
//返回: 显示区域,显示器的绝对坐标
//-----------------------------------------------------------------------------
void GK_GetScreenLocation(struct GkWinObj *gkwin, struct PointCdn *lc)
{
    if((gkwin != NULL) && (lc != NULL))
    {
        lc->x = gkwin->ScreenX;
        lc->y = gkwin->ScreenY;
    }
    return ;
}

//----取窗口所在显示器尺寸---------------------------------------------------
//功能: 取窗口尺寸
//参数: gkwin，窗口
//      size，用于返回尺寸的坐标
//返回: 显示区域,显示器的绝对坐标
//-----------------------------------------------------------------------------
void GK_GetDspSize(struct GkWinObj *gkwin, struct PointCdn *size)
{
    if((gkwin != NULL) && (size != NULL))
    {
        size->x = gkwin->disp->width;
        size->y = gkwin->disp->height;
    }
    return ;
}

//----窗口改名-----------------------------------------------------------------
//功能: 修改窗口名字，窗口最多127字符，超过部分将截断。
//参数: gcwin，被修改的窗口句柄(指针)
//      name，新的名字
//返回: 无
//-----------------------------------------------------------------------------
void GK_SetName(struct GkWinObj *gkwin,const char *name)
{
    s32 size;
    size = strnlen(name,CN_GKWIN_NAME_LIMIT+1);
    if(size > CN_GKWIN_NAME_LIMIT)     //名字长度超过限制
        size = CN_GKWIN_NAME_LIMIT;   //强制切掉超长部分
    memcpy(gkwin->win_name,name,size);//copy名字，因可能超长，故未copy串结束符\0。
    gkwin->win_name[size] = '\0';     //串封口(加结束符)
}

//----取窗口名-----------------------------------------------------------
//功能: 略
//参数: gkwin，窗口
//返回: 显示区域
//-----------------------------------------------------------------------------
char *GK_GetName(struct GkWinObj *gkwin)
{
    if(gkwin != NULL)
    {
        return gkwin->win_name;
    }
    else
        return NULL;
}

//----判定窗口是否可见---------------------------------------------------------
//功能: 略
//参数: gkwin，目标窗口
//返回: true=可见（有可视域），false=不可见
//-----------------------------------------------------------------------------
bool_t GK_IsWinVisible(struct GkWinObj *gkwin)
{
    if(NULL == gkwin)
        return false;
    if(gkwin->WinProperty.VisibleExec == CN_GKWIN_HIDE)
        return false;
    else
        return true;
}

extern struct GkChunnel g_tGkChunnel;

//------------------------------------------------------------------------------
//功能：读取一条guikernel传递给（ 例如gdd）的消息
//参数：Request，接收消息的缓冲区
//      bufsize，缓冲区尺寸
//      timeout，超时时间，uS数
//返回：成功获取返回命令码（消息的第一个16位数），没有命令则返回  CN_GKUC_NULL
//------------------------------------------------------------------------------
u32 GK_ReadRequest(u8 *Request, u16 bufsize, u32 timeout)
{
    u16 id,size;

    if(bufsize > CN_USERCALL_MSG_SIZE)
        size = CN_USERCALL_MSG_SIZE;
    else
        size = bufsize;

    if(MsgQ_Receive(g_tGkChunnel.usercall_msgq,Request,size,timeout))
    {
//      id   = *Request + *((Request+1)<<8);
//      size = Request[1];
//      Request = &Request[4];
        id = *(u16*)Request;
        return id;
    }
    else
        return CN_GKUC_NULL;
}

