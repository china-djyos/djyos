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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
int h_errno;
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
struct servent *getservbyname(const char *name, const char *proto)
{
    int i = 0;
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

in_addr_t inet_addr(const char *addr)
{
    int para;
    int tmp[4];
    in_addr_t result = INADDR_NONE;
    u8  ip[4];

    para = sscanf(addr,"%d.%d.%d.%d",&tmp[0],&tmp[1],&tmp[2],&tmp[3]);
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

int inet_aton(const char *string,struct in_addr *addr)
{
    int para;
    int tmp[4];
    u8  ip[4];
    int result = 0;

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


#define CN_INET_ADDRLEN     16
static  char gInetAddr[CN_INET_ADDRLEN];

char *inet_ntoa(struct in_addr addr)
{
    u8 ip[4];
    int tmp[4];
    memcpy((void *)ip,(void *)&addr,sizeof(ip));
    memset((void *)gInetAddr,0,CN_INET_ADDRLEN);

    tmp[0] = (int)ip[0];
    tmp[1] = (int)ip[1];
    tmp[2] = (int)ip[2];
    tmp[3] = (int)ip[3];
    sprintf(gInetAddr,"%d.%d.%d.%d",tmp[0],tmp[1],tmp[2],tmp[3]);

    return (char *)gInetAddr;
}
const char *inet_ntop(int af, const void *src, char *dst, socklen_t cnt)
{
    char *str;
    char          *result = NULL;
    struct in_addr addr;

    if((NULL == src)||(NULL == dst))
    {
        return result;
    }

    if(af==AF_INET)
    {

        memcpy((void *)&addr,src,sizeof(addr));
        str = inet_ntoa(addr);

        if((NULL != str)&&(cnt > (strlen(str)+1)))
        {
            memcpy((void*)dst,(void*)str,strlen(str)+1);
            result = dst;
        }
    }

    return result;
}
int inet_pton(int af, const char *src, void *dst)
{

    int result = -1;
//    unsigned char *str;
    struct in_addr addr;

    if((NULL == src)||(NULL == dst))
    {
        result = 0;
        return result;
    }

    if(af==AF_INET)
    {
        if(inet_aton(src,&addr))
        {
            memcpy((void *)dst,&addr,sizeof(addr));
            result = 1;
        }
    }

    return result;
}

extern struct hostent*  DnsNameResolve(const char *name);



static char gHostName[32] = "DJYOS_TERNIMAL";
int  gethostname(char *name, int len)
{
    strcpy(name,gHostName);
    return 0;
}

int sethostname(const char *name, size_t len)
{
    int ret = -1;
    if((len > 0)&&(len <32)&&(NULL != name))
    {
        memset(gHostName,0,32);
        strncpy(gHostName,name,31);
        ret = 0;
    }
    return ret;
}
static struct hostent gHostEnt;
static struct in_addr gHostAddr;
static struct in_addr gLocalAddr;
static void  *gHLAddr[3];

struct hostent *gethostbyname(const char *name)
{
    struct hostent *ret;
    if((NULL == name)||(0 == strcmp(name,"localhost")))
    {
        gHostEnt.h_addrtype = AF_INET;
        gHostEnt.h_length = sizeof(struct in_addr);
        gHostEnt.h_name =(char *) name;
        inet_aton("127.0.0.1",&gLocalAddr);
        gHLAddr[0]=&gLocalAddr;
        gHLAddr[1]=&gHostAddr;
        gHLAddr[2] = NULL;
        gHostEnt.h_addr_list = (char **)&gHLAddr;
        ret = &gHostEnt;
    }
    else if(0==strcmp(name,gHostName))
    {

        gHostEnt.h_addrtype = AF_INET;
        gHostEnt.h_length = sizeof(struct in_addr);
        gHostEnt.h_name = (char *)name;
        gHostAddr.s_addr = INADDR_LOOPBACK;
        gHLAddr[0]=&gHostAddr;
        gHLAddr[1]=&gLocalAddr;
        gHLAddr[2] = NULL;
        gHostEnt.h_addr_list = (char **)&gHLAddr;
        ret = &gHostEnt;
    }
    else
    {
        ret = DnsNameResolve(name);
    }
    return ret;
}


//these functions must be implement,but not now;
//the gethostbyname only used for the ipv4,and this one is used for both ipv4
//and ipv6,you also can request for the service and port
//return 0 success while others failed
//--TODO,should make a list for more than one address and port,but now only one
int getaddrinfo( const char *hostname, const char *service, const struct addrinfo *hints, struct addrinfo **result )
{
    int res = -1;
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
void freeaddrinfo (struct addrinfo*ai)
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


