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
// 模块描述: SysBlackBox_Shell.c
// 模块版本: V1.00
// 创建人员: zhangqf_cyg
// 创建时间: 9:20:36 AM/Mar 27, 2014
// =============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// =============================================================================
// 备注:该文件主要是为了实现SHELL的一些接口功能


#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "blackbox.h"
#include "blackbox_record.h"
#include "shell.h"
#include "systime.h"
#include "dbug.h"

bool_t  BlackBox_InfoDecoder(struct BlackBoxRecordPara *recordpara);

// =============================================================================
// 函数功能：BlackBox_ShellBrowseAssignedRecord
//           shell查看指定条目的异常信息
// 输入参数：param,指定条目字符串
// 输出参数：无
// 返回值  ：true成功  false失败
// 说明    ：
// =============================================================================
//bool_t BlackBox_ShellBrowseAssignedRecord(char *param)
bool_t blackboxi(char *param)
{
    bool_t   result;
    char *word,*next_param;
    u32  item_num;
    u32  item_len;
    u8   *infomem;
    struct BlackBoxRecordPara recordpara;

    if(NULL == param)
    {
        result = false;
        goto browse_end;
    }

    word = shell_inputs(param,&next_param);
    item_num = strtol(word, (char **)NULL, 0);

    result = BlackBox_RecordCheckLen(item_num, &item_len);
    if(false == result)
    {
        debug_printf("shell","请指定正确的条目号\n\r");
        return true;
    }

    infomem = M_MallocLc(item_len, CN_TIMEOUT_FOREVER);
    if(NULL == infomem)
    {
        debug_printf("shell","请指定正确的条目号\n\r");
        return true;
    }

    result = BlackBox_RecordGet(item_num,item_len,infomem, &recordpara);
    if(false == result)
        debug_printf("shell","读取异常信息失败\n\r");
    else
        BlackBox_InfoDecoder(&recordpara);

    free(infomem);
    return true;

browse_end:
    return result;

}
// =============================================================================
// 函数功能：BlackBox_ShellBrowseRecordNum
//           shell查看存储的异常条目数
// 输入参数：param,无
// 输出参数：无
// 返回值  ：true成功  false失败
// 说明    ：
// =============================================================================
//bool_t BlackBox_ShellBrowseRecordNum(char *param)
bool_t blackboxn(char *param)
{
    bool_t  result;

    u32 blackboxnum;

    result = BlackBox_RecordCheckNum(&blackboxnum);
    if(true == result)
    {
        debug_printf("shell","ShellSysBlackBox:There has been 0x%08x BlackBox record!\n\r",blackboxnum);
        if(blackboxnum > 0)
        {
            debug_printf("shell","ShellSysBlackBox:VALID NO.(1-%d)\n\r",blackboxnum);
        }
        else
        {
            debug_printf("shell","There has been no exception record yet!\n\r");
        }
    }
    else
        debug_printf("shell","Get Exception Number Failed\n\r");
    return result;
}

// =============================================================================
// 函数功能：BlackBox_ShellRecordClear
//           shell清除所有的异常信息
// 输入参数：param,无
// 输出参数：无
// 返回值  ：true成功  false失败
// 说明    ：
// =============================================================================
//bool_t BlackBox_ShellRecordClear(char *param)

bool_t blackboxc(char *param)
{
    bool_t  result;

    result = BlackBox_RecordClear();

    if(true == result)
    {
        debug_printf("shell","ShellSysBlackBox:Clear SysBlackBox info success!\r\n");
    }
    else
    {
        debug_printf("shell","ShellSysBlackBox:Clear SysBlackBox info failed!\r\n");
    }

    return result;
}

// =============================================================================
// 函数功能：BlackBox_ShellInit
//          blackboxshell模块初始化
// 输入参数：param,无
// 输出参数：无
// 返回值  ：true成功  false失败
// 说明    ：
// =============================================================================
bool_t BlackBox_ShellInit()
{
    return true;
}
ADD_TO_ROUTINE_SHELL(blackboxc,blackboxc,"clear all the exception item");
ADD_TO_ROUTINE_SHELL(blackboxn,blackboxn,"browse the exception number");
ADD_TO_ROUTINE_SHELL(blackboxi,blackboxi,"browse the specified exception item");

