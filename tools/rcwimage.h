/*
 * Copyright 2016 TQ Systems GmbH
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef RCWIMAGE_H
#define RCWIMAGE_H

#define RCW_BYTES	64
#define RCW_PREAMBLE	0xaa55aa55
#define RCW_HEADER	0x010e0100

struct rcw_header {
	uint32_t preamble;
	uint32_t rcwheader;
	uint8_t rcw_data[RCW_BYTES];
};

#endif /* RCWIMAGE_H */
