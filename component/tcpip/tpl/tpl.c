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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������
// 3. ��������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ��������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
#include "../common/tpl.h"
#include "../component_config_tcpip.h"

typedef struct
{
    int                af_inet;
    int                type;
    int                protocol;
    tagTlayerProto    *proto;
}tagTplProtoItem;
static tagTplProtoItem    *pTplProtoTab = NULL;
static struct MutexLCB    *pTplProtoSync = NULL;


// =============================================================================
// FUNCTION��this function is used to initialize the transmit layer
// PARA  IN��
// PARA OUT��
// RETURN  ��
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

    pTplProtoSync = mutex_init(NULL);
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

// =============================================================================
// FUNCTION��this function is used to find an transmit layer proto
// PARA  IN��
// PARA OUT��
// RETURN  :the proto found or NULL failed
// INSTRUCT:
// =============================================================================
tagTlayerProto *TPL_GetProto(int family, int type, int protocol)
{
    int i = 0;
    tagTlayerProto *result = NULL;

    if((NULL!=pTplProtoTab)&&mutex_lock(pTplProtoSync))
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

        mutex_unlock(pTplProtoSync);
    }

    return result;
}
// =============================================================================
// FUNCTION��this function is used to find an register an transmit layer protocol
// PARA  IN��
// PARA OUT��
// RETURN  :true success while false failed
// INSTRUCT:
// =============================================================================
bool_t TPL_RegisterProto(int family, int type, int protocol,tagTlayerProto *proto)
{
    int i = 0;

    bool_t result = false;
    tagTlayerProto *tmp = NULL;

    if((NULL!=pTplProtoTab)&&mutex_lock(pTplProtoSync))
    {
        //check any one existed
        for(i =0; i< CFG_TPL_PROTONUM;i++)
        {
            if((pTplProtoTab[i].af_inet == family)&&(pTplProtoTab[i].type == type)&&\
                pTplProtoTab[i].protocol == protocol)
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

        mutex_unlock(pTplProtoSync);
    }

    return result;
}



