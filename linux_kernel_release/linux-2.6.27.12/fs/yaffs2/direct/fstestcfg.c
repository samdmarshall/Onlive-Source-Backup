#include "yaffs_guts.h"
#include "fstest.h"
#include "yaffsfs.h"
#include <errno.h>

unsigned yaffs_traceMask = 

//	YAFFS_TRACE_SCAN |
//	YAFFS_TRACE_GC | YAFFS_TRACE_GC_DETAIL | 
//	YAFFS_TRACE_ERASE | 
//	YAFFS_TRACE_TRACING | 
//	YAFFS_TRACE_ALLOCATE | 
	YAFFS_TRACE_CHECKPOINT |
	YAFFS_TRACE_BAD_BLOCKS |
//	YAFFS_TRACE_SCAN_DEBUG |
	YAFFS_TRACE_ALWAYS |
	YAFFS_TRACE_ACP_INFO |
	YAFFS_TRACE_ACP_ERROR |
//	YAFFS_TRACE_VERIFY_ALL |
//	YAFFS_TRACE_ACP_DEBUG |
	0;


//static int yaffsfs_lastError;

void yaffsfs_SetError(int err)
{
	//Do whatever to set error
//	yaffsfs_lastError = err;
	errno = err;
}


int yaffsfs_GetLastError(void)
{
//	return yaffsfs_lastError;
	return errno;
}

void yaffsfs_Lock(void)
{
}

void yaffsfs_Unlock(void)
{
}

__u32 yaffsfs_CurrentTime(void)
{
	return 0;
}


static int yaffs_kill_alloc = 0;
static size_t total_malloced = 0;
static size_t malloc_limit = 0 & 6000000;

void *yaffs_malloc(size_t size)
{
	void * this;
	if(yaffs_kill_alloc)
		return NULL;
	if(malloc_limit && malloc_limit <(total_malloced + size) )
		return NULL;

	this = malloc(size);
	if(this)
		total_malloced += size;
	return this;
}

void yaffs_free(void *ptr)
{
	free(ptr);
}

void yaffsfs_LocalInitialisation(void)
{
	// Define locking semaphore.
}

// Configuration for:
// /ram  2MB ramdisk
// /boot 2MB boot disk (flash)
// /flash 14MB flash disk (flash)
// NB Though /boot and /flash occupy the same physical device they
// are still disticnt "yaffs_Devices. You may think of these as "partitions"
// using non-overlapping areas in the same device.
// 

static yaffs_Device fstestDev;

char rootPrefix[] = "/flash/bigblock";

static yaffsfs_DeviceConfiguration fstest_yaffsfs_config[] = {
	{ rootPrefix, &fstestDev},
	{(void *)0,(void *)0} /* Null entry to terminate list */
};


static int nPagesPerBlock;
static int nPageDataSize;
static int nPageOOBSize;

static int bForceImgBlockAlign;
static int bImgContainOOB;

static int nShortOpCaches;
static int acpDisabled;
static int skipCPRead;
static int skipCPWrite;
static int bUseInbandTags;

static char defaultDiskPath[] = "./em2k.img";
static char * fileDiskPath = NULL;

void fstest_config_nPagesPerblock(char * cmdString)
{
	if(cmdString)
		sscanf(cmdString, "%d", &nPagesPerBlock);
}

void fstest_config_nPageDataSize(char * cmdString)
{
	if(cmdString)
		sscanf(cmdString, "%d", &nPageDataSize);
}

void fstest_config_nPageOOBSize(char * cmdString)
{
	if(cmdString)
		sscanf(cmdString, "%d", &nPageOOBSize);
}

void fstest_config_nShortOpCaches(char * cmdString)
{
	if(cmdString)
		sscanf(cmdString, "%d", &nShortOpCaches);
}

void fstest_config_acpDisabled(char * cmdString)
{
	if(cmdString)
		sscanf(cmdString, "%d", &acpDisabled);
}

void fstest_config_skipCPRead(char * cmdString)
{
	if(cmdString)
		sscanf(cmdString, "%d", &skipCPRead);
}

void fstest_config_skipCPWrite(char * cmdString)
{
	if(cmdString)
		sscanf(cmdString, "%d", &skipCPWrite);
}

void fstest_config_errACPPageSeq(char * cmdString)
{
	if(cmdString)
		sscanf(cmdString, "%d", &errACPPageSeq);
}

void fstest_config_bUseInbandTags(char * cmdString)
{
	if(cmdString)
		sscanf(cmdString, "%d", &bUseInbandTags);
}

void fstest_config_bForceImgBlockAlign(char * cmdString)
{
	if(cmdString)
		sscanf(cmdString, "%d", &bForceImgBlockAlign);
}

void fstest_config_bImgContainOOB(char * cmdString)
{
	if(cmdString)
		sscanf(cmdString, "%d", &bImgContainOOB);
}

typedef struct {
	char *name; 	 /* name of command */
	void (*func)(); /* pointer to callback function */
	char *help; 	 /* descriptive help string */
} fstestConfigList;

static fstestConfigList cfgList[] = 
{
	{"nPagesPerBlock", fstest_config_nPagesPerblock, NULL},
	{"nPageDataSize", fstest_config_nPageDataSize, NULL},
	{"nPageOOBSize", fstest_config_nPageOOBSize, NULL},
	{"bForceImgBlockAlign", fstest_config_bForceImgBlockAlign, NULL},
	{"bImgContainOOB", fstest_config_bImgContainOOB, NULL},

	{"nShortOpCaches", fstest_config_nShortOpCaches, NULL},
	{"acpDisabled", fstest_config_acpDisabled, NULL},
	{"skipCPRead", fstest_config_skipCPRead, NULL},
	{"skipCPWrite", fstest_config_skipCPWrite, NULL},
	{"bUseInbandTags", fstest_config_bUseInbandTags, NULL},

	{"errACPPageSeq", fstest_config_errACPPageSeq, NULL},
	{NULL, NULL, NULL}
};

void fstest_LoadDefaultConfig(void)
{
	if(!fileDiskPath)
		fileDiskPath = defaultDiskPath;

	nPagesPerBlock = 128;
	nPageDataSize = 2048;
	nPageOOBSize = 64;

	nShortOpCaches = 10;
	acpDisabled = 0;
	skipCPRead = skipCPWrite = 1;
	bUseInbandTags = 0;

	bForceImgBlockAlign = 1;
	bImgContainOOB = 1;
}

void fstest_readConfigFile(char * configFilePath)
{
	char buffer[MAX_LINE+1];
	FILE *fp;
	int len; /* length of parsed command */
	int i;

	if( (fp=fopen(configFilePath,"r")) ) /* open config file */
	{
		printf("Reading configuration from file '%s'\n",configFilePath);
		while (fgets(buffer,MAX_LINE,fp)) /* read lines until EOF */
		{
			if(buffer[0] == '#')
				continue;
			for(i=0; cfgList[i].name; i++) {
				if (!strncmp(cfgList[i].name, buffer, len=strlen(cfgList[i].name))) {
					/* if command matches... */
					(cfgList[i].func)(((int)strlen(buffer)>len) ? buffer+len+1 : NULL);
					break; /* call function and pass remainder of line as parameter */
				}
			}
		}

		fclose(fp);
	} else {
		fprintf(stderr,"Error: cannot read configuration file '%s',"
			"using default configuration\n", configFilePath);
	}
}
void fstest_InitDeviceCfg(void)
{
	yaffs_Device * yDevice = &fstestDev;

	memset(yDevice,0,sizeof(yaffs_Device));

	yDevice->totalBytesPerChunk = nPageDataSize;
	yDevice->nChunksPerBlock = nPagesPerBlock;
	yDevice->nReservedBlocks = 5;
	yDevice->startBlock = 0;
	yDevice->endBlock = fileem_GetNumberOfBlocks()-1;
	yDevice->isYaffs2 = 1;
	yDevice->wideTnodesDisabled=0;
	yDevice->nShortOpCaches = nShortOpCaches; // Use caches
	yDevice->genericDevice = (void *) 2;	// Used to identify the device in fstat.
	yDevice->name = "sim";
	yDevice->writeChunkWithTagsToNAND = fileem_WriteChunkWithTagsToNAND;
	yDevice->readChunkWithTagsFromNAND = fileem_ReadChunkWithTagsFromNAND;
	yDevice->eraseBlockInNAND = fileem_EraseBlockInNAND;
	yDevice->initialiseNAND = fileem_InitialiseNAND;
	yDevice->markNANDBlockBad = fileem_MarkNANDBlockBad;
	yDevice->queryNANDBlock = fileem_QueryNANDBlock;
	yDevice->skipCheckpointRead = skipCPRead;
	yDevice->skipCheckpointWrite = skipCPWrite;
#ifdef YAFFS_ACP
	yDevice->acpDisabled = acpDisabled;
#endif
	yDevice->inbandTags = bUseInbandTags;
}

int fstest_LoadConfig(char * configFilePath, char * diskFilePath)
{
	/* we have a default configuration */
	fstest_LoadDefaultConfig();

	/* read from a configuration file */
	fstest_readConfigFile(configFilePath);

	/* augument from command line overrides any other configurations */
	if(diskFilePath)
		fileDiskPath = diskFilePath;

	/* load configuration */
	FileEmulatorInit(fileDiskPath, nPagesPerBlock,
		nPageDataSize, nPageOOBSize, bForceImgBlockAlign, bImgContainOOB);

	if(!bForceImgBlockAlign){
		skipCPWrite = 1;
		acpDisabled = 1;
	}

	fstest_InitDeviceCfg();
	yaffs_initialise(fstest_yaffsfs_config);

	return YAFFS_OK;
}

void SetCheckpointReservedBlocks(int n)
{
//	flashDev.nCheckpointReservedBlocks = n;
}

int fstest_yaffs_StartUp(char * configFilePath, char * diskFilePath)
{
	// Stuff to configure YAFFS
	// Stuff to initialise anything special (eg lock semaphore).
	yaffsfs_LocalInitialisation();

	// initialize file emulator and yaffs_Device
	fstest_LoadConfig(configFilePath, diskFilePath);

	return 0;
}

int fstest_yaffs_Shutdown(void)
{
	return 0;
}

