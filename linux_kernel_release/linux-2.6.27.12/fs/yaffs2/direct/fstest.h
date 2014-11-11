#ifndef FSTEST_H
#define FSTEST_H

#include "yaffs_guts.h"

#define PROMPT "fstest>"
#define MAX_LINE 512

int fileem_WriteChunkWithTagsToNAND(yaffs_Device *dev,int chunkInNAND,const __u8 *data, const yaffs_ExtendedTags *tags);
int fileem_GetNumberOfBlocks(void);
int fileem_ReadChunkWithTagsFromNAND(yaffs_Device *dev,int chunkInNAND, __u8 *data, yaffs_ExtendedTags *tags);
int fileem_MarkNANDBlockBad(struct yaffs_DeviceStruct *dev, int blockNo);
int fileem_EraseBlockInNAND(yaffs_Device *dev, int blockNumber);
int fileem_InitialiseNAND(yaffs_Device *dev);
int fileem_QueryNANDBlock(struct yaffs_DeviceStruct *dev, int blockNo, yaffs_BlockState *state, __u32 *sequenceNumber);
void FileEmulatorInit(char * diskpath, int nPagesPerBlock, int nPageDataSize, int nPageOOBSize,
	int forceBlockAlign, int imgContainOOB);

void fstest_InitDeviceCfg(void);
int fstest_yaffs_StartUp(char * configFilePath, char * diskFilePath);
int fstest_yaffs_Shutdown(void);

int fstest_parse_line(char * buffer);

extern int fileemPageDataSize;
extern int fileemPageOOBSize;
extern int fileemPageSize;
extern int fileemPagesPerBlock;

extern char rootPrefix[];

#endif

