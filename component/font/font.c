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

//所属模块: 字体驱动
//作者：lst
//版本：V1.0.0
//文件描述: 字体资源初始化
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:2009-11-21
//       作者:lst
//       新版本号：
//       修改说明:初始版本
//------------------------------------------------------
#include "stdint.h"
#include "stdio.h"
#include "object.h"
#include "dbug.h"
#include "../include/font/font.h"
#include "../include/font/font_ascii.h"
#include "../include/font/font_gb2312.h"
#include "../include/font/gb2312.h"
#include "string.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_Font(void);
//    ModuleInstall_Font ( );
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"font"//字体驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_FONT == false )
//#warning  " font  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_FONT    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,128,
#define CFG_FONT_DEFAULT  "gb2312_song_16"      //"默认字体",字体名在include/font目录中找
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

static struct FontObj *s_ptCurFont;
struct Object *pFontRoot;
static bool_t g_bUserSetFont = false;

//----获取字体资源-------------------------------------------------------------
//功能: 根据字体名称获取字体资源，"C"是默认字体的代号。
//参数: 字体名称
//返回: 匹配的字体，若未有匹配编码，使用默认的ASCII编码
//-----------------------------------------------------------------------------
struct FontObj* Font_GetLocFont(const char* font_name)
{
    struct FontObj* font;

    if(strcmp(font_name, "C") == 0){

        font = Font_SearchFont(CFG_FONT_DEFAULT);

    }else
    {
        font = Font_SearchFont(font_name);
    }

    return font;
}

//----安装字体-----------------------------------------------------------------
//功能: 把新字体安装到系统资源队列中
//参数: font，新增的字体资源指针
//      name，新增字体名
//返回: true
//-----------------------------------------------------------------------------
bool_t Font_InstallFont(struct FontObj *font,const char* name)
{
    if(!pFontRoot)
        return (FALSE);       //字体资源根结点未创建

    font->HostObj = OBJ_NewChild(pFontRoot, (fnObjOps)-1, font, name);
    if(!font->HostObj)
        return (FALSE);

    if(!s_ptCurFont)
        s_ptCurFont = font;
    {
        //当前字体如果是调用API设置的，则维持，否则，如果新安装的字符集是默认字体，
        //则更新当前字体
        if( ! g_bUserSetFont)
        {
            if(strcmp(name,CFG_FONT_DEFAULT) == 0)
                s_ptCurFont = font;
        }
    }

    return (TRUE);
}

//----字体驱动模块初始化-------------------------------------------------------
//功能: 初始化字体驱动模块
//参数: para: 无效
//返回: true = 成功，false = 失败
//-----------------------------------------------------------------------------
bool_t ModuleInstall_Font(void)
{
    pFontRoot = OBJ_NewChild(OBJ_GetRoot(), NULL, 0, "font");
    if(pFontRoot)
    {
        info_printf("module","font install \r\n");
        return true;
    }else
    {
        error_printf("module","font install failed\r\n");
        return false;
    }
}

//----获取当前字体---------------------------------------------------------
//功能: 获取当前使用的字体
//返回: 当前字体
//-----------------------------------------------------------------------------
struct FontObj* Font_GetCurFont(void)
{
    return s_ptCurFont;
}

//----设定当前字体---------------------------------------------------------
//功能: 把新字体设为当前使用的字体，新字体必须事先安装到系统中，如果
//      字体树资源节点未创建，本函数什么都不做，返回原来的当前字体。
//参数: font，指定的字体。
//返回: NULL，设定失败
//      设定之前的字体。
//-----------------------------------------------------------------------------
struct FontObj* Font_SetCurFont(struct FontObj* font)
{
    struct Object *rsc;
    char *Name;
    if(font == NULL)
        return NULL;

    rsc = OBJ_SearchChild(OBJ_GetRoot(), CN_FONT_RSC_TREE);
    if(rsc == NULL)
        return NULL;       //字体资源树未创建
    Name = OBJ_GetName(font->HostObj);
    rsc = OBJ_SearchChild(rsc,(const char*)Name);
    if(rsc != NULL)
    {
        s_ptCurFont = (struct FontObj*)OBJ_GetPrivate(rsc);
        g_bUserSetFont = true;
    }
    return s_ptCurFont;
}

//----搜索字体资源---------------------------------------------------------
//功能: 根据名称搜索字体资源
//参数: name，指定的字体名称
//返回: NULL，无此字体资源
//      要找的字体资源
//-----------------------------------------------------------------------------
struct FontObj* Font_SearchFont(const char* name)
{
    struct Object *rsc;
    rsc = OBJ_SearchChild(OBJ_GetRoot(), CN_FONT_RSC_TREE);
    if(rsc == NULL)
        return NULL;       //字体资源树未创建

    rsc = OBJ_SearchChild(rsc, name);
    return (struct FontObj*)OBJ_GetPrivate(rsc);
}

//----取字体点阵行高-----------------------------------------------------------
//功能: 获取字体中字符的点阵行高,即该字体中最高的那个字符的高度.
//参数: font, 被查询的字体.
//返回: 该字库最高的那个字符的高度(像素值)
//-----------------------------------------------------------------------------
s32 Font_GetFontLineHeight(struct FontObj* font)
{
    if(font == NULL)
        return 0;
    return font->MaxHeight;
}

//----取字体点阵竖行宽---------------------------------------------------------
//功能: 获取字体中字符的点阵竖行宽度,即该字体中最宽的那个字符的宽度.
//参数: font, 被查询的字体.
//返回: 该字库最宽的那个字符的宽度(像素值)
//-----------------------------------------------------------------------------
s32 Font_GetFontLineWidth(struct FontObj* font)
{
    if(font == NULL)
        return 0;
    return font->MaxWidth;
}

//----取字体属性字-------------------------------------------------------------
//功能: 获取字体的属性字
//参数: font, 被查询的字体.
//返回: 字体属性字,font.c模块并不解析该属性字
//-----------------------------------------------------------------------------
s32 Font_GetFontAttr(struct FontObj* font)
{
    if(font == NULL)
        return 0;
    return font->Attr;
}
