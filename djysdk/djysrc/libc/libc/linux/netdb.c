/*
 * netdb.c
 *
 *  Created on: 2015Äê9ÔÂ6ÈÕ
 *      Author: zqf
 */

#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>


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
struct hostent *gethostbyname(const char *name)
{
    struct hostent *result;

    result = DnsNameResolve(name);

    return result;
}

static char gHostName[] = "DJYOS_TERNIMAL";
int  gethostname(char *name, int len)
{
	strcpy(name,gHostName);
	return 0;
}



//usage:use this data to storage the netdb errors
int h_errno;
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

	answer= malloc(sizeof(struct addrinfo));
	if(NULL == answer)
	{
		res = EAI_MEMORY;
		goto EXIT_INFOMEM;
	}
	memset(answer,0,sizeof(struct addrinfo));

	addr =malloc(sizeof(struct sockaddr_in));
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
	if((NULL == hostname)||(0 == strcmp(hostname,"localhost")))
	{
		addr->sin_family = AF_INET;
		addr->sin_addr.s_addr = INADDR_ANY;
		//look up the port
		if(NULL != service)
		{
			protoname = NULL;
			if(NULL != hints)
			{
				if(hints->ai_socktype == SOCK_STREAM)
				{
					protoname = "tcp";
				}
				else
				{
					protoname = "udp";
				}
				addr->sin_family = hints->ai_family;
			}
			serve = getservbyname(service,protoname);
			if(NULL == serve)
			{
				res = EAI_NODATA;
				goto EXIT_GETFAIL;
			}
			addr->sin_port = (in_port_t)serve->s_port;
			addr->sin_addr.s_addr = (in_addr_t)inet_addr("127.0.0.1");
		}
		answer->ai_family = addr->sin_family;
		answer->ai_addrlen = sizeof(struct sockaddr_in);
		answer->ai_addr = (struct sockaddr*)addr;
		answer->ai_next = NULL;
		if(NULL != hints)
		{
			answer->ai_socktype = hints->ai_socktype;
			answer->ai_protocol = hints->ai_protocol;
		}
		res = EAI_OK;
	}
	else
	{
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
			answer->ai_socktype = hints->ai_socktype;
			answer->ai_protocol = hints->ai_protocol;
		}
		res = EAI_OK;
	}
	*result = answer;
	return res;

EXIT_GETFAIL:
EXIT_SOCKTYPE:
EXIT_FAMILY:
EXIT_PARAM:
	free(addr);
EXIT_ADDRMEM:
	free(answer);
EXIT_INFOMEM:
	h_errno = res;
	return res;
}
//free the ai by getaddrinfo returned;
void freeaddrinfo (struct addrinfo*ai)
{
	if(NULL != ai)
	{
		if(NULL != ai->ai_addr)
		{
			free(ai->ai_addr);
		}
		free(ai);
	}
	return;
}


