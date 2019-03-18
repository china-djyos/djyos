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
#ifdef CFG_CORTEX_M0
/*#include "norflash.h"*/
#include "silan_m0_cache.h"
#include "silan_irq.h"
#include "int.h"
#include "string.h"
#include <stdlib.h>
#include "flash.h"
#include "dbug.h"
#include <device/include/unit_media.h>
#include "filesystems.h"

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    s32 ModuleInstall_EmbededFlash(const char *TargetFs,s32 bstart, s32 bend, u32 doformat);
//    ModuleInstall_EmbededFlash(CFG_EFLASH_FSMOUNT_NAME,CFG_EFLASH_PART_START, CFG_EFLASH_PART_END, CFG_EFLASH_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_emflash"     //Ƭ��flash��д
//parent:"xip"                          //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                         //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                      //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                       //��ʼ��ʱ������ѡֵ��early��medium��later��
                                        //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"xip","devfile","lock" //������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г�
//weakdependence:"none"                 //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                          //������������������������none����ʾ�������������
                                        //����������������������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_EFLASH_FSMOUNT_NAME   //****�������Ƿ��Ѿ����ú�
#warning    embeded_flash�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,-1,1024,
#define CFG_EFLASH_PART_START      6          //������ʼ
#define CFG_EFLASH_PART_END        -1         //��������
//%$#@enum,true,false,
#define CFG_EFLASH_PART_FORMAT     false      //����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
//%$#@string,1,32,
//%$#@enum,EN_XIP_APP_TARGET,EN_XIP_IBOOT_TARGET,NULL
#define CFG_EFLASH_FSMOUNT_NAME   EN_XIP_APP_TARGET    //�谲װ���ļ�ϵͳ��mount�����֣�NULL��ʾ��flash�������ļ�ϵͳ
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
// ============================================================================


//Ϊ�˵��� ���㣬������ Debug �汾Ҳ���ÿ���ͨ���ն�����

#define CN_APP_DEBUG  (true)

//��ǰ��·״̬

typedef enum EN_LINK_STATUS{
    EN_DOWN_APP_DEBUG_MODE = 0x00,  //����APP debugģʽ
    EN_DOWN_APP_RELEASE_MODE,       //release ģʽ
    EN_DOWN_RISC_MODE,              //����CKģʽ
    EN_DOWN_DSP_MODE,               //����DSPģʽ

}EN_LinkStatus;

static EN_LinkStatus en_gStaus = EN_DOWN_APP_DEBUG_MODE;

enum EN_SPI_OPT{
    EN_OPT_START = 0,
    EN_OPT_END,
};


/*����app ���Ե�129���Ӧ-��0x40800
 *Risc�ļ��ص�ַΪ��0x100000
 *��������Risc,��ôд��ַӦ��Ӧ0x100000��g_Map_Add_Start = 0x100000 - 0x40800
 *��������Dsp 0x100000 + 0x40000 ΪDsp �ļ��ص�ַ����ʱ��
 *��������Dsp g_Map_Add_Start������0x100000 + 0x80000������0x40800
 * */

//Ĭ�� ������app
static volatile u32 g_Map_Add_Start = 0;

static struct umedia *emflash_um;
extern int32_t EraseSomeBlocks(uint32_t addr, uint32_t size);
extern int32_t ProgramOnePackage(char *data, uint32_t addr, uint32_t size);

/*Ϊʹ�ֿ�ʱΪ2^n Iboot Code ��ʼ��ŵ�ַΪ0x800 = 2048,iboot ��СΪ256k
 *��ôapp ��ʼ��ŵ�ַΪ0x800 + 256k = 40000 + 0x800 = 0x40800 = 264192
 *Ϊ1024��������-���㻮�ֿ顣��һ���С����6k,һҳ��С����Ϊ512�ֽڡ���غ궨�����£�
 *��ô��app�����ַΪ:0x40800 / (2*1024) = 129 �顣��ʼ
*/

#define FLASH_PAGE_SIZE        (0x100)       /* 256 byte */  //256   bytes
//#define SECTOR_SIZE          (0x1000)      /* 4 Kbyte */  //         bytes
#define FLASH_BLOCK_SIZE       (0x800)       /* 2K*/   //         bytes
#define S_FLASH_SIZE           (0x400000)    /* 2 MBytes */



#define CN_APP_START_ADDR      (0x00000000)    //IAP�ļ�ϵͳ������ʼ��ַ
#define CN_APP_END_ADDR        (0x200000)     /*2M �ռ�*/
#define CN_FLASH_RANGE         (CN_APP_END_ADDR - CN_APP_START_ADDR)

//д֮ǰ��������
#define CN_REASE_RISC_START (0x100000)
#define CN_REASE_RISC_LEN   (0x80000)

#define CN_REASE_APP_START  (0x40800)
#define CN_REASE_APP_LEN    (1024*256)////����256bytes �ļ�ͷ

#define CN_REASE_DSP_START  (0x100000 + CN_REASE_RISC_LEN)
#define CN_REASE_DSP_LEN    (0x100000 + 0x80000)

static const char *EmflashName = "emflash";      //��flash��obj�ڵ�����
extern struct obj *s_ptDeviceRoot;
extern struct __xip_drv XIP_EMFLASH_DRV;

//flash ��Ϣ����
static struct EmbdFlashDescr
{
    u16     BytesPerPage;                //һҳ�а������ֽ���
    u32     PagesPerBlock;               //ÿ���е�ҳ��
    u32     TotalPages;                  //��ҳ����
    u32     BytesPerBlock;               //һ���е��ֽ���
    u16     ToltalBlock;				 //�ܿ�����
    u32     RemainBytes;                 //ʣ���ֽ���
	u32     MappedStAddr;
}*sp_tFlashDesrc;


extern u32 gc_ptIbootSize;
extern u32 gc_ptFlashOffset;
extern u32 gc_ptFlashRange;


extern u32 Lock_Cache_Add_Start;
extern u32 Lock_Cache_Add_End;

static struct EmbdFlashDescr *sp_tFlashDesrc;


extern u32 gc_ptIbootSize;

// ============================================================================
// ���ܣ�ι��
// ������
// ���أ�
// ��ע��
// ============================================================================
extern bool_t BrdWdt_FeedDog(void) __attribute__((weak));
bool_t BrdWdt_FeedDog(void)
{
    return (TRUE); // �պ���
}

// ============================================================================
// ���ܣ�����FLASH�ĳ�ʼ��
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 Flash_Init(struct EmbdFlashDescr *Description)
{

      Description->BytesPerPage     = FLASH_PAGE_SIZE;                    //һҳ�а������ֽ���
      Description->PagesPerBlock    = FLASH_BLOCK_SIZE / FLASH_PAGE_SIZE;       //12Pages
//      Description->PagesPerSector   = SECTOR_SIZE / FLASH_PAGE_SIZE;
//      Description->SectorsPerBlock  = FLASH_BLOCK_SIZE / SECTOR_SIZE;
      Description->ToltalBlock      = CN_FLASH_RANGE / FLASH_BLOCK_SIZE;  //2097152/2048=1024
      Description->TotalPages       = (Description->PagesPerBlock)*(Description->ToltalBlock);    //2097152/512=4096
      Description->RemainBytes      = CN_FLASH_RANGE % FLASH_PAGE_SIZE;   //898Page
      
      Description->MappedStAddr = 0x00000000;
    return (0);
}


// ============================================================================
// ���ܣ���ȡ����FLASH����Ϣ
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 Flash_GetDescr(struct EmFlashDescr *Description)
{
    Description->BytesPerPage = sp_tFlashDesrc->BytesPerPage;
    Description->TotalPages   = sp_tFlashDesrc->TotalPages;
                                
    Description->MappedStAddr = sp_tFlashDesrc->MappedStAddr;
    
    return (0);
}


// ============================================================================
// ���ܣ�����flash ����ģʽ(����ģʽ����д������)
// ������
// ���أ�
// ��ע��
// ============================================================================
extern void silan_m0_cache_enable(void);
extern void silan_m0_cache_disable(void);
extern void silan_m0_cache_clear(void);
extern void silan_m0_cache_lock(uint32_t lock_addr, uint32_t len);
extern void silan_m0_cache_unlock(uint32_t lock_addr, uint32_t len, uint8_t unvalid);
extern int32_t iap_norflash_init();

static void SpiFlashOptMode(u8 Mode)
{
     u32 optStartAddr;
     u32 Remain;
     u32 optAddrLen;

    switch(Mode)
    {
        case EN_OPT_START:
             silan_m0_cache_disable();
             silan_m0_cache_clear();
             optStartAddr =    ((u32) &Lock_Cache_Add_Start) & 0xFFFFFFF0;
             Remain       =    ((u32) &Lock_Cache_Add_Start) & 0x0000000F;
             optAddrLen   =    ((((u32) &Lock_Cache_Add_End) - ((u32) &Lock_Cache_Add_Start)) + Remain + M0_CACHE_LINE_SIZE - 1) / M0_CACHE_LINE_SIZE;
             silan_m0_cache_lock(optStartAddr, optAddrLen);
             silan_m0_cache_enable();
             iap_norflash_init();
             break;
        case EN_OPT_END:
             optStartAddr =    ((u32) &Lock_Cache_Add_Start) & 0xFFFFFFF0;
             Remain       =    ((u32) &Lock_Cache_Add_Start) & 0x0000000F;
             optAddrLen   =    ((((u32) &Lock_Cache_Add_End) - ((u32) &Lock_Cache_Add_Start)) + Remain + M0_CACHE_LINE_SIZE - 1) / M0_CACHE_LINE_SIZE;
             silan_m0_cache_unlock(optStartAddr, optAddrLen, 0);
             break;
    }
}



// ============================================================================
// ���ܣ������
// ������SectorNo -- ������;
// ���أ�"0" -- �ɹ�;"-1" -- ʧ��;
// ��ע��
// ============================================================================
static s32 Flash_SectorEarse(u32 SectorNo)
{
    u32 Addr;
    u8 retry = 0; // �����п��ܻ�ʧ�ܣ�
    s32 Ret = 0;
    u32 SECTORError=0;//�������������Ϣ
    atom_high_t high_atom;
    high_atom =Int_HighAtomStart();
    SpiFlashOptMode(EN_OPT_START);

    Addr = SectorNo * FLASH_BLOCK_SIZE + sp_tFlashDesrc->MappedStAddr + g_Map_Add_Start;
    EraseSomeSectors(Addr,FLASH_BLOCK_SIZE);

    SpiFlashOptMode(EN_OPT_END);
    Int_HighAtomEnd(high_atom);
    return Ret;
}


// ============================================================================
// ���ܣ�дĳҳ
// ������PageNo -- ҳ��;
//       Data -- д����;
//       Flags -- д��������λ;
// ���أ�">0" -- ������д����ֽ���;
//       "-1" -- ��������;
//       "-2" -- дʧ��;
// ��ע��
// ============================================================================
static s32 Flash_PageProgram(u32 Page, u8 *Data, u32 Flags)
{
    u32 datLen;
    u32 DatAddr;
    atom_high_t high_atom;
    if(en_gStaus == EN_DOWN_APP_DEBUG_MODE || \
       en_gStaus == EN_DOWN_RISC_MODE || \
       en_gStaus == EN_DOWN_DSP_MODE)//App ����
    {

        DatAddr = (Page-1) * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr + g_Map_Add_Start;

    }else//app rease
    {
        DatAddr = Page * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr + g_Map_Add_Start;
    }

    u32 DatToWrite = 0;
    high_atom =Int_HighAtomStart();
    SpiFlashOptMode(EN_OPT_START);

    datLen       = sp_tFlashDesrc->BytesPerPage;
    //ProgramOnePackage((u8*)Data, DatAddr, datLen);
    ProgramOnePackage(Data,DatAddr,datLen);

    SpiFlashOptMode(EN_OPT_END);
    Int_HighAtomEnd(high_atom);
    return sp_tFlashDesrc->BytesPerPage;
}


// ============================================================================
// ���ܣ���ĳҳ
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 Flash_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    u8  *ptReadAddr;
    u32 readDatNo;
    u32 loop;    
    
    if(en_gStaus == EN_DOWN_APP_DEBUG_MODE || \
           en_gStaus == EN_DOWN_RISC_MODE || \
           en_gStaus == EN_DOWN_DSP_MODE)//App ����
    {
        ptReadAddr   = (u8*)((PageNo - 1) * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr + g_Map_Add_Start);
    }else
    {
        ptReadAddr   = (u8*)(PageNo * sp_tFlashDesrc->BytesPerPage + sp_tFlashDesrc->MappedStAddr + g_Map_Add_Start);
    }

    readDatNo    = sp_tFlashDesrc->BytesPerPage;

    if(Data)
    {
        for(loop = 0;loop < readDatNo;loop++)
        {
            Data[loop] = ptReadAddr[loop];
        }

    }

    return (sp_tFlashDesrc->BytesPerPage);
}

// ============================================================================
// ���ܣ�����page����sector
// ������PageNo -- ҳ��
//       Remains -- ʣ��ҳ��
//       SectorNo -- ҳ����sector
// ���أ�
// ��ע�� sector0��normal sector��Сʱһ���ģ��������ֿ�����ֻ��Ϊ�˱����Ķ����
// ============================================================================
s32 Flash_PageToSector(u32 PageNo, u32 *Remains, u32 *SectorNo)
{
    s32 ret;
    u16 secNum;
    u16 remainPage;

    //��ҳ���ڵڼ�����
//    secNum      = PageNo * sp_tFlashDesrc->BytesPerPage / SECTOR_SIZE;
//    remainPage  = PageNo * sp_tFlashDesrc->BytesPerPage % SECTOR_SIZE /FLASH_PAGE_SIZE;
//    SectorNo[0] = secNum;
//    Remains [0] = remainPage;

    return secNum;
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
  
        case whichblock:   //�ҳ���ҳ���ڿ�,�ֻ���һ��=4Pages
        {
            va_list list;
            s64 *unit;
            u32 *block;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            if(*unit <= sp_tFlashDesrc->TotalPages)     //�ж��Ƿ���ҳ��
            {
                *block = *unit / sp_tFlashDesrc->PagesPerBlock;
            }
            else
            {
                res = -1;
            }

//            if((*unit<12)&&(*unit>=0))//one blk = 12 Pages
//               *block = 0;
           break;
        }

        case totalblocks:                                //�ܿ�����
        {
            // args = &blocks
            *((u32*)args) =  sp_tFlashDesrc->ToltalBlock;
            break;
        }

        case blockunits:                              //ÿ���е�ҳ����
        {
          
            *((u32*)args)  = sp_tFlashDesrc->PagesPerBlock;
            break;
        }

        case unitbytes:                               //ÿһҳ�е��ֽ���
        {
            // args = &bytes
            *((u32*)args) = sp_tFlashDesrc->BytesPerPage;
            break;
        }

        case format:                                  //��ʽ
        {
            va_list list;
            s32 start,end;
            struct uesz *sz;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(!sz->block)
               return (-1);

//            tmp = malloc(sp_tFlashDesrc->BytesPerPage);
//            if(!tmp)
//                return (-1);
//
            if(-1==end)
                end = sp_tFlashDesrc->ToltalBlock; // �����ĺţ�

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
        case remain :          //��ҳ���ڵĿ��л�ʣ�����ҳ,ҳ�������ε����ӵ�
        {
            va_list list;
            u32 *left;
            s64 *unit;
            u16 blkNum;

            left = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);

            if(*unit < sp_tFlashDesrc->TotalPages)
            {
                //�����ҳ���ڵĿ�
                blkNum = *unit / sp_tFlashDesrc->PagesPerBlock;
                *left = (blkNum + 1) * (sp_tFlashDesrc->PagesPerBlock) - *unit;
            }
            else
                res = -1;

             break;
        }
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
// ������fs -- ��Ҫ���ص��ļ�ϵͳ��MountPart -- ���ص���ý��ĵڼ���������������0��ʼ��
//       bstart -- ��ʼ�飬bend -- �����飨�������ÿ飬ֻ���ÿ����һ�飩
// ���أ�0 -- �ɹ��� -1 -- ʧ��
// ��ע��
// ============================================================================
s32 __embed_FsInstallInit(const char *fs, s32 bstart, s32 bend)
{
    char *FullPath,*notfind;
    struct obj *targetobj;
    struct FsCore *super;
    s32 res;
    s32 BlockNum;

    targetobj = obj_matchpath(fs, &notfind);
    if(notfind)
    {
        error_printf("spiflash"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)obj_GetPrivate(targetobj);
    super->MediaInfo = emflash_um;
    //�����"XIP-APP"��"XIP-IBOOT"Ϊ�ļ�ϵͳ��������
    if((strcmp(super->pFsType->pType, "XIP-APP") == 0) || (strcmp(super->pFsType->pType, "XIP-IBOOT") == 0))
    {
        super->MediaDrv = &XIP_EMFLASH_DRV;
    }
    else
    {
        super->MediaDrv = 0;
        error_printf("embed"," \"%s\" file system type nonsupport", super->pFsType->pType);
        return -1;
    }

    if(bend == -1)
    {
        bend = sp_tFlashDesrc->ToltalBlock;
        BlockNum = bend - bstart;
    }
    else
    {
        BlockNum = bend - bstart;
    }
    super->AreaSize = BlockNum * sp_tFlashDesrc->BytesPerBlock;
    super->MediaStart = bstart * sp_tFlashDesrc->PagesPerBlock; // ��ʼunit��

    res = strlen(EmflashName)+strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,EmflashName);      //��ȡ�豸��ȫ·��
    FsBeMedia(FullPath,fs);     //�����豸�����ļ�ϵͳ
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);

}

 bool_t downapp(char *Param);
 bool_t downrisc(char *Param);
 bool_t downdsp(char *Param);

#include "ymodem.h"
#include "shell.h"

//������������������ʽ������Ϊ4K������������������ģ��ʱӦע��������ģ��ǰ��ǰ4Kû������
//��Ȼ��Ѷ�Ӧ�����ݲ���
void PrepareForDownLoad(u32 startAddr,u32 len)
{
    u32 BytesPage;
    atom_high_t high_atom;
    BytesPage = sp_tFlashDesrc->BytesPerPage;
    //����ǰ�Ȳ���
    high_atom =Int_HighAtomStart();
    SpiFlashOptMode(EN_OPT_START);
    //����Ӧ������ȥǰ256�ֽڵ��ļ�ͷ
    EraseSomeSectors(startAddr - BytesPage,len+BytesPage);
    SpiFlashOptMode(EN_OPT_END);
    Int_HighAtomEnd(high_atom);
}

//ADD_TO_IN_SHELL_HELP(downapp,"����app    �����ʽ: downapp");
bool_t downapp(char *Param)
{

    g_Map_Add_Start = 0;
    PrepareForDownLoad(CN_REASE_APP_START,CN_REASE_APP_LEN);
    if(CN_APP_DEBUG)
    {
        en_gStaus = EN_DOWN_APP_DEBUG_MODE;
    }else
        en_gStaus = EN_DOWN_APP_RELEASE_MODE;

    downloadym(NULL);
}

//ADD_TO_IN_SHELL_HELP(downrisc,"����risc    �����ʽ: downrisc");
bool_t downrisc(char *Param)
{
    u32 BytesPage;
    g_Map_Add_Start = 0x100000 - 0x40800;

    PrepareForDownLoad(CN_REASE_RISC_START,CN_REASE_RISC_LEN);
    en_gStaus = EN_DOWN_RISC_MODE;
    downloadym(NULL);
}

//ADD_TO_IN_SHELL_HELP(downdsp,"����dsp    �����ʽ: downdsp");
bool_t downdsp(char *Param)
{
    g_Map_Add_Start = 0x100000 + 0x80000 - 0x40800;
    //����ǰ�Ȳ���
    PrepareForDownLoad(CN_REASE_DSP_START,CN_REASE_DSP_LEN);
    en_gStaus = EN_DOWN_DSP_MODE;
    downloadym(NULL);
}

bool_t Module_Install_Update()
{
    return true;
}

//-----------------------------------------------------------------------------
// ���ܣ���װƬ��Flash����
// ������TargetFs -- Ҫ���ص��ļ�ϵͳ
//      �������� -- ��ʼ�飬�����飨�����������6����ʼ����0����÷���ʹ�õĿ�Ϊ0��1��2��3��4��5�飬�����������飩���Ƿ��ʽ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע���������֪��Ҫ��װʲô�ļ�ϵͳ�����߲���װ�ļ�ϵͳTargetFs��NULL��TargetPart��-1��
//-----------------------------------------------------------------------------
s32 ModuleInstall_EmbededFlash(const char *TargetFs,s32 bstart, s32 bend, u32 doformat)
{
    static u8 emflashinit = 0;
    u32 units;

    if(!sp_tFlashDesrc)
    {
        sp_tFlashDesrc = malloc(sizeof(*sp_tFlashDesrc));
        if(!sp_tFlashDesrc)
        {
            return (-1);
        }

        Flash_Init(sp_tFlashDesrc);
    }

    if(doformat)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __embed_req(format, (ptu32_t)bstart , bend, &sz);       //��ʽ��ָ������
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

    if(TargetFs != NULL)
    {
        if(__embed_FsInstallInit(TargetFs, bstart, bend))
        {
            return -1;
        }
    }

    return 0;
}

ADD_TO_ROUTINE_SHELL(downapp,downapp,"����app    �����ʽ: downapp");
ADD_TO_ROUTINE_SHELL(downrisc,downrisc,"����risc    �����ʽ: downrisc");
ADD_TO_ROUTINE_SHELL(downdsp,downdsp,"����dsp    �����ʽ: downdsp");

#endif

