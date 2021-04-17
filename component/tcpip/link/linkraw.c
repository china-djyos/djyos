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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//author:zhangqf
//date  :����5:14:21/2016��12��28��
//usage :
//------------------------------------------------------

//standard includes
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <os.h>
#include "dbug.h"
//add your own specified header here
#include <sys/socket.h>
#include <netbsp.h>
#include <netdb.h>
#include "../common/link.h"
#include "../common/netdev.h"

//data flow in the loop
//ipout->linkout->linkoutRAW->devout->devin->LinkInRaw->linkIn->link2Ip
#define CN_LINKRAW_NAME   "RAW"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//-----------------------------------------------------------------------------
//����:this is the link raw out function here
//����:
//����:
//��ע:
//����:zhangqf@����8:55:19/2016��12��28��
//-----------------------------------------------------------------------------
static bool_t __Link_Out(struct NetDev *iface,struct NetPkg *pkg,u32 devtask,\
        u16 proto,enum_ipv_t ver,ipaddr_t ipdst,ipaddr_t ipsrc)
{
    bool_t ret = false;
    if((NULL != iface)&&(proto == EN_LINKPROTO_IPV4))
    {
        NetDev_PkgsndInc(iface);
        ret = NetDev_Send(iface,pkg,devtask);
//      ret = iface->ifsend(iface,pkg,framlen,devtask);
        if(ret == false)
        {
            NetDev_PkgsndErrInc(iface);
        }
//      ret = NetDev_Send(iface,pkg,framlen,devtask);
    }
    return ret;
}

#pragma GCC diagnostic pop

//-----------------------------------------------------------------------------
//����:the link layer will call this function to deal the loop in package
//����:
//����:
//��ע:
//����:zhangqf@����9:18:35/2016��12��29��
//-----------------------------------------------------------------------------
static bool_t  __Link_LinIn(struct NetDev *iface,struct NetPkg *pkg)
{
    return Link_Push(iface,pkg,EN_LINKPROTO_IPV4);
}

//-----------------------------------------------------------------------------
//����: ע����·��Э�飬ÿ���������ж�Ӧ����·��Э�飬IP����������ͣ�Ӳ������������
//      ���ϲ��������ݡ����������ӦΨһ����·������ enum enLinkType ��ÿ����·����
//      �Ͷ�Ӧһ����·��Э�飬�� pLinkHal ����ָ��
//����:
//����:
//��ע:
//����:zhangqf@����9:26:04/2016��12��29��
//-----------------------------------------------------------------------------
bool_t Link_RawInit(void)
{
    bool_t ret;
    //first we will register a loop link type to the link hal
    struct LinkOps   ops;
    memset(&(ops),0,sizeof(ops));
    ops.linkin = __Link_LinIn;
    ops.linkout =__Link_Out;
    ret = Link_Register(EN_LINK_RAW,CN_LINKRAW_NAME,&ops);
    if(ret == false)
    {
        debug_printf("LINRAW","REGISTER ERR\n\r");
    }
    return ret;
}

