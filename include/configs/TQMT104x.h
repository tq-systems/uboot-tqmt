/*
 * Copyright 2015 TQ Systems GmbH
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_TQMT104x_H
#define __CONFIG_TQMT104x_H


#include <linux/kconfig.h>

/*
 * TQMT104x board configuration file
 */
#define CONFIG_PHYS_64BIT
#define CONFIG_SYS_GENERIC_BOARD
#define CONFIG_DISPLAY_BOARDINFO

#define CONFIG_BOOKE
#define CONFIG_E500			/* BOOKE e500 family */
#define CONFIG_E500MC			/* BOOKE e500mc family */
#define CONFIG_SYS_BOOK3E_HV		/* Category E.HV supported */
#define CONFIG_MP			/* support multiple processors */
#include <asm/config_mpc85xx.h>

#define CONFIG_PCI	/* Enable PCI/PCIE */

#ifdef CONFIG_RAMBOOT_PBL
#define CONFIG_SYS_FSL_PBL_PBI $(SRCTREE)/board/tqc/tqmt104x/tqmt104x_pbi.cfg
#define CONFIG_SYS_FSL_PBL_RCW CONFIG_SYS_FSL_RCW /* use same configuration for RAMBOOT and NOR boot */

#define CONFIG_SPL_MPC8XXX_INIT_DDR_SUPPORT
#define CONFIG_SPL_ENV_SUPPORT
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_FLUSH_IMAGE
#define CONFIG_SPL_TARGET		"u-boot-with-spl.bin"
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_I2C_SUPPORT
#define CONFIG_SPL_DRIVERS_MISC_SUPPORT
#define CONFIG_FSL_LAW                 /* Use common FSL init code */
#define CONFIG_SYS_TEXT_BASE		0x30001000
#define CONFIG_SPL_TEXT_BASE		0xFFFD8000
#define CONFIG_SPL_PAD_TO		0x40000
#define CONFIG_SPL_MAX_SIZE		0x28000
#ifdef CONFIG_SPL_BUILD
#define CONFIG_SPL_SKIP_RELOCATE
#define CONFIG_SPL_COMMON_INIT_DDR
#define CONFIG_SYS_CCSR_DO_NOT_RELOCATE
#define CONFIG_SYS_NO_FLASH
#endif
#define RESET_VECTOR_OFFSET		0x27FFC
#define BOOT_PAGE_OFFSET		0x27000

#ifdef CONFIG_SDCARD
#define	CONFIG_RESET_VECTOR_ADDRESS		0x30000FFC
#define CONFIG_SPL_MMC_SUPPORT
#define CONFIG_SPL_MMC_MINIMAL
#define CONFIG_SYS_MMC_U_BOOT_SIZE	(768 << 10)
#define CONFIG_SYS_MMC_U_BOOT_DST	(0x30000000)
#define CONFIG_SYS_MMC_U_BOOT_START	(0x30000000)
#define CONFIG_SYS_MMC_U_BOOT_OFFS	(260 << 10)
#define CONFIG_SYS_LDSCRIPT	"arch/powerpc/cpu/mpc85xx/u-boot.lds"
#ifndef CONFIG_SPL_BUILD
#define	CONFIG_SYS_MPC85XX_NO_RESETVEC
#endif
#define CONFIG_SPL_MMC_BOOT
#endif
#else
#define CONFIG_FSL_RCW_BIN

#ifndef CONFIG_SYS_FSL_RCW
#error "RCW configuration file not selected (CONFIG_SYS_FSL_RCW not defined)!!!"
#endif

#endif

#define CONFIG_FAT_WRITE

/* support deep sleep */
#undef CONFIG_DEEP_SLEEP /* TODO: disabled for bringup */
#if defined(CONFIG_DEEP_SLEEP)
#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_SILENT_CONSOLE
#endif

#ifndef CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_TEXT_BASE	0xeff40000
#endif

#ifndef CONFIG_RESET_VECTOR_ADDRESS
#define CONFIG_RESET_VECTOR_ADDRESS	0xeffffffc
#endif

#define CONFIG_SYS_FSL_CPC		/* Corenet Platform Cache */
#define CONFIG_SYS_NUM_CPC		CONFIG_NUM_DDR_CONTROLLERS
#define CONFIG_FSL_IFC			/* Enable IFC Support */
#define CONFIG_FSL_CAAM			/* Enable SEC/CAAM */


#define CONFIG_PCI_INDIRECT_BRIDGE
#define CONFIG_PCIE1			/* PCIE controler 1 */
#define CONFIG_PCIE2			/* PCIE controler 2 */
#define CONFIG_PCIE3			/* PCIE controler 3 */
#define CONFIG_PCIE4			/* PCIE controler 4 */

#ifdef CONFIG_PCI
#define CONFIG_FSL_PCI_INIT		/* Use common FSL init code */
#define CONFIG_SYS_PCI_64BIT		/* enable 64-bit PCI resources */
#endif

#define CONFIG_FSL_LAW			/* Use common FSL init code */

#define CONFIG_ENV_OVERWRITE

#ifndef CONFIG_SYS_NO_FLASH
#define CONFIG_FLASH_CFI_DRIVER
#define CONFIG_SYS_FLASH_CFI
#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE
#endif

#if defined(CONFIG_SDCARD)
#define CONFIG_SYS_EXTRA_ENV_RELOC
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV          0
#define CONFIG_ENV_SIZE			0x2000
#define CONFIG_ENV_OFFSET		(512 * 0x800)
#else
#define CONFIG_ENV_IS_IN_FLASH
#define CONFIG_ENV_ADDR		(CONFIG_SYS_MONITOR_BASE - CONFIG_ENV_SECT_SIZE)
#define CONFIG_ENV_SIZE		0x2000
#define CONFIG_ENV_SECT_SIZE	0x20000 /* 128K (one sector) */
#endif

#define CONFIG_SYS_CLK_FREQ	100000000
#define CONFIG_DDR_CLK_FREQ	100000000

/*
 * These can be toggled for performance analysis, otherwise use default.
 */
#define CONFIG_SYS_CACHE_STASHING
#define CONFIG_BACKSIDE_L2_CACHE
#define CONFIG_SYS_INIT_L2CSR0		L2CSR0_L2E
#define CONFIG_BTB			/* toggle branch predition */
#define CONFIG_DDR_ECC
#ifdef CONFIG_DDR_ECC
#define CONFIG_ECC_INIT_VIA_DDRCONTROLLER
#define CONFIG_MEM_INIT_VALUE		0xdeadbeef
#endif

#define CONFIG_ENABLE_36BIT_PHYS

#define CONFIG_ADDR_MAP
#define CONFIG_SYS_NUM_ADDR_MAP		64	/* number of TLB1 entries */

#define CONFIG_SYS_MEMTEST_START	0x00200000	/* memtest works on */
#define CONFIG_SYS_MEMTEST_END		0x00400000
#define CONFIG_SYS_ALT_MEMTEST
#define CONFIG_PANIC_HANG	/* do not reset board on panic */

/*
 *  Config the L3 Cache as L3 SRAM
 */
#define CONFIG_SYS_INIT_L3_ADDR		0xFFFC0000
#define CONFIG_SYS_L3_SIZE		256 << 10
#define CONFIG_SPL_GD_ADDR		(CONFIG_SYS_INIT_L3_ADDR + 32 * 1024)
#ifdef CONFIG_RAMBOOT_PBL
#define CONFIG_ENV_ADDR			(CONFIG_SPL_GD_ADDR + 4 * 1024)
#endif
#define CONFIG_SPL_RELOC_MALLOC_ADDR	(CONFIG_SPL_GD_ADDR + 12 * 1024)
#define CONFIG_SPL_RELOC_MALLOC_SIZE	(30 << 10)
#define CONFIG_SPL_RELOC_STACK		(CONFIG_SPL_GD_ADDR + 64 * 1024)
#define CONFIG_SPL_RELOC_STACK_SIZE	(22 << 10)

#define CONFIG_SYS_DCSRBAR		0xf0000000
#define CONFIG_SYS_DCSRBAR_PHYS		0xf00000000ull

/*
 * DDR Setup
 */
#define CONFIG_VERY_BIG_RAM
#define CONFIG_SYS_DDR_SDRAM_BASE	0x00000000
#define CONFIG_SYS_SDRAM_BASE		CONFIG_SYS_DDR_SDRAM_BASE

/* CONFIG_NUM_DDR_CONTROLLERS is defined in include/asm/config_mpc85xx.h */
#define CONFIG_DIMM_SLOTS_PER_CTLR	1
#define CONFIG_CHIP_SELECTS_PER_CTRL	1

#define CONFIG_SYS_FSL_DDR3
#define CONFIG_SYS_SDRAM_SIZE  2048    /* for fixed parameter use */

/*
 * IFC Definitions
 */
#ifndef CONFIG_SYS_NO_FLASH
#define CONFIG_SYS_FLASH_BASE	0xe8000000
#define CONFIG_SYS_FLASH_BASE_PHYS	(0xf00000000ull | CONFIG_SYS_FLASH_BASE)

#define CONFIG_SYS_NOR_CSPR_EXT	CSPR_EXT_PHYS_ADDR(CONFIG_SYS_FLASH_BASE_PHYS)
#define CONFIG_SYS_NOR_CSPR	(CSPR_PHYS_ADDR(CONFIG_SYS_FLASH_BASE_PHYS) | \
				CSPR_PORT_SIZE_16 | \
				CSPR_MSEL_NOR | \
				CSPR_V)
#define CONFIG_SYS_NOR_AMASK	IFC_AMASK(128*1024*1024)

/* NOR Flash Timing Params */
#define CONFIG_SYS_NOR_CSOR	CSOR_NOR_TRHZ_20
#define CONFIG_SYS_NOR_FTIM0	(FTIM0_NOR_TACSE(0x1) | \
				FTIM0_NOR_TEADC(0x4) | \
				FTIM0_NOR_TEAHC(0x3))
#define CONFIG_SYS_NOR_FTIM1	(FTIM1_NOR_TACO(0x1) | \
				FTIM1_NOR_TRAD_NOR(0x27) |\
				FTIM1_NOR_TSEQRAD_NOR(0x00))
#define CONFIG_SYS_NOR_FTIM2	(FTIM2_NOR_TCS(0x1) | \
				FTIM2_NOR_TCH(0x1) | \
				FTIM2_NOR_TWPH(0x00) | \
				FTIM2_NOR_TWP(0x21))
#define CONFIG_SYS_NOR_FTIM3	0x0

#define CONFIG_SYS_FLASH_QUIET_TEST
#define CONFIG_FLASH_SHOW_PROGRESS	45 /* count down from 45/5: 9..1 */

#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* number of banks */
#define CONFIG_SYS_MAX_FLASH_SECT	1024	/* sectors per device */
#define CONFIG_SYS_FLASH_ERASE_TOUT	60000	/* Flash Erase Timeout (ms) */
#define CONFIG_SYS_FLASH_WRITE_TOUT	500	/* Flash Write Timeout (ms) */

#define CONFIG_SYS_FLASH_EMPTY_INFO
#define CONFIG_SYS_FLASH_BANKS_LIST	{CONFIG_SYS_FLASH_BASE_PHYS}


#define CONFIG_SYS_CSPR0_EXT		CONFIG_SYS_NOR_CSPR_EXT
#define CONFIG_SYS_CSPR0		CONFIG_SYS_NOR_CSPR
#define CONFIG_SYS_AMASK0		CONFIG_SYS_NOR_AMASK
#define CONFIG_SYS_CSOR0		CONFIG_SYS_NOR_CSOR
#define CONFIG_SYS_CS0_FTIM0		CONFIG_SYS_NOR_FTIM0
#define CONFIG_SYS_CS0_FTIM1		CONFIG_SYS_NOR_FTIM1
#define CONFIG_SYS_CS0_FTIM2		CONFIG_SYS_NOR_FTIM2
#define CONFIG_SYS_CS0_FTIM3		CONFIG_SYS_NOR_FTIM3
#endif

/*
 * TDM Definition
 */
#define T1040_TDM_QUIRK_CCSR_BASE	0xfe000000

#ifdef CONFIG_SPL_BUILD
#define CONFIG_SYS_MONITOR_BASE	CONFIG_SPL_TEXT_BASE
#else
#define CONFIG_SYS_MONITOR_BASE	CONFIG_SYS_TEXT_BASE	/* start of monitor */
#endif

#if defined(CONFIG_RAMBOOT_PBL)
#define CONFIG_SYS_RAMBOOT
#endif

#define CONFIG_BOARD_EARLY_INIT_R
#define CONFIG_MISC_INIT_R

#define CONFIG_HWCONFIG

/* define to use L1 as initial stack */
#define CONFIG_L1_INIT_RAM
#define CONFIG_SYS_INIT_RAM_LOCK
#define CONFIG_SYS_INIT_RAM_ADDR	0xfdd00000	/* Initial L1 address */
#define CONFIG_SYS_INIT_RAM_ADDR_PHYS_HIGH	0xf
#define CONFIG_SYS_INIT_RAM_ADDR_PHYS_LOW	0xfe0ec000
/* The assembler doesn't like typecast */
#define CONFIG_SYS_INIT_RAM_ADDR_PHYS \
	((CONFIG_SYS_INIT_RAM_ADDR_PHYS_HIGH * 1ull << 32) | \
	  CONFIG_SYS_INIT_RAM_ADDR_PHYS_LOW)
#define CONFIG_SYS_INIT_RAM_SIZE		0x00004000

#define CONFIG_SYS_GBL_DATA_OFFSET	(CONFIG_SYS_INIT_RAM_SIZE - \
					GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_OFFSET	CONFIG_SYS_GBL_DATA_OFFSET

#define CONFIG_SYS_MONITOR_LEN		(768 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(4 * 1024 * 1024)

/* Serial Port - controlled on board with jumper J8
 * open - index 2
 * shorted - index 1
 */
#define CONFIG_CONS_INDEX	1
#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	1
#define CONFIG_SYS_NS16550_CLK		(get_bus_freq(0)/2)

#define CONFIG_SYS_BAUDRATE_TABLE	\
	{300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200}

#define CONFIG_SYS_NS16550_COM1	(CONFIG_SYS_CCSRBAR+0x11C500)
#define CONFIG_SYS_NS16550_COM2	(CONFIG_SYS_CCSRBAR+0x11C600)
#define CONFIG_SYS_NS16550_COM3	(CONFIG_SYS_CCSRBAR+0x11D500)
#define CONFIG_SYS_NS16550_COM4	(CONFIG_SYS_CCSRBAR+0x11D600)
#define CONFIG_SERIAL_MULTI		/* Enable both serial ports */
#ifndef CONFIG_SPL_BUILD
#define CONFIG_SYS_CONSOLE_IS_IN_ENV	/* determine from environment */
#endif

/* Use the HUSH parser */
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2 "> "

/* Video */
#define CONFIG_FSL_DIU_FB

#ifdef CONFIG_FSL_DIU_FB
#define CONFIG_FSL_DIU_CH7301
#define CONFIG_SYS_DIU_ADDR	(CONFIG_SYS_CCSRBAR + 0x180000)
#define CONFIG_VIDEO
#define CONFIG_CMD_BMP
#define CONFIG_CFB_CONSOLE
#define CONFIG_CFB_CONSOLE_ANSI
#define CONFIG_VIDEO_SW_CURSOR
#define CONFIG_VGA_AS_SINGLE_DEVICE
#define CONFIG_VIDEO_LOGO
#define CONFIG_VIDEO_BMP_LOGO
#endif

/* pass open firmware flat tree */
#define CONFIG_OF_LIBFDT
#define CONFIG_OF_BOARD_SETUP
#define CONFIG_OF_STDOUT_VIA_ALIAS

/* new uImage format support */
#define CONFIG_FIT
#define CONFIG_FIT_VERBOSE	/* enable fit_format_{error,warning}() */

/* I2C */
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_FSL		/* Use FSL common I2C driver */
#define CONFIG_SYS_FSL_I2C_SPEED	400000	/* I2C speed in Hz */
#define CONFIG_SYS_FSL_I2C2_SPEED	400000
#define CONFIG_SYS_FSL_I2C3_SPEED	400000
#define CONFIG_SYS_FSL_I2C4_SPEED	400000
#define CONFIG_SYS_FSL_I2C_SLAVE	0x7F
#define CONFIG_SYS_FSL_I2C2_SLAVE	0x7F
#define CONFIG_SYS_FSL_I2C3_SLAVE	0x7F
#define CONFIG_SYS_FSL_I2C4_SLAVE	0x7F
#define CONFIG_SYS_FSL_I2C_OFFSET	0x118000
#define CONFIG_SYS_FSL_I2C2_OFFSET	0x118100
#define CONFIG_SYS_FSL_I2C3_OFFSET	0x119000
#define CONFIG_SYS_FSL_I2C4_OFFSET	0x119100

/*
 * RTC configuration
 */
#define RTC
#if defined RTC
#define CONFIG_CMD_DATE
#define CONFIG_RTC_PCF85063             1
#define CONFIG_SYS_I2C_RTC_ADDR         0x51
#endif

/*
 * General PCI
 * Memory space is mapped 1-1, but I/O space must start from 0.
 */

#ifdef CONFIG_PCI
/* controller 1, direct to uli, tgtid 3, Base address 20000 */
#ifdef CONFIG_PCIE1
#define	CONFIG_SYS_PCIE1_MEM_VIRT	0x80000000
#define	CONFIG_SYS_PCIE1_MEM_BUS	0xe0000000
#define	CONFIG_SYS_PCIE1_MEM_PHYS	0xc00000000ull
#define CONFIG_SYS_PCIE1_MEM_SIZE	0x10000000	/* 256M */
#define CONFIG_SYS_PCIE1_IO_VIRT	0xf8000000
#define CONFIG_SYS_PCIE1_IO_BUS		0x00000000
#define CONFIG_SYS_PCIE1_IO_PHYS	0xff8000000ull
#define CONFIG_SYS_PCIE1_IO_SIZE	0x00010000	/* 64k */
#endif

/* controller 2, Slot 2, tgtid 2, Base address 201000 */
#ifdef CONFIG_PCIE2
#define CONFIG_SYS_PCIE2_MEM_VIRT	0x90000000
#define CONFIG_SYS_PCIE2_MEM_BUS	0xe0000000
#define CONFIG_SYS_PCIE2_MEM_PHYS	0xc10000000ull
#define CONFIG_SYS_PCIE2_MEM_SIZE	0x10000000	/* 256M */
#define CONFIG_SYS_PCIE2_IO_VIRT	0xf8010000
#define CONFIG_SYS_PCIE2_IO_BUS		0x00000000
#define CONFIG_SYS_PCIE2_IO_PHYS	0xff8010000ull
#define CONFIG_SYS_PCIE2_IO_SIZE	0x00010000	/* 64k */
#endif

/* controller 3, Slot 1, tgtid 1, Base address 202000 */
#ifdef CONFIG_PCIE3
#define CONFIG_SYS_PCIE3_MEM_VIRT	0xa0000000
#define CONFIG_SYS_PCIE3_MEM_BUS	0xe0000000
#define CONFIG_SYS_PCIE3_MEM_PHYS	0xc20000000ull
#define CONFIG_SYS_PCIE3_MEM_SIZE	0x10000000	/* 256M */
#define CONFIG_SYS_PCIE3_IO_VIRT	0xf8020000
#define CONFIG_SYS_PCIE3_IO_BUS		0x00000000
#define CONFIG_SYS_PCIE3_IO_PHYS	0xff8020000ull
#define CONFIG_SYS_PCIE3_IO_SIZE	0x00010000	/* 64k */
#endif

/* controller 4, Base address 203000 */
#ifdef CONFIG_PCIE4
#define CONFIG_SYS_PCIE4_MEM_VIRT	0xb0000000
#define CONFIG_SYS_PCIE4_MEM_BUS	0xe0000000
#define CONFIG_SYS_PCIE4_MEM_PHYS	0xc30000000ull
#define CONFIG_SYS_PCIE4_MEM_SIZE	0x10000000	/* 256M */
#define CONFIG_SYS_PCIE4_IO_VIRT	0xf8030000
#define CONFIG_SYS_PCIE4_IO_BUS		0x00000000
#define CONFIG_SYS_PCIE4_IO_PHYS	0xff8030000ull
#define CONFIG_SYS_PCIE4_IO_SIZE	0x00010000	/* 64k */
#endif

#define CONFIG_PCI_PNP			/* do pci plug-and-play */
#define CONFIG_E1000

#define CONFIG_PCI_SCAN_SHOW		/* show pci devices on startup */
#define CONFIG_DOS_PARTITION
#endif	/* CONFIG_PCI */

/* SATA */
#define CONFIG_FSL_SATA_V2
#ifdef CONFIG_FSL_SATA_V2
#define CONFIG_LIBATA
#define CONFIG_FSL_SATA

#define CONFIG_SYS_SATA_MAX_DEVICE	1
#define CONFIG_SATA1
#define CONFIG_SYS_SATA1		CONFIG_SYS_MPC85xx_SATA1_ADDR
#define CONFIG_SYS_SATA1_FLAGS		FLAGS_DMA

#define CONFIG_LBA48
#define CONFIG_CMD_SATA
#define CONFIG_DOS_PARTITION
#define CONFIG_CMD_EXT2
#endif

/*
* USB
*/
#define CONFIG_HAS_FSL_DR_USB

#ifdef CONFIG_HAS_FSL_DR_USB
#define CONFIG_USB_EHCI

#ifdef CONFIG_USB_EHCI
#define CONFIG_CMD_USB
#define CONFIG_USB_STORAGE
#define CONFIG_USB_EHCI_FSL
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_CMD_EXT2
#endif
#endif

#define CONFIG_MMC

#ifdef CONFIG_MMC
#define CONFIG_FSL_ESDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR       CONFIG_SYS_MPC85xx_ESDHC_ADDR
#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION
#endif

/* Qman/Bman */
#ifndef CONFIG_NOBQFMAN
#define CONFIG_SYS_DPAA_QBMAN		/* Support Q/Bman */
#define CONFIG_SYS_BMAN_NUM_PORTALS	10
#define CONFIG_SYS_BMAN_MEM_BASE	0xf4000000
#define CONFIG_SYS_BMAN_MEM_PHYS	0xff4000000ull
#define CONFIG_SYS_BMAN_MEM_SIZE	0x02000000
#define CONFIG_SYS_BMAN_SP_CENA_SIZE    0x4000
#define CONFIG_SYS_BMAN_SP_CINH_SIZE    0x1000
#define CONFIG_SYS_BMAN_CENA_BASE       CONFIG_SYS_BMAN_MEM_BASE
#define CONFIG_SYS_BMAN_CENA_SIZE       (CONFIG_SYS_BMAN_MEM_SIZE >> 1)
#define CONFIG_SYS_BMAN_CINH_BASE       (CONFIG_SYS_BMAN_MEM_BASE + \
					CONFIG_SYS_BMAN_CENA_SIZE)
#define CONFIG_SYS_BMAN_CINH_SIZE       (CONFIG_SYS_BMAN_MEM_SIZE >> 1)
#define CONFIG_SYS_BMAN_SWP_ISDR_REG	0xE08
#define CONFIG_SYS_QMAN_NUM_PORTALS	10
#define CONFIG_SYS_QMAN_MEM_BASE	0xf6000000
#define CONFIG_SYS_QMAN_MEM_PHYS	0xff6000000ull
#define CONFIG_SYS_QMAN_MEM_SIZE	0x02000000
#define CONFIG_SYS_QMAN_SP_CENA_SIZE    0x4000
#define CONFIG_SYS_QMAN_SP_CINH_SIZE    0x1000
#define CONFIG_SYS_QMAN_CENA_BASE       CONFIG_SYS_QMAN_MEM_BASE
#define CONFIG_SYS_QMAN_CENA_SIZE       (CONFIG_SYS_QMAN_MEM_SIZE >> 1)
#define CONFIG_SYS_QMAN_CINH_BASE       (CONFIG_SYS_QMAN_MEM_BASE + \
					CONFIG_SYS_QMAN_CENA_SIZE)
#define CONFIG_SYS_QMAN_CINH_SIZE       (CONFIG_SYS_QMAN_MEM_SIZE >> 1)
#define CONFIG_SYS_QMAN_SWP_ISDR_REG	0xE08

#define  CONFIG_SYS_DPAA_FMAN
#define CONFIG_SYS_DPAA_PME

#define CONFIG_QE
#define CONFIG_U_QE

#if defined(CONFIG_SDCARD)
/*
 * PBL SD boot image should stored at 0x1000(8 blocks), the size of the image is
 * about 1MB (2048 blocks), Env is stored after the image, and the env size is
 * 0x2000 (16 blocks), 8 + 2048 + 16 = 2072, enlarge it to 2080.
 */
#define CONFIG_SYS_QE_FMAN_FW_IN_MMC
#define CONFIG_SYS_FMAN_FW_ADDR	(512 * 0x820)
#else
#define CONFIG_SYS_QE_FMAN_FW_IN_NOR
#define CONFIG_SYS_FMAN_FW_ADDR		0xEFF00000
#endif

#if defined(CONFIG_SDCARD)
#define CONFIG_SYS_QE_FW_ADDR		(512 * 0x920)
#else
#define CONFIG_SYS_QE_FW_ADDR		0xEFF10000
#endif


#define CONFIG_SYS_QE_FMAN_FW_LENGTH	0x10000
#define CONFIG_SYS_FDT_PAD		(0x3000 + CONFIG_SYS_QE_FMAN_FW_LENGTH)
#endif /* CONFIG_NOBQFMAN */

#ifdef CONFIG_SYS_DPAA_FMAN
#define CONFIG_FMAN_ENET
#define CONFIG_PHY_VITESSE
#endif

#ifdef CONFIG_FMAN_ENET
#define CONFIG_SYS_SGMII1_PHY_ADDR		0x1D
#define CONFIG_SYS_SGMII2_PHY_ADDR		0x03
#define CONFIG_SYS_SGMII3_PHY_ADDR		0x1C
#define CONFIG_SYS_SGMII4_PHY_ADDR		0x01
#define CONFIG_SYS_RGMII1_PHY_ADDR		0x0E
#define CONFIG_SYS_RGMII2_PHY_ADDR		0x05

#define CONFIG_MII		/* MII PHY management */
#define CONFIG_ETHPRIME		"FM1@DTSEC4"
#define CONFIG_PHY_GIGE		/* Include GbE speed/duplex detection */
#endif

/*
 * Environment
 */
#define CONFIG_LOADS_ECHO		/* echo on for serial download */
#define CONFIG_SYS_LOADS_BAUD_CHANGE	/* allow baudrate change */

/*
 * Command line configuration.
 */
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_ELF
#define CONFIG_CMD_ERRATA
#define CONFIG_CMD_GREPENV
#define CONFIG_CMD_IRQ
#define CONFIG_CMD_I2C
#define CONFIG_CMD_MII
#define CONFIG_CMD_PING
#define CONFIG_CMD_REGINFO

#ifdef CONFIG_PCI
#define CONFIG_CMD_PCI
#endif

/* Hash command with SHA acceleration supported in hardware */
#ifdef CONFIG_FSL_CAAM
#define CONFIG_CMD_HASH
#define CONFIG_SHA_HW_ACCEL
#endif

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP			/* undef to save memory	*/
#define CONFIG_CMDLINE_EDITING			/* Command-line editing */
#define CONFIG_AUTO_COMPLETE			/* add autocompletion support */
#define CONFIG_SYS_LOAD_ADDR	0x2000000	/* default load address */
#ifdef CONFIG_CMD_KGDB
#define CONFIG_SYS_CBSIZE	1024		/* Console I/O Buffer Size */
#else
#define CONFIG_SYS_CBSIZE	1024		/* Console I/O Buffer Size */
#endif
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16)
#define CONFIG_SYS_MAXARGS	16		/* max number of command args */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE/* Boot Argument Buffer Size */
/* automatic reprogramming of 849N202 clock generator based on RCW */
#define CONFIG_SYS_TQMT1042_PLL1_CLKGEN_AUTOREPROG
#define CONFIG_SYS_849N202_I2C_BUSNR 0x1	/* I2C bus number where 849N202's is located */
#define CONFIG_SYS_849N202_CLKGEN_ADDR 0x6C	/* 849N202's I2C address */

/*
 * For booting Linux, the board info and command line data
 * have to be in the first 64 MB of memory, since this is
 * the maximum mapped by the Linux kernel during initialization.
 */
#define CONFIG_SYS_BOOTMAPSZ	(64 << 20)	/* Initial map for Linux*/
#define CONFIG_SYS_BOOTM_LEN	(64 << 20)	/* Increase max gunzip size */

#ifdef CONFIG_CMD_KGDB
#define CONFIG_KGDB_BAUDRATE	230400	/* speed to run kgdb serial port */
#endif

/*
 * Dynamic MTD Partition support with mtdparts
 */
#ifndef CONFIG_SYS_NO_FLASH
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_CMD_MTDPARTS
#define CONFIG_FLASH_CFI_MTD
#define MTDIDS_DEFAULT "nor0=fe8000000.nor"
#define MTDPARTS_DEFAULT	"mtdparts=fe8000000.nor:1m(uboot),5m(kernel)," \
				"128k(dtb),96m(fs),-(user);"
#endif

/*
 * Enable PCA953x I2C GPIO expander access
 */
#define CONFIG_PCA953X
#define CONFIG_CMD_PCA953X
#define CONFIG_CMD_PCA953X_INFO

/*
 * Reset pin for 88E1340 ethernet phy
 * provided by PCA953x I2C GPIO expander
 */
#define CONFIG_SYS_88E1340_RST_I2C_PCA953X_ADDR	0x20
#define CONFIG_SYS_88E1340_RST_I2C_PCA953X_GPIO	0x7

/*
 * Environment Configuration
 */
#define CONFIG_ROOTPATH		"/srv/nfs/tqmt104x"
#define CONFIG_BOOTFILE		"uImage"

/* U-Boot image on TFTP server*/
#define CONFIG_UBOOTPATH	"u-boot-nor-2015.07-tqmt-r1.bin"

/* default location for tftp and bootm */
#define CONFIG_LOADADDR		0x4000000

#define CONFIG_BOOTDELAY	5	/*-1 disables auto-boot*/

#define CONFIG_BAUDRATE	115200

#define __USB_PHY_TYPE	utmi
#define RAMDISK_FILE	"ramdisk.ext2.gz.u-boot"
/* FDT_FILE moved to TQMT1040.h resp. TQMT1042.h*/

#define FMAN_UCODE_FILE		"fsl_fman_ucode_t1040_r1.1_107_4_2.bin"
#define ROOTFS_FILE		"tq-image-generic-tqmt1042-64bit-stkt104x.jffs2"
#define WIC_FILE		"tq-image-generic-tqmt1042-64bit-stkt104x.wic"

/* RCW file name */
#ifdef CONFIG_RCW_CFG_TQMT1040_SERDES66
#define RCW_FILE		"fsl_rcw-nor-TQMT1040_SERDES66.bin"
#elif CONFIG_RCW_CFG_TQMT1042_SERDES86
#define RCW_FILE		"fsl_rcw-nor-TQMT1042_SERDES86.bin"
#elif CONFIG_RCW_CFG_TQMT1042_SERDES88
#define RCW_FILE		"fsl_rcw-nor-TQMT1042_SERDES88.bin"
#elif CONFIG_RCW_CFG_TQMT1042_SERDES8E
#define RCW_FILE		"fsl_rcw-nor-TQMT1042_SERDES8E.bin"
#else
#define RCW_FILE		""
#endif

#ifdef CONFIG_FSL_DIU_FB
#define DIU_ENVIRONMENT "video-mode=fslfb:1024x768-24@60,monitor=lvds"
#else
#define DIU_ENVIRONMENT
#endif

#define TQMT104x_UPDATE_ENV_SETINGS                                            \
	"set_getcmd=if test \"${ipmode}\" != static; then "                    \
			"setenv getcmd dhcp; setenv autoload yes; "            \
		"else setenv getcmd tftp; setenv autoload no; fi\0"            \
	"ipmode=static\0"                                                      \
	"mmcdev=0\0"                                                           \
	"fman_ucode_mmc_blk_start=0x820\0"                                     \
	"rcw_mmc_blk_start=0x8\0"                                              \
	"uboot_mmc_blk_start=0x8\0"                                            \
	"wic_mmc_blk_start=0x0\0"                                              \
	"wic_file=" __stringify(WIC_FILE) "\0"                                 \
	"nor_update=run set_getcmd; "                                          \
		"if ${getcmd} ${nor_file}; then "                              \
			"if itest ${filesize} > 0; then "                      \
				"echo updating ${nor_name}...; "               \
				"protect off ${nor_addr_flsh} +${filesize}; "  \
				"erase ${nor_addr_flsh} +${filesize}; "        \
				"cp.b ${loadaddr} "                            \
					"${nor_addr_flsh} ${filesize}; "       \
				"protect on ${nor_addr_flsh} +${filesize}; "   \
			"else echo no ${nor_name} size!; "                     \
			"fi; "                                                 \
		"else echo ${nor_name} file not found!; "                      \
		"fi; "                                                         \
		"setenv getcmd; setenv filesize; "                             \
		"setenv nor_addr_flsh; setenv nor_file; setenv nor_name\0"     \
	"update_nor_uboot=setenv nor_addr_flsh ${uboot_addr_flsh}; "           \
		"setenv nor_file ${uboot}; "                                   \
		"setenv nor_name 'u-boot'; "                                   \
		"run nor_update;\0"                                            \
	"update_nor_rcw=setenv nor_addr_flsh ${rcw_addr_flsh}; "               \
		"setenv nor_file ${rcw_file}; "                                \
		"setenv nor_name 'rcw'; "                                      \
		"run nor_update;\0"                                            \
	"update_nor_kernel=setenv nor_addr_flsh ${kernel_addr_flsh}; "         \
		"setenv nor_file ${bootfile}; "                                \
		"setenv nor_name 'kernel'; "                                   \
		"run nor_update;\0"                                            \
	"update_nor_fdt=setenv nor_addr_flsh ${fdt_addr_flsh}; "               \
		"setenv nor_file ${fdt_file}; "                                \
		"setenv nor_name 'fdt'; "                                      \
		"run nor_update;\0"                                            \
	"update_nor_rootfs=setenv nor_addr_flsh ${rootfs_addr_flsh}; "         \
		"setenv nor_file ${rootfs_file}; "                             \
		"setenv nor_name 'rootfs'; "                                   \
		"run nor_update;\0"                                            \
	"update_nor_fman_ucode=setenv nor_addr_flsh ${fman_ucode_addr_flsh}; " \
		"setenv nor_file ${fman_ucode_file}; "                         \
		"setenv nor_name 'fman_ucode'; "                               \
		"run nor_update;\0"                                            \
	"mmc_update=run set_getcmd; "                                          \
		"mmc dev ${mmcdev}; mmc rescan; "                              \
		"if ${getcmd} ${mmc_file}; then "                              \
			"if itest ${filesize} > 0; then "                      \
				"setexpr blkc ${filesize} + 0x1ff; "           \
				"setexpr blkc ${blkc} / 0x200; "               \
				"echo updating ${mmc_name}...; "               \
				"mmc write ${loadaddr} "                       \
					"${mmc_blk_start} ${blkc}; "           \
			"else echo no ${mmc_name} size!; "                     \
			"fi; "                                                 \
		"else echo ${mmc_name} file not found!; "                      \
		"fi; "                                                         \
		"setenv getcmd; setenv filesize; setenv blkc; "                \
		"setenv mmc_blk_start; setenv mmc_file; setenv mmc_name;\0"    \
	"mmc_fs_update=run set_getcmd; "                                       \
		"mmc dev ${mmcdev}; mmc rescan; "                              \
		"if ${getcmd} ${mmc_file}; then "                              \
			"if itest ${filesize} > 0; then "                      \
				"echo updating ${mmc_name}...; "               \
				"fatwrite mmc ${mmcdev}:1 ${loadaddr} ${mmc_file} ${filesize};"\
			"else echo no ${mmc_name} size!; "                     \
			"fi; "                                                 \
		"else echo ${mmc_name} file not found!; "                      \
		"fi; "                                                         \
		"setenv getcmd; setenv filesize;"                              \
		"setenv mmc_file; setenv mmc_name;\0"			       \
	"update_mmc=setenv mmc_name 'wic image'; "                             \
		"setenv mmc_blk_start ${wic_mmc_blk_start}; "                  \
		"setenv mmc_file ${wic_file}; "                                \
		"run mmc_update;\0"                                            \
	"update_mmc_uboot=setenv mmc_name 'u-boot'; "                          \
		"setenv mmc_blk_start ${uboot_mmc_blk_start}; "                \
		"setenv mmc_file ${uboot}; "                                   \
		"run mmc_update;\0"                                            \
	"update_mmc_rcw=setenv mmc_name 'rcw'; "                               \
		"setenv mmc_blk_start ${rcw_mmc_blk_start}; "                  \
		"setenv mmc_file ${rcw_file}; "                                \
		"mmc dev ${mmcdev}; mmc rescan; "                              \
		"mmc read ${loadaddr} ${rcw_mmc_blk_start} 80;"                \
		"run mmc_update;\0"                                            \
	"update_mmc_kernel=setenv mmc_name 'kernel'; "                         \
		"setenv mmc_file ${bootfile}; "                                \
		"run mmc_fs_update;\0"                                         \
	"update_mmc_fdt=setenv mmc_name 'fdt'; "                               \
		"setenv mmc_file ${fdt_file}; "                                \
		"run mmc_fs_update;\0"                                         \
	"update_mmc_rootfs=setenv mmc_name 'rootfs'; "                         \
		"setenv mmc_blk_start ${rootfs_mmc_blk_start}; "               \
		"setenv mmc_file ${rootfs_file}; "                             \
		"run mmc_fs_update;\0"                                         \
	"update_mmc_fman_ucode=setenv mmc_name 'fman_ucode'; "                 \
		"setenv mmc_blk_start ${fman_ucode_mmc_blk_start}; "           \
		"setenv mmc_file ${fman_ucode_file}; "                         \
		"run mmc_update;\0"                                            \

#define	CONFIG_EXTRA_ENV_SETTINGS				\
	"hwconfig=fsl_ddr:bank_intlv=cs0_cs1;"              	\
    "usb1:dr_mode=host,phy_type=" __stringify(__USB_PHY_TYPE) ";"\
    "usb2:dr_mode=host,phy_type=" __stringify(__USB_PHY_TYPE) "\0"\
	"netdev=fm1-mac3\0"					\
	"video-mode=" __stringify(DIU_ENVIRONMENT) "\0"		\
	"uboot=" __stringify(CONFIG_UBOOTPATH) "\0"		\
	"uboot_addr=" __stringify(CONFIG_SYS_TEXT_BASE) "\0"	\
	"uboot_addr_flsh=0xeff40000\0"				\
	"uboot_size=0xc0000\0"					\
	"consoledev=ttyS0\0"					\
	"ramdisk_addr=0x2000000\0"				\
	"ramdisk_addr_flsh=0xe8840000\0"			\
	"ramdisk_file=" __stringify(RAMDISK_FILE) "\0"		\
	"ramdisk_size=0x2500000\0"				\
	"fdt_addr=0x3e00000\0"					\
	"fdt_addr_flsh=0xe8820000\0"				\
	"fdt_file=" __stringify(FDT_FILE) "\0"			\
	"baudrate=115200\0"					\
	"ethact=FM1@DTSEC4\0"					\
	"ethprime=FM1@DTSEC4\0"					\
	"kernel_addr_flsh=0xe8020000\0"				\
	"fman_ucode_addr_flsh=0xeff00000\0"			\
	"fman_ucode_file=" __stringify(FMAN_UCODE_FILE) "\0"	\
	"rcw_file=" __stringify(RCW_FILE) "\0"			\
	"rcw_addr_flsh=0xe8000000\0"				\
	"rootfs_addr_flsh=0xe8840000\0"				\
	"rootfs_file=" __stringify(ROOTFS_FILE)"\0"		\
	"flashboot=" CONFIG_HDBOOT "\0"				\
	"mmcboot=" CONFIG_MMCBOOT "\0"				\
	"nfsboot=" CONFIG_NFSBOOTCOMMAND "\0"			\
	"norboot=" CONFIG_NORBOOTCOMMAND "\0"			\
	"sdboot=" CONFIG_SDCARDBOOTCOMMAND "\0"			\
	TQMT104x_UPDATE_ENV_SETINGS

#define CONFIG_LINUX

#define CONFIG_HDBOOT                       			\
	"setenv bootargs root=/dev/ram rw "            		\
	"console=$consoledev,$baudrate $othbootargs "  		\
	"ramdisk_size=$ramdisk_size; "	    			\
	"bootm $kernel_addr_flsh $ramdisk_addr_flsh "		\
	"$fdt_addr_flsh"

#define CONFIG_MMCBOOT						\
	"setenv bootargs root=/dev/mmcblk0p1 rw "		\
	"rootfstype=ext2 rootwait "				\
	"ip=$ipaddr:$serverip:$gatewayip:$netmask:$hostname:$netdev:off " \
	"console=$consoledev,$baudrate $othbootargs; "		\
	"ext2load mmc 0:1 $fdt_addr $fdt_file; "		\
	"ext2load mmc 0:1 $loadaddr $bootfile; " 		\
	"bootm $loadaddr - $fdt_addr"

#define CONFIG_NFSBOOTCOMMAND					\
	"setenv bootargs root=/dev/nfs rw "			\
	"nfsroot=$serverip:$rootpath "				\
	"ip=$ipaddr:$serverip:$gatewayip:$netmask:$hostname:$netdev:off " \
	"console=$consoledev,$baudrate $othbootargs; "		\
	"nfs $loadaddr $serverip:$rootpath/$bootfile; "	\
	"nfs $fdt_addr $serverip:$rootpath/$fdt_file; "		\
	"bootm $loadaddr - $fdt_addr"

#define CONFIG_NORBOOTCOMMAND \
	"setenv bootargs root=/dev/mtdblock3 rootfstype=jffs2 rootwait rw " \
	"ip=$ipaddr:$serverip:$gatewayip:$netmask:$hostname:$netdev:off " \
	"console=$consoledev,$baudrate $othbootargs; " \
	"bootm $kernel_addr_flsh - $fdt_addr_flsh"

#define CONFIG_SDCARDBOOTCOMMAND \
	"setenv bootargs root=/dev/mmcblk0p2 rw rootwait " \
	"ip=$ipaddr:$serverip:$gatewayip:$netmask:$hostname:$netdev:off " \
	"console=$consoledev,$baudrate $othbootargs; "		\
	"fatload mmc 0:1 $loadaddr $bootfile; " \
	"fatload mmc 0:1 $fdt_addr $fdt_file; " \
	"bootm $loadaddr - $fdt_addr"


#ifdef CONFIG_SDCARD
#define CONFIG_BOOTCOMMAND		CONFIG_SDCARDBOOTCOMMAND
#else
#define CONFIG_BOOTCOMMAND		CONFIG_NORBOOTCOMMAND
#endif

#ifdef CONFIG_SECURE_BOOT
#include <asm/fsl_secure_boot.h>
#define CONFIG_CMD_BLOB
#endif

#endif	/* __CONFIG_TQMT104x_H */
