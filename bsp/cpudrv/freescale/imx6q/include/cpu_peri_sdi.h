#ifndef __CPU_PERI_SDI_H__
#define __CPU_PERI_SDI_H__
#include "stdint.h"
#ifdef __cplusplus
extern "C" {
#endif

#define INICLK  300000
#define SDCLK   24000000    //PCLK=49.392MHz
#define MMCCLK  15000000    //PCLK=49.392MHz

#define POL 0
#define INT 1
#define DMA 2

#define rGPEUP     (*(volatile unsigned *)0x56000048)   //Pull-up control E
#define rGPECON    (*(volatile unsigned *)0x56000040)   //Port E control

// SD Interface
#define rSDICON     (*(volatile unsigned *)0x5a000000)  //SDI control
#define rSDIPRE     (*(volatile unsigned *)0x5a000004)  //SDI baud rate prescaler
#define rSDICARG    (*(volatile unsigned *)0x5a000008)  //SDI command argument
#define rSDICCON    (*(volatile unsigned *)0x5a00000c)  //SDI command control
#define rSDICSTA    (*(volatile unsigned *)0x5a000010)  //SDI command status
#define rSDIRSP0    (*(volatile unsigned *)0x5a000014)  //SDI response 0
#define rSDIRSP1    (*(volatile unsigned *)0x5a000018)  //SDI response 1
#define rSDIRSP2    (*(volatile unsigned *)0x5a00001c)  //SDI response 2
#define rSDIRSP3    (*(volatile unsigned *)0x5a000020)  //SDI response 3
#define rSDIDTIMER  (*(volatile unsigned *)0x5a000024)  //SDI data/busy timer
#define rSDIBSIZE   (*(volatile unsigned *)0x5a000028)  //SDI block size
#define rSDIDCON    (*(volatile unsigned *)0x5a00002c)  //SDI data control
#define rSDIDCNT    (*(volatile unsigned *)0x5a000030)  //SDI data remain counter
#define rSDIDSTA    (*(volatile unsigned *)0x5a000034)  //SDI data status
#define rSDIFSTA    (*(volatile unsigned *)0x5a000038)  //SDI FIFO status
#define rSDIIMSK    (*(volatile unsigned *)0x5a00003c)  //SDI interrupt mask. edited for 2440A

#ifdef __BIG_ENDIAN  /* edited for 2440A */
#define rSDIDAT    (*(volatile unsigned *)0x5a00004c)   //SDI data
#define SDIDAT     0x5a00004c
#else  // Little Endian
#define rSDIDAT    (*(volatile unsigned *)0x5a000040)   //SDI data
#define SDIDAT     0x5a000040
#endif   //SD Interface


// Function prototypes
void byte_conversion(uint32_t *inta,uint32_t *intb);
int cmd13(void);    // Send card status
int cmd9(void);
int module_init_SD(void);
int chk_dat_end(void);
int chk_cmd_end(int cmd, int be_resp);
int chk_MMC_OCR(void);
int chk_SD_OCR(void);
void card_sel_desel(char sel_desel);
void cmd0(void);
int cmd55(void);
void read_one_sector(u32 sector_addr,unsigned int *buf);
void write_one_sector(u32 sector_addr,unsigned int *buf);
void set_4bit_bus(void);

#ifdef __cplusplus
}
#endif

#endif // __CPU_PERI_SDI_H__

