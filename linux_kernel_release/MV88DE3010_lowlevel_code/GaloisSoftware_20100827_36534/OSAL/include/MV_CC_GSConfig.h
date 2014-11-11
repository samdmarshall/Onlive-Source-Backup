/*******************************************************************************
*******************************************************************************/



#ifndef MV_CC_GSConfig_H
#define MV_CC_GSConfig_H

#include "com_type.h"
#include "OSAL_config.h"
#include "OSAL_type.h"

#ifdef __cplusplus
extern "C"
{
#endif	// #ifdef __cplusplus


#define MV_CC_ServiceID_None				(0x00000000)
#define MV_CC_ServiceID_StaticStart			(0x00000001)
#define MV_CC_GSConfigList_EndFlagID		(0x7FFFFFFF)
#define MV_CC_ServiceID_DynamicStart		(0x80000000)
#define MV_CC_ServiceID_DynamicApply		(0x80000000)



/*
** Service ID for Test Service
*/

#define MV_GS_AppMsgQ00						(0x00000001)
#define MV_GS_AppMsgQ01						(0x00000002)
#define MV_GS_AppMsgQ02						(0x00000003)

#define MV_GS_AppRPC01						(0x00000004)
#define MV_GS_AppRPC02						(0x00000005)
#define MV_GS_AppRPC03						(0x00000006)

#define MV_GS_AppCBUF01						(0x00000007)
#define MV_GS_AppCBUF02						(0x00000008)
#define MV_GS_AppCBUF03						(0x00000009)
#define MV_GS_AppCBUF04						(0x0000000a)

#define MV_GS_AppCBUF01_CPU1				(0x0000000b)
#define MV_GS_AppCBUF02_CPU1				(0x0000000c)
#define MV_GS_AppCBUF03_CPU1				(0x0000000d)
#define MV_GS_AppCBUF04_CPU1				(0x0000000e)

#define MV_GS_AppRPC01_CPU1					(0x0000000f)

#define MV_GS_TestMsgQEx0					(0x00000010)
#define MV_GS_TestMsgQEx1					(0x00000011)

#define MV_GS_CUnitMsgQInCPU0				(0x00000020)
#define MV_GS_CUnitMsgQInCPU1				(0x00000021)
#define MV_GS_CUnitRPCServer1				(0x00000022)
#define MV_GS_CUnitRPCServer2				(0x00000023)
#define MV_GS_CUnitRPCServer3				(0x00000024)
#define MV_GS_CUnitCBufServer1				(0x00000025)
#define MV_GS_CUnitCBufServer2				(0x00000026)
#define MV_GS_CUnitCBufServer3				(0x00000027)

/*
** Service ID for System Service
*/

#define MV_GS_SysSrvStatus_CPU0				(0x00000100)
#define MV_GS_SysSrvStatus_CPU1				(0x00000101)
#define MV_GS_SysRPC_SHM					(0x00000102)
#define MV_GS_SysRPC_DSS					(0x00000103)
#define MV_GS_EventMsgQ						(0x00000104)
#define MV_GS_MonitorCPU1					(0x00000105)
#define MV_GS_AppRPC_GFX					(0x00800001)


/*
**service id for PE
*/

#define MV_GS_AppSrv						(0x00010001)
#define MV_GS_InbSrv						(0x00020000)
#define MV_GS_SrcSrv						(0x00030001)
#define MV_GS_StmSrv						(0x00040001)
#define MV_GS_PspSrv						(0x00050001)
#define MV_GS_CbkInvoke						(0x00060001)
#define MV_GS_CbkReg						(0x00060002)
#define MV_GS_CbkCbufSrv					(0x00060003)
#define MV_GS_Gfx_CmdQ_0_1 			        (0x00060004)
#define MV_GS_Gfx_CmdQ_0_2 			        (0x00060005)
#define MV_GS_Gfx_CmdQ_0_3 			        (0x00060006)
#define MV_GS_Gfx_CmdQ_0_4 			        (0x00060007)


/*
**define service id for DRM module
*/
#ifdef BERLIN_SINGLE_CPU
#define MV_GS_DMX_Serv 			            (0x00060008)
#define MV_GS_VPP_Serv 			            (0x00060009)
#define MV_GS_AUD_Serv				    (0x0006000a)
#endif


/*
**define service id for UI module
*/

#define MV_GS_UISrv1						(0x00200001)
#define MV_GS_UISrv2						(0x00200002)



/*
 * define service id for OnlineUpgrade
 */
#define	MV_OU_MsgQ				(0x00300001)


typedef struct _MV_CC_GSConfigItem {

	char m_ServiceName[20];

	MV_CC_ServiceID_U32_t m_ServiceID;	/* when m_SrvLevel = IPC, m_ServiceID is just as id for IPC */

	MV_CC_SrvLevel_U8_t m_SrvLevel;
	MV_CC_SrvType_U8_t m_SrvType;
	
	MV_CC_ICCAddr_t m_SrvAddr;
					
	UINT32 Option;

} MV_CC_GSConfigItem_t;


extern const MV_CC_GSConfigItem_t MV_CC_GSConfigList[];



#define MV_CC_GSConfigList_GetSrvAddr(x)	(MV_CC_ICCAddr_t *)(&(MV_CC_GSConfigList[x].m_SrvAddr))



INT GSConfigList_Search(MV_CC_ServiceID_U32_t ServiceID);


#ifdef __cplusplus
}		// extern "C"
#endif	// #ifdef __cplusplus

#endif
