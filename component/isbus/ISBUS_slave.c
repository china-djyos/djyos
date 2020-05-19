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
#include <shell.h>
#include "component_config_isbus.h"

//ͨ�Ŷ˿ڵ�����
struct Slave_ISBUSPort
{
    struct Slave_ISBUSPort *Next;           //��ɵ�������ĩ��ָ��NULL
    struct ISBUS_FunctionSocket *SocketHead; //��ɵ���ѭ������
    struct ISBUS_FunctionSocket *SocketCurrent; //ָ��ǰ���յĹ��ܺ�ֵ
    struct MultiplexSetsCB *MultiplexPort;  //�˿ڶ�·����
    struct SemaphoreLCB *MTC_Semp;          //�㲥���鲥��ȷ��APP������ɲ����»ذ����ٷ��ͻذ�������
    s32 SerialDevice;                       //��Ӧ���豸
    ISBUS_FntProtocolError fnError;         //�����Ӧ�Ļص�����
    u32 ErrorPkgs;                          //�ۼƴ�����
    u32 ErrorLast;                          //���һ�δ����
    u32 Timeout;                            //��ʱʱ��
    u32 Resttime;                           //�봥����ʱʣ��ʱ��
//  s32 SendTimes;                          //����buffer�е����ݷ��ʹ�����-1��ʾ���޴���
    u8  MTCcast[256];                       //����յ��Ĺ㲥���鲥���ݰ�����
    u8  EchoModel;                          //��ǰӦ��ģʽ�� ONE_BY_ONE ��
    u8  HostSerial;                         //���յ������������
    u8  src;                                //��ǰ���յ��İ���Դ��ַ
    u8  dst;                                //��ǰ���յ��İ���Ŀ�ĵ�ַ
    s16 analyzeoff;                         //���ָ��
    s16 recvoff;                            //��ǰ����ָ��
    u8  BoardcastPre;                       //�����㲥��ѯ��ǰ���ַ���յ��ӻ���֮ǰ����Ӧ��㲥
    u8  MTCPre;                             //�����鲥��ѯ��ǰ���ַ���յ��鲥��֮ǰ����Ӧ��㲥
    u8 *SendPkgBuf;                         //�������ݰ�ָ�루����Э��ͷ��
    u8 *RecvPkgBuf;                         //�������ݰ�ָ�루����Э��ͷ��
    u16 PortMaxRecvLen;                     //���˿������հ����ȣ�����Э��ͷ
    u16 PortMaxSendLen;                     //���˿�����Ͱ����ȣ�����Э��ͷ
    u8 MTC_Address;                         //���˿ڽ��ܵ��鲥��ַ�����������鲥
                                            //����ΪCN_MTC_RESV��Ĭ�ϣ�
};

u8 sg_u8SlaveAddress = 1;                          //�ӻ����ص�ַ
struct Slave_ISBUSPort *sg_ptSlavePortHead = NULL; //�ӻ��������ʼ��
static u16 sg_ptSlaveEvtt;

void __ISBUS_Ack(struct Slave_ISBUSPort *Port);
void __ISBUS_PushMtcPkg(struct Slave_ISBUSPort *Port);

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

//struct dbgrecord
//{
//    u32 ev;
//    u32 time;
//    u8 len;
//    u8  data[64];
//};
//struct dbgrecord dbgrecord[1010] = {0};
//u32 offset485 = 0;
//
//void recdbg(u32 ev, u8 *buf, u32 len)
//{
//    u32 cpylen;
//
//    if(offset485 < 1000)
//    {
//        if(len < 64)
//            cpylen = len;
//        else
//            cpylen = 64;
//        if(cpylen != 0)
//            memcpy(dbgrecord[offset485].data, buf, cpylen);
//        dbgrecord[offset485].len = cpylen;
//        dbgrecord[offset485].time = DjyGetSysTime();
//        dbgrecord[offset485].ev = ev;
//        offset485++;
//    }
//    else
//        offset485 = 0;
//}

// ============================================================================
// �������ܣ��ӻ���Э�鴦�������ô�����һ����Ϊһ�������ȼ����̵߳���
// �����������
// ����ֵ��  ��
// ============================================================================
ptu32_t ISBUS_SlaveProcess(void)
{
    u32 starttime;
    struct Slave_ISBUSPort *Port;
    struct ISBUS_FunctionSocket *Me;
    struct ISBUS_Protocol protohead;
    s32 DevRe;
    u8 *protobuf;
    u8 chk,len, mydst;
    s16 restlen,Completed,readed,startoffset,tmp,tmp1;
    bool_t needread = true;
    bool_t newpkg;

    Djy_GetEventPara((ptu32_t*)&Port, NULL);
    DevRe = Port->SerialDevice;
    mydst = sg_u8SlaveAddress;
    while(1)
    {

        bool_t Gethead = false;

        protobuf = Port->RecvPkgBuf;
        startoffset = Port->analyzeoff;
        readed = Port->recvoff;
        starttime = (u32)DjyGetSysTime();
        while(1)        //��ѭ�����ڽ��հ�ͷ
        {
            if(startoffset == readed)
            {
                startoffset = 0;
                readed = 0;
                Port->analyzeoff = 0;
                Port->recvoff = 0;
                needread = true;
            }
            else if((readed >= 256) && (startoffset > 128))
            {
                memcpy(protobuf, &protobuf[startoffset], readed-startoffset);
                readed -= startoffset;
                startoffset = 0;
                Port->analyzeoff = 0;
                Port->recvoff = readed;
            }
            if(needread)
            {
//              recdbg(2, NULL,0);
                tmp = DevRead(DevRe, &protobuf[readed], 256+sizeof(struct ISBUS_Protocol) - readed,
                                                0, Port->Timeout);
                if(tmp != 0)
                {
//                  recdbg(1, &protobuf[readed],tmp);
                    if(debug_ctrl ==true)
                    {
                        printf("\r\nslave recv:");
                        for(tmp1 = 0; tmp1<tmp;tmp1++)
                            printf("%02x ",protobuf[tmp1+readed]);
                    }
                    readed += tmp;
                }
                else
                {
                    continue;
                }
            }
//            if((tmp == 12) && (protobuf[readed+7] == 4))
//                tmp = 12;
            if( ! Gethead)
            {
                for(; startoffset < readed;startoffset++)
                {
                    if(protobuf[startoffset] == 0xEB)       //�ҵ���ͷ��������
                    {
                        Gethead = true;
                        starttime = (u32)DjyGetSysTime();
                        break;
                    }
                    else
                        Gethead = false;
                }
            }
            if(Gethead )
            {
                if(readed - (s16)startoffset >= (s16)sizeof(struct ISBUS_Protocol))
                {
                    u8 *proto = protobuf + startoffset;
                    needread = false;
//                  if((proto[CN_OFF_DST] == mydst)    //������ַ
//                      ||(proto[CN_OFF_DST] >= CN_INS_MULTICAST)  //�㲥���鲥��ַ
//                      ||(proto[CN_OFF_SRC] == Port->BoardcastPre)//�㲥ǰ�õ�ַ
//                      ||(proto[CN_OFF_SRC] == Port->MTCPre))     //��ǰ�õ�ַ
//                  {
                        chk = 0xEB + proto[CN_OFF_DST]
                                   + proto[CN_OFF_PROTO]
                                   + proto[CN_OFF_SRC]
                                   + proto[CN_OFF_SERIAL]
                                   + proto[CN_OFF_LEN];    //����chk
                        if(chk == proto[CN_OFF_CHKSUM])
                        {
                            if(proto[CN_OFF_SRC] == 0)      //�鿴�Ƿ����������İ�
                            {
                                if(proto[CN_OFF_SERIAL] != Port->HostSerial)
                                {
                                    newpkg = true;
                                    Port->HostSerial = proto[CN_OFF_SERIAL];
                                }
                                else
                                    newpkg = false;
                            }
                            break;      //Э��ͷУ����ȷ���˳�ѭ���������պʹ������ݰ�
                        }
                        else            //��ͷУ�������¼������Ϣ
                        {
                            if(Port->fnError != NULL)
                                Port->fnError((void*)Port, CN_INS_CHKSUM_ERR, sg_u8SlaveAddress);
                            Port->ErrorLast = CN_INS_CHKSUM_ERR;
                            Port->ErrorPkgs++;
                            startoffset++;
                            Gethead = false;
                            continue;       //whileѭ����,�ӵ�ǰλ������Ѱ�� 0xEB
                        }
//                  }
//                  else
//                  {
//                      Gethead = false;
//                      startoffset++;
//                      continue;       //whileѭ����,�ӵ�ǰλ������Ѱ�� 0xEB
//                  }
//                  break;
                }
                else        //һ����ʱ���ڹ�ȥ�ˣ�û�յ�������Э��ͷ���϶���ʱ�ˡ�
                {
                    if(((u32)DjyGetSysTime() - starttime) > Port->Timeout)
                    {
                        if(Port->fnError != NULL)
                            Port->fnError((void*)Port, CN_INS_TIMEROUT_ERR,sg_u8SlaveAddress);
                        Port->ErrorLast = CN_INS_TIMEROUT_ERR;
                        Port->ErrorPkgs++;
                        Gethead = false;
                        startoffset = 0;
                        readed = 0;
                        printf("\r\n timeover return....");
                    }
                    needread = true;
                    continue;
                }
            }
            else
                needread = true;
        };
//      recdbg(7, NULL, 0);
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
//              recdbg(6, NULL, 0);
                tmp = DevRead(DevRe, &protobuf[readed],restlen, 0, Port->Timeout);
//              if(tmp != 0) recdbg(3, &protobuf[readed],tmp);
                Completed += tmp;
                readed += tmp;
                if(Completed >= restlen)
                    break;
                if(((u32)DjyGetSysTime() - starttime) > Port->Timeout)
                {
                    if(Port->fnError != NULL)
                        Port->fnError((void*)Port, CN_INS_TIMEROUT_ERR,sg_u8SlaveAddress);
                    Port->ErrorLast = CN_INS_TIMEROUT_ERR;
                    Port->ErrorPkgs++;
                    Gethead = false;
                    startoffset = 0;
                    readed = 0;
                    printf("\r\n protocol timeover return....");
                    break;
                }
            }
        }

        if(Completed >= restlen)    //����Ƿ�����������
        {
//          recdbg(9, &protohead, 7);
            Me = __Slave_GetProtocol(Port, protohead.Protocol);
            if(Me != NULL)
            {
//              recdbg(10, NULL, 0);
                if(protohead.SrcAddress == 0)   //�������������ݰ�
                {
                    if(protohead.DstAddress < CN_INS_MULTICAST)
                        Port->EchoModel = ONE_BY_ONE;
                    else
                        Port->EchoModel = BROADCAST_MODEL;
                }
                if(protohead.DstAddress == mydst)  //�յ��㲥��
                {
//                  recdbg(11, NULL,0);
                    if((newpkg) && (Me->MyProcess != NULL))
                    {
//                      recdbg(8, NULL, 0);
                        Me->MyProcess(Me, protohead.SrcAddress,
                                  protobuf + startoffset + sizeof(struct ISBUS_Protocol), len);
                    }
                    else
                    {
                        __ISBUS_Ack(Port);        //Ack�������յ���������
                        printf("\r\nignore repead pkg");
                    }
                }
                else if(protohead.DstAddress >= CN_INS_MULTICAST)   //�յ��㲥���鲥��
                {
//                  recdbg(12, NULL,0);
                    memcpy(Port->MTCcast, protobuf + startoffset + sizeof(struct ISBUS_Protocol),len);
                    if((newpkg) && (Me->MyProcess != NULL))
                    {
                        Me->MyProcess(Me, protohead.SrcAddress,Port->MTCcast, len);
                    }
                    else
                    {
                        __ISBUS_Ack(Port);        //Ack�������յ���������
                        printf("\r\nignore repead pkg");
                    }
                    if(Port->BoardcastPre == 0)     //������ַ�ǵ�һ���ӻ���
                    {
                        __ISBUS_PushMtcPkg(Port);   //�˺�����ȴ��ź��������û�׼�������ݰ�
                    }
                }
                else if((protohead.SrcAddress == Port->BoardcastPre)
                            && (Port->EchoModel == BROADCAST_MODEL))
                {
//                  recdbg(13, NULL,0);
                    //�㲥״̬�£��յ�ǰ���ַ���İ�
                    __ISBUS_PushMtcPkg(Port);   //�˺�����ȴ��ź��������û�׼�������ݰ�
                }
            }
            else
            {
//              recdbg(14, NULL,0);
                __ISBUS_Ack(Port);        //Ack�������յ���������
            }
        }
//      recdbg(15, NULL,0);
        startoffset += sizeof(struct ISBUS_Protocol) + len;
        if(startoffset == readed)
        {
            Port->analyzeoff = 0;
            Port->recvoff = 0;
            needread = true;
        }
        else
        {
            Port->analyzeoff = startoffset;
            Port->recvoff = readed;
        }
//      Port->src = protohead.SrcAddress;
//      Port->dst = protohead.DstAddress;
    }
}

//bool_t showbuf(void)
//{
//    u32 loop,n;
//    for(loop = 0; loop < offset485; loop++)
//    {
//        if(dbgrecord[loop].ev == 1)
//            printf("\r\n�״Σ�");
//        else if(dbgrecord[loop].ev == 2)
//            printf("\r\nʼ����");
//        else if(dbgrecord[loop].ev == 3)
//            printf("\r\n���䣺");
//        else if(dbgrecord[loop].ev == 4)
//            printf("\r\n�㲥��");
//        else if(dbgrecord[loop].ev == 5)
//            printf("\r\n�㲥��");
//        else if(dbgrecord[loop].ev == 6)
//            printf("\r\n�ٶ���");
//        else if(dbgrecord[loop].ev == 7)
//            printf("\r\n�հ���");
//        else if(dbgrecord[loop].ev == 8)
//            printf("\r\n����");
//        else if(dbgrecord[loop].ev == 9)
//            printf("\r\n��9 ��");
//        else if(dbgrecord[loop].ev == 10)
//            printf("\r\n��10��");
//        else if(dbgrecord[loop].ev == 11)
//            printf("\r\n��11��");
//        else if(dbgrecord[loop].ev == 12)
//            printf("\r\n��12��");
//        else if(dbgrecord[loop].ev == 13)
//            printf("\r\n��13��");
//        else if(dbgrecord[loop].ev == 14)
//            printf("\r\n��14��");
//        else if(dbgrecord[loop].ev == 15)
//            printf("\r\n��15��");
//        else
//            printf("\r\n");
//        printf("time = %d num = %d  ", dbgrecord[loop].time,dbgrecord[loop].len);
//        for(n = 0; n < dbgrecord[loop].len;n++)
//            printf(" %x",dbgrecord[loop].data[n]);
//    }
//}
//ADD_TO_ROUTINE_SHELL(showbuf, showbuf, "");

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

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
    u8 nearest = 255;   //С�ڱ�����ַ����ӽ�������ַ��
    u8 least = 255;     //�б�����С��ַ
    u8 loop,now;
    bool_t found = false;
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
        if(now == sg_u8SlaveAddress)
            found = true;
    }
    if(found == true)
    {
        if(sg_u8SlaveAddress == least)  //�������ַ�����С������ǰ�õ�ַ��������ַ
            sg_ptSlavePortHead->BoardcastPre = 0;
        else
            sg_ptSlavePortHead->BoardcastPre = nearest;
    }
    else
        sg_ptSlavePortHead->BoardcastPre = 0xff;    //���������б��У�����Ӧ�㲥��
    ISBUS_SlaveSendPkg(ProtocolSocket, 0, NULL, 0);
}

//-----------------------------------------------------------------------------
//���ܣ�Э�鴦���������ڻ�Ӧ������ѯ״̬��ͨ������ɨ��ӻ��Ƿ����
//������ProtocolSocket��Э���ָ��
//      buf�����յ������ݰ�
//      len�����ݰ�����
//���أ�������
//-----------------------------------------------------------------------------
void __CHK_SlaveSend(struct ISBUS_FunctionSocket *ProtocolSocket,u8 src,u8 *buf,u32 len)
{
    struct Slave_ISBUSPort *Port;
    Port = ProtocolSocket->CommPort;
    Port->HostSerial = 255;         //�յ�����ɨ��ӻ������������3������
    Port->BoardcastPre = 255;
    Port->MTCPre = 255;
    ISBUS_SlaveSendPkg(ProtocolSocket, 0, NULL, 0);
}
#pragma GCC diagnostic pop

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
    memset(Port, 0, sizeof(struct Slave_ISBUSPort ));
    recvbuf = malloc(512+2*sizeof(struct ISBUS_Protocol));
    Port->MTC_Semp = Lock_SempCreate(1, 0, CN_BLOCK_FIFO, "Slave Mtc");
    if((Port != NULL) && (recvbuf != NULL) && (Port->MTC_Semp != NULL)) //����ɹ�
    {
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
        Port->ErrorLast = CN_INS_OK;
        Port->Timeout = Timeout;
        Port->HostSerial = 255;
        Port->BoardcastPre = 255;
        Port->MTCPre = 255;
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
        Lock_SempDelete(Port->MTC_Semp);
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

u32 baktime;
//-----------------------------------------------------------------------------
//���ܣ��յ���ַ��ȷ����Э����ȴ�����ڵ����ݰ������߲�۵Ĵ������ǿյĵ���������ñ�����
//      ����Ӧ������������ȡ�
//������Port���������Ķ˿ڡ�
//���أ���
//------------------------------------------------------------------------------
void __ISBUS_Ack(struct Slave_ISBUSPort *Port)
{
    u8 *SendBuf;
    SendBuf = Port->SendPkgBuf;
    SendBuf[CN_OFF_START]   = 0xEB;
    SendBuf[CN_OFF_DST]     = 0;
    SendBuf[CN_OFF_PROTO]   = CN_SLAVE_ACK;
    SendBuf[CN_OFF_SRC]     = sg_u8SlaveAddress;
    SendBuf[CN_OFF_LEN]     = 0;
    SendBuf[CN_OFF_SERIAL]  = 0;        //�ӻ����͵İ��������0
    SendBuf[CN_OFF_CHKSUM]  = 0xEB + CN_SLAVE_ACK + sg_u8SlaveAddress;

    if(Port->EchoModel == ONE_BY_ONE)
        DevWrite(Port->SerialDevice, SendBuf, sizeof(struct ISBUS_Protocol),
                                    0, CN_TIMEOUT_FOREVER);
    else
    {
        baktime = (u32)DjyGetSysTime();
        Lock_SempPost(Port->MTC_Semp);
    }
}

//-----------------------------------------------------------------------------
//���ܣ����鲥��㲥ģʽ�´����͵����ݰ����ͳ�ȥ�����鲥��㲥ģʽ�£�Ӧ�ó�������ɽ���
//      �������ݺ󣬲����������ذ������Ǳ����յ�������ǰ��Ĵӻ������İ�����ܷ�����
//������Port���������Ķ˿ڡ�
//���أ���
//------------------------------------------------------------------------------
void __ISBUS_PushMtcPkg(struct Slave_ISBUSPort *Port)
{
    if(Lock_SempPend(Port->MTC_Semp, Port->Timeout))
    {
        if(DjyGetSysTime() - baktime > 2000000)
            baktime = 0;
//      recdbg(4, Port->SendPkgBuf,Port->SendPkgBuf[CN_OFF_LEN] + sizeof(struct ISBUS_Protocol));
        DevWrite(Port->SerialDevice, Port->SendPkgBuf,
                Port->SendPkgBuf[CN_OFF_LEN] + sizeof(struct ISBUS_Protocol),
                0, Port->Timeout);
        return ;
    }
}
// ============================================================================
// ���������÷������ݰ����ɴӻ����û����ã�����������Э��ͷȻ���ͳ�ȥ�������ǰ�����鲥
//      ��㲥��ģʽ���򲢲��������ͳ�ȥ�������ͷ�һ���ź������ȵ�����ǰ��Ĵӻ�������Ϻ�
//      ��Э����պ���pend����ź������õ�֮���͡�
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
    extern u32 dbg_stopack;
    if(dbg_stopack != 0)        //���ڵ���ģ��ͨ�Ŵ���
    {
        dbg_stopack--;
        return false;
    }
    if(ISBUS_FunctionSocket == NULL)
        return 0;
    if(len > ISBUS_FunctionSocket->ProtocolSendLen)
        return 0;
    Port = ISBUS_FunctionSocket->CommPort;
//  Port->SendTimes = times;
    SendBuf = Port->SendPkgBuf;
    SendBuf[CN_OFF_START]   = 0xEB;
    SendBuf[CN_OFF_DST]     = 0;
    SendBuf[CN_OFF_PROTO]   = ISBUS_FunctionSocket->Protocol;
    SendBuf[CN_OFF_SRC]     = sg_u8SlaveAddress;
    SendBuf[CN_OFF_LEN]     = len;
    SendBuf[CN_OFF_SERIAL]  = 0;        //�ӻ����͵İ��������0
    SendBuf[CN_OFF_CHKSUM]  = 0xEB + ISBUS_FunctionSocket->Protocol
                              + sg_u8SlaveAddress + len;
    SendLen = len + sizeof(struct ISBUS_Protocol);
    memcpy(SendBuf + sizeof(struct ISBUS_Protocol), buf, len);

    if(Port->EchoModel == ONE_BY_ONE)
    {
//      recdbg(5, SendBuf,SendLen);
        DevWrite(Port->SerialDevice, SendBuf, SendLen, 0, Port->Timeout);
        if((debug_ctrl ==true))
        {
            printf("\r\nslave send:");
            for(tmp = 0;tmp < SendLen;tmp++)
            {
                printf("%02x ",SendBuf[tmp]);
            }
        }
    }
    else
    {
        Lock_SempPost(Port->MTC_Semp);
        baktime = (u32)DjyGetSysTime();
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

