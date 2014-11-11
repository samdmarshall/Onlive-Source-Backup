#ifndef	_IMAGE2_HEADER_H_
#define	_IMAGE2_HEADER_H_

#include "version_table.h"

/*
 * bootloader block layout:
 * 0       : Im2-part1
 * 12K     : VERSION TABLE
 * 16K     : cust figo
 * 20K     : cust key
 * 24K     : Im2-part2
 */
#define IMG2_PART1_SIZE		12<<10
#define VT_OFFSET		(IMG2_PART1_SIZE)
#define VT_SIZE			4096
#define FIGO_OFFSET		((VT_OFFSET) + (VT_SIZE))
#define FIGO_SIZE		4096
#define KEY_OFFSET		((FIGO_OFFSET) + (FIGO_SIZE))
#define KEY_SIZE		4096
#define IMG2_PART2_OFFSET	((KEY_OFFSET) + (KEY_SIZE))

typedef struct _gen_header_t_ {
	unsigned int magic;
	unsigned int offset;
	unsigned int length;
	unsigned int exe_addr;
	unsigned int crc;
	unsigned int flag;
} gen_header_t;

typedef struct _image2_header_t {
	char part1[IMG2_PART1_SIZE];
	union{
		version_table_t vt;
		char vt_pad[VT_SIZE];
	};
	union{
		gen_header_t figo;
		char figo_pad[FIGO_SIZE];
	};
	union{
		gen_header_t cust_key;
		char cust_key_pad[KEY_SIZE];
	};
	char part2[];
} image2_header_t;

#endif
