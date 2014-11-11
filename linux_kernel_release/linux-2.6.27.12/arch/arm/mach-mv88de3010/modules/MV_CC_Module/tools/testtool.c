

#if defined( __LINUX__ )
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdio.h>
#elif defined( __ECOS__ )
#include <cyg/kernel/kapi.h>
#include <sys/time.h>
#endif

#include "com_type.h"

#include "OSAL_type.h"
#include "OSAL_Local_CPU_def.h"

#include "testtool.h"
#include "ICCNet.h"



/**************** Test Frame Handle *****************/
	
INT SetTestFrame(UCHAR *pFrameBuf, UINT FrameSize, UINT seqid, UINT8 val)
{
	if ((pFrameBuf == NULL) || (FrameSize < 20))
		MV_OSAL_ErrorHandle("SetTestFrame",E_INVALIDARG);
		
	((UINT *)pFrameBuf)[0] = seqid;
	GaloisMemSet((void *)((UCHAR *)(pFrameBuf) + sizeof(UINT)), 
					val, FrameSize - sizeof(UINT));

	return S_OK;	
}


INT CheckTestFrame(UCHAR *pFrameBuf, UINT FrameSize, UINT seqid)
{
	UINT i;
	UCHAR c;
	
	if ((pFrameBuf == NULL) || (FrameSize < 20))
		MV_OSAL_ErrorHandle("CheckTestFrame",E_INVALIDARG);
		
	if (((UINT *)pFrameBuf)[0] != seqid)
		MV_OSAL_ErrorHandle("CheckTestFrame",E_FAIL);
		
	i = sizeof(UINT);
	c = ((UCHAR *)(pFrameBuf))[i];
	i++;
	
	for (i; i<FrameSize; i++)
		if (c != ((UCHAR *)(pFrameBuf))[i])
			MV_OSAL_ErrorHandle("CheckTestFrame",E_FAIL);
			
	return S_OK;
}



/**************** Test 4-6 Frame Handle *****************/

int CpuRd_statistics[MV_OSAL_CPU_NUM][TASK_NUM];

WrTask_statistics_t CpuWr_statistics[TASK_NUM];




INT TestRdStatisticsClear()
{
	int i,j;
	
	for (i=0; i<MV_OSAL_CPU_NUM; i++)
	for (j=0; j<TASK_NUM; j++)
		CpuRd_statistics[i][j] = 0;

	return S_OK;
}

INT TestRdStatisticsPrint()
{
	int i,j;

	LOGPRINTF(("-----  CPU-%d Read Test Statistics  -----\n", MV_OSAL_CPU_LOCAL));	
	LOGPRINTF(("----------------------------------------\n"));	
	LOGPRINTF((" Src CPU |"));
	for (j=0; j<TASK_NUM; j++)
		LOGPRINTF((" Task %2d |", j));		
	LOGPRINTF(("\n"));
	LOGPRINTF(("----------------------------------------\n"));	
	
	for (i=0; i<MV_OSAL_CPU_NUM; i++) {
		
		LOGPRINTF(("  CPU-%1d  |", i));
		
		for (j=0; j<TASK_NUM; j++)
			LOGPRINTF(("  %6d |", CpuRd_statistics[i][j]));
			
		LOGPRINTF(("\n"));
		
	}	
	
	LOGPRINTF(("----------------------------------------\n"));	
	return S_OK;		
}

INT TestWrStatisticsClear()
{
	int i;
	
	for (i=0; i<TASK_NUM; i++)
	{
		CpuWr_statistics[i].m_FullCount = 0;
		//CpuWr_statistics[i].m_PostErrCount = 0;
	}

	return S_OK;
}

INT TestWrStatisticsPrint()
{
	int i,j;

	LOGPRINTF(("-----  CPU-%d Write Test Statistics -----\n", MV_OSAL_CPU_LOCAL));	
	LOGPRINTF(("----------------------------------------\n"));	
	LOGPRINTF(("   Item  |"));
	for (j=0; j<TASK_NUM; j++)
		LOGPRINTF((" Task %2d |", j));		
	LOGPRINTF(("\n"));
	LOGPRINTF(("----------------------------------------\n"));	
	
	LOGPRINTF((" Wr Full |"));
	
	for (i=0; i<TASK_NUM; i++)
		LOGPRINTF(("  %6d |", CpuWr_statistics[i].m_FullCount));
			
	LOGPRINTF(("\n"));

	//LOGPRINTF((" PostErr |"));
		
	//for (j=0; j<TASK_NUM; j++)
	//	LOGPRINTF(("  %6d |", CpuWr_statistics[i].m_PostErrCount));
			
	//LOGPRINTF(("\n"));

	LOGPRINTF(("----------------------------------------\n"));	
	
	return S_OK;		
}

INT TestFramePrint(UCHAR *pFrameBuf,
					UINT FrameSize)
{
	UINT i,count = 0, column;

	LOGPRINTF(("------------------------------------------------------------\n"));	
	LOGPRINTF(("Address   |"));
	for (i=0; i<16; i++)
		LOGPRINTF((" %02X", i));		
	LOGPRINTF(("\n"));
	LOGPRINTF(("------------------------------------------------------------\n"));
		
	while (count < FrameSize) {
		
		LOGPRINTF(("%08Xh |", count));
		
		for (column=0; column<16; column++) {
			
			LOGPRINTF((" %02X", pFrameBuf[count]));
			
			count++;
			if (count == (FrameSize) )
				break;
		}
		LOGPRINTF(("\n"));
	}	
	
	LOGPRINTF(("------------------------------------------------------------\n"));	
	return S_OK;		
}


INT TestICCNetStatisticsPrint(pMV_CC_ICCNet_t pHandle)
{
	LOGPRINTF(("\n"));
	LOGPRINTF(("-----    CPU-%d ICCNet Statistics   -----\n", MV_OSAL_CPU_LOCAL));	
	LOGPRINTF(("----------------------------------------\n"));	
	LOGPRINTF(("  RdCount        | %10d  |\n", pHandle->m_RdCount));
	LOGPRINTF(("  RdEmptyCount   | %10d  |\n", pHandle->m_RdEmptyCount));
	LOGPRINTF(("----------------------------------------\n"));	
	LOGPRINTF(("  WrCount        | %10d  |\n", pHandle->m_WrCount));
	LOGPRINTF(("  WrFullCount    | %10d  |\n", pHandle->m_WrFullCount));		
	LOGPRINTF(("  WrSemErrCount  | %10d  |\n", pHandle->m_WrSemErrCount));	
	LOGPRINTF(("----------------------------------------\n"));	
	LOGPRINTF(("\n"));	
	
	return S_OK;			
	
}


INT TestFrameCreate(UCHAR *pFrameBuf,
					UINT FrameSize, 
					MV_OSAL_CPUID_U8_t SrcCpu,
					MV_OSAL_CPUID_U8_t DstCpu,
					UINT8 taskid,
					UINT32 seqid,
					UINT8 padding)
{

	if ((pFrameBuf == NULL) || 
		(FrameSize < 20) ||
		(SrcCpu >= MV_OSAL_CPU_NUM) ||
		(DstCpu >= MV_OSAL_CPU_NUM) ||
		(taskid >= TASK_NUM))
		MV_OSAL_ErrorHandle("TestFrameCreate",E_INVALIDARG);
		
	pFrameBuf[0] = SrcCpu;
	pFrameBuf[1] = 0;
	pFrameBuf[2] = DstCpu;	
	pFrameBuf[3] = 0;
	
	*(UINT32 *)(&(pFrameBuf[4])) = seqid;
	
	pFrameBuf[8] = taskid;
	
	GaloisMemSet((void *)((UCHAR *)(pFrameBuf) + 9), 
					padding, FrameSize - 9);

	return S_OK;
	
}


INT TestFrameAnalyze(UCHAR *pFrameBuf, UINT FrameSize)
{
	MV_OSAL_CPUID_U8_t SrcCpu;
	MV_OSAL_CPUID_U8_t DstCpu;
	UINT8 taskid;
	UINT32 seqid;
	UINT8 padding;
	
	UINT i;	
	
	if ((pFrameBuf == NULL) || (FrameSize < 20))
		MV_OSAL_ErrorHandle("TestFrameAnalyze",E_INVALIDARG);
	
	
	if (((SrcCpu = pFrameBuf[0]) >= MV_OSAL_CPU_NUM) ||
		(pFrameBuf[1] != 0) ||
		((DstCpu = pFrameBuf[2]) != MV_OSAL_CPU_LOCAL) ||
		(pFrameBuf[3] != 0) ||
		((taskid = pFrameBuf[8]) >= TASK_NUM))
	{
		TestFramePrint(pFrameBuf, FrameSize);
		MV_OSAL_ErrorHandle("TestFrameAnalyze step 1",E_BADVALUE);
	}

	seqid = *(UINT32 *)(&(pFrameBuf[4]));

	if (seqid != (CpuRd_statistics[SrcCpu][taskid] + 1))
	{
		TestFramePrint(pFrameBuf, FrameSize);
		MV_OSAL_ErrorHandle("TestFrameAnalyze step 2",E_BADVALUE);
	}
	
	CpuRd_statistics[SrcCpu][taskid]++;

			
	i = 9;	
	padding = pFrameBuf[i];
	i++;
	
	for (i; i<FrameSize; i++)
		if (padding != ((UCHAR *)(pFrameBuf))[i])
		{
			TestFramePrint(pFrameBuf, FrameSize);
			MV_OSAL_ErrorHandle("TestFrameAnalyze step 3",E_BADVALUE);
		}
		
	return S_OK;
	
}

void MsSleep(int msec)     
 {     
#if defined( WIN32 )
	 Sleep(msec);
#else
	struct timeval timeout;     
	    
	GaloisMemSet(&timeout, 0, sizeof(struct timeval));
	    
	if(msec >= 1000)
	{   
		timeout.tv_sec = msec/1000;     
		timeout.tv_usec = 0;   
	}   
	else     
	{   
		timeout.tv_sec = 0;     
		timeout.tv_usec = msec;
	}   
	    
#if defined( __LINUX__ )
	select(0, NULL, NULL, NULL, &timeout);  
#elif defined( __ECOS__ )
	cyg_thread_delay( (cyg_tick_count_t)( msec * 6000 ) );
#endif
#endif //ndef WIN32

	return ;  
}   



void CBufTest_SaveAppendToFile(const char *path, const UINT32 param1, const UINT32 param2)
{
	FILE *fp;

#ifdef WIN32
#pragma warning( push )
#pragma warning( disable: 4996 )
#endif
	fp = fopen(path, "a");
	if (fp == NULL)
	{
		MV_OSAL_ErrorPrint("CBufTest_SaveAppendToFile fopen", E_FILEOPEN);
		return ;
	}
		
	fprintf(fp,"%d,%d\n", param1, param2);
	
	fclose(fp);	
#ifdef WIN32
#pragma warning ( pop )
#endif

	return ;	
}

