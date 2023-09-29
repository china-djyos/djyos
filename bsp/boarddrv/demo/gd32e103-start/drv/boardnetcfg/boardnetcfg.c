#include <stdint.h>
#include <get_cpuid.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern void ModuleInstall_InitNet( );
//    ModuleInstall_InitNet( );
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"network config"//��������
//parent:"tcpip"     //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:later               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"tcpip","cpu onchip ETH" //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_NETWORK_CONFIG == false )
//#warning  " network_config  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_NETWORK_CONFIG    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100
//%$#@enum,true,false
#define CFG_STATIC_IP       true            //"ʹ�þ�̬IP?",
//%$#@string,1,32
#define CFG_SELECT_NETCARD  "STM32F4_ETH"   //"��������",������ѡ�е��������������õ�������ͬ
//%$#@string,7,15
#define CFG_MY_IPV4         "192.168.0.179" //"��̬IP",
#define CFG_MY_SUBMASK      "255.255.255.0" //"��������",
#define CFG_MY_GATWAY       "192.168.0.1"   //"����",
#define CFG_MY_DNS          "192.168.0.1"   //"��ѡDNS",
#define CFG_MY_DNSBAK       "192.168.0.1"   //"����DNS",
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

//please refers the following function in the module-trim in proper place.
//make sure that that os kernel has been initialize ok and the interrupt system intialize ok
//make sure that that board net hardware has initialized and could work ok
void ModuleInstall_InitNet(void)   //static ip example
{
    u32  signature[3];
#if CFG_STATIC_IP == 1
    u32 hop,subnet,ip,submask,dns,dnsbak;
    tagRouterPara para;
    struct NetDev *NetDev;

    NetDev = NetDev_GetHandle(CFG_SELECT_NETCARD);
    if(NetDev == NULL)          //����δ��װ
    {
        printk("%s netcard not install\r\n",CFG_SELECT_NETCARD);
        return;
    }

    memset(&para,0,sizeof(para));
    ip      = inet_addr(CFG_MY_IPV4);
    submask = inet_addr(CFG_MY_SUBMASK);
    hop     = inet_addr(CFG_MY_GATWAY);
    dns     = inet_addr(CFG_MY_DNS);
    dnsbak  = inet_addr(CFG_MY_DNSBAK);

    subnet = ip & submask;
    para.ver = EN_IPV_4;
    para.ifname = CFG_SELECT_NETCARD;
    para.mask = &submask;
    para.net = &subnet;
    para.host = &ip;
//  para.hop = &hop;
//  para.dns = &dns;
//  para.dnsbak = &dnsbak;
    para.prior = CN_ROUT_PRIOR_UNI;
    para.flags = 0;

    NetDev_SetDns(EN_IPV_4,NetDev, &dns, &dnsbak);
    NetDev_SetGateway(EN_IPV_4,NetDev, &hop);

    if(RouterCreate(&para))
    {
        printk("%s CreateRout:%s success\r\n",CFG_SELECT_NETCARD,CFG_MY_IPV4);
    }
    else
    {
        printk("%s CreateRout:%s failed\r\n",CFG_SELECT_NETCARD,CFG_MY_IPV4);
    }

    //��һ��·�ɣ��������������ã�����CPU ID �������������ַ�������ַ�� 192.168.1
    //WE WILL ADD A ROUT DIFFERENT FOR EACH DEVICE USE THE CPU SIGNATURE
    //USE THE NET:192.168.1.xx
    u8 value8 = 0;
    GetCpuID(&signature[0],&signature[1],&signature[2]);
    value8 = +((u8)signature[0]>>0)+((u8)signature[0]>>8)+((u8)signature[0]>>16)+((u8)signature[0]>>24);
    if((value8==0)||(value8==1)||(value8==255))
    {
        value8=253;
    }
    u32 value32 = 0;
    value32 = inet_addr("192.168.1.0");
    value32 = ntohl(value32);
    value32 =(value32&0xffffff00) + value8;
    ip      = htonl(value32);
    submask = inet_addr("255.255.255.0");
//  hop  = inet_addr("192.168.1.1");

    subnet = ip & submask;
    para.ver = EN_IPV_4;
    para.ifname = CFG_SELECT_NETCARD;
    para.mask = &submask;
    para.net = &subnet;
    para.host = &ip;
//  para.hop = &hop;
    para.prior = CN_ROUT_PRIOR_UNI;
    para.flags = 0;

    if(RouterCreate(&para))
    {
        printk("%s CreateRout success\r\n",CFG_SELECT_NETCARD);
    }
    else
    {
        printk("%s CreateRout failed\r\n",CFG_SELECT_NETCARD);
    }

#else
    //�����������ϵͳ��ϣ���������ӣ�����ʹ������ǰ��IP����ÿ�����������뵽��IP
    //�������ļ��У����õڶ���������
    if(DHCP_AddClientTask(CFG_SELECT_NETCARD,0))
    {
       printk("Add %s success\r\n",CFG_SELECT_NETCARD);
    }
    else
    {
        printk("Add %s failed\r\n",CFG_SELECT_NETCARD);
    }

#endif

    return ;
}


