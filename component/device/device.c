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
#include <filesystems.h>
#include <../../djyos/core/component_config_core.h>
#include <lock.h>
#include "dbug.h"
#include "../include/device.h"
#include "component_config_devfile.h"

struct __device
{
    fnDevOpen dOpen; //
    fnDevClose dClose; //
    fnDevWrite dWrite; //
    fnDevRead dRead; //
    fnDevCntl dCntl; //
    ptu32_t DrvTag;        // ���豸�����������õı�ǩ
    ptu32_t UserTag;       // Ӧ�ó������õı�ǩ
};

struct Object *s_ptDeviceRoot; // TODO ����object
static struct MemCellPool *s_ptDevicePool; // �豸���ƿ��ڴ��ͷָ��
static struct __device s_tDeviceInitPool[CFG_DEVFILE_LIMIT];
static struct MutexLCB s_tDeviceMutex; // �����豸��ȫ
static struct objhandle *s_ptDeviceDirHandle;

// ============================================================================
// ���ܣ����Ա�־�Ƿ�����ɲ���д���
// ������flags -- �ļ�ʹ�ñ�־��
// ���أ���1��д�����ڴ�����ɺ󷵻أ���0��д������д��buffer�����أ�
// ============================================================================
s32 handle_IsBlockComplete(u32 flags)
{
    if(flags & O_BLOCK_COMPLETE)
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
static struct objhandle *__Device_open(struct Object *ob, u32 flags, u32 timeout)
{
    struct objhandle *devfile;
    struct __device *dev;
    s32 res;

    devfile = Handle_New();
    if(!devfile)
        return (NULL);

    Handle_Init(devfile, ob, flags, 0);
    dev = (struct __device*)OBJ_GetPrivate(ob);
    if(dev)
    {
        if(dev->dOpen)
        {
            res = dev->dOpen(devfile, flags, timeout);
            if(res)
            {
                Handle_Delete(devfile);
                debug_printf("device","cannot open device \"%s\".", OBJ_GetName(ob));
                return (NULL);
            }
//            OBJ_LinkHandle(devfile, ob);      //��objhandle��obj����
            return (devfile);
        }
        else
            return NULL;
    }
    else
    {
        if(File_ObjIsMount(ob))
        {
            // ��������"/dev"
            if(!Handle_FlagIsDirectory(flags))
            {
                Handle_Delete(devfile);
                return (NULL); // ֻ�а�װ����Ŀ¼�����������ļ�
            }
        }
        else
        {
            debug_printf("device","open device \"%s\" unknown(type).", OBJ_GetName(ob));
        }
    }

    return NULL;
}

// ============================================================================
// ���ܣ��ر��豸��
// ������hdl -- �豸��������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 __Device_close(struct objhandle *hdl)
{
    s32 res = 0;
    struct __device *dev = (struct __device*)handle_GetHostObjectPrivate(hdl);

    if(dev && dev->dClose)
        res = (s32)dev->dClose(hdl);

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
static s32 __Device_write(struct objhandle *hdl, u8 *data, u32 len)
{
    u32 res = 0;
    struct __device *dev = (struct __device*)handle_GetHostObjectPrivate(hdl);

    if(dev&&dev->dWrite)
    {
        res = dev->dWrite(hdl, data, len, 0, handle_gettimeout(hdl));
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
static ssize_t __Device_read(struct objhandle *hdl, u8 *data, ssize_t len)
{
    s32 res = 0;
    struct __device *dev = (struct __device*)handle_GetHostObjectPrivate(hdl);

    if(dev && dev->dRead)
    {
        res = dev->dRead(hdl, data, len, 0, handle_gettimeout(hdl));
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
static s32 __Device_dentry(struct objhandle *hdl, struct dirent *dentry)
{
    if(!dentry->d_ino)
        dentry->d_ino = (long)Handle_GetHostObj(hdl);

    dentry->d_ino = (long)OBJ_ForeachChild(Handle_GetHostObj(hdl), (struct Object*)(dentry->d_ino));
    if(dentry->d_ino)
    {
        strcpy(dentry->d_name, OBJ_GetName((struct Object*)(dentry->d_ino)));
        dentry->d_reclen = strlen(dentry->d_name);
        if(File_ObjIsMount((struct Object*)dentry->d_ino))
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
static s32 __Device_cntl(struct objhandle *hdl, u32 cmd, va_list *arg0)
{
    struct __device *dev;
    s32 ret = 0;

    switch(cmd)
    {
        case F_SETTIMEOUT:
        {
            u32 timeout;
            timeout = va_arg(*arg0, u32);
            Handle_SetTimeOut(hdl, timeout);
            break;
        }

        case F_GETTIMEOUT:
        {
            u32 *timeout;
            timeout = va_arg(*arg0, u32 *);
            *timeout = handle_gettimeout(hdl);
            break;
        }

        case F_GETDRVTAG:
        {
            ptu32_t *drv;
            drv = va_arg(*arg0, ptu32_t *);
            *drv = Device_GetDrvTag(Handle2fd(hdl));
            break;
        }

        case F_GETUSERTAG:
        {
            ptu32_t *usr;
            usr = va_arg(*arg0, ptu32_t *);
            *usr = Device_GetUserTag(Handle2fd(hdl));
            break;
        }

        case F_SETDRVTAG:
        {
            ptu32_t drv;
            drv = va_arg(*arg0, ptu32_t);
            Device_SetDrvTag(Handle2fd(hdl),drv);
            break;
        }

        case F_SETUSERTAG:
        {
            ptu32_t user;
            user = va_arg(*arg0, ptu32_t);
            Device_SetUserTag(Handle2fd(hdl),user);
            break;
        }

        default:
            dev = (struct __device*)handle_GetHostObjectPrivate(hdl);
            if(dev && dev->dCntl)
                ret = dev->dCntl(hdl, cmd, arg0);
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
static s32 __Device_stat(struct Object *ob, struct stat *data)
{
    struct __device *dev;

    memset(data, 0x0, sizeof(*data));
    dev = (struct __device*)OBJ_GetPrivate(ob);
    if(dev)
    {
        data->st_mode = S_IFFLOW|S_IRUGO|S_IWUGO; // �豸�ļ�������ʽ���ݣ�
        data->st_dev = (dev_t)dev;
    }
    else
    {
        return -1;
    }

    return (0);
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----�豸Ŀ¼��������---------------------------------------------------------
//���ܣ�ʵ�ִ򿪺Ͷ�Ŀ¼�Ĺ��ܡ�
//������Target���ο�����ָ������ fnObjOps �Ķ���
//      cmd�������룬�μ� CN_OBJ_CMD_OPEN �嶨��
//      para1��para2�������������
//���أ������������
//-----------------------------------------------------------------------------
s32 Device_DirObjOps(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                         ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    s32 result = CN_OBJ_CMD_EXECUTED;
    switch(objcmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            OBJ_DutyUp((struct Object*)opsTarget);
            *(struct objhandle **)OpsArgs1 = s_ptDeviceDirHandle;
        }break;
        case CN_OBJ_CMD_READDIR:
        {
            DIR *dir = (DIR*)OpsArgs1;
            struct dirent *direntry;
            if(dir->__fd == NULL)
                dir->__fd = (struct Object*)opsTarget;
            dir->__fd = OBJ_ForeachChild((struct Object*)opsTarget,dir->__fd);
            if(dir->__fd == NULL)
                result = 1;
            else
            {
                direntry = &dir->__ptr;
                strcpy(direntry->d_name, OBJ_GetName((struct Object *)(dir->__fd)));
                direntry->d_reclen = strlen(direntry->d_name);
                direntry->d_type |= S_IFREG;
                result = 0;
            }
        }break;
        default:
            result = CN_OBJ_CMD_UNSUPPORT;
            break;
    }
    return result;
}
#pragma GCC diagnostic pop

// ============================================================================
// ���ܣ��豸������
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 Device_DevObjOps(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                         ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    s32 result = CN_OBJ_CMD_EXECUTED;

    switch(objcmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct objhandle *hdl;
            if((char*)OpsArgs3 != NULL)     // �豸���Ƕ��Ѿ������˵ģ�opsTarget��Ӧ����NULL
            {
                *(struct objhandle **)OpsArgs1 = NULL;
                break;
            }
            if(Handle_FlagIsOnlyCreate((u32)(*(u64*)OpsArgs2)))       // dev�豸�����½���������Ŀ¼
            {
                *(struct objhandle **)OpsArgs1 = NULL;
                break;
            }
            hdl = __Device_open((struct Object *)opsTarget, (u32)(*(u64*)OpsArgs2), CN_TIMEOUT_FOREVER);
            *(struct objhandle **)OpsArgs1 = hdl;
            break;
        }

//      case OBJ_TRAVERSE: // TODO : ����ǲ��ǿ��Կ��ǲ���������
//      {
//          struct objhandle *devfile = (struct objhandle*)opsTarget;
//          struct dirent *ret = (struct dirent*)OpsArgs1;
//
//          return(__Device_dentry(devfile, ret));
//      }

        case CN_OBJ_CMD_READ:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = __Device_read(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_WRITE:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = __Device_write(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_CLOSE:
        {
            __Device_close((struct objhandle*)opsTarget);
            result = CN_OBJ_CMD_TRUE;
            break;
        }

        case CN_OBJ_CMD_STAT:
        {
            char * path = (char*)OpsArgs2;
            if(path&&('\0'!=*path))
                return (-1);        // ��ѯ���ļ������ڣ�
            if(__Device_stat((struct Object*)opsTarget, (struct stat *)OpsArgs1) == 0)
            {
                result = CN_OBJ_CMD_TRUE;
            }
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_FCNTL:
        case CN_OBJ_IOCTL:
        {
//          ptu32_t arg0, arg1;
//          struct objhandle *devfile = (struct objhandle*)opsTarget;
//          u32 cmd = (u32)OpsArgs1;
//
//          va_start(list, OpsArgs1);
//          list = *((va_list*)va_arg(list, u32)); // ���ﴫ�����ı��¾��ǲ����ĵ�ַ
//          arg0 = (ptu32_t)va_arg(list, u32);
//          arg1 = (ptu32_t)va_arg(list, u32);
//          va_end(list);

            *(s32*)OpsArgs1 = __Device_cntl((struct objhandle*)opsTarget, OpsArgs2,
                                                (va_list *)OpsArgs3);
            break;
        }

        default:
        {
            return CN_OBJ_CMD_UNSUPPORT;
            break;
        }
    }

    return result;
}

// ============================================================================
// ���ܣ������豸��������ǩ��
// ������fd -- �豸�ļ���������
//       DrvTag --�豸��������ǩ��
// ���أ�
// ��ע��
// ============================================================================
void Device_SetDrvTag(s32 fd,ptu32_t DrvTag)
{
    struct __device *dev;
    struct objhandle *hdl;

    if(fd >= 0)
    {
        hdl = fd2Handle(fd);
        dev = (struct __device *)handle_GetHostObjectPrivate(hdl);
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
ptu32_t Device_SetUserTag(s32 fd,ptu32_t UserTag)
{
    struct __device *dev;
    struct objhandle *hdl;

    if(fd >= 0)
    {
        hdl = fd2Handle(fd);
        dev = (struct __device *)handle_GetHostObjectPrivate(hdl);
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
ptu32_t Device_GetDrvTag(s32 fd)
{
    struct __device *dev;
    struct objhandle *hdl;

    if(fd >= 0)
    {
        hdl = fd2Handle(fd);
        dev = (struct __device *)handle_GetHostObjectPrivate(hdl);
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
ptu32_t Device_GetUserTag(s32 fd)
{
    struct __device *dev;
    struct objhandle *hdl;

    if(fd >= 0)
    {
        hdl = fd2Handle(fd);
        dev = (struct __device *)handle_GetHostObjectPrivate(hdl);
        return (dev->UserTag);
    }

    return (0);
}

//// ============================================================================
//// ���ܣ�����豸����
//// ������name -- �豸��������
//// ���أ��ɹ����豸���ࣩ��ʧ�ܣ�NULL����
//// ��ע��
//// ============================================================================
//struct Object *dev_group_addo(char *name)
//{
//#if 0
//    struct Object *ob;
//    struct __portBasic *group;
//
//    group = malloc(sizeof(*group));
//    if(!group)
//        return(NULL);
//
//    handle_initbasic(group, O_RDWR);
//    ob = of_virtualizeo(s_ptDeviceRoot, group, name);
//    if(!ob)
//    {
//        return (NULL);
//    }
//
//    return (ob);
//#else
//    struct Object *ob;
//
//    ob = OBJ_NewChild(s_ptDeviceRoot, Device_DevObjOps, 0, name);
//    if(!ob)
//        return (NULL);
//
//    if(obj_allow2set(ob)) // ��������Ͻ�������(��)��
//    {
//        OBJ_Delete(ob);
//        return (NULL);
//    }
//
//    if(obj_change2set(ob)) // ������תΪ���ϣ��ࣩ��
//    {
//        OBJ_Delete(ob);
//        return (NULL);
//    }
//
//    return (ob);
//#endif
//}

//// ============================================================================
//// ���ܣ�ɾ���豸����
//// ������grp -- �豸����ڵ㣻
//// ���أ��ɹ���0����ʧ�ܣ�-1����
//// ��ע��
//// ============================================================================
//s32 dev_group_delo(struct Object *grp)
//{
//    struct Object *dev0, *devx;
//    u8 end = 0;
//    s32 res = 0;
//
//    if((!grp)
//       ||(!__isdev(grp)))
//        return (-1);
//
//    dev0 = OBJ_GetChild(grp);
//    do
//    {
//        devx = OBJ_GetNext(dev0);
//        if(devx == dev0)
//            end = 1; // ���һ����Ա��
//
//        if(OBJ_IsOnDuty(devx))
//            res = -1; // �豸����ʹ���У��޷�ɾ��
//        else
//            OBJ_Delete(devx);
//    }
//    while(!end);
//
//    if(!res)
//    {
//        if(OBJ_IsOnDuty(grp))
//        {
//            OBJ_Delete(grp);
//        }
//        else
//        {
//            res = -1; // ��Ŀ¼����ʹ���У��޷�ɾ��
//        }
//    }
//
//    return (res);
//}

//// ============================================================================
//// ���ܣ�ɾ���豸����
//// ������name -- �豸��������
//// ���أ��ɹ���0����ʧ�ܣ�-1����
//// ��ע��
//// ============================================================================
//s32 dev_group_del(char *name)
//{
//    char *left, *tmp;
//    u8 index = 0;
//    struct Object *ob;
//
//    if(name)
//    {
//        // �ж�group��ʽ��ƴ��������/dev·��
//        if('/'==name[0])
//        {
//            index = 1;
//        }
//
//        if(('d'==name[index])&&('e'==name[index+1])
//           &&('v'==name[index+2])&&('/'==name[index+3]))
//        {
//            tmp = name; // group�Ѿ�������dev/
//        }
//        else
//        {
//            tmp = malloc(strlen(name)+4);
//            if(!tmp)
//            {
//                printf("\r\n: warn : device : memory out(%s).", tmp);
//                return (-1);
//            }
//
//            sprintf(tmp, "%s/%s", "dev/", name);
//        }
//
//        ob = OBJ_MatchPath(tmp, &left, NULL);
//        if((!left) || (ob))
//        {
//            free(tmp);
//            return (-1);
//        }
//
//        free(tmp);
//    }
//
//    return (dev_group_delo(ob));
//}

struct Object *Device_Create(const char *name,fnDevOpen dopen, fnDevClose dclose,
                               fnDevWrite dwrite, fnDevRead dread,
                               fnDevCntl dcntl, ptu32_t DrvTag)
{

   struct Object *devo;
   struct __device *dev;


   dev = (struct __device*)Mb_Malloc(s_ptDevicePool, 0);
   if(!dev)
       return (NULL);

   devo = OBJ_NewChild(s_ptDeviceRoot, Device_DevObjOps, (ptu32_t)dev, name);
   if(!devo)
   {
       Mb_Free(s_ptDevicePool, dev);
       return (NULL);
   }

   dev->dClose = dclose;
   dev->dOpen = dopen;
   dev->dCntl = dcntl;
   dev->dRead = dread;
   dev->dWrite = dwrite;
   dev->DrvTag = DrvTag;
   return (devo);
}

//// ============================================================================
//// ���ܣ����һ���豸�����豸�ࣻ
//// ������grp -- �豸���ࣻΪNULL�����ʾ����dev�����ࣻ
////      name -- �豸����
//// ���أ��ɹ����豸���󣩣�ʧ�ܣ�NULL����
//// ��ע��
//// ============================================================================
//struct Object *dev_addo(struct Object *grpo, const char *name,
//                               fnDevOpen dopen, fnDevClose dclose,
//                               fnDevWrite dwrite, fnDevRead dread,
//                               fnDevCntl dcntl, ptu32_t DrvTag)
//{
//
//    struct Object *devo;
//    struct __device *dev;
//
//    if(!grpo)
//    {
//        grpo = s_ptDeviceRoot;
//    }
//    else
//    {
//        if((grpo!=s_ptDeviceRoot)
//           &&(OBJ_GetParent(grpo)!=s_ptDeviceRoot)) // �߼����豸�����ĸ�������dev�����ƶ�������ȣ�
//            return (NULL);
//    }
//
//    dev = (struct __device*)Mb_Malloc(s_ptDevicePool, 0);
//    if(!dev)
//        return (NULL);
//
//    devo = OBJ_NewChild(grpo, NULL, (ptu32_t)dev, name);
//    if(!devo)
//    {
//        Mb_Free(s_ptDevicePool, dev);
//        return (NULL);
//    }
//
//    dev->dClose = dclose;
//    dev->dOpen = dopen;
//    dev->dCntl = dcntl;
//    dev->dRead = dread;
//    dev->dWrite = dwrite;
//    dev->DrvTag = DrvTag;
//    return (devo);
//}

//// ============================================================================
//// ���ܣ����һ���豸�����豸�ࣻ
//// ������grp -- �豸�ࣻ
////       name -- �豸����
//// ���أ��ɹ���0����ʧ�ܣ�-1����
//// ��ע��
//// ============================================================================
//s32 dev_add(const char *grp, const char *name, fnDevOpen dopen, fnDevClose dclose,
//            fnDevWrite dwrite, fnDevRead dread, fnDevCntl dcntl, ptu32_t dtag)
//{
//    struct Object *ob = s_ptDeviceRoot;
//    char *left, *tmp;
//    u8 index = 0;
//
//    if(grp)
//    {
//        // �ж�group��ʽ��ƴ��������/dev·��
//        if('/' == grp[0])
//        {
//            index = 1;
//        }
//
//        if(('d' == grp[index]) && ('e' == grp[index+1]) &&
//           ('v' == grp[index+2]) && ('/' == grp[index+3]))
//        {
//            tmp = (char*)grp; // group�Ѿ�������dev/
//        }
//        else
//        {
//            tmp = malloc(strlen(grp)+6);
//            if(!tmp)
//            {
//                warning_printf("device","memory out(%s).", tmp);
//                return (-1);
//            }
//
//            sprintf(tmp, "%s/%s", "/dev", grp);
//        }
//
//        ob = OBJ_MatchPath(tmp, &left, NULL);
//        if(left||(!ob))
//        {
//            free(tmp);
//            return (-1);
//        }
//
//        free(tmp);
//    }
//
//    if(dev_addo(ob, name, dopen, dclose, dwrite, dread, dcntl, dtag))
//        return (0);
//
//    return (-1);
//}

// ============================================================================
// ���ܣ�ɾ���豸����
// ������devo -- �豸�ڵ㣻
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע�����ļ�ϵͳ�ӿ�Ҫ�γɻ��⣻��Ҫ����ɾ���Ĺ����г�����ʹ�ã�
// ============================================================================
s32 Device_DeleteAtObject(struct Object *dev)
{
    // struct device *device;

    if((!dev) ||(OBJ_IsOnDuty(dev))) // �豸��ʹ����
        return (-1);

    Mb_Free(s_ptDevicePool, (void*)OBJ_GetPrivate(dev));
    OBJ_Delete(dev);
    return (0);
}

// ============================================================================
// ���ܣ�ɾ���豸��
// ������grp -- �豸���ࣻ
//      name -- �豸����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 Device_DeleteByName(const char *name)
{
    struct Object *ob;

    ob = OBJ_SearchChild(s_ptDeviceRoot, name); // �����豸�ڵ㣻
    if(!ob)
        return (-1);

    return(Device_DeleteAtObject(ob));
}

//// ============================================================================
//// ���ܣ�ȡ����ȡ�豸��������ǩ��
//// ������devo -- �豸����
//// ���أ�struct __device�� DrvTag ��Ա��������-1�����û��Է���-1Ϊ��������Ҫ��������Ϊ
////      DrvTag ����ʵֵҲ������-1.
//// ��ע������Է� Dev �� object ���ô˺������������Ԥ�ϡ�
//// ============================================================================
//ptu32_t dev_GetDrvTagFromObj(struct obj *devboj)
//{
//    struct __device *dev;
//
//    if(!devboj)
//        return (0);
//
//    dev = (struct __device*)OBJ_GetPrivate(devboj);
//    if(dev)
//        return (dev->DrvTag);
//    else
//        return (0);
//}

// ============================================================================
// ���ܣ���ȡ�豸���������
// ������devo -- �豸����ָ��
// ���أ��豸���������
// ��ע��
// ============================================================================
const char *Device_GetName(struct Object *devo)
{
    if(devo)
        return (OBJ_GetName(devo));

    return (NULL);
}

// ============================================================================
// ���ܣ���ʼ���豸�ļ�ϵͳ
// ������
// ���أ�
// ��ע��TODO
// ============================================================================
s32 ModuleInstall_dev(void)
{
    s_ptDevicePool = Mb_CreatePool((void*)s_tDeviceInitPool,
                                   CFG_DEVFILE_LIMIT,
                                   sizeof(struct __device),
                                   0, 0,
                                   "�豸���ƿ��");
    if(!s_ptDevicePool)
    {
        debug_printf("device","cannot install device file system(pool).");
        return (-1);
    }

    Lock_MutexCreate_s(&s_tDeviceMutex, "dev");
    s_ptDeviceRoot = OBJ_NewChild(OBJ_GetRoot(), Device_DirObjOps, 0, "dev");
    s_ptDeviceDirHandle = Handle_New();
    if((NULL == s_ptDeviceRoot) ||(NULL == s_ptDeviceDirHandle))
    {
        OBJ_Delete(s_ptDeviceRoot);
        Handle_Delete(s_ptDeviceDirHandle);
        debug_printf("device","cannot install device file system(mount).");
        return (-1);
    }
    else
    {
        Handle_Init(s_ptDeviceDirHandle,s_ptDeviceRoot,O_DIRECTORY,0);
        OBJ_LinkHandle(s_ptDeviceDirHandle,s_ptDeviceRoot);
        return (0);
    }
}

// ============================================================================
// ���ܣ������豸�����豸�����������豸��Դ�����ҵ�������nameƥ�����Դ��㣬
//      ���ظþ��ָ�롣���̻߳��ⷽʽ����:
//      A�߳���D_RDWRģʽ��,------�����̲߳����ٴ�
//      A�߳���D_RDONLYģʽ��,----�����߳�(����A)��������D_WRONLYģʽ��
//      D_WRONLYͬ��
// ������name,�豸���ַ���,����·�����������ذ���'dev\'��4���ַ�
//      flags,�豸��ģʽ,O_RDONLY,O_WRONLY,O_RDWR �е�һ��
//      timeout����ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//          ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
// ���أ��ɹ����豸����������,���򷵻�-1.
// ��ע��O_RDONLY��O_WRONLY��O_RDWR �����POSIX��2016�������¶���
// ============================================================================
s32 Device_Open(const char *name, s32 flags, u32 timeout)
{
    struct Object *dev;
    struct objhandle *devfile = NULL;

    if(!Lock_MutexPend(&s_tDeviceMutex, timeout))
        return (-1);

    dev = OBJ_SearchPath(s_ptDeviceRoot, name); // ���豸Ŀ¼�������豸
    if(dev)
    {
        devfile = __Device_open(dev, flags, timeout);
    }

    Lock_MutexPost(&s_tDeviceMutex);
    if(!devfile)
        return (-1);

    return Handle2fd(devfile);
}

// ============================================================================
// ���ܣ��ر��豸�������豸�ܻ�������������ֻ���豸ӵ����(�����豸���¼�)���ܹ�
//      ���豸�������ɻ��������ص�����ġ�
// ������fd -- �豸�ļ�������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 Device_Close(s32 fd)
{
#if 0
    struct __device * dev;
    struct objhandle* devFile;
    struct Object *ob;
    s32 Mode;

    devFile = fd2Handle( dwFD );
    if(devFile == NULL)
        return false;
    ob = Handle_GetHostObj(devFile);
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

    res = __Device_close(devfile);

    //ע�⣺��socketһ����dev�豸�ļ����Լ��Ĺرպ�����û�о�������ϵͳ��close���ã�����Ҫ
    //�Լ�ɾ���������fat��yaffs2��efs�ȾͲ����Լ�ɾ����
    if(res == 0)
        Handle_Delete(devfile);

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
s32 Device_Read(s32 fd, void *buf, u32 len, u32 offset, u32 timeout)
{
    struct __device *dev;
    struct objhandle *devfile;

    if((!len) || (!buf))
        return (0);

    devfile = fd2Handle(fd);
    if(!devfile)
        return (0);

    dev = (struct __device*)handle_GetHostObjectPrivate(devfile);
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
s32 Device_Write(s32 fd, void *buf, u32 len, u32 offset, u32 timeout)
{
    struct __device * dev;
    struct objhandle *devfile;

    if((!len) || (!buf))
        return (0);

    devfile = fd2Handle(fd);
    if(!devfile)
        return (0);

    dev = (struct __device*)handle_GetHostObjectPrivate(devfile);
    if(dev&&dev->dWrite)
        return (dev->dWrite(devfile, buf, len, offset, timeout));

    return (0);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ============================================================================
// ���ܣ������豸.
// ������fd -- �豸ָ��
//       dwCmd -- ����ţ�driver.h��Ԥ������һЩ����Լ��û������ʹ�ù��򣬿ɲ���
//          ���� CN_DEV_CTRL_START ���帽�����롣
//       data1��data2 -- ������������Ҫ�Ĳ��������豸�����߶���
// ���أ�-1��ʾ��������������ֵ���豸�����߶���
// ��ע��
// ============================================================================
s32 Device_Ctrl(s32 fd, u32 dwCmd, ptu32_t data1, ptu32_t data2)
{
//    struct __device *dev;
//    struct objhandle *devfile;
//
//    devfile = fd2Handle(fd);
//    if(!devfile)
//        return (-1);
//
//    dev = (struct __device*)handle_GetHostObjectPrivate(devfile);
//    if(dev && dev->dCntl)
//        return (dev->dCntl(devfile, dwCmd, data1, data2));
//
    return (-1);
}
#pragma GCC diagnostic pop
