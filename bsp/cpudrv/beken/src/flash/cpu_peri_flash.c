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
#include <device.h>
#include <device/unit_media.h>
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
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
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
bool_t data_mode = false;      //true����flash�����ݲ�����CRC��false����flash������״̬����CRC

struct NorDescr *nordescription;

extern void calc_crc(u32 *buf, u32 packet_num);
void encrypt(u32 *rx, u8 *tx, u32 num);
//static struct FlashDescr{
//    u32     BytesPerPage;                // ҳ�а������ֽ���
//    u32     PagesPerSector;               //  sector��ҳ��
//    u32     AllSectorNum;               //  ���е�sector��
//    u32     MappedStAddr;
//} *description;

// ============================================================================
// ���ܣ���������FLASH����Ϣ
// ������
// ���أ�
// ��ע��
// ============================================================================
//static s32 SetFlash_Init(struct FlashDescr *Description)
//{
//    Description->BytesPerPage = 256;
//    Description->PagesPerSector = 16;
//    Description->AllSectorNum = 1024;
//    Description->MappedStAddr = 0x0;
//    return (0);
//}

// ============================================================================
// ���ܣ����ò���flash����ʱ�費��Ҫ����CRC
// ������flag:true--��д��������Ѿ���crc�����߲���ҪCRC��
//      false--��Ҫcrc�������ݲ���crc����д�뺯�����CRC
// ���أ�
// ��ע��
// ============================================================================
void SetOperFalshMode(bool_t flag)
{
    data_mode = flag;
}

// ============================================================================
// ���ܣ���ȡ����flash����ʱ�費��Ҫ����CRC
// ������flag:true--����Ҫ����CRC��false--��Ҫ����CRC
// ���أ�
// ��ע��
// ============================================================================
bool_t GetOperFalshMode(void)
{
    return data_mode;
}


static s32 SetFlash_Init(struct NorDescr *Description)
{
    Description->BytesPerPage = 256;
    Description->PagesPerSector = 16;
    Description->SectorNum = 1024;
//    Description->MappedStAddr = 0x0;
    return (0);
}

// ============================================================================
// ���ܣ���ȡ��crc����flash���ݣ����ص������а�����crc���򣬵���Ϊ��ͨ����ʹ��
// ������address -- ��ַ��
//      data -- �����ݻ��棻
//      size -- ��ȡ���ֽ���
// ���أ���
// ��ע��
// ============================================================================
void djy_flash_read_crc(uint32_t address, void *data, uint32_t size)
{
    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
//    flash_protection_op(0,FLASH_PROTECT_NONE);
    flash_read(data, size, address);
//    flash_protection_op(0,FLASH_PROTECT_ALL);
    Lock_MutexPost(flash_mutex);
}
// ============================================================================
// ���ܣ���ȡ����crc��flash���ݣ����Ѷ�����������ȥ��crc���ύ���û���
// ������address -- ��ַ��������crc֮��������ַ��
//      data -- �����ݻ��棻
//      size -- ��ȡ���ֽ���
// ���أ���
// ��ע��
// ============================================================================
void djy_flash_read(uint32_t address, void *data, uint32_t size)
{
    u32 i = 0;   //����ʱ������Ҳ����ƫ�ƣ��Ͳ���һ������xip������ƫ�ơ�
    u8 buf[272];
    if (size == 0)
    {
        return;
    }
    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
    while(size)
    {
        memset(buf, 0xFF, 272);
        flash_read((char *)buf, 272, address);//��ȡ��crc��256�����ݣ���Ϊ��crc������Ҫ��272
        i = 0;
        while(i < 272)  //��crc����ȥ����ֻ����Ч����
        {
            if(size > 32)
                memcpy(data, buf + i, 32);
            else
            {
                memcpy(data, buf + i, size);
                size = 0;
                break;
            }
            data += 32;
            i += 34;
            size -= 32;
        }
        address += 272;
    }
    Lock_MutexPost(flash_mutex);
}
// ============================================================================
// ���ܣ�д��flash��д��ʱ�Ƿ����crc���� GetOperFalshMode() ����ȷ����
// ������address��д���ַ,�������ַ��
//      data -- �����ݻ��棻
//      size -- ��ȡ���ֽ���
// ���أ���
// ��ע��
// ============================================================================
void djy_flash_write(uint32_t address, const void *data, uint32_t size)
{
    u32 i, len;
    u8 buf_crc[272],buf[256];
    if (size == 0)
        return;

    if(address + size >= 0x400000)
    {
        error_printf("embedflash"," write address out of range.\r\n");
        return;
    }

    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
    if(GetOperFalshMode() == true)
    {
        //������crc,���crc����ȷ��Ҳ��������д�뵫�����õ�ַӳ�䷽ʽ���ʣ�����CPU
        //����벻��֪״̬��������Ҳ�޷����ӣ���
        flash_write((char *)data, size, address);
    }
    else
    {
        while(size)             //д����crc�����ݣ����ַ�ʽ���Զ����crc��д��flash
        {
            memset(buf_crc, 0xff, 272);
            if(size >= 256)
            {
                encrypt((u32 *)data, buf_crc, 256 / 32);
                size -= 256;
                data += 256;
                len = 272;
                calc_crc((u32 *)buf_crc, 256 / 32);     //����256�ֽڵ�crc����
            }
            else
            {
                memset(buf, 0xff, 256);
                memcpy(buf, data, size);
                if(size % 32)
                    i = (size + 32) - (size % 32);
                else
                    i = size;
                encrypt((u32 *)buf, buf_crc, i / 32);
                size = 0;
                len = i * 34 / 32;
                calc_crc((u32 *)buf_crc, i / 32);
            }

            flash_write((char *)buf_crc, len, address);
            address += len;
        }
    }

//    flash_write((char *)data, size, practical_addr);
//    flash_protection_op(0,FLASH_PROTECT_ALL);
    Lock_MutexPost(flash_mutex);
}
// ============================================================================
// ���ܣ�д��flash��д��ʱ�Ƿ����crc����GetOperFalshMode() ����ȷ����
// ������address��д���ַ,���GetOperFalshMode==true���������ַ���������߼���ַ
//      data -- �����ݻ��棻
//      size -- ��ȡ���ֽ���
// ���أ���
// ��ע��
// ============================================================================
void djy_flash_write_ori(uint32_t address, const void *data, uint32_t size)
{
    u32 i, len,inaddress;
    u8 buf_crc[272],buf[256];
    if (size == 0)
        return;

    if(address + size >= 0x400000)
    {
        error_printf("embedflash"," write address out of range.\r\n");
        return;
    }

    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
    if(GetOperFalshMode() == true)
    {
        //������crc,���crc����ȷ��Ҳ��������д�뵫�����õ�ַӳ�䷽ʽ���ʣ�����CPU
        //����벻��֪״̬��������Ҳ�޷����ӣ���
        flash_write((char *)data, size, address);
    }
    else
    {
        inaddress = address *34/32; //�߼���ַת��Ϊ�����ַ��
        while(size)             //д����crc�����ݣ����ַ�ʽ���Զ����crc��д��flash
        {
            memset(buf_crc, 0xff, 272);
            if(size >= 256)
            {
                encrypt((u32 *)data, buf_crc, 256 / 32);
                size -= 256;
                data += 256;
                len = 272;
                calc_crc((u32 *)buf_crc, 256 / 32);     //����256�ֽڵ�crc����
            }
            else
            {
                memset(buf, 0xff, 256);
                memcpy(buf, data, size);
                if(size % 32)
                    i = (size + 32) - (size % 32);
                else
                    i = size;
                encrypt((u32 *)buf, buf_crc, i / 32);
                size = 0;
                len = i * 34 / 32;
                calc_crc((u32 *)buf_crc, i / 32);
            }

            flash_write((char *)buf_crc, len, inaddress);
            inaddress += len;
        }
    }

//    flash_write((char *)data, size, practical_addr);
//    flash_protection_op(0,FLASH_PROTECT_ALL);
    Lock_MutexPost(flash_mutex);
}


//-----------------------------------------------------------------------------
//���ܣ�����flashһ��������һ��sector��4K��
//������address�����������������ڵ������ַ
//���أ���
//-----------------------------------------------------------------------------
void djy_flash_erase(uint32_t address)
{
//    u32 practical_addr = address * 34 / 32;
    address &= (0xFFF000);
    if(address >= 0x400000)
        return;
    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
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
            *left = (nordescription->PagesPerSector - 1) - ((*unit / nordescription->BytesPerPage) % nordescription->PagesPerSector);
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

            *block = *unit / (nordescription->BytesPerPage * nordescription->PagesPerSector);

            break;
        }
        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = nordescription->BytesPerPage;
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
                end = nordescription->SectorNum;// �����ĺţ�
            do
            {
                djy_flash_erase((u32)((--end * ((u32)(nordescription->BytesPerPage * nordescription->PagesPerSector)))));
            }
            while(end!=start);

            break;
        }
        case blockunits:
        case totalblocks:
        {
            *((u32*)args) = nordescription->PagesPerSector;
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
// ������fs -- ��Ҫ���ص��ļ�ϵͳ��
//      mediadrv -- ý��������
//      bstart -- ��ʼ�飬
//      bend -- �����飨�������ÿ飬ֻ���ÿ����һ�飩
// ���أ�0 -- �ɹ��� -1 -- ʧ��
// ��ע��todo:�߼����ˣ������ļ�ϵͳ��װ��flash������flash�ҽӵ��ļ�ϵͳ�У����֮��
// ============================================================================
s32 EmbFsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv)
{
     char *FullPath,*notfind;
     struct Object *targetobj;
     struct FsCore *super;
     s32 res,endblock = bend;

     if(mediadrv == NULL)
         return -1;
     targetobj = OBJ_MatchPath(fs, &notfind);
     if(notfind)
     {
         error_printf("embedflash"," not found need to install file system.\r\n");
         return -1;
     }
     super = (struct FsCore *)OBJ_GetPrivate(targetobj);
     super->MediaInfo = flash_um;
     super->MediaDrv = mediadrv;

     super->MediaStart = bstart * nordescription->PagesPerSector; // ��ʼunit��

     //������������������������������оƬ���������ˣ����оƬ������������ʼλ��
     //�����XIP��ָXIP-APP��XIP-IBOOT������������ļ�ϵͳ�� pType ���ˣ�����ҲҪ����Ӧ���޸�
     if(strstr(super->pFsType->pType, "XIP"))
     {

         if(-1 == (s32)endblock)
             endblock = nordescription->SectorNum*32/34;       // �����
         super->AreaSize = ((endblock * nordescription->BytesPerPage * nordescription->PagesPerSector)- (super->MediaStart * nordescription->BytesPerPage)) * 34 / 32;
         if(super->AreaSize + ((super->MediaStart * nordescription->BytesPerPage) * 34 / 32)  >
                                     nordescription->SectorNum * nordescription->BytesPerPage * nordescription->PagesPerSector)
         {
             super->AreaSize = nordescription->SectorNum * nordescription->BytesPerPage *
                     nordescription->PagesPerSector - ((super->MediaStart * nordescription->BytesPerPage) * 34 / 32);
         }
         info_printf("embedf","%s fileOS total capacity(size : %lld)�� available capacity (size : %lld).", fs, super->AreaSize,super->AreaSize / 34 * 32);
     }
     else
     {
         if(-1 == (s32)endblock)
             endblock = nordescription->SectorNum; // �����
         //����XIP���ļ�ϵͳ����ʲô�������ʲô���򣬲���ȥ��*34/32��ƫ�ƣ���ʹ��ʱ����Ҫ���м����λ�á�
         super->AreaSize = (endblock * nordescription->BytesPerPage * nordescription->PagesPerSector)- (super->MediaStart * nordescription->BytesPerPage);
         if(super->AreaSize + (super->MediaStart * nordescription->BytesPerPage) >
                                     nordescription->SectorNum * nordescription->BytesPerPage * nordescription->PagesPerSector)
         {
             super->AreaSize = nordescription->SectorNum * nordescription->BytesPerPage *
                     nordescription->PagesPerSector - (super->MediaStart * nordescription->BytesPerPage);
         }
         info_printf("embedf","%s fileOS available capacity (size : %lld).", fs, super->AreaSize);
     }
     res = strlen(flash_name) + strlen(s_ptDeviceRoot->name) + 1;
     FullPath = malloc(res);
     memset(FullPath, 0, res);
     sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,flash_name);   //��ȡ���豸��ȫ·��
     File_BeMedia(FullPath,fs); //�����豸�����ļ�ϵͳ
     free(FullPath);
     info_printf("embedf","device : %s added(start:%d, end:%d).", fs, bstart, bend);
     return (0);
}
// ============================================================================
// ���ܣ���ʼ��Ƭ��flash
// ��������
// ���أ�0 -- �ɹ��� -1 -- ʧ��
// ��ע��
// ============================================================================
s32 ModuleInstall_Flash(void)
{
    if(!nordescription) //��ʼ��nor����Ϣ
    {
        nordescription = malloc(sizeof(struct NorDescr));
        if(!nordescription)
        {
            printf("\r\n: erro : device : memory out.\r\n");
            return -1;
        }
        memset(nordescription, 0x0, (sizeof(struct NorDescr)));
    }
    SetFlash_Init(nordescription);


    flash_um = malloc(sizeof(struct umedia)+nordescription->BytesPerPage);
    if(!flash_um)
        return (-1);

    flash_um->mreq = djy_flash_req;
    flash_um->type = embed;
    flash_um->ubuf = (u8*)flash_um + sizeof(struct umedia);

    if(!Device_Create((const char*)flash_name, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)flash_um)))
    {
        error_printf("embedflash","device : %s addition failed.", flash_name);
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
// ��������
// ���أ��ѳɹ���װ��true����δ�ɹ���װ��false����
// ��ע��
// =============================================================================
bool_t flash_is_install(void)
{
    return sflashInited;
}

//����Իᵼ��BK7251�ڲ�flash��crcУ����ң���ֻҪ����crc��������CPU�ͻ����һ��
//������֪��״̬����������Ҳ�����ϣ�ֻ��ʹ����¼�����ճ��򣬸�������Դ�����������ѡ�
//���ñ�����ǰ���������δִ�е�crc���ҵĵط������ܹ�����׽������ˣ�������Ӧ�����
//�á�һ����˵���ڳ���ո�����ʱ������ã���ʱ��δ��ʼ�����ڣ��޷���ӡ��

struct flash_CrcUnit
{
    u8 DataUnit[32];
    u16 CrcUnit;
};
//start��������ʼ��ַ��len�������ȣ�����1088������������Ӧ�߼�����1Kbytes��
bool_t flash_CheckCrc(u32 start, u32 len)
{
    struct flash_CrcUnit flashbuf[32];
    u16 flashcrc[32];
    bool_t result = true;;
    u32 address,verify;
    for(address = start; address < len; address +=sizeof(flashbuf))
    {
        flash_read((char *)flashbuf, sizeof(flashbuf), address);
        for(verify = 0; verify < 32; verify++)
            flashcrc[verify] = flashbuf[verify].CrcUnit;
        calc_crc((u32 *)flashbuf, 32);     //����32����Ԫ��crc����
        for(verify = 0; verify < 32; verify++)
        {
            if(flashcrc[verify] != flashbuf[verify].CrcUnit)
            {
                result = false;
                return result;
            }
        }
    }
    return result;
}

