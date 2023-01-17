#
# Copyright 2015 TQ Systems GmbH
#
# SPDX-License-Identifier:	GPL-2.0+
#

ifdef CONFIG_SPL_BUILD
obj-y += spl.o
else
obj-y	+= tqmt10xx.o
obj-y	+= eth.o
obj-$(CONFIG_PCI)	+= pci.o
obj-$(CONFIG_FSL_DIU_FB)+= diu.o
endif
obj-y	+= ddr.o
obj-y	+= law.o
obj-y	+= tlb.o
obj-y	+= sleep.o
