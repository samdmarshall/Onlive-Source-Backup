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

/*
 * This provides a YAFFS nand emulation on a file for emulating 2kB pages.
 * This is only intended as test code to test persistence etc.
 */

const char *fstest_flashEm_c_version = "$Id: fstest_fileem.c,v 1.15 2008/07/21 01:03:19 charles Exp $";


#include "yportenv.h"

#include "yaffs_guts.h"
#include "devextras.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include <string.h>

#include "yaffs_packedtags2.h"

#include "fstest.h"

//#define SIMULATE_FAILURES

#define YAFFS2_TAG_OFFSET 2

#define REPORT_ERROR 0

char isFileEmulatorInitilized = 0;

int fileemPageDataSize = 2048;
int fileemPageOOBSize = 64;
int fileemPageSize = 2112;
int fileemPagesPerBlock = 128;

typedef struct
{
	int handle;
	int nBlocks;
	char * filediskpath;
	int forceBlockAlign;
	int containOOB;
	int realSize;
} fileem_Device;

char defFileDiskPath[] = "em2k.img";
static fileem_Device fileemDevice = {.filediskpath = defFileDiskPath};

int yaffs_testPartialWrite = 0;

static __u8 * localBuffer;

int FileEmulatorCheckDiskAlignment(fileem_Device * fileemDev)
{
	int bytesPerBlock;
	struct stat diskFileStat;
	int bytesToAdd;
	unsigned char * tmpBuff = NULL;
	int properSize;

	bytesPerBlock = fileemPagesPerBlock * fileemPageSize;

	if( fstat(fileemDev->handle, &diskFileStat)<0 ){
		fprintf(stderr, "can't stat disk file %s\n", fileemDevice.filediskpath);
		exit(1);
	}
	if(diskFileStat.st_size % fileemPageSize) {
		fprintf(stderr, "disk file size(%lu) is not multiple of page size(%d) containOOB(%d)\n",
				diskFileStat.st_size, fileemPageSize, fileemDev->containOOB);
		exit(1);
	}

	properSize = 32*bytesPerBlock;
	if(properSize < diskFileStat.st_size)
		properSize = (diskFileStat.st_size % bytesPerBlock) ? 
			(diskFileStat.st_size/bytesPerBlock+1)*bytesPerBlock : diskFileStat.st_size;
	bytesToAdd = properSize - diskFileStat.st_size;

	if(fileemDev->forceBlockAlign) {
		if(bytesToAdd > 0) {
			printf("disk file size(%lu) will be enlarged to %d!\n",
					diskFileStat.st_size, properSize);
			tmpBuff = malloc(bytesToAdd);
			memset(tmpBuff, 0xff, bytesToAdd);
			lseek(fileemDev->handle, 0, SEEK_END);
			if( write(fileemDev->handle, tmpBuff, bytesToAdd)<0 )
				goto failed;
		}

		/* recheck image file size */
		fstat(fileemDev->handle, &diskFileStat);
		if(diskFileStat.st_size % bytesPerBlock)
			goto failed;

		fileemDev->nBlocks = diskFileStat.st_size / bytesPerBlock;
	} else {
		fileemDev->nBlocks = properSize / bytesPerBlock;
	}

	fileemDev->realSize = diskFileStat.st_size;

	goto succeed;

failed:
	fprintf(stderr, "can't enlarge disk file to block aligned");
	if(tmpBuff)
		free(tmpBuff);
	exit(1);

succeed:
	if(tmpBuff)
		free(tmpBuff);
	return 1;
}

void FileEmulatorInit(char * diskpath, int nPagesPerBlock, int nPageDataSize, int nPageOOBSize,
	int forceBlockAlign, int imgContainOOB)
{
	fileemPagesPerBlock = nPagesPerBlock;

	fileemPageDataSize = nPageDataSize;
	fileemPageOOBSize = nPageOOBSize;
	if(imgContainOOB)
		fileemPageSize = fileemPageDataSize + fileemPageOOBSize;
	else
		fileemPageSize = nPageDataSize;

	if(diskpath){
		fileemDevice.filediskpath = (char*)malloc(strlen(diskpath)+1);
		strcpy(fileemDevice.filediskpath, diskpath);
	}
	T(YAFFS_TRACE_ALWAYS, (TSTR("File Emulator is using '%s' as the image" TENDSTR), fileemDevice.filediskpath));
	fileemDevice.handle = open(fileemDevice.filediskpath, O_RDWR, S_IREAD | S_IWRITE);
	if(fileemDevice.handle < 0 ){
		char string[512];
		sprintf(string, "Can't open '%s'", fileemDevice.filediskpath);
		perror(string);
		exit(1);
	}
	
	fileemDevice.forceBlockAlign = forceBlockAlign;
	fileemDevice.containOOB = imgContainOOB;

	FileEmulatorCheckDiskAlignment(&fileemDevice);

	localBuffer = malloc(fileemPageSize);
	if(!localBuffer) {
		fprintf(stderr, "couldn't allocate buffer for file emulator\n");
		exit(1);
	}

	isFileEmulatorInitilized = 1;
}
void FileEmulatorDeInit()
{
	free(localBuffer);
	close(fileemDevice.handle);

	if(fileemDevice.filediskpath && (fileemDevice.filediskpath != defFileDiskPath))
		free(fileemDevice.filediskpath);
}
void FileEmulatorPrintInfo()
{
	printf("** [FileEmulator] disk file name: %s\n", fileemDevice.filediskpath);
	printf("** [FileEmulator] NAND geometry: [( %d + %d ) * %d] * %d\n",
			fileemPageDataSize, fileemPageOOBSize, fileemPageSize, fileemDevice.nBlocks);
	printf("** [FileEmulator]  \n");
	printf("** [FileEmulator]  \n");
}

static void CheckInit(void)
{
	if(!isFileEmulatorInitilized) 
	{
		fprintf(stderr, "FileEmulator not initialized\n");
		exit(1);
	}
}


int fileem_GetNumberOfBlocks(void)
{
	CheckInit();
	
	return fileemDevice.nBlocks;
}

int fileem_WriteChunkWithTagsToNAND(yaffs_Device *dev,int chunkInNAND,const __u8 *data, const yaffs_ExtendedTags *tags)
{
	int written;
	int pos;
	int h;
	int i;
	int nRead;
	int error;
	
	T(YAFFS_TRACE_MTD, (TSTR("write chunk %d data %x tags %x" TENDSTR),
			chunkInNAND,(unsigned)data, (unsigned)tags));

	CheckInit();

	pos = chunkInNAND * fileemPageSize;
	h = fileemDevice.handle;
	lseek(h,pos,SEEK_SET);

	if( !fileemDevice.containOOB && !dev->inbandTags) {
		T(YAFFS_TRACE_ALWAYS, (TSTR("Image doesn't contain OOB data,"
				" file system should use inband-tags option!" TENDSTR)));
		exit(1);
	}
	if(pos >= fileemDevice.realSize) {
		T(YAFFS_TRACE_ALWAYS, (TSTR("%s: Write out of range, ForceImageBlockAlign=%d" TENDSTR),
			__FUNCTION__, fileemDevice.forceBlockAlign));
		return YAFFS_FAIL;
	}

	if(dev->inbandTags) {

		yaffs_PackedTags2TagsPart * pt2tp;
		pt2tp = (yaffs_PackedTags2TagsPart *)&data[dev->nDataBytesPerChunk];
		yaffs_PackTags2TagsPart(pt2tp,tags);

		written = write(h,data,dev->totalBytesPerChunk);

		if(yaffs_testPartialWrite){
			close(h);
			exit(1);
		}

		if(written != dev->totalBytesPerChunk) return YAFFS_FAIL;

	}	else {
		if(data)
		{
			nRead =  read(h, localBuffer, dev->nDataBytesPerChunk);
			for(i = error = 0; i < dev->nDataBytesPerChunk && !error; i++){
				if(REPORT_ERROR && localBuffer[i] != 0xFF){
					printf("file emulation: write a unerased chunk(%d) data byte %d was %0x2\n",
						chunkInNAND,i,localBuffer[i]);
					error = 1;
				}
			}

			for(i = 0; i < dev->nDataBytesPerChunk; i++)
				localBuffer[i] &= data[i];

			if(REPORT_ERROR && memcmp(localBuffer,data,dev->nDataBytesPerChunk))
				fprintf(stderr, "file emulator: data does not match\n");

			lseek(h,pos,SEEK_SET);
			written = write(h,localBuffer,dev->nDataBytesPerChunk);
		
			if(yaffs_testPartialWrite){
				close(h);
				exit(1);
			}
		
#ifdef SIMULATE_FAILURES
			if((chunkInNAND >> 6) == 100) 
				written = 0;

			if((chunkInNAND >> 6) == 110) 
				written = 0;
#endif

			if(written != dev->nDataBytesPerChunk) return YAFFS_FAIL;
		}
	
		if(tags)
		{
			pos = (chunkInNAND) * fileemPageSize + fileemPageDataSize + YAFFS2_TAG_OFFSET;
			h = fileemDevice.handle;

			lseek(h,pos,SEEK_SET);

			{
				yaffs_PackedTags2 pt;
				yaffs_PackTags2(&pt,tags);
				__u8 * ptab = (__u8 *)&pt;

				nRead = read(h,localBuffer,sizeof(pt));
				for(i = error = 0; REPORT_ERROR && i < sizeof(pt) && !error; i++){
					if(localBuffer[i] != 0xFF){
						printf("file emulation: write a unerased chunk(%d) data byte %d was %0x2\n",
							chunkInNAND,i,localBuffer[i]);
							error = 1;
					}
				}

				for(i = 0; i < sizeof(pt); i++)
					localBuffer[i] &= ptab[i];

				if(REPORT_ERROR && memcmp(localBuffer,&pt,sizeof(pt)))
					printf("file emulation: tags corruption\n");

				lseek(h,pos,SEEK_SET);

				written = write(h,localBuffer,sizeof(pt));
				if(written != sizeof(pt)) return YAFFS_FAIL;
			}
		}

	}

	return YAFFS_OK;	
}

#ifdef SIMULATE_FAILURES

static int fail300 = 1;
static int fail320 = 1;
static int failRead10 = 2;
#endif

int fileem_ReadChunkWithTagsFromNAND(yaffs_Device *dev,int chunkInNAND, __u8 *data, yaffs_ExtendedTags *tags)
{
	int nread;
	int pos;
	int h;
	int localData = 0;
	int retval = YAFFS_OK;
	
	T(YAFFS_TRACE_MTD,(TSTR("read chunk %d data %x tags %x" TENDSTR),chunkInNAND,(unsigned)data, (unsigned)tags));
	
	CheckInit();

	if( !fileemDevice.containOOB && !dev->inbandTags) {
		T(YAFFS_TRACE_ALWAYS, (TSTR("Image doesn't contain OOB data,"
				" file system should use inband-tags option!" TENDSTR)));
		exit(1);
	}

	if(dev->inbandTags){

		yaffs_PackedTags2TagsPart * pt2tp;

		/* Got to suck the tags out of the data area */
		if(!data) {
			localData=1;
			data = yaffs_GetTempBuffer(dev,__LINE__);
		}

		pt2tp = (yaffs_PackedTags2TagsPart *)&data[dev->nDataBytesPerChunk];

		pos = (chunkInNAND) * fileemPageSize;
		h = fileemDevice.handle;

		if(pos<fileemDevice.realSize) {
			lseek(h, pos, SEEK_SET);
			nread = read(h, data, dev->totalBytesPerChunk);
			if(nread != dev->totalBytesPerChunk)
				retval = YAFFS_FAIL;
		} else {
			if(fileemDevice.forceBlockAlign) {
				T(YAFFS_TRACE_ALWAYS, (TSTR("%s: read out of range" TENDSTR), __FUNCTION__));
				retval = YAFFS_FAIL;
			} else {
				memset(data, 0xff, dev->totalBytesPerChunk);
				retval = YAFFS_OK;
			}
		}

		if(retval == YAFFS_OK)
			yaffs_UnpackTags2TagsPart(tags,pt2tp);

		if(localData)
			yaffs_ReleaseTempBuffer(dev,data,__LINE__);

	} else {

		if(data)
		{

			pos = (chunkInNAND) * fileemPageSize;
			h = fileemDevice.handle;
			if(pos < fileemDevice.realSize) {
				lseek(h,pos,SEEK_SET);
				nread = read(h,data,dev->nDataBytesPerChunk);

				if(nread != dev->nDataBytesPerChunk) 
					retval = YAFFS_FAIL;
			} else {
				if(fileemDevice.forceBlockAlign) {
					T(YAFFS_TRACE_ALWAYS, (TSTR("%s: read out of range" TENDSTR), __FUNCTION__));
					retval = YAFFS_FAIL;
 				} else {
					memset(data, 0xff, dev->nDataBytesPerChunk);
					retval = YAFFS_OK;
				}
			}
		}
	
		if(tags)
		{
			pos = (chunkInNAND) * fileemPageSize + fileemPageDataSize + YAFFS2_TAG_OFFSET;
			h = fileemDevice.handle;
			lseek(h,pos,SEEK_SET);

			{
				yaffs_PackedTags2 pt;
				if(pos < fileemDevice.realSize){
					nread= read(h,&pt,sizeof(pt));
				}else{
					if( fileemDevice.forceBlockAlign ) {
						T(YAFFS_TRACE_ALWAYS, (TSTR("%s: read out of range" TENDSTR), __FUNCTION__));
						retval = YAFFS_FAIL;
					} else {
						memset(&pt, 0xff, sizeof(pt));
						retval = YAFFS_OK;
					}
				}
				if(retval == YAFFS_OK) {
					yaffs_UnpackTags2(tags,&pt);

					#ifdef SIMULATE_FAILURES
						if((chunkInNAND >> 6) == 100) {
							if(fail300 && tags->eccResult == YAFFS_ECC_RESULT_NO_ERROR){
								tags->eccResult = YAFFS_ECC_RESULT_FIXED;
								fail300 = 0;
							}
						}
						
						if((chunkInNAND >> 6) == 110) {
							if(fail320 && tags->eccResult == YAFFS_ECC_RESULT_NO_ERROR){
								tags->eccResult = YAFFS_ECC_RESULT_FIXED;
								fail320 = 0;
							}
						}
						if(failRead10>0 && chunkInNAND == 10){
							failRead10--;
							nread = 0;
						}
					#endif
					if(nread != sizeof(pt))
						retval = YAFFS_FAIL;
				}
			}
		}
	}

	return retval;	

}

int fileem_MarkNANDBlockBad(struct yaffs_DeviceStruct *dev, int blockNo)
{
	int written;
	int h;
	
	yaffs_PackedTags2 pt;

	CheckInit();
	
	memset(&pt,0,sizeof(pt));
	h = fileemDevice.handle;
	lseek(h, (blockNo * dev->nChunksPerBlock) * fileemPageSize + fileemPageDataSize, SEEK_SET);
	written = write(h,&pt,sizeof(pt));

	if(written != sizeof(pt))
		return YAFFS_FAIL;

	return YAFFS_OK;
}

int fileem_EraseBlockInNAND(yaffs_Device *dev, int blockNumber)
{

	int i;
	int h;

	CheckInit();

	T(YAFFS_TRACE_ERASE,(TSTR("erase block %d" TENDSTR), blockNumber));

#ifdef SIMULATE_FAILURES
	if(blockNumber == 320)
		fail320 = 1;
#endif

	if(blockNumber < 0 || blockNumber >= fileemDevice.nBlocks)
	{

		T(YAFFS_TRACE_ALWAYS,("Attempt to erase non-existant block %d\n",blockNumber));
		return YAFFS_FAIL;

	}	else {

		unsigned char * pg;

		if(!fileemDevice.forceBlockAlign) {
			return YAFFS_OK;
		}

		pg = (unsigned char*)alloca(fileemPageSize);
		memset(pg,0xff,fileemPageSize);

		h = fileemDevice.handle;
		lseek(h,((blockNumber ) * dev->nChunksPerBlock) * fileemPageSize, SEEK_SET);
		for(i = 0; i < dev->nChunksPerBlock; i++)
		{
			write(h,pg,fileemPageSize);
		}

		return YAFFS_OK;
	}
}

int fileem_InitialiseNAND(yaffs_Device *dev)
{
	CheckInit();

	return YAFFS_OK;
}

int fileem_QueryNANDBlock(struct yaffs_DeviceStruct *dev, int blockNo, yaffs_BlockState *state, __u32 *sequenceNumber)
{
	yaffs_ExtendedTags tags;
	int chunkNo;

	*sequenceNumber = 0;

	chunkNo = blockNo * dev->nChunksPerBlock;

	fileem_ReadChunkWithTagsFromNAND(dev,chunkNo,NULL,&tags);
	if(tags.blockBad)
	{
		*state = YAFFS_BLOCK_STATE_DEAD;
	}
	else if(!tags.chunkUsed)
	{
		*state = YAFFS_BLOCK_STATE_EMPTY;
	}
	else if(tags.chunkUsed)
	{
		*state = YAFFS_BLOCK_STATE_NEEDS_SCANNING;
		*sequenceNumber = tags.sequenceNumber;
#ifdef YAFFS_ACP
		if( tags.sequenceNumber == YAFFS_SEQUENCE_ACP_DATA1 ||
			tags.sequenceNumber == YAFFS_SEQUENCE_ACP_DATA2)
			dev->acpCurrentQueryBlockPageSequence = tags.chunkId;
#endif
	}
	return YAFFS_OK;
}

