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
//�ļ�����: �������뼯
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include <align.h>
#include    "gdd_private.h"

#pragma pack(1)

typedef struct
{
    u16 bfType;       /*   �ļ�����,   ����Ϊ   "BM "   (0x4D42)   */
    u32 bfSize;       /*   �ļ��Ĵ�С(�ֽ�)   */
    u16 bfRserved1;   /*   ����,   ����Ϊ   0   */
    u16 bfReserved2;  /*   ����,   ����Ϊ   0   */
    u32 bfOffBits;    /*   λͼ����������ļ�ͷ��ƫ����(�ֽ�)   */
}tagBITMAPFILEHEADER;

#pragma pack()


/*============================================================================*/

typedef struct
{
    u32 biSize;           /* size   of   BITMAPINFOHEADER   */
    u32 biWidth;          /* λͼ���(����)   */
    u32 biHeight;         /* λͼ�߶�(����)   */
    u16 biPlanes;         /* Ŀ���豸��λƽ����,   ������Ϊ1   */
    u16 biBitCount;       /* ÿ�����ص�λ��,   1,4,8��24   */
    u32 biCompression;    /* λͼ���е�ѹ������,0=��ѹ��   */
    u32 biSizeImage;      /* ͼ���С(�ֽ�)   */
    u32 biXPelsPerMeter;  /* Ŀ���豸ˮƽÿ�����ظ���   */
    u32 biYPelsPerMeter;  /* Ŀ���豸��ֱÿ�����ظ���   */
    u32 biColorUsed;      /* λͼʵ��ʹ�õ���ɫ�����ɫ��   */
    u32 biColorImportant; /* ��Ҫ��ɫ�����ĸ���   */

}tagBITMAPINFOHEADER;

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L

//-----------------------------------------------------------------------------
//���ܣ�����BMP�ļ���ʽ�Ķ�����ͼ�����ݣ�ͼ���������Ѷ������������ʽ�����ڳ����л���
//      �ڴ��еģ����ͼ��洢���ļ�ϵͳ�У����� GDD_DrawBMPFile ����
//������hdc��Ŀ�괰�ھ��
//      x��y������ʾ�����꣬���Ͻ�
//      bmp_data��ָ��ͼ�����ݵ�ָ��
//      palette���Զ����ɫ�壬���ڵ�ɫ��λͼ�����������Ϊ NULL����ʹ��ͼ���ļ��еĵ�ɫ��
//              ����ʹ�ñ���ɫ�塣ע��16λ�����ϵ����ɫ�޵�ɫ�塣
//      HyalineColor�����hdc�е�ropcodeʹ��͸��ɫ���˴�������ɫֵ��������Ч
//���أ�true=�ɹ����ã�false = ʧ��
//����ԭ���� GDD_DrawBMP
//------------------------------------------------------------------------------
bool_t GDD_DrawWinBMPArray(HDC hdc,s32 x,s32 y,const void *bmp_data,u32 *palette,
                        u32 HyalineColor)
{
    tagBMP_HEADER *BMP;
    struct RectBitmap bm;
    s32 line_bytes, w, h, bfOffsetBits;
    u32 offset;

    if (NULL == bmp_data )
    {
        return FALSE;
    }
    BMP = bmp_data;

    if (BMP->bfType != 0x4D42)
    {
        return FALSE;
    }

    bfOffsetBits =BMP->bfOffBits;

    w = BMP->biWidth;
    h = BMP->biHeight;

    if (h < 0)
    {
        bm.reversal = 0;
        h = 0-h;
    }
    else
        bm.reversal = 1;
    switch (BMP->biBitCount)
    {
    case    1:
        {
            u32 color_tbl[2];

            line_bytes = align_up(8,BMP->biWidth) >> 3;     //λ���ֽڱ߽����϶��룬Ȼ���8
            line_bytes = align_up(4, line_bytes);           //�гߴ�4�ֽڶ���

            offset =bfOffsetBits-(2*4);
            if (palette == NULL)
                memcpy((u8*)color_tbl, bmp_data+offset,2*4);

            bm.PixelFormat  =CN_SYS_PF_PALETTE1;
            bm.width        =w;
            bm.height       =h;
            bm.linebytes    =line_bytes;
            bm.ExColor      =(palette==NULL)?(ptu32_t)color_tbl:(ptu32_t)palette;
            bm.bm_bits      =(u8*)bmp_data+bfOffsetBits;
            GDD_DrawBitmap(hdc, x, y, &bm, HyalineColor);
            //������Ҫִ�л��Ʋ�������Ϊ bm �� color_tbl �Ǿֲ�����
            GDD_PostMessage(hdc->hwnd,MSG_SYNC_DISPLAY,0,0);
            return TRUE;
        }
        ////////

    case    4:
        {
            u32 color_tbl[16];

            line_bytes = align_up(8,BMP->biWidth*4) >> 3;   //λ���ֽڱ߽����϶��룬Ȼ���8
            line_bytes = align_up(4, line_bytes);           //�гߴ�4�ֽڶ���

            offset = bfOffsetBits-(16*4);
            if (palette == NULL)
                memcpy((u8*)color_tbl, bmp_data+offset,16*4);

            bm.PixelFormat  =CN_SYS_PF_PALETTE4;
            bm.width        =w;
            bm.height       =h;
            bm.linebytes    =line_bytes;
            bm.ExColor      =(palette==NULL)?(ptu32_t)color_tbl:(ptu32_t)palette;
            bm.bm_bits      =(u8*)bmp_data + bfOffsetBits;
            GDD_DrawBitmap(hdc, x, y, &bm, HyalineColor);
            //������Ҫִ�л��Ʋ�������Ϊ bm �� color_tbl �Ǿֲ�����
            GDD_PostMessage(hdc->hwnd,MSG_SYNC_DISPLAY,0,0);

            return TRUE;
        }
        ////////

    case    8:
        {
            u32 *color_tbl;

            line_bytes = align_up(4, BMP->biWidth);           //�гߴ�4�ֽڶ���

            offset = bfOffsetBits-(256*4);
            if (palette == NULL)
            {
                if (align_up(4, bmp_data+offset) == bmp_data+offset)
                    color_tbl = bmp_data+offset;    //���λͼ�еĵ�ɫ���ַ�����Ѿ����룬��ֱ���á�
                else
                {
                    color_tbl = malloc(256*4);
                    if (color_tbl == NULL)
                        return false;
                    memcpy((u8*)color_tbl, bmp_data+offset,16*4);
                }
            }

            bm.PixelFormat  =CN_SYS_PF_PALETTE4;
            bm.width        =w;
            bm.height       =h;
            bm.linebytes    =line_bytes;
            bm.ExColor      =(palette==NULL)?(ptu32_t)color_tbl:(ptu32_t)palette;
            bm.bm_bits      =(u8*)bmp_data + bfOffsetBits;
            GDD_DrawBitmap(hdc, x, y, &bm, HyalineColor);
            //������Ҫִ�л��Ʋ�������Ϊ bm �� color_tbl �Ǿֲ�����
            GDD_PostMessage(hdc->hwnd,MSG_SYNC_DISPLAY,0,0);
            free(color_tbl);
            return TRUE;
        }
        ////////

    case 16:

        line_bytes = BMP->biWidth * 2;
        line_bytes = align_up(4, line_bytes);

        bm.PixelFormat =CN_SYS_PF_RGB565;
        bm.width  =w;
        bm.height =h;
        bm.linebytes =line_bytes;
        bm.ExColor =(ptu32_t)0;
        bm.bm_bits =(u8*)bmp_data + bfOffsetBits;
        GDD_DrawBitmap(hdc,x,y,&bm,HyalineColor);
        //������Ҫִ�л��Ʋ�������Ϊ bm �Ǿֲ�����
        GDD_PostMessage(hdc->hwnd,MSG_SYNC_DISPLAY,0,0);
        return TRUE;

    case 24:

        line_bytes = BMP->biWidth * 3;
        line_bytes = align_up(4, line_bytes);
        bm.PixelFormat =CN_SYS_PF_RGB888;
        bm.width  =w;
        bm.height =h;
        bm.linebytes =line_bytes;
        bm.ExColor =(ptu32_t)0;
        bm.bm_bits = (u8*)bmp_data + bfOffsetBits;
        GDD_DrawBitmap(hdc,x,y,&bm,HyalineColor);
        //������Ҫִ�л��Ʋ�������Ϊ bm �Ǿֲ�����
        GDD_PostMessage(hdc->hwnd,MSG_SYNC_DISPLAY,0,0);
        return TRUE;
        ////////

    default:
        break;
        ////////
    }

    return FALSE;
}


