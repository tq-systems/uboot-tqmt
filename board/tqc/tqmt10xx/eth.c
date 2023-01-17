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
#include <vsc9953.h>
#include <i2c.h>
#include <hwconfig.h>

#define CONFIG_SYS_MODULE_EEPROM         	0x50
#define CONFIG_SYS_MODULE_EEPROM_ADLEN          0x1
#define CONFIG_SYS_MODULE_EEPROM_MAC_AD_OFFSET  0x20

static int eth_phy_qsgmii_reprogram = 0;

static int _tq_get_macaddress_from_eeprom(void) {

	int err;
	int i,j;
	uint8_t ethAddrBuf[6];
	char ethAddrStr[18]; /* xx:xx:xx:xx:xx:xx\0 */
	const char *varname;
	unsigned int oldI2cBus;

	/* Read MAC address from EEPROM */
	oldI2cBus = i2c_get_bus_num();
	i2c_set_bus_num(0);
	err = i2c_read(CONFIG_SYS_MODULE_EEPROM, CONFIG_SYS_MODULE_EEPROM_MAC_AD_OFFSET, \
			CONFIG_SYS_MODULE_EEPROM_ADLEN, ethAddrBuf, sizeof(ethAddrBuf));
	i2c_set_bus_num(oldI2cBus);

	if(err) {
		printf("Could not read MAC addresses: %d\n", err);
		return err;
	} else {
		for(i = 0; i < 5; i++) {
			if(i == 0) {
				varname = "ethaddr";
			} else if(i == 1) {
				varname = "eth1addr";
			} else if(i == 2) {
				varname = "eth2addr";
			} else if(i == 3) {
				varname = "eth3addr";
			} else if(i == 4) {
				varname = "eth4addr";
			}
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
			} while(ethAddrBuf[j] == 0 && j > 0);
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
#define MII_DP83867_REGCR             0x000D
#define MII_DP83867_ADDAR             0x000E
#define MII_DP83867_REGCR_AD          0x001F
#define MII_DP83867_REGCR_DAT         0x401F
#define MII_DP83867_RGMIICTL          0x0032
#define MII_DP83867_RGMIIDCTL         0x0086
#define MII_DP83867_BYTEORDER         0x00DC
#define MII_88E1340_PAGEAD            0x0016
#define MII_DP83867_IOMUXCFG          0x0170

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
		/* set EC2 RGMII phy to output 125MHz on CLK_OUT pin (required for MAC) */
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_REGCR,MII_DP83867_REGCR_AD);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_ADDAR,MII_DP83867_IOMUXCFG);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_REGCR,MII_DP83867_REGCR_DAT);
		phy_write(phydev,MDIO_DEVAD_NONE,MII_DP83867_ADDAR,0x000D);

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

#ifdef CONFIG_TQMT1040
	if ((phydev->addr == CONFIG_SYS_FM1_QSGMII11_PHY_ADDR) && (eth_phy_qsgmii_reprogram == 1)) {
		printf("Eth:   reprogramming 88E1340 ethernet phy to QSGMII\n");
                phy_write(phydev,MDIO_DEVAD_NONE,MII_88E1340_PAGEAD,0x06);
                phy_write(phydev,MDIO_DEVAD_NONE,0x14,0x8200);
                phy_write(phydev,MDIO_DEVAD_NONE,MII_88E1340_PAGEAD,0x00);
	}
#endif

	return 0;
}

void board_ft_fman_fixup_port(void *blob, char * prop, phys_addr_t pa,
				enum fm_port port, int offset)
{
	if (fm_info_get_enet_if(port) == PHY_INTERFACE_MODE_RGMII)
		fdt_fixup_phy_connection(blob, offset, PHY_INTERFACE_MODE_RGMII_ID);
}

int board_eth_init(bd_t *bis)
{
#ifdef CONFIG_FMAN_ENET
	struct memac_mdio_info memac_mdio_info;
	unsigned int i;
	int phy_addr = 0;
#ifdef CONFIG_VSC9953
        phy_interface_t phy_int;
        struct mii_dev *bus;
#endif

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

#ifdef CONFIG_VSC9953
        /* SerDes configured for QSGMII */
        if (serdes_get_first_lane(FSL_SRDS_1, QSGMII_SW1_A) >= 0) {
                for (i = 0; i < 4; i++) {
                        bus = miiphy_get_dev_by_name(DEFAULT_FM_MDIO_NAME);
                        phy_addr = CONFIG_SYS_FM1_QSGMII11_PHY_ADDR + i;
                        phy_int = PHY_INTERFACE_MODE_QSGMII;

                        vsc9953_port_info_set_mdio(i, bus);
                        vsc9953_port_info_set_phy_address(i, phy_addr);
                        vsc9953_port_info_set_phy_int(i, phy_int);
                        vsc9953_port_enable(i);
                }
        }

        /* Connect DTSEC1 to L2 switch if it doesn't have a PHY */
        if (serdes_get_first_lane(FSL_SRDS_1, SGMII_FM1_DTSEC1) < 0)
                vsc9953_port_enable(8);

        /* Connect DTSEC2 to L2 switch if it doesn't have a PHY */
        if (serdes_get_first_lane(FSL_SRDS_1, SGMII_FM1_DTSEC2) < 0) {
                /* Enable L2 On MAC2 using SCFG */
                struct ccsr_scfg *scfg = (struct ccsr_scfg *)
                                CONFIG_SYS_MPC85xx_SCFG;

                out_be32(&scfg->esgmiiselcr, in_be32(&scfg->esgmiiselcr) |
                         (0x80000000));
                vsc9953_port_enable(9);
        }
#endif

        /* get MAC addresses from I2C EEPROM */
        _tq_get_macaddress_from_eeprom();

	cpu_eth_init(bis);
#endif

	return pci_eth_init(bis);
}
