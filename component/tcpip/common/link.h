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
/*
 * link.h
 *
 *  Created on: 2018��5��3��
 *      Author: zhangqf
 */

#ifndef TCPIP_LINK_H_
#define TCPIP_LINK_H_

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

//return value is the really receive or send data units(bytes)
//the device driver use this function to pass the receive package to the tcpip stack
typedef bool_t (*fnLinkIn)(struct NetDev *iface,struct NetPkg *pkg); //pkg is the link frame
//the tcpip stack use this function to pass the package to the hard device
typedef bool_t (*fnLinkOut)(struct NetDev *iface,struct NetPkg *pkglst,u32 devtask,u16 proto,\
                            enum_ipv_t ver,ipaddr_t ipdst,ipaddr_t ipsrc);//pkg is the ip frame
//this function is used do some command from the upper protocols;
typedef bool_t (*fnLinkIo)(struct NetDev *dev,u32 cmd,u32 para);
struct LinkOps
{
    fnLinkIn     linkin;
    fnLinkOut    linkout;
//  fnLinkIo     linkio;
};
bool_t Link_Register(enum enLinkType type,const char *name,struct LinkOps *ops);
bool_t Link_UnRegister(enum enLinkType type,const char *name);
bool_t Link_Send(struct NetDev *DevFace,struct NetPkg *pkg,u32 devtask,u16 proto,\
        enum_ipv_t ver,ipaddr_t ipdst,ipaddr_t ipsrc);
struct LinkOps  *Link_FindOps(enum enLinkType type);
const char *Link_TypeName(enum enLinkType type);
bool_t Link_Init(void);
bool_t Link_PushRegister(u16 protocol,fnLinkProtoDealer dealer);//ip and arp intall their push functions
bool_t Link_Push(struct NetDev  *iface,struct NetPkg *pkg,enum enLinkProto protocol); //and the linker layer call this function
//bool_t LinkDeal(struct NetDev *iface,struct NetPkg *pkg);//the net device layer call this function


#endif /* TCPIP_LINK_H_ */
