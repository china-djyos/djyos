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
//*******************************PKGMEM***************************************//
const u32 gNetPkgMemSize         = 0x4000 ;

//*******************************TPL******************************************//
const u32  gTplProtoNum          = 5      ; //the transmit protocol number supported

//*******************************UDP******************************************//
const bool_t   gUseUdp           = true   ; //use udp, and the icmp must be open
const u32  gUdpSockNum           = 10     ; //you could create mostly 10 udpv4 socket at the same time

//*******************************TCP******************************************//
const bool_t   gUseTcp           = true   ;
const u32  gTcpClientNum         = 10     ;
const u32  gTcpServerNum         = 5      ;

//*******************************SOCKET***************************************//
const u32  gSockNum              = 10     ;  //Determine how many socket you could create

//*******************************ARP******************************************//
const u32  gArpItemHashLen       = 32     ;  //determines the arp tab hash len

//*******************************TELNET***************************************//
const bool_t  gUseTelnet         = true   ;  //load the telnet module or not
const bool_t  gUseTelnetServer   = true   ;  //enable the telnet server or not
const bool_t  gUseTelnetClient   = false  ;  //enable the telnet client or not

//*******************************FTP******************************************//
const bool_t  gUseFtp            = true   ;  //load the ftp module or not
const bool_t  gUseFtpServer      = true   ;  //enable the ftp server or not
const bool_t  gUseFtpClient      = false  ;  //enable the ftp server or not
//******************************TFTP******************************************//
const bool_t  gUseTftp           = true   ;  //load the tftp module or not
const bool_t  gUseTftpServer     = true   ;  //enable the tftp server or not
const bool_t  gUseTftpClient     = true   ;  //enable the tftp server or not
//*******************************DHCP SERVER**********************************//
const bool_t   gUseDhcp          = true   ;  //load the dhcp module or not
const bool_t   gUseDhcpClient    = true   ;  //enable the dhcp client or not
const bool_t   gUseDhcpServer    = false  ;  //enable the dhcp server or not
const u32      gDhcpRenewTime    = 3600;            //one hour
const u32      gDhcpRebindTime   = 3600;            //one hour
const u32      gDhcpLeaseTime    = 3600;            //one hour
const u32      gDhcpServerIpNum  = 0x40;            //64
const u32      gDhcpIpMapBytes   = 0x08;            //which defines the bitmap lenth
const char    *pDhcpServerIp     = "192.168.0.100"; //DHCP SERVER IP
const char    *pDhcpRouterIp     = "192.168.0.100"; //ROUTER SERVER IP
const char    *pDhcpSubnetMask   = "255.255.255.0"; //USED TO MASK THE IP
const char    *pDhcpDns          = "192.168.0.100"; //DNSSERVER
const char    *pDhcpDomainName   = "www.djyos.com"; //domain name

//*******************************TCPIPAPPS************************************//
const u32      gTcpIpFsStack         = 0x800;     //if you use the file system in the apps.such as tftp ftp and so on

