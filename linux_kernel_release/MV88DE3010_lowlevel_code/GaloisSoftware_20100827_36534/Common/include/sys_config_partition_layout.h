#ifndef	__SYS_CONFIG_PARTITION_LAYOUT_H
#define	__SYS_CONFIG_PARTITION_LAYOUT_H

#define	BOOTFLOW_SPRUCE_VARIABLE_BLOCK_NUMBER		(16)
#define	BOOTFLOW_SPRUCE_IMAGE_BLOCK_NUMBER			(16)

#define	SPRUCE_MAGIC_NUMBER		(0xD2ADA3F1)
#define SPRUCE_IMAGE_TYPE		1
#define SPRUCE_VARIABLE_TYPE	2
typedef struct spruce_data_header {
	int magic1; //MAGIC_NUMBER
	int magic2; //MAGIC_NUMBER
	int type; //1 or 2
	int length;//flashless data length: image or variable length
	int block_size; //NAND flash block size
	int rsv[3];
} spruce_data_header_t;

typedef struct spruce_data_tail {
	int magic1; //MAGIC_NUMBER
	int magic2; //MAGIC_NUMBER
} spruce_data_tail_t;

#endif
