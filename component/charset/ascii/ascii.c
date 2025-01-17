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

//所属模块: ASCII字符编码驱动
//作者：mjh
//版本：V1.0.0
//文件描述: ASCII字符编码解析
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
#include "stddef.h"
#include "ascii.h"
#include "charset.h"
#include "gkernel.h"
#include "dbug.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_CharsetAscii(void);
//    ModuleInstall_CharsetAscii ( );
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"ascii charset"      //ascii字符集，纯英文版本才需要勾选
//parent:"Nls Charset"//填写该组件的父组件名字，none表示没有父组件
//attribute:system                    //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                    //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                      //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                    //初始化时机，可选值：early，medium，later, pre-main。
                                      //表示初始化时间，分别是早期、中期、后期
//dependence:"Nls Charset"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                      //选中该组件时，被依赖组件将强制选中，
                                      //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"               //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                      //选中该组件时，被依赖组件不会被强制选中，
                                      //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"gb2312 charset"              //该组件的互斥组件名（可以是none，表示无互斥组件），
                                      //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_ASCII_CHARSET == false )
//#warning  " ascii_charset  组件参数未配置，使用默认配置"
//%$#@target = header             //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_ASCII_CHARSET    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

s32 ASCII_MbToUcs4(u32* pwc, const char* s, s32 n);
s32 ASCII_MbsToUcs4s(u32* pwcs, const char* mbs, s32 n);
s32 ASCII_Ucs4ToMb(char* s, u32 wc);
s32 ASCII_Ucs4sToMbs(char* mbs, const u32* pwcs, s32 n);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// 注释参照 charset.h-> struct Charset -> GetOneMb
s32 ASCII_GetOneMb(const char* mbs,s32 n)
{
    if (mbs == NULL)
    {
        return 0;
    }
    else if(*(u8*)mbs >= (u8)0x80)
        return -1;
    else if(*mbs == 0)
        return 0;
    else
        return 1;
}

// 注释参照 charset.h-> struct Charset -> MbToUcs4
s32 ASCII_MbToUcs4(u32* pwc, const char* mbs,s32 n)
{
    u8 c;
    s32 result;
    if (mbs == NULL)
    {
        return 0;
    }
    c = *mbs;

    if(c == 0)
    {
        result = 0;
    }else if(c < (u8)0x80)
    {
        result = 1;
    }
    else
        result = -1;
    if(pwc != NULL)
        *pwc = c;
    return result;
}
#pragma GCC diagnostic pop

// 注释参照 charset.h-> struct Charset -> MbsToUcs4s
//todo：扩展ascii码怎么算?
s32 ASCII_MbsToUcs4s(u32* pwcs, const char* mbs, s32 n)
{
    s32 wcn,len;
    if(mbs == NULL)
        return 0;
    if(n != -1)
        len = n;
    else
        len = CN_LIMIT_SINT32;
    for(wcn = 0; wcn < len; wcn++)
    {
        if( (mbs[wcn] == 0) || ((u8)mbs[wcn] >= (u8)0x80) )
        {
            if(pwcs != NULL)
                pwcs[wcn] = (u32)0;
            return wcn;
        }
        else
        {
            if(pwcs != NULL)
                pwcs[wcn] = (u32)mbs[wcn];
        }
    }
    return wcn;
}

// 注释参照 charset.h-> struct Charset -> Ucs4ToMb
s32 ASCII_Ucs4ToMb(char* mbs, u32 wc)
{

    if(wc < 0x80){

        *mbs = wc;
        return 1;
    }

    return -1;
}

// 注释参照 charset.h-> struct Charset -> Ucs4sToMbs
s32 ASCII_Ucs4sToMbs(char* mbs, const u32* pwcs, s32 n)
{
    s32 asciin,len;
    if(pwcs == NULL)
        return 0;
    if(n != -1)
        len = n;
    else
        len = CN_LIMIT_SINT32;
    for(asciin = 0; asciin < len; asciin++)
    {
        if( (pwcs[asciin] == 0) || (pwcs[asciin] >= 0x80) )
        {
            if(mbs != NULL)
                mbs[asciin] = '\0';
            break;
        }
        else
        {
            if(mbs != NULL)
                mbs[asciin] = (u8)pwcs[asciin];
        }
    }
    return asciin;
}

//----安装ascii字符集----------------------------------------------------------
//功能: 安装ascii字符集，当系统使用西方字符界面时，使用这个字符集。特别注意，
//      gb2312已经包含了英文字符集，使用中文或中英文混合界面的，不需要安装ascii
//      字符集。但是，由于GB2312的字库只包含了全角的英文字符，故还需要安装ascii
//      的字体资源，尺寸(8*8、8*16)可选。
//参数: 无意义
//返回: 1=成功，0=失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_CharsetAscii(void)
{
    static struct Charset encoding;

    encoding.max_len = 1;
    encoding.EOC_Size = 1;
    encoding.GetOneMb = ASCII_GetOneMb;
    encoding.MbToUcs4 = ASCII_MbToUcs4;
    encoding.Ucs4ToMb = ASCII_Ucs4ToMb;
    encoding.MbsToUcs4s = ASCII_MbsToUcs4s;
    encoding.Ucs4sToMbs = ASCII_Ucs4sToMbs;
    if( Charset_NlsInstallCharset(&encoding, CN_NLS_CHARSET_ASCII))
    {
        debug_printf("ascii","ASCII encoding install sucess\r\n");
        return true;
    }else
    {
        DJY_SaveLastError(EN_GK_CHARSET_INSTALL_ERROR);
        debug_printf("ascii","ASCII encoding install fail\n\r");
        return false;
    }
}

