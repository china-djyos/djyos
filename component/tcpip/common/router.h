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

#ifndef TCPIP_ROUT_ROUTER_H_
#define TCPIP_ROUT_ROUTER_H_

#include <sys/socket.h>
#include <osarch.h>

struct RoutItem4;
//����˵����
//���ļ���·��ģ�飬��ģ����ɵ���ҪΪ������ṩ���ں���ڵ�ƥ�����
//������·�ɷ���ǽ���ڸ�ģ��ʵ�֡�
//next hop:in the item,if the host and the dest net is not in the same net, then it will use the
//next hop as the destination,and use the original destination ip as the rout destination
//should do a external function in the net hal to get the device handle and Name
//NULL FOR FAILED
typedef enum
{
    EN_IPTYPE_V4_UNKOWN = 0,
    EN_IPTYPE_V4_ANY,   //0.0.0.0
    EN_IPTYPE_V4_HOST,  //127.0.0.1--�������⴦��,2020.1.1
    EN_IPTYPE_V4_LOCAL, //the other ip you configure or get dynamic
    EN_IPTYPE_V4_BROAD, //such as 192.168.0.255 or 255.255.255.255
    EN_IPTYPE_V4_SUBNET, //in the subnet
    EN_IPTYPE_V4_MULTI,
}enIPTYPE;



bool_t RouterInit(void);
//routitem is the router internal data structure, should not be exposed to external
//so we make this one
typedef struct
{
    enum_ipv_t       ver;     //���� RouterMatch ǰ�����ṩ
    enIPTYPE         type;    //RouterMatch�������ؽ����
    struct NetDev   *DevFace; //�����豸�ӿ�
    void            *DstIP;   //this structure depends on the ver:ipv4 ipv6
    void            *HopIP;   //this structure depends on the ver:ipv4 ipv6
    void            *HostIP;  //this structure depends on the ver:ipv4 ipv6
}tagRoutLink; //you could use this structure to storage the message need in the external
//you could do the decision on the type,if UNKNOWN,which means we could resolve this
//if need more function,you could extend it this structure -- TODO,zhangqf
bool_t RouterSearchHost(struct in_addr host);
bool_t RouterMatch(tagRoutLink *rout);
//if not call, then will not do the loop internal
void RouterSetHost(enum_ipv_t ver,const char *ifname);

#endif /* TCPIP_ROUT_ROUTER_H_ */
