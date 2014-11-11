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

#ifndef _RLIST_H_
#define _RLIST_H_

#include "devextras.h"
#include "yaffs_ACP.h"

#ifdef YAFFS_ACP
#define RL_SUCC 0
#define RL_FAIL (-1)

#define YAFFS_ALLOCATION_NRLENTRIES	100

struct yaffs_DeviceStruct;


typedef struct _RListEntry {
	struct ylist_head entry;
	struct ylist_head clean_entry;/* linked in device ACPR clean list */
	struct ylist_head dirty_entry;/* linked in device ACPR write buffer, in version order(older entry is inserted earlier) */
	int objHasBeenDeleted;

	ACPRecord acpr;
	ACPExtraInfo acpExtInfo;
} RListEntry;

#define RLIST_CACHE
typedef struct _RList{
	struct yaffs_DeviceStruct *dev;
#ifdef RLIST_CACHE
	RListEntry *cacheEntry;/*last accessed RListEntry*/
#endif
	int len;
	int nDirty;/* number of dirty rlist entries */
	struct ylist_head head;
} RList;

typedef struct _RListEntryList{
	struct _RListEntryList *next;
	RListEntry *rlEntries;
} RListEntryList;

void yaffs_AddToACPRCleanList(yaffs_Device *dev, RListEntry *rlEntry);
void yaffs_AddToACPRDirtyList(yaffs_Device *dev, RListEntry *rlEntry);
void yaffs_RMFromACPRDirtyList(yaffs_Device *dev, RListEntry *rlEntry);
void yaffs_RMFromACPRCleanList(yaffs_Device *dev, RListEntry *rlEntry);

void yaffs_InitialiseRlist(yaffs_Device *dev);
void yaffs_DeinitialiseRlist(yaffs_Device *dev);
void RList_Init(yaffs_Device *dev, RList *rl);
void RList_Dump(RList *rl);
int RList_Truncate(RList *rl, int startOff);
void RList_FreeRListEntry(yaffs_Device *dev, RListEntry *rlEntry);
void RList_DelRListEntry(RList *rl, RListEntry *rlEntry);
RListEntry *RList_GetAndInitRListEntry(struct yaffs_DeviceStruct *dev, unsigned objectId,
				unsigned chunkInFile, int nBlankBytesInLastPage,
				int chunkInNAND, unsigned sequenceNumber, int nPages);
void RList_AdjustRListEntryToFileOffset(RList *rl, RListEntry *scanRLE, 
					__u32 fileEndOffset, int *newACPRSize);
int RList_AdjustRListEntryToBlockSequence(RList *rl, RListEntry *scanRLE,
					int *newACPRSize);

int RList_Update_Range_Backward(RList *rl, RListEntry *scanRLE, int *overlap);

#endif

#endif
