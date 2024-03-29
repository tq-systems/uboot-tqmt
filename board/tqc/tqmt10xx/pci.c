/*
 * Copyright 2015 TQ Systems GmbH
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <pci.h>
#include <asm/fsl_pci.h>
#include <libfdt.h>
#include <fdt_support.h>
#include <asm/fsl_serdes.h>

void pci_init_board(void)
{
	fsl_pcie_init_board(0);
}

void pci_of_setup(void *blob, bd_t *bd)
{
	FT_FSL_PCI_SETUP;
}
