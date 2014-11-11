#include "mv_include.h"

#include "core_main.h"

#include "core_sata.h"

#include "hba_exp.h"
#include "hba_mod.h"
#include "hba_timer.h"

// from Common/include
#include "sys_memmap.h"
#include "sys_config_partition_layout.h"

/*
 * imported from linux_main.c
 */
extern int early_host_reset;
extern int lowlevel_probe_done;
extern int highlevel_probe_done;
extern int enable_disc_ready;
extern int wait_loader_ready; /* only for Spruce flash-less */
extern int spruce_flashless_loader;

volatile int berlin_spruce_loader = 0; /* if any port connects spruce loader, it's 1 */
EXPORT_SYMBOL(berlin_spruce_loader);

#ifdef BERLIN_SUPPORT_SPRUCE_DRIVE
static void Device_PowerStandby(PDomain_Port pPort, PDomain_Device pDevice);
static void Device_StartUnit(PDomain_Port pPort, PDomain_Device pDevice);
#endif
#ifdef BERLIN_ATAPI_GET_READY
static void Device_TestUnitReady(PDomain_Port pPort, PDomain_Device pDevice);
static void Device_MakeRequestSense(PDomain_Port pPort, PDomain_Device pDevice);
#endif
#ifdef BERLIN_SPRUCE_INTEGRATION
extern char spruce_microcode_start[];
extern char spruce_microcode_end[];
extern char spruce_variable_end[];
static void Device_SpruceDownloadCode(PDomain_Port pPort, PDomain_Device pDevice);
static void Device_SpruceExecuteCode(PDomain_Port pPort, PDomain_Device pDevice);
#endif
static void Device_IssueIdentify(PDomain_Port pPort, PDomain_Device pDevice);
static void Device_IssueSetUDMAMode(PDomain_Port pPort, PDomain_Device pDevice);
static void Device_IssueSetPIOMode(PDomain_Port pPort, PDomain_Device pDevice);
static void Device_EnableWriteCache(PDomain_Port pPort, PDomain_Device pDevice);
static void Device_EnableReadAhead(PDomain_Port pPort, PDomain_Device pDevice);

PMV_Request GetInternalReqFromPool( PCore_Driver_Extension pCore )
{
	if( !List_Empty(&pCore->Internal_Req_List) )
		return ((PMV_Request)List_GetFirstEntry(&pCore->Internal_Req_List, 
					MV_Request, Queue_Pointer));
	else
		return NULL;
}

void ReleaseInternalReqToPool( PCore_Driver_Extension pCore, PMV_Request pReq)
{
	MV_ZeroMvRequest(pReq);
	List_AddTail( &pReq->Queue_Pointer, &pCore->Internal_Req_List );	
}

/*
 * Initialize this port including possible device hard or soft reset.
 * called by __core_req_timeout_handler and SATA_SoftResetDevice
 */
MV_BOOLEAN SATA_DoSoftReset(PDomain_Port pPort, MV_U8 PMPort)
{
	MV_U8 tag = Tag_GetOne(&pPort->Tag_Pool);
	PMV_Command_Header header = SATA_GetCommandHeader(pPort, tag);
	PMV_Command_Table pCmdTable = Port_GetCommandTable(pPort, tag);
	PSATA_FIS_REG_H2D pFIS = (PSATA_FIS_REG_H2D)pCmdTable->FIS;
	PCore_Driver_Extension pCore = pPort->Core_Extension;
	MV_LPVOID portMmio = pPort->Mmio_Base;
	MV_U32 old_stat;
	MV_U32 temp = 0, count = 0;
	MV_U8 reset = 1;

	if( PMPort == 0xF )
		MV_DASSERT( tag == 0 );

	if (PMPort != 0x0) {
		MV_ERROR_PRINT("Error: Driver doesn't support port multifier.\n");
		return MV_FALSE;
	}

	/* toggle the start bit in cmd register to make sure CI is cleared. */
	temp = MV_REG_READ_DWORD( portMmio, PORT_CMD );
	MV_REG_WRITE_DWORD( portMmio, PORT_CMD, temp & ~MV_BIT(0));
	MV_ERROR_PRINT(("[Galois]Waiting for 0.5 second to start host.\n"));
	HBA_DelayMillisecond(pCore, 500);
	MV_REG_WRITE_DWORD( portMmio, PORT_CMD, temp | MV_BIT(0));

	temp = MV_REG_READ_DWORD(portMmio, PORT_TFDATA);
	if ((temp & PORT_TF_STATUS_DRQ) || (temp & PORT_TF_STATUS_ERR)) {
		MV_PRINT("Error: There's DRQ or ERR is TaskFileData[0x%08x]\n", temp);
		return MV_FALSE;
	}

	mvDisableIntr(portMmio, old_stat);
	do
	{
		MV_ERROR_PRINT("[Galois]SATA_DoSoftReset:Sending out dummy reset(%d) frame... ", reset);
		MV_ZeroMemory(header, sizeof(MV_Command_Header));
		MV_ZeroMemory(pCmdTable, sizeof(MV_Command_Table));

		/*
		 * According to SerialATA_AHCI_rev1_2.pdf
		 * For 1st FIS, CH.R=1, CH.C=1; FIS->Control.SRST=1, FIS->C=0
		 * For 2nd FIS, CH.R=0, CH.C=0; FIS->Control.SRST=0, FIS->C=0
		 * XXX: Marvell HOST doesn't set CH.C=1 for 1st FIS.
		 */
		header->FIS_Length = FIS_REG_H2D_SIZE_IN_DWORD;
		header->Reset = (reset)? 1:0;
		header->PM_Port = PMPort;

		*((MV_U16 *) header) = CPU_TO_LE_16( *((MV_U16 *) header) );
		header->Table_Address = 
				CPU_TO_LE_32(pPort->Cmd_Table_DMA.low + SATA_CMD_TABLE_SIZE*tag);
		header->Table_Address_High = CPU_TO_LE_32(pPort->Cmd_Table_DMA.high);//TBD
	
		pFIS->FIS_Type = SATA_FIS_TYPE_REG_H2D;
		pFIS->PM_Port = PMPort;
		/* pFIS->Device = 0x40; */
		pFIS->Control = (reset)?MV_BIT(2):0;
		
		pFIS->C = 0;

		MV_REG_WRITE_DWORD(portMmio, PORT_CMD_ISSUE, 1<<tag);
		MV_REG_READ_DWORD(portMmio, PORT_CMD_ISSUE);	/* flush */

		HBA_DelayMillisecond(pCore, 2);

		reset = reset ^ 1; /*SRST CLEAR*/

		count = 0;
		// make sure CI is cleared before moving on
		do {
			temp = MV_REG_READ_DWORD(portMmio, PORT_CMD_ISSUE) & (1<<tag);
			count++;
			HBA_DelayMillisecond(pCore, 10);
			//MV_PRINT(".");
		} while (temp != 0 && count < 100);
		MV_ERROR_PRINT("%s!\n", temp? "fail": "succeed");
	} while(reset==0);
	
	Tag_ReleaseOne(&pPort->Tag_Pool, tag);
	mvEnableIntr(portMmio, old_stat);
	if (temp != 0)
	{
		MV_ERROR_PRINT("\nSATA_DoSoftReset: soft reset issued, but CI is not cleared!\n");
		return MV_FALSE;
	} else
	{
		MV_ERROR_PRINT("\nSATA_DoSoftReset: soft reset issued, and finished!\n");
		return MV_TRUE;
	}
}

/*
 * only called by mvHandleDevicePlugin, in interrupt context.
 */
MV_BOOLEAN SATA_SoftResetDevice(PDomain_Port pPort, MV_U8 PMPort)
{
	PCore_Driver_Extension pCore = pPort->Core_Extension;
	MV_LPVOID portMmio = pPort->Mmio_Base;
	MV_U32 status1, status2, loop;

	if ( !(SATA_DoSoftReset(pPort, PMPort)) )
		return MV_FALSE;

	loop = 0;
	while (loop<5000)
	{
		status1 = MV_REG_READ_DWORD(portMmio, PORT_SCR_STAT) & 0xf;
		status2 = MV_REG_READ_DWORD(portMmio, PORT_TFDATA) & 0xff;
		if (((status1 & 0xf) == 0x3) && ((status2 & PORT_TF_STATUS_BSY) == 0))
		{
			MV_ERROR_PRINT("SATA_SoftResetDevice: loop = %d, link detected\n", loop);
			return MV_TRUE;
		}
		HBA_DelayMicrosecond(pCore, 1000); /* in us */
		loop++;
	}
	MV_ERROR_PRINT("SATA_SoftResetDevice: Did not detect device after soft reset\n");
	return MV_FALSE;
}

/*
 * only called by __core_req_timeout_handler.
 */
MV_BOOLEAN SATA_PortResetDevice(PDomain_Port pPort)
{
	PCore_Driver_Extension pCore = pPort->Core_Extension;
	MV_LPVOID portMmio = pPort->Mmio_Base;
	MV_U32 status1, status2;
	MV_U32 tmp, loop;
#if 1
	MV_ERROR_PRINT("SATA_PortResetDevice: Port Reset doesn't work, disabled.\n");
	return MV_FALSE;
#endif

	/* 
	 * clear start bit.
	 */
	tmp = MV_REG_READ_DWORD(portMmio, PORT_CMD);
	tmp &= ~(PORT_CMD_START);
	MV_REG_WRITE_DWORD(portMmio, PORT_CMD, tmp);

	/* 
	 * wait for at most 50ms*10=500ms for CR to clear.
	 */
	loop = 0;
	while (loop < 10) {
		tmp = MV_REG_READ_DWORD(portMmio, PORT_CMD);
		if (!(tmp & PORT_CMD_LIST_ON))
			break;
		else
			HBA_DelayMillisecond(pCore, 50);
		loop++;
	}

	/*
	 * if command is still hanging, do port reset
	 */
	if (loop >= 10)
		MV_ERROR_PRINT("SATA_PortResetDevice: CR not cleared\n");
	else
		MV_ERROR_PRINT("SATA_PortResetDevice: CR cleared\n");

	/* 
	 * reset DET
	 */
	tmp = MV_REG_READ_DWORD(portMmio, PORT_SCR_CTL);
	tmp &= ~0xF;
	tmp |= 0x1;
	MV_REG_WRITE_DWORD(portMmio, PORT_SCR_CTL, tmp);
	HBA_DelayMillisecond(pCore, 10);

	tmp &= ~0xF;
	MV_REG_WRITE_DWORD(portMmio, PORT_SCR_CTL, tmp);
	HBA_DelayMillisecond(pCore, 10);

	/*
	 * wait for PORT_SCR_STAT.DET == 1
	 */
	loop = 0;
	while (loop<5000)
	{
		status1 = MV_REG_READ_DWORD(portMmio, PORT_SCR_STAT) & 0xf;
		status2 = MV_REG_READ_DWORD(portMmio, PORT_TFDATA) & 0xff;
		if (((status1 & 0xf) == 0x3) && ((status2 & PORT_TF_STATUS_BSY) == 0))
		{
			/* clear status error */
			MV_REG_WRITE_DWORD(portMmio, PORT_SCR_ERR, 0xffffffff);
			/* 
			 * set start bit.
			 */
			tmp = MV_REG_READ_DWORD(portMmio, PORT_CMD);
			tmp |= (PORT_CMD_START);
			MV_REG_WRITE_DWORD(portMmio, PORT_CMD, tmp);

			MV_ERROR_PRINT("SATA_PortResetDevice: loop = %d, link detected, port reset finished\n", loop);
			return MV_TRUE;
		}
		HBA_DelayMicrosecond(pCore, 1000); /* in us */
		loop++;
	}

	/* 
	 * else set start bit, return error.
	 */
	MV_ERROR_PRINT("SATA_PortResetDevice: Did not detect device after port reset\n");
	tmp = MV_REG_READ_DWORD(portMmio, PORT_CMD);
	tmp |= (PORT_CMD_START);
	MV_REG_WRITE_DWORD(portMmio, PORT_CMD, tmp);

	return MV_FALSE;
}

void SATA_PortReportNoDevice (PCore_Driver_Extension pCore, PDomain_Port pPort)
{
	PDomain_Device pDevice;
	MV_U8 temp, i;
	MV_PVOID pUpperLayer = HBA_GetModuleExtension(pCore, MODULE_HBA);

	mvRemovePortWaitingList( pCore, pPort->Id );

	/* if PM - clear all the device attached to the port */
	if( pPort->Type == PORT_TYPE_PM )
		temp = MAX_DEVICE_PER_PORT-1;
	else
		temp = 0;
		
	for( i=0; i<=temp; i++ )
	{
		pDevice = &pPort->Device[i];

		if( pDevice->Status & DEVICE_STATUS_FUNCTIONAL )
		{
			if( pDevice->Internal_Req != NULL )
			{
				pCore->Total_Device_Count--;
				ReleaseInternalReqToPool( pCore, pDevice->Internal_Req );
				pDevice->Internal_Req = NULL;
			}

			HBA_ModuleNotification(pUpperLayer, EVENT_DEVICE_REMOVAL, pDevice->Id);
			pPort->Device_Number--;
		}
		
		pDevice->Status = DEVICE_STATUS_NO_DEVICE;
		pDevice->State = DEVICE_STATE_INIT_DONE;
	}
}

//#define FORCE_1_5_G
void SATA_PortReset(PDomain_Port pPort, MV_U32 atomic)
{
	PCore_Driver_Extension pCore = pPort->Core_Extension;

	PDomain_Device pDevice = &pPort->Device[0];
	MV_U32 signature;
	MV_U32 status;
#ifdef FORCE_1_5_G
	MV_U32 SControl, SStatus;
#endif
	MV_LPVOID portMmio = pPort->Mmio_Base;
	//MV_U32 numPMPorts = 0;
	MV_U32 i;

	/* No running commands at this moment */
	MV_ASSERT( pPort->Running_Slot==0 );
	MV_ASSERT( pPort->Port_State==PORT_STATE_IDLE );

	pPort->Device_Number = 0;

	galois_sata_event(GALOIS_EVENT_L2H_DRIVEINIT | (GALOIS_EVENT_FROM_PORT(pPort->Id)), pPort, pDevice);
	MV_PRINT("\n[Galois]Enter SATA_PortReset(portMmio=0x%08x) (port=%d)\n", (MV_U32)portMmio, pPort->Id);
	/* If we already reached the max number of devices supported,
	   disregard the rest */
	if( pCore->Total_Device_Count >= MAX_DEVICE_SUPPORTED )
	{
		for( i=0; i<MAX_DEVICE_PER_PORT; i++ )
		{
			pPort->Device[i].State = DEVICE_STATE_INIT_DONE;
			pPort->Device[i].Status = DEVICE_STATUS_NO_DEVICE;
		}
		MV_DPRINT(("We have too many devices %d.", pCore->Total_Device_Count));
		return;
	}

#ifdef FORCE_1_5_G
	/* It'll trigger OOB. Looks like PATA hardware reset. 
	 * Downgrade 3G to 1.5G
	 * If Port Multiplier is attached, only the PM is downgraded. */
	{
		MV_ERROR_PRINT("[Galois]Force SATA port %d to 1.5G\n", pPort->Id);
		//TBD: Not tested
		SStatus = MV_REG_READ_DWORD(portMmio, PORT_SCR_STAT);
		MV_ERROR_PRINT("[Galois]SATA SCR_STAT=0x%08x\n", SStatus);
		if ( (SStatus&0xF0)==0x20 )
		{
			/* 3G */
			SControl = MV_REG_READ_DWORD(portMmio, PORT_SCR_CTL);
			SControl &= ~0x000000FF;
			SControl |= 0x11;
			MV_REG_WRITE_DWORD(portMmio, PORT_SCR_CTL, SControl);
			if (atomic == GALOIS_SATA_ATOMIC)
				HBA_DelayMillisecond(pCore, 500);
			else
				HBA_SleepMillisecond(pCore, 500);
			SControl &= ~0x000000FF;
			SControl |= 0x10;
			MV_REG_WRITE_DWORD(portMmio, PORT_SCR_CTL, SControl);
			if (atomic == GALOIS_SATA_ATOMIC)
				HBA_DelayMillisecond(pCore, 500);
			else
				HBA_SleepMillisecond(pCore, 500);
		}
		SStatus = MV_REG_READ_DWORD(portMmio, PORT_SCR_STAT);
		MV_ERROR_PRINT("[Galois]SATA SCR_STAT=0x%08x\n", SStatus);
	}
#endif

	if( !SATA_PortDeviceDetected(pPort) ) /* Check SCR_STAT[DET] */
	{
		MV_PRINT("[Galois]There's no device detected, according to SCR_STAT[DET].\n");
#if defined(SUPPORT_ERROR_HANDLING)
		if( pPort->Setting & PORT_SETTING_PM_FUNCTIONAL )
		{
			pPort->Setting &= ~PORT_SETTING_PM_FUNCTIONAL;
			pPort->Setting &= ~PORT_SETTING_PM_EXISTING;
			MV_DPRINT(("PM on port %d is gone\n", pPort->Id));
			SATA_PortReportNoDevice( pCore, pPort );
		}
		else if( pDevice->Status & DEVICE_STATUS_FUNCTIONAL ) 
		{
			MV_DPRINT(("device on port %d is gone\n", pPort->Id));
			SATA_PortReportNoDevice( pCore, pPort );
		}
#endif

		// fixed: have to set each device individually - or hot plug will have problem
		for (i=0; i<MAX_DEVICE_PER_PORT; i++)
		{	
			pDevice = &pPort->Device[i];
			pDevice->State = DEVICE_STATE_INIT_DONE;
		}
				
		mvDeviceStateMachine(pCore, pDevice, atomic);
		return;
	}

	if( !SATA_PortDeviceReady(pPort) ) /* Check SCR_STAT[IPM] */
	{	
		MV_PRINT("[Galois]There's no device ready, according to SCR_STAT[IPM].\n");
#if defined(SUPPORT_ERROR_HANDLING)
		if( pPort->Setting & PORT_SETTING_PM_FUNCTIONAL ) 
		{
			pPort->Setting &= ~PORT_SETTING_PM_FUNCTIONAL;
			MV_DPRINT(("PM on port %d is non-functional\n", pPort->Id));
			SATA_PortReportNoDevice( pCore, pPort );
		}
		else if( pDevice->Status & DEVICE_STATUS_FUNCTIONAL )
		{
			MV_DPRINT(("device on port %d is non-functional\n", pPort->Id));
			SATA_PortReportNoDevice( pCore, pPort );
			pDevice->Status = DEVICE_STATUS_EXISTING;		
		}
#endif
		for (i=0; i<MAX_DEVICE_PER_PORT; i++)
		{	
			pDevice = &pPort->Device[i];
			pDevice->State = DEVICE_STATE_INIT_DONE;
		}
				
		mvDeviceStateMachine(pCore, pDevice, atomic);
		return;
	}

	/* polling for the status register, wait for device ready */
	i = 0;
	while (i < HBARESET_STATUS_POLL_COUNT) {
		status = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_TFDATA);
		status &= 0xFF;
		if ((!(status & PORT_TF_STATUS_BSY)) 
				/*&& (status & PORT_TF_STATUS_DRDY)*/ 
				&& (status != 0x7F)) {
			break;
		} else {
			if (atomic == GALOIS_SATA_ATOMIC)
				HBA_DelayMillisecond(pCore, 100);
			else
				HBA_SleepMillisecond(pCore, 100);
		}
		i++;
	}
	if (i >= HBARESET_STATUS_POLL_COUNT)
		MV_ERROR_PRINT("SATA_PortReset(%d): device status not ready in %dms(0x%02x)\n", pPort->Id, i*100, status);
	else
		MV_ERROR_PRINT("SATA_PortReset(%d): device status ready in %dms(0x%02x)\n", pPort->Id, i*100, status);

	/* Read signature of the device */
	signature = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_SIG);
	MV_PRINT("[Galois]SATA device signature is 0x%08x: ", signature);
	if (signature==0xEB140101) {/* ATAPI signature */
		pDevice->Device_Type |= DEVICE_TYPE_ATAPI;
		MV_PRINT("ATAPI device.\n");
	} else if (signature==0x00000101) {/* ATA signature */
		MV_PRINT("ATA device.\n");
	} else {
#if 1
		MV_ERROR_PRINT("No signature(port=%d), do soft reset and read again.\n", pPort->Id);
		SATA_SoftResetDevice(pPort, 0x0);
		HBA_DelayMillisecond(pCore, 500);
#else
		MV_ERROR_PRINT("No signature(port=%d), wait for 2 seconds and read again.\n", pPort->Id);

		if (atomic == GALOIS_SATA_ATOMIC)
			HBA_DelayMillisecond(pCore, 2000);
		else
			HBA_SleepMillisecond(pCore, 2000);
#endif
		signature = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_SIG);
		MV_ERROR_PRINT("[Galois]2nd read SATA device signature(port=%d) is 0x%08x: ", pPort->Id, signature);

		/* reset it */
		if ((signature != 0xEB140101) && (signature != 0x00000101)) {
			unsigned int tmp;
			MV_LPVOID mmio = pCore->Mmio_Base;
			MV_ERROR_PRINT("do HBA reset again(port=%d).\n", pPort->Id);
			/* Reset controller */
			tmp = MV_REG_READ_DWORD(mmio, HOST_CTL);
			if ((tmp & HOST_RESET) == 0) {
				MV_REG_WRITE_DWORD(mmio, HOST_CTL, tmp|HOST_RESET);
				MV_REG_READ_DWORD(mmio, HOST_CTL); /* flush */
			}
			HBA_DelayMillisecond(pCore, 2000);

			tmp = MV_REG_READ_DWORD(mmio, HOST_CTL);
			if (tmp & HOST_RESET) {
				MV_ERROR_PRINT("HBA reset fails: drive is dead, power off!\n");
			} else {
				signature = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_SIG);
				MV_ERROR_PRINT("[Galois]3rd read SATA device signature is 0x%08x: \n", signature);
				SATA_InitHost(pCore, GALOIS_SATA_ATOMIC);
				HBA_DelayMillisecond(pCore, 2000);
				signature = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_SIG);
				MV_ERROR_PRINT("[Galois]4th read SATA device signature is 0x%08x: \n", signature);
			}
		}

		if (signature==0xEB140101) {/* ATAPI signature */
			pDevice->Device_Type |= DEVICE_TYPE_ATAPI;
			MV_ERROR_PRINT("ATAPI device.\n");
		} else if (signature==0x00000101) {/* ATA signature */
			MV_ERROR_PRINT("ATA device.\n");
		} else {
			MV_ERROR_PRINT("No device(port=%d) or broken device(fatal error, please power off).\n", pPort->Id);
			MV_DumpHBA(pCore, pPort);
			galois_sata_event(GALOIS_EVENT_L2H_NOSIG | (GALOIS_EVENT_FROM_PORT(pPort->Id)), pPort, pDevice);
			if (pDevice->Status & DEVICE_STATUS_FUNCTIONAL)
				SATA_PortReportNoDevice(pCore, pPort);
		}
	}

	/* Device is ready */
	pDevice->Internal_Req = GetInternalReqFromPool(pCore);
	if(pDevice->Internal_Req == NULL) {
		MV_DPRINT(("ERROR: Unable to get an internal request buffer\n"));
		pDevice->Status = DEVICE_STATUS_NO_DEVICE;
		pDevice->State = DEVICE_STATE_INIT_DONE;
	} else if ((signature != 0xEB140101) && (signature != 0x00000101)) {
		pDevice->Status = DEVICE_STATUS_NO_DEVICE;
		pDevice->State = DEVICE_STATE_INIT_DONE;
	} else {
		pDevice->Status = DEVICE_STATUS_EXISTING|DEVICE_STATUS_FUNCTIONAL;
		pDevice->State = DEVICE_STATE_RESET_DONE;
		pPort->Device_Number = 1;	/* We have one device here. */
	}

	/* set the rest of the devices on this port */
	for ( i=1; i<MAX_DEVICE_PER_PORT; i++ )
	{
		pDevice = &pPort->Device[i];
		pDevice->Status = DEVICE_STATUS_NO_DEVICE;
		pDevice->State = DEVICE_STATE_INIT_DONE;
	}

	/* 
	 * Initialize the device connected to port.
	 * pDevice->State = DEVICE_STATE_RESET_DONE for Device[0]
	 */
	MV_ERROR_PRINT("SATA_PortReset: start port(0x%p) %d mvDeviceStateMachine\n", pPort, pPort->Id);
	mvDeviceStateMachine(pCore, &pPort->Device[0], atomic);
}

/*
 * Called by mvAdapterStateMachine first.
 * then by mvDeviceStateMachine.
 */
static MV_BOOLEAN mvChannelStateMachine(PCore_Driver_Extension pCore, PDomain_Port pPort, MV_U32 atomic)
{
	MV_U8 i;
	MV_U8 portState;
	PDomain_Device pDevice;
	PDomain_Port pOrgPort = pPort;
	MV_PVOID pUpperLayer = HBA_GetModuleExtension(pCore, MODULE_HBA);

	if ( pPort==NULL )
		portState = PORT_STATE_IDLE; /* From mvAdapterStateMachine */
	else {
		portState = pPort->Port_State; /* From mvDeviceStateMachine */
	}

	//Each step: if fail like no device, should go to the end.
	/* Channel state machine */
	switch ( portState )
	{
		case PORT_STATE_IDLE:
			MV_PRINT("mvChannelStateMachine PORT_STATE_IDLE.\n");
			/* To do reset */
			for( i=0; i<pCore->Port_Num; i++ )
			{
				pPort = &pCore->Ports[i];
				MV_DASSERT( pPort->Port_State==PORT_STATE_IDLE );
				SATA_PortReset(pPort, atomic); /* call mvDeviceStateMachine. */
				//MV_PRINT("[Galois]wait 500ms.\n");
				//HBA_SleepMillisecond(pCore, 500);
			}
			break;

		/* 
		 * Each port will call mvDeviceStateMachine for its devices. 
		 * When all the devices for that port are done, will call mvChannelStateMachine.
		 */
		case PORT_STATE_INIT_DONE:

			MV_PRINT("mvChannelStateMachine PORT_STATE_INIT_DONE.\n\n");
			galois_sata_event(GALOIS_EVENT_L2H_DRIVEDONE| (GALOIS_EVENT_FROM_PORT(pOrgPort->Id)), pOrgPort, &pOrgPort->Device[0]);
			/* Check whether all the ports are done. */
			for ( i=0; i<pCore->Port_Num; i++ )
			{
				pPort = &pCore->Ports[i];
				if ( pPort->Port_State!=PORT_STATE_INIT_DONE )
					return MV_TRUE;
			}

			/* Discovery procedure is finished. */
			if ( pCore->State==CORE_STATE_IDLE ) /* set in Core_ModuleInitialize */
			{
				pCore->State = CORE_STATE_STARTED;
				HBA_ModuleStarted(pCore);	/* The first time initialization */
				/* it will call Core's start, i.e. Core_ModuleStart=>... */
			}
			else
			{
				/* check which device on this port needs to be reported */
				for (i=0; i<MAX_DEVICE_PER_PORT; i++)
				{
					pDevice = &pOrgPort->Device[i];
					if ( pDevice->Need_Notify )
					{
						/* notify linux kernel that a new device added */
						HBA_ModuleNotification(pUpperLayer, 
								EVENT_DEVICE_ARRIVAL, pDevice->Id);
						galois_sata_event(GALOIS_EVENT_L2H_DRIVEDONE| (GALOIS_EVENT_FROM_PORT(pOrgPort->Id)), pOrgPort, &pOrgPort->Device[0]);
						pDevice->Need_Notify = MV_FALSE;	
					}
				}
			}
			break;
	}

	return MV_TRUE;
}

/* 
 * Global host controller reset 
 */
MV_BOOLEAN SATA_ResetHost(PCore_Driver_Extension pCore, MV_U32 atomic)
{
	PDomain_Port pPort;
#if 0
	MV_U32 signature;
#endif
	int need_host_reset;

	MV_LPVOID mmio = pCore->Mmio_Base;
	MV_U32 tmp;
	MV_BOOLEAN ret = MV_TRUE;
	MV_U32 i=0;

	if (early_host_reset && !lowlevel_probe_done)
		need_host_reset = 0;
	else {
		need_host_reset = 1;
#if 0
		for( i=0; i<pCore->Port_Num; i++ )
		{
			pPort = &pCore->Ports[i];
			signature = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_SIG);
			if ((signature == 0xEB140101) || (signature == 0x00000101)) 
			{
				need_host_reset = 0;
				break;
			}
		}
#endif
	}

	/*
	 * lowlevel_probe_done==0 means it's the first time powerup
	 * i.e. we don't need to reset host
	 */
	if (need_host_reset) {
		MV_ERROR_PRINT("SATA_ResetHost: do HBA reset.\n");
		/* Reset controller */
		tmp = MV_REG_READ_DWORD(mmio, HOST_CTL);
		if ((tmp & HOST_RESET) == 0) {
			if(pCore->VS_Reg_Saved != VS_REG_SIG)
			{
				for ( i=0; i<pCore->SATA_Port_Num; i++ )
				{
					Domain_Port *port;
					port = &pCore->Ports[i];
					MV_REG_WRITE_DWORD(port->Mmio_Base, PORT_VSR_ADDR, 0xc);
					port->VS_RegC= MV_REG_READ_DWORD(port->Mmio_Base, PORT_VSR_DATA);
					pCore->VS_Reg_Saved=VS_REG_SIG;
				}
			}
			MV_REG_WRITE_DWORD(mmio, HOST_CTL, tmp|HOST_RESET);
			MV_REG_READ_DWORD(mmio, HOST_CTL); /* flush */
		}

		/* 
		 * Reset must complete within 1 second, 
		 * or the hardware should be considered fried.
		 */
		MV_ERROR_PRINT("SATA_ResetHost: Resetting HBA, wait 1s.\n");
		if (atomic == GALOIS_SATA_ATOMIC)
			HBA_DelayMillisecond(pCore, 1000);
		else
			HBA_SleepMillisecond(pCore, 1000);
	}

	MV_PRINT("SATA_ResetHost: sleep at most +1s again to wait for reset finished...");
	i = 0;
	while (i < 1000) {
		tmp = MV_REG_READ_DWORD(mmio, HOST_CTL);
		if (!(tmp & HOST_RESET)) /* if HR is cleared, passed */
			break;

		if (atomic == GALOIS_SATA_ATOMIC)
			HBA_DelayMillisecond(pCore, 10);
		else
			HBA_SleepMillisecond(pCore, 10);

		i+=10;
	}
	MV_PRINT(" %d ms passed.\n", i);
	if (i >= 1000){
		MV_ERROR_PRINT("Berlin SATA reset fails.\n\n");
		MV_ASSERT(MV_FALSE);	//TBD;
		ret = MV_FALSE;
	}

	/*
	 * Now reset finished
	 */
	if(pCore->VS_Reg_Saved == VS_REG_SIG)
	{
		for ( i=0; i<pCore->SATA_Port_Num; i++ )
		{
			Domain_Port *port;
			port = &pCore->Ports[i];
			MV_REG_WRITE_DWORD(port->Mmio_Base, PORT_VSR_ADDR, 0xc);
			MV_REG_WRITE_DWORD(port->Mmio_Base, PORT_VSR_DATA, port->VS_RegC);
		}
	}
	/* link error work around */
	for ( i=0; i<pCore->SATA_Port_Num; i++ )
	{
		MV_U32 tmp, old_stat;
		Domain_Port *port;
		port = &pCore->Ports[i];

		mvDisableIntr( port->Mmio_Base, old_stat );
		MV_REG_WRITE_DWORD( port->Mmio_Base, PORT_VSR_ADDR, 0x5 );
		tmp = MV_REG_READ_DWORD( port->Mmio_Base, PORT_VSR_DATA );
		MV_REG_WRITE_DWORD( port->Mmio_Base, PORT_VSR_DATA, tmp | MV_BIT(26));

		if (atomic == GALOIS_SATA_ATOMIC)
			HBA_DelayMillisecond(pCore, 10);
		else
			HBA_SleepMillisecond(pCore, 10);

		mvEnableIntr( port->Mmio_Base, old_stat );
	}

	return ret;
}

/*
 * Initilize setting of sata port
 */
static void SATA_InitPort(PCore_Driver_Extension pCore, MV_U8 portId, MV_U32 atomic)
{
	PDomain_Port pPort = &pCore->Ports[portId];
	MV_LPVOID mmio = pCore->Mmio_Base;
	MV_LPVOID portMmio = pPort->Mmio_Base;
	MV_U32 tmp, j;

	/* 
	 * Make sure port is not active. If yes, stop the port.
	 */
	tmp = MV_REG_READ_DWORD(portMmio, PORT_CMD);
	/* For ACHI, four bits are avaiable. For 614x, PORT_CMD_FIS_ON is reserved. */
	if (tmp & (PORT_CMD_LIST_ON | PORT_CMD_FIS_ON | PORT_CMD_FIS_RX | PORT_CMD_START)) {
		tmp &= ~(PORT_CMD_LIST_ON | PORT_CMD_FIS_ON | PORT_CMD_FIS_RX | PORT_CMD_START);
		MV_REG_WRITE_DWORD(portMmio, PORT_CMD, tmp);
		MV_REG_READ_DWORD(portMmio, PORT_CMD); /* flush */

		/* 
		 * spec says 500 msecs for each bit, so this is slightly incorrect.
		 */
		MV_ERROR_PRINT("[Galois]SATA_InitPort: sleep 500 ms again to wait for cmd stop.\n");
		if (atomic == GALOIS_SATA_ATOMIC)
			HBA_DelayMillisecond(pCore, 500);
		else
			HBA_SleepMillisecond(pCore, 500);
	}

	//TBD: PORT_CMD enable bit(5): PIO command will issue PIO setup interrupt bit. 
	// Only after clear the PIO setup interrupt bit, the hardware will issue the PIO done interrupt bit.
	//TBD: Maybe in this case, we needn't enable PIO setup interrupt bit but for some others we should.

	#ifdef AHCI
	/* For 614x, it's reserved. */
	MV_REG_WRITE_DWORD(portMmio, PORT_CMD, PORT_CMD_SPIN_UP);
	#endif

#if 1
	/* Wait for SATA DET(Device Detection) */
	MV_PRINT("[Galois]SATA_InitPort Waiting/Polling SCR status[port=%d]\n", portId);
	j = 0;
	while (j < 1) { /* old value is 100 */
		tmp = MV_REG_READ_DWORD(portMmio, PORT_SCR_STAT);
		if ((tmp & 0xf) == 0x3) {
			break;
		}
		if (atomic == GALOIS_SATA_ATOMIC)
			HBA_DelayMillisecond(pCore, 10);
		else
			HBA_SleepMillisecond(pCore, 10);
		j++;
	}
	MV_PRINT("passed %dms(j=%d)\n", 10*j, j);
#endif
	
	/* Clear SATA error */
	tmp = MV_REG_READ_DWORD(portMmio, PORT_SCR_ERR);
	MV_REG_WRITE_DWORD(portMmio, PORT_SCR_ERR, tmp);

	/* Ack any pending irq events for this port */
	tmp = MV_REG_READ_DWORD(portMmio, PORT_IRQ_STAT);
	if (tmp)
		MV_REG_WRITE_DWORD(portMmio, PORT_IRQ_STAT, tmp);

	/* Ack pending irq in the host interrupt status register */
	MV_REG_WRITE_DWORD(mmio, HOST_IRQ_STAT, 1 << portId);

	/* set irq mask (enables interrupts) */
	MV_REG_WRITE_DWORD(portMmio, PORT_IRQ_MASK, DEF_PORT_IRQ);

	/* FIFO controller workaround for 6121-B0B1, 6111-B0B1, and 6145-A0
		tmp = (MV_REG_READ_DWORD( portMmio, PORT_FIFO_CTL ) & 0xFFFFF0FF ) | 0x500;
		MV_REG_WRITE_DWORD( portMmio, PORT_FIFO_CTL, tmp);
		MV_REG_READ_DWORD( portMmio, PORT_FIFO_CTL); flush
	 */
}

/*
 * It's equivalent to ahci_host_init and ahci_port_start
 */
void SATA_InitHost(PCore_Driver_Extension pCore, MV_U32 atomic)
{
	MV_LPVOID mmio = pCore->Mmio_Base;
	MV_U8 i;
	PDomain_Port pPort;
	MV_U32 tmp;
	
	pCore->Capacity = MV_REG_READ_DWORD(mmio, HOST_CAP);
	
	/* 
	 * For 614x, enable enhanced mode for PATA and interrupt. 
	 * For AHCI, enable AHCI.
	 */
	tmp = MV_REG_READ_DWORD(mmio, HOST_CTL);
	MV_REG_WRITE_DWORD(mmio, HOST_CTL, (MV_U32)(tmp | HOST_IRQ_EN | HOST_MVL_EN));
	tmp = MV_REG_READ_DWORD(mmio, HOST_CTL);

	/* Ports implemented: enable ports */
	pCore->Port_Map = MV_REG_READ_DWORD(mmio, HOST_PORTS_IMPL);
	tmp = MV_REG_READ_DWORD(mmio, HOST_CAP);

	/* Initialize ports */
	for ( i = 0; i<pCore->Port_Num; i++) {
		pPort = &pCore->Ports[i];
		/* make sure port is not active */
		SATA_InitPort(pCore, i, atomic);
	}

	/* Initialize port, set uncached memory pointer. */
	for ( i = 0; i<pCore->Port_Num; i++) {
		pPort = &pCore->Ports[i];

		/* Set the sata port register */
		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_LST_ADDR_HI, pPort->Cmd_List_DMA.high);
		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_LST_ADDR, pPort->Cmd_List_DMA.low);
		MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_LST_ADDR);

		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_FIS_ADDR_HI, pPort->RX_FIS_DMA.high);
		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_FIS_ADDR, pPort->RX_FIS_DMA.low);
		MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_FIS_ADDR);

		/* AHCI is different with Thor */
		#ifdef AHCI
		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_CMD, 
				PORT_CMD_ICC_ACTIVE | PORT_CMD_FIS_RX |	
				PORT_CMD_POWER_ON | PORT_CMD_SPIN_UP | PORT_CMD_START );
		#else
		/* 
		 * Workaround: Don't enable PORT_CMD_FIS_RX otherwise system will hang.
		 * Marvell sata reserves Cmd list override, power on device, spinup device.
		 * MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_CMD, PORT_CMD_START );
		 */
		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_CMD, PORT_CMD_RRDY_EN | PORT_CMD_STM_EN | PORT_CMD_FIS_RX | PORT_CMD_START );
		#endif
	}

	MV_DUMPC32(0xCCCCFF01);
}

/*
 * Called by Core_ModuleStart()
 */
MV_BOOLEAN mvAdapterStateMachine(IN OUT MV_PVOID This, MV_U32 atomic)
{
	PCore_Driver_Extension pCore = (PCore_Driver_Extension)This;

	switch (pCore->Adapter_State)
	{
		case ADAPTER_INITIALIZING:
			MV_PRINT("mvAdapterStateMachine ADAPTER_INITIALIZING.\n");
			if (SATA_ResetHost(pCore, atomic) == MV_FALSE) 
				return MV_FALSE;

			SATA_InitHost(pCore, atomic);
			pCore->Adapter_State = ADAPTER_READY;

#if 0
			MV_PRINT("[Galois]mvAdapterStateMachine sleeps 1 second.\n");
			HBA_RequestTimer(pCore, 1000, (MV_VOID(*)(MV_PVOID))mvAdapterStateMachine);	
#else
			mvAdapterStateMachine(pCore, atomic);
#endif
			break;

		case ADAPTER_READY:
			MV_PRINT("mvAdapterStateMachine ADAPTER_READY.\n");
			mvChannelStateMachine(pCore, NULL, atomic);
			break;

		default:
			break;
	}
	return MV_TRUE;
}

/*
 * state machine of device connected to port
 */
MV_BOOLEAN mvDeviceStateMachine(PCore_Driver_Extension pCore, PDomain_Device pDevice, MV_U32 atomic)
{
	MV_U8 i;
	PDomain_Port pPort = pDevice->PPort;

	MV_PRINT("\n[Galois]Enter mvDeviceStateMachine.\n");

	switch ( pDevice->State )
	{
		case DEVICE_STATE_RESET_DONE:
			/* restart unit retry */
			pPort->port_unit_ready_retry = 15;
			MV_PRINT("Device %d DEVICE_STATE_RESET_DONE.\n", pDevice->Id);
#ifdef BERLIN_SUPPORT_SPRUCE_DRIVE
			Device_PowerStandby(pDevice->PPort, pDevice);
			break;

		case DEVICE_STATE_POWERSTANDBY_DONE:
			MV_PRINT("Device %d DEVICE_STATE_POWERSTANDBY_DONE.\n", pDevice->Id);
			Device_StartUnit(pDevice->PPort, pDevice);
			break;

		case DEVICE_STATE_START_DONE:
			MV_PRINT("Device %d DEVICE_STATE_START_DONE.\n", pDevice->Id);
#endif
			/* To do identify */
			Device_IssueIdentify(pDevice->PPort, pDevice); 
			break;

		case DEVICE_STATE_IDENTIFY_DONE:
			MV_PRINT("Device %d DEVICE_STATE_IDENTIFY_DONE.\n", pDevice->Id);
			/* To do set PIO mode */
			Device_IssueSetPIOMode(pDevice->PPort, pDevice);
			break;

		case DEVICE_STATE_SET_PIO_DONE:
			MV_PRINT("Device %d DEVICE_STATE_SET_PIO_DONE.\n", pDevice->Id);
			/* To do set UDMA mode */
			Device_IssueSetUDMAMode(pDevice->PPort, pDevice);
			break;

		case DEVICE_STATE_SET_UDMA_DONE:
			MV_PRINT("Device %d DEVICE_STATE_SET_UDMA_DONE.\n", pDevice->Id);
#ifdef BERLIN_SPRUCE_INTEGRATION
			/* download Spruce firmware */
			Device_SpruceDownloadCode(pDevice->PPort, pDevice);
			break;

		case DEVICE_STATE_SPRUCE_DOWNLOAD_DONE:
			MV_PRINT("Device %d DEVICE_STATE_SPRUCE_DOWNLOAD_DONE.\n", pDevice->Id);

			/* execute Spruce firmware */
			Device_SpruceExecuteCode(pDevice->PPort, pDevice);
			break;

		case DEVICE_STATE_SPRUCE_EXECUTE_DONE:
			MV_PRINT("Device %d DEVICE_STATE_SPRUCE_EXECUTE_DONE.\n", pDevice->Id);
#endif
#ifdef BERLIN_ATAPI_GET_READY
			/* only for ATAPI device to get ready */
			Device_TestUnitReady(pDevice->PPort, pDevice);
			break;

		case DEVICE_STATE_TESTUNITREADY_DONE:
			MV_PRINT("Device %d DEVICE_STATE_TESTUNITREADY_DONE.\n", pDevice->Id);
			/* only for ATAPI device to get ready */
			Device_MakeRequestSense(pDevice->PPort, pDevice);
			break;

		case DEVICE_STATE_REQUESTSENSE_DONE:
			MV_PRINT("Device %d DEVICE_STATE_REQUESTSENSE_DONE.\n", pDevice->Id);
#endif
			/* To do enable write cache */
			Device_EnableWriteCache(pDevice->PPort, pDevice);
			break;

		case DEVICE_STATE_ENABLE_WRITE_CACHE_DONE:
			MV_PRINT("Device %d DEVICE_STATE_ENABLE_WRITE_CACHE_DONE.\n", pDevice->Id);
			/* To do enable read ahead */
			Device_EnableReadAhead( pDevice->PPort, pDevice );
			break;

		case DEVICE_STATE_ENABLE_READ_AHEAD_DONE:
			MV_PRINT("Device %d DEVICE_STATE_ENABLE_READ_AHEAD_DONE.\n", pDevice->Id);

			/* Initialization procedure is done. */
			pDevice->State = DEVICE_STATE_INIT_DONE;
			pCore->Total_Device_Count++;
		/* No break here. */

		case DEVICE_STATE_INIT_DONE:
			MV_PRINT("Device %d DEVICE_STATE_INIT_DONE.\n", pDevice->Id);

			/* Check whether all devices attached to this port are done. */
			for ( i=0; i<MAX_DEVICE_PER_PORT; i++ )
			{
				if ( pPort->Device[i].State!=DEVICE_STATE_INIT_DONE )
					return MV_TRUE;
			}
			pPort->Port_State = PORT_STATE_INIT_DONE;
			mvChannelStateMachine(pCore, pDevice->PPort, atomic);
			break;

		default:
			break;
	}

	return MV_TRUE;
}

void Device_ParseIdentifyData(
	IN PDomain_Device pDevice,
	IN PATA_Identify_Data pATAIdentify
	);

static void Core_InternalReqCallback(
	 IN PCore_Driver_Extension pCore,
	 IN PMV_Request pReq
	 )
{
	PDomain_Port pPort;
	PDomain_Device pDevice;
	PATA_Identify_Data pATAIdentify;
	MV_U8 portId, deviceId;

	portId = PATA_MapPortId(pReq->Device_Id);
	deviceId = PATA_MapDeviceId(pReq->Device_Id);

	pPort = &pCore->Ports[portId];
	pDevice = &pPort->Device[deviceId];

	if( pReq->Scsi_Status != REQ_STATUS_SUCCESS )
		MV_PRINT("Core_InternalReqCallback(port=%d): Scsi_Status failed\n", portId);

	//It's possible that CDB_CORE_READ_LOG_EXT returns error and come here
	//because we send CDB_CORE_READ_LOG_EXT no matter NCQ is running or not.
	if ((pReq->Cdb[2]!=CDB_CORE_READ_LOG_EXT)
			&& (pReq->Cdb[0]!=SCSI_CMD_MARVELL_SPECIFIC)
#ifdef BERLIN_SUPPORT_SPRUCE_DRIVE
			&& (pReq->Cdb[0]!=SCSI_CMD_START_STOP_UNIT) 
#endif
#ifdef BERLIN_ATAPI_GET_READY
			&& (pReq->Cdb[0]!=SCSI_CMD_TEST_UNIT_READY) && (pReq->Cdb[0]!=SCSI_CMD_REQUEST_SENSE)
#endif
#ifdef BERLIN_SPRUCE_INTEGRATION
			&& (pReq->Cdb[0]!=SCSI_CMD_SPRUCE_DOWNLOAD_CODE)
#endif
			)
	{
		if( pReq->Scsi_Status != REQ_STATUS_SUCCESS )
		{
			MV_ERROR_PRINT("Core_InternalReqCallback(port=%d): return after cmd fails.\n", portId);
			/* request didn't finish correctly - we set device to existing
			   and finish state machine */
			pDevice->Status = DEVICE_STATUS_EXISTING;
			pDevice->State = DEVICE_STATE_INIT_DONE;
			mvDeviceStateMachine(pCore, pDevice, GALOIS_SATA_ATOMIC);
			return;
		}
	}

	pATAIdentify = (PATA_Identify_Data)pPort->Device[deviceId].Scratch_Buffer;

	/* Handle internal request like identify */
	//MV_DASSERT( pReq->Cdb[0]==SCSI_CMD_MARVELL_SPECIFIC );
	//MV_DASSERT( pReq->Cdb[1]==CDB_CORE_MODULE );
	MV_ASSERT( portId < MAX_PORT_NUMBER );
#ifdef BERLIN_SUPPORT_SPRUCE_DRIVE
	if ((pReq->Cdb[0]==SCSI_CMD_START_STOP_UNIT) && (pReq->Cdb[4]==0x30)) /* standby */
	{
		pDevice->State = DEVICE_STATE_POWERSTANDBY_DONE;
		mvDeviceStateMachine(pCore, pDevice, GALOIS_SATA_ATOMIC);
		return;
	}
	else if ((pReq->Cdb[0]==SCSI_CMD_START_STOP_UNIT) && (pReq->Cdb[4]==0x1)) /* start */
	{
		pDevice->State = DEVICE_STATE_START_DONE;
		mvDeviceStateMachine(pCore, pDevice, GALOIS_SATA_ATOMIC);
		return;
	}
	else
#endif
	if ( pReq->Cdb[2]==CDB_CORE_IDENTIFY )
	{
		hba_swap_buf_le16((MV_PU16) pATAIdentify, 
				  sizeof(ATA_Identify_Data)/sizeof(MV_U16));
		Device_ParseIdentifyData(pDevice, pATAIdentify);

		//MV_ASSERT( pDevice->State == DEVICE_STATE_RESET_DONE );
		pDevice->State = DEVICE_STATE_IDENTIFY_DONE;
		mvDeviceStateMachine(pCore, pDevice, GALOIS_SATA_ATOMIC);
		return;
	}
	else if ( pReq->Cdb[2]==CDB_CORE_SET_PIO_MODE )
	{
		pDevice->State = DEVICE_STATE_SET_PIO_DONE;
		mvDeviceStateMachine(pCore, pDevice, GALOIS_SATA_ATOMIC);
	}
	else if ( pReq->Cdb[2]==CDB_CORE_SET_UDMA_MODE )
	{
		pDevice->State = DEVICE_STATE_SET_UDMA_DONE;
		mvDeviceStateMachine(pCore, pDevice, GALOIS_SATA_ATOMIC);
	} else
#ifdef BERLIN_SPRUCE_INTEGRATION
	if (pReq->Cdb[0]==SCSI_CMD_SPRUCE_DOWNLOAD_CODE && pReq->Cdb[1]==0x1)
	{
		pDevice->State = DEVICE_STATE_SPRUCE_DOWNLOAD_DONE;
		mvDeviceStateMachine(pCore, pDevice, GALOIS_SATA_ATOMIC);
	} else if (pReq->Cdb[0]==SCSI_CMD_SPRUCE_DOWNLOAD_CODE && pReq->Cdb[1]==0x0)
	{
		pDevice->State = DEVICE_STATE_SPRUCE_EXECUTE_DONE;
		mvDeviceStateMachine(pCore, pDevice, GALOIS_SATA_ATOMIC);
	} else
#endif
#ifdef BERLIN_ATAPI_GET_READY
	if (pReq->Cdb[0]==SCSI_CMD_TEST_UNIT_READY) 
	{
		pDevice->State = DEVICE_STATE_TESTUNITREADY_DONE;
		mvDeviceStateMachine(pCore, pDevice, GALOIS_SATA_ATOMIC);
	}
	else if (pReq->Cdb[0]==SCSI_CMD_REQUEST_SENSE)
	{
		PSENSE_DATA senseBuffer = (PSENSE_DATA)pPort->Device[deviceId].Scratch_Buffer;
		if((pDevice->Device_Type & DEVICE_TYPE_ATAPI))
			MV_ERROR_PRINT("MV Sense(port=%d, try=%d, enable=%d): Sense Key(0x%x):%s, ASC:%x ASCQ:%x\n", 
				portId, pPort->port_unit_ready_retry, enable_disc_ready,
				senseBuffer->SenseKey, MV_DumpSenseKey(senseBuffer->SenseKey),
				senseBuffer->AdditionalSenseCode, 
				senseBuffer->AdditionalSenseCodeQualifier);
		if ((pDevice->Device_Type & DEVICE_TYPE_ATAPI) && (pPort->port_unit_ready_retry > 0) && (enable_disc_ready)
				&& (((senseBuffer->SenseKey == 0x2) && (senseBuffer->AdditionalSenseCode == 0x4))       /* media not ready */
					|| ((senseBuffer->SenseKey == 0x6) && (senseBuffer->AdditionalSenseCode == 0x28))   /* media changed */
					|| ((senseBuffer->SenseKey == 0x6) && (senseBuffer->AdditionalSenseCode == 0x29)))) /* during reset */
		{
			pPort->port_unit_ready_retry--;
			if ((senseBuffer->SenseKey == 0x2) && (senseBuffer->AdditionalSenseCode == 0x4) && (pPort->port_unit_ready_retry <= 12))
				HBA_DelayMillisecond(pCore, 2000);
			else
				HBA_DelayMillisecond(pCore, 1000);
#ifdef BERLIN_SPRUCE_INTEGRATION
			pDevice->State = DEVICE_STATE_SPRUCE_EXECUTE_DONE;
#else
			pDevice->State = DEVICE_STATE_SET_UDMA_DONE;
#endif
			mvDeviceStateMachine(pCore, pDevice, GALOIS_SATA_ATOMIC);
		}
		else
		{
			if (pPort->port_unit_ready_retry == 0) {
				galois_sata_event(GALOIS_EVENT_L2H_BADDISC | (GALOIS_EVENT_FROM_PORT(pPort->Id)), pPort, pDevice);
			}
			pDevice->State = DEVICE_STATE_REQUESTSENSE_DONE;
			mvDeviceStateMachine(pCore, pDevice, GALOIS_SATA_ATOMIC);
		}
	} else
#endif
	if ( pReq->Cdb[2]==CDB_CORE_ENABLE_WRITE_CACHE )
	{
		pDevice->State = DEVICE_STATE_ENABLE_WRITE_CACHE_DONE;
		mvDeviceStateMachine(pCore, pDevice, GALOIS_SATA_ATOMIC);
	}
	else if ( pReq->Cdb[2]==CDB_CORE_ENABLE_READ_AHEAD )
	{
		pDevice->State = DEVICE_STATE_ENABLE_READ_AHEAD_DONE;
		mvDeviceStateMachine(pCore, pDevice, GALOIS_SATA_ATOMIC);
	}
	else if ( pReq->Cdb[2]==CDB_CORE_READ_LOG_EXT )
	{
		/* Do nothing. Just use this command to clear outstanding IO during error handling. */
		MV_PRINT("Read Log Ext is finished on device 0x%x.\n", pDevice->Id);
	}
}

#ifdef BERLIN_SUPPORT_SPRUCE_DRIVE
static void Device_PowerStandby(
	IN PDomain_Port pPort,
	IN PDomain_Device pDevice
	)
{
	PCore_Driver_Extension pCore = pPort->Core_Extension;
	PMV_Request pReq = pDevice->Internal_Req;
	
	MV_ZeroMvRequest(pReq);

	MV_PRINT("[Galois]Power standby unit(port=%d).\n", pPort->Id);
	/* Prepare set UDMA mode task */
	pReq->Cdb[0] = SCSI_CMD_START_STOP_UNIT;
	pReq->Cdb[4] = 0x30;	/* powerstandby */
	pReq->Device_Id = pDevice->Id;
	pReq->Cmd_Initiator = pPort->Core_Extension;
	pReq->Data_Transfer_Length = 0;
	pReq->Data_Buffer = NULL;
	pReq->Completion = (void(*)(MV_PVOID,PMV_Request))Core_InternalReqCallback;

	/* Brian said that we don't need send power-standby command */
#if 0
	/* skip if this is ATA device */
	if( pDevice->Device_Type & DEVICE_TYPE_ATAPI )
	{
		Core_ModuleSendRequest(pPort->Core_Extension, pReq);
	}
	else
#endif
	{
		MV_PRINT("[Faked command.]\n");
		pReq->Scsi_Status = REQ_STATUS_SUCCESS;
		pReq->Completion(pCore, pReq);
	}
}

static void Device_StartUnit(
	IN PDomain_Port pPort,
	IN PDomain_Device pDevice
	)
{
	PCore_Driver_Extension pCore = pPort->Core_Extension;
	PMV_Request pReq = pDevice->Internal_Req;
	
	MV_ZeroMvRequest(pReq);

	MV_PRINT("[Galois]Start unit(port=%d).\n", pPort->Id);
	/* Prepare set UDMA mode task */
	pReq->Cdb[0] = SCSI_CMD_START_STOP_UNIT;
	pReq->Cdb[1] = 0x1;	/* Immed */
	pReq->Cdb[4] = 0x1;	/* start */
	pReq->Device_Id = pDevice->Id;
	pReq->Cmd_Initiator = pPort->Core_Extension;
	pReq->Data_Transfer_Length = 0;
	pReq->Data_Buffer = NULL;
	pReq->Completion = (void(*)(MV_PVOID,PMV_Request))Core_InternalReqCallback;

#if 0
	/* skip if this is ATA device */
	if( pDevice->Device_Type & DEVICE_TYPE_ATAPI )
	{
		Core_ModuleSendRequest(pPort->Core_Extension, pReq);
	}
	else
#endif
	{
		MV_PRINT("[Faked command.]\n");
		pReq->Scsi_Status = REQ_STATUS_SUCCESS;
		pReq->Completion(pCore, pReq);
	}
}
#endif

#ifdef BERLIN_SPRUCE_INTEGRATION
/*
 * for 64MB shared memory, the data is at 0x1c000000+0x200000+(20<<20)
 * i.e. 0x1d600000
 */
static int Device_ParseExternalSpruceData(spruce_data_header_t** spruce_var, spruce_data_header_t** spruce_image)
{
	spruce_data_header_t* data_header;
	unsigned int block_size;

	spruce_data_header_t *header_ptr;
	unsigned int ptr;
	int i;

	if (CUSTOMER_SATA_DATA_ENTRY_OFF + sizeof(spruce_data_header_t) > CONFIG_MV88DE3010_SHMMEM_SIZE) {
		MV_ERROR_PRINT("Flash-less loader: SHM doesn't contain flashless data, exit\n");
		return -1;
	}

	data_header = (spruce_data_header_t *)(CONFIG_MV88DE3010_SHMMEM_VIRTSTART + CUSTOMER_SATA_DATA_ENTRY_OFF);
	block_size = data_header->block_size; 
	MV_ERROR_PRINT("Flash-less loader: check NAND block size %d\n", block_size);
	if ((block_size%4) || (block_size > 0x04000000) || (16*block_size + CUSTOMER_SATA_DATA_ENTRY_OFF >= CONFIG_MV88DE3010_SHMMEM_SIZE)) {
		MV_ERROR_PRINT("Flash-less loader: bad block size, exit\n");
		return -1;
	}

	/* 
	 * get flashless variables from 15, 14... 0 block in sequence, if got it, 
	 * that one must be the failed safe one in NAND flash, exit
	 *
	 * NOTE: when burning, it always burns 2n and then 2n+1 block with variable in one time
	 * if one of the two blocks is broken(bad block), programm will burn to 2(n+1) and then 2(n+1)+1 block, and erase 0~2n blocks
	 * so in the 0~15 blocks, if scanning from 15 to 0 in decreasing order. there's always failed-safe block.
	 */
	*spruce_var = NULL;
	for (i = (BOOTFLOW_SPRUCE_VARIABLE_BLOCK_NUMBER-1); i >= 0; i--) {
		header_ptr = (spruce_data_header_t *)(CONFIG_MV88DE3010_SHMMEM_VIRTSTART + CUSTOMER_SATA_DATA_ENTRY_OFF + i*block_size);

		if ((header_ptr->magic1 == SPRUCE_MAGIC_NUMBER)
				&& (header_ptr->magic2 == SPRUCE_MAGIC_NUMBER)
				&& (header_ptr->type == SPRUCE_VARIABLE_TYPE)) {
			ptr = (unsigned int)header_ptr + sizeof(spruce_data_header_t) + (unsigned int)(header_ptr->length);
			if ((*(unsigned int *)ptr == SPRUCE_MAGIC_NUMBER) && (*(unsigned int *)(ptr+4) == SPRUCE_MAGIC_NUMBER)) {
				MV_ERROR_PRINT("Spruce flashless variable is at block %d, len=%d\n", i, header_ptr->length);
				*spruce_var = header_ptr;
				header_ptr->magic1 = 0;//clear to prevent reuse
				break;
			}
		}
	}

	/* 
	 * get flashless image
	 * Spruce flashless is for debug only, so it doesn't support power-failure or bad block
	 * the contents for spruce image has to be a continuous data
	 * it's stored at 16th block
	 */
	*spruce_image = NULL;
	for (i = BOOTFLOW_SPRUCE_VARIABLE_BLOCK_NUMBER; i < (BOOTFLOW_SPRUCE_VARIABLE_BLOCK_NUMBER+BOOTFLOW_SPRUCE_IMAGE_BLOCK_NUMBER); i++) {
		header_ptr = (spruce_data_header_t *)(CONFIG_MV88DE3010_SHMMEM_VIRTSTART + CUSTOMER_SATA_DATA_ENTRY_OFF + i*block_size);

		if ((header_ptr->magic1 == SPRUCE_MAGIC_NUMBER)
				&& (header_ptr->magic2 == SPRUCE_MAGIC_NUMBER)
				&& (header_ptr->type == SPRUCE_IMAGE_TYPE)) {
			ptr = (unsigned int)header_ptr + sizeof(spruce_data_header_t) + (unsigned int)(header_ptr->length);
			if ((*(unsigned int *)ptr == SPRUCE_MAGIC_NUMBER) && (*(unsigned int *)(ptr+4) == SPRUCE_MAGIC_NUMBER)) {
				MV_ERROR_PRINT("Spruce flashless image is at block %d, len=%d\n", i, header_ptr->length);
				*spruce_image = header_ptr;
				header_ptr->magic1 = 0;//clear to prevent reuse
				break;
			}
		}
	}

	MV_ERROR_PRINT("Flash-less loader: %s image, %s variable\n", 
			(*spruce_image == NULL)? "internal":"external", (*spruce_var == NULL)? "internal":"external");
	return 0;
}

static void Device_SpruceDownloadCode(PDomain_Port pPort, PDomain_Device pDevice)
{
	PCore_Driver_Extension pCore = pPort->Core_Extension;
	PMV_Request pReq = pDevice->Internal_Req;
	PMV_SG_Table pSGTable = &pReq->SG_Table;

	spruce_data_header_t* spruce_image = NULL;
	spruce_data_header_t* spruce_var = NULL;
	unsigned char *boot_package_phys_addr = 0;
	unsigned char *boot_package_virt_addr = 0;
	unsigned int boot_package_len = 0;
	unsigned int spruce_exec_offset = 0;

	/* get the image&var from NAND */
	Device_ParseExternalSpruceData(&spruce_var, &spruce_image);

	/* start burnning image+var */
	MV_ZeroMvRequest(pReq);

	if (!spruce_image && spruce_var && (spruce_var->length < 16384)) {
		MV_ERROR_PRINT("Flash-less loader: internal image, external variable\n");
		//there's no external image, but has external variable, then copy external variable to end of internal image
		boot_package_virt_addr = spruce_microcode_start;
		boot_package_phys_addr = (MV_PU8)virt_to_phys(boot_package_virt_addr);
		memcpy(spruce_microcode_end, (unsigned char *)spruce_var + sizeof(spruce_data_header_t), spruce_var->length);
		boot_package_len = ((unsigned int)spruce_microcode_end - (unsigned int)spruce_microcode_start) + spruce_var->length;
	} else if (spruce_image && spruce_var && (spruce_var->length < 16384)) {
		MV_ERROR_PRINT("Flash-less loader: external image, external variable\n");
		//there's both external image and external variable, then copy external variable to end of external image
		boot_package_virt_addr = ((unsigned char *)spruce_image) + sizeof(spruce_data_header_t);
		boot_package_phys_addr = (boot_package_virt_addr) - (CONFIG_MV88DE3010_SHMMEM_VIRTSTART - CONFIG_MV88DE3010_SHMMEM_START);
		memcpy(((unsigned char *)spruce_image) + sizeof(spruce_data_header_t) + spruce_image->length, 
				((unsigned char *)spruce_var) + sizeof(spruce_data_header_t), spruce_var->length);
		//boot_package_virt_addr = spruce_microcode_start;
		//boot_package_phys_addr = (MV_PU8)virt_to_phys(boot_package_virt_addr);
		//memcpy(boot_package_virt_addr, ((unsigned char *)spruce_image) + sizeof(spruce_data_header_t), spruce_image->length);
		//memcpy(boot_package_virt_addr + spruce_image->length, ((unsigned char *)spruce_var) + sizeof(spruce_data_header_t), spruce_var->length);
		boot_package_len = spruce_image->length + spruce_var->length;
	} else if (spruce_image && (!spruce_var || (spruce_var->length >= 16384))) {
		MV_ERROR_PRINT("Flash-less loader: external image, no variable\n");
		//there's external image, but no external variable, then just use external image
		boot_package_virt_addr = ((unsigned char *)spruce_image) + sizeof(spruce_data_header_t);
		boot_package_phys_addr = (boot_package_virt_addr) - (CONFIG_MV88DE3010_SHMMEM_VIRTSTART - CONFIG_MV88DE3010_SHMMEM_START);
		//boot_package_virt_addr = spruce_microcode_start;
		//boot_package_phys_addr = (MV_PU8)virt_to_phys(boot_package_virt_addr);
		//memcpy(boot_package_virt_addr, ((unsigned char *)spruce_image) + sizeof(spruce_data_header_t), spruce_image->length);
		boot_package_len = spruce_image->length;
	} else {
		MV_ERROR_PRINT("Flash-less loader: internal image, no variable\n");
		//use internal image without variable
		boot_package_virt_addr = spruce_microcode_start;
		boot_package_phys_addr = (MV_PU8)virt_to_phys(boot_package_virt_addr);
		boot_package_len = (unsigned int)spruce_microcode_end - (unsigned int)spruce_microcode_start;
	}
	MV_ERROR_PRINT("Download Spruce Microcode(port=%d), boot package len 0x%08x, start 0x%08x(phys 0x%08x)\n", 
			pPort->Id, boot_package_len, (unsigned int)boot_package_virt_addr, (unsigned int)boot_package_phys_addr);

	/* Prepare set UDMA mode task */
	pReq->Cdb[0] = SCSI_CMD_SPRUCE_DOWNLOAD_CODE;
	pReq->Cdb[1] = 0x1;	/* 1 for download; 0 for execute */

	pReq->Cdb[2] = (spruce_exec_offset >> 24) & 0xFF;
	pReq->Cdb[3] = (spruce_exec_offset >> 16) & 0xFF;
	pReq->Cdb[4] = (spruce_exec_offset >> 8) & 0xFF;
	pReq->Cdb[5] = spruce_exec_offset & 0xFF;

	pReq->Cdb[6] = (boot_package_len >> 24) & 0xFF;
	pReq->Cdb[7] = (boot_package_len >> 16) & 0xFF;
	pReq->Cdb[8] = (boot_package_len >> 8) & 0xFF;
	pReq->Cdb[9] = boot_package_len & 0xFF;

	pReq->Cdb[10] = 0x0;	/* reserved */
	pReq->Cdb[11] = 0x0;	/* control */

	pReq->Device_Id = pDevice->Id;
	pReq->Cmd_Initiator = pPort->Core_Extension;
	pReq->Data_Transfer_Length = boot_package_len;
	pReq->Data_Buffer = boot_package_virt_addr;
	pReq->Completion = (void(*)(MV_PVOID,PMV_Request))Core_InternalReqCallback;

	/* Make SG table */
	SGTable_Init(pSGTable, 0);
	SGTable_Append(pSGTable, 
				(MV_U32)boot_package_phys_addr, 
				(MV_U32)0,
				(MV_U32)boot_package_len
				); 

	/* skip if this is Marvell ATAPI device and it's at 1st time bootup(doesn't support hotplug) */
	if((pDevice->Device_Type & DEVICE_TYPE_ATAPI) && (pPort->port_flashless_drive) && (!highlevel_probe_done))
	{
		MV_ERROR_PRINT("[Galois]Send ATAPI DownloadMicrocode command(port=%d).\n", pPort->Id);
		Core_ModuleSendRequest(pPort->Core_Extension, pReq);
	}
	else
	{
		MV_ERROR_PRINT("[Faked command.]\n");
		pReq->Scsi_Status = REQ_STATUS_SUCCESS;
		pReq->Completion(pCore, pReq);
	}
}

static void Device_SpruceExecuteCode(PDomain_Port pPort, PDomain_Device pDevice)
{
	PCore_Driver_Extension pCore = pPort->Core_Extension;
	PMV_Request pReq = pDevice->Internal_Req;
	PMV_SG_Table pSGTable = &pReq->SG_Table;

	/* start burnning image+var */
	MV_ZeroMvRequest(pReq);

	/* execute microcode in flashless spruce */
	pReq->Cdb[0] = SCSI_CMD_SPRUCE_DOWNLOAD_CODE;
	pReq->Cdb[1] = 0x0;	/* 1 for download; 0 for execute */

	pReq->Cdb[2] = 0x0;
	pReq->Cdb[3] = 0x0;
	pReq->Cdb[4] = 0x0;
	pReq->Cdb[5] = 0x0;
	pReq->Cdb[6] = 0x0;
	pReq->Cdb[7] = 0x0;
	pReq->Cdb[8] = 0x0;
	pReq->Cdb[9] = 0x0;

	pReq->Cdb[10] = 0x0;	/* reserved */
	pReq->Cdb[11] = 0x0;	/* control */

	pReq->Device_Id = pDevice->Id;
	pReq->Cmd_Initiator = pPort->Core_Extension;
	pReq->Data_Transfer_Length = 0;
	pReq->Data_Buffer = NULL;
	pReq->Completion = (void(*)(MV_PVOID,PMV_Request))Core_InternalReqCallback;

	/* skip if this is Marvell ATAPI device and it's at 1st time bootup(doesn't support hotplug) */
	if((pDevice->Device_Type & DEVICE_TYPE_ATAPI) && (pPort->port_flashless_drive) && (!highlevel_probe_done))
	{
		MV_ERROR_PRINT("[Galois]Send ATAPI ExecuteMicrocode command(port=%d).\n", pPort->Id);
		Core_ModuleSendRequest(pPort->Core_Extension, pReq);
	}
	else
	{
		MV_ERROR_PRINT("[Faked command.]\n");
		pReq->Scsi_Status = REQ_STATUS_SUCCESS;
		pReq->Completion(pCore, pReq);
	}

}
#endif

#ifdef BERLIN_ATAPI_GET_READY
static void Device_TestUnitReady(PDomain_Port pPort, PDomain_Device pDevice)
{
	PCore_Driver_Extension pCore = pPort->Core_Extension;
	PMV_Request pReq = pDevice->Internal_Req;
	
	MV_ZeroMvRequest(pReq);

	MV_PRINT("[Galois]Test unit ready(port=%d).\n", pPort->Id);
	/* Prepare set UDMA mode task */
	pReq->Cdb[0] = SCSI_CMD_TEST_UNIT_READY;
	pReq->Cdb[1] = 0x0;	/* Immed */
	pReq->Cdb[4] = 0x0;	/* start */
	pReq->Device_Id = pDevice->Id;
	pReq->Cmd_Initiator = pPort->Core_Extension;
	pReq->Data_Transfer_Length = 0;
	pReq->Data_Buffer = NULL;
	pReq->Completion = (void(*)(MV_PVOID,PMV_Request))Core_InternalReqCallback;

	/* skip if this is ATA device */
	if((pDevice->Device_Type & DEVICE_TYPE_ATAPI) && (lowlevel_probe_done) && (highlevel_probe_done))
	{
		Core_ModuleSendRequest(pPort->Core_Extension, pReq);
	}
	else
	{
		MV_PRINT("[Faked command.]\n");
		pReq->Scsi_Status = REQ_STATUS_SUCCESS;
		pReq->Completion(pCore, pReq);
	}
}

static void Device_MakeRequestSense(PDomain_Port pPort, PDomain_Device pDevice)
{
	PCore_Driver_Extension pCore = pPort->Core_Extension;
	PMV_Request pReq = pDevice->Internal_Req;
	PMV_SG_Table pSGTable = &pReq->SG_Table;
	
	MV_ZeroMvRequest(pReq);

	MV_PRINT("[Galois]Request sense(port=%d).\n", pPort->Id);
	/* Prepare set UDMA mode task */
	pReq->Cdb[0] = SCSI_CMD_REQUEST_SENSE;
	pReq->Cdb[4] = 18;	/* 0x12 */
	pReq->Device_Id = pDevice->Id;
	pReq->Cmd_Initiator = pPort->Core_Extension;
	pReq->Data_Transfer_Length = 18;
	pReq->Data_Buffer = pDevice->Scratch_Buffer;
	//pReq->Cmd_Flag = CMD_FLAG_DATA_IN;
	pReq->Completion = (void(*)(MV_PVOID,PMV_Request))Core_InternalReqCallback;

	/* Make SG table */
	SGTable_Init(pSGTable, 0);
	SGTable_Append(pSGTable, 
				pDevice->Scratch_Buffer_DMA.low, 
				pDevice->Scratch_Buffer_DMA.high,
				pReq->Data_Transfer_Length
				); 
	MV_ZeroMemory(pReq->Data_Buffer, pReq->Data_Transfer_Length);

	/* skip if this is ATA device */
	if((pDevice->Device_Type & DEVICE_TYPE_ATAPI) && (lowlevel_probe_done) && (highlevel_probe_done))
	{
		Core_ModuleSendRequest(pPort->Core_Extension, pReq);
	}
	else
	{
		MV_PRINT("[Faked command.]\n");
		pReq->Scsi_Status = REQ_STATUS_SUCCESS;
		pReq->Completion(pCore, pReq);
	}
}
#endif

static void Device_IssueIdentify(
	IN PDomain_Port pPort,
	IN PDomain_Device pDevice
	)
{
	PMV_Request pReq = pDevice->Internal_Req;
	PMV_SG_Table pSGTable = &pReq->SG_Table;

/*	MV_ZeroMemory(pReq, MV_REQUEST_SIZE);*/
	MV_ZeroMvRequest(pReq);

	MV_PRINT("[Galois]Issue ID command(port=%d).\n", pPort->Id);
	/* Prepare identify ATA task */
	pReq->Cdb[0] = SCSI_CMD_MARVELL_SPECIFIC;
	pReq->Cdb[1] = CDB_CORE_MODULE;
	pReq->Cdb[2] = CDB_CORE_IDENTIFY;
	pReq->Device_Id = pDevice->Id;

	//pReq->Req_Flag;
	pReq->Cmd_Initiator = pPort->Core_Extension;
	pReq->Data_Transfer_Length = sizeof(ATA_Identify_Data);
	pReq->Data_Buffer = pDevice->Scratch_Buffer;
	pReq->Completion = (void(*)(MV_PVOID,PMV_Request))Core_InternalReqCallback;
	MV_DASSERT( SATA_SCRATCH_BUFFER_SIZE>=sizeof(ATA_Identify_Data) );

	/* Make SG table */
	SGTable_Init(pSGTable, 0);
	SGTable_Append(pSGTable, 
				pDevice->Scratch_Buffer_DMA.low, 
				pDevice->Scratch_Buffer_DMA.high,
				pReq->Data_Transfer_Length
				); 
	MV_DASSERT( pReq->Data_Transfer_Length%2==0 );
	//MV_DUMPC32(0xCCCCBB40);

	/* Send this internal request */
	Core_ModuleSendRequest(pPort->Core_Extension, pReq);
}

void Device_IssueReadLogExt(
	IN PDomain_Port pPort,
	IN PDomain_Device pDevice
	)
{
	PMV_Request pReq = pDevice->Internal_Req;
	PMV_SG_Table pSGTable = &pReq->SG_Table;

/*	MV_ZeroMemory(pReq, MV_REQUEST_SIZE);*/
	MV_ZeroMvRequest(pReq);
	MV_PRINT("Device_IssueReadLogExt on device 0x%x.\n", pDevice->Id);

	//TBD: Disable NCQ after we found NCQ error.
	pDevice->Capacity &= ~(DEVICE_CAPACITY_NCQ_SUPPORTED);

	/* We support READ LOG EXT command with log page of 10h. */
	pReq->Cdb[0] = SCSI_CMD_MARVELL_SPECIFIC;
	pReq->Cdb[1] = CDB_CORE_MODULE;
	pReq->Cdb[2] = CDB_CORE_READ_LOG_EXT;
	pReq->Device_Id = pDevice->Id;

	//pReq->Req_Flag;
	pReq->Cmd_Initiator = pPort->Core_Extension;
	pReq->Data_Transfer_Length = SATA_SCRATCH_BUFFER_SIZE;
	pReq->Data_Buffer = pDevice->Scratch_Buffer;
	pReq->Completion = (void(*)(MV_PVOID,PMV_Request))Core_InternalReqCallback;
	MV_DASSERT( SATA_SCRATCH_BUFFER_SIZE>=sizeof(ATA_Identify_Data) );

	/* Make SG table */
	SGTable_Init(pSGTable, 0);
	SGTable_Append(pSGTable, 
				pDevice->Scratch_Buffer_DMA.low, 
				pDevice->Scratch_Buffer_DMA.high,
				pReq->Data_Transfer_Length
				); 
	MV_DASSERT( pReq->Data_Transfer_Length%2==0 );
	//MV_DUMPC32(0xCCCCBB40);

	/* Send this internal request */
	Core_ModuleSendRequest(pPort->Core_Extension, pReq);
}

static MV_VOID mvAta2HostString(IN MV_U16 *source,
                                OUT MV_U16 *target,
                                IN MV_U32 wordsCount
                               )
{
    MV_U32 i;
    for (i=0 ; i < wordsCount; i++)
    {
        target[i] = (source[i] >> 8) | ((source[i] & 0xff) << 8);
        target[i] = MV_LE16_TO_CPU(target[i]);
    }
}

void Device_ParseIdentifyData(
	IN PDomain_Device pDevice,
	IN PATA_Identify_Data pATAIdentify
	)
{
	PDomain_Port pPort = pDevice->PPort;
	MV_U8 i;
	MV_U32 temp;

	/* Get serial number, firmware revision and model number. */
	MV_CopyMemory(pDevice->Serial_Number, pATAIdentify->Serial_Number, 20);
	MV_CopyMemory(pDevice->Firmware_Revision, pATAIdentify->Firmware_Revision, 8);

	MV_CopyMemory(pDevice->Model_Number, pATAIdentify->Model_Number, 40);
	mvAta2HostString((MV_U16 *)pDevice->Serial_Number, (MV_U16 *)pDevice->Serial_Number, 10);
	mvAta2HostString((MV_U16 *)pDevice->Firmware_Revision, 
			(MV_U16 *)pDevice->Firmware_Revision, 4);

	mvAta2HostString((MV_U16 *)pDevice->Model_Number, (MV_U16 *)pDevice->Model_Number, 20);
	MV_ERROR_PRINT("ATA/ATAPI(port=%d) ID, Model:%s, F/W rev:%s\n", pPort->Id, pDevice->Model_Number, pDevice->Firmware_Revision);

	/*
	 * check if it's Flash-less Spruce drive "Boot ROM"
	 */
	if (strstr(pDevice->Model_Number, "Boot ROM")) 
	{
		MV_ERROR_PRINT("Port %d connects Flash-less Marvell drive\n", pPort->Id);
		pPort->port_flashless_drive = 1;
		wait_loader_ready = 1;
		spruce_flashless_loader = 1;
	}
	else
	{
		pPort->port_flashless_drive = 0;
		wait_loader_ready = 0;
		spruce_flashless_loader = 0;
	}

	/*
	 * check if it's Spruce drive, which has model like  "Marvell System On Chip..."
	 */
	if (strstr(pDevice->Model_Number, "Marvell")) 
	{
		MV_ERROR_PRINT("Port %d connects Marvell drive(don't enable READ10->READ12)\n", pPort->Id);
		berlin_spruce_loader = 1;
		pPort->port_marvell_drive = 1;
		pPort->port_read10_to_read12 = 0;
	}
	else
	{
		pPort->port_marvell_drive = 0;
		pPort->port_read10_to_read12 = 0;
	}

	/* Capacity: 48 bit LBA, smart, write cache and NCQ */
	pDevice->Capacity = 0;
	pDevice->Setting = 0;
	if ( pATAIdentify->Command_Set_Supported[1] & MV_BIT(10) )
	{
		MV_DPRINT(("Device: %d 48 bit supported.\n", pDevice->Id));
		MV_PRINT("[yfzheng]Device: %d 48 bit supported.\n", pDevice->Id);
		pDevice->Capacity |= DEVICE_CAPACITY_48BIT_SUPPORTED;
	}
	else
	{
		MV_DPRINT(("Device: %d 48 bit not supported.\n", pDevice->Id));
	}

	if ( pATAIdentify->Command_Set_Supported[0] & MV_BIT(0) ) 
	{
		MV_PRINT("[yfzheng]capacity smart supported.\n");
		pDevice->Capacity |= DEVICE_CAPACITY_SMART_SUPPORTED;
		if ( pATAIdentify->Command_Set_Enabled[0] & MV_BIT(0) )
		{
			MV_PRINT("[yfzheng]capacity smart enabled.\n");
			pDevice->Setting |= DEVICE_SETTING_SMART_ENABLED;
		}
	}	
	if ( pATAIdentify->Command_Set_Supported[0] & MV_BIT(5) ) 
	{
		MV_PRINT("[yfzheng]write cache supported.\n");
		pDevice->Capacity |= DEVICE_CAPACITY_WRITECACHE_SUPPORTED;
		if ( pATAIdentify->Command_Set_Enabled[0] & MV_BIT(5) )
		{
			MV_PRINT("[yfzheng]write cache enabled.\n");
			pDevice->Setting |= DEVICE_SETTING_WRITECACHE_ENABLED;
		}
	}
	if ( pATAIdentify->SATA_Capabilities & MV_BIT(8) )
	{
		if (pDevice->Capacity & DEVICE_CAPACITY_48BIT_SUPPORTED) 
		{
			MV_PRINT("[yfzheng]NCQ supported.\n");
			pDevice->Capacity |= DEVICE_CAPACITY_NCQ_SUPPORTED;
		}
	}
	if ( pATAIdentify->Command_Set_Supported_Extension & MV_BIT(5) )
	{
		if ( pATAIdentify->Command_Set_Default & MV_BIT(5) )
			pDevice->Capacity |= DEVICE_CAPACITY_READLOGEXT_SUPPORTED;
	}

	temp = MV_REG_READ_DWORD( pPort->Mmio_Base, PORT_SCR_STAT );
	if ( ((temp >> 4) & 0xF) == 1 )
		pDevice->Capacity |= DEVICE_CAPACITY_RATE_1_5G;
	else if ( ((temp >> 4) & 0xF) == 2 )
		pDevice->Capacity |= DEVICE_CAPACITY_RATE_3G;
	MV_PRINT("[yfzheng]capacity=0x%08x\n", pDevice->Capacity);

	/* Disk size */
	if ( pDevice->Capacity&DEVICE_CAPACITY_48BIT_SUPPORTED )
	{
		pDevice->Max_LBA.low = *((MV_PU32)&pATAIdentify->Max_LBA[0]);
		pDevice->Max_LBA.high = *((MV_PU32)&pATAIdentify->Max_LBA[2]);
	}else 
	{
		pDevice->Max_LBA.low = *((MV_PU32)&pATAIdentify->User_Addressable_Sectors[0]);
		pDevice->Max_LBA.high = 0;
	}
	MV_PRINT("[yfzheng]maxlba=0x%08x, minlba=0x%08x\n", 
			pDevice->Max_LBA.high, pDevice->Max_LBA.low);
	
	/* PIO, MDMA and UDMA mode */	
   	if ( (pATAIdentify->Fields_Valid&MV_BIT(1)) && (pATAIdentify->PIO_Modes&0x0F) )	
	{
		if ( (MV_U8)pATAIdentify->PIO_Modes>=0x2 )
			pDevice->PIO_Mode = 0x04; /* XXX:??? */
		else
			pDevice->PIO_Mode = 0x03; 
	}
	else
	{
		pDevice->PIO_Mode = 0x02;
	}
	MV_PRINT("[yfzheng]pio_mode=%d\n", pDevice->PIO_Mode);

	pDevice->MDMA_Mode = 0xFF;
	if ( pATAIdentify->Multiword_DMA_Modes & MV_BIT(2) )
		pDevice->MDMA_Mode = 2;
	else if ( pATAIdentify->Multiword_DMA_Modes & MV_BIT(1) )
		pDevice->MDMA_Mode = 1;
	else if ( pATAIdentify->Multiword_DMA_Modes & MV_BIT(0) )
		pDevice->MDMA_Mode = 0;

	pDevice->UDMA_Mode = 0xFF;
	if ( pATAIdentify->Fields_Valid&MV_BIT(2) )
	{
		for ( i=0; i<7; i++ )
		{
			if ( pATAIdentify->UDMA_Modes & MV_BIT(i) )
				pDevice->UDMA_Mode = i;	
		}
		MV_PRINT("[yfzheng]udma=0x%08x, mode=%d\n", 
				pATAIdentify->UDMA_Modes, pDevice->UDMA_Mode);
	}
	/* CRC identify buffer to get the U32 GUID. */
	pDevice->WWN = MV_CRC((MV_PU8)pATAIdentify, sizeof(ATA_Identify_Data));
}

static void Device_IssueSetMDMAMode(
	IN PDomain_Port pPort,
	IN PDomain_Device pDevice
	)
{
//	PCore_Driver_Extension pCore = pPort->Core_Extension;
	PMV_Request pReq = pDevice->Internal_Req;
	MV_U8 mode = pDevice->MDMA_Mode;

	/* Only if the Device doesn't support UDMA, we'll use MDMA mode. */
	MV_ASSERT( pDevice->UDMA_Mode==0xFF );
	/* Is that possible that one device doesn't support either UDMA and MDMA? */
	MV_ASSERT( (pDevice->MDMA_Mode<=2) );
/*	MV_ZeroMemory(pReq, MV_REQUEST_SIZE);*/
	MV_ZeroMvRequest(pReq);

	MV_PRINT("\n[Galois]Setting to MDMA mode %d (port=%d).\n", mode, pPort->Id);
	/* Prepare set UDMA mode task */
	pReq->Cdb[0] = SCSI_CMD_MARVELL_SPECIFIC;
	pReq->Cdb[1] = CDB_CORE_MODULE;
	pReq->Cdb[2] = CDB_CORE_SET_UDMA_MODE;
	pReq->Cdb[3] = mode;
	/* Means we are setting MDMA mode. I still use CDB_CORE_SET_UDMA_MODE 
	 * because I don't want to change the state machine. */
	pReq->Cdb[4] = MV_TRUE;
	pReq->Device_Id = pDevice->Id;
	//pReq->Req_Flag;
	pReq->Cmd_Initiator = pPort->Core_Extension;
	pReq->Data_Transfer_Length = 0;
	pReq->Data_Buffer = NULL;
	pReq->Completion = (void(*)(MV_PVOID,PMV_Request))Core_InternalReqCallback;

	/* Send this internal request */
	Core_ModuleSendRequest(pPort->Core_Extension, pReq);
}

static void Device_IssueSetUDMAMode(
	IN PDomain_Port pPort,
	IN PDomain_Device pDevice
	)
{
	PCore_Driver_Extension pCore = pPort->Core_Extension;
	PMV_Request pReq = pDevice->Internal_Req;
	MV_U8 mode = pDevice->UDMA_Mode;
	
	if ( pDevice->UDMA_Mode==0xFF )
	{
		Device_IssueSetMDMAMode(pPort, pDevice);
		return;
	}

	/* Hardware team required us to downgrade UDMA mode to zero. */
	//if ( pDevice->Device_Type&DEVICE_TYPE_ATAPI )
	//	mode = 0;
	//if ( mode>=5 ) mode = 4; //???

/*	MV_ZeroMemory(pReq, MV_REQUEST_SIZE);*/
	MV_ZeroMvRequest(pReq);

	/*
	 * Degrade ATAPI device UDMA mode always to 2.
	 * Workaround:
	 * Thor lite A0 has problem with Hitesh(IBM) HDD under UDMA 5
	 * And it has problem with any HDD under UDMA 6
	 * So we degrade the HDD mode to 5 and ignore Hitesh HDD for now.
	 */
#if 0
	if ( pDevice->Device_Type&DEVICE_TYPE_ATAPI )
		mode = (mode > 2)? 2 : mode;
	else
		mode = (mode > 5)? 5 : mode;
#endif

	MV_PRINT("\n[Galois]Setting mode to UltraDMA %d (port=%d).\n", mode, pPort->Id);

	/* Prepare set UDMA mode task */
	pReq->Cdb[0] = SCSI_CMD_MARVELL_SPECIFIC;
	pReq->Cdb[1] = CDB_CORE_MODULE;
	pReq->Cdb[2] = CDB_CORE_SET_UDMA_MODE;
	pReq->Cdb[3] = mode;
	/* Not setting MDMA but UDMA mode. */
	pReq->Cdb[4] = MV_FALSE;
	pReq->Device_Id = pDevice->Id;
	//pReq->Req_Flag;
	pReq->Cmd_Initiator = pPort->Core_Extension;
	pReq->Data_Transfer_Length = 0;
	pReq->Data_Buffer = NULL;
	pReq->Completion = (void(*)(MV_PVOID,PMV_Request))Core_InternalReqCallback;

#if 1
	/* Send this internal request */
	Core_ModuleSendRequest(pPort->Core_Extension, pReq);
#else
	MV_PRINT("[Faked command.]\n");
	pReq->Scsi_Status = REQ_STATUS_SUCCESS;
	pReq->Completion(pCore, pReq);
#endif
}

static void Device_IssueSetPIOMode(
	IN PDomain_Port pPort,
	IN PDomain_Device pDevice
	)
{
	PCore_Driver_Extension pCore = pPort->Core_Extension;
	PMV_Request pReq = pDevice->Internal_Req;
	MV_U8 mode = pDevice->PIO_Mode;
	
/*	MV_ZeroMemory(pReq, MV_REQUEST_SIZE);*/
	MV_ZeroMvRequest(pReq);

	/* Hardware team required us to downgrade PIO mode to zero. */
	if ( pDevice->Device_Type&DEVICE_TYPE_ATAPI )
		mode = 0;

	MV_PRINT("\n[Galois]Setting mode to PIO = %d (port=%d).\n", mode, pPort->Id);
	/* Prepare set PIO mode task */
	pReq->Cdb[0] = SCSI_CMD_MARVELL_SPECIFIC;
	pReq->Cdb[1] = CDB_CORE_MODULE;
	pReq->Cdb[2] = CDB_CORE_SET_PIO_MODE;
	pReq->Cdb[3] = mode;
	pReq->Device_Id = pDevice->Id;
	//pReq->Req_Flag;
	pReq->Cmd_Initiator = pPort->Core_Extension;
	pReq->Data_Transfer_Length = 0;
	pReq->Data_Buffer = NULL;
	pReq->Completion = (void(*)(MV_PVOID,PMV_Request))Core_InternalReqCallback;

#if 1
	/* Send this internal request */
	Core_ModuleSendRequest(pPort->Core_Extension, pReq);
#else
	MV_PRINT("[Faked command.]\n");
	pReq->Scsi_Status = REQ_STATUS_SUCCESS;
	pReq->Completion(pCore, pReq);
#endif
}

static void Device_EnableWriteCache(
	IN PDomain_Port pPort,
	IN PDomain_Device pDevice
	)
{
	PCore_Driver_Extension pCore = pPort->Core_Extension;
	PMV_Request pReq = pDevice->Internal_Req;

/*	MV_ZeroMemory(pReq, MV_REQUEST_SIZE);*/
	MV_ZeroMvRequest(pReq);

	MV_PRINT("\n[Galois]Enable write cache (port=%d).\n", pPort->Id);
	/* Prepare enable write cache command */
	pReq->Cdb[0] = SCSI_CMD_MARVELL_SPECIFIC;
	pReq->Cdb[1] = CDB_CORE_MODULE;
	pReq->Cdb[2] = CDB_CORE_ENABLE_WRITE_CACHE;
	pReq->Device_Id = pDevice->Id;
	//pReq->Req_Flag;
	pReq->Cmd_Initiator = pPort->Core_Extension;
	pReq->Data_Transfer_Length = 0;
	pReq->Data_Buffer = NULL;
	pReq->Completion = (void(*)(MV_PVOID,PMV_Request))Core_InternalReqCallback;

	/* skip if this is ATAPI device */
	if( pDevice->Device_Type & DEVICE_TYPE_ATAPI )
	{
		MV_PRINT("[Faked command.]\n");
		pReq->Scsi_Status = REQ_STATUS_SUCCESS;
		pReq->Completion(pCore, pReq);
	}
	else
	{
		/* Send this internal request */
		Core_ModuleSendRequest(pPort->Core_Extension, pReq);
	}
}

static void Device_EnableReadAhead(
	IN PDomain_Port pPort,
	IN PDomain_Device pDevice
	)
{
	PCore_Driver_Extension pCore = pPort->Core_Extension;
	PMV_Request pReq = pDevice->Internal_Req;

/*	MV_ZeroMemory(pReq, MV_REQUEST_SIZE);*/
	MV_ZeroMvRequest(pReq);

	MV_PRINT("\n[Galois]Enable read ahead (port=%d).\n", pPort->Id);
	/* Prepare enable read ahead command */
	pReq->Cdb[0] = SCSI_CMD_MARVELL_SPECIFIC;
	pReq->Cdb[1] = CDB_CORE_MODULE;
	pReq->Cdb[2] = CDB_CORE_ENABLE_READ_AHEAD;
	pReq->Device_Id = pDevice->Id;
	//pReq->Req_Flag;
	pReq->Cmd_Initiator = pPort->Core_Extension;
	pReq->Data_Transfer_Length = 0;
	pReq->Data_Buffer = NULL;
	pReq->Completion = (void(*)(MV_PVOID,PMV_Request))Core_InternalReqCallback;

	/* skip if this is ATAPI device */
	if( pDevice->Device_Type & DEVICE_TYPE_ATAPI )
	{
		MV_PRINT("[Faked command.]\n");
		pReq->Scsi_Status = REQ_STATUS_SUCCESS;
		pReq->Completion(pCore, pReq);
	}
	else
	{
		/* Send this internal request */
		Core_ModuleSendRequest(pPort->Core_Extension, pReq);
	}
}

