/*
 * Copyright 2015 TQ Systems GmbH
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <netdev.h>
#include <linux/compiler.h>
#include <asm/mmu.h>
#include <asm/processor.h>
#include <asm/cache.h>
#include <asm/immap_85xx.h>
#include <asm/fsl_law.h>
#include <asm/fsl_serdes.h>
#include <asm/fsl_portals.h>
#include <asm/fsl_liodn.h>
#include <fm_eth.h>
#include "sleep.h"
#include "tqmt104x.h"
#include <i2c.h>
#include <asm/io.h>
#include <linux/sizes.h>
#include <pca953x.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	struct cpu_type *cpu = gd->arch.cpu;

	printf("Board: TQM%s\n", cpu->name);

	return 0;
}

typedef struct _stktxxxx_849n202_clkgen_config {
	int	reg;
	uchar	val;
} STKTxxxx_849N202_CLKGEN_CONFIG;

static STKTxxxx_849N202_CLKGEN_CONFIG stktxxxx_849n202_clkgen_125mhz_configtbl[] = {
	{0x00, 0x00},
	{0x01, 0x00},
	{0x02, 0x00},
	{0x03, 0x00},
	{0x04, 0x14},
	{0x05, 0x14},
	{0x06, 0x00},
	{0x07, 0x00},
	{0x08, 0x7d},
	{0x09, 0x00},
	{0x0a, 0x08},
	{0x0b, 0x00},
	{0x0c, 0xdc},
	{0x0d, 0x00},
	{0x0e, 0x00},
	{0x0f, 0x01},
	{0x10, 0x20},
	{0x11, 0x90},
	{0x12, 0xd7},
	{0x13, 0xd4},
	{0x14, 0x5c},
	{0x15, 0x08},
	{0x16, 0xa0},
	{0x17, 0x00},
	{-1, 0},
};

static int stktxxxx_849n202_clkgen_write_reg(int reg, uchar buf)
{
	if (i2c_write(CONFIG_SYS_849N202_CLKGEN_ADDR, reg, 1, &buf, 1) != 0) {
		puts ("Error writing the chip.\n");
		return 1;
	}
	return 0;
}

static int port_exp_direction_output(int i2c_busnr, int port_exp_i2c_ad, int gpio_nr, int value)
{
	int ret;
	int oldbus = i2c_get_bus_num();

	i2c_set_bus_num(i2c_busnr);
	ret = i2c_probe(port_exp_i2c_ad);
	if (ret)
		return ret;

	/* set direction to output */
	ret = pca953x_set_dir(port_exp_i2c_ad,
				(1 << gpio_nr),
				(PCA953X_DIR_OUT << gpio_nr));
	if (ret)
		return ret;

	/* set value */
	ret = pca953x_set_val(port_exp_i2c_ad,
		(1 << gpio_nr),
		(value << gpio_nr));
	if (ret)
		return ret;

	i2c_set_bus_num(oldbus);

	return 0;
}

static int clkgen_849n202_125mhz_init(void)
{
	int oldbus = i2c_get_bus_num();
	int i = 0;
	int ret;
	u32 pll_num, pll_status, pll_rstctl;
	serdes_corenet_t  __iomem *srds_regs = (void *)CONFIG_SYS_FSL_CORENET_SERDES_ADDR;

	/* set 849n202 clock generator to output 125 MHz */

	printf("reprogramming 849n202 clock generator to 125 MHz\n");
	i2c_set_bus_num(1);
	while (stktxxxx_849n202_clkgen_125mhz_configtbl[i].reg != -1) {
		debug("write reg 0x%02x\n", i);
		ret = stktxxxx_849n202_clkgen_write_reg(stktxxxx_849n202_clkgen_125mhz_configtbl[i].reg,
			stktxxxx_849n202_clkgen_125mhz_configtbl[i].val);
		if (ret != 0)
			return -1;
		i++;
	}
	i2c_set_bus_num(oldbus);

	/* reset CPU-internal PLL1 block */

	printf("resetting CPU-internal PLL1 block\n");
	pll_num = 0;
	/* debug */
	pll_status = in_be32(&srds_regs->bank[pll_num].pllcr0);
	debug("TQMT1042_PLL_RESET: pll_num=%x pllcr0=%x\n",
			pll_num, pll_status);
	/* Write PLL Reset Control Register, RSTREQ bit to reset PLL */
	/* Write SRDS_PLLnRSTCTL[0] = 1 (write 1, self-clearing) */
	pll_rstctl = in_be32(&srds_regs->bank[pll_num].rstctl);
	out_be32(&srds_regs->bank[pll_num].rstctl,
		 (pll_rstctl |= SRDS_RSTCTL_RST));
	debug("TQMT1042_PLL_RESET: pll_num=%x Updated PLLRSTCTL=%x\n",
	      pll_num, (pll_rstctl |= SRDS_RSTCTL_RST));
	/* Wait 750us to verify the PLL is locked */
	/* by checking SRDSxPLLnCR0[8] = 1. */
	udelay(750);
	pll_status = in_be32(&srds_regs->bank[pll_num].pllcr0);
	debug("TQMT1042_PLL_RESET: pll_num=%x pllcr0=%x\n",
			pll_num, pll_status);
	if ((pll_status & SRDS_PLLCR0_PLL_LCK) == 0)
		printf("TQMT1042_PLL_RESET: Serdes PLL not locked\n");
	else
		debug("TQMT1042_PLL_RESET: Serdes PLL locked\n");

	/* reset 88E1340 ethernet phy via gpio */

	printf("resetting 88E1340 ethernet phy\n");
	/* set 88E1340_RST signal to 0 */
	ret = port_exp_direction_output(CONFIG_SYS_849N202_I2C_BUSNR, \
			CONFIG_SYS_88E1340_RST_I2C_PCA953X_ADDR, CONFIG_SYS_88E1340_RST_I2C_PCA953X_GPIO, 0);
	udelay(10000);
	/* set 88E1340_RST signal to 1 */
	ret = port_exp_direction_output(CONFIG_SYS_849N202_I2C_BUSNR, \
			CONFIG_SYS_88E1340_RST_I2C_PCA953X_ADDR, CONFIG_SYS_88E1340_RST_I2C_PCA953X_GPIO, 1);

	return 0;
}

int board_early_init_f(void)
{
#if defined(CONFIG_DEEP_SLEEP)
	if (is_warm_boot())
		fsl_dp_disable_console();
#endif

	return 0;
}

int board_early_init_r(void)
{
#ifdef CONFIG_SYS_FLASH_BASE
	int flash_esel = find_tlb_idx((void *)CONFIG_SYS_FLASH_BASE, 1);
	int flash_esel2 =
		find_tlb_idx((void *)(CONFIG_SYS_FLASH_BASE + SZ_64M), 1);
	u32 srds_pll_ref_clk_sel_s1;
	ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	int ret;

	/*
	 * Remap Boot flash region to caching-inhibited
	 * so that flash can be erased properly.
	 */

	/* Flush d-cache and invalidate i-cache of any FLASH data */
	flush_dcache();
	invalidate_icache();

	if (flash_esel == -1) {
		/* very unlikely unless something is messed up */
		puts("Error: Could not find TLB for FLASH BASE\n");
	} else {
		/* invalidate existing TLB entry for flash */
		disable_tlb(flash_esel);
		set_tlb(1, CONFIG_SYS_FLASH_BASE, CONFIG_SYS_FLASH_BASE_PHYS,
			MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
			0, flash_esel, BOOKE_PAGESZ_64M, 1);
	}
	if (flash_esel2 == -1) {
		/* very unlikely unless something is messed up */
		puts("Error: Could not find TLB for FLASH BASE + 64M\n");
	} else {
		/* invalidate existing TLB entry for flash */
		disable_tlb(flash_esel2);
		set_tlb(1, CONFIG_SYS_FLASH_BASE + SZ_64M,
			CONFIG_SYS_FLASH_BASE_PHYS + SZ_64M,
			MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
			0, flash_esel2, BOOKE_PAGESZ_64M, 1);
	}

#endif
	set_liodns();
#ifdef CONFIG_SYS_DPAA_QBMAN
	setup_portals();
#endif

#ifdef CONFIG_SYS_TQMT1042_PLL1_CLKGEN_AUTOREPROG
	/* read RCW SRDS_PLL_REF_CLK_SEL_S1
	 * to detemine which PLL1 reference clock frequency is required */
	srds_pll_ref_clk_sel_s1 = in_be32(&gur->rcwsr[5]) &
			FSL_CORENET2_RCWSR5_SRDS_PLL_REF_CLK_SEL_S1_PLL1;
	srds_pll_ref_clk_sel_s1 >>= FSL_CORENET2_RCWSR5_SRDS_PLL_REF_CLK_SEL_S1_PLL1_SHIFT;

	switch (srds_pll_ref_clk_sel_s1) {
		/* SRDS_PLL_REF_CLK_SEL_S1 set to 125 MHz
		 * reprogram clock generator, reset PLL, reset 88E1340 ethernet phy */
		case 0x1:
			printf("SRDS_PLL_REF_CLK_SEL_S1 = 1 (125MHz) detected in RCW\n");
			ret = clkgen_849n202_125mhz_init();
			if (ret != 0)
				printf("error on reconfiguring serdes pll1 to 125 MHz\n");
			break;
		/* SRDS_PLL_REF_CLK_SEL_S1 set to 100 MHz, no action required */
		case 0x0:
		default:
			break;
	}
#endif

	return 0;
}

int misc_init_r(void)
{
/* As in some STKTxxxx SerDes configurations EC1 is inactive,
 * use EC2_GTX_CLK125 as GTX clock if EC1 is not active.
 */
	struct ccsr_scfg __iomem *scfg = (void *)CONFIG_SYS_MPC85xx_SCFG;
	ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	u32 rcwsr13 = in_be32(&gur->rcwsr[13]);
	u32 emiiocr;

	if ((rcwsr13 & FSL_CORENET_RCWSR13_EC1) != FSL_CORENET_RCWSR13_EC1_FM1_DTSEC4_RGMII) {
		emiiocr = (in_be32(&scfg->emiiocr));
		out_be32(&scfg->emiiocr, emiiocr |= CCSR_SCFG_EMIIOCR_EC2_GTX_CLK125);
	}
	return 0;
}

int ft_board_setup(void *blob, bd_t *bd)
{
	phys_addr_t base;
	phys_size_t size;

	ft_cpu_setup(blob, bd);

	base = getenv_bootm_low();
	size = getenv_bootm_size();

	fdt_fixup_memory(blob, (u64)base, (u64)size);

#ifdef CONFIG_PCI
	pci_of_setup(blob, bd);
#endif

	fdt_fixup_liodn(blob);

#ifdef CONFIG_HAS_FSL_DR_USB
	fdt_fixup_dr_usb(blob, bd);
#endif

#ifdef CONFIG_SYS_DPAA_FMAN
	fdt_fixup_fman_ethernet(blob);
#endif

	return 0;
}
