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
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================
#include "cpu_peri.h"
#include "lock.h"
#include "systime.h"
#include "stdlib.h"
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
//%$#@target = header    //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
//%$#@end configue  ****�������ý���
//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������
//@#$%component end configure

struct MutexLCB *flash_mutex = NULL;

void djy_flash_read(uint32_t address, void *data, uint32_t size)
{
    if (size == 0)
    {
        return;
    }
    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
    flash_protection_op(0,FLASH_PROTECT_NONE);
    flash_read(data, size, address);
    flash_protection_op(0,FLASH_PROTECT_ALL);
    Lock_MutexPost(flash_mutex);
}

void djy_flash_write(uint32_t address, const void *data, uint32_t size)
{
    if (size == 0)
    {
        return;
    }

    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
    flash_protection_op(0,FLASH_PROTECT_NONE);
    flash_write((char *)data, size, address);
    flash_protection_op(0,FLASH_PROTECT_ALL);
    Lock_MutexPost(&flash_mutex);
}

/*һ��sector��4K*/
void djy_flash_erase(uint32_t address)
{
    Lock_MutexPend(flash_mutex, CN_TIMEOUT_FOREVER);
    address &= (0xFFF000);
    flash_protection_op(0,FLASH_PROTECT_NONE);
    flash_ctrl(CMD_FLASH_ERASE_SECTOR, &address);
    flash_protection_op(0,FLASH_PROTECT_ALL);
    Lock_MutexPost(flash_mutex);
}

int ModuleInstall_Flash(void)
{
    flash_mutex = Lock_MutexCreate("flash_mutex");
    if(flash_mutex==NULL)
        return -1;
    else
        return 0;
}

///////////////////////////////////////////IAP FS/////////////////////////////////////////////
//#include <device/include/unit_media.h>
//
////flash ��Ϣ����
//static struct EmbdFlashDescr
//{
//    u16     BytesPerPage;                //һҳ�а������ֽ���
//    u16     PagesPerBlock;               //ÿ���е�ҳ��
//    u16     TotalPages;                  //��ҳ����
//    u32     BytesPerBlock;               //һ���е��ֽ���
//    u16     ToltalBlock;                 //�ܿ�����
//    u32     RemainBytes;                 //ʣ���ֽ���
//    u32     MappedStAddr;
//}*sp_tFlashDesrc;
//
//s32 __embed_req(enum ucmd cmd, ptu32_t args, ...)
//{
//    s32 res = 0;
//
//    switch(cmd)
//    {
//
//        case remain:
//        {
//            va_list list;
//            u32 *left;
//            s64 *unit;
//
//            left = (u32*)args;
//            va_start(list, args);
//            unit = (s64*)va_arg(list, u32);
//            va_end(list);
////            if((*unit<64)&&(*unit>=0))
////                *left = 63 - *unit;
////            else if((*unit<128)&&(*unit>=64))
////                *left = 127 - *unit;
////            else if((*unit<192)&&(*unit>=128))
////                *left = 191 - *unit;
////            else if((*unit<256)&&(*unit>=192))
////                *left = 255 - *unit;
////            else
////                res = -1;
//            if(*unit > sp_tFlashDesrc->TotalPages)
//                res = -1;
//            else
//                *left = sp_tFlashDesrc->PagesPerBlock - 1 - (*unit)%sp_tFlashDesrc->PagesPerBlock;
//
//            break;
//        }
//
//        case whichblock:                              //��һ��
//        {
//            va_list list;
//            s64  *unit;
//            u32 *block;
//
//            block = (u32*)args;
//            va_start(list, args);
//            unit = (s64*)va_arg(list, u32);
//            va_end(list);
//            *block = (*unit)/sp_tFlashDesrc->PagesPerBlock;
//            break;
//        }
//
//        case totalblocks:                             //�ܿ�����
//        {
//            // args = &blocks
//            *((u32*)args) =  sp_tFlashDesrc->ToltalBlock;
//            break;
//        }
//
//        case blockunits:                              //ÿ���е�ҳ����
//        {
//
//            *((u32*)args)  = sp_tFlashDesrc->PagesPerBlock;
//            break;
//        }
//
//        case unitbytes:                               //ÿһҳ�е��ֽ���
//        {
//            // args = &bytes
//            *((u32*)args) = sp_tFlashDesrc->BytesPerPage;
//            break;
//        }
//
//        case format:                                  //��ʽ
//        {
//            va_list list;
//            u32 start, end;
//            struct uesz *sz;
//
//            start = (u32)args;
//            va_start(list, args);
//            end = va_arg(list, u32);
//            sz = (struct uesz*)va_arg(list, u32);
//            va_end(list);
//
//            if(!sz->block)
//                return (-1);
//
////            tmp = malloc(sp_tFlashDesrc->BytesPerPage);
////            if(!tmp)
////                return (-1);
//
//            if(-1==end)                                //��������ȫ������
//                    end = sp_tFlashDesrc->ToltalBlock;
//            else if (start)
//                    end += start;
//
//            do
//            {
//                if(__embed_erase((s64)--end, *sz))
//                {
//                    res = -1;
//                    break;
//                }
//            }
//            while(end!=start);
//
//            break;
//        }
//
//        case mapaddr:
//        {
//
//            *((u32*)args) = sp_tFlashDesrc->MappedStAddr;
//            break;
//        }
//        case checkbad: break;
//        default: res = -1; break;
//    }
//
//    return (res);
//}
//
//// ============================================================================
//// ���ܣ�embed ����
//// ������unit -- ������ţ�ҳ����
////      data -- �������ݣ�
////      opt -- ���ķ�ʽ��
//// ��ע��
//// ============================================================================
//static s32 __embed_read(s64 unit, void *data, struct uopt opt)
//{
//    uint32_t start_addr = 0x80000 + 1024 * unit;
//    djy_flash_read(start_addr, data, 1024);
//    return 0;
//}
//
//// ============================================================================
//// ���ܣ�embed д��
//// ������unit -- д����ţ�ҳ����
////      data -- д�����ݣ�
////      opt -- д�ķ�ʽ��
//// ���أ��ɹ���0����ʧ�ܣ�-1����
//// ��ע��
//// ============================================================================
//static s32 __embed_write(s64 unit, void *data, struct uopt opt)
//{
//    if(sp_tFlashDesrc==NULL)
//        return -1;
//    if(unit>(sp_tFlashDesrc->TotalPages))
//        return -1;
//    uint32_t start_addr = 0x80000 + 1024 * unit;
//    djy_flash_write(start_addr, data, 1024);
//    return 0;
//}
//
//// ============================================================================
//// ���ܣ�nand ����
//// ������unit -- ��������ţ�
////      sz -- �����ĵ�λ��unit��block��
//// ���أ��ɹ���0����ʧ�ܣ�-1����
//// ��ע��
//// ============================================================================
//s32 __embed_erase(s64 unit, struct uesz sz)
//{
//    u32 block;
//    if(sp_tFlashDesrc==NULL)
//        return -1;
//    if(unit>(sp_tFlashDesrc->TotalPages))
//        return -1;
//    if(sz.unit)
//    {
//        if(__embed_req(whichblock, (ptu32_t)&block, &unit))
//            return (-1);
//    }
//    else
//        block = (u32)unit;
//
//    uint32_t start_addr = 0x80000 + 4096 * block;
//    djy_flash_erase(start_addr);
//    return 0;
//}
//
//static s32 Flash_Init(struct EmbdFlashDescr *Description)
//{
//
//      Description->BytesPerPage     = 1024;
//      Description->PagesPerBlock    = 4;
////      Description->PagesPerSector   = SECTOR_SIZE / PAGE_SIZE;
////      Description->SectorsPerBlock  = BLOCK_SIZE / SECTOR_SIZE;
//      Description->TotalPages       = 600;
//      Description->ToltalBlock      = 150;
//      Description->RemainBytes      = 0;
//
//      Description->MappedStAddr = 0;
//    return (0);
//}
//
//s32 __embed_part_init(u32 bstart, u32 bcount, u32 doformat)
//{
//    struct umedia *um;
//    struct uopt opt;
//    char name[16], part[3];
//    u32 units, total = 0;
//    static u8 count;
//
//    if(!sp_tFlashDesrc)
//    {
//        sp_tFlashDesrc = malloc(sizeof(*sp_tFlashDesrc));
//        if(!sp_tFlashDesrc)
//        {
//            return (-1);
//        }
//        Flash_Init(sp_tFlashDesrc);
////        djy_flash_init();
//    }
//
////    if(doformat)
////    {
////            djybsp_spiflash_step(DJYBSP_SPIFLASH_START);
////            djybsp_erase_some_sectors(0xc0000,SECTOR_SIZE);
////            djybsp_spiflash_step(DJYBSP_SPIFLASH_END);
////    }
//
//    um = malloc(sizeof(struct umedia)+sp_tFlashDesrc->BytesPerPage);
//    if(!um)
//    {
//        return (-1);
//    }
//
//    opt.hecc = 0;
//    opt.main = 1;
//    opt.necc = 1;
//    opt.secc = 0;
//    opt.spare = 1;
//    if(-1 == bcount)//������
//    {
//        bcount = sp_tFlashDesrc->ToltalBlock;
//        bcount -= bstart;
//        um->asz = sp_tFlashDesrc->BytesPerPage * sp_tFlashDesrc->PagesPerBlock * bcount;
//    }
//    else
//        um->asz = sp_tFlashDesrc->BytesPerPage * sp_tFlashDesrc->PagesPerBlock * bcount;
//
//
////    um->esz = log2(sp_tFlashDesrc->BytesPerPage * sp_tFlashDesrc->PagesPerBlock); //
////    um->usz = log2(sp_tFlashDesrc->BytesPerPage);
//    um->esz = 12;
//    um->usz = 10;
//    //um->esz = 0; // ��������ͬ
//    um->merase = __embed_erase;
//    um->mread = __embed_read;
//    um->mreq = __embed_req;
//    um->mwrite = __embed_write;
//    um->opt = opt;
//    um->type = nand;
//    um->ubuf = (u8*)um + sizeof(struct umedia);
//    um->ustart = bstart*sp_tFlashDesrc->PagesPerBlock; // ��ʼunit��
//    bcount = 0;
//    total = 0;
//
//    um->ustart = total; // ��ʼunit��
//    itoa(count++, part, 10);
//    sprintf(name, "embed part %s", part);
//    if(um_add((const char*)name, um))
//    {
//        printf("\r\n: erro : device : %s addition failed.", name);
//        return (-1);
//    }
//
//    printf("\r\n: info : device : %s added(start:%d, blocks:%d).", name, bstart, bcount);
//    return (0);
//
//}

