#
# Copyright 2017 TQ Systems GmbH
#
# Author: Stefan Lange <s.lange@gateware.de>
#
# SPDX-License-Identifier:      GPL-2.0+
#

ifdef CONFIG_SPL_BUILD
obj-y += spl.o
else
obj-$(CONFIG_TQMT2081) += tqmt2081.o
obj-$(CONFIG_TQMT2081) += eth.o
obj-$(CONFIG_TQMT2081) += vid.o
obj-$(CONFIG_PCI)      += pci.o
endif

obj-y   += ddr.o
obj-y   += law.o
obj-y   += tlb.o
