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



extern bool_t ServiceDhcpcInit(void);
extern bool_t ServiceDhcpdInit(void);
extern bool_t ServiceDnsInit(void);
extern bool_t ServicePingInit(void);
extern bool_t ServiceSntpInit(void);
extern bool_t ServiceFtpInit(void);
extern bool_t ServiceInit_Telnetd(void);
extern bool_t ServiceTftpInit(void);

//THIS IS THE TCP IP SERVICE LOAD MODULE
bool_t ServiceInit(void)
{
    bool_t result = true;

    if((CFG_DHCPC_ENABLE)&&(false == ServiceDhcpcInit()))
    {
        error_printf("tcpip","###err: service dhcpc failed");
        result = false;
    }
    if((CFG_DHCPD_ENABLE)&&(false == ServiceDhcpdInit()))
    {
        error_printf("tcpip","###err: service dhcpd failed");
        result = false;
    }

    if(false == ServiceDnsInit())
    {
        error_printf("tcpip","###err: service dns failed");
        result = false;
    }
    if(false == ServicePingInit())
    {
        error_printf("tcpip","###err: service ping failed");
        result = false;
    }
    if(false == ServiceSntpInit())
    {
        error_printf("tcpip","###err: service sntp failed");
        result = false;
    }

    if((CFG_FTP_ENABLE)&&(false == ServiceFtpInit()))
    {
        error_printf("tcpip","###err: service ftp failed");
        result = false;
    }
    if((CFG_TELNET_ENABLE)&&(false == ServiceInit_Telnetd()))
    {
        error_printf("tcpip","###err: service telnet failed\n\r");
        result = false;
    }
    if((CFG_TFTP_ENABLE)&&(false == ServiceTftpInit()))
    {
        error_printf("tcpip","###err: service tftp failed");
        result = false;
    }
    return result;
}
