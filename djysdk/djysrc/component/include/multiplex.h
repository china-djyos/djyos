//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��: multiplexģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:ʵ�ֶ�·���õĹ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-06-24
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __MULTIPLEX_H__
#define __MULTIPLEX_H__
#include "lock.h"
#ifdef __cplusplus
extern "C" {
#endif

#define CN_MULTIPLEX_MAX_SETS      10
#define CN_MULTIPLEX_MAX_OBJECTS   1000

#define CN_MULTIPLEX_SUCCESS   0   //���óɹ�
#define CN_MULTIPLEX_INVALID   1   //����֧��Multiplex����
#define CN_MULTIPLEX_ERROR     2   //������Ȼ֧��Multiplex�����ܾ��˱��ε���

//struct MultiplexObjectCB��SensingBit��Ա�Ķ��壬��24��Sensing bit
#define CN_MULTIPLEX_SENSINGBIT_READ      (1<<0)  //����ɶ�
#define CN_MULTIPLEX_SENSINGBIT_WRITE     (1<<1)  //�����д
#define CN_MULTIPLEX_SENSINGBIT_ERROR     (1<<2)  //�������

//SensingBit��ģʽ
#define CN_MULTIPLEX_SENSINGBIT_MODE      (0xff<<24) //ģʽλ��λƫ��
#define CN_MULTIPLEX_SENSINGBIT_LT         0      //��ƽ������Level��
#define CN_MULTIPLEX_SENSINGBIT_ET        (1<<30) //���ش�����Edge��
#define CN_MULTIPLEX_SENSINGBIT_AND        0      //SensingBit��ȫ����λ�Ŵ���
#define CN_MULTIPLEX_SENSINGBIT_OR        (1<<31) //SensingBit����һ����λ�ʹ���


struct MultiplexSetsCB;
struct MultiplexObjectCB;

struct MultiplexSetsCB
{
    struct MultiplexObjectCB *ObjectQ;   // ָ��δ�����Ķ�������
    struct MultiplexObjectCB *ActiveQ;   // ָ���Ѵ����Ķ�������
    u32 ObjectSum;          // �����а����Ķ�������
    u32 ActiveLevel;        // ����ˮƽ�����������������ﵽActiveLevel��������
                            // ���󼯣�����ObjectSum ��ʾȫ����
    u32 Actived;            // �Ѿ������Ķ�����
    bool_t SetsActived;     // �����Ƿ��Ѿ�������
    struct SemaphoreLCB Lock;       // �ȴ�Sets�������õ�����
};

struct MultiplexObjectCB
{
    struct MultiplexObjectCB *NextObject;//����˫����������������һ��MultiplexSets
                                         //�����Ķ��object
    struct MultiplexObjectCB *PreObject;
    struct MultiplexObjectCB *NextSets;  //����������������һ��object�����
                                         //MultiplexSets���������
    struct MultiplexSetsCB *MySets;      //ָ�������ƿ�
    ptu32_t ObjectID;                    //��MultiplexSets�ȴ��Ķ���
    u32 PendingBit;                      //�������Ѿ�������bit
    u32 SensingBit;                      //����λ��־����24bit
                                         //bit24~31��ʾ����λ������ͣ��μ�
                                         //CN_MULTIPLEX_SENSINGBIT_MODE����
};

ptu32_t ModuleInstall_Multiplex(ptu32_t para);
struct MultiplexSetsCB *Multiplex_Create(u32 ActiveLevel);
bool_t Multiplex_AddObject(struct MultiplexSetsCB  *Sets,
                    struct MultiplexObjectCB **ObjectHead,
                    u32 ObjectStatus,ptu32_t ObjectID, u32 SensingBit);
bool_t Multiplex_DelObject(struct MultiplexSetsCB  *Sets,
               struct MultiplexObjectCB **ObjectHead);
bool_t Multiplex_Set(struct MultiplexObjectCB *ObjectHead,u32 Status);
ptu32_t Multiplex_Wait( struct MultiplexSetsCB *Sets,u32 *Status,u32 Timeout);

#ifdef __cplusplus
}
#endif

#endif //__MULTIPLEX_H__

