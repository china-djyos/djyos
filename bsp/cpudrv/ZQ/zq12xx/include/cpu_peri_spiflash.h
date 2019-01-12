#ifndef CPU_PERI_SPIFLASH_H_
#define CPU_PERI_SPIFLASH_H_

#ifdef CFG_CK803S
#include <stdint.h>

#define PAGE_SIZE			  (0x100)	    /* 256 byte */
#define SECTOR_SIZE          (0x1000)      /* 4 Kbyte */
#define BLOCK_SIZE           (0x10000)
#define FLASH_SIZE			  (0x400000)    /* 4 MBytes */

#define DJYBSP_SPIFLASH_START	(0U)
#define DJYBSP_SPIFLASH_END		(1U)

int32_t djybsp_spiflash_init(void);
int32_t djybsp_erase_some_sectors(uint32_t addr, uint32_t size);
int32_t djybsp_erase_some_blocks(uint32_t addr, uint32_t size);
int32_t djybsp_program_one_package(char *data, uint32_t addr, uint32_t size);
void djybsp_spiflash_step(uint8_t step);
uint32_t djybsp_get_flash_capacity(void);

#endif
#endif//CPU_PERI_SPIFLASH_H_
