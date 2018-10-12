//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// Copyright (c) 2018 ����Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��:tcpip
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:ģ�������ļ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern bool_t ModuleInstall_TcpIp(void);
//    ModuleInstall_TcpIp( );
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"tcpip"        //tcpipЭ��ջ
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:�������             //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                   //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","heap","devfile",             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_NETPKG_MEMSIZE   //****�������Ƿ��Ѿ����ú�
#warning    tcpip�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,,,
#define     CFG_NETPKG_MEMSIZE          0x4000  //"���ݰ��������ߴ�"
//*******************************ARP******************************************//
#define     CFG_ARP_HASHLEN             32      //"ARP��ϣ����"��ռ��һ��ָ��
//*******************************TPL******************************************//
#define     CFG_TPL_PROTONUM            5       //"֧�ֵĴ���Э����"��ռ��һ�� tagTplProtoItem �ṹ
//*******************************UDP******************************************//
//%$#@enum,true,false,
#define     CFG_UDP_ENABLE              true    //"udpЭ��ʹ��"
//%$#@num,,,
#define     CFG_UDP_CBNUM               10      //"UDP socket����ֵ"��ռ��һ�� tagUdpCB �ṹ
#define     CFG_UDP_HASHLEN             4       //"udp socket ��ϣ����"��ռ��һ��ָ��
//*******************************TCP******************************************//
//%$#@enum,true,false,
#define     CFG_TCP_ENABLE              true    //"tcpЭ��ʹ��"
#define     CFG_TCP_REORDER             true    //"TCP��������ʹ��",��Դ����Ŵ�
//%$#@num,,,
#define     CFG_TCP_CCBNUM              10      //"tcp �ͻ�������ֵ"��ռһ�� ָ�� �� tagCCB
#define     CFG_TCP_SCBNUM              5       //"tcp ����������ֵ"����ռһ�� ָ�� �� tagSCB
//*******************************SOCKET***************************************//
#define     CFG_SOCKET_NUM              10      //"socket����ֵ"��ռһ�� tagItem �ṹ
//*******************************TELNET***************************************//
//%$#@enum,true,false,
#define     CFG_TELNET_ENABLE           true    //"telnet ʹ��"
#define     CFG_TELNETD_ENABLE          true    //"telnet ������ʹ��"
#define     CFG_TELNETC_ENABLE          false   //"telnet �ͻ���ʹ��"
//*******************************FTP******************************************//
#define     CFG_FTP_ENABLE              true    //"ftp ʹ��"
#define     CFG_FTPD_ENABLE             true    //"ftp ������ʹ��"
#define     CFG_FTPC_ENABLE             false   //"ftp �ͻ���ʹ��"

//******************************TFTP******************************************//
#define     CFG_TFTP_ENABLE             true    //"tftp ʹ��"
#define     CFG_TFTPD_ENABLE            false   //"tftp ������ʹ��"
#define     CFG_TFTPC_ENABLE            true    //"tftp �ͻ���ʹ��"
#define     CN_TFTP_PATHDEFAULT       "/efs/"   //TFTPĬ�Ϲ���Ŀ¼
//*******************************DHCP SERVER**********************************//
#define     CFG_DHCP_ENABLE             true    //"DHCP ʹ��"
#define     CFG_DHCPD_ENABLE            false   //"DHCP ������ʹ��"
#define     CFG_DHCPC_ENABLE            true    //"DHCP �ͻ���ʹ��"
//%$#@num,,,
#define     CFG_DHCP_RENEWTIME          3600    //"renew timer",����
#define     CFG_DHCP_REBINDTIME         3600    //"rebind timer",����
#define     CFG_DHCP_LEASETIME          3600    //"lease timer",����
#define     CFG_DHCPD_IPNUM             0x40    //"IP�سߴ�",64
//%$#@string,7,32,
#define     CFG_DHCPD_IPSTART           "192.168.0.2"    //"DHCP��ʼIP",
#define     CFG_DHCPD_SERVERIP          "192.168.0.253"  //"DHCP SERVER IP"
#define     CFG_DHCPD_ROUTERIP          "192.168.0.253"  //"DHCP ROUTER SERVER IP"
#define     CFG_DHCPD_NETIP             "255.255.255.0"  //"DHCP MASK IP"
#define     CFG_DHCPD_DNS               "192.168.0.253"  //"DHCP DNSSERVER IP"
//%$#@string,1,32,
#define     CFG_DHCPD_DOMAINNAME       ""               //"DHCP domain name"
//*******************************PPP******************************************//
#define     CN_PPP_ENABLE             1         //�Ƿ���Ҫ ppp Э��
//%$#@select
#define CFG_MYCONFIG6           //"name",�ҵ�����
//%$#@free
#define CFG_MYCONFIG7   CN_CONFIG           //�ҵ�����
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure


