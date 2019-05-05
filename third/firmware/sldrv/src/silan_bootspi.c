#include "silan_types.h"
#include "silan_bootspi_regs.h"
#include "silan_bootspi.h"
#include "silan_errno.h"
#include "silan_printf.h"
#include "silan_pmu.h"
#include "silan_irq.h"

#if defined(__CC_ARM)||  defined ( __GNUC__ )
#include "silan_m0_cache.h"
#endif

#ifdef BOOTSPI_SUPPORT_CACHELOCK
extern unsigned char Image$$BOOTSPI_ROM$$Base;
extern unsigned char Image$$BOOTSPI_ROM$$Length;

static u32 restored;
#endif

static void delay_cycle(unsigned int num)
{
		unsigned int i;
		for(i=0; i<num; i++);
}

void ex_spiflash_power_recover_enable(u8 flash_recover_en)
{
	silan_spiflash_power_enable();
	spi_st->cache_sub_config &= ~CACHE_CRC_LIMIT_ERR_NUM(0x3f);
	spi_st->cache_sub_config |=CACHE_CRC_LIMIT_ERR_NUM(0xf);
	spi_st->cache_sub_config |=CACHE_CRC_ERR_OUT_EN;
	spi_st->flash_power_down =  0xffff;  //don't write 0
	spi_st->flash_power_up  = 0xffff;  //!0
	spi_st->cache_config |=CACHE_SET_CFG;
	if(flash_recover_en)
		spi_st->main_ctrl |= (FLASH_PWD_RESUME_EN|SPICACHE_SYSRST_EN);
	else
		spi_st->main_ctrl &= ~(FLASH_PWD_RESUME_EN|SPICACHE_SYSRST_EN);
}

void ex_spi_crc_enable(u32 addr)
{
	spi_st->cache_crc_addr = addr;
	spi_st->cache_sub_config |=CACHE_CRC_ENABLE;
	spi_st->cache_config |=CACHE_SET_CFG;
}

static void cache_xwire_sel(u8 type,u8 wire)
{
    switch(type) 
    {
			case Cache_Cmd:
				if(wire == 1) {
						spi_st->key=KEY2;
						spi_st->cache_config &= ~CACHE_CMD_WIREMODE(3);
				}else if(wire == 2){
						spi_st->key=KEY2;
						spi_st->cache_config &= ~CACHE_CMD_WIREMODE(3);
						spi_st->key=KEY2;
						spi_st->cache_config |=  CACHE_CMD_WIREMODE(1);
				} else if(wire == 4) {
						spi_st->key=KEY2;
						spi_st->cache_config &= ~CACHE_CMD_WIREMODE(3);
						spi_st->key=KEY2;
						spi_st->cache_config |=  CACHE_CMD_WIREMODE(2); 
				}
				break;
			case Cache_Addr:
				if(wire == 1) {
						spi_st->key=KEY2;
						spi_st->cache_config &= ~CACHE_ADDR_WIREMODE(3);
				}else if(wire == 2){
						spi_st->key=KEY2;
						spi_st->cache_config &= ~CACHE_ADDR_WIREMODE(3);
						spi_st->key=KEY2;
						spi_st->cache_config |=  CACHE_ADDR_WIREMODE(1);
				}else if(wire ==4 ) {
						spi_st->key=KEY2;
						spi_st->cache_config &= ~CACHE_ADDR_WIREMODE(3);
						spi_st->key=KEY2;
						spi_st->cache_config |=  CACHE_ADDR_WIREMODE(2);
				}
				break;
			case Cache_Data:
				if(wire ==1 ) {
						spi_st->key=KEY2;
						spi_st->cache_config &= ~CACHE_DATA_WIREMODE(3);
				}else if(wire == 2){
						spi_st->key=KEY2;
						spi_st->cache_config &= ~CACHE_DATA_WIREMODE(3);
						spi_st->key=KEY2;
						spi_st->cache_config |= CACHE_DATA_WIREMODE(1);
				}else if(wire == 4){
						spi_st->key=KEY2;
						spi_st->cache_config &= ~CACHE_DATA_WIREMODE(3);
						spi_st->key=KEY2;
						spi_st->cache_config |= CACHE_DATA_WIREMODE(2); 
				}
				break;
    }
}

void cache_spi_cmd_sel(u8 spi_cmd)
{
	unsigned int tmp;
/*
test
*/	
	spi_st->cache_config &=~CACHE_DUMMY_CLK(0xff);
	spi_st->cache_config &=~CAHCE_DTR_MODE;
	switch(spi_cmd)
	{
		case 0x0d:			
				spi_st->cache_config |=CACHE_DUMMY_CLK(6);
		
			  tmp = spi_st->cache_sub_config;
				tmp &=~CACHE_CMD(0xff);
				tmp |=CACHE_CMD(spi_cmd);
				spi_st->cache_sub_config = tmp;
		
			  spi_st->cache_config |=CAHCE_DTR_MODE;
				cache_xwire_sel(Cache_Cmd,1);
				cache_xwire_sel(Cache_Addr,1);
				cache_xwire_sel(Cache_Data,1);
			break;
		case 0xbd:
				spi_st->cache_config |=CACHE_DUMMY_CLK(6);	
		
			  tmp = spi_st->cache_sub_config;
				tmp &=~CACHE_CMD(0xff);
				tmp |=CACHE_CMD(spi_cmd);
				spi_st->cache_sub_config = tmp;
		
		    spi_st->cache_config |=CAHCE_DTR_MODE;
				cache_xwire_sel(Cache_Cmd,1);
				cache_xwire_sel(Cache_Addr,2);
				cache_xwire_sel(Cache_Data,2);
			break;
		case 0xed:
				spi_st->cache_config |=CACHE_DUMMY_CLK(8);		
		
			  tmp = spi_st->cache_sub_config;
				tmp &=~CACHE_CMD(0xff);
				tmp |=CACHE_CMD(spi_cmd);
				spi_st->cache_sub_config = tmp;
		
				spi_st->cache_config |=CAHCE_DTR_MODE;
				cache_xwire_sel(Cache_Cmd,1);
				cache_xwire_sel(Cache_Addr,4);
				cache_xwire_sel(Cache_Data,4);
			break;
		case 0x03:
			
				tmp = spi_st->cache_sub_config;
				tmp &=~CACHE_CMD(0xff);
				tmp |=CACHE_CMD(spi_cmd);
				spi_st->cache_sub_config = tmp;
				cache_xwire_sel(Cache_Cmd,1);
				cache_xwire_sel(Cache_Addr,1);
				cache_xwire_sel(Cache_Data,1);
			break;
		case 0x0b:
				tmp = spi_st->cache_sub_config;
				tmp &=~CACHE_CMD(0xff);
				tmp |=CACHE_CMD(spi_cmd);
				spi_st->cache_sub_config = tmp;
		
				spi_st->cache_config |=CACHE_DUMMY_CLK(8);	
				cache_xwire_sel(Cache_Cmd,1);
				cache_xwire_sel(Cache_Addr,1);
				cache_xwire_sel(Cache_Data,1);
			break;
		case 0x3b:
				tmp = spi_st->cache_sub_config;
				tmp &=~CACHE_CMD(0xff);
				tmp |=CACHE_CMD(spi_cmd);
				spi_st->cache_sub_config = tmp;
		
				spi_st->cache_config |=CACHE_DUMMY_CLK(8);	
				cache_xwire_sel(Cache_Cmd,1);
				cache_xwire_sel(Cache_Addr,1);
				cache_xwire_sel(Cache_Data,2);
			break;
		case 0x6b:
				tmp = spi_st->cache_sub_config;
				tmp &=~CACHE_CMD(0xff);
				tmp |=CACHE_CMD(spi_cmd);
				spi_st->cache_sub_config = tmp;
		
				spi_st->cache_config |=CACHE_DUMMY_CLK(8);	
				cache_xwire_sel(Cache_Cmd,1);
				cache_xwire_sel(Cache_Addr,1);
				cache_xwire_sel(Cache_Data,4);
			break;
		case  0xeb: 
				tmp = spi_st->cache_sub_config;
				tmp &=~CACHE_CMD(0xff);
				tmp |=CACHE_CMD(spi_cmd);
				spi_st->cache_sub_config = tmp;
#if 0
				spi_st->cache_sub_config &= ~CACHE_CRM_DATA(0xff);
				spi_st->cache_sub_config |= CACHE_CRM_DATA(0x20);
		    spi_st->cache_config |= CACHE_CRM_EXIST|CACHE_CRM_NOCMD;
				spi_st->cache_config |= CACHE_DUMMY_CLK(0x4);//2*2
#else
		    spi_st->cache_config |= CACHE_DUMMY_CLK(0x6);//2*2
#endif
				cache_xwire_sel(Cache_Addr,4);
				cache_xwire_sel(Cache_Data,4);
				break;
		case 0xe3:
				tmp = spi_st->cache_sub_config;
				tmp &=~CACHE_CMD(0xff);
				tmp |=CACHE_CMD(spi_cmd);
				spi_st->cache_sub_config = tmp;
#if 0
				spi_st->cache_sub_config &= ~CACHE_CRM_DATA(0xff);
				spi_st->cache_sub_config |= CACHE_CRM_DATA(0x20);
				spi_st->cache_config |= CACHE_CRM_EXIST|CACHE_CRM_NOCMD;;
#else
				spi_st->cache_config |= CACHE_DUMMY_CLK(0x2);
#endif
				cache_xwire_sel(Cache_Addr,4);
				cache_xwire_sel(Cache_Data,4);
				break;
		default:
			break;
	}
	spi_st->cache_config |=CACHE_SET_CFG;
	tmp = spi_st->spi_sub_config;	
	tmp &= ~SPI_CLK_DIV(7);
	tmp |= SPI_CLK_DIV(0);
	if(!((spi_cmd ==0x0d)||(spi_cmd ==0xbd)||(spi_cmd ==0xed))){
		if(!(tmp&SPI_CLK_DIV(0)))
			tmp |= SPI_RCV_CPOL;  //can't open in dtr mode 
		else
			tmp &=~SPI_RCV_CPOL;  
	}
	tmp |= SPI_SET_CFG;
	tmp &= ~SPI_IDLE_NUM(0x3ff);
	tmp |= SPI_IDLE_NUM(0x10);
	spi_st-> spi_sub_config = tmp;
	
}

static void spi_xwire_sel(unsigned char Type,unsigned char Wire)
{
    switch(Type) 
    {
			case Host_Cmd:
					if(Wire==1) {
						spi_st->key=KEY2;
						spi_st->spi_config &= ~SPI_CMD_WIREMODE(3);}
					else if(Wire==2){
						spi_st->key=KEY2;
						spi_st->spi_config &= ~SPI_CMD_WIREMODE(3);
						spi_st->key=KEY2;
						spi_st->spi_config |=  SPI_CMD_WIREMODE(1);}
					else if(Wire==4) {
						spi_st->key=KEY2;
						spi_st->spi_config &= ~SPI_CMD_WIREMODE(3);
						spi_st->key=KEY2;
						spi_st->spi_config |=  SPI_CMD_WIREMODE(2); }
					break;
			case Host_Addr:
					if(Wire==1) {
						spi_st->key=KEY2;
						spi_st->spi_config &= ~SPI_ADDR_WIREMODE(3);}
					else if(Wire==2){
						spi_st->key=KEY2;
						spi_st->spi_config &= ~SPI_ADDR_WIREMODE(3);
						spi_st->key=KEY2;
						spi_st->spi_config |=  SPI_ADDR_WIREMODE(1);}
					else if(Wire==4) {
						spi_st->key=KEY2;
						spi_st->spi_config &= ~SPI_ADDR_WIREMODE(3);
						spi_st->key=KEY2;
						spi_st->spi_config |=  SPI_ADDR_WIREMODE(2); }
					break;
			case Host_Data:
					if(Wire==1) {
						spi_st->key=KEY2;
						spi_st->spi_config &= ~SPI_DATA_WIREMODE(3);}
					else if(Wire==2){
						spi_st->key=KEY2;
						spi_st->spi_config &= ~SPI_DATA_WIREMODE(3);
						spi_st->key=KEY2;
						spi_st->spi_config |= SPI_DATA_WIREMODE(1);}
					else if(Wire==4){
						spi_st->key=KEY2;
						 spi_st->spi_config &= ~SPI_DATA_WIREMODE(3);
						spi_st->key=KEY2;
						spi_st->spi_config |= SPI_DATA_WIREMODE(2); }
					break;
    }
}
void spi_flash_init(u8 spiclk_sel,u8 if_preread,u8 if_rcv_cpol)
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

void spi_flash_uninit(void)
{
		spi_st->spi_sub_config &= ~NO_PROREADING;
		spi_st->spi_sub_config &= ~SPI_RCV_CPOL;
		spi_st->spi_sub_config |= SPI_SET_CFG;
		spi_st->main_ctrl &= ~HOST_NOT_CACHE;  	
}


static void  spi_read_mode_sel(u8 read_cmd,u32 addr)
{
    switch (read_cmd)
		{
			 case READ:
					spi_st->spi_cmd = SPI_OP_CMD(0x3);
					spi_st->key = KEY2;
					spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
					spi_st->spi_config |= SPI_DUMMY_CLK(0);
//           spi_st->spi_config |= SPI_ADDR_XBYTE(3);
					break;
			case 0x13:
					spi_st->spi_cmd = SPI_OP_CMD(0x13);
					spi_st->key = KEY2;
					spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
					spi_st->spi_config |= SPI_DUMMY_CLK(0);
					break;
			case FASTREAD:
					spi_st->spi_cmd = SPI_OP_CMD(0xb);
					spi_st->key = KEY2;
					spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
					spi_st->spi_config |= SPI_DUMMY_CLK(0x8);
					break;
			case 0x8b:
					spi_st->spi_cmd = SPI_OP_CMD(0xb);
					spi_st->key = KEY2;
					spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
					spi_st->spi_config |= SPI_DUMMY_CLK(0x2);
					spi_xwire_sel(Host_Addr,4);
					spi_xwire_sel(Host_Data,4);
					break;
			case DUALREAD:
					spi_st->spi_cmd = SPI_OP_CMD(0x3b);
					spi_st->key = KEY2;
					spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
					spi_st->spi_config |= SPI_DUMMY_CLK(0x8);
					spi_xwire_sel(Host_Data,2);
					break;
			case QUADREAD:
//            SPI_FLASH_write_status_reg2(0x02);
					spi_st->spi_cmd = SPI_OP_CMD(0x6b);
					spi_st->key = KEY2;
					spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
					spi_st->spi_config |= SPI_DUMMY_CLK(0x8);
					spi_xwire_sel(Host_Data,4);
					break;
			case DUALIOREAD:
					spi_st->spi_cmd = SPI_OP_CMD(0xbb);
					spi_st->key = KEY2;
					spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
					spi_st->spi_config |= SPI_DUMMY_CLK(0x4);//4*1
					spi_xwire_sel(Host_Addr,2);
					spi_xwire_sel(Host_Data,2);
					break;
			case  QUADIOREAD:
					spi_st->spi_cmd = SPI_OP_CMD(0xeb);
				//  spi_st->spi_config &= ~HOST_CRM_DATA(0xff);
				//  spi_st->spi_config |= HOST_CRM_DATA(0x20);
				//  spi_st->spi_config |= HOST_CRM_EXIST;
					spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
					spi_st->spi_config |= SPI_DUMMY_CLK(0x6);//3*2
				//  spi_st->spi_config |= SPI_DUMMY_CLK(0x4);//2*2
					spi_xwire_sel(Host_Addr,4);
					spi_xwire_sel(Host_Data,4);
					break;
			case  WORDQUADREAD:
					spi_st->spi_cmd = SPI_OP_CMD(0xe7);
					spi_st->key = KEY2;
					spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
					spi_st->spi_config |= SPI_DUMMY_CLK(0x4);//2*2
					spi_xwire_sel(Host_Addr,4);
					spi_xwire_sel(Host_Data,4);

					break;
			case  OCTALWORDQUADREAD:
					spi_st->spi_cmd = SPI_OP_CMD(0xe3);
					spi_st->key = KEY2;
					spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
					spi_st->spi_config |= SPI_DUMMY_CLK(0x2);//2*1
					spi_xwire_sel(Host_Addr,4);
					spi_xwire_sel(Host_Data,4);
					break;
			case  0x92:
					spi_st->spi_cmd = SPI_OP_CMD(0x92);
					spi_st->key = KEY2;
					spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
					spi_st->spi_config |= SPI_DUMMY_CLK(0x4);//??? 4*2
					spi_xwire_sel(Host_Addr,2);
					spi_xwire_sel(Host_Data,2);
					break;
			case  0x94:
					spi_st->spi_cmd = SPI_OP_CMD(0x94);
					spi_st->key = KEY2;
					spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
					spi_st->spi_config |= SPI_DUMMY_CLK(0x6);//??? 2*3
					spi_xwire_sel(Host_Addr,4);
					spi_xwire_sel(Host_Data,4);
					break;

			default:
					break;
    }
    spi_st->spi_addr = SPI_OP_ADDR(addr);
    spi_st->spi_subctrl &= ~HOST_DATA_NUM(3);
    spi_st->spi_subctrl |= HOST_DATA_NUM(3);


}
static int spi_flash_read_u32(u32 addr, u32 *data, u32 num, u8 read_cmd,u32 fast_mode)
{
    u32 i;
    spi_flash_init(bootspi.clk,bootspi.cfg&BOOTSPI_CFG_PROREAD,bootspi.cfg&BOOTSPI_CFG_RCV_CPOL);
    spi_read_mode_sel(read_cmd,addr);
		while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
    if(fast_mode){
			spi_st->spi_ctrl |= FAST_READ_WRITE;
			spi_st->spi_ctrl &= ~HOST_WREQ;
			spi_st->spi_ctrl |= HOST_RREQ;
    }
    else{
			spi_st->spi_ctrl &= ~FAST_READ_WRITE;
			spi_st->spi_ctrl &= ~HOST_WREQ;
			spi_st->spi_ctrl |= HOST_RREQ;
    }
    /*8bit传输*/
/*
		while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
    spi_st->spi_subctrl &= ~HOST_DATA_NUM(3);
    spi_st->spi_subctrl |= HOST_DATA_NUM(0);
*/   
		
    if(fast_mode)
			spi_st->spi_subctrl |= HOST_RREADY;
		
    for (i=0; i<num; i++){
			if(fast_mode==0x00){
				spi_st->spi_subctrl |= HOST_RREADY;
				while ((spi_st->spi_subctrl & HOST_RVALID) == 0);
				while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
			}
				data[i] = spi_st->spi_rdata;
		}
		while ((spi_st->spi_subctrl & HOST_RVALID) == 0);
		while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
    
    spi_xwire_sel(Host_Data,1);
    spi_xwire_sel(Host_Addr,1);
    spi_xwire_sel(Host_Cmd,1);

    spi_st->key = KEY2;
    spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);

    spi_st->key = KEY2; 
    spi_st->spi_ctrl &= ~HOST_RREQ;
    spi_flash_uninit(); 
    return 0;
}
static int spi_flash_read_u8(u32 addr, u8 *data, u32 num, u8 read_cmd,u32 fast_mode)
{
    unsigned int i;
    spi_flash_init(bootspi.clk,bootspi.cfg&BOOTSPI_CFG_PROREAD,bootspi.cfg&BOOTSPI_CFG_RCV_CPOL);
    spi_read_mode_sel(read_cmd,addr);
    if(fast_mode){
			spi_st->spi_ctrl |= FAST_READ_WRITE;
			spi_st->spi_ctrl &= ~HOST_WREQ;
			spi_st->spi_ctrl |= HOST_RREQ;
    }
    else{
			spi_st->spi_ctrl &= ~FAST_READ_WRITE;
			spi_st->spi_ctrl &= ~HOST_WREQ;
			spi_st->spi_ctrl |= HOST_RREQ;
    }
    /*8bit传输*/
		while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
    spi_st->spi_subctrl &= ~HOST_DATA_NUM(3);
    spi_st->spi_subctrl |= HOST_DATA_NUM(0);
		if(fast_mode)
			spi_st->spi_subctrl |= HOST_RREADY;
    
    for (i=0; i<num; i++){
			if(fast_mode==0x00){
				 spi_st->spi_subctrl |= HOST_RREADY;
				 while ((spi_st->spi_subctrl & HOST_RVALID) == 0);
				 while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
			}
			data[i] = spi_st->spi_rdata;
    }
		while ((spi_st->spi_subctrl & HOST_RVALID) == 0);
		while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);

    spi_xwire_sel(Host_Data,1);
    spi_xwire_sel(Host_Addr,1);
    spi_xwire_sel(Host_Cmd,1);
    spi_st->key = KEY2;
    spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);

    spi_st->key = KEY2;
    spi_st->spi_ctrl &= ~HOST_RREQ;
		spi_flash_uninit();
    return 0;
}


static u32 spiflash_read_status_low(void)
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



static u32 spiflash_read_status_high(void)
{
	u32 rdata = 0;
	spi_st->main_ctrl |= HOST_NOT_CACHE;
	spi_st->spi_cmd = SPI_OP_CMD(0x35);
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
int spi_status_done(void)
{
	u32 flash_status =0;
	u32 retry = 100000;
	flash_status = spiflash_read_status_low();	
	while(retry){
		if((flash_status & 0x1)  == 0){
			return 0;
		}
		flash_status = spiflash_read_status_low();
		retry --;
	}
	return -SLEAGAIN;
}
int spi_write_en_done(void)
{
    u32 flash_status =0;
    u32 retry = 10000;
    flash_status = spiflash_read_status_low();  
    while(retry){
			if((flash_status & 0x2)  == 0x2){
					 return 0;
			}
			flash_status = spiflash_read_status_low();
			retry --;
    }
    return -SLEAGAIN;
}

static int spiflash_write_enable(void)
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
	return (spi_write_en_done());
}

int spiflash_otp_mode_set(void)
{
	spi_st->main_ctrl |= HOST_NOT_CACHE;
	spi_st->spi_cmd = SPI_OP_CMD(0x3a);
	spi_st->spi_subctrl &=~HOST_DATA_NUM(3);
	spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
	spi_st->key = KEY2;
	spi_st->spi_ctrl = HOST_WREQ;
	while ((spi_st->spi_subctrl & HOST_WREADY) == 0);
	while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
	spi_st->key = KEY2;
	spi_st->spi_ctrl &= ~HOST_WREQ;
	spi_st->main_ctrl &= ~HOST_NOT_CACHE;
	return 0;
}
int spiflash_otp_mode_exit(void)
{
	spi_st->main_ctrl |= HOST_NOT_CACHE;
	spi_st->spi_cmd = SPI_OP_CMD(0x04);
	spi_st->spi_subctrl &=~HOST_DATA_NUM(3);
	spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
	spi_st->key = KEY2;
	spi_st->spi_ctrl = HOST_WREQ;
	while ((spi_st->spi_subctrl & HOST_WREADY) == 0);
	while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
	spi_st->key = KEY2;
	spi_st->spi_ctrl &= ~HOST_WREQ;
	spi_st->main_ctrl &= ~HOST_NOT_CACHE;
	return 0;
}

void spiflash_write_disable(void)
{
	spi_st->main_ctrl |= HOST_NOT_CACHE;
	spi_st->spi_cmd = SPI_OP_CMD(4);
	spi_st->spi_subctrl &=~HOST_DATA_NUM(3);
	spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
	spi_st->key = KEY2;
	spi_st->spi_ctrl = HOST_WREQ;
	while ((spi_st->spi_subctrl & HOST_WREADY) == 0);
	while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
	spi_st->key = KEY2;
	spi_st->spi_ctrl &= ~HOST_WREQ;
	spi_st->main_ctrl &= ~HOST_NOT_CACHE;
}
void spiflash_write_sr_enable(void)
{
	spi_st->main_ctrl |= HOST_NOT_CACHE;
	spi_st->spi_cmd = SPI_OP_CMD(0x50);
	spi_st->spi_subctrl &=~HOST_DATA_NUM(3);
	spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
	spi_st->key = KEY2;
	spi_st->spi_ctrl = HOST_WREQ;
	while ((spi_st->spi_subctrl & HOST_WREADY) == 0);
	while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
	spi_st->key = KEY2;
  spi_st->spi_ctrl &= ~HOST_WREQ;
	spi_st->main_ctrl &= ~HOST_NOT_CACHE;
}
static int spi_flash_write_status_reg1(u8 reg_val)
{   
	int g_result;
	spiflash_write_enable();
	spiflash_write_sr_enable();
	g_result =spi_status_done();
	if(g_result)
			return g_result;
	
	spi_st->main_ctrl |= HOST_NOT_CACHE;
	spi_st->spi_cmd = SPI_OP_CMD(0x01);
	spi_st->spi_subctrl &=~HOST_DATA_NUM(3);
	spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
	spi_st->key = KEY2;
	spi_st->spi_ctrl = HOST_WREQ; 
	while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);  
	while ((spi_st->spi_subctrl & HOST_WREADY) == 0);   
	spi_st->spi_wdata = reg_val;                        
	spi_st->spi_subctrl |= HOST_WVALID;                    
	while ((spi_st->spi_subctrl & HOST_WREADY) == 0);        
	while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);  
	spi_st->key = KEY2;
	spi_st->spi_ctrl &= ~HOST_WREQ;
	spi_st->main_ctrl &= ~HOST_NOT_CACHE;
	return 0;
}
static int spi_flash_write_status_reg2(u8 reg_val)
{   
	int g_result;
	spiflash_write_enable();
	spiflash_write_sr_enable();
	g_result = spi_status_done();
	if(g_result)
		return g_result;
	
	spi_st->main_ctrl |= HOST_NOT_CACHE;
	spi_st->spi_cmd = SPI_OP_CMD(0x31);
	spi_st->spi_subctrl &=~HOST_DATA_NUM(3);
	spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
	spi_st->key = KEY2;
	spi_st->spi_ctrl = HOST_WREQ; 
	while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);  
	while ((spi_st->spi_subctrl & HOST_WREADY) == 0);   
	spi_st->spi_wdata = (unsigned char)(reg_val);                        
	spi_st->spi_subctrl |= HOST_WVALID;                    
	while ((spi_st->spi_subctrl & HOST_WREADY) == 0);        
	while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);  
	spi_st->key = KEY2;
	spi_st->spi_ctrl &= ~HOST_WREQ;
	spi_st->main_ctrl &= ~HOST_NOT_CACHE;
	return 0;
}

int spi_quad_status_done(void)
{
	unsigned int flash_status =0;
  	unsigned int retry = 10;	
	//spiflash_otp_mode_set();
	flash_status = spiflash_read_status_high();	
	SL_DBGLOG("status_reg2=%x", flash_status);

	while(retry){
		if((flash_status & 0x2)  == 0x2){
			 return 0;
		}
		spi_flash_write_status_reg2(0x2);
		flash_status = spiflash_read_status_high();
		SL_DBGLOG("status_reg2=%x", flash_status);
		retry--;
	}
//	spiflash_otp_mode_exit();
	return -SLEAGAIN;
}

int spi_flash_enter_qpi_mode(void)
{   
	int g_result;
	spiflash_write_disable();
	g_result = spi_status_done();
	if(g_result)
		return g_result;
	
	spi_st->main_ctrl |= HOST_NOT_CACHE;
	spi_st->spi_cmd = SPI_OP_CMD(0x38);
	spi_st->spi_subctrl &= ~HOST_DATA_NUM(3);
	spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
	spi_st->key = KEY2;
	spi_st->spi_ctrl = HOST_WREQ;
	while ((spi_st->spi_subctrl & HOST_WREADY) == 0);
	while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);  
	spi_st->key = KEY2;
	spi_st->spi_ctrl &= ~HOST_WREQ;
	spi_xwire_sel(Host_Addr,4);
	spi_xwire_sel(Host_Data,4);
	spi_xwire_sel(Host_Cmd,4);
	spi_st->main_ctrl &= ~HOST_NOT_CACHE;
	return 0;
}
void spi_flash_exit_qpi_mode(void)
{   
	spiflash_write_disable();
	
	spi_st->main_ctrl |= HOST_NOT_CACHE;
	spi_st->spi_cmd = SPI_OP_CMD(0xFF);
	spi_st->spi_subctrl &=~HOST_DATA_NUM(3);
	spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
	spi_st->key = KEY2;
	spi_st->spi_ctrl = HOST_WREQ;
	while ((spi_st->spi_subctrl & HOST_WREADY) == 0);
	while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);	
	spi_st->key = KEY2;
	spi_st->spi_ctrl &= ~HOST_WREQ;
	spi_st->main_ctrl &= ~HOST_NOT_CACHE;
}

static int spi_flash_pagewrite_u32(u32* pBuffer, u32 WriteAddr, u32 NumByteToWrite,u32 fast_mode)
{
	u32 i;
	int g_result;
	spi_flash_init(bootspi.clk,bootspi.cfg&BOOTSPI_CFG_PROREAD,bootspi.cfg&BOOTSPI_CFG_RCV_CPOL);
	spiflash_write_enable();	                 
	g_result = spi_status_done(); 
	if(g_result){
			spi_flash_uninit();
			return g_result;
	}
	spi_st->main_ctrl |= HOST_NOT_CACHE;
	spi_st->spi_cmd = SPI_OP_CMD(2) ;  
	spi_st->spi_addr =  SPI_OP_ADDR(WriteAddr); 
	spi_st->spi_subctrl |= HOST_DATA_NUM(3);               
	spi_st->key = KEY2;
	if(fast_mode){
			spi_st->spi_ctrl = FAST_READ_WRITE;
			spi_st->spi_ctrl &= ~HOST_RREQ;
			spi_st->spi_ctrl |= HOST_WREQ;
	}else{
			spi_st->spi_ctrl &= ~FAST_READ_WRITE;
			spi_st->spi_ctrl &= ~HOST_RREQ;
			spi_st->spi_ctrl |= HOST_WREQ;
			while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
			while ((spi_st->spi_subctrl & HOST_WREADY) == 0);      
	}
	for (i=0; i<NumByteToWrite; i++){
			spi_st->spi_wdata = pBuffer[i];                    
			if(fast_mode==0x00){
					spi_st->spi_subctrl |= HOST_WVALID;
					while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0); 
					while ((spi_st->spi_subctrl & HOST_WREADY) == 0);      
			}
	}
	while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);  
	spi_st->key = KEY2;
	spi_st->spi_ctrl &= ~HOST_WREQ;   
  spi_st->main_ctrl &= ~HOST_NOT_CACHE;	
	g_result = spi_status_done(); 
	spi_flash_uninit();
	if(g_result)
			return g_result;
	return 0;
}

static int spi_flash_pagewrite_u8(u8* pBuffer, u32 WriteAddr, u32 NumByteToWrite,u32 fast_mode)
{
	int g_result;
	unsigned int i;
	spi_flash_init(bootspi.clk,bootspi.cfg&BOOTSPI_CFG_PROREAD,bootspi.cfg&BOOTSPI_CFG_RCV_CPOL);
	spiflash_write_enable();	                   
	g_result = spi_status_done(); 
	if(g_result)
	{
		spi_flash_uninit();
		return g_result;
	}
	spi_st->main_ctrl |= HOST_NOT_CACHE;
	spi_st->spi_cmd = SPI_OP_CMD(2) ;  
	spi_st->spi_addr =  SPI_OP_ADDR(WriteAddr);  
	spi_st->spi_subctrl |= HOST_DATA_NUM(3);                  
	spi_st->key = KEY2;
	if(fast_mode){
			spi_st->spi_ctrl = FAST_READ_WRITE;
			spi_st->spi_ctrl &= ~HOST_RREQ;
			spi_st->spi_ctrl |= HOST_WREQ;
	}else{
			spi_st->spi_ctrl &= ~FAST_READ_WRITE;
			spi_st->spi_ctrl &= ~HOST_RREQ;
			spi_st->spi_ctrl |= HOST_WREQ;
			while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);
			while ((spi_st->spi_subctrl & HOST_WREADY) == 0);    
	}
	
	spi_st->spi_subctrl &= ~HOST_DATA_NUM(3);
	spi_st->spi_subctrl |= HOST_DATA_NUM(0);
	
	for (i=0; i<NumByteToWrite; i++){
		spi_st->spi_wdata = pBuffer[i];                       
		if(fast_mode==0x00){
					spi_st->spi_subctrl |= HOST_WVALID;
					while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0); 
					while ((spi_st->spi_subctrl & HOST_WREADY) == 0);      
				}
	}
	while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);  
	spi_st->key = KEY2;
	spi_st->spi_ctrl &= ~HOST_WREQ; 
  spi_st->main_ctrl &= ~HOST_NOT_CACHE;	
	g_result = spi_status_done(); 
	spi_flash_uninit();
	if(g_result)
		return g_result;
	return 0;
}
static int spi_flash_sectorerase(u32 SectorAddr)
{
	int g_result;
	spi_flash_init(bootspi.clk,bootspi.cfg&BOOTSPI_CFG_PROREAD,bootspi.cfg&BOOTSPI_CFG_RCV_CPOL);
	spiflash_write_enable();	                           
	g_result = spi_status_done(); 
	if(g_result){
		spi_flash_uninit();
		return g_result;
	}
	spi_st->main_ctrl |= HOST_NOT_CACHE;
	spi_st->spi_cmd = SPI_OP_CMD(SE);  
	spi_st->spi_addr = SPI_OP_ADDR(SectorAddr);  
	spi_st->spi_subctrl |= HOST_DATA_NUM(3);
	
	spi_st->spi_config &= ~SPI_DUMMY_CLK(0xff);
	spi_st->key = KEY2;
	spi_st->spi_ctrl = HOST_WREQ;
	while ((spi_st->spi_subctrl & HOST_WREADY) == 0);
	while ((spi_st->spi_subctrl & HOST_SPI_TRANS_IDLE) == 0);	 
	spi_st->key = KEY2;
	spi_st->spi_ctrl &= ~HOST_WREQ;
	spi_st->main_ctrl &= ~HOST_NOT_CACHE;
	g_result = spi_status_done(); 
	spi_flash_uninit();
	if(g_result)
		return g_result;    
	return 0;
}

bootspi_t bootspi;
int bootspi_init(u32 clk_div,u32 mode_cfg)
{
	bootspi.cfg = mode_cfg;
	bootspi.clk = clk_div;
	return 0;
}
int bootspi_sectorerase(u32 addr, u32 bytes)
{
	//sector erase
	u32 sector_num = 0;
	u32 i = 0;
  if(addr%(4*1024) != 0x00)
        return -SLEFAULT;
	if(bytes%(1024*4)==0x00)
			sector_num = bytes/1024/4;
	else if(bytes%(1024*4)!=0x00)
			sector_num = bytes/1024/4 + 1;
	for(i=0;i<sector_num;i++)
    {
        if(spi_flash_sectorerase(addr+4*1024*i))
        return -SLEERROR;
    }
	return 0;
}

int bootspi_read32(u32 addr, u32 *buf, u32 count)
{	
	u32 i;
	if(count>1024)
		return -SLENOMEM;
	for(i=0;i<16;i++)
		spi_flash_read_u32(addr+i*256,&buf[i*64],64,FASTREAD,bootspi.cfg&BOOTSPI_CFG_FASTMODE);
	return 0;
}
int bootspi_read8(u32 addr, u8 *buf, u32 count)
{
	u32 i;
	if(count>4096)
		return -SLENOMEM;
	for(i=0;i<16;i++)
		spi_flash_read_u8(addr+i*256,&buf[i*256],256,FASTREAD,bootspi.cfg&BOOTSPI_CFG_FASTMODE);
	return 0;
}

int bootspi_io_done(void)
{
	return(spi_status_done()); 
}

#ifdef BOOTSPI_SUPPORT_CACHELOCK
void bootspi_cachelock_init(void)
{
	u32 RoBase, RoLength, Remain;

	restored = pic_get_enable();
	pic_clr_enable(restored);
	//SL_LOG("restored = %08x", restored);

	RoBase = ((int)&Image$$BOOTSPI_ROM$$Base) & 0xFFFFFFF0;
	Remain = RoLength - RoBase;
	RoLength = ((int)&Image$$BOOTSPI_ROM$$Length + Remain + M0_CACHE_LINE_SIZE - 1) / M0_CACHE_LINE_SIZE;
	//SL_LOG("BASE = %08x", RoBase);
	//SL_LOG("LEGTH = %08x", RoLength);
	
	m0_cache_init(CACHE_DISABLE, 0, 0, 0);
	m0_cache_init(CACHE_ENABLE, 1, RoBase, RoLength);
}

void bootspi_cachelock_quit(void)
{
	m0_cache_init(CACHE_DISABLE, 0, 0, 0);
	m0_cache_init(CACHE_ENABLE, 0, 0, 0);

	pic_set_enable(restored);
}
#endif

