/*
 * Copyright 2014 Freescale Semiconductor, Inc.
 *
 * Based on board/freescale/common/vid.c
 *
 * Copyright 2017 TQ Systems GmbH
 *
 * Author: Stefan Lange <s.lange@gateware.de>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <i2c.h>

#define SYSCTRLUC_I2C_ADDR		0x11
#define SYSCTRLUC_I2C_REG_VID_VALUE	0x09

/* helper sleep function */
static void usleep(int i)
{
/* CPU core clock: 1800 MhZ => 0,55ns */
#define CPU_TICKS_PER_US 2000

	int z;

	for (z = 0; z < (i * CPU_TICKS_PER_US); z++) {
		/* do nothing */
	}
}

/*
 * set voltage via writing to system microcontroller (uC)
 *
 * The uC acts as an I2C Device. The desired VID value must be
 * written to SYSCTRL_I2C_REG_VID_VALUE. The uC then alters the
 * DC/DC converters output voltage by superseding a PWM signal
 * on the DC/DC convertres FB pin.
 */
static int set_voltage(u8 vid)
{
	int ret;

	/* write to uC I2C register */
	ret = i2c_write(SYSCTRLUC_I2C_ADDR, SYSCTRLUC_I2C_REG_VID_VALUE,
			1, (void *)&vid, sizeof(vid));

	return ret;
}

int adjust_vdd(void)
{
	ccsr_gur_t __iomem *gur =
		(void __iomem *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	u32 fusesr;
	u8 vid;
	int vdd_target;
	int ret;
	static const uint16_t vdd[32] = {
		0,      /* unused */
		9875,   /* 0.9875V */
		9750,
		9625,
		9500,
		9375,
		9250,
		9125,
		9000,
		8875,
		8750,
		8625,
		8500,
		8375,
		8250,
		8125,
		10000,  /* 1.0000V */
		10125,
		10250,
		10375,
		10500,
		10625,
		10750,
		10875,
		11000,
		0,      /* reserved */
	};

	/* get the voltage ID from fuse status register */
	fusesr = in_be32(&gur->dcfg_fusesr);
	/*
	 * VID is used according to the table below
	 *                ---------------------------------------
	 *                |                DA_V                 |
	 *                |-------------------------------------|
	 *                | 5b00000 | 5b00001-5b11110 | 5b11111 |
	 * ---------------+---------+-----------------+---------|
	 * | D | 5b00000  | NO VID  | VID = DA_V      | NO VID  |
	 * | A |----------+---------+-----------------+---------|
	 * | _ | 5b00001  |VID =    | VID =           |VID =    |
	 * | V |   ~      | DA_V_ALT|   DA_V_ALT      | DA_A_VLT|
	 * | _ | 5b11110  |         |                 |         |
	 * | A |----------+---------+-----------------+---------|
	 * | L | 5b11111  | No VID  | VID = DA_V      | NO VID  |
	 * | T |          |         |                 |         |
	 * ------------------------------------------------------
	 */
	vid = (fusesr >> FSL_CORENET_DCFG_FUSESR_ALTVID_SHIFT) &
	       FSL_CORENET_DCFG_FUSESR_ALTVID_MASK;
	if ((vid == 0) || (vid == FSL_CORENET_DCFG_FUSESR_ALTVID_MASK)) {
		vid = (fusesr >> FSL_CORENET_DCFG_FUSESR_VID_SHIFT) &
		FSL_CORENET_DCFG_FUSESR_VID_MASK;
	}
	vdd_target = vdd[vid];

	if (vdd_target == 0) {
		printf("VID: VID value invalid, not used\n");
		return -1;
	}

	/* set voltage to fused vid value */
	ret = set_voltage(vid);

	if (ret < 0) {
		printf("VID: error on setting VID voltage\n");
		return -1;
	}

	/* wait for value to settle */
	usleep(1000);

	/* divide and round up by 10 to get a value in mV */
	vdd_target = DIV_ROUND_UP(vdd_target, 10);
	printf("VID: voltage set to vid = %d mV\n", vdd_target);

	return 0;
}
