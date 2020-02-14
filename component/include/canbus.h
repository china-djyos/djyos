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
// �ļ���     ��canbus.h
// ģ������: CAN HAL����㣬����������·��������֮�䡣
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 04/09.2018
// =============================================================================
#ifndef __CAN_BUS_H__
#define __CAN_BUS_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdio.h>

// CAN���߳��ò�����.
typedef enum {
    EN_CAN_BUS_BAUD_5K = 5,                     //    5 Kbps (0)
    EN_CAN_BUS_BAUD_10K=10,                        //   10 Kbps (1)
    EN_CAN_BUS_BAUD_20K=20,                        //   20 Kbps (2)
    EN_CAN_BUS_BAUD_40K=40,                        //   40 Kbps (3)
    EN_CAN_BUS_BAUD_50K=50,                        //   50 Kbps (4)
    EN_CAN_BUS_BAUD_80K=80,                        //   80 Kbps (5)
    EN_CAN_BUS_BAUD_100K=100,                     //   100 Kbps (6)
    EN_CAN_BUS_BAUD_125K=125,                     //   125 Kbps (7)
    EN_CAN_BUS_BAUD_200K=200,                     //   200 Kbps (8)
    EN_CAN_BUS_BAUD_250K=250,                     //   250 Kbps (9)
    EN_CAN_BUS_BAUD_400K=400,                     //   400 Kbps (10)
    EN_CAN_BUS_BAUD_500K=500,                    //    500 Kbps (11)
    EN_CAN_BUS_BAUD_666K=666,                    //    660 Kbps (12)
    EN_CAN_BUS_BAUD_800K=800,                    //    800 Kbps (13)
    EN_CAN_BUS_BAUD_1000K=1000                   //  1000 Kbps (14)
}CanBaudRateDef;


//CAN���Ʋ�����
typedef enum CAN_CTRL_CMD
{
    EN_CAN_BUS_RESET=0,                   //��λCAN������
    EN_CAN_BUS_SET_BAUDRATE,              //����CANͨ�Ų�����
    EN_CAN_BUS_GET_BAUDRATE,              //��ȡCANͨ�Ų�����
    EN_CAN_BUS_SET_FILTER,                //����CAN�˲�����
    EN_CAN_BUS_GET_FILTER,                //��ȡCAN�˲�����
}CanCtrlCmd;

//CAN����ģʽ
typedef enum CAN_SEND_MODE
{
    EN_CAN_BUS_SEND_ASYN_MODE=0,    //�첽����
    EN_CAN_BUS_SEND_SYN_MODE,       //ͬ������
}CanSendMode;

//CAN��������
typedef enum
{
    EN_CAN_BUS_STD=(unsigned char)(0x00),
    EN_CAN_BUS_RTR=(unsigned char)(0x40),
    EN_CAN_BUS_EXD=(unsigned char)(0x80)
}CanFrameType;

//CAN���ߴ�������
typedef enum
{
    EN_CAN_BUS_BUS_OFF_ERR=0,           //���߹ر�
    EN_CAN_BUS_CRC_ERR,                 //CRC����
    EN_CAN_BUS_BIT_ERR,                 //λ����,��������λ���������λ����
    EN_CAN_BUS_STUFF_ERR,               //������
    EN_CAN_BUS_FORMAT_ERR,              //֡��ʽ����
    EN_CAN_BUS_ACK_ERR,                 //ȷ�ϴ���
    EN_CAN_BUS_LOSS_ARB_ERR,            //�ٲ�ʧ��
    EN_CAN_BUS_SEND_BAD,                //����ʧ��
    EN_CAN_BUS_RST_FAILED,              //��λʧ��
}CanErrType;


//CAN���ߴ���ͳ��
typedef struct
{
    uint64_t BusOffErrCnt;    //���߹رմ��������
    uint64_t CrcErrCnt;       //CRC���������
    uint64_t BitErrCnt;       //λ���������
    uint64_t StuErrCnt;       //�����������
    uint64_t FormatErrCnt;    //��ʽ���������
    uint64_t ACKErrCnt;       //ACK���������
    uint64_t LossArbCnt;      //�ٲ�ʧ��������
}CanErrStatDef;


//CANBUS����ͳ��
typedef struct
{
    uint64_t AppSndCnt;          //CANBUS�ϲ㷢��CAN����֡��
    uint64_t HardSndCnt;         //CAN�������ɹ�����CAN����֡��
    uint64_t HardRcvCnt;         //CAN����������CAN����֡��
    uint64_t AppRcvCnt;          //CANBUS�ϲ����CAN����֡��
    uint64_t SendSempTimeoutCnt; //CANBUS���ͱ���ʱpend�ź�����ʱ
    uint64_t RecvSempTimeoutCnt; //CANBUS���ձ���ʱpend�ź�����ʱ
    uint64_t LostSendFrameCnt;   //CANBUS���ڷ��ͻ���������ʧCAN����֡��
    uint64_t LostRecvFrameCnt;   //CANBUS���ڽ��ջ���������ʧCAN����֡��
    uint64_t SndPkgBadCnt;       //CAN����������ʧ�ܴ���������ָCAN������δ�ɹ������ķ��͵�����
    uint64_t RstFailedCnt;       //CAN��������λʧ�ܴ���
    CanErrStatDef CanErrStat;    //CAN�������
}CanStatDef;


#pragma pack(1)
typedef struct CAN_Frame
{
    CanFrameType Type;  //CAN֡�������ͣ�CANBUS֧��CAN2.0A��CAN2.0B����֧�ֱ�׼֡����չ֡��Զ��֡���ࡣ
    uint8_t DLC;        //CAN֡��Ч�����ֽ���.
    uint32_t Id;        //CAN֡Id������Ǳ�׼֡����IdΪ11λ��������չ֡��Ϊ29λ�������Ǳ�׼֡������չ֡����LSB��ʽ��
    uint8_t Data[8];    //CAN֡����Ч���ݻ�������һ֡CAN���������Ч�ֽ���Ϊ8.
}CanFarmeDef;
#pragma pack()

struct CANBusCB;
// =============================================================================
// ���ܣ�CAN��������һ֡CAN���ķ��͵�CAN�����ϡ�
// ������Frame,CAN֡�ṹ��ָ�롣
// ���أ��ɹ������򷵻�true��ʧ���򷵻�false��
// =============================================================================
typedef bool_t  (*fnCanWrite)(struct CANBusCB * CANBus,CanFarmeDef *Frame);

// =============================================================================
// ���ܣ�CAN���������Ʋ�����������������CANͨ�Ų����ʣ���λ���˲������ȹ��ܡ�
// ������ctrlcmd,CAN���Ʋ����룬����ο�enum CAN_CTRL_CMD;
//     param1,CAN���Ʋ�������1��
//     param2,CAN���Ʋ�������2;
// ���أ��ɹ������򷵻�true��ʧ���򷵻�false��
// =============================================================================
typedef bool_t (*fnCanCtrl)(struct CANBusCB * CANBus,uint8_t ctrlcmd,ptu32_t param1,ptu32_t param2);

struct CanDev
{
   char *ChipName;           //оƬ���ƣ���stm32ϵ��Ƭ������CAN������������Ϊ��MCAN1/MCAN2/MCAN3��.
   CanBaudRateDef BaudRate;  //ͨ�Ų�����
   fnCanWrite CanWrite;      //CAN�����������з���һ֡����
   fnCanCtrl  CanCtrl;       //CAN���������ƺ�������������CANͨ�Ų����ʣ�CAN�˲�������.
   ptu32_t PrivateTag;       //���Ա�ǣ��ɾ�����������
};


//CAN���߿��ƿ�ṹ��,��ģ��ɼ�
struct CANBusCB
{
    struct Object          *CAN_BusNode;      //������Դ�ڵ�
    struct SemaphoreLCB    *CAN_BusSemp;     //CANBUS�����ź���,���ڿ��Ʋ�������
    char                   *BusName;         //CANBUS���ƣ���CAN���������Ʊ���һ��
    CanBaudRateDef         BaudRate;         //ͨ�Ų�����
    fnCanWrite             CanWrite;         //CANBUS����֡���Ӻ������ײ�CAN��������ע��
    fnCanCtrl              CanCtrl;          //CANBUS���ƹ��Ӻ������ײ�CAN��������ע��
    struct RingBuf         *RcvRing;         //CANBUS���ջ��λ�����
    CanStatDef             CanStat;          //CANBUSͳ��
    ptu32_t                PrivateTag;       //���Ա�ǣ��ɾ�����������
};


//��������
 // =============================================================================
 // ���ܣ���CAN�������ͽ��ҽӵ�DjyBus����㡣
 // ������para,��ʵ�����塣
 // ���أ����ؽ�������Դ���ָ�룬ʧ��ʱ����NULL��
 // =============================================================================
struct Object * ModuleInstall_CANBus();
// =============================================================================
// ���ܣ����CAN���ߵ�CAN�������ͽڵ��£�ÿ��CAN��������Ӧһ��CAN���ߡ�
// ������pNewCanDev,CAN Dev�ṹ��ָ�롣
// ���أ�������ӵ�CANBUS���ƿ�ṹ��ָ�룬ʧ��ʱ����NULL��
// =============================================================================
struct CANBusCB *CAN_BusAdd(struct CanDev *pNewCanDev);
// =============================================================================
// ���ܣ����CAN���ߵ�CAN�������ͽڵ��£�ÿ��CAN��������Ӧһ��CAN���ߣ�CAN���߿��ƿ�
//       �ɵ��÷��ṩ��
// ������pNewCanDev,CAN Dev�ṹ��ָ�룻
//       NewCAN,CANBUS���ƿ����ݽṹ�塣
// ���أ�������ӵ�CANBUS���ƿ�ṹ��ָ�룬ʧ��ʱ����NULL��
// =============================================================================
struct CANBusCB * CAN_BusAdd_s(struct CanDev *pNewCanDev,\
                                   struct CANBusCB *NewCAN);
// =============================================================================
// ���ܣ�ɾ����CAN���������ϵ�CANBUSָ����㡣
// ������DelCAN,CANBUS���ƿ�ָ�롣
// ���أ�true,ɾ���ɹ�;false,ɾ��ʧ��
// =============================================================================
bool_t CAN_BusDelete(struct CANBusCB *DelCAN);
// =============================================================================
// ���ܣ�ɾ����CAN���������ϵ�CANBUSָ����㡣
// ������DelCAN,CANBUS���ƿ�ָ�롣
// ���أ�true,ɾ���ɹ�;false,ɾ��ʧ��
// =============================================================================
bool_t CAN_BusDelete_s(struct CANBusCB *DelCAN);
// =============================================================================
// ���ܣ�ͨ���������Ʋ���CANBUS�����
// ������BusName,CANBUS��������;
// ���أ����ɹ��ҵ��򷵻�CANBUS�ľ��(һ���޷��ŵ�32λ��)��ʧ���򷵻�NULL.
// =============================================================================
struct CANBusCB* CAN_BusFind(char *BusName);
// =============================================================================
// ���ܣ�CANBUSдһ֡CAN���ĺ�����ͨ���ú������ɽ���Ҫ���͵�һ֡CAN����ͨ��ָ��CANBUS(ָ����CAN������)���͵�CAN�����ϡ�
// ������CANBus,CANBUS���ƿ�ṹ��ָ��;
//     Frame,CAN֡�ṹ��ָ��;
// ���أ����ͳɹ��򷵻�true��������ʧ�ܣ��򷵻�false.
// =============================================================================
bool_t CAN_BusSendFrame(struct CANBusCB * CANBus,CanFarmeDef *Frame,CanSendMode Mode);
// =============================================================================
// ���ܣ�CANBUS��������ͨ���ú������ɶ�ȡָ��CANBUS(ָ��CAN������)�Ͻ��յ�һ֡CAN���ģ��ײ�CAN���������յ����ݺ��
//     �����һ�����λ�������.
// ������CANBus,CANBUS���ƿ�ṹ��ָ��;
//     Frame,CAN֡�ṹ��ָ��;
// ���أ����سɹ��������ݳ��ȣ�������ʧ�ܣ��򷵻�-1.
// =============================================================================
bool_t CAN_BusRecvFrame(struct CANBusCB * CANBus,CanFarmeDef *Frame);
// =============================================================================
// ���ܣ�CANBUS���ƺ�����ͨ���ú����ɶ�ָ��CANBUS(ָ��CAN������)���в������û���������������μ�canbus.h��
//     CAN_CTRL_CMD���塣
// ������CANBus,CANBUS���ƿ�ṹ��ָ��;
//     CtrlCmd,�����룬����μ�canbus.h��CAN_CTRL_CMD����;
//     param,�����������;
//     timeout����ʱʱ�䣬ָ�ȴ����߿��е�ʱ��.
// ���أ��ɹ������򷵻�true,ʧ���򷵻�false.
// =============================================================================
bool_t CAN_BusCtrl(struct CANBusCB * CANBus,uint8_t CtrlCmd,ptu32_t param1,ptu32_t param2);



bool_t CAN_BusAddRcvFrameToRing(struct CANBusCB * CANBus,CanFarmeDef *Frame);

#ifdef __cplusplus
}
#endif
#endif
