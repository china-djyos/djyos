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

//bit0 ����DHCP�ͻ��� bit1 ����DHCP server
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
