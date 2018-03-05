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

#include "../tcpipconfig.h"

extern bool_t ServiceDhcpcInit(ptu32_t para);
extern bool_t ServiceDhcpdInit(ptu32_t para);
extern bool_t ServiceDnsInit(ptu32_t para);
extern bool_t ServiceFtpInit(ptu32_t para);
extern bool_t ServicePingInit(ptu32_t para);
extern bool_t ServiceSntpInit(ptu32_t para);
extern bool_t ServiceTelnetInit(ptu32_t para);
extern bool_t ServiceTftpInit(ptu32_t para);

//THIS IS THE TCP IP SERVICE LOAD MODULE
bool_t ServiceInit(void)
{
    bool_t result = true;

    if((gUseDhcpClient)&&(false == ServiceDhcpcInit(0)))
    {
        printf("###err: service dhcpc failed\n\r");
        result = false;
    }
    if((gUseDhcpServer)&&(false == ServiceDhcpdInit(0)))
    {
        printf("###err: service dhcpd failed\n\r");
        result = false;
    }
    if(false == ServiceDnsInit(0))
    {
        printf("###err: service dns failed\n\r");
        result = false;
    }
    if((gUseFtp)&&(false == ServiceFtpInit(0)))
    {
        printf("###err: service ftp failed\n\r");
        result = false;
    }
    if(false == ServicePingInit(0))
    {
        printf("###err: service ping failed\n\r");
        result = false;
    }
    if(false == ServiceSntpInit(0))
    {
        printf("###err: service sntp failed\n\r");
        result = false;
    }
    if((gUseTelnet)&&(false == ServiceTelnetInit(0)))
    {
        printf("###err: service telnet failed\n\r");
        result = false;
    }
    if((gUseTftp)&&(false == ServiceTftpInit(0)))
    {
        printf("###err: service tftp failed\n\r");
        result = false;
    }
    return result;
}
