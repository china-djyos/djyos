#define CONFIG_MX6Q
#define CONFIG_MX6
/*
 * Based on the iomux-v3.c from Linux kernel:
 * Copyright (C) 2008 by Sascha Hauer <kernel@pengutronix.de>
 * Copyright (C) 2009 by Jan Weitzel Phytec Messtechnik GmbH,
 *                       <armlinux@phytec.de>
 *
 * Copyright (C) 2004-2015 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#if 0
#include <common.h>
#include <asm/io.h>
#include <asm/arch/imx-regs.h>
#if !defined(CONFIG_MX25) && !defined(CONFIG_VF610)
#include <asm/arch/sys_proto.h>
#endif
#include <asm/imx-common/iomux-v3.h>
#endif


#include "iomux-v3.h"

/* imx-regs.h */
#ifndef IOMUXC_BASE_ADDR
#define IOMUXC_BASE_ADDR 0x020E0000
#endif

static void *base = (void *)IOMUXC_BASE_ADDR;

/* io.h */

/*
 * Generic virtual read/write.  Note that we don't support half-word
 * read/writes.  We define __arch_*[bl] here, and leave __arch_*w
 * to the architecture specific code.
 */
#define __arch_getb(a)                  (*(volatile unsigned char *)(a))
#define __arch_getw(a)                  (*(volatile unsigned short *)(a))
#define __arch_getl(a)                  (*(volatile unsigned int *)(a))
#define __arch_getq(a)                  (*(volatile unsigned long long *)(a))

#define __arch_putb(v,a)                (*(volatile unsigned char *)(a) = (v))
#define __arch_putw(v,a)                (*(volatile unsigned short *)(a) = (v))
#define __arch_putl(v,a)                (*(volatile unsigned int *)(a) = (v))
#define __arch_putq(v,a)                (*(volatile unsigned long long *)(a) = (v))


#define __raw_writeb(v,a)       __arch_putb(v,a)
#define __raw_writew(v,a)       __arch_putw(v,a)
#define __raw_writel(v,a)       __arch_putl(v,a)
#define __raw_writeq(v,a)       __arch_putq(v,a)

#define __raw_readb(a)          __arch_getb(a)
#define __raw_readw(a)          __arch_getw(a)
#define __raw_readl(a)          __arch_getl(a)
#define __raw_readq(a)          __arch_getq(a)

 /*
  * TODO: The kernel offers some more advanced versions of barriers, it might
  * have some advantages to use them instead of the simple one here.
  */
#define mb()            asm volatile("dsb sy" : : : "memory")
#define dmb()           __asm__ __volatile__ ("" : : : "memory")
#define __iormb()       dmb()
#define __iowmb()       dmb()

#define writeb(v,c)     ({ u8  __v = v; __iowmb(); __arch_putb(__v,c); __v; })
#define writew(v,c)     ({ u16 __v = v; __iowmb(); __arch_putw(__v,c); __v; })
#define writel(v,c)     ({ u32 __v = v; __iowmb(); __arch_putl(__v,c); __v; })
#define writeq(v,c)     ({ u64 __v = v; __iowmb(); __arch_putq(__v,c); __v; })

#define readb(c)        ({ u8  __v = __arch_getb(c); __iormb(); __v; })
#define readw(c)        ({ u16 __v = __arch_getw(c); __iormb(); __v; })
#define readl(c)        ({ u32 __v = __arch_getl(c); __iormb(); __v; })
#define readq(c)        ({ u64 __v = __arch_getq(c); __iormb(); __v; })



/*
 * configures a single pad in the iomuxer
 */
void imx_iomux_v3_setup_pad(iomux_v3_cfg_t pad)
{
	u32 mux_ctrl_ofs = (pad & MUX_CTRL_OFS_MASK) >> MUX_CTRL_OFS_SHIFT;
	u32 mux_mode = (pad & MUX_MODE_MASK) >> MUX_MODE_SHIFT;
	u32 sel_input_ofs =
		(pad & MUX_SEL_INPUT_OFS_MASK) >> MUX_SEL_INPUT_OFS_SHIFT;
	u32 sel_input =
		(pad & MUX_SEL_INPUT_MASK) >> MUX_SEL_INPUT_SHIFT;
	u32 pad_ctrl_ofs =
		(pad & MUX_PAD_CTRL_OFS_MASK) >> MUX_PAD_CTRL_OFS_SHIFT;
	u32 pad_ctrl = (pad & MUX_PAD_CTRL_MASK) >> MUX_PAD_CTRL_SHIFT;

#if defined CONFIG_MX6SL
	/* Check whether LVE bit needs to be set */
	if (pad_ctrl & PAD_CTL_LVE) {
		pad_ctrl &= ~PAD_CTL_LVE;
		pad_ctrl |= PAD_CTL_LVE_BIT;
	}
#endif

#ifdef CONFIG_IOMUX_LPSR
	u32 lpsr = (pad & MUX_MODE_LPSR) >> MUX_MODE_SHIFT;

	if (lpsr == IOMUX_CONFIG_LPSR) {
		base = (void *)IOMUXC_LPSR_BASE_ADDR;
		mux_mode &= ~IOMUX_CONFIG_LPSR;
		/* set daisy chain sel_input */
		if (sel_input_ofs)
			sel_input_ofs += IOMUX_LPSR_SEL_INPUT_OFS;
	}
#endif

	__raw_writel(mux_mode, base + mux_ctrl_ofs);

	if (sel_input_ofs)
		__raw_writel(sel_input, base + sel_input_ofs);

#ifdef CONFIG_IOMUX_SHARE_CONF_REG
	if (!(pad_ctrl & NO_PAD_CTRL))
		__raw_writel((mux_mode << PAD_MUX_MODE_SHIFT) | pad_ctrl,
			base + pad_ctrl_ofs);
#else
	if (!(pad_ctrl & NO_PAD_CTRL) && pad_ctrl_ofs)
		__raw_writel(pad_ctrl, base + pad_ctrl_ofs);
#endif

#ifdef CONFIG_IOMUX_LPSR
	if (lpsr == IOMUX_CONFIG_LPSR)
		base = (void *)IOMUXC_BASE_ADDR;
#endif

}

/* configures a list of pads within declared with IOMUX_PADS macro */
void imx_iomux_v3_setup_multiple_pads(iomux_v3_cfg_t const *pad_list,
				      unsigned count)
{
	iomux_v3_cfg_t const *p = pad_list;
	int stride;
	int i;

#if defined(CONFIG_MX6QDL)
	stride = 2;
	if (!is_cpu_type(MXC_CPU_MX6Q) && !is_cpu_type(MXC_CPU_MX6D))
		p += 1;
#else
	stride = 1;
#endif
	for (i = 0; i < count; i++) {
		imx_iomux_v3_setup_pad(*p);
		p += stride;
	}
}

void imx_iomux_set_gpr_register(int group, int start_bit,
					int num_bits, int value)
{
	int i = 0;
	u32 reg;
	reg = readl(base + group * 4);
	while (num_bits) {
		reg &= ~(1<<(start_bit + i));
		i++;
		num_bits--;
	}
	reg |= (value << start_bit);
	writel(reg, base + group * 4);
}
