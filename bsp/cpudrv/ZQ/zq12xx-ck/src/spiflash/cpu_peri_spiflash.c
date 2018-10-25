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
#include "int.h"

bootspi_t iap_bootspi;

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
    static atom_low_t atom_low = 0;
    switch(step)
    {
        case 0:
            atom_low = Int_LowAtomStart();
            csi_icache_disable();
            csi_dcache_disable();
            csi_dcache_clean();
            djybsp_spiflash_init();
            break;
        case 1:
            csi_dcache_enable();
            csi_icache_enable();
            if(atom_low!=0)
                Int_HighAtomEnd(atom_low);
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

