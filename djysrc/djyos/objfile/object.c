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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
//����ģ��: �������
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ��������ӡ��ƶ���ɾ������������ڵ�ķ���
//����˵��:
//�޶���ʷ:
//4. ����:20130220
//   ����:  ������.
//   �°汾�ţ�v1.3.0
//   �޸�˵��:
//      1.������rsc�������ܵ������������⣬bug������ë���ύ�ġ�
//3. ����:20090518
//   ����:  ������.
//   �°汾�ţ�v1.2.0
//   �޸�˵��:
//      1.�ڸ����ϣ��ֿ��������Ͷ������������Ҳ��Ӧ�ص�����
//      2.����rsc_get_root������ԭrsc_get_root����Ϊrsc_get_tree���Ҳ��ټ�ְ
//      3.������rsc_get_root��ȡ��1.1.0�汾��"�޸�3"
//      4.��ȫ��NULL�������
//2. ����:20090131
//   ����:  ������.
//   �°汾�ţ�v1.1.0
//   �޸�˵��:
//      1.���˸��ܸ��ڵ㣬�����ģ���ʼ������
//      2.���ź��������������ʰ�ȫ
//      3.�Ľ��˱���������������NULL���������������
//      4.������һЩbug
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           �������㷨˵��
// ��ͬ���ڵ�����˫��ѭ������
// ÿ���ڵ��childָ��ֱ��ָ��Ľڵ��Ϊ�ýڵ�����ڶ��еĶ���ͷ������previous�ڵ�
// �Ƕ���β��
// ���нڵ��Parentָ���ָ�򸸽ڵ�

// �ر���ʾ��������еĲ������ʰ�ȫ�ɶ������ģ�鸺�𣬶���ڵ�����Ĳ�������
//          ������ʹ���߸���
// ��������޸Ķ���ʹ��ԭ�Ӳ�����֤������걸��
// ������Ķ�������ʹ���κΰ�ȫ���ƣ��ɶ�������ģ������֤�������İ�ȫ��
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <int.h>
#include <pool.h>
#include <list.h>
#include <object.h>
#include <systime.h>
#include <lock.h>
#include "__object.h"
#include "dbug.h"
// OBJ˫�������ʼ��
#define __OBJ_LIST_INIT(l)              (l)->next = (l)->prev = (l)

// ��˫��ѭ�������lλ�ú������ڵ�o
#define __OBJ_LIST_INS_AFTER(l, o)      \
                                        do \
                                        {  \
                                            (l)->next->prev = (o); \
                                            (o)->next = (l)->next; \
                                            (l)->next = (o); \
                                            (o)->prev = (l); \
                                        }while(0)

// ��˫��ѭ�������lλ��ǰ�����ڵ�o
#define __OBJ_LIST_INS_BEFORE(l, o)     \
                                        do \
                                        { \
                                            (l)->prev->next = (o); \
                                            (o)->prev = (l)->prev; \
                                            (l)->prev = (o); \
                                            (o)->next = (l); \
                                        }while(0)

// ��˫��ѭ���������Ƴ�, NOTE: ����δ�������¼�����Ĵ���
#define __OBJ_LIST_REMOVE(l)            \
                                        do \
                                        { \
                                            (l)->next->prev = (l)->prev; \
                                            (l)->prev->next = (l)->next; \
                                            (l)->next = (l)->prev = (l); \
                                        }while(0)

// �Ƿ������һ�������Ա��
#define __OBJ_LIST_LAST(l)              (((l)->next == (l)) ? 1 : 0)

// ѭ����ѯ�ֵ�����
#define __OBJ_LIST_FOR_EACH_BROTHERS(l, h)            \
                                        for((l) = (h)->next; (l) != (h); (l) = (l)->next)

#define DEFAULT_LOCK_TIMEOUT        (360*1000*mS)   // һ��Сʱ������δFOREVER��Ϊ�˷�ֹ�߼�����
#define BASIC_OBJECTS               32
static struct MemCellPool basicObjPool;
static struct Object basicObjSpace[BASIC_OBJECTS];
static struct MutexLCB * pObjMutex;
static struct Object *pRootObj, *pCurObj;

ptu32_t OBJ_DefaultOps(u32 dwCMD, ptu32_t context, ptu32_t args, ...);

struct __statics
{
    u32 news;
    u32 fress;
    u32 alives;
}objStatics;

// ============================================================================
// ���ܣ���ʼ�������
// ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��POOL�������ǿ����Զ���չ
// ============================================================================
s32 __OBJ_InitBasicPool(void)
{
    if(Mb_CreatePool_s(&basicObjPool, basicObjSpace,
            BASIC_OBJECTS, sizeof(struct Object), 16, 16384, "object"))
        return (0);

    return (-1);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��TODO: INLINE
// ============================================================================
struct Object *__OBJ_New(void)
{
    struct Object *obj;

    obj = (struct Object*)Mb_Malloc(&basicObjPool, 0);
    objStatics.news++;
    objStatics.alives++;
    return (obj);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��INLINE
// ============================================================================
inline void __OBJ_Free(struct Object *pObj)
{
    Mb_Free(&basicObjPool, pObj);
    objStatics.fress++;
    objStatics.alives--;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��INLINE
// ============================================================================
inline struct Object *__OBJ_Child(struct Object *pObj)
{
    return (pObj->child);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��INLINE
// ============================================================================
inline struct Object *__OBJ_Parent(struct Object* pObj)
{
    return (pObj->parent);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��INLINE
// ============================================================================
inline struct Object *__OBJ_Next(struct Object *pObj)
{
    return (pObj->next);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��INLINE
// ============================================================================
inline struct Object *__OBJ_Prev(struct Object *pObj)
{
    return (pObj->prev);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��INLINE
// ============================================================================
inline void __OBJ_SetRepresent(struct Object *pObj, ptu32_t new)
{
    pObj->represent = new;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��INLINE
// ============================================================================
inline s32 __OBJ_SetMountPoint(struct Object *pObj)
{
    if(!pObj->reload)
        pObj->reload = (struct Object *)(-1);

    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ��ɹ���0����
// ��ע��INLINE
// ============================================================================
inline s32 __OBJ_ClearMountPoint(struct Object *pObj)
{
    if(pObj->reload && (-1 == (s32)pObj->reload))
        pObj->reload = (struct Object *)(0);

    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��INLINE
// ============================================================================
inline ptu32_t __OBJ_Represent(struct Object *pObj)
{
    return (pObj->represent);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��INLINE
// ============================================================================
inline fnObjOps __OBJ_Ops(struct Object *pObj)
{
    return (pObj->ops);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��INLINE
// ============================================================================
inline void __OBJ_Init(struct Object *pObj)
{
    memset(pObj, 0x0, sizeof(*pObj));
    dListInit(&pObj->port);
}

// ============================================================================
// ���ܣ���ʼ������ϵͳ
// ��������
// ���أ��ɹ���1����ʧ�ܣ�0����
// ��ע��
// ============================================================================
s32 __OBJ_InitSystem(void)
{
    s32 res;

    pObjMutex = Lock_MutexCreate("object");
    if(!pObjMutex)
        return (-1);

    res = __OBJ_InitBasicPool();
    if(res)
    {
        Lock_MutexDelete(pObjMutex);
        return (-1);
    }

    pRootObj = __OBJ_New();
    if(!pRootObj)
    {
        Lock_MutexDelete(pObjMutex);
        return (-1);
    }

    pCurObj = pRootObj;
    __OBJ_Init(pRootObj);
    __OBJ_LIST_INIT(pRootObj);
    __OBJ_SetName(pRootObj, "");
    pRootObj->ops = (fnObjOps)OBJ_DefaultOps;

    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
inline s32 __OBJ_Del(struct Object *pObj)
{
    if(pObj->next == pObj)   //˵���ýڵ�û���ֵܽڵ�.
    {
        pObj->parent->child = NULL;
    }
    else
    {
        if(pObj->parent->child == pObj)
            pObj->parent->child = pObj->next; // ˵���ýڵ��Ƕ���ͷ�ڵ�,��Ҫ�ı����ͷ�ڵ�

        __OBJ_LIST_REMOVE(pObj);
    }

    return (0);
}

// ============================================================================
// ���ܣ����������
// ������pObj -- ����
// ���أ��������ͣ�
// ��ע��
// ============================================================================
s32 __OBJ_Type(struct Object *pObj)
{
    if(pObj->reload)
        return (GROUP_POINT);

    if(pObj->child)
    {
        return (DIR_POINT); // DIR_POINT;
    }
    else // û���ӽڵ�
    {
        if(dListIsEmpty(&(pObj->port))) // û��������
        {
            return (TEMPORARY_POINT);
        }
        else
        {
            extern s32 isdirectoryo(struct Object *pObj);
            if(isdirectoryo(pObj))
                return (DIR_POINT);
        }
    }

    return (REG_POINT);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��INLINE
// ============================================================================
inline const char *__OBJ_Name(struct Object *pObj)
{
    return((const char*)(pObj->name));
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��INLINE
// ============================================================================
inline void __OBJ_SetName(struct Object *pObj, const char *pName)
{
    pObj->name = (char*)pName;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��INLINE
// ============================================================================
inline void __OBJ_SetOps(struct Object *pObj, fnObjOps ops)
{
    pObj->ops = ops;
}

// ============================================================================
// ���ܣ���һ����Ա������
// ������pObj -- ����
//      pNew -- �µ�����Ա��
// ���أ��ɹ���0����
// ��ע��
// ============================================================================
inline s32 __OBJ_Link(struct Object *pObj, list_t *pNew)
{
    dListInsertAfter(&pObj->port, pNew);
    return (0);
}

// ============================================================================
// ���ܣ����������˵ĳ�Ա����
// ������pObj -- ����
//      dwLinks -- �������ޣ����Ｋ�޾Ͳ���ͳ�ƣ�
// ���أ�����dwLinks,�򷵻أ�-1��,С�ڵ���dwLinks���򷵻�ʵ��link��
// ��ע��TODO:����
// ============================================================================
inline s32 __OBJ_LinksCheck(struct Object *pObj, u32 dwLinks)
{
    list_t *head = &pObj->port;
    list_t *cur;
    u32 link;

    cur = dListGetAfter(head);
    if(cur == head)
        return (0);

    for(link = 0; link < dwLinks; link++)
    {
        cur = dListGetAfter(cur);
        if(cur == head)
        {
            break;
        }
    }

    if((++link) <= dwLinks)
        return (link);

    return (-1);
}

// ============================================================================
// ���ܣ�������뱣����������ɾ����
// ������pObj -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��INLINE
// ============================================================================
inline s32 __OBJ_Protect(struct Object *pObj)
{
    if(FALSE == Lock_MutexPend(pObjMutex, DEFAULT_LOCK_TIMEOUT))
        return (-1);

    if(pObj->duty != CN_LIMIT_UINT32)
    {
        pObj->duty++;
        Lock_MutexPost(pObjMutex);
        return (0);
    }

    Lock_MutexPost(pObjMutex);
    return (-1);
}

// ============================================================================
// ���ܣ���ֹ��������
// ������pObj -- ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��INLINE����Ҫɾ������ִ�У�
// ============================================================================
inline s32 __OBJ_Forbid(struct Object *pObj)
{
    if(FALSE == Lock_MutexPend(pObjMutex, DEFAULT_LOCK_TIMEOUT))
        return (-1);

    if(!pObj->duty)
    {
        pObj->duty = CN_LIMIT_UINT32;
        Lock_MutexPost(pObjMutex);
        return (0);
    }

    Lock_MutexPost(pObjMutex);
    return (-1);
}

// ============================================================================
// ���ܣ������Ƿ񱻱����򱻽�ֹ��
// ������pObj -- ����
// ���أ�����������������δ��������0����
// ��ע��INLINE
// ============================================================================
inline s32 __OBJ_IsProtected(struct Object *pObj)
{
    return (pObj->duty);
}

// ============================================================================
// ���ܣ������˳��������ֹ��
// ������pObj -- ����
// ���أ��Ϸ���0�����Ƿ���0����
// ��ע��INLINE
// ============================================================================
inline s32 __OBJ_Release(struct Object *pObj)
{
    if(FALSE == Lock_MutexPend(pObjMutex, DEFAULT_LOCK_TIMEOUT))
        return (-1);

    if(CN_LIMIT_UINT32 != pObj->duty)
        pObj->duty = 0; // forbid
    else if(pObj->duty)
        pObj->duty--;

    Lock_MutexPost(pObjMutex);
    return (0);
}

// ============================================================================
// ���ܣ����������ĺϷ��ԣ�
// ������pName -- ��������
// ���أ��Ϸ���0�����Ƿ���0����
// ��ע��INLINE
// ============================================================================
inline s32 __OBJ_NameSantiy(const char *pName)
{
    u16 i;

    for(i=0; i<CN_OBJ_NAME_LIMIT; i++)
    {
        if(('/' == pName[i])||('\\' == pName[i]))
            return (-1);

        if('\0' == pName[i])
            return (0);
    }

    return (-1);
}

// ============================================================================
// ���ܣ�ƥ��·��
// ������pPath -- ��ƥ���·����
//      pLeft -- ��ȫƥ�䣬ΪNULL������ȫƥ�䣬�򷵻ز�ƥ�䲿�֣���֤����'/'��ͷ����
//      pFull -- ������İ�װ�㿪ʼ��ƥ��·����ͬһ���ļ�ϵͳ�µ�·������
// ���أ�ƥ��·������Ӧ�Ķ���
// ��ע��
// ============================================================================
struct Object *OBJ_MatchPath(const char *pPath, char **pLeft, char **pFull)
{
    char *path = (char*)pPath;
    char *full = NULL;
    char *segment;
    struct Object *current, *head;
    u32 i;

    // ���ӻ���
    if('/' == *path)
    {
        current = OBJ_Root(); // ����·��
    }
    else
    {
        current = OBJ_Current();// �����������·��
    }

    if(GROUP_POINT == __OBJ_Type(current)) // ��װ��
        full = path;

    while(current)
    {
        while('/' == *path)
            path++; // ���˶����'/'

        if('\0' == *path)
            break; // ����·������

        if(('.' == path[0]) && ('.' == path[1]) &&
            (('\\' == path[3]) || ('/' == path[3]) || ('\0' == path[3])))
        {
            if(GROUP_POINT == __OBJ_Type(current))
                full = NULL; // �Ӱ�װ�����ϼ�������ȫ·���߼���ʧЧ��

            current = __OBJ_Parent(current);
            path += 2; // ".."�ַ�����ʾ��һ��
            continue;
        }
        else if(('.' == path[0]) &&
            (('\\' == path[1]) || ('/' == path[1]) || ('\0' == path[1])))
        {
            path += 1; // "."�ַ�����ʾ��ǰ��
            continue;
        }

        head = __OBJ_Child(current); // �����ӽڵ�
        if(!head)
            break;

        segment = (char*)__OBJ_Name(head);
        for(i=0; i<CN_OBJ_NAME_LIMIT; i++) // ��������������
        {
            if('\0' == segment[i])
            {
                if('\0' == path[i])
                {
                    path += i; // pathΪ'\0'
                    current = head;
                    goto __MATCH_DONE; // ƥ�������������������
                }
                else if(('\\' == path[i]) || ('/' == path[i]))
                {
                    path += i;
                    current = head;
                    goto __MATCH_NEXT; // ƥ�䵱ǰ�ڵ㣬����ƥ��һ��һ���ڵ�
                }
                else
                {
                    break; // ���뵱ǰ�ڵ���ڲ�ƥ����ַ�
                }
            }
            else if(segment[i] == path[i])
            {
                continue;
            }
            else
            {
                break; // ���뵱ǰ�ڵ���ڲ�ƥ����ַ�
            }
        }

        __OBJ_LIST_FOR_EACH_BROTHERS(current, head)
        {
            segment = (char*)__OBJ_Name(current);
            for(i=0; i<CN_OBJ_NAME_LIMIT; i++) // ��������������
            {
                if('\0' == segment[i])
                {
                    if('\0' == path[i])
                    {
                        path += i; // pathΪ'\0'
                        goto __MATCH_DONE; // ƥ�������������������
                    }
                    else if(('\\' == path[i]) || ('/' == path[i]))
                    {
                        path += i;
                        goto __MATCH_NEXT; // ƥ�䵱ǰ�ڵ㣬����ƥ��һ��һ���ڵ�
                    }
                    else
                    {
                        break; // ���뵱ǰ�ڵ���ڲ�ƥ����ַ�
                    }
                }
                else if(segment[i] == path[i])
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
        }

        current = __OBJ_Parent(current); // ��ǰ�ڵ���·����ƥ�䣬�˻������Լ��ڵ�
        break; // δ�ҵ��ڵ�

__MATCH_NEXT:

        if(GROUP_POINT == __OBJ_Type(current)) // ƥ��Ľڵ��ǰ�װ��
            full = path;
    }

__MATCH_DONE:
    if(pFull)
        *pFull = full;

    if(pLeft)
    {
        if('\0' == *path)
            *pLeft = NULL;
        else
            *pLeft = path; // �߼����豣֤������'/'��ͷ
    }

    return (current);
}

// ============================================================================
// ���ܣ�������ʱobject·��
// ������pBegin -- ��ʼ����ڵ㣻
//      pPath -- �轨������ʱ·����
// ���أ�Ŀ�����
// ��ע�������½�ʱ���ж��������жϣ�
// ============================================================================
struct Object *OBJ_BufferPath(struct Object *begin, char *path)
{
    char *segst, *name;
    struct Object *current, *find;
    u16 i, seglen;

    if(!path)
        return (begin);

    current = begin;
    while('\0' != *path)
    {
        while('/' == *path)
            path++; // ���˶����'/'

        segst = path;
        while(('/' != *path) && ('\\' != *path) && ('\0' != *path))
            path++; // ����ֱ��'/'��'\0'

        seglen = path - segst;
        name = malloc(seglen + 1);
        if(!name)
        {
            debug_printf("object","memory out(%s). ", __FUNCTION__);
            return (NULL);
        }

        for(i = 0 ; i < seglen; i++)
            name[i] = segst[i];

        name[seglen] = '\0';
        find = OBJ_SearchChild(current, name);
        if(find)
        {
            current = find; // child�Ѿ����ڣ�
            free(name);
        }
        else
        {
            // �����ops��represent��һ���̳й�ϵ�����̳и��ڵ��
            current = OBJ_AddChild(current, current->ops, current->represent, (const char*)name);
            if(!current)
            {
                debug_printf("object","memory out(%s). ", __FUNCTION__);
                return (NULL);
            }
        }
    }

    return (current);
}

// ============================================================================
// ���ܣ��ͷ���ʱobject·��
// ������
// ���أ�
// ��ע��TODO:����
// ============================================================================
void OBJ_UnBufferPath(struct Object *pBegin)
{
    struct Object *parent, *current = pBegin;

    while(1)
    {
        if((TEMPORARY_POINT == __OBJ_Type(current)) &&
           (!__OBJ_IsProtected(current)))
        {
            parent = __OBJ_Parent(current);
            __OBJ_Del(current);
            __OBJ_Free(current);
            current = parent;
        }
        else
        {
            break;
        }
    }
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
struct Object *OBJ_Replace(struct Object *pObj)
{
    struct Object *replacer;
    atom_low_t lock;

    replacer = __OBJ_New();
    if(!replacer)
    {
        debug_printf("object","can not create for replace.");
        return (NULL);
    }

    __OBJ_Init(replacer);

    lock = Int_LowAtomStart();
    replacer->child = NULL;
    replacer->parent = pObj->parent;
    replacer->next = pObj->next;
    replacer->prev = pObj->prev;
    replacer->name = pObj->name;

    pObj->next->prev = replacer;
    pObj->prev->next = replacer;
    if(pObj->parent->child == pObj)
        pObj->parent->child = replacer;

    replacer->reload = pObj;
    Int_LowAtomEnd(lock);

    return (replacer);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
struct Object *OBJ_Restore(struct Object *pObj)
{
    if(-1 == (u32)pObj->reload)
        return (pObj);

    if(pObj->reload->parent != pObj->parent)
    {
        debug_printf("object","replacement changed \"%s\"'s parent relation", pObj->name);
        pObj->reload->parent = pObj->parent;
    }

    if(pObj->reload->next != pObj->next)
    {
        debug_printf("object","replacement changed \"%s\"'s next relation", pObj->name);
        pObj->reload->next = pObj->next;
    }

    if(pObj->reload->prev != pObj->prev)
    {
        debug_printf("object","replacement changed \"%s\"'s previous relation", pObj->name);
        pObj->reload->prev = pObj->prev;
    }

    if(pObj->parent->child == pObj)
        pObj->parent->child = pObj->reload;

    if(dListIsEmpty(&(pObj->port)))
        __OBJ_Free(pObj); // ԭ�ڵ�û�б����õ������ͷ�

    return (pObj->reload);
}

// ============================================================================
// ���ܣ����������ĺϷ��ԣ�
// ������pName -- ��������
// ���أ��Ϸ���0�����Ƿ���0����
// ��ע��
// ============================================================================
s32 OBJ_NameSantiy(const char *pName)
{
    if(pName)
        return (__OBJ_NameSantiy((char*)pName));

    return (-1);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
struct Object * OBJ_Current(void)
{
    return (pCurObj);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void OBJ_SetCurrent(struct Object *pObj)
{
    pCurObj = pObj;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
struct Object * OBJ_Root(void)
{
    return (pRootObj);
}

// ============================================================================
// ���ܣ��Ƿ��Ǹ�����
// ����������
// ���أ��Ϸ���0�����Ƿ���0����
// ��ע��
// ============================================================================
s32 OBJ_IsRoot(struct Object *pObj)
{
    return (pObj == pRootObj);
}

// ============================================================================
// ���ܣ�Ϊ�����ṩһ��Ĭ�ϲ�������������ֻ��Ҫ����һ�����󣬲�����Ҫ��Ϊ�ļ�����
//      ʱ���Ͳ���Ҫ�ṩ Operation ��������ʱ��ϵͳ���Զ��Ѷ����������ָ�����
// ��������û�����塣
// ���أ��̶����� CN_OBJ_CMD_UNSUPPORT
// ��ע��
// ============================================================================
inline ptu32_t OBJ_DefaultOps(u32 dwCMD, ptu32_t context, ptu32_t args, ...)
{
    return (CN_OBJ_CMD_UNSUPPORT);
}
#if 0
// ============================================================================
// ���ܣ�
// ������
// ���أ��ɹ�������ָ�룩��ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
void *OBJ_GetOBJ(char *pPath)
{
    struct Object *start = pRootObj;

    if(('/' != *pPath) && (!g_ptCWD))
        start = g_ptCWD;

    return (OBJ_Search(start, pPath));
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
struct Object *OBJ_Backup(struct Object *pBackup, struct Object *pNew)
{
    if((!pBackup) || (!pNew))
        return (NULL);

    pBackup->prev->next = pNew;
    pBackup->next->prev = pNew;
    pNew->next = pBackup->next;
    pNew->prev = pBackup->prev;
    __OBJ_LIST_INIT(pBackup);
    pNew->reload = pBackup;
    return (pNew);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
struct Object *OBJ_Restore(struct Object *pObj)
{
    struct Object *restore;

    if(!pObj)
        return (NULL);

    restore = pObj->reload;
    pObj->prev->next = restore;
    pObj->next->prev = restore;
    restore->next = pObj->next;
    restore->prev = pObj->prev;
    __OBJ_LIST_INIT(pObj);
    return (pObj);
}
#endif

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 OBJ_SetOps(struct Object *pObj, fnObjOps ops)
{
    if(!pObj)
        return (-1);

    __OBJ_SetOps(pObj, ops);
    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
fnObjOps OBJ_Ops(struct Object *pObj)
{
    if(!pObj)
        return (NULL);

    return (__OBJ_Ops(pObj));
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 OBJ_SetRepresent(struct Object *pObj, ptu32_t represent)
{
    if(!pObj)
        return (-1);

    __OBJ_SetRepresent(pObj, represent);

    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
ptu32_t OBJ_Represent(struct Object *pObj)
{
    if(!pObj)
        return (0);

    return (__OBJ_Represent(pObj));
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
list_t *OBJ_PortList(struct Object *pObj)
{
    if(pObj != NULL)
        return (&pObj->port);
    else
        return  (NULL);
}

#if 0
// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
struct Object *__OBJ_NewChild(struct Object *pParent, fnObjOps ops,
                            ptu32_t represent, const char *pName)
{
    struct Object *child;
    atom_low_t lock;

    if(pParent == NULL)
        return (NULL);

#if 0
    if(NULL != OBJ_SearchChild(pParent, pName))
        return (NULL); // child�Ѿ����ڣ�
#endif

    child = __OBJ_New();
    if(child == NULL)
        return (NULL);

    lock = Int_LowAtomStart();

    child->parent = pParent;
    child->child = NULL;
    if(represent)
        child->represent = represent;
    else
        child->represent = pParent->represent;

    if(ops)
        child->ops = ops;
    else
        child->ops = pParent->ops;

    child->duty = 0;
    child->name = (char *)pName;
    child->reload = NULL;
    dListInit(&child->port);
    if(pParent->child == NULL)
    {
        pParent->child = child;
        __OBJ_LIST_INIT(child);
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(pParent->child, child);
    }

    Int_LowAtomEnd(lock);

    return (child);
}
#endif

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��pName�Ƿ������NULL��
// ============================================================================
struct Object *OBJ_NewSanityChild(struct Object *pParent, fnObjOps ops,
                            ptu32_t represent, const char *pName)
{
    struct Object *child;
    atom_low_t lock;
    char *name;

    if(pParent == NULL)
        return (NULL);

    if(OBJ_NameSantiy(pName))
        return (NULL);

    if(NULL != OBJ_SearchChild(pParent, pName))
        return (NULL); // child�Ѿ����ڣ�

    name = malloc(strlen(pName)+1);
    if(!name)
        return (NULL);

    strcpy(name, pName);

    child = __OBJ_New();
    if(child == NULL)
        return (NULL);

    lock = Int_LowAtomStart();

    child->parent = pParent;
    child->child = NULL;
    if(represent)
        child->represent = represent;
    else
        child->represent = pParent->represent;

    if(ops)
        child->ops = ops;
    else
        child->ops = pParent->ops;

    child->duty = 0;
    child->name = (char *)name;
    child->reload = NULL;
    dListInit(&child->port);
    if(pParent->child == NULL)
    {
        pParent->child = child;
        __OBJ_LIST_INIT(child);
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(pParent->child, child);
    }

    Int_LowAtomEnd(lock);

    return (child);
}

// ============================================================================
// ���ܣ��ڶ������в���һ���ڵ㣬�½ڵ���ԭ�ڵ��previousλ�ã�
// ������pLoc -- �ڴ˽ڵ�ǰ�����ڵ㣻
//      ops -- �������������
//      represent -- �����ʾ��
//      pName -- �������֣���ָ�����ַ����ڴ��������ͷţ��������ǿա�
// ���أ��¼���Ľڵ�
// ��ע��
// ============================================================================
struct Object *OBJ_AddToPrevious(struct Object *pLoc, fnObjOps ops,
                                 ptu32_t represent, const char *pName)
{
    struct Object *new;
    atom_low_t low_atom;

    if(pLoc == NULL)
        return (NULL);

    new = __OBJ_New();
    if(new == NULL)
        return (NULL);

    low_atom = Int_LowAtomStart();
    new->parent = pLoc->parent;
    new->child = NULL;
    new->represent = represent;
    if(ops)
        new->ops = ops;
    else
        new->ops = (fnObjOps)OBJ_DefaultOps;

    new->duty = 0;
    new->name = (char *)pName;
    new->reload = NULL;
    dListInit(&new->port);
    __OBJ_LIST_INS_BEFORE(pLoc, new);
    Int_LowAtomEnd(low_atom);
    return (new);
}

// ============================================================================
// ���ܣ��ڶ�����������һ���ڵ㣬�½ڵ���ԭ�ڵ��nextλ��
// ������pLoc -- �ڴ˽ڵ�������ڵ㣻
//      ops -- �������������
//      represent -- �����ʾ��
//      pName -- �������֣���ָ�����ַ����ڴ��������ͷţ��������ǿա�
// ���أ��¼���Ľڵ�
// ��ע��
// ============================================================================
struct Object *OBJ_AddToNext(struct Object *pLoc, fnObjOps ops,
                             ptu32_t represent, const char *pName)
{
    struct Object *new;
    atom_low_t   low_atom;

    if(pLoc == NULL)
        return (NULL);

    new = __OBJ_New();
    if(new == NULL)
        return (NULL);

    low_atom = Int_LowAtomStart();
    new->parent = pLoc->parent;
    new->child = NULL;
    new->represent = represent;
    if(ops != NULL)
        new->ops = ops;
    else
        new->ops = (fnObjOps)OBJ_DefaultOps;

    new->duty = 0;
    new->name = (char *)pName;
    new->reload = NULL;
    dListInit(&new->port);
    __OBJ_LIST_INS_AFTER(pLoc, new);
    Int_LowAtomEnd(low_atom);
    return (new);
}

// ============================================================================
// ���ܣ���ָ���ڵ�����һ���ӽڵ�,�½ڵ㽫�ڶ���βλ�ã�
// ������pLoc -- �½ڵ�ĸ��ڵ㣻
//      ops -- �������������
//      represent -- �����ʾ��
//      pName -- �������֣���ָ�����ַ����ڴ��������ͷţ��������ǿգ�
// ���أ��¼���Ľڵ㣻
// ��ע��
// ============================================================================
struct Object *OBJ_AddChild(struct Object *pLoc, fnObjOps ops,
                            ptu32_t represent, const char *pName)
{
    struct Object *child;
    atom_low_t low_atom;

    if(pLoc == NULL)
        return (NULL);

    child = __OBJ_New();
    if(child == NULL)
        return (NULL);

    __OBJ_Init(child);

    low_atom = Int_LowAtomStart();
    child->parent = pLoc;
    child->child = NULL;
    child->represent = represent;

    if(ops != NULL)
        child->ops = ops;
    else
        child->ops = (fnObjOps)OBJ_DefaultOps;

    child->duty = 0;
    child->name = (char *)pName;
    child->reload = NULL;
    dListInit(&child->port);
    if(pLoc->child == NULL)
    {
        pLoc->child = child;
        __OBJ_LIST_INIT(child);
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(pLoc->child, child);
    }

    Int_LowAtomEnd(low_atom);

    return (child);
}

// ============================================================================
// ���ܣ���ָ���ڵ�����һ���ӽڵ�,�½ڵ㽫�ڶ���ͷλ�ã�
// ������pLoc -- �½ڵ�ĸ��ڵ㣻
//      ops -- �������������
//      represent -- �����ʾ��
//      pName -- �������֣���ָ�����ַ����ڴ��������ͷţ��������ǿգ�
// �� �أ� �¼���Ľڵ�
// ��ע��
// ============================================================================
struct Object *OBJ_AddChildHead(struct Object *pLoc,fnObjOps ops,
                                ptu32_t represent,const char *pName)
{
    struct Object *child;
    atom_low_t low_atom;

    if(pLoc == NULL)
        return (NULL);

    child = __OBJ_New();
    if(child == NULL)
        return (NULL);

    low_atom = Int_LowAtomStart();
    child->parent = pLoc;
    child->child = NULL;
    child->represent = represent;
    if(ops != NULL)
        child->ops = ops;
    else
        child->ops = (fnObjOps)OBJ_DefaultOps;

    child->duty = 0;
    child->name = (char *)pName;
    child->reload = NULL;
    dListInit(&child->port);
    if(pLoc->child == NULL)
    {
        pLoc->child = child;
        __OBJ_LIST_INIT(child);
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(pLoc->child, child);
        pLoc->child = child;
    }

    Int_LowAtomEnd(low_atom);
    return (child);
}

// ============================================================================
// ���ܣ���һ��������뵽��һ��������ӽڵ�λ�ã��½ڵ㽫λ�ڶ���ͷλ�ã�ע�⣬
//      �½ڵ�������ӽڵ㡣
// ������pLoc -- ���ڵ㣻
//      pObj -- ��������ӽڵ㣬�������¼��ڵ㣻
// ���أ��½ڵ�ָ�룻
// ��ע��
// ============================================================================
struct Object *OBJ_InsertChild(struct Object *pLoc, struct Object *pObj)
{
    atom_low_t low_atom;

    if( (pLoc == NULL) || (pObj == NULL))
        return (NULL);

    pObj->parent = pLoc;
    low_atom = Int_LowAtomStart();
    if(pLoc->child == NULL)
    {
        pLoc->child = pObj;
        __OBJ_LIST_INIT(pObj);
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(pLoc->child, pObj);
        pLoc->child = pObj;
    }

    Int_LowAtomEnd(low_atom);
    return (pObj);
}

#if 0
// ============================================================================
// ���ܣ���һ���½ڵ��滻����������е�ԭ�нڵ㣬�½ڵ�ԭ�����벻�Ƕ����еĽڵ�
// ������pOld -- ���滻�Ľڵ㣻
//      pNew -- �½ڵ㣻
// ���أ��ɹ���TRUE��;
// ��ע��
// ============================================================================
bool_t OBJ_Displace(struct Object *pOld, struct Object *pNew)
{
    struct Object *temp1, *temp2;
    atom_low_t low_atom;

    if((NULL == pOld) || (NULL == pNew))
        return (FALSE);

    low_atom = Int_LowAtomStart();
    temp1 = pOld->child;
    if(temp1 != NULL)   //��oldnode�����ӽڵ�ĸ�ָ��ָ��New
    {
        temp2 = temp1;
        do
        {
            temp2->parent = pNew;
            temp2 = temp2->next;
        }while(temp2 != temp1);
    }

    pNew->child = temp1;
    pNew->next = pOld->next;
    pNew->prev = pOld->prev;
    pNew->parent = pOld->parent;
    pOld->next->prev = pNew;
    pOld->prev->next = pNew;
    if(pOld->parent->child == pOld) //����Ƕ���ͷ�ڵ�
        pOld->parent->child = pNew; //���ڵ���ӽڵ�ָ�����ͷ�ڵ�
    Int_LowAtomEnd(low_atom);

    return (TRUE);
}
#endif

// ============================================================================
// ���ܣ���һ����֦�Ӷ��������ɾ��
// ������pBranch -- ��ɾ���ķ�֧��
// ���أ���ɾ����ָ֧�룬NULL��ʾ��֧������
// ��ע��
// ============================================================================
struct Object *OBJ_TakeOutBranch(struct Object *pBranch)
{
    struct Object *result;
    atom_low_t low_atom;

    if(pBranch == NULL)
        return (NULL);

    low_atom = Int_LowAtomStart();
    if(pBranch->next == pBranch)   //˵���ýڵ�û���ֵܽڵ�.
    {
        pBranch->parent->child = NULL;
    }
    else
    {
        if(pBranch->parent->child == pBranch)
        {   //˵���ýڵ��Ƕ���ͷ�ڵ�,��Ҫ�ı����ͷ�ڵ�
            pBranch->parent->child = pBranch->next;
        }
        __OBJ_LIST_REMOVE(pBranch);
    }

    result = pBranch;
    Int_LowAtomEnd(low_atom);

    return (result);
}
// ============================================================================
// ���ܣ���һ���ڵ�Ӷ������жϿ��ڵ�,�ýڵ㲻�����ӽڵ�!
// ������pObj -- ��ɾ���Ľڵ�,��ýڵ����ӽڵ������ʹ����ɾ��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 OBJ_Del(struct Object *pObj)
{
    atom_low_t low_atom;

    if(pObj == NULL)
        return (-1);

    if((pObj->child != NULL) || __OBJ_IsProtected(pObj)) // ���ӽڵ㣬������ʹ�ã�������
        return (-1);

    low_atom = Int_LowAtomStart();
    __OBJ_Del(pObj);
    Int_LowAtomEnd(low_atom);
    __OBJ_Free(pObj);

    return (0);
}

// ============================================================================
// ���ܣ��ƶ�һ��������֦����Ľڵ������Ϊ��������
// ������pParent -- �¸��ڵ㣻
//      pObj����������ӽڵ㣬�������¼��ڵ㣻
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 OBJ_MoveToBranch(struct Object *pParent,struct  Object *pObj)
{
    atom_low_t low_atom;
    if( (pParent == NULL) || (pObj == NULL))
        return (-1);

    low_atom = Int_LowAtomStart();
    OBJ_TakeOutBranch(pObj);
    pObj->parent = pParent;
    if(pParent->child == NULL)
    {
        pParent->child = pObj;
        __OBJ_LIST_INIT(pObj);
    }
    else
    {
        __OBJ_LIST_INS_BEFORE(pParent->child, pObj);
        pParent->child = pObj;
    }
    Int_LowAtomEnd(low_atom);
    return (0);
}

// ============================================================================
// ���ܣ���nextָ�뷽���ƶ���������е�һ���ڵ㣬��ͬ������ͷ��ǰһ���ڵ�λ�ã�
// ������pObj -- ���ƶ��Ľڵ�ָ�룻
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 OBJ_MoveToLast(struct Object *pObj)
{
    struct Object *eldest;
    atom_low_t low_atom;

    if(pObj == NULL)
        return (-1);

    low_atom = Int_LowAtomStart();
    if(pObj->parent != NULL)    //���ڵ㲻���ƶ�
    {
        eldest = pObj->parent->child;
        if(eldest == pObj)
        //���node��ͷ�ڵ�,��ֱ���ƶ����ڵ����ָ�뵽��һ���ڵ�Ϳ�����.
            pObj->parent->child = pObj->next;
        else if(eldest->prev != pObj)    //�������һ���ڵ�
        {
            __OBJ_LIST_REMOVE(pObj);
            __OBJ_LIST_INS_BEFORE(eldest, pObj);
        }
    }
    Int_LowAtomEnd(low_atom);

    return (0);
}

// ============================================================================
// ���ܣ���previousָ�뷽���ƶ���������е�һ���ڵ㣬��Ϊͬ������ͷ��
// ������pObj -- ���ƶ��Ľڵ�ָ�룻
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 OBJ_MoveToHead(struct Object *pObj)
{
    struct Object *eldest;
    atom_low_t low_atom;

    if(pObj == NULL)
        return (-1);

    low_atom = Int_LowAtomStart();
    if(pObj->parent != NULL)    //���ڵ㲻���ƶ�
    {
        eldest = pObj->parent->child;
        if(eldest->prev == pObj)
        //���node��last�ڵ�,��ֱ���ƶ����ڵ����ָ�뵽��һ���ڵ�Ϳ�����.
            pObj->parent->child = pObj->prev;
        else if(eldest != pObj)    //����ͷ�ڵ�
        {
            __OBJ_LIST_REMOVE(pObj);
            __OBJ_LIST_INS_BEFORE(eldest, pObj);
            pObj->parent->child = pObj;
        }
    }

    Int_LowAtomEnd(low_atom);
    return (0);
}

// ============================================================================
// ���ܣ��ƶ���������е�һ���ڵ㵽��һ���ڵ��nextλ�ã����ƶ��Ľڵ�������ӽڵ㣻
// ������pLoc -- Ŀ��λ�ã�Obj�ƶ������ڵ���棻
//      pObj -- Ŀ��ڵ�,������ԭ������Object�����У�
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 OBJ_MoveToNext(struct Object *pLoc, struct Object *pObj)
{
    atom_low_t low_atom;

    if((pLoc == NULL) || (pObj == NULL) || (pLoc == pObj))
        return (-1);

    low_atom = Int_LowAtomStart();
    if(pLoc->next != pObj)    //���Ƿ��Ѿ�����Ҫ��
    {
        //�Ȱ�Ŀ��ڵ�Ӷ�����ȡ��
        if(pObj->next == pObj)   //˵���ýڵ�û���ֵܽڵ�.
        {
            pObj->parent->child = NULL;
        }
        else
        {
            if(pObj->parent->child == pObj)
            {   //˵���ýڵ��Ƕ���ͷ�ڵ�,��Ҫ�ı����ͷ�ڵ�
                pObj->parent->child = pObj->next;
            }
            __OBJ_LIST_REMOVE(pObj);
        }

        __OBJ_LIST_INS_AFTER(pLoc, pObj);
        pObj->parent = pLoc->parent;
    }
    Int_LowAtomEnd(low_atom);

    return (0);
}

// ============================================================================
// ���ܣ��ƶ���������е�һ���ڵ�NewPre����һ���ڵ�Obj��previousλ�ã����ƶ��Ľڵ�
//      �������ӽڵ�
// ������pLoc -- Ŀ��λ�ã�Obj�ƶ������ڵ�ǰ��
//      pObj -- Ŀ��ڵ㣬��ԭ������Object�����С�
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 OBJ_MoveToPrevious(struct Object *pLoc, struct Object *pObj)
{
    atom_low_t low_atom;

    if((pLoc == NULL) || (pObj == NULL) || (pLoc == pObj))
        return (-1);

    low_atom = Int_LowAtomStart();
    if(pLoc->prev != pObj) // ���Ƿ��Ѿ�����Ҫ��
    {
        //�Ȱ�Ŀ��ڵ�Ӷ�����ȡ��
        if(pObj->next == pObj)   //˵���ýڵ�û���ֵܽڵ�.
        {
            pObj->parent->child = NULL;
        }
        else
        {
            if(pObj->parent->child == pObj)
            {   //˵���ýڵ��Ƕ���ͷ�ڵ�,��Ҫ�ı����ͷ�ڵ�
                pObj->parent->child = pObj->next;
            }
            __OBJ_LIST_REMOVE(pObj);
        }

        __OBJ_LIST_INS_BEFORE(pLoc, pObj);
        pObj->parent = pLoc->parent;
    }
    Int_LowAtomEnd(low_atom);

    return (0);
}

// ============================================================================
// ����: Parent���ӽڵ�����λ�ò���,����ͷ��previous�����ƶ�һ��
// ������pParent -- ���ڵ�ָ��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 OBJ_RoundPrevious(struct Object *pParent)
{
    atom_low_t low_atom;

    if(pParent == NULL)
        return (-1);

    low_atom = Int_LowAtomStart();
    if(pParent->child != NULL)
    {
        pParent->child = pParent->child->prev;
    }
    Int_LowAtomEnd(low_atom);

    return (0);
}

// ============================================================================
// ���ܣ�Parent���ӽڵ�����λ�ò���,����ͷ��next�����ƶ�һ��
// ������pParent -- ���ڵ�ָ��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 OBJ_RoundNext(struct Object *pParent)
{
    atom_low_t low_atom;

    if(pParent == NULL)
        return (-1);

    low_atom = Int_LowAtomStart();
    if(pParent->child != NULL)
    {
        pParent->child = pParent->child->next;
    }

    Int_LowAtomEnd(low_atom);

    return (0);
}

// ============================================================================
// ���ܣ��޸Ķ�������
// ������pObj -- ���޸ĵĶ���ڵ�ָ�룻
//      pName -- �����֣�
// ���أ��ɹ���0����ʧ�ܣ�-1��;
// ��ע��
// ============================================================================
s32 OBJ_Rename(struct Object *pObj, const char *pName)
{
    if((pObj == NULL) || (pName == NULL))
        return (-1);

    pObj->name = (char *)pName;  //�����ж��������ַ����ĳ��ȣ��������������ǿմ�

    return (0);
}

// ============================================================================
// ���ܣ�����ĳ�ڵ�����֣�
// ������pObj,�������Ķ���ڵ㣻
// ���أ��ýڵ�����֣�
// ע�⣺�ýڵ����û�����֣���˷��صļ�ʹ�ǳɹ�Ҳ����ΪNULL����Ϊȷʵû������
//      �ɹ�ʧ�ܲ�Ӧ�þݴ����ж�
// ============================================================================
const char *OBJ_Name(struct Object *pObj)
{
    if(!pObj)
        return (NULL);

    return (__OBJ_Name(pObj));
}

// ============================================================================
// ����: ����ĳ�ڵ�ĸ��ڵ�,
// ����: pObj -- ��Ҫ���ҵ���֦������һ���ӽڵ�
// ����: �ɹ������ڵ㣩��ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct Object *OBJ_Parent(struct Object *pObj)
{
    if(!pObj)
        return (NULL);

    return (__OBJ_Parent(pObj));
}

// ============================================================================
// ���ܣ������ӽڵ㣬�򸸽ڵ��childָ��ָ���ӽڵ�Ķ���ͷ��ʵ�ʷ����ӽڵ�Ķ���ͷ��
// ������pObj -- ��Ҫ���ҵ���֦�ĸ��ڵ㣻
// ���أ��ɹ����ӽڵ㣩��ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
struct Object *OBJ_Child(struct Object *pObj)
{
    if(!pObj)
        return (NULL);

    return (__OBJ_Child(pObj));
}

// ============================================================================
// ���ܣ�����Obj�ڵ��previous�ڵ�
// ������pObj -- ��Ҫ���ҵ���֦���ֽڵ��е�һ���ڵ�
// ���أ�previous�ڵ�.
// ��ע��
// ============================================================================
struct Object *OBJ_Previous(struct Object *pObj)
{
    if(!pObj)
        return (NULL);

    return (__OBJ_Prev(pObj));
}

// ============================================================================
// ���ܣ�����Obj�ڵ��next�ڵ㣻
// ������pObj -- ��Ҫ���ҵ���֦���ֽڵ��е�һ���ڵ㣻
// ���أ�next�ڵ㣻
// ��ע��
// ============================================================================
struct Object *OBJ_Next(struct Object *pObj)
{
    if(!pObj)
        return (NULL);

    return (__OBJ_Next(pObj));
}

// ============================================================================
// ���ܣ�����Obj�ڵ����ڶ��еĶ���ͷ��
// ������pObj,��Ҫ���ҵ���֦������ڵ㣻
// ���أ�����ͷָ�룻
// ��ע��
// ============================================================================
struct Object *OBJ_Head(struct Object *pObj)
{
    if(!pObj || !pObj->parent)
        return (NULL);

    return (pObj->parent->child);
}

// ============================================================================
//���ܣ�����ĳ��֦��һ��ĩ�ҽڵ�,ĩ�ҽڵ���ָû���ӽڵ�Ľڵ�.
//������pObj,��Ҫ��������֦�ĸ��ڵ�
//���أ���֦��һ��ĩ�ҽڵ�.��û���ӽڵ�ʱ������NULL
//��ע������Ҫɾ��������֦ʱ,������������,���rsc_del_node����,�������ñ�����,
//     ���ѷ��صĽڵ�ɾ��,ֱ������������NULL.��Ҫɾ��һ���ļ��л���ɾ��һ����
//     ���Ӵ��ڵ�gui����ʱ,����Ҫ�õ�ɾ��������֦�Ĳ���.
// ============================================================================
struct Object *OBJ_Twig(struct Object *pObj)
{
    struct Object *result = NULL, *current;

    if(pObj == NULL)
        return (NULL);

    current = pObj;
    while(current->child != NULL)
    {
        current = current->child;
    }

    if(current == pObj)
        result = NULL;
    else
        result = current;

    return (result);
}

// ============================================================================
// ���ܣ��鿴ĳ�ڵ��Ǹ��ڵ�ĵڼ����ӽڵ㣻
// ������pObj -- ����Ľڵ㣻
// ���أ��ӽڵ㼶����0������ڵ㡣cn_limit_uint32�������
// ��ע��
// ============================================================================
u32 OBJ_Level(struct Object *pObj)
{
    u32 n = 0;
    struct Object *pl_node = pObj;

    if(pl_node == NULL)
        return CN_LIMIT_UINT32;

    while(pl_node->parent !=  pRootObj)
    {
        if(NULL != pl_node->parent)
            pl_node = pl_node->parent;
        else
        {
            n = CN_LIMIT_UINT32;
            break;
        }
        n++;
    }

    return (n);
}

#if 0
//----ȡ�ڵ��·��-------------------------------------------------------------
//����: �鿴ĳ�ڵ��Ǹ��ڵ�ĵڼ����ӽڵ㣬
//����: pObj,����ȡ·���Ľڵ�
//      path������·���Ļ�����
//      limit������������
//����: path��ַ�������򷵻�NULL
//-----------------------------------------------------------------------------
char *OBJ_GetPath(struct Object *pObj, char *path, u32 limit)
{
    struct Object *Current,*root;
    char *pos;
    u32 count = 0;
    if((pObj == NULL)||(path == NULL) || (limit < 2))
        return (NULL);
    Current = pObj;
    root = pRootObj;
    pos = path + limit - 2;     //�������һ�� '\0'�ַ�λ��
    while(1)
    {
        if(Current != root )
        {
            count = strlen(Current->name);
            pos -= count;
            if(pos <= path)     //path[0]ҲҪ������
                return (NULL);
            memcpy(pos, Current->name, count);
            pos--;
            *pos = '/';
            Current = Current->parent;
        }
        else
        {
            count = limit - 2 - (pos - path);
            memcpy(path+1, pos, count);
            path[0] = '/';
            path[count +1] = '\0';
            break;
        }
    }
    return path;
}
#endif

// ============================================================================
// ���ܣ��ӵ�ǰ�ڵ㿪ʼ,��ȡ��һ���ڵ��ָ��,ֱ��������ȫ���ӽڵ�
// ������pParent -- ��Ҫ��������֦�����Ƚڵ�
//      pChild -- ��ǰ����λ��,��ʼλ��Ӧ����Ϊ��parent��ȣ���������ȫ���ӽڵ㡣
// ���أ���ǰ����λ�õ���һ���ڵ�ָ��,����Ѿ��������,�򷵻�NULL.
// ��ע��
// ============================================================================
struct Object *OBJ_TraveChild(struct Object *pParent, struct Object *pChild)
{
    if((pParent == NULL) || (pChild == NULL))
        return (NULL);

    if(pChild == pParent)
    {
        pChild = pParent->child;
    }
    else
    {
        pChild = pChild->next;
        if(pChild == pParent->child)
            pChild = NULL;
    }

    return (pChild);
}

// ============================================================================
// ���ܣ��ӵ�ǰ�ڵ㿪ʼ,��ȡ��һ���ڵ��ָ��,��������·��,ֱ��������������֦.����
//      ·��Ϊ:��ǰ�ڵ���ӽڵ�,����ӽڵ�Ϊ��������next�ڵ�,next�ڵ���Ϊ������
//      �����ڵ��next�ڵ�,ֱ���������.
//      ������ֻ��������ڵ㣬������ancestor_node����
// ������pAncestor,��Ҫ��������֦�����Ƚڵ�
//      Current,��ǰ����λ��,
// ���أ���ǰ����λ�õ���һ���ڵ�ָ��,����Ѿ��������,�򷵻�NULL.
// ��ע������Ҫ�Զ�������ĳһ����֦�������������еĽڵ���һ����ĳ�ֲ���ʱ,��
//      �������ñ�����,��һ�ε���current_node = parent_node,���current_node
//      = �ϴη���ֵ��ֱ�����ؿ�.
//      �������������ӡ������....��˳������������ֱϵ��������ϵ��ȷ����������
//      �ڵ㶼�ܹ����ʵ�������Է���˳��������Ҫ�󣬲���ʹ�ñ�����
// ============================================================================
struct Object *OBJ_TraveScion(struct Object *pAncestor, struct Object *pCurrent)
{
    struct  Object *result = NULL,*current_copy;
    bool_t up = FALSE;

    if((pAncestor == NULL) || (pCurrent == NULL))
        return (NULL);

    if((pCurrent != pAncestor) || pAncestor->child)
    {
        current_copy = pCurrent;
        do
        {
            if((up == FALSE) && (current_copy->child != NULL))
            {   //�ӽڵ�ǿ�,�����ӽڵ�
                result = current_copy->child;
                break;
            }
            else if(current_copy->next != current_copy->parent->child)
            {   //�ӽڵ��,�����ڵ㲻�Ƕ���β�ڵ�,����next�ڵ�
                result = current_copy->next;
                break;
            }
            else
            {   //���ӽڵ�,�ұ��ڵ��Ѿ��Ƕ���β�ڵ�,��Ҫ�жϵ�ǰ���ڵ��next�ڵ�
                current_copy = current_copy->parent;
                up = true;
            }
        }while(current_copy != pAncestor);
    }

    return (result);
}

// ============================================================================
// ���ܣ���ĳһ������ڵ�������ֵܽڵ��������������ֵĶ���
// ������pBrother,�ֵܽڵ�������һ������ָ��.
//      pName,��Ҫ�����Ķ�����
// ���أ�������������󷵻ض���ڵ�ָ��,���򷵻�NULL
// ��ע��
// ============================================================================
struct Object *OBJ_SearchSibling(struct Object *pBrother, const char *pName)
{
    struct Object *temp, *result = NULL;

    if((pBrother == NULL) || (pName == NULL))
        return (NULL);

    temp = pBrother;
    do
    {
        if(strcmp(temp->name, pName) == 0)
        {
            result = temp;
            break;
        }
        temp = temp->next;
    }while (temp != pBrother);

    return (result);
}

// ============================================================================
// ���ܣ���ĳһ������ڵ�������ӽڵ��������������ֵĶ���
// ������pParent -- ���ڵ����ָ�룻
//      pName -- ��Ҫ�����Ķ�����,��������,������·����
// ���أ�������������󷵻ض���ڵ�ָ��,���򷵻�NULL��
// ��ע��
// ============================================================================
struct Object *OBJ_SearchChild(struct Object *pParent, const char *pName)
{
    struct Object *temp, *result = NULL;

    if((pName == NULL) || (pParent == NULL))
        return (NULL);

    temp = pParent->child;
    if(temp == NULL)
    {
        result = NULL;
    }
    else
    {
        do
        {
            if(strcmp(temp->name, pName)==0)
            {
                result = temp;
                break;
            }
            temp = temp->next;
        }while (temp != pParent->child);
    }

    return (result);
}

// ============================================================================
// ���ܣ���OBJ_SearchChild����,����Ancestor�����к���ڵ�,ֱ���ҵ�һ������
//      ƥ�����
// ������pAncestor,��֦�ĸ��ڵ�
//      pName,��Ҫ�����Ķ�����,��������,������·��.
// ���أ�������������󷵻ض���ڵ�ָ��,���򷵻�NULL
// ��ע��
// ============================================================================
struct Object *OBJ_SearchScion(struct Object *pAncestor, const char *pName)
{
    struct Object *current, *temp, *result = NULL;

    if((pAncestor == NULL) || (pName == NULL))
        return (NULL);

    current = pAncestor;
    //��rsc_trave_scion���Ѿ����ź����������˴����뱣��
    while((temp = OBJ_TraveScion(pAncestor, current)) != NULL)
    {
        if(strcmp(temp->name, pName) == 0)
        {
            result = temp;
            break;
        }
        current = temp;
    }

    return (result);
}

// ============================================================================
// ���ܣ���OBJ_SearchScion����,��ͬ���ǣ�path�ǰ���·�������ַ�����ִ�о�ȷ������
// ������pStart -- ��֦�ĸ��ڵ㣻
//      pPath -- ����·�����Ķ�������
// ���أ�������������󷵻ض���ڵ�ָ�룬���򷵻�NULL��
// ��ע��
// ============================================================================
struct Object *OBJ_Search(struct Object *pStart, const char *pPath)
{
    const char *path_name;
    char *ObjName;
    bool_t end = FALSE;
    u32 i;
    struct Object *current, *result = NULL;

    if((pPath == NULL) || (pStart == NULL))
        return (NULL);

    current = pStart->child;
    if(current == NULL)
    {
        return (NULL);
    }

    path_name = pPath;
    while(1)
    {
        result = current;
        ObjName = current->name;
        for(i=0;i<CN_OBJ_NAME_LIMIT;i++)        //��������������
        {
            if(ObjName[i] == path_name[i])
            {
                if(path_name[i] == '\0')        //ƥ�䣬��path����
                {
                    result = current;
                    end = true;
                }
            }
            else if( (ObjName[i] == '\0')
                    && ((path_name[i] == '\\') || (path_name[i] == '/')))
            {   //�������ֽ�����path�������ַָ���
                if(path_name[i+1] == '\0')      //ƥ�䣬��path����
                {
                    result = current;
                    end = true;
                }
                else                        //ƥ�䣬��pathδ����,���������ӽڵ�
                {
                    path_name += i+1;
                    current = current->child;
                    if(current == NULL)     //û���ӽڵ��ˣ���pathδ����
                    {
                        result = NULL;
                        end = true;
                        break;
                    }
                }
            }
            else // �ַ��������
            {
                current = current->next;
                if(current == current->parent->child)
                {
                    //���������нڵ��Ѿ�������,û���ҵ�ƥ���.
                    result = NULL;
                    end = true;
                }

                break;
            }
        }
        if(end)
            break;
    }

    return result;
}
#if 0
//----��·��ƥ�������---------------------------------------------------------
//����: ��OBJ_Search����,��ͬ���ǣ��ҵ���һ��ƥ�䲻�ϵľͷ��ء����磬����������
//      "obj1\obj2\"��path="obj1\obj2\obj3\obj4"��������obj2��ָ�롣
//����: Start,��֦�ĸ��ڵ�
//      Path,����·�����Ķ�����
//      pos����һ����ƥ��ģ�NULL��ʾȫƥ�䡣
//����: ���һ��ƥ��Ķ���ָ�룬��ȫ��ƥ���򷵻�NULL
//-----------------------------------------------------------------------------
struct Object *OBJ_MatchPath(struct Object *Start, const char *Path,char **pos)
{
    char *path_name;
    char *ObjName;
    bool_t end = FALSE;
    u32 i;
    struct Object *current, *result = NULL;

    if((Path == NULL) || (Start == NULL))
        return NULL;

    current = Start->child;
    if(current == NULL)
    {
        return NULL;
    }
    path_name = (char *)Path;
    while(1)
    {
        result = current;
        ObjName = current->name;
        for(i=0;i<CN_OBJ_NAME_LIMIT;i++)        //��������������
        {
            if(ObjName[i] == path_name[i])
            {
                if(path_name[i] == '\0')        //ƥ�䣬��path����
                {
                    result = current;
                    *pos = NULL;
                    end = true;
                }
            }
            else if( (ObjName[i] == '\0')
                    && ((path_name[i] == '\\') || (path_name[i] == '/')))
            {   //�������ֽ�����path�������ַָ���
                result = current;
                if(path_name[i+1] == '\0')      //ƥ�䣬��path����
                {
                    *pos = NULL;
                    end = true;
                }
                else                        //ƥ�䣬��pathδ����,���������ӽڵ�
                {
                    path_name += i+1;
                    *pos = path_name;
                    current = current->child;
                    if(current == NULL)
                    {
                        end = true;
                        break;
                    }
                }
            }
            else
            {   //�ַ��������
                current = current->next;
                if(current == current->parent->child)
                {
                //���������нڵ��Ѿ�������,û���ҵ�ƥ���.
                    end = true;
                }
                break;
            }
        }
        if(end)
            break;
    }

    return result;
}
#endif

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void OBJ_DelChild(struct Object *pChild)
{
    __OBJ_LIST_REMOVE(pChild);
    __OBJ_Free(pChild);
}

#if 0
// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
u32 OBJ_IncUseOne(struct Object *pObj)
{
    if(pObj == NULL)
    {
        return (0);
    }
    else
    {
        if(pObj->duty != CN_LIMIT_UINT32)
            pObj->duty++;

        return (pObj->duty);
    }
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void OBJ_IncUseAlongPath(struct Object *pObj)
{
    while(pObj != NULL)
    {
        if(pObj->duty != CN_LIMIT_UINT32)
            pObj->duty++;

        pObj = pObj->parent;
    }
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
u32 OBJ_DecUseOne(struct Object *pObj)
{
    if(!pObj)
        return (0);

    if(pObj->duty != 0)
        pObj->duty--;

    return (pObj->duty);

}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void OBJ_DecUseAlongPath(struct Object *pObj)
{
    while(pObj != NULL)
    {
        if(pObj->duty != 0)
            pObj->duty++;
        pObj = pObj->parent;
    }
    return ;
}
#endif

//----�Ƿ��ӹ�ϵ-------------------------------------------------------------
//���ܣ��ж����������Ƿ��ӹ�ϵ
//������pParent�����ܵĸ�����
//      pChild�� ���ܵ��Ӷ���
//���أ�true = ���ӹ�ϵ������FALSE = ������
//-----------------------------------------------------------------------------
bool_t OBJ_IsParentChild(struct Object *pParent, struct Object *pChild)
{
    struct Object *temp;
    bool_t result = FALSE;

    if((pChild == NULL) || (pParent == NULL))
        return (FALSE);

    temp = pParent->child;
    if((temp == NULL) || (pChild->parent != pParent))
        result = FALSE;
    else
    {
        do
        {
            if(pParent->child == pChild)
            {
                result = true;
                break;
            }
            temp = temp->next;
        }while (temp != pParent->child);
    }

    return result;
}

//----�Ƿ����ͷ-------------------------------------------------------------
//����: �жϸ����ڵ��Ƿ��ֵܽڵ��еĶ���ͷ�ڵ�
//����: pObj�������Խڵ�
//����: true=�Ƕ���ͷ��FALSE=���Ƕ���ͷ
//-----------------------------------------------------------------------------
bool_t OBJ_IsHead(struct Object *pObj)
{
    if(NULL == pObj)
        return (FALSE);

    if(pObj == pRootObj)
        return (TRUE);

    if((pObj) && (pObj  == pObj->parent->child))
        return (TRUE);
    else
        return (FALSE);
}

//----�Ƿ����β---------------------------------------------------------------
//����: �жϸ����ڵ��Ƿ��ֵܽڵ��е����һ���ڵ�
//����: pObj�������Խڵ�
//����: true=�����һ���ڵ㣬FALSE=����
//-----------------------------------------------------------------------------
bool_t OBJ_IsLast(struct Object *pObj)
{
    if(NULL == pObj)
        return (FALSE);

    if(pObj == pRootObj)
        return (TRUE);

    if(pObj->next  == pObj->parent->child)
        return (TRUE);
    else
        return (FALSE);
}

//----ȡ�ڵ�˳��-------------------------------------------------------------
//����: ȡ�����ڵ����ֵܽڵ��е�λ�ã�head=0����һ����1������֮
//����: pObj�������Խڵ�
//����: �ڵ�λ�ú�
//-----------------------------------------------------------------------------
u32 OBJ_Sequencing(struct Object *pObj)
{
    u32 ranking;
    struct Object *temp;

    if(NULL == pObj)
        return (-1);

    if(pObj == pRootObj)
        return (0);
        
    ranking = 0;
    temp = pObj->parent->child;
    while(temp != pObj)
    {
        ranking +=1;
        temp = temp->next;
    }
    
    return (ranking);
}

