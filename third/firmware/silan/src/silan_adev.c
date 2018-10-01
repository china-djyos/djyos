#define MODULE_NAME		"ADEV"
//#define __DEBUG__

#include "stdlib.h"
#include "string.h"
#include "silan_printf.h"
#include "silan_errno.h"
#include "silan_buffer.h"
#include "silan_adev.h"
#include "silan_syscfg.h"
#include "silan_audio_clkgen.h"

#if defined(__XCC__)

#ifdef ADEV_NOT_USE_SWAP
#if (ADEV_MAX_LLI_SIZE%64)
#error DMAC_MAX_LLI_SIZE must 64B aligned!
#endif

#if (ADEV_MAX_DMAC_CHNL>ADMAC_MAX_CHANNELS)
#error ADEV_MAX_DMAC_CHNL bigger than ADMAC Max CH!
#endif

static int			__buf[ADEV_MAX_DMAC_CHNL][ADEV_MAX_LLI_SIZE*ADEV_LLI_NUM/sizeof(int)]	__attribute__ ((aligned(64)));
static DMA_LLI		__dma_lli[ADEV_MAX_DMAC_CHNL + 1][ADEV_LLI_NUM]					__attribute__ ((aligned(64)));
#else
static int			__buf[ADEV_MAX_DMAC_CHNL][ADEV_MAX_LLI_SIZE*ADEV_LLI_NUM/sizeof(int)]	__attribute__ ((section(".dsp_dram")));
static DMA_LLI		__dma_lli[ADEV_MAX_DMAC_CHNL][ADEV_LLI_NUM]						__attribute__ ((section(".dsp_dram")));
#endif

#endif

#if defined(__CC_ARM)
static int			__buf[ADEV_MAX_DMAC_CHNL][ADEV_MAX_LLI_SIZE*ADEV_LLI_NUM/sizeof(int)];
static DMA_LLI		__dma_lli[ADEV_MAX_DMAC_CHNL][ADEV_LLI_NUM];
#endif

#if defined(__CSKY__)
static int			__buf[ADEV_MAX_DMAC_CHNL][ADEV_MAX_LLI_SIZE*ADEV_LLI_NUM/sizeof(int)]	__attribute__ ((aligned(16)));
static DMA_LLI		__dma_lli[ADEV_MAX_DMAC_CHNL + 1][ADEV_LLI_NUM]					__attribute__ ((aligned(16)));
#endif

static DMA_REQUEST	__dma_req[ADEV_MAX_DMAC_CHNL];
static buf_t 		__buf_t[ADEV_MAX_DMAC_CHNL];

static uint8_t		__adev_req[ADEV_MAX_DMAC_CHNL];

static int			*__zero = (int *)(SRAM1_ADDR_BASE + 0x00000);
static int			*__wbuf = (int *)(SRAM1_ADDR_BASE + 0x10000);

static ADEV_I2S_HANDLE __adev_handler[I2S_ID_LMT];
static uint32_t        __adev_handler_index = 0;

static int32_t silan_adev_get_req_num(void)
{
	int i;
	
	for(i=0;i<ADEV_MAX_DMAC_CHNL;i++) {
		if(__adev_req[i] == 0) {
			__adev_req[i] = 1;
			return i;
		}
	}
	SL_LOG("No more Space");
	return -ENOSPC_S;
}

static int32_t silan_adev_release_req_num(uint8_t req_num)
{
	if(__adev_req[req_num] == 1) {
		__adev_req[req_num] = 0;
		return ENONE_S;
	}
	
	SL_LOG("No Such Sum");
	return -EINVAL_S;
}

static void silan_dmac_i2s_callback(DMA_REQUEST *req)
{
	// int i;
	int *data, bytes, offset, lli_size;
	// uint8_t which_dmac = req->which_dmac;
	// uint8_t ch = req->ch;
	uint8_t dma_dir = req->dma_dir;
	
	DMA_LLI *next_lli, *done_lli;
	next_lli = silan_dmac_get_next_lli(req);
	done_lli = req->done_lli;
	buf_t *buf = req->buf_t;
	lli_size = req->lli_size;
	//sl_printf("*");

	if (buf) {
		if(dma_dir == DMAC_DIR_M2P) {
#if 0
			/* FIXME, try to fix dma irq lost */
			while (done_lli->pnextlli->pnextlli != next_lli) {
				if (done_lli->psrcaddr != __zero) {
					SL_ASSERT(done_lli->psrcaddr == buf->rd);
					SL_ASSERT(bytes == lli_size);
				}
				done_lli = done_lli->pnextlli;
				req->done_lli = done_lli;
			}
#endif	
			if (done_lli->psrcaddr != __zero) {
				SL_ASSERT(done_lli->psrcaddr == buf->rd);
				bytes = buf_read(buf, lli_size);
				SL_ASSERT(bytes == lli_size);
				req->buf_pre_io_bytes -= lli_size;
	
				done_lli->psrcaddr     = (void *)__zero;
				done_lli->raw.srcaddr = (uint32_t)ptr_cpu_to_dma((uint32_t *)__zero);
				dcache_writeback(done_lli, sizeof(*done_lli));
			}

			offset = req->buf_pre_io_bytes;
			bytes = buf_pre_read(buf, (char **)&data, offset, lli_size);
			SL_ASSERT((bytes == 0) || (bytes == lli_size));
			if (bytes == lli_size) {
				while (next_lli->pnextlli != done_lli->pnextlli) {
					if (next_lli->psrcaddr == __zero) {
						next_lli->psrcaddr     = (void *)data;
						next_lli->raw.srcaddr  = (uint32_t)ptr_cpu_to_dma((uint32_t *)data);
						req->buf_pre_io_bytes += lli_size;
						dcache_writeback(next_lli, sizeof(*next_lli));
						break;
					}
					next_lli = next_lli->pnextlli;
				}
			}
			else {
				//sl_printf("#");
			}
			req->done_lli = done_lli->pnextlli;
		}
		else if(dma_dir == DMAC_DIR_P2M) {
#if 0
			/* FIXME, check if dma irq is lost. */
			while (done_lli->pnextlli->pnextlli != next_lli) {
				if (done_lli->pdstaddr != __wbuf) {
					SL_ASSERT(done_lli->pdstaddr == buf->wt);
					SL_ASSERT(bytes == lli_size);
				}
				done_lli = done_lli->pnextlli;
				req->done_lli = done_lli;
			}
#endif
			if (done_lli->pdstaddr != __wbuf) {
				SL_ASSERT(done_lli->pdstaddr == buf->wt);
				bytes = buf_write(buf, lli_size);
				SL_ASSERT(bytes == lli_size);
				req->buf_pre_io_bytes -= lli_size;
			}
	
			offset = req->buf_pre_io_bytes;
			bytes  = buf_pre_write(buf, (char **)&data, offset, lli_size);
	
			SL_ASSERT((bytes == 0) || (bytes == lli_size));
			if (bytes == lli_size) {
				req->buf_pre_io_bytes += lli_size;
			} else {
				data = (int *)__wbuf;
			}
			next_lli->pdstaddr	  = (void *)data;
			next_lli->raw.dstaddr = (uint32_t)ptr_cpu_to_dma((uint32_t *)data);
			dcache_writeback(next_lli, sizeof(*next_lli));

			req->done_lli = done_lli->pnextlli;
		} 
	}
}

int32_t silan_adev_i2s_set_rate(ADEV_I2S_HANDLE *pdev, int rate)
{
	uint8_t codec_id, tr;

	codec_id = pdev->i2s_cfg.codec;
	tr = pdev->i2s_cfg.tr;

	pdev->adev_cfg.sample_rate = rate;
	silan_i2s_clock_config(pdev->i2s_cfg.id, rate);

	if(codec_id & CODEC_ID_1) {
		if(tr & I2S_TR_TO)
			silan_codec_dac_config(CODEC1, rate);
		if(tr & I2S_TR_RO)
			silan_codec_adc_config(CODEC1, rate);
	}
	if(codec_id & CODEC_ID_2) {
		if(tr & I2S_TR_TO)
			silan_codec_dac_config(CODEC2, rate);
		if(tr & I2S_TR_RO)
			silan_codec_adc_config(CODEC2, rate);
	}
	
	return ENONE_S;
}

int32_t silan_adev_i2s_set_gain(ADEV_I2S_HANDLE *pdev, int32_t l_gain, int32_t r_gain)
{
	uint8_t codec_id, tr;
	int32_t codec_l, codec_r;

	codec_id = pdev->i2s_cfg.codec;
	tr = pdev->i2s_cfg.tr;

	if(tr == I2S_TR_TO) {
		if((l_gain < CODEC_DAC_GAIN_MIN) && (r_gain < CODEC_DAC_GAIN_MIN)) {
			if(codec_id & CODEC_ID_1)
				silan_codec_dac_mute(CODEC1);
			if(codec_id & CODEC_ID_2)
				silan_codec_dac_mute(CODEC2);
		}
		else {
			codec_l = (l_gain - CODEC_DAC_GAIN_MIN)/CODEC_DAC_GAIN_STEP;
			codec_r = (r_gain - CODEC_DAC_GAIN_MIN)/CODEC_DAC_GAIN_STEP;
			if(codec_id & CODEC_ID_1) {
				silan_codec_dac_set_gain(CODEC1, codec_l, codec_r);
				silan_codec_dac_unmute(CODEC1);
			}
			if(codec_id & CODEC_ID_2) {
				silan_codec_dac_set_gain(CODEC2, codec_l, codec_r);
				silan_codec_dac_unmute(CODEC2);
			}
		}
	}
	
	if(tr == I2S_TR_RO) {
		codec_l = (l_gain - CODEC_ADC_GAIN_MIN)/CODEC_ADC_GAIN_STEP;
		codec_r = (r_gain - CODEC_ADC_GAIN_MIN)/CODEC_ADC_GAIN_STEP;
		if(codec_id & CODEC_ID_1) {
			silan_codec_adc_set_gain(CODEC1, codec_l, codec_r);
		}
		if(codec_id & CODEC_ID_2) {
			silan_codec_adc_set_gain(CODEC2, codec_l, codec_r);
		}
	}
	
	return ENONE_S;
}

int32_t silan_adev_i2s_submit(ADEV_I2S_HANDLE *pdev)
{
	int32_t req_num;
	uint32_t *src, *dst, lli_size, width_calib, sample_per_1ms, ch_calib;
	uint8_t which_dmac, perid;
	int8_t rx_ch, tx_ch;
	DMA_REQUEST *tx_pdma_req, *rx_pdma_req;
	DMA_LLI *tx_pdma_lli, *rx_pdma_lli;
	ADEV_CFG *padev_cfg;
	I2S_CFG *pi2s_cfg;
	buf_t *rx_pbuf_t, *tx_pbuf_t;
	int i;

	padev_cfg = &pdev->adev_cfg;
	pi2s_cfg = &pdev->i2s_cfg;

	sample_per_1ms = padev_cfg->sample_rate/1000;
	width_calib = (padev_cfg->width == 16) ? 16 : 32;
	ch_calib = (pi2s_cfg->ch+1)*2;
	lli_size = sample_per_1ms * width_calib / 8 * ch_calib;
	SL_LOG("I2S ID-%d LLI_SIZE: %d", pi2s_cfg->id, lli_size);
	
	if(pi2s_cfg->tr & I2S_TR_TO) {
		req_num = silan_adev_get_req_num();
		if(req_num < 0) {
			SL_LOG("No More Space.");
			return req_num;
		}
		padev_cfg->tx_num = req_num;
		
		tx_pdma_req = &__dma_req[req_num];
		tx_pdma_lli = &__dma_lli[req_num][0];
		tx_pbuf_t   = &__buf_t[req_num];
		memset(tx_pdma_req,		0, sizeof(DMA_REQUEST));
		memset(tx_pdma_lli,		0, sizeof(DMA_LLI)*ADEV_LLI_NUM);
		//dcache_writeback(tx_pdma_lli, sizeof(DMA_LLI)*ADEV_LLI_NUM);
		
		memset(tx_pbuf_t,		0, sizeof(buf_t));
		
		buf_init(tx_pbuf_t, (char *)&__buf[req_num][0], lli_size*ADEV_LLI_NUM);

		which_dmac = silan_dmac_get_perid(tx_pdma_req->srcid, &perid);
		tx_ch = silan_dmac_get_ch(which_dmac);
		if(tx_ch < 0) {
			SL_LOG("No DMAC CH.");
			return tx_ch;
		}

		tx_pdma_req->lli_size = lli_size;
		tx_pdma_req->pdev = (void *)pdev;
		tx_pdma_req->callback = (void *)silan_dmac_i2s_callback;
		tx_pdma_req->dma_lli = tx_pdma_lli;
		tx_pdma_req->burst = padev_cfg->burst;
		tx_pdma_req->result = -1;
		tx_pdma_req->lli_num = ADEV_LLI_NUM;
		tx_pdma_req->ch = tx_ch;
		tx_pdma_req->width = (padev_cfg->width == 24) ? DMAC_WIDTH_32BIT : DMAC_WIDTH_16BIT;
		tx_pdma_req->buf_t = (void *)tx_pbuf_t;
		tx_pdma_req->dma_dir = DMAC_DIR_M2P;
		tx_pdma_req->srcid = pi2s_cfg->tx_srcid;
		tx_pdma_req->done_lli = tx_pdma_lli;
		tx_pdma_req->buf_pre_io_bytes = 0;
		src = (void *)__zero;
		dst = (void *)&pi2s_cfg->regs->I2S_CHNL[0].TFR;
		tx_pdma_req->which_dmac = which_dmac;
		
		for (i = 0; i < ADEV_LLI_NUM; i++) {
			tx_pdma_lli[i].psrcaddr	 = (void *)src;
			tx_pdma_lli[i].pdstaddr	 = (void *)dst;
			tx_pdma_lli[i].pnextlli	 = (void *)&tx_pdma_lli[i+1];
			tx_pdma_lli[i].num_xfer	 = lli_size * 8 / width_calib;
			memset(&tx_pdma_lli[i].raw, 0, sizeof(DMA_RAW_LLI));
		}
		tx_pdma_lli[ADEV_LLI_NUM-1].pnextlli = (void *)&tx_pdma_lli[0];
	
		pdev->adev_cfg.tx_dma_req = tx_pdma_req;
		SL_LOG("TX REQ Submit: %d", req_num);
		silan_dmac_request_transfer(tx_pdma_req);
	}
	if(pi2s_cfg->tr & I2S_TR_RO) {
		req_num = silan_adev_get_req_num();
		if(req_num < 0) {
			SL_LOG("No More Space.");
			return req_num;
		}
		padev_cfg->rx_num = req_num;
		
		rx_pdma_req = &__dma_req[req_num];
		rx_pdma_lli = &__dma_lli[req_num][0];
		rx_pbuf_t   = &__buf_t[req_num];
		memset(rx_pdma_req,		0, sizeof(DMA_REQUEST));
		memset(rx_pdma_lli,		0, sizeof(DMA_LLI)*ADEV_LLI_NUM);
		//dcache_writeback(rx_pdma_lli, sizeof(DMA_LLI)*ADEV_LLI_NUM);

		memset(rx_pbuf_t,		0, sizeof(buf_t));
		
		buf_init(rx_pbuf_t, (char *)&__buf[req_num][0], lli_size*ADEV_LLI_NUM);

		which_dmac = silan_dmac_get_perid(rx_pdma_req->srcid, &perid);
		rx_ch = silan_dmac_get_ch(which_dmac);
		if(rx_ch < 0) {
			SL_LOG("No DMAC CH.");
			return rx_ch;
		}
		
		rx_pdma_req->lli_size = lli_size;
		rx_pdma_req->pdev = (void *)pdev;
		rx_pdma_req->callback = (void *)silan_dmac_i2s_callback;
		rx_pdma_req->dma_lli = rx_pdma_lli;
		rx_pdma_req->burst = padev_cfg->burst;
		rx_pdma_req->result = -1;
		rx_pdma_req->lli_num = ADEV_LLI_NUM;
		rx_pdma_req->ch = rx_ch;
		rx_pdma_req->width = (padev_cfg->width == 24) ? DMAC_WIDTH_32BIT : DMAC_WIDTH_16BIT;
		rx_pdma_req->buf_t = rx_pbuf_t;
		rx_pdma_req->srcid = pi2s_cfg->rx_srcid;
		rx_pdma_req->done_lli = rx_pdma_lli;
		rx_pdma_req->buf_pre_io_bytes = 0;
		rx_pdma_req->dma_dir = DMAC_DIR_P2M;
		src = (void *)&pi2s_cfg->regs->I2S_CHNL[0].RFR;
		dst = (void *)__wbuf;
		which_dmac = silan_dmac_get_perid(rx_pdma_req->srcid, &perid);
		rx_pdma_req->which_dmac = which_dmac;
	
		for (i = 0; i < ADEV_LLI_NUM; i++) {
			rx_pdma_lli[i].psrcaddr	 = (void *)src;
			rx_pdma_lli[i].pdstaddr	 = (void *)dst;
			rx_pdma_lli[i].pnextlli	 = (void *)&rx_pdma_lli[i+1];
			rx_pdma_lli[i].num_xfer	 = lli_size * 8 / width_calib;
			memset(&rx_pdma_lli[i].raw, 0, sizeof(DMA_RAW_LLI));
		}
		rx_pdma_lli[ADEV_LLI_NUM-1].pnextlli = (void *)&rx_pdma_lli[0];
	
		pdev->adev_cfg.rx_dma_req = rx_pdma_req;
		SL_LOG("RX REQ Submit: %d", req_num);
		silan_dmac_request_transfer(rx_pdma_req);
	}

	return lli_size;
}

int32_t silan_adev_i2s_revoke(ADEV_I2S_HANDLE *pdev)
{
	ADEV_CFG *padev_cfg = &pdev->adev_cfg;
	if(padev_cfg->tx_dma_req) {
		silan_adev_release_req_num(padev_cfg->tx_num);
		silan_dmac_release_transfer(padev_cfg->tx_dma_req);
	}

	if(padev_cfg->rx_dma_req) {
		silan_adev_release_req_num(padev_cfg->rx_num);
		silan_dmac_release_transfer(padev_cfg->rx_dma_req);
	}
	
	return ENONE_S;
}

int32_t silan_adev_i2s_start(ADEV_I2S_HANDLE *pdev)
{
	uint8_t tr = pdev->i2s_cfg.tr;
	if(tr & I2S_TR_TO)
		silan_dmac_request_start(pdev->adev_cfg.tx_dma_req);
	if(tr & I2S_TR_RO)
		silan_dmac_request_start(pdev->adev_cfg.rx_dma_req);
	silan_i2s_start(&pdev->i2s_cfg);
	
	return ENONE_S;
}

int32_t silan_adev_i2s_stop(ADEV_I2S_HANDLE *pdev)
{
	uint8_t tr = pdev->i2s_cfg.tr;
	if(tr & I2S_TR_TO)
		silan_dmac_request_stop(pdev->adev_cfg.tx_dma_req);
	if(tr & I2S_TR_RO)
		silan_dmac_request_stop(pdev->adev_cfg.rx_dma_req);
	silan_i2s_stop(&pdev->i2s_cfg);
	
	return ENONE_S;
}

int32_t silan_adev_i2s_open(ADEV_I2S_HANDLE *pdev)
{
	uint8_t codec_id, tr, burst_calib;
	CODEC_CFG *pcodec;
	ADEV_CFG *padev_cfg;
	I2S_CFG *pi2s_cfg;
	int32_t ret;
	
	pcodec = &pdev->codec_cfg;
	padev_cfg = &pdev->adev_cfg;
	pi2s_cfg = &pdev->i2s_cfg;

	// On-chip CODEC1&CODEC2, fix master and slave mode
	if(pi2s_cfg->codec > CODEC_ID_EXT) {
		if((pi2s_cfg->id == I2S_ID_O1) || (pi2s_cfg->id == I2S_ID_O2))
			pi2s_cfg->master = CLK_MASTER;
		else if((pi2s_cfg->id == I2S_ID_I1) || (pi2s_cfg->id == I2S_ID_I2))
			pi2s_cfg->master = CLK_SLAVE;
	}

	// Default Config
	padev_cfg->burst = DMAC_BURST_SIZE_4;
	burst_calib = padev_cfg->burst + 1;
	if(burst_calib == 1)
		burst_calib = 0;
	pi2s_cfg->burst = (1 << burst_calib);
	pi2s_cfg->wlen = (padev_cfg->width == 16) ? I2S_WLEN_16 :
												I2S_WLEN_24;

	ret = silan_i2s_route_config(pi2s_cfg->id, pi2s_cfg->codec, pi2s_cfg->master);
	if(ret != 0) {
		SL_LOG("I2S and CODEC config Error");
		return ret;
	}
	
	ret = silan_i2s_dev_open(pi2s_cfg);
	if(ret != 0) {
		SL_LOG("No I2S Device");
		return ret;
	}

	SL_LOG("I2S Device: ID=%d", pi2s_cfg->id);
	SL_DBGLOG("  WIDTH: %d", padev_cfg->width);
	SL_DBGLOG("  CH:    %d", (pi2s_cfg->ch+1) << 1);
	SL_DBGLOG("  CODEC: %d", pi2s_cfg->codec);
	
	tr = pi2s_cfg->tr;
	codec_id = (pdev->i2s_cfg.codec);

	if(codec_id & CODEC_ID_1) {
		if(silan_codec_check(CODEC1) == 0)
			silan_codec_open(CODEC1);
		if(tr & I2S_TR_TO) {
			pcodec->dac_wl = (padev_cfg->width == 16) ? CODEC_WL_16 :
														CODEC_WL_24;
			pcodec->dac_master = CLK_SLAVE;
			silan_codec_dac_init(CODEC1, pcodec);
		}
		if(tr & I2S_TR_RO) {
			pcodec->adc_wl = (padev_cfg->width == 16) ? CODEC_WL_16 :
														CODEC_WL_24;
			pcodec->adc_master = CLK_MASTER;
			silan_codec_adc_init(CODEC1, pcodec);
		}
	}
	if(codec_id & CODEC_ID_2) {
		if(silan_codec_check(CODEC2) == 0)
			silan_codec_open(CODEC2);
		if(tr & I2S_TR_TO) {
			pcodec->dac_wl = (padev_cfg->width == 16) ? CODEC_WL_16 :
														CODEC_WL_24;
			pcodec->dac_master = CLK_SLAVE;
			silan_codec_dac_init(CODEC2, pcodec);
		}
		if(tr & I2S_TR_RO) {
			pcodec->adc_wl = (padev_cfg->width == 16) ? CODEC_WL_16 :
														CODEC_WL_24;
			pcodec->adc_master = CLK_MASTER;
			silan_codec_adc_init(CODEC2, pcodec);
		}
	}
	
	return ENONE_S;
}

ADEV_I2S_HANDLE *silan_adev_i2s_alloc(I2S_ID i2s)
{
	ADEV_I2S_HANDLE *pdev;
	if(__adev_handler_index > (I2S_ID_LMT-1)) {
		SL_LOG("ADEV I2S alloc fail.");
		return NULL;
	}
	pdev = &__adev_handler[__adev_handler_index];
	memset(pdev, 0, sizeof(ADEV_I2S_HANDLE));
	pdev->i2s_cfg.id = i2s;

	__adev_handler_index++;
	return pdev;
}

int32_t silan_adev_pre_write(void *pdev, char **write_p, int bytes)
{
	ADEV_CFG *adev_cfg = (ADEV_CFG *)pdev;

	return buf_pre_write(adev_cfg->tx_dma_req->buf_t, write_p, 0, bytes);
}

int32_t silan_adev_write(void *pdev, int bytes)
{
	ADEV_CFG *adev_cfg = (ADEV_CFG *)pdev;
	
	return buf_write(adev_cfg->tx_dma_req->buf_t, bytes);
}

int32_t silan_adev_pre_read(void *pdev, char **read_p, int bytes)
{
	ADEV_CFG *adev_cfg = (ADEV_CFG *)pdev;

	return buf_pre_read(adev_cfg->rx_dma_req->buf_t, read_p, 0, bytes);
}

int32_t silan_adev_read(void *pdev, int bytes)
{
	ADEV_CFG *adev_cfg = (ADEV_CFG *)pdev;
	
	return buf_read(adev_cfg->rx_dma_req->buf_t, bytes);
}

void silan_adev_init(void)
{
	memset(&__dma_req, 0, ADEV_MAX_DMAC_CHNL*sizeof(DMA_REQUEST));
	memset(&__dma_lli, 0, ADEV_MAX_DMAC_CHNL*sizeof(DMA_LLI)*ADEV_LLI_NUM);
	memset(&__buf_t,   0, ADEV_MAX_DMAC_CHNL*sizeof(buf_t));
	memset(&__buf,     0, sizeof(__buf));
	memset(&__adev_req,0, sizeof(__adev_req));
}

