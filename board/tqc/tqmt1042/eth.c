/*
 * Copyright 2015 TQ Systems GmbH
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <netdev.h>
#include <asm/fsl_serdes.h>
#include <asm/immap_85xx.h>
#include <fm_eth.h>
#include <fsl_mdio.h>
#include <malloc.h>
#include <asm/fsl_dtsec.h>

int board_eth_init(bd_t *bis)
{
#ifdef CONFIG_FMAN_ENET
	struct memac_mdio_info memac_mdio_info;
	unsigned int i;
	int phy_addr = 0;

	printf("Initializing Fman\n");

	memac_mdio_info.regs =
		(struct memac_mdio_controller *)CONFIG_SYS_FM1_DTSEC_MDIO_ADDR;
	memac_mdio_info.name = DEFAULT_FM_MDIO_NAME;

	/* Register the real 1G MDIO bus */
	fm_memac_mdio_init(bis, &memac_mdio_info);

	/*
	 * Program on board RGMII, SGMII PHY addresses.
	 */
	for (i = FM1_DTSEC1; i < FM1_DTSEC1 + CONFIG_SYS_NUM_FM1_DTSEC; i++) {
		int idx = i - FM1_DTSEC1;

		switch (fm_info_get_enet_if(i)) {
		case PHY_INTERFACE_MODE_SGMII:
                        if (FM1_DTSEC1 == i)
                                phy_addr = CONFIG_SYS_SGMII1_PHY_ADDR;
                        if (FM1_DTSEC2 == i)
                                phy_addr = CONFIG_SYS_SGMII2_PHY_ADDR;
                        if (FM1_DTSEC3 == i)
                                phy_addr = CONFIG_SYS_SGMII3_PHY_ADDR;
                        if (FM1_DTSEC4 == i)
                                phy_addr = CONFIG_SYS_SGMII4_PHY_ADDR;
			fm_info_set_phy_address(i, phy_addr);
			break;
		case PHY_INTERFACE_MODE_RGMII:
			if (FM1_DTSEC4 == i)
				phy_addr = CONFIG_SYS_RGMII1_PHY_ADDR;
			if (FM1_DTSEC5 == i)
				phy_addr = CONFIG_SYS_RGMII2_PHY_ADDR;
			fm_info_set_phy_address(i, phy_addr);
			break;
		case PHY_INTERFACE_MODE_QSGMII:
			/* QSGMII unsupported on T1042 */
			fm_info_set_phy_address(i, 0);
			break;
		case PHY_INTERFACE_MODE_NONE:
			fm_info_set_phy_address(i, 0);
			break;
		default:
			printf("Fman1: DTSEC%u set to unknown interface %i\n",
			       idx + 1, fm_info_get_enet_if(i));
			fm_info_set_phy_address(i, 0);
			break;
		}
		if (fm_info_get_enet_if(i) == PHY_INTERFACE_MODE_QSGMII ||
		    fm_info_get_enet_if(i) == PHY_INTERFACE_MODE_NONE)
			fm_info_set_mdio(i, NULL);
		else
			fm_info_set_mdio(i,
					 miiphy_get_dev_by_name(
							DEFAULT_FM_MDIO_NAME));
	}

	cpu_eth_init(bis);
#endif

	return pci_eth_init(bis);
}
