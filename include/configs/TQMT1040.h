/*
 * Copyright 2015 TQ Systems GmbH
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_TQMT1040_H
#define __CONFIG_TQMT1040_H

/* Enable VSC9953 L2 Switch driver on T1040 SoC */
#define CONFIG_VSC9953
#define CONFIG_VSC9953_CMD
#define CONFIG_SYS_FM1_QSGMII11_PHY_ADDR             0x1C

/* name and path of kernel image and device tree
 * set in default environment
 * for nfs boot and sd card boot
 */
#define CONFIG_BOOTFILE "tqmt1040-linuximage"
#define FDTFILE         "tqmt1040-stkt104x.dtb"

#include <configs/TQMT104x.h>

#endif	/* __CONFIG_TQMT1040_H */
