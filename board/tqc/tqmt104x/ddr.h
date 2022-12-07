/*
 * Copyright 2015 TQ Systems GmbH.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __DDR_H__
#define __DDR_H__


#define DDR_SDRAM_CFG                   0x67040008
#define DDR_CS0_BNDS                    0x0000007F
#define DDR_CS0_CONFIG                  0x80044302
#define DDR_CS0_CONFIG2                 0x00000000
#define DDR_TIMING_CFG_0                0x0064400C
#define DDR_TIMING_CFG_1                0xBCB48C56
#define DDR_TIMING_CFG_2                0x0040C120
#define DDR_TIMING_CFG_3                0x010C1000
#define DDR_TIMING_CFG_4                0x00000001
#define DDR_TIMING_CFG_5                0x04401400
#define DDR_SDRAM_CFG_2                 0x00401010
#define DDR_SDRAM_MODE                  0x00441C70
#define DDR_SDRAM_MODE_2                0x00180000
#define DDR_SDRAM_INTERVAL              0x0C300100
#define DDR_DDR_WRLVL_CNTL              0x8645C607
#define DDR_DDR_WRLVL_CNTL_2            0x09050600
#define DDR_DDR_WRLVL_CNTL_3            0x06050600
#define DDR_DDR_CDR1                    0x80040000
#define DDR_DDR_CDR2                    0x00000001
#define DDR_SDRAM_CLK_CNTL              0x02000000
#define DDR_DDR_ZQ_CNTL                 0x89080600
#define DDR_DATA_INIT                   0xDEADBEEF
#define DDR_SDRAM_MD_CNTL               0x00000000
#define DDR_SR_CNTR                     0x00000000
#define DDR_SDRAM_RCW_1                 0x00000000
#define DDR_SDRAM_RCW_2                 0x00000000
#define DDR_INIT_ADDR                   0x00000000
#define DDR_INIT_EXT_ADDR               0x00000000
#define DDR_SDRAM_CFG_MEM_EN            0x80000000

#endif
