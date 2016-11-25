/*
 * Copyright 2015 TQ Systems GmbH
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_TQMT1042_H
#define __CONFIG_TQMT1042_H

/* name and path of kernel image and device tree
 * set in default environment
 * for nfs boot and sd card boot
 */
#define CONFIG_BOOTFILE         "boot/uImage-tqmt1042-64b-stk.bin"
#define FDTFILE         "boot/uImage-tqmt1042.dtb"

#include <configs/TQMT104x.h>

#endif	/* __CONFIG_TQMT1042_H */
