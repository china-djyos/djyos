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

#include <string.h>
#include <stdlib.h>
#include <device.h>
#include <device/flash/flash.h>
#include <cpu_peri.h>
#include <stm32l4xx_hal_flash.h>
#include <int.h>
#include <math.h>
#include <dbug.h>
#include <xip.h>
#include <filesystems.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//s32 ModuleInstall_EmbededFlash(u32 doformat);
//ModuleInstall_EmbededFlash(CFG_EFLASH_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu drive inner flash"//Ƭ��flash
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                         //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                      //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                       //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                        //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","lock"//������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г�
//weakdependence:"xip app file system","xip iboot file system"  //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                          //������Ļ����������������none����ʾ�޻����������
                                        //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_DRIVE_INNER_FLASH == false )
//#warning  " cpu_drive_inner_flash  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_DRIVE_INNER_FLASH    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
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

//����ķ�����ʼ�ͷ���������д����-1��10000��������ֵΪ��NormalSectorsPerPlane��

static struct EmbdFlashDescr{
    u16     BytesPerPage;                   // ҳ�а������ֽ���
    u16     PagesPerSmallSect;           // small sector��ҳ��
    u16     PagesPerLargeSect;           // large sector��ҳ��
    u16     PagesPerNormalSect;          // Normal sector��ҳ��
    u16     SmallSectorsPerPlane;        // ÿplane��small sector��
    u16     LargeSectorsPerPlane;        // ÿplane��large sector��
    u16     NormalSectorsPerPlane;       // ÿplane��normal sector��
    u16     Planes;                      // plane��
    u32     MappedStAddr;
} *s_ptEmbdFlash;

extern u32 gc_ptIbootSize;
//extern u32 gc_ptFlashOffset;
extern u32 gc_ptFlashRange;

static const char *EmflashName = "emflash";      //��flash��obj�ڵ�����
static struct umedia *emflash_um;
extern struct Object *s_ptDeviceRoot;
static bool_t sEmflashInited = false;
s32 __embed_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
s32 __embed_read(s64 unit, void *data, struct uopt opt);
s32 __embed_req(enum ucmd cmd, ptu32_t args, ...);
s32 __embed_write(s64 unit, void *data, struct uopt opt);
s32 __embed_erase(s64 unit, struct uesz sz);

// ============================================================================
// ����: ι��
// ����:
// ����:
// ��ע: �������ļ�max706.cʵ��
// ============================================================================
extern bool_t BrdWdt_FeedDog(void) __attribute__((weak));
bool_t BrdWdt_FeedDog(void)
{
    return (TRUE); // �պ���
}

// ============================================================================
// ����: ��ȡ����FLASH����Ϣ
// ����:
// ����:
// ��ע:
// ============================================================================
static s32 EmFlash_Init(struct EmbdFlashDescr *Description)
{
    Description->BytesPerPage = 512;
    Description->PagesPerSmallSect = 4;
    Description->PagesPerLargeSect = 4;
    Description->PagesPerNormalSect = 4;
    Description->SmallSectorsPerPlane = 0;
    Description->LargeSectorsPerPlane = 0;
    Description->NormalSectorsPerPlane = gc_ptFlashRange/2048;
    Description->Planes = 1;
    Description->MappedStAddr = 0x08000000;
    return (0);
}

// ============================================================================
// ����: ��ȡ����FLASH����Ϣ
// ����:
// ����:
// ��ע:
// ============================================================================
static s32 Flash_GetDescr(struct EmFlashDescr *Description)
{
    Description->BytesPerPage = s_ptEmbdFlash->BytesPerPage;
    Description->TotalPages = (s_ptEmbdFlash->PagesPerSmallSect *
                        s_ptEmbdFlash->SmallSectorsPerPlane +
                        s_ptEmbdFlash->PagesPerLargeSect *
                        s_ptEmbdFlash->LargeSectorsPerPlane +
                        s_ptEmbdFlash->PagesPerNormalSect *
                        s_ptEmbdFlash->NormalSectorsPerPlane) *
                       s_ptEmbdFlash->Planes;
    Description->ReservedPages = gc_ptIbootSize / s_ptEmbdFlash->BytesPerPage;
    Description->MappedStAddr = s_ptEmbdFlash->MappedStAddr;
    return (0);
}
// ============================================================================
// ����: ������
// ����: BlkNo -- ���;
// ����: "0" -- �ɹ�;"-1" -- ʧ��;
// ��ע:
// ============================================================================
static s32 Flash_SectorEarse(u32 SectorNo)
{
    s32 Ret;
    u32 SECTORError=0;//�������������Ϣ
    u8 bank,page;
    static FLASH_EraseInitTypeDef EraseInitStruct;

    if(SectorNo < gc_ptFlashRange/4096)
    {
        bank = FLASH_BANK_1;
        page = SectorNo;
    }
    else
    {
        bank = FLASH_BANK_2;
        page = SectorNo - gc_ptFlashRange/4096;
    }

    HAL_FLASH_Unlock();

    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Banks       = bank;
    EraseInitStruct.Page        = page;
    EraseInitStruct.NbPages     = 1;

    BrdWdt_FeedDog(); // ι��
    Int_CutTrunk();
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
         Ret=-1;
    else
        Ret=0;
    Int_ContactTrunk();
    BrdWdt_FeedDog(); // ι��

    HAL_FLASH_Lock();

    return Ret;
}
// ============================================================================
// ����: дĳҳ
// ����: PageNo -- ҳ��;
//       Data -- д����;
//       Flags -- д��������λ;
// ����: ">0" -- ������д����ֽ���;
//       "-1" -- ��������;
//       "-2" -- дʧ��;
// ��ע:
// ============================================================================
static s32 Flash_PageProgram(u32 Page, u8 *Data, u32 Flags)
{
    u32 Ret,i;
    u64 *pData = (u64*)Data;

    u32 Addr = Page * s_ptEmbdFlash->BytesPerPage + s_ptEmbdFlash->MappedStAddr;

    if(!Data)
        return (-1);

    HAL_FLASH_Unlock();

    for(i = 0; i < s_ptEmbdFlash->BytesPerPage;)
    {
        if(*(u64*)Addr != *pData)
        {
            BrdWdt_FeedDog(); // ι��
            Int_CutTrunk();
            Ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,Addr,*pData);
            BrdWdt_FeedDog(); // ι��
            Int_ContactTrunk();
            if(Ret != HAL_OK)
            {
                break;
            }
        }
        pData++;
        i += 8;
        Addr += 8;
    }

    HAL_FLASH_Lock();

    return i;
}
// ============================================================================
// ����:
// ����:
// ����:
// ��ע:
// ============================================================================
static s32 Flash_PageRead(u32 Page, u8 *Data, u32 Flags)
{
    u32 Addr = Page * s_ptEmbdFlash->BytesPerPage + s_ptEmbdFlash->MappedStAddr;

    if(!Data)
        return (-1);

    memcpy(Data, (u8*)Addr, s_ptEmbdFlash->BytesPerPage);
    return (s_ptEmbdFlash->BytesPerPage);
}

// ============================================================================
// ����:
// ����:
// ����:
// ��ע:
// ============================================================================
static s32 Flash_PageToSector(u32 PageNo, u32 *Remains, u32 *SectorNo)
{
    u32 PagesLeft, PagesDone;
    u32 i;
    u32 Sector;

    if((!Remains) || (!SectorNo))
        return (-1);

    Sector = 0;
    PagesDone = 0;
    PagesLeft = s_ptEmbdFlash->PagesPerSmallSect -
               (PageNo % s_ptEmbdFlash->PagesPerSmallSect);
    for(i = 1; i <= s_ptEmbdFlash->SmallSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + s_ptEmbdFlash->PagesPerSmallSect * i))
            goto DONE;
        Sector++;
    }

    PagesDone += s_ptEmbdFlash->SmallSectorsPerPlane *
            s_ptEmbdFlash->PagesPerSmallSect;
    PagesLeft = s_ptEmbdFlash->PagesPerLargeSect -
                   (PageNo % s_ptEmbdFlash->PagesPerLargeSect);
    for(i = 1; i <= s_ptEmbdFlash->LargeSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + s_ptEmbdFlash->PagesPerLargeSect * i))
            goto DONE;
        Sector++;
    }

    PagesDone += s_ptEmbdFlash->LargeSectorsPerPlane *
            s_ptEmbdFlash->PagesPerLargeSect;
    PagesLeft = s_ptEmbdFlash->PagesPerNormalSect -
                   (PageNo % s_ptEmbdFlash->PagesPerNormalSect);
    for(i = 1; i <= s_ptEmbdFlash->NormalSectorsPerPlane; i++)
    {
        if(PageNo < (PagesDone + s_ptEmbdFlash->PagesPerNormalSect * i))
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
    if(s_ptEmbdFlash)
        return (-4); // �豸��ע��

    s_ptEmbdFlash = malloc(sizeof(*s_ptEmbdFlash));
    if(!s_ptEmbdFlash)
        return (-1);

    EmFlash_Init(s_ptEmbdFlash);

    if(doformat)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __embed_req(format, (ptu32_t)0 , -1, &sz);           //��ʽ��ָ������
    }

    emflash_um = malloc(sizeof(struct umedia)+s_ptEmbdFlash->BytesPerPage);
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
    sEmflashInited = true;
    return (0);
}
// =============================================================================
// ���ܣ��ж�emflash�Ƿ�װ
// ������  ��
// ���أ��ѳɹ���װ��true����δ�ɹ���װ��false����
// ��ע��
// =============================================================================
bool_t emflash_is_install(void)
{
    return sEmflashInited;
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
            s64 *unit , unitlocation;


            left = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            if(*unit < (s_ptEmbdFlash->NormalSectorsPerPlane * s_ptEmbdFlash->PagesPerNormalSect))
            {
                //�����ҳ�����ڿ��е�λ��
                unitlocation = *unit % s_ptEmbdFlash->PagesPerNormalSect;
                *left = s_ptEmbdFlash->PagesPerNormalSect - unitlocation + 1;   //��1����Ϊҳ���Ǵ�0��ʼ���
            }
            else
                res = -1;

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

            if(*unit < (s_ptEmbdFlash->NormalSectorsPerPlane * s_ptEmbdFlash->PagesPerNormalSect))
            {
                *block = *unit / s_ptEmbdFlash->PagesPerNormalSect;
            }
            else
                res = -1;

            break;
        }

        case totalblocks:
        {
            *((u32*)args) =  s_ptEmbdFlash->NormalSectorsPerPlane;
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

            if(*block <= s_ptEmbdFlash->NormalSectorsPerPlane)
            {
                *units = s_ptEmbdFlash->PagesPerNormalSect;;
            }
            else
                res = -1;

            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = s_ptEmbdFlash->BytesPerPage;
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
                end = s_ptEmbdFlash->NormalSectorsPerPlane;; // �����ĺţ�

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

            *((u32*)args) = s_ptEmbdFlash->MappedStAddr;
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
    if(res!=s_ptEmbdFlash->BytesPerPage)
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
    if(res!=s_ptEmbdFlash->BytesPerPage)
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
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res,BlockNum;
    targetobj = obj_matchpath(fs, &notfind);
    if(notfind)
    {
        error_printf("embed"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)obj_GetPrivate(targetobj);
    super->MediaInfo = emflash_um;
    super->MediaDrv = mediadrv;
    if((s32)bend == -1)
    {
        bend = s_ptEmbdFlash->NormalSectorsPerPlane; // �����
        BlockNum = bend - bstart;
    }
    else
    {
        BlockNum = bend - bstart;
    }
    super->AreaSize = BlockNum * s_ptEmbdFlash->PagesPerSmallSect * s_ptEmbdFlash->PagesPerNormalSect;
    super->MediaStart = bstart * s_ptEmbdFlash->PagesPerSmallSect; // ��ʼunit��


    res = strlen(EmflashName) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,EmflashName);   //��ȡ���豸��ȫ·��
    FsBeMedia(FullPath,fs); //�����豸�����ļ�ϵͳ
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);

}

