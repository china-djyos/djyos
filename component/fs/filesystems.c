//-----------------------------------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <list.h>
#include <objfile.h>
#include "filesystems.h"
#include "dbug.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"filesystem"   //�ļ�ϵͳ���
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:�������            //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                   //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:none                    //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"none",             //������������������������none����ʾ�������������
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
//%$#@num,0,100
//%$#@enum,true,false
//%$#@string
//%$#@select
//%$#@free
//%$#@end configue  ****�������ý���
//@#$%component end configure


#define DJYFS_PATH_BUFFER_SIZE          266

static char __DJYFS_PATH_BUFFER[DJYFS_PATH_BUFFER_SIZE];
char *DJYFS_PATH_BUFFER = __DJYFS_PATH_BUFFER; // ������ֲ���ļ�ϵͳ��·��ƴ��

struct filesystem{
    struct dListNode list;
    tagFST *pType;
} *pFileSystemTypes;


// ============================================================================
// ���ܣ������ļ�ϵͳ���͡�
// ������pType -- �ļ�ϵͳ��������
// ���أ��ļ�ϵͳ���ͣ�δ�ҵ���NULL����
// ��ע��
// ============================================================================
tagFST *FS_Find(const char *pType)
{
    list_t *cur;
    struct filesystem *fs;

    if(!pFileSystemTypes)
        return (NULL);

    fs = pFileSystemTypes;
    if(!strcmp(fs->pType->pType, pType))
        return (fs->pType);

    dListForEach(cur, &pFileSystemTypes->list)
    {
        fs = dListEntry(cur, struct filesystem, list);

        if(!strcmp(fs->pType->pType, pType))
        {
            return (fs->pType);
        }
    }

    return (NULL);
}

// ============================================================================
// ���ܣ��µ��ļ�ϵͳ����ע�ᡣ
// ������pType -- �µ��ļ�ϵͳ���͡�
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 FS_Register(tagFST *pType)
{
    struct filesystem *new;

    if(FS_Find(pType->pType))
        return (-1);

    new = malloc(sizeof(*new));
    new->pType = pType;

    if(!pFileSystemTypes)
    {
        dListInit(&(new->list));
        pFileSystemTypes = new;
    }
    else
    {
        dListInsertAfter(&(pFileSystemTypes->list), &(new->list));
    }

    return (0);
}

// ============================================================================
// ���ܣ��µ��ļ�ϵͳ��װ
// ������pSource -- ��Ҫ���ϵ��ļ�ϵͳ��ͨ����һ���豸����
//      pTarget -- �ļ�ϵͳ��Ҫ���ص�Ŀ�����Ŀ¼����
//      pType -- �ļ�ϵͳ���ͣ�
//      dwFlags -- �ļ�ϵͳ��д���ʱ�־��
//      pData -- �ļ�ϵͳ���в�����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 FS_Mount(const char *pSource, const char *pTarget, const char *pType, u32 dwFlags, void *data)
{
    tagFST *type;
    tagFSC *super;
    struct Object *source, *target, *temp;
    s32 res;
    char *notfind;

    type = FS_Find(pType);
    if(!type)
    {
        debug_printf("FS","mount failed. cannot find \"%s\"<type>.", pType);
        return (-1);
    }

    source = OBJ_MatchPath(pSource, &notfind, 0);
    if(notfind)
    {
        // δ�ҵ��豸
        debug_printf("FS","mount failed. cannot find \"%s\"<device>.", pSource);
        return (-1);
    }

    target = OBJ_MatchPath(pTarget, &notfind, 0);
    if(notfind)
    {
#if 0
        // δ�ҵ���װ��
        target = OBJ_BufferPath(target, notfind); // ������װ��
#else
        return (-1); // ��װ�����׼���á�
#endif
    }
    else
    {
        // Ŀ��ڵ��Ѵ��ڣ��ж����Ƿ���Ŀ¼
        res = __OBJ_Type(target);
        if((DIR_POINT == res) || (GROUP_POINT == res))
        {
            temp = target;
            target = OBJ_Replace(temp); // ԭ���Ľڵ㱻�µ������
            if(!target)
            {
                debug_printf("FS","mount failed. cannot mount on the \"%s\"<target replace>.", pTarget);
                return (-1);
            }

            __OBJ_SetMountPoint(temp); // ��ԭ�ڵ�����Ϊ��װ�㣬��ֹ��ر�ʱ���ͷţ���Ե��������ļ�ϵͳ������Ϊ�ָ�ʱ����Ҫ��
        }
        else
        {
            debug_printf("FS","mount failed. cannot mount on the \"%s\"<target type>.", pTarget);
            return (-1);
        }

    }

    super = malloc(sizeof(*super));
    super->pTarget = target;
    super->pDev = source;
    __OBJ_SetRepresent(target, (ptu32_t)super);
    __OBJ_SetOps(super->pTarget, type->fileOps);
    res = type->install(super, dwFlags, data);
    if(res)
    {
         // ��װʧ��
        temp = OBJ_Restore(super->pTarget);
        __OBJ_ClearMountPoint(temp); //
        free(super);
        return (-1);
    }

    // if(!DJYFS_PATH_BUFFER)



    return (0);

}

// ============================================================================
// ���ܣ�
// ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
void *FS_Core(struct Object *pObj)
{
    tagFSC *super;

    if(!pObj)
        return (NULL);

    super = (tagFSC *)__OBJ_Represent(pObj);
    if(!super)
        return (NULL);

    return (super->pCore);
}

