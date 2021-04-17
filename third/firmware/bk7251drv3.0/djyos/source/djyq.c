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
#include <djyq.h>
#include <stddef.h>

// queue
void djyq_init(struct djyq_t *q)
{
    q->head = q->tail = NULL;
}

void djyq_append(struct djyq_t *q, struct djyq_node_t *n)
{
    n->next = NULL;
    if (NULL == q->tail)
    {
        q->head = q->tail = n;
    }
    else
    {
        q->tail->next = n;
        q->tail = n;
    }
}

void djyq_remove(struct djyq_t *q, struct djyq_node_t *n)
{
    struct djyq_node_t *head = q->head;

    if (head == n)
    {
        q->head = head->next;
        if (NULL == q->head)
        {
            q->tail = NULL;
        }
    }
    else if (head != NULL)
    {
        while (head->next != NULL)
        {
            if (head->next == n)
            {
                head->next = head->next->next;
                if (NULL == head->next)
                {
                    q->tail = head;
                }
                break;
            }
            head = head->next;
        }
    }
}

void djyq_enqueue(struct djyq_t *q, struct djyq_node_t *n)
{
    struct djyq_node_t *tmp = q->head;

    n->next = NULL;
    if (NULL == tmp)
    {
        q->head = q->tail = n;
    }
    else if (tmp->addr >= n->addr)
    {
        n->next = tmp;
        q->head = n;
    }
    else
    {
        while (tmp->next != NULL)
        {
            if (tmp->next->addr >= n->addr)
            {
                n->next = tmp->next;
                tmp->next = n;
                break;
            }
            tmp = tmp->next;
        }
        // insert last
        tmp->next = n;
    }
}

struct djyq_node_t* djyq_dequeue(struct djyq_t *q)
{
    struct djyq_node_t *head = q->head;
    if (q->head != NULL)
    {
        q->head = q->head->next;
        if (NULL == q->head)
        {
            q->tail = NULL;
        }
    }
    return head;
}

