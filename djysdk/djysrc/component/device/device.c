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
#include <objfile.h>
#include <pool.h>
#include <systime.h>
#include <multiplex.h>
#include <../../djyos/core/component_config_core.h>
#include <lock.h>
#include "dbug.h"
#include "../include/device.h"
#include "component_config_devfile.h"

static struct MemCellPool *pDevPool; // �豸���ƿ��ڴ��ͷָ��
static struct MutexLCB s_tDevMutex; // �����豸��ȫ
struct Object *devRoot; // TODO ����object


struct __device
{
    fntDevOpen dOpen; //
    fntDevClose dClose; //
    fntDevWrite dWrite; //
    fntDevRead dRead; //
    fntDevCntl dCntl; //
    ptu32_t dTag; // ���豸���������ݣ��ڴ����豸ʱ����������
    ptu32_t uTag; // �û��������ݣ��û�ʹ�ù���������
};


struct device{
    struct __portBasic basic;
    struct __device *dev;   //lst : �� pObj->represent �ظ�����
};

// ============================================================================
// ���ܣ��Ƿ�Block Complete����
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ�Ŀ¼��1������Ŀ¼��0����
// ��ע�������豸�Ĳ���ģʽ
// ============================================================================
inline s32 isbc(u32 flags)
{
    if(flags & O_DBC)
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ��Ƿ�RAW����
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ�Ŀ¼��1������Ŀ¼��0�����Ƿ���-1����
// ��ע�������豸�Ĳ���ģʽ
// ============================================================================
inline s32 israw(tagOFile *devof)
{
    if(!devof)
        return (-1);

    if(of_mode(devof) & O_DRAW)
        return (1);

    return (0);
}

// ============================================================================
// ���ܣ����豸�ļ���
// ������
// ���أ�
// ��ע��
// ============================================================================
tagOFile *__devfileopen(struct Object *obj, u32 flags, u32 timeout)
{
    tagOFile *devfile;
    struct __device *dev;
    s32 res;

    devfile = of_new();
    if(!devfile)
        return (NULL);

    of_init(devfile, obj, flags, 0);
    dev = (struct __device*)__OBJ_Represent(obj);
    if(dev)
    {
        if(dev->dOpen)
        {
            res = dev->dOpen(devfile, flags, timeout);
            if(res)
            {
                of_free(devfile);
                debug_printf("device","cannot open device \"%s\".", __OBJ_Name(obj));
                return (NULL);
            }
        }
    }
    else
    {
        if(GROUP_POINT == __OBJ_Type(obj))
        {
            // ��������"/dev"
            if(!testdirectory(flags))
            {
                of_free(devfile);
                return (NULL); // ֻ�а�װ����Ŀ¼�����������ļ�
            }
        }
        else
        {
            debug_printf("device","open device \"%s\" unknown(type).", __OBJ_Name(obj));
        }
    }

    return (devfile);
}

// ============================================================================
// ���ܣ��ر��豸�ļ���
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __devfileclose(tagOFile *devof)
{
    s32 res = 0;
    struct __device *dev = (struct __device*)of_represent(devof);

    if(dev && dev->dClose)
        res = (s32)dev->dClose(devof);

    if(!res)
        of_free(devof);

    return (res);
}

// ============================================================================
// ���ܣ�д�豸�ļ���
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __devfilewrite(tagOFile *devof, u8 *pBuf, u32 dwLen)
{
    u32 res = 0;
    struct __device *dev = (struct __device*)of_represent(devof);

    if(dev && dev->dWrite)
    {
        res = dev->dWrite(devof, pBuf, dwLen, 0, of_timeout(devof));
    }

    return (res);
}

// ============================================================================
// ���ܣ����豸�ļ���
// ������
// ���أ�
// ��ע��TODO
// ============================================================================
static s32 __devfileread(tagOFile *devof, u8 *pBuf, u32 dwLen)
{
    s32 res = 0;
    struct __device *dev = (struct __device*)of_represent(devof);

    if(dev && dev->dRead)
    {
        res = dev->dRead(devof, pBuf, dwLen, 0, of_timeout(devof));
    }

    return (res);
}

// ============================================================================
// ���ܣ����豸Ŀ¼�
// ������
// ���أ�ȫ�����꣨1������ȡȡһ��Ŀ¼�0��
// ��ע��
// ============================================================================
static s32 __devfiledentry(tagOFile *devof, struct dirent *pDirent)
{
    if(!pDirent->d_ino)
        pDirent->d_ino = (long)of_o(devof);

    pDirent->d_ino = (long)OBJ_TraveChild(of_o(devof), (struct Object*)(pDirent->d_ino));
    if(pDirent->d_ino)
    {
        strcpy(pDirent->d_name, __OBJ_Name((struct Object*)(pDirent->d_ino)));
        pDirent->d_reclen = strlen(pDirent->d_name);
        pDirent->d_type |= S_IFREG;
        return (0);
    }

    return (1);
}

// ============================================================================
// ���ܣ��豸�ļ����ƣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __devfilecntl(tagOFile *devof, u32 cmd, ptu32_t arg0, ptu32_t arg1)
{
    struct __device *dev;
    s32 care = 0, res = 0, ret = -1;

    switch(cmd)
    {
        case F_OF_SETTIMEOUT:
        {
            of_settimeout(devof, arg0); // arg0 = timeout;
            break;
        }

        case F_OF_GETTIMEOUT:
        {
            u32 *timeout = (u32*)arg0;
            *timeout = of_timeout(devof);
            break;
        }

        case F_GETDDTAG:
        {
            ptu32_t *dTag = (ptu32_t*)arg0;
            *dTag = devfiledtag(devof);
            break;
        }

        case F_GETDUTAG:
        {
            ptu32_t *uTag = (ptu32_t*)arg0;
            *uTag = devfileutag(devof);
            break;
        }

        default:break;
    }

    dev = (struct __device*)of_represent(devof);
    if(dev && dev->dCntl)
        ret = dev->dCntl(devof, cmd, arg0, arg1);

    if(care) // �Ƿ�����dCntl�ķ���ֵ
        res = ret;

    return (res);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 __devfilestat(struct Object *obj, struct stat *pStat)
{
    struct __device *dev;

    memset(pStat, 0x0, sizeof(*pStat));
    dev = (struct __device*)__OBJ_Represent(obj);
    if(dev)
    {
        // TODO�������FLASH�ȿ��豸���������basic�߼����ƿ��豸�߼���
        pStat->st_mode = S_IFCHR; // �豸�ļ������Ϊ�ַ����㣩���ͣ�
        pStat->st_dev = (dev_t)dev;
    }
    else
    {
        if(GROUP_POINT == __OBJ_Type(obj))
            pStat->st_mode = S_IFDIR; // "/dev"�ڵ�ΪĿ¼
        else
            debug_printf("device","stat device \"%s\" unknown(type).", __OBJ_Name(obj));
    }

    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static ptu32_t __devfileoperations(u32 cmd, ptu32_t oof, ptu32_t args, ...)
{
    va_list list;

    switch(cmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct Object *obj = (struct Object*)oof;
            u32 flags = (u32)args;
            u32 notFind;

            va_start(list, args);
            notFind = va_arg(list, u32);
            va_end(list);

            if(notFind) // �豸���Ƕ��Ѿ������˵ģ������Ҳ���
                return (0);

            if(testonlycreat(flags)) // dev�豸�����½���������Ŀ¼
                return (0);

            return ((ptu32_t)__devfileopen(obj, flags, 0));
        }

        case CN_OBJ_CMD_CLOSE:
        {
            tagOFile *devfile = (tagOFile*)oof;
            return ((ptu32_t)__devfileclose(devfile));
        }

        case CN_OBJ_CMD_READDIR: // TODO : ����ǲ��ǿ��Կ��ǲ���������
        {
            tagOFile *devfile = (tagOFile*)oof;
            struct dirent *ret = (struct dirent*)args;

            return(__devfiledentry(devfile, ret));
        }

        case CN_OBJ_CMD_READ:
        {
            u32 len;
            tagOFile *devfile = (tagOFile*)oof;
            u8 *buf = (u8*)args;

            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__devfileread(devfile, buf, len));
        }

        case CN_OBJ_CMD_WRITE:
        {
            u32 len;
            tagOFile *devfile = (tagOFile*)oof;
            u8 *buf = (u8*)args;

            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__devfilewrite(devfile, buf, len));
        }

        case CN_OBJ_CMD_STAT:
        {
            struct Object *obj = (struct Object*)oof;
            u32 notFind= (u32)args;
            struct stat *ret;

            va_start(list, args);
            ret = (struct stat*)va_arg(list, u32);
            va_end(list);

            if(notFind)
                return (-1);

            return((ptu32_t)__devfilestat(obj, ret));
        }

        default:
        {
            ptu32_t arg0, arg1;
            tagOFile *devfile = (tagOFile*)oof;
            va_list list = *((va_list*)args); // ���ﴫ�����ı��¾��ǲ����ĵ�ַ

            arg0 = (ptu32_t)va_arg(list, u32);
            arg1 = (ptu32_t)va_arg(list, u32);
            va_end(list);

            if((CN_OBJ_FCNTL_START <= cmd) && (cmd < CN_DEV_CTRL_START))
            {
                cmd -= CN_OBJ_FCNTL_START;
                return ((ptu32_t)__devfilecntl(devfile, cmd, arg0, arg1));
            }

            if(cmd >= CN_DEV_CTRL_START)
            {
                cmd -= CN_DEV_CTRL_START;
                return ((ptu32_t)__devfilecntl(devfile, cmd, arg0, arg1));
            }

            return ((ptu32_t)CN_OBJ_CMD_UNSUPPORT);
        }
    }

    return (-1);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
ptu32_t devfiledtag(tagOFile *of)
{
    struct __device *dev;

    if(of)
    {
        dev = (struct __device *)of_represent(of);
        return (dev->dTag);
    }

    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
ptu32_t devfileutag(tagOFile *of)
{
    struct __device *dev;

    if(of)
    {
        dev = (struct __device *)of_represent(of);
        return (dev->uTag);
    }

    return (0);
}

// ============================================================================
// ���ܣ�����豸����
// ������name -- �豸��������
// ���أ�
// ��ע��
// ============================================================================
inline struct Object *dev_grpaddo(char *name)
{
    struct Object *obj;
    struct __portBasic *group;

    group = malloc(sizeof(*group));
    if(!group)
        return(NULL);

    of_initbasic(group, O_RDWR);
    obj = of_virtualizeo(devRoot, group, name);
    if(!obj)
    {
        return (NULL);
    }

    return (obj);
}

// ============================================================================
// ���ܣ�����豸����
// ������name -- �豸��������
// ���أ�
// ��ע��
// ============================================================================
s32 dev_grpadd(char *name)
{
    if(dev_grpaddo(name))
        return (0);

    return (-1);

}

// ============================================================================
// ���ܣ�ɾ���豸����
// ������grp -- �豸����ڵ㣻
// ���أ�
// ��ע��
// ============================================================================
inline s32 dev_grpdelo(struct Object *grp)
{
    struct Object *dev0, *devx;
    u8 end = 0;
    s32 res = 0;

    if(!grp)
        return (-1);

    dev0 = OBJ_Child(grp);
    do
    {
        devx = OBJ_Next(dev0);
        if(devx == dev0)
            end = 1; // ���һ����Ա��

        if(isondutyo(devx))
            res = -1; // �豸����ʹ���У��޷�ɾ��
        else
            __OBJ_Del(devx);


    }
    while(!end);

    if(!res)
    {
        if(isondutyo(grp))
        {
            __OBJ_Del(grp);
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
// ������grp -- �豸��������
// ���أ�
// ��ע��
// ============================================================================
s32 dev_grpdel(char *name)
{
    char *left, *tmp;
    u8 index = 0;
    struct Object *obj;

    if(name)
    {
        // �ж�group��ʽ��ƴ��������/dev·��
        if('/' == name[0])
        {
            index = 1;
        }

        if(('d' == name[index]) && ('e' == name[index+1]) &&
           ('v' == name[index+2]) && ('/' == name[index+3]))
        {
            tmp = name; // group�Ѿ�������dev/
        }
        else
        {
            tmp = malloc(strlen(name)+4);
            if(!tmp)
            {
                warning_printf("device","memory out(%s).", tmp);
                return (-1);
            }

            sprintf(tmp, "%s/%s", "dev/", name);
        }

        obj = OBJ_MatchPath(tmp, &left, NULL);
        if((!left) || (obj))
        {
            free(tmp);
            return (-1);
        }

        free(tmp);
    }

    return (dev_grpdelo(obj));
}

// ============================================================================
// ���ܣ����һ���豸������Ŀ��
// ������grp -- �豸���ࣻΪNULL�����ʾ����devcie�����ࣻ
//      name -- �豸����
// ���أ�
// ��ע��
// ============================================================================
inline struct Object *dev_addo(struct Object *grpo, const char *name,
                               fntDevOpen dopen, fntDevClose dclose,
                               fntDevWrite dwrite, fntDevRead dread,
                               fntDevCntl dcntl, ptu32_t dtag)
{
    struct device *device;
    struct Object *devo;

    if(!grpo)
        grpo = devRoot;

    device = malloc(sizeof(*device));
    if(!device)
    {
        return (NULL);
    }

    device->dev = (struct __device*)Mb_Malloc(pDevPool, 0);
    if(!device->dev)
    {
        free(device);
        return (NULL);
    }

    of_initbasic(&device->basic, O_RDWR);
    devo = of_virtualizeo(grpo, &device->basic, (char*)name);
    if(!devo)
    {
        free(device->dev);
        free(device);
        return (NULL);
    }

    __OBJ_SetRepresent(devo, (ptu32_t)device->dev);
    device->dev->dClose = dclose;
    device->dev->dOpen = dopen;
    device->dev->dCntl = dcntl;
    device->dev->dRead = dread;
    device->dev->dWrite = dwrite;
    device->dev->dTag = dtag;

    return (devo);
}

// ============================================================================
// ���ܣ�
// ������group -- �豸��ӵ����豸�ࣻ
//      name -- �豸����
// ���أ�
// ��ע��
// ============================================================================
s32 dev_add(const char *grp, const char *name, fntDevOpen dopen, fntDevClose dclose,
                       fntDevWrite dwrite, fntDevRead dread,
                       fntDevCntl dcntl, ptu32_t dtag)
{
    struct Object *obj = devRoot;
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

        obj = OBJ_MatchPath(tmp, &left, NULL);
        if(left||(!obj))
        {
            free(tmp);
            return (-1);
        }

        free(tmp);
    }

    if(dev_addo(obj, name, dopen, dclose, dwrite, dread, dcntl, dtag))
        return (0);

    return (-1);
}

// ============================================================================
// ���ܣ�
// ������devo -- �豸�ڵ㣻
// ���أ�
// ��ע�����ļ�ϵͳ�ӿ�Ҫ�γɻ��⣻��Ҫ����ɾ���Ĺ����г�����ʹ�ã�
// ============================================================================
inline s32 dev_delo(struct Object *devo)
{
    struct device *device;

    if(!devo)
        return (-1);

    if(isondutyo(devo))
        return (-1); // �豸��ʹ����

    device = dListEntry(of_basiclinko(devo), struct device, basic);
    free(device->dev);
    free(device);
    __OBJ_Del(devo);

    return (0);
}

// ============================================================================
// ���ܣ�
// ������grp -- �豸���ࣻ
//      name -- �豸����
// ���أ�
// ��ע��
// ============================================================================
s32 dev_del(const char *grp, const char *name)
{
    char *left, *tmp;
    struct Object *obj = devRoot;
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

        obj = OBJ_MatchPath(tmp, &left, NULL);
        if((!left) || (obj))
        {
            free(tmp);
            return (-1);
        }

        free(tmp);
    }

    obj = OBJ_SearchChild(obj, name); // �����豸�ڵ㣻
    if(!obj)
        return (-1);

    return(dev_delo(obj));
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��TODO: INLINE
// ============================================================================
ptu32_t dev_dtago(struct Object *devo)
{
    struct __device *dev;

    if(!devo)
        return (0);

    dev = (struct __device*)__OBJ_Represent(devo);
    if(dev)
        return (dev->dTag);

    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
inline const char *dev_nameo(struct Object *devo)
{
    if(devo)
        return (__OBJ_Name(devo));

    return (0);
}

// ============================================================================
// ���ܣ��豸�ļ�ϵͳ��װ
// ������
// ���أ�
// ��ע��TODO
// ============================================================================
s32 __InstallDevFS(void)
{
    u8 *mem;

    if(devRoot || pDevPool)
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

    pDevPool = Mb_CreatePool((void*)mem,
    								CFG_DEVFILE_LIMIT,
                                   sizeof(struct __device),
                                   0, 0,
                                   "�豸���ƿ��");
    if(!pDevPool)
    {
        debug_printf("device","cannot install device file system(pool).");
        return (-1);
    }

    Lock_MutexCreate_s(&s_tDevMutex, "dev");
    devRoot = __mounto("dev", "/", (tagObjOps)__devfileoperations, NULL);
    if(!devRoot)
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
// ���أ��ɹ����豸�����豸���,���򷵻�-1.
// ��ע��O_RDONLY��O_WRONLY��O_RDWR �����POSIX��2016�������¶���
// ============================================================================
s32 DevOpen(const char *pName, s32 flags, u32 timeout)
{
    struct Object *dev;
    tagOFile *devfile = NULL;

    if(!Lock_MutexPend(&s_tDevMutex, timeout))
        return (-1);

    dev = OBJ_SearchChild(devRoot, pName); // ���豸Ŀ¼�������豸
    if(dev)
    {
        devfile = __devfileopen(dev, (flags|O_DRAW), timeout);
    }

    Lock_MutexPost(&s_tDevMutex);
    if(!devfile)
        return (-1);

    return ofno(devfile);
}

// ============================================================================
// ���ܣ��ر��豸�������豸�ܻ�������������ֻ���豸ӵ����(�����豸���¼�)���ܹ�
//      ���豸�������ɻ��������ص�����ġ�
// ������dwFD -- �豸�ļ����
// ���أ��ɹ���TRUE����ʧ�ܣ�FALSE����
// ��ע��
// ============================================================================
s32 DevClose(s32 dwFD)
{
#if 0
    struct __device * dev;
    tagOFile* devFile;
    struct Object *obj;
    s32 Mode;

    devFile = of( dwFD );
    if(devFile == NULL)
        return false;
    obj = of_o(devFile);
    dev = (struct __device *)OBJ_GetPrivate(obj);
    Mode = (s32)KF_GetFileContext(devFile);

    if(Mode & O_RDONLY)
    {
        Lock_MutexPost(dev->dReadMutex );
    }

    if(Mode & O_WRONLY)
    {
        Lock_MutexPost(dev->dWriteMutex );
    }
    OBJ_DecUseOne(obj);
    return true;
#else

    s32 res;
    tagOFile *devfile;

    devfile = of(dwFD);
    if(!devfile)
        return (-1);

    res = __devfileclose(devfile);
    if(res)
        return (-1);

    // TODO: δ��һ
    return (0);
#endif
}

// ============================================================================
// ���ܣ����豸��ȡ����.
// ������dev,�豸ָ��
//      pBuf,���ڽ������ݵĻ����������������벻С��len��buf�������޷���顣
//      dwLen,buf���������������ݽ�������������С��buf��ʵ��������
//      offset,��ȡλ�����豸�е�ƫ�ƣ��������豸(���紮��)��˵��ͨ����0.
//      timeout����ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//          ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
//      ���ϲ�����driverģ��ֻ����ʵ���͸������豸���豸������Ҳ���Բ���������
//      �涨��
// ���أ�ʵ�ʶ�������������
// ��ע��
// ============================================================================
s32 DevRead(s32 handle, void *buf, u32 len, u32 offset, u32 timeout)
{
    struct __device *dev;
    tagOFile *devfile;

    if((!len) || (!buf))
        return (0);

    devfile = of(handle);
    if(!devfile)
        return (0);

    dev = (struct __device*)of_represent(devfile);
    if(dev && dev->dRead)
        return (dev->dRead(devfile, buf, len, offset, timeout));

    return (0);

}

// ============================================================================
// ���ܣ�������д���豸.
// ������dwFD -- �豸ָ��
//      pBuf -- ���������ݵĻ�������
//      dwLen -- ���͵�������
//      dwOffset -- д��λ�����豸�е�ƫ�ƣ��������豸(���紮��)��˵��ͨ����0.
//      timeout -- ��ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//          ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
// ���أ�ʵ��д������������
// ��ע��
// ============================================================================
s32 DevWrite(s32 handle, void *buf, u32 len, u32 offset, u32 timeout)
{
    struct __device * dev;
    tagOFile *devfile;

    if((!len) || (!buf))
        return (0);

    devfile = of(handle);
    if(!devfile)
        return (0);

    dev = (struct __device*)of_represent(devfile);
    if(dev && dev->dWrite)
        return (dev->dWrite(devfile, buf, len, offset, timeout));

    return (0);

}

// ============================================================================
// ���ܣ������豸.
// ������dwFD,�豸ָ��
//      dwCmd,����ţ�driver.h��Ԥ������һЩ����Լ��û������ʹ�ù��򣬿ɲ���
//          ����CN_DEV_CTRL_START���帽�����롣
//      data1��data2,������������Ҫ�Ĳ��������豸�����߶���
// ���أ�-1��ʾ��������������ֵ���豸�����߶���
// ��ע��
// ============================================================================
s32 DevCntl(s32 dwFD, u32 dwCmd, ptu32_t data1, ptu32_t data2)
{
    struct __device *dev;
    tagOFile *devfile;

    devfile = of(dwFD);
    if(!devfile)
        return (-1);

    dev = (struct __device*)of_represent(devfile);
    if(dev && dev->dCntl)
        return (dev->dCntl(devfile, dwCmd, data1, data2));

    return (-1);
}
