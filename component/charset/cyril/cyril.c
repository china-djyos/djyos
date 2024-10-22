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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块:menu tree build module
//作者:  .
//版本：V1.0.0
//文件描述:对菜单俄文的支持
//其他说明:
//修订历史:
//
//1. 日期: 2012-11-5
//   作者:
//   新版本号: V1.0.0
//   修改说明: 原始版本
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
//unsigned char *text="приве?



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

    if('M' == fileHeader.magic[0])     //mbcs 编码
    {
        debug_printf("cyril","This is the Mbcs code!\n");
    }
    else
    {
        debug_printf("cyril","This is the Unicode code!\n");
    }
    //对fileHeader的数据的顺序进行调整
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

// 注释参照 charset.h-> struct Charset -> GetOneMb
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
        if((code<0x20) && (code != 0) )     //串结束符也是合法字符
        {
           return -1;
        }
        else
            return 1;
    }
}

// 注释参照 charset.h-> struct Charset -> MbsToUcs4s
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
   //加载字库
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

//----点阵提取-----------------------------------------------------------------
//功能: 提取cyril n*16点阵字体，如果charcode超出0~0xff的范围，显示编码为0x00的字
//      符，ascii包含扩展的ascii码
//参数: charcode，待显示的cyril码得ucs4编码
//      size，无效
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
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
    AddrOffset=(charcode&0x03FFFFFF);//低26位表示偏移地址
    width=(charcode>>24);//高6位表示宽度
    width=width>>2;
 //   debug_printf("cyril","width=%02x\n",width);

    lineBytes=(width+7)/8;
//    debug_printf("cyril","linbte=%02x\n",lineBytes);
//   debug_printf("cyril","width=%02x\n",width);

    if((AddrOffset+lineBytes*height)>fileHeader.Size)//超出字符的集的范围
        {

          result = false;
          debug_printf("cyril","ecceed!\n");
          goto exit;
    }

    // 得到字符字模在数组中的偏移量
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
//----安装gb2312 16点阵字体----------------------------------------------------
//功能: 安装gb2312 16点阵字体。
//参数: 无意义
//返回: 1=成功，0=失败
//-----------------------------------------------------------------------------
ptu32_t module_init_font_cyril16(ptu32_t para)
{
    static struct FontObj font_cyril16;
  //  __Charset_Gb2312_816_1616LoadFont((u8*)para);  //加载字库
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
//初始化字体编码


  struct FontObj  *pfont=NULL,font;
  struct Charset  *pencode=NULL,encode;

  #if (cfg_charset_cyril== 1)       //初始化gb2312字符集
    module_init_encode_cyril(0);
  #endif


//以下初始化字体点阵
  #if (cfg_cyril_16== 1)     //初始化gb2312点阵字库
    module_init_font_cyril16(0);
  #endif

  //设置cyril为默认字体

  encode.node.name="cyril";
  pencode=Charset_NlsSetCurCharset(&encode);
  font.node.name="cyril_fontx16";
  pfont=Font_SetCurFont(&font);

  debug_printf("cyril","cur_encode=%s\n",pencode->node.name);
  debug_printf("cyril","cur_font=%s\n",pfont->node.name);

}

#endif
