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

//����ģ��: ASCII 8x8������������
//���ߣ�mjh
//�汾��V1.0.0
//�ļ�����: ASCII 8x8�����������
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2011-05-17
//       ����:mjh
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stdio.h"
#include "gkernel.h"
#include "dbug.h"
#include "./font/font.h"
#include "./font/font_ascii.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���

//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    #if CFG_ASCII_8X8 == 1
//    extern bool_t ModuleInstall_FontAscii8x8(void);
//    ModuleInstall_FontAscii8x8( );
//    #endif      //CFG_ASCII_8X8 == 1
//    #if CFG_ASCII_6X12 == 1
//    extern bool_t ModuleInstall_FontAscii6x12(void);
//    ModuleInstall_FontAscii6x12( );
//    #endif      //CFG_ASCII_6X12 == 1
//
//    #if CFG_ASCII_8X16_SONG == 1
//    extern bool_t ModuleInstall_FontAscii8x16(void);
//    ModuleInstall_FontAscii8x16( );
//    #endif      //CFG_ASCII_8X16_SONG == 1
//    #if CFG_ASCII_8X16_YUAN == 1
//    extern bool_t ModuleInstall_FontAscii8x16yuan(void);
//    ModuleInstall_FontAscii8x16yuan( );
//    #endif      //CFG_ASCII_8X16_YUAN == 1
//    #if CFG_ASCII_8X16_KAI == 1
//    extern bool_t ModuleInstall_FontAscii8x16kai(void);
//    ModuleInstall_FontAscii8x16kai( );
//    #endif      //CFG_ASCII_8X16_KAI == 1
//    #if CFG_ASCII_8X16_HEI == 1
//    extern bool_t ModuleInstall_FontAscii8x16hei(void);
//    ModuleInstall_FontAscii8x16hei( );
//    #endif      //CFG_ASCII_8X16_HEI == 1
//    #if CFG_ASCII_8X16_FANG == 1
//    extern bool_t ModuleInstall_FontAscii8x16fang(void);
//    ModuleInstall_FontAscii8x16fang( );
//    #endif      //CFG_ASCII_8X16_FANG == 1
//
//    #if CFG_ASCII_12X24_SONG == 1
//    extern bool_t ModuleInstall_FontAscii12x24(void);
//    ModuleInstall_FontAscii12x24( );
//    #endif      //CFG_ASCII_12X24_SONG == 1
//    #if CFG_ASCII_12X24_YUAN == 1
//    extern bool_t ModuleInstall_FontAscii12x24yuan(void);
//    ModuleInstall_FontAscii12x24yuan( );
//    #endif      //CFG_ASCII_12X24_YUAN == 1
//    #if CFG_ASCII_12X24_KAI == 1
//    extern bool_t ModuleInstall_FontAscii12x24kai(void);
//    ModuleInstall_FontAscii12x24kai( );
//    #endif      //CFG_ASCII_12X24_KAI == 1
//    #if CFG_ASCII_12X24_HEI == 1
//    extern bool_t ModuleInstall_FontAscii12x24hei(void);
//    ModuleInstall_FontAscii12x24hei( );
//    #endif      //CFG_ASCII_12X24_HEI == 1
//    #if CFG_ASCII_12X24_FANG == 1
//    extern bool_t ModuleInstall_FontAscii12x24fang(void);
//    ModuleInstall_FontAscii12x24fang( );
//    #endif      //CFG_ASCII_12X24_FANG == 1
//
//    #if CFG_ASCII_16X32_SONG == 1
//    extern bool_t ModuleInstall_FontAscii16x32(void);
//    ModuleInstall_FontAscii16x32( );
//    #endif      //CFG_ASCII_16X32_SONG == 1
//    #if CFG_ASCII_16X32_YUAN == 1
//    extern bool_t ModuleInstall_FontAscii16x32yuan(void);
//    ModuleInstall_FontAscii16x32yuan( );
//    #endif      //CFG_ASCII_16X32_YUAN == 1
//    #if CFG_ASCII_16X32_KAI == 1
//    extern bool_t ModuleInstall_FontAscii16x32kai(void);
//    ModuleInstall_FontAscii16x32kai( );
//    #endif      //CFG_ASCII_16X32_KAI == 1
//    #if CFG_ASCII_16X32_HEI == 1
//    extern bool_t ModuleInstall_FontAscii16x32hei(void);
//    ModuleInstall_FontAscii16x32hei( );
//    #endif      //CFG_ASCII_16X32_HEI == 1
//    #if CFG_ASCII_16X32_FANG == 1
//    extern bool_t ModuleInstall_FontAscii16x32fang(void);
//    ModuleInstall_FontAscii16x32fang( );
//    #endif      //CFG_ASCII_16X32_FANG == 1
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"ascii_font"   //��д�����������
//parent:"font"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:�������               //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:init                    //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"font"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_ASCII_8X8           //****�������Ƿ��Ѿ����ú�
#warning    ascii_font�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100,
#define CFG_ASCII_8X8         0         //"ASCII_8��8",8*8����ascii����
#define CFG_ASCII_6X12        0         //"ASCII_6��12",6*12����ascii����
#define CFG_ASCII_8X16_SONG   1         //"ASCII_8��16����",8*16����ascii����_����
#define CFG_ASCII_8X16_YUAN   0         //"ASCII_8��16Բ��",8*16����ascii����_Բ��
#define CFG_ASCII_8X16_KAI    0         //"ASCII_8��16����",8*16����ascii����_����
#define CFG_ASCII_8X16_HEI    0         //"ASCII_8��16����",8*16����ascii����_����
#define CFG_ASCII_8X16_FANG   0         //"ASCII_8��16����",8*16����ascii����_����
#define CFG_ASCII_12X24_SONG  0         //"ASCII_12��24����",12*24����ascii����_����
#define CFG_ASCII_12X24_YUAN  0         //"ASCII_12��24Բ��",12*24����ascii����_Բ��
#define CFG_ASCII_12X24_KAI   0         //"ASCII_12��24����",12*24����ascii����_����
#define CFG_ASCII_12X24_HEI   0         //"ASCII_12��24����",12*24����ascii����_����
#define CFG_ASCII_12X24_FANG  0         //"ASCII_12��24����",12*24����ascii����_����
#define CFG_ASCII_16X32_YUAN  0         //"ASCII_16��32Բ��",16*32����ascii����_Բ��
#define CFG_ASCII_16X32_KAI   0         //"ASCII_16��32����",16*32����ascii����_����
#define CFG_ASCII_16X32_HEI   0         //"ASCII_16��32����",16*32����ascii����_����
#define CFG_ASCII_16X32_FANG  0         //"ASCII_16��32����",16*32����ascii����_����
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


#if CFG_ASCII_8X8 == 1
#include "dot-ascii8x8.h"

bool_t __Font_Ascii8x8LoadFont(void *zk_addr);
void __Font_Ascii8x8UnloadFont(void);
bool_t __Font_Ascii8x8GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

bool_t __Font_Ascii8x8LoadFont(void *zk_addr)
{
    // Nothing ...

    return true;
}

void __Font_Ascii8x8UnloadFont(void)
{
    // Nothing ...
}

#define ASCII8x8_DEF_CHAR_CODE 0x00

//----������ȡ-----------------------------------------------------------------
//����: ��ȡascii 8*8�������壬���charcode����0~0xff�ķ�Χ����ʾ����Ϊ0x00����
//      ����ascii������չ��ascii��
//����: charcode������ʾ��ascii���ucs4����
//      size����Ч
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Ascii8x8GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u32 offset;
    bool_t result = true;


    // �õ��ַ���ģ�������е�ƫ����
    if(charcode > 0xff)
    {
        offset = ASCII8x8_DEF_CHAR_CODE;
        result = false;
    }else
        offset = charcode*8;
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    bitmap->width = 8;
    bitmap->height = 8;
    bitmap->linebytes = 1;
    if(bitmap->bm_bits != NULL)
    {
        for(i=0; i<8; i++)
        {
            bitmap->bm_bits[i] = cs_ascii_8x8[offset + i];
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Ascii8x8GetCharWidth(u32 CharCode)
{
    return 8;
}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Ascii8x8GetCharHeight(u32 CharCode)
{
    return 8;
}

//----��װascii8x8����-------------------------------------------------------
//����: ��װascii���壬��ϵͳʹ�������ַ�����ʱ��ʹ������ַ������ر�ע�⣬
//      gb2312�Ѿ�������Ӣ�����壬��ʹ�����ĵĽ����п��Բ���װascii���塣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontAscii8x8(void)
{
    static struct FontObj font_a8x8;

    font_a8x8.MaxWidth = 8;
    font_a8x8.MaxHeight = 8;
    font_a8x8.Attr = CN_FONT_TYPE_DOT;
    font_a8x8.LoadFont = __Font_Ascii8x8LoadFont;
    font_a8x8.UnloadFont = __Font_Ascii8x8UnloadFont;
    font_a8x8.GetBitmap = __Font_Ascii8x8GetCharBitMap;
    font_a8x8.GetCharWidth = __Font_Ascii8x8GetCharWidth;
    font_a8x8.GetCharHeight = __Font_Ascii8x8GetCharHeight;

    if(Font_InstallFont(&font_a8x8, CN_FONT_ASCII_8x8))
    {
        debug_printf("dot-ascii","ascii 8x8 font install sucess\n\r");
        return true;
    }else
    {
        Djy_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-ascii","ascii 8x8 font install fail\n\r");
        return false;
    }
}
#endif      // CFG_ASCII_8X8 == 1

#if CFG_ASCII_8X16_SONG == 1

#include "dot-ascii8x16song.h"

bool_t __Font_Ascii8x16LoadFont(void *);
void __Font_Ascii8x16UnloadFont(void);
bool_t __Font_Ascii8x16GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);


bool_t __Font_Ascii8x16LoadFont(void* zk)
{
    // Nothing ...

    return true;
}

void __Font_Ascii8x16UnloadFont(void)
{
    // Nothing ...
}

#define ASCII8x16_DEF_CHAR_CODE 0x00

//----������ȡ-----------------------------------------------------------------
//����: ��ȡascii 8*16�������壬���charcode����0~0xff�ķ�Χ����ʾ����Ϊ0x00����
//      ����ascii������չ��ascii��
//����: charcode������ʾ��ascii���ucs4����
//      size����Ч
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Ascii8x16GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u32 offset;
    bool_t result = true;


    // �õ��ַ���ģ�������е�ƫ����
    if(charcode > 0xff)
    {
        offset = ASCII8x16_DEF_CHAR_CODE;
        result = false;
    }else
        offset = charcode*16;
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    bitmap->width = 8;
    bitmap->height = 16;
    bitmap->linebytes = 1;
    if(bitmap->bm_bits != NULL)
    {
        for(i=0; i<16; i++)
        {
            bitmap->bm_bits[i] = cs_ascii_8x16[offset + i];
        }
    }
    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Ascii8x16GetCharWidth(u32 CharCode)
{
    return 6;
}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Ascii8x16GetCharHeight(u32 CharCode)
{
    return 16;
}

//----��װascii8x16����--------------------------------------------------------
//����: ��װascii���壬��ϵͳʹ�������ַ�����ʱ��ʹ������ַ������ر�ע�⣬
//      gb2312�Ѿ�������Ӣ�����壬��ʹ�����ĵĽ����п��Բ���װascii���塣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontAscii8x16(void)
{
    static struct FontObj font_a8x16;

    font_a8x16.MaxWidth = 8;
    font_a8x16.MaxHeight = 16;
    font_a8x16.Attr = CN_FONT_TYPE_DOT;
    font_a8x16.LoadFont = __Font_Ascii8x16LoadFont;
    font_a8x16.UnloadFont = __Font_Ascii8x16UnloadFont;
    font_a8x16.GetBitmap = __Font_Ascii8x16GetCharBitMap;
    font_a8x16.GetCharWidth = __Font_Ascii8x16GetCharWidth;
    font_a8x16.GetCharHeight = __Font_Ascii8x16GetCharHeight;

    if(Font_InstallFont(&font_a8x16, CN_FONT_ASCII_8x16))
    {
        debug_printf("dot-ascii","ascii 8x16 font install sucess\n\r");
        return true;
    }else
    {
        Djy_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-ascii","ascii 8x16 font install fail\n\r");
        return false;
    }
}

#endif      //CFG_ASCII_8X16_SONG == 1

#if CFG_ASCII_6X12 == 1

#include "dot-ascii6x12.h"

bool_t __Font_Ascii6x12LoadFont(void *);
void __Font_Ascii6x12UnloadFont(void);
bool_t __Font_Ascii6x12GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);


bool_t __Font_Ascii6x12LoadFont(void * ziku)
{
    // Nothing ...

    return true;
}

void __Font_Ascii6x12UnloadFont(void)
{
    // Nothing ...
}

#define ASCII6x12_DEF_CHAR_CODE 0x00

//----������ȡ-----------------------------------------------------------------
//����: ��ȡascii 8*16�������壬���charcode����0~0xff�ķ�Χ����ʾ����Ϊ0x00����
//      ����ascii������չ��ascii��
//����: charcode������ʾ��ascii���ucs4����
//      size����Ч
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Ascii6x12GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u32 offset;
    bool_t result = true;


    // �õ��ַ���ģ�������е�ƫ����
    if(charcode > 0xff)
    {
        offset = ASCII6x12_DEF_CHAR_CODE;
        result = false;
    }else
        offset = charcode*16;
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    bitmap->width = 6;
    bitmap->height = 12;
    bitmap->linebytes = 1;
    if(bitmap->bm_bits != NULL)
    {
        for(i=0; i<12; i++)
        {
            bitmap->bm_bits[i] = cs_ascii_6x12[offset + i];
        }
    }
    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Ascii6x12GetCharWidth(u32 CharCode)
{
    return 6;
}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Ascii6x12GetCharHeight(u32 CharCode)
{
    return 12;
}

//----��װascii6x12����--------------------------------------------------------
//����: ��װascii���壬��ϵͳʹ�������ַ�����ʱ��ʹ������ַ������ر�ע�⣬
//      gb2312�Ѿ�������Ӣ�����壬��ʹ�����ĵĽ����п��Բ���װascii���塣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontAscii6x12(void)
{
    static struct FontObj font_a6x12;

    font_a6x12.MaxWidth = 6;
    font_a6x12.MaxHeight = 12;
    font_a6x12.Attr = CN_FONT_TYPE_DOT;
    font_a6x12.LoadFont = __Font_Ascii6x12LoadFont;
    font_a6x12.UnloadFont = __Font_Ascii6x12UnloadFont;
    font_a6x12.GetBitmap = __Font_Ascii6x12GetCharBitMap;
    font_a6x12.GetCharWidth = __Font_Ascii6x12GetCharWidth;
    font_a6x12.GetCharHeight = __Font_Ascii6x12GetCharHeight;

    if(Font_InstallFont(&font_a6x12, CN_FONT_ASCII_6x12))
    {
        debug_printf("dot-ascii","ascii 6x12 font install sucess\n\r");
        return true;
    }else
    {
        Djy_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-ascii","ascii 6x12 font install fail\n\r");
        return false;
    }
}

#endif      //CFG_ASCII_6X12 == 1

#if CFG_ASCII_8X16_YUAN == 1        //Բ��

#include "dot-ascii8x16yuan.h"

#endif      //CFG_ASCII_8X16_YUAN == 1

#if CFG_ASCII_8X16_KAI == 1         //����

#include "dot-ascii8x16kai.h"

#endif      //CFG_ASCII_8X16_KAI == 1

#if CFG_ASCII_8X16_HEI == 1         //����

#include "dot-ascii8x16hei.h"

#endif      //CFG_ASCII_8X16_HEI == 1

#if CFG_ASCII_8X16_FANG == 1         //����

#include "dot-ascii8x16fang.h"

#endif      //CFG_ASCII_8X16_FANG == 1


#if CFG_ASCII_12X24_SONG == 1        //Բ��

#include "dot-ascii12x24song.h"

bool_t __Font_Ascii12x24GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

#define ASCII12x24_DEF_CHAR_CODE 0x00

//----������ȡ-----------------------------------------------------------------
//����: ��ȡascii 12*24�������壬���charcode����0~0xff�ķ�Χ����ʾ����Ϊ0x00����
//      ����ascii������չ��ascii��
//����: charcode������ʾ��ascii���ucs4����
//      size����Ч
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Ascii12x24GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u32 offset;
    bool_t result = true;

    if(charcode > 0xff)
    {
        offset = ASCII12x24_DEF_CHAR_CODE;
        result = false;
    }
    else
        offset = charcode*48;
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    bitmap->width = 12;
    bitmap->height = 24;
    bitmap->linebytes = 2;

    if(bitmap->bm_bits != NULL)//�������ʱ��ử��16x24��ͼ��������һ���ַ����ڿ��Ϊ12��λ�ÿ�ʼ����֮��ĵ�
    {
        for(i=0; i<48; i++)
        {
            bitmap->bm_bits[i] = cs_ascii_12x24[offset + i];
        }
    }
    return result;
}

s32 __Font_Ascii12x24GetCharWidth(u32 CharCode)
{
    return 12;
}

s32 __Font_Ascii12x24GetCharHeight(u32 CharCode)
{
    return 24;
}

//----��װascii12x24����--------------------------------------------------------
//����: ��װascii���壬��ϵͳʹ�������ַ�����ʱ��ʹ������ַ������ر�ע�⣬
//      gb2312�Ѿ�������Ӣ�����壬��ʹ�����ĵĽ����п��Բ���װascii���塣
//����: ������
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontAscii12x24(void)
{
    static struct FontObj font_a12x24;

    font_a12x24.MaxWidth = 12;
    font_a12x24.MaxHeight = 24;
    font_a12x24.Attr = CN_FONT_TYPE_DOT;
    font_a12x24.LoadFont = NULL;
    font_a12x24.UnloadFont = NULL;
    font_a12x24.GetBitmap = __Font_Ascii12x24GetCharBitMap;
    font_a12x24.GetCharWidth = __Font_Ascii12x24GetCharWidth;
    font_a12x24.GetCharHeight = __Font_Ascii12x24GetCharHeight;

    if(Font_InstallFont(&font_a12x24, CN_FONT_ASCII_12x24))
    {
        debug_printf("dot-ascii","ascii 12x24 font install sucess\n\r");
        return true;
    }
    else
    {
        Djy_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-ascii","ascii 12x24 font install fail\n\r");
        return false;
    }
}

#endif      //CFG_ASCII_12X24_SONG == 1

#if CFG_ASCII_12X24_YUAN == 1        //Բ��

#include "dot-ascii12x24yuan.h"

#endif      //CFG_ASCII_12X24_YUAN == 1

#if CFG_ASCII_12X24_KAI == 1         //����

#include "dot-ascii12x24kai.h"

#endif      //CFG_ASCII_12X24_KAI == 1

#if CFG_ASCII_12X24_HEI == 1         //����

#include "dot-ascii12x24hei.h"

#endif      //CFG_ASCII_12X24_HEI == 1

#if CFG_ASCII_12X24_FANG == 1         //����

#include "dot-ascii12x24fang.h"

#endif      //CFG_ASCII_12X24_FANG == 1


#if CFG_ASCII_16X32_SONG == 1        //Բ��

#include "dot-ascii16x32song.h"

bool_t __Font_Ascii16x32GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

#define ASCII16x32_DEF_CHAR_CODE 0x00

//----������ȡ-----------------------------------------------------------------
//����: ��ȡascii 12*24�������壬���charcode����0~0xff�ķ�Χ����ʾ����Ϊ0x00����
//      ����ascii������չ��ascii��
//����: charcode������ʾ��ascii���ucs4����
//      size����Ч
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Ascii16x32GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u32 offset;
    bool_t result = true;

    if(charcode > 0xff)
    {
        offset = ASCII16x32_DEF_CHAR_CODE;
        result = false;
    }
    else
        offset = charcode*48;
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    bitmap->width = 12;
    bitmap->height = 24;
    bitmap->linebytes = 2;

    if(bitmap->bm_bits != NULL)//�������ʱ��ử��16x24��ͼ��������һ���ַ����ڿ��Ϊ12��λ�ÿ�ʼ����֮��ĵ�
    {
        for(i=0; i<48; i++)
        {
            bitmap->bm_bits[i] = cs_ascii_16x32[offset + i];
        }
    }
    return result;
}

s32 __Font_Ascii16x32GetCharWidth(u32 CharCode)
{
    return 16;
}

s32 __Font_Ascii16x32GetCharHeight(u32 CharCode)
{
    return 32;
}

//----��װascii16x32����--------------------------------------------------------
//����: ��װascii���壬��ϵͳʹ�������ַ�����ʱ��ʹ������ַ������ر�ע�⣬
//      gb2312�Ѿ�������Ӣ�����壬��ʹ�����ĵĽ����п��Բ���װascii���塣
//����: ������
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontAscii16x32(void)
{
    static struct FontObj font_a16x32;

    font_a16x32.MaxWidth = 16;
    font_a16x32.MaxHeight = 32;
    font_a16x32.Attr = CN_FONT_TYPE_DOT;
    font_a16x32.LoadFont = NULL;
    font_a16x32.UnloadFont = NULL;
    font_a16x32.GetBitmap = __Font_Ascii16x32GetCharBitMap;
    font_a16x32.GetCharWidth = __Font_Ascii16x32GetCharWidth;
    font_a16x32.GetCharHeight = __Font_Ascii16x32GetCharHeight;

    if(Font_InstallFont(&font_a16x32, CN_FONT_ASCII_16x32))
    {
        debug_printf("dot-ascii","ascii 16x32 font install sucess\n\r");
        return true;
    }
    else
    {
        Djy_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-ascii","ascii 16x32 font install fail\n\r");
        return false;
    }
}

#endif      //CFG_ASCII_16X32_SONG == 1

#if CFG_ASCII_16X32_YUAN == 1        //Բ��

#include "dot-ascii16x32yuan.h"

#endif      //CFG_ASCII_16X32_YUAN == 1

#if CFG_ASCII_16X32_KAI == 1         //����

#include "dot-ascii16x32kai.h"

#endif      //CFG_ASCII_16X32_KAI == 1

#if CFG_ASCII_16X32_HEI == 1         //����

#include "dot-ascii16x32hei.h"

#endif      //CFG_ASCII_16X32_HEI == 1

#if CFG_ASCII_16X32_FANG == 1         //����

#include "dot-ascii16x32fang.h"

#endif      //CFG_ASCII_16X32_FANG == 1


