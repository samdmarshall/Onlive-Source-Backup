/*
 * rlist.c
 * 	Range list for address mapping
 *
 * Copyright (C) 2008-2009 Marvell Ltd.
 *
 * Created by Zheng Shi <zhengshi@marvell.com>
 *
 */
 
#include "yaffs_guts.h"
#include "yaffs_getblockinfo.h"

#ifdef __KERNEL__
#define RL_BUG(line) \
	do{printk("rlist bug! (%s:line %d)\n", __FILE__, line);/*while(1);*/}while(0)
#else
#define RL_BUG(line) \
	do{printf("rlist bug! (%s:line %d)\n", __FILE__, line);assert(0);}while(0)
#endif

typedef enum{
	UNKNOWN_ACTION=0,
	DOWN_AWAY=1,
	DOWN_ADJACENT=2,
	OVERLAP=3,
	UP_ADJACENT=4,
	UP_AWAY=5,
	EMPTY_INSERT=6,
} RLIST_UPDATE_ACTION;

typedef struct __RListUpdateHistory{
	__u32 objectId;
	int offset;
	int chunkInNAND;
	int sequence;
	int byteCount;
	RLIST_UPDATE_ACTION action;
	int stage;
	ACPRecord acpr;
} RListUpdateHistory;

#define MAX_HISTORY 16
#define HISTORY_MASK 15
static RListUpdateHistory rlUpdateHistory[MAX_HISTORY];
static unsigned currHistoryIndex = 0;

static inline void RList_InitUpdateHistory(void)
{
	currHistoryIndex = 0;
}

static void RList_AddUpdateHistory(RList *rl, RListEntry *rlEntry,
				const yaffs_ExtendedTags *tags, int chunkInNAND,
				RLIST_UPDATE_ACTION action, int stage)
{
	RListUpdateHistory * pCurrRLUH;
	pCurrRLUH = &rlUpdateHistory[currHistoryIndex&HISTORY_MASK];
	currHistoryIndex++;// = (currHistoryIndex+1)&HISTORY_MASK;

	pCurrRLUH->objectId = tags->objectId;
	pCurrRLUH->offset = tags->chunkId;
	pCurrRLUH->chunkInNAND = chunkInNAND;
	pCurrRLUH->sequence = tags->sequenceNumber;
	pCurrRLUH->byteCount = tags->byteCount;
	pCurrRLUH->action = action;
	pCurrRLUH->stage = stage;

	memcpy(&(pCurrRLUH->acpr), &(rlEntry->acpr), sizeof(ACPRecord));
}

static void RList_PrintUpdateHistory(void)
{
	int i;
	int index;
	int count;
	RListUpdateHistory * pCurrRLUH;

	if (currHistoryIndex >= MAX_HISTORY) {
		index = currHistoryIndex;
		count = MAX_HISTORY;
	} else{
		index = 0;
		count = currHistoryIndex;
	}
	T(YAFFS_TRACE_ALWAYS, ("[%s] printing history! currHistoryIndex=%d, index=%d, count=%d\n",
		__FUNCTION__, currHistoryIndex, index, count));

	for(i = 0; i < count; i++, index++) {
		int j = index & HISTORY_MASK;
		pCurrRLUH = rlUpdateHistory+j;
		T(YAFFS_TRACE_ALWAYS,
			(TSTR("[%s %d] <oid:%d off:%d addr:%d byteCount:%d seq:%d action:%d stage:%d>"),
			__FUNCTION__, j, pCurrRLUH->objectId, pCurrRLUH->offset, pCurrRLUH->chunkInNAND,
			pCurrRLUH->byteCount, pCurrRLUH->sequence, pCurrRLUH->action, pCurrRLUH->stage));
		T_ACPR(YAFFS_TRACE_ALWAYS, &(pCurrRLUH->acpr), __FUNCTION__, __LINE__);
	}
}

void yaffs_DeinitialiseRlist(yaffs_Device *dev)
{
	RListEntryList *tmp;
	while (dev->allocatedRLEntryList) {
		tmp = dev->allocatedRLEntryList->next;

		YFREE(dev->allocatedRLEntryList->rlEntries);
		YFREE(dev->allocatedRLEntryList);
		dev->allocatedRLEntryList = tmp;
	}
	YINIT_LIST_HEAD(&dev->freeRLEntries);
	dev->nFreeRLEntries = 0;
}

void yaffs_InitialiseRlist(yaffs_Device *dev)
{
	dev->allocatedRLEntryList = NULL;
	YINIT_LIST_HEAD(&dev->freeRLEntries);
	dev->nFreeRLEntries = 0;
	dev->nRListEntryCreated = 0;
	dev->nMaxRListLen = 0;
	dev->nMaxAveRListLen = 0;
	RList_InitUpdateHistory();
}

static int RList_CreateRListEntries(yaffs_Device *dev, int nRLEntries)
{
	RListEntry *newRLEntries;
	RListEntry *curr;
	RListEntryList *rlEntriesList;
	unsigned char *mem;
	int i;

	if (nRLEntries < 1)
		return RL_SUCC;

	newRLEntries = YMALLOC(nRLEntries * sizeof(RListEntry));
	mem = (unsigned char *)newRLEntries;

	if (!newRLEntries) {
		T(YAFFS_TRACE_ERROR,
			(TSTR("Couldn't allocate RListEntries" TENDSTR)));
		return RL_FAIL;
	}

	for (i = 0; i < nRLEntries; i++) {
		curr = (RListEntry *) &mem[i*sizeof(RListEntry)];
		ylist_add_tail(&curr->entry, &dev->freeRLEntries);
	}

	dev->nFreeRLEntries += nRLEntries;
	dev->nRListEntryCreated += nRLEntries;

	rlEntriesList = YMALLOC(sizeof(RListEntryList));
	if (!rlEntriesList) {
		T(YAFFS_TRACE_ERROR,
		  (TSTR
		   ("RList: Could not add rlEntries to management list" TENDSTR)));
		return RL_FAIL;
	} else{
		rlEntriesList->rlEntries = newRLEntries;
		rlEntriesList->next = dev->allocatedRLEntryList;
		dev->allocatedRLEntryList = rlEntriesList;
	}

	T(YAFFS_TRACE_ALLOCATE, (TSTR("RList: RLEntries added" TENDSTR)));

	return RL_SUCC;
}

static RListEntry *RList_GetRListEntryRaw(yaffs_Device *dev)
{
	RListEntry *rlEntry = NULL;
	struct ylist_head *first;

	if (ylist_empty(&dev->freeRLEntries))
		RList_CreateRListEntries(dev, YAFFS_ALLOCATION_NRLENTRIES);
	if (!ylist_empty(&dev->freeRLEntries)) {
		first = dev->freeRLEntries.next;
		rlEntry = ylist_entry(first, RListEntry, entry);
		ylist_del_init(first);
		dev->nFreeRLEntries--;
	}
	return rlEntry;
}

static RListEntry *RList_GetRListEntry(yaffs_Device *dev)
{
	RListEntry *rlEntry = RList_GetRListEntryRaw(dev);
	if (rlEntry) {
		rlEntry->acpr.size = 0;
		rlEntry->acpr.startAddress = 0;
		rlEntry->acpr.startOffset = 0;

		rlEntry->acpExtInfo.extraHeaderInfoAvailable = 0;
	}
	return rlEntry;
}

void RList_FreeRListEntry(yaffs_Device *dev, RListEntry *rlEntry)
{
	if (rlEntry) {
		ylist_add(&rlEntry->entry, &dev->freeRLEntries);
		dev->nFreeRLEntries++;
	}
}

RListEntry *RList_GetAndInitRListEntry(yaffs_Device *dev, unsigned objectId, 
				unsigned chunkInFile, int nBlankBytesInLastPage,
				int chunkInNAND, unsigned sequenceNumber, int nPages)
{
	RListEntry * entry;
	/*int chunkInFile = tags->chunkId;*/

	entry = RList_GetRListEntry(dev);
	if (entry) {
		YINIT_LIST_HEAD(&entry->entry);
		YINIT_LIST_HEAD(&entry->dirty_entry);
		YINIT_LIST_HEAD(&entry->clean_entry);
		entry->objHasBeenDeleted = 0;

		entry->acpr.objectId = /*tags->*/objectId;
		entry->acpr.startOffset = chunkInFile;
		entry->acpr.startAddress = chunkInNAND;
		entry->acpr.size = nPages;
		entry->acpr.nBlankBytesInLastPage = nBlankBytesInLastPage;
		/*entry->bytes = tags->byteCount;*/
		entry->acpr.version = sequenceNumber;
//		entry->acpr.newestPageAddress = chunkInNAND+nPages-1;
	}
	return entry;
}

static RListEntry *RList_GetAndInitRListEntryWithACPExtInfo(yaffs_Device *dev,
			unsigned objectId, unsigned chunkInFile,
			int nBlankBytesInLastPage, int chunkInNAND,
			unsigned sequenceNumber, int nPages,
			ACPExtraInfo *acpExtInfo)
{
	RListEntry * entry;
	unsigned long long t1 = YTIME();

	entry = RList_GetRListEntry(dev);
	if (entry) {
		YINIT_LIST_HEAD(&entry->entry);
		YINIT_LIST_HEAD(&entry->dirty_entry);
		YINIT_LIST_HEAD(&entry->clean_entry);
		entry->objHasBeenDeleted = 0;

		entry->acpr.objectId = objectId;
		entry->acpr.startOffset = chunkInFile;
		entry->acpr.startAddress = chunkInNAND;
		entry->acpr.size = nPages;
		entry->acpr.nBlankBytesInLastPage = nBlankBytesInLastPage;
		entry->acpr.version = sequenceNumber;
		if (entry->acpr.startOffset == 0) {
			ACPExtraInfo *extInfo = &entry->acpExtInfo;
			if (!acpExtInfo || acpExtInfo->extraHeaderInfoAvailable==0) {
				RL_BUG(__LINE__);
				RList_FreeRListEntry(dev, entry);
				dev->gutsInitInfo.dataBlkACPRScanAllocateTime += YTIME() - t1;
				return NULL;
			}
			memcpy(extInfo, acpExtInfo, sizeof(ACPExtraInfo));
		}
	}
	dev->gutsInitInfo.dataBlkACPRScanAllocateTime += YTIME() - t1;
	return entry;
}


static void RList_SetRListEntryExtraInfo(RListEntry *rlEntry, const yaffs_ExtendedTags *tags)
{
	if (!rlEntry || !tags || (rlEntry->acpr.startOffset != 0) || 
			tags->chunkId!=0 || (tags->extraHeaderInfoAvailable==0)) {
		RL_BUG(__LINE__);
		return;
	}

	rlEntry->acpExtInfo.extraHeaderInfoAvailable = 1;
	rlEntry->acpExtInfo.extraParentObjectId = tags->extraParentObjectId;
	rlEntry->acpExtInfo.extraFileLength = tags->extraFileLength;
	rlEntry->acpExtInfo.extraIsShrinkHeader = tags->extraIsShrinkHeader;
	rlEntry->acpExtInfo.extraObjectType = tags->extraObjectType;
	rlEntry->acpExtInfo.extraEquivalentObjectId = tags->extraEquivalentObjectId;
	rlEntry->acpExtInfo.extraShadows = tags->extraShadows;
}
/*
void RList_SetRListEntryExtraInfoDirectly(RListEntry *rlEntry, const ACPExtraInfo *acpExtInfo)
{
	if (!rlEntry || !acpExtInfo || (rlEntry->acpr.startOffset != 0) || \
			(acpExtInfo->extraHeaderInfoAvailable==0) ) {
		RL_BUG(__LINE__);
		return;
	}

	rlEntry->acpExtInfo.extraHeaderInfoAvailable = 1;
	rlEntry->acpExtInfo.extraParentObjectId = acpExtInfo->extraParentObjectId;
	rlEntry->acpExtInfo.extraFileLength = acpExtInfo->extraFileLength;
	rlEntry->acpExtInfo.extraIsShrinkHeader = acpExtInfo->extraIsShrinkHeader;
	rlEntry->acpExtInfo.extraObjectType = acpExtInfo->extraObjectType;
	rlEntry->acpExtInfo.extraEquivalentObjectId = acpExtInfo->extraEquivalentObjectId;
	rlEntry->acpExtInfo.extraShadows = acpExtInfo->extraShadows;
}*/

void RList_Init(yaffs_Device *dev, RList *rl)
{
	if (rl) {
		rl->dev = dev;
#ifdef RLIST_CACHE
		rl->cacheEntry = 0;
#endif
		rl->len = 0;
		YINIT_LIST_HEAD(&rl->head);
	}
}

/**
 * return previous rlist entry if exist, otherwise null is returned
 */
static inline RListEntry *RList_GetPrevRListEntry(RList *rl, RListEntry *curr)
{
	if (curr->entry.prev == &rl->head)
		return 0;
	return ylist_entry(curr->entry.prev, RListEntry, entry);
}

/**
 * return next rlist entry if exist, otherwise null is returned
 */
static inline RListEntry *RList_GetNextRListEntry(RList *rl, RListEntry *curr)
{
	if (curr->entry.next == &rl->head)
		return 0;
	return ylist_entry(curr->entry.next, RListEntry, entry);
}

/**
 * insert newRLEntry into an empty list
 */
static void RList_AddToEmptyRList(RList *rl, RListEntry *newRLEntry)
{
	ylist_add(&newRLEntry->entry, &rl->head);
	rl->len++;
	if(rl->len > rl->dev->nMaxRListLen)
		rl->dev->nMaxRListLen = rl->len;

	yaffs_AddToACPRCleanList(rl->dev, newRLEntry);
	yaffs_AddToACPRDirtyList(rl->dev, newRLEntry);
}

/**
 * insert newRLEntry before rlEntry
 */
static void RList_InsertBeforeRListEntry(RList *rl, RListEntry *rlEntry, RListEntry *newRLEntry)
{
	if (ylist_empty(&newRLEntry->entry)) {
		ylist_add(&newRLEntry->entry, rlEntry->entry.prev);
		rl->len++;
		if (rl->len > rl->dev->nMaxRListLen)
			rl->dev->nMaxRListLen = rl->len;
	} else {
		ylist_del(&newRLEntry->entry);
		ylist_add(&newRLEntry->entry, rlEntry->entry.prev);
	}

	yaffs_AddToACPRCleanList(rl->dev, newRLEntry);
	yaffs_AddToACPRDirtyList(rl->dev, newRLEntry);
}

/**
 * insert newRLEntry after rlEntry
 */
static void RList_InsertAfterRListEntry(RList *rl, RListEntry *rlEntry, RListEntry *newRLEntry)
{
	if (ylist_empty(&newRLEntry->entry)) {
		ylist_add(&newRLEntry->entry, &rlEntry->entry);
		rl->len++;
		if(rl->len > rl->dev->nMaxRListLen)
			rl->dev->nMaxRListLen = rl->len;
	} else {
		ylist_del(&newRLEntry->entry);
		ylist_add(&newRLEntry->entry, &rlEntry->entry);
	}

	yaffs_AddToACPRCleanList(rl->dev, newRLEntry);
	yaffs_AddToACPRDirtyList(rl->dev, newRLEntry);
}

/**
 * delete rlEntry from RList
 * remove rlEntry from ACP WB if needed
 */
void RList_DelRListEntry(RList *rl, RListEntry *rlEntry)
{
	ylist_del(&rlEntry->entry);
	rl->len--;

	yaffs_RMFromACPRDirtyList(rl->dev, rlEntry);
	yaffs_RMFromACPRCleanList(rl->dev, rlEntry);

	RList_FreeRListEntry(rl->dev, rlEntry);
}

/**
 * adjust down margin of rlEntry
 * deltaSize is in pages
 */
static void RList_AdjustRListEntryDownMargin(RList * rl, RListEntry *rlEntry,
					int deltaPage, int newVersion)
{
	yaffs_Device * dev = rl->dev;

	if( !deltaPage )
		RL_BUG(__LINE__);

	rlEntry->acpr.startAddress += deltaPage;
	rlEntry->acpr.startOffset += deltaPage;
	rlEntry->acpr.size -= deltaPage;
	if (rlEntry->acpr.size <= 0)
		RL_BUG(__LINE__);

	if (dev->acpLoggingStage == ACPLOGGING_STAGE_NORMAL)
		rlEntry->acpr.version = newVersion;

	if (deltaPage < 0) {
		yaffs_AddToACPRCleanList(rl->dev, rlEntry);
		yaffs_AddToACPRDirtyList(rl->dev, rlEntry);
	}
}

/**
 * adjust up margin of rlEntry
 * deltaSize is in pages
 */
static void RList_AdjustRListEntryUpMargin(RList *rl, RListEntry *rlEntry,
			int deltaPage, int nBlankBytesInLastPage, int newVersion)
{
	yaffs_Device *dev = rl->dev;

	rlEntry->acpr.size += deltaPage;
	if (rlEntry->acpr.size <= 0)
		RL_BUG(__LINE__);
	if (deltaPage>0 && rlEntry->acpr.nBlankBytesInLastPage != 0)
		RL_BUG(__LINE__);
	rlEntry->acpr.nBlankBytesInLastPage = nBlankBytesInLastPage;

	if (dev->acpLoggingStage == ACPLOGGING_STAGE_NORMAL)
		rlEntry->acpr.version = newVersion;

	if (deltaPage > 0) {
		yaffs_AddToACPRCleanList(rl->dev, rlEntry);
		yaffs_AddToACPRDirtyList(rl->dev, rlEntry);
	}
}

/**
 * adjust up margin of rlEntry
 * deltaSize is in bytes
 */
static void RList_RListEntryUpdateStartAddr(RList *rl, RListEntry *rlEntry,
				int startAddr, int nBlankBytesInLastPage,
				unsigned sequenceNumber)
{
	rlEntry->acpr.startAddress = startAddr;
	rlEntry->acpr.nBlankBytesInLastPage = nBlankBytesInLastPage;
	rlEntry->acpr.version = sequenceNumber;

	yaffs_AddToACPRCleanList(rl->dev, rlEntry);
	yaffs_AddToACPRDirtyList(rl->dev, rlEntry);
}

static int RList_Do_Update_Range_Ascendant(RList *rl, RListEntry *startRLE,
					RListEntry *rle, int *overlap)
{
	struct ylist_head *entry;
	RListEntry *currRLE, *newRLE;
	ACPRecord *currACPR, *acpr;
	int result = RL_SUCC;
	int nDeprecatedPages;
	unsigned long long t1 = YTIME();
	yaffs_Device *dev = rl->dev;

	acpr = &rle->acpr;
	entry = (startRLE->entry.prev);
	currRLE = NULL;

	while (entry != &(rl->head)) {
		struct ylist_head * temp;
		currRLE = ylist_entry(entry, RListEntry, entry);
		currACPR = &(currRLE->acpr);
		temp = entry->prev;

		if (currACPR->startOffset+currACPR->size > acpr->startOffset+acpr->size){
			result = RL_FAIL;
			RL_BUG(__LINE__);
			goto Update_Range_Ascendant_Finish;
		}
		if (currACPR->startOffset >= acpr->startOffset) {
			if(currACPR->startOffset+currACPR->size < acpr->startOffset+acpr->size) {
				newRLE = RList_GetAndInitRListEntryWithACPExtInfo(rl->dev, acpr->objectId,
					currACPR->startOffset+currACPR->size, acpr->nBlankBytesInLastPage,
					acpr->startAddress+currACPR->startOffset+currACPR->size-acpr->startOffset,
					acpr->version,
					acpr->startOffset+acpr->size-currACPR->startOffset-currACPR->size,
					&rle->acpExtInfo);
				if (!newRLE) {
					result = RL_FAIL;
					goto Update_Range_Ascendant_Finish;
				}
				RList_InsertAfterRListEntry(rl, currRLE, newRLE);
				if (yaffs_ACPCallback_HandleACPR(rl->dev, 1, newRLE) == YAFFS_FAIL){
					result = RL_FAIL;
					goto Update_Range_Ascendant_Finish;
				}
			}
			nDeprecatedPages = currACPR->size;
			yaffs_ACPCallback_HandleACPR(rl->dev, 0, &nDeprecatedPages);
			if (currACPR->startOffset > acpr->startOffset) {
				RList_AdjustRListEntryUpMargin(rl, rle,
					-(acpr->startOffset+acpr->size-currACPR->startOffset),
					0, acpr->version);
			} else {
				acpr->size = 0;/* indicate rle should be freed */
				goto Update_Range_Ascendant_Finish;
			}
		} else
			break;
		entry = temp;
	}
	if (entry == &rl->head) {
		if (currRLE) {
			RList_InsertBeforeRListEntry(rl, currRLE, rle);
		} else {
			RList_InsertBeforeRListEntry(rl, startRLE, rle);
		}
		yaffs_ACPCallback_HandleACPR(rl->dev, 1, rle);
	} else {
		if (!currRLE || (currACPR->startOffset+currACPR->size > acpr->startOffset+acpr->size)
			|| (currACPR->startOffset >= acpr->startOffset)) {
			result = RL_FAIL;
			RL_BUG(__LINE__);
			goto Update_Range_Ascendant_Finish;
		}
		if (currACPR->startOffset+currACPR->size > acpr->startOffset) {
			nDeprecatedPages = currACPR->startOffset+currACPR->size - acpr->startOffset;
			if (currACPR->startOffset+currACPR->size < acpr->startOffset+acpr->size){
				RList_AdjustRListEntryDownMargin(rl, rle,
					nDeprecatedPages, acpr->version);
				RList_InsertAfterRListEntry(rl, currRLE, rle);
				yaffs_ACPCallback_HandleACPR(rl->dev, 1, rle);
			} else {
				if(nDeprecatedPages != acpr->size)
					RL_BUG(__LINE__);
				acpr->size = 0;/* indicate rle should be freed */
			}
			yaffs_ACPCallback_HandleACPR(rl->dev, 0, &nDeprecatedPages);
		} else if(currACPR->startOffset+currACPR->size == acpr->startOffset){
			if (currACPR->startAddress+currACPR->size == acpr->startAddress){
				RList_AdjustRListEntryUpMargin(rl, currRLE,
					acpr->size, acpr->nBlankBytesInLastPage, currACPR->version);
				yaffs_ACPCallback_HandleACPR(rl->dev, 1, rle);
				acpr->size = 0;/* indicate rle should be freed */
			} else {
				RList_InsertAfterRListEntry(rl, currRLE, rle);
				yaffs_ACPCallback_HandleACPR(rl->dev, 1, rle);
			}
		} else {
			RList_InsertAfterRListEntry(rl, currRLE, rle);
			yaffs_ACPCallback_HandleACPR(rl->dev, 1, rle);
		}
	}

Update_Range_Ascendant_Finish:
	dev->gutsInitInfo.dataBlkACPRScanRListUpdateAscendantTime += YTIME()-t1;
	return result;
}

int RList_AdjustRListEntryToBlockSequence(RList *rl, RListEntry *scanRLE, int *newACPRSize)
{
	yaffs_Device * dev = rl->dev;
	yaffs_BlockInfo * bi;
	ACPRecord *acpr = &scanRLE->acpr;
	RListEntry *newRLE;
	int startblk, blk, chunk;
	int res = YAFFS_OK;
	int delta;

	startblk = acpr->startAddress / dev->nChunksPerBlock;
	blk = (acpr->startAddress + acpr->size - 1) / dev->nChunksPerBlock;
	chunk = (acpr->startAddress + acpr->size - 1);
	while (blk>=startblk) {
		bi = yaffs_GetBlockInfo(dev, blk);
		if (bi->sequenceNumber > acpr->version ||
				bi->blockState==YAFFS_BLOCK_STATE_DEAD ||
				bi->blockState==YAFFS_BLOCK_STATE_EMPTY) {
			if (acpr->startAddress + acpr->size - 1 > chunk) {
				if (blk*dev->nChunksPerBlock-1 >= acpr->startAddress) {
					newRLE = RList_GetAndInitRListEntryWithACPExtInfo(rl->dev, acpr->objectId,
						acpr->startOffset, 0,
						acpr->startAddress,
						acpr->version,
						blk*dev->nChunksPerBlock-acpr->startAddress,
						&scanRLE->acpExtInfo);
					if (!newRLE) {
						RL_BUG(__LINE__);
						res = YAFFS_FAIL;
						goto adjust_blockseq_finish;
					}
					//RList_InsertAfterRListEntry(rl, scanRLE, newRLE);
					ylist_add(&newRLE->entry, &scanRLE->entry);
				}
				RList_AdjustRListEntryDownMargin(rl, scanRLE,
					chunk-acpr->startAddress+1, acpr->version);
				*newACPRSize = acpr->size;
			} else if (acpr->startAddress+acpr->size-1 == chunk) {
				if (blk>startblk) {
					delta = chunk-blk*dev->nChunksPerBlock+1;
					if(delta<=0)
						RL_BUG(__LINE__);
					RList_AdjustRListEntryUpMargin(rl, scanRLE,
						-delta, 0, acpr->version);
					yaffs_ACPCallback_HandleACPR(dev, 0, &delta);
					*newACPRSize = acpr->size;
				} else {
					delta = acpr->size;
					yaffs_ACPCallback_HandleACPR(dev, 0, &delta);
					*newACPRSize = 0;
				}
			} else
				RL_BUG(__LINE__);
			goto adjust_blockseq_finish;
		}
		chunk = blk*dev->nChunksPerBlock-1;
		blk--;
	}
	*newACPRSize = acpr->size;
adjust_blockseq_finish:
	return res;
}

void RList_AdjustRListEntryToFileOffset(RList *rl, RListEntry *scanRLE,
				__u32 fileEndOffset, int *newACPRSize)
{
	ACPRecord *acpr = &scanRLE->acpr;
	yaffs_Device *dev = rl->dev;
	int nDeprecatedPages;
	int nBytesInLastPage;
	__u32 lastFileOffsetInACPR;

	if (acpr->startOffset > 0) {
		if((__u32)(acpr->startOffset-1)*dev->nDataBytesPerChunk > fileEndOffset) {
			nDeprecatedPages = acpr->size;
			yaffs_ACPCallback_HandleACPR(rl->dev, 0, &nDeprecatedPages);
			*newACPRSize = 0;
			return;
		}
	}
	nBytesInLastPage = dev->nDataBytesPerChunk - acpr->nBlankBytesInLastPage;
	if (nBytesInLastPage <= 0)
		RL_BUG(__LINE__);
	lastFileOffsetInACPR = (__u32)(acpr->startOffset+acpr->size-2)*dev->nDataBytesPerChunk +
			nBytesInLastPage - 1;
	if (lastFileOffsetInACPR > fileEndOffset) {
		/* FIXME: can't find a better way to substitute division? */
		int endPageIndex = fileEndOffset/dev->nDataBytesPerChunk + 1;
		nDeprecatedPages = acpr->startOffset+acpr->size-1-endPageIndex;
		if (nDeprecatedPages < 0)
			RL_BUG(__LINE__);
		if (nDeprecatedPages > 0) {
			RList_AdjustRListEntryUpMargin(rl, scanRLE,
				-nDeprecatedPages, 0, acpr->version);
			yaffs_ACPCallback_HandleACPR(rl->dev, 0, &nDeprecatedPages);
		}
	}
	*newACPRSize = acpr->size;
}

int RList_Update_Range_Backward(RList *rl, RListEntry *scanRLE, int *overlap)
{
	RListEntry *currRLE, *newRLE, *prevRLE;
	ACPRecord *acpr, *currACPR;
	ACPExtraInfo *acpExtInfo;
	struct ylist_head *entry;
	int result = RL_SUCC;
	int nDeprecatedPages;
	yaffs_Device * dev=rl->dev;

	acpr = &scanRLE->acpr;
	acpExtInfo = &scanRLE->acpExtInfo;

	/* empty list */
	if (ylist_empty(&rl->head)) {
		if (rl->len) {
			T(YAFFS_TRACE_ALWAYS,
			 (TSTR("%s: length mismatch for empty list" TENDSTR), __FUNCTION__));
			RList_Dump(rl);
		}
		newRLE = RList_GetAndInitRListEntryWithACPExtInfo(rl->dev, acpr->objectId,
			acpr->startOffset, acpr->nBlankBytesInLastPage,
			acpr->startAddress, acpr->version, acpr->size, acpExtInfo);
		if (!newRLE) {
			result = RL_FAIL;
			goto Update_Range_Backward_Finish;
		}
		RList_AddToEmptyRList(rl, newRLE);
		yaffs_ACPCallback_HandleACPR(dev, 1, newRLE);
		goto Update_Range_Backward_Finish;
	}

#ifdef RLIST_CACHE
	if (rl->cacheEntry) {
		ACPRecord *cacheACPR = &(rl->cacheEntry->acpr);
		currRLE = NULL;
		entry = &(rl->cacheEntry->entry);
		if (cacheACPR->startOffset >= acpr->startOffset+acpr->size+1) {
			goto find_update_range_startEntry;
		} else {
			entry = entry->next;
			while(entry != &(rl->head)) {
				currRLE = ylist_entry(entry, RListEntry, entry);
				currACPR = &(currRLE->acpr);
				if (currACPR->startOffset>=acpr->startOffset+acpr->size+1) {
					goto find_update_range_startEntry;
				}
				entry = entry->next;
				dev->gutsInitInfo.dataBlkACPRScanRListUpdateIterations++;
			}
		}
	}
	entry = rl->head.prev;
	currRLE = NULL;
#else
	entry = rl->head.prev;
	currRLE = NULL;
#endif
find_update_range_startEntry:
	/* non empty list */
	/*up away from scanRLE
	 * startOff >= acpr->endOff+2
	 * continue
	 */
	while (entry != &(rl->head)) {
		dev->gutsInitInfo.dataBlkACPRScanRListUpdateIterations++;

		currRLE = ylist_entry(entry, RListEntry, entry);
		currACPR = &(currRLE->acpr);

		if (currACPR->startOffset >= acpr->startOffset+acpr->size+1){
			entry = entry->prev;
			rl->cacheEntry = currRLE;
			continue;
		} else {
			break;
		}
	}
	if (entry == &(rl->head)) {
		newRLE = RList_GetAndInitRListEntryWithACPExtInfo(rl->dev, acpr->objectId,
			acpr->startOffset, acpr->nBlankBytesInLastPage, acpr->startAddress,
			acpr->version, acpr->size, acpExtInfo);
		if (!newRLE) {
			result = RL_FAIL;
			goto Update_Range_Backward_Finish;
		}
		if (!currRLE) {
			result = RL_FAIL;
			RL_BUG(__LINE__);
			goto Update_Range_Backward_Finish;
		}
		RList_InsertBeforeRListEntry(rl, currRLE, newRLE);
		yaffs_ACPCallback_HandleACPR(rl->dev, 1, newRLE);
		goto Update_Range_Backward_Finish;
	}

	/*
	 * startOff == acpr->endOff+1
	 */
	if ((currACPR->startOffset == acpr->startOffset+acpr->size)) {
		newRLE = RList_GetAndInitRListEntryWithACPExtInfo(dev, acpr->objectId,
			acpr->startOffset, acpr->nBlankBytesInLastPage, acpr->startAddress,
			acpr->version, acpr->size, acpExtInfo);
		if (!newRLE) {
			result = RL_FAIL;
			goto Update_Range_Backward_Finish;
		}
		result = RList_Do_Update_Range_Ascendant(rl, currRLE, newRLE, overlap);
		if (result == RL_FAIL) {
			RList_FreeRListEntry(dev, newRLE);
			goto Update_Range_Backward_Finish;
		}
		if (newRLE->acpr.size == 0)
			RList_FreeRListEntry(rl->dev, newRLE);
		prevRLE = RList_GetPrevRListEntry(rl, currRLE);
		if (!prevRLE) {
			result = RL_FAIL;
			RL_BUG(__LINE__);
			goto Update_Range_Backward_Finish;
		}
		if (prevRLE->acpr.startAddress + prevRLE->acpr.size == currACPR->startAddress) {
			RList_AdjustRListEntryDownMargin(rl, currRLE, -prevRLE->acpr.size,
				currACPR->version);
			if (prevRLE->acpr.startOffset == 0)
				memcpy(&(currRLE->acpExtInfo), &(prevRLE->acpExtInfo), sizeof(ACPExtraInfo));
			RList_DelRListEntry(rl, prevRLE);
		}
		goto Update_Range_Backward_Finish;
	}
	/*
	 * acpr->startOff <= startOff <= acpr->endOff
	 */
	if (currACPR->startOffset >= acpr->startOffset) {
		if (currACPR->startOffset+currACPR->size < acpr->startOffset+acpr->size) {
			newRLE = RList_GetAndInitRListEntryWithACPExtInfo(dev, acpr->objectId,
				currACPR->startOffset+currACPR->size,
				acpr->nBlankBytesInLastPage,
				acpr->startAddress+ currACPR->startOffset+currACPR->size-acpr->startOffset,
				acpr->version, acpr->startOffset+acpr->size-currACPR->startOffset-currACPR->size,
				acpExtInfo);
			if (!newRLE) {
				result = RL_FAIL;
				goto Update_Range_Backward_Finish;
			}
			RList_InsertAfterRListEntry(rl, currRLE, newRLE);
			yaffs_ACPCallback_HandleACPR(dev, 1, newRLE);
			nDeprecatedPages = currACPR->size;
		} else
			nDeprecatedPages = acpr->startOffset+acpr->size - currACPR->startOffset;
		yaffs_ACPCallback_HandleACPR(dev, 0, &nDeprecatedPages);
		if (currACPR->startOffset > acpr->startOffset ) {
			newRLE = RList_GetAndInitRListEntryWithACPExtInfo(dev, acpr->objectId,
				acpr->startOffset, 0, acpr->startAddress, acpr->version,
				currACPR->startOffset-acpr->startOffset, acpExtInfo);
			if (!newRLE) {
				result = RL_FAIL;
				goto Update_Range_Backward_Finish;
			}
			result = RList_Do_Update_Range_Ascendant(rl, currRLE, newRLE, overlap);
			if (result == RL_FAIL || newRLE->acpr.size == 0)
				RList_FreeRListEntry(dev, newRLE);
		}
		goto Update_Range_Backward_Finish;
	}
	/*
	 * startOff < acpr->startOff && endOff >= acpr->startOff
	 */
	if (currACPR->startOffset+currACPR->size-1 >= acpr->startOffset) {
		if (currACPR->startOffset+currACPR->size < acpr->startOffset+acpr->size){
			nDeprecatedPages = currACPR->startOffset+currACPR->size - acpr->startOffset;
			newRLE = RList_GetAndInitRListEntryWithACPExtInfo(rl->dev, acpr->objectId,
				currACPR->startOffset + nDeprecatedPages, acpr->nBlankBytesInLastPage,
				acpr->startAddress + nDeprecatedPages, acpr->version,
				acpr->size - nDeprecatedPages, acpExtInfo);
			if (!newRLE) {
				result = RL_FAIL;
				goto Update_Range_Backward_Finish;
			}
			RList_InsertAfterRListEntry(rl, currRLE, newRLE);
			yaffs_ACPCallback_HandleACPR(dev, 1, newRLE);
		} else {
			nDeprecatedPages = acpr->size;
		}
		yaffs_ACPCallback_HandleACPR(dev, 0, &nDeprecatedPages);
		goto Update_Range_Backward_Finish;
	}
	/*
	 * endOff == acpr->startOff-1 &&
	 * endAddr == acpr->endAddr-1
	 */
	if (currACPR->startOffset+currACPR->size == acpr->startOffset &&
		(currACPR->startAddress+currACPR->size == acpr->startAddress)) {
		RList_AdjustRListEntryUpMargin(rl, currRLE, acpr->size, acpr->nBlankBytesInLastPage, currACPR->version);
		yaffs_ACPCallback_HandleACPR(dev, 1, scanRLE);
		goto Update_Range_Backward_Finish;
	}
	/*
	 * endOff <= acpr->endOff-2
	 *    OR
	 * can't merge currACPR with acpr
	 */
	newRLE = RList_GetAndInitRListEntryWithACPExtInfo(rl->dev, acpr->objectId, acpr->startOffset,
		acpr->nBlankBytesInLastPage, acpr->startAddress, acpr->version, acpr->size, acpExtInfo);
	if (!newRLE) {
		result = RL_FAIL;
		goto Update_Range_Backward_Finish;
	}
	RList_InsertAfterRListEntry(rl, currRLE, newRLE);
	yaffs_ACPCallback_HandleACPR(dev, 1, scanRLE);

Update_Range_Backward_Finish:
	/* add debug information*/
//	T_ACPR(YAFFS_TRACE_ALWAYS, &(scanRLE->acpr), __FUNCTION__, __LINE__);
//	RList_Verify(rl);
	return result;
}

/*
 * verify RList should be upwards, and no overlapping
 */
static int RList_Verify(RList *rl)
{
	RListEntry *rlEntry;
	struct ylist_head *entry;
	ACPRecord *prevACPR;
	ACPRecord *currACPR;
	int i = 0;
	int ret = YAFFS_OK;

	prevACPR = 0;
	entry = rl->head.next;
	while (entry != &(rl->head)) {
		rlEntry = ylist_entry(entry, RListEntry, entry);
		currACPR = &rlEntry->acpr;
		entry = entry->next;

		if (prevACPR) {
			if (currACPR->startOffset < prevACPR->startOffset+prevACPR->size ||
					(currACPR->startAddress <= prevACPR->startAddress+prevACPR->size-1 &&
					 currACPR->startAddress+currACPR->size-1 >= prevACPR->startAddress))
			{
				T(YAFFS_TRACE_ALWAYS, (TSTR("Overlapping" TENDSTR)));
				T_ACPR(YAFFS_TRACE_ALWAYS, prevACPR, __FUNCTION__, __LINE__);
				T_ACPR(YAFFS_TRACE_ALWAYS, currACPR, __FUNCTION__, __LINE__);
				ret = YAFFS_FAIL;
			}
		}

		i++;
		if (i > rl->len) {
			T(YAFFS_TRACE_ALWAYS, (TSTR("rl len invalid:i=%d, rl_len=%d" TENDSTR), i, rl->len));
			ret = YAFFS_FAIL;
			break;
		}

		prevACPR = currACPR;
	}

	if (i!=rl->len) {
		YBUG(rl->dev);
#ifdef __KERNEL__
		dump_stack();
#endif
	}

	return ret;
}

/**
 * update an address mapping in the RList
 *
 * ACPLOGGING_STAGE_DATABLOCK_SCANNING
 *	no_replace=1	update_dirty_list=1
 * ACPLOGGING_STAGE_ACPR_SCANNING
 *	no_replace=1	update_dirty_list=0
 * ACPLOGGING_STAGE_NORMAL
 *	no_replace=0	update_dirty_list=1
 *
 */
int RList_Update(RList *rl, const yaffs_ExtendedTags *tags,
		int chunkInNAND, int stage, int *overlap)
{
	struct ylist_head *entry_list;
	RListEntry *rlEntry;
	RListEntry *prevRLEntry;
	RListEntry *nextRLEntry;
	RListEntry *newRLEntry;
	RListEntry *newRLEntry1;
	RListEntry *pRLE = 0;
	int chunkInFile = tags->chunkId;
	yaffs_Device *dev = rl->dev;
	yaffs_ACPRGroupInfo *acprGroup;
	int nBlankBytesInLastPage;
	int replace;
	RLIST_UPDATE_ACTION action;
	ACPRecord overlapACPR;

	action = UNKNOWN_ACTION;
	if (overlap)
		*overlap = 0;
	if (tags->byteCount > dev->nDataBytesPerChunk)
		RL_BUG(__LINE__);

	acprGroup = yaffs_GetCurrentACPRGroup(dev);
	if (!acprGroup) {
		RL_BUG(__LINE__);
		return RL_FAIL;
	}
	if (tags->chunkId == 0)
		nBlankBytesInLastPage = 0;
	else if (tags->byteCount == dev->nDataBytesPerChunk)
		nBlankBytesInLastPage = 0;
	else /* if(tags->byteCount < dev->nDataBytesPerChunk)*/
		nBlankBytesInLastPage = dev->nDataBytesPerChunk - tags->byteCount%dev->nDataBytesPerChunk;

	switch (stage) {
	case ACPLOGGING_STAGE_DATABLOCK_SCANNING:
		replace = 0;
		break;
	case ACPLOGGING_STAGE_ACPR_SCANNING:
		replace = 0;
		break;
	case ACPLOGGING_STAGE_NORMAL:
		replace = 1;
		break;
	default:
		T(YAFFS_TRACE_ALWAYS, (TSTR("unknown ACPR logging stage"TENDSTR)));
		return YAFFS_FAIL;
	}

	/** empty list*/
	if (ylist_empty(&rl->head)) {
		if (rl->len!=0){
			T(YAFFS_TRACE_ALWAYS,
			 (TSTR("%s: length mismatch for empty list" TENDSTR), __FUNCTION__));
			RList_Dump(rl);
		}
		newRLEntry = RList_GetAndInitRListEntry(rl->dev, tags->objectId, tags->chunkId, nBlankBytesInLastPage, \
				chunkInNAND, tags->sequenceNumber, 1);
		if (!newRLEntry)
			return RL_FAIL;
		RList_AddToEmptyRList(rl, newRLEntry);

		pRLE = newRLEntry;
		action = EMPTY_INSERT;
		goto update_finish;
	}

#ifdef RLIST_CACHE
	if (rl->cacheEntry) {
		ACPRecord *acpr;
		entry_list = &rl->cacheEntry->entry;
		rlEntry = rl->cacheEntry;
		prevRLEntry = RList_GetPrevRListEntry(rl, rlEntry);
		nextRLEntry = RList_GetNextRListEntry(rl, rlEntry);
		acpr = &(rlEntry->acpr);

		if ((chunkInFile >= acpr->startOffset) &&
			  (chunkInFile < acpr->startOffset+acpr->size)) {
			goto update_overlap;
		}
		if (chunkInFile == acpr->startOffset+acpr->size) {
			goto up_adjacent;
		}
		if (chunkInFile >= acpr->startOffset + acpr->size + 1) {
			goto up_away;
		}
	}
#endif

	/** non-empty RList*/
	ylist_for_each(entry_list, &rl->head) {
		dev->gutsInitInfo.dataBlkACPRScanRListUpdateIterations++;

		rlEntry = ylist_entry(entry_list, RListEntry, entry);
		prevRLEntry = RList_GetPrevRListEntry(rl, rlEntry);
		nextRLEntry = RList_GetNextRListEntry(rl, rlEntry);

		if (chunkInFile <= (rlEntry->acpr.startOffset-2)) {
			action = DOWN_AWAY;

			if (!prevRLEntry ||
				(prevRLEntry &&
				((prevRLEntry->acpr.startOffset+prevRLEntry->acpr.size+1)<=chunkInFile)))
			{
				newRLEntry = RList_GetAndInitRListEntry(rl->dev,
						tags->objectId, tags->chunkId,
						nBlankBytesInLastPage, chunkInNAND,
						tags->sequenceNumber, 1);
				if (!newRLEntry)
					return RL_FAIL;
				RList_InsertBeforeRListEntry(rl, rlEntry, newRLEntry);

				pRLE = newRLEntry;
				goto update_finish;
			}

			RL_BUG(__LINE__);

		} else if (chunkInFile == rlEntry->acpr.startOffset-1) {
			action = DOWN_ADJACENT;
			if(!prevRLEntry ||
				  (prevRLEntry &&
				  ((prevRLEntry->acpr.startOffset+prevRLEntry->acpr.size+1)<=chunkInFile)))
			{
				if ((chunkInNAND+1) == rlEntry->acpr.startAddress && (nBlankBytesInLastPage == 0)) {
					RList_AdjustRListEntryDownMargin(rl, rlEntry, -1, tags->sequenceNumber);

					pRLE = rlEntry;
				} else {
					newRLEntry = RList_GetAndInitRListEntry(rl->dev,
						tags->objectId, tags->chunkId,
						nBlankBytesInLastPage, chunkInNAND,
						tags->sequenceNumber, 1);
					if (!newRLEntry)
						return RL_FAIL;
					RList_InsertBeforeRListEntry(rl, rlEntry, newRLEntry);

					pRLE = newRLEntry;
				}
				goto update_finish;
			}

			RL_BUG(__LINE__);

		} else if ((chunkInFile >= rlEntry->acpr.startOffset) &&
			  (chunkInFile < rlEntry->acpr.startOffset+rlEntry->acpr.size)) {

#ifdef RLIST_CACHE
update_overlap:
#endif
			action = OVERLAP;
			memcpy(&overlapACPR, &(rlEntry->acpr), sizeof(ACPRecord));

			if (overlap)
				*overlap = 1;
			if (!replace)
				/* if replacement not allowed, update is over*/
				goto update_finish; 

			if (rlEntry->acpr.size == 1) {
				if (!prevRLEntry && !nextRLEntry) {
					RList_RListEntryUpdateStartAddr(rl, rlEntry,
						chunkInNAND, nBlankBytesInLastPage,
						tags->sequenceNumber);
					pRLE = rlEntry;
				} else if (prevRLEntry && !nextRLEntry) {
					if ((chunkInFile == (prevRLEntry->acpr.startOffset+prevRLEntry->acpr.size)) &&
						(chunkInNAND == (prevRLEntry->acpr.startAddress+prevRLEntry->acpr.size)) &&
						prevRLEntry->acpr.nBlankBytesInLastPage == 0) {
						RList_AdjustRListEntryUpMargin(rl,
							prevRLEntry, 1,
							nBlankBytesInLastPage,
							tags->sequenceNumber);
						RList_DelRListEntry(rl, rlEntry);
						pRLE = prevRLEntry;
					} else {
						RList_RListEntryUpdateStartAddr(rl,
							rlEntry, chunkInNAND,
							nBlankBytesInLastPage,
							tags->sequenceNumber);
						pRLE = rlEntry;
					}
				} else if (!prevRLEntry && nextRLEntry) {
//					if(nBlankBytesInLastPage)/*nBlankBytesInLastPage should be 0*/
//						RL_BUG(__LINE__);
					if ((chunkInFile+1 == nextRLEntry->acpr.startOffset) &&
						  (chunkInNAND+1 == nextRLEntry->acpr.startAddress) &&
						  nBlankBytesInLastPage == 0) {
						RList_AdjustRListEntryDownMargin(rl, nextRLEntry, -1, tags->sequenceNumber);
						RList_DelRListEntry(rl, rlEntry);

						pRLE = nextRLEntry;
					} else {
						RList_RListEntryUpdateStartAddr(rl,
							rlEntry, chunkInNAND,
							nBlankBytesInLastPage,
							tags->sequenceNumber);
						pRLE = rlEntry;
					}
				} else /**if(prevRLEntry && nextRLEntry)*/{
//					if(nBlankBytesInLastPage)/*nBlankBytesInLastPage should be 0*/
//						RL_BUG(__LINE__);
					if ((chunkInFile == (prevRLEntry->acpr.startOffset+prevRLEntry->acpr.size)) &&
						 (chunkInNAND == (prevRLEntry->acpr.startAddress+prevRLEntry->acpr.size)) &&
						 prevRLEntry->acpr.nBlankBytesInLastPage == 0) {
						RList_AdjustRListEntryUpMargin(rl,
							prevRLEntry, 1,
							nBlankBytesInLastPage,
							tags->sequenceNumber);
						RList_DelRListEntry(rl, rlEntry);
						if((chunkInFile+1 == nextRLEntry->acpr.startOffset) &&
							 ((chunkInNAND+1) == nextRLEntry->acpr.startAddress) &&
							 nBlankBytesInLastPage == 0) {
							RList_AdjustRListEntryUpMargin(rl,
								prevRLEntry,
								nextRLEntry->acpr.size,
								nextRLEntry->acpr.nBlankBytesInLastPage, tags->sequenceNumber);
							RList_DelRListEntry(rl, nextRLEntry);
						}
						pRLE = prevRLEntry;
					} else if ((chunkInFile+1) == nextRLEntry->acpr.startOffset &&
							(chunkInNAND+1) == nextRLEntry->acpr.startAddress &&
							nBlankBytesInLastPage == 0) {
						RList_AdjustRListEntryDownMargin(rl, nextRLEntry, -1, tags->sequenceNumber);

						RList_DelRListEntry(rl, rlEntry);
						pRLE = nextRLEntry;
					} else {
						RList_RListEntryUpdateStartAddr(rl,
							rlEntry, chunkInNAND,
							nBlankBytesInLastPage,
							tags->sequenceNumber);
						pRLE = rlEntry;
					}
				}
			} /** if rlEntry->size == 1*/else {
				if(chunkInFile == rlEntry->acpr.startOffset){/** down margin*/
					RList_AdjustRListEntryDownMargin(rl, rlEntry, 1, rlEntry->acpr.version);
					if (prevRLEntry &&
							(prevRLEntry->acpr.startOffset+prevRLEntry->acpr.size == chunkInFile) &&
							(prevRLEntry->acpr.startAddress+prevRLEntry->acpr.size == chunkInNAND) &&
							prevRLEntry->acpr.nBlankBytesInLastPage == 0) {
						RList_AdjustRListEntryUpMargin(rl,
							prevRLEntry, 1,
							nBlankBytesInLastPage,
							tags->sequenceNumber);
						pRLE = prevRLEntry;
					} else {
						newRLEntry = RList_GetAndInitRListEntry(rl->dev, tags->objectId, tags->chunkId, nBlankBytesInLastPage,\
							chunkInNAND, tags->sequenceNumber, 1);
						if (!newRLEntry)
							return RL_FAIL;
						RList_InsertBeforeRListEntry(rl, rlEntry, newRLEntry);
						pRLE = newRLEntry;
					}
				} else if(chunkInFile == (rlEntry->acpr.startOffset+rlEntry->acpr.size-1)){/** up margin*/
					RList_AdjustRListEntryUpMargin(rl, rlEntry, -1, 0/*nBlankBytesInLastPage should be 0*/, \
						rlEntry->acpr.version);
					if (nextRLEntry &&
							(chunkInFile + 1 == nextRLEntry->acpr.startOffset) &&
							(chunkInNAND + 1 == nextRLEntry->acpr.startAddress) &&
							nBlankBytesInLastPage == 0) {
						RList_AdjustRListEntryDownMargin(rl, nextRLEntry, -1, tags->sequenceNumber);
						pRLE = nextRLEntry;
					} else {
						newRLEntry = RList_GetAndInitRListEntry(rl->dev, tags->objectId, tags->chunkId, \
							nBlankBytesInLastPage, chunkInNAND, tags->sequenceNumber, 1);
						if(!newRLEntry)
							return RL_FAIL;
						RList_InsertAfterRListEntry(rl, rlEntry, newRLEntry);

						pRLE = newRLEntry;
					}
				} else {/** middle*/
					int deltaSize, originalBlankBytesInLastPage;

					deltaSize = rlEntry->acpr.size - (chunkInFile - rlEntry->acpr.startOffset);
					originalBlankBytesInLastPage = rlEntry->acpr.nBlankBytesInLastPage;

					/* allocate 2 new RLEntries */
					newRLEntry = RList_GetAndInitRListEntry(rl->dev,
							tags->objectId, chunkInFile,
							nBlankBytesInLastPage, chunkInNAND,
							tags->sequenceNumber, 1);
					if(!newRLEntry)
						return RL_FAIL;
					newRLEntry1 = RList_GetAndInitRListEntry(rl->dev,
							tags->objectId, chunkInFile+1,
							originalBlankBytesInLastPage,
							rlEntry->acpr.startAddress+(chunkInFile+1-rlEntry->acpr.startOffset),
							rlEntry->acpr.version, deltaSize-1);
					if (!newRLEntry1) {
						RList_FreeRListEntry(rl->dev, newRLEntry);
						return RL_FAIL;
					}

					RList_AdjustRListEntryUpMargin(rl, rlEntry, -deltaSize, 0/*nBlankBytesInLastPage should be 0*/,\
						rlEntry->acpr.version);
					RList_InsertAfterRListEntry(rl, rlEntry, newRLEntry1);
					RList_InsertAfterRListEntry(rl, rlEntry, newRLEntry);

					pRLE = newRLEntry;
				}
			}

			goto update_finish;

		} else if(chunkInFile == rlEntry->acpr.startOffset+rlEntry->acpr.size){
#ifdef RLIST_CACHE
up_adjacent:
#endif
			action = UP_ADJACENT;
			if (!nextRLEntry ||
				(nextRLEntry &&
				 (nextRLEntry->acpr.startOffset > (chunkInFile+1)))) {
				if (chunkInNAND == (rlEntry->acpr.startAddress+rlEntry->acpr.size) &&
					 rlEntry->acpr.nBlankBytesInLastPage == 0) {
					RList_AdjustRListEntryUpMargin(rl, rlEntry, 1,
							nBlankBytesInLastPage,
							tags->sequenceNumber);

					pRLE = rlEntry;
				} else {
					newRLEntry = RList_GetAndInitRListEntry(rl->dev,
						tags->objectId, chunkInFile,
						nBlankBytesInLastPage, chunkInNAND,
						tags->sequenceNumber, 1);
					if (!newRLEntry)
						return RL_FAIL;
					RList_InsertAfterRListEntry(rl, rlEntry, newRLEntry);

					pRLE = newRLEntry;
				}
				goto update_finish;
			}
			if (nextRLEntry->acpr.startOffset == (chunkInFile+1)) {
//				if(nBlankBytesInLastPage)
//					RL_BUG(__LINE__);
				if(chunkInNAND == (rlEntry->acpr.startAddress+rlEntry->acpr.size) &&
					 rlEntry->acpr.nBlankBytesInLastPage == 0) {
					RList_AdjustRListEntryUpMargin(rl, rlEntry, 1, 0,
							tags->sequenceNumber);
					if ((chunkInNAND+1) == nextRLEntry->acpr.startAddress && nBlankBytesInLastPage == 0) {
						RList_AdjustRListEntryUpMargin(rl,
							rlEntry, nextRLEntry->acpr.size,
							nextRLEntry->acpr.nBlankBytesInLastPage, tags->sequenceNumber);
						RList_DelRListEntry(rl, nextRLEntry);
					}

					pRLE = rlEntry;
				} else if ((chunkInNAND+1) == nextRLEntry->acpr.startAddress && nBlankBytesInLastPage == 0) {
					RList_AdjustRListEntryDownMargin(rl, nextRLEntry, -1, tags->sequenceNumber);

					pRLE = nextRLEntry;
				} else {
					newRLEntry = RList_GetAndInitRListEntry(rl->dev,
							tags->objectId, tags->chunkId,
							nBlankBytesInLastPage, chunkInNAND,
							tags->sequenceNumber, 1);
					if (!newRLEntry)
						return RL_FAIL;
					RList_InsertAfterRListEntry(rl, rlEntry, newRLEntry);
					pRLE = newRLEntry;
				}
				goto update_finish;
			}

			continue;/** handle next RLEntry*/

		} else {/** chunkInFile >= rlEntry->startOffset+rlEntry->size+1*/
#ifdef RLIST_CACHE
up_away:
#endif
			action = UP_AWAY;
			if (!nextRLEntry) {
				newRLEntry = RList_GetAndInitRListEntry(rl->dev,
						tags->objectId, tags->chunkId,
						nBlankBytesInLastPage, chunkInNAND,
						tags->sequenceNumber, 1);
				if (!newRLEntry)
					return RL_FAIL;
				RList_InsertAfterRListEntry(rl, rlEntry, newRLEntry);
				pRLE = newRLEntry;
				goto update_finish;
			}
			continue;/** handle next RLEntry*/
		}

	} /** end of list iteration*/

update_finish:
	if (!pRLE && !(action==OVERLAP && replace==0)) {
		T(YAFFS_TRACE_ALWAYS, (TSTR("[%s] <oid:%d off:%d addr:%d count:%d ver:%d> not handled!" TENDSTR),
			__FUNCTION__, tags->objectId, tags->chunkId, tags->byteCount, tags->sequenceNumber, chunkInNAND));
		RList_PrintUpdateHistory();
		RList_Dump(rl);
		yaffs_ACPR_PrintLoggingHistory();
		RL_BUG(__LINE__);
	}

#ifdef RLIST_CACHE
	rl->cacheEntry = pRLE;
#endif

	if (pRLE) {
		/* update extra infomation for header chunk */
		if (tags->chunkId == 0)
			RList_SetRListEntryExtraInfo(pRLE, tags);
		if (action == OVERLAP)
			acprGroup->nDeprecatedPagesCovered++;
		else
			acprGroup->nValidPagesCovered++;
	}

	/* TODO: for more detailed debug information, maybe deleted later */
	if (pRLE) {
		RList_AddUpdateHistory(rl, pRLE, tags, chunkInNAND, action, stage);
		if (chunkInFile < pRLE->acpr.startOffset ||
			chunkInFile >= pRLE->acpr.startOffset+pRLE->acpr.size ||
			chunkInNAND != chunkInFile-pRLE->acpr.startOffset+pRLE->acpr.startAddress) {
			T(YAFFS_TRACE_ALWAYS, (TSTR("[%s] <oid:%d off:%d addr:%d> != "),
				__FUNCTION__, tags->objectId, tags->chunkId, chunkInNAND));
			T_ACPR(YAFFS_TRACE_ALWAYS, &(pRLE->acpr), __FUNCTION__, __LINE__);

			if (action == OVERLAP) {
				T(YAFFS_TRACE_ALWAYS, (TSTR("[%s] overlapped chunk isnot properly updated" TENDSTR), __FUNCTION__));
				T_ACPR(YAFFS_TRACE_ALWAYS, &overlapACPR, __FUNCTION__, __LINE__);
			}

			RList_PrintUpdateHistory();
			RList_Dump(rl);
			yaffs_ACPR_PrintLoggingHistory();
			RL_BUG(__LINE__);
		}
	}

	if (yaffs_traceMask & YAFFS_TRACE_ACP_DEBUG) {
		if (yaffs_ACPVerifyACPRDirtyList(dev) == YAFFS_FAIL ||
				RList_Verify(rl)==YAFFS_FAIL) {
			RList_PrintUpdateHistory();
			RList_Dump(rl);
			yaffs_ACPR_PrintLoggingHistory();
		}
	}

	return RL_SUCC;
}

/**
 * \brief delete information of chunks whose offset>=startOff
 *
 */
int RList_Truncate(RList *rl, int startOff)
{
	struct ylist_head *entry_list;
	RListEntry *rlEntry;

	/** non-empty list*/
	for (entry_list = (&rl->head)->next; entry_list != (&rl->head);) {

		rlEntry = ylist_entry(entry_list, RListEntry, entry);
		entry_list = entry_list->next;

		if (rlEntry->acpr.startOffset >= startOff) {
#ifdef RLIST_CACHE
			if (rlEntry == rl->cacheEntry)
				rl->cacheEntry = 0;
#endif
			RList_DelRListEntry(rl, rlEntry);
		} else if (startOff > rlEntry->acpr.startOffset &&
			startOff<rlEntry->acpr.startOffset+rlEntry->acpr.size) {
		  	RList_AdjustRListEntryUpMargin(rl, rlEntry,
				-(rlEntry->acpr.size-(startOff-rlEntry->acpr.startOffset)),
				0, rlEntry->acpr.version);
		}
	}

	return RL_SUCC;
}

void RList_Dump(RList *rl)
{
	struct ylist_head *entry_list;
	RListEntry *rlEntry;
	int i = 0;

	T(YAFFS_TRACE_ALWAYS, ("[%s]\n", __FUNCTION__));

	/** non-empty list*/
	ylist_for_each(entry_list, &rl->head) {

		rlEntry = ylist_entry(entry_list, RListEntry, entry);
		//printk("rlEntry(%p)\n", rlEntry);
		T_ACPR(YAFFS_TRACE_ALWAYS, &(rlEntry->acpr), __FUNCTION__, __LINE__);
		i++;
	}

	if (i != rl->len) {
		T(YAFFS_TRACE_ALWAYS, (TSTR("[%s]i=%d rl_len=%d" TENDSTR), __FUNCTION__, i, rl->len));
		YBUG(rl->dev);
	}
	T(YAFFS_TRACE_ALWAYS, ("[%s ends]\n", __FUNCTION__));
}

