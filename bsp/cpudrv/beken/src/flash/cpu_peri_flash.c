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
// =============================================================================
#include "cpu_peri.h"
#include "lock.h"
#include "systime.h"
#include "stdlib.h"
#include <xip.h>
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <device/include/unit_media.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern int ModuleInstall_Flash(void);
//  ModuleInstall_Flash();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip flash"//CPU��rtc��������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_FLASH == false )
//#warning  " cpu_onchip_flash  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header    //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_FLASH    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������
//@#$%component end configure

struct MutexLCB *flash_mutex = NULL;
static bool_t sflashInited = false;
static struct umedia *flash_um;
const char *flash_name = "emflash";      //��flash��obj�ڵ�����
extern struct Object *s_ptDeviceRoot;

static struct FlashDescr{
    u32     BytesPerPage;                // ҳ�а������ֽ���
    u32     PagesPerSector;               //  sector��ҳ��
    u32     AllSectorNum;               //  ���е�sector��
    u32     MappedStAddr;
} *description;

// ============================================================================
// ���ܣ���������FLASH����Ϣ
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 SetFlash_Init(struct FlashDescr *Description)
{
    Description->BytesPerPage = 256;
    Description->PagesPerSector = 16;
    Description->AllSectorNum = 1024;
    Description->MappedStAddr = 0x0;
    return (0);
}


void djy_flash_read(uint32_t address, void *data, uint32_t size)
{
    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
//    flash_protection_op(0,FLASH_PROTECT_NONE);
    flash_read(data, size, address);
//    flash_protection_op(0,FLASH_PROTECT_ALL);
    Lock_MutexPost(flash_mutex);
}

//void djy_flash_read(uint32_t address, void *data, uint32_t size)
//{
//    u32 i = 0, j = 0, practical_addr = address * 34 / 32;
//    u8 buf[272];
//    if (size == 0)
//    {
//        return;
//    }
//    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
//    while(size)
//    {
//        memset(buf, 0xFF, 272);
//        djy_flash_read_crc(practical_addr, buf, 272);
//        i = j = 0;
//        while(i < 272)
//        {
//            if(size > 32)
//                memcpy(data + j, buf + i, 32);
//            else
//            {
//                memcpy(data + j, buf + i, size);
//                size = 0;
//                break;
//            }
//            j += 32;
//            i += 34;
//            size -= 32;
//        }
//        practical_addr += 272;
////        Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
////    //    flash_protection_op(0,FLASH_PROTECT_NONE);
////        flash_read(data, size, practical_addr);
////    //    flash_protection_op(0,FLASH_PROTECT_ALL);
////        Lock_MutexPost(flash_mutex);
//    }
//    Lock_MutexPost(flash_mutex);
//}

void djy_flash_write(uint32_t address, const void *data, uint32_t size)
{
//    u32 i, j, len, practical_addr = address * 34 / 32;
//    u8 buf_crc[272],buf[256];
    if (size == 0)
        return;

    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
//    flash_protection_op(0,FLASH_PROTECT_NONE);

//    while(size)
//    {
//        memset(buf, 0xff, 272);
//        if(size >= 256)
//        {
//            for(i = 0; i < 8; i++)
//            {
//                memcpy(buf + (i * 34), data + (i * 32), 32);
//            }
//            size -= 256;
//            len = 272;
//            calc_crc((u32 *)buf, 256 / 32);
//        }
//        else
//        {
//            for(i = 0; i < (size / 32); i++)
//            {
//                memcpy(buf + (i * 34), data + (i * 32), 32);
//            }
//            j = size % 32;
//            if(j)
//            {
//                memcpy(buf + (i * 34), data + (i * 32), j);
//            }
//
//        }
//    while(size)
//    {
//        memset(buf_crc, 0xff, 272);
//        if(size >= 256)
//        {
//            encrypt((u32 *)data, buf_crc, 256 / 32);
//            size -= 256;
//            data += 256;
//            len = 272;
//            calc_crc((u32 *)buf_crc, 256 / 32);
//        }
//        else
//        {
//            memset(buf, 0xff, 256);
//            memcpy(buf, data, size);
//            if(size % 32)
//                i = (size + 32) - (size % 32);
//            else
//                i = size;
//            encrypt((u32 *)buf, buf_crc, i / 32);
//            size = 0;
//            len = i * 34 / 32;
//            calc_crc((u32 *)buf_crc, i / 32);
//        }
//
//        flash_write((char *)buf_crc, len, practical_addr);
//        practical_addr += len;
//    }


    flash_write((char *)data, size, address);
//    flash_protection_op(0,FLASH_PROTECT_ALL);
    Lock_MutexPost(flash_mutex);
}

/*һ��sector��4K*/
void djy_flash_erase(uint32_t address)
{
//    u32 practical_addr = address * 34 / 32;
    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
    address &= (0xFFF000);

//    flash_protection_op(0,FLASH_PROTECT_NONE);
    flash_ctrl(CMD_FLASH_ERASE_SECTOR, &address);
//    flash_protection_op(0,FLASH_PROTECT_ALL);
    Lock_MutexPost(flash_mutex);
}


// ============================================================================
// ���ܣ�embeded flash ����
// ������ucmd -- ���
//      ���� -- ���������
// ���أ�
// ��ע��
// ============================================================================
s32 djy_flash_req(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case remain:
        {
            va_list list;
            u32 *left,  *unit;

            left = (u32*)args;
            va_start(list, args);
            unit = (u32*)va_arg(list, u32);
            va_end(list);
            //PagesPerSector��1����Ϊҳ�Ŵ�0��ʼ
            *left = (description->PagesPerSector - 1) - ((*unit / description->BytesPerPage) % description->PagesPerSector);
            break;
        }
        case whichblock:
        {
            va_list list;
            s64 *unit;
            u32 *block;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            *block = *unit / (description->BytesPerPage * description->PagesPerSector);

            break;
        }
        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = description->BytesPerPage;
            break;
        }

        case format:
        {
            va_list list;
            s32 start, end;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            va_end(list);


            if(-1== end)
                end = description->AllSectorNum;// �����ĺţ�

            do
            {
                djy_flash_erase((u32)((--end * ((u32)(description->BytesPerPage * description->PagesPerSector)))+description->MappedStAddr));
            }
            while(end!=start);

            break;
        }

        case mapaddr:
        {

            *((u32*)args) = description->MappedStAddr;
            break;
        }
        case totalblocks:
        {
            *((u32*)args) = description->PagesPerSector;
            break;
        }
        case lock:
        {
            u32 time_ou = args;
            Lock_MutexPend(flash_mutex, time_ou);
            break;
        }
        case unlock:
        {
            Lock_MutexPost(flash_mutex);
            break;
        }

        case checkbad: break;
        default: res = -1; break;
    }

    return (res);
}

// ============================================================================
// ���ܣ�����flash��װ�ļ�ϵͳ
// ������fs -- ��Ҫ���ص��ļ�ϵͳ��mediadrv -- ý��������
//       bstart -- ��ʼ�飬bend -- �����飨�������ÿ飬ֻ���ÿ����һ�飩
// ���أ�0 -- �ɹ��� -1 -- ʧ��
// ��ע��
// ============================================================================
s32 FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv)
{
    u32 units, total = 0;
     char *FullPath,*notfind;
     struct Object *targetobj;
     struct FsCore *super;
     s32 res,endblock = bend;

     if(mediadrv == NULL)
         return -1;
     targetobj = obj_matchpath(fs, &notfind);
     if(notfind)
     {
         error_printf("embed"," not found need to install file system.\r\n");
         return -1;
     }
     super = (struct FsCore *)obj_GetPrivate(targetobj);
     super->MediaInfo = flash_um;
     super->MediaDrv = mediadrv;

     if(-1 == (s32)endblock)
         endblock = bend = description->AllSectorNum; // �����

     super->AreaSize = (bend - bstart) * description->BytesPerPage * description->PagesPerSector;
     super->MediaStart = (bstart * description->BytesPerPage * description->PagesPerSector) + description->MappedStAddr; // ��ʼunit��
     super->MediaStart = super->MediaStart * 34 / 32;
//     super->AreaSize = ((bend * description->BytesPerPage * description->PagesPerSector)- super->MediaStart) * 34 / 32;
     if(super->AreaSize + super->MediaStart > description->AllSectorNum * description->BytesPerPage * description->PagesPerSector)
     {
         super->AreaSize = description->AllSectorNum * description->BytesPerPage * description->PagesPerSector - super->MediaStart;
     }
     res = strlen(flash_name) + strlen(s_ptDeviceRoot->name) + 1;
     FullPath = malloc(res);
     memset(FullPath, 0, res);
     sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,flash_name);   //��ȡ���豸��ȫ·��
     FsBeMedia(FullPath,fs); //�����豸�����ļ�ϵͳ
     free(FullPath);

     printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
     return (0);
}
// ============================================================================
// ���ܣ���ʼ��Ƭ��flash
// ��������
// ���أ�0 -- �ɹ��� -1 -- ʧ��
// ��ע��
// ============================================================================
int ModuleInstall_Flash(void)
{
    description = malloc(sizeof(*description));
    if(!description)
        return (-1);

    SetFlash_Init(description);

    flash_um = malloc(sizeof(struct umedia)+description->BytesPerPage);
    if(!flash_um)
        return (-1);

    flash_um->mreq = djy_flash_req;
    flash_um->type = embed;
    flash_um->ubuf = (u8*)flash_um + sizeof(struct umedia);

    if(!dev_Create((const char*)flash_name, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)flash_um)))
    {
        printf("\r\n: erro : device : %s addition failed.", flash_name);
        free(flash_um);
        return (-1);
    }

    flash_mutex = Lock_MutexCreate("flash_mutex");
    if(flash_mutex==NULL)
        return -1;
    else
    {
        sflashInited = true;
        return 0;
    }

}

// =============================================================================
// ���ܣ��ж�flash�Ƿ�װ
// ������  ��
// ���أ��ѳɹ���װ��true����δ�ɹ���װ��false����
// ��ע��
// =============================================================================
bool_t flash_is_install(void)
{
    return sflashInited;
}



