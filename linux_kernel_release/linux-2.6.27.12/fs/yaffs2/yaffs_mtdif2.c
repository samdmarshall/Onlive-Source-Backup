/*
 * YAFFS: Yet Another Flash File System. A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2007 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/* mtd interface for YAFFS2 */

const char *yaffs_mtdif2_c_version =
	"$Id: yaffs_mtdif2.c,v 1.23 2009/03/06 17:20:53 wookey Exp $";

#include "yportenv.h"


#include "yaffs_mtdif2.h"

#include "linux/mtd/mtd.h"
#include "linux/types.h"
#include "linux/time.h"

#include "yaffs_packedtags2.h"

/* NB For use with inband tags....
 * We assume that the data buffer is of size totalBytersPerChunk so that we can also
 * use it to load the tags.
 */
int nandmtd2_WriteChunkWithTagsToNAND(yaffs_Device *dev, int chunkInNAND,
				      const __u8 *data,
				      const yaffs_ExtendedTags *tags)
{
	struct mtd_info *mtd = (struct mtd_info *)(dev->genericDevice);
#if (MTD_VERSION_CODE > MTD_VERSION(2, 6, 17))
	struct mtd_oob_ops ops;
#else
	size_t dummy;
#endif
	int retval = 0;

	loff_t addr;
	yaffs_PackedTags2 pt;

	T(YAFFS_TRACE_MTD,
	  (TSTR
	   ("nandmtd2_WriteChunkWithTagsToNAND chunk %d data %p tags %p"
	    TENDSTR), chunkInNAND, data, tags));

	addr  = ((loff_t) chunkInNAND) * dev->totalBytesPerChunk;

	/* For yaffs2 writing there must be both data and tags.
	 * If we're using inband tags, then the tags are stuffed into
	 * the end of the data buffer.
	 */
	if (!data || !tags)
		BUG();
	else if (dev->inbandTags) {
		yaffs_PackedTags2TagsPart *pt2tp;
		pt2tp = (yaffs_PackedTags2TagsPart *)(data + dev->nDataBytesPerChunk);
		yaffs_PackTags2TagsPart(pt2tp, tags);
	} else
		yaffs_PackTags2(&pt, tags);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 17))
	ops.mode = MTD_OOB_AUTO;
	ops.ooblen = (dev->inbandTags) ? 0 : sizeof(pt);
	ops.len = dev->totalBytesPerChunk;
	ops.ooboffs = 0;
	ops.datbuf = (__u8 *)data;
	ops.oobbuf = (dev->inbandTags) ? NULL : (void *)&pt;
	retval = mtd->write_oob(mtd, addr, &ops);
	
#else
	if (!dev->inbandTags) {
		retval =
		    mtd->write_ecc(mtd, addr, dev->nDataBytesPerChunk,
				   &dummy, data, (__u8 *) &pt, NULL);
	} else {
		retval =
		    mtd->write(mtd, addr, dev->totalBytesPerChunk, &dummy,
			       data);
	}
#endif

	if (retval == 0)
		return YAFFS_OK;
	else{
		T(YAFFS_TRACE_ALWAYS, (TSTR("%s error, oid %d, off %d, *chunk %d" TENDSTR),
			__FUNCTION__, tags->objectId, tags->chunkId, chunkInNAND));
		return YAFFS_FAIL;
	}
}

int nReadErasedChunk = 0;
int nReadWrittenChunkErr = 0;

static int nandmtd_CheckAllFF(const __u8 *buf, int n)
{
	__u32 *p, *uintp = (__u32 *)buf + (n>>2);
	__u8 *q, *ucharp=(__u8*)buf+n;

	for (p = (__u32 *)buf; p < uintp; ++p) {
		if (*p != 0xffffffff)
			return 1;
        }
	
        for (q = (__u8*)p; q < ucharp; ++q) {
                if (*q != 0xff)
                        return 1;
        }
        return 0;
}

/*
	distinguish 2 kinds of read errors:
	a written page read error: return YAFFS_FAIL
	an erased page read error: return YAFFS_OK
*/
static int nandmtd2_ReadDistinguishErr(yaffs_Device * dev, loff_t addr, __u8 * data)
{
	struct mtd_info *mtd = (struct mtd_info *)(dev->genericDevice);
	size_t dummy = 0;
	int localData = 0;
	int ret = YAFFS_OK;

	if (!data && dev->inbandTags) {
		YBUG(dev);
		return YAFFS_FAIL;
	}
	if (!data) {
		localData = 1;
		data = yaffs_GetTempBuffer(dev, __LINE__);
		if (!data) {
			T(YAFFS_TRACE_ALWAYS, (TSTR("%s: Cannot allocate temp buffer" TENDSTR), __FUNCTION__));
			return YAFFS_FAIL;
		}
	}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 17))
	if (dev->inbandTags) {
		/* data buffer must be already filled with real page data, no need to reread */
		if (nandmtd_CheckAllFF(data, dev->totalBytesPerChunk)){
			T(YAFFS_TRACE_ALWAYS, (TSTR("%s: inband page data not all 0xff" TENDSTR), __FUNCTION__));
			ret = YAFFS_FAIL;goto finish;
		}

	} else {

		if (nandmtd_CheckAllFF(dev->spareBuffer, sizeof(yaffs_PackedTags2))) {
			T(YAFFS_TRACE_ALWAYS, (TSTR("%s: page oob not all 0xff" TENDSTR), __FUNCTION__));
			ret = YAFFS_FAIL;goto finish;
		}

		/* data buffer may not be filled, reread it!! */
		if (localData)
			mtd->read(mtd, addr, dev->totalBytesPerChunk, &dummy, data);

		if (nandmtd_CheckAllFF(data, dev->totalBytesPerChunk)){
			T(YAFFS_TRACE_ALWAYS, (TSTR("%s: page data not all 0xff" TENDSTR), __FUNCTION__));
			ret = YAFFS_FAIL;goto finish;
		}
	}
#else
	/* TODO */
#endif

finish:
	if (localData)
		yaffs_ReleaseTempBuffer(dev, data, __LINE__);

	return ret;
}

int nandmtd2_ReadChunkWithTagsFromNAND(yaffs_Device *dev, int chunkInNAND,
				       __u8 *data, yaffs_ExtendedTags *tags)
{
	struct mtd_info *mtd = (struct mtd_info *)(dev->genericDevice);
#if (MTD_VERSION_CODE > MTD_VERSION(2, 6, 17))
	struct mtd_oob_ops ops;
#endif
	size_t dummy;
	int retval = 0;
	int ret;
	int localData = 0;

	loff_t addr = ((loff_t) chunkInNAND) * dev->totalBytesPerChunk;

	yaffs_PackedTags2 pt;

	T(YAFFS_TRACE_MTD,
	  (TSTR
	   ("nandmtd2_ReadChunkWithTagsFromNAND chunk %d data %p tags %p"
	    TENDSTR), chunkInNAND, data, tags));

	if (dev->inbandTags) {

		if (!data) {
			localData = 1;
			data = yaffs_GetTempBuffer(dev, __LINE__);
		}
		

	}

	if(yaffs_traceMask & YAFFS_TRACE_READSTACK)
		dump_stack();

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 17))
	if (dev->inbandTags || (data && !tags))
		retval = mtd->read(mtd, addr, dev->totalBytesPerChunk,
				&dummy, data);
	else if (tags) {
		ops.mode = MTD_OOB_AUTO;
		ops.ooblen = sizeof(pt);
		ops.len = data ? dev->nDataBytesPerChunk : sizeof(pt);
		ops.ooboffs = 0;
		ops.datbuf = data;
		ops.oobbuf = dev->spareBuffer;
		retval = mtd->read_oob(mtd, addr, &ops);
	}
#else
	if (!dev->inbandTags && data && tags) {

		retval = mtd->read_ecc(mtd, addr, dev->nDataBytesPerChunk,
					  &dummy, data, dev->spareBuffer,
					  NULL);
	} else {
		if (data)
			retval =
			    mtd->read(mtd, addr, dev->nDataBytesPerChunk, &dummy,
				      data);
		if (!dev->inbandTags && tags)
			retval =
			    mtd->read_oob(mtd, addr, mtd->oobsize, &dummy,
					  dev->spareBuffer);
	}
#endif

	if (dev->inbandTags) {
		if (tags) {
			yaffs_PackedTags2TagsPart *pt2tp;
			pt2tp = (yaffs_PackedTags2TagsPart *)&data[dev->nDataBytesPerChunk];
			yaffs_UnpackTags2TagsPart(tags, pt2tp);
		}
	} else {
		if (tags) {
			memcpy(&pt, dev->spareBuffer, sizeof(pt));
			yaffs_UnpackTags2(tags, &pt);
		}
	}

	if (tags && retval == -EBADMSG && tags->eccResult == YAFFS_ECC_RESULT_NO_ERROR)
		tags->eccResult = YAFFS_ECC_RESULT_UNFIXED;
	if (retval == 0)
		ret = YAFFS_OK;
	else {
		if (nandmtd2_ReadDistinguishErr(dev, addr, data) == YAFFS_OK) {
			nReadErasedChunk++;
			ret = YAFFS_OK;
		} else {
			T(YAFFS_TRACE_ALWAYS, (TSTR("%s error on a non-erased page, oid %d, off %d, *chunkInNAND %d" TENDSTR),
				__FUNCTION__, tags->objectId, tags->chunkId, chunkInNAND));
			nReadWrittenChunkErr++;
			ret = YAFFS_FAIL;
		}
	}

	if (localData)
		yaffs_ReleaseTempBuffer(dev, data, __LINE__);

	return ret;
}

int nandmtd2_MarkNANDBlockBad(struct yaffs_DeviceStruct *dev, int blockNo)
{
	struct mtd_info *mtd = (struct mtd_info *)(dev->genericDevice);
	int retval;
	T(YAFFS_TRACE_MTD,
	  (TSTR("nandmtd2_MarkNANDBlockBad %d" TENDSTR), blockNo));

	retval =
	    mtd->block_markbad(mtd,
			       blockNo * dev->nChunksPerBlock *
			       dev->totalBytesPerChunk);

	if (retval == 0)
		return YAFFS_OK;
	else{
		T(YAFFS_TRACE_ALWAYS, (TSTR("%s error, blockNo %d" TENDSTR),
			__FUNCTION__, blockNo));
		return YAFFS_FAIL;
	}

}

int nandmtd2_QueryNANDBlock(struct yaffs_DeviceStruct *dev, int blockNo,
			    yaffs_BlockState *state, __u32 *sequenceNumber)
{
	struct mtd_info *mtd = (struct mtd_info *)(dev->genericDevice);
	int retval;

	T(YAFFS_TRACE_MTD,
	  (TSTR("nandmtd2_QueryNANDBlock %d" TENDSTR), blockNo));
	retval =
	    mtd->block_isbad(mtd,
			     blockNo * dev->nChunksPerBlock *
			     dev->totalBytesPerChunk);

	if (retval) {
		T(YAFFS_TRACE_MTD, (TSTR("block is bad" TENDSTR)));

		*state = YAFFS_BLOCK_STATE_DEAD;
		*sequenceNumber = 0;
	} else {
		yaffs_ExtendedTags t;
		if( nandmtd2_ReadChunkWithTagsFromNAND(dev,
						   blockNo *
						   dev->nChunksPerBlock, NULL,
						   &t) == YAFFS_OK) {
			if (t.chunkUsed) {
				*sequenceNumber = t.sequenceNumber;
				*state = YAFFS_BLOCK_STATE_NEEDS_SCANNING;
#ifdef YAFFS_ACP
				if( t.sequenceNumber == YAFFS_SEQUENCE_ACP_DATA1 ||
				    t.sequenceNumber == YAFFS_SEQUENCE_ACP_DATA2 )
					dev->acpCurrentQueryBlockPageSequence = t.chunkId;
#endif
			} else {
				*sequenceNumber = 0;
				*state = YAFFS_BLOCK_STATE_EMPTY;
			}
		} else {
			T(YAFFS_TRACE_ALWAYS,
				(TSTR("block is really bad althougn without bad mark" TENDSTR)));
			*state = YAFFS_BLOCK_STATE_DEAD;
			*sequenceNumber = 0;
			retval = -1;
		}
	}
	T(YAFFS_TRACE_MTD,
	  (TSTR("block is bad seq %d state %d" TENDSTR), *sequenceNumber,
	   *state));

	if (retval == 0)
		return YAFFS_OK;
	else
		return YAFFS_FAIL;
}

