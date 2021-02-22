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
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然:
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明:本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "../component_config_tcpip.h"

//usage:use this data to storage the netdb errors
s32 h_errno;
//服务器列表，TODO：应该改为注册式，否则添加服务器很麻烦
static struct servent  gServEnt[]={
        {"sntp",NULL,htons(123),"udp"},
        {"ftp",NULL,htons(21),"tcp"},
        {"telnet",NULL,htons(23),"tcp"},
        {"smtp",NULL,htons(25),"tcp"},
        {"dns",NULL,htons(53),"udp"},
        {"dns",NULL,htons(53),"tcp"},
        {"dhcps",NULL,htons(67),"udp"},
        {"dhcpc",NULL,htons(68),"udp"},
        {"tftp",NULL,htons(69),"udp"},
};

#define CN_SERVENT_NUM  (sizeof(gServEnt)/sizeof(struct servent))

//----获取服务入口---------------------------------------------------------------
//功能：根据服务的功能和协议，取服务入口
//参数：name，服务器名，例如 "ftp"
//     proto，字符串表示的传输层协议，例如 "tcp"
//返回：struct servent结构指针，失败则返回NULL
//------------------------------------------------------------------------------
struct servent *getservbyname(const char *name, const char *proto)
{
    u32 i = 0;
    struct servent *result = NULL;

    for(i= 0; i <CN_SERVENT_NUM;i++)
    {
        if(0==strcmp(name,gServEnt[i].s_name))
        {
            if((NULL == proto)||(0==strcmp(proto,gServEnt[i].s_proto)))
            {
                result = &gServEnt[i];
                break;
            }
        }
    }
    return result;
}

//----网络地址转换--------------------------------------------------------------
//功能：字符串点分十进制模式的IPV4地址转换为 u32 格式地址，一律按大端，即字符串最左边的
//     数字（IP的最高字节）出现在 u32 存储单元的最低地址字节。
//参数：string，被转换的 IPV4 地址。
//返回：转换的结果，如果失败则返回 INADDR_NONE
//-----------------------------------------------------------------------------
in_addr_t inet_addr(const char *string)
{
    s32 para;
    s32 tmp[4];
    in_addr_t result = INADDR_NONE;
    u8  ip[4];

    para = sscanf(string,"%d.%d.%d.%d",&tmp[0],&tmp[1],&tmp[2],&tmp[3]);
    if(para == 4)
    {
        ip[0] = (u8)tmp[0];
        ip[1] = (u8)tmp[1];
        ip[2] = (u8)tmp[2];
        ip[3] = (u8)tmp[3];
        memcpy((void *)&result,(void *)ip,sizeof(result));
    }
    return result;
}

//----网络地址转换--------------------------------------------------------------
//功能：同inet_addr，但把 INADDR_NONE(0xffffffff) 看成合法地址
//参数：string，被转换的 IPV4 地址。
//      addr，接收结果的地址
//返回：成功则返回1，失败返回0
//-----------------------------------------------------------------------------
s32 inet_aton(const char *string,struct in_addr *addr)
{
    s32 para;
    s32 tmp[4];
    u8  ip[4];
    s32 result = 0;

    para = sscanf(string,"%d.%d.%d.%d",&tmp[0],&tmp[1],&tmp[2],&tmp[3]);
    if(para == 4)
    {
        ip[0] = (u8)tmp[0];
        ip[1] = (u8)tmp[1];
        ip[2] = (u8)tmp[2];
        ip[3] = (u8)tmp[3];
        memcpy((void *)addr,(void *)ip,sizeof(ip));
        result = 1;
    }

    return result;
}

//----IPv4地址转换---------------------------------------------------------------
//功能：将网络字节序的整数 IPv4地址转换成点分十进制地址，不可重入。
//参数：addr，整数IPv4地址
//返回：点分十进制地址
//注意：使用静态变量返回地址，函数不可重入
//------------------------------------------------------------------------------
char *inet_ntoa(struct in_addr addr)
{
#define CN_INET_ADDRLEN     16
static  char gInetAddr[CN_INET_ADDRLEN];
    u8 ip[4];
    s32 tmp[4];
    memcpy((void *)ip,(void *)&addr,sizeof(ip));
    memset((void *)gInetAddr,0,CN_INET_ADDRLEN);

    tmp[0] = (s32)ip[0];
    tmp[1] = (s32)ip[1];
    tmp[2] = (s32)ip[2];
    tmp[3] = (s32)ip[3];
    sprintf(gInetAddr,"%d.%d.%d.%d",tmp[0],tmp[1],tmp[2],tmp[3]);

    return (char *)gInetAddr;
}

//----支持IPv6网路地址转化--------------------------------------------------------
//功能：将数值格式转化为点分十进制的ip地址格式
//参数：family，地址族
//     src，待转化的地址
//     dst，保存转换结果的指针
//     cnt，dst buffer的长度
//TODO：IPv6部分并没有实现
//------------------------------------------------------------------------------
const char *inet_ntop(s32 family, const void *src, char *dst, socklen_t cnt)
{
    char *str;
    char          *result = NULL;
    struct in_addr addr;

    if((NULL == src)||(NULL == dst))
    {
        return result;
    }

    if(family==AF_INET)
    {

        memcpy((void *)&addr,src,sizeof(addr));
        str = inet_ntoa(addr);

        if((NULL != str)&&(cnt > (s32)(strlen(str)+1)))
        {
            memcpy((void*)dst,(void*)str,strlen(str)+1);
            result = dst;
        }
    }

    return result;
}

//----支持IPv6网路地址转化--------------------------------------------------------
//功能：将点分十进制的ip地址转化为用于网络传输的数值格式
//参数：family，地址族
//     src，待转化的地址
//     dst，保存转换结果的指针
//TODO：IPv6部分并没有实现
//------------------------------------------------------------------------------
s32 inet_pton(s32 family, const char *src, void *dst)
{

    s32 result = -1;
//    unsigned char *str;
    struct in_addr addr;

    if((NULL == src)||(NULL == dst))
    {
        result = 0;
        return result;
    }

    if(family==AF_INET)
    {
        if(inet_aton(src,&addr))
        {
            memcpy((void *)dst,&addr,sizeof(addr));
            result = 1;
        }
    }

    return result;
}

static char gHostName[32] = "DJYOS_TERMINAL";
//----取主机名-------------------------------------------------------------------
//功能：取主机名
//参数：name，接收结果的buffer
//     len，buffer尺寸
//返回：0
//------------------------------------------------------------------------------
s32  gethostname(char *name, s32 len)
{
    strncpy(name,gHostName,len);
    return 0;
}

//----设置主机名-----------------------------------------------------------------
//功能：设置主机名
//参数：name，新名字，＜32
//     len，新名字长度，必须＜32，否则无效
//返回：0
//------------------------------------------------------------------------------
s32 sethostname(const char *name, size_t len)
{
    s32 ret = -1;
    if((len > 0)&&(len <32)&&(NULL != name))
    {
        strncpy(gHostName,name,31);
        ret = 0;
    }
    return ret;
}
static struct hostent gHostEnt;
static struct in_addr gHostAddr;    //主机地址
static struct in_addr gLocalAddr;   //本地地址
static struct in_addr *gHLAddr[3];  //指向数值格式的网络地址
extern struct hostent*  DNS_NameResolve(const char *name);
//------------------------------------------------------------------------------
//功能：对应于给定主机名的包含主机名字和地址信息的hostent结构的指针。引用静态定义数据
//      结构，非多线程安全。如果主机名NULL，则认为是"localhost"。
//参数：name，给定的主机名。
//返回：struct hostent *
//------------------------------------------------------------------------------
struct hostent *gethostbyname(const char *name)
{
    struct hostent *ret;
    if((NULL == name)||(0 == strcmp(name,"localhost")))
    {
        gHostEnt.h_addrtype = AF_INET;
        gHostEnt.h_length = sizeof(struct in_addr);
        gHostEnt.h_name =(char *) name;
        inet_aton("127.0.0.1", &gLocalAddr);
        gHLAddr[0] = &gLocalAddr;
        gHLAddr[1] = &gHostAddr;
        gHLAddr[2] = NULL;              //结束标志
        gHostEnt.h_addr_list = (char **)&gHLAddr;
        ret = &gHostEnt;
    }
    else if(0==strcmp(name,gHostName))
    {

        gHostEnt.h_addrtype = AF_INET;
        gHostEnt.h_length = sizeof(struct in_addr);
        gHostEnt.h_name = (char *)name;
        gHostAddr.s_addr = INADDR_LOOPBACK;
        gHLAddr[0] = &gHostAddr;
        gHLAddr[1] = &gLocalAddr;
        gHLAddr[2] = NULL;
        gHostEnt.h_addr_list = (char **)&gHLAddr;
        ret = &gHostEnt;
    }
    else
    {
        ret = DNS_NameResolve(name);
    }
    return ret;
}


void init_hostent_ext(struct hostent_ext *phostent_ext)
{
    int i=0;
    if(phostent_ext) {
        memset(phostent_ext, 0, sizeof(struct hostent_ext));
        phostent_ext->h_name = &phostent_ext->arr_name[0];
        for (i=0; i<CN_RESULT_NUM+1; i++)
        {
            phostent_ext->arr_aliases[i] = &phostent_ext->dns_res.arrDnsCNameAddr[i][0];
            phostent_ext->arr_addr_list[i] = &phostent_ext->dns_res.arrDnsINameAddrV4[i][0];
        }
        phostent_ext->h_aliases = (char **)phostent_ext->arr_aliases;
        phostent_ext->h_addr_list = (char **)phostent_ext->arr_addr_list;
    }
}

void ended_hostent_ext(struct hostent_ext *phostent_ext)
{
    int i=0;
    if(phostent_ext) {
        for (i=0; i<CN_RESULT_NUM+1; i++)
        {
            if (*(int*)phostent_ext->arr_addr_list[i] == 0) {
                phostent_ext->arr_addr_list[i] = 0;
                phostent_ext->arr_aliases[i] = 0;
                break;
            }
        }
        if(i == CN_RESULT_NUM+1) {
            phostent_ext->arr_addr_list[CN_RESULT_NUM] = 0;
            phostent_ext->arr_aliases[CN_RESULT_NUM] = 0;
        }
    }
}

int DNS_NameResolveExt(const char *name, struct hostent_ext *phostent_ext);

//------------------------------------------------------------------------------
//功能：gethostbyname的多线程安全版本，由调用方提供 hostent_ext 结构。
//参数：name，给定的主机名。
//     pnew，调用方提供的 hostent_ext 结构的指针
//返回：struct hostent *
//------------------------------------------------------------------------------
struct hostent * gethostbyname_r(const char *name,struct hostent_ext *pnew)
{
    u32 len=0;
    struct in_addr temp;
//  struct hostent_ext *pnew = (struct hostent_ext*)malloc(sizeof(struct hostent_ext));
    if(pnew==NULL)
        return NULL;
    init_hostent_ext(pnew);

    if((NULL == name)||(0 == strcmp(name,"localhost")))
    {
        pnew->h_addrtype = AF_INET;
        pnew->h_length = sizeof(struct in_addr);
        len = sizeof(pnew->arr_name)-1;
        len = (len<strlen(name))?len:strlen(name);
        memcpy(pnew->h_name, name, len);
        inet_aton("127.0.0.1", &gLocalAddr);
        memcpy(&pnew->dns_res.arrDnsINameAddrV4[0], &gLocalAddr, 4);
        memcpy(&pnew->dns_res.arrDnsINameAddrV4[1], &gHostAddr, 4);
        memset(&pnew->dns_res.arrDnsINameAddrV4[2], 0x00, 4);//结束标志
    }
    else if(0==strcmp(name,gHostName))
    {
        pnew->h_addrtype = AF_INET;
        pnew->h_length = sizeof(struct in_addr);
        len = sizeof(pnew->arr_name)-1;
        len = (len<strlen(name))?len:strlen(name);
        memcpy(pnew->h_name, name, len);
        temp.s_addr = INADDR_LOOPBACK;
        memcpy(&pnew->dns_res.arrDnsINameAddrV4[0], &temp, 4);
        memcpy(&pnew->dns_res.arrDnsINameAddrV4[1], &gLocalAddr, 4);
        memset(&pnew->dns_res.arrDnsINameAddrV4[2], 0x00, 4);//结束标志
    }
    else
    {
        DNS_NameResolveExt(name, pnew);
    }
    ended_hostent_ext(pnew);
    return (struct hostent *)pnew;
}

//void gethostbyname_free(struct hostent *phostent)
//{
//    if(phostent) {
//        free(phostent);
//    }
//}



//these functions must be implement,but not now;
//the gethostbyname only used for the ipv4,and this one is used for both ipv4
//and ipv6,you also can request for the service and port
//return 0 success while others failed
//--TODO,should make a list for more than one address and port,but now only one
//------------------------------------------------------------------------------
//功能：gethostbyname和gethostbyaddr这两个函数仅仅支持IPv4，getaddrinfo函数能够处理
//      名字到地址以及服务到端口这两种转换，返回的是一个sockaddr结构的链表而不是一个地址
//      清单。这些sockaddr结构随后可由套接口函数直接使用。如此一来，getaddrinfo函数把协
//      议相关性安全隐藏在这个库函数内部。应用程序只要处理由getaddrinfo函数填写的套接口
//      地址结构。
//参数：hostname，一个主机名或者地址串(IPv4的点分十进制串或者IPv6的16进制串)
//      service，服务名可以是十进制的端口号，也可以是已定义的服务名称，如ftp、http等
//      hints，可以是一个空指针，也可以是一个指向某个addrinfo结构体的指针，调用者在这个
//             结构中填入关于期望返回的信息类型的暗示。举例来说：指定的服务既可支持TCP也
//             可支持UDP，所以调用者可以把hints结构中的ai_socktype成员设置成
//             SOCK_DGRAM使得返回的仅仅是适用于数据报套接口的信息。
//      result，返回一个指向addrinfo结构体链表的指针。
//返回：0=成功，其他值=失败
//TODO：应该为多个地址和端口创建一个列表，但目前只有一个
//------------------------------------------------------------------------------
s32 getaddrinfo( const char *hostname, const char *service,
                 const struct addrinfo *hints, struct addrinfo **result )
{
    s32 res = -1;
    struct hostent     *host;
    struct servent     *serve;
    struct addrinfo    *answer;
    struct sockaddr_in *addr;
    const char         *protoname;

    answer= net_malloc(sizeof(struct addrinfo));
    if(NULL == answer)
    {
        res = EAI_MEMORY;
        goto EXIT_INFOMEM;
    }
    memset(answer,0,sizeof(struct addrinfo));

    addr =net_malloc(sizeof(struct sockaddr_in));
    if(NULL == addr)
    {
        res = EAI_MEMORY;
        goto EXIT_ADDRMEM;
    }
    memset(addr,0,sizeof(struct sockaddr_in));


    if(((NULL == hostname)&&(NULL== service))||(NULL == result))
    {
        res = EAI_NONAME;
        goto EXIT_PARAM;
    }
    if((NULL != hints)&&\
       (hints->ai_family != AF_INET)&&(hints->ai_family != AF_INET6)&&(hints->ai_family != AF_UNSPEC))
    {
        res = EAI_FAMILY;
        goto EXIT_FAMILY;
    }
    if((NULL != hints)&&\
        (hints->ai_socktype != SOCK_STREAM)&&(hints->ai_socktype != SOCK_DGRAM))
    {
        res = EAI_SOCKTYPE;
        goto EXIT_SOCKTYPE;
    }
    //get the host
    if(inet_aton(hostname,&addr->sin_addr) > 0)
    {
        answer->ai_family = AF_INET;
        answer->ai_next = NULL;
        answer->ai_addrlen = sizeof(struct sockaddr_in);
        answer->ai_addr = (struct sockaddr*)addr;
    }
    else
    {
        host = gethostbyname(hostname);
        if(NULL == host)
        {
            res = EAI_NODATA;
            goto EXIT_GETFAIL;
        }

        memcpy(&addr->sin_addr,host->h_addr_list[0],sizeof(addr->sin_addr));
        addr->sin_family = AF_INET;
        answer->ai_family = AF_INET;
        answer->ai_next = NULL;
        answer->ai_addrlen = sizeof(struct sockaddr_in);
        answer->ai_addr = (struct sockaddr*)addr;
    }
    if(NULL != hints)
    {
        protoname = NULL;
        if(hints->ai_socktype == SOCK_STREAM)
        {
            protoname = "tcp";
        }
        else
        {
            protoname = "udp";
        }
        addr->sin_family = hints->ai_family;
        serve = getservbyname(service,protoname);
        if((NULL != service)&&(NULL == serve))
        {
            res = EAI_NODATA;
            goto EXIT_GETFAIL;
        }
        addr->sin_port = (in_port_t)serve->s_port;
        answer->ai_socktype = hints->ai_socktype;
        answer->ai_protocol = hints->ai_protocol;
    }
    res = EAI_OK;

    *result = answer;
    return res;

EXIT_GETFAIL:
EXIT_SOCKTYPE:
EXIT_FAMILY:
EXIT_PARAM:
    net_free(addr);
EXIT_ADDRMEM:
    net_free(answer);
EXIT_INFOMEM:
    h_errno = res;
    return res;
}
//net_free the ai by getaddrinfo returned;
void freeaddrinfo (struct addrinfo *ai)
{
    if(NULL != ai)
    {
        if(NULL != ai->ai_addr)
        {
            net_free(ai->ai_addr);
        }
        net_free(ai);
    }
    return;
}


