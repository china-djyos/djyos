/*
 * silan_bootspi.h
 */

#ifndef __BOOTSPI_H__
#define __BOOTSPI_H__

#include "silan_types.h"

//#define BOOTSPI_SUPPORT_CACHELOCK

#define BOOTSPI_CFG_MOD_SINGLE           (0x0<<0)
#define BOOTSPI_CFG_MOD_DUAL             (0x1<<0)
#define BOOTSPI_CFG_MOD_QUAD             (0x2<<0)
#define BOOTSPI_CFG_MOD_MASK             (0x3<<0)
#define BOOTSPI_CFG_ADDR_WIDTH(x)        (((x)&0x3)<<2)
#define BOOTSPI_CFG_ADDR_WIDTH_MASK      (0x3<<2)
#define BOOTSPI_CFG_DATA_WIDTH(x)        (((x)&0x3)<<4)
#define BOOTSPI_CFG_DATA_WIDTH_MASK      (0x3<<4)
#define BOOTSPI_CFG_DTR                  (0x1<<6)
#define BOOTSPI_CFG_CRC                  (0x1<<7)
#define BOOTSPI_CFG_FASTMODE             (0x1<<8)
#define BOOTSPI_CFG_RCV_CPOL             (0x1<<9)
#define BOOTSPI_CFG_PROREAD            	 (0x1<<10)

#define BOOTSPI_CLK_DIV2                 (0x0)
#define BOOTSPI_CLK_DIV4                 (0x1)
#define BOOTSPI_CLK_DIV8                 (0x2)
#define BOOTSPI_CLK_DIV16                (0x3)
#define BOOTSPI_CLK_DIV32                (0x4)
#define BOOTSPI_CLK_DIV64                (0x5)
#define BOOTSPI_CLK_DIV128               (0x6)
#define BOOTSPI_CLK_DIV256               (0x7)

typedef enum {
		WRITE   =   0x02 , /* Write to Memory instruction */
		WRSR    =   0x01 , /* Write Status Register instruction */ 
		WREN    =   0x06 , /* Write enable instruction */
		READ    =   0x03 , /* Read from Memory instruction */
		RDSR    =   0x05 , /* Read Status Register instruction  */
		RDID    =   0x9F , /* Read identification */
		SE      =   0x20 , /* Sector Erase instruction */
		BE      =   0xD8 , /* Bulk Erase instruction */
		FASTREAD =  0x0b ,
		DUALREAD =  0x3b ,
		QUADREAD =  0x6b ,
	
		DUALIOREAD = 0xbb ,
		QUADIOREAD = 0xeb ,
		WORDQUADREAD =  0xe7 ,
		OCTALWORDQUADREAD =  0xe3 ,
			 
}spicmd;

typedef enum {
	SF_CMD_DE		= 0x0D, // Dual Edge
	SF_CMD_DDE		= 0xBD, // Dual Line Dual Edge
	SF_CMD_QDE		= 0xED, // Quad Line Dual Edge
	SF_CMD_READ		= 0x03, // Read
	SF_CMD_FR		= 0x0B, // Fast Read
	SF_CMD_FRD		= 0x3B, // Fast Read Dual
	SF_CMD_FRQ		= 0x6B, // Fast Read Quad
} bootspi_cache_cmd_t;

typedef struct {
	u32 cfg;
	u32 clk;
	u32 crc_tbl_addr;
}bootspi_t;

extern bootspi_t bootspi;
/*
bootspi_init()
clk_div :BOOTSPI_CLK_DIV x
mode_cfg:BOOTSPI_CFG_MOD_X
...
*/
int bootspi_init(u32 clk_div,u32 mode_cfg);
/*
bootspi_read32()
addr: addr%4k == 0
count:<=4k
...
*/
int bootspi_read32(u32 addr, u32 *buf, u32 count);
/*
bootspi_write32()
addr: addr%4k == 0
count;offset : count+offset<=1k
...
*/
int bootspi_write32(u32 addr, u32 *buf, u32 count,u32 offset);
/*
bootspi_read8()
addr: addr%4k == 0
count:<=4k
...
*/
int bootspi_read8(u32 addr, u8 *buf, u32 count);
/*
bootspi_write8()
addr: addr%4k == 0
count;offset : count+offset<=4k
...
*/
int bootspi_write8(u32 addr, u8 *buf, u32 count,u32 offset);
/*
bootspi_sectorerase()
addr: addr%4k == 0
...
*/
int bootspi_sectorerase(u32 addr, u32 bytes);
/*
bootspi_io_done()
spi busy wait
...
*/
int bootspi_io_done(void);

int spi_quad_status_done(void);
void ex_spiflash_power_recover_enable(u8 flash_recover_en);
void ex_spi_crc_enable(u32 addr);
void cache_spi_cmd_sel(u8 spi_cmd);

#endif

