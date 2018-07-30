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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
#include <shell.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "dbug.h"
#include "newshell.h"
#include "../../component_config_tcpip.h"

#include "../../common/icmp.h"


#define CN_PING_DEFAULT_TIMEOUT   (1*1000*mS)   //1s
#define CN_PING_WAIT_TIME         (1*1000*mS)
#define CN_PING_DEFAULT_COUNTER   (4)
#define CN_PING_DEFAULT_SIZE      (60)
static void pingusage(void)
{
    debug_printf("ping","usage:\n\r\
-l         length\n\r\
-t         timeout\n\r\
-n         times\n\r \
-h         help\n\r");
    return;
}
// =============================================================================
// FUNCTION:This function is used to do the ping echo
// PARA  IN:param
//         :must be in the style:host name   times
// PARA OUT:
// RETURN  :
// INSTRUCT:
// =============================================================================
//static bool_t pingshell(char *param)
ADD_TO_SHELL_HELP(ping,"usage:ping hostname [options/-help]");
ADD_TO_IN_SHELL static bool_t ping(char *param)
{
    u32 i;
    u32 ip;
    u32 timeused;
    u32 timestart;
    u32 timeend;
    struct in_addr ipaddr;
    struct hostent *host;
    u32    times;
    u32    numrcv = 0;
    u32    waittime;
    int    len = CN_PING_DEFAULT_SIZE;

    int argc = 6;
    char *argv[6];

    times = CN_PING_DEFAULT_COUNTER;
    waittime = CN_PING_DEFAULT_TIMEOUT;

    argc = getargs(argc-1,&argv[1],param);
    if(argc < 1)
    {
        pingusage();
        return true;
    }
    else if(argc >1)
    {
        //do some set here
        i =2;
        argc++;
        while(i<argc)
        {
            if(0 == strcmp(argv[i],"-l"))
            {
                i++;
                if(i < argc)
                {
                    len = strtol(argv[i],NULL,NULL);
                    i++;
                }
            }
            else if(0 == strcmp(argv[i],"-n"))
            {
                i++;
                if(i < argc)
                {
                    times = strtol(argv[i],NULL,NULL);
                    i++;
                }
            }
            else if(0 == strcmp(argv[i],"-t"))
            {
                i++;
                if(i < argc)
                {
                    waittime = strtol(argv[i],NULL,NULL);
                    waittime = waittime*1000*mS;
                    i++;
                }
            }
            else if(0 == strcmp(argv[i],"-help"))
            {
                i++;
                pingusage();
            }
            else
            {
                i++;
            }
        }
    }
    if(0 == inet_aton(argv[1],&ipaddr))
    {
        //use the dns to get the ip
        host = gethostbyname(argv[1]);
        if(NULL != host)
        {
            memcpy((void *)&ipaddr,host->h_addr_list[0],sizeof(ipaddr));
        }
        else
        {
            debug_printf("ping","%s:Unknown host:%s\n\r",__FUNCTION__,param);
            return true;
        }
    }
    ip = ipaddr.s_addr;

    u8 *buf;
    if(len < 32)
    {
        len = 32;
    }
    len = len -28;
    buf = net_malloc(len);
    if(NULL == buf)
    {
        return true;
    }
    memset(buf,0x55,len);
    for(i = 0; i <times; i++)
    {
        timestart = (u32)DjyGetSysTime();
        if(Icmp_EchoRequest(ip,buf,len,CN_PING_WAIT_TIME))
        {
            timeend = (u32)DjyGetSysTime();
            timeused = (u32)(timeend - timestart);
            debug_printf("ping","0x%08x %s reply:Time = %d ms\n\r",i,inet_ntoa(ipaddr),timeused/1000);
            Djy_EventDelay(waittime);
            numrcv++;
        }
        else
        {
            debug_printf("ping","0x%08x %s reply:Timeout\n\r",i,inet_ntoa(ipaddr));
        }
    }
        debug_printf("ping","%s:snd:%d rcv:%d miss:%d\n\r",__FUNCTION__,times,numrcv,times-numrcv);
    net_free(buf);
    return true;
}





//bool_t Ping(char *param)
//{
//    u32 i;
//    u32 ip;
//    u32 timeused;
//    u32 timestart;
//    u32 timeend;
//    u8  datapad[CN_PING_DEFAULT_SIZE];
//    struct in_addr ipaddr;
//    struct hostent *host;
//    u32    times;
//    u32    numrcv = 0;
//    u32    waittime;
//
//    char *argv[3];
//    int argc = 3;
//    memset(argv,0,sizeof(argv));
//    if(NULL == param)
//    {
//        return false;
//    }
//    string2arg(&argc,argv,param);
//    if(argc == 0)
//    {
//      return false;
//    }
//
//    times = CN_PING_DEFAULT_COUNTER;
//    waittime = CN_PING_DEFAULT_TIMEOUT;
//    if(NULL != argv[1])
//    {
//        times = strtol(argv[1],NULL,0);
//    }
//    if(NULL != argv[2])
//    {
//        waittime = strtol(argv[2],NULL,0);
//        waittime =waittime*1000*mS;
//    }
//    if(0 == inet_aton(argv[0],&ipaddr))
//    {
//        //use the dns to get the ip
//        host = gethostbyname(argv[0]);
//        if(NULL != host)
//        {
//            memcpy((void *)&ipaddr,host->h_addr_list[0],sizeof(ipaddr));
//        }
//        else
//        {
//            debug_printf("ping","%s:Unknown host:%s\n\r",__FUNCTION__,param);
//            return true;
//        }
//    }
//    ip = ipaddr.s_addr;
//
//    memset(datapad,0x55,CN_PING_DEFAULT_SIZE);
//    for(i = 0; i <times; i++)
//    {
//        timestart = (u32)DjyGetSysTime();
//        if(Icmp_EchoRequest(ip,datapad,CN_PING_DEFAULT_SIZE,CN_PING_WAIT_TIME))
//        {
//            timeend = (u32)DjyGetSysTime();
//            timeused = (u32)(timeend - timestart);
//            debug_printf("ping","0x%08x %s reply:Time = %d ms\n\r",i,inet_ntoa(ipaddr),timeused/1000);
//            Djy_EventDelay(waittime);
//            numrcv++;
//        }
//        else
//        {
//            debug_printf("ping","0x%08x %s reply:Timeout\n\r",i,inet_ntoa(ipaddr));
//        }
//    }
//    debug_printf("ping","%s:snd:%d rcv:%d miss:%d\n\r",__FUNCTION__,times,numrcv,times-numrcv);
//    return true;
//}

//struct ShellCmdTab  gServicePing[] =
//{
//    {
//        "ping",
//        pingshell,
//        "usage:ping hostname [options/-help]",
//        "usage:ping hostname [options/-help]",
//    }
//};
//
//#define CN_PINGDEBUG_NUM  ((sizeof(gServicePing))/(sizeof(struct ShellCmdTab)))
//static struct ShellCmdRsc gServicePingCmdRsc[CN_PINGDEBUG_NUM];

//THIS IS PING MODULE FUNCTION
//bool_t ServicePingInit(ptu32_t para)
//{
//    bool_t result;
//
//    result = Sh_InstallCmd(gServicePing,gServicePingCmdRsc,CN_PINGDEBUG_NUM);
//
//    return result;
//}

