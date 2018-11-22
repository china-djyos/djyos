/*
 * pic_port.c
 */

#include "silan_types.h"
#include "silan_core_m0.h"
#include "silan_irq.h"
#include "silan_errno.h"
#include "string.h"

typedef struct IRQ_Handler
{
    uint32_t    irqid;
    uint32_t    priority;
    void        (*handler)(uint32_t irqid);
}IRQ_HANDLER, *PIRQ_HANDLER;

IRQ_HANDLER irq[32];

// IRQ_HANDLER *irq_p;

int pic_port_request(int id, hdl_t hdl)
{
	IRQ_HANDLER *irq_p;
	int err = 0;

	irq_p = &irq[id];
	memset(irq_p, 0, sizeof(IRQ_HANDLER));
	irq_p->irqid    = (uint32_t)id;
	irq_p->handler  = hdl;
	
	NVIC_ClearPendingIRQ(irq_p->irqid);
	NVIC_SetPriority(irq_p->irqid, irq_p->priority);
	NVIC_EnableIRQ(irq_p->irqid);
	return err;
}

void pic_port_free(int id)
{
	IRQ_HANDLER *irq_p;
	
	irq_p = &irq[id];
	NVIC_ClearPendingIRQ(irq_p->irqid);
	NVIC_DisableIRQ(irq_p->irqid);
	
	irq_p = &irq[id];
	memset(irq_p, 0, sizeof(IRQ_HANDLER));
}

int pic_port_init(void)
{
	int i;
	memset(irq, 0, sizeof(irq));
	
	for(i=0; i<32; i++) {
		irq[i].priority = 0;
	}
}

__STATIC_INLINE uint32_t NVIC_GetEnable(void)
{
	return((uint32_t)(NVIC->ISER[0U]));
}

__STATIC_INLINE void NVIC_SetEnable(uint32_t Restored)
{
	NVIC->ISER[0U] = Restored;
}

__STATIC_INLINE void NVIC_ClrEnable(uint32_t Restored)
{
	NVIC->ICER[0U] = Restored;
}

uint32_t pic_get_enable(void)
{
	return NVIC_GetEnable();
}

void pic_set_enable(uint32_t restored)
{
	NVIC_SetEnable(restored);
}

void pic_clr_enable(uint32_t restored)
{
	NVIC_ClrEnable(restored);
}

void do_handler(IRQ_HANDLER *irq_p)
{
	NVIC_ClearPendingIRQ(irq_p->irqid);
	if(irq_p->handler != NULL)
		irq_p->handler(irq_p->irqid);
}

void SARADC_Handler()
{
	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_SAR];
	
	do_handler(irq_p);
}
void SPIFLASH_Handler()
{
	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_SPIFLASH];
	
	do_handler(irq_p);
}
void UART_Handler()
{
	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_MISC];
	
	do_handler(irq_p);
}
void PDM_Handler()
{
	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_PDM];
	
	do_handler(irq_p);
}

#ifndef IRQ_USER_GPIO1
void GPIO1_Handler()
{
	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_GPIO1];

	do_handler(irq_p);
}
#endif

void GPIO2_Handler()
{
	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_GPIO2];

	do_handler(irq_p);
}

void TIMER_Handler()
{
	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_TIMER];
	
	do_handler(irq_p);
}
void SDMAC_Handler()
{
	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_SDMAC];

	do_handler(irq_p);
}

void IIR_Handler()
{
	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_IIR];

	do_handler(irq_p);
}

//void SDMMC_Handler()
//{
//	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_SDMMC];

//	do_handler(irq_p);
//}
//void SDIO_Handler()
//{
//	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_SDIO];

//	do_handler(irq_p);
//}

//#ifndef IRQ_USER_USBFS
//void USB_OTG_FS_ISR_Handler()
//{
//	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_USBFS];

//	do_handler(irq_p);
//}
//#endif

//#ifndef IRQ_USER_USBHS
//void USB_OTG_HS_ISR_Handler()
//{
//	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_USBHS];

//	do_handler(irq_p);
//}
//#endif

void CXC_Handler()
{
	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_CXC];

	do_handler(irq_p);
}
void ADMAC_Handler()
{
	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_ADMAC];

	do_handler(irq_p);
}
void SPDIF_Handler()
{
	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_SPDIF];

	do_handler(irq_p);
}
void PCM_Handler()
{
	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_PCM];

	do_handler(irq_p);
}
void IIS_Handler()
{
	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_I2S];

	do_handler(irq_p);
}
void RTC_Handler()
{
	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_RTC];

	do_handler(irq_p);
}
void PMU_Handler()
{
	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_PMU];

	do_handler(irq_p);
}
void DMAC_Handler()
{
//	IRQ_HANDLER *irq_p = &irq[PIC_IRQID_DMAC];
//
//	do_handler(irq_p);
}
void HardFault_Handler()
{
}

