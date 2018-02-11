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
const char *gNetDevName = "k70eth";

//please refers the following function in the module-trim in proper place.
//make sure that that os kernel has been initialize ok and the interrupt system intialize ok
//make sure that that board net hardware has initialized and could work ok
ptu32_t ModuleInstall_BoardNetStaic(ptu32_t para)   //static ip example
{
    //install the tcpip stack
    ModuleInstall_TcpIp(0);
	//install the net device you used,you could use more than one, but they
	//has different names and macs
    //use the corresponding net device install function you use
    extern bool_t ModuleInstall_Enet(ptu32_t para);
    tagEnetCfg EnetCfg = {gNetDevName,MAC_RMII,AUTONEG_ON,MII_100BASET,
                          MII_FDX,NO_LOOPBACK,EXTERNAL_NONE,0,
                          {0x00,0x01,0x02,0x03,0x04,0x03}};
    ModuleInstall_Enet((ptu32_t)&EnetCfg);

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
	if(RoutCreate(gNetDevName,EN_IPV_4,(void *)&ipv4addr,CN_ROUT_NONE))
	{
	   printk("%s:Add %s success\r\n",__FUNCTION__,gNetDevName);
	}
	else
	{
		printk("%s:Add %s failed\r\n",__FUNCTION__,gNetDevName);
	}

	return 0;
}

ptu32_t ModuleInstall_BoardNetDynamic(ptu32_t para)   //use the dhcp to malloc an ip dynamic
{
    //install the tcpip stack
    ModuleInstall_TcpIp(0);
	//install the net device you used,you could use more than one, but they
	//has different names and macs
    //use the corresponding net device install function you use
    extern bool_t ModuleInstall_Enet(ptu32_t para);
    tagEnetCfg EnetCfg = {gNetDevName,MAC_RMII,AUTONEG_ON,MII_100BASET,
                          MII_FDX,NO_LOOPBACK,EXTERNAL_NONE,0,
                          {0x00,0x01,0x02,0x03,0x04,0x03}};
    ModuleInstall_Enet((ptu32_t)&EnetCfg);

    //make an link rout for the netdeb you installed
    //if you will malloc the ip from the dhcp server,make sure that the dhcp client has been enabled
    //actually,you could only install one rout for the same net dev
    if(DhcpAddClientTask(gNetDevName))
    {
       printk("%s:Add %s success\r\n",__FUNCTION__,gNetDevName);
    }
    else
    {
        printk("%s:Add %s failed\r\n",__FUNCTION__,gNetDevName);
    }

	return 0;
}

