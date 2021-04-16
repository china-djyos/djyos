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
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netbsp.h>
#include "dbug.h"
#include <shell.h>

enum __DNS_OPCODE
{
    DNS_OPCODE_STANDQUERY = 0,
    DNS_OPCODE_REVERSEQUERY,
    DNS_OPCODE_SERVERSTAT,
    DNS_OPCODE_RESERVE1,
    DNS_OPCODE_INSTRUCTION,
    DNS_OPCODE_DIAGRAMUPDATE,
};

enum __DNS_RCODE
{
    DNS_RCODE_NOERR = 0,
    DNS_RCODE_SERIOUSEFAIL,
    DNS_RCODE_NAMEERR,
    DNS_RCODE_NOIMPLEMENT,
    DNS_RCODE_REFUSE,
    DND_RCODE_RESERVE,
};

/* http://en.wikipedia.org/wiki/List_of_DNS_record_types */
enum dns_query_type {
    DNS_A_RECORD     = 0x01,
    DNS_CNAME_RECORD = 0x05,
    DNS_MX_RECORD    = 0x0f,
};

enum QueryClass //ָ����Ϣ��Э���顣
{
    IN            =0x01,    //ָ�� Internet ���
    CSNET         =0x02,    //ָ�� CSNET ��𡣣��ѹ�ʱ��
    CHAOS         =0x03,    //ָ�� Chaos ���
    HESIOD        =0x04,    //ָ�� MIT Athena Hesiod ���
    ANY           =0xFF     //ָ���κ���ǰ�г���ͨ�����
};

#pragma pack(1)
typedef struct
{
    u16             tid;        /* Transaction ID */
    u16             flags;      /* Flags */
    u16             nqueries;   /* Questions */
    u16             nanswers;   /* Answers */
    u16             nauth;      /* Authority PRs */
    u16             nother;     /* Other PRs */
    u8              data[0];    /* Data, variable length */
}tagDnsHeader;
#pragma pack()

//#define CN_RESULT_NUM     10
static unsigned char      gDnsCNameOffset;
static unsigned char     *gDnsCNameAddr[CN_RESULT_NUM+1];
static unsigned char      gDnsINameV4Offset;
static unsigned char     *gDnsINameAddrV4[CN_RESULT_NUM+1];

typedef struct
{
    u16 type;
    u16 class;
}tagDnsQType;
typedef struct
{
    u16  type;
    u16  class;
    u32  ttl;
}tagDnsAType;


static void __DNS_ClearResult()
{
    memset((void *)gDnsCNameAddr,0,sizeof(gDnsCNameAddr));
    memset((void *)gDnsINameAddrV4,0,sizeof(gDnsINameAddrV4));
    gDnsCNameOffset = 0;
    gDnsINameV4Offset = 0;
}

#define CN_DNS_ERR_LENTH  31
//decode the dns answers
enum __DNS_DECODE_STAT
{
    EN_DECODE_STAT_HEADER = 0,
    EN_DECODE_STAT_QUESTION,
    EN_DECODE_STAT_ANSWER,
    EN_DECODE_STAT_SUCCESS,
    EN_DECODE_STAT_ERR,
    EN_DECODE_STAT_DONE,
};

//must add the len parameter to avoid the memory overrride
static  void __DNS_DecodeName(unsigned  char *name,int len)
{
    unsigned char *s;
    unsigned char num;

    s = name;
    while((*s != '\0')&&((ptu32_t)s < ((ptu32_t)name + len)))
    {
        num = *s+1;
        *s = '.';
        s += num;
    }

    return;
}

static void __DNS_EncodeName(unsigned char *name)
{
    char *s,*tmp;
    char *pos;
    char num;

    s = (char *)name;
    pos =(char *) name-1;
    while(*s != '\0')
    {
        tmp = strchr(s,'.');
        if(tmp == NULL)
        {
            num = strlen(s);
            *pos = num;
            s += num;
            pos = s;
        }
        else //got the '.'
        {
            num = tmp -s;
            *pos = num;
            s = tmp+1;
            pos = tmp;
        }
    }

    return;
}

static void __DNS_PackageData(const char *resolvename,unsigned char *buf, int *len)
{
    tagDnsHeader *header;
    int name_len, datalen;
    unsigned char  *p, *pkt;
    u16 qtype = htons(DNS_A_RECORD);
    u16 qclass = htons(IN);
    pkt = p = (unsigned char *)buf;

    /* Prepare DNS packet header */
    header           = (tagDnsHeader *) pkt;
    header->tid      = htons(1);
    header->flags    = htons(0x100);    /* standard query */
    header->nqueries = htons(1);        /* Just one query */
    header->nanswers = htons(0);
    header->nauth    = htons(0);
    header->nother   = htons(0);

    /* Encode DNS name */
    p = (unsigned char *) &header->data;    /* For encoding host name into packet */

    p++;
    name_len = strlen(resolvename) +1;
    memcpy((void *)p, (void *)resolvename,name_len);
    __DNS_EncodeName(p);
    p += name_len;

    memcpy((void *)p,(void *)&qtype,sizeof(qtype));
    p+=sizeof(qtype);

    memcpy((void *)p,(void *)&qclass,sizeof(qclass));
    p+=sizeof(qclass);

    datalen = (int)(p-pkt);
    *len = datalen;

    return;
}


static void  __DNS_UnpackageData(unsigned char *data, unsigned int datalen)
{
    u32 stat;
    int len;
    unsigned char *p;
    unsigned short rsclen;
    tagDnsHeader  *header;
    tagDnsQType   qtype;
    tagDnsAType   atype;

    stat= EN_DECODE_STAT_HEADER;
    p = data;

    while((stat != EN_DECODE_STAT_DONE)&&(p < (data +datalen)))
    {
        switch(stat)
        {
            case EN_DECODE_STAT_HEADER:
                header = (tagDnsHeader  *)p;
                p += sizeof(tagDnsHeader);
                header->tid = ntohs(header->tid);
                header->flags = ntohs(header->flags);
                header->nqueries = ntohs(header->nqueries);
                header->nanswers = ntohs(header->nanswers);
                header->nauth = ntohs(header->nauth);
                header->nother = ntohs(header->nother);

                if((header->nanswers <1)||(header->nqueries >1))
                {
                    stat = EN_DECODE_STAT_DONE;
                }
                else
                {
                    stat = EN_DECODE_STAT_QUESTION;
                }
                break;
            case EN_DECODE_STAT_QUESTION:
                len = strlen((const char *)p) +1;
                gDnsCNameAddr[gDnsCNameOffset] = p+1;
                gDnsCNameOffset++;
                __DNS_DecodeName(p,len);
                p+=len;

                memcpy((void *)&qtype,p,sizeof(qtype));
                qtype.type = ntohs(qtype.type);
                qtype.class = ntohs(qtype.class);
                p += sizeof(qtype);

                stat = EN_DECODE_STAT_ANSWER;
                break;
            case EN_DECODE_STAT_ANSWER:
                if(*p == 0xc0)
                {
                    //this is the offset name
//                    debug_printf("dns","%s:answer domain name:offset:%d\n\r",__FUNCTION__,p[1]);
                    p += 2;
                }
                else
                {
                    len = strlen((const char *)p) +1;
                    gDnsCNameAddr[gDnsCNameOffset] = p+1;
                    gDnsCNameOffset++;
                    __DNS_DecodeName(p,len);
                    p+=len;
                }

                memcpy((void *)&atype,p,sizeof(atype));
                atype.type = ntohs(atype.type);
                atype.class = ntohs(atype.class);
                atype.ttl = ntohl(atype.ttl);
                p += sizeof(atype);

                memcpy((void *)&rsclen,p,sizeof(rsclen));
                p += sizeof(rsclen);
                rsclen = ntohs(rsclen);

                if((atype.type == DNS_A_RECORD)&&(atype.class == IN))
                {
                    //this is the ipaddress
                    gDnsINameAddrV4[gDnsINameV4Offset] = p;
                    gDnsINameV4Offset++;
                }
                else if((atype.type == DNS_CNAME_RECORD)&&(atype.class == IN))
                {
                    if(*p != 0xc0)
                    {
                        len = strlen((const char *)p) +1;
                        gDnsCNameAddr[gDnsCNameOffset] = p+1;
                        gDnsCNameOffset++;
                        __DNS_DecodeName(p,len);
                    }
                }
                else
                {

                }
                p += rsclen;
                break;
            default:
                break;
        }
    }
    gDnsCNameAddr[gDnsCNameOffset] = NULL;
    gDnsINameAddrV4[gDnsINameV4Offset] = NULL;

    return ;
}


void  DNS_UnpackageDataExt(unsigned char *data, unsigned int datalen, struct StDnsResult *pOutDnsRes)
{
    u32 stat;
    int len;
    unsigned char *p;
    unsigned short rsclen;
    tagDnsHeader  *header;
    tagDnsQType   qtype;
    tagDnsAType   atype;
    unsigned char  nDnsCNameOffset=0;
    unsigned char  nDnsINameV4Offset=0;
    size_t min = 0;

    stat= EN_DECODE_STAT_HEADER;
    p = data;

    while((stat != EN_DECODE_STAT_DONE)&&(p < (data +datalen)))
    {
        switch(stat)
        {
            case EN_DECODE_STAT_HEADER:
                header = (tagDnsHeader  *)p;
                p += sizeof(tagDnsHeader);
                header->tid = ntohs(header->tid);
                header->flags = ntohs(header->flags);
                header->nqueries = ntohs(header->nqueries);
                header->nanswers = ntohs(header->nanswers);
                header->nauth = ntohs(header->nauth);
                header->nother = ntohs(header->nother);

                if((header->nanswers <1)||(header->nqueries >1))
                {
                    stat = EN_DECODE_STAT_DONE;
                }
                else
                {
                    stat = EN_DECODE_STAT_QUESTION;
                }
                break;
            case EN_DECODE_STAT_QUESTION:
                len = strlen((const char *)p) +1;
                min = sizeof(pOutDnsRes->arrDnsCNameAddr[nDnsCNameOffset])-1;
                min = (min < strlen((const char *)p+1)) ? min : strlen((const char *)p+1);
                memcpy(pOutDnsRes->arrDnsCNameAddr[nDnsCNameOffset], p+1,  min);
                nDnsCNameOffset++;
                __DNS_DecodeName(p,len);
                p+=len;

                memcpy((void *)&qtype,p,sizeof(qtype));
                qtype.type = ntohs(qtype.type);
                qtype.class = ntohs(qtype.class);
                p += sizeof(qtype);

                stat = EN_DECODE_STAT_ANSWER;
                break;
            case EN_DECODE_STAT_ANSWER:
                if(*p == 0xc0)
                {
                    //this is the offset name
 //                   debug_printf("dns","%s:answer domain name:offset:%d\n\r",__FUNCTION__,p[1]);
                    p += 2;
                }
                else
                {
                    len = strlen((const char *)p) +1;
                    min = sizeof(pOutDnsRes->arrDnsCNameAddr[nDnsCNameOffset])-1;
                    min = (min < strlen((const char *)p+1)) ? min : strlen((const char *)p+1);
                    memcpy(pOutDnsRes->arrDnsCNameAddr[nDnsCNameOffset], p+1,  min);
                    //gDnsCNameAddr[nDnsCNameOffset] = p+1;
                    nDnsCNameOffset++;
                    __DNS_DecodeName(p,len);
                    p+=len;
                }

                memcpy((void *)&atype,p,sizeof(atype));
                atype.type = ntohs(atype.type);
                atype.class = ntohs(atype.class);
                atype.ttl = ntohl(atype.ttl);
                p += sizeof(atype);

                memcpy((void *)&rsclen,p,sizeof(rsclen));
                p += sizeof(rsclen);
                rsclen = ntohs(rsclen);

                if((atype.type == DNS_A_RECORD)&&(atype.class == IN))
                {
                    //this is the ipaddress
                    memcpy(pOutDnsRes->arrDnsINameAddrV4[nDnsINameV4Offset], p, 4);
                    nDnsINameV4Offset++;
                }
                else if((atype.type == DNS_CNAME_RECORD)&&(atype.class == IN))
                {
                    if(*p != 0xc0)
                    {
                        len = strlen((const char *)p) +1;
                        min = sizeof(pOutDnsRes->arrDnsCNameAddr[nDnsCNameOffset])-1;
                        min = (min < strlen((const char *)p+1)) ? min : strlen((const char *)p+1);
                        memcpy(pOutDnsRes->arrDnsCNameAddr[nDnsCNameOffset], p+1,  min);
                        nDnsCNameOffset++;
                        __DNS_DecodeName(p,len);
                    }
                }
                else
                {

                }
                p += rsclen;
                break;
            default:
                break;
        }
    }
    memset(pOutDnsRes->arrDnsCNameAddr[nDnsCNameOffset], 0x00, sizeof(pOutDnsRes->arrDnsCNameAddr[nDnsCNameOffset]));
    memset(pOutDnsRes->arrDnsINameAddrV4[nDnsINameV4Offset], 0x00, sizeof(pOutDnsRes->arrDnsINameAddrV4[nDnsINameV4Offset]));
    return ;
}

#define CN_DNS_SERVER_PORT  53
#define CN_DNS_MSGBUF_LEN   0x100
static struct hostent       gHostEnt;
static unsigned char        gDnsBuf[CN_DNS_MSGBUF_LEN];
struct hostent  *DNS_NameResolve(const char *name)
{
    struct hostent *result = NULL;
    struct sockaddr_in addr;
    int sock;
    u32 dnsip;
    int datalen;
    int msglen;
    int addrlen;
    int opt =1;

    sock = socket(AF_INET, SOCK_DGRAM,0);
    if(sock == -1)
    {
        goto EXIT_DNSMAIN;
    }

    if(false == NetDev_GetDefaultDns(EN_IPV_4,&dnsip,NULL))
    {
        goto EXIT_DNSMAIN;
    }

    opt = 5*1000*mS;  //time out time
    if(0 != setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,&opt,sizeof(opt)))
    {
        goto EXIT_DNSMAIN;
    }

    memset((void *)&addr,0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(CN_DNS_SERVER_PORT);
    addr.sin_addr.s_addr = dnsip;

    __DNS_PackageData(name,gDnsBuf,&datalen);
    __DNS_ClearResult();

    msglen = sendto(sock,gDnsBuf,datalen,0,(struct sockaddr*)&addr,sizeof(addr));

    if(msglen != datalen)
    {
        goto EXIT_DNSMAIN;
    }
    msglen = recvfrom(sock,gDnsBuf,CN_DNS_MSGBUF_LEN,0,(struct sockaddr *)&addr,&addrlen);

    if((msglen > 0)&&(addr.sin_addr.s_addr == dnsip))
    {
        //this is the msg we need
        __DNS_UnpackageData(gDnsBuf,msglen);
        gHostEnt.h_name = (char *)name;
        gHostEnt.h_addrtype = AF_INET;
        gHostEnt.h_length = 4;
        gHostEnt.h_aliases = (char **)gDnsCNameAddr;
        gHostEnt.h_addr_list = (char **)gDnsINameAddrV4;

        result = &gHostEnt;
    }
    else
    {
        goto EXIT_DNSMAIN;
    }

EXIT_DNSMAIN:
    closesocket(sock);
    return result;
}

int DNS_NameResolveExt(const char *name, struct hostent_ext *phostent_ext)
{
    //struct hostent *result = NULL;
    int ret = -1;
    struct sockaddr_in addr;
    int sock;
    u32 dnsip;
    int datalen;
    int msglen;
    int addrlen;
    int opt =1;
    unsigned char *pnew = 0;

    pnew = (unsigned char*)malloc(CN_DNS_MSGBUF_LEN);
    if(pnew==NULL) return -1;

    sock = socket(AF_INET, SOCK_DGRAM,0);
    if(sock == -1)
    {
        goto EXIT_DNSMAIN;
    }

    if(false == NetDev_GetDefaultDns(EN_IPV_4,&dnsip,NULL))
    {
        goto EXIT_DNSMAIN;
    }

    opt = 5*1000*mS;  //time out time
    if(0 != setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,&opt,sizeof(opt)))
    {
        goto EXIT_DNSMAIN;
    }

    memset((void *)&addr,0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(CN_DNS_SERVER_PORT);
    addr.sin_addr.s_addr = dnsip;


    __DNS_PackageData(name,pnew,&datalen);
    //__DNS_ClearResult();

    msglen = sendto(sock,pnew,datalen,0,(struct sockaddr*)&addr,sizeof(addr));

    if(msglen != datalen)
    {
        goto EXIT_DNSMAIN;
    }
    msglen = recvfrom(sock,pnew,CN_DNS_MSGBUF_LEN,0,(struct sockaddr *)&addr,&addrlen);

    if((msglen > 0)&&(addr.sin_addr.s_addr == dnsip))
    {
        //this is the msg we need
        struct StDnsResult *ptmp = &phostent_ext->dns_res;
        DNS_UnpackageDataExt(pnew,msglen, ptmp);

        size_t len = sizeof(phostent_ext->arr_name)-1;
        len = (len<strlen(name))?len:strlen(name);
        memcpy(phostent_ext->h_name, name, len);
        phostent_ext->h_addrtype = AF_INET;
        phostent_ext->h_length = sizeof(struct in_addr);

        //gHostEnt.h_name = (char *)name;
        //gHostEnt.h_addrtype = AF_INET;
        //gHostEnt.h_length = 4;
        //gHostEnt.h_aliases = (char **)gDnsCNameAddr;
        //gHostEnt.h_addr_list = (char **)gDnsINameAddrV4;
        //result = &gHostEnt;
        ret = 0;
    }
    else
    {
        goto EXIT_DNSMAIN;
    }

EXIT_DNSMAIN:
    closesocket(sock);
    if(pnew) {
        free(pnew);
        pnew = 0;
    }
    return ret;
}




//bool_t DnsNameResolveShell(char *param)
bool_t DNS_NetGetHostByName(char *param)
{
    bool_t result = true;
    char *name;
    char **alias;
    char **addr;
    struct hostent *host;
    char   ipstr[20];

    name = param;
    debug_printf("dns","%s:Resolve Name:%s\n\r",__FUNCTION__,name);
    host = DNS_NameResolve(name);

    if(NULL != host)
    {
        debug_printf("dns","%s:GetName:%s addrtype:%d lenth:%d\n\r",\
                __FUNCTION__,host->h_name,host->h_addrtype,host->h_length);

        alias = host->h_aliases;
        while(NULL != *alias)
        {
            debug_printf("dns","%s:alias:%s\n\r",__FUNCTION__,*alias);
            alias++;
        }
        addr = host->h_addr_list;
        while(NULL != *addr)
        {
            if(inet_ntop(AF_INET,*addr,ipstr,20) != NULL)
            {
                debug_printf("dns","%s:ip:%s\n\r",__FUNCTION__,ipstr);
            }
            addr++;
        }
    }
    else
    {
        debug_printf("dns","%s:Resolve Name:%s--FAILED\n\r",__FUNCTION__,name);
    }

    return result;
}


ADD_TO_ROUTINE_SHELL(netgethostbyname,DNS_NetGetHostByName,"usage:netgethostbyname hostname");

