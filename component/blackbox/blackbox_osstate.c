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
// AND ANY BLACKBOXRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
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
// 模块描述: 异常时OS运行状态以及关键模块的信息搜集
// 模块版本: V1.00
// 创建人员: zhangqf_cyg
// 创建时间: 7:14:43 PM/Apr 16, 2014
// =============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// =============================================================================

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <endian.h>
#include <string.h>
#include <sys/time.h>
#include "dbug.h"
#include <os.h>

#include "blackbox_osstate.h"

#define CN_BLACKBOX_OSSTATEINFO_MAGICNUMBER  ((u32)0x11111111)
//跑出异常时搜集的系统信息
struct BlackBoxOsState
{
    u32     magicnumber;                             //系统信息有效标志
    u16     eventrunning_id;                         //正在运行的事件ID
    u16     evttrunning_id;                          //其事件类型Id
    char    evttrunningname[CN_EVTTNAME_LIMIT];      //事件类型名字
    time_t  blackboxtime;                            //异常事件发生时刻
};
static struct BlackBoxOsState s_tBlackBoxOsstateInfo;
// =============================================================================
// 函数功能:__BlackBox_OsStateInfoGather
//         异常时刻系统的运行状态
// 输入参数:throwpara,异常抛出者抛出的异常信息
//          传递过来是为了更好的搜集异常信息
// 输出参数:infoaddr,存储OS状态信息的地址
//          infolen,存储OS状态信息的长度
// 返回值  :true成功处理，false 处理失败
// 说明    :软件异常的通用信息处理，主要是搜集信息等。
//      异常模块内部使用，不对外开放
//      目前未针对异常具体抛出者搜集特别信息，做通用处理
// =============================================================================
void  __BlackBox_OsStateInfoGather(ptu32_t *infoaddr,u32 *infolen)
{
    memset(&s_tBlackBoxOsstateInfo,0,sizeof(s_tBlackBoxOsstateInfo));
    s_tBlackBoxOsstateInfo.blackboxtime = time(NULL);
    s_tBlackBoxOsstateInfo.eventrunning_id = DJY_GetMyEventId();
    s_tBlackBoxOsstateInfo.evttrunning_id  = DJY_GetMyEvttId();
    DJY_GetEvttName(s_tBlackBoxOsstateInfo.evttrunning_id,\
                    s_tBlackBoxOsstateInfo.evttrunningname,CN_EVTTNAME_LIMIT);
    s_tBlackBoxOsstateInfo.magicnumber = CN_BLACKBOX_OSSTATEINFO_MAGICNUMBER;
    *infoaddr = (ptu32_t)(&s_tBlackBoxOsstateInfo);
    *infolen = sizeof(s_tBlackBoxOsstateInfo);
#ifdef DEBUG
    printk("OSSTATE:运行事件：%d 所属类型：%d 系统运行时间：%s 类型名：%s\n\r",s_tBlackBoxOsstateInfo.eventrunning_id,
                                  s_tBlackBoxOsstateInfo.evttrunning_id&(~CN_EVTT_ID_MASK),
                                  ctime(&s_tBlackBoxOsstateInfo.blackboxtime),
                                  s_tBlackBoxOsstateInfo.evttrunningname);
#endif
    return;
}

// =============================================================================
// 函数功能:__BlackBox_OsstateinfoSwapByEndian
//          转换硬件CPU异常信息字节序
// 输入参数:osstate, 异常OS通用信息
// 输出参数:osstate, 异常OS通用信息（转换后）
// 返回值  :无
// 说明    :内部调用
// =============================================================================
void __BlackBox_OsstateinfoSwapByEndian(struct BlackBoxOsState *osstate)
{
    osstate->magicnumber = swapl(osstate->magicnumber);
    osstate->eventrunning_id = swaps(osstate->eventrunning_id);
    osstate->evttrunning_id = swaps(osstate->evttrunning_id);
}
// =============================================================================
// 函数功能:__BlackBox_OsStateInfoDecoder
//          软件通用异常信息解析
// 输入参数:throwpara,抛出者的参数
//          infoaddr,异常信息存储地址
//          infolen,异常信息长度
// 输出参数:
// 返回值  :true,成功设置;false,设置失败，主要是因为参数错误
// 说明    :内部调用
// =============================================================================
bool_t  __BlackBox_OsStateInfoDecoder(ptu32_t infoaddr,u32 infolen,u32 endian)
{
    bool_t  result;
    struct BlackBoxOsState osstate;
    if((infoaddr == 0)||(infolen != (sizeof(s_tBlackBoxOsstateInfo))))
    {
        debug_printf("osstate","Invalid para or incomplete info!\n\r");
        result = false;
    }
    else
    {
        memcpy((char *)(&osstate),(char *)infoaddr,infolen);
        if(((u32)CN_CFG_BYTE_ORDER) != endian)
        {
            __BlackBox_OsstateinfoSwapByEndian(&osstate);
        }
        if(CN_BLACKBOX_OSSTATEINFO_MAGICNUMBER == osstate.magicnumber)
        {
            debug_printf("osstate","OSSTATE:运行事件：%d 所属类型：%d 系统运行时间：%s 类型名：%s\n\r",\
                    osstate.eventrunning_id,osstate.evttrunning_id&(~CN_EVTT_ID_MASK), ctime(&osstate.blackboxtime),osstate.evttrunningname);
            result = true;
        }
        else
        {
            debug_printf("osstate","DESTROYED PACKAGE \n\r");
            result = false;
        }
    }
    return result;
}

