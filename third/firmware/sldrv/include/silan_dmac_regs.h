/*
 * silan_dmac_regs.h
 */

#ifndef __SILAN_DMAC_REGS_H__
#define __SILAN_DMAC_REGS_H__

#include "silan_types.h"
#include "ap1508.h"

/********************************************************************
 *                      DMAC REQUEST MATRIX                         *
 *******************************************************************/

#define SILAN_ADMAC_REQ0_BASE_ADDR  (SCFG_APB_DMAREQ_ADDR_BASE + 0x00)
#define SILAN_ADMAC_REQ1_BASE_ADDR  (SCFG_APB_DMAREQ_ADDR_BASE + 0x04)
#define SILAN_ADMAC_REQ2_BASE_ADDR  (SCFG_APB_DMAREQ_ADDR_BASE + 0x08)
#define SILAN_ADMAC_REQ3_BASE_ADDR  (SCFG_APB_DMAREQ_ADDR_BASE + 0x0c)
#define SILAN_SDMAC_REQ0_BASE_ADDR  (SCFG_APB_DMAREQ_ADDR_BASE + 0x10)
#define SILAN_SDMAC_REQ1_BASE_ADDR  (SCFG_APB_DMAREQ_ADDR_BASE + 0x14)
#define SILAN_SDMAC_REQ2_BASE_ADDR  (SCFG_APB_DMAREQ_ADDR_BASE + 0x18)
#define SILAN_SDMAC_REQ3_BASE_ADDR  (SCFG_APB_DMAREQ_ADDR_BASE + 0x1c)

#define DMAC_REQ_SRCID_SPDIF_O         0
#define DMAC_REQ_SRCID_SPDIF_I         1
#define DMAC_REQ_SRCID_PCM_I2S_TX      2
#define DMAC_REQ_SRCID_PCM_I2S_RX      3
#define DMAC_REQ_SRCID_O3_I2S_TX       4
#define DMAC_REQ_SRCID_O2_I2S_TX       5
#define DMAC_REQ_SRCID_I3_I2S_RX       6
#define DMAC_REQ_SRCID_I2_I2S_RX       7
#define DMAC_REQ_SRCID_O1_I2S_TX0      8
#define DMAC_REQ_SRCID_O1_I2S_TX1      9
#define DMAC_REQ_SRCID_O1_I2S_TX2     10
#define DMAC_REQ_SRCID_I1_I2S_RX0     11
#define DMAC_REQ_SRCID_I1_I2S_RX1     12
#define DMAC_REQ_SRCID_I1_I2S_RX2     13
#define DMAC_REQ_SRCID_IIR_AI         14
#define DMAC_REQ_SRCID_IIR_AO         15
#define DMAC_REQ_SRCID_IIR_BI         16
#define DMAC_REQ_SRCID_IIR_BO         17
#define DMAC_REQ_SRCID_PDM            18

#define DMAC_REQ_SRCID_UARTHS_RX      20
#define DMAC_REQ_SRCID_UARTHS_TX      21
#define DMAC_REQ_SRCID_SPI1_TX        22
#define DMAC_REQ_SRCID_SPI1_RX        23
#define DMAC_REQ_SRCID_SPI2_TX        24
#define DMAC_REQ_SRCID_SPI2_RX        25	

#define DMAC_REQ_SRCID_UARTLS_RX      28
#define DMAC_REQ_SRCID_UARTLS_TX      29

#define DMAC_REQ_SRCID_PDP            30	
#define DMAC_REQ_SRCID_NULL           31

typedef enum DMAC_dir
{
	DMAC_DIR_M2M	= 0,
	DMAC_DIR_M2P,
	DMAC_DIR_P2M,
	DMAC_DIR_LMT,
}
DMAC_DIR, *PDMAC_DIR;

typedef struct DMAC_chnl_regs
{
	u32 srcaddr;
	u32 destaddr;
	u32 lli;
	u32 control;
	u32 config;
	u32 reserved1;
	u32 reserved2;
	u32 reserved3;
}
DMAC_CHNL_REGS, *PDMAC_CHNL_REGS;

#define DMAC_NUM_RSRVD_WRDS_BEFORE_CHANNELS       ((0x100-0x038)>>2)
#define DMAC_NUM_RSRVD_WRDS_BEFORE_PREIPHERAL_ID  ((0xfe0-0x200)>>2)

typedef struct DMAC_core_regs
{
	u32 intStatus;
	u32 intTCStatus;
	u32 intTCClear;
	u32 intErrorStatus;
	u32 intErrorClear;
	u32 rawIntTCStatus;
	u32 rawIntErrorStatus;
	u32 activeChannels;

	u32 softBReq;    /* SPECIAL USAGE */
	u32 softSReq;    /* SPECIAL USAGE */
	u32 softLBReq;   /* SPECIAL USAGE */
	u32 softSBReq;   /* SPECIAL USAGE */

	u32 DMACConfiguration;
	u32 sync;        /* SPECIAL USAGE */

	u32 Reserved1[DMAC_NUM_RSRVD_WRDS_BEFORE_CHANNELS];

	DMAC_CHNL_REGS sDmaChannels[8];

	u32 Reserved2[DMAC_NUM_RSRVD_WRDS_BEFORE_PREIPHERAL_ID];

	u32 peripheralid0;
	u32 peripheralid1;
	u32 peripheralid2;
	u32 peripheralid3;

	u32 cellid0;
	u32 cellid1;
	u32 cellid2;
	u32 cellid3;
}
DMAC_CORE_REGS, *PDMAC_CORE_REGS;

#define DMAC_DMEM                   0
#define DMAC_DBUS                   1

#define DMAC_SMEM                   0
#define DMAC_SBUS                   1

#define DMAC_WIDTH_8BIT             0
#define DMAC_WIDTH_16BIT            1
#define DMAC_WIDTH_32BIT            2

#define DMAC_BURST_SIZE_1           0
#define DMAC_BURST_SIZE_4           1
#define DMAC_BURST_SIZE_8           2
#define DMAC_BURST_SIZE_16          3
#define DMAC_BURST_SIZE_32          4
#define DMAC_BURST_SIZE_64          5
#define DMAC_BURST_SIZE_128         6
#define DMAC_BURST_SIZE_256         7

#define DMAC_FLOWCTRL_M2M			0
#define DMAC_FLOWCTRL_M2P			1
#define DMAC_FLOWCTRL_P2M			2
#define DMAC_FLOWCTRL_P2P			3

#endif
