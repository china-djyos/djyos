//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
//date  :����4:54:56/2016��12��29��
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
//add your own specified header here
#include <sys/socket.h>

#include "../linkhal.h"
#include "../rout.h"
#include "../arp.h"
//data flow in the loop
//ipout->linkout->linkoutloop->loopout->loopin->LinkIn->linkInLoop->link2ip
//the internet proto we supported now
#pragma pack(1)
typedef struct EthernetHdr
{
    u8 macdst[CN_MACADDR_LEN];
    u8 macsrc[CN_MACADDR_LEN];
    u16 type;
}tagEthernetHdr;
#pragma pack()
#define CN_ETHERNET_HEADLEN  sizeof(tagEthernetHdr)
//-----------------------------------------------------------------------------
//����:this is the loop out function here
//����:
//����:
//��ע:
//����:zhangqf@����8:55:19/2016��12��28��
//-----------------------------------------------------------------------------
static bool_t __LinkOutEth(tagRout *rout,tagNetPkg *pkg,u32 framlen,u32 devtask,u16 proto,enum_ipv_t ver,ipaddr_t ipdst)
{
    bool_t            result;
    tagEthernetHdr   *hdr;
    tagNetPkg        *ethpkg;

    result = false;
    ethpkg = PkgMalloc(CN_ETHERNET_HEADLEN,0);
    if(NULL != ethpkg)
    {
        hdr = (tagEthernetHdr  *)(ethpkg->buf + ethpkg->offset);
        memcpy(hdr->macsrc, NetDevGetMac(rout->dev), CN_MACADDR_LEN);
        hdr->type = htons(proto);
        if(ver == EN_IPV_4)
        {
        	if((ipdst == INADDR_BROADCAST)||(ipdst == rout->ipaddr.ipv4.broad))
        	{
        		memcpy(hdr->macdst,CN_MAC_BROAD,CN_MACADDR_LEN);
        		result = true;
        	}
        	else
        	{
        		//we must find the mac we self
        		//if in the subnet then use the ipdst
        		//else we pass the package to the gatway
        		if((ipdst&rout->ipaddr.ipv4.submask) !=\
        		   ((rout->ipaddr.ipv4.ip&rout->ipaddr.ipv4.submask)))
        		{
        			ipdst = rout->ipaddr.ipv4.gatway;
        		}
                result = ArpFindMac(ipdst,hdr->macdst);  //the ipv4 support
        	}
            if(result)
            {
                ethpkg->datalen = CN_ETHERNET_HEADLEN;
                ethpkg->partnext = pkg;
                framlen += CN_ETHERNET_HEADLEN;
                result = LinkSendRaw(rout->dev,ethpkg,framlen, devtask);
            }
            else
            {
                 //can't find the mac
            }
            PkgTryFreePart(ethpkg);
        }
        else
        {
            //not support the ipv6 yet
        }

    }
    return result;
}

//-----------------------------------------------------------------------------
//����:the link layer will call this function to deal the loop in package
//����:
//����:
//��ע:here we dispatch the ethernet header and pass it to the ip
//����:zhangqf@����9:18:35/2016��12��29��
//-----------------------------------------------------------------------------
bool_t  __LinkInEth(tagNetDev * dev,tagNetPkg *pkg)
{
    bool_t          result=false;
    tagEthernetHdr *hdr;
    u16             proto;
    if(pkg->datalen > CN_ETHERNET_HEADLEN)
    {
        //we analyze the ethernet header first, which type it has
        hdr = (tagEthernetHdr *)(pkg->buf + pkg->offset);
        memcpy(&proto,&hdr->type,sizeof(proto));
        proto = ntohs(proto);
        pkg->offset += CN_ETHERNET_HEADLEN;
        pkg->datalen -= CN_ETHERNET_HEADLEN;
        result = Link2IP(dev,pkg,proto);
    }
	return result;
}
//-----------------------------------------------------------------------------
//����:we use this function to register the ethernet type to the link
//����:
//����:
//��ע:
//����:zhangqf@����9:26:04/2016��12��29��
//-----------------------------------------------------------------------------
bool_t LinkEthernetInit(ptu32_t para)
{
    //first we will register a loop link type to the link hal
    tagLinkOps   ops;
    memset(&(ops),0,sizeof(ops));
    ops.linkin = __LinkInEth;
    ops.linkout =__LinkOutEth;
    return LinkRegister(EN_LINK_ETHERNET,"ETHERNET",&ops);
}

