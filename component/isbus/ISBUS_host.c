//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

//================================================================================
//������ƣ�Host_ISBUS
//�����������ҵ�������ߣ�Industrial Serial Bus��ͨ��ģ�������˲���
//�����汾��V1.00
//������Ա:YT
//================================================================================
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <djyos.h>
#include <object.h>
#include <lock.h>
#include <multiplex.h>
#include <device.h>
#include <dbug.h>
//#include <driver.h>
#include <systime.h>
#include "ISBUS.h"
#include "ISBUS_Protocol.h"
#include "component_config_isbus.h"

struct IM_Pkg
{
    struct IM_Pkg *next;
    u8 *IM_buf;
};

//�ӻ�����
struct SlaveList                             //�ӻ��б�
{
    struct Host_ISBUSPort *CommPort;         //��Ӧ��ͨ�ſ�ָ��
    u8  Address;                             //�ӻ���ַ
//    u32 Timeout;                             //��ʱʱ��
    struct SlaveList *Next;                     //���ӻ����һ��ѭ������ĩ��ָ���ͷ
};


//ͨ�Ŷ˿ڵ�����
struct Host_ISBUSPort
{
    struct Host_ISBUSPort *Next;            //��ɵ�������ĩ��ָ��NULL
    struct ISBUS_FunctionSocket *SocketHead; //��ɵ���ѭ������
    struct ISBUS_FunctionSocket *SocketCurrent; //ָ��ǰ���յĹ��ܺ�ֵ
    struct SlaveList* SlaveHead;            //�ӻ�����ͷ������ѭ������
    struct SlaveList* SlaveTail;            //�ӻ�����β
    struct SlaveList* SlaveCurrent;         //�ӻ����е�ǰ��ѯ
    struct SemaphoreLCB *PortSemp;          //���������͵���Ϣ���С�
    s32 SerialDevice;                       //��Ӧ���豸
    ISBUS_FntProtocolError fnError;         //�����Ӧ�Ļص�����
    u32 ErrorPkgs;                          //�ۼƴ�����
    u32 ErrorLast;                          //���һ�δ����
    u32 Timeout;                            //��ʱʱ��
    u32 Resttime;                           //�봥����ʱʣ��ʱ��
    s32 SendTimes;                          //����buffer�е����ݷ��ʹ�����-1��ʾ���޴���
    u8  EchoModel;                          //��ǰӦ��ģʽ�� ONE_BY_ONE ��
    u32 PollCycle;                          //��ѯ���ڣ�uS�������� PollModel �й�
    u8  PollModel;                          //��ѯģʽ��ȡֵ CN_POLL_FASTEST ��
    u8  fsm;                                //��ǰЭ�����״̬
    s16 analyzeoff;                         //���ָ��
    s16 recvoff;                            //��ǰ����ָ��
    u8 *ResendPkgBuf;                       //����ָ�뱸�ݣ������ط�
    u8 *PollSendPkgBuf;                     //�������ݰ�ָ�루����Э��ͷ��
    struct IM_Pkg *IM_Head;                 //�������Ͱ�����ͷ
    struct IM_Pkg *IM_Tail;                 //�������Ͱ�����β
    u8 *RecvPkgBuf;                         //�������ݰ�ָ�루����Э��ͷ��
    u16 PortMaxRecvLen;                     //���˿������հ����ȣ�����Э��ͷ
    u16 PortMaxSendLen;                     //���˿�����Ͱ����ȣ�����Э��ͷ
//  u16 PortSendLen;                        //���˿����ڷ��͵İ����ȣ�����Э��ͷ
//    u16 RecvP;                              //����ƫ����������Э��ͷ
//    u32 SendP;                              //����ƫ��������������Э��ͷ
    u8 MTC_Address;                         //���˿ڽ��ܵ��鲥��ַ�����������鲥
                                            //����ΪCN_MTC_RESV��Ĭ�ϣ�
};

struct Host_ISBUSPort *sg_ptHostPortHead = NULL; //�����������ʼ��

struct SemaphoreLCB *UartTimerSemp  = NULL; //�ź���
static u16 sg_ptHostEvtt;
u8 COUNT_Slave;
u8 Effective_Slave=0;
u8 g_TimeOut=0;  //��ʱ��־λ
bool_t __HostSendPkg(struct Host_ISBUSPort *Port, u8 resend, u8 *dst);


struct ISBUS_FunctionSocket * __Host_GetProtocol(struct Host_ISBUSPort *Port,u8 Protocol)
{
    struct ISBUS_FunctionSocket *Next,*SocketStart;
    bool_t found = false;
    SocketStart = Port->SocketCurrent;
    if(SocketStart != NULL)
    {
        Next = SocketStart;
        do
        {
            if(Next->Protocol == Protocol)
            {
                found = true;
                break;
            }
            Next = Next->Next;
        } while(Next != SocketStart);
    }
    if(found)
    {
        Port->SocketCurrent = Next; //��ס��ǰֵ���´β���������ͬһ�����ܡ�
        return Next;
    }
    else
        return NULL;
}

//-----------------------------------------------------------------------------
//���ܣ���Ե㷽ʽ���ղ��������ݰ�
//������Port���������ݰ��Ķ˿�
//      src���������ݰ���Դ��ַ���㲥��ַ
//���أ�true= �ɹ���false=  ʧ��
//-----------------------------------------------------------------------------
bool_t __ISBUS_UniProcess(struct Host_ISBUSPort *Port,u8 src)
{
    struct ISBUS_FunctionSocket *Me;
    struct ISBUS_Protocol protohead;
    u32 starttime;
    s32 DevRe = Port->SerialDevice;
    u8 *protobuf;
    u8 chk,len;
    s16 restlen,Completed,readed,startoffset,tmp,tmp1;
    s16 exdata;

    bool_t Gethead = false;

    if(src >= CN_INS_MULTICAST)
        return false;
    protobuf = Port->RecvPkgBuf;
    startoffset = Port->analyzeoff;
    readed = Port->recvoff;
    starttime = (u32)DjyGetSysTime();
    while(1)
    {
        if(startoffset == readed)
        {
            startoffset = 0;
            readed = 0;
            Port->analyzeoff = 0;
            Port->recvoff = 0;
        }
        if((readed >= 256) && (startoffset > 128))
        {
            memcpy(protobuf, &protobuf[startoffset], readed-startoffset);
            readed -= startoffset;
            startoffset = 0;
            Port->analyzeoff = 0;
            Port->recvoff = readed;
        }
        tmp = DevRead(DevRe, &protobuf[readed], 256+sizeof(struct ISBUS_Protocol) - readed, 0, Port->Timeout);
        if(tmp != 0)
        {
            if(debug_ctrl ==true)
            {
                printf("\r\nhost recv:");
                for(tmp1 = 0; tmp1<tmp;tmp1++)
                    printf("%02x ",protobuf[tmp1+readed]);
            }
            readed += tmp;
        }
        if( ! Gethead)
        {
            for(; startoffset < readed;startoffset++)
            {
                if(protobuf[startoffset] == 0xEB)       //�ҵ���ͷ��������
                {
                    Gethead = true;
                    break;
                }
            }
        }
        if(Gethead && (readed - (s16)startoffset >= (s16)sizeof(struct ISBUS_Protocol)))
        {
            if((protobuf[startoffset + CN_OFF_DST] != 0)     //ֻ�շ��������İ���������ַ����0
                ||(protobuf[startoffset + CN_OFF_SRC] != src))  //У��Դ��ַ
            {
                Gethead = false;
                startoffset++;
                continue;       //startoffset���䣬whileѭ����,�ӵ�ǰλ������Ѱ�� 0xEB
            }
            else
            {
                chk = 0xEB + 0 + protobuf[startoffset + CN_OFF_PROTO]
                               + src
                               + protobuf[startoffset + CN_OFF_LEN];    //����chk
                if(chk == protobuf[startoffset + CN_OFF_CHKSUM])
                {
                    break;       //�ҵ��Ϸ���Э��ͷ���˳�ѭ�������������ݰ�
                }
                else
                {
                    if(Port->fnError != NULL)
                        Port->fnError((void*)Port, CN_INS_CHKSUM_ERR);
                    Port->ErrorLast = CN_INS_CHKSUM_ERR;
                    Port->ErrorPkgs++;
                    startoffset++;
                    Gethead = false;
                    printf("\r\nhost chksum error:");
                    continue;       //startoffset���䣬whileѭ����,�ӵ�ǰλ������Ѱ�� 0xEB
                }
            }
            break;
        }
        else        //һ����ʱ���ڹ�ȥ�ˣ�û�յ�������Э��ͷ���϶���ʱ�ˡ�
                    //�˴���ӻ���һ�����ӻ�ֻ���յ����ְ�ʱ�г�ʱ����Ϊ�ӻ�ʱ�������ա�
                    //�����Ƿ�����ȴ��ӻ���Ӧ���ӻ�����������Ӧ��
        {
            if(((u32)DjyGetSysTime() - starttime) > Port->Timeout)
            {
                if(Port->fnError != NULL)
                    Port->fnError((void*)Port, CN_INS_TIMEROUT_ERR);
                Port->ErrorLast = CN_INS_TIMEROUT_ERR;
                Port->ErrorPkgs++;
                Port->analyzeoff = 0;
                Port->recvoff = 0;
                return false;
                printf("\r\nhost protocol timeout");
            }
        }

    };
    memcpy((u8*)&protohead, protobuf+startoffset, sizeof(struct ISBUS_Protocol));
    len = protobuf[startoffset + CN_OFF_LEN];
    //��������Ҫ���յ��ֽ���������Ѿ��������գ�����Ǹ���
    restlen = len - (readed-startoffset - sizeof(struct ISBUS_Protocol));
    Completed = 0;
    if(restlen > 0)                 //����������Ҫ����
    {
        //���½����û�Э���
        if((Port->PortMaxRecvLen-readed) < restlen)    //buf��ʣ��ռ䲻���������ݰ�
        {
            //bufʣ��ռ䲻�����Ȱ��ѽ��յ������ƶ���buf�Ŀ�ͷ��
            memcpy(protobuf,protobuf+startoffset,readed-startoffset);
            readed -= startoffset;
            startoffset = 0;
        }
        while(((u32)DjyGetSysTime() - starttime) < Port->Timeout)
        {
            tmp = DevRead(DevRe, &protobuf[readed],restlen, 0, Port->Timeout);
            Completed += tmp;
            readed += tmp;
            if(Completed >= restlen)
                break;
        }
        exdata = Completed - restlen;
    }
    else
        exdata = 0 - restlen;


    if(exdata >= 0)
    {
        Me = __Host_GetProtocol(Port, protohead.Protocol);
        if(Me != NULL)
        {
            if(Me->MyProcess != NULL)
                Me->MyProcess(Me,src,protobuf+startoffset+sizeof(struct ISBUS_Protocol),len);
        }
        startoffset += sizeof(struct ISBUS_Protocol) + len;
        if(startoffset == readed)
        {
            Port->analyzeoff = 0;
            Port->recvoff = 0;
        }
        else
        {
            Port->analyzeoff = startoffset;
            Port->recvoff = readed;
        }
        return true;
    }
    else
    {
        if(Port->fnError != NULL)
            Port->fnError((void*)Port, CN_INS_TIMEROUT_ERR);
        Port->ErrorLast = CN_INS_TIMEROUT_ERR;
        Port->ErrorPkgs++;
        Port->analyzeoff = 0;
        Port->recvoff = 0;
        printf("\r\nhost data timeout");
        return false;
    }
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool_t __ISBUS_BroadcastProcess(struct Host_ISBUSPort *Port,u8 src)
{
    u8 slaveaddr;
    struct SlaveList *Current = Port->SlaveHead;
    if(Current == NULL)
        return false;
    do
    {
        slaveaddr = Current->Address;
        __ISBUS_UniProcess(Port, slaveaddr);
        Current = Current->Next;
    }while(Current != Port->SlaveHead);
    return true;
}

bool_t __ISBUS_MulticastProcess(struct Host_ISBUSPort *Port,u8 src)
{
    return true;
}
#pragma GCC diagnostic pop

// ============================================================================
// �������ܣ�������Э�鴦����
// ���������  ReplyModel��Ӧ��ģʽ��ȡֵ����
//                   ONE_BY_ONE �����ӻ�һ��һ��ģʽ��
//                   BOOADCAST_MODEL�����ӻ�һ�ʶ��ģʽ�����ͨѶЧ�ʡ��㲥ģʽ
//                   MULTICAST_MODEL�����ӻ�һ�ʶ��ģʽ�����ͨѶЧ�ʡ��鲥ģʽ
//        PoolModel����ʱ��ѯģʽ��ȡֵ����
//                   NO_TIMER_POLL����ʱ���ڲ���Ҫ�󣬸����ӻ��ж��Ͷ��ظ���
//                   EVENLY_SPACED_POLL���ȼ��������ѯ�����ӻ���
//                   ACCUMU_TIMER_POLL���ܼ��������ѯ���дӻ���
//        PoolTimes����ѯ������ȡ UNLIMITED_NUMBER ʱΪ�ܶ���ʼ����ѯ
//        Protocol�����ܺ�
//        GroupModel������ѯ�Ĵӻ�����ȫ���򲿷��飬�����û�����
//                   GroupModelȡALL_GROUP_POLLʱȫ����ѯ��������ֵΪ��������
//        SlaveNum���ӻ�������
//        Send_Buff�������͵����ݰ�������Э��ͷ
//        len���������ݰ�����
// ����ֵ��  ��
// ============================================================================
ptu32_t ISBUS_HostProcess(void)
{
    struct Host_ISBUSPort *Port;
    s64 Polltime;
    u8 dst;
    bool_t send,resend = false;
    Djy_GetEventPara((ptu32_t*)&Port, NULL);
    Polltime = DjyGetSysTime();
    while(1)
    {
        send = __HostSendPkg(Port, resend, &dst);   //dst������ȡ���Ͱ���dst��ַ
        if(send == true)
        {
            if(dst < CN_INS_MULTICAST)          //��Ե�ͨ��,һӦһ��ģʽ
            {
                resend = ! __ISBUS_UniProcess(Port,dst);          //���ղ��������ݰ�
                //ע��������ѯֱ�Ӽ���ִ����һ��whileѭ�������뵥������
                if(Port->PollModel == CN_POLL_SAME_INTERVAL)   //�ȼ����ѯ
                {
                    Polltime += Port->PollCycle;        //��ʱPollCycle�����ѯÿ���ӻ��ļ��ʱ��
                    Djy_EventDelayTo(Polltime);
                }
                else if(Port->PollModel == CN_POLL_SAME_CYCLE)         //��������ѯ
                {
                    if(dst == Port->SlaveTail->Address)
                    {
                        Polltime += Port->PollCycle;    //��ʱPollCycle������ѯһ�����ڵ�ʱ��
                        Djy_EventDelayTo(Polltime);
                    }
                }
            }
            else       //�鲥/�㲥��ַ���鲥���ڵĴӻ������ϴ�
            {
                resend = ! __ISBUS_UniProcess(Port,dst);
                if(Port->PollModel == CN_POLL_SAME_CYCLE)         //��������ѯ
                {
                    Polltime += Port->PollCycle;    //��ʱPollCycle������ѯһ�����ڵ�ʱ��
                    Djy_EventDelayTo(Polltime);
                }
            }
        }
        else
        {
            //����portʱ���¼�������������ʱpollcycle = 0����û�����ݰ�Ҫ���ͣ������¼�100%ռ��CPU
            //������������ɾ���˷������ݰ�����pollcycle������Ϊ0���ᵼ���¼�100%ռ��CPU��
            if(Port->PollCycle < 20000)
                Djy_EventDelay(20000);
            else
                Djy_EventDelay(Port->PollCycle);
            resend = false;
        }
    }
    return 0;
}

// ============================================================================
// �������ܣ�ע��ģ��ʹ�õ�ͨ�Ŷ˿ڣ���ʵ�ϣ����Ǵ���Ҳ����ע��ġ�
//        ͨ�Ŷ˿ڱ��밴djyos���豸��������������������ˡ�
// ���������dev��ͨ�Ŷ˿ڵ��豸ָ��
//        fnError��ͨ�Ŵ����Ӧ�Ļص�����������Ҫ���NULL
//        Timeout����ʱ��û���յ����ݵĳ�ʱ�澯���ޣ�uSΪ��λ����ʱ�侫����
//                ����ϵͳtick������ơ�0xffffffff��-1�������賬ʱ��
// ����ֵ��  ͨ�Ų��ָ��
// ============================================================================
struct Host_ISBUSPort *ISBUS_HostRegistPort(char *dev,
                                ISBUS_FntProtocolError fnError,u32 Timeout)
{
    struct SemaphoreLCB *Semp;
    struct Host_ISBUSPort *Port;
    u8 *recvbuf;
    s32 devfd;
    devfd = open(dev, O_RDWR);
    if(devfd == -1)
        return NULL;
    Port = (struct Host_ISBUSPort *)malloc(sizeof(struct Host_ISBUSPort ));
    recvbuf = malloc(512+2*sizeof(struct ISBUS_Protocol));
    Semp = Lock_SempCreate(1, 1, CN_BLOCK_FIFO, "ISBUS_Host");
    if((Port != NULL) && (recvbuf != NULL)&&(Semp != NULL)) //����ɹ�
    {
        memset(Port, 0, sizeof(struct Host_ISBUSPort ));
        Port->PortSemp = Semp;
        if(sg_ptHostPortHead == NULL)
        {
            Port->Next = NULL;
        }
        else
        {
            Port->Next = sg_ptHostPortHead;
        }
        sg_ptHostPortHead = Port;
        Port->SocketHead = NULL;
        Port->SocketCurrent = NULL;
        Port->SerialDevice = devfd;
        Port->fnError = fnError;
        Port->ErrorPkgs = 0;
        Port->ErrorLast = CN_INS_OK;
        Port->Timeout = Timeout;
        Port->Resttime = 0;
        Port->analyzeoff = 0;
        Port->recvoff = 0;
        Port->ResendPkgBuf = NULL;
        Port->PollSendPkgBuf = recvbuf + 256 + sizeof(struct ISBUS_Protocol);
        Port->RecvPkgBuf = recvbuf;
        Port->PortMaxRecvLen = sizeof(struct ISBUS_Protocol);
        Port->PortMaxSendLen = sizeof(struct ISBUS_Protocol);
//      Port->PortSendLen = 0;
//        Port->RecvP = 0;
        //����0����ʼ״̬�ᷢ��һ�����ݡ�
//        Port->SendP = sizeof(struct ISBUS_Protocol);  //����ƫ����ΪЭ��ͷ
        Port->MTC_Address = CN_INS_MTC_INVALID;
        dev_SetUserTag(devfd, (ptu32_t)Port);
//        Multiplex_AddObject(Port->MultiplexPort, devfd,
//                                           CN_MULTIPLEX_SENSINGBIT_READ
////                                            |  CN_MULTIPLEX_SENSINGBIT_WRITE  //���������ˣ���д������
//                                        |  CN_MULTIPLEX_SENSINGBIT_ERROR
//                                        |  CN_MULTIPLEX_SENSINGBIT_ET
//                                        |  CN_MULTIPLEX_SENSINGBIT_OR    );
        Djy_EventPop(sg_ptHostEvtt, NULL, 0, (ptu32_t)Port, 0, 0);
    }
    else
    {
        free(Port);
        free(recvbuf);
        Lock_SempDelete(Semp);
    }
    return Port;
}

// ============================================================================
// �������ܣ�ע��ͨ��Э�顣�����ˡ�
// ���������Port��ͨ�Ų��ָ�룬ΪINS_RegistPort�����ķ���ֵ
//        Protocol�����ܺţ��� CN_INS_PROTOCOL_DEFAULT �ȳ�������
//        MaxRecvLen����Э��������ݰ�����󳤶�
//        MaxSendLen����Э�鷢�����ݰ�����󳤶�
//        fn���������ݰ���Э�鴦����
// ����ֵ��  Э����ָ��
// ============================================================================
struct ISBUS_FunctionSocket *ISBUS_HostRegistProtocol(struct Host_ISBUSPort *Port, u8 Protocol,
                                          u16 MaxRecvLen,u16 MaxSendLen, ISBUS_FntProtocolProcess fn)
{
    struct ISBUS_FunctionSocket *ProtocolSocket;
    if(Port == NULL)
        return NULL;
    ProtocolSocket = (struct ISBUS_FunctionSocket *)malloc(sizeof(struct Host_ISBUSPort ));
    if(ProtocolSocket != NULL)
    {
        //result���ڳ����жϣ�������ָ���Ƿ�NULL����Ϊ�е���������ͨ�ſڣ�Len=0
        if(MaxRecvLen > Port->PortMaxRecvLen - sizeof(struct ISBUS_Protocol))
        {
//          Port->RecvPkgBuf = (u8*)realloc(Port->RecvPkgBuf,  //�������ݰ�ָ�루����Э��ͷ��
//                                      (MaxRecvLen+ sizeof(struct ISBUS_Protocol)));

            Port->PortMaxRecvLen = (MaxRecvLen+ sizeof(struct ISBUS_Protocol));
//          result = (Port->RecvPkgBuf != NULL);
        }
        if(MaxSendLen > Port->PortMaxSendLen - sizeof(struct ISBUS_Protocol)) //���Ͳ���
        {
//          Port->PollSendPkgBuf = (u8*)realloc(Port->PollSendPkgBuf,
//                                     MaxSendLen + sizeof(struct ISBUS_Protocol));
            Port->PortMaxSendLen = MaxSendLen + sizeof(struct ISBUS_Protocol);
//          result |= (Port->RecvPkgBuf != NULL);
        }
        if(Port->SocketHead == NULL)
        {
            ProtocolSocket->Next = ProtocolSocket;
            Port->SocketHead = ProtocolSocket;
            Port->SocketCurrent = ProtocolSocket;
        }
        else
        {
            ProtocolSocket->Next = Port->SocketHead->Next;
            Port->SocketHead->Next = ProtocolSocket;
        }
        ProtocolSocket->Protocol = Protocol;           //���ܺ�
        ProtocolSocket->ProtocolRecvLen = MaxRecvLen;  //��Э�������հ�����
        ProtocolSocket->ProtocolSendLen = MaxSendLen;  //��Э������Ͱ�����
        ProtocolSocket->CommPort = Port;               //��Ӧ��ͨ�ſ�ָ��
        ProtocolSocket->MyProcess = fn;                //Э�鴦����ָ��
    }
    else
    {
        debug_printf("ISBUS","ISBUS_HostRegistProtocol failure");
//      free(Port->RecvPkgBuf);
//      free(ProtocolSocket);
//      free(Port->PollSendPkgBuf);
//      return NULL;
    }
    return ProtocolSocket;
}
// ============================================================================
//���ܣ����ݰ����ͺ������ѷ��ͻ������е����ݷ��ͳ�ȥ������Ǽ�ʱ���ֱ�ӷ��ͳ�ȥ��
//      �������ѯ�����dst�ǵ㲥��ַ�������Ŀ�ĵ�ַ�ٷ��͡�
//������Port���������ݵĶ˿�
//      dst�����ط��͵�Ŀ�ĵ�ַ
//����ֵ�� ���͵�Ŀ�ĵ�ַ�����ڵ��÷�������һ�����չ��̡�
// ============================================================================
bool_t __HostSendPkg(struct Host_ISBUSPort *Port, u8 resend, u8 *dst)
{
    u8 *SendBuf;
    bool_t result;
    struct IM_Pkg *me;
    u8 SendLen,tmp;
    if((resend) && (Port->ResendPkgBuf != NULL))
    {
        SendBuf = Port->ResendPkgBuf;
        SendLen = SendBuf[CN_OFF_LEN] + sizeof(struct ISBUS_Protocol);
        DevWrite(Port->SerialDevice, SendBuf, SendLen,0,0);
        *dst = SendBuf[CN_OFF_DST];
        printf("\r\nhost resend:");
            for(tmp = 0;tmp < SendLen;tmp++)
            {
                printf("%02x ",SendBuf[tmp]);
            }
        result = true;
    }
    else
    {
        Lock_SempPend(Port->PortSemp,CN_TIMEOUT_FOREVER);
        if(Port->IM_Head != NULL)       //���ͼ�ʱ��Ϣ
        {
                me = Port->IM_Head;
                if(Port->IM_Head == Port->IM_Tail)
                {
                    Port->IM_Tail = NULL;
                    Port->IM_Head = NULL;
                }
                else
                {
                    Port->IM_Head = Port->IM_Head->next;
                }
                SendBuf = me->IM_buf;
                SendLen = SendBuf[CN_OFF_LEN] + sizeof(struct ISBUS_Protocol);
                DevWrite(Port->SerialDevice, SendBuf, SendLen,0,0);
                if((debug_ctrl ==true))
                {
                    printf("\r\nhost send:");
                    for(tmp = 0;tmp < SendLen;tmp++)
                    {
                        printf("%02x ",SendBuf[tmp]);
                    }
                }
                *dst = SendBuf[CN_OFF_DST];
                result = true;
                free(me);
                Port->ResendPkgBuf = SendBuf;
        }
        else                            //������������ѯ��Ϣ
        {
            if((Port->SendTimes != 0) && (Port->SlaveHead != NULL))
            {
                SendBuf = Port->PollSendPkgBuf;
                if(SendBuf[CN_OFF_DST] < CN_INS_MULTICAST)
                {
                    SendBuf[CN_OFF_DST] = Port->SlaveCurrent->Address;
                    Port->SlaveCurrent = Port->SlaveCurrent->Next;
                    if((Port->SlaveCurrent == Port->SlaveHead) && (Port->SendTimes != -1))
                        Port->SendTimes--;
                }
                else
                    if(Port->SendTimes != -1)
                        Port->SendTimes--;
                SendBuf[CN_OFF_CHKSUM]  = 0xEB + SendBuf[CN_OFF_DST]  + SendBuf[CN_OFF_PROTO]
                                          + SendBuf[CN_OFF_SRC]  + SendBuf[CN_OFF_LEN];
                *dst = SendBuf[CN_OFF_DST];
                SendLen = SendBuf[CN_OFF_LEN] + sizeof(struct ISBUS_Protocol);
                DevWrite(Port->SerialDevice, SendBuf, SendLen,0,0);
                if((debug_ctrl ==true))
                {
                    printf("\r\nhost send:");
                    for(tmp = 0;tmp < SendLen;tmp++)
                    {
                        printf("%02x ",SendBuf[tmp]);
                    }
                }
                result = true;
                Port->ResendPkgBuf = SendBuf;
            }
            else
            {
                *dst = 0;
                result = false;
            }
        }
        Lock_SempPost(Port->PortSemp);
    }

    return result;
}

// ============================================================================
//���ܣ�������ѯ���ݰ����������ͣ����ֵ��Լ�����ʱ�����͡�����ʱ��������ɨ�赽�Ĵӻ�
//      ��ַ�����θı�Ŀ�ĵ�ַ������������Ҫ���API��
//������Host_FunctionSocket��ͨ��Э����ָ�룬ΪISBUS_HostRegistProtocol�����ķ���ֵ
//        dst��Ŀ�ĵ�ַ�������ǹ㲥���鲥��ַ������ǵ㲥��ַ���������ԣ��Զ���Ϊ�ӻ�
//              �б��еĵ�һ���ӻ���ַ��
//        buf�������͵����ݰ�������Э��ͷ
//        len�����ͳ���
// ����ֵ��  ���͵���������ֻ��copy���˷���buf��
// ============================================================================
u32 ISBUS_SetPollPkg(struct ISBUS_FunctionSocket  *ISBUS_FunctionSocket,u8 dst,
                        u8 *buf, u8 len, s32 times)
{
    struct Host_ISBUSPort *Port;
    u8 *SendBuf;
    if(ISBUS_FunctionSocket == NULL)
        return 0;
    if(len > ISBUS_FunctionSocket->ProtocolSendLen)
        return 0;
    Port = ISBUS_FunctionSocket->CommPort;
    if(Port->SlaveHead == NULL)
        return 0;
    Lock_SempPend(Port->PortSemp,CN_TIMEOUT_FOREVER);
    Port->SendTimes = times;
    SendBuf = Port->PollSendPkgBuf;
    SendBuf[CN_OFF_START]   = 0xEB;
    if(dst < CN_INS_MULTICAST)
        SendBuf[CN_OFF_DST]     = Port->SlaveHead->Address;
    else
        SendBuf[CN_OFF_DST]     = dst;
    SendBuf[CN_OFF_PROTO]   = ISBUS_FunctionSocket->Protocol;
    SendBuf[CN_OFF_SRC]     = 0;
    SendBuf[CN_OFF_LEN]     = len;
    SendBuf[CN_OFF_CHKSUM]  = 0xEB + SendBuf[CN_OFF_DST] + SendBuf[CN_OFF_PROTO] + len;
    memcpy(SendBuf + sizeof(struct ISBUS_Protocol), buf, len);
    Lock_SempPost(Port->PortSemp);

    return len;
}

// ============================================================================
//���ܣ����ü�ʱ�������ݰ����������ͣ����ֵ��Լ�����ʱ�����͡�
//������ISBUS_FunctionSocket��ͨ��Э����ָ�룬ΪISBUS_HostRegistProtocol�����ķ���ֵ
//        dst��Ŀ�ĵ�ַ�����������ˣ�Ŀ�ĵ�ַΪ�ӻ���ַ��ԭ��ַΪ 0��
//             ���ڴӻ���Ŀ�ĵ�ַΪ������ַ��Դ��ַΪ�����ַ
//        buf�������͵����ݰ�������Э��ͷ
//        len�����ͳ���
// ����ֵ��  ���͵���������ֻ��copy���˷���buf��
// ============================================================================
u32 ISBUS_HostSetIM_Pkg(struct ISBUS_FunctionSocket  *ISBUS_FunctionSocket,u8 dst,
                            u8 *buf, u8 len)
{
    struct Host_ISBUSPort *Port;
    struct SlaveList *current;
    struct IM_Pkg *mypkg;
    u8 *SendBuf;
    bool_t found = false;
    if(ISBUS_FunctionSocket == NULL)
        return 0;
    if(len > ISBUS_FunctionSocket->ProtocolSendLen)
        return 0;
    Port = ISBUS_FunctionSocket->CommPort;
    current = Port->SlaveHead;
    if(current != NULL)
    {
        do
        {
            if(current->Address == dst)
            {
                found = true;
                break;
            }
            else
                current = current->Next;
        }while(current != Port->SlaveHead);
        if( ! found )                  //�ӻ������ڣ��ܾ�����
            return 0;
    }
    Lock_SempPend(Port->PortSemp,CN_TIMEOUT_FOREVER);
    mypkg = malloc(sizeof(struct IM_Pkg)+len+sizeof(struct ISBUS_Protocol));
    if(mypkg != NULL)
    {
        mypkg->IM_buf = (u8*)(mypkg+1);
        if(Port->IM_Tail == NULL)
        {
            Port->IM_Head = mypkg;
            Port->IM_Tail = mypkg;
        }
        else
        {
            Port->IM_Tail->next = mypkg;
            Port->IM_Tail = mypkg;
        }
        mypkg->next = NULL;

//      Port->PortSendLen = len;    //����Э��ͷ
        SendBuf = mypkg->IM_buf;
        SendBuf[CN_OFF_START]   = 0xEB;
        SendBuf[CN_OFF_DST]     = dst;
        SendBuf[CN_OFF_PROTO]   = ISBUS_FunctionSocket->Protocol;
        SendBuf[CN_OFF_SRC]     = 0;
        SendBuf[CN_OFF_LEN]     = len;
        SendBuf[CN_OFF_CHKSUM]  = 0xEB + dst + SendBuf[CN_OFF_PROTO] + 0 + len;
        memcpy(SendBuf + sizeof(struct ISBUS_Protocol), buf, len);
    }
    else
        len = 0;
    Lock_SempPost(Port->PortSemp);
    return len;
}

void __ISBUS_SentChkSlave(struct Host_ISBUSPort *Port,u8 dst)
{
    u8 *SendBuf;
    SendBuf = Port->PollSendPkgBuf;
    SendBuf[CN_OFF_START]   = 0xEB;
    SendBuf[CN_OFF_DST]     = dst;
    SendBuf[CN_OFF_PROTO]   = CN_CHK_SLAVE;
    SendBuf[CN_OFF_SRC]     = 0;
    SendBuf[CN_OFF_LEN]     = 0;
    SendBuf[CN_OFF_CHKSUM]  = 0xEB + dst + SendBuf[CN_OFF_PROTO];
    DevWrite(Port->SerialDevice, SendBuf, sizeof(struct ISBUS_Protocol),0,0);
    return ;
}

// ============================================================================
// �������ܣ���ʱ��ѯ�������á�
//       ˵����������ѯ����ǰ���˽�ӻ����ԣ������ӻ���ʱʱ�估����ظ�ʱ�䣬����ACCUMU_TIMER_POLL�ܼ����
//           ����ѯģʽ���ܵ�һȦ����ѯ����Ϊ�����ӻ���ʱʱ����������ö���Ķ�ʱʱ�䡣
// ���������timercycle����ʱ���ڣ���λus��NO_TIMER_POLLģʽ��ȡNULL.
//        PoolModel����ѯģʽ��CN_POLL_FASTEST      ����ʱ���ڲ���Ҫ���ж�����ѯ���
//                            CN_POLL_SAME_INTERVAL���ȼ��������ѯ�����ӻ� ��
//                            CN_POLL_SAME_CYCLE   ����������ѯ���дӻ�
// ����ֵ����
// ============================================================================
void ISBUS_PollConfig(struct Host_ISBUSPort *Port,u32 PollCycle,u8 PollModel)
{
    if(Port != NULL)
    {
        Port->PollModel = PollModel;
        Port->PollCycle = PollCycle;
    }
    return;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// ============================================================================
// ���ܣ�ɨ���ӻ��Ĺ��ܺ�������Ӵӻ������ò�����
// ���������InSerSocket�����ܲۺ�
//        src���ӻ���ַ
//        buf�����յ���Э�����ݰ��������ܲ�����
//        len�����ݰ����ȣ�����
// ����ֵ���ӻ��ṹ������ָ��
// ============================================================================
void __ISBUS_AddSlave(struct ISBUS_FunctionSocket  *InSerSocket , u8 src, u8 *buf, u32 len)
{
    struct Host_ISBUSPort *Port;
    Port = InSerSocket->CommPort;
    ISBUS_AddSlave(Port, src);
}
#pragma GCC diagnostic pop

void ISBUS_AddSlave(struct Host_ISBUSPort *Port, u8 address)
{
    struct SlaveList *p1;
    struct SlaveList* current;
    if(Port == NULL)
        return ;
    current = Port->SlaveHead;
    if(current != NULL)
    {
        do
        {
            if(current->Address == address)
                return;     //�ӻ��Ѿ�����
            else
                current = current->Next;
        }while(current != Port->SlaveHead);
    }
    p1=(struct SlaveList*)malloc(sizeof(struct SlaveList));//�����½ڵ�
    if(p1==NULL)
    {
        debug_printf("ISBUS","�ڴ�����ʧ��\r\n");
    }
    p1->CommPort = Port;
    p1->Address = address;
//    p1->Timeout = CN_LIMIT_UINT32;
    if(Port->SlaveHead == NULL)
    {
        p1->Next = p1;
        Port->SlaveHead = p1;
        Port->SlaveCurrent = p1;
        Port->SlaveTail = p1;
    }
    else if(Port->SlaveHead == Port->SlaveTail)
    {
        p1->Next = Port->SlaveHead;
        Port->SlaveHead->Next = p1;
        Port->SlaveTail = p1;
    }
    else
    {
        p1->Next = Port->SlaveHead;
        Port->SlaveTail->Next = p1;
        Port->SlaveTail = p1;
    }
}

// ============================================================================
// ���ܣ���ȡ�ӻ���ַ�б�ISBUSģ��ɨ�����п��ܵĵ�ַ��
// ������Port����ɨ��Ķ˿�
//      MaxAddress������ַ���
// ����ֵ���ӻ��б�ָ��
// ˵������������ϵ�����ɨ�裬�ӻ����ܻ�û�ϵ磬�ṩһ���ӿڣ�Ӧ�ó����ʱ��ɨ��
// ============================================================================
struct SlaveList* ISBUS_ScanSlave(struct Host_ISBUSPort *Port,u8 MinAddress,u8 MaxAddress)
{
    u8 SlaveAddress = MinAddress;
    ISBUS_HostRegistProtocol(Port, CN_CHK_SLAVE, 0, 0, __ISBUS_AddSlave);
    while(1)
    {
        __ISBUS_SentChkSlave(Port,SlaveAddress);
        __ISBUS_UniProcess(Port,SlaveAddress);
        SlaveAddress++;
        if(SlaveAddress > MaxAddress)
           break;
    }
    Port->SlaveCurrent = Port->SlaveHead;
    Port->ErrorLast = 0;
    Port->ErrorPkgs = 0;
    return Port->SlaveHead;
}

//-----------------------------------------------------------------------------
//���ܣ���ȡ�ӻ��б�������ʽ��
//������Port��Ŀ��˿�
//      address������ӻ���ַ���buffer�������NULL����ֻ���شӻ�����
//���أ��ӻ�����
//�÷��������� address = NULL���ã�����������ٷ����ڴ棬�ٴε��ñ�������
//-----------------------------------------------------------------------------
u8 ISBUS_GetSlaveTable(struct Host_ISBUSPort *Port,u8 *address)
{
    struct SlaveList* current;
    u8 result = 0;
    if(Port == NULL)
        return 0;
    current = Port->SlaveHead;
    if(current != NULL)
    {
        do
        {
            if(address != NULL)
                address[result] = current->Address;
            current = current->Next;
            result++;
       }while(current != Port->SlaveHead);
    }
    return result;
}

// ============================================================================
// ���ܣ����ʹӻ���ַ�б�
// ������Port�������͵Ķ˿�
//      address����ַ��ָ��
//      num���ӻ�����
// ����ֵ����
// ˵����
// ============================================================================
void ISBUS_SendSlaveTable(struct Host_ISBUSPort *Port,u8 *address, u8 num)
{
    struct ISBUS_FunctionSocket *sock;
    sock = ISBUS_HostRegistProtocol(Port, CN_SET_SLAVE_TABLE, 0, 0, NULL);
    ISBUS_HostSetIM_Pkg(sock,CN_INS_BROADCAST, address,num);
}

// ============================================================================
// ���ܣ����Ѵ����Ĵӻ���ɾ��ָ����ַ�ӻ���
// ������pHead���ӻ��ṹ������ָ��
//      address����ɾ���Ĵӻ���ַ
// ���أ� ��
// ============================================================================
void ISBUS_DeleteSlave (struct Host_ISBUSPort * Port, u8 address)
{
    struct SlaveList* current,*pre;

    if(Port == NULL)
        return ;
    current = Port->SlaveHead;
    pre = Port->SlaveTail;
    while(current != NULL)
    {
        if(address == current->Address)
        {
            if(current != current->Next)
            {
                pre->Next = current->Next;
                if(Port->SlaveHead == current)
                    Port->SlaveHead = current->Next;
                if(Port->SlaveCurrent == current)
                    Port->SlaveCurrent = current->Next;
                if(Port->SlaveTail == current)
                    Port->SlaveTail = current->Next;
            }
            else
            {
                Port->SlaveHead = NULL;
                Port->SlaveCurrent = NULL;
                Port->SlaveTail = NULL;
            }
            free(current);
            break;
        }
        pre = current;
        current = current->Next;
    }
    return;
}

//-----------------------------------------------------------------------------
//���ܣ������˳�ʼ��
//������HostStackSize��Ӧ�ó���ע��Ļص���������Ҫ��ջ�ռ�
//���أ���
//-----------------------------------------------------------------------------
void ISBUS_HostInit(u32 HostStackSize)
{

    sg_ptHostEvtt = Djy_EvttRegist(EN_INDEPENDENCE, CN_PRIO_REAL, 1, 1, ISBUS_HostProcess,
                            NULL, HostStackSize, "ISBUS host");
    if(sg_ptHostEvtt == CN_EVTT_ID_INVALID)
    {
        debug_printf("ISBUS","ISBUSͨ��ģ�������˳�ʼ���쳣\n\r");
        while(1);                               //��ʼ���׶ξ��쳣��ֱ����ѭ��
    }

}


