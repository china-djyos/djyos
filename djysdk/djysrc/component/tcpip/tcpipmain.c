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

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "tcpipconfig.h"

#define CN_TCPIP_VERSION    "V1.2.0"
#define CN_TCPIP_AUTHOR     "luost@sznari.com,zhangqf1314@163.com"
bool_t TcpIpVersion(char *param)
{
    printf("Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.\r\n");
    printf("VERSION  :%s \r\n",CN_TCPIP_VERSION);
    printf("TIME     :%s \r\n",__TIME__);
    printf("DAT      :%s \r\n",__DATE__);
    printf("SUBMIT2  :Any idea, please contact with %s \r\n",CN_TCPIP_AUTHOR);
    printf("ChangeLog:\r\n");
    printf("FUNCTIONS:/ETHERNET/IPV4/TCP/UDP/ICMP/FTP/TFTP/SNTP/TELNET/DHCP/PING\r\n");
    return true;
}
// =============================================================================
// FUNCTION:this function used to initialize the tcpip stack
// PARA  IN:
// PARA OUT:
// RETURN  :
// INSTRUCT:
// =============================================================================
ptu32_t ModuleInstall_TcpIp(ptu32_t para)
{
    printk("*********DJY TCP/IP INIT START**********************\n\r");
    printk("Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.\r\n");
    printk("VERSION  :%s \r\n",CN_TCPIP_VERSION);
    printk("TIME     :%s \r\n",__TIME__);
    printk("DAT      :%s \r\n",__DATE__);
	//do the package manage module initialize
	extern bool_t PkgInit(void);
	if(false == PkgInit())
	{
		printk("%s:#PKG  Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printk("%s:#PKG  Module-------------------OK\n\r",__FUNCTION__);
	}
	//do the link init
	extern bool_t LinkInit(void);
    if(false == LinkInit())
    {
        printk("%s:#LINK Module-------------------BAD\n\r",__FUNCTION__);
        goto TCPIP_INITERR;
    }
    else
    {
        printk("%s:#LINK Module-------------------OK\n\r",__FUNCTION__);
    }
	//do the rout initialize
	extern bool_t RoutInit(void);
	if(false == RoutInit())
	{
		printk("%s:#ROUT Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printk("%s:#ROUT Module-------------------OK\n\r",__FUNCTION__);
	}
	//do the arp initialize
	extern bool_t ArpInit(void);
	if(false == ArpInit())
	{
		printk("%s:#ARP  Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printk("%s:#ARP  Module-------------------OK\n\r",__FUNCTION__);
	}
	
	//do the ip initialize
	extern  bool_t IpInit(void);
	if(false == IpInit())
	{
		printk("%s:#IP   Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printk("%s:#IP   Module-------------------OK\n\r",__FUNCTION__);
	}
	//do the transport layer initialize
	extern bool_t TPLInit(ptu32_t para);
	if(false == TPLInit((ptu32_t)0))
	{
		printk("%s:#TPL  Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printk("%s:#TPL  Module-------------------OK\n\r",__FUNCTION__);
	}

	//do the socket interface initialize
	extern bool_t SocketInit(ptu32_t para);
	if(false == SocketInit((ptu32_t)0))
	{
		printk("%s:#SOCK Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printk("%s:#SOCK Module-------------------OK\n\r",__FUNCTION__);
	}
	//install the tcp protocol
	extern bool_t TcpInit(ptu32_t para);
	if(false == TcpInit((ptu32_t)0))
	{
		printk("%s:#TCP  Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printk("%s:#TCP  Module-------------------OK\n\r",__FUNCTION__);
	}
	//install the udp protocol
	bool_t UdpInit(ptu32_t para);
	if(false == UdpInit((ptu32_t)0))
	{
		printk("%s:#UDP  Module-------------------BAD",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printk("%s:#UDP  Module-------------------OK\n\r",__FUNCTION__);
	}
	//install the icmp protocol
	extern bool_t IcmpInit(ptu32_t para);
	if(false == IcmpInit((ptu32_t)0))
	{
		printk("%s:#ICMP Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printk("%s:#ICMP Module-------------------OK\n\r",__FUNCTION__);
	}

	//add the loop device
	extern bool_t LoopInit(void);
	if(false == LoopInit())
	{
		printk("%s:#LOOP Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printk("%s:#LOOP Module-------------------OK\n\r",__FUNCTION__);
	}
	//add the ppp module
	extern bool_t PppInit(void);
	if(gUsePpp&&(false == PppInit()))
	{
		printk("%s:#PPP Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printk("%s:#PPP Module-------------------OK\n\r",__FUNCTION__);
	}
    //add the tcpip service
    extern bool_t ServiceInit(void);
    if(false == ServiceInit())
    {
        printk("%s:#APPS Module-------------------BAD\n\r",__FUNCTION__);
        goto TCPIP_INITERR;
    }
    else
    {
        printk("%s:#APPS Module-------------------OK\n\r",__FUNCTION__);
    }
	printk("*********DJY TCP/IP INIT SUCCESS**********************\n\r");
	return 0;

TCPIP_INITERR:
	printk("*********DJY TCP/IP INIT  FAILED**********************\n\r");
	return -1;
}

