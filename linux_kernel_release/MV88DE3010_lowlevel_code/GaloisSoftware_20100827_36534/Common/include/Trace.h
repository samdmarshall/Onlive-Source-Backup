/********************************************************************************
*********************************************************************************/


#ifndef _TRACE_H_
#define _TRACE_H_

#ifdef WIN32
#define ENABLE_TRACE
#endif

#ifdef ENABLE_TRACE

#define TRACE_PUSH Trace_Push((CHAR*)__FILE__, (UINT)__LINE__, (CHAR*)__FUNCTION__, (HRESULT)(hr), (CHAR*)"")

HRESULT Trace_Push(CHAR* szFileName, UINT uiLineNumber, CHAR* szFunctionName, HRESULT hr, CHAR* szReserved);
HRESULT Trace_Dump();
HRESULT Trace_Clear();

#else //<! #ifdef ENABLE_TRACE

#define TRACE_PUSH
#define Trace_Push()
#define Trace_Dump()
#define Trace_Clear()

#endif //<! #ifdef ENABLE_TRACE

#endif //<! _TRACE_H_
