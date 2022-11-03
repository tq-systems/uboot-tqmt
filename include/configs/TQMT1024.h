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

#endif	/* __CONFIG_TQMT1024_H */
