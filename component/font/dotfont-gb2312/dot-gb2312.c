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

//����ģ��: GB2312-80 16*16������������
//���ߣ�mjh
//�汾��V1.0.0
//�ļ�����: GB2312-80�������
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2011-05-17
//       ����:lst
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "gkernel.h"
#include "charset.h"
#include "dbug.h"
#include "./font/font.h"
#include "./font/font_gb2312.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�
#define zk_disable      0
#define from_file       1
#define from_array      2
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern void ModuleInstall_FontGB2312(void);
//  ModuleInstall_FontGB2312();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"gb2312 dot"//gb2312��������,12~32����
//parent:"font"      //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"font","gb2312 charset"  //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"ascii dot font"//������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_GB2312_DOT == false )
//#warning  " gb2312_dot  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_GB2312_DOT    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_12_SONG              zk_disable              //"12��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"  //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_16_SONG              from_array              //"16��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_16_SONG_FILENAME "zk_gb2316_16song.bin"  //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_16_YAHEI             zk_disable              //"16����΢���ź�",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_16_YAHEI_FILENAME "zk_gb2316_16yahei.bin"  //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_16_YUAN              zk_disable              //"16����Բ��",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_16_YUAN_FILENAME "zk_gb2316_16yuan.bin"  //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_16_KAI              zk_disable               //"16������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_16_KAI_FILENAME  "zk_gb2316_16kai.bin"   //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_16_HEI              zk_disable               //"16�����",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_16_HEI_FILENAME  "zk_gb2316_16hei.bin"   //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_16_FANG              zk_disable              //"16���������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_16_FANG_FILENAME "zk_gb2316_16fang.bin"  //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_24_SONG              zk_disable              //"24��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_24_SONG_FILENAME "zk_gb2316_24song.bin"  //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_24_YAHEI              zk_disable             // "24����΢���ź�",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_24_YAHEI_FILENAME "zk_gb2316_24yahei.bin"// "�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_24_YUAN              zk_disable              //"24����Բ��",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_24_YUAN_FILENAME "zk_gb2316_24yuan.bin"  //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_24_KAI              zk_disable               //"24������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_24_KAI_FILENAME  "zk_gb2316_24kai.bin"   //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_24_HEI              zk_disable               //"24�������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_24_HEI_FILENAME  "zk_gb2316_24hei.bin"   //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_24_FANG              zk_disable              //"24���������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_24_FANG_FILENAME "zk_gb2316_24fang.bin"  //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_32_SONG              zk_disable              //"32��������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_32_SONG_FILENAME "zk_gb2316_32song.bin"  //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_32_YUAN              zk_disable              //"32����Բ��",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_32_YUAN_FILENAME "zk_gb2316_32yuan.bin"  //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_32_KAI              zk_disable               //"32������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_32_KAI_FILENAME  "zk_gb2316_32kai.bin"   //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_32_HEI              zk_disable               //"32�������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_32_HEI_FILENAME  "zk_gb2316_32hei.bin"   //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_32_FANG              zk_disable              //"32���������",GB2312����,zk_disable������Ҫ��from_array���������ȡ��from_file�����ļ���
//%$#@string,0,255,
    #define CFG_GB2312_32_FANG_FILENAME "zk_gb2316_32fang.bin"  //"�ֿ��ļ���",�����ļ���ȡ���������ļ���
//%$#@string,0,255,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


static struct Charset* s_ptGb2312Set;
s32 Gb2312Ucs4ToMb (u8* mb, u32 wc);
#define FONT_GB2312_DEF_CHAR      0xa1a1

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#if CFG_GB2312_12_SONG != zk_disable
#include "../dotfont-ascii/dot-ascii8x12song.h"

bool_t __Gb2312_12LoadFromFileSong(const char* FileName);
void __Gb2312_12UnLoadFromFileSong(void);
bool_t __Font_Gb2312_12GetCharBitMapSong(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231212x12Song;
u8 *pg_Ascii8x12Song;

#define FONT_HZ12_W             12              // Width
#define FONT_HZ12_H             12              // Height
#define FONT_HZ12_ASCII_W       6               // Width
#define FONT_HZ12_GLYPH_BYTES   24              // �ַ���ģ���ֽ���
#define FONT_HZ12_ASCII_BYTES   12              // ASCII���ֽ���

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ12�������庺�ֺ�ascii 8*12�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_12GetCharBitMapSong(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ16_ASCII_BYTES;
        bitmap->width = FONT_HZ12_ASCII_W;
        bitmap->height = FONT_HZ12_H;
        bitmap->linebytes = (FONT_HZ12_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ12_ASCII_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii8x12Song[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ12_GLYPH_BYTES;
        bitmap->width = FONT_HZ12_W;
        bitmap->height = FONT_HZ12_H;
        bitmap->linebytes = (FONT_HZ12_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ12_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231212x12Song[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_12GetCharWidthSong(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ12_ASCII_W;
            }
            else
            {
                a=FONT_HZ12_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_12GetCharHeightSong(u32 CharCode)
{
    return 12;
}

#if CFG_GB2312_12_SONG == from_array

#include "dot-gb2312_12song.rsc"

#else       //CFG_GB2312_12_SONG == from_array

bool_t __Gb2312_12LoadFromFileSong(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231212x12Song = M_Malloc(FONT_HZ12_GLYPH_BYTES*8813,0);
    if(pg_GB231212x12Song != NULL)
    {
        fread(pg_GB231212x12Song,FONT_HZ12_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_12UnLoadFromFileSong(void)
{
    free(pg_GB231212x12Song);
}

#endif      //CFG_GB2312_12_SONG != zk_disable

//----��װgb2312 12��������----------------------------------------------------
//����: ��װgb2312 12��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_12_Song(void)
{
    static struct FontObj font_gb2312_8x12_1212;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii8x12Song = (u8*)cs_ascii_6x12;
    font_gb2312_8x12_1212.MaxWidth = FONT_HZ12_W;
    font_gb2312_8x12_1212.MaxHeight = FONT_HZ12_H;
    font_gb2312_8x12_1212.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_12_FILENAME == from_array
    pg_GB231212x12Song = (u8*)cs_GB2312Song12x12;
    font_gb2312_8x12_1212.LoadFont = NULL;
    font_gb2312_8x12_1212.UnloadFont = NULL;
#else       //CFG_GB2312_12_SONG == from_array
    __Gb2312_12LoadFromFileSong(CFG_GB2312_12_SONG_FILENAME);  //�����ֿ�
    font_gb2312_8x12_1212.LoadFont = __Gb2312_12LoadFromFileSong;
    font_gb2312_8x12_1212.UnloadFont = __Gb2312_12UnLoadFromFileSong;
#endif      //CFG_GB2312_12_SONG == from_array
    font_gb2312_8x12_1212.GetBitmap = __Font_Gb2312_12GetCharBitMapSong;
    font_gb2312_8x12_1212.GetCharWidth = __Font_Gb2312_12GetCharWidthSong;
    font_gb2312_8x12_1212.GetCharHeight = __Font_Gb2312_12GetCharHeightSong;

    if(Font_InstallFont(&font_gb2312_8x12_1212, CN_FONT_GB2312_SONG_12))
    {
        debug_printf("dot-gb2312","gb2312 12�������尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 12�������尲װʧ��\n\r");
        return 0;
    }
}
#endif      //CFG_GB2312_12_SONG != zk_disable

#define FONT_HZ16_W             16              // Width
#define FONT_HZ16_H             16              // Height
#define FONT_HZ16_ASCII_W       8              // Width
#define FONT_HZ16_GLYPH_BYTES   32              // �ַ���ģ���ֽ���
#define FONT_HZ16_ASCII_BYTES   16              // ASCII���ֽ���

#if CFG_GB2312_16_SONG != zk_disable
#include "../dotfont-ascii/dot-ascii8x16song.h"

bool_t __Gb2312_16LoadFromFileSong(const char* FileName);
void __Gb2312_16UnLoadFromFileSong(void);
bool_t __Font_Gb2312_16GetCharBitMapSong(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231216x16Song;
u8 *pg_Ascii8x16Song;

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ16�������庺�ֺ�ascii 8*16�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_16GetCharBitMapSong(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ16_ASCII_BYTES;
        bitmap->width = FONT_HZ16_ASCII_W;
        bitmap->height = FONT_HZ16_H;
        bitmap->linebytes = (FONT_HZ16_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ16_ASCII_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii8x16Song[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ16_GLYPH_BYTES;
        bitmap->width = FONT_HZ16_W;
        bitmap->height = FONT_HZ16_H;
        bitmap->linebytes = (FONT_HZ16_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ16_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231216x16Song[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_16GetCharWidthSong(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ16_ASCII_W;
            }
            else
            {
                a=FONT_HZ16_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_16GetCharHeightSong(u32 CharCode)
{
    return 16;
}

#if CFG_GB2312_16_SONG == from_array

#include "dot-gb2312_16song.rsc"

#else       //CFG_GB2312_16_SONG == from_array

bool_t __Gb2312_16LoadFromFileSong(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231216x16Song = M_Malloc(FONT_HZ16_GLYPH_BYTES*8813,0);
    if(pg_GB231216x16Song != NULL)
    {
        fread(pg_GB231216x16Song,FONT_HZ16_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_16UnLoadFromFileSong(void)
{
    free(pg_GB231216x16Song);
}

#endif      //CFG_GB2312_16_SONG == from_array

//----��װgb2312 16��������----------------------------------------------------
//����: ��װgb2312 16��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_16_Song(void)
{
    static struct FontObj font_gb2312_8x16_1616;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii8x16Song = (u8*)cs_ascii_8x16;
    font_gb2312_8x16_1616.MaxWidth = FONT_HZ16_W;
    font_gb2312_8x16_1616.MaxHeight = FONT_HZ16_H;
    font_gb2312_8x16_1616.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_16_SONG == from_array
    pg_GB231216x16Song = (u8*)cs_GB2312_Song_16x16;
    font_gb2312_8x16_1616.LoadFont = NULL;
    font_gb2312_8x16_1616.UnloadFont = NULL;
#else       //CFG_GB2312_16_SONG == from_array
    __Gb2312_16LoadFromFileSong(CFG_GB2312_16_SONG_FILENAME);  //�����ֿ�
    font_gb2312_8x16_1616.LoadFont = __Gb2312_16LoadFromFileSong;
    font_gb2312_8x16_1616.UnloadFont = __Gb2312_16UnLoadFromFileSong;
#endif      //CFG_GB2312_16_SONG == from_array
    font_gb2312_8x16_1616.GetBitmap = __Font_Gb2312_16GetCharBitMapSong;
    font_gb2312_8x16_1616.GetCharWidth = __Font_Gb2312_16GetCharWidthSong;
    font_gb2312_8x16_1616.GetCharHeight = __Font_Gb2312_16GetCharHeightSong;

    if(Font_InstallFont(&font_gb2312_8x16_1616, CN_FONT_GB2312_SONG_16))
    {
        debug_printf("dot-gb2312","gb2312 16�������尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 16�������尲װʧ��\n\r");
        return 0;
    }
}

#endif      //CFG_GB2312_16_SONG != zk_disable

#if CFG_GB2312_16_YAHEI != zk_disable
#include "../dotfont-ascii/dot-ascii8x16song.h"

bool_t __Gb2312_16LoadFromFileYahei(const char* FileName);
void __Gb2312_16UnLoadFromFileYahei(void);
bool_t __Font_Gb2312_16GetCharBitMapYahei(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231216x16Yahei;
u8 *pg_Ascii8x16Song;

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ16�������庺�ֺ�ascii 8*16�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_16GetCharBitMapYahei(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ16_ASCII_BYTES;
        bitmap->width = FONT_HZ16_ASCII_W;
        bitmap->height = FONT_HZ16_H;
        bitmap->linebytes = (FONT_HZ16_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ16_ASCII_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii8x16Song[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ16_GLYPH_BYTES;
        bitmap->width = FONT_HZ16_W;
        bitmap->height = FONT_HZ16_H;
        bitmap->linebytes = (FONT_HZ16_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ16_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231216x16Yahei[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_16GetCharWidthYahei(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ16_ASCII_W;
            }
            else
            {
                a=FONT_HZ16_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_16GetCharHeightYahei(u32 CharCode)
{
    return 16;
}

#if CFG_GB2312_16_YAHEI == from_array

#include "dot-gb2312_16Yahei.rsc"

#else       //CFG_GB2312_16_YAHEI == from_array

bool_t __Gb2312_16LoadFromFileYahei(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231216x16Yahei = M_Malloc(FONT_HZ16_GLYPH_BYTES*8813,0);
    if(pg_GB231216x16Yahei != NULL)
    {
        fread(pg_GB231216x16Yahei,FONT_HZ16_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_16UnLoadFromFileYahei(void)
{
    free(pg_GB231216x16Yahei);
}

#endif      //CFG_GB2312_16_YAHEI == from_array

//----��װgb2312 16��������----------------------------------------------------
//����: ��װgb2312 16��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_16_Yahei(void)
{
    static struct FontObj font_gb2312_8x16_1616;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii8x16Song = (u8*)cs_ascii_8x16;
    font_gb2312_8x16_1616.MaxWidth = FONT_HZ16_W;
    font_gb2312_8x16_1616.MaxHeight = FONT_HZ16_H;
    font_gb2312_8x16_1616.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_16_YAHEI == from_array
    pg_GB231216x16Yahei = (u8*)cs_GB2312_Yahei_16x16;
    font_gb2312_8x16_1616.LoadFont = NULL;
    font_gb2312_8x16_1616.UnloadFont = NULL;
#else       //CFG_GB2312_16_YAHEI == from_array
    __Gb2312_16LoadFromFileYahei(CFG_GB2312_16_YAHEI_FILENAME);  //�����ֿ�
    font_gb2312_8x16_1616.LoadFont = __Gb2312_16LoadFromFileYahei;
    font_gb2312_8x16_1616.UnloadFont = __Gb2312_16UnLoadFromFileYahei;
#endif      //CFG_GB2312_16_YAHEI == from_array
    font_gb2312_8x16_1616.GetBitmap = __Font_Gb2312_16GetCharBitMapYahei;
    font_gb2312_8x16_1616.GetCharWidth = __Font_Gb2312_16GetCharWidthYahei;
    font_gb2312_8x16_1616.GetCharHeight = __Font_Gb2312_16GetCharHeightYahei;

    if(Font_InstallFont(&font_gb2312_8x16_1616, CN_FONT_GB2312_YAHEI_16))
    {
        debug_printf("dot-gb2312","gb2312 16�������尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 16�������尲װʧ��\n\r");
        return 0;
    }
}

#endif      //CFG_GB2312_16_YAHEI != zk_disable

#if CFG_GB2312_16_FANG != zk_disable
#include "../dotfont-ascii/dot-ascii8x16fang.h"

bool_t __Gb2312_16LoadFromFileFang(const char* FileName);
void __Gb2312_16UnLoadFromFileFang(void);
bool_t __Font_Gb2312_16GetCharBitMapFang(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231216x16Fang;
u8 *pg_Ascii8x16Fang;

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ16�������庺�ֺ�ascii 8*16�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_16GetCharBitMapFang(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ16_ASCII_BYTES;
        bitmap->width = FONT_HZ16_ASCII_W;
        bitmap->height = FONT_HZ16_H;
        bitmap->linebytes = (FONT_HZ16_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i < FONT_HZ16_ASCII_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii8x16Fang[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ16_GLYPH_BYTES;
        bitmap->width = FONT_HZ16_W;
        bitmap->height = FONT_HZ16_H;
        bitmap->linebytes = (FONT_HZ16_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ16_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231216x16Fang[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_16GetCharWidthFang(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ16_ASCII_W;
            }
            else
            {
                a=FONT_HZ16_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_16GetCharHeightFang(u32 CharCode)
{
    return 16;
}

#if CFG_GB2312_16_FANG == from_array

#include "dot-gb2312_16fang.rsc"

#else       //CFG_GB2312_16_FANG == from_array

bool_t __Gb2312_16LoadFromFileFang(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231216x16Fang = M_Malloc(FONT_HZ16_GLYPH_BYTES*8813,0);
    if(pg_GB231216x16Fang != NULL)
    {
        fread(pg_GB231216x16Fang,FONT_HZ16_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_16UnLoadFromFileFang(void)
{
    free(pg_GB231216x16Fang);
}

#endif      //CFG_GB2312_16_FANG == from_array

//----��װgb2312 16��������----------------------------------------------------
//����: ��װgb2312 16��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_16_Fang(void)
{
    static struct FontObj font_gb2312_8x16_1616;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii8x16Fang = (u8*)cs_ascii_8x16;
    font_gb2312_8x16_1616.MaxWidth = FONT_HZ16_W;
    font_gb2312_8x16_1616.MaxHeight = FONT_HZ16_H;
    font_gb2312_8x16_1616.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_16_FANG == from_array
    pg_GB231216x16Fang = (u8 *)cs_GB2312_Fang_16x16;
    font_gb2312_8x16_1616.LoadFont = NULL;
    font_gb2312_8x16_1616.UnloadFont = NULL;
#else       //CFG_GB2312_16_FANG == from_array
    __Gb2312_16LoadFromFileFang(CFG_GB2312_16_FANG_FILENAME);  //�����ֿ�
    font_gb2312_8x16_1616.LoadFont = __Gb2312_16LoadFromFileFang;
    font_gb2312_8x16_1616.UnloadFont = __Gb2312_16UnLoadFromFileFang;
#endif      //CFG_GB2312_16_FANG == from_array
    font_gb2312_8x16_1616.GetBitmap = __Font_Gb2312_16GetCharBitMapFang;
    font_gb2312_8x16_1616.GetCharWidth = __Font_Gb2312_16GetCharWidthFang;
    font_gb2312_8x16_1616.GetCharHeight = __Font_Gb2312_16GetCharHeightFang;

    if(Font_InstallFont(&font_gb2312_8x16_1616, CN_FONT_GB2312_FANG_16))
    {
        debug_printf("dot-gb2312","gb2312 16��������尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 16��������尲װʧ��\n\r");
        return 0;
    }
}
#endif      //CFG_GB2312_16_FANG != zk_disable

#if CFG_GB2312_16_HEI != zk_disable
#include "../dotfont-ascii/dot-ascii8x16hei.h"

bool_t __Gb2312_16LoadFromFileHei(const char* FileName);
void __Gb2312_16UnLoadFromFileHei(void);
bool_t __Font_Gb2312_16GetCharBitMapHei(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231216x16Hei;
u8 *pg_Ascii8x16Hei;

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ16�������庺�ֺ�ascii 8*16�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_16GetCharBitMapHei(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ16_ASCII_BYTES;
        bitmap->width = FONT_HZ16_ASCII_W;
        bitmap->height = FONT_HZ16_H;
        bitmap->linebytes = (FONT_HZ16_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i < FONT_HZ16_ASCII_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii8x16Hei[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ16_GLYPH_BYTES;
        bitmap->width = FONT_HZ16_W;
        bitmap->height = FONT_HZ16_H;
        bitmap->linebytes = (FONT_HZ16_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ16_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231216x16Hei[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_16GetCharWidthHei(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ16_ASCII_W;
            }
            else
            {
                a=FONT_HZ16_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_16GetCharHeightHei(u32 CharCode)
{
    return 16;
}

#if CFG_GB2312_16_HEI == from_array

#include "dot-gb2312_16hei.rsc"

#else       //CFG_GB2312_16_HEI == from_array

bool_t __Gb2312_16LoadFromFileHei(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231216x16Hei = M_Malloc(FONT_HZ16_GLYPH_BYTES*8813,0);
    if(pg_GB231216x16Hei != NULL)
    {
        fread(pg_GB231216x16Hei,FONT_HZ16_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_16UnLoadFromFileHei(void)
{
    free(pg_GB231216x16Hei);
}

#endif      //CFG_GB2312_16_HEI == from_array

//----��װgb2312 16��������----------------------------------------------------
//����: ��װgb2312 16��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_16_Hei(void)
{
    static struct FontObj font_gb2312_8x16_1616;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii8x16Hei = (u8*)cs_ascii_8x16;
    font_gb2312_8x16_1616.MaxWidth = FONT_HZ16_W;
    font_gb2312_8x16_1616.MaxHeight = FONT_HZ16_H;
    font_gb2312_8x16_1616.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_16_HEI == from_array
    pg_GB231216x16Hei = (u8*)cs_GB2312_Hei_16x16;
    font_gb2312_8x16_1616.LoadFont = NULL;
    font_gb2312_8x16_1616.UnloadFont = NULL;
#else       //CFG_GB2312_16_HEI == from_array
    __Gb2312_16LoadFromFileHei(CFG_GB2312_16_HEI_FILENAME);  //�����ֿ�
    font_gb2312_8x16_1616.LoadFont = __Gb2312_16LoadFromFileHei;
    font_gb2312_8x16_1616.UnloadFont = __Gb2312_16UnLoadFromFileHei;
#endif      //CFG_GB2312_16_HEI == from_array
    font_gb2312_8x16_1616.GetBitmap = __Font_Gb2312_16GetCharBitMapHei;
    font_gb2312_8x16_1616.GetCharWidth = __Font_Gb2312_16GetCharWidthHei;
    font_gb2312_8x16_1616.GetCharHeight = __Font_Gb2312_16GetCharHeightHei;

    if(Font_InstallFont(&font_gb2312_8x16_1616, CN_FONT_GB2312_HEI_16))
    {
        debug_printf("dot-gb2312","gb2312 16������尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 16������尲װʧ��\n\r");
        return 0;
    }
}

#endif      //CFG_GB2312_16_HEI != zk_disable

#if CFG_GB2312_16_KAI != zk_disable
#include "../dotfont-ascii/dot-ascii8x16song.h"

bool_t __Gb2312_16LoadFromFileKai(const char* FileName);
void __Gb2312_16UnLoadFromFileKai(void);
bool_t __Font_Gb2312_16GetCharBitMapKai(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231216x16Kai;
u8 *pg_Ascii8x16Kai;

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ16�������庺�ֺ�ascii 8*16�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_16GetCharBitMapKai(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ16_ASCII_BYTES;
        bitmap->width = FONT_HZ16_ASCII_W;
        bitmap->height = FONT_HZ16_H;
        bitmap->linebytes = (FONT_HZ16_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i < FONT_HZ16_ASCII_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii8x16Kai[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ16_GLYPH_BYTES;
        bitmap->width = FONT_HZ16_W;
        bitmap->height = FONT_HZ16_H;
        bitmap->linebytes = (FONT_HZ16_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ16_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231216x16Kai[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_16GetCharWidthKai(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ16_ASCII_W;
            }
            else
            {
                a=FONT_HZ16_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_16GetCharHeightKai(u32 CharCode)
{
    return 16;
}

#if CFG_GB2312_16_KAI == from_array

#include "dot-gb2312_16song.rsc"

#else       //CFG_GB2312_16_KAI == from_array

bool_t __Gb2312_16LoadFromFileKai(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231216x16Kai = M_Malloc(FONT_HZ16_GLYPH_BYTES*8813,0);
    if(pg_GB231216x16Kai != NULL)
    {
        fread(pg_GB231216x16Kai,FONT_HZ16_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_16UnLoadFromFileKai(void)
{
    free(pg_GB231216x16Kai);
}

#endif      //CFG_GB2312_16_KAI == from_array

//----��װgb2312 16��������----------------------------------------------------
//����: ��װgb2312 16��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_16_Kai(void)
{
    static struct FontObj font_gb2312_8x16_1616;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii8x16Kai = (u8*)cs_ascii_8x16;
    font_gb2312_8x16_1616.MaxWidth = FONT_HZ16_W;
    font_gb2312_8x16_1616.MaxHeight = FONT_HZ16_H;
    font_gb2312_8x16_1616.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_16_KAI == from_array
    pg_GB231216x16Kai = (u8*)cs_GB2312_Kai_16x16;
    font_gb2312_8x16_1616.LoadFont = NULL;
    font_gb2312_8x16_1616.UnloadFont = NULL;
#else       //CFG_GB2312_16_KAI == from_array
    __Gb2312_16LoadFromFileKai(CFG_GB2312_16_KAI_FILENAME);  //�����ֿ�
    font_gb2312_8x16_1616.LoadFont = __Gb2312_16LoadFromFileKai;
    font_gb2312_8x16_1616.UnloadFont = __Gb2312_16UnLoadFromFileKai;
#endif      //CFG_GB2312_16_KAI == from_array
    font_gb2312_8x16_1616.GetBitmap = __Font_Gb2312_16GetCharBitMapKai;
    font_gb2312_8x16_1616.GetCharWidth = __Font_Gb2312_16GetCharWidthKai;
    font_gb2312_8x16_1616.GetCharHeight = __Font_Gb2312_16GetCharHeightKai;

    if(Font_InstallFont(&font_gb2312_8x16_1616, CN_FONT_GB2312_KAI_16))
    {
        debug_printf("dot-gb2312","gb2312 16�����尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 16�����尲װʧ��\n\r");
        return 0;
    }
}

#endif      //CFG_GB2312_16_KAI != zk_disable

#if CFG_GB2312_16_YUAN != zk_disable
#include "../dotfont-ascii/dot-ascii8x16yuan.h"

bool_t __Gb2312_16LoadFromFileYuan(const char* FileName);
void __Gb2312_16UnLoadFromFileYuan(void);
bool_t __Font_Gb2312_16GetCharBitMapYuan(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231216x16Yuan;
u8 *pg_Ascii8x16Yuan;

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ16�������庺�ֺ�ascii 8*16�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_16GetCharBitMapYuan(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ16_ASCII_BYTES;
        bitmap->width = FONT_HZ16_ASCII_W;
        bitmap->height = FONT_HZ16_H;
        bitmap->linebytes = (FONT_HZ16_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i < FONT_HZ16_ASCII_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii8x16Yuan[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ16_GLYPH_BYTES;
        bitmap->width = FONT_HZ16_W;
        bitmap->height = FONT_HZ16_H;
        bitmap->linebytes = (FONT_HZ16_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ16_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231216x16Yuan[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_16GetCharWidthYuan(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ16_ASCII_W;
            }
            else
            {
                a=FONT_HZ16_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_16GetCharHeightYuan(u32 CharCode)
{
    return 16;
}

#if CFG_GB2312_16_YUAN == from_array

#include "dot-gb2312_16yuan.rsc"

#else       //CFG_GB2312_16_YUAN == from_array

bool_t __Gb2312_16LoadFromFileYuan(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231216x16Yuan = M_Malloc(FONT_HZ16_GLYPH_BYTES*8813,0);
    if(pg_GB231216x16Yuan != NULL)
    {
        fread(pg_GB231216x16Yuan,FONT_HZ16_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_16UnLoadFromFileYuan(void)
{
    free(pg_GB231216x16Yuan);
}

#endif      //CFG_GB2312_16_YUAN == from_array

//----��װgb2312 16��������----------------------------------------------------
//����: ��װgb2312 16��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_16_Yuan(void)
{
    static struct FontObj font_gb2312_8x16_1616;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii8x16Yuan = (u8*)cs_ascii_yuan_8x16;
    font_gb2312_8x16_1616.MaxWidth = FONT_HZ16_W;
    font_gb2312_8x16_1616.MaxHeight = FONT_HZ16_H;
    font_gb2312_8x16_1616.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_16_YUAN == from_array
    pg_GB231216x16Yuan = (u8*)cs_GB2312_Yuan_16x16;
    font_gb2312_8x16_1616.LoadFont = NULL;
    font_gb2312_8x16_1616.UnloadFont = NULL;
#else       //CFG_GB2312_16_YUAN == from_array
    __Gb2312_16LoadFromFileYuan(CFG_GB2312_16_YUAN_FILENAME);  //�����ֿ�
    font_gb2312_8x16_1616.LoadFont = __Gb2312_16LoadFromFileYuan;
    font_gb2312_8x16_1616.UnloadFont = __Gb2312_16UnLoadFromFileYuan;
#endif      //CFG_GB2312_16_YUAN == from_array
    font_gb2312_8x16_1616.GetBitmap = __Font_Gb2312_16GetCharBitMapYuan;
    font_gb2312_8x16_1616.GetCharWidth = __Font_Gb2312_16GetCharWidthYuan;
    font_gb2312_8x16_1616.GetCharHeight = __Font_Gb2312_16GetCharHeightYuan;

    if(Font_InstallFont(&font_gb2312_8x16_1616, CN_FONT_GB2312_YUAN_16))
    {
        debug_printf("dot-gb2312","gb2312 16����Բ�尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 16����Բ�尲װʧ��\n\r");
        return 0;
    }
}

#endif      //CFG_GB2312_16_YUAN != zk_disable

#define FONT_HZ24_W             24              // Width
#define FONT_HZ24_H             24              // Height
#define FONT_HZ24_ASCII_W       12              // Width
#define FONT_HZ24_GLYPH_BYTES   72              // �ַ���ģ���ֽ���
#define FONT_HZ24_ASCII_BYTES   48              // ASCII���ֽ���

#if CFG_GB2312_24_SONG != zk_disable
#include "../dotfont-ascii/dot-ascii12x24song.h"

bool_t __Gb2312_24LoadFromFileSong(const char* FileName);
void __Gb2312_24UnLoadFromFileSong(void);
bool_t __Font_Gb2312_24GetCharBitMapSong(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231224x24Song;
u8 *pg_Ascii12x24Song;

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ24�������庺�ֺ�ascii 8*24�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_24GetCharBitMapSong(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ24_ASCII_BYTES;
        bitmap->width = FONT_HZ24_ASCII_W;
        bitmap->height = FONT_HZ24_H;
        bitmap->linebytes = (FONT_HZ24_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<24; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii12x24Song[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ24_GLYPH_BYTES;
        bitmap->width = FONT_HZ24_W;
        bitmap->height = FONT_HZ24_H;
        bitmap->linebytes = (FONT_HZ24_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ24_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231224x24Song[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_24GetCharWidthSong(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ24_ASCII_W;
            }
            else
            {
                a=FONT_HZ24_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_24GetCharHeightSong(u32 CharCode)
{
    return 24;
}

#if CFG_GB2312_24_SONG == from_array

#include "dot-gb2312_24song.rsc"

#else       //CFG_GB2312_24_SONG == from_array

bool_t __Gb2312_24LoadFromFileSong(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231224x24Song = M_Malloc(FONT_HZ24_GLYPH_BYTES*8813,0);
    if(pg_GB231224x24Song != NULL)
    {
        fread(pg_GB231224x24Song,FONT_HZ24_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_24UnLoadFromFileSong(void)
{
    free(pg_GB231224x24Song);
}

#endif      //CFG_GB2312_24_SONG == from_array

//----��װgb2312 24��������----------------------------------------------------
//����: ��װgb2312 24��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_24_Song(void)
{
    static struct FontObj font_gb2312_8x24_2424;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii12x24Song = (u8*)cs_ascii_12x24;
    font_gb2312_8x24_2424.MaxWidth = FONT_HZ24_W;
    font_gb2312_8x24_2424.MaxHeight = FONT_HZ24_H;
    font_gb2312_8x24_2424.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_24_SONG == from_array
    pg_GB231224x24Song = (u8*)cs_GB2312_Song_24x24;
    font_gb2312_8x24_2424.LoadFont = NULL;
    font_gb2312_8x24_2424.UnloadFont = NULL;
#else       //CFG_GB2312_24_SONG == from_array
    __Gb2312_24LoadFromFileSong(CFG_GB2312_24_SONG_FILENAME);  //�����ֿ�
    font_gb2312_8x24_2424.LoadFont = __Gb2312_24LoadFromFileSong;
    font_gb2312_8x24_2424.UnloadFont = __Gb2312_24UnLoadFromFileSong;
#endif      //CFG_GB2312_24_SONG == from_array
    font_gb2312_8x24_2424.GetBitmap = __Font_Gb2312_24GetCharBitMapSong;
    font_gb2312_8x24_2424.GetCharWidth = __Font_Gb2312_24GetCharWidthSong;
    font_gb2312_8x24_2424.GetCharHeight = __Font_Gb2312_24GetCharHeightSong;

    if(Font_InstallFont(&font_gb2312_8x24_2424, CN_FONT_GB2312_SONG_24))
    {
        debug_printf("dot-gb2312","gb2312 24�������尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 24�������尲װʧ��\n\r");
        return 0;
    }
}

#endif      //CFG_GB2312_24_SONG != zk_disable

#if CFG_GB2312_24_YAHEI != zk_disable
#include "../dotfont-ascii/dot-ascii12x24song.h"

bool_t __Gb2312_24LoadFromFileYahei(const char* FileName);
void __Gb2312_24UnLoadFromFileYahei(void);
bool_t __Font_Gb2312_24GetCharBitMapYahei(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231224x24Yahei;
u8 *pg_Ascii12x24Yahei;

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ24����΢���źں��ֺ�ascii 12*24�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_24GetCharBitMapYahei(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ24_ASCII_BYTES;
        bitmap->width = FONT_HZ24_ASCII_W;
        bitmap->height = FONT_HZ24_H;
        bitmap->linebytes = (FONT_HZ24_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ24_ASCII_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii12x24Yahei[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ24_GLYPH_BYTES;
        bitmap->width = FONT_HZ24_W;
        bitmap->height = FONT_HZ24_H;
        bitmap->linebytes = (FONT_HZ24_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ24_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231224x24Yahei[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_24GetCharWidthYahei(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ24_ASCII_W;
            }
            else
            {
                a=FONT_HZ24_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_24GetCharHeightYahei(u32 CharCode)
{
    return 24;
}

#if CFG_GB2312_24_YAHEI == from_array

#include "dot-gb2312_24yahei.rsc"

#else       //CFG_GB2312_24_Yahei == from_array

bool_t __Gb2312_24LoadFromFileYahei(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231224x24Yahei = M_Malloc(FONT_HZ24_GLYPH_BYTES*8813,0);
    if(pg_GB231224x24Yahei != NULL)
    {
        fread(pg_GB231224x24Yahei,FONT_HZ24_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_24UnLoadFromFileYahei(void)
{
    free(pg_GB231224x24Yahei);
}

#endif      //CFG_GB2312_24_Yahei == from_array

//----��װgb2312 24��������----------------------------------------------------
//����: ��װgb2312 24��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_24_Yahei(void)
{
    static struct FontObj font_gb2312_8x24_2424;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii12x24Yahei = (u8*)cs_ascii_12x24;
    font_gb2312_8x24_2424.MaxWidth = FONT_HZ24_W;
    font_gb2312_8x24_2424.MaxHeight = FONT_HZ24_H;
    font_gb2312_8x24_2424.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_24_YAHEI == from_array
    pg_GB231224x24Yahei = (u8*)cs_GB2312_Yahei_24x24;
    font_gb2312_8x24_2424.LoadFont = NULL;
    font_gb2312_8x24_2424.UnloadFont = NULL;
#else       //CFG_GB2312_24_Yahei == from_array
    __Gb2312_24LoadFromFileYahei(CFG_GB2312_24_YAHEI_FILENAME);  //�����ֿ�
    font_gb2312_8x24_2424.LoadFont = __Gb2312_24LoadFromFileYahei;
    font_gb2312_8x24_2424.UnloadFont = __Gb2312_24UnLoadFromFileYahei;
#endif      //CFG_GB2312_24_Yahei == from_array
    font_gb2312_8x24_2424.GetBitmap = __Font_Gb2312_24GetCharBitMapYahei;
    font_gb2312_8x24_2424.GetCharWidth = __Font_Gb2312_24GetCharWidthYahei;
    font_gb2312_8x24_2424.GetCharHeight = __Font_Gb2312_24GetCharHeightYahei;

    if(Font_InstallFont(&font_gb2312_8x24_2424, CN_FONT_GB2312_YAHEI_24))
    {
        debug_printf("dot-gb2312","gb2312 24�������尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 24�������尲װʧ��\n\r");
        return 0;
    }
}

#endif      //CFG_GB2312_24_SONG != zk_disable

#if CFG_GB2312_24_FANG != zk_disable
#include "../dotfont-ascii/dot-ascii8x24fang.h"

bool_t __Gb2312_24LoadFromFileFang(const char* FileName);
void __Gb2312_24UnLoadFromFileFang(void);
bool_t __Font_Gb2312_24GetCharBitMapFang(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231224x24Fang;
u8 *pg_Ascii12x24Fang;


//----������ȡ-----------------------------------------------------------------
//����: ��ȡ24�������庺�ֺ�ascii 8*24�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_24GetCharBitMapFang(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ24_ASCII_BYTES;
        bitmap->width = FONT_HZ24_ASCII_W;
        bitmap->height = FONT_HZ24_H;
        bitmap->linebytes = (FONT_HZ24_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<24; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii12x24Fang[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ24_GLYPH_BYTES;
        bitmap->width = FONT_HZ24_W;
        bitmap->height = FONT_HZ24_H;
        bitmap->linebytes = (FONT_HZ24_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ24_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231224x24Fang[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_24GetCharWidthFang(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ24_ASCII_W;
            }
            else
            {
                a=FONT_HZ24_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_24GetCharHeightFang(u32 CharCode)
{
    return 24;
}

#if CFG_GB2312_24_FANG == from_array

#include "dot-gb2312_24fang.rsc"

#else       //CFG_GB2312_24_FANG == from_array

bool_t __Gb2312_24LoadFromFileFang(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231224x24Fang = M_Malloc(FONT_HZ24_GLYPH_BYTES*8813,0);
    if(pg_GB231224x24Fang != NULL)
    {
        fread(pg_GB231224x24Fang,FONT_HZ24_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_24UnLoadFromFileFang(void)
{
    free(pg_GB231224x24Fang);
}

#endif      //CFG_GB2312_24_FANG == from_array

//----��װgb2312 24��������----------------------------------------------------
//����: ��װgb2312 24��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_24_Fang(void)
{
    static struct FontObj font_gb2312_8x24_2424;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii12x24Fang = (u8*)cs_ascii_12x24;
    font_gb2312_8x24_2424.MaxWidth = FONT_HZ24_W;
    font_gb2312_8x24_2424.MaxHeight = FONT_HZ24_H;
    font_gb2312_8x24_2424.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_24_FANG == from_array
    pg_GB231224x24Fang = (u8 *)cs_GB2312_Fang_24x24;
    font_gb2312_8x24_2424.LoadFont = NULL;
    font_gb2312_8x24_2424.UnloadFont = NULL;
#else       //CFG_GB2312_24_FANG == from_array
    __Gb2312_24LoadFromFileFang(CFG_GB2312_24_FANG_FILENAME);  //�����ֿ�
    font_gb2312_8x24_2424.LoadFont = __Gb2312_24LoadFromFileFang;
    font_gb2312_8x24_2424.UnloadFont = __Gb2312_24UnLoadFromFileFang;
#endif      //CFG_GB2312_24_FANG == from_array
    font_gb2312_8x24_2424.GetBitmap = __Font_Gb2312_24GetCharBitMapFang;
    font_gb2312_8x24_2424.GetCharWidth = __Font_Gb2312_24GetCharWidthFang;
    font_gb2312_8x24_2424.GetCharHeight = __Font_Gb2312_24GetCharHeightFang;

    if(Font_InstallFont(&font_gb2312_8x24_2424, CN_FONT_GB2312_FANG_24))
    {
        debug_printf("dot-gb2312","gb2312 24��������尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 24��������尲װʧ��\n\r");
        return 0;
    }
}
#endif      //CFG_GB2312_24_FANG != zk_disable

#if CFG_GB2312_24_HEI != zk_disable
#include "../dotfont-ascii/dot-ascii12x24hei.h"

bool_t __Gb2312_24LoadFromFileHei(const char* FileName);
void __Gb2312_24UnLoadFromFileHei(void);
bool_t __Font_Gb2312_24GetCharBitMapHei(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231224x24Hei;
u8 *pg_Ascii12x24Hei;

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ24�������庺�ֺ�ascii 8*24�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_24GetCharBitMapHei(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ24_ASCII_BYTES;
        bitmap->width = FONT_HZ24_ASCII_W;
        bitmap->height = FONT_HZ24_H;
        bitmap->linebytes = (FONT_HZ24_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ24_ASCII_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii12x24Hei[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ24_GLYPH_BYTES;
        bitmap->width = FONT_HZ24_W;
        bitmap->height = FONT_HZ24_H;
        bitmap->linebytes = (FONT_HZ24_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ24_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231224x24Hei[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_24GetCharWidthHei(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ24_ASCII_W;
            }
            else
            {
                a=FONT_HZ24_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_24GetCharHeightHei(u32 CharCode)
{
    return 24;
}

#if CFG_GB2312_24_HEI == from_array

#include "dot-gb2312_24hei.rsc"

#else       //CFG_GB2312_24_HEI == from_array

bool_t __Gb2312_24LoadFromFileHei(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231224x24Hei = M_Malloc(FONT_HZ24_GLYPH_BYTES*8813,0);
    if(pg_GB231224x24Hei != NULL)
    {
        fread(pg_GB231224x24Hei,FONT_HZ24_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_24UnLoadFromFileHei(void)
{
    free(pg_GB231224x24Hei);
}

#endif      //CFG_GB2312_24_HEI == from_array

//----��װgb2312 24��������----------------------------------------------------
//����: ��װgb2312 24��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_24_Hei(void)
{
    static struct FontObj font_gb2312_8x24_2424;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii12x24Hei = (u8*)cs_ascii_hei_12x24;
    font_gb2312_8x24_2424.MaxWidth = FONT_HZ24_W;
    font_gb2312_8x24_2424.MaxHeight = FONT_HZ24_H;
    font_gb2312_8x24_2424.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_24_HEI == from_array
    pg_GB231224x24Hei = (u8*)cs_GB2312_Hei_24x24;
    font_gb2312_8x24_2424.LoadFont = NULL;
    font_gb2312_8x24_2424.UnloadFont = NULL;
#else       //CFG_GB2312_24_HEI == from_array
    __Gb2312_24LoadFromFileHei(CFG_GB2312_24_HEI_FILENAME);  //�����ֿ�
    font_gb2312_8x24_2424.LoadFont = __Gb2312_24LoadFromFileHei;
    font_gb2312_8x24_2424.UnloadFont = __Gb2312_24UnLoadFromFileHei;
#endif      //CFG_GB2312_24_HEI == from_array
    font_gb2312_8x24_2424.GetBitmap = __Font_Gb2312_24GetCharBitMapHei;
    font_gb2312_8x24_2424.GetCharWidth = __Font_Gb2312_24GetCharWidthHei;
    font_gb2312_8x24_2424.GetCharHeight = __Font_Gb2312_24GetCharHeightHei;

    if(Font_InstallFont(&font_gb2312_8x24_2424, CN_FONT_GB2312_HEI_24))
    {
        debug_printf("dot-gb2312","gb2312 24������尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 24������尲װʧ��\n\r");
        return 0;
    }
}

#endif      //CFG_GB2312_24_HEI != zk_disable

#if CFG_GB2312_24_KAI != zk_disable
#include "../dotfont-ascii/dot-ascii8x24song.h"

bool_t __Gb2312_24LoadFromFileKai(const char* FileName);
void __Gb2312_24UnLoadFromFileKai(void);
bool_t __Font_Gb2312_24GetCharBitMapKai(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231224x24Kai;
u8 *pg_Ascii12x24Kai;

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ24�������庺�ֺ�ascii 8*24�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_24GetCharBitMapKai(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ24_ASCII_BYTES;
        bitmap->width = FONT_HZ24_ASCII_W;
        bitmap->height = FONT_HZ24_H;
        bitmap->linebytes = (FONT_HZ24_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<24; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii12x24Kai[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ24_GLYPH_BYTES;
        bitmap->width = FONT_HZ24_W;
        bitmap->height = FONT_HZ24_H;
        bitmap->linebytes = (FONT_HZ24_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ24_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231224x24Kai[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_24GetCharWidthKai(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ24_ASCII_W;
            }
            else
            {
                a=FONT_HZ24_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_24GetCharHeightKai(u32 CharCode)
{
    return 24;
}

#if CFG_GB2312_24_KAI == from_array

#include "dot-gb2312_24song.rsc"

#else       //CFG_GB2312_24_KAI == from_array

bool_t __Gb2312_24LoadFromFileKai(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231224x24Kai = M_Malloc(FONT_HZ24_GLYPH_BYTES*8813,0);
    if(pg_GB231224x24Kai != NULL)
    {
        fread(pg_GB231224x24Kai,FONT_HZ24_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_24UnLoadFromFileKai(void)
{
    free(pg_GB231224x24Kai);
}

#endif      //CFG_GB2312_24_KAI == from_array

//----��װgb2312 24��������----------------------------------------------------
//����: ��װgb2312 24��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_24_Kai(void)
{
    static struct FontObj font_gb2312_8x24_2424;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii12x24Kai = (u8*)cs_ascii_12x24;
    font_gb2312_8x24_2424.MaxWidth = FONT_HZ24_W;
    font_gb2312_8x24_2424.MaxHeight = FONT_HZ24_H;
    font_gb2312_8x24_2424.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_24_KAI == from_array
    pg_GB231224x24Kai = (u8*)cs_GB2312_Kai_24x24;
    font_gb2312_8x24_2424.LoadFont = NULL;
    font_gb2312_8x24_2424.UnloadFont = NULL;
#else       //CFG_GB2312_24_KAI == from_array
    __Gb2312_24LoadFromFileKai(CFG_GB2312_24_KAI_FILENAME);  //�����ֿ�
    font_gb2312_8x24_2424.LoadFont = __Gb2312_24LoadFromFileKai;
    font_gb2312_8x24_2424.UnloadFont = __Gb2312_24UnLoadFromFileKai;
#endif      //CFG_GB2312_24_KAI == from_array
    font_gb2312_8x24_2424.GetBitmap = __Font_Gb2312_24GetCharBitMapKai;
    font_gb2312_8x24_2424.GetCharWidth = __Font_Gb2312_24GetCharWidthKai;
    font_gb2312_8x24_2424.GetCharHeight = __Font_Gb2312_24GetCharHeightKai;

    if(Font_InstallFont(&font_gb2312_8x24_2424, CN_FONT_GB2312_KAI_24))
    {
        debug_printf("dot-gb2312","gb2312 24�����尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 24�����尲װʧ��\n\r");
        return 0;
    }
}

#endif      //CFG_GB2312_24_KAI != zk_disable

#if CFG_GB2312_24_YUAN != zk_disable
#include "../dotfont-ascii/dot-ascii8x24yuan.h"

bool_t __Gb2312_24LoadFromFileYuan(const char* FileName);
void __Gb2312_24UnLoadFromFileYuan(void);
bool_t __Font_Gb2312_24GetCharBitMapYuan(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231224x24Yuan;
u8 *pg_Ascii12x24Yuan;

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ24�������庺�ֺ�ascii 8*24�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_24GetCharBitMapYuan(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ24_ASCII_BYTES;
        bitmap->width = FONT_HZ24_ASCII_W;
        bitmap->height = FONT_HZ24_H;
        bitmap->linebytes = (FONT_HZ24_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<24; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii12x24Yuan[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ24_GLYPH_BYTES;
        bitmap->width = FONT_HZ24_W;
        bitmap->height = FONT_HZ24_H;
        bitmap->linebytes = (FONT_HZ24_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ24_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231224x24Yuan[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_24GetCharWidthYuan(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ24_ASCII_W;
            }
            else
            {
                a=FONT_HZ24_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_24GetCharHeightYuan(u32 CharCode)
{
    return 24;
}

#if CFG_GB2312_24_YUAN == from_array

#include "dot-gb2312_24yuan.rsc"

#else       //CFG_GB2312_24_YUAN == from_array

bool_t __Gb2312_24LoadFromFileYuan(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231224x24Yuan = M_Malloc(FONT_HZ24_GLYPH_BYTES*8813,0);
    if(pg_GB231224x24Yuan != NULL)
    {
        fread(pg_GB231224x24Yuan,FONT_HZ24_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_24UnLoadFromFileYuan(void)
{
    free(pg_GB231224x24Yuan);
}

#endif      //CFG_GB2312_24_YUAN == from_array

//----��װgb2312 24��������----------------------------------------------------
//����: ��װgb2312 24��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_24_Yuan(void)
{
    static struct FontObj font_gb2312_8x24_2424;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii12x24Yuan = (u8*)cs_ascii_12x24;
    font_gb2312_8x24_2424.MaxWidth = FONT_HZ24_W;
    font_gb2312_8x24_2424.MaxHeight = FONT_HZ24_H;
    font_gb2312_8x24_2424.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_24_YUAN == from_array
    pg_GB231224x24Yuan = (u8*)cs_GB2312_Yuan_24x24;
    font_gb2312_8x24_2424.LoadFont = NULL;
    font_gb2312_8x24_2424.UnloadFont = NULL;
#else       //CFG_GB2312_24_YUAN == from_array
    __Gb2312_24LoadFromFileYuan(CFG_GB2312_24_YUAN_FILENAME);  //�����ֿ�
    font_gb2312_8x24_2424.LoadFont = __Gb2312_24LoadFromFileYuan;
    font_gb2312_8x24_2424.UnloadFont = __Gb2312_24UnLoadFromFileYuan;
#endif      //CFG_GB2312_24_YUAN == from_array
    font_gb2312_8x24_2424.GetBitmap = __Font_Gb2312_24GetCharBitMapYuan;
    font_gb2312_8x24_2424.GetCharWidth = __Font_Gb2312_24GetCharWidthYuan;
    font_gb2312_8x24_2424.GetCharHeight = __Font_Gb2312_24GetCharHeightYuan;

    if(Font_InstallFont(&font_gb2312_8x24_2424, CN_FONT_GB2312_YUAN_24))
    {
        debug_printf("dot-gb2312","gb2312 24����Բ�尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 24����Բ�尲װʧ��\n\r");
        return 0;
    }
}

#endif      //CFG_GB2312_24_YUAN != zk_disable

#define FONT_HZ32_W             32              // Width
#define FONT_HZ32_H             32              // Height
#define FONT_HZ32_ASCII_W       16              // Width
#define FONT_HZ32_GLYPH_BYTES   128             // �ַ���ģ���ֽ���
#define FONT_HZ32_ASCII_BYTES   64              // ASCII���ֽ���

#if CFG_GB2312_32_SONG != zk_disable
#include "../dotfont-ascii/dot-ascii16x32song.h"

bool_t __Gb2312_32LoadFromFileSong(const char* FileName);
void __Gb2312_32UnLoadFromFileSong(void);
bool_t __Font_Gb2312_32GetCharBitMapSong(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231232x32Song;
u8 *pg_Ascii16x32Song;

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ32�������庺�ֺ�ascii 8*32�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_32GetCharBitMapSong(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ32_ASCII_BYTES;
        bitmap->width = FONT_HZ32_ASCII_W;
        bitmap->height = FONT_HZ32_H;
        bitmap->linebytes = (FONT_HZ32_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ32_ASCII_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii16x32Song[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ32_GLYPH_BYTES;
        bitmap->width = FONT_HZ32_W;
        bitmap->height = FONT_HZ32_H;
        bitmap->linebytes = (FONT_HZ32_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ32_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231232x32Song[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_32GetCharWidthSong(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ32_ASCII_W;
            }
            else
            {
                a=FONT_HZ32_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_32GetCharHeightSong(u32 CharCode)
{
    return 32;
}

#if CFG_GB2312_32_SONG == from_array

#include "dot-gb2312_32song.rsc"

#else       //CFG_GB2312_32_SONG == from_array

bool_t __Gb2312_32LoadFromFileSong(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231232x32Song = M_Malloc(FONT_HZ32_GLYPH_BYTES*8813,0);
    if(pg_GB231232x32Song != NULL)
    {
        fread(pg_GB231232x32Song,FONT_HZ32_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_32UnLoadFromFileSong(void)
{
    free(pg_GB231232x32Song);
}

#endif      //CFG_GB2312_32_SONG == from_array

//----��װgb2312 32��������----------------------------------------------------
//����: ��װgb2312 32��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_32_Song(void)
{
    static struct FontObj font_gb2312_8x32_3232;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii16x32Song = (u8*)cs_ascii_16x32;
    font_gb2312_8x32_3232.MaxWidth = FONT_HZ32_W;
    font_gb2312_8x32_3232.MaxHeight = FONT_HZ32_H;
    font_gb2312_8x32_3232.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_32_SONG == from_array
    pg_GB231232x32Song = (u8*)cs_GB2312_Song_32x32;
    font_gb2312_8x32_3232.LoadFont = NULL;
    font_gb2312_8x32_3232.UnloadFont = NULL;
#else       //CFG_GB2312_32_SONG == from_array
    __Gb2312_32LoadFromFileSong(CFG_GB2312_32_SONG_FILENAME);  //�����ֿ�
    font_gb2312_8x32_3232.LoadFont = __Gb2312_32LoadFromFileSong;
    font_gb2312_8x32_3232.UnloadFont = __Gb2312_32UnLoadFromFileSong;
#endif      //CFG_GB2312_32_SONG == from_array
    font_gb2312_8x32_3232.GetBitmap = __Font_Gb2312_32GetCharBitMapSong;
    font_gb2312_8x32_3232.GetCharWidth = __Font_Gb2312_32GetCharWidthSong;
    font_gb2312_8x32_3232.GetCharHeight = __Font_Gb2312_32GetCharHeightSong;

    if(Font_InstallFont(&font_gb2312_8x32_3232, CN_FONT_GB2312_SONG_32))
    {
        debug_printf("dot-gb2312","gb2312 32�������尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 32�������尲װʧ��\n\r");
        return 0;
    }
}

#endif      //CFG_GB2312_32_SONG != zk_disable

#if CFG_GB2312_32_FANG != zk_disable
#include "../dotfont-ascii/dot-ascii8x32fang.h"

bool_t __Gb2312_32LoadFromFileFang(const char* FileName);
void __Gb2312_32UnLoadFromFileFang(void);
bool_t __Font_Gb2312_32GetCharBitMapFang(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231232x32Fang;
u8 *pg_Ascii16x32Fang;


//----������ȡ-----------------------------------------------------------------
//����: ��ȡ32�������庺�ֺ�ascii 8*32�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_32GetCharBitMapFang(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ32_ASCII_BYTES;
        bitmap->width = FONT_HZ32_ASCII_W;
        bitmap->height = FONT_HZ32_H;
        bitmap->linebytes = (FONT_HZ32_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<32; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii16x32Fang[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ32_GLYPH_BYTES;
        bitmap->width = FONT_HZ32_W;
        bitmap->height = FONT_HZ32_H;
        bitmap->linebytes = (FONT_HZ32_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ32_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231232x32Fang[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_32GetCharWidthFang(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ32_ASCII_W;
            }
            else
            {
                a=FONT_HZ32_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_32GetCharHeightFang(u32 CharCode)
{
    return 32;
}

#if CFG_GB2312_32_FANG == from_array

#include "dot-gb2312_32fang.rsc"

#else       //CFG_GB2312_32_FANG == from_array

bool_t __Gb2312_32LoadFromFileFang(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231232x32Fang = M_Malloc(FONT_HZ32_GLYPH_BYTES*8813,0);
    if(pg_GB231232x32Fang != NULL)
    {
        fread(pg_GB231232x32Fang,FONT_HZ32_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_32UnLoadFromFileFang(void)
{
    free(pg_GB231232x32Fang);
}

#endif      //CFG_GB2312_32_FANG == from_array

//----��װgb2312 32��������----------------------------------------------------
//����: ��װgb2312 32��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_32_Fang(void)
{
    static struct FontObj font_gb2312_8x32_3232;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii16x32Fang = (u8*)cs_ascii_16x32;
    font_gb2312_8x32_3232.MaxWidth = FONT_HZ32_W;
    font_gb2312_8x32_3232.MaxHeight = FONT_HZ32_H;
    font_gb2312_8x32_3232.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_32_FANG == from_array
    pg_GB231232x32Fang = (u8 *)cs_GB2312_Fang_32x32;
    font_gb2312_8x32_3232.LoadFont = NULL;
    font_gb2312_8x32_3232.UnloadFont = NULL;
#else       //CFG_GB2312_32_FANG == from_array
    __Gb2312_32LoadFromFileFang(CFG_GB2312_32_FANG_FILENAME);  //�����ֿ�
    font_gb2312_8x32_3232.LoadFont = __Gb2312_32LoadFromFileFang;
    font_gb2312_8x32_3232.UnloadFont = __Gb2312_32UnLoadFromFileFang;
#endif      //CFG_GB2312_32_FANG == from_array
    font_gb2312_8x32_3232.GetBitmap = __Font_Gb2312_32GetCharBitMapFang;
    font_gb2312_8x32_3232.GetCharWidth = __Font_Gb2312_32GetCharWidthFang;
    font_gb2312_8x32_3232.GetCharHeight = __Font_Gb2312_32GetCharHeightFang;

    if(Font_InstallFont(&font_gb2312_8x32_3232, CN_FONT_GB2312_FANG_32))
    {
        debug_printf("dot-gb2312","gb2312 32��������尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 32��������尲װʧ��\n\r");
        return 0;
    }
}
#endif      //CFG_GB2312_32_FANG != zk_disable

#if CFG_GB2312_32_HEI != zk_disable
#include "../dotfont-ascii/dot-ascii16x32hei.h"

bool_t __Gb2312_32LoadFromFileHei(const char* FileName);
void __Gb2312_32UnLoadFromFileHei(void);
bool_t __Font_Gb2312_32GetCharBitMapHei(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231232x32Hei;
u8 *pg_Ascii16x32Hei;

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ32�������庺�ֺ�ascii 8*32�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_32GetCharBitMapHei(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ32_ASCII_BYTES;
        bitmap->width = FONT_HZ32_ASCII_W;
        bitmap->height = FONT_HZ32_H;
        bitmap->linebytes = (FONT_HZ32_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<32; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii16x32Hei[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ32_GLYPH_BYTES;
        bitmap->width = FONT_HZ32_W;
        bitmap->height = FONT_HZ32_H;
        bitmap->linebytes = (FONT_HZ32_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ32_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231232x32Hei[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_32GetCharWidthHei(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ32_ASCII_W;
            }
            else
            {
                a=FONT_HZ32_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_32GetCharHeightHei(u32 CharCode)
{
    return 32;
}

#if CFG_GB2312_32_HEI == from_array

#include "dot-gb2312_32hei.rsc"

#else       //CFG_GB2312_32_HEI == from_array

bool_t __Gb2312_32LoadFromFileHei(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231232x32Hei = M_Malloc(FONT_HZ32_GLYPH_BYTES*8813,0);
    if(pg_GB231232x32Hei != NULL)
    {
        fread(pg_GB231232x32Hei,FONT_HZ32_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_32UnLoadFromFileHei(void)
{
    free(pg_GB231232x32Hei);
}

#endif      //CFG_GB2312_32_HEI == from_array

//----��װgb2312 32��������----------------------------------------------------
//����: ��װgb2312 32��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_32_Hei(void)
{
    static struct FontObj font_gb2312_8x32_3232;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii16x32Hei = (u8*)cs_ascii_hei_16x32;
    font_gb2312_8x32_3232.MaxWidth = FONT_HZ32_W;
    font_gb2312_8x32_3232.MaxHeight = FONT_HZ32_H;
    font_gb2312_8x32_3232.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_32_HEI == from_array
    pg_GB231232x32Hei = (u8*)cs_GB2312_Hei_32x32;
    font_gb2312_8x32_3232.LoadFont = NULL;
    font_gb2312_8x32_3232.UnloadFont = NULL;
#else       //CFG_GB2312_32_HEI == from_array
    __Gb2312_32LoadFromFileHei(CFG_GB2312_32_HEI_FILENAME);  //�����ֿ�
    font_gb2312_8x32_3232.LoadFont = __Gb2312_32LoadFromFileHei;
    font_gb2312_8x32_3232.UnloadFont = __Gb2312_32UnLoadFromFileHei;
#endif      //CFG_GB2312_32_HEI == from_array
    font_gb2312_8x32_3232.GetBitmap = __Font_Gb2312_32GetCharBitMapHei;
    font_gb2312_8x32_3232.GetCharWidth = __Font_Gb2312_32GetCharWidthHei;
    font_gb2312_8x32_3232.GetCharHeight = __Font_Gb2312_32GetCharHeightHei;

    if(Font_InstallFont(&font_gb2312_8x32_3232, CN_FONT_GB2312_HEI_32))
    {
        debug_printf("dot-gb2312","gb2312 32������尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 32������尲װʧ��\n\r");
        return 0;
    }
}

#endif      //CFG_GB2312_32_HEI != zk_disable

#if CFG_GB2312_32_KAI != zk_disable
#include "../dotfont-ascii/dot-ascii8x32song.h"

bool_t __Gb2312_32LoadFromFileKai(const char* FileName);
void __Gb2312_32UnLoadFromFileKai(void);
bool_t __Font_Gb2312_32GetCharBitMapKai(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231232x32Kai;
u8 *pg_Ascii16x32Kai;

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ32�������庺�ֺ�ascii 8*32�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_32GetCharBitMapKai(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ32_ASCII_BYTES;
        bitmap->width = FONT_HZ32_ASCII_W;
        bitmap->height = FONT_HZ32_H;
        bitmap->linebytes = (FONT_HZ32_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<32; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii16x32Kai[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ32_GLYPH_BYTES;
        bitmap->width = FONT_HZ32_W;
        bitmap->height = FONT_HZ32_H;
        bitmap->linebytes = (FONT_HZ32_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ32_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231232x32Kai[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_32GetCharWidthKai(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ32_ASCII_W;
            }
            else
            {
                a=FONT_HZ32_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_32GetCharHeightKai(u32 CharCode)
{
    return 32;
}

#if CFG_GB2312_32_KAI == from_array

#include "dot-gb2312_32song.rsc"

#else       //CFG_GB2312_32_KAI == from_array

bool_t __Gb2312_32LoadFromFileKai(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231232x32Kai = M_Malloc(FONT_HZ32_GLYPH_BYTES*8813,0);
    if(pg_GB231232x32Kai != NULL)
    {
        fread(pg_GB231232x32Kai,FONT_HZ32_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_32UnLoadFromFileKai(void)
{
    free(pg_GB231232x32Kai);
}

#endif      //CFG_GB2312_32_KAI == from_array

//----��װgb2312 32��������----------------------------------------------------
//����: ��װgb2312 32��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_32_Kai(void)
{
    static struct FontObj font_gb2312_8x32_3232;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii16x32Kai = (u8*)cs_ascii_16x32;
    font_gb2312_8x32_3232.MaxWidth = FONT_HZ32_W;
    font_gb2312_8x32_3232.MaxHeight = FONT_HZ32_H;
    font_gb2312_8x32_3232.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_32_KAI == from_array
    pg_GB231232x32Kai = (u8*)cs_GB2312_Kai_32x32;
    font_gb2312_8x32_3232.LoadFont = NULL;
    font_gb2312_8x32_3232.UnloadFont = NULL;
#else       //CFG_GB2312_32_KAI == from_array
    __Gb2312_32LoadFromFileKai(CFG_GB2312_32_KAI_FILENAME);  //�����ֿ�
    font_gb2312_8x32_3232.LoadFont = __Gb2312_32LoadFromFileKai;
    font_gb2312_8x32_3232.UnloadFont = __Gb2312_32UnLoadFromFileKai;
#endif      //CFG_GB2312_32_KAI == from_array
    font_gb2312_8x32_3232.GetBitmap = __Font_Gb2312_32GetCharBitMapKai;
    font_gb2312_8x32_3232.GetCharWidth = __Font_Gb2312_32GetCharWidthKai;
    font_gb2312_8x32_3232.GetCharHeight = __Font_Gb2312_32GetCharHeightKai;

    if(Font_InstallFont(&font_gb2312_8x32_3232, CN_FONT_GB2312_KAI_32))
    {
        debug_printf("dot-gb2312","gb2312 32�����尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 32�����尲װʧ��\n\r");
        return 0;
    }
}

#endif      //CFG_GB2312_32_KAI != zk_disable

#if CFG_GB2312_32_YUAN != zk_disable
#include "../dotfont-ascii/dot-ascii8x32yuan.h"

bool_t __Gb2312_32LoadFromFileYuan(const char* FileName);
void __Gb2312_32UnLoadFromFileYuan(void);
bool_t __Font_Gb2312_32GetCharBitMapYuan(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231232x32Yuan;
u8 *pg_Ascii16x32Yuan;

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ32�������庺�ֺ�ascii 8*32�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size���ֺţ�����������
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_32GetCharBitMapYuan(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;
    // �õ��ַ���ģ�������е�ƫ����
    if (s_ptGb2312Set->Ucs4ToMb((char*)gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_GB2312_DEF_CHAR &0xff;
        gbcode[1] = (FONT_GB2312_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    if(charcode < 0x80)
    {
        offset = charcode * FONT_HZ32_ASCII_BYTES;
        bitmap->width = FONT_HZ32_ASCII_W;
        bitmap->height = FONT_HZ32_H;
        bitmap->linebytes = (FONT_HZ32_ASCII_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<32; i++)
            {
                bitmap->bm_bits[i] = pg_Ascii16x32Yuan[offset + i];
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*FONT_HZ32_GLYPH_BYTES;
        bitmap->width = FONT_HZ32_W;
        bitmap->height = FONT_HZ32_H;
        bitmap->linebytes = (FONT_HZ32_W+7)/8;
        bitmap->PixelFormat = CN_SYS_PF_GRAY1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ32_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_GB231232x32Yuan[offset + i];
            }
        }
    }

    return result;
}

//----��ȡ�ַ����-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ�����
//����: CharCode,����ѯ���ַ�
//����: �ַ����
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_32GetCharWidthYuan(u32 CharCode)
{
    int a=0;


    switch(CharCode)
    {
    case '\r':
    case '\n':
            a=0;
            break;
    default:
            if(CharCode<256)
            {
                a=FONT_HZ32_ASCII_W;
            }
            else
            {
                a=FONT_HZ32_W;
            }
            break;
    }

    return a;

}

//----��ȡ�ַ��߶�-------------------------------------------------------------
//����: ȡĳ�ַ��ĵ���߶�
//����: CharCode,����ѯ���ַ�
//����: �ַ��߶�
//-----------------------------------------------------------------------------
s32 __Font_Gb2312_32GetCharHeightYuan(u32 CharCode)
{
    return 32;
}

#if CFG_GB2312_32_YUAN == from_array

#include "dot-gb2312_32yuan.rsc"

#else       //CFG_GB2312_32_YUAN == from_array

bool_t __Gb2312_32LoadFromFileYuan(const char* FileName)
{
    bool_t result;
    FILE *zk_gb2312;
    //���ֿ��м��غ��ֵ���
    zk_gb2312 = fopen(FileName,"rb");
    if(zk_gb2312 == NULL)
        return false;
    pg_GB231232x32Yuan = M_Malloc(FONT_HZ32_GLYPH_BYTES*8813,0);
    if(pg_GB231232x32Yuan != NULL)
    {
        fread(pg_GB231232x32Yuan,FONT_HZ32_GLYPH_BYTES*8813,1,zk_gb2312);
        result = true;
    }
    else
        result = false;
    fclose(zk_gb2312);
    return result;
}

void __Gb2312_32UnLoadFromFileYuan(void)
{
    free(pg_GB231232x32Yuan);
}

#endif      //CFG_GB2312_32_YUAN == from_array

//----��װgb2312 32��������----------------------------------------------------
//����: ��װgb2312 32��������,��ģ�������ļ���,�ļ����ɲ������롣
//����: ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_32_Yuan(void)
{
    static struct FontObj font_gb2312_8x32_3232;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 �ַ���δ��װ\n\r");
        return false;
    }

    pg_Ascii16x32Yuan = (u8*)cs_ascii_16x32;
    font_gb2312_8x32_3232.MaxWidth = FONT_HZ32_W;
    font_gb2312_8x32_3232.MaxHeight = FONT_HZ32_H;
    font_gb2312_8x32_3232.Attr = CN_FONT_TYPE_DOT;
#if CFG_GB2312_32_YUAN == from_array
    pg_GB231232x32Yuan = (u8*)cs_GB2312_Yuan_32x32;
    font_gb2312_8x32_3232.LoadFont = NULL;
    font_gb2312_8x32_3232.UnloadFont = NULL;
#else       //CFG_GB2312_32_YUAN == from_array
    __Gb2312_32LoadFromFileYuan(CFG_GB2312_32_YUAN_FILENAME);  //�����ֿ�
    font_gb2312_8x32_3232.LoadFont = __Gb2312_32LoadFromFileYuan;
    font_gb2312_8x32_3232.UnloadFont = __Gb2312_32UnLoadFromFileYuan;
#endif      //CFG_GB2312_32_YUAN == from_array
    font_gb2312_8x32_3232.GetBitmap = __Font_Gb2312_32GetCharBitMapYuan;
    font_gb2312_8x32_3232.GetCharWidth = __Font_Gb2312_32GetCharWidthYuan;
    font_gb2312_8x32_3232.GetCharHeight = __Font_Gb2312_32GetCharHeightYuan;

    if(Font_InstallFont(&font_gb2312_8x32_3232, CN_FONT_GB2312_YUAN_32))
    {
        debug_printf("dot-gb2312","gb2312 32����Բ�尲װ���\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 32����Բ�尲װʧ��\n\r");
        return 0;
    }
}

#endif      //CFG_GB2312_32_YUAN != zk_disable

#pragma GCC diagnostic pop

void ModuleInstall_FontGB2312(void)
{
    #if CFG_GB2312_12_SONG != zk_disable
    ModuleInstall_FontGb2312_12_Song( );
    #endif      //CFG_GB2312_12 != zk_disable
    #if CFG_GB2312_16_SONG != zk_disable
    ModuleInstall_FontGb2312_16_Song( );
    #endif      //CFG_GB2312_16_SONG != zk_disable
    #if CFG_GB2312_16_YAHEI != zk_disable
    ModuleInstall_FontGb2312_16_Yahei( );
    #endif      //CFG_GB2312_16_YAHEI != zk_disable
    #if CFG_GB2312_16_YUAN != zk_disable
    ModuleInstall_FontGb2312_16_Yuan( );
    #endif      //CFG_GB2312_16_YUAN != zk_disable
    #if CFG_GB2312_16_KAI != zk_disable
    ModuleInstall_FontGb2312_16_Kai( );
    #endif      //CFG_GB2312_16_KAI != zk_disable
    #if CFG_GB2312_16_HEI != zk_disable
    ModuleInstall_FontGb2312_16_Hei( );
    #endif      //CFG_GB2312_16_HEI != zk_disable
    #if CFG_GB2312_16_FANG != zk_disable
    ModuleInstall_FontGb2312_16_Fang( );
    #endif      //CFG_GB2312_16_FANG != zk_disable
    #if CFG_GB2312_24_SONG != zk_disable
    ModuleInstall_FontGb2312_24_Song( );
    #endif      //CFG_GB2312_24_SONG != zk_disable
    #if CFG_GB2312_24_YAHEI != zk_disable
    ModuleInstall_FontGb2312_24_Yahei( );
    #endif      //CFG_GB2312_24_YAHEI != zk_disable
    #if CFG_GB2312_24_YUAN != zk_disable
    ModuleInstall_FontGb2312_24_Yuan( );
    #endif      //CFG_GB2312_24_YUAN != zk_disable
    #if CFG_GB2312_24_KAI != zk_disable
    ModuleInstall_FontGb2312_24_Kai( );
    #endif      //CFG_GB2312_24_KAI != zk_disable
    #if CFG_GB2312_24_HEI != zk_disable
    ModuleInstall_FontGb2312_24_Hei( );
    #endif      //CFG_GB2312_24_HEI != zk_disable
    #if CFG_GB2312_24_FANG != zk_disable
    ModuleInstall_FontGb2312_24_Fang( );
    #endif      //CFG_GB2312_24_FANG != zk_disable
    #if CFG_GB2312_32_SONG != zk_disable
    ModuleInstall_FontGb2312_32_Song( );
    #endif      //CFG_GB2312_32_SONG != zk_disable
    #if CFG_GB2312_32_YUAN != zk_disable
    ModuleInstall_FontGb2312_32_Yuan( );
    #endif      //CFG_GB2312_32_YUAN != zk_disable
    #if CFG_GB2312_32_KAI != zk_disable
    ModuleInstall_FontGb2312_32_Kai( );
    #endif      //CFG_GB2312_32_KAI != zk_disable
    #if CFG_GB2312_32_HEI != zk_disable
    ModuleInstall_FontGb2312_32_Hei( );
    #endif      //CFG_GB2312_32_HEI != zk_disable
    #if CFG_GB2312_32_FANG != zk_disable
    ModuleInstall_FontGb2312_32_Fang( );
    #endif      //CFG_GB2312_32_FANG != zk_disable
}
