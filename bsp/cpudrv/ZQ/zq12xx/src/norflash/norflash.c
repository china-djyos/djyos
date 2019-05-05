#include "silan_types.h"
#include "silan_bootspi.h"
#include "silan_errno.h"
//#include "iap.h"
#include "silan_bootspi_regs.h"
#include "errno.h"
#include "norflash.h"
//#include "common.h"

#ifdef __DEBUG__
#define FLASH_LOG(x)		//printk(x)
#else
#define FLASH_LOG(x)
#endif

bootspi_t iap_bootspi;

static void iap_spi_init(uint8_t spiclk_sel,uint8_t if_preread,uint8_t if_rcv_cpol)
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

static void iap_spi_uninit(void)
{
	spi_st->spi_sub_config &= ~NO_PROREADING;
	spi_st->spi_sub_config &= ~SPI_RCV_CPOL;
	spi_st->spi_sub_config |= SPI_SET_CFG;
	spi_st->main_ctrl &= ~HOST_NOT_CACHE;  	
}

static uint32_t iap_spiflash_read_status(void)
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

static uint32_t iap_spiflash_read_capid(void)
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

static int iap_spiflash_done(void)
{
	uint32_t flash_status =0;
	uint32_t retry = 100000;
	flash_status = iap_spiflash_read_status();	
	while(retry){
		if((flash_status & 0x1)  == 0){
			return 0;
		}
		flash_status = iap_spiflash_read_status();
		retry --;
	}
	return -EAGAIN;
}

static int iap_spiflash_write_done(void)
{
    uint32_t flash_status =0;
    uint32_t retry = 10000;
    flash_status = iap_spiflash_read_status();  
    while(retry){
		if((flash_status & 0x2)  == 0x2){
				 return 0;
		}
		flash_status = iap_spiflash_read_status();
		retry --;
    }
    return -EAGAIN;
}

static int iap_spiflash_write_enable(void)
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
	return (iap_spiflash_write_done());
}

int32_t iap_spiflash_pagewrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
	int g_result;
	unsigned int i;
	FLASH_LOG("pagewrite 1\r\n");
	iap_spi_init(iap_bootspi.clk, iap_bootspi.cfg&BOOTSPI_CFG_PROREAD, iap_bootspi.cfg&BOOTSPI_CFG_RCV_CPOL);
	iap_spiflash_write_enable();	                   
	g_result = iap_spiflash_done();
	if(g_result)
	{
		iap_spi_uninit();
		return g_result;
	}
	FLASH_LOG("pagewrite 2\r\n");
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

	FLASH_LOG("pagewrite 3\r\n");
	spi_st->spi_subctrl &= ~HOST_DATA_NUM(3);
	spi_st->spi_subctrl |= HOST_DATA_NUM(0);
	
	for (i=0; i<NumByteToWrite; i++){
		spi_st->spi_wdata = pBuffer[i];                       
		spi_st->spi_subctrl |= HOST_WVALID;
		while ((spi_st->spi_subctrl & HOST_WREADY) == 0);      
		while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0); 
	}
	FLASH_LOG("pagewrite 4\r\n");
	while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);  
	spi_st->key = KEY2;
	spi_st->spi_ctrl &= ~HOST_WREQ; 
  	spi_st->main_ctrl &= ~HOST_NOT_CACHE;	
	g_result = iap_spiflash_done(); 
	iap_spi_uninit();
	if(g_result)
		return g_result;
	FLASH_LOG("pagewrite 5\r\n");
	return 0;
}

int32_t iap_spiflash_erase(uint32_t Addr, uint8_t cmd)
{
	int g_result;
	FLASH_LOG("erase 1\r\n");
	iap_spi_init(iap_bootspi.clk, iap_bootspi.cfg&BOOTSPI_CFG_PROREAD, iap_bootspi.cfg&BOOTSPI_CFG_RCV_CPOL);
	iap_spiflash_write_enable();
	g_result = iap_spiflash_done();
	if(g_result){
		iap_spi_uninit();
		return g_result;
	}
	FLASH_LOG("erase 2\r\n");
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
	FLASH_LOG("erase 3\r\n");
	g_result = iap_spiflash_done();
	iap_spi_uninit();
	if(g_result)
		return g_result;    
	FLASH_LOG("erase 4\r\n");
	return 0;
}

int32_t EraseSomeSectors(uint32_t addr, uint32_t size)
{
	uint32_t sector, start, remain;
	uint32_t i;
	start  = addr & 0x00FFF000;
	remain = addr - start;
	sector = (size + remain + SECTOR_SIZE - 1) / SECTOR_SIZE;
	for(i=0; i<sector; i++)
		if(iap_spiflash_erase(i*SECTOR_SIZE + start, 0x20))
			return 0;
	return 1;
}

int32_t EraseSomeBlocks(uint32_t addr, uint32_t size)
{
	uint32_t sector, start, remain;
	uint32_t i;
	start  = addr & 0x00FF0000;
	remain = addr - start;
	sector = (size + remain + BLOCK_SIZE - 1)/ BLOCK_SIZE;
	for(i=0; i<sector; i++)
		if(iap_spiflash_erase(i*BLOCK_SIZE + start, 0xD8))
			return 0;
	return 1;
}

int32_t ProgramOnePackage(char *data, uint32_t addr, uint32_t size)
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
		iap_spiflash_pagewrite(ptr, faddr, remain);
	}
	else {
		for(i=0; i<page; i++) {
			iap_spiflash_pagewrite(ptr, faddr, PAGE_SIZE);
			ptr += PAGE_SIZE;
			faddr += PAGE_SIZE;
		}
		if(remain != 0)
			iap_spiflash_pagewrite(ptr, faddr, remain);
	}
	return 0;
}

uint32_t GetFlashCapacity(void)
{
    uint32_t cap;
    cap = (1 << (iap_spiflash_read_capid() & 0xff));
    return cap;
}

int32_t iap_norflash_init()
{
	iap_bootspi.cfg = 0;
	iap_bootspi.clk = BOOTSPI_CLK_DIV4;
	return 0;
}

