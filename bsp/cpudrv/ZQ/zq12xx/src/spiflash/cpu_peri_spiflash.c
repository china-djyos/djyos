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
#ifdef CFG_CPU_ZQ12XX_CK
#include "cpu_peri.h"
#include "int.h"
#include <string.h>
#include <stdlib.h>
#include <device.h>
#include <device/djy_flash.h>
#include <cpu_peri.h>
#include <djyos.h>
#include <math.h>
#include "stm32f7xx_hal_conf.h"
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <device/unit_media.h>
#include <board.h>
#include <libc/misc/ecc/ecc_256.h>

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//s32 ModuleInstall_SpiFlash(u32 doformat);
//ModuleInstall_SpiFlash(CFG_SPIFLASH_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip spi"//Ƭ��flash
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
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_SPI == false )
//#warning  " cpu_onchip_spi  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_SPI    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,true,false,
#define CFG_SPIFLASH_PART_FORMAT     false      //����ѡ��,�Ƿ���Ҫ������оƬ��
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

bootspi_t iap_bootspi;
extern struct Object *s_ptDeviceRoot;
static const char *SpiFlashName = "SpiFlash";      //��flash��obj�ڵ�����
struct umedia *sipflash_umedia;
static bool_t sSpiflashInited = false;
static void djybsp_spi_init(uint8_t spiclk_sel,uint8_t if_preread,uint8_t if_rcv_cpol)
{
    spi_st->spi_sub_config &= ~SPI_CLK_DIV(0x7);
    spi_st->spi_sub_config |= SPI_CLK_DIV(spiclk_sel);

    if(if_preread)
        spi_st->spi_sub_config &= ~NO_PROREADING;
    else
        spi_st->spi_sub_config |= NO_PROREADING;
    if(if_rcv_cpol)
        spi_st->spi_sub_config |= SPI_RCV_CPOL;
    else
        spi_st->spi_sub_config &= ~SPI_RCV_CPOL;

    spi_st->spi_sub_config |= SPI_SET_CFG;
    spi_st->key = KEY1;
    spi_st->main_ctrl |= HOST_NOT_CACHE;
}

static void djybsp_spiflash_uninit(void)
{
    spi_st->spi_sub_config &= ~NO_PROREADING;
    spi_st->spi_sub_config &= ~SPI_RCV_CPOL;
    spi_st->spi_sub_config |= SPI_SET_CFG;
    spi_st->main_ctrl &= ~HOST_NOT_CACHE;
}

static uint32_t djybsp_spiflash_read_status(void)
{
    unsigned int rdata = 0;
    spi_st->main_ctrl |= HOST_NOT_CACHE;
    spi_st->spi_cmd = SPI_OP_CMD(0x05);
    spi_st->spi_subctrl &=~HOST_DATA_NUM(3);
    spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
    spi_st->key = KEY2;
    spi_st->spi_ctrl = HOST_RREQ;
    spi_st->spi_subctrl |= HOST_RREADY;
    while ((spi_st->spi_subctrl & HOST_RVALID) == 0);
    while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
    rdata = spi_st->spi_rdata;
    spi_st->key = KEY2;
    spi_st->spi_ctrl &= ~HOST_RREQ;
    spi_st->main_ctrl &= ~HOST_NOT_CACHE;
    return rdata;
}

static uint32_t djybsp_spiflash_read_capid(void)
{
    unsigned int rdata = 0;
    uint32_t i;

    spi_st->main_ctrl |= HOST_NOT_CACHE;
    spi_st->spi_cmd = SPI_OP_CMD(0x9f);
    spi_st->spi_subctrl &=~HOST_DATA_NUM(3);
    spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
    spi_st->key = KEY2;
    spi_st->spi_ctrl = HOST_RREQ;

    for(i=0; i<3; i++) {
        spi_st->spi_subctrl |= HOST_RREADY;
        while ((spi_st->spi_subctrl & HOST_RVALID) == 0);
        while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
        rdata = spi_st->spi_rdata;
    }

    spi_st->key = KEY2;
    spi_st->spi_ctrl &= ~HOST_RREQ;
    spi_st->main_ctrl &= ~HOST_NOT_CACHE;
    return rdata;
}

static int djybsp_spiflash_done(void)
{
    uint32_t flash_status =0;
    uint32_t retry = 100000;
    flash_status = djybsp_spiflash_read_status();
    while(retry){
        if((flash_status & 0x1)  == 0){
            return 0;
        }
        flash_status = djybsp_spiflash_read_status();
        retry --;
    }
    return -EAGAIN;
}

static int djybsp_spiflash_write_done(void)
{
    uint32_t flash_status =0;
    uint32_t retry = 10000;
    flash_status = djybsp_spiflash_read_status();
    while(retry){
        if((flash_status & 0x2)  == 0x2){
                 return 0;
        }
        flash_status = djybsp_spiflash_read_status();
        retry --;
    }
    return -EAGAIN;
}

static int djybsp_spiflash_write_enable(void)
{
    spi_st->main_ctrl |= HOST_NOT_CACHE;
    spi_st->spi_cmd = SPI_OP_CMD(6);
    spi_st->spi_subctrl &=~HOST_DATA_NUM(3);
    spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
    spi_st->key = KEY2;
    spi_st->spi_ctrl = HOST_WREQ;
    while ((spi_st->spi_subctrl & HOST_WREADY) == 0);
    while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
    spi_st->key = KEY2;
    spi_st->spi_ctrl &= ~HOST_WREQ;
    spi_st->main_ctrl &= ~HOST_NOT_CACHE;
    return (djybsp_spiflash_write_done());
}

int32_t djybsp_spiflash_pagewrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
    int g_result;
    unsigned int i;

    djybsp_spi_init(iap_bootspi.clk, iap_bootspi.cfg&BOOTSPI_CFG_PROREAD, iap_bootspi.cfg&BOOTSPI_CFG_RCV_CPOL);
    djybsp_spiflash_write_enable();
    g_result = djybsp_spiflash_done();
    if(g_result)
    {
        djybsp_spiflash_uninit();
        return g_result;
    }

    spi_st->main_ctrl |= HOST_NOT_CACHE;
    spi_st->spi_cmd = SPI_OP_CMD(0x02) ;
    spi_st->spi_addr =  SPI_OP_ADDR(WriteAddr);
    spi_st->spi_subctrl |= HOST_DATA_NUM(3);
    spi_st->key = KEY2;

    spi_st->spi_ctrl &= ~FAST_READ_WRITE;
    spi_st->spi_ctrl &= ~HOST_RREQ;
    spi_st->spi_ctrl |= HOST_WREQ;
    while ((spi_st->spi_subctrl & HOST_WREADY) == 0);
    while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);

    spi_st->spi_subctrl &= ~HOST_DATA_NUM(3);
    spi_st->spi_subctrl |= HOST_DATA_NUM(0);

    for (i=0; i<NumByteToWrite; i++){
        spi_st->spi_wdata = pBuffer[i];
        spi_st->spi_subctrl |= HOST_WVALID;
        while ((spi_st->spi_subctrl & HOST_WREADY) == 0);
        while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
    }

    while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
    spi_st->key = KEY2;
    spi_st->spi_ctrl &= ~HOST_WREQ;
    spi_st->main_ctrl &= ~HOST_NOT_CACHE;
    g_result = djybsp_spiflash_done();
    djybsp_spiflash_uninit();
    if(g_result)
        return g_result;

    return 0;
}

int32_t djybsp_spiflash_erase(uint32_t Addr, uint8_t cmd)
{
    int g_result;

    djybsp_spi_init(iap_bootspi.clk, iap_bootspi.cfg&BOOTSPI_CFG_PROREAD, iap_bootspi.cfg&BOOTSPI_CFG_RCV_CPOL);
    djybsp_spiflash_write_enable();
    g_result = djybsp_spiflash_done();
    if(g_result){
        djybsp_spiflash_uninit();
        return g_result;
    }

    spi_st->main_ctrl |= HOST_NOT_CACHE;
    spi_st->spi_cmd = SPI_OP_CMD(cmd);
    spi_st->spi_addr = SPI_OP_ADDR(Addr);
    spi_st->spi_subctrl |= HOST_DATA_NUM(3);

    spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
    spi_st->key = KEY2;
    spi_st->spi_ctrl = HOST_WREQ;
    while ((spi_st->spi_subctrl & HOST_WREADY) == 0);
    while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
    spi_st->key = KEY2;
    spi_st->spi_ctrl &= ~HOST_WREQ;
    spi_st->main_ctrl &= ~HOST_NOT_CACHE;

    g_result = djybsp_spiflash_done();
    djybsp_spiflash_uninit();
    if(g_result)
        return g_result;

    return 0;
}

int32_t djybsp_erase_some_sectors(uint32_t addr, uint32_t size)
{
    uint32_t sector, start, remain;
    uint32_t i;
    start  = addr & 0x00FFF000;
    remain = addr - start;
    sector = (size + remain + SECTOR_SIZE - 1) / SECTOR_SIZE;
    for(i=0; i<sector; i++)
        if(djybsp_spiflash_erase(i*SECTOR_SIZE + start, 0x20))
            return 0;
    return 1;
}

int32_t djybsp_erase_some_blocks(uint32_t addr, uint32_t size)
{
    uint32_t sector, start, remain;
    uint32_t i;
    start  = addr & 0x00FF0000;
    remain = addr - start;
    sector = (size + remain + BLOCK_SIZE - 1)/ BLOCK_SIZE;
    for(i=0; i<sector; i++)
        if(djybsp_spiflash_erase(i*BLOCK_SIZE + start, 0xD8))
            return 0;
    return 1;
}

int32_t djybsp_program_one_package(char *data, uint32_t addr, uint32_t size)
{
    uint32_t page, remain;
    uint32_t i;
    char *ptr;
    uint32_t faddr;

    page = size / PAGE_SIZE;
    remain = size - page*PAGE_SIZE;
    ptr = data;
    faddr = addr;

    if(page == 0) {
        djybsp_spiflash_pagewrite(ptr, faddr, remain);
    }
    else {
        for(i=0; i<page; i++) {
            djybsp_spiflash_pagewrite(ptr, faddr, PAGE_SIZE);
            ptr += PAGE_SIZE;
            faddr += PAGE_SIZE;
        }
        if(remain != 0)
            djybsp_spiflash_pagewrite(ptr, faddr, remain);
    }
}

void djybsp_spiflash_step(uint8_t step)
{
    switch(step)
    {
        case 0:
            csi_icache_disable();
            csi_dcache_disable();
            csi_dcache_clean();
            //djybsp_spiflash_init();
            break;
        case 1:
            csi_dcache_enable();
            csi_icache_enable();
            break;
    }
}

uint32_t djybsp_get_flash_capacity(void)
{
    uint32_t cap;
    cap = (1 << (djybsp_spiflash_read_capid() & 0xff));
    return cap;
}

int32_t djybsp_spiflash_init(void)
{
    iap_bootspi.cfg = 0;
    iap_bootspi.clk = BOOTSPI_CLK_DIV4;
    return 0;
}

/////////////////////////////////////////IAP FS/////////////////////////////////////////////
#include <device/unit_media.h>


//flash ��Ϣ����
static struct EmbdFlashDescr
{
    u16     BytesPerPage;                //һҳ�а������ֽ���
    u16     PagesPerBlock;               //ÿ���е�ҳ��
    u16     TotalPages;                  //��ҳ����
    u32     BytesPerBlock;               //һ���е��ֽ���
    u16     ToltalBlock;                 //�ܿ�����
    u32     RemainBytes;                 //ʣ���ֽ���
    u32     MappedStAddr;
}*sp_tFlashDesrc;

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
//            if((*unit<64)&&(*unit>=0))
//                *left = 63 - *unit;
//            else if((*unit<128)&&(*unit>=64))
//                *left = 127 - *unit;
//            else if((*unit<192)&&(*unit>=128))
//                *left = 191 - *unit;
//            else if((*unit<256)&&(*unit>=192))
//                *left = 255 - *unit;
//            else
//                res = -1;
            if(*unit > sp_tFlashDesrc->TotalPages)
                res = -1;
            else
                *left = sp_tFlashDesrc->PagesPerBlock - 1 - (*unit)%sp_tFlashDesrc->PagesPerBlock;

            break;
        }

        case whichblock:                              //��һ��
        {
            va_list list;
            s64  *unit;
            u32 *block;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);
            *block = (*unit)/sp_tFlashDesrc->PagesPerBlock;
            break;
        }

        case totalblocks:                             //�ܿ�����
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
            s32 start, end;
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

            if(-1==end)                                //��������ȫ������
                    end = sp_tFlashDesrc->ToltalBlock;

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
static s32 __embed_read(s64 unit, void *data, struct uopt opt)
{
    uint32_t start_addr = 0xc0000 + 1024 * unit;
    uint8_t *buf_temp = (uint8_t*)data;
    csi_dcache_clean();
    for(int i=0;i<1024;i++)
        buf_temp[i] = *(uint8_t*)(start_addr + i);
    return 0;
}

// ============================================================================
// ���ܣ�embed д��
// ������unit -- д����ţ�ҳ����
//      data -- д�����ݣ�
//      opt -- д�ķ�ʽ��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 __embed_write(s64 unit, void *data, struct uopt opt)
{
    if(sp_tFlashDesrc==NULL)
        return -1;
    if(unit>(sp_tFlashDesrc->TotalPages))
        return -1;
    atom_low_t atom_low = 0;
    uint32_t start_addr = 0xc0000 + 1024 * unit;
    uint8_t *buf_temp = (uint8_t*)data;
    atom_low = Int_LowAtomStart();
    djybsp_spiflash_step(DJYBSP_SPIFLASH_START);
    djybsp_program_one_package(buf_temp,start_addr,1024);
    djybsp_spiflash_step(DJYBSP_SPIFLASH_END);
    Int_LowAtomEnd(atom_low);
    return 0;
}

// ============================================================================
// ���ܣ�nand ����
// ������unit -- ��������ţ�
//      sz -- �����ĵ�λ��unit��block��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 __embed_erase(s64 unit, struct uesz sz)
{
    u32 block;
    if(sp_tFlashDesrc==NULL)
        return -1;
    if(unit>(sp_tFlashDesrc->TotalPages))
        return -1;
    if(sz.unit)
    {
        if(__embed_req(whichblock, (ptu32_t)&block, &unit))
            return (-1);
    }
    else
        block = (u32)unit;
    atom_low_t atom_low = 0;
    uint32_t start_addr = 0xc0000 + 4096 * block;
    atom_low = Int_LowAtomStart();
    djybsp_spiflash_step(DJYBSP_SPIFLASH_START);
    djybsp_erase_some_sectors(start_addr,4096);
    djybsp_spiflash_step(DJYBSP_SPIFLASH_END);
    Int_LowAtomEnd(atom_low);
    return 0;
}

static s32 Flash_Init(struct EmbdFlashDescr *Description)
{

      Description->BytesPerPage     = 1024;
      Description->PagesPerBlock    = 4;
//      Description->PagesPerSector   = SECTOR_SIZE / PAGE_SIZE;
//      Description->SectorsPerBlock  = BLOCK_SIZE / SECTOR_SIZE;
      Description->TotalPages       = 600;
      Description->ToltalBlock      = 150;
      Description->RemainBytes      = 0;

      Description->MappedStAddr = 0;
    return (0);
}

// ============================================================================
// ���ܣ���ʼ��Ƭ��SPIflash
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

    targetobj = OBJ_MatchPath(fs, &notfind);
    if(notfind)
    {
        error_printf("spiflash"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj);
    super->MediaInfo = sipflash_umedia;
    super->MediaDrv = mediadrv;

    if((s32)dwEnd == -1)
    {
        dwEnd = sp_tFlashDesrc->ToltalBlock;
        BlockNum = dwEnd - dwStart;
    }
    else
    {
        BlockNum = dwEnd - dwStart;
    }
    super->AreaSize = BlockNum * sp_tFlashDesrc->BytesPerBlock;
    super->MediaStart = dwStart * sp_tFlashDesrc->PagesPerBlock; // ��ʼunit��

    res = strlen(SpiFlashName)+strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,SpiFlashName);      //��ȡ�豸��ȫ·��
    File_BeMedia(FullPath,fs);     //�����豸�����ļ�ϵͳ
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, dwStart, dwEnd);
    return (0);
}

//-----------------------------------------------------------------------------
// ���ܣ���װƬ��Flash����
// ������doformat -- �Ƿ��ʽ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע���������֪��Ҫ��װʲô�ļ�ϵͳ�����߲���װ�ļ�ϵͳTargetFs��NULL��TargetPart��-1��
//-----------------------------------------------------------------------------
s32 ModuleInstall_SpiFlash(u32 doformat)
{
    if(!sp_tFlashDesrc)
    {
        sp_tFlashDesrc = malloc(sizeof(*sp_tFlashDesrc));
        if(!sp_tFlashDesrc)
        {
            return (-1);
        }
        Flash_Init(sp_tFlashDesrc);
        djybsp_spiflash_init();
    }

    if(doformat)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __embed_req(format, 0 , -1, &sz);
    }

    sipflash_umedia = malloc(sizeof(struct umedia)+sp_tFlashDesrc->BytesPerPage);
    if(!sipflash_umedia)
    {
        return (-1);
    }

    sipflash_umedia->mreq = __embed_req;
    sipflash_umedia->type = nand;
    sipflash_umedia->ubuf = (u8*)sipflash_umedia + sizeof(struct umedia);

    if(!Device_Create((const char*)SpiFlashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)sipflash_umedia)))
    {
        printf("\r\n: erro : device : %s addition failed.", SpiFlashName);
        free(sipflash_umedia);
        return (-1);
    }

    sSpiflashInited = true;
    return 0;
}

// =============================================================================
// ���ܣ��ж�spiflash�Ƿ�װ
// ������  ��
// ���أ��ѳɹ���װ��true����δ�ɹ���װ��false����
// ��ע��
// =============================================================================
bool_t spiflash_is_install(void)
{
    return sSpiflashInited;
}
#endif


