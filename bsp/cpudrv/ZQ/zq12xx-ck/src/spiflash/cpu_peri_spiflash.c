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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
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

