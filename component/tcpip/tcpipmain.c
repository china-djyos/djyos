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

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <shell.h>
#include "dbug.h"
#include "component_config_tcpip.h"

#define CN_TCPIP_VERSION    "V1.2.0"

//static bool_t __TcpIpVersion(char *param)
bool_t tcpipver(char *param)
{
    param = param;
    info_printf("tcpip","Copyright (c) 2018, Djyos Open source Development team. All rights reserved.");
    info_printf("tcpip","VERSION  :%s ",CN_TCPIP_VERSION);
    info_printf("tcpip","TIME     :%s ",__TIME__);
    info_printf("tcpip","DATE     :%s ",__DATE__);
    return true;
}


//we need a function to do the format result
static void __LoadLog(const char *name,bool_t ret)
{
    info_printf("tcpip","LOAD:%-12s------------%s",name,ret?"SUCCESS":"FAILURE");
    return;
}
// =============================================================================
// FUNCTION:this function used to initialize the tcpip stack
// PARA  IN:
// PARA OUT:
// RETURN  :
// INSTRUCT:
// =============================================================================
bool_t ModuleInstall_TcpIp(void)
{
    bool_t ret;
#if CFG_MODULE_ENABLE_EXTSTACK
    //do the NETDEVICE LAYER initialize
    extern bool_t NetDev_Init(void);
    ret = NetDev_Init();
    __LoadLog("NETDEV",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
    extern bool_t RouterInit(void);
    ret = RouterInit();
    __LoadLog("ROUTER",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
    //do the transport layer initialize
    extern bool_t TPLInit(void);
    ret = TPLInit();
    __LoadLog("TPL",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
    extern bool_t ExtStackInit(void);
    ret = ExtStackInit();
    __LoadLog("ExtStackInit",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }

    //do the socket interface initialize
    extern bool_t SocketInit(void);
    ret = SocketInit();
    __LoadLog("SOCKET",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
#else
    tcpipver(NULL);

    extern bool_t OsArchInit();
    ret = OsArchInit();
    __LoadLog("OSARCH",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
    //do the package manage module initialize
    extern bool_t PkgModuleInit(void);
    ret = PkgModuleInit();
    __LoadLog("PKG",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
    //do the link init
    extern bool_t Link_Init(void);
    ret = Link_Init();
    __LoadLog("LINK",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
    //FOR THE LINKER LAYER HAS BEEN INITIALIZED,SO ADD SOME LINKER METHOD HERE
    extern bool_t LinkEthernetInit(void);
    ret = LinkEthernetInit();
    __LoadLog("LINKETERNET",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
    extern bool_t Link_RawInit(void);
    ret = Link_RawInit();
    __LoadLog("LINKRAW",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
    //do the NETDEVICE LAYER initialize
    extern bool_t NetDev_Init(void);
    ret = NetDev_Init();
    __LoadLog("NETDEV",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
    extern bool_t RouterInit(void);
    ret = RouterInit();
    __LoadLog("ROUTER",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
    //DO THE LINKER PROTOCOL INITIALIZE
    extern bool_t ArpInit(void);
    ret = ArpInit();
    __LoadLog("ARP",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
    //do the ip initialize
    extern  bool_t IpInit(void);
    ret = IpInit();
    __LoadLog("IP",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
    //do the transport layer initialize
    extern bool_t TPLInit(void);
    ret = TPLInit();
    __LoadLog("TPL",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
    //do the socket interface initialize
    extern bool_t SocketInit(void);
    ret = SocketInit();
    __LoadLog("SOCKET",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
#if (CFG_MODULE_ENABLE_TCP == true)
  //install the tcp protocol
    extern bool_t TcpInit(void);
    ret = TcpInit();
    __LoadLog("TCP",ret);
    if(false == ret)
    {
      goto TCPIP_INITERR;
    }
#endif

    //install the udp protocol
#if (CFG_MODULE_ENABLE_UDP == true)
    extern bool_t UdpInit(void);
    ret = UdpInit();
    __LoadLog("UDP",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
#endif

    //install the icmp protocol
    extern bool_t IcmpInit(void);
    ret = IcmpInit();
    __LoadLog("ICMP",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
    //add the loop device
    extern bool_t LoopInit(void);
    ret = LoopInit();
    __LoadLog("LOOPDEV",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
    //add the ppp module
#if (CFG_MODULE_ENABLE_PPP == true)
        extern bool_t PppInit(void);
        ret = PppInit();
        __LoadLog("PPP",ret);
        if(false == ret)
        {
            goto TCPIP_INITERR;
        }
#endif
        //add the tcpip service
    extern bool_t TCPIP_ServiceInit(void);
    ret = TCPIP_ServiceInit();
    __LoadLog("SERVICE",ret);
    if(false == ret)
    {
        goto TCPIP_INITERR;
    }
    info_printf("tcpip","*********DJY TCP/IP INIT SUCCESS**********************");

    return ret;
#endif

TCPIP_INITERR:
    error_printf("tcpip","*********DJY TCP/IP INIT  FAILED**********************");
    return ret;
}
ADD_TO_ROUTINE_SHELL(tcpipver,tcpipver,"usage:tcpipver");
