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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
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
