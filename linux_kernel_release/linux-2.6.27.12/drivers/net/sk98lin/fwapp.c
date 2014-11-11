/******************************************************************************
 *
 * Name:    fwapp.c
 * Project: Gigabit Ethernet Adapters, Common Modules
 * Version: $Revision: 1.2 $
 * Date:    $Date: 2010/02/02 14:33:41 $
 * Purpose: Embedded SDK firmware application code.
 *
 ******************************************************************************/

/******************************************************************************
 *
 *	LICENSE:
 *	(C)Copyright Marvell.
 *	
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *	
 *	The information in this file is provided "AS IS" without warranty.
 *	/LICENSE
 *
 *****************************************************************************/

#if (defined(DEBUG) || ((!defined(LINT)) && (!defined(SK_SLIM))))
static const char SysKonnectFileId[] =
"$Header: /data/cvs/sweprojects/yukon2/fwcommon/embedded_sdk/fwapp.c,v 1.2 2010/02/02 14:33:41 tschilli Exp $" ;
#endif

#include "h/skdrv1st.h"
#include "h/skdrv2nd.h"

/********************************************************
		Local Variables
********************************************************/

/********************************************************
		Global Variables
********************************************************/

/********************************************************
		Local Functions
********************************************************/

/********************************************************
		Global Functions
********************************************************/

/*****************************************************************************
 *
 *  @brief This function initializes the FW application
 *         code at level 0.
 *
 *  @param pAC      Pointer to adapter context
 *  @param IoC      IO context handle 
 *  @return         SK_TRUE or SK_FALSE
 */
int FwAppInit0(SK_AC *pAC, SK_IOC IoC)
{
	int RetCode = SK_ASF_PNMI_ERR_OK;

	SK_DBG_MSG(pAC, SK_DBGMOD_ASF, SK_DBGCAT_INIT,
		("%s: FwAppInit0 ==>\n", SK_DRV_NAME));

	/* Set structure to zero */
	SK_MEMSET((char *)&(pAC->FwApp), 0, sizeof(SK_FWAPP));

	pAC->FwApp.ActivePort   = 0;
	pAC->FwApp.DualMode     = SK_GEASF_Y2_SINGLEPORT;
	pAC->FwApp.OpMode       = SK_GEASF_MODE_UNKNOWN;
	pAC->FwApp.ChipMode     = SK_GEASF_CHIP_UNKNOWN;
	pAC->FwApp.InitState    = ASF_INIT_UNDEFINED;
	pAC->RamAddr = 0;
	pAC->RamSelect = 0;

	SK_DBG_MSG(pAC, SK_DBGMOD_ASF, SK_DBGCAT_INIT,
		("%s: FwAppInit0 <==\n", SK_DRV_NAME));

	return RetCode;

} /* FwAppInit0 */

/*****************************************************************************
 *
 *  @brief This function initializes the FW application
 *         code at level 1.
 *
 *  @param pAC      Pointer to adapter context
 *  @param IoC      IO context handle 
 *  @return         SK_TRUE or SK_FALSE
 */
int FwAppInit1(SK_AC *pAC, SK_IOC IoC)
{
	int RetCode = SK_ASF_PNMI_ERR_OK;

	SK_DBG_MSG(pAC, SK_DBGMOD_ASF, SK_DBGCAT_INIT,
		("%s: FwAppInit1 ==>\n", SK_DRV_NAME));

	/* Check chip ID and set appropriate flash offsets, */
	/* operational modes and chip modes depending on Chip ID. */
	switch (pAC->FwCommon.ChipID) {
	case CHIP_ID_YUKON_SUPR:
		pAC->FwApp.ChipMode        = SK_GEASF_CHIP_SU;
		pAC->FwApp.FlashSize       = ASF_FLASH_SU_SIZE;

		SK_STRNCPY(pAC->FwApp.DriverVersion, FW_API_VERSION, FW_API_VERSION_SIZE);

		SK_DBG_MSG(pAC, SK_DBGMOD_ASF, SK_DBGCAT_INIT,
			("%s: FwAppInit1: pAC->FwApp.DriverVersion: %s!\n",
			SK_DRV_NAME, pAC->FwApp.DriverVersion));

		break;

	default:
		/* Chip ID does not match! */
		pAC->FwApp.InitState = ASF_INIT_ERROR_CHIP_ID;
		printk("%s: FwAppInit1: Chip ID does not match!\n", SK_DRV_NAME);

		RetCode = SK_ASF_PNMI_ERR_GENERAL;
		break;
	}

	if (RetCode != SK_ASF_PNMI_ERR_OK) {
		pAC->FwApp.InitState = ASF_INIT_ERROR_CHIP_ID;

		SK_DBG_MSG(pAC, SK_DBGMOD_ASF, SK_DBGCAT_INIT,
			("%s: FwAppInit1 <==\n", SK_DRV_NAME));

		return RetCode;
	}

	SetRamAddr(pAC, SK_ST_FIFOTYPE, HOST_READ_QWORD, SK_ST_BUFADDR_HIGH);
	pAC->FwApp.OpMode = SK_GEASF_MODE_EMBEDDED_SDK;

	SK_DBG_MSG(pAC, SK_DBGMOD_ASF, SK_DBGCAT_INIT,
		("%s: FwAppInit1 <==\n", SK_DRV_NAME));

	return RetCode;

} /* FwAppInit1 */

/*****************************************************************************
 *
 *  @brief This function initializes the FW application
 *         code at level 2.
 *
 *  @param pAC      Pointer to adapter context
 *  @param IoC      IO context handle 
 *  @return         SK_TRUE or SK_FALSE
 */
int FwAppInit2(SK_AC *pAC, SK_IOC IoC)
{
	int RetCode = SK_ASF_PNMI_ERR_OK;

	SK_DBG_MSG(pAC, SK_DBGMOD_ASF, SK_DBGCAT_INIT,
		("%s: FwAppInit2 ==>\n", SK_DRV_NAME));

	/* FW initialization ok */
	pAC->FwApp.InitState = ASF_INIT_OK;

	/* Check if CPU is running */
	if (FwCpuState(pAC, IoC) != ASF_CPU_STATE_RUNNING) {
		SK_DBG_PRINTF("%s: FwAppInit2: CPU is NOT running! Start CPU!\n", SK_DRV_NAME);

		/* If not, start CPU */
		FwStartCpu(pAC, IoC);
		pAC->FwApp.CpuAlive = 1;
	}

	SK_DBG_MSG(pAC, SK_DBGMOD_ASF, SK_DBGCAT_INIT,
		("%s: FwAppInit2: INIT OK pAC->FwApp.InitState: %d pAC->FwApp.OpMode: %d\n",
		SK_DRV_NAME, pAC->FwApp.InitState, pAC->FwApp.OpMode));

	/* Otherwise the transmitter and the receiver will be */
	/* deactivated if we shut down the network interface. */
	FwAsfEnable(pAC, IoC);

	SK_DBG_PRINTF("%s: Checking Driver <-> FW communication ...", DRV_NAME);
	if (FwHciSendCommand(pAC, IoC, YASF_HOSTCMD_CHECK_ALIVE, 0, 0, 0, ASF_HCI_WAIT, 0) != HCI_EN_CMD_READY) {
		SK_DBG_PRINTF(" FAILED!\n");
		pAC->FwState = SK_FALSE;
	} else {
		SK_DBG_PRINTF(" working!\n");
		pAC->FwState = SK_TRUE;
	}

	SK_DBG_MSG(pAC, SK_DBGMOD_ASF, SK_DBGCAT_INIT,("%s: FwAppInit2 <==\n", SK_DRV_NAME));

	return RetCode;

} /* FwAppInit2 */

/*****************************************************************************
 *
 *  @brief This function prepares the patches for the FW image.
 *
 *  @param pAC      Pointer to adapter context
 *  @param IoC      IO context handle 
 *  @return         SK_TRUE or SK_FALSE
 */
int FwAppPatchImage(SK_AC *pAC, SK_IOC IoC)
{
	int RetCode;

	/* Nothing to patch... */
	pAC->FwCommon.PatchNumber = 0;

	SK_DBG_MSG(pAC, SK_DBGMOD_ASF, SK_DBGCAT_INIT,
		("%s: FwAppPatchImage: pAC->FwCommon.PatchNumber: %d\n",
		SK_DRV_NAME, pAC->FwCommon.PatchNumber));

	RetCode = SK_TRUE;
	return RetCode;

} /* FwAppPatchImage */

/*****************************************************************************
 *
 *  @brief This function returns the full pathname
 *         of the FW image file.
 *
 *  @param pAC      Pointer to adapter context
 *  @return         Pointer to full FW file pathname
 */
char *FwAppGetImageName(SK_AC *pAC)
{
	SK_DBG_MSG(pAC, SK_DBGMOD_ASF, SK_DBGCAT_INIT,
		("%s: FwAppGetImageName: pAC->FwOs.pFilePathName: %s\n",
		SK_DRV_NAME, pAC->FwOs.pFilePathName));

	return pAC->FwOs.pFilePathName;

} /* FwAppGetImageName */

/*****************************************************************************
 *
 *  @brief This function compares the required FW version
 *         with the version found in the FW image file.
 *
 *  @param pAC      Pointer to adapter context
 *  @return         SK_TRUE or SK_FALSE
 */
SK_BOOL FwAppIsVersionOk(SK_AC *pAC)
{
	SK_BOOL RetCode = SK_TRUE;
	int     i;

	/*
	 * Compare file firmware version with
	 * required firmware version.
	 */
	for (i = 0; i < FW_API_VERSION_SIZE; i++) {
		if (pAC->FwApp.DriverVersion[i] != pAC->FwCommon.FileVersion[i]) {
			RetCode = SK_FALSE;
		}
	}

	SK_DBG_MSG(pAC, SK_DBGMOD_ASF, SK_DBGCAT_INIT,
		("%s: FwAppIsVersionOk: RetCode: %d\n", SK_DRV_NAME, RetCode));

	return RetCode;

} /* FwAppIsVersionOk */

/*****************************************************************************
*
* FwMainStateMachine - general ASF  timer
*
* Description:
*
* Returns:
*   Always 0
*/
void FwMainStateMachine(
SK_AC  *pAC,	/* Pointer to adapter context */
SK_IOC IoC)		/* IO context handle */
{
	SK_EVPARA EventParam;

	SK_DBG_MSG(pAC, SK_DBGMOD_ASF, SK_DBGCAT_INIT,
		("%s: FwMainStateMachine() called!\n", SK_DRV_NAME));

	if (pAC->FwApp.GlHciState == 0) { /* idle */

		SK_MEMSET((char *)&EventParam, 0, sizeof(EventParam));
		SkTimerStart(pAC, IoC, &pAC->FwApp.AsfTimer,
					1000000, SKGE_ASF, SK_ASF_EVT_TIMER_EXPIRED,
					EventParam);
	}
	else {
		SK_MEMSET((char *)&EventParam, 0, sizeof(EventParam));
		SkTimerStart(pAC, IoC, &pAC->FwApp.AsfTimer,
					30000, SKGE_ASF, SK_ASF_EVT_TIMER_EXPIRED,
					EventParam);
	}

	return;

} /* FwMainStateMachine */

/*****************************************************************************
*
* FwDriverHello - Hand over control from firmware to driver
*
* Description:
*
* Returns:
*   Nothing
*/
void FwDriverHello(
SK_AC  *pAC,	/* Pointer to adapter context */
SK_IOC IoC)		/* IO context handle */
{
	SK_U8   Result;
	SK_U8   TmpBuffer[4];

	/* Set OS Present Flag in ASF Status and Command Register */
	FwSetOsPresentBit(pAC, pAC->IoBase);

	TmpBuffer[0] = YASF_HOSTCMD_DRV_HELLO;
	TmpBuffer[1] = 3; /* Length */
	TmpBuffer[2] = 0; /* Original handling */
	TmpBuffer[3] = 0;

	/* Set the FW queue handling values */
	if (pAC->SdkQH == SK_TRUE) {
		if (pAC->Suspended)
			TmpBuffer[2] = 3; /* Suspend/new handling */
		else
			TmpBuffer[2] = 1; /* New handling */
	} else {
		if (pAC->Suspended)
			TmpBuffer[2] = 2; /* Suspend */
	}

	Result = FwHciSendData(pAC, pAC->IoBase, TmpBuffer, 0, ASF_HCI_WAIT, 0);
	if (Result == HCI_EN_CMD_ERROR) {
		printk("%s: FwDriverHello: YASF_HOSTCMD_DRV_HELLO NOT in FW!\n",
			SK_DRV_NAME);
		pAC->FwState = SK_FALSE;
	} else {
		printk("%s: FwDriverHello: YASF_HOSTCMD_DRV_HELLO in FW!\n",
			SK_DRV_NAME);
	}

	return;

} /* FwDriverHello */

/*****************************************************************************
*
* FwDriverGoodbye - Hand over control from driver to firmware
*
* Description:
*
* Returns:
*   Nothing
*/
void FwDriverGoodbye(
SK_AC  *pAC,	/* Pointer to adapter context */
SK_IOC IoC)		/* IO context handle */
{
	SK_U8   Result;
	SK_U8   TmpBuffer[4];

	/* Reset OS Present Flag in ASF Status and Command Register */
	FwResetOsPresentBit(pAC, pAC->IoBase);

	/* Inform the FW that the driver will be unloaded */
	TmpBuffer[0] = YASF_HOSTCMD_DRV_GOODBYE;
	TmpBuffer[1] = 3; /* Length */
	TmpBuffer[2] = 0; /* Original handling */
	TmpBuffer[3] = 0;

	/* Set the FW queue handling values */
	if (pAC->SdkQH == SK_TRUE) {
		if (pAC->Suspended)
			TmpBuffer[2] = 3; /* Suspend/new handling */
		else
			TmpBuffer[2] = 1; /* New handling */
	} else {
		if (pAC->Suspended)
			TmpBuffer[2] = 2; /* Suspend */
	}

	Result = FwHciSendData(pAC, pAC->IoBase, TmpBuffer, 0, ASF_HCI_WAIT, 0);
	if (Result == HCI_EN_CMD_ERROR) {
		printk("%s: FwDriverGoodbye: YASF_HOSTCMD_DRV_GOODBYE NOT in FW!\n",
			SK_DRV_NAME);
		pAC->FwState = SK_FALSE;
	} else {
		printk("%s: FwDriverGoodbye: YASF_HOSTCMD_DRV_GOODBYE in FW!\n",
			SK_DRV_NAME);
	}

	return;

} /* FwDriverGoodbye */

#ifdef MV_INCLUDE_FW_SDK_LINK_MAINTENANCE
/*****************************************************************************
*
* FwCheckLinkMode - Check the firmware link state and link mode
*
* Description:
*
* Returns:
*   Nothing
*/
void FwCheckLinkMode(
SK_AC  *pAC,	/* Pointer to adapter context */
SK_IOC IoC,		/* IO context handle */
int    Port)
{
	SK_U16 PhyStat;
	SK_U32 PhyCtrl;

	PhyStat = 0x0;
	PhyCtrl = 0x0;

	pAC->GIni.GIDontInitPhy = SK_FALSE;

	if (pAC->UseLinkMaintenance == SK_TRUE) {

		/* Check if link is up */
		SK_IN32(pAC->IoBase, MR_ADDR(Port, GPHY_CTRL), &PhyCtrl);

		if ((PhyCtrl & GPC_PHY_LINK_UP) == 0) {
#ifdef MV_FW_SDK_LINK_MAINTENANCE_DEBUG
			printk("FwCheckLinkMode: No link on port %d\n", Port);
#endif
			return;
		}
		else {
#ifdef MV_FW_SDK_LINK_MAINTENANCE_DEBUG
			printk("FwCheckLinkMode: Link up on port %d\n", Port);
#endif
			/* Check link state */
			GM_IN16(IoC, Port, GM_GP_CTRL, &PhyStat);
#ifdef MV_FW_SDK_LINK_MAINTENANCE_DEBUG
			printk("FwCheckLinkMode: GM_GP_CTRL:  0x%x\n",
				PhyStat);
#endif
			if (pAC->DriverLinkStat != PhyStat) {
#ifdef MV_FW_SDK_LINK_MAINTENANCE_DEBUG
				printk("FwCheckLinkMode: Link status changed\n");
				printk("FwCheckLinkMode: pAC->DriverLinkStat:     0x%x\n",
					pAC->DriverLinkStat);
#endif
				return;
			}

			/* Store link state */
			pAC->DriverLinkStat = PhyStat;
			
			/* Enable link maintenance */
			pAC->GIni.GIDontInitPhy = SK_TRUE;
#ifdef MV_FW_SDK_LINK_MAINTENANCE_DEBUG
			printk("FwCheckLinkMode: Link match, GIDontInitPhy: %d\n",
				pAC->GIni.GIDontInitPhy);
#endif
		}
	}
	else {
#ifdef MV_FW_SDK_LINK_MAINTENANCE_DEBUG
		printk("FwCheckLinkMode: UseLinkMaintenance not set!\n");
#endif
	}

	return;

} /* FwCheckLinkMode */
#endif

