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
//�ļ�����: gui core����ʾ�����֣�������Ӳ����ز���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-11-21
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#ifndef __GK_DISPLAY_H__
#define __GK_DISPLAY_H__
#include "stdint.h"
#include "gkernel.h"
#ifdef __cplusplus
extern "C" {
#endif
//���Ʋ������Ĵ���:
//1����bitmap�ϻ���
//2����screen�ϻ���
//3��ȡbitmap�ϵ����ػ�bitmap
//4��ȡscreen�ϵ����ػ�bitmap��
//��bitmap�ϣ������ء�����ʱ���Ǵ�rop2������ģ�Ӳ������:��֧�֡�����֧�֡�ȫ֧��
//bitmap�������ε�ʱ�򣬲���rop2�����룬Ӳ�����ٿ���֧�֣�Ҳ���ܲ�֧��
//���ϼ�������������ܸ�ֵΪNULL����������֧�ֵ�ʵ��֮����֧�ֵķ���false����gui
//kernelʵ��֮�������Ļ���Ҳ������check_raster�����ˡ�

//gui kernelȷ��������Ա�����Ĳ�������Ϊ��ͼĿ���bitmap��������win buffer����
//frame buffer�е�bitmap�������û������ṩ��bitmap��
//����Ϊͼ��Դ��bitmap���������CN_SYS_PF_xxx�е�����һ�֣�������CN_CUSTOM_PF

//----�����������,�ṹ�ڵ���ʾ��������ʵ�ֹ���:
//1.��ʾ��ʹ��CN_CUSTOM_PF��ʽ��gkernel����֪����ô���ơ�
//2.�Կ���Ӳ�����ٹ��ܣ��Ҹ�Ӳ���������ܹ�֧����Ҫ��Ļ�������,Ӳ�����ٹ��ܱ�
//  ����driver������,gkernel�ǲ�֪����ô�����ض��Կ���Ӳ�����ٹ��ܵ�.
//3.�Ż�����,��ʹ����CN_CUSTOM_PF��ʽ,Ҳû��Ӳ������,������ʵ�ֱ�����,ִ��Ч��
//  ���ܱ���gkernel��ʵ��Ҫ�ߵö�,��Ϊgkernel��Ҫ���Ǹ����������,������ж�,
//  ����driverʵ��,ֻ��Ҫ���Ǹ���ʾ�����ض����.
//----���������ֱ�ӷ���false,�������ܽ���gkernelʵ��:
//1.������д�ĳ���,�ļ������ȿ�һ���ܷ���ʾ
//2.��Ȼ��Ӳ�����٣����ü��ٹ��ܲ�֧����Ҫ��Ļ�������.
//3.��ִ��Ч������ν
//----������Ҫ��Ļ��Ʋ���,Ӳ��������ֻ֧�ֲ��ֵ����,�����ں������������ж�,
//    ʵ����֧�ֵĹ���,��֧�ֵĹ����򷵻�false,��gkernelʵ��
//�������:������ʾ��,��ҪԶ�̽��浫����ȴû����ʾ�������,�����ڱ��������һ��
//��ʾ��,������ʾ��driver�й�ȫ������ͼ��screen�ĺ���,������ʵ��,����ͬǰ������

//�������������ʾ�������2������SetPixelToScreen,CopyBitmapToScreen������ʵ��
//һ�������Ӧ�ò�ʹ��ֱ��д��������framebuffer������ʵ��CopyBitmapToScreen���ɡ�
struct DispDraw
{
    //�ھ���λͼ�л�һ������
    //ʲô�����Ӧ��ʵ�ֱ������Ĺ���,�μ����ṹǰ���ע��
    //������ʾ������ʵ�ֱ�����,ֱ�ӷ���false����
    //����:
    // bitmap: ���Ƶ�Ŀ��λͼ
    // x,y: Ŀ������,�����bitmap��.
    // color: ��ʽ��cn_sys_pf_e8r8g8b8
    // r2_code: ��ʾЧ��,���gkernel.h�е�CN_R2_BLACK�峣������
    bool_t (*SetPixelToBitmap)(struct RectBitmap *bitmap,
                         s32 x,s32 y,u32 color,u32 r2_code);

    //�ھ���λͼ�л�һ����.�����,�������յ�
    //ʲô�����Ӧ��ʵ�ֱ������Ĺ���,�μ����ṹǰ���ע��
    //������ʾ������ʵ�ֱ�����,ֱ�ӷ���false����
    //����:
    // bitmap: ���Ƶ�Ŀ��λͼ
    // limit: ������ֻ������limit��Χ�ڵĲ��ֲű�����,limitʹ����ʾ���еľ�������,
    //       ����bitmap�е��������.limit�������������ϱ߽�,�������Һ��±߽�.
    // x1,y1: �������,����
    // x2,y2: �����յ�,������
    // color: ��ʽ��cn_sys_pf_e8r8g8b8
    // r2_code: ��ʾЧ��,���gkernel.h�е�CN_R2_BLACK�峣������
    bool_t (*LineToBitmap)(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);

    //�ھ���λͼ�л�һ����.���������յ�
    //ʲô�����Ӧ��ʵ�ֱ������Ĺ���,�μ����ṹǰ���ע��
    //������ʾ������ʵ�ֱ�����,ֱ�ӷ���false����
    //����:
    // bitmap: ���Ƶ�Ŀ��λͼ
    // limit: ������ֻ������limit��Χ�ڵĲ��ֲű�����,limitʹ����ʾ���еľ�������,
    //       ����bitmap�е��������.limit�������������ϱ߽�,�������Һ��±߽�.
    // x1,y1: �������,����
    // x2,y2: �����յ�,����
    // color: ��ʽ��cn_sys_pf_e8r8g8b8
    // r2_code: ��ʾЧ��,���gkernel.h�е�CN_R2_BLACK�峣������
    bool_t (*LineToBitmapIe)(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);

    //Bitmap�о������,֧�ֽ���ɫ
    //ʲô�����Ӧ��ʵ�ֱ������Ĺ���,�μ����ṹǰ���ע��
    //������ʾ������ʵ�ֱ�����,ֱ�ӷ���false����
    //����:
    // DstBitmap: ���Ƶ�Ŀ��λͼ
    // Target: Ŀ���������,����ɫ���ʱ������������������ɫ�������յ�
    // Focus: ʵ���������,gkernelȷ������Target�����ڲ�
    // Color0,Color1: Target���ϽǺ����½���ɫֵ,������ǽ������,Color1��������,
    //              ���ظ�ʽ��CN_SYS_PF_ERGB8888
    //Mode: ���ģʽ,CN_FILLRECT_MODE_N�峣��
    bool_t (*FillRectToBitmap)(    struct RectBitmap *DstBitmap,
                               struct Rectangle *Target,
                               struct Rectangle *Focus,
                               u32 Color0,u32 Color1,u32 Mode);

    //����������λͼ��λ�鴫��,������γߴ粻���,��Ҫʵ�������ѹ��
    //ʲô�����Ӧ��ʵ�ֱ������Ĺ���,�μ����ṹǰ���ע��
    //������ʾ������ʵ�ֱ�����,ֱ�ӷ���false����
    //����:
    // dst_bitmap: Ŀ��λͼ
    // DstRect: Ŀ�����
    // src_bitmap: Դλͼ
    // SrcRect: Դ����
    // RopCode: ��ʾЧ��,�μ� gkernel.h�ļ��� CN_ROP_ALPHA_SRC_MSK �峣����˵��
    // HyalineColor: ͸��ɫ,�������RopCode���趨�� CN_ROP_KEYCOLOR_EN,��
    //                  ������ָ��͸��ɫ,CN_SYS_PF_ERGB8888��ʽ
    bool_t (*BltBitmapToBitmap)( struct RectBitmap *dst_bitmap,
                            struct Rectangle *DstRect,
                            struct RectBitmap *src_bitmap,
                            struct Rectangle *SrcRect,
                            struct RopGroup RopCode,u32 HyalineColor);

    //��������screen�л�ͼ�ĺ���
    //��screen�л�һ�����أ���frame buffer������£�������ʾgui����������������
    //�������direct_screen==true������ܵ��ñ������������Ƿ���frame buffer��
    //driver�������ṩ���ұ���ʵ�ֱ�����
    //������ʾ������ʵ�ֱ������Ĺ���,����ֱ�ӷ���false
    //����:
    // x,y: ��������.
    // color: ��ɫֵ,CN_SYS_PF_ERGB8888��ʽ
    // r2_code: ��ʾЧ��,���gkernel.h�е�CN_R2_BLACK�峣������
    bool_t (*SetPixelToScreen)(s32 x,s32 y,u32 color,u32 r2_code);

    //��screen�л�һ�����أ���frame buffer������£�������ʾgui����������������
    //�������direct_screen==true������ܵ��ñ������������Ƿ���frame buffer��
    //driver�������ṩ���ұ���ʵ�ֱ�����
    //������ʾ������ʵ�ֱ������Ĺ���,����ֱ�ӷ���false
    //����:
    // PixelGroup: ������������ָ��.
    // color: ��ɫֵ,CN_SYS_PF_ERGB8888��ʽ
    // n: ����������
    // r2_code: ��ʾЧ��,���gkernel.h�е�CN_R2_BLACK�峣������
    bool_t (*SetPixelGroupToScreen)(struct PointCdn *PixelGroup,u32 color,u32 n, u32 r2_code);

    //��screen�л�һ������ֱ�ߣ�����㲻���յ�.
    //ʲô�����Ӧ��ʵ�ֱ������Ĺ���,�μ����ṹǰ���ע��
    //������ʾ������ʵ�ֱ������Ĺ���,����ֱ�ӷ���false
    //����:
    // limit: ���߷�Χ,����������,ֻ�������䷶Χ�ڵĲ���Ч,�����ϱ߽�,�������±߽�.
    // x1,y1: �������,����
    // x2,y2: �����յ�,������
    // color: ��ɫֵ,CN_SYS_PF_ERGB8888��ʽ
    // r2_code: ��ʾЧ��,���gkernel.h�е�CN_R2_BLACK�峣������
    bool_t (*LineToScreen)(struct Rectangle *limit,
                          s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);

    //��screen�л�һ������ֱ�ߣ����������յ�.
    //ʲô�����Ӧ��ʵ�ֱ������Ĺ���,�μ����ṹǰ���ע��
    //������ʾ������ʵ�ֱ������Ĺ���,����ֱ�ӷ���false
    //����:
    // limit: ���߷�Χ,����������,ֻ�������䷶Χ�ڵĲ���Ч,�����ϱ߽�,�������±߽�.
    // x1,y1: �������,����
    // x2,y2: �����յ�,����
    // color: ��ɫֵ,CN_SYS_PF_ERGB8888��ʽ
    // r2_code: ��ʾЧ��,���gkernel.h�е�CN_R2_BLACK�峣������
    bool_t (*LineToScreenIe)(struct Rectangle *limit,
                             s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);

    //screen�о������.
    //ʲô�����Ӧ��ʵ�ֱ������Ĺ���,�μ����ṹǰ���ע��
    //������ʾ�����Բ�ʵ�ֱ������Ĺ���,ֱ�ӷ���false,gkernel������ SetPixelToScreen
    //ʵ��,�����ô˺���Ч�ʼ���,ǿ�ҽ��龵����ʾ��ʵ�ֱ�����
    //����:
    // Target: Ŀ���������,����ɫ���ʱ������������������ɫ�������յ�
    // Focus: ʵ���������,gkernelȷ������Target�����ڲ�
    // Color0,Color1: Target���ϽǺ����½���ɫֵ,������ǽ������,Color1��������,
    //              ���ظ�ʽ��CN_SYS_PF_ERGB8888
    // Mode: ���ģʽ,CN_FILLRECT_MODE_N�峣��
    bool_t (*FillRectToScreen)(struct Rectangle *Target,
                               struct Rectangle *Focus,
                               u32 Color0,u32 Color1,u32 Mode);

    //���ڴ滺������screenλ�鴫�ͣ�ֻ֧�ֿ鿽������֧��rop��Ч,Ҳ��֧��ѹ�������졣
    //ʲô�����Ӧ��ʵ�ֱ������Ĺ���,�μ����ṹǰ���ע��
    //������ʾ������ʵ�ֱ������Ĺ���,����ֱ�ӷ���false
    //����:
    // dst_rect: screen�ϵ�Ŀ������,�����˷�Χ�Ĳ����.�����ϱ߽�,���������±߽�
    // src_bitmap: ��copy��Դλͼ
    // xsrc,ysrc: Դλͼ�б����͵���ʼ����.
    bool_t (*CopyBitmapToScreen)(struct Rectangle *dst_rect,
                           struct RectBitmap *src_bitmap,s32 xsrc,s32 ysrc);

    //��screen��ȡһ���أ���ת����CN_SYS_PF_ERGB8888
    // ��ʾ��driver��Ҫʵ�ֱ�����.
    //������ʾ������ʵ�ֱ�����,ֱ�ӷ���false����
    //����:
    // x,y: ����ȡ����������
    u32 (*GetPixelFromScreen)(s32 x,s32 y);

    //��screen�ھ�����������ݸ��Ƶ�bitmap������ǰ�������ú�dest��pf_type
    //ʲô�����Ӧ��ʵ�ֱ������Ĺ���,�μ����ṹǰ���ע��,ǿ�ҽ���,���ڸ���ʾ����
    // struct DisplayRsc�ṹ��Աframebuf_direct==true�������,�ſ��Ƿ���false,
    // ��gkernelȥʵ��.
    //������ʾ������ʵ�ֱ�����,ֱ�ӷ���false����
    //����:
    //rect: screen�д����Ƶ�����
    //dest: ����λͼ��λͼ�ṹָ��,�䳤��ߴ�Ӧ�� �� rect.
    bool_t (*GetRectFromScreen)(struct Rectangle *rect,struct RectBitmap *dest);
};

struct DisplayObj
{
    struct Object  *HostObj;            //ע:������ʾ����Ϊ����Դ����������
    u32 width_um,height_um;             //��΢�ױ�ʾ����ʾ���ߴ磬���4294�ס�
    s32   width,height;                 //�����ر�ʾ����ʾ���ߴ�
    u32   pixel_format;                 //���ظ�ʽ����gk_win.h�ж���
    bool_t reset_clip;                  //true = ����ʾ�������˿�����
    //֡����ֱ����ʾ����Щcpuʹ��ϵͳ�ڴ����Դ棬���driverʱ���ɽ��Դ�ֱ����Ϊ
    //֡�����á�true=�Դ���֡�����غ�
    bool_t framebuf_direct;
    struct GkWinObj  *frame_buffer;  //����ʾ����֡���壬�Դ��ڹ���
    struct GkWinObj  *desktop;       //����ʾ��������
    struct GkWinObj *z_topmost;      //z����ǰ�˴���ָ��

    //A�������ϵͳ���Ϸ��䴰�ڻ��壬DisplayHeap����ΪNULL��
    //B�����Ҫ��ר�õ��Դ�Heap�Ϸ��䴰�ڻ��棬�Կ�������ʹָ��DisplayHeapָ���
    //   Heap
    struct HeapCB *DisplayHeap;

    struct DispDraw draw;            //��ʾ������

    //������ʾ��
    //������driver�ṩ��һ��Ӧ�ó������ڣ���Ӧ�ó��������ṩһ�����ӻ��ķ�ʽ
    //�趨����ʾ�����п������û��趨�Ĳ���������ֱ��ʺͺ�ɫ�ʲ����������Ĺ���
    //����ͳһ�涨������������ĵ�Ӧ���ṩ������ʹ��˵����
    //���ñ������������ṩ����windows��������ʾ�����ԵĹ��ܡ�
    bool_t (*disp_ctrl)(struct DisplayObj *disp);
};

bool_t GK_InstallDisplay(struct DisplayObj *display,const char *name);
bool_t GK_InstallDisplayMirror(struct DisplayObj *base_display,
                                 struct DisplayObj *mirror_display,char *name);
bool_t GK_SetDefaultDisplay(const char *name);
struct GkWinObj *GK_GetRootWin(struct DisplayObj *display);
bool_t GK_SwitchFrameBuffer(struct DisplayObj *display,
                              struct RectBitmap *fbuf);
struct RectBitmap *GK_CreateFrameBuffer(struct DisplayObj *display);

#ifdef __cplusplus
}
#endif

#endif //__GK_DISPLAY_H__

