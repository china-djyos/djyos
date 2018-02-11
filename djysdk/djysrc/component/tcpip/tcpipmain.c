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
    printf("*********DJY TCP/IP INIT START**********************\n\r");
    printf("Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.\r\n");
    printf("VERSION  :%s \r\n",CN_TCPIP_VERSION);
    printf("TIME     :%s \r\n",__TIME__);
    printf("DAT      :%s \r\n",__DATE__);
	//do the package manage module initialize
	extern bool_t PkgInit(void);
	if(false == PkgInit())
	{
		printf("%s:#PKG  Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printf("%s:#PKG  Module-------------------OK\n\r",__FUNCTION__);
	}
	//do the link init
	extern bool_t LinkInit(void);
    if(false == LinkInit())
    {
        printf("%s:#LINK Module-------------------BAD\n\r",__FUNCTION__);
        goto TCPIP_INITERR;
    }
    else
    {
        printf("%s:#LINK Module-------------------OK\n\r",__FUNCTION__);
    }
	//do the rout initialize
	extern bool_t RoutInit(void);
	if(false == RoutInit())
	{
		printf("%s:#ROUT Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printf("%s:#ROUT Module-------------------OK\n\r",__FUNCTION__);
	}
	//do the arp initialize
	extern bool_t ArpInit(void);
	if(false == ArpInit())
	{
		printf("%s:#ARP  Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printf("%s:#ARP  Module-------------------OK\n\r",__FUNCTION__);
	}
	
	//do the ip initialize
	extern  bool_t IpInit(void);
	if(false == IpInit())
	{
		printf("%s:#IP   Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printf("%s:#IP   Module-------------------OK\n\r",__FUNCTION__);
	}
	//do the transport layer initialize
	extern bool_t TPLInit(ptu32_t para);
	if(false == TPLInit((ptu32_t)0))
	{
		printf("%s:#TPL  Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printf("%s:#TPL  Module-------------------OK\n\r",__FUNCTION__);
	}

	//do the socket interface initialize
	extern bool_t SocketInit(ptu32_t para);
	if(false == SocketInit((ptu32_t)0))
	{
		printf("%s:#SOCK Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printf("%s:#SOCK Module-------------------OK\n\r",__FUNCTION__);
	}
	//install the tcp protocol
	extern bool_t TcpInit(ptu32_t para);
	if(false == TcpInit((ptu32_t)0))
	{
		printf("%s:#TCP  Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printf("%s:#TCP  Module-------------------OK\n\r",__FUNCTION__);
	}
	//install the udp protocol
	bool_t UdpInit(ptu32_t para);
	if(false == UdpInit((ptu32_t)0))
	{
		printf("%s:#UDP  Module-------------------BAD",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printf("%s:#UDP  Module-------------------OK\n\r",__FUNCTION__);
	}
	//install the icmp protocol
	extern bool_t IcmpInit(ptu32_t para);
	if(false == IcmpInit((ptu32_t)0))
	{
		printf("%s:#ICMP Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printf("%s:#ICMP Module-------------------OK\n\r",__FUNCTION__);
	}

	//add the loop device
	extern bool_t LoopInit(void);
	if(false == LoopInit())
	{
		printf("%s:#LOOP Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printf("%s:#LOOP Module-------------------OK\n\r",__FUNCTION__);
	}
	//add the ppp module
	extern bool_t PppInit(void);
	if(gUsePpp&&(false == PppInit()))
	{
		printf("%s:#PPP Module-------------------BAD\n\r",__FUNCTION__);
		goto TCPIP_INITERR;
	}
	else
	{
		printf("%s:#PPP Module-------------------OK\n\r",__FUNCTION__);
	}
    //add the tcpip service
    extern bool_t ServiceInit(void);
    if(false == ServiceInit())
    {
        printf("%s:#APPS Module-------------------BAD\n\r",__FUNCTION__);
        goto TCPIP_INITERR;
    }
    else
    {
        printf("%s:#APPS Module-------------------OK\n\r",__FUNCTION__);
    }
	printf("*********DJY TCP/IP INIT SUCCESS**********************\n\r");
	return 0;

TCPIP_INITERR:
	printf("*********DJY TCP/IP INIT  FAILED**********************\n\r");
	return -1;
}

