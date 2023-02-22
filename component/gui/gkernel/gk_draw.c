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
//�ļ�����: ����Ļ������صĲ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2010-03-30
//   ����:  ������.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "align.h"
#include "stdlib.h"
#include "string.h"
#include "object.h"
#include "gkernel.h"
#include <gui/gk_display.h>
#include "gk_win.h"
#include "gk_draw.h"
#include "gk_clip.h"
#include "font.h"

#define __gk_swap(a, b){a^=b; b^=a; a^=b;}

#ifndef MAX
#define MAX(x,y)    (x>y?x:y)
#endif

#ifndef MIN
#define MIN(x,y)    (x<y?x:y)
#endif

//----��դ�������1------------------------------------------------------------
//����: ���ָ���Ĺ�դ�������Ƿ���ҪĿ��λͼ�μ����㡣
//����: rop_code����դ������
//����: 1=��Ҫ��0=����Ҫ
//ע: �ж��߼����ʽ(((fore_code & CN_ROP_D)>>1) == (fore_code & ~CN_ROP_D))��
//    ���壬̫����ע���޷�д�£���ο��������׵��顣
//-----------------------------------------------------------------------------
u32 __GK_RopIsNeedDestination(struct RopGroup RopCode)
{
    u8 rop2;
    if((RopCode.AlphaMode & CN_ALPHA_MODE_NEED_DST) || (RopCode.HyalineEn == 1))
        return CN_GKWIN_DEST_VISIBLE;
    else
    {
        rop2 = RopCode.Rop2Mode;
        if((rop2 == CN_R2_BLACK) || (rop2 == CN_R2_NOTCOPYEN)
           || (rop2 == CN_R2_COPYPEN) || (rop2 == CN_R2_MERGEPEN) )
            return CN_GKWIN_DEST_HIDE;
        else
            return CN_GKWIN_DEST_VISIBLE;
    }
}

//----���ظ�ʽת����rgb24------------------------------------------------------
//����: ��ɫ��ʽת������һ�����ظ�ʽ��ɫת����ERGB8888�����λͼ��ʽ�������
//      alphaͨ�������ȼ���alpha��
//����: PixelFormat�����ظ�ʽ
//      color��Դɫ��
//      ExColor,˫���ܲ��������PixelFormat=CN_SYS_PF_GRAY1~8������PixelFormat=
//          CN_SYS_PF_ALPHA1~8����ExColor��ʾ��ɫ��
//          ���PixelFormat=CN_SYS_PF_PALETTE1,��ExColor��ʾ��ɫ��ָ�롣
//          ���PixelFormat����Ҫ�˲������ɸ�0��NULL.
//����: ���ɫ��ʽ
//-----------------------------------------------------------------------------
u32 GK_ConvertColorToRGB24(u16 PixelFormat,u32 color,ptu32_t ExColor)
{
    u32 a,r,g,b;
    switch(PixelFormat)
    {
        case CN_SYS_PF_RGB565:
        {
            r = (color>>8) & 0xf8;
            g = (color>>3) & 0xfc;
            b = (color<<3) & 0xf8;
        }break;
        case CN_SYS_PF_RGB332:
        {
            r = color & 0xe0;
            g = (color<<3) & 0xe0;
            b = (color<<6) & 0xc0;
        }break;
        case CN_SYS_PF_RGBE4444:
        {
            r = (color>>8) & 0xf0;
            g = (color>>4) & 0xf0;
            b = color & 0xf0;
        }break;
        case CN_SYS_PF_ERGB4444:
        {
            r = (color>>4) & 0xf0;
            g = color & 0xf0;
            b = (color<<4) & 0xf0;
        }break;
        //5551��ʽ�ܱ�Ť����ʱ���ε�������Ҫʱ�ٸ���ʵ�ʸ�ʽ���ӡ�
//        case CN_SYS_PF_RGBC5551:
//        {
//            u32 c = (color & 1)<<2;
//            r = ((color>>8) & 0xf8) + c;
//            g = ((color>>3) & 0xf8) + c;
//            b = ((color<<2) & 0xfc);
//        }break;
        case CN_SYS_PF_ERGB6666:
        {
            r = (color>>10) & 0xfc;
            g = (color>>4) & 0xfc;
            b = (color<<2) & 0xfc;
        }break;
        case CN_SYS_PF_E12RGB666:
        {
            r = (color>>10) & 0xfc;
            g = (color>>4) & 0xfc;
            b = (color<<2) & 0xfc;
        }break;
        case CN_SYS_PF_RGB101010:
        {
            r = (color>>22) & 0xff;
            g = (color>>12) & 0xff;
            b = (color>>2) & 0xff;
        }break;
        case CN_SYS_PF_RGB888:
        {
            return color;
        }break;
        case CN_SYS_PF_ERGB8888:
        {
            return color;
        }break;
        case CN_SYS_PF_ARGB1555:
        {
            if(color & 0x8000)
            {
                r = (color>>7) & 0xf8;
                g = (color>>2) & 0xf8;
                b = (color<<3) & 0xf8;
            }
            else
            {
                return 0;
            }
        }break;
        case CN_SYS_PF_RGBA5551:
        {
            if(color & 1)
            {
                r = (color>>8) & 0xf8;
                g = (color>>3) & 0xf8;
                b = (color<<2) & 0xf8;
            }
            else
            {
                return 0;
            }
        }break;
        case CN_SYS_PF_ARGB8888:
        {
            a = (color>>24) & 0xff;
            r = ((color>>16) & 0xff) *a / 255;
            g = ((color>>8) & 0xff) *a / 255;
            b = ((color>>0) & 0xff) *a / 255;
        }break;
        case CN_SYS_PF_GRAY1:
        case CN_SYS_PF_ALPHA1:
        {
            return color * ExColor;
        }break;

        case CN_SYS_PF_GRAY2:
        case CN_SYS_PF_ALPHA2:
        {
            r = (((ExColor>>16) & 0xff) * color) / 3;
            g = (((ExColor>>8) & 0xff) * color) / 3;
            b = ((ExColor & 0xff) * color) / 3;
        }break;
        case CN_SYS_PF_GRAY4:
        case CN_SYS_PF_ALPHA4:
        {
            r = (((ExColor>>16) & 0xff) * color) / 15;
            g = (((ExColor>>8) & 0xff) * color) / 15;
            b = ((ExColor & 0xff) * color) / 15;
        }break;
        case CN_SYS_PF_GRAY8:
        case CN_SYS_PF_ALPHA8:
        {
            r = (((ExColor>>16) & 0xff) * color) / 255;
            g = (((ExColor>>8) & 0xff) * color) / 255;
            b = ((ExColor & 0xff) * color) / 255;
        }break;
        case CN_SYS_PF_PALETTE1:
        {
            return ((u32*)ExColor)[color];
        }break;
        case CN_SYS_PF_PALETTE2:
        {
            return ((u32*)ExColor)[color];
        }break;
        case CN_SYS_PF_PALETTE4:
        {
            return ((u32*)ExColor)[color];
        }break;
        case CN_SYS_PF_PALETTE8:
        {
            return ((u32*)ExColor)[color];
        }break;
        default:
        {
            return color;
        }break;
    }
    return (r<<16) | (g<<8) | b;
}

//----rgb24ת��������----------------------------------------------------------
//����: ��һ��ERGB8888��ʽ����ɫת���ɱ��ظ�ʽ�����ظ�ʽ��������alphaͨ����Ҳ��
//      �����ǵ�ɫ��λͼ��
//����: PixelFormat��Ŀ���ʽ
//      color��Դɫ��
//����: PixelFormat��ʽ��ɫ��
//ע: ��֧�ֵ�ɫ���ʽ��ֱ�ӷ���color
//    �Ҷ�ͼ���㷨������color��HLSɫ��ģ�͵����ȡ�
//-----------------------------------------------------------------------------
u32 GK_ConvertRGB24ToPF(u16 PixelFormat,u32 color)
{
    u32 r,g,b;
    //ת�����̲���Ҫargb�ֽ����ӵ�ת�����ڴ�switch��ɣ������Ч��
    switch(PixelFormat)
    {
        case CN_SYS_PF_RGB888:
        case CN_SYS_PF_ERGB8888:
        case CN_SYS_PF_ARGB8888:
        case CN_SYS_PF_ARGB1555:
        case CN_SYS_PF_RGBA5551:
        //����4����ʽ���������������ڸ�ʽ��ʵ����Ҳ�������������ΪĿ��ı任
        case CN_SYS_PF_PALETTE1:
        case CN_SYS_PF_PALETTE2:
        case CN_SYS_PF_PALETTE4:
        case CN_SYS_PF_PALETTE8:
        {
            return color;
        }break;
        default: break;
    }
    r = (color>>16) &0xff;
    g = (color>>8) & 0xff;
    b = color & 0xff;
    switch(PixelFormat)
    {
        case CN_SYS_PF_RGB565:
        {
            return ((r>>3)<<11) | ((g>>2)<<5) | (b>>3);
        }break;
        case CN_SYS_PF_RGB332:
        {
            return ((r>>5)<<5) | ((g>>5)<<2) | (b>>6);
        }break;
        case CN_SYS_PF_RGBE4444:
        {
            return ((r>>4)<<12) | ((g>>4)<<8) | ((b>>4)<<4);
        }break;
        case CN_SYS_PF_ERGB4444:
        {
            return ((r>>4)<<8) | ((g>>4)<<4) | (b>>4);
        }break;
        //5551��ʽ�ܱ�Ť����ʱ���ε�������Ҫʱ�ٸ���ʵ�ʸ�ʽ���ӡ�
//        case CN_SYS_PF_RGBC5551:    //ʵ���϶��������λ
//        {
//            return ((r>>3)<<11) | ((g>>3)<<6) | ((b>>3)<<1);
//        }break;
        case CN_SYS_PF_ERGB6666:
        {
            return ((r>>2)<<12) | ((g>>2)<<6) | (b>>2);
        }break;
        case CN_SYS_PF_E12RGB666:
        {
            return ((r>>2)<<12) | ((g>>2)<<6) | (b>>2);
        }break;
        case CN_SYS_PF_RGB101010:
        {
            return (r<<20) | (g<<10) | b;
        }break;
        case CN_SYS_PF_GRAY1:
        case CN_SYS_PF_ALPHA1:
        {
            //����rgbֵ����hlsģ�͵�����
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*1) + 255 )/(2*255);
        }break;
        case CN_SYS_PF_GRAY2:
        case CN_SYS_PF_ALPHA2:
        {
            //����rgbֵ����hlsģ�͵�����
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*3) + 255 )/(2*255);
        }break;
        case CN_SYS_PF_GRAY4:
        case CN_SYS_PF_ALPHA4:
        {
            //����rgbֵ����hlsģ�͵�����
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*15) + 255 )/(2*255);
        }break;
        case CN_SYS_PF_GRAY8:
        case CN_SYS_PF_ALPHA8:
        {
            //����rgbֵ����hlsģ�͵�����
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*255) + 255 )/(2*255);
        }break;
        default:
        {
            return color;
        }
        break;
    }
    return color;
}
//----������ɫ��ʽ����໥ת��-------------------------------------------------
//����: ������ɫ��ʽ����໥ת����Ŀ���ʽ��������alphaͨ����Ҳ�������ǵ�ɫ��λ
//      ͼ�����Դ��ʽ��alphaͨ���Ļ�������ɫ��ֵ��˺���ת����
//����: dpf_type��Ŀ���ʽ
//      spf_type��Դ��ʽ
//      color��Դɫ�ʣ�Դpf_type��ʽ��
//      ExColor,Դɫ�ʵ�˫���ܲ��������spf_type=CN_SYS_PF_GRAY1~8������spf_type=
//          CN_SYS_PF_ALPHA1~8����ExColor��ʾ��ɫ��
//      ���spf_type=CN_SYS_PF_PALETTE1,��ExColor��ʾ��ɫ��ָ�롣
//����: dpf_type��ʽ��ɫ��
//-----------------------------------------------------------------------------
u32 GK_ConvertPF2PF(u16 dpf_type,u16 spf_type,u32 color,ptu32_t ExColor)
{
    u32 result;
    //Դ��ʽ��Ŀ���ʽ��ͬ��ֱ�ӷ���Դɫ��
    if(spf_type == dpf_type)
        return color;
    //��Դɫ��ת�������ɫ����alphaͨ���Ļ�������֮��
    result = GK_ConvertColorToRGB24(spf_type,color,ExColor);
    //�����ɫת����Ŀ���ʽ����ɫ
    result = GK_ConvertRGB24ToPF(dpf_type,result);
    return result;
}
//----rop2���-----------------------------------------------------------------
//����: ����rop2ɫ�ʻ�ϡ�
//����: dest��������ɫ
//      PenColor��������ɫ
//      rop2_code����Ԫ��դ������
//����: ��Ϻ��ɫ��
//-----------------------------------------------------------------------------
u32 GK_BlendRop2(u32 dest,u32 PenColor,u32 Rop2Code)
{
    switch(Rop2Code)
    {
        case CN_R2_BLACK:         // ��Ϻ����ɫΪ��ɫ(black)
        {
            return CN_COLOR_BLACK;
        }break;
        case CN_R2_NOTMERGEPEN:   //��Ϻ����ɫΪ~(dest | PenColor)
        {
            return ~(dest | PenColor);
        }break;
        case CN_R2_MASKNOTPEN:    //��Ϻ����ɫΪ~PenColor & dest
        {
            return ~PenColor & dest;
        }break;
        case CN_R2_NOTCOPYEN:    //��Ϻ����ɫΪ~PenColor
        {
            return ~PenColor;
        }break;
        case CN_R2_MASKPENNOT:    //��Ϻ����ɫΪ~dest & PenColor
        {
            return ~dest & PenColor;
        }break;
        case CN_R2_NOT:           //��Ϻ����ɫΪ~dest
        {
            return ~dest;
        }break;
        case CN_R2_XORPEN :        //��Ϻ����ɫΪdest ^ PenColor
        {
            return dest ^ PenColor;
        }break;
        case CN_R2_NOTMASKPEN:    //��Ϻ����ɫΪ~(dest & PenColor)
        {
            return ~(dest & PenColor);
        }break;
        case CN_R2_MASKPEN:       //��Ϻ����ɫΪdest & PenColor
        {
            return dest & PenColor;
        }break;
        case CN_R2_NOTXORPEN:     //��Ϻ����ɫΪ~(dest ^ PenColor)
        {
            return ~(dest ^ PenColor);
        }break;
        case CN_R2_NOP:           //��Ϻ����ɫΪdest��ԭɫ��
        {
            return dest;
        }break;
        case CN_R2_MERGENOTPEN:   //��Ϻ����ɫΪ~PenColor | dest
        {
            return ~PenColor | dest;
        }break;
        case CN_R2_COPYPEN:       //��Ϻ����ɫΪpen������ɫ��
        {
            return PenColor;
        }break;
        case CN_R2_MERGEPENNOT:   //��Ϻ����ɫΪ~dest | PenColor
        {
            return ~dest | PenColor;
        }break;
        case CN_R2_MERGEPEN:      //��Ϻ����ɫΪdest | PenColor
        {
            return dest | PenColor;
        }break;
        case CN_R2_WHITE:         //��Ϻ����ɫΪ��ɫ(white)
        {
            return CN_COLOR_WHITE ;
        }break;
        default:
        {
            return PenColor;
            break;
        }
    }
    return 0;
}

#define CN_AREA_IN      0
#define CN_AREA_LEFT    1
#define CN_AREA_RIGHT   2
#define CN_AREA_TOP     4
#define CN_AREA_BOTTOM  8
//----�ж�Ŀ����λ��-----------------------------------------------------
//����: �ж�Ŀ������ڵ����򣬷ֳɾ�������ڲ������ҡ��ϡ����岿�֡�
//����: x��y��Ŀ���,��Դ��ڵ�����
//      limit��������
//����: ��־��c��c = 0��ʾĿ����ھ������ڣ������ھ�������
//-----------------------------------------------------------------------------
u32 __GK_PointSect(s32 x,s32 y,struct Rectangle *limit)  //ȷ��
{
    s32 c;
    c = CN_AREA_IN;

//  limit->bottom--;
//  limit->right--;
    if(x < limit->left)                 //�ж�Ŀ����Ƿ��ھ�������
        c |= CN_AREA_LEFT;
    else
    {
        if(x > limit->right)            //�ж�Ŀ����Ƿ��ھ������ҷ�
            c |= CN_AREA_RIGHT;
    }
    if(y < limit->top)                  //�ж�Ŀ����Ƿ��ھ������Ϸ�
        c |= CN_AREA_TOP;
    else
    {
        if(y > limit->bottom)           //�ж�Ŀ����Ƿ��ھ������·�
            c |= CN_AREA_BOTTOM;
    }
    return c;
}

//----��ȡб���ھ����ڲ�����-------------------------------------------------
//����: ��ȡֱ���ھ����ڲ����֡�
//����: limit��������
//      x1��y1��x2��y2�����յ�,��Դ��ڵ�����
//����: ��־��flag��flag = 0��ʾб�߲���limit�ڣ�flag = 1��ʾб�߲��ֻ�ȫ����limit�ڡ�
//      ��� flag != 0,���� x1��y1��x2��y2 ���ؽ�������
//˵��: �˴���ֱ�߶������б�߶ε�
//-----------------------------------------------------------------------------
u32 __GK_OlineSectInter(struct Rectangle *limit,s32 *x1,s32 *y1,s32 *x2,s32 *y2)
{
    s32 c1,c2,i=0;
    u32 flag = 1;
    struct Rectangle lmt = *limit;
    lmt.bottom--;
    lmt.right--;
    c1 = __GK_PointSect(*x1,*y1,&lmt);
    c2 = __GK_PointSect(*x2,*y2,&lmt);

    if((c1 == CN_AREA_IN)&&(c2 == CN_AREA_IN))      //����б����limit��
        return flag;
    else if((c1&c2) != 0)           //б����limit�⣬�����߶���limit��һ���߽���
    {
        flag = 0;
        return flag;
    }
    else
    {
        if(c1 == 0)                 //��(x1,y1)��limit�ڣ���(x2,y2)�϶�����
        {
            do
            {
                if((c2&CN_AREA_LEFT) != 0)
                {
                    *y2 = *y1+(*y2-*y1)*(lmt.left-*x1)/(*x2-*x1);
                    *x2 = lmt.left;
                    c2 = __GK_PointSect(*x2,*y2,&lmt);
                }
                else if((c2&CN_AREA_RIGHT) != 0)
                {
                    *y2 = *y1+(*y2-*y1)*(lmt.right-*x1)/(*x2-*x1);
                    *x2 = lmt.right;
                    c2 = __GK_PointSect(*x2,*y2,&lmt);
                }
                else if((c2&CN_AREA_TOP) != 0)
                {
                    *x2 = *x1+(*x2-*x1)*(lmt.top-*y1)/(*y2-*y1);
                    *y2 = lmt.top;
                    c2 = __GK_PointSect(*x2,*y2,&lmt);
                }
                else
                {
                    *x2 = *x1+(*x2-*x1)*(lmt.bottom-*y1)/(*y2-*y1);
                    *y2 = lmt.bottom;
                    c2 = __GK_PointSect(*x2,*y2,&lmt);
                }
            }while(c2 != CN_AREA_IN);
            return flag;
        }
        else                    //��(x1,y1)����limit�ڣ���(x2,y2)������limit��
        {                       //Ҳ������limit��
            do{
                if((c1&CN_AREA_LEFT) != 0)
                {
                    *y1 = *y1+(*y2-*y1)*(lmt.left-*x1)/(*x2-*x1);
                    *x1 = lmt.left;
                    c1 = __GK_PointSect(*x1,*y1,&lmt);
                    i++;
                }
                else if((c1&CN_AREA_RIGHT) != 0)
                {
                    *y1 = *y1+(*y2-*y1)*(lmt.right-*x1)/(*x2-*x1);
                    *x1 = lmt.right;
                    c1 = __GK_PointSect(*x1,*y1,&lmt);
                    i++;
                }
                else if((c1&CN_AREA_TOP) != 0)
                {
                    *x1 = *x1+(*x2-*x1)*(lmt.top-*y1)/(*y2-*y1);
                    *y1 = lmt.top;
                    c1 = __GK_PointSect(*x1,*y1,&lmt);
                    i++;
                }
                else
                {
                    if((c1&CN_AREA_BOTTOM) != 0)
                    {
                        *x1 = *x1+(*x2-*x1)*(lmt.bottom-*y1)/(*y2-*y1);
                        *y1 = lmt.bottom;
                        c1 = __GK_PointSect(*x1,*y1,&lmt);
                        i++;
                    }
                }
                //ֱ�ߺ�limit�н���ʱ��һ���˵���������ν���
                if(i>2)
                {
                    flag = 0;
                    return flag;
                }
            }while(c1 != CN_AREA_IN);
            if(c2 == CN_AREA_IN)
                return flag;
            else
            {
                do
                {
                    if((c2&CN_AREA_LEFT) != 0)
                    {
                        *y2 = *y1+(*y2-*y1)*(lmt.left-*x1)/(*x2-*x1);
                        *x2 = lmt.left;
                        c2 = __GK_PointSect(*x2,*y2,&lmt);
                    }
                    else if((c2&CN_AREA_RIGHT) != 0)
                    {
                        *y2 = *y1+(*y2-*y1)*(lmt.right-*x1)/(*x2-*x1);
                        *x2 = lmt.right;
                        c2 = __GK_PointSect(*x2,*y2,&lmt);
                    }
                    else if((c2&CN_AREA_TOP) != 0)
                    {
                        *x2 = *x1+(*x2-*x1)*(lmt.top-*y1)/(*y2-*y1);
                        *y2 = lmt.top;
                        c2 = __GK_PointSect(*x2,*y2,&lmt);
                    }
                    else
                    {
                        *x2 = *x1+(*x2-*x1)*(lmt.bottom-*y1)/(*y2-*y1);
                        *y2 = lmt.bottom;
                        c2 = __GK_PointSect(*x2,*y2,&lmt);
                    }
                }while(c2 != CN_AREA_IN);
                return flag;
            }
        }
    }
}

//----��ȡλͼ�е�����---------------------------------------------------------
//����: ��һ��bitmap��ȡ���أ���bitmap�����ظ�ʽ������δ���κ�ת����
//����: bitmap��Ŀ��λͼ
//      x��y��Ŀ�����ص�����
//����: ������ɫֵ,ֱ�Ӷ�ȡ��������ʽת������ʹ�Ҷ�ͼ���ɫ���ʽ��Ҳû��ת����
//      ��ʵɫ��
//-----------------------------------------------------------------------------
u32 GK_GetPixelBm(struct RectBitmap *bitmap, s32 x,s32 y)
{
    u8 color_bits,bit_offset;
    u32 color=0,byte_offset;
    if(bitmap == NULL)
        return 0;
    color_bits = (bitmap->PixelFormat & CN_PF_BITWIDE_MSK) >> 8;
    //����λͼ��ɫ��ʽ��ȡ����
    switch(color_bits)
    {
        case 1:
        {
            //���������Ӧ���ֽ�ƫ����
            byte_offset = y*bitmap->linebytes + x/8;
            //������������Ӧ�ֽ��е�λƫ��
            bit_offset = 7-x%8;     //���λ��ʾ����ߣ����λ���ұ�
            //ȡ������ɫ
            color = (u32)(bitmap->bm_bits[byte_offset]>>bit_offset)&0x1;
        }break;
        case 2:
        {
            byte_offset = y*bitmap->linebytes + x/4;
            bit_offset = (3-x%4)<<1;     //���λ��ʾ����ߣ����λ���ұ�
            color = (u32)(bitmap->bm_bits[byte_offset]>>bit_offset)&0x3;
        }break;
        case 4:
        {
            byte_offset = y*bitmap->linebytes + x/2;
            bit_offset = (1-x%2)<<2;     //���λ��ʾ����ߣ����λ���ұ�
            color = (u32)(bitmap->bm_bits[byte_offset]>>bit_offset)&0xf;
        }break;
        case 8:
        {
            byte_offset = y*bitmap->linebytes + x;
            color = (u32)bitmap->bm_bits[byte_offset];
        }break;
        case 12:
        {
            byte_offset = y*bitmap->linebytes + x*3/2;
            bit_offset = (x*12)%8;
            //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
            if(bit_offset == 0)
            {
                color = (u32)bitmap->bm_bits[byte_offset]+
                        (u32)((bitmap->bm_bits[byte_offset+1]&0xf)<<8);
            }
            else
            {
                color = (u32)((bitmap->bm_bits[byte_offset]&0xf0)>>4)+
                        (u32)(bitmap->bm_bits[byte_offset+1]<<4);
            }
        }break;
        case 16:
        {
            byte_offset = y*bitmap->linebytes + x*2;
            color = (u32)(bitmap->bm_bits[byte_offset]
                    |(bitmap->bm_bits[byte_offset+1]<<8));
        }break;
        case 24:
        {
            byte_offset = y*bitmap->linebytes + x*3;
            color = (u32)bitmap->bm_bits[byte_offset];
            color += (u32)bitmap->bm_bits[byte_offset+1]<<8;
            color += (u32)bitmap->bm_bits[byte_offset+2]<<16;
        }break;
        case 32:
        {
            byte_offset = y*bitmap->linebytes + x*4;
            color = (u32)(*(u32*)(&bitmap->bm_bits[byte_offset]));
        }break;
        default:
        break;
    }
    return color;
}
//----��ȡĿ����ɫ��argb����---------------------------------------------------
//����: ��ȡĿ����ɫ��argb�����������ظ�ʽ����Ҫ��λ������û��ת����ʡ�
//����: PixelFormat��Ŀ����ɫ��ɫ��ʽ
//      color��Ŀ����ɫ
//      a��r��g��b��Ŀ����ɫ��a��r��g��b����
//      ExColor,˫���ܲ��������PixelFormat=CN_SYS_PF_GRAY1~8������PixelFormat=
//          CN_SYS_PF_ALPHA1~8����ExColor��ʾ��ɫ��
//          ���PixelFormat=CN_SYS_PF_PALETTE1,��ExColor��ʾ��ɫ��ָ�롣
//          ���PixelFormat����Ҫ�˲������ɸ�0��NULL.
//����: �ޣ�
//ע: ����ǻҶȻ��ɫ���ʽ����ת��ΪRGB24��ȡ��������a(alpha)����Ϊ0
//-----------------------------------------------------------------------------
void __GK_GetRgbBm(u16 PixelFormat,u32 color,u32 *a,u32 *r,
                                            u32 *g,u32 *b,ptu32_t ExColor)
{
    u32 rgb32,lr,lg,lb;
    //����λͼ��ɫ��ʽ����Ŀ����ɫ�е�rgb����
    switch(PixelFormat)
    {
        case CN_SYS_PF_RGB565:
        {
            *r = (color>>11) & 0x1f;
            *g = (color>>5) & 0x3f;
            *b = color & 0x1f;
        }break;
        case CN_SYS_PF_RGB332:
        {
            *r = (color>>5) & 0x07;
            *g = (color<<2) & 0x07;
            *b = color & 0x03;
        }break;
        case CN_SYS_PF_RGBE4444:
        {
            *r = (color>>12) & 0x0f;
            *g = (color>>8) & 0x0f;
            *b = (color>>4) & 0x0f;
        }break;
        case CN_SYS_PF_ERGB4444:
        {
            *r = (color>>8) & 0x0f;
            *g = (color>>4) & 0x0f;
            *b = color & 0x0f;
        }break;
        //5551��ʽ�ܱ�Ť����ʱ���ε�������Ҫʱ�ٸ���ʵ�ʸ�ʽ���ӡ�
//        case CN_SYS_PF_RGBC5551:
//        {
//            u32 c;
//            c = (color & 1);
//            *r = (((color>>10) & 0x3e)+c);
//            *g = (((color>>5) & 0x3e)+c);
//            *b = color & 0x3f;
//        }break;
        case CN_SYS_PF_ERGB6666:
        {
            *r = (color>>12) & 0x3f;
            *g = (color>>6) & 0x3f;
            *b = color & 0x3f;
        }break;
        case CN_SYS_PF_E12RGB666:
        {
            *r = (color>>12) & 0x3f;
            *g = (color>>6) & 0x3f;
            *b = color & 0x3f;
        }break;
        case CN_SYS_PF_RGB101010:
        {
            *r = (color>>20) & 0x3ff;
            *g = (color>>10) & 0x3ff;
            *b = color & 0x3ff;
        }break;
        case CN_SYS_PF_RGB888:
        {
            *r = (color>>16) & 0xff;
            *g = (color>>8) & 0xff;
            *b = color & 0xff;
        }break;
        case CN_SYS_PF_ERGB8888:
        {
            *r = (color>>16) & 0xff;
            *g = (color>>8) & 0xff;
            *b = color & 0xff;
        }break;

        case CN_SYS_PF_ARGB1555:
        {
            *a = (color>>16) & 0x01;
            *r = (color>>10) & 0x1f;
            *g = (color>>5) & 0x1f;
            *b = color & 0x1f;
        }break;
        case CN_SYS_PF_RGBA5551:
        {
            *a = (color) & 0x01;
            *r = (color>>11) & 0x1f;
            *g = (color>>6) & 0x1f;
            *b = (color>>1) & 0x1f;
        }break;
        case CN_SYS_PF_ARGB8888:
        {
            *a = (color>>24) & 0xff;
            *r = (color>>16) & 0xff;
            *g = (color>>8) & 0xff;
            *b = color & 0xff;
        }break;
        case CN_SYS_PF_GRAY1:
        case CN_SYS_PF_ALPHA1:
        {
            rgb32 = color * ExColor;
            *r = (rgb32>>16) & 0xff;
            *g = (rgb32>>8) & 0xff;
            *b = rgb32 & 0xff;
        }break;
        case CN_SYS_PF_GRAY2:
        case CN_SYS_PF_ALPHA2:
        {
            lr = (ExColor>>16) & 0xff;
            lg = (ExColor>>8) & 0xff;
            lb = ExColor & 0xff;
            *r = (lr * color) / 3;
            *g = (lg * color) / 3;
            *b = (lb * color) / 3;
        }break;
        case CN_SYS_PF_GRAY4:
        case CN_SYS_PF_ALPHA4:
        {
            lr = (ExColor>>16) & 0xff;
            lg = (ExColor>>8) & 0xff;
            lb = ExColor & 0xff;
            *r = (lr * color) / 15;
            *g = (lg * color) / 15;
            *b = (lb * color) / 15;
        }break;
        case CN_SYS_PF_GRAY8:
        case CN_SYS_PF_ALPHA8:
        {
            lr = (ExColor>>16) & 0xff;
            lg = (ExColor>>8) & 0xff;
            lb = ExColor & 0xff;
            *r = (lr * color) / 255;
            *g = (lg * color) / 255;
            *b = (lb * color) / 255;
        }break;
        case CN_SYS_PF_PALETTE1:
        case CN_SYS_PF_PALETTE2:
        case CN_SYS_PF_PALETTE4:
        case CN_SYS_PF_PALETTE8:
        {
            lr = ((((u32*)ExColor)[color])>>16) & 0xff;
            lg = ((((u32*)ExColor)[color])>>8) & 0xff;
            lb = (((u32*)ExColor)[color]) & 0xff;
            *r = (lr * color) / 255;
            *g = (lg * color) / 255;
            *b = (lb * color) / 255;
        }break;
        default:
        {
            *a = 0;
            *r = 0;
            *g = 0;
            *b = 0;
        }
        break;
    }
}
//----����alpha���-------------------------------------------------------
//����: ������������alpha���ֵ,����color,RGB�ֱ����,ת����dst�����ظ�ʽ����.
//      dst�ĸ�ʽ�����ɫ�����ͣ���Ϻ󽫷���RGB24��ʽ��ֵ����Ϊ��Ϻ����ɫ��
//      �ܲ�����ɫ���С�
//      dst�ĸ�ʽ����ǻҶ�ͼ����Ϻ����ɫ�ټ������������
//����: SrcColor,DstColor������ϵ�ɫ�ʣ����ϲ��� PixelFormat ��ʽ
//      PixelFormat, ɫ�ʵĸ�ʽ
//      ExColor,˫���ܲ��������PixelFormat=CN_SYS_PF_GRAY1~8������PixelFormat=
//          CN_SYS_PF_ALPHA1~8����ExColor��ʾ��ɫ��
//          ���PixelFormat=CN_SYS_PF_PALETTE1,��ExColor��ʾ��ɫ��ָ�롣
//          ���PixelFormat����Ҫ�˲������ɸ�0��NULL.
//      Asrc��Adst��src �� dst alpha,0~255
//      AlphaMode,alpha���ģʽ���μ� CN_ALPHA_MODE_MSK ����ض���
//����: ��Ϻ����ɫ
//-----------------------------------------------------------------------------
u32 __GK_AlphaBlendBitmapToWin(u32 SrcColor,u32 DstColor, u16 PixelFormat,
                  ptu32_t dst_ExColor,u32 Asrc,u32 Adst,u32 AlphaMode)
{
    u32 a,r,g,b;
    u32 a_src=0,r_src,g_src,b_src;
    u32 a_dst=0,r_dst,g_dst,b_dst;
    u32 result;
    //��ȡĿ��λͼ��ɫ��rgb����
    __GK_GetRgbBm(PixelFormat,DstColor,&a_dst,&r_dst,&g_dst,&b_dst,dst_ExColor);
    //��ȡԴλͼ��ɫ��rgb����
    __GK_GetRgbBm(PixelFormat,SrcColor,&a_src,&r_src,&g_src,&b_src,dst_ExColor);
    //����Ŀ��λͼ��ɫ��ʽ��ȡ��Ϻ����ɫ
    switch(AlphaMode)
    {
        case CN_ALPHA_MODE_AsAdN:
        {
            a = a_src*Asrc/255+a_dst*(255-Asrc)*Adst/255;
            r = r_src*Asrc/255+r_dst*(255-Asrc)*Adst/255;
            g = g_src*Asrc/255+g_dst*(255-Asrc)*Adst/255;
            b = b_src*Asrc/255+b_dst*(255-Asrc)*Adst/255;
        }break;
        case CN_ALPHA_MODE_AsAd:
        {
            a = a_src*Asrc/255+a_dst*Adst/255;
            r = r_src*Asrc/255+r_dst*Adst/255;
            g = g_src*Asrc/255+g_dst*Adst/255;
            b = b_src*Asrc/255+b_dst*Adst/255;
        }break;
        case CN_ALPHA_MODE_As:
        {
            a = a_src*Asrc/255;
            r = r_src*Asrc/255;
            g = g_src*Asrc/255;
            b = b_src*Asrc/255;
        }break;
        case CN_ALPHA_MODE_AsN:
        default:
        {
            a = a_src*Asrc/255+a_dst*(255-Asrc)/255;
            r = r_src*Asrc/255+r_dst*(255-Asrc)/255;
            g = g_src*Asrc/255+g_dst*(255-Asrc)/255;
            b = b_src*Asrc/255+b_dst*(255-Asrc)/255;
        }break;
    }
    switch(PixelFormat)
    {
        case CN_SYS_PF_RGB565:
        {
            result = (r<<11) | (g<<5) | b;
        }break;
        case CN_SYS_PF_RGB332:
        {
            result = (r<<5) | (g<<2) | b;
        }break;
        case CN_SYS_PF_RGBE4444:
        {
            result = (r<<16) | (g<<8) | (b<<4);
        }break;
        case CN_SYS_PF_ERGB4444:
        {
            result = (r<<8) | (g<<4) | b;
        }break;
        //5551��ʽ�ܱ�Ť����ʱ���ε�������Ҫʱ�ٸ���ʵ�ʸ�ʽ���ӡ�
//        case CN_SYS_PF_RGBC5551:
//        {
//            u32 c;
//            c = r & 1;
//            r = r>>1;
//            g = g>>1;
//            b = b>>1;
//            color = (r<<11) | (g<<6) | (b<<1) | c;
//            return color;
//        }break;
        case CN_SYS_PF_ERGB6666:
        {
            result = (r<<12) | (g<<6) | b;
        }break;
        case CN_SYS_PF_E12RGB666:
        {
            result = (r<<12) | (g<<6) | b;
        }break;
        case CN_SYS_PF_RGB101010:
        {
            result = (r<<20) | (g<<10) | b;
        }break;
        case CN_SYS_PF_RGB888:
        case CN_SYS_PF_ERGB8888:
        {
            result = (r<<16) | (g<<8) | b;
        }break;
        case CN_SYS_PF_ARGB1555:
        {
            result = (a<<15) | (r<<10) | (g<<5) | b;
        }break;
        case CN_SYS_PF_RGBA5551:
        {
            result = (a &1) | (r<<11) | (g<<6) | (b<<1);
        }break;
        case CN_SYS_PF_ARGB8888:
        {
            result = (a<<24) | (r<<16) | (g<<8) | b;
       }break;
        case CN_SYS_PF_GRAY1:
        {
            //����rgbֵ����hlsģ�͵�����
            result = (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*1) + 255 )/(2*255);
        }break;
        case CN_SYS_PF_GRAY2:
        {
            //����rgbֵ����hlsģ�͵�����
            result = (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*3) + 255 )/(2*255);
        }break;
        case CN_SYS_PF_GRAY4:
        {
            //����rgbֵ����hlsģ�͵�����
            result = (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*15) + 255 )/(2*255);
        }break;
        case CN_SYS_PF_GRAY8:
        {
            //����rgbֵ����hlsģ�͵�����
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*255) + 255 )/(2*255);
        }break;
        case CN_SYS_PF_PALETTE1:
        case CN_SYS_PF_PALETTE2:
        case CN_SYS_PF_PALETTE4:
        case CN_SYS_PF_PALETTE8:
        {
            result = (r<<16) | (g<<8) | b;
        }break;
        default:
        {
            result = 0;
        }
        break;
    }

    return result;
}


//----�ڵ�ɫλͼ��ֱ�ӻ���---------------------------------------------
//����: ��һ��ɫ�ĸ�bitmap�ϻ����㣬Ҫ��color��bitmap�ĸ�ʽ��ͬ����Ҫ�����޸������롣
//����: bitmap��Ŀ��λͼ
//      x��y����ͼ����
//����: ��
//-----------------------------------------------------------------------------
void __GK_SetChangeMsk(struct RectBitmap *bitmap,s32 x,s32 y)
{
    u8 bit_offset;
    u32 byte_offset;
    byte_offset = y*bitmap->linebytes + x/8;//���Ӧ������ֽ�ƫ����
    //���Ӧ�������ֽ��е�λƫ������λ����ߣ���λ����
    bit_offset = 7-x%8;
    //Ŀ��λͼ�϶�Ӧ�������ɫ
    bitmap->bm_bits[byte_offset] |= (u8)1<<bit_offset;
}

//----���ڴ滭��(ִ�ж�Ԫ��դ����)---------------------------------------------
//����: ��һ��bitmap�ϻ��㣬ִ�ж�Ԫ��դ������Ҫ��color��bitmap�ĸ�ʽ��ͬ��
//����: bitmap��Ŀ��λͼ
//      x��y����ͼ����
//      color����ɫ��bitmap�ļ��ݸ�ʽ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_SetPixelRop2Bm(struct RectBitmap *bitmap,
                         s32 x,s32 y,u32 color,u32 Rop2Code)
{
    u8 bit_offset;
    u8 color_bits;
    u32 byte_offset;
    u32 pf,alpha;
    color_bits = (bitmap->PixelFormat & CN_PF_BITWIDE_MSK) >>8;
    //����λͼ��ɫ��ʽ���ڴ��ϻ���
    switch(color_bits)
    {
        case 1:
        {
            byte_offset = y*bitmap->linebytes + x/8;//���Ӧ������ֽ�ƫ����
            //���Ӧ�������ֽ��е�λƫ������λ����ߣ���λ����
            bit_offset = 7-x%8;
            //Ŀ��λͼ�϶�Ӧ�������ɫ
            pf = (u32)bitmap->bm_bits[byte_offset];
            //���ж�Ԫ��դ���������ɫ
            pf = GK_BlendRop2(pf,color<<bit_offset,Rop2Code);
            //������Ŀ��λ�ģ�����λ��ɫ
            bitmap->bm_bits[byte_offset] &= ~(1<<bit_offset);
            //����Ϻ����ɫ��ӵ���Ӧλ
            bitmap->bm_bits[byte_offset] |= (u8)pf;
        }break;
        case 2:
        {
            byte_offset = y*bitmap->linebytes + x/4;//���Ӧ������ֽ�ƫ����
            //���Ӧ�������ֽ��е�λƫ������λ����ߣ���λ����
            bit_offset = (3-x%4)<<1;
            //Ŀ��λͼ�϶�Ӧ�������ɫ
            pf = (u32)bitmap->bm_bits[byte_offset];
            //���ж�Ԫ��դ���������ɫ
            pf = GK_BlendRop2(pf,color<<bit_offset,Rop2Code);
            //������Ŀ��λ�ģ�����λ��ɫ
            bitmap->bm_bits[byte_offset] &= ~(3<<bit_offset);
            //����Ϻ����ɫ��ӵ���Ӧλ
            bitmap->bm_bits[byte_offset] |= (u8)pf;
        }break;
        case 4:
        {
            byte_offset = y*bitmap->linebytes + x/2;//���Ӧ������ֽ�ƫ����
            //���Ӧ�������ֽ��е�λƫ������λ����ߣ���λ����
            bit_offset = (1-x%2)<<2;
            //Ŀ��λͼ�϶�Ӧ�������ɫ
            pf = (u32)bitmap->bm_bits[byte_offset];
            //���ж�Ԫ��դ���������ɫ
            pf = GK_BlendRop2(pf,color<<bit_offset,Rop2Code);
            //������Ŀ��λ�ģ�����λ��ɫ
            bitmap->bm_bits[byte_offset] &= ~(0xf<<bit_offset);
            //����Ϻ����ɫ��ӵ���Ӧλ
            bitmap->bm_bits[byte_offset] |= (u8)pf;
        }break;
        case 8:
        {
            byte_offset = y*bitmap->linebytes + x;//���Ӧ������ֽ�ƫ����
            //���ж�Ԫ��դ���������ɫ
            pf = GK_BlendRop2((u32)bitmap->bm_bits[byte_offset],
                    color,Rop2Code);
            bitmap->bm_bits[byte_offset] = (u8)pf;//����Ϻ����ɫ��ӵ���Ӧλ
        }break;
        case 12:
        {
            byte_offset = y*bitmap->linebytes + x*3/2;//���Ӧ������ֽ�ƫ����
            bit_offset = (x*12)%8;              //���Ӧ�������ֽ��е�λƫ����
            //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
            pf = (u32)bitmap->bm_bits[byte_offset]
                  + (u32)(bitmap->bm_bits[byte_offset+1]<<8);
            pf = pf>>bit_offset;
            pf = GK_BlendRop2(pf,color,Rop2Code);//���ж�Ԫ��դ���������ɫ
            if(bit_offset == 0)
            {
                bitmap->bm_bits[byte_offset] = (u8)(pf&0xff);
                bitmap->bm_bits[byte_offset+1] &= 0xf0;
                bitmap->bm_bits[byte_offset+1] |= (u8)(pf>>8);
            }else
            {
                bitmap->bm_bits[byte_offset] &= 0xf;
                bitmap->bm_bits[byte_offset] |= (u8)((pf&0xf)<<4);
                bitmap->bm_bits[byte_offset+1] = (u8)(pf>>4);
            }
        }break;
        case 16:
        {
            byte_offset = y*bitmap->linebytes + x*2;//���Ӧ������ֽ�ƫ����
            //Ŀ��λͼ�϶�Ӧ�������ɫ
            pf = (u32)(*(u16*)(&bitmap->bm_bits[byte_offset]));
//            alpha = pf;
            //���ж�Ԫ��դ���������ɫ
            pf = GK_BlendRop2(pf,color,Rop2Code);
//            if(bitmap->PixelFormat == CN_SYS_PF_ARGB1555)
//            {
//                pf &= 0x7fff;
//                pf |= alpha & 0x8000;
//            }
            //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
            bitmap->bm_bits[byte_offset] = (u8)pf;
            bitmap->bm_bits[byte_offset+1] = (u8)(pf>>8);
        }break;
        case 24:
        {
            byte_offset = y*bitmap->linebytes + x*3;//���Ӧ������ֽ�ƫ����
            pf = (u32)bitmap->bm_bits[byte_offset];
            pf += (u32)bitmap->bm_bits[byte_offset+1]<<8;
            pf += (u32)bitmap->bm_bits[byte_offset+2]<<16;
            //���ж�Ԫ��դ���������ɫ
            pf = GK_BlendRop2(pf,color,Rop2Code);
            bitmap->bm_bits[byte_offset] = (u8)pf;
            bitmap->bm_bits[byte_offset+1] = (u8)(pf >> 8);
            bitmap->bm_bits[byte_offset+2] = (u8)(pf >> 16);
        }break;
        case 32:
        {
            byte_offset = y*bitmap->linebytes + x*4;//���Ӧ������ֽ�ƫ����
            pf = (u32)(*(u32*)(&bitmap->bm_bits[byte_offset]));
            alpha = pf;
            //���ж�Ԫ��դ���������ɫ
            pf = GK_BlendRop2(pf,color,Rop2Code);
            if(bitmap->PixelFormat == CN_SYS_PF_ARGB8888)
            {
                pf &= 0x00ffffff;
                pf |= alpha & 0xff000000;
            }
            //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����32λָ�롣
            bitmap->bm_bits[byte_offset] = (u8)pf;
            bitmap->bm_bits[byte_offset+1] = (u8)(pf >> 8);
            bitmap->bm_bits[byte_offset+2] = (u8)(pf >> 16);
            bitmap->bm_bits[byte_offset+3] = (u8)(pf >> 24);
        }break;
    }
}

//----RGB888��ʽAlpha���-------------------------------------------------------
//����: ������ɫ��ǰ��ɫ����Alpha���
//����: bk_c, ����ɫ
//     fr_c, ǰ��ɫ
//     alpha, Alphaֵ
//����:�µ���ɫֵ(RGB888��ʽ)
//-----------------------------------------------------------------------------
static  u32 __alpha_blend_color(u32 bk_c,u32 fr_c,u8 alpha)
{
    u8 bk_r,bk_g,bk_b;
    u8 fr_r,fr_g,fr_b;

    bk_r =bk_c>>16;
    bk_g =bk_c>>8;
    bk_b =bk_c;
    fr_r =fr_c>>16;
    fr_g =fr_c>>8;
    fr_b =fr_c;
    fr_r =(((fr_r-bk_r)*alpha)>>8) + bk_r;
    fr_g =(((fr_g-bk_g)*alpha)>>8) + bk_g;
    fr_b =(((fr_b-bk_b)*alpha)>>8) + bk_b;

    return  (fr_r<<16)|(fr_g<<8)|(fr_b);
}

//----λͼ��copy����(RopCode)----------------------------------------------
//����: ��src_bitmapλͼ��ָ����һ�����أ����Ƶ�dst_bitmap�ϣ�ִ����չrop������
//      ��չrop�����Ķ��壬�� CN_ROP_ALPHA_SRC_MSK ����ض��塣ע�⣬Ŀ��λͼ��
//      ����alphaͨ����Ҳ�������ǵ�ɫ��λͼ��
//����: dst_bitmap��Ŀ��λͼ
//      src_bitmap��Դλͼ
//      x_dst��y_dst��Ŀ��λͼ�е�Ŀ������
//      x_src��y_src��Դλͼ�е�Ŀ������
//      RopCode����դ�����룬�� CN_ROP_ALPHA_SRC_MSK ����ض���
//      HyalineColor��͸��ɫ,ERGB8888��ʽ
//����: ��
//-----------------------------------------------------------------------------
void __GK_CopyPixelRopBm(struct RectBitmap *dst_bitmap,
                                struct RectBitmap *src_bitmap,
                                s32 x_dst,s32 y_dst,s32 x_src,s32 y_src,
                                struct RopGroup RopCode,u32 HyalineColor)
{
    u8 src_alpha,dst_alpha;
    u8 Rop2;
    u32 color;
    u32 dst_color,src_color;

    //��ȡԴλͼ�е���ɫ��δת����ERGB8888
    src_color = GK_GetPixelBm(src_bitmap,x_src,y_src);
    //ת��ʱ���Ѿ�����Դλͼ��alphaͨ����(�����)
    src_color = GK_ConvertColorToRGB24(src_bitmap->PixelFormat,
                                       src_color,src_bitmap->ExColor);
    if(RopCode.AlphaEn == 1)       //alpha���
    {
        if((RopCode.HyalineEn == 0) || (src_color != HyalineColor))
        {
            src_alpha = RopCode.SrcAlpha;
            dst_alpha = RopCode.DstAlpha;
            //��Դλͼ��ɫ������ɫ��ʽת�����õ���Ŀ��λͼ��ɫ��ʽһ�µ���ɫ
            src_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,src_color);
            //��ȡĿ��λͼ�е���ɫ��δת����ERGB8888
            dst_color = GK_GetPixelBm(dst_bitmap,x_dst,y_dst);
            color = __GK_AlphaBlendBitmapToWin(src_color,dst_color,
                            dst_bitmap->PixelFormat,
                            dst_bitmap->ExColor,src_alpha,dst_alpha,
                            RopCode.AlphaMode);
            //��������
            __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,color,CN_R2_COPYPEN);
        }
    }
    else if(RopCode.Rop2Mode != CN_R2_COPYPEN) //���ͬʱʹ��alpha��Ϻ�rop2��rop2��������
    {
        if((RopCode.HyalineEn ==0) || (src_color != HyalineColor))
        {
            Rop2 = RopCode.Rop2Mode;
            //��Դλͼ��ɫ������ɫ��ʽת�����õ���Ŀ��λͼ��ɫ��ʽһ�µ���ɫ
            src_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,src_color);
            //��������
            __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,src_color,Rop2);
        }
    }
    else if(RopCode.HyalineEn == 1)     //͸��ɫʹ��
    {
        //��HyalineColor��ERGB8888��ʽת��Ϊ���ظ�ʽ
//        HyalineColor=GK_ConvertRGB24ToPF(src_bitmap->PixelFormat,HyalineColor);
//         //��ȡԴλͼ�е���ɫ��δת����ERGB8888
//        src_color = GK_GetPixelBm(src_bitmap,x_src,y_src);

        if(src_color != HyalineColor)
        {
            //��Դλͼ��ɫ������ɫ��ʽת�����õ���Ŀ��λͼ��ɫ��ʽһ�µ���ɫ
            src_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,src_color);
            //��������
            __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,src_color,CN_R2_COPYPEN);
        }
    }
    else
    {
        switch(src_bitmap->PixelFormat)
        {
           case CN_SYS_PF_ALPHA1:
               if(GK_GetPixelBm(src_bitmap,x_src,y_src))
               {
                    //��Դλͼ��ɫ������ɫ��ʽת�����õ���Ŀ��λͼ��ɫ��ʽһ�µ���ɫ
                    src_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,src_bitmap->ExColor);
                    //��������
                    __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,src_color,CN_R2_COPYPEN);
               }
               break;

           case CN_SYS_PF_ALPHA2:
               src_alpha = GK_GetPixelBm(src_bitmap,x_src,y_src);
               src_alpha =(src_alpha<<6)&0xC0;

               dst_color = GK_GetPixelBm(dst_bitmap,x_dst,y_dst);
               dst_color = GK_ConvertColorToRGB24(dst_bitmap->PixelFormat,dst_color,dst_bitmap->ExColor);

               dst_color = __alpha_blend_color(dst_color,src_bitmap->ExColor,src_alpha);
               dst_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,dst_color);
               __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,dst_color,CN_R2_COPYPEN);
               break;

           case CN_SYS_PF_ALPHA4:
               src_alpha = GK_GetPixelBm(src_bitmap,x_src,y_src);
               src_alpha = (src_alpha<<4)&0xF0;

               dst_color = GK_GetPixelBm(dst_bitmap,x_dst,y_dst);
               dst_color = GK_ConvertColorToRGB24(dst_bitmap->PixelFormat,dst_color,dst_bitmap->ExColor);

               dst_color = __alpha_blend_color(dst_color,src_bitmap->ExColor,src_alpha);
               dst_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,dst_color);
               __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,dst_color,CN_R2_COPYPEN);
               break;

           case CN_SYS_PF_ALPHA8:
               src_alpha = GK_GetPixelBm(src_bitmap,x_src,y_src);
               src_alpha = src_alpha&0xFF;

               dst_color = GK_GetPixelBm(dst_bitmap,x_dst,y_dst);
               dst_color = GK_ConvertColorToRGB24(dst_bitmap->PixelFormat,dst_color,dst_bitmap->ExColor);

               dst_color = __alpha_blend_color(dst_color,src_bitmap->ExColor,src_alpha);
               dst_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,dst_color);
               __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,dst_color,CN_R2_COPYPEN);
               break;

           default:
               //��Դλͼ��ɫ������ɫ��ʽת�����õ���Ŀ��λͼ��ɫ��ʽһ�µ���ɫ
               src_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,src_color);
               __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,src_color,CN_R2_COPYPEN);
               break;
        }

    }

}

//----���ڴ滭��---------------------------------------------------------------
//����: ��һ��bitmap�ϵ�һ����copy����һ��bitmap�����Դλͼ��alphaͨ��������Ҫ
//      ����alpha
//����: dst_bitmap��Ŀ��λͼ
//      src_bitmap��Դλͼ
//      x_dst��y_dst��Ŀ��λͼ�е�Ŀ������
//      x_src��y_src��Դλͼ�е�Ŀ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_CopyPixelBm(struct RectBitmap *dst_bitmap,
                      struct RectBitmap *src_bitmap,
                      s32 x_dst,s32 y_dst,s32 x_src,s32 y_src)
{
    u32 pf_src,cvt_src;
    //Ŀ��λͼ��ɫ��ʽ
//    color_bits = (dst_bitmap->PixelFormat & CN_PF_BITWIDE_MSK)>>8;
    //ȡԴλͼ��ɫ������Դλͼ��ɫ��ʽ������ֵ
    pf_src = GK_GetPixelBm(src_bitmap,x_src,y_src);
    //��ȡ����Դλͼ��ɫ������ɫ��ʽת�����õ���Ŀ��λͼ��ɫ��ʽһ�µ���ɫ
    cvt_src = GK_ConvertPF2PF(dst_bitmap->PixelFormat,
                            src_bitmap->PixelFormat,pf_src,src_bitmap->ExColor);
    __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,cvt_src,CN_R2_COPYPEN);
}

//----����---------------------------------------------------------------------
//����: �ڴ����ڻ�һ�����ص㡣
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��ָ��
//      x��y����ͼ����
//      color������ʹ�õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_SetPixel(struct GkscParaSetPixel *para)
{
    struct ClipRect *clip;
    struct GkWinObj *fb_gkwin,*pixelwin;
    struct RectBitmap *bitmap;
    u32 pf_color;
    s32 offsetx,offsety;
    if((para->Rop2Code == CN_R2_NOP) || (para->Rop2Code > CN_R2_LAST))
        return;                                     //��ִ�в���
    pixelwin = para->gkwin;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((pixelwin->wm_bitmap != NULL)
        && (pixelwin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
    {   //������:��win buffer�л�ͼ����־changed_msk
        bitmap = pixelwin->wm_bitmap;
        //����ʾ��ʹ��cn_custom_pf��ʽ��Ӧ����Ӳ��ʵ�ֻ�������
        if(bitmap->PixelFormat & CN_CUSTOM_PF)
        {
            pixelwin->disp->draw.SetPixelToBitmap(bitmap,para->x,para->y,
                                                 para->color,para->Rop2Code);
        }else//�����ʵ�ֻ�������
        {
            //�涨�����߸�������ɫ�������ɫ
            pf_color = GK_ConvertRGB24ToPF(bitmap->PixelFormat,para->color);
            __GK_SetPixelRop2Bm(bitmap,para->x,para->y,
                                pf_color,para->Rop2Code);      //��������
        }
        __GK_ShadingPixel(pixelwin,para->x,para->y);//��־���ص�changed_msk
    }else       //��win buffer����ֱ��д������Ϊtrue
    {
        clip = pixelwin->visible_clip;
        if(clip == NULL)        //������Ϊ�գ�ֱ�ӷ���
            return ;
        fb_gkwin = pixelwin->disp->frame_buffer;
        offsetx = pixelwin->ScreenX +para->x;
        offsety = pixelwin->ScreenY +para->y;
        //��frame buffer����ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (pixelwin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            bitmap = fb_gkwin->wm_bitmap;
            do{
                //���ض�Ӧ�������ڿ������ڣ����������
                if(!((offsetx<clip->rect.left) || (offsetx>=clip->rect.right)
                    || (offsety<clip->rect.top) ||(offsety>=clip->rect.bottom)))
                {
                    //����ʾ��ʹ��cn_custom_pf��ʽ��Ӧ����Ӳ��driverʵ�ֻ�������
                    if(bitmap->PixelFormat & CN_CUSTOM_PF)
                    {
                        fb_gkwin->disp->draw.SetPixelToBitmap(bitmap,offsetx,
                                           offsety,para->color,para->Rop2Code);
                    }else//�����ʵ�ֻ�������
                    {
                        //�涨�����߸�������ɫ��ɫ��ʽ�������ɫ
                        pf_color = GK_ConvertRGB24ToPF(
                                                bitmap->PixelFormat,para->color);
                        __GK_SetPixelRop2Bm(bitmap,offsetx,offsety,
                                                pf_color,para->Rop2Code);
                    }
                    //��־���ص�changed_msk
                    __GK_ShadingPixel(fb_gkwin,offsetx,offsety);
                    break;      //�����Ѿ����ƣ��˳�ѭ��
                }else//���ض�Ӧ�����겻�ڿ������ڣ�ָ����һ��������
                    clip = clip->next;
            }while(clip != pixelwin->visible_clip);
        }
        else
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{
                //���ض�Ӧ�������ڿ������ڣ����������
                if(!((offsetx<clip->rect.left) || (offsetx>=clip->rect.right)
                    || (offsety<clip->rect.top) ||(offsety>=clip->rect.bottom)))
                {
                    __GK_SetPixelScreen(pixelwin->disp,offsetx,offsety,
                                          para->color,para->Rop2Code);
                    break;      //�����Ѿ����ƣ��˳�ѭ��
                }else//���ض�Ӧ�����겻�ڿ������ڣ�ָ����һ��������
                    clip = clip->next;
            }while(clip != pixelwin->visible_clip);
        }
    }
}

//----����changed_msk:����-----------------------------------------------------
//����: ��������ʱ�����ô��ڵ�changed_msk���Ѹ�д����������ɫ���ɵ�����ȷ������
//      �Ϸ���
//����: gkwin��Ŀ�괰��ָ��
//      x��y��Ŀ�����ض�Ӧ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_ShadingPixel(struct GkWinObj *gkwin,s32 x,s32 y)
{
    s32 msk_x,msk_y;
    if(gkwin->WinProperty.ChangeFlag == CN_GKWIN_CHANGE_ALL)   //�Ѿ�ȫ����ɫ
    {
        return;
    }
    gkwin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_PART;
    msk_x = x/8;       //����x��mskλͼ�еڼ���
    msk_y = y/8;       //����y��mskλͼ�еڼ���
    //���������ڵ�changed_msk�е�bit��Ϊ1
    __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
    return;
}

//----����changed_msk----------------------------------------------------------
//����: ��һ�����ڵ�rect�޶�����������Ϊ����д
//����: bitmap��Ŀ��bitmap
//      rect����д����
//����: ��
//˵��: �ڲ����������������Ϸ��ԣ��ɵ����߱�֤����ĺϷ���
//-----------------------------------------------------------------------------
void __GK_ShadingRect(struct GkWinObj *gkwin,struct Rectangle *rect)
{
    struct RectBitmap *bitmap;
    s32 x,y;
    s32 msk_x1,msk_x2,msk_y1,msk_y2;

    if(gkwin->WinProperty.ChangeFlag == CN_GKWIN_CHANGE_ALL)   //�Ѿ�ȫ����ɫ
    {
        return;
    }
    bitmap = gkwin->wm_bitmap;
    if((rect->left <= 0) && (rect->top <= 0)
            && (rect->right >= bitmap->width)&&(rect->bottom >= bitmap->height))
    {
        gkwin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_ALL;   //��msk�����򸲸���������
        return;
    }

    gkwin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_PART;
    msk_x1 = rect->left/8;          //����x1��mskλͼ�еڼ���
    msk_x2 = (rect->right-1)/8;     //����x2��mskλͼ�еڼ��У��������յ㣬-1
    msk_y1 = rect->top/8;           //����y1��mskλͼ�еڼ���
    msk_y2 = (rect->bottom-1)/8;    //����y2��mskλͼ�еڼ��У��������յ㣬-1

    for(y = msk_y1;y <= msk_y2;y++)
    {
        for(x = msk_x1;x <= msk_x2;x++)
            __GK_SetChangeMsk(&(gkwin->changed_msk),x,y);
    }
}

//----����changed_msk----------------------------------------------------------
//����: �ڴ��ڻ�frame buffer����ֱ��ʱ�����ô��ڵ�changed_msk��
//      �Ѹ�д����������ɫ��
//����: gkwin��Ŀ�괰��
//      limit�������ͼ����������һ�����ε����򣬳����������ֱ�߲���
//      x1��y1��x2��y2�� ��㡢�յ�����
//����: ��
//-----------------------------------------------------------------------------
void __GK_ShadingLine(struct GkWinObj *gkwin,struct Rectangle *limit,
                     s32 x1,s32 y1,s32 x2,s32 y2)
{
    if(gkwin->WinProperty.ChangeFlag == CN_GKWIN_CHANGE_ALL)   //�Ѿ�ȫ����ɫ
    {
        return;
    }
    //ȡ�߶���limit�ڵĲ��֣����˶���Ҫ���Ƶĵ㡣
    if(!__GK_OlineSectInter(limit,&x1,&y1,&x2,&y2))
        return;
    gkwin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_PART;

    if(y1 == y2)        //ˮƽֱ����ɫ
    {
        s32 x;
        s32 msk_x1,msk_x2,msk_y1;
//      if(x1>x2)
//          __gk_swap(x1,x2);
//        if((y1 < limit->top) || (y1 >= limit->bottom)
//            ||(x2 < limit->left) || (x1 >= limit->right))
//            return;         //����ֱ����limit֮��
//      if(x1 < limit->top)
//          x1 = limit->top;
//      if(x2 > limit->bottom)
//          x2 = limit->bottom;
        msk_x1 = x1/8;     //����x1��mskλͼ�еڼ���
        msk_x2 = (x2)/8; //����(x2)��mskλͼ�еڼ���
        msk_y1 = y1/8;     //����y1��mskλͼ�еڼ���
        if(msk_x1>msk_x2)
            __gk_swap(msk_x1,msk_x2);
        for(x = msk_x1;x <= msk_x2;x++)
        {
            //��changed_msk��Ӧ��bit��1
            __GK_SetChangeMsk(&(gkwin->changed_msk),x,msk_y1);
        }

    }
    else if(x1 == x2)   //��ֱ����ɫ
    {
        s32 y;
        s32 msk_x1,msk_y1,msk_y2;
//      if(y1>y2)
//          __gk_swap(y1,y2);
//      if((x1<limit->left) || (x1 >= limit->right)
//          ||(y2 < limit->top) || (y1 >= limit->bottom))
//          return;         //����ֱ����limit֮��
//      if(y1 < limit->top)
//          y1 = limit->top;
//      if(y2 > limit->bottom)
//          y2 = limit->bottom;
        msk_x1 = x1/8;     //����x1��mskλͼ�еڼ���
        msk_y1 = y1/8;     //����y1��mskλͼ�еڼ���
        msk_y2 = (y2)/8; //����(y2)��mskλͼ�еڼ���
        if(msk_y1>msk_y2)
            __gk_swap(msk_y1,msk_y2);
        for(y = msk_y1;y <= msk_y2;y++)
        {
            //��changed_msk��Ӧ��bit��1
            __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x1,y);
        }
    }
    else
    {
        s32 x,y;
        s32 msk_x,msk_y;
        s32 dx,dy;
        dx = abs(x2-x1);
        dy = abs(y2-y1);

        //x����仯����
        if(dx > dy)
        {
            if(((x1>x2)&&(y1>y2)) || ((x1<x2)&&(y1<y2)))
            {//x��y����������ͬ����ͬ������ͬ��
                //ѡȡx1��x2�н�С������Ϊx1
                if(x1 > x2)
                {
                    __gk_swap(x1,x2);
                    __gk_swap(y1,y2);
                }
                //���¶��룬ȡ8*8���أ�xÿ�仯8���غ�y��ֱ�ߵĽ��㣬
                //��б�������еĵ����ڵ�8*8���������Ӧ��
                //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                for(x = align_down(8,x1); x <= x2; x+=8)
                {
                    //�Լ���Ľ�������������봦��x��y����������ͬ
                    //������0.5Ϊ��׼��
                    y = ((((x-x1)<<1)*(y2-y1)+(x2-x1))/((x2-x1)<<1))+y1;
                    msk_x = x/8;    //����x��mskλͼ�еڼ���
                    msk_y = y/8;    //����y��mskλͼ�еڼ���
                    //��changed_msk��Ӧ��bit��1
                    __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
                    //��־(msk_x��msk_y)�����һ����
                    msk_y += 1;
                    //��changed_msk��Ӧ��bit��1
                    __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
                }
            }
            else
            {//x��y���������෴����x����y����x����y��
                //ѡȡx1��x2�н�С������Ϊx1
                if(x1 > x2)
                {
                    __gk_swap(x1,x2);
                    __gk_swap(y1,y2);
                }
                //���¶��룬ȡ8*8���أ�xÿ�仯8���غ�y��ֱ�ߵĽ��㣬
                //��б�������еĵ����ڵ�8*8���������Ӧ��
                //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                for(x = align_down(8,x1); x <= x2; x+=8)
                {
                    //�����ý�������������봦��yΪ������������-0.5Ϊ��׼��
                    y = ((((x-x1)<<1)*(y2-y1)-(x2-x1))/((x2-x1)<<1))+y1;
                    msk_x = x/8;    //����x��mskλͼ�еڼ���
                    msk_y = y/8;    //����y��mskλͼ�еڼ���
                    //��changed_msk��Ӧ��bit��1
                    __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
                    //��־(msk_x��msk_y)�����һ����
                    msk_y -= 1;
                    //��changed_msk��Ӧ��bit��1
                    __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
                }
            }
        }else
        {//y����仯���󣬻���x��y�仯�����
            if(((x1>x2)&&(y1>y2)) || ((x1<x2)&&(y1<y2)))
            {//x��y����������ͬ����ͬ������ͬ��
                //ѡȡy1��y2�н�С������Ϊy1
                if(y1 > y2)
                {
                    __gk_swap(x1,x2);
                    __gk_swap(y1,y2);
                }
                //���¶��룬ȡ8*8���أ�yÿ�仯8���غ�x��ֱ�ߵĽ��㣬
                //��б�������еĵ����ڵ�8*8���������Ӧ��
                //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                for(y = align_down(8,y1); y <= y2; y+=8)
                {
                    x = ((((y-y1)<<1)*(x2-x1)+(y2-y1))/((y2-y1)<<1))+x1;
                    msk_x = x/8;    //����x��mskλͼ�еڼ���
                    msk_y = y/8;    //����y��mskλͼ�еڼ���
                    //��changed_msk��Ӧ��bit��1
                    __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
                    msk_x += 1;
                    //��changed_msk��Ӧ��bit��1
                    __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
                }
            }
            else
            {//x��y���������෴����x����y����x����y��
                //ѡȡy1��y2�н�С������Ϊy1
                if(y1 > y2)
                {
                    __gk_swap(x1,x2);
                    __gk_swap(y1,y2);
                }
                //���¶��룬ȡ8*8���أ�yÿ�仯8���غ�x��ֱ�ߵĽ��㣬
                //��б�������еĵ����ڵ�8*8���������Ӧ��
                //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                for(y = align_down(8,y1); y <= y2; y+=8)
                {
                    x = ((((y-y1)<<1)*(x2-x1)-(y2-y1))/((y2-y1)<<1))+x1;
                    msk_x = x/8;    //����x��mskλͼ�еڼ���
                    msk_y = y/8;    //����y��mskλͼ�еڼ���
                    //��changed_msk��Ӧ��bit��1
                    __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
                    msk_x -= 1;
                    //��changed_msk��Ӧ��bit��1
                    __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
                }
            }
        }
    }
}

//----���changed_msk----------------------------------------------------------
//����: ���һ�����ڵĸ�д��־��
//����: gkwin��Ŀ�괰��ָ��
//����: ��
//-----------------------------------------------------------------------------
void __GK_ShadingClear(struct GkWinObj *gkwin)
{
    u32 msk_size;
    if(gkwin->changed_msk.bm_bits)
    {
        msk_size =
            (gkwin->wm_bitmap->width +7)/8 /8*((gkwin->wm_bitmap->height +7)/8);
        memset(gkwin->changed_msk.bm_bits,0,msk_size);
    }
}

//----����б�߲��������-------------------------------------------------------
//����: ����б�߲���־λ��
//����: gkwin��Ŀ�괰��
//      x1��y1��x2��y2�����յ�
//      color����������ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//˵��: ���õ���Bresenham�㷨
//-----------------------------------------------------------------------------
void __GK_DrawOline(struct GkWinObj *gkwin,s32 x1,s32 y1,
                                        s32 x2,s32 y2,u32 color,u32 Rop2Code)
{
    u32 pf_color;
    s32 x,y,dx,dy,s1,s2,p,interchange,i;

    struct RectBitmap *bitmap;
    bitmap = gkwin->wm_bitmap;

    x = x1;
    y = y1;
    dx = abs(x2-x1);
    dy = abs(y2-y1);
    //��־����б�߷���
    if(x2 > x1)
        s1 = 1;
    else
        s1 = -1;
    if(y2 > y1)
        s2 = 1;
    else
        s2 = -1;
    //��־���仯�ı�����interchangeΪ1��y�仯���Ϊ0��x�仯���
    if(dy > dx)
    {
        __gk_swap(dx, dy);
        interchange = 1;
    }
    else
        interchange = 0;
    p = (dy<<1)-dx;            //���ó�ʼ����б�ֵ
    //��ɫ��ʽΪ�Կ��Զ����ʽ
    if(bitmap->PixelFormat & CN_CUSTOM_PF)
    {
        for(i=1;i<=dx;i++)
        {
            gkwin->disp->draw.SetPixelToBitmap(bitmap,x,y,color,Rop2Code);
            //ȡ����ѧ��Ͻ�������
            if(p >= 0)
            {
                if(interchange == 0)
                    y = y+s2;
                else
                    x = x+s1;
                p = p-(dx<<1);
            }
            if(interchange == 0)
                x = x+s1;
            else
                y = y+s2;
            p = p+(dy<<1);
        }
    }
    else
    {//��ɫ��ʽΪ������ʽ
        pf_color = GK_ConvertRGB24ToPF(bitmap->PixelFormat,color);
        for(i=1;i<=dx;i++)
        {
            __GK_SetPixelRop2Bm(bitmap,x,y,pf_color,Rop2Code);
            //ȡ����ѧ��Ͻ�������
            if(p >= 0)
            {
                if(interchange == 0)
                    y = y+s2;
                else
                    x = x+s1;
                p = p-(dx<<1);
            }
            if(interchange == 0)
                x = x+s1;
            else
                y = y+s2;
            p = p+(dy<<1);
        }
    }
}

//----����ֱ��-------------------------------------------------------
//����: �ڴ����ڻ�һ����ֱ�ߣ�ֻ��limit�޶��Ĳ���
//����: gkwin��Ŀ�괰��ָ��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x1��y1����ʼ�������
//      y2���������������
//      color����ɫ��CN_SYS_PF_ERGB8888��ʽ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_VlinetoWin(struct GkWinObj *gkwin,struct Rectangle *limit,
                     s32 x1,s32 y1,s32 y2,u32 color,u32 Rop2Code)
{
    s32 y;
    s32 msk_x1,msk_y1,msk_y2;
    s32 flag = (y1>y2)?-1:1;
    u32 pf_color;

    struct RectBitmap *bitmap;


    //��ֱ�߳���limit�Ĳ��ֲ�����
    if(flag==-1)
    {
        if((x1<limit->left) || (x1 >= limit->right)
                            ||(y1 <= limit->top) || (y2 >= limit->bottom))
                        return;         //����ֱ����limit֮��
        bitmap = gkwin->wm_bitmap;
        if(y1>= limit->bottom)
                y1 = limit->bottom-1;
        if(y2 < limit->top)
                y2 = limit->top;
    }
    else if(flag == 1)
    {
        if((x1<limit->left) || (x1 >= limit->right)
                    ||(y2 <= limit->top) || (y1 >= limit->bottom))
                return;         //����ֱ����limit֮��
        bitmap = gkwin->wm_bitmap;
        if(y1 < limit->top)
            y1 = limit->top;
        if(y2 >= limit->bottom)
            y2 = limit->bottom-1;
    }

    if(bitmap->PixelFormat & CN_CUSTOM_PF)
    {
        if(flag==1)
        {
            for(y = y1; y <= y2;y++)
            {
                gkwin->disp->draw.SetPixelToBitmap(bitmap,x1,y,color,Rop2Code);
            }
        }else
        {
            for(y = y1; y >= y2;y--)
            {
                gkwin->disp->draw.SetPixelToBitmap(bitmap,x1,y,color,Rop2Code);
            }
        }
    }else
    {
        //�û�������ɫ��ɫ��ʽΪrgb24
        pf_color = GK_ConvertRGB24ToPF(bitmap->PixelFormat,color);
        if(flag==1)
        {
            for(y = y1; y <= y2;y++)
            {
                __GK_SetPixelRop2Bm(bitmap,x1,y,pf_color,Rop2Code);
            }
        }else
        {
            for(y = y1; y >= y2;y--)
            {
                __GK_SetPixelRop2Bm(bitmap,x1,y,pf_color,Rop2Code);
            }
        }
    }

    //���洦��changed_msk��־λͼ���Ѵ��������߶��޸ĵ�������ɫ
    if(gkwin->WinProperty.ChangeFlag != CN_GKWIN_CHANGE_ALL)
    {
        gkwin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_PART;  //��־���ڲ����޸�
        msk_y1 = y1/8;     //����y1��mskλͼ�еڼ���
        msk_x1 = x1/8;     //����x1��mskλͼ�еڼ���
        if(flag==1)
        {
            msk_y2 = (y2-1)/8; //����(y2-1)��mskλͼ�еڼ��У�-1����Ϊ�յ㲻��������
            for(y = msk_y1;y <= msk_y2;y++)
            {   //��changed_msk��Ӧ��bit��1
                __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x1,y);
            }
        }else
        {
            msk_y2 = (y2+1)/8; //����(y2-1)��mskλͼ�еڼ��У�-1����Ϊ�յ㲻��������
            for(y = msk_y1;y >= msk_y2;y--)
            {   //��changed_msk��Ӧ��bit��1
                __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x1,y);
            }
        }
    }
}

//----��ˮƽֱ��-------------------------------------------------------
//����: �ڴ����ڻ�һ��ˮƽֱ�ߣ�ֻ��limit�޶��Ĳ���
//����: gkwin��Ŀ�괰��ָ��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x1��y1����ʼ�������
//      x2��������ĺ�����
//      color����ɫ��CN_SYS_PF_ERGB8888��ʽ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_HlinetoWin(struct GkWinObj *gkwin,struct Rectangle *limit,
                     s32 x1,s32 y1,s32 x2,u32 color,u32 Rop2Code)
{
    s32 x;
    s32 msk_x1,msk_x2,msk_y1;
    s32 flag = (x1>x2)?-1:1;
    u32 pf_color;

    struct RectBitmap *bitmap;

    //ˮƽ����limit�ⲿ�ֲ�����
    if(flag == 1)
    {
        if((y1<limit->top) || (y1 >= limit->bottom)
        ||(x2 <= limit->left) || (x1 >= limit->right))
            return;         //����ֱ����limit֮��
        bitmap = gkwin->wm_bitmap;
        if(x1 < limit->left)
            x1 = limit->left;
        if(x2 >= limit->right)
            x2 = limit->right-1;
    }
    else if(flag == -1)
    {
        if((y1<limit->top) || (y1 >= limit->bottom)
                    ||(x1 <= limit->left) || (x2 >= limit->right))
                    return;         //����ֱ����limit֮��
        bitmap = gkwin->wm_bitmap;

        if(x1 >= limit->right)
            x1 = limit->right-1;
        if(x2 < limit->left)
            x2 = limit->left;
    }

    if(bitmap->PixelFormat & CN_CUSTOM_PF)
    {
        if(flag==1)
        {
            for(x = x1; x <= x2;x++)
            {
                gkwin->disp->draw.SetPixelToBitmap(bitmap,x,y1,color,Rop2Code);
            }
        }else
        {
            for(x = x1; x >= x2;x--)
            {
                gkwin->disp->draw.SetPixelToBitmap(bitmap,x,y1,color,Rop2Code);
            }
        }
    }else
    {
        //�û�������ɫ��ɫ��ʽΪrgb24
        pf_color = GK_ConvertRGB24ToPF(bitmap->PixelFormat,color);
        if(flag==1)
        {
            for(x = x1;x <= x2;x++)
            {//��1��2��4λɫscreen�����ѭ���ܺ�cpu�����ǸĽ�--db
                __GK_SetPixelRop2Bm(bitmap,x,y1,pf_color,Rop2Code);
            }
        }else
        {
            for(x = x1;x >= x2;x--)
            {//��1��2��4λɫscreen�����ѭ���ܺ�cpu�����ǸĽ�--db
                __GK_SetPixelRop2Bm(bitmap,x,y1,pf_color,Rop2Code);
            }
        }

        //���洦��changed_msk��־λͼ���Ѵ��������߶��޸ĵ�������ɫ
        if(gkwin->WinProperty.ChangeFlag != CN_GKWIN_CHANGE_ALL)
        {
            gkwin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_PART;  //��־���ڲ����޸�
            msk_x1 = x1/8;     //����x1��mskλͼ�еڼ���
            msk_y1 = y1/8;     //����y1��mskλͼ�еڼ���
            if(flag==1)
            {
                msk_x2 = (x2-1)/8; //����(x2-1)��mskλͼ�еڼ��У�-1����Ϊ�յ㲻��������
                for(x = msk_x1;x <= msk_x2;x++)
                {   //��changed_msk��Ӧ��bit��1
                        __GK_SetChangeMsk(&(gkwin->changed_msk),x,msk_y1);
                }
            }else
            {
                msk_x2 = (x2+1)/8; //����(x2-1)��mskλͼ�еڼ��У�-1����Ϊ�յ㲻��������
                for(x = msk_x1;x >= msk_x2;x--)
                {//��changed_msk��Ӧ��bit��1
                    __GK_SetChangeMsk(&(gkwin->changed_msk),x,msk_y1);
                }
            }
        }
    }
}

//----��б��---------------------------------------------------------
//����: �ڴ����ڻ�һ��б�ߣ�ֻ��limit�޶��Ĳ���
//����: gkwin��Ŀ�괰��ָ��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x1��y1����ʼ�������
//      x2��y2�������������
//      color����ɫ��CN_SYS_PF_ERGB8888��ʽ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_OlinetoWin(struct GkWinObj *gkwin,struct Rectangle *limit,//ȷ��
                       s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 Rop2Code)
{
    s32 x,y;
    s32 msk_x,msk_y;
    s32 dx,dy;

    //ȡ��б����limit�Ľ��㣬����limit����Ĳ���
    if(__GK_OlineSectInter(limit,&x1,&y1,&x2,&y2))
    {
        if(y1 == y2)        //������limit�ڲ�����ˮƽ��
            __GK_HlinetoWin(gkwin,limit,x1,y1,x2,color,Rop2Code);
        else if(x1 == x2)   //������limit�ڲ����Ǵ�ֱ��
            __GK_VlinetoWin(gkwin,limit,x1,y1,y2,color,Rop2Code);
        else
        {
            __GK_DrawOline(gkwin,x1,y1,x2,y2,color,Rop2Code);//����б��

            //���洦��changed_msk��־λͼ���Ѵ��������߶��޸ĵ�������ɫ
            if(gkwin->WinProperty.ChangeFlag != CN_GKWIN_CHANGE_ALL)
            {
                gkwin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_PART;//��־���ڲ����޸�
                dx = abs(x2-x1);
                dy = abs(y2-y1);
                //x����仯����
                if(dx > dy)
                {
                    if(((x1>x2)&&(y1>y2)) || ((x1<x2)&&(y1<y2)))
                    {//x��y����������ͬ����ͬ������ͬ��
                        //ѡȡx1��x2�н�С������Ϊx1
                        if(x1 > x2)
                        {
                            __gk_swap(x1,x2);
                            __gk_swap(y1,y2);
                        }
                        //���¶��룬ȡ8*8���أ�xÿ�仯8���غ�y��ֱ�ߵĽ��㣬
                        //��б�������еĵ����ڵ�8*8���������Ӧ��
                        //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                        for(x = align_down(8,x1); x <= x2; x+=8)
                        {
                            //�Լ���Ľ�������������봦��x��y����������ͬ
                            //������0.5Ϊ��׼��
                            y = ((((x-x1)<<1)*(y2-y1)+(x2-x1))/((x2-x1)<<1))+y1;
                            msk_x = x/8;    //����x��mskλͼ�еڼ���
                            msk_y = y/8;    //����y��mskλͼ�еڼ���
                            //��changed_msk��Ӧ��bit��1
                            __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
                            //��־(msk_x��msk_y)�����һ����
                            msk_y += 1;
                            //��changed_msk��Ӧ��bit��1
                            __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
                        }
                    }
                    else
                    {//x��y���������෴����x����y����x����y��
                        //ѡȡx1��x2�н�С������Ϊx1
                        if(x1 > x2)
                        {
                            __gk_swap(x1,x2);
                            __gk_swap(y1,y2);
                        }
                        //���¶��룬ȡ8*8���أ�xÿ�仯8���غ�y��ֱ�ߵĽ��㣬
                        //��б�������еĵ����ڵ�8*8���������Ӧ��
                        //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                        for(x = align_down(8,x1); x <= x2; x+=8)
                        {
                            //�����ý�������������봦��yΪ������������-0.5Ϊ��׼��
                            y = ((((x-x1)<<1)*(y2-y1)-(x2-x1))/((x2-x1)<<1))+y1;
                            msk_x = x/8;    //����x��mskλͼ�еڼ���
                            msk_y = y/8;    //����y��mskλͼ�еڼ���
                            //��changed_msk��Ӧ��bit��1
                            __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
                            //��־(msk_x��msk_y)�����һ����
                            msk_y -= 1;
                            //��changed_msk��Ӧ��bit��1
                            __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
                        }
                    }
                }else
                {//y����仯���󣬻���x��y�仯�����
                    if(((x1>x2)&&(y1>y2)) || ((x1<x2)&&(y1<y2)))
                    {//x��y����������ͬ����ͬ������ͬ��
                        //ѡȡy1��y2�н�С������Ϊy1
                        if(y1 > y2)
                        {
                            __gk_swap(x1,x2);
                            __gk_swap(y1,y2);
                        }
                        //���¶��룬ȡ8*8���أ�yÿ�仯8���غ�x��ֱ�ߵĽ��㣬
                        //��б�������еĵ����ڵ�8*8���������Ӧ��
                        //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                        for(y = align_down(8,y1); y <= y2; y+=8)
                        {
                            x = ((((y-y1)<<1)*(x2-x1)+(y2-y1))/((y2-y1)<<1))+x1;
                            msk_x = x/8;    //����x��mskλͼ�еڼ���
                            msk_y = y/8;    //����y��mskλͼ�еڼ���
                            //��changed_msk��Ӧ��bit��1
                            __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
                            msk_x += 1;
                            //��changed_msk��Ӧ��bit��1
                            __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
                        }
                    }
                    else
                    {//x��y���������෴����x����y����x����y��
                        //ѡȡy1��y2�н�С������Ϊy1
                        if(y1 > y2)
                        {
                            __gk_swap(x1,x2);
                            __gk_swap(y1,y2);
                        }
                        //���¶��룬ȡ8*8���أ�yÿ�仯8���غ�x��ֱ�ߵĽ��㣬
                        //��б�������еĵ����ڵ�8*8���������Ӧ��
                        //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                        for(y = align_down(8,y1); y <= y2; y+=8)
                        {
                            x = ((((y-y1)<<1)*(x2-x1)-(y2-y1))/((y2-y1)<<1))+x1;
                            msk_x = x/8;    //����x��mskλͼ�еڼ���
                            msk_y = y/8;    //����y��mskλͼ�еڼ���
                            //��changed_msk��Ӧ��bit��1
                            __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
                            msk_x -= 1;
                            //��changed_msk��Ӧ��bit��1
                            __GK_SetChangeMsk(&(gkwin->changed_msk),msk_x,msk_y);
                        }
                    }
                }
            }
        }
    }
}
//----����---------------------------------------------------------
//����: �ڴ����ϻ�ֱ�ߣ��˵㲻�����봦��changed_msk����
//����: gkwin��Ŀ�괰��ָ��
//      limit�������ͼ����������һ�����ε�����,�����������ֱ�߲���
//      x1��y1��x2��y2�������յ�����
//      color����ͼ���õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_LinetoWin(struct GkWinObj *gkwin,struct Rectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 Rop2Code)
{
    if(y1 == y2)        //��ˮƽ��
        __GK_HlinetoWin(gkwin,limit,x1,y1,x2,color,Rop2Code);
    else if(x1 == x2)   //�洹ֱ��
        __GK_VlinetoWin(gkwin,limit,x1,y1,y2,color,Rop2Code);
    else                //��б��
        __GK_OlinetoWin(gkwin,limit,x1,y1,x2,y2,color,Rop2Code);
}

//----��screen�ϻ�����---------------------------------------------------------
//����: ��screen��ֱ�ӻ��㡣
//����: gkwin�����Ƶ�Ŀ�괰��
//      x��y�� ����
//      color����ͼ���õ���ɫ��CN_SYS_PF_ERGB8888��ʽ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_SetPixelScreen(struct DisplayObj *display,s32 x,s32 y,
                           u32 color,u32 Rop2Code)
{
// TODO: ����߼�
    struct DisplayObj *MirrorDisplay;
    struct Object *mirror,*current;

    display->draw.SetPixelToScreen(x,y,color,Rop2Code);//��screen�ϻ���
    mirror = display->HostObj;

    current = OBJ_GetChild(mirror);
    while(current != NULL)
    {
        MirrorDisplay = (struct DisplayObj*)OBJ_GetPrivate(current);
        MirrorDisplay->draw.SetPixelToScreen(x,y,color,Rop2Code);
        current = OBJ_ForeachChild(mirror, current);
    }
}

//----��ֱ��(�����˵�)---------------------------------------------------------
//����: ��screen��ֱ�ӻ�ֱ�ߣ��˵㲻����
//����: display�����Ƶ�Ŀ����ʾ��
//      limit�������ͼ����������һ�����ε����򣬳����������ֱ�߲���
//      x1��y1��x2��y2�������յ�����
//      color����ͼ���õ���ɫ��CN_SYS_PF_ERGB8888��ʽ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_LinetoScreen(struct DisplayObj *display,struct Rectangle *limit,
                   s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 Rop2Code)
{
    struct DisplayObj *MirrorDisplay;
    struct Object *mirror,*current;
    s32 x,y,ax,ay,s1,s2,p,interchange,i;

    if(display->draw.LineToScreen(limit, x1, y1, x2, y2, color,Rop2Code))
    {
    }else   //Ӳ�����ٲ�֧��ֱ�߻��ƣ����ʵ��
    {
        if(y1 == y2)    //��ˮƽ��
        {
            if(x1 > x2)
                __gk_swap(x1,x2);
            if((y1 < limit->top) || (y1 >= limit->bottom)
                ||(x2 <= limit->left) || (x1 >= limit->right))
                return;         //����ֱ����limit֮��
            if(x1 < limit->left)
                x1 = limit->left;
            if(x2 > limit->right)
                x2 = limit->right;
            for(x = x1; x < x2;x++)
            {
                display->draw.SetPixelToScreen(x,y1,color,Rop2Code);
            }
        }else if(x1 == x2)   //�洹ֱ��
        {
            if(y1 > y2)
                __gk_swap(y1,y2);
            if((x1 < limit->left) || (x1 >= limit->right)
                ||(y2 <= limit->top) || (y1 >= limit->bottom))
                return;         //����ֱ����limit֮��
            if(y1 < limit->top)
                y1 = limit->top;
            if(y2 > limit->bottom)
                y2 = limit->bottom;
            for(y = y1; y < y2;y++)
            {
                display->draw.SetPixelToScreen(x1,y,color,Rop2Code);
            }
        }else            //��б��
        {
            //ȡ��б����limit�Ľ��㣬����limit����Ĳ���
            if( __GK_OlineSectInter(limit,&x1,&y1,&x2,&y2))
            {
                x = x1;
                y = y1;
                ax = abs(x2-x1);
                ay = abs(y2-y1);
                if(x2 > x1)   //����б�߷���
                   s1 = 1;
                else
                   s1 = -1;
                if(y2 > y1)
                   s2 = 1;
                else
                   s2 = -1;
                if(ay > ax)
                {
                    __gk_swap(ax,ay);
                    interchange = 1;
                }
                else
                    interchange = 0;
                p = (ay<<1)-ax;   //���ó�ʼ����б�ֵ
                for(i = 1;i <= ax;i++)
                {
                    display->draw.SetPixelToScreen(x,y,color,Rop2Code);

                    if(p >= 0)     //ȡ����ѧ��Ͻ�������
                    {
                        if(interchange == 0)
                           y = y+s2;
                        else
                           x = x+s1;
                        p = p-(ax<<1);
                    }
                    if(interchange == 0)
                        x = x+s1;
                    else
                        y = y+s2;
                    p = p+(ay<<1);
                }
            }
        }
    }
    mirror = display->HostObj;
    current = OBJ_GetChild(mirror);
    while(current != NULL)
    {
        MirrorDisplay = (struct DisplayObj*)OBJ_GetPrivate(current);
        MirrorDisplay->draw.LineToScreen(limit, x1, y1, x2, y2, color,Rop2Code);
        current = OBJ_ForeachChild(mirror,current);
    }
}
//----����bitmap��screen-------------------------------------------------------
//����: ��bitmap�Ĳ��ִ��ڴ洫�͵�screen��
//����: gkwin�����Ƶ�Ŀ�괰��
//      rect����ͼ����һ�����ε����򣬳���������Ĳ��ֲ���
//      bitmap�����Ƶ�λͼ
//      x��y��bitmapҪ���Ͳ������Ͻ�����
//����: ��
//-----------------------------------------------------------------------------
void __GK_BltBmToScreen(struct DisplayObj *display,struct Rectangle *limit,
                        struct RectBitmap *bitmap,s32 x,s32 y)
{
    //todo :����alpha1,2,4,8��ʽ.
    // �ο� __GK_CopyPixelRopBm;

    u32 color;
    s32 x_bmap,y_bmap,x_rect,y_rect;

    struct DisplayObj *MirrorDisplay;
    struct Object *mirror,*current;

    if(display->draw.CopyBitmapToScreen(limit,bitmap,x,y))
    {
    }
    else//bitmap��ɫ��ʽ����ʾ����һ������Ӳ�����ٲ�֧�������ʵ��
    {
        //����bitmap��screen
        y_bmap = y;
        for(y_rect = limit->top;y_rect < limit->bottom;y_rect++)
        {
            x_bmap = x;
            for(x_rect = limit->left;x_rect < limit->right;x_rect++)
            {

                color = GK_GetPixelBm(bitmap,x_bmap,y_bmap);
                switch(bitmap->PixelFormat)
                {
                case CN_SYS_PF_GRAY1:
                case CN_SYS_PF_ALPHA1:
                        if(color!=0)
                        {
                            display->draw.SetPixelToScreen(x_rect,y_rect,
                                    bitmap->ExColor,CN_R2_COPYPEN);
                        }
                        break;
                        ////

                    default:
                         color = GK_ConvertColorToRGB24(bitmap->PixelFormat,color,
                                 bitmap->ExColor);
                         display->draw.SetPixelToScreen(x_rect,y_rect,
                                                         color,CN_R2_COPYPEN);

                }


                x_bmap++;
            }
            y_bmap++;
        }
    }
    mirror = display->HostObj;
    current = OBJ_GetChild(mirror);
    while(current != NULL)
    {
        MirrorDisplay = (struct DisplayObj*)OBJ_GetPrivate(current);
        MirrorDisplay->draw.CopyBitmapToScreen(limit,bitmap,x,y);
        current = OBJ_ForeachChild(mirror,current);
    }
}

//----������-----------------------------------------------------------------
//����: ��dst_bitmap��dst_rect��ռ�ݵ�λ���ϣ���color��ɫ��䡣color�ĸ�ʽ��
//      CN_SYS_PF_ERGB8888��Ҫת����dst_bitmap->pf_typeƥ�����ɫ��䡣
//����: dst_bitmap��������λͼ
//      dst_rect������λ��
//      color��������ɫ����ʽ��CN_SYS_PF_ERGB8888
//����: ��
//-----------------------------------------------------------------------------
void __GK_FillRect(struct RectBitmap *dst_bitmap,
                     struct Rectangle *dst_rect,u32 color)
{
    s32 x,y,x1,y1,x2,y2;
    u8 bit_offset,i;
    u8 color_bits;
    s32 byte_offset,temp_bytes;
    u32 pf,pf_color;
    u8 bit_offset1,bit_offset2,LeftColor,LeftMsk,RightColor,RightMsk,FillColor;
    s32 byte_offset1,byte_offset2;

    x1 = dst_rect->left;
    y1 = dst_rect->top;
    y2 = dst_rect->bottom;
    x2 = dst_rect->right;
    temp_bytes = dst_bitmap->linebytes;
    //color�ĸ�ʽ��CN_SYS_PF_ERGB8888��ת����dst_bitmap->pf_typeƥ��ĸ�ʽ
    pf_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,color);
    //������λͼ��ÿ����λ��
    color_bits = (dst_bitmap->PixelFormat & CN_PF_BITWIDE_MSK)>>8;
    LeftColor = 0;
    RightColor = 0;
    switch(color_bits)
    {
        case 1:
        {
            pf = pf_color & 0x01;               //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1/8;  //dst_rect������߽������ֽ�
            // -1����Ϊ�����ұ߽��ǲ������ڻ��������ڵġ�
            byte_offset2 = y1*temp_bytes+(x2-1)/8;  //dst_rect�����ұ߽������ֽ�
            bit_offset1 = x1%8;   //dst_rect��߽��������ֽڵ�λ��x=0��bit=7
            bit_offset2 = x2%8;     //7��ȥdst_rect�ұ߽��������ֽڵ�λ��7-bit
            if(pf == 1)
                FillColor = 0xff;
            else
                FillColor = 0;
            //dst_rect�����ֽ���β�ֽڲ���һ���ֽ���
            if(byte_offset1 != byte_offset2)
            {
                //dst_rectÿ�е����ֽڸ�λ����ɫ
                //���ֽ�ȡ��ɫ��λ����bit��ͬ���ұ��Ǹ�λ
                LeftColor = FillColor &(0xff >> bit_offset1);//����������ֽڵ�λ
                RightColor = FillColor << (8-bit_offset2);  //�Ҷ��������ֽڸ�λ
                LeftMsk = ~(0xff>>bit_offset1);     //����뱣����λ
                RightMsk = (0xff>>bit_offset2);     //�Ҷ��뱣����λ
                for(y = y1;y < y2;y++)  //�����ɫ
                {
                    //���dst_rectÿ���м��ֽڵ���ɫ
                    for(x = byte_offset1+1; x < byte_offset2; x++)
                    {
                        dst_bitmap->bm_bits[x] = FillColor;
                    }
                    dst_bitmap->bm_bits[byte_offset1] &= LeftMsk;
                    dst_bitmap->bm_bits[byte_offset1] |= LeftColor;  //�ֽ�ͷ��ֵ
                    dst_bitmap->bm_bits[byte_offset2] &= RightMsk;
                    dst_bitmap->bm_bits[byte_offset2] |= RightColor; //�ֽ�β��ֵ
                    //�ֽ�������
                    byte_offset1 += temp_bytes;
                    byte_offset2 += temp_bytes;
                }
            }
            else//dst_rect���ֽ���β�ֽ���һ���ֽ���
            {   //bit1����bit2���ң��ұ�Ϊ��λ���˴���bit_offset2=7-bit2
                for(i = 7-bit_offset2;i <= bit_offset1;i++)
                {
                    RightColor |= 1<<i;
                    LeftColor |= pf<<i;
                }
                LeftColor = ((u8)0xff << bit_offset1)>>bit_offset1;
                LeftColor = (LeftColor >> (8-bit_offset2))<<(8-bit_offset2);
                RightColor = LeftColor & FillColor;
                for(y = y1;y < y2;y++)
                {   //����Ŀ���ֽ��ڲ���Ҫ����λ
                    dst_bitmap->bm_bits[byte_offset1] &= ~LeftColor;
                    //�����Ӧ��λ
                    dst_bitmap->bm_bits[byte_offset1] |= RightColor;
                    byte_offset1 += temp_bytes; //�ֽ�������
                }
            }
        }break;
        case 2:
        {
            pf = pf_color & 0x03;               //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1/4;  //dst_rect������߽������ֽ�
            // -1����Ϊ�����ұ߽��ǲ������ڻ��������ڵġ�
            byte_offset2 = y1*temp_bytes+(x2-1)/4;  //dst_rect�����ұ߽������ֽ�
            bit_offset1 = (x1%4)<<1;//dst_rect��߽��������ֽڵ�λ��x=0��bit=7
            bit_offset2 = (x2%4)<<1;//7��ȥdst_rect�ұ߽��������ֽڵ�λ��7-bit
            FillColor = (u8)((pf<<6)|(pf<<4)|(pf<<2)|pf);//ÿ���м��ֽڵ���ɫ
            //dst_rect���е����ֽ���β�ֽڲ���һ���ֽ���
            if(byte_offset1 != byte_offset2)
            {   //dst_rectÿ�е����ֽڸ�λ����ɫ
                LeftColor = FillColor &(0xff >> bit_offset1);//����������ֽڵ�λ
                RightColor = FillColor << (8-bit_offset2);  //�Ҷ��������ֽڸ�λ
                LeftMsk = ~(0xff>>bit_offset1);     //����뱣����λ
                RightMsk = (0xff>>bit_offset2);     //�Ҷ��뱣����λ
                for(y = y1;y < y2;y++)  //�����ɫ
                {
                    //���dst_rectÿ���м��ֽڵ���ɫ
                    for(x = byte_offset1+1; x < byte_offset2; x++)
                    {
                        dst_bitmap->bm_bits[x] = FillColor;
                    }
                    dst_bitmap->bm_bits[byte_offset1] &= LeftMsk;
                    dst_bitmap->bm_bits[byte_offset1] |= LeftColor;  //�ֽ�ͷ��ֵ
                    dst_bitmap->bm_bits[byte_offset2] &= RightMsk;
                    dst_bitmap->bm_bits[byte_offset2] |= RightColor;    //�ֽ�β��ֵ
                    //�ֽ�������
                    byte_offset1 += temp_bytes;
                    byte_offset2 += temp_bytes;
                }
            }
            else//dst_rect���е����ֽ���β�ֽ���һ���ֽ���
            {
                //����������ֽڸ�λ
                for(i = 6-bit_offset2;i <= bit_offset1;i = i+2)
                {
                    RightColor |= 0x3<<i;
                    LeftColor |= pf<<i;
                }
                LeftColor = ((u8)0xff << bit_offset1)>>bit_offset1;
                LeftColor = (LeftColor >> (8-bit_offset2))<<(8-bit_offset2);
                RightColor = LeftColor & FillColor;
                for(y = y1;y < y2;y++)
                {   //����Ŀ���ֽ��ڲ���Ҫ����λ
                    dst_bitmap->bm_bits[byte_offset1] &= ~LeftColor;
                    //�����Ӧ��λ
                    dst_bitmap->bm_bits[byte_offset1] |= RightColor;
                    byte_offset1 += temp_bytes; //�ֽ�������
                }
            }
        }break;
        case 4:
        {
            pf = pf_color & 0xf;                //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1/2;  //dst_rect������߽������ֽ�
            // -1����Ϊ�����ұ߽��ǲ������ڻ��������ڵġ�
            byte_offset2 = y1*temp_bytes+(x2-1)/2;  //dst_rect�����ұ߽������ֽ�
            bit_offset1 = (x1%2)<<2;//dst_rect��߽��������ֽڵ�λ��x=0��bit=7
            bit_offset2 = (x2%2)<<2;//7��ȥdst_rect�ұ߽��������ֽڵ�λ��7-bit
            FillColor = (u8)((pf<<4)|pf);     //ÿ���м��ֽڵ���ɫ
            //dst_rect���е����ֽ���β�ֽڲ���һ���ֽ���
            if(byte_offset1 != byte_offset2)
            {
                LeftColor = FillColor &(0xff >> bit_offset1);//����������ֽڵ�λ
                RightColor = FillColor << (8-bit_offset2);  //�Ҷ��������ֽڸ�λ
                LeftMsk = ~(0xff>>bit_offset1);     //����뱣����λ
                RightMsk = (0xff>>bit_offset2);     //�Ҷ��뱣����λ
                for(y = y1;y < y2;y++)          //�����ɫ
                {
                    //���dst_rectÿ���м��ֽڵ���ɫ
                    for(x = byte_offset1+1; x < byte_offset2; x++)
                    {
                        dst_bitmap->bm_bits[x] = FillColor;
                    }
                    dst_bitmap->bm_bits[byte_offset1] &= LeftMsk;
                    dst_bitmap->bm_bits[byte_offset1] |= LeftColor;  //�ֽ�ͷ��ֵ
                    dst_bitmap->bm_bits[byte_offset2] &= RightMsk;
                    dst_bitmap->bm_bits[byte_offset2] |= RightColor;    //�ֽ�β��ֵ
                    //�ֽ�������
                    byte_offset1 += temp_bytes;
                    byte_offset2 += temp_bytes;
                }
            }
            else//dst_rect���е����ֽ���β�ֽ���һ���ֽ���
            {   //��λ�����
                if(x1 & 1)
                {
                    LeftMsk = 0xf0;
                    LeftColor = FillColor >> 4;
                }
                else
                    LeftColor = FillColor << 4;
                if((x2-1) & 1)
                    LeftColor |= FillColor >> 4;
                else
                {
                    LeftMsk = 0x0f;
                    LeftColor &= FillColor << 4;
                }

                for(y = y1;y < y2;y++)
                {   //����Ŀ���ֽ��ڲ���Ҫ����λ
                    dst_bitmap->bm_bits[byte_offset1] &= LeftMsk;
                    //�����Ӧ��λ
                    dst_bitmap->bm_bits[byte_offset1] |= LeftColor;
                    byte_offset1 += temp_bytes; //�ֽ�������
                }
            }
        }break;
        case 8:
        {
            pf = pf_color;                      //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1;    //dst_rect������߽������ֽ�
            byte_offset2 = y1*temp_bytes+x2-1;    //dst_rect�����ұ߽������ֽ�
            for(y = y1;y < y2;y++)      //�����ɫ��ÿ�����һ������
            {
                //���dst_rectÿ���м��ֽڵ���ɫ
                for(x = byte_offset1;x <= byte_offset2;x++)
                {
                    dst_bitmap->bm_bits[x] = (u8)pf;
                }
                //�ֽ�������
                byte_offset1 += temp_bytes;
                byte_offset2 += temp_bytes;
             }
        }break;
        case 12:
        {
            pf = pf_color;           //ȡ�����������ɫ
            for(y = y1;y < y2;y++)  //�����ɫ
            {
                temp_bytes = y*temp_bytes;              //y�е��ֽ�ƫ����
                for(x = x1;x < x2;x++)
                {
                    byte_offset = temp_bytes + x*3/2;   //Ŀ�����������ֽ�
                    bit_offset = (x*12)%8;              //Ŀ�����ص�λƫ����
                    if(bit_offset == 0)
                    {
                        dst_bitmap->bm_bits[byte_offset] = (u8)(pf&0xff);
                        dst_bitmap->bm_bits[byte_offset+1] &= 0xf0;
                        dst_bitmap->bm_bits[byte_offset+1] |= (u8)(pf>>8);
                    }else
                    {
                        dst_bitmap->bm_bits[byte_offset] &= 0x0f;
                        dst_bitmap->bm_bits[byte_offset] |= (u8)pf<<4;
                        dst_bitmap->bm_bits[byte_offset+1] = (u8)(pf>>4);
                    }
                }
             }
        }break;
        case 16:
        {
            pf = pf_color;                       //ȡ�����������ɫ
            if((u32)dst_bitmap->bm_bits & 1 )      //�Ƕ����ַ
            {
                byte_offset1 = y1*temp_bytes+x1*2;  //dst_rect������߽������ֽ�
                byte_offset2 = y1*temp_bytes+(x2-1)*2;  //dst_rect�����ұ߽������ֽ�
                for(y = y1;y < y2;y++)              //�����ɫ��ÿ�������������
                {
                    for(x = byte_offset1;x <= byte_offset2;x = x+2)
                    {
    //                    alpha = pf;
    //                    if(dst_bitmap->PixelFormat == CN_SYS_PF_ARGB1555)
    //                    {
    //                        pf &= 0x7fff;
    //                        pf |= alpha & 0x8000;
    //                    }
                        //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
                        dst_bitmap->bm_bits[x] = (u8)pf;
                        dst_bitmap->bm_bits[x+1] = (u8)(pf>>8);
                    }
                    //�ֽ�������
                    byte_offset1 += temp_bytes;
                    byte_offset2 += temp_bytes;
                }
            }
            else                        //����ĵ�ַ
            {
                u16 *vm;
                vm = (u16*)((u32)dst_bitmap->bm_bits+y1*dst_bitmap->linebytes);
                for(y = y1;y < y2;y++)              //�����ɫ��ÿ�������������
                {
                    for(x = x1;x < x2;x++)
                    {
    //                    alpha = pf;
    //                    if(dst_bitmap->PixelFormat == CN_SYS_PF_ARGB1555)
    //                    {
    //                        pf &= 0x7fff;
    //                        pf |= alpha & 0x8000;
    //                    }
                        vm[x] = pf;
                    }
                    vm += dst_bitmap->linebytes>>1;
                }

            }
        }break;
        case 24:
        {
            pf = pf_color;                      //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1*3;  //dst_rect������߽������ֽ�
            byte_offset2 = y1*temp_bytes+(x2-1)*3;  //dst_rect�����ұ߽������ֽ�
            for(y = y1;y < y2;y++)              //�����ɫ��ÿ�����һ������
            {
                for(x = byte_offset1;x <= byte_offset2;x = x+3)
                {
                    dst_bitmap->bm_bits[x] = (u8)pf;
                    dst_bitmap->bm_bits[x+1] = (u8)(pf >> 8);
                    dst_bitmap->bm_bits[x+2] = (u8)(pf >> 16);
                }
                //�ֽ�������
                byte_offset1 += temp_bytes;
                byte_offset2 += temp_bytes;
             }
        }break;
        case 32:
        {
            pf = pf_color;                      //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1*4;  //dst_rect������߽������ֽ�
            byte_offset2 = y1*temp_bytes+(x2-1)*4;  //dst_rect�����ұ߽������ֽ�
            for(y = y1;y < y2;y++)              //�����ɫ��ÿ�����һ������
            {
                for(x = byte_offset1;x <= byte_offset2;x = x+4)
                {
                    pf = pf_color;
//                    alpha = pf;
//                    if(dst_bitmap->PixelFormat == CN_SYS_PF_ARGB8888)
//                    {
//                        pf &= 0x00ffffff;
//                        pf |= alpha & 0xff000000;
//                    }
                    //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����32λָ�롣
                    dst_bitmap->bm_bits[x] = (u8)pf;
                    dst_bitmap->bm_bits[x+1] = (u8)(pf >> 8);
                    dst_bitmap->bm_bits[x+2] = (u8)(pf >> 16);
                    dst_bitmap->bm_bits[x+3] = (u8)(pf >> 24);
                }
                //�ֽ�������
                byte_offset1 += temp_bytes;
                byte_offset2 += temp_bytes;
             }
        }break;
    }
}

//----���λͼ-----------------------------------------------------------------
//����: ��������__GK_FillRect���������������Ҫ���ľ���������bitmap��
//����: dst_bitmap,������λͼ
//      color��������ɫ����ʽ��CN_SYS_PF_ERGB8888
//����: ��
//-----------------------------------------------------------------------------
void __GK_FillBm(struct RectBitmap *dst_bitmap,u32 color)
{
    u32 pf_color,color_bits;
    u32 byte_offset,total_bytes;

    //color�ĸ�ʽ��CN_SYS_PF_ERGB8888��ת����dst_bitmap->pf_typeƥ��ĸ�ʽ
    pf_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,color);
    //������λͼ��ÿ����λ��
    color_bits = (dst_bitmap->PixelFormat & CN_PF_BITWIDE_MSK)>>8;
    switch(color_bits)
    {
        case 1:
        {//�����ɫ��ֱ�ӽ���Ӧ��λ����
            if(1 == pf_color)
                memset( dst_bitmap->bm_bits,0xff,
                        dst_bitmap->linebytes*dst_bitmap->height);
            else
                memset( dst_bitmap->bm_bits,0,
                        dst_bitmap->linebytes*dst_bitmap->height);
        }break;
        case 2:
        {//�����ɫ��ֱ�ӽ���Ӧ��λ����
            memset( dst_bitmap->bm_bits,
                    pf_color + (pf_color<<2) + (pf_color<<4) + (pf_color<<6),
                    dst_bitmap->linebytes*dst_bitmap->height);
        }break;
        case 4:
        {//�����ɫ��ֱ�ӽ���Ӧ��λ����
            memset( dst_bitmap->bm_bits,(u8)(pf_color|(pf_color<<4)),
                    dst_bitmap->linebytes*dst_bitmap->height);
        }break;
        case 8:
        {//ֱ�ӽ���Ӧ��λ����
            memset( dst_bitmap->bm_bits,(u8)pf_color,
                    dst_bitmap->linebytes*dst_bitmap->height);
        }break;
        case 12:
        {   //�ܹ���Ҫ�����ֽ���
            total_bytes = dst_bitmap->linebytes*dst_bitmap->height;
            //12λ����ɫ��ʽ��һ�ο����3���ֽڣ�����������
            for(byte_offset = 0;byte_offset < total_bytes;)
            {
                dst_bitmap->bm_bits[byte_offset] = (u8)(pf_color&0xff);
                dst_bitmap->bm_bits[byte_offset+1] = (u8)((pf_color>>8)&0xf);
                dst_bitmap->bm_bits[byte_offset+1] |= (u8)((pf_color&0xf)<<4);
                dst_bitmap->bm_bits[byte_offset+2] = (u8)(pf_color>>4);
                byte_offset += 3;
            }
        }break;
        case 16:
        {   //�ܹ���Ҫ�����ֽ���
            total_bytes = dst_bitmap->linebytes*dst_bitmap->height;
            if((u32)dst_bitmap->bm_bits & 1 )
            {
                for(byte_offset = 0;byte_offset < total_bytes;byte_offset += 2)
                {
    //                alpha = pf_color;
    //                if(dst_bitmap->PixelFormat == CN_SYS_PF_ARGB1555)
    //                {
    //                    pf_color &= 0x7fff;
    //                    pf_color |= alpha & 0x8000;
    //                }
                    //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
                    //һ��дһ�����أ������ֽ�
                    dst_bitmap->bm_bits[byte_offset] = (u8)pf_color;
                    dst_bitmap->bm_bits[byte_offset+1] = (u8)(pf_color>>8);
                }
            }
            else
            {
                u16 *vm;
                vm = (u16*)(u32)dst_bitmap->bm_bits;
                for(byte_offset = 0;byte_offset < total_bytes>>1;byte_offset++)
                {
                    vm[byte_offset] = pf_color;
                }
            }
        }break;
        case 24:
        {   //�ܹ���Ҫ�����ֽ���
            total_bytes = dst_bitmap->linebytes*dst_bitmap->height;
            for(byte_offset = 0;byte_offset < total_bytes;)
            {
                dst_bitmap->bm_bits[byte_offset] = (u8)pf_color;
                dst_bitmap->bm_bits[byte_offset+1] = (u8)(pf_color >> 8);
                dst_bitmap->bm_bits[byte_offset+2] = (u8)(pf_color >> 16);
                byte_offset += 3;   //һ��дһ�����أ������ֽ�
             }
        }break;
        case 32:
        {   //�ܹ���Ҫ�����ֽ���
            total_bytes = dst_bitmap->linebytes*dst_bitmap->height;
            for(byte_offset = 0;byte_offset < total_bytes;)
            {
//                alpha = pf_color;
//                if(dst_bitmap->PixelFormat == CN_SYS_PF_ARGB8888)
//                {
//                    pf_color &= 0x00ffffff;
//                    pf_color |= alpha & 0xff000000;
//                }
                //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����32λָ�롣
                dst_bitmap->bm_bits[byte_offset] = (u8)pf_color;
                dst_bitmap->bm_bits[byte_offset+1] = (u8)(pf_color >> 8);
                dst_bitmap->bm_bits[byte_offset+2] = (u8)(pf_color >> 16);
                dst_bitmap->bm_bits[byte_offset+3] = (u8)(pf_color >> 24);
                byte_offset += 4;   //һ��дһ�����أ��ĸ��ֽ�
             }
        }break;
    }
}
//----����8�����ص�------------------------------------------------------------
//����: ����8�����ص㣬�ڻ�Բ������µ��ã�������Բ�İ˷����ԣ�
//      ͬʱ��־changed_msk��
//����: gkwin��Ŀ�괰��
//      bitmap��Ŀ��λͼ
//      x0��y0��Բ������
//      x��y��Ҫ���Ƶĵ�����꣬�����Բ��
//      color����������ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_set_all_pixels_circle(struct GkWinObj *gkwin,
                                struct RectBitmap *bitmap,s32 x0,s32 y0,
                                s32 x,s32 y,u32 color,u32 Rop2Code)
{
    //����8�����ص�
    __GK_SetPixelRop2Bm(bitmap,x0+x,y0+y,color,Rop2Code);
    __GK_SetPixelRop2Bm(bitmap,x0+x,y0-y,color,Rop2Code);
    __GK_SetPixelRop2Bm(bitmap,x0-x,y0+y,color,Rop2Code);
    __GK_SetPixelRop2Bm(bitmap,x0-x,y0-y,color,Rop2Code);
    __GK_SetPixelRop2Bm(bitmap,x0+y,y0+x,color,Rop2Code);
    __GK_SetPixelRop2Bm(bitmap,x0+y,y0-x,color,Rop2Code);
    __GK_SetPixelRop2Bm(bitmap,x0-y,y0+x,color,Rop2Code);
    __GK_SetPixelRop2Bm(bitmap,x0-y,y0-x,color,Rop2Code);
    //��8�����ص�ֱ����changed_msk��־
    __GK_SetChangeMsk(&(gkwin->changed_msk),(x0+x)/8,(y0+y)/8);
    __GK_SetChangeMsk(&(gkwin->changed_msk),(x0+x)/8,(y0-y)/8);
    __GK_SetChangeMsk(&(gkwin->changed_msk),(x0-x)/8,(y0+y)/8);
    __GK_SetChangeMsk(&(gkwin->changed_msk),(x0-x)/8,(y0-y)/8);
    __GK_SetChangeMsk(&(gkwin->changed_msk),(x0+y)/8,(y0+x)/8);
    __GK_SetChangeMsk(&(gkwin->changed_msk),(x0+y)/8,(y0-x)/8);
    __GK_SetChangeMsk(&(gkwin->changed_msk),(x0-y)/8,(y0+x)/8);
    __GK_SetChangeMsk(&(gkwin->changed_msk),(x0-y)/8,(y0-x)/8);
}
//----����Ŀ��㲢��ɫ---------------------------------------------------------
//����: ����Ŀ��㣬ͬʱ��־changed_msk��
//����: gkwin��Ŀ�괰��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      bitmap��Ŀ��λͼ
//      x��y��Ҫ���Ƶĵ������
//      color����������ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_pixel_sect_inter(struct GkWinObj *gkwin,struct Rectangle *limit,
                                struct RectBitmap *bitmap,s32 x,s32 y,
                                u32 color,u32 Rop2Code)
{
    if((x >= limit->left)&&(x < limit->right)
        &&(y >= limit->top)&&(y < limit->bottom))
    {//�����Ƶ�Ŀ������������ڣ����㲢��־changed_msk
        __GK_SetPixelRop2Bm(bitmap,x,y,color,Rop2Code);
        __GK_SetChangeMsk(&(gkwin->changed_msk),x/8,y/8);
    }
    else    //�����Ƶ�Ŀ������������⣬ֱ�ӷ���
        return;
}
//----���ƶ��Ŀ��㲢��־-----------------------------------------------------
//����: ���ƶ��Ŀ��㣬ͬʱ��־changed_msk��
//����: gkwin��Ŀ�괰��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      bitmap��Ŀ��λͼ
//      x0��y0��Բ������
//      x��y��Ҫ���Ƶĵ������
//      color����������ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_set_part_pixels_circle(struct GkWinObj *gkwin,struct Rectangle *limit,
                                struct RectBitmap *bitmap,s32 x0,s32 y0,
                                s32 x,s32 y,u32 color,u32 Rop2Code)
{
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0+x,y0+y,color,Rop2Code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0+x,y0-y,color,Rop2Code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0-x,y0+y,color,Rop2Code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0-x,y0-y,color,Rop2Code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0+y,y0+x,color,Rop2Code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0+y,y0-x,color,Rop2Code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0-y,y0+x,color,Rop2Code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0-y,y0-x,color,Rop2Code);
}
//----��Բ---------------------------------------------------------------------
//����: �ڴ����ڻ�һ��Բ��
//����: gkwin��Ŀ�괰��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x0��y0��Բ������
//      r��Բ�İ뾶
//      color����������ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//˵��: ʹ���е㻭Բ�㷨
//-----------------------------------------------------------------------------
void __GK_DrawCircleBm(struct GkWinObj *gkwin,struct Rectangle *limit,
                        s32 x0,s32 y0,s32 r,u32 color,u32 Rop2Code)
{
    s32 x,y,e;
    s32 left,right,top,bottom;
    u32 pf_color,flag;
    struct RectBitmap *bitmap;

    //ѡȡ(x,y)Ϊ(0,r)��Ϊ��ʼ����(����ʵ�ʵ�Բ��ƽ�Ƶ�ԭ���Բ)��Բ�ϵ������
    x = 0;
    y = r;
    e = 1-r;
    left = limit->left-x0;      //������߽絽Բ��x����ľ���
    top = limit->top-y0;        //������߽絽Բ��y����ľ���
    right = limit->right-x0;    //�����ұ߽絽Բ��x����ľ���
    bottom = limit->bottom-y0;  //�����ұ߽絽Բ��y����ľ���
    bitmap = gkwin->wm_bitmap;
    //Ҫ���������ɫ����ɫ��ʽΪ24λ
    //��������ǰ�轫24λ����Ϊ����ʾ��һ�µ���ɫ��ʽ
    pf_color = GK_ConvertRGB24ToPF(gkwin->disp->pixel_format,color);

    //����Բ����limit��
    if((x0-limit->left >= r)&&(limit->right-x0 > r)
        &&(y0-limit->top >= r)&&(limit->bottom-y0 > r))
        flag = 1;
    //Բ�ϵĵ㲻����limit�ڣ�������Բ�ڴ�����Ͳ����ڴ������������
    else if((left*left+top*top < r*r)
        &&(right*right+top*top < r*r)
        &&(left*left+bottom*bottom < r*r)
        &&(right*right+bottom*bottom < r*r))
        return;
    //Բ�ϵĵ㲿����limit��
    else
        flag = 0;
    if(flag)
    {//����Բ����limit�ڣ�������ķ�֮һ��Բ������
     //���ݶԳ��ԣ��ɵ�Բ���������е������
        while(x <= y)
        {
            //�����жϣ�����8�����أ�ͬʱ��־���ص�changed_msk
            __gk_set_all_pixels_circle(gkwin,bitmap,x0,y0,x,y,
                                                pf_color,Rop2Code);
            if(e < 0)
                e += x*2+3;
            else
            {
                e += 2*(x-y)+5;
                y--;
            }
            x++;
        }
    }
    else
    {//Բ�ϲ��ֵ���limit�ڣ���Ҫ��Բ�ϸ������Ƿ���limit�������ж�
        while(x <= y)
        {
            //�ж�Ҫ���Ƶ��λ�ú󣬾����Ƿ���ƣ����Ƶ�����ͬʱ��־changed_msk
            __gk_set_part_pixels_circle(gkwin,limit,bitmap,x0,y0,x,y,
                                                pf_color,Rop2Code);
            if(e < 0)
                e += x*2+3;
            else
            {
                e += 2*(x-y)+5;
                y--;
            }
            x++;
        }
    }
    if(gkwin->WinProperty.ChangeFlag != CN_GKWIN_CHANGE_ALL)
        //��һ��Բֻ�ı��˴��ڵĲ�������
        gkwin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_PART;
}
//----Ŀ���Ļ���-------------------------------------------------------------
//����: ��Ŀ������ڵ�λ�ã������Ƿ���Ҫ���ơ�
//����: gkwin��Ŀ�괰��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x��y��Ŀ��������
//      color����Բʹ�õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __Gk_PixelSectInterScreen(struct DisplayObj *display,
                                    struct Rectangle *limit,
                                    s32 x,s32 y,u32 color,u32 Rop2Code)
{
    if((x >= limit->left)&&(x < limit->right)
        &&(y >= limit->top)&&(y < limit->bottom))
    //Ŀ�����limit�ڣ���Ҫ����
        display->draw.SetPixelToScreen(x,y,color,Rop2Code);
    else
        return;
}

//----��screen�ϻ�Բ-----------------------------------------------------------
//����: ��screen�ϻ�һ��Բ��
//����: gkwin��Ŀ�괰��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x0��y0��Բ������
//      r��Բ�İ뾶
//      color����Բʹ�õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//ʹ���е㻭Բ�㷨
//-----------------------------------------------------------------------------
void __GK_DrawCircleScreen(struct DisplayObj *display,struct Rectangle *limit,
                        s32 x0,s32 y0,s32 r,u32 color,u32 Rop2Code)//ȷ��
{
    s32 x,y,e;
    s32 left,right,top,bottom;
    u32 flag;
    struct DisplayObj *MirrorDisplay;
    struct Object *mirror,*current;
    struct DispDraw *mydraw;

    mydraw = &display->draw;
    //ѡȡ(x,y)Ϊ(0,r)��Ϊ��ʼ����(����ʵ�ʵ�Բ��ƽ�Ƶ�ԭ���Բ)��Բ�ϵ������
    x = 0;
    y = r;
    e = 1-r;
    left = x0 - limit->left;
    top = y0 - limit->top;
    right = limit->right-x0;
    bottom = limit->bottom-y0;
//    pf_color = GK_ConvertRGB24ToPF(display->pixel_format,color);

    //����Բ����limit��
    if((x0-limit->left >= r)&&(limit->right-x0 > r)
        &&(y0-limit->top >= r)&&(limit->bottom-y0 > r))
        flag = 1;

    //Բ�ϵĵ㶼����limit��
    else if((left*left+top*top < r*r)
        &&(right*right+top*top < r*r)
        &&(left*left+bottom*bottom < r*r)
        &&(right*right+bottom*bottom < r*r))
        return;
    //Բ�ϵĵ㲿����limit��
    else
        flag = 0;

    if(flag)
    {//����Բ����limit�ڣ�����Բ�İ˷����ԣ�ֻ��Ҫ����˷�֮һ��Բ������
        while(x <= y)
        {
            mydraw->SetPixelToScreen(x0+x,y0+y,color,Rop2Code);
            mydraw->SetPixelToScreen(x0+x,y0-y,color,Rop2Code);
            mydraw->SetPixelToScreen(x0-x,y0+y,color,Rop2Code);
            mydraw->SetPixelToScreen(x0-x,y0-y,color,Rop2Code);
            mydraw->SetPixelToScreen(x0+y,y0+x,color,Rop2Code);
            mydraw->SetPixelToScreen(x0+y,y0-x,color,Rop2Code);
            mydraw->SetPixelToScreen(x0-y,y0+x,color,Rop2Code);
            mydraw->SetPixelToScreen(x0-y,y0-x,color,Rop2Code);
            if(e < 0)
                e += x*2+3;
            else
            {
                e += 2*(x-y)+5;
                y--;
            }
            x++;
        }
    }
    else
    {//Բ�ϵĵ㲿����limit�ڣ���Ҫ����Բ�ϸ������Ƿ���limit��
        while(x <= y)
        {
            __Gk_PixelSectInterScreen(display,limit,
                                x0+x,y0+y,color,Rop2Code);
            __Gk_PixelSectInterScreen(display,limit,
                                x0+x,y0-y,color,Rop2Code);
            __Gk_PixelSectInterScreen(display,limit,
                                x0-x,y0+y,color,Rop2Code);
            __Gk_PixelSectInterScreen(display,limit,
                                x0-x,y0-y,color,Rop2Code);
            __Gk_PixelSectInterScreen(display,limit,
                                x0+y,y0+x,color,Rop2Code);
            __Gk_PixelSectInterScreen(display,limit,
                                x0+y,y0-x,color,Rop2Code);
            __Gk_PixelSectInterScreen(display,limit,
                                x0-y,y0+x,color,Rop2Code);
            __Gk_PixelSectInterScreen(display,limit,
                                x0-y,y0-x,color,Rop2Code);
            if(e < 0)
                e += x*2+3;
            else
            {
                e += 2*(x-y)+5;
                y--;
            }
            x++;
        }
    }
    //������ʾ
    mirror = display->HostObj;
    current = OBJ_GetChild(mirror);
    while(current != NULL)
    {//���ھ�����ʾ��
        x = 0;
        y = r;
        MirrorDisplay = (struct DisplayObj*)OBJ_GetPrivate(current);
        if(flag)
        {//����Բ����limit�ڣ�����Բ�İ˷����ԣ�ֻ��Ҫ����˷�֮һ��Բ������
            while(x <= y)
            {
                MirrorDisplay->draw.SetPixelToScreen(x0+x,y0+y,color,Rop2Code);
                MirrorDisplay->draw.SetPixelToScreen(x0+x,y0-y,color,Rop2Code);
                MirrorDisplay->draw.SetPixelToScreen(x0-x,y0+y,color,Rop2Code);
                MirrorDisplay->draw.SetPixelToScreen(x0-x,y0-y,color,Rop2Code);
                MirrorDisplay->draw.SetPixelToScreen(x0+y,y0+x,color,Rop2Code);
                MirrorDisplay->draw.SetPixelToScreen(x0+y,y0-x,color,Rop2Code);
                MirrorDisplay->draw.SetPixelToScreen(x0-y,y0+x,color,Rop2Code);
                MirrorDisplay->draw.SetPixelToScreen(x0-y,y0-x,color,Rop2Code);
                if(e < 0)
                    e += x*2+3;
                else
                {
                    e += 2*(x-y)+5;
                    y--;
                }
                x++;
            }
        }
        else
        {//Բ�ϵĵ㲿����limit�ڣ���Ҫ����Բ�ϸ������Ƿ���limit��
            while(x <= y)
            {
                //ֻ������limit�ڵ�����
                if((x0+x >= limit->left)&&(x0+x < limit->right)
                        &&(y0+y >= limit->top)&&(y0+y < limit->bottom))
                    MirrorDisplay->draw.SetPixelToScreen(x0+x,y0+y,color,Rop2Code);
                if((x0+x >= limit->left)&&(x0+x < limit->right)
                        &&(y0-y >= limit->top)&&(y0+y < limit->bottom))
                    MirrorDisplay->draw.SetPixelToScreen(x0+x,y0-y,color,Rop2Code);
                if((x0-x >= limit->left)&&(x0-x < limit->right)
                        &&(y0+y >= limit->top)&&(y0+y < limit->bottom))
                    MirrorDisplay->draw.SetPixelToScreen(x0-x,y0+y,color,Rop2Code);
                if((x0-x >= limit->left)&&(x0-x < limit->right)
                        &&(y0-y >= limit->top)&&(y0-y < limit->bottom))
                    MirrorDisplay->draw.SetPixelToScreen(x0-x,y0-y,color,Rop2Code);
                if((x0+y >= limit->left)&&(x0+y < limit->right)
                        &&(y0+x >= limit->top)&&(y0+x < limit->bottom))
                    MirrorDisplay->draw.SetPixelToScreen(x0+y,y0+x,color,Rop2Code);
                if((x0+y >= limit->left)&&(x0+y < limit->right)
                        &&(y0-x >= limit->top)&&(y0-x < limit->bottom))
                    MirrorDisplay->draw.SetPixelToScreen(x0+y,y0-x,color,Rop2Code);
                if((x0-y >= limit->left)&&(x0-y < limit->right)
                        &&(y0+x >= limit->top)&&(y0+x < limit->bottom))
                    MirrorDisplay->draw.SetPixelToScreen(x0-y,y0+x,color,Rop2Code);
                if((x0-y >= limit->left)&&(x0-y < limit->right)
                        &&(y0-x >= limit->top)&&(y0-x < limit->bottom))
                    MirrorDisplay->draw.SetPixelToScreen(x0-y,y0-x,color,Rop2Code);
                if(e < 0)
                    e += x*2+3;
                else
                {
                    e += 2*(x-y)+5;
                    y--;
                }
                x++;
            }
        }
        current = OBJ_ForeachChild(mirror,current);
    }
}
//----��Բ---------------------------------------------------------------------
//����: �ڴ����ڻ�һ��Բ��
//����: gkwin��Ŀ�괰��
//      x0��y0��Բ�����꣬����ڴ���
//      r��Բ�İ뾶
//      color����Բʹ�õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_DrawCircle(struct GkscParaDrawCircle *para)//ȷ��
{
    struct Rectangle limit,limitclip;
    struct ClipRect *clip;
    struct GkWinObj *fb_gkwin,*cirwin;
//    struct DispDraw *my_draw_fun;
    s32 offsetx,offsety;
    if((para->Rop2Code == CN_R2_NOP) || (para->Rop2Code > CN_R2_LAST))
        return;                                         //��ִ�в���
    if(para->r <= 0)                                         //�������
        return;
    cirwin = para->gkwin;
    if(para->range.left == para->range.right)
    {
        limit = cirwin->limit;
    }
    else
    {
        __GK_GetRectInts(&para->range, &cirwin->limit, &limit);
    }
//    my_draw_fun = &cirwin->disp->draw;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((cirwin->wm_bitmap!=NULL)&&(cirwin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
    {   //������:��win buffer�л�ͼ����־changed_msk
//      limit.left = 0;
//      limit.top = 0;
//      limit.right = cirwin->wm_bitmap->width;
//      limit.bottom = cirwin->wm_bitmap->height;
        //��Ӳ�����ٲ�֧��Բ�Ļ��ƿ��ǣ������ʵ��
        __GK_DrawCircleBm(cirwin,&limit,para->x0,para->y0,para->r,
                para->color,para->Rop2Code);
    }
    else       //��win buffer����ֱ��д������Ϊtrue
    {
        clip = cirwin->visible_clip;
        if(clip == NULL)
            return ;
        fb_gkwin = cirwin->disp->frame_buffer;
        offsetx = cirwin->ScreenX;
        offsety = cirwin->ScreenY;
        //��frame buffer����ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (cirwin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            do{
                __GK_GetRectInts(&limit, &clip->rect, &limitclip);
                //��Ӳ�����ٲ�֧��Բ�Ļ��ƿ��ǣ������ʵ��
                __GK_DrawCircleBm(fb_gkwin,&limitclip,
                                    para->x0+offsetx,para->y0+offsety,
                                    para->r,para->color,para->Rop2Code);
                clip = clip->next;
            }while(clip != cirwin->visible_clip);
        }else
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{
                __GK_GetRectInts(&limit, &clip->rect, &limitclip);
                __GK_DrawCircleScreen(cirwin->disp,&limitclip,
                            para->x0+offsetx,para->y0+offsety,para->r,
                            para->color,para->Rop2Code);
                clip = clip->next;
            }while(clip != cirwin->visible_clip);
        }
    }
}

//----������������-------------------------------------------------------------
//����: �ڴ����ϻ����������ߡ�
//����: gkwin��Ŀ�괰��
//      limit�������ͼ����������һ�����ε����򣬳����������ֱ�߲���
//      x1��y1��x2��y2��x3��y3��x4��y4���˵�����
//      color����ͼ���õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_BezierBm(struct GkWinObj *gkwin,struct Rectangle *limit,
                    float x1,float y1,float x2,float y2,float x3,float y3,
                    float x4,float y4,u32 color,u32 Rop2Code)
{
    float a,b,c;
    float x12,y12,x23,y23,x34,y34;
    float x1223,y1223,x2334,y2334;
    float x,y,ab;

    //a��b��c��ֱ��(x1,y1)��(x4,y4)��һ��ʽax+by+c=0ʱ��Ӧ�ĸ�������
    a = y4-y1;
    b = x1-x4;
    c = y1*(x4-x1)-x1*(y4-y1);
    ab = a*a+b*b;

    //��(x2,y2)��(x3,y3)��ֱ��(x1,y1)��(x4,y4)�ľ���С��1(������)��
    //����ֱ�ߴ�������
    if(((a*x2+b*y2+c)*(a*x2+b*y2+c) < ab) &&((a*x3+b*y3+c)*(a*x3+b*y3+c) < ab))
    {
        __GK_LinetoWin(gkwin,limit,(s32)x1,(s32)y1,(s32)x4,(s32)y4,
                                color,Rop2Code);
        return;
    }
    x12 = x1+x2;
    y12 = y1+y2;
    x23 = x2+x3;
    y23 = y2+y3;
    x34 = x3+x4;
    y34 = y3+y4;

    x1223 = x12+x23;
    y1223 = y12+y23;
    x2334 = x23+x34;
    y2334 = y23+y34;

    x = x1223+x2334;
    y = y1223+y2334;

    //�������м��������Ƶ㵽�Զ˵�Ϊ���յ��ֱ�ߵľ���С��1(������)ʱ��
    //�����ߵ��е㴦�ָ�����
    __GK_BezierBm(gkwin,limit,x1,y1,x12/2,y12/2,
                    x1223/4,y1223/4,x/8,y/8,color,Rop2Code);
    __GK_BezierBm(gkwin,limit,x/8,y/8,x2334/4,y2334/4,
                    x34/2,y34/2,x4,y4,color,Rop2Code);
}

//----��screen�ϻ�����������---------------------------------------------------
//����: ��screen�ϻ����������ߡ�
//����: gkwin��Ŀ�괰��
//      limit�������ͼ����������һ�����ε����򣬳����������ֱ�߲���
//      x1��y1��x2��y2��x3��y3��x4��y4���˵�����
//      color����ͼ���õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_BezierScreen(struct DisplayObj *display,struct Rectangle *limit,
                    float x1,float y1,float x2,float y2,float x3,float y3,
                    float x4,float y4,u32 color,u32 Rop2Code)
{
    float a,b,c;
    float x12,y12,x23,y23,x34,y34;
    float x1223,y1223,x2334,y2334;
    float x,y,ab;

    //a��b��c��ֱ��(x1,y1)��(x4,y4)��һ��ʽax+by+c=0ʱ��Ӧ�ĸ�������
    a = y4-y1;
    b = x1-x4;
    c = y1*(x4-x1)-x1*(y4-y1);
    ab = a*a+b*b;

    //��(x2,y2)��(x3,y3)��ֱ��(x1,y1)��(x4,y4)�ľ���С��1(������)��
    //����ֱ�ߴ�������
    if(((a*x2+b*y2+c)*(a*x2+b*y2+c) < ab) &&((a*x3+b*y3+c)*(a*x3+b*y3+c) < ab))
    {
        __GK_LinetoScreen(display,limit,(s32)x1,(s32)y1,(s32)x4,(s32)y4,
                           color,Rop2Code);
        return;
    }
    x12 = x1+x2;
    y12 = y1+y2;
    x23 = x2+x3;
    y23 = y2+y3;
    x34 = x3+x4;
    y34 = y3+y4;

    x1223 = x12+x23;
    y1223 = y12+y23;
    x2334 = x23+x34;
    y2334 = y23+y34;

    x = x1223+x2334;
    y = y1223+y2334;

    //�������м��������Ƶ㵽�Զ˵�Ϊ���յ��ֱ�ߵľ���С��1(������)ʱ��
    //�����ߵ��е㴦�ָ�����
    __GK_BezierScreen(display,limit,x1,y1,x12/2,y12/2,
                    x1223/4,y1223/4,x/8,y/8,color,Rop2Code);
    __GK_BezierScreen(display,limit,x/8,y/8,x2334/4,y2334/4,
                    x34/2,y34/2,x4,y4,color,Rop2Code);
}
//----������������-------------------------------------------------------------
//����: �ڴ����ڻ�һ�����������ߡ�
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��ָ��
//      x1��y1��x2��y2��x3��y3��x4��y4������Bezier���ߵ��ĸ����Ƶ�
//      color������ʹ�õ���ɫ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_Bezier(struct GkscParaBezier *para)
{
    s32 offsetx,offsety;
    struct Rectangle limit,limitclip;
    struct ClipRect *clip;
    struct GkWinObj *fb_gkwin,*bzrwin;
//    struct DispDraw *my_draw_fun;
    if((para->Rop2Code == CN_R2_NOP) || (para->Rop2Code > CN_R2_LAST))
        return;                                         //��ִ�в���
    bzrwin = para->gkwin;
    if(para->range.left == para->range.right)
    {
        limit = bzrwin->limit;
    }
    else
    {
        __GK_GetRectInts(&para->range, &bzrwin->limit, &limit);
    }
//    my_draw_fun = &bzrwin->disp->draw;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((bzrwin->wm_bitmap != NULL)
        && (bzrwin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
    {   //������:��win buffer�л�ͼ����־changed_msk
//      limit.left = 0;
//      limit.top = 0;
//      limit.right = bzrwin->wm_bitmap->width;
//      limit.bottom = bzrwin->wm_bitmap->height;
        //��Ӳ�����ٲ�֧�ֱ��������ߵĻ��ƿ��ǣ������ʵ��
        __GK_BezierBm(bzrwin,&limit,para->x1,para->y1,para->x2,para->y2,
                            para->x3,para->y3,para->x4,para->y4,
                            para->color,para->Rop2Code);
    }else       //��win buffer����ֱ��д������Ϊtrue
    {
        clip = bzrwin->visible_clip;
        if(clip == NULL)
            return ;
        fb_gkwin = bzrwin->disp->frame_buffer;
        offsetx = bzrwin->ScreenX;
        offsety = bzrwin->ScreenY;
        //��frame buffer����ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (bzrwin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            do{
                __GK_GetRectInts(&limit, &clip->rect, &limitclip);
                //��Ӳ�����ٲ�֧�ֱ��������ߵĻ��ƿ��ǣ������ʵ��
                __GK_BezierBm(fb_gkwin,&limitclip,para->x1+offsetx,
                            para->y1+offsety,para->x2+offsetx,para->y2+offsety,
                            para->x3+offsetx,para->y3+offsety,para->x4+offsetx,
                            para->y4+offsety,para->color,para->Rop2Code);
                clip = clip->next;
            }while(clip != bzrwin->visible_clip);
        }else
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻�������ֱ�ӻ���screen��
        {
            do{
                __GK_GetRectInts(&limit, &limitclip, &limitclip);
                __GK_BezierScreen(bzrwin->disp,&clip->rect,para->x1+offsetx,
                            para->y1+offsety,para->x2+offsetx,para->y2+offsety,
                            para->x3+offsetx,para->y3+offsety,para->x4+offsetx,
                            para->y4+offsety,para->color,para->Rop2Code);
                clip = clip->next;
            }while(clip != bzrwin->visible_clip);
        }
    }
}
//----��ֱ��-------------------------------------------------------------------
//����: �ڴ����ڻ�һ��ֱ�ߡ�
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��ָ��
//      x1��y1�������������
//      x2��y2�������յ�����(������)
//      color������ʹ�õ���ɫ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_Lineto(struct GkscParaLineto *para)
{
    struct Rectangle limit,limitclip;
    struct ClipRect *clip;
    struct GkWinObj *fb_gkwin,*linetowin;
    struct DispDraw *my_draw_fun;
    s32 offsetx,offsety;
    if((para->Rop2Code == CN_R2_NOP) || (para->Rop2Code > CN_R2_LAST))
        return;                                         //��ִ�в���
    if((para->x1==para->x2) && (para->y1==para->y2))    //�������
        return;
    linetowin = para->gkwin;
    if(para->range.left == para->range.right)
    {
        limit = linetowin->limit;
    }
    else
    {
        __GK_GetRectInts(&para->range, &linetowin->limit, &limit);
    }
    my_draw_fun = &linetowin->disp->draw;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((linetowin->wm_bitmap != NULL)
        && (linetowin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
    {   //������:��win buffer�л�ͼ����־changed_msk
//      limit.left = 0;
//      limit.top = 0;
//      limit.right = linetowin->wm_bitmap->width;
//      limit.bottom = linetowin->wm_bitmap->height;
        if(!my_draw_fun->LineToBitmap(linetowin->wm_bitmap,&limit,para->x1,
                    para->y1,para->x2,para->y2,para->color,para->Rop2Code))
        {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
            //ͬʱ������changed_msk
            __GK_LinetoWin(linetowin,&limit,para->x1,para->y1,para->x2,
                                    para->y2,para->color,para->Rop2Code);
        }
        else    //Ӳ�����ٻ�ͼʱ��δ����changed_msk������֮
        {
            __GK_ShadingLine(linetowin,&limit,
                                para->x1,para->y1,para->x2,para->y2);
        }
    }
    else       //��win buffer����ֱ��д������Ϊtrue
    {
        clip = linetowin->visible_clip;
        if(clip == NULL)
            return ;
        fb_gkwin = linetowin->disp->frame_buffer;
        offsetx = linetowin->ScreenX;
        offsety = linetowin->ScreenY;
        //��frame buffer,��ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (linetowin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            do{
                __GK_GetRectInts(&limit, &clip->rect, &limitclip);
                if(!my_draw_fun->LineToBitmap(
                            fb_gkwin->wm_bitmap,&limitclip,
                            para->x1+offsetx,para->y1+offsety,para->x2+offsetx,
                            para->y2+offsety,para->color,para->Rop2Code))
                {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
                    //ͬʱ������changed_msk
                    __GK_LinetoWin(fb_gkwin,&limitclip,
                                    para->x1+offsetx,para->y1+offsety,
                                    para->x2+offsetx,para->y2+offsety,
                                    para->color,para->Rop2Code);
                }
                else    //Ӳ�����ٻ�ͼʱ��δ����changed_msk������֮
                {
                    __GK_ShadingLine(fb_gkwin,&limitclip,para->x1+offsetx,
                                      para->y1+offsety,para->x2+offsetx,
                                      para->y2+offsety);
                }
                clip = clip->next;
            }while(clip != linetowin->visible_clip);
        }
        else
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{//��screen��ֱ�ӻ�ֱ�ߣ������յ�
                __GK_GetRectInts(&limit, &clip->rect, &limitclip);
                if(!my_draw_fun->LineToScreen(&limitclip,para->x1+offsetx,
                             para->y1+offsety,para->x2+offsetx,para->y2+offsety,
                             para->color,para->Rop2Code))
                {
                    __GK_LinetoScreen(linetowin->disp,&limitclip,
                            para->x1+offsetx,para->y1+offsety,para->x2+offsetx,
                            para->y2+offsety,para->color,para->Rop2Code);
                }
                clip = clip->next;
            }while(clip != linetowin->visible_clip);
        }
    }
}

//----��ֱ��-------------------------------------------------------------------
//����: �ڴ����ڻ�һ��ֱ��,��������Ҫ���ơ�
//����: gkwin��Ŀ�괰��ָ��
//      gkwin��Ŀ�괰��ָ��
//      x1��y1�������������
//      x2��y2�������յ�����(����)
//      color������ʹ�õ���ɫ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
//void __GK_LinetoIe(struct GkscParaLineto *para)
//{
//    s32 dx,dy;
//    struct Rectangle limit;
//    struct ClipRect *clip;
//    struct GkWinObj *fb_gkwin,*lintoiewin;
//    struct DispDraw *my_draw_fun;
//    struct DisplayObj *display;
//    s32 offsetx,offsety;
//    if((para->Rop2Code == CN_R2_NOP) || (para->Rop2Code > CN_R2_LAST))
//        return;                                         //��ִ�в���
//    if((para->x1==para->x2) && (para->y1==para->y2))    //�������
//        return;
//    lintoiewin = para->gkwin;
//    display = lintoiewin->disp;
//    my_draw_fun = &display->draw;
//    if(para->y1 == para->y2)    //����ˮƽ��
//    {
//        //˵����win buffer����ֱ��д������Ϊfalse
//        if((lintoiewin->wm_bitmap != NULL)
//            && (lintoiewin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
//        {   //������:��win buffer�л�ͼ����־changed_msk
//            limit.left = 0;
//            limit.top = 0;
//            limit.right = lintoiewin->wm_bitmap->width;
//            limit.bottom = lintoiewin->wm_bitmap->height;
//            if(!my_draw_fun->LineToBitmapIe(lintoiewin->wm_bitmap,&limit,para->x1,
//                        para->y1,para->x2,para->y2,para->color,para->Rop2Code))
//            {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
//                //ͬʱ������changed_msk
//                __GK_HlinetoWin(lintoiewin,&limit,para->x1,para->y1,
//                                para->x2+1,para->color,para->Rop2Code);
//            }
//            else
//            {   //Ӳ������֧��ֱ�߻��ƣ�δ����changed_msk������֮
//                __GK_ShadingLine(lintoiewin,&limit,
//                                para->x1,para->y1,para->x2+1,para->y2);
//            }
//
//        }
//        else       //��win buffer����ֱ��д������Ϊtrue
//        {
//            clip = lintoiewin->visible_clip;
//            if(clip == NULL)
//                return ;
//            fb_gkwin = display->frame_buffer;
//            offsetx = lintoiewin->ScreenX;
//            offsety = lintoiewin->ScreenY;
//            //��frame buffer����ֱ��д������Ϊfalse
//            if((fb_gkwin != NULL) && (lintoiewin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
//            {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
//                do
//                {
//                    if(!my_draw_fun->LineToBitmapIe(
//                                fb_gkwin->wm_bitmap,&clip->rect,
//                                para->x1+offsetx,para->y1+offsety,
//                                para->x2+offsetx,para->y2+offsety,
//                                para->color,para->Rop2Code))
//                    {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
//                        //ͬʱ������changed_msk
//                        __GK_HlinetoWin(fb_gkwin,&clip->rect,
//                                    para->x1+offsetx,para->y1+offsety,
//                                    para->x2+offsetx+1,
//                                    para->color,para->Rop2Code);
//                    }
//                    else
//                    {   //Ӳ�����ٻ�ͼʱ��δ����changed_msk������֮
//                        __GK_ShadingLine(fb_gkwin,&clip->rect,
//                                        para->x1+offsetx,para->y1+offsety,
//                                        para->x2+offsetx+1,para->y2+offsety);
//                    }
//                    clip = clip->next;
//                }while(clip != lintoiewin->visible_clip);
//            }
//            else
//            //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
//            //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
//            {
//                do{//��screen��ֱ�ӻ�ֱ�ߣ����յ�
////                  if(!my_draw_fun->LineToScreenIe(&clip->rect,
////                                para->x1+offsetx,para->y1+offsety,
////                                para->x2+offsetx,para->y2+offsety,
////                               para->color,para->Rop2Code))
//                    {
//                        __GK_LinetoScreen(display,&clip->rect,
//                                        para->x1+offsetx,para->y1+offsety,
//                                        para->x2+offsetx+1,para->y2+offsety,
//                                        para->color,para->Rop2Code);
//                    }
//                    clip = clip->next;
//                }while(clip != lintoiewin->visible_clip);
//            }
//        }
//    }
//    else if(para->x1 == para->x2)    //���ƴ�ֱ��
//    {
//        //˵����win buffer����ֱ��д������Ϊfalse
//        if((lintoiewin->wm_bitmap != NULL)
//            && (lintoiewin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
//        {   //������:��win buffer�л�ͼ����־changed_msk
//            limit.left = 0;
//            limit.top = 0;
//            limit.right = lintoiewin->wm_bitmap->width;
//            limit.bottom = lintoiewin->wm_bitmap->height;
//            if(!my_draw_fun->LineToBitmapIe(lintoiewin->wm_bitmap,&limit,para->x1,
//                        para->y1,para->x2,para->y2,para->color,para->Rop2Code))
//            {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
//                //ͬʱ������changed_msk
//                __GK_VlinetoWin(lintoiewin,&limit,para->x1,
//                            para->y1,para->y2+1,para->color,para->Rop2Code);
//            }
//            else
//            {   //Ӳ������֧��ֱ�߻��ƣ�δ����changed_msk������֮
//                __GK_ShadingLine(lintoiewin,&limit,
//                                para->x1,para->y1,para->x2,para->y2+1);
//            }
//        }
//        else       //��win buffer����ֱ��д������Ϊtrue
//        {
//            clip = lintoiewin->visible_clip;
//            if(clip == NULL)
//                return ;
//            fb_gkwin = display->frame_buffer;
//            offsetx = lintoiewin->ScreenX;
//            offsety = lintoiewin->ScreenY;
//            //��frame buffer����ֱ��д������Ϊfalse
//            if((fb_gkwin != NULL) && (lintoiewin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
//            {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
//                do
//                {
//                    if(!my_draw_fun->LineToBitmapIe(
//                                fb_gkwin->wm_bitmap,&clip->rect,
//                                para->x1+offsetx,para->y1+offsety,
//                                para->x2+offsetx,para->y2+offsety,
//                                para->color,para->Rop2Code))
//                    {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
//                        //ͬʱ������changed_msk
//                        __GK_VlinetoWin(fb_gkwin,&clip->rect,
//                                    para->x1+offsetx,para->y1+offsety,
//                                    para->y2+offsety+1,
//                                    para->color,para->Rop2Code);
//                    }
//                    else
//                    {   //Ӳ�����ٻ�ͼʱ��δ����changed_msk������֮
//                        __GK_ShadingLine(fb_gkwin,&clip->rect,
//                                        para->x1+offsetx,para->y1+offsety,
//                                        para->x2+offsetx,para->y2+offsety+1);
//                    }
//                    clip = clip->next;
//                }while(clip != lintoiewin->visible_clip);
//            }
//            else
//            //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
//            //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
//            {
//                do{//��screen��ֱ�ӻ�ֱ�ߣ����յ�
////                  if(!my_draw_fun->LineToScreenIe(&clip->rect,
////                                para->x1+offsetx,para->y1+offsety,
////                                para->x2+offsetx,para->y2+offsety,
////                               para->color,para->Rop2Code))
//                    {
//                        __GK_LinetoScreen(display,&clip->rect,
//                                        para->x1+offsetx,para->y1+offsety,
//                                        para->x2+offsetx,para->y2+offsety+1,
//                                        para->color,para->Rop2Code);
//                    }
//                    clip = clip->next;
//                }while(clip != lintoiewin->visible_clip);
//            }
//        }
//    }
//    else    //����б��
//    {
//        dx = abs(para->x2-para->x1);
//        dy = abs(para->y2-para->y1);
//        //˵����win buffer����ֱ��д������Ϊfalse
//        if((lintoiewin->wm_bitmap != NULL)
//            && (lintoiewin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
//        {   //������:��win buffer�л�ͼ����־changed_msk
//            limit.left = 0;
//            limit.top = 0;
//            limit.right = lintoiewin->wm_bitmap->width;
//            limit.bottom = lintoiewin->wm_bitmap->height;
//            if(!my_draw_fun->LineToBitmapIe(lintoiewin->wm_bitmap,&limit,para->x1,
//                        para->y1,para->x2,para->y2,para->color,para->Rop2Code))
//            {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
//                //ͬʱ������changed_msk
//                if(dy > dx)
//                {
//                    __GK_OlinetoWin(lintoiewin,&limit,para->x1,para->y1,
//                                    para->x2,para->y2+1,
//                                    para->color,para->Rop2Code);
//                }
//                else
//                {
//                    __GK_OlinetoWin(lintoiewin,&limit,para->x1,para->y1,
//                                    para->x2+1,para->y2,
//                                    para->color,para->Rop2Code);
//                }
//            }
//            else
//            {   //Ӳ������֧��ֱ�߻��ƣ�δ����changed_msk������֮
//                __GK_ShadingLine(lintoiewin,&limit,
//                                para->x1,para->y1,para->x2,para->y2);
//            }
//
//        }
//        else       //��win buffer����ֱ��д������Ϊtrue
//        {
//            clip = lintoiewin->visible_clip;
//            if(clip == NULL)
//                return ;
//            fb_gkwin = display->frame_buffer;
//            offsetx = lintoiewin->ScreenX;
//            offsety = lintoiewin->ScreenY;
//            //��frame buffer����ֱ��д������Ϊfalse
//            if((fb_gkwin != NULL) && (lintoiewin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
//            {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
//                do
//                {
//                    if(!my_draw_fun->LineToBitmapIe(
//                                fb_gkwin->wm_bitmap,&clip->rect,
//                                para->x1+offsetx,para->y1+offsety,
//                                para->x2+offsetx,para->y2+offsety,
//                                para->color,para->Rop2Code))
//                    {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
//                        //ͬʱ������changed_msk
//                            __GK_OlinetoWin(fb_gkwin,&clip->rect,
//                                            para->x1+offsetx,para->y1+offsety,
//                                            para->x2+offsetx,para->y2+offsety,
//                                            para->color,para->Rop2Code);
//                    }
//                    else
//                    {   //Ӳ�����ٻ�ͼʱ��δ����changed_msk������֮
//                            __GK_ShadingLine(fb_gkwin,&clip->rect,
//                                            para->x1+offsetx,para->y1+offsety,
//                                            para->x2+offsetx,para->y2+offsety);
//                    }
//                    clip = clip->next;
//                }while(clip != lintoiewin->visible_clip);
//            }
//            else
//            //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
//            //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
//            {
//                do{//��screen��ֱ�ӻ�ֱ�ߣ����յ�
////                  if(!my_draw_fun->LineToScreenIe(&clip->rect,para->x1+offsetx,
////                               para->y1+offsety,para->x2+offsetx,para->y2+offsety,
////                               para->color,para->Rop2Code))
//                    {
//                            __GK_LinetoScreen(display,&clip->rect,
//                                            para->x1+offsetx,para->y1+offsety,
//                                            para->x2+offsetx,para->y2+offsety,
//                                            para->color,para->Rop2Code);
//                    }
//                    clip = clip->next;
//                }while(clip != lintoiewin->visible_clip);
//            }
//        }
//    }
//}

//----��ʾ�ı�-----------------------------------------------------------------
//����: ��Ĭ�������Ĭ���ַ�����ʾһ���ı�����
//����: gkwin����ʾ��Ŀ�괰��
//      x��y����ʾλ�ã������gkwin
//      text������ʾ���ַ���
//      count��Ҫ��ʾ���ַ�����-1��ʾҪ��ʾ����
//����: ��
//-----------------------------------------------------------------------------
void __GK_DrawText(struct GkscParaDrawText *para,const char *text,u32 *Bytes)
{
    struct GkscParaDrawBitmapRop bitmap_para;
    struct FontObj* cur_font;
    struct Charset* cur_enc;
    const char *String = text;
    s32 len, char_num = 0,size,size_bak;
    u32 wc;
    u8 buf[128],*dbuf;  //����һ���㹻��32�����ֵĻ������������Ҫ��ʾ���ַ�
                        //�����˵��󣬾���Ҫ��̬����
    *Bytes = 0;
    if(para->Rop2Code == CN_R2_NOP)
        return;

    cur_font = para->pFont;
    if(cur_font == NULL)
        cur_font = Font_GetCurFont();
    if(cur_font == NULL)
        return;

    cur_enc = para->pCharset;
    if(cur_enc == NULL)
        cur_enc = Charset_NlsGetCurCharset();
    if(cur_enc == NULL)
        return;

//    bitmap_para.command =CN_GKSC_DRAW_BITMAP;
    bitmap_para.gkwin = para->gkwin;
    bitmap_para.range = para->range;
    bitmap_para.HyalineColor = 0;
    bitmap_para.x = para->x;
    bitmap_para.y = para->y;
    bitmap_para.RopCode = (struct RopGroup){ 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };
    bitmap_para.RopCode.HyalineEn = 1;
    bitmap_para.RopCode.Rop2Mode = para->Rop2Code;
    bitmap_para.bitmap.ExColor = para->color;
    bitmap_para.bitmap.reversal = false;
    dbuf = NULL;
    size_bak = 0;
    for(; ;)
    {
        if((para->count != -1)  && (char_num >= para->count))
        {
            break;
        }
        len= cur_enc->MbToUcs4(&wc, String, -1);
        if(len == -1)
        { // ��Ч�ַ�
            String++;
        }
        else
        { // ��Ч�ַ�
            if(len == 0)
                break;
            String += len;
            char_num++;
            if(wc==L'\r')
            {
                bitmap_para.x =  para->x;
                continue;
            }

            if(wc==L'\n')
            {
                bitmap_para.y += cur_font->MaxHeight;
                continue;
            }

            // ȡ����
            bitmap_para.bitmap.bm_bits = NULL;      //NULL=��ѯbitmap.
            cur_font->GetBitmap(wc,0,0,&(bitmap_para.bitmap));
            size = bitmap_para.bitmap.linebytes * bitmap_para.bitmap.height;
            if(size < 128)
                bitmap_para.bitmap.bm_bits = buf;
            else
            {
                if(size > size_bak)
                {
                    free(dbuf);
                    dbuf = M_MallocLc(size,0);
                    if(dbuf == NULL)
                        break;
                    size_bak = size;
                }
                bitmap_para.bitmap.bm_bits = dbuf;
            }
            cur_font->GetBitmap(wc,0,0,&(bitmap_para.bitmap));
            __GK_DrawBitMap(&bitmap_para);

            bitmap_para.x += bitmap_para.bitmap.width;
        }
    }
    *Bytes =(u32)( String - text);
    free(dbuf);
}
//----���ƹ�դλͼ-------------------------------------------------------------
//����: �ڴ����ڻ���դλͼ��λͼ�������ڵĲ��ֽ�������ơ�������һһ��Ӧcopy��
//      ���������ѹ��
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��ָ��
//      bitmap�������Ƶ�λͼ
//      pattern��Ҫ���õ�ͼ��λͼ�����ظ�ʽ�� bitmap һ��
//      bitmsk��rop4��������Ҫ������λͼ��һ����ɫλͼ
//      transparentcolor��Ҫ���õ�͸��ɫ
//      x��y�����Ƶ�Ŀ��λ��
//      rop_code����դ������
//����: ��
//todo �ÿͻ���������ʱ������Ҫ�޸�����������֡�
//-----------------------------------------------------------------------------
void __GK_DrawBitMap(struct GkscParaDrawBitmapRop *para)
{
//    u32 HyalineColor;
    s32 x_src,y_src,x_dst,y_dst;
    struct PointCdn InsOffset;
    struct Rectangle DstRect,SrcRect,InsRect;
    struct ClipRect *clip;
    struct GkWinObj *fb_gkwin,*DstGkwin;
    struct RectBitmap *DstBitmap;
    struct RectBitmap *SrcBitmap;
    struct DisplayObj *display;
    struct Rectangle limit;
    DstGkwin = para->gkwin;
    display = DstGkwin->disp;
    if(para->range.left == para->range.right)
    {
        limit = DstGkwin->limit;
    }
    else
    {
        __GK_GetRectInts(&para->range, &DstGkwin->limit, &limit);
    }

//    HyalineColor = para->HyalineColor;
    DstBitmap = DstGkwin->wm_bitmap;
    SrcBitmap = &para->bitmap;
//    SrcBitmap->reversal=true;
    //�����Ƶ�λͼҪ���Ƶ�λ�������Ŀ�괰�ڵ�����
    SrcRect.left = 0;
    SrcRect.top = 0;
    SrcRect.right = SrcBitmap->width;
    SrcRect.bottom = SrcBitmap->height;
    DstRect.left = para->x;
    DstRect.top = para->y;
    DstRect.right = para->x+SrcRect.right;
    DstRect.bottom = para->y+SrcRect.bottom;
    //����������λͼҪ���Ƶ�λ�ã������ڴ�����
    if(        (DstRect.right < limit.left)
            || (DstRect.left >= limit.right)
            || (DstRect.bottom < limit.top)
            || (DstRect.top >= limit.bottom))
        return;                             //��ͼ�����ڻ���������
    if(DstRect.left < limit.left)         //������߽��ڻ������ڲ�
    {
        SrcRect.left = limit.left - DstRect.left;
        DstRect.left = limit.left;
    }
    if(DstRect.top < limit.top)           //�����ϱ߽��ڻ������ڲ�
    {
        SrcRect.top = limit.top - DstRect.top;
        DstRect.top = limit.top;
    }
    if(DstRect.right > limit.right)       //�����ұ߽��ڻ������ڲ�
    {
        SrcRect.right -= DstRect.right - limit.right;
        DstRect.right = limit.right;
    }
    if(DstRect.bottom > limit.bottom )      //�����±߽��ڻ������ڲ�
    {
        SrcRect.bottom -= DstRect.bottom - limit.bottom;
        DstRect.bottom = limit.bottom;
    }

    //˵����win buffer����ֱ��д������Ϊfalse
    if((DstBitmap != NULL) && (DstGkwin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
    {   //������:��win buffer�л�ͼ����־changed_msk
        if(!display->draw.BltBitmapToBitmap(DstBitmap,&DstRect,
                                       SrcBitmap,&SrcRect,
                                       para->RopCode,para->HyalineColor))
        {
            //������֧�� BltBitmapToBitmap ���ƣ����ɴ���ʵ��
            y_dst= DstRect.top;
            if(SrcBitmap->reversal == true)
            {
                for(y_src= SrcBitmap->height - SrcRect.top;
                            y_src > SrcBitmap->height - SrcRect.bottom;y_src--)
                {
                    x_dst = DstRect.left;
                    for(x_src = SrcRect.left;x_src < SrcRect.right;x_src++)
                    {
                        __GK_CopyPixelRopBm(DstBitmap,SrcBitmap,
                                                x_dst,y_dst,x_src,y_src,
                                                para->RopCode,para->HyalineColor);
                        x_dst++;
                    }
                    y_dst++;
                }
            }
            else
            {
                for(y_src= SrcRect.top;y_src < SrcRect.bottom; y_src++)
                {
                    x_dst = DstRect.left;
                    for(x_src = SrcRect.left;x_src < SrcRect.right;x_src++)
                    {
                        __GK_CopyPixelRopBm(DstBitmap,SrcBitmap,
                                                x_dst,y_dst,x_src,y_src,
                                                para->RopCode,para->HyalineColor);
                        x_dst++;
                    }
                    y_dst++;
                }
            }
        }
        __GK_ShadingRect(DstGkwin,&DstRect);    //���û��Ʋ��ֵ�changed_msk
    }
    else        //��win buffer����ֱ��д������Ϊtrue
    {
        clip = DstGkwin->visible_clip;
        if(clip == NULL)                            //���ڿ�����Ϊ�գ�ֱ�ӷ���
            return ;
        //��clip��Ӧ��SrcRectλ�û��Ƶ�framebuffer�ϣ�����clip�϶���framebuffer
        //���ڣ���ֻ��Ҫ�������꣬�����ж��Ƿ�Խframebuffer�߽硣
        fb_gkwin = display->frame_buffer;
        DstBitmap = fb_gkwin->wm_bitmap;
        //��frame buffer����ֱ��д������Ϊfalse���ѿ�������Ƶ�framebuffer��
        if((fb_gkwin != NULL) && (DstGkwin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
        {   //������:��frame_buffer�л�ͼ����ֻ��gkwin�еĿ�������
            //Ϊ����������걣��һ�£�����λͼҪ���Ƶ�λ�õ�
            //�����������screenԭ��Ϊԭ������꣬ע�����������꣬���ܳ���
            //framebuffer�ĳߴ���ޣ���û�й�ϵ�ģ�ȡ�����򽻼���ʱ��ᴦ�����
            DstRect.left += DstGkwin->ScreenX;
            DstRect.right += DstGkwin->ScreenX;
            DstRect.top += DstGkwin->ScreenY;
            DstRect.bottom += DstGkwin->ScreenY;
            //SrcRect���ڱ�������Ƶ�bitmap���framebuffer��ƫ�ƣ����ڼ���clip
            //��framebuffer�ж�Ӧ��λ��
            InsOffset.x = DstGkwin->ScreenX + para->x;
            InsOffset.y = DstGkwin->ScreenY + para->y;
            do{
                if(__GK_GetRectInts(&clip->rect,&DstRect,&InsRect))
                {
                    SrcRect.left = InsRect.left-InsOffset.x;
                    SrcRect.top = InsRect.top-InsOffset.y;
                    SrcRect.right = InsRect.right-InsOffset.x;
                    SrcRect.bottom = InsRect.bottom-InsOffset.y;
                    if(!fb_gkwin->disp->draw.BltBitmapToBitmap(
                                        DstBitmap,&InsRect,
                                        SrcBitmap,&SrcRect,
                                        para->RopCode,para->HyalineColor))
                    {
                        SrcBitmap = &para->bitmap;

                        y_dst= DstRect.top;
                        //�����Ĺ�դ������Ϊ��չ�Ĺ�դ������
                        if(        (para->RopCode.AlphaEn == 0)
                                && (para->RopCode.HyalineEn == 0)
                                && (para->RopCode.Rop2Mode == CN_R2_COPYPEN ) )
                        {
                            if(SrcBitmap->reversal == true)
                            {
                                // -1 ����Ϊͼ�������Ǵ� 0 ��ʼ���
                                for(y_src= SrcBitmap->height - SrcRect.top;
                                    y_src > SrcBitmap->height - SrcRect.bottom;
                                    y_src--)
                                {
                                    x_dst = DstRect.left;
                                    for(x_src = SrcRect.left;
                                        x_src < SrcRect.right;
                                        x_src++)
                                    {
                                        __GK_CopyPixelBm(DstBitmap,SrcBitmap,
                                                         x_dst,y_dst,x_src, y_src);
                                        x_dst++;
                                    }
                                    y_dst++;
                                }

                            }
                            else
                            {
                                for(y_src= SrcRect.top;
                                    y_src < SrcRect.bottom;
                                    y_src++)
                                {
                                    x_dst = DstRect.left;
                                    for(x_src = SrcRect.left;
                                        x_src < SrcRect.right;
                                        x_src++)
                                    {
                                        __GK_CopyPixelBm(DstBitmap,SrcBitmap,
                                                         x_dst,y_dst,x_src, y_src);
                                        x_dst++;
                                    }
                                    y_dst++;
                                }
                            }
                        }
                        else
                        {
                            if(SrcBitmap->reversal == true)
                            {
                                // -1 ����Ϊͼ�������Ǵ� 0 ��ʼ���
                                for(y_src= SrcBitmap->height - SrcRect.top;
                                    y_src > SrcBitmap->height - SrcRect.bottom;
                                    y_src--)
                                {
                                    x_dst = DstRect.left;
                                    for(x_src = SrcRect.left;
                                        x_src < SrcRect.right;
                                        x_src++)
                                    {
                                        __GK_CopyPixelRopBm(DstBitmap,SrcBitmap,
                                                            x_dst,y_dst,x_src,
                                                            y_src,para->RopCode,
                                                            para->HyalineColor);
                                        x_dst++;
                                    }
                                    y_dst++;
                                }
                            }
                            else
                            {
                                for(y_src= SrcRect.top;
                                    y_src < SrcRect.bottom;
                                    y_src++)
                                {
                                    x_dst = DstRect.left;
                                    for(x_src = SrcRect.left;
                                        x_src < SrcRect.right;
                                        x_src++)
                                    {
                                        __GK_CopyPixelRopBm(DstBitmap,SrcBitmap,
                                                            x_dst,y_dst,x_src,
                                                            y_src,para->RopCode,
                                                            para->HyalineColor);
                                        x_dst++;
                                    }
                                    y_dst++;
                                }
                            }
                        }
                    }
                    //���û��Ʋ��ֵ�changed_msk
                    __GK_ShadingRect(fb_gkwin,&DstRect);
                }
                clip = clip->next;
            }while(clip != DstGkwin->visible_clip);
        }
        else
        //��rop������λͼ��֧��ֱ��д������win buffer��Ҳ��frame buffer��
        //����ֱ��д������Ϊtrue����������˻�ΪGK_DrawBitMap�Ĺ���
        {
            //Ϊ����������걣��һ�£���λͼҪ���Ƶ�λ�õ�
            //�����������screenԭ��Ϊԭ�������
            DstRect.left += DstGkwin->ScreenX;
            DstRect.right += DstGkwin->ScreenX;
            DstRect.top += DstGkwin->ScreenY;
            DstRect.bottom += DstGkwin->ScreenY;
            do{
                //Ҫ���Ƶ�λͼĿ��λ���ڴ��ڿ������ڲ�ִ�л��Ƴ���
                if(__GK_GetRectInts(&clip->rect,&DstRect,&InsRect))
                {
                    //����bitmap��screen
                    __GK_BltBmToScreen(display,&InsRect,
                                        &para->bitmap,
                                        InsRect.left-DstRect.left,
                                        InsRect.top-DstRect.top);
                }
                clip = clip->next;
            }while(clip != DstGkwin->visible_clip);
        }
    }
}

//----����ɫ���λͼ---------------------------------------------------------
//����: �ý���ɫ�����ʾ���е�һ����������
//����: Draw,Ŀ����ʾ���Ļ��ƺ�����ָ�롣
//      Target: ������̸��ǵ�����
//      Focus�������ļ������������Target�ڲ�
//      Color0,��ɫ1�������CN_FILLRECT_MODE_LR�峣������
//      Color1,��ɫ2�������CN_FILLRECT_MODE_LR�峣������
//      Mode,����ģʽ
//����: ��
//ע: б�߽���ʹ�õ��ǽ���ģʽ���ϸ�ģʽӦ����������������붥��ľ��룬������
//    ������漰���������㡣���۶���ɫ�仯�ǲ����еģ����Ƽ���Ҳûɶ�����á�
//-----------------------------------------------------------------------------
void __GK_GradientFillRectSoft( struct RectBitmap *bitmap,
                                struct Rectangle *Target,
                                struct Rectangle *Focus,
                                u32 Color0,u32 Color1,u32 Mode)
{
    u32 a0,r0,g0,b0,a1,r1,g1,b1,Color;
    u32 ax,rx,gx,bx,ay,ry,gy,by;
    s32 L0,L1,x,y,CrossX,CrossY;
    a0 = Color0>>24;
    r0 = (Color0>>16)&0xff;
    g0 = (Color0>>8)&0xff;
    b0 = Color0&0xff;
    a1 = Color1>>24;
    r1 = (Color1>>16)&0xff;
    g1 = (Color1>>8)&0xff;
    b1 = Color1&0xff;
    CrossY = Target->bottom - Target->top;
    CrossX = Target->right - Target->left;
    switch(Mode)
    {
        case CN_FILLRECT_MODE_LR:
        {
            for(x = Focus->left; x<Focus->right; x++)
            {
                L0 = x-Target->left;
                L1 = Target->right - x;
                ax = ( ( (L0*a1 + L1*a0)<<8) / CrossX>>8) & 0xff;
                rx = ( ( (L0*r1 + L1*r0)<<8) / CrossX>>8) & 0xff;
                gx = ( ( (L0*g1 + L1*g0)<<8) / CrossX>>8) & 0xff;
                bx = ( ( (L0*b1 + L1*b0)<<8) / CrossX>>8) & 0xff;
                Color = ( ax << 24) +( rx << 16) +( gx << 8) +( bx << 0);
                Color = GK_ConvertRGB24ToPF(bitmap->PixelFormat,Color);
                for(y = Focus->top; y<Focus->bottom; y++)
                {
                    __GK_SetPixelRop2Bm(bitmap,x,y,Color,CN_R2_COPYPEN);
                }
            }
        } break;
        case CN_FILLRECT_MODE_UD:
        {
            for(y = Focus->top; y<Focus->bottom; y++)
            {
                L0 = y-Target->top;
                L1 = Target->bottom- y;
                ay = ( ( (L0*a1 + L1*a0)<<8) / CrossY>>8) & 0xff;
                ry = ( ( (L0*r1 + L1*r0)<<8) / CrossY>>8) & 0xff;
                gy = ( ( (L0*g1 + L1*g0)<<8) / CrossY>>8) & 0xff;
                by = ( ( (L0*b1 + L1*b0)<<8) / CrossY>>8) & 0xff;
                Color = ( ay << 24) +( ry << 16) +( gy << 8) +( by << 0);
                Color = GK_ConvertRGB24ToPF(bitmap->PixelFormat,Color);
                for(x = Focus->left; x<Focus->right; x++)
                {
                    __GK_SetPixelRop2Bm(bitmap,x,y,Color,CN_R2_COPYPEN);
                }
            }

        } break;
        case CN_FILLRECT_MODE_LU2RD:
        {
            for(x = Focus->left; x<Focus->right; x++)
            {
                L0 = x-Target->left;
                L1 = Target->right - x;
                ax = ( ( (L0*a1 + L1*a0)<<8) / CrossX>>8) & 0xff;
                rx = ( ( (L0*r1 + L1*r0)<<8) / CrossX>>8) & 0xff;
                gx = ( ( (L0*g1 + L1*g0)<<8) / CrossX>>8) & 0xff;
                bx = ( ( (L0*b1 + L1*b0)<<8) / CrossX>>8) & 0xff;
                for(y = Focus->top; y<Focus->bottom; y++)
                {
                    L0 = y-Target->top;
                    L1 = Target->bottom- y;
                    ay = ( ( (L0*a1 + L1*a0)<<8) / CrossY>>8) & 0xff;
                    ry = ( ( (L0*r1 + L1*r0)<<8) / CrossY>>8) & 0xff;
                    gy = ( ( (L0*g1 + L1*g0)<<8) / CrossY>>8) & 0xff;
                    by = ( ( (L0*b1 + L1*b0)<<8) / CrossY>>8) & 0xff;
                    Color = ( ((ax+ay)>>1) << 24)
                            +( ((rx+ry)>>1) << 16)
                            +( ((gx+gy)>>1) << 8)
                            +( ((bx+by)>>1) << 0);
                    Color = GK_ConvertRGB24ToPF(bitmap->PixelFormat,Color);
                    __GK_SetPixelRop2Bm(bitmap,x,y,Color,CN_R2_COPYPEN);
                }
            }
        } break;
        case CN_FILLRECT_MODE_RU2LD:
        {
            for(x = Focus->left; x<Focus->right; x++)
            {
                L1 = x-Target->left;
                L0 = Target->right - x;
                ax = ( ( (L0*a1 + L1*a0)<<8) / CrossX>>8) & 0xff;
                rx = ( ( (L0*r1 + L1*r0)<<8) / CrossX>>8) & 0xff;
                gx = ( ( (L0*g1 + L1*g0)<<8) / CrossX>>8) & 0xff;
                bx = ( ( (L0*b1 + L1*b0)<<8) / CrossX>>8) & 0xff;
                for(y = Focus->top; y<Focus->bottom; y++)
                {
                    L0 = y-Target->top;
                    L1 = Target->bottom- y;
                    ay = ( ( (L0*a1 + L1*a0)<<8) / CrossY>>8) & 0xff;
                    ry = ( ( (L0*r1 + L1*r0)<<8) / CrossY>>8) & 0xff;
                    gy = ( ( (L0*g1 + L1*g0)<<8) / CrossY>>8) & 0xff;
                    by = ( ( (L0*b1 + L1*b0)<<8) / CrossY>>8) & 0xff;
                    Color = ( ((ax+ay)>>1) << 24)
                            +( ((rx+ry)>>1) << 16)
                            +( ((gx+gy)>>1) << 8)
                            +( ((bx+by)>>1) << 0);
                    Color = GK_ConvertRGB24ToPF(bitmap->PixelFormat,Color);
                    __GK_SetPixelRop2Bm(bitmap,x,y,Color,CN_R2_COPYPEN);
                }
            }

        } break;
    }
}


//----����ɫ�����ʾ��---------------------------------------------------------
//����: �ý���ɫ�����ʾ���е�һ����������
//����: Draw,Ŀ����ʾ���Ļ��ƺ�����ָ�롣
//      Target: ������̸��ǵ�����
//      Focus�������ļ������������Target�ڲ�
//      Color0,��ɫ1�������CN_FILLRECT_MODE_LR�峣������
//      Color1,��ɫ2�������CN_FILLRECT_MODE_LR�峣������
//      Mode,����ģʽ
void __GK_GradientFillScreenRect(struct DispDraw *Draw,
                                 struct Rectangle *Target,
                                 struct Rectangle *Focus,
                                 u32 Color0,u32 Color1,u32 Mode)
{
    u32 a0,r0,g0,b0,a1,r1,g1,b1,Color;
    u32 ax,rx,gx,bx,ay,ry,gy,by;
    s32 L0,L1,x,y,CrossX,CrossY;
    a0 = Color0>>24;
    r0 = (Color0>>16)&0xff;
    g0 = (Color0>>8)&0xff;
    b0 = Color0&0xff;
    a1 = Color1>>24;
    r1 = (Color1>>16)&0xff;
    g1 = (Color1>>8)&0xff;
    b1 = Color1&0xff;
    CrossY = Target->bottom - Target->top;
    CrossX = Target->right - Target->left;
    switch(Mode)
    {
        case CN_FILLRECT_MODE_N:
        {
            for(x = Focus->left; x<Focus->right; x++)
            {
                for(y = Focus->top; y<Focus->bottom; y++)
                {
                    Draw->SetPixelToScreen(x,y,Color0,CN_R2_COPYPEN);
                }
            }
        } break;
        case CN_FILLRECT_MODE_LR:
        {
            for(x = Focus->left; x<Focus->right; x++)
            {
                L0 = x-Target->left;
                L1 = Target->right - x;
                ax = ( ( (L0*a1 + L1*a0)<<8) / CrossX>>8) & 0xff;
                rx = ( ( (L0*r1 + L1*r0)<<8) / CrossX>>8) & 0xff;
                gx = ( ( (L0*g1 + L1*g0)<<8) / CrossX>>8) & 0xff;
                bx = ( ( (L0*b1 + L1*b0)<<8) / CrossX>>8) & 0xff;
                Color = ( ax << 24) +( rx << 16) +( gx << 8) +( bx << 0);
                for(y = Focus->top; y<Focus->bottom; y++)
                {
                    Draw->SetPixelToScreen(x,y,Color,CN_R2_COPYPEN);
                }
            }
        } break;
        case CN_FILLRECT_MODE_UD:
        {
            for(y = Focus->top; y<Focus->bottom; y++)
            {
                L0 = y-Target->top;
                L1 = Target->bottom- y;
                ay = ( ( (L0*a1 + L1*a0)<<8) / CrossY>>8) & 0xff;
                ry = ( ( (L0*r1 + L1*r0)<<8) / CrossY>>8) & 0xff;
                gy = ( ( (L0*g1 + L1*g0)<<8) / CrossY>>8) & 0xff;
                by = ( ( (L0*b1 + L1*b0)<<8) / CrossY>>8) & 0xff;
                Color = ( ay << 24) +( ry << 16) +( gy << 8) +( by << 0);
                for(x = Focus->left; x<Focus->right; x++)
                {
                    Draw->SetPixelToScreen(x,y,Color,CN_R2_COPYPEN);
                }
            }

        } break;
        case CN_FILLRECT_MODE_LU2RD:
        {
            for(x = Focus->left; x<Focus->right; x++)
            {
                L0 = x-Target->left;
                L1 = Target->right - x;
                ax = ( ( (L0*a1 + L1*a0)<<8) / CrossX>>8) & 0xff;
                rx = ( ( (L0*r1 + L1*r0)<<8) / CrossX>>8) & 0xff;
                gx = ( ( (L0*g1 + L1*g0)<<8) / CrossX>>8) & 0xff;
                bx = ( ( (L0*b1 + L1*b0)<<8) / CrossX>>8) & 0xff;
                for(y = Focus->top; y<Focus->bottom; y++)
                {
                    L0 = y-Target->top;
                    L1 = Target->bottom- y;
                    ay = ( ( (L0*a1 + L1*a0)<<8) / CrossY>>8) & 0xff;
                    ry = ( ( (L0*r1 + L1*r0)<<8) / CrossY>>8) & 0xff;
                    gy = ( ( (L0*g1 + L1*g0)<<8) / CrossY>>8) & 0xff;
                    by = ( ( (L0*b1 + L1*b0)<<8) / CrossY>>8) & 0xff;
                    Color = ( ((ax+ay)>>1) << 24)
                            +( ((rx+ry)>>1) << 16)
                            +( ((gx+gy)>>1) << 8)
                            +( ((bx+by)>>1) << 0);
                    Draw->SetPixelToScreen(x,y,Color,CN_R2_COPYPEN);
                }
            }
        } break;
        case CN_FILLRECT_MODE_RU2LD:
        {
            for(x = Focus->left; x<Focus->right; x++)
            {
                L1 = x-Target->left;
                L0 = Target->right - x;
                ax = ( ( (L0*a1 + L1*a0)<<8) / CrossX>>8) & 0xff;
                rx = ( ( (L0*r1 + L1*r0)<<8) / CrossX>>8) & 0xff;
                gx = ( ( (L0*g1 + L1*g0)<<8) / CrossX>>8) & 0xff;
                bx = ( ( (L0*b1 + L1*b0)<<8) / CrossX>>8) & 0xff;
                for(y = Focus->top; y<Focus->bottom; y++)
                {
                    L0 = y-Target->top;
                    L1 = Target->bottom- y;
                    ay = ( ( (L0*a1 + L1*a0)<<8) / CrossY>>8) & 0xff;
                    ry = ( ( (L0*r1 + L1*r0)<<8) / CrossY>>8) & 0xff;
                    gy = ( ( (L0*g1 + L1*g0)<<8) / CrossY>>8) & 0xff;
                    by = ( ( (L0*b1 + L1*b0)<<8) / CrossY>>8) & 0xff;
                    Color = ( ((ax+ay)>>1) << 24)
                            +( ((rx+ry)>>1) << 16)
                            +( ((gx+gy)>>1) << 8)
                            +( ((bx+by)>>1) << 0);
                    Draw->SetPixelToScreen(x,y,Color,CN_R2_COPYPEN);
                }
            }

        } break;
    }
}


//----��䴰�ڵ�һ����---------------------------------------------------------
//����: ��ָ������ɫ��䴰�ڵ�һ���֡�
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��
//      rect�������ľ���
//      color�����ʹ�õ���ɫ
//����: ��
//��ע: ���������ܻ��޸�Rect�ĳ�Ա�����Բ������ⲿ���á�
//-----------------------------------------------------------------------------
void __GK_FillPartWin(struct GkWinObj *Gkwin,struct Rectangle *Rect,u32 Color)
{
    struct RectBitmap *bitmap;
    struct Rectangle ins_rect;
    struct DispDraw *my_draw_fun;
    struct ClipRect *clip;
    struct GkWinObj *fb_gkwin;

    bitmap = Gkwin->wm_bitmap;
    //��������Ҫ���Ƶ�λ�õ����꣬�����Ŀ�괰��

    my_draw_fun = &Gkwin->disp->draw;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((Gkwin->wm_bitmap != NULL)
        && (Gkwin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
    {
        //������:��win buffer�л�ͼ����ɫchanged_msk
        if(!my_draw_fun->FillRectToBitmap(bitmap,Rect,Rect,Color,
                                        0,CN_FILLRECT_MODE_N))
        {
            //Ӳ�����ٲ�֧�����λͼ���������ʵ��
            __GK_FillRect(bitmap,Rect,Color);
        }
        __GK_ShadingRect(Gkwin,Rect);//��ɫ��������changed_msk
    }else       //��win buffer����ֱ��д������Ϊtrue
    {
        clip = Gkwin->visible_clip;
        if(clip == NULL)                //���ڿ�����Ϊ�գ�ֱ�ӷ���
            return ;
        fb_gkwin = Gkwin->disp->frame_buffer;
        //�������Ļ��ƣ�����֡�������screen�ϻ��ƣ�ʹ�þ������꣬�任֮
        Rect->left += Gkwin->ScreenX;
        Rect->right += Gkwin->ScreenX;
        Rect->top += Gkwin->ScreenY;
        Rect->bottom += Gkwin->ScreenY;
        //��frame buffer,��ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (Gkwin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            do{
                //ֻ��䴰�ڿ�������Ҫ���Ƶ�����Ľ���
                if(__GK_GetRectInts(&clip->rect,Rect,&ins_rect))
                {
                    //֡��������꼴�������꣬����������任
                    if(!my_draw_fun->FillRectToBitmap(fb_gkwin->wm_bitmap,
                                                  Rect,&ins_rect,Color,
                                                  0,CN_FILLRECT_MODE_N))
                    {
                        //Ӳ�����ٲ�֧�����λͼ���������ʵ��
                        __GK_FillRect(fb_gkwin->wm_bitmap,
                                            &ins_rect,Color);
                    }
                    //��־��������changed_msk
                    __GK_ShadingRect(fb_gkwin,&ins_rect);
                }
                clip = clip->next;
            }while(clip != Gkwin->visible_clip);
        }
        else
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{
                struct DisplayObj *MirrorDisplay;
                struct Object *mirror,*current;
                //ֻ��䴰�ڿ�������Ҫ���Ƶ�����Ľ���
                if(__GK_GetRectInts(&clip->rect,Rect,&ins_rect))
                {
                    //Ӳ�����ٲ�֧�����λͼ���������ʵ��
                    if(!my_draw_fun->FillRectToScreen(Rect,&ins_rect,
                                             Color,0,CN_FILLRECT_MODE_N))
                    {
                        __GK_GradientFillScreenRect(my_draw_fun,Rect,
                                    &ins_rect,Color,0,CN_FILLRECT_MODE_N);
                    }
                    mirror = Gkwin->disp->HostObj;
                    current = OBJ_GetChild(mirror);
                    while(current != NULL)
                    {
                        MirrorDisplay = (struct DisplayObj*)OBJ_GetPrivate(current);
                        //Ӳ�����ٲ�֧�����λͼ���������ʵ��
                        if(!MirrorDisplay->draw.FillRectToScreen(Rect,&ins_rect,
                                              Color,0,CN_FILLRECT_MODE_N))
                        {
                            __GK_GradientFillScreenRect(&MirrorDisplay->draw,Rect,
                                    &ins_rect,Color,0,CN_FILLRECT_MODE_N);
                        }
                        current = OBJ_ForeachChild(mirror,current);
                    }
                }
                clip = clip->next;
            }while(clip != Gkwin->visible_clip);
        }
    }
}

//----����ɫ��䴰�ڵ�һ����---------------------------------------------------
//����: ��ָ������ɫ��䴰�ڵ�һ���֡�
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��
//      rect�������ľ���(ʹ���������)
//      color�����ʹ�õ���ɫ
//����: ��
//-----------------------------------------------------------------------------
void __GK_GradientFillRect(struct GkscParaGradientFillWin *para)
{
    struct RectBitmap *bitmap;
    struct Rectangle target,ins_rect;
    struct DispDraw *my_draw_fun;
    struct ClipRect *clip;
    struct GkWinObj *fb_gkwin,*DstGkwin;
    u32 Color0,Color1,Mode;
    struct Rectangle limit;

    DstGkwin = para->gkwin;
    bitmap = DstGkwin->wm_bitmap;

    if(para->range.left == para->range.right)
    {
        limit = DstGkwin->limit;
    }
    else
    {
        __GK_GetRectInts(&para->range, &DstGkwin->limit, &limit);
    }

    if(!__GK_GetRectInts(&para->rect,&limit,&target))
    {
        return;
    }

    Mode = para->Mode;
    if(Mode== CN_FILLRECT_MODE_N)
    {
        __GK_FillPartWin(DstGkwin,&target,para->Color0);
        return;
    }

    Color0 = para->Color0;
    Color1 = para->Color1;
    my_draw_fun = &DstGkwin->disp->draw;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((DstGkwin->wm_bitmap != NULL)
        && (DstGkwin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
    {
        //������:��win buffer�л�ͼ����ɫchanged_msk
        if(!my_draw_fun->FillRectToBitmap(bitmap,&target,&target,Color0,Color1,Mode))
        {
            //Ӳ�����ٲ�֧�����λͼ���������ʵ��
            __GK_GradientFillRectSoft(bitmap,&target,&target,Color0,Color1,Mode);
        }
        __GK_ShadingRect(DstGkwin,&para->rect);//��ɫ��������changed_msk
    }else       //��win buffer����ֱ��д������Ϊtrue
    {
        clip = DstGkwin->visible_clip;
        if(clip == NULL)                //���ڿ�����Ϊ�գ�ֱ�ӷ���
            return ;
        fb_gkwin = DstGkwin->disp->frame_buffer;
        //�������Ļ��ƣ�����֡�������screen�ϻ��ƣ�ʹ�þ������꣬�任֮
        target.left += DstGkwin->ScreenX;
        target.right += DstGkwin->ScreenX;
        target.top += DstGkwin->ScreenY;
        target.bottom += DstGkwin->ScreenY;
        //��frame buffer,��ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (DstGkwin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            do{
                //ֻ��䴰�ڿ�������Ҫ���Ƶ�����Ľ���
                if(__GK_GetRectInts(&clip->rect,&target,&ins_rect))
                {
                    //֡��������꼴�������꣬����������任
                    if(!my_draw_fun->FillRectToBitmap(fb_gkwin->wm_bitmap,
                                         &target,&ins_rect,Color0,Color1,Mode))
                    {
                        //Ӳ�����ٲ�֧�����λͼ���������ʵ��
                        __GK_GradientFillRectSoft(fb_gkwin->wm_bitmap,&target,
                                                &ins_rect,Color0,Color1,Mode);
                    }
                    //��־��������changed_msk
                    __GK_ShadingRect(fb_gkwin,&ins_rect);
                }
                clip = clip->next;
            }while(clip != DstGkwin->visible_clip);
        }
        else
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{
                struct DisplayObj *MirrorDisplay;
                struct Object *mirror,*current;
                //ֻ��䴰�ڿ�������Ҫ���Ƶ�����Ľ���
                if(__GK_GetRectInts(&clip->rect,&target,&ins_rect))
                {
                    //Ӳ�����ٲ�֧�����λͼ���������ʵ��
                    if(!my_draw_fun->FillRectToScreen(&target,&ins_rect,
                                                      Color0,Color1,Mode))
                    {
                        __GK_GradientFillScreenRect(my_draw_fun,&target,
                                                &ins_rect,Color0,Color1,Mode);
                    }
                    mirror = DstGkwin->disp->HostObj;
                    current = OBJ_GetChild(mirror);
                    while(current != NULL)
                    {
                        MirrorDisplay = (struct DisplayObj*)OBJ_GetPrivate(current);
                        if(!MirrorDisplay->draw.FillRectToScreen(&target,&ins_rect,
                                                       Color0,Color1,Mode) )
                        {
                            __GK_GradientFillScreenRect(&MirrorDisplay->draw,&target,
                                                &ins_rect,Color0,Color1,Mode);
                        }
                        current = OBJ_ForeachChild(mirror,current);
                    }
                }
                clip = clip->next;
            }while(clip != DstGkwin->visible_clip);
        }
    }
}

//----��䴰��-----------------------------------------------------------------
//����: ��ָ������ɫ��䴰�ڡ�
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��ָ��
//      color�����ʹ�õ���ɫ
//����: ��
//-----------------------------------------------------------------------------
void __GK_FillWin(struct GkscParaFillWin *para)
{
    struct Rectangle rect;
    struct ClipRect *clip;
    struct GkWinObj *fb_gkwin,*fpwin;
    struct RectBitmap *bitmap;

    struct DispDraw *my_draw_fun;
    fpwin = para->gkwin;
    bitmap = fpwin->wm_bitmap;
    my_draw_fun = &fpwin->disp->draw;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((bitmap!=NULL)&&(fpwin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
    {
        rect.left = 0;
        rect.top = 0;
        rect.right = bitmap->width;
        rect.bottom = bitmap->height;
        //������:��win buffer�л�ͼ����־changed_msk
        if(!my_draw_fun->FillRectToBitmap(bitmap,&rect,&rect,para->color,
                                        0,CN_FILLRECT_MODE_N))
        {//Ӳ�����ٲ�֧�����λͼ���������ʵ��
            __GK_FillBm(bitmap,para->color);
        }
        fpwin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_ALL;
    }
    else       //��win buffer����ֱ��д������Ϊtrue
    {
        clip = fpwin->visible_clip;
        if(clip == NULL)
            return ;
        fb_gkwin = fpwin->disp->frame_buffer;
        bitmap = fb_gkwin->wm_bitmap;
        rect.left = 0;
        rect.top = 0;
        rect.right = bitmap->width;
        rect.bottom = bitmap->height;
        //��frame buffer����ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (fpwin->WinProperty.DirectDraw == CN_GKWIN_UNDIRECT_DRAW))
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            do{
                //֡����������clip�е����궼�Ǿ������꣬����任
                if(!my_draw_fun->FillRectToBitmap(bitmap,&rect,&clip->rect,
                                              para->color,0,CN_FILLRECT_MODE_N))
                {//Ӳ�����ٲ�֧�����λͼ���������ʵ��
                    __GK_FillRect(bitmap,&clip->rect,para->color);
                }
                //��־��������changed_msk
                __GK_ShadingRect(fb_gkwin,&clip->rect);
                clip = clip->next;
            }while(clip != fpwin->visible_clip);
        }else
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{
                struct DisplayObj *MirrorDisplay;
                struct Object *mirror,*current;
                if(!my_draw_fun->FillRectToScreen(&rect,&clip->rect,
                                            para->color,0,CN_FILLRECT_MODE_N))
                {
                    //Ӳ�����ٲ�֧�����screen�ϵľ������������ʵ��
                    __GK_GradientFillScreenRect(my_draw_fun,&rect,&clip->rect,
                                            para->color,0,CN_FILLRECT_MODE_N);
                }
                mirror = fpwin->disp->HostObj;
                current = OBJ_GetChild(mirror);
                while(current != NULL)
                {
                    MirrorDisplay = (struct DisplayObj*)OBJ_GetPrivate(current);
                    if(!MirrorDisplay->draw.FillRectToScreen(&rect,&clip->rect,
                                            para->color,0,CN_FILLRECT_MODE_N))
                    {
                        //Ӳ�����ٲ�֧�����screen�ϵľ������������ʵ��
                        __GK_GradientFillScreenRect(&MirrorDisplay->draw,&rect,
                                                    &clip->rect,
                                                    para->color,
                                                    0,CN_FILLRECT_MODE_N);
                    }
                    current = OBJ_ForeachChild(mirror,current);
                }
                clip = clip->next;
            }while(clip != fpwin->visible_clip);
        }
    }
}


