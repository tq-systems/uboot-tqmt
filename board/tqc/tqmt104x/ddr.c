/*
 * Copyright 2015 TQ Systems GmbH
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <i2c.h>
#include <hwconfig.h>
#include <asm/mmu.h>
#include <fsl_ddr_sdram.h>
#include <fsl_ddr_dimm_params.h>
#include <asm/fsl_law.h>
#include <asm/mpc85xx_gpio.h>
#include "ddr.h"
#include "sleep.h"

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_SYS_DDR_RAW_TIMING
#if (defined(CONFIG_TQMT1042) || defined(CONFIG_TQMT1040))
/*
 * Micron MT41K256M16HA-125IT
 */
dimm_params_t ddr_raw_timing = {
        .n_ranks = 1,
        .rank_density = 2147483648u,
        .capacity = 2147483648u,
        .primary_sdram_width = 64,
        .ec_sdram_width = 8,
        .registered_dimm = 0,
        .mirrored_dimm = 0,
        .n_row_addr = 15,
        .n_col_addr = 10,
        .n_banks_per_sdram_device = 8,
        .edc_config = EDC_ECC,
        .burst_lengths_bitmask = 0x0c,

        .tckmin_x_ps = 1250,    /* CL = 11 ; CWL = 8 tCK (AVG) (JEDEC:same)*/
        .caslat_x = 0xfe << 4,  /* 5,6,7,8,9,10,11 */
        .taa_ps = 13750,        /* Internal READ command to first data tAA (JEDEC:same)*/
        .twr_ps = 15000,        /* Write recovery time tWR (JEDEC:same) */
        .trcd_ps = 13750,       /* ACTIVATE to internal READ or WRITE delay time tRCD (JEDEC:same)*/
        .trrd_ps = 7500,        /* ACTIVATE-to-ACTIVATE minimum command
				 * period x16 (2KB page size) MIN = greater
				 * of 4CK (5ns) or 7.5ns (JEDEC:same) */
        .trp_ps = 13750,        /* PRECHARGE command period tRP (JEDEC:same) */
        .tras_ps = 35000,       /* ACTIVATE-to-PRECHARGE command period tRAS (JEDEC:same, max<9*tREFI)*/    
        .trc_ps = 48750,        /* ACTIVATE-to-ACTIVATE command period tRC (JEDEC:same)*/
        .trfc_ps = 260000,      /* REFRESH-to-ACTIVATE or REFRESH command period tRFC -> 4Gb (JEDEC:same)*/
        .twtr_ps = 7500,        /* Delay from start of internal WRITE
				 * transaction to internal READ command tWTR:
				 * MIN = greater of 4CK (5ns) or 7.5ns (JEDEC:same)*/
        .trtp_ps = 7500,        /* READ-to-PRECHARGE time tRTP:
				 * MIN = greater of 4CK (5ns) or 7.5ns (JEDEC:same)*/
        .refresh_rate_ps = 3900000, /* Max average periodic refresh  tREFI (JEDEC:same)*/
        .tfaw_ps = 40000,       /* 2kB page size/x16 -> min 40ns (JEDEC:40ns) */
};
#else
#error Missing raw timing data for this board
#endif

int fsl_ddr_get_dimm_params(dimm_params_t *pdimm,
                unsigned int controller_number,
                unsigned int dimm_number)
{
        const char dimm_model[] = "Fixed DDR on board";

        if ((controller_number == 0) && (dimm_number == 0)) {
                memcpy(pdimm, &ddr_raw_timing, sizeof(dimm_params_t));
                memset(pdimm->mpart, 0, sizeof(pdimm->mpart));
                memcpy(pdimm->mpart, dimm_model, sizeof(dimm_model) - 1);
        }

        return 0;
}
#endif /* CONFIG_SYS_DDR_RAW_TIMING */

void fsl_ddr_board_options(memctl_options_t *popts,
				dimm_params_t *pdimm,
				unsigned int ctrl_num)
{
	const struct board_specific_parameters *pbsp, *pbsp_highest = NULL;
	ulong ddr_freq;

	if (ctrl_num > 1) {
		printf("Not supported controller number %d\n", ctrl_num);
		return;
	}
	if (!pdimm->n_ranks)
		return;

	pbsp = udimms[0];

	/* Get clk_adjust according to the board ddr
	 * freqency and n_banks specified in board_specific_parameters table.
	 */
	ddr_freq = get_ddr_freq(0) / 1000000;
	while (pbsp->datarate_mhz_high) {
		if (pbsp->n_ranks == pdimm->n_ranks &&
		    (pdimm->rank_density >> 30) >= pbsp->rank_gb) {
			if (ddr_freq <= pbsp->datarate_mhz_high) {
				popts->clk_adjust = pbsp->clk_adjust;
				popts->wrlvl_start = pbsp->wrlvl_start;
				popts->wrlvl_ctl_2 = pbsp->wrlvl_ctl_2;
				popts->wrlvl_ctl_3 = pbsp->wrlvl_ctl_3;
				goto found;
			}
			pbsp_highest = pbsp;
		}
		pbsp++;
	}

	if (pbsp_highest) {
		printf("Error: board specific timing not found\n");
		printf("for data rate %lu MT/s\n", ddr_freq);
		printf("Trying to use the highest speed (%u) parameters\n",
		       pbsp_highest->datarate_mhz_high);
		popts->clk_adjust = pbsp_highest->clk_adjust;
		popts->wrlvl_start = pbsp_highest->wrlvl_start;
		popts->wrlvl_ctl_2 = pbsp->wrlvl_ctl_2;
		popts->wrlvl_ctl_3 = pbsp->wrlvl_ctl_3;
	} else {
		panic("DIMM is not supported by this board");
	}
found:
	debug("Found timing match: n_ranks %d, data rate %d, rank_gb %d\n"
		"\tclk_adjust %d, wrlvl_start %d, wrlvl_ctrl_2 0x%x, "
		"wrlvl_ctrl_3 0x%x\n",
		pbsp->n_ranks, pbsp->datarate_mhz_high, pbsp->rank_gb,
		pbsp->clk_adjust, pbsp->wrlvl_start, pbsp->wrlvl_ctl_2,
		pbsp->wrlvl_ctl_3);

	/*
	 * Write leveling override
	 */
	popts->wrlvl_override = 1;
	popts->wrlvl_sample = 0x12;                  /* min. 12 */

	/*
	 * rtt and rtt_wr override
	 */
	popts->rtt_override = 1;
	popts->rtt_wr_override_value = 0;            /* Rtt(WR) */
	popts->rtt_override_value = DDR3_RTT_40_OHM; /* Rtt(nom) */
	popts->half_strength_driver_enable = 1;      /* half-strength driver enable: 0 -> 18 Ohm; 1 -> 27 Ohm */
	popts->quad_rank_present = 1;                /* output driver strength: 0 -> 40 Ohm ; 1 -> 34 Ohm */

	/* Enable ZQ calibration */
	popts->zq_en = 1;

	/* DHC_EN =1, ODT = 75 Ohm */
	popts->ddr_cdr1 = DDR_CDR1_DHC_EN | DDR_CDR1_ODT(DDR_CDR_ODT_75ohm);
	popts->ddr_cdr2 = DDR_CDR2_ODT(DDR_CDR_ODT_75ohm);
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
	puts("Initializing....using SPD\n");

	dram_size = fsl_ddr_sdram();

	dram_size = setup_ddr_tlbs(dram_size / 0x100000);
	dram_size *= 0x100000;

#else
	dram_size =  fsl_ddr_sdram_size();
#endif

#if defined(CONFIG_DEEP_SLEEP) && !defined(CONFIG_SPL_BUILD)
	fsl_dp_resume();
#endif

	return dram_size;
}
