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
//    without specific prior written permiss ion.

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

//所属模块: ASCII 8x8点阵字体驱动
//作者：mjh
//版本：V1.0.0
//文件描述: ASCII 8x8点阵字体解析
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:2011-05-17
//       作者:mjh
//       新版本号：
//       修改说明:初始版本
//------------------------------------------------------
#include "stdint.h"
#include "stdio.h"
#include "gkernel.h"
#include "dbug.h"
#include "./font/font.h"
#include "./font/font_ascii.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern void ModuleInstall_FontAscii(void);
//  ModuleInstall_FontAscii();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"ascii dot font"//ascii点阵字体，纯英文版本才需要勾选
//parent:"font"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"font","ascii charset"  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"gb2312 dot"            //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_ASCII_DOT_FONT == false )
//#warning  " ascii_dot_font  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_ASCII_DOT_FONT    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
#define CFG_ASCII_8X8         false         //"ASCII_8×8",8*8点阵ascii字体
#define CFG_ASCII_6X12        false         //"ASCII_6×12",6*12点阵ascii字体
#define CFG_ASCII_8X16_SONG   true          //"ASCII_8×16宋体",8*16点阵ascii字体_宋体
#define CFG_ASCII_8X16_YUAN   false         //"ASCII_8×16圆体",8*16点阵ascii字体_圆体
#define CFG_ASCII_8X16_KAI    false         //"ASCII_8×16楷体",8*16点阵ascii字体_楷体
#define CFG_ASCII_8X16_HEI    false         //"ASCII_8×16黑体",8*16点阵ascii字体_黑体
#define CFG_ASCII_8X16_FANG   false         //"ASCII_8×16仿宋",8*16点阵ascii字体_仿宋
#define CFG_ASCII_12X24_SONG  false         //"ASCII_12×24宋体",12*24点阵ascii字体_宋体
#define CFG_ASCII_12X24_YUAN  false         //"ASCII_12×24圆体",12*24点阵ascii字体_圆体
#define CFG_ASCII_12X24_KAI   false         //"ASCII_12×24楷体",12*24点阵ascii字体_楷体
#define CFG_ASCII_12X24_HEI   false         //"ASCII_12×24黑体",12*24点阵ascii字体_黑体
#define CFG_ASCII_12X24_FANG  false         //"ASCII_12×24仿宋",12*24点阵ascii字体_仿宋
#define CFG_ASCII_16X32_SONG  false         //"ASCII_16X32宋体",12*24点阵ascii字体_宋体
#define CFG_ASCII_16X32_YUAN  false         //"ASCII_16×32圆体",16*32点阵ascii字体_圆体
#define CFG_ASCII_16X32_KAI   false         //"ASCII_16×32楷体",16*32点阵ascii字体_楷体
#define CFG_ASCII_16X32_HEI   false         //"ASCII_16×32黑体",16*32点阵ascii字体_黑体
#define CFG_ASCII_16X32_FANG  false         //"ASCII_16×32仿宋",16*32点阵ascii字体_仿宋
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#if CFG_ASCII_8X8 == true
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

#define ASCII8x8_DEFAULT_CHAR_CODE 0x00

//----点阵提取-----------------------------------------------------------------
//功能: 提取ascii 8*8点阵字体，如果charcode超出0~0xff的范围，显示编码为0x00的字
//      符，ascii包含扩展的ascii码
//参数: charcode，待显示的ascii码得ucs4编码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Ascii8x8GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u32 offset;
    bool_t result = true;


    // 得到字符字模在数组中的偏移量
    if(charcode > 0xff)
    {
        offset = ASCII8x8_DEFAULT_CHAR_CODE;
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
//-----------------------------------------------------------------------------
s32 __Font_Ascii8x8GetCharWidth(u32 CharCode)
{
    return 8;
}

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
//-----------------------------------------------------------------------------
s32 __Font_Ascii8x8GetCharHeight(u32 CharCode)
{
    return 8;
}

//----安装ascii8x8字体-------------------------------------------------------
//功能: 安装ascii字体，当系统使用西方字符界面时，使用这个字符集。特别注意，
//      gb2312已经包含了英文字体，在使用中文的界面中可以不安装ascii字体。
//参数: 无
//返回: true=成功，false=失败
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
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-ascii","ascii 8x8 font install fail\n\r");
        return false;
    }
}
#endif      // CFG_ASCII_8X8 == true
#if CFG_ASCII_6X12 == true

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

#define ASCII6x12_DEFAULT_CHAR_CODE 0x00

//----点阵提取-----------------------------------------------------------------
//功能: 提取ascii 8*16点阵字体，如果charcode超出0~0xff的范围，显示编码为0x00的字
//      符，ascii包含扩展的ascii码
//参数: charcode，待显示的ascii码得ucs4编码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Ascii6x12GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u32 offset;
    bool_t result = true;


    // 得到字符字模在数组中的偏移量
    if(charcode > 0xff)
    {
        offset = ASCII6x12_DEFAULT_CHAR_CODE;
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
//-----------------------------------------------------------------------------
s32 __Font_Ascii6x12GetCharWidth(u32 CharCode)
{
    return 6;
}

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
//-----------------------------------------------------------------------------
s32 __Font_Ascii6x12GetCharHeight(u32 CharCode)
{
    return 12;
}

//----安装ascii6x12字体--------------------------------------------------------
//功能: 安装ascii字体，当系统使用西方字符界面时，使用这个字符集。特别注意，
//      gb2312已经包含了英文字体，在使用中文的界面中可以不安装ascii字体。
//参数: 无
//返回: true=成功，false=失败
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
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-ascii","ascii 6x12 font install fail\n\r");
        return false;
    }
}

#endif      //CFG_ASCII_6X12 == true

#if CFG_ASCII_8X16_SONG == true

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

#define ASCII8x16_DEFAULT_CHAR_CODE 0x00

//----点阵提取-----------------------------------------------------------------
//功能: 提取ascii 8*16点阵字体，如果charcode超出0~0xff的范围，显示编码为0x00的字
//      符，ascii包含扩展的ascii码
//参数: charcode，待显示的ascii码得ucs4编码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Ascii8x16GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u32 offset;
    bool_t result = true;


    // 得到字符字模在数组中的偏移量
    if(charcode > 0xff)
    {
        offset = ASCII8x16_DEFAULT_CHAR_CODE;
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

//----获取字符宽度-------------------------------------------------------------
//功能: 取某字符的点阵宽度
//参数: CharCode,被查询的字符
//返回: 字符宽度
//-----------------------------------------------------------------------------
s32 __Font_Ascii8x16GetCharWidth(u32 CharCode)
{
    return 6;
}

//----获取字符高度-------------------------------------------------------------
//功能: 取某字符的点阵高度
//参数: CharCode,被查询的字符
//返回: 字符高度
//-----------------------------------------------------------------------------
s32 __Font_Ascii8x16GetCharHeight(u32 CharCode)
{
    return 16;
}

//----安装ascii8x16字体--------------------------------------------------------
//功能: 安装ascii字体，当系统使用西方字符界面时，使用这个字符集。特别注意，
//      gb2312已经包含了英文字体，在使用中文的界面中可以不安装ascii字体。
//参数: 无
//返回: true=成功，false=失败
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
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-ascii","ascii 8x16 font install fail\n\r");
        return false;
    }
}

#endif      //CFG_ASCII_8X16_SONG == true


#if CFG_ASCII_8X16_YUAN == true        //圆体

#include "dot-ascii8x16yuan.h"

#endif      //CFG_ASCII_8X16_YUAN == true

#if CFG_ASCII_8X16_KAI == true         //楷体

#include "dot-ascii8x16kai.h"

#endif      //CFG_ASCII_8X16_KAI == true

#if CFG_ASCII_8X16_HEI == true         //黑体

#include "dot-ascii8x16hei.h"

#endif      //CFG_ASCII_8X16_HEI == true

#if CFG_ASCII_8X16_FANG == true         //仿宋

#include "dot-ascii8x16fang.h"

#endif      //CFG_ASCII_8X16_FANG == true


#if CFG_ASCII_12X24_SONG == true        //宋体

#include "dot-ascii12x24song.h"

bool_t __Font_Ascii12x24GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

#define ASCII12x24_DEFAULT_CHAR_CODE 0x00

//----点阵提取-----------------------------------------------------------------
//功能: 提取ascii 12*24点阵字体，如果charcode超出0~0xff的范围，显示编码为0x00的字
//      符，ascii包含扩展的ascii码
//参数: charcode，待显示的ascii码得ucs4编码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Ascii12x24GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u32 offset;
    bool_t result = true;

    if(charcode > 0xff)
    {
        offset = ASCII12x24_DEFAULT_CHAR_CODE;
        result = false;
    }
    else
        offset = charcode*48;
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    bitmap->width = 12;
    bitmap->height = 24;
    bitmap->linebytes = 2;

    if(bitmap->bm_bits != NULL)//画点阵的时候会画出16x24的图，但是下一个字符会在宽度为12的位置开始覆盖之后的点
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

//----安装ascii12x24字体--------------------------------------------------------
//功能: 安装ascii字体，当系统使用西方字符界面时，使用这个字符集。特别注意，
//      gb2312已经包含了英文字体，在使用中文的界面中可以不安装ascii字体。
//参数: 无意义
//返回: true=成功，false=失败
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
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-ascii","ascii 12x24 font install fail\n\r");
        return false;
    }
}

#endif      //CFG_ASCII_12X24_SONG == true

#if CFG_ASCII_12X24_YUAN == true        //圆体

#include "dot-ascii12x24yuan.h"

#endif      //CFG_ASCII_12X24_YUAN == true

#if CFG_ASCII_12X24_KAI == true         //楷体

#include "dot-ascii12x24kai.h"

#endif      //CFG_ASCII_12X24_KAI == true

#if CFG_ASCII_12X24_HEI == true         //黑体

#include "dot-ascii12x24hei.h"

bool_t __Font_Ascii12x24heiGetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

#define ASCII12x24HEI_DEFAULT_CHAR_CODE 0x00

//----点阵提取-----------------------------------------------------------------
//功能: 提取ascii 12*24点阵字体，如果charcode超出0~0xff的范围，显示编码为0x00的字
//      符，ascii包含扩展的ascii码
//参数: charcode，待显示的ascii码得ucs4编码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Ascii12x24heiGetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u32 offset;
    bool_t result = true;

    if(charcode > 0xff)
    {
        offset = ASCII12x24HEI_DEFAULT_CHAR_CODE;
        result = false;
    }
    else
        offset = charcode*48;
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    bitmap->width = 12;
    bitmap->height = 24;
    bitmap->linebytes = 2;

    if(bitmap->bm_bits != NULL)//画点阵的时候会画出16x24的图，但是下一个字符会在宽度为12的位置开始覆盖之后的点
    {
        for(i=0; i<48; i++)
        {
            bitmap->bm_bits[i] = cs_ascii_hei_12x24[offset + i];
        }
    }
    return result;
}

s32 __Font_Ascii12x24heiGetCharWidth(u32 CharCode)
{
    return 12;
}

s32 __Font_Ascii12x24heiGetCharHeight(u32 CharCode)
{
    return 24;
}

//----安装ascii12x24黑体字体----------------------------------------------------
//功能: 安装ascii字体，当系统使用西方字符界面时，使用这个字符集。特别注意，
//      gb2312已经包含了英文字体，在使用中文的界面中可以不安装ascii字体。
//参数: 无意义
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_FontAscii12x24hei(void)
{
    static struct FontObj font_a12x24_hei;

    font_a12x24_hei.MaxWidth = 12;
    font_a12x24_hei.MaxHeight = 24;
    font_a12x24_hei.Attr = CN_FONT_TYPE_DOT;
    font_a12x24_hei.LoadFont = NULL;
    font_a12x24_hei.UnloadFont = NULL;
    font_a12x24_hei.GetBitmap = __Font_Ascii12x24heiGetCharBitMap;
    font_a12x24_hei.GetCharWidth = __Font_Ascii12x24heiGetCharWidth;
    font_a12x24_hei.GetCharHeight = __Font_Ascii12x24heiGetCharHeight;

    if(Font_InstallFont(&font_a12x24_hei, CN_FONT_ASCII_12x24hei))
    {
        debug_printf("dot-ascii","ascii 12x24 hei font install sucess\n\r");
        return true;
    }
    else
    {
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-ascii","ascii 12x24 hei font install fail\n\r");
        return false;
    }
}

#endif      //CFG_ASCII_12X24_HEI == true

#if CFG_ASCII_12X24_FANG == true         //仿宋

#include "dot-ascii12x24fang.h"

#endif      //CFG_ASCII_12X24_FANG == true


#if CFG_ASCII_16X32_SONG == true        //宋体

#include "dot-ascii16x32song.h"

bool_t __Font_Ascii16x32GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap);

#define ASCII16x32_DEFAULT_CHAR_CODE 0x00

//----点阵提取-----------------------------------------------------------------
//功能: 提取ascii 12*24点阵字体，如果charcode超出0~0xff的范围，显示编码为0x00的字
//      符，ascii包含扩展的ascii码
//参数: charcode，待显示的ascii码得ucs4编码
//      size，字号，本函数不用
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __Font_Ascii16x32GetCharBitMap(u32 charcode, u32 size,u32 resv,
                                    struct RectBitmap *bitmap)
{
    u8 i;
    u32 offset;
    bool_t result = true;

    if(charcode > 0xff)
    {
        offset = ASCII16x32_DEFAULT_CHAR_CODE;
        result = false;
    }
    else
        offset = charcode*48;
    bitmap->PixelFormat = CN_SYS_PF_GRAY1;
    bitmap->width = 12;
    bitmap->height = 24;
    bitmap->linebytes = 2;

    if(bitmap->bm_bits != NULL)//画点阵的时候会画出16x24的图，但是下一个字符会在宽度为12的位置开始覆盖之后的点
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

//----安装ascii16x32字体--------------------------------------------------------
//功能: 安装ascii字体，当系统使用西方字符界面时，使用这个字符集。特别注意，
//      gb2312已经包含了英文字体，在使用中文的界面中可以不安装ascii字体。
//参数: 无意义
//返回: true=成功，false=失败
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
        DJY_SaveLastError(EN_GK_FONT_INSTALL_ERROR);
        debug_printf("dot-ascii","ascii 16x32 font install fail\n\r");
        return false;
    }
}

#endif      //CFG_ASCII_16X32_SONG == true

#if CFG_ASCII_16X32_YUAN == true        //圆体

#include "dot-ascii16x32yuan.h"

#endif      //CFG_ASCII_16X32_YUAN == true

#if CFG_ASCII_16X32_KAI == true         //楷体

#include "dot-ascii16x32kai.h"

#endif      //CFG_ASCII_16X32_KAI == true

#if CFG_ASCII_16X32_HEI == true         //黑体

#include "dot-ascii16x32hei.h"

#endif      //CFG_ASCII_16X32_HEI == true

#if CFG_ASCII_16X32_FANG == true         //仿宋

#include "dot-ascii16x32fang.h"

#endif      //CFG_ASCII_16X32_FANG == true

#pragma GCC diagnostic pop

void ModuleInstall_FontAscii(void)
{
#if CFG_ASCII_8X8 == true
    extern bool_t ModuleInstall_FontAscii8x8(void);
    ModuleInstall_FontAscii8x8( );
#endif      //CFG_ASCII_8X8 == true
#if CFG_ASCII_6X12 == true
    extern bool_t ModuleInstall_FontAscii6x12(void);
    ModuleInstall_FontAscii6x12( );
#endif      //CFG_ASCII_6X12 == true
#if CFG_ASCII_8X16_SONG == true
    extern bool_t ModuleInstall_FontAscii8x16(void);
    ModuleInstall_FontAscii8x16( );
#endif      //CFG_ASCII_8X16_SONG == true
#if CFG_ASCII_8X16_YUAN == true
    extern bool_t ModuleInstall_FontAscii8x16yuan(void);
    ModuleInstall_FontAscii8x16yuan( );
#endif      //CFG_ASCII_8X16_YUAN == true
#if CFG_ASCII_8X16_KAI == true
    extern bool_t ModuleInstall_FontAscii8x16kai(void);
    ModuleInstall_FontAscii8x16kai( );
#endif      //CFG_ASCII_8X16_KAI == true
#if CFG_ASCII_8X16_HEI == true
    extern bool_t ModuleInstall_FontAscii8x16hei(void);
    ModuleInstall_FontAscii8x16hei( );
#endif      //CFG_ASCII_8X16_HEI == true
#if CFG_ASCII_8X16_FANG == true
    extern bool_t ModuleInstall_FontAscii8x16fang(void);
    ModuleInstall_FontAscii8x16fang( );
#endif      //CFG_ASCII_8X16_FANG == true
#if CFG_ASCII_12X24_SONG == true
    extern bool_t ModuleInstall_FontAscii12x24(void);
    ModuleInstall_FontAscii12x24( );
#endif      //CFG_ASCII_12X24_SONG == true
#if CFG_ASCII_12X24_YUAN == true
    extern bool_t ModuleInstall_FontAscii12x24yuan(void);
    ModuleInstall_FontAscii12x24yuan( );
#endif      //CFG_ASCII_12X24_YUAN == true
#if CFG_ASCII_12X24_KAI == true
    extern bool_t ModuleInstall_FontAscii12x24kai(void);
    ModuleInstall_FontAscii12x24kai( );
#endif      //CFG_ASCII_12X24_KAI == true
#if CFG_ASCII_12X24_HEI == true
    extern bool_t ModuleInstall_FontAscii12x24hei(void);
    ModuleInstall_FontAscii12x24hei( );
#endif      //CFG_ASCII_12X24_HEI == true
#if CFG_ASCII_12X24_FANG == true
    extern bool_t ModuleInstall_FontAscii12x24fang(void);
    ModuleInstall_FontAscii12x24fang( );
#endif      //CFG_ASCII_12X24_FANG == true
#if CFG_ASCII_16X32_SONG == true
    extern bool_t ModuleInstall_FontAscii16x32(void);
    ModuleInstall_FontAscii16x32( );
#endif      //CFG_ASCII_16X32_SONG == true
#if CFG_ASCII_16X32_YUAN == true
    extern bool_t ModuleInstall_FontAscii16x32yuan(void);
    ModuleInstall_FontAscii16x32yuan( );
#endif      //CFG_ASCII_16X32_YUAN == true
#if CFG_ASCII_16X32_KAI == true
    extern bool_t ModuleInstall_FontAscii16x32kai(void);
    ModuleInstall_FontAscii16x32kai( );
#endif      //CFG_ASCII_16X32_KAI == true
#if CFG_ASCII_16X32_HEI == true
    extern bool_t ModuleInstall_FontAscii16x32hei(void);
    ModuleInstall_FontAscii16x32hei( );
#endif      //CFG_ASCII_16X32_HEI == true
#if CFG_ASCII_16X32_FANG == true
    extern bool_t ModuleInstall_FontAscii16x32fang(void);
    ModuleInstall_FontAscii16x32fang( );
#endif      //CFG_ASCII_16X32_FANG == true


}

