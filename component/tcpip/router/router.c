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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netbsp.h>
#include <dbug.h>
#include <shell.h>
#include <misc/misc.h>
#include "../common/router.h"
#include "../common/netdev.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"router"      //路由协议
//parent:"tcpip"       //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_ROUTER == false )
//#warning  " router  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_ROUTER    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,,,
//%$#@enum,true,false,
//%$#@string,1,256,
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
//@#$%component end configure

//发送报文时，先从路由表匹配目标地址，如果不成功，则检查目标是否主机，如果与主机匹配，则使
//用本地路由，否则使用默认路由（如果已经设置）。
//收到报文时，检查目的地址是不是主机，是则接受，否则向前转发。

#define CFG_IP_STRMAX           20      //IP字符串的最大长度

//使用类 Unix 标志
typedef struct
{
    u16 G_Gateway :1;   //G标志，路由指向网关
    u16 U_Active :1;    //U标志，路由是活动的。
    u16 R_Restore :1;   //R标志，恢复动态路由产生的表项
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
//  struct in_addr   hop;   //for the local,if no router exist, then the nxthop is INADDR_ANY,which means no nxthop
//  struct in_addr   broad; //for the broadcast
    struct in_addr   mask;  //for the mask
};
//defines for the rout structure
struct RoutItem4
{
    struct RoutItem4    *nxt;           //point to the next rout item，NULL end
    struct NetDev       *DevFace;       //point to the  interface binded
    struct RoutV4       *rout4;
    tagRoutPro           pro;            //like the unix,we use G U R AND SO ON FLAGS
};
struct RoutItem6
{
    struct RoutItem6    *nxt;           //point to the next rout item，NULL end
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
    struct MutexLCB  *lock;      //which used to protect the ipv6 and ipv4 rout list
}tagRoutCB;
static tagRoutCB  gRoutCB;
static bool_t __ShowTab(enum_ipv_t ver)
{
    int i = 0;
    struct NetDev *iface = NULL;
    struct RoutItem4 *item;
    char net[CFG_IP_STRMAX];
    char host[CFG_IP_STRMAX];
//  char hop[CFG_IP_STRMAX];
    char mask[CFG_IP_STRMAX];
    struct RoutV4 *rout4;
    if (Lock_MutexPend(gRoutCB.lock,CN_TIMEOUT_FOREVER))
    {
        //show the ipv4
        OsPrintSplit('*',100);
        debug_printf("router","%s:IPV4 KERNEL ROUT:\n\r", CN_ERR_ROUTPREDIX);
        //print the format:NO NET HOST HOP U G R PRI LEN REFIN REFSOUT INTERFACE
        if(ver == EN_IPV_4) //do the ipv4
        {
            debug_printf("router","%2s %-15s %-15s %-15s %s%s%s %-3s %-3s %-4s %s\n\r",
                    "NO", "NET","MASK", "HOST", "U", "G", "R", "PRI", "LEN","MTU","IFACE");
            iface = NetDev_ForEachFromDefault(iface);
            while(iface != NULL)
            {
                item = NetDev_GetIPv4RoutEntry(iface);
                while (NULL != item)
                {
                    rout4 = item->rout4;
                    inet_ntop(AF_INET, &rout4->net, net, CFG_IP_STRMAX);
                    inet_ntop(AF_INET, &rout4->host, host, CFG_IP_STRMAX);
//                  inet_ntop(AF_INET, &rout4->hop, hop, CFG_IP_STRMAX);
                    inet_ntop(AF_INET, &rout4->mask, mask, CFG_IP_STRMAX);
                    debug_printf("router","%-2d %-15s %-15s %-15s %d%d%d %-3d %-3d %-4x %s\n\r", \
                                     i++, net, mask, host, item->pro.flag.bits.U_Active,\
                                     item->pro.flag.bits.G_Gateway, item->pro.flag.bits.R_Restore, item->pro.pri,\
                            item->pro.len,item->pro.mtu,NetDev_GetName(item->DevFace));
                    item = item->nxt;
                }
                iface = NetDev_ForEachFromDefault(iface);
            }
//            if(NULL != gRoutCB.v4host)
//            {
//                item =  gRoutCB.v4host;
//                rout4 = item->rout4;
//                inet_ntop(AF_INET, &rout4->net, net, CFG_IP_STRMAX);
//                inet_ntop(AF_INET, &rout4->host, host, CFG_IP_STRMAX);
////              inet_ntop(AF_INET, &rout4->hop, hop, CFG_IP_STRMAX);
//                inet_ntop(AF_INET, &rout4->mask, mask, CFG_IP_STRMAX);
//                debug_printf("router","%-2d %-15s %-15s %-15s %d%d%d %-3d %-3d %-4x %s\n\r",
//                                 i++, net, mask, host, item->pro.flag.bits.U_Active,
//                                 item->pro.flag.bits.G_Gateway, item->pro.flag.bits.R_Restore, item->pro.pri,
//                        item->pro.len,item->pro.mtu,NetDev_GetName(item->DevFace));
//            }
        }
        else if(ver == EN_IPV_6) //do the ipv6
        {
            debug_printf("router","%s:IPV6 KERNEL ROUT:\n\r", CN_ERR_ROUTPREDIX);
        }
        else //do nothing here
        {

        }
        Lock_MutexPost(gRoutCB.lock);
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

//usage:rout -a/d/p(action) -v(4/6) -i ifname -n net -h host -g gateway -dns -dnsbak
bool_t rout(char *param)
{
    enum_ipv_t ver = EN_IPV_4;
    enRoutOperation  opt = enRoutOperation_print;
    tagHostAddrV4 v4addr;
    u32 hop,subnet,ip,submask;
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
                inet_pton(AF_INET,argv[i+1],&subnet);
                para.net =&subnet;
            }
            i += 2;
        }
        else if(0 == strcmp(argv[i],"-m"))  //mask
        {
            if(ver == EN_IPV_4)
            {
                inet_pton(AF_INET,argv[i+1],&submask);
                para.mask =&submask;
            }
            i += 2;
        }
        else if(0 == strcmp(argv[i],"-h")) //host
        {
            if(ver == EN_IPV_4)
            {
                inet_pton(AF_INET,argv[i+1],&ip);
                para.host =&ip;
            }
            i += 2;
        }
//        else if(0 == strcmp(argv[i],"-g")) //gateway
//        {
//            if(ver == EN_IPV_4)
//            {
//                inet_pton(AF_INET,argv[i+1],&hop);
//                para.hop =&hop;
//            }
//            i += 2;
//        }
//      else if(0 == strcmp(argv[i],"-dns")) //dns
//      {
//          if(ver == EN_IPV_4)
//          {
//              inet_pton(AF_INET,argv[i+1],&dns);
//              para.dns =&dns;
//          }
//          i += 2;
//      }
//      else if(0 == strcmp(argv[i],"-dnsbak")) //dnsbak
//      {
//          if(ver == EN_IPV_4)
//          {
//              inet_pton(AF_INET,argv[i+1],&dnsbak);
//              para.dnsbak =&dnsbak;
//          }
//          i += 2;
//      }

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
                rout4.HostIP = &ip;
                rout4.HopIP = &hop;
                if(RouterMatch(&rout4))
                {
                    debug_printf("router","host :%s \n\r",inet_ntoa(*(struct in_addr*)rout4.HostIP));
                    debug_printf("router","hop  :%s \n\r",inet_ntoa(*(struct in_addr*)rout4.HopIP));
                    debug_printf("router","iface:%s \n\r",NetDev_GetName(rout4.DevFace));
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
    gRoutCB.lock = Lock_MutexCreate(NULL);
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
//检查网络掩码中有多少个 1
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
//------------------------------------------------------------------------------
//功能：从路由表中搜索主机地址，如果该地址存在，则返回true。
//参数：host，主机地址
//返回：true = host存在，false= 不存在
//------------------------------------------------------------------------------
bool_t RouterSearchHost(struct in_addr host)
{
    bool_t ret = false;
    struct RoutItem4 *tmp = NULL;
    struct RoutV4   *v4;
    struct NetDev *iface = NULL;

    if(Lock_MutexPend(gRoutCB.lock,CN_TIMEOUT_FOREVER))
    {
        iface = NetDev_ForEachFromDefault(iface);
        while(iface != NULL)
        {
            tmp = NetDev_GetIPv4RoutEntry(iface);
            while(NULL != tmp)
            {
                v4 = tmp->rout4;
                if(v4->host.s_addr == host.s_addr)  //主机地址
                {
                    ret = true;
                    break;
                }
                else
                {
                    tmp = tmp->nxt;
                }
            }
            if(NULL != tmp)
            {
                break;
            }
            else
                iface = NetDev_ForEachFromDefault(iface);
        }
        Lock_MutexPost(gRoutCB.lock);
    }
    return ret;
}

//-----------------------------------------------------------------------------
//功能：路由条目匹配，调用前，para的 ver 和 dst 成员必须赋值，本函数将填充 type 和
//     iface、hop（用于发送）和hostip（用于接收处理） 成员。
//参数：para，参数结构
//返回：true=  成功执行，false=失败
//-----------------------------------------------------------------------------
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
    if(Lock_MutexPend(gRoutCB.lock,CN_TIMEOUT_FOREVER))
    {
        ret = true;
        para->type = EN_IPTYPE_V4_UNKOWN;
        if(para->ver ==EN_IPV_4)
        {
            memcpy(&addr.s_addr,para->DstIP,sizeof(addr));
            if(addr.s_addr == INADDR_ANY) // 非法主机
            {
                para->type = EN_IPTYPE_V4_ANY;
            }
            else
            {
                iface = NetDev_ForEachFromDefault(iface);
                while(iface != NULL)
                {
                    tmp = NetDev_GetIPv4RoutEntry(iface);
                    while(NULL != tmp)
                    {
                        //比较顺序：1、是否与主机地址相同，
                        //         2、是否网络地址相同且其他位全1,=广播地址
                        //         3、是否在同一个子网内的其他主机
                        //todo：改为每个步骤都比较所有路由条目，再进行下一步骤比较。
                        v4 = tmp->rout4;
                        if(v4->host.s_addr == addr.s_addr)  //主机地址
                        {
                            para->type = EN_IPTYPE_V4_LOCAL;
                            tmp = gRoutCB.v4host;           //取自环路由条目
                            if(tmp == NULL)
                                break;
                            para->DevFace = tmp->DevFace;   //取自环网卡
                            if(NULL != para->HopIP)
                            {
                                //这是发给自己的包，下一跳当然是自己
                                memcpy(para->HopIP,&addr.s_addr,sizeof(struct in_addr));
                            }
                            if(NULL != para->HostIP)
                            {
                                // 主机IP由路由条目指定，实际上也是自己
                                memcpy(para->HostIP,&addr.s_addr,sizeof(struct in_addr));
                            }
                            break;
                        }
                        else if(((~v4->mask.s_addr & addr.s_addr) == ~v4->mask.s_addr)
                                && ((v4->mask.s_addr & addr.s_addr) == v4->net.s_addr) )
                        {   //广播地址：网络地址相同，其他位数全1
                            para->type = EN_IPTYPE_V4_BROAD;
                            v4 = tmp->rout4;
                            para->DevFace = tmp->DevFace;
                            if(NULL != para->HopIP)     //下一跳就是广播地址本身
                            {
                                memcpy(para->HopIP,&addr.s_addr,sizeof(struct in_addr));
                            }
                            if(NULL != para->HostIP)    //主机地址由路由条目指定
                            {
                                memcpy(para->HostIP,&v4->host.s_addr,sizeof(struct in_addr));
                            }
                            break;
                        }
                        else if(v4->net.s_addr == (addr.s_addr&v4->mask.s_addr))
                        {
                            //同一个子网内，
                            para->type = EN_IPTYPE_V4_SUBNET;
                            v4 = tmp->rout4;
                            para->DevFace = tmp->DevFace;
                            if(NULL != para->HopIP)
                            {
                                memcpy(para->HopIP,&addr.s_addr,sizeof(struct in_addr));
                            }
                            if(NULL != para->HostIP)
                            {
                                memcpy(para->HostIP,&v4->host.s_addr,sizeof(struct in_addr));
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
//                      v4 = tmp->rout4;
//                      para->DevFace = tmp->DevFace;
//                      if(NULL != para->HostIP)
//                      {
//                          memcpy(para->HostIP,&v4->host.s_addr,sizeof(struct in_addr));
//                      }
                        break;
                    }
                    else
                        iface = NetDev_ForEachFromDefault(iface);
                }
                if(NULL == tmp)
                {
                    if((u32)-1 ==addr.s_addr)
                        para->type = EN_IPTYPE_V4_BROAD;
                    else
                        para->type = EN_IPTYPE_V4_UNKOWN;
                    iface = NetDev_GetDefault();
                    para->DevFace = iface;
                    if(NULL != para->HopIP)             //从默认网卡取下一跳
                    {
                        NetDev_GetDefaultGateway(EN_IPV_4, para->HopIP);
//                      memcpy(para->HopIP, &iface->gateway, sizeof(struct in_addr));
                    }
                    if(NULL != para->HostIP)    //取默认网卡任意一条路由的主机IP
                    {
                        NetDev_GetDefaultIP(EN_IPV_4, para->HostIP);
//                      memcpy(para->HostIP,&addr.s_addr,sizeof(struct in_addr));
                    }
                }
            }
        }
        else        //IPv6尚待完善
        {
        }
        Lock_MutexPost(gRoutCB.lock);
    }
    return ret;
}

//------------------------------------------------------------------------------
//功能：比较两个IPV路由条目是否相同，只比较rout4成员
bool_t __Item4Compare(struct RoutItem4 *a,struct RoutItem4 *b)
{
    return memcmp(a->rout4, b->rout4, sizeof(struct RoutV4));
}

//we find all the matched item in the table,storage the result in the array
//return how many item found, and storage the result in the array
//if handle is NULL,then NOT check the handle
//if net is NULL,then NOT check the net:if not must check the bits specified
//if local is NULL,then NOT check the local
static int __ItemFind(enum_ipv_t ver, struct NetDev *iface, void *net, void *host,
                       void *array[], int len)
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
//          if ((NULL != hop) && (0 != memcmp((void *) &v4->hop, hop, sizeof(struct in_addr))))
//          {
//              goto V4_CHECKMORE;
//          }
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
//            if ((NULL != hop) && (0 != memcmp((void *) &v6->hop, hop, sizeof(struct in6_addr))))
//            {
//                goto V6_CHECKMORE;
//            }
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


//比较两个路由项目的优先级，顺序：prior成员（大者优先）——>掩码长者优先——>后到者优先
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

//添加一个路由条目到路由队列中
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
struct RoutItem4 * __RemoveFromQueueV4(struct RoutItem4 *queue, struct RoutItem4 *item) //--TODO
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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static struct RoutItem6 * __RemoveFromQueueV6(struct RoutItem6 *queue, struct RoutItem6 *item)
{
    return NULL;
}
#pragma GCC diagnostic pop

struct in_addr __Router_PickIPv4(struct RoutItem4 *Item)
{
    return Item->rout4->host;
}
struct in6_addr __Router_PickIPv6(struct RoutItem6 *Item)
{
    return Item->rout6->host;
}

struct RoutItem4 *__Router_GetNextV4(struct RoutItem4 *Item)
{
    return Item->nxt;
}
struct RoutItem6 *__Router_GetNextV6(struct RoutItem6 *Item)
{
    return Item->nxt;
}

//-----------------------------------------------------------------------------
//功能：创建并设置主机路由，即自环路由条目，自环条目属于 “loop”网卡，除了挂在网卡的 数
//      据结构的路由表入口外，也在路由控制块 gRoutCB 中。因为一台主机可以有多个ip（即路由
//      条目的host），但不管给哪个host发数据，都应该是自环，使用自环条目，故把自环条目
//      独立保存在 gRoutCB.v4host 或 gRoutCB.v6host 中。
//参数：ver，路由条目版本，EN_IPV_4 或 EN_IPV_6
//      ifname，已经由 NetDev_Install 安装的网卡名字
//返回：无
//-----------------------------------------------------------------------------
void RouterSetHost(enum_ipv_t ver,const char *ifname)
{
    struct NetDev  *iface;

    iface = NetDev_GetHandle(ifname);
    if(iface == NULL)       //自环网卡不存在
    {
        return ;
    }
    if (Lock_MutexPend(gRoutCB.lock,CN_TIMEOUT_FOREVER))
    {
        if(ver == EN_IPV_4)
            gRoutCB.v4host = NetDev_GetIPv4RoutEntry( iface );
        else
            gRoutCB.v6host = NetDev_GetIPv6RoutEntry( iface );
        Lock_MutexPost(gRoutCB.lock);
    }
    return;
}

//add a route to the hosts
//------------------------------------------------------------------------------
//功能：添加路由条目并把它加入到网卡的路由表中
//参数：para，参数结构指针
//返回：新添加的路由条目，因可能是struct RoutItem4/6，故用void*
//------------------------------------------------------------------------------
void *RouterCreate(tagRouterPara *para)
{
    void *ret = NULL;
    struct RoutItem4 *tmp = NULL;
    struct RoutItem4 *newitem = NULL;
    struct NetDev  *iface;
    int memsize;
    struct RoutV4 *v4;
    if((para == NULL)||(NULL == para->ifname)||(NULL==para->mask)||\
       (NULL==para->host)||(NULL==para->net))       //参数不完整
    {
        return ret;
    }
    iface = NetDev_GetHandle( para->ifname);
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
    newitem->rout4 = (struct RoutV4 *) (newitem+1);
    //USED TO SEE IF ANY ROUT USE THE SAME ROUTE
    if (Lock_MutexPend(gRoutCB.lock,CN_TIMEOUT_FOREVER))
    {
        tmp = NULL;
        __ItemFind(para->ver, iface, para->net, para->host, (void*)&tmp,1);
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
                newitem->pro.mtu=NetDev_GetMtu(iface);
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
//              if(NULL != para->hop)
//              {
//                  memcpy((void *) &v4->hop, para->hop, sizeof(struct in_addr));
//              }
//              if(NULL != para->dns)   //设置网卡的dns地址，后设置的会替代前面设置的
//              {                       //暂时只支持一个dns地址，要支持多个，可改为链表
//                  memcpy((void *) &iface->dns, para->dns, sizeof(struct in_addr));
//              }
                newitem->pro.len = __CheckOnes(v4->mask.s_addr);
                newitem->pro.flag.bits.IPV4 = 1;
                NetDev_AddIPv4RoutItem(iface,newitem);
//              gRoutCB.v4lst = __Add2Queue(gRoutCB.v4lst, newitem);
            }
            else
            {
                //IPv6 not implement yet
            }
            ret = newitem;
        }
        Lock_MutexPost(gRoutCB.lock);
    }
    return ret;
}

//such as you added the rout and storage the result,then you could use it to remove
void RouterRemoveByHandle(struct RoutItem4 *rout)
{
    struct NetDev *iface = NULL;
    struct RoutItem4 *tmp,*entry;
    bool_t found = false;
    if (NULL == rout)
    {
        return;
    }
    if (Lock_MutexPend(gRoutCB.lock,CN_TIMEOUT_FOREVER))
    {
        iface = NetDev_ForEachFromDefault(iface);
        while(iface != NULL)
        {
            tmp = NetDev_GetIPv4RoutEntry(iface);
            entry = tmp;
            while(NULL != tmp)
            {
                if(memcmp(tmp, rout, sizeof(struct RoutItem4)) == 0)
                {
                    NetDev_SetIPv4RoutEntry(iface,__RemoveFromQueueV4(entry, tmp));
                    net_free(tmp);
                    found = true;
                    break;
                }
                else
                {
                    tmp = tmp->nxt;
                }
            }
            if(found == false)
                iface = NetDev_ForEachFromDefault(iface);
            else
                break;
        }

        Lock_MutexPost(gRoutCB.lock);
    }
    return;
}

void RouterRemoveByNetDev(const char *ifname)
{
    struct NetDev *iface;
    struct RoutItem4 *mark;
    struct RoutItem4 *root;

    iface = NetDev_GetHandle(ifname);
    root = NetDev_GetIPv4RoutEntry(iface);
    if (NULL == root)
    {
        return;
    }
    if (Lock_MutexPend(gRoutCB.lock,CN_TIMEOUT_FOREVER))
    {
        while(root) {
            mark = root;
            root = root->nxt;
            net_free(mark);
        }
        NetDev_SetIPv4RoutEntry(iface, NULL);
        Lock_MutexPost(gRoutCB.lock);
    }
    NetDev_SetIPv4RoutEntry(iface, 0); //清0
    return;
}

//delete the rout item filtered by if name ,net ,host ,hop
//delete the rout,such as you get a new ip,then you will delete all the old binded to the old host address
void   RouterRemove(tagRouterPara *para)
{
    int ret;
    struct NetDev  *iface = NULL;

    iface = NetDev_GetHandle(para->ifname);
    if (Lock_MutexPend(gRoutCB.lock,CN_TIMEOUT_FOREVER))
    {
        if (para->ver == EN_IPV_4) //deal the ipv4
        {
            struct RoutItem4 *item;
            do
            {
                ret = __ItemFind(EN_IPV_4, iface, para->net, para->host, (void*)&item, 1);
                gRoutCB.v4lst = __RemoveFromQueueV4(gRoutCB.v4lst, item);
            }
            while (ret > 0); //delete all the rout(IPV4) binded to the interface
            net_free(item);
        }
        else
        {
            struct RoutItem6 *item;
            do
            {
                ret = __ItemFind(EN_IPV_6, iface, para->net, para->host, (void*)&item, 1);
                gRoutCB.v6lst = __RemoveFromQueueV6(gRoutCB.v6lst, item);
            }
            while (ret > 0); //delete all the rout(IPV4) binded to the interface
            net_free(item);
        }
        Lock_MutexPost(gRoutCB.lock);
    }
    return;
}

// ============================================================================
// 功能：修改已存在的路由条目
// 参数：fd，IP版本号，取值 EN_IPV_4 或 EN_IPV_6
//      ipold，被修改的条目的IP地址。
//      newaddr，新的地址
// 返回：false = 出错，true = 成功。
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
//              if(newaddr->gatway != INADDR_ANY)
//              {
//                  rout4->hop.s_addr = newaddr->gatway;
//              }
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












