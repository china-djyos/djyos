//-----------------------------------------------------------------------------
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
#ifndef __ROUT_H
#define __ROUT_H

#include <sys/socket.h>

typedef struct NetDev  tagNetDev;
typedef struct  __Rout
{
   struct __Rout  *nxt;        //rout chain
   tagNetDev      *dev;        //this rout owner
   u32             pro;        //rout property
   u32             life;       //this rout time
   u32             func;       //inherit from the device
   enum_ipv_t      ver;        //ipv4 or v6
   union
   {
       tagHostAddrV4 ipv4;
       tagHostAddrV6 ipv6;
   }ipaddr;
   //the following used for the debug
   u64 innum;
   u64 inerr;
   u64 outnum;
   u64 outerr;
}tagRout;


//return value is the really receive or send data units(bytes)
//the device driver use this function to pass the receive package to the tcpip stack
typedef bool_t (*fnLinkIn)(tagNetDev *dev,tagNetPkg *pkg); //pkg is the link frame
//the tcpip stack use this function to pass the package to the hard device
typedef bool_t (*fnLinkOut)(tagRout *rout,tagNetPkg *pkglst,u32 framlen,u32 devtask,u16 proto,enum_ipv_t ver,ipaddr_t ipdst);//pkg is the ip frame
//this function is used do some command from the upper protocols;
typedef bool_t (*fnLinkIo)(tagNetDev *dev,u32 cmd,u32 para);
typedef struct
{
	fnLinkIn     linkin;
	fnLinkOut    linkout;
	fnLinkIo     linkio;
	void       *ctx;    //this is the link control block here,used by the link here
	//do the debug info
	s64         innum;
	s64         inerr;
	s64         outnum;
	s64         outerr;
}tagLinkOps;
//each device must have a link operations

typedef struct
{
	vu32       enable:1;             //enable it or not
	vu32       uaction:1;            //if more than the upperlimit then set it
	vu32       laction:1;            //if less than lower limit then set it
	vu32       lactionb:5;           //low begin message
	vu32       lactione:5;           //low end message
	vu32       uactionb:5;           //upper begin message
	vu32       uactione:5;           //upper end message
	vu32       actiontimes;          //which means the action total times
	vu32       fcounter;             //which means how many frame received
	vu32       fulimit;              //which means the upper limit
	vu32       fllimit;              //which means the lower limit
	vu32       period;               //which means the measure period
	vu64       ftotal;               //which means the total frame of the type
	vs64       deadtime;             //which means the measure end time
}tagNetDevRcvFilter;
struct NetDev
{
    struct NetDev                 *nxt;                    //dev chain
    char                           name[CN_TCPIP_NAMELEN]; //dev name
    u8                             iftype;   //dev type
    fnIfSend                       ifsend;   //dev snd function
    fnIfCtrl                       ifctrl;   //dev ctrl or stat get fucntion
    fnNetDevEventHook              eventhook;//dev event hook dealer
    u32                            devfunc;  //dev hard function,such as tcp chksum
    u16                            mtu;      //dev mtu
    ptu32_t                        private;  //the dev driver use this to has its owner property
    u8                             mac[CN_MACADDR_LEN];   //mac address
    tagRout                       *routq;    //an net maqybe has many ip address
    //the following used to debug the net device,show some main status
    u32                            fsnd;     //frame send
    u32                            fsnderr;  //frame snd failed
    u32                            frcv;     //frame receive
    u32                            frcverr;  //frame receive err
    tagNetDevRcvFilter             rfilter[EN_NETDEV_FLOW_LAST];  //the recv filter
    //the following function used to package the info
    tagLinkOps                    *linkops;
};

tagNetDev  *NetDevGet(const char  *name);
#define     NetDevGetMac(dev)     (dev->mac)       //get the dev mac(stack use)
#define     NetDevGetFunc(dev)    (dev->devfunc)   //get the dev func(stack use)
#define     RoutFunc(rout)        (rout->func)     //use this to get the rout func
bool_t   RoutUpdate(char *name,enum_ipv_t ver,void *netaddr);// use this function to update
tagRout *RoutGetDefault(enum_ipv_t ver);//use this function to get the default rout
tagRout *RoutMatch(enum_ipv_t ver,ipaddr_t ipaddr);//use this function to find a rout to output the ip package
bool_t   RoutHostIp(enum_ipv_t ver,ipaddr_t ipaddr);//check if the ip is the host address
bool_t   RoutHostTarget(enum_ipv_t ver,ipaddr_t ipaddr);//check if the ip is the host address
bool_t   RoutSubNet(tagNetDev *dev,enum_ipv_t ver,ipaddr_t ipaddr);
#endif /* __ROUT_H */
