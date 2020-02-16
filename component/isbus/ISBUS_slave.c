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
#include "ISBUS_slave.h"
#include "timer_hard.h"
#include "dbug.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t Slave_ModuleInstall_InSerial(u32 StackSize);
//    Slave_ModuleInstall_InSerial(CFG_INSERIAL_STACK_SIZE);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"isbus slave"//Ƕ��ʽ����ͨ�Ŵӻ���
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","lock","multiplex"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������
//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_ISBUS_SLAVE == false )
//#warning  " isbus_slave  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_ISBUS_SLAVE    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,0x10000,
#define CFG_INSERIAL_STACK_SIZE     4096     //"Э�鴦��ջ�ߴ�",����serialģ��Э�鴦��������Ҫ�ľֲ��ڴ�
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
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
struct Slave_ISBUSProtocol
{
    u8 start;           //0xeb
    u8 DstAddress;      //Ŀ���ַ
    u8 Protocol;        //������
    u8 SrcAddress;      //Դ��ַ
    u8 Len;             //���ݰ����ȣ�2byte
    u8 ChkSum;          //У��ͣ���ǰ5����Ա�ĺ͡�
};

//ͨ�Ŷ˿ڵ�����
struct Slave_ISBUSPort
{
    struct Slave_ISBUSPort *Next;           //��ɵ�������ĩ��ָ��NULL
    struct Slave_FunctionSocket *SocketHead;//��ɵ���ѭ������Headָ��ǰ���յĹ��ܺ�ֵ
    s32 SerialDevice;         //��Ӧ���豸ָ��
    Slave_FntProtocolError fnError;         //�����Ӧ�Ļص�����
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
struct Slave_FunctionSocket
{
    struct Slave_FunctionSocket *Next;      //���һ������ĩ��ָ��NULL
    u8 Protocol;                            //������
    u16 ProtocolRecvLen;                    //��Э�������հ�����
    u16 ProtocolSendLen;                    //��Э������Ͱ�����
    struct Slave_ISBUSPort *CommPort;       //��Ӧ��ͨ�ſ�ָ��
    Slave_FntProtocolProcess MyProcess;     //Э�鴦����ָ��
};

struct MultiplexSetsCB * Slave_sg_ptMultiplexPort;  //�ӻ���·����
u8 Slave_sg_u8Address = 0;                          //�ӻ����ص�ַ
u8 Slave_Broadcast_u8Address = 0;                   //�ӻ��鲥��
struct Slave_ISBUSPort *Slave_sg_ptPortHead = NULL; //�ӻ��������ʼ��

u8 Host_Address=0;  //���ʱ�����������ַ

void __Slave_GetProtocol(struct Slave_ISBUSPort *Port,u8 Protocol)
{
    struct Slave_FunctionSocket *Next,*SocketHead;
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

// ============================================================================
// �������ܣ��ӻ���Э�鴦�������ô�����һ����Ϊһ�������ȼ����̵߳���
// �����������
// ����ֵ��  ��
// ============================================================================
ptu32_t Slave_ISBUS_Process(void)
{
    u32 timeout,sta,Completed,Completed1;
    s32 DevRe;
    struct Slave_ISBUSPort *Port;
    struct Slave_FunctionSocket *Me;
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
//        Port = (struct Slave_ISBUSPort *)Driver_GetUserTag(Device); //��ȡ�û���ǩ
        Port = (struct Slave_ISBUSPort *)Dev_GetDevTag(Device); //��ȡ�û���ǩ
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
//              Completed1 = Driver_ReadDevice(Device, &Port->RecvPkgBuf[Port->relen],Port->PortMaxRecvLen, 0, 0);
                Completed1 = DevRead(Device, &Port->RecvPkgBuf[Port->relen],Port->PortMaxRecvLen, 0, 0);
                if(Completed1==0) //����0������
                    break;
                FSM = Port->fsm;  //��ǰ���յ������״̬��FSM
                offset = FSM;
                Completed1=Completed1+Port->relen;
                Port->relen=0;
                Completed=sizeof(struct Slave_ISBUSProtocol); //��ͷ����
//              Completed=6;
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
                                offset++;
                                break ;
                            case CN_OFF_PROTO: //���չ�����ʱ��֪���ĸ����
                                __Slave_GetProtocol(Port,Port->RecvPkgBuf[offset]);
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
                                //*******************************************************************************
//                              if((Port->RecvPkgBuf[1]==CN_INS_BROADCAST)||
//                                          (Port->RecvPkgBuf[1] == Port->MTC_Address))  //����յ��㲥��ַ���鲥��ַ��
//                                                                                         //��ʱ��������ַ��¼����
//                              {
                                    Host_Address=Port->RecvPkgBuf[3];
//                              }
                                //*******************************************************************************
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
                                else  //У��͵���ǰ5����Ա�ĺ�
                                {
                                    if(Port->RecvPkgBuf[offset-5] != Slave_sg_u8Address)  //��ַ����
//                                   &&(Port->RecvPkgBuf[offset-5] != Port->MTC_Address)
//                                   &&(Port->RecvPkgBuf[offset-5] != CN_INS_BROADCAST ))
                                    {
                                        //******************************************************************************
                                        if((Port->RecvPkgBuf[offset-5]==CN_INS_BROADCAST)||
                                                (Port->RecvPkgBuf[offset-5] == Port->MTC_Address)) //�����ַ���ԣ���Ϊ�㲥���鲥ʱ
                                        {
                                            if(Slave_sg_u8Address==1) //�����ַΪ�ӻ��б��е�ַΪ1���Ǹ�
                                            {
                                                FSM = CN_OFF_USER;
                                            }
                                        }

                                        if((Port->RecvPkgBuf[1]==Host_Address)&&
                                                  (Port->RecvPkgBuf[3])==Slave_sg_u8Address-1) //һ�ʶ�𣬱����յ���һ��ظ������ı���
                                        {
                                            FSM = CN_OFF_USER;  //�û�����,FSM=6,ֱ�ӵ���Э�鴦�������з�������
                                        }
                                        //*******************************************************************************

                                        u32 reln=Port->RecvPkgBuf[CN_OFF_LEN];
                                        //����1��,��һ���Ǳ����
                                        if(((reln+sizeof(struct Slave_ISBUSProtocol))<Completed1))
                                        {
                                            for(loop=0;loop<(Completed1-(reln+sizeof(struct Slave_ISBUSProtocol)));loop++) //��λ
                                            Port->RecvPkgBuf[loop] = Port->RecvPkgBuf[reln+sizeof(struct Slave_ISBUSProtocol)+loop];
                                            Port->relen=(Completed1-(reln+sizeof(struct Slave_ISBUSProtocol)));
                                            Completed1=Port->relen;
                                            goto head;
                                        }
                                        if((reln+sizeof(struct Slave_ISBUSProtocol))==Completed1)  //�жϵڶ����Ƿ�Ϊ������
                                        {
                                            Port->relen=0;
                                        }
                                        Completed1=sizeof(struct Slave_ISBUSProtocol); //����Ӧ������
                                        FSM=0;  //״̬������
                                        goto tail;  //����Ӧ������ֱ������
                                    }
                                    else//�����ַ������
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
                if((Len>(Completed1-sizeof(struct Slave_ISBUSProtocol)))&&(FSM==CN_OFF_USER))
                {
                    goto tail; //����������ѭ��
                }
                if((Len<=(Completed1-sizeof(struct Slave_ISBUSProtocol)))&&(FSM==CN_OFF_USER))
                {
                    FSM = CN_OFF_START; //״̬�����ص���������Э��ͷ��״̬
                    Port->fsm = FSM;
                    Me = Port->SocketHead;  //Slave_ISBUSPort��ͨѶЭ�鸳��Me
                    Me->MyProcess(Me, &Port->RecvPkgBuf[CN_OFF_USER],Len);
                    Port->RecvP = 0;
                    Port->relen=0;
                }
                //����1��,��1��Ϊ�����
                if(((Len+sizeof(struct Slave_ISBUSProtocol))<Completed1))
                {
                   for(loop = 0; loop < (Completed1-(Len+sizeof(struct Slave_ISBUSProtocol))); loop++) //��λ
                       Port->RecvPkgBuf[loop] = Port->RecvPkgBuf[loop+(Len+sizeof(struct Slave_ISBUSProtocol))];
                   Port->relen=Completed1-(Len+sizeof(struct Slave_ISBUSProtocol));
                   Completed1=Port->relen;
                   goto head;
                }
                tail:break;
            }
        }
        if(sta & CN_MULTIPLEX_SENSINGBIT_WRITE) //�������״̬��д���ж��ҵ�ǰ�ķ���û��,û���꿴����Ҫ������,������
        {
             if(Port->SendP < Port->PortSendLen + sizeof(struct Slave_ISBUSProtocol))
            {
                u32 SendLen;
                SendLen = Port->PortSendLen + sizeof(struct Slave_ISBUSProtocol) - Port->SendP;
//                Completed = Driver_WriteDevice(Device, Port->SendPkgBuf + Port->SendP,
//                                                SendLen, 0, CN_BLOCK_BUFFER, 0);
                Completed = DevWrite(Device, Port->SendPkgBuf + Port->SendP,
                                                SendLen, 0,0);
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
// �������ܣ���װ�ڲ�����ͨ��ģ�顣��ģ��������ڴӻ����֡�
// ���������StackSize��ģ����Ҫ���ڴ�ߴ磬���ڴ���ͨ��Э�������Ҫ�����û�
//              �ṩ�Ļص������������ݣ���ģ��������Ҫ���ڴ���٣���಻������
//              �ֽڣ����û������Լ��Ļص�����������
//       CN_PRIO_RLYMAIN���¼����ȼ������û��Լ�ѡȡ��һ��ѡ��ϸ����ȼ�
// ����ֵ�� ���ǳɹ������ɹ��ͽ�����ѭ����
// ============================================================================
bool_t Slave_ModuleInstall_InSerial(u32 StackSize)
{
    u16 evtt,event;
    Slave_sg_ptMultiplexPort = Multiplex_Create(1);

    evtt = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RLYMAIN-5, 1, 1, Slave_ISBUS_Process,
                            NULL, StackSize, "ISBUS com process with slave");
    event = Djy_EventPop(evtt, NULL, 0, 0, 0, 0);
    if((evtt == CN_EVTT_ID_INVALID) || (event == CN_EVENT_ID_INVALID)
         || (Slave_sg_ptMultiplexPort == NULL) )
    {
        debug_printf("ISBUS","ISBUSͨ��ģ��ӻ��˳�ʼ���쳣\n\r");
        while(1);                               //��ʼ���׶ξ��쳣��ֱ����ѭ��
    }
    else
    {
        return true;
    }
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
struct Slave_ISBUSPort *Slave_ISBUS_RegistPort(s32 dev,\
                               Slave_FntProtocolError fnError,u32 Timeout)
{
    struct Slave_ISBUSPort *Port;
    if(dev == NULL)
        return NULL;
    Port = (struct Slave_ISBUSPort *)malloc(sizeof(struct Slave_ISBUSPort ));
    if(Port != NULL) //����ɹ�
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
        Port->SocketHead = NULL;   //��Slave_FunctionSocket������г�ʼ��
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
        Port->SendP = sizeof(struct Slave_ISBUSProtocol);  //����ƫ����ΪЭ��ͷ
        Port->MTC_Address = CN_INS_MULTICAST;
//        Driver_SetUserTag(dev, (ptu32_t)Port);
        dev_SetUserTag(dev, (ptu32_t)Port);
//        Driver_MultiplexAdd(Slave_sg_ptMultiplexPort, &dev, 1,
//                                               CN_MULTIPLEX_SENSINGBIT_READ
//                                            |  CN_MULTIPLEX_SENSINGBIT_WRITE
//                                            |  CN_MULTIPLEX_SENSINGBIT_ERROR
//                                            |  CN_MULTIPLEX_SENSINGBIT_ET
//                                            |  CN_MULTIPLEX_SENSINGBIT_OR    );
        Multiplex_AddObject(Slave_sg_ptMultiplexPort,  dev,
                                               CN_MULTIPLEX_SENSINGBIT_READ
                                            |  CN_MULTIPLEX_SENSINGBIT_WRITE
                                            |  CN_MULTIPLEX_SENSINGBIT_ERROR
                                            |  CN_MULTIPLEX_SENSINGBIT_ET
                                            |  CN_MULTIPLEX_SENSINGBIT_OR    );
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
struct Slave_FunctionSocket *Slave_ISBUS_RegistProtocol(struct Slave_ISBUSPort *Port, u8 Protocol,
                                            u16 MaxRecvLen,u16 MaxSendLen, Slave_FntProtocolProcess fn)
{
    struct Slave_FunctionSocket *ProtocolSocket;
    bool_t result = true;
    if(Port == NULL)
        return NULL;
    ProtocolSocket = (struct Slave_FunctionSocket *)malloc(sizeof(struct Slave_ISBUSPort ));
    if(ProtocolSocket != NULL)
    {
        //result���ڳ����жϣ�������ָ���Ƿ�NULL����Ϊ�е���������ͨ�ſڣ�Len=0
        if(MaxRecvLen > Port->PortMaxRecvLen)
        {
            Port->RecvPkgBuf = (u8*)realloc(Port->RecvPkgBuf,  //�������ݰ�ָ�루����Э��ͷ��
                                        (MaxRecvLen+ sizeof(struct Slave_ISBUSProtocol)));

            Port->PortMaxRecvLen = (MaxRecvLen+ sizeof(struct Slave_ISBUSProtocol));
            result = (Port->RecvPkgBuf != NULL);
        }
        if(MaxSendLen > Port->PortMaxSendLen) //���Ͳ���
        {
            Port->SendPkgBuf = (u8*)realloc(Port->SendPkgBuf,
                                       MaxSendLen + sizeof(struct Slave_ISBUSProtocol));
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
        ProtocolSocket->Protocol = Protocol;           //������
        ProtocolSocket->ProtocolRecvLen = MaxRecvLen;  //��Э�������հ�����
        ProtocolSocket->ProtocolSendLen = MaxSendLen;  //��Э������Ͱ�����
        ProtocolSocket->CommPort = Port;               //��Ӧ��ͨ�ſ�ָ��
        ProtocolSocket->MyProcess = fn;                //Э�鴦����ָ��
    }
    if((ProtocolSocket == NULL) || (result == false) )
    {
        debug_printf("ISBUS","Slave_INS_RegistProtocol failure");
        free(ProtocolSocket);
        free(Port->RecvPkgBuf);
        free(Port->SendPkgBuf);
        return NULL;
    }
    else
        return ProtocolSocket;
}
// ============================================================================
// �������ܣ����ݰ����ͺ��������û����ã�����������Э��ͷȻ���ͳ�ȥ���ӻ��ˡ�
// ���������Slave_FunctionSocket��ͨ��Э����ָ�룬ΪINS_RegistProtocol�����ķ���ֵ
//        dst�����ڴӻ���Ŀ�ĵ�ַΪ������ַ��Դ��ַΪ�����ַSlave_sg_u8Address
//        src��Դ��ַ
//        buf�������͵����ݰ�������Э��ͷ
//        len�����ͳ���
// ����ֵ��  ���͵���������ֻ��copy���˷���buf��
// ============================================================================
u32 Slave_ISBUS_SendPkg(struct Slave_FunctionSocket  *Slave_FunctionSocket, u8 dst,u8 src, u8 *buf, u16 len)
{
    struct Slave_ISBUSPort *Port;
    u8 *SendBuf;
    u16 SendLen;
    s32 Completed;
    if(Slave_FunctionSocket == NULL)
        return 0;
    if(len > Slave_FunctionSocket->ProtocolSendLen)
        return 0;
    Port = Slave_FunctionSocket->CommPort;
    Port->PortSendLen = len;    //����Э��ͷ
    SendBuf = Port->SendPkgBuf;
    SendBuf[CN_OFF_START]   = 0xEB;
    SendBuf[CN_OFF_DST]     = dst;
    SendBuf[CN_OFF_PROTO]   = Slave_FunctionSocket->Protocol;
    SendBuf[CN_OFF_SRC]     = src;
    SendBuf[CN_OFF_LEN]     = len;
    SendBuf[CN_OFF_CHKSUM]  = 0xEB + dst + Slave_FunctionSocket->Protocol
                              + src + (len & 0xff) + (len >> 8);
    SendLen = len + sizeof(struct Slave_ISBUSProtocol);
    memcpy(SendBuf + sizeof(struct Slave_ISBUSProtocol), buf, len);

//    Completed = Driver_WriteDevice(Port->SerialDevice, SendBuf, SendLen,
//                                   0, CN_BLOCK_BUFFER, 0);
    Completed = DevWrite(Port->SerialDevice, SendBuf, SendLen,
                                                           0,0);
    if(Completed != -1)
        Port->SendP = Completed;

    return len;
}

// ============================================================================
// �������ܣ����ôӻ���ַ��
// ���������Addr��������ַ��8λ
// ����ֵ�� ��
// ============================================================================
void Slave_ISBUS_SetAddress(u8 Addr)
{
    if(Addr < CN_INS_MULTICAST)  //���ܳ����鲥��ʼ��ַ
        Slave_sg_u8Address = Addr;
}

// ============================================================================
// �������ܣ����ñ������ܵ��鲥��ַ��
// ���������Port�������õ�Ŀ��ͨ�ſ�
//           Addr���鲥��ַ��8λ
// ����ֵ�� ��
// ============================================================================
void Slave_ISBUS_SetMtcAddress(struct Slave_ISBUSPort *Port, u8 Addr)
{
    if((Addr >= CN_INS_MULTICAST) && (Addr != CN_INS_BROADCAST) && (Port != NULL))
        Port->MTC_Address = Addr;
}
