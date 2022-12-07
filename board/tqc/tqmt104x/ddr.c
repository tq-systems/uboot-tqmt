/*
 * Copyright 2015 TQ Systems GmbH
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/mmu.h>
#include <fsl_ddr_sdram.h>
#include <asm/fsl_law.h>
#include "ddr.h"
#include "sleep.h"

DECLARE_GLOBAL_DATA_PTR;

phys_size_t fixed_sdram(void)
{
	size_t ddr_size;
	fsl_ddr_cfg_regs_t ddr_cfg_regs = {
		.cs[0].bnds = DDR_CS0_BNDS,
		.cs[0].config = DDR_CS0_CONFIG,
		.cs[0].config_2 = DDR_CS0_CONFIG2,
		.timing_cfg_3 = DDR_TIMING_CFG_3,
		.timing_cfg_0 = DDR_TIMING_CFG_0,
		.timing_cfg_1 = DDR_TIMING_CFG_1,
		.timing_cfg_2 = DDR_TIMING_CFG_2,
		.ddr_sdram_cfg = DDR_SDRAM_CFG,
		.ddr_sdram_cfg_2 = DDR_SDRAM_CFG_2,
		.ddr_sdram_mode = DDR_SDRAM_MODE,
		.ddr_sdram_mode_2 = DDR_SDRAM_MODE_2,
		.ddr_sdram_md_cntl = DDR_SDRAM_MD_CNTL,
		.ddr_sdram_interval = DDR_SDRAM_INTERVAL,
		.ddr_data_init = DDR_DATA_INIT,
		.ddr_sdram_clk_cntl = DDR_SDRAM_CLK_CNTL,
		.ddr_init_addr = DDR_INIT_ADDR,
		.ddr_init_ext_addr = DDR_INIT_EXT_ADDR,
		.timing_cfg_4 = DDR_TIMING_CFG_4,
		.timing_cfg_5 = DDR_TIMING_CFG_5,
		.ddr_zq_cntl = DDR_DDR_ZQ_CNTL,
		.ddr_wrlvl_cntl = DDR_DDR_WRLVL_CNTL,
		.ddr_wrlvl_cntl_2 = DDR_DDR_WRLVL_CNTL_2,
		.ddr_wrlvl_cntl_2 = DDR_DDR_WRLVL_CNTL_3,
		.ddr_cdr1 = DDR_DDR_CDR1,
		.ddr_cdr2 = DDR_DDR_CDR2,
		.ddr_sr_cntr = DDR_SR_CNTR,
		.ddr_sdram_rcw_1 = DDR_SDRAM_RCW_1,
		.ddr_sdram_rcw_2 = DDR_SDRAM_RCW_2
	};

	fsl_ddr_set_memctl_regs(&ddr_cfg_regs, 0, 0);
	ddr_size = (phys_size_t)CONFIG_SYS_SDRAM_SIZE * 1024 * 1024;
	if (set_ddr_laws(CONFIG_SYS_DDR_SDRAM_BASE,
			 ddr_size, LAW_TRGT_IF_DDR_1) < 0) {
		printf("ERROR setting Local Access Windows for DDR\n");
		return 0;
	};
	return ddr_size;
}

#if defined(CONFIG_DEEP_SLEEP)
void board_mem_sleep_setup(void)
{
	/* Disable MCKE isolation through system controller */
	gpio_set_value(29, 0);
	udelay(1);
}
#endif

phys_size_t initdram(int board_type)
{
	phys_size_t dram_size;

#if defined(CONFIG_SPL_BUILD) || !defined(CONFIG_RAMBOOT_PBL)
        puts("Initializing....using fixed ddr timings\n");
        dram_size = fixed_sdram();

	dram_size = setup_ddr_tlbs(dram_size / 0x100000);
	dram_size *= 0x100000;

#else
	dram_size =  (phys_size_t) CONFIG_SYS_SDRAM_SIZE * 1024 * 1024;
#endif /* CONFIG_SPL_BUILD || !CONFIG_RAMBOOT_PBL */

#if defined(CONFIG_DEEP_SLEEP) && !defined(CONFIG_SPL_BUILD)
	fsl_dp_resume();
#endif

	return dram_size;
}
