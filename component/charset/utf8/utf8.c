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

//所属模块: UTF-8字符编码驱动
//作者：mjh
//版本：V1.0.0
//文件描述: UTF-8字符编码解析
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:2011-05-17
//       作者:mjh
//       新版本号：
//       修改说明:初始版本
//------------------------------------------------------
/* ---------------------------------------------------------------------
    Legal UTF-8 sequences are:

    1st---- 2nd---- 3rd---- 4th---- 5th--- 6th---   Codepoints---

    00-7F                                           0x00000000-   0x0000007F
    C2-DF   80-BF                                   0x00000080-   0x000007FF
    E0      A0-BF   80-BF                           0x00000800-   0x00000FFF
    E1-EC   80-BF   80-BF                           0x00001000-   0x0000CFFF
    ED      80-9F   80-BF                           0x0000D000-   0x0000D7FF
    EE-EF   80-BF   80-BF                           0x0000E000-   0x0000FFFF
    F0      90-BF   80-BF   80-BF                   0x00010000-   0x0003FFFF
    F1-F7   80-BF   80-BF   80-BF                   0x00040000-   0x001FFFFF
    F8      88-BF   80-BF   80-BF  80-BF            0x00200000-   0x00FFFFFF
    F9-FB   80-BF   80-BF   80-BF  80-BF            0x01000000-   0x03FFFFFF
    FC      84-BF   80-BF   80-BF  80-BF   80-BF    0x04000000-   0x3FFFFFFF
    FD      80-BF   80-BF   80-BF  80-BF   80-BF    0x40000000-   0x7FFFFFFF
-------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------
Rule
 U-00000000 - U-0000007F: 0xxxxxxx
 U-00000080 - U-000007FF: 110xxxxx 10xxxxxx
 U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
 U-00010000 - U-001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
---------------------------------------------------------------------------*/
#include "stdint.h"
#include "stdio.h"
#include "gkernel.h"
#include "charset.h"
#include "ascii.h"
#include "dbug.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//   extern bool_t ModuleInstall_CharsetUtf8(void);
//   ModuleInstall_CharsetUtf8 ( );
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"utf8 charset"//utf8字符集
//parent:"Nls Charset"//填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"Nls Charset"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_UTF8_CHARSET == false )
//#warning  " utf8_charset  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_UTF8_CHARSET    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
s32 Utf8MbToUcs4(u32* pwc, const char* s, s32 n);
s32 Utf8MbsToUcs4s(u32* pwcs, const char* mbs, s32 n);
s32 Utf8Ucs4ToMb(char* s, u32 wc);
s32 Utf8Ucs4sToMbs(char* mbs, const u32* pwcs, s32 n);

//计算给定首字节的UTF-8编码字符字节数
#define utf8_len(c)\
    (((u8)c < 0x80) ? 1 : \
        (((u8)c < 0xc2) ? 0 : \
            (((u8)c < 0xe0) ? 2 : \
                (((u8)c < 0xf0) ? 3 : \
                    (((u8)c < 0xf8) ? 4 : \
                        (((u8)c < 0xfc) ? 5 : \
                            (((u8)c < 0xfe) ? 6 : 0)\
                        )\
                    )\
                )\
            )\
        )\
    )

//--------UTF-8字节序列合法性判断----------------------------------------
//功能: 判断给定的UTF-8字节序列是否合法
//参数: src 字节序列指针
//      len 字节序列的长度
//返回: 0, 非法UTF-8字符
//      合法字符返回第一个UTF-8字符的字节数
//-----------------------------------------------------------------
static u8 Utf8IsLegal(const char* src, s32 len)
{
    u8 a;
    const char *trail;
    s32 length;

    if(len <= 0 /*|| src == NULL*/)
        goto __illegal;

    // 计算合法UTF-8字符的字节数
    length = utf8_len(*src);

    // 判断长度
    if(length > len)
        goto __illegal;

    // 指向最后一个字节的后一个字节
    trail = (src + length);

    // 判断编码是否正确
    switch(length){
    default: goto __illegal;
        case 6: if ((a = (*--trail)) < 0x80 || a > 0xBF) goto __illegal;
        case 5: if ((a = (*--trail)) < 0x80 || a > 0xBF) goto __illegal;
        case 4: if ((a = (*--trail)) < 0x80 || a > 0xBF) goto __illegal;
        case 3: if ((a = (*--trail)) < 0x80 || a > 0xBF) goto __illegal;
        case 2: if ((a = (*--trail)) > 0xBF) goto __illegal;

        // 判断第二个字节(必定在0x80 - 0xBF之间)
        switch(*src){
            case (char)0xE0: if (a < 0xA0) goto __illegal; break;
            case (char)0xED: if (a > 0x9F) goto __illegal; break;
            case (char)0xF0: if (a < 0x90) goto __illegal; break;
            case (char)0xF8: if (a < 0x88) goto __illegal; break;
            case (char)0xFC: if (a < 0x84) goto __illegal; break;
            default:   if (a < 0x80) goto __illegal;
        }

        // 判断多字节UTF-8字符的第一个字节的范围
        case 1: if (*src >= 0x80 && *src < 0xC0) goto __illegal;
    }

    // 判断第一个字节
    if (*src > 0xFD) goto __illegal;

    return length;

__illegal:
    return 0;
}

// 注释参照 charset.h-> struct Charset -> GetOneMb
s32 Utf8GetOneMb(const char* mbs,s32 n)
{
    s32 count;

    if(mbs == NULL)
        return -1;
    if(*mbs == 0)
        return 1;       //串结束符也是合法字符
    count = Utf8IsLegal(mbs, n);
    if(count == 0)
        return -1;
    else
        return count;

}

// 注释参照 charset.h-> struct Charset -> MbToUcs4
s32 Utf8MbToUcs4(u32* pwc, const char* mbs, s32 n)
{
    s32 count, len;
    u32 wc;

    if(mbs == NULL)
        return -1;
    if(!(count = Utf8IsLegal(mbs, n)))
        goto __illegal;

    if(count == 1)
        wc = (u32)*mbs;
    else{ // 2,3,4

        // 见文件头部UTF-8规则
        len = count;
        wc = (u32)*mbs;
        wc &= ((1 << (8-len-1)) - 1);
        len--;

        while(len-- > 0){
            wc = (wc << 6) | (*++mbs & 0x3f);
        }

        // FIXME 是否需要做UCS-4合法性判断?
    }

    if(pwc != NULL)
        *pwc = wc;

    return count;

__illegal:
    return -1;
}

// 注释参照 charset.h-> struct Charset -> MbsToUcs4s
s32 Utf8MbsToUcs4s(u32* pwcs, const char* mbs, s32 n)
{
    return 0;
}

// 注释参照 charset.h-> struct Charset -> Ucs4ToMb
s32 Utf8Ucs4ToMb(char* mbs, u32 wc)
{
    s32 count;

    // 计算wc对应的UTF-8字符的字节数
    if(wc < 0x00000080)
        count = 1;
    else if(wc < 0x00000800)
        count = 2;
    else if(wc < 0x00010000)
        count = 3;
    else if(wc < 0x00200000)
        count = 4;
    else if(wc < 0x04000000)
        count = 5;
    else if(wc < 0x80000000)
        count = 6;
    else
        goto __illegal;

    switch(count){
        case 6: mbs[5] = 0x80 | (wc & 0x3f); wc = wc >> 6; wc |= 0x4000000;
        case 5: mbs[4] = 0x80 | (wc & 0x3f); wc = wc >> 6; wc |= 0x200000;
        case 4: mbs[3] = 0x80 | (wc & 0x3f); wc = wc >> 6; wc |= 0x10000;
        case 3: mbs[2] = 0x80 | (wc & 0x3f); wc = wc >> 6; wc |= 0x800;
        case 2: mbs[1] = 0x80 | (wc & 0x3f); wc = wc >> 6; wc |= 0xc0;
        case 1: mbs[0] = wc;
    }

    return count;

__illegal:
        return -1;
}

// 注释参照 charset.h-> struct Charset -> Ucs4sToMbs
s32 Utf8Ucs4sToMbs(char* mbs, const u32* pwcs, s32 n)
{
    return 0;
}

//----安装utf8字符编码解析器---------------------------------------------------
//功能: 安装utf8字符编码解析器，执行ucs4和编码和utf8编码之间转换。
//参数: 无意义
//返回：1=成功，0=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_CharsetUtf8(void)
{
    static struct Charset encoding;

    encoding.max_len = 6;
    encoding.EOC_Size = 1;
    encoding.GetOneMb = Utf8GetOneMb;
    encoding.MbToUcs4 = Utf8MbToUcs4;
    encoding.Ucs4ToMb = Utf8Ucs4ToMb;
    encoding.MbsToUcs4s = Utf8MbsToUcs4s;
    encoding.Ucs4sToMbs = Utf8Ucs4sToMbs;
    if( Charset_NlsInstallCharset(&encoding, CN_NLS_CHARSET_UTF8))
    {
        debug_printf("utf8","utf8 encoding install sucess\n\r");
        return true;
    }else
    {
        Djy_SaveLastError(EN_GK_CHARSET_INSTALL_ERROR);
        debug_printf("utf8","utf8 encoding install fail\n\r");
        return false;
    }
}

