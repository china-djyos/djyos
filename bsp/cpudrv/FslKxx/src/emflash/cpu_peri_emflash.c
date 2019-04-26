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
//weakdependence:"xip_app","xip_iboot"                 //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                          //������������������������none����ʾ�������������
                                        //����������������������г�
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

//���������ѡ���У���ʼ�鵽������������ʱд����-1~10000,֪��׼ȷ��Χ����Ҫ�޸ģ�����NormalSectorsPerPlane��ֵ��

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
static bool_t sEmflashInited = false;
s32 __embed_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
s32 __embed_read(s64 unit, void *data, struct uopt opt);
s32 __embed_req(enum ucmd cmd, ptu32_t args, ...);
s32 __embed_write(s64 unit, void *data, struct uopt opt);
s32 __embed_erase(s64 unit, struct uesz sz);

void Cache_Invalidate(void)
{

//  if (cache == CODE_CACHE)
//  {
      /* Cache Set Command: set command bits in CCR */
      /* set invalidate way 1 and invalidate way 0 bits */
      LMEM->PCCCR = LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK;

      /* set ccr[go] bit to initiate command to invalidate cache */
      LMEM->PCCCR |= LMEM_PCCCR_GO_MASK;

      /* wait until the ccr[go] bit clears to indicate command complete */
      while(((LMEM->PCCCR & LMEM_PCCCR_GO_MASK)>>LMEM_PCCCR_GO_SHIFT)== 0x1)
      {};

//  }else if (cache == SYS_CACHE)
//  {
      /* Cache Set Command: set command bits in CCR */
      /* set invalidate way 1 and invalidate way 0 bits */
      LMEM->PSCCR = LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK;

      /* set ccr[go] bit to initiate command to invalidate cache */
      LMEM->PSCCR |= LMEM_PSCCR_GO_MASK;

      /* wait until the ccr[go] bit clears to indicate command complete */
      while(((LMEM->PSCCR & LMEM_PSCCR_GO_MASK)>>LMEM_PSCCR_GO_SHIFT)== 0x1)
      {};

//  } else
//  {
//     return CACHE_INVALID_PARAM;
//  }

//  return CACHE_OK;

}
static void Kinetis_FlashInit(void)
{
//  Cache_Disable();
    //���Flash���ʴ���
    if (FTFE->FSTAT & FTFE_FSTAT_ACCERR_MASK)
    {
    FTFE->FSTAT |= FTFE_FSTAT_ACCERR_MASK;//��������־
    }
    //��鱣������
    else if (FTFE->FSTAT & FTFE_FSTAT_FPVIOL_MASK)
    {
    FTFE->FSTAT |= FTFE_FSTAT_FPVIOL_MASK;
    }
    else if (FTFE->FSTAT & FTFE_FSTAT_RDCOLERR_MASK)
    {
    //������ͻ����
    FTFE->FSTAT |= FTFE_FSTAT_RDCOLERR_MASK;
    } /* EndIf */

    //����Flashģ������ݻ���
    FMC->PFB01CR &= ~FMC_PFB01CR_B01DCE_MASK;
    FMC->PFB23CR &= ~FMC_PFB23CR_B23DCE_MASK;

    while(!(FTFE->FSTAT & FTFE_FSTAT_CCIF_MASK));    // �ȴ��������
}
static s32 Kinetis_CmdLaunch(void)
{
    // ������ʴ����־λ�ͷǷ����ʱ�־λ
    FTFE->FSTAT = (1<<5) | (1<<4);

    // ��������
    FTFE->FSTAT = FTFE_FSTAT_CCIF_MASK;

    // �ȴ��������
    while(!(FTFE->FSTAT & FTFE_FSTAT_CCIF_MASK));

    u32 temp;
    temp = FTFE->FSTAT;
    // �������־
    if(temp & ((1<<5) | (1<<4) | 1))
        return 1 ; //ִ���������

    return 0; //ִ������ɹ�
}
static s32 Kinetis_EraseSector(u16 sectorNo)
{
    union
    {
        u32  word;
        u8   byte[4];
    } dest;

    dest.word    = (u32)(sectorNo*(1<<11));

    FTFE->FCCOB0 = 0x09; //��������

    // ����Ŀ���ַ
    FTFE->FCCOB1 = dest.byte[2];
    FTFE->FCCOB2 = dest.byte[1];
    FTFE->FCCOB3 = dest.byte[0];

    // ִ����������
    if(1 == Kinetis_CmdLaunch())    //��ִ��������ִ���
        return 1;     //�����������

    // ������sector0ʱ��������豸
    if(dest.word <= 0x800)
    {

        FTFE->FCCOB0 = 0x06; // д��4�ֽ�
        // ����Ŀ���ַ
        FTFE->FCCOB1 = 0x00;
        FTFE->FCCOB2 = 0x04;
        FTFE->FCCOB3 = 0x0C;
        // ����
        FTFE->FCCOB4 = 0xFF;
        FTFE->FCCOB5 = 0xFF;
        FTFE->FCCOB6 = 0xFF;
        FTFE->FCCOB7 = 0xFE;
        // ִ����������
        if(1 == Kinetis_CmdLaunch())  //��ִ��������ִ���
            return 2;   //�����������
    }

    return 0;  //�ɹ�����
}

static s32 Kinetis_PageWrite(u16 sectNo,u8 *buf)
{
    u32 size;
    u32 offset;
    u8 *Data;

    union
    {
        u32   word;
        u8  byte[4];
    } dest;

    // ����д������
    FTFE->FCCOB0 = 0x07;
    size = sp_tFlashDesrc->BytesPerPage;
    offset = 0;
    Data = buf;
    while(size)
    {
        dest.word = (u32)(sectNo*(1<<11) + offset);//�����ַ
        // ����Ŀ���ַ
        FTFE->FCCOB1 = dest.byte[2];
        FTFE->FCCOB2 = dest.byte[1];
        FTFE->FCCOB3 = dest.byte[0];

        // ��������
        FTFE->FCCOB4 = Data[3];
        FTFE->FCCOB5 = Data[2];
        FTFE->FCCOB6 = Data[1];
        FTFE->FCCOB7 = Data[0];
        FTFE->FCCOB8 = Data[7];
        FTFE->FCCOB9 = Data[6];
        FTFE->FCCOBA = Data[5];
        FTFE->FCCOBB = Data[4];

        if(1 == Kinetis_CmdLaunch())
            break;  //д���������
        Cache_Invalidate();
        offset += 8;
        size   -= 8;
        Data   += 8;
    }

    return offset;  //�ɹ�ִ��
}

extern u32 gc_ptIbootSize;
extern u32 gc_ptFlashOffset;
extern u32 gc_ptFlashRange;
//-----------------------------------------------------------------------------
//����: ����FLASH��ʼ��
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static s32 Flash_Init(struct EmbdFlashDescr *Description)
{
    Kinetis_FlashInit();

    Description->BytesPerPage = 2048;
    Description->PagesPerSmallSect = 0;
    Description->PagesPerLargeSect = 0;
    Description->PagesPerNormalSect = 1;
    Description->SmallSectorsPerPlane = 0;
    Description->LargeSectorsPerPlane = 0;
    Description->NormalSectorsPerPlane = gc_ptFlashRange/2048;
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
    s32 Ret;

    Ret = Kinetis_EraseSector(SectorNo);

    if(Ret != 0)
        Ret = -1;
    return (Ret);
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
    u32 Ret;

    if(!Data)
        return (-1);

    Ret = Kinetis_PageWrite(Page,Data);
    return Ret;
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
//����: ����page����sector
//����: PageNo -- ҳ��
//     Remains -- ʣ��ҳ��
//     SectorNo -- ҳ����sector
//����:
//��ע: sector0��normal sector��Сʱһ���ģ��������ֿ�����ֻ��Ϊ�˱����Ķ����
//-----------------------------------------------------------------------------
s32 Flash_PageToSector(u32 PageNo, u32 *Remains, u32 *SectorNo)
{
    if((!Remains) || (!SectorNo))
        return (-1);

    *Remains  = 1;
    *SectorNo = PageNo;

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
            s64 *unit;

            left = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            if(*unit < sp_tFlashDesrc->NormalSectorsPerPlane)
            {
                *left = 0;          //ÿһ����ֻ��һҳ
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

            if(*unit <= sp_tFlashDesrc->NormalSectorsPerPlane)
            {
                *block = *unit;          //ÿһ����ֻ��һҳ
            }
            else
                res = -1;

            break;
        }

        case totalblocks:
        {
            *((u32*)args) =  sp_tFlashDesrc->NormalSectorsPerPlane;
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

            if(*block <= sp_tFlashDesrc->NormalSectorsPerPlane)
            {
                *units = 1;
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
                end = sp_tFlashDesrc->NormalSectorsPerPlane; // �����ĺţ�

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
        bend = sp_tFlashDesrc->NormalSectorsPerPlane; // �����
        BlockNum = bend - bstart;
    }
    else
    {
        BlockNum = bend - bstart;
    }
    super->AreaSize = BlockNum * sp_tFlashDesrc->BytesPerPage;
    super->MediaStart = bstart * sp_tFlashDesrc->PagesPerNormalSect; // ��ʼunit��

    res = strlen(EmflashName) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,EmflashName);   //��ȡ���豸��ȫ·��
    FsBeMedia(FullPath,fs); //�����豸�����ļ�ϵͳ
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);

}


