/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2015, 2022 TQ-Systems GmbH <license@tq-group.com>, D-82229 Seefeld, Germany.
 * Author: Gregor Herburger
 *
 */

#ifndef __CONFIG_TQMT1024_H
#define __CONFIG_TQMT1024_H

/*
 * name and path of device tree set in default environment
 * for nfs boot and sd card boot
 */
#define FDT_FILE		"tqmt1024-stkt104x.dtb"

#include <configs/TQMT10xx.h>

#define CONFIG_SYS_FSL_USB_INTERNAL_CLK

#ifdef CONFIG_FMAN_ENET
#define CONFIG_SYS_SGMII1_PHY_ADDR		0x1D
#define CONFIG_SYS_SGMII2_PHY_ADDR		0x01
#define CONFIG_SYS_SGMII3_PHY_ADDR		0x1C
#define CONFIG_SYS_SGMII4_PHY_ADDR		0x03
#define CONFIG_SYS_RGMII1_PHY_ADDR		0x0E
#define CONFIG_SYS_RGMII2_PHY_ADDR		0x05

#define CONFIG_MII		/* MII PHY management */
#define CONFIG_ETHPRIME		"FM1@DTSEC4"
#define CONFIG_PHY_GIGE		/* Include GbE speed/duplex detection */
#endif

#define CONFIG_SYS_XFI_RETIMER_BUS_NUM	0x1
#define CONFIG_SYS_XFI_RETIMER_ADDR	0x18

#define CONFIG_SYS_GPIO_EXP_BUS_NUM	0x1
#define CONFIG_SYS_GPIO_XFI_ENSMB_ADDR	0x20
#define CONFIG_SYS_GPIO_XFI_ENSMB_PIN	6
#define CONFIG_SYS_GPIO_XFI_TX_DIS_ADDR	0x23
#define CONFIG_SYS_GPIO_XFI_TX_DIS_PIN	1

#ifndef CONFIG_SYS_NO_FLASH
#define CONFIG_SYS_CSPR0_EXT		CONFIG_SYS_NOR_CSPR_EXT
#define CONFIG_SYS_CSPR0		CONFIG_SYS_NOR_CSPR
#define CONFIG_SYS_AMASK0		CONFIG_SYS_NOR_AMASK
#define CONFIG_SYS_CSOR0		CONFIG_SYS_NOR_CSOR
#define CONFIG_SYS_CS0_FTIM0		CONFIG_SYS_NOR_FTIM0_400
#define CONFIG_SYS_CS0_FTIM1		CONFIG_SYS_NOR_FTIM1_400
#define CONFIG_SYS_CS0_FTIM2		CONFIG_SYS_NOR_FTIM2_400
#define CONFIG_SYS_CS0_FTIM3		CONFIG_SYS_NOR_FTIM3_400
#endif

#endif	/* __CONFIG_TQMT1024_H */
