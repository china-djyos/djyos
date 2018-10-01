//-----------------------------------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include <object.h>
#include <objhandle.h>
#include <stdio.h>
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
//init time:none                //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
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
static tagFST *__findtype(const char *pType)
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
// ���أ��ɹ���0����ʧ�ܣ�-1������ע�ᣨ1����
// ��ע��
// ============================================================================
s32 regfs(tagFST *type)
{
    struct filesystem *newt;

    if(__findtype(type->pType))
        return (1);

    newt = malloc(sizeof(*newt));
    newt->pType = type;

    if(!pFileSystemTypes)
    {
        dListInit(&(newt->list));
        pFileSystemTypes = newt;
    }
    else
    {
        dListInsertAfter(&(pFileSystemTypes->list), &(newt->list));
    }

    return (0);
}

// ============================================================================
// ���ܣ��µ��ļ�ϵͳ��װ
// ������source -- ��Ҫ���ϵ��ļ�ϵͳ��ͨ����һ���豸����
//      target -- �ļ�ϵͳ��Ҫ���ص�Ŀ�����Ŀ¼����
//      type -- �ļ�ϵͳ���ͣ�
//      flags -- �ļ�ϵͳ��д���ʱ�־��
//      data -- �ļ�ϵͳ���в�����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 mountfs(const char *source, const char *target, const char *type, u32 opt, void *data)
{
    tagFST *fstype;
    tagFSC *super;
    struct obj *srco, *targeto, *tmpo;
    s32 res;
    char *notfind;

    fstype = __findtype(type);
    if(!fstype)
    {
        debug_printf("fs","mount failed(cannot find type \"%s\")", type);
        return (-1);
    }

    srco = obj_matchpath(source, &notfind, 0);
    if(notfind)
    {
        // δ�ҵ��豸
        debug_printf("fs","mount failed(cannot find device \"%s\").", source);
        return (-1);
    }

    targeto = obj_matchpath(target, &notfind, 0);
    if(notfind)
    {
#if 0
        // δ�ҵ���װ��
        targeto = obj_buildpath(targeto, notfind); // ������װ��
#else
        return (-1); // ��װ�����׼���á�
#endif
    }
    else
    {
        if(obj_testset(targeto)) // �����Ͽɷ񴴽�����
        {
            tmpo = targeto;
            targeto = obj_replacebyset(tmpo, 0, 0); // ԭ���Ķ��󱻶��󼯺ϣ��µ��ļ����ͼ��ϣ������
            if(!targeto)
            {
                debug_printf("fs","mount failed(cannot target replace the \"%s\").", target);
                return (-1);
            }
        }
        else
        {
            debug_printf("fs","mount failed(cannot mount on the \"%s\" for wrong target type).", target);
            return (-1);
        }

    }

    super = malloc(sizeof(*super));
    super->pTarget = targeto;
    super->pDev = srco;
    obj_setval(targeto, (ptu32_t)super); // ���ö�������ݣ����������Ǽ��ϣ�
    obj_setops(targeto, fstype->fileOps); // ���ö��󷽷������������Ǽ��ϣ�
    res = fstype->install(super, opt, data);
    if(res)
    {
         // ��װʧ��
        tmpo = obj_destoryset(super->pTarget);
        free(super);
        return (-1);
    }

    // if(!DJYFS_PATH_BUFFER)
    return (0);
}

// ============================================================================
// ���ܣ���ȡ�ļ�ϵͳ���󣨼��ϵ㣩�Ĺ�����
// ������ob -- �ļ�ϵͳ���󼯺ϣ�
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
void *corefs(struct obj *ob)
{
    tagFSC *super;

    if(!ob)
        return (NULL);

    if(!obj_isset(ob)) // �����Ǽ��ϵ㣻
    {
        ob = obj_set(ob); // ��ȡ���󼯺ϣ�
        if((!ob)&&(!obj_isset(ob)))
            return (NULL); // ���Ǽ��ϻ��߲����ڣ�
    }

    super = (tagFSC *)obj_val(ob);
    if(!super)
        return (NULL);

    return (super->pCore);
}

