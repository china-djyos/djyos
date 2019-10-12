//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include "component_config_fs.h"
#include <device.h>
#include <dirent.h>


static char __DJYFS_PATH_BUFFER[DJYFS_PATH_BUFFER_SIZE];
char *DJYFS_PATH_BUFFER = __DJYFS_PATH_BUFFER; // ������ֲ���ļ�ϵͳ��·��ƴ��

struct filesystem *pFileSystemTypes;
// ============================================================================
// ���ܣ������ļ�ϵͳ���͡�
// ������pType -- �ļ�ϵͳ��������
// ���أ��ļ�ϵͳ���ͣ�δ�ҵ���NULL����
// ��ע��
// ============================================================================
static struct filesystem *__findtype(const char *pType)
{
    list_t *cur;
    struct filesystem *fs;

    if(!pFileSystemTypes)
        return (NULL);

    fs = pFileSystemTypes;
    if(!strcmp(fs->pType, pType))
        return (fs);

    dListForEach(cur, &pFileSystemTypes->list)
    {
        fs = dListEntry(cur, struct filesystem, list);
        if(!strcmp(fs->pType, pType))
        {
            return (fs);
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
s32 regfs(struct filesystem *type)
{
    if(__findtype(type->pType))
        return (1);

    if(!pFileSystemTypes)
    {
        dListInit(&(type->list));
        pFileSystemTypes = type;
    }
    else
    {
        dListInsertAfter(&(pFileSystemTypes->list), &(type->list));
    }

    return (0);
}

// ============================================================================
// ���ܣ�mount����ļ������ӿڣ�opsTarget �϶��� mountpoint����ʵ�ʲ����ģ��������¼�
//      ·���ϵ��ļ���Ŀ¼��ֻ�Ǹ��ļ���Ŀ¼���ڴ������棬��û�йҵ�object���С���ʱ��Ӧ
//      ��ת���þ����ļ�ϵͳ��ops������ opsTarget �� struct Object *�����OpsArgs3 ��
//      ��ʾ������·���� opsTarget ��Ĳ��֣���ˣ�ֻҪ opsTarget �� struct Object *����
//      opsTarget != NULL �ģ���Ҫ���þ����ļ�ϵͳ�� ops ������
// ��������׼�߼����鿴�ӿ�˵����
// ���أ���׼�߼����鿴�ӿ�˵����
// ��ע:
// ============================================================================
s32 __mount_ops(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                       ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    struct FsCore *super;
    struct Object *me;
    struct objhandle *hdl;
    fnObjOps MyOps;
    s32 result = CN_OBJ_CMD_EXECUTED;

    if((objcmd & CN_TARGET_IS_OBJ) && ((((u32)(*(u64*)OpsArgs2) & O_DIRECTORY) || (char*)OpsArgs3 != NULL)))
    {
        me = (struct Object *)opsTarget;
        super = (struct FsCore *)obj_GetPrivate(me);
        MyOps = super->pFsType->fileOps;
        result = MyOps(opsTarget, objcmd, OpsArgs1, OpsArgs2, OpsArgs3);
    }
    else
    {
        switch(objcmd)
        {
            case CN_OBJ_CMD_STAT:
            {
                ((struct stat *)OpsArgs1)->st_mode = S_IFMOUNT;
                result = CN_OBJ_CMD_TRUE;
                break;
            }
            case CN_OBJ_CMD_CLOSE:
            {
                hdl = (struct objhandle *)opsTarget;
                me = hdl->HostObj;
                super = (struct FsCore *)obj_GetPrivate(me);
                MyOps = super->pFsType->fileOps;
                result = MyOps(opsTarget, objcmd, OpsArgs1, OpsArgs2, OpsArgs3);
                break;
            }
            default:
            {
                result = CN_OBJ_CMD_UNSUPPORT;
                break;
            }
        }
    }
    return (result);
}

//----��ȡ����·��---------------------------------------------------------------
//���ܣ���ȡĳ�ļ�ϵͳ�ڡ��Թҽڵ㿪ʼ������·�������磬yaffs2��mount��"/abc/d/"Ŀ¼��
//      ����object��Ӧ"/abc/d/123/"��path = "456/my.txt"��������"123/456/my.txt"
//������ BaseObject��������ָ�룬�������mount��
//      PathTail������� BaseObject ��·��
//      EntirePath����������·����buffer
//���أ�false = ʧ�ܣ�true = �ɹ�
//-----------------------------------------------------------------------------
bool_t GetEntirePath(struct Object *BaseObject, char * PathTail, char * EntirePath,
                     u32 BufSize)
{
    struct Object *current;
    char *Name;
    char *Entire = EntirePath;
    u32 objnum = 0,len;
    s32 offset = BufSize;
    //���ѭ������ BaseObject->parent һֱ�鵽���ص㣬���Ѹ��� object->nameָ�뱣��
    //�� EntirePath �У��� EntirePath �ĺ�����ǰ�档
    for(current = BaseObject; current != obj_root(); current = obj_parent(current))
    {
        offset -= sizeof(void*);
        if(offset < 0)
            return false;
        objnum++;
        memcpy(EntirePath + offset, &current->name, sizeof(void*));
        if(obj_GetOps(current) == __mount_ops)
            break;
    }
    //�� EntirePath ȡ��ָ�룬����������copy�� EntirePath �С�
    for(; objnum != 0; objnum--)
    {
        memcpy(&Name, EntirePath + offset, sizeof(void*));
        offset += sizeof(void*);
        len = strlen(Name);
        memcpy(Entire,Name,len);
        Entire[len] = '/';
        Entire += len+1;
    }
    //��PathTail�е��ַ���copy��EntirePath��
    Name = PathTail;
    while('/' == *Name)
        Name++; // ���˶����'/'
    if((Name != 0) && (*Name != 0))
    {
        len = strlen(Name);
        strcpy(Entire,Name);
    }
    else
    {
        if(*(Entire - 1) == '/')
        {
            *(Entire - 1) = '\0';
        }
    }
    return true;
}

// ============================================================================
// ���ܣ��������������ļ��Ƿ���Ŀ¼��
// ������hdl -- ��������
// ���أ�Ŀ¼��1������Ŀ¼��0����
// ============================================================================
bool_t isDirectory(struct objhandle *hdl)
{
//    mode_t mymode;
//    mymode = handle_GetHostObjectPrivate(hdl);
//    return S_ISDIR(mymode);
    return test_directory(hdl->flags);
}


// ============================================================================
// ���ܣ��ж�ĳ�����Ƿ��ļ�ϵͳ mount ��
// ������obj������
// ���أ�true = �� mount �㣬false = ����
// ============================================================================
bool_t obj_isMount(struct Object *obj)
{
    if(obj_GetOps(obj) == __mount_ops)
        return true;
    else
        return false;
}
// ============================================================================
// ���ܣ��µ��ļ�ϵͳ��װ
// ������source -- ��Ҫ���ϵ��ļ�ϵͳ��ͨ����һ���豸����
//      target -- �ļ�ϵͳ��Ҫ���ص�Ŀ�����Ŀ¼����
//      type -- �ļ�ϵͳ���ͣ�
//      flags -- �ļ�ϵͳ��д���ʱ�־��
//      data -- �ļ�ϵͳ���в�����
// ���أ��ɹ���0����ʧ�ܣ�-1������δ���ص������ý����ȥ��-2����
// ��ע��
// ============================================================================
s32 mountfs(const char *source, const char *target, const char *type, u32 opt, void *data)
{
    struct filesystem *fstype;
    struct FsCore *super;
    struct Object *targetobj, *tmpobj;
    char *notfind;

    fstype = __findtype(type);
    if(!fstype)
    {
        debug_printf("fs","mount failed(cannot find type \"%s\")", type);
        return (-1);
    }

    targetobj = obj_matchpath(target, &notfind);
    if(notfind)
    {
#if 0
        // δ�ҵ���װ��
        targetobj = obj_BuildTempPath(targetobj, notfind); // ������װ��
#else
        return (-1); // ��װ�����׼���á�
#endif
    }
    else
    {
        super = malloc(sizeof(*super));
        memset(super, 0, sizeof(*super));
        super->pFsType = fstype;
        if(opt & MS_DIRECTMOUNT)
        {
            obj_SetPrivate(targetobj, (ptu32_t)super);
            super->pTarget = targetobj;
            super->MountBak = NULL;
            obj_DutyUp(targetobj);
        }
        else
        {
            tmpobj = obj_newprev(targetobj, __mount_ops, (ptu32_t)super, obj_name(targetobj));
            obj_detach(targetobj);
            super->pTarget = tmpobj;
            super->MountBak = targetobj;
            obj_DutyUp(tmpobj);
        }
        super->InstallWay = opt;
        super->Config = data;
    }

    return (0);
}
// ============================================================================
// ���ܣ��ļ�ϵͳע������
// ������
//      target -- �ļ�ϵͳ��Ҫ���ص�Ŀ�����Ŀ¼����
//      type -- �ļ�ϵͳ���ͣ�
// ���أ��ɹ���0����ʧ�ܣ�-1������δ���ص������ý����ȥ��-2����
// ��ע��
// ============================================================================
s32 unmountfs(const char *target, const char *type)
{
    struct filesystem *fstype;
    struct Object *targetobj;
    struct FsCore *super;
    char *notfind;

    fstype = __findtype(type);
    if(!fstype)
    {
        debug_printf("fs","mount failed(cannot find type \"%s\")", type);
        return (-1);
    }

    targetobj = obj_matchpath(target, &notfind);
    if(notfind)
    {
#if 0
        // δ�ҵ���װ��
        targetobj = obj_BuildTempPath(targetobj, notfind); // ������װ��
#else
        return (-1); // ��װ�����׼���á�
#endif
    }
    else
    {
        super = (struct FsCore *)obj_GetPrivate(targetobj);
        super->pFsType->uninstall(super->Config);
        memset(super, 0, sizeof(*super));
        free(super);
    }
    return (0);
}

//-----------------------------------------------------------------------------
//����: ��ʽ���ļ�ϵͳ
//����:
//����: -1 -- ��������; -2 -- �ļ�ϵͳ�����ļ����ڱ�ʹ��; -3 -- ��ʽ��ʧ��;
//      0 -- �ɹ�;
//��ע:
//-----------------------------------------------------------------------------
s32 Format(const char *MountPath)
{
    DIR *dir;
    s32 res = 0;
    struct objhandle *hdl;
    struct Object *ob;
    struct FsCore *super;
    struct filesystem *pFsType;

    if(!MountPath)
        return (-1);

    dir = opendir(MountPath);
    if(!dir)
        return (-1);
    hdl = (struct objhandle*)(dir->__fd); // Ŀ¼��������
    ob = hdl->HostObj;      // Ŀ¼�Ľڵ�
    if(!obj_isMount(ob))
        return (-1);
    super = (struct FsCore*)ob->ObjPrivate;
    pFsType = super->pFsType;
    res = pFsType->format(super->pCore);
    closedir(dir);

    return res;
}
// ============================================================================
// ���ܣ���ȡ�ļ�ϵͳ���󣨼��ϵ㣩�Ĺ�����
// ������ob -- �ļ�ϵͳ���󼯺ϣ�
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
void *corefs(struct Object *ob)
{
    struct FsCore *super;
    struct Object *parent;

    if(!ob)
        return (NULL);

//  if(!obj_isMount(ob)) // �����Ǽ��ϵ㣻
//  {
//      ob = obj_set(ob); // ��ȡ���󼯺ϣ�
//      if((!ob)&&(!obj_isMount(ob)))
//          return (NULL); // ���Ǽ��ϻ��߲����ڣ�
//  }
    parent = ob;
    while( (!obj_isMount(parent)) && (parent != obj_root()))
    {
        parent = obj_parent(parent);
    }
    super = (struct FsCore *)obj_GetPrivate(parent);
    if(!super)
        return (NULL);

    return (super->pCore);
}

// ============================================================================
// ���ܣ��ж��ļ�Ŀ���ļ�ϵͳ�Ƿ���ָ��media�����û��ָ��������ļ�ϵͳ��װ��source��ȥ
// ������source -- ��Ҫ�����ļ�ϵͳ��ý�壬ͨ����һ���豸����
//      target -- �ļ�ϵͳ��Ҫ���ص�Ŀ�����
// ���أ�
// ��ע��
// ============================================================================
void FsBeMedia(const char *source, const char *target)
{
    struct Object *srcobj,*targetobj;
    char *notfind;
    struct FsCore *super;
    s32 res;

    if((!target) || (!source))
    {
        return ;
    }
    srcobj = obj_matchpath(source, &notfind);
    if(notfind)
    {
        return ;
    }
    targetobj = obj_matchpath(target, &notfind);
    if(notfind)
    {
        return ;
    }
    super = (struct FsCore *)obj_GetPrivate(targetobj);
    if(!super->MediaDrv)
    {
        return ;        //���û���ҵ�ý�������򲻹����ļ�ϵͳ
    }
    res = super->pFsType->install(super,super->InstallWay,super->Config);  //����
    if(res == 0)
    {
        debug_printf("fs","file system \"%s\" installed on \"%s\".",targetobj->name,srcobj->name);
    }
    else
    {
        error_printf("fs","file system \"%s\" installed fail on \"%s\".",targetobj->name,srcobj->name);
    }

}

