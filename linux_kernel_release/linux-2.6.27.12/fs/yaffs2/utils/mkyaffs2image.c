/*
 * YAFFS: Yet Another Flash File System. A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2007 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 * Nick Bane modifications flagged NCB
 * Endian handling patches by James Ng.
 * mkyaffs2image hacks by NCB
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/*
 * makeyaffs2image.c 
 *
 * Makes a YAFFS2 file system image that can be used to load up a file system.
 * Uses default Linux MTD layout - change if you need something different.
 */
 
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#define __user
#include <mtd/mtd-user.h>
#include "yaffs_ecc.h"
#include "yaffs_guts.h"

#include "yaffs_packedtags2.h"

void display_version(void);
void display_help(void);
void process_options (int argc, char *argv[]);
void printOptions (void);

unsigned yaffs_traceMask=0;

#define MAX_OBJECTS 200000

int dataSize = 2048;
int oobSize = 64;
int chunkSize;
int oobOffset = 2;
int chunksPerBlock = 128;

int inband = 0;
int quiet = 0;
int pad = 1;
int paddingSizeInMB = 0;

static char default_rootdir[] = ".";
char * rootDir = NULL;
char * imgPath = NULL;

unsigned char * oobBuf = NULL;
unsigned char * dataBuf = NULL;

int bFakeroot = 0;

const char * mkyaffs2image_c_version = "$Id: mkyaffs2image.c,v 1.4 2007/02/14 01:09:06 wookey Exp $";

#if 0
static int layout_no;

static struct nand_oobinfo oob_layout[] = {
 /* KSI:
 * Dummy "raw" layout - no ECC, all the bytes are free. Does NOT
 * really work, only used for compatibility with CVS YAFFS2 that
 * never ever worked with any stock MTD.
 */
 {
 .useecc = MTD_NANDECC_AUTOPLACE,
 .eccbytes = 0,
 .eccpos = {},
 .oobfree = { {0, 64} }
 },
 /* KSI:
 * Regular MTD AUTOPLACED ECC for large page NAND devices, the
 * only one existing in stock MTD so far. It corresponds to layout# 1
 * in command line arguments. Any other layouts could be added to
 * the list when they made their way in kernel's MTD. The structure
 * is simply copied from kernel's drivers/mtd/nand/nand_base.c as-is.
 */
 {
 .useecc = MTD_NANDECC_AUTOPLACE,
 .eccbytes = 24,
 .eccpos = {
 40, 41, 42, 43, 44, 45, 46, 47,
 48, 49, 50, 51, 52, 53, 54, 55,
 56, 57, 58, 59, 60, 61, 62, 63},
 .oobfree = { {2, 38} }
 },
 /* End-of-list marker */
 {
 .useecc = -1,
 }
};
#endif

typedef struct
{
	dev_t dev;
	ino_t ino;
	int   obj;
	int   isReg;
	int   reg_chunknum;
} objItem;


static objItem obj_list[MAX_OBJECTS];
static int n_obj = 0;
static int obj_id = YAFFS_NOBJECT_BUCKETS + 5;

static unsigned sequenceNum = YAFFS_LOWEST_SEQUENCE_NUMBER+1;
static long int nWrittenPages = 0;

static int nObjects = 0, nPages = 0, nTnodes = 0;
static int nDirectories = 0,  nRegs = 0, nDirs = 0, nLinks = 0, nSocks = 0, nFifos = 0, nBlks = 0, nChrs = 0;

static int outFileFD;

static int convert_endian = 0;

static int obj_compare(const void *a, const void * b)
{
  objItem *oa, *ob;
  
  oa = (objItem *)a;
  ob = (objItem *)b;
  
  if(oa->dev < ob->dev) return -1;
  if(oa->dev > ob->dev) return 1;
  if(oa->ino < ob->ino) return -1;
  if(oa->ino > ob->ino) return 1;
  
  return 0;
}


static void add_obj_to_list(dev_t dev, ino_t ino, int obj)
{
	if(n_obj < MAX_OBJECTS)
	{
		obj_list[n_obj].dev = dev;
		obj_list[n_obj].ino = ino;
		obj_list[n_obj].obj = obj;
		n_obj++;
		qsort(obj_list,n_obj,sizeof(objItem),obj_compare);
		
	}
	else
	{
		// oops! not enough space in the object array
		fprintf(stderr,"Not enough space in object array\n");
		exit(2);
	}
}


static int find_obj_in_list(dev_t dev, ino_t ino)
{
	objItem *i = NULL;
	objItem test;

	test.dev = dev;
	test.ino = ino;
	
	if(n_obj > 0)
	{
		i = bsearch(&test,obj_list,n_obj,sizeof(objItem),obj_compare);
	}

	if(i)
	{
		return i->obj;
	}
	return -1;
}


/* This little function converts a little endian tag to a big endian tag.
 * NOTE: The tag is not usable after this other than calculating the CRC
 * with.
 */
static void little_to_big_endian(yaffs_ExtendedTags *tagsPtr)
{
#if 0 // FIXME NCB
    yaffs_TagsUnion * tags = (yaffs_TagsUnion* )tagsPtr; // Work in bytes.
    yaffs_TagsUnion   temp;

    memset(&temp, 0, sizeof(temp));
    // Ick, I hate magic numbers.
    temp.asBytes[0] = ((tags->asBytes[2] & 0x0F) << 4) | ((tags->asBytes[1] & 0xF0) >> 4);
    temp.asBytes[1] = ((tags->asBytes[1] & 0x0F) << 4) | ((tags->asBytes[0] & 0xF0) >> 4);
    temp.asBytes[2] = ((tags->asBytes[0] & 0x0F) << 4) | ((tags->asBytes[2] & 0x30) >> 2) | ((tags->asBytes[3] & 0xC0) >> 6);
    temp.asBytes[3] = ((tags->asBytes[3] & 0x3F) << 2) | ((tags->asBytes[2] & 0xC0) >> 6);
    temp.asBytes[4] = ((tags->asBytes[6] & 0x03) << 6) | ((tags->asBytes[5] & 0xFC) >> 2);
    temp.asBytes[5] = ((tags->asBytes[5] & 0x03) << 6) | ((tags->asBytes[4] & 0xFC) >> 2);
    temp.asBytes[6] = ((tags->asBytes[4] & 0x03) << 6) | (tags->asBytes[7] & 0x3F);
    temp.asBytes[7] = (tags->asBytes[6] & 0xFC) | ((tags->asBytes[7] & 0xC0) >> 6);

    // Now copy it back.
    tags->asBytes[0] = temp.asBytes[0];
    tags->asBytes[1] = temp.asBytes[1];
    tags->asBytes[2] = temp.asBytes[2];
    tags->asBytes[3] = temp.asBytes[3];
    tags->asBytes[4] = temp.asBytes[4];
    tags->asBytes[5] = temp.asBytes[5];
    tags->asBytes[6] = temp.asBytes[6];
    tags->asBytes[7] = temp.asBytes[7];
#endif
}

#if 0
#define MAXTRY 100
int confirm_write(int fd, char * buffer, int len)
{
	int retry = 0;
	int nwritten = 0;
	int error;

	while(1) {
		error = write(outFileFD, data, dataSize);
		if(error<0){
			nwritten = error;
			break;
		}
		nwritten += error;
		if(nwritten<len){
			retry++;
			if(retry > MAXTRY){
				nwritten = -1; errno = EIO;
				break;
			}
		}else
			break;
	}

	return nwritten;
}
#endif
#if 0
#define PT2_BYTES (25)

/**
 * Calculate the ECC using the YAFFS internal functions,
 * and then put it into the oob buffer in the correct place
 */
static void yaffs_PutDataECC(const __u8 * data, unsigned char *oob_buf)
{
	unsigned char ecc_code[3];
	int eccsteps = chunkSize / 256;
	int eccidx = 0, datidx = 0, i;
	struct nand_oobinfo *oobsel;
	__u32 *oob_config;

	oobsel = &oob_layout[layout_no];
	oob_config = oobsel->eccpos;

	for (; eccsteps; eccsteps--) {
		yaffs_ECCCalculate (&data[datidx], ecc_code);
		for (i = 0; i < 3; i++, eccidx++)
			oob_buf[oob_config[eccidx]] = ecc_code[i];
		datidx += 256;
	}
}

static void nandmtd2_pt2buf(unsigned char *buf, yaffs_PackedTags2 *pt)
{
	int i, j = 0, k, n;
	unsigned char pt2_byte_buf[PT2_BYTES];

	*((unsigned int *) &pt2_byte_buf[0]) = pt->t.sequenceNumber;
	*((unsigned int *) &pt2_byte_buf[4]) = pt->t.objectId;
	*((unsigned int *) &pt2_byte_buf[8]) = pt->t.chunkId;
	*((unsigned int *) &pt2_byte_buf[12]) = pt->t.byteCount;
	pt2_byte_buf[16] = pt->ecc.colParity;
	pt2_byte_buf[17] = pt->ecc.lineParity & 0xff;
	pt2_byte_buf[18] = (pt->ecc.lineParity >> 8) & 0xff;
	pt2_byte_buf[19] = (pt->ecc.lineParity >> 16) & 0xff;
	pt2_byte_buf[20] = (pt->ecc.lineParity >> 24) & 0xff;
	pt2_byte_buf[21] = pt->ecc.lineParityPrime & 0xff;
	pt2_byte_buf[22] = (pt->ecc.lineParityPrime >> 8) & 0xff;
	pt2_byte_buf[23] = (pt->ecc.lineParityPrime >> 16) & 0xff;
	pt2_byte_buf[24] = (pt->ecc.lineParityPrime >> 24) & 0xff;

	k = oob_layout[layout_no].oobfree[j][0];
	n = oob_layout[layout_no].oobfree[j][1];

	if (n == 0) {
		fprintf(stderr, "No OOB space for tags");
		exit(-1);
	}

	for (i = 0; i < PT2_BYTES; i++) {
		if (n == 0) {
			j++;
			k = oob_layout[layout_no].oobfree[j][0];
			n = oob_layout[layout_no].oobfree[j][1];
			if (n == 0) {
				fprintf(stderr, "No OOB space for tags");
				exit(-1);
			}
		}
		buf[k++] = pt2_byte_buf[i];
		n--;
	}
}

static int write_chunk(__u8 *data, __u32 objId, __u32 chunkId, __u32 nBytes)
{
	yaffs_ExtendedTags t;
	yaffs_PackedTags2 pt;
	unsigned char spare_buf[spareSize];

	error = write(outFileFD,data,chunkSize);
	if(error < 0) return error;

	yaffs_InitialiseTags(&t);
	
	t.chunkId = chunkId;
//	t.serialNumber = 0;
	t.serialNumber = 1;	// **CHECK**
	t.byteCount = nBytes;
	t.objectId = objId;
	
	t.sequenceNumber = YAFFS_LOWEST_SEQUENCE_NUMBER;

// added NCB **CHECK**
	t.chunkUsed = 1;

	if (convert_endian)
	{
    	    little_to_big_endian(&t);
	}

	nPages++;

	yaffs_PackTags2(&pt,&t);
	
//	return write(outFileFD,&pt,sizeof(yaffs_PackedTags2));
//	return write(outFileFD,&pt,spareSize);

	memset(spare_buf, 0xff, sizeof(spare_buf));

	if (layout_no == 0) {
		memcpy(spare_buf, &pt, sizeof(yaffs_PackedTags2));
	} else {
		nandmtd2_pt2buf(spare_buf, &pt);
	}

	yaffs_PutDataECC(data, &spare_buf[0]);

	return write(outFileFD,spare_buf,spareSize);

}
#else
static int write_chunk_bbb(__u8 *data, yaffs_ExtendedTags * t)
{
	yaffs_PackedTags2 pt;
	int error;
	int nDataBytes;
	int nMetaDataBytes;

	if(inband) {
		nDataBytes = dataSize - sizeof(yaffs_PackedTags2TagsPart);
		nMetaDataBytes = sizeof(yaffs_PackedTags2TagsPart);
	} else {
		nDataBytes = dataSize;
		nMetaDataBytes = oobSize;
	}

	error = write(outFileFD, data, nDataBytes);

	if(error != nDataBytes) {
		fprintf(stderr, "Only %d out of %d bytes are written\n", error, nDataBytes);
		return -1;
	}

	if (convert_endian)	{
		little_to_big_endian(t);
	}

	nPages++;

	if(inband){
		yaffs_PackTags2TagsPart(&(pt.t), t);
		error = write(outFileFD, &(pt.t), nMetaDataBytes);

	} else {
		yaffs_PackTags2(&pt, t);

		memset(oobBuf, 0xff, oobSize);
		memcpy(oobBuf+2, &pt, sizeof(yaffs_PackedTags2));

		error = write(outFileFD, oobBuf, oobSize);
	}

	if(error == nMetaDataBytes){
		nWrittenPages++;
		if( (nWrittenPages%chunksPerBlock) == 0)
			sequenceNum++;
	} else {
		fprintf(stderr, "Only %d out of %d meta bytes are written\n", error, nMetaDataBytes);
		return -1;
	}

	return error;
}
#endif

#define SWAP32(x)   ((((x) & 0x000000FF) << 24) | \
                     (((x) & 0x0000FF00) << 8 ) | \
                     (((x) & 0x00FF0000) >> 8 ) | \
                     (((x) & 0xFF000000) >> 24))

#define SWAP16(x)   ((((x) & 0x00FF) << 8) | \
                     (((x) & 0xFF00) >> 8))
        
// This one is easier, since the types are more standard. No funky shifts here.
static void object_header_little_to_big_endian(yaffs_ObjectHeader* oh)
{
    oh->type = SWAP32(oh->type); // GCC makes enums 32 bits.
    oh->parentObjectId = SWAP32(oh->parentObjectId); // int
    oh->sum__NoLongerUsed = SWAP16(oh->sum__NoLongerUsed); // __u16 - Not used, but done for completeness.
    // name = skip. Char array. Not swapped.
    oh->yst_mode = SWAP32(oh->yst_mode);
#ifdef CONFIG_YAFFS_WINCE // WinCE doesn't implement this, but we need to just in case. 
    // In fact, WinCE would be *THE* place where this would be an issue!
    oh->notForWinCE[0] = SWAP32(oh->notForWinCE[0]);
    oh->notForWinCE[1] = SWAP32(oh->notForWinCE[1]);
    oh->notForWinCE[2] = SWAP32(oh->notForWinCE[2]);
    oh->notForWinCE[3] = SWAP32(oh->notForWinCE[3]);
    oh->notForWinCE[4] = SWAP32(oh->notForWinCE[4]);
#else
    // Regular POSIX.
    oh->yst_uid = SWAP32(oh->yst_uid);
    oh->yst_gid = SWAP32(oh->yst_gid);
    oh->yst_atime = SWAP32(oh->yst_atime);
    oh->yst_mtime = SWAP32(oh->yst_mtime);
    oh->yst_ctime = SWAP32(oh->yst_ctime);
#endif

    oh->fileSize = SWAP32(oh->fileSize); // Aiee. An int... signed, at that!
    oh->equivalentObjectId = SWAP32(oh->equivalentObjectId);
    // alias  - char array.
    oh->yst_rdev = SWAP32(oh->yst_rdev);

#ifdef CONFIG_YAFFS_WINCE
    oh->win_ctime[0] = SWAP32(oh->win_ctime[0]);
    oh->win_ctime[1] = SWAP32(oh->win_ctime[1]);
    oh->win_atime[0] = SWAP32(oh->win_atime[0]);
    oh->win_atime[1] = SWAP32(oh->win_atime[1]);
    oh->win_mtime[0] = SWAP32(oh->win_mtime[0]);
    oh->win_mtime[1] = SWAP32(oh->win_mtime[1]);
    oh->roomToGrow[0] = SWAP32(oh->roomToGrow[0]);
    oh->roomToGrow[1] = SWAP32(oh->roomToGrow[1]);
    oh->roomToGrow[2] = SWAP32(oh->roomToGrow[2]);
    oh->roomToGrow[3] = SWAP32(oh->roomToGrow[3]);
    oh->roomToGrow[4] = SWAP32(oh->roomToGrow[4]);
    oh->roomToGrow[5] = SWAP32(oh->roomToGrow[5]);
#else
    oh->roomToGrow[0] = SWAP32(oh->roomToGrow[0]);
    oh->roomToGrow[1] = SWAP32(oh->roomToGrow[1]);
    oh->roomToGrow[2] = SWAP32(oh->roomToGrow[2]);
    oh->roomToGrow[3] = SWAP32(oh->roomToGrow[3]);
    oh->roomToGrow[4] = SWAP32(oh->roomToGrow[4]);
    oh->roomToGrow[5] = SWAP32(oh->roomToGrow[5]);
    oh->roomToGrow[6] = SWAP32(oh->roomToGrow[6]);
    oh->roomToGrow[7] = SWAP32(oh->roomToGrow[7]);
    oh->roomToGrow[8] = SWAP32(oh->roomToGrow[8]);
    oh->roomToGrow[9] = SWAP32(oh->roomToGrow[9]);
    oh->roomToGrow[10] = SWAP32(oh->roomToGrow[10]);
    oh->roomToGrow[11] = SWAP32(oh->roomToGrow[11]);
#endif
}

static int write_object_header(int objId, yaffs_ObjectType t, struct stat *s,
	int parent, const char *name, int equivalentObj, const char * alias)
{
	__u8 * bytes = dataBuf;
	yaffs_ExtendedTags tag;

	yaffs_ObjectHeader *oh = (yaffs_ObjectHeader *)bytes;
	
	memset(bytes,0xff,dataSize);
	
	oh->type = t;

	oh->parentObjectId = parent;
	
	strncpy(oh->name,name,YAFFS_MAX_NAME_LENGTH);

	if(t != YAFFS_OBJECT_TYPE_HARDLINK)
	{
		oh->yst_mode = s->st_mode;
		if (bFakeroot) {
			oh->yst_uid = 0;
			oh->yst_gid = 0;
		} else {
			oh->yst_uid = s->st_uid;
	// NCB 12/9/02		oh->yst_gid = s->yst_uid;
			oh->yst_gid = s->st_gid;
		}
		oh->yst_atime = s->st_atime;
		oh->yst_mtime = s->st_mtime;
		oh->yst_ctime = s->st_ctime;
		oh->yst_rdev  = s->st_rdev;
	}
	
	if(t == YAFFS_OBJECT_TYPE_FILE)
	{
		oh->fileSize = s->st_size;
	}
	
	if(t == YAFFS_OBJECT_TYPE_HARDLINK)
	{
		oh->equivalentObjectId = equivalentObj;
	}
	
	if(t == YAFFS_OBJECT_TYPE_SYMLINK)
	{
		strncpy(oh->alias,alias,YAFFS_MAX_ALIAS_LENGTH);
	}

	if (convert_endian)
	{
    		object_header_little_to_big_endian(oh);
	}

	yaffs_InitialiseTags(&tag);
	
	tag.chunkId = 0;
	tag.objectId = objId;
	tag.serialNumber = 1; // **CHECK**
	tag.byteCount = 0xffff;
	tag.sequenceNumber = sequenceNum;
	tag.chunkUsed = 1;

	tag.extraHeaderInfoAvailable = 1;
	tag.extraParentObjectId = oh->parentObjectId;
	tag.extraIsShrinkHeader = 0;
	tag.extraShadows = 0;
	tag.extraObjectType = t;
	tag.extraFileLength = oh->fileSize;
	tag.extraEquivalentObjectId = oh->equivalentObjectId;

	oh->shadowsObject = oh->inbandShadowsObject = tag.extraShadows;
	oh->isShrink = oh->inbandIsShrink = tag.extraIsShrinkHeader;

//	return write_chunk(bytes,objId,0,0xffff);
	return write_chunk_bbb(bytes, &tag); 
}


static int process_directory(int parent, const char *path)
{
	DIR *dir;
	struct dirent *entry;
	int error = 0;
	int nDataBytes;

	if(inband)
		nDataBytes = dataSize - sizeof(yaffs_PackedTags2TagsPart);
	else
		nDataBytes = dataSize;

	nDirectories++;

	dir = opendir(path);

	if(dir)
	{
		while((entry = readdir(dir)) != NULL && error>=0)
		{
		
			/* Ignore . and .. */
			if(strcmp(entry->d_name,".") &&
			   strcmp(entry->d_name,".."))
 			{
 				char full_name[500];
				struct stat stats;
				int equivalentObj;
				int newObj;

				sprintf(full_name,"%s/%s",path,entry->d_name);

				error = lstat(full_name,&stats);
				if(error<0)
					break;

				if(S_ISLNK(stats.st_mode) ||
				    S_ISREG(stats.st_mode) ||
				    S_ISDIR(stats.st_mode) ||
				    S_ISFIFO(stats.st_mode) ||
				    S_ISBLK(stats.st_mode) ||
				    S_ISCHR(stats.st_mode) ||
				    S_ISSOCK(stats.st_mode))
				{

					newObj = obj_id++;
					nObjects++;

					if(!quiet)
						printf("Object %d, %s is a ",newObj,full_name);

					/* We're going to create an object for it */
					if((equivalentObj = find_obj_in_list(stats.st_dev, stats.st_ino)) > 0)
					{
					 	/* we need to make a hard link */
						if(!quiet)
							printf("hard link to object %d\n",equivalentObj);
						error =  write_object_header(newObj, YAFFS_OBJECT_TYPE_HARDLINK,
							&stats, parent, entry->d_name, equivalentObj, NULL);
					}
					else 
					{

						add_obj_to_list(stats.st_dev,stats.st_ino,newObj);

						if(S_ISLNK(stats.st_mode))
						{

							char symname[500];

							memset(symname,0, sizeof(symname));

							error = readlink(full_name,symname,sizeof(symname) -1);

							if(!quiet)
								printf("symlink to \"%s\"\n",symname);
							error =  write_object_header(newObj, YAFFS_OBJECT_TYPE_SYMLINK,
								&stats, parent, entry->d_name, -1, symname);
							if(error >= 0)
								nLinks++;
						}
						else if(S_ISREG(stats.st_mode))
						{
							if(!quiet)
								printf("file, ");
							error =  write_object_header(newObj, YAFFS_OBJECT_TYPE_FILE,
								&stats, parent, entry->d_name, -1, NULL);
							obj_list[n_obj-1].isReg = 1;

							if(error >= 0)
							{
								int h;
								__u8 * bytes = dataBuf;
								int nBytes;
								int chunk = 0;
								int bytesRead = 0;

								h = open(full_name,O_RDONLY);
								if(h >= 0)
								{

									memset(bytes, 0xff, dataSize);
									while((nBytes = read(h, bytes, nDataBytes)) > 0 && (error>=0))
									{
										bytesRead += nBytes;
										if(bytesRead != stats.st_size && nBytes!=nDataBytes) {
											fprintf(stderr, "Could not fill a full buffer of data chunk\n");
											return -1;
										}
										chunk++;
										obj_list[n_obj-1].reg_chunknum++;
#if 0
										write_chunk(bytes,newObj,chunk,nBytes);
#else
										{
											yaffs_ExtendedTags tag;
											tag.chunkUsed = 1;
											tag.chunkId = chunk;
											tag.objectId = newObj;
											tag.serialNumber = 1;
											tag.byteCount = nBytes;
											tag.sequenceNumber = sequenceNum;
											error = write_chunk_bbb(bytes, &tag);
										}
#endif
										memset(bytes,0xff,dataSize);
									}
									if(nBytes < 0) {
									   return nBytes;
									}

									if(!quiet)
										printf("%d data chunks written\n", chunk);
									nRegs++;
								}
								else
								{
									perror("Error opening file");
									error = -1;
								}
								close(h);

							}							
														
						}
						else if(S_ISSOCK(stats.st_mode))
						{
							if(!quiet)
								printf("socket\n");
							error =  write_object_header(newObj, YAFFS_OBJECT_TYPE_SPECIAL,
								&stats, parent, entry->d_name, -1, NULL);
							if(error>=0)
								nSocks++;
						}
						else if(S_ISFIFO(stats.st_mode))
						{
							if(!quiet)
								printf("fifo\n");
							error =  write_object_header(newObj, YAFFS_OBJECT_TYPE_SPECIAL,
								&stats, parent, entry->d_name, -1, NULL);
							if(error>=0)
								nFifos++;
						}
						else if(S_ISCHR(stats.st_mode))
						{
							if(!quiet)
								printf("character device\n");
							error =  write_object_header(newObj, YAFFS_OBJECT_TYPE_SPECIAL,
								&stats, parent, entry->d_name, -1, NULL);
							if(error>=0)
								nChrs++;
						}
						else if(S_ISBLK(stats.st_mode))
						{
							if(!quiet)
								printf("block device\n");
							error =  write_object_header(newObj, YAFFS_OBJECT_TYPE_SPECIAL,
								&stats, parent, entry->d_name, -1, NULL);
							if(error>=0)
								nBlks++;
						}
						else if(S_ISDIR(stats.st_mode))
						{
							if(!quiet)
								printf("directory\n");
							error =  write_object_header(newObj, YAFFS_OBJECT_TYPE_DIRECTORY,
								&stats, parent, entry->d_name, -1, NULL);
// NCB modified 10/9/2001				process_directory(1,full_name);
							if(error>=0)
								nDirs++;
							error = process_directory(newObj, full_name);
							if(error<0)
								return error;
						}
					}
				}
				else
				{
					fprintf(stderr, " we don't handle this type\n");
				}
			}
		}

		closedir(dir);
	} else
		error = -1;
	
	return error;

}

static int mkYaffs2Img_CalcCheckpointBlocksRequired(void)
{
	int endBlock = 80*4-1;/*80M partition*/
	int startBlock = 0;
	int tnodeWidth = 32;
	int nDataBytesPerChunk = 2048;

	int nChunksPerBlock = 128;
	int chunkBitmapStride = (nChunksPerBlock + 7) / 8; /* round up bytes */
	int i;

	/* Not a valid value so recalculate */
	int nBytes = 0;
	int nBlocks;
	int devBlocks = (endBlock - startBlock + 1);
	int tnodeSize;

	tnodeSize = (tnodeWidth * YAFFS_NTNODES_LEVEL0)/8;

	if(tnodeSize < sizeof(yaffs_Tnode))
		tnodeSize = sizeof(yaffs_Tnode);

	/** calculate the tnodes*/
	for(i=0; i<n_obj; i++){
		if(obj_list[i].isReg)
			nTnodes += ((obj_list[i].reg_chunknum/16+1)*8-1)/7;
	}

	nBytes += sizeof(yaffs_CheckpointValidity);
	nBytes += sizeof(yaffs_CheckpointDevice);
	nBytes += devBlocks * sizeof(yaffs_BlockInfo);
	nBytes += devBlocks * chunkBitmapStride;
	nBytes += (sizeof(yaffs_CheckpointObject) + sizeof(__u32)) * nObjects;
	nBytes += (tnodeSize + sizeof(__u32)) * nTnodes;
	nBytes += sizeof(yaffs_CheckpointValidity);
	nBytes += sizeof(__u32); /* checksum*/

	/* Round up and add 2 blocks to allow for some bad blocks, so add 3 */

	nBlocks = (nBytes/(nDataBytesPerChunk * nChunksPerBlock)) + 3;

	return nBlocks;
}

void display_version()
{
	printf("%s\n", mkyaffs2image_c_version);
	exit(0);
}

void display_help()
{
printf(
"Usage: mkyaffs2img [OPTION] OUTPUTFILE\n"
"Generate an image file of YAFFS version 2.\n"
"\n"
"  -s SIZEinByte         NAND flash page size (in byte, default: 2048)\n"
"  -e NUMBER             There are NUMBER of pages per erase block (default: 128)\n"
"  -o SIZEinByte         OOB size per page (default: 64)\n"
"  --ooboffset OFFSET    YAFFS2 tag is put to OFFSET of oob area (default: 2)\n"
"                   !!!  Should use this carefully!!\n\n"
"  -i                    Put tags inside data area instead of oob area\n"

"  -p SIZEinMByte        Pad output to SIZEinMByte with 0xFF.\n"
"                   !!!  Should NOT use this if the image will be burned with bootflow  !!!\n"
"                        (default: no padding.)\n\n"
"  -r DIR                Build file system from directory DIR (default: cwd)\n"
"  --fakeroot            Make root as each file's owner and group\n"

"  -q                    don't display progress messages\n"
"  -h                    display help information and exit\n"
"  -v                    output version information and exit\n");
	exit(0);
}

void process_options (int argc, char *argv[])
{
	int error = 0;
	static const struct option long_options[] = {
		{"ooboffset", required_argument, 0, 1000},
		{"fakeroot", no_argument, 0, 2000},
		{0, 0, 0, 0},
	};

	for (;;) {
		int option_index = 0;

		int c = getopt_long(argc, argv, "is:e:o:p:r:qhv", long_options, &option_index);
		if (c == -1) {
			break;
		}

		switch (c) {
			case 'i':
				inband = 1;
				break;
			case 1000:
				oobOffset = strtol(optarg, NULL, 0);
				break;
			case 2000:
				bFakeroot = 1;
				break;
			case 's':
				dataSize = strtol(optarg, NULL, 0);
				break;
			case 'e':
				chunksPerBlock = strtol(optarg, NULL, 0);
				break;
			case 'o':
				oobSize = strtol(optarg, NULL, 0);
				break;
			case 'p':
				paddingSizeInMB = strtol(optarg, NULL, 0);
				fprintf(stdout, "\n!!!  Don't use -p if the image will be burned with bootflow  !!!\n");
				break;
			case 'r':
				rootDir = optarg;
				break;
			case 'q':
				quiet = 1;
				break;
			case 'h':
				display_help();
				break;
			case 'v':
				display_version();
				break;
			case '?':
				error = 1;
				break;
		}
	}

	if ((argc - optind) != 1 || error)
		display_help ();

	imgPath = argv[optind];
}

void printOptions()
{
	if (!quiet) {
		printf("======== mkyaffs2image: image building tool for YAFFS2 built "__DATE__" ========\n");
		printf("sizeof(object header) = %d\n",   sizeof(yaffs_ObjectHeader));
		printf("sizeof(packedTags2)   = %d\n\n", sizeof(yaffs_PackedTags2));
		printf("\tInput directory   = %s\n", rootDir);
		printf("\tOutput image file = %s\n", imgPath);
	}
}

#if 1
int main(int argc, char *argv[])
{
	struct stat stats;
	int error=0;
	int blocksPerMB;

	process_options(argc, argv);

/* check options*/
	if(oobOffset >= oobSize) {
		fprintf(stderr, "OOB offset is not compatible with oob area size\n");
		exit(-1);
	}
	if(sizeof(yaffs_ObjectHeader)>dataSize) {
		fprintf(stderr, "OOB size is not compatible with header chunk size\n");
		exit(-1);
	}

	if(!rootDir)
		rootDir = default_rootdir;
	if( stat(rootDir, &stats) < 0 ) {
		fprintf(stderr, "Could not stat %s\n", rootDir);
		exit(-1);
	}
	if( !S_ISDIR(stats.st_mode) ){
		fprintf(stderr, " %s is not a directory\n", rootDir);
		exit(-1);
	}

	outFileFD = open(imgPath, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if(outFileFD < 0) {
		fprintf(stderr, "Could not open output file %s\n", imgPath);
		exit(-1);
	}

	dataBuf = malloc(dataSize);
	if(!dataBuf){
		fprintf(stderr, "Could not alloc memory for data area\n");
		exit(-1);
	}

	oobBuf = malloc(oobSize);
	if(!oobBuf){
		fprintf(stderr, "Could not alloc memory for oob area\n");
		exit(-1);
	}

	chunkSize = dataSize + oobSize;
	blocksPerMB = 1024*1024/(dataSize*chunksPerBlock);

	printOptions();

/* begin to write image */
	memset(obj_list, 0, MAX_OBJECTS*sizeof(objItem));
	error =  write_object_header(1, YAFFS_OBJECT_TYPE_DIRECTORY, &stats, 1, "", -1, NULL);
	if(error)
		error = process_directory(YAFFS_OBJECTID_ROOT, rootDir);

	if(pad){
		int nPaddingPages, i;
		if(paddingSizeInMB){
			nPaddingPages = paddingSizeInMB*blocksPerMB*chunksPerBlock - nWrittenPages;
		}else{
//			if(nWrittenPages%chunksPerBlock)
//				nPaddingPages = chunksPerBlock - nWrittenPages%chunksPerBlock;
//			else
				nPaddingPages = 0;
		}
		if(nPaddingPages>0) {
			if (!quiet) {
				printf("=== padding ===\n");
				printf("padding %d pages in the end\n", nPaddingPages);
			}
			memset(oobBuf, 0xff, oobSize);
			memset(dataBuf, 0xff, dataSize);
			for(i=0; i<nPaddingPages; i++){
				error = write(outFileFD, dataBuf, dataSize);
				if(error<0)
					break;
				error = write(outFileFD, oobBuf, oobSize);
				if(error<0)
					break;
				nWrittenPages++;
			}
		}
	}

	close(outFileFD);
	free(oobBuf);
	free(dataBuf);

	if( error >= 0){
		struct stat img_state;
		int size;

		if(inband)
			size = nWrittenPages*dataSize;
		else
			size = nWrittenPages*(dataSize+oobSize);

		error = stat(imgPath, &img_state);
		if(!error){
			if(img_state.st_size != size){
				error = -1;
				fprintf(stderr, "image size %lu mismatch, should be %d\n", img_state.st_size, size);
			}
		}
	}

	if (error < 0) {
		perror("operation incomplete");
		exit(1);
	} else {
		if (!quiet) {
			printf("======== Operation complete ========\n");
			printf("flash geometry per block is [( %d + %d ) * %d]\n", dataSize, oobSize, chunksPerBlock);
			printf("%d objects in %d directories\n %d NAND pages\n",nObjects, nDirectories, nPages);
			printf("regs=%d, dirs=%d, links=%d, socks=%d, fifos=%d, blocks=%d, chars=%d\n",
   	         nRegs, nDirs, nLinks, nSocks, nFifos, nBlks, nChrs);
			printf("%d blocks for checkpoint area\n", mkYaffs2Img_CalcCheckpointBlocksRequired());
		}
	}

	exit(0);
}
#else

static void usage(void)
{
//	printf("usage: mkyaffs2image layout# dir image_file [convert]\n");
	printf("usage: mkyaffs2image layout# dir image_file [paddingSizeInMB]\n");
	printf(" layout# NAND OOB layout # (0 - raw, 1 - nand_oob_64)\n");
	printf(" dir the directory tree to be converted\n");
	printf(" image_file the output file to hold the image\n");
//	printf(" 'convert' make a big-endian img on a little-endian machine. BROKEN !\n");
	printf(" 'paddingSizeInMB' pad image_file to the size\n");
	exit(1);
}

int main(int argc, char *argv[])
{
	struct stat stats;
	int i;
	int padding_size=-1;
	
	printf("mkyaffs2image: image building tool for YAFFS2 built "__DATE__"\n");
	printf("data size in object header chunk:%d\n", sizeof(yaffs_ObjectHeader));
	printf("size of packedTags2:%d\n", sizeof(yaffs_PackedTags2));
//	if(argc < 3)
	if ((argc < 4) || (sscanf(argv[1], "%u", &layout_no) != 1))
	{
//		printf("usage: mkyaffs2image dir image_file [convert]\n");
//		printf("           dir        the directory tree to be converted\n");
//		printf("           image_file the output file to hold the image\n");
//      printf("           'convert'  produce a big-endian image from a little-endian machine\n");
//		exit(1);
		usage();
	}

	i = 0;

	while (oob_layout[i].useecc != -1)
		i++;

	if (layout_no >= i)
		usage();
#if 0
	if ((argc == 5) && (!strncmp(argv[4], "convert", strlen("convert"))))
	{
		/* KSI: Broken as of now. TBD. Fail. */
		usage();
		convert_endian = 1;
	}
#endif
	if(stat(argv[2],&stats) < 0)
	{
		printf("Could not stat %s\n",argv[2]);
		exit(1);
	}

	if(!S_ISDIR(stats.st_mode))
	{
		printf(" %s is not a directory\n",argv[2]);
		exit(1);
	}
	
	outFileFD = open(argv[3],O_CREAT | O_TRUNC | O_WRONLY, S_IREAD | S_IWRITE);
	
	
	if(outFileFD < 0)
	{
		printf("Could not open output file %s\n",argv[3]);
		exit(1);
	}

	if (argc==5){
		unsigned char * dummyBuffer;
		int i;
		int paddingSizeInMB;
		paddingSizeInMB = strtol(argv[4], 0, 10);
		//padding_size = paddingSizeInMB*1024*1024;

		if(paddingSizeInMB%blocksPerMB)
			printf("padding size is wrong!\n");
		//i=paddingSizeInMB/blocksPerMB;
		//printf("%d %d %d\n",paddingSizeInMB, blocksPerMB, i);
		//printf("padding %s to %d MBytes(actually %d bytes)\n", argv[3], padding_size,
		//	paddingSizeInMB/blocksPerMB*blockTotalSize);

		dummyBuffer = (unsigned char *)malloc(blockTotalSize);
		if(!dummyBuffer){
			printf("could not allocate dummy buffer for padding\n");
			exit(1);
		}
		memset(dummyBuffer, 0xff, blockTotalSize);
		for(i=0;i<paddingSizeInMB*blocksPerMB;i++){
			if(write(outFileFD, dummyBuffer, blockTotalSize)!= blockTotalSize){
				free(dummyBuffer);
				exit(1);
			}
		}
		free(dummyBuffer);
		lseek(outFileFD, 0, SEEK_SET);
	}
	
	printf("Processing directory %s into image file %s\n",argv[2],argv[3]);
	memset(obj_list, 0, MAX_OBJECTS*sizeof(objItem));
	error =  write_object_header(1, YAFFS_OBJECT_TYPE_DIRECTORY, &stats, 1,"", -1, NULL);
	if(error)
		error = process_directory(YAFFS_OBJECTID_ROOT,argv[2]);
	
	close(outFileFD);
	
	if(error < 0)
	{
		perror("operation incomplete");
		exit(1);
	}
	else
	{
		printf("======== Operation complete ========\n"
		       "%d objects in %d directories\n"
		       "%d NAND pages\n",nObjects, nDirectories, nPages);
		printf("regs=%d, dirs=%d, links=%d, socks=%d, fifos=%d, blocks=%d, chars=%d\n",
			nRegs, nDirs, nLinks, nSocks, nFifos, nBlks, nChrs);
		printf("%d blocks for checkpoint area\n", mkYaffs2Img_CalcCheckpointBlocksRequired());
	}
	
	close(outFileFD);
	
	exit(0);
}	
#endif

