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

//����ģ��: gui
//����:  ������.
//�汾��V1.0.0
//�ļ�����: �������ظ�ʽ���弰�ڻ�������ʾ��������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-10-26
//   ����:  ������.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __GK_DRAW_H__
#define __GK_DRAW_H__
#include "gk_syscall.h"
#ifdef __cplusplus
extern "C" {
#endif

//ֱ��д����֧��rop����
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
void __GK_LinetoIe(struct GkscParaLineto *para);
void __GK_DrawText(struct GkscParaDrawText *para,const char *text,u32 *Bytes);
void __GK_DrawBitMap(struct GkscParaDrawBitmapRop *para);
void __GK_GradientFillRect(struct GkscParaGradientFillWin *para);
void __GK_FillWin(struct GkscParaFillWin *para);

#ifdef __cplusplus
}
#endif

#endif /*__GK_DRAW_H__*/

