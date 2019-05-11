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
#include <sys/socket.h>
#include "param_config.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern void ModuleInstall_InitNet( );
//    ModuleInstall_InitNet( );
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"board driver net config"//��������
//parent:"component tcpip"     //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"cpu driver eth","chip driver lan8720","component tcpip"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_STATIC_IP   //****�������Ƿ��Ѿ����ú�
#warning    ���������á��������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100
//%$#@enum,true,false
#define CFG_STATIC_IP       false            //"IP����",true=ʹ�þ�̬IP��false=��̬IP
//%$#@string,1,16
#define CFG_NETCARD_NAME    "BK7251_WIFI"    //"������",
//%$#@string,7,15
#define CFG_MY_IPV4         "192.168.0.179" //"��̬IP",
#define CFG_MY_SUBMASK      "255.255.255.0" //"��������",
#define CFG_MY_GATWAY       "192.168.0.1"   //"����",
#define CFG_MY_DNS          "192.168.0.1"   //"DNS",
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****�������ý���

#define CFG_DHCPD_IPV4      "192.168.0.253"
#define CFG_DHCPD_SUBMASK   "255.255.255.0"
#define CFG_DHCPD_GATWAY    "192.168.0.1"
#define CFG_DHCPD_DNS       "192.168.0.1"   //"DNS"

static u8   gc_NetMac[CN_MACADDR_LEN] = DEFAULT_MAC_ADDR;


//please refers the following function in the module-trim in proper place.
//make sure that that os kernel has been initialize ok and the interrupt system intialize ok
//make sure that that board net hardware has initialized and could work ok
void ModuleInstall_InitNet(void)   //static ip example
{
    extern bool_t ModuleInstall_Wifi(const char *devname, u8 *macaddress,\
                              bool_t loop,u32 loopcycle,\
                              bool_t (*rcvHook)(u8 *buf, u16 len));
    ModuleInstall_Wifi(CFG_NETCARD_NAME,gc_NetMac,false,1*mS,NULL);

    tagHostAddrV4  ipv4addr;
    //we use the static ip we like
    memset((void *)&ipv4addr,0,sizeof(ipv4addr));
    ipv4addr.ip      = inet_addr(CFG_DHCPD_IPV4);
    ipv4addr.submask = inet_addr(CFG_DHCPD_SUBMASK);
    ipv4addr.gatway  = inet_addr(CFG_DHCPD_GATWAY);
    ipv4addr.dns     = inet_addr(CFG_DHCPD_DNS);
    ipv4addr.broad   = inet_addr("255.255.255.255");
    if(RoutCreate(CFG_NETCARD_NAME,EN_IPV_4,(void *)&ipv4addr,CN_ROUT_NONE))
    {
        printk("%s:CreateRout:%s:%s success\r\n",__FUNCTION__,CFG_NETCARD_NAME,inet_ntoa(ipv4addr.ip));
    }
    else
    {
        printk("%s:CreateRout:%s:%s failed\r\n",__FUNCTION__,CFG_NETCARD_NAME,inet_ntoa(ipv4addr.ip));
    }

//    if(DhcpAddClientTask(CFG_NETCARD_NAME))
//    {
//       printk("%s:Add %s success\r\n",__FUNCTION__,CFG_NETCARD_NAME);
//    }
//    else
//    {
//        printk("%s:Add %s failed\r\n",__FUNCTION__,CFG_NETCARD_NAME);
//    }

}

