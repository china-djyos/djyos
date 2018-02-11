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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
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

