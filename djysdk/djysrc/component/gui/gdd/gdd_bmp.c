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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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

#include    <gui/gdd/gdd_private.h>

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


typedef long FXPT16DOT16, *LPFXPT16DOT16;
typedef long FXPT2DOT30, *LPFXPT2DOT30;

typedef struct tagCIEXYZ
{
    FXPT2DOT30 ciexyzX;
    FXPT2DOT30 ciexyzY;
    FXPT2DOT30 ciexyzZ;
} CIEXYZ;
typedef CIEXYZ *LPCIEXYZ;

typedef struct tagICEXYZTRIPLE
{
    CIEXYZ ciexyzRed;
    CIEXYZ ciexyzGreen;
    CIEXYZ ciexyzBlue;
} CIEXYZTRIPLE;
typedef CIEXYZTRIPLE *LPCIEXYZTRIPLE;

typedef struct
{
    u32 biSize;
    u32 biWidth;
    u32 biHeight;
    u16 biPlanes;
    u16 biBitCount;
    u32 biCompression;
    u32 biSizeImage;
    u32 biXPelsPerMeter;
    u32 biYPelsPerMeter;
    u32 biColorUsed;
    u32 biColorImportant;

    u32 biRedMask;
    u32 biGreenMask;
    u32 biBlueMask;
    u32 biAlphaMask;

    u32 biCSType;
    CIEXYZTRIPLE biEndpoints;
    u32 biGammaRed;
    u32 biGammaGreen;
    u32 biGammaBlue;
} BITMAPHEADER_V4;

typedef struct
{
    u32 biSize;
    u32 biWidth;
    u32 biHeight;
    u16 biPlanes;
    u16 biBitCount;
    u32 biCompression;
    u32 biSizeImage;
    u32 biXPelsPerMeter;
    u32 biYPelsPerMeter;
    u32 biColorUsed;
    u32 biColorImportant;

    u32 biRedMask;
    u32 biGreenMask;
    u32 biBlueMask;
    u32 biAlphaMask;

    u32 biCSType;
    CIEXYZTRIPLE biEndpoints;
    u32 biGammaRed;
    u32 biGammaGreen;
    u32 biGammaBlue;

    u32 biIntent;
    u32 biProfileData;
    u32 biProfileSize;
    u32 biReserved;
} BITMAPHEADER_V5;


#define PROFILE_LINKED          'LINK'
#define PROFILE_EMBEDDED        'MBED'


#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L

/*============================================================================*/
typedef    s32 FN_READ_DATA(u8 *buf,u32 offset,u32 size,const void *pdata);

typedef    struct{
    const void *pData;
    FN_READ_DATA *pfReadData;
}GUI_GET_DATA;

/*============================================================================*/

static s32  __ReadDataFromMemory(u8 *buf,u32 offset,u32 size,const void *pdata)
{
    memcpy(buf,(u8*)pdata+offset,size);
    return size;
}

/*============================================================================*/

bool_t GetBMPInfo(tagBMP_INFO *bm_info,tagBMP_HEADER *pBmpHdr)
{
    s32 line_bytes;

    if (bm_info == NULL )
    {
        return FALSE;
    }

    if (pBmpHdr->bfType != 0x4D42)
    {
        return FALSE;
    }

    bm_info->Width = pBmpHdr->biWidth;
    bm_info->Height = pBmpHdr->biHeight;

    switch (pBmpHdr->biBitCount)
    {
    case 1:
        line_bytes = pBmpHdr->biWidth >> 3;
        if (line_bytes & 0x03)
        {
            line_bytes += 4 - (line_bytes & 0x03);
        }
        bm_info->LineBytes = line_bytes;
        bm_info->Format = CN_SYS_PF_PALETTE1;
        return TRUE;
        ////

    case 4:
        line_bytes = pBmpHdr->biWidth >> 1;
        if (line_bytes & 0x03)
        {
            line_bytes += 4 - (line_bytes & 0x03);
        }
        bm_info->LineBytes = line_bytes;
        bm_info->Format = CN_SYS_PF_PALETTE4;
        return TRUE;
        ////

    case 8:
        line_bytes = pBmpHdr->biWidth;
        if (line_bytes & 0x03)
        {
            line_bytes += 4 - (line_bytes & 0x03);
        }
        bm_info->LineBytes = line_bytes;
        bm_info->Format = CN_SYS_PF_PALETTE8;
        return TRUE;
        ////

    case 16:
        line_bytes = pBmpHdr->biWidth * 2;
        if (line_bytes & 0x03)
        {
            line_bytes += 4 - (line_bytes & 0x03);
        }
        bm_info->LineBytes = line_bytes;
        bm_info->Format = CN_SYS_PF_RGB565;
        return TRUE;
        ////

    case 24:
        line_bytes = pBmpHdr->biWidth * 3;
        if (line_bytes & 0x03)
        {
            line_bytes += 4 - (line_bytes & 0x03);
        }
        bm_info->LineBytes = line_bytes;
        bm_info->Format = CN_SYS_PF_RGB888;
        return TRUE;
        ////

    default:
        bm_info->Format = 0xFFFF;
        bm_info->Width = 0;
        bm_info->Height = 0;
        bm_info->LineBytes = 0;
        return FALSE;

    }

    return FALSE;

}

/*============================================================================*/

bool_t __DrawBMP(HDC hdc, s32 x, s32 y, GUI_GET_DATA *rd)
{
    u32 buf[64 / 4];
    BITMAPHEADER_V4 *pBmp;
    tagBITMAPFILEHEADER *p_bmp_hdr;
    BITMAP *bm;
    s32 i, line_bytes, w, h, bfOffsetBits, xoff, yoff;
    u32 offset;
    struct RopGroup RopCode = (struct RopGroup){ 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };
    ////

    if (rd == NULL )
    {
        return FALSE;
    }

    if (rd->pfReadData == NULL )
    {
        return FALSE;
    }

    offset = 0;
    rd->pfReadData((u8*)buf, offset, 14, rd->pData);

    p_bmp_hdr =(tagBITMAPFILEHEADER*)buf;
    if (p_bmp_hdr->bfType != 0x4D42)
    {
        return FALSE;
    }

    bfOffsetBits =p_bmp_hdr->bfOffBits;

    offset = 14;
    rd->pfReadData((u8*)buf, offset, 56, rd->pData);
    pBmp = (BITMAPHEADER_V4*) buf;

    xoff = 0;
    yoff = 0;
    w = pBmp->biWidth;
    h = pBmp->biHeight;

    switch (pBmp->biBitCount)
    {
    case    1:
        {

            u32 *color_tbl;

            line_bytes = pBmp->biWidth >> 3;
            if (line_bytes & 0x03)
            {
                line_bytes += 4 - (line_bytes & 0x03);
            }

            offset =bfOffsetBits-(2*4);
            color_tbl =(u32*)malloc(2*4+line_bytes);

            for(i=0;i<2;i++)
            {
                u32 xrgb;
                rd->pfReadData((u8*)&xrgb,offset,4,rd->pData);
                color_tbl[i]=xrgb;
                offset += 4;
            }

            offset = bfOffsetBits;
            offset += pBmp->biHeight * line_bytes - line_bytes;
            offset -= yoff * line_bytes;

            bm    =(BITMAP*)buf;
            bm->PixelFormat  =CN_SYS_PF_PALETTE1;
            bm->width        =w;
            bm->height       =1;
            bm->linebytes    =line_bytes;
            bm->ExColor      =(ptu32_t)color_tbl;
            bm->bm_bits      =(u8*)color_tbl+(2*4);

            for (i = 0; i < h; i++)
            {
                rd->pfReadData((u8*)bm->bm_bits, offset, line_bytes, rd->pData);
                DrawBitmap(hdc, x, y + i, bm, 0,RopCode);
                offset -= line_bytes;
            }

            free(color_tbl);
            return TRUE;
        }
        ////////

    case    4:
        {
            u32 *color_tbl;

            line_bytes = pBmp->biWidth >> 1;
            if (line_bytes & 0x03)
            {
                line_bytes += 4 - (line_bytes & 0x03);
            }

            offset =bfOffsetBits-(16*4);
            color_tbl =(u32*)malloc(16*4+line_bytes);

            for(i=0;i<16;i++)
            {
                u32 xrgb;
                rd->pfReadData((u8*)&xrgb,offset,4,rd->pData);
                color_tbl[i]=xrgb;
                offset += 4;
            }

            offset = bfOffsetBits;
            offset += pBmp->biHeight * line_bytes - line_bytes;
            offset -= yoff * line_bytes;

            bm    =(BITMAP*)buf;
            bm->PixelFormat  =CN_SYS_PF_PALETTE4;
            bm->width        =w;
            bm->height       =1;
            bm->linebytes    =line_bytes;
            bm->ExColor      =(ptu32_t)color_tbl;
            bm->bm_bits      =(u8*)color_tbl+(16*4);

            for (i = 0; i < h; i++)
            {
                rd->pfReadData((u8*)bm->bm_bits, offset, line_bytes, rd->pData);
                DrawBitmap(hdc, x, y + i, bm, 0,RopCode);
                offset -= line_bytes;
            }

            free(color_tbl);
            return TRUE;
        }
        ////////

    case    8:
        {
            u32 *color_tbl;

            line_bytes = pBmp->biWidth;
            if (line_bytes & 0x03)
            {
                line_bytes += 4 - (line_bytes & 0x03);
            }

            offset =bfOffsetBits-(256*4);
            color_tbl =(u32*)malloc(256*4+line_bytes);

            for(i=0;i<256;i++)
            {
                u32 xrgb;
                rd->pfReadData((u8*)&xrgb,offset,4,rd->pData);
                color_tbl[i]=xrgb;
                offset += 4;
            }

            offset = bfOffsetBits;
            offset += pBmp->biHeight * line_bytes - line_bytes;
            offset -= yoff * line_bytes;
            offset += xoff;

            bm    =(BITMAP*)buf;
            bm->PixelFormat  =CN_SYS_PF_PALETTE8;
            bm->width        =w;
            bm->height       =1;
            bm->linebytes    =line_bytes;
            bm->ExColor      =(ptu32_t)color_tbl;
            bm->bm_bits      =(u8*)color_tbl+(256*4);

            for (i = 0; i < h; i++)
            {
                rd->pfReadData((u8*)bm->bm_bits, offset, line_bytes, rd->pData);
                DrawBitmap(hdc, x, y + i, bm,0,RopCode);
                offset -= line_bytes;
            }

            free(color_tbl);
            return TRUE;
        }
        ////////

    case 16:

        line_bytes = pBmp->biWidth * 2;
        if (line_bytes & 0x03)
        {
            line_bytes += 4 - (line_bytes & 0x03);
        }

        offset  = bfOffsetBits;
        offset += pBmp->biHeight * line_bytes - line_bytes;
        offset -= yoff * line_bytes;
        offset += xoff * 2;

        bm = (BITMAP*)buf;
        bm->PixelFormat =CN_SYS_PF_RGB565;
        bm->width  =w;
        bm->height =h;
        bm->linebytes =line_bytes;
        bm->ExColor =(ptu32_t)0;
//      bm->bm_bits =malloc(line_bytes);
        bm->bm_bits =rd->pData + bfOffsetBits;
        DrawBitmap(hdc,x,y,bm,0,RopCode);

//      if(bm->bm_bits!=NULL)
//      {
//          for (i = 0; i < h; i++)
//          {
//              rd->pfReadData((u8*)bm->bm_bits,offset,line_bytes,rd->pData);
//              DrawBitmap(hdc,x,y+i,bm,0,RopCode);
//              offset -= line_bytes;
//          }
//
//          free(bm->bm_bits);
//      }
        return TRUE;
        ////////

    case 24:

        line_bytes = pBmp->biWidth * 3;
        if (line_bytes & 0x03)
        {
            line_bytes += 4 - (line_bytes & 0x03);
        }

        offset  = bfOffsetBits;
        offset += pBmp->biHeight * line_bytes - line_bytes;
        offset -= yoff * line_bytes;
        offset += xoff * 3;

        bm = (BITMAP*)buf;
        bm->PixelFormat =CN_SYS_PF_RGB888;
        bm->width  =w;
        bm->height =1;
        bm->linebytes =line_bytes;
        bm->ExColor =(ptu32_t)0;
        bm->bm_bits =malloc(line_bytes);

        if(bm->bm_bits!=NULL)
        {
            for (i = 0; i < h; i++)
            {
                rd->pfReadData((u8*)bm->bm_bits,offset,line_bytes,rd->pData);
                DrawBitmap(hdc,x,y+i,bm,0,RopCode);
                offset -= line_bytes;
            }

            free(bm->bm_bits);
        }
        return TRUE;
        ////////

    default:
        break;
        ////////
    }

    return FALSE;
}

/*============================================================================*/

bool_t DrawBMP(HDC hdc,s32 x,s32 y,const void *bmp_data)
{
    GUI_GET_DATA rd_data;
    rd_data.pData =bmp_data;
    rd_data.pfReadData =__ReadDataFromMemory;
    return __DrawBMP(hdc,x,y,&rd_data);
}


/*============================================================================*/
