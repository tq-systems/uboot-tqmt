/*
 * Copyright 2015 TQ Systems GmbH
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/io.h>
#include <common.h>
#include <command.h>
#include <fsl_diu_fb.h>
#include <linux/ctype.h>
#include <video_fb.h>

#include "tqmt10xx.h"

/*
 * DIU Area Descriptor
 *
 * Note that we need to byte-swap the value before it's written to the AD
 * register.
 */
#define AD_BYTE_F		0x10000000
#define AD_ALPHA_C_SHIFT	25
#define AD_BLUE_C_SHIFT		23
#define AD_GREEN_C_SHIFT	21
#define AD_RED_C_SHIFT		19
#define AD_PIXEL_S_SHIFT	16
#define AD_COMP_3_SHIFT		12
#define AD_COMP_2_SHIFT		8
#define AD_COMP_1_SHIFT		4
#define AD_COMP_0_SHIFT		0

void diu_set_pixel_clock(unsigned int pixclock)
{
	unsigned long speed_ccb, temp;
	u32 pixval;

	speed_ccb = get_bus_freq(0);
	temp = 1000000000 / pixclock;
	temp *= 1000;
	pixval = speed_ccb / temp;

	/* Program pixel clock */
	out_be32((unsigned *)CONFIG_SYS_FSL_SCFG_PIXCLK_ADDR,
		 ((pixval << PXCK_BITS_START) & PXCK_MASK));

	/* enable clock*/
	out_be32((unsigned *)CONFIG_SYS_FSL_SCFG_PIXCLK_ADDR, PXCKEN_MASK |
		 ((pixval << PXCK_BITS_START) & PXCK_MASK));
}

int platform_diu_init(unsigned int xres, unsigned int yres, const char *port)
{
	u32 pixel_format;

	pixel_format = cpu_to_le32(AD_BYTE_F | (3 << AD_ALPHA_C_SHIFT) |
		(0 << AD_BLUE_C_SHIFT) | (1 << AD_GREEN_C_SHIFT) |
		(2 << AD_RED_C_SHIFT) | (8 << AD_COMP_3_SHIFT) |
		(8 << AD_COMP_2_SHIFT) | (8 << AD_COMP_1_SHIFT) |
		(8 << AD_COMP_0_SHIFT) | (3 << AD_PIXEL_S_SHIFT));

	printf("DIU: Switching to monitor LVDS @ %ux%u\n",  xres, yres);

	return fsl_diu_init(xres, yres, pixel_format, 0);
}
