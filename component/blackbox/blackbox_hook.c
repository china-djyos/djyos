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
// 模块描述: 异常的钩子处理部分
// 模块版本: V1.00
// 创建人员: zhangqf_cyg
// 创建时间: 2:29:24 PM/Apr 17, 2014
// =============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// =============================================================================
// 备注:主要处理钩子的注册和调用


#include "stdint.h"
#include "stddef.h"
#include "blackbox.h"
#include "blackbox_hook.h"

static fnBlackBox_Hook   s_fnBlackBoxHookDealer = NULL;
static fnBlackBox_HookParse  s_fnBlackBoxHookInfoDecoder = NULL;

// =============================================================================
// 函数功能: 执行HOOKDealer
// 输入参数:throwpara,异常抛出者抛出的异常参数
// 输出参数:infoaddr, 存储异常信息的地址
//          infolen,存储搜集信息长度
//          BlackBoxAction,保存处理结果，当没有HOOK时，该值无效
// 返回值  :true成功  false 失败（没有注册等因素，钩子结果不用采纳）
// =============================================================================
bool_t __BlackBox_HookDealer(struct BlackBoxThrowPara *throwpara,\
                          ptu32_t *infoaddr,u32 *infolen,enum EN_BlackBoxAction *BlackBoxAction)
{
    if(NULL != s_fnBlackBoxHookDealer)
    {
        *BlackBoxAction = s_fnBlackBoxHookDealer(throwpara,infoaddr,infolen);
        return true;
    }
    else
    {
        *infoaddr = (ptu32_t)NULL;
        *infolen = 0;
        *BlackBoxAction = EN_BLACKBOX_DEAL_DEFAULT;
        return true;
    }
}
// =============================================================================
// 函数功能: 执行HOOKDecoder
// 输入参数:throwpara, 异常参数，hook能解就解，不能解就算了
//          infoaddr, 信息存储地址
//          infolen, 信息有效长度
//          endian,存储时候的大小端
// 输出参数:无
// 返回值  :true成功  false 失败（没有注册等因素）
// =============================================================================
bool_t __BlackBox_HookInfoDecoder(struct BlackBoxThrowPara *throwpara,\
                           ptu32_t infoaddr, u32 infolen,u32 endian)
{
    if(NULL != s_fnBlackBoxHookInfoDecoder)
    {
         return  s_fnBlackBoxHookInfoDecoder(throwpara,infoaddr,infolen,endian);
    }
    else
    {
        return false;
    }
}


// =============================================================================
// 函数功能：注册APP提供的异常处理HOOK，当异常发生后，将调用fnHookFunc函数，
//           详见 fnBlackBox_Hook类型定义的注释。fnHookFunc函数可以通过infoaddr指针
//           返回一些信息，这些信息如果不是字符串，可以进一步提供fnHookParse，
//           用于把信息解析成可读信息。
// 输入参数:fnHookFunc，异常处理器
//          fnHookParse，异常信息解析器
// 输出参数:无
// 返回值  ：true成功  false失败
// =============================================================================
bool_t BlackBox_RegisterHook(fnBlackBox_Hook  fnHookFunc,
                             fnBlackBox_HookParse fnHookParse)
{
    s_fnBlackBoxHookDealer = fnHookFunc;
    s_fnBlackBoxHookInfoDecoder = fnHookParse;
    return true;
}

// =============================================================================
// 函数功能:BlackBox_UnRegisterHook
//          注销APP提供的异常处理HOOK以及异常信息解析器
// 输入参数:无
// 输出参数:无
// 返回值  :true成功  false失败
// =============================================================================
bool_t BlackBox_UnRegisterHook(void)
{
    s_fnBlackBoxHookDealer = NULL;
    s_fnBlackBoxHookInfoDecoder = NULL;

    return true;
}

