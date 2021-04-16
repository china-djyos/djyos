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
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#include <shell.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <misc/misc.h>
#include "dbug.h"
#include "../../component_config_tcpip.h"

#include "../../common/icmp.h"


#define CN_PING_DEFAULT_TIMEOUT   (1*1000*mS)   //1s
#define CN_PING_WAIT_TIME         (1*1000*mS)
#define CN_PING_DEFAULT_COUNTER   (4)
#define CN_PING_DEFAULT_SIZE      (60)
static void PING_Usage(void)
{
    debug_printf("ping","usage:\n\r\
-l         length\n\r\
-t         timeout\n\r\
-n         times\n\r \
-h         help\n\r");
    return;
}
// =============================================================================
// FUNCTION:This function is used to do the PING echo
// PARA  IN:param
//         :must be in the style:host name   times
// PARA OUT:
// RETURN  :
// INSTRUCT:
// =============================================================================
//static bool_t pingshell(char *param)
bool_t PING(char *param)
{
    s32 i;
    u32 ip;
    u32 timeused;
    u32 timestart;
    u32 timeend;
    struct in_addr ipaddr;
    struct hostent *host;
    s32    times;
    u32    numrcv = 0;
    u32    waittime;
    s32    len = CN_PING_DEFAULT_SIZE;

    s32 argc = 6;
    char *argv[6];

    times = CN_PING_DEFAULT_COUNTER;
    waittime = CN_PING_DEFAULT_TIMEOUT;

    argc = getargs(argc-1,&argv[1],param);
    if(argc < 1)
    {
        PING_Usage();
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
                    len = strtol(argv[i],NULL,0);
                    i++;
                }
            }
            else if(0 == strcmp(argv[i],"-n"))
            {
                i++;
                if(i < argc)
                {
                    times = strtol(argv[i],NULL,0);
                    i++;
                }
            }
            else if(0 == strcmp(argv[i],"-t"))
            {
                i++;
                if(i < argc)
                {
                    waittime = strtol(argv[i],NULL,0);
                    waittime = waittime*1000*mS;
                    i++;
                }
            }
            else if(0 == strcmp(argv[i],"-help"))
            {
                i++;
                PING_Usage();
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
            printf("ping %s:Unknown host:%s\n\r",__FUNCTION__,param);
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
//  memset(buf,0x55,len);
    //by chenws: 不可以设置为0x55, 有些服务器要求a-w重复，不然不回应。
    //例如：ping 183.232.231.172
    for(i=0; i<len; i++) {
        buf[i] = 'a' + i%23;
    }
    for(i = 0; i <times; i++)
    {
        timestart = (u32)DJY_GetSysTime();
        if(Icmp_EchoRequest(ip,buf,len,CN_PING_WAIT_TIME))
        {
            timeend = (u32)DJY_GetSysTime();
            timeused = (u32)(timeend - timestart);
            printf("ping  0x%08x %s reply:Time = %d ms\n\r",i,inet_ntoa(ipaddr),timeused/1000);
            DJY_EventDelay(waittime);
            numrcv++;
        }
        else
        {
            printf("ping 0x%08x %s reply:Timeout\n\r",i,inet_ntoa(ipaddr));
        }
    }
        printf("ping %s:snd:%d rcv:%d miss:%d\n\r",__FUNCTION__,times,numrcv,times-numrcv);
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
//    s32 argc = 3;
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
//        timestart = (u32)DJY_GetSysTime();
//        if(Icmp_EchoRequest(ip,datapad,CN_PING_DEFAULT_SIZE,CN_PING_WAIT_TIME))
//        {
//            timeend = (u32)DJY_GetSysTime();
//            timeused = (u32)(timeend - timestart);
//            debug_printf("ping","0x%08x %s reply:Time = %d ms\n\r",i,inet_ntoa(ipaddr),timeused/1000);
//            DJY_EventDelay(waittime);
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



//THIS IS PING MODULE FUNCTION
bool_t PING_ServicePingInit(void)
{
        return (TRUE);
}
ADD_TO_ROUTINE_SHELL(ping,PING,"usage:ping hostname [options/-help]");

