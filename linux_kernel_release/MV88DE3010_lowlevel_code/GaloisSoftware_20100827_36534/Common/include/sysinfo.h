/*******************************************************************************
*******************************************************************************/

#ifndef _ONLINEUPGRADE_DEFINE_H_
#define _ONLINEUPGRADE_DEFINE_H_

#include <com_type.h>
#include <version_table.h>
#include <image2_header.h>

#define	BAD_BLOCK_TO_WRITE			(-1)


#define MAGIC_NUMBER	(0xD2ADA3F1)

#define OU_UPGRADING 0x5550494E /* UPIN*/
#define OU_UPGRADED  0x55504544 /* UPED*/

#define		OU_COMMAND_TASK_CREATE				(0xF0000)
#define		OU_COMMAND_SET_PARAM				(0xF0001)
#define		OU_COMMAND_SET_UPGRADE_METHOD		(0xF0002)
#define		OU_COMMAND_CHECK_LATEST_IMAGE		(0xF0003)
#define		OU_COMMAND_CONTINUE					(0xF0004)
#define		OU_COMMAND_START					(0xF0004)
#define		OU_COMMAND_CANCEL					(0xF0005) 
#define		OU_COMMAND_STOP						(0xF0006)
#define		OU_COMMAND_QUIT						(0xF0007)
#define		OU_COMMAND_FURTHER_ACT				(0xF0008)
#define		OU_COMMAND_GETSTATUS				(0xF0009)
#define		OU_COMMAND_UPGRADE_OVER			(0xF000A)
#define		OU_COMMAND_GET_IMAGE_INFO		(0xF000B)
#define     OU_COMMAND_EXTRACT_ROOTFS_FOR_OU    (0xF000C)
#define		OU_COMMAND_DownloadImage_FOR_OU	 (0xF000D)
#define		OU_COMMAND_REBURN_BOOTLOADER		(0xF000E)

#define		OU_COMMAND_ERROR_MSG					(0xFFFFF)
#define		OU_COMMAND_TASK_CREATE_RESP  	(0xFF000)
#define		OU_COMMAND_CHECK_LATEST_IMAGE_RESP	(0xFF001)
#define		OU_COMMAND_CONTINUE_RESP				(0xFF004)
#define		OU_COMMAND_START_RESP					(0xF0004)
#define		OU_COMMAND_CANCEL_RESP					(0xFF005)
#define		OU_COMMAND_STOP_RESP					(0xFF006)
#define		OU_COMMAND_QUIT_RESP					(0xFF007)
#define		OU_COMMAND_GETSTATUS_RESP				(0xFF009)
#define		OU_COMMAND_UPGRADE_OVER_RESP			(0xFF00A) 
#define		OU_COMMAND_GET_IMAGE_INFO_RESP		(0xFF00B)
#define		OU_COMMAND_EXTRACT_ROOTFS_FOR_OU_RESP	(0xFF00C)
#define		OU_COMMAND_DownloadImage_FOR_OU_RESP (0xFF00D)
#define		OU_COMMAND_REBURN_BOOTLOADER_RESP		(0xFF00E)

#define		OU_COMMAND_INVALID_COMMAND		(0xFFFFFFFF)

typedef struct _partition_t {
	INT32 fd;
	CHAR *name;
	UINT32 offset;
	UINT32 size;
} partition_t;
typedef struct _partTable_t {
	INT32 partition_num;
	partition_t *parts;
} partTable_t;

typedef struct _MV_mtd_info_t {
	INT32 chipfd;
	partTable_t pt;
	INT32 erasesize;
	INT32 pagesize;
	unsigned long long chipsize;
} MV_mtd_info_t;

typedef struct _MV_sysInfo_t {
	version_table_t *vt;
	MV_mtd_info_t	nandInfo;
} MV_sysInfo_t;

typedef struct _uniImgInfo_t {
	struct archive *ar;
	BOOL ptchange;
	img_hdr_t *imghdr;
	INT32 *subimg_partind;
} uniImgInfo_t;

#define LEGACY_NAND_CHIP_DEVICE "/dev/mtd_mv"

INT32 OU_get_system_info(MV_sysInfo_t *sysInfo);
INT32 get_bootloader_block(MV_sysInfo_t *sysInfo, CHAR *buffer);

#endif
