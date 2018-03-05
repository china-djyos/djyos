//================================================================================
//������ƣ�serialcomm
//���������װ���ڲ�����ͨ�ŵ�������·��
//       ��ģ�������������ӻ���
//�����汾��V1.00
//������Ա: ������
//================================================================================
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <djyos.h>
#include <object.h>
#include <lock.h>
#include <multiplex.h>
#include <driver.h>
#include <systime.h>
#include "serialcomm.h"
#include "timer_hard.h"

#define CN_OFF_START        0
#define CN_OFF_DST          1
#define CN_OFF_PROTO        2
#define CN_OFF_SRC          3
#define CN_OFF_LENL         4
#define CN_OFF_LENH         5
#define CN_OFF_CHKSUM       6
#define CN_OFF_USER         7

//Э���ͷ��7�ֽ�
struct InSerProtocol
{
    u8 start;           //0xeb
    u8 DstAddress;      //Ŀ���ַ
    u8 Protocol;        //Э���
    u8 SrcAddress;      //Դ��ַ
    u8 LenL;            //���ݰ����ȵ��ֽ�
    u8 LenH;            //���ݰ����ȸ��ֽ�
    u8 ChkSum;          //У��ͣ���ǰ6����Ա�ĺ͡�
};

//ͨ�Ŷ˿ڵ�����
struct InSerSocketPort
{
    struct InSerSocketPort *Next;           //��ɵ�������ĩ��ָ��NULL
    struct InSerSocket *SocketHead;         //��ɵ���ѭ������Headָ��ǰ����
                                            //��Э����
    struct DjyDevice *SerialDevice;         //��Ӧ���豸ָ��
    fntProtocolError fnError;               //�����Ӧ�Ļص�����
//    u8 Recving[sizeof(struct InSerProtocol)];//���ڽ���Э��ͷ��
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

//ͨ��Э�������
struct InSerSocket
{
    struct InSerSocket *Next;               //���һ������ĩ��ָ��NULL
    u8 Protocol;                            //Э���
    u16 ProtocolRecvLen;                    //��Э�������հ�����
    u16 ProtocolSendLen;                    //��Э������Ͱ�����
    struct InSerSocketPort *CommPort;       //��Ӧ��ͨ�ſ�ָ��
    fntProtocolProcess MyProcess;           //Э�鴦����ָ��
};

ptu32_t uartTimerHandle;

struct MultiplexSetsCB * Master_sg_ptMultiplexPort;  //������·����
u8 Master_sg_u8Address = 0;                          //���������ַ
struct InSerSocketPort *Master_sg_ptPortHead = NULL; //�����������ʼ��

struct MultiplexSetsCB * Slave_sg_ptMultiplexPort;  //�ӻ���·����
u8 Slave_sg_u8Address = 0;                          //�ӻ������ַ
struct InSerSocketPort *Slave_sg_ptPortHead = NULL; //�ӻ��������ʼ��

struct Slave* SlaveList=NULL;  //�ӻ�����
struct SemaphoreLCB *UartTimerSemp  = NULL; //�ź���

u8 COUNT_Slave;
u8 Effective_Slave=0;

void __INS_GetProtocol(struct InSerSocketPort *Port,u8 Protocol)
{
    struct InSerSocket *Next,*SocketHead;
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

void __INS_Process(u32 timeout)
{
    struct InSerSocket *Me;
    struct InSerSocketPort *Port;
    struct DjyDevice *Device;
    ptu32_t DevRe;
    u32 sta,Completed,Completed1;
    u8 FSM,temp,loop;

    while(1)
    {
        u32 Len;
        //�ȴ���·���ô��������ͨ�ſ�ֻҪ��һ��������������
        //ע�����ڶ�·���ñ�����ΪET�����ش��������ԣ�ÿ�δ���������ȫ����ȡ���ݡ�
        DevRe = Multiplex_Wait(Master_sg_ptMultiplexPort, &sta, timeout); //���������,����һ����������ObjectID,����-1
        if(DevRe != -1)
            Device = (struct DjyDevice *)DevRe;
        else
        {
            continue; //����-1������ȴ�
        }
        //�����豸��UserTag��Ϊ�˿�ָ��
        Port = (struct InSerSocketPort *)Driver_GetUserTag(Device); //��ȡ�û���ǩ
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
                Completed1 = Driver_ReadDevice(Device, &Port->RecvPkgBuf[Port->relen],Port->PortMaxRecvLen, 0, 0);
                if(Completed1==0) //����0������
                    break;
                FSM = Port->fsm;  //��ǰ���յ������״̬��FSM
                offset = FSM;
                Completed1=Completed1+Port->relen;
                Port->relen=0;
                Completed=sizeof(struct InSerProtocol); //��ͷ����

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
                                if((Port->RecvPkgBuf[offset] == Master_sg_u8Address)
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
                            case CN_OFF_PROTO: //����Э���ʱ��֪���ĸ����
                                __INS_GetProtocol(Port,Port->RecvPkgBuf[offset]);
                                offset++;
                                FSM = CN_OFF_SRC;
                                if(FSM==Port->relen)
                                {
                                    goto tail;
                                }
                                break ;
                            case CN_OFF_SRC: //Դ��ַ
                                offset++;
                                FSM = CN_OFF_LENL;
                                if(FSM==Port->relen)
                                {
                                    goto tail;
                                }
                                break ;
                            case CN_OFF_LENL:  //���ȵ��ֽ�
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
                                FSM = CN_OFF_LENH;
                                if(FSM==Port->relen)
                                {
                                    goto tail;
                                }
                                break ;
                            case CN_OFF_LENH: //���ȸ��ֽ�
                                if( (Port->RecvPkgBuf[offset-1] + ((u16)Port->RecvPkgBuf[offset] << 8))
                                    > Port->SocketHead->ProtocolRecvLen)
                                {
                                    offset++;
                                    if(Port->fnError != NULL)
                                        Port->fnError(Port, CN_INS_LEN_ERR);
                                    Port->ErrorLast = CN_INS_LEN_ERR;
                                    Port->ErrorPkgs++; //�������
                                    FSM = CN_OFF_START;
                                    if(FSM==Port->relen)
                                    {
                                        goto tail;
                                    }
                                }
                                offset++;
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
                                if(temp != Port->RecvPkgBuf[offset++])  //У��Ͳ�����ǰ6����Ա�ĺ�,�򷵻���CN_OFF_START
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
                                else  //У��͵���ǰ6����Ա�ĺ�
                                {
                                    COUNT_Slave++;
                                    Effective_Slave=1;

                                    if((Port->RecvPkgBuf[offset-6] != Master_sg_u8Address) //��ַ����
                                     &&(Port->RecvPkgBuf[offset-6] != Port->MTC_Address)
                                     &&(Port->RecvPkgBuf[offset-6] != CN_INS_BROADCAST ))
                                    {
                                        u32 reln=Port->RecvPkgBuf[CN_OFF_LENL]+ (Port->RecvPkgBuf[CN_OFF_LENH] << 8);
                                        //����1��,��һ���Ǳ����
                                        if(((reln+sizeof(struct InSerProtocol))<Completed1))
                                        {
                                            for(loop=0;loop<(Completed1-(reln+sizeof(struct InSerProtocol)));loop++) //��λ
                                            Port->RecvPkgBuf[loop] = Port->RecvPkgBuf[reln+sizeof(struct InSerProtocol)+loop];
                                            Port->relen=(Completed1-(reln+sizeof(struct InSerProtocol)));
                                            Completed1=Port->relen;
                                            goto head;
                                        }
                                        if((reln+sizeof(struct InSerProtocol))==Completed1)  //�жϵڶ����Ƿ�Ϊ������
                                        {
                                            Port->relen=0;
                                        }
                                        Completed1=sizeof(struct InSerProtocol); //����Ӧ������
                                        FSM=0;  //״̬������
                                        goto tail;  //����Ӧ������ֱ������
                                    }
                                    else
                                    {
                                        FSM = CN_OFF_USER;  //�û�����,FSM=7
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
                Len = (Port->RecvPkgBuf[CN_OFF_LENL] + (Port->RecvPkgBuf[CN_OFF_LENH] << 8));
                if((Len>(Completed1-sizeof(struct InSerProtocol)))&&(FSM==CN_OFF_USER))
                {
                    goto tail; //����������ѭ��
                }
                if((Len<=(Completed1-sizeof(struct InSerProtocol)))&&(FSM==CN_OFF_USER))
                {
                    FSM = CN_OFF_START; //״̬�����ص���������Э��ͷ��״̬
                    Port->fsm = FSM;
                    Me = Port->SocketHead;  //InSerSocketPort��ͨѶЭ�鸳��Me
                    Me->MyProcess(Me, &Port->RecvPkgBuf[CN_OFF_USER],Len);  //InSerSocket��ӦЭ�鴦����
                    Port->RecvP = 0;
                    Port->relen=0;
                }
                //����1��,��1��Ϊ�����
                if(((Len+sizeof(struct InSerProtocol))<Completed1))
                {
                   for(loop = 0; loop < (Completed1-(Len+sizeof(struct InSerProtocol))); loop++) //��λ
                       Port->RecvPkgBuf[loop] = Port->RecvPkgBuf[loop+(Len+sizeof(struct InSerProtocol))];
                   Port->relen=Completed1-(Len+sizeof(struct InSerProtocol));
                   Completed1=Port->relen;
                   goto head;
                }
                tail:break; //����������ѭ��
            }
        }
//        if(sta & CN_MULTIPLEX_SENSINGBIT_WRITE) //�������״̬��д
//        {    //�ж��ҵ�ǰ�ķ���û��,û���꿴����Ҫ������,������
//             if(Port->SendP < Port->PortSendLen + sizeof(struct InSerProtocol))
//            {
//                u32 SendLen;
//                SendLen = Port->PortSendLen + sizeof(struct InSerProtocol) - Port->SendP;
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
//                    ONE_BY_ONE�����ӻ�һ��һ��ģʽ��
//                    ONE_BY_MORE�����ӻ�һ�ʶ��ģʽ�����ͨѶЧ�ʡ�
//ConfSlave�����ôӻ���ȡֵ����
//                   SLAVE_INFORM_NUKNOWN���ӻ�����δ֪�������Ȳ�֪���ӻ���ַ���·�˳��Ȳ�����
//                   SLAVE_INFORM_KNOWN���ӻ�������֪�����û����ȸ�֪�����á�
//        Timeout��SLAVE_INFORM_NUKNOWNģʽ�µȴ��ӻ���Ӧ���ĵĳ�ʱʱ�䣬�ò������û����á�SLAVE_NFORM_KNOWNģʽ����Ч,ȡNULL��
//        PoolModel����ʱ��ѯģʽ��ȡֵ����
//                   NO_TIMER_POLL����ʱ���ڲ���Ҫ�󣬸����ӻ��ж��Ͷ��ظ���
//                   EVENLY_SPACED_POLL���ȼ��������ѯ�����ӻ���
//                   ACCUMU_TIMER_POLL���ܼ��������ѯ���дӻ���
//        SlaveNum���ӻ�������SLAVE_INFORM_NUKNOWNģʽ��ȡNULL��
//        Send_Buff�������͵����ݰ�������Э��ͷ
//        len���������ݰ�����
// ����ֵ��  ��
// ============================================================================
ptu32_t Master_INS_Process(u32 ReplyModel,u32 ConfSlave,u32 Timeout,u32 PoolModel,u32 SlaveNum,u8 *Send_Buff,u16 len)
{
    u8 pos=1;
    u32 times=0;
    static u32 timeout;
    static u8 Slave_Address=1;
    static u8 Slave_Rank=1;
    struct InSerSocket *Me;
    struct InSerSocketPort *Port;

    while(1) //�ȴ���ֱ��ע��ͨ�Ŷ˿�
    {
        if(Master_sg_ptPortHead != NULL)
            break ;
        Djy_EventDelay(100*mS);
    }
    timeout=Timeout;
    Port=Master_sg_ptPortHead;
    Me=Port->SocketHead;

    if(ConfSlave==SLAVE_INFORM_NUKNOWN)
    {
        while(1)
        {
            INS_SendPkg(Me,Slave_Address,Master_sg_u8Address,Send_Buff,len);
            __INS_Process(timeout);

            if(Effective_Slave)
            {
               SlaveList=Slave_Creat(Me->CommPort,Me->CommPort->SerialDevice,SlaveList,
                                                     Slave_Address,Slave_Rank,timeout,HALF_DUPLEX_COMM);
               Slave_Rank++;
               Effective_Slave=0;
            }
            Slave_Address++;
               if(Slave_Address==255)
                   break;
        }
        SlaveNum=COUNT_Slave;
    }

    while(SlaveList->Rank!=pos) //�����ҵ��·�˳��Ϊ1�Ĵӻ�
    {
        SlaveList=SlaveList->Next;
    }

    //1����ʱ���ڲ���Ҫ�󣬸����ӻ��ж��Ͷ��ظ�
    if((PoolModel==NO_TIMER_POLL)&&(ReplyModel==ONE_BY_ONE))
    {
        while(1)
        {
            INS_SendPkg(Me,SlaveList->Address,Master_sg_u8Address,Send_Buff,len);
            __INS_Process(SlaveList->Timeout);
            SlaveList=SlaveList->Next;
        }
    }
    //2���ȼ��������ѯ�����ӻ�
    if((PoolModel==EVENLY_SPACED_POLL)&&(ReplyModel==ONE_BY_ONE))
    {
        while(1)
        {
            if(Lock_SempPend(UartTimerSemp,CN_TIMEOUT_FOREVER))
            {
                INS_SendPkg(Me,SlaveList->Address,Master_sg_u8Address,Send_Buff,len);
                __INS_Process(SlaveList->Timeout);
                SlaveList=SlaveList->Next;
            }
        }
    }
    //3���ܼ��������ѯ���дӻ�
    if((PoolModel==ACCUMU_TIMER_POLL)&&(ReplyModel==ONE_BY_ONE))
    {
        while(1)
        {
            if(Lock_SempPend(UartTimerSemp,CN_TIMEOUT_FOREVER))
            {
                while(1)
                {
                    INS_SendPkg(Me,SlaveList->Address,Master_sg_u8Address,Send_Buff,len);
                    __INS_Process(SlaveList->Timeout);
                    SlaveList=SlaveList->Next;
                    times++;
                    if(times==SlaveNum)
                    {
                        times=0;
                        break;
                    }
                }
            }
        }
    }
    //4�����ӻ�һ�ʶ��ģʽ
    if((PoolModel==NO_TIMER_POLL)&&(ReplyModel==ONE_BY_MORE))
    {
       INS_SendPkg(Me,SlaveList->Address,Master_sg_u8Address,Send_Buff,len);
       while(1)
       {
           __INS_Process(SlaveList->Timeout);
           SlaveList=SlaveList->Next;
       }
    }
    return 0;
}


// ============================================================================
// �������ܣ��ӻ���Э�鴦�������ô���������Ϊһ�������ȼ����̵߳���
// �����������
// ����ֵ��  ��
// ============================================================================
ptu32_t Slave_INS_Process(void)
{
    u32 timeout,sta,Completed,Completed1;
    ptu32_t DevRe;
    struct InSerSocketPort *Port;
    struct InSerSocket *Me;
    struct DjyDevice *Device; //��Ӧ���豸ָ��
    u8 FSM,temp,loop;
    while(1)
    {
        //���ڶ��¼��������Լ����ȼ��Ĺ�ϵ����������ʱ�����ܻ�û��ע��ͨ�ſڣ���
        //����û100mS�鿴һ�£�ֱ��ע��ͨ�ſڡ�
        if(Slave_sg_ptPortHead != NULL)
            break ;
        Djy_EventDelay(100*mS);
    }
    Port = Slave_sg_ptPortHead;
    timeout = Port->Timeout;
    while(Port != NULL)
    {
        if(timeout > Port->Timeout)
            timeout = Port->Timeout;
        Port = Port->Next;
    }

    while(1)
    {
        u32 Len;
        //�ȴ���·���ô��������ͨ�ſ�ֻҪ��һ��������������
        //ע�����ڶ�·���ñ�����ΪET�����ش��������ԣ�ÿ�δ���������ȫ����ȡ���ݡ�
        DevRe = Multiplex_Wait(Slave_sg_ptMultiplexPort, &sta, timeout); //���������,����һ����������ObjectID,����-1
        if(DevRe != -1)
            Device = (struct DjyDevice *)DevRe;
        else
        {
            continue; //����-1������ȴ�
        }
        //�����豸��UserTag��Ϊ�˿�ָ��
        Port = (struct InSerSocketPort *)Driver_GetUserTag(Device); //��ȡ�û���ǩ
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
				Completed1 = Driver_ReadDevice(Device, &Port->RecvPkgBuf[Port->relen],Port->PortMaxRecvLen, 0, 0);
				if(Completed1==0) //����0������
					break;
				FSM = Port->fsm;  //��ǰ���յ������״̬��FSM
				offset = FSM;
				Completed1=Completed1+Port->relen;
				Port->relen=0;
            	Completed=sizeof(struct InSerProtocol); //��ͷ����

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
								if((Port->RecvPkgBuf[offset] == Slave_sg_u8Address)
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
							case CN_OFF_PROTO: //����Э���ʱ��֪���ĸ����
								__INS_GetProtocol(Port,Port->RecvPkgBuf[offset]);
								offset++;
								FSM = CN_OFF_SRC;
								if(FSM==Port->relen)
								{
									goto tail;
								}
								break ;
							case CN_OFF_SRC: //Դ��ַ
								offset++;
								FSM = CN_OFF_LENL;
								if(FSM==Port->relen)
								{
									goto tail;
								}
								break ;
							case CN_OFF_LENL:  //���ȵ��ֽ�
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
								FSM = CN_OFF_LENH;
								if(FSM==Port->relen)
								{
									goto tail;
								}
								break ;
							case CN_OFF_LENH: //���ȸ��ֽ�
								if( (Port->RecvPkgBuf[offset-1] + ((u16)Port->RecvPkgBuf[offset] << 8))
									> Port->SocketHead->ProtocolRecvLen)
								{
								    offset++;
									if(Port->fnError != NULL)
										Port->fnError(Port, CN_INS_LEN_ERR);
									Port->ErrorLast = CN_INS_LEN_ERR;
									Port->ErrorPkgs++; //�������
									FSM = CN_OFF_START;
									if(FSM==Port->relen)
									{
										goto tail;
									}
								}
								offset++;
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
								if(temp != Port->RecvPkgBuf[offset++])  //У��Ͳ�����ǰ6����Ա�ĺ�,�򷵻���CN_OFF_START
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
								else  //У��͵���ǰ6����Ա�ĺ�
								{
								    if((Port->RecvPkgBuf[offset-6] != Slave_sg_u8Address) //��ַ����
								     &&(Port->RecvPkgBuf[offset-6] != Port->MTC_Address)
								     &&(Port->RecvPkgBuf[offset-6] != CN_INS_BROADCAST ))
									{
										u32 reln=Port->RecvPkgBuf[CN_OFF_LENL]+ (Port->RecvPkgBuf[CN_OFF_LENH] << 8);
										//����1��,��һ���Ǳ����
										if(((reln+sizeof(struct InSerProtocol))<Completed1))
										{
											for(loop=0;loop<(Completed1-(reln+sizeof(struct InSerProtocol)));loop++) //��λ
											Port->RecvPkgBuf[loop] = Port->RecvPkgBuf[reln+sizeof(struct InSerProtocol)+loop];
											Port->relen=(Completed1-(reln+sizeof(struct InSerProtocol)));
											Completed1=Port->relen;
											goto head;
										}
										if((reln+sizeof(struct InSerProtocol))==Completed1)  //�жϵڶ����Ƿ�Ϊ������
										{
											Port->relen=0;
										}
										Completed1=sizeof(struct InSerProtocol); //����Ӧ������
										FSM=0;  //״̬������
										goto tail;  //����Ӧ������ֱ������
									}
									else
									{
 									    FSM = CN_OFF_USER;  //�û�����,FSM=7
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
                Len = (Port->RecvPkgBuf[CN_OFF_LENL] + (Port->RecvPkgBuf[CN_OFF_LENH] << 8));
                if((Len>(Completed1-sizeof(struct InSerProtocol)))&&(FSM==CN_OFF_USER))
                {
                	goto tail; //����������ѭ��
                }
                if((Len<=(Completed1-sizeof(struct InSerProtocol)))&&(FSM==CN_OFF_USER))
				{
                	FSM = CN_OFF_START; //״̬�����ص���������Э��ͷ��״̬
					Port->fsm = FSM;
					Me = Port->SocketHead;  //InSerSocketPort��ͨѶЭ�鸳��Me
					Me->MyProcess(Me, &Port->RecvPkgBuf[CN_OFF_USER],Len);  //InSerSocket��ӦЭ�鴦����
					Port->RecvP = 0;
					Port->relen=0;
				}
                //����1��,��1��Ϊ�����
                if(((Len+sizeof(struct InSerProtocol))<Completed1))
                {
				   for(loop = 0; loop < (Completed1-(Len+sizeof(struct InSerProtocol))); loop++) //��λ
					   Port->RecvPkgBuf[loop] = Port->RecvPkgBuf[loop+(Len+sizeof(struct InSerProtocol))];
				   Port->relen=Completed1-(Len+sizeof(struct InSerProtocol));
				   Completed1=Port->relen;
				   goto head;
                }
                tail:break; //����������ѭ��
            }
        }
        if(sta & CN_MULTIPLEX_SENSINGBIT_WRITE) //�������״̬��д
        {    //�ж��ҵ�ǰ�ķ���û��,û���꿴����Ҫ������,������
             if(Port->SendP < Port->PortSendLen + sizeof(struct InSerProtocol))
            {
                u32 SendLen;
                SendLen = Port->PortSendLen + sizeof(struct InSerProtocol) - Port->SendP;
                Completed = Driver_WriteDevice(Device, Port->SendPkgBuf + Port->SendP,
                                                SendLen, 0, CN_BLOCK_BUFFER, 0);
                if(Completed != -1)
                    Port->SendP += Completed;
            }
        }
        if(sta & CN_MULTIPLEX_SENSINGBIT_ERROR) //�������״̬����,�͵��ó���Ļص�����,��¼�����
        {
            if(Port->fnError != NULL)
                Port->fnError(Port, CN_INS_PHY_ERR);
            Port->ErrorLast = CN_INS_PHY_ERR;
            Port->ErrorPkgs++;
        }
    }
}

// ============================================================================
// �������ܣ��ӻ����ְ�װ�ڲ�����ͨ��ģ�顣��ģ��������ڴӻ����֡�
// ���������StackSize��ģ����Ҫ���ڴ�ߴ磬���ڴ���ͨ��Э�������Ҫ�����û�
//              �ṩ�Ļص������������ݣ���ģ��������Ҫ���ڴ���٣���಻������
//              �ֽڣ����û������Լ��Ļص�����������
// ����ֵ�� ���ǳɹ������ɹ��ͽ�����ѭ����
// ============================================================================
bool_t Slave_ModuleInstall_InSerial(u32 StackSize)
{
    u16 evtt,event;
    Slave_sg_ptMultiplexPort = Multiplex_Create(1);

    evtt = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RLYMAIN, 1, 1, Slave_INS_Process,
                            NULL, StackSize, "In serial com process with slave");
    event = Djy_EventPop(evtt, NULL, 0, 0, 0, 0);
    if((evtt == CN_EVTT_ID_INVALID) || (event == CN_EVENT_ID_INVALID)
         || (Slave_sg_ptMultiplexPort == NULL) )
    {
        printf("�ӻ������ڲ�����ͨ��ģ���ʼ���쳣\n\r");
        while(1);                               //��ʼ���׶ξ��쳣��ֱ����ѭ��
    }
    else
    {
        return true;
    }
}

// ============================================================================
// �������ܣ�ע��ģ��ʹ�õ�ͨ�Ŷ˿ڣ���ʵ�ϣ����Ǵ���Ҳ����ע��ġ�
//        ͨ�Ŷ˿ڱ��밴djyos���豸��������������������ӻ����á�
// ���������dev��ͨ�Ŷ˿ڵ��豸ָ��
//        comType��ͨ�����ͣ�ȡֵMASTER_COMM_PORT��SLAVE_COMM_PORT
//                 ע��������ͨ�Ŷ˿�--MASTER_COMM_PORT;ע��ӻ���ͨ�Ŷ˿�--SLAVE_COMM_PORT
//        fnError��ͨ�Ŵ����Ӧ�Ļص�����������Ҫ���NULL
//        Timeout����ʱ��û���յ����ݵĳ�ʱ�澯���ޣ�uSΪ��λ����ʱ�侫����
//                ����ϵͳtick������ơ�0xffffffff��-1�������賬ʱ��
// ����ֵ��  ͨ�Ų��ָ��
// ============================================================================
struct InSerSocketPort *INS_RegistPort(struct DjyDevice *dev,u8 comType,
                                       fntProtocolError fnError,u32 Timeout)
{
    struct InSerSocketPort *Port;
    if(dev == NULL)
        return NULL;
    Port = (struct InSerSocketPort *)malloc(sizeof(struct InSerSocketPort ));
    if(Port != NULL) //����ɹ�
    {
        if(comType==0)
        {
            if(Master_sg_ptPortHead == NULL)
            {
                Port->Next = NULL;
            }
            else
            {
                Port->Next = Master_sg_ptPortHead;
            }
            Master_sg_ptPortHead = Port;
        }
        else
        {
            if(Slave_sg_ptPortHead == NULL)
            {
                Port->Next = NULL;
            }
            else
            {
                Port->Next = Slave_sg_ptPortHead;
            }
            Slave_sg_ptPortHead = Port;//���뵽��Port����sg_ptPortHead��Port���м���
        }
        Port->SocketHead = NULL; //��InSerSocket������г�ʼ��
        Port->SerialDevice = dev;
        Port->fnError = fnError;
        Port->ErrorPkgs = 0;
        Port->ErrorLast = CN_INS_OK;
        Port->Timeout = Timeout;
        Port->Resttime = 0;
        Port->fsm = CN_OFF_START;
        Port->relen = 0;        //����ƫ����
        Port->SendPkgBuf = NULL;
        Port->RecvPkgBuf = NULL;
        Port->PortMaxRecvLen = 0;
        Port->PortMaxSendLen = 0;
        Port->PortSendLen = 0;
        Port->RecvP = 0;
        //����0����ʼ״̬�ᷢ��һ�����ݡ�
        Port->SendP = sizeof(struct InSerProtocol);  //����ƫ����ΪЭ��ͷ
        Port->MTC_Address = CN_MTC_RESV;
        Driver_SetUserTag(dev, (ptu32_t)Port);
        if(comType==0)//��Ӷ�·����
        {
            Driver_MultiplexAdd(Master_sg_ptMultiplexPort, &dev, 1,
                                               CN_MULTIPLEX_SENSINGBIT_READ
//                                            |  CN_MULTIPLEX_SENSINGBIT_WRITE  //���������ˣ���д������
                                            |  CN_MULTIPLEX_SENSINGBIT_ERROR
                                            |  CN_MULTIPLEX_SENSINGBIT_ET
                                            |  CN_MULTIPLEX_SENSINGBIT_OR    );
        }
        else
        {
            Driver_MultiplexAdd(Slave_sg_ptMultiplexPort, &dev, 1,
                                               CN_MULTIPLEX_SENSINGBIT_READ
                                            |  CN_MULTIPLEX_SENSINGBIT_WRITE
                                            |  CN_MULTIPLEX_SENSINGBIT_ERROR
                                            |  CN_MULTIPLEX_SENSINGBIT_ET
                                            |  CN_MULTIPLEX_SENSINGBIT_OR    );
        }
    }
    return Port;
}

// ============================================================================
// �������ܣ�ע��ͨ��Э�顣
// ���������Port��ͨ�Ų��ָ�룬ΪINS_RegistPort�����ķ���ֵ
//        Protocol��Э��ţ��� CN_INS_PROTOCOL_DEFAULT �ȳ�������
//        MaxRecvLen����Э��������ݰ�����󳤶�
//        MaxSendLen����Э�鷢�����ݰ�����󳤶�
//        fn���������ݰ���Э�鴦����
// ����ֵ��  Э����ָ��
// ============================================================================
struct InSerSocket *INS_RegistProtocol(struct InSerSocketPort *Port, u8 Protocol,
                                      u16 MaxRecvLen,u16 MaxSendLen, fntProtocolProcess fn)
{
    struct InSerSocket *ProtocolSocket;
    bool_t result = true;
    if(Port == NULL)
        return NULL;
    ProtocolSocket = (struct InSerSocket *)malloc(sizeof(struct InSerSocketPort ));
    if(ProtocolSocket != NULL)
    {
        //result���ڳ����жϣ�������ָ���Ƿ�NULL����Ϊ�е���������ͨ�ſڣ�Len=0
        if(MaxRecvLen > Port->PortMaxRecvLen)
        {
            Port->RecvPkgBuf = (u8*)realloc(Port->RecvPkgBuf,  //�������ݰ�ָ�루����Э��ͷ��
            		                    (MaxRecvLen+ sizeof(struct InSerProtocol)));

            Port->PortMaxRecvLen = (MaxRecvLen+ sizeof(struct InSerProtocol));
            result = (Port->RecvPkgBuf != NULL);
        }
        if(MaxSendLen > Port->PortMaxSendLen) //���Ͳ���
        {
            Port->SendPkgBuf = (u8*)realloc(Port->SendPkgBuf,
                                       MaxSendLen + sizeof(struct InSerProtocol));
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
        ProtocolSocket->Protocol = Protocol;           //Э���
        ProtocolSocket->ProtocolRecvLen = MaxRecvLen;  //��Э�������հ�����
        ProtocolSocket->ProtocolSendLen = MaxSendLen;  //��Э������Ͱ�����
        ProtocolSocket->CommPort = Port;               //��Ӧ��ͨ�ſ�ָ��
        ProtocolSocket->MyProcess = fn;                //Э�鴦����ָ��
    }
    if((ProtocolSocket == NULL) || (result == false) )
    {
        printf("INS_RegistProtocol failure");
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
// ���������InSerSocket��ͨ��Э����ָ�룬ΪINS_RegistProtocol�����ķ���ֵ
//        dst��Ŀ�ĵ�ַ�����������ˣ�Ŀ�ĵ�ַΪ�ӻ��ӻ���ַ��ԭ��ַΪMaster_Address�����ڴӻ���Ŀ�ĵ�ַΪ������ַ��
//             Դ��ַΪ�����ַSlave_sg_u8Address
//        src��Դ��ַ
//        buf�������͵����ݰ�������Э��ͷ
//        len�����ͳ���
// ����ֵ��  ���͵���������ֻ��copy���˷���buf��
// ============================================================================
u32 INS_SendPkg(struct InSerSocket  *InSerSocket, u8 dst,u8 src, u8 *buf, u16 len)
{
    struct InSerSocketPort *Port;
    u8 *SendBuf;
    u16 SendLen;
    u32 Completed;
    if(InSerSocket == NULL)
        return 0;
    if(len > InSerSocket->ProtocolSendLen)
        return 0;
    Port = InSerSocket->CommPort;
    Port->PortSendLen = len;    //����Э��ͷ
    SendBuf = Port->SendPkgBuf;
    SendBuf[CN_OFF_START]   = 0xEB;
    SendBuf[CN_OFF_DST]     = dst;
    SendBuf[CN_OFF_PROTO]   = InSerSocket->Protocol;
    SendBuf[CN_OFF_SRC]     = src;
    SendBuf[CN_OFF_LENL]    = len & 0xff;
    SendBuf[CN_OFF_LENH]    = len >> 8;
    SendBuf[CN_OFF_CHKSUM]  = 0xEB + dst + InSerSocket->Protocol
                              + src + (len & 0xff) + (len >> 8);
    SendLen = len + sizeof(struct InSerProtocol);
    memcpy(SendBuf + sizeof(struct InSerProtocol), buf, len);

    Completed = Driver_WriteDevice(Port->SerialDevice, SendBuf, SendLen,
                                   0, CN_BLOCK_BUFFER, 0);
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

    if(PoolModel==NO_TIMER_POLL)
       return;
    if(PoolModel==EVENLY_SPACED_POLL||PoolModel==ACCUMU_TIMER_POLL)
    {
        UartTimerSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"UART_TIMER_SEMP");
        uartTimerHandle=HardTimer_Alloc(__Uart_HardTimer);   //��ʱ������
        if(uartTimerHandle==(-1))
           printf("�����˶�ʱ��ѯģʽ�����쳣\n\r");
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
// �������ܣ���ӴӰ岢���ò�����
// ���������Port����Ӧͨ�Ŷ˿�ָ��
//        SerialDeviceͨ�Ŷ˿��豸ָ��
//        pHead���ӻ��ṹ������ָ��
//        address���ӻ���ַ
//        rank������������ӻ��·����ݰ�˳��
//        timeout����ʱʱ�䣬��֪����ʱʱ�������ΪNULL
//        commcode��ͨѶģʽ����˫���򵥹���ȡֵHALF_DUPLEX_COMM��SIMPLES_COMM
// ����ֵ���ӻ��ṹ������ָ��
// ============================================================================
struct Slave *Slave_Creat(struct InSerSocketPort *Port,struct DjyDevice *dev,
                                   struct Slave *pHead,u8 address,u8 rank,u32 timeout,u32 commcode)
{
    struct Slave *p1;
    p1=(struct Slave*)malloc(sizeof(struct Slave));//�����½ڵ�
    if(p1==NULL)
    {
        printf("�ڴ�����ʧ��\r\n");
    }
    p1->CommPort=Port;
    p1->SerialDevice=dev;
    p1->Address=address;
    p1->Rank = rank;
    p1->Timeout=timeout;
    p1->Commcode=commcode;

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
        printf("��Ӵӻ�ʧ��\r\n");
        while(1);   //��ʼ���׶ξ��쳣��ֱ����ѭ��
    }
    return pHead;
}

// ============================================================================
// �������ܣ����Ѵ����Ĵӻ���ɾ��ָ���ӻ���
// ���������pHead���ӻ��ṹ������ָ��
//       deleteaddr����ɾ���ӻ���ַ
// ����ֵ�� �ӻ��ṹ������ָ��
// ============================================================================
struct Slave * Slave_Delete (struct Slave * pHead, int deleteaddr)
{
    struct Slave* p,*delete;
    p = pHead;

    if(p==NULL)
    {
        printf("No Slave!\n"); //û�дӻ�
    }
    do
    {
        if(p->Next->Address==deleteaddr)
        break;
        else
        p = p->Next;
    } while (p!= pHead);

    if (p==pHead&&pHead->Next->Address!=deleteaddr)
    {
        printf("Not found!\n");
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
        printf("No Slave!\n"); //û�дӻ�
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
// �������ܣ�����������ַ��
// ���������Addr��������ַ��8λ
// ����ֵ�� ��
// ============================================================================
void Master_INS_SetAddress(u8 Addr)
{
    if(Addr < CN_MTC_RESV)
        Master_sg_u8Address = Addr;
}

// ============================================================================
// �������ܣ����ôӻ���ַ��
// ���������Addr��������ַ��8λ
// ����ֵ�� ��
// ============================================================================
void Slave_INS_SetAddress(u8 Addr)
{
    if(Addr < CN_MTC_RESV)
        Slave_sg_u8Address = Addr;
}

// ============================================================================
// �������ܣ����ñ������ܵ��鲥��ַ��
// ���������Port�������õ�Ŀ��ͨ�ſ�
//           Addr���鲥��ַ��8λ
// ����ֵ�� ��
// ============================================================================
void INS_SetMtcAddress(struct InSerSocketPort *Port, u8 Addr)
{
    if((Addr >= CN_MTC_RESV) && (Addr != CN_INS_BROADCAST) && (Port != NULL))
        Port->MTC_Address = Addr;
}
