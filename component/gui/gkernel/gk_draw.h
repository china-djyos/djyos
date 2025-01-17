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
//文件描述: 本地像素格式定义及在缓冲区显示函数定义
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-10-26
//   作者:  罗侍田.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __GK_DRAW_H__
#define __GK_DRAW_H__
#include "gk_syscall.h"
#ifdef __cplusplus
extern "C" {
#endif

//直接写屏不支持rop操作
bool_t __GK_RopIsNeedDestination(struct RopGroup RopCode);
u32 __GK_PointSect(s32 x,s32 y,struct Rectangle *limit);
u32 __GK_OlineSectInter(struct Rectangle *limit,s32 *x1,s32 *y1,s32 *x2,s32 *y2);
void __GK_DrawOline(struct GkWinObj *gkwin,s32 x1,s32 y1,
                        s32 x2,s32 y2,u32 color,u32 rop2_code);
void __GK_GetRgbBm(u16 PixelFormat,u32 color,u32 *a,u32 *r,
                                            u32 *g,u32 *b,ptu32_t ExColor);
void __GK_CopyPixelRopBm(struct RectBitmap *dst_bitmap,
                        struct RectBitmap *src_bitmap,
                        s32 x_dst,s32 y_dst,s32 x_src,s32 y_src,
                        struct RopGroup RopCode,u32 HyalineColor);
void __GK_SetPixelRop2Bm(struct RectBitmap *bitmap,
                        s32 x,s32 y,u32 color,u32 rop2_code);
void __GK_CopyPixelBm(struct RectBitmap *dst_bitmap,
                                struct RectBitmap *src_bitmap,
                                s32 x_dst,s32 y_dst,s32 x_src,s32 y_src);
void __GK_ShadingPixel(struct GkWinObj *gkwin,s32 x,s32 y);
void __GK_ShadingRect(struct GkWinObj *gkwin,struct Rectangle *rect);
void __GK_ShadingLine(struct GkWinObj *gkwin,struct Rectangle *limit,
                     s32 x1,s32 y1,s32 x2,s32 y2);
void __GK_ShadingClear(struct GkWinObj *gkwin);
void __GK_VlinetoBm(struct GkWinObj *gkwin,struct Rectangle *limit,
                    s32 x1,s32 y1,s32 y2,u32 color,u32 rop2_code);
void __GK_HlinetoBm(struct GkWinObj *gkwin,struct Rectangle *limit,
                    s32 x1,s32 y1,s32 x2,u32 color,u32 rop2_code);
void __GK_OlinetoBm(struct GkWinObj *gkwin,struct Rectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code);
void __GK_LinetoBm(struct GkWinObj *gkwin,struct Rectangle *limit,
                s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code);
void __GK_SetPixelScreen(struct DisplayObj *display,s32 x,s32 y,
                        u32 color,u32 rop2_code);
void __GK_LinetoScreen(struct DisplayObj *display,struct Rectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code);
void __GK_BltBmToScreen(struct DisplayObj *display,struct Rectangle *rect,
                        struct RectBitmap *bitmap,s32 x,s32 y);
void __GK_FillroundRect(struct RectBitmap *dst_bitmap,
                     struct Rectangle *Target,struct Rectangle *Focus,u32 color,s32 radius);
void __GK_FillRect(struct RectBitmap *dst_bitmap,
                     struct Rectangle *dst_rect,u32 color);
void __GK_FillBm(struct RectBitmap *dst_bitmap,u32 color);
void __GK_SetPixelsCircle(struct GkWinObj *gkwin,struct Rectangle *limit,u32 r,
                                struct RectBitmap *bitmap,s32 x0,s32 y0,
                                s32 x,s32 y,u32 color,u32 rop2_code);
void __GK_DrawCircleBm(struct GkWinObj *gkwin,struct Rectangle *limit,
                        s32 x0,s32 y0,s32 r,u32 color,u32 rop2_code);
void __GK_DrawCircleScreen(struct DisplayObj *display,struct Rectangle *limit,
                        s32 x0,s32 y0,s32 r,u32 color,u32 rop2_code);
void __GK_DrawCircle(struct GkscParaDrawCircle *para);
void __GK_BezierBm(struct GkWinObj *gkwin,struct Rectangle *limit,
                    float x1,float y1,float x2,float y2,float x3,float y3,
                    float x4,float y4,u32 color,u32 rop2_code);
void __GK_BezierScreen(struct DisplayObj *display,struct Rectangle *limit,
                    float x1,float y1,float x2,float y2,float x3,float y3,
                    float x4,float y4,u32 color,u32 rop2_code);

void __GK_SetPixel(struct GkscParaSetPixel *para);
void __GK_Bezier(struct GkscParaBezier *para);
void __GK_Lineto(struct GkscParaLineto *para);
//void __GK_LinetoIe(struct GkscParaLineto *para);
void __GK_DrawText(struct GkscParaDrawText *para,const char *text,u32 *Bytes);
void __GK_DrawBitMap(struct GkscParaDrawBitmapRop *para);
void __GK_GradientFillRect(struct GkscParaGradientFillWin *para);
void __GK_FillWin(struct GkscParaFillWin *para);

#ifdef __cplusplus
}
#endif

#endif /*__GK_DRAW_H__*/

