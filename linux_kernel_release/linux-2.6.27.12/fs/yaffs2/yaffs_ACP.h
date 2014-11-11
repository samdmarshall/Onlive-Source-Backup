/*
 * YAFFS Advanced Checkpoint implementation.
 *
 * Copyright (C) 2008-2009 Marvell Technology Group Ltd.
 *
 * Created by Zheng Shi <zhengshi@marvell.com>
 * Modified by Jisheng Zhang <jszhang@marvell.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __YAFFS_ACP_H__
#define __YAFFS_ACP_H__

struct yaffs_DeviceStruct;

typedef struct yaffs_DeviceStruct yaffs_Device;
#ifdef YAFFS_ACP

#define ACPLOGGING_STAGE_PRE_SCAN 0
#define ACPLOGGING_STAGE_ACPR_COLLECTING 1
#define ACPLOGGING_STAGE_DATABLOCK_SCANNING 2
#define ACPLOGGING_STAGE_ACPR_SCANNING 3
#define ACPLOGGING_STAGE_NORMAL 4 /* during YAFFS run-time*/


extern int verify_clean_list;
extern int verify_dirty_list;
extern int verify_valid_ACPR_InScan;

#define TEST_ACP

#ifdef TEST_ACP
extern unsigned errACPPageSeq;
extern int simACPRcrcERR;
extern int nACPRcrcERR;
extern int nACPRrerr;

extern int nACPRwerr;
extern int simACPRwerr;
#endif

typedef struct __ACPRecord{
	__u32 objectId;
	int startOffset;
	int startAddress;
	int size;	/* in pages */
	int nBlankBytesInLastPage;
	__u32 version;
	__u32 extInfo;
	__u32 crc;
} ACPRecord;

typedef struct __ACPExtraInfo{

	unsigned extraHeaderInfoAvailable;	/* There is extra info available if this is not zero */
	unsigned extraParentObjectId;	/* The parent object */
	unsigned extraIsShrinkHeader;	/* Is it a shrink header? */
	unsigned extraShadows;		/* Does this shadow another object? */

	yaffs_ObjectType extraObjectType;	/* What object type? */

	unsigned extraFileLength;		/* Length if it is a file */
	unsigned extraEquivalentObjectId;	/* Equivalent object Id if it is a hard link */
} ACPExtraInfo;

typedef struct __yaffs_ACPRGroupInfo {
	unsigned int groupSequence;
	unsigned int blockSequence;
	unsigned int pageSequence;/* starting from 0(ACPR group header chunk)*/

	int *acpBlockList;
	int nACPBlocks;

	/* gross scan of an ACPR group */
	int nEstimateACPChunks;
	__u32 estimateNewestBlkSeqCovered;
	__u32 estimateNewestPageAddr;

	int nACPRs;

	/* coverage information*/
	__u32 newestBlockSequenceCovered;
	__u32 newestPageInBlkCovered;
	__u32 nDeprecatedPagesCovered;
	__u32 nValidPagesCovered;
	__u32 nTotalPagesCovered;
	__u32 nPagesUncovered;

	/* ACP block allocation information */
	int currBlock;
	int currChunk;
} yaffs_ACPRGroupInfo;

typedef struct __yaffs_acprGroupHeader {
	unsigned int groupSequence;
} yaffs_acprGroupHeader;


int yaffs_ACPInitialise(yaffs_Device *dev);
int yaffs_ACPDeInitialise(yaffs_Device *dev);
int yaffs_ACPWriteACPR(yaffs_Device *dev, const ACPRecord *acpr,
			const ACPExtraInfo *acpExtInfo);
int yaffs_ACPFlushBuffer(yaffs_Device *dev);
int yaffs_ACPWriteACPRGroupHeader(yaffs_Device *dev, yaffs_ACPRGroupInfo *acprGroup);
int yaffs_ACPStartNewACPRGroup(yaffs_Device *dev);
int yaffs_ACPEraseACPRGroup(yaffs_Device *dev, yaffs_ACPRGroupInfo *acprGroup);
void yaffs_ACPEraseACPRGroups(yaffs_Device *dev);
void yaffs_ACPR_PrintLoggingHistory(void);
void yaffs_UnpackExtInfoFromACPR(ACPRecord *acpr, ACPExtraInfo *acpExtInfo);
int yaffs_ACPVerifyACPR(yaffs_Device *dev, ACPRecord *acpr, const char *funcName);
int yaffs_ACPVerifyACPRDirtyList(yaffs_Device *dev);
int yaffs_ACPSave(struct yaffs_DeviceStruct *dev);
int yaffs_ACPCallback_HandleACPR(yaffs_Device *dev, int valid, void *data);
int yaffs_ACPCommitACPRDirtyList(yaffs_Device *dev, int force);


#endif

#endif
