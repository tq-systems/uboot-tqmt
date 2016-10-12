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
#include <phy.h>

static int eth_phy_qsgmii_reprogram = 0;

/*
 * Set RGMII delay and other values in ethernet phys
 * located on the STKTxxxx starterkit
 */
int board_phy_config(struct phy_device *phydev)
{
#define MII_DP83867_REGCR             0x000D
#define MII_DP83867_ADDAR             0x000E
#define MII_DP83867_REGCR_AD          0x001F
#define MII_DP83867_REGCR_DAT         0x401F
#define MII_DP83867_RGMIICTL          0x0032
#define MII_DP83867_RGMIIDCTL         0x0086
#define MII_DP83867_BYTEORDER         0x00DC
#define MII_88E1340_PAGEAD            0x0016

	if (phydev->drv->config)
		phydev->drv->config(phydev);

	if (phydev->addr == CONFIG_SYS_RGMII1_PHY_ADDR) {
		/* set EC1 RGMII delay (extended write) */
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_REGCR,MII_DP83867_REGCR_AD);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_ADDAR,MII_DP83867_RGMIIDCTL);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_REGCR,MII_DP83867_REGCR_DAT);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_ADDAR,0x0097);

		/* activate EC1 RGMII TX and RX delay (extended write) */
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_REGCR,MII_DP83867_REGCR_AD);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_ADDAR,MII_DP83867_RGMIICTL);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_REGCR,MII_DP83867_REGCR_DAT);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_ADDAR,0x00D3);
	}

	if (phydev->addr == CONFIG_SYS_RGMII2_PHY_ADDR) {
		/* set EC2 RGMII delay (extended write) */
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_REGCR,MII_DP83867_REGCR_AD);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_ADDAR,MII_DP83867_RGMIIDCTL);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_REGCR,MII_DP83867_REGCR_DAT);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_ADDAR,0x0097);

		/* activate EC2 RGMII TX and RX delay (extended write) */
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_REGCR,MII_DP83867_REGCR_AD);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_ADDAR,MII_DP83867_RGMIICTL);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_REGCR,MII_DP83867_REGCR_DAT);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_ADDAR,0x00D3);
	}

	if (phydev->addr == CONFIG_SYS_SGMII2_PHY_ADDR) {
		/* set DP83867 byteordering hidden register (extended write) */
		/* workaround for pre-production grade silicon */
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_REGCR,MII_DP83867_REGCR_AD);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_ADDAR,MII_DP83867_BYTEORDER);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_REGCR,MII_DP83867_REGCR_DAT);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_ADDAR,0x3800);
	}

	if (phydev->addr == CONFIG_SYS_SGMII4_PHY_ADDR) {
		/* set DP83867 byteordering hidden register (extended write) */
		/* workaround for pre-production grade silicon */
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_REGCR,MII_DP83867_REGCR_AD);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_ADDAR,MII_DP83867_BYTEORDER);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_REGCR,MII_DP83867_REGCR_DAT);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_ADDAR,0x3800);
	}

	if (phydev->addr == CONFIG_SYS_SGMII3_PHY_ADDR) {
		/* do 88E1340 phy initialisation #1 */
		/* as described in MV-S301615 release note */
		phy_write(phydev,MDIO_DEVAD_NONE,MII_88E1340_PAGEAD,0xFF);
		phy_write(phydev,MDIO_DEVAD_NONE,0x18,0x2800);
		phy_write(phydev,MDIO_DEVAD_NONE,0x17,0x2001);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_88E1340_PAGEAD,0x00);
		/* do 88E1340 phy initialisation #2 */
		/* as described in MV-S301615 release note */
		phy_write(phydev,MDIO_DEVAD_NONE,MII_88E1340_PAGEAD,0x00);
		phy_write(phydev,MDIO_DEVAD_NONE,0x1d,0x0003);
		phy_write(phydev,MDIO_DEVAD_NONE,0x1e,0x0002);
		phy_write(phydev,MDIO_DEVAD_NONE,0x1d,0x0000);
	}

	if ((phydev->addr == CONFIG_SYS_QSGMII1_PHY_ADDR) && (eth_phy_qsgmii_reprogram == 1)) {
		printf("Eth:   reprogramming 88E1340 ethernet phy to QSGMII\n");
                phy_write(phydev,MDIO_DEVAD_NONE,MII_88E1340_PAGEAD,0x06);
                phy_write(phydev,MDIO_DEVAD_NONE,0x14,0x8200);
                phy_write(phydev,MDIO_DEVAD_NONE,MII_88E1340_PAGEAD,0x00);
	}

	return 0;
}

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
                        printf("Eth:   configuring FM1_DTSEC%i as SGMII\n", i+1);
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
			printf("Eth:   configuring FM1_DTSEC%i as RGMII\n", i+1);
			if (FM1_DTSEC4 == i)
				phy_addr = CONFIG_SYS_RGMII1_PHY_ADDR;
			if (FM1_DTSEC5 == i)
				phy_addr = CONFIG_SYS_RGMII2_PHY_ADDR;
			fm_info_set_phy_address(i, phy_addr);
			break;
		case PHY_INTERFACE_MODE_QSGMII:
			/* QSGMII unsupported on T1042, supported on T1040
			 * TODO: QSGMII currently errorous on hardware
			 * implement after hardware has been fixed
			 * only automatic reprogramming of the phy
			 * from SGMII to QSGII
			 * has been implemented so far */
			printf("Eth:   configuring FM1_DTSEC%i as QSGMII\n", i+1);
			eth_phy_qsgmii_reprogram = 1;
			fm_info_set_phy_address(i, phy_addr);
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
