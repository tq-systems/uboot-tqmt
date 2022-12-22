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

/*
 * name and path of device tree set in default environment
 * for nfs boot and sd card boot
 */
#define FDT_FILE         "tqmt1040-stkt104x.dtb"

#include <configs/TQMT10xx.h>

#ifdef CONFIG_FMAN_ENET
#define CONFIG_SYS_SGMII1_PHY_ADDR		0x1D
#define CONFIG_SYS_SGMII2_PHY_ADDR		0x03
#define CONFIG_SYS_SGMII3_PHY_ADDR		0x1C
#define CONFIG_SYS_SGMII4_PHY_ADDR		0x01
#define CONFIG_SYS_RGMII1_PHY_ADDR		0x0E
#define CONFIG_SYS_RGMII2_PHY_ADDR		0x05

#define CONFIG_MII		/* MII PHY management */
#define CONFIG_ETHPRIME		"FM1@DTSEC4"
#define CONFIG_PHY_GIGE		/* Include GbE speed/duplex detection */
#endif

#endif	/* __CONFIG_TQMT1040_H */
