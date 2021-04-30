//-----------------------------------------------------------------------------
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
#include "../common/tpl.h"
#include "../component_config_tcpip.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。


typedef struct
{
    int                af_inet;     //在socket.h中定义，例如： AF_INET
    int                type;        //在socket.h中定义，例如： SOCK_DGRAM
    int                protocol;    //在socket.h中定义，例如： IPPROTO_UDP
    struct TPL_ProtocalOps  *proto;       //在各协议模块中初始化
}tagTplProtoItem;
static tagTplProtoItem    *pTplProtoTab = NULL;
static struct MutexLCB    *pTplProtoSync = NULL;


// =============================================================================
// FUNCTION：this function is used to initialize the transmit layer
// PARA  IN：
// PARA OUT：
// RETURN  ：
// INSTRUCT:
// =============================================================================
bool_t TPLInit(void)
{
    bool_t result;
    pTplProtoTab = (tagTplProtoItem *)net_malloc(sizeof(tagTplProtoItem)*CFG_TPL_PROTONUM);
    if(NULL == pTplProtoTab)
    {
        goto EXIT_MEMFAIL;
    }
    memset((void *)pTplProtoTab,0,sizeof(tagTplProtoItem)*CFG_TPL_PROTONUM);

    pTplProtoSync = Lock_MutexCreate(NULL);
    if(NULL == pTplProtoSync)
    {
        goto EXIT_SYNCFAILED;
    }
    result = true;
    return result;

EXIT_SYNCFAILED:
    net_free((void *)pTplProtoTab);
    pTplProtoTab = NULL;
EXIT_MEMFAIL:
    result = false;
    return result;
}

//------------------------------------------------------------------------------
//功能：获取一个传输层协议
//参数：family，地址族，如 AF_INET 等
//     type，协议类型，如 SOCK_STREAM 等
//     protocol，协议，如 IPPROTO_TCP 等
//返回：传输层接口 struct TPL_ProtocalOps 指针，或 NULL
//------------------------------------------------------------------------------
struct TPL_ProtocalOps *TPL_GetProto(int family, int type, int protocol)
{
    int i = 0;
    struct TPL_ProtocalOps *result = NULL;

    if((NULL!=pTplProtoTab)&&Lock_MutexPend(pTplProtoSync,CN_TIMEOUT_FOREVER))
    {
        for(i =0; i< CFG_TPL_PROTONUM;i++)
        {
            if((pTplProtoTab[i].af_inet == family)&&(pTplProtoTab[i].type == type)&&\
                    pTplProtoTab[i].protocol == protocol)
            {
                result = pTplProtoTab[i].proto;
                break;
            }
        }

        Lock_MutexPost(pTplProtoSync);
    }

    return result;
}

//------------------------------------------------------------------------------
//功能：注册一个传输层协议
//参数：family，地址族，如 AF_INET 等
//     type，协议类型，如 SOCK_STREAM 等
//     protocol，协议，如 IPPROTO_TCP 等
//     proto，传输协议接口函数集
//返回：true or false
//------------------------------------------------------------------------------
bool_t TPL_RegisterProto(int family, int type, int protocol,
                            struct TPL_ProtocalOps *proto)
{
    int i = 0;

    bool_t result = false;
    struct TPL_ProtocalOps *tmp = NULL;

    if((NULL!=pTplProtoTab)&&Lock_MutexPend(pTplProtoSync,CN_TIMEOUT_FOREVER))
    {
        //check any one existed
        for(i =0; i< CFG_TPL_PROTONUM;i++)
        {
            if((pTplProtoTab[i].af_inet == family)&&(pTplProtoTab[i].type == type)
                        && (pTplProtoTab[i].protocol == protocol))
            {
                tmp = pTplProtoTab[i].proto;
                break;
            }
        }
        if(NULL == tmp)
        {
            //ok, could register it now,find an place and insert
            for(i =0; i< CFG_TPL_PROTONUM;i++)
            {
                if(NULL == pTplProtoTab[i].proto)
                {
                    pTplProtoTab[i].af_inet = family;
                    pTplProtoTab[i].type = type;
                    pTplProtoTab[i].protocol = protocol;
                    pTplProtoTab[i].proto = proto;
                    result = true;
                    break;
                }
            }
        }

        Lock_MutexPost(pTplProtoSync);
    }

    return result;
}




