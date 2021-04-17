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
//author:zhangqf
//date  :����7:51:33/2017��3��14��
//usage :
//-----------------------------------------------------------------------------
//use this file to debug the mosquitto sub and publish
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <os.h>
#include <shell.h>


extern int Pubmain(int argc, char *argv[]);
extern int Submain(int argc, char *argv[]);
#define CN_NAME_LIMIT   0X40
static  char FuncName[CN_NAME_LIMIT]     = "mosquitto";
static  char Host[CN_NAME_LIMIT]         ="iot1.mqtt.iot.gz.baidubce.com";
static  char Port[CN_NAME_LIMIT]         ="1883";
static  char User[CN_NAME_LIMIT]         ="iot1/dev1";
static  char Passwd[CN_NAME_LIMIT]       ="dXNbXAY5WxyleSVbw6N/3pRLDjDmK1E7VIVer08YkZ4=";
static  char ClientSID[CN_NAME_LIMIT]     ="zhangqfsub";
static  char ClientPID[CN_NAME_LIMIT]     ="zhangqfpub";
static  char Topic[CN_NAME_LIMIT]        ="zhangqf";
static  char Message[CN_NAME_LIMIT]      ="Hello New World!";

ptu32_t subtask(void)
{
    char *argv[]={
        FuncName,
        "-h",
        Host,
       "-p",
        Port,
        "-u",
        User,
        "-P",
        Passwd,
        "-t",
        Topic,
        "-i",
        ClientSID,
        "-d",
    };
    int argc = (sizeof(argv))/(sizeof(char *));
    printf("start\n\r");

    Submain(argc,argv);
    return 0;
}

bool_t subshell(char *param)
{
    pthread_create(NULL,NULL,subtask,NULL);
    return true;
}
ptu32_t pubtask(void)
{
    char *argv[]={
        FuncName,
        "-d",
        "-h",
        Host,
       "-p",
        Port,
        "-u",
        User,
        "-P",
        Passwd,
        "-t",
        Topic,
        "-i",
        ClientPID,
        "-m",
        Message,
    };
    int argc = (sizeof(argv))/(sizeof(char *));
    Pubmain(argc,argv);
    return true;
}
bool_t pubshell(char *param)
{
    static u16 evttID = CN_EVTT_ID_INVALID;
    if(NULL != param)
    {
        strncpy(Message,param,CN_NAME_LIMIT);
    }

    if(evttID == CN_EVENT_ID_INVALID)
    {
        evttID = DJY_EvttRegist(EN_CORRELATIVE,200,1,1,pubtask,NULL,0x2000,"PUBTASK");
    }
    DJY_EventPop(evttID,NULL,0,NULL,NULL,0);
    return true;
}

#include <getopt.h>
static bool_t mosquittoconfig(char *param)
{
    int    argc = 10;
    char  *argv[10];

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
                                memset(Host,0,CN_NAME_LIMIT);
                                strncpy(Host,optarg,CN_NAME_LIMIT);
                                break;
                        case 'p':
                                memset(Port,0,CN_NAME_LIMIT);
                                strncpy(Port,optarg,CN_NAME_LIMIT);
                                break;
                        case 'u':
                                memset(User,0,CN_NAME_LIMIT);
                                strncpy(User,optarg,CN_NAME_LIMIT);
                                break;
                        case 'P':
                                memset(Passwd,0,CN_NAME_LIMIT);
                                strncpy(Passwd,optarg,CN_NAME_LIMIT);
                                break;
                        case 't':
                                memset(Topic,0,CN_NAME_LIMIT);
                                strncpy(Topic,optarg,CN_NAME_LIMIT);
                                break;
                        case 'i':
                                memset(ClientPID,0,CN_NAME_LIMIT);
                                strncpy(ClientPID,optarg,CN_NAME_LIMIT);
                                break;
                        case 'I':
                                memset(ClientSID,0,CN_NAME_LIMIT);
                                strncpy(ClientSID,optarg,CN_NAME_LIMIT);
                                break;
                        case 'm':
                                memset(Message,0,CN_NAME_LIMIT);
                                strncpy(Message,optarg,CN_NAME_LIMIT);
                                break;
                        default:
                                printf("other option :%c\n",ch);
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
        printf("mosquittpconfig:\n\r");
        printf("host :%s\n\r",Host);
        printf("port :%s\n\r",Port);
        printf("user :%s\n\r",User);
        printf("pass :%s\n\r",Passwd);
        printf("csid :%s\n\r",ClientSID);
        printf("cpid :%s\n\r",ClientPID);
        printf("topic:%s\n\r",Topic);
        printf("messg:%s\n\r",Message);
    }
    return true;
}

ADD_TO_ROUTINE_SHELL(sub,subshell,"usage:sub");
ADD_TO_ROUTINE_SHELL(pub,pubshell,"usage:pub [message]");
ADD_TO_ROUTINE_SHELL(moscfg,mosquittoconfig,"config the environment,usage:moscfg -h[host] -p[port] -u[user] -P[passwd] -I[subID] -i[pubid] -t[topic] -m[message]");

