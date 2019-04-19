//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.DGTD

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

//所属模块: 字符集驱动
//作者：lst
//版本：V1.0.0
//文件描述: 字符集资源初始化
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:2011-05-17
//       作者:lst
//       新版本号：
//       修改说明:初始版本
//------------------------------------------------------
#include "stdint.h"
#include "object.h"
#include "stdio.h"
#include "dbug.h"
#include "charset.h"
#include "component_config_NlsCharset.h"

static struct Charset *s_ptCurCharset;      // 当前字符集
static struct Object *s_ptCharsetDir;       // 字符集目录
static bool_t g_bUserSetCharset = false;
//----安装字符集-------------------------------------------------------------
//功能: 把新字符集安装到字符集目录中
//参数: encoding，新增的字符集指针
//      name，新增字符集名
//返回: true
//-----------------------------------------------------------------------------
bool_t  Charset_NlsInstallCharset(struct Charset *encoding,const char* name)
{
    if(s_ptCharsetDir == NULL)
        return (FALSE);       // 字符集目录未创建

    encoding->HostObj = obj_newchild(s_ptCharsetDir,(fnObjOps)-1,(ptu32_t)encoding,name);
    if(!encoding->HostObj)
    {
        return (FALSE);
    }

    if(!s_ptCurCharset)     //如果当前字符集未设置
        s_ptCurCharset = encoding;
    else
    {
        //当前字符集如果是调用API设置的，则维持，否则，如果新安装的字符集是本地字符集，
        //则把当前字符集设置为本地字符集
        if( ! g_bUserSetCharset)
        {
            if(strcmp(name,CFG_LOCAL_CHARSET) == 0)
                s_ptCurCharset = encoding;
        }
    }

    return (TRUE);
}

//----字符集模块初始化-------------------------------------------------------
//功能: 初始化字符集管理模块
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Charset(ptu32_t para)
{
    s_ptCurCharset = NULL;
    // 创建字符集目录
    s_ptCharsetDir = obj_newchild(obj_root(),(fnObjOps)-1,0,CN_CHAR_ENCODING_RSC_TREE);
    if(s_ptCharsetDir)
    {
        return 1;
    }else
    {
        debug_printf("charset","install charset module fail\r\n");
        return 0;
    }
}

//----获取当前字符集---------------------------------------------------------
//功能: 获取当前使用的字符集
//返回: 当前字符集
//-----------------------------------------------------------------------------
struct Charset* Charset_NlsGetCurCharset(void)
{
    return s_ptCurCharset;
}

//----设定当前字符集---------------------------------------------------------
//功能: 把新字符集设为当前使用的字符集,新字符集必须事先安装到系统中，如果
//      encoding字符集未安装，本函数什么都不做，返回原来的当前字符集
//参数: encoding，指定的字符集
//返回: NULL，设定失败
//      设定之前的字符集
//-----------------------------------------------------------------------------
struct Charset* Charset_NlsSetCurCharset(struct Charset* encoding)
{
    struct Object *Me;
    if(encoding == NULL)
        return NULL;
    if(encoding == s_ptCurCharset)
        return encoding;
    if(s_ptCharsetDir == NULL)
        return NULL;       //字符集目录未创建
    Me = encoding->HostObj;
    if(s_ptCharsetDir==obj_parent(Me)) // 字符集确已安装到s_ptCharsetDir目录
    {
        s_ptCurCharset = (struct Charset*)encoding;
        g_bUserSetCharset = true;
    }
    return s_ptCurCharset;
}

//----搜索字符集---------------------------------------------------------
//功能: 根据名称搜索字符集
//参数: name，指定的字符集名称
//返回: 要找的字符集指针，NULL=无此字符集
//
//-----------------------------------------------------------------------------
struct Charset* Charset_NlsSearchCharset(const char* name)
{
    struct Object *CharsetObj;
    if(s_ptCharsetDir == NULL)
        return NULL;       //字符资源树未创建

    CharsetObj = obj_search_child(s_ptCharsetDir,name);
    if(CharsetObj != NULL)
        return (struct Charset*)obj_GetPrivate(CharsetObj);
    else
        return NULL;
}

