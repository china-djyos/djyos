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
//this file create an module that leads you how to initialize the net to match your
//board.if you want to configure the tcpip stack itself,please  cpy the tcpipconfig.cfg
//file to your user directory and modify the data as the instruction

#include <sys/socket.h>
#include "cpu_peri.h"     //for the k70 net device in the soc
const char *gc_NetCardName = "APOLLO_ETH";
static u8   gc_NetMac[CN_MACADDR_LEN] ={0x00,0x02,0x03,0x04,0x05,0x06};
static u16   gVlan88E6060[] = {0x20,0x20,0x20,0x20,0x20,0x1F};

//please refers the following function in the module-trim in proper place.
//make sure that that os kernel has been initialize ok and the interrupt system intialize ok
//make sure that that board net hardware has initialized and could work ok
ptu32_t ModuleInstall_NetStaticIP(ptu32_t para)   //static ip example
{
    //install the tcpip stack
    ModuleInstall_TcpIp(0);
	//install the net device you used,you could use more than one, but they
	//has different names and macs
    //use the corresponding net device install function you use
    extern bool_t ModuleInstall_ETH(const char *devname, u8 *mac,\
            bool_t loop,u32 loopcycle,\
            bool_t (*rcvHook)(u8 *buf, u16 len));
    ModuleInstall_ETH(gc_NetCardName,gc_NetMac,false,1*mS,NULL);
//    NetDevFilterSet(gc_NetCardName,EN_NETDEV_FRAME_NOPKG,10000,1000*mS,10*1000*mS);
//    NetDevFilterEnable(gc_NetCardName,EN_NETDEV_FRAME_NOPKG,true);
    NetDevFilterSet(gc_NetCardName,EN_NETDEV_FRAME_BROAD,100,100*1000*mS,1*1000*mS);
    NetDevFilterEnable(gc_NetCardName,EN_NETDEV_FRAME_BROAD,true);
    //make an link rout for the netdeb you installed,you could install more
	//than one link rout for the same net device with different net address
	tagHostAddrV4  ipv4addr;
	//we use the static ip we like
	memset((void *)&ipv4addr,0,sizeof(ipv4addr));
	ipv4addr.ip      = inet_addr("192.168.0.179");
	ipv4addr.submask = inet_addr("255.255.255.0");
	ipv4addr.gatway  = inet_addr("192.168.0.1");
	ipv4addr.dns     = inet_addr("192.168.0.1");
	ipv4addr.broad   = inet_addr("192.168.0.255");
	if(RoutCreate(gc_NetCardName,EN_IPV_4,(void *)&ipv4addr,CN_ROUT_NONE))
	{
	   printk("%s:Add %s success\r\n",__FUNCTION__,gc_NetCardName);
	}
	else
	{
		printk("%s:Add %s failed\r\n",__FUNCTION__,gc_NetCardName);
	}

	//do the 88e6060 chip set
    bool_t lan8720Init(u16 vlan[6]);
    lan8720Init(gVlan88E6060);

	return 0;
}

ptu32_t ModuleInstall_NetDynamicIP(ptu32_t para)   //use the dhcp to malloc an ip dynamic
{
    //install the tcpip stack
    ModuleInstall_TcpIp(0);
	//install the net device you used,you could use more than one, but they
	//has different names and macs
    //use the corresponding net device install function you use
    extern bool_t ModuleInstall_ETH(const char *devname, u8 *mac,\
            bool_t loop,u32 loopcycle,\
            bool_t (*rcvHook)(u8 *buf, u16 len));
    ModuleInstall_ETH(gc_NetCardName,gc_NetMac,false,1*mS,NULL);
    //set the netdevice filter
    //maybe you want to add some net device filter:such as broadcast limited
    //no frame reset and so on, if you use filter,operate it like following:
    //set the netdevice filter
    //initialize the filter(EN_NETDEV_FRAME_NOPKG),the measure time is 10 SECONS
    //and the action time(which will reset the net device) is 1 seconds
    //if in the 10s time, no package comes reached 10000 times, then the filter
    //initialize the nopkg filter
//    NetDevFilterSet(gc_NetCardName,EN_NETDEV_FRAME_NOPKG,10000,1000*mS,10*1000*mS);
//    NetDevFilterEnable(gc_NetCardName,EN_NETDEV_FRAME_NOPKG,true);

    NetDevFilterSet(gc_NetCardName,EN_NETDEV_FRAME_BROAD,100,100*1000*mS,1*1000*mS);
    NetDevFilterEnable(gc_NetCardName,EN_NETDEV_FRAME_BROAD,true);

    //make an link rout for the netdeb you installed
    //if you will malloc the ip from the dhcp server,make sure that the dhcp client has been enabled
    //actually,you could only install one rout for the same net dev
    if(DhcpAddClientTask(gc_NetCardName))
    {
       printk("%s:Add %s success\r\n",__FUNCTION__,gc_NetCardName);
    }
    else
    {
        printk("%s:Add %s failed\r\n",__FUNCTION__,gc_NetCardName);
    }

    //do the 88e6060 chip set
    bool_t lan8720Init(u16 vlan[6]);
    lan8720Init(gVlan88E6060);

	return 0;
}

