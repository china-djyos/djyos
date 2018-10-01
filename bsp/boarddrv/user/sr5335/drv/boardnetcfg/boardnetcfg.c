//-----------------------------------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
//this file create an module that leads you how to initialize the net to match your
//board.if you want to configure the tcpip stack itself,please  cpy the tcpipconfig.cfg
//file to your user directory and modify the data as the instruction

#include <sys/socket.h>
#include "cpu_peri.h"     //for the k70 net device in the soc
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern void ModuleInstall_NetStaticIP( );
//  ModuleInstall_NetStaticIP( );
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"netconfig"    //��������
//parent:"tcpip"                //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���             //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                   //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"cpu_peri_eth"     //������������������������none����ʾ�������������
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
#ifndef CFG_STATIC_IP   //****�������Ƿ��Ѿ����ú�
#warning    ���������á��������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100
//%$#@enum,true,false
#define CFG_STATIC_IP       true            //"IP����",true=ʹ�þ�̬IP��false=��̬IP
//%$#@string,1,16
#define CFG_NETCARD_NAME    "SR5333_ETH"    //"������",
//%$#@string,7,15
#define CFG_MY_IPV4         "192.168.0.179" //"��̬IP",
//%$#@string,7,15
#define CFG_MY_SUBMASK      "255.255.255.0" //"��������",
//%$#@string,7,15
#define CFG_MY_GATWAY       "192.168.0.1"   //"����",
//%$#@string,7,15
#define CFG_MY_DNS          "192.168.0.1"   //"DNS",
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****�������ý���

static u8   gc_NetMac[CN_MACADDR_LEN] ={0x00,0x02,0x03,0x04,0x05,0x06};
//static u16   gVlan88E6060[] = {0x20,0x20,0x20,0x20,0x20,0x1F};

extern in_addr_t inet_addr(const char *addr);

//please refers the following function in the module-trim in proper place.
//make sure that that os kernel has been initialize ok and the interrupt system intialize ok
//make sure that that board net hardware has initialized and could work ok
void ModuleInstall_NetStaticIP(void)   //static ip example
{
    //install the tcpip stack
    extern bool_t ModuleInstall_TcpIp(void);
    ModuleInstall_TcpIp( );
    //install the net device you used,you could use more than one, but they
    //has different names and macs
    //use the corresponding net device install function you use
    extern bool_t ModuleInstall_ETH(const char *devname, u8 *mac,\
            bool_t loop,u32 loopcycle,\
            bool_t (*rcvHook)(u8 *buf, u16 len));
    ModuleInstall_ETH(CFG_NETCARD_NAME,gc_NetMac,false,1*mS,NULL);
//    NetDevFilterSet(gc_NetCardName,EN_NETDEV_FRAME_NOPKG,10000,1000*mS,10*1000*mS);
//    NetDevFilterEnable(gc_NetCardName,EN_NETDEV_FRAME_NOPKG,true);
//    NetDevFilterSet(gc_NetCardName,EN_NETDEV_FRAME_BROAD,100,100*1000*mS,1*1000*mS);
//    NetDevFilterEnable(gc_NetCardName,EN_NETDEV_FRAME_BROAD,true);
    //make an link rout for the netdeb you installed,you could install more
    //than one link rout for the same net device with different net address
    tagHostAddrV4  ipv4addr;
    //we use the static ip we like
    memset((void *)&ipv4addr,0,sizeof(ipv4addr));
    ipv4addr.ip      = inet_addr(CFG_MY_IPV4);
    ipv4addr.submask = inet_addr(CFG_MY_SUBMASK);
    ipv4addr.gatway  = inet_addr(CFG_MY_GATWAY);
    ipv4addr.dns     = inet_addr(CFG_MY_DNS);
    ipv4addr.broad   = inet_addr("192.168.0.255");
    if(RoutCreate(CFG_NETCARD_NAME,EN_IPV_4,(void *)&ipv4addr,CN_ROUT_NONE))
    {
       printk("%s:Add %s success\r\n",__FUNCTION__,CFG_NETCARD_NAME);
    }
    else
    {
        printk("%s:Add %s failed\r\n",__FUNCTION__,CFG_NETCARD_NAME);
    }

    //do the 88e6060 chip set
//  extern ptu32_t LS88e6060Init(u16 vlan[6]);
}

ptu32_t ModuleInstall_NetDynamicIP(ptu32_t para)   //use the dhcp to malloc an ip dynamic
{
    //install the tcpip stack
    extern bool_t ModuleInstall_TcpIp(void);
    ModuleInstall_TcpIp( );
    //install the net device you used,you could use more than one, but they
    //has different names and macs
    //use the corresponding net device install function you use
    extern bool_t ModuleInstall_ETH(const char *devname, u8 *mac,\
            bool_t loop,u32 loopcycle,\
            bool_t (*rcvHook)(u8 *buf, u16 len));
    ModuleInstall_ETH(CFG_NETCARD_NAME,gc_NetMac,false,1*mS,NULL);
    //make an link rout for the netdeb you installed
    //if you will malloc the ip from the dhcp server,make sure that the dhcp client has been enabled
    //actually,you could only install one rout for the same net dev
    if(DhcpAddClientTask(CFG_NETCARD_NAME))
    {
       printk("%s:Add %s success\r\n",__FUNCTION__,CFG_NETCARD_NAME);
    }
    else
    {
        printk("%s:Add %s failed\r\n",__FUNCTION__,CFG_NETCARD_NAME);
    }

    //do the 88e6060 chip set
//  extern ptu32_t LS88e6060Init(u16 vlan[6]);
//  LS88e6060Init(gVlan88E6060);

    return 0;
}

