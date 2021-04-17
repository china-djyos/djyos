//----------------------------------------------------
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
//author:zhangqf
//date  :上午11:14:17/2017年4月18日
//usage :
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <os.h>
#include <cpu_peri.h>
#include <osarch.h>
#include <mosquitto.h>

#include <sys/socket.h>

#define CN_BUF_LEN        256
typedef struct
{
    struct SemaphoreLCB *sempr;        //used for the read wait semp
    struct SemaphoreLCB *sempw;        //used for the write wait semp
    u8     buf[CN_BUF_LEN];
    u16    datalen;
}tagRwBuf;  //used as the read and write buffer structure

typedef struct  __MosDev
{
    struct  __MosDev  *nxt;
    tagRwBuf rbuf;   //used for the read buffer
    tagRwBuf wbuf;   //used for the write buffer
    struct MutexLCB *mutex;  //used to protect the mosquitto device
    char *devname;   //used for the dev to create
    char *id;        //used for the client name
    char *user;      //used for the log user
    char *passwd;    //used for the log token
    char *host;      //used for the host name
    char *stopic;    //used for the topic subscribe
    char *ptopic;    //used for the topic pubic
    char *wtopic;    //used for the will topic pubic
    char *wmsg;      //used for the will message
    bool_t  clean;   //used for the mqtt clean each time or continue state
    bool_t  will;    //if the mqtt has any will
    bool_t  retain;  //if the message for the new client
    bool_t  debug;   //if the debug message is put
    u16     port;    //default is 1883
    u8      sqos;    //subscribe qos
    u8      pqos;    //publish qos
    u16     keeptime;//do the echo and req cycle

    //the following for the debug here
    bool_t  running; //debug to run or quit!
    u32     sndnum;
    u32     snderr;
    u32     rcvnum;
    u32     rcvdrop;

    void  *mos;      //the mosquitto control block
    fnNetDevEventHook hook; //used

}tagMqttDev;
static tagMqttDev *pMqttDevLst;  //this is the mosquitto device list

//app use this function to read the device
static u32 __DevRead(ptu32_t PrivateTag,u8 *buf,u32 len,u32 offset,u32 timeout)
{
    u32 result = 0;
    tagMqttDev *dev;

    dev = (tagMqttDev *)PrivateTag;
    if(NULL != dev)
    {
        if(Lock_SempPend(dev->rbuf.sempr,timeout))
        {
            if(Lock_MutexPend(dev->mutex,CN_TIMEOUT_FOREVER))
            {
                result = dev->rbuf.datalen > len?len:dev->rbuf.datalen;
                memcpy(buf,dev->rbuf.buf,result);
                dev->rbuf.datalen = 0;
                Lock_SempPost(dev->rbuf.sempw);  //tell the down layer to write data

                Lock_MutexPost(dev->mutex);
            }
        }
    }
    return result;
}


//app use this function to write the device
static u32 __DevWrite(ptu32_t PrivateTag,u8 *buf,u32 len,u32 offset,bool_t BlockOption,u32 timeout)
{
    int mid;
    int result = 0;
    tagMqttDev *dev;

    dev = (tagMqttDev *)PrivateTag;
    if((NULL != dev)&&(NULL != dev->mos))
    {
        TCPIP_DEBUG_INC(dev->sndnum);
        result = mosquitto_publish(dev->mos,&mid,dev->ptopic,len,buf,dev->pqos,dev->retain);
        if(result == MOSQ_ERR_SUCCESS)
        {
            result = len;
            if(dev->debug)
            {
                printf("%s:topic:%s qos:%d mid:%d len:%d\n\r",__FUNCTION__,dev->ptopic,dev->pqos,mid,len);
            }
        }
        else
        {
            TCPIP_DEBUG_INC(dev->snderr);
            printf("%s:error:%d\n\r",__FUNCTION__,result);
        }
    }
    return result;
}


//static functions for the mosquito client to inout or out put data
static u32 __Inputdata(tagMqttDev *dev,u8 *buf, u16 len,u32 timeout)
{
    u32 result = 0;
    if(NULL != dev)
    {
        TCPIP_DEBUG_INC(dev->rcvnum);
        if(Lock_SempPend(dev->rbuf.sempw,0))
        {
            if(Lock_MutexPend(dev->mutex,CN_TIMEOUT_FOREVER))
            {
                result =  CN_BUF_LEN> len?len:CN_BUF_LEN;
                memcpy(dev->rbuf.buf,buf,result);
                dev->rbuf.datalen = result;
                Lock_SempPost(dev->rbuf.sempr);  //tell the down layer to write data

                Lock_MutexPost(dev->mutex);
            }
        }
        else
        {
            TCPIP_DEBUG_INC(dev->rcvdrop);
            printf("Drops:%d bytes\n\r",len);
        }
    }
    return result;
}

//here is the mosquitto client work,we make the mosquitto client work as a
//device
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mosquitto.h>

static void msg_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
    tagMqttDev  *dev;
    dev = (tagMqttDev *)obj;
    if((NULL != message)&&(message->payloadlen > 0)&&(dev->debug))
    {
        int i = 0;
        printf("Topic:%s:hex:",message->topic);
        for(i =0;i<message->payloadlen;i++)
        {
            printf("%02x ",((u8 *)message->payload)[i]);
        }
        printf("\n\r");
        fflush(stdout);
    }
    __Inputdata(obj,message->payload,message->payloadlen,0);
    return;
}

static void con_callback(struct mosquitto *mosq, void *obj, int result)
{
    tagMqttDev  *dev;
    dev = (tagMqttDev *)obj;
    if(!result)
    {
        mosquitto_subscribe(mosq, NULL,dev->stopic, dev->sqos);
    }
    else
    {
        fprintf(stderr, "%s\n\r", mosquitto_connack_string(result));
    }
    return;
}

static void sub_callback(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
    tagMqttDev  *dev;
    dev = (tagMqttDev *)obj;
    if(dev->debug)
    {
        printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
    }

    return;
}

static void log_callback(struct mosquitto *mosq, void *obj, int level, const char *str)
{
    tagMqttDev  *dev;
    dev =   obj;
    if((NULL != dev)&&(dev->debug))
    {
        printf("%s\n\r", str);
    }
    return;
}

static ptu32_t __mqtttask(void)
{
    tagMqttDev  *dev;
    char err[1024];
    struct mosquitto *mosq;
    int rc;

    DJY_GetEventPara((ptu32_t *)&dev,NULL);
    if((NULL == dev)||(NULL!=dev->mos))
    {
        printf("%s:No dev or Mosq existed!\n\r",__FUNCTION__);
        return 0;
    }
    mosquitto_lib_init();
    mosq = mosquitto_new(dev->id, dev->clean, dev);
    if(NULL == mosq)
    {
        return 0;
    }

    mosquitto_username_pw_set(mosq, dev->user, dev->passwd);
    mosquitto_log_callback_set(mosq, log_callback);
    mosquitto_connect_callback_set(mosq, con_callback);
    mosquitto_message_callback_set(mosq, msg_callback);
    mosquitto_subscribe_callback_set(mosq, sub_callback);

    while(rc = mosquitto_connect(mosq, dev->host, dev->port,dev->keeptime))
    {
        if(dev->debug){
            if(rc == MOSQ_ERR_ERRNO){
                strerror_r(errno, err, 1024);
                fprintf(stderr, "Error: %s\n\r", err);
            }else{
                fprintf(stderr, "Unable to connect (%d).\n\r", rc);
            }
        }
    }//continue do the connect for the mosquitto client
    //set the mosq
    dev->mos = mosq;
    if(NULL != dev->hook)
    {
        dev->hook((ptu32_t)dev,EN_NETDEVEVENT_LINKUP);
    }
    dev->running = true;
    do{
        rc = mosquitto_loop(mosq, -1, 100);
        DJY_EventDelay(10*mS);
    }while((rc == MOSQ_ERR_SUCCESS)&&(dev->running));
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    if(rc){
        if(rc == MOSQ_ERR_ERRNO){
            fprintf(stderr, "Error: %s\n\r", strerror(errno));
        }else{
            fprintf(stderr, "Error: %s\n\r", mosquitto_strerror(rc));
        }
    }
    dev->mos = NULL;
    if(NULL != dev->hook)
    {
        dev->hook((ptu32_t)dev,EN_NETDEVEVENT_LINKDOWN);
    }

    return rc;
}

//usage:do the mosquittocfg here
#include <getopt.h>
static bool_t mqttconfig(char *param)
{
    int    argc = 10;
    char  *argv[10];

    tagMqttDev *dev;
    dev = pMqttDevLst;
    if(NULL == dev)
    {
        return false;
    }
    if(NULL!=param)
    {
        string2arg(&argc,&argv[1],param);
        argc++;
        argv[0] = __FUNCTION__;
        int ch;
        opterr = 0;
        while ((ch = getopt(argc,argv,"h:p:u:P:t:i:I:m:"))!=-1)
        {
                switch(ch)
                {
                        case 'h':
                                if(NULL != dev->host)
                                {
                                    free(dev->host);
                                    dev->host = NULL;
                                }
                                dev->host = strdup(optarg);
                                break;
                        case 'p':
                                dev->port = strtol(optarg,NULL,0);
                                break;
                        case 'u':
                                if(NULL != dev->user)
                                {
                                    free(dev->user);
                                    dev->user = NULL;
                                }
                                dev->user = strdup(optarg);
                                break;
                        case 'P':
                                if(NULL != dev->passwd)
                                {
                                    free(dev->passwd);
                                    dev->passwd = NULL;
                                }
                                dev->passwd = strdup(optarg);
                                break;
                        case 't':
                                if(NULL != dev->stopic)
                                {
                                    free(dev->stopic);
                                    dev->stopic = NULL;
                                }
                                dev->stopic = strdup(optarg);
                                break;
                        case 'i':
                                if(NULL != dev->id)
                                {
                                    free(dev->id);
                                    dev->id = NULL;
                                }
                                dev->id = strdup(optarg);
                                break;
                        default:
                                printf("other option :%c\n",ch);
                                break;
                }
        }
        //do the reset here
        optarg = NULL;
        optind = 0;
        opterr = 0;
        optopt = '?';
    }
    else
    {
        //print all the config status
        printf("mqttconfig:\n\r");
        printf("host    :%s\n\r",dev->host?dev->host:"NULL");
        printf("port    :%d\n\r",dev->port);
        printf("clean   :%s\n\r",dev->clean?"true":"false");
        printf("debug   :%s\n\r",dev->debug?"true":"false");
        printf("id      :%s\n\r",dev->id?dev->id:"NULL");
        printf("user    :%s\n\r",dev->user?dev->user:"NULL");
        printf("passwd  :%s\n\r",dev->passwd?dev->passwd:"NULL");
        printf("stopic  :%s  qos:%d\n\r",dev->stopic?dev->stopic:"NULL",dev->sqos);
        printf("ptopic  :%s  qos:%d\n\r",dev->ptopic?dev->ptopic:"NULL",dev->pqos);
        if(dev->will)
        {
            printf("willtopic:%s\n\r",dev->wtopic?dev->wtopic:"NULL");
            printf("willmsg  :%s\n\r",dev->wmsg?dev->wmsg:"NULL");
        }
    }
    return true;
}

//usage:we will create a virtual device with the name of id,you could read and write message to it
void * mqttcreate(char *devname,char *id,char *host,char *user,char *passwd,\
        char *stopic,char *ptopic,fnNetDevEventHook hook,bool_t debug)
{
    tagMqttDev *dev;

    //malloc some resource for the mosquitto client
    dev = malloc(sizeof(tagMqttDev));
    if(NULL == dev)
    {
        printf("%s:mem error!\n\r",__FUNCTION__);
        goto EXIT_MEM;
    }
    memset(dev,0,sizeof(tagMqttDev));

    dev->rbuf.sempr = Lock_SempCreate(1,0,CN_BLOCK_FIFO,NULL);
    if(NULL == dev->rbuf.sempr)
    {
        printf("%s:rbuf-rsemperr\n\r",__FUNCTION__);
        goto EXIT_RBUFR;
    }
    dev->rbuf.sempw = Lock_SempCreate(1,0,CN_BLOCK_FIFO,NULL);
    if(NULL == dev->rbuf.sempw)
    {
        printf("%s:rbuf-wsemperr\n\r",__FUNCTION__);
        goto EXIT_RBUFW;
    }
    dev->rbuf.sempw->lamp_counter =1;

    dev->mutex = Lock_MutexCreate(NULL);
    if(NULL == dev->mutex)
    {
        printf("%s:mutexerr\n\r",__FUNCTION__);
        goto EXIT_MUTEX;
    }

    //initialize all the configure to the default
    dev->port =1883;
    dev->clean = true;
    dev->debug = debug;
    dev->hook = hook;
    dev->keeptime = 60;//60 seconds

    if(NULL == devname)
    {
        dev->devname = strdup("mqttio");
    }
    else
    {
        dev->devname = strdup(devname);
    }

    if(NULL == host)
    {
        dev->host = strdup("iot1.mqtt.iot.gz.baidubce.com");
    }
    else
    {
        dev->host = strdup(host);
    }
    if(NULL == id)
    {
        dev->id = strdup("djyosterm");
    }
    else
    {
        dev->id = strdup(id);
    }
    if(NULL == user)
    {
        dev->user = strdup("zhangqf");
    }
    else
    {
        dev->user = strdup(user);
    }
    if(NULL == passwd)
    {
        dev->passwd = strdup("123456");
    }
    else
    {
        dev->passwd = strdup(passwd);
    }
    if(NULL == stopic)
    {
        dev->stopic = strdup("subtopics");
    }
    else
    {
        dev->stopic = strdup(stopic);
    }
    if(NULL == ptopic)
    {
        dev->ptopic = strdup("pubtopics");
    }
    else
    {
        dev->ptopic = strdup(ptopic);
    }

    //here we should create the mosquitto device here,then the app could open the device and
    //do the read and write
    Device_Create(dev->devname,NULL,NULL,__DevWrite,__DevRead,NULL,(ptu32_t)dev);
    //add it to the list
    dev->nxt = pMqttDevLst;
    pMqttDevLst = dev;

    return dev;

EXIT_MUTEX:
    Lock_SempDelete(dev->rbuf.sempw);
EXIT_RBUFW:
    Lock_SempDelete(dev->rbuf.sempr);
EXIT_RBUFR:
    free(dev);
    dev = NULL;
EXIT_MEM:
    return NULL;
}

//uages:used to set the publish topic
bool_t mqttsetpubtopic(char *topics)
{
    tagMqttDev *dev;
    dev = pMqttDevLst;
    if(NULL == dev)
    {
        return false;
    }
    if(NULL != dev->ptopic)
    {
        free(dev->ptopic);
        dev->ptopic = NULL;
    }
    dev->ptopic = strdup(topics);
    return true;
}
//usage:used to set the subscribe topic
bool_t mqttsetsubtopic(char *topics)
{
    tagMqttDev *dev;
    dev = pMqttDevLst;
    if(NULL == dev)
    {
        return false;
    }
    if(NULL != dev->stopic)
    {
        free(dev->stopic);
        dev->stopic = NULL;
    }
    dev->stopic = strdup(topics);
    return true;
}


//usage:check if the configuration is completed
static bool_t __mqttcfgcheck(tagMqttDev *dev)
{
    bool_t result = true;

    if(NULL == dev->devname)
    {
        printf("devname not set \n\r");
        result = false;
    }

    if(NULL == dev->id)
    {
        printf("id not set \n\r");
        result = false;
    }
    if(NULL == dev->host)
    {
        printf("host not set \n\r");
        result = false;
    }
    if((NULL != dev->user)&&(NULL == dev->passwd))
    {
        printf("need passwd\n\r");
        result = false;
    }
    if(NULL == dev->stopic)
    {
        printf("subscribe topic not set \n\r");
        result = false;
    }
    if(NULL == dev->ptopic)
    {
        printf("publish topic not set \n\r");
        result = false;
    }
    if(dev->will&&((NULL == dev->wtopic)||(NULL == dev->wmsg)))
    {
        printf("will topic or message not set \n\r");
        result = false;
    }

    return result;
}

//usage:use this function to start the mqtt or stop the mqtt task
static u16  gMqttEvttID = CN_EVTT_ID_INVALID;
bool_t mqttstart(void *handle)
{
    bool_t result = false;
    u16   eventID;
    tagMqttDev *dev;
    dev = handle;
    if(NULL != dev)
    {
        if(__mqttcfgcheck(dev))
        {
            //make a mqtt task
            if(gMqttEvttID != CN_EVTT_ID_INVALID)
            {
                eventID = DJY_EventPop(gMqttEvttID,NULL,0,(ptu32_t)dev,NULL,0);
                if(eventID != CN_EVENT_ID_INVALID)
                {
                    printf("event pop ok\n\r");
                }
                else
                {
                    printf("event pop error\n\r");
                }
            }
        }
    }
    else
    {
        printf("No mqtt get!\n\r");
    }
    return result;
}
bool_t mqttstartshell(char *param)
{

    tagMqttDev *dev;
    dev = pMqttDevLst;
    if(NULL != dev)
    {
        mqttstart(dev);
    }
    return true;
}
bool_t mqttstopshell(char *param)
{
    tagMqttDev *dev;
    dev = pMqttDevLst;
    if(NULL != dev)
    {
        dev->running = false;
    }
    return true;
}

//usage:used to set the debug on/off
bool_t mqttdebugset(bool_t mode)
{
    tagMqttDev *dev;
    dev = pMqttDevLst;
    if(NULL != dev)
    {
        dev->debug = mode;
    }
    return true;
}
bool_t mqttdebugsetshell(char *param)
{
    bool_t mode;
    if(0 == strcmp(param,"on"))
    {
        mode = true;
    }
    else
    {
        mode = false;
    }
    mqttdebugset(mode);
    return true;
}

static bool_t mqttpubmsg(char *param)
{
    int mid;
    int len = 0;
    int result = 0;
    tagMqttDev *dev;

    dev = pMqttDevLst;
    if(NULL != dev)
    {
        TCPIP_DEBUG_INC(dev->sndnum);
        len = strlen(param);
        result = mosquitto_publish(dev->mos,&mid,dev->ptopic,len,param,dev->pqos,dev->retain);
        if(result == MOSQ_ERR_SUCCESS)
        {
            if(dev->debug)
            {
                printf("%s:topic:%s qos:%d mid:%d len:%d\n\r",__FUNCTION__,dev->ptopic,dev->pqos,mid,len);
            }
        }
        else
        {
            TCPIP_DEBUG_INC(dev->snderr);
            printf("%s:error:%d\n\r",__FUNCTION__,result);
        }
    }
    return true;
}

static bool_t mqttstat(char *param)
{
    tagMqttDev *dev;
    dev = pMqttDevLst;
    if(NULL != dev)
    {
        //print all the config status
        printf("mqttconfig:\n\r");
        printf("dev     :%s\n\r",dev->devname?dev->devname:"NULL");
        printf("host    :%s\n\r",dev->host?dev->host:"NULL");
        printf("port    :%d\n\r",dev->port);
        printf("clean   :%s\n\r",dev->clean?"true":"false");
        printf("debug   :%s\n\r",dev->debug?"true":"false");
        printf("id      :%s\n\r",dev->id?dev->id:"NULL");
        printf("user    :%s\n\r",dev->user?dev->user:"NULL");
        printf("passwd  :%s\n\r",dev->passwd?dev->passwd:"NULL");
        printf("stopic  :%s  qos:%d\n\r",dev->stopic?dev->stopic:"NULL",dev->sqos);
        printf("ptopic  :%s  qos:%d\n\r",dev->ptopic?dev->ptopic:"NULL",dev->pqos);
        if(dev->will)
        {
            printf("willtopic:%s\n\r",dev->wtopic?dev->wtopic:"NULL");
            printf("willmsg  :%s\n\r",dev->wmsg?dev->wmsg:"NULL");
        }
        printf("snd:0x%08x snderr :0x%08x\n\r",dev->sndnum,dev->snderr);
        printf("rcv:0x%08x rcvdrop:0x%08x\n\r",dev->rcvnum,dev->rcvdrop);
    }
    return true;
}



#include <os.h>
#include <shell.h>

//work flow:mqttservicestart->add mqtt device ->configure the device->enable the device
//usage:use this function to do the mqtt  module initialize
int mqttserviceinit(void)
{
    bool_t result = false;
    gMqttEvttID = DJY_EvttRegist(EN_INDEPENDENCE,200,0,5,__mqtttask,NULL,0x1000,"mqtttask");
    if(gMqttEvttID == CN_EVTT_ID_INVALID)
    {
        printf("%s:evtt register error\n\r",__FUNCTION__);
    }
    else
    {
        result = true;
    }
    return result;
}


ADD_TO_ROUTINE_SHELL(mqttcfg,mqttconfig,"usage:moscfg -h[host] -p[port] -u[user] -P[passwd] -i[id] -t[topic] -m[message]");
ADD_TO_ROUTINE_SHELL(subset,mqttsetsubtopic,"usage:subset subtopics");
ADD_TO_ROUTINE_SHELL(pubset,mqttsetpubtopic,"usage:pubset pubtopics");
ADD_TO_ROUTINE_SHELL(mqttstart,mqttstartshell,"usage:mqttstart ");
ADD_TO_ROUTINE_SHELL(mqttstop,mqttstopshell,"usage:mqttstop ");
ADD_TO_ROUTINE_SHELL(mqttdebug, mqttdebugsetshell, "usage:mqttdebug on/off");
ADD_TO_ROUTINE_SHELL(mqttstat,mqttstat,"usage:mqttstat");
ADD_TO_ROUTINE_SHELL(mqttpub,mqttpubmsg,"usage:mqttpub messages");
