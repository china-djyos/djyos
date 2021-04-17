//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��:menu tree build module
//����:  .
//�汾��V1.0.0
//�ļ�����:�Բ˵����ĵ�֧��
//����˵��:
//�޶���ʷ:
//
//1. ����: 2012-11-5
//   ����:
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "dbug.h"
#if 0

#define cfg_charset_cyril  1
#define cfg_cyril_16  1
#define cn_font_cyril16  "cyril_fontx16"

unsigned char *FileName="arialuni_M16_CP1251.bin";
unsigned char *LanguageCode;

unsigned char *Cyril16mbcs_encode;
unsigned char *Cyril16mbcs_bmap;
//unsigned char *text="���ڧӧ�?



unsigned char * GetLanguagecodeFromFile(const char *FileName)
{

   FILE  *fp;
   struct FontLibHeader fileHeader;
   u32    FileLength;
   unsigned char *LanguageCode=NULL;

   fp=fopen(FileName,"rb");
   if(fp==NULL)
    {
       debug_printf("cyril","open the file failed!\n");
      goto exit;
   }
   fseek(fp,0L,SEEK_SET);

   fread(&fileHeader,sizeof(struct FontLibHeader),1,fp);

    if((fileHeader.magic[0] != 'U' && fileHeader.magic[0] != 'M'))
    {
        debug_printf("cyril","Cann't support file format!\n");
        return 0;
    }

    if('M' == fileHeader.magic[0])     //mbcs ����
    {
        debug_printf("cyril","This is the Mbcs code!\n");
    }
    else
    {
        debug_printf("cyril","This is the Unicode code!\n");
    }
    //��fileHeader�����ݵ�˳����е���
  //  AdjustProcedure( &fileHeader.Size, &fileHeader.Size, sizeof(fileHeader.Size));
//  AdjustProcedure( &fileHeader.wCpFlag, &fileHeader.wCpFlag, sizeof(fileHeader.wCpFlag));

    debug_printf("cyril","filesize=%08x\n",fileHeader.Size);
    debug_printf("cyril","LanguageF=%04x\n",fileHeader.wCpFlag);



    LanguageCode=M_MallocLc(fileHeader.Size,0);
    if(NULL==LanguageCode)
        {

      fclose(fp);
      goto exit;
    }
    fseek(fp,0L,SEEK_SET);
    FileLength=fread(LanguageCode,fileHeader.Size,sizeof(unsigned char),fp);
    debug_printf("cyril","---File--Length=%08x\n",FileLength);
//  memcpy(&fileHeader,LanguageCode,0x10);
    fclose(fp);

exit:
    return LanguageCode;
}

// ע�Ͳ��� charset.h-> struct Charset -> GetOneMb
s32 CyrilGetOneMb(const char* mbs,s32 n)
{
    u8  code=0;

    if( (NULL==mbs) || (LanguageCode==NULL) )
    {
        return -1;
    }
    else
    {
        code=*mbs;
        if((code<0x20) && (code != 0) )     //��������Ҳ�ǺϷ��ַ�
        {
           return -1;
        }
        else
            return 1;
    }
}

// ע�Ͳ��� charset.h-> struct Charset -> MbsToUcs4s
s32 CyrilMbsToUcs4s(u32* pwcs, const char* mbs, s32 n)
{
    return 0;
}

s32 CyrilUcs4sToMbs(u32* pwcs, const char* mbs, s32 n)
{
    return 0;
}
s32 CyrilUcs4ToMb(u32* pwcs, const char* mbs, s32 n)
{
    return 0;
}
s32 CyrilMbToUcs4(u32* pwc, const u8* mbs, s32 n)
{
    u8  code=0;

    if(NULL==mbs)
    {
        return -1;
    }
    code=*mbs;
    if(code<0x20)
    {
       return -1;
    }
    if(LanguageCode==NULL)
    {
        return -1;
    }
    if(pwc != NULL)
        *pwc=*((u32 *)(LanguageCode+0x10+code*4));

    return 1;
}
void module_init_encode_cyril(ptu32_t para)
{
    static struct Charset encoding;
   //�����ֿ�
    debug_printf("cyril","Init the code\n");
    LanguageCode=GetLanguagecodeFromFile(FileName);

    encoding.max_len = 1;
    encoding.EOC_Size = 1;
    encoding.GetOneMb = CyrilGetOneMb;
    encoding.MbToUcs4 = CyrilMbToUcs4;
    encoding.Ucs4ToMb = CyrilUcs4ToMb;
    encoding.MbsToUcs4s = CyrilMbsToUcs4s;
    encoding.Ucs4sToMbs = CyrilUcs4sToMbs;
    if( Charset_NlsInstallCharset(&encoding, CN_NLS_CHARSET_CYRIL))
    {
        debug_printf("cyril","cyril encoding install sucess\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_CHARSET_INSTALL_ERROR);
        debug_printf("cyril","cyril encoding install fail\n\r");
        return 0;
    }


}

//----������ȡ-----------------------------------------------------------------
//����: ��ȡcyril n*16�������壬���charcode����0~0xff�ķ�Χ����ʾ����Ϊ0x00����
//      ����ascii������չ��ascii��
//����: charcode������ʾ��cyril���ucs4����
//      size����Ч
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __cyril16_get_char_bitmap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u32 offset;
    bool_t result = true;

    struct FontLibHeader fileHeader;

    u32  AddrOffset;
    u8   width;
    u8   lineBytes;
    u8   height=16;

    fileHeader=*(struct FontLibHeader *)LanguageCode;


//  debug_printf("cyril","charcode=%08x\n",charcode);
    AddrOffset=(charcode&0x03FFFFFF);//��26λ��ʾƫ�Ƶ�ַ
    width=(charcode>>24);//��6λ��ʾ���
    width=width>>2;
 //   debug_printf("cyril","width=%02x\n",width);

    lineBytes=(width+7)/8;
//    debug_printf("cyril","linbte=%02x\n",lineBytes);
//   debug_printf("cyril","width=%02x\n",width);

    if((AddrOffset+lineBytes*height)>fileHeader.Size)//�����ַ��ļ��ķ�Χ
        {

          result = false;
          debug_printf("cyril","ecceed!\n");
          goto exit;
    }

    // �õ��ַ���ģ�������е�ƫ����
    bitmap->pf_type = CN_SYS_PF_GRAY1;
    bitmap->width = width;
    bitmap->height = height;
    bitmap->linebytes = lineBytes;

    if(bitmap->bm_bits != NULL)
    {
        for(i=0; i<lineBytes*height; i++)
        {
            bitmap->bm_bits[i] = LanguageCode[AddrOffset+i];
        }
    }

exit:
    return result;
}
bool_t __cyril16_load_font(u8* zk_addr)
{


   return true;//do nothing


}
bool_t __cyril16_unload_font(u8* zk_addr)
{

   return true;//do nothing


}
//----��װgb2312 16��������----------------------------------------------------
//����: ��װgb2312 16�������塣
//����: ������
//����: 1=�ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_font_cyril16(ptu32_t para)
{
    static struct FontObj font_cyril16;
  //  __Charset_Gb2312_816_1616LoadFont((u8*)para);  //�����ֿ�
    font_cyril16.fi.type = CN_FONT_TYPE_DOT;
    font_cyril16.LoadFont = __cyril16_load_font;
    font_cyril16.UnloadFont = __cyril16_unload_font;
    font_cyril16.GetBitmap = __cyril16_get_char_bitmap;

    if(Font_InstallFont(&font_cyril16, cn_font_cyril16))
    {
        debug_printf("cyril","cyril  font install sucess\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("cyril","cyril font install fail\n\r");
        return 0;
    }
}




void test_cyril( void)
{
//��ʼ���������


  struct FontObj  *pfont=NULL,font;
  struct Charset  *pencode=NULL,encode;

  #if (cfg_charset_cyril== 1)       //��ʼ��gb2312�ַ���
    module_init_encode_cyril(0);
  #endif


//���³�ʼ���������
  #if (cfg_cyril_16== 1)     //��ʼ��gb2312�����ֿ�
    module_init_font_cyril16(0);
  #endif

  //����cyrilΪĬ������

  encode.node.name="cyril";
  pencode=Charset_NlsSetCurCharset(&encode);
  font.node.name="cyril_fontx16";
  pfont=Font_SetCurFont(&font);

  debug_printf("cyril","cur_encode=%s\n",pencode->node.name);
  debug_printf("cyril","cur_font=%s\n",pfont->node.name);

}

#endif
