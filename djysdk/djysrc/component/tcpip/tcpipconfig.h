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


#ifndef __TCPIP_CONFIG_H
#define __TCPIP_CONFIG_H

//*******************************PKGMEM***************************************//
extern const u32 gNetPkgMemSize         ; //the stack package memory size

//*******************************IP*******************************************//
extern const u32    gIpRcvTaskPrior        ; //the ip processor task prior
extern const u32    gIpRcvTaskStack        ; //ip receive task stack size
extern const bool_t gEnableIpAssemble      ; //ipv4 could receive the fragment package
extern const bool_t gEnableIpFragment      ; //ipv4 could send fragment package
//*******************************ARP******************************************//
extern const u32  gArpItemHashLen       ;  //determines the arp tab hash len
extern const u32  gArpTaskPrior         ;  //the arp processor task
extern const u32  gArpTaskStack         ;  //the arp task stack size

//*******************************TPL******************************************//
extern const u32  gTplProtoNum          ; //the transmit protocol number supported

//*******************************UDP******************************************//
extern const bool_t   gUseUdp           ; //use udp, and the icmp must be open
extern const u32  gUdpSockNum           ; //you could create mostly 10 udpv4 socket at the same time
extern const u32  gUdpHashLen           ; //udp hash search tab
//*******************************TCP******************************************//
extern const bool_t   gUseTcp           ; //use tcp
extern const bool_t   gEnableTcpReoder  ; //enable the tcp reorder or not
extern const u32  gTcpClientNum         ; //tcp maxi client
extern const u32  gTcpServerNum         ; //tcp maxi server
extern const u32  gTcpTickTaskPrior     ; //tcp tick task prior
extern const u32  gTcpTickTaskStack     ; //the tcp tick task stack size
extern const int  gTcpSndBufSizeDefault ; //the tcp buf size default
extern const int  gTcpRcvBufSizeDefault ; //the tcp buf size default
extern const u16  gTcpMssDefault        ; //the tcp mss default used to


//*******************************SOCKET***************************************//
extern const u32  gSockNum              ;  //Determine how many socket you could create

//*******************************TELNET***************************************//
extern const bool_t  gUseTelnet         ;  //load the telnet module or not
extern const bool_t  gUseTelnetServer   ;  //enable the telnet server or not
extern const bool_t  gUseTelnetClient   ;  //enable the telnet client or not
extern const u32     gTelnetAcceptStack ;  //telnet accept task stack size
extern const u32     gTelnetProcessStack;  //telnet processor task stack size,if use fs,please configure it more
extern const u32     gTelnetAcceptPrior ;  //telnet accept task prior
extern const u32     gTelnetProcessPrior;  //telnet processor task prior

//*******************************FTP******************************************//
extern const bool_t  gUseFtp;              //load the ftp module or not
extern const bool_t  gUseFtpServer      ;  //enable the ftp server or not
extern const bool_t  gUseFtpClient      ;  //enable the ftp server or not
extern const u32     gFtpAcceptStack    ;  //the ftp accept task stack size
extern const u32     gFtpProcessStack   ;  //the ftp processor task stack,use fs,configure more
extern const u32     gFtpAcceptPrior    ;  //ftp accept task prior
extern const u32     gFtpProcessPrior   ;  //ftp processor task prior

//******************************TFTP******************************************//
extern const bool_t  gUseTftp           ;  //load the tftp module or not
extern const bool_t  gUseTftpServer     ;  //enable the tftp server or not
extern const bool_t  gUseTftpClient     ;  //enable the tftp server or not
extern const char   *pTftpWSDefault     ;  //set the default tftp work space
extern const u32     gTftpServerStack   ;  //tftp server stack size,use fs, pay more
extern const u32     gTftpServerPrior   ;  //tftp server prior


//*******************************DHCP SERVER**********************************//
extern const bool_t   gUseDhcp;             //load the dhcp module or not
extern const bool_t   gUseDhcpClient;       //enable the dhcp client or not
extern const bool_t   gUseDhcpServer;       //enable the dhcp server or not
extern const u32      gDhcpRenewTime ;      //one hour
extern const u32      gDhcpRebindTime ;     //one hour
extern const u32      gDhcpLeaseTime  ;     //one hour
extern const u32      gDhcpServerIpNum ;    //64
extern const char    *pDhcpServerIp   ;     //DHCP SERVER IP
extern const char    *pDhcpRouterIp   ;     //ROUTER SERVER IP
extern const char    *pDhcpSubnetMask ;     //USED TO MASK THE IP
extern const char    *pDhcpDns        ;     //DNSSERVER
extern const char    *pDhcpDomainName ;     //domain name

//*******************************PPP******************************************//
extern const bool_t  gUsePpp            ;  //load the ppp module or not
extern const bool_t  gPppStackSize      ;  //the ppp task stack size
extern const u32     gPppTaskPrior      ;  //the ppp task prior
extern const bool_t  gPppDevLimit       ;  //how many ppp device could be add
extern const char   *pPppEvttName       ; //the ppp task name(evtt name)

#endif /* __TCPIP_CONFIG_H */
