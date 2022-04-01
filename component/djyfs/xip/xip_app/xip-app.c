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
#include <Iboot_info.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_XIP_APP_FS(u32 opt, void *data);
//    ModuleInstall_XIP_APP_FS(0,NULL);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"xip app file system"//����app����������
//parent:"file system"//��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
//dependence:"file system"             //������������������������none����ʾ�������������
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_XIP_APP_FILE_SYSTEM == false )
//#warning  " xip_app_file_system  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_XIP_APP_FILE_SYSTEM    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
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

// �ײ�ӿں���
//

static s32 xip_app_fs_install(struct FsCore *super, u32 opt, void *config);
s32 xip_app_ops(void *opsTarget, u32 cmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3);

//
//
//
#define __FILE_NAME_MAX_LEN               (240) // ֧�ֵ�����ļ�������(����������)

//
// xip�ļ���;��ǩ
//
#define __TAG_RELEASE                      ((u32)0x61707000) // ����ģʽ���ļ���
#define __TAG_DEBUG                        ((u32)0x00676264) // ����ģʽ���ļ���

// xip�ļ�״̬
#define __STATUS_TEMP                   ((u32)0x1)
#define __STATUS_UPDATING                    ((u32)0x2)
#define __STATUS_UPDATED                     ((u32)0x3)


//extern s32 __ll_write(struct __icore *core, u8 *data, u32 bytes, u32 pos);
//extern s32 __ll_read(struct __icore *core, u8 *data, u32 bytes, u32 pos);
//extern s32 __ll_erase(struct __icore *core, u32 bytes, u32 pos);
//extern u32 __ll_crc32(struct __icore *core, u32 pos, u32 len);
// ============================================================================
// ���ܣ������ļ���ʽ(ͷ��)��
// ������head -- xip�ļ�ͷ����ʽ��
//      file -- xip�ļ���
// ���أ�xip�ļ���
// ��ע��
// ============================================================================
struct __ifile *xip_app_decodefilehead(void *head, struct __ifile *file)
{
    if(!file)
        return (NULL);

    if(false == XIP_AppFileCheckEasy(head))
    {
        //printf("\r\n: info : xipfs  : Error checking local app file header,��鱾�ص�appʱ������Ӱ���������app�� \r\n");
        printf("\r\nInfo: the fireware was erased, so ready to write new firmware, ok!\r\n");
        return (NULL);
    }
    file->sz = XIP_GetAppSize(head);
    file->status = __STATUS_UPDATED;
    return (file);
}

//==========================================================================
// ���ܣ��ļ�����
// ������
// ���أ�
// ��ע��
// ============================================================================
static inline void xip_app_lock(struct __icore *core)
{
    Lock_MutexPend(core->lock, CN_TIMEOUT_FOREVER);
}

// ============================================================================
// ���ܣ��ļ�����
// ������
// ���أ�
// ��ע��
// ============================================================================
static inline void xip_app_unlock(struct __icore *core)
{
    Lock_MutexPost(core->lock);
}

// ============================================================================
// ���ܣ��½�xip�ļ����ݽṹ
// ������core -- xip�ļ�ϵͳ��Ϣ��
// ���أ��ɹ���xip�ļ����� ʧ��(NULL);
// ��ע��
// ============================================================================
static struct __ifile *xip_app_newfile(struct __icore *core)
{
    struct __ifile *file;
    u32 size = sizeof(*file);

    core = core;
    file = malloc(size);
    if(!file)
        return (NULL);

    memset(file, 0x0, size);
    file->cxbase = Iboot_GetAppHeadSize();
    file->status = __STATUS_TEMP;
    return (file);
}

// ============================================================================
// ���ܣ� �ļ�ͷ��λ��
// ������file -- xip�ļ�
// ���أ��ļ�ͷ��λ��ƫ��
// ��ע��
// ============================================================================
static inline u32 xip_app_locatefilehead(struct __ifile *file)
{
    return (file->cxbase - Iboot_GetAppHeadSize());
}

// ============================================================================
// ���ܣ� ��ʽ��xip�ļ���ͷ����
// ������core -- xip�ļ�ϵͳ��Ϣ��
//      file -- xip�ļ���
// ���أ�ʧ�ܣ�-1�����ɹ���0����
// ��ע��
// ============================================================================
s32 xip_app_formatfilehead(struct __icore *core, struct __ifile *file)
{
    u32 base = 0;

    if(file)
        base = xip_app_locatefilehead(file);

   return (core->drv->xip_erase_media(core, Iboot_GetAppHeadSize(), base));
}

// ============================================================================
// ���ܣ������ļ���ʽ��ͷ����
// ������core -- xip�ļ�ϵͳ��Ϣ��
//      file -- xip�ļ���
//      name -- xip�ļ�����
// ���أ�ʧ�ܣ�-1�����ɹ���0����
// ��ע��
// ============================================================================
s32 xip_app_makefilehead(struct __icontext *cx,struct __icore *core,
                                    struct __ifile *file, const char *name)
{
    if(file->cxbase != cx->Wappsize)
        return -1;
    file->sz += file->cxbase;
    Iboot_RewriteAppHeadFileInfo(cx->apphead,name,file->sz);
    if(-1 == core->drv->xip_write_media(core, cx->apphead, file->cxbase, xip_app_locatefilehead(file)))
        return (-1);

    file->status = __STATUS_UPDATED;
    return (0);
}

// ============================================================================
// ���ܣ�ɨ��xip�ļ�
// ������core -- xip�ļ�ϵͳ��Ϣ��
// ���أ�ʧ�ܣ�δ�ҵ��ļ���-1�����ɹ��ҵ��ļ���0����
// ��ע��
// ============================================================================
static s32 xip_app_scanfiles(struct __icore *core)
{
    s32 res;
    u32 size = Iboot_GetAppHeadSize();
    u8 * structFileHead = malloc(size);
    char *name;
    struct __ifile *file=NULL;
    memset(structFileHead, 0xff, size);
    res = core->drv->xip_read_media(core, structFileHead,size, 0);
    if(res)
        goto Error;

    // ��ǰֻ��һ���ļ�
    file = xip_app_newfile(core);
    if(NULL == xip_app_decodefilehead(structFileHead, file))
    {   // ��ǰ�߼������ϵ�����ļ���ʱ���ʽ�����ռ�,��ֻ��ʽһ��ͷ����
        // ����Ҫ����߼�����Ϊ���������̵��жϣ�������ͷ�������ڣ������������ݡ�
        // ����һ��д��ʱ���������չ�����߼���
        res = xip_app_formatfilehead(core, file);
        goto Error; // ��ǰϵͳ�����ļ��������߼���ִ��
    }
    name = Iboot_GetAppName(structFileHead);
    // �����ݽ����ļ�ϵͳ
    if(!OBJ_NewChild(core->root, xip_app_ops, (ptu32_t)file, name))
    {
        goto Error;
    }
//  obj_Close(core->root->child);   //  ���ӽڵ����ô�����һ����Ϊxip���ļ���û�д򿪣�ֻ�����ȼӵ�obj��
    printf("\r\n: info : xipfs  : valid file found, name(%s), size(%dKB).", name, (file->sz>>10));
    free(file);
    free(structFileHead);
    return (0);
Error:
    free(file);
    free(structFileHead);
    return (-1);
}

// ============================================================================
// ���ܣ��ͷ�xip�ļ����ڴ��еĿռ䣩
// ������file -- xip�ļ���
// ���أ�
// ��ע��INLINE
// ============================================================================
static void xip_app_freefile(struct __ifile *file)
{
    if(file->status == __STATUS_UPDATING)
        return;

//    of_unlinkbasic(&file->basic);
//    Lock_MutexDelete(file->lock);
    free(file);
}

// ============================================================================
// ���ܣ�ɾ��xip�ļ��������洢�����ϵ�����
// ������core -- xip�ļ�ϵͳ��Ϣ��
//      file -- xip�ļ���
// ���أ�ʧ�ܣ�-1�����ɹ���0����
// ��ע��INLINE
// ============================================================================
static s32 xip_app_delfile(struct __icore *core, struct __ifile *file)
{
    if(core->drv->xip_erase_media(core, Iboot_GetAppHeadSize(), xip_app_locatefilehead(file)))
        return (-1);

    xip_app_freefile(file);
    return (0);
}

// ============================================================================
// ���ܣ��½�xip�ļ������ģ�
// ������core -- xip�ļ�ϵͳ��Ϣ��
// ���أ��ɹ���xip�ļ������ģ���ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
static struct __icontext *xip_app_newcontext(struct __icore *core)
{
    struct __icontext *cx;
    u32 appheadsize = Iboot_GetAppHeadSize();

    cx = malloc(sizeof(*cx) + core->bufsz+appheadsize);
    if(!cx)
        return (NULL);

    memset(cx, 0, sizeof(*cx) + core->bufsz+appheadsize);
    cx->pos = 0;
    cx->buf = (u8*)cx + sizeof(*cx);
    cx->apphead = cx->buf+core->bufsz;
    cx->Wappsize = 0;
    cx->bufed = 0;
    return (cx);
}

// ============================================================================
// ���ܣ��ͷ�xip�ļ�������
// ������pContext -- xip�ļ������ģ�
// ���أ�
// ��ע��INLINE
// ============================================================================
void xip_app_freecontext(struct __icontext *pContext)
{
    return (free(pContext));
}

// ============================================================================
// ���ܣ����ļ�
// ������ob -- �ļ�����
//      flags -- �ļ�������־λ��
//      uncached -- ���½����ļ����ƣ�
// ���أ��ɹ���xip�ļ������ģ���ʧ�ܣ�NULL����
// ��ע��
// ============================================================================
static struct objhandle *xip_app_open(struct Object *ob, u32 flags, char *uncached)
{
    s32 size;
    struct objhandle *hdl;
    struct Object *tmp;
    struct __ifile *file = NULL;
    struct __icontext *cx = NULL;
    struct __icore *core = (struct __icore*)File_Core(ob);
//    mode_t mode;
    xip_app_lock(core);
    if((!uncached)&&(File_ObjIsMount(ob))) // ��Ŀ¼
    {
        if(!Handle_FlagIsDirectory(flags))
        {
            xip_app_unlock(core);
            return (NULL);
        }
    }
    if(uncached)
    {
    //�ر��ļ����ļ���obj���mount���obj��ɾ�����������mount���obj��û���ļ���obj���ȴ�flash�л�ȡ���ļ���Ϣ
        xip_app_scanfiles(core);//ɨ���ļ�
        do
        {
            if(strcmp(uncached,OBJ_GetName(ob)) == 0)
            {
                uncached = NULL;
                break;
            }
            tmp = OBJ_GetChild(ob);
            if(tmp == NULL)
            {
                break;
            }
            ob = tmp;
        }while(1);
    }
    if(Handle_FlagIsDirectory(flags)) // Ŀ¼�߼�
    {
        xip_app_scanfiles(core);//ɨ���ļ�
        if(uncached)// ��֧���½�Ŀ¼
        {
            printf("\r\n: info : xipfs  : do not support create directory.");
            xip_app_unlock(core);
            return (NULL);
        }
    }
    else // �ļ����߼�
    {
        if(uncached) // �ļ������ڣ���Ҫ�½������ļ������ѻ���ģ�
        {
            if(!Handle_FlagIsCreate(flags))
            {
                xip_app_unlock(core);
                return (NULL); // �򿪲��������½�Ҫ���򷵻ز����ڣ�
            }

            // �ж�xip�ļ�ϵͳ�ǲ����Ѿ����ļ��ˣ���������Ƿ�����ʹ�ã�
            tmp = OBJ_GetChild(core->root);
            if(tmp)
            {
                if(Handle_FlagIsWriteable(flags))
                {
                    if(OBJ_IsOnDuty(tmp))
                    {
                        printf("\r\n: info : xipfs  : cannot create new for old are using.");
                        xip_app_unlock(core);
                        return (NULL);
                    }
                    ob = OBJ_GetParent(tmp);
                    if(OBJ_Delete(tmp))        //xip�ļ�ϵͳ��ֻҪ����дһ���������ļ��Ĳ�������ֱ�Ӹ�������ԭ�����ļ�
                    {
                        printf("\r\n: info : xipfs  : cannot create new for old cannot delete.");
                        xip_app_unlock(core);
                        return (NULL);
                    }
                }
                else
                {
                    printf("\r\n: info : xipfs  : \"%s\"  file nonentity.", uncached);
                    xip_app_unlock(core);
                    return (NULL);
                }
            }
            file = xip_app_newfile(core);
            if(!file)
            {
                xip_app_unlock(core);
                return (NULL);
            }

            xip_app_formatfilehead(core, file);
#if 0
            if(of_virtualize(ob, &file->basic, uncached)) // xip�ļ��������ļ�ϵͳ
                return (NULL);
//#else
//            if(!OBJ_NewChild(core->root, xip_app_ops, (ptu32_t)file, uncached))
//            {
//                xip_app_unlock(core);
//                return (NULL);
//            }
#endif
        }
        else // �ļ��Ѵ���
        {
            if(Handle_FlagIsOnlyCreate(flags))
            {
                xip_app_unlock(core);
                return (NULL); // �����½��߼������ļ��Ѵ���
            }

            file = (struct __ifile*)OBJ_GetPrivate(ob);
            if(Handle_FlagIsTrunc(flags))
            {
                if(-1 == xip_app_formatfilehead(core, file))
                {
                    xip_app_unlock(core);
                    return (NULL);
                }

                file->sz = 0;
                file->status = __STATUS_UPDATING;
            }
        }

        cx = xip_app_newcontext(core);
        if(!cx)
        {
            if(uncached)
                xip_app_freefile(file); // �ͷŵ����洴����

            xip_app_unlock(core);
            return (NULL);
        }

        // Ԥ�����߼�
        if(file->sz<core->inhead)
        {
            size = file->sz;
        }
        else
        {
            size = core->inhead;
        }

        if(Handle_FlagIsAppend(flags))
        {
            cx->pos = file->sz;
            if(file->sz<core->inhead)
                size = file->sz;
            else if(size == (s32)file->sz)
                size = 0;
            else
                size = (file->sz - core->inhead) % core->bufsz;

            cx->bufed = size;
        }
        else
        {
            size = core->inhead;
            cx->bufed = 0;
        }

        if(core->drv->xip_read_media(core, cx->buf, size, (cx->pos+file->cxbase-cx->bufed)))
        {
            if(uncached)
                xip_app_freefile(file); // �ͷŵ����洴����

            xip_app_unlock(core);
            return (NULL);
        }
    }

    hdl = Handle_New();
    if(!hdl)
    {
        xip_app_freecontext(cx);
        if(uncached)
            xip_app_freefile(file); // �ͷŵ����洴����
    }

    Handle_Init(hdl, NULL, flags, (ptu32_t)cx);

    if(hdl)
    {
        //TODO����yaffs2�ж�ȡȨ�޵ȣ���ʱ����ȫ��Ȩ�ޡ�
//        mode = S_IALLUGO | S_IFREG;     //������·����������Ŀ¼��
        //�̳в��������������˽�г�Ա�������ģʽ���� stat �� st_mode ��
        ob = OBJ_BuildTempPath(ob, xip_app_ops, (ptu32_t)file,uncached);
        OBJ_LinkHandle(hdl, ob);
    }
    xip_app_unlock(core);
    return (hdl);
}

// ============================================================================
// ���ܣ��ر��ļ�
// ������hdl -- xip�ļ���������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 xip_app_close(struct objhandle *hdl)
{
    s32 size;
    struct __icore *core;
    struct __ifile *file;
    struct __icontext *cx = (struct __icontext*)Handle_GetContext(hdl);

    if(cx) // NULLʱ��ʾĿ¼
    {
        core = (struct __icore*)File_Core(Handle_GetHostObj(hdl));
        file = (struct __ifile*)handle_GetHostObjectPrivate(hdl);
        xip_app_lock(core);
        if(__STATUS_UPDATED != file->status) // ���ݴ���д����������ļ����½���
        {
            if(cx->bufed)
            {
                if(cx->pos<=core->inhead)
                    size = core->inhead;
                else
                    size = core->bufsz;

                if(-1 == core->drv->xip_write_media(core, cx->buf, size, (cx->pos-cx->bufed+file->cxbase)))
                {
                    xip_app_unlock(core);
                    return (-1);
                }
            }

            if(!iscontender(hdl)) // ���һ���ļ�ʹ���߹ر��ļ�ʱ���Ż������ļ�ͷ
            {
                if(xip_app_makefilehead(cx,core, file, Handle_GetName(hdl)))
                {
                    xip_app_unlock(core);
                    return (-1);
                }
            }
        }

        xip_app_unlock(core);
    }

    xip_app_freecontext(cx);
//  Handle_Delete(hdl);
    return (0);
}

// ============================================================================
// ���ܣ�д�ļ�
// ������hdl -- xip�ļ���������
//      data -- ��д�����ݵĿռ䣻
//      size -- ��д�����ݵĴ�С��
// ���أ�ʵ��д����ֽ�����
// ��ע��
// ============================================================================
static s32 xip_app_write(struct objhandle *hdl, u8 *data, u32 size)
{
    s32 pos, once, free, res;
    u32 left;
    struct __icontext *cx = (struct __icontext *)Handle_GetContext(hdl);
    struct __icore *core = (struct __icore*)File_Core(Handle_GetHostObj(hdl));
    struct __ifile *file = (struct __ifile*)handle_GetHostObjectPrivate(hdl);
    u32 wsize = 0;

    if(cx->Wappsize < file->cxbase)
    {
        if(size > (file->cxbase - cx->Wappsize))
        {
            wsize = (file->cxbase - cx->Wappsize);
            memcpy(&cx->apphead[cx->Wappsize],data,wsize);
            size-= wsize;
            data+=wsize;
            cx->Wappsize+=wsize;
        }
        else
        {
            memcpy(&cx->apphead[cx->Wappsize],data,size);
            cx->Wappsize+=size;
            return size;
        }
    }

    left = size;               //��һ��Ӧ�ü�ȥ�ļ�ͷ��Ϣ.����Ҫд���ֽ���Ӧ������
    xip_app_lock(core);
    if(cx->pos<=core->inhead) // ������ʣ���д�ռ䣻������д�Ͳ�����д������ô����256ʱ��
        free = core->inhead - cx->pos; // �ڿ�ʼ�������У��ļ�ͷ��ռ���˹̶��ռ䣻
    else
        free = core->bufsz - ((cx->pos - core->inhead) % core->bufsz);

    memset(cx->buf, 0xff, core->bufsz);
    while(left)
    {
        once = left;
        if(once>free) // ������д�뵽�����У�
            once = free;
        memcpy(cx->buf+cx->bufed, data, once);
        cx->bufed += once;
        if((cx->bufed==core->bufsz) || // ����������ˢ��
           ((cx->bufed==(s16)core->inhead) // �ļ�ͷ��ռ���˹̶��ռ䣬�����Ҳ�������룩
             &&(((cx->pos+once)<=core->inhead)))) // (�������������ڴ�����seek�߼�)
        {
            pos = cx->pos - (cx->bufed - once) + file->cxbase; // ��ǰλ��д��ʱ��λ�ã���������˵ģ�
            res = core->drv->xip_write_media(core, cx->buf, cx->bufed, pos);// ��ʱbufed�Ĵ�СΪbufsz����inhead��
            if(-1==res) // д����
            {
                break;
            }
            else if(-2==res) // ��Ҫû�п�д�ռ䣬ɾ��һЩ��
            {
                if(core->drv->xip_erase_media(core, core->bufsz, (pos+cx->bufed+1)))// '+1'��ʾ��һ��Ҫд�Ŀռ��ַ��
                    break;
            }

            // �������ã��������Ļ���Ԥȡ��������һ��head�����ǲ�����������Ҫ����ģ�
            // �˺�seek��ֱ���ƶ�bufed;
            if(left < (u32)core->bufsz)
            {
                if(core->drv->xip_read_media(core, cx->buf, core->bufsz, pos+cx->bufed))
                    break;
            }
            cx->bufed = 0;
            memset(cx->buf, 0xff, core->bufsz);
        }

        left -= once;
        data += once;
        cx->pos += once;
        free = core->bufsz;
    }

    if(file->sz<cx->pos)
        file->sz = cx->pos;

    if((__STATUS_UPDATING != file->status)&&(left != size))
        file->status = __STATUS_UPDATING; // �ļ����ݷ�����ı�

    xip_app_unlock(core);
    return (wsize+size-left);
}
static s32 xip_app_sync(struct objhandle *hdl)
{
    s32 size;
    struct __icontext *cx = (struct __icontext *)Handle_GetContext(hdl);
    struct __icore *core = (struct __icore*)File_Core(Handle_GetHostObj(hdl));
    struct __ifile *file = (struct __ifile*)handle_GetHostObjectPrivate(hdl);
    xip_app_lock(core);
    if(__STATUS_UPDATED != file->status) // ���ݴ���д����������ļ����½���
    {
        if(cx->bufed)
        {
            if(cx->pos<=core->inhead)
                size = core->inhead;
            else
                size = core->bufsz;

            if(0 != core->drv->xip_write_media(core, cx->buf, size, (cx->pos-cx->bufed+file->cxbase)))
            {
                xip_app_unlock(core);
                return (-1);
            }
        }
    }

    xip_app_unlock(core);
    return (0);
}

// ============================================================================
// ���ܣ����ļ�
// ������hdl -- xip�ļ���������
//      data -- ��������ݵĴ�ſռ䣻
//      size -- ��������ݵĴ�С��
// ���أ�ʵ�ʶ����ĵ�Ԫ����
// ��ע��
// ============================================================================
static s32 xip_app_read(struct objhandle *hdl, u8 *data, u32 size)
{
    s32 once, left = size;
    struct __icontext *cx = (struct __icontext*)Handle_GetContext(hdl);
    struct __icore *core = (struct __icore*)File_Core(Handle_GetHostObj(hdl));
    struct __ifile *file = (struct __ifile*)handle_GetHostObjectPrivate(hdl);

    xip_app_lock(core);
    if((cx->pos+size)>file->sz)
        size = file->sz - cx->pos; // ���ܶ�Խ��

    // ��ȡԤ����ռ䣻һ�����ļ���Ԥ�Ȼ�����buf�У����Ƕ��룻��һ����size�ǲ�������ļ���С��
    if(cx->pos<=core->inhead)
        once = core->inhead - cx->pos; // �ڿ�ʼ�������У��ļ�ͷ��ռ���˹̶��ռ䣻
    else
        once = (core->bufsz - (cx->pos - core->inhead)) % core->bufsz;

    if(once>=(s32)size)
    {
        // ��������һ�����������Ƕ������ݿ��ܺͽ����еĲ�һ����
        // ��ִ������߼���Ч�ʾͲ������ecc�Ͽ��ܻ����鷳��
        memcpy(data, cx->buf+cx->bufed, size);
        cx->pos += size;
        cx->bufed += size;
        return (size);
    }
#if 0
    // �����ʣ��ռ����ɲ��˻ض������ݣ����Ƚ�����ˢ��ȥ���ٻض���
    pos = cx->pos - cx->bufed + file->cxbase; // �������
    if(-1 == core->drv->xip_write_media(core, cx->buf, (cx->bufed+once), pos)) // ��������д
    {
        xip_app_unlock(core);
        return (0); // ��������ˢ��ʧ�ܣ�
    }
#endif
    memcpy(data, cx->buf+cx->bufed, once); // ����������������ȡ������
    cx->pos += once; // �˺�pos�߼����ǻ������ģ�
    left -= once;
    data += once;
    cx->bufed += once;
    while(left)
    {
        cx->bufed = 0;
        if(core->drv->xip_read_media(core, cx->buf, core->bufsz, cx->pos+file->cxbase))
        {
            xip_app_unlock(core);
            return (size - left); // ��������ˢ��ʧ�ܣ�
        }

        if(left>core->bufsz)
            once = core->bufsz;
        else
            once = left;

        memcpy(data, cx->buf, once);
        data += once;
        cx->bufed += once;
        left -= once;
        cx->pos += once;
    }

    xip_app_unlock(core);
    return (size);
}

// ============================================================================
// ���ܣ������ļ���ǰλ�ã�
// ������hdl -- xip�ļ���������
//      offset -- �ƶ�λ�õ�����
//      whence -- �ƶ�λ�õ���㣻
// ���أ��ɹ���0��*pOffsetָ���µĵ�ǰλ�ã���ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static off_t xip_app_seek(struct objhandle *hdl, off_t *offset, s32 whence)
{
    struct __icontext *cx;
    struct __ifile *file;
    struct __icore *core;
    s32  npos, movs, pos;
    off_t position = *offset;
#if 0 // Ӧ�����ϼ��߼��ж�
    if(File_IsDirectory(hdl))
    {
        printf("\r\n: dbug : xipfs  : cannot seek directory.");
        return (-1);
    }
#endif

    core = File_Core(Handle_GetHostObj(hdl));
    cx = (struct __icontext*)Handle_GetContext(hdl);
    //file = dListEntry(of_basic(hdl), struct __ifile, basic);
    file = (struct __ifile*)handle_GetHostObjectPrivate(hdl);
    xip_app_lock(core);
    switch(whence)
    {
        case SEEK_END: // תΪ��ͷSEEK�߼�
        {
            position = file->sz + position;
            if(position<0)
                position = 0; // ��λ��Խ����
        }
        __attribute__((fallthrough));

        case SEEK_SET: // תΪ��ǰλ�õ�SEEK���߼�
        {
            position = position - cx->pos;
            if((position+cx->pos)<0)
                position = cx->pos; // ��λ��Խ����
        }

        case SEEK_CUR:
        {
            npos = position + cx->pos;
            if(npos<0)
                npos = 0;

            // ������ڻ��淶Χ�ڵ��ƶ�����ֱ�ӷ��أ�
            movs = npos - cx->pos; // �ƶ��ķ���
            if(movs<0)
            {
                if((movs+cx->bufed)>=0)
                {
                    cx->bufed -= movs;
                    position = cx->pos = npos;
                    return (position);
                }
            }
            else if(movs>0)
            {
                if(cx->pos<=core->inhead)
                {
                    if((u32)(movs+cx->bufed)<core->inhead)
                    {
                        cx->bufed += movs;
                        position = cx->pos = npos;
                        return (position);
                    }
                }
                else
                {
                    if((movs+cx->bufed)<core->bufsz)
                    {
                        cx->bufed += movs;
                        position = cx->pos = npos;
                        return (position);
                    }
                }
            }
            else // ����Ҫ�ƶ���
            {
                position = cx->pos;
                return (position);
            }

            // ��������ڻ��淶Χ�ڵ��ƶ���������ˢ�����
            pos = cx->pos - cx->bufed + file->cxbase;
            if(cx->pos<core->inhead)
            {
                if(-1 == core->drv->xip_write_media(core, cx->buf, core->inhead, pos))
                    return (-1);
            }
            else
            {
                if(-1 == core->drv->xip_write_media(core, cx->buf, core->bufsz, pos))
                    return (-1);
            }

            if(npos<=(s32)core->inhead) // �����µ�λ��
            {
                if(core->drv->xip_read_media(core, cx->buf, core->inhead, file->cxbase))
                {
                    memset(cx->buf, 0xFF, core->bufsz);
                    cx->bufed = 0;
                    return (-1);
                }

                cx->bufed = npos;
            }
            else
            {
                pos = npos - (npos + file->cxbase) % core->bufsz + file->cxbase;
                if(core->drv->xip_read_media(core, cx->buf, core->bufsz, pos))
                {
                    memset(cx->buf, 0xFF, core->bufsz);
                    cx->bufed = 0;
                    return (-1);
                }

                cx->bufed = (npos - core->inhead) % core->bufsz;
            }

            position = cx->pos = npos;
            return (position);
        }

        default:
        {
            break;
        }
    }

    return (-1);
}

// ============================================================================
// ���ܣ�ɾ���ļ���
// ������ob -- xip�ļ�����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��δ���ǻ��⣻��pNameΪNULLʱ����ʾ�ļ����ڱ�ʹ�ã�
// ============================================================================
static s32 xip_app_remove(struct Object *ob)
{
    struct __ifile *file;
    struct __icore *core;

    core = (struct __icore *)File_Core(ob);
    file = (struct __ifile*)OBJ_GetPrivate(ob);
    return (xip_app_delfile(core, file));
}

// ============================================================================
// ���ܣ��ļ���ѯ
// ������ob -- xip�ļ�����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 xip_app_stat(struct Object *ob, struct stat *data)
{
    struct __ifile *file;

    if(File_ObjIsMount(ob))
    {
        data->st_size = 0;
        data->st_mode = S_IFDIR;
    }
    else
    {
        file = (struct __ifile*)OBJ_GetPrivate(ob);
        data->st_size = file->sz;
        data->st_mode = S_IFREG|S_IRWXUGO;
    }

    return (0);
}

// ============================================================================
// ���ܣ���xip�ļ�ϵͳĿ¼�
// ������hdl -- xip�ļ���������
//      dentry -- Ŀ¼�
// ���أ�ȫ�����꣨1����ʧ�ܣ�-1��������һ�0����
// ��ע��
// ============================================================================
static s32 xip_app_readdentry(struct objhandle *hdl, struct dirent *dentry)
{
    struct Object *ob = Handle_GetHostObj(hdl);

    ob = OBJ_GetChild(ob);
    if((ob)&&(dentry->d_ino!=(long)ob))
    {
        dentry->d_ino = (long)ob;
        strcpy(dentry->d_name, OBJ_GetName(ob));
        dentry->d_type = DIRENT_IS_REG;
        return (0);
    }

    return (1);
}
// ============================================================================
// ���ܣ���ʼ��xip��media������
// ������core -- efs�ļ�ϵͳ����  drv -- media�Ĳ���������
// ���أ�0 -- �ɹ���  -1 --ʧ��
// ��ע��
// ============================================================================
int xip_app_install_drv(struct __icore *core, struct __xip_drv *drv)
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
static s32 xip_app_fs_install(struct FsCore *super, u32 opt, void *config)
{

    struct __icore *core;
    struct umedia *um;

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
//    um->mreq(unitbytes,(ptu32_t)&flash_page_size);
    xip_app_install_drv(core,super->MediaDrv);
    core->ASize = super->AreaSize;
    core->MStart = super->MediaStart;
    core->vol = (void*)um;
    core->bufsz = (s16)512; // xip�ļ�ϵͳ�ļ��Ļ����С����unit�ĳߴ磻todo��512�������ģ�
    if(core->bufsz<(s16)Iboot_GetAppHeadSize())
    {
        free(core);
        return (-1);
    }

    core->inhead = core->bufsz - Iboot_GetAppHeadSize();
    core->root = super->pTarget;
    core->lock = Lock_MutexCreate("xip-app fs");
    if(!core->lock)
    {
        free(core);
        return (-1);
    }
    xip_app_scanfiles(core); // ɨ���Ѵ����ļ�

    super->pCore = (void*)core;
    return (0);
}

// ============================================================================
// ���ܣ�xip�ļ������ӿ�
// ��������׼�߼����鿴�ӿ�˵����
// ���أ���׼�߼����鿴�ӿ�˵����
// ��ע:
// ============================================================================
s32 xip_app_ops(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
//    va_list list;
    s32 result = CN_OBJ_CMD_EXECUTED;
    switch(objcmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct objhandle *hdl;
            hdl = xip_app_open((struct Object *)opsTarget, (u32)(*(u64*)OpsArgs2), (char*)OpsArgs3);
            *(struct objhandle **)OpsArgs1 = hdl;
            break;
        }

      case CN_OBJ_CMD_READDIR:
      {
          struct objhandle *hdl = (struct objhandle*)OpsArgs3;
          struct dirent *ret = (struct dirent *)OpsArgs1;

          if((ptu32_t)xip_app_readdentry(hdl, ret) == 0)
              result = CN_OBJ_CMD_TRUE;
          else
              result = CN_OBJ_CMD_FALSE;
          break;
      }

        case CN_OBJ_CMD_READ:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = xip_app_read(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_WRITE:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = xip_app_write(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_CLOSE:
        {
            if(xip_app_close((struct objhandle*)opsTarget) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_SEEK:
        {
            *(off_t*)OpsArgs1 = xip_app_seek((struct objhandle *)opsTarget,
                                        (off_t*)OpsArgs2, (s32)OpsArgs3);
            break;
        }

        case CN_OBJ_CMD_DELETE:
        {
            if(xip_app_remove((struct Object*)opsTarget) == 0)
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
            if(xip_app_stat((struct Object*)opsTarget, (struct stat *)OpsArgs1) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_SYNC:
        {
            if((ptu32_t)xip_app_sync((struct objhandle*)opsTarget) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

__attribute__((weak)) s32 xip_fs_format(void *core)
{
    return 0;
}
#pragma GCC diagnostic pop

// ============================================================================
// ���ܣ���װxip�ļ�ϵͳ
// ������target -- ��װĿ¼��
//      opt -- �ļ�ϵͳ����ѡ���MS_INSTALLCREAT
//      data -- ���ݸ�xip��װ�߼������ݣ�
// ���أ�ʧ��(-1)�� �ɹ�(0)��
// ��ע:
// ============================================================================
s32 ModuleInstall_XIP_APP_FS(u32 opt, void *data)
{
    struct Object * mountobj;
    static struct filesystem *typeXIPAPP = NULL;
    s32 res;

    if(typeXIPAPP == NULL)
    {
        typeXIPAPP = malloc(sizeof(*typeXIPAPP));

        typeXIPAPP->fileOps = xip_app_ops;
        typeXIPAPP->install = xip_app_fs_install;
        typeXIPAPP->pType = "XIP-APP";
        typeXIPAPP->format = xip_fs_format;
        typeXIPAPP->uninstall = NULL;
    }
    res = File_RegisterFs(typeXIPAPP);
    if(-1==res)
    {
        printf("\r\n: dbug : module : cannot register \"XIP-APP\"(file system type).");
        return (-1); // ʧ��;
    }

    mountobj = OBJ_NewChild(OBJ_GetRoot(), __File_MountOps, 0, CN_XIP_APP_MOUNT);
    if(NULL == mountobj)
    {
        printf("\r\n: dbug : module : mount \"xip\" failed, cannot create \"%s\"(target).", CN_XIP_APP_MOUNT);
        return (-1);
    }
//    OBJ_DutyUp(mountobj);
    opt |= MS_DIRECTMOUNT;      //ֱ�ӹ��ز��ñ���
    res = File_Mount(NULL, CN_XIP_APP_MOUNT, "XIP-APP", opt, data);
    if(res == -1)
    {
        printf("\r\n: dbug : module : mount \"XIP-APP\" failed, cannot install.");
        OBJ_Delete(mountobj);
        return (-1);
    }

    return (0);
}


