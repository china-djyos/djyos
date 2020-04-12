//================================================================================
//������ƣ�ISBUS_slave
//�����������ҵ�������ߣ�Industrial Serial Bus��ͨ��ģ��ӻ��˲���
//�����汾��V1.00
//������Ա: YT
//================================================================================
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <djyos.h>
#include <object.h>
#include <lock.h>
#include <multiplex.h>
#include <device.h>
//#include <driver.h>
#include <systime.h>
#include "ISBUS.h"
#include "ISBUS_Protocol.h"
#include "timer_hard.h"
#include "dbug.h"
#include "component_config_isbus.h"

//ͨ�Ŷ˿ڵ�����
struct Slave_ISBUSPort
{
    struct Slave_ISBUSPort *Next;           //��ɵ�������ĩ��ָ��NULL
    struct ISBUS_FunctionSocket *SocketHead; //��ɵ���ѭ������
    struct ISBUS_FunctionSocket *SocketCurrent; //ָ��ǰ���յĹ��ܺ�ֵ
    struct MultiplexSetsCB *MultiplexPort;  //�˿ڶ�·����
    s32 SerialDevice;                       //��Ӧ���豸
    ISBUS_FntProtocolError fnError;         //�����Ӧ�Ļص�����
    u32 ErrorPkgs;                          //�ۼƴ�����
    u32 ErrorLast;                          //���һ�δ����
    u32 Timeout;                            //��ʱʱ��
    u32 Resttime;                           //�봥����ʱʣ��ʱ��
//  s32 SendTimes;                          //����buffer�е����ݷ��ʹ�����-1��ʾ���޴���
    u8  MTCcast[256];                       //����յ��Ĺ㲥���鲥���ݰ�����
    u8  EchoModel;                          //��ǰӦ��ģʽ�� ONE_BY_ONE ��
    u8  src;                                //��ǰ���յ��İ���Դ��ַ
    u8  dst;                                //��ǰ���յ��İ���Ŀ�ĵ�ַ
    s16 analyzeoff;                         //���ָ��
    s16 recvoff;                            //��ǰ����ָ��
    u8  BoardcastPre;                       //�����㲥��ѯ��ǰ���ַ
    u8  MTCPre;                             //�����鲥��ѯ��ǰ���ַ
    u8 *SendPkgBuf;                         //�������ݰ�ָ�루����Э��ͷ��
    u8 *RecvPkgBuf;                         //�������ݰ�ָ�루����Э��ͷ��
    u16 PortMaxRecvLen;                     //���˿������հ����ȣ�����Э��ͷ
    u16 PortMaxSendLen;                     //���˿�����Ͱ����ȣ�����Э��ͷ
//  u16 PortSendLen;                        //���˿����ڷ��͵İ����ȣ�����Э��ͷ
//    u16 RecvP;                              //����ƫ����������Э��ͷ
//    u32 SendP;                              //����ƫ��������������Э��ͷ
    u8 MTC_Address;                         //���˿ڽ��ܵ��鲥��ַ�����������鲥
                                            //����ΪCN_MTC_RESV��Ĭ�ϣ�
};

u8 sg_u8SlaveAddress = 1;                          //�ӻ����ص�ַ
struct Slave_ISBUSPort *sg_ptSlavePortHead = NULL; //�ӻ��������ʼ��
static u16 sg_ptSlaveEvtt;


struct ISBUS_FunctionSocket * __Slave_GetProtocol(struct Slave_ISBUSPort *Port,u8 Protocol)
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

// ============================================================================
// �������ܣ��ӻ���Э�鴦�������ô�����һ����Ϊһ�������ȼ����̵߳���
// �����������
// ����ֵ��  ��
// ============================================================================
ptu32_t ISBUS_SlaveProcess(void)
{
    u32 starttime,nowtime;
    struct Slave_ISBUSPort *Port;
    struct ISBUS_FunctionSocket *Me;
    struct ISBUS_Protocol protohead;
    s32 DevRe = Port->SerialDevice;
    u8 *protobuf;
    u8 chk,len, mydst;
    s16 restlen,Completed,readed,startoffset,tmp,tmp1;
    s16 exdata;

    Djy_GetEventPara((ptu32_t*)&Port, NULL);
    DevRe = Port->SerialDevice;
    mydst = sg_u8SlaveAddress;
    while(1)
    {

        bool_t Gethead = false;

        protobuf = Port->RecvPkgBuf;
        startoffset = Port->analyzeoff;
        readed = Port->recvoff;
        while(1)
        {
            starttime = (u32)DjyGetSysTime();
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
            tmp = DevRead(DevRe, &protobuf[readed], 256+sizeof(struct ISBUS_Protocol) - readed,
                                        0, Port->Timeout);
            if(tmp != 0)
            {
                if(debug_ctrl ==true)
                {
                    printf("\r\nslave recv:");
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
            if(Gethead && (readed - (s16)startoffset >= sizeof(struct ISBUS_Protocol)))
            {
                if((protobuf[startoffset + CN_OFF_DST] == mydst)    //������ַ
                    ||(protobuf[startoffset + CN_OFF_DST] >= CN_INS_MULTICAST)  //�㲥���鲥��ַ
                    ||(protobuf[startoffset + CN_OFF_SRC] == Port->BoardcastPre)//�㲥ǰ�õ�ַ
                    ||(protobuf[startoffset + CN_OFF_SRC] == Port->MTCPre))     //��ǰ�õ�ַ
                {
                    chk = 0xEB + protobuf[startoffset + CN_OFF_DST]
                               + protobuf[startoffset + CN_OFF_PROTO]
                               + protobuf[startoffset + CN_OFF_SRC]
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
                        continue;       //startoffset���䣬whileѭ����,�ӵ�ǰλ������Ѱ�� 0xEB
                    }
                }
                else
                {
                    Gethead = false;
                    startoffset++;
                    continue;       //startoffset���䣬whileѭ����,�ӵ�ǰλ������Ѱ�� 0xEB
                }
                break;
            }
            else        //һ����ʱ���ڹ�ȥ�ˣ�û�յ�������Э��ͷ���϶���ʱ�ˡ�
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
            while(1)
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

        Me = __Slave_GetProtocol(Port, protohead.Protocol);
        if(Me != NULL)
        {
            if((protohead.DstAddress == mydst) && (Me->MyProcess != NULL))
            {
                Me->MyProcess(Me, protohead.SrcAddress,
                              protobuf + startoffset + sizeof(struct ISBUS_Protocol), len);
            }
            else if(protohead.DstAddress >= CN_INS_MULTICAST)
            {
                Port->EchoModel = BROADCAST_MODEL;
                memcpy(Port->MTCcast, protobuf + startoffset + sizeof(struct ISBUS_Protocol),len);
                if((Port->BoardcastPre == 0) && (Me->MyProcess != NULL))     //������ַ�ǵ�һ���ӻ���
                {
                    Me->MyProcess(Me, protohead.SrcAddress,Port->MTCcast, len);
                }
            }
            else if((protohead.SrcAddress == Port->BoardcastPre)
                        && (Port->EchoModel == BROADCAST_MODEL)
                        && (Me->MyProcess != NULL))
            {
                Me->MyProcess(Me, protohead.SrcAddress,Port->MTCcast, len);
            }
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
        Port->src = protohead.SrcAddress;
        Port->dst = protohead.DstAddress;
//        return true;
    }
}

// ============================================================================
// �������ܣ���װ�ڲ�����ͨ��ģ�顣��ģ��������ڴӻ����֡�
// ���������StackSize��ģ����Ҫ���ڴ�ߴ磬���ڴ���ͨ��Э�������Ҫ�����û�
//              �ṩ�Ļص������������ݣ���ģ��������Ҫ���ڴ���٣���಻������
//              �ֽڣ����û������Լ��Ļص�����������
//       CN_PRIO_RLYMAIN���¼����ȼ������û��Լ�ѡȡ��һ��ѡ��ϸ����ȼ�
// ����ֵ�� ���ǳɹ������ɹ��ͽ�����ѭ����
// ============================================================================
bool_t ISBUS_SlaveInit(u32 StackSize)
{
    sg_ptSlaveEvtt = Djy_EvttRegist(EN_INDEPENDENCE, CN_PRIO_REAL, 1, 1, ISBUS_SlaveProcess,
                            NULL, StackSize, "ISBUS slave");
    if(sg_ptSlaveEvtt == CN_EVTT_ID_INVALID)
    {
        debug_printf("ISBUS","ISBUSͨ��ģ��ӻ��˳�ʼ���쳣\n\r");
        while(1);                               //��ʼ���׶ξ��쳣��ֱ����ѭ��
    }
    else
    {
        return true;
    }
}

//-----------------------------------------------------------------------------
//���ܣ�Э�鴦���������ڽ����鲥�б����ñ��˿��鲥�š��鲥ǰ���ַ�����ѱ��˿�
//      ��Ӧ��ģʽ��Ϊ�鲥����ģʽ��������Ч��
//������ProtocolSocket��Э���ָ��
//      buf�����յ������ݰ�
//      len�����ݰ�����
//���أ�������
//-----------------------------------------------------------------------------
void __SetMTC_Address(struct ISBUS_FunctionSocket *ProtocolSocket,u8 src, u8 *buf,u32 len)
{

}

//-----------------------------------------------------------------------------
//���ܣ�Э�鴦���������ڽ��մӻ��б����ñ��˿ڹ㲥ǰ���ַ�����ѱ��˿ڵ�Ӧ��
//      ģʽ��Ϊ�㲥����ģʽ��������Ч��
//������ProtocolSocket��Э���ָ��
//      buf�����յ������ݰ�
//      len�����ݰ�����
//���أ�������
//-----------------------------------------------------------------------------
void __SetSlaveList(struct ISBUS_FunctionSocket *ProtocolSocket,u8 src,u8 *buf,u32 len)
{
    struct Slave_ISBUSPort *Port;
    u8 nearest = 255,least = 255;
    u8 loop,now;
    Port = ProtocolSocket->CommPort;
    for(loop = 0;loop < len; loop++)
    {
        now = buf[loop];
        if(now < sg_u8SlaveAddress)
        {
            if(now- sg_u8SlaveAddress < nearest)
                nearest = now;
        }
        if(now < least)
            least = now;            //�ҳ��ӻ����б����С�ĵ�ַ
    }
    if(sg_u8SlaveAddress == least)  //�������ַ�����С������ǰ�õ�ַ��������ַ
        sg_ptSlavePortHead->BoardcastPre = 0;
    else
        sg_ptSlavePortHead->BoardcastPre = nearest;
}

//-----------------------------------------------------------------------------
//���ܣ�Э�鴦���������ڷ���������ѯ
//      ģʽ��Ϊ�㲥����ģʽ��������Ч��
//������ProtocolSocket��Э���ָ��
//      buf�����յ������ݰ�
//      len�����ݰ�����
//���أ�������
//-----------------------------------------------------------------------------
void __CHK_SlaveSend(struct ISBUS_FunctionSocket *ProtocolSocket,u8 src,u8 *buf,u32 len)
{
    ISBUS_SlaveSendPkg(ProtocolSocket, 0, NULL, 0);
}

// ============================================================================
// �������ܣ�ע��ʹ�õ�ͨ�Ŷ˿ڣ���ʵ�ϣ����Ǵ���Ҳ����ע��ġ��ӻ��ˡ�
//        ͨ�Ŷ˿ڱ��밴djyos���豸���������������
// ���������dev��ͨ�Ŷ˿ڵ��豸ָ��
//        fnError��ͨ�Ŵ����Ӧ�Ļص�����������Ҫ���NULL
//        Timeout����ʱ��û���յ����ݵĳ�ʱ�澯���ޣ�uSΪ��λ����ʱ�侫����
//                ����ϵͳtick������ơ�0xffffffff��-1�������賬ʱ��
// ����ֵ��  ͨ�Ų��ָ��
// ============================================================================
struct Slave_ISBUSPort *ISBUS_SlaveRegistPort(char *dev,\
                                ISBUS_FntProtocolError fnError,u32 Timeout)
{
    struct Slave_ISBUSPort *Port;
    u8 *recvbuf;
    s32 devfd;
    devfd = open(dev, O_RDWR);
    if(devfd == -1)
        return NULL;
    Port = (struct Slave_ISBUSPort *)malloc(sizeof(struct Slave_ISBUSPort ));
    recvbuf = malloc(512+2*sizeof(struct ISBUS_Protocol));
    if((Port != NULL) && (recvbuf != NULL)) //����ɹ�
    {
        memset(Port, 0, sizeof(struct Slave_ISBUSPort ));
        if(sg_ptSlavePortHead == NULL)
        {
            Port->Next = NULL;
        }
        else
        {
            Port->Next = sg_ptSlavePortHead;
        }
        sg_ptSlavePortHead = Port;//���뵽��Port����sg_ptPortHead��Port���м���
        Port->SocketHead = NULL;   //��Slave_FunctionSocket������г�ʼ��
        Port->SocketCurrent = NULL;
        Port->SerialDevice = devfd;
        Port->fnError = fnError;
        Port->ErrorPkgs = 0;
        Port->ErrorLast = CN_INS_OK;
        Port->Timeout = Timeout;
        Port->Resttime = 0;
        Port->analyzeoff = 0;
        Port->recvoff = 0;
        Port->SendPkgBuf = recvbuf + 256 + sizeof(struct ISBUS_Protocol);
        Port->RecvPkgBuf = recvbuf;
        Port->PortMaxRecvLen = sizeof(struct ISBUS_Protocol);
        Port->PortMaxSendLen = sizeof(struct ISBUS_Protocol);
//      Port->PortSendLen = 0;
//        Port->RecvP = 0;
        //����0����ʼ״̬�ᷢ��һ�����ݡ�
//        Port->SendP = sizeof(struct ISBUS_Protocol);  //����ƫ����ΪЭ��ͷ
        Port->MTC_Address = CN_INS_MTC_INVALID;
//        Driver_SetUserTag(dev, (ptu32_t)Port);
        dev_SetUserTag(devfd, (ptu32_t)Port);
//        Multiplex_AddObject(Port->MultiplexPort, devfd,
//                                           CN_MULTIPLEX_SENSINGBIT_READ
////                                            |  CN_MULTIPLEX_SENSINGBIT_WRITE  //���������ˣ���д������
//                                        |  CN_MULTIPLEX_SENSINGBIT_ERROR
//                                        |  CN_MULTIPLEX_SENSINGBIT_ET
//                                        |  CN_MULTIPLEX_SENSINGBIT_OR    );
        Djy_EventPop(sg_ptSlaveEvtt, NULL, 0, (ptu32_t)Port, 0, 0);
        ISBUS_SlaveRegistProtocol(Port, CN_SET_SLAVE_TABLE, 255, 0, __SetSlaveList);
        ISBUS_SlaveRegistProtocol(Port, CN_SET_MTC_TABLE, 255, 0, __SetMTC_Address);
        ISBUS_SlaveRegistProtocol(Port, CN_CHK_SLAVE, 0, 0, __CHK_SlaveSend);
    }
    else
    {
        free(Port);
        free(recvbuf);
    }
    return Port;
}

// ============================================================================
// �������ܣ�ע��ͨ��Э�顣�ӻ��ˡ�
// ���������Port��ͨ�Ų��ָ�룬ΪINS_RegistPort�����ķ���ֵ
//        Protocol�������룬�� CN_INS_PROTOCOL_DEFAULT �ȳ�������
//        MaxRecvLen����Э��������ݰ�����󳤶�
//        MaxSendLen����Э�鷢�����ݰ�����󳤶�
//        fn���������ݰ���Э�鴦����
// ����ֵ��  Э����ָ��
// ============================================================================
struct ISBUS_FunctionSocket *ISBUS_SlaveRegistProtocol(struct Slave_ISBUSPort *Port, u8 Protocol,
                                            u16 MaxRecvLen,u16 MaxSendLen, ISBUS_FntProtocolProcess fn)
{
    struct ISBUS_FunctionSocket *ProtocolSocket;
    bool_t result = true;
    if(Port == NULL)
        return NULL;
    ProtocolSocket = (struct ISBUS_FunctionSocket *)malloc(sizeof(struct Slave_ISBUSPort ));
    if(ProtocolSocket != NULL)
    {
        //result���ڳ����жϣ�������ָ���Ƿ�NULL����Ϊ�е���������ͨ�ſڣ�Len=0
        if(MaxRecvLen > Port->PortMaxRecvLen - sizeof(struct ISBUS_Protocol))
        {
//            Port->RecvPkgBuf = (u8*)realloc(Port->RecvPkgBuf,  //�������ݰ�ָ�루����Э��ͷ��
//                                        (MaxRecvLen+ sizeof(struct ISBUS_Protocol)));

            Port->PortMaxRecvLen = (MaxRecvLen+ sizeof(struct ISBUS_Protocol));
//            result = (Port->RecvPkgBuf != NULL);
        }
        if(MaxSendLen > Port->PortMaxSendLen - sizeof(struct ISBUS_Protocol)) //���Ͳ���
        {
//            Port->SendPkgBuf = (u8*)realloc(Port->SendPkgBuf,
//                                       MaxSendLen + sizeof(struct ISBUS_Protocol));
            Port->PortMaxSendLen = MaxSendLen + sizeof(struct ISBUS_Protocol);
//            result |= (Port->RecvPkgBuf != NULL);
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
        debug_printf("ISBUS","ISBUS_SlaveRegistProtocol failure");
//      free(Port->RecvPkgBuf);
//      free(ProtocolSocket);
//      free(Port->PollSendPkgBuf);
//      return NULL;
    }
    return ProtocolSocket;
}

// ============================================================================
// ���������÷������ݰ����ͺ��������û����ã�����������Э��ͷȻ���ͳ�ȥ���ӻ��ˡ�
// ���������Slave_FunctionSocket��ͨ��Э����ָ�룬ΪINS_RegistProtocol�����ķ���ֵ
//        dst�����ڴӻ���Ŀ�ĵ�ַΪ������ַ��Դ��ַΪ�����ַSlave_sg_u8Address
//        buf�������͵����ݰ�������Э��ͷ
//        len�����ͳ���
//        times�����ʹ�����-1��ʾ���޴�����
// ����ֵ��  ���͵���������ֻ��copy���˷���buf��
// ============================================================================
u32 ISBUS_SlaveSendPkg(struct ISBUS_FunctionSocket  *ISBUS_FunctionSocket, u8 dst, u8 *buf, u8 len)
{
    struct Slave_ISBUSPort *Port;
    u8 *SendBuf;
    u16 SendLen,tmp;
    if(ISBUS_FunctionSocket == NULL)
        return 0;
    if(len > ISBUS_FunctionSocket->ProtocolSendLen)
        return 0;
    Port = ISBUS_FunctionSocket->CommPort;
//  Port->SendTimes = times;
    SendBuf = Port->SendPkgBuf;
    SendBuf[CN_OFF_START]   = 0xEB;
    SendBuf[CN_OFF_DST]     = dst;
    SendBuf[CN_OFF_PROTO]   = ISBUS_FunctionSocket->Protocol;
    SendBuf[CN_OFF_SRC]     = sg_u8SlaveAddress;
    SendBuf[CN_OFF_LEN]     = len;
    SendBuf[CN_OFF_CHKSUM]  = 0xEB + dst + ISBUS_FunctionSocket->Protocol
                              + sg_u8SlaveAddress + len;
    SendLen = len + sizeof(struct ISBUS_Protocol);
    memcpy(SendBuf + sizeof(struct ISBUS_Protocol), buf, len);

    DevWrite(Port->SerialDevice, SendBuf, SendLen, 0, CN_TIMEOUT_FOREVER);
    if((debug_ctrl ==true))
    {
        printf("\r\nslave send:");
        for(tmp = 0;tmp < SendLen;tmp++)
        {
            printf("%02x ",SendBuf[tmp]);
        }
    }
//  if(Completed != -1)
//      Port->SendP = Completed;

    return len;
}

// ============================================================================
// �������ܣ����ôӻ��ı�����ַ��
// ���������Addr��������ַ��8λ
// ����ֵ�� ��
// ע�⣺����ע��˿�֮ǰ����
// ============================================================================
void ISBUS_SlaveSetAddress(u8 Addr)
{
    if(Addr < CN_INS_MULTICAST)  //���ܳ����鲥��ʼ��ַ
    {
        sg_u8SlaveAddress = Addr;
    }
}

// ============================================================================
// �������ܣ����ñ������ܵ��鲥��ַ��
// ���������Port�������õ�Ŀ��ͨ�ſ�
//           Addr���鲥��ַ��8λ
// ����ֵ�� ��
// ============================================================================
void ISBUS_SlaveSetMtcAddress(struct Slave_ISBUSPort *Port, u8 Addr)
{
    if((Addr >= CN_INS_MULTICAST) && (Addr != CN_INS_BROADCAST) && (Port != NULL))
        Port->MTC_Address = Addr;
}

