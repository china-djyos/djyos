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
#include <netbsp.h>
#include <netdb.h>
#include "../common/link.h"
#include "../common/netdev.h"
#include "../common/netpkg.h"
#include "arp.h"


//data flow in the loop
//ipout->linkout->linkoutloop->loopout->loopin->LinkIn->linkInLoop->link2ip
//the internet proto we supported now
#pragma pack(1)
typedef struct EthernetHdr
{
    u8 macdst[CN_MACADDR_LEN];
    u8 macsrc[CN_MACADDR_LEN];
    u16 type_or_tpid;           //�����vlan֡����=0x8100�����򣬴���Э������
    u16 vlantci;                //�����vlan֡������vlan��tci��
    u16 type_if_vlan;           //�����vlan֡��Э�������Ƶ�����

}tagEthernetHdr;
#pragma pack()
#define CN_ETHERNET_HEADLEN_UNVLAN  14      //��vlan֡��֡ͷ����
#define CN_ETHERNET_HEADLEN_VLAN    18      //vlan֡��֡ͷ����
//-----------------------------------------------------------------------------
//����: ��·�㷢�ͺ�����׼����Ethernet�İ�ͷ�������֪���Է�Mac��ַ�������ARP���ܻ�ȡ��
//      Ȼ����������豸�˶�ifsend�������Ͱ���
//����: iface�������豸ָ��
//      pkg���뷢�͵İ���
//      framlen���뷢�͵ı����ܳ��ȣ����������е����нڵ�
//      devtask������Ӳ�����ٹ�����Ҫ��ɵĹ������������tcp��IPͷ��У���
//      proto��Э�����ͣ�0800/0806/0835��
//      ver��IP�汾�š�TODO���˴�ֵ����ȶ�������ѵ�ֻ�ܷ���IP����
//      ipdst��Ŀ��IP��ַ
//      ipsrc��ԴIP��ַ
//����:
//��ע:
//����:zhangqf@����8:55:19/2016��12��28��
//-----------------------------------------------------------------------------
static bool_t __Link_Out(struct NetDev *iface,struct NetPkg *pkg,u32 devtask,\
                           u16 proto,enum_ipv_t ver,ipaddr_t ipdst,ipaddr_t ipsrc)
{
    bool_t            ret;
    tagEthernetHdr   *hdr;
    struct NetPkg    *ethpkg;

    ret = false;
    ethpkg = PkgMalloc(CN_ETHERNET_HEADLEN_UNVLAN,0);
    if(NULL != ethpkg)
    {
        hdr = (tagEthernetHdr *)PkgGetCurrentBuffer(ethpkg);
//      hdr = (tagEthernetHdr  *)(ethpkg->buf + ethpkg->offset);
        memcpy(hdr->macsrc, NetDev_GetMac(iface), CN_MACADDR_LEN);
        hdr->type_or_tpid = htons(proto);
        if(ver == EN_IPV_4)
        {
            //we should just do it here
            if(devtask&CN_IPDEV_IPBROAD) //do broad here
            {
                memcpy(hdr->macdst,CN_MAC_BROAD,CN_MACADDR_LEN);
                ret = true;
            }
            else if(devtask&CN_IPDEV_IPMULTI) //do multi
            {
                ret = true; //not implement yet
            }
            else
            {
                //we must find the mac address use the arp processor
                ret = ResolveMacByArp(ipdst,ipsrc,iface,hdr->macdst);  //the ipv4 support
            }
            if(false == ret)//if false,we use the broadcast mac address
            {
                memcpy(hdr->macdst,CN_MAC_BROAD,CN_MACADDR_LEN);
            }
            PkgSetNextUnit(ethpkg,pkg);
            PkgSetDataLen(ethpkg, CN_ETHERNET_HEADLEN_UNVLAN);
//          ethpkg->datalen = CN_ETHERNET_HEADLEN;
//          ethpkg->partnext = pkg;
//          framlen += CN_ETHERNET_HEADLEN_UNVLAN;
//          ret = NetDev_Send(iface,ethpkg,framlen,devtask);
            NetDev_PkgsndInc(iface);
            ret = NetDev_Send(iface,ethpkg,devtask);
            if(ret == false)
            {
                NetDev_PkgsndErrInc(iface);
            }
            PkgTryFreePart(ethpkg);
        }
        else
        {
            //not support the ipv6 yet
        }

    }
    return ret;
}

//-----------------------------------------------------------------------------
//����: ���������ͺ������ã�������·�㲿�֣�ȡ���ϲ�Э�飬������LinkPush���������͵�����
//      ��Э�鴦��
//����: iface������Դ�����豸ָ��
//      pkg���������������ݰ�
//����: true or false
//-----------------------------------------------------------------------------
static bool_t  __LinkIn(struct NetDev *iface,struct NetPkg *pkg)
{
    bool_t          ret=false;
    u8              *buf;
    tagEthernetHdr *hdr;
    u16             proto;
    if(PkgGetDataLen(pkg) > CN_ETHERNET_HEADLEN_UNVLAN)
//  if(pkg->datalen > CN_ETHERNET_HEADLEN)
    {
        //we analyze the ethernet header first, which type it has
        hdr = (tagEthernetHdr *)PkgGetCurrentBuffer(pkg);
//      hdr = (tagEthernetHdr *)(pkg->buf + pkg->offset);
//      memcpy(&proto,&hdr->type,sizeof(proto));
        buf = (u8*)&hdr->type_or_tpid;
        proto = (((u16)*buf)<<8) + *(buf+1);
        if(proto==0x8100)           //��vlan��ǩ��֡����Щ���������vlan֡������
        {
            buf += 4;               //vlan �� 4 �ֽ�
            proto = (((u16)*buf)<<8) + *(buf+1);        //ȡ��������Э���ֶ�
            PkgMoveOffsetUp(pkg,CN_ETHERNET_HEADLEN_VLAN);
        }
        else
            PkgMoveOffsetUp(pkg,CN_ETHERNET_HEADLEN_UNVLAN);

//      pkg->offset += CN_ETHERNET_HEADLEN;
//      pkg->datalen -= CN_ETHERNET_HEADLEN;
        ret = Link_Push(iface,pkg,(enum enLinkProto)proto);
    }
    return ret;
}
//-----------------------------------------------------------------------------
//����:we use this function to register the ethernet type to the link
//����:
//����:
//��ע:
//����:zhangqf@����9:26:04/2016��12��29��
//-----------------------------------------------------------------------------
bool_t LinkEthernetInit(void)
{
    bool_t ret = true;
    //first we will register a loop link type to the link hal
    struct LinkOps   ops;
    memset(&(ops),0,sizeof(ops));
    ops.linkin = __LinkIn;
    ops.linkout =__Link_Out;
    ret = Link_Register(EN_LINK_ETHERNET,"ETHERNET",&ops);

    return ret;
}

