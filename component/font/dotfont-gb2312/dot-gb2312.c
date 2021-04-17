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
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

//所属模块: GB2312-80 16*16点阵宋体驱动
//作者：mjh
//版本：V1.0.0
//文件描述: GB2312-80字体解析
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:2011-05-17
//       作者:lst
//       新版本号：
//       修改说明:初始版本
//------------------------------------------------------
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "gkernel.h"
#include "charset.h"
#include "dbug.h"
#include "./font/font.h"
#include "./font/font_gb2312.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。
#define zk_disable      0
#define from_file       1
#define from_array      2
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern void ModuleInstall_FontGB2312(void);
//  ModuleInstall_FontGB2312();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"gb2312 dot"//gb2312点阵字体,12~32点阵
//parent:"font"      //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"font","gb2312 charset"  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"ascii dot font"//该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_GB2312_DOT == false )
//#warning  " gb2312_dot  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_GB2312_DOT    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_12_SONG              zk_disable              //"12点阵宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_12_SONG_FILENAME "zk_gb2316_12song.bin"  //"字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_16_SONG              from_array              //"16点阵宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_16_SONG_FILENAME "zk_gb2316_16song.bin"  //"字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_16_YAHEI             zk_disable              //"16点阵微软雅黑",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_16_YAHEI_FILENAME "zk_gb2316_16yahei.bin"  //"字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_16_YUAN              zk_disable              //"16点阵圆体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_16_YUAN_FILENAME "zk_gb2316_16yuan.bin"  //"字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_16_KAI              zk_disable               //"16点阵楷体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_16_KAI_FILENAME  "zk_gb2316_16kai.bin"   //"字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_16_HEI              zk_disable               //"16点阵黑",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_16_HEI_FILENAME  "zk_gb2316_16hei.bin"   //"字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_16_FANG              zk_disable              //"16点阵仿宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_16_FANG_FILENAME "zk_gb2316_16fang.bin"  //"字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_24_SONG              zk_disable              //"24点阵宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_24_SONG_FILENAME "zk_gb2316_24song.bin"  //"字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_24_YAHEI              zk_disable             // "24点阵微软雅黑",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_24_YAHEI_FILENAME "zk_gb2316_24yahei.bin"// "字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_24_YUAN              zk_disable              //"24点阵圆体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_24_YUAN_FILENAME "zk_gb2316_24yuan.bin"  //"字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_24_KAI              zk_disable               //"24点阵楷体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_24_KAI_FILENAME  "zk_gb2316_24kai.bin"   //"字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_24_HEI              zk_disable               //"24点阵黑体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_24_HEI_FILENAME  "zk_gb2316_24hei.bin"   //"字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_24_FANG              zk_disable              //"24点阵仿宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_24_FANG_FILENAME "zk_gb2316_24fang.bin"  //"字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_32_SONG              zk_disable              //"32点阵宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_32_SONG_FILENAME "zk_gb2316_32song.bin"  //"字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_32_YUAN              zk_disable              //"32点阵圆体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_32_YUAN_FILENAME "zk_gb2316_32yuan.bin"  //"字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_32_KAI              zk_disable               //"32点阵楷体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_32_KAI_FILENAME  "zk_gb2316_32kai.bin"   //"字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_32_HEI              zk_disable               //"32点阵黑体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_32_HEI_FILENAME  "zk_gb2316_32hei.bin"   //"字库文件名",若从文件读取，则配置文件名
//%$#@enum,zk_disable,from_file,from_array
#define CFG_GB2312_32_FANG              zk_disable              //"32点阵仿宋体",GB2312字体,zk_disable：不需要，from_array：从数组读取，from_file：从文件读
//%$#@string,0,255,
    #define CFG_GB2312_32_FANG_FILENAME "zk_gb2316_32fang.bin"  //"字库文件名",若从文件读取，则配置文件名
//%$#@string,0,255,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
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
#define FONT_HZ12_GLYPH_BYTES   24              // 字符字模的字节数
#define FONT_HZ12_ASCII_BYTES   12              // ASCII的字节数

//----点阵提取-----------------------------------------------------------------
//功能: 提取12点阵宋体汉字和ascii 8*12点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_12GetCharBitMapSong(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 12点阵字体----------------------------------------------------
//功能: 安装gb2312 12点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_12_Song(void)
{
    static struct FontObj font_gb2312_8x12_1212;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_12LoadFromFileSong(CFG_GB2312_12_SONG_FILENAME);  //加载字库
    font_gb2312_8x12_1212.LoadFont = __Gb2312_12LoadFromFileSong;
    font_gb2312_8x12_1212.UnloadFont = __Gb2312_12UnLoadFromFileSong;
#endif      //CFG_GB2312_12_SONG == from_array
    font_gb2312_8x12_1212.GetBitmap = __Font_Gb2312_12GetCharBitMapSong;
    font_gb2312_8x12_1212.GetCharWidth = __Font_Gb2312_12GetCharWidthSong;
    font_gb2312_8x12_1212.GetCharHeight = __Font_Gb2312_12GetCharHeightSong;

    if(Font_InstallFont(&font_gb2312_8x12_1212, CN_FONT_GB2312_SONG_12))
    {
        debug_printf("dot-gb2312","gb2312 12点阵宋体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 12点阵宋体安装失败\n\r");
        return 0;
    }
}
#endif      //CFG_GB2312_12_SONG != zk_disable

#define FONT_HZ16_W             16              // Width
#define FONT_HZ16_H             16              // Height
#define FONT_HZ16_ASCII_W       8              // Width
#define FONT_HZ16_GLYPH_BYTES   32              // 字符字模的字节数
#define FONT_HZ16_ASCII_BYTES   16              // ASCII的字节数

#if CFG_GB2312_16_SONG != zk_disable
#include "../dotfont-ascii/dot-ascii8x16song.h"

bool_t __Gb2312_16LoadFromFileSong(const char* FileName);
void __Gb2312_16UnLoadFromFileSong(void);
bool_t __Font_Gb2312_16GetCharBitMapSong(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231216x16Song;
u8 *pg_Ascii8x16Song;

//----点阵提取-----------------------------------------------------------------
//功能: 提取16点阵宋体汉字和ascii 8*16点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_16GetCharBitMapSong(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 16点阵字体----------------------------------------------------
//功能: 安装gb2312 16点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_16_Song(void)
{
    static struct FontObj font_gb2312_8x16_1616;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_16LoadFromFileSong(CFG_GB2312_16_SONG_FILENAME);  //加载字库
    font_gb2312_8x16_1616.LoadFont = __Gb2312_16LoadFromFileSong;
    font_gb2312_8x16_1616.UnloadFont = __Gb2312_16UnLoadFromFileSong;
#endif      //CFG_GB2312_16_SONG == from_array
    font_gb2312_8x16_1616.GetBitmap = __Font_Gb2312_16GetCharBitMapSong;
    font_gb2312_8x16_1616.GetCharWidth = __Font_Gb2312_16GetCharWidthSong;
    font_gb2312_8x16_1616.GetCharHeight = __Font_Gb2312_16GetCharHeightSong;

    if(Font_InstallFont(&font_gb2312_8x16_1616, CN_FONT_GB2312_SONG_16))
    {
        debug_printf("dot-gb2312","gb2312 16点阵宋体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 16点阵宋体安装失败\n\r");
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

//----点阵提取-----------------------------------------------------------------
//功能: 提取16点阵宋体汉字和ascii 8*16点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_16GetCharBitMapYahei(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 16点阵字体----------------------------------------------------
//功能: 安装gb2312 16点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_16_Yahei(void)
{
    static struct FontObj font_gb2312_8x16_1616;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_16LoadFromFileYahei(CFG_GB2312_16_YAHEI_FILENAME);  //加载字库
    font_gb2312_8x16_1616.LoadFont = __Gb2312_16LoadFromFileYahei;
    font_gb2312_8x16_1616.UnloadFont = __Gb2312_16UnLoadFromFileYahei;
#endif      //CFG_GB2312_16_YAHEI == from_array
    font_gb2312_8x16_1616.GetBitmap = __Font_Gb2312_16GetCharBitMapYahei;
    font_gb2312_8x16_1616.GetCharWidth = __Font_Gb2312_16GetCharWidthYahei;
    font_gb2312_8x16_1616.GetCharHeight = __Font_Gb2312_16GetCharHeightYahei;

    if(Font_InstallFont(&font_gb2312_8x16_1616, CN_FONT_GB2312_YAHEI_16))
    {
        debug_printf("dot-gb2312","gb2312 16点阵宋体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 16点阵宋体安装失败\n\r");
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

//----点阵提取-----------------------------------------------------------------
//功能: 提取16点阵宋体汉字和ascii 8*16点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_16GetCharBitMapFang(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 16点阵字体----------------------------------------------------
//功能: 安装gb2312 16点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_16_Fang(void)
{
    static struct FontObj font_gb2312_8x16_1616;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_16LoadFromFileFang(CFG_GB2312_16_FANG_FILENAME);  //加载字库
    font_gb2312_8x16_1616.LoadFont = __Gb2312_16LoadFromFileFang;
    font_gb2312_8x16_1616.UnloadFont = __Gb2312_16UnLoadFromFileFang;
#endif      //CFG_GB2312_16_FANG == from_array
    font_gb2312_8x16_1616.GetBitmap = __Font_Gb2312_16GetCharBitMapFang;
    font_gb2312_8x16_1616.GetCharWidth = __Font_Gb2312_16GetCharWidthFang;
    font_gb2312_8x16_1616.GetCharHeight = __Font_Gb2312_16GetCharHeightFang;

    if(Font_InstallFont(&font_gb2312_8x16_1616, CN_FONT_GB2312_FANG_16))
    {
        debug_printf("dot-gb2312","gb2312 16点阵仿宋体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 16点阵仿宋体安装失败\n\r");
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

//----点阵提取-----------------------------------------------------------------
//功能: 提取16点阵宋体汉字和ascii 8*16点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_16GetCharBitMapHei(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 16点阵字体----------------------------------------------------
//功能: 安装gb2312 16点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_16_Hei(void)
{
    static struct FontObj font_gb2312_8x16_1616;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_16LoadFromFileHei(CFG_GB2312_16_HEI_FILENAME);  //加载字库
    font_gb2312_8x16_1616.LoadFont = __Gb2312_16LoadFromFileHei;
    font_gb2312_8x16_1616.UnloadFont = __Gb2312_16UnLoadFromFileHei;
#endif      //CFG_GB2312_16_HEI == from_array
    font_gb2312_8x16_1616.GetBitmap = __Font_Gb2312_16GetCharBitMapHei;
    font_gb2312_8x16_1616.GetCharWidth = __Font_Gb2312_16GetCharWidthHei;
    font_gb2312_8x16_1616.GetCharHeight = __Font_Gb2312_16GetCharHeightHei;

    if(Font_InstallFont(&font_gb2312_8x16_1616, CN_FONT_GB2312_HEI_16))
    {
        debug_printf("dot-gb2312","gb2312 16点阵黑体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 16点阵黑体安装失败\n\r");
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

//----点阵提取-----------------------------------------------------------------
//功能: 提取16点阵宋体汉字和ascii 8*16点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_16GetCharBitMapKai(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 16点阵字体----------------------------------------------------
//功能: 安装gb2312 16点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_16_Kai(void)
{
    static struct FontObj font_gb2312_8x16_1616;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_16LoadFromFileKai(CFG_GB2312_16_KAI_FILENAME);  //加载字库
    font_gb2312_8x16_1616.LoadFont = __Gb2312_16LoadFromFileKai;
    font_gb2312_8x16_1616.UnloadFont = __Gb2312_16UnLoadFromFileKai;
#endif      //CFG_GB2312_16_KAI == from_array
    font_gb2312_8x16_1616.GetBitmap = __Font_Gb2312_16GetCharBitMapKai;
    font_gb2312_8x16_1616.GetCharWidth = __Font_Gb2312_16GetCharWidthKai;
    font_gb2312_8x16_1616.GetCharHeight = __Font_Gb2312_16GetCharHeightKai;

    if(Font_InstallFont(&font_gb2312_8x16_1616, CN_FONT_GB2312_KAI_16))
    {
        debug_printf("dot-gb2312","gb2312 16点阵楷体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 16点阵楷体安装失败\n\r");
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

//----点阵提取-----------------------------------------------------------------
//功能: 提取16点阵宋体汉字和ascii 8*16点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_16GetCharBitMapYuan(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 16点阵字体----------------------------------------------------
//功能: 安装gb2312 16点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_16_Yuan(void)
{
    static struct FontObj font_gb2312_8x16_1616;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_16LoadFromFileYuan(CFG_GB2312_16_YUAN_FILENAME);  //加载字库
    font_gb2312_8x16_1616.LoadFont = __Gb2312_16LoadFromFileYuan;
    font_gb2312_8x16_1616.UnloadFont = __Gb2312_16UnLoadFromFileYuan;
#endif      //CFG_GB2312_16_YUAN == from_array
    font_gb2312_8x16_1616.GetBitmap = __Font_Gb2312_16GetCharBitMapYuan;
    font_gb2312_8x16_1616.GetCharWidth = __Font_Gb2312_16GetCharWidthYuan;
    font_gb2312_8x16_1616.GetCharHeight = __Font_Gb2312_16GetCharHeightYuan;

    if(Font_InstallFont(&font_gb2312_8x16_1616, CN_FONT_GB2312_YUAN_16))
    {
        debug_printf("dot-gb2312","gb2312 16点阵圆体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 16点阵圆体安装失败\n\r");
        return 0;
    }
}

#endif      //CFG_GB2312_16_YUAN != zk_disable

#define FONT_HZ24_W             24              // Width
#define FONT_HZ24_H             24              // Height
#define FONT_HZ24_ASCII_W       12              // Width
#define FONT_HZ24_GLYPH_BYTES   72              // 字符字模的字节数
#define FONT_HZ24_ASCII_BYTES   48              // ASCII的字节数

#if CFG_GB2312_24_SONG != zk_disable
#include "../dotfont-ascii/dot-ascii12x24song.h"

bool_t __Gb2312_24LoadFromFileSong(const char* FileName);
void __Gb2312_24UnLoadFromFileSong(void);
bool_t __Font_Gb2312_24GetCharBitMapSong(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231224x24Song;
u8 *pg_Ascii12x24Song;

//----点阵提取-----------------------------------------------------------------
//功能: 提取24点阵宋体汉字和ascii 8*24点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_24GetCharBitMapSong(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 24点阵字体----------------------------------------------------
//功能: 安装gb2312 24点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_24_Song(void)
{
    static struct FontObj font_gb2312_8x24_2424;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_24LoadFromFileSong(CFG_GB2312_24_SONG_FILENAME);  //加载字库
    font_gb2312_8x24_2424.LoadFont = __Gb2312_24LoadFromFileSong;
    font_gb2312_8x24_2424.UnloadFont = __Gb2312_24UnLoadFromFileSong;
#endif      //CFG_GB2312_24_SONG == from_array
    font_gb2312_8x24_2424.GetBitmap = __Font_Gb2312_24GetCharBitMapSong;
    font_gb2312_8x24_2424.GetCharWidth = __Font_Gb2312_24GetCharWidthSong;
    font_gb2312_8x24_2424.GetCharHeight = __Font_Gb2312_24GetCharHeightSong;

    if(Font_InstallFont(&font_gb2312_8x24_2424, CN_FONT_GB2312_SONG_24))
    {
        debug_printf("dot-gb2312","gb2312 24点阵宋体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 24点阵宋体安装失败\n\r");
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

//----点阵提取-----------------------------------------------------------------
//功能: 提取24点阵微软雅黑汉字和ascii 12*24点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_24GetCharBitMapYahei(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 24点阵字体----------------------------------------------------
//功能: 安装gb2312 24点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_24_Yahei(void)
{
    static struct FontObj font_gb2312_8x24_2424;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_24LoadFromFileYahei(CFG_GB2312_24_YAHEI_FILENAME);  //加载字库
    font_gb2312_8x24_2424.LoadFont = __Gb2312_24LoadFromFileYahei;
    font_gb2312_8x24_2424.UnloadFont = __Gb2312_24UnLoadFromFileYahei;
#endif      //CFG_GB2312_24_Yahei == from_array
    font_gb2312_8x24_2424.GetBitmap = __Font_Gb2312_24GetCharBitMapYahei;
    font_gb2312_8x24_2424.GetCharWidth = __Font_Gb2312_24GetCharWidthYahei;
    font_gb2312_8x24_2424.GetCharHeight = __Font_Gb2312_24GetCharHeightYahei;

    if(Font_InstallFont(&font_gb2312_8x24_2424, CN_FONT_GB2312_YAHEI_24))
    {
        debug_printf("dot-gb2312","gb2312 24点阵宋体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 24点阵宋体安装失败\n\r");
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


//----点阵提取-----------------------------------------------------------------
//功能: 提取24点阵宋体汉字和ascii 8*24点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_24GetCharBitMapFang(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 24点阵字体----------------------------------------------------
//功能: 安装gb2312 24点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_24_Fang(void)
{
    static struct FontObj font_gb2312_8x24_2424;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_24LoadFromFileFang(CFG_GB2312_24_FANG_FILENAME);  //加载字库
    font_gb2312_8x24_2424.LoadFont = __Gb2312_24LoadFromFileFang;
    font_gb2312_8x24_2424.UnloadFont = __Gb2312_24UnLoadFromFileFang;
#endif      //CFG_GB2312_24_FANG == from_array
    font_gb2312_8x24_2424.GetBitmap = __Font_Gb2312_24GetCharBitMapFang;
    font_gb2312_8x24_2424.GetCharWidth = __Font_Gb2312_24GetCharWidthFang;
    font_gb2312_8x24_2424.GetCharHeight = __Font_Gb2312_24GetCharHeightFang;

    if(Font_InstallFont(&font_gb2312_8x24_2424, CN_FONT_GB2312_FANG_24))
    {
        debug_printf("dot-gb2312","gb2312 24点阵仿宋体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 24点阵仿宋体安装失败\n\r");
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

//----点阵提取-----------------------------------------------------------------
//功能: 提取24点阵宋体汉字和ascii 8*24点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_24GetCharBitMapHei(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 24点阵字体----------------------------------------------------
//功能: 安装gb2312 24点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_24_Hei(void)
{
    static struct FontObj font_gb2312_8x24_2424;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_24LoadFromFileHei(CFG_GB2312_24_HEI_FILENAME);  //加载字库
    font_gb2312_8x24_2424.LoadFont = __Gb2312_24LoadFromFileHei;
    font_gb2312_8x24_2424.UnloadFont = __Gb2312_24UnLoadFromFileHei;
#endif      //CFG_GB2312_24_HEI == from_array
    font_gb2312_8x24_2424.GetBitmap = __Font_Gb2312_24GetCharBitMapHei;
    font_gb2312_8x24_2424.GetCharWidth = __Font_Gb2312_24GetCharWidthHei;
    font_gb2312_8x24_2424.GetCharHeight = __Font_Gb2312_24GetCharHeightHei;

    if(Font_InstallFont(&font_gb2312_8x24_2424, CN_FONT_GB2312_HEI_24))
    {
        debug_printf("dot-gb2312","gb2312 24点阵黑体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 24点阵黑体安装失败\n\r");
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

//----点阵提取-----------------------------------------------------------------
//功能: 提取24点阵宋体汉字和ascii 8*24点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_24GetCharBitMapKai(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 24点阵字体----------------------------------------------------
//功能: 安装gb2312 24点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_24_Kai(void)
{
    static struct FontObj font_gb2312_8x24_2424;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_24LoadFromFileKai(CFG_GB2312_24_KAI_FILENAME);  //加载字库
    font_gb2312_8x24_2424.LoadFont = __Gb2312_24LoadFromFileKai;
    font_gb2312_8x24_2424.UnloadFont = __Gb2312_24UnLoadFromFileKai;
#endif      //CFG_GB2312_24_KAI == from_array
    font_gb2312_8x24_2424.GetBitmap = __Font_Gb2312_24GetCharBitMapKai;
    font_gb2312_8x24_2424.GetCharWidth = __Font_Gb2312_24GetCharWidthKai;
    font_gb2312_8x24_2424.GetCharHeight = __Font_Gb2312_24GetCharHeightKai;

    if(Font_InstallFont(&font_gb2312_8x24_2424, CN_FONT_GB2312_KAI_24))
    {
        debug_printf("dot-gb2312","gb2312 24点阵楷体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 24点阵楷体安装失败\n\r");
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

//----点阵提取-----------------------------------------------------------------
//功能: 提取24点阵宋体汉字和ascii 8*24点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_24GetCharBitMapYuan(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 24点阵字体----------------------------------------------------
//功能: 安装gb2312 24点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_24_Yuan(void)
{
    static struct FontObj font_gb2312_8x24_2424;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_24LoadFromFileYuan(CFG_GB2312_24_YUAN_FILENAME);  //加载字库
    font_gb2312_8x24_2424.LoadFont = __Gb2312_24LoadFromFileYuan;
    font_gb2312_8x24_2424.UnloadFont = __Gb2312_24UnLoadFromFileYuan;
#endif      //CFG_GB2312_24_YUAN == from_array
    font_gb2312_8x24_2424.GetBitmap = __Font_Gb2312_24GetCharBitMapYuan;
    font_gb2312_8x24_2424.GetCharWidth = __Font_Gb2312_24GetCharWidthYuan;
    font_gb2312_8x24_2424.GetCharHeight = __Font_Gb2312_24GetCharHeightYuan;

    if(Font_InstallFont(&font_gb2312_8x24_2424, CN_FONT_GB2312_YUAN_24))
    {
        debug_printf("dot-gb2312","gb2312 24点阵圆体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 24点阵圆体安装失败\n\r");
        return 0;
    }
}

#endif      //CFG_GB2312_24_YUAN != zk_disable

#define FONT_HZ32_W             32              // Width
#define FONT_HZ32_H             32              // Height
#define FONT_HZ32_ASCII_W       16              // Width
#define FONT_HZ32_GLYPH_BYTES   128             // 字符字模的字节数
#define FONT_HZ32_ASCII_BYTES   64              // ASCII的字节数

#if CFG_GB2312_32_SONG != zk_disable
#include "../dotfont-ascii/dot-ascii16x32song.h"

bool_t __Gb2312_32LoadFromFileSong(const char* FileName);
void __Gb2312_32UnLoadFromFileSong(void);
bool_t __Font_Gb2312_32GetCharBitMapSong(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

u8 *pg_GB231232x32Song;
u8 *pg_Ascii16x32Song;

//----点阵提取-----------------------------------------------------------------
//功能: 提取32点阵宋体汉字和ascii 8*32点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_32GetCharBitMapSong(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 32点阵字体----------------------------------------------------
//功能: 安装gb2312 32点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_32_Song(void)
{
    static struct FontObj font_gb2312_8x32_3232;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_32LoadFromFileSong(CFG_GB2312_32_SONG_FILENAME);  //加载字库
    font_gb2312_8x32_3232.LoadFont = __Gb2312_32LoadFromFileSong;
    font_gb2312_8x32_3232.UnloadFont = __Gb2312_32UnLoadFromFileSong;
#endif      //CFG_GB2312_32_SONG == from_array
    font_gb2312_8x32_3232.GetBitmap = __Font_Gb2312_32GetCharBitMapSong;
    font_gb2312_8x32_3232.GetCharWidth = __Font_Gb2312_32GetCharWidthSong;
    font_gb2312_8x32_3232.GetCharHeight = __Font_Gb2312_32GetCharHeightSong;

    if(Font_InstallFont(&font_gb2312_8x32_3232, CN_FONT_GB2312_SONG_32))
    {
        debug_printf("dot-gb2312","gb2312 32点阵宋体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 32点阵宋体安装失败\n\r");
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


//----点阵提取-----------------------------------------------------------------
//功能: 提取32点阵宋体汉字和ascii 8*32点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_32GetCharBitMapFang(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 32点阵字体----------------------------------------------------
//功能: 安装gb2312 32点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_32_Fang(void)
{
    static struct FontObj font_gb2312_8x32_3232;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_32LoadFromFileFang(CFG_GB2312_32_FANG_FILENAME);  //加载字库
    font_gb2312_8x32_3232.LoadFont = __Gb2312_32LoadFromFileFang;
    font_gb2312_8x32_3232.UnloadFont = __Gb2312_32UnLoadFromFileFang;
#endif      //CFG_GB2312_32_FANG == from_array
    font_gb2312_8x32_3232.GetBitmap = __Font_Gb2312_32GetCharBitMapFang;
    font_gb2312_8x32_3232.GetCharWidth = __Font_Gb2312_32GetCharWidthFang;
    font_gb2312_8x32_3232.GetCharHeight = __Font_Gb2312_32GetCharHeightFang;

    if(Font_InstallFont(&font_gb2312_8x32_3232, CN_FONT_GB2312_FANG_32))
    {
        debug_printf("dot-gb2312","gb2312 32点阵仿宋体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 32点阵仿宋体安装失败\n\r");
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

//----点阵提取-----------------------------------------------------------------
//功能: 提取32点阵宋体汉字和ascii 8*32点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_32GetCharBitMapHei(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 32点阵字体----------------------------------------------------
//功能: 安装gb2312 32点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_32_Hei(void)
{
    static struct FontObj font_gb2312_8x32_3232;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_32LoadFromFileHei(CFG_GB2312_32_HEI_FILENAME);  //加载字库
    font_gb2312_8x32_3232.LoadFont = __Gb2312_32LoadFromFileHei;
    font_gb2312_8x32_3232.UnloadFont = __Gb2312_32UnLoadFromFileHei;
#endif      //CFG_GB2312_32_HEI == from_array
    font_gb2312_8x32_3232.GetBitmap = __Font_Gb2312_32GetCharBitMapHei;
    font_gb2312_8x32_3232.GetCharWidth = __Font_Gb2312_32GetCharWidthHei;
    font_gb2312_8x32_3232.GetCharHeight = __Font_Gb2312_32GetCharHeightHei;

    if(Font_InstallFont(&font_gb2312_8x32_3232, CN_FONT_GB2312_HEI_32))
    {
        debug_printf("dot-gb2312","gb2312 32点阵黑体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 32点阵黑体安装失败\n\r");
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

//----点阵提取-----------------------------------------------------------------
//功能: 提取32点阵宋体汉字和ascii 8*32点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_32GetCharBitMapKai(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 32点阵字体----------------------------------------------------
//功能: 安装gb2312 32点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_32_Kai(void)
{
    static struct FontObj font_gb2312_8x32_3232;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_32LoadFromFileKai(CFG_GB2312_32_KAI_FILENAME);  //加载字库
    font_gb2312_8x32_3232.LoadFont = __Gb2312_32LoadFromFileKai;
    font_gb2312_8x32_3232.UnloadFont = __Gb2312_32UnLoadFromFileKai;
#endif      //CFG_GB2312_32_KAI == from_array
    font_gb2312_8x32_3232.GetBitmap = __Font_Gb2312_32GetCharBitMapKai;
    font_gb2312_8x32_3232.GetCharWidth = __Font_Gb2312_32GetCharWidthKai;
    font_gb2312_8x32_3232.GetCharHeight = __Font_Gb2312_32GetCharHeightKai;

    if(Font_InstallFont(&font_gb2312_8x32_3232, CN_FONT_GB2312_KAI_32))
    {
        debug_printf("dot-gb2312","gb2312 32点阵楷体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 32点阵楷体安装失败\n\r");
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

//----点阵提取-----------------------------------------------------------------
//功能: 提取32点阵宋体汉字和ascii 8*32点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Gb2312_32GetCharBitMapYuan(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;
    // 得到字符字模在数组中的偏移量
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
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

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
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
    //从字库中加载汉字点阵
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

//----安装gb2312 32点阵字体----------------------------------------------------
//功能: 安装gb2312 32点阵字体,字模保存在文件中,文件名由参数传入。
//参数: 无
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontGb2312_32_Yuan(void)
{
    static struct FontObj font_gb2312_8x32_3232;

    s_ptGb2312Set = Charset_NlsSearchCharset(CN_NLS_CHARSET_GB2312);
    if (s_ptGb2312Set == NULL)
    {
        debug_printf("dot-gb2312","gb2312 字符集未安装\n\r");
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
    __Gb2312_32LoadFromFileYuan(CFG_GB2312_32_YUAN_FILENAME);  //加载字库
    font_gb2312_8x32_3232.LoadFont = __Gb2312_32LoadFromFileYuan;
    font_gb2312_8x32_3232.UnloadFont = __Gb2312_32UnLoadFromFileYuan;
#endif      //CFG_GB2312_32_YUAN == from_array
    font_gb2312_8x32_3232.GetBitmap = __Font_Gb2312_32GetCharBitMapYuan;
    font_gb2312_8x32_3232.GetCharWidth = __Font_Gb2312_32GetCharWidthYuan;
    font_gb2312_8x32_3232.GetCharHeight = __Font_Gb2312_32GetCharHeightYuan;

    if(Font_InstallFont(&font_gb2312_8x32_3232, CN_FONT_GB2312_YUAN_32))
    {
        debug_printf("dot-gb2312","gb2312 32点阵圆体安装完成\n\r");
        return 1;
    }else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-gb2312","gb2312 32点阵圆体安装失败\n\r");
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
