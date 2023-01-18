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
//文件描述: 公共代码集
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2014-11-10
//   作者:  LiuWei.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include <align.h>
#include    "gdd_private.h"

#pragma pack(1)

typedef struct
{
    u16 bfType;       /*   文件类型,   必须为   "BM "   (0x4D42)   */
    u32 bfSize;       /*   文件的大小(字节)   */
    u16 bfRserved1;   /*   保留,   必须为   0   */
    u16 bfReserved2;  /*   保留,   必须为   0   */
    u32 bfOffBits;    /*   位图阵列相对于文件头的偏移量(字节)   */
}tagBITMAPFILEHEADER;

#pragma pack()


/*============================================================================*/

typedef struct
{
    u32 biSize;           /* size   of   BITMAPINFOHEADER   */
    u32 biWidth;          /* 位图宽度(像素)   */
    u32 biHeight;         /* 位图高度(像素)   */
    u16 biPlanes;         /* 目标设备的位平面数,   必须置为1   */
    u16 biBitCount;       /* 每个像素的位数,   1,4,8或24   */
    u32 biCompression;    /* 位图阵列的压缩方法,0=不压缩   */
    u32 biSizeImage;      /* 图像大小(字节)   */
    u32 biXPelsPerMeter;  /* 目标设备水平每米像素个数   */
    u32 biYPelsPerMeter;  /* 目标设备垂直每米像素个数   */
    u32 biColorUsed;      /* 位图实际使用的颜色表的颜色数   */
    u32 biColorImportant; /* 重要颜色索引的个数   */

}tagBITMAPINFOHEADER;

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L

//-----------------------------------------------------------------------------
//功能：绘制BMP文件格式的二进制图像数据，图像数据是已二进制数组的形式保存在程序中或者
//      内存中的，如果图像存储在文件系统中，请用 GDD_DrawBMPFile 函数
//参数：hdc，目标窗口句柄
//      x，y，欲显示的坐标，左上角
//      bmp_data，指向图像数据的指针
//      palette，自定义调色板，对于调色板位图，如果本参数为 NULL，则使用图形文件中的调色板
//              否则使用本调色板。注：16位及以上的真彩色无调色板。
//      HyalineColor，如果hdc中的ropcode使能透明色，此处定义颜色值，否则无效
//返回：true=成功调用，false = 失败
//函数原名： GDD_DrawBMP
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

            line_bytes = align_up(8,BMP->biWidth) >> 3;     //位数字节边界向上对齐，然后÷8
            line_bytes = align_up(4, line_bytes);           //行尺寸4字节对齐

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
            //这里需要执行绘制操作，因为 bm 和 color_tbl 是局部变量
            GDD_PostMessage(hdc->hwnd,MSG_SYNC_DISPLAY,0,0);
            return TRUE;
        }
        ////////

    case    4:
        {
            u32 color_tbl[16];

            line_bytes = align_up(8,BMP->biWidth*4) >> 3;   //位数字节边界向上对齐，然后÷8
            line_bytes = align_up(4, line_bytes);           //行尺寸4字节对齐

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
            //这里需要执行绘制操作，因为 bm 和 color_tbl 是局部变量
            GDD_PostMessage(hdc->hwnd,MSG_SYNC_DISPLAY,0,0);

            return TRUE;
        }
        ////////

    case    8:
        {
            u32 *color_tbl;

            line_bytes = align_up(4, BMP->biWidth);           //行尺寸4字节对齐

            offset = bfOffsetBits-(256*4);
            if (palette == NULL)
            {
                if (align_up(4, bmp_data+offset) == bmp_data+offset)
                    color_tbl = bmp_data+offset;    //如果位图中的调色板地址本身已经对齐，则直接用。
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
            //这里需要执行绘制操作，因为 bm 和 color_tbl 是局部变量
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
        //这里需要执行绘制操作，因为 bm 是局部变量
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
        //这里需要执行绘制操作，因为 bm 是局部变量
        GDD_PostMessage(hdc->hwnd,MSG_SYNC_DISPLAY,0,0);
        return TRUE;
        ////////

    default:
        break;
        ////////
    }

    return FALSE;
}


