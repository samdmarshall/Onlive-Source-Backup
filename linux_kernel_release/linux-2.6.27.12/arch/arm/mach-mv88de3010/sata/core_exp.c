#include "mv_include.h"

#include "com_event.h"
#include "com_tag.h"

#include "core_sata.h"
#include "core_main.h"

#include "hba_exp.h"
#include "hba_mod.h"
#include "hba_timer.h"

#include <mach/galois_generic.h>
#include <mach/gpio.h>

#define GALOIS_COMRESET_HANG 1

#define FIS_REG_H2D_SIZE_IN_DWORD	5
/* For debug purpose only. */
PCore_Driver_Extension gCore;

void MV_DumpHeader(PMV_Command_Header header);
void MV_DumpFIS(PSATA_FIS_REG_H2D pFIS);
extern MV_VOID SCSI_To_FIS(MV_PVOID pCore, PMV_Request pReq, MV_U8 tag, PATA_TaskFile pTaskFile);

extern MV_BOOLEAN Category_CDB_Type(
	IN PDomain_Device pDevice,
	IN PMV_Request pReq
	);

extern MV_BOOLEAN ATAPI_CDB2TaskFile(
	IN PDomain_Device pDevice,
	IN PMV_Request pReq, 
	OUT PATA_TaskFile pTaskFile
	);

extern MV_BOOLEAN ATA_CDB2TaskFile(
	IN PDomain_Device pDevice,
	IN PMV_Request pReq, 
	IN MV_U8 tag,	//TBD: Do we really need it?
	OUT PATA_TaskFile pTaskFile
	);

extern void Device_IssueReadLogExt(
	IN PDomain_Port pPort,
	IN PDomain_Device pDevice
	);

extern MV_BOOLEAN mvDeviceStateMachine(
	PCore_Driver_Extension pCore,
	PDomain_Device pDevice,
	MV_U32 atomic
	);

void CompleteRequest(
	IN PCore_Driver_Extension pCore,
	IN PMV_Request pReq,
	IN PATA_TaskFile taskFiles
	);

void CompleteRequestAndSlot(
	IN PCore_Driver_Extension pCore,
	IN PDomain_Port pPort,
	IN PMV_Request pReq,
	IN PATA_TaskFile taskFiles,
	IN MV_U8 slotId
	);

#if defined(SUPPORT_ERROR_HANDLING) && defined(_OS_LINUX)
void Core_ResetChannel(MV_PVOID Device);

void MV_DumpHBA(PCore_Driver_Extension pCore, PDomain_Port pPort)
{
	MV_LPVOID mmio = pCore->Mmio_Base;
	MV_LPVOID port_mmio = pPort->Mmio_Base;
	MV_U32 value;

	value = MV_REG_READ_DWORD(mmio, HOST_CTL);
	MV_PRINT("HOST_CTL=0x%08x\n", value);
	value = MV_REG_READ_DWORD(port_mmio, PORT_SCR_ACT);
	MV_PRINT("PORT_SCR_ACT=0x%08x\n", value);
	value = MV_REG_READ_DWORD(port_mmio, PORT_SCR_STAT);
	MV_PRINT("PORT_SCR_STAT=0x%08x\n", value);
	value = MV_REG_READ_DWORD(port_mmio, PORT_SCR_ERR);
	MV_PRINT("PORT_SCR_ERR=0x%08x\n", value);
	value = MV_REG_READ_DWORD(port_mmio, PORT_IRQ_STAT);
	MV_PRINT("PORT_IRQ_STAT=0x%08x\n", value);
	value = MV_REG_READ_DWORD(port_mmio, PORT_CMD);
	MV_PRINT("PORT_CMD=0x%08x\n", value);
	value = MV_REG_READ_DWORD(port_mmio, PORT_CMD_ISSUE);
	MV_PRINT("PORT_CMD_ISSUE=0x%08x\n", value);
	value = MV_REG_READ_DWORD(port_mmio, PORT_TFDATA);
	MV_PRINT("PORT_TFDATA=0x%08x\n", value);
	value = MV_REG_READ_DWORD(port_mmio, PORT_SIG);
	MV_PRINT("PORT_SIG=0x%08x\n", value);
	MV_PRINT("\n");
}

void Port_HangReturnRunningReq(MV_PVOID This)
{
	PCore_Driver_Extension pCore = (PCore_Driver_Extension)This;
	PDomain_Port pPort = NULL;
	PMV_Request pReq = NULL;
	MV_U8 i, j;

	for ( i=0; i<pCore->Port_Num; i++ ) 
	{
		pPort = &pCore->Ports[i];
		for ( j=0; j<MAX_SLOT_NUMBER; j++ ) {
			if ( pPort->Running_Req[j]!=NULL ) {
				MV_PRINT("[Current hanging command slots]: port=%d, slot=%d\n", i, j);
				pReq = pPort->Running_Req[j];
				pReq->Scsi_Status = REQ_STATUS_MEDIA_ERROR;
				CompleteRequestAndSlot(pCore, pPort, pReq, NULL, j);
			}
		}
	}
}

static void Galois_ErrorDumpHBA(PCore_Driver_Extension pCore, PDomain_Port pPort)
{
	MV_LPVOID mmio = pCore->Mmio_Base;
	MV_LPVOID port_mmio;
	MV_U32 value;
	int i;

	value = MV_REG_READ_DWORD(mmio, HOST_CAP);
	MV_ERROR_PRINT("HOST_CAP=0x%08x\n", value);
	value = MV_REG_READ_DWORD(mmio, HOST_CTL);
	MV_ERROR_PRINT("HOST_CTL=0x%08x\n", value);
	value = MV_REG_READ_DWORD(mmio, HOST_IRQ_STAT);
	MV_ERROR_PRINT("HOST_IRQ_STAT=0x%08x\n", value);

	for (i = 0; i < pCore->Port_Num; i++) {
		MV_ERROR_PRINT("port %d:\n", i);
		port_mmio = (pCore->Ports[i]).Mmio_Base;
		value = MV_REG_READ_DWORD(port_mmio, PORT_CMD);
		MV_ERROR_PRINT("PORT_CMD=0x%08x\n", value);
		value = MV_REG_READ_DWORD(port_mmio, PORT_IRQ_STAT);
		MV_ERROR_PRINT("PORT_IRQ_STAT=0x%08x\n", value);
		value = MV_REG_READ_DWORD(port_mmio, PORT_TFDATA);
		MV_ERROR_PRINT("PORT_TFDATA=0x%08x\n", value);
		value = MV_REG_READ_DWORD(port_mmio, PORT_SCR_ACT);
		MV_ERROR_PRINT("PORT_SCR_ACT=0x%08x\n", value);
		value = MV_REG_READ_DWORD(port_mmio, PORT_SCR_STAT);
		MV_ERROR_PRINT("PORT_SCR_STAT=0x%08x\n", value);
		value = MV_REG_READ_DWORD(port_mmio, PORT_SCR_ERR);
		MV_ERROR_PRINT("PORT_SCR_ERR=0x%08x\n", value);
		value = MV_REG_READ_DWORD(port_mmio, PORT_CMD_ISSUE);
		MV_ERROR_PRINT("PORT_CMD_ISSUE=0x%08x\n", value);
		value = MV_REG_READ_DWORD(port_mmio, PORT_SIG);
		MV_ERROR_PRINT("PORT_SIG=0x%08x\n", value);
	}
	return;
}

static void Galois_ErrorDumpCmd(PMV_Request pReq)
{
	int i;
	MV_ERROR_PRINT("Cmd: ");
	for (i = 0; i < 16; i++) {
		MV_ERROR_PRINT("%02x ", pReq->Cdb[i]);
	}
	MV_ERROR_PRINT("\n");
	return;
}

void Galois_ErrorDumpHeader(PMV_Command_Header header)
{
	MV_ERROR_PRINT("Command header structure:\n");
	MV_ERROR_PRINT("\tFIS_Length=0x%02x, ATAPI=%d, RESET=%d, NCQ=%d, PMP=%d, PRDTL=%d\n", 
			header->FIS_Length, header->Packet_Command, 
			header->Reset, header->NCQ, header->PM_Port, header->PRD_Entry_Count);
	MV_ERROR_PRINT("\tTable_Address=0x%08x, Table_Address_High=0x%08x\n", 
			header->Table_Address, header->Table_Address_High);
}

void Galois_ErrorDumpFIS(PSATA_FIS_REG_H2D pFIS)
{
	MV_ERROR_PRINT("CFIS structure:\n");
	MV_ERROR_PRINT("\tFIS_Type=0x%02x, C=%d, PMP=%d\n", pFIS->FIS_Type, pFIS->C, pFIS->PM_Port);
	MV_ERROR_PRINT("\tCommand=0x%02x, Control=0x%02x, Device=0x%02x\n", 
			pFIS->Command, pFIS->Control, pFIS->Device);
	MV_ERROR_PRINT("\tFeatures=0x%02x, Features_exp=0x%02x\n", pFIS->Features, pFIS->Features_Exp);
	MV_ERROR_PRINT("\tLBA_low=0x%02x, LBA_mid=0x%02x, LBA_high=0x%02x\n", 
			pFIS->LBA_Low, pFIS->LBA_Mid, pFIS->LBA_High);
	MV_ERROR_PRINT("\tLBA_low_exp=0x%02x, LBA_mid_exp=0x%02x, LBA_high_exp=0x%02x\n", 
			pFIS->LBA_Low_Exp, pFIS->LBA_Mid_Exp, pFIS->LBA_High_Exp);
	MV_ERROR_PRINT("\tSector_count=0x%02x, Sector_count_exp=0x%02x\n", 
			pFIS->Sector_Count, pFIS->Sector_Count_Exp);
}

void Galois_ErrorDumpSGTable(PMV_SG_Table pSGTable)
{
	PMV_SG_Entry pSGEntry;
	MV_U32 i;
	MV_ERROR_PRINT("SG Table: size(0x%x)\n", pSGTable->Byte_Count);
	for ( i=0; i<pSGTable->Valid_Entry_Count; i++ )
	{
//		pSGEntry = &pSGTable->Entry[i];
		pSGEntry = &pSGTable->Entry_Ptr[i];
		MV_ERROR_PRINT("\t#%d entry(PRD): addr(0x%08x%08x), size(0x%x).\n", 
			i, pSGEntry->Base_Address_High, pSGEntry->Base_Address, pSGEntry->Size);
	}
}

static void Galois_ErrorDumpAll(PCore_Driver_Extension pCore)
{
	PDomain_Port pPort;
	PMV_Request pReq;
	PList_Head head = &pCore->Waiting_List;
	MV_U8 i, j;

	i = 0;
	MV_ERROR_PRINT("All commands in software waiting list:\n");
	for (pReq = LIST_ENTRY((head)->next, MV_Request, Queue_Pointer);	\
	     &pReq->Queue_Pointer != (head); 	\
	     pReq = LIST_ENTRY(pReq->Queue_Pointer.next, MV_Request, Queue_Pointer))
	{
		MV_ERROR_PRINT("%d(port=%d). ", i, pReq->Device_Id);
		Galois_ErrorDumpCmd(pReq);
		i++;
	}

	MV_ERROR_PRINT("\n");
	for (i=0; i<pCore->Port_Num; i++)
	{
		pPort = &pCore->Ports[i];
		MV_ERROR_PRINT("Port[%d] commands in hardware slots 0x%08x:\n", 
				i, MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_CMD_ISSUE));
		for (j=0; j<MAX_SLOT_NUMBER; j++)
		{
			if (pPort->Running_Req[j] != NULL)
			{
				MV_ERROR_PRINT("%d. ", j);
				//Galois_ErrorDumpCmd(pPort->Running_Req[i]);
			}
		}
		MV_ERROR_PRINT("\n");
	}
}

#if 0
void Port_HangSWRecover(MV_PVOID Port)
{
	PDomain_Port pPort = (PDomain_Port)Port;
	MV_U32 temp;

	/* CMD.ST=0 */	
	temp = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_CMD);
	temp &= ~PORT_CMD_START;
	MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_CMD, temp);
	temp = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_CMD); /* flush? */

	MV_PRINT("Waiting for clear CR (PORT_CMD=0x%08x): ", temp);;
//	while (MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_CMD) & PORT_CMD_LIST_ON) 
//		MV_PRINT(".");
	MV_PRINT("\n");

	/* clear SERR */	
	temp = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_SCR_ERR);
	MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_SCR_ERR, temp);

	/* clear irq status */
	temp = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_IRQ_STAT);
	MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_IRQ_STAT, temp);

	/* clear tfdata.bsy/drq register */
	temp = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_TFDATA);
	if ((temp & PORT_TF_STATUS_DRQ) 
			|| (temp & PORT_TF_STATUS_ERR) 
			|| (temp & PORT_TF_STATUS_BSY)) {
		MV_PRINT("Error: There's DRQ or ERR is TaskFileData[0x%08x]\n", temp);
	}

	/* CMD.ST=1 */
	temp = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_CMD);
	temp |= PORT_CMD_START;
	MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_CMD, temp );
	temp = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_CMD); /* flush? */
}
#endif

#if 1
static MV_U32 dbg_intstatus = 0;
static MV_U32 dbg_cmdissue = 0, prev_dbg_cmdissue = 0;
static MV_U32 dbg_scract = 0, prev_dbg_scract = 0;
#endif
static MV_VOID __core_req_timeout_handler(MV_PVOID data)
{
	PMV_Request req = (PMV_Request) data;
	PCore_Driver_Extension pCore;
	PDomain_Port pPort;
	PDomain_Device dev;
	PHBA_Extension phba;
	MV_U32 i;

	if ( NULL == req )
		return;

	pCore = HBA_GetModuleExtension(req->Cmd_Initiator, MODULE_CORE);
	pPort = &pCore->Ports[PATA_MapPortId(req->Device_Id)];
	dev = &pPort->Device[PATA_MapDeviceId(req->Device_Id)];
	phba = HBA_GetModuleExtension(req->Cmd_Initiator, MODULE_HBA);
	
	MV_ERROR_PRINT("\n[Berlin SATA error]Drive hangs for %d seconds.\n", req->timeout);

	/* sent out drive event to application */
	Galois_ErrorDumpCmd(req);
	galois_sata_event(GALOIS_EVENT_L2H_DRIVEHANG | (GALOIS_EVENT_FROM_PORT(pPort->Id)), pPort, dev);
	//Galois_ErrorDumpAll(pCore);
	Galois_ErrorDumpHBA(pCore, pPort);

	{
#ifdef GALOIS_HANG_DEBUG
		Core_ModuleMonitor(pCore);
#endif
#if 0
		mvRemovePortWaitingList(pCore, 0);
		Port_HangReturnRunningReq(pCore);
		MV_PRINT("\n");
#endif
#ifdef GALOIS_HANG_DEBUG
		MV_PRINT("(previous status) dbg_intstatus=0x%08x\n", dbg_intstatus);
		MV_PRINT("(previous status) dbg_cmdissue=0x%08x, prev_dbg_cmdissue=0x%08x\n", 
				dbg_cmdissue, prev_dbg_cmdissue);
		MV_PRINT("(previous status) dbg_scract=0x%08x, prev_dbg_scract=0x%08x\n", 
				dbg_scract, prev_dbg_scract);
		MV_DumpHBA(pCore, pPort);
#endif /* GALOIS_HANG_DEBUG */

		MV_PRINT("[Galois 1]Clear hanging commands and S/W resetting device.\n");
		//Port_HangSWRecover(pPort);
		hba_spin_lock_irq(&phba->lock);
		pPort->Setting &= ~PORT_SETTING_NCQ_RUNNING;

		if (!(SATA_SoftResetDevice(pPort, 0x0)))
		{
			MV_ERROR_PRINT("[Galois]SATA_SoftResetDevice failed.\n");
			if (SATA_PortResetDevice(pPort)) /* port reset success */
			{
				MV_ERROR_PRINT("[Galois]SATA_PortResetDevice(%d) passed.\n", pPort->Id);
				MV_ERROR_PRINT("\n[Berlin]Configure current hung device(%d)...\n", dev->Id);
				Core_ResetChannel((MV_PVOID)dev);
			}
			else /* port reset failure, then do HBA reset */
			{
				MV_ERROR_PRINT("[Galois]SATA_PortResetDevice(%d) failed, do HBA reset.\n", pPort->Id);
#ifdef GALOIS_COMRESET_HANG
				galois_sata_event(GALOIS_EVENT_L2H_DRIVERESET | (GALOIS_EVENT_FROM_PORT(pPort->Id)), pPort, dev);
				if (!SATA_ResetHost(pCore, GALOIS_SATA_ATOMIC))
					MV_ERROR_PRINT("[Galois]SATA_ResetHost failed.\n");
				SATA_InitHost(pCore, GALOIS_SATA_ATOMIC);
				MV_ERROR_PRINT("\n[Berlin]Configure all devices...\n");
				for( i=0; i<pCore->Port_Num; i++ )
				{
					PDomain_Port reset_port;
					PDomain_Device reset_dev;
					reset_port = &pCore->Ports[i];
					reset_dev = &reset_port->Device[0];
					Core_ResetChannel((MV_PVOID)reset_dev);
				}
#endif
			}
		}
		hba_spin_unlock_irq(&phba->lock);
#if 0
		MV_PRINT("[Galois]Clear device hanging commands.\n");
		value = MV_REG_READ_DWORD( portMmio, PORT_CMD );
		MV_REG_WRITE_DWORD( portMmio, PORT_CMD, value & ~MV_BIT(0));
		MV_REG_WRITE_DWORD( portMmio, PORT_CMD, value | MV_BIT(0));
		value = MV_REG_READ_DWORD(portMmio, PORT_TFDATA);
		if ((value & PORT_TF_STATUS_DRQ) || (value & PORT_TF_STATUS_ERR))
			MV_PRINT("Error: There's DRQ or ERR is TaskFileData[0x%08x]\n", value);
#endif
	}
}
#endif /* SUPPORT_ERROR_HANDLING && _OS_LINUX */


//TBD: In Dump. How many request can be? Not always 1.
MV_U32 Core_ModuleGetResourceQuota(enum Resource_Type type, MV_U16 maxIo)
{
	MV_U32 size = 0;
	MV_U8 sgEntryCount;
	
	/* Extension quota */
	if ( type==RESOURCE_CACHED_MEMORY )		
	{
		size = ROUNDING(sizeof(Core_Driver_Extension), 8);

		/* resource for SG Entry */
		if (maxIo==1)
			sgEntryCount = MAX_SG_ENTRY_REDUCED;
		else
			sgEntryCount = MAX_SG_ENTRY;
		size += sizeof(MV_SG_Entry) * sgEntryCount * INTERNAL_REQ_COUNT;

		size += sizeof(MV_Request) * INTERNAL_REQ_COUNT;
	       	return size;
	}
	
	/* Uncached memory quota */
	if ( type==RESOURCE_UNCACHED_MEMORY )
	{
		/* 
		 * SATA port alignment quota:
		 * Command list and received FIS is 64 byte aligned.
		 * Command table is 128 byte aligned.
		 * Data buffer is 8 byte aligned.
		 * This is different with AHCI.
		 */
        /* 
		 * PATA port alignment quota: Same with SATA.
		 * The only difference is that PATA doesn't have the FIS.
		 */
		MV_DPRINT(("Command List Size = 0x%x.\n", (MV_U32)SATA_CMD_LIST_SIZE));
		MV_DPRINT(("Received FIS Size = 0x%x.\n", (MV_U32)SATA_RX_FIS_SIZE));
		MV_DPRINT(("Command Table Size = 0x%x.\n", (MV_U32)SATA_CMD_TABLE_SIZE));
		MV_ASSERT(SATA_CMD_LIST_SIZE==ROUNDING(SATA_CMD_LIST_SIZE, 64));
		MV_ASSERT(SATA_RX_FIS_SIZE==ROUNDING(SATA_RX_FIS_SIZE, 64));
		MV_ASSERT(SATA_CMD_TABLE_SIZE==ROUNDING(SATA_CMD_TABLE_SIZE, 128));
		MV_ASSERT(SATA_SCRATCH_BUFFER_SIZE==ROUNDING(SATA_SCRATCH_BUFFER_SIZE, 8));
		if ( maxIo>1 )
		{
			size = 64 + SATA_CMD_LIST_SIZE*MAX_PORT_NUMBER;								/* Command List*/
			size += 64 + SATA_RX_FIS_SIZE*MAX_PORT_NUMBER;							/* Received FIS */
			size += 128 + SATA_CMD_TABLE_SIZE*MAX_SLOT_NUMBER*MAX_PORT_NUMBER;			/* Command Table */
			size += 8 + SATA_SCRATCH_BUFFER_SIZE*MAX_DEVICE_NUMBER;						/* Buffer for initialization like identify */
		}
		else
		{
			size = 64 + SATA_CMD_LIST_SIZE*MAX_PORT_NUMBER;
			size += 64 + SATA_RX_FIS_SIZE*MAX_PORT_NUMBER;
			size += 128 + SATA_CMD_TABLE_SIZE*MAX_PORT_NUMBER;
			size += 8 + SATA_SCRATCH_BUFFER_SIZE*MAX_DEVICE_NUMBER;
		}

		return size;
	}

	return 0;
}

//TBD: In Dump
void Core_ModuleInitialize(MV_PVOID This, MV_U32 extensionSize, MV_U16 maxIo)
{
	PCore_Driver_Extension pCore = (PCore_Driver_Extension)This;
	PMV_Request pReq;
	Assigned_Uncached_Memory dmaResource;
	PDomain_Port port;
	MV_PVOID memVir;
	MV_PHYSICAL_ADDR memDMA;
	Controller_Infor controller;
	MV_PTR_INTEGER temp, tmpSG;
	MV_U32 offset, internalReqSize;
	MV_U8 i,j, flagSaved, sgEntryCount;
	MV_U32 vsr_c[MAX_PORT_NUMBER];
	MV_U8 vsrSkipPATAPort = 0;

	gCore = pCore;

	flagSaved=pCore->VS_Reg_Saved;

	if(flagSaved==VS_REG_SIG)
	{
		for ( j=0; j<MAX_PORT_NUMBER; j++ )
		{
			port = &pCore->Ports[j];
			vsr_c[j]=port->VS_RegC;
		}
		/* Save the PATA Port detection skip flag */
		vsrSkipPATAPort = pCore->Flag_Fastboot_Skip & FLAG_SKIP_PATA_PORT;
	}

	/* 
	 * Zero core driver extension. After that, I'll ignore many variables initialization. 
	 */
	MV_ZeroMemory(This, extensionSize);

	if(flagSaved==VS_REG_SIG)
	{
		pCore->VS_Reg_Saved=flagSaved;

		for ( j=0; j<MAX_PORT_NUMBER; j++ )
		{
			port = &pCore->Ports[j];
			port->VS_RegC=vsr_c[j];
		}
		/* Restore the PATA Port detection skip flag */
		/* Only this flag should survive the S3 */
		/* The others should be kept as default (0) */
		pCore->Flag_Fastboot_Skip = vsrSkipPATAPort;
	}

	pCore->State = CORE_STATE_IDLE;

	/* Set up controller information */
	HBA_GetControllerInfor(pCore, &controller);

	pCore->Vendor_Id = controller.Vendor_Id;
	pCore->Device_Id = controller.Device_Id;
	pCore->Revision_Id = controller.Revision_Id;
	pCore->Base_Address = controller.Base_Address;
	pCore->Mmio_Base = controller.Base_Address;

	pCore->Adapter_State = ADAPTER_INITIALIZING;
	MV_LIST_HEAD_INIT(&pCore->Waiting_List);
	MV_LIST_HEAD_INIT(&pCore->Internal_Req_List);

	if ( maxIo==1 )
		pCore->Is_Dump = MV_TRUE;
	else
		pCore->Is_Dump = MV_FALSE;

	/* -Yufeng */
	pCore->PATA_Port_Num = 0;
#ifdef CONFIG_MV88DE3010_PLT_DISABLE2SATA
	pCore->SATA_Port_Num = 1;
	pCore->Port_Num = 1;
#else
	pCore->SATA_Port_Num = 2;
	pCore->Port_Num = 2;
#endif
	MV_DPRINT(("Galois SATA found.\n"));

	if (pCore->Is_Dump)
		sgEntryCount = MAX_SG_ENTRY_REDUCED;
	else
		sgEntryCount = MAX_SG_ENTRY;

	tmpSG = (MV_PTR_INTEGER)This + ROUNDING(sizeof(Core_Driver_Extension),8);
	temp = 	tmpSG + sizeof(MV_SG_Entry) * sgEntryCount * INTERNAL_REQ_COUNT;

	internalReqSize = MV_REQUEST_SIZE * INTERNAL_REQ_COUNT;
	MV_ASSERT( extensionSize >= ROUNDING(sizeof(Core_Driver_Extension),8) + internalReqSize );
	for ( i=0; i<INTERNAL_REQ_COUNT; i++ )
	{
		pReq = (PMV_Request)temp;
		pReq->SG_Table.Entry_Ptr = (PMV_SG_Entry)tmpSG;
		pReq->SG_Table.Max_Entry_Count = sgEntryCount;
		List_AddTail(&pReq->Queue_Pointer, &pCore->Internal_Req_List);
		tmpSG += sizeof(MV_SG_Entry) * sgEntryCount;
		temp += MV_REQUEST_SIZE;	/* MV_Request is 64bit aligned. */
	}	

	/* Init port data structure */
	for ( i=0; i<pCore->Port_Num; i++ )
	{
		port = &pCore->Ports[i];
		
		port->Id = i;
		port->Port_State = PORT_STATE_IDLE;

		port->Core_Extension = pCore;
		port->Mmio_Base = (MV_PU8)pCore->Mmio_Base + 0x100 + (i * 0x80);
		port->Mmio_SCR = (MV_PU8)port->Mmio_Base + PORT_SCR;

		Tag_Init(&port->Tag_Pool, MAX_TAG_NUMBER);

		for (j=0; j<MAX_DEVICE_PER_PORT; j++) 
		{
			port->Device[j].Id = i*MAX_DEVICE_PER_PORT + j;
			port->Device[j].PPort = port;
			port->Device[j].Reset_Count = 0;
		}

		port->Device_Number = 0;

		//TBD: Set function table for each port here.
		port->Type = PORT_TYPE_SATA;

		port->port_read10_to_read12 = 0;
		port->port_marvell_drive = 0;
		port->port_flashless_drive = 0;
	}

	/* Get uncached memory */
	HBA_GetResource(pCore, RESOURCE_UNCACHED_MEMORY, &dmaResource);
	memVir = dmaResource.Virtual_Address;
	memDMA = dmaResource.Physical_Address;
	
	/* Assign uncached memory for command list (64 byte align) */
	offset = (MV_U32)(ROUNDING(memDMA.value,64)-memDMA.value);
	memDMA.value += offset;
	memVir = (MV_PU8)memVir + offset;
	for ( i=0; i<pCore->Port_Num; i++ )
	{
		port = &pCore->Ports[i];
		port->Cmd_List = memVir;
		port->Cmd_List_DMA = memDMA;
		memVir = (MV_PU8)memVir + SATA_CMD_LIST_SIZE;
		memDMA.value += SATA_CMD_LIST_SIZE;
	}

	/* Assign uncached memory for received FIS (64 byte align) */
	offset = (MV_U32)(ROUNDING(memDMA.value,64)-memDMA.value);
	memDMA.value += offset;
	memVir = (MV_PU8)memVir + offset;
	for ( i=0; i<pCore->SATA_Port_Num; i++ )
	{
		port = &pCore->Ports[i];	
		port->RX_FIS = memVir;
		port->RX_FIS_DMA = memDMA;
		memVir = (MV_PU8)memVir + SATA_RX_FIS_SIZE;
		memDMA.value += SATA_RX_FIS_SIZE;
	}

	/* Assign the 32 command tables. (128 byte align) */
	offset = (MV_U32)(ROUNDING(memDMA.value,128)-memDMA.value);
	memDMA.value += offset;
	memVir = (MV_PU8)memVir + offset;
	for ( i=0; i<pCore->Port_Num; i++ )
	{
		port = &pCore->Ports[i];
		port->Cmd_Table = memVir;
		port->Cmd_Table_DMA = memDMA;

		if ( !pCore->Is_Dump )
		{
			memVir = (MV_PU8)memVir + SATA_CMD_TABLE_SIZE * MAX_SLOT_NUMBER;
			memDMA.value += SATA_CMD_TABLE_SIZE * MAX_SLOT_NUMBER;
		}
		else
		{
			memVir = (MV_PU8)memVir + SATA_CMD_TABLE_SIZE;
			memDMA.value += SATA_CMD_TABLE_SIZE;
		}
	}

	/* Assign the scratch buffer (8 byte align) */
	offset = (MV_U32)(ROUNDING(memDMA.value,8)-memDMA.value);
	memDMA.value += offset;
	memVir = (MV_PU8)memVir + offset;
	for ( i=0; i<pCore->Port_Num; i++ )
	{
		port = &pCore->Ports[i];
		for ( j=0; j<MAX_DEVICE_PER_PORT; j++ )
		{
			port->Device[j].Scratch_Buffer = memVir;
			port->Device[j].Scratch_Buffer_DMA = memDMA;
		
			memVir = (MV_PU8)memVir + SATA_SCRATCH_BUFFER_SIZE;
			memDMA.value += SATA_SCRATCH_BUFFER_SIZE;
		}
	}

	/* Let me confirm the following assumption */
	MV_ASSERT( sizeof(SATA_FIS_REG_H2D)==sizeof(MV_U32)*FIS_REG_H2D_SIZE_IN_DWORD );
	MV_ASSERT( sizeof(MV_Command_Table)==0x80+MAX_SG_ENTRY*sizeof(MV_SG_Entry) );
	MV_ASSERT( sizeof(ATA_Identify_Data)==512 ); 
	MV_ASSERT( MAX_TAG_NUMBER==MAX_SLOT_NUMBER );
}

void Core_ModuleStart(MV_PVOID This)
{
	PCore_Driver_Extension pCore = (PCore_Driver_Extension)This;

	mvAdapterStateMachine(pCore, GALOIS_SATA_SLEEP);
}

void Core_ModuleShutdown(MV_PVOID This)
{
	/* 
	 * This function is equivalent to ahci_port_stop 
	 */
	PCore_Driver_Extension pCore = (PCore_Driver_Extension)This;
	MV_U32 tmp, i;
	MV_LPVOID mmio;
	for ( i=0; i<pCore->Port_Num; i++ )
	{
		mmio = pCore->Ports[i].Mmio_Base;

		tmp = MV_REG_READ_DWORD(mmio, PORT_CMD);
		if ( pCore->Ports[i].Type==PORT_TYPE_SATA )
			tmp &= ~(PORT_CMD_START | PORT_CMD_FIS_RX);
		else
			tmp &= ~PORT_CMD_START;
		MV_REG_WRITE_DWORD(mmio, PORT_CMD, tmp);
		MV_REG_READ_DWORD(mmio, PORT_CMD); /* flush */

		/* 
		 * spec says 500 msecs for each PORT_CMD_{START,FIS_RX} bit, so
		 * this is slightly incorrect.
		 */
		HBA_SleepMillisecond(pCore, 500);
	}

	/* Disable the controller interrupt */
	tmp = MV_REG_READ_DWORD(pCore->Mmio_Base, HOST_CTL);
	tmp &= ~(HOST_IRQ_EN);
	MV_REG_WRITE_DWORD(pCore->Mmio_Base, HOST_CTL, tmp);
}

void Core_ModuleNotification(MV_PVOID This, enum Module_Event event, MV_PVOID event_param)
{
	;
}

/*
 * Called by HBA_HandleWaitingList to process request
 */
void Core_ModuleSendRequest(MV_PVOID This, PMV_Request pReq)
{	
	Core_InternalSendRequest(This, pReq);
}

void Core_InternalSendRequest(MV_PVOID This, PMV_Request pReq)
{
	PCore_Driver_Extension pCore = (PCore_Driver_Extension)This;
	/* Check whether we can handle this request */
	switch (pReq->Cdb[0])
	{
		case SCSI_CMD_INQUIRY:
		case SCSI_CMD_START_STOP_UNIT:
		case SCSI_CMD_TEST_UNIT_READY:
		case SCSI_CMD_READ_12:
		case SCSI_CMD_READ_10:
		case SCSI_CMD_WRITE_10:
		case SCSI_CMD_VERIFY_10:
		case SCSI_CMD_READ_CAPACITY_10:
		case SCSI_CMD_REQUEST_SENSE:
		case SCSI_CMD_MODE_SELECT_10:
		case SCSI_CMD_MODE_SENSE_10:
		case SCSI_CMD_MARVELL_SPECIFIC:
		default:
			if ( pReq->Cmd_Initiator==pCore ) /* from CompleteRequest */
			{
				if ( !SCSI_IS_READ(pReq->Cdb[0]) && !SCSI_IS_WRITE(pReq->Cdb[0]) )
				{
					/* Reset request or request sense command. */
					List_Add(&pReq->Queue_Pointer, &pCore->Waiting_List);//Add to the header
				}
				else
				{
					MV_ASSERT(MV_FALSE);
				}
			}
			else
			{
				List_AddTail(&pReq->Queue_Pointer, &pCore->Waiting_List);//Append to the tail
			}
			Core_HandleWaitingList(pCore);
			break;
	}
}

void SATA_PrepareCommandHeader(PDomain_Port pPort, PMV_Request pReq, MV_U8 tag)
{
	MV_PHYSICAL_ADDR table_addr;
	PMV_Command_Header header = NULL;
	PMV_SG_Table pSGTable = &pReq->SG_Table;
	//PDomain_Device pDevice = &pPort->Device[PATA_MapDeviceId(pReq->Device_Id)];

	header = SATA_GetCommandHeader(pPort, tag);
	/* 
	 * Set up the command header.
	 * TBD: Table_Address and Table_Address_High are fixed. Needn't set every time.
	 */
	header->FIS_Length = FIS_REG_H2D_SIZE_IN_DWORD;
	header->Packet_Command = (pReq->Cmd_Flag&CMD_FLAG_PACKET)?1:0;
	header->Reset = 0;
	header->NCQ = (pReq->Cmd_Flag&CMD_FLAG_NCQ)?1:0;

	header->PM_Port = 0;

	*((MV_U16 *) header) = CPU_TO_LE_16( *((MV_U16 *) header) );
	header->PRD_Entry_Count = CPU_TO_LE_16(pSGTable->Valid_Entry_Count);
#ifdef GALOIS_NCQ_PRDTL_HACK
	header->PRD_Entry_Count = 1;
#endif

	table_addr.low = pPort->Cmd_Table_DMA.low + SATA_CMD_TABLE_SIZE*tag;
	MV_ASSERT(table_addr.low>=pPort->Cmd_Table_DMA.low);	//TBD
	table_addr.high = pPort->Cmd_Table_DMA.high;

	header->Table_Address = CPU_TO_LE_32(table_addr.low);
	header->Table_Address_High = CPU_TO_LE_32(table_addr.high);
}

/*
 * Fill SATA command table
 */
MV_VOID SATA_PrepareCommandTable(
	PDomain_Port pPort, 
	PMV_Request pReq, 
	MV_U8 tag,
	PATA_TaskFile pTaskFile
	)
{
	PMV_Command_Table pCmdTable = Port_GetCommandTable(pPort, tag);

	PMV_SG_Table pSGTable = &pReq->SG_Table;
	PMV_SG_Entry pSGEntry = NULL;
#ifdef GALOIS_NCQ_PRDTL_HACK
	MV_U32 trans_size = 0;
#endif
	MV_U8 i;

	/* Step 1: fill the command FIS: MV_Command_Table */
	SCSI_To_FIS(pPort->Core_Extension, pReq, tag, pTaskFile);

	/* Step 2. fill the ATAPI CDB */
	if ( pReq->Cmd_Flag&CMD_FLAG_PACKET )
	{
		MV_CopyMemory(pCmdTable->ATAPI_CDB, pReq->Cdb, MAX_CDB_SIZE);
	}

	/* Step 3: fill the PRD Table if necessary. */
	if ( (pSGTable) && (pSGTable->Valid_Entry_Count) )
	{
		/* "Transfer Byte Count" in AHCI and 614x PRD table is zero based. */
		for ( i=0; i<pSGTable->Valid_Entry_Count; i++ )
		{
			pSGEntry = &pCmdTable->PRD_Entry[i];
			pSGEntry->Base_Address = CPU_TO_LE_32(pSGTable->Entry_Ptr[i].Base_Address);
			pSGEntry->Base_Address_High = CPU_TO_LE_32(pSGTable->Entry_Ptr[i].Base_Address_High);
			pSGEntry->Size = CPU_TO_LE_32(pSGTable->Entry_Ptr[i].Size-1);
#ifdef GALOIS_NCQ_PRDTL_HACK
			trans_size += CPU_TO_LE_32(pSGTable->Entry_Ptr[i].Size);
#endif
		}
#ifdef GALOIS_NCQ_PRDTL_HACK
		pCmdTable->PRD_Entry[0].Size = trans_size - 1;
#endif
	}
	else
	{
		MV_DASSERT( !SCSI_IS_READ(pReq->Cdb[0]) && !SCSI_IS_WRITE(pReq->Cdb[0]) );
	}
}

void SATA_SendFrame(PDomain_Port pPort, PMV_Request pReq, MV_U8 tag)
{
	MV_LPVOID portMmio = pPort->Mmio_Base;

	MV_DASSERT( (pPort->Running_Slot&(1<<tag))==0 );
	MV_DASSERT( pPort->Running_Req[tag]==0 );
	MV_DASSERT( (MV_REG_READ_DWORD(portMmio, PORT_CMD_ISSUE)&(1<<tag))==0 );
	MV_DASSERT( (MV_REG_READ_DWORD(portMmio, PORT_SCR_ACT)&(1<<tag))==0 );

	pPort->Running_Slot |= 1<<tag;
	pPort->Running_Req[tag] = pReq;

	if ( pReq->Cmd_Flag&CMD_FLAG_NCQ )
		pPort->Setting |= PORT_SETTING_NCQ_RUNNING;
	else
		pPort->Setting &= ~PORT_SETTING_NCQ_RUNNING;

	if ( pReq->Scsi_Status==REQ_STATUS_RETRY )
	{
		MV_PRINT("Retry request...");
		MV_DumpRequest(pReq, MV_FALSE);
		pPort->Setting |= PORT_SETTING_DURING_RETRY;
	}
	else
	{
		pPort->Setting &= ~PORT_SETTING_DURING_RETRY;
	}

	if ( pPort->Setting&PORT_SETTING_NCQ_RUNNING )
	{
		MV_REG_WRITE_DWORD(portMmio, PORT_SCR_ACT, 1<<tag);
		MV_REG_READ_DWORD(portMmio, PORT_SCR_ACT);	/* flush */
	}

	MV_REG_WRITE_DWORD(portMmio, PORT_CMD_ISSUE, 1<<tag);
	MV_REG_READ_DWORD(portMmio, PORT_CMD_ISSUE);	/* flush */
}

//Hardware Reset. Added by Lily

MV_BOOLEAN Core_WaitingForIdle(MV_PVOID pExtension)
{
	PCore_Driver_Extension pCore = (PCore_Driver_Extension)pExtension;
	PDomain_Port pPort = NULL;
	MV_U8 i;

	for ( i=0; i<pCore->Port_Num; i++ )
	{
		pPort = &pCore->Ports[i];

		if ( pPort->Running_Slot!=0 )
			return MV_FALSE;
	}
	
	return MV_TRUE;
}

#if 0
//TBD: Replace this function with existing functions.
void Core_ResetHardware(MV_PVOID pExtension)
{
	PCore_Driver_Extension pCore = (PCore_Driver_Extension)pExtension;
	MV_U32 i, j;
	PDomain_Port pPort = NULL;
	PDomain_Device pDevice = NULL;

	/* Re-initialize some variables to make the reset go. */
	//TBD: Any more variables?
	pCore->Adapter_State = ADAPTER_INITIALIZING;
	for ( i=0; i<MAX_PORT_NUMBER; i++ )
	{
		pPort = &pCore->Ports[i];
		pPort->Port_State = PORT_STATE_IDLE;
		for ( j=0; j<MAX_DEVICE_PER_PORT; j++ )
		{
			pDevice = &pPort->Device[j];
			pDevice->State = DEVICE_STATE_IDLE;
		}
	}

	/* Go through the mvAdapterStateMachine. */
	if( pCore->Resetting==0 )
	{
		pCore->Resetting = 1;
		if( !mvAdapterStateMachine(pCore) )
		{
			MV_ASSERT(MV_FALSE);
		}
	}
	else
	{
		/* I suppose that we only have one chance to call Core_ResetHardware. */
		MV_DASSERT(MV_FALSE);
	}
	
	return;
}
#endif

void Core_FillSenseData(PMV_Request pReq, MV_U8 senseKey, MV_U8 adSenseCode)
{
	if (pReq->Sense_Info_Buffer != NULL) {
		((MV_PU8)pReq->Sense_Info_Buffer)[0] = 0x70;	/* Current */
		((MV_PU8)pReq->Sense_Info_Buffer)[2] = senseKey;
		((MV_PU8)pReq->Sense_Info_Buffer)[7] = 0;		/* additional sense length */
		((MV_PU8)pReq->Sense_Info_Buffer)[12] = adSenseCode;	/* additional sense code */
	}
}

void mvScsiInquiry(PCore_Driver_Extension pCore, PMV_Request pReq)
{
	PDomain_Device pDevice = NULL;
	MV_U8 portId, deviceId;

	portId = PATA_MapPortId(pReq->Device_Id);
	deviceId = PATA_MapDeviceId(pReq->Device_Id);
	pDevice = &pCore->Ports[portId].Device[deviceId];
	MV_ZeroMemory(pReq->Data_Buffer, pReq->Data_Transfer_Length);

	if ( pReq->Cdb[1] & CDB_INQUIRY_EVPD )
	{
		MV_U8 MV_INQUIRY_VPD_PAGE0_DATA[6] = {0x00, 0x00, 0x00, 0x02, 0x00, 0x80};
		MV_U32 tmpLen = 0;
		pReq->Scsi_Status = REQ_STATUS_SUCCESS;

		/* Shall return the specific page of Vital Production Data */
		switch (pReq->Cdb[2]) {
		case 0x00:	/* Supported VPD pages */
			tmpLen = MV_MIN(pReq->Data_Transfer_Length, 6);
			MV_CopyMemory(pReq->Data_Buffer, MV_INQUIRY_VPD_PAGE0_DATA, tmpLen);
			break;
		case 0x80:	/* Unit Serial Number VPD Page */
			if (pReq->Data_Transfer_Length > 1)
				*(((MV_PU8)(pReq->Data_Buffer)) + 1) = 0x80;
			tmpLen = MV_MIN(pReq->Data_Transfer_Length, 4);
			if (tmpLen >= 4) {
				tmpLen = MV_MIN((pReq->Data_Transfer_Length-4), 20);
				MV_CopyMemory(((MV_PU8)(pReq->Data_Buffer)+4), pDevice->Serial_Number, tmpLen);
				*(((MV_PU8)(pReq->Data_Buffer)) + 3) = (MV_U8)tmpLen;
				tmpLen += 4;
			}
			break;
		case 0x83:	/* Device Identification VPD Page */
			/* Here is using Vendor Specific Identifier Format */
			if (pReq->Data_Transfer_Length > 8) {
				*(((MV_PU8)(pReq->Data_Buffer)) + 1) = 0x83;
				*(((MV_PU8)(pReq->Data_Buffer)) + 4) = 0x02;	/* Code Set */
				tmpLen = MV_MIN((pReq->Data_Transfer_Length-8), 40);
				MV_CopyMemory(((MV_PU8)(pReq->Data_Buffer)+8), pDevice->Model_Number, tmpLen);
				*(((MV_PU8)(pReq->Data_Buffer)) + 7) = (MV_U8)tmpLen;	/* Identifier Length */
				*(((MV_PU8)(pReq->Data_Buffer)) + 3) = (MV_U8)(tmpLen + 4);	/* Page Length */
			}
			tmpLen += 8;
			break;
		default:
			pReq->Scsi_Status = REQ_STATUS_HAS_SENSE;
			Core_FillSenseData(pReq, SCSI_SK_ILLEGAL_REQUEST, SCSI_ASC_INVALID_FEILD_IN_CDB);
			break;
		}
		pReq->Data_Transfer_Length = tmpLen;
	} 
	else
	{
		/* Standard inquiry */
		if (pReq->Cdb[2]!=0) {
			/* PAGE CODE field must be zero when EVPD is zero for a valid request */
			/* sense key as ILLEGAL REQUEST and additional sense code as INVALID FIELD IN CDB */
			pReq->Scsi_Status = REQ_STATUS_HAS_SENSE;
			Core_FillSenseData(pReq, SCSI_SK_ILLEGAL_REQUEST, SCSI_ASC_INVALID_FEILD_IN_CDB);
			return;
		}
		
		if ( (portId>=pCore->Port_Num)||(deviceId>=MAX_DEVICE_PER_PORT) )
		{
			pReq->Scsi_Status = REQ_STATUS_NO_DEVICE;
		}
		else
		{
//			pDevice = &pCore->Ports[portId].Device[deviceId];
			if ( pDevice->Status&DEVICE_STATUS_FUNCTIONAL )
			{
#if 0
				#define STANDARD_INQUIRY_DATA_SIZE	36
				MV_U8 MV_INQUIRY_DATA[STANDARD_INQUIRY_DATA_SIZE] = {
					0, 0, 0x02, 0x02, STANDARD_INQUIRY_DATA_SIZE - 5, 0, 0, 0x13,
					'M', 'a', 'r', 'v', 'e', 'l', 'l', ' ',
					'P', 'r', 'o', 'd ', 'u', 'c', 't', ' ', 'I', 'd', 'e', 'n', 't', 'i', 'f', 'c',
					'1', '.', '0', '1'};

				MV_CopyMemory( pReq->Data_Buffer, 
								MV_INQUIRY_DATA, 
								MV_MIN(pReq->Data_Transfer_Length, STANDARD_INQUIRY_DATA_SIZE)
								);
#else
				{
					MV_U8 Vendor[9],Product[17], temp[24];
				    MV_U8 buff[42];
					MV_U32 inquiryLen;
					MV_ZeroMemory(buff, 42);
					
					if (pDevice->Device_Type & \
					    DEVICE_TYPE_ATAPI) {
						buff[0] = 0x05;
						buff[1] = 0x00 | 1U<<7; 
					} else {
						buff[0] = 0;
						buff[1] = 0;
					}

					buff[2] = 0x05;   //TBD 3  /*claim conformance to SCSI-3*/
					buff[3] = 0x12;    /* set RESPONSE DATA FORMAT to 2*/
					buff[4] = 42 - 5;
					buff[6] = 0x0;     /* tagged queuing*/
					buff[7] = 0X13;	//TBD 2;

					MV_CopyMemory(temp, pDevice->Model_Number, 24);
					{
						MV_U32 i;
						for (i = 0; i < 9; i++)
						{
							if (temp[i] == ' ')
							{
								break;
							}
						}
						if (i == 9)
						{
							if (((temp[0] == 'I') && (temp[1] == 'C')) ||
								((temp[0] == 'H') && (temp[1] == 'T')) ||
								((temp[0] == 'H') && (temp[1] == 'D')) ||
								((temp[0] == 'D') && (temp[1] == 'K')))
							{ /*Hitachi*/
								Vendor[0] = 'H';
								Vendor[1] = 'i';
								Vendor[2] = 't';
								Vendor[3] = 'a';
								Vendor[4] = 'c';
								Vendor[5] = 'h';
								Vendor[6] = 'i';
								Vendor[7] = ' ';
								Vendor[8] = '\0';
							}
							else if ((temp[0] == 'S') && (temp[1] == 'T'))
							{
								/*Seagate*/
								Vendor[0] = 'S';
								Vendor[1] = 'e';
								Vendor[2] = 'a';
								Vendor[3] = 'g';
								Vendor[4] = 'a';
								Vendor[5] = 't';
								Vendor[6] = 'e';
								Vendor[7] = ' ';
								Vendor[8] = '\0';
							}
							else
							{
								/*Unkown*/
								Vendor[0] = 'A';
								Vendor[1] = 'T';
								Vendor[2] = 'A';
								Vendor[3] = ' ';
								Vendor[4] = ' ';
								Vendor[5] = ' ';
								Vendor[6] = ' ';
								Vendor[7] = ' ';
								Vendor[8] = '\0';
							}
							MV_CopyMemory(Product, temp, 16);
							Product[16] = '\0';
						}
						else
						{
							MV_U32 j = i;
							MV_CopyMemory(Vendor, temp, j);
							for (; j < 9; j++)
							{
								Vendor[j] = ' ';
							}
							Vendor[8] = '\0';
							for (; i < 24; i++)
							{
								if (temp[i] != ' ')
								{
									break;
								}
							}
							MV_CopyMemory(Product, &temp[i], 24 - i);
							Product[16] = '\0';
						}
						MV_CopyMemory(&buff[8], Vendor, 8);
						MV_CopyMemory(&buff[16], Product, 16);
						MV_CopyMemory(&buff[32], pDevice->Firmware_Revision, 4);
					}
					MV_CopyMemory(&buff[36], "MVSATA", 6);

					/*buff[32] = '3';*/

					inquiryLen = 42;
					MV_CopyMemory( pReq->Data_Buffer, 
								buff, 
								MV_MIN(pReq->Data_Transfer_Length, inquiryLen)
								);
					pReq->Data_Transfer_Length = MV_MIN(pReq->Data_Transfer_Length, inquiryLen);
				}
#endif
				pReq->Scsi_Status = REQ_STATUS_SUCCESS;
			}
			else
			{
				pReq->Scsi_Status = REQ_STATUS_NO_DEVICE;
			}
		}
	}
}

#define MAX_MODE_PAGE_LENGTH	28
MV_U32 Core_get_mode_page_caching(MV_PU8 pBuf, PDomain_Device pDevice)
{
	pBuf[0] = 0x08;		/* Page Code, PS = 0; */
	pBuf[1] = 0x12;		/* Page Length */
	/* set the WCE and RCD bit based on device identification data */
	if (pDevice->Setting & DEVICE_SETTING_WRITECACHE_ENABLED)
		pBuf[2] |= MV_BIT(2);
	pBuf[3] = 0;	/* Demand read/write retention priority */
	pBuf[4] = 0xff;	/* Disable pre-fetch trnasfer length (4,5) */
	pBuf[5] = 0xff;	/* all anticipatory pre-fetching is disabled */
	pBuf[6] = 0;	/* Minimum pre-fetch (6,7) */
	pBuf[7] = 0;
	pBuf[8] = 0;	/* Maximum pre-fetch (8,9) */
	pBuf[9] = 0x01;
	pBuf[10] = 0;	/* Maximum pre-fetch ceiling (10,11) */
	pBuf[11] = 0x01;
//	pBuf[12] |= MV_BIT(5);	/* How do I know if Read Ahead is enabled or disabled???  */
	pBuf[12] = 0x00;
	pBuf[13] = 0x01;	/* Number of cache segments */
	pBuf[14] = 0xff;	/* Cache segment size (14, 15) */
	pBuf[15] = 0xff;
	return 0x14;	/* Total page length in byte */
}

void mvScsiModeSense(PCore_Driver_Extension pCore, PMV_Request pReq)
{
	MV_U8 pageCode = pReq->Cdb[2] & 0x3F;		/* Same for mode sense 6 and 10 */
	MV_U8 ptmpBuf[MAX_MODE_PAGE_LENGTH];
	MV_U32 pageLen = 0, tmpLen = 0;
	PDomain_Device pDevice = NULL;
	MV_U8 portId, deviceId;
	MV_U8 *buf = pReq->Data_Buffer;

	portId = PATA_MapPortId(pReq->Device_Id);
	deviceId = PATA_MapDeviceId(pReq->Device_Id);
	pDevice = &pCore->Ports[portId].Device[deviceId];

	MV_ZeroMemory(buf, pReq->Data_Transfer_Length);
	MV_ZeroMemory(ptmpBuf, MAX_MODE_PAGE_LENGTH);
	/* Block Descriptor Length set to 0 - No Block Descriptor */

	switch (pageCode) {
	case 0x3F:		/* Return all pages */
	case 0x08:		/* Caching mode page */
		if (pReq->Cdb[0]==SCSI_CMD_MODE_SENSE_6) {
			pageLen = Core_get_mode_page_caching((ptmpBuf+4), pDevice);
			ptmpBuf[0] = (MV_U8)(4 + pageLen - 1);	/* Mode data length */
			ptmpBuf[2] = 0x10;
			tmpLen = MV_MIN(pReq->Data_Transfer_Length, (pageLen+4));
		}
		else {	/* Mode Sense 10 */
			pageLen = Core_get_mode_page_caching((ptmpBuf+8), pDevice);
			/* Mode Data Length, it does not include the number of bytes in */
			/* Mode Data Length field */
			tmpLen = 8 + pageLen - 2;
			ptmpBuf[0] = (MV_U8)(((MV_U16)tmpLen) >> 8);
			ptmpBuf[1] = (MV_U8)tmpLen;
			ptmpBuf[2] = 0x00;
			ptmpBuf[3] = 0x10;
			tmpLen = MV_MIN(pReq->Data_Transfer_Length, (pageLen+8));
		}
		MV_CopyMemory(buf, ptmpBuf, tmpLen);
		pReq->Data_Transfer_Length = tmpLen;
		pReq->Scsi_Status = REQ_STATUS_SUCCESS;
		break;
	default:
		pReq->Scsi_Status = REQ_STATUS_HAS_SENSE;
		Core_FillSenseData(pReq, SCSI_SK_ILLEGAL_REQUEST, SCSI_ASC_INVALID_FEILD_IN_CDB);
		break;
	}
}

void mvScsiReportLun(PCore_Driver_Extension pCore, PMV_Request pReq)
{
	MV_U32 allocLen, lunListLen;
	MV_PU8 pBuf = pReq->Data_Buffer;

	allocLen = ((MV_U32)(pReq->Cdb[6] << 24)) |
			   ((MV_U32)(pReq->Cdb[7] << 16)) |
			   ((MV_U32)(pReq->Cdb[8] << 8)) |
			   ((MV_U32)(pReq->Cdb[9]));

	/* allocation length should not less than 16 bytes */
	if (allocLen < 16) {
		pReq->Scsi_Status = REQ_STATUS_HAS_SENSE;
		Core_FillSenseData(pReq, SCSI_SK_ILLEGAL_REQUEST, SCSI_ASC_INVALID_FEILD_IN_CDB);
		return;
	}

	MV_ZeroMemory(pBuf, pReq->Data_Transfer_Length);
	/* Only LUN 0 has device */
	lunListLen = 8;
	pBuf[0] = (MV_U8)((lunListLen & 0xFF000000) >> 24);
	pBuf[1] = (MV_U8)((lunListLen & 0x00FF0000) >> 16);
	pBuf[2] = (MV_U8)((lunListLen & 0x0000FF00) >> 8);
	pBuf[3] = (MV_U8)(lunListLen & 0x000000FF);
	pReq->Scsi_Status = REQ_STATUS_SUCCESS;
}

void mvScsiReadCapacity(PCore_Driver_Extension pCore, PMV_Request pReq)
{
	PDomain_Device pDevice = NULL;
	MV_LBA maxLBA;
	MV_U32 blockLength;
	MV_PU32 pU32Buffer;
	MV_U8 portId, deviceId;

	portId = PATA_MapPortId(pReq->Device_Id);
	deviceId = PATA_MapDeviceId(pReq->Device_Id);
#ifndef SECTOR_SIZE
	#define SECTOR_SIZE	512	//TBD
#endif

	MV_DASSERT( portId < MAX_PORT_NUMBER );

	if ((pReq->Cdb[8] & MV_BIT(1)) == 0)
	{
		if ( pReq->Cdb[2] || pReq->Cdb[3] || pReq->Cdb[4] || pReq->Cdb[5] )
		{
			pReq->Scsi_Status = REQ_STATUS_INVALID_REQUEST;
			return;
		}
	}

    /* 
	 * The disk size as indicated by the ATA spec is the total addressable
     * sectors on the drive ; while the SCSI translation of the command
     * should be the last addressable sector.
     */
	pDevice = &pCore->Ports[portId].Device[deviceId];
	maxLBA.value = pDevice->Max_LBA.value-1;
	blockLength = SECTOR_SIZE;			//TBD
	pU32Buffer = (MV_PU32)pReq->Data_Buffer;
	MV_ASSERT(maxLBA.high==0);	//TBD: Support Read Capactiy 16 

	pU32Buffer[0] = CPU_TO_BIG_ENDIAN_32(maxLBA.low);
	pU32Buffer[1] = CPU_TO_BIG_ENDIAN_32(blockLength);

	pReq->Scsi_Status = REQ_STATUS_SUCCESS;
}

MV_BOOLEAN Core_IsInternalRequest(PCore_Driver_Extension pCore, 
				  PMV_Request pReq)
{
	PDomain_Device pDevice;
	MV_U8 portId = PATA_MapPortId(pReq->Device_Id);
	MV_U8 deviceId = PATA_MapDeviceId(pReq->Device_Id);

	if ( portId>=MAX_PORT_NUMBER )
		return MV_FALSE;
	if ( deviceId>=MAX_DEVICE_PER_PORT )
		return MV_FALSE;

	pDevice = &pCore->Ports[portId].Device[deviceId];
	if ( pReq==pDevice->Internal_Req ) 
		return MV_TRUE;
	else
		return MV_FALSE;
}

void Port_Monitor(PDomain_Port pPort);
#if defined(SUPPORT_ERROR_HANDLING)
void Core_ResetChannel(MV_PVOID Device)
{
	PDomain_Device pDevice = (PDomain_Device)Device;
	PDomain_Port pPort = pDevice->PPort;
	PCore_Driver_Extension pCore = pPort->Core_Extension;
	PMV_Request pReq;
	MV_LPVOID portMmio = pPort->Mmio_Base;
	MV_U32 tmp;
	MV_U8 i;
#ifdef BERLIN_POWER_OFFON_DRIVE
	unsigned int status;
	unsigned int old_inten, old_intsts;
	unsigned int signature;
#endif

	MV_DPRINT(("[Core_ResetChannel]Request time out. Resetting channel %d.\n", pPort->Id));
#ifdef SUPPORT_EVENT
	HBA_AddEvent( pCore, EVT_ID_HD_TIMEOUT, pDevice->Id, 3, 0, NULL );
#endif /* SUPPORT_EVENT */

	/* toggle the start bit in cmd register */
	tmp = MV_REG_READ_DWORD( portMmio, PORT_CMD );
	MV_REG_WRITE_DWORD( portMmio, PORT_CMD, tmp & ~MV_BIT(0));
	MV_REG_WRITE_DWORD( portMmio, PORT_CMD, tmp | MV_BIT(0));
	HBA_DelayMillisecond( pCore, 100 );

//	Port_Monitor( pPort );
	pDevice->Reset_Count++;

	/* Whether it's during reset, we got reset again. */
	if ( pPort->Port_State!=PORT_STATE_INIT_DONE )
	{
		MV_ERROR_PRINT("[Berlin]Timeout during last reset, drive has been dead.\n");
		galois_sata_event(GALOIS_EVENT_L2H_DEADDRIVE | (GALOIS_EVENT_FROM_PORT(pPort->Id)), pPort, pDevice);
		MV_ERROR_PRINT("[Berlin]return all OS requests.\n");
		mvRemoveDeviceWaitingList(pCore, pDevice->Id, MV_TRUE);
		/* XXX:how to tell kernel that I'm gone. */
		if (pDevice->Status & DEVICE_STATUS_FUNCTIONAL)
			SATA_PortReportNoDevice(pCore, pPort); /* remove scd */
	}

	/* put all the running requests back into waiting list */
	for ( i=0; i<MAX_SLOT_NUMBER; i++ )
	{
		pReq = pPort->Running_Req[i];

		if (pReq) {
			if (pReq->retry_cnt >= GALOIS_MAX_HANG_RETRY) 
			{
#ifdef GALOIS_HANG_DEBUG
				MV_PRINT("###############################################################\n");
				MV_ERROR_PRINT("[Core_ResetChannel]dropped bad req (retry=%d).\n", pReq->retry_cnt);
				MV_ERROR_PRINT("\tCdb[%2x,%2x,%2x,%2x, %2x,%2x,%2x,%2x, %2x,%2x,%2x,%2x].\n",
						pReq->Cdb[0], pReq->Cdb[1], pReq->Cdb[2], pReq->Cdb[3], 
						pReq->Cdb[4], pReq->Cdb[5], pReq->Cdb[6], pReq->Cdb[7],
						pReq->Cdb[8], pReq->Cdb[9], pReq->Cdb[10], pReq->Cdb[11]);
				MV_DumpRequest(pReq, MV_TRUE);
				MV_DumpSGTable(&pReq->SG_Table);
				MV_PRINT("###############################################################\n");
#endif
				pPort->Running_Req[i] = NULL;
				pPort->Running_Slot &= ~(1L<<i);
				Tag_ReleaseOne(&pPort->Tag_Pool, i);
				pReq->Scsi_Status = REQ_STATUS_MEDIA_ERROR;
				CompleteRequest(pCore, pReq, NULL);
				galois_sata_event(GALOIS_EVENT_L2H_REQDROP | (GALOIS_EVENT_FROM_PORT(pPort->Id)), pPort, pDevice);
				continue;
			}
			/*
			 * If this channel has multiple devices, pReq is 
			 * not the internal request of pDevice
			 */
			if ( !Core_IsInternalRequest(pCore, pReq) ) /* sent from linux */
			{
				pReq->retry_cnt++;
				List_Add(&pReq->Queue_Pointer, &pCore->Waiting_List);
			}
			else 
			{
				/* Can be reset command or request sense command */
				if ( SCSI_IS_REQUEST_SENSE(pReq->Cdb[0]) )
				{
					//MV_ASSERT( pReq->Org_Req!=NULL );
					if ( pReq->Org_Req ) 
					{
						((PMV_Request)pReq->Org_Req)->retry_cnt++;
						List_Add(&((PMV_Request)pReq->Org_Req)->Queue_Pointer, &pCore->Waiting_List);
					}
				}
			}
			
#if defined(SUPPORT_ERROR_HANDLING)
			hba_remove_timer(pReq);
			pReq->eh_flag = 1;
#endif
			pPort->Running_Req[i] = NULL;
			pPort->Running_Slot &= ~(1L<<i);
			Tag_ReleaseOne(&pPort->Tag_Pool, i);
		}
	}

	/* reset device related variables */
	for ( i=0; i<MAX_DEVICE_PER_PORT; i++ )
	{
		pDevice = &pPort->Device[i];
		
		pDevice->Device_Type = 0;
		pDevice->Need_Notify = MV_FALSE;
		pDevice->Outstanding_Req = 0;
		
		/*
		 * Go through the waiting list. If there is some reset 
		 * request, remove that request. 
		 */
		mvRemoveDeviceWaitingList(pCore, pDevice->Id, MV_FALSE);
	}

	// reset the tag stack - to guarantee soft reset is issued at slot 0
	Tag_Init( &pPort->Tag_Pool, MAX_TAG_NUMBER );

	for( i=0; i<MAX_DEVICE_PER_PORT; i++ )
	{
		if( (pPort->Device[i].Status & DEVICE_STATUS_FUNCTIONAL) && 
			(pPort->Device[i].Internal_Req != NULL) )
		{
			pCore->Total_Device_Count--;
			ReleaseInternalReqToPool( pCore, pPort->Device[i].Internal_Req );
			pPort->Device[i].Internal_Req = NULL;
		}
	}
	pPort->Port_State = PORT_STATE_IDLE;

#ifdef BERLIN_POWER_OFFON_DRIVE
	i = 0;
	/* polling for the status register, wait for device ready */
	while (i < HBARESET_STATUS_POLL_COUNT) {
		status = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_TFDATA);
		status &= 0xFF;
		if ((!(status & PORT_TF_STATUS_BSY)) 
				/*&& (status & PORT_TF_STATUS_DRDY)*/ 
				&& (status != 0x7F)) {
			break;
		} else {
			HBA_DelayMillisecond(pCore, 100);
		}
		i++;
	}
	if (i >= HBARESET_STATUS_POLL_COUNT)
		MV_ERROR_PRINT("Core_ResetChannel(%d before power cycle): not ready in %dms(0x%02x)\n", pPort->Id, i*100, status);

	/* Read signature of the device */
	signature = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_SIG);
	if (signature!=0xEB140101 && signature!=0x00000101) {
		/* disable interrupt and power on off */
		MV_ERROR_PRINT("Core_ResetChannel: disable interrupt\n");
		old_inten = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_IRQ_MASK);
		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_IRQ_MASK, 0x0);

		/* TODO: power off and delay 2s */
		MV_ERROR_PRINT("Core_ResetChannel: power off drive(%d), wait for 2s\n", pPort->Id);
#ifdef HACK_GPIO21_DRIVE_POWER
		mv_gpio_setup_pinmux(21);
		GPIO_PortSetInOut(21, 1); /* output */
		GPIO_PortWrite(21, 0); /* write 0 to turn off */
#endif
		HBA_DelayMillisecond(pCore, 2000);

		/* TODO: power on and delay 1s */
		MV_ERROR_PRINT("Core_ResetChannel: power on drive(%d), wait for 1s\n", pPort->Id);
#ifdef HACK_GPIO21_DRIVE_POWER
		GPIO_PortWrite(21, 1); /* write 1 to turn off */
#endif
		HBA_DelayMillisecond(pCore, 1000);

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
				HBA_DelayMillisecond(pCore, 100);
			}
			i++;
		}
		if (i >= HBARESET_STATUS_POLL_COUNT)
			MV_ERROR_PRINT("Core_ResetChannel(%d after power cycle): not ready in %dms(0x%02x)\n", pPort->Id, i*100, status);
		MV_ERROR_PRINT("Core_ResetChannel: delay 500ms more before reading signature.\n");
		HBA_DelayMillisecond(pCore, 500);

		/* clear interrupt status and re-enable interrupts */
		old_intsts = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_IRQ_STAT);
		MV_ERROR_PRINT("Core_ResetChannel: clear interrupt status(0x%08x) and enable interrupt(0x%08x)\n", old_intsts, old_inten);
		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_IRQ_STAT, old_intsts);
		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_IRQ_MASK, old_inten);
	}
#endif /* BERLIN_POWER_OFFON_DRIVE */

	/*
	 * Reset port, it will call mvDeviceStateMachine after detecting device.
	 */
	MV_ERROR_PRINT("start SATA_PortReset %d\n", pPort->Id);
	SATA_PortReset(pPort, GALOIS_SATA_ATOMIC);
}
#endif /* SUPPORT_ERROR_HANDLING */

/*
 * Handle this request if it needs hardware access.
 * called by Core_HandleWaitingList
 */
MV_BOOLEAN HandleInstantRequest(PCore_Driver_Extension pCore, PMV_Request pReq)
{
	/* 
	 * Some of the requests can be returned immediately without hardware 
	 * access. 
	 * Handle Inquiry and Read Capacity.
	 * If return MV_TRUE, means the request can be returned to OS now.
	 */
	PDomain_Device pDevice = NULL;
	MV_U8 portId, deviceId;

	portId = PATA_MapPortId(pReq->Device_Id);
	deviceId = PATA_MapDeviceId(pReq->Device_Id);

	if ( portId < MAX_PORT_NUMBER )	
		pDevice = &pCore->Ports[portId].Device[deviceId];

	/* ATAPI device */
	if (pDevice && (pDevice->Device_Type & DEVICE_TYPE_ATAPI) 
			&& (pDevice->Status & DEVICE_STATUS_FUNCTIONAL))
	{
		switch (pReq->Cdb[0]) 
		{
		case SCSI_CMD_MODE_SENSE_6:
			/* convert to atapi cdb12 */
			pReq->Cdb[0] = SCSI_CMD_MODE_SENSE_10;
			pReq->Cdb[1] = 0x8;	/* Disable Block Descriptor =1 */
			pReq->Cdb[8] = 32;//((pReq->Cdb[2] & 0x3f) == 0x2a)? 32 : 0;	/* XXX: hack from PowerDVD */
			pReq->Cdb[9] = pReq->Cdb[5];
			pReq->Cdb[4] = 0;
			pReq->Cdb[5] = 0;
			pReq->Cdb[2] = pReq->Cdb[2];
			pReq->Cdb[3] = pReq->Cdb[3];
			pReq->Cdb[6] = 0;
			pReq->Cdb[7] = 0;
			pReq->Cdb[10] = 0;
			pReq->Cdb[11] = 0;
			MV_ERROR_PRINT("HandleInstantRequest: translate SCSI_CMD_MODE_SENSE_6\n");
			{
				int i;
				MV_ERROR_PRINT("MODE 10 cmd: ");
				for (i = 0; i < 16; i++) {
					MV_ERROR_PRINT("%02x ", pReq->Cdb[i]);
				}
				MV_ERROR_PRINT("\n");
			}
			if (pReq->Cmd_Initiator == HBA_GetModuleExtension(pReq->Cmd_Initiator, MODULE_HBA))
				HBA_kunmap_sg(pReq);
			break;
#if 0
		case SCSI_CMD_MODE_SENSE_10:
			pReq->Cdb[1] |= 0x8;	/* Disable Block Descriptor =1 */
			pReq->Cdb[8] = 32;	/* DBD=1, so len=32 */
			if (pReq->Cmd_Initiator == HBA_GetModuleExtension(pReq->Cmd_Initiator, MODULE_HBA))
				HBA_kunmap_sg(pReq);
			break;
#endif
		case SCSI_CMD_MODE_SENSE_10:
		case SCSI_CMD_INQUIRY:
		case SCSI_CMD_READ_CAPACITY_10:
		case SCSI_CMD_READ_CAPACITY_16:
		case SCSI_CMD_REPORT_LUN:
			if (pReq->Cmd_Initiator == HBA_GetModuleExtension(pReq->Cmd_Initiator, MODULE_HBA))
				HBA_kunmap_sg(pReq);
			break;
		default:
			break;
		}
		/* 
		 * it's not instant command, transfer to hardware
		 */
		return MV_FALSE;
	}

	/* ATA device */
	switch ( pReq->Cdb[0] )
	{
	case SCSI_CMD_INQUIRY:
		mvScsiInquiry(pCore, pReq);
		return MV_TRUE;
	case SCSI_CMD_MODE_SENSE_6:
	case SCSI_CMD_MODE_SENSE_10:
		mvScsiModeSense(pCore, pReq);
		return MV_TRUE;
	case SCSI_CMD_REPORT_LUN:
		mvScsiReportLun(pCore, pReq);
		return MV_TRUE;
	case SCSI_CMD_READ_CAPACITY_10:
		mvScsiReadCapacity(pCore, pReq);
		return MV_TRUE;
	case SCSI_CMD_READ_CAPACITY_16: /* 0x9e SERVICE_ACTION_IN */
		if ((pReq->Cdb[1] & 0x1f) == 0x10 /* SAI_READ_CAPACITY_16 */) {
			MV_PU32 pU32Buffer = (MV_PU32)pReq->Data_Buffer;
			MV_LBA maxLBA;
			MV_U32 blockLength = SECTOR_SIZE;
			maxLBA.value = pDevice->Max_LBA.value-1;;

			pU32Buffer[0] = CPU_TO_BIG_ENDIAN_32(maxLBA.low);
			pU32Buffer[1] = CPU_TO_BIG_ENDIAN_32(maxLBA.high);
			pU32Buffer[2] =  CPU_TO_BIG_ENDIAN_32(blockLength);;
			pReq->Scsi_Status = REQ_STATUS_SUCCESS;;
		}
		else
			pReq->Scsi_Status = REQ_STATUS_INVALID_REQUEST;
		return MV_TRUE;
	case SCSI_CMD_REQUEST_SENSE:	/* This is only for Thor hard disk */
	case SCSI_CMD_TEST_UNIT_READY:
	case SCSI_CMD_RESERVE_6:	/* For Thor, just return good status */
	case SCSI_CMD_RELEASE_6:
//	case SCSI_CMD_START_STOP_UNIT:
		pReq->Scsi_Status = REQ_STATUS_SUCCESS;
		return MV_TRUE;
	}

	return MV_FALSE;
}

/*
 * by Core_HandleWaitingList
 * ATA_CDB2TaskFile, SATA_PrepareCommandHeader, SATA_PrepareCommandTable, SATA_SendFrame
 */
MV_QUEUE_COMMAND_RESULT PrepareAndSendCommand(
	IN PCore_Driver_Extension pCore,
	IN PMV_Request pReq
	)
{
	PDomain_Device pDevice = NULL;
	PDomain_Port pPort = NULL;
	MV_U8 tag, i, count=0;
	ATA_TaskFile taskFile;
	MV_BOOLEAN ret;

	/* Associate this request to the corresponding device and port */
	pDevice = &pCore->Ports[PATA_MapPortId(pReq->Device_Id)].Device[PATA_MapDeviceId(pReq->Device_Id)];
	pPort = pDevice->PPort;

	if ( !(pDevice->Status&DEVICE_STATUS_FUNCTIONAL) )
	{
		pReq->Scsi_Status = REQ_STATUS_NO_DEVICE;
		return MV_QUEUE_COMMAND_RESULT_FINISHED;
	}

	/* Set the Cmd_Flag to indicate which type of commmand it is. */
	if ( !Category_CDB_Type(pDevice, pReq) )
	{
		pReq->Scsi_Status = REQ_STATUS_INVALID_REQUEST;
		/* Invalid request and can be returned to OS now. */
		return MV_QUEUE_COMMAND_RESULT_FINISHED;
	}

	MV_DASSERT( pPort!=NULL );
	if ( pPort->Running_Slot!=0 )	/* Some requests are running. */
	{
#if 0
		if (((pReq->Cmd_Flag&CMD_FLAG_NCQ) && !(pPort->Setting&PORT_SETTING_NCQ_RUNNING))
			|| (!(pReq->Cmd_Flag&CMD_FLAG_NCQ) && (pPort->Setting&PORT_SETTING_NCQ_RUNNING)))
		{
			return MV_QUEUE_COMMAND_RESULT_FULL;
		}
	
		/* In order for request sense to immediately follow the error request. */
		if ( pDevice->Device_Type&DEVICE_TYPE_ATAPI )
			return MV_QUEUE_COMMAND_RESULT_FULL;

		/* One request at a time */
		if ((pReq->Scsi_Status==REQ_STATUS_RETRY)
			|| (pPort->Setting&PORT_SETTING_DURING_RETRY))
			return MV_QUEUE_COMMAND_RESULT_FULL;
#else
		return MV_QUEUE_COMMAND_RESULT_FULL;
#endif
	}

	/* we always reserve one slot in case of PM hot plug */
	for (i=0; i<MAX_SLOT_NUMBER; i++)
	{
		if (pPort->Running_Slot & MV_BIT(i))
			count++;
	}
	if (count >= (MAX_SLOT_NUMBER - 1))
	{
		return MV_QUEUE_COMMAND_RESULT_FULL;
	}

	/* Get one slot for this request. */
	tag = Tag_GetOne(&pPort->Tag_Pool);

	if (pDevice->Device_Type&DEVICE_TYPE_ATAPI)
		ret = ATAPI_CDB2TaskFile(pDevice, pReq, &taskFile);
	else
		ret = ATA_CDB2TaskFile(pDevice, pReq, tag, &taskFile);
	if (!ret)
	{
		pReq->Scsi_Status = REQ_STATUS_INVALID_REQUEST;
		Tag_ReleaseOne(&pPort->Tag_Pool, tag);
		/* Invalid request and can be returned to OS now. */
		return MV_QUEUE_COMMAND_RESULT_FINISHED;	
	}

	/* clear Received FIS area */
	MV_ZeroMemory(pPort->RX_FIS, SATA_RX_FIS_SIZE);

	/* Prepare structure of tag and send it out */
	SATA_PrepareCommandHeader(pPort, pReq, tag);
	SATA_PrepareCommandTable(pPort, pReq, tag, &taskFile);
	SATA_SendFrame(pPort, pReq, tag);
	pReq->retry_cnt++;

#ifdef BERLIN_SPRUCE_INTEGRATION
		if (pReq->Cdb[0] == SCSI_CMD_SPRUCE_DOWNLOAD_CODE) {
			PMV_Command_Header pHeader= SATA_GetCommandHeader(pPort, tag);
			PMV_Command_Table pCmdTable = Port_GetCommandTable(pPort, tag);
			PSATA_FIS_REG_H2D pFIS = (PSATA_FIS_REG_H2D)pCmdTable->FIS;

			Galois_ErrorDumpCmd(pReq);
			Galois_ErrorDumpSGTable(&pReq->SG_Table);
			Galois_ErrorDumpHeader(pHeader);
			Galois_ErrorDumpFIS(pFIS);
		}
#endif

	/* Request is send to the hardware and not finished yet. */
	return MV_QUEUE_COMMAND_RESULT_SENDTED;
}

/*
 * called by Core_InternalSendRequest.
 * call PrepareAndSendCommand to send command.
 */
void Core_HandleWaitingList(PCore_Driver_Extension pCore)
{
	PMV_Request pReq = NULL;
	MV_QUEUE_COMMAND_RESULT result;
#ifdef SUPPORT_HOT_PLUG
	PDomain_Device pDevice;
	MV_U8 portId=0xff, deviceId=0xff;
#endif	

	/* Get the request header */
	while ( !List_Empty(&pCore->Waiting_List) )
	{
		pReq = (PMV_Request) List_GetFirstEntry(&pCore->Waiting_List, MV_Request, Queue_Pointer);
		if ( NULL == pReq ) {
			MV_ASSERT(0);
			break;
		}

#if defined(SUPPORT_ERROR_HANDLING)
		//pReq->eh_flag = 0;
		hba_init_timer(pReq);
#endif 

#ifdef SUPPORT_HOT_PLUG
		/* hot plug - device is gone, reject this request */
		{
			portId = PATA_MapPortId(pReq->Device_Id);
			deviceId = PATA_MapDeviceId(pReq->Device_Id);
			pDevice = &pCore->Ports[portId].Device[deviceId];

			if ( !(pDevice->Status & DEVICE_STATUS_FUNCTIONAL) )
			{
				pReq->Scsi_Status = REQ_STATUS_NO_DEVICE;
				CompleteRequest(pCore, pReq, NULL);
				return;
			}

			/* Reset is not done yet. */
			if ( pDevice->State!=DEVICE_STATE_INIT_DONE )
			{
				/* check if it is the reset commands */
				if ( !Core_IsInternalRequest(pCore, pReq) )
				{
					List_Add(&pReq->Queue_Pointer, &pCore->Waiting_List);//Return request back
					return;
				} 
				else 
				{
					/* 
					 * Cannot be the request sense. 
					 * It's not pushed back. 
					 */
					//MV_ASSERT( !SCSI_IS_REQUEST_SENSE(pReq->Cdb[0]) );
				}
			}
		}
#endif /* SUPPORT_HOT_PLUG */


		/* Whether we can handle this request without hardware access? */
		if ( HandleInstantRequest(pCore, pReq) ) 
		{
#ifdef GALOIS_ERROR_DEBUG
			int i;
			MV_PRINT("\n[Galois]This's instant commands(port=%d, device=%d): ", portId, deviceId);
			for (i = 0; i < 16; i++) {
				MV_PRINT("%02x ", pReq->Cdb[i]);
			}
			MV_PRINT("\n\n");
#endif
			CompleteRequest(pCore, pReq, NULL);
			continue;
		}

#ifdef GALOIS_ERROR_DEBUG
		if (!SCSI_IS_READ(pReq->Cdb[0]) && !SCSI_IS_WRITE(pReq->Cdb[0])) {
			int i;
			MV_PRINT("\n[Galois]This's not READ/WRITE commands(port=%d, device=%d): ", portId, deviceId);
			for (i = 0; i < 16; i++) {
				MV_PRINT("%02x ", pReq->Cdb[i]);
			}
			//MV_DumpRequest(pReq, MV_TRUE);
			//MV_DumpSGTable(&pReq->SG_Table);
			MV_PRINT("\n\n");
		}
#endif

		/* Send out the command to disk */
		result = PrepareAndSendCommand(pCore, pReq);	

		switch ( result )
		{
			case MV_QUEUE_COMMAND_RESULT_FINISHED:
				CompleteRequest(pCore, pReq, NULL);
				break;

			case MV_QUEUE_COMMAND_RESULT_FULL:
				List_Add(&pReq->Queue_Pointer, &pCore->Waiting_List);
				return;

			case MV_QUEUE_COMMAND_RESULT_SENDTED:
			{
				portId = PATA_MapPortId(pReq->Device_Id);
				deviceId = PATA_MapDeviceId(pReq->Device_Id);
				pDevice = &pCore->Ports[portId].Device[deviceId];
				pDevice->Outstanding_Req++;
#if defined(SUPPORT_ERROR_HANDLING)
				/*
				 * timeout to 15 secs if the port has just been reset.
				 */
				if (pReq->eh_flag) 
				{
					pReq->timeout = (pDevice->Device_Type & DEVICE_TYPE_ATAPI)?
						HBA_ATAPI_REQ_TIMER_AFTER_RESET : HBA_ATA_REQ_TIMER_AFTER_RESET;
					pReq->eh_flag = 0; 
				}
				else
				{
					pReq->timeout = (pDevice->Device_Type & DEVICE_TYPE_ATAPI)?
						HBA_ATAPI_REQ_TIMER: HBA_ATA_REQ_TIMER;

				}

				/* Spruce F/W upgrade: > 30s */
				if (pReq->Cdb[0] == SCSI_CMD_SPRUCE_WRITE_BUFFER)
					pReq->timeout = 120;
				else if (pReq->Cdb[0] == SCSI_CMD_READ_TOC)
					pReq->timeout = 20;

				/* For internal command, timeout is longer for irq disable during init process */
				if( Core_IsInternalRequest(pCore, pReq) && (pDevice->State!=DEVICE_STATE_INIT_DONE) ) {
					pReq->timeout = 60;
				}

				hba_add_timer(pReq, pReq->timeout, __core_req_timeout_handler);
#endif
				break;
			}
			default:
				MV_ASSERT(MV_FALSE);
		}
	}
}

/*
 * Interrupt service routine and related funtion
 * We can split this function to two functions. 
 * One is used to check and clear interrupt, called in ISR. 
 * The other is used in DPC.
 */
void SATA_PortHandleInterrupt(
	IN PCore_Driver_Extension pCore,
	IN PDomain_Port pPort
	);
void SATA_HandleSerialError(
	IN PDomain_Port pPort,
	IN MV_U32 serialError
	);
void SATA_HandleHotplugInterrupt(
	IN PDomain_Port pPort,
	IN MV_U32 serialError
	);

MV_BOOLEAN Core_InterruptServiceRoutine(MV_PVOID This)
{
	PCore_Driver_Extension pCore = (PCore_Driver_Extension)This;
	MV_U32	irqStatus;
	MV_U8 i;
	PDomain_Port pPort = NULL;

	/* Get interrupt status */
	irqStatus = MV_REG_READ_DWORD(pCore->Mmio_Base, HOST_IRQ_STAT);

	irqStatus &= pCore->Port_Map;
	if (!irqStatus ) 
	{
		return MV_FALSE;
	}
	
	for ( i=0; i<pCore->Port_Num; i++ ) 
	{
		if ( !(irqStatus&(1<<i)) )	/* no interrupt for this port. */
			continue;

		pPort = &pCore->Ports[i];
		SATA_PortHandleInterrupt(pCore, pPort);
	}

	Core_HandleWaitingList(pCore);
	return MV_TRUE;
}

void SATA_HandleSerialError(
	IN PDomain_Port pPort,
	IN MV_U32 serialError
	)
{
	MV_ERROR_PRINT("SATA_HandleSerialError: port=%d error=0x%x.\n", pPort->Id, serialError);
}

#ifdef SUPPORT_HOT_PLUG
void Device_SoftReset(PDomain_Port pPort, PDomain_Device pDevice);

void mvRemoveDeviceWaitingList(MV_PVOID This, MV_U16 deviceId, 
			       MV_BOOLEAN returnOSRequest)
{
	PCore_Driver_Extension pCore = (PCore_Driver_Extension)This;
	PMV_Request pReq = NULL;
	List_Head *pPos;
	List_Head remove_List;
	MV_U8 count = 0, myCount=0, i;
	PDomain_Device pDevice;
	MV_U8 portNum = PATA_MapPortId(deviceId);
	MV_U8 deviceNum = PATA_MapDeviceId(deviceId);
	pDevice = &pCore->Ports[portNum].Device[deviceNum];

	LIST_FOR_EACH(pPos, &pCore->Waiting_List) {
		count++;
	}

	if (count!=0){
		MV_LIST_HEAD_INIT(&remove_List);
	}

	/* 
	 * If returnOSRequest is MV_FALSE, actually we just remove the 
	 * internal reset command. 
	 */
	while ( count>0 )
	{
		/* XXX:0. remove all req out from Waiting_List */
		pReq = (PMV_Request)List_GetFirstEntry(&pCore->Waiting_List, MV_Request, Queue_Pointer);

		if ( pReq->Device_Id==deviceId )
		{
			/* 
			 * TBD: should make change to the mvRemovePortWaitingList too.
			 */
			if ( !Core_IsInternalRequest(pCore, pReq) )
			{
				if ( returnOSRequest ) {
					pReq->Scsi_Status = REQ_STATUS_NO_DEVICE;
					List_AddTail(&pReq->Queue_Pointer, &remove_List);
					myCount++;
				} else {
					/* XXX:1. re-queue OS request */
					List_AddTail(&pReq->Queue_Pointer, &pCore->Waiting_List);
				}
			}
			else 
			{
				/* Reset command or int request sense or int init commands */
				if ( SCSI_IS_REQUEST_SENSE(pReq->Cdb[0]) )
				{
					pReq = (PMV_Request)pReq->Org_Req;
					if ( pReq ) {
						if ( returnOSRequest ) {
							pReq->Scsi_Status = REQ_STATUS_NO_DEVICE;
							List_AddTail(&pReq->Queue_Pointer, &remove_List);
							myCount++;
						} else {
							/* XXX:2. re-queue old OS request */
							List_AddTail(&pReq->Queue_Pointer, &pCore->Waiting_List);
						}
					}
				} else {
					/* Reset command is removed. */
				}
			}
			/* XXX:3. all other commands(reset, internal sense, internal init commands) are discarded */
		}
		else
		{
			List_AddTail(&pReq->Queue_Pointer, &pCore->Waiting_List);
		}
		count--;
	}

	for (i=0; i<myCount; i++){
		pReq = (PMV_Request)List_GetFirstEntry(&remove_List, MV_Request, Queue_Pointer);
		MV_DASSERT(pReq && (pReq->Scsi_Status==REQ_STATUS_NO_DEVICE));
		CompleteRequest(pCore, pReq, NULL);
	}
}

void mvRemovePortWaitingList( MV_PVOID This, MV_U8 portId )
{
	PCore_Driver_Extension pCore = (PCore_Driver_Extension)This;
	PMV_Request pReq;
	List_Head *pPos;
	List_Head remove_List;
	MV_U8 count = 0, myCount=0, i;

	LIST_FOR_EACH(pPos, &pCore->Waiting_List) {
		count++;
	}

	if (count!=0){
		MV_LIST_HEAD_INIT(&remove_List);
	}

	while ( count>0 )
	{
		/* 1. remove all req out from Waiting_List */
		pReq = (PMV_Request)List_GetFirstEntry(&pCore->Waiting_List, MV_Request, Queue_Pointer);
		if ( PATA_MapPortId(pReq->Device_Id) == portId )
		{
			/* 2.1. for portId, only Complete(move to remove_List) cmds from Linux kernel */
			if ( pReq->Cmd_Initiator==pCore ) {
				if ( !Core_IsInternalRequest(pCore, pReq) ) {
				//if ( SCSI_IS_READ(pReq->Cdb[0]) || SCSI_IS_WRITE(pReq->Cdb[0]) ) {
					pReq->Scsi_Status = REQ_STATUS_NO_DEVICE;
					List_AddTail(&pReq->Queue_Pointer, &remove_List);
					myCount++;
				} else if ( SCSI_IS_REQUEST_SENSE(pReq->Cdb[0]) ) {
					pReq = (PMV_Request)pReq->Org_Req; /* Request sense */
					if ( pReq ) {
						pReq->Scsi_Status = REQ_STATUS_NO_DEVICE;
						List_AddTail(&pReq->Queue_Pointer, &remove_List);
						myCount++;
					} 
				} else {
					/* 2.2. Reset/Init command. directly dropped from Waiting_List. */
				}
			} else {
				/* 3. ? cmd */
				pReq->Scsi_Status = REQ_STATUS_NO_DEVICE;
				List_AddTail(&pReq->Queue_Pointer, &remove_List);
				myCount++;
			}
		}
		else
		{
			/* 4. Re-queue cmd of other ports into Waiting_List */
			List_AddTail(&pReq->Queue_Pointer, &pCore->Waiting_List);
		}
		count--;
	}

	for (i=0; i<myCount; i++){
		pReq = (PMV_Request)List_GetFirstEntry(&remove_List, MV_Request, Queue_Pointer);
		MV_DASSERT(pReq && (pReq->Scsi_Status==REQ_STATUS_NO_DEVICE));
		CompleteRequest(pCore, pReq, NULL);
	}
}

void mvHandleDeviceUnplug (PCore_Driver_Extension pCore, PDomain_Port pPort)
{
	PMV_Request pReq;
	//PDomain_Device pDevice;
	MV_LPVOID portMmio = pPort->Mmio_Base;
	MV_U8 i, j;
	MV_U32 temp;

	/*
	 * wait for PORT_SCR_STAT to become non-zero(for a faked plug-out irq)
	 */
	MV_ERROR_PRINT("[Galois]port %d unplug, wait for at most 1 seconds.\n\n", pPort->Id);
	HBA_DelayMillisecond(pCore, 500);
	j = 0;
	while (j < 95) {
		HBA_DelayMillisecond(pCore, 10);
		if ((MV_REG_READ_DWORD(portMmio, PORT_SCR_STAT) & 0xf))
			break;
		j++;
	}

	if( !SATA_PortDeviceDetected(pPort) )
	{	
		/* clear the start bit in cmd register, 
		   stop the controller from handling anymore requests */
		temp = MV_REG_READ_DWORD( portMmio, PORT_CMD );
		MV_REG_WRITE_DWORD( portMmio, PORT_CMD, temp & ~MV_BIT(0));

		SATA_PortReportNoDevice( pCore, pPort ); /* remove scd */
		
		/* Device is gone. Return the Running_Req */
		for ( i=0; i<MAX_SLOT_NUMBER; i++ )
		{
			pReq =  pPort->Running_Req[i];
			if ( pReq !=NULL )
			{
				pReq->Scsi_Status = REQ_STATUS_NO_DEVICE;
				CompleteRequestAndSlot(pCore, pPort, pReq, NULL, i);
			}
		}
		
		if( pPort->Type == PORT_TYPE_PM )
		{
			pPort->Setting &= ~PORT_SETTING_PM_FUNCTIONAL;
			pPort->Setting &= ~PORT_SETTING_PM_EXISTING;
		}
		galois_sata_event(GALOIS_EVENT_L2H_HOTPLUGOUT | (GALOIS_EVENT_FROM_PORT(pPort->Id)), pPort, NULL);
	}
}

void sendDummyFIS( PDomain_Port pPort )
{
	MV_U8 tag = Tag_GetOne(&pPort->Tag_Pool);
	PMV_Command_Header header = SATA_GetCommandHeader(pPort, tag);
	PMV_Command_Table pCmdTable = Port_GetCommandTable(pPort, tag);
	PSATA_FIS_REG_H2D pFIS = (PSATA_FIS_REG_H2D)pCmdTable->FIS;
	PCore_Driver_Extension pCore = pPort->Core_Extension;
	MV_LPVOID portMmio = pPort->Mmio_Base;
	MV_U32 old_stat;

	MV_DASSERT( tag == 0 );

	mvDisableIntr(portMmio, old_stat);

	MV_ZeroMemory(header, sizeof(MV_Command_Header));
	MV_ZeroMemory(pCmdTable, sizeof(MV_Command_Table));
	
	header->FIS_Length = 0;
	header->Reset = 0;
	header->PM_Port = 0xE;
	
	header->Table_Address = pPort->Cmd_Table_DMA.low + SATA_CMD_TABLE_SIZE*tag;
	header->Table_Address_High = pPort->Cmd_Table_DMA.high;//TBD
	
	pFIS->FIS_Type = SATA_FIS_TYPE_REG_H2D;
	pFIS->PM_Port = 0;
	pFIS->Control = 0;

	MV_REG_WRITE_DWORD(portMmio, PORT_CMD_ISSUE, 1<<tag);
	MV_REG_READ_DWORD(portMmio, PORT_CMD_ISSUE);	/* flush */


	HBA_DelayMicrosecond(pCore, 10);
	
	Tag_ReleaseOne(&pPort->Tag_Pool, tag);
	mvEnableIntr(portMmio, old_stat);
}

void mvHandleDevicePlugin(PCore_Driver_Extension pCore, PDomain_Port pPort)
{
	PDomain_Device pDevice = &pPort->Device[0];
	MV_LPVOID portMmio = pPort->Mmio_Base;
	MV_U32 temp;
	MV_U32 i;

	if( pCore->Total_Device_Count >= MAX_DEVICE_SUPPORTED )
		return;
	
	/* hardware workaround - send dummy FIS first to clear FIFO */
	temp = MV_REG_READ_DWORD( portMmio, PORT_CMD );
	MV_REG_WRITE_DWORD( portMmio, PORT_CMD, temp & ~MV_BIT(0));
	MV_REG_WRITE_DWORD( portMmio, PORT_CMD, temp | MV_BIT(0));
	Tag_Init( &pPort->Tag_Pool, MAX_TAG_NUMBER );
//	sendDummyFIS( pPort );
	
	// start command handling on this port
	temp = MV_REG_READ_DWORD( portMmio, PORT_CMD );
	MV_REG_WRITE_DWORD( portMmio, PORT_CMD, temp & ~MV_BIT(0));
	MV_REG_WRITE_DWORD( portMmio, PORT_CMD, temp | MV_BIT(0));

	MV_ERROR_PRINT("[Galois]port %d plugin, wait for 1.5 seconds.\n", pPort->Id);
	HBA_DelayMillisecond(pCore, 1500);

	// reset the tag stack - to guarantee soft reset is issued at slot 0
	Tag_Init( &pPort->Tag_Pool, MAX_TAG_NUMBER );

	/* Always turn the PM bit on - otherwise won't work! */
	temp = MV_REG_READ_DWORD(portMmio, PORT_CMD);					
	MV_REG_WRITE_DWORD(portMmio, PORT_CMD, temp | MV_BIT(17));
	temp=MV_REG_READ_DWORD(portMmio, PORT_CMD);	/* flush */

	// do software reset
	MV_PRINT(("[Galois plugin]Detected device plug-in, doing soft reset\n"));
	if (!(SATA_SoftResetDevice(pPort, 0x0)))
		return;
	
	/* not a PM - turn off the PM bit in command register */
	temp = MV_REG_READ_DWORD(portMmio, PORT_CMD);					
	MV_REG_WRITE_DWORD(portMmio, PORT_CMD, temp & (~MV_BIT(17)));
	temp=MV_REG_READ_DWORD(portMmio, PORT_CMD);	/* flush */

	if( SATA_PortDeviceDetected(pPort) )
	{
		if ( SATA_PortDeviceReady(pPort) )
		{
			MV_U32 signature;

			pDevice->Internal_Req = GetInternalReqFromPool(pCore);
			if( pDevice->Internal_Req == NULL )
			{
				MV_PRINT(("ERROR: Unable to get an internal request buffer\n"));
				// can't initialize without internal buffer - just set this disk down
				pDevice->Status = DEVICE_STATUS_NO_DEVICE;
				pDevice->State = DEVICE_STATE_INIT_DONE;
			}
			else 
			{
				pDevice->Status = DEVICE_STATUS_EXISTING|DEVICE_STATUS_FUNCTIONAL;
				pDevice->State = DEVICE_STATE_RESET_DONE;
				pPort->Device_Number++;
				pDevice->Id = (pPort->Id)*MAX_DEVICE_PER_PORT;
				pDevice->Need_Notify = MV_TRUE;
			}
			
			/* polling for the status register, wait for device ready */
#define PLUGIN_STATUS_POLL_COUNT 800
			i = 0;
			while (i < PLUGIN_STATUS_POLL_COUNT) {
				temp = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_TFDATA);
				temp &= 0xFF;
				if ((!(temp & PORT_TF_STATUS_BSY)) 
						/*&& (temp & PORT_TF_STATUS_DRDY)*/
						&& (temp != 0x7F)) {
					break;
				} else {
					HBA_DelayMillisecond(pCore, 10);
				}
				i++;
			}
			if (i >= PLUGIN_STATUS_POLL_COUNT)
				MV_ERROR_PRINT("mvHandleDevicePlugin: device status not ready in %dms(0x%02x)\n", i*10, temp);

			/* Read signature of the device */
			signature = MV_REG_READ_DWORD(portMmio, PORT_SIG);
			MV_PRINT("[Galois]hotplug SATA device signature is 0x%08x: ", signature);
			if (signature==0xEB140101) {/* ATAPI signature */
				pDevice->Device_Type |= DEVICE_TYPE_ATAPI;
				MV_PRINT("ATAPI device.\n");
				galois_sata_event(GALOIS_EVENT_L2H_HOTPLUGIN | (GALOIS_EVENT_FROM_PORT(pPort->Id)), pPort, NULL);
			} else if (signature==0x00000101) {/* ATA signature */
				MV_PRINT("ATA device.\n");
				galois_sata_event(GALOIS_EVENT_L2H_HOTPLUGIN | (GALOIS_EVENT_FROM_PORT(pPort->Id)), pPort, NULL);
			} else {
#if 1
				MV_ERROR_PRINT("No signature, do soft reset and read again.\n");
				SATA_SoftResetDevice(pPort, 0x0);
				HBA_DelayMillisecond(pCore, 500);
#else
				MV_ERROR_PRINT("hotplug no signature, wait for 2 seconds and read again.\n");
				HBA_DelayMillisecond(pCore, 2000);
#endif
				signature = MV_REG_READ_DWORD(portMmio, PORT_SIG);
				MV_ERROR_PRINT("[Galois]2nd read hotplug SATA device signature is 0x%08x: ", signature);
				if (signature==0xEB140101) {/* ATAPI signature */
					pDevice->Device_Type |= DEVICE_TYPE_ATAPI;
					MV_ERROR_PRINT("ATAPI device.\n");
				} else if (signature==0x00000101) {/* ATA signature */
					MV_ERROR_PRINT("ATA device.\n");
				} else {
					MV_ERROR_PRINT("No hotplug device or broken device(fatal error, please power off).\n");
					MV_DumpHBA(pCore, pPort);
					galois_sata_event(GALOIS_EVENT_L2H_NOSIG | (GALOIS_EVENT_FROM_PORT(pPort->Id)), pPort, pDevice);
					pDevice->Status = DEVICE_STATUS_NO_DEVICE;
					pDevice->State = DEVICE_STATE_INIT_DONE;
				}
			}

			MV_PRINT("\n[Galois plugin]Start mvDeviceStateMachine at port %d.\n", pPort->Id);
			mvDeviceStateMachine (pCore, pDevice, GALOIS_SATA_ATOMIC);
		}
	}
}
#endif	/* #ifdef SUPPORT_HOT_PLUG */

void SATA_HandleHotplugInterrupt(
	IN PDomain_Port pPort,
	IN MV_U32 intStatus
	)
{
#ifdef SUPPORT_HOT_PLUG
	PDomain_Device pDevice = &pPort->Device[0];
	PCore_Driver_Extension pCore = pPort->Core_Extension;
	MV_U8 plugout=0, plugin=0;
	MV_LPVOID portMmio = pPort->Mmio_Base;

	MV_U32 hotPlugDevice = intStatus & PORT_IRQ_PHYRDY;
	//hotPlugPM = (intStatus & PORT_IRQ_ASYNC_NOTIF) || (intStatus & PORT_IRQ_SDB_FIS);
		
	MV_ERROR_PRINT("[Berlin]__MV__ Hotplug interrupt status(%d): 0x%08x\n", pPort->Id, intStatus);
	intStatus &= ~(PORT_IRQ_D2H_REG_FIS|PORT_IRQ_SDB_FIS|PORT_IRQ_PIO_DONE);

	/* if a hard drive or a PM is plugged in/out of the controller */
	if( hotPlugDevice )
	{
		intStatus &= ~PORT_IRQ_PHYRDY;

		/* use Phy status to determine if this is a plug in/plug out */
		HBA_DelayMillisecond(pCore, 500);
		if ((MV_REG_READ_DWORD(portMmio, PORT_SCR_STAT) & 0xf) == 0)
			plugout = MV_TRUE;
		else
			plugin = MV_TRUE;

		/* following are special cases, so we take care of these first */
		if( plugout )
		{
			if ( (pPort->Type != PORT_TYPE_PM ) 
					&& (pDevice->Status & DEVICE_STATUS_EXISTING) 
					&& !(pDevice->Status & DEVICE_STATUS_FUNCTIONAL) )
			{
				// a bad drive was unplugged
				pDevice->Status = DEVICE_STATUS_NO_DEVICE;
				MV_PRINT(("bad drive was unplugged\n"));
				return;
			}

			if ( (pPort->Setting & PORT_SETTING_PM_EXISTING) 
					&& !(pPort->Setting & PORT_SETTING_PM_FUNCTIONAL) )
			{
				// a bad PM was unplugged
				pPort->Setting &= ~PORT_SETTING_PM_EXISTING;
				MV_PRINT(("bad PM was unplugged\n"));
				return;
			}
		}
		
		if ( ((pPort->Type == PORT_TYPE_PM) && (pPort->Setting & PORT_SETTING_PM_FUNCTIONAL))
				||
		     ((pPort->Type != PORT_TYPE_PM) && (pDevice->Status & DEVICE_STATUS_FUNCTIONAL)) 
			)
		{
			if( plugout ){
				mvHandleDeviceUnplug( pCore, pPort );
			}
		}
		else
		{
			if( plugin ) {
				mvHandleDevicePlugin( pCore, pPort );
			}
		}
	}
#endif

	if ( intStatus )
	{
		MV_PRINT("[Berlin]Error: SATA_HandleHotplugInterrupt port=%d intStatus=0x%08x.\n", 
				pPort->Id, intStatus);
	}
}

void mvCompleteSlots( PDomain_Port pPort, MV_U32 completeSlot, PATA_TaskFile taskFiles )
{
	PCore_Driver_Extension pCore = pPort->Core_Extension;
#ifdef MV_DEBUG
	MV_LPVOID port_mmio = pPort->Mmio_Base;
#endif
	PDomain_Device pDevice;
	PMV_Request pReq = NULL, pOrgReq = NULL;
	MV_U8 slotId;

	/* Complete finished commands. All of them are finished successfully.
	 * There are three situations code will come here.
	 * 1. No error for both NCQ and Non-NCQ.
	 * 2. Under NCQ, some requests are completed successfully. At lease one is not.
	 *	For the error command, by specification, SActive isn't cleared.
	 * 3. Under non-NCQ, since no interrupt coalescing, no succesful request. 
	 *  Hardware will return one request is completed. But software clears it above. */

	for ( slotId=0; slotId<MAX_SLOT_NUMBER; slotId++ )
	{
		if ( !(completeSlot&(1L<<slotId)) )
			continue;

		MV_DASSERT( (MV_REG_READ_DWORD(port_mmio, PORT_CMD_ISSUE)&(1<<slotId))==0 );
		MV_DASSERT( (MV_REG_READ_DWORD(port_mmio, PORT_SCR_ACT)&(1<<slotId))==0 );

		completeSlot &= ~(1L<<slotId);
				
		/* This slot is finished. */
		pReq = pPort->Running_Req[slotId];
		MV_DASSERT( pReq );
		pDevice = &pPort->Device[PATA_MapDeviceId(pReq->Device_Id)];

		if ( pReq->Scsi_Status==REQ_STATUS_RETRY )
		{
			MV_PRINT("Retried request is finished...");
			MV_DumpRequest(pReq, MV_FALSE);
		}
	
		if ( Core_IsInternalRequest(pCore, pReq)&&(pReq->Org_Req) )
		{
			/* This internal request is used to request sense. */
			MV_ASSERT( pDevice->Device_Type&DEVICE_TYPE_ATAPI );
			pOrgReq = pReq->Org_Req;
			/* Copy sense from the scratch buffer to the request sense buffer. */
			MV_CopyMemory(
					pOrgReq->Sense_Info_Buffer,
					pReq->Data_Buffer,
					MV_MIN(pOrgReq->Sense_Info_Buffer_Length, pReq->Data_Transfer_Length)
					);
			pOrgReq->Scsi_Status = REQ_STATUS_HAS_SENSE;
#if defined(SUPPORT_ERROR_HANDLING)
			/* remove internal req's timer */
			hba_remove_timer(pReq);
#endif
			pReq = pOrgReq;
		}
		else
		{
			pReq->Scsi_Status = REQ_STATUS_SUCCESS;
		}

		CompleteRequestAndSlot(pCore, pPort, pReq, taskFiles, slotId);

		if ( completeSlot==0 )
			break;
	}
}

void SATA_PortHandleInterrupt(
	IN PCore_Driver_Extension pCore,
	IN PDomain_Port pPort
	)
{
	PDomain_Device pDevice = &pPort->Device[0];
	MV_LPVOID mmio = pCore->Mmio_Base;
	MV_LPVOID port_mmio = pPort->Mmio_Base;
	MV_U32 orgIntStatus, intStatus, serialError, commandIssue, serialActive, temp;
	PMV_Request pReq = NULL, pOrgReq = NULL;
	MV_U32 completeSlot = 0;
	MV_U8 slotId = 0, i;
	MV_BOOLEAN hasError = MV_FALSE, finalError = MV_FALSE;
	MV_U32 errorSlot = 0;
	ATA_TaskFile	taskFiles;
#ifdef MV_DEBUG
	MV_U32 orgCommandIssue, orgSerialActive, orgCompleteSlot, orgRunningSlot;
#endif

	/* Read port interrupt status register */
	orgIntStatus = MV_REG_READ_DWORD(port_mmio, PORT_IRQ_STAT);
	intStatus = orgIntStatus;

	if ( pPort->Setting&PORT_SETTING_NCQ_RUNNING )
	{
		serialActive = MV_REG_READ_DWORD(port_mmio, PORT_SCR_ACT);
		completeSlot =  (~serialActive) & pPort->Running_Slot;
	}
	else
	{
		commandIssue = MV_REG_READ_DWORD(port_mmio, PORT_CMD_ISSUE);
		completeSlot = (~commandIssue) & pPort->Running_Slot;
	}

	if (completeSlot) {
		volatile MV_U8 *D2H_FIS = (MV_U8 *)pPort->RX_FIS + 0x40;
		int d2h_try;
		/* if Device-to-Host FIS is not received, delay 10*50us */
		for (d2h_try = 0; d2h_try < 20; d2h_try++) {
			if (D2H_FIS[0] != 0x34) {
				//MV_ERROR_PRINT("D2H(%d):0x%x, \t", d2h_try, D2H_FIS[0]);
				HBA_DelayMicrosecond(pCore, 50);
				//MV_ERROR_PRINT("read D2H again:0x%x\n", D2H_FIS[0]);
			} else {
				break;
			}
		}
	}

#ifdef MV_DEBUG
	orgCommandIssue = commandIssue;
	orgSerialActive = serialActive;
	orgCompleteSlot = completeSlot;
	orgRunningSlot = pPort->Running_Slot;
#endif

	intStatus &= ~(PORT_IRQ_D2H_REG_FIS|PORT_IRQ_SDB_FIS|PORT_IRQ_PIO_DONE);	/* Used to check request is done. */
	intStatus &= ~(PORT_IRQ_DMAS_FIS|PORT_IRQ_PIOS_FIS); /* Needn't care. */

#ifdef GALOIS_ERROR_DEBUG
	dbg_intstatus = orgIntStatus;
	prev_dbg_cmdissue = dbg_cmdissue;
	dbg_cmdissue = MV_REG_READ_DWORD(port_mmio, PORT_CMD_ISSUE);
	prev_dbg_scract = dbg_scract;
	dbg_scract = MV_REG_READ_DWORD(port_mmio, PORT_SCR_ACT);
#endif

	/* Error handling */
	if ( (intStatus&PORT_IRQ_TF_ERR) 
			||	(intStatus&PORT_IRQ_LINK_RECEIVE_ERROR)
			||	(intStatus&PORT_IRQ_LINK_TRANSMIT_ERROR) )
	{
#ifdef GALOIS_ERROR_DEBUG
		MV_ERROR_PRINT("\nSATA_PortHandleInterrupt error(port=%d): intStatus=0x%x orgIntStatus=0x%x completeSlot=0x%x\n", 
			pPort->Id, intStatus, orgIntStatus, completeSlot);
#endif
		if (intStatus&PORT_IRQ_TF_ERR)
		{
#ifdef GALOIS_ERROR_DEBUG
			MV_ERROR_PRINT("SATA_PortHandleInterrupt: Task File Data is 0x%08x\n", MV_REG_READ_DWORD(port_mmio, PORT_TFDATA));
#endif
			/* Don't do error handling when receive link error. 
			 * Wait until we got the Task File Error */

			/* read serial error only when there is error */
			serialError = MV_REG_READ_DWORD(port_mmio, PORT_SCR_ERR);
			MV_REG_WRITE_DWORD(port_mmio, PORT_SCR_ERR, serialError);

			/* Handle serial error interrupt */
			if ( serialError )
			{
				SATA_HandleSerialError(pPort, serialError); 
			}

			/* read errorSlot only when there is error */
			errorSlot = MV_REG_READ_DWORD(port_mmio, PORT_CMD);
			errorSlot = (errorSlot>>8)&0x1F;
			hasError = MV_TRUE;

			if ( pPort->Setting&PORT_SETTING_DURING_RETRY )
				finalError = MV_TRUE;
			else
			{
				/* if the error request is any internal requests, we don't retry 
				 *     1) read log ext - don't retry
				 *	   2) any initialization requests such as identify - buffer
				 *		  will conflict when we try to send read log ext to retry
				 *	   3) request sense - included in the ATAPI condition below
				 */
				pReq = pPort->Running_Req[errorSlot];
				if ( (pReq != NULL) && Core_IsInternalRequest(pCore, pReq) )
					finalError = MV_TRUE;

				/* For ATAPI device, we don't do retry. OS already has done a lot.
				* ATAPI device: one request at a time. */
				if ( completeSlot==((MV_U32)1L<<errorSlot) )
				{
					pReq = pPort->Running_Req[errorSlot];
					MV_ASSERT( pReq!=NULL );
					pDevice = &pPort->Device[PATA_MapDeviceId(pReq->Device_Id)];
					if ( pDevice->Device_Type&DEVICE_TYPE_ATAPI )
						finalError = MV_TRUE;
				}
			}
		}
		intStatus &= 
			~(PORT_IRQ_TF_ERR|PORT_IRQ_LINK_RECEIVE_ERROR|PORT_IRQ_LINK_TRANSMIT_ERROR);		
	}


	/* Final Error: 
	 * 1. retry command, 2. internal command, or 3. all ATAPI command that has error.
	 * we give up this error request. Only one request is running. 
	 * And during retry we won't use NCQ command. */
	if ( finalError )
	{
		MV_ASSERT( !(pPort->Setting&PORT_SETTING_NCQ_RUNNING) );
		MV_ASSERT( completeSlot==((MV_U32)1L<<errorSlot) );
		MV_ASSERT( pPort->Running_Slot==completeSlot );

		/* clear global before channel */
		MV_REG_WRITE_DWORD(mmio, HOST_IRQ_STAT, (1L<<pPort->Id));
		MV_REG_WRITE_DWORD(port_mmio, PORT_IRQ_STAT, orgIntStatus);

		/* This is the failed request. */
		pReq = pPort->Running_Req[errorSlot];
		MV_ASSERT( pReq!=NULL );
		pDevice = &pPort->Device[PATA_MapDeviceId(pReq->Device_Id)];

		if( Core_IsInternalRequest(pCore, pReq) )
		{
			if( pReq->Org_Req )
			{
				/* This internal request is used to request sense. */
				MV_ASSERT( pDevice->Device_Type&DEVICE_TYPE_ATAPI );
				pOrgReq = pReq->Org_Req;
				pOrgReq->Scsi_Status = REQ_STATUS_ERROR;
#if defined(SUPPORT_ERROR_HANDLING)
				/* remove internal req's timer */
				hba_remove_timer(pReq);
#endif
				pReq = pOrgReq;
			}
			else if( pReq->Cdb[2] == CDB_CORE_READ_LOG_EXT )
			{
				pReq->Scsi_Status = REQ_STATUS_ERROR;
			}
			else
			{
				/* This internal request is initialization request like identify */
				MV_DASSERT( pDevice->State != DEVICE_STATE_INIT_DONE );
				pReq->Scsi_Status = REQ_STATUS_ERROR;
			}
		}
		else
		{
			if ( pReq->Cmd_Flag&CMD_FLAG_PACKET )
			{
				pReq->Scsi_Status = REQ_STATUS_REQUEST_SENSE;
			}
			else
			{
				MV_DPRINT(("Finally SATA error for Req 0x%x.\n", pReq->Cdb[0]));
				pReq->Scsi_Status = REQ_STATUS_ERROR;
			}
		}

		CompleteRequestAndSlot(pCore, pPort, pReq, &taskFiles, (MV_U8)errorSlot);

		/* Handle interrupt status register */
		if ( intStatus )
		{
			SATA_HandleHotplugInterrupt(pPort, intStatus);
		}
		return;
	}

	/* 
	 * The first time to hit the error. Under error condition, 
	 * figure out all the successful requests. */
	if ( hasError )
	{
		MV_ASSERT( !finalError );
		if ( pPort->Setting&PORT_SETTING_NCQ_RUNNING )
		{
			/* For NCQ command, if error happens on one slot.
			 * This slot is not completed. SActive is not cleared. */
		}
		else
		{
			/* For Non-NCQ command, last command is the error command. 
			 * ASIC will stop whenever there is an error.
			 * And we only have one request if there is no interrupt coalescing or NCQ. */
			MV_ASSERT( completeSlot==((MV_U32)1L<<errorSlot) );

			/* The error command is finished but we clear it to make it to be retried. */
			completeSlot=0;
		}
		/* Now all the completed commands are completed successfully. */

		/* Reset this port to prepare for the retry. At least one request will be retried. */

		MV_ASSERT( finalError==MV_FALSE );

		/* Toggle the port start bit to clear up the hardware to prepare for the retry. */
		temp = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_CMD);
		temp &= ~PORT_CMD_START;
		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_CMD, temp );
		HBA_DelayMillisecond(pCore, 1);
		temp |= PORT_CMD_START;
		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_CMD, temp );
		HBA_DelayMillisecond(pCore, 1);
		MV_PRINT("SATA_PortHandleInterrupt: Toggle CMD register start bit at port 0x%x.\n", pPort->Id);

		/* Toggle should before we clear the channel interrupt status but not the global interrupt. */
		MV_REG_WRITE_DWORD(mmio, HOST_IRQ_STAT, (1L<<pPort->Id));
		/* Abort all the others requests and retry. */
		for ( slotId=0; slotId<MAX_SLOT_NUMBER; slotId++ )
		{
			pReq = pPort->Running_Req[slotId];
			if ( !(completeSlot&(1L<<slotId)) && pReq )
			{
				pReq->Cmd_Flag &= 0xFF;	/* Remove NCQ setting. */
				pReq->Scsi_Status = REQ_STATUS_RETRY;

				/* Put requests to the queue head but don't run them. Should run ReadLogExt first. */
				pPort->Running_Req[slotId] = NULL;
				pPort->Running_Slot &= ~(1L<<slotId);
				Tag_ReleaseOne(&pPort->Tag_Pool, slotId);
#if defined(SUPPORT_ERROR_HANDLING)
				hba_remove_timer(pReq);
#endif
				List_Add(&pReq->Queue_Pointer, &pCore->Waiting_List);		/* Add to the header. */
				MV_PRINT("SATA_PortHandleInterrupt: Abort error requests....\n");
				MV_DumpRequest(pReq, MV_FALSE);
			}
		}
		MV_ASSERT( MV_REG_READ_DWORD(port_mmio, PORT_CMD_ISSUE)==0 );
		MV_ASSERT( MV_REG_READ_DWORD(port_mmio, PORT_IRQ_STAT)==0 );
		MV_ASSERT( (MV_REG_READ_DWORD(mmio, HOST_IRQ_STAT)&(1L<<pPort->Id))==0 );

		/* Send ReadLogExt command to clear the outstanding commands on the device. 
		 * This request will be put to the queue head because it's Cmd_Initiator is Core Driver. 
		 * Consider the port multiplier. */
		for ( i=0; i<MAX_DEVICE_PER_PORT; i++ )
		{
			pDevice = &pPort->Device[i];
			if ( !(pDevice->Device_Type&DEVICE_TYPE_ATAPI)
				&& (pDevice->Capacity&DEVICE_CAPACITY_READLOGEXT_SUPPORTED)
				//&& (pPort->Setting&PORT_SETTING_NCQ_RUNNING)
				)
			{
				Device_IssueReadLogExt(pPort, pDevice);
			}
			else
			{
				Core_HandleWaitingList(pCore);	//TBD: Should port based.
			}
		}

		/* Needn't run interrupt_handle_bottom_half except the hot plug.
		 * Toggle start bit will clear all the interrupt. So don't clear interrupt again. 
		 * Otherwise it'll clear Read Log Ext interrupt. 
		 * If Device_IssueReadLogExt is called, needn't run Core_HandleWaitingList. */
		//TBD: How about the hot plug.
		
		/* handle completed slots */
		if( completeSlot )
			mvCompleteSlots( pPort, completeSlot, &taskFiles );
		return;
	}

	/* 
	 * if no error
	 * clear global before channel
	 */
	MV_REG_WRITE_DWORD(mmio, HOST_IRQ_STAT, (1L<<pPort->Id));
	MV_REG_WRITE_DWORD(port_mmio, PORT_IRQ_STAT, orgIntStatus);

	/* handle completed slots */
	if( completeSlot )
		mvCompleteSlots( pPort, completeSlot, &taskFiles );

	/* Handle interrupt status register */
	if ( intStatus )
	{
		SATA_HandleHotplugInterrupt(pPort, intStatus);
	}
}

void Device_MakeRequestSenseRequest(
	IN PCore_Driver_Extension pCore,
	IN PDomain_Device pDevice,
	IN PMV_Request pNewReq,
	IN PMV_Request pOrgReq
	)
{
	PMV_SG_Table pSGTable = &pNewReq->SG_Table;
	MV_U8 senseSize = 18;

	MV_ZeroMvRequest(pNewReq);

	pNewReq->Device_Id = pDevice->Id;
	pNewReq->Scsi_Status = REQ_STATUS_PENDING;
	pNewReq->Cmd_Initiator = pCore;

	pNewReq->Data_Transfer_Length = senseSize;
	pNewReq->Data_Buffer = pDevice->Scratch_Buffer;

	pNewReq->Org_Req = pOrgReq;

	pNewReq->Cmd_Flag = CMD_FLAG_DATA_IN;
#ifdef USE_DMA_FOR_ALL_PACKET_COMMAND	
	pNewReq->Cmd_Flag |=CMD_FLAG_DMA;
#endif

	pNewReq->Completion = NULL;

	/* Make the SG table. */
	SGTable_Init(pSGTable, 0);
	SGTable_Append(
		pSGTable, 
		pDevice->Scratch_Buffer_DMA.low,
		pDevice->Scratch_Buffer_DMA.high,
		senseSize
		);
	MV_DASSERT( senseSize%2==0 );

	/* Request Sense request */
	pNewReq->Cdb[0]=SCSI_CMD_REQUEST_SENSE;
	pNewReq->Cdb[4]=senseSize;

	/* Fixed sense data format is 18 bytes. */
	MV_ZeroMemory(pNewReq->Data_Buffer, senseSize);
}

void CompleteRequest(
	IN PCore_Driver_Extension pCore,
	IN PMV_Request pReq,
	IN PATA_TaskFile pTaskFile
	)
{
	PDomain_Port pPort = &pCore->Ports[PATA_MapPortId(pReq->Device_Id)];
	PDomain_Device pDevice = &pPort->Device[PATA_MapDeviceId(pReq->Device_Id)];

	//TBD: Some of the command, we need read the received FIS like smart command.
#if (SUPPORT_ERROR_HANDLING)
	hba_remove_timer(pReq);
#endif /* defined(SUPPORT_ERROR_HANDLING) && defined(_OS_LINUX) */	

	if (pReq->Scsi_Status == REQ_STATUS_REQUEST_SENSE)
	{
		/* Use the internal request to request sense. */
		MV_ERROR_PRINT("[Berlin]Device_MakeRequestSenseRequest for prev command(port=%d):\n", pPort->Id);
		MV_ERROR_PRINT("\tCdb[%2x,%2x,%2x,%2x, %2x,%2x,%2x,%2x, %2x,%2x,%2x,%2x].\n",
				pReq->Cdb[0], pReq->Cdb[1], pReq->Cdb[2], pReq->Cdb[3], 
				pReq->Cdb[4], pReq->Cdb[5], pReq->Cdb[6], pReq->Cdb[7],
				pReq->Cdb[8], pReq->Cdb[9], pReq->Cdb[10], pReq->Cdb[11]
		);

		Device_MakeRequestSenseRequest(pCore, pDevice, pDevice->Internal_Req, pReq);
		/* pReq is linked to the */
		Core_ModuleSendRequest(pCore, pDevice->Internal_Req);

		return;
	}

	/* Do something if necessary to return back the request. */
	if ( (pReq->Cdb[0]==SCSI_CMD_MARVELL_SPECIFIC) && (pReq->Cdb[1]==CDB_CORE_MODULE) ) 
	{
		if ( pReq->Cdb[2]==CDB_CORE_SHUTDOWN )
		{
			if ( pReq->Device_Id<MAX_DEVICE_NUMBER-1 )
			{
				pReq->Device_Id++;
				pReq->Scsi_Status = REQ_STATUS_PENDING;
				Core_ModuleSendRequest(pCore, pReq);
				return;
			}
			else
			{
				pReq->Scsi_Status = REQ_STATUS_SUCCESS;
			}
		}
	}

	if (pReq->Completion)
		pReq->Completion(pReq->Cmd_Initiator, pReq);
#if 0
	MV_PRINT("Losed req num %d\n", pDevice->Outstanding_Req);
	if (pReq->Cdb[0] == 0x1b) {
		MV_DumpRequest(pReq, MV_TRUE);
		MV_PRINT("0x%08x\n", pReq->Scsi_Status);
	}
#endif
}

void CompleteRequestAndSlot(
	IN PCore_Driver_Extension pCore,
	IN PDomain_Port pPort,
	IN PMV_Request pReq,
	IN PATA_TaskFile pTaskFile,
	IN MV_U8 slotId
	)
{
#ifdef SUPPORT_ERROR_HANDLING
	PDomain_Device pDevice = &pPort->Device[PATA_MapDeviceId(pReq->Device_Id)];
#endif

	/* Release running slot */
	pPort->Running_Req[slotId] = NULL;
	pPort->Running_Slot &= ~(1L<<slotId);
	Tag_ReleaseOne(&pPort->Tag_Pool, slotId);
	MV_DASSERT( (MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_CMD_ISSUE)&(1<<slotId))==0 );

	if ( pPort->Type!=PORT_TYPE_PATA )
	{
		MV_DASSERT( (MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_SCR_ACT)&(1<<slotId))==0 );
	}

#ifdef SUPPORT_ERROR_HANDLING
	pDevice->Outstanding_Req--;
#endif /* SUPPORT_ERROR_HANDLING */
	CompleteRequest(pCore, pReq, pTaskFile);
}

void Port_Monitor(PDomain_Port pPort)
{
	struct scsi_cmnd *pSCmd;
	MV_U8 i;
	
	MV_PRINT("[Galois] Port_Monitor: Running_Slot=0x%08x.\n", pPort->Running_Slot);
	for ( i=0; i<MAX_SLOT_NUMBER; i++ )
	{
		if ( pPort->Running_Req[i]!=NULL ) {
			PMV_Command_Header pHeader= SATA_GetCommandHeader(pPort, i);
			PMV_Command_Table pCmdTable = Port_GetCommandTable(pPort, i);
			PSATA_FIS_REG_H2D pFIS = (PSATA_FIS_REG_H2D)pCmdTable->FIS;

			MV_PRINT("Hang slot %d: \n", i);
			MV_DumpRequest(pPort->Running_Req[i], MV_TRUE);
			MV_DumpHeader(pHeader);
			MV_DumpFIS(pFIS);
			MV_DumpSGTable(&(pPort->Running_Req[i]->SG_Table));

			pSCmd = (struct scsi_cmnd *)pPort->Running_Req[i]->Org_Req;
			if (pSCmd) {
				MV_PRINT("pSCmd->cmnd[%2x,%2x,%2x,%2x, %2x,%2x,%2x,%2x, %2x,%2x,%2x,%2x, %2x,%2x,%2x,%2x].\n", 
						pSCmd->cmnd[0],
						pSCmd->cmnd[1],
						pSCmd->cmnd[2],
						pSCmd->cmnd[3],
						pSCmd->cmnd[4],
						pSCmd->cmnd[5],
						pSCmd->cmnd[6],
						pSCmd->cmnd[7],
						pSCmd->cmnd[8],
						pSCmd->cmnd[9],
						pSCmd->cmnd[10],
						pSCmd->cmnd[11],
						pSCmd->cmnd[12],
						pSCmd->cmnd[13],
						pSCmd->cmnd[14],
						pSCmd->cmnd[15]
				);
			}
			MV_PRINT("\n");
		}
	}
}

void Core_ModuleMonitor(MV_PVOID This)
{
	PCore_Driver_Extension pCore = (PCore_Driver_Extension)This;
	PMV_Request pReq = NULL;
	PList_Head head = &pCore->Waiting_List;
	PDomain_Port pPort = NULL;
	MV_U8 i;

	//TBD: typeof
	MV_PRINT("[Hang commands]Core_ModuleMonitor Waiting_List:\n");
	for (pReq = LIST_ENTRY((head)->next, MV_Request, Queue_Pointer);	\
	     &pReq->Queue_Pointer != (head); 	\
	     pReq = LIST_ENTRY(pReq->Queue_Pointer.next, MV_Request, Queue_Pointer))
	{
		MV_DumpRequest(pReq, MV_FALSE);
		MV_DumpSGTable(&pReq->SG_Table);
	}

	MV_PRINT("\n");
	for ( i=0; i<pCore->Port_Num; i++ )
	{
		MV_PRINT("Port[%d]:\n", i);
		pPort = &pCore->Ports[i];
		Port_Monitor(pPort);
	}
	MV_PRINT("\n\n");
}

void Core_ModuleReset(MV_PVOID This)
{
	MV_U32 extensionSize = 0; 

	extensionSize = ROUNDING(sizeof(Core_Driver_Extension),8);
			
	/* Re-initialize all the variables even discard all the requests. */
	Core_ModuleInitialize(This, extensionSize, 32);

	//TBD: Merge with Core_ResetHardware
#if 0
	{
		PCore_Driver_Extension pCore = (PCore_Driver_Extension)This;

		pCore->Adapter_State = ADAPTER_INITIALIZING;
		for ( i=0; i<MAX_PORT_NUMBER; i++ )
		{
			pPort = &pCore->Ports[i];
			pPort->Port_State = PORT_STATE_IDLE;
			for ( j=0; j<MAX_DEVICE_PER_PORT; j++ )
			{
				pDevice = &pPort->Device[j];
				pDevice->State = DEVICE_STATE_IDLE;
			}
		}
	}
#endif 
}

