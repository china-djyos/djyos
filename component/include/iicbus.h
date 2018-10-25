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
// �ļ���     ��iicbus.h
// ģ������: IICģ��Ľṹ�弰API����
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 17/06.2014
// =============================================================================

#ifndef __IICBUS_H__
#define __IICBUS_H__
#include "object.h"
#include "lock.h"
//IIC �ײ�Ӳ�������ӿ�
// =============================================================================
// ����: ����дʱ������дʱ��Ĺ���Ϊ��������ַ��д�����洢��ַ��д�������洢��ַ��
//       ��ʱ������жϣ��������üĴ���Ϊ����ģʽ��֮�󽫻ᷢ�������жϣ����жϷ���
//       �����У�ÿ�η���һ�����ֽ�����ֱ��len��������post�ź���iic_bus_semp��������
//       ֹͣʱ��
// ����: SpecificFlag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//      DevAddr,������ַ��ǰ7���أ��ѽ��ڲ���ַ��ռ��bitλ���£��ú����轫�õ�ַ��
//               ��һλ�����������һλ��/д����;
//      MemAddr,�洢���ڲ���ַ�������͵������ϵĵ�ַ���õ�ַδ��������������ַ�ϵ�
//               ����λ��
//      MenAddrLen,�洢���ڲ���ַ�ĳ��ȣ��ֽڵ�λ��δ������������ַ����ı���λ��
//      Length,���յ������������������ݵĵ�����һ�ֽڣ���count-1��ֹͣ����ACK�źţ�
//          �����յ��ֽ���Ϊcountʱ������ֹͣʱ�򣬲��ͷ��ź���iic_semp;
//      IIC_BusSemp,�������ʱ�������ͷŵ��ź��������ͳ����IIC_PortReadʱ������������
//          �����ͷŸ��ź�������������ʱ��
// ����: TRUE��������ʱ��ɹ���FALSEʧ��
// =============================================================================
typedef bool_t (*WriteStartFunc)(ptu32_t  SpecificFlag,u8 DevAddr,\
                                u32 MemAddr,u8 MenAddrLen, u32 Length,\
                                struct SemaphoreLCB *IIC_BusSemp);
// =============================================================================
// ����: ������ʱ��������ʱ��Ĺ���Ϊ��������ַ��д�����洢��ַ��д����������ַ������
//       ��������ַ���������ʱ������жϣ��������üĴ���Ϊ����ģʽ��֮�󽫻ᷢ��
//       ���������жϣ����ж��н����յ������ݵ���IIC_PortWriteд�뻺�壬���յ�len��
//       ���������ݺ��ͷ��ź���iic_semp
// ����: SpecificFlag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//      DevAddr,������ַ��ǰ7���أ��ѽ��ڲ���ַ��ռ��bitλ���£��ú����轫�õ�ַ��
//               ��һλ�����������һλ��/д����;
//     MemAddr,�洢���ڲ���ַ�������͵������ϵĵ�ַ���õ�ַδ��������������ַ�ϵ�
//               ����λ��
//      MenAddrLen,�洢���ڲ���ַ�ĳ��ȣ��ֽڵ�λ��δ������������ַ����ı���λ��
//      Length,���յ������������������ݵĵ�����һ�ֽڣ���count-1��ֹͣ����ACK�źţ�����
//          �յ��ֽ���Ϊcountʱ������ֹͣʱ�򣬲��ͷ��ź���iic_semp;
//     IIC_BusSemp,�����ʱ���������ͷŵ��ź������������ź�����
// ����: TRUE��������ʱ��ɹ���FALSEʧ��
// =============================================================================
typedef bool_t (*ReadStartFunc)(ptu32_t  SpecificFlag,u8 DevAddr,\
                                u32 MemAddr,u8 MemAddrLen, u32 Length,\
                                struct SemaphoreLCB *IIC_BusSemp);
// =============================================================================
// ����: �������ζ�д�ص����������ֶ�д�Ĳ�ֹͬͣʱ�򣬵����ڷ���ʱ����ֱ��ֹͣʱ��
//      ��Ϊ��������ֹͣ�ظ�ACK����ֹͣ��
// ����: SpecificFlag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ��
// ����: ��
// =============================================================================
typedef void (*GenerateEndFunc)(ptu32_t  SpecificFlag);
// =============================================================================
// ���ܣ�IIC���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�IICʱ��Ƶ������
// ������SpecificFlag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
typedef s32 (*IICBusCtrlFunc)      (ptu32_t SpecificFlag,u32 cmd,\
                                ptu32_t data1,ptu32_t data2);
// =============================================================================
// ���ܣ���ѯ��ʽ��дIIC�豸
// ������SpecificFlag,�Ĵ�����ַ
//       DevAddr,�豸��ַ
//       MemAddr,�豸�ڲ���ַ
//       MemAddrLen,�豸�ڲ���ַ����
//       Buf,�洢����
//       Length,���ݳ��ȣ��ֽ�
//       WrRdFlag,��д��ǣ�Ϊ0ʱд��1ʱΪ��
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================
typedef bool_t (*WriteReadPoll)(ptu32_t  SpecificFlag,u8 DevAddr,u32 MemAddr,\
                                u8 MenAddrLen,u8* Buf, u32 Length,u8 WrRdFlag);

//IIC��ʼ�������ṹ��
struct IIC_Param
{
    char                *BusName;                   //�������ƣ���IIC1
    u8                  *IICBuf;                    //���߻�����ָ��
    u32                 IICBufLen;                  //���߻�������С���ֽ�
    ptu32_t             SpecificFlag;               //ָ����ǣ���IIC�Ĵ�����ַ
    WriteReadPoll       pWriteReadPoll;             //��ѯ���ж�δ��ʱʹ��
    WriteStartFunc      pGenerateWriteStart;        //д��������
    ReadStartFunc       pGenerateReadStart;         //����������
    GenerateEndFunc     pGenerateEnd;               //����ͨ��
    IICBusCtrlFunc      pBusCtrl;                   //���ƺ���
};
struct IIC_Device;
struct IIC_CB;

//����BusCtrl()�Ĳ���cmd,0x80������ϵͳ���壬0x80�����û�����
#define CN_IIC_SET_CLK               0  //����ʱ��Ƶ��
#define CN_IIC_DMA_USED              1  //ʹ��dma����
#define CN_IIC_DMA_UNUSED            2  //��ֹdma����
#define CN_IIC_SET_ERROR_POP_EVENT   3  //���������¼�
#define CN_IIC_SET_POLL              4  //ʹ����ѯ��ʽ���ͽ���
#define CN_IIC_SET_INT            5  //ʹ���жϷ�ʽ���ͽ���
#define CN_IIC_OS_USED               0x80

//IIC��д��־
#define CN_IIC_WRITE_FLAG           0x00
#define CN_IIC_READ_FLAG            0x01
//�����������
#define CN_IIC_EXIT_UNKNOW_ERR              -1
#define CN_IIC_EXIT_TIMEOUT                 -2
#define CN_IIC_EXIT_PARAM_ERR               -3
#define CN_IIC_EXIT_FUN_NULL                -4

//���巢�����󵯳��¼�����ֵ
#define CN_IIC_POP_UNKNOW_ERR           (0x01 << 16)        //δ֪����
#define CN_IIC_POP_NO_ACK_ERR           (0x01 << 17)        //δ�յ���������ACK�ź�
#define CN_IIC_POP_MAL_LOST_ERR         (0x01 << 18)        //�ٲö�ʧ


//IICģ��API����
bool_t ModuleInstall_IICBus(void);

struct IIC_CB *IIC_BusAdd(struct IIC_Param *NewIICParam);
bool_t IIC_BusDelete(struct IIC_CB *DelIIC);
struct IIC_CB *IIC_BusFind(const char *BusName);

struct IIC_Device *IIC_DevAdd(const char *BusName , const char *DevName, u8 DevAddr,
                                u8 BitOfMaddrInDaddr, u8 BitOfMaddr);
bool_t IIC_DevDelete(struct IIC_Device *DelDev);
struct IIC_Device *IIC_DevFind(const char *BusName ,const char *DevName);


s32  IIC_Write(struct IIC_Device *DelDev, u32 addr,u8 *buf,u32 len,
                bool_t block_option,u32 timeout);
s32  IIC_Read(struct IIC_Device *DelDev,u32 addr,u8 *buf,u32 len,u32 timeout);
s32  IIC_PortRead( struct IIC_CB *IIC,u8 *buf,u32 len);
s32  IIC_PortWrite(struct IIC_CB *IIC,u8 *buf,u32 len);
s32  IIC_BusCtrl(struct IIC_Device *DelDev,u32 cmd,ptu32_t data1,ptu32_t data2);
s32  IIC_ErrPop(struct IIC_CB *IIC, u32 ErrNo);

#endif /* __IICBUS_H__ */
