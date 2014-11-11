#ifndef _TEST_TOOL_H_
#define _TEST_TOOL_H_

#ifndef WIN32
#include <sys/types.h>
#include <unistd.h>
#endif

#include "com_type.h"
#include "OSAL_type.h"

#include "ICCHAL.h"

#include "ICCNet.h"

#define TEST_TIMES			(0x0001FEEE)



////////////////////////////////////////////////////////////////////////////////
//! \brief Constant Data: TEST_HW_SHM | TEST_HW_SEM
//!
////////////////////////////////////////////////////////////////////////////////
extern const UINT TEST_HW_SHM;
extern const UINT TEST_HW_SEM[MV_OSAL_CPU_NUM];


/**************** Test 0-2 Handle *****************/

INT SetTestFrame(UCHAR *pFrameBuf, UINT FrameSize, UINT seqid, UINT8 val);

INT CheckTestFrame(UCHAR *pFrameBuf, UINT FrameSize, UINT seqid);




/**************** Test 4-6 Handle *****************/

#define TASK_NUM				(5)
#define TASK_TEST_TIMES			(100)



typedef struct {
	int m_FullCount;
//	int m_PostErrCount;
} WrTask_statistics_t;


extern WrTask_statistics_t CpuWr_statistics[TASK_NUM];



INT TestRdStatisticsClear();

INT TestRdStatisticsPrint();

INT TestWrStatisticsClear();

INT TestWrStatisticsPrint();

INT TestFramePrint(UCHAR *pFrameBuf,
					UINT FrameSize);

INT TestFrameCreate(UCHAR *pFrameBuf,
					UINT FrameSize, 
					MV_OSAL_CPUID_U8_t SrcCpu,
					MV_OSAL_CPUID_U8_t DstCpu,
					UINT8 taskid,
					UINT32 seqid,
					UINT8 padding);
		
INT TestFrameAnalyze(UCHAR *pFrameBuf, UINT FrameSize);

INT TestICCNetStatisticsPrint(pMV_CC_ICCNet_t pHandle);




#define TestFrameGetSeq(x)		*(UINT32 *)(&(x[4]))

#define TestFrameGetTask(x)		x[8]

#define TestFrameGetData(x)		x[9]



/**************** Misc Test tools *****************/

void MsSleep(int msec);


/**************** Test for CBuf *****************/

#define CBufTestDataSize			(12345678)

#define CBufTestRandTime			((1 + (int)(20.0 * (rand() / (RAND_MAX + 1.0))))*10)

#define CBufTestBlockSize			(1024)

#define CBufTestBufSize 			(10*1024)

void CBufTest_SaveAppendToFile(const char *path, const UINT32 param1, const UINT32 param2);




#endif
