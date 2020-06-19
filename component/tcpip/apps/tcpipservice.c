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
#include "dbug.h"
#include "../component_config_tcpip.h"
#include <shell.h>


//extern bool_t DHCP_ServiceDhcpcInit(void);
//extern bool_t DHCP_ServiceDhcpdInit(void);
extern bool_t ServiceDnsInit(void);
extern bool_t PING_ServicePingInit(void);
extern bool_t SNTP_ServiceSNTP_Init(void);
extern bool_t FTP_ServiceInit(void);
extern bool_t Telnet_ServiceInit(void);
extern bool_t TFTP_ServiceInit(void);

//bit0 开启DHCP客户端 bit1 开启DHCP server
//__attribute__((weak))  u32 Get_DhcpInitflag(void)
//{
//    return 1;
//}


// dhcp_mode: 0->dhcp_client; 1->dhcp_server; -1: not use dhcp
//bool_t DHCP_ModeInit(int dhcp_mode)
//{
//    bool_t result = true;
//#if  (CFG_MODULE_ENABLE_DHCP == true)
//    if(dhcp_mode==0 &&CFG_DHCPC_ENABLE)
//    {
//        printf("------dhcp client---------\r\n");
//        if((false == DHCP_ServiceDhcpcInit()))
//        {
//            error_printf("tcpip","###err: service dhcpc failed");
//            result = false;
//        }
//    }
//    if(dhcp_mode==1&&CFG_DHCPD_ENABLE)
//    {
//        printf("------dhcp server---------\r\n");
//        if((false == DHCP_ServiceDhcpdInit()))
//        {
//            error_printf("tcpip","###err: service dhcpd failed");
//            result = false;
//        }
//    }
//#endif
//    return result;
//}

//THIS IS THE TCP IP SERVICE LOAD MODULE
bool_t TCPIP_ServiceInit(void)
{
    bool_t result = true;

#if (CFG_MODULE_ENABLE_FTP == true)
    if(false == FTP_ServiceInit())
    {
        error_printf("tcpip","###err: service ftp failed");
        result = false;
    }
#endif

#if  (CFG_MODULE_ENABLE_TELNET == true)
    if(false == Telnet_ServiceInit())
    {
        error_printf("tcpip","###err: service telnet failed\n\r");
        result = false;
    }
#endif

#if  (CFG_MODULE_ENABLE_TFTP == true)
    if(false == TFTP_ServiceInit())
    {
        error_printf("tcpip","###err: service tftp failed");
        result = false;
    }
#endif
    PING_ServicePingInit();
    return result;
}
