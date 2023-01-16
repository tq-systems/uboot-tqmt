/*
 * Copyright 2015 TQ Systems GmbH
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __TQMT10xx_H__
#define __TQMT10xx_H__

void fdt_fixup_board_enet(void *blob);
void pci_of_setup(void *blob, bd_t *bd);

#endif
