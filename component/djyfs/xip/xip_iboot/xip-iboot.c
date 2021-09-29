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
//TODO: ֧�ֶ���ļ�����ô�Ϳ���֧��multiboot
#include <systime.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <device.h>
#include <dirent.h>
#include <djyos.h>
#include <list.h>
#include <stdarg.h>
#include <xip.h>
#include <device/unit_media.h>
#include <stdio.h>
#include <djyfs/filesystems.h>
#include "Iboot_info.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_XIP_IBOOT_FS(u32 opt, void *data);
//    ModuleInstall_XIP_IBOOT_FS(0,NULL);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****����������������ʼ
//component name:"xip iboot file system"//����iboot����������
//parent:"file system"//��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"file system"             //������������������������none����ʾ�������������
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_XIP_IBOOT_FILE_SYSTEM == false )
//#warning  " xip_iboot_file_system  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_XIP_IBOOT_FILE_SYSTEM    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure


u64 FileNowPos = 0;
//static const char *xip_iboot_target = "xip-iboot";
// �ײ�ӿں���
//
//extern s32 xip_fs_format(void *core);
static s32 xip_iboot_fs_install(struct FsCore *super, u32 opt, void *config);
s32 xip_iboot_ops(void *opsTarget, u32 cmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

__attribute__((weak)) s32 xip_fs_format(void *core)
{
    return 0;
}

//==========================================================================
// ���ܣ��ļ�����
// ������
// ���أ�
// ��ע��
// ============================================================================
static inline void xip_iboot_lock(struct __icore *core)
{
    Lock_MutexPend(core->lock, CN_TIMEOUT_FOREVER);
}

// ============================================================================
// ���ܣ��ļ�����
// ������
// ���أ�
// ��ע��
// ============================================================================
static inline void xip_iboot_unlock(struct __icore *core)
{
    Lock_MutexPost(core->lock);
}

// ============================================================================
// ���ܣ����ļ�
// ������ob -- �ļ�����
//      flags -- �ļ�������־λ��
//      uncached -- ���½����ļ����ƣ�
// ���أ��ɹ���xip-iboot�ļ��������ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
static struct objhandle *xip_iboot_open(struct Object *ob, u32 flags, char *uncached)
{
    struct objhandle *hdl = NULL;
    struct __icore *core = (struct __icore*)File_Core(ob);
//    mode_t mode;

    if((Handle_FlagIsWriteable(flags) == 0) || (Handle_FlagIsDirectory(flags) == 1))
    {
        printf("\r\n: error :cannot open the xip-iboot directory and the files in it only support write mode.");
        return (NULL);    //xip-ibootֻ֧��дģʽ
    }

    xip_iboot_lock(core);

//    if(strcmp(OBJ_GetName(ob),EN_XIP_IBOOT_TARGET) == 0)      //�жϷ��ʵ�·���ǲ���xip-iboot�����������ֱ�ӷ���NULL
//    {
        xip_fs_format(core);        //����iboot���ڵ�flash����

//        if(!OBJ_NewChild(core->root, xip_iboot_ops, (ptu32_t)0, uncached))
//        {
//            xip_iboot_unlock(core);
//            return (NULL);
//        }

        hdl = Handle_New();
        if(!hdl)
        {
            return (NULL);
        }

        Handle_Init(hdl, NULL, flags, 0);

        if(hdl)
        {
            //TODO����yaffs2�ж�ȡȨ�޵ȣ���ʱ����ȫ��Ȩ�ޡ�
//            mode = S_IALLUGO | S_IFREG;     //������·����������Ŀ¼��
            //�̳в��������������˽�г�Ա�������ģʽ���� stat �� st_mode ��
            ob = OBJ_BuildTempPath(ob, xip_iboot_ops, (ptu32_t)0,uncached);
            OBJ_LinkHandle(hdl, ob);
        }
//    }
    xip_iboot_unlock(core);
    return (hdl);
}

// ============================================================================
// ���ܣ��ر��ļ�
// ������hdl -- xip-iboot�ļ���������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 xip_iboot_close(struct objhandle *hdl)
{
    FileNowPos = 0;
//  Handle_Delete(hdl);
    return (0);
}

// ============================================================================
// ���ܣ�д�ļ�
// ������hdl -- xip-iboot�ļ���������
//      data -- ��д�����ݵĿռ䣻
//      size -- ��д�����ݵĴ�С��
// ���أ�ʵ��д����ֽ�����
// ��ע��
// ============================================================================
static s32 xip_iboot_write(struct objhandle *hdl, u8 *data, u32 size)
{
    s32 res;
    struct __icore *core = (struct __icore*)File_Core(Handle_GetHostObj(hdl));
    xip_iboot_lock(core);
    res = core->drv->xip_write_media(core,data,size,FileNowPos);
    if(-1 == res)
    {
        xip_iboot_unlock(core);
        return (-1);
    }
    if(-2 == res)
    {
        xip_iboot_unlock(core);
        printf("\r\n: dbug : xip_iboot : storage space full.");
        return (-1);
    }
    FileNowPos += size;
    xip_iboot_unlock(core);
    return (size);
}

// ============================================================================
// ���ܣ���ѯxip-iboot״̬��
// ������ob -- stdio����
//      data -- stdio״̬��
// ���أ�
// ��ע��
// ============================================================================
static s32 xip_iboot_stat(struct Object *ob, struct stat *data)
{
    memset(data, 0x0, sizeof(struct stat));

    data->st_mode = S_IFREG|S_IRUGO|S_IWUGO;

    return (0);
}
#pragma GCC diagnostic pop

// ============================================================================
// ���ܣ���ʼ��xip��media������
// ������core -- efs�ļ�ϵͳ����  drv -- media�Ĳ���������
// ���أ�0 -- �ɹ���  -1 --ʧ��
// ��ע��
// ============================================================================
int xip_iboot_install_drv(struct __icore *core, struct __xip_drv *drv)
{
    if (!core || !drv)
         return (-1);
    core->drv = malloc(sizeof(*drv));

    core->drv->xip_erase_media = drv->xip_erase_media;
    core->drv->xip_read_media = drv->xip_read_media;
    core->drv->xip_write_media = drv->xip_write_media;
    return (0);
}
// ============================================================================
// ���ܣ���װxip�ļ�ϵͳ
// ������super -- �ļ�ϵͳ������Ϣ��
//      opt -- ��װ�߼���־λ��
//      config -- �Զ������ݣ�
// ���أ��ɹ�(0)�� ʧ��(-1)��
// ��ע��
// ============================================================================
static s32 xip_iboot_fs_install(struct FsCore *super, u32 opt, void *config)
{

    struct __icore *core;
    struct umedia *um;
    u32 flash_page_size;
    config = config;
    opt = opt;
    core = malloc(sizeof(*core));
    if(!core)
    {
        printf("\r\n: erro : xipfs  : install failed(memory out).");
        return (-1);
    }

    memset(core, 0x0, sizeof(*core));
    um = (struct umedia*)super->MediaInfo;
    if(!um)
    {
        free(core);
        return (-1);
    }
    um->mreq(unitbytes,(ptu32_t)&flash_page_size);
    xip_iboot_install_drv(core,super->MediaDrv);
    core->ASize = super->AreaSize;
    core->MStart = super->MediaStart;
    core->vol = (void*)um;
    core->bufsz = (s16)flash_page_size; // xip�ļ�ϵͳ�ļ��Ļ����С����unit�ĳߴ磻
    core->inhead = 0;  //inhead�ⲿ��Ϊ�������ݵĴ�С
    core->root = super->pTarget;
    core->lock = Lock_MutexCreate("xip-iboot�� fs");
    if(!core->lock)
    {
        free(core);
        return (-1);
    }

    super->pCore = (void*)core;
    return (0);
}

// ============================================================================
// ���ܣ�xip�ļ������ӿ�
// ��������׼�߼����鿴�ӿ�˵����
// ���أ���׼�߼����鿴�ӿ�˵����
// ��ע:
// ============================================================================
s32 xip_iboot_ops(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    s32 result = CN_OBJ_CMD_EXECUTED;
    switch(objcmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct objhandle *hdl;
            hdl = xip_iboot_open((struct Object *)opsTarget, (u32)(*(u64*)OpsArgs2), (char*)OpsArgs3);
            *(struct objhandle **)OpsArgs1 = hdl;
            break;
        }

        case CN_OBJ_CMD_WRITE:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = xip_iboot_write(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_CLOSE:
        {
            if(xip_iboot_close((struct objhandle*)opsTarget) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }
        case CN_OBJ_CMD_STAT:
        {
            char * path = (char*)OpsArgs2;
            if(path&&('\0'!=*path))
                return (-1); // ��ѯ���ļ������ڣ�
            if(xip_iboot_stat((struct Object*)opsTarget, (struct stat *)OpsArgs1) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
            break;
        }
        default:
        {
            result = CN_OBJ_CMD_UNSUPPORT;
            break;
        }
    }

    return result;
}
// ============================================================================
// ���ܣ���װxip�ļ�ϵͳ
// ������ opt -- �ļ�ϵͳ����ѡ���MS_INSTALLCREAT
//      data -- ���ݸ�xip-iboot��װ�߼������ݣ�
// ���أ�ʧ��(-1)�� �ɹ�(0)��
// ��ע:
// ============================================================================
s32 ModuleInstall_XIP_FS(u32 opt, void *data,char * xip_target)
{
    struct Object * mountobj;
    static struct filesystem *typeXIPIBOOT = NULL;
    s32 res;
    if(typeXIPIBOOT == NULL)
    {
        typeXIPIBOOT = malloc(sizeof(*typeXIPIBOOT));

        typeXIPIBOOT->fileOps = xip_iboot_ops;
        typeXIPIBOOT->install = xip_iboot_fs_install;
        typeXIPIBOOT->pType = "XIP-IBOOT";
        typeXIPIBOOT->format = xip_fs_format;
        typeXIPIBOOT->uninstall = NULL;
    }
    res = File_RegisterFs(typeXIPIBOOT);
    if(-1==res)
    {
        printf("\r\n: dbug : module : cannot register \"xip-iboot\"(file system type).");
        return (-1); // ʧ��;
    }

    mountobj = OBJ_NewChild(OBJ_GetRoot(), __File_MountOps, 0, xip_target);
    if(NULL == mountobj)
    {
        printf("\r\n: dbug : module : mount \"xip\" failed, cannot create \"%s\"(target).", xip_target);
        return (-1);
    }
//    OBJ_DutyUp(mountobj);
    opt |= MS_DIRECTMOUNT;      //ֱ�ӹ��ز��ñ���
    res = File_Mount(NULL, xip_target, "XIP-IBOOT", opt, data);
    if(res == -1)
    {
        printf("\r\n: dbug : module : mount \"XIP-IBOOT\" failed, cannot install.");
        OBJ_Delete(mountobj);
        return (-1);
    }
    return (0);
}

// ============================================================================
// ���ܣ���װxip�ļ�ϵͳ
// ������ opt -- �ļ�ϵͳ����ѡ���MS_INSTALLCREAT
//      data -- ���ݸ�xip-iboot��װ�߼������ݣ�
// ���أ�ʧ��(-1)�� �ɹ�(0)��
// ��ע:
// ============================================================================
s32 ModuleInstall_XIP_IBOOT_FS(u32 opt, void *data)
{
    ModuleInstall_XIP_FS(opt,data,CN_XIP_IBOOT_MOUNT);
    return 0;
}

