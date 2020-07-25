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
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netbsp.h>
#include "dbug.h"
#include <shell.h>
#include "../common/router.h"
#include "../common/netdev.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"router"      //·��Э��
//parent:"tcpip"       //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","heap","device file system"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_ROUTER == false )
//#warning  " router  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_ROUTER    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,,,
#define CFG_IP_STRMAX           20 //���·����Ŀ��
//%$#@enum,true,false,
//%$#@string,1,256,
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//���ͱ���ʱ���ȴ�·�ɱ�ƥ��Ŀ���ַ��������ɹ�������Ŀ���Ƿ����������������ƥ�䣬��ʹ
//�ñ���·�ɣ�����ʹ��Ĭ��·�ɣ�����Ѿ����ã���
//�յ�����ʱ�����Ŀ�ĵ�ַ�ǲ���������������ܣ�������ǰת����

//ʹ���� Unix ��־
typedef struct
{
    u16 G_Gateway :1;   //G��־��·��ָ������
    u16 U_Active :1;    //U��־��·���ǻ�ġ�
    u16 R_Restore :1;   //R��־���ָ���̬·�ɲ����ı���
    u16 IPV4 :1;        //1=IPV4,0=IPV6
    u16 S  :1;          //static or not
    u16 C:1  ;          //clone or not
    u16 B:1;            //broadcast or not
    u16 M:1;            //multicast or not
}tagRoutFlag;

typedef union
{
    tagRoutFlag bits;
    u16         value;
}tagURoutFlag;

typedef struct
{
    tagURoutFlag flag;
    u8 pri;
    u8 len; //most 256 bits
    u16 mtu; //how much bytes could be sent by the rout:default decided by the interface
             //but could be modified by the protocols:such as the icmpv6
}tagRoutPro;

struct RoutV6
{
    struct in6_addr  host;  //if the source addr is INADDR_ANY,we will use this addr to  replace
    struct in6_addr  net;  //which used to match the destination
    struct in6_addr  hop;  //for the local,if no router exist, then the nxthop is INADDR_ANY,which means no nxthop
};
struct RoutV4
{
    struct in_addr   host;  //if the source addr is INADDR_ANY,we will use this addr to  replace
    struct in_addr   net;   //sequence the queue use the same
    struct in_addr   hop;   //for the local,if no router exist, then the nxthop is INADDR_ANY,which means no nxthop
    struct in_addr   broad; //for the broadcast
    struct in_addr   mask;  //for the mask
};
//defines for the rout structure
struct RoutItem4
{
    struct RoutItem4    *nxt;           //point to the next rout item��NULL end
    struct NetDev   *DevFace;       //point to the  interface binded
    struct RoutV4       *rout4;
    tagRoutPro      pro;            //like the unix,we use G U R AND SO ON FLAGS
};
struct RoutItem6
{
    struct RoutItem6    *nxt;           //point to the next rout item��NULL end
    struct NetDev       *DevFace;       //point to the  interface binded
    struct RoutV6       *rout6;
    tagRoutPro          pro;            //like the unix,we use G U R AND SO ON FLAGS
};

#define CN_ERR_ROUTPREDIX     "ROUTMODULE"
#define CN_IP_STRMAX           20
typedef struct
{
    struct RoutItem4 *v4lst;     //which point to the ipv4 route list
    struct RoutItem4 *v4host;    //use the host address here
    struct RoutItem6 *v6lst;     //which point to the ipv6 route list
    struct RoutItem6 *v6host;    //use the host address here
    mutex_t      lock;      //which used to protect the ipv6 and ipv4 rout list
}tagRoutCB;
static tagRoutCB  gRoutCB;
static bool_t __ShowTab(enum_ipv_t ver)
{
    int i = 0;
    struct NetDev *iface = NULL;
    struct RoutItem4 *item;
    char net[CFG_IP_STRMAX];
    char host[CFG_IP_STRMAX];
    char hop[CFG_IP_STRMAX];
    char mask[CFG_IP_STRMAX];
    struct RoutV4 *rout4;
    if (mutex_lock(gRoutCB.lock))
    {
        //show the ipv4
        OsPrintSplit('*',100);
        debug_printf("router","%s:IPV4 KERNEL ROUT:\n\r", CN_ERR_ROUTPREDIX);
        //print the format:NO NET HOST HOP U G R PRI LEN REFIN REFSOUT INTERFACE
        if(ver == EN_IPV_4) //do the ipv4
        {
            debug_printf("router","%2s %-15s %-15s %-15s %-15s %s%s%s %-3s %-3s %-4s %s\n\r",
                    "NO", "NET","MASK", "HOST", "HOP","U", "G", "R", "PRI", "LEN","MTU","IFACE");
            iface = NetDev_ForEachFromDefault(iface);
            while(iface != NULL)
            {
                item = NetDev_GetIPv4RoutEntry(iface);
                while (NULL != item)
                {
                    rout4 = item->rout4;
                    inet_ntop(AF_INET, &rout4->net, net, CFG_IP_STRMAX);
                    inet_ntop(AF_INET, &rout4->host, host, CFG_IP_STRMAX);
                    inet_ntop(AF_INET, &rout4->hop, hop, CFG_IP_STRMAX);
                    inet_ntop(AF_INET, &rout4->mask, mask, CFG_IP_STRMAX);
                    debug_printf("router","%-2d %-15s %-15s %-15s %-15s %d%d%d %-3d %-3d %-4x %s\n\r", \
                                     i++, net, mask, host, hop, item->pro.flag.bits.U_Active,\
                                     item->pro.flag.bits.G_Gateway, item->pro.flag.bits.R_Restore, item->pro.pri,\
                            item->pro.len,item->pro.mtu,NetDevName(item->DevFace));
                    item = item->nxt;
                }
                iface = NetDev_ForEachFromDefault(iface);
            }
            if(NULL != gRoutCB.v4host)
            {
                item =  gRoutCB.v4host;
                rout4 = item->rout4;
                inet_ntop(AF_INET, &rout4->net, net, CFG_IP_STRMAX);
                inet_ntop(AF_INET, &rout4->host, host, CFG_IP_STRMAX);
                inet_ntop(AF_INET, &rout4->hop, hop, CFG_IP_STRMAX);
                inet_ntop(AF_INET, &rout4->mask, mask, CFG_IP_STRMAX);
                debug_printf("router","%-2d %-15s %-15s %-15s %-15s %d%d%d %-3d %-3d %-4x %s\n\r", \
                                 i++, net, mask, host, hop, item->pro.flag.bits.U_Active,\
                                 item->pro.flag.bits.G_Gateway, item->pro.flag.bits.R_Restore, item->pro.pri,\
                        item->pro.len,item->pro.mtu,NetDevName(item->DevFace));
            }
        }
        else if(ver == EN_IPV_6) //do the ipv6
        {
            debug_printf("router","%s:IPV6 KERNEL ROUT:\n\r", CN_ERR_ROUTPREDIX);
        }
        else //do nothing here
        {

        }
        mutex_unlock(gRoutCB.lock);
        debug_printf("router","%s:ROUT PRINT DONE\n\r", CN_ERR_ROUTPREDIX);
        OsPrintSplit('*',100);
    }
    return true;
}

typedef enum
{
    enRoutOperation_print= 0,
    enRoutOperation_add,
    enRoutOperation_del,
    enRoutOperation_match,
}enRoutOperation;

//usage:rout -a/d/p(action) -v(4/6) -i ifname -n net -h host -g gateway
//static bool_t __RoutShell(char *param)
bool_t rout(char *param)
{
    enum_ipv_t ver = EN_IPV_4;
    enRoutOperation  opt = enRoutOperation_print;
    tagHostAddrV4 v4addr;
    struct in_addr  ipmatchv4;
    const char *ifname=NULL;
    tagRouterPara para;
    int argc = 12;
    char *argv[12];
    int i = 0;
    //del or add operation
    memset(&para,0,sizeof(para));
    memset(&v4addr,0,sizeof(v4addr));
    string2arg(&argc,argv,param);
    //now deal all the parameters here
    i = 0;
    while(i < argc)
    {
        if(0 == strcmp(argv[i],"-a")) //which means add a rout
        {
            opt = enRoutOperation_add;
            i++;
        }
        else if(0 == strcmp(argv[i],"-d"))
        {
            opt = enRoutOperation_del;
            i++;
        }
        else if(0 == strcmp(argv[i],"-p"))
        {
            opt = enRoutOperation_print;
            i++;
        }
        else if(0 == strcmp(argv[i],"-v4"))
        {
            ver = EN_IPV_4;
            i++;
        }
        else if(0 == strcmp(argv[i],"-v6"))
        {
            ver = EN_IPV_6;
            i++;
        }
        else if(0 == strcmp(argv[i],"-match")) //do match
        {
            opt = enRoutOperation_match;
            if(ver == EN_IPV_4)
            {
                inet_pton(AF_INET,argv[i+1],&ipmatchv4);
            }
            i += 2;
        }
        else if(0 == strcmp(argv[i],"-p")) //do prior
        {
            para.prior = (u8)strtol(argv[i+1],NULL,0);
            i += 2;
        }
        else if(0 == strcmp(argv[i],"-i"))
        {
            ifname = argv[i+1];
            i += 2;
        }
        else if(0 == strcmp(argv[i],"-n"))  //net
        {
            if(ver == EN_IPV_4)
            {
                inet_pton(AF_INET,argv[i+1],&v4addr.net);
                para.net =&v4addr.net;
            }
            i += 2;
        }
        else if(0 == strcmp(argv[i],"-m"))  //mask
        {
            if(ver == EN_IPV_4)
            {
                inet_pton(AF_INET,argv[i+1],&v4addr.mask);
                para.mask =&v4addr.mask;
            }
            i += 2;
        }
        else if(0 == strcmp(argv[i],"-h")) //host
        {
            if(ver == EN_IPV_4)
            {
                inet_pton(AF_INET,argv[i+1],&v4addr.host);
                para.host =&v4addr.host;
            }
            i += 2;
        }
        else if(0 == strcmp(argv[i],"-g")) //gateway
        {
            if(ver == EN_IPV_4)
            {
                inet_pton(AF_INET,argv[i+1],&v4addr.hop);
                para.hop =&v4addr.hop;
            }
            i += 2;
        }

        else
        {
            i++; //default not support yet
        }
    }
    //now do the operation now
    switch(opt)
    {
        case enRoutOperation_print:
            __ShowTab(ver);
            break;
        case enRoutOperation_match:
            if(ver == EN_IPV_4)
            {
                tagRoutLink  rout4;
                memset(&rout4,0,sizeof(rout4));
                rout4.ver = EN_IPV_4;
                rout4.DstIP = &ipmatchv4;
                rout4.HostIP = &v4addr.host;
                rout4.HopIP = &v4addr.hop;
                if(RouterMatch(&rout4))
                {
                    debug_printf("router","host :%s \n\r",inet_ntoa(*(struct in_addr*)rout4.HostIP));
                    debug_printf("router","hop  :%s \n\r",inet_ntoa(*(struct in_addr*)rout4.HopIP));
                    debug_printf("router","iface:%s \n\r",NetDevName(rout4.DevFace));
                }
                else
                {
                    debug_printf("router","COULD NOT MATCH\n\r");
                }
            }
            break;
        case enRoutOperation_del:
            para.ver = ver;
            para.ifname = ifname;
            RouterRemove(&para);
            break;
        case enRoutOperation_add:
            para.ver = ver;
            para.ifname = ifname;
            RouterCreate(&para);
            break;
        default: //not support yet
            break;
    }

    return true;
}

//this is the router controller
bool_t RouterInit(void)
{
    bool_t ret = false;
    memset(&gRoutCB,0,sizeof(gRoutCB));
    gRoutCB.lock = mutex_init(NULL);
    if(NULL == gRoutCB.lock)
    {
        debug_printf("router","%s:LOCK CREATED FOR ROUTER FAILED\n\r", CN_ERR_ROUTPREDIX);
    }
    else
    {

        return (TRUE);
    }
    return ret;
}
//��������������ж��ٸ� 1
static u8 __CheckOnes(u32 value)
{
    u8 ret = 0;
    int i = 0;
    for(i = 0;i<32;i++)
    {
        if((1<<i)&value)
        {
            ret ++;
        }
    }
    return ret;
}
//we will match the destination
//you also could use this function to do the address type check
//-----------------------------------------------------------------------------
//���ܣ�·����Ŀƥ�䣬����ǰ��para�� ver �� dst ��Ա���븳ֵ������������� type ��
//     iface ��Ա��
//������para��
bool_t RouterMatch(tagRoutLink *para)
{
    bool_t ret = false;
    struct RoutItem4 *tmp = NULL;
    struct RoutV4   *v4;
    struct NetDev *iface = NULL;
    struct in_addr addr;
    if((NULL == para)||(NULL == para->DstIP))
    {
        return ret;
    }
    if(mutex_lock(gRoutCB.lock))
    {
        ret = true;
        para->type = EN_IPTYPE_V4_UNKOWN;
        if(para->ver ==EN_IPV_4)
        {
            memcpy(&addr.s_addr,para->DstIP,sizeof(addr));
            if(addr.s_addr == INADDR_ANY) //use the host
            {
                para->type = EN_IPTYPE_V4_ANY;
            }
            else if(addr.s_addr == INADDR_LOOPBACK) //use the host
            {
                para->type = EN_IPTYPE_V4_HOST;
                tmp = gRoutCB.v4host;
                if(NULL != tmp)
                {
                    v4 = tmp->rout4;
                    para->DevFace = tmp->DevFace;
                    if(NULL != para->HostIP)
                    {
                        memcpy(para->HostIP,&v4->host.s_addr,sizeof(struct in_addr));
                    }
                    if(NULL != para->HopIP)
                    {
                        memcpy(para->HopIP,&v4->hop.s_addr,sizeof(struct in_addr));
                    }
                }
            }
            else  //do the find
            {
//              tmp = gRoutCB.v4lst;
                iface = NetDev_ForEachFromDefault(iface);
                while(iface != NULL)
                {
                    tmp = NetDev_GetIPv4RoutEntry(iface);
                    while(NULL != tmp)
                    {
                        v4 = tmp->rout4;
                        if(v4->host.s_addr == addr.s_addr)
                        {
                            para->type = EN_IPTYPE_V4_LOCAL;
                            tmp = gRoutCB.v4host;
                            break;
                        }
                        else if (v4->broad.s_addr == addr.s_addr)
                        {
                            para->type = EN_IPTYPE_V4_BROAD;
                            break;
                        }
                        else if(v4->net.s_addr == (addr.s_addr&v4->mask.s_addr))
                        {
                            if(addr.s_addr != INADDR_ANY)
                            {
                                para->type = EN_IPTYPE_V4_SUBNET;
                            }
                            break;
                        }
                        else
                        {
                            tmp = tmp->nxt;
                        }
                    }
                    if(NULL != tmp)
                    {
                        v4 = tmp->rout4;
                        para->DevFace = tmp->DevFace;
                        if(NULL != para->HostIP)
                        {
                            memcpy(para->HostIP,&v4->host.s_addr,sizeof(struct in_addr));
                        }
                        if(NULL != para->HopIP)
                        {
                            memcpy(para->HopIP,&v4->hop.s_addr,sizeof(struct in_addr));
                        }
                        break;
                    }
                    else
                        iface = NetDev_ForEachFromDefault(iface);
                }
            }
        }
        else        //IPv6�д�����
        {
        }
        mutex_unlock(gRoutCB.lock);
    }
    return ret;
}


//we find all the matched item in the table,storage the result in the array
//return how many item found, and storage the result in the array
//if handle is NULL,then NOT check the handle
//if net is NULL,then NOT check the net:if not must check the bits specified
//if local is NULL,then NOT check the local
static int __ItemFind(enum_ipv_t ver, struct NetDev *iface, void *net, void *host,
                      void *hop, struct RoutItem4 *array[], int len)
{
    int ret = 0;
    struct RoutV4 *v4;
    struct RoutV6 *v6;

    if (ver == EN_IPV_4)
    {
        struct RoutItem4 *tmp = NULL;
//      tmp = gRoutCB.v4lst;
        tmp = NetDev_GetIPv4RoutEntry(iface);
        while ((NULL != tmp) && (ret < len))
        {
            v4 = tmp->rout4;
            if ((NULL != iface) && (iface != tmp->DevFace))
            {
                goto V4_CHECKMORE;
            }
            if ((NULL != net) && (0 != memcmp((void *) &v4->net, net, sizeof(struct in_addr))))
            {
                goto V4_CHECKMORE;
            }
            if ((NULL != host) && (0 != memcmp((void *) &v4->host, host, sizeof(struct in_addr))))
            {
                goto V4_CHECKMORE;
            }
            if ((NULL != hop) && (0 != memcmp((void *) &v4->hop, hop, sizeof(struct in_addr))))
            {
                goto V4_CHECKMORE;
            }
            //OK this is the one we get
            array[ret] = tmp;
            ret++;
            //and check more
            V4_CHECKMORE:
            tmp = tmp->nxt;
        }
    }
    else if (ver == EN_IPV_6)
    {
        struct RoutItem6 *tmp = NULL;
//        tmp = gRoutCB.v6lst;
        while ((NULL != tmp) && (ret < len))
        {
            v6 = tmp->rout6;
            if ((NULL != iface) && (iface != tmp->DevFace))
            {
                goto V6_CHECKMORE;
            }
            if ((NULL != net) && (0 != memcmp((void *) &v6->net, net, sizeof(struct in6_addr))))
            {
                goto V6_CHECKMORE;
            }
            if ((NULL != host) && (0 != memcmp((void *) &v6->host, host, sizeof(struct in6_addr))))
            {
                goto V6_CHECKMORE;
            }
            if ((NULL != hop) && (0 != memcmp((void *) &v6->hop, hop, sizeof(struct in6_addr))))
            {
                goto V6_CHECKMORE;
            }
            //OK this is the one we get
            array[ret] = tmp;
            ret++;
            V6_CHECKMORE:
            tmp = tmp->nxt;
        }
    }
    else
    {
        //NOT SUPPORT YET
    }

    return ret;
}


//�Ƚ�����·����Ŀ�����ȼ���˳��prior��Ա���������ȣ�����>���볤�����ȡ���>��������
//rules:first prior, then mask length,else the refers times
//return:>0 which means a should be before b
//       =0 which means a should be after b
static int  __SeqCompare(struct RoutItem4 *a,struct RoutItem4 *b)
{
    int ret = 1;
    if(a->pro.pri > b->pro.pri)
    {
        ret =1;
    }
    else if(a->pro.pri == b->pro.pri) //same prior
    {
        if(a->pro.len >= b->pro.len)
        {
            ret = 1;
        }
        else
        {
            ret = 0;
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}

//���һ��·����Ŀ��·�ɶ�����
struct RoutItem4 *__Add2Queue(struct RoutItem4 *queue, struct RoutItem4 *item)
{
    struct RoutItem4 *tmp;
    struct RoutItem4 *nxt;
    if (NULL == queue) //if the queue is empty now
    {
        queue = item;
        return queue;
    }
    //if the item should be inserted into the header
    if (__SeqCompare(item,queue))
    {
        item->nxt = queue;
        queue = item;
        return queue;
    }
    //then select a proper place to insert the item
    tmp = queue;
    while (NULL != tmp)
    {
        if (NULL == tmp->nxt) //the tmp is the last one
        {
            tmp->nxt = item;
            break;
        }
        else
        {
            nxt = tmp->nxt;
            if (__SeqCompare(item,nxt)) //insert it between the tmp and nxt
            {
                tmp->nxt = item;
                item->nxt = nxt;
                break;
            }
            else //do the continue loop
            {
                tmp = nxt;
            }
        }
    }
    return queue;
}
//return the new queue header
static struct RoutItem4 * __RemoveFromQueueV4(struct RoutItem4 *queue, struct RoutItem4 *item) //--TODO
{
    struct RoutItem4 *tmp;
    //queue is empty now
    if (NULL == queue)
    {
        return queue;
    }
    //if the item is the head
    if (item == queue)
    {
        queue = queue->nxt;
        item->nxt = NULL;
    }
    else
    {
        //now do the loop,this operation will not change the queue head
        tmp = queue;
        while (tmp != NULL)
        {
            if (item == tmp->nxt) //find it here
            {
                tmp->nxt = item->nxt;
                item->nxt = NULL;
                break;
            }
            tmp = tmp->nxt; //for the loop
        }
    }
    return queue;
}
static struct RoutItem6 * __RemoveFromQueueV6(struct RoutItem6 *queue, struct RoutItem6 *item) //--TODO
{
    return NULL;
}

struct in_addr __Router_PickIPv4(struct RoutItem4 *Item)
{
    return Item->rout4->host;
}
struct in6_addr __Router_PickIPv6(struct RoutItem6 *Item)
{
    return Item->rout6->host;
}

struct in_addr *__Router_GetNextV4(struct RoutItem4 *Item)
{
    return Item->nxt;
}
struct in6_addr *__Router_GetNextV6(struct RoutItem6 *Item)
{
    return Item->nxt;
}
//-----------------------------------------------------------------------------
//���ܣ���������������·�ɣ����Ի�·����Ŀ���Ի���Ŀ��Ȼ���� ��loop��������������������
//     ���� ���ݽṹ�У�������·�ɿ��ƿ� gRoutCB �С�
//������ver��·����Ŀ�汾��EN_IPV_4 �� EN_IPV_6
//      ifname���Ѿ��� NetDevInstall ��װ����������
//���أ���
//-----------------------------------------------------------------------------
void RouterSetHost(enum_ipv_t ver,const char *ifname)
{
    struct RoutItem4 *newitem = NULL;
    struct NetDev  *iface;
    int memsize;
    struct RoutV4 *v4;
    iface = NetDevGet(ifname);
    if(iface == NULL)       //�Ի�����������
    {
        return ;
    }

    memsize = (ver == EN_IPV_4) ? sizeof(struct RoutV4) : sizeof(struct RoutV6);
    memsize += sizeof(struct RoutItem4);
    newitem = net_malloc(memsize);
    if (NULL == newitem)
    {
        return;
    }
    memset(newitem, 0, memsize);
    newitem->rout4 = (struct RoutItem4 *) (newitem++);

    if (mutex_lock(gRoutCB.lock))
    {
        if(ver == EN_IPV_4)
        {
            if(NULL != gRoutCB.v4host)
            {
                net_free(newitem);
            }
            else
            {
                newitem->pro.flag.bits.IPV4 = 1;
                newitem->pro.len = 32;
                newitem->pro.pri = CN_ROUT_PRIOR_LOOP;
                newitem->pro.flag.value = 0;
                newitem->pro.mtu = NetDevMtu(iface);
                newitem->nxt = NULL;
                newitem->DevFace = iface;
                v4 = newitem->rout4;
                v4->net.s_addr = INADDR_LOOPBACK;
                v4->host.s_addr = INADDR_LOOPBACK;
                v4->mask.s_addr = INADDR_BROAD;
                gRoutCB.v4host = newitem;
            }
        }
        else // IPV6 ��δ���
        {
            net_free(newitem);
        }
        mutex_unlock(gRoutCB.lock);
    }
    return;
}
//add a route to the hosts
void *RouterCreate(tagRouterPara *para)
{
    void *ret = NULL;
    struct RoutItem4 *tmp = NULL;
    struct RoutItem4 *newitem = NULL;
    struct NetDev  *iface;
    int memsize;
    struct RoutV4 *v4;
    if((para == NULL)||(NULL == para->ifname)||(NULL==para->mask)||\
       (NULL==para->host)||(NULL==para->net))       //����������
    {
        return ret;
    }
    iface = NetDevGet( para->ifname);
    if(iface == NULL) //no interface for this rout,so return NULL.
    {
        return ret;
    }
    //first we should make a new one:supposed that you could create one
    //we do it here for we will not do the block following
    memsize = (para->ver == EN_IPV_4) ? sizeof(struct RoutV4) : sizeof(struct RoutV6);
    memsize += sizeof(struct RoutItem4);
    newitem = net_malloc(memsize);
    if (NULL == newitem)
    {
        return ret;
    }
    memset(newitem, 0, memsize);
    newitem->rout4 = (struct RoutItem4 *) (newitem++);
    //USED TO SEE IF ANY ROUT USE THE SAME ROUTE
    if (mutex_lock(gRoutCB.lock))
    {
        tmp = NULL;
        __ItemFind(para->ver, iface, para->net, para->host, para->hop,&tmp,1);
        if (tmp) //not NULL,which means has get the same rout item,so could not add it anymore
        {
            debug_printf("router","%s:ROUT EXSITED\n\r", CN_ERR_ROUTPREDIX);
            net_free(newitem);
            ret = tmp;
        }
        else
        {
            //so initialize the new one and  add it to the queue
            newitem->pro.pri = para->prior;
            newitem->pro.flag.value = para->flags;
            if(para->mtu == 0) //use the interfa
            {
                newitem->pro.mtu=NetDevMtu(iface);
            }
            else
            {
                newitem->pro.mtu = para->mtu;
            }
            newitem->nxt = NULL;
            newitem->DevFace = iface;
            if (para->ver == EN_IPV_4)
            {
                v4 = newitem->rout4;
                memcpy((void *) &v4->net, para->net, sizeof(struct in_addr));
                memcpy((void *) &v4->host, para->host, sizeof(struct in_addr));
                memcpy((void *) &v4->mask, para->mask, sizeof(struct in_addr));
                if(NULL != para->hop)
                {
                    memcpy((void *) &v4->hop, para->hop, sizeof(struct in_addr));
                }
                //compute the broad address
                if(NULL != para->broad)
                {
                    memcpy((void *) &v4->broad, para->broad, sizeof(struct in_addr));
                }
                newitem->pro.len = __CheckOnes(v4->mask.s_addr);
                newitem->pro.flag.bits.IPV4 = 1;
                NetDev_AddIPv4RoutItem(iface,newitem);
//              gRoutCB.v4lst = __Add2Queue(gRoutCB.v4lst, newitem);
            }
            else
            {
                //not implement yet --TODO
            }
            ret = newitem;
        }
        mutex_unlock(gRoutCB.lock);
    }
    return ret;
}

//such as you added the rout and storage the result,then you could use it to remove
void RouterRemoveByHandle(struct RoutItem4 *rout)
{
    struct RoutItem4 *item;
    if (NULL == rout)
    {
        return;
    }
    if (mutex_lock(gRoutCB.lock))
    {
        item = rout;
        if (item->pro.flag.bits.IPV4)
        {
            gRoutCB.v4lst = __RemoveFromQueueV4(gRoutCB.v4lst, item);
        }
        else
        {
            gRoutCB.v6lst = __RemoveFromQueueV6(gRoutCB.v6lst, item);
        }
        net_free(item);
        mutex_unlock(gRoutCB.lock);
    }
    return;
}

//delete the rout item filtered by if name ,net ,host ,hop
//delete the rout,such as you get a new ip,then you will delete all the old binded to the old host address
void   RouterRemove(tagRouterPara *para)
{
    int ret;
    struct RoutItem4 *item;

    struct NetDev  *iface = NULL;
    iface = NetDevGet(para->ifname);
    if (mutex_lock(gRoutCB.lock))
    {
        if (para->ver == EN_IPV_4) //deal the ipv4
        {
            do
            {
                ret = __ItemFind(EN_IPV_4, iface, para->net, para->host, para->hop, &item, 1);
                gRoutCB.v4lst = __RemoveFromQueueV4(gRoutCB.v4lst, item);
            }
            while (ret > 0); //delete all the rout(IPV4) binded to the interface
        }
        else
        {
            do
            {
                ret = __ItemFind(EN_IPV_6, iface, para->net, para->host, para->hop, &item, 1);
                gRoutCB.v6lst = __RemoveFromQueueV6(gRoutCB.v6lst, item);
            }
            while (ret > 0); //delete all the rout(IPV4) binded to the interface
        }
        net_free(item);
        mutex_unlock(gRoutCB.lock);
    }
    return;
}

// ============================================================================
// ���ܣ��޸��Ѵ��ڵ�·����Ŀ
// ������fd��IP�汾�ţ�ȡֵ EN_IPV_4 �� EN_IPV_6
//      ipold�����޸ĵ���Ŀ��IP��ַ��
//      newaddr���µĵ�ַ
// ���أ�false = ����true = �ɹ���
// ============================================================================
bool_t RouterItemChange(enum_ipv_t ver,ipaddr_t ipold,tagHostAddrV4 *newaddr)
{
    bool_t     result = false;
    struct RoutItem4 *CurrentItem4;
    struct RoutV4   *rout4  = NULL;

    if(Lock_MutexPend(gRoutCB.lock, CN_TIMEOUT_FOREVER))
    {
        if(ver == EN_IPV_4)
        {
            CurrentItem4 = gRoutCB.v4lst;
            while(CurrentItem4 != NULL)
            {
                rout4 = CurrentItem4->rout4;
                if(rout4->net.s_addr == ipold)
                    break;
                else
                    CurrentItem4 = CurrentItem4->nxt;
            }
            if(CurrentItem4 != NULL)
            {
                if(newaddr->broad != INADDR_ANY)
                {
                    rout4->broad.s_addr = newaddr->broad;
                }
                if(newaddr->gatway != INADDR_ANY)
                {
                    rout4->hop.s_addr = newaddr->gatway;
                }
                if(newaddr->submask != INADDR_ANY)
                {
                    rout4->mask.s_addr = newaddr->submask;
                }
                if(newaddr->ip != INADDR_ANY)
                {
                    rout4->net.s_addr = newaddr->ip;
                }
                result = true;
            }
        }
        Lock_MutexPost(gRoutCB.lock);
    }
    return result;
}

ADD_TO_ROUTINE_SHELL(rout,rout,"usage:rout -a/d/p(action) -v(4/6) -i ifname -n net -h host -g gateway");












