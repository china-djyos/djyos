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

//����ģ��:�豸����ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:�ṩ�豸������
//����˵��:
//�޶���ʷ:
//3. ����:2014-06-14
//   ����:  ������.
//   �°汾�ţ�2.0.0
//   �޸�˵��: ���豸����ģ������ȫ���޸ģ������ṩ���������׽ӿڡ�
//2. ����:2009-03-03
//   ����:  ������.
//   �°汾�ţ�1.0.1
//   �޸�˵��: ������dev_close_left������dev_close_right�����Ĵ��󣬸�bug��
//             ����sniper�ύ
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <object.h>
#include <objhandle.h>
#include <pool.h>
#include <systime.h>
#include <multiplex.h>
#include <../../djyos/core/component_config_core.h>
#include <lock.h>
#include "dbug.h"
#include "../include/device.h"
#include "component_config_devfile.h"

static struct MemCellPool *__dev_struct_pool; // �豸���ƿ��ڴ��ͷָ��
static struct MutexLCB __dev_sys_lock; // �����豸��ȫ
static struct obj *__dev_sys_root; // TODO ����object

struct __device
{
    fntDevOpen dOpen; //
    fntDevClose dClose; //
    fntDevWrite dWrite; //
    fntDevRead dRead; //
    fntDevCntl dCntl; //
    ptu32_t DrvTag;        // ���豸�����������õı�ǩ
    ptu32_t UserTag;       // Ӧ�ó������õı�ǩ
};

// ============================================================================
// ���ܣ��Ƿ�Ϊд��ɷ��ز���
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ���1��д��ɷ��أ���0����д��ɷ��أ�
// ��ע�������ж��豸�Ĳ���ģʽ
// ============================================================================
inline s32 isbc(u32 flags)
{
    if(flags & O_DBC)
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ��Ƿ���������������ļ�����
// ������had -- ���ж��豸����ľ����
// ���أ�-1 �������� 1����RAW���� 0������RAW����
// ��ע�������ж��豸�Ĳ���ģʽ
// ============================================================================
inline s32 israw(struct objhandle *hdl)
{
    if(!hdl)
        return (-1);

    if(handle_mode(hdl) & O_DRAW)
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ����豸��
// ������ob -- �豸����
//       flags -- ��ģʽ��
//       timeout -- ��ʱʱ�䣻
// ���أ��ɹ����豸����������ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
static struct objhandle *__devopen(struct obj *ob, u32 flags, u32 timeout)
{
    struct objhandle *devfile;
    struct __device *dev;
    s32 res;

    devfile = handle_new();
    if(!devfile)
        return (NULL);

    handle_init(devfile, ob, flags, 0);
    dev = (struct __device*)obj_GetPrivate(ob);
    if(dev)
    {
        if(dev->dOpen)
        {
            res = dev->dOpen(devfile, flags, timeout);
            if(res)
            {
                handle_free(devfile);
                debug_printf("device","cannot open device \"%s\".", obj_name(ob));
                return (NULL);
            }
        }
    }
    else
    {
        if(obj_isset(ob))
        {
            // ��������"/dev"
            if(!test_directory(flags))
            {
                handle_free(devfile);
                return (NULL); // ֻ�а�װ����Ŀ¼�����������ļ�
            }
        }
        else
        {
            debug_printf("device","open device \"%s\" unknown(type).", obj_name(ob));
        }
    }

    return (devfile);
}

// ============================================================================
// ���ܣ��ر��豸��
// ������hdl -- �豸��������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 __devclose(struct objhandle *hdl)
{
    s32 res = 0;
    struct __device *dev = (struct __device*)handle_val(hdl);

    if(dev && dev->dClose)
        res = (s32)dev->dClose(hdl);

    if(!res)
        handle_free(hdl);

    return (res);
}

// ============================================================================
// ���ܣ�д�豸��
// ������hdl -- �豸��������
//      data -- ���ݣ�
//      len -- ���ݴ�С��
// ���أ�
// ��ע��
// ============================================================================
static s32 __devwrite(struct objhandle *hdl, u8 *data, u32 len)
{
    u32 res = 0;
    struct __device *dev = (struct __device*)handle_val(hdl);

    if(dev&&dev->dWrite)
    {
        res = dev->dWrite(hdl, data, len, 0, handle_timeout(hdl));
    }

    return (res);
}

// ============================================================================
// ���ܣ����豸��
// ������hdl -- �豸��������
//      data -- ���ݣ�
//      len -- ���ݴ�С��
// ��ע��
// ============================================================================
static s32 __devread(struct objhandle *hdl, u8 *data, u32 len)
{
    s32 res = 0;
    struct __device *dev = (struct __device*)handle_val(hdl);

    if(dev && dev->dRead)
    {
        res = dev->dRead(hdl, data, len, 0, handle_timeout(hdl));
    }

    return (res);
}

// ============================================================================
// ���ܣ����豸�����е��豸��
// ������hdl -- �豸���������
//      dentry -- Ŀ¼�
// ���أ�ȫ�����꣨1������ȡȡһ��Ŀ¼�0��
// ��ע��
// ============================================================================
static s32 __devdentry(struct objhandle *hdl, struct dirent *dentry)
{
    if(!dentry->d_ino)
        dentry->d_ino = (long)handle2obj(hdl);

    dentry->d_ino = (long)obj_foreach_child(handle2obj(hdl), (struct obj*)(dentry->d_ino));
    if(dentry->d_ino)
    {
        strcpy(dentry->d_name, obj_name((struct obj*)(dentry->d_ino)));
        dentry->d_reclen = strlen(dentry->d_name);
        if(obj_isset((struct obj*)dentry->d_ino))
            dentry->d_type |= S_IFDIR;
        else
            dentry->d_type |= S_IFREG;

        return (0);
    }

    return (1);
}

// ============================================================================
// ���ܣ��豸���ƣ�
// ������hdl -- �豸��������
//      cmd -- �������
//      arg0�� arg0 -- ���������
// ���أ��ɾ����������
// ��ע��
// ============================================================================
static s32 __devcntl(struct objhandle *hdl, u32 cmd, ptu32_t arg0, ptu32_t arg1)
{
    struct __device *dev;
    s32 ret = 0;

    switch(cmd)
    {
        case F_SETTIMEOUT:
        {
            handle_settimeout(hdl, arg0); // arg0 = timeout;
            break;
        }

        case F_GETTIMEOUT:
        {
            u32 *timeout = (u32*)arg0;
            *timeout = handle_timeout(hdl);
            break;
        }

        case F_GETDRVTAG:
        {
            ptu32_t *drv = (ptu32_t*)arg0;
            *drv = dev_GetDrvTag(Handle2fd(hdl));
            break;
        }

        case F_GETUSERTAG:
        {
            ptu32_t *usr = (ptu32_t*)arg0;
            *usr = dev_GetUserTag(Handle2fd(hdl));
            break;
        }

        case F_SETDRVTAG:
        {
            dev_SetDrvTag(Handle2fd(hdl),arg0);
            break;
        }

        case F_SETUSERTAG:
        {
            dev_SetUserTag(Handle2fd(hdl),arg0);
            break;
        }

        default:
            dev = (struct __device*)handle_val(hdl);
            if(dev && dev->dCntl)
                ret = dev->dCntl(hdl, cmd, arg0, arg1);
    }

    return (ret);
}

// ============================================================================
// ���ܣ��豸��ѯ��
// ������ob -- �豸����
//      data -- �豸��Ϣ��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 __devstat(struct obj *ob, struct stat *data)
{
    struct __device *dev;

    memset(data, 0x0, sizeof(*data));
    dev = (struct __device*)obj_GetPrivate(ob);
    if(dev)
    {
        // TODO�������FLASH�ȿ��豸���������basic�߼����ƿ��豸�߼���
        data->st_mode = S_IFCHR; // �豸�ļ������Ϊ�ַ����㣩���ͣ�
        data->st_dev = (dev_t)dev;
    }
    else
    {
        if(obj_isset(ob))
            data->st_mode = S_IFDIR; // "/dev"�ڵ�ΪĿ¼
        else
            printf("\r\n: dbug : device : stat device \"%s\" unknown(type).", obj_name(ob));
    }

    return (0);
}

// ============================================================================
// ���ܣ��豸������
// ������
// ���أ�
// ��ע��
// ============================================================================
static ptu32_t Dev_DevObjOps(enum objops ops, ptu32_t o_hdl, ptu32_t args, ...)
{
    va_list list;

    switch(ops)
    {
        case OBJOPEN:
        {
            struct obj *ob = (struct obj*)o_hdl;
            u32 flags = (u32)args;
            u32 notFind;

            va_start(list, args);
            notFind = va_arg(list, u32);
            va_end(list);

            if(notFind) // �豸���Ƕ��Ѿ������˵ģ������Ҳ���
                return (0);

            if(test_onlycreat(flags)) // dev�豸�����½���������Ŀ¼
                return (0);

            return ((ptu32_t)__devopen(ob, flags, 0));
        }

        case OBJCLOSE:
        {
            struct objhandle *devfile = (struct objhandle*)o_hdl;
            return ((ptu32_t)__devclose(devfile));
        }

        case OBJCHILDS: // TODO : ����ǲ��ǿ��Կ��ǲ���������
        {
            struct objhandle *devfile = (struct objhandle*)o_hdl;
            struct dirent *ret = (struct dirent*)args;

            return(__devdentry(devfile, ret));
        }

        case OBJREAD:
        {
            u32 len;
            struct objhandle *devfile = (struct objhandle*)o_hdl;
            u8 *buf = (u8*)args;

            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__devread(devfile, buf, len));
        }

        case OBJWRITE:
        {
            u32 len;
            struct objhandle *devfile = (struct objhandle*)o_hdl;
            u8 *buf = (u8*)args;

            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__devwrite(devfile, buf, len));
        }

        case OBJSTAT:
        {
            char *path;
            struct obj *ob = (struct obj*)o_hdl;
            struct stat *ret = (struct stat*)args;

            va_start(list, args);
            path = (char *)va_arg(list, u32);
            va_end(list);

            if(path&&('\0'!=*path))
                return (-1); // �豸�Ķ��󲻴��ڣ������ѻ���ģ���

            return((ptu32_t)__devstat(ob, ret));
        }

        case OBJIOCTL:
        {
            ptu32_t arg0, arg1;
            struct objhandle *devfile = (struct objhandle*)o_hdl;
            u32 cmd = (u32)args;

            va_start(list, args);
            arg0 = (ptu32_t)va_arg(list, u32);
            list = *((va_list*)va_arg(list, u32)); // ���ﴫ�����ı��¾��ǲ����ĵ�ַ
            //list = (va_list)va_arg(list, va_list);
            //list = *((va_list*)va_arg(list, u32)); // ���ﴫ�����ı��¾��ǲ����ĵ�ַ
            arg1 = (ptu32_t)va_arg(list, u32);
            va_end(list);

            return ((ptu32_t)__devcntl(devfile, cmd, arg0, arg1));
        }

        case OBJCTL:
        {
            ptu32_t arg0, arg1;
            struct objhandle *devfile = (struct objhandle*)o_hdl;
            u32 cmd = (u32)args;

            va_start(list, args);
            list = (va_list)va_arg(list, va_list);
            //list = *((va_list*)va_arg(list, u32)); // ���ﴫ�����ı��¾��ǲ����ĵ�ַ
            arg0 = (ptu32_t)va_arg(list, u32);
            arg1 = (ptu32_t)va_arg(list, u32);
            va_end(list);

            return ((ptu32_t)__devcntl(devfile, cmd, arg0, arg1));
        }

        default:
        {
            break;
        }
    }

    return (-1);
}

// ============================================================================
// ���ܣ������Ƿ����豸���ࣩ��
// ������
// ���أ��ǣ�1������0����
// ��ע��
// ============================================================================
static s32 __isdev(struct obj *dev)
{
    dev = dev;
    return (1);
}

// ============================================================================
// ���ܣ������豸��������ǩ��
// ������fd -- �豸�ļ���������
//       DrvTag --�豸��������ǩ��
// ���أ�
// ��ע��
// ============================================================================
void dev_SetDrvTag(s32 fd,ptu32_t DrvTag)
{
    struct __device *dev;
    struct objhandle *hdl;

    if(fd >= 0)
    {
        hdl = fd2Handle(fd);
        dev = (struct __device *)handle_val(hdl);
        dev->DrvTag = DrvTag;
    }

    return ;
}

// ============================================================================
// ���ܣ������豸���û���ǩ
// ��������fd -- �豸�ļ���������
//         DrvTag --�豸���û���ǩ��
// ���أ��û���ǩ
// ��ע��
// ============================================================================
ptu32_t dev_SetUserTag(s32 fd,ptu32_t UserTag)
{
    struct __device *dev;
    struct objhandle *hdl;

    if(fd >= 0)
    {
        hdl = fd2Handle(fd);
        dev = (struct __device *)handle_val(hdl);
        dev->UserTag = UserTag;
    }

    return (0);
}

// ============================================================================
// ���ܣ���ȡ�豸��������ǩ��
// ������fd -- �豸�ļ���������
// ���أ�������ǩ��
// ��ע��
// ============================================================================
ptu32_t dev_GetDrvTag(s32 fd)
{
    struct __device *dev;
    struct objhandle *hdl;

    if(fd >= 0)
    {
        hdl = fd2Handle(fd);
        dev = (struct __device *)handle_val(hdl);
        return (dev->DrvTag);
    }

    return (0);
}

// ============================================================================
// ���ܣ���ȡ�豸���û���ǩ
// ������fd -- �豸�ļ���������
// ���أ��û���ǩ
// ��ע��
// ============================================================================
ptu32_t dev_GetUserTag(s32 fd)
{
    struct __device *dev;
    struct objhandle *hdl;

    if(fd >= 0)
    {
        hdl = fd2Handle(fd);
        dev = (struct __device *)handle_val(hdl);
        return (dev->UserTag);
    }

    return (0);
}

// ============================================================================
// ���ܣ�����豸����
// ������name -- �豸��������
// ���أ��ɹ����豸���ࣩ��ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
inline struct obj *dev_group_addo(char *name)
{
#if 0
    struct obj *ob;
    struct __portBasic *group;

    group = malloc(sizeof(*group));
    if(!group)
        return(NULL);

    handle_initbasic(group, O_RDWR);
    ob = of_virtualizeo(__dev_sys_root, group, name);
    if(!ob)
    {
        return (NULL);
    }

    return (ob);
#else
    struct obj *ob;

    ob = obj_newchild(__dev_sys_root, Dev_DevObjOps, O_RDWR, 0, name);
    if(!ob)
        return (NULL);

    if(obj_allow2set(ob)) // ��������Ͻ�������(��)��
    {
        obj_del(ob);
        return (NULL);
    }

    if(obj_change2set(ob)) // ������תΪ���ϣ��ࣩ��
    {
        obj_del(ob);
        return (NULL);
    }

    return (ob);
#endif
}

// ============================================================================
// ���ܣ�����豸����
// ������name -- �豸��������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 dev_group_add(char *name)
{
    if(dev_group_addo(name))
        return (0);

    return (-1);
}

// ============================================================================
// ���ܣ�ɾ���豸����
// ������grp -- �豸����ڵ㣻
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
inline s32 dev_group_delo(struct obj *grp)
{
    struct obj *dev0, *devx;
    u8 end = 0;
    s32 res = 0;

    if((!grp)
       ||(!__isdev(grp)))
        return (-1);

    dev0 = obj_child(grp);
    do
    {
        devx = obj_next(dev0);
        if(devx == dev0)
            end = 1; // ���һ����Ա��

        if(obj_isonduty(devx))
            res = -1; // �豸����ʹ���У��޷�ɾ��
        else
            obj_del(devx);
    }
    while(!end);

    if(!res)
    {
        if(obj_isonduty(grp))
        {
            obj_del(grp);
        }
        else
        {
            res = -1; // ��Ŀ¼����ʹ���У��޷�ɾ��
        }
    }

    return (res);
}

// ============================================================================
// ���ܣ�ɾ���豸����
// ������name -- �豸��������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 dev_group_del(char *name)
{
    char *left, *tmp;
    u8 index = 0;
    struct obj *ob;

    if(name)
    {
        // �ж�group��ʽ��ƴ��������/dev·��
        if('/'==name[0])
        {
            index = 1;
        }

        if(('d'==name[index])&&('e'==name[index+1])
           &&('v'==name[index+2])&&('/'==name[index+3]))
        {
            tmp = name; // group�Ѿ�������dev/
        }
        else
        {
            tmp = malloc(strlen(name)+4);
            if(!tmp)
            {
                printf("\r\n: warn : device : memory out(%s).", tmp);
                return (-1);
            }

            sprintf(tmp, "%s/%s", "dev/", name);
        }

        ob = obj_matchpath(tmp, &left, NULL);
        if((!left) || (ob))
        {
            free(tmp);
            return (-1);
        }

        free(tmp);
    }

    return (dev_group_delo(ob));
}

// ============================================================================
// ���ܣ����һ���豸�����豸�ࣻ
// ������grp -- �豸���ࣻΪNULL�����ʾ����dev�����ࣻ
//      name -- �豸����
// ���أ��ɹ����豸���󣩣�ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
inline struct obj *dev_addo(struct obj *grpo, const char *name,
                               fntDevOpen dopen, fntDevClose dclose,
                               fntDevWrite dwrite, fntDevRead dread,
                               fntDevCntl dcntl, ptu32_t DrvTag)
{
#if 0
    struct device *device;
    struct obj *devo;

    if(!grpo)
        grpo = __dev_sys_root;

    device = malloc(sizeof(*device));
    if(!device)
    {
        return (NULL);
    }

    device->dev = (struct __device*)Mb_Malloc(__dev_struct_pool, 0);
    if(!device->dev)
    {
        free(device);
        return (NULL);
    }

    handle_initbasic(&device->basic, O_RDWR);
    devo = of_virtualizeo(grpo, &device->basic, (char*)name);
    if(!devo)
    {
        free(device->dev);
        free(device);
        return (NULL);
    }

    obj_setval(devo, (ptu32_t)device->dev);
    device->dev->dClose = dclose;
    device->dev->dOpen = dopen;
    device->dev->dCntl = dcntl;
    device->dev->dRead = dread;
    device->dev->dWrite = dwrite;
    device->dev->drv = dtag;

    return (devo);
#else
    struct obj *devo;
    struct __device *dev;

    if(!grpo)
    {
        grpo = __dev_sys_root;
    }
    else
    {
        if((grpo!=__dev_sys_root)
           &&(obj_parent(grpo)!=__dev_sys_root)) // �߼����豸�����ĸ�������dev�����ƶ�������ȣ�
            return (NULL);
    }

    dev = (struct __device*)Mb_Malloc(__dev_struct_pool, 0);
    if(!dev)
        return (NULL);

    devo = obj_newchild(grpo, NULL, O_RDWR, (ptu32_t)dev, name);
    if(!devo)
    {
        Mb_Free(__dev_struct_pool, dev);
        return (NULL);
    }

    dev->dClose = dclose;
    dev->dOpen = dopen;
    dev->dCntl = dcntl;
    dev->dRead = dread;
    dev->dWrite = dwrite;
    dev->DrvTag = DrvTag;
    return (devo);
#endif
}

// ============================================================================
// ���ܣ����һ���豸�����豸�ࣻ
// ������grp -- �豸�ࣻ
//       name -- �豸����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 dev_add(const char *grp, const char *name, fntDevOpen dopen, fntDevClose dclose,
            fntDevWrite dwrite, fntDevRead dread, fntDevCntl dcntl, ptu32_t dtag)
{
    struct obj *ob = __dev_sys_root;
    char *left, *tmp;
    u8 index = 0;

    if(grp)
    {
        // �ж�group��ʽ��ƴ��������/dev·��
        if('/' == grp[0])
        {
            index = 1;
        }

        if(('d' == grp[index]) && ('e' == grp[index+1]) &&
           ('v' == grp[index+2]) && ('/' == grp[index+3]))
        {
            tmp = (char*)grp; // group�Ѿ�������dev/
        }
        else
        {
            tmp = malloc(strlen(grp)+6);
            if(!tmp)
            {
                warning_printf("device","memory out(%s).", tmp);
                return (-1);
            }

            sprintf(tmp, "%s/%s", "/dev", grp);
        }

        ob = obj_matchpath(tmp, &left, NULL);
        if(left||(!ob))
        {
            free(tmp);
            return (-1);
        }

        free(tmp);
    }

    if(dev_addo(ob, name, dopen, dclose, dwrite, dread, dcntl, dtag))
        return (0);

    return (-1);
}

// ============================================================================
// ���ܣ�ɾ���豸����
// ������devo -- �豸�ڵ㣻
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע�����ļ�ϵͳ�ӿ�Ҫ�γɻ��⣻��Ҫ����ɾ���Ĺ����г�����ʹ�ã�
// ============================================================================
inline s32 dev_delo(struct obj *devo)
{
    // struct device *device;

    if((!devo)
       ||(obj_isonduty(devo)) // �豸��ʹ����
       ||(!__isdev(devo))) // �����豸����
        return (-1);

    // TODO������ɾ���豸���ƣ�
    Mb_Free(__dev_struct_pool, (void*)obj_GetPrivate(devo));
    obj_del(devo);
    return (0);
}

// ============================================================================
// ���ܣ�ɾ���豸��
// ������grp -- �豸���ࣻ
//      name -- �豸����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 dev_del(const char *grp, const char *name)
{
    char *left, *tmp;
    struct obj *ob = __dev_sys_root;
    u8 index = 0;

    if(grp)
    {
        // �ж�group��ʽ��ƴ��������/dev·��
        if('/' == grp[0])
        {
            index = 1;
        }

        if(('d' == grp[index]) && ('e' == grp[index+1]) &&
           ('v' == grp[index+2]) && ('/' == grp[index+3]))
        {
            tmp = (char*)grp; // group�Ѿ�������dev/
        }
        else
        {
            tmp = malloc(strlen(grp)+4);
            if(!tmp)
            {
                warning_printf("device","memory out(%s).", tmp);
                return (-1);
            }

            sprintf(tmp, "%s/%s", "dev/", grp);
        }

        ob = obj_matchpath(tmp, &left, NULL);
        if((!left) || (ob))
        {
            free(tmp);
            return (-1);
        }

        free(tmp);
    }

    ob = obj_search_child(ob, name); // �����豸�ڵ㣻
    if(!ob)
        return (-1);

    return(dev_delo(ob));
}

// ============================================================================
// ���ܣ��豸������豸��ǩ��
// ������devo -- �豸����
// ���أ�
// ��ע��TODO: INLINE
// ============================================================================
ptu32_t devo2drv(struct obj *devo)
{
    struct __device *dev;

    if((!devo)||(!__isdev(devo)))
        return (0);

    dev = (struct __device*)obj_GetPrivate(devo);
    if(dev)
        return (dev->DrvTag);

    return (0);
}

// ============================================================================
// ���ܣ���ȡ�豸���������
// ������devo -- �豸����ָ��
// ���أ��豸���������
// ��ע��
// ============================================================================
inline const char *devo2name(struct obj *devo)
{
    if((devo)&&(__isdev(devo)))
        return (obj_name(devo));

    return (NULL);
}

// ============================================================================
// ���ܣ��豸�ļ�ϵͳ��װ
// ������
// ���أ�
// ��ע��TODO
// ============================================================================
s32 mount_device_system(void)
{
    u8 *mem;

    if(__dev_sys_root || __dev_struct_pool)
    {
        debug_printf("device","device file system has been installed.");
        return (-1);
    }

    mem = M_Malloc(CFG_DEVFILE_LIMIT * sizeof(struct __device), 0x1);
    if(!mem)
    {
        debug_printf("device","cannot install device file system(memory out).");
        return (-1);
    }

    __dev_struct_pool = Mb_CreatePool((void*)mem,
                                   CFG_DEVFILE_LIMIT,
                                   sizeof(struct __device),
                                   0, 0,
                                   "�豸���ƿ��");
    if(!__dev_struct_pool)
    {
        debug_printf("device","cannot install device file system(pool).");
        return (-1);
    }

    Lock_MutexCreate_s(&__dev_sys_lock, "dev");
    __dev_sys_root = obj_newchild_set(objsys_root(), "dev", Dev_DevObjOps, 0, O_RDWR);
    if(!__dev_sys_root)
    {
        debug_printf("device","cannot install device file system(mount).");
        return (-1);
    }

    return (0);
}

// ============================================================================
// ���ܣ������豸�����豸�����������豸��Դ�����ҵ�������nameƥ�����Դ��㣬
//      ���ظþ��ָ�롣���̻߳��ⷽʽ����:
//      A�߳���D_RDWRģʽ��,------�����̲߳����ٴ�
//      A�߳���D_RDONLYģʽ��,----�����߳�(����A)��������D_WRONLYģʽ��
//      D_WRONLYͬ��
// ������name,�豸���ַ���,����·�����������ذ���'dev\'��4���ַ�
//      flags,�豸��ģʽ,D_RDONLY,D_WRONLY,D_RDWR�е�һ��
//      timeout����ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//          ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
// ���أ��ɹ����豸����������,���򷵻�-1.
// ��ע��O_RDONLY��O_WRONLY��O_RDWR �����POSIX��2016�������¶���
// ============================================================================
s32 DevOpen(const char *name, s32 flags, u32 timeout)
{
    struct obj *dev;
    struct objhandle *devfile = NULL;

    if(!Lock_MutexPend(&__dev_sys_lock, timeout))
        return (-1);

    dev = obj_search_path(__dev_sys_root, name); // ���豸Ŀ¼�������豸
    if(dev)
    {
        devfile = __devopen(dev, (flags|O_DRAW), timeout);
    }

    Lock_MutexPost(&__dev_sys_lock);
    if(!devfile)
        return (-1);

    return Handle2fd(devfile);
}

// ============================================================================
// ���ܣ��ر��豸�������豸�ܻ�������������ֻ���豸ӵ����(�����豸���¼�)���ܹ�
//      ���豸�������ɻ��������ص�����ġ�
// ������fd -- �豸�ļ�������
// ���أ��ɹ���TRUE����ʧ�ܣ�FALSE����
// ��ע��
// ============================================================================
s32 DevClose(s32 fd)
{
#if 0
    struct __device * dev;
    struct objhandle* devFile;
    struct obj *ob;
    s32 Mode;

    devFile = fd2Handle( dwFD );
    if(devFile == NULL)
        return false;
    ob = handle2obj(devFile);
    dev = (struct __device *)OBJ_GetPrivate(ob);
    Mode = (s32)KF_GetFileContext(devFile);

    if(Mode & O_RDONLY)
    {
        Lock_MutexPost(dev->dReadMutex );
    }

    if(Mode & O_WRONLY)
    {
        Lock_MutexPost(dev->dWriteMutex );
    }
    OBJ_DecUseOne(ob);
    return true;
#else

    s32 res;
    struct objhandle *devfile;

    devfile = fd2Handle(fd);
    if(!devfile)
        return (-1);

    res = __devclose(devfile);
    if(res)
        return (-1);

    // TODO: δ��һ
    return (0);
#endif
}

// ============================================================================
// ���ܣ����豸��ȡ����.
// ������fd -- �豸������
//       buf -- ���ڽ������ݵĻ����������������벻С��len��buf�������޷���顣
//       offset -- ��ȡλ�����豸�е�ƫ�ƣ��������豸(���紮��)��˵��ͨ����0.
//       timeout -- ��ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//          ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
//      ���ϲ�����driverģ��ֻ����ʵ���͸������豸���豸������Ҳ���Բ���������
//      �涨��
// ���أ�ʵ�ʶ�������������
// ��ע��
// ============================================================================
s32 DevRead(s32 fd, void *buf, u32 len, u32 offset, u32 timeout)
{
    struct __device *dev;
    struct objhandle *devfile;

    if((!len) || (!buf))
        return (0);

    devfile = fd2Handle(fd);
    if(!devfile)
        return (0);

    dev = (struct __device*)handle_val(devfile);
    if(dev && dev->dRead)
        return (dev->dRead(devfile, buf, len, offset, timeout));

    return (0);

}

// ============================================================================
// ���ܣ�������д���豸.
// ������fd -- �豸������
//       buf -- ���������ݵĻ�������
//       len -- ���͵�������
//       offset -- д��λ�����豸�е�ƫ�ƣ��������豸(���紮��)��˵��ͨ����0.
//       timeout -- ��ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//          ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
// ���أ�ʵ��д������������
// ��ע��
// ============================================================================
s32 DevWrite(s32 fd, void *buf, u32 len, u32 offset, u32 timeout)
{
    struct __device * dev;
    struct objhandle *devfile;

    if((!len) || (!buf))
        return (0);

    devfile = fd2Handle(fd);
    if(!devfile)
        return (0);

    dev = (struct __device*)handle_val(devfile);
    if(dev&&dev->dWrite)
        return (dev->dWrite(devfile, buf, len, offset, timeout));

    return (0);
}

// ============================================================================
// ���ܣ������豸.
// ������fd -- �豸ָ��
//       dwCmd -- ����ţ�driver.h��Ԥ������һЩ����Լ��û������ʹ�ù��򣬿ɲ���
//          ����CN_DEV_CTRL_START���帽�����롣
//       data1��data2 -- ������������Ҫ�Ĳ��������豸�����߶���
// ���أ�-1��ʾ��������������ֵ���豸�����߶���
// ��ע��
// ============================================================================
s32 DevCntl(s32 fd, u32 dwCmd, ptu32_t data1, ptu32_t data2)
{
    struct __device *dev;
    struct objhandle *devfile;

    devfile = fd2Handle(fd);
    if(!devfile)
        return (-1);

    dev = (struct __device*)handle_val(devfile);
    if(dev && dev->dCntl)
        return (dev->dCntl(devfile, dwCmd, data1, data2));

    return (-1);
}
