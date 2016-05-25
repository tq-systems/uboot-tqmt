/*
 * Copyright 2016 TQ Systems GmbH
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include "imagetool.h"
#include <image.h>
#include "rcwimage.h"
#include "pbl_crc32.h"

/*
 * need to store all bytes in memory for calculating crc32, then write the
 * bytes to RCW image file.
 */
static unsigned char mem_buf[128];
static unsigned char *pmem_buf = mem_buf;
static int rcw_size;
static char *fname = "Unknown";
static int lineno = -1;
static struct rcw_header rcwimage_header;
static int arch_flag;

static uint32_t rcw_crc_cmd1;
static uint32_t rcw_crc_cmd2;

static union
{
	char c[4];
	unsigned char l;
} endian_test = { {'l', '?', '?', 'b'} };

#define ENDIANNESS ((char)endian_test.l)

static void check_get_hexval(char *token)
{
	uint32_t hexval;
	int i;

	if (!sscanf(token, "%x", &hexval)) {
		printf("Error:%s[%d] - Invalid hex data(%s)\n", fname,
			lineno, token);
		exit(EXIT_FAILURE);
	}
	for (i = 3; i >= 0; i--) {
		*pmem_buf++ = (hexval >> (i * 8)) & 0xff;
		rcw_size++;
	}
}

static void rcw_parser(char *name)
{
	FILE *fd = NULL;
	char *line = NULL;
	char *token, *saveptr1, *saveptr2;
	size_t len = 0;

	fname = name;
	fd = fopen(name, "r");
	if (fd == NULL) {
		printf("Error:%s - Can't open\n", fname);
		exit(EXIT_FAILURE);
	}

	while ((getline(&line, &len, fd)) > 0) {
		lineno++;
		token = strtok_r(line, "\r\n", &saveptr1);
		/* drop all lines with zero tokens (= empty lines) */
		if (token == NULL)
			continue;
		for (line = token;; line = NULL) {
			token = strtok_r(line, " \t", &saveptr2);
			if (token == NULL)
				break;
			/* Drop all text starting with '#' as comments */
			if (token[0] == '#')
				break;
			check_get_hexval(token);
		}
	}
	if (line)
		free(line);
	fclose(fd);
}

static uint32_t reverse_byte(uint32_t val)
{
	uint32_t temp;
	unsigned char *p1;
	int j;

	temp = val;
	p1 = (unsigned char *)&temp;
	for (j = 3; j >= 0; j--)
		*p1++ = (val >> (j * 8)) & 0xff;
	return temp;
}

/* write crc command to memory. */
static void add_crc_cmd(void)
{
	uint32_t crc32_rcw;

	/* Add CRC command. */
	*pmem_buf++ = 0x08;
	*pmem_buf++ = rcw_crc_cmd1;
	*pmem_buf++ = rcw_crc_cmd2;
	*pmem_buf++ = 0x40;
	rcw_size += 4;

	/* calculated CRC32 and write it to memory. */
	crc32_rcw = pbl_crc32(0, (const char *)mem_buf, rcw_size);
	*pmem_buf++ = (crc32_rcw >> 24) & 0xff;
	*pmem_buf++ = (crc32_rcw >> 16) & 0xff;
	*pmem_buf++ = (crc32_rcw >> 8) & 0xff;
	*pmem_buf++ = (crc32_rcw) & 0xff;
	rcw_size += 4;
}

void rcw_load_cfg_file(int ifd, struct image_tool_params *params)
{
	int size;

	/* parse the rcw.cfg file. */
	rcw_parser(params->imagename);

	add_crc_cmd();

        lseek(ifd, 0, SEEK_SET);
	size = rcw_size;
	if (write(ifd, (const void *)&mem_buf, size) != size) {
		fprintf(stderr, "Write error on %s: %s\n",
			params->imagefile, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

static int rcwimage_check_image_types(uint8_t type)
{
	if (type == IH_TYPE_RCWIMAGE)
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

static int rcwimage_verify_header(unsigned char *ptr, int image_size,
			struct image_tool_params *params)
{
	struct rcw_header *rcw_hdr = (struct rcw_header *) ptr;

	/* Only a few checks can be done: search for magic numbers */
	if (ENDIANNESS == 'l') {
		if (rcw_hdr->preamble != reverse_byte(RCW_PREAMBLE))
			return -FDT_ERR_BADSTRUCTURE;

		if (rcw_hdr->rcwheader != reverse_byte(RCW_HEADER))
			return -FDT_ERR_BADSTRUCTURE;
	} else {
		if (rcw_hdr->preamble != RCW_PREAMBLE)
			return -FDT_ERR_BADSTRUCTURE;

		if (rcw_hdr->rcwheader != RCW_HEADER)
			return -FDT_ERR_BADSTRUCTURE;
	}
	return 0;
}

static void rcwimage_print_header(const void *ptr)
{
	printf("Image Type:   Freescale RCW binary image\n");
}

static void rcwimage_set_header(void *ptr, struct stat *sbuf, int ifd,
				struct image_tool_params *params)
{
	/*nothing need to do, rcw_load_cfg_file takes care of whole file. */
}

int rcwimage_check_params(struct image_tool_params *params)
{
	if (!params)
		return EXIT_FAILURE;

	if (params->arch == IH_ARCH_PPC) {
		arch_flag = IH_ARCH_PPC;
		rcw_crc_cmd1 = 0x13;
		rcw_crc_cmd2 = 0x80;
	}

	return 0;
};

/* rcwimage parameters */
U_BOOT_IMAGE_TYPE(
	rcwimage,
	"Freescale RCW binary image support",
	sizeof(struct rcw_header),
	(void *)&rcwimage_header,
	rcwimage_check_params,
	rcwimage_verify_header,
	rcwimage_print_header,
	rcwimage_set_header,
	NULL,
	rcwimage_check_image_types,
	NULL,
	NULL
);
