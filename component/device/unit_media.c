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
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <objhandle.h>
#include <systime.h>
#include <device.h>
#include <string.h>
#include <stdio.h>
#include <dbug.h>
#include "device/unit_media.h"

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"uint media"   //ģ����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"stdio" //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //���������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure

struct umcontext{
    struct uopt opt;
    s64 offset;
};

#if 0
enum ustatstype{
    wr = 1,
    wrecc,
    rd,
    rdecc,
    erase,
};

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static void __um_stats(enum ustatstype type)
{
    ;
}


// ============================================================================
// ���ܣ���unit media�豸�ļ�;
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __um_open(struct objhandle *hdl, u32 mode, u32 timeout)
{
    struct umcontext *mc;
    struct umedia *media;

    timeout = timeout;
    media = (struct umedia*)Device_GetDrvTag(Handle2fd(hdl));
    mc = (struct umcontext*)malloc(sizeof(struct umcontext));
    if(!mc)
        return (-1);

    memset(mc, 0x0, sizeof(struct umcontext));
    mc->opt= media->opt;

    Handle_SetContext(hdl, (ptu32_t)mc);
    return (0);
}

// ============================================================================
// ���ܣ��ر�unit media�豸�ļ�;
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __um_close(struct objhandle *hdl)
{
    struct umcontext *mc = (struct umcontext*)Handle_GetContext(hdl);

    free(mc);
    return (0);
}

// ============================================================================
// ���ܣ�д�ر�unit media�豸�ļ�;
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __um_write(struct objhandle *hdl, u8 *data, u32 len, u32 offset, u32 timeout)
{
    s32 res, ret, page;
    s64 pos;
    s32 unaligns, unaligno;;
    u8 *dbuf, *buf;
    struct umedia *media;
    struct umcontext *mc;

    media = (struct umedia*)Device_GetDrvTag(Handle2fd(hdl));
    mc = (struct umcontext*)(hdl);
    dbuf = media->ubuf;
    buf = (u8*)data;

//  if(israw(hdl))
//      pos = offset & (0xFFFFFFFF);
//  else
//      pos = mc->offset + offset;

//    if((pos > media->ustart + media->size) || ((pos+len) > media->start + media->size))
//    {
//        error_printf("device","write out of range<%s>.", Handle_GetName(hdl));
//        debug_printf("device","range from <%l> to <%l>.", media->start, (media->start+media->size));
//        return (0);
//    }

    res = len;
    media->mreq(lock, CN_TIMEOUT_FOREVER, 0);

    // ��ʼ������
    unaligno = pos & ((1 << media->asz) - 1); // unaligned offset
    unaligns = (1 << media->asz) - offset; // unaligned size
    page = pos >> media->asz;

    if(unaligno)
    {
        ret =  media->mread(page, dbuf, mc->opt);
        if(ret != media->asz)
        {
           goto __ERR_W;
        }

        memcpy(dbuf+unaligns, buf, unaligns);

        ret =  media->mwrite(page, dbuf, mc->opt);
        if(ret != media->asz)
        {
            goto __ERR_W;
        }
    }

    len -= unaligns;
    buf += unaligns;
    page ++;
    while(len >= media->asz)
    {
        ret =  media->mwrite(page, buf, mc->opt);
        if(ret != media->asz)
        {
            break;
        }

        len -= media->asz;
        buf += media->asz;
        page ++;
    }

    // ����������
    if(len)
    {
        ret =  media->mread(page, dbuf, mc->opt);
        if(ret != media->asz)
        {
           goto __ERR_W;
        }

        memcpy(dbuf, buf, len);
        ret =  media->mwrite(page, dbuf, mc->opt);
        if(ret != media->asz)
        {
            goto __ERR_W;
        }

        len -= len;
    }

__ERR_W:
    media->mreq(unlock, 0, 0);
    return (res-len);
}

// ============================================================================
// ���ܣ���unit media�豸�ļ�;
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __um_read(struct objhandle *hdl, u8 *data, u32 len, u32 offset, u32 timeout)
{
    s32 res, ret, page;
    s64 pos;
    s32 unaligns, unaligno;;
    u8 *dbuf, *buf;
    struct umedia *media;
    struct umcontext *mc;

    media = (struct umedia*)Device_GetDrvTag(Handle2fd(hdl));
    mc = (struct umcontext*)(hdl);
    dbuf = media->ubuf;
    buf = (u8*)data;

//  if(israw(hdl))
//      pos = offset & (0xFFFFFFFF);
//  else
//      pos = mc->offset + offset;

//    if((pos > media->ustart + media->size) || ((pos+len) > media->ustart + media->size))
//    {
//        error_printf("device","read out of range<%s>.", Handle_GetName(hdl));
//        debug_printf("device","range from <%l> to <%l>.", media->start, (media->start+media->size));
//        return (0);
//    }

    res = len;
    media->mreq(lock, CN_TIMEOUT_FOREVER, 0);

    // ��ʼ������
    unaligno = pos & ((1 << media->asz) - 1); // unaligned offset
    unaligns = (1 << media->asz) - offset; // unaligned size
    page = pos >> media->asz;

    if(unaligno)
    {
        ret =  media->mread(page, dbuf, mc->opt);
        if(ret != media->asz)
        {
           goto __ERR_R;
        }

        memcpy(buf, dbuf+unaligns, unaligns);
    }

    len -= unaligns;
    buf += unaligns;
    page ++;

    while(len >= media->asz)
    {
        ret =  media->mread(page, buf, mc->opt);
        if(ret != media->asz)
        {
           goto __ERR_R;
        }

        len -= media->asz;
        buf += media->asz;
        page ++;
    }

    // ����������
    if(len)
    {
        ret =  media->mread(page, dbuf, mc->opt);
        if(ret != media->asz)
        {
           goto __ERR_R;
        }

        memcpy(buf, dbuf, len);
        len -= len;
    }

__ERR_R:
    media->mreq(unlock, 0, 0);
    return (res-len);
}

// ============================================================================
// ���ܣ�����unit media�豸�ļ�;
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __um_cntl(struct objhandle *hdl, u32 cmd, ptu32_t arg1, ptu32_t arg2)
{
    return (0);
}

//// ============================================================================
//// ���ܣ�����unit media�豸�ࣻ
//// ������
//// ���أ�
//// ��ע��ֻ�����һ�Σ�
//// ============================================================================
//static inline struct Object *__isbuild(void)
//{
//    static u8 inited = 0;
//    static struct Object *mm;
//
//    if(inited)
//        return (mm);
//
//    mm = dev_group_addo("unit media");
//    if(!mm)
//    {
//        printf("\r\n: erro : device : cannot build \"unit media\".");
//        return (NULL);
//    }
//
//    inited = 1;
//    return (mm);
//}

// ============================================================================
// ���ܣ����unit media�豸;
// ������name -- unit media����
//      media -- ���ƽӿڣ�
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 um_add(const char *name, struct umedia *media)
{
    struct Object *mmo;

//  mmo = __isbuild();
//  if(!mmo)
//  {
//      error_printf("device","cannot add  \"%s\"<build>.", name);
//      return (-1);
//  }

    if(Device_Create(name, __um_open, __um_close, __um_write,
                __um_read, __um_cntl, (ptu32_t)media))
        return (0);

    error_printf("device","cannot add unit-media \"%s\".", name);
    return (-1);
}

// ============================================================================
// ���ܣ���ĳ���豸�ϰ�װUnit Media
// ������ um_init -- �豸����������
//      parts -- ��������
//      �������� -- ��ʼ�飬�����������Ƿ��ʽ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 ModuleInstall_UnitMedia(s32(*dev_init)(u32 bstart, u32 bcount, u32 doformat),
                            u8 parts, ...)
{
    u8 part;
    u32 startblock, blocks, doformat;
    va_list list;
    s32 res = 0;

    if(!dev_init)
    {
        error_printf("device","cannot add unit-media(no driver).");
        return (-1);
    }

    va_start(list, parts);
    for(part=0; part<parts; part++)
    {
        startblock = (u32)va_arg(list, u32);
        blocks = (u32)va_arg(list, u32);
        doformat = (u32)va_arg(list, u32);
        if(dev_init(startblock, blocks, doformat))
        {
            error_printf("device","cannot add unit-media(driver error).");
            res = -1;
            break;
        }
    }

    va_end(list);
    return (res);
}
#endif
