//================================================================================
//������ƣ�Host_ISBUS
//�����������ҵ�������ߣ�Industrial Serial Bus��ͨ��ģ�������˲���
//�����汾��V1.00
//������Ա:YT
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
#include "ISBUS_host.h"
#include "timer_hard.h"
#include "dbug.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���

//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"isbus_host"   //Ƕ��ʽ����ͨ��������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:�������             //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                   //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"devfile","lock","multiplex",         //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure

#define CN_OFF_START        0
#define CN_OFF_DST          1
#define CN_OFF_PROTO        2
#define CN_OFF_SRC          3
#define CN_OFF_LEN          4
#define CN_OFF_CHKSUM       5
#define CN_OFF_USER         6

//Э���ͷ��6�ֽ�
struct Host_ISBUSProtocol
{
    u8 start;           //0xeb
    u8 DstAddress;      //Ŀ���ַ
    u8 Protocol;        //���ܺ�
    u8 SrcAddress;      //Դ��ַ
    u8 Len;             //���ݰ����ȣ�2byte
    u8 ChkSum;          //У��ͣ���ǰ5����Ա�ĺ͡�
};

//ͨ�Ŷ˿ڵ�����
struct Host_ISBUSPort
{
    struct Host_ISBUSPort *Next;            //��ɵ�������ĩ��ָ��NULL
    struct Host_FunctionSocket *SocketHead; //��ɵ���ѭ������Headָ��ǰ���յĹ��ܺ�ֵ
    struct DjyDevice *SerialDevice;         //��Ӧ���豸ָ��
    Host_FntProtocolError fnError;          //�����Ӧ�Ļص�����
    u32 ErrorPkgs;                          //�ۼƴ�����
    u32 ErrorLast;                          //���һ�δ����
    u32 Timeout;                            //��ʱʱ��
    u32 Resttime;                           //�봥����ʱʣ��ʱ��
    u8  fsm;                                //��ǰЭ�����״̬
    u32 relen;                              //���ƫ����
    u8 *SendPkgBuf;                         //�������ݰ�ָ�루����Э��ͷ��
    u8 *RecvPkgBuf;                         //�������ݰ�ָ�루����Э��ͷ��
    u16 PortMaxRecvLen;                     //���˿������հ����ȣ�����Э��ͷ
    u16 PortMaxSendLen;                     //���˿�����Ͱ����ȣ�����Э��ͷ
    u16 PortSendLen;                        //���˿����ڷ��͵İ����ȣ�����Э��ͷ
    u16 RecvP;                              //����ƫ����������Э��ͷ
    u32 SendP;                              //����ƫ��������������Э��ͷ
    u8 MTC_Address;                         //���˿ڽ��ܵ��鲥��ַ�����������鲥
                                            //����ΪCN_MTC_RESV��Ĭ�ϣ�
};

//ͨ��Э�飨���ܺţ�������
struct Host_FunctionSocket
{
    struct Host_FunctionSocket *Next;       //���һ������ĩ��ָ��NULL
    u8 Protocol;                            //Э���
    u16 ProtocolRecvLen;                    //��Э�������հ�����
    u16 ProtocolSendLen;                    //��Э������Ͱ�����
    struct Host_ISBUSPort *CommPort;        //��Ӧ��ͨ�ſ�ָ��
    Host_FntProtocolProcess MyProcess;      //Э�鴦����ָ��
};

ptu32_t uartTimerHandle; //��ʱ�����

struct MultiplexSetsCB * Host_sg_ptMultiplexPort; //������·����
u8 Host_sg_u8Address = 0;                         //���������ַ
struct Host_ISBUSPort *Host_sg_ptPortHead = NULL; //�����������ʼ��

struct Slave* SlaveList=NULL;               //�ӻ�����
struct SemaphoreLCB *UartTimerSemp  = NULL; //�ź���

u8 COUNT_Slave;
u8 Effective_Slave=0;
u8 g_TimeOut=0;  //��ʱ��־λ

void __Host_GetProtocol(struct Host_ISBUSPort *Port,u8 Protocol)
{
    struct Host_FunctionSocket *Next,*SocketHead;
    SocketHead = Port->SocketHead;
    Next = SocketHead;
    do
    {
        if(Next->Protocol == Protocol)
            break;
        Next = Next->Next;
    } while(Next != SocketHead);
    Port->SocketHead = Next;
    return ;
}

void __Uart_HardTimer(void)
{
    Lock_SempPost(UartTimerSemp);
}

void __ISBUS_Process(u32 timeout)
{
    struct Host_FunctionSocket *Me;
    struct Host_ISBUSPort *Port;
    struct DjyDevice *Device;
    ptu32_t DevRe;
    u32 sta,Completed,Completed1;
    u8 FSM,temp,loop;

    while(1)
    {
        u32 Len;
        //�ȴ���·���ô��������ͨ�ſ�ֻҪ��һ��������������
        //ע�����ڶ�·���ñ�����ΪET�����ش��������ԣ�ÿ�δ���������ȫ����ȡ���ݡ�
        DevRe = Multiplex_Wait(Host_sg_ptMultiplexPort, &sta, timeout); //���������,����һ����������ObjectID,����-1
        if(DevRe != -1)
            Device = (struct DjyDevice *)DevRe;
        else
        {
//            continue; //����-1������ȴ�
            //��ʱ�ˣ��ó�ʱ��־λ
            g_TimeOut=1;      //�˱�־λ����һ�ʶ��ģʽ��ĳһ�ӻ���ʱδ������
            break;
        }
        //�����豸��UserTag��Ϊ�˿�ָ��
//        Port = (struct Host_ISBUSPort *)Driver_GetUserTag(Device); //��ȡ�û���ǩ
        Port = (struct Host_ISBUSPort *)Dev_GetDevTag(Device); //��ȡ�û���ǩ
        if(sta & CN_MULTIPLEX_SENSINGBIT_READ)  //�������״̬�ɶ�
        {
            u8 offset;
            if(Port->relen!=0)  //ָ��ƫ������Ϊ0,Ϊ1��������
            {
                Port->fsm=0;
            }
            while(1)
            {
                //һ���Զ�����,�������Э��ͷ
//                Completed1 = Driver_ReadDevice(Device, &Port->RecvPkgBuf[Port->relen],Port->PortMaxRecvLen, 0, 0);
                Completed1 = DevRead(Device, &Port->RecvPkgBuf[Port->relen],Port->PortMaxRecvLen, 0, 0);
                if(Completed1==0) //����0������
                    break;
                FSM = Port->fsm;  //��ǰ���յ������״̬��FSM
                offset = FSM;
                Completed1=Completed1+Port->relen;
                Port->relen=0;
                Completed=sizeof(struct Host_ISBUSProtocol); //��ͷ����

                if(FSM < CN_OFF_USER)  //����Э��ͷ(7���ֽ�),״̬��С���û�����
                {
                    head: offset=0,FSM=0;
                    while(offset < Completed)
                    {
                        switch(FSM) //һ��������
                        {
                            case CN_OFF_START:  //FSM=CN_OFF_START�����ֽ�״̬
                                //���Ѷ����Ļ������У�����0xEB
                                while(offset < Completed + FSM)
                                {
                                    if(Port->RecvPkgBuf[offset++] == 0xEB)
                                    {
                                        FSM = CN_OFF_DST; //�ҵ�0xEB����ָ����һ��
                                        if(FSM==Port->relen)
                                        {
                                            goto tail;
                                        }
                                        break;
                                    }
                                }
                                break ;
                            case CN_OFF_DST: //�Ƿ񱾰��ַ�򱾰��鲥��ַ
                                if((Port->RecvPkgBuf[offset] == Host_sg_u8Address)
                                   || (Port->RecvPkgBuf[offset] == Port->MTC_Address)
                                   || (Port->RecvPkgBuf[offset] == CN_INS_BROADCAST ))
                                {
                                    FSM = CN_OFF_PROTO; //������ȷ���,�����½��
                                    if(FSM==Port->relen)
                                    {
                                        goto tail;
                                    }
                                }
                                else
                                {
                                    FSM = CN_OFF_PROTO;
                                    if(FSM==Port->relen)
                                    {
                                        goto tail;
                                    }
                                }
                                offset++;
                                break ;
                            case CN_OFF_PROTO: //���չ��ܺ�ʱ��֪���ĸ����
                                __Host_GetProtocol(Port,Port->RecvPkgBuf[offset]);
                                offset++;
                                FSM = CN_OFF_SRC;
                                if(FSM==Port->relen)
                                {
                                    goto tail;
                                }
                                break ;
                            case CN_OFF_SRC: //Դ��ַ
                                offset++;
                                FSM = CN_OFF_LEN;
                                if(FSM==Port->relen)
                                {
                                    goto tail;
                                }
                                break ;
                            case CN_OFF_LEN: //���ݰ�����
                                if((u16)Port->RecvPkgBuf[offset++] > Port->SocketHead->ProtocolRecvLen)
                                {
                                    if(Port->fnError != NULL)
                                        Port->fnError(Port, CN_INS_LEN_ERR);
                                    Port->ErrorPkgs++;  //�������
                                    Port->ErrorLast = CN_INS_LEN_ERR;
                                    FSM = CN_OFF_START;
                                    if(FSM==Port->relen)
                                    {
                                        goto tail;
                                    }
                                }
                                FSM = CN_OFF_CHKSUM;
                                if(FSM==Port->relen)
                                {
                                    goto tail;
                                }
                                break ;
                            case CN_OFF_CHKSUM: //У���
                                temp = 0;
                                for(loop = 0; loop < offset; loop++)
                                    temp += Port->RecvPkgBuf[loop];
                                if(temp != Port->RecvPkgBuf[offset++])  //У��Ͳ�����ǰ5����Ա�ĺ�,�򷵻���CN_OFF_START
                                {
                                    if(Port->fnError != NULL)
                                        Port->fnError(Port, CN_INS_CHKSUM_ERR); //У��ͼ��鲻��ȷ,���ô���ص���������
                                    Port->ErrorLast = CN_INS_CHKSUM_ERR;
                                    Port->ErrorPkgs++;
                                    FSM = CN_OFF_START;
                                    if(FSM==Port->relen)
                                    {
                                        goto tail;
                                    }
                                }
                                else  //У��͵���ǰ5����Ա�ĺ�
                                {
                                    COUNT_Slave++;
                                    Effective_Slave=1;

                                    if((Port->RecvPkgBuf[offset-5] != Host_sg_u8Address) //��ַ����
                                     &&(Port->RecvPkgBuf[offset-5] != Port->MTC_Address)
                                     &&(Port->RecvPkgBuf[offset-5] != CN_INS_BROADCAST ))
                                    {
                                        u32 reln=Port->RecvPkgBuf[CN_OFF_LEN];
                                        //����1��,��һ���Ǳ����
                                        if(((reln+sizeof(struct Host_ISBUSProtocol))<Completed1))
                                        {
                                            for(loop=0;loop<(Completed1-(reln+sizeof(struct Host_ISBUSProtocol)));loop++) //��λ
                                            Port->RecvPkgBuf[loop] = Port->RecvPkgBuf[reln+sizeof(struct Host_ISBUSProtocol)+loop];
                                            Port->relen=(Completed1-(reln+sizeof(struct Host_ISBUSProtocol)));
                                            Completed1=Port->relen;
                                            goto head;
                                        }
                                        if((reln+sizeof(struct Host_ISBUSProtocol))==Completed1)  //�жϵڶ����Ƿ�Ϊ������
                                        {
                                            Port->relen=0;
                                        }
                                        Completed1=sizeof(struct Host_ISBUSProtocol); //����Ӧ������
                                        FSM=0;  //״̬������
                                        goto tail;  //����Ӧ������ֱ������
                                    }
                                    else
                                    {
                                        FSM = CN_OFF_USER;  //�û�����,FSM=6
                                        if(FSM==Port->relen)
                                        {
                                            goto tail;
                                        }
                                    }
                                }
                                break ;
                            default:break;
                        }
                    }
                }
                Len = Port->RecvPkgBuf[CN_OFF_LEN]; //Ӧ�����ݰ�����
                if((Len>(Completed1-sizeof(struct Host_ISBUSProtocol)))&&(FSM==CN_OFF_USER))
                {
                    goto tail; //����������ѭ��
                }
                if((Len<=(Completed1-sizeof(struct Host_ISBUSProtocol)))&&(FSM==CN_OFF_USER))
                {
                    FSM = CN_OFF_START; //״̬�����ص���������Э��ͷ��״̬
                    Port->fsm = FSM;
                    Me = Port->SocketHead;  //Host_ISBUSPort��ͨѶЭ�鸳��Me
                    Me->MyProcess(Me, &Port->RecvPkgBuf[CN_OFF_USER],Len);  //Host_FunctionSocket��ӦЭ�鴦����
                    Port->RecvP = 0;
                    Port->relen=0;
                }
                //����1��,��1��Ϊ�����
                if(((Len+sizeof(struct Host_ISBUSProtocol))<Completed1))
                {
                   for(loop = 0; loop < (Completed1-(Len+sizeof(struct Host_ISBUSProtocol))); loop++) //��λ
                       Port->RecvPkgBuf[loop] = Port->RecvPkgBuf[loop+(Len+sizeof(struct Host_ISBUSProtocol))];
                   Port->relen=Completed1-(Len+sizeof(struct Host_ISBUSProtocol));
                   Completed1=Port->relen;
                   goto head;
                }
                tail:break; //����������ѭ��
            }
        }
//        if(sta & CN_MULTIPLEX_SENSINGBIT_WRITE) //�������״̬��д
//        {    //�ж��ҵ�ǰ�ķ���û��,û���꿴����Ҫ������,������
//             if(Port->SendP < Port->PortSendLen + sizeof(struct Host_ISBUSProtocol))
//            {
//                u32 SendLen;
//                SendLen = Port->PortSendLen + sizeof(struct Host_ISBUSProtocol) - Port->SendP;
//                Completed = Driver_WriteDevice(Device, Port->SendPkgBuf + Port->SendP,
//                                                SendLen, 0, CN_BLOCK_BUFFER, 0);
//                if(Completed != -1)
//                    Port->SendP += Completed;
//            }
//        }
        if(sta & CN_MULTIPLEX_SENSINGBIT_ERROR) //�������״̬����,�͵��ó���Ļص�����,��¼�����
        {
            if(Port->fnError != NULL)
                Port->fnError(Port, CN_INS_PHY_ERR);
            Port->ErrorLast = CN_INS_PHY_ERR;
            Port->ErrorPkgs++;
        }
        break;
    }

}
// ============================================================================
// �������ܣ�������Э�鴦����
// ���������  ReplyModel��Ӧ��ģʽ��ȡֵ����
//                   ONE_BY_ONE�����ӻ�һ��һ��ģʽ��
//                   BOOADCAST_MODEL�����ӻ�һ�ʶ��ģʽ�����ͨѶЧ�ʡ��㲥ģʽ
//                   MULTICAST_MODEL�����ӻ�һ�ʶ��ģʽ�����ͨѶЧ�ʡ��鲥ģʽ
//        PoolModel����ʱ��ѯģʽ��ȡֵ����
//                   NO_TIMER_POLL����ʱ���ڲ���Ҫ�󣬸����ӻ��ж��Ͷ��ظ���
//                   EVENLY_SPACED_POLL���ȼ��������ѯ�����ӻ���
//                   ACCUMU_TIMER_POLL���ܼ��������ѯ���дӻ���
//        PoolTimes����ѯ������ȡUNLIMITED_NUMBERʱΪ�ܶ���ʼ����ѯ
//        Protocol�����ܺ�
//        GroupModel������ѯ�Ĵӻ�����ȫ���򲿷��飬�����û�����
//                   GroupModelȡALL_GROUP_POLLʱȫ����ѯ��������ֵΪ��������
//        SlaveNum���ӻ�������
//        Send_Buff�������͵����ݰ�������Э��ͷ
//        len���������ݰ�����
// ����ֵ��  ��
// ============================================================================
ptu32_t Host_ISBUS_Process(u32 ReplyModel,u32 PoolModel,u32 PoolTimes,u8 Protocol,u8 GroupModel,u32 SlaveNum,u8 *Send_Buff,u16 len)
{
    u8 pos=1;
    u8 cnt=0;
    u8 cn[15];
    u32 times=0;
    u32 pooltimes=0;
    struct Host_FunctionSocket *Me;
    struct Host_ISBUSPort *Port;

    while(1) //�ȴ���ֱ��ע��ͨ�Ŷ˿�
    {
        if(Host_sg_ptPortHead != NULL)
            break ;
        Djy_EventDelay(100*mS);
    }
    Port=Host_sg_ptPortHead;
    Me=Port->SocketHead;

    if(ReplyModel==ONE_BY_ONE) //һ��һ�𣬵�Ե�ģʽ
    {
        while(SlaveList->Rank!=pos) //�����ҵ��·�˳��Ϊ1�Ĵӻ�
        {
            SlaveList=SlaveList->Next;
        }

        switch(PoolModel)
        {
            //1����ʱ���ڲ���Ҫ������������ѯ�������ӻ��ж��Ͷ��ظ�
            case NO_TIMER_POLL:
                while(1)
                {
                    Host_ISBUS_SendPkg(Me,Protocol,SlaveList->Address,Host_sg_u8Address,Send_Buff,len);
                    pooltimes++;
                    __ISBUS_Process(SlaveList->Timeout);
                    SlaveList=SlaveList->Next;
                    if(pooltimes==PoolTimes)
                    {
                        break; //��ѯ������������
                    }
                }
                break;
            //2���ȼ��������ѯ�����ӻ�
            case EVENLY_SPACED_POLL:
                while(1)
                {
                    if(Lock_SempPend(UartTimerSemp,CN_TIMEOUT_FOREVER))
                    {
                        Host_ISBUS_SendPkg(Me,Protocol,SlaveList->Address,Host_sg_u8Address,Send_Buff,len);
                        pooltimes++;
                        __ISBUS_Process(SlaveList->Timeout);
                        SlaveList=SlaveList->Next;
                        if(pooltimes==PoolTimes)
                        {
                            break; //��ѯ������������
                        }
                    }
                }
                break;
            //3���ܼ��������ѯ���дӻ�
            case ACCUMU_TIMER_POLL:
                while(1)
                {
                    if(Lock_SempPend(UartTimerSemp,CN_TIMEOUT_FOREVER))
                    {
                        while(1)
                        {
                            Host_ISBUS_SendPkg(Me,Protocol,SlaveList->Address,Host_sg_u8Address,Send_Buff,len);
                            pooltimes++;
                            __ISBUS_Process(SlaveList->Timeout);
                            SlaveList=SlaveList->Next;
                            times++;
                            if(times==SlaveNum)
                            {
                                times=0;
                                break;
                            }
                            if(pooltimes==PoolTimes)
                            {
                                break; //��ѯ������������
                            }
                        }
                        if(pooltimes==PoolTimes)
                        {
                            break; //��ѯ������������
                        }
                    }
                }
                break;
            default:break;
        }
    }
    else //�㲥���鲥ģʽ
    {
        //4�����ӻ�һ�ʶ��㲥ģʽ
        if((PoolModel==NO_TIMER_POLL)&&(ReplyModel==BOOADCAST_MODEL))
        {
           Host_ISBUS_SendPkg(Me,Protocol,CN_INS_BROADCAST,Host_sg_u8Address,Send_Buff,len); //�㲥��ַ���ڶ����ֽ�Ϊ0XFF
           while(1)
           {
               __ISBUS_Process(SlaveList->Timeout);
               if(g_TimeOut)  //���ֳ�ʱ��ֱ���·���һ�ӻ�
               {
                   Host_ISBUS_SendPkg(Me,Protocol,SlaveList->Next->Address,
                                                       Host_sg_u8Address,Send_Buff,len); //��ַ����һ�ӻ���ַ
               }
               SlaveList=SlaveList->Next;
           }
        }

        //ɾ���ӻ����з��ض���ŵĴӻ���
        for(int i=0;i<Slave_GetNum(SlaveList);i++)
        {
            if(SlaveList->Group!=GroupModel)
            {
                cn[cnt]=SlaveList->Group;
                cnt++;
            }
            SlaveList=SlaveList->Next;
        }
        for(int i=0;i<cnt;i++)
        {
            SlaveList=Slave_Delete(SlaveList,cn[i]);
        }

        //5�����ӻ�һ�ʶ���鲥ģʽ
        if((PoolModel==NO_TIMER_POLL)&&(ReplyModel==MULTICAST_MODEL))
        {
           Host_ISBUS_SendPkg(Me,Protocol,(CN_INS_MULTICAST+GroupModel),Host_sg_u8Address,Send_Buff,len);
           while(1)
           {
               __ISBUS_Process(SlaveList->Timeout);
               SlaveList=SlaveList->Next;
           }
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
struct Host_ISBUSPort *Host_ISBUS_RegistPort(struct DjyDevice *dev,
                                             Host_FntProtocolError fnError,u32 Timeout)
{
    struct Host_ISBUSPort *Port;
    if(dev == NULL)
        return NULL;
    Port = (struct Host_ISBUSPort *)malloc(sizeof(struct Host_ISBUSPort ));
    if(Port != NULL) //����ɹ�
    {
        if(Host_sg_ptPortHead == NULL)
        {
            Port->Next = NULL;
        }
        else
        {
            Port->Next = Host_sg_ptPortHead;
        }
        Host_sg_ptPortHead = Port;
        Port->SocketHead = NULL; //��Host_FunctionSocket������г�ʼ��
        Port->SerialDevice = dev;
        Port->fnError = fnError;
        Port->ErrorPkgs = 0;
        Port->ErrorLast = CN_INS_OK;
        Port->Timeout = Timeout;
        Port->Resttime = 0;
        Port->fsm = CN_OFF_START;
        Port->relen = 0;            //����ƫ����
        Port->SendPkgBuf = NULL;
        Port->RecvPkgBuf = NULL;
        Port->PortMaxRecvLen = 0;
        Port->PortMaxSendLen = 0;
        Port->PortSendLen = 0;
        Port->RecvP = 0;
        //����0����ʼ״̬�ᷢ��һ�����ݡ�
        Port->SendP = sizeof(struct Host_ISBUSProtocol);  //����ƫ����ΪЭ��ͷ
        Port->MTC_Address = CN_MTC_RESV;
        Driver_SetUserTag(dev, (ptu32_t)Port);
        Driver_MultiplexAdd(Host_sg_ptMultiplexPort, &dev, 1,
                                           CN_MULTIPLEX_SENSINGBIT_READ
//                                            |  CN_MULTIPLEX_SENSINGBIT_WRITE  //���������ˣ���д������
                                        |  CN_MULTIPLEX_SENSINGBIT_ERROR
                                        |  CN_MULTIPLEX_SENSINGBIT_ET
                                        |  CN_MULTIPLEX_SENSINGBIT_OR    );
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
struct Host_FunctionSocket *Host_ISBUS_RegistProtocol(struct Host_ISBUSPort *Port, u8 Protocol,
                                          u16 MaxRecvLen,u16 MaxSendLen, Host_FntProtocolProcess fn)
{
    struct Host_FunctionSocket *ProtocolSocket;
    bool_t result = true;
    if(Port == NULL)
        return NULL;
    ProtocolSocket = (struct Host_FunctionSocket *)malloc(sizeof(struct Host_ISBUSPort ));
    if(ProtocolSocket != NULL)
    {
        //result���ڳ����жϣ�������ָ���Ƿ�NULL����Ϊ�е���������ͨ�ſڣ�Len=0
        if(MaxRecvLen > Port->PortMaxRecvLen)
        {
            Port->RecvPkgBuf = (u8*)realloc(Port->RecvPkgBuf,  //�������ݰ�ָ�루����Э��ͷ��
                                        (MaxRecvLen+ sizeof(struct Host_ISBUSProtocol)));

            Port->PortMaxRecvLen = (MaxRecvLen+ sizeof(struct Host_ISBUSProtocol));
            result = (Port->RecvPkgBuf != NULL);
        }
        if(MaxSendLen > Port->PortMaxSendLen) //���Ͳ���
        {
            Port->SendPkgBuf = (u8*)realloc(Port->SendPkgBuf,
                                       MaxSendLen + sizeof(struct Host_ISBUSProtocol));
            Port->PortMaxSendLen = MaxSendLen;
            result |= (Port->RecvPkgBuf != NULL);
        }
        if(Port->SocketHead == NULL)
        {
            ProtocolSocket->Next = ProtocolSocket;
            Port->SocketHead = ProtocolSocket;
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
    if((ProtocolSocket == NULL) || (result == false) )
    {
        debug_printf("ISBUS","Host_ISBUS_RegistProtocol failure");
        free(ProtocolSocket);
        free(Port->RecvPkgBuf);
        free(Port->SendPkgBuf);
        return NULL;
    }
    else
        return ProtocolSocket;
}
// ============================================================================
// �������ܣ����ݰ����ͺ��������û����ã�����������Э��ͷȻ���ͳ�ȥ�������ӻ����á�
// ���������Host_FunctionSocket��ͨ��Э����ָ�룬ΪHost_ISBUS_RegistProtocol�����ķ���ֵ
//        Protocol�����ܺ�
//        dst��Ŀ�ĵ�ַ�����������ˣ�Ŀ�ĵ�ַΪ�ӻ��ӻ���ַ��ԭ��ַΪHost_Address�����ڴӻ���Ŀ�ĵ�ַΪ������ַ��
//             Դ��ַΪ�����ַSlave_sg_u8Address
//        src��Դ��ַ
//        buf�������͵����ݰ�������Э��ͷ
//        len�����ͳ���
// ����ֵ��  ���͵���������ֻ��copy���˷���buf��
// ============================================================================
u32 Host_ISBUS_SendPkg(struct Host_FunctionSocket  *Host_FunctionSocket,u8 Protocol,u8 dst,u8 src, u8 *buf, u16 len)
{
    struct Host_ISBUSPort *Port;
    u8 *SendBuf;
    u16 SendLen;
    u32 Completed;
    if(Host_FunctionSocket == NULL)
        return 0;
    if(len > Host_FunctionSocket->ProtocolSendLen)
        return 0;
    Port = Host_FunctionSocket->CommPort;
    Port->PortSendLen = len;    //����Э��ͷ
    SendBuf = Port->SendPkgBuf;
    SendBuf[CN_OFF_START]   = 0xEB;
    SendBuf[CN_OFF_DST]     = dst;
    SendBuf[CN_OFF_PROTO]   = Host_FunctionSocket->Protocol;
    SendBuf[CN_OFF_SRC]     = src;
    SendBuf[CN_OFF_LEN]     = len;
    SendBuf[CN_OFF_CHKSUM]  = 0xEB + dst + Host_FunctionSocket->Protocol
                              + src + (len & 0xff) + (len >> 8);
    SendLen = len + sizeof(struct Host_ISBUSProtocol);
    memcpy(SendBuf + sizeof(struct Host_ISBUSProtocol), buf, len);

//    Completed = Driver_WriteDevice(Port->SerialDevice, SendBuf, SendLen,
//                                   0, CN_BLOCK_BUFFER, 0);
    Completed = DevWrite(Port->SerialDevice, SendBuf, SendLen,
                                                             0,0);

    if(Completed != -1)
        Port->SendP = Completed;

    return len;
}

// ============================================================================
// �������ܣ���ʱ��ѯ�������á�
//       ˵����������ѯ����ǰ���˽�ӻ����ԣ������ӻ���ʱʱ�估����ظ�ʱ�䣬����ACCUMU_TIMER_POLL�ܼ����
//           ����ѯģʽ���ܵ�һȦ����ѯ����Ϊ�����ӻ���ʱʱ����������ö���Ķ�ʱʱ�䡣
// ���������timercycle����ʱ���ڣ���λus��NO_TIMER_POLLģʽ��ȡNULL.
//        PoolModel����ʱ��ѯģʽ��EVENLY_SPACED_POLL--�ȼ��������ѯ�����ӻ���
//                            ACCUMU_TIMER_POLL--�ܼ��������ѯ�����ӻ�����ѯһ�ּ��������ѯһ��ʱ��Ϊtimercycle���ϸ��ӻ���ʱ������ʱ�䣻
//                            NO_TIMER_POLL--��ʱ���ڲ���Ҫ���ж�����ѯ��졣
// ����ֵ����
// ============================================================================
void Timer_Poll_Config(u32 timercycle,u8 PoolModel)
{
    static u32 CrystalFreq;
    u32 timer;
    timer=timercycle;

    if(PoolModel==NO_TIMER_POLL) //��ʱʱ�䲻��Ҫ��
       return;
    if(PoolModel==EVENLY_SPACED_POLL||PoolModel==ACCUMU_TIMER_POLL)
    {
        UartTimerSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"UART_TIMER_SEMP");
        uartTimerHandle=HardTimer_Alloc(__Uart_HardTimer);   //��ʱ������
        if(uartTimerHandle==(-1))
            debug_printf("ISBUS","�����˶�ʱ��ѯģʽ�����쳣\n\r");
        CrystalFreq = HardTimer_GetFreq(uartTimerHandle);  //��ȡ��ʱ��Ƶ�ʣ�����F3��˵��72MHZ
        HardTimer_Ctrl(uartTimerHandle,EN_TIMER_SETCYCLE,(CrystalFreq/1000000)*timer); //���ö�ʱ����,��λus
        HardTimer_Ctrl(uartTimerHandle,EN_TIMER_SETRELOAD,true);    // reloadģʽ
        HardTimer_Ctrl(uartTimerHandle,EN_TIMER_ENINT,true);        // �ж�ʹ��
        HardTimer_Ctrl(uartTimerHandle,EN_TIMER_SETINTPRO,false);   // �ж���������,true����ʵʱ�ź�
        HardTimer_Ctrl(uartTimerHandle,EN_TIMER_STARTCOUNT,0);      // ʹ�ܼ���
    }
    return;
}

// ============================================================================
// �������ܣ���ȡ�ӻ���ַ�б�ISBUSģ��ɨ�����п��ܵĵ�ַ���ڲ�֪���ӻ����������ʹ�á�
//    ˵���������ϵ磬�ӻ����ܻ�û�ϵ磬�ṩһ���ӿڣ�Ӧ�ó����ʱ��ɨ��
// ���������
// ����ֵ���ӻ��б�
// ============================================================================
struct Slave* Slave_Addr_Get(struct Host_FunctionSocket  *InSerSocket)
{
    struct Host_FunctionSocket *Me;
    struct Host_ISBUSPort *Port;
    static u8 SlaveAddress=1;
    static u8 Slave_Rank=1;

    while(1) //�ȴ���ֱ��ע��ͨ�Ŷ˿�
    {
        if(Host_sg_ptPortHead != NULL)
            break ;
        Djy_EventDelay(100*mS);
    }
    Port=Host_sg_ptPortHead;
    Me=Port->SocketHead;
    u8 Send_Buff[1]={0x00};
    while(1)
    {
        Host_ISBUS_SendPkg(Me,0,SlaveAddress,Host_sg_u8Address,Send_Buff,sizeof(Send_Buff));
        __ISBUS_Process(10*mS);  //��ʱʱ��Ĭ��Ϊ10*mS

        if(Effective_Slave)
        {
           SlaveList=Slave_Creat(InSerSocket->CommPort,InSerSocket->CommPort->SerialDevice,SlaveList,
                                                       SlaveAddress,ALL_GROUP_POLL,Slave_Rank,10*mS);
           Slave_Rank++;
           Effective_Slave=0;
        }
        SlaveAddress++;
        if(SlaveAddress==255)
           break;
    }
    return SlaveList;
}

// ============================================================================
// �������ܣ���Ӵӻ������ò�����
// ���������Port����Ӧͨ�Ŷ˿�ָ��
//        SerialDeviceͨ�Ŷ˿��豸ָ��
//        pHead���ӻ��ṹ������ָ��
//        address���ӻ���ַ
//        group���ӻ�������ţ��鲥ʱ�õ�����groupȡALL_GROUP_POLL������
//        rank������������ӻ��·����ݰ�˳��
//        timeout����ʱʱ�䣬��֪����ʱʱ�������ΪNULL
//        commcode��ͨѶģʽ����˫���򵥹���ȡֵHALF_DUPLEX_COMM��SIMPLES_COMM
// ����ֵ���ӻ��ṹ������ָ��
// ============================================================================
struct Slave *Slave_Creat(struct Host_ISBUSPort *Port,struct DjyDevice *dev,
                                   struct Slave *pHead,u8 address,u8 group,u8 rank,u32 timeout)
{
    struct Slave *p1;
    p1=(struct Slave*)malloc(sizeof(struct Slave));//�����½ڵ�
    if(p1==NULL)
    {
        debug_printf("ISBUS","�ڴ�����ʧ��\r\n");
    }
    p1->CommPort=Port;
    p1->SerialDevice=dev;
    p1->Address=address;
    p1->Group = group;
    p1->Rank = rank;
    p1->Timeout=timeout;

    if(pHead==NULL) //�ձ������ͷ
    {
        pHead->Next=pHead=p1;
    }
    else
    {
        //ֱ�ӽ�p��������β�����Ϊ�µ�β���
        p1->Next=pHead->Next;
        pHead=pHead->Next=p1;
    }
    if(pHead==NULL)
    {
        debug_printf("ISBUS","��Ӵӻ�ʧ��\r\n");
        while(1);   //��ʼ���׶ξ��쳣��ֱ����ѭ��
    }
    return pHead;
}

// ============================================================================
// �������ܣ����Ѵ����Ĵӻ���ɾ��ָ����Ŵӻ���
// ���������pHead���ӻ��ṹ������ָ��
//        group��ɾ��ĳһ��ӻ�
// ����ֵ�� �ӻ��ṹ������ָ��
// ============================================================================
struct Slave * Slave_Delete (struct Slave * pHead, int group)
{
    struct Slave* p,*delete;
    p = pHead;

    while(1)
    {
        if(p==NULL)
        {
            debug_printf("ISBUS","No Slave!\n"); //û�дӻ�
        }
        do
        {
            if(p->Next->Group==group)
            break;
            else
            p = p->Next;
        } while (p!= pHead);

        if (p==pHead&&pHead->Next->Group!=group)
        {
            debug_printf("ISBUS","Not found!\n");
            break;
        }
        else
        {
            if (pHead->Next==pHead)
            {
                //������ʱ�����н���һ��������Ǵ�ɾ���
                free(pHead);
                pHead=NULL;
            }
            else
            {
                //��ʱ��������������㼰������
                delete=p->Next;
                p->Next=delete->Next;

                if(delete==pHead)
                {
                    //������ɾ�����β��㣬������ҪŲβָ��
                    pHead = p;
                }
                free(delete);
            }
        }
    }
    return pHead;
}

// ============================================================================
// �������ܣ���ȡ�ӻ�������
// ���������pHead���ӻ��ṹ������ָ��
// ����ֵ��    �ӻ�����
// ============================================================================
u32 Slave_GetNum(struct Slave * pHead)
{
    struct Slave* p;
    u32 num=1;
    p=pHead;

    if(p==NULL)
    {
        debug_printf("ISBUS","No Slave!\n"); //û�дӻ�
        return 0;
    }
    p=p->Next;
    while(p!=pHead)
    {
        num++;
        p=p->Next;
    }
    return num;
}

// ============================================================================
// �������ܣ�����������ַ�������ˡ�
// ���������Addr��������ַ��8λ
// ����ֵ�� ��
// ============================================================================
void Host_ISBUS_SetAddress(u8 Addr)
{
    Host_sg_u8Address = Addr;
}

