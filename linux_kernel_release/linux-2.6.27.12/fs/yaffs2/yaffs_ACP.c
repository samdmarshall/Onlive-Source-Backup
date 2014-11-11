/*
 * YAFFS Advanced Checkpoint implementation.
 *
 * Copyright (C) 2008-2009 Marvell Technology Group Ltd.
 *
 * Created by Zheng Shi <zhengshi@marvell.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "yaffs_getblockinfo.h"
#include "yportenv.h"
#include "yaffs_nand.h"

/* Some of the information is "extra" struff which can be packed in to
 * speed scanning
 * This is defined by having the EXTRA_HEADER_INFO_FLAG set.
 */

/* Extra flags applied to acpr->startOff */

#define EXTRA_HEADER_INFO_FLAG	0x80000000
#define EXTRA_SHRINK_FLAG	0x40000000
#define EXTRA_SHADOWS_FLAG	0x20000000
#define EXTRA_SPARE_FLAGS	0x10000000

#define ALL_EXTRA_FLAGS		0xF0000000

/* Also, the top 4 bits of the object Id are set to the object type. */
#define EXTRA_OBJECT_TYPE_SHIFT (28)
#define EXTRA_OBJECT_TYPE_MASK  ((0x0F) << EXTRA_OBJECT_TYPE_SHIFT)


/*
 * for debugging information
 */
typedef struct __LoggingHistory{
	__u32 objectId;
	int offset;
	int chunkInNAND;
	int sequence;
	int byteCount;
} LoggingHistory;

#define MAX_LOGGING_HISTORY 128
#define LOGGING_HISTORY_MASK 0x7f
static LoggingHistory loggingHistory[MAX_LOGGING_HISTORY];
static unsigned currLoggingHistoryIndex = 0;

int verify_dirty_list = 0;
int verify_clean_list = 0;
int verify_valid_ACPR_InScan = 0;

#ifdef TEST_ACP
int nACPRrerr = 0;
int nACPRwerr = 0;
int simACPRwerr = 0;
unsigned errACPPageSeq=0xffffffff;
int simACPRcrcERR = 0;
int nACPRcrcERR = 0;
#endif

static inline void yaffs_ACPR_InitLoggingHistory(void)
{
	currLoggingHistoryIndex = 0;
}

static void yaffs_InitACPRGroupInfo(yaffs_Device *dev, yaffs_ACPRGroupInfo *acprGroup)
{
	int i, acprGroupIndex = acprGroup - dev->acprGroup;

	acprGroup->groupSequence = 0;
	(acprGroupIndex == 0) ? (acprGroup->blockSequence=YAFFS_SEQUENCE_ACP_DATA1) :
		(acprGroup->blockSequence=YAFFS_SEQUENCE_ACP_DATA2);
	acprGroup->pageSequence = 0;

	acprGroup->acpBlockList = dev->acpBlockList + acprGroupIndex * dev->nACPRGroupMaxBlocks;
	for (i = 0; i < dev->nACPRGroupMaxBlocks; i++)
		acprGroup->acpBlockList[i] = -1;

	acprGroup->nACPBlocks = acprGroup->nACPRs = 0;

	acprGroup->newestBlockSequenceCovered = 0;
	acprGroup->newestPageInBlkCovered = -1;
	acprGroup->nDeprecatedPagesCovered = acprGroup->nTotalPagesCovered = 0;
	acprGroup->nPagesUncovered = 0;
	acprGroup->currBlock = -1;
	acprGroup->currChunk = 0;
}

int yaffs_ACPInitialise(yaffs_Device *dev)
{
	int acpPagesPerCommit, acpTotalPages;
	int nBlocks = dev->internalEndBlock - dev->internalStartBlock + 1;

	/* Got the functions we need? */
	if (!dev->writeChunkWithTagsToNAND || !dev->readChunkWithTagsFromNAND ||
			!dev->eraseBlockInNAND || !dev->markNANDBlockBad)
		return YAFFS_FAIL;

	yaffs_InitialiseRlist(dev);

	dev->acpBuffer = YMALLOC_DMA(dev->totalBytesPerChunk);
	if (!dev->acpBuffer) {
		YBUG(dev);
		return YAFFS_FAIL;
	}
	memset(dev->acpBuffer, 0xff, dev->totalBytesPerChunk);

	dev->acpNextBlock = 0;

	dev->acprSize = sizeof(ACPRecord);
	dev->nMaxACPRsPerChunk = dev->nDataBytesPerChunk/dev->acprSize;
	dev->nMaxACPRBytesPerChunk = dev->nMaxACPRsPerChunk * dev->acprSize;

	dev->acpMaxCoveredChunksPerCommit = dev->nChunksPerBlock;

	/* reserved blocks for ACP */
	acpPagesPerCommit = dev->acpMaxCoveredChunksPerCommit * dev->acprSize / dev->nDataBytesPerChunk + 1;
	acpTotalPages = nBlocks*dev->nChunksPerBlock / dev->acpMaxCoveredChunksPerCommit * acpPagesPerCommit;
	dev->nACPRGroupMaxBlocks = acpTotalPages / dev->nChunksPerBlock + 4;
	dev->nReservedBlocks += 2*dev->nACPRGroupMaxBlocks;
	T(YAFFS_TRACE_ACP_INFO, (TSTR("[ACP] reserve %d*2 blocks for ACPRs" TENDSTR), dev->nACPRGroupMaxBlocks));

	dev->acpBlockList = YMALLOC(sizeof(int) * dev->nACPRGroupMaxBlocks * 2);
	if (!dev->acpBlockList) {
		YBUG(dev);
		YFREE(dev->acpBuffer);
		return YAFFS_FAIL;
	}
	yaffs_InitACPRGroupInfo(dev, &dev->acprGroup[0]);
	yaffs_InitACPRGroupInfo(dev, &dev->acprGroup[1]);
	dev->nACPRGroups = 0;
	dev->currACPRGroupIndex = -1;

	YINIT_LIST_HEAD(&dev->acprCleanList);
	YINIT_LIST_HEAD(&dev->acprDirtyList);
	YINIT_LIST_HEAD(&dev->acprTempDirtyList);
	YINIT_LIST_HEAD(&dev->acprScanList);

	yaffs_ACPR_InitLoggingHistory();

	return YAFFS_OK;
}

int yaffs_ACPDeInitialise(yaffs_Device *dev)
{
	YFREE(dev->acpBlockList);
	dev->acpBlockList = NULL;
	dev->acprGroup[0].acpBlockList = dev->acprGroup[1].acpBlockList = NULL;

	yaffs_DeinitialiseRlist(dev);

	if (dev->acpBuffer) {
		/* free the buffer */
		YFREE(dev->acpBuffer);
		dev->acpBuffer = NULL;
		return 1;
	} else
		return 0;
}

void yaffs_ACPR_AddLoggingHistory(const yaffs_ExtendedTags *tags, int chunkInNAND)
{
	LoggingHistory *pCurrRLUH;
	pCurrRLUH = &loggingHistory[currLoggingHistoryIndex & LOGGING_HISTORY_MASK];
	currLoggingHistoryIndex++;

	pCurrRLUH->objectId = tags->objectId;
	pCurrRLUH->offset = tags->chunkId;
	pCurrRLUH->chunkInNAND = chunkInNAND;
	pCurrRLUH->sequence = tags->sequenceNumber;
	pCurrRLUH->byteCount = tags->byteCount;
}

void yaffs_ACPR_PrintLoggingHistory(void)
{
	int i, j, index, count;
	LoggingHistory *pCurrRLUH;

	if (currLoggingHistoryIndex >= MAX_LOGGING_HISTORY) {
		index = currLoggingHistoryIndex;
		count = MAX_LOGGING_HISTORY;
	} else {
		index = 0;
		count = currLoggingHistoryIndex;
	}
	T(YAFFS_TRACE_ACP_ERROR,
		(TSTR("[ACP] ACPR history! currLoggingHistoryIndex=%d, index=%d, count=%d\n" TENDSTR),
		currLoggingHistoryIndex, index, count));

	for (i = 0; i < count; i++, index++) {
		j = index & LOGGING_HISTORY_MASK;
		pCurrRLUH = loggingHistory + j;
		T(YAFFS_TRACE_ACP_ERROR, (TSTR("[ACP] [%3d] <oid:%d off:%d addr:%d byteCount:%d seq:%d>" TENDSTR),
			j, pCurrRLUH->objectId, pCurrRLUH->offset, pCurrRLUH->chunkInNAND,
			pCurrRLUH->byteCount, pCurrRLUH->sequence));
	}
}

static void yaffs_ACPFindNextErasedBlock(yaffs_Device *dev)
{
	int  i;

	T(YAFFS_TRACE_ACP_DETAIL,
		(TSTR("[ACP] allocating ACP block: erased %d reserved %d next %d "TENDSTR),
		dev->nErasedBlocks,dev->nReservedBlocks,dev->acpNextBlock));

	for (i = dev->internalStartBlock; i <= dev->internalEndBlock; i++) {
		yaffs_BlockInfo *bi;

		dev->acpNextBlock++;
		if (dev->acpNextBlock < dev->internalStartBlock
				|| dev->acpNextBlock > dev->internalEndBlock) {
			dev->acpNextBlock = dev->internalStartBlock;
		}
		bi = yaffs_GetBlockInfo(dev, dev->acpNextBlock);

		if (bi->blockState == YAFFS_BLOCK_STATE_EMPTY) {
			T(YAFFS_TRACE_ACP_DETAIL, (TSTR("[ACP] allocated ACP block %d" TENDSTR),
				dev->acpNextBlock));
			return;
		}
	}

	T(YAFFS_TRACE_ACP_ERROR, (TSTR("[ACP] out of ACP blocks, disable ACP" TENDSTR)));
	dev->acpDisabled = 1;
	dev->acpNextBlock = -1;
}

int yaffs_ACPVerifyACPR(yaffs_Device *dev, ACPRecord *acpr, const char *funcName)
{
	int chunkInNAND, offset;
	int i, byteCount;
	yaffs_ExtendedTags tags;
	yaffs_Object *obj;

	if (!(yaffs_traceMask & YAFFS_TRACE_ACP_DEBUG))
		return YAFFS_OK;

	if (dev->acpLoggingStage == ACPLOGGING_STAGE_NORMAL) {
		obj = yaffs_FindObjectByNumber(dev, acpr->objectId);
		for (i = 0; i < acpr->size; i++) {
			int findChunkAddr = 0;
			offset = acpr->startOffset + i;
			chunkInNAND = acpr->startAddress + i;
			if (obj) {
				if (offset == 0)
					findChunkAddr = obj->hdrChunk;
				else
					findChunkAddr = yaffs_FindChunkInFile(obj, offset, &tags);
				if (findChunkAddr == chunkInNAND)
					continue;
			}

			if (i < acpr->size-1)
				byteCount = dev->nDataBytesPerChunk;
			else
				byteCount = dev->nDataBytesPerChunk - acpr->nBlankBytesInLastPage;

			yaffs_ReadChunkWithTagsFromNAND(dev, chunkInNAND, NULL, &tags);
			if (tags.objectId != acpr->objectId ||
				 tags.chunkId != (acpr->startOffset+i) || 
				 (tags.chunkId!=0 && tags.byteCount!=byteCount) ||
				 tags.sequenceNumber > acpr->version) {
				if (obj)
					T(YAFFS_TRACE_ACP_ERROR,
						(TSTR("[ACP] %s: oid %d off %d in ACPR stored at %d,"
						" but yaffs_FindChunkInFile find at %d" TENDSTR),
						funcName, obj->objectId, acpr->startOffset+i,
						chunkInNAND, findChunkAddr));
				T_ACPR_DIFF(dev, YAFFS_TRACE_ACP_ERROR, acpr, i, &tags, funcName);
				return YAFFS_FAIL;
			}
		}
	} else if (dev->acpLoggingStage == ACPLOGGING_STAGE_ACPR_COLLECTING ||
			dev->acpLoggingStage == ACPLOGGING_STAGE_ACPR_SCANNING) {
		for (i = 0; i < acpr->size; i++) {
			chunkInNAND = acpr->startAddress + i;
			offset = acpr->startOffset + i;

			if (i < acpr->size-1)
				byteCount = dev->nDataBytesPerChunk;
			else
				byteCount = dev->nDataBytesPerChunk - acpr->nBlankBytesInLastPage;

			yaffs_ReadChunkWithTagsFromNAND(dev, chunkInNAND, NULL, &tags);
			if (tags.objectId != acpr->objectId ||
				 tags.chunkId != offset || 
				 (tags.chunkId!=0 && tags.byteCount!=byteCount) ||
				 tags.sequenceNumber > acpr->version) {
				T_ACPR_DIFF(dev, YAFFS_TRACE_ACP_ERROR, acpr, i, &tags, funcName);
				return YAFFS_FAIL;
			}
		}
	}

	return YAFFS_OK;
}

int yaffs_ACPVerifyACPRDirtyList(yaffs_Device *dev)
{
	ACPRecord *acpr;
	struct ylist_head *entry;
	RListEntry *rlEntry;
	int result = YAFFS_OK;

	entry = dev->acprDirtyList.next;

	while (entry != &dev->acprDirtyList) {
		rlEntry = ylist_entry(entry, RListEntry, dirty_entry);
		entry = entry->next;

		acpr = &(rlEntry->acpr);
		if (yaffs_ACPVerifyACPR(dev, acpr, __FUNCTION__) == YAFFS_FAIL)
			result = YAFFS_FAIL;
	}

	return result;
}

void yaffs_UnpackExtInfoFromACPR(ACPRecord *acpr, ACPExtraInfo *acpExtInfo)
{
	memset(acpExtInfo, 0, sizeof(ACPExtraInfo));

	if (acpr->startOffset & EXTRA_HEADER_INFO_FLAG) {
		acpExtInfo->extraHeaderInfoAvailable = 1;
		acpExtInfo->extraParentObjectId = acpr->startOffset & ~EXTRA_OBJECT_TYPE_MASK;
		acpExtInfo->extraIsShrinkHeader = 
			(acpr->startOffset & EXTRA_SHRINK_FLAG) ? 1 : 0;;
		acpExtInfo->extraShadows = 
			(acpr->startOffset & EXTRA_SHADOWS_FLAG) ? 1 : 0;

		acpExtInfo->extraObjectType = acpr->objectId >> EXTRA_OBJECT_TYPE_SHIFT;

		if (acpExtInfo->extraObjectType == YAFFS_OBJECT_TYPE_HARDLINK)
			acpExtInfo->extraEquivalentObjectId = acpr->extInfo;
		else
			acpExtInfo->extraFileLength = acpr->extInfo;

		acpr->startOffset = 0;
		acpr->objectId &= ~EXTRA_OBJECT_TYPE_MASK;
	}
}

static void yaffs_PackExtInfoIntoACPR(yaffs_Device *dev, ACPRecord *acpr,
				const ACPExtraInfo *acpExtInfo)
{
	if (acpr->startOffset==0 && !acpExtInfo->extraHeaderInfoAvailable) {
		T(YAFFS_TRACE_ACP_ERROR,
			(TSTR("[ACP] No extInfo for header chunk during packing ACPR" TENDSTR)));
		YBUG(dev);
	}
	if (acpr->startOffset==0 && acpExtInfo->extraHeaderInfoAvailable) {
		acpr->startOffset = EXTRA_HEADER_INFO_FLAG | acpExtInfo->extraParentObjectId;
		if (acpExtInfo->extraIsShrinkHeader)
			acpr->startOffset |= EXTRA_SHRINK_FLAG;
		if (acpExtInfo->extraShadows)
			acpr->startOffset |= EXTRA_SHADOWS_FLAG;
		acpr->objectId &= ~EXTRA_OBJECT_TYPE_MASK;
		acpr->objectId |= (acpExtInfo->extraObjectType << EXTRA_OBJECT_TYPE_SHIFT);
		if (acpExtInfo->extraObjectType == YAFFS_OBJECT_TYPE_HARDLINK)
			acpr->extInfo = acpExtInfo->extraEquivalentObjectId;
		else if (acpExtInfo->extraObjectType == YAFFS_OBJECT_TYPE_FILE)
			acpr->extInfo = acpExtInfo->extraFileLength;
		else
			acpr->extInfo = 0;
	}
}

int yaffs_ACPEraseACPRGroup(yaffs_Device *dev, yaffs_ACPRGroupInfo *acprGroup)
{
	int i;

	for(i = acprGroup->nACPBlocks - 1; i>=0; i--) {
		int blk = acprGroup->acpBlockList[i];
		yaffs_BlockInfo *bi = yaffs_GetBlockInfo(dev, blk);

		T(YAFFS_TRACE_ACP_DETAIL,
			(TSTR("[ACP] erasing ACPR group: groupSeq %d block %d" TENDSTR),
			acprGroup->groupSequence, blk));

		if (dev->eraseBlockInNAND(dev, blk-dev->blockOffset)){
			bi->blockState = YAFFS_BLOCK_STATE_EMPTY;
			dev->nErasedBlocks++;
			dev->nFreeChunks += dev->nChunksPerBlock;
		} else {
			dev->markNANDBlockBad(dev, blk);
			bi->blockState = YAFFS_BLOCK_STATE_DEAD;
		}
		acprGroup->acpBlockList[i] = -1;
	}

	dev->nACPRGroups--;
	acprGroup->nACPBlocks = 0;

	return YAFFS_OK;
}

/*
 * Erase the ACPR group other than the current ACPR group
 */
static int yaffs_ACPEraseDeprecatedACPRGroup(yaffs_Device *dev)
{
	int deprecatedGroupIndex;
	yaffs_ACPRGroupInfo *deprecatedACPRGroup;

	if (dev->nACPRGroups!=2) {
		YBUG(dev);
		return YAFFS_FAIL;
	}

	deprecatedGroupIndex = (dev->currACPRGroupIndex+1) & 1;
	deprecatedACPRGroup = &dev->acprGroup[deprecatedGroupIndex];

	T(YAFFS_TRACE_ACP_INFO,
		(TSTR("[ACP] Erase deprecated ACPR group, Gindex %d Gseq %d GPseq %d"
		TENDSTR), deprecatedGroupIndex,
		deprecatedACPRGroup->groupSequence,
		deprecatedACPRGroup->pageSequence));
	yaffs_ACPEraseACPRGroup(dev, deprecatedACPRGroup);
	T(YAFFS_TRACE_ACP_INFO, (TSTR("[ACP] Deprecated ACPR group erased,"
		" left nACPRGroups %d" TENDSTR), dev->nACPRGroups));

	return YAFFS_OK;
}

/*
 * Erase blocks in all current ACPR groups
 */
void yaffs_ACPEraseACPRGroups(yaffs_Device *dev)
{
	yaffs_ACPRGroupInfo *acprGroup;
	if (dev->nACPRGroups == 0)
		return;

	if (dev->nACPRGroups == 1) {
		acprGroup = yaffs_GetCurrentACPRGroup(dev);
		if (!acprGroup) {
			YBUG(dev);
			return;
		}
		yaffs_ACPEraseACPRGroup(dev, acprGroup);
		return;
	}

	if (dev->nACPRGroups == 2) {
		int anotherIndex = (dev->currACPRGroupIndex + 1) & 1;
		if (dev->currACPRGroupIndex != 0 && dev->currACPRGroupIndex != 1) {
			YBUG(dev);
			return;
		}
		yaffs_ACPEraseACPRGroup(dev, &dev->acprGroup[dev->currACPRGroupIndex]);
		yaffs_ACPEraseACPRGroup(dev, &dev->acprGroup[anotherIndex]);
		return;
	}

	YBUG(dev);
}

int yaffs_ACPWriteACPRGroupHeader(yaffs_Device *dev, yaffs_ACPRGroupInfo *acprGroup)
{
	__u8 *buffer;
	int chunk, result;
	yaffs_BlockInfo *bi;
	yaffs_ExtendedTags tags;
	yaffs_acprGroupHeader *acprGroupHeader;

	if (acprGroup->pageSequence != 0) {
		YBUG(dev);
		return YAFFS_FAIL;
	}
	/* allocate an ACP block */
	yaffs_ACPFindNextErasedBlock(dev);
	acprGroup->currBlock = dev->acpNextBlock;
	acprGroup->currChunk = 0;
	if (acprGroup->currBlock < 0)
		return YAFFS_FAIL;
	acprGroup->acpBlockList[acprGroup->nACPBlocks++] = acprGroup->currBlock;

	buffer = yaffs_GetTempBuffer(dev, __LINE__);
	acprGroupHeader = (yaffs_acprGroupHeader*)buffer;
	memset(buffer, 0xff, dev->totalBytesPerChunk);

	bi = yaffs_GetBlockInfo(dev, acprGroup->currBlock);
	bi->blockState = YAFFS_BLOCK_STATE_ACP;
	bi->sequenceNumber = acprGroup->blockSequence;
	dev->nFreeChunks -= dev->nChunksPerBlock;
	dev->nErasedBlocks--;
	dev->acpByteOffset = 0;

	/* write group header */
	yaffs_InitialiseTags(&tags);
	tags.chunkDeleted = 0;
	tags.chunkUsed = 1;
	tags.objectId = dev->acpNextBlock;
	tags.chunkId = acprGroup->pageSequence++;
	tags.sequenceNumber = acprGroup->blockSequence;
	tags.byteCount = sizeof(yaffs_acprGroupHeader);

	acprGroupHeader->groupSequence = acprGroup->groupSequence;

	chunk = acprGroup->currBlock * dev->nChunksPerBlock + acprGroup->currChunk;
	result = yaffs_WriteChunkWithTagsToNAND(dev, chunk, buffer, &tags);
	if (result == YAFFS_FAIL)
		T(YAFFS_TRACE_ACP_ERROR, (TSTR("[ACP] ACPR group header"
			" write failure: blk %d seq %u chunk %d groupseq %d" TENDSTR),
			acprGroup->currBlock, acprGroup->blockSequence,
			acprGroup->currChunk, acprGroup->groupSequence));

	yaffs_ReleaseTempBuffer(dev, buffer, __LINE__);
	return result;
}

/*
 * start a new ACPR group
 */
int yaffs_ACPStartNewACPRGroup(yaffs_Device *dev)
{
	yaffs_ACPRGroupInfo *oldACPRGroup;
	yaffs_ACPRGroupInfo *newACPRGroup;
	struct ylist_head *temp;
	unsigned int newACPRGroupSeq;
	int newGroupIndex, result = YAFFS_OK;

	if (dev->nACPRGroups == 2) {
		YBUG(dev);
		return YAFFS_FAIL;
	}

	dev->isWritingNewACPRGroup = 1;

	if (dev->nACPRGroups == 1) {
		oldACPRGroup = yaffs_GetCurrentACPRGroup(dev);
		if (!oldACPRGroup)
			YBUG(dev);
		newGroupIndex = (dev->currACPRGroupIndex + 1) & 1;
		newACPRGroupSeq = oldACPRGroup->groupSequence + 1;
	} else {
		newGroupIndex = 0;
		newACPRGroupSeq = 0;
	}

	newACPRGroup = &(dev->acprGroup[newGroupIndex]);
	yaffs_InitACPRGroupInfo(dev, newACPRGroup);
	newACPRGroup->groupSequence = newACPRGroupSeq;

	T(YAFFS_TRACE_ACP_INFO,
		(TSTR("[ACP] Start a new ACPR group, Gind %d Gseq %d W %d E %d, devSeq %d"
		TENDSTR), newGroupIndex, newACPRGroupSeq,
		dev->nPageWrites, dev->nBlockErasures,
		dev->sequenceNumber));

	result = yaffs_ACPWriteACPRGroupHeader(dev, newACPRGroup);
	if (result == YAFFS_FAIL) {
		T(YAFFS_TRACE_ACP_ERROR,
			(TSTR("[ACP] Disable ACP because group header write failure!" TENDSTR)));
		dev->acpDisabled = 1;
		goto newACPRGroupFinish;
	}

	/* switch to the new ACPR group */
	newACPRGroup->currChunk++;
	dev->nACPRGroups++;
	dev->currACPRGroupIndex = newGroupIndex;

	/* commit all current valid ACPRs */
	temp = dev->acprCleanList.prev;
	if (verify_clean_list)
		yaffs_traceMask |= YAFFS_TRACE_ACP_DEBUG;
	while (temp != &dev->acprCleanList && result==YAFFS_OK) {
		RListEntry * rlEntry;
		rlEntry = ylist_entry(temp, RListEntry, clean_entry);
		temp = temp->prev;

		if ((!rlEntry->objHasBeenDeleted) &&
		   yaffs_ACPVerifyACPR(dev, &rlEntry->acpr, __FUNCTION__) == YAFFS_FAIL){
			if (rlEntry->objHasBeenDeleted)
				T(YAFFS_TRACE_ACP_ERROR, (TSTR("[ACP] OBJ has been deleted"
				  "should not commit this ACPR" TENDSTR)));
			T(YAFFS_TRACE_ACP_ERROR, (TSTR(
				"[ACP] Clean ACPR verification error, force ACP disabled" TENDSTR)));
			yaffs_ACPR_PrintLoggingHistory();
			dev->acpDisabled = 1;
			break;
		}
		if (yaffs_ACPWriteACPR(dev, &rlEntry->acpr, &rlEntry->acpExtInfo)<0) {
			T(YAFFS_TRACE_ACP_ERROR,
				(TSTR("[ACP] Write clean ACPR list failed" TENDSTR)));
			result = YAFFS_FAIL;
			break;
		}
		yaffs_RMFromACPRDirtyList(dev, rlEntry);
		if (rlEntry->objHasBeenDeleted) {
			T(YAFFS_TRACE_ACP_DETAIL,
				(TSTR("[ACP] Free ACPR, obj has been deleted" TENDSTR)));
			yaffs_RMFromACPRCleanList(dev, rlEntry);
			RList_FreeRListEntry(dev, rlEntry);
		}
	}
	yaffs_traceMask &= ~YAFFS_TRACE_ACP_DEBUG;

	T(YAFFS_TRACE_ACP_INFO,
		(TSTR("[ACP] A new ACPR group created, Gind %d Gseq %d W %d E %d" TENDSTR),
		newGroupIndex, newACPRGroupSeq, dev->nPageWrites, dev->nBlockErasures));

newACPRGroupFinish:
	dev->isWritingNewACPRGroup = 0;
	return result;
}

static int yaffs_ACPRNeedsNewACPRGroup(yaffs_Device *dev)
{
	int needNewACPRGroup = 0;
	yaffs_ACPRGroupInfo *currACPRGroup;

	if (dev->isWritingNewACPRGroup)
		return 0;

	currACPRGroup = yaffs_GetCurrentACPRGroup(dev);
	if (!currACPRGroup) {
		YBUG(dev);
		return 1;
	}

	if (currACPRGroup->pageSequence >= dev->nACPRGroupMaxBlocks*dev->nChunksPerBlock)
		/* 1. current ACPR group is full */
		needNewACPRGroup = 1;
	else if ((currACPRGroup->pageSequence >= dev->nChunksPerBlock) &&
		   (2*currACPRGroup->nDeprecatedPagesCovered >= currACPRGroup->nTotalPagesCovered))
		/* 2. too many deprecated pages in current ACPR group*/ 
		   needNewACPRGroup = 1;
	else if ((currACPRGroup->pageSequence >= dev->nChunksPerBlock) &&
		(currACPRGroup->pageSequence*dev->nMaxACPRBytesPerChunk >=
			2*currACPRGroup->nACPRs*sizeof(ACPRecord)))
		/* 3. current ACP pages take up at least 1 block,
		 *    but waste more than 50% space in ACP pages
		 */
			needNewACPRGroup = 1;

	return needNewACPRGroup;
}

static int yaffs_ACPRNeedsEraseDeprecatedACPRGroup(yaffs_Device *dev, int needNewACPRGroup)
{
	if (dev->isWritingNewACPRGroup)
		return 0;
	if ((dev->nACPRGroups == 2) &&
		((dev->acpLoggingStage == ACPLOGGING_STAGE_ACPR_SCANNING) ||
		 (dev->acpLoggingStage == ACPLOGGING_STAGE_NORMAL && needNewACPRGroup))){
		   return 1;
	} else
		return 0;
}

static void yaffs_CheckACPRCleanUp(yaffs_Device *dev)
{
	int needNewACPRGroup = yaffs_ACPRNeedsNewACPRGroup(dev);
	if (yaffs_ACPRNeedsEraseDeprecatedACPRGroup(dev, needNewACPRGroup))
		yaffs_ACPEraseDeprecatedACPRGroup(dev);
	if (needNewACPRGroup)
		yaffs_ACPStartNewACPRGroup(dev);
}

int yaffs_ACPFlushBuffer(yaffs_Device *dev)
{
	int result, tries;
	int chunk;
	yaffs_ACPRGroupInfo *acprGroup;
	yaffs_ExtendedTags tags;

	if (dev->acpDisabled)
		return YAFFS_OK;

	if (!dev->acpByteOffset) {
		T(YAFFS_TRACE_ACP_ERROR, (TSTR("[ACP] No ACPRs in buffer" TENDSTR)));
		return YAFFS_FAIL;
	}
	if (dev->acpByteOffset%dev->acprSize) {
		YBUG(dev);
		T(YAFFS_TRACE_ACP_ERROR,
			(TSTR("[ACP] Flush ACP buffer, size error, acpByteOffset %d"),
			dev->acpByteOffset));
		return YAFFS_FAIL;
	}

	yaffs_CheckACPRCleanUp(dev);
	/* cleanup may cause acpByteOffset==0 if start a new ACPR group*/
	if (!dev->acpByteOffset)
		return YAFFS_OK;

	acprGroup = yaffs_GetCurrentACPRGroup(dev);
	if (!acprGroup) {
		YBUG(dev);
		return YAFFS_FAIL;
	}
	if (!(acprGroup->nACPBlocks<dev->nACPRGroupMaxBlocks ||
		(acprGroup->nACPBlocks==dev->nACPRGroupMaxBlocks &&
		 acprGroup->currChunk<=dev->nChunksPerBlock-1))) {
		T(YAFFS_TRACE_ACP_ERROR,
			(TSTR("[ACP] ACP block gets full, nACPBlocks %d currChunk %d pageSeq %d"
			TENDSTR), acprGroup->nACPBlocks, acprGroup->currChunk,
			acprGroup->pageSequence));
		YBUG(dev);
		return YAFFS_FAIL;
	}

	tries = 0;
	do{
		if (acprGroup->currBlock < 0) {
			yaffs_ACPFindNextErasedBlock(dev);
			acprGroup->currBlock = dev->acpNextBlock;
			acprGroup->currChunk = 0;
		}

		if (acprGroup->currBlock < 0)
			return YAFFS_FAIL;

		if (acprGroup->currChunk == 0) {
			/* First chunk we write for the block? Set block state to ACP */
			yaffs_BlockInfo *bi = yaffs_GetBlockInfo(dev,acprGroup->currBlock);
			bi->blockState = YAFFS_BLOCK_STATE_ACP;
			bi->sequenceNumber = acprGroup->blockSequence;

			acprGroup->acpBlockList[acprGroup->nACPBlocks++] = acprGroup->currBlock;
			dev->nFreeChunks -= dev->nChunksPerBlock;
			dev->nErasedBlocks--;
		}

		yaffs_InitialiseTags(&tags);
		tags.chunkDeleted = 0;
		tags.chunkUsed = 1;
		tags.objectId = dev->acpNextBlock; /* Hint to next place to look */
		tags.chunkId = acprGroup->pageSequence++;
		tags.sequenceNumber =  acprGroup->blockSequence;
		tags.byteCount = dev->acpByteOffset; /* How many ACP bytes is stored in the page */

		chunk = acprGroup->currBlock * dev->nChunksPerBlock + acprGroup->currChunk;
		result = yaffs_WriteChunkWithTagsToNAND(dev,chunk,dev->acpBuffer,&tags);
		T(YAFFS_TRACE_ACP_DETAIL,
			(TSTR("[ACP] Flush ACP chunk blk %d chunk %d blkseq %u pSeq %u"
			" byteCount %d" TENDSTR),
			acprGroup->currBlock, acprGroup->currChunk,
			acprGroup->blockSequence, acprGroup->pageSequence, tags.byteCount));

#ifdef TEST
		if (simACPRwerr) {
			nACPRwerr++;
			result = YAFFS_FAIL;
			simACPRwerr = 0;
		}
#endif

		dev->nACPPageWrites++;
		acprGroup->currChunk++;
		if (acprGroup->currChunk >= dev->nChunksPerBlock) {
			acprGroup->currChunk = 0;
			acprGroup->currBlock = -1;
		}
		tries++;
	} while(0/*result==YAFFS_FAIL && tries<=YAFFS_WR_ATTEMPTS*/);

	acprGroup->nACPRs += dev->acpByteOffset/dev->acprSize;
	dev->acpByteOffset = 0;
	memset(dev->acpBuffer, 0xff, dev->nDataBytesPerChunk);

	if (result == YAFFS_FAIL) {
		yaffs_BlockInfo *bi = yaffs_GetBlockInfo(dev, acprGroup->currBlock);

		T(YAFFS_TRACE_ACP_ERROR, (TSTR("[ACP] Disable ACP because"
			" ACPR write failure of blk %d chunk %d, " \
			"and erase the failed block!" TENDSTR),
			acprGroup->currBlock, acprGroup->currChunk));
		dev->acpDisabled = 1;

		if(dev->eraseBlockInNAND(dev, acprGroup->currBlock - dev->blockOffset)){
			bi->blockState = YAFFS_BLOCK_STATE_EMPTY;
			dev->nErasedBlocks++;
			dev->nFreeChunks += dev->nChunksPerBlock;
		} else {
			dev->markNANDBlockBad(dev, acprGroup->currBlock);
			bi->blockState = YAFFS_BLOCK_STATE_DEAD;
		}

		return YAFFS_FAIL;
	}

	return YAFFS_OK;
}

/*
 * return number of bytes successfully written
 * -1 to indicate write error
 */
int yaffs_ACPWriteACPR(yaffs_Device *dev, const ACPRecord *acpr,
			const ACPExtraInfo *acpExtInfo)
{
	int ok = YAFFS_OK;
	ACPRecord *localACPR;
	yaffs_ACPRGroupInfo *acprGroup;
	
	acprGroup = yaffs_GetCurrentACPRGroup(dev);
	if (!acprGroup) {
		YBUG(dev);
		return -1;
	}

	T(YAFFS_TRACE_ACP_DETAIL,
		(TSTR("[ACP] Writing ACPR oid %d off %d addr %d size %d "
		"blankByte %d ver %d, doingGC %d" TENDSTR),
		acpr->objectId, acpr->startOffset, acpr->startAddress, acpr->size,
		acpr->nBlankBytesInLastPage, acpr->version, dev->isDoingGC));

	if (dev->acpBuffer == NULL)
		return -1;

	localACPR = (ACPRecord *)(dev->acpBuffer+dev->acpByteOffset);
	memcpy(localACPR, acpr, dev->acprSize);

	yaffs_PackExtInfoIntoACPR(dev, localACPR, acpExtInfo);
	localACPR->crc = YCRC32(0, (void *)localACPR, sizeof(ACPRecord)-4);

	dev->acpByteOffset += dev->acprSize;
	acprGroup->nTotalPagesCovered += acpr->size;

	/* ACPR filled an ACP chunk, so flush them to Flash */
	if (dev->acpByteOffset == dev->nMaxACPRBytesPerChunk)
		ok = yaffs_ACPFlushBuffer(dev);
	else if (dev->acpByteOffset > dev->nMaxACPRBytesPerChunk)
		YBUG(dev);

	if (ok==YAFFS_FAIL)
		return -1;
	return 	dev->acprSize;
}

/*
 * management of ACPR clean/dirty list
 */
void yaffs_AddToACPRCleanList(yaffs_Device *dev, RListEntry *rlEntry)
{
	if (dev->acpLoggingStage == ACPLOGGING_STAGE_NORMAL) {
		/* rlEntry is new, add to head */
		if (!ylist_empty(&rlEntry->clean_entry)) {
			ylist_del(&rlEntry->clean_entry);
			ylist_add(&rlEntry->clean_entry, &dev->acprCleanList);
		} else {
			ylist_add(&rlEntry->clean_entry, &dev->acprCleanList);
			dev->nCleanACPRs++;
		}
	} else if (dev->acpLoggingStage == ACPLOGGING_STAGE_DATABLOCK_SCANNING ||
			dev->acpLoggingStage == ACPLOGGING_STAGE_ACPR_SCANNING) {
		/* rlEntry is old, add to tail only when not in clean acpr list
		 * because we are backward
		 */
		if (ylist_empty(&rlEntry->clean_entry)) {
			ylist_add(&rlEntry->clean_entry, dev->acprCleanList.prev);
			dev->nCleanACPRs++;
		}
	} else
		YBUG(dev);
}

void yaffs_RMFromACPRCleanList(yaffs_Device *dev, RListEntry *rlEntry)
{
	if (!ylist_empty(&rlEntry->clean_entry)) {
		ylist_del_init(&rlEntry->clean_entry);
		dev->nCleanACPRs--;
	}
}

void yaffs_AddToACPRDirtyList(yaffs_Device *dev, RListEntry *rlEntry)
{
	if (ylist_empty(&rlEntry->clean_entry))
		YBUG(dev);
	if (dev->acpLoggingStage == ACPLOGGING_STAGE_NORMAL) {
		if (!ylist_empty(&rlEntry->dirty_entry)) {
			/* already in ACP dirty list, then readd it to the head*/
			ylist_del(&rlEntry->dirty_entry);
			ylist_add(&rlEntry->dirty_entry, &dev->acprDirtyList);
		} else{
			/* add to the head of dirty list */
			ylist_add(&rlEntry->dirty_entry, &dev->acprDirtyList);
			dev->nDirtyACPRs++;
		}
	} else if (dev->acpLoggingStage == ACPLOGGING_STAGE_DATABLOCK_SCANNING) {
		/*
		 * Only need to consider rlEntry isnot in the dirty list.
		 *
		 * If already in ACP dirty list, we must not re-add rlEntry
		 * to the tail of dirty list because we are **backward**.
		 * This may happen under GC which make older physically adjacent
		 * to newer chunk.
		 *
		 */
		if (ylist_empty(&rlEntry->dirty_entry)) {
			/* add to the tail of dirty list */
			ylist_add(&rlEntry->dirty_entry, dev->acprDirtyList.prev);
			dev->nDirtyACPRs++;
		}
	} else if (dev->acpLoggingStage == ACPLOGGING_STAGE_ACPR_SCANNING) {
		/* already on flash, no need to add to dirty list */
		return;
	} else
		YBUG(dev);
}

void yaffs_RMFromACPRDirtyList(yaffs_Device *dev, RListEntry *rlEntry)
{
	if (!ylist_empty(&rlEntry->dirty_entry)) {
		ylist_del_init(&rlEntry->dirty_entry);
		dev->nDirtyACPRs--;
	}
}

static void yaffs_CheckDirtyACPRs(yaffs_Device *dev)
{
	struct ylist_head *entry;
	int nDirtyACPRs = 0;
	int nDirtyACPRsInCleanList = 0;
	RListEntry *rlEntry;

	entry = dev->acprCleanList.next;
	while (entry != &dev->acprCleanList) {
		rlEntry = ylist_entry(entry, RListEntry, clean_entry);
		if (!ylist_empty(&rlEntry->dirty_entry))
			nDirtyACPRsInCleanList++;
		entry = entry->next;
	}
	entry = dev->acprDirtyList.next;
	while (entry != &dev->acprDirtyList) {
		nDirtyACPRs++;
		entry = entry->next;
	}
	if (nDirtyACPRs != dev->nDirtyACPRs)
		YBUG(dev);
	if (nDirtyACPRs != nDirtyACPRsInCleanList)
		YBUG(dev);
}

/*
 * return:
 * 1	Don't need commit dirty list
 * 0	Need to commit dirty list
 */
static int yaffs_ACPNeedCommitACPRDirtyList(yaffs_Device *dev)
{
	yaffs_ACPRGroupInfo *acprGroup = yaffs_GetCurrentACPRGroup(dev);
	if (acprGroup->nPagesUncovered >= dev->acpMaxCoveredChunksPerCommit) {
		/* by default: the limit is dev->nChunksPerBlock*/
		return 1;
	}
	return 0;
}

int yaffs_ACPCommitACPRDirtyList(yaffs_Device *dev, int force)
{
	struct ylist_head *entry;
	struct ylist_head *dirty_list_head = &(dev->acprDirtyList);
	RListEntry *rlEntry;
	ACPRecord *acpr;
	ACPExtraInfo *acpExtInfo;
	int res = YAFFS_OK;
	yaffs_ACPRGroupInfo *acprGroup = yaffs_GetCurrentACPRGroup(dev);

	if(!yaffs_ACPNeedCommitACPRDirtyList(dev) && !force)
		return YAFFS_OK;

	if (verify_dirty_list)
		yaffs_CheckDirtyACPRs(dev);

	/*
	 * write dirty ACP records
	 * ACPRWrite may cause dirty entries discarded if starting a new ACPR group
	 */
	T(YAFFS_TRACE_ACP_DETAIL,
		(TSTR("[ACP] Commit ACPR dirtyList nDirty %d W %d E %d nPagesUncovered=%d"
		TENDSTR), dev->nDirtyACPRs, dev->nPageWrites,
		dev->nBlockErasures, acprGroup->nPagesUncovered));

	if (verify_dirty_list)
		yaffs_traceMask |= YAFFS_TRACE_ACP_DEBUG;
	entry = dirty_list_head->prev;
	/* nDirtyACPRs may get to 0 in yaffs_ACPStartNewACPRGroup */
	while ((entry != dirty_list_head) &&
			(dev->nDirtyACPRs > 0)) {
		rlEntry = ylist_entry(entry, RListEntry, dirty_entry);
		acpr = &rlEntry->acpr;
		acpExtInfo = &rlEntry->acpExtInfo;
		/*
		 * should move to prev before write ACPR
		 * it may get freed in yaffs_ACPStartNewACPRGroup
		 */
		entry = entry->prev;

		if ((!rlEntry->objHasBeenDeleted) &&
		   yaffs_ACPVerifyACPR(dev, acpr, __FUNCTION__) == YAFFS_FAIL) {
			res = YAFFS_FAIL;
			break;
		}
		if (yaffs_ACPWriteACPR(dev, acpr, acpExtInfo) < 0) {
			res = YAFFS_FAIL;
			break;
		}
	}
	yaffs_traceMask &= ~YAFFS_TRACE_ACP_DEBUG;

	if (res == YAFFS_FAIL) {
		T(YAFFS_TRACE_ACP_ERROR,
			(TSTR("[ACP] Commit ACPR dirty list err, force ACP disabled" TENDSTR)));
		dev->acpDisabled = 1;

		yaffs_ACPR_PrintLoggingHistory();

		T(YAFFS_TRACE_ACP_ERROR,
			(TSTR("[ACP] === dumping device dirty acpr list ===" TENDSTR)));
		entry = dirty_list_head->prev; 
		while (entry != dirty_list_head) {
			rlEntry = ylist_entry(entry, RListEntry, dirty_entry);
			entry = entry->prev;

			acpr = &rlEntry->acpr;
			T_ACPR(YAFFS_TRACE_ACP_ERROR, acpr, __FUNCTION__, __LINE__);
		}
		T(YAFFS_TRACE_ACP_ERROR,
			(TSTR("[ACP] === device dirty acpr list finished dumping ===" TENDSTR)));

	} else {

		if (dev->acpByteOffset)
			yaffs_ACPFlushBuffer(dev);

		acprGroup->nPagesUncovered = 0;
	}
	T(YAFFS_TRACE_ACP_DETAIL,
		(TSTR("[ACP] Commit ACPR dirtyList finished, W %d E %d" TENDSTR),
		dev->nPageWrites, dev->nBlockErasures));

	/* remove the ACP record from dirty list */
	while (dirty_list_head->prev != dirty_list_head) {
		entry = dirty_list_head->prev;
		rlEntry = ylist_entry(entry, RListEntry, dirty_entry);
		acpr = &rlEntry->acpr;

		if (rlEntry->objHasBeenDeleted) {
			/* free rlEntry */
			T(YAFFS_TRACE_ACP_DETAIL, (TSTR("Free ACPR, obj has been deleted" TENDSTR)));
			T_ACPR(YAFFS_TRACE_ACP_DETAIL, &rlEntry->acpr, __FUNCTION__, __LINE__);
			yaffs_RMFromACPRDirtyList(dev, rlEntry);
			yaffs_RMFromACPRCleanList(dev, rlEntry);
			RList_FreeRListEntry(dev, rlEntry);

		} else {
			yaffs_Object *in;

			yaffs_RMFromACPRDirtyList(dev, rlEntry);

			/* TODO: verify the existence of the object, may be ommitted*/
			in = yaffs_FindObjectByNumber(dev, acpr->objectId);
			if (!in) {
				YBUG(dev);
				T(YAFFS_TRACE_ACP_ERROR,
				  (TSTR("[ACP] %s, rlEntry(%p) obj(%d) already unhashed,"
				  " memroy leak" TENDSTR),
				  __FUNCTION__, rlEntry, acpr->objectId));
				T_ACPR(YAFFS_TRACE_ACP_ERROR, &(rlEntry->acpr),
					__FUNCTION__, __LINE__);
			}
		}
	}

	return res;
}

