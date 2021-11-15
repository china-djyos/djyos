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
//����ģ��:�ѹ���ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:�ṩ�������ڴ�������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __HEAP_IN_H__
#define __HEAP_IN_H__

#include "stdint.h"
#include "board-config.h"
#include "lock.h"
#include "component_config_heap.h"

#ifdef __cplusplus
extern "C" {
#endif

struct HeapCession
{
    struct HeapCession *Next;
    u8   *static_bottom;        //�ѵ�ָ�룬��׼��̬��������
    u8   *heap_bottom;          //�ѵ�ָ�룬�ӿ�������̬��������
    u8   *heap_top;             //�Ѷ�ָ��
    list_t   *last;             //��̬�������һ��������ַָ�롣
#if ((CFG_DYNAMIC_MEM == true))
    u32    PageSize;
    u32    ua_pages_num;        //��ҳ��
    u32    free_pages_num;      //�����ڴ�ҳ��
    u32    ua_block_max;        //����ڴ��ߴ�(�ֽ���)
    u32    ua_free_block_max;   //���е�����ڴ���С(�ֽ���);
    ufast_t     uf_grades;      //�ܽ���

    //����3�����ڳ�ʼ��ʱ�Ӷ��л�����
    //��һ���֣�ÿҳ����һ��16λ�ּ�¼ӵ�б�ҳ�ڴ���¼���ID����������¼������
    //�ͷžֲ��ڴ棬����һ��������ɻ��ҵĲ�����
    //�������ڱ�ʾ�ôη����ҳ��,free�ݴ˲�ѯ���ͷŵ��ֽ���.
    //���__Djy_EventExit��__Djy_EventFinal���������¼�����δ�ͷŵľֲ��ڴ�飬����
    //index_event_id �ͷ�֮����ʽ����:
    //1.��ҳ�ֲ��ڴ�:�¼�id,
    //2.��ҳȫ���ڴ�:�¼�id + CN_EVTT_ID_MASK,
    //3.˫ҳ�ֲ��ڴ�:CN_MEM_DOUBLE_PAGE_LOCAL+�¼�id
    //4.��ҳ�ֲ��ڴ�:CN_MEM_MANY_PAGE_LOCAL+�¼�id+�׺�
    //5.˫ҳȫ���ڴ�:CN_MEM_DOUBLE_PAGE_GLOBAL+�¼�id,
    //6.˫(��)ҳȫ���ڴ�:CN_MEM_MANY_PAGE_GLOBAL+�¼�id+�׺�.
    //7.�����ڴ�:CN_MEM_FREE_PAGE
    u16     *index_event_id;
    ufast_t *p_classes;     //�ڶ�����,���׿���н�����������
    ucpu_t  ***ppp_bitmap;  //��������,ָ��"���н�����λͼָ���ָ��",
                                //ÿ��һ��,ָ��ý׿��н������ĵ�һ��ָ��
#endif   //for #if ((CFG_DYNAMIC_MEM == true))
};

struct HeapCB
{
    struct HeapCB *PreHeap;
    struct HeapCB *NextHeap;
    struct HeapCession *Cession;
    u32  CessionNum;            //��ɸ�heap��session������
    u32  AlignSize;             //��heap��Ҫ��Ķ���ߴ磬��Щ������dma��������
                                //;���ڸ�heap�Ϸ�����ڴ棬������Ķ���ߴ硣
    u32  HeapProperty;          //0=ͨ�ö�,1=ר�ö�
    ptu32_t FreeMin;            //���ѿ����ڴ����Сֵ
    ptu32_t SyncFreeMin;        //�ܿ����ڴ����ֵ�½�ʱ�����ѿ����ڴ�����
    char *HeapName;
#if ((CFG_DYNAMIC_MEM == true))
    struct MutexLCB HeapMutex;
    // �ȴ������ڴ���¼�����,˫��ѭ������
    // �����Ƿ�֧�ֶ�̬����,��׼��̬�����ڼ�,ָ����Ч.
    // ��ר�öѵ�ָ����Ч,ͨ�öѽ��þ�̬����s_ptMemSync���.
    struct EventECB *mem_sync;
#endif   //for #if ((CFG_DYNAMIC_MEM == true))
} ;

//���ڰ��¼�����ľֲ��ڴ�鴮����������y_event_done����ʱ���ݴ˻����ڴ档
struct MemRecord
{
    struct MemRecord *next;
    struct EventECB *owner;
    u8 *address;
};


#ifdef __cplusplus
}
#endif

#endif //__HEAP_IN_H__


