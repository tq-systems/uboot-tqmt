/*
 * Copyright 2013 Freescale Semiconductor, Inc.
 *
 * Based on board/freescale/t104xrdb/eth.c
 *
 * Copyright 2017 TQ Systems GmbH
 *
 * Author: Stefan Lange <s.lange@gateware.de>
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
#include <vsc9953.h>
#include <i2c.h>
#include <hwconfig.h>

#define CONFIG_SYS_MODULE_EEPROM		0x50
#define CONFIG_SYS_MODULE_EEPROM_ADLEN          0x1
#define CONFIG_SYS_MODULE_EEPROM_MAC_AD_OFFSET  0x20

#define MII_DP83867_REGCR             0x000D
#define MII_DP83867_ADDAR             0x000E
#define MII_DP83867_REGCR_AD          0x001F
#define MII_DP83867_REGCR_DAT         0x401F
#define MII_DP83867_RGMIICTL          0x0032
#define MII_DP83867_RGMIIDCTL         0x0086
#define MII_DP83867_BYTEORDER         0x00DC
#define MII_88E1340_PAGEAD            0x0016
#define MII_DP83867_IOMUXCFG          0x0170

static int eth_phy_qsgmii_reprogram;

static int _tq_get_macaddress_from_eeprom(void)
{
	int err;
	int i, j;
	uint8_t ethAddrBuf[6];
	char ethAddrStr[18]; /* xx:xx:xx:xx:xx:xx\0 */
	const char *varname;
	unsigned int old_i2c_bus;

	/* Read MAC address from EEPROM */
	old_i2c_bus = i2c_get_bus_num();
	i2c_set_bus_num(0);
	err = i2c_read(CONFIG_SYS_MODULE_EEPROM,
		       CONFIG_SYS_MODULE_EEPROM_MAC_AD_OFFSET,
		       CONFIG_SYS_MODULE_EEPROM_ADLEN, ethAddrBuf,
		       sizeof(ethAddrBuf));
	i2c_set_bus_num(old_i2c_bus);

	if (err) {
		printf("Could not read MAC addresses: %d\n", err);
		return err;
	} else {
		for (i = 0; i < (CONFIG_SYS_NUM_FM1_DTSEC +
		    CONFIG_SYS_NUM_FM1_10GEC); i++) {
			if (i == 0)
				varname = "ethaddr";
			else if (i == 1)
				varname = "eth1addr";
			else if (i == 2)
				varname = "eth2addr";
			else if (i == 3)
				varname = "eth3addr";
			else if (i == 4)
				varname = "eth4addr";
			else if (i == 5)
				varname = "eth5addr";
			else if (i == 6)
				varname = "eth6addr";
			else if (i == 7)
				varname = "eth7addr";
			else if (i == 8)
				varname = "eth8addr";
			else if (i == 9)
				varname = "eth9addr";
			else if (i == 10)
				varname = "eth10addr";
			sprintf(
				ethAddrStr, "%02x:%02x:%02x:%02x:%02x:%02x",
				ethAddrBuf[0], ethAddrBuf[1], ethAddrBuf[2],
				ethAddrBuf[3], ethAddrBuf[4], ethAddrBuf[5]
			);
			setenv(varname, ethAddrStr);

			/* Increment MAC address by 1 with overflow. */
			j = sizeof(ethAddrBuf);
			do {
				j--;
				ethAddrBuf[j]++;
			} while (ethAddrBuf[j] == 0 && j > 0);
		}
	}
	return 0;
}

/*
 * Set RGMII delay and other values in ethernet phys
 * located on the STKTxxxx starterkit
 */
int board_phy_config(struct phy_device *phydev)
{
	if (phydev->drv->config)
		phydev->drv->config(phydev);

	if (phydev->addr == CONFIG_SYS_RGMII1_PHY_ADDR) {
		/* set EC1 RGMII delay (extended write) */
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_REGCR,
			  MII_DP83867_REGCR_AD);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_ADDAR,
			  MII_DP83867_RGMIIDCTL);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_REGCR,
			  MII_DP83867_REGCR_DAT);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_ADDAR, 0x0097);

		/* activate EC1 RGMII TX and RX delay (extended write) */
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_REGCR,
			  MII_DP83867_REGCR_AD);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_ADDAR,
			  MII_DP83867_RGMIICTL);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_REGCR,
			  MII_DP83867_REGCR_DAT);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_ADDAR, 0x00D3);
	}

	if (phydev->addr == CONFIG_SYS_RGMII2_PHY_ADDR) {
		/* set EC2 RGMII phy to output 125MHz on CLK_OUT pin
		 * this is required for the MAC
		 */
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_REGCR,
			  MII_DP83867_REGCR_AD);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_ADDAR,
			  MII_DP83867_IOMUXCFG);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_REGCR,
			  MII_DP83867_REGCR_DAT);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_ADDAR, 0x000D);

		/* set EC2 RGMII delay (extended write) */
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_REGCR,
			  MII_DP83867_REGCR_AD);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_ADDAR,
			  MII_DP83867_RGMIIDCTL);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_REGCR,
			  MII_DP83867_REGCR_DAT);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_ADDAR, 0x0097);

		/* activate EC2 RGMII TX and RX delay (extended write) */
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_REGCR,
			  MII_DP83867_REGCR_AD);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_ADDAR,
			  MII_DP83867_RGMIICTL);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_REGCR,
			  MII_DP83867_REGCR_DAT);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_ADDAR, 0x00D3);
	}

	if (phydev->addr == CONFIG_SYS_SGMII1_PHY_ADDR) {
		/* do 88E1340 phy initialisation #1 */
		/* as described in MV-S301615 release note */
		phy_write(phydev, MDIO_DEVAD_NONE, MII_88E1340_PAGEAD, 0xFF);
		phy_write(phydev, MDIO_DEVAD_NONE, 0x18, 0x2800);
		phy_write(phydev, MDIO_DEVAD_NONE, 0x17, 0x2001);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_88E1340_PAGEAD, 0x00);
		/* do 88E1340 phy initialisation #2 */
		/* as described in MV-S301615 release note */
		phy_write(phydev, MDIO_DEVAD_NONE, MII_88E1340_PAGEAD, 0x00);
		phy_write(phydev, MDIO_DEVAD_NONE, 0x1d, 0x0003);
		phy_write(phydev, MDIO_DEVAD_NONE, 0x1e, 0x0002);
		phy_write(phydev, MDIO_DEVAD_NONE, 0x1d, 0x0000);
	}

	if (phydev->addr == CONFIG_SYS_SGMII2_PHY_ADDR) {
		/* set DP83867 byteordering hidden register (extended write) */
		/* workaround for pre-production grade silicon */
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_REGCR,
			  MII_DP83867_REGCR_AD);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_ADDAR,
			  MII_DP83867_BYTEORDER);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_REGCR,
			  MII_DP83867_REGCR_DAT);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_ADDAR, 0x3800);
	}

	if (phydev->addr == CONFIG_SYS_SGMII5_PHY_ADDR) {
		/* set DP83867 byteordering hidden register (extended write) */
		/* workaround for pre-production grade silicon */
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_REGCR,
			  MII_DP83867_REGCR_AD);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_ADDAR,
			  MII_DP83867_BYTEORDER);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_REGCR,
			  MII_DP83867_REGCR_DAT);
		phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_ADDAR, 0x3800);
	}

	return 0;
}

int board_eth_init(bd_t *bis)
{
#ifdef CONFIG_FMAN_ENET
	struct memac_mdio_info memac_mdio_info;
	struct memac_mdio_info tgec_mdio_info;
	unsigned int i;
	int phy_addr = 0;
	struct mii_dev *dev;

	printf("Initializing Fman\n");

	memac_mdio_info.regs =
		(struct memac_mdio_controller *)CONFIG_SYS_FM1_DTSEC_MDIO_ADDR;
	memac_mdio_info.name = DEFAULT_FM_MDIO_NAME;

	/* Register the real 1G MDIO bus */
	fm_memac_mdio_init(bis, &memac_mdio_info);

	tgec_mdio_info.regs =
		(struct memac_mdio_controller *)CONFIG_SYS_FM1_TGEC_MDIO_ADDR;
	tgec_mdio_info.name = DEFAULT_FM_TGEC_MDIO_NAME;

	/* Register the 10G MDIO bus */
	fm_memac_mdio_init(bis, &tgec_mdio_info);

	/*
	 * Program on board RGMII, SGMII PHY addresses.
	 */
	for (i = FM1_DTSEC1; i < FM1_DTSEC1 + CONFIG_SYS_NUM_FM1_DTSEC; i++) {
		int idx = i - FM1_DTSEC1;

		switch (fm_info_get_enet_if(i)) {
		case PHY_INTERFACE_MODE_SGMII:
			printf("Eth:   configuring FM1_DTSEC%i as SGMII\n",
			       i + 1);
			if (i == FM1_DTSEC1)
				phy_addr = CONFIG_SYS_SGMII1_PHY_ADDR;
			if (i == FM1_DTSEC2)
				phy_addr = CONFIG_SYS_SGMII2_PHY_ADDR;
			if (i == FM1_DTSEC5)
				phy_addr = CONFIG_SYS_SGMII5_PHY_ADDR;
			/* SGMII DTSEC6 Serdes Lane H connected to SATA
			 * on STKTxxxx, not used
			 */
			if (i == FM1_DTSEC6) {
				printf("Eth:   SGMII DTSEC6 not used on STKT2081\n");
				phy_addr = 0;
			}
			fm_info_set_phy_address(i, phy_addr);
			break;
		case PHY_INTERFACE_MODE_RGMII:
			printf("Eth:   configuring FM1_DTSEC%i as RGMII\n",
			       i + 1);
			if (i == FM1_DTSEC3)
				phy_addr = CONFIG_SYS_RGMII1_PHY_ADDR;
			if (i == FM1_DTSEC4)
				phy_addr = CONFIG_SYS_RGMII2_PHY_ADDR;
			fm_info_set_phy_address(i, phy_addr);
			break;
		case PHY_INTERFACE_MODE_QSGMII:
			printf("Eth:   configuring FM1_DTSEC%i as QSGMII\n",
			       i + 1);
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

	for (i = FM1_10GEC1; i < FM1_10GEC1 + CONFIG_SYS_NUM_FM1_10GEC; i++) {
		switch (fm_info_get_enet_if(i)) {
		case PHY_INTERFACE_MODE_XGMII:
			printf("Eth:   configuring FM1_10GEC%i as XGMII\n",
			       i + 1);
			dev = miiphy_get_dev_by_name(DEFAULT_FM_TGEC_MDIO_NAME);
			fm_info_set_mdio(i, dev);
			break;
		default:
			break;
		}
	}

	/* get MAC addresses from I2C EEPROM */
	_tq_get_macaddress_from_eeprom();

	cpu_eth_init(bis);
#endif

	return pci_eth_init(bis);
}

void fdt_fixup_board_enet(void *fdt)
{
	return;
}
