/*
 * silan_pic.c
 */

#define MODULE_NAME            "PIC"
//#define __DEBUG__

#include "silan_irq.h"
#include "silan_pic_regs.h"
#include "silan_types.h"
#include "silan_printf.h"
#include "silan_errno.h"
#include "string.h"

#include "int_hard.h"

#if defined(__XCC__)
#include "xtensa/xos.h"

#ifdef _SUPPORT_XOS_
extern XosThread thread_audio;
#endif
#endif

#define SPDIF_HDL_NUM      2
static hdl_t spdif_hdl[SPDIF_HDL_NUM];

#define DMAC_HDL_NUM       8
static hdl_t sdmac_hdl[DMAC_HDL_NUM];
static hdl_t admac_hdl[DMAC_HDL_NUM];

#define GPIO_HDL_NUM       32
static hdl_t gpio1_hdl[GPIO_HDL_NUM];
static hdl_t gpio2_hdl[GPIO_HDL_NUM];

#define TIMER_HDL_NUM      7
static hdl_t timer_hdl[TIMER_HDL_NUM];

#define MISC_HDL_NUM       10
static hdl_t misc_hdl[MISC_HDL_NUM];

#define PMU_HDL_NUM        11
static hdl_t pmu_hdl[PMU_HDL_NUM];

void pic_do_hdl(uint32_t stat, hdl_t *hdl)
{
	int subid = 0;
	
	while (stat) {
		if (stat & 0x1) {
			if(hdl[subid])
				(*hdl[subid])(subid);
		}
		stat >>= 1;
		subid++;
	}
}

 void pic_hdl_spdif(uint32_t irqid)
{
	uint32_t stat;

	stat = INTR_STS_MSK(PIC_IRQID_SPDIF);
	pic_do_hdl(stat, spdif_hdl);
}

 void pic_hdl_sdmac(uint32_t irqid)
{
	uint32_t stat;

	stat = INTR_STS_MSK(PIC_IRQID_SDMAC);
	pic_do_hdl(stat, sdmac_hdl);
}

 void pic_hdl_admac(uint32_t irqid)
{
	uint32_t stat;

	stat = INTR_STS_MSK(PIC_IRQID_ADMAC);
	pic_do_hdl(stat, admac_hdl);
#if defined(__XCC__)
#ifdef _SUPPORT_XOS_
	xos_thread_wake(&thread_audio, NULL, 0);
#endif
#endif
}

 void pic_hdl_gpio1(uint32_t irqid)
{
	uint32_t stat;

	stat = INTR_STS_MSK(PIC_IRQID_GPIO1);
	pic_do_hdl(stat, gpio1_hdl);
}

 void pic_hdl_gpio2(uint32_t irqid)
{
	uint32_t stat;

	stat = INTR_STS_MSK(PIC_IRQID_GPIO2);
	pic_do_hdl(stat, gpio2_hdl);
}

 void pic_hdl_misc(uint32_t irqid)
{
	uint32_t stat;

	stat = INTR_STS_MSK(PIC_IRQID_MISC);
	pic_do_hdl(stat, misc_hdl);
}

 void pic_hdl_pmu(uint32_t irqid)
{
	uint32_t stat;

	stat = INTR_STS_MSK(PIC_IRQID_PMU);
	pic_do_hdl(stat, pmu_hdl);
}

 void pic_hdl_timer(uint32_t irqid)
{
	uint32_t stat;

	stat = INTR_STS_MSK(PIC_IRQID_TIMER);
	pic_do_hdl(stat, timer_hdl);
}

int silan_pic_request(pic_irq_t id, int subid, hdl_t hdl)
{
	int err = 0;

	SL_DBGLOG("Request irq. (id=%d, subid=%d, hdl=%p)", id, subid, hdl);

	if ((id < 0) || (id >= PIC_IRQID_LMT) || (!hdl)) {
		SL_DBGLOG("request irq failed.");
		return -EINVAL_S;
	}

	INTR_CTR_MSK(id) &= ~(1 << (subid));/* 1 */

	if ((id == PIC_IRQID_SPDIF) && (subid != PIC_SUBID_SOLO)) {
		if (subid >= SPDIF_HDL_NUM) {
			SL_TRACE();
			return -EINVAL_S;
		}

		SL_LOG("SPDIF subid = %d", subid);
		SL_LOG("hdl         = %p", hdl);

		if (!spdif_hdl[subid]) {
			spdif_hdl[subid] = hdl;
			INTR_CTR_MSK(id) |= (1 << (subid));	/* 2 */
		}
		else if (spdif_hdl[subid] != hdl) {
			SL_DBGLOG("IRQID(%d) is registered.");
			return -EBUSY_S;
		}
	}
	else if ((id == PIC_IRQID_SDMAC) && (subid != PIC_SUBID_SOLO)) {
		if (subid >= DMAC_HDL_NUM) {
			SL_TRACE();
			return -EINVAL_S;
		}

		SL_LOG("SDMAC subid = %d", subid);
		SL_LOG("hdl         = %p", hdl);

		if (!sdmac_hdl[subid]) {
			sdmac_hdl[subid] = hdl;
			INTR_CTR_MSK(id) |= (1 << (subid));
		}
		else if (sdmac_hdl[subid] != hdl) {
			SL_DBGLOG("IRQID(%d) is registered.");
			return -EBUSY_S;
		}
	}
	else if ((id == PIC_IRQID_ADMAC) && (subid != PIC_SUBID_SOLO)) {
		if (subid >= DMAC_HDL_NUM) {
			SL_TRACE();
			return -EINVAL_S;
		}

		SL_LOG("ADMAC subid = %d", subid);
		SL_LOG("hdl         = %p", hdl);

		if (!admac_hdl[subid]) {
			admac_hdl[subid] = hdl;
			INTR_CTR_MSK(id) |= (1 << (subid));
		}
		else if (admac_hdl[subid] != hdl) {
			SL_DBGLOG("IRQID(%d) is registered.");
			return -EBUSY_S;
		}
	}
	else if ((id == PIC_IRQID_GPIO1) && (subid != PIC_SUBID_SOLO)) {
		if (subid >= GPIO_HDL_NUM) {
			SL_TRACE();
			return -EINVAL_S;
		}

		SL_LOG("GPIO1 subid = %d", subid);
		SL_LOG("hdl         = %p", hdl);

		if (!gpio1_hdl[subid]) {
			gpio1_hdl[subid] = hdl;
			INTR_CTR_MSK(id) |= (1 << (subid));
		}
		else if (gpio1_hdl[subid] != hdl) {
			SL_DBGLOG("IRQID(%d) is registered.");
			return -EBUSY_S;
		}
	}
	else if ((id == PIC_IRQID_GPIO2) && (subid != PIC_SUBID_SOLO)) {
		if (subid >= GPIO_HDL_NUM) {
			SL_TRACE();
			return -EINVAL_S;
		}

		SL_LOG("GPIO1 subid = %d", subid);
		SL_LOG("hdl         = %p", hdl);

		if (!gpio2_hdl[subid]) {
			gpio2_hdl[subid] = hdl;
			INTR_CTR_MSK(id) |= (1 << (subid));
		}
		else if (gpio2_hdl[subid] != hdl) {
			SL_DBGLOG("IRQID(%d) is registered.");
			return -EBUSY_S;
		}
	}
	else if ((id == PIC_IRQID_TIMER) && (subid != PIC_SUBID_SOLO)) {
		if (subid >= TIMER_HDL_NUM) {
			SL_TRACE();
			return -EINVAL_S;
		}

		SL_LOG("TIMER subid = %d", subid);
		SL_LOG("hdl         = %p", hdl);
		if (!timer_hdl[subid]) {
			timer_hdl[subid] = hdl;
			INTR_CTR_MSK(id) |= (1 << (subid));
		} else if (timer_hdl[subid] != hdl) {
			SL_DBGLOG("IRQID(%d) is registered.");
			return -EBUSY_S;
		}
	}
	else if ((id == PIC_IRQID_MISC) && (subid != PIC_SUBID_SOLO)) {
		if (subid >= MISC_HDL_NUM) {
			SL_TRACE();
			return -EINVAL_S;
		}

		SL_LOG("MISC  subid = %d", subid);
		SL_LOG("hdl         = %p", hdl);

		if (!misc_hdl[subid]) {
			misc_hdl[subid]   = hdl;
			INTR_CTR_MSK(id) |= (1 << (subid));
		}
		else if (misc_hdl[subid] != hdl) {
			SL_DBGLOG("IRQID(%d) is registered.");
			return -EBUSY_S;
		}
	}
	else if ((id == PIC_IRQID_PMU) && (subid != PIC_SUBID_SOLO)) {
		if (subid >= PMU_HDL_NUM) {
			SL_TRACE();
			return -EINVAL_S;
		}

		SL_LOG("PMU  subid = %d", subid);
		SL_LOG("hdl         = %p", hdl);

		if (!pmu_hdl[subid]) {
			pmu_hdl[subid]   = hdl;
			INTR_CTR_MSK(id) |= (1 << (subid));
		} else if (pmu_hdl[subid] != hdl) {
			SL_DBGLOG("IRQID(%d) is registered.");
			return -EBUSY_S;
		} 
	}
	else {
		SL_LOG("Normal   id = %d", id);
		SL_LOG("hdl 		= %p", hdl);
		//err = int_pic_port_request(id, hdl);
		if (err) {
			SL_DBGLOG("request irq failed. (pic port, %d)", err);
			return err;
		} 
		// INTR_CTR_MSK(id) |= (1 << (subid));
	}

	SL_DBGLOG("Request irq successful.(id = %d, subid = %d)", id, subid);
	SL_DBGLOG("mask(%p) = 0x%x", &INTR_CTR_MSK(id), INTR_CTR_MSK(id));

	return 0;
}

void silan_pic_free(pic_irq_t id, int subid)
{
	uint32_t mask;

	SL_LOG("Free irq. (id=%d, subid=%d)", id, subid);

	if ((id < 0) || (id >= PIC_IRQID_LMT) || \
			(subid < PIC_SUBID_SOLO) || (subid > PIC_SUBID_LMT)) {
		return;
	}

	mask = INTR_CTR_MSK(id);
	SL_DBGLOG("free id(%d) subid(%d)", id, subid);
	SL_DBGLOG("mask = 0x%x", mask);
	if (mask & (1<<subid)) {
		mask &= (~(1<<subid));
		INTR_CTR_MSK(id) = mask;

		if (id == PIC_IRQID_SPDIF) {
			if (subid >= SPDIF_HDL_NUM)
				return;
			SL_DBGLOG("free SPDIF subid(%d)", subid);
			spdif_hdl[subid] = NULL;
		} else if (id == PIC_IRQID_GPIO1) {
			gpio1_hdl[subid] = NULL;
		} else if (id == PIC_IRQID_GPIO2) {
			gpio2_hdl[subid] = NULL;
		} else if (id == PIC_IRQID_TIMER) {
			timer_hdl[subid] = NULL;
		} else if (id == PIC_IRQID_MISC) {
			misc_hdl[subid]  = NULL;
		} else if (id == PIC_IRQID_ADMAC) {
			admac_hdl[subid]  = NULL;
		} else if (id == PIC_IRQID_SDMAC) {
			sdmac_hdl[subid]  = NULL;
		} else if (id == PIC_IRQID_PMU) {
			pmu_hdl[subid]  = NULL;
		}
	}
	if (!mask) {
		//pic_port_free(id);
	}
	return;
}

uint32_t silan_pic_get_subid(pic_irq_t id)
{
	return INTR_CTR_MSK(id);
}

uint32_t silan_pic_get_subid_stat(pic_irq_t id)
{
	return INTR_STS_MSK(id);
}

void silan_pic_init()
{
	//SL_DBGLOG("Init PIC.");

	/* FIXME, all interrupt should be masked. */
	//pic_port_init();

	memset(spdif_hdl, 0, SPDIF_HDL_NUM * sizeof(hdl_t));
	memset(sdmac_hdl, 0, DMAC_HDL_NUM  * sizeof(hdl_t));
	memset(admac_hdl, 0, DMAC_HDL_NUM  * sizeof(hdl_t));
	memset(gpio1_hdl, 0, GPIO_HDL_NUM  * sizeof(hdl_t));
	memset(gpio2_hdl, 0, GPIO_HDL_NUM  * sizeof(hdl_t));
	memset(timer_hdl, 0, TIMER_HDL_NUM * sizeof(hdl_t));
	memset(misc_hdl,  0, TIMER_HDL_NUM * sizeof(hdl_t));

	// Register second-level interrupt
//	int_pic_port_request(PIC_IRQID_SPDIF, (void *)pic_hdl_spdif);
//	int_pic_port_request(PIC_IRQID_SDMAC, (void *)pic_hdl_sdmac);
//	int_pic_port_request(PIC_IRQID_ADMAC, (void *)pic_hdl_admac);
//	int_pic_port_request(PIC_IRQID_GPIO1, (void *)pic_hdl_gpio1);
//	int_pic_port_request(PIC_IRQID_GPIO2, (void *)pic_hdl_gpio2);
//	int_pic_port_request(PIC_IRQID_MISC,  (void *)pic_hdl_misc);
//	int_pic_port_request(PIC_IRQID_PMU,   (void *)pic_hdl_pmu);
//	int_pic_port_request(PIC_IRQID_TIMER, (void *)pic_hdl_timer);
}
