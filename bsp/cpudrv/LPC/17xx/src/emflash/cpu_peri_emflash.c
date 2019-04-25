//-----------------------------------------------------------------------------
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

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <device.h>
#include <device/flash/flash.h>
#include <cpu_peri.h>
#include <int.h>
#include <device/include/unit_media.h>
#include <dbug.h>
#include <xip.h>
#include <filesystems.h>
#include <math.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//s32 ModuleInstall_EmbededFlash(u32 doformat);
//ModuleInstall_EmbededFlash(CFG_EFLASH_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_emflash"     //Ƭ��flash
//parent:"none"                          //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                         //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                      //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                       //��ʼ��ʱ������ѡֵ��early��medium��later��
                                        //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"devfile","lock" //������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г�
//weakdependence:"none"                 //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                      //������Ļ����������������none����ʾ�޻����������
                                    //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_EFLASH_PART_FORMAT   //****�������Ƿ��Ѿ����ú�
#warning    cpu_peri_emflash �������δ���ã�ʹ��Ĭ��ֵ
//%$#@enum,true,false,
#define CFG_EFLASH_PART_FORMAT     false      //����ѡ��,�Ƿ���Ҫ������оƬ��
//%$#@string,1,32,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
// ============================================================================


static struct EmbdFlashDescr{
    u16     BytesPerPage;                // ҳ�а������ֽ���
    u16     PagesPerSmallSect;           // small sector��ҳ��
    u16     PagesPerLargeSect;           // large sector��ҳ��
    u16     PagesPerNormalSect;          // Normal sector��ҳ��
    u16     SmallSectorsPerPlane;        // ÿplane��small sector��
    u16     LargeSectorsPerPlane;        // ÿplane��large sector��
    u16     NormalSectorsPerPlane;       // ÿplane��normal sector��
    u16     Planes;                      // plane��
    u32     MappedStAddr;
} *sp_tFlashDesrc;

static const char *EmflashName = "emflash";      //��flash��obj�ڵ�����
static struct umedia *emflash_um;
extern struct Object *s_ptDeviceRoot;
s32 __embed_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
s32 __embed_read(s64 unit, void *data, struct uopt opt);
s32 __embed_req(enum ucmd cmd, ptu32_t args, ...);
s32 __embed_write(s64 unit, void *data, struct uopt opt);
s32 __embed_erase(s64 unit, struct uesz sz);

typedef struct __NxpIAP
{
    unsigned long cmd;           // Command
    unsigned long par[4];        // Parameters
    unsigned long stat;          // Status
    unsigned long res[2];        // Result
}tagIAP;

/* IAP Call */
typedef void (*IAP_Entry) (unsigned long *cmd, unsigned long *stat);
#define IAP_Call ((IAP_Entry) 0x1FFF1FF1)

#define CLCK 100000
static tagIAP sIAP;


extern u32 gc_ptIbootSize;
extern u32 gc_ptFlashOffset;
//-----------------------------------------------------------------------------
//����: ��ȡ����FLASH����Ϣ
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static s32 Flash_Init(struct EmbdFlashDescr *Description)
{
    sIAP.cmd = 54;
    sIAP.stat = 0;

    IAP_Call(&sIAP.cmd,&sIAP.stat);

    Description->BytesPerPage = 1024;
    Description->PagesPerSmallSect = 4;
    Description->PagesPerLargeSect = 0;
    Description->PagesPerNormalSect = 32;
    Description->SmallSectorsPerPlane = 16;
    Description->LargeSectorsPerPlane = 0;
    Description->NormalSectorsPerPlane = 14;
    Description->Planes = 1;
    Description->MappedStAddr = gc_ptFlashOffset;
    return (0);
}

//-----------------------------------------------------------------------------
//����: ��ȡ����FLASH����Ϣ
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static s32 Flash_GetDescr(struct EmFlashDescr *Description)
{
    Description->BytesPerPage = sp_tFlashDesrc->BytesPerPage;
    Description->TotalPages = (sp_tFlashDesrc->PagesPerSmallSect *
                               sp_tFlashDesrc->SmallSectorsPerPlane +
                               sp_tFlashDesrc->PagesPerLargeSect *
                               sp_tFlashDesrc->LargeSectorsPerPlane +
                               sp_tFlashDesrc->PagesPerNormalSect *
                               sp_tFlashDesrc->NormalSectorsPerPlane) *
                              sp_tFlashDesrc->Planes;
    Description->ReservedPages = gc_ptIbootSize / sp_tFlashDesrc->BytesPerPage;
    Description->MappedStAddr = sp_tFlashDesrc->MappedStAddr;
    return (0);
}
//-----------------------------------------------------------------------------
//����: ������
//����: BlkNo -- ���;
//����: "0" -- �ɹ�;"-1" -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
static s32 Flash_SectorEarse(u32 SectorNo)
{
    sIAP.cmd    = 50;                             // Prepare Sector for Erase
    sIAP.par[0] = SectorNo;                         // Start Sector
    sIAP.par[1] = SectorNo;                         // End Sector
    IAP_Call (&sIAP.cmd, &sIAP.stat);             // Call IAP Command
    if (sIAP.stat)
        return (1);                               // Command Failed

    sIAP.cmd    = 52;                             // Erase Sector
    sIAP.par[0] = SectorNo;                         // Start Sector
    sIAP.par[1] = SectorNo;                         // End Sector
    sIAP.par[2] = CLCK;                           // CCLK in kHz
    IAP_Call (&sIAP.cmd, &sIAP.stat);             // Call IAP Command
    if (sIAP.stat)
        return (-1);                              // Command Failed
    return (0);
}
//-----------------------------------------------------------------------------
//����: дĳҳ
//����: PageNo -- ҳ��;
//      Data -- д����;
//      Flags -- д��������λ;
//����: ">0" -- ������д����ֽ���;
//      "-1" -- ��������;
//      "-2" -- дʧ��;
//��ע:
//-----------------------------------------------------------------------------
static s32 Flash_PageProgram(u32 Page, u8 *Data, u32 Flags)
{
    u32 Addr;
    u32 Sector;

    if(!Data)
        return (-1);

    Addr = Page * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr;
    Sector = Addr >> 12;                          //  4kB Sector
    if (Sector >= 0x10)
    {
        Sector = 0x0E + (Sector >> 3);            // 32kB Sector
    }

    sIAP.cmd    = 50;                             // Prepare Sector for Write
    sIAP.par[0] = Sector;                         // Start Sector
    sIAP.par[1] = Sector;                         // End Sector
    IAP_Call (&sIAP.cmd, &sIAP.stat);             // Call IAP Command
    if (sIAP.stat)
        return (-2);                              // Command Failed

    Djy_EventDelay(2000);
    sIAP.cmd    = 51;                             // Copy RAM to Flash
    sIAP.par[0] = Addr;                           // Destination Flash Address
    sIAP.par[1] = (unsigned long)Data;            // Source RAM Address
    sIAP.par[2] = sp_tFlashDesrc->BytesPerPage;   // Fixed Page Size
    sIAP.par[3] = CLCK;                           // CCLK in kHz
    IAP_Call (&sIAP.cmd, &sIAP.stat);             // Call IAP Command
    if (sIAP.stat)
        return (1);                    // Command Failed


//  Djy_EventDelay(1000);

    return (sp_tFlashDesrc->BytesPerPage);                                  // Finished without Errors
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static s32 Flash_PageRead(u32 Page, u8 *Data, u32 Flags)
{
    u32 Addr = Page * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr;

    if(!Data)
        return (-1);

    memcpy(Data, (u8*)Addr, sp_tFlashDesrc->BytesPerPage);
    return (sp_tFlashDesrc->BytesPerPage);

}

//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static s32 Flash_PageToSector(u32 PageNo, u32 *Remains, u32 *SectorNo)
{
    u32 PagesLeft, PagesDone;
    u32 i;
    u32 Sector;

    if((!Remains) || (!SectorNo))
        return (-1);

    Sector = 0;
    PagesDone = 0;

    PagesLeft = sp_tFlashDesrc->PagesPerSmallSect -
               (PageNo % sp_tFlashDesrc->PagesPerSmallSect);
    for(i = 1; i <= sp_tFlashDesrc->SmallSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + sp_tFlashDesrc->PagesPerSmallSect * i))
            goto DONE;
        Sector++;
    }

    PagesDone += sp_tFlashDesrc->SmallSectorsPerPlane *
                 sp_tFlashDesrc->PagesPerSmallSect;
//  PagesLeft = sp_tFlashDesrc->PagesPerLargeSect -
//                 (PageNo % sp_tFlashDesrc->PagesPerLargeSect);
//  for(i = 1; i <= sp_tFlashDesrc->LargeSectorsPerPlane; i++)
//  {
//      if(PageNo < (PagesDone + sp_tFlashDesrc->PagesPerLargeSect * i))
//          goto DONE;
//      Sector++;
//  }
//
//  PagesDone += sp_tFlashDesrc->LargeSectorsPerPlane *
//                   sp_tFlashDesrc->PagesPerLargeSect;
    PagesLeft = sp_tFlashDesrc->PagesPerNormalSect -
                   (PageNo % sp_tFlashDesrc->PagesPerNormalSect);
    for(i = 1; i <= sp_tFlashDesrc->NormalSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + sp_tFlashDesrc->PagesPerNormalSect * i))
            goto DONE;
        Sector++;
    }

    return (-1);

DONE:
    *SectorNo = Sector; // sector�����
    *Remains = PagesLeft - 1; // page�����
    return (0);
}

//-----------------------------------------------------------------------------
// ���ܣ���װƬ��Flash����
// ������doformat -- �Ƿ��ʽ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע���������֪��Ҫ��װʲô�ļ�ϵͳ�����߲���װ�ļ�ϵͳTargetFs��NULL��TargetPart��-1��
//-----------------------------------------------------------------------------
s32 ModuleInstall_EmbededFlash(u32 doformat)
{
    static u8 emflashinit = 0;
    u32 units;


    if(sp_tFlashDesrc)
        return (-4); // �豸��ע��

    sp_tFlashDesrc = malloc(sizeof(*sp_tFlashDesrc));
    if(!sp_tFlashDesrc)
        return (-1);

    Flash_Init(sp_tFlashDesrc);

    if(doformat)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __embed_req(format, (ptu32_t)0 , -1, &sz);           //��ʽ��ָ������
    }

    if(emflashinit == 0)
    {
        emflash_um = malloc(sizeof(struct umedia)+sp_tFlashDesrc->BytesPerPage);
        if(!emflash_um)
        {
            return (-1);
        }

        emflash_um->mreq = __embed_req;
        emflash_um->type = embed;
        emflash_um->ubuf = (u8*)emflash_um + sizeof(struct umedia);

        if(!dev_Create((const char*)EmflashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)emflash_um)))
        {
            printf("\r\n: erro : device : %s addition failed.", EmflashName);
            free(emflash_um);
            return (-1);
        }
        emflashinit = 1;
    }

    return (0);
}

// ============================================================================
// ���ܣ�embeded flash ����
// ������ucmd -- ���
//      ���� -- ���������
// ���أ�
// ��ע��
// ============================================================================
s32 __embed_req(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case remain:
        {
            va_list list;
            u32 *left;
            s64 *unit;

            left = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            if(*unit < (sp_tFlashDesrc->SmallSectorsPerPlane * sp_tFlashDesrc->PagesPerSmallSect))
            {
                *unit = (*unit % sp_tFlashDesrc->PagesPerSmallSect) + 1;        //�����ҳ���ڿ��λ��
                *left = sp_tFlashDesrc->PagesPerSmallSect - *unit;
            }
            else if(*unit < ((sp_tFlashDesrc->NormalSectorsPerPlane * sp_tFlashDesrc->PagesPerNormalSect) +
                                        (sp_tFlashDesrc->SmallSectorsPerPlane * sp_tFlashDesrc->PagesPerSmallSect)))
            {
                *unit = *unit - (sp_tFlashDesrc->SmallSectorsPerPlane * sp_tFlashDesrc->PagesPerSmallSect); //  ��ȥ��һ���ֵ�ҳ��
                *unit = (*unit % sp_tFlashDesrc->PagesPerNormalSect) + 1;        //�����ҳ���ڿ��λ��
                *left = sp_tFlashDesrc->PagesPerNormalSect - *unit;
            }
            else
            {
                res = -1;
            }

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

            if(*unit < (sp_tFlashDesrc->SmallSectorsPerPlane * sp_tFlashDesrc->PagesPerSmallSect))
            {
                *block = *unit / sp_tFlashDesrc->PagesPerSmallSect;
            }
            else if(*unit < ((sp_tFlashDesrc->NormalSectorsPerPlane * sp_tFlashDesrc->PagesPerNormalSect) +
                                        (sp_tFlashDesrc->SmallSectorsPerPlane * sp_tFlashDesrc->PagesPerSmallSect)))
            {
                *unit = *unit - (sp_tFlashDesrc->SmallSectorsPerPlane * sp_tFlashDesrc->PagesPerSmallSect); //  ��ȥ��һ���ֵ�ҳ��
                *block = (*unit / sp_tFlashDesrc->PagesPerNormalSect) + sp_tFlashDesrc->SmallSectorsPerPlane;
            }
            else
            {
                res = -1;
            }

            break;
        }

        case totalblocks:
        {
            *((u32*)args) =  sp_tFlashDesrc->SmallSectorsPerPlane + sp_tFlashDesrc->NormalSectorsPerPlane;
            break;
        }

        case blockunits:
        {
            va_list list;
            u32 *units = (u32*)args;
            u32 block;

            va_start(list, args);
            block = (u32)va_arg(list, u32);
            va_end(list);

            if(block < sp_tFlashDesrc->SmallSectorsPerPlane)
            {
                *units = sp_tFlashDesrc->PagesPerSmallSect;
            }
            else if(block < sp_tFlashDesrc->SmallSectorsPerPlane + sp_tFlashDesrc->NormalSectorsPerPlane)
            {
                *units = sp_tFlashDesrc->PagesPerNormalSect;
            }
            else
                res = -1;

            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = sp_tFlashDesrc->BytesPerPage;
            break;
        }

        case format:
        {
            va_list list;
            s32 start, end;
            struct uesz *sz;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(!sz->block)
                return (-1);

            if(-1==(s32)end)
                end = sp_tFlashDesrc->SmallSectorsPerPlane + sp_tFlashDesrc->NormalSectorsPerPlane; // �����ĺţ�

            do
            {
                if(__embed_erase((s64)--end, *sz))
                {
                    res = -1;
                    break;
                }
            }
            while(end!=start);

            break;
        }

        case mapaddr:
        {

            *((u32*)args) = sp_tFlashDesrc->MappedStAddr;
            break;
        }
        case checkbad: break;
        default: res = -1; break;
    }

    return (res);
}

// ============================================================================
// ���ܣ�embed ����
// ������unit -- ������ţ�ҳ����
//      data -- �������ݣ�
//      opt -- ���ķ�ʽ��
// ��ע��
// ============================================================================
s32 __embed_read(s64 unit, void *data, struct uopt opt)
{
    s32 res;

    res = Flash_PageRead((u32)unit, data, 0);
    if(res!=sp_tFlashDesrc->BytesPerPage)
        return (-1);

    return (0);
}

// ============================================================================
// ���ܣ�embed д��
// ������unit -- д����ţ�ҳ����
//      data -- д�����ݣ�
//      opt -- д�ķ�ʽ��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 __embed_write(s64 unit, void *data, struct uopt opt)
{
    s32 res;

    res = Flash_PageProgram((u32)unit, data, 0);
    if(res!=sp_tFlashDesrc->BytesPerPage)
        return (-1);

    return (0);
}

// ============================================================================
// ���ܣ�embed ����
// ������unit -- ��������ţ�
//      sz -- �����ĵ�λ��unit��block��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 __embed_erase(s64 unit, struct uesz sz)
{
    u32 block;

    if(sz.unit)
    {
        if(__embed_req(whichblock, (ptu32_t)&block, &unit))
            return (-1);
    }
    else
        block = (u32)unit;

    return (Flash_SectorEarse(block));
}


// ============================================================================
// ���ܣ���ʼ��Ƭ��flash
// ������fs -- ��Ҫ���ص��ļ�ϵͳ��mediadrv -- ý��������
//       bstart -- ��ʼ�飬bend -- �����飨�������ÿ飬ֻ���ÿ����һ�飩
// ���أ�0 -- �ɹ��� -1 -- ʧ��
// ��ע��
// ============================================================================
s32 __embed_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv)
{
    u32 units, total = 0;
    char *FullPath,*notfind;
    struct obj *targetobj;
    struct FsCore *super;
    s32 res,endblock = bend;
    targetobj = obj_matchpath(fs, &notfind);
    if(notfind)
    {
        error_printf("embed"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)obj_GetPrivate(targetobj);
    super->MediaInfo = emflash_um;
    super->MediaDrv = mediadrv;

    if(-1 == (s32)endblock)
    {
        endblock = bend = sp_tFlashDesrc->SmallSectorsPerPlane + sp_tFlashDesrc->NormalSectorsPerPlane; // �����
    }

    do
    {
        if(__embed_req(blockunits, (ptu32_t)&units, --endblock))
        {
            return (-1);
        }

        total += units;     //����÷���һ���ж���ҳ
    }
    while(endblock!=bstart);

    super->AreaSize = total * sp_tFlashDesrc->BytesPerPage;
    endblock = 0;
    total = 0;
    while(endblock<bstart)
    {
        if(__embed_req(blockunits, (ptu32_t)&units, endblock++))
        {
            return (-1);
        }
        total += units;
    }

    super->MediaStart = total; // ��ʼunit��

    res = strlen(EmflashName) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,EmflashName);   //��ȡ���豸��ȫ·��
    FsBeMedia(FullPath,fs); //�����豸�����ļ�ϵͳ
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);

}
