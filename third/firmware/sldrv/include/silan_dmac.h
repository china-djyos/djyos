/*
 * silan_dmac.h
 */

#ifndef __SILAN_DMAC_H__
#define __SILAN_DMAC_H__

#include "silan_types.h"
#include "silan_dmac_regs.h"

#define ADMAC					0
#define SDMAC					1

#define DMAC_MAX_CHANNELS		8

#define ADMAC_MAX_CHANNELS		8
#define SDMAC_MAX_CHANNELS		4

#define ALL_DMAC_CHANNELS		(ADMAC_MAX_CHANNELS+SDMAC_MAX_CHANNELS)

typedef struct DMA_raw_lli
{
	u32 srcaddr;
	u32 dstaddr;
	u32 nextlli;
	u32 TransferCtrl;
}
DMA_RAW_LLI, *PDMA_RAW_LLI;

typedef struct DMA_lli
{
	DMA_RAW_LLI raw;
	u32			num_xfer;
	void		*psrcaddr;
	void 		*pdstaddr;
	struct DMA_lli		*pnextlli;
}
DMA_LLI, *PDMA_LLI;

typedef struct DMA_request
{
	DMA_LLI		*dma_lli;
	void		(*callback)(struct DMA_request *);
	int			result;
	u32			lli_num;
	u8			ch;
	u8			dma_dir;
	u8			width;
	u8			burst;

	void		*pdev;
	void		*buf_t;
	DMA_LLI		*done_lli;
	int			buf_pre_io_bytes;
	u8			srcid;
	u8			which_dmac;
	u32			lli_size;
}
DMA_REQUEST, *PDMA_REQUEST;

typedef struct DMA_ch
{
	u8			active;
	DMA_REQUEST *request;
	void		(*callback)(DMA_REQUEST *);
	//void		*pdev;
}
DMA_CH, *PDMA_CH;

void silan_dmac_init(void);

int silan_request_dma_transfer(DMA_REQUEST *req);
DMA_LLI *silan_dmac_get_next_lli(DMA_REQUEST *req);
int silan_dmac_get_perid(int id, char *perid);
int silan_dmac_request_transfer(DMA_REQUEST *req);
int silan_dmac_request_start(DMA_REQUEST *req);
void silan_dmac_req_matrix_init(void);
int silan_dmac_get_ch(u8 which_dmac);
int silan_dmac_release_ch(u8 which_dmac, u8 ch);
int silan_dmac_request_stop(DMA_REQUEST *req);
int silan_dmac_release_transfer(DMA_REQUEST *req);

int *silan_dmac_remap(int *addr);

#endif
